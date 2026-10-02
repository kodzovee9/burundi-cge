"""Loader for the Burundi 2019-2024 actuals workbook.

Reads `pymodel/data/burundi-actuals-2019-2024-FINAL.xlsx` — the collection
template filled in by Kodzovi (September 2026) — into plain dicts keyed the way
the model needs them. Pure parsing: nothing here decides units, bases or which
series to trust. Those decisions live in the backcast harness.

    from gemcore.actuals import load_actuals
    act = load_actuals()
    act.inputs["IN03"]["2024"]        -> 2.172   (FX premium, ratio)
    act.targets["TG02"]["2023"]       -> 26.94   (CPI inflation, %)
    act.sectors["ind"]["2021"]        -> 104.6 ... see SECTOR_GROUPS
    act.trade[("c-refpet","imports")]["2022"] -> 300.52 (USD mn)
    act.notes["IN02"]                 -> the Notes cell, verbatim

Layout facts this relies on (do not change the workbook without updating):
  * Inputs / Targets: ID in column A, year columns found by header text.
  * Sectors: value added is given as GROUP TOTALS placed on one model-sector
    row per group (see SECTOR_GROUPS); the other rows in a group are empty.
    Two reference blocks follow, headed "Real VA growth" and "UNSD AMA".
  * Trade: one row per (commodity, flow); reference TOTAL rows and an HS
    mapping block follow.
"""

from __future__ import annotations

import os
from dataclasses import dataclass, field

import openpyxl

HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT = os.path.join(os.path.dirname(HERE), "data",
                       "burundi-actuals-2019-2024-FINAL.xlsx")
YEARS = [str(y) for y in range(2019, 2026)]

# Which model sectors each group total on the Sectors sheet covers, and which
# model-sector row the total was written on. Derived from the "Group total
# given once (rows X-Y)" notes in the filled workbook.
SECTOR_GROUPS = {
    "agr":      {"row": "a-agr",      "members": ["a-agr"]},
    "ind":      {"row": "a-min",      "members": ["a-min", "a-food", "a-texwapp",
                                                  "a-woodpaper", "a-chemplast",
                                                  "a-prodminnmet", "a-met",
                                                  "a-oman", "a-elect", "a-water"]},
    "construc": {"row": "a-construc", "members": ["a-construc"]},
    "trade":    {"row": "a-trade",    "members": ["a-trade", "a-hotelrest"]},
    "transp":   {"row": "a-transp",   "members": ["a-transp"]},
    "pubsvc":   {"row": "a-admpub",   "members": ["a-admpub", "a-edu",
                                                  "a-health", "a-oser"]},
}
# The Sectors sheet also carries a-food as its own group total (rows 4-10 =
# a-food..a-oman), which overlaps "ind". Kept separately so nothing double
# counts: use "ind" for the C-E total and "manuf" for the D subset.
SECTOR_GROUPS["manuf"] = {"row": "a-food",
                          "members": ["a-food", "a-texwapp", "a-woodpaper",
                                      "a-chemplast", "a-prodminnmet", "a-met",
                                      "a-oman"]}


@dataclass
class Actuals:
    inputs: dict = field(default_factory=dict)    # ID -> {year: value}
    targets: dict = field(default_factory=dict)   # ID -> {year: value}
    sectors: dict = field(default_factory=dict)   # group -> {year: value}
    sector_ref: dict = field(default_factory=dict)  # label -> {year: value}
    trade: dict = field(default_factory=dict)     # (c, flow) -> {year: value}
    trade_ref: dict = field(default_factory=dict)  # label -> {year: value}
    units: dict = field(default_factory=dict)     # ID/key -> unit text
    sources: dict = field(default_factory=dict)   # ID/key -> "Source used"
    notes: dict = field(default_factory=dict)     # ID/key -> Notes
    labels: dict = field(default_factory=dict)    # ID -> Variable name
    mfmod: dict = field(default_factory=dict)     # MFMod mnemonic -> {year: value}
    insbu: object = None                          # gemcore.insbu.Insbu, or None
    path: str = ""

    def series(self, key):
        """Any series by key, whichever sheet it came from."""
        for d in (self.inputs, self.targets, self.sectors, self.trade,
                  self.sector_ref, self.trade_ref):
            if key in d:
                return d[key]
        raise KeyError(key)


