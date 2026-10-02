"""Declared equation-to-variable pairing for the within-period system.

The period system is square: each equation determines one variable. Which
variable that is is a modelling statement, not something to be discovered, so it
is written down here rather than left to the bipartite matcher in
`partition.py`. The matcher stays on as a fall-back for the handful of rows
whose pairing is genuinely arbitrary, and as the check that the declared
pairing really is a perfect matching on a non-singular Jacobian.

Two things make this a *preference list* per equation rather than a single pair:

1. The natural partner depends on the closure. `EQ_WFADEF` reads
   `WFA = WF*WFDIST*(1+TFA)`; it determines `WFDIST` for a factor whose
   sector-specific rental is endogenous (private capital under the dynamic
   closure, land under facclos 2, the labour nest aggregates), and `WFA` for a
   factor whose `WFDIST` the closure pins (facclos 4 labour). Same equation,
   different partner, decided by `base_closure_fixed`.
2. A degenerate cell can knock out the first choice. `EQ_COMEQ` clears the
   commodity market and normally sets the domestic supply price `PDS`, but for a
   commodity with no domestic output it has to fall through to imports.

So each entry lists candidates in order of economic preference and the first one
that is free and unclaimed wins. Anything that exhausts its list falls through
to the matcher, which is the honest outcome for rows like `EQ_DPIDEF`, whose own
variable is the numeraire and is therefore fixed: with `DPI` pinned the equation
becomes a restriction on the domestic price level and no single price owns it.

`GOVRECGDPDEF` and its siblings are handled specially: under rule 1 the account's
GDP-ratio variable is endogenous and owns the row, while under rule 2 the ratio
is a target and the row is owned by the scaling variable that hits it (a transfer
scalar, `RNDFG`, `NFFG`, `QGSCAL`, `ISCAL`, `FDISCAL`, `MPSSCAL`,
`NFFINSSCAL`). The candidate list is built from the same rule maps the closure
uses, so the two cannot drift apart.
"""

from __future__ import annotations


# macclos.inc pairs each government-receipt / government-spending /
# non-government-payment account with ONE scaling variable. Rule 1 fixes that
# scaling variable and lets the account's GDP-ratio and absolute-level report
# variables float; rule 2 fixes the GDP ratio; rule 3 fixes the absolute level.
# Accounts absent from these maps have no LOOP in macclos.inc (trgovngov,
# trrowngov, the cssoc-* social-security accounts), so nothing is fixed for them
# whatever their rule says. A `None` index means the variable is period-indexed.
#
# `solver.base_closure_fixed` reads these to decide what to fix, and the
# GDP-ratio rows below read them to decide who owns the row. Keeping both on the
# same maps is what stops the closure and the pairing from drifting apart.
_GOVREC_SCAL = {
    "trgovrow": ("TRNSFRSCAL", "trgovrow"),
    "tax-dir": ("TYSCAL", None),
    "tax-act": ("TASCAL", None),
    "tax-com": ("TQSCAL", None),
    "tax-exp": ("TESCAL", None),
    "tax-imp": ("TMSCAL", None),
    "netforfingov": ("NFFG", None),
    "netdomfin": ("RNDFG", None),
}
_GOVSPND_SCAL = {
    "trngovgov": ("TRNSFRSCAL", "trngovgov"),
    "trrowgov": ("TRNSFRSCAL", "trrowgov"),
    "congov": ("QGSCAL", None),
    "f-capgov": ("ISCAL", "f-capgov"),
}
_NGOVPAY_SCAL = {
    "trngovrow": ("TRNSFRSCAL", "trngovrow"),
    "trfacrow": ("TRNSFRSCAL", "trfacrow"),
    "trrowfac": ("TRNSFRSCAL", "trrowfac"),
    "savngov": ("MPSSCAL", None),
    "netforfinngov": ("NFFINSSCAL", None),
    "f-capprv": ("ISCAL", "f-capprv"),
    "fdi": ("FDISCAL", None),
    "tourismrec": ("TRSMREC", None),
}

