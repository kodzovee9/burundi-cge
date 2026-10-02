* repspec.gms

*START: time sets for reports
SET
 tgrwgdpbaseyy(t) years for annual GDP growth report for base
 ;
 tgrwgdpbaseyy(t)
  $(tsol(t) AND (ORD(t) LE SUM(tp$tmaxrep(tp), ORD(tp))))
  = YES;
DISPLAY tgrwgdpbaseyy;

SET
 tgrwyy(t) years for annual growth reports
 ;
 tgrwyy(t)
  $(tsol(t) AND (ORD(t) GT SUM(tp$tminrep(tp), ORD(tp))) AND (ORD(t) LE SUM(tp$tmaxrep(tp), ORD(tp))))
  = YES;
DISPLAY tgrwyy;

*START-HL-Data check=================================================

*popx(ac,t,sim)
*qfinsx(ins,ac,t,sim)
*simsam(ac,acp,t,sim) 

SET
 t25tmaxrep(t)
 ;
 t25tmaxrep('2025')  = YES;
 t25tmaxrep(tmaxrep) = YES;
 
PARAMETERS
 hhdincbase(ac,acp,t) base income of hhd ac from source acp in t (2025 or tmaxrep)
 hhdpopbase(ac,*)     base population of hhd ac in t (2025 or tmaxrep) 
;

 hhdincbase(h,acntp,t)$t25tmaxrep(t) = SUM(sim$simbase(sim), simsam(h,acntp,t,sim));
 hhdincbase(h,'total',t)        = SUM(acntp, hhdincbase(h,acntp,t));
 hhdincbase('total',acp,t)      = SUM(h, hhdincbase(h,acp,t));
 
 hhdpopbase(h,t)$t25tmaxrep(t) = SUM(sim$simbase(sim), popx(h,t,sim));
 hhdpopbase('total',t)    = SUM(h, hhdpopbase(h,t));

 hhdpopbase(ac,'ratio')$hhdpopbase(ac,'2025')
  = SUM(tmaxrep, hhdpopbase(ac,tmaxrep))/hhdpopbase(ac,'2025');

OPTION hhdincbase:3:1:1;
DISPLAY hhdincbase, hhdpopbase; 

PARAMETER
 hhdincpcbase(ac,acp,t)  base per-capita income of hhd ac from source acp in t (2025 or tmaxrep)
 ;
 hhdincpcbase(ac,acp,t)$hhdpopbase(ac,t) = hhdincbase(ac,acp,t)/hhdpopbase(ac,t);
 
OPTION hhdincpcbase:3:1:1;
DISPLAY hhdincpcbase; 


PARAMETER
 hhdincpcbaseratio(ac,acp)  base per-cap inc of hhd ac fr acp in tmaxrep & 2025 - ratio
 ;
 hhdincpcbaseratio(ac,acp)$hhdincpcbase(ac,acp,'2025')
 = SUM(tmaxrep, hhdincpcbase(ac,acp,tmaxrep))/hhdincpcbase(ac,acp,'2025');

DISPLAY hhdincpcbaseratio;

PARAMETER
 hhdlabpc(ac,*) household per capita labor endowment in t (2025 and tmaxrep)
 ;
 
 hhdlabpc(h,t)$(hhdpopbase(h,t) AND t25tmaxrep(t))
  = SUM((flab,simbase), QFINSX(h,flab,t,simbase))/hhdpopbase(h,t);
 
 hhdlabpc('total',t)$t25tmaxrep(t)
  = SUM((h,flab,simbase), QFINSX(h,flab,t,simbase))/hhdpopbase('total',t);
  
 hhdlabpc(ac,'ratio')$hhdlabpc(ac,'2025') 
  = SUM(tmaxrep, hhdlabpc(ac,tmaxrep))/hhdlabpc(ac,'2025');

DISPLAY hhdlabpc;

PARAMETER
 hhdincshrbase(ac,acp,t) share of income of hhd ac from source acp in t (2025 or tmaxrep)
 ;
 hhdincshrbase(ac,acntp,t)$(hhdincbase(ac,'total',t) AND t25tmaxrep(t))
  = 100*hhdincbase(ac,acntp,t)/hhdincbase(ac,'total',t);

 hhdincshrbase(ac,'total',t) = SUM(acntp, hhdincshrbase(ac,acntp,t));

OPTION hhdincshrbase:3:1:1;
DISPLAY hhdincshrbase;

PARAMETER
 hhdincshrbase2025(ac,acp) share of income of hhd ac from source acp in 2025
 ;
 hhdincshrbase2025(ac,acp) = hhdincshrbase(ac,acp,'2025');
DISPLAY hhdincshrbase2025;


PARAMETER
 hhdincshrbaseratio(ac,acp) ratio bt inc shares for of hhd ac fr acp in tmaxrep & 2025
 ;
 hhdincshrbaseratio(ac,acp)$hhdincshrbase(ac,acp,'2025')
  = SUM(tmaxrep, hhdincshrbase(ac,acp,tmaxrep))/hhdincshrbase(ac,acp,'2025');

DISPLAY hhdincshrbaseratio;

PARAMETER
 curaccdefgdp(sim,t) current account deficit (% of GDP)
 ;

 curaccdefgdp(simcur,t) = simsamgdp('cap-row','row',t,simcur)
 ;
DISPLAY curaccdefgdp;

*END-HL-Data check===================================================
*START-Parameter preparation=========================================
*This section includes parameters with more content than what is used
*in other reports (background, base, non-base, appendices)

SET 
 cssoc(ac) aggregate tax on factor use
 /cssoc-m-n, cssoc-m-p, cssoc-m-s, cssoc-m-t, 
  cssoc-f-n, cssoc-f-p, cssoc-f-s, cssoc-f-t/


