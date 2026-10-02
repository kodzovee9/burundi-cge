"""Counterfactual scenarios: sim.gms plus user-files/<app>/<app>-sim2.inc.

A scenario is a set of multiplicative or additive overrides on the calibrated
parameters, plus closure switches, applied on top of the *reference* path — the
solution `dynamics.run_reference` produces and `dynamics.par_redefn_0` rebases
the "0"-parameters on. That ordering is not cosmetic: `sim.gms` restarts from
`save\\mod`, so every shock is measured against the reference, and shocks like
the exchange-rate premium are expressed as ratios to `PREXR0` — which only means
the right thing once `PREXR0` holds the solved reference premium rather than its
flat calibrated placeholder.

**The base scenario is never solved.** `sim.gms` line 507 reads
`IF (NOT simbase(sim), SOLVE GEM USING MCP;)`, so `base` is reported straight
from the levels restored from `save\\mod`. The closure overrides sim2.inc writes
for `base` (`siclossim('base') = 1`, `govrecrulesim('base',acgovrec) = 1`, ...)
are therefore dead code, and applying them is wrong: doing so moves the base
growth rates 0.65 to 1.76 percentage points away from the reference application's results,
against 0.014 to 0.085 for the reference closure.

How a scenario's row-of-the-world closure is resolved is worth spelling out,
because three files interact. mod.gms sets `rowclossim(sim,t)` from the data for
every period. sim2.inc then sets `rowclossim(simcur,tmin) = 1` — tmin only.
sim.gms line 417 would copy the base closure across, but only for a sim with no
`rowclos` set at *any* period, so it skips these. Finally
`diagnostics-clos.inc` propagates a sim's tmin value to every period where the
sim has none. Net effect: `base` keeps the dataset's 2-then-4 path, and every
other current scenario runs `rowclos = 1` throughout — a flexible real exchange
rate with the premium pinned, which is what makes "a single flexible exchange
rate" mean anything.
"""

from __future__ import annotations

from dataclasses import dataclass, field

from .dynamics import _solve_path
from .complementarity import sweep_solve
from .state import Closure


@dataclass
class Scenario:
    """Overrides applied to a calibrated model to define a counterfactual.

    Ratios multiply the corresponding reference parameter, matching
    par-defn-sim.inc (`qmbar(c,t) = qmbar0(c,t)*qmbarsim(sim,c,t)`); absent keys
    leave the reference value untouched.
    """

    name: str
    rowclos: dict | None = None            # period -> closure code
    govclos: int | None = None
    siclos: int | None = None
    prexr_ratio: dict = field(default_factory=dict)     # t -> ratio on PREXR0
    rexr_ratio: dict = field(default_factory=dict)      # t -> ratio on REXR0
    qmbar_ratio: dict = field(default_factory=dict)     # (c, t) -> ratio
    # NOTE the target. GAMS calls this `qfssim`, but par-defn-sim.inc 260-261
    # applies it to `qfinsb` -- the INSTITUTIONAL endowment bar -- not to QFS.
    # Total factor supply then follows endogenously through EQ_FACSUP. Shocking
    # QFS directly would fix supply and break that link.
    qfinsb_ratio: dict = field(default_factory=dict)   # (f, t) -> ratio
    quota_slack: set = field(default_factory=set)       # (c, t) off the quota
    rules: dict = field(default_factory=dict)  # par name -> {account: value}
    ddkins: dict = field(default_factory=dict)   # (ins2, fcap, t) -> quantity
    mpcapgov: dict = field(default_factory=dict)  # fcap -> marginal product
    mtfp: dict = field(default_factory=dict)      # (a, fcap) -> relative strength
    qgb_ratio: dict = field(default_factory=dict)     # (c, t) -> ratio on qgb0
    fprdab_ratio: dict = field(default_factory=dict)  # (f, a, t) -> ratio
    # Factors on the idle-resource closure (complementarity.py): the rent is
    # held at its reference path and the endowment may go partly idle; a
    # period in which demand exceeds the endowment reverts to full employment.
    idle_factors: tuple = ()
    # (ins, ins', t) -> ratio on the transfer path, e.g. grants to government
    trnsfr_ratio: dict = field(default_factory=dict)
    # technology shifters (port-only): (c, a, t) -> factor on an input
    # coefficient; (ct, c, t) -> factor on the margin of ct on c
    ica_scale: dict = field(default_factory=dict)
    margin_scale: dict = field(default_factory=dict)
    # targeted FDI (port-only): t -> foreign currency, all to `fdi_target`
    fdi_add: dict = field(default_factory=dict)
    fdi_target: str | None = None

    def apply(self, cal):
        """Mutate `cal` in place. Call on a model already carrying the
        reference parameters, i.e. after `par_redefn_0`."""
        P = cal.db.pars
        if self.rowclos is not None:
            P["rowclos0"] = {(t,): float(v) for t, v in self.rowclos.items()}
        if self.govclos is not None:
            P["govclos0"] = {(): float(self.govclos)}
        if self.siclos is not None:
            P["siclos0"] = {(): float(self.siclos)}
        for par, spec in self.rules.items():
            P[par] = {(a,): float(v) for a, v in spec.items()}
        for t, r in self.prexr_ratio.items():
            if t in cal.PREXR0:
                cal.PREXR0[t] *= r
        for t, r in self.rexr_ratio.items():
            if hasattr(cal, "REXR0") and isinstance(cal.REXR0, dict):
                if t in cal.REXR0:
                    cal.REXR0[t] *= r
        for key, r in self.qmbar_ratio.items():
            if key in cal.qmbar0:
                cal.qmbar0[key] *= r
                cal.qmbar_scale[key] = cal.qmbar_scale.get(key, 1.0) * r
        for (f, t), r in self.qfinsb_ratio.items():
            for key in [k for k in cal.QFINS0 if k[1] == f and k[2] == t]:
                cal.QFINS0[key] *= r
        for key, r in self.qgb_ratio.items():
            if key in getattr(cal, "qgb0", {}):
                cal.qgb0[key] *= r
        if self.fprdab_ratio:
            # par-defn-sim.inc 144-146: the ratio multiplies fprdab0, which is
            # flat at the calibrated value for every period.
            cal.fprdab_path = {k: cal.fprdab00.get(k[:2], 0.0) * r
                               for k, r in self.fprdab_ratio.items()}
        else:
            cal.fprdab_path = None
        cal.quota_slack = set(self.quota_slack)
        cal.trnsfrb_ratio = dict(self.trnsfr_ratio)
        cal.ica_scale = dict(self.ica_scale)
        cal.margin_scale = dict(self.margin_scale)
        cal.fdi_add = dict(self.fdi_add)
        cal.fdi_target = self.fdi_target
        cal.ddkins = dict(self.ddkins)
        if self.mpcapgov or self.mtfp:
            cal.mpcapgov = dict(self.mpcapgov)
            cal.mtfp = dict(self.mtfp)
            recompute_mpk(cal)
        return cal


