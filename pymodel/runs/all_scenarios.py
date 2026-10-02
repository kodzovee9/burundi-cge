"""Produce the final results table for the validated scenarios.

    python3 runs/all_scenarios.py

Solves the base and reference passes once, then each validated scenario, and
writes the macro table to `reports/` as CSV and Markdown alongside the published
GAMS numbers. Roughly three minutes.

`combi+` is not yet included (not implemented). `combi` runs on the
idle-resource closure -- OUR combi, "the endowment triples", not GAMS's "use
triples" -- see APPENDIX.md, A.5.7.
"""

from __future__ import annotations

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
from run import DATA, GAMS, ORDER, growth                          # noqa: E402

VALIDATED = ["uni", "uni+inf", "uni+inf+hd", "combi"]
OUTDIR = os.path.join(ROOT, "reports")


def main():
    import argparse
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--gams-replication", action="store_true",
                    help="drop the fuel-shortage mechanisms (gemcore/fuel.py) to "
                         "re-check the port against the reference application")
    args = ap.parse_args()
    t0 = time.time()
    os.makedirs(OUTDIR, exist_ok=True)
    cal = calibrate(load_database(DATA, data2_hook=bdi2019_data2),
                    **({"fuel_mech": None} if args.gams_replication else {}))
    per = [t for t in cal.db.sets["tsol"] if int(t) <= 2040]
    print(f"periods {per[0]}..{per[-1]}", flush=True)

    pass1 = run_base(cal, dcal01=True, verbose=False, periods=per)
    ref = run_reference(cal, pass1, verbose=False, periods=per)
    par_redefn_0(cal, ref, verbose=False)
    print(f"base + reference done ({time.time()-t0:.0f}s)", flush=True)

    cols = {"base": growth(cal, ref)}
    snap = scen_mod._snapshot(cal)
    for name in VALIDATED:
        scen_mod._restore(cal, snap)
        build = scen_mod.SCENARIOS[name]
        sol = scen_mod.run_scenario(cal, build(cal, per), ref, per,
                                    verbose=False)
        scen_mod._restore(cal, snap)
        cols[name] = growth(cal, sol)
        print(f"{name} done ({time.time()-t0:.0f}s)", flush=True)

    names = ["base"] + VALIDATED
    # the comparison with GAMS: replication mode only, and only when the
    # reference application's results (data/gams-reference.json) are present
    cmp = args.gams_replication and all(n in GAMS for n in names)
    stem = "macro-growth-2026-2040" + ("-gams-replication" if args.gams_replication else "")
    csv_path = os.path.join(OUTDIR, stem + ".csv")
    with open(csv_path, "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["indicator"] + [f"{n} (py)" for n in names]
                   + ([f"{n} (GAMS)" for n in names] + [f"{n} (diff)" for n in names] if cmp else []))
        for k in ORDER:
            w.writerow([k] + [f"{cols[n][k]:.3f}" for n in names]
                       + ([f"{GAMS[n][k]:.3f}" for n in names]
                          + [f"{cols[n][k] - GAMS[n][k]:.3f}" for n in names] if cmp else []))

    md_path = os.path.join(OUTDIR, stem + ".md")
    with open(md_path, "w") as fh:
        fh.write("# Average annual growth 2026-2040 (%)\n\n")
        if cmp:
            fh.write("Replication mode against the reference application (GAMS, "
                     "`data/gams-reference.json`, Table D.2).\n\n")
            fh.write("| indicator | " + " | ".join(
                f"{n} py | {n} GAMS | diff" for n in names) + " |\n")
            fh.write("| --- |" + " ---: |" * (3 * len(names)) + "\n")
        else:
            fh.write(("Replication mode (this model's additions switched off).\n\n"
                      if args.gams_replication else "Full model.\n\n"))
            fh.write("| indicator | " + " | ".join(names) + " |\n")
            fh.write("| --- |" + " ---: |" * len(names) + "\n")
        for k in ORDER:
            cells = []
            for n in names:
                cells += ([f"{cols[n][k]:.3f}", f"{GAMS[n][k]:.3f}",
                           f"{cols[n][k] - GAMS[n][k]:+.3f}"] if cmp else [f"{cols[n][k]:.3f}"])
            fh.write(f"| {k} | " + " | ".join(cells) + " |\n")
        fh.write("\n`combi` is on the idle-resource closure (endowment triples; "
                 "use follows demand at the reference rent). `combi+` not yet "
                 "implemented. See APPENDIX.md, A.5.7.\n")

    print(f"\n=== average annual growth 2026-2040 (%)   [{time.time()-t0:.0f}s]")
    hdr = f"{'indicator':<12}"
    for n in names:
        hdr += f"{n:>13}" + (f"{'GAMS':>8}{'diff':>8}" if cmp else "")
    print(hdr)
    for k in ORDER:
        line = f"{k:<12}"
        for n in names:
            line += f"{cols[n][k]:>13.3f}"
            if cmp:
                line += f"{GAMS[n][k]:>8.3f}{cols[n][k]-GAMS[n][k]:>8.3f}"
        print(line)
    print(f"\nwrote {csv_path}")
    print(f"wrote {md_path}")


if __name__ == "__main__":
    main()
