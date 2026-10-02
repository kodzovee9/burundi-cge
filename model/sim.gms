* sim.gms

DISPLAY "#### START: sim.gms";

OPTION DOMLIM = 1000;

* default CNS solver is PATH
PARAMETER
  SolverCNS
  /2/
;  

* empty simcur
simcur('fixgdp-run') = NO;
simcur('ref0')     = NO;

DISPLAY rgdpfc0;


$ONMULTI

SET
  sim
    /base/
  simcur(sim)
    /base/
  simbase(sim)
    /base/
    
  simpovrep(sim)    simulations for which poverty reports are generated
  simineqrep(sim)   simulations for which inequality reports are generated
  tpovrep(t)        years for which poverty reports are generated
  tineqrep(t)       years for which inequality reports are generated
;

PARAMETER
* sim parameters

  tab2sim(sim,a,t)                  rate of tax on producer gross output value
  tabsim(sim,a,t)                   rate of tax on producer gross output value
  teb2sim(sim,c,t)                  export tax rate for commodity c
  tebsim(sim,c,t)                   export tax rate for commodity c
  tfab2sim(sim,f,a,t)               rate of tax on factor f use by activity a
  tfabsim(sim,f,a,t)                rate of tax on factor f use by activity a
  tmb2sim(sim,c,t)                  import tariff rate for commodity c
  tmbsim(sim,c,t)                   import tariff rate for commodity c
  tqbsim(sim,c,t)                   rate of sales tax
  tqb2sim(sim,c,t)                  rate of sales tax
  tvacb2sim(sim,c,ac,t)             rate of value added tax commodity c demander ac
  tvacbsim(sim,c,ac,t)              rate of value added tax commodity c demander ac
  tyb2sim(sim,ins,t)                rate of income tax for domestic non-gov inst
  tybsim(sim,ins,t)                 rate of income tax for domestic non-gov inst
  ty01sim(sim,ins,t)                   0-1 parameter for flexing rate of direct tax on dom inst ins

  tfbsim(sim,f,t)                   rate of direct tax on factors (soc sec tax)
  tfb2sim(sim,f,t)                  rate of direct tax on factors (soc sec tax)
  tf01sim(sim,f,t)                   0-1 parameter for flexing rate of direct tax on factors


  PWESIM(sim,c,t)                   export price for c (foreign currency) in simulation sim (deviation wrt base)
  PWMSIM(sim,c,t)                   import price for c to r (foreign currency) in simulation sim (deviation wrt base)

  qgbsim(sim,c,t)                   qnty of government demand for commodity c in simulation sim (deviation wrt base)
  dqgsim(sim,c,t)                  qnty of government demand for commodity c in simulation sim (level deviation wrt base in same units as SAM)

  dkinsbsim(sim,ins2,fcap,t)        real gross fixed capital investment in capital stock fcap in simulation sim (deviation wrt base)
  ddkinssim(sim,ins2,fcap,t)       real gross fixed capital investment in capital stock fcap in simulation sim (level deviation wrt base in same units as SAM)
  ddkinsgdp0sim(sim,ins2,fcap,t)    real gross fixed capital investment in capital stock fcap in simulation sim (share of base GDP)

  dmpsbsim(sim,ins,t)                exogenous marginal propensity to save for dom non-gov inst insdng
  mpsbsim(sim,ins,t)                exogenous marginal propensity to save for dom non-gov inst insdng

  qmbarsim(sim,c,t)               index for import quota volume for commodity c 
  rexrsim(sim,t)                      index for (official) exchange rate (dom. currency per unit of for. currency)
  prexrsim(sim,t)                    index for exchange rate premium

  trnsfrbsim(sim,ac,ins,t)          transfers from insp to ins or factor in simulation sim (deviation wrt base)
  dtrnsfrsim(sim,ac,ins,t)            transfers from insp to ins or factor in simulation sim (level deviation wrt base in same units as SAM)
  
  tfpexogsim(sim,a,t)               exogenous component of sectoral TFP in simulation sim (deviation wrt base)
  fprdbsim(sim,f,t)                 constant in definition of productivity of factor f
  fprdabsim(sim,f,a,t)              constant in definition of productivity of factor f

  CPISIM(sim,t)                     consumer price index in simulation sim (deviation wrt base)
  invvalfbsim(sim,t)                FDI value (in FCU) (deviation wrt base)
  invvalfb2sim(sim,t)               FDI value (in FCU) (level deviation wrt base in same units as SAM)
  invvalfbgdp0sim(sim,t)            FDI value (in FCU) (share of base GDP)
  dkafdisim(sim,f,a,t)              sector-specific fdi (deviation wrt base)
  dkafdi2sim(sim,f,a,t)             sector-specific fdi (level deviation wrt base in same units as SAM)
  dkaexog01sim(sim,a)               0-1 parameter for selecting activities with exog non-gov investment by destination
 

 
  qtrstbsim(sim,c,instrst,t)        exogenous quantity consumed of commodity c by tourist instrst

  TRSMRECSIM(sim,t)                 total tourism receipts (FCU) (deviation wrt base)
  ta01sim(sim,a)                    0-1 parameter for flexing rate of tax on producer gross output value

  popsim(sim,ac,t)                  population (deviation wrt base)
  pop2sim(sim,ac,t)                 population (level deviation wrt base in same units as pop)
  shrbtvat2sim(sim,c,a,t)           share of VAT rebate
  shrbtvatsim(sim,c,a,t)            share of VAT rebate
  qgc01sim(sim,c,t)                 0-1 parameter for flexing government consumption
  subcbsim(sim,c,t)                 subsidy rate for demander ac on comm c (deviation wrt base)
  subcb2sim(sim,c,ac,t)             subsidy rate for demander ac on comm c (deviation wrt base)

  icasim(sim,c,a,t)                 intermediate input c per unit of aggregate intermediate in activity a

  mpcapgovsim(sim,fcap)             marginal product of capital fcap
  mtfpsim(sim,a,fcap)               mapping - TFP in activity a affected by capital stock fcap (with rel value indicating strength of effect)
  mpksim(sim,ac,fcap,t)             marginal product in act a per unit of additional capital stock fcap

  tfp01sim(sim,a)                   0-1 par determining (relative) rate of act a TFP growth
  WFDISTSIM(sim,f,a,t)              wage distortion factor for factor f in activity a
  qebsim(sim,c2,t)                   export demand for c if pwe = pwse (world price for substitutes)
  qfssim(sim,f,t)                   supply of factor f

  

