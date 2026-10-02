"""Recursive-dynamic driver: solve the base run 2019-2040 period by period.

Replicates sim.gms SOLUCION 1 (pass 1, dcal01=1: flex TFPSCAL, fix RGDPFC ->
calibrate TFP so GDP follows the target growth path) marching through tsol,
with between-period capital/debt accumulation carried by the model's own
CAPACCUM* / FORDEBT / GOVDOMDEBT equations (which read t-1 levels via the
lag state L).

For t = tmin, capital stocks and debt are fixed at base levels (closure); for
t > tmin they are endogenous, determined by accumulation from the previous
period's solution.
"""

from __future__ import annotations

import time

from .state import build_state, Closure
from .solver import (solve_period_newton, solve_period_lsq, closure_fixed,
                     zero_fixed)
from .partition import build_partition

# quantity-like variable families (scale with GDP growth when warm-starting)
_QTY_FAMILIES = {
    "QA", "QX", "QD", "QE", "QM", "QQ", "QT", "QINT", "QF", "QG", "QH",
    "QINV", "QNGO", "QTRST", "QXAC", "QFS", "QFINS", "QFHEND", "YF", "YI",
    "YG", "EG", "EH", "GDPMP", "RGDPFC", "RGDPMP", "ABSNOM", "SAV", "SAVF",
    "TRII", "TRNSFR", "YIF", "INVVAL", "INVVALG", "INVVALF", "NDFG", "NFFG",
    "NFFINS", "GPRIMDEF", "RGPRIMDEF", "RNDFG", "GDEBT", "FDEBT", "GBOR",
    "FBOR", "DKINS", "DKA", "SUBCT", "YTAXIMP", "YTAXEXP", "YTAXVAT",
    "TRSMREC", "YPREXR", "YPREXRT", "YPRQMBAR", "YPRQMBART",
}


