* repperc-xpy.inc

DISPLAY "#### START: repperc-xp.inc";  

PREXRXPY(t,simcur)$(PREXRX(t,simcur) AND PREXRX(t-1,simcur)) = 
  100*(PREXRX(t,simcur)/PREXRX(t-1,simcur) - 1);

YPREXRTXPY(c,t,simcur)$(YPREXRTX(c,t,simcur) AND YPREXRTX(c,t-1,simcur)) = 
  100*(YPREXRTX(c,t,simcur)/YPREXRTX(c,t-1,simcur) - 1);

YPREXRXPY(c,ac,t,simcur)$(YPREXRX(c,ac,t,simcur) AND YPREXRX(c,ac,t-1,simcur)) = 
  100*(YPREXRX(c,ac,t,simcur)/YPREXRX(c,ac,t-1,simcur) - 1);

PRQMBARXPY(c,t,simcur)$(PRQMBARX(c,t,simcur) AND PRQMBARX(c,t-1,simcur)) = 
  100*(PRQMBARX(c,t,simcur)/PRQMBARX(c,t-1,simcur) - 1);

YPRQMBARTXPY(c,t,simcur)$(YPRQMBARTX(c,t,simcur) AND YPRQMBARTX(c,t-1,simcur)) = 
  100*(YPRQMBARTX(c,t,simcur)/YPRQMBARTX(c,t-1,simcur) - 1);

YPRQMBARXPY(c,ac,t,simcur)$(YPRQMBARX(c,ac,t,simcur) AND YPRQMBARX(c,ac,t-1,simcur)) = 
  100*(YPRQMBARX(c,ac,t,simcur)/YPRQMBARX(c,ac,t-1,simcur) - 1);



PWEXPY(c,t,simcur)$PWEX(c,t-1,simcur) = 
  100*(PWEX(c,t,simcur)/PWEX(c,t-1,simcur) - 1);

PWMXPY(c,t,simcur)$PWMX(c,t-1,simcur) = 
  100*(PWMX(c,t,simcur)/PWMX(c,t-1,simcur) - 1);



QXACXPY(a,c,t,simcur)$QXACX(a,c,t-1,simcur) = 
  100*(QXACX(a,c,t,simcur)/QXACX(a,c,t-1,simcur) - 1);

PXACXPY(a,c,t,simcur)$PXACX(a,c,t-1,simcur) = 
  100*(PXACX(a,c,t,simcur)/PXACX(a,c,t-1,simcur) - 1);

QEMIXPY(ghg,ac,acp,t,simcur)$QEMIX(ghg,ac,acp,t-1,simcur) = 
  100*(QEMIX(ghg,ac,acp,t,simcur)/QEMIX(ghg,ac,acp,t-1,simcur) - 1);


ABSNOMXPY(t,simcur)$ABSNOMX(t-1,simcur) = 
  100*(ABSNOMX(t,simcur)/ABSNOMX(t-1,simcur) - 1);

CPIXPY(t,simcur)$CPIX(t-1,simcur) = 
  100*(CPIX(t,simcur)/CPIX(t-1,simcur) - 1);

DKAXPY(f,a,t,simcur)$DKAX(f,a,t-1,simcur) = 
  100*(DKAX(f,a,t,simcur)/DKAX(f,a,t-1,simcur) - 1);

DKINSXPY(ins2,f,t,simcur)$DKINSX(ins2,f,t-1,simcur) = 
  100*(DKINSX(ins2,f,t,simcur)/DKINSX(ins2,f,t-1,simcur) - 1);

DPIXPY(t,simcur)$DPIX(t-1,simcur) = 
  100*(DPIX(t,simcur)/DPIX(t-1,simcur) - 1);

EGXPY(t,simcur)$EGX(t-1,simcur) = 
  100*(EGX(t,simcur)/EGX(t-1,simcur) - 1);

EHXPY(h,t,simcur)$EHX(h,t-1,simcur) = 
  100*(EHX(h,t,simcur)/EHX(h,t-1,simcur) - 1);

EXRXPY(t,simcur)$EXRX(t-1,simcur) = 
  100*(EXRX(t,simcur)/EXRX(t-1,simcur) - 1);

