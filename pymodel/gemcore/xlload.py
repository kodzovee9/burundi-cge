"""GDXXRW emulator: read GAMS sets/parameters from an Excel workbook
driven by a `layout` index sheet, replicating
  $CALL GDXXRW <app>-data.xlsx index=layout!A1

Semantics implemented (subset used by GEM-Core):
  set, rdim=1, cdim=0 : one column of labels, read down from anchor until blank
  set, rdim=2, cdim=0 : two columns of label pairs
  par, rdim=0, cdim=0 : scalar at anchor cell
  par, rdim=1, cdim=0 : labels in anchor column, values one column right
  par, rdim=0, cdim=1 : column labels in anchor row, values in next row
  par, rdim=1, cdim=1 : corner at anchor; col labels right of anchor in anchor
                        row, row labels below anchor in anchor column
  par, rdim=2, cdim=1 : two row-label columns; col labels start 2 right of anchor

All labels are normalized to lowercase strings (GAMS is case-insensitive);
numeric labels like years become '2019'. Zero values are dropped, matching
GDX storage of parameters.
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field

import openpyxl
from openpyxl.utils import column_index_from_string


def norm_label(v) -> str | None:
    """Normalize an Excel cell value to a GAMS label (lowercase str)."""
    if v is None:
        return None
    if isinstance(v, float) and v.is_integer():
        v = int(v)
    s = str(v).strip()
    if s == "":
        return None
    return s.lower()


def _num(v) -> float | None:
    if v is None or isinstance(v, str):
        return None
    try:
        return float(v)
    except (TypeError, ValueError):
        return None


_ANCHOR_RE = re.compile(r"^\s*([^!]+)!\s*([A-Za-z]+)(\d+)\s*$")


@dataclass
class LayoutEntry:
    kind: str          # 'set' or 'par'
    name: str
    sheet: str
    row: int           # 1-based anchor row
    col: int           # 1-based anchor col
    rdim: int
    cdim: int


@dataclass
class Workbook:
    """Thin wrapper caching sheet cell grids (values only)."""
    path: str
    _grids: dict = field(default_factory=dict)

    def __post_init__(self):
        self._wb = openpyxl.load_workbook(self.path, read_only=True, data_only=True)
        self._sheetmap = {ws.title.lower(): ws.title for ws in self._wb.worksheets}

    def grid(self, sheet: str):
        key = sheet.lower()
        if key not in self._grids:
            title = self._sheetmap[key]
            ws = self._wb[title]
            self._grids[key] = [list(r) for r in ws.iter_rows(values_only=True)]
        return self._grids[key]

    def cell(self, grid, r, c):
        """1-based access into a cached grid; None if out of range."""
        if r - 1 >= len(grid):
            return None
        row = grid[r - 1]
        if c - 1 >= len(row):
            return None
        return row[c - 1]


def parse_layout(wb: Workbook, layout_sheet: str = "layout") -> list[LayoutEntry]:
    grid = wb.grid(layout_sheet)
    entries = []
    for row in grid:
        if not row or norm_label(row[0]) not in ("set", "par"):
            continue
        kind = norm_label(row[0])
        name = norm_label(row[1])
        m = _ANCHOR_RE.match(str(row[2]))
        if not m:
            raise ValueError(f"bad anchor for {name}: {row[2]!r}")
        sheet, colstr, rowstr = m.group(1), m.group(2), m.group(3)
        rdim = int(row[3]) if row[3] is not None else 0
        cdim = int(row[4]) if row[4] is not None else 0
        entries.append(LayoutEntry(kind, name, sheet.strip(),
                                   int(rowstr), column_index_from_string(colstr.upper()),
                                   rdim, cdim))
    return entries


# A run of this many consecutive blank cells ends a parameter data block.
# Tolerating a single isolated blank matches the layout of sheets whose label
# column has a one-row gap (e.g. activities a-met | <blank> | a-oman), while
# still stopping before annotation blocks stacked well below the data (which
# are separated by many blank rows, e.g. qfacgrw "Original data").
_BLOCK_GAP = 2


def _labels_down(wb, grid, r0, c, block=False):
    """Read labels going down from (r0, c).

    block=False (sets): skip all blank cells to the end of the used range.
    block=True (parameters): read the contiguous data block, stopping once
      _BLOCK_GAP consecutive blank cells are seen.
    """
    out, blanks = [], 0
    for r in range(r0, len(grid) + 1):
        lab = norm_label(wb.cell(grid, r, c))
        if lab is None:
            blanks += 1
            if block and blanks >= _BLOCK_GAP:
                break
            continue
        blanks = 0
        out.append((r, lab))
    return out


def _labels_right(wb, grid, r, c0, block=False):
    width = max((len(row) for row in grid), default=0)
    out, blanks = [], 0
    for c in range(c0, width + 1):
        lab = norm_label(wb.cell(grid, r, c))
        if lab is None:
            blanks += 1
            if block and blanks >= _BLOCK_GAP:
                break
            continue
        blanks = 0
        out.append((c, lab))
    return out


def read_entry(wb: Workbook, e: LayoutEntry):
    """Return a list of labels / label-tuples for sets, or a dict for pars.

    Sets: rdim=1 -> list[str]; rdim=2 -> list[(str, str)]
    Pars: dict[tuple[str, ...], float]; scalar -> dict[(), float]
    """
    grid = wb.grid(e.sheet)
    r0, c0 = e.row, e.col

    if e.kind == "set":
        if e.rdim == 1:
            return [lab for _, lab in _labels_down(wb, grid, r0, c0)]
        if e.rdim == 2:
            out = []
            for r in range(r0, len(grid) + 1):
                l1 = norm_label(wb.cell(grid, r, c0))
                l2 = norm_label(wb.cell(grid, r, c0 + 1))
                if l1 is not None and l2 is not None:
                    out.append((l1, l2))
            return out
        raise NotImplementedError(f"set rdim={e.rdim} for {e.name}")

    # parameters
    vals: dict[tuple, float] = {}
    if e.rdim == 0 and e.cdim == 0:
        v = _num(wb.cell(grid, r0, c0))
        if v is not None and v != 0:
            vals[()] = v
    elif e.rdim == 1 and e.cdim == 0:
        for r, lab in _labels_down(wb, grid, r0, c0, block=True):
            v = _num(wb.cell(grid, r, c0 + 1))
            if v is not None and v != 0:
                vals[(lab,)] = v
    elif e.rdim == 0 and e.cdim == 1:
        for c, lab in _labels_right(wb, grid, r0, c0, block=True):
            v = _num(wb.cell(grid, r0 + 1, c))
            if v is not None and v != 0:
                vals[(lab,)] = v
    elif e.rdim == 1 and e.cdim == 1:
        cols = _labels_right(wb, grid, r0, c0 + 1, block=True)
        rows = _labels_down(wb, grid, r0 + 1, c0, block=True)
        for r, rl in rows:
            for c, cl in cols:
                v = _num(wb.cell(grid, r, c))
                if v is not None and v != 0:
                    vals[(rl, cl)] = v
    elif e.rdim == 2 and e.cdim == 0:
        for r in range(r0, len(grid) + 1):
            l1 = norm_label(wb.cell(grid, r, c0))
            l2 = norm_label(wb.cell(grid, r, c0 + 1))
            if l1 is not None and l2 is not None:
                v = _num(wb.cell(grid, r, c0 + 2))
                if v is not None and v != 0:
                    vals[(l1, l2)] = v
    elif e.rdim == 2 and e.cdim == 1:
        cols = _labels_right(wb, grid, r0, c0 + 2, block=True)
        for r in range(r0 + 1, len(grid) + 1):
            l1 = norm_label(wb.cell(grid, r, c0))
            l2 = norm_label(wb.cell(grid, r, c0 + 1))
            if l1 is not None and l2 is not None:
                for c, cl in cols:
                    v = _num(wb.cell(grid, r, c))
                    if v is not None and v != 0:
                        vals[(l1, l2, cl)] = v
    else:
        raise NotImplementedError(f"par rdim={e.rdim} cdim={e.cdim} for {e.name}")
    return vals


def load_workbook_symbols(path: str):
    """Load all symbols per the layout sheet.

    Returns (sets, pars): dicts keyed by lowercase symbol name.
    Duplicate layout lines for the same target (e.g. obs from hhdsurvey)
    keep the first definition and union later ones for sets.
    """
    wb = Workbook(path)
    sets: dict[str, list] = {}
    pars: dict[str, dict] = {}
    for e in parse_layout(wb):
        data = read_entry(wb, e)
        if e.kind == "set":
            if e.name in sets:
                seen = set(sets[e.name])
                sets[e.name].extend([x for x in data if x not in seen])
            else:
                sets[e.name] = data
        else:
            pars[e.name] = data
    return sets, pars