PARAMETER
 tabgovbudlcu(*,ac,sim,t)  Government spending or receipt item ac by sim & year (minrep & maxrep) (LCU)
 ;
 tabgovbudlcu('spending','com',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = SUM(c, simsam(c,'gov',t,sim));
 
 tabgovbudlcu('spending','insdom',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = SUM(insd, simsam(insd,'gov',t,sim));

 tabgovbudlcu('spending','row',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = simsam('row','gov',t,sim);

 tabgovbudlcu('spending','inv-gov',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = simsam('inv-gov','cap-gov',t,sim);
 
 tabgovbudlcu('spending','total',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = SUM(acnt, tabgovbudlcu('spending',acnt,sim,t));

*===

 tabgovbudlcu('receipts',actax,sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = simsam('gov',actax,t,sim);

 tabgovbudlcu('receipts','cssoc',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = SUM(cssoc, simsam('gov',cssoc,t,sim));

 tabgovbudlcu('receipts',cssoc,sim,t) = 0;
   ;

 tabgovbudlcu('receipts','insdom',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = SUM(insd, simsam('gov',insd,t,sim));

 tabgovbudlcu('receipts','row',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = simsam('gov','row',t,sim);

 tabgovbudlcu('receipts','factor',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = SUM(f, simsam('gov',f,t,sim));

 tabgovbudlcu('receipts','cap-ngov',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = simsam('cap-gov','cap-ngov',t,sim);

 tabgovbudlcu('receipts','cap-row',sim,t)$(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = simsam('cap-gov','cap-row',t,sim);

 tabgovbudlcu('receipts','total',sim,t)	
  = SUM(acnt, tabgovbudlcu('receipts',acnt,sim,t));

DISPLAY tabgovbudlcu;


ALIAS(sim,simp);

PARAMETER
 tabgovbudshrgdp0(*,ac,sim,t)  Government spending or receipt item ac by sim in t (% of GDP)
 ;
 tabgovbudshrgdp0('spending',ac,sim,t)$GDPMPX(t,sim)
  = 10*tabgovbudlcu('spending',ac,sim,t)/GDPMPX(t,sim);

 tabgovbudshrgdp0('receipts',ac,sim,t)$GDPMPX(t,sim)
  = 10*tabgovbudlcu('receipts',ac,sim,t)/GDPMPX(t,sim);
	
DISPLAY tabgovbudshrgdp0;

PARAMETER
 tabgovbudshrgdp(*,ac,*)  Government spending or receipt item ac by sim in t (% of GDP)
 ;
 tabgovbudshrgdp('spending',ac,tminrep) = SUM(simbase, tabgovbudshrgdp0('spending',ac,simbase,tminrep));
 tabgovbudshrgdp('spending',ac,simcur)  = SUM(tmaxrep, tabgovbudshrgdp0('spending',ac,simcur,tmaxrep));

 tabgovbudshrgdp('receipts',ac,tminrep) = SUM(simbase, tabgovbudshrgdp0('receipts',ac,simbase,tminrep));
 tabgovbudshrgdp('receipts',ac,simcur)  = SUM(tmaxrep, tabgovbudshrgdp0('receipts',ac,simcur,tmaxrep));

DISPLAY tabgovbudshrgdp;

*=====

PARAMETER
 tablabshr(ac,sim,t) Shares of labor stock by labor category in 2025 and tmaxrep (%)
 ;
 tablabshr(f,sim,t)
  $(flab(f) AND (tminrep(t) OR tmaxrep(t)) AND SUM((ins,fp)$flab(fp), qfinsx(ins,fp,t,sim)))
  = 100*SUM(ins, qfinsx(ins,f,t,sim))
       /SUM((ins,fp)$flab(fp), qfinsx(ins,fp,t,sim));

 tablabshr('total',sim,t) = SUM(f, tablabshr(f,sim,t));

 tablabshr(f2,sim,t) = SUM(f3$mf3f2(f3,f2), tablabshr(f3,sim,t));

DISPLAY tablabshr;

*=====

PARAMETER
 tabuerate0(ac,sim,t) Unemployment rate by labor type and sim in 2025 and tmaxrep (%)
 ;
 tabuerate0(ac,sim,t)$(tminrep(t) OR tmaxrep(t))
  = 100*ueratx(ac,t,sim);
  
 tabuerate0('total',sim,t)   = tabuerate0('tot-lab',sim,t);
 tabuerate0('tot-lab',sim,t) = 0;

DISPLAY tabuerate0;

PARAMETER
 tabuerate(ac,*) Unemployment rate by labor type in 2025 and by sim in tmaxrep (%)
 ;
 tabuerate(ac,tminrep) = SUM(simbase, tabuerate0(ac,simbase,tminrep));
 tabuerate(ac,simcur)  = SUM(tmaxrep, tabuerate0(ac,simcur,tmaxrep));
DISPLAY tabuerate;

*=====
PARAMETER
 tabwfsim1(ac,sim,t) Wage by labor category - sim - year (wfavg);
 ;
*Wage = weighted average of actual wages, including WFDIST 
 tabwfsim1(f,sim,t)$(flab(f) AND simcur(sim)) =  1000*wfavgx(f,t,sim);
 tabwfsim1('total',sim,t)$simcur(sim)         =  1000*wfavgx('tot-lab',t,sim);
DISPLAY '1', tabwfsim1;

*==
PARAMETER
 tabwfsim2(ac,sim,t) Wage by labor category - sim - year (wf)
 ;

*Alt: wage = wfx, for total employment-weighted
 tabwfsim2(f,sim,t)$(flab(f) AND simcur(sim)) =  1000*wfx(f,t,sim);

 tabwfsim2('total',sim,t)$(simcur(sim) AND SUM(fp$flab(fp), qfx(fp,'total',t,sim)))
  = 1000
   *SUM(f$flab(f), wfx(f,t,sim)
    *qfx(f,'total',t,sim)
     /SUM(fp$flab(fp), qfx(fp,'total',t,sim)));
DISPLAY '2', tabwfsim2;

*=====

PARAMETER
 tabwfindexsim1(ac,*) Wage by labor type & sim in tmaxrep (2025=100) (wfavg)
 ;
 tabwfindexsim1(ac,sim)$SUM(tminrep, tabwfsim1(ac,sim,tminrep))
  = 100*SUM(tmaxrep, tabwfsim1(ac,sim,tmaxrep))
       /SUM(tminrep, tabwfsim1(ac,sim,tminrep));

 tabwfindexsim1(ac,tminrep)$SUM(simbase, tabwfsim1(ac,simbase,tminrep))
  = SUM(simbase, tabwfsim1(ac,simbase,tminrep));

DISPLAY tabwfindexsim1;

PARAMETER
 tabwfindexsim2(ac,sim) Wage by labor type & sim in tmaxrep (2025=100) (wf)
 ;
 tabwfindexsim2(ac,sim)$SUM(tminrep, tabwfsim2(ac,sim,tminrep))
  = 100*SUM(tmaxrep, tabwfsim2(ac,sim,tmaxrep))
       /SUM(tminrep, tabwfsim2(ac,sim,tminrep));
DISPLAY tabwfindexsim2;

*======
PARAMETER
 tabqhpcindex(ac,*) 'Household consumption per capita by RH and sim in tmaxrep (index 2025=100)'
;
 tabqhpcindex(ac,tminrep)
  =  100*SUM(simbase, qhpcx(ac,tminrep,simbase))
        /SUM(simbase, qhpcx('total',tminrep,simbase));

tabqhpcindex(ac,sim)$SUM((tminrep,simbase), qhpcx('total',tminrep,simbase))
  =  100*SUM(tmaxrep, qhpcx(ac,tmaxrep,sim))
        /SUM((tminrep,simbase), qhpcx('total',tminrep,simbase));
 
 tabqhpcindex('rural',t)   = 0;
 tabqhpcindex('urban',t)   = 0;
 tabqhpcindex('rural',sim) = 0;
 tabqhpcindex('urban',sim) = 0;
DISPLAY tabqhpcindex;

*======
PARAMETER
 tabpovrate0(ac,sim,t) 'Poverty rate by household & sim in 2025 and tmaxrep (%)'
 ;
 tabpovrate0(ac,sim,t)
  $(simcur(sim) AND (tminrep(t) OR tmaxrep(t)))
  = fgt0yy(sim,ac,t);
OPTION tabpovrate0:3:1:1;
DISPLAY tabpovrate0;

PARAMETER
 tabpovrategini(*,*) 'Poverty rate by household in 2025 and by sim in tmaxrep (%)'
 ;
 tabpovrategini(ac,tminrep) = SUM(simbase, fgt0yy(simbase,ac,tminrep));
 tabpovrategini(ac,simcur)  = SUM(tmaxrep, fgt0yy(simcur,ac,tmaxrep));
 tabpovrategini('gini2',tminrep) = SUM(simbase, gini('nation',tminrep,simbase)); 
 tabpovrategini('gini2',simcur)  = SUM(tmaxrep, gini('nation',tmaxrep,simcur));
DISPLAY tabpovrategini;

*END-Parameter preparation===========================================
*START-Sec 3-Econ Struc & App C=======================================

*START-Econ Struc================================
PARAMETER
 tabmacrosamgdp(ac2,ac2p)  Macro SAM for 2019 (% of GDP)
 ;
 tabmacrosamgdp(ac2,ac2p) = SUM((tmin,simbase), macrosam2gdp(ac2,ac2p,tmin,simbase));
DISPLAY tabmacrosamgdp;

PARAMETER
 figsectorstruc3(acrep,sectorcol)  Aggregate sectoral structure in base-year (%)
 figexpimpint3(acrep,sectorcol2)   Aggregate export and import intensity in base-year (%)
 figfaccoststruc3(acrep,acrepp)    Aggregate sectoral factor cost composition in base-year (%)
 figlabdemstruc3(acrep,acrepp)     Aggregate sectoral labor demand by level of education in base-year (%)
 figlabsupstruc3(acrep,acrepp)     Labor supply by level of education across aggregate sectors in 2019 (%)
 figdemstruc3(acrep,demcol)        Aggregate sectoral demand composition in base-year (%)
 ;
 figsectorstruc3(acrep,sectorcol1) = sectorstruc200(acrep,sectorcol1);
 figexpimpint3(acrep,sectorcol2)   = sectorstruc200(acrep,sectorcol2);
 figfaccoststruc3(acrep,acrepp)     = facdemstruc200(acrep,acrepp);
 
 figfaccoststruc3('f-lab-n',acrepp) = figfaccoststruc3('f-labm-n',acrepp) + figfaccoststruc3('f-labf-n',acrepp);
 figfaccoststruc3('f-lab-p',acrepp) = figfaccoststruc3('f-labm-p',acrepp) + figfaccoststruc3('f-labf-p',acrepp);
 figfaccoststruc3('f-lab-s',acrepp) = figfaccoststruc3('f-labm-s',acrepp) + figfaccoststruc3('f-labf-s',acrepp);
 figfaccoststruc3('f-lab-t',acrepp) = figfaccoststruc3('f-labm-t',acrepp) + figfaccoststruc3('f-labf-t',acrepp);
 
 figfaccoststruc3('f-labm-n',acrepp) = 0; 
 figfaccoststruc3('f-labf-n',acrepp) = 0;
 figfaccoststruc3('f-labm-p',acrepp) = 0;  
 figfaccoststruc3('f-labf-p',acrepp) = 0;
 figfaccoststruc3('f-labm-s',acrepp) = 0;  
 figfaccoststruc3('f-labf-s',acrepp) = 0;
 figfaccoststruc3('f-labm-t',acrepp) = 0;  
 figfaccoststruc3('f-labf-t',acrepp) = 0;

 figlabdemstruc3(acrep,acrepp) = labordemstruc200(acrep,acrepp);

 figlabdemstruc3('f-lab-n',acrepp) = figlabdemstruc3('f-labm-n',acrepp) + figlabdemstruc3('f-labf-n',acrepp);
 figlabdemstruc3('f-lab-p',acrepp) = figlabdemstruc3('f-labm-p',acrepp) + figlabdemstruc3('f-labf-p',acrepp);
 figlabdemstruc3('f-lab-s',acrepp) = figlabdemstruc3('f-labm-s',acrepp) + figlabdemstruc3('f-labf-s',acrepp);
 figlabdemstruc3('f-lab-t',acrepp) = figlabdemstruc3('f-labm-t',acrepp) + figlabdemstruc3('f-labf-t',acrepp);
 
 figlabdemstruc3('f-labm-n',acrepp) = 0; 
 figlabdemstruc3('f-labf-n',acrepp) = 0;
 figlabdemstruc3('f-labm-p',acrepp) = 0;  
 figlabdemstruc3('f-labf-p',acrepp) = 0;
 figlabdemstruc3('f-labm-s',acrepp) = 0;  
 figlabdemstruc3('f-labf-s',acrepp) = 0;
 figlabdemstruc3('f-labm-t',acrepp) = 0;  
 figlabdemstruc3('f-labf-t',acrepp) = 0;


*QFX(f,a,t,simcur)
*figlabsupstruc3(acrep,acrepp)     Labor supply by level of education across aggregate sectors in 2019 (%)

 figlabsupstruc3(acrep,acrepp)
  = SUM((tmin,simbase), 
*    SUM(ac$macacrep(ac,acrep), SUM(acp$macacrep(acp,acrepp), QFX(ac,acp,tmin,simbase))));
     SUM(flab$macacrep(flab,acrep), SUM(acp$macacrep(acp,acrepp), QFX(flab,acp,tmin,simbase))));
DISPLAY figlabsupstruc3;

 figlabsupstruc3('f-lab-n',acrepp) = 1*(figlabsupstruc3('f-labm-n',acrepp) + figlabsupstruc3('f-labf-n',acrepp));
 figlabsupstruc3('f-lab-p',acrepp) = 1*(figlabsupstruc3('f-labm-p',acrepp) + figlabsupstruc3('f-labf-p',acrepp));
 figlabsupstruc3('f-lab-s',acrepp) = 1*(figlabsupstruc3('f-labm-s',acrepp) + figlabsupstruc3('f-labf-s',acrepp));
 figlabsupstruc3('f-lab-t',acrepp) = 1*(figlabsupstruc3('f-labm-t',acrepp) + figlabsupstruc3('f-labf-t',acrepp));

 figlabsupstruc3('f-labm-n',acrepp) = 0; 
 figlabsupstruc3('f-labf-n',acrepp) = 0;
 figlabsupstruc3('f-labm-p',acrepp) = 0;  
 figlabsupstruc3('f-labf-p',acrepp) = 0;
 figlabsupstruc3('f-labm-s',acrepp) = 0;  
 figlabsupstruc3('f-labf-s',acrepp) = 0;
 figlabsupstruc3('f-labm-t',acrepp) = 0;  
 figlabsupstruc3('f-labf-t',acrepp) = 0;

 figlabsupstruc3('total3',acrepp) = SUM(acrep, figlabsupstruc3(acrep,acrepp));

DISPLAY figlabsupstruc3;

 figlabsupstruc3(acrep,acrepp)$figlabsupstruc3(acrep,'total3')
  = 100*figlabsupstruc3(acrep,acrepp)
       /figlabsupstruc3(acrep,'total3');
DISPLAY figlabsupstruc3;

 figdemstruc3(acrep,demcol)        = demstruc200(acrep,demcol);
DISPLAY figsectorstruc3, figexpimpint3, figfaccoststruc3, figlabdemstruc3, figlabsupstruc3, figdemstruc3;

*======
PARAMETER
 figpopbaseyr(ac) Population share by RH in 2019 (%)
 ;
 figpopbaseyr(h)
  = 100*SUM((tmin,simbase), popx(h,tmin,simbase))
       /SUM((hp,tmin,simbase), popx(hp,tmin,simbase));
DISPLAY figpopbaseyr;

*======
PARAMETER
 figqhpcindexbaseyr(ac) Consumption per capita in 2019 by RH (index average=100)
 ;
 figqhpcindexbaseyr(ac)
  = 100*SUM((tmin,simbase), qhpcx(ac,tmin,simbase))
        /SUM((tmin,simbase), qhpcx('total',tmin,simbase));
 figqhpcindexbaseyr('rural') = 0;
 figqhpcindexbaseyr('urban') = 0;
DISPLAY figqhpcindexbaseyr;

*======
PARAMETER
 figpovratebaseyr(ac) Poverty rate by household in 2019 (%)
 ;
*Poverty rate reported for base for tmin and tmaxrep for ac if data for tmin
  figpovratebaseyr(ac) = SUM((simbase,tmin), fgt0yy(simbase,ac,tmin));
DISPLAY figpovratebaseyr;

PARAMETER
 figginibaseyr(ac) National Gini coefficient in 2019
 ;
 figginibaseyr(ac) = SUM((tmin,simbase), gini(ac,tmin,simbase)); 
DISPLAY figginibaseyr;

*======
PARAMETER
 tabhhdincshrbaseyr(ac,acp) share of income of hhd ac from source acp in 2019
 tabhhdincshr2baseyr(acp,ac) share of income of hhd ac from source acp in 2019
 ;
 tabhhdincshrbaseyr(h,acnt)
  $SUM((acntp,t,sim)$(tmin(t) AND simbase(sim)), simsam(h,acntp,t,sim))
  = 100*SUM((t,sim)$(tmin(t) AND simbase(sim)), simsam(h,acnt,t,sim))
       /SUM((acntp,t,sim)$(tmin(t) AND simbase(sim)), simsam(h,acntp,t,sim));

 tabhhdincshrbaseyr('total',acnt)
  $SUM((h,acntp,t,sim)$(tmin(t) AND simbase(sim)), simsam(h,acntp,t,sim))
  = 100*SUM((h,t,sim)$(tmin(t) AND simbase(sim)), simsam(h,acnt,t,sim))
       /SUM((h,acntp,t,sim)$(tmin(t) AND simbase(sim)), simsam(h,acntp,t,sim));

 tabhhdincshrbaseyr(ac,'total') = SUM(acnt, tabhhdincshrbaseyr(ac,acnt)); 

 tabhhdincshr2baseyr(acp,ac) =  tabhhdincshrbaseyr(ac,acp);
 
 tabhhdincshr2baseyr(acp,ac) =  tabhhdincshrbaseyr(ac,acp);
 
 tabhhdincshr2baseyr(f2,ac) =  SUM(f3$mf3f2(f3,f2), tabhhdincshr2baseyr(f3,ac));
 
SET
 mf3gen(acp,ac) labor mapping from disaggregated acp to aggregate ac by gender
 /(f-labm-n, f-labm-p, f-labm-s, f-labm-t).f-labm
  (f-labf-n, f-labf-p, f-labf-s, f-labf-t).f-labf/
;

 tabhhdincshr2baseyr(acp,ac)$SUM(f3$mf3gen(f3,acp), 1)
   =  SUM(f3$mf3gen(f3,acp), tabhhdincshr2baseyr(f3,ac));  

 
OPTION tabhhdincshr2baseyr:3:1:1;
DISPLAY tabhhdincshr2baseyr;

PARAMETER
 fighhdincshr2baseyr(acp,ac) share of income of hhd ac from source acp in 2019;
 ;

 fighhdincshr2baseyr(acp,ac) = tabhhdincshr2baseyr(acp,ac);
 fighhdincshr2baseyr(f3,ac) = 0;
 fighhdincshr2baseyr(acp,ac)$SUM(f3$mf3gen(f3,acp), 1) = 0;
OPTION fighhdincshr2baseyr:3:1:1;
DISPLAY fighhdincshr2baseyr;

*END-Econ Struc==================================
*START-App B=====================================

PARAMETER
 labtab2019(ac,*) Labor stocks and unemployment rates - 2019
 ;
 labtab2019(flab,'stock')    = 10*SUM((ins,tmin,simbase), QFINSX(ins,flab,tmin,simbase));
 labtab2019('total','stock') = 10*SUM((ins,flab,tmin,simbase), QFINSX(ins,flab,tmin,simbase));
 
 labtab2019(flab,'uerat')    = 100*SUM((tmin,simbase), UERATX(flab,tmin,simbase));
 labtab2019('total','uerat') = 100*SUM((tmin,simbase), UERATX('tot-lab',tmin,simbase));
DISPLAY labtab2019;

PARAMETER
 elastab(ac,*) elasticity table
 ;
*elastab(c,'VA')        = SUM(a$sam(a,c), prodelas(a));
 elastab(c,'VA')        = SUM(a$(ABS(SMAX(ap, sam(ap,c)) - sam(a,c)) LE 1.0e-6), prodelas(a));
 elastab(c,'CET')       = tradelas(c,'sigma_x');
 elastab(c,'Armington') = tradelas(c,'sigma_q');
 elastab(c,'LES')       = SUM(h, leselas0(c,h))/CARD(h);
 DISPLAY elastab;
 
PARAMETER
 leselaspchng(c,h) percent change in LES elasticity c for hhd h due to Engel adjustment;
 ;
 leselaspchng(c,h) =  100*(leselas(c,h)/leselas0(c,h) - 1);
DISPLAY leselaspchng;

*END: HL-APPENDIX B==================================================


*END-App B=======================================
*START-App C=====================================

PARAMETER
 tabsectorstruc3(ac,sectorcol)     Sectoral structure in base-year (%)
 tabfaccoststruc3(ac,acp)           Sectoral factor cost composition in base-year (%)
 tablabdemstruc3(ac,acp)           Sectoral labor demand by labor type in base-year (%)
 tablabsupstruc3(ac,acp)           Labor supply by sector in 2019 (%)
 tabdemstruc3(ac,demcol)           Sectoral demand composition in base-year (%)
  ;
 tabsectorstruc3(ac,sectorcol1)     = sectorstruc00(ac,sectorcol1);
 tabsectorstruc3(ac,sectorcol2)     = sectorstruc00(ac,sectorcol2);
 tabfaccoststruc3(ac,acp)            = facdemstruc00(acp,ac);
 tablabdemstruc3(ac,acp)            = labordemstruc00(acp,ac);
 tablabsupstruc3(ac,acp)            = employsectorstruc00(acp,ac);
 tabdemstruc3(ac,demcol)            = demstruc00(ac,demcol);

*END-App C=======================================
*START-App D=======================================


*END-App D=======================================


*======
PARAMETER
 figgovbudgdp(ac2,ac2p)  

$ONTEXT 
PARAMETER
 figbopgdp(ac2,ac2p)  

 
PARAMETER
 figsavinvgdp(ac2,ac2p)  
$OFFTEXT

*END-Sec 3-Econ Struc & App C=========================================
*START-Sec4-Base & App D==============================================

*======
PARAMETER
 figgdpgrwbase(t) 'Annual growth for GDP at factor cost for base 2026-2040 (%)'
  ;
 figgdpgrwbase(t)$tgrwgdpbaseyy(t) = SUM(simbase, macgrowthyy(simbase,'gdpfc',t));
DISPLAY figgdpgrwbase;

*======

SET
 acmaccol(maccol) macro accounts in following table
 /
 Absorption, PrvCon, GovCon, FixInv, Exports, Imports, GDPFC
*PrvFixInv, GovFixInv, StockChange, GDPMP, 
*GDPMP_alt, NetIndTax, REXR, Wage, CapRet, UnempRat
 /

SET
 t25tmaxrepyy(t);
 t25tmaxrepyy(t)
  $(tsol(t) 
   AND 
   (ORD(t) GE SUM(tp$tminrep(tp), ORD(tp))) 
   AND (ORD(t) LE SUM(tp$tmaxrep(tp), ORD(tp)))) 
   = YES;

DISPLAY t25tmaxrepyy

SET
 t26tmaxrepyy(t)
 ;
 t26tmaxrepyy(t)
  $(tsol(t) 
   AND 
   (ORD(t) GT SUM(tp$tminrep(tp), ORD(tp))) 
   AND (ORD(t) LE SUM(tp$tmaxrep(tp), ORD(tp)))) 
   = YES;
DISPLAY t26tmaxrepyy;

PARAMETER
 figmacgrwbase(acmaccol)
 ;
 figmacgrwbase(acmaccol) = SUM(simbase, macgrowth(acmaccol,simbase));
DISPLAY figmacgrwbase;


PARAMETER
 figmacgrwbaseyy(acmaccol,t)
 ;
 figmacgrwbaseyy(acmaccol,t)
  $(t26tmaxrepyy(t) AND SUM((simbase,tp), macgrowthyy(simbase,acmaccol,tp)))
 = SUM(simbase, macgrowthyy(simbase,acmaccol,t)) + 1.0E-6; 
DISPLAY figmacgrwbaseyy;


*======
PARAMETER
 figexratebase(*,t) Official and parallel exchange rate for the base scenario;

*figexratebase('official',t)$t25tmaxrepyy(t) = rexrx(t,'base');
*figexratebase('premium',t)$t25tmaxrepyy(t)  = prexrx(t,'base');

 figexratebase('official',t)$t25tmaxrepyy(t) = rexrx(t,'base')/SUM(tminrep, rexrx(tminrep,'base'));
 figexratebase('parallel',t)$t25tmaxrepyy(t) = prexrx(t,'base')*rexrx(t,'base')/SUM(tminrep, rexrx(tminrep,'base'));
 figexratebase('premium',t)$t25tmaxrepyy(t)  = prexrx(t,'base');
DISPLAY figexratebase;

*======
PARAMETER
 figdebtgdpbase(*,t) Debt stocks for the base scenario (% of GDP);

 figdebtgdpbase(debtcol,t)$t25tmaxrepyy(t) = SUM(simbase, debtindic(debtcol,t,simbase));
DISPLAY figdebtgdpbase;

*======
PARAMETER
 figdebtbase(*,t) Debt stocks for the base scenario -- LCU or FCU (index 2025=100);

 figdebtbase('NonGov-ForDebt',t)$t25tmaxrepyy(t) 
  = 100*SUM(simbase, fdebtx('ngovz',t,simbase))
       /SUM((tminrep,simbase), fdebtx('ngovz',tminrep,simbase));

 figdebtbase('Gov-ForDebt',t)$t25tmaxrepyy(t) 
  = 100*SUM(simbase, fdebtx('govz',t,simbase))
       /SUM((tminrep,simbase), fdebtx('govz',tminrep,simbase));

 figdebtbase('Gov-DomDebt',t)$t25tmaxrepyy(t) 
  = 100*SUM(simbase, gdebtx(t,simbase))
       /SUM((tminrep,simbase), gdebtx(tminrep,simbase));

DISPLAY figdebtbase;

*======
PARAMETER
 figfdebtfexyybase(t) Foreign debt stock as % of current foreign exchange receipts;

 figfdebtfexyybase(t)$t25tmaxrepyy(t) 
  = 100*SUM(simbase, debtindic('NonGov-ForDebt-GDPshr',t,simbase) + debtindic('Gov-ForDebt-GDPshr',t,simbase))
    /SUM((insrow,simbase), 
	   SUM(c, simsamgdp(c,insrow,t,simbase)) 
 	 + SUM(f, simsamgdp(f,insrow,t,simbase)) 
 	 + SUM(insd, simsamgdp(insd,insrow,t,simbase))
	 );
DISPLAY figfdebtfexyybase;


*======
PARAMETER
 figgovbudbase(*,ac,t)  Government spending or receipt item ac in t (% of GDP)
 ;

 figgovbudbase('spending',ac,t)$(tminrep(t) OR tmaxrep(t))
  = SUM(sim$simbase(sim), tabgovbudshrgdp0('spending',ac,sim,t));

 figgovbudbase('receipts',ac,t)$(tminrep(t) OR tmaxrep(t))
  = SUM(sim$simbase(sim), tabgovbudshrgdp0('receipts',ac,sim,t));

DISPLAY figgovbudbase;

*======
PARAMETER
*figsecgrwbase(ac)     'Base: Average annual growth in value added by sector 2026-2040 (%)'
 figsecgrwbase(acrep)  'Base: Average annual growth in value added by aggregate sector 2026-2040 (%)'
  ;
*figsecgrwbase(ac) = SUM(simbase, qvagrowth(ac,simbase));
 figsecgrwbase(acrep) = SUM(simbase, qvagrowth2(acrep,simbase));

DISPLAY figsecgrwbase;

*======
PARAMETER
 figlabshrbase(ac,t) Base shares of labor stock by labor category in 2025 and tmaxrep (%)
 ;
 figlabshrbase(ac,t) = SUM(simbase, tablabshr(ac,simbase,t));
DISPLAY figlabshrbase;

*======
PARAMETER
 figueratebase(ac,t) 'Base: Unemployment rate by labor type in 2025 and tmaxrep (%)'
 ;
 figueratebase(ac,t) = SUM(sim$simbase(sim), tabuerate0(ac,sim,t));

*======
PARAMETER
 figwfindexbase(ac) 'Base: Wage in tmaxrep by labor category (index 2025=100)'
 ;
 figwfindexbase(ac) = SUM(sim$simbase(sim), tabwfindexsim1(ac,sim));
DISPLAY figwfindexbase;

*======
PARAMETER
 figqhpcindexbase(ac,*) 'Base: Household consumption per capita in tmaxrep by RH (index 2025=100)'
 ;
 figqhpcindexbase(ac,tminrep) = tabqhpcindex(ac,tminrep);
 figqhpcindexbase(ac,simbase) = tabqhpcindex(ac,simbase);
DISPLAY figqhpcindexbase;

*======
PARAMETER
 figpovratebase(ac,t) 'Base: Poverty rate by household in 2025 and tmaxrep (%)'
 ;
*Poverty rate reported for base for tminrep and tmaxrep for ac if data for tminrep
 figpovratebase(ac,t)$SUM((simbase,tminrep), tabpovrate0(ac,simbase,tminrep))
  = SUM(sim$simbase(sim), tabpovrate0(ac,sim,t));

PARAMETER
 figginibase(ac,t) 'Gini coefficient in 2025 and tmaxrep'
 ;
 figginibase(ac,tminrep) = SUM(simbase, gini(ac,tminrep,simbase)); 
 figginibase(ac,tmaxrep) = SUM(simbase, gini(ac,tmaxrep,simbase)); 
DISPLAY figginibase, gini;


*======
PARAMETER
 figpovbaseyy(ac,t) 'Base: Poverty rate by household 2025-2040 (%)'
 ;
 figpovbaseyy(ac,t)$t25tmaxrepyy(t) 
  = SUM(simbase, fgt0yy(simbase,ac,t));
DISPLAY figpovbaseyy;



*END-Sec4-Base & App D===============================================
*BASE-Sec5-Non-Base & App D==========================================
*!@

PARAMETER
 figmacgrw(acmaccol,sim) Macro indicator average annual growth 2026-2040 (%)
 ;
 figmacgrw(acmaccol,sim)$simcur(sim) 
  = macgrowth(acmaccol,sim);
DISPLAY figmacgrw;

PARAMETER
 figmacgrwdev(acmaccol,sim) Macro indicator average annual growth 2026-2040 (%pt deviation from base)
 ;
 figmacgrwdev(acmaccol,sim)$(simcur(sim) AND (NOT simbase(sim)) AND SUM(simbase, macgrowth(acmaccol,simbase))) 
  = macgrowth(acmaccol,sim) - SUM(simbase, macgrowth(acmaccol,simbase)) + 1.0E-6;
DISPLAY figmacgrwdev;

PARAMETER
 figmacgrwyydev(sim,acmaccol,t) y-y growth for macro indicator maccol for the period 2025-2040 (%pt dev from base)
 ;
 figmacgrwyydev(sim,acmaccol,t)
  $(simcur(sim) AND t25tmaxrepyy(t) AND (NOT simbase(sim)) AND SUM(simbase, macgrowthyy(simbase,acmaccol,t))) 
  = macgrowthyy(sim,acmaccol,t) - SUM(simbase, macgrowthyy(simbase,acmaccol,t)) + 1.0E-9;
OPTION figmacgrwyydev:3:1:1;
DISPLAY figmacgrwyydev;

*======================================
*gdpexpindicxp(maccol,kgdp,t,sim)           expanded gdp table (% deviation from base in t)

*Selected macro indicators - all simulations
PARAMETER
 figmacyyxp(sim,acmaccol,t) macro indicator maccol for the period 2025-2040 by year (%dev from base)
 ;
 figmacyyxp(sim,acmaccol,t)
  $(simcur(sim) AND t25tmaxrepyy(t) AND (NOT simbase(sim))) 
  = gdpexpindicxp(acmaccol,'real',t,sim) + 1.0E-9;
OPTION figmacyyxp:3:1:1;
DISPLAY figmacyyxp;

*Selected macro indicators - unif
PARAMETER
 figmacyyxpuni(acmaccol,t) unif - macro indicator maccol for the period 2025-2040 by year (%dev from base)
 ;
 figmacyyxpuni(acmaccol,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp(acmaccol,'real',tmaxrep,'uni')))
  = gdpexpindicxp(acmaccol,'real',t,'uni') + 1.0E-9;
OPTION figmacyyxpuni:3:1:1;
DISPLAY figmacyyxpuni;

*Selected macro indicators - hum
PARAMETER
 figmacyyxpuniinf(acmaccol,t) hum - macro indicator maccol for the period 2025-2040 by year (%dev from base)
 ;
 figmacyyxpuniinf(acmaccol,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp(acmaccol,'real',tmaxrep,'uni+inf')))
  = gdpexpindicxp(acmaccol,'real',t,'uni+inf') + 1.0E-9;
OPTION figmacyyxpuniinf:3:1:1;
DISPLAY figmacyyxpuniinf;

*Selected macro indicators - min
PARAMETER
 figmacyyxpuniinfhd(acmaccol,t) min - macro indicator maccol for the period 2025-2040 by year (%dev from base)
 ;
 figmacyyxpuniinfhd(acmaccol,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp(acmaccol,'real',tmaxrep,'uni+inf+hd')))
  = gdpexpindicxp(acmaccol,'real',t,'uni+inf+hd') + 1.0E-9;
OPTION figmacyyxpuniinfhd:3:1:1;
DISPLAY figmacyyxpuniinfhd;

*Selected macro indicators - inf-all
PARAMETER
 figmacyyxpcombi(acmaccol,t) inf-all - macro indicator maccol for the period 2025-2040 by year (%dev from base)
 ;
 figmacyyxpcombi(acmaccol,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp(acmaccol,'real',tmaxrep,'combi')))
  = gdpexpindicxp(acmaccol,'real',t,'combi') + 1.0E-9;
OPTION figmacyyxpcombi:3:1:1;
DISPLAY figmacyyxpcombi;

$ONTEXT
*Selected macro indicators - infra
PARAMETER
 figmacyyxpinftrg(acmaccol,t) infra - macro indicator maccol for the period 2025-2040 by year (%dev from base)
 ;
 figmacyyxpinftrg(acmaccol,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp(acmaccol,'real',tmaxrep,'inf')))
  = gdpexpindicxp(acmaccol,'real',t,'inf') + 1.0E-9;
OPTION figmacyyxpinftrg:3:1:1;
DISPLAY figmacyyxpinftrg;

*Selected macro indicators - combi
PARAMETER
 figmacyyxpcombi(acmaccol,t) combi - macro indicator maccol for the period 2025-2040 by year (%dev from base)
 ;
 figmacyyxpcombi(acmaccol,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp(acmaccol,'real',tmaxrep,'combi')))
  = gdpexpindicxp(acmaccol,'real',t,'combi') + 1.0E-9;
OPTION figmacyyxpcombi:3:1:1;
DISPLAY figmacyyxpcombi;
$OFFTEXT

*Absorption - all simulations
PARAMETER
 figmacyyxpabs(sim,t) absorption for the period 2025-2040 by sim and year (%dev from base)
 ;
 figmacyyxpabs(simcur,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp('absorption','real',tmaxrep,simcur)))
  = gdpexpindicxp('absorption','real',t,simcur) + 1.0E-9;
OPTION figmacyyxpabs:3:1:1;
DISPLAY figmacyyxpabs;

*PrvCon - all simulations
PARAMETER
 figmacyyxpprvcon(sim,t) prvcon for the period 2025-2040 by sim and year (%dev from base)
 ;
 figmacyyxpprvcon(simcur,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp('prvcon','real',tmaxrep,simcur)))
  = gdpexpindicxp('prvcon','real',t,simcur) + 1.0E-9;
OPTION figmacyyxpprvcon:3:1:1;
DISPLAY figmacyyxpprvcon;

*FixInv - all simulations
PARAMETER
 figmacyyxpfixinv(sim,t) fixinv for the period 2025-2040 by sim and year (%dev from base)
 ;
 figmacyyxpfixinv(simcur,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp('fixinv','real',tmaxrep,simcur)))
  = gdpexpindicxp('fixinv','real',t,simcur) + 1.0E-9;
OPTION figmacyyxpfixinv:3:1:1;
DISPLAY figmacyyxpfixinv;

*exports - all simulations
PARAMETER
 figmacyyxpexports(sim,t) exports for the period 2025-2040 by sim and year (%dev from base)
 ;
 figmacyyxpexports(simcur,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp('exports','real',tmaxrep,simcur)))
  = gdpexpindicxp('exports','real',t,simcur) + 1.0E-9;
OPTION figmacyyxpexports:3:1:1;
DISPLAY figmacyyxpexports;

*imports - all simulations
PARAMETER
 figmacyyxpimports(sim,t) imports for the period 2025-2040 by sim and year (%dev from base)
 ;
 figmacyyxpimports(simcur,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp('imports','real',tmaxrep,simcur)))
  = gdpexpindicxp('imports','real',t,simcur) + 1.0E-9;
