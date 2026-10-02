* reploop.inc

DISPLAY "#### START: reploop.inc";

QXACX(a,c,t,sim) = QXAC.L(a,c,t);
PXACX(a,c,t,sim) = PXAC.L(a,c,t);

QEMIX(ghg,ac,acp,tcur(t),sim) = QEMI.L(ghg,ac,acp,t);
QEMIX(ghg,ac,'total',tcur(t),sim)     = SUM(acnt, QEMIX(ghg,ac,acnt,t,sim));
QEMIX(ghg,'total',acp,tcur(t),sim)    = SUM(ac, QEMIX(ghg,ac,acp,t,sim));
QEMIX('total',ac,acp,tcur(t),sim)     = SUM(acnt, QEMIX(acnt,ac,acp,t,sim));

PWEX(c,tcur(t),sim) = PWE.L(c,t);
PWMX(c,tcur(t),sim) = PWM.L(c,t);

PREXRX(t,sim) = PREXR.L(t);
YPREXRTX(c,tcur(t),sim) = YPREXRT.L(c,t);
YPREXRX(c,ac,tcur(t),sim) = YPREXR.L(c,ac,t);
PRQMBARX(c,tcur(t),sim) = PRQMBAR.L(c,t);
YPRQMBARTX(c,tcur(t),sim) = YPRQMBART.L(c,t);
YPRQMBARX(c,ac,tcur(t),sim) = YPRQMBAR.L(c,ac,t);

ABSNOMX(tcur(t),sim)               = ABSNOM.L(t);                            
CPIX(tcur(t),sim)                  = CPI.L(t);           
DKAX(f,a,tcur(t),sim)              = DKA.L(f,a,t);
DKINSX(ins2,f,tcur(t),sim)          = DKINS.L(ins2,f,t);
DPIX(tcur(t),sim)                  = DPI.L(t);           
EGX(tcur(t),sim)                   = EG.L(t);            
EHX(h,tcur(t),sim)                 = EH.L(h,t);         
EXRX(tcur(t),sim)                  = EXR.L(t);          
FBORX(ins2,tcur(t),sim)            = FBOR.L(ins2,t);     
FDEBTX(ins2,tcur(t),sim)           = FDEBT.L(ins2,t);  
FDISCALX(tcur(t),sim)              = FDISCAL.L(t);         
FPRDAX(f,a,tcur(t),sim)            = FPRDA.L(f,a,t);     
GBORX(tcur(t),sim)                 = GBOR.L(t);          
GDEBTX(tcur(t),sim)                = GDEBT.L(t);         
GDPMPX(tcur(t),sim)                = GDPMP.L(t);
GOVRECABSX(acgovrec,tcur(t),sim)   = GOVRECABS.L(acgovrec,t); 
GOVRECGDPX(acgovrec,tcur(t),sim)   = GOVRECGDP.L(acgovrec,t);     
GOVSPNDABSX(acgovspnd,tcur(t),sim)  = GOVSPNDABS.L(acgovspnd,t);      
GOVSPNDGDPX(acgovspnd,tcur(t),sim) = GOVSPNDGDP.L(acgovspnd,t);   
GPRIMDEFX(tcur(t),sim)             = GPRIMDEF.L(t);          
IADJX(ins2,tcur(t),sim)            = IADJ.L(ins2,t);
INVVALFX(tcur(t),sim)              = INVVALF.L(t);    
INVVALGX(tcur(t),sim)              = INVVALG.L(t);    
INVVALX(tcur(t),sim)               = INVVAL.L(t); 
ISCALX(fcap,tcur(t),sim)           = ISCAL.L(fcap,t);        
LABPARTRATX(tcur(t),sim)           = LABPARTRAT.L(t);
MPSADJX(tcur(t),sim)               = MPSADJ.L(t);  
MPSSCALX(tcur(t),sim)       = MPSSCAL.L(t);      
MPSX(insdng,tcur(t),sim)           = MPS.L(insdng,t);   
NDFGX(tcur(t),sim)                 = NDFG.L(t);           
NFFGX(tcur(t),sim)                 = NFFG.L(t);          
NFFINSSCALX(tcur(t),sim)           = NFFINSSCAL.L(t);    
NFFINSX(tcur(t),sim)           = NFFINS.L(t);    
NGOVPAYABSX(acngovpay,tcur(t),sim)  = NGOVPAYABS.L(acngovpay,t);      
NGOVPAYGDPX(acngovpay,tcur(t),sim) = NGOVPAYGDP.L(acngovpay,t);   
PAX(a,tcur(t),sim)                 = PA.L(a,t);         
PDDX(c,tcur(t),sim)                = PDD.L(c,t);         
PDSX(c,tcur(t),sim)                = PDS.L(c,t);         
PEX(c,tcur(t),sim)                 = PE.L(c,t);         
PKX(fcap,tcur(t),sim)              = PK.L(fcap,t);    
PMX(c,tcur(t),sim)                 = PM.L(c,t);         
PQDX(c,ac,tcur(t),sim)             = PQD.L(c,ac,t);
PQSX(c,tcur(t),sim)                = PQS.L(c,t); 
PVAX(a,tcur(t),sim)                = PVA.L(a,t);        
PXX(c,tcur(t),sim)                 = PX.L(c,t);         
QAX(a,tcur(t),sim)                 = QA.L(a,t);         
QDX(c,tcur(t),sim)                 = QD.L(c,t);         
QEX(c,tcur(t),sim)                 = QE.L(c,t);         
QFHENDSCALX(f,tcur(t),sim)         = QFHENDSCAL.L(f,t);   
QFHENDX(h,f,tcur(t),sim)           = QFHEND.L(h,f,t);     
QFINSSCALX(f,tcur(t),sim)          = QFINSSCAL.L(f,t);          
QFINSX(ins,ac,tcur(t),sim)         = QFINS.L(ins,ac,t);
QFSX(f,tcur(t),sim)                = QFS.L(f,t);        
QFX(f,a,tcur(t),sim)               = QF.L(f,a,t);       
QFX('tot-lab',a,tcur(t),sim)       = SUM(flab, QF.L(flab,a,t));
QFX(ac,'total',tcur(t),sim)        = SUM(a, QFX(ac,a,t,sim));
QGSCALX(tcur(t),sim)               = QGSCAL.L(t);         
QGX(c,tcur(t),sim)                 = QG.L(c,t);         
QHX(c,h,tcur(t),sim)               = QH.L(c,h,t);       
QHX('total',h,tcur(t),sim)         = SUM(c, PQD00(c,h)*QH.L(c,h,t));
QINTX(c,a,tcur(t),sim)             = QINT.L(c,a,t);     
QINVX(c,tcur(t),sim)               = QINV.L(c,t);  
QLABSCALX(tcur(t),sim)             = QLABSCAL.L(t);
QMX(c,tcur(t),sim)                 = QM.L(c,t);     
QNGOSCALX(insngo,tcur(t),sim)      = QNGOSCAL.L(insngo,t);
QNGOX(c,insngo,tcur(t),sim)        = QNGO.L(c,insngo,t);
QQX(c,tcur(t),sim)                 = QQ.L(c,t);         
QTRSTSCALX(tcur(t),sim)            = QTRSTSCAL.L(t);
QTRSTX(c,instrst,tcur(t),sim)      = QTRST.L(c,instrst,t);
QTX(c,tcur(t),sim)                 = QT.L(c,t);          
QXX(c,tcur(t),sim)                 = QX.L(c,t);         
RBTVATX(a,tcur(t),sim)             = RBTVAT.L(a,t);
REXRX(tcur(t),sim)                 = REXR.L(t);          
RGDPFCX(tcur(t),sim)               = RGDPFC.L(t);   
RGDPMPX(tcur(t),sim)               = RGDPMP.L(t);
RGPRIMDEFX(tcur(t),sim)            = RGPRIMDEF.L(t);
RNDFGX(tcur(t),sim)                = RNDFG.L(t);      
SAVFX(tcur(t),sim)                 = SAVF.L(t);         
SAVX(ins,tcur(t),sim)              = SAV.L(ins,t); 
SAVX('total',tcur(t),sim)          = SUM(insdng, SAV.L(insdng,t)); 
SHIFX(ins,f,tcur(t),sim)           = SHIF.L(ins,f,t);
SUBCSCALX(tcur(t),sim)             = SUBCSCAL.L(t);      
SUBCTX(tcur(t),sim)                = SUBCT.L(t);
SUBCX(c,ac,tcur(t),sim)            = SUBC.L(c,ac,t);     
TASCALX(tcur(t),sim)               = TASCAL.L(t); 
TAX(a,tcur(t),sim)                 = TA.L(a,t);
TESCALX(tcur(t),sim)               = TESCAL.L(t); 
TEX(c,tcur(t),sim)                 = TE.L(c,t);
TFASCALX(tcur(t),sim)              = TFASCAL.L(t);
TFAX(f,a,tcur(t),sim)              = TFA.L(f,a,t);
TFPSCALX(tcur(t),sim)              = TFPSCAL.L(t); 
TFPX(a,tcur(t),sim)                = TFP.L(a,t);  
TFSCALX(tcur(t),sim)               = TFSCAL.L(t); 
TFX(f,tcur(t),sim)                 = TF.L(f,t);
TMSCALX(tcur(t),sim)               = TMSCAL.L(t); 
TMX(c,tcur(t),sim)                 = TM.L(c,t);
TQSCALX(tcur(t),sim)               = TQSCAL.L(t); 
TQX(c,tcur(t),sim)                 = TQ.L(c,t);
TRDGDPX(tcur(t),sim)               = TRDGDP.L(t); 
TRIIX(ins,insp,tcur(t),sim)        = TRII.L(ins,insp,t);
TRNSFRSCALX(ac,tcur(t),sim)        = TRNSFRSCAL.L(ac,t); 
TRNSFRX(ac,ins,tcur(t),sim)        = TRNSFR.L(ac,ins,t);
TRSMRECX(tcur(t),sim)              = TRSMREC.L(t);
TVACSCALX(tcur(t),sim)             = TVACSCAL.L(t);        
TVACX(c,ac,tcur(t),sim)            = TVAC.L(c,ac,t);       
TYSCALX(tcur(t),sim)               = TYSCAL.L(t);
TYX(ins,tcur(t),sim)               = TY.L(ins,t);
UERATX('tot-lab',tcur(t),sim)      = SUM(f, UERAT.L(f,t)*QFS.L(f,t)/SUM(fp$flab(fp), QFS.L(fp,t)));
UERATX(f,tcur(t),sim)              = UERAT.L(f,t);
WALRASX(tcur(t),sim)               = WALRAS.L(t);
WFAVGX(f,tcur(t),sim)              = WFAVG.L(f,t);  
WFAX(f,a,tcur(t),sim)              = WFA.L(f,a,t);
WFAVGX('tot-lab',tcur(t),sim)      = SUM(f$flab(f), SUM(a, WF.L(f,t)*WFDIST.L(f,a,t)*QF.L(f,a,t))) / SUM(f$flab(f), SUM(a, QF.L(f,a,t)));
WFAVGX('tot-capng',tcur(t),sim)    = SUM(fcapng, SUM(a, WF.L(fcapng,t)*WFDIST.L(fcapng,a,t)*QF.L(fcapng,a,t))) / SUM(fcapng, SUM(a, QF.L(fcapng,a,t)));
WFDISTX(f,a,tcur(t),sim)           = WFDIST.L(f,a,t);   
WFX(f,tcur(t),sim)                 = WF.L(f,t);         
YFX(f,tcur(t),sim)                 = YF.L(f,t);         
YGX(tcur(t),sim)                   = YG.L(t);            
YIFX(ins,f,tcur(t),sim)            = YIF.L(ins,f,t);     
YIX(ins,tcur(t),sim)               = YI.L(ins,t);       
YTAXIMPX(tcur(t),sim)              = YTAXIMP.L(t);
YTAXEXPX(tcur(t),sim)              = YTAXEXP.L(t);
YTAXVATX(tcur(t),sim)              = YTAXVAT.L(t);
RBTVATXP(a,tcur(t),sim)            = RBTVAT.L(a,t);


