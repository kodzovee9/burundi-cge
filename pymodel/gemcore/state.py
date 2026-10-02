"""Build the level State (V) for a period from the calibrated "0" paths,
and the base Closure object holding the exogenous "bar" parameters and
scaling paths referenced by model.residuals.

At the base year (tmin) V equals the calibrated levels, so evaluating
residuals(V, ...) yields ~0 for every equation -- the transcription check.
"""

from __future__ import annotations

from .calibration import G


class Closure:
    """Holds exogenous bar-parameters and scaling paths, plus the tax/qg
    bases used by the tax-rate and transfer equations. For the base run all
    bars equal calibrated base-year values (flat where GAMS would index by t).
    """

    def __init__(self, cal, dcal01=False):
        self.cal = cal
        self.dcal01 = dcal01
        S = cal.db.sets
        T = cal.TSOL
        tmin = S["tmin"][0]

        # tax bases (bar). GAMS: tyb(ins,t)=TY00(ins) etc., flat baseline.
        self._tyb = {i: cal.TY00.get(i, 0.0) for i in S["insdng"]}
        # par-redefn-0.inc 94 + 161: TY0 is rebased to the solved direct-tax
        # rate and TYSCAL0 is then reset to zero, exactly as for QG/QGSCAL. The
        # reference moves TYSCAL to about 0.995, so leaving the bar at its
        # base-year level while the scenario closure pins TYSCAL at zero would
        # wipe out essentially all direct tax.
        self._tyb0 = getattr(cal, "TY0", None)
        self._tfb = {f: cal.TF00.get(f, 0.0) for f in S["f"]}
        self._tab = {a: cal.TA00.get(a, 0.0) for a in S["a"]}
        self._tqb = {c: cal.TQ00.get(c, 0.0) for c in S["c"]}
        self._teb = {c: cal.TE00.get(c, 0.0) for c in S["c"]}
        self._tmb = {c: cal.TM00.get(c, 0.0) for c in S["c"]}
        self._tvacb = dict(cal.TVAC00)
        self._subcb = dict(cal.SUBC00)
        self._tfab = dict(cal.TFA00)
        self.tvacb_keys = set(self._tvacb.keys())
        self.subcb_keys = set(self._subcb.keys())

        # ty01/tf01/ta01 = 1 baseline
        self._ty01 = {i: 1.0 for i in S["ins"]}
        self._tf01 = {f: 1.0 for f in S["f"]}
        self._ta01 = {a: 1.0 for a in S["a"]}

        # gov consumption 0-1 flag (rate-like, flat); qgb GROWS -> accessor
        self._qgc01 = {(c, tt): (1.0 if cal.QG00.get(c) else 0.0)
                       for c in S["c"] for tt in T}

        # transfers per-capita bar is flat (dmod=1); level bars grow -> accessor
        self._trnsfrpcb = dict(cal.trnsfrpcb00)

        self._qgb = getattr(cal, "qgb0", None)
        self._dkinsb = getattr(cal, "DKINS0", None)
        self._ddkins = getattr(cal, "ddkins", None)
        # par-defn-sim.inc 144-146: fprdab(f,a,t) = fprdab0(f,a,t)*fprdabsim.
        # Flat at one in the base and reference, so this is None unless a
        # scenario shocks labour productivity.
        self._fprdab = getattr(cal, "fprdab_path", None)
        self._qeb_path = getattr(cal, "qeb_path", None)
        self._invvalfb = dict(cal.INVVALF0)
        self._gintrat = dict(cal.gintrat)
        self._fintrat = dict(cal.fintrat)
        self._gdpindex = cal.gdpindex

    # ---- accessors (t-indexed, flat baseline) ----
    def tyb(self, ins, t):
        if self._tyb0 is not None:
            return self._tyb0.get((ins, t), 0.0)
        return self._tyb.get(ins, 0.0)
    def ty01(self, ins, t): return self._ty01.get(ins, 0.0)
    def tfb(self, f, t): return self._tfb.get(f, 0.0)
    def tf01(self, f, t): return self._tf01.get(f, 0.0)
    def tab(self, a, t): return self._tab.get(a, 0.0)
    def ta01(self, a): return self._ta01.get(a, 0.0)
    def tqb(self, c, t): return self._tqb.get(c, 0.0)
    def teb(self, c, t): return self._teb.get(c, 0.0)
    def tmb(self, c, t): return self._tmb.get(c, 0.0)
    def tvacb(self, c, d, t): return self._tvacb.get((c, d), 0.0)
    def subcb(self, c, d, t): return self._subcb.get((c, d), 0.0)
    def tfab(self, f, a, t): return self._tfab.get((f, a), 0.0)
    # time-varying "0" paths (GAMS: xxxb(...,t) = XXX0(...,t)); grow with the
    # GDP index off base-year values.
    def qgb(self, c, t):
        # After par_redefn_0 the government-demand bar is the solved QG rather
        # than a growth-scaled base level (par-redefn-0.inc: qgb0(c,t)=QG0(c,t),
        # QGSCAL0=0), so the reference path is reproduced with QGSCAL back at
        # zero and a scenario moves QGSCAL away from that.
        if self._qgb is not None:
            return self._qgb.get((c, t), 0.0)
        return self.cal.QG00.get(c, 0.0) * self._gdpindex[t]
    def qgc01(self, c, t): return self._qgc01.get((c, t), 0.0)
    def trnsfrpcb(self, h, ins, t): return self._trnsfrpcb.get((h, ins), 0.0)
    def trnsfrb(self, ac, acp, t):
        # a scenario's multiplier (Scenario.trnsfr_ratio), e.g. budget support
        r = getattr(self.cal, "trnsfrb_ratio", {}).get((ac, acp, t), 1.0)
        return self.cal.TRNSFR00.get((ac, acp), 0.0) * self._gdpindex[t] * r
    def nffinsbar(self, t):
        return self.cal.NFFINS00 * self._gdpindex[t]
    def invvalfb(self, t): return self._invvalfb.get(t, 0.0)
    def qdstk(self, c, i2, t):
        """Stock change by commodity and investor. mod.gms 874-875 grows it with
        the GDP index -- qdstk0(c,ins2,t) = qdstk00(c,ins2)*gdpindex(t) -- so
        holding it at the base level understates absorption in every out-year
        (invisible at tmin, where the index is one)."""
        base = self.cal.qdstk00.get((c, i2), 0.0) * self._gdpindex[t]
        # backcast only: observed extra inventory build-up (backcast.set_stock_path)
        add = getattr(self.cal, "qdstk_add", None)
        if add and i2 == "ngovz":
            base += add.get((c, t), 0.0)
        return base

    def ddkins(self, i2, fc, t):
        """Additive shifter on gross capital formation (mod.gms 3038, 3043).

        Zero in the base and reference (`ddkins0 = 0`, mod.gms 1137). A scenario
        sets it to put an exogenous quantity of investment into a stock on top of
        whatever the closure would otherwise deliver -- which is how the
        infrastructure scenarios add their 2%-of-GDP public capital push without
        going through ISCAL or IADJ. It appears only on the government and
        non-government rows; EQ_DKROWDEF has no such term.
        """
        if self._ddkins is None:
            return 0.0
        return self._ddkins.get((i2, fc, t), 0.0)

    def fprdab(self, f, a, t):
        """Factor productivity bar (mod.gms: `FPRDA = fprdab*(1+FPRDASCAL*fprda01)`).

        One everywhere in the base and reference. The `hd` scenarios raise it for
        labour to represent a human-capital gain, which is why it has to be
        t-indexed even though the calibration only ever needs a scalar.
        """
        if self._fprdab is not None:
            v = self._fprdab.get((f, a, t))
            if v is not None:
                return v
        return self.cal.fprdab00.get((f, a), 0.0)

    def qeb(self, c, t):
        """Exogenous export volume for a `cesexog` (or `ced`) commodity.

        mod.gms 1063: `qeb0(c,t) = qeb00(c)*gdpindex(t)` -- the default path
        grows with GDP. A backcast replaces it with the observed volume path
        (`cal.qeb_path`), which is the point of the quantity closure: exports
        of coffee, tea and gold are what they were, and the model works out
        what that implies for everything else.
        """
        if self._qeb_path is not None:
            v = self._qeb_path.get((c, t))
            if v is not None:
                return v
        return self.cal.qeb00.get(c, 0.0) * self._gdpindex[t]

    def dkinsb(self, i2, fc, t):
        # mod.gms 1132-1136: dkinsb0 = DKINS0, which starts as
        # DKINS00*gdpindex(t) but is rebased by par-redefn-0.inc to the solved
        # reference DKINS. Under a scenario that fixes ISCAL (investment rule 1)
        # this bar IS the government investment path, so leaving it at
        # base*index makes government capital formation grow with the GDP index
        # instead of tracking the reference.
        if self._dkinsb is not None:
            return self._dkinsb.get((i2, fc, t), 0.0)
        return self.cal.DKINS00.get((i2, fc), 0.0) * self._gdpindex[t]
    def qfinsb(self, ins, f, t):
        return self.cal.QFINS0.get((ins, f, t), 0.0)
    def gintrat(self, t): return self._gintrat.get(t, 0.0)
    def fintrat(self, i2, t): return self._fintrat.get((i2, t), 0.0)


