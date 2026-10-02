# The dual exchange-rate market and the fuel quota in the Burundi model

This note documents the two Burundi-specific mechanisms in the GEM-Core
application, as implemented in this Python port (which reproduces the GAMS
model equation by equation). It is written for someone who will run new
simulations, so it states exactly what is modelled, where the numbers come
from, how the closures switch, and what is *not* modelled.

Notation follows GEM-Core: `EXR` is the official exchange rate (BIF per unit
of foreign currency), `PWM`/`PWE` world import/export prices in foreign
currency, `QM`/`QE` import/export quantities, `c` a commodity, `t` a period.
Where a GAMS line is cited it is `model/mod.gms` unless stated.

---

## 1. The dual exchange-rate market

### 1.1 What is modelled

Burundi has an official exchange rate and a parallel (market) rate. The model
represents this with one extra variable, the **premium ratio**

    PREXR(t) = parallel rate / official rate      (PREXR = 1 means unified)

and two commodity-specific data parameters giving the **share of each trade
flow that clears at the official rate**:

| parameter | meaning | Burundi values (`bdi2019-data.xlsx`, sheets `shrom000`, `shroe000`) |
| --- | --- | --- |
| `shrom(c)` | share of imports of `c` bought with foreign exchange at the official rate | 1.0 for refined petroleum (`c-refpet`) and chemicals/plastics (`c-chemplast`); **0 for every other commodity** (all their imports are paid at the parallel rate) |
| `shroe(c)` | share of export receipts of `c` surrendered at the official rate | 0.75 for agriculture (`c-agr`, i.e. coffee and tea: a quarter of receipts is sold on the parallel market); 1.0 for every other export |

So in the data, the official rate is reserved for fuel and chemicals imports
and for most export surrender; everything else transacts at the parallel rate.

### 1.2 Prices

The effective exchange rate for a commodity is a blend of the two rates, and
enters the import and export price equations (`EQ_PMDEF`, `EQ_PEDEF`;
`gemcore/model.py` around lines 412-428):

    PM(c) = (1 + TM(c) + PRQMBAR(c)) * PWM(c) * [ (1-shrom(c))*EXR*PREXR + shrom(c)*EXR ] + trade margins
    PE(c) = (1 - TE(c))              * PWE(c) * [ (1-shroe(c))*EXR*PREXR + shroe(c)*EXR ] - trade margins

`PRQMBAR(c)` is the import-quota rent rate of section 2 (zero for commodities
without a quota). A higher premium therefore raises the domestic price of
every import bought at the parallel rate and raises the BIF receipts of the
exporters who sell foreign exchange on the parallel market.

### 1.3 The premium rent

The gap between the two rates is a transfer. Per commodity, the model computes
the rent (`EQ_FOREXRENT`, `model.py` ~399):

    YPREXRT(c) = (1-shrom(c)) * (PREXR-1) * EXR * PWM(c) * QM(c)
               - (1-shroe(c)) * (PREXR-1) * EXR * PWE(c) * QE(c)

The first term is what importers pay above the official rate; the second is
what parallel-market exporters receive above it. The net rent is allocated to
accounts by fixed shares `shryprexr(c,ac)` (`EQ_FOREXRENTALLOC`). **In the
Burundi data the whole rent accrues to the private capital factor
(`f-capprv`)** — `bdi2019-data2.inc` line 199, `SAM('f-capprv','prexr') =
SUM(c, SAM('prexr',c))` — and reaches households through their capital income
shares. There is no separate "rent-seeker" institution.

### 1.4 How the base-year SAM absorbs the premium

The SAM is at official-rate values. `bdi2019-data2.inc` (ported in
`gemcore/database.py::bdi2019_data2`, lines ~375-425) adds a `prexr` account:

1. `SAM('prexr',c) = (1-shrom)(prexr000-1)·SAM('row',c) − (1-shroe)(prexr000-1)·SAM(c,'row')`
   — the base-year rent per commodity, from the base-year ratio `prexr000`;
2. the rent is paid by domestic demanders of `c` in proportion to their base
   use (households, government, investment, stocks), which raises the
   domestic value of those purchases;