OPTION figmacyyxpimports:3:1:1;
DISPLAY figmacyyxpimports;

*gdpfc - all simulations
PARAMETER
 figmacyyxpgdpfc(sim,t) gdpfc for the period 2025-2040 by sim and year (%dev from base)
 ;
 figmacyyxpgdpfc(simcur,t)
  $(t25tmaxrepyy(t) AND SUM(tmaxrep, gdpexpindicxp('gdpfc','real',tmaxrep,simcur)))
   = gdpexpindicxp('gdpfc','real',t,simcur) + 1.0E-9;
OPTION figmacyyxpgdpfc:3:1:1;
DISPLAY figmacyyxpgdpfc;


*======
PARAMETER
 figsecgrwdev(acrep,sim)  'Non-base: Average annual growth in value added by aggregate sector 2026-2040 (% pt deviation from base)'
  ;
 figsecgrwdev(acrep,sim)$(simcur(sim) AND (NOT simbase(sim))) 
  = qvagrowth2(acrep,sim) - SUM(simbase, qvagrowth2(acrep,simbase));
DISPLAY figsecgrwdev;

*========
PARAMETER
 figdebtgdpdev(debtcol,sim) Non-base: Debt stocks in tmaxrep (% of GDP deviation from base);

 figdebtgdpdev(debtcol,sim)$simcur(sim) 
  = SUM(tmaxrep, debtindic(debtcol,tmaxrep,sim)) - SUM((tmaxrep,simbase), debtindic(debtcol,tmaxrep,simbase));
