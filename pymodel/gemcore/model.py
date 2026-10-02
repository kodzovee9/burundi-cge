"""GEM-Core within-period equation system (port of mod.gms equation block,
lines 2328-3488). Builds the residual vector F(vars; params, lags) for a
single period tcur, replicating the GAMS MODEL GEM.

Design:
  - A `State` holds the *level* of every model variable at the current period
    as plain Python floats (dict keyed by index-tuple). Between-period lags
    are supplied separately (prev-period State).
  - `residuals(state, cal, t, tprev_state, closure)` returns an ordered dict
    {eqname: value}. At the calibrated base-year point every residual is ~0.
  - The same residual code drives the CasADi solve: `solve_period` packs the
    *free* variables (per closure) into a vector, wraps `residuals` through a
    numeric bridge, and calls a Newton rootfinder.

Only what bdi2019 needs is fully fleshed out; emissions and a few unused
report-only equations are computed post-solve, not in the square system.
"""

from __future__ import annotations

from collections import OrderedDict

from .calibration import G
from . import fuel as fuel_mod


# ---------------------------------------------------------------------------
# Variable registry: name -> index domain (tuple of set names) at a period.
# Levels initialised from calibration "0" paths in state.py.
# ---------------------------------------------------------------------------

def _pow(base, exp):
    # GAMS ** with possible negative base guarded by calibrated masks;
    # base is positive at solution, so plain power is fine.
    return base ** exp


