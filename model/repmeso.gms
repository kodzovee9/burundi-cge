* repmeso.gms

DISPLAY "#### START: repmeso.gms";

PARAMETER

  qegrowth(ac,*)               real exports average growth rate from first to last year of simulation
  qegrowthyy(*,ac,t)           real exports yearly growth rate from first to last year of simulation
  qerealxp(sim,ac,t)             real exports percent dev wrt base of real exports from first to last year of simulation

  qegrowth2(acrep,sim)               real exports average growth rate from first to last year of simulation
  qegrowthyy2(sim,acrep,t)           real exports yearly growth rate from first to last year of simulation
  qerealxp2(sim,acrep,t)             real exports percent dev wrt base from first to last year of simulation
  qerealxpTT2(acrep,sim,t)             real exports percent dev wrt base for selected years

                                 
  qmgrowth(ac,*)               real imports average growth rate from first to last year of simulation
  qmgrowthyy(sim,ac,t)           real imports yearly growth rate from first to last year of simulation
  qmrealxp(sim,ac,t)             real imports percent dev wrt base of real imports from first to last year of simulation


  qmgrowth2(acrep,sim)               real imports average growth rate from first to last year of simulation
  qmgrowthyy2(sim,acrep,t)           real imports yearly growth rate from first to last year of simulation
  qmrealxp2(sim,acrep,t)             real imports percent dev wrt base from first to last year of simulation
  qmrealxpTT2(acrep,sim,t)             real imports percent dev wrt base for selected years

                                 
  qvagrowth(ac,*)              real value added average growth rate from first to last year of simulation
  qvagrowthyy(sim,ac,t)          real value added yearly growth rate from first to last year of simulation
  qvarealxp(sim,ac,t)            real value added percent dev wrt base of real value added from first to last year of simulation
  qvarealyy(sim,ac,t)            level of real value added from first to last year of simulation

  qvagrowth2(acrep,*)             real value added average growth rate from first to last year of simulation (aggregated commodities)
  qvagrowthyy2(sim,acrep,t)         real value added yearly growth rate from first to last year of simulation (aggregated commodities)
  qvarealxp2(sim,acrep,t)           real value added percent dev wrt base of real value added from first to last year of simulation (aggregated commodities)
  qvarealyy2(sim,acrep,t)           level of real value added from first to last year of simulation (aggregated commodities)

  qvarealxp2tt(sim,acrep,t)            real value added percent dev wrt base of real value added for selected years (aggregated commodities)
  
  qdgrowth(ac,*)               real domestic sales average growth rate from first to last year of simulation
  qdgrowthyy(sim,ac,t)           real domestic sales yearly growth rate from first to last year of simulation
  qdrealxp(sim,ac,t)             real domestic sales percent dev wrt base of real value added from first to last year of simulation  
  
  qggrowth(ac,*)               real government consumption average growth rate from first to last year of simulation
  qggrowthyy(sim,ac,t)           real government consumption yearly growth rate from first to last year of simulation
  qgrealxp(sim,ac,t)             real government consumption percent dev wrt base of real value added from first to last year of simulation

  qhgrowth(ac,ac,*)            real household consumption average growth rate from first to last year of simulation
  qhgrowthyy(sim,ac,ac,t)        real household consumption yearly growth rate from first to last year of simulation
  qhrealxp(sim,ac,ac,t)          real household consumption percent dev wrt base of real value added from first to last year of simulation
  
  
  qhpcgrowth(ac,*)            real household consumption per capita average growth rate from first to last year of simulation
  qhpcgrowthyy(sim,ac,t)        real household consumption per capita yearly growth rate from first to last year of simulation
  qhpcrealxp(sim,ac,t)          real household consumption per capita percent dev wrt base of real value added from first to last year of simulation
  qhpcrealyy(sim,ac,t)          level of real household consumption per capita from first to last year of simulation
  
  
  employgrowth(ac,*)           employment average growth rate from first to last year of simulation
  employgrowthyy(sim,ac,t)       employment yearly growth rate from first to last year of simulation
  employxp(sim,ac,t)             employment percent dev wrt base of real value added from first to last year of simulation
  employgrowth2(acrep,sim)           employment average growth rate from first to last year of simulation
  employxp2(sim,acrep,t)             employment percent dev wrt base from first to last year of simulation
  employxpTT2(acrep,sim,t)             employment percent dev wrt base for selected years

  
  wagegrowth(f,*)              factor remunerations average growth rate from first to last year of simulation
  wagegrowthyy(sim,f,t)          factor remunerations yearly growth rate from first to last year of simulation
  wagexp(sim,f,t)                factor remunerations percent dev wrt base of real value added from first to last year of simulation
  
  capgdprep(ac,*)              capital stock as GDP share first and last year of simulation
  capgdpyy(sim,ac,t)             capital stock as GDP share from first to last year of simulation
  caprealyy(sim,ac,t)            level of real capital stock from first to last year of simulation
  caprealxp(sim,ac,t)            real capital stock percent dev wrt base of real value added from first to last year of simulation

  govdetgdp(fiscaldetcol,ac,*)            government detailed budget as GDP share first and last year of simulation
  govdetgdpyy(sim,fiscaldetcol,ac,t)      government detailed budget as GDP share from first to last year of simulation

  govdetnom(fiscaldetcol,ac,*)            nominal government detailed budget first and last year of simulation
  govdetnomyy(sim,fiscaldetcol,ac,t)      nominal government detailed budget from first to last year of simulation
  
  
  unempratetot(t,sim)                   total unemployment rate
  
  
