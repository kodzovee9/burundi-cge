"""Numeric solver for one within-period GEM-Core system.

Strategy (robust, dependency-light): treat the model as a square system
F(x) = 0 where x is the vector of *free* variable entries (those not fixed by
the closure). We use scipy.optimize.root (hybr / LM) with a numeric Jacobian.
The residual function reuses gemcore.model.residuals unchanged.

For the base year with base closure the calibrated point is already a root,
so this mainly validates the solve machinery and enables shocks/dynamics.

The closure decides, per variable entry, whether it is:
  - fixed  : value comes from the closure/initial state, entry removed from x
  - free   : entry is an unknown in x
and the *equation set* is filtered to stay square. We keep every equation
that the GAMS MODEL GEM includes for this period, and free exactly as many
variable entries. WALRAS(t) absorbs the redundant market-clearing row
(Walras' law), matching the GAMS MCP.
"""

from __future__ import annotations

import numpy as np
from scipy.optimize import root

from .model import residuals
from .partition import build_partition
from .closure import _GOVREC_SCAL, _GOVSPND_SCAL, _NGOVPAY_SCAL


# Variables that are ALWAYS exogenous / fixed in every closure of the base run
# (their EQ_* definitional equations are still present, so we keep them free
# only when the closure frees them). See macclos.inc / facclos.inc.
#
# For the base bdi2019 closure:
#   numeraire=2 -> DPI fixed; CPI, EXR free
#   govclos=1   -> TYSCAL free (clears gov budget); TASCAL,TQSCAL,... fixed
#   siclos=2    -> MPSSCAL free; IADJ(ngovz) fixed
#   rowclos=2   -> REXR fixed, NFFINSSCAL free, PREXR fixed
#   facclos=4 (labor) -> WF free, UERAT free, WFDIST fixed, QFINSSCAL fixed
#   capital (dynamic, tmin) -> QF(cap) fixed, WF(cap) fixed, WFDIST(cap) free


# Variable families that GAMS fixes at zero wherever their base ("00") value
# is zero (mod.gms 3877-3915). Restricting the zero-fix to these families
# keeps genuine scaling/slack unknowns (WALRAS, SUBCT, the closure scalars)
# free even though their base level is 0.
ZEROFIX_FAMILIES = {
    "DKINS", "FPRDA", "MPS", "SAV", "PDD", "PDS", "PE", "PM", "PQD", "PQS",
    "QA", "QD", "QE", "QF", "QFINS", "QG", "QH", "QNGO", "QTRST", "TRSMREC",
    "QINT", "QINV", "QM", "QQ", "QT", "QX", "SHIF", "TRII", "TRNSFR",
    "WFDIST", "WFA", "YF", "YI", "YIF", "INVVAL", "INVVALF", "QFHEND",
    "PVA", "QXAC", "PXAC", "QNGOSCAL",
    # premium / quota rent allocations: fixed at 0 where share is 0
    "YPREXR", "YPREXRT", "YPRQMBAR", "YPRQMBART", "PRQMBAR",
}

ZTOL = 1e-11


def zero_fixed(cal, t, V):
    """Return the set of (name, idx) entries GAMS fixes at zero because their
    base value is zero (plus structural fixings that have no equation)."""
    S = cal.db.sets
    A, C, F = S["a"], S["c"], S["f"]
    FSAM, FVA = set(S["fsam"]), set(S["fva"])
    CMBAR = set(S["cmbar"])
    fixed = set()

    for name in ZEROFIX_FAMILIES:
        d = V.get(name, {})
        for idx, val in d.items():
            if abs(val) < ZTOL:
                fixed.add((name, idx))

    # PXAC has no defining equation -> always fixed (at 1)
    for idx in V.get("PXAC", {}):
        fixed.add(("PXAC", idx))

    # TFA: free only where TFADEF fires (fsam AND fva); rest fixed
    for (f, a) in list(V.get("TFA", {})):
        if not (f in FSAM and f in FVA):
            fixed.add(("TFA", (f, a)))

    # PRQMBAR / YPRQMBART free only for cmbar commodities (EQ_QMCONST,
    # EQ_IMPQUOTARENT); rest fixed at 0 (mod.gms 2612, 2617)
    for c in C:
        if c not in CMBAR:
            fixed.add(("PRQMBAR", c))
            fixed.add(("YPRQMBART", c))

    return fixed