3. the rent income is booked to `f-capprv`, and household and government rows
   are rebalanced through that factor.

The SAM stays balanced to machine precision after the adjustment
(`validation/check_baseyear_residuals.py` verifies the model reproduces it).

**The base-year ratio.** `bdi2019-data2.inc` line 188 hardcodes
`prexr000 /2.5/`. This is a *ratio* (parallel = 2.5 × official), as the
authors' Figure 4.3 labels it — but 2.5 is the late-2024/2025 value. The
actual 2019 ratio was about 1.583 (World Bank / IMF parallel-rate series,
see `data/burundi-actuals-2019-2024-FINAL.xlsx`, row IN02). The port keeps
the authors' 2.5 as the default so the reference application reproduces, and lets
you override it:

```python
db = load_database(DATA, data2_hook=bdi2019_data2, overrides={"prexr000": {(): 1.583}})
```

The backcast runner (`runs/backcast.py --prexr`) does exactly this.

### 1.5 The premium over time: closure switch `rowclos`

Whether the premium is an input or an outcome is set per period by the
rest-of-world closure `rowclos0(t)` (sheet `rowclos0`; replayed in
`gemcore/solver.py::base_closure_fixed`, ~lines 204-221):

| `rowclos` | real exchange rate `REXR` | premium `PREXR` | what clears the external balance |
| --- | --- | --- | --- |
| 1 | endogenous | fixed at `PREXR0(t)` | the real exchange rate |
| 2 | fixed | fixed | non-government net foreign financing (`NFFINS`) |
| 3 | fixed | fixed | government net foreign financing |
| 4 | fixed | **endogenous** | the premium |

With the numeraire on the domestic producer price index (`numeraire0 = 2`),
the official rate `EXR` is always `REXR × DPI` (`EQ_REXRDEF`), so "REXR fixed"
means the official rate moves only with domestic prices.

The Burundi base path uses **`rowclos = 2` for 2019-2027 and `rowclos = 4`
from 2028**. Read: the premium is held at its 2019 level (`PREXR0(t) =
PREXR00 × prexrindex0(t)`, index flat at 1) while foreign financing absorbs
the balance-of-payments gap through 2027; from 2028 foreign financing is on
its exogenous path and the premium is what balances the market — it rises
endogenously from 2.5 over 2028-40.

