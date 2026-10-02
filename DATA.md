# Data

What the model reads, where it comes from, and what is not in this repository.

## In the repository

| File | What it is | Source |
|---|---|---|
| `model/user-files/bdi2019/bdi2019-data.xlsx` | The calibration workbook: the 2019 social accounting matrix (sheet `SAM`), sets, elasticities, closure switches and exogenous paths to 2040 | The author's 2019 SAM for Burundi, built on INSBU's rebased national accounts. The household-survey sheet (`hhdsurvey`) used by GEM-Core's poverty module has been removed; the Python model does not use it |
| `model/user-files/bdi2019/bdi2019-data2.inc`, `bdi2019-sim2.inc` | SAM adjustments for the exchange-rate premium and the fuel quota; scenario definitions | The Burundi application of GEM-Core |
| `pymodel/data/burundi-actuals-2019-2024-FINAL.xlsx` | Observed 2019–2024 series fed to the backcast and compared with its results | Every row names its source: World Bank WDI and Pink Sheet, UNSD National Accounts Main Aggregates, UN Comtrade, BRB, IMF Article IV and IFS, and a few series from the World Bank MFMod Burundi datasheet (official exchange rate, fiscal items), which for these years carries the published national and WDI figures |
| `pymodel/data/insbu-aggregates-2016-2024.csv` | The 355 values of INSBU's rebased national accounts the backcast uses: GDP, value added (total, ISIC groups, extraction), consumption, investment, stockbuilding, exports and imports, at current and previous-year prices, 2016–2024; stockbuilding of six products (crops and livestock A01–A04, extraction B05, food C06); four items of the integrated accounts for 2021, 2023 and 2024. Results are identical to reading the original files | INSBU: supply-use tables (TRE) and integrated economic accounts (TCEI). Extracted with `python -m gemcore.insbu --write-extract` |
| `pymodel/data/burundi-minimum-data-2019-2024-INSBU.xlsx` | The minimum data set for the backcast, filled from INSBU's accounts | INSBU |
| `pymodel/data/*-TEMPLATE.xlsx` | Blank versions of the two workbooks above, to update or extend the data | — |
| `pymodel/gemcore/brb.py` | Official fuel and chemicals import volumes and other figures, typed in from BRB publications (each value cites its table) | BRB: *Rapport annuel 2022*, *Indicateurs de conjoncture* (Dec. 2022, Dec. 2023), *Rapport de politique monétaire* (2024 Q2, Q4), available from brb.bi |

## Not in the repository

| Data | Used for | How to get it |
|---|---|---|
| INSBU's original files: supply-use tables (TRE) 2016–2024 at current and previous-year prices, integrated economic accounts (TCEI) 2021, 2023, 2024 | Nothing beyond the extract above, which carries every value the model reads from them | Institut National de la Statistique du Burundi (INSBU). Placed in `raw-insbu/` at the top of the repository (file names in `pymodel/gemcore/insbu.py`), they are read instead of the extract |
| World Bank MFMod Burundi datasheet | Optional switches of the backcast only (`--basis mfmod`, `--sector-source mfmod`, `--mining-index mfmod-ind`), kept to reproduce earlier comparisons. No reported finding uses them | Internal to the World Bank. The series these switches read are World Bank WDI series (codes in `pymodel/gemcore/backcast.py`) |
| Household survey records | GEM-Core's GAMS poverty module only | Removed from the calibration workbook |