def base_closure_fixed(cal, t):
    """Return a set of (name, idx) variable entries that are FIXED for the
    base-run closure.

    This mirrors mod.gms 3784-3915 plus macclos.inc and facclos.inc, driven by
    the closure-rule parameters actually loaded from the dataset
    (govrecrule0 / govspndrule0 / ngovpayrule0 / facclos0 / govclos0 /
    rowclos0 / siclos0 / numeraire0) rather than by hard-coded choices, so the
    same code reproduces the GAMS closure for any GEM-Core dataset. Statement
    order follows macclos.inc: the receipt/spending/payment rules first, then
    numeraire, govclos, rowclos and siclos, each of which may re-free a
    variable an earlier rule fixed.
    """
    S = cal.db.sets
    P = cal.db.pars
    A, C, F, H = S["a"], S["c"], S["f"], S["h"]
    FCAP = set(S["fcap"])
    FSAM, FVA, FNSAM = set(S["fsam"]), set(S["fva"]), set(S["fnsam"])
    CED = set(S["ced"])
    INS = S["ins"]
    IN2 = S["ins2"]
    tmin = S["tmin"][0]
    fixed = set()

    def fx(name, *idx):
        fixed.add((name, idx if len(idx) != 1 else idx[0]))

    def unfx(name, *idx):
        fixed.discard((name, idx if len(idx) != 1 else idx[0]))

    def rule(par, key):
        return int(P.get(par, {}).get((key,), 0) or 0)

    def scalar_rule(par, key=()):
        return int(P.get(par, {}).get(key, 0) or 0)

    # ---- mod.gms 3860-3868: world prices, factor-productivity scaling ------
    # PWM is exogenous for every commodity; PWE is endogenous only for the
    # constant-elasticity-of-export-demand commodities that actually export.
    for c in C:
        fx("PWM", c)
        if c not in CED or not cal.QE00.get(c):
            fx("PWE", c)
    fx("FPRDASCAL", t)

    # ---- macclos.inc: government receipts, spending, non-gov payments -----
    def apply_rules(par, accounts, scalmap, gdpvar, absvar):
        for ac in accounts:
            ent = scalmap.get(ac)
            if ent is None:
                # macclos.inc has no LOOP for this account, so its rule value is
                # never acted on and nothing is fixed -- whatever the rule says.
                # trgovngov and trrowngov are both rule 2 with no loop (the file
                # even says "not yet: trgovngov"), and the cssoc-* accounts sit
                # in an empty taxfac set. Fixing their GDP ratio anyway removes
                # two genuinely endogenous columns and leaves the period system
                # over-determined.
                continue
            r = rule(par, ac)
            if r == 1:
                nm, key = ent
                fx(nm, t if key is None else key)
            elif r == 2:
                fx(gdpvar, ac)
            elif r == 3:
                fx(absvar, ac)

    apply_rules("govrecrule0", S["acgovrec"], _GOVREC_SCAL,
                "GOVRECGDP", "GOVRECABS")
    fx("TFASCAL", t)                      # macclos.inc 281, unconditional
    apply_rules("govspndrule0", S["acgovspnd"], _GOVSPND_SCAL,
                "GOVSPNDGDP", "GOVSPNDABS")
    fx("SUBCSCAL", t)                     # macclos.inc 480, unconditional
    fx("MPSADJ", t)                       # macclos.inc 624, unconditional
    apply_rules("ngovpayrule0", S["acngovpay"], _NGOVPAY_SCAL,
                "NGOVPAYGDP", "NGOVPAYABS")

    # TFSCAL / TVACSCAL have no macclos LOOP when taxfac / taxvatc are empty.
    # Their columns are then numerically null (tfb = tvacb = 0), so fixing
    # them is equivalent to GAMS dropping them and keeps the candidate set
    # small.
    if not S.get("taxfac"):
        fx("TFSCAL", t)
    if not S.get("taxvatc"):
        fx("TVACSCAL", t)

    # ---- macclos.inc: numeraire -------------------------------------------
    num = scalar_rule("numeraire0")
    fx({1: "CPI", 2: "DPI", 3: "EXR"}[num], t)

    # ---- macclos.inc: government closure ----------------------------------
    fx("IADJ", "govz")
    govclos = scalar_rule("govclos0")
    if govclos == 1:                      # direct tax clears the gov budget
        unfx("TYSCAL", t)
        for ac in S["taxdir"]:
            unfx("GOVRECGDP", ac)
            unfx("GOVRECABS", ac)
    elif govclos == 2:                    # net domestic financing clears
        unfx("RNDFG", t)
        unfx("GOVRECGDP", "netdomfin")
        unfx("GOVRECABS", "netdomfin")
    elif govclos == 3:                    # net foreign financing clears
        unfx("NFFG", t)
        unfx("GOVRECGDP", "netforfingov")
        unfx("GOVRECABS", "netforfingov")
    else:
        raise NotImplementedError(f"govclos={govclos} not ported")

    # ---- macclos.inc: rest-of-world closure (indexed by period) -----------
    rowclos = int(P.get("rowclos0", {}).get((t,), 0) or 0)
    fx("PREXR", t)
    if rowclos == 1:
        pass                              # REXR flexes, PREXR fixed
    elif rowclos in (2, 3, 4):
        fx("REXR", t)
        if rowclos == 2:                  # non-gov net foreign financing
            unfx("NFFINSSCAL", t)
            unfx("NGOVPAYGDP", "netforfinngov")
            unfx("NGOVPAYABS", "netforfinngov")
        elif rowclos == 3:                # gov net foreign financing
            unfx("NFFG", t)
            unfx("GOVRECGDP", "netforfingov")
            unfx("GOVRECABS", "netforfingov")
        else:                             # 4: exchange-rate premium flexes
            unfx("PREXR", t)
    else:
        raise NotImplementedError(f"rowclos={rowclos} not ported")

    # ---- macclos.inc: savings-investment closure --------------------------
    fx("IADJ", "ngovz")
    fx("IADJ", "rowz")
    fx("MPSSCAL", t)
    siclos = scalar_rule("siclos0")
    if siclos == 1:                       # investment clears
        # macclos.inc 1006-1015 frees three things, not one: the investment
        # adjuster AND the non-gov capital account's GDP-ratio and absolute
        # report variables. Under the dataset's ngovpayrule 2 the GDP ratio is
        # pinned, so freeing only IADJ leaves the period system over-determined
        # and Walras' law blows up rather than failing cleanly.
        unfx("IADJ", "ngovz")
        for fc in S["fcapng"]:
            unfx("NGOVPAYGDP", fc)
            unfx("NGOVPAYABS", fc)
    elif siclos == 2:                     # savings rate clears (scaled MPS)
        unfx("MPSSCAL", t)
        unfx("NGOVPAYGDP", "savngov")
        unfx("NGOVPAYABS", "savngov")
    else:
        raise NotImplementedError(f"siclos={siclos} not ported")

    # ---- facclos.inc: factor markets --------------------------------------
    fx("LABPARTRAT", t)                   # QLABSCAL flexes instead
    for f in F:
        # facclos.inc forces facclossim=0 for factors outside the SAM: their
        # WFDIST is free (no factor market to arbitrage) and WF is exogenous.
        r = 0 if f in FNSAM else rule("facclos0", f)
        if f in FNSAM:
            fx("WF", f)
        if r == 1:                        # mobile, full employment
            for a in A:
                fx("WFDIST", f, a)
            fx("QFINSSCAL", f)
            fx("UERAT", f)
        elif r == 2:                      # sector-specific, full employment
            fx("WF", f)
            for a in A:
                fx("QF", f, a)
            fx("UERAT", f)
        elif r == 3:                      # mobile, fixed real wage
            for a in A:
                fx("WFDIST", f, a)
            fx("UERAT", f)
        elif r == 4:                      # mobile, wage curve
            for a in A:
                fx("WFDIST", f, a)
            fx("QFINSSCAL", f)

    # Idle-resource closure (not a GAMS facclos rule; used by the backcast).
    # A facclos-1 factor is mobile and fully employed: its price clears the
    # market whatever the demand. For a natural resource whose export volume
    # is pinned below capacity that is the wrong closure -- the price is driven
    # to zero and the solve breaks (APPENDIX.md, A.5.7, `combi`). Here the roles
    # swap: the rent is held at its reference path and EQ_FACEQ determines
    # utilisation through UERAT, so gold that cannot be sold at the going rent
    # stays in the ground. WFDIST and QFINSSCAL stay fixed as in rule 1.
    # It is a complementarity, UERAT >= 0 perpendicular to WF >= reservation
    # rent, so the idle branch applies per (factor, period): when demand at the
    # reservation rent exceeds the endowment the cell reverts to full
    # employment with the rent free (rule 1). `cal.idle_cells` holds the cells
    # currently on the idle branch; the backcast runner sweeps it.
    idle_cells = getattr(cal, "idle_cells", None)
    for f in getattr(cal, "idle_factors", ()):
        if idle_cells is not None and (f, t) not in idle_cells:
            continue
        fx("WF", f)
        unfx("UERAT", f)

    # facclos.inc tail: in the dynamic model capital is always sector-specific
    # and its stock is predetermined, so WF is exogenous, WFDIST arbitrages
    # across activities, and QF is fixed only in the base year.
    for f in FCAP:
        fx("WF", f)
        for a in A:
            if cal.QF00.get((f, a)):
                unfx("WFDIST", f, a)
            if t == tmin:
                fx("QF", f, a)
            else:
                unfx("QF", f, a)

    # ---- mod.gms 3877-3915: entries whose BASE value is zero --------------
    # "The parts of the declared domains of variables with a zero base value
    # are fixed at zero and never part of the model." Note the condition is on
    # the base value of the *governing* quantity, not of the variable itself:
    # FPRDA/WFA/WFDIST are keyed off QF00, PDD/PDS/QD off QD00, and so on, so
    # e.g. FPRDA is fixed at zero for an unused factor cell even though its own
    # base value is one.
    def zfx(name, keys, base):
        for k in keys:
            if not base.get(k):
                fixed.add((name, k))

    zfx("DKINS", [(i2, f) for i2 in IN2 for f in F], cal.DKINS00)
    zfx("MPS", S["insdng"], cal.MPS00)
    zfx("SAV", S["insdng"], cal.SAV00)
    zfx("QA", A, cal.QA00)
    zfx("PVA", A, cal.PVA00)
    zfx("QG", C, cal.QG00)
    zfx("QINV", C, cal.QINV00)
    zfx("QQ", C, cal.QQ00)
    zfx("PQS", C, cal.QQ00)
    zfx("QT", C, cal.QT00)
    zfx("QX", C, cal.QX00)
    zfx("QD", C, cal.QD00)
    zfx("PDD", C, cal.QD00)
    zfx("PDS", C, cal.QD00)
    zfx("QINT", list(cal.QINT00), cal.QINT00)
    zfx("QH", [(c, h) for c in C for h in H], cal.QH00)
    zfx("PQD", list(cal.PQD00), cal.PQD00)
    zfx("SHIF", [(i, f) for i in INS for f in F], cal.SHIF00)
    zfx("TRII", [(i, ip) for i in INS for ip in S["insdng"]], cal.TRII00)
    zfx("TRNSFR", list(cal.TRNSFR00), cal.TRNSFR00)
    zfx("YF", F, cal.YF00)
    zfx("YI", S["insdng"], cal.YI00)
    zfx("YIF", [(i, f) for i in INS for f in F], cal.YIF00)
    zfx("QFINS", [(i, f) for i in INS for f in F], cal.QFINS00)
    zfx("QFHEND", [(h, f) for h in H for f in F], cal.QFINS00)
    for f in F:
        for a in A:
            if not cal.QF00.get((f, a)):
                fixed.add(("FPRDA", (f, a)))
                fixed.add(("WFA", (f, a)))
                fixed.add(("WFDIST", (f, a)))
    # mod.gms 3784: no factor-use tax on value-added factors outside the SAM
    for f in FVA - FSAM:
        for a in A:
            fixed.add(("TFA", (f, a)))
    # mod.gms 2612-2617: only quota commodities carry an import quota rent.
    # PRQMBAR is the rent rate, paired with EQ_QMCONST in the GAMS MCP, and is
    # zero everywhere else; leaving it free lets it absorb the import price
    # block of any commodity whose imports go slack (PM and PRQMBAR then run
    # off to 1e8 together while QM collapses).
    CMBAR = set(S["cmbar"])
    slack = getattr(cal, "quota_slack", ())
    for c in C:
        if c not in CMBAR or (c, t) in slack:
            # Non-quota commodity, or a quota a scenario has relaxed onto the
            # slack branch of the EQ_QMCONST complementarity: either way the
            # rent is zero and EQ_QMCONST does not fire (see model.residuals).
            fx("PRQMBAR", c)
        if c not in CMBAR:
            fx("YPRQMBART", c)
            for d in list(A) + list(H) + list(S["insgov"]) + ["ngovz"]:
                fixed.add(("YPRQMBAR", (c, d)))
    if not cal.TRSMREC00:
        fx("TRSMREC", t)
    if not cal.INVVAL00:
        fx("INVVAL", t)
    if not cal.INVVALF00:
        fx("INVVALF", t)

    # ---- mod.gms 3091-3143, 3485-3486: base-year dynamic stocks -----------
    if t == tmin:
        for f in F:
            fx("QFHENDSCAL", f)
        for ins in INS:
            for f in FCAP:
                if cal.QFINS00.get((ins, f)):
                    fx("QFINS", ins, f)
        fx("GDEBT", t)
        for i2 in IN2:
            fx("FDEBT", i2)

    # degenerate trade block for a non-traded commodity (GAMS QE.FX/QM.FX/
    # PE.FX/PM.FX where the base quantity is 0). Fixing the zero quantity is
    # what removes the symbolic PE*QE / PM*QM edge, so the export/import price
    # column stops being numerically null and the LU pivot stays regular.
    for c in C:
        if not cal.QE00.get(c):
            fx("QE", c)
            fx("PE", c)
        if not cal.QM00.get(c):
            fx("QM", c)
            fx("PM", c)

    # RoW factor-endowment shares are exogenous paths (mod.gms 2682:
    # SHIF.FX(insrow,f,t) = SHIF0). EQ_SHIFDEF only covers domestic insd;
    # leaving SHIF(row,f) free lets it absorb the domestic rescaling and
    # makes the YIFDEF block inconsistent.
    for r in S["insrow"]:
        for f in S["f"]:
            fixed.add(("SHIF", (r, f)))

    # factor cells never used in the base (GAMS QF.FX(f,a)$(QF00=0)=0).
    # Left free, each such cell appears ONLY in the FACSUP market-clearing sum,
    # so any two of them for the same factor give identical Jacobian columns
    # (an exact swap null-direction; e.g. land between a-food and a-min).
    for f in S["f"]:
        for a in S["a"]:
            if not cal.QF00.get((f, a)):
                fixed.add(("QF", (f, a)))

    # RGDPFC: pass2 frees it (flex gdp); pass1 fixes it. Handle via caller.
    return fixed


