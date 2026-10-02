* repsetup.inc

DISPLAY "#### START: repsetup.inc";

SET

  maccol                macro items
    /
      IntermCon         intermediate consumption
      DistMarg          distribution margin
      Absorption        total absorption
      PrvCon            private consumption
      GovCon            government consumption                        	  
      FixInv            fixed investment
      PrvFixInv         private fixed investment
      GovFixInv         government fixed investment
      GovFixInvInfra    government fixed investment in infra
      StockChange       stock change                                  
      TourismReceipts   tourism receipts
      Exports           exports                                       
      Imports           imports                                       
      GDPMP             GDP at market prices (alt. 1: spending)       
      GDPMP_alt         GDP at market prices (alt. 2: value-added)    
      NetIndTax         net indirect taxes                            
      GDPFC             GDP at factor cost        
      REXR              real exchange rate
      Wage              average wage
      CapRet            average capital return
      UnempRat          unemployment rate

      SavFor            foreign savings
      SavGov            government savings
      SavIns            domestic non-governments inst savings

      ForDebtGov        (net) foreign debt of government
      ForDebtPrv        (net) foreign debt of non-government (private)
      DomDebtGov        (net) domestic debt of government 
      
    /


  igdp(maccol)          items for GDP and national accounts
    /
      Absorption        total absorption
      PrvCon            private consumption
      FixInv            fixed investment
      PrvFixInv         private fixed investment
      GovFixInv         government fixed investment
      StockChange       stock change                                  
      GovCon            government consumption                        
      Exports           exports                                       
      Imports           imports                                       
      GDPMP             GDP at market prices (alt. 1: spending)       
      GDPMP_alt         GDP at market prices (alt. 2: value-added)    
      NetIndTax         net indirect taxes                            
      GDPFC             GDP at factor cost                            
    /

  igdp2(igdp)           items for GDPMP summation
    /
      PrvCon            private consumption
      FixInv            fixed investment
      StockChange       stock change                                  
      GovCon            government consumption                        
      Exports           exports                                       
      Imports           imports                                       
    /

  kgdp                  second index in GDP tables
    /Nominal, Real, GDPshr, GDPshr-Real, NomFCU, shrTOT/

  ac2                   todas las cuentas de la macrosam
    /
      act2              activities
      com2              commodities
      marg2             trade and transport margins
      lab2
      cap2
      fac2              factors
      
      imp2              importaciones
      hhd2              domestic non-gov institution (households + enterprises)
      ent2
      gov2              current government
      row2              rest of world
      tax2              taxes
      sub2              subsidies
      rebate-vat2 
      prqmbar2
      prexr2
      
      cap-ngov2
      cap-gov2
      cap-row2
      drf2
      
      inv-prv2
      inv-gov2
      dstk2

      s-i2              savings investment account


      total2            totals
    
    /

  sectorcol
    /
   VAshr                value-added share (%)
   PRDshr               production share (%)
   EMPshr               share in total employment (%)
   EXPshr               sector share in total exports (%)
   EXP-OUTshr           exports as share in sector output (%)
   IMPshr               sector share in total imports (%)
   IMP-DEMshr           imports as share of domestic demand (%)
   
    /

  sectorcol1(sectorcol)
    /
   VAshr       value-added share (%)
   PRDshr      production share (%)
   EMPshr      share in total employment (%)
   EXPshr      sector share in total exports (%)
   IMPshr      sector share in total imports (%)
    /  

  sectorcol2(sectorcol)
    /
   EXP-OUTshr           exports as share in sector output (%)
   IMP-DEMshr           imports as share of domestic demand (%)
    /  


  sectorcol3
    /
   VA          value-added
   EXP         exports
   IMP         imports
   DOM         domestic sales
   EMP         employment   
   Output      output
    
    /

  taxcol
    /
    TaxRev
    shrTaxRev
    GDPshr
    /
 
  fiscalcol
    /
    
    IncCur-DirTax
    IncCur-FacTax
    IncCur-FacActTax
    IncCur-ActTax
    IncCur-ComTax
    IncCur-VATax
    IncCur-FacUseTax
    IncCur-ImpTax
    IncCur-ExpTax
    IncCur-TrnsfrDom
    IncCur-TrnsfrFor
    IncCur-FacInc
    IncCur-YPRQMBAR
    IncCur-YPREXR
    IncCur-Total
                         
                      
    SpndCur-GovCon
    SpndCur-TrnsfrDom
    SpndCur-TrnsfrFor
    SpndCur-ComSub
    SpndCur-Total    

    GovSav
    SpndCap-Inv
    GovSurp
    IncCap-NetDomFin
    IncCap-NetForFin
    Inc-Total
    Spnd-Total


    
    /  
    
  fiscalcol1(fiscalcol)
    /
    IncCur-DirTax
    IncCur-FacTax
    IncCur-FacActTax
    IncCur-ActTax
    IncCur-ComTax
    IncCur-VATax
    IncCur-FacUseTax
    IncCur-ImpTax
    IncCur-ExpTax
    IncCur-TrnsfrDom
    IncCur-TrnsfrFor
    IncCur-FacInc
    IncCur-YPRQMBAR
    IncCur-YPREXR
    
    IncCap-NetDomFin
    IncCap-NetForFin

    /  
    
  fiscalcol2(fiscalcol)
    /
    SpndCur-GovCon
    SpndCur-ComSub
    SpndCur-TrnsfrDom
    SpndCur-TrnsfrFor
    
    SpndCap-Inv
    /  

  fiscaldetcol
    /
