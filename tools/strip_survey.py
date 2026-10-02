"""Copy the calibration workbook with the household-survey records removed.

    python3 tools/strip_survey.py SRC.xlsx DST.xlsx

Works on the .xlsx zip directly rather than through openpyxl: the workbook's
set sheets are formulas (`=A2+1` ...) and the model reads their CACHED values,
which openpyxl drops on re-save. Only the `hhdsurvey` sheet's XML is rewritten
-- the title and header rows are kept, every `obsN` record row is dropped, and
a note is put in the first data row. Every other part of the file is copied
byte for byte.
"""

import re
import sys
import zipfile
from xml.etree import ElementTree as ET

NS = {"m": "http://schemas.openxmlformats.org/spreadsheetml/2006/main",
      "r": "http://schemas.openxmlformats.org/officeDocument/2006/relationships"}
PR = "http://schemas.openxmlformats.org/package/2006/relationships"
NOTE = "(household survey records removed from this copy; not used by the Python model)"


def sheet_part(z, name):
    wb = ET.fromstring(z.read("xl/workbook.xml"))
    rid = None
    for s in wb.find("m:sheets", NS):
        if s.get("name") == name:
            rid = s.get(f"{{{NS['r']}}}id")
    if rid is None:
        raise SystemExit(f"sheet {name!r} not in workbook")
    rels = ET.fromstring(z.read("xl/_rels/workbook.xml.rels"))
    for rel in rels:
        if rel.get("Id") == rid:
            target = rel.get("Target").lstrip("/")
            return target if target.startswith("xl/") else "xl/" + target
    raise SystemExit(f"no relationship for {name!r}")


def strip_sheet(xml: bytes, shared: list[str]) -> tuple[bytes, int]:
    """Drop every row whose first cell is an obsN label; return (xml, dropped)."""
    m = ET.fromstring(xml)
    sd = m.find("m:sheetData", NS)
    dropped, first = 0, None
    for row in list(sd):
        c0 = row.find("m:c", NS)
        v = c0.find("m:v", NS) if c0 is not None else None
        txt = None
        if v is not None:
            txt = shared[int(v.text)] if c0.get("t") == "s" else v.text
        elif c0 is not None and c0.get("t") == "inlineStr":
            txt = "".join(t.text or "" for t in c0.iter(f"{{{NS['m']}}}t"))
        is_rec = bool(txt) and re.match(r"obs\d+$", txt.strip().lower()) is not None
        if is_rec and first is None:
            first = int(row.get("r"))
        # drop the records and everything below them (blank styled rows), so
        # the note row appended next keeps the rows in ascending order
        if first is not None and int(row.get("r")) >= first:
            sd.remove(row)
            dropped += is_rec
    if not dropped:
        raise SystemExit("no obsN rows found in hhdsurvey")
    ET.register_namespace("", NS["m"])
    ET.register_namespace("r", NS["r"])
    note = ET.SubElement(sd, f"{{{NS['m']}}}row", r=str(first))
    c = ET.SubElement(note, f"{{{NS['m']}}}c", r=f"A{first}", t="inlineStr")
    ET.SubElement(ET.SubElement(c, f"{{{NS['m']}}}is"), f"{{{NS['m']}}}t").text = NOTE
    dim = m.find("m:dimension", NS)
    if dim is not None:
        dim.set("ref", f"A1:E{first}")
    return ET.tostring(m, xml_declaration=True, encoding="UTF-8"), dropped


def main(src, dst):
    with zipfile.ZipFile(src) as z:
        part = sheet_part(z, "hhdsurvey")
        ss = ET.fromstring(z.read("xl/sharedStrings.xml"))
        shared = ["".join(t.text or "" for t in si.iter(f"{{{NS['m']}}}t")) for si in ss]
        new_xml, dropped = strip_sheet(z.read(part), shared)
        with zipfile.ZipFile(dst, "w", zipfile.ZIP_DEFLATED) as out:
            for info in z.infolist():
                data = new_xml if info.filename == part else z.read(info.filename)
                out.writestr(info, data)
    print(f"hhdsurvey: dropped {dropped} record rows -> {dst}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    main(sys.argv[1], sys.argv[2])