def closure_fixed(cal, t, clo):
    """Full fixed set for the period: economic closure + pass1/pass2 switch."""
    fixed = base_closure_fixed(cal, t)
    # Sector-group targets (backcast): when any group's value added is pinned
    # in this period the sector shifters TFPGRP carry the calibration and the
    # aggregate pair reverts to the reference closure -- GDP is an outcome of
    # the sector paths, TFPSCAL stays at zero. Pinning GDP as well would
    # over-determine the system (the groups cover every activity).
    # With `cal.sector_pin_gdp` the aggregate pin stays as well: GDP is pinned
    # through TFPSCAL (every activity) while the targeted groups' TFPGRP
    # compensates inside them, so the untargeted sectors carry the aggregate
    # productivity residual. Only valid while the groups do not cover every
    # activity.
    # informal fuel (gemcore/fuel.py): fixed at zero on the inactive branch
    fcfg = getattr(cal, "fuel", None)
    if fcfg and fcfg.informal and (fcfg.fuel, t) not in getattr(cal, "inf_active", set()):
        fixed.add(("QMI", fcfg.fuel))
    va_target = getattr(cal, "va_target", {})
    sector_cal = (any(tt == t for (_, tt) in va_target)
                  and not getattr(cal, "sector_pin_gdp", False))
    if clo.dcal01 and not sector_cal:   # calibration pass: fix RGDPFC, flex TFP
        fixed.add(("RGDPFC", t))
        fixed.discard(("TFPSCAL", t))
    else:                               # reference pass: flex RGDPFC, fix TFP
        fixed.add(("TFPSCAL", t))
        fixed.discard(("RGDPFC", t))
    return fixed