DISPLAY figdebtgdpdev;

PARAMETER
 figdebtgdpyydev(debtcol,sim,t) Non-base: Debt stocks by simulation and year (% of GDP deviation from base);

 figdebtgdpyydev(debtcol,sim,t)
  $(simcur(sim) AND t25tmaxrepyy(t) AND (NOT simbase(sim)) AND SUM(simbase, debtindic(debtcol,t,simbase)))
  = debtindic(debtcol,t,sim) - SUM(simbase, debtindic(debtcol,t,simbase));
DISPLAY figdebtgdpyydev;

PARAMETER
 figfdebtfexyy(sim,t) Foreign debt stock as % of current FEX receipts (by sim and year)
 ;
 figfdebtfexyy(sim,t)$(simcur(sim) AND t25tmaxrepyy(t))
  = 100*(debtindic('NonGov-ForDebt-GDPshr',t,sim) + debtindic('Gov-ForDebt-GDPshr',t,sim))
    /SUM(insrow, 
	   SUM(c, simsamgdp(c,insrow,t,sim)) 
 	 + SUM(f, simsamgdp(f,insrow,t,sim)) 
 	 + SUM(insd, simsamgdp(insd,insrow,t,sim))
	 );
DISPLAY figfdebtfexyy;


*========
PARAMETER
 figpov(ac,*) 'Poverty rate by household in 2025 and by sim in tmaxrep (%)'
 ;
 figpov(ac,tminrep) = SUM(simbase, fgt0yy(simbase,ac,tminrep));
 figpov(ac,simcur)  = SUM(tmaxrep, fgt0yy(simcur,ac,tmaxrep));
DISPLAY figpov;

PARAMETER
 figpovdev(ac,sim) 'Non-base: Poverty rate deviation from base by household and sim in tmaxrep (%pt)'
 ;
 figpovdev(ac,sim)$(simcur(sim) AND (NOT simbase(sim)) AND SUM((simbase,tminrep), fgt0yy(simbase,ac,tminrep)))
  = SUM(tmaxrep, fgt0yy(sim,ac,tmaxrep)) - SUM((simbase,tmaxrep), fgt0yy(simbase,ac,tmaxrep)) + 1.0E-9;
DISPLAY figpovdev;

PARAMETER
 figpovyy(sim,t) 'National poverty rate by sim 2025-2040 (%)'
 ;
 figpovyy(sim,t)$(simcur(sim) AND t25tmaxrepyy(t)) = fgt0yy(sim,'total',t);
DISPLAY figpovyy;

PARAMETER
 figpovyydev(ac,sim,t) 'Non-base: Poverty rate deviation from base by household and sim 2025-2040 (%pt)'
 ;
 figpovyydev(ac,sim,t)$(simcur(sim) AND (NOT simbase(sim)) AND t25tmaxrepyy(t) AND SUM(simbase, fgt0yy(simbase,ac,t)))
  = fgt0yy(sim,ac,t) - SUM(simbase, fgt0yy(simbase,ac,t)) + 1.0E-9;
DISPLAY figpovyydev;

PARAMETER
 figexrate(*,*) Official and parallel exchange rate in 2025 and by scenario in tmaxrep;

*figexrate('official',tminrep) = SUM(simbase, rexrx(tminrep,simbase));
*figexrate('official',sim)$simcur(sim) = SUM(tmaxrep, rexrx(tmaxrep,sim));

 figexrate('official',tminrep)            = rexrx(tminrep,'base')            /rexrx(tminrep,'base');
 figexrate('official',sim)$simcur(sim)    = SUM(tmaxrep, rexrx(tmaxrep,sim))/SUM(tminrep, rexrx(tminrep,'base'));

 figexrate('premium',tminrep)             = SUM(simbase, prexrx(tminrep,simbase));
 figexrate('premium',sim)$simcur(sim)     = SUM(tmaxrep, prexrx(tmaxrep,sim));

 figexrate('parallel',tminrep)             
  = SUM(simbase, prexrx(tminrep,simbase)) * rexrx(tminrep,'base')/rexrx(tminrep,'base');
  
 figexrate('parallel',sim)$simcur(sim)     
  = SUM(tmaxrep, prexrx(tmaxrep,sim))     * SUM(tmaxrep, rexrx(tmaxrep,sim))/SUM(tminrep, rexrx(tminrep,'base'));
 
DISPLAY figexrate;




PARAMETER
 figexratedev(*,sim) Official and parallel exchange rate deviation from base by scenario in tmaxrep;

 figexratedev('official',sim)$(simcur(sim) AND (NOT simbase(sim)))
* = SUM(tmaxrep, rexrx(tmaxrep,sim)) - SUM((tmaxrep,simbase), rexrx(tmaxrep,simbase));
  = figexrate('official',sim) - SUM(simbase, figexrate('official',simbase));
  
 figexratedev('parallel',sim)$(simcur(sim) AND (NOT simbase(sim)))
* = SUM(tmaxrep, prexrx(tmaxrep,sim)) - SUM((tmaxrep,simbase), prexrx(tmaxrep,simbase));
  = figexrate('parallel',sim) - SUM(simbase, figexrate('parallel',simbase));

DISPLAY figexratedev;

PARAMETER
 figoffexrateyy(sim,t) Official exchange rate by scenario and year 2025-2040
 figparexrateyy(sim,t) Parallel exchange rate by scenario and year 2025-2040
 ;

 figoffexrateyy(sim,t)$(simcur(sim) AND t25tmaxrepyy(t)) 
   = rexrx(t,sim)/SUM(tminrep, rexrx(tminrep,'base'));

 figparexrateyy(sim,t)$(simcur(sim) AND t25tmaxrepyy(t)) 
  = prexrx(t,sim) * rexrx(t,sim)/SUM(tminrep, rexrx(tminrep,'base'));

OPTION figoffexrateyy:3:1:1, figparexrateyy:3:1:1;
DISPLAY figoffexrateyy, figparexrateyy;


PARAMETER
 figexrateyydev(*,sim,t) Official and parallel exchange rate deviation from base by scenario and year;

 figexrateyydev('official',sim,t)
  $(simcur(sim) AND (NOT simbase(sim)) AND t25tmaxrepyy(t))
   = figoffexrateyy(sim,t) - SUM(simbase, figoffexrateyy(simbase,t)) + 1.0E-9;
 
 figexrateyydev('parallel',sim,t)
  $(simcur(sim) AND (NOT simbase(sim)) AND t25tmaxrepyy(t))
  = figparexrateyy(sim,t) - SUM(simbase, figparexrateyy(simbase,t)) + 1.0E-9;  