Income-Current
Spending-Current
Savings
Spending-Capital
Surplus
Income-Capital
Income-Total
Spending-Total    
    
    /

  bopcol
    /
* inflows
  
    InCur-Exports             
    InCur-TourismReceipts     
    InCur-Trnsfr-NoGov
    InCur-Trnsfr-Gov
    InCur-FacInc
    InCur-RowSav
    InCur-Total
    
* outflows
    OutCur-Imports
    OutCur-Trnsfr-NoGov
    OutCur-Trnsfr-Gov
    OutCur-FacInc
    OutCur-Total

* (net) capital inflows    
    InCap-NetForFin-NoGov
    InCap-NetForFin-Gov
    InCap-FDI
    InCap-ChangeForRes
    InCap-Total         
    / 
    
  bopcol1(bopcol)
    /
    InCur-Exports
    InCur-TourismReceipts
    InCur-Trnsfr-NoGov
    InCur-Trnsfr-Gov
    InCur-FacInc
    InCur-RowSav
    / 

  bopcol2(bopcol)
    /
    OutCur-Imports
    OutCur-Trnsfr-NoGov
    OutCur-Trnsfr-Gov
    OutCur-FacInc

    / 
    
  bopcol3(bopcol)
    /
    InCap-NetForFin-NoGov
    InCap-NetForFin-Gov
    InCap-FDI
    InCap-ChangeForRes
    /
    
  demcol
    /
    IntermCon
    DistMarg
    PrvCon
    GovCon
    FixInv
    StockChange
    TourismReceipts
    Exports
    TotDem
    /



  inccol    
    /
    POPshr

    INCshr
    CONshr

    INCpc
    CONpc

    INCval
    ENDOWval
    ENDOWshr
    
    
    /    

  debtcol
    /
    NonGov-ForDebt-GDPshr
    Gov-ForDebt-GDPshr
    Gov-DomDebt-GDPshr
   
    /
    
  spndcol 
    /
    SPNDshr
    SPNDval
    /    
    

  emicol
    /
    EmiVol       emissions volume
    EmiSHR-ByUser       emissions share 
    EmiSHR-ByCommodity       emissions share 
    /
    
  solcol
    /
solvestat
modelstat    
    /        


  ac2nt(ac2)  todas las cuentas de la macrosam excepto total
  
;


    
ac2nt(ac2) = YES;
ac2nt('total2') = NO;

ALIAS (ac2p,ac2), (ac2ntp,ac2nt);

PARAMETER

* level endogenous variables (name of variable  + X)  
* % deviation from baseline result (name of variable + XP)
* average % annual growth rates bt tmin and tmax (name of variable + XPP)
* average % annual growth rates bt tminrep and tmaxrep (name of variable + XPPREP)
* % annual growth from preceding period (name of variable + XPY)
* see par-decl-aux.inc