def solve_period_casadi(cal, t, V, L, clo, partition=None, verbose=False,
                        tol=1e-10):
    """Solve one period with CasADi: the SAME residual code is traced with SX
    symbols for the free variables, giving a sparse AD Jacobian and a fast
    Newton rootfinder. Returns (Vsol, info, partition)."""
    import casadi as ca
    cal._solve_t = t
    if partition is None:
        cf = closure_fixed(cal, t, clo)
        free, fixed, kept, allkeys = build_partition(cal, t, V, L, clo, cf)
        partition = (free, fixed, kept, allkeys)
    free, fixed, kept, allkeys = partition
    n = len(free)

    x = ca.SX.sym("x", n)
    # symbolic state: free entries -> x[i], others -> float
    Vsym = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsym[nm][idx] = x[i]

    R = residuals(Vsym, cal, t, L, clo)
    g = ca.vertcat(*[R[k] for k in kept])
    f = ca.Function("f", [x], [g])

    x0 = ca.DM([V[nm][idx] for (nm, idx) in free])
    rf = ca.rootfinder("rf", "newton", {"x": x, "g": g},
                       {"abstol": tol, "max_iter": 200,
                        "error_on_fail": False})
    sol = rf(x0=x0)
    xs = sol["x"]
    res_kept = float(ca.norm_inf(f(xs)))

    Vsol = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsol[nm][idx] = float(xs[i])
    Rall = residuals(Vsol, cal, t, L, clo)
    import numpy as _np
    res_all = float(_np.max(_np.abs([Rall[k] for k in allkeys])))
    info = {"N": n, "max_resid_kept": res_kept, "max_resid_all": res_all,
            "success": res_kept < 1e-6}
    if verbose:
        print(f"[casadi t={t}] N={n} max|res kept|={res_kept:.2e} "
              f"all={res_all:.2e}")
    return Vsol, info, partition


