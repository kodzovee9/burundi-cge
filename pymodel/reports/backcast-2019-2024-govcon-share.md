# Backcast 2019-2024

| setup | |
| --- | --- |
| Macro basis (growth, deflator, ratios, targets) | INSBU rebased accounts |
| Real GDP | pinned to the basis's real growth (calibration pass, aggregate TFP backed out) |
| Sector reference | INSBU rebased accounts |
| Government consumption | INSBU share path |
| Exports | every exporter but gold pinned to INSBU rebased accounts real exports |
| Trade prices | realised deflators in USD (INSBU) |
| Real official exchange rate | observed (official rate over the basis GDP deflator) |
| Mining resource | scaled by insbu-b05 |
| Inventories | observed change added for c-agr,c-food (INSBU) |
| Investment | public and private on INSBU's total GFCF share |
| Parallel premium | observed (IN03) |
| Fuel and chemicals import quotas | fuel at BRB's observed volumes to 2024, then set by foreign exchange (share of official FX carried from 2024); chemicals and fertiliser at BRB's observed volumes to 2024, then set by foreign exchange (share of official FX carried from 2024) |
| Premium pass-through to import prices | full (published model) |
| Rest-of-world closure | rowclos 2 (data): real rate and premium pinned, foreign financing clears |
| Base-year premium | prexr000 = 1.583 (PREXR00 = 1.5830); base-year max residual 1.28e-13 |

## Shares of GDP and rates (%)

| indicator | 2019 m | 2019 a | 2020 m | 2020 a | 2021 m | 2021 a | 2022 m | 2022 a | 2023 m | 2023 a | 2024 m | 2024 a | summary |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| PrvCon %GDP | 90.3 | 89.2 | 89.6 | 88.1 | 86.3 | 86.7 | 86.9 | 88.1 | 90.4 | 91.9 | 88.4 | 87.5 | base gap 2019 1.1; model chg -1.9; actual chg -1.6 |
| GovCon %GDP | 9.9 | 10.6 | 9.8 | 10.5 | 9.4 | 10.1 | 8.9 | 9.6 | 7.9 | 8.4 | 7.0 | 7.5 | base gap 2019 -0.7; model chg -2.9; actual chg -3.1 |
| GFCF %GDP | 11.7 | 11.3 | 10.6 | 10.6 | 12.7 | 12.8 | 12.0 | 12.0 | 11.9 | 11.6 | 12.2 | 12.1 | base gap 2019 0.4; model chg 0.5; actual chg 0.8 |
| Stocks %GDP | 1.2 | 0.3 | 3.1 | 2.5 | 3.8 | 3.0 | 5.2 | 4.4 | 3.1 | 2.4 | 2.9 | 3.8 | base gap 2019 0.9; model chg 1.7; actual chg 3.5 |
| Exports %GDP | 5.3 | 6.0 | 4.5 | 5.2 | 4.6 | 5.5 | 4.7 | 5.3 | 5.4 | 6.1 | 4.0 | 5.0 | base gap 2019 -0.7; model chg -1.3; actual chg -1.0 |
| Imports %GDP | 18.4 | 20.3 | 17.7 | 19.8 | 17.0 | 21.3 | 17.7 | 22.4 | 18.7 | 23.3 | 14.4 | 19.1 | base gap 2019 -1.9; model chg -4.0; actual chg -1.2 |
| CA balance %GDP | -7.2 | -7.9 | -7.6 | -6.7 | -7.0 | -7.3 | -8.7 | -9.9 | -8.8 | -9.5 | -6.3 | -6.0 | base gap 2019 0.7; model chg 0.9; actual chg 1.9 |
| Ext public debt %GDP | 8.7 | 11.0 | 9.3 | 10.5 | 9.0 | 12.4 | 8.3 | 12.4 | 8.8 | 11.1 | 8.2 | 11.1 | base gap 2019 -2.3; model chg -0.6; actual chg 0.1 |
| Gov domestic debt %GDP | 24.2 | 25.1 | 27.7 | 28.7 | 31.9 | 28.3 | 34.9 | 31.3 | 37.6 | 28.0 | 37.3 | 24.5 | base gap 2019 -0.9; model chg 13.1; actual chg -0.6 |
| Unemployment % | 1.1 | 1.0 | 1.2 | 1.0 | 1.2 | 1.1 | 1.2 | 0.9 | 1.3 | 0.9 | 1.7 | 0.9 | base gap 2019 0.1; model chg 0.5; actual chg -0.1 |

