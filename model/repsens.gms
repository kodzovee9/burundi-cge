* repsens.gms

* load sets

$ONMULTI

SET 
  sim
  simcur(sim)
  simbase
  ac
  a
  c
  h
  ac /ElasCent, Mean, DesvEst, LowerBound, UpperBound, min, max/
  igdp
  kgdp
  t
  r
  solcol
  tsol(t)
  acrep

;

$GDXIN 'sim.gdx' 
$LOADDC sim
$LOADDC simcur
$LOADDC simbase
$LOADDC ac
$LOADDC a
$LOADDC c
$LOADDC h
$LOADDC t
$LOADDC tsol
$LOADDC igdp
$LOADDC kgdp
$LOADDC acrep
$LOADDC solcol




$OFFMULTI

* load merged sensitivity analysis results

SET 
  iter
  iterwarn(iter)
  iternwarn(iter)
  iter(iter)

;  

PARAMETER
  modsolstat(iter,solcol,t,sim)                   model and solver status
  modsolwarn(iter,t,sim)
  
  
  employxptt2(iter,acrep,sim,t)
  employxptt2_ci(ac,acrep,sim,t)

  minmaxprm(ac,ac)
  
  gdpindicXP(iter,igdp,kgdp,t,sim)  
  gdpindicXP_ci(ac,igdp,kgdp,t,sim)   intervalo confianza normal
  gdpindicXP_ci2(ac,igdp,kgdp,t,sim)  intervalo confianza empirico
  macrogrwAVG(iter,*,sim)
  macrogrwAVG_ci(ac,*,sim)
  
  
  sigma_Q(iter,c)
  sigma_Qrep(ac,c)
  sigma_X(iter,c)
  
  itertot                              numero de iteraciones
  
  k                                    parametro para desigualdad Chebyshev
  

;
 
 
$GDXIN 'tmp\merged.gdx'

*!! elements in iter differ accross files
$LOADDC iter=merged_set_1
*$LOADDC modsolstat
*$LOADDC modsolwarn
$LOADDC gdpindicXP






* identificar iteraciones con error
iterwarn(iter) = NO;
*iterwarn(iter)$SUM((ms1,sim,ms3), modsolwarn(iter,ms1,sim,ms3)) = YES;

* identificar iteraciones sin error
iternwarn(iter) = YES;
iternwarn(iter)$iterwarn(iter) = NO;

itertot = SUM(iter$(iternwarn(iter)), 1);

*sigma_Qrep('min',c) = SMIN(iter, sigma_Q(iter,c));
*sigma_Qrep('max',c) = SMAX(iter, sigma_Q(iter,c));

*### intervalo de confianza suponiendo distribucion normal

$ONTEXT
intervalo de confianza suponiendo distribucion normal
local lim_inf = `mu' - invnorm((1-`confianza')/2) * (`sigma'/sqrt(`n'))
local lim_sup = `mu' + invnorm((1-`confianza')/2) * (`sigma'/sqrt(`n'))
$OFFTEXT


gdpindicXP_ci('Mean',igdp,kgdp,t,sim)$tsol(t)   = SUM(iter$iternwarn(iter), gdpindicXP(iter,igdp,kgdp,t,sim))/itertot;
gdpindicXP_ci('DesvEst',igdp,kgdp,t,sim)$tsol(t) = SUM(iter$iternwarn(iter), POWER(gdpindicXP(iter,igdp,kgdp,t,sim) - gdpindicXP_ci('Mean',igdp,kgdp,t,sim),2)); 
gdpindicXP_ci('DesvEst',igdp,kgdp,t,sim)$tsol(t) = SQRT(gdpindicXP_ci('DesvEst',igdp,kgdp,t,sim)/(itertot-1));
gdpindicXP_ci('LowerBound',igdp,kgdp,t,sim)$tsol(t)  = gdpindicXP_ci('Mean',igdp,kgdp,t,sim) - 1.96 * gdpindicXP_ci('DesvEst',igdp,kgdp,t,sim);
gdpindicXP_ci('UpperBound',igdp,kgdp,t,sim)$tsol(t)  = gdpindicXP_ci('Mean',igdp,kgdp,t,sim) + 1.96 * gdpindicXP_ci('DesvEst',igdp,kgdp,t,sim);

SET
igdpsel(igdp)
/PrvCon, GDPFC/
;
gdpindicXP_ci(ac,igdp,kgdp,t,sim)$(NOT igdpsel(igdp)) = 0;


PARAMETER 
  repstataprvcon(iter,sim)
  repstatagdpfc(iter,sim)
;
repstataprvcon(iter,sim) = gdpindicXP(iter,'prvcon','real','2030',sim);
repstatagdpfc(iter,sim) = gdpindicXP(iter,'gdpfc','real','2030',sim);


* write SAM to excel
EXECUTE_UNLOAD 'dum.gdx' 
  repstataprvcon
  repstatagdpfc
;

EXECUTE "GDXXRW dum.gdx o=tmp\repstataprvcon.xlsx par=repstataprvcon rng=data!A1"
EXECUTE "GDXXRW dum.gdx o=tmp\repstatagdpfc.xlsx par=repstatagdpfc rng=data!A1"