def predetermined_stocks(cal, t, tprev, L):
    """Compute the predetermined capital & debt stocks at period t from the
    previous period's solution L, exactly as the GAMS accumulation equations
    (CAPACCUM*, GOVDOMDEBT, FORDEBT) define them. Returns {(name, idx): value}.
    These are fixed each period so the stiff identities leave the simultaneous
    equilibrium system (recursive dynamics = predetermined stocks)."""
    S = cal.db.sets
    A, H = S["a"], S["h"]
    FCAPNG, FCAPG = S["fcapng"], S["fcapg"]
    INSDNH, INSROW, INSGOV, IN2 = (S["insdnh"], S["insrow"], S["insgov"],
                                   S["ins2"])
    dep = cal.deprcap

    def lg(nm, *idx):
        return L.get(nm, {}).get(idx if len(idx) != 1 else idx[0], 0.0)

    pre = {}
    # sectoral non-gov capital: QF(fcapng,a)
    for fc in FCAPNG:
        for a in A:
            if cal.QF00.get((fc, a)):
                pre[("QF", (fc, a))] = (lg("QF", fc, a) * (1 - dep[fc])
                                        + lg("DKA", fc, a))
    # institutional non-gov capital endowments (non-household)
    for ins in INSDNH:
        for fc in FCAPNG:
            if cal.QFINS00.get((ins, fc)):
                pre[("QFINS", (ins, fc))] = (
                    lg("QFINS", ins, fc) * (1 - dep[fc])
                    + lg("SHIF", ins, fc)
                    * (lg("DKINS", "ngovz", fc) + lg("DKINS", "govz", fc)))
    # RoW-owned non-gov capital
    for r in INSROW:
        for fc in FCAPNG:
            if cal.QFINS00.get((r, fc)):
                pre[("QFINS", (r, fc))] = (lg("QFINS", r, fc) * (1 - dep[fc])
                                           + lg("DKINS", "rowz", fc))
    # government capital
    for g in INSGOV:
        for fc in FCAPG:
            if cal.QFINS00.get((g, fc)):
                pre[("QFINS", (g, fc))] = (
                    lg("QFINS", g, fc) * (1 - dep[fc])
                    + sum(lg("DKINS", i2, fc) for i2 in IN2))
    # household end-of-period real endowment (pre-redistribution) and the
    # redistribution itself. QFHEND is predetermined; the redistribution
    # (CAPREDIST + CAPREDISTCONST) only redistributes that predetermined stock
    # across households by population growth, conserving the total -- it does
    # NOT depend on the within-period equilibrium, so QFHENDSCAL and the
    # post-redistribution QFINS(h,fcapng) are predetermined too. Computing
    # them here (rather than leaving them in the simultaneous system) removes
    # the tightly-coupled near-singular block that stalled the solver.
    popr = {h: cal.pop[(h, t)] / cal.pop[(h, tprev)] for h in H}
    for fc in FCAPNG:
        qfhend = {}
        for h in H:
            if cal.QFINS00.get((h, fc)):
                qfhend[h] = (lg("QFINS", h, fc) * (1 - dep[fc])
                             + lg("SHIF", h, fc)
                             * (lg("DKINS", "ngovz", fc)
                                + lg("DKINS", "govz", fc)))
                pre[("QFHEND", (h, fc))] = qfhend[h]
        if not qfhend:
            continue
        # QFHENDSCAL conserves the total stock across the redistribution
        denom = sum(qfhend[h] * popr[h] for h in qfhend)
        scal = (sum(qfhend.values()) / denom) if denom else 1.0
        pre[("QFHENDSCAL", fc)] = scal
        for h in qfhend:
            pre[("QFINS", (h, fc))] = qfhend[h] * popr[h] * scal
    # QFHENDSCAL exists for every factor in the state, but CAPREDISTCONST
    # only determines it where households own the (fcapng) stock; the rest
    # have no equation at t>tmin and must stay fixed at their base value 1
    # (GAMS QFHENDSCAL.FX at tmin carries over structurally).
    for f in cal.db.sets["f"]:
        if ("QFHENDSCAL", f) not in pre:
            pre[("QFHENDSCAL", f)] = 1.0
    # debt stocks
    pre[("GDEBT", t)] = lg("GDEBT", tprev) + lg("GBOR", tprev)
    for i2 in ("govz", "ngovz"):
        pre[("FDEBT", i2)] = lg("FDEBT", i2) + lg("FBOR", i2)
    return pre


