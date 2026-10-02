"""INSBU rebased national accounts: supply-use tables (TRE) and integrated
economic accounts (TCEI), 2016-2024.

    from gemcore.insbu import load_insbu
    ins = load_insbu()          # None if the raw files are not present
    ins.vol["GDP"]["2024"]      # BIF bn at 2019 prices (chain-linked)
    ins.growth["VA_agr"]["2021"]

Source files (received 2026-09-29) live in `<repo>/raw-insbu/` -- one level
above `pymodel/` -- as delivered: `TRE COURANT yyyy.xls`, `TRE CONSTANT
yyyy.xls`, `TCEI2021_VF.xls`, `TCE_2023_BANGA_IMCSVF.xls`, `TCEI2024_VF.xls`.
When they are absent, `load_insbu` reads `data/insbu-aggregates-2016-2024.csv`
instead: the 355 source values the model uses, at full precision, so both
routes give identical results. Regenerate it after INSBU revises its accounts:
`python -m gemcore.insbu --write-extract`.

Two facts every user of these numbers must know:

* **The "constant" tables are at PREVIOUS-YEAR prices.** Real growth in year
  t is constant(t) / current(t-1); levels at fixed prices come from
  chain-linking, done here to 2019 prices. Chain-linked components do not add
  up exactly.
* **This is the basis the 2019 SAM was built on.** Without the premium
  adjustment of `bdi2019-data2.inc` the SAM's 2019 GDP is within 1.3 % of
  INSBU's and every activity's value-added share within 1.6 points
  (APPENDIX.md, A.2.1).

TRE layout (sheet 'TRE', BIF millions), checked on every file: supply rows
7-40 with column 47 = imports and row 41 = totals; use rows 47-80 with row 81
= totals, column 47 exports, 48 final consumption, 49 households, 52
government, 53 NPISH, 54 GFCF, 55 change in stocks, 56 valuables; row 82 =
value added by branch (columns 11-44), taxes on products (5-9), total economy
(46 = GDP).
"""

from __future__ import annotations

import glob
import os
import re
from dataclasses import dataclass, field

HERE = os.path.dirname(os.path.abspath(__file__))
RAW = os.path.join(os.path.dirname(os.path.dirname(HERE)), "raw-insbu")

BRANCH_COLS = range(11, 45)
# ISIC groupings of the 34 branch codes, by first letter
GROUP = {"agr": "A", "ind": "BCDEF", "man": "C",
         "srv": "GHIJKLMNOPQRSTU"}
# model commodity <- INSBU products whose change in stocks it carries
STOCK_MAP = {"c-agr": ("A01", "A02", "A03", "A04"), "c-food": ("C06",),
             "c-min": ("B05",)}

AGG_KEYS = ("GDP", "VA", "TAXPROD", "PRVC", "HHC", "NPISH", "GOVC", "GFCF",
            "STK", "EXP", "IMP", "VA_agr", "VA_ind", "VA_man", "VA_srv",
            "VA_B05", "VA_oind")
# chain-linking a series that can be near zero or change sign is meaningless
NO_VOLUME = ("STK", "TAXPROD")


def _num(v):
    return float(v) if isinstance(v, (int, float)) else 0.0


def _check_tre(sh):
    assert sh.nrows >= 91 and sh.ncols >= 57, (sh.nrows, sh.ncols)
    assert str(sh.cell_value(81, 1)).strip() == "Total"
    assert "Valeur ajout" in str(sh.cell_value(82, 1))
    assert str(sh.cell_value(6, 11)).strip() == "A01"
    assert str(sh.cell_value(6, 44)).strip() == "Z99"
    assert "Impor" in str(sh.cell_value(4, 47))
    assert "Expor" in str(sh.cell_value(44, 47))