*HL-Start
  qfinsscalsim(sim,f,t)             scaling factor for real factor endowments

  qfacgrwsim(sim,f,t)               growth rate factor stocks
  qfacindexsim(sim,f,t)             index for factor supply

  gdpgrwsim(sim,t)                  growth rate real GDP at factor cost
  gdpindexsim(sim,t)                index for GDP
  rgdpfcsim(sim,t)                  real GDP at factor cost (at constant base-year prices)

  NFFGSIM(sim,t)                    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU)
  dnffinssim(sim,t)   change in net foreign financing dom non-gov inst (FCU)
  nffinsbarsim(sim,t)           base-year net foreign financing dom non-gov inst (FCU)

*The following two parameters are used in repdebtbor.inc
  gintratsim(t,sim)             real interest rate on domestic government debt to ins (in insdng)
  fintratsim(ins2,t,sim)             real interest rate on foreign debt for ins (in insd)

*HL-End


  qeqdratsim(sim,c,t)               ratio bt exports and domestic sales in simulation sim
  delta_dssim(sim,c,t)              CET function share parameter for domestic commodity c in simulation sim (defined in par-defn-sim)
  delta_esim(sim,c,t)               CET function share parameter for exports commodity c in simulation sim (defined in par-defn-sim)
  phi_xsim(sim,c,t)                 CET function shift parameter for commodity c in simulation sim (defined in par-defn-sim)

