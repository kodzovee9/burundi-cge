"""Reform scenarios with the first group of benefit-raising adjustments.

    python3 runs/reform_boost.py --rent-cost 0.25      # writes one column set
    python3 runs/reform_boost.py --collect             # merges into reports/

Two additions (APPENDIX.md, equation A.5a and section A.7.1):

- the premium rent as a real cost: a share `rent_cost` of the parallel-market
  premium rent is dissipated in unproductive activity (model.py EQ_TFPDEF);
- `uni+bs`: unification with temporary budget support (scenarios.uni_bs).

For each rent-cost share it solves the base and reference passes, then `uni`,
`uni+bs`, `uni+inf`, `uni+inf+hd` and `combi`, and records average growth
2026-40 (the published measure) and the 2040 level of each aggregate against
the base run. The 2040 level is the better measure of a reform's size: the
growth average starts in 2026, when half the premium cut has already happened.
"""

from __future__ import annotations

import argparse
import json
import os
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, ROOT)
sys.path.insert(0, HERE)

from gemcore.database import load_database, bdi2019_data2          # noqa: E402
from gemcore.calibration import calibrate                          # noqa: E402
from gemcore.dynamics import (run_base, run_reference,              # noqa: E402
                              par_redefn_0)
from gemcore import scenarios as scen_mod                          # noqa: E402
from run import DATA, ORDER, growth, real_aggregates               # noqa: E402

SCENS = ["uni", "uni+bs", "uni+bs-inv", "uni+inf", "uni+inf-m", "uni+inf-x",
         "uni+inf+hd", "uni+inf+hd-x", "combi", "combi-x"]
OUTDIR = os.path.join(ROOT, "reports")
TMP = os.path.join(OUTDIR, "reform-boost")


def run_one(omega):
    t0 = time.time()
    cal = calibrate(load_database(DATA, data2_hook=bdi2019_data2), rent_cost=omega)
    per = [t for t in cal.db.sets["tsol"] if int(t) <= 2040]
    pass1 = run_base(cal, dcal01=True, verbose=False, periods=per)
    ref = run_reference(cal, pass1, verbose=False, periods=per)
    par_redefn_0(cal, ref, verbose=False)
    base40 = real_aggregates(cal, ref["2040"], "2040")
    base27 = real_aggregates(cal, ref["2027"], "2027")
    out = {"rent_cost": omega, "rent_share00": cal.rent_share00,
           "rent_share_ref": {t: sum(ref[t]["YPREXRT"].values()) / ref[t]["GDPMP"][t]
                              for t in ("2019", "2026", "2030", "2040")},
           "growth": {"base": growth(cal, ref)}, "level2040": {}, "level2027": {}}
    out["growth"]["base"]["GDPMP"] = 100 * ((ref["2040"]["RGDPMP"]["2040"]
                                             / ref["2026"]["RGDPMP"]["2026"]) ** (1 / 14) - 1)
    print(f"omega={omega}: base+reference {time.time()-t0:.0f}s; rent/GDP 2019 "
          f"{100*cal.rent_share00:.1f}%, 2040 {100*out['rent_share_ref']['2040']:.1f}%",
          flush=True)
    snap = scen_mod._snapshot(cal)
    for name in SCENS:
        scen_mod._restore(cal, snap)
        sol = scen_mod.run_scenario(cal, scen_mod.SCENARIOS[name](cal, per), ref, per,
                                    verbose=False)
        scen_mod._restore(cal, snap)
        out["growth"][name] = growth(cal, sol)
        out["growth"][name]["GDPMP"] = 100 * ((sol["2040"]["RGDPMP"]["2040"]
                                               / sol["2026"]["RGDPMP"]["2026"]) ** (1 / 14) - 1)
        s40 = real_aggregates(cal, sol["2040"], "2040")
        out["level2040"][name] = {k: 100 * (s40[k] / base40[k] - 1) for k in ORDER}
        out["level2040"][name]["GDPMP"] = 100 * (sol["2040"]["RGDPMP"]["2040"]
                                                  / ref["2040"]["RGDPMP"]["2040"] - 1)
        s27 = real_aggregates(cal, sol["2027"], "2027")
        out["level2027"][name] = {k: 100 * (s27[k] / base27[k] - 1) for k in ORDER}
        print(f"  {name}: GDP {out['growth'][name]['GDPFC']:.3f} %/yr, 2040 level "
              f"{out['level2040'][name]['GDPFC']:+.1f} % ({time.time()-t0:.0f}s)", flush=True)
    os.makedirs(TMP, exist_ok=True)
    json.dump(out, open(os.path.join(TMP, f"omega-{omega:g}.json"), "w"), indent=1)