def residuals(V, cal, t, L, clo):
    """Return OrderedDict of equation residuals at period t.

    V : dict name -> dict[idxtuple -> float]   (current-period levels)
    L : dict name -> dict[idxtuple -> float]   (previous-period levels; {} at tmin)
    clo : closure flags/paths (see closures.py)
    """
    S = cal.db.sets
    A, C, F, H = S["a"], S["c"], S["f"], S["h"]
    F1, F2, F3 = S["f1"], S["f2"], S["f3"]
    FLEO = set(S.get("fleo", []))
    FVA, FSAM, FNSAM = set(S["fva"]), set(S["fsam"]), set(S["fnsam"])
    FCAP, FCAPG, FCAPNG = S["fcap"], S["fcapg"], S["fcapng"]
    FLAB, FNCAP = S["flab"], S["fncap"]
    FUENDOG = set(S["fuendog"])
    INS, INSD, INSDNG = S["ins"], S["insd"], S["insdng"]
    INSGOV, INSROW, INSNGO, INSTRST = (S["insgov"], S["insrow"],
                                       S["insngo"], S["instrst"])
    INSDNGNH, INSDNH, INSNH = S["insdngnh"], S["insdnh"], S["insnh"]
    CT = S["ct"]
    CMBAR = set(S["cmbar"])
    CED, CESEXOG = set(S["ced"]), set(S["cesexog"])
    ACNT = S["acnt"]
    IN2 = S["ins2"]
    TACD, TACM, TACE = S["tacd"], S["tacm"], S["tace"]
    D = S["d"]
    tmin = S["tmin"][0]
    is_tmin = (t == tmin)

    mf2f1 = cal.mf2f1
    mf3f2 = cal.mf3f2
    mfcapinv = cal.mfcapinv
    mtaxfa_set = set(map(tuple, cal.mtaxfa))
    mcapins_set = set(map(tuple, cal.mcapins))

    R = OrderedDict()

    def v(name, *idx):
        return V[name].get(idx if len(idx) != 1 else idx[0], 0.0)

    def lag(name, *idx):
        return L.get(name, {}).get(idx if len(idx) != 1 else idx[0], 0.0)

    # convenient calibrated-parameter accessors
    QF00 = cal.QF00
    QM00, QE00, QD00, QQ00, QX00 = (cal.QM00, cal.QE00, cal.QD00,
                                    cal.QQ00, cal.QX00)
    QXAC00, QINT00 = cal.QXAC00, cal.QINT00
    QG00, QH00, QINV00 = cal.QG00, cal.QH00, cal.QINV00
    QNGO00, QTRST00 = cal.QNGO00, cal.QTRST00
    PQD00, PVA00, PWE00, PWM00, PE00, PM00 = (cal.PQD00, cal.PVA00, cal.PWE00,
                                              cal.PWM00, cal.PE00, cal.PM00)
    QFINS00, YIF00, YF00, YI00 = cal.QFINS00, cal.YIF00, cal.YF00, cal.YI00
    SHIF00, TRII00, SAV00, MPS00 = cal.SHIF00, cal.TRII00, cal.SAV00, cal.MPS00
    TRNSFR00 = cal.TRNSFR00
    capcomp, deprcap = cal.capcomp, cal.deprcap

    # fuel-shortage mechanisms (gemcore/fuel.py); all inert when cal.fuel is None
    fcfg = getattr(cal, "fuel", None)
    FUEL = fcfg.fuel if fcfg else None
    FAF = set(fcfg.AF) if (fcfg and fcfg.sigma_act) else set()
    INF_ON = bool(fcfg and fcfg.informal)
    HH_ON = bool(fcfg and fcfg.eps_hh)

    def vq(a):
        """Value-added quantity: QA, times VXI for fuel-substituting activities."""
        return v("QA", a) * v("VXI", a) if a in FAF else v("QA", a)

    # Scenario technology shifters (port-only; 1 in the base and reference):
    # input coefficients (e.g. hydropower replacing diesel in electricity,
    # better roads cutting transport inputs) and trade/transport margins.
    _icas = getattr(cal, "ica_scale", None) or {}
    _mgs = getattr(cal, "margin_scale", None) or {}

    def ica(c, a):
        return cal.ica00.get((c, a), 0.0) * _icas.get((c, a, t), 1.0)

    def icm(ct, c):
        return cal.icm.get((ct, c), 0.0) * _mgs.get((ct, c, t), 1.0)

    def ice(ct, c):
        return cal.ice.get((ct, c), 0.0) * _mgs.get((ct, c, t), 1.0)

    def icd(ct, c):
        return cal.icd.get((ct, c), 0.0) * _mgs.get((ct, c, t), 1.0)

    # ==== PRODUCTION ========================================================
    for a in A:
        for f in F:
            if QF00.get((f, a)):
                R[("WFADEF", f, a)] = (
                    v("WFA", f, a)
                    - v("WF", f) * v("WFDIST", f, a) * (1 + v("TFA", f, a)))

    # level-1 production fn (dcal01: two forms). Use EQ_PRODFN1 form (calib
    # pass) when clo.dcal01 else EQ_PRODFN with gov-capital MP term.
    for a in A:
        if not any(cal.db.sam(f, a) for f in F):
            continue
        sig = cal.sigma_va[a]
        rho = cal.rho_va[a]
        agg = sum(cal.delta_va[(f, a)]
                  * (v("FPRDA", f, a) * v("QF", f, a)) ** (-rho)
                  for f in F1 if f not in FLEO and QF00.get((f, a)))
        core = v("TFP", a) * cal.phi_va[a] * agg ** (-1 / rho)
        if clo.dcal01:
            R[("PRODFN", a)] = vq(a) - core
        else:
            extra = sum((sum(v("QFINS", ins, fc) for ins in INS)
                         - sum(cal.QFINS0[(ins, fc, t)] for ins in INS))
                        * cal.mpk[(a, fc, t)]
                        for fc in FCAP
                        if cal.mpcapgov.get(fc) and cal.mtfp.get((a, fc)))
            R[("PRODFN", a)] = vq(a) - core - extra

    # level-1 factor demand FOC
    for a in A:
        for f in F1:
            if f in FLEO or not QF00.get((f, a)):
                continue
            rho = cal.rho_va[a]
            denom = sum(cal.delta_va[(fp, a)]
                        * (v("FPRDA", fp, a) * v("QF", fp, a)) ** (-rho)
                        for fp in F if fp not in FLEO and (fp, a) in cal.delta_va)
            R[("FACDEM", f, a)] = (
                v("WFA", f, a)
                - v("PVA", a) * vq(a) * denom ** (-1)
                * cal.delta_va[(f, a)] * v("QF", f, a) ** (-rho - 1)
                * v("FPRDA", f, a) ** (-rho))

    # Leontief factor demand
    for a in A:
        for f in F:
            if f in FLEO and QF00.get((f, a)):
                fl01 = G(cal.db.pars.get("fleo01", {}), f, a)
                R[("FACDEMLEO", f, a)] = (
                    v("QF", f, a)
                    - (cal.ifa0.get((f, a), 0.0)
                       / (1 + fl01 * (v("TFP", a) / cal.TFP00[a] - 1)))
                    * vq(a))

    # level-2 production fn: aggregate factor f1 (fnsam) from its f2 children
    for a in A:
        for f1 in F1:
            if f1 in FNSAM and QF00.get((f1, a)) and cal.sigma2.get((f1, a)):
                rho = cal.rho2[(f1, a)]
                R[("PRODFNCES2", f1, a)] = (
                    v("QF", f1, a) - cal.phi2[(f1, a)]
                    * sum(cal.delta2[(f2, a)]
                          * (v("FPRDA", f2, a) * v("QF", f2, a)) ** (-rho)
                          for (f2, ff1) in mf2f1 if ff1 == f1)
                    ** (-1 / rho))
    # level-2 factor demand FOC: demand for f2 given its parent f1
    for a in A:
        for f2 in F2:
            if not QF00.get((f2, a)):
                continue
            sig1 = sum(cal.sigma2.get((f1, a), 0.0)
                       for (ff2, f1) in mf2f1 if ff2 == f2)
            R[("FACDEMCES2", f2, a)] = (
                v("QF", f2, a)
                - (sum(v("WFA", f1, a) for (ff2, f1) in mf2f1 if ff2 == f2)
                   / v("WFA", f2, a)) ** sig1
                * cal.delta2.get((f2, a), 0.0) ** sig1
                * sum(cal.phi2.get((f1, a), 1.0) ** (cal.sigma2.get((f1, a), 0.0) - 1)
                      for (ff2, f1) in mf2f1 if ff2 == f2)
                * v("FPRDA", f2, a) ** sum(cal.sigma2.get((f1, a), 0.0) - 1
                                           for (ff2, f1) in mf2f1 if ff2 == f2)
                * sum(v("QF", f1, a) for (ff2, f1) in mf2f1 if ff2 == f2))
    # level-3 production fn: aggregate factor f2 (fnsam) from its f3 children
    for a in A:
        for f2 in F2:
            if f2 in FNSAM and QF00.get((f2, a)) and cal.sigma3.get((f2, a)):
                rho = cal.rho3[(f2, a)]
                R[("PRODFNCES3", f2, a)] = (
                    v("QF", f2, a) - cal.phi3[(f2, a)]
                    * sum(cal.delta3[(f3, a)]
                          * (v("FPRDA", f3, a) * v("QF", f3, a)) ** (-rho)
                          for (f3, ff2) in mf3f2 if ff2 == f2)
                    ** (-1 / rho))
    # level-3 factor demand FOC: demand for f3 given its parent f2
    for a in A:
        for f3 in F3:
            if not QF00.get((f3, a)):
                continue
            sig2 = sum(cal.sigma3.get((f2, a), 0.0)
                       for (ff3, f2) in mf3f2 if ff3 == f3)
            R[("FACDEMCES3", f3, a)] = (
                v("QF", f3, a)
                - (sum(v("WFA", f2, a) for (ff3, f2) in mf3f2 if ff3 == f3)
                   / v("WFA", f3, a)) ** sig2
                * cal.delta3.get((f3, a), 0.0) ** sig2
                * sum(cal.phi3.get((f2, a), 1.0) for (ff3, f2) in mf3f2
                      if ff3 == f3) ** (sig2 - 1)
                * v("FPRDA", f3, a) ** sum(cal.sigma3.get((f2, a), 0.0) - 1
                                           for (ff3, f2) in mf3f2 if ff3 == f3)
                * sum(v("QF", f2, a) for (ff3, f2) in mf3f2 if ff3 == f3))

    # TFP definition. `TFPGRP(g)` is the port's sector-group productivity
    # shifter (not in GAMS): a multiplier on TFP for every activity in group g,
    # used by the backcast to back sector supply shocks out of observed sector
    # value-added growth, the way dcal01 backs TFPSCAL out of GDP. It exists
    # only when `cal.tfp_groups` is set; otherwise TFP is exactly mod.gms.
    tfp_groups = getattr(cal, "tfp_groups", {})
    # Premium rent as a real cost (port-only; Krueger 1974). A share
    # `rent_cost` of the premium rent is dissipated in unproductive activity
    # rather than transferred, which in reduced form lowers productivity by
    # that share of the rent's weight in GDP. Normalised to the base year, so
    # it is exactly 1 there; 0 reproduces GEM-Core.
    # Resources leave rent-seeking gradually: the effective rent share moves a
    # fraction `rent_adjust` of the way to the current share each year
    # (partial adjustment, carried as numbers from the previous solution).
    omega = getattr(cal, "rent_cost", 0.0)
    if omega:
        rent_sh = sum(v("YPREXRT", c) for c in C) / v("GDPMP", t)
        lam = getattr(cal, "rent_adjust", 1.0)
        if is_tmin or not L:
            eff = rent_sh
        else:
            eff = (1 - lam) * rent_eff_prev(cal, L, t) + lam * rent_sh
        rent_wedge = (1 - omega * eff) / (1 - omega * cal.rent_share00)
    else:
        rent_wedge = 1.0
    for a in A:
        base = cal.tfpexog[(a, t)] * (1 + v("TFPSCAL", t) * cal.tfp01[a])
        trd = (v("TRDGDP", t) / cal.TRDGDP00) ** G(cal.tfpelas, a, "trdgdp")
        if a in tfp_groups:
            base = base * v("TFPGRP", tfp_groups[a])
        R[("TFPDEF", a)] = v("TFP", a) - base * trd * rent_wedge

    # Sector-group value-added targets. For a targeted (g, t), real value
    # added of the group (PVA00-weighted QA, the backcast's own measure) grows
    # by the observed ratio from the previous period, and TFPGRP(g) is what
    # delivers it. For an untargeted period the shifter holds its previous
    # value (1 at the base year), so productivity does not snap back when the
    # targets end.
    if tfp_groups:
        va_target = getattr(cal, "va_target", {})
        for g in getattr(cal, "tfp_group_list", ()):
            members = [a for a in A if tfp_groups.get(a) == g]
            if (g, t) in va_target and L.get("QA"):
                prev = sum(cal.PVA00[a] * L["QA"][a]
                           * (L.get("VXI", {}).get(a, 1.0) if a in FAF else 1.0)
                           for a in members)
                R[("GRPVADEF", g)] = (
                    sum(cal.PVA00[a] * vq(a) for a in members)
                    - va_target[(g, t)] * prev)
            else:
                hold = L.get("TFPGRP", {}).get(g, 1.0)
                R[("GRPHOLD", g)] = v("TFPGRP", g) - hold

    # FPRDA (exogenous)
    for a in A:
        for f in F:
            if QF00.get((f, a)):
                R[("FPRDADEF", f, a)] = (
                    v("FPRDA", f, a)
                    - clo.fprdab(f, a, t)
                    * (1 + v("FPRDASCAL", t) * G(cal.fprda01, f, a)))

    # intermediate demand
    for a in A:
        for c in C:
            if QINT00.get((c, a)):
                R[("INTDEM", c, a)] = (v("QINT", c, a)
                                       - ica(c, a) * v("QA", a)
                                       * (v("FXI", a) if (c == FUEL and a in FAF) else 1.0))

    # fuel-value-added substitution (gemcore/fuel.py, option 1)
    for a in FAF:
        from .fuel import pvac
        rel = ((v("PQD", FUEL, a) / PQD00[(FUEL, a)])
               / (pvac(v, cal, a) / fcfg.pvac00[a]))
        R[("FUELSUB", a)] = v("FXI", a) / v("VXI", a) - rel ** (-fcfg.sigma_act)
        rho_f = fcfg.rho
        R[("FUELCES", a)] = ((1 - fcfg.sF[a]) * v("VXI", a) ** rho_f
                             + fcfg.sF[a] * v("FXI", a) ** rho_f - 1.0)

    # commodity production
    for a in A:
        for c in C:
            if QXAC00.get((a, c)):
                R[("COMPRDFN", a, c)] = (v("QXAC", a, c)
                                         - cal.theta[(a, c)] * v("QA", a))
    for c in C:
        if QX00.get(c):
            rho = cal.rho_ac[c]
            R[("OUTAGGFN", c)] = (
                v("QX", c) - cal.phi_ac[c]
                * sum(cal.delta_ac[(a, c)] * v("QXAC", a, c) ** (-rho)
                      for a in A if (a, c) in cal.delta_ac) ** (-1 / rho))
    for a in A:
        for c in C:
            if QXAC00.get((a, c)):
                sig = cal.sigma_ac[c]
                R[("OUTAGGFOC", a, c)] = (
                    v("QXAC", a, c)
                    - (v("PX", c) / v("PXAC", a, c)) ** sig
                    * cal.delta_ac[(a, c)] ** sig * cal.phi_ac[c] ** (sig - 1)
                    * v("QX", c))

    # PVA and PA
    for a in A:
        qa = v("QA", a)
        vxi = v("VXI", a) if a in FAF else 1.0
        fxi = v("FXI", a) if a in FAF else 1.0
        R[("PVADEF", a)] = (
            v("PA", a) * (1 - v("TA", a)
                          - sum(cal.shfcapga.get((fc, a), 0.0)
                                for fc in FCAPG))
            + sum(v("YPREXR", c, a) for c in C) / qa
            + sum(v("YPRQMBAR", c, a) for c in C) / qa
            - v("PVA", a) * vxi
            - sum(v("WFA", f, a) * cal.ifa0.get((f, a), 0.0) for f in F
                  if f in FLEO) * vxi
            - sum(v("PQD", c, a) * ica(c, a)
                  * (fxi if c == FUEL else 1.0) for c in C)
            + v("RBTVAT", a) / qa)
    for a in A:
        R[("PADEF", a)] = v("PA", a) - sum(cal.theta.get((a, c), 0.0)
                                           * v("PXAC", a, c) for c in C)

    # wage curve
    for f in F:
        if f in FUENDOG:
            R[("WAGECURVE", f)] = (
                v("WF", f) / v("CPI", t)
                - (cal.WF00[f] * cal.fprdindex[(f, t)] / cal.CPI00)
                * (v("UERAT", f) / cal.UERAT00[f]) ** cal.eta_wf[f])

    # factor market equilibrium (non-capital)
    for f in F:
        if f in FSAM and f in FVA and f not in set(FCAP):
            R[("FACEQ", f)] = (v("QFS", f) * (1 - v("UERAT", f))
                               - sum(v("QF", f, a) for a in A))

    # factor incomes
    shffp = {("f-capprv", "f-capgov"): 1.0}
    for f in F:
        if YF00.get(f) and f in FSAM and f in FVA:
            R[("YFDEF", f)] = (
                v("YF", f)
                - sum(v("WF", f) * v("WFDIST", f, a) * v("QF", f, a) for a in A)
                - sum(v("TRNSFR", f, r) * v("EXR", t) for r in INSROW)
                - sum(v("YPREXR", c, f) for c in C)
                - sum(v("YF", fg) * shffp.get((f, fg), 0.0) for fg in FCAPG))
    for fg in FCAPG:
        if YF00.get(fg):
            R[("YCAPGDEF", fg)] = (
                v("YF", fg)
                - sum(cal.shfcapga.get((fg, a), 0.0) * v("PA", a) * v("QA", a)
                      for a in A))

    # ==== TRADE =============================================================
    for c in C:
        if QD00[c] > 0 and QM00[c] > 0:
            rho = cal.rho_q[c]
            R[("ARMING", c)] = (
                v("QQ", c) - cal.phi_q[c]
                * (cal.delta_m[c] * v("QM", c) ** (-rho)
                   + cal.delta_dd[c] * v("QD", c) ** (-rho)) ** (-1 / rho))
            R[("IMPDOMRAT", c)] = (
                v("QM", c) / v("QD", c)
                - (v("PDD", c) / v("PM", c) * cal.delta_m[c] / cal.delta_dd[c])
                ** (1 / (1 + rho)))
        elif (QD00[c] > 0) != (QM00[c] > 0):
            R[("ARMING2", c)] = (v("QQ", c) - v("QD", c) - v("QM", c)
                                 - (v("QMI", c) if (INF_ON and c == FUEL) else 0.0))
        if QD00[c]:
            R[("PDDDEF", c)] = (
                v("PDD", c) - v("PDS", c)
                - sum(v("PQD", ct, td) * icd(ct, c)
                      for ct in CT for td in TACD))
        if QQ00[c]:
            R[("ABSORB", c)] = (v("PQS", c) * v("QQ", c)
                                - v("PDD", c) * v("QD", c)
                                - v("PM", c) * v("QM", c)
                                - (v("PM", c) * v("QMI", c)
                                   if (INF_ON and c == FUEL) else 0.0))
        for d in D:
            if PQD00.get((c, d)):
                R[("PQDDEF", c, d)] = (
                    v("PQD", c, d)
                    - v("PQS", c) * (1 + v("TQ", c)) * (1 - v("SUBC", c, d))
                    * (1 + v("TVAC", c, d)))

    for c in C:
        cet_std = QD00[c] > 0 and QE00[c] > 0 and c not in CESEXOG
        if cet_std:
            rho = cal.rho_x[c]
            R[("CET", c)] = (
                v("QX", c) - cal.phi_x[c]
                * (cal.delta_e[c] * v("QE", c) ** rho
                   + cal.delta_ds[c] * v("QD", c) ** rho) ** (1 / rho))
            R[("EXPDOMRAT", c)] = (
                v("QE", c) / v("QD", c)
                - (v("PE", c) / v("PDS", c) * cal.delta_ds[c] / cal.delta_e[c])
                ** (1 / (rho - 1)))
        elif ((QD00[c] > 0 and QE00[c] == 0)
              or (QD00[c] == 0 and QE00[c] > 0)
              or (c in CESEXOG and QE00[c])):
            R[("CET2", c)] = v("QX", c) - v("QD", c) - v("QE", c)
        if c in CESEXOG and QE00[c]:
            R[("ESUPPLYEXOG", c)] = v("QE", c) - clo.qeb(c, t)
        if QX00.get(c):
            R[("OUTVAL", c)] = (v("PX", c) * v("QX", c)
                                - v("PDS", c) * v("QD", c)
                                - v("PE", c) * v("QE", c))

    # import quota. EQ_QMCONST is a complementarity in GAMS -- `qmbar =G= QM`
    # paired with PRQMBAR (mod.gms 2608, 3533) -- so either the quota binds
    # (QM = qmbar, rent positive) or it is slack (QM < qmbar, rent zero). The
    # base year and reference path sit on the binding branch, which is why the
    # equality below is right there. A scenario that relaxes the quota puts it
    # on the slack branch, and forcing the equality would then drag imports up
    # to the relaxed ceiling instead of letting them settle; `cal.quota_slack`
    # marks those (c, t), where the row is dropped and PRQMBAR pinned at zero
    # by the closure instead. Check QM < qmbar afterwards to confirm the branch.
    # With the fuel channels, the fuel ceiling is BRB's foreign-exchange
    # allocation over the world price from `fx_from` on (fuel.py, channel 4).
    quota_slack = getattr(cal, "quota_slack", ())
    fxq = {c: fuel_mod.fx_share_at(cal, L, t, c) for c in CMBAR
           if fcfg and fuel_mod.fx_rule_on(cal, t, c)}
    for c in C:
        if c in CMBAR:
            if (c, t) not in quota_slack:
                if c in fxq:
                    R[("QMCONST", c)] = (
                        getattr(cal, "qmbar_scale", {}).get((c, t), 1.0) * fxq[c]
                        * fuel_mod.official_fx(cal, v, t) / v("PWM", c) - v("QM", c))
                else:
                    R[("QMCONST", c)] = cal.qmbar0[(c, t)] - v("QM", c)
            R[("IMPQUOTARENT", c)] = (
                v("YPRQMBART", c)
                - v("PRQMBAR", c) * v("PWM", c) * v("EXR", t) * v("QM", c))
    for c in C:
        # Quota rent exists only for quota commodities (YPRQMBART(c)=0 for the
        # rest). Generating the allocation equations for non-quota commodities
        # yields trivial 0=0 rows that make the Jacobian rank-deficient (a
        # near-null QG<->QT direction, identified by the null-vector
        # diagnostic). Restrict to cmbar so YPRQMBAR(c,.) for non-quota goods
        # is fixed at 0 by the partition instead.
        den0 = QQ00[c] - cal.QT00.get(c, 0.0)   # calibrated structure guard
        if c not in CMBAR or not den0:
            continue
        den = v("QQ", c) - v("QT", c)
        for h in H:
            R[("HHDIMPQUOTARENT", c, h)] = (
                v("YPRQMBAR", c, h)
                - v("QH", c, h) / den * v("YPRQMBART", c))
        for a in A:
            R[("ACTIMPQUOTARENT", c, a)] = (
                v("YPRQMBAR", c, a)
                - v("QINT", c, a) / den * v("YPRQMBART", c))
        R[("GOVIMPQUOTARENT", c)] = (
            sum(v("YPRQMBAR", c, g) for g in INSGOV)
            - v("QG", c) / den * v("YPRQMBART", c))
        R[("INVIMPQUOTARENT", c)] = (
            v("YPRQMBAR", c, "ngovz")
            - (v("QINV", c) + sum(clo.qdstk(c, i2, t) for i2 in IN2))
            / den * v("YPRQMBART", c))

    # Premium pass-through to IMPORT costs (port-only; not in GAMS). The
    # premium importers pay is `anchor + theta * (PREXR - anchor)`: theta = 1
    # (default, `cal.prexr_pt` unset) is mod.gms exactly; theta < 1 passes
    # only part of a CHANGE in the premium, measured from `anchor(t)`, into
    # import prices -- and, to keep the accounts consistent, into the premium
    # rent and the tariff base, which are valued at the same premium. The
    # anchor is the reference path for a scenario and the 2019 level for a
    # backcast, so the base year and the reference are untouched. Exports
    # keep the full premium (most receipts are surrendered at the official
    # rate). See runs/uni_passthrough.py.
    pt = getattr(cal, "prexr_pt", None)
    if pt is None:
        prexr_m = v("PREXR", t)
    else:
        anc = cal.prexr_pt_anchor[t]
        prexr_m = anc + pt * (v("PREXR", t) - anc)

    # informal fuel: border cost in BIF and the rent on it (premium + rising-cost markup)
    if INF_ON:
        inf_cost = (v("PWM", FUEL) * (1 + fcfg.mu) * v("EXR", t) * v("PREXR", t)
                    * (1 + v("QMI", FUEL) / QM00[FUEL]) ** (1.0 / fcfg.eta))
        # paid abroad: world price plus transport through neighbouring
        # countries; everything above that (parallel premium, smugglers'
        # margin, rising cost) is domestic rent
        inf_rent = (inf_cost - v("PWM", FUEL) * (1 + fcfg.mu_abroad) * v("EXR", t)) * v("QMI", FUEL)

    # forex premium rent
    for c in C:
        R[("FOREXRENT", c)] = (
            (-inf_rent if (INF_ON and c == FUEL) else 0.0)
            + v("YPREXRT", c)
            - (1 - cal.shrom00[c]) * (prexr_m - 1) * v("EXR", t)
            * v("PWM", c) * v("QM", c)
            + (1 - cal.shroe00[c]) * (v("PREXR", t) - 1) * v("EXR", t)
            * v("PWE", c) * v("QE", c))
        for ac in ACNT:
            if cal.shryprexr00.get((c, ac)):
                R[("FOREXRENTALLOC", c, ac)] = (
                    v("YPREXR", c, ac)
                    - cal.shryprexr00[(c, ac)] * v("YPREXRT", c))

    # world prices
    for c in C:
        if QM00[c]:
            R[("PMDEF", c)] = (
                v("PM", c)
                - (1 + v("TM", c) + v("PRQMBAR", c)) * v("PWM", c)
                * ((1 - cal.shrom00[c]) * v("EXR", t) * prexr_m
                   + cal.shrom00[c] * v("EXR", t))
                - sum(v("PQD", ct, tm) * icm(ct, c)
                      for ct in CT for tm in TACM))
        if QE00[c]:
            R[("PEDEF", c)] = (
                v("PE", c)
                - (1 - v("TE", c)) * v("PWE", c)
                * ((1 - cal.shroe00[c]) * v("EXR", t) * v("PREXR", t)
                   + cal.shroe00[c] * v("EXR", t))
                + sum(v("PQD", ct, te) * ice(ct, c)
                      for ct in CT for te in TACE))
        if c in CED and QE00[c]:
            R[("EDEMAND", c)] = (
                v("QE", c) - clo.qeb(c, t)
                * (v("PWE", c) / cal.pwse00.get(c, 1.0)) ** cal.eta_e[c])

    # informal fuel arbitrage (active branch only; complementarity.sweep_solve)
    if INF_ON and (FUEL, t) in getattr(cal, "inf_active", set()):
        R[("INFARB", FUEL)] = (
            v("PM", FUEL) - inf_cost
            - sum(v("PQD", ct, tm) * icm(ct, FUEL)
                  for ct in CT for tm in TACM))

    # trade/transport demand
    for c in C:
        if cal.QT00.get(c):
            R[("QTDEM", c)] = (
                v("QT", c)
                - sum(icm(c, cp) * v("QM", cp) + ice(c, cp) * v("QE", cp) for cp in C)
                - (icm(c, FUEL) * v("QMI", FUEL) if INF_ON else 0.0)
                - sum(icd(c, cp) * v("QD", cp) for cp in C))

    # ==== INSTITUTIONS ======================================================
    for f in F:
        for insd in INSD:
            if SHIF00.get((insd, f)):
                den = sum(v("QFINS", insdp, f) for insdp in INSD
                          if SHIF00.get((insdp, f)))
                R[("SHIFDEF", insd, f)] = (
                    v("SHIF", insd, f)
                    - v("QFINS", insd, f) / den
                    * sum(SHIF00.get((insdp, f), 0.0) for insdp in INSD))
    for f in F:
        for ins in INS:
            if YIF00.get((ins, f)):
                R[("YIFDEF", ins, f)] = (
                    v("YIF", ins, f)
                    - v("SHIF", ins, f) * v("YF", f) * (1 - v("TF", f)))
    for i in INSDNG:
        if YI00.get(i):
            R[("YIDEF", i)] = (
                v("YI", i)
                - sum(v("YIF", i, f) for f in F)
                - sum(v("TRNSFR", i, g) * v("CPI", t) for g in INSGOV)
                - sum(v("TRNSFR", i, r) * v("EXR", t) for r in INSROW)
                - sum(v("TRII", i, ip) for ip in INSDNG)
                - sum(v("YPREXR", c, i) for c in C)
                - sum(v("YPRQMBAR", c, i) for c in C))
    for i in INSDNG:
        if MPS00.get(i):
            R[("MPSDEF", i)] = (
                v("MPS", i)
                - cal.MPS00[i] * v("MPSSCAL", t) - v("MPSADJ", t))
        if SAV00.get(i):
            R[("INSSAVDEF", i)] = (
                v("SAV", i)
                - cal.alpha_sav00[i] * v("CPI", t)
                - v("MPS", i) * (1 - v("TY", i)) * v("YI", i))
    for i in INSDNG:
        for ins in INS:
            if TRII00.get((ins, i)):
                R[("TRIIDEF", ins, i)] = (
                    v("TRII", ins, i)
                    - cal.shii0[(ins, i, t)]
                    * (v("YI", i) * (1 - v("TY", i)) - v("SAV", i)))
    for h in H:
        R[("EHDEF", h)] = (
            v("EH", h) - (1 - v("TY", h)) * v("YI", h) + v("SAV", h)
            + sum(v("TRII", ins, h) for ins in INS))
    def les_exp(c, h):
        """LES expenditure on c by household h."""
        gam = cal.gamma00[(c, h)] * cal.pop[(h, t)]
        return (v("PQD", c, h) * gam
                + cal.beta[(c, h)]
                * (v("EH", h)
                   - sum(v("PQD", cp, h) * cal.gamma00[(cp, h)]
                         * cal.pop[(h, t)] for cp in C)))

    for h in H:
        hh_sub = HH_ON and QH00.get((FUEL, h)) and QH00.get((fcfg.bio, h))
        for c in C:
            if QH00.get((c, h)):
                exp_c = les_exp(c, h)
                if hh_sub and c == FUEL:
                    exp_c = v("HXI", h) * les_exp(FUEL, h)
                elif hh_sub and c == fcfg.bio:
                    exp_c = exp_c + (1 - v("HXI", h)) * les_exp(FUEL, h)
                R[("HHDDEM", c, h)] = v("PQD", c, h) * v("QH", c, h) - exp_c
        if hh_sub:
            # fuel-to-firewood/charcoal switching (gemcore/fuel.py, option 5)
            rel = ((v("PQD", FUEL, h) / PQD00[(FUEL, h)])
                   / (v("PQD", fcfg.bio, h) / PQD00[(fcfg.bio, h)]))
            R[("HHFUELSUB", h)] = v("HXI", h) - rel ** (-fcfg.eps_hh)

    # NGOs / tourists
    for n in INSNGO:
        if cal.QNGOSCAL00.get(n):
            R[("ENGODEF", n)] = (
                sum(v("PQD", c, n) * v("QNGO", c, n) for c in C)
                - (1 - v("TY", n)) * v("YI", n) + v("SAV", n)
                + sum(v("TRII", ins, n) for ins in INS))
        for c in C:
            if QNGO00.get((c, n)):
                R[("NGODEM", c, n)] = (v("QNGO", c, n)
                                       - cal.QNGO00[(c, n)] * v("QNGOSCAL", n))
    for s in INSTRST:
        for c in C:
            if QTRST00.get((c, s)):
                R[("TRSTDEM", c, s)] = (v("QTRST", c, s)
                                        - cal.QTRST00[(c, s)] * v("QTRSTSCAL", t))
    if cal.TRSMREC00:
        R[("TRSMRECDEF",)] = (
            v("TRSMREC", t) * v("EXR", t)
            - sum(v("PQD", c, s) * v("QTRST", c, s)
                  for c in C for s in INSTRST))

    # ==== GOVERNMENT ========================================================
    TAXCOM = S["taxcom"]
    R[("GOVREV",)] = (
        v("YG", t)
        - sum(v("TY", i) * v("YI", i) for i in INSDNG)
        - sum(v("TF", f) * v("YF", f) for f in F if f in FSAM and f in FVA)
        - sum(v("TQ", c) * v("PQS", c) * v("QQ", c) for c in C)
        - v("YTAXVAT", t) - v("YTAXIMP", t) - v("YTAXEXP", t)
        - sum(v("TFA", f, a) * v("WF", f) * v("WFDIST", f, a) * v("QF", f, a)
              for f in F for a in A if f in FSAM and f in FVA)
        - sum(v("TA", a) * v("PA", a) * v("QA", a) for a in A)
        - v("EXR", t) * sum(v("TRNSFR", g, r) for g in INSGOV for r in INSROW)
        - sum(v("TRII", g, i) for g in INSGOV for i in INSDNG)
        - sum(v("YIF", g, f) for g in INSGOV for f in F)
        - sum(v("YPRQMBAR", c, g) for c in C for g in INSGOV)
        - sum(v("YPREXR", c, g) for c in C for g in INSGOV))

    # tax-rate scaling defs (base bars from calibration, scaled)
    for ins in INS:
        R[("TYDEF", ins)] = (v("TY", ins)
                             - clo.tyb(ins, t) * (1 + clo.ty01(ins, t)
                                                  * v("TYSCAL", t)))
    for f in F:
        R[("TFDEF", f)] = (v("TF", f)
                           - clo.tfb(f, t) * (1 + clo.tf01(f, t)
                                              * v("TFSCAL", t)))
    for a in A:
        R[("TADEF", a)] = (v("TA", a)
                           - clo.tab(a, t) * (1 + clo.ta01(a) * v("TASCAL", t)))
    for c in C:
        R[("TQDEF", c)] = v("TQ", c) - clo.tqb(c, t) * v("TQSCAL", t)
        R[("TEDEF", c)] = v("TE", c) - clo.teb(c, t) * v("TESCAL", t)
        R[("TMDEF", c)] = v("TM", c) - clo.tmb(c, t) * v("TMSCAL", t)
    for (c, d) in clo.tvacb_keys:
        R[("TVACDEF", c, d)] = (v("TVAC", c, d)
                                - clo.tvacb(c, d, t) * v("TVACSCAL", t))
    for f in F:
        for a in A:
            if f in FSAM and f in FVA:
                R[("TFADEF", f, a)] = (v("TFA", f, a)
                                       - clo.tfab(f, a, t) * v("TFASCAL", t))

    for a in A:
        R[("VATREBATEDEF", a)] = (
            v("RBTVAT", a)
            - sum(cal.shrbtvat00.get((c, a), 1.0) * v("TVAC", c, a)
                  * v("PQS", c) * (1 - v("SUBC", c, a)) * (1 + v("TQ", c))
                  * v("QINT", c, a) for c in C))
    for (c, d) in clo.subcb_keys:
        R[("SUBCDEF", c, d)] = (v("SUBC", c, d)
                                - clo.subcb(c, d, t) * v("SUBCSCAL", t))

    R[("YTAXEXPDEF",)] = (
        v("YTAXEXP", t)
        - sum(v("TE", c) * v("PWE", c) * v("QE", c)
              * ((1 - cal.shroe00[c]) * v("EXR", t) * v("PREXR", t)
                 + cal.shroe00[c] * v("EXR", t)) for c in C))
    R[("YTARIMPDEF",)] = (
        v("YTAXIMP", t)
        - sum(v("TM", c) * v("PWM", c) * v("QM", c)
              * ((1 - cal.shrom00[c]) * v("EXR", t) * prexr_m
                 + cal.shrom00[c] * v("EXR", t)) for c in C))
    # VAT revenue (zero for bdi2019 -- no taxvatc) kept general
    R[("YTAXVATDEF",)] = v("YTAXVAT", t) - _vat_revenue(v, cal, C, A, H,
                                                        INSNGO, INSTRST,
                                                        INSGOV, S, IN2)

    R[("GOVEXP",)] = (
        v("EG", t)
        - sum(v("PQD", c, g) * v("QG", c) for c in C for g in INSGOV)
        - sum(v("TRNSFR", i, g) * v("CPI", t) for i in INSDNG for g in INSGOV)
        - sum(v("TRNSFR", r, g) * v("EXR", t) for r in INSROW for g in INSGOV)
        - v("SUBCT", t)
        - sum(v("RBTVAT", a) for a in A))
    for c in C:
        if QG00.get(c):
            R[("GOVDEM", c)] = (v("QG", c)
                                - clo.qgb(c, t) * (1 + clo.qgc01(c, t)
                                                  * v("QGSCAL", t)))
    R[("SUBCTDEF",)] = v("SUBCT", t) - _subc_total(v, cal, C, A, H, INSNGO,
                                                   INSTRST, INSGOV, S, IN2)

    # transfers following rules
    for h in H:
        for r in INSROW:
            if TRNSFR00.get((h, r)):
                R[("TRHROWDEF", h, r)] = (
                    v("TRNSFR", h, r)
                    - clo.trnsfrpcb(h, r, t) * cal.pop[(h, t)]
                    * v("TRNSFRSCAL", "trngovrow"))
    for i in INSDNGNH:
        for r in INSROW:
            if TRNSFR00.get((i, r)):
                R[("TRINSDNHROWDEF", i, r)] = (
                    v("TRNSFR", i, r)
                    - clo.trnsfrb(i, r, t) * v("TRNSFRSCAL", "trngovrow"))
    for f in F:
        for r in INSROW:
            if TRNSFR00.get((f, r)):
                R[("TRFACROWDEF", f, r)] = (
                    v("TRNSFR", f, r)
                    - clo.trnsfrb(f, r, t) * v("TRNSFRSCAL", "trfacrow"))
    for h in H:
        for g in INSGOV:
            if TRNSFR00.get((h, g)):
                R[("TRHGOVDEF", h, g)] = (
                    v("TRNSFR", h, g)
                    - clo.trnsfrpcb(h, g, t) * cal.pop[(h, t)]
                    * v("TRNSFRSCAL", "trngovgov"))
    for i in INSDNGNH:
        for g in INSGOV:
            if TRNSFR00.get((i, g)):
                R[("TRINSDNHGOVDEF", i, g)] = (
                    v("TRNSFR", i, g)
                    - clo.trnsfrb(i, g, t) * v("TRNSFRSCAL", "trngovgov"))
    for r in INSROW:
        for g in INSGOV:
            if TRNSFR00.get((r, g)):
                R[("TRROWGOVDEF", r, g)] = (
                    v("TRNSFR", r, g)
                    - clo.trnsfrb(r, g, t) * v("TRNSFRSCAL", "trrowgov"))
    for g in INSGOV:
        for r in INSROW:
            if TRNSFR00.get((g, r)):
                R[("TRGOVROWDEF", g, r)] = (
                    v("TRNSFR", g, r)
                    - clo.trnsfrb(g, r, t) * v("TRNSFRSCAL", "trgovrow"))

    # ==== INVESTMENT / SYSTEM ==============================================
    R[("GOVPRIMDEF",)] = v("GPRIMDEF", t) - v("EG", t) - v("INVVALG", t) + v("YG", t)
    R[("GOVPRIMDEFREALDEF",)] = v("RGPRIMDEF", t) * v("CPI", t) - v("GPRIMDEF", t)
    R[("GOVINVCOST",)] = (
        v("INVVALG", t)
        - sum(v("PK", fc) * v("DKINS", "govz", fc) for fc in FCAP)
        - sum(v("PQD", c, dk) * clo.qdstk(c, "govz", t)
              for c in C for dk in S["dstk"]))
    R[("GOVCAPACC",)] = v("GPRIMDEF", t) - v("EXR", t) * v("NFFG", t) - v("NDFG", t)
    R[("GOVNETDOMFINREAL",)] = v("RNDFG", t) - v("NDFG", t) / v("CPI", t)
    if cal.INVVAL00:
        R[("NGOVINVFIN",)] = (
            v("INVVAL", t)
            - sum(v("SAV", i) for i in INSDNG)
            - v("NFFINS", t) * v("EXR", t)
            + v("NDFG", t) + cal.drf.get(t, 0.0) * v("EXR", t)
            - sum(v("YPREXR", c, "ngovz") for c in C)
            - sum(v("YPRQMBAR", c, "ngovz") for c in C))
        R[("NGOVINVCOST",)] = (
            v("INVVAL", t)
            - sum(v("PK", fc) * v("DKINS", "ngovz", fc) for fc in FCAP)
            - sum(v("PQD", c, dk) * clo.qdstk(c, "ngovz", t)
                  for c in C for dk in S["dstk"]))
    R[("NGOVNETFORFIN",)] = v("NFFINS", t) - clo.nffinsbar(t) * v("NFFINSSCAL", t)

    for fc in FCAP:
        if cal.DKINS00.get(("govz", fc)):
            R[("DKGOVDEF", "govz", fc)] = (
                v("DKINS", "govz", fc)
                - clo.dkinsb("govz", fc, t) * v("ISCAL", fc)
                * (1 + v("IADJ", "govz") * cal.iadj010[fc])
                - clo.ddkins("govz", fc, t))
        if cal.DKINS00.get(("ngovz", fc)):
            R[("DKNGOVDEF", "ngovz", fc)] = (
                v("DKINS", "ngovz", fc)
                - clo.dkinsb("ngovz", fc, t) * v("ISCAL", fc)
                * (1 + v("IADJ", "ngovz") * cal.iadj010[fc])
                - clo.ddkins("ngovz", fc, t))
    for fc in FCAPNG:
        if cal.DKINS00.get(("rowz", fc)):
            R[("DKROWDEF", "rowz", fc)] = (
                v("DKINS", "rowz", fc)
                - cal.invshr00.get((fc, "rowz"), 0.0) * v("INVVALF", t)
                * v("EXR", t) / v("PK", fc))
    if cal.INVVALF00:
        # + a scenario's targeted FDI (e.g. mining), foreign currency
        R[("INVVALFDEF",)] = (v("INVVALF", t) - clo.invvalfb(t) * v("FDISCAL", t)
                              - getattr(cal, "fdi_add", {}).get(t, 0.0))
    for fc in FCAP:
        R[("PCAPDEF", fc)] = (v("PK", fc)
                              - sum(capcomp.get((fc, c), 0.0) * v("PQD", c, fc)
                                    for c in C))
    for c in C:
        if QINV00.get(c):
            R[("INVDEM", c)] = (
                v("QINV", c)
                - sum(capcomp.get((fc, c), 0.0) * v("DKINS", i2, fc)
                      for i2 in IN2 for fc in FCAP))

    # capital accumulation (dynamic; identities at tmin handled by fixing)
    if not is_tmin:
        for insd in INSDNH:
            for fc in FCAPNG:
                if QFINS00.get((insd, fc)):
                    R[("CAPACCUMNGOVDOM", insd, fc)] = (
                        v("QFINS", insd, fc)
                        - lag("QFINS", insd, fc) * (1 - deprcap[fc])
                        - lag("SHIF", insd, fc)
                        * (lag("DKINS", "ngovz", fc) + lag("DKINS", "govz", fc)))
        for h in H:
            for fc in FCAPNG:
                if QFINS00.get((h, fc)):
                    R[("CAPACCUMNGOVHHD", h, fc)] = (
                        v("QFHEND", h, fc)
                        - lag("QFINS", h, fc) * (1 - deprcap[fc])
                        - lag("SHIF", h, fc)
                        * (lag("DKINS", "ngovz", fc) + lag("DKINS", "govz", fc)))
                    R[("CAPREDIST", h, fc)] = (
                        v("QFINS", h, fc)
                        - v("QFHEND", h, fc) * cal.pop[(h, t)]
                        / cal.pop[(h, cal.TSOL[cal.TSOL.index(t) - 1])]
                        * v("QFHENDSCAL", fc))
            # constraint per fcapng
        for fc in FCAPNG:
            if sum(QFINS00.get((h, fc), 0) for h in H):
                R[("CAPREDISTCONST", fc)] = (
                    sum(v("QFINS", h, fc) for h in H)
                    - sum(v("QFHEND", h, fc) for h in H))
        for r in INSROW:
            for fc in FCAPNG:
                if QFINS00.get((r, fc)):
                    R[("CAPACCUMNGOVFOR", r, fc)] = (
                        v("QFINS", r, fc)
                        - lag("QFINS", r, fc) * (1 - deprcap[fc])
                        - lag("DKINS", "rowz", fc))
        for g in INSGOV:
            for fc in FCAPG:
                if QFINS00.get((g, fc)):
                    R[("CAPACCUMGOV", g, fc)] = (
                        v("QFINS", g, fc)
                        - lag("QFINS", g, fc) * (1 - deprcap[fc])
                        - sum(lag("DKINS", i2, fc) for i2 in IN2))
        for fc in FCAPNG:
            for a in A:
                if QF00.get((fc, a)):
                    R[("CAPACCUMACT", fc, a)] = (
                        v("QF", fc, a)
                        - lag("QF", fc, a) * (1 - deprcap[fc])
                        - lag("DKA", fc, a))

    # endowment defs
    for ins in INS:
        for f in F:
            if QFINS00.get((ins, f)) and f in FLAB:
                R[("LABENDOWDEF", ins, f)] = (
                    v("QFINS", ins, f)
                    - cal.QFINS0[(ins, f, t)] / cal.QFINS00[(ins, f)]
                    * cal.QFINS00[(ins, f)] * v("QFINSSCAL", f) * v("QLABSCAL", t)) \
                    if False else (
                    v("QFINS", ins, f)
                    - clo.qfinsb(ins, f, t) * v("QFINSSCAL", f) * v("QLABSCAL", t))
            elif QFINS00.get((ins, f)) and f in FNCAP and f not in set(FLAB):
                R[("OTHENDOWDEF", ins, f)] = (
                    v("QFINS", ins, f)
                    - clo.qfinsb(ins, f, t) * v("QFINSSCAL", f))
    for f in F:
        if cal.QFS00.get(f):
            R[("FACSUP", f)] = v("QFS", f) - sum(v("QFINS", ins, f) for ins in INS)

    R[("LABPARTRATDEF",)] = (
        v("LABPARTRAT", t)
        - sum(v("QFINS", ins, f) for ins in INS for f in FLAB)
        / cal.pop[("agelab", t)])

    for f in F:
        if f in FSAM and f in FVA:
            R[("WFAVGDEF", f)] = (
                v("WFAVG", f)
                - sum(v("WFA", f, a) * v("QF", f, a) for a in A)
                / sum(v("QF", f, a) for a in A))
    # new capital allocation (dynamic). EQ_NEWCAPALLOC spans every activity,
    # including one that used no private capital in the base year: there the
    # row still pins DKA to zero, and dropping it would leave that DKA cell
    # without a defining equation.
    # A scenario's targeted FDI (port-only, `cal.fdi_add` with
    # `cal.fdi_target`) is capital for one activity: it goes there, and only
    # the rest of new capital is allocated by the rule.
    fdi_t = getattr(cal, "fdi_add", {}).get(t, 0.0)
    fdi_a = getattr(cal, "fdi_target", None)
    for fc in FCAPNG:
        tgt = (cal.invshr00.get((fc, "rowz"), 0.0) * fdi_t * v("EXR", t) / v("PK", fc)
               if (fdi_t and fdi_a) else 0.0)
        for a in A:
            R[("NEWCAPALLOC", fc, a)] = (
                v("DKA", fc, a)
                - (sum(v("DKINS", i2, fc) for i2 in IN2) - tgt)
                * v("QF", fc, a) / sum(v("QF", fc, ap) for ap in A)
                * (1 + cal.kappa * (v("WFA", fc, a) / v("WFAVG", fc) - 1))
                - (tgt if a == fdi_a else 0.0))

    # ==== MARKET CLEARING / MACRO ==========================================
    for c in C:
        if QQ00.get(c):
            R[("COMEQ", c)] = (
                sum(v("QH", c, h) for h in H)
                + sum(v("QNGO", c, n) for n in INSNGO)
                + sum(v("QINT", c, a) for a in A)
                + v("QINV", c)
                + sum(clo.qdstk(c, i2, t) for i2 in IN2)
                + v("QG", c) + v("QT", c)
                + sum(v("QTRST", c, s) for s in INSTRST)
                - v("QQ", c))

    R[("CURACC",)] = (
        sum(v("PWE", c) * v("QE", c) for c in C)
        + sum(v("PQD", c, s) * v("QTRST", c, s)
              for c in C for s in INSTRST) / v("EXR", t)
        + sum(v("TRNSFR", i, r) for i in INSD for r in INSROW)
        + sum(v("TRNSFR", f, r) for f in F for r in INSROW)
        + v("SAVF", t)
        - sum(v("PWM", c) * v("QM", c) for c in C)
        - (v("PWM", FUEL) * (1 + fcfg.mu_abroad) * v("QMI", FUEL) if INF_ON else 0.0)
        - sum(v("TRNSFR", r, g) for r in INSROW for g in INSGOV)
        - sum(v("YIF", r, f) for r in INSROW for f in F) / v("EXR", t)
        - sum(v("TRII", r, i) for r in INSROW for i in INSDNG) / v("EXR", t))

    R[("CAPACC",)] = (
        v("SAVF", t) - v("NFFG", t) - v("NFFINS", t) - v("INVVALF", t)
        + cal.drf.get(t, 0.0) - v("WALRAS", t))

    R[("CPIDEF",)] = (sum(v("PQD", c, h) * cal.cwts[(c, h)]
                          for c in C for h in H) - v("CPI", t))
    R[("DPIDEF",)] = (sum(v("PDS", c) * cal.dwts[c] for c in C) - v("DPI", t))
    R[("REXRDEF",)] = v("REXR", t) - v("EXR", t) / v("DPI", t)

    R[("GDPREALFCDEF",)] = (
        v("RGDPFC", t)
        - sum(PVA00[a] * vq(a) for a in A)
        - sum(cal.WFA00.get((f, a), 0.0) * v("QF", f, a)
              for f in FLEO for a in A))
    R[("GDPMPDEF",)] = (
        v("GDPMP", t) - _gdpmp(v, cal, C, H, INSNGO, INSTRST, FCAP,
                               S["dstk"], INSGOV, IN2, clo, t))
    R[("GDPREALMPDEF",)] = (
        v("RGDPMP", t) - _rgdpmp(v, cal, C, H, INSNGO, INSTRST, FCAP,
                                 S["dstk"], INSGOV, IN2, clo, t))
    R[("GDPPCREALDEF",)] = (
        v("RGDPPC", t) * sum(cal.pop[(h, t)] for h in H) - v("RGDPMP", t))
    R[("TRDGDPDEF",)] = (
        v("TRDGDP", t) * v("RGDPMP", t)
        - sum(cal.EXR00 * PWE00.get(c, 0.0) * v("QE", c) for c in C)
        - sum(cal.EXR00 * PWM00.get(c, 0.0) * v("QM", c) for c in C)
        - (cal.EXR00 * PWM00[FUEL] * (1 + fcfg.mu_abroad) * v("QMI", FUEL) if INF_ON else 0.0))
    R[("ABSNOMDEF",)] = (
        v("ABSNOM", t) - _absnom(v, cal, C, H, INSNGO, FCAP, S["dstk"],
                                 INSGOV, IN2, clo, t))

    # gov receipt/spend/ngovpay GDP & ABS share defs
    _gov_share_eqs(R, v, cal, clo, t, S)

    # debt stocks
    R[("GOVDOMBOR",)] = (v("GBOR", t) - v("NDFG", t)
                         - clo.gintrat(t) * v("GDEBT", t))
    if not is_tmin:
        R[("GOVDOMDEBT",)] = v("GDEBT", t) - lag("GDEBT", t_prev(cal, t)) \
            - lag("GBOR", t_prev(cal, t))
    R[("GOVFORBOR", "govz")] = (v("FBOR", "govz") - v("NFFG", t)
                                - clo.fintrat("govz", t) * v("FDEBT", "govz"))
    R[("NGOVFORBOR", "ngovz")] = (v("FBOR", "ngovz") - v("NFFINS", t)
                                  - clo.fintrat("ngovz", t) * v("FDEBT", "ngovz"))
    if not is_tmin:
        for i2 in IN2:
            if i2 in ("govz", "ngovz"):
                R[("FORDEBT", i2)] = (v("FDEBT", i2)
                                      - lag("FDEBT", i2) - lag("FBOR", i2))

    return R