* other reports
  mpkX(ac,fcap,t,sim)          marginal product in act a per unit of additional capital stock fcap

  qfinsbX(ins,ac,t,sim)                 exogenous real endowment of factor ac for institution ins

  poptotX(t,sim)
  popx(ac,t,sim)                   population (level)
  popxp(ac,t,sim)                  population (% deviation from base in t)
  popxpp(ac,sim)                   population (avg annual % growth tmin – tmax)
  popxpy(ac,t,sim)                 population (annual % growth from t-1) 
  popxpprep(ac,sim)                population (avg annual % growth tminrep – tmaxrep)

  pwex(c,t,sim)                    export price for c (foreign currency) (level)
  pwexp(c,t,sim)                   export price for c (foreign currency) (% deviation from base in t)
  pwexpp(c,sim)                  export price for c (foreign currency) (avg annual % growth tmin – tmax)
  pwexpy(c,t,sim)                  export price for c (foreign currency) (annual % growth from t-1) 
  pwexpprep(c,sim)               export price for c (foreign currency) (avg annual % growth tminrep – tmaxrep)

  pwmx(c,t,sim)                    import price for c (foreign currency) (level)
  pwmxp(c,t,sim)                   import price for c (foreign currency) (% deviation from base in t)
  pwmxpp(c,sim)                  import price for c (foreign currency) (avg annual % growth tmin – tmax)
  pwmxpy(c,t,sim)                  import price for c (foreign currency) (annual % growth from t-1) 
  pwmxpprep(c,sim)               import price for c (foreign currency) (avg annual % growth tminrep – tmaxrep)

  qa2x(a,t,sim)                    intermediate variable to compute tfpa (output with constant productivity indexes) (level)
  qa2xp(a,t,sim)                   intermediate variable to compute tfpa (output with constant productivity indexes) (% deviation from base in t)
  qa2xpp(a,sim)                    intermediate variable to compute tfpa (output with constant productivity indexes) (avg annual % growth tmin – tmax)
  qa2xpy(a,t,sim)                  intermediate variable to compute tfpa (output with constant productivity indexes) (annual % growth from t-1) 
  qa2xpprep(a,sim)                 intermediate variable to compute tfpa (output with constant productivity indexes) (avg annual % growth tminrep – tmaxrep)

  qgbx(c,t,sim)                    exogenous qnty of government demand for commodity c (level)
  qgbxp(c,t,sim)                   exogenous qnty of government demand for commodity c (% deviation from base in t)
  qgbxpp(c,sim)                    exogenous qnty of government demand for commodity c (avg annual % growth tmin – tmax)
  qgbxpy(c,t,sim)                  exogenous qnty of government demand for commodity c (annual % growth from t-1) 
  qgbxpprep(c,sim)                 exogenous qnty of government demand for commodity c (avg annual % growth tminrep – tmaxrep)

  qhpcx(ac,t,sim)                  household per capita consumption (level)
  qhpcxp(ac,t,sim)                 household per capita consumption (% deviation from base in t)
  qhpcxpp(ac,sim)                  household per capita consumption (avg annual % growth tmin – tmax)
  qhpcxpy(ac,t,sim)                household per capita consumption (annual % growth from t-1) 
  qhpcxpprep(ac,sim)               household per capita consumption (avg annual % growth tminrep – tmaxrep)

  qhpc2x(acrep,t,sim)                  household per capita consumption (level)
  qhpc2xp(acrep,t,sim)                 household per capita consumption (% deviation from base in t)
  qhpc2xpp(acrep,sim)                  household per capita consumption (avg annual % growth tmin – tmax)
  qhpc2xpy(acrep,t,sim)                household per capita consumption (annual % growth from t-1) 
  qhpc2xpprep(acrep,sim)               household per capita consumption (avg annual % growth tminrep – tmaxrep)

  yhpcx(ac,t,sim)                  household per capita income (level)
  yhpcxp(ac,t,sim)                 household per capita income (% deviation from base in t)
  yhpcxpp(ac,sim)                  household per capita income (avg annual % growth tmin – tmax)
  yhpcxpy(ac,t,sim)                household per capita income (annual % growth from t-1) 
  yhpcxpprep(ac,sim)               household per capita income (avg annual % growth tminrep – tmaxrep)

  rsavgx(t,sim)                    real government savings (level)
  rsavgxp(t,sim)                   real government savings (% deviation from base in t)
  rsavgxpp(sim)                    real government savings (avg annual % growth tmin – tmax)  
  rsavgxpy(t,sim)                  real government savings (annual % growth from t-1) 
  rsavgxpprep(sim)                 real government savings (avg annual % growth tminrep – tmaxrep)  

  savgx(t,sim)                     government savings (level)
  savgxp(t,sim)                    government savings (% deviation from base in t)
  savgxpp(sim)                     government savings (avg annual % growth tmin – tmax)  
  savgxpy(t,sim)                   government savings (annual % growth from t-1) 
  savgxpprep(sim)                  government savings (avg annual % growth tminrep – tmaxrep)  

  tfpax(a,t,sim)                   activity-specific implied total factor productivity (level)
  tfpaxp(a,t,sim)                  activity-specific implied total factor productivity (% deviation from base in t)
  tfpaxpp(a,sim)                   activity-specific implied total factor productivity (avg annual % growth tmin – tmax)
  tfpaxpy(a,t,sim)                 activity-specific implied total factor productivity (annual % growth from t-1) 
  tfpaxpprep(a,sim)                activity-specific implied total factor productivity (avg annual % growth tminrep – tmaxrep)

  tfpatotx(t,sim)                  overall implied total factor productivity (level)
  tfpatotxp(t,sim)                 overall implied total factor productivity (% deviation from base in t)
  tfpatotxpp(sim)                  overall implied total factor productivity (avg annual % growth tmin – tmax)
  tfpatotxpy(t,sim)                overall implied total factor productivity (annual % growth from t-1) 
  tfpatotxpprep(sim)               overall implied total factor productivity (avg annual % growth tminrep – tmaxrep)

  wfarealx(f,a,t,sim)              real wage for factor f in activity a (level)
  wfarealxp(f,a,t,sim)             real wage for factor f in activity a (% deviation from base in t)
  wfarealxpp(f,a,sim)              real wage for factor f in activity a (avg annual % growth tmin – tmax)
  wfarealxpy(f,a,t,sim)            real wage for factor f in activity a (annual % growth from t-1) 
  wfarealxpprep(f,a,sim)           real wage for factor f in activity a (avg annual % growth tminrep – tmaxrep)

  wfrealx(f,t,sim)                 real average price of factor f (level)
  wfrealxp(f,t,sim)                real average price of factor f (% deviation from base in t)
  wfrealxpp(f,sim)                 real average price of factor f (avg annual % growth tmin – tmax)
  wfrealxpy(f,t,sim)               real average price of factor f (annual % growth from t-1) 
  wfrealxpprep(f,sim)              real average price of factor f (avg annual % growth tminrep – tmaxrep)

  shvatrbtx(c,a,t,sim)             share of VAT rebate
  shvatrbtxp(c,a,t,sim)            share of VAT rebate
  shvatrbtxpp(c,a,sim)           share of VAT rebate
  shvatrbtxpy(c,a,t,sim)           share of VAT rebate
  shvatrbtxpprep(c,a,sim)        share of VAT rebate

  trnsfrpcX(h,ins,t,sim)  transfers per capita from gov or row to households 

                                             
  modsolstat(solcol,t,sim)                   model and solver status
                                             
  gdpindic(igdp,kgdp,t,sim)                  gdp table (level)
  gdpindicxp(igdp,kgdp,t,sim)                gdp table (% deviation from base in t)
  gdpindicxpp(igdp,kgdp,sim)                 gdp table (avg annual % growth tmin – tmax)
  gdpindicxpy(igdp,kgdp,t,sim)               gdp table (annual % growth from t-1) 
  gdpindicxpprep(igdp,kgdp,sim)              gdp table (avg annual % growth tminrep – tmaxrep)
  
  gdpexpindic(maccol,kgdp,t,sim)             expanded gdp table (level)
  gdpexpindicxp(maccol,kgdp,t,sim)           expanded gdp table (% deviation from base in t)
  gdpexpindicxpp(maccol,kgdp,sim)          expanded gdp table (avg annual % growth tmin – tmax)
  gdpexpindicxpy(maccol,kgdp,t,sim)          expanded gdp table (annual % growth from t-1) 
  gdpexpindicxpprep(maccol,kgdp,sim)       expanded gdp table (avg annual % growth tminrep – tmaxrep)
  
  sectorindic(ac,sectorcol3,kgdp,t,sim)      sectoral results (level)
  sectorindicxp(ac,sectorcol3,kgdp,t,sim)    sectoral results (% deviation from base in t)
  sectorindicxpp(ac,sectorcol3,kgdp,sim)     sectoral results (avg annual % growth tmin – tmax)
  sectorindicxpy(ac,sectorcol3,kgdp,t,sim)   sectoral results (annual % growth from t-1) 
  sectorindicxpprep(ac,sectorcol3,kgdp,sim)  sectoral results (avg annual % growth tminrep – tmaxrep)
 
  sectorindic2(acrep,sectorcol3,kgdp,t,sim)      sectoral results (aggregated commodities) (level)
  sectorindic2xp(acrep,sectorcol3,kgdp,t,sim)    sectoral results (aggregated commodities) (% deviation from base in t)
  sectorindic2xpp(acrep,sectorcol3,kgdp,sim)     sectoral results (aggregated commodities) (avg annual % growth tmin – tmax)
  sectorindic2xpy(acrep,sectorcol3,kgdp,t,sim)   sectoral results (aggregated commodities) (annual % growth from t-1) 
  sectorindic2xpprep(acrep,sectorcol3,kgdp,sim)  sectoral results (aggregated commodities) (avg annual % growth tminrep – tmaxrep)
  
  emiindic(emicol,ac,ac,t,sim)    emissions indicators (level) (level)
  emiindicXP(emicol,ac,ac,t,sim)    emissions indicators (level) (% deviation from base in t)
  emiindicXPP(emicol,ac,ac,sim)    emissions indicators (level) (avg annual % growth tmin – tmax)
  emiindicXPPREP(emicol,ac,ac,sim)    emissions indicators (level) (avg annual % growth tminrep – tmaxrep)
  emiindicXPY(emicol,ac,ac,t,sim)    emissions indicators (level) (annual % growth from t-1)
                                             
  emiindic2(emicol,acrep,acrep,t,sim)    emissions indicators (aggregated) (level) (level)
  emiindic2XP(emicol,acrep,acrep,t,sim)    emissions indicators (aggregated) (level) (% deviation from base in t)
  emiindic2XPP(emicol,acrep,acrep,sim)    emissions indicators (aggregated) (level) (avg annual % growth tmin – tmax)
  emiindic2XPPREP(emicol,acrep,acrep,sim)    emissions indicators (aggregated) (level) (avg annual % growth tminrep – tmaxrep)
  emiindic2XPY(emicol,acrep,acrep,t,sim)    emissions indicators (aggregated) (level) (annual % growth from t-1)

  SIMSAM(ac,acp,t,sim)                       SAM defined using simulation results
  simsambalchk(ac,t,sim)                     simsam account surplus (row less column total)
  errsimsambalchk(ac,t,sim)
  simsamgdp(ac,acp,t,sim)                    SAM defined using simulation results (percent of GDP)
  simsampprep(ac,acp,sim)                    SAM defined using simulation results (avg annual % growth tminrep-tmaxrep)

  MACROSAM(ac2,ac2p,t,sim)                   macro sam defined using simulation results
  macrosambalchk(ac2,t,sim)                  macrosam account surplus (row less column total)
  errmacrosambalchk(ac2,t,sim)               simsam account surplus (row less column total)
  
  macrosam2(ac2,ac2p,t,sim)                   macro sam defined using simulation results
  macrosam2balchk(ac2,t,sim)                  macrosam account surplus (row less column total)
  errmacrosam2balchk(ac2,t,sim)               simsam account surplus (row less column total)
  macrosam2gdp(ac2,ac2p,t,sim)                macro sam defined using simulation results (percent of GDP)
   
  taxstruc(ac,taxcol,t,sim)    
  sectorstruc(ac,sectorcol,t,sim)            sectoral structure
  sectorstruc2(acrep,sectorcol,t,sim)           sectoral structure (aggregated commodities)
  
  fiscalindic(fiscalcol,kgdp,t,sim)          government budget
  errfiscalindic1(t,sim)                     if =UNDF imbalance between GovSav and (IncCur-Total - SpndCur-Total)
  errfiscalindic2(t,sim)                     if =UNDF imbalance between Inc-Total and Spnd-Total

  fiscaldetindic(fiscaldetcol,ac,kgdp,t,sim)            detailed government budget

  bopindic(bopcol,kgdp,t,sim)                balance of payments
  errbopindic1(t,sim)                        if =UNDF imbalance between InCur-RowSav and InCap-Total
  errbopindic2(t,sim)                        if =UNDF imablance between InCur-Total and OutCur-Total

  
  facdemstruc(ac,ac,t,sim)                   factor demand structure by activity
  facdemstruc2(acrep,acrep,t,sim)            factor demand structure by activity (aggregated activities and factors)

  actvashr(ac,t,sim)                         share of activity a in total value added
                                             
  ev(ac,t,sim)                                equivalent variation
  cv(ac,t,sim)                                compensatory variation
                                            
  demstruc(kgdp,ac,demcol,t,sim)             demand structure
  demstruc2(kgdp,acrep,demcol,t,sim)         demand structure (aggregated commodities)

  incomestruc(inccol,ac,ac,t,sim)            income structure by institution
  incomestruc2(inccol,acrep,acrep,t,sim)     income structure by institution (aggregated inst and income sources)
  incomedistindic(inccol,ac,t,sim)           income distribution
  
  capgdp(ac,t,sim)                           capital stocks-GDP ratio
  debtindic(debtcol,t,sim)                   debt stock indicators

  spndstruc(spndcol,ac,ac,t,sim)             spending structure by institution
  coststruc(ac,ac,t,sim)                     cost structure by activity
  coststruc2(acrep,acrep,t,sim)                     cost structure by activity (aggregated commodities and activities)
  salestruc(ac,ac,t,sim)                     sales structure by commodity

  laborsupstruc(ac,t,sim)                 structure of labor supply

  labordemstruc(ac,ac,t,sim)                 structure of labor demand by sector
  labordemstruc2(acrep,acrep,t,sim)          structure of labor demand by sector (aggregated activities)
  employsectorstruc(ac,ac,t,sim)             sectoral structure of employment
  employsectorstruc2(acrep,acrep,t,sim)      sectoral structure of employment (aggregated activities)


  taxratcom(c,acgovrec,t,sim)                tax rates for taxes on commodities
  taxratact(a,acgovrec,t,sim)                tax rates for taxes on activities
  subratcom(c,ac,t,sim)                      subsidy rates for subsidities on commodities by demander
  taxratins(ins,t,sim)                       tax rates for tax on income

  scaldelta_dsX(c,t,sim)          scaling factor for delta_ds
  scaldelta_eX(c,t,sim)           scaling factor for delta_e
  scalphi_xX(c,t,sim)             scaling factor for phi_x