;

* qegrowth ==========================================================

qegrowth(ac,tminrep) = sectorindic(ac,'EXP','Real',tminrep,'base');
qegrowth(ac,simcur) = sectorindicXPPREP(ac,'EXP','Real',simcur);

* qegrowthyy ========================================================

qegrowthyy(simcur,ac,tmin) = sectorindic(ac,'EXP','Real',tmin,'base');
qegrowthyy(simcur,ac,t)$tnmin(t) = sectorindicXPY(ac,'EXP','Real',t,simcur);

* qerealxp ==========================================================  

qerealxp(simcur,ac,t) = sectorindicXP(ac,'EXP','Real',t,simcur);
qerealxp('baseyr',ac,tmin)$(dmod=0) = sectorindic(ac,'EXP','Real',tmin,'base'); 


* qegrowth2 =========================================================

qegrowth2(acrep,'baseyr') = SUM(tmin, sectorindic2(acrep,'EXP','Real',tmin,'base'));
qegrowth2(acrep,simcur) = sectorindic2XPPREP(acrep,'EXP','Real',simcur);



* qerealxp2 =========================================================


qerealxp2(simcur,acrep,t) = sectorindic2XP(acrep,'EXP','Real',t,simcur);
qerealxp2('baseyr',acrep,tmin)$(dmod=0) = sectorindic2(acrep,'EXP','Real',tmin,'base'); 

  

* qerealxpTT2 =======================================================

  
qerealxpTT2(acrep,simcur,t)$trep(t) = qerealxp2(simcur,acrep,t);
  


  
* qmgrowth ==========================================================

qmgrowth(ac,tminrep) = sectorindic(ac,'IMP','Real',tminrep,'base');
qmgrowth(ac,simcur) = sectorindicXPPREP(ac,'IMP','Real',simcur);

* qmgrowthyy ========================================================

qmgrowthyy(simcur,ac,tmin) = sectorindic(ac,'IMP','Real',tmin,'base');
qmgrowthyy(simcur,ac,t)$tnmin(t) = sectorindicXPY(ac,'IMP','Real',t,simcur);


* qmrealxp ==========================================================

qmrealxp(simcur,ac,t) = sectorindicXP(ac,'IMP','Real',t,simcur);
qmrealxp('baseyr',ac,tmin)$(dmod=0) = sectorindic(ac,'IMP','Real',tmin,'base'); 


* qmgrowth2 ==========================================================

qmgrowth2(acrep,'baseyr') = SUM(tmin, sectorindic2(acrep,'IMP','Real',tmin,'base'));
qmgrowth2(acrep,simcur) = sectorindic2XPPREP(acrep,'IMP','Real',simcur);
  
  

* qmrealxp2 =========================================================


qmrealxp2(simcur,acrep,t) = sectorindic2XP(acrep,'IMP','Real',t,simcur);
qmrealxp2('baseyr',acrep,tmin)$(dmod=0) = sectorindic2(acrep,'IMP','Real',tmin,'base'); 
  
* qmrealxpTT2 =======================================================

qmrealxpTT2(acrep,simcur,t)$trep(t) = qmrealxp2(simcur,acrep,t);


* qvagrowth =========================================================

qvagrowth(ac,tminrep) = sectorindic(ac,'VA','Real',tminrep,'base');
qvagrowth(ac,simcur) = sectorindicXPPREP(ac,'VA','Real',simcur);


* qvagrowthyy =======================================================

qvagrowthyy(simcur,ac,tmin) = sectorindic(ac,'VA','Real',tmin,'base');
qvagrowthyy(simcur,ac,t)$tnmin(t) = sectorindicXPY(ac,'VA','Real',t,simcur);


* qvarealxp =========================================================
  