# index selectors -----------------------------------------------------------
# Each takes the equation key's index part (a tuple, possibly empty) and the
# period label, and returns the variable's key.


def SAME(idx, t):
    """Variable is indexed exactly like the equation."""
    return idx[0] if len(idx) == 1 else idx


def PERIOD(idx, t):
    """Scalar variable, indexed by the period."""
    return t


def _with(*extra):
    """Equation index extended by fixed labels, e.g. YPRQMBAR(c, 'ngovz')."""
    def sel(idx, t):
        return tuple(idx) + extra
    return sel


# Rows whose own left-hand-side variable is a closure target, so the row is a
# restriction the matcher has to route through an augmenting path rather than a
# missing declaration. DPIDEF sets the numeraire under numeraire=2, GDPREALFCDEF
# holds the GDP target under dcal01, LABPARTRATDEF pins labour-force
# participation while QLABSCAL (which does not appear in it) flexes.
CLOSURE_PINNED_EQS = {"DPIDEF", "GDPREALFCDEF", "LABPARTRATDEF", "CPIDEF"}


# Equations that carry no partner because the stock they define is
# predetermined from the previous period (see dynamics.predetermined_stocks).
# They are expected to be dropped, and are not a defect.
PREDETERMINED_EQS = {
    "CAPACCUMNGOVDOM", "CAPACCUMNGOVHHD", "CAPACCUMNGOVFOR", "CAPACCUMGOV",
    "CAPACCUMACT", "CAPREDIST", "CAPREDISTCONST", "GOVDOMDEBT", "FORDEBT",
}