FBORXPY(ins2,t,simcur)$FBORX(ins2,t-1,simcur) = 
  100*(FBORX(ins2,t,simcur)/FBORX(ins2,t-1,simcur) - 1);

FDEBTXPY(ins2,t,simcur)$FDEBTX(ins2,t-1,simcur) = 
  100*(FDEBTX(ins2,t,simcur)/FDEBTX(ins2,t-1,simcur) - 1);

FDISCALXPY(t,simcur)$FDISCALX(t-1,simcur) = 
  100*(FDISCALX(t,simcur)/FDISCALX(t-1,simcur) - 1);

FPRDAXPY(f,a,t,simcur)$FPRDAX(f,a,t-1,simcur) = 
  100*(FPRDAX(f,a,t,simcur)/FPRDAX(f,a,t-1,simcur) - 1);

GBORXPY(t,simcur)$GBORX(t-1,simcur) = 
  100*(GBORX(t,simcur)/GBORX(t-1,simcur) - 1);

GDEBTXPY(t,simcur)$GDEBTX(t-1,simcur) = 
  100*(GDEBTX(t,simcur)/GDEBTX(t-1,simcur) - 1);

GDPMPXPY(t,simcur)$GDPMPX(t-1,simcur) = 
  100*(GDPMPX(t,simcur)/GDPMPX(t-1,simcur) - 1);

GOVRECABSXPY(acgovrec,t,simcur)$GOVRECABSX(acgovrec,t-1,simcur) = 
  100*(GOVRECABSX(acgovrec,t,simcur)/GOVRECABSX(acgovrec,t-1,simcur) - 1);

GOVRECGDPXPY(acgovrec,t,simcur)$GOVRECGDPX(acgovrec,t-1,simcur) = 
  100*(GOVRECGDPX(acgovrec,t,simcur)/GOVRECGDPX(acgovrec,t-1,simcur) - 1);

GOVSPNDABSXPY(acgovspnd,t,simcur)$GOVSPNDABSX(acgovspnd,t-1,simcur) = 
  100*(GOVSPNDABSX(acgovspnd,t,simcur)/GOVSPNDABSX(acgovspnd,t-1,simcur) - 1);

GOVSPNDGDPXPY(acgovspnd,t,simcur)$GOVSPNDGDPX(acgovspnd,t-1,simcur) = 
  100*(GOVSPNDGDPX(acgovspnd,t,simcur)/GOVSPNDGDPX(acgovspnd,t-1,simcur) - 1);

GPRIMDEFXPY(t,simcur)$GPRIMDEFX(t-1,simcur) = 
  100*(GPRIMDEFX(t,simcur)/GPRIMDEFX(t-1,simcur) - 1);

IADJXPY(ins2,t,simcur)$IADJX(ins2,t-1,simcur) = 
  100*(IADJX(ins2,t,simcur)/IADJX(ins2,t-1,simcur) - 1);

INVVALXPY(t,simcur)$INVVALX(t-1,simcur) = 
  100*(INVVALX(t,simcur)/INVVALX(t-1,simcur) - 1);

INVVALFXPY(t,simcur)$INVVALFX(t-1,simcur) = 
  100*(INVVALFX(t,simcur)/INVVALFX(t-1,simcur) - 1);

INVVALGXPY(t,simcur)$INVVALGX(t-1,simcur) = 
  100*(INVVALGX(t,simcur)/INVVALGX(t-1,simcur) - 1);

ISCALXPY(ac,t,simcur)$ISCALX(ac,t-1,simcur) = 
  100*(ISCALX(ac,t,simcur)/ISCALX(ac,t-1,simcur) - 1);

LABPARTRATXPY(t,simcur)$LABPARTRATX(t-1,simcur) = 
  100*(LABPARTRATX(t,simcur)/LABPARTRATX(t-1,simcur) - 1);

MPSXPY(ins,t,simcur)$MPSX(ins,t-1,simcur) = 
  100*(MPSX(ins,t,simcur)/MPSX(ins,t-1,simcur) - 1);

MPSADJXPY(t,simcur)$MPSADJX(t-1,simcur) = 
  100*(MPSADJX(t,simcur)/MPSADJX(t-1,simcur) - 1);

MPSSCALXPY(t,simcur)$MPSSCALX(t-1,simcur) = 
  100*(MPSSCALX(t,simcur)/MPSSCALX(t-1,simcur) - 1);

