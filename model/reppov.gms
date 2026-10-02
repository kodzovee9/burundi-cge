* reppov.gms

*$EXIT
DISPLAY "#### START: reppov.gms";

* this is an alternative to reppov.gms in MAMS
* first version: March 17, 2010
* this version: May 30, 2025

* if approach=1, need data on p0 and p0elas

PARAMETER
  errpovmodule(h)    if =UNDF approach eq 1 but no data in p0 and p0elas for at least on household
;

errpovmodule(h)$(povmodule('approach') = 1 AND (NOT povdata(h,'p0') OR NOT povdata(h,'p0elas'))) = 1/0;
DISPLAY errpovmodule;

* if no data is provided, set approach eq 1 to avoid an error in ISIM
IF (NOT SUM((h,acpov), povdata(h,acpov)) AND NOT SUM((obs,isurvey), hhdsurvey(obs,isurvey)),
  povmodule('approach') = 1;
);


* error if tpovdata not included in tsol
PARAMETER 
  errtpovdata1             tmp prm to compute errtpovdata2
  errtpovdata2             if =UNDF if tpovdata GT tmax
;
errtpovdata1 = SUM(t$tsol(t), ORD(t)) - SUM(t$tpovdata(t), ORD(t));
errtpovdata2$(errtpovdata1<0) = 1/0;
DISPLAY errtpovdata1, errtpovdata2;
DISPLAY tpovdata;
* if tpovdata is empty, assume tpovdata=tmin
tpovdata(t)$(tmin(t) AND NOT SUM(tp$tpovdata(tp), 1)) = YES;
DISPLAY "!" ,tpovdata, tineqrep;




* start: read sets from simulation Excel file

$GDXIN %app%-sim.gdx

$ONMULTI
$LOADDC simpovrep
$LOADDC simineqrep
$LOADDC tpovrep
$LOADDC tineqrep
$OFFMULTI

* end: read sets from simulation Excel file


*!! default values for simpovrep and simineqrep

* include base in simpovrep and simpovineq in case they are not empty
simpovrep(sim)$(SUM(simp$simpovrep(simp), 1) AND simbase(sim)) = YES;
simineqrep(sim)$(SUM(simp$simineqrep(simp), 1) AND simbase(sim)) = YES;

* exclude from simpovrep and simineqrep simulations not in simcur
simpovrep(sim)$(NOT simcur(sim)) = NO;
simineqrep(sim)$(NOT simcur(sim)) = NO;

*!! default values for tpovrep and tineqrep
tpovrep(tmin)$(NOT SUM(tp$tpovrep(tp), 1) AND SUM(simp$simpovrep(simp), 1)) = YES;
tineqrep(tmin)$(NOT SUM(tp$tineqrep(tp), 1) AND SUM(simp$simineqrep(simp), 1)) = YES;
*!! to do: use tminmax(t) as defult


SET 
  simpovineqrep(sim)    simulations for which poverty or inequality reports are generated
  tpovineqrep(t)        years for which poverty or inequality reports are generated
;

simpovineqrep(sim)$(simpovrep(sim) OR simineqrep(sim)) = YES;
tpovineqrep(t)$(tpovrep(t) OR tineqrep(t)) = YES;


*approaches: 1 = elasticity, 2 = log-normal, 3 = survey
*povmodule('approach') = 2;


* start: code to run with IMv2 --------------------------------------

$IF EXIST isi-reppov2.inc $INCLUDE isi-reppov2.inc

* end: code to run with IMv2 ----------------------------------------







$ONTEXT
*HL20250528
tineqrep(t1)$(tpovdata(t1) AND CARD(h) NE 1) = YES;
DISPLAY "1", tineqrep;
tineqrep(t1)$(tmax(t1) AND CARD(h) NE 1) = YES;
DISPLAY "2", tineqrep;
tineqrep(t1)$(tminrep(t1) AND CARD(h) NE 1) = YES;
tineqrep(t1)$(tmaxrep(t1) AND CARD(h) NE 1) = YES;
DISPLAY "3", tineqrep;
$OFFTEXT

ALIAS (h,hp), (obs,obsp);


SET
  povcol(ac)
;

* used to process fgt indicators
povcol(h) = YES;
povcol('total') = YES;