# equation family -> ordered candidate list of (variable family, selector)
PAIRING = {
    # ---- production and the factor nest ----------------------------------
    # Every entry leads with the variable on the left-hand side of the GAMS
    # equation, which is the author's own statement of what it determines.
    # WFADEF reads `WFA = WF*WFDIST*(1+TFA)`, so it owns WFA -- except for a
    # factor whose WFA is already claimed by its demand equation, where it falls
    # through to the sector-specific rental WFDIST instead.
    "PRODFN":       [("QA", SAME)],
    "FACDEM":       [("WFA", SAME)],
    "FACDEMLEO":    [("QF", SAME)],
    "PRODFNCES2":   [("QF", SAME)],
    "FACDEMCES2":   [("WFA", SAME), ("QF", SAME)],
    "PRODFNCES3":   [("QF", SAME)],
    "FACDEMCES3":   [("QF", SAME)],
    "TFPDEF":       [("TFP", SAME)],
    # port-only sector-group productivity shifter (model.py, backcast targets)
    "GRPVADEF":     [("TFPGRP", SAME)],
    "GRPHOLD":      [("TFPGRP", SAME)],
    "FPRDADEF":     [("FPRDA", SAME)],
    "INTDEM":       [("QINT", SAME)],
    # port-only fuel mechanisms (gemcore/fuel.py)
    "FUELSUB":      [("FXI", SAME)],
    "FUELCES":      [("VXI", SAME)],
    "INFARB":       [("QMI", SAME)],
    "HHFUELSUB":    [("HXI", SAME)],
    "PVADEF":       [("PVA", SAME)],
    "PADEF":        [("PA", SAME)],
    "VATREBATEDEF": [("RBTVAT", SAME)],

    # ---- output aggregation across activities ----------------------------
    "COMPRDFN":     [("QXAC", SAME)],
    "OUTAGGFOC":    [("PXAC", SAME)],
    "OUTAGGFN":     [("QX", SAME)],
    "OUTVAL":       [("PX", SAME), ("PDS", SAME)],

    # ---- factor markets --------------------------------------------------
    # FACEQ is the market-clearing condition: it sets the economy-wide wage
    # where that is endogenous, and the supply where the wage is pinned
    # (facclos 2). FACSUP aggregates institutional endowments into supply.
    "WAGECURVE":    [("WF", SAME), ("UERAT", SAME)],
    "FACEQ":        [("UERAT", SAME), ("QFS", SAME)],
    "FACSUP":       [("QFS", SAME)],
    "WFAVGDEF":     [("WFAVG", SAME)],
    "YFDEF":        [("YF", SAME)],
    "YCAPGDEF":     [("YF", SAME)],
    "LABENDOWDEF":  [("QFINS", SAME)],
    "OTHENDOWDEF":  [("QFINS", SAME)],
    "LABPARTRATDEF": [("LABPARTRAT", PERIOD)],
    "SHIFDEF":      [("SHIF", SAME)],

    # ---- trade and the domestic commodity market -------------------------
    # Armington sets the composite quantity, IMPDOMRAT the import share, CET the
    # domestic/export split, and COMEQ clears the market on the domestic price.
    "ARMING":       [("QQ", SAME)],
    "ARMING2":      [("QQ", SAME), ("QD", SAME), ("QE", SAME)],
    "IMPDOMRAT":    [("QM", SAME)],
    "CET":          [("QD", SAME), ("QX", SAME)],
    "CET2":         [("QD", SAME), ("QE", SAME), ("QX", SAME)],
    "EXPDOMRAT":    [("QE", SAME)],
    # Quantity closure for a `cesexog` commodity: EQ_CET and EQ_EXPDOMRAT are
    # dropped, EQ_CET2 (linear) takes QD, and this row pins the export volume.
    "ESUPPLYEXOG":  [("QE", SAME)],
    "COMEQ":        [("QQ", SAME), ("QM", SAME)],
    "ABSORB":       [("PQS", SAME)],
    "PDDDEF":       [("PDD", SAME)],
    "PQDDEF":       [("PQD", SAME)],
    "PMDEF":        [("PM", SAME)],
    "PEDEF":        [("PE", SAME)],
    "QTDEM":        [("QT", SAME)],
    "INVDEM":       [("QINV", SAME)],
    "GOVDEM":       [("QG", SAME)],
    "HHDDEM":       [("QH", SAME)],

    # ---- import quota rent (EQ_QMCONST.PRQMBAR is an explicit GAMS pair) --
    "QMCONST":          [("QM", SAME)],
    "IMPQUOTARENT":     [("YPRQMBART", SAME)],
    "HHDIMPQUOTARENT":  [("YPRQMBAR", SAME)],
    "ACTIMPQUOTARENT":  [("YPRQMBAR", SAME)],
    "GOVIMPQUOTARENT":  [("YPRQMBAR", _with("gov"))],
    "INVIMPQUOTARENT":  [("YPRQMBAR", _with("ngovz"))],

    # ---- exchange-rate premium rent --------------------------------------
    "FOREXRENT":        [("YPREXRT", SAME)],
    "FOREXRENTALLOC":   [("YPREXR", SAME)],

    # ---- institutions ----------------------------------------------------
    "YIFDEF":       [("YIF", SAME)],
    "YIDEF":        [("YI", SAME)],
    "MPSDEF":       [("MPS", SAME)],
    "INSSAVDEF":    [("SAV", SAME)],
    "TRIIDEF":      [("TRII", SAME)],
    "EHDEF":        [("EH", SAME)],
    "TRHROWDEF":    [("TRNSFR", SAME)],
    "TRFACROWDEF":  [("TRNSFR", SAME)],
    "TRHGOVDEF":    [("TRNSFR", SAME)],
    "TRGOVROWDEF":  [("TRNSFR", SAME)],

    # ---- tax rates -------------------------------------------------------
    "TYDEF":        [("TY", SAME)],
    "TFDEF":        [("TF", SAME)],
    "TADEF":        [("TA", SAME)],
    "TQDEF":        [("TQ", SAME)],
    "TEDEF":        [("TE", SAME)],
    "TMDEF":        [("TM", SAME)],
    "TFADEF":       [("TFA", SAME)],

    # ---- government ------------------------------------------------------
    "GOVREV":           [("YG", PERIOD)],
    "GOVEXP":           [("EG", PERIOD)],
    "YTAXEXPDEF":       [("YTAXEXP", PERIOD)],
    "YTARIMPDEF":       [("YTAXIMP", PERIOD)],
    "YTAXVATDEF":       [("YTAXVAT", PERIOD)],
    "SUBCTDEF":         [("SUBCT", PERIOD)],
    "GOVPRIMDEF":       [("GPRIMDEF", PERIOD)],
    "GOVPRIMDEFREALDEF": [("RGPRIMDEF", PERIOD)],
    "GOVINVCOST":       [("INVVALG", PERIOD)],
    "GOVNETDOMFINREAL": [("RNDFG", PERIOD), ("NDFG", PERIOD)],
    "GOVDOMBOR":        [("GBOR", PERIOD)],
    "GOVCAPACC":        [("GPRIMDEF", PERIOD), ("NDFG", PERIOD), ("GBOR", PERIOD)],
    "GOVFORBOR":        [("FBOR", SAME)],
    "NGOVFORBOR":       [("FBOR", SAME)],

    # ---- savings-investment and the rest of the world --------------------
    # NGOVINVFIN is the savings-investment balance, so under siclos 2 it is
    # owned by the savings-rate scalar that clears it. NGOVNETFORFIN owns
    # NFFINSSCAL, which occurs nowhere else in the model.
    "NGOVINVCOST":   [("INVVAL", PERIOD)],
    "NGOVINVFIN":    [("INVVAL", PERIOD), ("MPSSCAL", PERIOD)],
    "NGOVNETFORFIN": [("NFFINS", PERIOD), ("NFFINSSCAL", PERIOD)],
    "INVVALFDEF":    [("INVVALF", PERIOD)],
    "CURACC":        [("SAVF", PERIOD)],
    "CAPACC":        [("SAVF", PERIOD), ("WALRAS", PERIOD)],
    "DKGOVDEF":      [("DKINS", SAME)],
    "DKNGOVDEF":     [("DKINS", SAME)],
    "DKROWDEF":      [("DKINS", SAME)],
    "PCAPDEF":       [("PK", SAME)],
    "NEWCAPALLOC":   [("DKA", SAME)],

    # ---- price indices and macro aggregates ------------------------------
    # DPIDEF has no candidate: DPI is the numeraire under numeraire=2, so the
    # row is a restriction on the price level that no single price owns. It is
    # left to the matcher deliberately.
    "CPIDEF":        [("CPI", PERIOD)],
    "REXRDEF":       [("REXR", PERIOD), ("EXR", PERIOD)],
    "GDPMPDEF":      [("GDPMP", PERIOD)],
    "GDPREALMPDEF":  [("RGDPMP", PERIOD)],
    "GDPPCREALDEF":  [("RGDPPC", PERIOD)],
    "GDPREALFCDEF":  [("RGDPFC", PERIOD)],
    "TRDGDPDEF":     [("TRDGDP", PERIOD)],
    "ABSNOMDEF":     [("ABSNOM", PERIOD)],
    "GOVRECABSDEF":  [("GOVRECABS", SAME)],
    "GOVSPNDABSDEF": [("GOVSPNDABS", SAME)],
    "NGOVPAYABSDEF": [("NGOVPAYABS", SAME)],
}