# variable families constrained strictly positive (prices & quantities)
POSITIVE_FAMILIES = {
    "PA", "PVA", "PX", "PDS", "PDD", "PE", "PM", "PQS", "PQD", "PK",
    "PWE", "PWM", "WF", "WFA", "WFAVG", "CPI", "DPI", "EXR", "REXR", "PREXR",
    "QA", "QX", "QD", "QE", "QM", "QQ", "QT", "QINT", "QF", "QG", "QH",
    "QINV", "QNGO", "QTRST", "QXAC", "PXAC", "QFS", "QFINS", "QFHEND",
    "YF", "YI", "YG", "EG", "EH", "GDPMP", "RGDPFC", "RGDPMP", "RGDPPC",
    "ABSNOM", "TFP", "PK", "TFPGRP", "FXI", "VXI", "HXI",
}


def unbounded_below(cal):
    """Entries whose positivity floor GAMS explicitly removes.

    `facclos.inc` rule 1 -- the mobile, fully-employed factor -- sets
    `WF.LO(f,t) = -INF` and `QF.LO(f,a,t) = -INF`, overriding the global
    `WF.LO(f) = lowlim` at mod.gms 3690. So for a facclos-1 factor the wage may
    take any value the market needs, negative included.

    This matters for `f-nrmin`, the only facclos-1 factor here. Tripling the
    mining endowment (scenario `combi`) drives its marginal product toward zero;
    with a 1e-9 floor in place the wage grinds into the bound, the market cannot
    clear, and every period from 2026 fails with residuals around 1e-1. It also
    silently destroys the resource rent -- about 6.2 units of household income --
    which turned a GDP expansion into a 14% contraction.
    """
    out = set()
    fac = {k[0] if isinstance(k, tuple) else k: v
           for k, v in cal.db.pars.get("facclos0", {}).items()}
    A = cal.db.sets["a"]
    for f, rule in fac.items():
        if int(rule) != 1:
            continue
        out.add(("WF", f))
        for a in A:
            out.add(("QF", (f, a)))
    return out


def log_scaled(cal):
    """Factors whose wage can approach zero. NOT CURRENTLY USED -- see below.

    Kept because the reasoning is worth not repeating: solving these in log
    space was tried and FAILS. The derivative with respect to w = log(WF) is
    dR/dWF * WF, so as WF approaches zero the column vanishes identically and
    the Jacobian goes exactly singular -- scipy reports MatrixRankWarning and
    the solve falls back at 1e-1, worse than in levels. Logs put the
    equilibrium at w = -infinity, which is unreachable. Do not retry this.

    The original reasoning, which is sound but does not apply here:

    `EQ_FACDEM` sets `WFA` equal to a product of strictly positive terms, so
    `WFA = WF*WFDIST*(1+TFA)` is strictly positive and `WF` can never be zero or
    negative however the bounds are set. For a facclos-1 factor whose supply a
    scenario expands sharply, though, the equilibrium wage becomes vanishingly
    small: tripling the `f-nrmin` endowment in `combi` drives it from 0.769
    toward zero, and with `rho_va = 1` the quantity enters as `QF^-2`, so the
    equation is badly conditioned there. In levels the Newton step overshoots
    straight through zero into a region the model forbids.

    Substituting `WF = exp(w)` and solving for `w` fixes both halves of that:
    positivity is structural rather than imposed, and the near-zero region is
    stretched out so a bounded step in `w` is a small multiplicative step in
    `WF`. Only the factors that can actually approach zero are transformed --
    doing it globally would change the scaling of a base run that already
    converges at 1e-13.
    """
    out = set()
    fac = {k[0] if isinstance(k, tuple) else k: v
           for k, v in cal.db.pars.get("facclos0", {}).items()}
    for f, rule in fac.items():
        if int(rule) == 1:
            out.add(("WF", f))
    return out