;

* initialize shock prm
$IF EXIST isi-sim.inc PWESIM(sim,c,t)            = 0;
$IF EXIST isi-sim.inc PWMSIM(sim,c,t)            = 0;
$IF EXIST isi-sim.inc qgbsim(sim,c,t)            = 0;
$IF EXIST isi-sim.inc dqgsim(sim,c,t)            = 0;
$IF EXIST isi-sim.inc dkinsbsim(sim,ins2,fcap,t) = 0;
$IF EXIST isi-sim.inc ddkinssim(sim,ins2,fcap,t)= 0;
$IF EXIST isi-sim.inc trnsfrbsim(sim,ac,ins,t)   = 0;
$IF EXIST isi-sim.inc tfpexogsim(sim,a,t)        = 0;
$IF EXIST isi-sim.inc CPISIM(sim,t)              = 0;
$IF EXIST isi-sim.inc invvalfbsim(sim,t)         = 0;
$IF EXIST isi-sim.inc invvalfb2sim(sim,t)        = 0;
$IF EXIST isi-sim.inc invvalfbgdp0sim(sim,t)     = 0;
$IF EXIST isi-sim.inc dkafdisim(sim,f,a,t)       = 0;
$IF EXIST isi-sim.inc dkafdi2sim(sim,f,a,t)      = 0;
$IF EXIST isi-sim.inc dkaexog01sim(sim,a)        = 0;

$IF EXIST isi-sim.inc qtrstbsim(sim,c,instrst,t) = 0;
$IF EXIST isi-sim.inc popsim(sim,acpop,t)        = 0;
$IF EXIST isi-sim.inc pop2sim(sim,acpop,t)       = 0;
$IF EXIST isi-sim.inc qfacgrwsim(sim,f,t)        = 0;
$IF EXIST isi-sim.inc gdpgrwsim(sim,t)           = 0;
$IF EXIST isi-sim.inc gintratsim(t,sim)      = 0;
$IF EXIST isi-sim.inc fintratsim(ins2,t,sim)      = 0;
$IF EXIST isi-sim.inc shrbtvat2sim(sim,c,a,t)    = 0;
$IF EXIST isi-sim.inc shrbtvatsim(sim,c,a,t)     = 0;
$IF EXIST isi-sim.inc fprdbsim(sim,f,t)          = 0;
$IF EXIST isi-sim.inc qgc01sim(sim,c,t)          = 0;
$IF EXIST isi-sim.inc subcbsim(sim,c,t)          = 0;
$IF EXIST isi-sim.inc subcb2sim(sim,c,ac,t)      = 0;
$IF EXIST isi-sim.inc TRSMRECSIM(sim,t)          = 0;
$IF EXIST isi-sim.inc icasim(sim,c,a,t)          = 0;
$IF EXIST isi-sim.inc fprdabsim(sim,f,a,t)       = 0;
$IF EXIST isi-sim.inc ta01sim(sim,a)             = 0;
$IF EXIST isi-sim.inc mpcapgovsim(sim,fcap)      = 0;
$IF EXIST isi-sim.inc mtfpsim(sim,a,fcap)        = 0;
$IF EXIST isi-sim.inc mpksim(sim,ac,fcap,t)      = 0;
$IF EXIST isi-sim.inc WFDISTSIM(sim,f,a,t)       = 0;
$IF EXIST isi-sim.inc qebsim(sim,c2,t)           = 0;
$IF EXIST isi-sim.inc tab2sim(sim,a,t)           = 0;       
$IF EXIST isi-sim.inc tabsim(sim,a,t)            = 0;      
$IF EXIST isi-sim.inc teb2sim(sim,c,t)           = 0;       
$IF EXIST isi-sim.inc tebsim(sim,c,t)            = 0;      
$IF EXIST isi-sim.inc tfab2sim(sim,f,a,t)        = 0;      
$IF EXIST isi-sim.inc tfabsim(sim,f,a,t)         = 0;       
$IF EXIST isi-sim.inc tmb2sim(sim,c,t)           = 0;       
$IF EXIST isi-sim.inc tmbsim(sim,c,t)            = 0;       
$IF EXIST isi-sim.inc tqbsim(sim,c,t)            = 0;       
$IF EXIST isi-sim.inc tqb2sim(sim,c,t)           = 0;
$IF EXIST isi-sim.inc tvacb2sim(sim,c,ac,t)      = 0;         
$IF EXIST isi-sim.inc tvacbsim(sim,c,ac,t)       = 0;         
$IF EXIST isi-sim.inc tyb2sim(sim,ins,t)         = 0;          
$IF EXIST isi-sim.inc tybsim(sim,ins,t)          = 0;          
$IF EXIST isi-sim.inc ty01sim(sim,ins,t)          = 0
$IF EXIST isi-sim.inc tf01sim(sim,f,t)          = 0
$IF EXIST isi-sim.inc tfbsim(sim,f,t)            = 0;          
$IF EXIST isi-sim.inc tfb2sim(sim,f,t)           = 0;           
$IF EXIST isi-sim.inc qfssim(sim,f,t)            = 0;
$IF EXIST isi-sim.inc NFFGSIM(sim,t)                 = 0;
$IF EXIST isi-sim.inc dmpsbsim(sim,ins,t)            = 0;
$IF EXIST isi-sim.inc mpsbsim(sim,ins,t)            = 0;
$IF EXIST isi-sim.inc qmbarsim(sim,c,t) = 0;           
$IF EXIST isi-sim.inc rexrsim(sim,t)     = 0;             
$IF EXIST isi-sim.inc prexrsim(sim,t)     = 0;             