def build_state(cal, t):
    """Construct V (level dict) at period t from calibrated "0" paths."""
    S = cal.db.sets
    A, C, F, H = S["a"], S["c"], S["f"], S["h"]
    INS, INSD, INSDNG = S["ins"], S["insd"], S["insdng"]
    INSGOV, INSROW, INSNGO, INSTRST = (S["insgov"], S["insrow"],
                                       S["insngo"], S["instrst"])
    FCAP, FCAPG, FCAPNG = S["fcap"], S["fcapg"], S["fcapng"]
    IN2 = S["ins2"]
    ACGOVREC, ACGOVSPND, ACNGOVPAY = (S["acgovrec"], S["acgovspnd"],
                                      S["acngovpay"])
    gi = cal.gdpindex[t]

    V = {}

    def scalar(name, val):
        V[name] = {t: val}

    # scalar (t-indexed) variables
    scalar("CPI", cal.CPI00)
    scalar("DPI", cal.DPI00)
    scalar("EXR", cal.EXR00)
    # REXR0(t) = REXR00 * rexrindex0(t) (mod.gms; the authors' index is flat,
    # so this equals REXR00 in the reference application's runs). Seeding from the path is
    # what lets rowclos 2/3/4 -- which fix REXR at the value found here --
    # follow an exogenous real-exchange-rate path (backcast `--rexr actual`,
    # scenario `rexr_ratio`). Until 2026-09-16 this read REXR00 and the path
    # was silently ignored.
    scalar("REXR", getattr(cal, "REXR0", {}).get(t, cal.REXR00))
    scalar("PREXR", cal.PREXR0[t])
    scalar("GDPMP", cal.GDPMP00 * gi)
    scalar("RGDPMP", cal.RGDPMP00 * gi)
    scalar("RGDPFC", cal.RGDPFC00 * gi)
    # per-capita real GDP tracks the period's own GDP and population, not the
    # base year's: starting it at the base-year level leaves EQ_GDPPCREALDEF
    # with a residual of order RGDPMP in every out-year.
    scalar("RGDPPC", cal.RGDPMP00 * gi / sum(cal.pop[(h, t)] for h in H))
    scalar("ABSNOM", cal.ABSNOM00 * gi)
    scalar("TRDGDP", cal.TRDGDP00)
    scalar("EG", cal.EG00 * gi)
    scalar("YG", cal.YG00 * gi)
    scalar("GPRIMDEF", cal.GPRIMDEF00 * gi)
    scalar("RGPRIMDEF", cal.RGPRIMDEF00 * gi if False else cal.GPRIMDEF00 * gi / cal.CPI00)
    scalar("INVVAL", cal.INVVAL00 * gi)
    scalar("INVVALG", cal.INVVALG00 * gi)
    scalar("INVVALF", cal.INVVALF0[t])
    scalar("NDFG", cal.NDFG00 * gi)
    scalar("RNDFG", cal.NDFG00 * gi / cal.CPI00)
    scalar("NFFG", cal.NFFG00 * gi)
    scalar("NFFINS", cal.NFFINS00 * gi)
    scalar("NFFINSSCAL", 1.0)
    scalar("SAVF", cal.SAVF00 * gi)
    scalar("SUBCT", 0.0)
    scalar("YTAXIMP", cal.YTAXIMP00 * gi)
    scalar("YTAXEXP", cal.YTAXEXP00 * gi)
    scalar("YTAXVAT", cal.YTAXVAT00 * gi)
    scalar("TRSMREC", cal.TRSMREC0[t])
    scalar("GDEBT", cal.GDEBT00 * gi)
    scalar("GBOR", cal.GBOR00 * gi)
    scalar("WALRAS", 0.0)
    scalar("LABPARTRAT", cal.LABPARTRAT0[t])
    # scaling variables (all 1 or 0 baseline)
    for nm, val in (("TFPSCAL", 0.0), ("FPRDASCAL", 0.0), ("QGSCAL", 0.0),
                    ("MPSSCAL", 1.0), ("MPSADJ", 0.0), ("TYSCAL", 0.0),
                    ("TFSCAL", 0.0), ("TASCAL", 0.0), ("TQSCAL", 1.0),
                    ("TESCAL", 1.0), ("TMSCAL", 1.0), ("TVACSCAL", 1.0),
                    ("TFASCAL", 1.0), ("SUBCSCAL", 1.0), ("QLABSCAL", 1.0),
                    ("FDISCAL", 1.0)):
        scalar(nm, val)

    # port-only sector-group productivity shifters (see model.py TFPDEF);
    # absent unless the backcast has declared groups
    groups = getattr(cal, "tfp_group_list", ())
    if groups:
        V["TFPGRP"] = {g: 1.0 for g in groups}

    # port-only fuel mechanisms (gemcore/fuel.py): indices at 1, informal fuel at 0
    fcfg = getattr(cal, "fuel", None)
    if fcfg is not None:
        if fcfg.sigma_act:
            V["FXI"] = {a: 1.0 for a in fcfg.AF}
            V["VXI"] = {a: 1.0 for a in fcfg.AF}
        if fcfg.informal:
            V["QMI"] = {fcfg.fuel: 0.0}
        if fcfg.eps_hh:
            V["HXI"] = {h: 1.0 for h in S["h"]}

    # indexed variables
    V["PA"] = {a: cal.PA00[a] for a in A}
    V["PVA"] = {a: cal.PVA00[a] for a in A}
    V["QA"] = {a: cal.QA00[a] * gi for a in A}
    V["TFP"] = {a: cal.TFP00[a] for a in A}
    V["TA"] = {a: cal.TA00[a] for a in A}
    V["RBTVAT"] = {a: cal.RBTVAT00.get(a, 0.0) * gi for a in A}
    V["PX"] = {c: cal.PX00[c] for c in C}
    V["PDS"] = {c: cal.PDS00[c] for c in C}
    V["PDD"] = {c: G(cal.PDD00, c) for c in C}
    V["PE"] = {c: G(cal.PE00, c) for c in C}
    V["PM"] = {c: G(cal.PM00, c) for c in C}
    V["PQS"] = {c: G(cal.PQS00, c) for c in C}
    V["QX"] = {c: cal.QX00[c] * gi for c in C}
    V["QD"] = {c: cal.QD00[c] * gi for c in C}
    V["QE"] = {c: cal.QE00[c] * gi for c in C}
    V["QM"] = {c: cal.QM00[c] * gi for c in C}
    V["QQ"] = {c: cal.QQ00[c] * gi for c in C}
    V["QT"] = {c: G(cal.QT00, c) * gi for c in C}
    V["QINV"] = {c: G(cal.QINV00, c) * gi for c in C}
    V["QG"] = {c: G(cal.QG00, c) * gi for c in C}
    V["TQ"] = {c: G(cal.TQ00, c) for c in C}
    V["TE"] = {c: G(cal.TE00, c) for c in C}
    V["TM"] = {c: G(cal.TM00, c) for c in C}
    V["PWE"] = {c: cal.PWE0.get((c, t), G(cal.PWE00, c)) for c in C}
    V["PWM"] = {c: cal.PWM0.get((c, t), G(cal.PWM00, c)) for c in C}
    V["PRQMBAR"] = {c: G(cal.PRQMBAR00, c) for c in C}
    V["YPRQMBART"] = {c: cal.YPRQMBART00.get(c, 0.0) * gi for c in C}
    V["YPREXRT"] = {c: cal.YPREXRT00.get(c, 0.0) * gi for c in C}

    V["PXAC"] = {(a, c): 1.0 for (a, c) in cal.QXAC00}
    V["QXAC"] = {(a, c): cal.QXAC00[(a, c)] * gi for (a, c) in cal.QXAC00}
    V["QINT"] = {(c, a): cal.QINT00[(c, a)] * gi for (c, a) in cal.QINT00}
    V["QF"] = {(f, a): cal.QF00[(f, a)] * cal.qfacindex[(f, t)]
               for (f, a) in cal.QF00}
    V["WF"] = {f: cal.WF00.get(f, 0.0) * cal.fprdindex[(f, t)] for f in F}
    V["WFA"] = {(f, a): G(cal.WFA00, f, a) for (f, a) in cal.QF00}
    V["WFDIST"] = {(f, a): G(cal.WFDIST00, f, a) for (f, a) in cal.QF00}
    V["WFAVG"] = {f: cal.WFAVG00.get(f, 0.0) * cal.fprdindex[(f, t)] for f in F}
    V["TFA"] = {(f, a): G(cal.TFA00, f, a) for (f, a) in cal.QF00}
    V["FPRDA"] = {(f, a): cal.FPRDA00.get((f, a), 0.0) for (f, a) in cal.QF00}
    V["UERAT"] = {f: cal.UERAT00.get(f, 0.0) for f in F}
    V["QFS"] = {f: cal.QFS00.get(f, 0.0) * cal.qfacindex[(f, t)] for f in F}
    V["TF"] = {f: G(cal.TF00, f) for f in F}
    V["YF"] = {f: cal.YF00.get(f, 0.0) * gi for f in F}
    V["QFINSSCAL"] = {f: 1.0 for f in F}
    V["ISCAL"] = {fc: 1.0 for fc in FCAP}
    V["PK"] = {fc: cal.PK00[fc] for fc in FCAP}

    V["QFINS"] = {(ins, f): cal.QFINS0.get((ins, f, t), 0.0)
                  for ins in INS for f in F}
    V["SHIF"] = {(ins, f): G(cal.SHIF0, ins, f, t) for ins in INS for f in F}
    V["QFHEND"] = {(h, f): cal.QFHEND0.get((h, f, t), 0.0)
                   for h in H for f in F}
    V["QFHENDSCAL"] = {f: 1.0 for f in F}
    V["YIF"] = {(ins, f): cal.YIF00.get((ins, f), 0.0) * gi
                for ins in INS for f in F}
    V["YI"] = {i: cal.YI00.get(i, 0.0) * gi for i in INSDNG}
    V["TY"] = {ins: G(cal.TY00, ins) for ins in INS}
    V["MPS"] = {i: G(cal.MPS00, i) for i in INSDNG}
    V["SAV"] = {i: cal.SAV00.get(i, 0.0) * gi for i in INSDNG}
    V["TRII"] = {(ins, i): cal.TRII00.get((ins, i), 0.0) * gi
                 for ins in INS for i in INSDNG}
    V["EH"] = {h: cal.EH00[h] * gi for h in H}
    V["QH"] = {(c, h): cal.QH00[(c, h)] * gi for (c, h) in cal.QH00}
    V["QNGO"] = {(c, n): cal.QNGO00[(c, n)] * gi for (c, n) in cal.QNGO00}
    V["QNGOSCAL"] = {n: G(cal.QNGOSCAL00, n) for n in INSNGO}
    V["QTRST"] = {(c, s): cal.QTRST00[(c, s)] * gi for (c, s) in cal.QTRST00}
    V["QTRSTSCAL"] = {t: cal.QTRSTSCAL00}
    V["SUBC"] = {(c, d): cal.SUBC00.get((c, d), 0.0) for (c, d) in cal.SUBC00}
    V["TVAC"] = {(c, d): cal.TVAC00.get((c, d), 0.0) for (c, d) in cal.TVAC00}

    V["TRNSFR"] = {(ac, acp): cal.TRNSFR00.get((ac, acp), 0.0) * gi
                   for (ac, acp) in cal.TRNSFR00}
    V["TRNSFRSCAL"] = {ac: 1.0 for ac in S["actrnsfr"]}

    V["DKINS"] = {(i2, fc): cal.DKINS00.get((i2, fc), 0.0) * gi
                  for i2 in IN2 for fc in FCAP}
    V["DKA"] = {(fc, a): cal.DKA00.get((fc, a), 0.0) * gi
                for fc in FCAPNG for a in A}
    V["IADJ"] = {i2: 0.0 for i2 in IN2}
    V["FBOR"] = {i2: cal.FBOR00.get(i2, 0.0) * gi for i2 in IN2}
    V["FDEBT"] = {i2: cal.FDEBT00.get(i2, 0.0) * gi for i2 in IN2}

    V["GOVRECGDP"] = {ac: G(cal.GOVRECGDP0, ac, t) for ac in ACGOVREC}
    V["GOVSPNDGDP"] = {ac: G(cal.GOVSPNDGDP0, ac, t) for ac in ACGOVSPND}
    V["NGOVPAYGDP"] = {ac: G(cal.NGOVPAYGDP0, ac, t) for ac in ACNGOVPAY}
    V["GOVRECABS"] = {ac: G(cal.GOVRECABS0, ac, t) for ac in ACGOVREC}
    V["GOVSPNDABS"] = {ac: G(cal.GOVSPNDABS0, ac, t) for ac in ACGOVSPND}
    V["NGOVPAYABS"] = {ac: G(cal.NGOVPAYABS0, ac, t) for ac in ACNGOVPAY}

    V["PQD"] = {(c, d): cal.PQD00[(c, d)] for (c, d) in cal.PQD00}

    # exchange-premium and import-quota rent allocations
    ACNT = S["acnt"]
    V["YPREXR"] = {(c, ac): cal.YPREXR00.get((c, ac), 0.0) * gi
                   for c in C for ac in ACNT
                   if cal.shryprexr00.get((c, ac))}
    # YPRQMBAR spans the full commodity x demander domain of the quota-rent
    # allocation equations (0 for non-quota commodities); each entry is solved
    # by its own HHDIMP/ACTIMP/GOVIMP/INVIMP equation.
    A_, INSGOV_ = S["a"], S["insgov"]
    V["YPRQMBAR"] = {}
    for c in C:
        for a in A_:
            V["YPRQMBAR"][(c, a)] = cal.YPRQMBAR00.get((c, a), 0.0) * gi
        for h in H:
            V["YPRQMBAR"][(c, h)] = cal.YPRQMBAR00.get((c, h), 0.0) * gi
        for g in INSGOV_:
            V["YPRQMBAR"][(c, g)] = cal.YPRQMBAR00.get((c, g), 0.0) * gi
        V["YPRQMBAR"][(c, "ngovz")] = cal.YPRQMBAR00.get((c, "ngovz"), 0.0) * gi

    _apply_ref0(cal, t, V)
    _apply_quota_slack(cal, t, V)
    return V