def _num(v):
    if v is None or (isinstance(v, str) and not v.strip()):
        return None
    if isinstance(v, (int, float)):
        return float(v)
    try:
        return float(str(v).replace(",", ""))
    except ValueError:
        return v.strip()          # text entries such as IN16 are kept as text


def _year_cols(header):
    out = {}
    for j, h in enumerate(header):
        if h is None:
            continue
        tok = str(h).strip().split()[0]
        if tok in YEARS:
            out[tok] = j
    return out


def _col(header, *names):
    for j, h in enumerate(header):
        if h and any(n.lower() in str(h).lower() for n in names):
            return j
    return None


def _rows(ws):
    for r in ws.iter_rows(min_row=1, values_only=True):
        yield list(r)


def _parse_series_sheet(ws, into, act):
    rows = list(_rows(ws))
    header = rows[0]
    yc = _year_cols(header)
    c_unit = _col(header, "Unit")
    c_src = _col(header, "Source used")
    c_note = _col(header, "Notes")
    for row in rows[1:]:
        key = row[0]
        if not key or not str(key).strip():
            continue
        key = str(key).strip()
        vals = {y: _num(row[j]) for y, j in yc.items()}
        vals = {y: v for y, v in vals.items() if v is not None}
        if not vals and not (c_src is not None and row[c_src]):
            continue
        into[key] = vals
        act.labels[key] = row[1]
        if c_unit is not None:
            act.units[key] = row[c_unit]
        if c_src is not None:
            act.sources[key] = row[c_src]
        if c_note is not None:
            act.notes[key] = row[c_note]


def _parse_sectors(ws, act):
    rows = list(_rows(ws))
    header = rows[0]
    yc = _year_cols(header)
    c_src = _col(header, "Source used")
    c_note = _col(header, "Notes")
    row_of = {g["row"]: name for name, g in SECTOR_GROUPS.items()}
    block = None
    for row in rows[1:]:
        key = row[0]
        if key is None or not str(key).strip():
            continue
        key = str(key).strip()
        vals = {y: _num(row[j]) for y, j in yc.items()}
        vals = {y: v for y, v in vals.items() if isinstance(v, float)}
        if key in row_of and vals:
            g = row_of[key]
            act.sectors[g] = vals
            act.sources[("sector", g)] = row[c_src] if c_src is not None else None
            act.notes[("sector", g)] = row[c_note] if c_note is not None else None
            continue
        if key == "TOTAL" and vals:
            act.sectors["total"] = vals
            act.notes[("sector", "total")] = row[c_note] if c_note is not None else None
            continue
        # reference blocks below the sector rows
        if key.startswith("Real VA growth"):
            block = "growth"; continue
        if key.startswith("UNSD AMA"):
            block = "unsd"; continue
        if block and vals:
            act.sector_ref[(block, key)] = vals
            if c_src is not None:
                act.sources[("sector_ref", block, key)] = row[c_src]
            if c_note is not None:
                act.notes[("sector_ref", block, key)] = row[c_note]


def _parse_trade(ws, act):
    rows = list(_rows(ws))
    header = rows[0]
    yc = _year_cols(header)
    c_flow = 2
    c_src = _col(header, "Source used")
    c_note = _col(header, "Notes")
    block = None
    for row in rows[1:]:
        key = row[0]
        if key is None or not str(key).strip():
            continue
        key = str(key).strip()
        vals = {y: _num(row[j]) for y, j in yc.items()}
        vals = {y: v for y, v in vals.items() if isinstance(v, float)}
        flow = str(row[c_flow]).strip().lower() if row[c_flow] else ""
        if key.startswith("c-") and flow in ("exports", "imports") and block is None:
            act.trade[(key, flow)] = vals
            act.units[(key, flow)] = row[5]
            if c_src is not None:
                act.sources[(key, flow)] = row[c_src]
            if c_note is not None:
                act.notes[(key, flow)] = row[c_note]
            continue
        if key.startswith("BoP and customs totals"):
            block = "totals"; continue
        if key.startswith("HS chapter"):
            block = "hsmap"; continue
        if block == "totals" and vals:
            lab = (key, str(row[1]).strip() if row[1] else "")
            act.trade_ref[lab] = vals
            if c_src is not None:
                act.sources[("trade_ref",) + lab] = row[c_src]
        elif block == "hsmap":
            act.trade_ref[("hsmap", key)] = str(row[1]).strip() if row[1] else ""