def rent_eff_prev(cal, L, t):
    """Effective rent share of the year before t, from its solution L:
    eff(tp) = (1 - lam) eff(tp - 1) + lam * share(tp), eff = the base-year share
    in the first year. Cached per year on `cal`, solved in order."""
    cache = cal.__dict__.setdefault("_rent_eff", {})
    tp = t_prev(cal, t)
    share = sum(L["YPREXRT"].values()) / L["GDPMP"][tp]
    i = cal.TSOL.index(tp)
    if i == 0:
        val = share
    else:
        lam = getattr(cal, "rent_adjust", 1.0)
        prev = cache.get(cal.TSOL[i - 1], cal.rent_share00)
        val = (1 - lam) * prev + lam * share
    cache[tp] = val
    return val


def t_prev(cal, t):
    i = cal.TSOL.index(t)
    return cal.TSOL[i - 1]


# ---- helper aggregators (shared by several equations) ---------------------

def _vat_revenue(v, cal, C, A, H, INSNGO, INSTRST, INSGOV, S, IN2):
    # zero for bdi2019 (TVAC calibrated to 0); general form
    tot = 0.0
    for c in C:
        for a in A:
            tot += ((1 - v("SUBC", c, a)) * v("PQS", c) * (1 + v("TQ", c))
                    * v("TVAC", c, a) * v("QINT", c, a))
        for h in H:
            tot += ((1 - v("SUBC", c, h)) * v("PQS", c) * (1 + v("TQ", c))
                    * v("TVAC", c, h) * v("QH", c, h))
        for g in INSGOV:
            tot += ((1 - v("SUBC", c, g)) * v("PQS", c) * (1 + v("TQ", c))
                    * v("TVAC", c, g) * v("QG", c))
    return tot