def recompute_mpk(cal):
    """Rebuild `cal.mpk` from `cal.mpcapgov` and `cal.mtfp` (mod.gms 1869-1878).

    The marginal product of public capital in each activity is the aggregate
    marginal product shared out by the activity's share of value added and by
    the relative strength `mtfp`, then rescaled so the activity-specific products
    sum back to the aggregate. `calibrate` does this once; a scenario that turns
    the channel on has to redo it, because `mpk` is a calibrated parameter rather
    than something the solver recovers.
    """
    A = cal.db.sets["a"]
    FCAP = cal.db.sets["fcap"]
    sam = cal.db.pars["sam"]
    va_cap = {a: sum(sam.get((fp, a), 0.0) for fp in FCAP) for a in A}
    va_tot = sum(va_cap.values())
    mpk00 = {}
    for fcap in FCAP:
        agg = cal.mpcapgov.get(fcap)
        if not agg:
            continue
        for a in A:
            if cal.mtfp.get((a, fcap)):
                mpk00[(a, fcap)] = (agg * (va_cap[a] / va_tot)
                                    * cal.mtfp[(a, fcap)])
        tot = sum(mpk00.get((ap, fcap), 0.0) for ap in A)
        if tot:
            for a in A:
                if mpk00.get((a, fcap)):
                    mpk00[(a, fcap)] *= agg / tot
        mpk00[("total", fcap)] = sum(mpk00.get((a, fcap), 0.0) for a in A)
    cal.mpk = {(ac, fc, tt): val for (ac, fc), val in mpk00.items()
               for tt in cal.TSOL}