def read_tre(path):
    """One TRE file -> aggregates (BIF millions) and branch detail."""
    import xlrd
    sh = xlrd.open_workbook(path).sheet_by_index(0)
    _check_tre(sh)
    codes = [str(sh.cell_value(6, c)).strip() for c in BRANCH_COLS]
    va = {k: _num(sh.cell_value(82, c)) for k, c in zip(codes, BRANCH_COLS)}
    stk = {}
    for r in range(47, 81):
        code = str(sh.cell_value(r, 0)).strip()
        if code:
            stk[code] = _num(sh.cell_value(r, 55))
    d = {
        "GDP": _num(sh.cell_value(82, 46)),
        "VA": _num(sh.cell_value(82, 45)),
        "TAXPROD": sum(_num(sh.cell_value(82, c)) for c in range(5, 10)),
        "IMP": _num(sh.cell_value(41, 47)),
        "EXP": _num(sh.cell_value(81, 47)),
        "HHC": _num(sh.cell_value(81, 49)),
        "GOVC": _num(sh.cell_value(81, 52)),
        "NPISH": _num(sh.cell_value(81, 53)),
        "GFCF": _num(sh.cell_value(81, 54)),
        "STK": _num(sh.cell_value(81, 55)),
        "va_branch": va,
        "stk_product": stk,
        "comp_branch": {k: _num(sh.cell_value(83, c))
                        for k, c in zip(codes, BRANCH_COLS)},
    }
    d["PRVC"] = d["HHC"] + d["NPISH"]
    for g, letters in GROUP.items():
        d["VA_" + g] = sum(v for k, v in va.items() if k[:1] in letters)
    d["VA_B05"] = va.get("B05", 0.0)
    # industry other than manufacturing (B, D, E, F): additive within one
    # file, so its growth at previous-year prices is exact
    d["VA_oind"] = d["VA_ind"] - d["VA_man"]
    return d


def _tcei_rows(path):
    import xlrd
    sh = xlrd.open_workbook(path).sheet_by_index(0)
    hdr = next(r for r in range(sh.nrows)
               if any("Economie Totale" in str(sh.cell_value(r, c))
                      for c in range(sh.ncols)))
    labels = [str(sh.cell_value(hdr, c)).strip() for c in range(sh.ncols)]
    code_col = next(c for c in range(sh.ncols)
                    if "soldes" in str(sh.cell_value(hdr, c)).lower())
    left = {labels[c]: c for c in range(1, code_col) if labels[c]}
    rows = {}
    for r in range(hdr + 1, sh.nrows):
        code = str(sh.cell_value(r, code_col)).strip()
        if code:
            rows[code] = {k: _num(sh.cell_value(r, c)) for k, c in left.items()}
    return rows


def read_tcei(path):
    """The few TCEI items the backcast uses (BIF millions)."""
    rows = _tcei_rows(path)

    def u(code, who):
        return rows.get(code, {}).get(who, 0.0)
    return {"GFCF_gov": u("P51", "APU"), "GFCF": u("P51", "Economie Totale"),
            "CA_deficit": u("B12", "Reste du monde"),
            "NL_gov": u("B09", "APU")}


@dataclass
class Insbu:
    cur: dict = field(default_factory=dict)      # key -> {year: BIF bn}
    vol: dict = field(default_factory=dict)      # key -> {year: BIF bn, 2019 prices}
    growth: dict = field(default_factory=dict)   # key -> {year: % real growth}
    defl: dict = field(default_factory=dict)     # key -> {year: index, 2019 = 1}
    share: dict = field(default_factory=dict)    # key -> {year: % of GDP}
    stock_share: dict = field(default_factory=dict)  # model c -> {year: % of GDP}
    tcei: dict = field(default_factory=dict)     # year -> dict, BIF bn
    years: list = field(default_factory=list)
    raw: str = ""


