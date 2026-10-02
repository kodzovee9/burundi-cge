"""Fuel-shortage mechanisms (port-only; not in GEM-Core). Off unless
`cal.fuel` is set, in which case the model gains three channels through
which the economy can live with less official fuel:

1. **Firms substitute away from fuel** (`sigma_act`). For every activity
   that uses fuel, fuel and value added form a CES with elasticity sigma
   (value-added composite = value added plus the fixed-proportion factor,
   the mining resource). Two indices per activity, both 1 at base prices:
   `FXI(a)`, fuel per unit of output, and `VXI(a)`, value added per unit of
   output. Cost minimisation gives
       FXI / VXI = [ (PQD_fuel / PQD_fuel00) / (PVAc / PVAc00) ]^(-sigma)
       sV * VXI^rho + sF * FXI^rho = 1,  rho = (sigma - 1) / sigma
   with sF the base-year fuel share of (value added + fuel). Saving fuel
   costs value added: output per unit of factors falls. Leontief is
   sigma -> 0 (FXI = VXI = 1).

2. **Informal fuel outside the quota** (`informal`). A second supply of the
   same fuel, `QMI`, bought with foreign exchange at the parallel rate plus
   a smuggling cost mu, at a cost rising with volume:
       border cost = PWM (1 + mu) EXR PREXR (1 + QMI / QM00)^(1 / eta)
   It flows only when the official price with the quota rent exceeds that
   cost -- a complementarity (QMI >= 0 _|_ PM <= informal cost + margins),
   swept like the quotas (`complementarity.sweep_solve`). Users pay the
   official market price; the premium and the rising-cost markup are rents
   (booked with the premium rents, to private capital); PWM (1 + mu) is paid
   abroad. It carries the same domestic distribution margins, no tariff.

3. **Households switch to firewood and charcoal** (`eps_hh`). Part of each
   household's fuel budget moves to farm and forest products (`c-agr`,
   which holds firewood and charcoal) when fuel gets dearer relative to
   them:  share of the fuel budget spent on fuel
       HXI = [ (PQD_fuel,h / PQD_fuel,h00) / (PQD_bio,h / PQD_bio,h00) ]^(-eps)
   and (1 - HXI) of the budget goes to the substitute. The household budget
   is unchanged. Base: HXI = 1.

4. **The pump quota follows foreign-exchange availability** (`fx_quota`).
   Official fuel imports are bought with foreign exchange at the official
   rate, which BRB allocates out of the foreign exchange that reaches it:
   exporters' surrendered receipts, grants to the government and the
   government's net foreign borrowing. The fuel ceiling is BRB's allocation
   to fuel over the world price,
       QM_fuel <= phi * OFX / PWM_fuel
       OFX = sum_c shroe(c) PWE(c) QE(c) + TRNSFR(gov,row) + NFFG
   with phi the share of official foreign exchange going to fuel (0.355 in
   2019). A fall in export receipts or aid, or a rise in the world fuel
   price, deepens the pump shortage by itself. Where the ceiling is observed
   (the backcast's BRB volumes) it is used as is, and the share it implies is
   carried forward afterwards; `fx_share` can set phi year by year. The same
   rule applies to chemicals and fertiliser (`fx_goods`), the other imports
   cleared at the official rate, each with its own share of the pool.

All four are exactly inert at base prices, so the base year stays exact.
They are part of the model by default (`DEFAULT_FUEL`, applied by
`calibration.calibrate`). `calibrate(db, fuel_mech=None)` removes them, which is
only for re-checking the port against the reference application
(`--gams-replication` on the runners).

How informal fuel switches on. The pump price is the official one: world
price at the official rate, plus the 19 % tariff and margins. Official
fuel imports are rationed by the quota, which is BRB's allocation of
official-rate foreign exchange to fuel (channel 4): when it binds, there is a
shortage at the pump and fuel acquires a scarcity value above the pump
price (the quota rent, which accrues to whoever gets pump fuel). Informal
fuel from the DRC and Tanzania is always dearer than the pump: it is bought
with parallel-market foreign exchange and carries a transport cost
(mu = 0.9 puts fuel's 2024 market price at 3.35 times the pump price, the
black-market estimate). It flows once
the shortage is deep enough that the scarcity value reaches the informal
price; from then on the informal price caps it, and further cuts in
official supply are made up informally at rising cost.
"""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class FuelConfig:
    fuel: str = "c-refpet"
    # 1. firms
    sigma_act: float = 0.0                 # 0 = off (Leontief)
    acts: tuple = ()                       # () = every activity using fuel
    # 2. informal supply
    informal: bool = False
    mu: float = 0.25                       # informal markup over PWM (before the premium)
    mu_abroad: float = 0.10                # part of it paid abroad (transport); rest is domestic margin
    eta: float = 2.0                       # supply elasticity of informal fuel
    # 5. households
    eps_hh: float = 0.0                    # 0 = off
    bio: str = "c-agr"                     # firewood and charcoal
    # 4. quota tied to official foreign exchange
    fx_quota: bool = False
    # goods whose import ceiling is a share of official FX (BRB's allocation):
    # fuel and, since 2026-09-30, chemicals and fertiliser -- together the
    # imports cleared at the official rate
    fx_goods: tuple = ("c-refpet",)
    fx_from: dict = field(default_factory=dict)    # c -> first year the rule sets the ceiling ("2020")
    fx_share: dict = field(default_factory=dict)   # (c, t) -> phi, explicit; else carried
    # filled by calibrate_fuel
    AF: tuple = ()
    sF: dict = field(default_factory=dict)
    pvac00: dict = field(default_factory=dict)
    fx_share00: dict = field(default_factory=dict)  # c -> 2019 share of official FX
    fx_used: dict = field(default_factory=dict)    # (c, t) -> phi applied (filled when solving)

    @property
    def rho(self):
        return (self.sigma_act - 1.0) / self.sigma_act if self.sigma_act else None


