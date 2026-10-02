"""Backcast 2019-2024: feed the model actual inputs, compare its outputs.

Two halves. `apply_actuals` mutates the loaded database so the 2020-`through`
exogenous paths are Burundi's actuals rather than the workbook's projections.
`model_series` / `actual_series` reduce a solved path and the actuals workbook
to the same set of comparable indicators, and `compare` lines them up.

What is fed in (all from `actuals.load_actuals()`):
  IN04  real GDP growth        -> gdpgrw(t)           pins RGDPFC in pass 1
  IN03  FX premium ratio       -> prexrindex0(t)      relative to 2019
  IN05  fuel import price      -> pwmindex('c-refpet',t)
  IN06  import price index     -> pwmindex(c,t), all other c
  IN07  export price index     -> pweindex(c,t), c-agr and c-min only
  IN06  (again)                -> pweindex(c,t), all other c
  IN08  gov consumption %GDP   -> govspndgdp0('congov',t)      rule 2, binds
  IN09  gov capital %GDP       -> govspndgdp0('f-capgov',t)    rule 2, binds
  IN11  grants %GDP            -> govrecgdp0('trgovrow',t)     rule 2, binds
  IN12  domestic financing     -> govrecgdp0('netdomfin',t)    rule 2, binds
  IN13  external financing     -> govrecgdp0('netforfingov',t) rule 2, binds
  TG17  remittances USD mn     -> ngovpaygdp0('trrowngov',t)   rule 2, binds
  TG16  FDI inflows USD mn     -> ngovpaygdp0('fdi',t)         rule 2, binds
  IN17  domestic interest rate -> gintrat0(t)
  IN18  external interest rate -> fintrat0(ins2,t)
Population (IN14/15) already equals the workbook's UN series to four figures
and is left alone. Tax revenue (IN10) is rule 1 -- the scalars are fixed and
revenue is an outcome -- so it is a target, not an input. Quota changes
(IN16) are text; the ceiling index is left as it is.

Beyond `through` every actual path is held flat at its last value so the
solve can run a year or two past the window for continuity; those years are
out of sample and not reported.

What is NOT comparable and why: real GDP growth is pinned, so it is an input
check, not a result. Nominal CPI inflation is not modelled (DPI is the
numeraire); what is testable is the CPI relative to the GDP deflator.
Absolute levels are not compared -- the SAM's units and price basis are
not the published accounts' (APPENDIX.md, A.2.1) -- so every indicator is a
share, a growth rate, or an index on 2019.
"""

from __future__ import annotations

from .actuals import SECTOR_GROUPS, YEARS

PREXR000_ACTUAL = 1.583          # IN03, 2019 -- parallel/official ratio

FISCAL_MAP = {                   # input id -> (parameter, account); all rule 2
    "IN08": ("govspndgdp0", "congov"),
    "IN09": ("govspndgdp0", "f-capgov"),
    "IN11": ("govrecgdp0", "trgovrow"),
    "IN12": ("govrecgdp0", "netdomfin"),
    "IN13": ("govrecgdp0", "netforfingov"),
}

# model sector groups -> the actuals' sector growth series. Two sources:
#   "mfmod" (default): the World Bank MFMod datasheet, sheet NIA-Vol, real
#           value added by broad sector (data/mfmod-bdi-NIA-2026-09.xlsx),
#           the user's reference; no manufacturing split.
#   "wdi":  the Sectors sheet's "Real VA growth" block in the actuals
#           workbook. Its 2020 values (agriculture -12.9, services +4.9) do
#           not match MFMod (+3.4, +2.9) and were withdrawn by the user on
#           2026-09-16; kept only to reproduce the earlier reports.
ISIC_MAP_WDI = {
    "Agriculture (A-B)":  (["agr"],                       "Agriculture (ISIC A-B)"),
    "Industry (C-F)":     (["ind", "construc"],           "Industry (ISIC C-F)"),
    "Manufacturing (D)":  (["manuf"],                     "Manufacturing (ISIC D)"),
    "Services (G-P)":     (["trade", "transp", "pubsvc"], "Services (ISIC G-P)"),
}
ISIC_MAP_MFMOD = {
    "Agriculture":        (["agr"],                       "NVAGRTOTLKN"),
    "Industry":           (["ind", "construc"],           "NVINDTOTLKN"),
    "Services":           (["trade", "transp", "pubsvc"], "NVSRVTOTLKN"),
}
# "insbu": INSBU's rebased supply-use tables (2026-09-29), the authoritative
# accounts and the basis the SAM was built on; four sectors incl. manufacturing
ISIC_MAP_INSBU = {
    "Agriculture":        (["agr"],                       "VA_agr"),
    "Industry":           (["ind", "construc"],           "VA_ind"),
    "Manufacturing":      (["manuf"],                     "VA_man"),
    "Services":           (["trade", "transp", "pubsvc"], "VA_srv"),
    "Mining (B)":         (["min"],                       "VA_B05"),
}
# "min" (mining alone, a-min) is added to the workbook's sector groups inside
# model_series
SECTOR_SOURCE = "mfmod"          # module default; runs may override
ISIC_MAP = ISIC_MAP_MFMOD
# INSBU demand-side volumes matched to the model's comparison rows. Private
# consumption is households only (the model's QH; NPISH is ~3% of GDP and
# sits in the model's NGO account). GDP is value added (the model pins and
# reports RGDPFC, GDP at factor cost).
INSBU_DEMAND = {
    "Real PrvCon growth %":  "HHC",
    "Real GovCon growth %":  "GOVC",
    "Real GFCF growth %":    "GFCF",
    "Real exports growth %": "EXP",
    "Real imports growth %": "IMP",
}
# MFMod demand-side volumes (NIA-Vol, growth %) matched to model aggregates
MFMOD_DEMAND = {
    "Real PrvCon growth %":  "NECONPRVTKN",
    "Real GovCon growth %":  "NECONGOVTKN",
    "Real GFCF growth %":    "NEGDIFTOTKN",
    "Real exports growth %": "NEEXPGNFSKN",
    "Real imports growth %": "NEIMPGNFSKN",
    "Real GDP growth %":     "NYGDPMKTPKN",
}


def set_sector_source(source):
    """Switch the sector reference ("mfmod" | "wdi") for targets and rows."""
    global SECTOR_SOURCE, ISIC_MAP
    if source not in ("insbu", "mfmod", "wdi"):
        raise ValueError(source)
    SECTOR_SOURCE = source
    ISIC_MAP = {"insbu": ISIC_MAP_INSBU, "mfmod": ISIC_MAP_MFMOD,
                "wdi": ISIC_MAP_WDI}[source]


def _hold(series, t, through):
    """Value at t; beyond `through`, the last actual, held flat."""
    if t <= through and t in series:
        return series[t]
    return series[through]


EXOG_EXPORTS = ("c-agr",)           # coffee/tea sit in c-agr
# Gold is NOT on the export pin. Pinning c-min exports under CET2 makes domestic
# sales the residual of output minus exports, against a near-Leontief domestic
# demand no price can stretch: PDS(c-min) hit zero at 2021 with the resource
# idle and again at 2022 with the endowment scaled. Gold is supply-constrained,
# so the lever is the RESOURCE (set_resource_supply) and exports are whatever
# output exceeds domestic use, at the world price -- the ordinary CET.


IDLE_FACTORS = ("f-nrmin",)          # the mining resource, when gold is pinned