def scaled(scen, lam):
    """`scen` with its parameter shocks scaled back to a fraction `lam`.

    Ratios interpolate **geometrically**, `r**lam`, from one at lam=0 to the
    full ratio at lam=1. Linear interpolation is wrong here: the import quota is
    relaxed by a factor of 45, and a quarter of that linearly is still a factor
    of 12 — a shock large enough to diverge on its own. Geometrically a quarter
    step is 45**0.25, about 2.6, which is what "a quarter of the way there"
    should mean for a multiplicative shock.

    Closure switches are discrete and carry over unscaled — they have to be
    taken in one step, which is why the continuation driver applies the closure
    change first and only then ramps the shock.
    """
    def s(d):
        return {k: r ** lam for k, r in d.items()}
    # The quota branch is discrete like the closure switch: partway through a
    # continuation the quota is either binding or it is not.
    # ddkins and mpcapgov are levels, not ratios: they scale linearly from zero,
    # where a ratio would scale geometrically from one.
    return Scenario(name=f"{scen.name}@{lam:g}", rowclos=scen.rowclos,
                    govclos=scen.govclos, siclos=scen.siclos,
                    prexr_ratio=s(scen.prexr_ratio),
                    rexr_ratio=s(scen.rexr_ratio),
                    qmbar_ratio=s(scen.qmbar_ratio),
                    qfinsb_ratio=s(scen.qfinsb_ratio),
                    quota_slack=set(scen.quota_slack) if lam > 0 else set(),
                    rules=scen.rules,
                    ddkins={k: v * lam for k, v in scen.ddkins.items()},
                    mpcapgov={k: v * lam for k, v in scen.mpcapgov.items()},
                    mtfp=scen.mtfp,
                    qgb_ratio=s(scen.qgb_ratio),
                    fprdab_ratio=s(scen.fprdab_ratio),
                    idle_factors=scen.idle_factors)


def uni(cal, periods):
    """`uni`: two-step exchange-rate unification and removal of import quotas.

    sim2.inc 497-503. The premium is halved in 2026 and taken to exactly one
    from 2027 — the ratio is written `(1 + (PREXR0-1)*k)/PREXR0` precisely so
    that `PREXR0 * ratio` lands on `1 + (PREXR0-1)*k` regardless of what the
    reference premium turned out to be. Import quotas on the `cmbar`
    commodities are multiplied by 45 from 2026, which removes them in all but
    name.

    Requires `rowclos = 1` throughout, so the real exchange rate flexes while
    the premium stays pinned at the shocked value.
    """
    cmbar = cal.db.sets["cmbar"]
    prexr, qmbar = {}, {}
    for t in periods:
        yr = int(t)
        if yr <= 2025:
            k = None                       # unchanged
        elif yr == 2026:
            k = 0.5                        # half the premium
        else:
            k = 0.0                        # unified
        if k is None:
            continue
        p0 = cal.PREXR0[t]
        prexr[t] = (1.0 + (p0 - 1.0) * k) / p0
        for c in cmbar:
            if (c, t) in cal.qmbar0:
                qmbar[(c, t)] = 45.0
    # Relaxing the quota by a factor of 45 is meant to stop it binding, so the
    # complementarity moves to its slack branch: rent zero, EQ_QMCONST dropped,
    # imports set by ordinary demand. Keeping the equality instead would force
    # imports up to the relaxed ceiling -- a 45-fold rise, not a liberalisation.
    return Scenario(name="uni",
                    rowclos={t: 1 for t in periods},
                    prexr_ratio=prexr, qmbar_ratio=qmbar,
                    quota_slack=set(qmbar),
                    **scenario_closure(cal))


def scenario_closure(cal):
    """The closure every non-base scenario inherits.

    sim.gms 407-417 cascades in two steps: `base` fills any unset switch from
    the dataset, then every other sim fills any unset switch from `base`.
    sim2.inc has already set `base` to `siclos = 1` with all three rule sets to
    1, so the scenarios inherit those — even though `base` itself is never
    solved and those settings never touch the reference application's base path.

    It matters most for government spending. Rule 1 fixes the scaling variable
    (`QGSCAL`, `ISCAL`) instead of the GDP ratio, which pins government
    consumption and investment in real terms rather than letting them track
    GDP. That is visible in the reference application's tables: `GovCon` and
    `GovFixInv` grow at the same rate in *both* base and uni, which only happens
    if the government path is exogenous in the scenarios.

    siclos = 1 likewise makes investment the item that clears savings-investment
    rather than the savings rate.
    """
    S = cal.db.sets
    return dict(siclos=1,
                rules={"govrecrule0": {a: 1 for a in S["acgovrec"]},
                       "govspndrule0": {a: 1 for a in S["acgovspnd"]},
                       "ngovpayrule0": {a: 1 for a in S["acngovpay"]}})


# bdi2019-sim2.inc 608-623 (the block above it is inside $ONTEXT). The
# infrastructure push ramps in over 2026-2029, runs flat 2030-2035, then ramps
# back out, so the capital stock it builds is temporary by construction.
SCALFACINF = {"2026": 0.2, "2027": 0.4, "2028": 0.6, "2029": 0.8,
              "2030": 1.0, "2031": 1.0, "2032": 1.0, "2033": 1.0,
              "2034": 1.0, "2035": 1.0, "2036": 0.8, "2037": 0.6,
              "2038": 0.4, "2039": 0.2, "2040": 0.0}

