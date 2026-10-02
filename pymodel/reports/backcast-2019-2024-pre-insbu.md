# Backcast 2019-2024

| setup | |
| --- | --- |
| Macro basis (growth, deflator, ratios, targets) | actuals workbook (WDI GDP) |
| Real GDP | pinned to the basis's real growth (calibration pass, aggregate TFP backed out) |
| Sector reference | MFMod datasheet |
| Government consumption | UNSD share (TG13), withdrawn |
| Exports | every exporter but gold pinned to MFMod datasheet real exports |
| Trade prices | realised deflators in USD (MFMod) |
| Real official exchange rate | observed (official rate over the basis GDP deflator) |
| Mining resource | scaled by mfmod-ind |
| Inventories | grow with GDP (model) |
| Investment | public from IN09, private on the authors' path |
| Parallel premium | observed (IN03) |
| Rest-of-world closure | rowclos 2 (data): real rate and premium pinned, foreign financing clears |
| Base-year premium | prexr000 = 1.583 (PREXR00 = 1.5830); base-year max residual 1.28e-13 |

## Shares of GDP and rates (%)

| indicator | 2019 m | 2019 a | 2020 m | 2020 a | 2021 m | 2021 a | 2022 m | 2022 a | 2023 m | 2023 a | 2024 m | 2024 a | summary |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| PrvCon %GDP | 90.3 | 87.2 | 88.9 | 84.5 | 88.0 | 84.5 | 88.8 | 83.5 | 89.6 | 85.2 | 86.3 | 86.1 | base gap 2019 3.1; model chg -4.0; actual chg -1.1 |
| GovCon %GDP | 9.9 | 20.3 | 11.4 | 23.4 | 11.2 | 22.9 | 11.3 | 23.1 | 11.1 | 22.8 | 11.4 | 23.4 | base gap 2019 -10.4; model chg 1.5; actual chg 3.1 |
| GFCF %GDP | 11.7 | 13.1 | 10.5 | 10.6 | 10.6 | 12.1 | 10.7 | 12.4 | 11.1 | 12.0 | 11.1 | 10.6 | base gap 2019 -1.4; model chg -0.6; actual chg -2.4 |
| Stocks %GDP | 1.2 | - | 1.2 | - | 1.2 | - | 1.2 | - | 1.2 | - | 1.1 | - | base gap 2019 -; model chg -0.1; actual chg - |
| Exports %GDP | 5.3 | 8.9 | 4.4 | 7.8 | 4.8 | 8.6 | 4.8 | 8.5 | 4.4 | 9.0 | 3.8 | 8.3 | base gap 2019 -3.6; model chg -1.6; actual chg -0.6 |
| Imports %GDP | 18.4 | 30.1 | 16.4 | 28.1 | 15.7 | 30.1 | 16.7 | 29.5 | 17.4 | 30.8 | 13.7 | 29.0 | base gap 2019 -11.7; model chg -4.8; actual chg -1.1 |
| CA balance %GDP | -7.2 | -11.7 | -6.9 | -9.3 | -6.4 | -9.6 | -8.2 | -13.2 | -9.2 | -12.8 | -5.8 | -8.2 | base gap 2019 4.5; model chg 1.4; actual chg 3.5 |
| Ext public debt %GDP | 8.7 | 16.4 | 8.5 | 14.7 | 7.9 | 16.4 | 7.2 | 16.6 | 7.8 | 14.9 | 7.5 | 15.3 | base gap 2019 -7.7; model chg -1.3; actual chg -1.1 |
| Gov domestic debt %GDP | 24.2 | 37.4 | 27.1 | 40.1 | 30.7 | 37.5 | 33.2 | 41.7 | 35.5 | 37.6 | 35.7 | 33.8 | base gap 2019 -13.2; model chg 11.5; actual chg -3.6 |
| Unemployment % | 1.1 | 1.0 | 1.1 | 1.0 | 1.2 | 1.1 | 1.2 | 0.9 | 1.3 | 0.9 | 1.5 | 0.9 | base gap 2019 0.1; model chg 0.3; actual chg -0.1 |

## Growth (%/yr)

| indicator | 2020 m | 2020 a | 2021 m | 2021 a | 2022 m | 2022 a | 2023 m | 2023 a | 2024 m | 2024 a | summary |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| Agriculture real VA growth % | -1.0 | 2.8 | 3.6 | 3.4 | 2.2 | -0.8 | 3.0 | 1.5 | 4.1 | 3.1 | model avg 2.4; actual avg 2.0 |
| Industry real VA growth % | -2.3 | 1.8 | 3.4 | 3.0 | 1.6 | 3.2 | 2.7 | 3.4 | 3.3 | 1.0 | model avg 1.7; actual avg 2.5 |
| Services real VA growth % | 2.7 | -1.7 | 2.8 | 2.9 | 2.7 | 3.1 | 3.7 | 3.1 | 4.5 | 5.7 | model avg 3.3; actual avg 2.6 |
| Real PrvCon growth % | 2.6 | 0.3 | 0.8 | 3.0 | 3.1 | 2.4 | 4.5 | 2.2 | -0.4 | 2.7 | model avg 2.2; actual avg 2.1 |
| Real GovCon growth % | 15.6 | 19.2 | 3.9 | 2.9 | 3.9 | 5.9 | 3.3 | 5.4 | 11.3 | 4.4 | model avg 7.6; actual avg 7.6 |
| Real GFCF growth % | 0.7 | -16.6 | -0.9 | 3.9 | 5.2 | 4.0 | 10.0 | 4.0 | -3.1 | 2.6 | model avg 2.4; actual avg -0.4 |
| Real exports growth % | -11.9 | -14.9 | 4.9 | 3.4 | 3.5 | 5.8 | 0.6 | 2.9 | 0.6 | 3.0 | model avg -0.5; actual avg 0.0 |
| Real imports growth % | 10.0 | 3.4 | -5.0 | 3.2 | 6.7 | 7.0 | 9.5 | 4.2 | -9.9 | 1.4 | model avg 2.2; actual avg 3.8 |
| Real GDP growth % | 0.3 | 0.3 | 3.2 | 3.2 | 2.3 | 2.3 | 3.2 | 3.2 | 4.1 | 4.1 | model avg 2.6; actual avg 2.6 |

## Indices, 2019 = 100

| indicator | 2019 m | 2019 a | 2020 m | 2020 a | 2021 m | 2021 a | 2022 m | 2022 a | 2023 m | 2023 a | 2024 m | 2024 a | summary |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| CPI/deflator idx | 100.0 | 100.0 | 97.5 | 93.8 | 98.0 | 91.5 | 98.7 | 94.5 | 99.4 | 100.8 | 99.2 | 102.2 | model last 99.2; actual last 102.2 |

Real GDP growth is an input (pinned), shown as a check only. Levels are not comparable (SAM GDP is about twice the national-accounts figure); every row is a share, growth rate or index. TFP shifter rows are model-only (100 = base year).