def _solve_path(cal, clo, TSOL, verbose, warm=None):
    """March through TSOL solving one period at a time.

    `warm` is an optional dict t -> State used to start each period instead of
    the growth-scaled previous solution; the reference pass uses it to start
    from the calibration pass, which is already at the answer.
    """
    sols = {}
    for k, t in enumerate(TSOL):
        cal._solve_t = t
        V0 = build_state(cal, t)               # correct period-t exogenous vals
        L = sols.get(TSOL[k - 1], {}) if k > 0 else {}

        cf = closure_fixed(cal, t, clo)

        # recursive dynamics: fix predetermined capital & debt stocks from the
        # previous period, so their identity equations leave the simultaneous
        # system (which then solves like a well-conditioned static model).
        if k > 0:
            tprev = TSOL[k - 1]
            prev = sols[tprev]
            pre = predetermined_stocks(cal, t, tprev, prev)
            for (nm, idx), val in pre.items():
                V0.setdefault(nm, {})[idx] = val
                cf.add((nm, idx))

        part = build_partition(cal, t, V0, L, clo, cf)

        # Sector-target periods fix TFPSCAL (solver.closure_fixed). Its level
        # is a per-period productivity residual, so when the aggregate
        # calibration hands over to sector targets mid-path the residual of
        # the last aggregate year must carry, not reset to zero.
        if (k > 0 and ("TFPSCAL", t) in cf
                and any(tt == t for (_, tt) in getattr(cal, "va_target", {}))):
            tprev = TSOL[k - 1]
            if tprev in prev.get("TFPSCAL", {}):
                V0["TFPSCAL"][t] = prev["TFPSCAL"][tprev]

        # warm start remaining free variables from the previous solution
        # (near-balanced base path), scaling quantity-like families by growth.
        if warm is not None and t in warm:
            src = warm[t]
            for (nm, idx) in part[0]:
                if idx in src.get(nm, {}):
                    V0[nm][idx] = src[nm][idx]
        elif k > 0:
            gr = cal.gdpindex[t] / cal.gdpindex[TSOL[k - 1]]
            free = part[0]
            for (nm, idx) in free:
                pd = prev.get(nm, {})
                if idx in pd:
                    V0[nm][idx] = (pd[idx] * gr if nm in _QTY_FAMILIES
                                   else pd[idx])

        t0 = time.time()
        Vs, info, _ = solve_period_newton(cal, t, V0, L, clo, partition=part)
        if not info["success"]:
            # A warm start scaled by GDP growth can be far off when an exogenous
            # path steps (government consumption jumps from 8.9% to 10.7% of GDP
            # in 2020), and a line-searched Newton stalls there. The trust-region
            # least-squares pass is robust enough to get inside the basin but
            # converges slowly once there, so hand back to Newton to finish.
            Vl, il, _ = solve_period_lsq(cal, t, V0, L, clo, partition=part)
            Vs, info = Vl, il
            for _ in range(3):
                Vn, inn, _ = solve_period_newton(cal, t, Vs, L, clo,
                                                 partition=part)
                if inn["max_resid_all"] >= info["max_resid_all"]:
                    break
                Vs, info = Vn, inn
                if info["success"]:
                    break
        sols[t] = Vs
        if verbose:
            print(f"  t={t}  resid={info['max_resid_all']:.1e}  "
                  f"GDPMP={Vs['GDPMP'][t]:9.2f}  ({time.time()-t0:.1f}s)  "
                  f"{'OK' if info['success'] else 'FALLBACK'}")
    return sols


# par-redefn-0.inc: "0"-paths keyed (..., t), and scalars keyed by t alone.
_REDEFN_PATHS = [
    ("QFINS0", "QFINS"), ("SHIF0", "SHIF"), ("QFS0", "QFS"),
    ("QFHEND0", "QFHEND"),
    ("GOVRECGDP0", "GOVRECGDP"), ("GOVSPNDGDP0", "GOVSPNDGDP"),
    ("NGOVPAYGDP0", "NGOVPAYGDP"), ("GOVRECABS0", "GOVRECABS"),
    ("GOVSPNDABS0", "GOVSPNDABS"), ("NGOVPAYABS0", "NGOVPAYABS"),
]
_REDEFN_SCALARS = [
    ("PREXR0", "PREXR"), ("TRSMREC0", "TRSMREC"), ("INVVALF0", "INVVALF"),
    ("LABPARTRAT0", "LABPARTRAT"),
]
# Value at which each tax/subsidy scaling variable leaves its bar unchanged.
_NEUTRAL_SCAL = {"TASCAL": 0.0, "TFSCAL": 0.0, "TQSCAL": 1.0, "TESCAL": 1.0,
                 "TMSCAL": 1.0, "TVACSCAL": 1.0, "TFASCAL": 1.0,
                 "SUBCSCAL": 1.0}