# bdi2019-sim2.inc 659-674: the activities whose productivity public capital is
# assumed to raise. `uni+inf` is in `siminftrg`, the TARGETED variant, so this is
# a strict subset of activities -- services other than transport, and the public
# sector itself, are excluded.
ATRG = ("a-agr", "a-food", "a-texwapp", "a-woodpaper", "a-chemplast",
        "a-prodminnmet", "a-met", "a-oman", "a-elect", "a-water",
        "a-construc", "a-transp")

INF_GDP_SHARE = 0.02        # sim2.inc 648
INF_MPCAPGOV = 0.65         # sim2.inc 656


def uni_inf(cal, periods):
    """`uni+inf`: exchange-rate unification plus a public infrastructure push.

    Three things on top of `uni` (bdi2019-sim2.inc 648-676):

    1. `ddkins('govz','f-capgov',t)`, an additive 2%-of-GDP of extra public
       capital formation, ramped by `scalfacinf`. It is expressed as
       `GDPMP0*0.02*scalfacinf/PK0` -- a quantity, deflated by the reference
       price of government capital. sim2.inc multiplies by `scaling('samsol')`
       and par-defn-sim.inc 114 divides it straight back out, so the two cancel
       and no scaling survives into model units.
    2. From 2040 the shifter switches to replacement-only: whatever the ramp
       accumulated, times the depreciation rate, so the extra stock is held
       rather than grown.
    3. `mpcapgov('f-capgov') = 0.65` with `mtfp(atrg,'f-capgov') = 1`, which is
       what actually turns the productivity channel on. Both are zero in the
       base, so `EQ_PRODFN`'s additive `(QFINS - QFINS0)*mpk` term vanishes
       there and this scenario is the first thing in the port to exercise it.

    Requires a solved reference: `GDPMP0` and `PK0` are reference levels, so
    `par_redefn_0` must have run.
    """
    ref = getattr(cal, "ref0", None)
    if not ref:
        raise RuntimeError("uni_inf needs a rebased reference; "
                           "call dynamics.par_redefn_0 first")
    dd = {}
    for t in periods:
        k = SCALFACINF.get(t, 0.0)
        if not k:
            continue
        gdp = ref[t]["GDPMP"][t]
        pk = ref[t]["PK"]["f-capgov"]
        dd[("govz", "f-capgov", t)] = gdp * INF_GDP_SHARE * k / pk
    # t4050: replacement investment only, sized off everything laid down so far.
    dep = cal.deprcap.get("f-capgov", 0.0)
    for t in periods:
        if int(t) < 2040:
            continue
        prior = sum(v for (i2, fc, tp), v in dd.items() if int(tp) < int(t))
        dd[("govz", "f-capgov", t)] = prior * dep

    base = uni(cal, periods)
    base.name = "uni+inf"
    base.ddkins = dd
    base.mpcapgov = {"f-capgov": INF_MPCAPGOV}
    base.mtfp = {(a, "f-capgov"): 1.0 for a in ATRG
                 if a in set(cal.db.sets["a"])}
    return base


# bdi2019-sim2.inc 733-769 and 935-950. Both ramp linearly to one; note they
# start in DIFFERENT years, which is the point of the scenario -- the spending
# begins in 2026 but the human-capital gain it buys only shows up from 2031.
SCALFACGSPNDPC = {str(y): min(0.1 * (y - 2025), 1.0) for y in range(2026, 2041)}
SCALFACHCI = {str(y): 0.1 * (y - 2030) for y in range(2031, 2041)}

# sim2.inc 721-728, 915-930: Burundi against the 90th percentile of low-income
# countries. `uni+inf+hd` is in `simhciLIC90pc`, so it closes the whole gap to
# the 90th percentile on spending (no 0.5 factor -- that is the `hd+tfphd`
# variant) and takes the corresponding HCI gain.
HCI_BDI, HCI_LIC90 = 0.3861706, 0.4174056
GEDUPC_BDI, GEDUPC_LIC90 = 50.80674, 82.37889
GHEALTHPC_BDI, GHEALTHPC_LIC90 = 18.33478, 33.86548


