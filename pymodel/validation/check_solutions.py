"""Audit every solved year of every scenario, independently of the solver.

    python validation/check_solutions.py                    # the full model
    python validation/check_solutions.py --gams-replication # the reference application

For the base run and each scenario it re-evaluates every model equation at the
solution of every year 2019-2040 and checks:

- every equation holds (largest residual below 1e-6);
- Walras' law: the slack in the saving-investment balance is zero;
- the import quotas are on a consistent branch: a binding quota has a rent
  rate of zero or more; a slack quota has imports at or below the ceiling and
  no rent;
- informal fuel: never negative, and when it is off, the market price of fuel
  is not above the informal price;
- the idle mining resource: utilisation never above 100 %;
- no price or quantity has been pushed to the solver's lower bound.

Writes reports/SOLUTION-AUDIT[-gams-replication].md and prints AUDIT: PASS or
AUDIT: FAIL.
"""

from __future__ import annotations

import os
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, ROOT)
sys.path.insert(0, os.path.join(ROOT, "runs"))

from gemcore.database import load_database, bdi2019_data2          # noqa: E402
from gemcore.calibration import calibrate                          # noqa: E402
from gemcore.dynamics import run_base, run_reference, par_redefn_0  # noqa: E402
from gemcore.state import Closure                                  # noqa: E402
from gemcore.model import residuals                                # noqa: E402
from gemcore.solver import POSITIVE_FAMILIES                       # noqa: E402
from gemcore import scenarios as scen_mod                          # noqa: E402
from gemcore import fuel as fuel_mod                               # noqa: E402
from run import DATA                                               # noqa: E402

FULL = ["uni", "uni+bs", "uni+bs-inv", "uni+inf", "uni+inf-m", "uni+inf-x",
        "uni+inf+hd", "uni+inf+hd-x", "combi", "combi-x"]
REPL = ["uni", "uni+inf", "uni+inf+hd", "combi"]
TOL_RES, TOL_WALRAS = 1e-6, 1e-6


def audit(cal, sols, per):
    """Return (max residual, max |WALRAS|, list of problems)."""
    S = cal.db.sets
    problems, worst, walras = [], 0.0, 0.0
    fcfg = getattr(cal, "fuel", None)
    for i, t in enumerate(per):
        V = sols[t]
        L = sols[per[i - 1]] if i else {}
        cal._solve_t = t
        R = residuals(V, cal, t, L, Closure(cal, dcal01=False))
        r = max(abs(x) for x in R.values() if x is not None)
        worst = max(worst, r)
        if r > TOL_RES:
            k = max(R, key=lambda k: abs(R[k]) if R[k] is not None else 0)
            problems.append(f"{t}: residual {r:.1e} in {k}")
        w = abs(V["WALRAS"][t])
        walras = max(walras, w)
        if w > TOL_WALRAS:
            problems.append(f"{t}: Walras slack {w:.1e}")
        # quotas
        for c in S["cmbar"]:
            rent = V["PRQMBAR"].get(c, 0.0)
            if (c, t) in getattr(cal, "quota_slack", set()):
                ceil = fuel_mod.quota_ceiling(cal, V, c, t)
                if ceil is not None and V["QM"][c] > ceil * (1 + 1e-6):
                    problems.append(f"{t}: slack quota {c} imports above the ceiling")
                if abs(rent) > 1e-8:
                    problems.append(f"{t}: slack quota {c} has rent {rent:.2e}")
            elif rent < -1e-8:
                problems.append(f"{t}: binding quota {c} has negative rent {rent:.2e}")
        # informal fuel
        if fcfg and fcfg.informal:
            f = fcfg.fuel
            qmi = V["QMI"].get(f, 0.0)
            if qmi < -1e-9 * cal.QM00[f]:
                problems.append(f"{t}: informal fuel negative ({qmi:.2e})")
            if (f, t) not in getattr(cal, "inf_active", set()) and \
                    fuel_mod.informal_gap(cal, V, t) > 1e-6 * V["PM"][f]:
                problems.append(f"{t}: informal fuel off although its price is below the market price")
        # idle resource
        for (fac, tt) in getattr(cal, "idle_cells", set()):
            if tt == t and V["UERAT"].get(fac, 0.0) < -1e-8:
                problems.append(f"{t}: idle resource {fac} over-used")
        # lower bounds
        for fam in POSITIVE_FAMILIES:
            for k, x in (V.get(fam) or {}).items():
                if isinstance(x, float) and 0 < x <= 1e-8:
                    problems.append(f"{t}: {fam}{k} at the solver's lower bound")
    return worst, walras, problems


def main():
    repl = "--gams-replication" in sys.argv
    t0 = time.time()
    cal = calibrate(load_database(DATA, data2_hook=bdi2019_data2),
                    **({"fuel_mech": None} if repl else {}))
    per = [t for t in cal.db.sets["tsol"] if int(t) <= 2040]
    p1 = run_base(cal, dcal01=True, verbose=False, periods=per)
    ref = run_reference(cal, p1, verbose=False, periods=per)
    # audit the reference path against the parameters it was solved with,
    # before par_redefn_0 re-bases them for the scenarios
    rows = [("base (reference path)",) + audit(cal, ref, per)]
    par_redefn_0(cal, ref, verbose=False)
    print(f"base audited ({time.time() - t0:.0f} s)", flush=True)
    snap = scen_mod._snapshot(cal)
    for name in (REPL if repl else FULL):
        scen_mod._restore(cal, snap)
        sol = scen_mod.run_scenario(cal, scen_mod.SCENARIOS[name](cal, per), ref, per,
                                    verbose=False)
        rows.append((name,) + audit(cal, sol, per))      # audit before restoring
        scen_mod._restore(cal, snap)
        print(f"{name} audited ({time.time() - t0:.0f} s)", flush=True)

    ok = all(not p for *_, p in rows)
    out = os.path.join(ROOT, "reports",
                       "SOLUTION-AUDIT" + ("-gams-replication" if repl else "") + ".md")
    md = ["# Solution audit" + (" (GAMS-replication mode)" if repl else " (full model)"), "",
          "Every equation re-evaluated at the solution of every year 2019-2040; "
          "quota, informal-fuel and idle-resource conditions and lower bounds checked. "
          "Generated by `validation/check_solutions.py`.", "",
          "| run | largest residual | largest Walras slack | problems |",
          "| --- | ---: | ---: | --- |"]
    for name, worst, walras, problems in rows:
        md.append(f"| {name} | {worst:.1e} | {walras:.1e} | "
                  + ("none" if not problems else "; ".join(problems[:5])
                     + (f" (+{len(problems) - 5} more)" if len(problems) > 5 else "")) + " |")
    md += ["", f"Result: **{'PASS' if ok else 'FAIL'}**."]
    open(out, "w").write("\n".join(md) + "\n")
    for name, worst, walras, problems in rows:
        print(f"  {name:<24} residual {worst:.1e}  Walras {walras:.1e}  "
              + ("ok" if not problems else f"{len(problems)} problem(s): {problems[0]}"))
    print(f"AUDIT: {'PASS' if ok else 'FAIL'}   ({out})")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