def gold_exports_bop_implied(act, through="2024"):
    """Mining (gold) exports in USD mn, implied by the balance of payments.

    Burundi-reported customs gold (HS 7108) is known to be under-declared --
    partner-reported gold runs up to twice as high -- but the mirror series is
    erratic and has no 2024. The BoP goods total (TG03, BRB-compiled, the same
    series the current account rests on) carries BRB's own estimate of
    unrecorded exports. So: BoP goods exports minus everything in customs
    that is NOT mining = the mining exports the BoP believes in.

    Customs totals stop at 2023; for later years the non-mining customs value
    is scaled by the BoP goods total, which is the convention Kodzovi used for
    the 2024-25 commodity rows. Only the SHAPE of this series is used (it is
    indexed to 2019 and applied to the model's own base-year export), so its
    level relative to the customs figure does not matter.
    """
    bop = act.targets["TG03"]
    cmin = act.trade[("c-min", "exports")]
    cust = act.trade_ref.get(("TOTAL", "Merchandise (customs, Comtrade)"), {})
    out, last_nongold, last_t = {}, None, None
    for t in sorted(bop):
        if t > through:
            break
        if t in cust and t in cmin:
            nongold = cust[t] - cmin[t]
            last_nongold, last_t = nongold, t
        elif last_nongold is not None:
            nongold = last_nongold * bop[t] / bop[last_t]
        else:
            continue
        out[t] = bop[t] - nongold
    return out


GOLD_SOURCE = "output"   # "output" | "bop" | "customs"


def mining_output_index(act, through="2024"):
    """Real mining value added, 2019 = 1: the Sectors sheet's a-min nominal VA
    deflated by the GDP deflator (TG01 nominal GDP over the IN04 real path).

    This, not any export series, is what the model should see for gold.
    Recorded gold exports fell ~45% over 2019-24 while mining output rose ~20%:
    the gold was mined and left, unrecorded -- beyond even BRB's BoP estimate.
    In the model c-min is near-wholly exported, so pinning exports to output
    (constant export share) keeps the resource employed and avoids the corner
    in which output cannot fall because a-min's capital is sector-specific and
    the linear CET2 dumps the surplus on a domestic gold market that barely
    exists (PDS(c-min) -> 0 at 2021 with the resource idle).
    """
    va = act.sectors["ind"]          # the a-min row carries the C-E group total
    va = {t: v for t, v in act.sectors["ind"].items()}
    # the a-min row itself is what the Sectors sheet placed there for mining:
    va = act.sectors["ind"]
    ngdp, g = act.targets["TG01"], act.inputs["IN04"]
    years = [t for t in sorted(va) if t <= through and t in ngdp and t in g]
    real_idx, defl, out = 1.0, 1.0, {}
    y0 = years[0]
    for i, t in enumerate(years):
        if i:
            real_idx *= 1 + g[t] / 100.0
            defl = (ngdp[t] / ngdp[y0]) / real_idx
        out[t] = (va[t] / va[y0]) / defl
    return out


def export_volume_index(act, c, through="2024", gold_source=GOLD_SOURCE):
    """Observed real export volume of commodity c, 2019 = 1.

    For c-min, `gold_source` picks the series: "output" (default; real mining
    VA, see `mining_output_index`), "bop" (BoP-implied gold USD deflated by
    IN07) or "customs" (Burundi-reported). For everything else the Trade
    sheet's customs value deflated by IN07. IN07 tracks gold closely but
    understates coffee's rise (ICO composite ~2.3x by 2024 against 1.72), so
    for c-agr this index is if anything too HIGH -- coffee alone, by volume
    (TG19b), fell to 0.30 of 2019 against this measure's 0.53.
    """
    if c == "c-min" and gold_source == "output":
        return mining_output_index(act, through)
    val = (gold_exports_bop_implied(act, through) if c == "c-min"
           else act.trade[(c, "exports")])
    px = act.inputs["IN07"]
    base = val["2019"] / px["2019"]
    return {t: (val[t] / px[t]) / base for t in val if t <= through and t in px}


def set_resource_supply(cal, act, factor="f-nrmin", through="2024", index=None):
    """Scale a resource endowment by the mining-output index: `qfinsb(ins,f,t)
    = QFINS0 * index(t)` for every holder, held flat beyond `through`.

    Pinning gold exports to output while the endowment stays fixed makes the
    idle share go NEGATIVE once output exceeds 2019 (it reaches 1.20 by 2024):
    over-employment the closure permits arithmetically but that means nothing.
    The coherent reading is that mining capacity grew with output -- new
    sites, more artisanal extraction -- so the endowment follows the same
    index. The idle closure then only bites in 2020, when output dipped.
    """
    idx = mining_output_index(act, through) if index is None else index
    T = cal.db.sets["t"]
    n = 0
    for (ins, f, t) in list(cal.QFINS0):
        if f == factor:
            cal.QFINS0[(ins, f, t)] *= _hold(idx, t, through)
            n += 1
    return {f"qfinsb({factor}) index": {t: round(idx[t], 3) for t in sorted(idx)},
            f"  ({n} endowment cells scaled)": ""}


MINING_INDEX_SOURCES = ("insbu-b05", "unsd", "mfmod-ind", "flat")


def mining_index(act, source="unsd", through="2024"):
    """Real mining-output index, 2019 = 1, for the resource endowment path.
    "insbu-b05": INSBU real value added of extraction (branch B05): -14% in
    2020, -14% in 2024; "unsd": the Sectors sheet's mining+utilities nominal
    VA deflated by the GDP deflator (jumps +37% in 2021); "mfmod-ind":
    MFMod's real Industry growth (no mining split); "flat": 1 throughout."""
    if source == "insbu-b05":
        from .insbu import volume_index
        return volume_index(act.insbu, "VA_B05", through)
    if source == "unsd":
        return mining_output_index(act, through)
    if source == "mfmod-ind":
        return mfmod_volume_index(act, "NVINDTOTLKN", through)
    if source == "flat":
        return {t: 1.0 for t in YEARS if t <= through}
    raise ValueError(source)


def set_idle_factors(cal, idle=IDLE_FACTORS):
    """Put the named factors on the idle-resource closure (solver.py)."""
    cal.idle_factors = set(idle or ())
    return {"idle_factors": sorted(cal.idle_factors)}


# ---------------------------------------------------------------------------
# sector supply shocks: back group productivity out of observed VA growth
# ---------------------------------------------------------------------------

# Four mutually exclusive groups covering all 19 activities, matched to the
# real-growth series the workbook carries (WDI, ISIC): A-B, D, G-P, and C-F
# less D. Pinning a group's VA growth replaces the aggregate GDP pin of the
# calibration pass: GDP becomes what the sector paths add up to (SAM weights),
# so the model's GDP growth turns into a check.
VA_GROUPS = {
    "agr":   ["a-agr"],
    "manuf": list(SECTOR_GROUPS["manuf"]["members"]),
    "oind":  ["a-min", "a-elect", "a-water", "a-construc"],
    "serv":  ["a-trade", "a-hotelrest", "a-transp", "a-admpub", "a-edu",
              "a-health", "a-oser"],
}
VA_GROUPS["ind"] = VA_GROUPS["manuf"] + VA_GROUPS["oind"]   # ISIC B-F "Industry"
VA_GROUPS["min"] = ["a-min"]                                 # ISIC B, mining alone
VA_GROUP_LABEL = {"min": "Mining (B)", "agr": "Agriculture", "manuf": "Manufacturing (D)",
                  "oind": "Other industry (C-F less D)", "ind": "Industry",
                  "serv": "Services"}
# Default targets: agriculture and industry, the two MFMod sectors whose
# growth is supply-side in nature; services left to the model as the check
# of its demand side. The WDI-era alternatives were dropped (pinning services
# was infeasible against a -13% farm year; the "oind" residual was unusable);
# with the MFMod series the 2020 farm collapse is gone.
DEFAULT_TARGET_GROUPS = ("agr", "ind")