# The GDP-ratio rows: own variable first, then the scaling variable the closure
# frees when that ratio is a target instead of an outcome.
_RULE_ROWS = {
    "GOVRECGDPDEF":  ("GOVRECGDP", _GOVREC_SCAL),
    "GOVSPNDGDPDEF": ("GOVSPNDGDP", _GOVSPND_SCAL),
    "NGOVPAYGDPDEF": ("NGOVPAYGDP", _NGOVPAY_SCAL),
}


def _wfadef_pair(idx, t, fixed):
    """EQ_WFADEF reads WFA = WF*WFDIST*(1+TFA), and which side it determines is
    decided by the factor-market closure. Where the sector-specific rental is
    endogenous -- private capital under the dynamic closure, land under
    facclos 2, the labour nest aggregates outside the SAM -- the wage WFA is
    already set by that factor's own demand equation and this row delivers
    WFDIST. Where facclos pins WFDIST (mobile labour under facclos 4) it
    delivers WFA instead."""
    return ([("WFDIST", idx)] if ("WFDIST", idx) not in fixed
            else [("WFA", idx)])


def _dkins_pair(idx, t, fixed):
    """EQ_DKGOVDEF / EQ_DKNGOVDEF read

        DKINS(ins2,fcap) = dkinsb*ISCAL(fcap) * (1 + IADJ(ins2)*iadj01(fcap))

    and normally determine DKINS. Under siclos = 1, though, the investment
    adjuster IADJ(ins2) is freed to clear savings-investment, and it appears
    NOWHERE ELSE in the model -- this row is its only equation, so it has no
    alternative. DKINS has several other homes (EQ_INVDEM, EQ_NEWCAPALLOC,
    EQ_NGOVINVCOST, the capital accumulation rows), so when IADJ is free the row
    belongs to IADJ and DKINS is reached by an augmenting path.

    Getting this wrong is silent rather than loud: the row goes to DKINS, IADJ
    is left unmatched and frozen at zero, and nothing then closes the
    savings-investment balance -- WALRAS absorbs it and the solve produces
    numbers instead of an error.
    """
    i2 = idx[0]
    if ("IADJ", i2) not in fixed:
        return [("IADJ", i2), ("DKINS", idx)]
    return [("DKINS", idx)]


