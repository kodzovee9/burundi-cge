"""Write the full solution of the base run and of every validated scenario.

    python3 runs/dump.py                 # base + reference + the four validated scenarios
    python3 runs/dump.py uni combi       # a subset
    python3 runs/dump.py --to 2030       # shorter horizon

One CSV per run in `reports/solutions/`, long format:

    variable, index, year, value

where `index` is the variable's set index joined with "|" (empty for scalars).
Every variable in the model state is written for every solved year, so the
files are large (about 90,000 rows per run) but complete: any indicator can
be rebuilt from them without re-solving. Levels are in model units (the SAM's,
10 bn BIF); the published GAMS tables divide by `samsol/samrep = 10`.

`reports/solutions/macro-levels.csv` collects the real macro aggregates
(the eight indicators of the growth table) by run and year for convenience.
"""

from __future__ import annotations

import argparse
import csv
import os
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, ROOT)

from gemcore.database import load_database, bdi2019_data2          # noqa: E402
from gemcore.calibration import calibrate                          # noqa: E402
from gemcore.dynamics import (run_base, run_reference,              # noqa: E402
                              par_redefn_0)
from gemcore import scenarios as scen_mod                          # noqa: E402
from gemcore.complementarity import sweep_report                   # noqa: E402
from run import DATA, ORDER, real_aggregates                       # noqa: E402
from all_scenarios import VALIDATED                                # noqa: E402

OUTDIR = os.path.join(ROOT, "reports", "solutions")


def write_solution(path, sols):
    """`sols`: {year: {variable: value | {index: value}}} -> long CSV."""
    n = 0
    with open(path, "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["variable", "index", "year", "value"])
        for t in sorted(sols):
            for var in sorted(sols[t]):
                val = sols[t][var]
                if isinstance(val, dict):
                    for key in sorted(val, key=lambda k: str(k)):
                        idx = "|".join(key) if isinstance(key, tuple) else str(key)
                        w.writerow([var, idx, t, repr(float(val[key]))])
                        n += 1
                else:
                    try:
                        w.writerow([var, "", t, repr(float(val))])
                        n += 1
                    except (TypeError, ValueError):
                        continue
    return n


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("scenarios", nargs="*", default=None,
                    help=f"scenarios to dump (default: {', '.join(VALIDATED)})")
    ap.add_argument("--to", type=int, default=2040)
    ap.add_argument("--gams-replication", action="store_true",
                    help="drop the fuel-shortage mechanisms (gemcore/fuel.py) to "
                         "re-check the port against the published GAMS tables")
    args = ap.parse_args()
    names = args.scenarios or VALIDATED
    unknown = [n for n in names if n not in scen_mod.SCENARIOS]
    if unknown:
        ap.error(f"unknown scenario(s) {unknown}; available: "
                 f"{', '.join(sorted(scen_mod.SCENARIOS))}")

    t0 = time.time()
    os.makedirs(OUTDIR, exist_ok=True)
    cal = calibrate(load_database(DATA, data2_hook=bdi2019_data2),
                    **({"fuel_mech": None} if args.gams_replication else {}))
    per = [t for t in cal.db.sets["tsol"] if int(t) <= args.to]
    print(f"periods {per[0]}..{per[-1]}", flush=True)

    pass1 = run_base(cal, dcal01=True, verbose=False, periods=per)
    ref = run_reference(cal, pass1, verbose=False, periods=per)
    par_redefn_0(cal, ref, verbose=False)
    print(f"base + reference done ({time.time()-t0:.0f}s)", flush=True)

    runs = {"base-pass1": pass1, "base": ref}
    branches = {}
    snap = scen_mod._snapshot(cal)
    for name in names:
        scen_mod._restore(cal, snap)
        sol = scen_mod.run_scenario(cal, scen_mod.SCENARIOS[name](cal, per),
                                    ref, per, verbose=False)
        branches[name] = sweep_report(cal, per)
        scen_mod._restore(cal, snap)
        runs[name] = sol
        print(f"{name} done ({time.time()-t0:.0f}s)", flush=True)

    for name, sols in runs.items():
        fname = name.replace("+", "-") + ".csv"
        n = write_solution(os.path.join(OUTDIR, fname), sols)
        print(f"wrote {fname}: {n} rows")

    # real macro aggregates by run and year
    mpath = os.path.join(OUTDIR, "macro-levels.csv")
    with open(mpath, "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["run", "year"] + ORDER)
        for name, sols in runs.items():
            for t in per:
                ra = real_aggregates(cal, sols[t], t)
                w.writerow([name, t] + [f"{ra[k]:.6f}" for k in ORDER])
    print(f"wrote macro-levels.csv")

    # which complementarity branch each scenario ended on
    bpath = os.path.join(OUTDIR, "branches.md")
    with open(bpath, "w") as fh:
        fh.write("# Complementarity branches by scenario\n\n")
        fh.write("Cells listed under *quota slack* import below the ceiling with "
                 "zero rent; cells under *idle branch* have the resource rent at "
                 "its reservation level and utilisation free. Anything not "
                 "listed is on the binding / full-employment branch.\n\n")
        for name, rep in branches.items():
            fh.write(f"## {name}\n\n")
            for k, v in rep.items():
                fh.write(f"- {k}: `{v}`\n")
            fh.write("\n")
    print(f"wrote branches.md   [{time.time()-t0:.0f}s]")


if __name__ == "__main__":
    main()