def sector_growth_targets(act, through="2024", groups=DEFAULT_TARGET_GROUPS,
                          source=None):
    """Observed real VA growth ratio (1 + g/100) per targeted group and year.

    source "mfmod": agr / ind / serv from NIA-Vol (no manufacturing split).
    source "wdi": A-B, D and G-P from the workbook's WDI block; C-F less D
    derived with previous-year nominal weights (constant-price shares are not
    in the workbook).
    """
    source = source or SECTOR_SOURCE
    out = {}
    if source == "insbu":
        code = {"agr": "VA_agr", "ind": "VA_ind", "manuf": "VA_man",
                "oind": "VA_oind", "serv": "VA_srv", "min": "VA_B05"}
        for grp in groups:
            if grp not in code:
                raise ValueError(f"INSBU has no series for group {grp!r}; "
                                 f"use {list(code)}")
            gser = act.insbu.growth[code[grp]]
            for t in YEARS:
                if "2019" < t <= through and t in gser:
                    out[(grp, t)] = 1 + gser[t] / 100.0
        return out
    if source == "mfmod":
        m = act.mfmod
        if not m:
            raise RuntimeError("MFMod datasheet not loaded (data/mfmod-*.xlsx)")
        code = {"agr": "NVAGRTOTLKN", "ind": "NVINDTOTLKN", "serv": "NVSRVTOTLKN"}
        for grp in groups:
            if grp not in code:
                raise ValueError(f"MFMod has no series for group {grp!r}; "
                                 f"use {list(code)} or --sector-source wdi")
            for t in YEARS:
                if "2019" < t <= through and t in m[code[grp]]:
                    out[(grp, t)] = 1 + m[code[grp]][t] / 100.0
        return out
    g = {k[1]: v for k, v in act.sector_ref.items() if k[0] == "growth"}
    src = {"agr": g["Agriculture (ISIC A-B)"], "manuf": g["Manufacturing (ISIC D)"],
           "serv": g["Services (ISIC G-P)"], "ind": g["Industry (ISIC C-F)"]}
    gCF, gD = g["Industry (ISIC C-F)"], g["Manufacturing (ISIC D)"]
    nom = act.sectors
    for t in YEARS:
        if not ("2019" < t <= through) or t not in gCF:
            continue
        tp = str(int(t) - 1)
        for grp in groups:
            if grp in src:
                out[(grp, t)] = 1 + src[grp][t] / 100.0
            elif grp == "oind":
                cf = nom["ind"][tp] + nom["manuf"][tp] + nom["construc"][tp]
                d = nom["manuf"][tp]
                out[(grp, t)] = 1 + (gCF[t] * cf - gD[t] * d) / (cf - d) / 100.0
            else:
                raise ValueError(f"no growth series for group {grp!r}")
    return out


def scale_armington(db, factors):
    """Multiply the Armington elasticity `sigma_q` of the named commodities
    BEFORE calibration, so the CES share and shift parameters are rebuilt on
    the new curvature and the base year stays exact. `factors`: {c: k}.
    Sensitivity tool for the import response to a domestic supply shock."""
    tl = db.pars["tradelas"]
    out = {}
    for c, k in factors.items():
        key = (c, "sigma_q")
        if key not in tl:
            raise KeyError(f"no sigma_q for {c!r}")
        tl[key] *= k
        out[c] = round(tl[key], 3)
    return {"sigma_q scaled": out}


def set_sector_targets(cal, act, through="2024", groups=DEFAULT_TARGET_GROUPS,
                       pin_gdp=False, start="2020"):
    """Declare the productivity groups on `cal` and pin their VA growth.

    Sets `cal.tfp_groups` (activity -> group, targeted groups only),
    `cal.tfp_group_list` and `cal.va_target` {(group, t): ratio}; model.py
    then multiplies TFP by TFPGRP(g) for those activities and adds the
    GRPVADEF / GRPHOLD equations. Activities outside every targeted group
    keep the GAMS TFP definition exactly.

    `pin_gdp=False` (default): TFPSCAL stays at zero and real GDP is an
    OUTCOME -- the pinned sectors plus whatever the model's demand side does
    with the rest. `pin_gdp=True` keeps the calibration pass's aggregate pin
    as well (GDP follows IN04 through the economy-wide TFPSCAL, the group
    shifters compensate inside the targeted sectors). It reads well --
    "the aggregate path is known; within it, these sectors had supply
    shocks" -- but it does not solve: with agriculture down 13% and GDP
    flat, the untargeted sectors (services, mostly) must grow ~10% in 2020
    through productivity, which is the services-pinning failure by another
    route (prices collapse, fallback at ~1e-2). Kept as an option.
    """
    A = cal.db.sets["a"]
    groups = tuple(groups)
    unknown = [g for g in groups if g not in VA_GROUPS]
    if unknown:
        raise ValueError(f"unknown VA group(s) {unknown}; have {list(VA_GROUPS)}")
    seen = {}
    for g in groups:
        for a in VA_GROUPS[g]:
            if a in seen:
                raise ValueError(f"{a} is in both {seen[a]!r} and {g!r}; "
                                 "targeted groups must not overlap (use "
                                 "'manuf' + 'oind' instead of 'ind')")
            seen[a] = g
    assign = {a: g for g in groups for a in VA_GROUPS[g]}
    untargeted = [a for a in A if a not in assign]
    if pin_gdp and not untargeted:
        raise ValueError("pin_gdp needs at least one untargeted activity")
    cal.tfp_group_list = groups
    cal.tfp_groups = assign
    # `start`: first targeted year. Earlier years keep the aggregate
    # calibration (GDP pinned, TFPSCAL flexes); from `start` the sector pins
    # take over and TFPSCAL is held at its last solved value (dynamics.py).
    cal.va_target = {k: v for k, v in sector_growth_targets(act, through, groups).items()
                     if k[1] >= start}
    cal.sector_pin_gdp = bool(pin_gdp)
    return {"sector VA growth pinned (%)":
            {g: {t: round(100 * (r - 1), 1)
                 for (gg, t), r in sorted(cal.va_target.items()) if gg == g}
             for g in groups},
            "  untargeted activities": untargeted,
            "  aggregate GDP pin kept": bool(pin_gdp)}


GOVCON_SOURCES = ("insbu-real", "insbu", "budget", "na")


def set_real_govcon(db):
    """Put government consumption on spending rule 1 (real quantity fixed,
    `QG = qgb`), before calibration. The path itself is attached afterwards
    by `set_real_govcon_path`. Rule 1 is what the paper's scenarios use; the
    base data have rule 2 (GDP share)."""
    db.pars.setdefault("govspndrule0", {})[("congov",)] = 1.0
    return {"govspndrule0(congov)": "1 (real path)"}


def set_real_govcon_path(cal, act, through="2024"):
    """`qgb(c,t) = QG00(c) x INSBU real government consumption index`,
    held flat in growth terms beyond `through` (grows with GDP)."""
    from .insbu import volume_index
    idx = volume_index(act.insbu, "GOVC", through)
    T = cal.db.sets["t"]
    last = max(idx)
    q = {}
    for t in T:
        if t not in cal.gdpindex:
            continue
        k = idx[t] if t in idx else idx[last] * cal.gdpindex[t] / cal.gdpindex[last]
        for c, q00 in cal.QG00.items():
            q[(c, t)] = q00 * k
    cal.qgb0 = q
    return {"qgb (real gov consumption index, INSBU)": {t: round(v, 3) for t, v in idx.items()}}


REXR_SOURCES = ("fixed", "actual")


def actual_rexr_index(act, through="2024"):
    """Real official exchange rate, 2019 = 1, as the model defines it:
    `REXR = EXR / DPI` (EQ_REXRDEF; world prices enter separately through
    pwmindex/pweindex). Official rate IN01 over the GDP deflator implied by
    nominal GDP (TG01) and real growth (IN04). Burundi's official rate
    appreciated about 24% in real terms over 2019-24 (nominal +56% against a
    deflator +106%); the parallel rate, official x premium (IN03), was
    roughly flat in real terms."""
    e, ngdp, g = act.inputs["IN01"], act.targets["TG01"], act.inputs["IN04"]
    real, out = 1.0, {}
    for t in YEARS:
        if t > through or t not in e or t not in ngdp:
            continue
        if t > "2019":
            real *= 1 + g[t] / 100.0
        defl = (ngdp[t] / ngdp["2019"]) / real
        out[t] = (e[t] / e["2019"]) / defl
    return out


MACRO_BASES = ("insbu", "workbook", "mfmod")