OPTION figexrateyydev:3:1:1;  
DISPLAY figexrateyydev;


PARAMETER
 figgovbuddev(*,ac,sim) Government budget (% pt GDP dev fr base in tmaxrep)
 ;
 figgovbuddev('spending',ac,sim)$simcur(sim)
  = SUM(t$tmaxrep(t), tabgovbudshrgdp0('spending',ac,sim,t))
    - SUM((simbase,t)$tmaxrep(t), tabgovbudshrgdp0('spending',ac,simbase,t));
  
 figgovbuddev('receipts',ac,sim)$simcur(sim)
  = SUM(tmaxrep, tabgovbudshrgdp0('receipts',ac,sim,tmaxrep))
    - SUM((simbase,tmaxrep), tabgovbudshrgdp0('receipts',ac,simbase,tmaxrep));

DISPLAY figgovbuddev;

PARAMETER
 figgovbuddevagg(*,ac,sim) Aggregated government budget (% pt GDP dev fr base in tmaxrep)
 ;
*Spending: same as the above figgovbudddev
 figgovbuddevagg('spending',ac,sim)$simcur(sim)
  = SUM(t$tmaxrep(t), tabgovbudshrgdp0('spending',ac,sim,t))
    - SUM((simbase,t)$tmaxrep(t), tabgovbudshrgdp0('spending',ac,simbase,t));
 
*Receipts: same as the above figgovbudddev except for taxes 
 figgovbuddevagg('receipts',ac,sim)$simcur(sim)
  = SUM(tmaxrep, tabgovbudshrgdp0('receipts',ac,sim,tmaxrep))
    - SUM((simbase,tmaxrep), tabgovbudshrgdp0('receipts',ac,simbase,tmaxrep));

  figgovbuddevagg('receipts','tax',sim)$simcur(sim)
  = SUM((actax,tmaxrep), tabgovbudshrgdp0('receipts',actax,sim,tmaxrep)) 
    + SUM(tmaxrep, tabgovbudshrgdp0('receipts','cssoc',sim,tmaxrep))
    - SUM((actax,simbase,tmaxrep), tabgovbudshrgdp0('receipts',actax,simbase,tmaxrep))
    - SUM((simbase,tmaxrep), tabgovbudshrgdp0('receipts','cssoc',simbase,tmaxrep));
  
  figgovbuddevagg('receipts',actax,sim)   = 0;
  figgovbuddevagg('receipts','cssoc',sim) = 0;
  figgovbuddevagg('receipts',ac,simbase)  = 0;
  
DISPLAY figgovbuddevagg;

PARAMETER
 figqhpcindexdev(ac,sim) cons per capita for hhd ac in tmaxrep by sim (dev from base in tmaxrep - 2025=100)
  ;
 figqhpcindexdev(ac,sim)$simcur(sim)
  = tabqhpcindex(ac,sim) - SUM(simbase, tabqhpcindex(ac,simbase));
DISPLAY figqhpcindexdev;


PARAMETER
 figqhpcpchng(ac,sim) cons per capita for hhd ac in tmaxrep by sim (% change from base in tmaxrep)
  ;
 figqhpcpchng(ac,sim)
  $(simcur(sim) AND SUM(simbase, tabqhpcindex(ac,simbase)))
  = 100*(tabqhpcindex(ac,sim)/SUM(simbase, tabqhpcindex(ac,simbase)) - 1);
DISPLAY figqhpcpchng;

*=-=-=-=-=-=-=
* start: add report for household consumption adjusted with government provision of education and health

SET
  ceduhealth(c)
/
c-edu
c-health
/
;


PARAMETER
 qhghdpcx(ac,sim,t)   household per capita consumption adjusted with government provision of education and health (HD) (level)
 qhghdpcxp(ac,sim,t)   household per capita consumption adjusted with government provision of education and health (HD) (% deviation from base in t)
 shrpop(ac,sim,t)
;

 shrpop(h,simcur,t)$SUM(hp, popx(hp,t,simcur)) = popx(h,t,simcur)/SUM(hp, popx(hp,t,simcur));

 qhghdpcx(h,simcur,t)$(pop(h,t)*scaling('qlabsol')*scaling('qlabto1')) = 
  ( SUM(c, PQD00(c,h)*QHX(c,h,t,simcur)) + shrpop(h,simcur,t)*SUM(c$ceduhealth(c), SUM(insgov, PQD00(c,insgov))*QGX(c,t,simcur)) ) * scaling('samsol')*scaling('samto1')
  /
  (pop(h,t)*scaling('qlabsol')*scaling('qlabto1'));

 qhghdpcx('total',simcur,t)$(SUM(h, pop(h,t))*scaling('qlabsol')*scaling('qlabto1')) = 
  ( SUM((c,h), PQD00(c,h)*QHX(c,h,t,simcur)) + SUM(c$ceduhealth(c), SUM(insgov, PQD00(c,insgov))*QGX(c,t,simcur)) ) * scaling('samsol')*scaling('samto1')
  /
  (SUM(h, pop(h,t))*scaling('qlabsol')*scaling('qlabto1'));


 qhghdpcxp(ac,simcur,t)$qhghdpcx(ac,'base',t) = 100*(qhghdpcx(ac,simcur,t)/qhghdpcx(ac,'base',t) - 1);

DISPLAY qhghdpcx, qhghdpcxp, shrpoP;


* end: add report for household consumption adjusted with government provision of education and health
*=-=-=-=-=-=-=
PARAMETER
 figgini(ac,*) 'Gini coefficient in 2025 and by sim in tmaxrep'
 ;
 figgini(ac,tminrep)         = SUM(simbase, gini(ac,tminrep,simbase)); 
 figgini(ac,sim)$simcur(sim) = SUM(t$tmaxrep(t), gini(ac,t,sim)); 
DISPLAY figgini;

PARAMETER
 figginidev(ac,sim) 'Gini coefficient by sim in tmaxrep (deviation from base)'
 ;
 figginidev(ac,sim)$simcur(sim)
  = SUM(t$tmaxrep(t), gini(ac,t,sim))
     - SUM((simbase,t)$tmaxrep(t), gini(ac,t,simbase)); 
DISPLAY figginidev;

PARAMETER
 figueratedev(ac,sim) 'Unemployment rate by labor type & sim in tmaxrep (% pt dev from base in tmaxrep)'
 ;
 figueratedev(ac,sim)$(simcur(sim) AND SUM(t$tminrep(t), tabuerate0(ac,sim,t)))
  = SUM(t$tmaxrep(t), tabuerate0(ac,sim,t))
    - SUM((simbase,t)$tmaxrep(t), tabuerate0(ac,simbase,t));
DISPLAY figueratedev;

PARAMETER
 figwfindexdev(ac,sim) index for wage of ac by sim in tmaxrep (deviation from base in tmaxrep)
 ;
 figwfindexdev(ac,sim)
  $(tabwfindexsim1(ac,sim) AND SUM(simbase, tabwfindexsim1(ac,simbase)))
  = tabwfindexsim1(ac,sim) - SUM(simbase, tabwfindexsim1(ac,simbase));
DISPLAY figwfindexdev, tabwfindexsim1;

PARAMETER
 figlabordev(*,sim) index for wage and unempl rates for total labor force by sim in tmaxrep (deviation from base in tmaxrep)
;
 figlabordev('wage',sim)
  $(tabwfindexsim1('total',sim) AND SUM(simbase, tabwfindexsim1('total',simbase)))
  = tabwfindexsim1('total',sim) - SUM(simbase, tabwfindexsim1('total',simbase));

 figlabordev('uerate',sim)
  $(simcur(sim) AND SUM(t$tminrep(t), tabuerate0('total',sim,t)))
  = SUM(t$tmaxrep(t), tabuerate0('total',sim,t))
    - SUM((simbase,t)$tmaxrep(t), tabuerate0('total',simbase,t));
DISPLAY figlabordev;	

PARAMETER
 figlabor(*,sim) index for wage and unempl rates for total labor force by sim in tmaxrep (deviation from base in tmaxrep)
;
 figlabor('wage',sim)
  $(tabwfindexsim1('total',sim) AND SUM(simbase, tabwfindexsim1('total',simbase)))
  = tabwfindexsim1('total',sim)
*    - SUM(simbase, tabwfindexsim1('total',simbase))
    ;

 figlabor('uerate',sim)
  $(simcur(sim) AND SUM(t$tminrep(t), tabuerate0('total',sim,t)))
  = SUM(t$tmaxrep(t), tabuerate0('total',sim,t))
*    - SUM((simbase,t)$tmaxrep(t), tabuerate0('total',simbase,t))
   ;
DISPLAY figlabor;	


*END: HL-NON-BASE====================================================



*END-Sec5-Non-Base & App D===========================================
*START-Note on exchange rate simulations=============================

$ONTEXT
macgrowth(maccol,*)
macgrowthyy(sim,maccol,t)
macrealxp(sim,maccol,t)
macrealyy(-----------)


PARAMETER
 figpovratebaseyr(ac) Poverty rate by household in 2019 (%)
 ;
*Poverty rate reported for base for tmin and tmaxrep for ac if data for tmin
  figpovratebaseyr(ac) = SUM((simbase,tmin), fgt0yy(simbase,ac,tmin));
DISPLAY figpovratebaseyr;

PARAMETER
 figginibaseyr(ac) National Gini coefficient in 2019
 ;
 figginibaseyr(ac) = SUM((tmin,simbase), gini(ac,tmin,simbase)); 
DISPLAY figginibaseyr;

$OFFTEXT

SET
 simnote(sim) simulations for exchange rate note
 /
 base
  50p
  20p
  10p  
  uni  
 /
 
 tnote(t)     years for reports for exchange rate note
 /2025*2035/

 tnote2(t)     years for reports for exchange rate note
 /2026*2035/


 acmac(maccol) macro accounts in following table
 /
 Absorption, PrvCon, GovCon, FixInv, Exports, Imports, GDPFC
*PrvFixInv, GovFixInv, StockChange, GDPMP, 
*GDPMP_alt, NetIndTax, Wage, CapRet, UnempRat
*REXR
 /

 iexr  exchange rate indicators
 /exr-off, exr-par, exr-prem/
 ;
ALIAS (simnote,simnotep), (tnote,tnotep);


PARAMETERS
 noteexr(sim,iexr,t)            exchange rates: market - official - premium 
 notemacgrw(maccol,*)           avg annual growth for macro indicator maccol for the period 2026-2030 (%)
 notemacgrwyy(sim,maccol,t) y-y growth  for macro indicator maccol for the period 2026-2030 (%)
 notemacrealxp(maccol,sim,t)    deviation of macro indicator maccol from base by sim and t (%)
 noteqhpcrealxp(ac,sim,t)       deviation of per-capital hhd consumption from base by sim and t (%)
 notepov(sim,ac,t)  poverty by sim - hhd - year
 notegini(sim,t)    national gini by sim - year
 ;

*========== 
 noteexr(sim,'exr-off',tnote)$(simnote(sim) AND simcur(sim))  = 100*EXRX(tnote,sim)/SUM((tminrep,simbase), EXRX(tminrep,simbase));
 noteexr(sim,'exr-par',tnote)$(simnote(sim) AND simcur(sim))  = PREXRX(tnote,sim)*noteexr(sim,'exr-off',tnote);
 noteexr(sim,'exr-prem',tnote)$(simnote(sim) AND simcur(sim)) = 100*(PREXRX(tnote,sim) - 1); 

 notemacgrw(acmac,tminrep) 
  = macgrowth(acmac,tminrep);
 notemacgrw(acmac,sim)$(simnote(sim) AND simcur(sim))
  = macgrowth(acmac,sim);

 notemacgrwyy(sim,acmac,tnote)$(simnote(sim) AND simcur(sim))
  = macgrowthyy(sim,acmac,tnote);
 
 notemacrealxp(acmac,sim,tnote)
  $((simnote(sim) AND simcur(sim)) AND SUM((simnotep,tnotep), macrealxp(simnotep,acmac,tnotep)) AND (NOT simbase(sim)))
  = macrealxp(sim,acmac,tnote) + 1.0E-6;
 notemacrealxp('rexr',sim,tnote) = 0;
 
 noteqhpcrealxp(ac,sim,tnote)
  $((simnote(sim) AND simcur(sim)) AND SUM((simnotep,tnotep), qhpcrealxp(simnotep,ac,tnotep)) AND (NOT simbase(sim)))
  = qhpcrealxp(sim,ac,tnote) + 1.0E-6;
 noteqhpcrealxp('rural',sim,tnote) = 0;
 noteqhpcrealxp('urban',sim,tnote) = 0;
  
 notepov(sim,ac,tnote)$(simnote(sim) AND simcur(sim))
  = fgt0yy(sim,ac,tnote);
 
 notegini(sim,tnote)$(simnote(sim) AND simcur(sim))   
  = gini('nation',tnote,sim);