The exogenous premium path is data: sheet `prexrindex0` (empty in the
authors' workbook, so the index is 1 throughout). The backcast feeds the
actual 2020-2024 parallel-rate ratios through this index.

### 1.6 The unification scenario `uni`

`bdi2019-sim2.inc` lines 497-503; `gemcore/scenarios.py::uni`. For every
period the scenario:

- sets `rowclos = 1` (real exchange rate flexes, premium is an input);
- halves the premium in 2026 and removes it from 2027:
  `PREXR = 1 + (PREXR0-1)×k` with `k = 0.5` in 2026 and `k = 0` after, coded
  as a ratio on `PREXR0` so it lands there whatever the reference premium is;
- multiplies the import quota ceilings by 45 from 2026 (section 2.4).

Unification removes the rent to private capital, lowers import prices for
parallel-rate imports, and lowers BIF receipts for parallel-market exporters
(coffee/tea); the real exchange rate then adjusts to clear the external
balance at the exogenous foreign-financing path.

### 1.7 What is not modelled

- No demand for foreign exchange as an asset, no reserve dynamics tied to the
  premium, no expectation channel. The premium is a price wedge with a rent.
- The official-rate shares `shrom`/`shroe` are constant parameters. A
  scenario that moves rationing between the two markets must change them
  (they are `cal.shrom00[c]`, `cal.shroe00[c]` after calibration).
- The rent recipient is whatever the SAM adjustment says (private capital
  here). Redirecting it (to government, say) means changing
  `bdi2019_data2` in `gemcore/database.py` where the `prexr` column is
  booked, then re-running the base-year check.
- The split itself is right. BRB reports that about 70 % of imports are
  priced at the parallel rate and 30 % at the official rate; the SAM has
  71.4 % and 28.6 %.

### 1.8 Pass-through of the premium to import prices (sensitivity)

GEM-Core passes every change in the premium fully into the
prices of imports bought at the parallel rate. A port-only parameter lets a
share θ through instead: `cal.prexr_pt = θ`, with `cal.prexr_pt_anchor` the
path the change is measured from (the reference premium for a scenario, the
2019 level for a backcast). It applies to the import price, the premium rent
and the tariff base together; exports keep the full premium. Unset, the model
is exactly GEM-Core.

This matters because the unification result is almost entirely this channel:

| `uni`, increment over base, 2026–40 | θ = 1 | 0.5 | 0 |
| --- | ---: | ---: | ---: |
| GDP at factor cost, pp/yr | +1.02 | +0.39 | +0.01 |
| exports, pp/yr | +3.93 | +0.71 | −0.75 |

The 2019–24 backcast does not pin θ, but the 2024 episode (premium up
from 1.60 to 2.17, imports down 10 %) is reproduced only near θ = 1. Run
`python3 runs/uni_passthrough.py` for the full table (any scenario with
`--scenario`), and `python3 runs/backcast.py --passthrough 0.5` to test a
value against 2020–24.

### 1.9 The premium rent as a real cost (added 2026-09-30)

**What it adds.** In GEM-Core the premium rent is a pure transfer to private
capital. With pass-through off (θ = 0), unification therefore only moves
income around. Following Krueger (1974), a share ω of the rent is treated as
a real resource cost instead: queuing for official foreign exchange, informal
intermediation and lobbying use up resources that produce nothing.

**How it enters.** In reduced form, productivity (`EQ_TFPDEF`) is multiplied
by

    RW = (1 − ω · RSH_eff) / (1 − ω · RSH_2019)
    RSH_eff(t) = (1 − λ) RSH_eff(t−1) + λ · Σc YPREXRT(c) / GDPMP

`RW` is 1 in 2019, so the base year is exact. The code is `model.py`
(`rent_eff_prev`) and `calibration.py` (`RENT_COST_DEFAULT = 0.25`,
`RENT_ADJUST_DEFAULT = 1/3`). `calibrate(db, rent_cost=...)` sets ω, and the
replication mode sets ω to 0.

**Size.** At the authors' premium of 2.5, the rent is 17 % of GDP in 2019
and 21 % by 2040 on the reference path. So ω = 0.25 puts resources worth
about 4–5 % of GDP into rent-seeking. Krueger estimated licence rents of
about 7 % of national income for India and 15 % of GNP for Turkey. With
λ = 1/3, most of the adjustment happens within three years.

**Effect.** It raises the unification gain from +1.05 to +1.49 points a
year (ω = 0.5: +1.95). GDP in 2040 is then 23 % above base instead of 16 %
(32 % at ω = 0.5). The GAMS-replication table and the backcast are
unchanged; the backcast pins GDP. `runs/reform_boost.py` runs the ω grid
and writes `reports/reform-boost-2026-2040.md`.

**Budget support** (`scenarios.uni_bs`, `uni_bs_inv`) is added through a
multiplier on the grant path, `Scenario.trnsfr_ratio`, applied in
`Closure.trnsfrb`. Grants of 1 % of GDP in 2026–27 and 0.5 % in 2028 that
lower the direct tax cushion consumption in 2027 (+2.5 points over `uni`)
but leave 2040 unchanged. Spent on public investment at the `uni+inf`
return, they add 1.4 points to 2040 GDP.

### 1.10 Scenario hooks for what reforms build (added 2026-09-30)

These are not exchange-rate mechanisms. They are recorded here because they
share the scenario machinery, and all are inert (factor 1, zero) unless a
scenario sets them.

- **Input coefficients** (`Scenario.ica_scale`, `(c, a, t) → factor`;
  `model.py` helper `ica`). Used for hydropower, which halves fuel use in
  electricity, and for roads, which cut transport inputs by 15 %.
- **Trade and transport margins** (`Scenario.margin_scale`,
  `(ct, c, t) → factor` on icm/ice/icd; helpers `icm`, `ice`, `icd`, also in
  `fuel.informal_gap`). Transport margins fall 15 %; marketing margins on
  farm and food products fall 10 %.
- **Targeted FDI** (`Scenario.fdi_add`, `t → foreign currency`, plus
  `fdi_target`). FDI rises by the amount in `INVVALFDEF`, and the capital it
  buys goes to one activity through the extra term T in `EQ_NEWCAPALLOC`.
  It is foreign-owned through `CAPACCUMNGOVFOR`. Used for mining in
  `combi-x`.

The scenarios are `uni+inf-m`, `uni+inf-x`, `uni+inf+hd-x` and `combi-x`
(`gemcore/scenarios.py`). Infrastructure effects phase in with the public
capital stock (`INFRA_EFFECT`, full from 2035). The education mix
(`EDU_SHIFT_2040`) moves 2 % of each sex's labour force from primary to
secondary education by 2040, through `qfinsb_ratio`, with totals unchanged.
Results are in `reports/reform-boost-2026-2040.md`.

---

## 2. The fuel shortage: rationed pump fuel, substitution, and informal supply

### 2.1 What is modelled

GEM-Core itself has only an **import quota** on two commodities, refined
petroleum (`c-refpet`) and chemicals/plastics (`c-chemplast`), the set
`cmbar`. The Burundi model adds four channels (2026-09-29; `gemcore/fuel.py`,
sections 2.6 and 2.7), part of the model by default, so that a fuel shortage
works the way it did in Burundi:

- the quota on refined petroleum is **the pump shortage**: official fuel
  imports are limited by BRB's allocation of official-rate foreign exchange,
  and the pump price stays at the official level. The ceiling itself **follows
  foreign-exchange availability** (section 2.7): it is BRB's allocation to
  fuel, a share of official foreign-exchange inflows, over the world price;
- **firms economise on fuel** at a cost in value added;
- **households switch part of their fuel spending** to firewood and charcoal;
- once the shortage is deep enough, **informal fuel from the DRC and
  Tanzania** flows, always dearer than the pump because it is bought with
  parallel-market foreign exchange and carries a transport cost.

Both quota commodities are also the two whose imports clear entirely at the
official rate (`shrom = 1`, section 1.1). So fuel in this model is: bought
with official-rate foreign exchange, in a quantity capped by the quota.

### 2.2 The quota

For `c` in `cmbar` the model imposes (`EQ_QMCONST`, mod.gms 2608,
complementarity 3533):

    qmbar(c,t)  ≥  QM(c,t)      ⊥      PRQMBAR(c,t) ≥ 0

i.e. either the quota **binds** (`QM = qmbar`) with a positive rent rate
`PRQMBAR`, or it is **slack** (`QM < qmbar`) with zero rent. `PRQMBAR` enters
the import price like an ad-valorem tariff (section 1.2), so a binding quota
raises the domestic price of fuel until demand equals the ceiling.

The ceiling path is data: `qmbar(c,t) = QM00(c) × qmbarindex0(c,t)` (sheet
`qmbarindex0`). In the Burundi workbook the index is 1.0 for 2019-2025 —
fuel imports frozen at their 2019 volume — and then grows about 2.2 % a year
from 2026 (1.022 in 2026, 1.39 by 2040), i.e. with the base GDP growth
assumption, so the shortage neither eases nor tightens relative to the
economy in the base run. **This path is now used only in the
GAMS-replication mode:** with the fuel channels on, the fuel ceiling and, since
2026-09-30, the chemicals and fertiliser ceiling are set by foreign exchange
from 2020 (section 2.7).

**The frozen 2020–25 ceilings do not match what happened.** BRB's customs
tonnages (`gemcore/brb.py`) show fuel imports +22 % by 2022, then −24 % by
2024 (the fuel crisis), and fertiliser plus chemicals up about 85 %. The
backcast replaces the frozen path with these observed volumes
(`runs/backcast.py --quotas brb`, default).

Without the channels of section 2.6, the 28 % cut in official fuel in 2024
does not solve: fuel enters production in fixed proportions and the quota
rent explodes. With them it does (section 2.6).

**The base-year rent is zero.** `bdi2019-data2.inc` sets `prqmbar000 /0.25/`
(line 115) and would add a 25 % rent to the SAM as a `prqmbar` account (lines
119-139), but the whole block sits between `$ONTEXT` (line 91) and `$OFFTEXT`
(line 149), so it is commented out and the SAM's `prqmbar` row is empty
(corrected 2026-09-30; this paragraph earlier said the 25 % rent applied). The
base year sits exactly at the ceiling with a zero rent. Any later rent is
generated by the model as demand outgrows the ceiling (appendix A.3.6).

### 2.3 The rent and who gets it

The rent income is (`EQ_IMPQUOTARENT`):

    YPRQMBART(c) = PRQMBAR(c) × PWM(c) × EXR × QM(c)

and is distributed to the **users of the composite commodity** — households,
activities, government, and investment — in proportion to their share of
domestic demand net of trade margins (`EQ_HHDIMPQUOTARENT`,
`EQ_ACTIMPQUOTARENT`, `EQ_GOVIMPQUOTARENT`, `EQ_INVIMPQUOTARENT`;
`model.py` ~380-395). In words: the licence to import at the official rate
is worth the rent, and whoever uses the fuel captures it. There are no
deadweight losses from the shortage beyond the price effect on demand.

Since intermediate demand is Leontief (`EQ_INTDEM`, fixed coefficients
`ica`), activities cannot substitute away from fuel; a tighter quota raises
their unit cost one-for-one with the fuel price and squeezes value added.
Households substitute through the LES demand system. Nothing else in the
economy "runs out" — the model rations by price.

### 2.4 Relaxing or tightening the quota

- **Path**: change `cal.qmbar0[(c, t)]` after calibration (a `Scenario` does
  it with `qmbar_ratio={(c, t): r}` — a multiplier on the reference ceiling,
  as `par-defn-sim.inc` does with `qmbarsim`).
- **Removal**: `uni` multiplies the ceilings by 45 from 2026. In GAMS the
  complementarity then moves to the slack branch on its own. In this port,
  which solves equalities, the scenario must also declare the cells slack
  (`quota_slack={(c, t), ...}`), which drops `EQ_QMCONST` for those cells
  and pins the rent at zero; `gemcore/complementarity.py::sweep_solve` then
  checks the branch after every solve — a cell with a **negative** rent is
  moved to slack, a slack cell importing **above** its ceiling is moved back
  to binding — and re-solves until every cell is consistent. Every runner
  goes through this sweep, so you do not normally need to think about it;
  `sweep_report(cal, periods)` lists which cells ended where.
- **Tightening** (a fuel-shortage scenario): a `qmbar_ratio` below one on
  `c-refpet`. The quota stays binding, the rent rises, and the domestic fuel
  price does the rationing. Check `PRQMBAR("c-refpet")` in the solution to
  read the implied tariff-equivalent of the shortage.

### 2.5 What the backcast found

With actual 2020-2024 inputs, both quotas go **slack in 2020** (the COVID
year: demand fell below the frozen ceilings) and chemicals stays slack
through 2021, then both re-bind. This came out of the sweep, not by
assumption, and it is the only reason the 2020 current account reproduces
the GAMS reference. See appendix A.6.3.

---

### 2.6 The three fuel channels (added 2026-09-29)

All three are in `gemcore/fuel.py` and `model.py`. They are exactly inert at
base prices, so the base year stays exact. They are on by default
(`fuel.default_fuel`); `calibrate(db, fuel_mech=None)`, or `--gams-replication`
on the runners, removes them only to re-check the port against the reference
application.

**Firms: fuel against value added** (`sigma_act = 0.1`). For every activity
that uses fuel, fuel and value added (including the fixed-proportion mining
resource) form a CES. Two indices, `FXI` (fuel per unit of output) and `VXI`
(value added per unit of output), both 1 at base prices, satisfy

    FXI / VXI = [ (P_fuel / P_fuel,2019) / (P_VA / P_VA,2019) ]^(-sigma)
    (1 - sF) VXI^rho + sF FXI^rho = 1,   rho = (sigma - 1) / sigma

with sF the 2019 fuel share of value added plus fuel: 0.45 for electricity,
0.31 for transport, 0.20 for construction. Saving fuel costs value added per
unit of output.

**Informal fuel** (`mu = 0.9`, of which `mu_abroad = 0.1`; `eta = 2`). A
second supply, `QMI`, outside the quota, at a landed cost

    PWM (1 + mu) EXR PREXR (1 + QMI / QM2019)^(1 / eta)  +  distribution margins

bought with parallel-market foreign exchange at the parallel rate. The
markup mu covers transport through the DRC and Tanzania (`mu_abroad`, paid
abroad) and the smugglers' margin (the rest, domestic income), and the cost
rises with volume. mu is calibrated to the black-market prices below: it
puts fuel's 2024 market price at 3.35 times the pump price. It flows only when
the pump shortage has pushed fuel's scarcity value (pump price plus quota
rent) up to that cost:

    QMI >= 0   ⊥   market price of fuel <= informal cost