def insbu_rescale(act):
    """Factor that moves a workbook ratio (on the WDI GDP) onto INSBU's GDP:
    WDI nominal GDP over INSBU nominal GDP, 0.67 in 2019 to 0.73 in 2024.
    The workbook's fiscal and BoP ratios are levels over the WDI GDP, and the
    levels themselves (BIF) agree across sources -- only the GDP differs."""
    ins, T = act.insbu, act.targets
    return {t: T["TG01"][t] / ins.cur["GDP"][t]
            for t in YEARS if t in T["TG01"] and t in ins.cur["GDP"]}


def mfmod_gdp_ratio(act):
    """MFMod nominal GDP over the workbook's (WDI, rebased) nominal GDP, by
    year: 0.90 in 2019 falling to 0.71 in 2024. The workbook's fiscal and
    balance-of-payments ratios are MFMod's, rescaled by this ratio to the
    rebased GDP (verified exact for TG07, TG08, TG10, IN08, IN10-IN13)."""
    m, T = act.mfmod, act.targets
    return {t: m["NYGDPMKTPCN"][t] / 1000.0 / T["TG01"][t]
            for t in YEARS if t in m["NYGDPMKTPCN"] and t in T["TG01"]}


def mfmod_deflator(act, through="2024"):
    """MFMod GDP deflator, 2019 = 1 (NYGDPMKTPXN growth): 1.65 by 2024,
    against 2.06 for the deflator implied by the workbook's rebased nominal
    GDP and 2.11 for the CPI."""
    return mfmod_volume_index(act, "NYGDPMKTPXN", through)


TRADE_PRICE_SOURCES = ("keyfitz", "implicit")


def insbu_trade_price_index(act, flow, through="2024"):
    """Realised trade price in USD, 2019 = 1, from INSBU: the export or
    import deflator of the supply-use tables over the official rate. Imports
    +2% by 2024, exports +25%."""
    d = act.insbu.defl["EXP" if flow == "exports" else "IMP"]
    e = act.inputs["IN01"]
    return {t: d[t] / (e[t] / e["2019"]) for t in d
            if "2019" <= t <= through and t in e}


def implicit_trade_price_index(act, flow, through="2024"):
    """Burundi's realised trade price in USD, 2019 = 1: MFMod's export or
    import deflator in BIF (NEEXPGNFSXN / NEIMPGNFSXN, cumulated) over the
    official-rate index (IN01). By construction volume x this price = the
    nominal BoP value, so it is the price consistent with the MFMod volumes
    the backcast compares against. Over 2019-24 it gives exports +15% and
    imports -3%, against the Keyfitz world-price proxies (IN07 +72%, IN06
    +20%) the workbook carries."""
    code = {"exports": "NEEXPGNFSXN", "imports": "NEIMPGNFSXN"}[flow]
    d = mfmod_volume_index(act, code, through)
    e = act.inputs["IN01"]
    return {t: d[t] / (e[t] / e["2019"]) for t in d if t in e}


