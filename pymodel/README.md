# Burundi 2019 CGE model — Python port of GEM-Core

A Python implementation of the GEM-Core recursive-dynamic CGE model (Lofgren &
Cicowiez) calibrated to a 2019 social accounting matrix for Burundi. It
reproduces the GAMS model equation for equation, solves the same base run and
scenarios without GAMS, and adds a 2019-2024 backcast against Burundi's actual
outcomes.

Author: Kodzovi Senu Abalo, Senior Economist, Fiscal Policy and Growth Global
Department, World Bank. *The findings, interpretations and conclusions expressed
here are those of the author and do not necessarily represent the views of the
World Bank, its Executive Directors or the governments they represent.*

**Not a Python user? Read `USER-GUIDE.docx` one level up.** It covers installing,
re-running everything with one double-click and checking the findings.

**Reproduce and check everything in one command** (2026-10-01):

```bash
python3 runs/reproduce_all.py            # about 15-30 min; --quick for the core checks; --workers N
```

This runs:

- the base-year check;
- the headline scenarios;
- the pass-through grid and the reform scenarios at ω = 0, 0.25 and 0.5;
- 12 backcast variants;
- an independent solution audit (`validation/check_solutions.py`).

It then compares about 50 documented findings (`runs/expected_results.json`, each with a tolerance) and
writes `reports/REPRODUCTION-CHECK.md`. On 2026-10-01 all 49 reproduced, and both audits passed:
largest residual 3e-9, Walras slack below 4e-9, every complementarity on a consistent branch.

The technical appendix (`APPENDIX.md`, `.docx`, `.pdf`, one level up) documents the data, equations, closures, validation and results.

## 1. Install

Python 3.11 or later (developed on 3.13). Packages:

```bash
python3 -m pip install -r requirements.txt
```