Like the quotas, this complementarity is swept (`complementarity.sweep_solve`).
Buyers who get pump fuel pay the pump price and keep the quota rent. Only
PWM (1 + mu_abroad) is paid abroad. The parallel premium, the smugglers'
margin and the rising-cost markup are domestic rents, booked to private
capital. Informal fuel is not in recorded imports.

**Black-market prices, 2024** (BIF per litre, from market reports provided
by the user; `fuel.BLACK_MARKET_2024`):

| | Jul 2024 | Aug | Sep | Oct | Jan 2025 |
| --- | ---: | ---: | ---: | ---: | ---: |
| pump price (official, unchanged) | 4,000 | 4,000 | 4,000 | 4,000 | 4,000 |
| black-market price | 17,500 | 12,500 | ~12,000 | ~12,000 | 22,000 |
| ratio | 4.4 | 3.1 | 3.0 | 3.0 | 5.5 |

The model is annual, so the target is a full-year 2024 estimate,
`fuel.black_market_ratio_2024()`. July–October are used as observed.
November and December are interpolated linearly between October and January
2025, giving about 15,300 and 18,700 BIF a litre. January–June, which are not
observed, are set at the stable August–October level. The result is **3.35
times the pump price**. The plausible range is 3.04 (August–October level
only) to 3.67 (first half equal to the second half). The first-half
assumption carries the most weight, so any first-half price observation would
tighten it most.