def read_raw(raw=RAW):
    """The source values the model uses, from INSBU's files in `raw` (BIF
    millions): {(basis, year): {key: value, "stk_product": {...}}} for the TRE
    tables, and {year: {item: value}} for the TCEIs. None if there are no files."""
    files = sorted(glob.glob(os.path.join(raw, "TRE*.xls")))
    if not files:
        return None
    tre = {}
    for p in files:
        name = os.path.basename(p).upper()
        basis = "current" if "COURANT" in name else "constant"
        year = re.search(r"(20\d\d)", name).group(1)
        tre[(basis, year)] = read_tre(p)
    tcei = {}
    for p in sorted(glob.glob(os.path.join(raw, "TCE*.xls"))):
        tcei[re.search(r"(20\d\d)", os.path.basename(p)).group(1)] = read_tcei(p)
    return tre, tcei


# The extract: every source value the model uses, so the backcast runs without
# INSBU's files. One row per value, at the files' full precision (BIF millions).
EXTRACT = os.path.join(os.path.dirname(HERE), "data", "insbu-aggregates-2016-2024.csv")
STOCK_PRODUCTS = tuple(p for prods in STOCK_MAP.values() for p in prods)
_EXTRACT_HEAD = """\
# INSBU rebased national accounts, Burundi 2016-2024: the aggregates the model uses.
# Source: Institut National de la Statistique du Burundi (INSBU), supply-use tables
# (TRE, current prices and previous-year prices) and integrated economic accounts
# (TCEI 2021, 2023, 2024). BIF millions. Extracted by gemcore/insbu.py --write-extract.
# table: tre-current = TRE at current prices; tre-prevyear = TRE at the previous
# year's prices; tre-stocks = change in stocks by product, current prices; tcei = TCEI.
# key: GDP, VA (value added), TAXPROD (taxes less subsidies on products), PRVC, HHC,
# NPISH, GOVC (final consumption: private, households, NPISH, government), GFCF, STK
# (change in stocks), EXP, IMP; VA_agr/ind/man/srv/oind = value added of ISIC groups
# A / B-F / C / G-U / B,D,E,F; VA_B05 = extraction; products by INSBU code; TCEI
# items GFCF_gov, GFCF, CA_deficit (B12 rest of world), NL_gov (B09 government).
"""


def write_extract(path=EXTRACT, raw=RAW):
    """Write the extract from INSBU's files; returns the number of values."""
    src = read_raw(raw)
    if src is None:
        raise SystemExit(f"no INSBU files in {raw}")
    tre, tcei = src
    rows = []
    for (basis, y), d in sorted(tre.items()):
        tab = "tre-current" if basis == "current" else "tre-prevyear"
        rows += [(tab, k, y, d[k]) for k in AGG_KEYS]
        if basis == "current":
            rows += [("tre-stocks", p, y, d["stk_product"].get(p, 0.0)) for p in STOCK_PRODUCTS]
    for y, d in sorted(tcei.items()):
        rows += [("tcei", k, y, v) for k, v in d.items()]
    with open(path, "w", newline="") as fh:
        fh.write(_EXTRACT_HEAD + "table,key,year,value\n")
        fh.writelines(f"{t},{k},{y},{v!r}\n" for t, k, y, v in rows)
    return len(rows)


def read_extract(path=EXTRACT):
    """The extract, in read_raw's form; None if the file is not there."""
    if not os.path.exists(path):
        return None
    tre, tcei = {}, {}
    with open(path) as fh:
        for line in fh:
            if line.startswith("#") or line.startswith("table,") or not line.strip():
                continue
            tab, k, y, v = line.rstrip("\n").split(",")
            v = float(v)
            if tab == "tcei":
                tcei.setdefault(y, {})[k] = v
            elif tab == "tre-stocks":
                tre.setdefault(("current", y), {}).setdefault("stk_product", {})[k] = v
            else:
                tre.setdefault(("current" if tab == "tre-current" else "constant", y), {})[k] = v
    return tre, tcei


def source(raw=RAW, extract=EXTRACT):
    """Where load_insbu reads from: "files", "extract" or None."""
    if glob.glob(os.path.join(raw, "TRE*.xls")):
        return "files"
    return "extract" if os.path.exists(extract) else None