DEFAULT_MFMOD = os.path.join(os.path.dirname(HERE), "data",
                             "mfmod-bdi-NIA-2026-09.xlsx")
MFMOD_SHEETS = ("NIA-Vol", "NIA-P", "NIA-VAL", "FISCAL", "BOP")


def load_mfmod(path: str = DEFAULT_MFMOD, sheets=MFMOD_SHEETS) -> dict:
    """The World Bank MFMod Burundi datasheet (user-supplied, 2026-09-16):
    one row per mnemonic, years across. Returns {mnemonic: {year: value}}
    plus {mnemonic + "@label": variable name, mnemonic + "@disp": display
    code} ('g' growth %, 'l' level, '%gdp', 'contr', 'add'). Only the first
    sheet that defines a mnemonic is kept. Missing file -> {}."""
    if not os.path.exists(path):
        return {}
    wb = openpyxl.load_workbook(path, read_only=True, data_only=True)
    out = {}
    for name in sheets:
        if name not in wb.sheetnames:
            continue
        hdr = None
        for row in wb[name].iter_rows(values_only=True):
            if row and row[0] == "Mnemonic":
                hdr = row
                continue
            if not hdr or not row or not isinstance(row[0], str) or not row[0].strip():
                continue
            code = row[0].strip()
            if code in out:
                continue
            ser = {}
            for h, v in zip(hdr, row):
                if isinstance(h, int) and isinstance(v, (int, float)):
                    ser[str(h)] = float(v)
            out[code] = ser
            out[code + "@label"] = str(row[1]).strip() if row[1] else ""
            out[code + "@disp"] = row[3]
    return out


def load_actuals(path: str = DEFAULT) -> Actuals:
    wb = openpyxl.load_workbook(path, data_only=True, read_only=True)
    act = Actuals(path=path)
    _parse_series_sheet(wb["Inputs"], act.inputs, act)
    _parse_series_sheet(wb["Targets"], act.targets, act)
    _parse_sectors(wb["Sectors"], act)
    _parse_trade(wb["Trade"], act)
    act.mfmod = load_mfmod()
    from .insbu import load_insbu
    act.insbu = load_insbu()
    return act


def summary(act: Actuals) -> str:
    lines = [f"actuals: {os.path.basename(act.path)}",
             f"  inputs  {len(act.inputs):3d} series: "
             + ", ".join(sorted(act.inputs)),
             f"  targets {len(act.targets):3d} series: "
             + ", ".join(sorted(act.targets)),
             f"  sectors {len(act.sectors):3d} groups: "
             + ", ".join(sorted(act.sectors)),
             f"  sector reference {len(act.sector_ref)} rows",
             f"  trade   {len(act.trade):3d} (commodity, flow) series; "
             f"{len(act.trade_ref)} reference rows"]
    return "\n".join(lines)


if __name__ == "__main__":
    a = load_actuals()
    print(summary(a))
    print("\nspot checks:")
    print("  IN03 premium 2019/2024:", a.inputs["IN03"].get("2019"),
          a.inputs["IN03"].get("2024"))
    print("  IN04 GDP growth 2020..2024:",
          [a.inputs["IN04"].get(y) for y in YEARS[1:6]])
    print("  TG02 CPI inflation 2022/2023:", a.targets["TG02"].get("2022"),
          a.targets["TG02"].get("2023"))
    print("  sectors ind 2019/2024:", a.sectors["ind"].get("2019"),
          a.sectors["ind"].get("2024"))
    print("  trade c-refpet imports 2022:",
          a.trade[("c-refpet", "imports")].get("2022"))
    print("  sector_ref growth Agriculture:",
          {k: v for k, v in a.sector_ref.items() if "Agric" in k[1]})
    print("  IN16 2023 (text):", str(a.inputs["IN16"].get("2023"))[:80])