DISPLAY
 noteexr
 notemacgrw
 notemacgrwyy 
 notemacrealxp 
 noteqhpcrealxp
 notepov
 notegini
 ;

*============
PARAMETERS
 noteexrbase(iexr,t)            exchange rates: market - official - premium 
 notemacgrwbase(maccol)         avg annual base growth for macro indicator maccol for the period 2026-2030 (%)
 noteqhpcbase(ac,t)             hhd consumption per capita for base for te period 2025-2035 (index total in 2025 = 100)
 notemacgrwyybase(sim,maccol,t) y-y base growth  for macro indicator maccol for the period 2026-2030 (%)
 notepovbase(sim,ac,t)          poverty for base by sim - hhd - year (%)
 ;
 noteexrbase(iexr,tnote)               = SUM(simbase, noteexr(simbase,iexr,tnote));
 notemacgrwbase(acmac)                 = SUM(simbase, notemacgrw(acmac,simbase));
 notemacgrwyybase(simbase,acmac,tnote) = notemacgrwyy(simbase,acmac,tnote);
 
 noteqhpcbase(ac,tnote)$SUM((simbase,tminrep), qhpcrealyy(simbase,ac,tminrep))
 = 100*SUM(simbase, qhpcrealyy(simbase,ac,tnote))
      /SUM((simbase,tminrep), qhpcrealyy(simbase,ac,tminrep));
 
 noteqhpcbase('rural',tnote) = 0;
 noteqhpcbase('urban',tnote) = 0;
 
 notepovbase(simbase,ac,tnote)         = notepov(simbase,ac,tnote);

DISPLAY
 noteexrbase, notemacgrwbase, notemacgrwyybase, noteqhpcbase, notepovbase;

*============

PARAMETERS
 noteexrall(iexr,sim,t)        exchange rates: offical and parallel 

 noteexrdev(iexr,sim,t)        exchange rates: market - official - premium (deviation from base)
 notemacgrwdev(maccol,*)       avg annual growth for macro indicator maccol for the period 2026-2030 (%pt dev from base)
 notemacgrwyydev(sim,maccol,t) y-y growth  for macro indicator maccol for the period 2026-2030 (%pt dev from base)
 notepovdev(ac,sim,t)          poverty by sim - hhd - year (%pt dev from base)
 ;

 noteexrall(iexr,simnote,tnote) = noteexr(simnote,iexr,tnote);
 
 noteexrdev(iexr,simnote,tnote)$((NOT simbase(simnote)) AND SUM(simbase, noteexr(simbase,iexr,tnote)))
  = noteexr(simnote,iexr,tnote) - SUM(simbase, noteexr(simbase,iexr,tnote)) + 1.0E-6;
 
 notemacgrwdev(acmac,simnote)$((NOT simbase(simnote)) AND SUM(simbase, notemacgrw(acmac,simbase)))
  = notemacgrw(acmac,simnote) - SUM(simbase, notemacgrw(acmac,simbase)) + 1.0E-6;

 notemacgrwyydev(simnote,acmac,tnote)$(NOT simbase(simnote))
  = notemacgrwyy(simnote,acmac,tnote) - SUM(simbase, notemacgrwyy(simbase,acmac,tnote));
 
 notepovdev(ac,simnote,tnote)$((NOT simbase(simnote)) AND SUM(simbase, notepov(simbase,ac,tnote)))
  = notepov(simnote,ac,tnote) - SUM(simbase, notepov(simbase,ac,tnote)) + 1.0E-6;

DISPLAY noteexrdev, notemacgrwdev, notemacgrwyydev, notepovdev;


*END-Note on exchange rate simulations===============================
*START-HL-Background=================================================

SET
 tbase(t) base year 
 /2019/
 
 indfighhd
 / 
 popshr2
 incshr2
 consshr2
 incpc2
 conspc2
 povrat2
 trngovpc2
 gini2
 /
 ;

PARAMETER
 fighhdlivstd(ac,indfighhd) Disaggregated household data for 2019
 ;

 fighhdlivstd(h,'popshr2') 
  = 100*SUM((t,sim)$(tbase(t) AND simbase(sim)), popx(h,t,sim))
       /SUM((hp,t,sim)$(tbase(t) AND simbase(sim)), popx(hp,t,sim));

 fighhdlivstd(h,'incshr2') 
  = 100*SUM((acnt,t,sim)$(tbase(t) AND simbase(sim)), simsam(h,acnt,t,sim))
       /SUM((hp,acnt,t,sim)$(tbase(t) AND simbase(sim)), simsam(hp,acnt,t,sim));

 fighhdlivstd(h,'consshr2') 
  = 100*SUM((c,t,sim)$(tbase(t) AND simbase(sim)), simsam(c,h,t,sim))
       /SUM((c,hp,t,sim)$(tbase(t) AND simbase(sim)), simsam(c,hp,t,sim));

 fighhdlivstd(h,'incpc2') 
  =  SUM((acnt,t,sim)$(tbase(t) AND simbase(sim)), simsam(h,acnt,t,sim))
    /SUM((t,sim)$(tbase(t) AND simbase(sim)), popx(h,t,sim));
	   
 fighhdlivstd(h,'conspc2') 
  =  SUM((c,t,sim)$(tbase(t) AND simbase(sim)), simsam(c,h,t,sim))
    /SUM((t,sim)$(tbase(t) AND simbase(sim)), popx(h,t,sim));

fighhdlivstd(h,'trngovpc2') 
  =  SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam(h,'gov',t,sim))
    /SUM((t,sim)$(tbase(t) AND simbase(sim)), popx(h,t,sim));


*Computing totals
 fighhdlivstd('total','popshr2')  = SUM(h, fighhdlivstd(h,'popshr2'));
 fighhdlivstd('total','incshr2')  = SUM(h, fighhdlivstd(h,'incshr2'));  
 fighhdlivstd('total','consshr2') = SUM(h, fighhdlivstd(h,'consshr2'));

 fighhdlivstd('total','incpc2') 
  =  SUM((h,acnt,t,sim)$(tbase(t) AND simbase(sim)), simsam(h,acnt,t,sim))
    /SUM((h,t,sim)$(tbase(t) AND simbase(sim)), popx(h,t,sim));
	   
 fighhdlivstd('total','conspc2') 
  = SUM((c,h,t,sim)$(tbase(t) AND simbase(sim)), simsam(c,h,t,sim))
    /SUM((h,t,sim)$(tbase(t) AND simbase(sim)), popx(h,t,sim));

 fighhdlivstd('total','trngovpc2') 
  =  SUM((h,t,sim)$(tbase(t) AND simbase(sim)), simsam(h,'gov',t,sim))
    /SUM((h,t,sim)$(tbase(t) AND simbase(sim)), popx(h,t,sim));


*HL 20240408 -- this should be tbase, not tminrep
*Poverty
* fighhdlivstd(ac,'povrat2')$fighhdlivstd(ac,'popshr2') 
  fighhdlivstd(ac,'povrat2')$SUM((sim,t), fgt0yy(sim,ac,t))
  = SUM((sim,t)
     $(simbase(sim) AND tbase(t)), 1.0E-10 +
  	 fgt0yy(sim,ac,t));

DISPLAY fighhdlivstd;

SET
 maphjor(ac,acp);
 maphjor(ac,acp) = NO;
 
 fighhdlivstd(ac,'popshr2')$SUM(acp, maphjor(ac,acp))
  = 100*SUM((h,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), popx(h,t,sim))
       /SUM((hp,t,sim)$(tbase(t) AND simbase(sim)), popx(hp,t,sim));

 fighhdlivstd(ac,'incshr2')$SUM(acp, maphjor(ac,acp)) 
  = 100*SUM((h,acnt,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), simsam(h,acnt,t,sim))
       /SUM((hp,acnt,t,sim)$(tbase(t) AND simbase(sim)), simsam(hp,acnt,t,sim));

 fighhdlivstd(ac,'consshr2')$SUM(acp, maphjor(ac,acp))
  = 100*SUM((h,c,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), simsam(c,h,t,sim))
       /SUM((c,hp,t,sim)$(tbase(t) AND simbase(sim)), simsam(c,hp,t,sim));

 fighhdlivstd(ac,'incpc2')$SUM(acp, maphjor(ac,acp))
  =  SUM((h,acnt,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), simsam(h,acnt,t,sim))
    /SUM((h,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), popx(h,t,sim));
	   
 fighhdlivstd(ac,'conspc2')$SUM(acp, maphjor(ac,acp))
  =  SUM((c,h,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), simsam(c,h,t,sim))
    /SUM((h,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), popx(h,t,sim));

 fighhdlivstd(ac,'trngovpc2')$SUM(acp, maphjor(ac,acp))
  =  SUM((h,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), simsam(h,'gov',t,sim))
    /SUM((h,t,sim)$(maphjor(ac,h) AND tbase(t) AND simbase(sim)), popx(h,t,sim));

*Gini coefficient
*HL20240408 -- this should be 2019!
 fighhdlivstd(ac,'gini2') = SUM((tbase,simbase), gini(ac,tbase,simbase));


PARAMETERS
 hhdincbase(ac,acp,t) base income of hhd ac from source acp in t (2025 or tmaxrep)
 hhdpopbase(ac,*)     base population of hhd ac in t (2025 or tmaxrep) 
;

 hhdincbase(h,acntp,t)$t25tmaxrep(t) = SUM(sim$simbase(sim), simsam(h,acntp,t,sim));

PARAMETER
 fighhdincshr(ac,acp) share of income of hhd ac from source acp in 2019
 ;
 
 fighhdincshr(ac,'f-lab')    = SUM(flab, tabhhdincshrbaseyr(ac,flab));
 fighhdincshr(ac,'f-nonlab') 
  = tabhhdincshrbaseyr(ac,'f-capprv')   + tabhhdincshrbaseyr(ac,'f-land') + 
	tabhhdincshrbaseyr(ac,'f-nrmin') 
*	+ tabhhdincshrbaseyr(ac,'ent')
 ;
 fighhdincshr(ac,'gov') = tabhhdincshrbaseyr(ac,'gov');
 fighhdincshr(ac,'row') = tabhhdincshrbaseyr(ac,'row'); 
 
 fighhdincshr(ac,'total') = SUM(acnt, fighhdincshr(ac,acnt));

OPTION fighhdincshr:3:1:1;
DISPLAY fighhdincshr;

PARAMETER
 tabgovbudraw(ac,*)  Government budget for 2019 (raw - mn LCU)
 ;
 tabgovbudraw(ac,'receipts') = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam('gov',ac,t,sim));
 tabgovbudraw(ac,'spending') = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam(ac,'gov',t,sim));
DISPLAY tabgovbudraw;
 tabgovbudraw(ac,'receipts') = 0;
 tabgovbudraw(ac,'spending') = 0;
 tabgovbudraw(ac,'receipts') = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam('gov',ac,t,sim) + simsam('cap-gov',ac,t,sim));
 tabgovbudraw(ac,'spending') = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam(ac,'gov',t,sim) + simsam(ac,'cap-gov',t,sim));
DISPLAY tabgovbudraw;

PARAMETER
 tabgovbudspending(ac,*)  Government spending for 2019 (mn LCU and % of GDP)
 ;
 tabgovbudspending('com','lcu')      = SUM((c,t,sim)$(tbase(t) AND simbase(sim)), simsam(c,'gov',t,sim));
 tabgovbudspending('insdom','lcu')   = SUM((insd,t,sim)$(tbase(t) AND simbase(sim)), simsam(insd,'gov',t,sim));
 tabgovbudspending('row','lcu')      = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam('row','gov',t,sim));
 tabgovbudspending('inv-gov','lcu')  = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam('inv-gov','cap-gov',t,sim)); 
 tabgovbudspending('total','lcu')    = SUM(acnt, tabgovbudspending(acnt,'lcu'));
 
 tabgovbudspending(ac,'%GDP')         = 100*tabgovbudspending(ac,'lcu')/(10*GDPMP00);
 
DISPLAY tabgovbudspending; 

PARAMETER
 tabgovbudreceipts(ac,*)  Government receipts for 2019 (mn LCU and % of GDP)
 ;
 tabgovbudreceipts(actax,'lcu')     = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam('gov',actax,t,sim));
*tabgovbudreceipts('tax-dir','lcu') = tabgovbudreceipts('tax-dir','lcu') + tabgovbudreceipts('tax-fac','lcu');
*tabgovbudreceipts('tax-fac','lcu') = 0;
 
 tabgovbudreceipts('insdom','lcu') = SUM((insd,t,sim)$(tbase(t) AND simbase(sim)), simsam('gov',insd,t,sim));
 tabgovbudreceipts('row','lcu')    = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam('gov','row',t,sim));
 tabgovbudreceipts('factor','lcu') = SUM((f,t,sim)$(tbase(t) AND simbase(sim)), simsam('gov',f,t,sim));
 tabgovbudreceipts('cap-ngov','lcu') = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam('cap-gov','cap-ngov',t,sim));
 tabgovbudreceipts('cap-row','lcu')  = SUM((t,sim)$(tbase(t) AND simbase(sim)), simsam('cap-gov','cap-row',t,sim)); 
 tabgovbudreceipts('total','lcu')    = SUM(acnt, tabgovbudreceipts(acnt,'lcu'));
 
 tabgovbudreceipts(ac,'%GDP')         = 100*tabgovbudreceipts(ac,'lcu')/(10*GDPMP00);