def _ruiz(J, iters=5):
    """Ruiz inf-norm equilibration. Returns (dr, dc): row and column scale
    vectors such that diag(dr) J diag(dc) has rows and columns of ~unit
    inf-norm. Robustly conditions the CGE Jacobian for a direct solve."""
    import numpy as _np
    import scipy.sparse as sp
    A = J.tocsr()
    m, n = A.shape
    dr = _np.ones(m)
    dc = _np.ones(n)
    for _ in range(iters):
        absA = abs(A)
        rmax = _np.asarray(absA.max(axis=1).todense()).ravel()
        cmax = _np.asarray(absA.max(axis=0).todense()).ravel()
        rmax[rmax == 0] = 1.0
        cmax[cmax == 0] = 1.0
        sr = 1.0 / _np.sqrt(rmax)
        sc = 1.0 / _np.sqrt(cmax)
        A = (sp.diags(sr) @ A @ sp.diags(sc)).tocsr()
        dr *= sr
        dc *= sc
    return dr, dc


def solve_period_lsq(cal, t, V, L, clo, partition=None, verbose=False,
                     tol=1e-11, maxit=300):
    """Solve one period with a trust-region least-squares solver
    (scipy least_squares, method='trf') using the analytic sparse Jacobian
    from CasADi and x_scale='jac' (automatic affine variable scaling). This
    is robust to the CGE debt-block ill-conditioning that stalls a plain
    Newton, and the trust region prevents NaN blow-ups. Returns
    (Vsol, info, partition)."""
    import casadi as ca
    import numpy as _np
    import scipy.sparse as sp
    from scipy.optimize import least_squares
    cal._solve_t = t
    if partition is None:
        cf = closure_fixed(cal, t, clo)
        partition = build_partition(cal, t, V, L, clo, cf)
    free, fixed, kept, allkeys = partition
    n = len(free)

    x = ca.SX.sym("x", n)
    Vsym = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsym[nm][idx] = x[i]
    R = residuals(Vsym, cal, t, L, clo)
    g = ca.vertcat(*[R[k] for k in kept])
    Ff = ca.Function("F", [x], [g])
    Jca = ca.Function("J", [x], [ca.jacobian(g, x)])

    x0 = _np.array([V[nm][idx] for (nm, idx) in free], dtype=float)
    lo = _np.full(n, -_np.inf)
    hi = _np.full(n, _np.inf)
    free_lo = unbounded_below(cal)
    for i, (nm, idx) in enumerate(free):
        if nm in POSITIVE_FAMILIES and (nm, idx) not in free_lo:
            lo[i] = 1e-9
    x0 = _np.clip(x0, lo + 1e-12, hi)

    def fun(xx):
        return _np.asarray(Ff(xx)).ravel()

    def jac(xx):
        J = Jca(xx)
        return sp.csr_matrix(J.sparse())

    sol = least_squares(fun, x0, jac=jac, bounds=(lo, hi), method="trf",
                        x_scale="jac", tr_solver="lsmr",
                        ftol=tol, xtol=tol, gtol=tol, max_nfev=maxit,
                        verbose=0)

    Vsol = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsol[nm][idx] = float(sol.x[i])
    Rall = residuals(Vsol, cal, t, L, clo)
    res_all = float(_np.max(_np.abs([Rall[k] for k in allkeys])))
    info = {"N": n, "max_resid_all": res_all,
            "success": bool(res_all < 1e-6), "nfev": sol.nfev,
            "cost": float(sol.cost)}
    if verbose:
        print(f"[lsq t={t}] N={n} nfev={sol.nfev} max|res all|={res_all:.2e}")
    return Vsol, info, partition