def default_fuel():
    """The model's default fuel mechanisms.

    sigma_act = 0.1: short-run substitution between fuel and value added
    (fewer trips, load consolidation, generator rationing), the low end of
    short-run fuel-demand evidence for low-income countries.
    eps_hh = 0.1: households' switching of fuel spending to firewood and
    charcoal, on top of the LES, which already makes household fuel demand
    roughly unit-elastic; household fuel is mostly transport fuel.
    mu = 0.9 (of which mu_abroad = 0.1 transport paid abroad, the rest the
    smugglers' domestic margin), eta = 2: calibrated so that fuel's 2024
    market price is 3.35 times the pump price, the full-year estimate built
    from the black-market prices in `BLACK_MARKET_2024`
    (`black_market_ratio_2024`); informal cost rises with volume.
    With these values the observed 2024 fuel cut (official imports 24 % below
    2019) produces what was seen: a pump shortage deep enough that fuel's
    market price reaches the black-market price and many buyers turn to
    informal channels. With sigma = 0.3 the shortage is absorbed by
    substitution alone and no informal fuel flows (APPENDIX.md, A.6.4).
    fx_quota: the pump quota is BRB's foreign-exchange allocation to fuel over
    the world price, the allocation a constant share of official foreign
    exchange unless set (see the module docstring, channel 4).
    """
    return FuelConfig(sigma_act=0.1, informal=True, mu=0.9, mu_abroad=0.1,
                      eta=2.0, eps_hh=0.1, fx_quota=True,
                      fx_goods=("c-refpet", "c-chemplast"))


# Informal (black-market) and official pump prices of fuel, BIF per litre,
# 2024 (provided by the user, 2026-09-29, from market reports; Sep-Oct
# derived). The pump price was unchanged at 4,000 all year.
BLACK_MARKET_2024 = {
    "2024-07": 17500, "2024-08": 12500, "2024-09": 12000, "2024-10": 12000,
    "2025-01": 22000,           # memo: early 2025
}
PUMP_PRICE_2024 = 4000