* start: not in Excel

mpcapgovsim(sim,fcap)          = 0;
mtfpsim(sim,a,fcap)            = 0;
mpksim(sim,ac,fcap,t)          = 0;
tfp01sim(sim,a)                = 0;
ddkinssim(sim,ins2,fcap,t) = 0;
ddkinsgdp0sim(sim,ins2,fcap,t) = 0;
dmpsbsim(sim,ins,t)            = 0;
mpsbsim(sim,ins,t)            = 0;
qeqdratsim(sim,c,t)            = 0;
invvalfbgdp0sim(sim,t)         = 0;

dkafdisim(sim,f,a,t)           = 0;
dkafdi2sim(sim,f,a,t)          = 0;
dkaexog01sim(sim,a)           = 0;
dtrnsfrsim(sim,ac,ins,t)  = 0;
NFFGSIM(sim,t)                 = 0;
ty01sim(sim,ins,t) = 0;
tf01sim(sim,f,t) = 0;
qmbarsim(sim,c,t) = 0;        
rexrsim(sim,t)     = 0;        
prexrsim(sim,t)     = 0;        
dqgsim(sim,c,t) = 0;
dnffinssim(sim,t) = 0;
nffinsbarsim(sim,t) = 0;

* end: not in Excel


* start: code to run with isim-mams ---------------------------------

* must force inclusion of imv2-default.inc so that ISIM considers it a model file 
$INCLUDE imv2-default.inc