def _subc_total(v, cal, C, A, H, INSNGO, INSTRST, INSGOV, S, IN2):
    tot = 0.0
    for c in C:
        for a in A:
            tot += v("SUBC", c, a) * v("PQS", c) * (1 + v("TQ", c)) * v("QINT", c, a)
        for h in H:
            tot += v("SUBC", c, h) * v("PQS", c) * (1 + v("TQ", c)) * v("QH", c, h)
        for g in INSGOV:
            tot += v("SUBC", c, g) * v("PQS", c) * (1 + v("TQ", c)) * v("QG", c)
    return tot


def _gdpmp(v, cal, C, H, INSNGO, INSTRST, FCAP, DSTK, INSGOV, IN2, clo, t):
    return (
        sum(v("PQD", c, h) * v("QH", c, h) for c in C for h in H)
        + sum(v("PQD", c, n) * v("QNGO", c, n) for c in C for n in INSNGO)
        + sum(v("PQD", c, s) * v("QTRST", c, s) for c in C for s in INSTRST)
        + sum(v("PQD", c, fc) * cal.capcomp.get((fc, c), 0.0)
              * sum(v("DKINS", i2, fc) for i2 in IN2) for c in C for fc in FCAP)
        + sum(v("PQD", c, dk) * sum(clo.qdstk(c, i2, t) for i2 in IN2)
              for c in C for dk in DSTK)
        + sum(v("PQD", c, g) * v("QG", c) for c in C for g in INSGOV)
        + sum(v("EXR", cal._solve_t) * v("PWE", c) * v("QE", c) for c in C)
        - sum(v("EXR", cal._solve_t) * v("PWM", c) * v("QM", c) for c in C)
        - _informal_imports(v, cal, current=True))