qvarealxp(simcur,ac,t) = sectorindicXP(ac,'VA','Real',t,simcur);
qvarealxp('baseyr',ac,tmin)$(dmod=0) = sectorindic(ac,'VA','Real',tmin,'base'); 



* qvarealyy =========================================================

qvarealyy(simcur,a,t) = sectorindic(a,'VA','Real',t,simcur);
  
* qvagrowth2 ========================================================
  
qvagrowth2(acrep,tminrep) = sectorindic2(acrep,'VA','Real',tminrep,'base');
qvagrowth2(acrep,simcur) = sectorindic2XPPREP(acrep,'VA','Real',simcur);


  
  
* qvagrowthyy2 ======================================================

qvagrowthyy2(simcur,acrep,tmin) = sectorindic2(acrep,'VA','Real',tmin,'base');
qvagrowthyy2(simcur,acrep,t)$tnmin(t) = sectorindic2XPY(acrep,'VA','Real',t,simcur);

* qvarealxp2 ========================================================

qvarealxp2(simcur,acrep,t) = sectorindic2XP(acrep,'VA','Real',t,simcur);
qvarealxp2('baseyr',acrep,tmin)$(dmod=0) = sectorindic2(acrep,'VA','Real',tmin,'base'); 


* qvarealyy2 ========================================================

qvarealyy2(simcur,acrep,t) = sectorindic2(acrep,'VA','Real',t,simcur);
    
* qvarealxp2tt ======================================================

qvarealxp2tt(simcur,acrep,t)$trep(t) = qvarealxp2(simcur,acrep,t);
  
  
  
  
* qdgrowth ==========================================================

qdgrowth(ac,tminrep) = sectorindic(ac,'DOM','Real',tminrep,'base');
qdgrowth(ac,simcur) = sectorindicXPPREP(ac,'DOM','Real',simcur);


* qdgrowthyy ========================================================

qdgrowthyy(simcur,ac,tmin) = sectorindic(ac,'DOM','Real',tmin,'base');
qdgrowthyy(simcur,ac,t) = sectorindicXPY(ac,'DOM','Real',t,simcur);

* qdrealxp ==========================================================  

qdrealxp(simcur,ac,t) = sectorindicXP(ac,'DOM','Real',t,simcur);
qdrealxp('baseyr',ac,tmin)$(dmod=0) = sectorindic(ac,'DOM','Real',tmin,'base'); 


* qggrowth ==========================================================

qggrowth(c,tminrep) = SUM(insgov, PQD00(c,insgov))*QGX(c,tminrep,'base');
qggrowth(c,simcur) = QGXPPREP(c,simcur);



* qggrowthyy ========================================================

qggrowthyy(simcur,c,tmin) = SUM(insgov, PQD00(c,insgov))*QGX(c,tmin,'base');
qggrowthyy(simcur,c,t) = QGXPY(c,t,simcur);

* qgrealxp ==========================================================
  
qgrealxp(simcur,c,t) = QGXP(c,t,simcur);
qgrealxp('baseyr',c,tmin)$(dmod=0) = SUM(insgov, PQD00(c,insgov))*QGX(c,tmin,'base'); 
  
  
* qhgrowth ==========================================================

qhgrowth(ac,h,tminrep) = QHX(ac,h,tminrep,'base'); 
qhgrowth(ac,h,simcur) = QHXPPREP(ac,h,simcur);
  
* qhgrowthyy ========================================================

qhgrowthyy(simcur,ac,h,tminrep) = QHX(ac,h,tminrep,'base'); 
qhgrowthyy(simcur,ac,h,t) = QHXPY(ac,h,t,simcur);
  

  
* qhrealxp ==========================================================
  
qhrealxp(simcur,ac,h,t) = QHXP(ac,h,t,simcur);
qhrealxp('baseyr',ac,h,tmin)$(dmod=0) = qhgrowth(ac,h,'baseyr'); 
  
 
  
* qhpcgrowth ========================================================

qhpcgrowth(ac,tminrep) = qhpcX(ac,tminrep,'base'); 
qhpcgrowth(ac,simcur)  = qhpcXPP(ac,simcur);

* qhpcgrowthyy ======================================================

*qhpcgrowthyy(sim,ac,ac,t) =

* qhpcrealxp ========================================================

qhpcrealxp(sim,ac,t) = qhpcXP(ac,t,sim);

* qhpcrealyy ========================================================

qhpcrealyy(sim,ac,t) = qhpcX(ac,t,sim);
  
  
  
  
* employgrowth ======================================================