def _apply_quota_slack(cal, t, V):
    """Zero the quota rent on the slack branch of the EQ_QMCONST complementarity.

    `base_closure_fixed` fixes PRQMBAR for a relaxed quota, but it never sees V
    and so can only pin the rent at whatever value is already there -- the
    calibrated rent, or after par_redefn_0 the solved reference rent. Both are
    the BINDING rent, which is the one value the slack branch rules out.

    It is not a small error. Under `uni` the rent on refined petroleum is 2.729,
    a 273% ad-valorem markup carried straight into PM through
    `(1 + TM + PRQMBAR)`, so the commodity whose quota the scenario removes goes
    on being priced as though the quota still bound.
    """
    for (c, tt) in getattr(cal, "quota_slack", ()):
        if tt == t and c in V.get("PRQMBAR", {}):
            V["PRQMBAR"][c] = 0.0
            if c in V.get("YPRQMBART", {}):
                V["YPRQMBART"][c] = 0.0


# After par-redefn-0.inc every "0"-parameter holds the solved reference level,
# and varinit.inc then starts each variable there. Two of those parameters are
# deliberately NOT left at the solved level: the government-demand and
# direct-tax scalars are reset to neutral once their bars have absorbed the
# level (par-redefn-0.inc 58 and 161), so that a scenario moves them away from
# zero rather than compounding on top of the reference. The tax and subsidy
# scalars sit at neutral in the reference anyway -- par_redefn_0 asserts it --
# so listing them here is a no-op that documents the intent.
_REF0_NEUTRAL = {"QGSCAL": 0.0, "TYSCAL": 0.0, "TASCAL": 0.0, "TFSCAL": 0.0,
                 "TQSCAL": 1.0, "TESCAL": 1.0, "TMSCAL": 1.0, "TVACSCAL": 1.0,
                 "TFASCAL": 1.0, "SUBCSCAL": 1.0}