NDFGXPY(t,simcur)$NDFGX(t-1,simcur) = 
  100*(NDFGX(t,simcur)/NDFGX(t-1,simcur) - 1);

NFFGXPY(t,simcur)$NFFGX(t-1,simcur) = 
  100*(NFFGX(t,simcur)/NFFGX(t-1,simcur) - 1);

NFFINSXPY(t,simcur)$NFFINSX(t-1,simcur) = 
  100*(NFFINSX(t,simcur)/NFFINSX(t-1,simcur) - 1);

NFFINSSCALXPY(t,simcur)$NFFINSSCALX(t-1,simcur) = 
  100*(NFFINSSCALX(t,simcur)/NFFINSSCALX(t-1,simcur) - 1);

NGOVPAYABSXPY(acngovpay,t,simcur)$NGOVPAYABSX(acngovpay,t-1,simcur) = 
  100*(NGOVPAYABSX(acngovpay,t,simcur)/NGOVPAYABSX(acngovpay,t-1,simcur) - 1);

NGOVPAYGDPXPY(acngovpay,t,simcur)$NGOVPAYGDPX(acngovpay,t-1,simcur) = 
  100*(NGOVPAYGDPX(acngovpay,t,simcur)/NGOVPAYGDPX(acngovpay,t-1,simcur) - 1);

PAXPY(a,t,simcur)$PAX(a,t-1,simcur) = 
  100*(PAX(a,t,simcur)/PAX(a,t-1,simcur) - 1);

PDDXPY(c,t,simcur)$PDDX(c,t-1,simcur) = 
  100*(PDDX(c,t,simcur)/PDDX(c,t-1,simcur) - 1);

PDSXPY(c,t,simcur)$PDSX(c,t-1,simcur) = 
  100*(PDSX(c,t,simcur)/PDSX(c,t-1,simcur) - 1);

PEXPY(c,t,simcur)$PEX(c,t-1,simcur) = 
  100*(PEX(c,t,simcur)/PEX(c,t-1,simcur) - 1);

PKXPY(fcap,t,simcur)$PKX(fcap,t-1,simcur) = 
  100*(PKX(fcap,t,simcur)/PKX(fcap,t-1,simcur) - 1);

PMXPY(c,t,simcur)$PMX(c,t-1,simcur) = 
  100*(PMX(c,t,simcur)/PMX(c,t-1,simcur) - 1);

PQDXPY(c,ac,t,simcur)$PQDX(c,ac,t-1,simcur) = 
  100*(PQDX(c,ac,t,simcur)/PQDX(c,ac,t-1,simcur) - 1);

PQSXPY(c,t,simcur)$PQSX(c,t-1,simcur) = 
  100*(PQSX(c,t,simcur)/PQSX(c,t-1,simcur) - 1);

PVAXPY(a,t,simcur)$PVAX(a,t-1,simcur) = 
  100*(PVAX(a,t,simcur)/PVAX(a,t-1,simcur) - 1);

PXXPY(c,t,simcur)$PXX(c,t-1,simcur) = 
  100*(PXX(c,t,simcur)/PXX(c,t-1,simcur) - 1);

QAXPY(a,t,simcur)$QAX(a,t-1,simcur) = 
  100*(QAX(a,t,simcur)/QAX(a,t-1,simcur) - 1);

QDXPY(c,t,simcur)$QDX(c,t-1,simcur) = 
  100*(QDX(c,t,simcur)/QDX(c,t-1,simcur) - 1);

QEXPY(c,t,simcur)$QEX(c,t-1,simcur) = 
  100*(QEX(c,t,simcur)/QEX(c,t-1,simcur) - 1);

QFXPY(ac,acp,t,simcur)$QFX(ac,acp,t-1,simcur) = 
  100*(QFX(ac,acp,t,simcur)/QFX(ac,acp,t-1,simcur) - 1);

QFHENDXPY(h,f,t,simcur)$QFHENDX(h,f,t-1,simcur) = 
  100*(QFHENDX(h,f,t,simcur)/QFHENDX(h,f,t-1,simcur) - 1);

QFHENDSCALXPY(f,t,simcur)$QFHENDSCALX(f,t-1,simcur) = 
  100*(QFHENDSCALX(f,t,simcur)/QFHENDSCALX(f,t-1,simcur) - 1);