def par_redefn_0(cal, sols, verbose=True):
    """par-redefn-0.inc (mod.gms 4078): rebase the "0"-parameters on the
    reference solution.

    After this a scenario is a deviation from the reference path rather than
    from the original calibration, which is what `sim.gms` assumes when it
    restarts from `save\\mod`.

    The file also rescales the tax and subsidy bars by their solved scaling
    variables (`tqb = tqb*TQSCAL.L` and so on). Every one of those scalars sits
    at its neutral value in the base run, so those statements are exact no-ops
    here and the bars are left alone. That is asserted rather than assumed: a
    scenario that moves one of them needs the bars made t-indexed first, and
    should fail loudly instead of silently rebasing onto the wrong level.
    """
    changed = {}
    for attr, var in _REDEFN_PATHS:
        path = getattr(cal, attr, None)
        if path is None:
            continue
        worst = 0.0
        for key in list(path):
            if not isinstance(key, tuple):
                continue
            t, idx = key[-1], key[:-1]
            if t not in sols:
                continue
            idx = idx[0] if len(idx) == 1 else idx
            got = sols[t].get(var, {}).get(idx)
            if got is None:
                continue
            worst = max(worst, abs(got - path[key]) / max(abs(path[key]), 1e-8))
            path[key] = got
        changed[attr] = worst

    for attr, var in _REDEFN_SCALARS:
        path = getattr(cal, attr, None)
        if path is None:
            continue
        worst = 0.0
        for t in list(path):
            if t not in sols:
                continue
            got = sols[t].get(var, {}).get(t)
            if got is None:
                continue
            worst = max(worst, abs(got - path[t]) / max(abs(path[t]), 1e-8))
            path[t] = got
        changed[attr] = worst

    for nm, neutral in _NEUTRAL_SCAL.items():
        for t, V in sols.items():
            got = V.get(nm, {}).get(t)
            if got is not None and abs(got - neutral) > 1e-9:
                raise NotImplementedError(
                    f"{nm} solved to {got} at {t}, not its neutral {neutral}. "
                    "par-redefn-0.inc rescales that variable's bar, which this "
                    "port only implements for the neutral case; make the bar "
                    "t-indexed in state.Closure before rebasing.")

    # Government demand: the bar becomes the solved quantity and QGSCAL resets
    # to zero, so the reference path is reproduced with no scaling applied.
    cal.qgb0 = {(c, t): V.get("QG", {}).get(c, 0.0)
                for t, V in sols.items() for c in cal.db.sets["c"]}
    # The investment bar is deliberately NOT rebased. `dkinsb0 = DKINS0` at
    # mod.gms 1135 is a one-time parameter assignment in the setup section;
    # par-redefn-0.inc runs three thousand lines later and rebases DKINS0
    # without ever recomputing dkinsb from it. The bar therefore stays at
    # DKINS00*gdpindex and the rebased ISCAL0/IADJ0 carry the reference level
    # through `DKINS = dkinsb*ISCAL*(1 + IADJ*iadj01)` (mod.gms 3038, 3043).
    # Rebasing the bar as well double-counts ISCAL -- worth 9.4 on DKNGOVDEF at
    # 2026, absorbed by IADJ drifting to 0.076.
    #
    # Contrast qgb and tyb, which par-redefn-0.inc DOES rebase (lines 56, 159)
    # and then zeroes their scalars, so the level moves into the bar exactly
    # once. Those two are the only such cases besides the tax bars, whose
    # scalars sit at neutral in the reference anyway.

    # Direct tax: TY0 = TY.L then TYSCAL0 = 0 (par-redefn-0.inc 94, 161), the
    # same bar-absorbs-the-level pattern as QG/QGSCAL above.
    cal.TY0 = {(ins, t): V.get("TY", {}).get(ins, 0.0)
               for t, V in sols.items() for ins in cal.db.sets["insdng"]}

    # The remaining ~100 statements in par-redefn-0.inc all say the same thing:
    # every "0"-parameter takes the solved reference level, and varinit.inc
    # starts the variables there. Rather than transcribe each one, hand the
    # whole reference solution to `state.build_state`, which seeds from it and
    # applies the handful of documented neutral resets.
    #
    # This is not merely a better initial guess. `base_closure_fixed` pins each
    # closure-fixed variable at the value it finds in V, so without this the
    # scenario closure clamps RNDFG, NFFG, MPSSCAL, NFFINSSCAL, ISCAL and the
    # transfer scalars at calibrated levels that differ from the reference by
    # up to 39% -- and an unshocked scenario year cannot reproduce the
    # reference no matter how the shock is ramped.
    cal.ref0 = {t: {nm: dict(d) for nm, d in V.items()} for t, V in sols.items()}

    if verbose:
        moved = {k: v for k, v in changed.items() if v > 1e-9}
        print("  par_redefn_0: rebased "
              f"{len(changed)} paths + qgb/TY0 + full ref0 levels; "
              "materially changed: "
              f"{ {k: round(v, 4) for k, v in sorted(moved.items())} }")
    return changed