DISPLAY tabgovbudreceipts; 


*$OFFTEXT

*END-HL-Background===================================================
*START-HL-BASE=======================================================


* start: report base-year data

PARAMETER
  figsectorstruc(ac,sectorcol)              'Figure X.X. Country: sectoral structure in base-year (%)'
  figsectorstruc2(acrep,sectorcol)          'Figure X.X. Country: sectoral structure in base-year (%)'

  figexpimpintensity(ac,sectorcol)          'Figure X.X. Country: export and import intensities in base-year (%)'
  figexpimpintensity2(acrep,sectorcol)      'Figure X.X. Country: export and import intensities in base-year (%)'

  figfaccoststruc(ac,ac)                     'Figure X.X. Country: sectoral factor cost composition in base-year (%)'
  figfaccoststruc2(acrep,acrep)              'Figure X.X. Country: sectoral factor cost composition in base-year (%)'

  figlabdemstruc(ac,ac)                     'Figure X.X. Country: sectoral labor demand by labor type in base-year (%)'
  figlabdemstruc2(acrep,acrep)              'Figure X.X. Country: sectoral labor demand by labor type in base-year (%)'

  figlabsupstruc(ac,acp)                    'Figure X.X. Country: labor supply by labor type across sectors in 2019 (%)'
  figlabsupstruc2(acrep,acrepp)             'Figure X.X. Country: labor supply by labor type across sectors in 2019 (%)'

  figdemstruc(ac,demcol)                    'Figure X.Y. Country: sectoral demand composition in base-year (%)'
  figdemstruc2(acrep,demcol)                'Figure X.Y. Country: sectoral demand composition in base-year (%)'
 
  figincomestruc(ins,ac)                    'Figure X.Y. Country: income sources by institution base-year (%)'
  figincomestruc2(acrep,acrep)                   'Figure X.Y. Country: income sources by institution base-year (%)'
  
;
 
       
 
figsectorstruc(ac,sectorcol1) = sectorstruc00(ac,sectorcol1);
figsectorstruc2(acrep,sectorcol1) = sectorstruc200(acrep,sectorcol1);

figexpimpintensity(ac,sectorcol2) = sectorstruc00(ac,sectorcol2);
figexpimpintensity2(acrep,sectorcol2) = sectorstruc200(acrep,sectorcol2);

figfaccoststruc(ac,acp) = facdemstruc00(acp,ac);
figfaccoststruc2(acrep,acrepp) = facdemstruc200(acrepp,acrep);   

figlabdemstruc(ac,acp) = labordemstruc00(acp,ac);
figlabdemstruc2(acrep,acrepp) = labordemstruc200(acrepp,acrep);

figlabsupstruc(ac,acp)            = employsectorstruc00(acp,ac);
figlabsupstruc2(acrep,acrepp)     =   employsectorstruc200(acrepp,acrep);

figdemstruc(ac,demcol) = demstruc00(ac,demcol);
figdemstruc2(acrep,demcol) = demstruc200(acrep,demcol);

figincomestruc(ins,ac)  = incomestruc00(ins,ac);   
figincomestruc2(acrepp,acrep) = incomestruc200(acrepp,acrep);              

     
* end: report base-year data




*START: HL-APPENDIX C================================================

SET 
 maccolsel2(maccol)
 /
 Absorption
 PrvCon     
 GovCon     
*FixInv     
 PrvFixInv  
 GovFixInv  
*StockChange
 Exports    
 Imports    
*GDPMP      
*NetIndTax  
 GDPFC      
*REXR       
*Wage       
*CapRet     
*UnempRat   
 /
 ;

PARAMETER
 tabgovbud2(*,ac,*)  Government spending or receipt item ac by sim in t (% of GDP)
 ;
 tabgovbud2('spending',ac,tminrep) 
  = SUM(simbase, tabgovbudshrgdp0('spending',ac,simbase,tminrep));

 tabgovbud2('spending',ac,sim)$simcur(sim) 
  = SUM(tmaxrep, tabgovbudshrgdp0('spending',ac,sim,tmaxrep));

 tabgovbud2('receipts',ac,tminrep) 
  = SUM(simbase, tabgovbudshrgdp0('receipts',ac,simbase,tminrep));

 tabgovbud2('receipts',ac,sim)$simcur(sim) 
  = SUM(tmaxrep, tabgovbudshrgdp0('receipts',ac,sim,tmaxrep));

DISPLAY tabgovbud2;

PARAMETER
 tabmacgrw(maccol,*) Annual growth in macroeconomic indicators
 ;
 tabmacgrw(maccolsel2,tminrep) = macgrowth(maccolsel2,tminrep);
 tabmacgrw(maccolsel2,simcur) = macgrowth(maccolsel2,simcur);
DISPLAY tabmacgrw;

*PARAMETER
* tabmacgrw(maccol,sim) Annual growth in macroeconomic indicators ;
* tabmacgrw(maccolsel2,simbase) = macgrowth(maccolsel2,simbase);
* tabmacgrw(maccolsel2,simcur)$(NOT tabmacgrw(maccolsel2,simcur))  
*  = macgrowth(maccolsel2,simcur) - SUM(simbase, macgrowth(maccolsel2,simbase));

PARAMETER
 tabqhpcindex2(ac,*) 'Household consumption per capita in 2025 (2019 LCU) by simulation in tmaxrep (index 2025=100)'
;

 tabqhpcindex2(ac,tminrep)
  =  SUM(simbase, qhpcx(ac,tminrep,simbase));

 tabqhpcindex2(ac,sim)$SUM(t$tminrep(t), qhpcx(ac,t,sim))
  =  100*SUM(t$tmaxrep(t), qhpcx(ac,t,sim))
        /SUM(t$tminrep(t), qhpcx(ac,t,sim));

DISPLAY tabqhpcindex2;


PARAMETER
 tabpovrate2(ac,*) 'Poverty rate by hhd in 2025 &  by household and simulation in tmaxrep (%)'
 ;
 tabpovrate2(ac,tminrep)
*$fighhdlivstd(ac,'popshr2') 
  = SUM(simbase, tabpovrate0(ac,simbase,tminrep))
*  + 1.0E-9
  ;
 
 tabpovrate2(ac,sim)
* $(fighhdlivstd(ac,'popshr2') AND simcur(sim))
  = SUM(tmaxrep, tabpovrate0(ac,sim,tmaxrep)) 
*    - SUM((simbase,tminrep), tabpovrate0(ac,simbase,tminrep)) 
*	+ 1.0E-9
	;
DISPLAY tabpovrate2;

*=-=-=

PARAMETER
 tabgini2(ac,*) 'Gini coefficient by hhd in 2025 &  by household and simulation in tmaxrep (%)'
 ;
 tabgini2(ac,tminrep)
* $fighhdlivstd(ac,'popshr2') 
  = SUM(simbase, gini(ac,tminrep,simbase));
 
 tabgini2(ac,sim)
* $(fighhdlivstd(ac,'popshr2') AND simcur(sim))
  = SUM(tmaxrep, gini(ac,tmaxrep,sim)); 
DISPLAY tabgini2;

*=-=-=

PARAMETER
 tabuerate2(ac,*) Unemployment rate by labor type in 2025 and by sim in tmaxrep (%)
 ;
 tabuerate2(ac,tminrep)         = SUM(simbase, tabuerate0(ac,simbase,tminrep));
 tabuerate2(ac,sim)$simcur(sim) = SUM(tmaxrep, tabuerate0(ac,sim,tmaxrep))
*                                 - SUM(tminrep, tabuerate2(ac,tminrep))
								  ; 
DISPLAY tabuerate2;

PARAMETER
 tabemplindex(ac,*) 'Employment by labor type in 2025 (thousands) & by labor type and simulation in tmaxrep (index 2025=100)';

 tabemplindex(f,tminrep)$flab(f)
  = SUM(simbase, qfx(f,'total',tminrep,simbase));

 tabemplindex('total',tminrep)
  = SUM((flab,simbase), qfx(flab,'total',tminrep,simbase));

 tabemplindex(f,sim)$(flab(f) AND simcur(sim)) 
  = 100*SUM(tmaxrep, qfx(f,'total',tmaxrep,sim))
   /SUM((tminrep,simbase), qfx(f,'total',tminrep,simbase));

 tabemplindex('total',sim)$simcur(sim) 
  = 100*SUM((flab,tmaxrep), qfx(flab,'total',tmaxrep,sim))
   /SUM((flab,tminrep,simbase), qfx(flab,'total',tminrep,simbase));
DISPLAY tabemplindex;


PARAMETER
 tabsectgrw(ac,*)  'Annual value-added growth by sector 2026-2040'
  ;
  
 tabsectgrw(ac,tminrep) = SUM(simbase, sectorstruc(ac,'VAshr',tminrep,simbase));
 tabsectgrw(ac,tminrep)$(ABS(tabsectgrw(ac,tminrep)) LT 1.0E-10) = 0;
 tabsectgrw(ac,simcur) = qvagrowth(ac,simcur);
* tabsectgrw(ac,simcur)$(NOT tabsectgrw(ac,simcur))
*   = qvagrowth(ac,simcur) - SUM(simbase, qvagrowth(ac,simbase)); 
DISPLAY tabsectgrw;

*END: HL-APPENDIX C==================================================
	
PARAMETER
  figgdpgrw0(t)               'Figure: Annual growth for GDP at factor cost for base 2020-2040 (%)'
  figmacreal0(t,maccol)       'Figure: selected macroeconomic indicators for base (LCU)'
  figdomfindemreal0(t,maccol) 'Figure: domestic final demands for base (LCU)'
  
  figmacgrowth0(maccol)  'Figure: real annual macroeconomic growth 2026-2040 (%)'
  figqhpc0(t,ac)         'Figure: real household consumption per capita and poverty rate'
  figpov0(t,ac)          'Figure: Poverty rate by RH and year 2019-2040 for base (%)'
  figsectgrw0(ac)        'Figure: real annual sector growth 2019-2040 (%)'
  figsectgrw20(acrep)    'Figure: real annual sector growth 2019-2040 (%)'
  
; 



SET maccolsel(maccol);


 figgdpgrw0(t)$tsol(t) = gdpgrw(t);


maccolsel(maccol) = NO;
maccolsel('Absorption') = YES;
maccolsel('Exports') = YES;
maccolsel('Imports') = YES;
maccolsel('GDPFC') = YES;
figmacreal0(t,maccol)$maccolsel(maccol) = macrealyy('base',maccol,t);

maccolsel(maccol) = NO;
maccolsel('PrvCon') = YES;
maccolsel('PrvFixInv') = YES;
maccolsel('GovCon') = YES;
maccolsel('GovFixInv') = YES;
figdomfindemreal0(t,maccol)$maccolsel(maccol) = macrealyy('base',maccol,t);


figmacgrowth0(maccol) = macgrowth(maccol,'base');

*figsectgrw0(ac) = actindicXPPREP('va','real',ac,'base');
 figsectgrw0(ac) =  SUM(simbase, qvagrowth(ac,simbase));

*figsectgrw20(acrep) = actindic2XPPREP('va','real',acrep,'base');

figqhpc0(t,ac) = QHPCX(ac,t,'base');
figpov0(t,ac) = fgt0yy('base',ac,t);





* end: report base scenario


* start: report results

PARAMETER
  figconprv(t,sim)      'Figure X.X. Country: private consumption (% deviation from base)'
  figinvprv(t,sim)      'Figure X.X. Country: private investment (% deviation from base)'
  figgdpfc(t,sim)       'Figure X.X. Country: GDP at factor cost (% deviation from base)'
  figqaXP(ac,t,sim)     'Figure X.X. Country: sectoral output (% deviation from base)'

  figmacrealXP(maccol,t,sim)  'Figure X.X. Country: macroindicators (% deviation from base)'
  figqhpcrealxp(ac,t,sim)     'Figure X.X. Country: real household consumption per capita (% deviation from base)'
  
  figqa2XP(acrep,t,sim)     'Figure X.X. Country: sectoral output (% deviation from base)'
  figpovXP(ac,t,sim)        'Figure X.X. Country: poverty rate (% deviation from base)'

  figpovXdev(ac,t,sim)
  
  figemployXP(ac,t,sim)    'Figure X.X. Country: empleoyment (% deviation from base)'
  figwageXP(ac,t,sim)
  
  figemitotXP(t,ac,sim)
;

