* rep.gms

DISPLAY "#### START: rep.inc";

PARAMETER
 tsolnb       no. of non-base years for simulations between tmin and tmax (used for reports)
 tsolnbrep    no. of non-base years for simulations between tminrep and tmaxrep (used for reports)
;




SET
  simnbase(sim)
  simbase(sim)


;

simnbase(simcur) = YES;
simnbase('base') = NO;
simbase('base')  = YES;

tmaxrep(t)$(NOT tsol(t)) = NO;
tminrep(t)$(NOT SUM(tp, tminrep(tp)) AND tmin(t)) = YES;
tmaxrep(t)$(NOT SUM(tp, tmaxrep(tp)) AND tsol(t) AND tmax(t)) = YES;


$ONTEXT
$IF EXIST isi-data1.inc tminrep(t) = NO;
$IF EXIST isi-data1.inc tmaxrep(t) = NO;
$IF EXIST isi-data1.inc tminrep(t)$tminrep(t) = YES;
$IF EXIST isi-data1.inc tmaxrep(t)$tmaxrep(t) = YES;
$OFFTEXT

SET trepdum(t);
trepdum(t)$trep(t) = YES;

trep(t)$(NOT SUM(tp, trepdum(tp)) AND tminrep(t)) = YES;
trep(t)$(NOT SUM(tp, trepdum(tp)) AND tmaxrep(t)) = YES;

* trep(t)$((ORD(t) GE SUM(tp$tminrep(tp), ORD(tp)))
*         AND
*                (ORD(t) LE SUM(tp$tmaxrep(tp), ORD(tp)))) = YES;
DISPLAY trep;

tavg(t)$(NOT SUM(tp, tavg(tp)) AND tsol(t)) = YES;


tsolnb = SUM(t$tmax(t), ORD(t)) - SUM(t$tmin(t), ORD(t));
tsolnb$(dmod=0) = 1;
DISPLAY tsolnb;


tsolnbrep = SUM(t$tmaxrep(t), ORD(t)) - SUM(t$tminrep(t), ORD(t));
tsolnbrep$(dmod=0) = 1;
DISPLAY tsolnbrep;


* debt-related reports
$INCLUDE repdebtbor.inc


* % deviation from baseline result (name of variable + XP)
$INCLUDE repperc-xp.inc

* average % annual growth rates bt tmin and tmax (name of variable + XPP)
$INCLUDE repperc-xpp.inc

* average % annual growth rates bt tminrep and tmaxrep (name of variable + XPPREP)
$INCLUDE repperc-xpprep.inc

* % annual growth from preceding period (name of variable + XPY)
$INCLUDE repperc-xpy.inc

* save reports to gdx file
$INCLUDE repgdx.inc
$INCLUDE repdisplay.inc


DISPLAY "#### END: rep.inc";