employgrowth(ac,tminrep) = QFX('tot-lab',ac,tminrep,'base');
employgrowth(ac,simcur) = QFXPPREP('tot-lab',ac,simcur);

* employgrowthyy ====================================================

employgrowthyy(simcur,ac,tmin) = QFX('tot-lab',ac,tmin,'base');
employgrowthyy(simcur,a,t) = QFXPY('tot-lab',a,t,simcur);

* employxp ==========================================================

employxp(simcur,ac,t) = QFXP('tot-lab',ac,t,simcur);
employxp('baseyr',ac,tmin)$(dmod=0) = QFX('tot-lab',ac,tmin,'base'); 
  

* employgrowth2 ======================================================

employgrowth2(acrep,'baseyr') = SUM(tmin, SUM(a$macacrep(a,acrep), QFX('tot-lab',a,tmin,'base')))*scaling('qlabsolrep');
employgrowth2(acrep,simcur) = sectorindic2XPPREP(acrep,'EMP','Real',simcur);

* employxp2 =========================================================

employxp2(simcur,acrep,t) = sectorindic2xp(acrep,'EMP','real',t,simcur);
employxp2('baseyr',acrep,tmin)$(dmod=0) = employgrowth2(acrep,'baseyr'); 
employxp2(simcur,acrep,tmin)$(dmod AND simnbase(simcur)) = employgrowth2(acrep,'baseyr');

* employxpTT2 =======================================================


employxpTT2(acrep,'baseyr',tmin) =  employgrowth2(acrep,'baseyr');
employxpTT2(acrep,simcur,trep)$simnbase(simcur) = employxp2(simcur,acrep,trep);


  
* wagegrowth ========================================================
  
wagegrowth(f,tminrep) = WFAVGX(f,tminrep,'base');
wagegrowth(f,simcur) = WFAVGXPPREP(f,simcur);

* wagegrowthyy ======================================================

wagegrowthyy(simcur,f,tmin) = WFAVGX(f,tmin,'base');
wagegrowthyy(simcur,f,t) = WFAVGXPY(f,t,simcur);

* wagexp ============================================================  

wagexp(simcur,f,t) = WFAVGXP(f,t,simcur);
wagexp('baseyr',f,tmin)$(dmod=0) = WFAVGX(f,tmin,'base'); 



  
* capgdprep ============================================================

capgdprep(fcap,tminrep) = 100*QFSX(fcap,tminrep,'base')*PKX(fcap,tminrep,'base')/GDPMPX(tminrep,'base'); 
capgdprep(fcap,simcur)$SUM(tmax, GDPMPX(tmax,simcur)) = 100*SUM(tmax, QFSX(fcap,tmax,'base')*PKX(fcap,tmax,simcur)/GDPMPX(tmax,simcur)); 

* capgdpyy ==========================================================

capgdpyy(simcur,fcap,t)$GDPMPX(t,simcur) = 100*QFSX(fcap,t,simcur)*PKX(fcap,t,simcur)/GDPMPX(t,simcur);

* caprealyy =========================================================

caprealyy(simcur,fcap,t) = 100*QFSX(fcap,t,simcur)*PK00(fcap);
  
* caprealxp =========================================================
  
caprealxp(simcur,fcap,t) = QFSXP(fcap,t,simcur);
caprealxp('baseyr',fcap,tmin)$(dmod=0) = QFSX(fcap,tmin,'base'); 

* govdetgdp =========================================================

govdetgdp(fiscaldetcol,ac,tminrep) = fiscaldetindic(fiscaldetcol,ac,'GDPshr',tminrep,'base');
govdetgdp(fiscaldetcol,ac,simcur) = SUM(tmaxrep, fiscaldetindic(fiscaldetcol,ac,'GDPshr',tmaxrep,simcur));


* govdetgdpyy =======================================================

govdetgdpyy(simcur,fiscaldetcol,ac,t) = fiscaldetindic(fiscaldetcol,ac,'GDPshr',t,simcur);



* govdetnom =========================================================

govdetnom(fiscaldetcol,ac,tminrep) = fiscaldetindic(fiscaldetcol,ac,'Nominal',tminrep,'base');
govdetnom(fiscaldetcol,ac,simcur) = SUM(tmaxrep, fiscaldetindic(fiscaldetcol,ac,'Nominal',tmaxrep,simcur));


* govdetnomyy =======================================================

govdetnomyy(simcur,fiscaldetcol,ac,t) = fiscaldetindic(fiscaldetcol,ac,'Nominal',t,simcur);


* unempratetot(t,sim) 

unempratetot(t,simcur) = UERATX('tot-lab',t,simcur)*100;