QFINSXPY(ins,ac,t,simcur)$QFINSX(ins,ac,t-1,simcur) = 
  100*(QFINSX(ins,ac,t,simcur)/QFINSX(ins,ac,t-1,simcur) - 1);

QFINSSCALXPY(f,t,simcur)$QFINSSCALX(f,t-1,simcur) = 
  100*(QFINSSCALX(f,t,simcur)/QFINSSCALX(f,t-1,simcur) - 1);

QFSXPY(f,t,simcur)$QFSX(f,t-1,simcur) = 
  100*(QFSX(f,t,simcur)/QFSX(f,t-1,simcur) - 1);

QGXPY(c,t,simcur)$QGX(c,t-1,simcur) = 
  100*(QGX(c,t,simcur)/QGX(c,t-1,simcur) - 1);

QGSCALXPY(t,simcur)$QGSCALX(t-1,simcur) = 
  100*(QGSCALX(t,simcur)/QGSCALX(t-1,simcur) - 1);

QHXPY(ac,h,t,simcur)$QHX(ac,h,t-1,simcur) = 
  100*(QHX(ac,h,t,simcur)/QHX(ac,h,t-1,simcur) - 1);

QINTXPY(c,a,t,simcur)$QINTX(c,a,t-1,simcur) = 
  100*(QINTX(c,a,t,simcur)/QINTX(c,a,t-1,simcur) - 1);

QINVXPY(c,t,simcur)$QINVX(c,t-1,simcur) = 
  100*(QINVX(c,t,simcur)/QINVX(c,t-1,simcur) - 1);

QLABSCALXPY(t,simcur)$QLABSCALX(t-1,simcur) = 
  100*(QLABSCALX(t,simcur)/QLABSCALX(t-1,simcur) - 1);

QMXPY(c,t,simcur)$QMX(c,t-1,simcur) = 
  100*(QMX(c,t,simcur)/QMX(c,t-1,simcur) - 1);

QNGOXPY(c,insngo,t,simcur)$QNGOX(c,insngo,t-1,simcur) = 
  100*(QNGOX(c,insngo,t,simcur)/QNGOX(c,insngo,t-1,simcur) - 1);

QNGOSCALXPY(insngo,t,simcur)$QNGOSCALX(insngo,t-1,simcur) = 
  100*(QNGOSCALX(insngo,t,simcur)/QNGOSCALX(insngo,t-1,simcur) - 1);

QQXPY(c,t,simcur)$QQX(c,t-1,simcur) = 
  100*(QQX(c,t,simcur)/QQX(c,t-1,simcur) - 1);

QTXPY(c,t,simcur)$QTX(c,t-1,simcur) = 
  100*(QTX(c,t,simcur)/QTX(c,t-1,simcur) - 1);

QTRSTXPY(c,instrst,t,simcur)$QTRSTX(c,instrst,t-1,simcur) = 
  100*(QTRSTX(c,instrst,t,simcur)/QTRSTX(c,instrst,t-1,simcur) - 1);

QTRSTSCALXPY(t,simcur)$QTRSTSCALX(t-1,simcur) = 
  100*(QTRSTSCALX(t,simcur)/QTRSTSCALX(t-1,simcur) - 1);

QXXPY(c,t,simcur)$QXX(c,t-1,simcur) = 
  100*(QXX(c,t,simcur)/QXX(c,t-1,simcur) - 1);

RBTVATXPY(a,t,simcur)$RBTVATX(a,t-1,simcur) = 
  100*(RBTVATX(a,t,simcur)/RBTVATX(a,t-1,simcur) - 1);

REXRXPY(t,simcur)$REXRX(t-1,simcur) = 
  100*(REXRX(t,simcur)/REXRX(t-1,simcur) - 1);

RGDPFCXPY(t,simcur)$RGDPFCX(t-1,simcur) = 
  100*(RGDPFCX(t,simcur)/RGDPFCX(t-1,simcur) - 1);

RGDPMPXPY(t,simcur)$RGDPMPX(t-1,simcur) = 
  100*(RGDPMPX(t,simcur)/RGDPMPX(t-1,simcur) - 1);

RGPRIMDEFXPY(t,simcur)$RGPRIMDEFX(t-1,simcur) = 
  100*(RGPRIMDEFX(t,simcur)/RGPRIMDEFX(t-1,simcur) - 1);