def uni_inf_hd(cal, periods):
    """`uni+inf+hd`: `uni+inf` plus a human-development push.

    `uni+inf+hd` sits in three scenario sets, and its shock is the union of what
    each contributes: `simuni` (the premium and quota), `siminftrg` (the
    infrastructure push), and `simhciLIC90pc` -- this function's addition
    (bdi2019-sim2.inc 799-800, 961-962, 971-972):

    * government demand for education and health rises to close Burundi's gap to
      the 90th percentile of low-income countries in per-capita spending, phased
      in over 2026-2035. That is a factor of 1.62 on education and 1.85 on
      health at full effect -- easily the largest of the three scenarios'
      shocks, and why `GovCon` growth jumps in the reference application's tables.
    * labour productivity (`fprdab`) rises with the human-capital index gap to
      the same benchmark, but only from **2031**, ramping to full by 2040. The
      lag is deliberate: educating a cohort does not raise output the same year.

    Note the spending shock hits `qgb`, which `par_redefn_0` has already rebased
    to the solved reference `QG`. Multiplying it is what makes this a deviation
    from the reference rather than from the calibration.
    """
    S = cal.db.sets
    edu_gap = GEDUPC_LIC90 / GEDUPC_BDI - 1.0
    health_gap = GHEALTHPC_LIC90 / GHEALTHPC_BDI - 1.0
    hci_gap = HCI_LIC90 / HCI_BDI - 1.0

    qgb_ratio = {}
    for t in periods:
        k = SCALFACGSPNDPC.get(t)
        if k is None:                      # 2019-2025: unchanged
            continue
        qgb_ratio[("c-edu", t)] = 1.0 + edu_gap * k
        qgb_ratio[("c-health", t)] = 1.0 + health_gap * k

    fprdab_ratio = {}
    FLAB, A = S["flab"], S["a"]
    for t in periods:
        k = SCALFACHCI.get(t)
        if k is None:                      # 2019-2030: unchanged
            continue
        for f in FLAB:
            for a in A:
                fprdab_ratio[(f, a, t)] = 1.0 + hci_gap * k

    base = uni_inf(cal, periods)
    base.name = "uni+inf+hd"
    base.qgb_ratio = qgb_ratio
    base.fprdab_ratio = fprdab_ratio
    return base


# bdi2019-sim2.inc 540-556: ramps 0.1 to 1.0 over 2026-2035, then holds.
SCALMIN = {str(y): min(0.1 * (y - 2025), 1.0) for y in range(2026, 2041)}

MIN_MULTIPLE = 2.0        # sim2.inc 559, the `simmin200p` variant


def combi(cal, periods):
    """`combi`: `uni+inf+hd` plus a mining expansion.

    The fourth and last layer (bdi2019-sim2.inc 558-559, set `simmin200p`): the
    endowment of the mining natural resource `f-nrmin` rises by 200% at full
    effect, phased in over 2026-2035. So the factor ends at three times its
    reference level.

    The shock goes to `qfinsb`, the institutional endowment bar, NOT to `QFS`,
    despite GAMS naming the parameter `qfssim` (par-defn-sim.inc 260-261).
    Total supply is then whatever `EQ_FACSUP` adds up from the institutions'
    holdings. That distinction matters: `f-nrmin` is held by households, the
    government and the rest of the world in different proportions, so who gets
    the income from the expansion follows from the existing ownership shares
    rather than being imposed.

    This is what drives the large export response in the reference application
    (export growth well above `uni+inf+hd`'s), since mining output is
    overwhelmingly exported.

    **DOES NOT CURRENTLY CONVERGE. Do not use these results.** The shock matches
    sim2.inc, but from 2026 every period falls back with residuals of 1e-1 rather
    than the 1e-13 every other scenario reaches. The cause is `WF('f-nrmin')`
    collapsing onto the 1e-9 positivity floor in `solver.POSITIVE_FAMILIES`: the
    resource is fully absorbed (QFS 23.94 against QF(a-min) 23.92) but its price
    goes to zero, which erases mining rent income -- 0.769 x 8.0, about 6.2 units
    -- and drags GDP down 14% where the reference application has it rising.

    A zero factor price is not necessarily wrong; it is what an inelastic demand
    curve gives when supply triples. What is wrong is that our equality system
    cannot represent it. In the GAMS MCP, `WF.LO = 0` makes EQ_FACEQ an
    INEQUALITY at the bound -- supply may exceed demand, with the surplus
    unemployed -- while we force exact clearing and grind WF into the floor.
    Getting `combi` right most likely means treating the factor market as a
    complementarity, as was already done for the import quota (EQ_QMCONST).
    """
    qfinsb_ratio = {}
    for t in periods:
        k = SCALMIN.get(t)
        if k is None:                      # 2019-2025: unchanged
            continue
        qfinsb_ratio[("f-nrmin", t)] = 1.0 + MIN_MULTIPLE * k

    base = uni_inf_hd(cal, periods)
    base.name = "combi"
    base.qfinsb_ratio = qfinsb_ratio
    # A tripled endowment is used only as far as demand at the reference rent
    # allows; the rest stays in the ground. Without this the resource is
    # forced into use, its rent is driven to zero, and the solve breaks
    # (APPENDIX.md, A.5.7). This is OUR combi: "what if the resource tripled",
    # not GAMS's "what if its use tripled".
    base.idle_factors = ("f-nrmin",)
    return base


