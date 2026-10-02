"""Sectoral growth, wages and unemployment by scenario against Tables D.5-D.8.

Uses validation targets in repspec-bdi2019.xlsx that the macro table does not:
sector value added growth (tabsectgrw), the 2040 wage index by labour type
(tabwfindexsim1) and the 2040 unemployment rate (tabuerate). Sectoral growth is
what localises the export gap to particular activities.
"""
import os, sys, time
HERE=os.path.dirname(os.path.abspath(__file__)); ROOT=os.path.dirname(HERE)
sys.path.insert(0, ROOT)
from gemcore.database import load_database, bdi2019_data2
from gemcore.calibration import calibrate
from gemcore.dynamics import run_base, run_reference, par_redefn_0
from gemcore import scenarios as sm
from run import DATA

from run import load_gams_reference

# The targets are the reference application's results (GAMS; Tables D.5, D.6,
# D.8), kept in data/gams-reference.json for internal use. Without that file
# there is nothing to compare with.
_REF = load_gams_reference().get("detail_2040")
if not _REF:
    print("data/gams-reference.json not found: the detail comparison with the "
          "reference application needs it. Nothing to do.")
    sys.exit(0)
G_SEC = {k: tuple(v) for k, v in _REF["sector_va_growth"].items()}
G_WF = {k: tuple(v) for k, v in _REF["wage_index"].items()}
G_UE = {k: tuple(v) for k, v in _REF["unemployment"].items()}
NAMES=["base","uni","uni+inf","uni+inf+hd"]

# --gams-replication: drop this model's additions (fuel channels, FX-linked
# quotas, rent cost) so the comparison is with the reference application
REPL = "--gams-replication" in sys.argv
t0=time.time()
cal=calibrate(load_database(DATA,data2_hook=bdi2019_data2),
              **({"fuel_mech": None} if REPL else {}))
per=[t for t in cal.db.sets["tsol"] if int(t)<=2040]
p1=run_base(cal,dcal01=True,verbose=False,periods=per)
ref=run_reference(cal,p1,verbose=False,periods=per); par_redefn_0(cal,ref,verbose=False)
sols={"base":ref}; snap=sm._snapshot(cal)
for n in NAMES[1:]:
    sm._restore(cal,snap)
    sols[n]=sm.run_scenario(cal,sm.SCENARIOS[n](cal,per),ref,per,verbose=False)
    sm._restore(cal,snap); print(f"{n} done ({time.time()-t0:.0f}s)",flush=True)

def secgrw(V):  # real VA = PVA00*QA, 2026->2040
    return {a:100*((V["2040"]["QA"][a]/V["2026"]["QA"][a])**(1/14)-1) for a in G_SEC}
def wfidx(V): return {f:100*V["2040"]["WF"][f]/V["2025"]["WF"][f] for f in G_WF}
def uerat(V): return {f:100*V["2040"]["UERAT"][f] for f in G_UE}

OUT=os.path.join(ROOT,"reports","detail-validation-2040"+("-gams-replication" if REPL else "")+".md")
md=["# Detail validation: sectors, wages, unemployment (python vs GAMS)\n",
    "Targets from the reference application (`data/gams-reference.json`): Table D.8 (sector value-added growth "
    "2026-2040), Table D.6 (real wage index 2040, 2025 = 100), Table D.5 "
    "(unemployment rate 2040). `combi` is not in these tables.\n"]
def table(title,fn,G,key_w=14):
    print(f"\n=== {title}   (py | GAMS | diff)")
    print(f"{'':<{key_w}}"+"".join(f"{n:>26}" for n in NAMES))
    md.append(f"\n## {title}\n")
    md.append("| | "+" | ".join(f"{n} py | GAMS | diff" for n in NAMES)+" |")
    md.append("| --- |"+" ---: |"*(3*len(NAMES)))
    for k in G:
        line=f"{k:<{key_w}}"; cells=[]
        for i,n in enumerate(NAMES):
            v=fn(sols[n])[k]; g=G[k][i]
            line+=f"{v:>9.3f}{g:>8.3f}{v-g:>+9.3f}"
            cells+=[f"{v:.3f}",f"{g:.3f}",f"{v-g:+.3f}"]
        print(line); md.append(f"| {k} | "+" | ".join(cells)+" |")
table("Sector VA growth 2026-2040 (%), Table D.8",secgrw,G_SEC)
table("Wage index 2040 (2025=100), Table D.6",wfidx,G_WF)
table("Unemployment rate 2040 (%), Table D.5",uerat,G_UE)
with open(OUT,"w") as fh: fh.write("\n".join(md)+"\n")
print(f"\nwrote {OUT}\n[{time.time()-t0:.0f}s]")