def apply_actuals(db, act, through="2024", exog_exports=EXOG_EXPORTS,
                  govcon_source="budget", rexr_source="fixed",
                  basis="workbook", trade_prices="keyfitz", rowclos="data",
                  investment="budget", premium="actual"):
    """Overwrite the 2020-`through` exogenous paths with actuals. Returns a
    dict describing what was set, for the run log.

    `exog_exports` names the commodities put on the quantity closure
    (`cesexog`): their export volume is pinned to the observed path instead of
    being chosen by the CET at world prices. Must be set before `calibrate`;
    the volume path itself is attached afterwards by `set_export_volumes`,
    because it is scaled to the calibrated base-year export `qeb00`.

    `govcon_source` picks the path for government consumption (share of GDP,
    `govspndgdp0(congov)`): "budget" is the fiscal-data ratio IN08 (MFMod,
    wages + goods and services; 9.6% of GDP in 2019, close to the SAM's 9.9%
    and falling to 7.6-8.7% through 2024); "na" keeps the 2019 level but moves
    it with the NATIONAL-ACCOUNTS government consumption share TG13 (UNSD; 20.3
    to 23.4% of GDP in 2020, then flat). The two disagree on both level and
    direction. The national-accounts measure is the one consistent with the
    sector VA growth targets -- the 2020 services growth those record is
    largely public services -- so it is the right companion for the sector
    backcast; the budget measure is the right one for the fiscal balance.
    """
    P, T, C = db.pars, db.sets["t"], db.sets["c"]
    log = {}
    # `basis` picks the GDP the ratios and growth rates sit on. "workbook":
    # the actuals workbook as filled -- real GDP growth from WDI (rebased),
    # fiscal and BoP ratios rescaled to the rebased nominal GDP, the deflator
    # implied by that GDP. "mfmod": everything on MFMod's own accounts -- its
    # real GDP growth (differs from WDI by up to 0.6 pp in 2021-23), its
    # deflator, and its fiscal/BoP ratios un-rescaled -- the basis the sector
    # and demand-side reference series are on. The two disagree most on the
    # price level (deflator 1.65 vs 2.06 by 2024).
    # "insbu" (default when the files are present): INSBU's rebased accounts
    # -- real value-added growth (the model pins GDP at factor cost), the
    # GDP deflator, and the workbook's fiscal/BoP ratios moved onto INSBU's
    # GDP (`insbu_rescale`).
    if basis not in MACRO_BASES:
        raise ValueError(f"basis must be one of {MACRO_BASES}")
    if basis == "mfmod" and not act.mfmod:
        raise RuntimeError("basis='mfmod' needs the MFMod datasheet")
    if basis == "insbu" and act.insbu is None:
        raise RuntimeError("basis='insbu' needs INSBU's accounts (raw-insbu/ or the extract in data/)")
    ratio = mfmod_gdp_ratio(act) if basis == "mfmod" else {}
    wdi2insbu = insbu_rescale(act) if basis == "insbu" else {}
    if exog_exports:
        db.sets["cesexog"] = list(exog_exports)
        log["cesexog"] = list(exog_exports)

    gsrc = {"mfmod": act.mfmod.get("NYGDPMKTPKN") if act.mfmod else None,
            "insbu": act.insbu.growth["VA"] if act.insbu is not None else None,
            "workbook": act.inputs["IN04"]}[basis]
    for t in T:
        if "2020" <= t <= through:
            P["gdpgrw"][(t,)] = gsrc[t] / 100.0
    log["gdpgrw " + {"mfmod": "[MFMod GDP]", "insbu": "[INSBU real VA]",
                     "workbook": "[IN04]"}[basis]] = {
        t: round(P["gdpgrw"][(t,)], 5) for t in T if "2020" <= t <= through}

    b = act.inputs["IN03"]["2019"]
    if premium == "flat":
        # sensitivity: the premium stays at its 2019 level -- what the model
        # would see if the non-fuel imports it charges at the parallel rate
        # had in fact cleared at the official rate after 2019
        P["prexrindex0"] = {(t,): 1.0 for t in T}
    else:
        P["prexrindex0"] = {(t,): _hold(act.inputs["IN03"], t, through) / b for t in T}
    log["prexrindex0" + (" [flat: sensitivity]" if premium == "flat" else "")] = {
        t: round(P["prexrindex0"][(t,)], 4) for t in T if t <= through}

    # Real official exchange rate. Under rowclos 2 (2019-2027) REXR is an
    # exogenous path, `REXR0 = REXR00 * rexrindex0`; the authors' sheet is
    # empty (flat). "actual" feeds the observed real appreciation of the
    # official rate, which is what let real imports grow 3.8%/yr while
    # world prices rose 20%: a fixed REXR makes import prices track domestic
    # prices plus the world-price rise plus the premium, and imports fall.
    if rexr_source not in REXR_SOURCES:
        raise ValueError(f"rexr_source must be one of {REXR_SOURCES}")
    if rexr_source == "actual":
        if basis in ("mfmod", "insbu"):
            e = act.inputs["IN01"]
            d = (mfmod_deflator(act, through) if basis == "mfmod" else
                 {t: v for t, v in act.insbu.defl["GDP"].items()
                  if "2019" <= t <= through})
            ridx = {t: (e[t] / e["2019"]) / d[t] for t in d if t in e}
        else:
            ridx = actual_rexr_index(act, through)
        P["rexrindex0"] = {(t,): _hold(ridx, t, through) for t in T}
        log[f"rexrindex0 [actual real official rate, {basis} deflator]"] = {
            t: round(P["rexrindex0"][(t,)], 4) for t in T if t <= through}

    b5, b6, b7 = (act.inputs[k]["2019"] for k in ("IN05", "IN06", "IN07"))
    if trade_prices not in TRADE_PRICE_SOURCES:
        raise ValueError(f"trade_prices must be one of {TRADE_PRICE_SOURCES}")
    if trade_prices == "implicit":
        # realised trade deflators in USD, one index for every commodity
        # (fuel keeps its customs unit value IN05); from INSBU on its basis,
        # otherwise from MFMod
        if basis == "insbu":
            pm = insbu_trade_price_index(act, "imports", through)
            px = insbu_trade_price_index(act, "exports", through)
        else:
            pm = implicit_trade_price_index(act, "imports", through)
            px = implicit_trade_price_index(act, "exports", through)
        P["pwmindex"] = {(c, t): (_hold(act.inputs["IN05"], t, through) / b5
                                  if c == "c-refpet" else _hold(pm, t, through))
                         for c in C for t in T}
        P["pweindex"] = {(c, t): _hold(px, t, through) for c in C for t in T}
        log["trade prices"] = (("INSBU" if basis == "insbu" else "MFMod")
                               + " implicit deflators in USD (fuel: IN05)")
    else:
        P["pwmindex"] = {(c, t): (_hold(act.inputs["IN05"], t, through) / b5
                                  if c == "c-refpet"
                                  else _hold(act.inputs["IN06"], t, through) / b6)
                         for c in C for t in T}
        # IN07 is MFMod's Keyfitz export price, an aggregate world price for
        # Burundi's basket (coffee, tea, gold). Applied to c-agr and c-min;
        # applying it to beer, textiles or cement hands them a 72% price rise
        # they never saw and manufactures an export boom in manufacturing.
        # Other goods take the general index (IN06).
        P["pweindex"] = {(c, t): (_hold(act.inputs["IN07"], t, through) / b7
                                  if c in ("c-agr", "c-min")
                                  else _hold(act.inputs["IN06"], t, through) / b6)
                         for c in C for t in T}
        log["trade prices"] = "Keyfitz world-price indices IN06/IN07 (fuel: IN05)"
    log["pwmindex c-refpet"] = {t: round(P["pwmindex"][("c-refpet", t)], 3)
                                for t in T if t <= through}
    log["pwmindex other"] = {t: round(P["pwmindex"][(C[0], t)], 3)
                             for t in T if t <= through}
    log["pweindex c-agr/c-min"] = {t: round(P["pweindex"][("c-min", t)], 3)
                                   for t in T if t <= through}
    log["pweindex other"] = {t: round(P["pweindex"][("c-food", t)], 3)
                             for t in T if t <= through}

    if govcon_source not in GOVCON_SOURCES:
        raise ValueError(f"govcon_source must be one of {GOVCON_SOURCES}")
    if govcon_source in ("insbu", "insbu-real") and act.insbu is None:
        raise RuntimeError(f"govcon_source={govcon_source!r} needs the INSBU files")
    for inid, (par, ac) in FISCAL_MAP.items():
        ser = act.inputs[inid]
        if basis == "mfmod":
            # undo the workbook's rescaling: back onto MFMod's own GDP
            ser = {t: v / ratio[t] for t, v in ser.items() if t in ratio}
        elif basis == "insbu":
            ser = {t: v * wdi2insbu[t] for t, v in ser.items() if t in wdi2insbu}
        if inid == "IN08" and govcon_source == "insbu-real":
            continue                      # real path set after calibration
        if inid == "IN08" and govcon_source == "insbu":
            # INSBU government final consumption share of GDP (10.6% in
            # 2019 -> 7.5% in 2024), applied as a proportional path to the
            # 2019 ratio so the base year is untouched
            na = act.insbu.share["GOVC"]
            ser = {t: act.inputs["IN08"]["2019"] * na[t] / na["2019"]
                   for t in na if t in act.inputs["IN08"]}
        elif inid == "IN08" and govcon_source == "na":
            if basis == "mfmod":
                # share path from MFMod's own nominal government consumption
                # against its nominal GDP (same +15% step in 2020 as TG13)
                m = act.mfmod
                g_c, g_y, idx, na = m["NECONGOVTCN"], m["NYGDPMKTPCN"], 1.0, {"2019": 1.0}
                for t in YEARS:
                    if t > "2019" and t in g_c and t in g_y:
                        idx *= (1 + g_c[t] / 100.0) / (g_y[t] / g_y[str(int(t) - 1)])
                        na[t] = idx
            else:
                na = act.targets["TG13"]
            ser = {t: ser["2019"] * na[t] / na["2019"] for t in na if t in ser}
        for t in T:
            if t <= through and t in ser:
                P.setdefault(par, {})[(ac, t)] = ser[t] / 100.0
        tag = ((" [NA share path]" if govcon_source == "na" else
                " [INSBU share path]" if govcon_source == "insbu" else "")
               if inid == "IN08" else "") \
            + {"mfmod": " [MFMod GDP]", "insbu": " [INSBU GDP]"}.get(basis, "")
        log[f"{par}({ac}){tag}"] = {t: round(ser[t] / 100.0, 5)
                                    for t in T if t <= through and t in ser}

    # Investment. Both public (govspndgdp0 f-capgov) and private (ngovpaygdp0
    # f-capprv) capital formation are rule-2 GDP-ratio paths. "insbu" moves
    # both with INSBU's total GFCF share of GDP (11.3% in 2019 -> 12.1% in
    # 2024), keeping the SAM's 2019 split: the TCEIs give the public share
    # only for 2021, 2023 and 2024, and the budget series IN09 falls where the
    # TCEI public investment rises. "budget" (old behaviour): public from
    # IN09, private left on the authors' path.
    if investment == "insbu":
        if act.insbu is None:
            raise RuntimeError("investment='insbu' needs the INSBU files")
        sh = act.insbu.share["GFCF"]
        for par, ac in (("govspndgdp0", "f-capgov"), ("ngovpaygdp0", "f-capprv")):
            base = P.get(par, {}).get((ac, "2019")) or 1.0
            for t in T:
                if t <= through and t in sh:
                    P.setdefault(par, {})[(ac, t)] = base * sh[t] / sh["2019"]
            log[f"{par}({ac}) [INSBU GFCF share path]"] = {
                t: round(sh[t] / sh["2019"], 3) for t in T if t <= through and t in sh}

    # Remittances and FDI are rule-2 accounts too -- exogenous GDP-ratio paths
    # that bind -- so they are inputs, not targets. Workbook basis: USD mn
    # over WDI nominal GDP at the official rate. MFMod basis: BoP remittance
    # inflows as MFMod reports them (% of its GDP); FDI from the workbook's
    # USD inflows over MFMod GDP (MFMod's own net FDI is ~0, a different
    # concept from the gross inflow the SAM account carries).
    for tgid, (par, ac) in (("TG17", ("ngovpaygdp0", "trrowngov")),
                            ("TG16", ("ngovpaygdp0", "fdi"))):
        ser, fx, ngdp = act.targets[tgid], act.inputs["IN01"], act.targets["TG01"]
        for t in T:
            if t <= through and t in ser and t in fx and t in ngdp:
                if basis == "insbu":
                    P.setdefault(par, {})[(ac, t)] = (ser[t] * fx[t] / 1000.0) / act.insbu.cur["GDP"][t]
                elif basis == "mfmod" and tgid == "TG17":
                    P.setdefault(par, {})[(ac, t)] = act.mfmod["BXFSTREMTCD"][t] / 100.0
                elif basis == "mfmod":
                    P.setdefault(par, {})[(ac, t)] = (ser[t] * fx[t] / 1000.0) / (ngdp[t] * ratio[t])
                else:
                    P.setdefault(par, {})[(ac, t)] = (ser[t] * fx[t] / 1000.0) / ngdp[t]
        log[f"{par}({ac})"] = {t: round(P[par][(ac, t)], 5)
                               for t in T if t <= through and (ac, t) in P[par]}

    for t in T:
        if t <= through:
            P.setdefault("gintrat0", {})[(t,)] = act.inputs["IN17"][t] / 100.0
            for i2 in ("govz", "ngovz"):
                P.setdefault("fintrat0", {})[(i2, t)] = act.inputs["IN18"][t] / 100.0

    # Rest-of-world closure. The dataset has rowclos 2 through 2027: real
    # official rate and premium exogenous, non-government net foreign
    # financing (NFFINS) clears. `rowclos="1"` flips 2020-through to rowclos
    # 1: NFFINS on its rule-2 GDP-ratio path, real rate free. The path is
    # the SAM's 2019 ratio moved by the change in the actual current account
    # net of the government and FDI inflows that are fed separately, so
    # total foreign financing moves as the actual current account moved.
    if rowclos == "1":
        ca = act.targets["TG07"]
        gov, fdi = act.inputs["IN13"], {}
        for t in YEARS:
            if t in act.targets["TG16"] and t in act.inputs["IN01"] and t in act.targets["TG01"]:
                fdi[t] = 100 * (act.targets["TG16"][t] * act.inputs["IN01"][t] / 1000.0) / act.targets["TG01"][t]
        base = P["ngovpaygdp0"].get(("netforfinngov", "2019"))
        if base is None:
            raise RuntimeError("ngovpaygdp0(netforfinngov, 2019) not in the data")
        path = {}
        for t in T:
            if "2020" <= t <= through and t in ca:
                d_inflow = -(ca[t] - ca["2019"])          # deficit narrows -> inflows fall
                d_ngov = d_inflow - (gov[t] - gov["2019"]) - (fdi[t] - fdi["2019"])
                path[t] = base + d_ngov / 100.0
                P["ngovpaygdp0"][("netforfinngov", t)] = path[t]
                P["rowclos0"][(t,)] = 1.0
        log["rowclos0 -> 1 (real rate free)"] = sorted(path)
        log["ngovpaygdp0(netforfinngov) [actual CA path]"] = {
            t: round(v, 4) for t, v in path.items()}
    return log


