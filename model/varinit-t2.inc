* varinit-t2.inc

DISPLAY "#### START: varinit-t2.inc";
                      
ABSNOM.L(tcur(t))$(NOT tmin(t))               = ABSNOM.L(t-1);                      
CPI.L(tcur(t))$(NOT tmin(t))                  = CPI.L(t-1);
DKA.L(f,a,tcur(t))$(NOT tmin(t))              = DKA.L(f,a,t-1);
DKINS.L(ins2,f,tcur(t))$(NOT tmin(t))         = DKINS.L(ins2,f,t-1);
DPI.L(tcur(t))$(NOT tmin(t))                  = DPI.L(t-1);
EG.L(tcur(t))$(NOT tmin(t))                   = EG.L(t-1);
EH.L(h,tcur(t))$(NOT tmin(t))                 = EH.L(h,t-1);
EXR.L(tcur(t))$(NOT tmin(t))                  = EXR.L(t-1);
FBOR.L(ins2,tcur(t))$(NOT tmin(t))            = FBOR.L(ins2,t-1);   
FDEBT.L(ins2,tcur(t))$(NOT tmin(t))           = FDEBT.L(ins2,t-1);  
FPRDA.L(f,a,tcur(t))$(NOT tmin(t))            = FPRDA.L(f,a,t-1);
GBOR.L(tcur(t))$(NOT tmin(t))                 = GBOR.L(t-1);   
GDEBT.L(tcur(t))$(NOT tmin(t))                = GDEBT.L(t-1);  
GDPMP.L(tcur(t))$(NOT tmin(t))                = GDPMP.L(t-1);
GOVRECGDP.L(acgovrec,tcur(t))$(NOT tmin(t))   = GOVRECGDP.L(acgovrec,t-1);   
GOVSPNDGDP.L(acgovspnd,tcur(t))$(NOT tmin(t)) = GOVSPNDGDP.L(acgovspnd,t-1); 
GPRIMDEF.L(tcur(t))$(NOT tmin(t))             = GPRIMDEF.L(t-1);
IADJ.L(ins2,tcur(t))$(NOT tmin(t))            = IADJ.L(ins2,t-1);
INVVAL.L(tcur(t))$(NOT tmin(t))               = INVVAL.L(t-1); 
INVVALG.L(tcur(t))$(NOT tmin(t))              = INVVALG.L(t-1);
ISCAL.L(fcap,tcur(t))$(NOT tmin(t))           = ISCAL.L(fcap,t-1);
MPS.L(ins,tcur(t))$(NOT tmin(t))              = MPS.L(ins,t-1);
MPSADJ.L(tcur(t))$(NOT tmin(t))               = MPSADJ.L(t-1);
MPSSCAL.L(tcur(t))$(NOT tmin(t))              = MPSSCAL.L(t-1);
NDFG.L(tcur(t))$(NOT tmin(t))                 = NDFG.L(t-1);     
NFFG.L(tcur(t))$(NOT tmin(t))                 = NFFG.L(t-1);
NFFINS.L(t)$(NOT tmin(t))                     = NFFINS.L(t-1);
NFFINSSCAL.L(tcur(t))$(NOT tmin(t))           = NFFINSSCAL.L(t-1);
NGOVPAYGDP.L(acngovpay,tcur(t))$(NOT tmin(t)) = NGOVPAYGDP.L(acngovpay,t-1);   
PA.L(a,tcur(t))$(NOT tmin(t))                 = PA.L(a,t-1);
PDD.L(c,tcur(t))$(NOT tmin(t))                = PDD.L(c,t-1);
PDS.L(c,tcur(t))$(NOT tmin(t))                = PDS.L(c,t-1);
PE.L(c,tcur(t))$(NOT tmin(t))                 = PE.L(c,t-1);
PK.L(fcap,tcur(t))$(NOT tmin(t))              = PK.L(fcap,t-1);
PM.L(c,tcur(t))$(NOT tmin(t))                 = PM.L(c,t-1);
PQD.L(c,ac,tcur(t))$(NOT tmin(t))             = PQD.L(c,ac,t-1);
PQS.L(c,tcur(t))$(NOT tmin(t))                = PQS.L(c,t-1);
PVA.L(a,tcur(t))$(NOT tmin(t))                = PVA.L(a,t-1);
PX.L(c,tcur(t))$(NOT tmin(t))                 = PX.L(c,t-1);
QA.L(a,tcur(t))$(NOT tmin(t))                 = QA.L(a,t-1);
QD.L(c,tcur(t))$(NOT tmin(t))                 = QD.L(c,t-1);
QE.L(c,tcur(t))$(NOT tmin(t))                 = QE.L(c,t-1);
QF.L(f,a,tcur(t))$(NOT tmin(t))               = QF.L(f,a,t-1);
QFINS.L(ins,f,tcur(t))$(NOT tmin(t))          = QFINS.L(ins,f,t-1);
QFINSSCAL.L(f,tcur(t))$(NOT tmin(t))          = QFINSSCAL.L(f,t-1);
QFS.L(f,tcur(t))$(NOT tmin(t))                = QFS.L(f,t-1);
QG.L(c,tcur(t))$(NOT tmin(t))                 = QG.L(c,t-1);
QGSCAL.L(tcur(t))$(NOT tmin(t))               = QGSCAL.L(t-1);
QH.L(c,h,tcur(t))$(NOT tmin(t))               = QH.L(c,h,t-1);
QINT.L(c,a,tcur(t))$(NOT tmin(t))             = QINT.L(c,a,t-1);
QINV.L(c,tcur(t))$(NOT tmin(t))               = QINV.L(c,t-1);
QM.L(c,tcur(t))$(NOT tmin(t))                 = QM.L(c,t-1);
QQ.L(c,tcur(t))$(NOT tmin(t))                 = QQ.L(c,t-1);
QT.L(c,tcur(t))$(NOT tmin(t))                 = QT.L(c,t-1);
QX.L(c,tcur(t))$(NOT tmin(t))                 = QX.L(c,t-1);
REXR.L(tcur(t))$(NOT tmin(t))                 = REXR.L(t-1);
RGDPFC.L(tcur(t))$(NOT tmin(t))               = RGDPFC.L(t-1);
RGPRIMDEF.L(tcur(t))$(NOT tmin(t))            = RGPRIMDEF.L(t-1);
RNDFG.L(tcur(t))$(NOT tmin(t))                 = RNDFG.L(t-1);
SAV.L(ins,tcur(t))$(NOT tmin(t))              = SAV.L(ins,t-1);
SAVF.L(tcur(t))$(NOT tmin(t))                 = SAVF.L(t-1);
SHIF.L(ins,f,tcur(t))$(NOT tmin(t))           = SHIF.L(ins,f,t-1);
SUBC.L(c,ac,tcur(t))$(NOT tmin(t))            = SUBC.L(c,ac,t-1);     
SUBCSCAL.L(tcur(t))$(NOT tmin(t))             = SUBCSCAL.L(t-1);       
SUBCT.L(tcur(t))$(NOT tmin(t))                = SUBCT.L(t-1);
TA.L(a,tcur(t))$(NOT tmin(t))                 = TA.L(a,t-1);  
TASCAL.L(tcur(t))$(NOT tmin(t))               = TASCAL.L(t-1); 
TE.L(c,tcur(t))$(NOT tmin(t))                 = TE.L(c,t-1);  
TESCAL.L(tcur(t))$(NOT tmin(t))               = TESCAL.L(t-1); 
TF.L(f,tcur(t))$(NOT tmin(t))                 = TF.L(f,t-1);  
TFP.L(a,tcur(t))$(NOT tmin(t))                = TFP.L(a,t-1);
TFPSCAL.L(tcur(t))$(NOT tmin(t))              = TFPSCAL.L(t-1);  
TFSCAL.L(tcur(t))$(NOT tmin(t))               = TFSCAL.L(t-1); 
TM.L(c,tcur(t))$(NOT tmin(t))                 = TM.L(c,t-1);  
TMSCAL.L(tcur(t))$(NOT tmin(t))               = TMSCAL.L(t-1); 
TQ.L(c,tcur(t))$(NOT tmin(t))                 = TQ.L(c,t-1);  
TQSCAL.L(tcur(t))$(NOT tmin(t))               = TQSCAL.L(t-1); 
TRII.L(ins,insp,tcur(t))$(NOT tmin(t))        = TRII.L(ins,insp,t-1);
TRNSFR.L(ac,acp,tcur(t))$(NOT tmin(t))        = TRNSFR.L(ac,acp,t-1);
TRNSFRSCAL.L(ac,tcur(t))$(NOT tmin(t))        = TRNSFRSCAL.L(ac,t-1);
TVAC.L(c,ac,tcur(t))$(NOT tmin(t))            = TVAC.L(c,ac,t-1);       
TVACSCAL.L(tcur(t))$(NOT tmin(t))             = TVACSCAL.L(t-1);         
TY.L(ins,tcur(t))$(NOT tmin(t))               = TY.L(ins,t-1);
TYSCAL.L(tcur(t))$(NOT tmin(t))               = TYSCAL.L(t-1);
UERAT.L(f,tcur(t))$(NOT tmin(t))              = UERAT.L(f,t-1);
WALRAS.L(tcur(t))$(NOT tmin(t))               = 0;
WF.L(f,tcur(t))$(NOT tmin(t))                 = WF.L(f,t-1);
WFAVG.L(f,tcur(t))$(NOT tmin(t))              = WFAVG.L(f,t-1);
WFDIST.L(f,a,tcur(t))$(NOT tmin(t))           = WFDIST.L(f,a,t-1);
YF.L(f,tcur(t))$(NOT tmin(t))                 = YF.L(f,t-1);
YG.L(tcur(t))$(NOT tmin(t))                   = YG.L(t-1);
YI.L(ins,tcur(t))$(NOT tmin(t))               = YI.L(ins,t-1);
YIF.L(ins,f,tcur(t))$(NOT tmin(t))            = YIF.L(ins,f,t-1);
YTAXIMP.L(tcur(t))$(NOT tmin(t))              = YTAXIMP.L(t-1);  
YTAXEXP.L(tcur(t))$(NOT tmin(t))              = YTAXEXP.L(t-1);  
YTAXVAT.L(tcur(t))$(NOT tmin(t))              = YTAXVAT.L(t-1);