def black_market_ratio_2024(first_half="aug-oct"):
    """Annual 2024 black-market price over the pump price, estimated from the
    partial monthly data (the model is annual).

    July-October as observed; November and December interpolated linearly
    between October (12,000) and January 2025 (22,000): 15,333 and 18,667;
    January-June, not observed, set at the stable Aug-Oct level
    (`first_half="aug-oct"`, default -> 3.35) or equal to the second-half
    average (`"h2"` -> 3.67). The Aug-Oct level alone is 3.04.
    """
    oct_, jan = BLACK_MARKET_2024["2024-10"], BLACK_MARKET_2024["2025-01"]
    nov = oct_ + (jan - oct_) / 3
    dec = oct_ + 2 * (jan - oct_) / 3
    h2 = [BLACK_MARKET_2024[m] for m in ("2024-07", "2024-08", "2024-09", "2024-10")] + [nov, dec]
    aug_oct = sum(BLACK_MARKET_2024[m] for m in ("2024-08", "2024-09", "2024-10")) / 3
    h1_level = aug_oct if first_half == "aug-oct" else sum(h2) / 6
    return (6 * h1_level + sum(h2)) / 12 / PUMP_PRICE_2024


# calibration target: full-year estimate (3.35); range 3.04 (Aug-Oct level
# only) to 3.67 (first half like the second)
BLACK_MARKET_RATIO_2024 = black_market_ratio_2024()


def calibrate_fuel(cal, cfg: FuelConfig):
    """Attach the configuration to `cal` with its base-year shares."""
    S = cal.db.sets
    f = cfg.fuel
    FLEO = set(S.get("fleo", []))
    if cfg.sigma_act:
        acts = cfg.acts or tuple(a for a in S["a"] if cal.ica00.get((f, a)))
        AF, sF, pvac = [], {}, {}
        for a in acts:
            if not cal.ica00.get((f, a)):
                continue
            fuel_cost = cal.PQD00[(f, a)] * cal.ica00[(f, a)]
            pva_c = cal.PVA00[a] + sum(cal.WFA00.get((fl, a), 0.0) * cal.ifa0.get((fl, a), 0.0)
                                       for fl in FLEO)
            AF.append(a)
            sF[a] = fuel_cost / (pva_c + fuel_cost)
            pvac[a] = pva_c
        cfg.AF, cfg.sF, cfg.pvac00 = tuple(AF), sF, pvac
    if cfg.informal:
        if f not in cal.QM00 or not cal.QM00[f]:
            raise ValueError(f"informal fuel needs base imports of {f}")
    if cfg.fx_quota:
        ofx00 = official_fx00(cal)
        cfg.fx_share00 = {}
        for c in cfg.fx_goods:
            if c not in cal.db.sets["cmbar"]:
                raise ValueError(f"fx_quota needs {c} to be a quota good (cmbar)")
            cfg.fx_share00[c] = cal.PWM00[c] * cal.QM00[c] / ofx00
            cfg.fx_from.setdefault(c, "2020")
        cfg.fx_used = {}
    cal.fuel = cfg
    cal.inf_active = set()
    return {"fuel mechanisms": {
        "firms: sigma": cfg.sigma_act, "activities": len(cfg.AF),
        "informal supply": (f"mu={cfg.mu} (abroad {cfg.mu_abroad}), eta={cfg.eta}" if cfg.informal else "off"),
        "households: eps": cfg.eps_hh,
        "quotas tied to FX": ({c: f"from {cfg.fx_from[c]}, {cfg.fx_share00[c]:.3f} of official FX (2019)"
                               for c in cfg.fx_goods} if cfg.fx_quota else "off")}}


def informal_gap(cal, V, t):
    """Official market price of fuel minus the landed informal price at zero
    informal volume (border cost plus the same distribution margins). Positive
    means the pump shortage has made fuel scarce enough that buyers turned away
    at the pump pay for informal fuel."""
    cfg = cal.fuel
    f = cfg.fuel
    S = cal.db.sets
    mgs = getattr(cal, "margin_scale", None) or {}
    margins = sum(V["PQD"].get((ct, tm), 0.0) * cal.icm.get((ct, f), 0.0)
                  * mgs.get((ct, f, t), 1.0)
                  for ct in S["ct"] for tm in S["tacm"])
    cost0 = V["PWM"][f] * (1 + cfg.mu) * V["EXR"][t] * V["PREXR"][t]
    return V["PM"][f] - cost0 - margins


