"""GEM-Core base-year calibration: port of mod.gms lines ~20-1965.

Produces a `Calib` object with base-year ("00") parameters, exogenous time
paths ("0" suffixed, keyed by (…, t)), and calibrated function parameters
(CES/CET/LES shares & shifts). All checks in the GAMS code are replicated
as assertions.

Conventions:
  - 1-dim params: dict[str -> float]; 2-dim: dict[(str, str) -> float]; etc.
  - Missing key == 0.0 (GAMS sparse semantics): always use .get / G().
  - Only dmod in {0,1} supported (bdi2019 uses dmod=1); dmod=2 (steady
    state) branches are not ported.
"""

from __future__ import annotations

from collections import defaultdict
from types import SimpleNamespace

TOL = 1e-6


def G(par, *key):
    """Sparse getter: G(par, i, j) == par[(i,j)] or 0."""
    if len(key) == 1:
        return par.get(key[0], 0.0)
    return par.get(key, 0.0)


def sd():
    return defaultdict(float)


_DEFAULT = object()


# Share of the parallel-market premium rent dissipated in unproductive
# activity (queuing, informal intermediation, rent-seeking) rather than
# transferred: Krueger (1974). Reduced form in model.py EQ_TFPDEF. Not in
# GEM-Core; 0 reproduces it.
RENT_COST_DEFAULT = 0.25
# ...and the resources leave rent-seeking gradually: a third of the gap
# between the effective and the current rent share closes each year.
RENT_ADJUST_DEFAULT = 1.0 / 3.0