## Growth (%/yr)

| indicator | 2020 m | 2020 a | 2021 m | 2021 a | 2022 m | 2022 a | 2023 m | 2023 a | 2024 m | 2024 a | summary |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| Agriculture real VA growth % | 1.2 | 2.8 | 2.8 | -1.1 | 3.2 | 2.4 | 0.4 | -3.2 | 6.0 | 4.5 | model avg 2.7; actual avg 1.1 |
| Industry real VA growth % | -2.0 | -2.5 | 2.4 | 4.9 | -0.8 | 2.0 | 3.0 | 1.7 | 2.3 | 4.8 | model avg 1.0; actual avg 2.2 |
| Manufacturing real VA growth % | -0.0 | 0.4 | 1.0 | 3.6 | -1.0 | 1.0 | 2.4 | 6.7 | 4.6 | -7.0 | model avg 1.4; actual avg 1.0 |
| Services real VA growth % | -0.3 | -1.9 | 0.3 | 3.4 | -0.0 | 0.0 | 2.6 | 7.4 | 2.6 | 4.6 | model avg 1.0; actual avg 2.7 |
| Mining (B) real VA growth % | -13.8 | -14.1 | -0.4 | -3.5 | 4.1 | 2.3 | 7.1 | 4.4 | -12.1 | -13.9 | model avg -3.0; actual avg -4.9 |
| Real PrvCon growth % | -0.3 | -1.3 | -1.4 | 2.3 | 1.5 | 2.9 | 7.0 | 6.6 | 0.8 | 1.5 | model avg 1.5; actual avg 2.4 |
| Real GovCon growth % | 0.9 | 2.4 | 1.1 | 3.1 | -2.9 | -4.0 | -7.3 | 1.4 | -0.4 | 2.8 | model avg -1.7; actual avg 1.1 |
| Real GFCF growth % | -5.8 | -4.7 | 17.1 | 18.3 | -3.3 | 1.5 | 5.2 | 5.9 | -2.2 | 2.1 | model avg 2.2; actual avg 4.6 |
| Real exports growth % | -9.1 | -5.4 | -2.4 | -0.6 | -4.7 | -9.5 | 11.1 | 11.7 | -7.6 | -2.3 | model avg -2.5; actual avg -1.2 |
| Real imports growth % | 1.9 | 1.3 | -0.2 | 13.6 | 2.4 | 9.2 | 11.1 | 14.0 | -11.0 | -10.1 | model avg 0.8; actual avg 5.6 |
| Real GDP growth % | -0.1 | -0.1 | 1.7 | 1.7 | 1.3 | 1.3 | 1.8 | 1.8 | 4.6 | 4.6 | model avg 1.9; actual avg 1.9 |

## Indices, 2019 = 100

| indicator | 2019 m | 2019 a | 2020 m | 2020 a | 2021 m | 2021 a | 2022 m | 2022 a | 2023 m | 2023 a | 2024 m | 2024 a | summary |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| CPI/deflator idx | 100.0 | 100.0 | 99.8 | 99.8 | 98.8 | 101.7 | 99.4 | 103.5 | 99.5 | 108.6 | 102.0 | 108.3 | model last 102.0; actual last 108.3 |
| Fuel: informal share of supply % | 0.0 | - | 0.0 | - | 0.0 | - | 0.0 | - | 0.0 | - | 6.4 | - | model last 6.4; actual last - |
| Fuel: market price / pump price | 100.0 | - | 104.5 | - | 100.0 | - | 100.0 | - | 100.0 | - | 334.9 | 335.4 | model last 334.9; actual last 335.4 |
| Fuel: share of official FX % | 35.5 | - | 37.0 | - | 38.9 | - | 48.9 | - | 50.0 | - | 43.9 | - | model last 43.9; actual last - |
| Chemicals: share of official FX % | 40.2 | - | 49.0 | - | 45.4 | - | 44.5 | - | 37.5 | - | 45.9 | - | model last 45.9; actual last - |
| Official FX inflow idx (USD) | 100.0 | - | 82.1 | - | 89.3 | - | 103.6 | - | 120.3 | - | 107.5 | - | model last 107.5; actual last - |

Real GDP growth is an input (pinned), shown as a check only. Levels are not comparable (SAM GDP is about twice the national-accounts figure); every row is a share, growth rate or index. TFP shifter rows are model-only (100 = base year).
