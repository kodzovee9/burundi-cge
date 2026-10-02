# A recursive-dynamic CGE model of Burundi

**Kodzovi Senu Abalo**, Senior Economist, Fiscal Policy and Growth Global Department, World Bank

*The findings, interpretations and conclusions expressed here are those of the author and do not necessarily represent the views of the World Bank, its Executive Directors or the governments they represent.*

A computable general equilibrium (CGE) model of Burundi, written in Python and calibrated to a 2019 social accounting matrix built on INSBU's rebased national accounts. It solves year by year from 2019 to 2040. It started as an equation-for-equation port of the World Bank's GEM-Core model (Lofgren and Cicowiez, GAMS) and adds what Burundi's economy needs:

- a **dual exchange-rate market**: an official and a parallel rate, with the premium, its partial pass-through to import costs, and the rent it creates, part of which is a real cost of rent-seeking;
- **foreign-exchange rationing of fuel and fertiliser**: official-rate imports capped by the foreign exchange the central bank receives, with the quota rent this creates;
- **fuel shortages**: substitution between fuel and value added in production, informal fuel imported from the DRC and Tanzania once the shortage is deep, and households switching to firewood;
- a **backcast against 2019–2024 outcomes**, as a test of the model against what actually happened.

The technical appendix ([APPENDIX.pdf](APPENDIX.pdf)) documents the data, every equation, the closures, the transmission channels, the validation and the results. The seminar slides ([slides/A CGE Model of Burundi.pdf](slides/A%20CGE%20Model%20of%20Burundi.pdf)) present the model, its transmission channels and the results.

## Main results

Real GDP growth 2026–40, % a year, under the full model (a quarter of the premium rent treated as a real cost):

| Scenario | Growth | 2040 GDP vs base |
|---|---:|---:|
| Base | 2.22 | – |
| Exchange-rate unification (`uni`) | 3.71 | +23 % |
| + public infrastructure (`uni+inf`) | 4.23 | +32 % |
| + human development (`uni+inf+hd`) | 4.51 | +38 % |
| + mining (`combi`) | 4.85 | +44 % |
| + margins, hydropower, education mix, mining FDI (`combi-x`) | 4.99 | +47 % |

The unification gain depends on how much of the premium reaches import prices: +1.49 points a year at full pass-through, +0.47 at half. Appendix Tables A.10 and A.11 have the details.

## Reproduce every finding

You need Python 3.11, 3.12 or 3.13 (tested on 3.13). **If you do not use Python, follow [USER-GUIDE.pdf](USER-GUIDE.pdf)**: double-click `setup_mac.command` or `setup_windows.bat` once, then `run_all_mac.command` or `run_all_windows.bat`.

From a terminal:

```bash
python3.13 -m venv .venv && . .venv/bin/activate
pip install -r pymodel/requirements.txt
cd pymodel && python runs/reproduce_all.py          # about 25 minutes; --quick for the core checks
```

`runs/reproduce_all.py` re-runs every simulation, audits every solution (largest equation residual in every year) and compares 41 documented findings with `runs/expected_results.json`. Its verdict is written to `pymodel/reports/REPRODUCTION-CHECK.md`. The 2019–24 backcasts read INSBU's national accounts from the extract in `pymodel/data/` (see [DATA.md](DATA.md)).

## What is in the repository

| Path | Content |
|---|---|
| `pymodel/` | the Python model: `gemcore/` (equations, calibration, closures, solver), `runs/` (scenarios, backcast, reproduction check), `validation/`, `docs/MECHANISMS.md`, `README.md` (layout, options, how to add a scenario) |
| `pymodel/reports/` | the results as last generated: growth tables, pass-through and reform-effect tables, backcasts and the solution audit (the reproduction check writes `REPRODUCTION-CHECK.md` here when run) |
| `model/` | the GEM-Core GAMS source and the Burundi application: calibration workbook (`user-files/bdi2019/bdi2019-data.xlsx`), SAM adjustments and scenario definitions (`bdi2019-data2.inc`, `bdi2019-sim2.inc`). GAMS is not needed to run anything |
| `APPENDIX.*`, `USER-GUIDE.*` | the technical appendix and the guide for non-Python users |
| `slides/` | the seminar deck presenting the model, as a PDF |
| `tools/` | builders for the Word versions of the documents and the data template |
| `DATA.md` | every data source, what it is used for and its status |

## Citation

See [CITATION.cff](CITATION.cff). Please cite the model as:

> Abalo, Kodzovi Senu (2026). A recursive-dynamic CGE model of Burundi with a dual exchange-rate market and foreign-exchange rationing of fuel.

The GAMS model this work builds on is GEM-Core, by Hans Lofgren and Martín Cicowiez (World Bank).

## Licence

Code: MIT ([LICENSE](LICENSE); credit for the GEM-Core files in [NOTICE](NOTICE)). Documents and results: CC BY 4.0 ([LICENSE-docs.md](LICENSE-docs.md)). Data: see [DATA.md](DATA.md).