def solve_period_newton(cal, t, V, L, clo, partition=None, verbose=False,
                        tol=1e-8, maxit=80):
    """Damped Newton with a sparse LU linear solve and backtracking line
    search. CasADi provides fast residual + analytic sparse Jacobian; we own
    the step control so we can damp and keep prices/quantities positive.
    Robust for the close, warm-started starts of a recursive base run."""
    import casadi as ca
    import numpy as _np
    import scipy.sparse as sp
    import scipy.sparse.linalg as spla
    cal._solve_t = t
    if partition is None:
        cf = closure_fixed(cal, t, clo)
        partition = build_partition(cal, t, V, L, clo, cf)
    free, fixed, kept, allkeys = partition
    n = len(free)

    x = ca.SX.sym("x", n)
    Vsym = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsym[nm][idx] = x[i]
    R = residuals(Vsym, cal, t, L, clo)
    g = ca.vertcat(*[R[k] for k in kept])
    Ff = ca.Function("F", [x], [g])
    Jf = ca.Function("J", [x], [ca.jacobian(g, x)])

    # lower bounds keeping positive families strictly positive
    lo = _np.full(n, -_np.inf)
    free_lo = unbounded_below(cal)
    for i, (nm, idx) in enumerate(free):
        if nm in POSITIVE_FAMILIES and (nm, idx) not in free_lo:
            lo[i] = 1e-9

    def merit(F):
        return 0.5 * float(F @ F)

    xk = _np.array([V[nm][idx] for (nm, idx) in free], dtype=float)
    Fk = _np.asarray(Ff(xk)).ravel()
    nrm = _np.max(_np.abs(Fk))
    mk = merit(Fk)
    converged = False
    lam = 1e-6                        # Levenberg-Marquardt damping, adaptive

    def try_step(dx, gTd, mk, xk):
        """Armijo backtracking on the least-squares merit 1/2||F||^2, keeping
        prices and quantities inside their positive domain. Returns
        (accepted, x, F, step)."""
        step = 1.0
        for _ls in range(50):
            xn = xk + step * dx
            bad = xn < lo
            if bad.any():
                xn[bad] = _np.maximum(xk[bad] - 0.5 * (xk[bad] - lo[bad]),
                                      lo[bad])
            Fn = _np.asarray(Ff(xn)).ravel()
            if (_np.all(_np.isfinite(Fn))
                    and merit(Fn) <= mk + 1e-4 * step * gTd):
                return True, xn, Fn, step
            step *= 0.5
        return False, xk, Fk, 0.0

    for it in range(maxit):
        if nrm < tol:
            converged = True
            break
        J = sp.csc_matrix(Jf(xk).sparse())
        # Ruiz equilibration on the (null-free, but scale-disparate) Jacobian:
        # solve (Dr J Dc) y = -Dr F, then dx = Dc y. With the numeric-null
        # columns already stripped by the partition, the equilibrated LU
        # factorization is regular and the step accurate.
        dr, dc = _ruiz(J)
        Js = (sp.diags(dr) @ J @ sp.diags(dc)).tocsc()
        rhs = -dr * Fk
        try:
            y = spla.spsolve(Js, rhs)
            ok = _np.all(_np.isfinite(y))
        except (RuntimeError, ValueError):
            ok = False
        if not ok:
            y = spla.lsmr(Js, rhs, damp=1e-10, atol=1e-14, btol=1e-14,
                          maxiter=8000)[0]
        dx = dc * y
        gTd = float(Fk @ (J @ dx))    # directional derivative of merit
        accepted, xn, Fn, step = try_step(dx, gTd, mk, xk)

        if not accepted:
            # The Newton direction overshoots. Far from the solution the
            # recursive-dynamic system is stiff enough that the full step
            # leaves the CES/CET domains entirely, and halving it just walks
            # a bad direction more slowly. Fall back to a Levenberg-Marquardt
            # step on the equilibrated normal equations,
            #     (A + lam*diag(A)) y = Js' * rhs,   A = Js'Js,
            # which interpolates between Newton (lam -> 0) and scaled steepest
            # descent (lam large) and always yields a descent direction.
            # Damping by diag(A) rather than the identity keeps lam
            # dimensionless across the price and quantity blocks.
            A = (Js.T @ Js).tocsc()
            diagA = _np.asarray(A.diagonal()).ravel()
            diagA[diagA <= 0] = 1.0
            g = Js.T @ rhs
            while lam < 1e8 and not accepted:
                try:
                    y = spla.spsolve((A + lam * sp.diags(diagA)).tocsc(), g)
                    good = _np.all(_np.isfinite(y))
                except (RuntimeError, ValueError):
                    good = False
                if good:
                    dx = dc * y
                    gTd = float(Fk @ (J @ dx))
                    accepted, xn, Fn, step = try_step(dx, gTd, mk, xk)
                if not accepted:
                    lam *= 10.0
            if accepted:
                lam = max(lam / 3.0, 1e-12)
        else:
            lam = max(lam / 3.0, 1e-12)

        if not accepted:
            break
        xk, Fk, mk = xn, Fn, merit(Fn)
        nrm = _np.max(_np.abs(Fk))
        if verbose and (it < 5 or it % 10 == 0):
            print(f"    newton it={it} ||F||={nrm:.2e} step={step:.2g} "
                  f"lam={lam:.1e}")

    Vsol = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsol[nm][idx] = float(xk[i])
    Rall = residuals(Vsol, cal, t, L, clo)
    res_all = float(_np.max(_np.abs([Rall[k] for k in allkeys])))
    info = {"N": n, "max_resid_all": res_all,
            "success": bool(converged and res_all < 1e-6), "iters": it}
    if verbose:
        print(f"[newton t={t}] N={n} it={it} max|res all|={res_all:.2e}")
    return Vsol, info, partition


def solve_period_kinsol(cal, t, V, L, clo, partition=None, verbose=False,
                        tol=1e-10, strategy="linesearch"):
    """Solve one period with CasADi's KINSOL (SUNDIALS) rootfinder — a
    line-searched Newton for square systems, robust for close starts (each
    dynamic period starts from the previous solution). Returns
    (Vsol, info, partition)."""
    import casadi as ca
    import numpy as _np
    cal._solve_t = t
    if partition is None:
        cf = closure_fixed(cal, t, clo)
        partition = build_partition(cal, t, V, L, clo, cf)
    free, fixed, kept, allkeys = partition
    n = len(free)

    x = ca.SX.sym("x", n)
    Vsym = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsym[nm][idx] = x[i]
    R = residuals(Vsym, cal, t, L, clo)
    g = ca.vertcat(*[R[k] for k in kept])

    x0 = ca.DM([V[nm][idx] for (nm, idx) in free])
    try:
        rf = ca.rootfinder("rf", "kinsol", {"x": x, "g": g},
                           {"strategy": strategy, "abstol": tol,
                            "max_iter": 1000, "print_level": 0,
                            "error_on_fail": False})
        xs = rf(x0=x0)["x"]
    except Exception as e:
        if verbose:
            print(f"[kinsol t={t}] failed: {e}; falling back to newton")
        rf = ca.rootfinder("rf", "newton", {"x": x, "g": g},
                           {"abstol": tol, "max_iter": 500,
                            "error_on_fail": False})
        xs = rf(x0=x0)["x"]

    Vsol = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsol[nm][idx] = float(xs[i])
    Rall = residuals(Vsol, cal, t, L, clo)
    res_all = float(_np.max(_np.abs([Rall[k] for k in allkeys])))
    info = {"N": n, "max_resid_all": res_all, "success": res_all < 1e-6}
    if verbose:
        print(f"[kinsol t={t}] N={n} max|res all|={res_all:.2e}")
    return Vsol, info, partition