trnsfrbX(ac,acp,tcur(t),sim) = trnsfrb(ac,acp,t);
trnsfrpcX(h,ins,tcur(t),sim) = TRNSFR.L(h,ins,t)/pop(h,t);

poptotX(tcur(t),sim)               = SUM(h, pop(h,t))*scaling('qlabsolrep');
popX(acpop,tcur(t),sim)            = pop(acpop,t)*scaling('qlabsolrep');
popX('total',tcur(t),sim)          = SUM(h, pop(h,t)*scaling('qlabsolrep'));
qgbX(c,tcur(t),sim)                = qgb(c,t);
rsavgX(tcur(t),sim)                = (YG.L(t) - EG.L(t))/CPI.L(t);
savgX(tcur(t),sim)                 = (YG.L(t) - EG.L(t));

mpkX(ac,fcap,tcur(t),sim) = mpk(ac,fcap,t);
                              
* qhpcX in LCU per capita                               
qhpcX(h,tcur(t),sim) = 
  SUM(c, PQD00(c,h)*QH.L(c,h,t)*scaling('samsol')*scaling('samto1'))/(pop(h,t)*scaling('qlabsol')*scaling('qlabto1'));

qhpcX('total',tcur(t),sim) = 
  SUM((c,h), PQD00(c,h)*QH.L(c,h,t)*scaling('samsol')*scaling('samto1'))/SUM(h, pop(h,t)*scaling('qlabsol')*scaling('qlabto1'));

*maphjor(ac,acp) mapping to aggreg Jordanian hhd ac from disaggreg hhd acp
* 
*qhpcX(ac,tcur(t),sim) = 
*  SUM((c,acp), PQD00(c,h)*QH.L(c,h,t)*scaling('samsol')*scaling('samto1'))/SUM(h, pop(h,t)*scaling('qlabsol')*scaling('qlabto1'));

qhpcX('rural',tcur(t),sim)$SUM(h$hrur(h), pop(h,t)) = 
  SUM((c,h)$hrur(h), PQD00(c,h)*QH.L(c,h,t)*scaling('samsol')*scaling('samto1'))/SUM(h$hrur(h), pop(h,t)*scaling('qlabsol')*scaling('qlabto1'));

qhpcX('urban',tcur(t),sim)$SUM(h$hurb(h), pop(h,t)) = 
  SUM((c,h)$hurb(h), PQD00(c,h)*QH.L(c,h,t)*scaling('samsol')*scaling('samto1'))/SUM(h$hurb(h), pop(h,t)*scaling('qlabsol')*scaling('qlabto1'));


qhpc2x(acrep,t,sim)$(SUM(h$macacrep(h,acrep), pop(h,t)*scaling('qlabsol')*scaling('qlabto1'))) = 
  SUM(h$macacrep(h,acrep), SUM(c, PQD00(c,h)*QH.L(c,h,t)*scaling('samsol')*scaling('samto1'))) / 
    (SUM(h$macacrep(h,acrep), pop(h,t)*scaling('qlabsol')*scaling('qlabto1')));


* yhpcX in LCU per capita                               
yhpcX(h,tcur(t),sim)               = 
  YI.L(h,t)*scaling('samsol')*scaling('samto1')/(pop(h,t)*scaling('qlabsol')*scaling('qlabto1'));
yhpcX('total',tcur(t),sim)         = 
  SUM(h, YI.L(h,t)*scaling('samsol')*scaling('samto1'))/SUM(h, pop(h,t)*scaling('qlabsol')*scaling('qlabto1'));


wfarealX(f,a,tcur(t),sim)          = wfax(f,a,t,sim) / CPI.L(t);
wfrealX(f,tcur(t),sim)             = WF.L(f,t)/CPI.L(t);
capgdp(fcap,tcur(t),sim)           = 100*PK.L(fcap,t)*SUM(ins, QFINS.L(ins,fcap,t))/GDPMP.L(t);

shvatrbtx(c,a,tcur(t),sim)         = shrbtvat(c,a,t);
  
  
qfinsbX(ins,ac,tcur(t),sim)  = qfinsb(ins,ac,t);

scaldelta_dsX(c,t,sim)      =scaldelta_ds(c,t);  
scaldelta_eX(c,t,sim)       =scaldelta_e(c,t);   
scalphi_xX(c,t,sim)         =scalphi_x(c,t);     



  
* status modelo
modsolstat('solvestat',tcur(t),sim) = GEM.SOLVESTAT;
modsolstat('modelstat',tcur(t),sim) = GEM.MODELSTAT;



*### start: gdpindic ==================================================

* Nominal

gdpindic('PrvCon','Nominal',tcur(t),sim)  =  SUM((c,h), PQD.L(c,h,t)*QH.L(c,h,t)) 
  + SUM((c,insngo), PQD.L(c,insngo,t)*QNGO.L(c,insngo,t));
gdpindic('FixInv','Nominal',tcur(t),sim)  =  SUM((ins2,fcap), PK.L(fcap,t)*DKINS.L(ins2,fcap,t));
gdpindic('PrvFixInv','Nominal',tcur(t),sim)  =  SUM((ins2,fcapng), PK.L(fcapng,t)*DKINS.L(ins2,fcapng,t));
gdpindic('GovFixInv','Nominal',tcur(t),sim)  =  SUM((ins2,fcapg), PK.L(fcapg,t)*DKINS.L(ins2,fcapg,t));
gdpindic('StockChange','Nominal',tcur(t),sim)  =  SUM((c,dstk), PQD.L(c,dstk,t)*SUM(ins2, qdstk(c,ins2,t)));
gdpindic('GovCon','Nominal',tcur(t),sim)  =  SUM((c,insgov), PQD.L(c,insgov,t)*QG.L(c,t));

*gdpindic('Exports','Nominal',tcur(t),sim) =  SUM(c, PWE.L(c,t)*QE.L(c,t)*( (1-shroe(c,t))*EXR.L(t)*PREXR.L(t) + shroe(c,t)*EXR.L(t) ))
*gdpindic('Exports','Nominal',tcur(t),sim) =  SUM(c, PWE.L(c,t)*QE.L(c,t)*EXR.L(t)*shroe(c,t)) + SUM(c, PWE.L(c,t)*QE.L(c,t)*EXR.L(t)*PREXR.L(t)*(1-shroe(c,t)))
gdpindic('Exports','Nominal',tcur(t),sim) =  SUM(c, PWE.L(c,t)*QE.L(c,t)*EXR.L(t))
  + SUM((c,instrst), PQD.L(c,instrst,t)*QTRST.L(c,instrst,t));
;

*gdpindic('Imports','Nominal',tcur(t),sim) = -SUM(c$cmoff(c), PWM.L(c,t)*EXR.L(t)*QM.L(c,t))
*  - SUM(c$(NOT cmoff(c)), PWM.L(c,t)*EXR.L(t)*PREXR.L(t)*QM.L(c,t));
gdpindic('Imports','Nominal',tcur(t),sim) = -SUM(c, PWM.L(c,t)*EXR.L(t)*QM.L(c,t));


gdpindic('GDPMP','Nominal',tcur(t),sim)   =  SUM(igdp2, gdpindic(igdp2,'Nominal',t,sim));
gdpindic('GDPFC','Nominal',tcur(t),sim)  =  SUM(a, PVA.L(a,t)*QA.L(a,t)) + SUM((f,a)$fleo(f), WFA.L(f,a,t)*QF.L(f,a,t));
gdpindic('NetIndTax','Nominal',tcur(t),sim) =
  + SUM(c, TQ.L(c,t)*PQS.L(c,t)*QQ.L(c,t))
  + YTAXIMP.L(t)
  + YTAXEXP.L(t)
  + SUM(a, TA.L(a,t)*PA.L(a,t)*QA.L(a,t))
  + SUM(c, YPRQMBART.L(c,t)) - SUM((c,a), YPRQMBAR.L(c,a,t))
  + SUM(c, YPREXRT.L(c,t))
  + YTAXVAT.L(t)
  - SUBCT.L(t);

gdpindic('GDPMP_alt','Nominal',tcur(t),sim)  =  gdpindic('GDPFC','Nominal',t,sim)
  + gdpindic('NetIndTax','Nominal',t,sim);

gdpindic('Absorption','Nominal',tcur(t),sim)  = gdpindic('GDPMP','Nominal',t,sim) - 
  gdpindic('Imports','Nominal',t,sim) - gdpindic('Exports','Nominal',t,sim);

* Real

gdpindic('PrvCon','Real',tcur(t),sim)  =  SUM((c,h), PQD00(c,h)*QH.L(c,h,t))
  + SUM((c,insngo), PQD00(c,insngo)*QNGO.L(c,insngo,t));
gdpindic('FixInv','Real',tcur(t),sim)  =  SUM((ins2,fcap), PK00(fcap)*DKINS.L(ins2,fcap,t));
gdpindic('PrvFixInv','Real',tcur(t),sim)  =  SUM((ins2,fcapng), PK00(fcapng)*DKINS.L(ins2,fcapng,t));
gdpindic('GovFixInv','Real',tcur(t),sim)  =  SUM((ins2,fcapg), PK00(fcapg)*DKINS.L(ins2,fcapg,t));
gdpindic('StockChange','Real',tcur(t),sim)  =  SUM((c,dstk), PQD00(c,dstk)*SUM(ins2,qdstk(c,ins2,t)));
gdpindic('GovCon','Real',tcur(t),sim)  =  SUM((c,insgov), PQD00(c,insgov)*QG.L(c,t));
gdpindic('Exports','Real',tcur(t),sim) =  SUM(c, pwe00(c)*EXR00*QE.L(c,t))
  + SUM((c,instrst), PQD00(c,instrst)*QTRST.L(c,instrst,t));
gdpindic('Imports','Real',tcur(t),sim) = -SUM(c, pwm00(c)*EXR00*QM.L(c,t));
gdpindic('GDPMP','Real',tcur(t),sim)   =  SUM(igdp2, gdpindic(igdp2,'Real',t,sim));

gdpindic('GDPFC','Real',tcur(t),sim) =  SUM(a, PVA00(a)*QA.L(a,t)) 
  + SUM((f,a)$fleo(f), WFA00(f,a)*QF.L(f,a,t));


