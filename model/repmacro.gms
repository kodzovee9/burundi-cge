* repmacro.gms

DISPLAY "#### START: repmacro.gms";

PARAMETER

  macgrowth(maccol,*)            real macro indicators average growth rate from first to last year of simulataion
  macgrowthyy(sim,maccol,t)      real macro indicatores yearly growth rate from first to last year of simulataion

  govgdp(fiscalcol,*)            government budget as GDP share first and last year of simulation
  govgdpyy(sim,fiscalcol,t)      government budget as GDP share from first to last year of simulation

  govnom(fiscalcol,*)            nominal government budget first and last year of simulation
  govnomyy(sim,fiscalcol,t)      nominal government budget from first to last year of simulation
  
  rowgdp(bopcol,*)               BoP items as GDP share first and last year of simulation
  rowgdpyy(sim,bopcol,t)         BoP items as GDP share from first to last year of simulation

  macgdp(maccol,*)               real macro indicators as GDP share first and last year of simulation
  macgdpyy(sim,maccol,t)         real macro indicators as GDP share from first to last year of simulation
  
  macreal(maccol,*)              level of real macro indicators first and last year of simulation
  macrealyy(sim,maccol,t)        level of real macro indicators from first to last year of simulation
  macrealxp(sim,maccol,t)        percent dev wrt base of real macro indicators from first to last year of simulation
  macrealxptt(sim,maccol,t)      percent dev wrt base of real macro indicators for selected years

;


* macgrowh ==========================================================

macgrowth(igdp,tminrep) = gdpindic(igdp,'Real',tminrep,'base');
macgrowth(igdp,simcur) = gdpindicxpprep(igdp,'Real',simcur);

macgrowth('REXR',tminrep) = 1;
macgrowth('REXR',simcur) = REXRXPPREP(simcur);

macgrowth('Wage',tminrep) = 1;
macgrowth('Wage',simcur) = WFAVGXPPREP('tot-lab',simcur);

macgrowth('CapRet',tminrep) = 1;
macgrowth('CapRet',simcur) = SUM(fcap, WFAVGXPPREP(fcap,simcur));

macgrowth('UnempRat',tminrep) = 100*UERATX('tot-lab',tminrep,'base');
macgrowth('UnempRat',simcur) = 100*SUM(tmaxrep, UERATX('tot-lab',tmaxrep,simcur));



* macgrowhyy ========================================================

macgrowthyy(sim,maccol,tminrep)$simcur(sim) = macgrowth(maccol,tminrep);

macgrowthyy(simcur,igdp,t)$(tnmin(t) AND gdpindic(igdp,'Real',t-1,simcur)) =
  100*(gdpindic(igdp,'Real',t,simcur)/gdpindic(igdp,'Real',t-1,simcur)-1);

macgrowthyy(simcur,'REXR',t)$(tnmin(t) AND REXRX(t-1,simcur)) = 100*(REXRX(t,simcur)/REXRX(t-1,simcur) -1);

macgrowthyy(simcur,'Wage',t)$(tnmin(t) AND WFAVGX('tot-lab',t-1,simcur)) = 100*(WFAVGX('tot-lab',t,simcur)/WFAVGX('tot-lab',t-1,simcur) -1);

macgrowthyy(simcur,'CapRet',t)$(tnmin(t) AND SUM(fcapng, WFAVGX(fcapng,t-1,simcur))) =
  100*(SUM(fcapng, WFAVGX(fcapng,t,simcur)/WFAVGX(fcapng,t-1,simcur)) -1);

macgrowthyy(simcur,'UnempRat',t)$(tnmin(t) AND UERATX('tot-lab',t-1,simcur)) = 100*(UERATX('tot-lab',t,simcur)/UERATX('tot-lab',t-1,simcur) -1);
  




* govgdp ============================================================

govgdp(fiscalcol,tminrep) = fiscalindic(fiscalcol,'GDPshr',tminrep,'base');
govgdp(fiscalcol,simcur) = SUM(tmaxrep, fiscalindic(fiscalcol,'GDPshr',tmaxrep,simcur));





* govgdpyy ==========================================================

govgdpyy(simcur,fiscalcol,t) = fiscalindic(fiscalcol,'GDPshr',t,simcur);

* govnom ============================================================

govnom(fiscalcol,tminrep) = fiscalindic(fiscalcol,'Nominal',tminrep,'base');
govnom(fiscalcol,simcur) = SUM(tmaxrep, fiscalindic(fiscalcol,'Nominal',tmaxrep,simcur));

* govnomyy ==========================================================

govnomyy(simcur,fiscalcol,t) = fiscalindic(fiscalcol,'Nominal',t,simcur);



* rowgdp ============================================================

rowgdp(bopcol,tminrep) = bopindic(bopcol,'GDPshr',tminrep,'base');
rowgdp(bopcol,simcur) = SUM(tmaxrep, bopindic(bopcol,'GDPshr',tmaxrep,simcur));





* rowgdpyy ==========================================================

rowgdpyy(simcur,bopcol,t) = bopindic(bopcol,'GDPshr',t,simcur);
  



* macgdp ============================================================


macgdp(maccol,tminrep) = gdpexpindic(maccol,'GDPshr',tminrep,'base');
macgdp(maccol,simcur) = SUM(tmaxrep, gdpexpindic(maccol,'GDPshr',tmaxrep,simcur));




* macgdpyy ==========================================================

macgdpyy(simcur,maccol,t) = gdpexpindic(maccol,'GDPshr',t,simcur);
  


* macreal ===========================================================

macreal(igdp,tminrep) = gdpindic(igdp,'Real',tminrep,'base');          
macreal(igdp,simcur) = SUM(tmaxrep, gdpindic(igdp,'Real',tmaxrep,simcur));

* macrealyy =========================================================

macrealyy(simcur,igdp,t) = gdpindic(igdp,'Real',t,simcur);


* macrealxp =========================================================

macrealxp(simcur,igdp,t)$(simnbase(simcur) AND tsol(t)) = 1e-15 + gdpindicXP(igdp,'Real',t,simcur);
macrealxp(simcur,'REXR',t) = REXRXP(t,simcur);
macrealxp(simcur,'Wage',t)   = WFAVGXP('tot-lab',t,simcur);
macrealxp(simcur,'CapRet',t)  = SUM(fcap, WFAVGXP(fcap,t,simcur));
macrealxp(simcur,'UnempRat',t) = UERATXP('tot-lab',t,simcur);


* macrealxptt =======================================================

macrealxptt(simcur,maccol,t)$trep(t) = macrealxp(simcur,maccol,t);


* set growth rates to zero if static model
macgrowth(maccol,sim)$(dmod=0) = 0;
macgrowthyy(sim,maccol,t)$(dmod=0) = 0;


* save GDX file
$IF %NonIMv2%==1 EXECUTE_UNLOAD 'repmacro2-%app%.gdx',
$IF NOT %NonIMv2%==1 EXECUTE_UNLOAD 'repmacro2.gdx',
  macgrowth
  macgrowthyy

  govgdp
  govgdpyy

  govnom
  govnomyy

  rowgdp
  rowgdpyy

  macgdp
  macgdpyy
  
  macreal
  macrealyy
  macrealxp
  
  macrealxptt
;

DISPLAY
  macgrowth
  macgrowthyy

  govgdp
  govgdpyy

  rowgdp
  rowgdpyy

  macgdp
  macgdpyy
  
  macreal
  macrealyy
  macrealxp
 ;
 
 DISPLAY "#### END: repmacro.gms";