**Households: fuel to firewood and charcoal** (`eps_hh = 0.1`). The share of
each household's fuel budget spent on fuel falls with fuel's price relative
to farm and forest products (`c-agr`, which holds firewood and charcoal):

    HXI = [ (P_fuel,h / P_fuel,h,2019) / (P_agr,h / P_agr,h,2019) ]^(-eps)

The remainder, `(1 - HXI)`, is spent on `c-agr`. The household budget is
unchanged.

**Calibration to 2024.** With the observed 2024 fuel cut imposed (official
imports 24 % below 2019), fuel's market price reaches the black-market price,
3.34 times the pump price against 3.35 estimated. Informal fuel supplies 6.1 %
of fuel used. The black-market price pins mu. The volume of informal fuel is
not observed, so σ and ε are set at the low end of short-run evidence. With
σ = 0.3 the shortage is absorbed by substitution alone and no informal fuel
flows, which does not fit the reports that many turned to informal channels.

| 2024, observed fuel cut, mu = 0.65 | σ = 0.03, ε = 0 | 0.05, 0.05 | 0.1, 0.1 |
| --- | ---: | ---: | ---: |
| informal fuel, % of 2019 volume | 14.8 | 11.5 | 6.5 |
| fuel market price / pump price | 3.05 | 3.00 | 2.94 |