gdpindic('NetIndTax','Real',tcur(t),sim) =
  + SUM(c, TQ.L(c,t)*PQS00(c)*QQ.L(c,t))
  + SUM(c, TM.L(c,t)*EXR00*PWM00(c)*QM.L(c,t))
  + SUM(c, TE.L(c,t)*EXR00*PWE00(c)*QE.L(c,t))
  + SUM(a, TA.L(a,t)*PA00(a)*QA.L(a,t))

  + SUM((c,a),         (1-SUBC.L(c,a,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,a,t)*QINT.L(c,a,t))
  + SUM((c,h),         (1-SUBC.L(c,h,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,h,t)*QH.L(c,h,t))
  + SUM((c,insngo),    (1-SUBC.L(c,insngo,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,insngo,t)*QNGO.L(c,insngo,t))
  + SUM((c,instrst),   (1-SUBC.L(c,instrst,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,instrst,t)*QTRST.L(c,instrst,t))  
  + SUM((c,insgov),    (1-SUBC.L(c,insgov,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,insgov,t)*QG.L(c,t))
  + SUM((c,fcap),      (1-SUBC.L(c,fcap,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,fcap,t)*capcomp(fcap,c)*SUM(ins2, DKINS.L(ins2,fcap,t))) 
  + SUM((c,dstk),      (1-SUBC.L(c,dstk,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,dstk,t)*SUM(ins2, qdstk(c,ins2,t)))
  + SUM((c,cp,tacm),   (1-SUBC.L(c,tacm,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,tacm,t)*icm(c,cp)*QM.L(cp,t))
  + SUM((c,cp,tace),   (1-SUBC.L(c,tace,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,tace,t)*ice(c,cp)*QE.L(cp,t))
  + SUM((c,cp,tacd),   (1-SUBC.L(c,tacd,t))*PQS00(c)*(1+TQ.L(c,t))*TVAC.L(c,tacd,t)*icd(c,cp)*QD.L(cp,t))
  
  - SUM((c,a),         SUBC.L(c,a,t)*PQS00(c)*(1+TQ.L(c,t))*QINT.L(c,a,t))
  - SUM((c,h),         SUBC.L(c,h,t)*PQS00(c)*(1+TQ.L(c,t))*QH.L(c,h,t))
  - SUM((c,insngo),    SUBC.L(c,insngo,t)*PQS00(c)*(1+TQ.L(c,t))*QNGO.L(c,insngo,t))
  - SUM((c,instrst),   SUBC.L(c,instrst,t)*PQS00(c)*(1+TQ.L(c,t))*QTRST.L(c,instrst,t))  
  - SUM((c,insgov),    SUBC.L(c,insgov,t)*PQS00(c)*(1+TQ.L(c,t))*QG.L(c,t))
  - SUM((c,fcap),      SUBC.L(c,fcap,t)*PQS00(c)*(1+TQ.L(c,t))*capcomp(fcap,c)*SUM(ins2, DKINS.L(ins2,fcap,t))) 
  - SUM((c,dstk),      SUBC.L(c,dstk,t)*PQS00(c)*(1+TQ.L(c,t))*SUM(ins2, qdstk(c,ins2,t)))
  - SUM((c,cp,tacm),   SUBC.L(c,tacm,t)*PQS00(c)*(1+TQ.L(c,t))*icm(c,cp)*QM.L(cp,t))
  - SUM((c,cp,tace),   SUBC.L(c,tace,t)*PQS00(c)*(1+TQ.L(c,t))*ice(c,cp)*QE.L(cp,t))
  - SUM((c,cp,tacd),   SUBC.L(c,tacd,t)*PQS00(c)*(1+TQ.L(c,t))*icd(c,cp)*QD.L(cp,t));


gdpindic('Absorption','Real',tcur(t),sim)  = gdpindic('GDPMP','Real',t,sim) - 
  gdpindic('Imports','Real',t,sim) - gdpindic('Exports','Real',t,sim);

* re-scale
gdpindic(igdp,'Nominal',tcur(t),sim) = gdpindic(igdp,'Nominal',t,sim)*scaling('samsolrep');
gdpindic(igdp,'Real',tcur(t),sim) = gdpindic(igdp,'Real',t,sim)*scaling('samsolrep');


* Nominal shares

gdpindic(igdp,'GDPshr',tcur(t),sim) = 100*gdpindic(igdp,'Nominal',t,sim) /
  gdpindic('GDPMP','Nominal',t,sim);

* Real shares

gdpindic(igdp,'GDPshr-Real',tcur(t),sim) = 100*gdpindic(igdp,'Real',t,sim) /
  gdpindic('GDPMP','Real',t,sim);

* change sign of imports  
gdpindic('Imports',kgdp,tcur(t),sim) = -gdpindic('Imports',kgdp,t,sim);  
  
DISPLAY gdpindic;

*### end: gdpindic ====================================================


* start: gdpexpindic

gdpexpindic('SavFor','Nominal',tcur(t),sim) = SAVF.L(t)*EXR.L(t);
gdpexpindic('SavGov','Nominal',tcur(t),sim) = savgX(t,sim);
gdpexpindic('SavIns','Nominal',tcur(t),sim) = SUM(insdng, SAV.L(insdng,t));

gdpexpindic('ForDebtGov','Nominal',tcur(t),sim) = FDEBT.L('govz',t)*EXR.L(t);
gdpexpindic('ForDebtPrv','Nominal',tcur(t),sim) = FDEBT.L('ngovz',t)*EXR.L(t);
gdpexpindic('DomDebtGov','Nominal',tcur(t),sim) = GDEBT.L(t);

* re-scale
gdpexpindic(maccol,'Nominal',tcur(t),sim) = gdpexpindic(maccol,'Nominal',t,sim)*scaling('samsolrep');

* note elements in igdp are already scaled
gdpexpindic(igdp,kgdp,tcur(t),sim) = gdpindic(igdp,kgdp,t,sim);



* Nominal shares

gdpexpindic(maccol,'GDPshr',tcur(t),sim) = 100*gdpexpindic(maccol,'Nominal',t,sim) /
  gdpexpindic('GDPMP','Nominal',t,sim);





* end: gdpexpindic


*### start: SIMSAM ==================================================

* filas tax
SIMSAM(taxdir,insdng,tcur(t),sim) = TY.L(insdng,t) * YI.L(insdng,t);

SIMSAM(taximp,c,tcur(t),sim) = TM.L(c,t)*PWM.L(c,t)*QM.L(c,t) * ( (1-shrom(c,t))*EXR.L(t)*PREXR.L(t) + shrom(c,t)*EXR.L(t) );
*SIMSAM(taximp,c,tcur(t),sim)$cmoff(c)      = TM.L(c,t)*EXR.L(t)*PWM.L(c,t)*QM.L(c,t);
*SIMSAM(taximp,c,tcur(t),sim)$(NOT cmoff(c))      = TM.L(c,t)*EXR.L(t)*PREXR.L(t)*PWM.L(c,t)*QM.L(c,t);

SIMSAM(taxexp,c,tcur(t),sim)      = 
*  TE.L(c,t)*EXR.L(t)*PWE.L(c,t)*QE.L(c,t);
  TE.L(c,t)*PWE.L(c,t)*QE.L(c,t) * ( (1-shroe(c,t))*EXR.L(t)*PREXR.L(t) + shroe(c,t)*EXR.L(t) );

SIMSAM(taxcom,c,tcur(t),sim)      = TQ.L(c,t)*(PDD.L(c,t)*QD.L(c,t)+PM.L(c,t)*QM.L(c,t));
SIMSAM(taxact,a,tcur(t),sim)      = TA.L(a,t)*PA.L(a,t)*QA.L(a,t);
SIMSAM(taxfac,f,tcur(t),sim)      = TF.L(f,t)*YF.L(f,t);
SIMSAM(taxvatc,c,tcur(t),sim) = 
    + SUM(a,         (1-SUBC.L(c,a,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,a,t)*QINT.L(c,a,t))
    + SUM(h,         (1-SUBC.L(c,h,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,h,t)*QH.L(c,h,t))
    + SUM(insngo,    (1-SUBC.L(c,insngo,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,insngo,t)*QNGO.L(c,insngo,t))
    + SUM(instrst,    (1-SUBC.L(c,instrst,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,instrst,t)*QTRST.L(c,instrst,t))
    + SUM(insgov,    (1-SUBC.L(c,insgov,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,insgov,t)*QG.L(c,t))
    + SUM(fcap,      (1-SUBC.L(c,fcap,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,fcap,t)*capcomp(fcap,c)*SUM(ins2, DKINS.L(ins2,fcap,t)))
    + SUM(dstk,      (1-SUBC.L(c,dstk,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,dstk,t)*SUM(ins2, qdstk(c,ins2,t)))
    + SUM((cp,tacm), (1-SUBC.L(c,tacm,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,tacm,t)*icm(c,cp)*QM.L(cp,t))
    + SUM((cp,tace), (1-SUBC.L(c,tace,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,tace,t)*ice(c,cp)*QE.L(cp,t))
    + SUM((cp,tacd), (1-SUBC.L(c,tacd,t))*PQS.L(c,t)*(1+TQ.L(c,t))*TVAC.L(c,tacd,t)*icd(c,cp)*QD.L(cp,t));
SIMSAM(taxfacact,a,tcur(t),sim) = SUM(f$mtaxfa(taxfacact,f), TFA.L(f,a,t)*WF.L(f,t)*WFDIST.L(f,a,t)*QF.L(f,a,t));    

SIMSAM('prqmbar',c,tcur(t),sim) = YPRQMBART.L(c,t);
SIMSAM('prexr',c,tcur(t),sim) = YPREXRT.L(c,t);


* filas vat rebate
SIMSAM('rebate-vat',a,tcur(t),sim) = -SUM(c, shrbtvat(c,a,t)*TVAC.L(c,a,t)*PQS.L(c,t)*(1-SUBC.L(c,a,t))*(1+TQ.L(c,t))*QINT.L(c,a,t));
SIMSAM('rebate-vat',insgov,tcur(t),sim) = SUM(a, RBTVAT.L(a,t));

SIMSAM(subcom,c,tcur(t),sim) =
      SUM(a,         -SUBC.L(c,a,t)*PQS.L(c,t)*(1+TQ.L(c,t))*QINT.L(c,a,t))
    + SUM(h,         -SUBC.L(c,h,t)*PQS.L(c,t)*(1+TQ.L(c,t))*QH.L(c,h,t))
    + SUM(insngo,    -SUBC.L(c,insngo,t)*PQS.L(c,t)*(1+TQ.L(c,t))*QNGO.L(c,insngo,t))
    + SUM(instrst,   -SUBC.L(c,instrst,t)*PQS.L(c,t)*(1+TQ.L(c,t))*QTRST.L(c,instrst,t))
    + SUM(insgov,    -SUBC.L(c,insgov,t)*PQS.L(c,t)*(1+TQ.L(c,t))*QG.L(c,t))
    + SUM(fcap,      -SUBC.L(c,fcap,t)*PQS.L(c,t)*(1+TQ.L(c,t))*capcomp(fcap,c)*SUM(ins2, DKINS.L(ins2,fcap,t)))
    + SUM(dstk,      -SUBC.L(c,dstk,t)*PQS.L(c,t)*(1+TQ.L(c,t))*SUM(ins2, qdstk(c,ins2,t)))
    + SUM((cp,tacm), -SUBC.L(c,tacm,t)*PQS.L(c,t)*(1+TQ.L(c,t))*icm(c,cp)*QM.L(cp,t))
    + SUM((cp,tace), -SUBC.L(c,tace,t)*PQS.L(c,t)*(1+TQ.L(c,t))*ice(c,cp)*QE.L(cp,t))
    + SUM((cp,tacd),   -SUBC.L(c,tacd,t)*PQS.L(c,t)*(1+TQ.L(c,t))*icd(c,cp)*QD.L(cp,t));


* filas com
SIMSAM(c,a,tcur(t),sim)     = PQD.L(c,a,t)*QINT.L(c,a,t);
SIMSAM(c,h,tcur(t),sim)     = PQD.L(c,h,t)*QH.L(c,h,t);
SIMSAM(c,insngo,tcur(t),sim)     = PQD.L(c,insngo,t)*QNGO.L(c,insngo,t);
SIMSAM(c,instrst,tcur(t),sim)     = PQD.L(c,instrst,t)*QTRST.L(c,instrst,t);

SIMSAM(c,inv,tcur(t),sim)   = SUM(fcap$mfcapinv(fcap,inv), capcomp(fcap,c)*SUM(ins2, DKINS.L(ins2,fcap,t))*PQD.L(c,fcap,t));
SIMSAM(c,dstk,tcur(t),sim)   = PQD.L(c,dstk,t)*SUM(ins2, qdstk(c,ins2,t));
SIMSAM(c,insgov,tcur(t),sim) = PQD.L(c,insgov,t)*QG.L(c,t);
SIMSAM(c,insrow,tcur(t),sim) = PWE.L(c,t)*EXR.L(t)*QE.L(c,t);
SIMSAM(c,tacd,tcur(t),sim)  = SUM(cp, PQD.L(c,tacd,t)*icd(c,cp)*QD.L(cp,t));
SIMSAM(c,tacm,tcur(t),sim)  = SUM(cp, PQD.L(c,tacm,t)*icm(c,cp)*QM.L(cp,t));
SIMSAM(c,tace,tcur(t),sim)  = SUM(cp, PQD.L(c,tace,t)*ice(c,cp)*QE.L(cp,t));


* filas marg
SIMSAM(tacd,c,tcur(t),sim) = SUM(ct, PQD.L(ct,tacd,t)*icd(ct,c)*QD.L(c,t));
SIMSAM(tacm,c,tcur(t),sim) = SUM(ct, PQD.L(ct,tacm,t)*icm(ct,c)*QM.L(c,t));
SIMSAM(tace,c,tcur(t),sim) = SUM(ct, PQD.L(ct,tace,t)*ice(ct,c)*QE.L(c,t));

* filas fac
SIMSAM(f,a,tcur(t),sim)$(fsam(f) AND fva(f))    = WF.L(f,t)*WFDIST.L(f,a,t)*QF.L(f,a,t);
SIMSAM(fcapg,a,tcur(t),sim) = shfcapga(fcapg,a)*PA.L(a,t)*QA.L(a,t);
SIMSAM(f,insrow,tcur(t),sim) = TRNSFR.L(f,insrow,t)*EXR.L(t);
SIMSAM(f,'prexr',tcur(t),sim) = SUM(c, YPREXR.L(c,f,t));
SIMSAM(f,fp,tcur(t),sim) = YF.L(fp,t)*shffp(f,fp);

* filas invng
SIMSAM(invng,capinsdng,tcur(t),sim) = SUM(fcapng$mfcapinv(fcapng,invng), PK.L(fcapng,t)*DKINS.L('ngovz',fcapng,t));
SIMSAM(invng,capgov,tcur(t),sim) = SUM(fcapng$mfcapinv(fcapng,invng), PK.L(fcapng,t)*DKINS.L('govz',fcapng,t));
SIMSAM(invng,caprow,tcur(t),sim) = SUM(fcapng$mfcapinv(fcapng,invng), PK.L(fcapng,t)*DKINS.L('rowz',fcapng,t));


* filas invg
SIMSAM(invg,capinsdng,tcur(t),sim) = SUM(fcapg$mfcapinv(fcapg,invg), PK.L(fcapg,t)*DKINS.L('ngovz',fcapg,t));
SIMSAM(invg,capgov,tcur(t),sim) = SUM(fcapg$mfcapinv(fcapg,invg), PK.L(fcapg,t)*DKINS.L('govz',fcapg,t));
SIMSAM(invg,caprow,tcur(t),sim) = SUM(fcapg$mfcapinv(fcapg,invg), PK.L(fcapg,t)*DKINS.L('rowz',fcapg,t));


* filas capngov
SIMSAM(capinsdng,insdng,tcur(t),sim)$mcapins(capinsdng,insdng) = SAV.L(insdng,t);
SIMSAM(capinsdng,caprow,tcur(t),sim) = NFFINS.L(t)*EXR.L(t);
SIMSAM(capinsdng,'prqmbar',tcur(t),sim) = SUM(c, YPRQMBAR.L(c,'ngovz',t));



* filas capgov
SIMSAM(capgov,insgov,tcur(t),sim) = SAVGX(t,sim);
SIMSAM(capgov,caprow,tcur(t),sim) = NFFG.L(t)*EXR.L(t);
SIMSAM(capgov,capinsdng,tcur(t),sim) = NDFG.L(t);

* filas caprow
SIMSAM(caprow,insrow,tcur(t),sim) = EXR.L(t)*SAVF.L(t);
SIMSAM(caprow,capfin,tcur(t),sim) = drf(t)*EXR.L(t);

* filas capfin
SIMSAM(capfin,capinsdng,tcur(t),sim) = drf(t)*EXR.L(t);


* filas dstk
SIMSAM(dstk,capinsdng,tcur(t),sim) = SUM(c, PQD.L(c,dstk,t)*qdstk(c,'ngovz',t));
SIMSAM(dstk,capgov,tcur(t),sim) = SUM(c, PQD.L(c,dstk,t)*qdstk(c,'govz',t));

  
* filas row
SIMSAM(insrow,c,tcur(t),sim)      = PWM.L(c,t)*EXR.L(t)*QM.L(c,t);
*SIMSAM(insrow,f,tcur(t),sim)      = TRNSFR.L(insrow,f,t)*EXR.L(t);
*SIMSAM(insrow,f,tcur(t),sim)      = YIF.L(insrow,f,t) - YIF.L(insrow,f,t) + YIF.L(insrow,f,t)/PREXR.L(t);
SIMSAM(insrow,f,tcur(t),sim)      = YIF.L(insrow,f,t);
*SIMSAM(insrow,insdng,tcur(t),sim) = TRII.L(insrow,insdng,t)/PREXR.L(t);
SIMSAM(insrow,insdng,tcur(t),sim) = TRII.L(insrow,insdng,t);
SIMSAM(insrow,insgov,tcur(t),sim)  = TRNSFR.L(insrow,insgov,t)*EXR.L(t);

* filas insdng
SIMSAM(insdng,f,tcur(t),sim)       = YIF.L(insdng,f,t);
SIMSAM(insdng,insgov,tcur(t),sim)   = TRNSFR.L(insdng,insgov,t)*CPI.L(t);
SIMSAM(insdng,insrow,tcur(t),sim)   = TRNSFR.L(insdng,insrow,t)*EXR.L(t);
SIMSAM(insdng,insdngp,tcur(t),sim) = TRII.L(insdng,insdngp,t);
SIMSAM(insdng,'prqmbar',tcur(t),sim) = SUM(c, YPRQMBAR.L(c,insdng,t));
SIMSAM(insdng,'prexr',tcur(t),sim) = SUM(c, YPREXR.L(c,insdng,t));

* filas instrst
SIMSAM(instrst,insrow,tcur(t),sim)   = SUM(c, SIMSAM(c,instrst,t,sim));

* filas act
SIMSAM(a,c,tcur(t),sim) = PXAC.L(a,c,t)*QXAC.L(a,c,t);
SIMSAM(a,'prqmbar',tcur(t),sim) = SUM(c, YPRQMBAR.L(c,a,t));

* filas gov
SIMSAM(insgov,taxdir,tcur(t),sim) = SUM(insdng, SIMSAM(taxdir,insdng,t,sim));
SIMSAM(insgov,taxcom,tcur(t),sim) = SUM(c, SIMSAM(taxcom,c,t,sim));
SIMSAM(insgov,taximp,tcur(t),sim) = SUM(c, SIMSAM(taximp,c,t,sim));
SIMSAM(insgov,taxexp,tcur(t),sim) = SUM(c, SIMSAM(taxexp,c,t,sim));
SIMSAM(insgov,taxact,tcur(t),sim) = SUM(a, SIMSAM(taxact,a,t,sim));
SIMSAM(insgov,taxfac,tcur(t),sim) = SUM(f, SIMSAM(taxfac,f,t,sim));
SIMSAM(insgov,taxvatc,tcur(t),sim) = SUM(c, SIMSAM(taxvatc,c,t,sim));
SIMSAM(insgov,taxfacact,tcur(t),sim) = SUM(a, SIMSAM(taxfacact,a,t,sim));
SIMSAM(insgov,subcom,tcur(t),sim) = SUM(c, SIMSAM(subcom,c,t,sim));

SIMSAM(insgov,insrow,tcur(t),sim)  = EXR.L(t)*TRNSFR.L(insgov,insrow,t);
SIMSAM(insgov,insdng,tcur(t),sim) = TRII.L(insgov,insdng,t);
SIMSAM(insgov,f,tcur(t),sim)      = YIF.L(insgov,f,t);
SIMSAM(insgov,'prqmbar',tcur(t),sim) = SUM(c, YPRQMBAR.L(c,insgov,t));
SIMSAM(insgov,'prexr',tcur(t),sim) = SUM(c, YPREXR.L(c,insgov,t));

* filas total
SIMSAM('total',ac,tcur(t),sim) = SUM(acntp, SIMSAM(acntp,ac,t,sim));
SIMSAM(ac,'total',tcur(t),sim) = SUM(acntp, SIMSAM(ac,acntp,t,sim));

*### end: SIMSAM ====================================================



simsambalchk(acnt,tcur(t),sim) = SIMSAM(acnt,'total',t,sim) - SIMSAM('total',acnt,t,sim);
errsimsambalchk(acnt,tcur(t),sim)$(ABS(simsambalchk(acnt,t,sim)) > 1e-4) = 1/0;
DISPLAY errsimsambalchk;


* re-scale
SIMSAM(ac,acp,tcur(t),sim) = SIMSAM(ac,acp,t,sim)*scaling('samsolrep');




simsamgdp(ac,acp,tcur(t),sim) = 100*SIMSAM(ac,acp,t,sim)/gdpindic('GDPMP','Nominal',t,sim);


actvashr(a,tcur(t),sim)$SUM((f,ap), SIMSAM(f,ap,t,sim)) =
  100*SUM(f, SIMSAM(f,a,t,sim)) / SUM((f,ap), SIMSAM(f,ap,t,sim));
actvashr('total',tcur(t),sim) = SUM(a, actvashr(a,t,sim));




* start: welfare ====================================================


* ev
ev(h,tcur(t),sim) = PROD(c$(PQD.L(c,h,t)), (PQD0(c,h,t)/PQD.L(c,h,t))**beta(c,h)) * ( EH.L(h,t) - SUM(c, gamma(c,h,t)*PQD.L(c,h,t)) )
  - ( EH0(h,t) - SUM(c, gamma(c,h,t)*PQD0(c,h,t)) );
ev('total',tcur(t),sim) = SUM(h, ev(h,t,sim));

* cv

cv(h,tcur(t),sim) = ( EH.L(h,t) - SUM(c, gamma(c,h,t)*PQD.L(c,h,t)) )
  - PROD(c$PQD0(c,h,t), (PQD.L(c,h,t)/PQD0(c,h,t))**beta(c,h)) * ( EH0(h,t) - SUM(c, gamma(c,h,t)*PQD0(c,h,t)) );
cv('total',tcur(t),sim) = SUM(h, cv(h,t,sim));

* re-scale
ev(ac,tcur(t),sim) = ev(ac,t,sim)*scaling('samsolrep');
cv(ac,tcur(t),sim) = cv(ac,t,sim)*scaling('samsolrep');


* end: welfare ======================================================

* start: macrosam

$ONTEXT
* fila tax2
MACROSAM('tax2','act2',tcur(t),sim) = SUM((actax,a), SIMSAM(actax,a,t,sim));
MACROSAM('tax2','com2',tcur(t),sim) = SUM((actax,c), SIMSAM(actax,c,t,sim)) + SUM((subcom,c), SIMSAM(subcom,c,t,sim));
MACROSAM('tax2','fac2',tcur(t),sim) = SUM((actax,f), SIMSAM(actax,f,t,sim));
MACROSAM('tax2','hhd2',tcur(t),sim) = SUM((actax,insdng), SIMSAM(actax,insdng,t,sim));

* filas rebate-vat2
MACROSAM('rebate-vat2','act2',tcur(t),sim) = SUM(a, SIMSAM('rebate-vat',a,t,sim));
MACROSAM('rebate-vat2','gov2',tcur(t),sim) = SUM(insgov, SIMSAM('rebate-vat',insgov,t,sim));
  


* filas com2
MACROSAM('com2','act2',tcur(t),sim) = SUM((c,a), SIMSAM(c,a,t,sim));
MACROSAM('com2','hhd2',tcur(t),sim) = SUM((c,insdng), SIMSAM(c,insdng,t,sim));
MACROSAM('com2','gov2',tcur(t),sim) = SUM((c,insgov), SIMSAM(c,insgov,t,sim));
MACROSAM('com2','s-i2',tcur(t),sim) = SUM((c,inv), SIMSAM(c,inv,t,sim)) + SUM((c,dstk), SIMSAM(c,dstk,t,sim));
MACROSAM('com2','row2',tcur(t),sim) = SUM((c,insrow), SIMSAM(c,insrow,t,sim)) + SUM((c,instrst), SIMSAM(c,instrst,t,sim));


* filas fac2
MACROSAM('fac2','act2',tcur(t),sim) = SUM((f,a), SIMSAM(f,a,t,sim));
MACROSAM('fac2','row2',tcur(t),sim) = SUM(f, SIMSAM(f,'row',t,sim));

* filas act2
MACROSAM('act2','com2',tcur(t),sim) = SUM((a,c), SIMSAM(a,c,t,sim));

* filas gov2
MACROSAM('gov2','tax2',tcur(t),sim) = SUM((insgov,actax), SIMSAM(insgov,actax,t,sim))
  + SUM((insgov,subcom), SIMSAM(insgov,subcom,t,sim));
MACROSAM('gov2','hhd2',tcur(t),sim) = SUM((insgov,insdng), SIMSAM(insgov,insdng,t,sim));
MACROSAM('gov2','row2',tcur(t),sim) = SUM(insgov, SIMSAM(insgov,'row',t,sim));
MACROSAM('gov2','fac2',tcur(t),sim) = SUM((insgov,f), SIMSAM(insgov,f,t,sim));

* filas hhd2
MACROSAM('hhd2','fac2',tcur(t),sim) = SUM((insdng,f), SIMSAM(insdng,f,t,sim));
MACROSAM('hhd2','gov2',tcur(t),sim) = SUM((insdng,insgov), SIMSAM(insdng,insgov,t,sim));
MACROSAM('hhd2','row2',tcur(t),sim) = SUM(insdng, SIMSAM(insdng,'row',t,sim));

* filas row2
MACROSAM('row2','hhd2',tcur(t),sim) = SUM((insrow,insdng), SIMSAM(insrow,insdng,t,sim));
MACROSAM('row2','gov2',tcur(t),sim) = SUM((insrow,insgov), SIMSAM(insrow,insgov,t,sim));
MACROSAM('row2','fac2',tcur(t),sim) = SUM((f,insrow), SIMSAM(insrow,f,t,sim));
MACROSAM('row2','com2',tcur(t),sim) = SUM((c,insrow), SIMSAM(insrow,c,t,sim));


* filas s-i2
MACROSAM('s-i2','hhd2',tcur(t),sim) = SUM((capinsdng,insdng), SIMSAM(capinsdng,insdng,t,sim));
MACROSAM('s-i2','gov2',tcur(t),sim) = SUM((capgov,insgov), SIMSAM(capgov,insgov,t,sim));
MACROSAM('s-i2','row2',tcur(t),sim) = SUM((caprow,insrow), SIMSAM(caprow,insrow,t,sim));

MACROSAM(ac2,ac2,tcur(t),sim) = 0;

* totales macrosam
MACROSAM('total2',ac2ntp,tcur(t),sim) = SUM(ac2nt,  MACROSAM(ac2nt,ac2ntp,t,sim));
MACROSAM(ac2nt,'total2',tcur(t),sim)   = SUM(ac2ntp, MACROSAM(ac2nt,ac2ntp,t,sim));



* end: macrosam

macrosambalchk(ac2nt,t,sim) = MACROSAM(ac2nt,'total2',t,sim) - MACROSAM('total2',ac2nt,t,sim);

errmacrosambalchk(ac2nt,t,sim)$(ABS(macrosambalchk(ac2nt,t,sim)/scaling('samsolrep')) > 1e-4) = 1/0;
DISPLAY errmacrosambalchk;

$OFFTEXT

* start: macrosam2


* start: aggregate SAM


macrosam2(ac2,ac2p,tcur(t),sim) = SUM((ac,acp)$(mapmacrosam(ac,ac2)$mapmacrosam(acp,ac2p)), SIMSAM(ac,acp,t,sim));

* totales macrosam2
macrosam2('total2',ac2ntp,tcur(t),sim) = SUM(ac2nt,  macrosam2(ac2nt,ac2ntp,t,sim));
macrosam2(ac2nt,'total2',tcur(t),sim)   = SUM(ac2ntp, macrosam2(ac2nt,ac2ntp,t,sim));
macrosam2(ac2,ac2,tcur(t),sim) = 0;

macrosam2gdp(ac2,ac2p,tcur(t),sim)  = 100*macrosam2(ac2,ac2p,t,sim)/gdpindic('GDPMP','Nominal',t,sim);

macrosam2balchk(ac2nt,tcur(t),sim) = macrosam2(ac2nt,'total2',t,sim) - macrosam2('total2',ac2nt,t,sim);

errmacrosam2balchk(ac2nt,tcur(t),sim)$(ABS(macrosam2balchk(ac2nt,t,sim)/scaling('samsolrep')) > 1e-4) = 1/0;
DISPLAY errmacrosam2balchk;



* end: aggregate SAM







* end: macrosam2


* start: facdemstruc ================================================

facdemstruc(f,a,tcur(t),sim)$(fva(f) AND SUM(fp$fva(fp), SIMSAM(fp,a,t,sim))) = 100 * SIMSAM(f,a,t,sim) / SUM(fp$fva(fp), SIMSAM(fp,a,t,sim));

facdemstruc(f,'total',tcur(t),sim)$fva(f) = 100 * SUM(a, SIMSAM(f,a,t,sim)) / SUM(a, SUM(fp$fva(fp), SIMSAM(fp,a,t,sim)));

facdemstruc('total',ac,tcur(t),sim) = SUM(f, facdemstruc(f,ac,t,sim));

* end: facdemstruc ==================================================


* start: facdemstruc2 ===============================================

facdemstruc2(acrepp,acrep,tcur(t),sim)$SUM(fp$fva(fp), SUM(a$macacrep(a,acrep), SIMSAM(fp,a,t,sim))) =
  100 * SUM(f$(fva(f) AND macacrep(f,acrepp)), SUM(a$macacrep(a,acrep), SIMSAM(f,a,t,sim))) / SUM(f$fva(f), SUM(a$macacrep(a,acrep), SIMSAM(f,a,t,sim)));

facdemstruc2(acrepp,'total3',tcur(t),sim) = SUM(f$(fva(f) AND macacrep(f,acrepp)), facdemstruc(f,'total',t,sim));

facdemstruc2('total3',acrep,tcur(t),sim) = SUM(acrepp, facdemstruc2(acrepp,acrep,t,sim));


* end: facdemstruc2 =================================================




* start: sectorstruc ================================================

*Note: For VAshr, PRDshr, and EMPshr, data scaled by share of activity
*in output of commodity to avoid double-counting when there is not a
*one-to-one mapping between activities and commodities.

sectorstruc(c,'VAshr',tcur(t),sim)       = 1e-15 + 100 * SUM(a, theta(a,c)*PVA.L(a,t)*QA.L(a,t)) / SUM(ap, PVA.L(ap,t)*QA.L(ap,t));
sectorstruc('total','VAshr',tcur(t),sim) = SUM(c, sectorstruc(c,'VAshr',t,sim));

sectorstruc(c,'PRDshr',tcur(t),sim)      = 1e-15 + 100 * SUM(a, theta(a,c)*PA.L(a,t)*QA.L(a,t)) / SUM(ap, PA.L(ap,t)*QA.L(ap,t));
sectorstruc('total','PRDshr',tcur(t),sim) = SUM(c, sectorstruc(c,'PRDshr',t,sim));

sectorstruc(c,'EMPshr',tcur(t),sim)        = 1e-15 + 100 * SUM(a, SUM(f$flab(f), theta(a,c)*QF.L(f,a,t))) / SUM(ap, SUM(f$flab(f), QF.L(f,ap,t)));
sectorstruc('total','EMPshr',tcur(t),sim) = SUM(c, sectorstruc(c,'EMPshr',t,sim));

* expos
*!! a precios mundiales
sectorstruc(c,'EXPshr',tcur(t),sim)       =
  1e-15 + [ 100 * (PE.L(c,t)*QE.L(c,t) + SUM(instrst, PQD.L(c,instrst,t)*QTRST.L(c,instrst,t))) / 
    SUM(cp, PE.L(cp,t)*QE.L(cp,t) + SUM(instrst, PQD.L(cp,instrst,t)*QTRST.L(cp,instrst,t))) ];
  
sectorstruc('total','EXPshr',tcur(t),sim) = SUM(c, sectorstruc(c,'EXPshr',t,sim));

* impos
*!! a precios mundiales
sectorstruc(c,'IMPshr',tcur(t),sim)       = 
  1e-15 + [ 100 * PM.L(c,t)*QM.L(c,t) / SUM(cp, PM.L(cp,t)*QM.L(cp,t)) ];
sectorstruc('total','IMPshr',tcur(t),sim) = SUM(c, sectorstruc(c,'IMPshr',t,sim));

* expos
sectorstruc(c,'EXP-OUTshr',tcur(t),sim)$(PX.L(c,t)*QX.L(c,t))
  = 1e-15 + 100 * (PE.L(c,t)*QE.L(c,t)+SUM(instrst, PQD.L(c,instrst,t)*QTRST.L(c,instrst,t))) / (PX.L(c,t)*QX.L(c,t));
sectorstruc('total','EXP-OUTshr',tcur(t),sim) =  
  100 * SUM(cp, PE.L(cp,t)*QE.L(cp,t) + SUM(instrst, PQD.L(cp,instrst,t)*QTRST.L(cp,instrst,t))) / 
  SUM(cp, PX.L(cp,t)*QX.L(cp,t));

* impos
sectorstruc(c,'IMP-DEMshr',tcur(t),sim)$(PQS.L(c,t)*QQ.L(c,t)) =
  1e-15 + 100 * PM.L(c,t)*QM.L(c,t) / (PQS.L(c,t)*QQ.L(c,t));
sectorstruc('total','IMP-DEMshr',tcur(t),sim) = 100 * SUM(cp, PM.L(cp,t)*QM.L(cp,t)) / SUM(cp, PQS.L(cp,t)*QQ.L(cp,t));;

* end: sectorstruc ==================================================


* start: sectorstruc2 ===============================================

sectorstruc2(acrep,sectorcol1,tcur(t),sim) = SUM(ac$macacrep(ac,acrep), sectorstruc(ac,sectorcol1,t,sim));

sectorstruc2(acrep,'EXP-OUTshr',tcur(t),sim)$(SUM(c$macacrep(c,acrep), PX.L(c,t)*QX.L(c,t)))
  = 1e-15 + 100 * SUM(c$macacrep(c,acrep), PE.L(c,t)*QE.L(c,t)) / SUM(c$macacrep(c,acrep), PX.L(c,t)*QX.L(c,t));
sectorstruc2('total3','EXP-OUTshr',tcur(t),sim) = sectorstruc('total','EXP-OUTshr',t,sim);

sectorstruc2(acrep,'IMP-DEMshr',tcur(t),sim)$(SUM(c$macacrep(c,acrep), PQS.L(c,t)*QQ.L(c,t)))
  = 1e-15 + 100 * SUM(c$macacrep(c,acrep), PM.L(c,t)*QM.L(c,t)) / SUM(c$macacrep(c,acrep), PQS.L(c,t)*QQ.L(c,t));
sectorstruc2('total3','IMP-DEMshr',tcur(t),sim) = sectorstruc('total','IMP-DEMshr',t,sim);

* end: sectorstruc2 =================================================



* start: taxstruc ===================================================

taxstruc(actax,'TaxRev',tcur(t),sim)  = SUM(acnt, SIMSAM(actax,acnt,t,sim));
taxstruc(taxact,'TaxRev',tcur(t),sim)  = taxstruc(taxact,'TaxRev',t,sim) + SUM(a, SIMSAM('rebate-vat',a,t,sim));

taxstruc('total','TaxRev',tcur(t),sim) = SUM(actax, taxstruc(actax,'TaxRev',t,sim));

* GDPshr
taxstruc(ac,'GDPshr',tcur(t),sim)  = 100 * taxstruc(ac,'TaxRev',t,sim) / gdpindic('GDPMP','Nominal',t,sim);

* shrTaxRev
taxstruc(ac,'shrTaxRev',tcur(t),sim)  = 100 * taxstruc(ac,'TaxRev',t,sim) / SUM((actax,insgov), SIMSAM(insgov,actax,t,sim));


* end: taxstruc =====================================================

*### start: fiscalindic =============================================

fiscalindic('IncCur-DirTax','Nominal',tcur(t),sim)    = SUM((insgov,taxdir), SIMSAM(insgov,taxdir,t,sim));
fiscalindic('IncCur-FacTax','Nominal',tcur(t),sim)    = SUM((insgov,taxfac), SIMSAM(insgov,taxfac,t,sim));
fiscalindic('IncCur-ActTax','Nominal',tcur(t),sim)    = SUM((insgov,taxact), SIMSAM(insgov,taxact,t,sim)) +
  SUM(a, SIMSAM('rebate-vat',a,t,sim));
fiscalindic('IncCur-FacUseTax','Nominal',tcur(t),sim) = SUM((insgov,taxfacact), SIMSAM(insgov,taxfacact,t,sim));
fiscalindic('IncCur-ComTax','Nominal',tcur(t),sim)    = SUM((insgov,taxcom), SIMSAM(insgov,taxcom,t,sim));
fiscalindic('IncCur-VATax','Nominal',tcur(t),sim)     = SUM((insgov,taxvatc), SIMSAM(insgov,taxvatc,t,sim));
fiscalindic('IncCur-ImpTax','Nominal',tcur(t),sim)    = SUM((insgov,taximp), SIMSAM(insgov,taximp,t,sim));
fiscalindic('IncCur-ExpTax','Nominal',tcur(t),sim)    = SUM((insgov,taxexp), SIMSAM(insgov,taxexp,t,sim));
fiscalindic('IncCur-TrnsfrDom','Nominal',tcur(t),sim) = SUM((insgov,insd), SIMSAM(insgov,insd,t,sim));
fiscalindic('IncCur-TrnsfrFor','Nominal',tcur(t),sim) = SUM((insgov,insrow), SIMSAM(insgov,insrow,t,sim));
fiscalindic('IncCur-FacInc','Nominal',tcur(t),sim)    = SUM((insgov,f), SIMSAM(insgov,f,t,sim));

fiscalindic('IncCur-YPRQMBAR','Nominal',tcur(t),sim)    = SUM((insgov), SIMSAM(insgov,'prqmbar',t,sim));
fiscalindic('IncCur-YPREXR','Nominal',tcur(t),sim)    = SUM((insgov), SIMSAM(insgov,'prexr',t,sim));

fiscalindic('IncCur-Total','Nominal',tcur(t),sim)     = SUM(fiscalcol1, fiscalindic(fiscalcol1,'Nominal',t,sim));


fiscalindic('SpndCur-GovCon','Nominal',tcur(t),sim)       = SUM((c,insgov), SIMSAM(c,insgov,t,sim));  
fiscalindic('SpndCur-ComSub','Nominal',tcur(t),sim)       = SUM((insgov,subcom), -SIMSAM(insgov,subcom,t,sim));  


fiscalindic('SpndCur-TrnsfrDom','Nominal',tcur(t),sim) = SUM((insd,insgov), SIMSAM(insd,insgov,t,sim));
fiscalindic('SpndCur-TrnsfrFor','Nominal',tcur(t),sim) = SUM((insrow,insgov), SIMSAM(insrow,insgov,t,sim));
fiscalindic('SpndCur-Total','Nominal',tcur(t),sim)     = SUM(fiscalcol2, fiscalindic(fiscalcol2,'Nominal',t,sim));

fiscalindic('GovSav','Nominal',tcur(t),sim)    = SUM((capgov,insgov), SIMSAM(capgov,insgov,t,sim));


fiscalindic('SpndCap-Inv','Nominal',tcur(t),sim)      = SUM((inv,capgov), SIMSAM(inv,capgov,t,sim)) 
  + SUM((dstk,capgov), SIMSAM(dstk,capgov,t,sim));
fiscalindic('GovSurp','Nominal',tcur(t),sim) = fiscalindic('GovSav','Nominal',t,sim) - fiscalindic('SpndCap-Inv','Nominal',t,sim);
fiscalindic('IncCap-NetDomFin','Nominal',tcur(t),sim) = SUM((capgov,capinsd), SIMSAM(capgov,capinsd,t,sim));
fiscalindic('IncCap-NetForFin','Nominal',tcur(t),sim) = SUM((capgov,caprow), SIMSAM(capgov,caprow,t,sim));

fiscalindic('Inc-Total','Nominal',tcur(t),sim) = SUM(fiscalcol1,fiscalindic(fiscalcol1,'Nominal',t,sim));
fiscalindic('Spnd-Total','Nominal',tcur(t),sim) = SUM(fiscalcol2,fiscalindic(fiscalcol2,'Nominal',t,sim));



fiscalindic(fiscalcol,'GDPshr',tcur(t),sim)$gdpindic('GDPMP','Nominal',t,sim) = 
  100 * fiscalindic(fiscalcol,'Nominal',t,sim) / gdpindic('GDPMP','Nominal',t,sim);


* check balance in fiscalindic
* SAVG - YG + EG = 0
errfiscalindic1(tcur(t),sim)$
  (ABS(fiscalindic('GovSav','Nominal',t,sim)/scaling('samsolrep') 
     - fiscalindic('IncCur-Total','Nominal',t,sim)/scaling('samsolrep')
     + fiscalindic('SpndCur-Total','Nominal',t,sim)/scaling('samsolrep')) >1e-4)=1/0;
DISPLAY errfiscalindic1;

errfiscalindic2(tcur(t),sim)$
  (ABS(fiscalindic('Inc-Total','Nominal',t,sim)/scaling('samsolrep') 
     - fiscalindic('Spnd-Total','Nominal',t,sim)/scaling('samsolrep')) >1e-4)=1/0;
DISPLAY errfiscalindic2;


*### end: fiscalindic ===============================================

* start: fiscaldetindic

fiscaldetindic('Income-Current',actax,'Nominal',tcur(t),sim) = SUM(insgov, SIMSAM(insgov,actax,t,sim));
* overwrite for taxact
fiscaldetindic('Income-Current',taxact,'Nominal',tcur(t),sim) = fiscaldetindic('Income-Current',taxact,'Nominal',t,sim) + 
  SUM(a, SIMSAM('rebate-vat',a,t,sim));

fiscaldetindic('Income-Current',insdng,'Nominal',tcur(t),sim) = SUM(insgov, SIMSAM(insgov,insdng,t,sim));
fiscaldetindic('Income-Current',insrow,'Nominal',tcur(t),sim) = SUM(insgov, SIMSAM(insgov,insrow,t,sim));
fiscaldetindic('Income-Current',f,'Nominal',tcur(t),sim)    = SUM(insgov, SIMSAM(insgov,f,t,sim));
fiscaldetindic('Income-Current','Total','Nominal',tcur(t),sim)     = SUM(acnt, fiscaldetindic('Income-Current',acnt,'Nominal',t,sim));

fiscaldetindic('Spending-Current',c,'Nominal',tcur(t),sim) = SUM(insgov, SIMSAM(c,insgov,t,sim));
fiscaldetindic('Spending-Current',subcom,'Nominal',tcur(t),sim) = -SUM(insgov, SIMSAM(insgov,subcom,t,sim));
fiscaldetindic('Spending-Current',insrow,'Nominal',tcur(t),sim) = SUM(insgov, SIMSAM(insrow,insgov,t,sim));
fiscaldetindic('Spending-Current',insdng,'Nominal',tcur(t),sim) = SUM(insgov, SIMSAM(insdng,insgov,t,sim));

fiscaldetindic('Spending-Current','Total','Nominal',tcur(t),sim)     = SUM(acnt, fiscaldetindic('Spending-Current',acnt,'Nominal',t,sim));


fiscaldetindic('Savings','Total','Nominal',tcur(t),sim)    = SUM((capgov,insgov), SIMSAM(capgov,insgov,t,sim));


fiscaldetindic('Spending-Capital',fcap,'Nominal',tcur(t),sim) = SUM(inv$mfcapinv(fcap,inv), SUM(capgov, SIMSAM(inv,capgov,t,sim)));


fiscaldetindic('Surplus','Total','Nominal',tcur(t),sim) = fiscaldetindic('Savings','Total','Nominal',t,sim) 
  - SUM(f, fiscaldetindic('Spending-Capital',f,'Nominal',t,sim));


fiscaldetindic('Income-Capital',netdomfin,'Nominal',tcur(t),sim) = SUM((capgov,capinsd), SIMSAM(capgov,capinsd,t,sim));
fiscaldetindic('Income-Capital',netforfingov,'Nominal',tcur(t),sim) = SUM((capgov,caprow), SIMSAM(capgov,caprow,t,sim));


fiscaldetindic('Income-Total','Total','Nominal',tcur(t),sim) = 
  SUM(acnt,fiscaldetindic('Income-Current',acnt,'Nominal',t,sim))
  + SUM(acnt,fiscaldetindic('Income-Capital',acnt,'Nominal',t,sim));
  
fiscaldetindic('Spending-Total','Total','Nominal',tcur(t),sim) = 
  SUM(acnt, fiscaldetindic('Spending-Current',acnt,'Nominal',t,sim))
  + SUM(acnt, fiscaldetindic('Spending-Capital',acnt,'Nominal',t,sim));

fiscaldetindic(fiscaldetcol,ac,'GDPshr',tcur(t),sim)$gdpindic('GDPMP','Nominal',t,sim) = 
  100 * fiscaldetindic(fiscaldetcol,ac,'Nominal',t,sim) / gdpindic('GDPMP','Nominal',t,sim);



* end: fiscaldetindic

*### start: bopindic

* inflows
bopindic('InCur-Exports','Nominal',tcur(t),sim) = SUM((c,insrow), SIMSAM(c,insrow,t,sim));
bopindic('InCur-TourismReceipts','Nominal',tcur(t),sim) = SUM((c,instrst), SIMSAM(c,instrst,t,sim));
bopindic('InCur-Trnsfr-NoGov','Nominal',tcur(t),sim) = SUM(insdng, SIMSAM(insdng,'row',t,sim));
bopindic('InCur-Trnsfr-Gov','Nominal',tcur(t),sim) = SUM(insgov, SIMSAM(insgov,'row',t,sim));
bopindic('InCur-FacInc','Nominal',tcur(t),sim) = SUM(f, SIMSAM(f,'row',t,sim));
bopindic('InCur-RowSav','Nominal',tcur(t),sim) = SUM(caprow, SIMSAM(caprow,'row',t,sim));
bopindic('InCur-Total','Nominal',tcur(t),sim) = SUM(bopcol1, bopindic(bopcol1,'Nominal',t,sim));

* outflows
bopindic('OutCur-Imports','Nominal',tcur(t),sim)  = SUM((insrow,c), SIMSAM(insrow,c,t,sim));
bopindic('OutCur-Trnsfr-NoGov','Nominal',tcur(t),sim) = SUM(insdng, SIMSAM('row',insdng,t,sim));
bopindic('OutCur-Trnsfr-Gov','Nominal',tcur(t),sim) = SUM(insgov, SIMSAM('row',insgov,t,sim));
bopindic('OutCur-FacInc','Nominal',tcur(t),sim) = SUM(f, SIMSAM('row',f,t,sim));
bopindic('OutCur-Total','Nominal',tcur(t),sim) = SUM(bopcol2, bopindic(bopcol2,'Nominal',t,sim));

* (net) capital inflows    
bopindic('InCap-NetForFin-NoGov','Nominal',tcur(t),sim) = SUM((capinsdng,caprow), SIMSAM(capinsdng,caprow,t,sim));
bopindic('InCap-NetForFin-Gov','Nominal',tcur(t),sim) = SUM((capgov,caprow), SIMSAM(capgov,caprow,t,sim));
bopindic('InCap-FDI','Nominal',tcur(t),sim) = SUM((inv,caprow), SIMSAM(inv,caprow,t,sim));
bopindic('InCap-ChangeForRes','Nominal',tcur(t),sim) = -SUM((capfin,capinsdng), SIMSAM(capfin,capinsdng,t,sim));

bopindic('InCap-Total','Nominal',tcur(t),sim) = SUM(bopcol3, bopindic(bopcol3,'Nominal',t,sim));


  
bopindic(bopcol,'NomFCU',tcur(t),sim) = bopindic(bopcol,'Nominal',t,sim)/EXR.L(t);
bopindic(bopcol,'GDPshr',tcur(t),sim)$gdpindic('GDPMP','Nominal',t,sim) = 100 * (bopindic(bopcol,'Nominal',t,sim) / gdpindic('GDPMP','Nominal',t,sim));


* check balance in bopindic
errbopindic1(tcur(t),sim)$
  (ABS(bopindic('InCur-RowSav','Nominal',t,sim)/scaling('samsolrep') - bopindic('InCap-Total','Nominal',t,sim)/scaling('samsolrep'))>1e-4)=1/0;
DISPLAY errbopindic1;

errbopindic2(tcur(t),sim)$
  (ABS(bopindic('InCur-Total','Nominal',t,sim)/scaling('samsolrep') - bopindic('OutCur-Total','Nominal',t,sim)/scaling('samsolrep'))>1e-4)=1/0;
DISPLAY errbopindic2;


*### end: bopindic

*### start: sectorindic =============================================

sectorindic(c,'VA','Real',tcur(t),sim) = SUM(a, theta(a,c)*(PVA00(a)*QA.L(a,t) 
  + SUM(f$fleo(f), WFA00(f,a)*QF.L(f,a,t))))*scaling('samsolrep');
sectorindic('total','VA','Real',tcur(t),sim) = SUM(c, sectorindic(c,'VA','Real',t,sim));

sectorindic(c,'EXP','Real',tcur(t),sim) = pwe00(c)*EXR00*QE.L(c,t)*scaling('samsolrep')
  + SUM(instrst, PQD00(c,instrst)*QTRST.L(c,instrst,t))*scaling('samsolrep');
sectorindic('total','EXP','Real',tcur(t),sim) = SUM(c, sectorindic(c,'EXP','Real',t,sim));

sectorindic(c,'IMP','Real',tcur(t),sim)       = pwm00(c)*EXR00*QM.L(c,t)*scaling('samsolrep');
sectorindic('total','IMP','Real',tcur(t),sim) = SUM(c, sectorindic(c,'IMP','Real',t,sim));

sectorindic(c,'DOM','Real',tcur(t),sim)       = PDS00(c)*QD.L(c,t)*scaling('samsolrep');
sectorindic('total','DOM','Real',tcur(t),sim) = SUM(c, sectorindic(c,'DOM','Real',t,sim));

sectorindic(a,'EMP','Real',tcur(t),sim)       = QFX('tot-lab',a,t,sim)*scaling('qlabsolrep');
sectorindic('total','EMP','Real',tcur(t),sim) = SUM(a, sectorindic(a,'EMP','Real',t,sim));

sectorindic(c,'Output','Real',tcur(t),sim)       = SUM(a, theta(a,c)*PA00(a)*QAX(a,t,sim))*scaling('samsolrep');
sectorindic('total','Output','Real',tcur(t),sim) = SUM(c, sectorindic(c,'Output','Real',t,sim));



*### end: sectorindic ===============================================


*### start: sectorindic2 ============================================

sectorindic2(acrep,sectorcol3,kgdp,tcur(t),sim) = SUM(ac$macacrep(ac,acrep), sectorindic(ac,sectorcol3,kgdp,t,sim));

*### end: sectorindic2 ==============================================



* start: demstruc
    
demstruc('Nominal',c,'IntermCon',tcur(t),sim) = SUM(a, SIMSAM(c,a,t,sim));
demstruc('Nominal',c,'DistMarg',tcur(t),sim) = SUM(tacd, SIMSAM(c,tacd,t,sim)) 
  + SUM(tacm, SIMSAM(c,tacm,t,sim)) + SUM(tace, SIMSAM(c,tace,t,sim));
demstruc('Nominal',c,'PrvCon',tcur(t),sim) = SUM(h, SIMSAM(c,h,t,sim)) + SUM(insngo, SIMSAM(c,insngo,t,sim));  
demstruc('Nominal',c,'TourismReceipts',tcur(t),sim) = SUM(instrst, SIMSAM(c,instrst,t,sim));  
demstruc('Nominal',c,'GovCon',tcur(t),sim) = SUM(insgov, SIMSAM(c,insgov,t,sim));  
demstruc('Nominal',c,'FixInv',tcur(t),sim) = SUM(inv, SIMSAM(c,inv,t,sim));
demstruc('Nominal',c,'StockChange',tcur(t),sim) = SUM(dstk, SIMSAM(c,dstk,t,sim));
demstruc('Nominal',c,'Exports',tcur(t),sim) = SUM(insrow, SIMSAM(c,insrow,t,sim));
demstruc('Nominal',c,'TotDem',tcur(t),sim) = SUM(acnt, SIMSAM(c,acnt,t,sim));
demstruc('Nominal','total',demcol,tcur(t),sim) = SUM(c, demstruc('Nominal',c,demcol,t,sim));

demstruc('shrTOT',ac,demcol,tcur(t),sim)$demstruc('Nominal',ac,'TotDem',t,sim) = 100*demstruc('Nominal',ac,demcol,t,sim)/demstruc('Nominal',ac,'TotDem',t,sim);
demstruc('GDPshr',ac,demcol,tcur(t),sim) = 100*demstruc('Nominal',ac,demcol,t,sim)/gdpindic('gdpmp','Nominal',t,sim);


  
* end: demstruc  

* start: demstruc2

demstruc2('Nominal',acrep,'IntermCon',tcur(t),sim) = SUM(c$macacrep(c,acrep), SUM(a, SIMSAM(c,a,t,sim)));
demstruc2('Nominal',acrep,'DistMarg',tcur(t),sim) = SUM(c$macacrep(c,acrep), SUM(tacd, SIMSAM(c,tacd,t,sim)) + SUM(tacm, SIMSAM(c,tacm,t,sim)) + SUM(tace, SIMSAM(c,tace,t,sim)));
demstruc2('Nominal',acrep,'PrvCon',tcur(t),sim) = SUM(c$macacrep(c,acrep), SUM(h, SIMSAM(c,h,t,sim)));  
demstruc2('Nominal',acrep,'GovCon',tcur(t),sim) = SUM(c$macacrep(c,acrep), SUM(insgov, SIMSAM(c,insgov,t,sim)));  
demstruc2('Nominal',acrep,'FixInv',tcur(t),sim) = SUM(c$macacrep(c,acrep), SUM(inv, SIMSAM(c,inv,t,sim)));
demstruc2('Nominal',acrep,'StockChange',tcur(t),sim) = SUM(c$macacrep(c,acrep), SUM(dstk, SIMSAM(c,dstk,t,sim)));
demstruc2('Nominal',acrep,'Exports',tcur(t),sim) = SUM(c$macacrep(c,acrep), SUM(insrow, SIMSAM(c,insrow,t,sim)));
demstruc2('Nominal',acrep,'TotDem',tcur(t),sim) = SUM(c$macacrep(c,acrep), SUM(acnt, SIMSAM(c,acnt,t,sim)));
demstruc2('Nominal','total3',demcol,tcur(t),sim) = SUM(acrep, demstruc2('Nominal',acrep,demcol,t,sim));

demstruc2('shrTOT',acrep,demcol,tcur(t),sim)$demstruc2('Nominal',acrep,'TotDem',t,sim) = 100*demstruc2('Nominal',acrep,demcol,t,sim)/demstruc2('Nominal',acrep,'TotDem',t,sim);
demstruc2('GDPshr',acrep,demcol,tcur(t),sim) = 100*demstruc2('Nominal',acrep,demcol,t,sim)/gdpindic('gdpmp','Nominal',t,sim);

* end: demstruc2



* start: incomestruc

incomestruc('INCval',ins,f,tcur(t),sim)            = SIMSAM(ins,f,t,sim);
incomestruc('INCval',ins,'tot-tax',tcur(t),sim)    = SUM(actax, SIMSAM(ins,actax,t,sim));
incomestruc('INCval',ins,'trgov',tcur(t),sim)      = SUM(insgov, SIMSAM(ins,insgov,t,sim));
incomestruc('INCval',ins,'trinsdng',tcur(t),sim)   = SUM(insdng, SIMSAM(ins,insdng,t,sim));
incomestruc('INCval',ins,'trrow',tcur(t),sim)      = SUM(insrow, SIMSAM(ins,insrow,t,sim));
incomestruc('INCval',insrow,'imports_alt',tcur(t),sim) = SUM(c, SIMSAM(insrow,c,t,sim));
incomestruc('INCval',ins,'total',tcur(t),sim)      = SUM(acnt, incomestruc('INCval',ins,acnt,t,sim));
incomestruc('INCshr',ins,ac,tcur(t),sim)           = 100*incomestruc('INCval',ins,ac,t,sim) / incomestruc('INCval',ins,'total',t,sim);


* end: incomestruc


incomestruc2('INCval',acrep,acrepp,tcur(t),sim) = SUM(ac$macacrep(ac,acrep), SUM(acp$macacrep(acp,acrepp), incomestruc('INCval',ac,acp,t,sim)));  
incomestruc2('INCshr',acrep,acrepp,tcur(t),sim)$incomestruc2('INCval',acrep,'total3',t,sim) = 
  100*incomestruc2('INCval',acrep,acrepp,t,sim) / incomestruc2('INCval',acrep,'total3',t,sim);


* start: incomedistindic --------------------------------------------

incomedistindic('POPshr',h,tcur(t),sim) = 100*pop(h,t)/SUM(hp, pop(hp,t));
incomedistindic('INCshr',h,tcur(t),sim) = 100*SIMSAM(h,'total',t,sim)/SUM(hp, SIMSAM(hp,'total',t,sim));
incomedistindic('CONshr',h,tcur(t),sim) = 100*SUM(c, SIMSAM(c,h,t,sim))/SUM((cp,hp), SIMSAM(cp,hp,t,sim));
incomedistindic(inccol,'total',tcur(t),sim) = SUM(h, incomedistindic(inccol,h,t,sim));
incomedistindic('INCpc',ac,tcur(t),sim)  = yhpcX(ac,t,sim);
incomedistindic('CONpc',ac,tcur(t),sim)  = qhpcX(ac,t,sim);

* end: incomedistindic ----------------------------------------------


* start: debtindic

debtindic('NonGov-ForDebt-GDPshr',tcur(t),sim) = 100*FDEBT.L('ngovz',t)*EXR.L(t)/GDPMP.L(t);
debtindic('Gov-ForDebt-GDPshr',tcur(t),sim) = 100*FDEBT.L('govz',t)*EXR.L(t)/GDPMP.L(t);
debtindic('Gov-DomDebt-GDPshr',tcur(t),sim) = 100*GDEBT.L(t)*EXR.L(t)/GDPMP.L(t);
  
* end: debtindic 


* start: spndstruc

spndstruc('SPNDval',ac,ins,tcur(t),sim) = SIMSAM(ac,ins,t,sim);
spndstruc('SPNDshr',ac,ins,tcur(t),sim) = spndstruc('SPNDval',ac,ins,t,sim)/spndstruc('SPNDval','total',ins,t,sim)*100;

* end: spndstruc
  
  
* start: coststruc

coststruc(ac,a,tcur(t),sim) = SIMSAM(ac,a,t,sim)/SIMSAM('total',a,t,sim)*100;


* end: coststruc

* start: coststruc2

coststruc2(acrep,acrepp,tcur(t),sim)$SUM(a$macacrep(a,acrepp), SIMSAM('total',a,t,sim)) = 
  100*SUM(ac$macacrep(ac,acrep), SUM(a$macacrep(a,acrepp), SIMSAM(ac,a,t,sim)))
  /
  SUM(a$macacrep(a,acrepp), SIMSAM('total',a,t,sim));
;

 
* end: coststruc2

* start: salestruc

salestruc(c,ac,tcur(t),sim) = SIMSAM(c,ac,t,sim)/SIMSAM(c,'total',t,sim)*100;



* end: salestruc




* start: laborsupstruc


laborsupstruc(f,tcur(t),sim)$(flab(f) AND fsam(f)) = 100*SUM(h, QFINS.L(h,f,t))/SUM((hp,fp)$(flab(fp) AND fsam(fp)), QFINS.L(hp,fp,t));
laborsupstruc('total',tcur(t),sim) = SUM(flab, laborsupstruc(flab,t,sim));
  
* end: laborsupstruc



* start: labordemstruc

labordemstruc(f,a,tcur(t),sim)$(fsam(f) AND flab(f) AND SUM(fp$(fsam(fp) AND flab(fp)), QF.L(fp,a,t))) = 
  100*QF.L(f,a,t)/SUM(fp$(fsam(fp) AND flab(fp)), QF.L(fp,a,t));

* total activities excluding leisure
labordemstruc(f,'total',tcur(t),sim)$(fsam(f) AND flab(f))
  = 100*SUM(a, QF.L(f,a,t))/SUM(fp$(fsam(fp) AND flab(fp)), SUM(a, QF.L(fp,a,t)));
labordemstruc('total',ac,tcur(t),sim) = SUM(flab, labordemstruc(flab,ac,t,sim));

* end: labordemstruc


* start: labordemstruc2

labordemstruc2(acrep,acrepp,tcur(t),sim) = 
  SUM(f$(fsam(f) AND flab(f) AND macacrep(f,acrep)), SUM(a$macacrep(a,acrepp), QF.L(f,a,t)));
labordemstruc2('total3',acrepp,tcur(t),sim) = SUM(acrep, labordemstruc2(acrep,acrepp,t,sim));

labordemstruc2(acrep,'total3',tcur(t),sim) = SUM(acrepp, labordemstruc2(acrep,acrepp,t,sim));

labordemstruc2(acrep,acrepp,tcur(t),sim)$labordemstruc2('total3',acrepp,t,sim) = 
  100*labordemstruc2(acrep,acrepp,t,sim)/labordemstruc2('total3',acrepp,t,sim);


* end: labordemstruc2




* start: employsectorstruc

employsectorstruc(f,a,tcur(t),sim)$(fsam(f) AND flab(f)) = 100*QF.L(f,a,t)/SUM(ap, QF.L(f,ap,t));
employsectorstruc('total',a,tcur(t),sim) 
  = 100*SUM(f$(fsam(f) AND flab(f)), QF.L(f,a,t)) / SUM(f$(fsam(f) AND flab(f)), SUM(ap, QF.L(f,ap,t)));

employsectorstruc(ac,'total',tcur(t),sim) = SUM(a, employsectorstruc(ac,a,t,sim));

* end: employsectorstruc



* start: employsectorstruc2

employsectorstruc2(acrep,acrepp,tcur(t),sim) =
  SUM(ac$macacrep(ac,acrep), SUM(acp$macacrep(acp,acrepp), employsectorstruc(ac,acp,t,sim)));
;

* end: employsectorstruc2



* start: TFP calculation

$ONTEXT
qa2X(a,tcur(t),sim) = TFP00(a) * phi_va(a) * SUM(f$(NOT fleo(f)), delta_va(f,a)*(FPRDA00(f,a)*QF.L(f,a,t))**(-rho_va(a))) ** (-1/rho_va(a));

tfpaX(a,tcur(t),sim)$(PVA00(a)*qa2X(a,t,sim)) = PVA00(a)*QA.L(a,t) / (PVA00(a)*qa2X(a,t,sim));

tfpatotX(tcur(t),sim) = SUM(a, PVA00(a)*QA.L(a,t)) / SUM(a, PVA00(a)*qa2X(a,t,sim));
$OFFTEXT

* end: TPF calculation

* start: taxratcom 

taxratcom(c,taxcom,tcur(t),sim) = TQ.L(c,t)*100;
taxratcom(c,taxvatc,tcur(t),sim)$SUM(d$TVAC.L(c,d,t), 1) = SUM(d, TVAC.L(c,d,t))/SUM(d$TVAC.L(c,d,t), 1)*100;
taxratcom(c,taximp,tcur(t),sim) = TM.L(c,t)*100;
taxratcom(c,taxexp,tcur(t),sim) = TE.L(c,t)*100;
taxratcom(c,actaxc,tcur(t),sim)$(NOT taxratcom(c,actaxc,t,sim)) = 1e-15;
  
* end: taxratcom

* start: taxratact 
  
taxratact(a,taxact,tcur(t),sim) = TA.L(a,t)*100;
taxratact(a,taxact,tcur(t),sim)$(NOT taxratact(a,taxact,t,sim)) = 1e-15;


* end: taxratact 

* start: subratcom

subratcom(c,d,tcur(t),sim) = SUBC.L(c,d,t)*100; 
subratcom(c,d,tcur(t),sim)$(NOT subratcom(c,d,t,sim)) = 1e-15;

* start: subratcom

* start: taxratins

taxratins(ins,tcur(t),sim) = TY.L(ins,t)*100;
taxratins(ins,tcur(t),sim)$(NOT taxratins(ins,t,sim)) = 1e-15;


* end: taxratins 


* start: emiindic

emiindic('EmiVol',ac,acp,tcur(t),sim) = QEMIX('total',ac,acp,t,sim)*scaling('emisolrep');

emiindic('EmiSHR-ByUser',ac,acp,tcur(t),sim)$SUM(acnt, emiindic('EmiVol',acnt,acp,t,sim)) = 
  100*emiindic('EmiVol',ac,acp,t,sim)/SUM(acnt, emiindic('EmiVol',acnt,acp,t,sim));

emiindic('EmiSHR-ByCommodity',ac,acp,tcur(t),sim)$SUM(acnt, emiindic('EmiVol',ac,acnt,t,sim)) = 
  100*emiindic('EmiVol',ac,acp,t,sim)/SUM(acnt, emiindic('EmiVol',ac,acnt,t,sim));

* end: emiindic


* start: emiindic2

emiindic2('EmiVol',acrep,acrepp,tcur(t),sim) = SUM(ac$macacrep(ac,acrep), SUM(acp$macacrep(acp,acrepp), QEMIX('total',ac,acp,t,sim)*scaling('emisolrep')));

emiindic2('EmiSHR-ByUser',acrep,acrepp,tcur(t),sim)$SUM(acrepnt, emiindic2('EmiVol',acrepnt,acrepp,t,sim)) = 
  100*emiindic2('EmiVol',acrep,acrepp,t,sim)/SUM(acrepnt, emiindic2('EmiVol',acrepnt,acrepp,t,sim));

emiindic2('EmiSHR-ByCommodity',acrep,acrepp,tcur(t),sim)$SUM(acrepnt, emiindic2('EmiVol',acrep,acrepnt,t,sim)) = 
  100*emiindic2('EmiVol',acrep,acrepp,t,sim)/SUM(acrepnt, emiindic2('EmiVol',acrep,acrepnt,t,sim));

* end: emiindic2




DISPLAY "#### END: reploop.inc";