def solve_period_ipopt(cal, t, V, L, clo, partition=None, verbose=False,
                       tol=1e-9):
    """Robust period solve via IPOPT (feasibility problem, min 0 s.t. g=0)
    with positivity bounds on prices/quantities. Handles the CES/CET power
    domains where a plain Newton would step to NaN."""
    import casadi as ca
    import numpy as _np
    cal._solve_t = t
    if partition is None:
        cf = closure_fixed(cal, t, clo)
        free, fixed, kept, allkeys = build_partition(cal, t, V, L, clo, cf)
        partition = (free, fixed, kept, allkeys)
    free, fixed, kept, allkeys = partition
    n = len(free)

    x = ca.SX.sym("x", n)
    Vsym = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsym[nm][idx] = x[i]
    R = residuals(Vsym, cal, t, L, clo)
    g = ca.vertcat(*[R[k] for k in kept])

    x0v = _np.array([V[nm][idx] for (nm, idx) in free], dtype=float)
    xsc = _np.maximum(_np.abs(x0v), 1e-3)
    lbx = _np.full(n, -1e12)
    ubx = _np.full(n, 1e12)
    free_lo = unbounded_below(cal)
    for i, (nm, idx) in enumerate(free):
        if nm in POSITIVE_FAMILIES and (nm, idx) not in free_lo:
            lbx[i] = 1e-7

    # small guess-anchoring regularizer (scaled) makes root selection well-
    # posed and steers IPOPT to the equilibrium nearest the warm start.
    obj = 1e-6 * ca.sumsqr((x - ca.DM(x0v)) / ca.DM(xsc))
    nlp = {"x": x, "f": obj, "g": g}
    opts = {"ipopt.tol": tol, "ipopt.print_level": 0, "print_time": 0,
            "ipopt.max_iter": 1000, "ipopt.sb": "yes",
            "ipopt.linear_solver": "mumps",
            "ipopt.mu_strategy": "adaptive",
            "ipopt.nlp_scaling_method": "gradient-based"}
    solver = ca.nlpsol("solver", "ipopt", nlp, opts)
    sol = solver(x0=ca.DM(x0v), lbx=lbx, ubx=ubx, lbg=0, ubg=0)
    xs = sol["x"]

    Vsol = {nm: dict(d) for nm, d in V.items()}
    for i, (nm, idx) in enumerate(free):
        Vsol[nm][idx] = float(xs[i])
    Rall = residuals(Vsol, cal, t, L, clo)
    res_all = float(_np.max(_np.abs([Rall[k] for k in allkeys])))
    ok = solver.stats()["success"]
    info = {"N": n, "max_resid_all": res_all, "success": bool(ok)}
    if verbose:
        print(f"[ipopt t={t}] N={n} success={ok} max|res all|={res_all:.2e}")
    return Vsol, info, partition


def solve_period(cal, t, V, L, clo, partition=None, verbose=False,
                 method="hybr", tol=1e-11):
    """Solve the period-t system. Returns (Vsol, info, partition).

    partition (free, fixed, kept_eqs) may be reused across periods with the
    same structure to skip the Jacobian-sparsity sweep.
    """
    cal._solve_t = t
    if partition is None:
        cf = closure_fixed(cal, t, clo)
        free, fixed, kept, allkeys = build_partition(cal, t, V, L, clo, cf)
        partition = (free, fixed, kept, allkeys)
    free, fixed, kept, allkeys = partition

    x0 = np.array([V[n][i] for (n, i) in free], dtype=float)

    def unpack(x):
        Vx = {n: dict(d) for n, d in V.items()}
        for (n, i), xi in zip(free, x):
            Vx[n][i] = xi
        return Vx

    def resid_vec(x):
        R = residuals(unpack(x), cal, t, L, clo)
        return np.array([R[k] for k in kept], dtype=float)

    n = len(free)
    if verbose:
        print(f"[solve t={t}] square N={n} (kept eq={len(kept)}, "
              f"dropped={len(allkeys) - len(kept)})")

    sol = root(resid_vec, x0, method=method,
               options=({"xtol": tol, "maxfev": 100 * (n + 1)}
                        if method == "hybr" else {"xtol": tol}))
    Vsol = unpack(sol.x)
    # residual over ALL equations (incl. dropped) to confirm redundancy
    Rall = residuals(Vsol, cal, t, L, clo)
    allres = np.array([Rall[k] for k in allkeys], dtype=float)
    info = {"success": sol.success, "N": n,
            "max_resid_kept": float(np.max(np.abs(resid_vec(sol.x)))),
            "max_resid_all": float(np.max(np.abs(allres))),
            "message": sol.message}
    return Vsol, info, partition
