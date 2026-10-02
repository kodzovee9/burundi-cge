* data.gms

DISPLAY "#### START: data.gms";

*SSA = Systematic Sensitivity Analysis (automatic on/off switch)
*see sens.gms
$IF NOT SET SSA $SET SSA 0


* START: Select dataset =============================================

*$SET app2 jor2019
$SET app2 bdi2019



*### code to run with/without IMv2
$IF NOT SET NonIMv2 $LOG 'NonIMv2 is Missing! -- Running ISIM-MAMS v2'

$SETGLOBAL NonIMv2 %NonIMv2%


$IF NOT SET app $LOG 'Using app2 since app option is not used. app2 = %app2%'
$IF NOT SET app $SET app %app2%

$SETGLOBAL app %app%


* END: Select dataset ===============================================


$ONTEXT

GEM-Core

By Martin Cicowiez (CEDLAS-UNLP) and Hans Lofgren


SEARCHABLE TEXT SEGMENTS TO FACILITATE NAVIGATION:
(Reference to include files are included; each segment has START and END.)
--------------------------------------------------------------------------

*START: data.gms
 Select dataset
 Sets in Excel database - declaration
 Parameters in Excel database - declaration
 Read-in of data from Excel and .inc data file
   $INCLUDE display-database.inc
 Initial processing of database
   $INCLUDE sambal.inc