def mfmod_volume_index(act, code, through="2024"):
    """Cumulative volume index, 2019 = 1, from an MFMod growth series."""
    g = act.mfmod[code]
    idx, out = 1.0, {"2019": 1.0}
    for t in YEARS:
        if "2019" < t <= through and t in g:
            idx *= 1 + g[t] / 100.0
            out[t] = idx
    return out


def all_exporters(db, exclude=("c-min",)):
    """Commodities with base-year exports, less `exclude` (gold stays on the
    CET: pinning it corners the domestic gold market, see EXOG_EXPORTS)."""
    return tuple(c for c in db.sets["c"]
                 if db.sam(c, "row") > 0 and c not in exclude)


def set_export_volumes(cal, act, through="2024", exog_exports=EXOG_EXPORTS,
                       aggregate=False, source="mfmod"):
    """Attach the observed volume path to a calibrated model: `qeb(c,t) =
    qeb00(c) * index(t)`, held flat beyond `through`.

    `aggregate=True` gives every pinned commodity the same index -- real
    exports of goods and services from INSBU (`source="insbu"`) or MFMod
    (NEEXPGNFSKN) -- instead of its own customs-based one: the "total real
    exports as observed" closure."""
    T = cal.db.sets["t"]
    cal.qeb_path = {}
    log = {}
    agg = None
    if aggregate and source == "insbu":
        from .insbu import volume_index
        agg = volume_index(act.insbu, "EXP", through)
    elif aggregate:
        agg = mfmod_volume_index(act, "NEEXPGNFSKN", through)
    if agg:
        log[f"qeb index (all pinned, {source} real exports GNFS)"] = {
            t: round(v, 3) for t, v in agg.items()}
    for c in exog_exports or ():
        idx = agg if agg else export_volume_index(act, c, through)
        for t in T:
            k = _hold(idx, t, through)
            cal.qeb_path[(c, t)] = cal.qeb00[c] * k
        if not agg:
            log[f"qeb({c}) index"] = {t: round(idx[t], 3) for t in sorted(idx)}
        if c == "c-min":
            log["gold source"] = GOLD_SOURCE
            for lab, ser in (("customs", export_volume_index(act, c, through, "customs")),
                             ("bop-implied", export_volume_index(act, c, through, "bop")),
                             ("mining output", mining_output_index(act, through))):
                log[f"  gold idx {lab}"] = {t: round(v, 3) for t, v in ser.items()}
    return log


def set_quota_path(cal, through="2024", fuel_volumes=True, chem_volumes=True):
    """Import-quota ceilings from BRB's observed import volumes.

    The authors' data freeze the ceilings on fuel (c-refpet) and chemicals
    (c-chemplast) at 2019 volumes through 2025 (`qmbarindex0` = 1), so the
    model can never import more of either -- 28.6% of imports -- whatever
    happens. BRB's customs tonnages show fuel +22% by 2022 then -24% by 2024
    (the fuel crisis), fertiliser and chemicals nearly doubling
    (`gemcore.brb.volume_index`). The ceiling becomes the observed volume:
    where demand at that ceiling is lower the complementarity sweep makes the
    quota slack; where it is higher the rent measures the shortage. Beyond
    `through` the authors' growth path resumes from the last observed level.

    With the fuel channels (`cal.fuel.fx_quota`), the fuel ceiling after
    `through` is BRB's foreign-exchange allocation over the world price, the
    allocation carried at the share of official foreign exchange that the
    last observed volume implies (fuel.py, channel 4). `fuel_volumes=False`
    leaves fuel on that rule from 2020, at the 2019 share: fuel imports are
    then what foreign-exchange availability alone allows. The same applies
    to chemicals and fertiliser, the other official-rate imports
    (`chem_volumes`), since 2026-09-30.
    """
    from .brb import QUOTA_GROUPS, volume_index
    log = {}
    fcfg = getattr(cal, "fuel", None)
    fx = bool(fcfg and fcfg.fx_quota)
    for c, keys in QUOTA_GROUPS.items():
        if (c, "2019") not in cal.qmbar0:
            continue
        on_fx = fx and c in fcfg.fx_goods
        if on_fx:
            observed = fuel_volumes if c == fcfg.fuel else chem_volumes
            if not observed:
                fcfg.fx_from[c] = "2020"
                log[f"quota ceiling {c}"] = (
                    f"foreign-exchange rule from 2020, {fcfg.fx_share00[c]:.3f} of official FX (2019 share)")
                continue
            fcfg.fx_from[c] = str(int(through) + 1)
        idx = volume_index(keys)
        base = cal.qmbar0[(c, "2019")]
        last = max(y for y in idx if y <= through)
        old_last = cal.qmbar0[(c, last)] / base
        for (cc, t) in list(cal.qmbar0):
            if cc != c or t <= "2019":
                continue
            if t in idx and t <= through:
                cal.qmbar0[(c, t)] = base * idx[t]
            elif t > last:
                cal.qmbar0[(c, t)] = cal.qmbar0[(c, t)] * idx[last] / old_last
        log[f"quota ceiling {c} (BRB volume, 2019 = 1)"] = {
            y: round(v, 3) for y, v in idx.items() if y <= through}
        if on_fx:
            log[f"quota ceiling {c} after {through}"] = (
                "foreign-exchange rule, share carried from the last observed year")
    return log