def calibrate(db, fuel_mech=_DEFAULT, rent_cost=_DEFAULT) -> SimpleNamespace:
    """Base-year calibration. `fuel_mech`: the fuel-shortage mechanisms
    (gemcore/fuel.py) -- the model's defaults unless given; None removes them
    (only for re-checking the port against the reference application).
    `rent_cost`: share of the premium rent that is a real resource cost
    (RENT_COST_DEFAULT unless given; 0 when `fuel_mech` is None, since that is
    the GAMS-replication mode, which removes every addition)."""
    S, M, P = db.sets, db.maps, db.pars
    sam = defaultdict(float, P["sam"])

    A, C, F, H, T = S["a"], S["c"], S["f"], S["h"], S["t"]
    TSOL = S["tsol"]
    tmin = S["tmin"][0]
    tmax = S["tmax"][0]
    INS, INSD, INSDNG = S["ins"], S["insd"], S["insdng"]
    INSGOV, INSROW = S["insgov"], S["insrow"]
    INSNGO, INSTRST = S["insngo"], S["instrst"]
    FCAP, FCAPG, FCAPNG = S["fcap"], S["fcapg"], S["fcapng"]
    FLAB, FSAM, FNSAM, FVA = S["flab"], S["fsam"], S["fnsam"], S["fva"]
    F1, F2, F3 = S["f1"], S["f2"], S["f3"]
    FLEO = set(S.get("fleo", []))
    FNCAP = S["fncap"]
    CT = S["ct"]
    CMBAR = S["cmbar"]
    CED, CESEXOG = set(S["ced"]), set(S["cesexog"])
    ACNT = S["acnt"]
    dmod = G(P.get("dmod", {}), ())
    assert dmod in (0.0, 1.0), "only dmod 0/1 ported"

    mf2f1 = M["mf2f1"]          # (f2, f1)
    mf3f2 = M["mf3f2"]          # (f3, f2)
    mfcapinv = M["mfcapinv"]    # (fcap, inv)
    mcapins = M["mcapins"]      # (capins, ins)
    mtaxfa = M["mtaxfa"]        # (taxfacact, f)
    msubcom = M.get("msubcom", [])   # (c, demander)
    mtaxvatc = M.get("mtaxvatc", [])

    def samtot_col(j):
        return sum(v for (i, jj), v in sam.items() if jj == j)

    def samtot_row(i):
        return sum(v for (ii, j), v in sam.items() if ii == i)

    cal = SimpleNamespace()
    cal.db = db

    # ------------------------------------------------------------------
    # Growth rates and indices (lines 22-92)
    # ------------------------------------------------------------------
    ACPOP = S["acpop"]
    pop0 = defaultdict(float, P["pop0"])
    popgrw = sd()
    for ap in ACPOP:
        for i, tt in enumerate(TSOL):
            if tt != tmin and pop0[(ap, TSOL[i - 1])]:
                popgrw[(ap, tt)] = pop0[(ap, tt)] / pop0[(ap, TSOL[i - 1])] - 1
    pop00 = {ap: pop0[(ap, tmin)] for ap in ACPOP}
    pop = dict(pop0)

    gdpgrw = defaultdict(float, {k[0]: v for k, v in P["gdpgrw"].items()})
    qfacgrw = defaultdict(float, P.get("qfacgrw", {}))
    fprdgrw = defaultdict(float, P.get("fprdgrw", {}))
    govspndgrw0 = defaultdict(float, P.get("govspndgrw0", {}))
    govrecgrw0 = defaultdict(float, P.get("govrecgrw0", {}))
    ngovpaygrw0 = defaultdict(float, P.get("ngovpaygrw0", {}))

    ACGOVREC, ACGOVSPND, ACNGOVPAY = (S["acgovrec"], S["acgovspnd"],
                                      S["acngovpay"])
    # default growth = gdp growth when a rule has no data at all (lines 63-65)
    for acg, grw in ((ACGOVSPND, govspndgrw0), (ACGOVREC, govrecgrw0),
                     (ACNGOVPAY, ngovpaygrw0)):
        for ac in acg:
            if not any(k[0] == ac for k in grw.keys()):
                for tt in TSOL:
                    grw[(ac, tt)] = gdpgrw[tt]

    # qfacgrw for labor from qlabins0 if empty (lines 68-69): qlabins0 empty
    # for bdi2019 -- skipped when no data.
    qlabins0 = defaultdict(float, P.get("qlabins0", {}))
    if not any(qfacgrw.values()) and any(qlabins0.values()):
        for i, tt in enumerate(TSOL[1:], start=1):
            for f in FLAB:
                prev = sum(qlabins0[(ins, f, TSOL[i - 1])] for ins in INS)
                cur = sum(qlabins0[(ins, f, tt)] for ins in INS)
                if prev:
                    qfacgrw[(f, tt)] = cur / prev - 1

    gdpindex = {tmin: 1.0}
    popindex = {(ap, tmin): 1.0 for ap in ACPOP}
    qfacindex = {(f, tmin): 1.0 for f in F}
    fprdindex = {(f, tmin): 1.0 for f in F}
    agelabindex = {tmin: 1.0}
    govspndindex0 = {(ac, tmin): 1.0 for ac in ACGOVSPND}
    govrecindex0 = {(ac, tmin): 1.0 for ac in ACGOVREC}
    ngovpayindex0 = {(ac, tmin): 1.0 for ac in ACNGOVPAY}
    for i, tt in enumerate(TSOL):
        if tt == tmin:
            continue
        tp = TSOL[i - 1]
        gdpindex[tt] = gdpindex[tp] * (1 + gdpgrw[tt])
        agelabindex[tt] = agelabindex[tp] * (1 + popgrw[("agelab", tt)])
        for ap in ACPOP:
            popindex[(ap, tt)] = popindex[(ap, tp)] * (1 + popgrw[(ap, tt)])
        for f in F:
            qfacindex[(f, tt)] = qfacindex[(f, tp)] * (1 + qfacgrw[(f, tt)])
            fprdindex[(f, tt)] = fprdindex[(f, tp)] * (1 + fprdgrw[(f, tt)])
        for ac in ACGOVSPND:
            govspndindex0[(ac, tt)] = (govspndindex0[(ac, tp)]
                                       * (1 + govspndgrw0[(ac, tt)]))
        for ac in ACGOVREC:
            govrecindex0[(ac, tt)] = (govrecindex0[(ac, tp)]
                                      * (1 + govrecgrw0[(ac, tt)]))
        for ac in ACNGOVPAY:
            ngovpayindex0[(ac, tt)] = (ngovpayindex0[(ac, tp)]
                                       * (1 + ngovpaygrw0[(ac, tt)]))

    tsolnb2 = len(TSOL) - 1  # ORD(tmax) - ORD(tmin) over t-positions in tsol
    # NOTE: GAMS uses ORD within full t; identical spacing here (annual).
    gdpgrwavg = (gdpindex[tmax] / gdpindex[tmin]) ** (1 / tsolnb2) - 1
    agelabgrwavg = (agelabindex[tmax] / agelabindex[tmin]) ** (1 / tsolnb2) - 1

    # ------------------------------------------------------------------
    # Prices (lines 119-198)
    # ------------------------------------------------------------------
    PA00 = {a: 1.0 for a in A}
    PX00 = {c: 1.0 for c in C}
    PDS00 = {c: 1.0 for c in C}
    PE00 = {c: 1.0 for c in C
            if sum(sam[(c, r)] for r in INSROW)}
    PM00 = {c: 1.0 for c in C
            if sum(sam[(r, c)] for r in INSROW)}
    EXR00 = 1.0

    shrom000 = defaultdict(float, {k[0]: v for k, v in
                                   P.get("shrom000", {}).items()})
    shroe000 = defaultdict(float, {k[0]: v for k, v in
                                   P.get("shroe000", {}).items()})

    denom = sum((1 - shrom000[c]) * sam[("row", c)]
                - (1 - shroe000[c]) * sam[(c, "row")] for c in C)
    prratexr00 = (sum(sam[("prexr", c)] for c in C) / denom) if denom else 0.0
    PREXR00 = 1 + prratexr00

    prexrindex0 = {k[0]: v for k, v in P.get("prexrindex0", {}).items()}
    if not any(prexrindex0.values()):
        prexrindex0 = {tt: 1.0 for tt in T}
    PREXR0 = {tt: PREXR00 * prexrindex0.get(tt, 1.0) for tt in TSOL}

    YPREXRT00 = {c: sam[("prexr", c)] for c in C}
    tot_prexr_col = samtot_col("prexr")
    shryprexr00 = sd()
    for c in C:
        for f in FCAPNG:
            if tot_prexr_col:
                shryprexr00[(c, f)] = sam[(f, "prexr")] / tot_prexr_col
    for c in C:
        s = sum(shryprexr00[(c, ac)] for ac in ACNT)
        assert abs(s - 1) < 1e-10 or tot_prexr_col == 0, f"shryprexr {c}: {s}"

    YPREXR00 = {(c, ac): shryprexr00[(c, ac)] * YPREXRT00[c]
                for c in C for ac in ACNT if shryprexr00.get((c, ac))}

    shroe00 = {c: shroe000[c] for c in C}
    shrom00 = {c: shrom000[c] for c in C}

    yprexrte = {c: (1 - shroe00[c]) * (PREXR00 - 1) * sam[(c, "row")]
                for c in C}
    yprexrtm = {c: (1 - shrom00[c]) * (PREXR00 - 1) * sam[("row", c)]
                for c in C}
    for c in C:
        gap = -yprexrte[c] + yprexrtm[c] - sam[("prexr", c)]
        assert abs(gap) < 1e-6, f"gapyprexrt {c}: {gap}"

    # ------------------------------------------------------------------
    # Quantities QA QE QM QX QD QQ (lines 205-238)
    # ------------------------------------------------------------------
    TAXEXP, TAXIMP = S["taxexp"], S["taximp"]
    TACD, TACM, TACE = S["tacd"], S["tacm"], S["tace"]
    TAXCOM, TAXVATC, TAXACT = S["taxcom"], S["taxvatc"], S["taxact"]
    TAXDIR, TAXFAC, TAXFACACT = S["taxdir"], S["taxfac"], S["taxfacact"]
    SUBCOM = S["subcom"]
    INV, INVNG, INVG, DSTK = S["inv"], S["invng"], S["invg"], S["dstk"]

    QA00 = {a: (samtot_row(a) - sam[(a, "prqmbar")]) / PA00[a] for a in A}
    QE00 = sd()
    for c in PE00:
        QE00[c] = (sum(sam[(c, r)] for r in INSROW)
                   - sum(sam[(tx, c)] for tx in TAXEXP)
                   - sum(sam[(te, c)] for te in TACE)
                   + yprexrte[c]) / PE00[c]
    QM00 = sd()
    for c in PM00:
        QM00[c] = (sum(sam[(r, c)] for r in INSROW)
                   + sum(sam[(tx, c)] for tx in TAXIMP)
                   + sam[("prqmbar", c)]
                   + sum(sam[(tm, c)] for tm in TACM)
                   + yprexrtm[c]) / PM00[c]
    QX00 = {c: sum(sam[(a, c)] for a in A) / PX00[c] for c in C}
    QD00 = {c: QX00[c] - QE00[c] for c in C}
    QQ00 = {c: QD00[c] + QM00[c] for c in C}

    PXAC00 = {(a, c): 1.0 for a in A for c in C if sam[(a, c)]}
    QXAC00 = {k: sam[k] / PXAC00[k] for k in PXAC00}

    # PDD, PQS (lines 243-251)
    PDD00 = {c: (PDS00[c] * QD00[c] + sum(sam[(td, c)] for td in TACD))
             / QD00[c] for c in C if QD00[c]}
    PQS00 = {c: (G(PM00, c) * QM00[c] + G(PDD00, c) * QD00[c]) / QQ00[c]
             for c in C if QQ00[c]}

    # sales tax TQ (line 256)
    TQ00 = sd()
    for c in C:
        if QQ00[c]:
            base = G(PDD00, c) * QD00[c] + G(PM00, c) * QM00[c]
            TQ00[c] = sum(sam[(tx, c)] for tx in TAXCOM) / base

    # VAT rates TVAC (lines 267-277) -- zero for bdi2019 (no taxvatc)
    mtaxvatc_set = set(map(tuple, mtaxvatc))
    TVAC00 = sd()
    for c in C:
        vat = sum(sam[(tv, c)] for tv in TAXVATC)
        if not vat:
            continue
        taxed_pay = sum(sam[(c, d)] for (cc, d) in mtaxvatc_set if cc == c)
        for (cc, d) in mtaxvatc_set:
            if cc == c:
                TVAC00[(c, d)] = vat / (taxed_pay - vat)
    for fcap in FCAP:
        for (fc, inv) in mfcapinv:
            if fc == fcap:
                for c in C:
                    if G(TVAC00, c, inv):
                        TVAC00[(c, fcap)] = TVAC00[(c, inv)]
                        TVAC00[(c, inv)] = 0.0

    # subsidies SUBC (lines 285-304) -- msubcom empty for bdi2019
    msub_set = set(map(tuple, msubcom))
    D = S["d"]
    vatpay = sd()
    for c in C:
        vat = sum(sam[(tv, c)] for tv in TAXVATC)
        dpay = sum(sam[(c, d)] for (cc, d) in mtaxvatc_set if cc == c)
        for d in D:
            if (c, d) in mtaxvatc_set and dpay:
                vatpay[(c, d)] = vat * sam[(c, d)] / dpay
    subsidy = sd()
    SUBC00 = sd()
    for c in C:
        subtot = -sum(sam[(sc, c)] for sc in SUBCOM)
        dpay = sum(sam[(c, d)] for (cc, d) in msub_set if cc == c)
        if not subtot or not dpay:
            continue
        for d in D:
            if (c, d) in msub_set:
                subsidy[(c, d)] = subtot * sam[(c, d)] / dpay
        den = sum(sam[(c, d)] + subsidy[(c, d)] - vatpay[(c, d)]
                  for (cc, d) in msub_set if cc == c)
        for d in D:
            if (c, d) in msub_set:
                SUBC00[(c, d)] = subtot / den
    for fcap in FCAP:
        for (fc, inv) in mfcapinv:
            if fc == fcap and any(G(SUBC00, c, inv) for c in C):
                for c in C:
                    SUBC00[(c, fcap)] = SUBC00[(c, inv)]
                    SUBC00[(c, inv)] = 0.0

    # demander prices PQD (lines 309-311)
    PQD00 = sd()
    for c in C:
        for d in D:
            if sam[(c, d)]:
                PQD00[(c, d)] = (G(PQS00, c) * (1 + TQ00[c])
                                 * (1 + G(TVAC00, c, d))
                                 * (1 - G(SUBC00, c, d)))
    for fcap in FCAP:
        for c in C:
            if any(sam[(c, inv)] for (fc, inv) in mfcapinv if fc == fcap):
                PQD00[(c, fcap)] = (G(PQS00, c) * (1 + TQ00[c])
                                    * (1 + G(TVAC00, c, fcap))
                                    * (1 - G(SUBC00, c, fcap)))
    for c in C:
        PQD00[(c, "row")] = 0.0
        for d in D:
            assert G(PQD00, c, d) >= 0

    # VAT rebate adaptation (lines 334-346) -- zero when no VAT
    RBTVAT00 = sd()
    for a in A:
        RBTVAT00[a] = sum(
            sam[(c, a)] / PQD00[(c, a)] * G(PQS00, c)
            * (1 - G(SUBC00, c, a)) * (1 + TQ00[c]) * G(TVAC00, c, a)
            for c in C if G(PQD00, c, a))
    shrbtvat00 = {(c, a): 1.0 for c in C for a in A}
    if any(RBTVAT00.values()):
        for a in A:
            sam[("rebate-vat", a)] = -RBTVAT00[a]
            for tx in TAXACT:
                sam[(tx, a)] += RBTVAT00[a]
        for g in INSGOV:
            for tx in TAXACT:
                sam[(g, tx)] += sum(RBTVAT00[a] for a in A)
        sam[("rebate-vat", "gov")] = sum(RBTVAT00[a] for a in A)

    # activity tax (line 364)
    TA00 = {a: sum(sam[(tx, a)] for tx in TAXACT) / (PA00[a] * QA00[a])
            for a in A}
    ta010 = {a: 1.0 for a in A}

    # tax on factor use (line 376)
    mtaxfa_set = set(map(tuple, mtaxfa))
    TFA00 = sd()
    for f in F:
        for a in A:
            if sam[(f, a)]:
                TFA00[(f, a)] = sum(
                    sam[(tfa, a)] for (tfa, ff) in mtaxfa_set if ff == f
                ) / sam[(f, a)]

    # ------------------------------------------------------------------
    # Trade/transport margins (lines 393-411)
    # ------------------------------------------------------------------
    shctd = {ct: sum(sam[(ct, td)] / samtot_col(td) for td in TACD
                     if samtot_col(td)) for ct in CT}
    shctm = {ct: sum(sam[(ct, tm)] / samtot_col(tm) for tm in TACM
                     if samtot_col(tm)) for ct in CT}
    shcte = {ct: sum(sam[(ct, te)] / samtot_col(te) for te in TACE
                     if samtot_col(te)) for ct in CT}
    icd, icm, ice = sd(), sd(), sd()
    for ct in CT:
        for c in C:
            if G(shctd, ct) and QD00[c]:
                icd[(ct, c)] = (shctd[ct] * sum(
                    sam[(td, c)] / PQD00[(ct, td)] for td in TACD
                    if G(PQD00, ct, td))) / QD00[c]
            if G(shctm, ct) and QM00[c]:
                icm[(ct, c)] = (shctm[ct] * sum(
                    sam[(tm, c)] / PQD00[(ct, tm)] for tm in TACM
                    if G(PQD00, ct, tm))) / QM00[c]
            if G(shcte, ct) and QE00[c]:
                ice[(ct, c)] = (shcte[ct] * sum(
                    sam[(te, c)] / PQD00[(ct, te)] for te in TACE
                    if G(PQD00, ct, te))) / QE00[c]
    QT00 = {ct: sum(sam[(ct, td)] / PQD00[(ct, td)] for td in TACD
                    if G(PQD00, ct, td))
            + sum(sam[(ct, te)] / PQD00[(ct, te)] for te in TACE
                  if G(PQD00, ct, te))
            + sum(sam[(ct, tm)] / PQD00[(ct, tm)] for tm in TACM
                  if G(PQD00, ct, tm))
            for ct in CT}

    # CPI and DPI (lines 415-427)
    hcons_tot = sum(sam[(c, h)] for c in C for h in H)
    cwts = {(c, h): sam[(c, h)] / hcons_tot for c in C for h in H}
    CPI00 = sum(cwts[(c, h)] * G(PQD00, c, h) for c in C for h in H)
    dwts_den = sum(sum(sam[(a, cp)] for a in A)
                   - sum(sam[(cp, r)] for r in INSROW) for cp in C)
    dwts = {c: (sum(sam[(a, c)] for a in A)
                - sum(sam[(c, r)] for r in INSROW)) / dwts_den for c in C}
    DPI00 = sum(dwts[c] * PDS00[c] for c in C)

    REXR00 = EXR00 / DPI00
    rexrindex0 = {k[0]: v for k, v in P.get("rexrindex0", {}).items()}
    if not any(rexrindex0.values()):
        rexrindex0 = {tt: 1.0 for tt in T}
    REXR0 = {tt: REXR00 * rexrindex0.get(tt, 1.0) for tt in TSOL}

    # share of gov capital income in output value (line 672)
    shfcapga = {(fcapg, a): sam[(fcapg, a)] / (PA00[a] * QA00[a])
                for fcapg in FCAPG for a in A if sam[(fcapg, a)]}

    # value added price (line 448)
    PVA00 = {}
    for a in A:
        num = sum(sam[(f, a)]
                  + sum(sam[(tfa, a)] for (tfa, ff) in mtaxfa_set if ff == f)
                  for f in FVA if f not in FLEO)
        PVA00[a] = num / ((samtot_row(a) + samtot_col(a)) / 2 / PA00[a]) \
            if False else num / ((samtot_col(a) - sam[(a, "prqmbar")])
                                 / PA00[a])

    # intermediates (lines 456-462)
    QINT00 = {(c, a): sam[(c, a)] / PQD00[(c, a)]
              for c in C for a in A if sam[(c, a)]}
    ica00 = {(c, a): QINT00[(c, a)] / QA00[a] for (c, a) in QINT00}

    # ------------------------------------------------------------------
    # Factors: employment, wages, supplies (lines 468-806)
    # ------------------------------------------------------------------
    fprdab00 = {(f, a): 1.0 for f in FVA for a in A}
    FPRDA00 = {(f, a): 1.0 for f in FVA for a in A}

    unemp = P.get("unemp", {})
    FUENDOG = set(S["fuendog"])
    UERAT00 = {f: G(unemp, f, "uerat00") for f in F if f in FUENDOG}
    eta_wf = {f: G(unemp, f, "eta_wf") for f in F if f in FUENDOG}

    qfbase = defaultdict(float, P.get("qfbase", {}))
    QF00 = sd()
    for f in F:
        for a in A:
            if qfbase[(f, a)]:
                QF00[(f, a)] = qfbase[(f, a)]
            elif sam[(f, a)]:
                QF00[(f, a)] = sam[(f, a)]
    for f in FCAPG:
        for a in A:
            QF00[(f, a)] = 0.0

    # labor endowment consistency check (lines 540-543)
    for f in FLAB:
        s1 = sum(qfbase[(f, a)] for a in A) / (1 - G(unemp, f, "uerat00")) \
            if (1 - G(unemp, f, "uerat00")) else 0.0
        s2 = sum(qlabins0[(h, f, tmin)] for h in H)
        if s2:
            assert abs(s1 - s2) < TOL, f"labor endow gap {f}"

    QFS00 = sd()
    for f in FNCAP:
        QFS00[f] = sum(QF00[(f, a)] for a in A) / (1 - G(UERAT00, f))

    # capest=2: initial private capital via net profit rate (lines 558-562)
    dinam = {k[0]: v for k, v in P.get("dinam", {}).items()}
    deprcap = {k[0]: v for k, v in P.get("deprcap", {}).items()}
    netprfrat = {}
    if db.capest == 2:
        for f in FCAP:
            netprfrat[f] = dinam["netprfrat"]
        for f in FCAPNG:
            for a in A:
                QF00[(f, a)] = sam[(f, a)] / (netprfrat[f] + deprcap[f])
            QFS00[f] = sum(QF00[(f, a)] for a in A)
    elif db.capest == 1:
        for f in FCAPNG:
            QFS00[f] = sum(
                sam[(c, inv)] / PQD00[(c, f)]
                for (fc, inv) in mfcapinv if fc == f and inv in INVNG
                for c in C if sam[(c, inv)]) / (gdpgrwavg + deprcap[f])
            tot = sum(sam[(f, ap)] for ap in A)
            for a in A:
                if sam[(f, a)] and f in set(FVA):
                    QF00[(f, a)] = sam[(f, a)] / tot * QFS00[f]

    # wages (lines 605-636)
    wfadum = {(f, a): sam[(f, a)] / QF00[(f, a)]
              for f in F for a in A if QF00[(f, a)]}
    WF00, WFDIST00, WFA00 = sd(), sd(), sd()
    fva_set, fsam_set = set(FVA), set(FSAM)
    for f in F:
        if f in fsam_set and f in fva_set:
            WF00[f] = (sum(sam[(f, a)] for a in A)
                       / sum(QF00[(f, a)] for a in A))
            for a in A:
                if (f, a) in wfadum:
                    WFDIST00[(f, a)] = wfadum[(f, a)] / WF00[f]
                WFA00[(f, a)] = (WF00[f] * G(WFDIST00, f, a)
                                 * (1 + G(TFA00, f, a)))
    # calibration check (line 622; GAMS only displays -- assert where valid)
    for f in F:
        if f not in fsam_set or f not in fva_set:
            continue
        for a in A:
            gap = (G(WF00, f) * G(WFDIST00, f, a) * QF00[(f, a)]
                   - sam[(f, a)])
            assert abs(gap) < 1e-6, f"costgap {f} {a}"

    # non-SAM aggregate factors (lines 631-648)
    f1set, f2set, f3set = set(F1), set(F2), set(F3)
    fnsam_set = set(FNSAM)
    for f in F:
        if (f in f1set or f in f2set) and f in fnsam_set:
            WF00[f] = 1.0
            for a in A:
                WFDIST00[(f, a)] = 1.0
                WFA00[(f, a)] = WF00[f] * 1.0
    for f2 in F2:
        if f2 in fnsam_set:
            for a in A:
                if G(WFA00, f2, a):
                    QF00[(f2, a)] = sum(
                        sam[(f3, a)] + sum(sam[(tfa, a)] for (tfa, ff)
                                           in mtaxfa_set if ff == f3)
                        for (f3, ff2) in mf3f2 if ff2 == f2) / WFA00[(f2, a)]
    for f1 in F1:
        if f1 in fnsam_set:
            for a in A:
                if G(WFA00, f1, a):
                    QF00[(f1, a)] = sum(
                        WFA00[(f2, a)] * QF00[(f2, a)]
                        + sum(sam[(tfa, a)] for (tfa, ff) in mtaxfa_set
                              if ff == f2)
                        for (f2, ff1) in mf2f1 if ff1 == f1) / WFA00[(f1, a)]

    # factor incomes and taxes (lines 653-664)
    YF00 = {f: sum(sam[(f, ac)] for ac in ACNT) for f in F}
    TF00 = {f: sum(sam[(tx, f)] for tx in TAXFAC) / YF00[f]
            for f in F if YF00[f]}
    tf010 = {(f, tt): 1.0 for f in F for tt in TSOL}

    # factor endowments SHIF/QFINS (lines 693-736)
    SHIF00 = sd()
    for f in F:
        if f in fsam_set and f in fva_set:
            den = samtot_col(f) - sum(sam[(tx, f)] for tx in TAXFAC)
            if den:
                for ins in INS:
                    if sam[(ins, f)]:
                        SHIF00[(ins, f)] = sam[(ins, f)] / den

    ssgrw = {k[0]: v for k, v in P.get("ssgrw", {}).items()}
    QFINS00 = sd()
    for g in INSGOV:
        for f in FCAPG:
            QFINS00[(g, f)] = sum(
                sam[(c, inv)] / PQD00[(c, f)]
                for (fc, inv) in mfcapinv if fc == f and inv in INVG
                for c in C if sam[(c, inv)]
            ) / (ssgrw.get("gdp", 0.0) + deprcap[f])

    for ins in INS:
        for f in F:
            if f in fsam_set and f in fva_set and f not in set(FCAP):
                QFINS00[(ins, f)] = SHIF00[(ins, f)] * QFS00[f]
    for ins in INS:
        for f in FLAB:
            if qlabins0[(ins, f, tmin)]:
                QFINS00[(ins, f)] = qlabins0[(ins, f, tmin)]
    for ins in INS:
        for f in FCAP:
            if f in fva_set and f in fsam_set:
                QFINS00[(ins, f)] = SHIF00[(ins, f)] * QFS00[f]

    # SHIF0 paths (lines 749-773): population-scaled for households
    INSNH = S["insnh"]
    SHIF0 = sd()
    for tt in TSOL:
        for f in F:
            if f in fsam_set and f in fva_set:
                for ins in INSNH:
                    SHIF0[(ins, f, tt)] = SHIF00[(ins, f)]
                den = sum(SHIF00[(hp, f)] * pop0[(hp, tt)] / pop00[hp]
                          for hp in H if pop00[hp])
                tot_h = sum(SHIF00[(hp, f)] for hp in H)
                for h in H:
                    if sam[(h, f)] and den:
                        SHIF0[(h, f, tt)] = (
                            tot_h * (SHIF00[(h, f)] * pop0[(h, tt)]
                                     / pop00[h]) / den)
    QFS0 = {(f, tt): QFS00[f] * qfacindex[(f, tt)]
            for f in F for tt in TSOL}
    QFINS0 = sd()
    for tt in TSOL:
        for ins in INS:
            for f in F:
                if f in fsam_set and f in fva_set:
                    QFINS0[(ins, f, tt)] = (QFS0[(f, tt)]
                                            * G(SHIF0, ins, f, tt))
        for g in INSGOV:
            for f in FCAPG:
                QFINS0[(g, f, tt)] = QFINS00[(g, f)] * gdpindex[tt]
    for f in F:
        for tt in TSOL:
            gap = (sum(SHIF00[(insd, f)] for insd in INSD)
                   - sum(G(SHIF0, insd, f, tt) for insd in INSD))
            assert abs(gap) < 1e-4, f"shifgap {f} {tt}"
    # overwrite SHIF0 for households and labor (line 773)
    for tt in TSOL:
        for f in FLAB:
            den = sum(QFINS0[(hp, f, tt)] for hp in H if SHIF00[(hp, f)])
            tot_h = sum(SHIF00[(hp, f)] for hp in H)
            for h in H:
                if den:
                    SHIF0[(h, f, tt)] = (QFINS0[(h, f, tt)] / den) * tot_h

    # labor force participation (lines 780-788)
    LABPARTRAT00 = (sum(QFINS00[(ins, f)] for ins in INS for f in FLAB)
                    / pop00["agelab"])
    assert LABPARTRAT00 <= 1
    labpartrat0_in = {k[0]: v for k, v in P.get("labpartrat0", {}).items()}
    LABPARTRAT0 = {}
    if not any(labpartrat0_in.values()):
        LABPARTRAT0 = {tt: LABPARTRAT00 for tt in TSOL}
    else:
        for tt in TSOL:
            LABPARTRAT0[tt] = (LABPARTRAT00 * labpartrat0_in.get(tt, 0.0)
                               / labpartrat0_in[tmin])

    # adjust QFS0 (line 793)
    for f in F:
        for tt in TSOL:
            QFS0[(f, tt)] = sum(QFINS0[(ins, f, tt)] for ins in INS)

    QFHEND0 = {(h, f, tt): QFINS0[(h, f, tt)]
               for h in H for f in F for tt in TSOL}

    YIF00 = sd()
    for ins in INS:
        for f in F:
            if SHIF00[(ins, f)]:
                YIF00[(ins, f)] = (SHIF00[(ins, f)] * YF00[f]
                                   * (1 - G(TF00, f)))

    # ------------------------------------------------------------------
    # Households, NGOs, tourists (lines 811-841)
    # ------------------------------------------------------------------
    QH00 = {(c, h): sam[(c, h)] / PQD00[(c, h)]
            for c in C for h in H if sam[(c, h)]}
    EH00 = {h: sum(sam[(c, h)] for c in C) for h in H}
    QNGO00 = {(c, n): sam[(c, n)] / PQD00[(c, n)]
              for c in C for n in INSNGO if sam[(c, n)]}
    QNGOSCAL00 = {n: 1.0 for n in INSNGO if sum(sam[(c, n)] for c in C)}
    QTRST00 = {(c, s): sam[(c, s)] / PQD00[(c, s)]
               for c in C for s in INSTRST if sam[(c, s)]}
    QTRSTSCAL00 = 1.0 if sum(sam[(c, s)] for c in C for s in INSTRST) else 0.0
    TRSMREC00 = sum(sam[(c, s)] for c in C for s in INSTRST) / EXR00
    TRSMREC0 = {tt: TRSMREC00 * ngovpayindex0[("tourismrec", tt)]
                for tt in TSOL}

    # ------------------------------------------------------------------
    # Institutions: income, taxes, savings (lines 846-916)
    # ------------------------------------------------------------------
    YI00 = {i: samtot_row(i) for i in INSDNG}
    TY00 = {i: sum(sam[(tx, i)] for tx in TAXDIR) / YI00[i] for i in INSDNG}
    ty010 = {(i, tt): 1.0 for i in INSDNG for tt in TSOL}

    QINV00 = sd()
    for c in C:
        if sum(sam[(c, inv)] for inv in INV):
            QINV00[c] = sum(
                sam[(c, inv)] / sum(G(PQD00, c, fc)
                                    for (fc, iv) in mfcapinv if iv == inv)
                for inv in INV if sam[(c, inv)])
    dstkcomp = {c: sum(sam[(c, dk)] / sum(sam[(cp, dk)] for cp in C)
                       for dk in DSTK if sum(sam[(cp, dk)] for cp in C))
                for c in C}
    CAPINSNG, CAPGOV = S["capinsng"], S["capgov"]
    CAPINSDNG, CAPROW, CAPFIN = S["capinsdng"], S["caprow"], S["capfin"]
    qdstk00 = sd()
    for c in C:
        if sum(sam[(c, dk)] for dk in DSTK):
            for dk in DSTK:
                qdstk00[(c, "ngovz")] += (
                    sum(sam[(dk, ci)] for ci in CAPINSNG) * dstkcomp[c]
                    / PQD00[(c, dk)])
                qdstk00[(c, "govz")] += (
                    sum(sam[(dk, cg)] for cg in CAPGOV) * dstkcomp[c]
                    / PQD00[(c, dk)])

    mcapins_set = set(map(tuple, mcapins))
    SAV00 = {i: sum(sam[(ci, i)] for ci in CAPINSDNG
                    if (ci, i) in mcapins_set) for i in INSDNG}
    MPS000 = {k[0]: v for k, v in P.get("mps000", {}).items()}
    MPS00, alpha_sav00 = {}, {}
    for i in INSDNG:
        mps = MPS000.get(i, 0.0)
        if mps:
            MPS00[i] = mps
            alpha_sav00[i] = (1 / CPI00) * (
                SAV00[i] - mps * YI00[i] * (1 - TY00[i]))
        else:
            MPS00[i] = SAV00[i] / (YI00[i] * (1 - TY00[i]))
            alpha_sav00[i] = 0.0
    savadj01 = {k[0]: v for k, v in P.get("savadj01", {}).items()} or \
        {i: 1.0 for i in INSDNG}

    SAVF00 = sum(sam[(cr, r)] for cr in CAPROW for r in INSROW) / EXR00

    # ------------------------------------------------------------------
    # Government (lines 922-965)
    # ------------------------------------------------------------------
    YTAXIMP00 = sum(sam[(g, tx)] for g in INSGOV for tx in TAXIMP)
    YTAXEXP00 = sum(sam[(g, tx)] for g in INSGOV for tx in TAXEXP)
    YTAXVAT00 = sum(sam[(g, tx)] for g in INSGOV for tx in TAXVATC)
    SUBCT00 = -sum(sam[(g, sc)] for g in INSGOV for sc in SUBCOM)

    EG00 = (sum(samtot_col(g) - sum(sam[(cg, g)] for cg in CAPGOV)
                for g in INSGOV)
            - sum(sam[(g, sc)] for g in INSGOV for sc in SUBCOM))
    INVVALG00 = (sum(sam[(inv, cg)] for inv in INV for cg in CAPGOV)
                 + sum(sam[(dk, cg)] for dk in DSTK for cg in CAPGOV))
    YG00 = (sum(samtot_row(g) for g in INSGOV)
            - sum(sam[(g, sc)] for g in INSGOV for sc in SUBCOM))
    GPRIMDEF00 = EG00 + INVVALG00 - YG00
    RGPRIMDEF00 = GPRIMDEF00 / CPI00

    QG00 = sd()
    for c in C:
        if sum(sam[(c, g)] for g in INSGOV):
            QG00[c] = sum(sam[(c, g)] / PQD00[(c, g)] for g in INSGOV
                          if sam[(c, g)])
    qgc010 = defaultdict(float, P.get("qgc010", {}))
    if not any(qgc010.values()):
        qgc010 = defaultdict(float, {(c, tt): 1.0 for c in C
                                     for tt in T if QG00[c]})

    # ------------------------------------------------------------------
    # Transfers (lines 970-1019)
    # ------------------------------------------------------------------
    TRNSFR00 = sd()
    for i in INSDNG:
        for g in INSGOV:
            TRNSFR00[(i, g)] = sam[(i, g)] / CPI00
    for r in INSROW:
        for g in INSGOV:
            TRNSFR00[(r, g)] = sam[(r, g)] / EXR00
    for i in INSD:
        for r in INSROW:
            TRNSFR00[(i, r)] = sam[(i, r)] / EXR00
    for f in F:
        for r in INSROW:
            TRNSFR00[(f, r)] = sam[(f, r)] / EXR00

    trnsfrpcb00 = sd()
    for h in H:
        for g in INSGOV:
            trnsfrpcb00[(h, g)] = TRNSFR00[(h, g)] / pop00[h]
        for r in INSROW:
            trnsfrpcb00[(h, r)] = TRNSFR00[(h, r)] / pop00[h]

    TRII00 = sd()
    shii00 = sd()
    for i in INSDNG:
        for ins in INS:
            if sam[(ins, i)]:
                TRII00[(ins, i)] = sam[(ins, i)]
                shii00[(ins, i)] = TRII00[(ins, i)] / (
                    YI00[i] * (1 - TY00[i]) - SAV00[i])
    # shii0 population-scaled for h -> insdngnh (lines 1009-1012)
    INSDNGNH = S["insdngnh"]
    shii0 = sd()
    for tt in TSOL:
        for ins in INS:
            for i in INSDNG:
                shii0[(ins, i, tt)] = shii00[(ins, i)]
        for i in INSDNGNH:
            raw = {h: shii00[(h, i)] * pop[(h, tt)] / pop00[h]
                   for h in H if pop00[h]}
            tot = sum(raw.values())
            if tot:
                for h in H:
                    shii0[(h, i, tt)] = raw.get(h, 0.0) / tot

    # ------------------------------------------------------------------
    # International trade calibration (lines 1027-1107)
    # ------------------------------------------------------------------
    qmbarindex0 = defaultdict(float, P.get("qmbarindex0", {}))
    for c in CMBAR:
        if not any(qmbarindex0[(cp, tt)] for cp in CMBAR for tt in T):
            for tt in T:
                qmbarindex0[(c, tt)] = 1.0
    qmbar00 = {c: QM00[c] for c in CMBAR}
    qmbar0 = {(c, tt): qmbar00[c] * qmbarindex0[(c, tt)]
              for c in CMBAR for tt in TSOL}
    # scenario multipliers on the ceilings (scenarios.py), kept apart so a
    # ceiling set by foreign exchange (fuel.py, channel 4) takes them too
    qmbar_scale = {}

    PRQMBAR00 = {c: sam[("prqmbar", c)] / sam[("row", c)]
                 for c in C if sam[("prqmbar", c)]}
    YPRQMBART00 = {c: sam[("prqmbar", c)] for c in C}

    TE00 = sd()
    for c in C:
        expv = sum(sam[(c, r)] for r in INSROW)
        if expv:
            TE00[c] = sum(sam[(tx, c)] for tx in TAXEXP) / expv
    PWE00 = sd()
    for c in C:
        if G(PE00, c):
            PWE00[c] = (PE00[c] + sum(G(PQD00, ct, te) * G(ice, ct, c)
                                      for ct in CT for te in TACE)) / (
                (1 - TE00[c]) * ((1 - shroe00[c]) * EXR00 * PREXR00
                                 + shroe00[c] * EXR00))
    for c in C:
        gap = G(PWE00, c) * EXR00 * QE00[c] - sam[(c, "row")]
        # gap includes premium wedge; exporters receive official-rate share
        # (replicates GAMS gapexp display -- not asserted there)

    qeb00 = {c: QE00[c] for c in C if c in CED or c in CESEXOG}
    pwse00 = {c: PWE00[c] for c in C if c in CED}
    eta_e = {c: G(P.get("tradelas", {}), c, "eta_e") for c in C}

    TM00 = sd()
    for c in C:
        impv = sum(sam[(r, c)] for r in INSROW)
        if impv:
            TM00[c] = (sum(sam[(tx, c)] for tx in TAXIMP)
                       / (impv + yprexrtm[c]))
    PWM00 = sd()
    for c in C:
        if G(PM00, c):
            PWM00[c] = (PM00[c] - sum(G(PQD00, ct, tm) * G(icm, ct, c)
                                      for ct in CT for tm in TACM)) / (
                (1 + TM00[c] + G(PRQMBAR00, c))
                * ((1 - shrom00[c]) * EXR00 * PREXR00
                   + shrom00[c] * EXR00))

    pweindex = defaultdict(float, P.get("pweindex", {}))
    pwmindex = defaultdict(float, P.get("pwmindex", {}))
    PWE0, PWM0 = {}, {}
    for c in C:
        has_we = any(pweindex[(c, tp)] for tp in T)
        has_wm = any(pwmindex[(c, tp)] for tp in T)
        for tt in TSOL:
            PWE0[(c, tt)] = (G(PWE00, c) * pweindex[(c, tt)]
                             if has_we else G(PWE00, c))
            PWM0[(c, tt)] = (G(PWM00, c) * pwmindex[(c, tt)]
                             if has_wm else G(PWM00, c))

    # ------------------------------------------------------------------
    # Dynamics parameters (lines 1115-1190)
    # ------------------------------------------------------------------
    kappa = dinam["kappa"]
    WFAVG00 = {f: sum(QF00[(f, a)] * G(WFA00, f, a) for a in A)
               / sum(QF00[(f, a)] for a in A)
               for f in F if f in fsam_set and f in fva_set
               and sum(QF00[(f, a)] for a in A)}

    capcomp = sd()
    for fcap in FCAP:
        invs = [inv for (fc, inv) in mfcapinv if fc == fcap]
        den = sum(sam[(cp, inv)] / PQD00[(cp, fcap)]
                  for inv in invs for cp in C if sam[(cp, inv)])
        for c in C:
            num = sum(sam[(c, inv)] for inv in invs)
            if num and den:
                capcomp[(fcap, c)] = (num / PQD00[(c, fcap)]) / den
    PK00 = {fcap: sum(capcomp[(fcap, c)] * G(PQD00, c, fcap) for c in C)
            for fcap in FCAP}

    DKINS00 = sd()
    for fcap in FCAP:
        invs = [inv for (fc, inv) in mfcapinv if fc == fcap]
        DKINS00[("ngovz", fcap)] = sum(
            sam[(inv, ci)] for inv in invs for ci in CAPINSDNG) / PK00[fcap]
        DKINS00[("govz", fcap)] = sum(
            sam[(inv, cg)] for inv in invs for cg in CAPGOV) / PK00[fcap]
        DKINS00[("rowz", fcap)] = sum(
            sam[(inv, cr)] for inv in invs for cr in CAPROW) / PK00[fcap]

    INVVALF00 = sum(sam[(inv, cr)] for inv in INVNG
                    for cr in CAPROW) / EXR00
    INVVALF0 = {tmin: INVVALF00}
    for i, tt in enumerate(TSOL):
        if tt != tmin:
            INVVALF0[tt] = (INVVALF0[TSOL[i - 1]]
                            * (1 + ngovpaygrw0[("fdi", tt)]))

    invshr00 = sd()
    for fcap in FCAP:
        invs = [inv for (fc, inv) in mfcapinv if fc == fcap]
        invshr00[(fcap, "rowz")] = sum(sam[(inv, cr)] for inv in invs
                                       for cr in CAPROW)
    tot_row = sum(invshr00[(fc, "rowz")] for fc in FCAP)
    if tot_row:
        for fcap in FCAP:
            invshr00[(fcap, "rowz")] /= tot_row

    iadj010 = {k[0]: v for k, v in P.get("iadj010", {}).items()}
    if not any(iadj010.values()):
        iadj010 = {fcap: 1.0 for fcap in FCAP}

    INVVAL00 = (sum(sam[(inv, ci)] for inv in INV for ci in CAPINSDNG)
                + sum(sam[(dk, ci)] for dk in DSTK for ci in CAPINSDNG))

    DKA00 = sd()
    for fcap in FCAPNG:
        totqf = sum(QF00[(fcap, ap)] for ap in A)
        for a in A:
            if totqf:
                DKA00[(fcap, a)] = (sum(DKINS00[(i2, fcap)]
                                        for i2 in S["ins2"])
                                    * QF00[(fcap, a)] / totqf)

    # ------------------------------------------------------------------
    # Macro aggregates (lines 1196-1279)
    # ------------------------------------------------------------------
    RGDPFC00 = (sum(PVA00[a] * QA00[a] for a in A)
                + sum(G(WFA00, f, a) * QF00[(f, a)]
                      for f in FLEO for a in A))
    TFP00 = {a: 1.0 for a in A}
    tfpexog0 = defaultdict(float, P.get("tfpexog0", {}))
    if not any(tfpexog0.values()):
        tfpexog0 = defaultdict(float, {(a, tt): 1.0 for a in A for tt in T})
    tfp010 = {k[0]: v for k, v in P.get("tfp010", {}).items()}
    if not any(tfp010.values()):
        tfp010 = {a: 1.0 for a in A}
    fprda01 = {(f, a): 1.0 for f in FLAB for a in A}

    IN2 = S["ins2"]
    GDPMP00 = (
        sum(G(PQD00, c, h) * G(QH00, c, h) for c in C for h in H)
        + sum(G(PQD00, c, n) * G(QNGO00, c, n) for c in C for n in INSNGO)
        + sum(G(PQD00, c, fc) * G(capcomp, fc, c)
              * sum(DKINS00[(i2, fc)] for i2 in IN2)
              for c in C for fc in FCAPNG)
        + sum(G(PQD00, c, fc) * G(capcomp, fc, c)
              * sum(DKINS00[(i2, fc)] for i2 in IN2)
              for c in C for fc in FCAPG)
        + sum(G(PQD00, c, dk) * sum(qdstk00[(c, i2)] for i2 in IN2)
              for c in C for dk in DSTK)
        + sum(G(PQD00, c, g) * G(QG00, c) for c in C for g in INSGOV)
        + sum(EXR00 * G(PWE00, c) * QE00[c] for c in C)
        + sum(G(PQD00, c, s) * G(QTRST00, c, s)
              for c in C for s in INSTRST)
        - sum(EXR00 * G(PWM00, c) * QM00[c] for c in C))
    RGDPMP00 = GDPMP00
    TRDGDP00 = (sum(EXR00 * G(PWE00, c) * QE00[c] for c in C)
                + sum(EXR00 * G(PWM00, c) * QM00[c] for c in C)) / RGDPMP00
    ABSNOM00 = (GDPMP00
                - sum(EXR00 * G(PWE00, c) * QE00[c] for c in C)
                - sum(G(PQD00, c, s) * G(QTRST00, c, s)
                      for c in C for s in INSTRST)
                + sum(EXR00 * G(PWM00, c) * QM00[c] for c in C))

    NDFG00 = sum(sam[(cg, ci)] for cg in CAPGOV for ci in S["capinsd"])
    RNDFG00 = NDFG00 / CPI00
    NFFG00 = sum(sam[(cg, cr)] for cg in CAPGOV for cr in CAPROW)
    NFFINS00 = sum(sam[(ci, cr)] for ci in CAPINSDNG for cr in CAPROW)
    drf00 = sum(sam[(cr, cf)] for cr in CAPROW for cf in CAPFIN)
    drf = {tt: drf00 * gdpindex[tt] for tt in TSOL}

    assert abs(GPRIMDEF00 - NDFG00 - EXR00 * NFFG00) < 1e-4
    assert abs(SAVF00 - NFFINS00 * EXR00 - NFFG00 * EXR00
               - INVVALF00 * EXR00 + drf00) < 1e-4
    gapinvest = INVVAL00 - (sum(SAV00[i] for i in INSDNG)
                            + NFFINS00 * EXR00 - (NDFG00 + drf00))
    assert abs(gapinvest) < 1e-4, f"gapinvest {gapinvest}"

    # investment-by-institution shares (lines 1295-1311)
    ndfgshr, savshr, gbadj00, drfadj00 = sd(), sd(), sd(), sd()
    dtot = sum(sam[(cg, ci)] for cg in CAPGOV for ci in CAPINSDNG)
    stot = sum(sam[(ci, i)] for ci in CAPINSDNG for i in INSDNG)
    ftot = sum(sam[(cf, ci)] for cf in CAPFIN for ci in CAPINSDNG)
    for i in INSDNG:
        own_caps = [ci for (ci, ii) in mcapins_set if ii == i]
        if dtot:
            ndfgshr[i] = sum(sam[(cg, ci)] for cg in CAPGOV
                             for ci in own_caps) / dtot
        savshr[i] = sum(sam[(ci, i)] for ci in own_caps) / stot
        if savshr[i]:
            gbadj00[i] = ndfgshr[i] / savshr[i]
        if ftot:
            drfadj00[i] = sum(sam[(cf, ci)] for cf in CAPFIN
                              for ci in own_caps) / ftot
        if savshr[i]:
            drfadj00[i] = drfadj00[i] / savshr[i]

    # gbadj/drfadj paths: =1 for t>tmin when dmod NE 2 (lines 1303, 1311)
    gbadj = {(i, tt): (gbadj00[i] if tt == tmin else 1.0)
             for i in INSDNG for tt in TSOL}
    drfadj = {(i, tt): (drfadj00[i] if tt == tmin else 1.0)
              for i in INSDNG for tt in TSOL}

    # ------------------------------------------------------------------
    # Gov receipts/spending/non-gov payment GDP & ABS shares (1328-1437)
    # ------------------------------------------------------------------
    GOVRECGDP00 = sd()
    for tx in TAXACT:
        GOVRECGDP00[tx] = sum(sam[(tx, a)] for a in A)
    for tx in TAXCOM:
        GOVRECGDP00[tx] = sum(sam[(tx, c)] for c in C)
    for tx in TAXDIR:
        GOVRECGDP00[tx] = sum(sam[(tx, i)] for i in INSDNG)
    for tx in TAXIMP:
        GOVRECGDP00[tx] = sum(sam[(tx, c)] for c in C)
    for tx in TAXEXP:
        GOVRECGDP00[tx] = sum(sam[(tx, c)] for c in C)
    for tx in TAXFAC:
        GOVRECGDP00[tx] = sum(sam[(tx, f)] for f in F)
    for tx in TAXVATC:
        GOVRECGDP00[tx] = sum(sam[(tx, c)] for c in C)
    GOVRECGDP00["trgovngov"] = sum(sam[(g, i)] for g in INSGOV
                                   for i in INSDNG)
    GOVRECGDP00["trgovrow"] = sum(sam[(g, "row")] for g in INSGOV)
    GOVRECGDP00["netdomfin"] = NDFG00
    GOVRECGDP00["netforfingov"] = EXR00 * NFFG00
    for ac in ACGOVREC:
        GOVRECGDP00[ac] = GOVRECGDP00[ac] / GDPMP00

    GOVSPNDGDP00 = sd()
    for fcapg in FCAPG:
        GOVSPNDGDP00[fcapg] = sum(
            sam[(c, inv)] for c in C
            for (fc, inv) in mfcapinv if fc == fcapg and inv in INVG)
    GOVSPNDGDP00["congov"] = sum(sam[(c, g)] for c in C for g in INSGOV)
    GOVSPNDGDP00["trngovgov"] = sum(sam[(i, g)] for i in INSDNG
                                    for g in INSGOV)
    GOVSPNDGDP00["trrowgov"] = sum(sam[(r, g)] for r in INSROW
                                   for g in INSGOV)
    for sc in SUBCOM:
        GOVSPNDGDP00[sc] = -sum(sam[(sc, c)] for c in C)
    for ac in ACGOVSPND:
        GOVSPNDGDP00[ac] = GOVSPNDGDP00[ac] / GDPMP00

    NGOVPAYGDP00 = sd()
    NGOVPAYGDP00["trngovrow"] = sum(sam[(i, r)] for i in INSDNG
                                    for r in INSROW)
    NGOVPAYGDP00["trrowngov"] = sum(sam[(r, i)] for r in INSROW
                                    for i in INSDNG)
    NGOVPAYGDP00["trfacrow"] = sum(sam[(f, r)] for f in F for r in INSROW)
    NGOVPAYGDP00["trrowfac"] = sum(sam[(r, f)] for r in INSROW for f in F)
    NGOVPAYGDP00["savngov"] = sum(sam[(ci, i)] for ci in CAPINSDNG
                                  for i in INSDNG)
    NGOVPAYGDP00["netforfinngov"] = EXR00 * NFFINS00
    for fcapng in FCAPNG:
        NGOVPAYGDP00[fcapng] = sum(
            sam[(inv, ci)] for (fc, inv) in mfcapinv
            if fc == fcapng and inv in INVNG for ci in CAPINSDNG)
    NGOVPAYGDP00["fdi"] = sum(sam[(inv, cr)] for inv in INVNG
                              for cr in CAPROW)
    NGOVPAYGDP00["tourismrec"] = sum(sam[(c, s)] for c in C
                                     for s in INSTRST)
    for ac in ACNGOVPAY:
        NGOVPAYGDP00[ac] = NGOVPAYGDP00[ac] / GDPMP00

    def build_path(base00, data0, group):
        """Rescale exogenous GDP-share paths to hit base year, else flat."""
        out = {}
        for ac in group:
            has = {tt: G(data0, ac, tt) for tt in TSOL}
            base_at_tmin = has.get(tmin, 0.0)
            for tt in TSOL:
                if has[tt] and base_at_tmin:
                    out[(ac, tt)] = has[tt] * base00[ac] / base_at_tmin
                elif has[tt]:
                    out[(ac, tt)] = has[tt]
                else:
                    out[(ac, tt)] = base00[ac]
        return out

    GOVRECGDP0 = build_path(GOVRECGDP00,
                            defaultdict(float, P.get("govrecgdp0", {})),
                            ACGOVREC)
    GOVSPNDGDP0 = build_path(GOVSPNDGDP00,
                             defaultdict(float, P.get("govspndgdp0", {})),
                             ACGOVSPND)
    NGOVPAYGDP0 = build_path(NGOVPAYGDP00,
                             defaultdict(float, P.get("ngovpaygdp0", {})),
                             ACNGOVPAY)

    GOVRECABS00 = {ac: GOVRECGDP00[ac] / GDPMP00 * ABSNOM00
                   for ac in ACGOVREC}
    GOVSPNDABS00 = {ac: GOVSPNDGDP00[ac] / GDPMP00 * ABSNOM00
                    for ac in ACGOVSPND}
    NGOVPAYABS00 = {ac: NGOVPAYGDP00[ac] / GDPMP00 * ABSNOM00
                    for ac in ACNGOVPAY}
    GOVRECABS0 = build_path(GOVRECABS00,
                            defaultdict(float, P.get("govrecabs0", {})),
                            ACGOVREC)
    GOVSPNDABS0 = build_path(GOVSPNDABS00,
                             defaultdict(float, P.get("govspndabs0", {})),
                             ACGOVSPND)
    NGOVPAYABS0 = build_path(NGOVPAYABS00,
                             defaultdict(float, P.get("ngovpayabs0", {})),
                             ACNGOVPAY)

    # ------------------------------------------------------------------
    # Debt stocks (lines 1442-1485)
    # ------------------------------------------------------------------
    gintrat0 = {k[0]: v for k, v in P.get("gintrat0", {}).items()}
    fintrat0 = defaultdict(float, P.get("fintrat0", {}))
    gintrat00 = gintrat0.get(tmin, 0.0)
    gintrat = {tt: gintrat0.get(tt, 0.0) for tt in TSOL}
    fintrat00 = {i2: fintrat0[(i2, tmin)] for i2 in IN2}
    fintrat = {(i2, tt): fintrat0[(i2, tt)] for i2 in IN2 for tt in TSOL}

    debt00 = defaultdict(float, P.get("debt00", {}))
    GDEBT00 = debt00[("ngovz", "govz")]
    FDEBT00 = {"ngovz": debt00[("rowz", "ngovz")],
               "govz": debt00[("rowz", "govz")]}
    GBOR00 = NDFG00 + gintrat00 * GDEBT00
    FBOR00 = {"govz": NFFG00 + fintrat00["govz"] * FDEBT00["govz"],
              "ngovz": NFFINS00 + fintrat00["ngovz"] * FDEBT00["ngovz"]}

    # ------------------------------------------------------------------
    # CES/CET/LES calibration (lines 1497-1728)
    # ------------------------------------------------------------------
    theta = {(a, c): QXAC00[(a, c)] / QA00[a] for (a, c) in QXAC00}

    sigma_ac = {c: 2.0 for c in C}
    rho_ac = {c: 1 / sigma_ac[c] - 1 for c in C}
    delta_ac, phi_ac = sd(), sd()
    for c in C:
        den = sum(QXAC00[(ap, c)] ** (1 / sigma_ac[c]) * PXAC00[(ap, c)]
                  for ap in A if (ap, c) in QXAC00)
        for a in A:
            if (a, c) in QXAC00:
                delta_ac[(a, c)] = (QXAC00[(a, c)] ** (1 / sigma_ac[c])
                                    * PXAC00[(a, c)]) / den
        if QX00[c]:
            phi_ac[c] = QX00[c] / sum(
                delta_ac[(a, c)] * QXAC00[(a, c)] ** (-rho_ac[c])
                for a in A if (a, c) in QXAC00) ** (-1 / rho_ac[c])

    # VA level 1
    prodelas = {k[0]: v for k, v in P.get("prodelas", {}).items()}
    sigma_va = {a: prodelas[a] for a in A}
    rho_va = {a: 1 / sigma_va[a] - 1 for a in A}
    delta_va, phi_va = sd(), sd()
    for a in A:
        den = sum(QF00[(fp, a)] ** (1 / sigma_va[a]) * G(WFA00, fp, a)
                  for fp in F1 if fp not in FLEO and QF00[(fp, a)])
        for f in F1:
            if f not in FLEO and QF00[(f, a)] and den:
                delta_va[(f, a)] = (QF00[(f, a)] ** (1 / sigma_va[a])
                                    * WFA00[(f, a)]) / den
        if QA00[a] and sum(sam[(f, a)] for f in F):
            phi_va[a] = QA00[a] / sum(
                delta_va[(f, a)] * QF00[(f, a)] ** (-rho_va[a])
                for f in F1 if f not in FLEO and QF00[(f, a)]
            ) ** (-1 / rho_va[a])

    ifa0 = {(f, a): QF00[(f, a)] / QA00[a]
            for f in FLEO for a in A if QF00[(f, a)]}

    # check level-1 factor demands (lines 1542-1547)
    for f in F1:
        for a in A:
            if f not in FLEO and QF00[(f, a)]:
                chk = ((PVA00[a] / WFA00[(f, a)]) ** sigma_va[a]
                       * delta_va[(f, a)] ** sigma_va[a]
                       * (TFP00[a] * phi_va[a]) ** (sigma_va[a] - 1)
                       * QA00[a] * FPRDA00[(f, a)] ** (sigma_va[a] - 1))
                assert abs(QF00[(f, a)] - chk) < max(
                    1e-6, 1e-6 * abs(QF00[(f, a)])), f"qfgap {f} {a}"

    # VA levels 2 and 3
    prodelas2 = defaultdict(float, P.get("prodelas2", {}))
    prodelas3 = defaultdict(float, P.get("prodelas3", {}))
    sigma2 = {(f1, a): prodelas2[(a, f1)] for f1 in F1 for a in A}
    rho2 = {(f1, a): 1 / sigma2[(f1, a)] - 1 for f1 in F1 for a in A
            if sigma2[(f1, a)]}
    delta2, phi2 = sd(), sd()
    for f1 in F1:
        for a in A:
            if not sigma2[(f1, a)]:
                continue
            kids = [f2 for (f2, ff1) in mf2f1 if ff1 == f1]
            den = sum(QF00[(f2p, a)] ** (1 / sigma2[(f1, a)])
                      * G(WFA00, f2p, a) for f2p in F2 if QF00[(f2p, a)])
            for f2 in kids:
                if QF00[(f2, a)] and den:
                    delta2[(f2, a)] = (QF00[(f2, a)] ** (1 / sigma2[(f1, a)])
                                       * WFA00[(f2, a)]) / den
            if f1 in fnsam_set and QF00[(f1, a)]:
                phi2[(f1, a)] = QF00[(f1, a)] / sum(
                    delta2[(f2, a)] * QF00[(f2, a)] ** (-rho2[(f1, a)])
                    for f2 in kids if QF00[(f2, a)]
                ) ** (-1 / rho2[(f1, a)])

    sigma3 = {(f2, a): prodelas3[(a, f2)] for f2 in F2 for a in A}
    rho3 = {(f2, a): 1 / sigma3[(f2, a)] - 1 for f2 in F2 for a in A
            if sigma3[(f2, a)]}
    delta3, phi3 = sd(), sd()
    for f2 in F2:
        for a in A:
            if not sigma3[(f2, a)]:
                continue
            kids = [f3 for (f3, ff2) in mf3f2 if ff2 == f2]
            den = sum(QF00[(f3p, a)] ** (1 / sigma3[(f2, a)])
                      * G(WFA00, f3p, a) for f3p in kids if QF00[(f3p, a)])
            for f3 in kids:
                if QF00[(f3, a)] and den:
                    delta3[(f3, a)] = (QF00[(f3, a)] ** (1 / sigma3[(f2, a)])
                                       * WFA00[(f3, a)]) / den
            if f2 in fnsam_set and QF00[(f2, a)]:
                phi3[(f2, a)] = QF00[(f2, a)] / sum(
                    delta3[(f3, a)] * QF00[(f3, a)] ** (-rho3[(f2, a)])
                    for f3 in kids if QF00[(f3, a)]
                ) ** (-1 / rho3[(f2, a)])

    # trade CET/Armington (lines 1630-1662)
    tradelas = defaultdict(float, P.get("tradelas", {}))
    sigma_x = {c: tradelas[(c, "sigma_x")] for c in C}
    sigma_q = {c: tradelas[(c, "sigma_q")] for c in C}
    rho_x = {c: 1 / sigma_x[c] + 1 for c in C if sigma_x[c]}
    rho_q = {c: 1 / sigma_q[c] - 1 for c in C if sigma_q[c]}
    delta_e, delta_ds, phi_x = sd(), sd(), sd()
    delta_m, delta_dd, phi_q = sd(), sd(), sd()
    for c in C:
        if QD00[c] > 0 and QE00[c] > 0:
            pe_term = G(PE00, c) * QE00[c] ** (-1 / sigma_x[c])
            pd_term = PDS00[c] * QD00[c] ** (-1 / sigma_x[c])
            delta_e[c] = pe_term / (pe_term + pd_term)
            delta_ds[c] = pd_term / (pe_term + pd_term)
            phi_x[c] = QX00[c] / (
                delta_e[c] * QE00[c] ** rho_x[c]
                + delta_ds[c] * QD00[c] ** rho_x[c]) ** (1 / rho_x[c])
        if QD00[c] > 0 and QM00[c] > 0:
            pm_term = G(PM00, c) * QM00[c] ** (1 / sigma_q[c])
            pd_term = G(PDD00, c) * QD00[c] ** (1 / sigma_q[c])
            delta_m[c] = pm_term / (pm_term + pd_term)
            delta_dd[c] = pd_term / (pm_term + pd_term)
            phi_q[c] = QQ00[c] / (
                delta_m[c] * QM00[c] ** (-rho_q[c])
                + delta_dd[c] * QD00[c] ** (-rho_q[c])) ** (-1 / rho_q[c])

    # LES (lines 1676-1726)
    leselas = defaultdict(float, P.get("leselas", {}))
    frisch = {k[0]: v for k, v in P.get("frisch", {}).items()}
    budshr = sd()
    for h in H:
        tot = sum(G(PQD00, cp, h) * G(QH00, cp, h) for cp in C)
        for c in C:
            budshr[(c, h)] = G(PQD00, c, h) * G(QH00, c, h) / tot
    elaschk = {h: sum(budshr[(c, h)] * leselas[(c, h)] for c in C)
               for h in H}
    leselas_adj = {(c, h): leselas[(c, h)] / elaschk[h]
                   for c in C for h in H}
    beta = {(c, h): budshr[(c, h)] * leselas_adj[(c, h)]
            for c in C for h in H}
    gamma00 = sd()
    for h in H:
        for c in C:
            if budshr[(c, h)]:
                gamma00[(c, h)] = (G(QH00, c, h)
                                   + (beta[(c, h)] / PQD00[(c, h)])
                                   * (EH00[h] / frisch[h])) / pop00[h]
    # LES check (lines 1713-1715)
    for h in H:
        supernum = EH00[h] - sum(gamma00[(c, h)] * pop00[h]
                                 * G(PQD00, c, h) for c in C)
        frisch2 = -EH00[h] / supernum
        assert abs(frisch[h] - frisch2) < 1e-8, f"leschk {h}"

    # ------------------------------------------------------------------
    # TFP link to gov capital: mpk (lines 1854-1906)
    # ------------------------------------------------------------------
    mpcapgov = {k[0]: v for k, v in P.get("mpcapgov", {}).items()}
    mtfp = defaultdict(float, P.get("mtfp", {}))
    va_tot = sum(sam[(fp, ap)] for fp in F for ap in A)
    mpk00 = sd()
    for fcap in FCAP:
        if not mpcapgov.get(fcap):
            continue
        for a in A:
            if mtfp[(a, fcap)]:
                mpk00[(a, fcap)] = (mpcapgov[fcap]
                                    * (sum(sam[(fp, a)] for fp in FCAP)
                                       / sum(sam[(fp, ap)] for fp in FCAP
                                             for ap in A))
                                    * mtfp[(a, fcap)])
        tot = sum(mpk00[(ap, fcap)] for ap in A)
        if tot:
            for a in A:
                if mpk00[(a, fcap)]:
                    mpk00[(a, fcap)] *= mpcapgov[fcap] / tot
        mpk00[("total", fcap)] = sum(mpk00[(a, fcap)] for a in A)
    mpk = {(ac, fc, tt): val for (ac, fc), val in mpk00.items()
           for tt in TSOL}
    # consistency: either both mpcapgov and mtfp nonzero, or neither
    for fcap in FCAP:
        has_mp = bool(mpcapgov.get(fcap))
        has_mtfp = any(mtfp[(a, fcap)] for a in A)
        assert has_mp == has_mtfp, f"mpcapgov/mtfp mismatch {fcap}"
    tfpelas = defaultdict(float, P.get("tfpelas", {}))
    for a in A:
        for fcap in FCAP:
            tfpelas[(a, fcap)] = 0.0   # line 1901: new mpk treatment

    # ------------------------------------------------------------------
    # Emissions (lines 1912-1929)
    # ------------------------------------------------------------------
    qemibase = P.get("qemibase", {})
    QEMI00 = {k: v for k, v in qemibase.items() if k[2] != "total"}
    iemi00 = sd()
    for (g, ac, acp), v in QEMI00.items():
        if acp in set(A):
            if (ac, acp) in QINT00:
                iemi00[(g, ac, acp)] = v / QINT00[(ac, acp)]
            elif QF00[(ac, acp)]:
                iemi00[(g, ac, acp)] = v / QF00[(ac, acp)]
            elif not sam[(ac, acp)]:
                iemi00[(g, ac, acp)] = v / QA00[acp]
        elif acp in set(H):
            if (ac, acp) in QH00:
                iemi00[(g, ac, acp)] = v / QH00[(ac, acp)]
            elif not sam[(ac, acp)]:
                iemi00[(g, ac, acp)] = v / sum(
                    G(PQD00, c, acp) * G(QH00, c, acp) for c in C)
        elif acp in set(INSGOV):
            if G(QG00, ac):
                iemi00[(g, ac, acp)] = v / QG00[ac]
            elif not sam[(ac, acp)]:
                iemi00[(g, ac, acp)] = v / sum(
                    G(PQD00, c, acp) * G(QG00, c) for c in C)

    # ------------------------------------------------------------------
    # Import quota rent shares (lines 1946-1958)
    # ------------------------------------------------------------------
    shryprqmbar00 = sd()
    for c in C:
        den = QQ00[c] - G(QT00, c)
        if not den:
            continue
        for a in A:
            shryprqmbar00[(c, a)] = G(QINT00, c, a) / den
        for g in INSGOV:
            shryprqmbar00[(c, g)] = G(QG00, c) / den
        for h in H:
            shryprqmbar00[(c, h)] = G(QH00, c, h) / den
        shryprqmbar00[(c, "ngovz")] = (G(QINV00, c) + sum(
            qdstk00[(c, i2)] for i2 in IN2)) / den
    YPRQMBAR00 = {(c, ac): shryprqmbar00[(c, ac)] * YPRQMBART00[c]
                  for c in C for ac in ACNT if shryprqmbar00[(c, ac)]}
    for c in C:
        s = sum(shryprqmbar00[(c, ac)] for ac in ACNT)
        if YPRQMBART00[c]:
            assert abs(s - 1) < 1e-8, f"shryprqmbar {c}: {s}"

    # aliases matching GAMS "current" parameter names used in equations
    tfpexog = tfpexog0
    tfp01 = tfp010
    ta01 = ta010
    fprda01 = fprda01
    fleo01 = defaultdict(float, P.get("fleo01", {}))

    # ------------------------------------------------------------------
    # pack everything into the namespace
    # ------------------------------------------------------------------
    for k, v in list(locals().items()):
        if k in ("db", "S", "M", "P", "cal", "fuel_mech", "rent_cost"):
            continue
        setattr(cal, k, v)
    from .fuel import calibrate_fuel, default_fuel
    cal.fuel = None
    if fuel_mech is _DEFAULT:
        fuel_mech = default_fuel()
    if fuel_mech is not None:
        cal.fuel_log = calibrate_fuel(cal, fuel_mech)
    # premium rent as a real cost (model.py, EQ_TFPDEF): base-year rent share
    # of GDP, so the productivity term is exactly 1 in the base year
    if rent_cost is _DEFAULT:
        rent_cost = 0.0 if fuel_mech is None else RENT_COST_DEFAULT
    cal.rent_cost = float(rent_cost or 0.0)
    cal.rent_adjust = RENT_ADJUST_DEFAULT
    cal.rent_share00 = sum(cal.YPREXRT00.values()) / cal.GDPMP00
    return cal