RNDFGXPY(t,simcur)$RNDFGX(t-1,simcur) = 
  100*(RNDFGX(t,simcur)/RNDFGX(t-1,simcur) - 1);

SAVXPY(ac,t,simcur)$SAVX(ac,t-1,simcur) = 
  100*(SAVX(ac,t,simcur)/SAVX(ac,t-1,simcur) - 1);

SAVFXPY(t,simcur)$SAVFX(t-1,simcur) = 
  100*(SAVFX(t,simcur)/SAVFX(t-1,simcur) - 1);

SHIFXPY(ins,f,t,simcur)$SHIFX(ins,f,t-1,simcur) = 
  100*(SHIFX(ins,f,t,simcur)/SHIFX(ins,f,t-1,simcur) - 1);

SUBCXPY(c,ac,t,simcur)$SUBCX(c,ac,t-1,simcur) = 
  100*(SUBCX(c,ac,t,simcur)/SUBCX(c,ac,t-1,simcur) - 1);

SUBCSCALXPY(t,simcur)$SUBCSCALX(t-1,simcur) = 
  100*(SUBCSCALX(t,simcur)/SUBCSCALX(t-1,simcur) - 1);

SUBCTXPY(t,simcur)$SUBCTX(t-1,simcur) = 
  100*(SUBCTX(t,simcur)/SUBCTX(t-1,simcur) - 1);

TAXPY(a,t,simcur)$TAX(a,t-1,simcur) = 
  100*(TAX(a,t,simcur)/TAX(a,t-1,simcur) - 1);

TASCALXPY(t,simcur)$TASCALX(t-1,simcur) = 
  100*(TASCALX(t,simcur)/TASCALX(t-1,simcur) - 1);

TEXPY(c,t,simcur)$TEX(c,t-1,simcur) = 
  100*(TEX(c,t,simcur)/TEX(c,t-1,simcur) - 1);

TESCALXPY(t,simcur)$TESCALX(t-1,simcur) = 
  100*(TESCALX(t,simcur)/TESCALX(t-1,simcur) - 1);

TFXPY(f,t,simcur)$TFX(f,t-1,simcur) = 
  100*(TFX(f,t,simcur)/TFX(f,t-1,simcur) - 1);

TFAXPY(f,a,t,simcur)$TFAX(f,a,t-1,simcur) = 
  100*(TFAX(f,a,t,simcur)/TFAX(f,a,t-1,simcur) - 1);

TFASCALXPY(t,simcur)$TFASCALX(t-1,simcur) = 
  100*(TFASCALX(t,simcur)/TFASCALX(t-1,simcur) - 1);

TFPXPY(a,t,simcur)$TFPX(a,t-1,simcur) = 
  100*(TFPX(a,t,simcur)/TFPX(a,t-1,simcur) - 1);

TFPSCALXPY(t,simcur)$TFPSCALX(t-1,simcur) = 
  100*(TFPSCALX(t,simcur)/TFPSCALX(t-1,simcur) - 1);

TFSCALXPY(t,simcur)$TFSCALX(t-1,simcur) = 
  100*(TFSCALX(t,simcur)/TFSCALX(t-1,simcur) - 1);

TMXPY(c,t,simcur)$TMX(c,t-1,simcur) = 
  100*(TMX(c,t,simcur)/TMX(c,t-1,simcur) - 1);

TMSCALXPY(t,simcur)$TMSCALX(t-1,simcur) = 
  100*(TMSCALX(t,simcur)/TMSCALX(t-1,simcur) - 1);

TQXPY(c,t,simcur)$TQX(c,t-1,simcur) = 
  100*(TQX(c,t,simcur)/TQX(c,t-1,simcur) - 1);

TQSCALXPY(t,simcur)$TQSCALX(t-1,simcur) = 
  100*(TQSCALX(t,simcur)/TQSCALX(t-1,simcur) - 1);

TRDGDPXPY(t,simcur)$TRDGDPX(t-1,simcur) = 
  100*(TRDGDPX(t,simcur)/TRDGDPX(t-1,simcur) - 1);