def set_stock_path(cal, act, through="2024", goods=None):
    """Feed INSBU's change in inventories by commodity.

    The model's stock demand is exogenous and grows with GDP (`qdstk00 *
    gdpindex`, mod.gms 874). INSBU records inventories rising from 0.3% of
    GDP in 2019 to 2.4-4.4% in 2020-24, all in domestic products: farm
    produce (A01-A04 -> c-agr), processed food (C06 -> c-food) and
    extraction (B05 -> c-min, 1.65% of GDP in 2024). The SAM's own 2019
    stocks (1.2% of GDP, mostly c-agr) are kept; what is added, per
    commodity, is the change in INSBU's stock share since 2019, as a real
    quantity: delta_share(t) x real GDP(t) at base prices / base price.
    Stored as `cal.qdstk_add[(c, t)]`, read by `Closure.qdstk` for the
    non-government investor.
    """
    from .insbu import STOCK_MAP
    ins = act.insbu
    T = cal.db.sets["t"]
    gdp00 = cal.GDPMP00
    add, log = {}, {}
    for c, share in ins.stock_share.items():
        if c not in cal.db.sets["c"] or (goods is not None and c not in goods):
            continue
        price = cal.PQD00.get((c, "dstk")) or 1.0     # stock-demand price, 2019
        d = {}
        for t in T:
            if "2019" < t and t in cal.gdpindex:
                dsh = _hold(share, t, through) - share["2019"]
                add[(c, t)] = dsh / 100.0 * gdp00 * cal.gdpindex[t] / price
                if t <= through:
                    d[t] = round(dsh, 2)
        log[f"stock change added, {c} (pp of GDP vs 2019)"] = d
    cal.qdstk_add = add
    return log


# ---------------------------------------------------------------------------
# reducing a solved path and the actuals to comparable indicators
# ---------------------------------------------------------------------------

def model_series(cal, sols, years):
    """Indicators from the model, per year. Shares in %, growth in %/yr,
    indices 2019 = 100."""
    from .state import Closure
    S = cal.db.sets
    A, C, H, G, F = S["a"], S["c"], S["h"], S["insgov"], S["f"]
    FLAB, IN2, FCAP = S["flab"], S["ins2"], S["fcap"]
    clo = Closure(cal)
    out = {}

    grp_members = {g: d["members"] for g, d in SECTOR_GROUPS.items()}
    grp_members["min"] = ["a-min"]

    def realva(V, members):
        return sum(cal.PVA00[a] * V["QA"][a]
                   for g in members for a in grp_members[g])

    prev = None
    for t in years:
        V = sols[t]
        gdp = V["GDPMP"][t]
        exr = V["EXR"][t]
        row = {}
        # nominal shares of GDP
        row["PrvCon %GDP"] = 100 * sum(V["PQD"][(c, h)] * V["QH"][(c, h)]
                                       for c in C for h in H
                                       if (c, h) in V["QH"]) / gdp
        row["GovCon %GDP"] = 100 * sum(V["PQD"][(c, g)] * V["QG"][c]
                                       for c in C for g in G
                                       if (c, g) in V["PQD"] and c in V["QG"]) / gdp
        row["GFCF %GDP"] = 100 * sum(V["PK"][fc] * V["DKINS"].get((i2, fc), 0.0)
                                     for i2 in IN2 for fc in FCAP) / gdp
        row["Exports %GDP"] = 100 * sum(V["PWE"][c] * exr * V["QE"][c]
                                        for c in C if c in V["QE"]) / gdp
        row["Imports %GDP"] = 100 * sum(V["PWM"][c] * exr * V["QM"][c]
                                        for c in C if c in V["QM"]) / gdp
        row["Stocks %GDP"] = 100 * sum(
            V["PQD"].get((c, "dstk"), 0.0) * clo.qdstk(c, i2, t)
            for c in C for i2 in IN2) / gdp
        row["CA balance %GDP"] = -100 * V["SAVF"][t] / gdp
        row["Ext public debt %GDP"] = 100 * V["FDEBT"]["govz"] * exr / gdp
        row["Gov domestic debt %GDP"] = 100 * V["GDEBT"][t] / gdp
        qfs = {f: V["QFS"][f] for f in FLAB if f in V["QFS"]}
        row["Unemployment %"] = 100 * sum(V["UERAT"][f] * qfs[f] for f in qfs) / sum(qfs.values())
        # relative price: CPI against the GDP deflator
        row["CPI/deflator idx"] = V["CPI"][t] / (gdp / V["RGDPMP"][t])
        fcfg = getattr(cal, "fuel", None)
        if fcfg is not None:
            fu = fcfg.fuel
            qmi = V.get("QMI", {}).get(fu, 0.0)
            row["Fuel: informal share of supply %"] = 100 * qmi / (V["QM"][fu] + qmi)
            pump = ((1 + V["TM"][fu]) * V["PWM"][fu] * V["EXR"][t]
                    + V["PM"][fu] - (1 + V["TM"][fu] + V["PRQMBAR"][fu])
                    * V["PWM"][fu] * V["EXR"][t])
            row["Fuel: market price / pump price"] = 100 * V["PM"][fu] / pump
            if fcfg.fx_quota:
                from .fuel import official_fx, sol_getter
                ofx = official_fx(cal, sol_getter(V), t)
                ofx0 = official_fx(cal, sol_getter(sols[years[0]]), years[0])
                row["Fuel: share of official FX %"] = (
                    100 * V["PWM"][fu] * V["QM"][fu] / ofx)
                if "c-chemplast" in fcfg.fx_goods:
                    row["Chemicals: share of official FX %"] = (
                        100 * V["PWM"]["c-chemplast"] * V["QM"]["c-chemplast"] / ofx)
                row["Official FX inflow idx (USD)"] = 100 * ofx / ofx0
        # levels needed for growth
        row["_rva"] = {g: realva(V, [g]) for g in grp_members}
        row["_rexp"] = sum(cal.PWE00.get(c, 0.0) * cal.EXR00 * V["QE"][c] for c in C if c in V["QE"])
        row["_rimp"] = sum(cal.PWM00.get(c, 0.0) * cal.EXR00 * V["QM"][c] for c in C if c in V["QM"])
        row["_rgdp"] = V["RGDPFC"][t]
        # real demand aggregates at base-year prices
        row["_rprv"] = sum(cal.PQD00.get((c, h), 0.0) * V["QH"][(c, h)]
                           for c in C for h in H if (c, h) in V["QH"])
        row["_rgov"] = sum(cal.PQD00.get((c, g), 0.0) * V["QG"][c]
                           for c in C for g in G if c in V["QG"] and (c, g) in cal.PQD00)
        row["_rinv"] = sum(cal.PK00.get(fc, 0.0) * V["DKINS"].get((i2, fc), 0.0)
                           for i2 in IN2 for fc in FCAP)
        for g in getattr(cal, "tfp_group_list", ()):
            row[f"TFP shifter {VA_GROUP_LABEL.get(g, g)}"] = 100 * V["TFPGRP"][g]
        if prev is not None:
            for lab, (groups, _) in ISIC_MAP.items():
                a = sum(prev["_rva"][g] for g in groups)
                b = sum(row["_rva"][g] for g in groups)
                row[f"{lab} real VA growth %"] = 100 * (b / a - 1)
            row["Real exports growth %"] = 100 * (row["_rexp"] / prev["_rexp"] - 1)
            row["Real imports growth %"] = 100 * (row["_rimp"] / prev["_rimp"] - 1)
            row["Real GDP growth %"] = 100 * (row["_rgdp"] / prev["_rgdp"] - 1)
            row["Real PrvCon growth %"] = 100 * (row["_rprv"] / prev["_rprv"] - 1)
            row["Real GovCon growth %"] = 100 * (row["_rgov"] / prev["_rgov"] - 1)
            row["Real GFCF growth %"] = 100 * (row["_rinv"] / prev["_rinv"] - 1)
        out[t] = row
        prev = row
    # CPI/deflator as an index on 2019
    base = out[years[0]]["CPI/deflator idx"]
    for t in years:
        out[t]["CPI/deflator idx"] = 100 * out[t]["CPI/deflator idx"] / base
    return out


