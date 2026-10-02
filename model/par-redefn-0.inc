* par-redefn-0.inc

DISPLAY "#### START: par-redefn-0.inc";

ABSNOM0(t)               = ABSNOM.L(t);
CPI0(t)                  = CPI.L(t);           
DKA0(f,a,t)              = DKA.L(f,a,t);       
DKINS0(ins2,f,t)         = DKINS.L(ins2,f,t);
DPI0(t)                  = DPI.L(t);           
EG0(t)                   = EG.L(t);            
EH0(h,t)                 = EH.L(h,t);          
EXR0(t)                  = EXR.L(t);           
FBOR0(ins2,t)            = FBOR.L(ins2,t); 
FDEBT0(ins2,t)           = FDEBT.L(ins2,t);
FPRDA0(f,a,t)            = FPRDA.L(f,a,t);
GBOR0(t)                 = GBOR.L(t);      
GDEBT0(t)                = GDEBT.L(t);     
GDPMP0(t)                = GDPMP.L(t);
GOVRECGDP0(acgovrec,t)   = GOVRECGDP.L(acgovrec,t);   
GOVSPNDGDP0(acgovspnd,t) = GOVSPNDGDP.L(acgovspnd,t); 
GPRIMDEF0(t)             = GPRIMDEF.L(t);          
IADJ0(ins2,t)            = IADJ.L(ins2,t);
INVVAL0(t)               = INVVAL.L(t); 
INVVALG0(t)              = INVVALG.L(t);
ISCAL0(fcap,t)           = ISCAL.L(fcap,t);          
MPS0(ins,t)              = MPS.L(ins,t);       
MPSADJ0(t)               = MPSADJ.L(t);        
MPSSCAL0(t)              = MPSSCAL.L(t);        
NDFG0(t)                 = NDFG.L(t);
NFFG0(t)                 = NFFG.L(t);
NFFINS0(t)               = NFFINS.L(t);
NFFINSSCAL0(t)           = NFFINSSCAL.L(t);
NGOVPAYGDP0(acngovpay,t) = NGOVPAYGDP.L(acngovpay,t);   
PA0(a,t)                 = PA.L(a,t);          
PDD0(c,t)                = PDD.L(c,t);         
PDS0(c,t)                = PDS.L(c,t);         
PE0(c,t)                 = PE.L(c,t);          
PK0(fcap,t)              = PK.L(fcap,t);        
PM0(c,t)                 = PM.L(c,t);          
PQD0(c,ac,t)             = PQD.L(c,ac,t);
PQS0(c,t)                = PQS.L(c,t);          
PVA0(a,t)                = PVA.L(a,t);         
PX0(c,t)                 = PX.L(c,t);          
QA0(a,t)                 = QA.L(a,t);          
QD0(c,t)                 = QD.L(c,t);          
QE0(c,t)                 = QE.L(c,t);          
QF0(f,a,t)               = QF.L(f,a,t);        
QFINS0(ins,ac,t)         = QFINS.L(ins,ac,t);
QFINSSCAL0(f,t)          = QFINSSCAL.L(f,t);
QFS0(f,t)                = QFS.L(f,t);         

QG0(c,t)                 = QG.L(c,t);          

* overwrite so that scaling eq 0
QGSCAL0(t)               = QGSCAL.L(t);
qgb0(c,t) = QG0(c,t);
qgb(c,t) = qgb0(c,t);
QGSCAL0(t) = 0;

          
QH0(c,h,t)               = QH.L(c,h,t);        
QINT0(c,a,t)             = QINT.L(c,a,t);      
QINV0(c,t)               = QINV.L(c,t);        
QM0(c,t)                 = QM.L(c,t);          
QQ0(c,t)                 = QQ.L(c,t);          
QT0(c,t)                 = QT.L(c,t);          
QX0(c,t)                 = QX.L(c,t);          
REXR0(t)                 = REXR.L(t);          
RGDPFC0(t)               = RGDPFC.L(t);        
RGPRIMDEF0(t)            = RGPRIMDEF.L(t);     
RNDFG0(t)                 = RNDFG.L(t);               
SAV0(ins,t)              = SAV.L(ins,t);    
SAVF0(t)                 = SAVF.L(t);          
SHIF0(ins,f,t)           = SHIF.L(ins,f,t);
SUBC0(c,ac,t)            = SUBC.L(c,ac,t);    
SUBCSCAL0(t)             = SUBCSCAL.L(t);      
SUBCT0(t)                = SUBCT.L(t); 
TA0(a,t)                 = TA.L(a,t);              
TASCAL0(t)               = TASCAL.L(t);            
TE0(c,t)                 = TE.L(c,t);              
TESCAL0(t)               = TESCAL.L(t);            
TF0(f,t)                 = TF.L(f,t);              
TFP0(a,t)                = TFP.L(a,t);
TFSCAL0(t)               = TFSCAL.L(t);            
TM0(c,t)                 = TM.L(c,t);              
TMSCAL0(t)               = TMSCAL.L(t);   
TQ0(c,t)                 = TQ.L(c,t);              
TQSCAL0(t)               = TQSCAL.L(t);            
TRII0(ins,insp,t)        = TRII.L(ins,insp,t); 
TRNSFR0(ac,acp,t)        = TRNSFR.L(ac,acp,t);
TRNSFRSCAL0(ac,t)        = TRNSFRSCAL.L(ac,t);
TVAC0(c,ac,t)            = TVAC.L(c,ac,t);      
TVACSCAL0(t)             = TVACSCAL.L(t);        
TY0(ins,t)               = TY.L(ins,t);
TYSCAL0(t)               = TYSCAL.L(t);
UERAT0(f,t)              = UERAT.L(f,t);
WF0(f,t)                 = WF.L(f,t);          
WFAVG0(f,t)              = WFAVG.L(f,t);         
WFDIST0(f,a,t)           = WFDIST.L(f,a,t);    
YF0(f,t)                 = YF.L(f,t);          
YG0(t)                   = YG.L(t);            
YI0(ins,t)               = YI.L(ins,t);        
YIF0(ins,f,t)            = YIF.L(ins,f,t);     
YTAXIMP0(t)              = YTAXIMP.L(t);           
YTAXEXP0(t)              = YTAXEXP.L(t);  
YTAXVAT0(t)              = YTAXVAT.L(t);