With σ = 0.1 and ε = 0.1, the market price rises about 1.6 times for each
unit of mu: 2.94 at mu = 0.65, 3.26 at 0.85, 3.34 at 0.9 (the default,
informal fuel 4.9 % of 2019 volume, 6.1 % of fuel used) and 3.43 at 0.95.
Matching 3.67 would need mu of about 1.1.

**Effect on the scenarios.** Small: 2026–40 growth changes by at most 0.06
points a year, and the unification gain is unchanged (GDP +1.02 points a
year with or without the channels). The channels matter for fuel shocks, not
for the paper's scenarios, where the quotas are slack or removed.

### 2.7 The fuel quota follows foreign-exchange availability (added 2026-09-29)

The pump shortage is caused by the shortage of official foreign exchange.
The fuel ceiling is therefore no longer an exogenous volume. It is BRB's
allocation of official foreign exchange to fuel, divided by the world price
of fuel (`QMCONST` for `c-refpet`; `fuel.official_fx`, `fuel.fx_share_at`):

    QM_fuel  <=  phi(t) × OFX(t) / PWM_fuel(t)

    OFX = Σc shroe(c) PWE(c) QE(c)     exporters' receipts surrendered at the official rate
        + TRNSFR(gov, row)              grants to the government
        + NFFG                          government net foreign borrowing