def pvac(v, cal, a):
    """Value-added composite unit cost: value added plus the fixed-proportion
    factors, per unit of value-added quantity."""
    FLEO = set(cal.db.sets.get("fleo", []))
    return v("PVA", a) + sum(v("WFA", fl, a) * cal.ifa0.get((fl, a), 0.0) for fl in FLEO
                             if cal.ifa0.get((fl, a)))


# ---- 4. quota tied to official foreign exchange ----------------------------

def official_fx(cal, get, t):
    """Official foreign-exchange inflow, foreign currency: exporters'
    receipts surrendered at the official rate, grants to the government and
    the government's net foreign borrowing. `get(name, *idx)` reads a model
    variable (symbolic in model.py, numeric from a solution dict)."""
    S = cal.db.sets
    return (sum(cal.shroe00[c] * get("PWE", c) * get("QE", c)
                for c in S["c"] if cal.shroe00.get(c) and cal.QE00.get(c))
            + sum(get("TRNSFR", g, r) for g in S["insgov"] for r in S["insrow"]
                  if cal.TRNSFR00.get((g, r)))
            + get("NFFG", t))


def official_fx00(cal):
    S = cal.db.sets
    return (sum(cal.shroe00[c] * cal.PWE00.get(c, 0.0) * cal.QE00[c]
                for c in S["c"] if cal.shroe00.get(c) and cal.QE00.get(c))
            + sum(cal.TRNSFR00.get((g, r), 0.0) for g in S["insgov"] for r in S["insrow"])
            + cal.NFFG00)


def sol_getter(V):
    """`get(name, *idx)` over a solution dict."""
    def get(name, *idx):
        return V[name][idx if len(idx) != 1 else idx[0]]
    return get


def fx_rule_on(cal, t, c=None):
    """Is the ceiling of good c (default: fuel) set by the FX rule in year t?"""
    cfg = getattr(cal, "fuel", None)
    if not (cfg and cfg.fx_quota):
        return False
    c = c or cfg.fuel
    return c in cfg.fx_goods and t >= cfg.fx_from.get(c, "2020")


def fx_share_at(cal, L, t, c=None):
    """phi(c, t): set explicitly in `fx_share`, else carried from the year
    before -- the share applied there if the FX rule set that year's
    ceiling, or the share implied by that year's volume ceiling (BRB's
    observed volume, or the base-year quota)."""
    cfg = cal.fuel
    c = c or cfg.fuel
    if (c, t) in cfg.fx_share:
        phi = cfg.fx_share[(c, t)]
    else:
        i = cal.TSOL.index(t)
        tp = cal.TSOL[i - 1] if i else None
        if tp is not None and fx_rule_on(cal, tp, c) and (c, tp) in cfg.fx_used:
            phi = cfg.fx_used[(c, tp)]
        elif tp is not None and L and "PWM" in L:
            get = sol_getter(L)
            # a scenario's quota multiplier is not part of BRB's allocation
            ceil = (cal.qmbar0[(c, tp)] / getattr(cal, "qmbar_scale", {}).get((c, tp), 1.0)
                    if not fx_rule_on(cal, tp, c) else L["QM"][c])
            phi = ceil * L["PWM"][c] / official_fx(cal, get, tp)
        else:
            phi = cfg.fx_share00[c]
    cfg.fx_used[(c, t)] = phi
    return phi


def quota_ceiling(cal, V, c, t):
    """The import ceiling in effect for (c, t) at solution V (volume)."""
    cfg = getattr(cal, "fuel", None)
    if cfg and fx_rule_on(cal, t, c):
        scale = getattr(cal, "qmbar_scale", {}).get((c, t), 1.0)
        phi = cfg.fx_used.get((c, t), cfg.fx_share00[c])
        return scale * phi * official_fx(cal, sol_getter(V), t) / V["PWM"][c]
    return cal.qmbar0[(c, t)] if (c, t) in cal.qmbar0 else None


def fx_share_implied(cal, V, t, c=None):
    """Official imports of c (default: fuel) as a share of official FX."""
    c = c or cal.fuel.fuel
    return V["PWM"][c] * V["QM"][c] / official_fx(cal, sol_getter(V), t)