*END: data.gms
*START: mod.gms

 Parameters and variables - definition
        $INCLUDE diagnostics-data.inc
        $INCLUDE irrmodel.inc [HL-20250526. Not used -- delete?
 Variables - declaration


*HL-2025052 INITIAL??:
 Parameters for initial variable values - declaration ??
 Parameters in model and their support parameters - declaration ??
 Parameters and variables - definition
        $INCLUDE diagnostics-data.inc
        $INCLUDE irrmodel.inc
 Variables - declaration
 Equations - declaration
        Production and factors
        Domestic and foreign trade
        Current payments involving domestic institutions
        Investment, system constraints, and numéraire
 Equations - definition
        Production and factors
        Domestic and foreign trade
        Current payments involving domestic institutions
        Investment, system constraints, and numéraire
 Model - declaration and definition
 Simulation sets - declarations and definitions
 Closure rule parameters for simulation - declarations
 Closure rule parameters for simulation - default definitions
   $INCLUDE diagnostics-clos.inc
 Variables with zero initial values - fixing at zero
 Solving the model
   $INCLUDE varinit.inc
   $INCLUDE facclos.inc
   $INCLUDE macclos.inc
   $INCLUDE varinit-t2.inc
   $INCLUDE diagnostics-sol.inc

--------------------------------------------------------------------------


$OFFTEXT




* next line is needed for IMv2
$ONEMPTY

* START: Sets in Excel database - declaration =======================

SET
* simulation sets
  sim                     all simulations

* time sets
  t                       all periods
  tsol(t)                 solution periods
  tsoldum(t)              solution periods (dummy) -- to run model with or without IMv2

  tcur(t)                 current period -- dynamic set for recursive model
  tmin(t)                 first period
  tmax(t)                 last period
  tnmin(t)                not first period

  trep(t)                 report periods
  tminrep(t)              first report period
  tmaxrep(t)              last report period
  tpovdata(t)             year for poverty data
  tavg(t)                 period to compute average


  ac                      global set (SAM accounts and other items)
  /

* elements of ins2
  govz              government
  ngovz             domestic non-government
  rowz              rest of world

* tradelas
  sigma_q           Armington substitution elas (bt imports and dom output in dom dem)
  sigma_x           CET elas (bt exports and dom supplies for dom marketed output)
  eta_e             elas of export dem (for commodities in the set ced)

* tfpelas -- elements in ac for which the par tfpelas may be non-zero:
  trdgdp

* dinam -- elements in ac for which the par dinam may be non-zero:
  kappa             capital mobility parameter
  netprfrat         net profit rate used to estimate initial non-gov capital stocks

* ssgrw -- elements in ac for which the par ssgrw may be non-zero:
*Note: ssgrw is used if dmod=2, to build reference scenario for dymamic model with balanced
*      growth assumption.
  gdp
  qlab
  prd

* scaling -- elements in acscal for which the par scaling may be non-zero:
  samsol      scaling SAM in data file to units to solve the model
  samrep      scaling SAM in data file to units for reports
  samto1      scaling SAM in data file to unscaled LCU (1 = one unit of LCU)
  samsolrep   scaling samsol times scaling samrep
  qlabsol     scaling labor and population in data file to units to solve the model
  qlabrep     scaling labor and population in data file to units for reports
  qlabto1     scaling labor and population in data file to unscaled number of persons (1 = one person)
  qlabsolrep  scaling qlabsol times scaling qlabrep
  emisol      scaling emissions in data file to units to solve the model
  emirep      scaling emissions in data file to units for reports
  emito1      scaling emissions in data file to unscaled number of persons (1 = one ton or similar)
  emisolrep   scaling emisol times scaling emirep

* emissions -- elements in the set ghg:
  CO2            Carbon dioxide
  CH4            Methane
  N2O            Nitrous oxide
  CH4+N2O

* government spending items -- elements in the set acgovspnd(ac)
  trngovgov               transfers from gov to non-gov
  trrowgov                transfers from gov to row
  congov                  government consumption

* government receipt items -- elements in the set acgovrec(ac):
  trgovngov               transfers from non-gov to gov
  trgovrow                transfers from row to gov
  netforfingov            gov net foreign financing
  netdomfin               gov net domestic financing

* non-government payment items -- elements in the set acngovpay(ac):
  trngovrow               transfers from row to non-gov
  trrowngov               transfers from non-gov to row
  trfacrow                transfers from row to factors
  trrowfac                transfers from factors to row
  savngov                 domestic non-government savings
  netforfinngov           non-gov net foreign financing
  fdi                     foreign direct investment
  tourismrec              total international tourism receipts in FCU

* unemployment --  -- elements in ac for which the par unemp may be non-zero:
  UERAT00                 base-year unemployment rate
  eta_wf                  elas of real wage with respect to unemployment rate

* used in reports -- elements over which various report parameters may be defined
  tot-lab                 total labor
  tot-capng               total non-government (private) capital
  tot-tax                 total tax
  trgov
  trrow
  trinsdng
  imports_alt
  exports_alt

* population subgroups -- element in the set acpop(ac):
  agelab         mult-yr age cohort in labor force (often 15-64 yrs)

* poverty module -- ??
  nation
  urban
  rural

*elements in the set acpov(ac) for which the par povmodule(ac) is non-zero:
  approach
  welfareindex
  
*elements in the set acpov(ac) for which the 2nd index of the par povdata(ac,acpov) may be nonzer0
  p0             headcount poverty rate
  p0elas         elasticity of headcount poverty rate with respect to hhd per-capita consumption
  gini

*elements in the set isurvey(ac):
  popwt          population weight
  welfare        welfare (real consumption or real income per capita)
  povline        poverty line
  povline_ext    extreme poverty line

* additional account for SAM used for VAT modeling
  rebate-vat
  /

  acscal(ac) indices for scaling selected data 
  /
  samsol      scaling SAM in data file to units to solve the model
  samrep      scaling SAM in data file to units for reports
  samto1      scaling SAM in data file to unscaled LCU (1 = one unit of LCU)
  samsolrep   scaling samsol times scaling samrep
  qlabsol     scaling labor and population in data file to units to solve the model
  qlabrep     scaling labor and population in data file to units for reports
  qlabto1     scaling labor and population in data file to unscaled number of persons (1 = one person)
  qlabsolrep  scaling qlabsol times scaling qlabrep
  emisol      scaling emissions in data file to units to solve the model
  emirep      scaling emissions in data file to units for reports
  emito1      scaling emissions in data file to unscaled number of persons (1 = one ton or similar)
  emisolrep   scaling emisol times scaling emirep
  /

  acpop(ac) elements for which population parameters may be non-zero
*Note: Below, the elements in h are added to acpop.
  /
  agelab
  /

  acpov(ac) elements for which poverty-related parameters may be non-zero
  /
  approach
  welfareindex
  p0
  p0elas
  gini
  /
  
  alpha    fgt (Foster–Greer–Thorbecke) indices
  /0,1,2/

  acgovspnd(ac)  government spending items
  /
  trngovgov
  trrowgov
  congov
  /

  acgovrec(ac)  government receipt items
  /
  trgovngov
  trgovrow
  netforfingov
  netdomfin
  /

  acngovpay(ac)  non-government payment items
  /
  trngovrow
  trrowngov
  trfacrow
  trrowfac
  savngov
  netforfinngov
  fdi
  tourismrec
  /

  actrnsfr(ac) transfer items
  /
  trngovrow
  trfacrow
  trngovgov
  trrowgov
  trgovrow
  trrowfac
  /

  ins2(ac) indices used for institutional parameters
  /
  govz
  ngovz
  rowz
  /

  ghg(ac)  greenhouse gas emmission categories
  /
  CO2            Carbon dioxide
  CH4            Methane
  N2O            Nitrous oxide
  CH4+N2O
  /

  congov(acgovspnd)       government consumption (total)
  trngovgov(acgovspnd)    transfers from governemt to non-government
  trrowgov(acgovspnd)     transfers from government to row
  fcapgz(acgovspnd)       factors capital government
  fcapngz(acngovpay)       factors capital non-government

  trgovngov(acgovrec)     transfers from non-government to governemt
  trgovrow(acgovrec)      transfers from row to government
  netforfingov(acgovrec)    gov net foreign financing
  netdomfin(acgovrec)     gov net domestic financing

  trngovrow(acngovpay)    transfers from row to non-gov
  trrowngov(acngovpay)    transfers from non-gov to row
  trfacrow(acngovpay)     transfers from row to factors
  trrowfac(acngovpay)     transfers from factors to row
  savngov(acngovpay)      domestic non-government savings
  netforfinngov(acngovpay)  non-gov net foreign financing
  fdi(acngovpay)          foreign direct investment
  tourismrec(acngovpay)      total tourism receipts

  a2(ac)                  activities including all
  a(a2)                   activities
  c2(ac)                  commodities including all
  c(c2)                   commodities
  ct(c)                   trade and transport commodities

*!! see default below
  cfood(c)                food commodities
  cplext(c)               commodities in the extreme poverty line
  
  d(ac)                   domestic demanders (or demand types)
  ced(c)                  commodities with constant elasticity export demand functions
  cesexog(c)              commodities with exogenous export supply
  cmbar(c)                commodities with import quota

  tacd(ac)                domestic trade and transport margin account
  tacm(ac)                import trade and transport margin account
  tace(ac)                export trade and transport margin account
  f(ac)                   factors
  fva(f)                  factors that earn VA (in SAM)
  flab(f)                 factors labor
  fcap(f)                 factors capital
  fcapg(fcap)             factors capital government
  fcapng(fcap)            factors capital non-government
  fncap(f)                factors non-capital
  fuendog(f)              factors with endogenous unemployment

* MC-2018-10-08
*!! to do: consider using fleo(f,a)
*!! factors in fleo should be activity-specific
  fleo(f)                 factors with fixed coefficient relation to output

  f1(f)                   factors at 1st (top) level of nest
  f2(f)                   factors at 2nd level of nest
  f3(f)                   factors at 3rd level of nest
  fsam(f)                 factors in SAM
  fnsam(f)                factors not in SAM

  ins(ac)                 institutions
  insd(ins)               domestic institutions
  insdng(insd)            domestic non-government institutions
  h(insdng)               households
  hrur(h)                 rural households
  hurb(h)                 urban households
  insngo(insdng)          non-gov organization institutions
  insnh(ins)              non-household institutions
  insdnh(insd)            domestic non-household institutions
  insent(insdng)          enterprises
  insdngnh(insd)          domestic non-government and non-households institutions

  insgov(ins)             government
  insrow(ins)             rest of the world
  instrst(ins)            foreign tourists
  insng(ins)              non-gov institutions

  capins(ac)              institutions capital accounts
  capinsd(capins)         domestic institutions capital accounts
  capinsng(capins)        non-government institutions capital accounts
  capinsdng(capinsd)      domestic non-government institutions capital accounts
  capgov(capinsd)         government capital account
  caprow(capins)          rest of the world capital account
  capfin(ac)              financial institution capital account

  actax(acgovrec)         all taxes
  actaxc(acgovrec)        all commodity taxes
  taxvatc(acgovrec)       value-added tax (commodies)
  taxcom(acgovrec)        sales tax
  taxact(acgovrec)        tax on producer gross output value
  taxdir(acgovrec)        direct tax on dom inst ins
  taximp(acgovrec)        import tariff
  taxexp(acgovrec)        export tax
  taxfac(acgovrec)        direct tax on factors (soc sec tax)
  taxfacact(acgovrec)     tax on factor use
  subcom(acgovspnd)       commodity demand subsidies

  inv(ac)                 investment accounts
  invng(inv)              non-government investment
  invg(inv)               government investment
  invginf(invg)           government investment in infrastructure
  invgogov(invg)          government investment in other gov
  dstk(ac)                changes in inventories

  acnt(ac)                all elements in ac except total

* sets and mappings used in aggregated reports
  acrep                       user-defined aggregated accounts used in reports
  /
* used in reports
  tot-lab                 total labor
  tot-tax                 total tax
  trgov
  trrow
  trinsdng
  imports_alt
  exports_alt
  /

  acrepnt(acrep)              all elements in acrep except total

  macacrep(ac,acrep)          mapping bt accounts in ac and accounts in acrep\
  /
  tot-lab         .tot-lab
  tot-tax         .tot-tax
  trgov           .trgov
  trrow           .trrow
  trinsdng        .trinsdng
  imports_alt     .imports_alt
  exports_alt     .exports_alt
  /

  acsam(ac)              accounts in SAM


* mappings

  mapaggreg(ac,ac)        account ac in SAM is aggregated to acp
  mfcapinv(fcap,ac)       mapping bt cap fac fcap and related capital factor account fcap

  mtaxvatc(c,ac)          demander ac pays VAT (tax-vat) when buying c
  msubcom(c,ac)           commodity c is subsidized (subcom) for demander ac

* this set is needed for syntactical reasons
  macgovrec(ac,ac)        mapping of acp (c or fcap linked to government) to itself
* this set is needed for syntactical reasons
  macgovspnd(ac,ac)       mapping of acp (c or fcap linked to government) to itself
* this set is needed for syntactical reasons
  macngovpay(ac,ac)       mapping of acp (invng linked to non-government) to itself

  mcapins(capins,ins)     mapping of capins to ins
  mtaxfa(taxfacact,f)     mapping bt taxfacact (tax on factor use) and related factor account f

  mf2f1(f2,f1)            mapping between factors in f2 and factors in f1 (f2 is aggregated to f1)
  mf3f2(f3,f2)            mapping between factors in f3 and factors in f2 (f3 is aggregated to f2)

* for poverty module
  obs                 survey observations
  mobsh(obs,h)        mapping bt obs and representative households in SAM and model
  mobshtmp(obs,ac)    mapping bt obs and representative households in SAM and model
  isurvey(ac)         indices for hhd survey
    /
      popwt           population weight
      welfare         welfare (real consumption or real income per capita)
      povline         poverty line
      povline_ext     extreme poverty line
   /

;

ALIAS
  (sim,simp),
  (tp,t),
  (tsolp,tsol),
  (tminp,tmin),
  (acpp,acp,ac),
  (cpp,cp,c),
  (ap,a),
  (fp,f),
  (flabp,flab),
  (f3p,f3)
  (f2p,f2)
  (f1p,f1)
  (fcapp,fcap),
  (hp,h),
  (ins,insp,inspp,insppp),
  (inv,invp)
  (insd,insdp),
  (insdng,insdngp),
  (acntp, acnt),
  (taximp,taximpp),
  (taxexp,taxexpp),
  (capins,capinsp),
  (capinsdng,capinsdngp),
  (d,dp)
  (acrep,acrepp,acreppp)
  (obs,obsp)
  (ins2,ins2p);

* END: Sets in Excel database - declaration =========================
* START: Parameters in Excel database - declaration =================

PARAMETER
  SAM(ac,acp)                  social accounting matrix
  qfbase(ac,ac)                base-year employment by factor and activity
  tradelas(c,ac)               Armington-CET-export demand elasticities by com
  prodelas(a)                  elasticity of substitution bt factors
  prodelas2(a,f)               elasticity of substitution for activity a between factors aggregated to composite factor fnsam (2nd level if three-level VA function)
  prodelas3(a,f)               elasticity of substitution for activity a between factors aggregated to composite factor fnsam (3rd level if three-level VA function)
                               
  MPS000(ins)                  marginal propensity to save for dom non-gov inst insdng
  leselas(c,h)                 expenditure elasticity of dem by com (c) - hhd (h)
  frisch(h)                    Frisch parameter for household LES demand
                               
  facclos0(f)                  closure rule market for factof f in ref0
  numeraire0                   numeraire in ref0
  govclos0                     closure rule government in ref0
  siclos0                      closure rule savings-investment in ref0
  rowclos0(t)                     closure rule rest of the world in ref0
  scaling(acscal)              scaling of sam and other param -- factor quantities)
  dmod                         model selection -- static or dynamic
                               
  ssgrw(ac)                    ss growth rate
  pop0(ac,t)                   population
  qlabins0(ac,acp,t)           labor supply by labor household and labor category

  qfacgrw(f,t)                 growth rate factor stocks
  fprdgrw(f,t)                 growth in productivity of factor f in t
  gdpgrw(t)                    GDP growth rate
  deprcap(fcap)                capital depreciation rates
  dinam(ac)                    kappa and netprfrat
  debt00(ins2,ins2p)           base-year debts of institution (column) "insp" to institution (row) "ins"
  gintrat0(t)                  real interest rate on domestic government debt
  fintrat0(ins2,t)             real interest rate on foreign debt for domestic institution insd