* set growth rates to zero if static model
qegrowth(ac,sim)$(dmod=0)             = 0;              
qegrowthyy(sim,ac,t)$(dmod=0)         = 0;          
qmgrowth(ac,sim)$(dmod=0)             = 0;              
qmgrowthyy(sim,ac,t)$(dmod=0)         = 0;          
qvagrowth(ac,sim)$(dmod=0)            = 0;             
qvagrowth2(acrep,sim)$(dmod=0)           = 0;             
qvagrowthyy(sim,ac,t)$(dmod=0)        = 0;         
qdgrowth(ac,sim)$(dmod=0)             = 0;              
qdgrowthyy(sim,ac,t)$(dmod=0)         = 0;          
qggrowth(ac,sim)$(dmod=0)             = 0;
qggrowthyy(sim,ac,t)$(dmod=0)         = 0;
qhgrowth(ac,acp,sim)$(dmod=0)         = 0;
qhgrowthyy(sim,ac,acp,t)$(dmod=0)     = 0;
employgrowth(ac,sim)$(dmod=0)         = 0;
employgrowthyy(sim,ac,t)$(dmod=0)     = 0;
wagegrowth(f,sim)$(dmod=0)            = 0;
wagegrowthyy(sim,f,t)$(dmod=0)        = 0;



* start: poverty module  

PARAMETER
  fgt0(ac,*)        headcount ratio (P0) first and last year of simulation
  fgt1(ac,*)        poverty gap (P1) first and last year of simulation
  fgt2(ac,*)        poverty severity (P2) first and last year of simulation

  fgt0yy(sim,ac,t)    headcount ratio (P0) from first to last year of simulation
  fgt1yy(sim,ac,t)    poverty gap (P1) from first to last year of simulation
  fgt2yy(sim,ac,t)    poverty severity (P2) from first to last year of simulation

  fgt0_ext(ac,*)        headcount ratio (P0) (extreme PL) first and last year of simulation
  fgt1_ext(ac,*)        poverty gap (P1) (extreme PL) first and last year of simulation
  fgt2_ext(ac,*)        poverty severity (P2) (extreme PL) first and last year of simulation

  fgt0yy_ext(sim,ac,t)    headcount ratio (P0) from first to last year of simulation
  fgt1yy_ext(sim,ac,t)    poverty gap (P1) from first to last year of simulation
  fgt2yy_ext(sim,ac,t)    poverty severity (P2) from first to last year of simulation
  
  ginirep(ac,*)        gini coefficient first and last year of simulation
  ginirepyy(sim,ac,t)    gini coefficient from first to last year of simulation
  
;

fgt0(ac,sim)        = 0;
fgt1(ac,sim)        = 0;
fgt2(ac,sim)        = 0;
fgt0yy(sim,ac,t)    = 0;
fgt1yy(sim,ac,t)    = 0;
fgt2yy(sim,ac,t)    = 0;

fgt0_ext(ac,sim)        = 0;
fgt1_ext(ac,sim)        = 0;
fgt2_ext(ac,sim)        = 0;
fgt0yy_ext(sim,ac,t)    = 0;
fgt1yy_ext(sim,ac,t)    = 0;
fgt2yy_ext(sim,ac,t)    = 0;

ginirep(ac,sim)     = 0;
ginirepyy(sim,ac,t) = 0;

* end: poverty module  


  
* save GDX file
$IF %NonIMv2%==1 EXECUTE_UNLOAD 'repmeso2-%app%.gdx',
$IF NOT %NonIMv2%==1 EXECUTE_UNLOAD 'repmeso2.gdx',
  qegrowth
  qegrowthyy
  qerealxp

  qmgrowth
  qmgrowthyy
  qmrealxp 
  
  qvagrowth
  qvagrowthyy
  qvarealxp
  qvarealyy
 
  qvagrowth2 
  qvagrowthyy2   
  qvarealxp2     
  qvarealyy2     
  
  qdgrowth
  qdgrowthyy
  qdrealxp

  qggrowth
  qggrowthyy
  qgrealxp 
  
  qhgrowth
  qhgrowthyy
  qhrealxp
  
  qhpcgrowth
*  qhpcgrowthyy
  qhpcrealxp
  qhpcrealyy

  
  employgrowth
  employgrowthyy
  employxp 
  employxp2
  
  wagegrowth
  wagegrowthyy
  wagexp
  
  capgdp
  capgdpyy
  caprealyy
  caprealxp
  capgdprep
  
  taxratact
  taxratcom
  subratcom
  taxratins
  
  govdetgdp
  govdetgdpyy
  
  govdetnom
  govdetnomyy
  
  unempratetot
  

;




DISPLAY "#### END: repmeso.gms";