def _informal_imports(v, cal, current):
    """Informal fuel imports valued at the official rate (GDP deduction):
    PWM (1 + mu) QMI, at current or base-year prices. Zero when off."""
    fcfg = getattr(cal, "fuel", None)
    if not (fcfg and fcfg.informal):
        return 0.0
    f = fcfg.fuel
    if current:
        return v("EXR", cal._solve_t) * v("PWM", f) * (1 + fcfg.mu_abroad) * v("QMI", f)
    return cal.EXR00 * cal.PWM00[f] * (1 + fcfg.mu_abroad) * v("QMI", f)


def _rgdpmp(v, cal, C, H, INSNGO, INSTRST, FCAP, DSTK, INSGOV, IN2, clo, t):
    return (
        sum(cal.PQD00.get((c, h), 0.0) * v("QH", c, h) for c in C for h in H)
        + sum(cal.PQD00.get((c, n), 0.0) * v("QNGO", c, n)
              for c in C for n in INSNGO)
        + sum(cal.PQD00.get((c, s), 0.0) * v("QTRST", c, s)
              for c in C for s in INSTRST)
        + sum(cal.PQD00.get((c, fc), 0.0) * cal.capcomp.get((fc, c), 0.0)
              * sum(v("DKINS", i2, fc) for i2 in IN2) for c in C for fc in FCAP)
        + sum(cal.PQD00.get((c, dk), 0.0)
              * sum(clo.qdstk(c, i2, t) for i2 in IN2)
              for c in C for dk in DSTK)
        + sum(cal.PQD00.get((c, g), 0.0) * v("QG", c) for c in C for g in INSGOV)
        + sum(cal.EXR00 * cal.PWE00.get(c, 0.0) * v("QE", c) for c in C)
        - sum(cal.EXR00 * cal.PWM00.get(c, 0.0) * v("QM", c) for c in C)
        - _informal_imports(v, cal, current=False))