TRIIXPY(ins,insp,t,simcur)$TRIIX(ins,insp,t-1,simcur) = 
  100*(TRIIX(ins,insp,t,simcur)/TRIIX(ins,insp,t-1,simcur) - 1);

TRNSFRXPY(ac,acp,t,simcur)$TRNSFRX(ac,acp,t-1,simcur) = 
  100*(TRNSFRX(ac,acp,t,simcur)/TRNSFRX(ac,acp,t-1,simcur) - 1);

TRNSFRSCALXPY(ac,t,simcur)$TRNSFRSCALX(ac,t-1,simcur) = 
  100*(TRNSFRSCALX(ac,t,simcur)/TRNSFRSCALX(ac,t-1,simcur) - 1);

TRSMRECXPY(t,simcur)$TRSMRECX(t-1,simcur) = 
  100*(TRSMRECX(t,simcur)/TRSMRECX(t-1,simcur) - 1);

TVACXPY(c,ac,t,simcur)$TVACX(c,ac,t-1,simcur) = 
  100*(TVACX(c,ac,t,simcur)/TVACX(c,ac,t-1,simcur) - 1);

TVACSCALXPY(t,simcur)$TVACSCALX(t-1,simcur) = 
  100*(TVACSCALX(t,simcur)/TVACSCALX(t-1,simcur) - 1);

TYXPY(ins,t,simcur)$TYX(ins,t-1,simcur) = 
  100*(TYX(ins,t,simcur)/TYX(ins,t-1,simcur) - 1);

TYSCALXPY(t,simcur)$TYSCALX(t-1,simcur) = 
  100*(TYSCALX(t,simcur)/TYSCALX(t-1,simcur) - 1);

UERATXPY(ac,t,simcur)$UERATX(ac,t-1,simcur) = 
  100*(UERATX(ac,t,simcur)/UERATX(ac,t-1,simcur) - 1);

WALRASXPY(t,simcur)$WALRASX(t-1,simcur) = 
  100*(WALRASX(t,simcur)/WALRASX(t-1,simcur) - 1);

WFXPY(f,t,simcur)$WFX(f,t-1,simcur) = 
  100*(WFX(f,t,simcur)/WFX(f,t-1,simcur) - 1);

WFAXPY(f,a,t,simcur)$WFAX(f,a,t-1,simcur) = 
  100*(WFAX(f,a,t,simcur)/WFAX(f,a,t-1,simcur) - 1);

WFAVGXPY(ac,t,simcur)$WFAVGX(ac,t-1,simcur) = 
  100*(WFAVGX(ac,t,simcur)/WFAVGX(ac,t-1,simcur) - 1);

WFDISTXPY(f,a,t,simcur)$WFDISTX(f,a,t-1,simcur) = 
  100*(WFDISTX(f,a,t,simcur)/WFDISTX(f,a,t-1,simcur) - 1);

YFXPY(f,t,simcur)$YFX(f,t-1,simcur) = 
  100*(YFX(f,t,simcur)/YFX(f,t-1,simcur) - 1);

YGXPY(t,simcur)$YGX(t-1,simcur) = 
  100*(YGX(t,simcur)/YGX(t-1,simcur) - 1);

YIXPY(ins,t,simcur)$YIX(ins,t-1,simcur) = 
  100*(YIX(ins,t,simcur)/YIX(ins,t-1,simcur) - 1);

YIFXPY(ins,f,t,simcur)$YIFX(ins,f,t-1,simcur) = 
  100*(YIFX(ins,f,t,simcur)/YIFX(ins,f,t-1,simcur) - 1);

YTAXIMPXPY(t,simcur)$YTAXIMPX(t-1,simcur) = 
  100*(YTAXIMPX(t,simcur)/YTAXIMPX(t-1,simcur) - 1);

YTAXEXPXPY(t,simcur)$YTAXEXPX(t-1,simcur) = 
  100*(YTAXEXPX(t,simcur)/YTAXEXPX(t-1,simcur) - 1);

YTAXVATXPY(t,simcur)$YTAXVATX(t-1,simcur) = 
  100*(YTAXVATX(t,simcur)/YTAXVATX(t-1,simcur) - 1);



popXPY(ac,t,simcur)$popX(ac,t-1,simcur) = 
  100*(popX(ac,t,simcur)/popX(ac,t-1,simcur) - 1);