*HL! Is it used??
  shrndfgms0(t)                shr of gov net dom financing via monetary system

  govrecrule0(acgovrec)        rule for government receipts acgovrec
  govrecgrw0(acgovrec,t)       growth rate government receipts acgovrec
  GOVRECGDP0(acgovrec,t)       GDP shr for government receipt acgovrec
  GOVRECABS0(acgovrec,t)       absorption shr for government receipt of ac

  govspndrule0(acgovspnd)      rule for government spending acgovspnd
  govspndgrw0(acgovspnd,t)     growth rate government spending acgovspnd
  GOVSPNDGDP0(acgovspnd,t)     GDP shr for government spending acgovspnd
  GOVSPNDABS0(acgovspnd,t)     absorption shr for government receipt of ac

  ngovpayrule0(acngovpay)      rule for non-government payment acngovpay
  ngovpaygrw0(acngovpay,t)     growth rate for non-government payment acngovpay
  NGOVPAYGDP0(acngovpay,t)     GDP shr for non-government payment acngovpay
  NGOVPAYABS0(acngovpay,t)     absorption shr for government receipt of ac

  taxrate0(acgovrec,ac,t)      rate for tax type ac imposed on acp in t (deviation wrt baseyr)
  taxrate20(acgovrec,ac,t)     rate for tax type ac imposed on acp in t (level)
  unemp(f,ac)                  unemployment data

  mpcapgov(fcap)               marginal product of government capital
  mtfp(a,fcap)                 mapping - TFP in activity a affected by capital stock f (with rel value indicating strength of effect)

  savadj01(ins)                0-1 parameter for selecting dom non-gov inst with endog marg prop to save
  iadj010(fcap)                0-1 parameter for flexing non-government investment
  qgc010(c,t)                  0-1 parameter for flexing government consumptoion

  tfpelas(a,ac)                elas of TFP for a wrt to determinant ac

  LABPARTRAT0(t)               ratio between labor force and population at labor force age

  qmbarindex0(c,t)             index for import quota volume for commodity c 
  rexrindex0(t)                 index for (official) exchange rate (dom. currency per unit of for. currency)
  shrom000(c)                  share of imports of commodity c at the official exchange rate
  shroe000(c)                  share of exports of commodity c at the official exchange rate
  prexrindex0(t)                    index for exchange rate premium

