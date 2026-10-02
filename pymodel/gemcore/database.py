"""GEM-Core database construction: replicates data.gms (post-GDX load
processing) and the app-specific bdi2019-data2.inc adjustments.

The result is a `Database` object holding:
  .sets : dict[str, list[str]]            (order-preserving)
  .maps : dict[str, list[tuple]]          (2-dim sets / mappings)
  .pars : dict[str, dict[tuple, float]]   (sparse parameters)
plus convenience accessors (.sam(i, j), .scal(name)).

Processing order follows data.gms exactly:
  load -> fixed sets -> complete mapaggreg -> app data2 adjustments ->
  zero SAM diagonal -> time sets -> derived sets -> aggregation
  (SAM, qfbase, pop0, qemibase, mobsh) -> zero diagonal -> ct ->
  scaling defaults -> rescale -> balance check (sambal) -> final subsets.
"""

from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass, field

from .xlload import load_workbook_symbols

TOL_SAMBAL = 1e-4


def _sparse():
    return defaultdict(float)


@dataclass
class Database:
    sets: dict = field(default_factory=dict)
    maps: dict = field(default_factory=dict)
    pars: dict = field(default_factory=dict)
    capest: int = 1

    # -- convenience -------------------------------------------------
    def sam(self, i, j):
        return self.pars["sam"].get((i, j), 0.0)

    def scal(self, name):
        return self.pars["scaling"].get((name,), 1.0)

    def par(self, name):
        return self.pars.setdefault(name, {})

    def in_set(self, name, el):
        return el in self._setlookup(name)

    def _setlookup(self, name):
        key = "_" + name
        cache = self.__dict__.setdefault("_lookups", {})
        s = self.sets.get(name, [])
        if key not in cache or len(cache[key]) != len(s):
            cache[key] = set(s)
        return cache[key]


def _add_elements(lst, new):
    seen = set(lst)
    for x in new:
        if x not in seen:
            lst.append(x)
            seen.add(x)


def sam_row_col_check(sam, acnt):
    """balchk(a) = column sum - row sum, matching GAMS
    SAMBALCHK(ACNT) = SAM('TOTAL',ACNT) - SAM(ACNT,'TOTAL')."""
    rowsum = defaultdict(float)
    colsum = defaultdict(float)
    for (i, j), v in sam.items():
        if i in acnt and j in acnt:
            rowsum[i] += v
            colsum[j] += v
    return {a: colsum[a] - rowsum[a] for a in acnt}, rowsum, colsum