pweXPY(c,t,simcur)$pweX(c,t-1,simcur) = 
  100*(pweX(c,t,simcur)/pweX(c,t-1,simcur) - 1);

pwmXPY(c,t,simcur)$pwmX(c,t-1,simcur) = 
  100*(pwmX(c,t,simcur)/pwmX(c,t-1,simcur) - 1);

*qa2XPY(a,t,simcur)$qa2X(a,t-1,simcur) = 
*  100*(qa2X(a,t,simcur)/qa2X(a,t-1,simcur) - 1);

qgbXPY(c,t,simcur)$qgbX(c,t-1,simcur) = 
  100*(qgbX(c,t,simcur)/qgbX(c,t-1,simcur) - 1);

qhpcXPY(ac,t,simcur)$qhpcX(ac,t-1,simcur) = 
  100*(qhpcX(ac,t,simcur)/qhpcX(ac,t-1,simcur) - 1);

yhpcXPY(ac,t,simcur)$yhpcX(ac,t-1,simcur) = 
  100*(yhpcX(ac,t,simcur)/yhpcX(ac,t-1,simcur) - 1);

rsavgXPY(t,simcur)$rsavgX(t-1,simcur) = 
  100*(rsavgX(t,simcur)/rsavgX(t-1,simcur) - 1);

savgXPY(t,simcur)$savgX(t-1,simcur) = 
  100*(savgX(t,simcur)/savgX(t-1,simcur) - 1);

*tfpaXPY(a,t,simcur)$tfpaX(a,t-1,simcur) = 
*  100*(tfpaX(a,t,simcur)/tfpaX(a,t-1,simcur) - 1);

*tfpatotXPY(t,simcur)$tfpatotX(t-1,simcur) = 
*  100*(tfpatotX(t,simcur)/tfpatotX(t-1,simcur) - 1);

wfarealXPY(f,a,t,simcur)$wfarealX(f,a,t-1,simcur) = 
  100*(wfarealX(f,a,t,simcur)/wfarealX(f,a,t-1,simcur) - 1);

wfrealXPY(f,t,simcur)$wfrealX(f,t-1,simcur) = 
  100*(wfrealX(f,t,simcur)/wfrealX(f,t-1,simcur) - 1);

shvatrbtXPY(c,a,t,simcur)$shvatrbtX(c,a,t-1,simcur) = 
  100*(shvatrbtX(c,a,t,simcur)/shvatrbtX(c,a,t-1,simcur) - 1);

gdpindicXPY(igdp,kgdp,t,simcur)$gdpindic(igdp,kgdp,t-1,simcur) = 
  100*(gdpindic(igdp,kgdp,t,simcur)/gdpindic(igdp,kgdp,t-1,simcur) - 1);

gdpexpindicXPY(maccol,kgdp,t,simcur)$gdpexpindic(maccol,kgdp,t-1,simcur) = 
  100*(gdpexpindic(maccol,kgdp,t,simcur)/gdpexpindic(maccol,kgdp,t-1,simcur) - 1);

sectorindicXPY(ac,sectorcol3,kgdp,t,simcur)$sectorindic(ac,sectorcol3,kgdp,t-1,simcur) = 
  100*(sectorindic(ac,sectorcol3,kgdp,t,simcur)/sectorindic(ac,sectorcol3,kgdp,t-1,simcur) - 1);

sectorindic2XPY(acrep,sectorcol3,kgdp,t,simcur)$sectorindic2(acrep,sectorcol3,kgdp,t-1,simcur) = 
  100*(sectorindic2(acrep,sectorcol3,kgdp,t,simcur)/sectorindic2(acrep,sectorcol3,kgdp,t-1,simcur) - 1);
  
emiindicXPY(emicol,ac,acp,t,simcur)$emiindic(emicol,ac,acp,t-1,simcur) = 
  100*(emiindic(emicol,ac,acp,t,simcur)/emiindic(emicol,ac,acp,t-1,simcur) - 1);

emiindic2XPY(emicol,acrep,acrepp,t,simcur)$emiindic2(emicol,acrep,acrepp,t-1,simcur) = 
  100*(emiindic2(emicol,acrep,acrepp,t,simcur)/emiindic2(emicol,acrep,acrepp,t-1,simcur) - 1);
  
DISPLAY "#### END: repperc-xp.inc";  