*HL-201809
   tfpexog0(a,t)               exogenous component of sectoral TFP

* poverty module
  povmodule(acpov)             definition of approach and welfareindex for poverty module
  povdata(ac,acpov)            poverty indicator acpov applies to RH or household aggregation ac in base year
  hhdsurvey(obs,isurvey)       household survey

*HL-Start
  pweindex(c,t)                export price index for commodity c (constant FCU) (base year = 1)
  pwmindex(c,t)                import price index for commodity c (constant FCU) (base year = 1)
*HL-End 
  fleo01(f,a)                  0-1 parameter for indicating if TFP growth affects factors in fleo(f)
  tfp010(a)                    0-1 par determining (relative) rate of act a TFP growth (for GDP calibration) (0 <= tfp01 <= 1)

  qemibase(ghg,ac,acp)         emissions (CO2 ton - CO2 equivalent ton)
  
  ced01(c)                     0-1 parameter to select commodities with constant elasticity export demand functions
  cesexog01(c)                 0-1 parameter to select commodities with exogenous export supply
  
;


PARAMETER
  capest   method to calculate initial private capital stock
  /1/
  
;

* END: Parameters in Excel database - declaration ===================

* START: Read-in of data from Excel and .inc data file===============
* Note: At end of this segment, also reading of data in optional .inc file