def collect():
    runs = sorted((json.load(open(os.path.join(TMP, f)))
                   for f in os.listdir(TMP) if f.endswith(".json")),
                  key=lambda r: r["rent_cost"])
    lines = ["# Reform scenarios with the benefit-raising adjustments", "",
             "Average growth 2026-40 (%/yr) and 2040 level against the base run (%). "
             "ω is the share of the premium rent that is a real cost (0 = GEM-Core). "
             "`uni+bs` is unification with budget support of 1 % of GDP in 2026-27 and "
             "0.5 % in 2028, which lowers the direct-tax rate; `uni+bs-inv` spends it on "
             "public investment instead. `-m`: infrastructure also cuts transport costs (-15 %) and "
             "marketing margins on farm and food products (-10 %); `-x`: plus hydropower (fuel per "
             "unit of electricity -50 %); `uni+inf+hd-x` adds the education-mix shift (2 % of the "
             "labour force from primary to secondary by 2040); `combi-x` adds FDI into mining. "
             "GDPFC is value added at base-year unit values and misses savings in intermediate "
             "inputs; GDPMP (final demand at base-year prices) captures them. "
             "Generated by `runs/reform_boost.py`.", ""]
    for r in runs:
        rs = r["rent_share_ref"]
        lines += [f"## ω = {r['rent_cost']:g}", "",
                  f"Premium rent in the reference path: {100*rs['2019']:.1f} % of GDP in 2019, "
                  f"{100*rs['2030']:.1f} % in 2030, {100*rs['2040']:.1f} % in 2040.", "",
                  "| indicator | base | " + " | ".join(SCENS) + " |",
                  "| --- | ---: | " + " | ".join("---:" for _ in SCENS) + " |"]
        for k in ORDER:
            lines.append(f"| {k} growth | {r['growth']['base'][k]:.2f} | "
                         + " | ".join(f"{r['growth'][s][k]:.2f}" for s in SCENS) + " |")
        lines.append(f"| GDP at market prices growth | {r['growth']['base']['GDPMP']:.2f} | "
                     + " | ".join(f"{r['growth'][s]['GDPMP']:.2f}" for s in SCENS) + " |")
        for k in ("GDPFC", "GDPMP", "PrvCon", "PrvFixInv", "Exports"):
            lines.append(f"| {k}, 2040 level vs base, % | – | "
                         + " | ".join(f"{r['level2040'][s][k]:+.1f}" for s in SCENS) + " |")
        for k in ("GDPFC", "PrvCon"):
            lines.append(f"| {k}, 2027 level vs base, % | – | "
                         + " | ".join(f"{r['level2027'][s][k]:+.1f}" for s in SCENS) + " |")
        lines.append("")
    path = os.path.join(OUTDIR, "reform-boost-2026-2040.md")
    open(path, "w").write("\n".join(lines) + "\n")
    print("wrote", path)


if __name__ == "__main__":
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--rent-cost", type=float, default=None)
    ap.add_argument("--collect", action="store_true")
    a = ap.parse_args()
    if a.collect:
        collect()
    else:
        run_one(a.rent_cost if a.rent_cost is not None else 0.25)