QNGO.L(c,insngo,tcur(t))$(NOT tmin(t))        = QNGO.L(c,insngo,t-1);
QNGOSCAL.L(insngo,tcur(t))$(NOT tmin(t))      = QNGOSCAL.L(insngo,t-1);
FPRDASCAL.L(tcur(t))$(NOT tmin(t))            = FPRDASCAL.L(t-1);

INVVALF.L(tcur(t))$(NOT tmin(t))              = INVVALF.L(t-1);
FDISCAL.L(tcur(t))$(NOT tmin(t))              = FDISCAL.L(t-1);

TRDGDP.L(tcur(t))$(NOT tmin(t))               = TRDGDP.L(t-1);
RGDPMP.L(tcur(t))$(NOT tmin(t))               = RGDPMP.L(t-1);

QFHEND.L(h,f,tcur(t))$(NOT tmin(t))           = QFHEND.L(h,f,t-1);     
QFHENDSCAL.L(f,tcur(t))$(NOT tmin(t))         = QFHENDSCAL.L(f,t-1);   

TFA.L(f,a,tcur(t))$(NOT tmin(t))              = TFA.L(f,a,t-1);           
TFASCAL.L(tcur(t))$(NOT tmin(t))              = TFASCAL.L(t-1); 

WFA.L(f,a,tcur(t))$(NOT tmin(t))              = WFA.L(f,a,t-1);
LABPARTRAT.L(tcur(t))$(NOT tmin(t))           = LABPARTRAT.L(t-1);   
QLABSCAL.L(tcur(t))$(NOT tmin(t))             = QLABSCAL.L(t-1);