# Variables whose level build_state reads from a "0"-path that a Scenario
# mutates. par-defn-sim.inc applies a shock to the PARAMETER (`prexr(t) =
# prexr0(t)*prexrsim(sim,t)`) and varinit then starts the variable there, so the
# shocked value has to win. Seeding these from the reference silently undoes the
# scenario -- with PREXR pinned by rowclos=1 it makes the whole exchange-rate
# unification a no-op, which reads as a model that simply does not respond.
_REF0_SKIP = {"PREXR", "REXR", "QFS"}


def _apply_ref0(cal, t, V):
    """Seed levels from the reference solution, if one has been rebased in.

    This is what makes a scenario a deviation from the *reference* rather than
    from the original calibration. It matters twice over, because these levels
    are not only the starting guess: `base_closure_fixed` pins a closure-fixed
    variable at whatever value it finds in V. Under the reference closure the
    scaling variables are free, so seeding them from the calibration is
    harmless; under the scenario closure they are fixed, and the calibrated
    values are off by up to 39% (RNDFG 21.17 against a reference 12.82, NFFG
    9.90 against 6.41, MPSSCAL 1 against 1.27). That is enough to stop an
    unshocked scenario year from reproducing the reference at all.
    """
    ref = getattr(cal, "ref0", None)
    if not ref or t not in ref:
        return
    for nm, d in ref[t].items():
        if nm in _REF0_NEUTRAL or nm in _REF0_SKIP or nm not in V:
            continue
        cur = V[nm]
        for idx, val in d.items():
            if idx in cur:
                cur[idx] = val
    for nm, neutral in _REF0_NEUTRAL.items():
        if nm in V and t in V[nm]:
            V[nm][t] = neutral
