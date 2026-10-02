# User Guide: Running the Burundi CGE Model and Reproducing the Findings

*Kodzovi Senu Abalo, Senior Economist, Fiscal Policy and Growth Global Department, World Bank. Version of 1 October 2026. For readers who do not use Python.*

Code, data and replication files: [github.com/kodzovee9/burundi-cge](https://github.com/kodzovee9/burundi-cge)

*The findings, interpretations and conclusions expressed here are those of the author and do not necessarily represent the views of the World Bank, its Executive Directors or the governments they represent.*

---

## 1. What this guide is for

This package contains a computable general equilibrium (CGE) model of Burundi, written in the Python programming language. You do not need to know Python to use it. This guide shows you how to:

1. install what the model needs, once, in about 10 minutes;
2. re-run every simulation behind the technical appendix and the seminar slides, with one double-click;
3. check, automatically, that your results match the documented findings;
4. find each result, and run a single simulation or change an assumption.

**Getting the package.** If you received it as a zip file, skip to section 2. Otherwise download it from [github.com/kodzovee9/burundi-cge](https://github.com/kodzovee9/burundi-cge): on that page, click the green **Code** button, then **Download ZIP**. You do not need a GitHub account.

Everything you need is in the package. Nothing is sent anywhere: the model runs entirely on your computer. You only need an internet connection during the one-time setup, to download the free Python packages.

The model itself is described in `APPENDIX.docx` (the technical appendix).

---

## 2. What is in the package

| Folder or file | What it is |
| --- | --- |
| `USER-GUIDE.docx` / `.pdf` | this guide |
| `APPENDIX.docx` / `.pdf` | the technical appendix: data, equations, closures, transmission channels, validation, simulations |
| `setup_mac.command`, `setup_windows.bat` | one-time setup (step 2 below) |
| `run_all_mac.command`, `run_all_windows.bat` | re-run everything and check the findings (step 3) |
| `run_quick_mac.command`, `run_quick_windows.bat` | a shorter check of the core findings |
| `pymodel/` | the model: program code (`gemcore/`), run scripts (`runs/`), checks (`validation/`), results (`reports/`) |
| `pymodel/reports/` | all results, as tables you can open in any text editor, Word or Excel |
| `model/` | the 2019 data workbook (`bdi2019-data.xlsx`) and the reference GAMS files the model was built from |
| `pymodel/data/` | the 2019–24 data the backcast uses, including INSBU's rebased national accounts (`insbu-aggregates-2016-2024.csv`) |

---

## 3. Step 1: install Python (once)

The model needs **Python 3.11, 3.12 or 3.13**; it was tested on 3.13. Newer versions, such as 3.14, have not been tested and some packages may not install on them, so please install 3.13.

**On a Mac**

1. Go to https://www.python.org/downloads/ and choose **Python 3.13** (scroll to "Looking for a specific release?" if the big button offers a newer version). Download the *macOS 64-bit universal2 installer*.
2. Open the downloaded file and click through the installer.
3. At the end, the installer opens a folder. Double-click **Install Certificates.command** in it.

**On Windows**

1. Go to https://www.python.org/downloads/ and download the **Python 3.13** *Windows installer (64-bit)*.
2. Run it. On the first screen, **tick "Add python.exe to PATH"**, then click *Install Now*.

If you already have Python 3.11–3.13, skip this step.

---

## 4. Step 2: set up the model (once)

1. Unzip the package anywhere, for example in *Documents*.
2. Open the unzipped folder and double-click:
   - on a Mac: **`setup_mac.command`**;
   - on Windows: **`setup_windows.bat`**.
3. A window opens and shows the installation. It creates a private folder `.venv` inside the package and installs five free packages into it: numpy, scipy, casadi, openpyxl and xlrd. Nothing else on your computer is changed.
4. Wait for **"Setup complete"**. This takes 2–5 minutes, then press Return (Mac) or any key (Windows).

**Mac security message.** macOS may say the file "cannot be opened because it is from an unidentified developer". Right-click (or Control-click) the file, choose **Open**, then **Open** again. You only need to do this once per file.

**If a text editor opens instead of a window.** Open the *Terminal* app, type `bash ` (with a space), drag the `setup_mac.command` file into the Terminal window, and press Return.

---

## 5. Step 3: reproduce all the findings

Double-click:

- on a Mac: **`run_all_mac.command`**;
- on Windows: **`run_all_windows.bat`**.

The window shows progress in three stages:

1. **Base-year check** (a few seconds). The model must reproduce the 2019 social accounting matrix exactly. If this fails, nothing else runs.
2. **The simulations** (15–30 minutes, depending on your computer). There are 18 of them, run several at a time:
   - the reform scenarios;
   - the pass-through and rent-cost sensitivities;
   - the second group of reform effects;
   - twelve versions of the 2019–24 backcast;
   - an independent audit of every solution.

   Each line ends with *ok* when that run has converged.
3. **The comparison.** About 50 key numbers are compared with the values documented in the appendix.

The result is written to **`pymodel/reports/REPRODUCTION-CHECK.md`**. Open it with any text editor (TextEdit, Notepad) or with Word.

- Each finding shows the documented value, the value you reproduced, and **PASS** or **FAIL**.
- The last line in the window says, for example, *"41 findings reproduced, 0 not, 0 skipped"*.

For a faster first test, use **`run_quick_mac.command`** or **`run_quick_windows.bat`**. It takes 5–10 minutes and covers the core findings: the base year, the headline scenarios and the default backcast.

**On a slow computer**, or if the computer becomes unresponsive, run fewer simulations at once. Open a terminal (section 8) and type `../.venv/bin/python runs/reproduce_all.py --workers 2` (Mac) or `..\.venv\Scripts\python runs\reproduce_all.py --workers 2` (Windows).

---

## 6. What the check covers, and where each finding is

Results can differ in the last decimal between computers, because computers round numbers slightly differently. Each finding therefore has a tolerance, 0.005 percentage points for growth rates and 0.05 points for levels, well below the precision shown in the appendix.

| Finding | Report in `pymodel/reports/` | Appendix | Slides |
| --- | --- | --- | --- |
| All of the below, checked at once | `REPRODUCTION-CHECK.md` | A.6.5 | 32 |
| The 2019 base year is reproduced exactly | `logs/baseyear.log` | A.6.1 | 29, 32 |
| Every solution satisfies every equation, Walras' law and the quota, informal-fuel and idle-resource conditions | `SOLUTION-AUDIT.md` | A.6.5 | 29, 32 |
| Backcast 2019–24 against INSBU and BRB data | `backcast-2019-2024.md` | Tables A.8, A.9 | 24, 31 |
| Backcast variants (fuel or chemicals set by FX alone, flat premium, pass-through, and others) | `backcast-2019-2024-<variant>.md` | Table A.10b | 24 |
| Reform scenarios with the full model | `macro-growth-2026-2040.md` | Table A.10 | 34 |
| Unification gain by premium pass-through | `uni-passthrough.md` | Table A.11 | 35 |
| Rent cost (ω), budget support, second and third groups of reform effects | `reform-boost-2026-2040.md` | Tables A.10c, A.10d | 36, 37 |

**A note on the two GDP measures.** "GDP at factor cost" (GDPFC) is the measure in the published tables. It does not register savings in intermediate inputs, so the transport and hydropower effects barely move it. "GDP at market prices" (GDPMP) does register them. Both are reported (appendix A.7).

---

## 7. If something does not reproduce

- **A FAIL in the base-year check** means the data workbook or a program file has been changed. Unzip a fresh copy of the package.
- **A run marked PROBLEM** names the issue: *NOT CONVERGED*, *exit code*, or *Traceback*, which is a program error. Its full screen output is in `pymodel/reports/logs/<run name>.log`. Send that file to the author.
- **A FAIL on a finding** with a reproduced value close to the expected one usually means a different package version. The tested versions are listed in `pymodel/requirements.txt`. Report the two values to the author.
- **"skipped (no INSBU data)"** means the file `pymodel/data/insbu-aggregates-2016-2024.csv` is missing. The backcast cannot run without it; everything else still reproduces.
- **"Python 3.11, 3.12 or 3.13 was not found"**: install Python 3.13 (section 3), then run the setup again.
- **The setup stops at "Package installation failed"**: usually there is no internet connection, or an institutional network blocks downloads. Try another network, or ask IT to allow *pypi.org* and *files.pythonhosted.org*.
- **Running the setup again is always safe.** To start completely fresh, delete the `.venv` folder first.

---

## 8. Running a single simulation, or changing an assumption

This part uses a terminal window, where you type commands.

**Open a terminal in the `pymodel` folder**

- *Mac:* open the *Terminal* app. Type `cd ` (with a space), drag the `pymodel` folder into the window, and press Return.
- *Windows:* open the `pymodel` folder in File Explorer, click in the address bar at the top, type `cmd` and press Return.

In the commands below, `PY` stands for the model's Python:

- type `../.venv/bin/python` on a Mac;
- type `..\.venv\Scripts\python` on Windows, and use `\` instead of `/` in file names.

**Commands**

| To do this | Type | Results in `reports/` | Time |
| --- | --- | --- | --- |
| Check the base year | `PY validation/check_baseyear_residuals.py` | on screen | seconds |
| All reform scenarios, full model | `PY runs/all_scenarios.py` | `macro-growth-2026-2040.md` | 5 min |
| One scenario, with its solution log | `PY runs/run.py uni` (or `uni+inf`, `uni+inf+hd`, `combi`) | on screen | 2 min |
| Pass-through sensitivity | `PY runs/uni_passthrough.py` | `uni-passthrough.md` | 6 min |
| Reform effects at one rent-cost share | `PY runs/reform_boost.py --rent-cost 0.25`, then `PY runs/reform_boost.py --collect` | `reform-boost-2026-2040.md` | 10 min |
| Backcast 2019–24 | `PY runs/backcast.py` | `backcast-2019-2024.md` | 1 min |
| Audit every solution | `PY validation/check_solutions.py` | `SOLUTION-AUDIT.md` | 9 min |
| Everything, with the check | `PY runs/reproduce_all.py` | `REPRODUCTION-CHECK.md` | 15–30 min |

Any run script lists its options when given `--help`, for example `PY runs/backcast.py --help`.

**Assumptions you can change from the command line**

| Assumption | How | Appendix |
| --- | --- | --- |
| Share of the premium rent that is a real cost, ω | `PY runs/reform_boost.py --rent-cost 0.5` (any value from 0 to 1) | A.3.1, A.5a |
| Premium pass-through to import prices, θ | `PY runs/uni_passthrough.py --thetas 1,0.6,0.3` | A.18 |
| Backcast: premium pass-through | `PY runs/backcast.py --passthrough 0.5 --tag mytest` | A.6.3 |
| Backcast: fuel and/or chemicals set by foreign exchange alone | `PY runs/backcast.py --fuel-quota fx --chem-quota fx --tag mytest` | A.5.2 |
| Backcast: premium held at its 2019 level | `PY runs/backcast.py --premium flat --tag mytest` | A.6.3 |

The `--tag mytest` part writes the result to a separate file, `backcast-2019-2024-mytest.md`, so the documented results are not overwritten.

**Assumptions set in the program files.** You can change these with a plain text editor: TextEdit in plain-text mode, Notepad, or any code editor. Change only the number, keep the rest of the line, and save.

| Assumption | File | Line to look for |
| --- | --- | --- |
| Default rent-cost share ω (0.25) and its adjustment speed (1/3) | `pymodel/gemcore/calibration.py` | `RENT_COST_DEFAULT = 0.25`, `RENT_ADJUST_DEFAULT = 1.0 / 3.0` |
| Fuel substitution σ, household switching ε, informal markup μ, supply elasticity η | `pymodel/gemcore/fuel.py` | `return FuelConfig(sigma_act=0.1, informal=True, mu=0.9, ...` |
| Second-group sizes: transport, marketing, hydropower, education mix | `pymodel/gemcore/scenarios.py` | `TRANSPORT_CUT = 0.15`, `MARKETING_CUT = 0.10`, `HYDRO_CUT = 0.50`, `EDU_SHIFT_2040 = 0.02` |
| Budget support amounts | `pymodel/gemcore/scenarios.py` | `BUDGET_SUPPORT = {"2026": 0.01, ...}` |

After such a change, the reproduction check will report FAIL on the findings that depend on it. That is expected: the check compares with the documented values. To return to the documented model, unzip a fresh copy.

---

## 9. Reading the result tables

The reports are in Markdown, a plain-text format. Tables appear as rows of `|`-separated cells. They are readable in any text editor, and display as formatted tables in many editors, for example Visual Studio Code, or Word after *File › Open*. The scenario, pass-through and backcast reports also have a `.csv` twin, which opens directly in Excel.

Growth rates are average annual percentages over 2026–40 unless stated. Levels are percentages above the base run in 2040.

---

## 10. Further reading

- `APPENDIX.docx`: the model in full, with equations (A.1)–(A.34), closures, transmission channels, validation, and the questions for discussion.
- `pymodel/docs/MECHANISMS.md`: the dual exchange-rate market, the fuel and foreign-exchange mechanisms, and the reform-effect options, with code references.
- `pymodel/README.md`: the technical guide to the code, for Python users.

Questions and problems: contact the author, Kodzovi Senu Abalo.