* start: code needed for IMv2

* t1 is needed to run the model with current version of IMv2

ALIAS(t1,t);

SET
* in MAMS, t(t1) = year(s) for which model is solved (dynamic set)
* in MAMS, t2(t1) = auxiliary time set identical to t as defined initially
  t2(t1)    auxiliary time set identical to tsol as defined initially -- see also tsoldum
;


* end: coded needed for IMv2

* start: code to run with IMv2 --------------------------------------

$ONMULTI
$IF EXIST isi-data1.inc $INCLUDE isi-data1.inc
$OFFMULTI

* end: code to run with IMv2 ----------------------------------------

$IF %NonIMv2%==1 $CALL GDXXRW user-files\%app%\%app%-data.xlsx index=layout!A1
$IF %NonIMv2%==1 $GDXIN %app%-data.gdx

$LOADDC dmod

* when using ISIM, t is loaded in app-data.inc
$IF %NonIMv2%==1 $LOADDC t

$LOADDC tsoldum=tsol
$LOADDC trep

* when using ISIM, default tminrep and tmaxrep are loaded in app-data.inc
$IF %NonIMv2%==1 $LOADDC tminrep
$IF %NonIMv2%==1 $LOADDC tmaxrep

$LOADDC tavg
$LOADDC tpovdata


$ONMULTI

SET
  ac
    /all/
  a2
    /all/
  c2
    /all/
    
;

$LOADDC ac
$LOADDC acgovrec=actax
$LOADDC acgovspnd=subcom
$LOADDC acgovspnd=fcapg
$LOADDC acngovpay=fcapng

$LOADDC a2=a
$LOADDC a

$LOADDC c2=c
$LOADDC c

$OFFMULTI

