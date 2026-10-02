* sens.gms

* write rsens2.cmd batch file 

$SET tempfolder tmp

* set the number of iterations
SET iter /0001*0013/;


FILE f /rsens2.cmd/;
PUT f, "@echo"/
  "del tmp\*.gdx"/
  "del *.gdx"/
  "del *.lst"/
  "md tmp"/

*    "call GAMS repmacro.gms r=save\rep s=save\repmacro gdx=repmacro --iter=",iter.TL/
*    "call GAMS repmeso.gms r=save\repmacro s=save\repmeso gdx=repmeso --iter=",iter.TL/

LOOP(iter,
  PUT 
    "del save\data.*"/
    "del save\mod.*"/
    "del save\sim.*"/
    "del save\rep.*"/
    "del save\repmacro.*"/
    "del save\repmeso.*"/
    "del save\repgtm.*"/
    "del *-data.gdx"/
    "del *-sim.gdx"/
    "del report.gdx"/
    "call GAMS mod.gms s=save\mod gdx=mod  pw=120 ps=9999 --NonIMv2=1 solvelink=5 --SSA=1 --iter=",iter.TL/
    "call GAMS sim.gms r=save\mod s=save\sim gdx=sim --iter=",iter.TL/
    "call GAMS rep.gms r=save\sim s=save\rep gdx=rep --iter=",iter.TL/
    "call copy report.gdx tmp\rep",iter.TL:0,".gdx"/

    ;
);

* merge rep gdx files into one
PUT "call gdxmerge %tempfolder%\rep*.gdx id=modsolstat,modsolwarn,gdpindicXP,QFXP,prodelas2,prodelas,tradelas,leselas"/;
PUT "call copy merged.gdx tmp\merged.gdx";

PUTCLOSE;

* display bath file on screen
EXECUTE "TYPE rsens2.cmd";


