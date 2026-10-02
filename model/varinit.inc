* varinit.inc

DISPLAY "#### START: varinit.inc";

ABSNOM.L(t)               = ABSNOM0(t);
CPI.L(t)                  = CPI0(t);
DKA.L(f,a,t)              = DKA0(f,a,t);
DKINS.L(ins2,f,t)         = DKINS0(ins2,f,t);
DPI.L(t)                  = DPI0(t);
EG.L(t)                   = EG0(t);
EH.L(h,t)                 = EH0(h,t);
EXR.L(t)                  = EXR0(t);
FBOR.L(ins2,t)            = FBOR0(ins2,t);       
FDEBT.L(ins2,t)           = FDEBT0(ins2,t);
FPRDA.L(f,a,t)            = FPRDA0(f,a,t);
GBOR.L(t)                 = GBOR0(t);            
GDEBT.L(t)                = GDEBT0(t);       
GDPMP.L(t)                = GDPMP0(t);
GOVRECGDP.L(acgovrec,t)   = GOVRECGDP0(acgovrec,t);   
GOVSPNDGDP.L(acgovspnd,t) = GOVSPNDGDP0(acgovspnd,t); 
GPRIMDEF.L(t)             = GPRIMDEF0(t);
IADJ.L(ins2,t)            = IADJ0(ins2,t);
INVVAL.L(t)               = INVVAL0(t); 
INVVALG.L(t)              = INVVALG0(t);
ISCAL.L(fcap,t)           = ISCAL0(fcap,t);
MPS.L(ins,t)              = MPS0(ins,t);
MPSADJ.L(t)               = MPSADJ0(t);
MPSSCAL.L(t)              = MPSSCAL0(t);
NDFG.L(t)                 = NDFG0(t);
NFFG.L(t)                 = NFFG0(t);
NFFINS.L(t)               = NFFINS0(t);
NFFINSSCAL.L(t)           = NFFINSSCAL0(t);
NGOVPAYGDP.L(acngovpay,t) = NGOVPAYGDP0(acngovpay,t); 
PA.L(a,t)                 = PA0(a,t);
PDD.L(c,t)                = PDD0(c,t);
PDS.L(c,t)                = PDS0(c,t);
PE.L(c,t)                 = PE0(c,t);
PK.L(fcap,t)              = PK0(fcap,t);
PM.L(c,t)                 = PM0(c,t);
PQD.L(c,ac,t)             = PQD0(c,ac,t);
PQS.L(c,t)                = PQS0(c,t);
PVA.L(a,t)                = PVA0(a,t);
PX.L(c,t)                 = PX0(c,t);
QA.L(a,t)                 = QA0(a,t);
QD.L(c,t)                 = QD0(c,t);
QE.L(c,t)                 = QE0(c,t);
QF.L(f,a,t)               = QF0(f,a,t);
QFINS.L(ins,ac,t)         = QFINS0(ins,ac,t);
QFINSSCAL.L(f,t)          = QFINSSCAL0(f,t);
QFS.L(f,t)                = QFS0(f,t);
QG.L(c,t)                 = QG0(c,t);
QGSCAL.L(t)               = QGSCAL0(t);
QH.L(c,h,t)               = QH0(c,h,t);
QINT.L(c,a,t)             = QINT0(c,a,t);
QINV.L(c,t)               = QINV0(c,t);
QM.L(c,t)                 = QM0(c,t);
QQ.L(c,t)                 = QQ0(c,t);
QT.L(c,t)                 = QT0(c,t);
QX.L(c,t)                 = QX0(c,t);
REXR.L(t)                 = REXR0(t);
RGDPFC.L(t)               = RGDPFC0(t);
RGPRIMDEF.L(t)            = RGPRIMDEF0(t);
RNDFG.L(t)                 = RNDFG0(t);               
SAV.L(ins,t)              = SAV0(ins,t);
SAVF.L(t)                 = SAVF0(t);
SHIF.L(ins,f,t)           = SHIF0(ins,f,t);
SUBC.L(c,ac,t)            = SUBC0(c,ac,t);  
SUBCSCAL.L(t)             = SUBCSCAL0(t);    
SUBCT.L(t)                = SUBCT0(t);
TA.L(a,t)                 = TA0(a,t);  
TASCAL.L(t)               = TASCAL0(t); 
TE.L(c,t)                 = TE0(c,t);  
TESCAL.L(t)               = TESCAL0(t); 
TF.L(f,t)                 = TF0(f,t);  
TFP.L(a,t)                = TFP0(a,t);
TFPSCAL.L(t)              = TFPSCAL0(t);
TFSCAL.L(t)               = TFSCAL0(t); 
TM.L(c,t)                 = TM0(c,t);  
TMSCAL.L(t)               = TMSCAL0(t); 
TQ.L(c,t)                 = TQ0(c,t);  
TQSCAL.L(t)               = TQSCAL0(t); 
TRII.L(ins,insp,t)        = TRII0(ins,insp,t);
TRNSFR.L(ac,acp,t)        = TRNSFR0(ac,acp,t);
TRNSFRSCAL.L(ac,t)        = TRNSFRSCAL0(ac,t);
TVAC.L(c,ac,t)            = TVAC0(c,ac,t);    
TVACSCAL.L(t)             = TVACSCAL0(t);      
TY.L(ins,t)               = TY0(ins,t);
TYSCAL.L(t)               = TYSCAL0(t);
UERAT.L(f,t)              = UERAT0(f,t);
WALRAS.L(t)               = 0;
WF.L(f,t)                 = WF0(f,t);
WFAVG.L(f,t)              = WFAVG0(f,t);
WFDIST.L(f,a,t)           = WFDIST0(f,a,t);
YF.L(f,t)                 = YF0(f,t);
YG.L(t)                   = YG0(t);
YI.L(ins,t)               = YI0(ins,t);
YIF.L(ins,f,t)            = YIF0(ins,f,t);
YTAXIMP.L(t)              = YTAXIMP0(t);  
YTAXEXP.L(t)              = YTAXEXP0(t);  
YTAXVAT.L(t)              = YTAXVAT0(t);
QNGO.L(c,insngo,t)        = QNGO0(c,insngo,t);
QNGOSCAL.L(insngo,t)      = QNGOSCAL0(insngo,t);
FPRDASCAL.L(t)            = FPRDASCAL0(t);