# Budget support accompanying unification (port-only): extra grants to the
# government, as a share of reference GDP, converted to foreign currency at
# the reference exchange rate. Reform programmes of this kind usually come
# with external financing; the amounts are illustrative, not announced.
BUDGET_SUPPORT = {"2026": 0.01, "2027": 0.01, "2028": 0.005}


def uni_bs(cal, periods):
    """`uni+bs`: `uni` plus temporary budget support (grants to government of
    1 % of GDP in 2026-27 and 0.5 % in 2028). Under the scenario closure the
    grants lower the direct-tax rate, raise saving and so investment, and add
    foreign exchange, which the real exchange rate absorbs."""
    ref = getattr(cal, "ref0", None)
    if not ref:
        raise RuntimeError("uni_bs needs a rebased reference; call par_redefn_0 first")
    S = cal.db.sets
    ratio = {}
    for t, share in BUDGET_SUPPORT.items():
        if t not in periods:
            continue
        V = ref[t]
        extra = share * V["GDPMP"][t] / V["EXR"][t]          # foreign currency
        for g in S["insgov"]:
            for r in S["insrow"]:
                cur = V["TRNSFR"].get((g, r), 0.0)
                if cur:
                    ratio[(g, r, t)] = 1.0 + extra / cur
    base = uni(cal, periods)
    base.name = "uni+bs"
    base.trnsfr_ratio = ratio
    return base


def uni_bs_inv(cal, periods):
    """`uni+bs-inv`: `uni+bs` with the budget support spent on public
    investment instead of lowering taxes. The extra public capital raises
    productivity in the same activities, at the same return, as `uni+inf`
    (ATRG, INF_MPCAPGOV), and is maintained at replacement level afterwards."""
    base = uni_bs(cal, periods)
    ref = cal.ref0
    dd = {}
    for t, share in BUDGET_SUPPORT.items():
        if t in periods:
            dd[("govz", "f-capgov", t)] = share * ref[t]["GDPMP"][t] / ref[t]["PK"]["f-capgov"]
    dep = cal.deprcap.get("f-capgov", 0.0)
    stock = 0.0
    for t in periods:
        if (("govz", "f-capgov", t)) in dd:
            stock += dd[("govz", "f-capgov", t)]
        elif int(t) > int(max(BUDGET_SUPPORT)) and stock:
            dd[("govz", "f-capgov", t)] = stock * dep       # maintenance only
    base.name = "uni+bs-inv"
    base.ddkins = dd
    base.mpcapgov = {"f-capgov": INF_MPCAPGOV}
    base.mtfp = {(a, "f-capgov"): 1.0 for a in ATRG if a in set(cal.db.sets["a"])}
    return base


# ---- second group of benefit-raising adjustments (port-only; illustrative
# assumptions, each reported as its own increment; APPENDIX.md A.5.2, Table A.10d) --

# Infrastructure effects follow the public capital the programme lays down:
# cumulative ramp, full from 2035 (the stock is then maintained).
_cum, _tot = 0.0, sum(v for y, v in SCALFACINF.items() if int(y) <= 2035)
INFRA_EFFECT = {}
for _y in range(2026, 2041):
    _cum += SCALFACINF.get(str(_y), 0.0) if _y <= 2035 else 0.0
    INFRA_EFFECT[str(_y)] = min(_cum / _tot, 1.0)

TRANSPORT_CUT = 0.15    # transport margins and transport inputs, all goods and activities
MARKETING_CUT = 0.10    # trade (marketing) margins on farm and food products
HYDRO_CUT = 0.50        # fuel per unit of electricity (hydropower replaces diesel)
EDU_SHIFT_2040 = 0.02   # share of each sex's labour force moved from primary to secondary by 2040