* isi-sim.inc exists (signals that model is run using IMv2, include imv2-dedault.inc; cannot use $IF NOT %NonIMv2%==1
$IF EXIST isi-sim.inc $INCLUDE imv2-default.inc


$IF EXIST isi-sim.inc $INCLUDE isi-sim.inc

* end: code to run with isim-mams -----------------------------------



* READ DATA FROM EXCEL ==============================================

$IF %NonIMv2%==1 $CALL GDXXRW user-files\%app%\%app%-sim.xlsx index=layout!A1
$IF %NonIMv2%==1 $GDXIN %app%-sim.gdx


$IF %NonIMv2%==1 $LOADDC sim
$IF %NonIMv2%==1 $LOADDC simcur

$IF %NonIMv2%==1 $LOADDC PWESIM
$IF %NonIMv2%==1 $LOADDC PWMSIM
$IF %NonIMv2%==1 $LOADDC TRSMRECSIM
$IF %NonIMv2%==1 $LOADDC WFDISTSIM
$IF %NonIMv2%==1 $LOADDC cpisim
$IF %NonIMv2%==1 $LOADDC dkinsbsim
$IF %NonIMv2%==1 $LOADDC facclossim
$IF %NonIMv2%==1 $LOADDC fprdabsim
$IF %NonIMv2%==1 $LOADDC fprdbsim
$IF %NonIMv2%==1 $LOADDC govclossim
$IF %NonIMv2%==1 $LOADDC govrecgdpsim
$IF %NonIMv2%==1 $LOADDC govrecgrwsim
$IF %NonIMv2%==1 $LOADDC govrecrulesim
$IF %NonIMv2%==1 $LOADDC govspndgdpsim
$IF %NonIMv2%==1 $LOADDC govspndgrwsim
$IF %NonIMv2%==1 $LOADDC govspndrulesim
$IF %NonIMv2%==1 $LOADDC icasim
$IF %NonIMv2%==1 $LOADDC invvalfb2sim
$IF %NonIMv2%==1 $LOADDC invvalfbsim
$IF %NonIMv2%==1 $LOADDC ngovpaygdpsim
$IF %NonIMv2%==1 $LOADDC ngovpaygrwsim
$IF %NonIMv2%==1 $LOADDC ngovpayrulesim
$IF %NonIMv2%==1 $LOADDC numerairesim
$IF %NonIMv2%==1 $LOADDC pop2sim
$IF %NonIMv2%==1 $LOADDC popsim
$IF %NonIMv2%==1 $LOADDC qebsim 
$IF %NonIMv2%==1 $LOADDC qgbsim
$IF %NonIMv2%==1 $LOADDC qgc01sim
$IF %NonIMv2%==1 $LOADDC qtrstbsim
$IF %NonIMv2%==1 $LOADDC rowclossim
$IF %NonIMv2%==1 $LOADDC shrbtvat2sim
$IF %NonIMv2%==1 $LOADDC shrbtvatsim
$IF %NonIMv2%==1 $LOADDC siclossim
$IF %NonIMv2%==1 $LOADDC subcb2sim
$IF %NonIMv2%==1 $LOADDC subcbsim
$IF %NonIMv2%==1 $LOADDC ta01sim
$IF %NonIMv2%==1 $LOADDC tab2sim
$IF %NonIMv2%==1 $LOADDC tabsim
$IF %NonIMv2%==1 $LOADDC teb2sim
$IF %NonIMv2%==1 $LOADDC tebsim
$IF %NonIMv2%==1 $LOADDC tmb2sim
$IF %NonIMv2%==1 $LOADDC tmbsim
$IF %NonIMv2%==1 $LOADDC tqb2sim
$IF %NonIMv2%==1 $LOADDC tqbsim
$IF %NonIMv2%==1 $LOADDC tfab2sim
$IF %NonIMv2%==1 $LOADDC tfabsim
$IF %NonIMv2%==1 $LOADDC tvacb2sim
$IF %NonIMv2%==1 $LOADDC tvacbsim
$IF %NonIMv2%==1 $LOADDC tyb2sim
$IF %NonIMv2%==1 $LOADDC tybsim
$IF %NonIMv2%==1 $LOADDC tfbsim
$IF %NonIMv2%==1 $LOADDC tfb2sim
$IF %NonIMv2%==1 $LOADDC taxrate2sim
$IF %NonIMv2%==1 $LOADDC taxrate3sim
$IF %NonIMv2%==1 $LOADDC taxratesim
$IF %NonIMv2%==1 $LOADDC tfpexogsim

$IF %NonIMv2%==1 $LOADDC trnsfrbsim
$IF %NonIMv2%==1 $LOADDC qfssim


* start: not in Excel

*$IF %NonIMv2%==1 $LOADDC mpcapgovsim
*$IF %NonIMv2%==1 $LOADDC mtfpsim
*$IF %NonIMv2%==1 $LOADDC mpksim
*$IF %NonIMv2%==1 $LOADDC dkinsbgdp0sim(sim,ins2,fcap,t)
*$IF %NonIMv2%==1 $LOADDC dtrnsfrsim


* end: not in Excel



*HL-Start
$IF %NonIMv2%==1 $LOADDC gdpgrwsim
$IF %NonIMv2%==1 $LOADDC gintratsim
$IF %NonIMv2%==1 $LOADDC fintratsim

*HL-End
$OFFMULTI

* setup reporte
$INCLUDE repsetup.inc


*!!
$IF EXIST user-files\%app%\%app%-sim2.inc $INCLUDE user-files\%app%\%app%-sim2.inc

*$EXIT

* expand all to all activities or commodities
$INCLUDE par-defn-sim-all.inc


* start: select CNS solver

IF (SolverCNS = 1,
  OPTION CNS = PATH;
);
IF (SolverCNS = 2,
  OPTION CNS = CONOPT;
);

* end: select CNS solver


* start: diagnose govclossim before imposing defaults

* note: currently, cannot move this code section to diagnostics-clos.inc

PARAMETER
  errgovclossim(sim,t)    govclossim for some post-base yr but not for t
;

errgovclossim(sim,t)$(tsol(t) AND SUM(tp$tnmin(tp), govclossim(sim,tp)) AND (NOT govclossim(sim,t))) = 1/0;
DISPLAY errgovclossim;

* If govclossim exists for sim-tmin but not for sim-t, use the value for sim-tmin
govclossim(sim,t)$(SUM(tmin, govclossim(sim,tmin)) AND (NOT govclossim(sim,t))) = SUM(tmin, govclossim(sim,tmin));

* end: diagnose govclossim before imposing defaults










* default closure for base es la usada en ref0
facclossim('base',f)$(NOT facclossim('base',f)) = facclos0(f);
numerairesim('base')$(NOT numerairesim('base')) = numeraire0;
govclossim('base',t)$(NOT SUM(tp, govclossim('base',tp)))     = govclos0;
siclossim('base')$(NOT siclossim('base'))       = siclos0;
rowclossim('base',t)$(NOT SUM(tp, rowclossim('base',tp)))     = rowclos0(t);

govrecrulesim('base',acgovrec)$(NOT govrecrulesim('base',acgovrec)) = govrecrule0(acgovrec);
govspndrulesim('base',acgovspnd)$(NOT govspndrulesim('base',acgovspnd)) = govspndrule0(acgovspnd);
ngovpayrulesim('base',acngovpay)$(NOT ngovpayrulesim('base',acngovpay)) = ngovpayrule0(acngovpay);

* default closure for sim NE base es la usada en base
facclossim(sim,f)$(NOT facclossim(sim,f)) = facclossim('base',f);
numerairesim(sim)$(NOT numerairesim(sim)) = numerairesim('base');
govclossim(sim,t)$(NOT SUM(tp, govclossim(sim,tp)))     = govclossim('base',t);
siclossim(sim)$(NOT siclossim(sim))       = siclossim('base');
rowclossim(sim,t)$(NOT SUM(tp, rowclossim(sim,tp)))     = rowclossim('base',t);
DISPLAY facclossim, numerairesim, govclossim, siclossim, rowclossim;


govrecrulesim(sim,acgovrec)$(NOT govrecrulesim(sim,acgovrec)) = govrecrulesim('base',acgovrec);
govspndrulesim(sim,acgovspnd)$(NOT govspndrulesim(sim,acgovspnd)) = govspndrulesim('base',acgovspnd);
ngovpayrulesim(sim,acngovpay)$(NOT ngovpayrulesim(sim,acngovpay)) = ngovpayrulesim('base',acngovpay);
DISPLAY govrecrulesim, govspndrulesim, ngovpayrulesim;

* if siclossim = 1, need to overwrite selection for ngovpayrulesim
ngovpayrulesim(sim,fcapngz)$(siclossim(sim)=1) = 1;

*transform govrecgrwsim, govspndgrwsim, and ngovpaygrwsim into growth indexes
 govspndindexsim(sim,acgovspnd,tmin) = 1;
 govrecindexsim(sim,acgovrec,tmin) = 1;
 ngovpayindexsim(sim,acngovpay,tmin) = 1;

*HL-Start
*Adding indices for factor supplies and real GDP at factor cost
 gdpindexsim(sim,tmin)$SUM(t, gdpgrwsim(sim,t))       = 1;
*HL-End


LOOP(t$(NOT tmin(t) AND tsol(t)),
  govspndindexsim(sim,acgovspnd,t) = govspndindexsim(sim,acgovspnd,t-1)*(1+govspndgrwsim(sim,acgovspnd,t));
  govrecindexsim(sim,acgovrec,t)   = govrecindexsim(sim,acgovrec,t-1)*(1+govrecgrwsim(sim,acgovrec,t));
  ngovpayindexsim(sim,acngovpay,t) = ngovpayindexsim(sim,acngovpay,t-1)*(1+ngovpaygrwsim(sim,acngovpay,t));

*HL-Start
  gdpindexsim(sim,t)    = gdpindexsim(sim,t-1)*(1 + gdpgrwsim(sim,t));
*HL-End

);


*HL-Start

DISPLAY gdpindexsim;
rgdpfcsim(sim,t)$(tsol(t) AND SUM(tp, gdpgrwsim(sim,tp))) = RGDPFC00*gdpindexsim(sim,t);
rgdpfcsim(simbase,t)$tsol(t) = RGDPFC0(t);


DISPLAY rgdpfcsim, rgdpfc0, gdpgrwsim, ngovpayindexsim, ngovpaygrwsim;
*HL-End



* diagnose model closure
$INCLUDE diagnostics-clos.inc

* diagnose simulations
$INCLUDE diagnostics-sim.inc

DISPLAY '1', simcur, tsol;
* SOLUTION STATEMENT ================================================

* iteracion sobre simcur
LOOP(sim$simcur(sim),

* define initial levels for all model variables using 0-parameters
$INCLUDE varinit.inc

* select closure rule for factor markets
$INCLUDE facclos.inc

  LOOP(t$tsol(t),
* select macro closure rule
$INCLUDE macclos.inc
  );

* install sim values for exogenous variables
$INCLUDE par-defn-sim.inc




* iteracion sobre tsol
  LOOP(tsol,

* assign time period tcur (the period for our single-period model)
    tcur(t) = NO;
    tcur(tsol)  = YES;

* endogenous variables in t = endogenous variables in t-1
* dmod = 1 + NOT base
    IF (dmod=1 AND NOT sim('base'),
$INCLUDE varinit-t2.inc
    );

*!! select one of the following two lines
    IF (NOT simbase(sim), SOLVE GEM USING MCP;);
*    IF (NOT simbase(sim), SOLVE GEM USING CNS;);
*    SOLVE GEM USING MCP;  
*    SOLVE GEM USING CNS;  
    
* reportar resulatdos simulacion
$INCLUDE reploop.inc

* diagnosticar solucion
$INCLUDE diagnostics-sol.inc


  );

*!!
$IF EXIST user-files\%app%\%app%-sim3.inc $INCLUDE user-files\%app%\%app%-sim3.inc

);


$INCLUDE repsimdisplay.inc


DISPLAY "#### END: sim.gms";