PARAMETER
  

  YHREALX(h,t,sim)          household real income in simulation sim
  EHREALX(h,t,sim)          household real expenditure in simulation sim
  
  fgt(alpha,ac,t,sim)       fgt poverty indicator
  fgt_ext(alpha,ac,t,sim)   fgt extreme poverty indicator

  poptot(t,sim)             total population
  poptot00                   base-year total population 
  
  yhavg(h,t,sim)               mean income
  p0(ac)                     base-year headcount ratio
  yhavg00(h)                    base-year mean income
  
  gini(ac,t,sim)             gini coefficient
  gini00(ac)                 base-year gini coefficient
  sigma_est(h)               standard deviation
  povline_est(h)             poverty line
  p0elas(h)                  growth-elasticity of poverty

  welfarexp(h,t,sim)        change in selected welfare index wrt base-year

* dummy parameters used to compute fgt under lognormal assumption
  dum1(h,t,sim)
  dum2(h,t,sim)
  dum3(h,t,sim)

  lambda(h)
  p0elas_est(h)              estimated growth-poverty elasticity -- is used under approach 2

  welfare_i(obs,t,sim)      individual welfare from hhdsurvey
  povline_i(obs,t,sim)             individual poverty line from hhdsurvey
  povline2_i(obs,t,sim)            individual extreme poverty line from hhdsurvey
  popwt00(obs)               base-year individual population weight from hhdsurvey
  popwt(obs,t,sim)          individual population weight from hhdsurvey
  popwttot(h,t,sim)         total popwt by RH

* to be used in gini computations when approach=3
  dum_i(obs)
  mutot_i
  obstot_i
  tmptmp(obs)
  ii(obs)

  welfareIndex(obs)
  welfareSorted(obs)
  welfare_aux(obs)
  popwtSorted(obs)
;


*for observations without welfare value, use EPS
*alternatively, ignore them
 hhdsurvey(obs,'welfare')$(NOT hhdsurvey(obs,'welfare')) = EPS;

*compute real disposable income and real expenditure
 YHREALX(h,t,sim)$(tsol(t) AND simcur(sim)) = YIX(h,t,sim)*(1-TYX(h,t,sim))/CPIX(t,sim);
*EHREALX(h,t,sim)$(tsol(t) AND simcur(sim)) = EHX(h,t,sim)/CPIX(t,sim);
 EHREALX(h,t,sim)$(tsol(t) AND simcur(sim)) = SUM(c, PQD00(c,h)*QHX(c,h,t,sim));


*### read benchmark data 

*total population
 poptot(t,sim)       = SUM(h, popX(h,t,sim));
 poptot00 = SUM(tpovdata, poptot(tpovdata,'base'));

*income distribution data; poverty and inequality
*note: povdata is used undear approaches 1 and 2
 p0(h)        = povdata(h,'p0');
 gini00(h)    = povdata(h,'gini');
 p0elas(h)    = povdata(h,'p0elas');
 p0('total') = SUM((h,tpovdata), popX(h,tpovdata,'base')/poptot00 * p0(h));
 yhavg00(h)$(povmodule('welfareindex')= 1) = SUM(tpovdata, YHREALX(h,tpovdata,'base')/POPX(h,tpovdata,'base'));
*yhavg00(h)$(povmodule('welfareindex')= 2) = SUM(tpovdata, EHREALX(h,tpovdata,'base')/POPX(h,tpovdata,'base'));
 yhavg00(h)$(povmodule('welfareindex')= 2) = SUM(tpovdata, qhpcX(h,tpovdata,'base'));

DISPLAY poptot, p0, yhavg00, gini00, p0elas;


*### read data for counterfactual scenarios
 yhavg(h,t,sim)$(tsol(t) AND simcur(sim) AND povmodule('welfareindex')= 1) = YHREALX(h,t,sim)/POPX(h,t,sim);
*yhavg(h,t,sim)$(tsol(t) AND simcur(sim) AND povmodule('welfareindex')= 2) = EHREALX(h,t,sim)/POPX(h,t,sim);
 yhavg(h,t,sim)$(tsol(t) AND simcur(sim) AND povmodule('welfareindex')= 2) = qhpcX(h,t,sim);

