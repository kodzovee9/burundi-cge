"""Run the 2019-2024 backcast and compare the model to Burundi's actuals.

    python3 runs/backcast.py                     # defaults: INSBU rebased accounts (basis,
                                                 # targets, government consumption, trade prices,
                                                 # export volumes, mining, inventories);
                                                 # prexr000 = 1.583; actual real official rate
    python3 runs/backcast.py --premium flat --tag premium-flat   # premium held at 2019 (sensitivity)
    python3 runs/backcast.py --govcon insbu --tag govcon-share   # gov consumption as a GDP share
    python3 runs/backcast.py --stocks model --tag stocks-model   # inventories grow with GDP
    python3 runs/backcast.py --basis workbook --sector-source mfmod --govcon na \
        --mining-index mfmod-ind --stocks model --tag pre-insbu  # the 2026-09-28 setup

Loads the workbook with the base-year premium overridden, replaces the
2020-2024 exogenous paths with actuals (`gemcore.backcast.apply_actuals`),
checks the base year is still exact, solves the calibration pass through
`--to`, and writes `reports/backcast-2019-2024[-tag].md` and `.csv`.

The calibration pass (dcal01 = 1) is the right one for a backcast: real GDP is
pinned to the actual growth path and productivity is backed out, so what the
comparison tests is the model's COMPOSITION -- shares, sector growth, trade,
external and fiscal balances -- given actual GDP and actual policy.
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
from gemcore.state import build_state, Closure                     # noqa: E402
from gemcore.model import residuals                                # noqa: E402
from gemcore.complementarity import sweep_solve, sweep_report     # noqa: E402
from gemcore.actuals import load_actuals                           # noqa: E402
from gemcore.backcast import (apply_actuals, set_export_volumes,    # noqa: E402
                              model_series, actual_series, compare,
                              PREXR000_ACTUAL, EXOG_EXPORTS,
                              set_idle_factors, IDLE_FACTORS,
                              set_resource_supply, set_sector_targets,
                              VA_GROUP_LABEL, VA_GROUPS,
                              DEFAULT_TARGET_GROUPS, GOVCON_SOURCES,
                              scale_armington, set_sector_source,
                              all_exporters, mining_index,
                              MINING_INDEX_SOURCES, REXR_SOURCES, MACRO_BASES,
                              set_stock_path, set_real_govcon,
                              set_real_govcon_path, set_quota_path,
                              TRADE_PRICE_SOURCES)

DATA = os.path.join(ROOT, "..", "model", "user-files", "bdi2019",
                    "bdi2019-data.xlsx")
OUTDIR = os.path.join(ROOT, "reports")


def fmt(v, kind):
    if v is None:
        return "-"
    return f"{v:.1f}" if kind != "index" else f"{v:.1f}"


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--prexr", type=float, default=PREXR000_ACTUAL,
                    help="base-year parallel/official ratio (default: actual 2019)")
    ap.add_argument("--through", default="2024", help="last year of actuals")
    ap.add_argument("--to", type=int, default=2026,
                    help="last year to solve (a little past --through, for continuity)")
    ap.add_argument("--tag", default="", help="suffix for the output files")
    ap.add_argument("--exog", default="all",
                    help="commodities on the quantity export closure, comma-"
                         "separated; '' for none (all exports on the CET at "
                         "world prices, the model's default); 'all' (default) "
                         "for every exporter but gold, on MFMod's aggregate "
                         "real-export index; 'c-agr' pins coffee/tea only")
    ap.add_argument("--no-exog-exports", action="store_true",
                    help="shorthand for --exog ''")
    ap.add_argument("--idle", default=",".join(IDLE_FACTORS),
                    help="factors on the idle-resource closure (rent fixed, "
                         "utilisation free), comma-separated; '' for none")
    ap.add_argument("--mining-index", default=None,
                    choices=list(MINING_INDEX_SOURCES),
                    help="real mining-output index scaling the f-nrmin "
                         "endowment: 'insbu-b05' (default with INSBU; real "
                         "value added of extraction), 'mfmod-ind' (MFMod real "
                         "Industry growth), 'unsd', 'flat'")
    ap.add_argument("--stock-goods", default="c-agr,c-food",
                    help="commodities whose observed stock change is added "
                         "(with --stocks insbu), comma-separated. c-min is "
                         "left out by default: INSBU's 2024 mineral stock "
                         "build-up (1.7%% of GDP) with extraction down 14%% "
                         "does not solve (APPENDIX.md, A.6.3)")
    ap.add_argument("--investment", default=None, choices=["insbu", "budget"],
                    help="investment paths: 'insbu' (default with INSBU; "
                         "public and private move with INSBU's total GFCF "
                         "share) or 'budget' (public from IN09, private on "
                         "the authors' path)")
    ap.add_argument("--quotas", default="brb", choices=["brb", "data"],
                    help="fuel/chemicals import-quota ceilings: 'brb' (default; "
                         "BRB's observed import volumes, gemcore/brb.py) or "
                         "'data' (the authors' path, frozen at 2019 volumes)")
    ap.add_argument("--quota-through", default="2024",
                    help="last year of observed quota ceilings (default 2024, "
                         "the fuel crisis included; later years hold that level)")
    ap.add_argument("--fuel-quota", default="brb", choices=["brb", "fx"],
                    help="fuel ceiling in the observed years: 'brb' (default; "
                         "BRB's volumes, the share of official foreign "
                         "exchange they imply is reported and carried after "
                         "--quota-through) or 'fx' (BRB's foreign-exchange "
                         "allocation held at its 2019 share of official FX "
                         "from 2020: fuel imports are what FX availability "
                         "alone allows; gemcore/fuel.py channel 4)")
    ap.add_argument("--chem-quota", default="brb", choices=["brb", "fx"],
                    help="chemicals and fertiliser ceiling in the observed years: "
                         "'brb' (default; BRB's volumes, implied FX share reported) "
                         "or 'fx' (BRB's FX allocation at its 2019 share from 2020)")
    ap.add_argument("--passthrough", type=float, default=1.0,
                    help="share of the change in the premium since 2019 that "
                         "reaches import prices (model.py, cal.prexr_pt); 1 = "
                         "the published model. Exports keep the full premium")
    ap.add_argument("--premium", default="actual", choices=["actual", "flat"],
                    help="parallel-market premium path: 'actual' (IN03) or "
                         "'flat' (2019 level; sensitivity for the official-"
                         "rate share of imports)")
    ap.add_argument("--stocks", default=None, choices=["insbu", "model"],
                    help="inventories: 'insbu' (default with INSBU; the "
                         "observed change in stocks by commodity is added) or "
                         "'model' (stocks grow with GDP, mod.gms)")
    ap.add_argument("--fixed-resource", action="store_true",
                    help="keep the f-nrmin endowment at its base path instead "
                         "of scaling it with mining output")
    ap.add_argument("--sector-targets", action="store_true",
                    help="sector supply shocks: pin the VA growth of the "
                         "--target-groups to the observed path and back out "
                         "group productivity (TFPGRP); real GDP becomes an "
                         "outcome unless --pin-gdp. Default: aggregate "
                         "calibration, real GDP pinned to IN04")
    ap.add_argument("--target-groups", default=",".join(DEFAULT_TARGET_GROUPS),
                    help="groups to pin, comma-separated, from "
                         f"{', '.join(VA_GROUPS)} (default "
                         f"{','.join(DEFAULT_TARGET_GROUPS)}; 'oind' is a "
                         "derived residual, see gemcore/backcast.py)")
    ap.add_argument("--pin-gdp", action="store_true",
                    help="with sector targets, ALSO keep real GDP pinned to "
                         "IN04 (TFPSCAL flexes, untargeted sectors absorb the "
                         "residual). Does not converge with agriculture "
                         "pinned -- see gemcore/backcast.py. Default: GDP is "
                         "an outcome (TFPSCAL = 0)")
    ap.add_argument("--basis", default=None, choices=list(MACRO_BASES),
                    help="GDP basis for growth, deflator, fiscal/BoP ratios "
                         "and targets: 'insbu' (default; INSBU's rebased "
                         "accounts, from raw-insbu/ or the extract in data/), 'workbook' (WDI "
                         "GDP) or 'mfmod' (MFMod's own accounts)")
    ap.add_argument("--trade-prices", default="implicit",
                    choices=list(TRADE_PRICE_SOURCES),
                    help="world trade prices fed to the model: 'implicit' "
                         "(default; MFMod's realised export/import deflators "
                         "in USD, consistent with its volumes) or 'keyfitz' "
                         "(IN06/IN07, MFMod's world-price proxies: +20%% and "
                         "+72%% by 2024 against realised -3%% and +15%%)")
    ap.add_argument("--rowclos", default="data", choices=["data", "1"],
                    help="rest-of-world closure 2020-through: 'data' (the "
                         "dataset's rowclos 2: real rate and premium pinned, "
                         "non-government foreign financing clears) or '1' "
                         "(foreign financing on the path implied by the "
                         "actual current account, real official rate FREE -- "
                         "tests whether the model produces the observed real "
                         "appreciation)")
    ap.add_argument("--rexr", default="actual", choices=list(REXR_SOURCES),
                    help="real official exchange rate under rowclos 2: "
                         "'actual' (default; IN01 over the GDP deflator, a 24%% "
                         "real appreciation by 2024) or 'fixed' (the authors' "
                         "flat path)")
    ap.add_argument("--sector-source", default=None, choices=["insbu", "mfmod", "wdi"],
                    help="reference for sector growth (targets and comparison "
                         "rows): INSBU (default when present), the MFMod "
                         "datasheet, or the workbook's WDI block (withdrawn)")
    ap.add_argument("--sector-from", default="2020",
                    help="first year of the sector targets; earlier years keep "
                         "the aggregate GDP pin (default 2020)")
    ap.add_argument("--armington", default="",
                    help="scale Armington sigma_q before calibration, e.g. "
                         "'c-agr:2,c-food:2' or 'all:2' (import-response "
                         "sensitivity)")
    ap.add_argument("--govcon", default=None, choices=list(GOVCON_SOURCES),
                    help="government-consumption path: 'insbu-real' (default "
                         "when present; INSBU's real volume, spending rule 1), "
                         "'insbu' (INSBU's share of GDP, 10.6%% -> 7.5%%), "
                         "'budget' (IN08, fiscal data) or 'na' (UNSD share "
                         "TG13 -- wrong, kept to reproduce old reports)")
    ap.add_argument("--verbose", action="store_true")
    args = ap.parse_args()
    sector = args.sector_targets
    tgroups = tuple(x.strip() for x in args.target_groups.split(",") if x.strip())
    pin_gdp = args.pin_gdp
    exog = () if (args.no_exog_exports or not args.exog.strip()) else \
        tuple(x.strip() for x in args.exog.split(",") if x.strip())
    t0 = time.time()

    db = load_database(DATA, data2_hook=bdi2019_data2,
                       overrides={"prexr000": {(): args.prexr}})
    act = load_actuals()
    have_insbu = act.insbu is not None
    if not have_insbu:
        print("!! no INSBU data (raw-insbu/ or data/insbu-aggregates-*.csv); falling back to the "
              "workbook/MFMod setup", file=sys.stderr)
    args.basis = args.basis or ("insbu" if have_insbu else "workbook")
    args.sector_source = args.sector_source or ("insbu" if have_insbu else "mfmod")
    args.govcon = args.govcon or ("insbu-real" if have_insbu else "budget")
    args.mining_index = args.mining_index or ("insbu-b05" if have_insbu else "mfmod-ind")
    args.stocks = args.stocks or ("insbu" if have_insbu else "model")
    args.investment = args.investment or ("insbu" if have_insbu else "budget")
    for need, what in ((args.govcon == "insbu-real", "--govcon insbu-real"),
                       (args.basis == "insbu", "--basis insbu"),
                       (args.sector_source == "insbu", "--sector-source insbu"),
                       (args.govcon == "insbu", "--govcon insbu"),
                       (args.mining_index == "insbu-b05", "--mining-index insbu-b05"),
                       (args.stocks == "insbu", "--stocks insbu")):
        if need and not have_insbu:
            ap.error(f"{what} needs INSBU's accounts (raw-insbu/ or the extract in data/)")
    if args.sector_source == "mfmod" and not act.mfmod:
        print("!! MFMod datasheet not found in data/ (it is not part of the "
              "share package); falling back to --sector-source wdi and "
              "--exog c-agr", file=sys.stderr)
        args.sector_source = "wdi"
        if exog == ("all",):
            exog = EXOG_EXPORTS
    set_sector_source(args.sector_source)
    # `--exog all`: every exporter but gold on the quantity closure, all on
    # MFMod's aggregate real-export index (total exports as observed)
    exog_all = exog == ("all",)
    exp_source = "insbu" if args.basis == "insbu" else "mfmod"
    if exog_all:
        if exp_source == "mfmod" and not act.mfmod:
            ap.error("--exog all needs the MFMod datasheet or the INSBU files")
        exog = all_exporters(db)
    if args.basis == "mfmod" and not act.mfmod:
        print("!! MFMod datasheet not found; --basis workbook", file=sys.stderr)
        args.basis = "workbook"
    log = apply_actuals(db, act, through=args.through, exog_exports=exog,
                        govcon_source=args.govcon, rexr_source=args.rexr,
                        basis=args.basis, trade_prices=args.trade_prices,
                        rowclos=args.rowclos, investment=args.investment,
                        premium=args.premium)
    if args.govcon == "insbu-real":
        log.update(set_real_govcon(db))
    if args.armington.strip():
        fac = {}
        for item in args.armington.split(","):
            c, k = item.split(":")
            if c.strip() == "all":
                fac.update({cc: float(k) for cc in db.sets["c"]
                            if (cc, "sigma_q") in db.pars["tradelas"]})
            else:
                fac[c.strip()] = float(k)
        log.update(scale_armington(db, fac))
    cal = calibrate(db)
    log.update(set_export_volumes(cal, act, through=args.through,
                                  exog_exports=exog, aggregate=exog_all,
                                  source=exp_source))
    if args.govcon == "insbu-real":
        log.update(set_real_govcon_path(cal, act, through=args.through))
    if args.quotas == "data" and getattr(cal, "fuel", None) is not None:
        cal.fuel.fx_quota = False      # the authors' volume ceilings, fuel included
    if args.quotas == "brb":
        log.update(set_quota_path(cal, through=min(args.quota_through, args.through),
                                  fuel_volumes=args.fuel_quota == "brb",
                                  chem_volumes=args.chem_quota == "brb"))
    if args.passthrough != 1.0:
        cal.prexr_pt = args.passthrough
        cal.prexr_pt_anchor = {t: cal.PREXR00 for t in cal.PREXR0}
        log["premium pass-through to import prices"] = (
            f"{args.passthrough:g} of the change since 2019 (anchor {cal.PREXR00:.3f})")
    if args.stocks == "insbu":
        log.update(set_stock_path(cal, act, through=args.through,
                                  goods=tuple(x.strip() for x in args.stock_goods.split(",") if x.strip())))
    idle = tuple(x.strip() for x in args.idle.split(",") if x.strip())
    log.update(set_idle_factors(cal, idle))
    if not args.fixed_resource:
        log.update(set_resource_supply(
            cal, act, "f-nrmin", through=args.through,
            index=mining_index(act, args.mining_index, args.through)))
    if sector:
        log.update(set_sector_targets(cal, act, through=args.through,
                                      groups=tgroups, pin_gdp=pin_gdp,
                                      start=args.sector_from))
    tmin = cal.db.sets["tmin"][0]
    print(f"prexr000 requested {args.prexr:.4f}  ->  PREXR00 calibrated "
          f"{cal.PREXR00:.4f}")

    # The SAM adjustment is rebuilt on the new premium; the base year must
    # still be exact or the adjustment is inconsistent.
    cal._solve_t = tmin
    V0 = build_state(cal, tmin)
    R = residuals(V0, cal, tmin, {}, Closure(cal, dcal01=True))
    worst = max(abs(v) for v in R.values() if v is not None)
    print(f"base-year max |residual| = {worst:.3e}  "
          f"{'OK' if worst < 1e-9 else '!! NOT EXACT'}")

    print("\nfed in (2020-{}):".format(args.through))
    for k, v in log.items():
        print(f"  {k:<28} {v}")

    per = [t for t in cal.db.sets["tsol"] if int(t) <= args.to]

    # EQ_QMCONST is a complementarity (mod.gms 2608): the quota either binds
    # with a positive rent, or is slack with imports below the ceiling and rent
    # zero. The port solves the binding branch as an equality and switches a
    # (c,t) to slack only when told (`cal.quota_slack`). So: solve, find any
    # cell whose rent came out NEGATIVE -- impossible on the binding branch --
    # mark it slack, and solve again, until no rent is negative and no slack
    # cell imports more than its ceiling.
    cal.quota_slack = set()
    sols = sweep_solve(cal, Closure(cal, dcal01=True), per, verbose=args.verbose,
                       idle_factors=idle)
    for k, v in sweep_report(cal, per, args.through).items():
        print(f"{k}: {v}")
    # convergence: every reported year must be a real solution
    bad = []
    for t in per:
        cal._solve_t = t
        Lp = sols[per[per.index(t) - 1]] if per.index(t) else {}
        Rt = residuals(sols[t], cal, t, Lp, Closure(cal, dcal01=True))
        w = max(abs(v) for v in Rt.values() if v is not None)
        if w > 1e-6:
            bad.append((t, w))
    print("solve residuals:", "all periods < 1e-6" if not bad
          else "!! NOT CONVERGED " + str([(t, f"{w:.1e}") for t, w in bad]))
    last = [t for t in per if t <= args.through][-1]
    if sector:
        print("backed-out group productivity (TFPGRP, 2019 = 1):")
        for g in cal.tfp_group_list:
            print(f"  {VA_GROUP_LABEL[g]:<30}",
                  {t: round(sols[t]["TFPGRP"][g], 3) for t in per if t <= last})
    r0 = sols[per[0]]["REXR"][per[0]]
    print("real official rate REXR (2019 = 1):",
          {t: round(sols[t]["REXR"][t] / r0, 3) for t in per if t <= last},
          "  premium PREXR:", {t: round(sols[t]["PREXR"][t], 3) for t in per if t <= last})
    fcfg = getattr(cal, "fuel", None)
    if fcfg is not None and fcfg.fx_quota:
        from gemcore.fuel import official_fx, sol_getter
        S = cal.db.sets
        fu = fcfg.fuel
        print("official foreign exchange, model USD (2019 = 100): exports surrendered "
              "/ grants to government / government net borrowing / total; fuel and "
              "chemicals shares %")
        o0 = official_fx(cal, sol_getter(sols[per[0]]), per[0])
        for t in [t for t in per if t <= last]:
            V = sols[t]
            xo = sum(cal.shroe00[c] * V["PWE"][c] * V["QE"][c] for c in S["c"]
                     if cal.shroe00.get(c) and cal.QE00.get(c))
            gr = sum(V["TRNSFR"][(g, r)] for g in S["insgov"] for r in S["insrow"]
                     if (g, r) in V["TRNSFR"])
            ofx = official_fx(cal, sol_getter(V), t)
            print(f"  {t}  {100*xo/o0:6.1f} {100*gr/o0:6.1f} {100*V['NFFG'][t]/o0:6.1f} "
                  f"{100*ofx/o0:6.1f}   fuel {100*V['PWM'][fu]*V['QM'][fu]/ofx:5.1f}  "
                  f"chemicals {100*V['PWM']['c-chemplast']*V['QM']['c-chemplast']/ofx:5.1f}  "
                  f"(world fuel price {V['PWM'][fu]/sols[per[0]]['PWM'][fu]:.2f})")
    for f in idle:
        print(f"idle share of {f} (UERAT):",
              {t: round(100 * sols[t]["UERAT"][f], 1) for t in per if t <= last},
              "%   rent WF:", {t: round(sols[t]["WF"][f], 3) for t in (per[0], last)})
    years = [t for t in per if t <= args.through]
    m = model_series(cal, sols, years)
    a = actual_series(act, years, basis=args.basis)
    rows = compare(m, a, years)

    # ---- terminal table ---------------------------------------------------
    w = 9
    print(f"\n=== backcast {years[0]}-{years[-1]}, prexr000={args.prexr}   "
          f"[{time.time()-t0:.0f}s]")
    hdr = f"{'indicator':<34}{'':<7}" + "".join(f"{y:>{w}}" for y in years)
    print(hdr)
    for name, kind, cells, summ in rows:
        ys = [y for y in years if y in cells]
        mline = f"{name:<34}{'model':<7}" + "".join(
            f"{fmt(cells[y][0], kind):>{w}}" if y in cells else " " * w for y in years)
        aline = f"{'':<34}{'actual':<7}" + "".join(
            f"{fmt(cells[y][1], kind):>{w}}" if y in cells else " " * w for y in years)
        s = "  ".join(f"{k}={fmt(v, kind)}" for k, v in summ.items())
        print(mline + f"   | {s}")
        print(aline)

    # ---- files -----------------------------------------------------------
    os.makedirs(OUTDIR, exist_ok=True)
    tag = f"-{args.tag}" if args.tag else ""
    md = os.path.join(OUTDIR, f"backcast-{years[0]}-{years[-1]}{tag}.md")
    cs = os.path.join(OUTDIR, f"backcast-{years[0]}-{years[-1]}{tag}.csv")
    with open(md, "w") as fh:
        fh.write(f"# Backcast {years[0]}-{years[-1]}\n\n")
        src = {"insbu": "INSBU rebased accounts", "workbook": "actuals workbook (WDI GDP)",
               "mfmod": "MFMod datasheet"}
        setup = [
            ("Macro basis (growth, deflator, ratios, targets)", src[args.basis]),
            ("Real GDP", "pinned to the basis's real growth (calibration pass, aggregate TFP backed out)"
             if not sector else
             f"sector targets: {', '.join(VA_GROUP_LABEL[g] for g in tgroups)} pinned to "
             f"{src.get(args.sector_source, args.sector_source)}, group productivity backed out; "
             + ("GDP also pinned" if pin_gdp else "GDP an outcome")),
            ("Sector reference", src.get(args.sector_source, args.sector_source)),
            ("Government consumption", {"insbu-real": "INSBU real path (spending rule 1)",
                                        "insbu": "INSBU share path", "budget": "fiscal data (IN08)",
                                        "na": "UNSD share (TG13), withdrawn"}[args.govcon]),
            ("Exports", "every exporter but gold pinned to " + src.get(exp_source, exp_source)
             + " real exports" if exog_all else
             ("quantity closure for " + ", ".join(exog) if exog else "CET at world prices")),
            ("Trade prices", ("realised deflators in USD (" + ("INSBU" if args.basis == "insbu" else "MFMod") + ")")
             if args.trade_prices == "implicit" else "Keyfitz world-price indices"),
            ("Real official exchange rate", "observed (official rate over the basis GDP deflator)"
             if args.rexr == "actual" else "flat (authors)"),
            ("Mining resource", "fixed" if args.fixed_resource else f"scaled by {args.mining_index}"),
            ("Inventories", f"observed change added for {args.stock_goods} (INSBU)" if args.stocks == "insbu"
             else "grow with GDP (model)"),
            ("Investment", "public and private on INSBU's total GFCF share" if args.investment == "insbu"
             else "public from IN09, private on the authors' path"),
            ("Parallel premium", "observed (IN03)" if args.premium == "actual"
             else "held at 2019 level (sensitivity)"),
            ("Fuel and chemicals import quotas",
             ("; ".join(
                 f"{lab} " + ("set by foreign exchange from 2020 (BRB's allocation at its 2019 share of "
                              "official FX, over the world price)" if mode == "fx" else
                              f"at BRB's observed volumes to {args.quota_through}, then set by foreign "
                              f"exchange (share of official FX carried from {args.quota_through})")
                 for lab, mode in (("fuel", args.fuel_quota), ("chemicals and fertiliser", args.chem_quota))))
             if args.quotas == "brb" else "authors' path (frozen at 2019 volumes)"),
            ("Premium pass-through to import prices",
             f"{args.passthrough:g} of the change since 2019" if args.passthrough != 1.0 else "full (published model)"),
            ("Rest-of-world closure", "rowclos 1 (experimental)" if args.rowclos == "1"
             else "rowclos 2 (data): real rate and premium pinned, foreign financing clears"),
            ("Base-year premium", f"prexr000 = {args.prexr} (PREXR00 = {cal.PREXR00:.4f}); "
             f"base-year max residual {worst:.2e}"),
        ]
        fh.write("| setup | |\n| --- | --- |\n")
        for k, v in setup:
            fh.write(f"| {k} | {v} |\n")
        fh.write("\n")
        for kind, title in (("share", "Shares of GDP and rates (%)"),
                            ("growth", "Growth (%/yr)"),
                            ("index", "Indices, 2019 = 100")):
            sub = [r for r in rows if r[1] == kind]
            if not sub:
                continue
            fh.write(f"## {title}\n\n")
            ys = years if kind != "growth" else years[1:]
            fh.write("| indicator | " + " | ".join(f"{y} m | {y} a" for y in ys)
                     + " | summary |\n")
            fh.write("| --- |" + " ---: |" * (2 * len(ys)) + " --- |\n")
            for name, _, cells, summ in sub:
                vals = []
                for y in ys:
                    mv, av = cells.get(y, (None, None))
                    vals += [fmt(mv, kind), fmt(av, kind)]
                s = "; ".join(f"{k} {fmt(v, kind)}" for k, v in summ.items())
                fh.write(f"| {name} | " + " | ".join(vals) + f" | {s} |\n")
            fh.write("\n")
        fh.write(("Real GDP growth is an outcome here: the sum of the pinned "
                  "sector paths at SAM weights against the national-accounts "
                  "figure. " if (sector and not pin_gdp) else
                  "Real GDP growth is an input (pinned), shown as a check only. ")
                 + "Levels are not comparable (SAM GDP is about twice the "
                 "national-accounts figure); every row is a share, growth "
                 "rate or index. TFP shifter rows are model-only (100 = base "
                 "year).\n")
    with open(cs, "w", newline="") as fh:
        wr = csv.writer(fh)
        wr.writerow(["indicator", "kind", "year", "model", "actual"])
        for name, kind, cells, _ in rows:
            for y, (mv, av) in cells.items():
                wr.writerow([name, kind, y,
                             "" if mv is None else f"{mv:.4f}",
                             "" if av is None else f"{av:.4f}"])
    print(f"\nwrote {md}\nwrote {cs}")


if __name__ == "__main__":
    main()
