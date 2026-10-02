"""Solve the model and print the macro table, for the base run or any scenario.

    python3 runs/run.py                 # base + reference only
    python3 runs/run.py uni             # ... plus the uni counterfactual
    python3 runs/run.py uni+inf --to 2030 --quiet

Every run does the base (SOLUCION 1) and reference (SOLUCION 2) passes first,
because a scenario is defined as a deviation from the reference, not from the
raw calibration. That costs about a minute; there is no way to skip it and still
get a meaningful counterfactual.

Output is average annual growth 2026-2040 by macro aggregate. With
--gams-replication, and when data/gams-reference.json is present, it is shown
next to the reference application's GAMS results.
"""

from __future__ import annotations

import argparse
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

DATA = os.path.join(ROOT, "..", "model", "user-files", "bdi2019",
                    "bdi2019-data.xlsx")

# The reference application's results (GEM-Core in GAMS, for internal World Bank
# use) are kept in data/gams-reference.json, which the public repository does
# not carry. Without it the comparison columns are simply left out.
GAMS_REF = os.path.join(ROOT, "data", "gams-reference.json")


def load_gams_reference():
    if not os.path.exists(GAMS_REF):
        return {}
    import json
    with open(GAMS_REF) as fh:
        return json.load(fh)


GAMS = {k: v for k, v in load_gams_reference().get("macro_growth_2026_2040", {}).items()
        if not k.startswith("_")}

ORDER = ["Absorption", "PrvCon", "GovCon", "PrvFixInv", "GovFixInv",
         "Exports", "Imports", "GDPFC"]


def real_aggregates(cal, V, t):
    """Macro aggregates at base-year prices, matching repspec.gms's definitions.

    Everything is valued at the BASE-YEAR price vector (`PQD00`, `PK00`,
    `PWE00`, `PWM00`), which is what makes these real volume indices rather than
    nominal values -- the same convention as the reference application's tables.
    """
    S = cal.db.sets
    C, H = S["c"], S["h"]
    G, IN2, NGO, DSTK = S["insgov"], S["ins2"], S["insngo"], S["dstk"]
    FCAPNG, FCAPG = S["fcapng"], S["fcapg"]
    P0, PK0 = cal.PQD00, cal.PK00
    gi = cal.gdpindex[t]

    def q(nm, *k):
        return V.get(nm, {}).get(k if len(k) > 1 else k[0], 0.0)

    prv = (sum(P0.get((c, h), 0.0) * q("QH", c, h) for c in C for h in H)
           + sum(P0.get((c, ng), 0.0) * q("QNGO", c, ng)
                 for c in C for ng in NGO))
    pfi = sum(PK0.get(fc, 0.0) * q("DKINS", i2, fc)
              for i2 in IN2 for fc in FCAPNG)
    gfi = sum(PK0.get(fc, 0.0) * q("DKINS", i2, fc)
              for i2 in IN2 for fc in FCAPG)
    stk = sum(P0.get((c, d), 0.0)
              * sum(cal.qdstk00.get((c, i2), 0.0) * gi for i2 in IN2)
              for c in C for d in DSTK)
    gov = sum(P0.get((c, g), 0.0) * q("QG", c) for c in C for g in G)
    exp = sum(cal.PWE00.get(c, 0.0) * cal.EXR00 * q("QE", c) for c in C)
    imp = -sum(cal.PWM00.get(c, 0.0) * cal.EXR00 * q("QM", c) for c in C)
    return {"Absorption": prv + pfi + gfi + stk + gov, "PrvCon": prv,
            "GovCon": gov, "PrvFixInv": pfi, "GovFixInv": gfi,
            "Exports": exp, "Imports": imp, "GDPFC": V["RGDPFC"][t]}


def growth(cal, sols, a="2026", b="2040"):
    if a not in sols or b not in sols:
        return None
    n = int(b) - int(a)
    ra, rb = real_aggregates(cal, sols[a], a), real_aggregates(cal, sols[b], b)
    return {k: 100 * ((rb[k] / ra[k]) ** (1.0 / n) - 1) for k in ra}


def table(rows):
    """rows: list of (label, growth-dict or None, gams-dict or None)."""
    hdr = f"{'indicator':<12}"
    for label, _, gams in rows:
        hdr += f"{label:>13}" + (f"{'GAMS':>8}{'diff':>8}" if gams else "")
    print(hdr)
    for k in ORDER:
        line = f"{k:<12}"
        for _, g, gams in rows:
            if g is None:
                line += f"{'-':>13}"
                continue
            line += f"{g[k]:>13.3f}"
            if gams:
                line += f"{gams[k]:>8.3f}{g[k] - gams[k]:>8.3f}"
        print(line)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("scenario", nargs="?", default=None,
                    help=f"one of: {', '.join(sorted(scen_mod.SCENARIOS))}")
    ap.add_argument("--to", type=int, default=2040,
                    help="last period to solve (default 2040)")
    ap.add_argument("--quiet", action="store_true",
                    help="suppress the per-period solve log")
    ap.add_argument("--gams-replication", action="store_true",
                    help="drop the fuel-shortage mechanisms (gemcore/fuel.py) to "
                         "re-check the port against the reference application")
    args = ap.parse_args()

    if args.scenario and args.scenario not in scen_mod.SCENARIOS:
        ap.error(f"unknown scenario {args.scenario!r}; available: "
                 f"{', '.join(sorted(scen_mod.SCENARIOS))}")

    if args.scenario in getattr(scen_mod, "UNVALIDATED", set()):
        print(f"!! WARNING: {args.scenario!r} is listed in scenarios.UNVALIDATED: "
              f"its results have not been checked against the reference application.\n"
              f"!! See APPENDIX.md, A.6.2.\n", file=sys.stderr, flush=True)

    verbose = not args.quiet
    t0 = time.time()
    cal = calibrate(load_database(DATA, data2_hook=bdi2019_data2),
                    **({"fuel_mech": None} if args.gams_replication else {}))
    per = [t for t in cal.db.sets["tsol"] if int(t) <= args.to]
    print(f"periods {per[0]}..{per[-1]}", flush=True)

    pass1 = run_base(cal, dcal01=True, verbose=verbose, periods=per)
    ref = run_reference(cal, pass1, verbose=verbose, periods=per)
    par_redefn_0(cal, ref, verbose=verbose)

    cmp = GAMS if args.gams_replication else {}
    rows = [("base", growth(cal, ref), cmp.get("base"))]
    if args.scenario:
        snap = scen_mod._snapshot(cal)
        build = scen_mod.SCENARIOS[args.scenario]
        sol = scen_mod.run_scenario(cal, build(cal, per), ref, per,
                                    verbose=verbose)
        scen_mod._restore(cal, snap)
        rows.append((args.scenario, growth(cal, sol),
                     cmp.get(args.scenario)))

    print(f"\n=== average annual growth 2026-2040 (%)   [{time.time()-t0:.0f}s]")
    if rows[0][1] is None:
        print("  (need --to 2040 for the growth table; solved levels only)")
        return
    table(rows)


if __name__ == "__main__":
    main()