INVVALF.L(t)              = INVVALF0(t);
FDISCAL.L(t)              = FDISCAL0(t);

TRDGDP.L(t)               = TRDGDP0(t);
RGDPMP.L(t)               = RGDPMP0(t);

QFHEND.L(h,f,t)           = QFHEND0(h,f,t);     
QFHENDSCAL.L(f,t)         = QFHENDSCAL0(f,t);   

TFA.L(f,a,t)              = TFA0(f,a,t);           
TFASCAL.L(t)              = TFASCAL0(t); 

WFA.L(f,a,t)              = WFA0(f,a,t);

LABPARTRAT.L(t)           = LABPARTRAT0(t);   
QLABSCAL.L(t)             = QLABSCAL0(t);

QTRST.L(c,instrst,t)      = QTRST0(c,instrst,t);
QTRSTSCAL.L(t)            = QTRSTSCAL0(t);
TRSMREC.L(t)              = TRSMREC0(t);

GOVRECABS.L(acgovrec,t)   = GOVRECABS0(acgovrec,t);   
GOVSPNDABS.L(acgovspnd,t) = GOVSPNDABS0(acgovspnd,t);  
NGOVPAYABS.L(acngovpay,t) = NGOVPAYABS0(acngovpay,t);  

RBTVAT.L(a,t)             = RBTVAT0(a,t);
QXAC.L(a,c,t)             = QXAC0(a,c,t);   
PXAC.L(a,c,t)             = PXAC0(a,c,t);   

PWE.L(c,t)                = PWE0(c,t);
PWM.L(c,t)                = PWM0(c,t);

YPREXR.L(c,ac,t)          = YPREXR0(c,ac,t);
YPREXRT.L(c,t)            = YPREXRT0(c,t);
PREXR.L(t)                = PREXR0(t);
PRQMBAR.L(c,t)            = PRQMBAR0(c,t);
YPRQMBART.L(c,t)          = YPRQMBART0(c,t);
YPRQMBAR.L(c,ac,t)        = YPRQMBAR0(c,ac,t);

YPREXR.M(c,ac,t)          = 0;
YPREXRT.M(c,t)            = 0;
PREXR.M(t)                = 0;
PRQMBAR.M(c,t)            = 0;
YPRQMBART.M(c,t)          = 0;
YPRQMBAR.M(c,ac,t)        = 0;

DISPLAY "#### END: varinit.inc";