def load_database(xlsx_path: str, data2_hook=None, overrides=None) -> Database:
    """Load the workbook and replay data.gms.

    `overrides` is a dict of parameter name -> {key: value} merged into
    `db.pars` BEFORE the app hook runs, for the few inputs the hook itself
    consumes. The only one today is `prexr000`, the base-year FX premium the
    hook hardcodes at 2.5; the backcast passes `{"prexr000": {(): 1.583}}` to
    build the SAM on the actual 2019 premium instead.
    """
    raw_sets, raw_pars = load_workbook_symbols(xlsx_path)

    db = Database()
    # split 1-dim sets from 2-dim maps
    for name, data in raw_sets.items():
        if data and isinstance(data[0], tuple):
            db.maps[name] = list(data)
        else:
            db.sets[name] = list(data)
    # 2-dim sets loaded from empty sheets come through as [] in raw_sets;
    # route known mappings to maps
    for m in ("mapaggreg", "mfcapinv", "mcapins", "mtaxfa", "mf2f1",
              "mf3f2", "msubcom", "mobsh", "macacrep"):
        if m not in db.maps:
            db.maps[m] = [tuple(x) if isinstance(x, tuple) else x
                          for x in raw_sets.get(m, [])]
            db.sets.pop(m, None)
    db.pars = {k: dict(v) for k, v in raw_pars.items()}
    for k, v in (overrides or {}).items():
        db.pars.setdefault(k, {}).update(v)
    # msubcom/mtaxfa may arrive as parameters (3-col layout) -- treat keys as pairs
    for m in ("msubcom", "mtaxfa"):
        if m in db.pars and db.pars[m]:
            db.maps[m] = list(db.pars.pop(m).keys())

    S, M, P = db.sets, db.maps, db.pars

    # -- fixed set elements declared inline in data.gms ---------------
    S["ins2"] = ["govz", "ngovz", "rowz"]
    S["ghg"] = ["co2", "ch4", "n2o", "ch4+n2o"]
    S["acscal"] = ["samsol", "samrep", "samto1", "samsolrep",
                   "qlabsol", "qlabrep", "qlabto1", "qlabsolrep",
                   "emisol", "emirep", "emito1", "emisolrep"]
    S["acpop"] = ["agelab"]
    S["acpov"] = ["approach", "welfareindex", "p0", "p0elas", "gini"]
    S["alpha"] = ["0", "1", "2"]
    S["acgovspnd"] = ["trngovgov", "trrowgov", "congov"]
    S["acgovrec"] = ["trgovngov", "trgovrow", "netforfingov", "netdomfin"]
    S["acngovpay"] = ["trngovrow", "trrowngov", "trfacrow", "trrowfac",
                      "savngov", "netforfinngov", "fdi", "tourismrec"]
    S["actrnsfr"] = ["trngovrow", "trfacrow", "trngovgov", "trrowgov",
                     "trgovrow", "trrowfac"]
    S["isurvey"] = ["popwt", "welfare", "povline", "povline_ext"]
    S.setdefault("acrep", [])
    _add_elements(S["acrep"], ["tot-lab", "tot-tax", "trgov", "trrow",
                               "trinsdng", "imports_alt", "exports_alt",
                               "total3"])

    # -- $LOADDC merges (data.gms lines 680-745) ----------------------
    _add_elements(S["acgovrec"], S.get("actax", []))
    _add_elements(S["acgovspnd"], S.get("subcom", []))
    _add_elements(S["acgovspnd"], S.get("fcapg", []))
    _add_elements(S["acngovpay"], S.get("fcapng", []))
    S["a2"] = ["all"] + S["a"]
    S["c2"] = ["all"] + S["c"]

    # universe: 'ac' from global-set sheet, merged with the elements
    # declared inline in the data.gms ac declaration (lines 126-232)
    _add_elements(S["ac"], [
        "govz", "ngovz", "rowz",
        "sigma_q", "sigma_x", "eta_e",
        "trdgdp", "kappa", "netprfrat",
        "gdp", "qlab", "prd",
        "samsol", "samrep", "samto1", "samsolrep",
        "qlabsol", "qlabrep", "qlabto1", "qlabsolrep",
        "emisol", "emirep", "emito1", "emisolrep",
        "co2", "ch4", "n2o", "ch4+n2o",
        "trngovgov", "trrowgov", "congov",
        "trgovngov", "trgovrow", "netforfingov", "netdomfin",
        "trngovrow", "trrowngov", "trfacrow", "trrowfac",
        "savngov", "netforfinngov", "fdi", "tourismrec",
        "uerat00", "eta_wf",
        "tot-lab", "tot-capng", "tot-tax", "trgov", "trrow", "trinsdng",
        "imports_alt", "exports_alt",
        "agelab", "nation", "urban", "rural",
        "approach", "welfareindex", "p0", "p0elas", "gini",
        "popwt", "welfare", "povline", "povline_ext",
        "rebate-vat",
        "all", "total"])

    # -- time sets -----------------------------------------------------
    t = S["t"]
    S["tsol"] = list(S.get("tsol", []))
    dmod = P.get("dmod", {}).get((), 0.0)
    if not dmod:
        S["tsol"] = t[:1]
    S["tmin"] = t[:1]
    S["tmax"] = S["tsol"][-1:]
    S["tnmin"] = [x for x in S["tsol"] if x != t[0]]

    # -- default zero params (data.gms lines 839-863) -------------------
    P.setdefault("ced01", {})
    P.setdefault("cesexog01", {})
    S["cfood"] = []
    S["cplext"] = []

    # acnt: all ac except 'total'
    S["acnt"] = [x for x in S["ac"] if x != "total"]

    # -- complete mapaggreg with identity (line 866) --------------------
    mapagg = list(M.get("mapaggreg", []))
    mapped_sources = {src for src, _ in mapagg}
    for acel in S["acnt"]:
        if acel not in mapped_sources:
            mapagg.append((acel, acel))
    M["mapaggreg"] = mapagg

    # -- app-specific data2 include (line 875) --------------------------
    if data2_hook is not None:
        data2_hook(db)

    # -- SAM diagonal zero (line 884) ------------------------------------
    sam = _sparse()
    for (i, j), v in P["sam"].items():
        if i != j and i != "total" and j != "total" and v != 0:
            sam[(i, j)] = v
    P["sam"] = sam

    # -- derived institution/factor sets (lines 915-966) -----------------
    _add_elements(S["acpop"], S["h"])
    S["fncap"] = [f for f in S["f"] if f not in set(S["fcap"])]
    S["capinsdng"] = [x for x in S["capinsd"] if x not in set(S["capgov"])]
    S["capinsng"] = [x for x in S["capins"] if x not in set(S["capgov"])]
    S["caprow"] = [x for x in S["capins"] if x not in set(S["capinsd"])]

    S["d"] = (S["a"] + S["tacd"] + S["tacm"] + S["tace"] + S["insd"]
              + S["fcap"] + S["dstk"] + S["instrst"])

    hset = set(S["h"])
    S["insnh"] = [x for x in S["ins"] if x not in hset]
    S["insdnh"] = [x for x in S["insd"] if x not in hset]
    gov = set(S["insgov"])
    S["insdngnh"] = [x for x in S["insd"] if x not in hset and x not in gov]
    S["insng"] = [x for x in S["ins"] if x not in gov]
    S["insent"] = [x for x in S["insdng"]
                   if x not in hset and x not in set(S["insngo"])]

    S["actaxc"] = (S.get("taxvatc", []) + S.get("taxcom", [])
                   + S.get("taximp", []) + S.get("taxexp", []))

    facclos0 = P.get("facclos0", {})
    S["fuendog"] = [f for f in S["f"] if facclos0.get((f,), 0) == 4]

    # -- aggregate SAM via mapaggreg (lines 1010-1013) --------------------
    agg_of = defaultdict(list)   # source -> targets (usually 1)
    for src, tgt in M["mapaggreg"]:
        agg_of[src].append(tgt)

    def aggregate2(par):
        out = _sparse()
        for (i, j), v in par.items():
            if i == "total" or j == "total" or v == 0:
                continue
            for ti in agg_of.get(i, []):
                for tj in agg_of.get(j, []):
                    out[(ti, tj)] += v
        return dict(out)

    P["sam"] = aggregate2(P["sam"])

    # qfbase(f,a) aggregation (line 1027)
    fset, aset = set(S["f"]), set(S["a"])
    qf_new = _sparse()
    for (i, j), v in P.get("qfbase", {}).items():
        for ti in agg_of.get(i, []):
            for tj in agg_of.get(j, []):
                if ti in fset and tj in aset:
                    qf_new[(ti, tj)] += v
    P["qfbase"] = dict(qf_new)

    # pop0 aggregation (line 1037): first index only
    pop_new = _sparse()
    for (i, tt), v in P.get("pop0", {}).items():
        for ti in agg_of.get(i, []):
            pop_new[(ti, tt)] += v
    P["pop0"] = dict(pop_new)

    # qemibase aggregation (line 1051): dims 2 and 3
    emi_new = _sparse()
    for (g, i, j), v in P.get("qemibase", {}).items():
        for ti in agg_of.get(i, []):
            for tj in agg_of.get(j, []):
                emi_new[(g, ti, tj)] += v
    P["qemibase"] = dict(emi_new)

    # mobsh aggregation (lines 1066-1069)
    mobs_new = []
    for obs, acel in M.get("mobsh", []):
        for tgt in agg_of.get(acel, []):
            if tgt in hset:
                mobs_new.append((obs, tgt))
    M["mobsh"] = mobs_new

    # zero diagonal again (line 1071)
    P["sam"] = {(i, j): v for (i, j), v in P["sam"].items() if i != j and v != 0}

    # -- ct(c): commodities paying margins (line 1075) --------------------
    margin_acs = set(S["tacd"]) | set(S["tacm"]) | set(S["tace"])
    S["ct"] = [c for c in S["c"]
               if any(P["sam"].get((c, m), 0) for m in margin_acs)]

    # -- scaling defaults and rescale (lines 1081-1110) --------------------
    scaling = P.setdefault("scaling", {})
    for base in ("sam", "qlab", "emi"):
        for suff in ("sol", "rep", "to1"):
            scaling.setdefault((base + suff,), 1.0)
        scaling[(base + "solrep",)] = (scaling[(base + "sol",)]
                                       / scaling[(base + "rep",)])
    samsol = scaling[("samsol",)]
    qlabsol = scaling[("qlabsol",)]
    emisol = scaling[("emisol",)]

    P["sam"] = {k: v / samsol for k, v in P["sam"].items()}
    P["debt00"] = {k: v / samsol for k, v in P.get("debt00", {}).items()}
    flabset = set(S["flab"])
    P["qfbase"] = {k: (v / qlabsol if k[0] in flabset else v)
                   for k, v in P["qfbase"].items()}
    P["pop0"] = {k: v / qlabsol for k, v in P["pop0"].items()}
    P["qemibase"] = {k: v / emisol for k, v in P["qemibase"].items()}

    # -- balance check / sambal (lines 1114-1132) ---------------------------
    acnt_set = set(S["acnt"])
    balchk, rowsum, colsum = sam_row_col_check(P["sam"], acnt_set)
    sumabsdev = sum(abs(v) for v in balchk.values())
    if sumabsdev > 1e-6:
        raise RuntimeError(
            f"SAM imbalance {sumabsdev:.3e} exceeds tolerance; "
            "cross-entropy rebalancing (sambal.inc) required but not "
            "implemented -- inspect the data.")
    worst = max((abs(v) for v in balchk.values()), default=0.0)
    assert worst <= TOL_SAMBAL, f"SAM unbalanced: {worst}"

    # -- final subsets (lines 1135-1153) -------------------------------------
    # mtaxvatc(c, insd): demander insd pays VAT on c
    M["mtaxvatc"] = [
        (c, insd) for c in S["c"] for insd in S["insd"]
        if P["sam"].get((c, insd), 0)
        and any(P["sam"].get((tv, c), 0) for tv in S.get("taxvatc", []))]

    S["acsam"] = [x for x in S["acnt"]
                  if any(P["sam"].get((y, x), 0) for y in S["acnt"])]

    f123 = set(S.get("f1", [])) | set(S.get("f2", [])) | set(S.get("f3", []))
    fcapg = set(S.get("fcapg", []))
    S["fva"] = [f for f in S["f"] if f in f123 and f not in fcapg]

    S["fsam"] = [f for f in S["f"]
                 if any(P["sam"].get((f, a), 0) for a in S["a"])]
    S["fnsam"] = [f for f in S["f"] if f not in set(S["fsam"])]

    return db