$LOADDC ced
$LOADDC cesexog
$LOADDC cmbar
$LOADDC tacd
$LOADDC tacm
$LOADDC tace
$LOADDC f
$LOADDC flab
$LOADDC fcap
* for technical reasons both fcapng and fcapg should be read from Excel
$LOADDC fcapng
$LOADDC fcapg
* for technical reasons we need fcapgz, fcapngz, and instrstz
$LOADDC fcapgz = fcapg
$LOADDC fcapngz = fcapng
$LOADDC fleo
$LOADDC f1
$LOADDC f2
$LOADDC f3
$LOADDC ins
$LOADDC insd
$LOADDC insdng
$LOADDC h
$LOADDC hrur
$LOADDC hurb
$LOADDC insngo
$LOADDC insgov
$LOADDC insrow
$LOADDC instrst
$LOADDC capins
$LOADDC capinsd
$LOADDC capgov
$LOADDC capfin
$LOADDC actax
$LOADDC taxvatc
$LOADDC taxcom
$LOADDC taxact
$LOADDC taxdir
$LOADDC taximp
$LOADDC taxexp
$LOADDC taxfac
$LOADDC taxfacact
$LOADDC subcom
$LOADDC inv
$LOADDC invng
$LOADDC invg
$LOADDC invginf
$LOADDC dstk

$ONMULTI
$LOADDC acrep
SET acrep/total3/;
$OFFMULTI


$LOADDC SAM
$LOADDC qfbase
$LOADDC mapaggreg
$LOADDC tradelas
$LOADDC prodelas
$LOADDC prodelas2
$LOADDC prodelas3
$LOADDC MPS000
$LOADDC leselas
$LOADDC frisch
$LOADDC tfpelas

$LOADDC facclos0
$LOADDC numeraire0
$LOADDC govclos0
$LOADDC siclos0
$LOADDC rowclos0
$LOADDC scaling

$LOADDC ssgrw
$LOADDC pop0
$LOADDC qlabins0
$LOADDC qfacgrw
$LOADDC gdpgrw
$LOADDC tfp010
$LOADDC deprcap
$LOADDC dinam
$LOADDC mfcapinv

$LOADDC debt00
$LOADDC gintrat0
$LOADDC fintrat0
$LOADDC shrndfgms0
$LOADDC govrecrule0
$LOADDC govrecgrw0
$LOADDC govrecgdp0
$LOADDC GOVRECABS0
$LOADDC govspndrule0
$LOADDC govspndgrw0
$LOADDC govspndgdp0
$LOADDC GOVSPNDABS0
$LOADDC ngovpayrule0
$LOADDC ngovpaygrw0
$LOADDC ngovpaygdp0
$LOADDC NGOVPAYABS0

$LOADDC taxrate0
$LOADDC taxrate20
$LOADDC unemp
$LOADDC fprdgrw

$LOADDC msubcom
$LOADDC mcapins
$LOADDC mtaxfa
$LOADDC mf2f1
$LOADDC mf3f2

$LOADDC mpcapgov
$LOADDC mtfp

$LOADDC iadj010
$LOADDC qgc010
$LOADDC LABPARTRAT0

* poverty module
$LOADDC povmodule
$LOADDC povdata
$LOADDC obs
$LOADDC hhdsurvey
$LOADDC mobshtmp=mobsh


$LOADDC pweindex
$LOADDC pwmindex
$LOADDC tfpexog0

$LOADDC qemibase
$LOADDC qmbarindex0
$LOADDC rexrindex0
$LOADDC prexrindex0
$LOADDC shrom000
$LOADDC shroe000

$ONMULTI
$LOADDC macacrep
$OFFMULTI

* END: Read-in of data from Excel and .inc data file=================

* next definitions are relevant when unsing IMv2
ced01(c) = 0;
cesexog01(c) = 0;



* START: Initial processing of database =============================

acnt(ac) = YES;
acnt('total') = NO;

acrepnt(acrep) = YES;
acrepnt('total3') = NO;


* default value for fleo01
fleo01(f,a) = 0;

* default value for savadj01
savadj01(ins) = 0;

* default for cfood
cfood(c) = NO;
  
* default for cplext
cplext(c) = NO;

* complete mapaggreg
mapaggreg(ac,ac)$(NOT SUM(acp, mapaggreg(ac,acp))) = YES;

* start: code to run with IMv2 --------------------------------------

$IF EXIST isi-data2.inc $INCLUDE isi-data2.inc

* end: code to run with IMv2 ----------------------------------------

* if exists, read additional data from include file app-data2.inc
$IF %NonIMv2%==1 $IF EXIST user-files\%app%\%app%-data2.inc $INCLUDE user-files\%app%\%app%-data2.inc

* Display of the data in the program at this point.
$INCLUDE display-appdata.inc
DISPLAY ac;




SAM(ac,ac) = 0;