QNGO0(c,insngo,t)        = QNGO.L(c,insngo,t);
QNGOSCAL0(insngo,t)      = QNGOSCAL.L(insngo,t);
FPRDASCAL0(t)            = FPRDASCAL.L(t);

INVVALF0(t)              = INVVALF.L(t);
FDISCAL0(t)              = FDISCAL.L(t);

TRDGDP0(t)               = TRDGDP.L(t);
RGDPMP0(t)               = RGDPMP.L(t);

TFA0(f,a,t)              = TFA.L(f,a,t);           
TFASCAL0(t)              = TFASCAL.L(t); 

QFHEND0(h,f,t)           = QFHEND.L(h,f,t);
QFHENDSCAL0(f,t)         = QFHENDSCAL.L(f,t);

WFA0(f,a,t)              = WFA.L(f,a,t);

LABPARTRAT0(t)           = LABPARTRAT.L(t);   
QLABSCAL0(t)             = QLABSCAL.L(t);     


QTRST0(c,instrst,t)      = QTRST.L(c,instrst,t);
QTRSTSCAL0(t)            = QTRSTSCAL.L(t);

TRSMREC0(t)              = TRSMREC.L(t);

GOVRECABS0(acgovrec,t)   = GOVRECABS.L(acgovrec,t);   
GOVSPNDABS0(acgovspnd,t) = GOVSPNDABS.L(acgovspnd,t);  
NGOVPAYABS0(acngovpay,t) = NGOVPAYABS.L(acngovpay,t);  


RBTVAT0(a,t)             = RBTVAT.L(a,t);

QXAC0(a,c,t)             = QXAC.L(a,c,t);   
PXAC0(a,c,t)             = PXAC.L(a,c,t);   
TFPSCAL0(t)              = TFPSCAL.L(t);    
PWE0(c,t)                = PWE.L(c,t);
PWM0(c,t)                = PWM.L(c,t);


* start: re-define exogenous component of transfers

trnsfrb0(ac,acp,t) = trnsfrb(ac,acp,t);

* end: re-define exogenous component of transfers



* start: re-define exogenous components tax rates

tyb(insdng,t) = tyb(insdng,t)*(1+ty01(insdng,t)*TYSCAL.L(t));
tyb0(insdng,t) = tyb(insdng,t);
TYSCAL0(t) = 0;

tmb(c,t) = tmb(c,t)*TMSCAL.L(t);
tmb0(c,t) = tmb(c,t);
TMSCAL0(t) = 1;

tfab(f,a,t) = tfab(f,a,t)*TFASCAL.L(t);
tfab0(f,a,t) = tfab(f,a,t);
TFASCAL0(t) = 1;

teb(c,t) = teb(c,t)*TESCAL.L(t);
teb0(c,t) = teb(c,t);
TESCAL0(t) = 1;

tqb(c,t) = tqb(c,t)*TQSCAL.L(t);
tqb0(c,t) = tqb(c,t);
TQSCAL0(t) = 1;

tab(a,t) = tab(a,t)*(1+ta01(a)*TASCAL.L(t));
tab0(a,t) = tab(a,t);
TASCAL0(t) = 0;

tfb(f,t) = tfb(f,t)*(1+tf01(f,t)*TFSCAL.L(t));
tfb0(f,t) = tfb(f,t);
TFSCAL0(t) = 0;

tvacb(c,ac,t) = tvacb(c,ac,t)*TVACSCAL.L(t);
tvacb0(c,ac,t) = tvacb(c,ac,t);
TVACSCAL0(t) = 1;

subcb(c,ac,t) = subcb(c,ac,t)*SUBCSCAL.L(t);
subcb0(c,ac,t) = subcb(c,ac,t);
SUBCSCAL0(t) = 1;


* end: re-define exogenous components tax rates


YPREXR0(c,ac,t)   = YPREXR.L(c,ac,t); 
YPREXRT0(c,t)     = YPREXRT.L(c,t);
PREXR0(t)         = PREXR.L(t);
PRQMBAR0(c,t)     = PRQMBAR.L(c,t);
YPRQMBART0(c,t)   = YPRQMBART.L(c,t);
YPRQMBAR0(c,ac,t) = YPRQMBAR.L(c,ac,t);

DISPLAY "#### END: par-redefn-0.inc";