def _margins_and_transport(cal, periods):
    S = cal.db.sets
    C, A = S["c"], S["a"]
    msc, isc = {}, {}
    for t in periods:
        k = INFRA_EFFECT.get(t)
        if not k:
            continue
        for c in C:
            for tab in (cal.icm, cal.ice, cal.icd):
                if tab.get(("c-transp", c)):
                    msc[("c-transp", c, t)] = 1 - TRANSPORT_CUT * k
            if c in ("c-agr", "c-food"):
                for tab in (cal.icm, cal.ice, cal.icd):
                    if tab.get(("c-trade", c)):
                        msc[("c-trade", c, t)] = 1 - MARKETING_CUT * k
        for a in A:
            if cal.ica00.get(("c-transp", a)):
                isc[("c-transp", a, t)] = 1 - TRANSPORT_CUT * k
    return msc, isc


def uni_inf_m(cal, periods):
    """`uni+inf-m`: `uni+inf` where the roads and corridor works also cut
    transport costs (margins and transport inputs, -15 % at full effect) and
    marketing margins on farm and food products (-10 %), phased in with the
    public capital the programme builds."""
    base = uni_inf(cal, periods)
    base.name = "uni+inf-m"
    base.margin_scale, base.ica_scale = _margins_and_transport(cal, periods)
    return base


def uni_inf_x(cal, periods):
    """`uni+inf-x`: `uni+inf-m` plus hydropower: fuel per unit of
    electricity falls by half at full effect, as hydro capacity replaces
    diesel generation."""
    base = uni_inf_m(cal, periods)
    base.name = "uni+inf-x"
    for t in periods:
        k = INFRA_EFFECT.get(t)
        if k:
            base.ica_scale[("c-refpet", "a-elect", t)] = 1 - HYDRO_CUT * k
    return base


def _education_shift(cal, periods):
    """qfinsb ratios moving EDU_SHIFT_2040 of each sex's labour force from
    primary to secondary education by 2040, linearly from 2031, when the
    cohorts schooled under the spending push start to enter work. Totals are
    unchanged."""
    ratio = {}
    for t in periods:
        yr = int(t)
        if yr < 2031:
            continue
        share = EDU_SHIFT_2040 * min((yr - 2030) / 10.0, 1.0)
        for sex in ("m", "f"):
            types = [f"f-lab{sex}-{e}" for e in ("n", "p", "s", "t")]
            tot = {f: sum(v for (i, ff, tt), v in cal.QFINS0.items() if ff == f and tt == t)
                   for f in types}
            moved = share * sum(tot.values())
            fp, fs = f"f-lab{sex}-p", f"f-lab{sex}-s"
            if tot[fp] and tot[fs]:
                ratio[(fp, t)] = 1 - moved / tot[fp]
                ratio[(fs, t)] = 1 + moved / tot[fs]
    return ratio


def uni_inf_hd_x(cal, periods):
    """`uni+inf+hd-x`: `uni+inf-x` plus the human-development push, with the
    education spending also changing the education mix of new workers
    (EDU_SHIFT_2040). The HCI-based productivity gain is kept, so this may
    partly overlap it; the increment is reported separately."""
    hd = uni_inf_hd(cal, periods)
    base = uni_inf_x(cal, periods)
    base.name = "uni+inf+hd-x"
    base.qgb_ratio = hd.qgb_ratio
    base.fprdab_ratio = hd.fprdab_ratio
    base.qfinsb_ratio = _education_shift(cal, periods)
    return base


def combi_x(cal, periods):
    """`combi-x`: `uni+inf+hd-x` plus the mining expansion, with foreign
    direct investment bringing in the capital to work the larger resource:
    mining capital rises in step with the endowment (x3 by 2035), financed
    by FDI into mining (INVVALF), so the extra capital and its profits are
    foreign-owned."""
    base = uni_inf_hd_x(cal, periods)
    cmb = combi(cal, periods)
    base.name = "combi-x"
    base.qfinsb_ratio = {**base.qfinsb_ratio, **cmb.qfinsb_ratio}
    base.idle_factors = cmb.idle_factors
    ref = cal.ref0
    dep = cal.deprcap.get("f-capprv", 0.0)
    kref = {t: ref[t]["QF"].get(("f-capprv", "a-min"), 0.0) for t in periods}
    last, prev = periods[-1], periods[-2]
    k_next = lambda t: (kref[periods[periods.index(t) + 1]] if t != last
                        else kref[last] * kref[last] / kref[prev])
    fdi = {}
    for t in periods:
        yr = int(t)
        if yr < 2026:
            continue
        s_now = SCALMIN.get(t, 0.0)
        s_before = SCALMIN.get(str(yr - 1), 0.0)
        extra_next = MIN_MULTIPLE * s_now * k_next(t)       # extra capital wanted next year
        extra_now = MIN_MULTIPLE * s_before * kref[t]
        units = extra_next - (1 - dep) * extra_now          # new capital to install this year
        if units > 0:
            fdi[t] = (units * ref[t]["PK"]["f-capprv"]
                      / (cal.invshr00.get(("f-capprv", "rowz"), 1.0) * ref[t]["EXR"][t]))
    base.fdi_add = fdi
    base.fdi_target = "a-min"
    return base