def run_base(cal, dcal01=True, verbose=True, periods=None):
    """SOLUCION 1 (mod.gms 3947-4010): the calibration pass.

    dcal01 = 1 pins RGDPFC to the exogenous target path and lets TFPSCAL flex,
    so productivity is backed out from the GDP path rather than the other way
    round. Returns dict t -> solved State V.
    """
    TSOL = periods or cal.db.sets["tsol"]
    # The import quotas are complementarities (mod.gms 2608). In the 2020
    # COVID year both go slack -- GAMS's MCP takes that branch; solving the
    # binding branch as an equality forces imports up to the ceiling and puts
    # NFFINS(2020) 1.11 above GAMS. Sweeping the base pass gives GAMS's value
    # to four decimals. Lazy import: complementarity imports _solve_path.
    from .complementarity import sweep_solve
    cal.quota_slack = set()
    return sweep_solve(cal, Closure(cal, dcal01=dcal01), TSOL, verbose,
                       log=print if verbose else (lambda *a, **k: None))


def run_reference(cal, pass1, verbose=True, periods=None):
    """SOLUCION 2 (mod.gms 4020-4076): the reference pass.

    Freezes the productivity the calibration pass backed out into `tfpexog`,
    fixes TFPSCAL at zero and frees RGDPFC, so from here on GDP is an outcome
    rather than a target.

    For the base run this describes the same equilibrium, and that is the point
    rather than a coincidence: `tfpexog` is set to exactly the `1 + TFPSCAL*tfp01`
    that pass 1 found, and resetting QFINS0 to the pass-1 capital stocks zeroes
    the government-capital term that EQ_PRODFN carries when dcal01 = 0. So the
    pass-1 solution satisfies every pass-2 equation, and we assert that rather
    than assume it.

    It still has to be run, because `sim.gms` restarts from `save\\mod` — every
    counterfactual is a deviation from this reference, not from the calibration
    pass, and `par_redefn_0` rewrites the "0"-parameters off this solution.

    Mutates `cal` in place (tfpexog, QFINS0), matching the GAMS control flow.
    """
    S = cal.db.sets
    TSOL = periods or S["tsol"]
    FCAP, A, INS = S["fcap"], S["a"], S["ins"]

    # mod.gms 4039-4040: tfpexog absorbs the calibrated scaling factor. Note it
    # REPLACES tfpexog rather than multiplying into it; that is value-preserving
    # only because tfpexog starts at one, which holds for this dataset.
    for t in TSOL:
        scal = pass1[t]["TFPSCAL"][t]
        for a in A:
            cal.tfpexog[(a, t)] = 1.0 + scal * cal.tfp01[a]

    # mod.gms 4030: the capital stocks EQ_PRODFN measures deviations from.
    for t in TSOL:
        for ins in INS:
            for fc in FCAP:
                if (ins, fc, t) in cal.QFINS0:
                    cal.QFINS0[(ins, fc, t)] = pass1[t]["QFINS"].get((ins, fc),
                                                                     0.0)

    from .complementarity import sweep_solve
    cal.quota_slack = set()
    return sweep_solve(cal, Closure(cal, dcal01=False), TSOL, verbose,
                       warm=pass1, log=print if verbose else (lambda *a, **k: None))