figconprv(t,simcur)     = macrealXP(simcur,'PrvCon',t);
figinvprv(t,simcur)     = macrealXP(simcur,'PrvFixInv',t);
figgdpfc(t,simcur)      = macrealXP(simcur,'GDPFC',t);
*figqaXP(ac,t,simcur)$trep(t)    = actindicXP('Output','Real',ac,t,simcur);;

maccolsel(maccol) = NO;
*maccolsel('Absorption') = YES;
maccolsel('Exports') = YES;
maccolsel('Imports') = YES;
maccolsel('GDPFC') = YES;
maccolsel('PrvCon') = YES;
maccolsel('PrvFixInv') = YES;
*maccolsel('GovCon') = YES;
*maccolsel('GovFixInv') = YES;

figmacrealXP(maccolsel,t,simcur)$trep(t) = macrealXP(simcur,maccolsel,t);

figqhpcrealxp(ac,t,simcur)$trep(t) = qhpcrealxp(simcur,ac,t);

*figqa2XP(acrep,t,simcur)$trep(t) = actindic2XP('Output','Real',acrep,t,simcur);

*figpovXP(ac,t,simcur)$trep(t) = fgt0xp(simcur,ac,t);
figpovXdev(ac,t,simcur)$trep(t) = fgt0yy(simcur,ac,t) - fgt0yy('base',ac,t);


figemployXP(flab,t,simcur)$trep(t) = QFXP(flab,'total',t,simcur);

*figwageXP(flab,t,simcur)$trep(t) = wagesupxp(simcur,flab,t); 


*figemitotXP(t,ac,simcur) = emiindicXP('EmiVol',ac,'a-leche',t,simcur);
 
* end: report results
DISPLAY '2', notemacgrwbase;


* save GDX file
EXECUTE_UNLOAD 'repspec-%app%.gdx',

*START-Sec3-Econ Struc & AppC-Background
 tabmacrosamgdp
 figsectorstruc3
 figexpimpint3
 figfaccoststruc3
 figlabdemstruc3
 figlabsupstruc3
 figdemstruc3
 figpopbaseyr      
 figqhpcindexbaseyr
 figpovratebaseyr  
 figginibaseyr     
 fighhdincshr2baseyr
  
 tabhhdincshr2baseyr
 tabsectorstruc3    
 tabfaccoststruc3    
 tablabdemstruc3    
 tablabsupstruc3    
 tabdemstruc3

*START-Sec4-Base
 figgdpgrwbase
 figmacgrwbase
 figexratebase
 figfdebtfexyybase
 figdebtgdpbase
 figsecgrwbase
 figgovbudbase
 figlabshrbase
 figueratebase
 figwfindexbase
 figqhpcindexbase
 figpovratebase
 figginibase
 
 figmacgrwbaseyy
 figpovbaseyy

*END-Sec4-Base

*START-Sec5-Non-base
*figmacgrw      
 figmacgrwdev   
*figmacgrwyydev

*figmacyyxp
*figmacyyxpinftrg
*figmacyyxpcombi
 figmacyyxpabs
 figmacyyxpprvcon
 figmacyyxpfixinv
 figmacyyxpexports
 figmacyyxpimports
 figmacyyxpgdpfc

 figsecgrwdev
 figfdebtfexyy	
*figdebtgdpdev
*figdebtgdpyydev 
*figpov         
 figpovdev      
*figpovyy       
*figpovyydev    
*figexrate      
*figexratedev   
 figoffexrateyy    
 figparexrateyy    
*figexrateyydev 
 figgovbuddev
*figgovbuddevagg 
 figqhpcindexdev
 qhghdpcxp
*figqhpcpchng   
 figgini        
*figginidev 
 figlabor 
*figlabordev 
*figueratedev   
*figwfindexdev  
*END-Sec5-Non-base

*START-App C - Database
 labtab2019
 elastab 
*END-App C - Database
*START-App D - Simulation results
 figmacyyxpuni
 figmacyyxpuniinf
 figmacyyxpuniinfhd
 figmacyyxpcombi
 figmacyyxpuni      
 figmacyyxpuniinf   
 figmacyyxpuniinfhd 
 figmacyyxpcombi    
 tabgovbudshrgdp     
 tabmacgrw           
 tabqhpcindex           
 tabpovrategini           
 tabuerate           
 tabwfindexsim1
 tabemplindex         
 tabsectgrw           
*END-App D - Simulation results
;


$ONECHO > taskout.txt

*START: Sec3-Econ Struc
 par = tabmacrosamgdp	   rng = tabmacrosamgdp!b30     rdim=1 cdim=1
 par = figsectorstruc3	   rng = figsectorstruc3!b30    rdim=1 cdim=1
 par = figexpimpint3	   rng = figexpimpint3!b30      rdim=1 cdim=1
 par = figdemstruc3	       rng = figdemstruc3!b40       rdim=1 cdim=1
 par = figfaccoststruc3	   rng = figfaccoststruc3!b40    rdim=1 cdim=1
 par = figlabdemstruc3	   rng = figlabdemstruc3!b40    rdim=1 cdim=1 
 par = figlabsupstruc3	   rng = figlabsupstruc3!b40    rdim=1 cdim=1 
 par = figpopbaseyr        rng = fighhdbaseyr!b30       rdim=1 cdim=0
 par = figqhpcindexbaseyr  rng = fighhdbaseyr!b35       rdim=1 cdim=0
 par = figpovratebaseyr    rng = fighhdbaseyr!b40       rdim=1 cdim=0
 par = figginibaseyr       rng = fighhdbaseyr!b45       rdim=1 cdim=0
 par = fighhdincshr2baseyr rng = fighhdincshr2baseyr!b40 rdim=1 cdim=1 
*END: Sec3-Econ Struc

*START: Sec4-Base
  par = figgdpgrwbase	  rng = figgdpgrwbase!b30     rdim=0 cdim=1
  par = figmacgrwbase	  rng = figmacgrwbase!b30     rdim=1 cdim=0
  par = figexratebase	  rng = figexratebase!b30     rdim=1 cdim=1
  par = figexratebase	  rng = figexratebase!b30     rdim=1 cdim=1
  par = figfdebtfexyybase rng = figfdebtfexyybase!b30 rdim=0 cdim=1
  par = figdebtgdpbase	  rng = figdebtgdpbase!b30    rdim=1 cdim=1
  par = figgovbudbase	  rng = figgovbudbase!b40     rdim=2 cdim=1
  par = figsecgrwbase	  rng = figsecgrwbase!b30     rdim=1 cdim=0
  par = figlabshrbase	  rng = figlabshrbase!b30     rdim=1 cdim=1
  par = figueratebase     rng = figueratebase!b30     rdim=1 cdim=1
  par = figwfindexbase    rng = figwfindexbase!b30    rdim=1 cdim=0
  par = figqhpcindexbase  rng = fighhdbase!b30        rdim=1 cdim=1
  par = figpovratebase    rng = fighhdbase!b35        rdim=1 cdim=1
  par = figginibase       rng = fighhdbase!b40        rdim=1 cdim=1
*END: Sec4-Base

*START: Sec5-Non-Base
* par = figmacgrw          rng = figmacgrw!b30        rdim=1 cdim=1
  par = figmacgrwdev       rng = figmacgrwdev!b40     rdim=1 cdim=1
* par = figmacgrwyydev     rng = figmacgrwyydev!b80   rdim=2 cdim=1        
* par = figmacyyxp       rng = figmacyyxp!b30       rdim=2 cdim=1       
* par = figmacyyxpabs    rng = figmacyyxpindic!b30  rdim=1 cdim=1
* par = figmacyyxpprvcon  rng = figmacyyxpindic!b40  rdim=1 cdim=1
* par = figmacyyxpfixinv  rng = figmacyyxpindic!b40  rdim=1 cdim=1
* par = figmacyyxpexports rng = figmacyyxpindic!b60 rdim=1 cdim=1
* par = figmacyyxpimports rng = figmacyyxpindic!b70 rdim=1 cdim=1
* par = figmacyyxpgdpfc   rng = figmacyyxpindic!b80 rdim=1 cdim=1

* par = figexrate          rng = figexrate!b30        rdim=1 cdim=1  
* par = figexratedev       rng = figexratedev!b30     rdim=1 cdim=1  
  par = figoffexrateyy     rng = figexrateyy!b30      rdim=1 cdim=1  
  par = figparexrateyy     rng = figexrateyy!b50      rdim=1 cdim=1    
* par = figexrateyydev     rng = figexrateyydev!b30   rdim=2 cdim=1  
  par = figsecgrwdev       rng = figsecgrwdev!b50     rdim=1 cdim=1
  par = figfdebtfexyy      rng = figfdebtfexyy!b30    rdim=1 cdim=1
* par = figdebtgdpdev      rng = figdebtgdpdev!b30    rdim=1 cdim=1
* par = figdebtgdpyydev    rng = figdebtgdpyydev!b80  rdim=2 cdim=1
* par = figpovyydev        rng = figpovyydev!b30      rdim=2 cdim=1    
  par = figgovbuddev       rng = figgovbuddev!b50     rdim=2 cdim=1  
* par = figgovbuddevagg    rng = figgovbuddevagg!b50  rdim=2 cdim=1  
* par = figpov             rng = figpov!b40           rdim=1 cdim=1
  par = figpovdev          rng = figpovdev!b50        rdim=1 cdim=1  
* par = figpovyy           rng = figpovyy!b40         rdim=1 cdim=1  
  par = figqhpcindexdev    rng = figqhpcindexdev!b50  rdim=1 cdim=1  
  par = qhghdpcxp          rng = figqhpcindexdev!b60  rdim=2 cdim=1  
* par = figqhpcpchng       rng = figqhpcpchng!b30     rdim=1 cdim=1    
  par = figgini            rng = figgini!b30          rdim=1 cdim=1    
* par = figginidev         rng = figginidev!b30       rdim=1 cdim=1    
  par = figlabor           rng = figlabor!b30         rdim=1 cdim=1    
* par = figlabordev        rng = figlabordev!b30      rdim=1 cdim=1    
* par = figueratedev       rng = figueratedev!b50     rdim=1 cdim=1    
* par = figwfindexdev      rng = figwfindexdev!b50    rdim=1 cdim=1    
*END: Sec5-Non-Base

*START: Appendix B
  par = labtab2019       rng = labtab2019!b30        rdim=1 cdim=1
  par = elastab          rng = elastab!b30           rdim=1 cdim=1
*END: Appendix B

*START: Appendix C
 par = tabhhdincshr2baseyr rng = tabhhdincshr2baseyr!b40 rdim=1 cdim=1 
 par = tabsectorstruc3    rng = tabsectorstruc3!b40    rdim=1 cdim=1
 par = tabfaccoststruc3    rng = tabfaccoststruc3!b40    rdim=1 cdim=1
 par = tablabdemstruc3    rng = tablabdemstruc3!b40    rdim=1 cdim=1
 par = tablabsupstruc3    rng = tablabsupstruc3!b40    rdim=1 cdim=1
 par = tabdemstruc3       rng = tabdemstruc3!b40       rdim=1 cdim=1
*END: Appendix C

*START: Appendix D
  par = figmacyyxpuni      rng = figmacyyxpsim!b30    rdim=1 cdim=1
  par = figmacyyxpuniinf   rng = figmacyyxpsim!b40    rdim=1 cdim=1
  par = figmacyyxpuniinfhd rng = figmacyyxpsim!b50    rdim=1 cdim=1
  par = figmacyyxpcombi    rng = figmacyyxpsim!b60    rdim=1 cdim=1
  par = tabgovbudshrgdp    rng = tabgovbudshrgdp!b30  rdim=2 cdim=1    
  par = tabmacgrw          rng = tabmacgrw!b30        rdim=1 cdim=1    
  par = tabqhpcindex       rng = tabqhpcindex!b30     rdim=1 cdim=1    
  par = tabpovrategini     rng = tabpovrategini!b30   rdim=1 cdim=1    
  par = tabuerate          rng = tabuerate!b30        rdim=1 cdim=1    
  par = tabwfindexsim1     rng = tabwfindexsim1!b30   rdim=1 cdim=1    
  par = tabemplindex       rng = tabemplindex!b30     rdim=1 cdim=1    
  par = tabsectgrw         rng = tabsectgrw!b40       rdim=1 cdim=1    
*END: Appendix D

*  par = figpovbaseyy      rng = figpovbaseyy!b30      rdim=1 cdim=1
*  par = figmacgrwbaseyy      rng = figmacgrwbaseyy!b30 rdim=1 cdim=1


$OFFECHO            


EXECUTE "XLSTALK.exe -C repspec-%app%.xlsx";
EXECUTE "GDXXRW repspec-%app%.gdx o=repspec-%app%.xlsx @taskout.txt";
*EXECUTE "XLSTALK -O repspec-%app%.xlsx";



* include file to compute gini coefficient by household
*$INCLUDE calc-ginihh.inc