;


SET
  mapmacrosam(ac,ac2)    mapping between ac and ac2 used in macrosam2
;
mapmacrosam(a,'act2')      = YES;
mapmacrosam(c,'com2')      = YES;
mapmacrosam(tacd,'marg2') = YES;
mapmacrosam(tacm,'marg2') = YES;
mapmacrosam(tace,'marg2') = YES;
mapmacrosam(flab,'lab2')   = YES;
mapmacrosam(f,'cap2')$(NOT flab(f))   = YES;
mapmacrosam(h,'hhd2')      = YES;
mapmacrosam(insngo,'hhd2') = YES;
mapmacrosam(insent,'ent2') = YES;
mapmacrosam(insgov,'gov2') = YES;
mapmacrosam(insrow,'row2') = YES;
mapmacrosam(insgov,'gov2') = YES;
mapmacrosam(actax,'tax2') = YES;
mapmacrosam(subcom,'sub2') = YES;
mapmacrosam('rebate-vat','rebate-vat2') = YES;
mapmacrosam(capinsdng,'cap-ngov2') = YES;
mapmacrosam(capgov,'cap-gov2') = YES;
mapmacrosam(caprow,'cap-row2') = YES;
mapmacrosam(capfin,'drf2') = YES;
mapmacrosam(invng,'inv-prv2') = YES;
mapmacrosam(invg,'inv-gov2') = YES;
mapmacrosam(dstk,'dstk2') = YES;
mapmacrosam('prqmbar','prqmbar2') = YES;
mapmacrosam('prexr','prexr2') = YES;
mapmacrosam('total','total2') = YES;


    

DISPLAY "#### END: repsetup.inc";