*compute change in welfare wrt year in tpovdata
 welfarexp(h,t,sim)$(tsol(t) AND simcur(sim) AND povmodule('welfareindex')= 1) 
   =
  (YHREALX(h,t,sim)/POPX(h,t,sim)) / SUM(tpovdata, YHREALX(h,tpovdata,sim)/popX(h,tpovdata,sim)) - 1;
  
 welfarexp(h,t,sim)$(tsol(t) AND simcur(sim) AND povmodule('welfareindex')= 2)
* = (EHREALX(h,t,sim)/POPX(h,t,sim)) / SUM(tpovdata, EHREALX(h,tpovdata,sim)/popX(h,tpovdata,sim)) - 1;
  = qhpcX(h,t,sim) / SUM(tpovdata, qhpcX(h,tpovdata,sim)) - 1;


* start: definition of small models ---------------------------------

*M1 (model 1) = compute standard deviations from gini coefficients
*see equation (1) in Lopez and Serven (2005)
*we need the inverse cumulative standard normal -- build a small model

*M2 (model 2) = compute poverty lines from poverty rates

VARIABLES
 X1(h)
 X2(h)
 ;
EQUATIONS
 E1(h)
 E2(h)
;

 E1(h)..
  ERRORF(X1(h)) =E= (1+gini00(h))/2;

 E2(h)..
  ERRORF(X2(h)) =E= p0(h);

MODEL
  M1 /E1/
  M2 /E2/  
;

* end: definition of small models -----------------------------------


*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* start: approach = 1 -> compute poverty based on p0elas
* need data for p0 and p0elas

IF (povmodule('approach') = 1,

  fgt('0',h,t,sim)$(simpovrep(sim) AND tpovrep(t)) = p0(h) * (1 + p0elas(h)*welfarexp(h,t,sim));
  fgt('0','total',t,sim)$(simpovrep(sim) AND tpovrep(t)) = SUM(h, POPX(h,t,sim)/poptot(t,sim) * fgt('0',h,t,sim));

);  
 
$ONTEXT
derivation of the above formula for elasticity-based headcount poverty rate
source: reppov.gms

notation:
P = headcount poverty rate; Q = household consumption per capita
e = elasticiy; "0" value in preceding year

