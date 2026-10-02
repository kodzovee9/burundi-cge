# Technical Appendix: A Recursive-Dynamic CGE Model of Burundi with a Dual Exchange-Rate Market and Foreign-Exchange Rationing of Fuel

**Kodzovi Senu Abalo**, Senior Economist, Fiscal Policy and Growth Global Department, World Bank

Code, data and replication files: [github.com/kodzovee9/burundi-cge](https://github.com/kodzovee9/burundi-cge)

*Draft for discussion, 30 September 2026. Figures refer to the model as of that date.*

*The findings, interpretations and conclusions expressed here are those of the author and do not necessarily represent the views of the World Bank, its Executive Directors or the governments they represent.*

---

## A.1 Overview

This appendix documents the computable general equilibrium (CGE) model used in the paper. It is written for readers who want to understand what drives the results, and for reviewers who want to assess the modelling choices. It covers:

- the data and calibration (A.2);
- the full equation system (A.3);
- the closure rules and solution method (A.4);
- the transmission mechanisms through which policies act (A.5);
- validation (A.6);
- the reference simulations and the simulations the model can support (A.7);
- the main limitations (A.8).

**Lineage.** The model is built on GEM-Core, the World Bank's single-country recursive-dynamic CGE framework developed by Hans Lofgren and Martín Cicowiez. GEM-Core belongs to the family of the IFPRI standard model (Lofgren, Harris and Robinson 2002). The reference application to Burundi, with a 2019 social accounting matrix (SAM), a base run to 2040 and a set of reform scenarios, was written in GAMS and solved as a mixed complementarity problem with PATH (Dirkse and Ferris 1995).

I re-implemented that model in Python. The implementation reproduces the base-year SAM exactly, the published base run, and the published scenario increments (A.6.2). I then extended it with mechanisms specific to Burundi's foreign-exchange regime.

**What is standard and what is added.** Table A.1 separates the two.

| Block | From GEM-Core (reference application) | Added in this model |
| --- | --- | --- |
| Production | nested CES value added with a three-level labour nest; Leontief intermediates; fixed-proportion natural resource; TFP with a trade-openness elasticity; productivity of public capital | CES substitution between fuel and value added; sector-group productivity shifters for historical calibration |
| Trade | Armington imports, CET exports | — |
| Exchange rate | dual market: official rate plus a parallel premium; commodity-specific shares cleared at the official rate; premium rent | partial pass-through of the premium to import prices (sensitivity parameter); part of the premium rent is a real resource cost (rent-seeking) |
| Quotas | import quotas on refined petroleum and chemicals, as a complementarity | fuel and chemicals (fertiliser) quotas set by official foreign-exchange availability; informal fuel supply outside the quota (complementarity) |
| Households | LES demand, two household groups | switching of household fuel spending to firewood and charcoal |
| Factor markets | wage curve with unemployment for eight labour types; sector-specific capital | idle-resource closure for the mining resource (complementarity) |
| Dynamics | capital accumulation with profitability-weighted allocation; demographic labour supply; debt stocks | foreign direct investment that can be targeted to one activity |
| Scenario technology | — | scenario paths for input coefficients and trade and transport margins (roads, hydropower) |
| Solution | GAMS/PATH, mixed complementarity | Python, square equality system solved by Newton's method, with complementarities resolved by an active-set sweep (A.4.4) |

All additions are exactly inert at base-year prices, so they leave the base-year calibration unchanged. They are switched on by default. A single switch (`--gams-replication`) removes them and reproduces the reference application's tables to the digit.

---

## A.2 Data and calibration

### A.2.1 The social accounting matrix

The model is calibrated to a 2019 SAM for Burundi. The SAM is consistent with the national accounts of the Institut National de la Statistique du Burundi (INSBU) rebased in 2024. Before the premium adjustment described below, the SAM's 2019 GDP is within 1.3 % of INSBU's figure. Table A.2 lists the accounts.

**Table A.2. Accounts of the model**

| Set | Elements |
| --- | --- |
| Activities (19) | agriculture; mining; food processing; textiles and apparel; wood and paper; chemicals and plastics; non-metallic mineral products; metals; other manufacturing; electricity; water; construction; trade; transport; hotels and restaurants; public administration; education; health; other services |
| Commodities (20) | one per activity, plus refined petroleum (fully imported) |
| Labour (8) | male and female, each with four education levels (none, primary, secondary, tertiary) |
| Other factors | private capital; public capital; agricultural land; mining natural resource |
| Institutions | rural households; urban households; government; rest of the world |
| Capital accounts | government investment; non-government investment; foreign direct investment |

**The premium adjustment.** The published SAM values all foreign transactions at the official exchange rate. The model adds a premium account that revalues imports bought at the parallel rate, and export receipts sold at it, by the base-year parallel/official ratio. The resulting rent is credited to private capital. The reference application sets that ratio at 2.5, which is the late-2024 value. The actual 2019 ratio was about 1.58. The scenario tables in A.7.1 keep 2.5 so that they stay comparable with the reference application. The historical backcast (A.6.3) uses 1.58. **This is one of the choices on which I seek feedback.**

**Table A.3. Structure of the economy in the calibrated 2019 SAM**

| Item | Value |
| --- | --- |
| Private consumption, % GDP | 90.3 |
| Government consumption, % GDP | 9.9 |
| Gross fixed capital formation, % GDP | 11.7 |
| Exports, % GDP | 5.3 |
| Imports, % GDP | 18.4 |
| Current account, % GDP | −7.2 |
| Value added: agriculture / other services / trade / food processing, % | 41.9 / 20.0 / 9.1 / 7.3 |
| Factor income: labour / private capital / land / mining resource, % | 38.2 / 48.7 / 12.2 / 0.8 |
| Imports cleared at the official rate (fuel 13.4 %, chemicals 15.2 %), % | 28.6 |
| Import share of domestic supply: refined petroleum / textiles / other manufacturing / transport services, % | 100 / 74 / 74 / 69 |
| Export share of output: mining / textiles / non-metallic minerals / food, % | 67 / 9 / 7 / 6 |
| Fuel cost share of (value added + fuel): electricity / transport / construction / chemicals, % | 45 / 31 / 20 / 11 |

Macro shares are computed with the 2019 premium (1.58). The 28.6 % of imports cleared at the official rate matches the Banque de la République du Burundi's (BRB) statement that about 30 % of imports are priced at the official rate.

### A.2.2 Elasticities and behavioural parameters

**Table A.4. Behavioural parameters**

| Parameter | Value | Source / note |
| --- | --- | --- |
| Substitution, top value-added nest (labour, capital, land) | 1.25 (mining 0.5) | reference application |
| Substitution between education levels | 4.0 | reference application |
| Substitution between male and female labour, within an education level | 2.0 | reference application |
| Armington, imports vs domestic | 2.0 primary; 1.5 manufactures; 0.9 services and utilities | reference application |
| CET, exports vs domestic | same values as the Armington elasticities | reference application |
| LES income elasticities | 0.84 (food, agriculture) to 1.64 (manufactures, most services); fuel 1.44 | reference application |
| Frisch parameter | −2.5 (both household groups) | reference application |
| Wage-curve elasticity, all labour types | −0.5 | reference application |
| Base-year unemployment | 0.3 % to 10.5 % by labour type | reference application |
| Capital-allocation sensitivity to relative returns, κ | 0.5 | reference application |
| Depreciation: private / public capital | 5 % / 2.5 % | reference application |
| TFP elasticity to trade/GDP | 0.01 | reference application |
| Fuel–value-added substitution, $\sigma^F$ | 0.1 | this model; low end of short-run evidence |
| Household fuel-to-firewood switching, ε | 0.1 | this model |
| Informal fuel markup over the world price, μ (of which transport paid abroad) | 0.9 (0.1) | this model; calibrated to 2024 black-market prices (A.6.4) |
| Informal supply elasticity, η | 2.0 | this model |
| Share of official foreign exchange allocated to fuel; to chemicals and fertiliser, φ (2019) | 0.355; 0.402 | this model; calibrated to the SAM |
| Pass-through of premium changes to import prices, θ | 1 (sensitivity 0 to 1) | reference value = GEM-Core |
| Share of the premium rent that is a real resource cost, ω | 0.25 (sensitivity 0 to 0.5) | this model; Krueger (1974); 0 = GEM-Core |
| Speed at which resources leave rent-seeking, λ | 1/3 a year | this model |
| Infrastructure: cut in transport margins and transport inputs; in marketing margins on farm and food products | 15 %; 10 % at full effect (2035) | this model; illustrative |
| Infrastructure: cut in fuel per unit of electricity (hydropower) | 50 % at full effect (2035) | this model; illustrative |
| Human development: labour force moved from primary to secondary education by 2040 | 2 % of each sex's labour force | this model; illustrative |
| Mining: capital brought in by FDI | mining capital rises with the resource, ×3 by 2035 | this model |

The elasticities are standard values rather than Burundi estimates. Section A.8 lists them among the main sources of uncertainty.

### A.2.3 Exogenous dynamic paths

The model is solved annually from 2019 to 2040. Its growth is driven by exogenous paths for:

- **Population and labour.** Population grows 2.7 % a year to 2025 and 2.3 % a year over 2026–40. The working-age population grows about 3.1 % a year over 2026–40. Labour endowments by type follow demographic projections.
- **Target real GDP growth**, used only in the calibration pass (A.4.3). Observed rates through 2025, then 2.22 % a year.
- **World prices.** Constant in foreign currency in the reference path. In the backcast, INSBU's realised trade deflators in US dollars.
- **Fiscal and external ratios**: grants, government and non-government foreign financing, FDI, remittances per capita. These follow the closure rules in A.4.
- **Interest rates**: 3 % on domestic public debt, 3.5 % on external public debt.
- **Import-quota ceilings**: fuel, and chemicals and fertiliser, set by the foreign-exchange rule (A.3.6).

### A.2.4 Calibration procedure

Share and scale parameters are calibrated so that the model reproduces the 2019 SAM exactly. At the calibrated point every equation holds to within 5 × 10⁻¹³.

LES parameters are derived from the income elasticities and the Frisch parameter. The dynamic path is calibrated in two passes, as in GEM-Core:

1. **Calibration pass.** Real GDP is pinned to its target path, and an economy-wide TFP scalar is solved for.
2. **Reference pass.** That productivity path is frozen and GDP becomes endogenous. By construction the reference pass reproduces the calibration pass.

All scenario parameters are then re-based on the reference solution, so scenarios are deviations from the reference path.

---

## A.3 Model equations

**Notation.** Subscripts: *a* activities, *c* commodities, *f* factors, *h* households, *i* institutions, *t* years. Upper-case symbols are endogenous variables and lower-case or barred symbols are parameters. A superscript 0 denotes a base-year (or reference-path) value. The time subscript is dropped where no confusion arises. Code names are given in parentheses where they help the reader locate an equation in the implementation. The system has about 2,700 equations and as many endogenous variables per year.

### A.3.1 Production and factor demand

Each activity produces a quantity $QA_a$. It combines a value-added composite, a fixed-proportion natural resource and intermediate inputs.

**Value added.** Value added is a CES aggregate of aggregate labour, private capital and land. $VXI_a$ is value added per unit of output (A.3.2):

$$QA_a \, VXI_a \;=\; TFP_a \, \phi^{va}_a \left[ \sum_{f} \delta^{va}_{f,a} \left(FPRD_{f,a}\, QF_{f,a}\right)^{-\rho^{va}_a} \right]^{-1/\rho^{va}_a} \qquad \text{(A.1)}$$

Factor demands equate the activity-specific factor price to the value of the marginal product:

$$WFA_{f,a} \;=\; PVA_a \, QA_a VXI_a \; \frac{\delta^{va}_{f,a}\, FPRD_{f,a}^{-\rho^{va}_a}\, QF_{f,a}^{-\rho^{va}_a-1}}{\sum_{f'} \delta^{va}_{f',a}\left(FPRD_{f',a} QF_{f',a}\right)^{-\rho^{va}_a}} \qquad \text{(A.2)}$$

$$WFA_{f,a} \;=\; WF_f \, WFDIST_{f,a}\,(1+tfa_{f,a}) \qquad \text{(A.3)}$$

Here $WF_f$ is the economy-wide factor price and *WFDIST* the sector differential. Aggregate labour is itself a CES of four education levels (σ = 4). Each education level is a CES of male and female labour (σ = 2), with first-order conditions of the same form. *FPRD* is a factor-specific productivity index. It carries the human-capital effect of the human-development scenario (A.5.6).

**Natural resource.** The mining resource enters in fixed proportion to output:

$$QF_{r,a} \;=\; ifa_{r,a}\, QA_a VXI_a \qquad \text{(A.4)}$$

**Productivity.** Total factor productivity combines an exogenous path, an economy-wide scalar solved for in the calibration pass, an optional sector-group shifter, a small openness externality, and the resource cost of rent-seeking:

$$TFP_{a,t} \;=\; tfp^{ex}_{a,t}\,\left(1 + TFPSCAL_t \, tfp01_a\right)\, TFPGRP_{g(a),t}\,\left(\frac{TRDGDP_t}{TRDGDP^0}\right)^{\epsilon^{trd}_a}\, RW_t \qquad \text{(A.5)}$$

Each term plays a different role:

- *TFPSCAL* is solved for in the calibration pass and frozen afterwards.
- *TFPGRP* is a sector-group shifter (this model). It is used only to back sector productivity out of observed sector growth in the backcast, and is otherwise equal to 1.
- *TRDGDP* is trade over GDP at base-year prices, with $\epsilon^{trd} = 0.01$.
- $RW_t$ is the productivity cost of rent-seeking (this model), defined below.

**The premium rent as a real cost (this model).** In GEM-Core the premium rent (A.19) is a pure transfer to private capital, so removing it only redistributes income. The rent-seeking literature (Krueger 1974) argues that such rents are at least partly competed away: resources go into queuing for official foreign exchange, informal intermediation and lobbying, and produce nothing. A share ω of the rent is treated as a real resource cost. In reduced form, productivity is lower by ω times the rent's share of GDP. Resources move out of rent-seeking gradually, so the effective rent share adjusts by a fraction λ a year:

$$RW_t = \frac{1 - \omega\, RSH^{eff}_t}{1 - \omega\, RSH_{2019}}, \qquad RSH^{eff}_t = (1-\lambda)\, RSH^{eff}_{t-1} + \lambda\, \frac{\sum_c YPREXRT_{c,t}}{GDPMP_t} \qquad \text{(A.5a)}$$

$RW$ equals 1 in 2019, so the base year is unchanged. The central value is ω = 0.25, with λ = 1/3 (most of the adjustment within three years). With the reference application's premium of 2.5, the rent is 17 % of GDP in 2019 and 21 % by 2040 in the reference path. So ω = 0.25 means that resources worth about 4–5 % of GDP are used up in rent-seeking. This is within the range of Krueger's estimates for India and Turkey (rents of about 7 % and 15 % of GNP, largely dissipated). ω = 0 reproduces GEM-Core and is used in the replication mode.

Outside the calibration pass, public capital adds an additive productivity term for targeted activities. This is how the infrastructure scenario raises output (A.5.5):

$$QA_a VXI_a \;=\; \left[\text{(A.1) right-hand side}\right] \;+\; \sum_{k} mpk_{a,k,t}\left(QFINS_{k,t} - QFINS^0_{k,t}\right) \qquad \text{(A.6)}$$

Here *mpk* is the marginal product implied by the assumed rate of return on public capital. It is zero in the reference path.

**Intermediates.** Intermediate demand is Leontief, except for fuel (A.3.2). In scenarios the coefficients *ica* can follow a path, for example when roads cut transport inputs or hydropower cuts fuel use in electricity (A.5.5). The trade and transport margin coefficients can too.

$$QINT_{c,a} \;=\; ica_{c,a}\, QA_a \cdot \begin{cases} FXI_a & c = \text{fuel} \\ 1 & \text{otherwise}\end{cases} \qquad \text{(A.7)}$$

**Producer prices.** The value-added price is what remains of the activity price after three deductions: the activity tax, a fixed revenue share $shg_a$ that remunerates public capital, and intermediate and resource costs. Quota rents rebated to the activity (A.3.6) are added:

$$PA_a\left(1-ta_a-shg_a\right) + \frac{YPRQMBAR_{a}}{QA_a} \;=\; PVA_a VXI_a + \sum_{r} WFA_{r,a}\, ifa_{r,a}VXI_a + \sum_c PQD_{c,a}\, ica_{c,a}\,[FXI_a]_{c=\text{fuel}} \qquad \text{(A.8)}$$

**Multi-product activities.** Activity output is split into commodities in fixed proportions, $QXAC_{a,c} = \theta_{a,c} QA_a$. Supplies of the same commodity from different activities are aggregated by CES, and $PA_a = \sum_c \theta_{a,c} PXAC_{a,c}$.

### A.3.2 Substitution between fuel and value added (this model)

In the reference application fuel is a Leontief input. A cut in fuel supply then has to be absorbed entirely by price, and a 25 % cut, like Burundi's in 2024, has no solution. I therefore allow each fuel-using activity to trade fuel against value added through a CES with elasticity $\sigma^F$. $FXI_a$ is fuel per unit of output and $VXI_a$ is value added per unit of output, both equal to 1 at base prices:

$$\frac{FXI_a}{VXI_a} \;=\; \left[\frac{PQD_{fuel,a}/PQD^0_{fuel,a}}{PVAC_a/PVAC^0_a}\right]^{-\sigma^F} \qquad \text{(A.9)}$$

$$\left(1-s^F_a\right) VXI_a^{\rho^F} + s^F_a\, FXI_a^{\rho^F} \;=\; 1, \qquad \rho^F = \frac{\sigma^F-1}{\sigma^F} \qquad \text{(A.10)}$$

Here $s^F_a$ is the base-year fuel share of value added plus fuel (Table A.3), and $PVAC_a$ is the unit cost of the value-added composite including the resource. Saving fuel costs value added per unit of output. With $\sigma^F \to 0$ the reference application's Leontief technology returns.

### A.3.3 Factor markets

**Labour.** For each of the eight labour types, employment equals supply net of unemployment:

$$QFS_f\,(1-UERAT_f) \;=\; \sum_a QF_{f,a} \qquad \text{(A.11)}$$

Wages and unemployment move along a wage curve (Blanchflower and Oswald 1994):

$$\frac{WF_f}{CPI} \;=\; \frac{WF^0_f\, fprdindex_{f,t}}{CPI^0} \left(\frac{UERAT_f}{UERAT^0_f}\right)^{\eta^{wf}}, \qquad \eta^{wf} = -0.5 \qquad \text{(A.12)}$$

Labour is mobile across activities within a type, and *WFDIST* is fixed.

**Capital and land.** Capital and land are sector-specific within a year. The stock is predetermined, and *WFDIST* clears each activity's market.

**The mining resource.** The resource is mobile and fully employed by default. In the backcast and the mining scenario it is on an **idle-resource closure**: utilisation is free and the rent is held at its reference path, as a complementarity (A.4.4):

$$0 \le UERAT_r \;\perp\; WF_r \ge WF^{res}_r \qquad \text{(A.13)}$$

Output that cannot be sold at the going rent stays in the ground, instead of driving the rent to zero.

### A.3.4 International trade

**Imports.** Domestic supply of each commodity is an Armington CES of imports and domestic output:

$$QQ_c \;=\; \phi^q_c\left[\delta^m_c QM_c^{-\rho^q_c} + \delta^d_c QD_c^{-\rho^q_c}\right]^{-1/\rho^q_c}, \qquad \frac{QM_c}{QD_c} \;=\; \left[\frac{PDD_c}{PM_c}\cdot\frac{\delta^m_c}{\delta^d_c}\right]^{\frac{1}{1+\rho^q_c}} \qquad \text{(A.14)}$$

**Exports.** Output is split between exports and domestic sales by a CET:

$$QX_c \;=\; \phi^x_c\left[\delta^e_c QE_c^{\rho^x_c} + \delta^s_c QD_c^{\rho^x_c}\right]^{1/\rho^x_c}, \qquad \frac{QE_c}{QD_c} \;=\; \left[\frac{PE_c}{PDS_c}\cdot\frac{\delta^s_c}{\delta^e_c}\right]^{\frac{1}{\rho^x_c-1}} \qquad \text{(A.15)}$$

World prices *PWM*, *PWE* are exogenous (small country). In the backcast, and in scenarios that pin them, export volumes can be set exogenously instead.

**Commodity prices.** Composite and demand prices include trade and transport margins, product taxes and, where they apply, the quota rent. Demand prices can differ across users *d*:

$$PQD_{c,d} \;=\; PQS_c\,(1+tq_c)(1-subc_{c,d})(1+tvac_{c,d})$$

### A.3.5 The dual exchange-rate market

Let *EXR* be the official exchange rate (BIF per unit of foreign currency) and *PREXR* the ratio of the parallel rate to the official rate. A share $shrom_c$ of imports of *c* is paid for with official-rate foreign exchange, and a share $shroe_c$ of export receipts is surrendered at the official rate:

- *shrom* = 1 for fuel and chemicals, and 0 for all other imports;
- *shroe* = 0.75 for agricultural exports (coffee and tea), and 1 otherwise.

Import and export prices are:

$$PM_c \;=\; \left(1+tm_c+PRQMBAR_c\right) PWM_c\, EXR\left[(1-shrom_c)\,PREXR^{m} + shrom_c\right] + \sum_{c'} PQD_{c'}\, icm_{c',c} \qquad \text{(A.16)}$$

$$PE_c \;=\; \left(1-te_c\right) PWE_c\, EXR\left[(1-shroe_c)\,PREXR + shroe_c\right] - \sum_{c'} PQD_{c'}\, ice_{c',c} \qquad \text{(A.17)}$$

$$PREXR^{m}_t \;=\; P^{anc}_t + \theta\left(PREXR_t - P^{anc}_t\right) \qquad \text{(A.18)}$$

Equation (A.18) is this model's pass-through parameter; $PREXR^{m}$ is the premium that reaches import costs. θ = 1 reproduces GEM-Core exactly. θ < 1 passes only part of a change in the premium, measured from an anchor path $P^{anc}_t$, into the prices importers pay. The anchor is the reference path in scenarios and the 2019 level in the backcast. The same effective premium is applied to the premium rent and the tariff base, so the accounts stay consistent.

**The premium rent.** The rent created by the gap between the two rates is:

$$YPREXRT_c \;=\; (1-shrom_c)(PREXR^{m}-1)\,EXR\,PWM_c QM_c \;-\; (1-shroe_c)(PREXR-1)\,EXR\,PWE_c QE_c \;+\; [\text{informal fuel rent}]_{c=\text{fuel}} \qquad \text{(A.19)}$$

It is allocated to accounts by fixed shares. In the Burundi data the whole rent goes to private capital, and reaches households through their capital-income shares. A share ω of it is also a real resource cost, through productivity (A.5a).

### A.3.6 Import quotas and the foreign-exchange rationing of fuel

Fuel and chemicals are subject to import quotas. Each quota is a complementarity: either the ceiling binds and a rent rate *PRQMBAR* opens between the domestic and the border price, or it is slack and the rent is zero:

$$QMBAR_{c,t} - QM_c \;\ge\; 0 \;\perp\; PRQMBAR_c \;\ge\; 0 \qquad \text{(A.20)}$$

The rent $YPRQMBART_c = PRQMBAR_c\, PWM_c\, EXR\, QM_c$ is rebated to the users of the commodity (households, activities, government, investment) in proportion to their use. The base-year rent is zero: the 2019 quota binds exactly at the 2019 volume.

**The fuel ceiling follows foreign-exchange availability (this model).** Official fuel imports are paid for with foreign exchange that BRB sells at the official rate. The fuel ceiling is therefore BRB's allocation of official foreign exchange to fuel, divided by the world price:

$$QMBAR_{fuel,t} \;=\; \varphi_t\,\frac{OFX_t}{PWM_{fuel,t}}, \qquad OFX_t \;=\; \sum_c shroe_c\, PWE_c\, QE_c \;+\; TR_{gov,row,t} \;+\; NFFG_t \qquad \text{(A.21)}$$

Official foreign exchange *OFX* has three sources: exporters' surrendered receipts, grants to the government, and the government's net foreign borrowing, all in foreign currency.

The share φ is 0.355 in 2019, calibrated to the SAM. Where the ceiling is observed (BRB's customs volumes in the backcast), the observed volume is used, and the share it implies is carried forward afterwards. A fall in export receipts or aid, or a rise in the world price of fuel, therefore tightens the pump quota directly.

**Chemicals and fertiliser.** These are the other imports cleared at the official rate. Together with fuel they are 28.6 % of imports in the SAM, against BRB's statement of about 30 %. They follow the same rule, with their own share of the same pool: $QMBAR_{chem,t} = \varphi^{chem}_t\, OFX_t / PWM_{chem,t}$. The 2019 share is 0.402, so fuel and chemicals together take 76 % of official foreign exchange in 2019.

### A.3.7 Informal fuel supply (this model)

When the pump shortage is deep enough, fuel is imported informally from the Democratic Republic of the Congo and Tanzania. Informal fuel is outside the quota. It is bought with parallel-market foreign exchange at a markup μ, and its cost rises with volume:

$$PM^{inf}_t \;=\; PWM_{fuel}\,(1+\mu)\,EXR\,PREXR\left(1+\frac{QMI}{QM^0_{fuel}}\right)^{1/\eta} + \text{margins} \qquad \text{(A.22)}$$

$$QMI \;\ge\; 0 \;\perp\; PM_{fuel} \;\le\; PM^{inf}_t \qquad \text{(A.23)}$$

Informal fuel is always dearer than pump fuel. It flows only once the quota rent has raised fuel's market price to the informal cost, and from then on the informal price caps it.

The costs and rents are split as follows:

- Only $PWM_{fuel}(1+\mu^{abroad})$ per unit is paid abroad, with $\mu^{abroad} = 0.1$ for transport.
- The parallel premium, the smugglers' domestic margin and the rising-cost markup are domestic rents, booked with the premium rent.

Informal fuel is added to domestic supply and to the current account, but not to recorded imports.

### A.3.8 Households and other institutions

**Institutional income.** Each institution receives:

- its shares of factor income, net of factor taxes;
- transfers from the government (indexed to the CPI) and from abroad (in foreign currency);
- inter-institutional transfers;
- its share of the premium and quota rents.

**Saving.** Saving combines a CPI-indexed constant with a marginal propensity:

$$SAV_i \;=\; \bar\alpha_i\, CPI + MPS_i\,(1-ty_i)\, YI_i, \qquad MPS_i = mps_i\, MPSSCAL_t + MPSADJ_t \qquad \text{(A.24)}$$

Household consumption spending is disposable income less saving and transfers paid: $EH_h = (1-ty_h)YI_h - SAV_h - \sum_i TRII_{i,h}$.

**Consumption demand.** Consumption follows a linear expenditure system with subsistence quantities per capita:

$$PQD_{c,h} QH_{c,h} \;=\; PQD_{c,h}\,\gamma_{c,h} POP_{h,t} + \beta_{c,h}\left(EH_h - \sum_{c'} PQD_{c',h}\,\gamma_{c',h} POP_{h,t}\right) \qquad \text{(A.25)}$$

**Household fuel switching (this model).** Of each household's LES fuel budget, a share $HXI_h$ is spent on fuel and the rest on firewood and charcoal (part of the agricultural commodity):

$$HXI_h \;=\; \left[\frac{PQD_{fuel,h}/PQD^0_{fuel,h}}{PQD_{agr,h}/PQD^0_{agr,h}}\right]^{-\varepsilon} \qquad \text{(A.26)}$$

The total budget is unchanged. *HXI* = 1 at base prices.

**Transfers.** Transfers between institutions follow rules set by the closure (A.4). Examples are remittances per capita, and grants as a share of GDP or in real terms.

### A.3.9 Government

**Revenue.** Government revenue *YG* is the sum of:

- direct taxes;
- product, activity, import and export taxes;
- grants (×*EXR*);
- factor income from public capital and the resource;
- the government's share of rents.

**Spending.** Spending *EG* is consumption $\sum_c PQD_{c,g}QG_c$, plus transfers, subsidies and VAT rebates. Tax rates and real consumption are scaled from base paths:

$$ty_i = ty^{b}_{i,t}\left(1+ty01_i\, TYSCAL_t\right), \qquad QG_c = qg^{b}_{c,t}\left(1 + qgc01_c\, QGSCAL_t\right) \qquad \text{(A.27)}$$

**Budget identity.** The primary deficit is financed abroad or at home:

$$EG + INVVALG - YG \;=\; EXR\cdot NFFG + NDFG \qquad \text{(A.28)}$$

**Debt.** Debt stocks accumulate from borrowing including interest:

$$GDEBT_t = GDEBT_{t-1} + GBOR_{t-1}, \quad GBOR = NDFG + r^{d}\, GDEBT$$

$$FDEBT_{t} = FDEBT_{t-1} + FBOR_{t-1}, \quad FBOR = NFFG + r^{f}\, FDEBT$$

Which item clears the budget is a closure choice (A.4.2).

### A.3.10 Investment, saving and the balance of payments

**Investment by capital type.** Real investment by each capital-owning account (government, non-government, foreign) and capital type is:

$$DKINS_{i,k} \;=\; dk^{b}_{i,k,t}\, ISCAL_k\left(1 + IADJ_i\, iadj01_k\right) + ddk_{i,k,t} \qquad \text{(A.29)}$$

*ddk* is an additive shifter used by the infrastructure scenario. Investment demand by commodity follows fixed capital-composition coefficients, $QINV_c = \sum_{i,k} capcomp_{k,c}\, DKINS_{i,k}$. The price of capital is $PK_k = \sum_c capcomp_{k,c} PQD_{c,k}$.

**Financing.** Non-government investment is financed by private saving, non-government foreign financing and the rents credited to investment, less government domestic borrowing:

$$INVVAL \;=\; \sum_i SAV_i + EXR\cdot NFFINS - NDFG + \text{rents to investment} \qquad \text{(A.30)}$$

Government domestic borrowing therefore crowds out private investment one-for-one in nominal terms.

**Balance of payments.** The current account (in foreign currency) and the capital account are:

$$\sum_c PWE_c QE_c + \text{transfers from abroad} + SAVF \;=\; \sum_c PWM_c QM_c + PWM_{fuel}(1+\mu^{abroad})QMI + \text{payments abroad} \qquad \text{(A.31)}$$

$$SAVF \;=\; NFFG + NFFINS + INVVALF \qquad \text{(A.32)}$$

Here *SAVF* is foreign saving, *NFFINS* non-government net foreign financing and *INVVALF* FDI. A slack variable, *WALRAS*, is added to (A.32). Walras' law guarantees that it is zero at a solution, and the solver checks that it is.

### A.3.11 Market clearing, prices and GDP

Composite supply equals the sum of household, intermediate, investment, stock, government and margin demand:

$$QQ_c = \sum_h QH_{c,h} + \sum_a QINT_{c,a} + QINV_c + QDST_c + QG_c + QT_c$$

The price indices and the real exchange rate are:

$$CPI = \sum_{c,h} cwts_{c,h}\, PQD_{c,h}, \quad DPI = \sum_c dwts_c\, PDS_c, \quad REXR = EXR / DPI$$

The domestic producer price index *DPI* is the numeraire. Real GDP at factor cost is value added at base-year prices. Real GDP at market prices is final demand at base-year prices, net of recorded and informal imports.

### A.3.12 Dynamics

The model is recursive-dynamic. Each year is a static equilibrium, and the years are linked by the following:

- **Capital accumulation**, by activity:
  $$QF_{k,a,t} = (1-\delta_k)\,QF_{k,a,t-1} + DKA_{k,a,t-1} \qquad \text{(A.33)}$$
- **New capital allocation**, by existing shares tilted towards activities with above-average returns:
  $$DKA_{k,a} = \Big(\sum_i DKINS_{i,k} - T_k\Big)\frac{QF_{k,a}}{\sum_{a'}QF_{k,a'}}\left[1+\kappa\left(\frac{WFA_{k,a}}{WFA^{avg}_k}-1\right)\right] + T_{k,a} \qquad \text{(A.34)}$$

  $T_{k,a}$ is capital financed by FDI targeted to activity *a*. Its value in foreign currency is added to FDI (*INVVALF*). It is zero except in the mining scenario with FDI (A.5.7). $T_k = \sum_a T_{k,a}$.
- **Public capital**, accumulated from government investment at 2.5 % depreciation.
- **Labour supply**, by type, from demographic projections. Labour productivity indices (*fprdindex*) carry human-capital shocks.
- **Debt stocks** (A.3.9), and institutions' capital holdings, which follow their investment.
- **Exogenous paths** for population, world prices, TFP and the fiscal and external ratios.

Expectations are myopic: investment responds to current relative returns, not to expected future ones.

### A.3.13 Glossary of symbols

**Table A.13. Main variables (endogenous unless stated)**

| Symbol | Definition | Equations |
| --- | --- | --- |
| *QA*, *PA* | activity output and price | A.1, A.8 |
| *VXI*, *FXI* | value added and fuel per unit of output (this model; 1 at base prices) | A.9–A.10 |
| *QF*, *QFS* | factor use by activity; factor supply | A.1, A.11 |
| *WF*, *WFA*, *WFDIST* | economy-wide factor price; activity-specific price; sector differential | A.2–A.3 |
| *PVA*, *PVAC* | value-added price; unit cost of the value-added composite | A.2, A.8, A.9 |
| *TFP*, *TFPSCAL*, *TFPGRP* | total factor productivity; calibrated scalar; sector-group shifter | A.5 |
| *FPRD*, *fprd* | factor-specific productivity; labour-productivity index (exogenous) | A.1, A.12 |
| *QINT*, *ica*, *ifa* | intermediate demand; input and resource coefficients (parameters) | A.4, A.7 |
| *UERAT* | unemployment rate by labour type, or idle share of the resource | A.11–A.13 |
| *QQ*, *QM*, *QD*, *QE*, *QX* | composite supply; imports; domestic sales; exports; output | A.14–A.15 |
| *PM*, *PDD*, *PE*, *PDS*, *PQD* | import, domestic demand, export, domestic supply and purchaser prices | A.14–A.17 |
| *PWM*, *PWE* | world import and export prices, USD (exogenous) | A.16–A.17 |
| *EXR*, *REXR* | official exchange rate (BIF per USD); real exchange rate *EXR/DPI* | A.16–A.17 |
| *PREXR*, $PREXR^m$, $P^{anc}$ | parallel/official ratio; premium reaching import costs; anchor path | A.16–A.19 |
| *shrom*, *shroe* | shares of imports and export receipts cleared at the official rate (parameters) | A.16–A.17 |
| *YPREXRT* | premium rent | A.19 |
| *QMBAR*, *PRQMBAR*, *YPRQMBART* | import ceiling; quota rent rate; quota rent | A.20–A.21 |
| *φ*, *OFX* | share of official FX allocated to fuel; official FX inflows (USD) | A.21 |
| *QMI*, $PM^{inf}$ | informal fuel volume; landed informal price | A.22–A.23 |
| *YI*, *SAV*, *MPS*, *EH* | institutional income; saving; marginal propensity to save; consumption spending | A.24 |
| *QH*, *HXI* | household demand; share of the fuel budget spent on fuel | A.25–A.26 |
| *YG*, *EG*, *QG* | government revenue, current spending, real consumption | A.27–A.28 |
| *TYSCAL*, *QGSCAL* | closure scalars on tax rates and real government consumption | A.27 |
| *NFFG*, *NDFG*, *GDEBT*, *FDEBT* | government net foreign and domestic borrowing; domestic and external debt | A.28 |
| *DKINS*, *ISCAL*, *IADJ*, *ddk* | real investment by account and capital type; scalars; additive public investment | A.29 |
| *INVVAL*, *NFFINS*, *INVVALF*, *SAVF* | private investment; private foreign financing; FDI; foreign saving | A.30–A.32 |
| *DKA*, *QFINS* | new capital by activity; institutional capital holdings | A.33–A.34, A.6 |
| *CPI*, *DPI* | consumer and domestic producer price indices (*DPI* is the numeraire) | A.3.11 |
| $RW$, $RSH^{eff}$ | productivity cost of rent-seeking; effective premium-rent share of GDP | A.5, A.5a |

Greek letters and lower-case symbols are parameters. The main values are in Table A.4.

---

## A.4 Closure and solution

### A.4.1 The closure-rule system

GEM-Core chooses closures through rule parameters in the data rather than in code. This model keeps that design. Table A.5 lists the rules, with the choice made in the reference path and in the scenarios.

**Table A.5. Closure rules**

| Block | Options | Reference path | Scenarios |
| --- | --- | --- | --- |
| Numeraire | CPI / domestic producer price index / official exchange rate | producer price index | same |
| Government balance (*govclos*) | 1 direct-tax rate clears; 2 domestic borrowing clears; 3 foreign borrowing clears | rule 1: direct-tax rate | rule 1: direct-tax rate |
| Government receipt and spending items | 1 real or rate fixed; 2 share of GDP fixed; 3 share of absorption fixed | tax rates fixed; grants, foreign and domestic financing, consumption and investment as GDP shares | all rule 1 (real paths, fixed rates) |
| Non-government payments | same three options | FDI, private saving, foreign financing, private investment as GDP shares | rule 1 |
| Saving–investment (*siclos*) | 1 investment clears; 2 savings rate clears | rule 2: savings rate | rule 1: investment |
| Rest of world (*rowclos*, per year) | 1 real exchange rate clears; 2 private foreign financing clears; 3 government foreign financing clears; 4 parallel premium clears | rule 2 through 2027 (private financing), rule 4 from 2028 (premium) | rule 1: real exchange rate |
| Labour | wage curve, mobile across sectors | wage curve | same |
| Capital, land | sector-specific | same | same |
| Mining resource | mobile, full employment; or idle-resource complementarity | full employment | idle-resource in `combi` |

Three features of this table matter most for reading the results:

1. **In the reference path the premium is exogenous through 2027 and then clears the foreign-exchange market.** Before 2028, private foreign financing absorbs any external gap. From 2028 financing is on its exogenous path and the parallel premium rises endogenously, from 2.5 to about 3.4 by 2040 in the reference application.
2. **Scenarios switch to a real-exchange-rate closure** (*rowclos* 1): foreign financing is exogenous, and the real exchange rate adjusts. This is the classic structuralist closure for a foreign-exchange-constrained economy. It is also why export responses in the scenarios are large.
3. **Scenarios fix government spending in real terms and let investment clear**. The reference path instead fixes GDP shares and lets the savings rate clear. So a scenario's gains show up in investment, not in the savings rate.

### A.4.2 Closure dependence of policy results

The same shock has different incidence under different closures:

- **A fall in grants.** Under *govclos* 1 the direct-tax rate rises. Under *govclos* 2 domestic borrowing rises and crowds out private investment (A.30). Under *govclos* 3 external borrowing rises and the fall is absorbed in the external accounts.
- **An improvement in the terms of trade.** Under *rowclos* 1 the real exchange rate appreciates. Under *rowclos* 2 private foreign financing falls. Under *rowclos* 4 the parallel premium falls.
- **A foreign-exchange shock.** Through (A.21), a shock to *OFX* also moves the fuel ceiling, whichever closure is in force.

I report the closure with every result, and Section A.7.2 lists the closure each new simulation should use.

### A.4.3 Two-pass dynamic calibration

As described in A.2.4:

1. The calibration pass pins real GDP and solves for $TFPSCAL_t$.
2. The reference pass frees GDP and fixes TFP at those values, reproducing the same path.
3. All "0" parameters (reference quantities and prices used by scenario shocks and rules) are then re-based on the reference solution.

Skipping the re-basing changes scenario results even though the reference path itself still validates. This is an easy error to make in GEM-Core-type models.

### A.4.4 Solution method

GAMS solves the model as a mixed complementarity problem, in which each equation is paired with a variable and bounds handle inequalities. This implementation solves a **square system of equalities for each year**, with lagged stocks predetermined. The steps are:

1. **Exogeneity.** The closure rules of A.4.1 fix the exogenous variables. They are read from the data, so changing a closure in the data needs no code change.
2. **Equation–variable pairing.** A declared pairing records which variable each equation determines. A maximum bipartite matching on the numerical Jacobian completes the pairing and verifies it is perfect and non-singular (2,700 × 2,700 in 2019). An unmatched variable would otherwise be silently frozen, with the imbalance absorbed by the Walras slack.
3. **Newton's method**, with exact derivatives from CasADi's automatic differentiation (Andersson et al. 2019). It falls back to Levenberg–Marquardt and trust-region least squares, warm-starts from the previous year, and uses continuation (the shock phased in over several steps) for large shocks. The convergence tolerance is 10⁻⁶ on every residual.
4. **Complementarities by active-set sweep.** Three inequality blocks are solved this way: the import quotas (A.20), the idle mining resource (A.13) and informal fuel (A.23). The path is solved with each cell on an assumed branch. Cells on the wrong branch are then switched, and the path is re-solved until no cell changes. A cell is on the wrong branch if it has a negative quota rent, imports above a slack ceiling, over-employment of an idle resource, a positive informal-fuel arbitrage gap, or negative informal volume. In practice this settles in two or three sweeps. Applied to the reference path, it finds the 2020 quotas slack, as PATH does, and reproduces the reference application's 2020 external accounts to four decimals.

A full base-plus-reference solution for 2019–2040 takes about 80 seconds. A scenario takes about 30 seconds on top. An independent audit re-checks every solution after the fact (A.6.5).

---

## A.5 Transmission mechanisms

This section traces how each class of policy or shock works through the model:

- the first-round price or quantity change;
- the market-clearing adjustments;
- the dependence on the closure;
- the dynamic effects.

Table A.6 summarises it.

**Table A.6. Policy levers and their channels**

| Policy area | Model instrument | Primary channel | Most sensitive to |
| --- | --- | --- | --- |
| Exchange-rate unification | premium path *PREXR*; rest-of-world closure | price of parallel-rate imports; premium rent; real exchange rate | pass-through θ; share of imports at the parallel rate |
| Foreign-exchange allocation to fuel | fuel share φ; official inflows *OFX* | pump-quota rent; informal fuel; substitution | $\sigma^F$, ε, μ; closure of the external account |
| Trade policy | tariff and export-tax scalars; quota ceilings | import and export prices; quota rents | Armington and CET elasticities |
| Fiscal policy | spending and tax rules; government closure | tax burden or crowding out | *govclos*; saving–investment closure |
| Public investment | additive public investment *ddk*; return on public capital | capital stock, productivity of targeted sectors | assumed return (*mpcapgov*); financing closure |
| Human development | government education and health demand; labour productivity | demand for services now, labour productivity later | size and lag of the productivity effect |
| Natural resources | endowment of the mining resource | mining output, exports, rents | idle-resource closure; ownership shares |
| External shocks | world prices; grants; remittances; FDI; foreign financing | income, foreign exchange, fuel ceiling | *rowclos* |
| Productivity and climate | sector TFP shifters | output and relative prices by sector | Armington and CET elasticities; LES |

### A.5.1 Exchange-rate unification

Unification lowers the premium *PREXR* to 1. Several things follow:

1. **Import prices fall.** Imports bought at the parallel rate are 71 % of the total, and their prices fall by the ratio of the premium (A.16). This lowers the cost of imported consumer goods, of imported intermediates and, through the capital-composition coefficients, of capital goods.
2. **The premium rent disappears** (A.19). As a transfer, it accrued to private capital, so capital income falls, and lower consumer prices roughly offset that for households. As a real cost (A.5a), its disappearance frees the resources used in rent-seeking. Productivity rises by about ω × 21 % ≈ 5 % by 2040 at ω = 0.25, phased in over about three years.
3. **Exporters lose.** Exporters of coffee and tea who sold a quarter of their receipts at the parallel rate receive less in BIF (A.17).
4. **The external account must clear.** Foreign financing is exogenous under the scenario closure, so the extra import demand requires a real depreciation. The real exchange rate roughly doubles by 2040 in `uni`. That depreciation drives a strong export response through the CET.
5. **Investment rises.** Capital goods are cheaper and investment clears the saving–investment balance, so real investment rises. Higher capital accumulation raises GDP growth over time.
6. **The quotas are removed** at the same time, so fuel and chemicals are no longer rationed.

**How the gain is attributed.** Two channels carry the gain: cheaper imports (channel 1) and the end of rent-seeking (channel 2). Table A.10c shows each:

- with ω = 0 (GEM-Core), unification adds +1.05 points a year to GDP growth, and GDP is 16 % above base by 2040;
- with ω = 0.25 it adds +1.49 points, and GDP is 23 % above base by 2040;
- with ω = 0.5 it adds +1.95 points, and GDP is 32 % above base by 2040.

Both channels need the premium to reach the prices importers pay: at θ = 0 the premium on imports, and so the rent, does not change, and the gain is nil (Table A.11). The 2024 episode supports substantial pass-through: the premium rose from 1.60 to 2.17 and real imports fell 10 %, which the model matches only near θ = 1.

### A.5.2 Foreign-exchange allocation and fuel

Under (A.21), official fuel imports are a share φ of official foreign-exchange inflows, divided by the world fuel price. So the following deepen the pump shortage without any change in policy:

- a fall in export receipts;
- a fall in grants or in government foreign borrowing;
- a rise in the world price of fuel.

The adjustment runs in stages. As the shortage deepens, the quota rent raises fuel's market price, and substitution absorbs part of the cut:

- firms substitute value added for fuel (A.9–A.10), which lowers output per unit of factors in electricity, transport and construction;
- households switch spending to firewood and charcoal (A.26).

Once the market price reaches the informal cost, fuel is imported informally at a price that rises with volume (A.22–A.23). The informal premium and markup become domestic rents, and only the world price plus transport is paid abroad.

The policy levers are:

- **the share φ.** BRB can prioritise fuel over other uses of official foreign exchange. The backcast shows it did so in 2022–23 (A.6.3);
- **the size of *OFX***, through export performance or financing;
- **the access regime.** Moving fuel imports from the official to the parallel market, by lowering $shrom_{fuel}$, is equivalent to a pump-price liberalisation at the parallel rate.

**Rationing beyond fuel.** Chemicals and fertiliser follow the same rule, so every import cleared at the official rate is rationed by official foreign exchange. This changes the results very little. The unification gain goes from +1.48 to +1.49 points a year, and 2040 levels change by 0.1 points at most. The chemicals quota was already nearly non-binding in the reference path, and a ceiling that grows with official foreign exchange (about 2.5 % a year) is no tighter.

In the backcast, BRB gave chemicals and fertiliser 40 % of official foreign exchange in 2019, 45–49 % in 2020–22 and 2024, and 37 % in 2023 (Table A.9). Holding the 2019 share would have rationed them more and fits imports worse: 0.3 % a year against 0.6 %.

The rationing that matters for growth is on the parallel market, which imports 71 % of goods. The 2021 import jump (+13 % in tonnes) came with a foreign-exchange inflow while the premium rose. The model clears the parallel market by price, and the backcast's closure lets private foreign financing absorb any gap. Modelling quantity rationing there needs 2019–24 series on private external financing and on BRB's foreign-exchange sales; BRB's published balance of payments stops in 2022. Without those series the result would be set by assumption, so this channel is not in the model (A.8).

### A.5.3 Trade policy

**Tariffs and export taxes.** These enter (A.16) and (A.17) through rate scalars. Their revenue effect goes to the government balance and is cleared by the government closure. Their price effect spreads through Armington substitution and intermediate costs.

**Quota relaxation.** Relaxing a quota lowers its rent, lowers the domestic price, and redistributes the rent that users had captured.

### A.5.4 Fiscal policy

**Higher real government consumption.** It raises demand for public administration, education and health. How it is financed depends on the closure:

- *govclos* 1: higher direct taxes, which lower household saving and consumption;
- *govclos* 2: domestic borrowing, which crowds out private investment one-for-one in nominal terms (A.30) and accumulates domestic debt;
- *govclos* 3: foreign borrowing, which raises foreign saving and, under a real-exchange-rate closure, appreciates the currency.

**Tax changes.** These work through prices (indirect taxes) or disposable income (direct taxes). Grants enter both government revenue and official foreign exchange (A.21), so aid cuts have a fiscal and a fuel-supply effect.

**Budget support accompanying a reform.** Temporary grants of 1 % of GDP in 2026–27 and 0.5 % in 2028 (`uni+bs`), used to lower the direct-tax rate, cushion the transition: private consumption in 2027 is 2.5 points higher than under `uni`. They leave the 2040 level unchanged, because they are mostly consumed. Spent on public investment instead (`uni+bs-inv`), with the same return as the infrastructure scenario, they add 1.4 points to 2040 GDP (Table A.10c). What matters for the long run is what the financing buys, not its amount.

### A.5.5 Public investment

The infrastructure scenario adds public investment of 2 % of reference GDP. It is phased in over 2026–29, held to 2035 and phased out by 2040. The extra public capital raises output in twelve targeted activities (agriculture, manufacturing, utilities, construction and transport) through (A.6), at an assumed return of 65 %. The effect builds with the stock and decays at 2.5 % a year after the programme ends. The financing closure determines who pays in the short run.

**What the infrastructure builds (`uni+inf-m`, `uni+inf-x`).** The published scenario raises output through productivity only. Two further effects of roads and power are well documented, and I add them, phased in with the public capital stock:

- *Transport costs.* Burundi is landlocked and relies on the Dar es Salaam corridor. Roads and corridor works cut transport margins and transport inputs by 15 % by 2035. Feeder roads cut marketing margins on farm and food products by 10 %.
- *Hydropower.* Fuel is 45 % of electricity's cost in the SAM. Hydropower replacing diesel generation halves fuel use per unit of electricity by 2035.

Both save intermediate inputs. The factor-cost GDP measure values value added at base-year unit values, so it does not register them. They show up in final demand. At ω = 0.25, the transport and marketing cuts add 1.2 points to 2040 real GDP at market prices and 1.3 points to private consumption. Hydropower adds a further 0.2, because electricity is only 0.3 % of value added (Table A.10d).

### A.5.6 Human development

The human-development scenario raises government demand for education and health services. It closes Burundi's per-capita spending gap to the 90th percentile of low-income countries (factors 1.62 and 1.85), phased in over 2026–35. Labour productivity (*FPRD* in A.1) rises with the implied gain in the human capital index, but only from 2031 and gradually. So the scenario first shifts demand towards services, which are labour-intensive and non-traded, and raises labour productivity only later.

**The education mix (`uni+inf+hd-x`).** The model's labour is counted in persons, in eight types. Secondary-educated workers earn 3 to 10 times what primary-educated workers earn. Part of what extra education spending buys is more secondary graduates among new workers. I move 2 % of each sex's labour force from primary to secondary education by 2040, linearly from 2031 when the first cohorts enter work. That is roughly 7 more points of secondary completion among the cohorts entering over 2031–40. Total labour is unchanged. This adds 1.6 points to 2040 GDP. It may partly overlap the HCI-based productivity gain, since the HCI includes years of schooling, so it is reported as a separate increment that can be dropped (A.8).

### A.5.7 Natural resources

The mining scenario triples the endowment of the mining resource over 2026–35. Mining output is 67 % exported, so the expansion earns foreign exchange. That supports a real appreciation or, through (A.21), a larger fuel ceiling. The resource's rent is distributed according to the ownership shares of households, government and the rest of the world.

Under the idle-resource closure (A.13), the extra endowment is used only as far as demand at the reference rent allows. Without that closure, forcing the whole endowment into use drives its price to zero and the equilibrium does not exist. This is a modelling choice with a large effect, and I flag it for discussion (A.8).

**FDI to work the resource (`combi-x`).** On the idle-resource closure, the tripled endowment is used only as far as the mining sector's capital and labour allow, and mining capital is only 1.8 % of the capital stock. A large mining project brings its own capital. I add FDI that raises mining capital in step with the resource, ×3 by 2035, through the targeted-capital term in (A.34). This is roughly 0.5–0.7 % of GDP a year during the build-up, well above the base path of FDI. Because it is FDI, the capital and its profits are foreign-owned. It adds 1.4 points to 2040 GDP at factor cost, 2.0 at market prices, and 15 points to private investment (Table A.10d).

### A.5.8 External shocks

External shocks enter through several parameters:

- **World prices**, through the price indices *pwmindex* and *pweindex*.
- **Grants**, as the government's transfers from abroad.
- **Remittances**, per capita by household.
- **FDI and foreign financing.**

Each of these moves the external account. Its incidence depends on *rowclos*: the real exchange rate, private financing or the parallel premium absorbs it. Grants and export receipts also move the fuel ceiling.

### A.5.9 Labour markets

With the wage curve (A.12), an increase in labour demand raises real wages and lowers unemployment, in proportions set by the elasticity −0.5. Unemployment is reported for eight labour types. Male and female workers substitute imperfectly within an education level, and education levels substitute imperfectly with each other, so shocks that favour skill-intensive sectors raise returns to education.

### A.5.10 Productivity and climate

Sector productivity shocks, such as a drought in agriculture, are applied through the sector-group shifter *TFPGRP* or the exogenous TFP path. Agriculture is 42 % of value added and has an import share of 3.5 %, so a farm-output shock is absorbed mainly through domestic prices. It then reaches households through the LES, where food and agriculture have income elasticities below one.

---

## A.6 Validation

I validate the model in five layers: transcription, replication of the reference application, a backcast against Burundi's 2019–24 outcomes, the calibration of the fuel block, and reproducibility.

### A.6.1 Transcription

At the calibrated base-year point every equation holds with a maximum residual of 4.5 × 10⁻¹³. The per-year system is square with a perfect matching. Every solved year has all residuals below 10⁻⁶ and a zero Walras slack.

### A.6.2 Replication of the reference application

With the extensions switched off (`--gams-replication`), the model reproduces the GEM-Core application to Burundi it was ported from: on the reference path every aggregate matches within 0.09 points a year, each scenario layer adds what it adds in the original to within 0.02 points a year on GDP and 0.1 on most aggregates, and unemployment by labour type matches to two or three decimals. That application's results are for internal World Bank use and are not reported here. Two differences remain: under `uni`, exports and private investment grow about a fifth less than in the original, while GDP, consumption and the exchange-rate path match; and the chemicals sector responds too strongly to the quota's removal.

### A.6.3 Backcast against Burundi's 2019–2024 outcomes

**Design.** Matching GAMS validates the transcription, not whether the model describes Burundi. I therefore ran the model over 2019–24 with the observed exogenous inputs and compared its endogenous outcomes with the data. The observed inputs are:

- real GDP growth (pinned, so the test is of composition);
- the real official exchange rate and the parallel premium;
- realised world trade prices and real export volumes;
- real government consumption and total investment;
- mining output, inventories;
- BRB's fuel and chemicals import volumes.

The main data sources are INSBU's rebased national accounts and supply–use tables (2016–24) and BRB's import statistics and reports.

**Table A.8. Backcast 2020–24, average annual growth (%) unless stated**

| Indicator | Model | Actual |
| --- | ---: | ---: |
| Real GDP (pinned) | 1.9 | 1.9 |
| Real private consumption | 1.2 | 2.4 |
| Real government consumption (fed) | 1.1 | 1.1 |
| Real gross fixed capital formation | 2.2 | 4.6 |
| Real exports | −2.5 | −1.2 |
| Real imports | 0.6 | 5.6 |
| Real value added: agriculture / industry / services | 2.5 / 0.8 / 1.3 | 1.1 / 2.2 / 2.7 |
| Real value added: mining | −3.0 | −4.9 |
| Current account, change 2019–24 (points of GDP) | +1.1 | +1.9 |
| Real imports, 2024 | −11.1 | −10.1 |
| Fuel market price / pump price, 2024 | 3.34 | 3.35 |

**What fits.** The model reproduces:

- the 2019 structure, with every share within two points of INSBU;
- the direction of the current account;
- the collapse of mining in 2020 and 2024;
- the 2024 import fall;
- the 2024 fuel crisis, with a market price 3.34 times the pump price against a black-market estimate of 3.35.

**What does not fit.** Real imports grow too little: 0.6 % a year against 5.6 %. Most of the gap is two episodes:

- **The 2021 volume jump.** Tonnage rose 13 %, alongside a foreign-exchange inflow (the SDR allocation, higher transfers). The model cannot translate that into imports, because foreign-exchange scarcity outside fuel and chemicals is modelled as a price, not as a volume limit.
- **The 2022–23 figures.** Here INSBU's real imports conflict with BRB's flat tonnages, which suggests the import deflator understates the fuel and fertiliser price rises.

The model also spreads growth too evenly across sectors: agriculture grows too fast and services too slowly.

**What the fuel rule reveals.** Holding BRB's observed fuel volumes, the model reports the share of official foreign exchange they absorbed (Table A.9).

**Table A.9. Fuel and official foreign exchange, 2019–24 (model)**

| | 2019 | 2020 | 2021 | 2022 | 2023 | 2024 |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Official FX inflow, USD (2019 = 100) | 100 | 82 | 89 | 104 | 120 | 107 |
| World fuel price, USD (2019 = 1) | 1.00 | 0.83 | 0.96 | 1.51 | 1.80 | 1.76 |
| Fuel share of official FX, % | 35.5 | 37.0 | 38.8 | 48.8 | 49.7 | 43.9 |
| Chemicals and fertiliser share of official FX, % | 40.2 | 48.9 | 45.2 | 44.4 | 37.0 | 45.2 |

BRB sustained fuel imports through the 2022–23 price shock by raising fuel's share of official foreign exchange from about a third to nearly a half. When export receipts fell in 2024 the share fell back and the crisis followed.

At a constant 2019 share, the model would have produced:

- a shortage from 2020;
- a crisis in 2022, with fuel at 2.6 times the pump price and 13 % of it informal;
- 3.54 times the pump price in 2024.

### A.6.4 Calibration of the fuel block

The informal markup μ is calibrated to black-market fuel prices observed in 2024:

- 12,000–12,500 BIF a litre in August–October, against a pump price of 4,000;
- 17,500 in July;
- 22,000 in January 2025.

The full-year ratio is 3.35. It interpolates the missing months and assumes the first half of the year at the August–October level. The plausible range is 3.04 to 3.67.

The volume of informal fuel is not observed, so $\sigma^F$ and ε are set at the low end of the short-run evidence. With $\sigma^F = 0.3$, substitution absorbs the whole shortage and no informal fuel flows, which contradicts reports that many buyers turned to informal channels.

### A.6.5 Reproducibility

Every result in this appendix can be reproduced, and checked, with one command (`runs/reproduce_all.py`). The model package also includes one-click scripts for macOS and Windows, and a user guide for readers who do not use Python.

The code, data and replication files are public at [github.com/kodzovee9/burundi-cge](https://github.com/kodzovee9/burundi-cge), under an MIT licence for the code and CC BY 4.0 for the documents and results. INSBU's national accounts enter there as an extract of the 355 values the model reads from the supply-use tables and integrated accounts; the backcast gives identical results from the extract and from the original files. The check described below, run on this repository exactly as published, reproduces all 41 findings.

**What the command does.**

1. It checks the 2019 base year. The largest equation residual must be below 10⁻⁹; it is 4.5 × 10⁻¹³.
2. It re-runs every simulation, 18 runs in all:
   - the scenario tables;
   - the pass-through grid;
   - the reform scenarios at ω = 0, 0.25 and 0.5;
   - twelve backcast variants;
   - the solution audit.
3. It compares 41 documented findings with the values in this appendix. Each has a tolerance: 0.005 points for growth rates, 0.05 for levels. The findings cover Tables A.8–A.11, A.10c and A.10d, the backcast figures and the fuel calibration.

**An independent audit of every solution.** Convergence of the solver is not taken on trust. The audit re-evaluates every equation of the model at the solution of every year, 2019–2040. It does this for the reference path and every scenario: the ten of the full model. It checks:

- that every equation holds;
- Walras' law;
- that each complementarity is on a consistent branch:
  - binding quotas have non-negative rents, and slack quotas have imports at or below the ceiling and no rent;
  - informal fuel is never negative, and never off while it would be profitable;
  - no idle-resource cell is over-used;
- that no price or quantity sits at the solver's lower bound.

**Results (2 October 2026), on this repository.**

- All 18 runs converged, and the results were identical to those reported here.
- In the audit, the largest equation residual is 3 × 10⁻⁹ and the largest Walras slack 3 × 10⁻⁹. No condition failed.
- All 41 findings reproduced.

**A test from a fresh copy.** The distributed package was unzipped into an empty folder and set up with its own script, as a colleague would. That installed newer versions of the numerical libraries than the model was developed with: CasADi 3.8.1, NumPy 2.5.3 and SciPy 1.18.1. In that copy:

- all runs converged, in 13 minutes on an eight-core laptop;
- all findings reproduced, and the audits passed;
- every result file matched the original to within 8 × 10⁻¹³.

The Windows scripts were not run, since no Windows machine was available.

---

## A.7 Simulations

### A.7.1 Reference scenarios

The reference application defines a sequence of cumulative reform scenarios starting in 2026. Each is a deviation from the reference path. Table A.10a defines them, with average real GDP growth over 2026–40 in the full model (ω = 0.25). Table A.10b lists the sensitivity and historical variants.

**Table A.10a. Definition of the scenarios**

| Scenario | What changes | Timing | Closure | GDP, %/yr |
| --- | --- | --- | --- | --- |
| **base** | Nothing: the reference path. GDP pinned to its target path (about 2.22 % a year after 2025), productivity backed out (A.4.3). The fuel and chemicals quotas follow official foreign exchange at their 2019 shares (0.355 and 0.402). | 2019–2040 | Private foreign financing clears to 2027, then the parallel premium. Savings rate clears. Fiscal items are GDP shares; the direct-tax rate clears the budget. | 2.22 |
| **uni** | Exchange-rate unification: premium halved in 2026, set to 1 from 2027. Fuel and chemicals quota ceilings ×45, so they no longer bind (A.5.1). | 2026: half; 2027 on: full | Real exchange rate clears; foreign financing exogenous. Investment clears saving–investment. Real government spending and fixed tax rates; the direct-tax rate clears the budget. | 3.71 |
| **uni+inf** | uni plus public infrastructure investment of 2 % of reference GDP (from 2040 replacement only). Public capital raises productivity at a 65 % return in 12 activities: agriculture, food, textiles, wood and paper, chemicals, non-metallic minerals, metals, other manufacturing, electricity, water, construction and transport (A.5.5). | Ramps 20 %→100 % over 2026–30, full to 2035, down to 0 by 2040 | as uni | 4.23 |
| **uni+inf+hd** | uni+inf plus a human-development push. Real education spending rises up to ×1.62 and health up to ×1.85, closing the per-capita gap to the 90th percentile of low-income countries. Labour productivity of all types rises up to +8.1 %, the human capital index gap (A.5.6). | Spending 2026–35, then held; productivity 2031–40 | as uni | 4.51 |
| **combi** | uni+inf+hd plus a tripling (+200 %) of the mining natural resource (A.5.7) | Ramps 2026–35, then held | as uni, plus the idle-resource closure for mining (A.13) | 4.85 |
| **combi+** | combi plus a gradual TFP gain reaching 17.5 % by 2040 | to 2040 | as uni | not yet implemented |
| **uni+bs** | uni plus budget support: grants of 1 % of GDP in 2026–27 and 0.5 % in 2028, lowering the direct-tax rate (A.5.4) | 2026–28 | as uni | 3.69 |
| **uni+bs-inv** | uni+bs with the grants spent on public investment, at the uni+inf return, then maintained | 2026–28 | as uni | 3.77 |
| **uni+inf-m** | uni+inf plus transport costs −15 % (margins and inputs) and marketing margins on farm and food products −10 %, phased in with the public capital stock (A.5.5) | full effect 2035 | as uni | 4.22 |
| **uni+inf-x** | uni+inf-m plus hydropower: fuel per unit of electricity −50 % (A.5.5) | full effect 2035 | as uni | 4.23 |
| **uni+inf+hd-x** | uni+inf-x plus the human-development push, with 2 % of the labour force moved from primary to secondary education by 2040 (A.5.6) | shift 2031–40 | as uni | 4.60 |
| **combi-x** | uni+inf+hd-x plus the mining expansion, with FDI raising mining capital in step with the resource (A.5.7) | FDI 2026–35, then maintenance | as combi | 4.99 |

**Table A.10b. Sensitivity and historical variants**

| Variant | Definition |
| --- | --- |
| uni pass-through grid | uni with θ = 1, 0.75, 0.5, 0.25, 0 (A.18). GDP gain over base: +1.49 / +0.86 / +0.47 / +0.19 / −0.02 points a year (Table A.11). |
| Rent-cost share | every scenario with ω = 0 (GEM-Core), 0.25 (default) and 0.5 (A.5a; Table A.10c). |
| GAMS-replication mode | Every scenario with the additions of this model switched off (fuel channels, foreign-exchange-linked fuel quota, pass-through, rent cost). Reproduces the reference application. |
| Backcast 2019–24, default | Observed inputs from INSBU and BRB, including BRB's fuel and chemicals volumes; GDP pinned; 2019 premium 1.58 (A.6.3). |
| Backcast, fuel set by FX | Fuel imports set by foreign exchange alone at the 2019 share, instead of BRB's observed volumes. |
| Backcast, frozen quotas | The reference application's quota ceilings, frozen at 2019 volumes. |
| Backcast, flat premium | Premium held at its 2019 level. |
| Backcast, partial pass-through | θ = 0.75, 0.5 or 0.25. |
| Backcast, government share | Government consumption as a share of GDP rather than a real path. |
| Backcast, model inventories | Inventories grow with GDP instead of following the data. |
| Backcast, mining pinned | Mining output pinned to INSBU, with its productivity backed out. |

Table A.10 reports the results of the full model.

**Table A.10. Reference scenarios, full model, average growth 2026–40 (%)**

| Indicator | Base | uni | uni+inf | uni+inf+hd | combi |
| --- | ---: | ---: | ---: | ---: | ---: |
| Absorption | 1.65 | 3.17 | 3.60 | 3.81 | 4.27 |
| Private consumption | 1.51 | 3.16 | 3.67 | 3.73 | 4.22 |
| Government consumption | 3.17 | 3.17 | 3.17 | 4.73 | 4.73 |
| Private investment | 1.01 | 3.68 | 4.05 | 4.11 | 4.88 |
| Exports | 2.98 | 7.63 | 8.49 | 8.60 | 9.87 |
| Imports | 0.82 | 3.36 | 3.82 | 3.87 | 4.63 |
| GDP at factor cost | 2.22 | 3.71 | 4.23 | 4.51 | 4.85 |
| GDP, 2040 level vs base, % | – | +23.4 | +32.4 | +37.7 | +44.3 |
| Private consumption, 2040 level vs base, % | – | +26.7 | +35.4 | +36.1 | +45.9 |

The model's additions change reference-path growth by at most 0.08 points a year. The rent cost (ω = 0.25) raises the gain from unification from +1.05 to +1.49 points a year, and carries into every later scenario. The 2040 level is the better measure of a reform's size. The growth average starts in 2026, when half the premium cut has already taken effect.

**Table A.10c. GDP gain by rent-cost share ω and with budget support (2040 GDP level vs base, %; growth 2026–40 in brackets, %/yr)**

| Scenario | ω = 0 (GEM-Core) | ω = 0.25 (default) | ω = 0.5 |
| --- | ---: | ---: | ---: |
| uni | +15.9 (3.27) | +23.4 (3.71) | +32.0 (4.17) |
| uni+bs | +15.9 (3.25) | +23.5 (3.69) | +32.0 (4.15) |
| uni+bs-inv | +17.3 (3.34) | +24.8 (3.77) | +33.3 (4.23) |
| uni+inf | +25.0 (3.83) | +32.4 (4.23) | +40.9 (4.66) |
| uni+inf+hd | +30.1 (4.11) | +37.7 (4.51) | +46.4 (4.94) |
| combi | +36.2 (4.44) | +44.3 (4.85) | +53.5 (5.27) |

Base growth is 2.22 % a year in every case. Private consumption in 2027 is 2.5 points above `uni` under `uni+bs` at every ω: the budget support cushions the transition. Source: `runs/reform_boost.py`, `reports/reform-boost-2026-2040.md`.

**Table A.10d. Second group of adjustments: 2040 level vs base, % (ω = 0.25)**

| Scenario | GDP at factor cost | GDP at market prices | Private consumption |
| --- | ---: | ---: | ---: |
| uni+inf (published definition) | +32.4 | +36.2 | +35.4 |
| + transport and marketing margins (uni+inf-m) | +32.3 | +37.4 | +36.7 |
| + hydropower (uni+inf-x) | +32.5 | +37.6 | +37.0 |
| uni+inf+hd (published definition) | +37.7 | +40.5 | +36.1 |
| extended: uni+inf+hd-x | +39.4 | +43.5 | +39.3 |
| combi (published definition) | +44.3 | +50.6 | +45.9 |
| extended: combi-x | +47.4 | +55.6 | +50.8 |

The increments are almost the same at ω = 0 and 0.5. The whole extended package, `combi-x`, gives 2026–40 growth of 4.99 % a year at factor cost (ω = 0: 4.61; ω = 0.5: 5.41), against 2.22 % in the base.

Each addition's own increment, at market prices (factor cost in brackets), is:

- transport and marketing margins: +1.2 points (−0.1);
- hydropower: +0.2 (+0.2);
- education mix: +1.6 (+1.6);
- mining FDI: +2.0 (+1.4).

**A note on the GDP measure.** GDP at factor cost is value added at base-year unit values, so savings in intermediate inputs do not raise it. GDP at market prices is final demand at base-year prices, and it does register them. The two measures also differ in the base, 1.84 % a year at market prices against 2.22 % at factor cost, because they weight activities and final demand differently. Increments are therefore compared within a measure.

**Table A.11. `uni` GDP gain by pass-through θ (points a year over base, 2026–40)**

| θ | 1 | 0.75 | 0.5 | 0.25 | 0 |
| --- | ---: | ---: | ---: | ---: | ---: |
| GDP at factor cost | +1.49 | +0.86 | +0.47 | +0.19 | −0.02 |
| Exports | +4.65 | +2.27 | +0.84 | −0.12 | −0.80 |

Full model, ω = 0.25.

### A.7.2 Simulations the model supports

Table A.12 lists simulations the model can run as it stands, with the instrument and the recommended closure. They are proposals for discussion.

**Table A.12. Candidate simulations**

| # | Question | Instrument | Closure notes |
| --- | --- | --- | --- |
| S1 | Gradual or partial unification | premium path over 2026–30; θ between 0.5 and 1 | *rowclos* 1; report the θ range |
| S2 | Unification with or without removing the fuel quota | premium path; fuel quota kept or removed | isolates the quota's role in the `uni` gain |
| S3 | Liberalising fuel imports at the market rate | $shrom_{fuel}$ from 1 towards 0; quota removed | pump price rises to import parity at the parallel rate; the rent disappears |
| S4 | Oil price shock (+50 %) | world fuel price index | the fuel ceiling tightens through (A.21); informal fuel may switch on |
| S5 | Aid cut (grants −30 %) | grants path | fiscal and fuel-supply effects together; compare *govclos* 1–3 |
| S6 | Coffee and tea price boom | world price of agricultural exports | raises official FX, and so the fuel ceiling |
| S7 | Reallocating official FX between fuel and fertiliser | φ and the chemicals quota path | a trade-off between transport and energy and agricultural inputs |
| S8 | Financing infrastructure: taxes, domestic debt or aid | `uni+inf` under *govclos* 1, 2, 3 | crowding-out through (A.30) under domestic borrowing |
| S9 | Human development with and without the productivity effect | spending shock only, against the full `hd` | separates demand and supply effects |
| S10 | Mining expansion, with and without the idle-resource closure | mining endowment | shows how much of `combi` depends on the closure |
| S11 | Agricultural climate shock | agricultural TFP −10 % in a year, and recurrent shocks | food prices, rural incomes |
| S12 | Tariff reform (e.g. EAC common external tariff alignment) | tariff-rate scalars by commodity | revenue replacement by *govclos* |
| S13 | `combi+` | TFP path reaching 17.5 % by 2040 | completes the reference set |

---

## A.8 Limitations and questions for discussion

The points on which I would most value feedback are:

1. **Foreign-exchange rationing on the parallel market.** All official-rate imports (fuel, chemicals and fertiliser) are now rationed by official foreign exchange (A.21). This changes the results little (A.5.2). On the parallel market, which imports 71 % of goods, scarcity is still a price, the premium, not a volume limit. This is why the model misses the 2021 import surge. *Is quantity rationing on the parallel market the right next step? Are there 2019–24 series on private external financing and BRB foreign-exchange sales that would identify it?*
2. **Pass-through of the premium.** The unification gain rests on the premium reaching import prices (Table A.11). The 2024 episode supports high pass-through, but the five-year evidence is mixed. *Is reporting the gain as a range (θ = 0.5–1) the right presentation?*
3. **How much of the premium rent is a real cost, and who receives the rest.** ω = 0.25 adds about 0.4 points a year to the unification gain, and ω = 0.5 about 0.9 (Table A.10c). The rent is large at the reference application's premium, 17 % of GDP. *Is ω = 0.25 a defensible central value, and is there Burundi evidence (parallel-market margins, time spent obtaining official foreign exchange) to pin it?* The part that is a transfer accrues to private capital. Alternatives are importers with official access, the government (as an implicit tax) or the central bank, and the incidence of unification depends on this choice.
4. **Base-year premium.** The reference application uses 2.5, which is the 2024–25 level. The 2019 value is 1.58. *Should the scenario results be re-based to 1.58, at the cost of comparability with the reference application?*
5. **Informal fuel parameters.** μ is pinned by prices, but $\sigma^F$, ε and η are not pinned by volumes, because informal volumes are unobserved. Any survey or border data on informal fuel volumes would help.
6. **The idle-resource closure for mining.** It determines whether a larger endowment is used. *Is holding the rent at its reference path the right reservation price?*
7. **No money or inflation.** The model is real, and nominal values are anchored by the numeraire. It cannot reproduce inflation's erosion of domestic debt: the model's domestic debt ratio rises 13 points over 2019–24 against a flat ratio in the data.
8. **Distribution.** There are two household groups, and the survey-based poverty module of GEM-Core is not activated.
9. **Myopic investment.** Investment responds to current returns, and there are no expectations about unification or reform credibility.
10. **The 2019 SAM against the 2023 SAM.** INSBU has produced a 2023 SAM. I kept 2019, because it precedes the 2022–24 shocks and permits a backcast, but a 2023 base would reflect the post-shock structure.
11. **Elasticities.** They are standard values. The trade elasticities matter most, because the export response to the real depreciation drives the scenario results.
12. **The second group of adjustments.** The sizes of the transport, marketing and hydropower effects, of the education-mix shift and of mining FDI are illustrative (Table A.4). *Are there project appraisals or Burundi evidence to anchor them?* Does the education mix double count the HCI-based productivity gain?
13. **Which GDP measure to report.** Factor-cost GDP misses input savings; market-price GDP catches them but moves with the premium in the base. I report both.

---

## References

Andersson, J. A. E., J. Gillis, G. Horn, J. B. Rawlings and M. Diehl (2019). "CasADi: A software framework for nonlinear optimization and optimal control." *Mathematical Programming Computation* 11(1): 1–36.

Armington, P. S. (1969). "A theory of demand for products distinguished by place of production." *IMF Staff Papers* 16(1): 159–178.

Blanchflower, D. G. and A. J. Oswald (1994). *The Wage Curve*. Cambridge, MA: MIT Press.

Dirkse, S. P. and M. C. Ferris (1995). "The PATH solver: A non-monotone stabilization scheme for mixed complementarity problems." *Optimization Methods and Software* 5(2): 123–156.

Krueger, A. O. (1974). "The political economy of the rent-seeking society." *American Economic Review* 64(3): 291–303.

Lofgren, H., R. L. Harris and S. Robinson (2002). *A Standard Computable General Equilibrium (CGE) Model in GAMS*. Microcomputers in Policy Research 5. Washington, DC: International Food Policy Research Institute.

Lofgren, H. and M. Cicowiez. GEM-Core model and documentation. Washington, DC: World Bank. [Full reference to be completed.]