*### to avoid GAMS error, need to use tsoldum if not using IMv2 and t2 if using IMv2
* without IMv2
$IF %NonIMv2%==1 tsol(tsoldum) = YES;
* with IMv2
$IF NOT %NonIMv2%==1 tsol(t2) = YES;


tmin(t) = NO;
tmin(t)$(ORD(t) = 1) = YES;
tnmin(t) = YES;
tnmin(t)$tmin(t) = NO;
tnmin(t)$(NOT tsol(t)) = NO;

tmax(t) = NO;
*tmax(t)$(ORD(t) = CARD(t)) = YES;


*### to avoid GAMS error, need to use tsoldum if not using IMv2 and t2 if using IMv2
* without IMv2
$IF %NonIMv2%==1 tmax(tsoldum)$(ORD(tsoldum) = CARD(tsoldum)) = YES;
* with IMv2
$IF NOT %NonIMv2%==1 tmax(t2)$(ORD(t2) = CARD(t2)) = YES;


tsol(t)$(NOT dmod) = NO;
tsol(t)$(NOT dmod AND tmin(t)) = YES;
DISPLAY tsol;

acpop(h) = YES;

fncap(f) = YES;
fncap(f)$fcap(f) = NO;

capinsdng(capinsd) = YES;
capinsdng(capinsd)$capgov(capinsd) = NO;

capinsng(capins) = YES;
capinsng(capinsd)$capgov(capinsd) = NO;

caprow(capins) = YES;
caprow(capins)$capinsd(capins) = NO;

d(ac)   = NO;
d(a)    = YES;
d(tacd) = YES;
d(tacm) = YES;
d(tace) = YES;
d(insd) = YES;
d(fcap) = YES;
d(dstk) = YES;
d(instrst) = YES;

insnh(ins) = YES;
insnh(insdng)$h(insdng) = NO;
DISPLAY insnh;

insdnh(insd) = YES;
insdnh(h) = NO;

insdngnh(insd) = YES;
insdngnh(insdng)$h(insdng) = NO;
insdngnh(insd)$insgov(insd) = NO;

insng(ins) = YES;
insng(ins)$insgov(ins) = NO;

insent(insdng) = YES;
insent(h) = NO;
insent(insngo) = NO;

* taxes on commodities
actaxc(acgovrec)$taxvatc(acgovrec) = YES;
actaxc(acgovrec)$taxcom(acgovrec)  = YES;
actaxc(acgovrec)$taximp(acgovrec)  = YES;
actaxc(acgovrec)$taxexp(acgovrec)  = YES;



* endogenous unemployment
fuendog(f)$(facclos0(f)=4) = YES;

trgovngov('trgovngov') = YES;
trgovrow('trgovrow')   = YES;
netforfingov('netforfingov') = YES;
netdomfin('netdomfin') = YES;

trngovgov('trngovgov') = YES;
trrowgov('trrowgov')   = YES;
congov('congov')       = YES;


trngovrow('trngovrow') = YES;
trrowngov('trrowngov') = YES;
trfacrow('trfacrow')   = YES;
trrowfac('trrowfac')   = YES;
savngov('savngov')     = YES;
netforfinngov('netforfinngov') = YES;
fdi('fdi')             = YES;
tourismrec('tourismrec')     = YES;


* start: sensitivity analysis --------------------------------------

*install random parameters (i.e., elasticities)
$IF %SSA%==1 $INCLUDE randprm-uniform.inc

*install piecemeal sensitivity analysis elasticities
*$IF %SSA%==1 $INCLUDE piecemeal-prm.inc

* end: sensitivity analysis -----------------------------------------



PARAMETER
  sambalchk(ac)       to check sam balance
  errsambalchk(ac)    error sam is not balanced
;

* start: aggregate SAM

PARAMETER
  samdummy(ac,acp) dummy SAM
;
samdummy(ac,acp) = SAM(ac,acp);
SAM(ac,acp)  = 0;

SAM(acnt,acntp) = SUM((ac,acp)$(mapaggreg(ac,acnt)$mapaggreg(acp,acntp)), samdummy(ac,acp));

* end: aggregate SAM





* start: aggregate qfbase

PARAMETER qfbasedummy(ac,ac);

qfbasedummy(ac,acp) = qfbase(ac,acp);
qfbase(ac,acp) = 0;
qfbase(f,a) = SUM(acp$mapaggreg(acp,f), SUM(ac$mapaggreg(ac,a), qfbasedummy(acp,ac)));

* end: aggregate qfbase

* start: aggregate pop0

PARAMETER pop0dummy(ac,t);

pop0dummy(ac,t) = pop0(ac,t);
pop0(ac,t) = 0;
pop0(ac,t) = SUM(acp$mapaggreg(acp,ac), pop0dummy(acp,t));