# ---------------------------------------------------------------------------
# bdi2019-data2.inc replication
# ---------------------------------------------------------------------------

def bdi2019_data2(db: Database):
    """Replicates user-files/bdi2019/bdi2019-data2.inc (active code only)."""
    db.capest = 2
    S, M, P = db.sets, db.maps, db.pars

    # complete mapaggreg (already done pre-hook) and aggregate SAM now,
    # exactly as data2.inc does before the exchange-premium adjustments.
    agg_of = defaultdict(list)
    for src, tgt in M["mapaggreg"]:
        agg_of[src].append(tgt)
    out = _sparse()
    for (i, j), v in P["sam"].items():
        if i == "total" or j == "total" or v == 0:
            continue
        for ti in agg_of.get(i, []):
            for tj in agg_of.get(j, []):
                out[(ti, tj)] += v
    sam = out

    # change place for change in international reserves
    sam[("cap-fin", "cap-ngov")] = sam[("cap-row", "cap-ngov")]
    sam[("cap-row", "cap-fin")] = sam[("cap-row", "cap-ngov")]
    sam[("cap-row", "cap-ngov")] = 0.0

    # move private capital payments in a-admpub to government capital
    sam[("f-capgov", "a-admpub")] = sam[("f-capprv", "a-admpub")]
    sam[("f-capprv", "f-capgov")] = sam[("f-capprv", "a-admpub")]
    sam[("f-capprv", "a-admpub")] = 0.0

    # -- rents from exchange rate premium --------------------------------
    # data2.inc declares `prexr000 /2.5/`, a parallel/official RATIO with no
    # derivation comment. The authors' Figure 4.3 reports it as "parallel 2.5,
    # relative to the official rate", so 2.5 is deliberate -- but it is the
    # late-2024/2025 value, not 2019's (1.583, WB FPG / IMF). Overridable via
    # load_database(overrides={"prexr000": {(): x}}).
    prexr000 = P.get("prexr000", {}).get((), 2.5)
    shrom = P.get("shrom000", {})
    shroe = P.get("shroe000", {})
    C = S["c"]

    for c in C:
        val = ((1 - shrom.get((c,), 0.0)) * (prexr000 - 1) * sam[("row", c)]
               - (1 - shroe.get((c,), 0.0)) * (prexr000 - 1) * sam[(c, "row")])
        sam[("prexr", c)] = val

    sam[("f-capprv", "prexr")] = sum(sam[("prexr", c)] for c in C)

    acfin = ["h-rur", "h-urb", "gov", "inv-prv", "inv-gov", "dstk"]
    demshr = {}
    for c in C:
        tot = sum(sam[(c, a)] for a in acfin)
        for a in acfin:
            demshr[(c, a)] = sam[(c, a)] / tot if tot else 0.0

    for c in C:
        for a in acfin:
            sam[(c, a)] += sam[("prexr", c)] * demshr[(c, a)]

    sam[("inv-prv", "cap-ngov")] += sum(
        sam[("prexr", c)] * demshr[(c, "inv-prv")] for c in C)
    sam[("dstk", "cap-ngov")] += sum(
        sam[("prexr", c)] * demshr[(c, "dstk")] for c in C)
    sam[("inv-gov", "cap-gov")] += sum(
        sam[("prexr", c)] * demshr[(c, "inv-gov")] for c in C)

    H = S["h"]
    caphtot = sum(sam[("cap-ngov", h)] for h in H)
    extra = sum(sam[("prexr", c)] * (demshr[(c, "inv-prv")]
                                     + demshr[(c, "dstk")]) for c in C)
    for h in H:
        sam[("cap-ngov", h)] += sam[("cap-ngov", h)] / caphtot * extra
    sam[("cap-gov", "gov")] += sum(
        sam[("prexr", c)] * demshr[(c, "inv-gov")] for c in C)

    # rebalance h and gov rows through f-capprv
    acnt = set(S["acnt"])
    balchk, _, _ = sam_row_col_check(sam, acnt)
    for h in H:
        sam[(h, "f-capprv")] += balchk.get(h, 0.0)
    sam[("gov", "f-capprv")] += balchk.get("gov", 0.0)

    for a in S["a"]:
        if sam[("f-capprv", a)] < 0:
            raise RuntimeError(f"negative private capital payment in {a}")

    P["sam"] = {k: v for k, v in sam.items() if v != 0}
