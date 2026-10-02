"""Diagnose the square partition: pair each equation with the variable entry
it solves for, and report any variable that is free-but-unmatched (excess) or
fixed-but-referenced-with-no-defining-equation.

Prints per-family balance so the remaining imbalance is easy to localize.
"""
import sys, os
from collections import Counter
sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from gemcore.database import load_database, bdi2019_data2
from gemcore.calibration import calibrate
from gemcore.state import build_state, Closure
from gemcore.model import residuals
from gemcore.solver import base_closure_fixed, zero_fixed

DATA = os.path.join(os.path.dirname(__file__), "..", "..",
                    "model", "user-files", "bdi2019", "bdi2019-data.xlsx")

# equation-family -> variable-family it solves for (the MCP pairing)
EQ_TO_VAR = {
    "WFADEF": "WFA", "PRODFN": "QA", "FACDEM": "QF", "FACDEMLEO": "QF",
    "PRODFNCES2": "QF", "FACDEMCES2": "QF", "PRODFNCES3": "QF",
    "FACDEMCES3": "QF", "TFPDEF": "TFP", "FPRDADEF": "FPRDA",
    "INTDEM": "QINT", "COMPRDFN": "QXAC", "OUTAGGFN": "QX",
    "OUTAGGFOC": "PXAC", "PVADEF": "PVA", "PADEF": "PA",
    "WAGECURVE": "UERAT", "FACEQ": "QFS", "YFDEF": "YF", "YCAPGDEF": "YF",
    "ARMING": "QQ", "ARMING2": "QQ", "IMPDOMRAT": "QM", "PDDDEF": "PDD",
    "ABSORB": "PQS", "PQDDEF": "PQD", "CET": "QX", "CET2": "QX",
    "EXPDOMRAT": "QE", "ESUPPLYEXOG": "QE", "OUTVAL": "PX",
    "QMCONST": "QM", "IMPQUOTARENT": "YPRQMBART",
    "HHDIMPQUOTARENT": "YPRQMBAR", "ACTIMPQUOTARENT": "YPRQMBAR",
    "GOVIMPQUOTARENT": "YPRQMBAR", "INVIMPQUOTARENT": "YPRQMBAR",
    "FOREXRENT": "YPREXRT", "FOREXRENTALLOC": "YPREXR",
    "PMDEF": "PM", "PEDEF": "PE", "EDEMAND": "QE", "QTDEM": "QT",
    "SHIFDEF": "SHIF", "YIFDEF": "YIF", "YIDEF": "YI", "MPSDEF": "MPS",
    "INSSAVDEF": "SAV", "TRIIDEF": "TRII", "EHDEF": "EH", "HHDDEM": "QH",
    "ENGODEF": "QNGOSCAL", "NGODEM": "QNGO", "TRSTDEM": "QTRST",
    "TRSMRECDEF": "TRSMREC", "GOVREV": "YG", "TYDEF": "TY", "TFDEF": "TF",
    "TADEF": "TA", "TQDEF": "TQ", "TEDEF": "TE", "TMDEF": "TM",
    "TVACDEF": "TVAC", "TFADEF": "TFA", "VATREBATEDEF": "RBTVAT",
    "SUBCDEF": "SUBC", "YTAXEXPDEF": "YTAXEXP", "YTARIMPDEF": "YTAXIMP",
    "YTAXVATDEF": "YTAXVAT", "GOVEXP": "EG", "GOVDEM": "QG",
    "SUBCTDEF": "SUBCT", "TRHROWDEF": "TRNSFR", "TRINSDNHROWDEF": "TRNSFR",
    "TRFACROWDEF": "TRNSFR", "TRHGOVDEF": "TRNSFR",
    "TRINSDNHGOVDEF": "TRNSFR", "TRROWGOVDEF": "TRNSFR",
    "TRGOVROWDEF": "TRNSFR", "GOVPRIMDEF": "GPRIMDEF",
    "GOVPRIMDEFREALDEF": "RGPRIMDEF", "GOVINVCOST": "INVVALG",
    "GOVCAPACC": "NDFG", "GOVNETDOMFINREAL": "RNDFG",
    "NGOVINVFIN": "INVVAL", "NGOVINVCOST": "PK", "NGOVNETFORFIN": "NFFINS",
    "DKGOVDEF": "DKINS", "DKNGOVDEF": "DKINS", "DKROWDEF": "DKINS",
    "INVVALFDEF": "INVVALF", "PCAPDEF": "PK", "INVDEM": "QINV",
    "CAPACCUMNGOVDOM": "QFINS", "CAPACCUMNGOVHHD": "QFHEND",
    "CAPREDIST": "QFINS", "CAPREDISTCONST": "QFHENDSCAL",
    "CAPACCUMNGOVFOR": "QFINS", "CAPACCUMGOV": "QFINS",
    "LABENDOWDEF": "QFINS", "OTHENDOWDEF": "QFINS", "FACSUP": "QFS",
    "LABPARTRATDEF": "QLABSCAL", "WFAVGDEF": "WFAVG",
    "NEWCAPALLOC": "DKA", "CAPACCUMACT": "QF", "COMEQ": "QQ",
    "CURACC": "SAVF", "CAPACC": "WALRAS", "CPIDEF": "CPI", "DPIDEF": "DPI",
    "REXRDEF": "REXR", "GDPREALFCDEF": "RGDPFC", "GDPMPDEF": "GDPMP",
    "GDPREALMPDEF": "RGDPMP", "GDPPCREALDEF": "RGDPPC",
    "TRDGDPDEF": "TRDGDP", "ABSNOMDEF": "ABSNOM",
    "GOVRECGDPDEF": "GOVRECGDP", "GOVSPNDGDPDEF": "GOVSPNDGDP",
    "NGOVPAYGDPDEF": "NGOVPAYGDP", "GOVRECABSDEF": "GOVRECABS",
    "GOVSPNDABSDEF": "GOVSPNDABS", "NGOVPAYABSDEF": "NGOVPAYABS",
    "GOVDOMBOR": "GBOR", "GOVDOMDEBT": "GDEBT", "GOVFORBOR": "FBOR",
    "NGOVFORBOR": "FBOR", "FORDEBT": "FDEBT",
}