e  = (dP/P0)/ (dQ/Q0);
dP = e*P0*dQ/Q0
P  = P0 + dP = P0 + e*P0*dQ/Q0 = P0(1 + e*(Q-Q0)/Q0) = P0(1 + e*(Q/Q0 - 1)
$OFFTEXT

* end: approach = 1 -> compute poverty based on p0elas


*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* start: approach = 2 -> compute poverty and ineq based on log-normal assumption
* need data for p0 and gini or standard deviation

IF (povmodule('approach') = 2,

*compute standard deviations from gini coefficients
 SOLVE M1 USING CNS;
 sigma_est(h) = SQRT(2) * X1.L(h);

*compute poverty lines from poverty rate
 SOLVE M2 USING CNS;
 povline_est(h) = EXP( (X2.L(h) - sigma_est(h)/2) * sigma_est(h) ) * yhavg00(h);

DISPLAY sigma_est, povline_est;

*### p0 
  dum1(h,t,sim)$(tpovrep(t) AND simpovrep(sim)) = LOG(povline_est(h)/yhavg(h,t,sim))/sigma_est(h) + sigma_est(h)/2;

  fgt('0',h,t,sim)$(tpovrep(t) AND simpovrep(sim)) = ERRORF(dum1(h,t,sim));

*### p1 
  dum2(h,t,sim)$(tpovrep(t) AND simpovrep(sim)) = LOG(povline_est(h)/yhavg(h,t,sim))/sigma_est(h) - sigma_est(h)/2;

  fgt('1',h,t,sim)$(tpovrep(t) AND simpovrep(sim)) = ERRORF(dum1(h,t,sim))
    - (yhavg(h,t,sim)/povline_est(h)) * ERRORF(dum2(h,t,sim));

*### p2
  dum3(h,t,sim)$(tpovrep(t) AND simpovrep(sim)) = LOG(povline_est(h)/yhavg(h,t,sim))/sigma_est(h) - 3*sigma_est(h)/2;

  fgt('2',h,t,sim)$(tpovrep(t) AND simpovrep(sim)) = ERRORF(dum1(h,t,sim))
    - 2 * (yhavg(h,t,sim)/povline_est(h)) * ERRORF(dum2(h,t,sim))
    + (yhavg(h,t,sim)/povline_est(h))**2 * EXP(sigma_est(h)**2) * ERRORF(dum3(h,t,sim));

  fgt(alpha,'total',t,sim)$(tpovrep(t) AND simpovrep(sim)) = SUM(h, popX(h,t,sim)/poptot(t,sim) * fgt(alpha,h,t,sim));

* in addition, compute the growth elasticity of poverty for each hhd

$ONTEXT
The standard normal density function is not very difficult:
  exp(-sqr(x)/2)/sqrt(2*pi)
$OFFTEXT
  lambda(h) =  SUM(tpovdata, EXP(-SQR(dum1(h,tpovdata,'base'))/2)/SQRT(2*PI) / ERRORF(dum1(h,tpovdata,'base')));
  p0elas_est(h) = -1/sigma_est(h) * lambda(h);
  DISPLAY lambda, p0elas_est;

);


* to do: compute national gini coefficient using an exact decomposition method
* of course, easier to compute national theil index

* end: approach = 2 -> compute poverty and ineq based on log-normal assumption

*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* start: approach = 3 -> compute poverty and ineq based on micro data from hhd survey
* need data for p0, microdata from hhd survey (household per capita income and weights),
* and mapping bt individuals and RH in SAM

* initialize rank
$LIBINCLUDE rank 

IF (povmodule('approach') = 3,

* initialize welfare_i
  welfare_i(obs,tpovdata,'base') = hhdsurvey(obs,'welfare');
  
  povline_i(obs,t,sim)$(tpovineqrep(t) AND simpovineqrep(sim))  = hhdsurvey(obs,'povline');
  povline2_i(obs,t,sim)$(tpovineqrep(t) AND simpovineqrep(sim)) = hhdsurvey(obs,'povline_ext');
  popwt00(obs) = hhdsurvey(obs,'popwt');
  
* adjust popwt using POPX -- static ageing (?)   
  popwt(obs,t,sim)$simpovineqrep(sim) =  popwt00(obs) * SUM(h$mobsh(obs,h), popX(h,t,sim)) / SUM(h$mobsh(obs,h), SUM(tpovdata, popX(h,tpovdata,sim)));
  
  popwttot(h,t,sim)$simpovineqrep(sim)   = SUM(obs$mobsh(obs,h), popwt(obs,t,sim));  
  
  welfare_i(obs,t,sim)$(tpovineqrep(t) AND simpovineqrep(sim)) = SUM(tpovdata, welfare_i(obs,tpovdata,'base')) * ( 1 + SUM(h$mobsh(obs,h), welfarexp(h,t,sim)) );

*### fgt

* added SUM(obs$mobsh(obs,h), 1) to consider cases in which a given h is not present in hhdsurvey
  fgt(alpha,h,t,sim)$(tpovrep(t) AND simpovrep(sim) AND SUM(obs$mobsh(obs,h), 1)) = 
    SUM(obs$(welfare_i(obs,t,sim)<povline_i(obs,t,sim) AND mobsh(obs,h)), 
    popwt(obs,t,sim) * ( (povline_i(obs,t,sim) - welfare_i(obs,t,sim)) / povline_i(obs,t,sim) ) ** (ORD(alpha)-1)) / popwttot(h,t,sim);
  
  fgt(alpha,'total',t,sim)$(tpovrep(t) AND simpovrep(sim)) =
    SUM(h, popwttot(h,t,sim)/SUM(hp, popwttot(hp,t,sim)) * fgt(alpha,h,t,sim));

*### fgt_ext

* added SUM(obs$mobsh(obs,h), 1) to consider cases in which a given h is not present in hhdsurvey
  fgt_ext(alpha,h,t,sim)$(tpovrep(t) AND simpovrep(sim) AND SUM(obs$mobsh(obs,h), 1)) = 
    SUM(obs$(welfare_i(obs,t,sim)<povline2_i(obs,t,sim) AND mobsh(obs,h)), 
    popwt(obs,t,sim) * ( (povline2_i(obs,t,sim) - welfare_i(obs,t,sim)) / povline2_i(obs,t,sim) ) ** (ORD(alpha)-1)) / popwttot(h,t,sim);
  
  fgt_ext(alpha,'total',t,sim)$(tpovrep(t) AND simpovrep(sim)) =
    SUM(h, popwttot(h,t,sim)/SUM(hp, popwttot(hp,t,sim)) * fgt_ext(alpha,h,t,sim));

*### gini

* to compute the gini coefficient, need to sort welfare parameter

  LOOP(sim$simineqrep(sim),
    
    LOOP(t$tineqrep(t),

      welfare_aux(obs) = welfare_i(obs,t,sim);

* start: sort welfare and popwt -------------------------------------


* start: old version

$ONTEXT
* write symbol welfare_aux to gdx file
      EXECUTE_UNLOAD "rank_in.gdx", welfare_aux;

* sort symbol; permutation index will be named welfare_aux also
      EXECUTE 'gdxrank rank_in.gdx rank_out.gdx';

* load the permutation index
      EXECUTE_LOAD "rank_out.gdx", welfareIndex=welfare_aux;

* create a sorted version of welfare_i
      welfareSorted(obs + (welfareIndex(obs) - ORD(obs))) = welfare_aux(obs);
* create a sorted version of popwt    
      popwtSorted(obs + (welfareIndex(obs) - ORD(obs))) = popwt(obs,t,sim);
$OFFTEXT

* end: old version


$LIBINCLUDE rank welfare_aux obs welfareIndex
DISPLAY welfare_aux;

* create a sorted version of welfare_i
      welfareSorted(obs + (welfareIndex(obs) - ORD(obs))) = welfare_aux(obs);

* create a sorted version of popwt    
      popwtSorted(obs + (welfareIndex(obs) - ORD(obs))) = popwt(obs,t,sim);

* end: sort welfare and popwt ---------------------------------------

      obstot_i = SUM(obs, popwt(obs,t,sim));
      mutot_i = SUM(obs, popwt(obs,t,sim)*welfare_aux(obs)) / obstot_i;
DISPLAY obs, t, sim, popwt, obstot_i, mutot_i;

      tmptmp(obs)$(ORD(obs)=1) = popwtSorted(obs);
      LOOP(obs,
        tmptmp(obs) = tmptmp(obs-1) + popwtSorted(obs);
      );

      ii(obs) = (2*tmptmp(obs)-popwtSorted(obs)+1)/2;

$ONTEXT
stata version:
    gen `tmptmp' = sum(`wt')
    gen `i' = (2*`tmptmp'-`wt'+1)/2
    gen `each' = `varlist'*(`obs'-`i'+1)
$OFFTEXT
  
      dum_i(obs) = welfareSorted(obs) * ( obstot_i - ii(obs) + 1 );

      gini('nation',t,sim) = 1 + 1/obstot_i
        - 2/(mutot_i * obstot_i**2) * SUM(obs, dum_i(obs)*popwtSorted(obs));

* include file to compute gini coefficient by household
* include sentence was moved to repspec.gms
*$INCLUDE calc-ginihh.inc


    );
  );

);

* end: approach = 3 -> compute poverty and ineq based on micro data from hhd survey


* fgt*100 + eps
fgt(alpha,povcol,t,sim)$(tpovrep(t) AND simpovrep(sim)) = 1e-20 + fgt(alpha,povcol,t,sim)*100;
fgt_ext(alpha,povcol,t,sim)$(tpovrep(t) AND simpovrep(sim)) = 1e-20 + fgt_ext(alpha,povcol,t,sim)*100;
DISPLAY fgt, fgt_ext, gini;

$ONTEXT
* note: see repmeso.gms
PARAMETER
  fgt0(ac,sim)        headcount ratio (P0) first and last year of simulation
  fgt1(ac,sim)        poverty gap (P1) first and last year of simulation
  fgt2(ac,sim)        poverty severity (P2) first and last year of simulation

  fgt0yy(sim,ac,t)    headcount ratio (P0) from first to last year of simulation
  fgt1yy(sim,ac,t)    poverty gap (P1) from first to last year of simulation
  fgt2yy(sim,ac,t)    poverty severity (P2) from first to last year of simulation

  
  ginirep(ac,sim)        gini coefficient first and last year of simulation
  ginirepyy(sim,ac,t)    gini coefficient from first to last year of simulation
  
;
$OFFTEXT

* poverty
fgt0(ac,tpovdata) = fgt('0',ac,tpovdata,'base');
fgt1(ac,tpovdata) = fgt('1',ac,tpovdata,'base');
fgt2(ac,tpovdata) = fgt('2',ac,tpovdata,'base');

fgt0(ac,simcur) = SUM(tmaxrep, fgt('0',ac,tmaxrep,simcur));
fgt1(ac,simcur) = SUM(tmaxrep, fgt('1',ac,tmaxrep,simcur));
fgt2(ac,simcur) = SUM(tmaxrep, fgt('2',ac,tmaxrep,simcur));

fgt0yy(simcur,ac,t)$tsol(t) = fgt('0',ac,t,simcur);
fgt1yy(simcur,ac,t)$tsol(t) = fgt('1',ac,t,simcur);
fgt2yy(simcur,ac,t)$tsol(t) = fgt('2',ac,t,simcur);

$ONTEXT
fgt0xp(simcur,ac,t)$fgt0yy('base',ac,t) = 100*(fgt0yy(simcur,ac,t)/fgt0yy('base',ac,t) - 1);
fgt1xp(simcur,ac,t)$fgt1yy('base',ac,t) = 100*(fgt1yy(simcur,ac,t)/fgt1yy('base',ac,t) - 1);
fgt2xp(simcur,ac,t)$fgt2yy('base',ac,t) = 100*(fgt2yy(simcur,ac,t)/fgt2yy('base',ac,t) - 1);
$OFFTEXT

 
* extreme poverty 
fgt0_ext(ac,tpovdata) = fgt_ext('0',ac,tpovdata,'base');
fgt1_ext(ac,tpovdata) = fgt_ext('1',ac,tpovdata,'base');
fgt2_ext(ac,tpovdata) = fgt_ext('2',ac,tpovdata,'base');

fgt0_ext(ac,simcur) = SUM(tmaxrep, fgt_ext('0',ac,tmaxrep,simcur));
fgt1_ext(ac,simcur) = SUM(tmaxrep, fgt_ext('1',ac,tmaxrep,simcur));
fgt2_ext(ac,simcur) = SUM(tmaxrep, fgt_ext('2',ac,tmaxrep,simcur));

fgt0yy_ext(simcur,ac,t)$tsol(t) = fgt_ext('0',ac,t,simcur);
fgt1yy_ext(simcur,ac,t)$tsol(t) = fgt_ext('1',ac,t,simcur);
fgt2yy_ext(simcur,ac,t)$tsol(t) = fgt_ext('2',ac,t,simcur);

* inequality
ginirep(ac,tpovdata) = gini(ac,tpovdata,'base');
ginirep(ac,simcur) = SUM(tmax, gini(ac,tmax,simcur));

ginirepyy(simcur,ac,t)$tsol(t) = gini(ac,t,simcur);




* save GDX file
$IF %NonIMv2%==1 EXECUTE_UNLOAD 'reppov2-%app%.gdx',
$IF NOT %NonIMv2%==1 EXECUTE_UNLOAD 'reppov2.gdx',
  povmodule
  povdata
  obs
  hhdsurvey
  mobsh


  fgt
  fgt_ext
  gini
  
  fgt0yy
  fgt1yy
  fgt2yy

  fgt0
  fgt1
  fgt2

  fgt0yy_ext
  fgt1yy_ext
  fgt2yy_ext

  fgt0_ext
  fgt1_ext
  fgt2_ext
  
  ginirep
  ginirepyy
  
  welfare_i
  popwt
  povline_i
  povline2_i


  poptotX

;  

DISPLAY
  povmodule
  povdata
  obs
  hhdsurvey
  mobsh


  fgt
  fgt_ext
  gini
  
  fgt0yy
  fgt1yy
  fgt2yy

  fgt0
  fgt1
  fgt2

  fgt0yy_ext
  fgt1yy_ext
  fgt2yy_ext

  fgt0_ext
  fgt1_ext
  fgt2_ext
  
  ginirep
  ginirepyy
  

;  


*EXCLUDED
*Include file to compute gini coefficient by household
*$INCLUDE calc-ginihh.inc

$EXIT
DISPLAY "#### END: reppov.gms";