SCENARIOS = {"uni": uni, "uni+inf": uni_inf, "uni+inf+hd": uni_inf_hd,
             "combi": combi, "uni+bs": uni_bs, "uni+bs-inv": uni_bs_inv,
             "uni+inf-m": uni_inf_m, "uni+inf-x": uni_inf_x,
             "uni+inf+hd-x": uni_inf_hd_x, "combi-x": combi_x}

# Scenarios that solve but whose results are NOT usable. Empty since 2026-09-15:
# `combi` converges on the idle-resource closure (complementarity.py), every
# period < 1e-6. Kept as the hook for anything that fails in future.
UNVALIDATED = set()


def run_scenario(cal, scen, reference, periods, verbose=True, warm=None):
    """Apply `scen` to `cal` and solve the path.

    By default each period is warm-started from the previous period *of this
    scenario*, which is what sim.gms does (`varinit-t2.inc` inside the period
    loop). Starting every period from the reference instead is actively harmful
    once the paths separate: at 2028 the reference switches to rowclos 4 and
    lets the premium float up to 2.82, while `uni` pins it at 1.0, so the
    reference is a worse starting point than the scenario's own previous year.

    Pass `warm=reference` to override, e.g. for a scenario whose shock is small
    enough that the reference is the better start throughout.

    `cal` is mutated, so pass a model built for this scenario alone.
    """
    scen.apply(cal)
    if verbose:
        print(f"=== scenario {scen.name}")
    # Both inequality closures are swept: the quota rents (starting from the
    # slack cells the scenario declared) and any idle-resource factors.
    return sweep_solve(cal, Closure(cal, dcal01=False), periods, verbose,
                       warm=warm, idle_factors=scen.idle_factors,
                       log=print if verbose else (lambda *a, **k: None))


_SNAPSHOT_PARS = ("rowclos0", "govclos0", "siclos0")
# qgb0 is MULTIPLIED in place by a scenario, so it has to be snapshotted or the
# shock compounds the next time a scenario is applied to the same model.
_SNAPSHOT_ATTRS = ("PREXR0", "REXR0", "qmbar0", "qmbar_scale", "QFS0", "qgb0",
                   "QFINS0")


def _snapshot(cal):
    """Copy the parameters a Scenario mutates, so a step can be undone."""
    pars = {k: dict(cal.db.pars[k]) for k in _SNAPSHOT_PARS
            if k in cal.db.pars}
    attrs = {a: dict(getattr(cal, a)) for a in _SNAPSHOT_ATTRS
             if isinstance(getattr(cal, a, None), dict)}
    return pars, attrs


def _restore(cal, snap):
    pars, attrs = snap
    for k, v in pars.items():
        cal.db.pars[k] = dict(v)
    for a, v in attrs.items():
        getattr(cal, a).clear()
        getattr(cal, a).update(v)


def run_scenario_continuation(build, cal, reference, periods,
                              lambdas=(0.0, 0.25, 0.5, 0.75, 1.0),
                              verbose=True):
    """Reach a scenario by continuation, warm-starting each step from the last.

    `uni` cuts the exchange-rate premium from 2.5 to 1.0 and multiplies the
    import quota by 45; taken in one jump from the reference the period solve
    diverges outright — the same globalisation problem the 2020
    government-consumption step caused, only larger. Stepping the shock in and
    carrying the previous solution forward keeps every solve inside its basin.

    `cal` must already carry the reference parameters (i.e. `par_redefn_0` has
    run). Each step is applied to a restored snapshot rather than a rebuilt
    model, because rebuilding means re-solving both passes — about a minute a
    step, for parameters that are cheap to copy.

    The first step is deliberately lam=0: the closure switch alone with no
    parameter shock, which isolates the discrete part of the change from the
    continuous part and should reproduce the reference exactly.
    """
    snap = _snapshot(cal)
    prev = reference
    for lam in lambdas:
        _restore(cal, snap)
        scen = scaled(build(cal, periods), lam)
        scen.apply(cal)
        if verbose:
            print(f"--- continuation step lam={lam:g}", flush=True)
        prev = _solve_path(cal, Closure(cal, dcal01=False), periods,
                           verbose, warm=prev)
    _restore(cal, snap)
    return prev