# equation family -> callable(idx, t, fixed) -> candidate list, for the rows
# whose partner depends on what the closure fixed.
PAIRING_FN = {"WFADEF": _wfadef_pair,
              "DKGOVDEF": _dkins_pair,
              "DKNGOVDEF": _dkins_pair}


def candidates(eqkey, t, fixed=frozenset()):
    """Ordered (name, idx) candidates for the variable this equation determines.

    An empty list means the pairing is deliberately left to the matcher.
    """
    fam, idx = eqkey[0], eqkey[1:]

    fn = PAIRING_FN.get(fam)
    if fn is not None:
        return fn(idx[0] if len(idx) == 1 else idx, t, fixed)

    if fam in _RULE_ROWS:
        varfam, scalmap = _RULE_ROWS[fam]
        ac = idx[0]
        out = [(varfam, ac)]
        ent = scalmap.get(ac)
        if ent is not None:
            nm, key = ent
            out.append((nm, t if key is None else key))
        return out

    return [(nm, sel(idx, t)) for nm, sel in PAIRING.get(fam, ())]


def declared_pairs(eqkeys, V, fixed, t):
    """Resolve the declaration against this period's state and closure.

    Returns (pairs, unpaired, report) where `pairs` maps equation key ->
    (name, idx), `unpaired` lists the equation keys whose candidates were all
    unavailable, and `report` counts why candidates were rejected. Assignment is
    first-come-first-served in `eqkeys` order; a variable is never handed to two
    equations.
    """
    claimed = set()
    pairs = {}
    unpaired = []
    report = {"missing": 0, "fixed": 0, "taken": 0, "undeclared": 0,
              "predetermined": 0, "closure_pinned": 0}

    for k in eqkeys:
        cands = candidates(k, t, fixed)
        if not cands:
            if k[0] in PREDETERMINED_EQS:
                report["predetermined"] += 1
            elif k[0] in CLOSURE_PINNED_EQS:
                report["closure_pinned"] += 1
            else:
                report["undeclared"] += 1
            unpaired.append(k)
            continue
        for nm, idx in cands:
            if nm not in V or idx not in V[nm]:
                report["missing"] += 1
                continue
            if (nm, idx) in fixed:
                report["fixed"] += 1
                report.setdefault("fixed_list", []).append((k, (nm, idx)))
                continue
            if (nm, idx) in claimed:
                report["taken"] += 1
                continue
            claimed.add((nm, idx))
            pairs[k] = (nm, idx)
            break
        else:
            unpaired.append(k)

    return pairs, unpaired, report