def actual_series(act, years, basis="workbook"):
    """The same indicators from the actuals workbook (and, on the MFMod
    basis, the BoP and fiscal ratios on MFMod's own GDP; the UNSD demand
    shares have no MFMod counterpart and stay as they are)."""
    I, T = act.inputs, act.targets
    if basis == "insbu":
        return _actual_series_insbu(act, years)
    mb = act.mfmod if basis == "mfmod" else {}
    out = {t: {} for t in years}
    g = {k[1]: v for k, v in act.sector_ref.items() if k[0] == "growth"}
    for t in years:
        r = out[t]
        r["PrvCon %GDP"] = T["TG12"].get(t)
        r["GovCon %GDP"] = T["TG13"].get(t)
        r["GFCF %GDP"] = T["TG11"].get(t)
        if mb:
            r["Exports %GDP"] = mb["BXGSRGNFSCD"].get(t)
            r["Imports %GDP"] = (mb["BMGSRMRCHCD"].get(t, 0) + mb["BMGSRNFSVCD"].get(t, 0)) or None
            r["CA balance %GDP"] = mb["BNCABFUNDCD"].get(t)
            r["Ext public debt %GDP"] = mb["GGDBTEXTLCN"].get(t)
            r["Gov domestic debt %GDP"] = mb["GGDBTDOMTCN"].get(t)
        else:
            r["Exports %GDP"] = T["TG21"].get(t)
            r["Imports %GDP"] = T["TG22"].get(t)
            r["CA balance %GDP"] = T["TG07"].get(t)
            r["Ext public debt %GDP"] = T["TG08"].get(t)
            r["Gov domestic debt %GDP"] = T["TG10"].get(t)
        r["Unemployment %"] = T["TG14"].get(t)
        m = act.mfmod if SECTOR_SOURCE == "mfmod" else {}
        for lab, (_, src) in ISIC_MAP.items():
            if t != years[0]:
                r[f"{lab} real VA growth %"] = (m.get(src, {}).get(t) if m
                                                else g.get(src, {}).get(t))
        r["Real GDP growth %"] = ((act.mfmod["NYGDPMKTPKN"].get(t) if basis == "mfmod"
                                   else I["IN04"].get(t)) if t != years[0] else None)
        if m and t != years[0]:
            for key, code in MFMOD_DEMAND.items():
                if key != "Real GDP growth %" and t in m.get(code, {}):
                    r[key] = m[code][t]
    if not (act.mfmod and SECTOR_SOURCE == "mfmod"):
        # real trade growth: BoP USD deflated by the price indices
        for i, t in enumerate(years[1:], start=1):
            tp = years[i - 1]
            for key, tg, px in (("Real exports growth %", "TG04", "IN07"),
                                ("Real imports growth %", "TG06", "IN06")):
                if all(k in T[tg] for k in (t, tp)):
                    real_t = T[tg][t] / I[px][t]
                    real_p = T[tg][tp] / I[px][tp]
                    out[t][key] = 100 * (real_t / real_p - 1)
    # CPI relative to the GDP deflator, index 2019 = 100
    idx = 100.0
    out[years[0]]["CPI/deflator idx"] = idx
    for i, t in enumerate(years[1:], start=1):
        tp = years[i - 1]
        if mb:
            defl = 1 + mb["NYGDPMKTPXN"][t] / 100.0
        else:
            ngdp = T["TG01"][t] / T["TG01"][tp]
            defl = ngdp / (1 + I["IN04"][t] / 100.0)
        cpi = 1 + T["TG02"][t] / 100.0
        idx *= cpi / defl
        out[t]["CPI/deflator idx"] = idx
    return out


def _actual_series_insbu(act, years):
    """Targets on INSBU's rebased accounts.

    Shares of GDP and real growth from the supply-use tables (private
    consumption = households; real GDP = real value added, to match the
    model's RGDPFC). Current account and debt ratios: the workbook's BIF
    levels (MFMod-sourced, which agree with INSBU's TCEI current account
    in level) over INSBU's GDP. CPI relative to INSBU's GDP deflator.
    Unemployment as in the workbook."""
    ins, I, T = act.insbu, act.inputs, act.targets
    f = insbu_rescale(act)
    out = {t: {} for t in years}
    for t in years:
        r = out[t]
        r["PrvCon %GDP"] = ins.share["HHC"].get(t)
        r["GovCon %GDP"] = ins.share["GOVC"].get(t)
        r["GFCF %GDP"] = ins.share["GFCF"].get(t)
        r["Stocks %GDP"] = ins.share["STK"].get(t)
        r["Exports %GDP"] = ins.share["EXP"].get(t)
        r["Imports %GDP"] = ins.share["IMP"].get(t)
        for key, tg in (("CA balance %GDP", "TG07"), ("Ext public debt %GDP", "TG08"),
                        ("Gov domestic debt %GDP", "TG10")):
            r[key] = T[tg][t] * f[t] if t in T[tg] and t in f else None
        r["Unemployment %"] = T["TG14"].get(t)
        if t == "2024":
            from .fuel import BLACK_MARKET_RATIO_2024
            r["Fuel: market price / pump price"] = 100 * BLACK_MARKET_RATIO_2024
        if t == years[0]:
            continue
        for lab, (_, code) in ISIC_MAP_INSBU.items():
            r[f"{lab} real VA growth %"] = ins.growth[code].get(t)
        for key, code in INSBU_DEMAND.items():
            r[key] = ins.growth[code].get(t)
        r["Real GDP growth %"] = ins.growth["VA"].get(t)
    idx = 100.0
    out[years[0]]["CPI/deflator idx"] = idx
    for i, t in enumerate(years[1:], start=1):
        tp = years[i - 1]
        defl = ins.defl["GDP"][t] / ins.defl["GDP"][tp]
        idx *= (1 + T["TG02"][t] / 100.0) / defl
        out[t]["CPI/deflator idx"] = idx
    return out


SHARES = ["PrvCon %GDP", "GovCon %GDP", "GFCF %GDP", "Stocks %GDP", "Exports %GDP",
          "Imports %GDP", "CA balance %GDP", "Ext public debt %GDP",
          "Gov domestic debt %GDP", "Unemployment %"]
def growth_rows():
    return ([f"{lab} real VA growth %" for lab in ISIC_MAP]
            + ["Real PrvCon growth %", "Real GovCon growth %",
               "Real GFCF growth %", "Real exports growth %",
               "Real imports growth %", "Real GDP growth %"])


GROWTH = growth_rows()
INDICES = (["CPI/deflator idx", "Fuel: informal share of supply %",
            "Fuel: market price / pump price", "Fuel: share of official FX %",
            "Chemicals: share of official FX %",
            "Official FX inflow idx (USD)"]
           + [f"TFP shifter {lab}" for lab in VA_GROUP_LABEL.values()])


def compare(model, actual, years):
    """Rows of (indicator, kind, {year: (model, actual)}, summary).

    For shares the summary is the 2019->last pp change, model vs actual, and
    the 2019 base-year gap. For growth it is the average annual rate over
    2020..last. For indices, the last-year value."""
    rows = []
    y0, yN = years[0], years[-1]

    def pair(k, t):
        return (model[t].get(k), actual[t].get(k))

    for k in SHARES:
        cells = {t: pair(k, t) for t in years}
        m0, a0 = cells[y0]; mN, aN = cells[yN]
        summ = {"base gap 2019": (m0 - a0) if (m0 is not None and a0 is not None) else None,
                "model chg": (mN - m0) if (mN is not None and m0 is not None) else None,
                "actual chg": (aN - a0) if (aN is not None and a0 is not None) else None}
        rows.append((k, "share", cells, summ))
    for k in growth_rows():
        cells = {t: pair(k, t) for t in years[1:]}
        if all(v[1] is None for v in cells.values()) and all(v[0] is None for v in cells.values()):
            continue
        mv = [v[0] for v in cells.values() if v[0] is not None]
        av = [v[1] for v in cells.values() if v[1] is not None]
        summ = {"model avg": sum(mv) / len(mv) if mv else None,
                "actual avg": sum(av) / len(av) if av else None}
        rows.append((k, "growth", cells, summ))
    for k in INDICES:
        cells = {t: pair(k, t) for t in years}
        if all(v == (None, None) for v in cells.values()):
            continue                      # e.g. TFP shifters when no targets
        summ = {"model last": cells[yN][0], "actual last": cells[yN][1]}
        rows.append((k, "index", cells, summ))
    return rows