All values are in US dollars. In 2019, OFX is 70.4 (model units): 50.3 from
exports, 11.9 from grants and 8.1 from borrowing. Fuel imports of 25.0 are
**35.5 %** of it, and that is `phi` in 2019. Two things now deepen the pump
shortage without any change to the quota:

- a fall in export receipts, aid or government borrowing;
- a rise in the world price of fuel.

In both cases the rent rises, and past the informal threshold (section 2.6)
informal fuel flows. The complementarity is unchanged: when demand at the pump
price is below the allocation the quota is slack, and the sweep finds it.

**phi over time.** Where phi is not set (`FuelConfig.fx_share`), it is
carried from the year before. A year on the foreign-exchange rule passes on
the share it used. A year with an observed volume ceiling passes on the share
that volume implies. With no observation, phi stays at its 2019 value.
Scenario multipliers on the ceiling (`qmbar_ratio`, e.g. `uni`'s ×45) apply to
the foreign-exchange ceiling too, through `cal.qmbar_scale`.

**Base run.** The rule applies from 2020. Official foreign exchange grows
about 2.5 % a year, against the workbook's ceiling path (frozen to 2025, then
2.2 % a year), so the ceiling does too. The fuel rent now peaks at 0.35 in 2027
instead of building up to 0.94 by 2025, and the quota goes slack from 2037.

GDP is pinned in the base, so the looser ceiling changes composition:

- 2040 fuel imports are 9 % higher, paid for by 1 % more exports;
- 2040 investment is 1 % lower;
- the consumption gain comes earlier, with 2025 private consumption 0.4 %
  higher and 2040 about the same.

The effect on 2026–40 growth:

| | Absorption | PrvCon | Investment | Exports | Imports |
| --- | ---: | ---: | ---: | ---: | ---: |
| change, pp a year | −0.03 | −0.03 | −0.08 | +0.07 | −0.05 |

The unification gain in GDP is +1.05 points a year, against +1.02 before
The GAMS-replication table is unchanged.

**Backcast** (`runs/backcast.py`). By default, 2020–24 keep BRB's observed
fuel volumes. The report now shows the share of official foreign exchange
those volumes took, and the rule takes over after `--quota-through`:

| | 2019 | 2020 | 2021 | 2022 | 2023 | 2024 |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| official FX inflow, USD, 2019 = 100 | 100 | 82 | 89 | 104 | 120 | 107 |
| of which exports | 72 | 62 | 68 | 79 | 89 | 77 |
| world fuel price, USD (IN05), 2019 = 1 | 1.00 | 0.83 | 0.96 | 1.51 | 1.80 | 1.76 |
| fuel share of official FX, % | 35.5 | 37.0 | 38.8 | 48.8 | 49.7 | 43.9 |

What the table shows:

- The world price of fuel rose about 80 % by 2023, while official foreign
  exchange rose only 20 %.
- BRB nonetheless raised official fuel volumes in 2022–23 (+22 % in 2022). It
  did so by giving fuel nearly half of official foreign exchange, up from about
  a third.
- In 2024 export receipts fell back, and the share fell to 44 %. Official fuel
  fell 24 %, and fuel's market price reached 3.3 times the pump price.