That installs `numpy`, `scipy`, `casadi` (automatic Jacobians), `openpyxl`
(reads the data workbook), `xlrd` (reads INSBU's .xls files for the backcast) and
`python-docx` (only for the memo tool). The share package also has one-click
`setup_*` and `run_*` scripts for macOS and Windows. Nothing
else: no GAMS, no PATH solver, no compiled code. Runs on macOS, Linux and
Windows; the paths below use `/`.

## 2. Layout

The data workbook must sit at `../model/user-files/bdi2019/bdi2019-data.xlsx`
relative to this folder — the runners resolve it from their own location, so
keep the two folders side by side:

```
<root>/
├── model/
│   ├── user-files/bdi2019/
│   │   ├── bdi2019-data.xlsx   THE CALIBRATION DATA: SAM, sets, elasticities, closures, growth paths
│   │   ├── bdi2019-data2.inc   SAM adjustments for the FX premium and the import quota (GAMS; ported)
│   │   └── bdi2019-sim2.inc    scenario definitions (GAMS; ported)
│   └── *.gms, *.inc            GEM-Core GAMS source (reference only; not needed to run)
└── pymodel/                    this folder
    ├── gemcore/                the model
    │   ├── database.py         reads the workbook; applies the data2.inc SAM adjustments
    │   ├── calibration.py      base-year calibration -> all parameters
    │   ├── model.py            every equation, as a residual (cites mod.gms line numbers)
    │   ├── state.py            variable levels per period; Closure accessors for exogenous paths
    │   ├── closure.py          declared equation -> variable pairing
    │   ├── partition.py        checks the system is square (bipartite matching on the Jacobian)
    │   ├── solver.py           closure rules (macclos.inc, facclos.inc) + Newton solver
    │   ├── dynamics.py         period loop, the two base passes, rebasing (par-redefn-0.inc)
    │   ├── complementarity.py  the sweep that enforces the quota and idle-resource inequalities
    │   ├── scenarios.py        Scenario dataclass + the paper's scenarios
    │   ├── actuals.py          reads data/burundi-actuals-2019-2024-FINAL.xlsx
    │   └── backcast.py         feeds actual 2020-24 inputs; model-vs-actual comparison
    ├── runs/                   entry points (section 3)
    ├── validation/             transcription and squareness checks
    ├── docs/MECHANISMS.md      the dual exchange rate and the fuel quota, as modelled
    ├── data/                   the 2019-2024 actuals workbook (filled) and its empty template
    ├── reports/                results (section 5)
    └── requirements.txt
```

## 3. Run

All commands from this folder. Times are for a 2020 laptop.

**Check the transcription first** (10 s). Evaluates every equation at the
calibrated 2019 point, where the SAM guarantees a solution; any non-zero
residual is a transcription error.

```bash
python3 validation/check_baseyear_residuals.py
```

Expect `GLOBAL max |residual| = 4.5e-13` and `RESULT: PASS`.

**Base run and one scenario** (about 70 s for the base, 2-3 min with a
scenario). Prints the per-period solve log and the 2026-2040 growth table.

```bash
python3 runs/run.py                # base only
python3 runs/run.py uni            # base + a scenario: uni, uni+inf, uni+inf+hd, combi
python3 runs/run.py combi --quiet --to 2035
```

**All validated scenarios** (about 4 min). Writes
`reports/macro-growth-2026-2040.md` and `.csv`. This is the command to re-run
after any change to the model.

```bash
python3 runs/all_scenarios.py
```

**Full solutions** (about 5 min). Writes every variable of every solved
year for the base run and each scenario to `reports/solutions/<run>.csv`
(long format: variable, index, year, value), plus `macro-levels.csv` and the
complementarity branches each scenario ended on.

```bash
python3 runs/dump.py               # or: python3 runs/dump.py uni combi
```

**Backcast 2019-2024** (about 30 s). Overrides the base-year premium with the
actual 2019 value, feeds the actual 2020-2024 exogenous paths from the
actuals workbook, solves the calibration pass and compares shares, growth
rates and indices with the actuals. Writes `reports/backcast-2019-2024[-tag].md/.csv`.

```bash
python3 runs/backcast.py                          # defaults below
python3 runs/backcast.py --premium flat --tag premium-flat   # premium held at 2019 (sensitivity)
python3 runs/backcast.py --govcon insbu --tag govcon-share   # gov consumption as a GDP share
python3 runs/backcast.py --stocks model --tag stocks-model   # inventories grow with GDP
python3 runs/backcast.py --prexr 2.5 --tag authors           # the authors' base-year premium
python3 runs/backcast.py --fuel-quota fx --tag fuel-fx       # fuel set by FX availability alone (2019 share)
```

**Reform benefit with the rent cost and budget support** (about 5 min per ω).
A share ω of the premium rent is a real cost of rent-seeking (default 0.25; `docs/MECHANISMS.md` 1.9).
`uni+bs` and `uni+bs-inv` add budget support. The second group adds `uni+inf-m` and `uni+inf-x` (transport, marketing, hydropower), `uni+inf+hd-x` (education mix) and `combi-x` (mining FDI); see `docs/MECHANISMS.md` 1.10.
Writes `reports/reform-boost-2026-2040.md`, with GDP at factor cost and at market prices.

```bash
python3 runs/reform_boost.py --rent-cost 0
python3 runs/reform_boost.py --rent-cost 0.25
python3 runs/reform_boost.py --rent-cost 0.5
python3 runs/reform_boost.py --collect
python3 runs/backcast.py --help                   # every switch
```

**Since 2026-09-29 the backcast runs on INSBU's rebased national accounts**
(supply-use tables 2016–24 and integrated accounts), read by
`gemcore/insbu.py` from `../raw-insbu/` or, when those files are absent, from
their extract `data/insbu-aggregates-2016-2024.csv` (the 355 values the model
uses, at full precision; identical results). They are the default for everything: the real GDP pin, the deflator behind the real
exchange rate, trade prices and export volumes, government consumption
(as a real path), investment, inventories, the mining resource, and every
comparison target. Regenerate the extract after INSBU revises its accounts:
`python -m gemcore.insbu --write-extract`. The MFMod datasheet is not in the
share package. Without any INSBU data the runner falls back to the actuals
workbook, with a warning. What matches and what does not is in appendix A.6.3. The paragraphs below describe the options that existed before.

Since 2026-09-28 the external side is on one source: world trade prices are
MFMod's realised trade deflators in dollars (`--trade-prices implicit`),
every exporter but gold follows MFMod's real export volume (`--exog all`),
and the mining resource follows MFMod's industry growth. Read the real
growth rows as the test; the nominal trade-share rows cannot be matched on
this data set (appendix A.6.3). `--rowclos 1` (real rate
free, foreign financing on the actual current-account path) is experimental
and does not converge for 2020–22.

Defaults: the actual 2019 premium (1.583); the **actual real official
exchange rate** (`--rexr actual`, the official rate over the GDP deflator, a
24 % real appreciation by 2024 on the rebased-GDP deflator the workbook
uses, only 6 % on MFMod's own deflator — `--basis mfmod` switches every
ratio, growth rate and deflator to MFMod's accounts); government
consumption on the national-accounts path (`--govcon na`); coffee/tea export
volumes pinned; the mining resource scaled with mining output and on the
idle-resource closure; sector and demand-side reference series from the
World Bank MFMod datasheet (`--sector-source mfmod`). The MFMod file is not
in the share package (World Bank internal); without it the runner falls back
to the actuals workbook's own sector block, which the user has withdrawn as
wrong for 2020, so ask for the file if you need the sector comparison.

`--sector-targets` pins observed real value-added growth by sector group and
backs out a productivity shifter per group (`TFPGRP`, a port-only variable;
`gemcore/backcast.py`), with real GDP an outcome unless `--pin-gdp`.
`--target-groups agr,ind,serv` pins all three. `--armington all:2` scales the
Armington elasticities before calibration. What the backcast matches and
what it does not is in appendix A.6.3.

Every runner solves the base run twice first (section 4) because a scenario
is defined relative to the rebased reference; there is no way to skip this.

## 4. How it works, in five paragraphs

**Equations.** `gemcore/model.py::residuals(V, cal, t, Lprev, clo)` returns a
dict equation -> residual for one period, given the variable levels `V`, the
calibrated parameters `cal`, the previous period's solution `Lprev` (stocks
are predetermined) and the closure accessors `clo`. A solution sets every
residual to zero. Each block cites the `mod.gms` line it transcribes.

**Squaring.** GAMS solves the model as a mixed complementarity problem, where
every variable is paired with an equation by construction. This port solves a
square system of equalities with Newton's method, so the pairing is explicit:
`solver.base_closure_fixed` decides which variables are exogenous by replaying
`macclos.inc` and `facclos.inc` from the dataset's own switches
(`numeraire0`, `govclos0`, `rowclos0`, `siclos0`, `facclos0`, the three rule
sets); `closure.py` declares which variable each equation determines; and
`partition.py` verifies on the numeric Jacobian that the result is a perfect
matching. At every period: 2660 equations, 2660 free variables. If a change
breaks this, `partition.py` says which equations are orphaned.

**Inequalities.** Two of the model's complementarities cannot be solved as
equalities: the import quotas (bind with a positive rent, or slack with rent
zero) and the idle-resource closure used for the mining resource in `combi`.
`complementarity.py::sweep_solve` solves the path, moves any cell found on
the wrong branch (negative rent, imports above a slack ceiling, negative
idle share) and re-solves until consistent. All runners go through it.

**Dynamics and the two passes.** `dynamics._solve_path` marches 2019..2040,
carrying capital and debt stocks forward. As in `sim.gms`, the base is solved
twice: pass 1 (`run_base`, `dcal01=True`) pins real GDP to the target path
and backs productivity out; pass 2 (`run_reference`) freezes that
productivity and frees GDP, reproducing pass 1 exactly. `par_redefn_0` then
rebases the "0"-parameters onto the reference, and scenarios are deviations
from that.

**Scenarios.** A `Scenario` (`scenarios.py`) is a set of ratios on reference
parameters plus closure switches, mirroring `par-defn-sim.inc`. The paper's
four are built by `uni`, `uni_inf`, `uni_inf_hd`, `combi`; each docstring
cites the `bdi2019-sim2.inc` lines it implements. `combi` uses an
idle-resource closure for the mining resource (the endowment triples and use
follows demand at the reference rent) rather than GAMS's "use triples", which
does not converge at a positive rent; the two agree on every macro increment
to three decimals. `combi+` is not implemented.

## 5. Results and how well they match

`reports/macro-growth-2026-2040.md`: average annual growth 2026-2040 in the
full model, for base and the four scenarios. In replication mode
(`--gams-replication`) the model reproduces the reference application: base
matches to within 0.09 pp on every line; scenario *increments* match on every line
except two known gaps (exports and private investment under `uni` about 1 pp
low; government investment under the infrastructure push about 1 pp low),
described in appendix A.6.2. Endogenous checks
that are not pinned by the closure — the premium path, unemployment by labour
type, debt ratios — match GAMS to four or five figures.

`reports/solutions/`: full variable dumps.
`reports/backcast-2019-2024*.md`: the model against Burundi's actual
2020-2024 outcomes: the default run and its variants (pass-through `-pt-*`,
quotas `-fuel-fx`, `-chem-fx`, `-fx-both`, `-quotas-frozen`, and others; appendix A.6.3).

Units: model levels are in the SAM's units (10 bn BIF); the reference application's tables
divide by 10 (`samsol/samrep`). Compare growth rates, shares and indices:
levels include the premium adjustment, which revalues parallel-rate transactions.

## 6. Writing a new scenario

1. Add a builder in `gemcore/scenarios.py` returning a `Scenario`. Available
   levers: `rowclos`/`govclos`/`siclos` (closure codes as in `macclos.inc`),
   `prexr_ratio` (premium), `rexr_ratio`, `qmbar_ratio` + `quota_slack`
   (import quotas), `qfinsb_ratio` (factor endowments), `ddkins` (additive
   public investment), `mpcapgov`/`mtfp` (public-capital productivity),
   `qgb_ratio` (government consumption), `fprdab_ratio` (factor
   productivity), `idle_factors`, and the three rule dicts. Ratios multiply
   the reference parameter for the given `(index, period)`.
2. Register it in `SCENARIOS`. Every non-base scenario should include
   `**scenario_closure(cal)` — the closure `sim.gms` cascades from `base`.
3. Run `python3 runs/run.py <name>` and check the solve log: every period
   should converge to ~1e-13; `WALRAS` should be ~0. A "converged" solve with
   a large `WALRAS` means a variable was silently frozen by the partition —
   see appendix A.4.4.
4. To change something not covered by a lever — a new tax, a different rent
   recipient, an exogenous path the data does not carry — set the parameter
   on `cal` before `run_scenario`, or add an accessor to `Closure` in
   `state.py` and use it in `model.py`, as `qeb` and `fprdab` do.

Anything multiplied in place on `cal` must be listed in
`scenarios._SNAPSHOT_ATTRS`, or the next scenario in the same process starts
from a mutated calibration.

## 7. Caveats the user of this model should know

- The base-year FX premium in the authors' data (`prexr000 = 2.5`) is the
  2024/25 parallel-rate ratio, not 2019's (1.58). The reference application
  uses 2.5; the backcast uses 1.583. See `docs/MECHANISMS.md`.
- The gains from exchange-rate unification (`uni` and everything built on
  it) run almost entirely through the premium passing into import prices:
  GDP +1.0 point a year at full pass-through, +0.4 at half, zero at none.
  `runs/uni_passthrough.py` reports the range; see `docs/MECHANISMS.md` 1.8.
- Fuel shortages work through three channels added to GEM-Core, on by default: firms economise on fuel, informal fuel from the DRC and Tanzania flows once the pump shortage is deep enough, and households switch part of their fuel spending to firewood and charcoal (`gemcore/fuel.py`, `docs/MECHANISMS.md` 2.6). The fuel quota itself follows foreign-exchange availability: BRB's allocation, a share of official foreign-exchange inflows, over the world price (`docs/MECHANISMS.md` 2.7). `--gams-replication` on the runners removes them, to re-check the port against the reference application.
- The SAM is built on INSBU's rebased national accounts: its 2019 GDP is
  within 1.3 % of INSBU's before the premium adjustment.
- Two scenario lines do not fully reproduce GAMS (section 5). The base run,
  `combi`, and the backcast are not affected.
- The poverty microsimulation module of GEM-Core (`reppov*.gms`, driven by
  the `hhdsurvey` sheet) is **not ported**. The port reads the survey sheet
  but uses nothing from it.
