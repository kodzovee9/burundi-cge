* repdist

SET idat
/
welf
popwt
pl
plext
/

tsel(t)
/
2019
2026
*2030
*2035
2040
/
;

tsel(t) = NO;
tsel(t)$tsol(t) = YES;


;
PARAMETER
  hhdsurvey2(obs,t,sim,idat)
;


$ONTEXT
simcur(sim) = NO;
simcur('base') = YES;
simcur('remesas2530_10') = YES;
simcur('remesas2530_15') = YES;
$OFFTEXT


hhdsurvey2(obs,t,sim,'welf') = welfare_i(obs,t,sim);
hhdsurvey2(obs,t,sim,'popwt') = popwt(obs,t,sim);
hhdsurvey2(obs,t,sim,'pl') = povline_i(obs,t,sim);
hhdsurvey2(obs,t,sim,'plext') = povline2_i(obs,t,sim);




$IF %NonIMv2%==1 FILE repdist /repdist-%app%.csv/;
$IF NOT %NonIMv2%==1 FILE repdist /repdist2.csv/;


PUT repdist, 

*PUT "obs,yr,sim,welf_,popwt_,pl_,plext_"/;
PUT "obs,yr,sim,welf,popwt,pl,plext"/;

$ONTEXT
.TL: Displays the names of the individual elements of a set.
.TE: Displays the explanatory text associated with a set element.
$OFFTEXT

LOOP((obs,t,sim)$(tsel(t) AND simcur(sim)),
* note the use of TE instead of TL
  PUT obs.TL, ",", t.TL, ",", sim.TL, ",", 
*  PUT obs.TL, ",", t.TL, ",", sim.TE(sim), ",", 
    hhdsurvey2(obs,t,sim,'welf'), ",",
    hhdsurvey2(obs,t,sim,'popwt'), ",",
    hhdsurvey2(obs,t,sim,'pl'), ",",
    hhdsurvey2(obs,t,sim,'plext')/;
);

PUTCLOSE;