`--fuel-quota fx` (report `-fuel-fx`) asks what foreign-exchange availability
alone would have done, with phi held at its 2019 share of 35.5 % from 2020:

| `--fuel-quota fx` | 2020 | 2021 | 2022 | 2023 | 2024 |
| --- | ---: | ---: | ---: | ---: | ---: |
| fuel market price / pump price | 1.20 | 1.45 | 2.60 | 2.57 | 3.54 |
| informal share of fuel used, % | 0 | 0 | 12.8 | 15.0 | 22.7 |

At a constant share, the shortage would have started in 2020 and become a
crisis in 2022, when the world price jumped. That this did not happen is
BRB's reallocation of foreign exchange to fuel. The 2024 crisis is what
remained once the reallocation could no longer be sustained. The model's
2024 price at a constant share is 3.54 times the pump price, a little above
the 3.35 observed.

**Chemicals and fertiliser (added 2026-09-30).** These are the other imports
cleared at the official rate. Together with fuel they make up 28.6 % of
imports in the model, against BRB's figure of about 30 %. They follow the same
rule, with their own share of the same pool, 0.402 in 2019
(`FuelConfig.fx_goods`; `fx_share00`, `fx_used` and `fx_from` are keyed by
good).

In the backcast, BRB's volumes imply a chemicals share of 49 %, 45 %, 44 %,
37 % and 45 % over 2020–24. `--chem-quota fx` (report `-chem-fx`) holds the
2019 share instead, which rations chemicals more and fits imports worse. The
effect on scenarios is negligible: the unification gain goes from +1.48 to
+1.49 points a year, because the chemicals quota barely binds on the
reference path.

Quantity rationing on the parallel market, where 71 % of imports are bought,
is not modelled. Identifying it needs 2019–24 data on private external
financing and BRB foreign-exchange sales.

**What is not in it.** The pieces BRB decides on are not modelled:

- the drawdown of reserves;
- the private sector's own official-rate purchases;
- the share of grants and loans tied to projects, which cannot be used for
  fuel.

Here phi absorbs them. Data on BRB's foreign-exchange sales by use (fuel,
medicines, fertiliser) would let phi be observed rather than implied.

## 3. Where to look in the code

| what | file |
| --- | --- |
| price equations with the blended rate and quota rent | `gemcore/model.py`, `PMDEF`/`PEDEF` |
| premium rent and allocation | `gemcore/model.py`, `FOREXRENT`, `FOREXRENTALLOC` |
| quota, rent, rent allocation | `gemcore/model.py`, `QMCONST`, `IMPQUOTARENT`, `*IMPQUOTARENT` |
| base-year SAM adjustment for both rents | `gemcore/database.py::bdi2019_data2` (`prqmbar` and `prexr` accounts) |
| calibration of `PREXR00`, `shrom00/shroe00`, `qmbar0` | `gemcore/calibration.py` |
| `rowclos` closure switch | `gemcore/solver.py::base_closure_fixed` |
| complementarity sweep | `gemcore/complementarity.py` |
| fuel channels (firm substitution, informal fuel, household switching) | `gemcore/fuel.py`; equations `FUELSUB`, `FUELCES`, `INFARB`, `HHFUELSUB` in `gemcore/model.py` |
| fuel quota set by foreign exchange | `gemcore/fuel.py` (`official_fx`, `fx_share_at`, `quota_ceiling`); `QMCONST` in `gemcore/model.py`; `backcast.set_quota_path` |
| scenario switches (`prexr_ratio`, `qmbar_ratio`, `quota_slack`, `rowclos`, `trnsfr_ratio`) | `gemcore/scenarios.py::Scenario`, `uni`, `uni_bs`, `uni_bs_inv` |
| premium rent as a real cost | `gemcore/model.py` (`EQ_TFPDEF`, `rent_eff_prev`); `gemcore/calibration.py` (`RENT_COST_DEFAULT`) |
| GAMS original | `model/mod.gms` (equations), `model/macclos.inc` (closures), `model/user-files/bdi2019/bdi2019-data2.inc` (SAM adjustments, `prexr000`, `prqmbar000`), `bdi2019-sim2.inc` (scenarios) |
