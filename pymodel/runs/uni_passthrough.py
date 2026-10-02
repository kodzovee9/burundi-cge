"""How much of the unification result rests on the premium's pass-through
to import prices?

    python3 runs/uni_passthrough.py                  # theta = 1, .75, .5, .25, 0
    python3 runs/uni_passthrough.py --thetas 1,0.5   # a subset
    python3 runs/uni_passthrough.py --scenario uni+inf+hd

The 2019-24 backcast matches Burundi's real imports only if the rise in the
parallel premium is kept out of import prices (APPENDIX.md, A.6.3
and Table A.11). `uni` removes the premium, and its gains run through exactly that
channel: cheaper imports for the ~70% of imports bought at the parallel rate.
This runner re-solves `uni` with only a share theta of the premium CHANGE
passed into import costs (`model.py`, `cal.prexr_pt`), measured from the
reference path, and reports 2026-2040 average growth and the increment over
the base run for each theta. theta = 1 is the model as published and must
reproduce the shipped `uni` column exactly.

Writes reports/uni-passthrough.md and .csv. About five minutes.
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
sys.path.insert(0, HERE)

from gemcore.database import load_database, bdi2019_data2          # noqa: E402
from gemcore.calibration import calibrate                          # noqa: E402
from gemcore.dynamics import run_base, run_reference, par_redefn_0  # noqa: E402
from gemcore import scenarios as scen_mod                          # noqa: E402
from run import DATA, GAMS, ORDER, growth                          # noqa: E402

OUTDIR = os.path.join(ROOT, "reports")


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--thetas", default="1,0.75,0.5,0.25,0")
    ap.add_argument("--scenario", default="uni")
    ap.add_argument("--gams-replication", action="store_true",
                    help="drop the fuel-shortage mechanisms (gemcore/fuel.py) to "
                         "re-check the port against the reference application")
    args = ap.parse_args()
    thetas = [float(x) for x in args.thetas.split(",")]
    t0 = time.time()

    cal = calibrate(load_database(DATA, data2_hook=bdi2019_data2),
                    **({"fuel_mech": None} if args.gams_replication else {}))
    per = [t for t in cal.db.sets["tsol"] if int(t) <= 2040]
    pass1 = run_base(cal, dcal01=True, verbose=False, periods=per)
    ref = run_reference(cal, pass1, verbose=False, periods=per)
    par_redefn_0(cal, ref, verbose=False)
    print(f"base + reference done ({time.time() - t0:.0f}s)", flush=True)
    base = growth(cal, ref)
    anchor = dict(cal.PREXR0)               # the reference premium path
    snap = scen_mod._snapshot(cal)

    cols, prem = {}, {}
    for th in thetas:
        scen_mod._restore(cal, snap)
        cal.prexr_pt = None if th == 1.0 else th
        cal.prexr_pt_anchor = anchor
        sol = scen_mod.run_scenario(cal, scen_mod.SCENARIOS[args.scenario](cal, per),
                                    ref, per, verbose=False)
        cols[th] = growth(cal, sol)
        prem[th] = {t: anchor[t] + th * (sol[t]["PREXR"][t] - anchor[t])
                    for t in ("2025", "2027", "2040")}
        scen_mod._restore(cal, snap)
        cal.prexr_pt = None
        print(f"theta {th:g} done ({time.time() - t0:.0f}s)", flush=True)

    shipped = GAMS.get(args.scenario, {}) if args.gams_replication else {}
    os.makedirs(OUTDIR, exist_ok=True)
    stem = "uni-passthrough" if args.scenario == "uni" else f"{args.scenario.replace('+', '-')}-passthrough"
    with open(os.path.join(OUTDIR, stem + ".csv"), "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["indicator", "base"] + [f"theta={th:g}" for th in thetas]
                   + [f"increment theta={th:g}" for th in thetas])
        for k in ORDER:
            w.writerow([k, f"{base[k]:.3f}"] + [f"{cols[th][k]:.3f}" for th in thetas]
                       + [f"{cols[th][k] - base[k]:.3f}" for th in thetas])
    with open(os.path.join(OUTDIR, stem + ".md"), "w") as fh:
        fh.write(f"# `{args.scenario}`: sensitivity to the premium's pass-through to import prices\n\n")
        fh.write("Average annual growth 2026-2040 (%). theta is the share of a change in "
                 "the parallel premium, measured from the reference path, that reaches "
                 "the prices of imports bought at the parallel rate (and the premium "
                 "rent and tariff base valued at it). theta = 1 is GEM-Core's assumption.\n\n")
        fh.write("| indicator | base | " + " | ".join(f"θ={th:g}" for th in thetas) + " |\n")
        fh.write("| --- | ---: |" + " ---: |" * len(thetas) + "\n")
        for k in ORDER:
            fh.write(f"| {k} | {base[k]:.3f} | " + " | ".join(f"{cols[th][k]:.3f}" for th in thetas) + " |\n")
        fh.write("\n## Increment over the base run (percentage points a year)\n\n")
        fh.write("| indicator | " + " | ".join(f"θ={th:g}" for th in thetas)
                 + (" | GAMS θ=1 |" if shipped else " |") + "\n")
        fh.write("| --- |" + " ---: |" * (len(thetas) + (1 if shipped else 0)) + "\n")
        for k in ORDER:
            cells = [f"{cols[th][k] - base[k]:+.3f}" for th in thetas]
            if shipped:
                cells.append(f"{shipped[k] - GAMS['base'][k]:+.3f}")
            fh.write(f"| {k} | " + " | ".join(cells) + " |\n")
        fh.write("\n## Premium on import costs (effective ratio to the official rate)\n\n")
        fh.write("| year | reference | " + " | ".join(f"θ={th:g}" for th in thetas) + " |\n")
        fh.write("| --- | ---: |" + " ---: |" * len(thetas) + "\n")
        for t in ("2025", "2027", "2040"):
            fh.write(f"| {t} | {anchor[t]:.3f} | " + " | ".join(f"{prem[th][t]:.3f}" for th in thetas) + " |\n")

    print(f"\n=== {args.scenario}: increment over base, 2026-2040 (pp/yr)   [{time.time() - t0:.0f}s]")
    print(f"{'indicator':<12}" + "".join(f"{'θ=' + format(th, 'g'):>10}" for th in thetas))
    for k in ORDER:
        print(f"{k:<12}" + "".join(f"{cols[th][k] - base[k]:>10.3f}" for th in thetas))
    print(f"\nwrote reports/{stem}.md and .csv")


if __name__ == "__main__":
    main()