def main():
    db = load_database(DATA, data2_hook=bdi2019_data2)
    cal = calibrate(db)
    t = cal.db.sets["tmin"][0]
    cal._solve_t = t
    V = build_state(cal, t)
    clo = Closure(cal, dcal01=False)
    R = residuals(V, cal, t, {}, clo)
    eqkeys = [k for k in R if R[k] is not None]

    allentries = {(n, i) for n, d in V.items() for i in d}
    fixed = (base_closure_fixed(cal, t) | zero_fixed(cal, t, V)) & allentries
    fixed.add(("TFPSCAL", t)); fixed.discard(("RGDPFC", t))
    free = allentries - fixed

    eqfam = Counter(k[0] for k in eqkeys)
    freefam = Counter(n for n, i in free)

    # for each var family, how many equations claim to solve it
    solved_by = Counter()
    for k in eqkeys:
        vf = EQ_TO_VAR.get(k[0])
        if vf:
            solved_by[vf] += 1

    # The verdict that matters: the partition the solver actually uses.
    # (The family table below is the older, base-year-only diagnostic; its
    # "gap" is the predetermined-stock equations and closure-fixed entries it
    # does not account for.)
    from gemcore.partition import build_partition
    from gemcore.solver import closure_fixed
    d = {}
    pfree, pfixed, kept, allkeys = build_partition(cal, t, V, {}, clo,
                                                   closure_fixed(cal, t, clo),
                                                   diag=d)
    print(f"build_partition at {t}: free = {len(pfree)}  kept equations = "
          f"{len(kept)}  ({'square' if len(pfree) == len(kept) else 'NOT SQUARE'});"
          f"  orphan equations = {len(d.get('orphan_eqs', []))}"
          f"  improvised pairs = {len(d.get('improvised', []))}"
          f"  strong unpaired = {len(d.get('strong_unpaired', []))}")
    if d.get("orphan_eqs"):
        print("  orphans:", d["orphan_eqs"][:10])
    print()
    print(f"free vars = {len(free)}   equations = {len(eqkeys)}   "
          f"gap = {len(free) - len(eqkeys)}  (old diagnostic; see above)\n")
    print(f"{'var family':<14}{'free':>6}{'#eqs solving it':>18}{'diff':>7}")
    print("-" * 48)
    fams = sorted(set(freefam) | set(solved_by))
    for vf in fams:
        fr, sv = freefam.get(vf, 0), solved_by.get(vf, 0)
        d = fr - sv
        flag = "  <--" if d != 0 else ""
        print(f"{vf:<14}{fr:>6}{sv:>18}{d:>7}{flag}")


if __name__ == "__main__":
    main()