* end: aggregate pop





* start: aggregate qemibase

PARAMETER emibasedummy(ghg,ac,ac);

emibasedummy(ghg,ac,acp) = qemibase(ghg,ac,acp);
qemibase(ghg,ac,acp) = 0;
qemibase(ghg,acnt,acntp) = SUM((ac,acp)$(mapaggreg(ac,acnt)$mapaggreg(acp,acntp)), emibasedummy(ghg,ac,acp));

* end: aggregate qemibase



* aggregate debt00
$ONTEXT
PARAMETER debt00dummy(ac,acp);
debt00dummy(ac,acp) = debt00(ac,acp);
debt00(ac,acp) = 0;
debt00(acnt,acntp) = SUM((ac,acp)$(mapaggreg(ac,acnt)$mapaggreg(acp,acntp)), debt00dummy(ac,acp));
$OFFTEXT

* aggregate mobsh
SET mobshdummy(obs,ac);
mobshdummy(obs,ac)$mobshtmp(obs,ac) = YES;
mobsh(obs,h) = NO;
mobsh(obs,h)$SUM(ac$(mobshdummy(obs,ac) AND mapaggreg(ac,h)), 1) = YES;

SAM(ac,ac) = 0;



ct(c)$(SUM(tacd, SAM(c,tacd)) OR SUM(tace, SAM(c,tace)) OR SUM(tacm, SAM(c,tacm))) = YES;
DISPLAY ct;


* default values for scaling
*?? is it a good idea to impose defaults
scaling('samsol')$(NOT scaling('samsol'))   = 1;
scaling('samrep')$(NOT scaling('samrep'))   = 1;
scaling('samto1')$(NOT scaling('samto1'))   = 1;
scaling('samsolrep') = scaling('samsol')/scaling('samrep');

scaling('qlabsol')$(NOT scaling('qlabsol')) = 1;
scaling('qlabrep')$(NOT scaling('qlabrep')) = 1;
scaling('qlabto1')$(NOT scaling('qlabto1')) = 1;
scaling('qlabsolrep') = scaling('qlabsol')/scaling('qlabrep');

* default values for scaling
scaling('emisol')$(NOT scaling('emisol')) = 1;
scaling('emirep')$(NOT scaling('emirep')) = 1;
scaling('emito1')$(NOT scaling('emito1')) = 1;
scaling('emisolrep') = scaling('emisol')/scaling('emirep');



* re-scale SAM
SAM(ac,acp) = SAM(ac,acp) / scaling('samsol');

* re-scale debt stocks
debt00(ins2,ins2p) = debt00(ins2,ins2p)/scaling('samsol');

* re-scale qlab
qfbase(flab,a) = qfbase(flab,a) / scaling('qlabsol');
pop0(ac,t) = pop0(ac,t)/scaling('qlabsol');

* re-scale emissions
qemibase(ghg,acp,ac) = qemibase(ghg,acp,ac)/scaling('emisol');


* check sam consistency
SAM('total',ac) = SUM(acntp, SAM(acntp,ac));
SAM(ac,'total') = SUM(acntp, SAM(ac,acntp));
sambalchk(acnt) = SAM('total',acnt) - SAM(acnt,'total');
DISPLAY sambalchk;




* if account balances exceed a critical maximum value, sambal.inc will balance
* the SAM exactly
$INCLUDE sambal.inc

* check sam consistency after sambal.inc and scaling
SAM('total',acnt) = SUM(acntp, SAM(acntp,acnt));
SAM(acnt,'total') = SUM(acntp, SAM(acnt,acntp));
sambalchk(acnt) = SAM('total',acnt) - SAM(acnt,'total');
DISPLAY sambalchk;
errsambalchk(ac)$(ABS(sambalchk(ac)) > 1e-4) = 1/0;
DISPLAY errsambalchk;

* mapping vat final consumption
mtaxvatc(c,insd)$(SAM(c,insd) AND SUM(taxvatc, SAM(taxvatc,c))) = YES;

* set with SAM accounts (after aggregation)
acsam(acntp)$SUM(acnt, SAM(acnt,acntp)) = YES;



* sets needed for syntactical reasons
macgovrec(ac,ac)$acgovrec(ac) = YES;
macgovspnd(ac,ac)$acgovspnd(ac) = YES;
macngovpay(ac,ac)$acngovpay(ac) = YES;


fva(f)$(f1(f) OR f2(f) OR f3(f)) = YES;
fva(fcap)$fcapg(fcap) = NO;

fsam(f)$(SUM(a, SAM(f,a))) = YES;
fnsam(f) = YES;
fnsam(f)$fsam(f) = NO;


* END: Initial processing of database ===============================


* activate the following sentence to debug
*$EXIT




DISPLAY "#### END: data.gms";