def _absnom(v, cal, C, H, INSNGO, FCAP, DSTK, INSGOV, IN2, clo, t):
    return (
        sum(v("PQD", c, h) * v("QH", c, h) for c in C for h in H)
        + sum(v("PQD", c, n) * v("QNGO", c, n) for c in C for n in INSNGO)
        + sum(v("PQD", c, fc) * cal.capcomp.get((fc, c), 0.0)
              * sum(v("DKINS", i2, fc) for i2 in IN2) for c in C for fc in FCAP)
        + sum(v("PQD", c, dk) * sum(clo.qdstk(c, i2, t) for i2 in IN2)
              for c in C for dk in DSTK)
        + sum(v("PQD", c, g) * v("QG", c) for c in C for g in INSGOV))


def _gov_share_eqs(R, v, cal, clo, t, S):
    ACGOVREC, ACGOVSPND, ACNGOVPAY = (S["acgovrec"], S["acgovspnd"],
                                      S["acngovpay"])
    A, C, H, F = S["a"], S["c"], S["h"], S["f"]
    INSDNG, INSGOV, INSROW = S["insdng"], S["insgov"], S["insrow"]
    FCAPG, FCAPNG, IN2 = S["fcapg"], S["fcapng"], S["ins2"]
    FSAM, FVA = set(S["fsam"]), set(S["fva"])
    taxcom, taxact, taxdir = set(S["taxcom"]), set(S["taxact"]), set(S["taxdir"])
    taximp, taxexp, taxvatc = set(S["taximp"]), set(S["taxexp"]), set(S["taxvatc"])
    taxfac = set(S["taxfac"])

    def gdpmp():
        return v("GDPMP", t)

    for ac in ACGOVREC:
        if not cal.GOVRECGDP00.get(ac):
            continue
        rhs = 0.0
        if ac in taxcom:
            rhs += sum(v("TQ", c) * v("PQS", c) * v("QQ", c) for c in C)
        if ac in taxact:
            rhs += sum(v("TA", a) * v("PA", a) * v("QA", a) for a in A)
        if ac in taxdir:
            rhs += sum(v("TY", i) * v("YI", i) for i in INSDNG)
        if ac in taximp:
            rhs += v("YTAXIMP", t)
        if ac in taxexp:
            rhs += v("YTAXEXP", t)
        if ac in taxvatc:
            rhs += v("YTAXVAT", t)
        if ac in taxfac:
            rhs += sum(v("TF", f) * v("YF", f) for f in F)
        if ac == "trgovngov":
            rhs += sum(v("TRII", g, i) for g in INSGOV for i in INSDNG)
        if ac == "trgovrow":
            rhs += v("EXR", t) * sum(v("TRNSFR", g, r)
                                     for g in INSGOV for r in INSROW)
        if ac == "netdomfin":
            rhs += v("NDFG", t)
        if ac == "netforfingov":
            rhs += v("EXR", t) * v("NFFG", t)
        R[("GOVRECGDPDEF", ac)] = v("GOVRECGDP", ac) * gdpmp() - rhs

    for ac in ACGOVSPND:
        rhs = 0.0
        if ac == "trngovgov":
            rhs += sum(v("TRNSFR", i, g) * v("CPI", t)
                       for i in INSDNG for g in INSGOV)
        if ac == "trrowgov":
            rhs += sum(v("EXR", t) * v("TRNSFR", r, g)
                       for r in INSROW for g in INSGOV)
        if ac == "congov":
            rhs += sum(v("PQD", c, g) * v("QG", c) for c in C for g in INSGOV)
        if ac == "subcom" or ac in set(S["subcom"]):
            rhs += v("SUBCT", t)
        if ac in set(FCAPG):
            rhs += sum(v("DKINS", i2, ac) for i2 in IN2) * v("PK", ac)
        R[("GOVSPNDGDPDEF", ac)] = v("GOVSPNDGDP", ac) * gdpmp() - rhs

    for ac in ACNGOVPAY:
        rhs = 0.0
        if ac == "trngovrow":
            rhs += sum(v("TRNSFR", i, r) * v("EXR", t)
                       for i in INSDNG for r in INSROW)
        if ac == "trrowngov":
            rhs += sum(v("TRII", r, i) for r in INSROW for i in INSDNG)
        if ac == "trfacrow":
            rhs += sum(v("TRNSFR", f, r) * v("EXR", t)
                       for f in F for r in INSROW)
        if ac == "trrowfac":
            rhs += sum(v("YIF", r, f) for r in INSROW for f in F)
        if ac == "savngov":
            rhs += sum(v("SAV", i) for i in INSDNG)
        if ac == "netforfinngov":
            rhs += v("EXR", t) * v("NFFINS", t)
        if ac in set(FCAPNG):
            rhs += v("DKINS", "ngovz", ac) * v("PK", ac)
        if ac == "fdi":
            rhs += sum(v("DKINS", "rowz", fc) * v("PK", fc) for fc in FCAPNG)
        if ac == "tourismrec":
            rhs += v("TRSMREC", t) * v("EXR", t)
        R[("NGOVPAYGDPDEF", ac)] = v("NGOVPAYGDP", ac) * gdpmp() - rhs

    for ac in ACGOVREC:
        if cal.GOVRECABS00.get(ac):
            R[("GOVRECABSDEF", ac)] = (v("GOVRECABS", ac)
                                       - v("GOVRECGDP", ac) / gdpmp()
                                       * v("ABSNOM", t))
    for ac in ACGOVSPND:
        R[("GOVSPNDABSDEF", ac)] = (v("GOVSPNDABS", ac)
                                    - v("GOVSPNDGDP", ac) / gdpmp()
                                    * v("ABSNOM", t))
    for ac in ACNGOVPAY:
        R[("NGOVPAYABSDEF", ac)] = (v("NGOVPAYABS", ac)
                                    - v("NGOVPAYGDP", ac) / gdpmp()
                                    * v("ABSNOM", t))