QTRST.L(c,instrst,tcur(t))$(NOT tmin(t))      = QTRST.L(c,instrst,t-1);
QTRSTSCAL.L(tcur(t))$(NOT tmin(t))            = QTRSTSCAL.L(t-1);

TRSMREC.L(tcur(t))$(NOT tmin(t))                    = TRSMREC.L(t-1);


GOVRECABS.L(acgovrec,tcur(t))$(NOT tmin(t))         = GOVRECABS.L(acgovrec,t-1);   
GOVSPNDABS.L(acgovspnd,tcur(t))$(NOT tmin(t))       = GOVSPNDABS.L(acgovspnd,t-1);  
NGOVPAYABS.L(acngovpay,tcur(t))$(NOT tmin(t))       = NGOVPAYABS.L(acngovpay,t-1);  

RBTVAT.L(a,tcur(t))$(NOT tmin(t))                   = RBTVAT.L(a,t-1);

QXAC.L(a,c,tcur(t))$(NOT tmin(t))             = QXAC.L(a,c,t-1);   
PXAC.L(a,c,tcur(t))$(NOT tmin(t))             = PXAC.L(a,c,t-1);   

PWE.L(c,tcur(t))$(NOT tmin(t))                = PWE.L(c,t-1);
PWM.L(c,tcur(t))$(NOT tmin(t))                = PWM.L(c,t-1);

YPREXR.L(c,ac,tcur(t))$(NOT tmin(t))          = YPREXR.L(c,ac,t-1);
YPREXRT.L(c,tcur(t))$(NOT tmin(t))            = YPREXRT.L(c,t-1);
PREXR.L(tcur(t))$(NOT tmin(t))                = PREXR.L(t-1);
PRQMBAR.L(c,tcur(t))$(NOT tmin(t))            = PRQMBAR.L(c,t-1);
YPRQMBART.L(c,tcur(t))$(NOT tmin(t))          = YPRQMBART.L(c,t-1);
YPRQMBAR.L(c,ac,tcur(t))$(NOT tmin(t))        = YPRQMBAR.L(c,ac,t-1);


DISPLAY "#### END: varinit-t2.inc";