def load_insbu(raw=RAW, extract=EXTRACT):
    """INSBU's accounts, from the files in `raw` when they are there, else from
    the extract in pymodel/data/; None if neither is."""
    src, where = read_raw(raw), raw
    if src is None:
        src, where = read_extract(extract), extract
    if src is None:
        return None
    tre, tcei = src
    years = sorted({y for (b, y) in tre if b == "current"})
    ins = Insbu(years=years, raw=where)
    for k in AGG_KEYS:
        ins.cur[k] = {y: tre[("current", y)][k] / 1000.0 for y in years}
        ins.share[k] = {y: 100.0 * tre[("current", y)][k] / tre[("current", y)]["GDP"]
                        for y in years}
        if k in NO_VOLUME:
            continue
        g = {}
        for y in years:
            yp = str(int(y) - 1)
            if ("constant", y) in tre and ("current", yp) in tre and tre[("current", yp)][k]:
                g[y] = 100.0 * (tre[("constant", y)][k] / tre[("current", yp)][k] - 1)
        ins.growth[k] = g
        # chain-link to 2019 prices: v(2019) = current(2019)
        v = {"2019": ins.cur[k]["2019"]}
        for y in years:
            if y > "2019" and y in g:
                v[y] = v[str(int(y) - 1)] * (1 + g[y] / 100.0)
        for y in sorted((y for y in years if y < "2019"), reverse=True):
            yn = str(int(y) + 1)
            if yn in g and yn in v:
                v[y] = v[yn] / (1 + g[yn] / 100.0)
        ins.vol[k] = v
        ins.defl[k] = {y: (ins.cur[k][y] / v[y]) / (ins.cur[k]["2019"] / v["2019"])
                       for y in v if v[y]}
    for c, prods in STOCK_MAP.items():
        ins.stock_share[c] = {
            y: 100.0 * sum(tre[("current", y)]["stk_product"].get(p, 0.0) for p in prods)
            / tre[("current", y)]["GDP"] for y in years}
    for y, d in tcei.items():
        ins.tcei[y] = {k: v / 1000.0 for k, v in d.items()}
    return ins


def volume_index(ins, key, through="2024"):
    """Chain-linked volume, 2019 = 1."""
    v = ins.vol[key]
    return {y: v[y] / v["2019"] for y in v if "2019" <= y <= through}


if __name__ == "__main__":
    import sys
    if "--write-extract" in sys.argv:
        n = write_extract()
        print(f"wrote {os.path.relpath(EXTRACT)} ({n} values)")
        sys.exit(0)
    ins = load_insbu()
    if ins is None:
        raise SystemExit(f"no INSBU files in {RAW} and no extract at {EXTRACT}")
    print(f"read from {ins.raw}")
    ys = [y for y in ins.years if y >= "2019"]
    print(f"{'real growth %':16}" + "".join(f"{y:>8}" for y in ys))
    for k in ("GDP", "VA", "VA_agr", "VA_ind", "VA_man", "VA_srv", "VA_B05",
              "HHC", "GOVC", "GFCF", "EXP", "IMP"):
        print(f"{k:16}" + "".join(f"{ins.growth[k].get(y, float('nan')):8.1f}" for y in ys))
    print(f"\n{'share of GDP %':16}" + "".join(f"{y:>8}" for y in ys))
    for k in ("HHC", "GOVC", "GFCF", "STK", "EXP", "IMP"):
        print(f"{k:16}" + "".join(f"{ins.share[k][y]:8.1f}" for y in ys))
    print("deflator GDP    " + "".join(f"{ins.defl['GDP'][y]:8.3f}" for y in ys))
    print("stocks by commodity (% GDP):", {c: {y: round(v, 2) for y, v in s.items() if y >= '2019'}
                                          for c, s in ins.stock_share.items()})
    print("TCEI:", {y: {k: round(v, 1) for k, v in d.items()} for y, d in ins.tcei.items()})
