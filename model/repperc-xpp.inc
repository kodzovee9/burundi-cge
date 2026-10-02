* repperc-pp.inc

DISPLAY "#### START: repperc-xpp.inc";

PREXRXPP(simcur)$((SUM(tmin, PREXRX(tmin,simcur))>0 AND SUM(tmax, PREXRX(tmax,simcur))>0) 
  OR (SUM(tmin, PREXRX(tmin,simcur))<0 AND SUM(tmax, PREXRX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PREXRX(tmax,simcur)) / SUM(tmin, PREXRX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

YPREXRTXPP(c,simcur)$((SUM(tmin, YPREXRTX(c,tmin,simcur))>0 AND SUM(tmax, YPREXRTX(c,tmax,simcur))>0) 
  OR (SUM(tmin, YPREXRTX(c,tmin,simcur))<0 AND SUM(tmax, YPREXRTX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YPREXRTX(c,tmax,simcur)) / SUM(tmin, YPREXRTX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

YPREXRXPP(c,ac,simcur)$((SUM(tmin, YPREXRX(c,ac,tmin,simcur))>0 AND SUM(tmax, YPREXRX(c,ac,tmax,simcur))>0) 
  OR (SUM(tmin, YPREXRX(c,ac,tmin,simcur))<0 AND SUM(tmax, YPREXRX(c,ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YPREXRX(c,ac,tmax,simcur)) / SUM(tmin, YPREXRX(c,ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PRQMBARXPP(c,simcur)$((SUM(tmin, PRQMBARX(c,tmin,simcur))>0 AND SUM(tmax, PRQMBARX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PRQMBARX(c,tmin,simcur))<0 AND SUM(tmax, PRQMBARX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PRQMBARX(c,tmax,simcur)) / SUM(tmin, PRQMBARX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

YPRQMBARTXPP(c,simcur)$((SUM(tmin, YPRQMBARTX(c,tmin,simcur))>0 AND SUM(tmax, YPRQMBARTX(c,tmax,simcur))>0) 
  OR (SUM(tmin, YPRQMBARTX(c,tmin,simcur))<0 AND SUM(tmax, YPRQMBARTX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YPRQMBARTX(c,tmax,simcur)) / SUM(tmin, YPRQMBARTX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

YPRQMBARXPP(c,ac,simcur)$((SUM(tmin, YPRQMBARX(c,ac,tmin,simcur))>0 AND SUM(tmax, YPRQMBARX(c,ac,tmax,simcur))>0) 
  OR (SUM(tmin, YPRQMBARX(c,ac,tmin,simcur))<0 AND SUM(tmax, YPRQMBARX(c,ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YPRQMBARX(c,ac,tmax,simcur)) / SUM(tmin, YPRQMBARX(c,ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );



PWEXPP(c,simcur)$((SUM(tmin, PWEX(c,tmin,simcur))>0 AND SUM(tmax, PWEX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PWEX(c,tmin,simcur))<0 AND SUM(tmax, PWEX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PWEX(c,tmax,simcur)) / SUM(tmin, PWEX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PWMXPP(c,simcur)$((SUM(tmin, PWMX(c,tmin,simcur))>0 AND SUM(tmax, PWMX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PWMX(c,tmin,simcur))<0 AND SUM(tmax, PWMX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PWMX(c,tmax,simcur)) / SUM(tmin, PWMX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );



QXACXPP(a,c,simcur)$((SUM(tmin, QXACX(a,c,tmin,simcur))>0 AND SUM(tmax, QXACX(a,c,tmax,simcur))>0) 
  OR (SUM(tmin, QXACX(a,c,tmin,simcur))<0 AND SUM(tmax, QXACX(a,c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QXACX(a,c,tmax,simcur)) / SUM(tmin, QXACX(a,c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PXACXPP(a,c,simcur)$((SUM(tmin, PXACX(a,c,tmin,simcur))>0 AND SUM(tmax, PXACX(a,c,tmax,simcur))>0) 
  OR (SUM(tmin, PXACX(a,c,tmin,simcur))<0 AND SUM(tmax, PXACX(a,c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PXACX(a,c,tmax,simcur)) / SUM(tmin, PXACX(a,c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QEMIXPP(ghg,ac,acp,simcur)$((SUM(tmin, QEMIX(ghg,ac,acp,tmin,simcur))>0 AND SUM(tmax, QEMIX(ghg,ac,acp,tmax,simcur))>0) 
  OR (SUM(tmin, QEMIX(ghg,ac,acp,tmin,simcur))<0 AND SUM(tmax, QEMIX(ghg,ac,acp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QEMIX(ghg,ac,acp,tmax,simcur)) / SUM(tmin, QEMIX(ghg,ac,acp,tmin,simcur)) ) ** (1/tsolnb) - 1 );



ABSNOMXPP(simcur)$((SUM(tmin, ABSNOMX(tmin,simcur))>0 AND SUM(tmax, ABSNOMX(tmax,simcur))>0) 
  OR (SUM(tmin, ABSNOMX(tmin,simcur))<0 AND SUM(tmax, ABSNOMX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, ABSNOMX(tmax,simcur)) / SUM(tmin, ABSNOMX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

CPIXPP(simcur)$((SUM(tmin, CPIX(tmin,simcur))>0 AND SUM(tmax, CPIX(tmax,simcur))>0) 
  OR (SUM(tmin, CPIX(tmin,simcur))<0 AND SUM(tmax, CPIX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, CPIX(tmax,simcur)) / SUM(tmin, CPIX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

DKAXPP(f,a,simcur)$((SUM(tmin, DKAX(f,a,tmin,simcur))>0 AND SUM(tmax, DKAX(f,a,tmax,simcur))>0) 
  OR (SUM(tmin, DKAX(f,a,tmin,simcur))<0 AND SUM(tmax, DKAX(f,a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, DKAX(f,a,tmax,simcur)) / SUM(tmin, DKAX(f,a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

DKINSXPP(ins2,f,simcur)$((SUM(tmin, DKINSX(ins2,f,tmin,simcur))>0 AND SUM(tmax, DKINSX(ins2,f,tmax,simcur))>0) 
  OR (SUM(tmin, DKINSX(ins2,f,tmin,simcur))<0 AND SUM(tmax, DKINSX(ins2,f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, DKINSX(ins2,f,tmax,simcur)) / SUM(tmin, DKINSX(ins2,f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

DPIXPP(simcur)$((SUM(tmin, DPIX(tmin,simcur))>0 AND SUM(tmax, DPIX(tmax,simcur))>0) 
  OR (SUM(tmin, DPIX(tmin,simcur))<0 AND SUM(tmax, DPIX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, DPIX(tmax,simcur)) / SUM(tmin, DPIX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

EGXPP(simcur)$((SUM(tmin, EGX(tmin,simcur))>0 AND SUM(tmax, EGX(tmax,simcur))>0) 
  OR (SUM(tmin, EGX(tmin,simcur))<0 AND SUM(tmax, EGX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, EGX(tmax,simcur)) / SUM(tmin, EGX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

EHXPP(h,simcur)$((SUM(tmin, EHX(h,tmin,simcur))>0 AND SUM(tmax, EHX(h,tmax,simcur))>0) 
  OR (SUM(tmin, EHX(h,tmin,simcur))<0 AND SUM(tmax, EHX(h,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, EHX(h,tmax,simcur)) / SUM(tmin, EHX(h,tmin,simcur)) ) ** (1/tsolnb) - 1 );

EXRXPP(simcur)$((SUM(tmin, EXRX(tmin,simcur))>0 AND SUM(tmax, EXRX(tmax,simcur))>0) 
  OR (SUM(tmin, EXRX(tmin,simcur))<0 AND SUM(tmax, EXRX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, EXRX(tmax,simcur)) / SUM(tmin, EXRX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

FBORXPP(ins2,simcur)$((SUM(tmin, FBORX(ins2,tmin,simcur))>0 AND SUM(tmax, FBORX(ins2,tmax,simcur))>0) 
  OR (SUM(tmin, FBORX(ins2,tmin,simcur))<0 AND SUM(tmax, FBORX(ins2,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, FBORX(ins2,tmax,simcur)) / SUM(tmin, FBORX(ins2,tmin,simcur)) ) ** (1/tsolnb) - 1 );

FDEBTXPP(ins2,simcur)$((SUM(tmin, FDEBTX(ins2,tmin,simcur))>0 AND SUM(tmax, FDEBTX(ins2,tmax,simcur))>0) 
  OR (SUM(tmin, FDEBTX(ins2,tmin,simcur))<0 AND SUM(tmax, FDEBTX(ins2,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, FDEBTX(ins2,tmax,simcur)) / SUM(tmin, FDEBTX(ins2,tmin,simcur)) ) ** (1/tsolnb) - 1 );

FDISCALXPP(simcur)$((SUM(tmin, FDISCALX(tmin,simcur))>0 AND SUM(tmax, FDISCALX(tmax,simcur))>0) 
  OR (SUM(tmin, FDISCALX(tmin,simcur))<0 AND SUM(tmax, FDISCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, FDISCALX(tmax,simcur)) / SUM(tmin, FDISCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

FPRDAXPP(f,a,simcur)$((SUM(tmin, FPRDAX(f,a,tmin,simcur))>0 AND SUM(tmax, FPRDAX(f,a,tmax,simcur))>0) 
  OR (SUM(tmin, FPRDAX(f,a,tmin,simcur))<0 AND SUM(tmax, FPRDAX(f,a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, FPRDAX(f,a,tmax,simcur)) / SUM(tmin, FPRDAX(f,a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

GBORXPP(simcur)$((SUM(tmin, GBORX(tmin,simcur))>0 AND SUM(tmax, GBORX(tmax,simcur))>0) 
  OR (SUM(tmin, GBORX(tmin,simcur))<0 AND SUM(tmax, GBORX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, GBORX(tmax,simcur)) / SUM(tmin, GBORX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

GDEBTXPP(simcur)$((SUM(tmin, GDEBTX(tmin,simcur))>0 AND SUM(tmax, GDEBTX(tmax,simcur))>0) 
  OR (SUM(tmin, GDEBTX(tmin,simcur))<0 AND SUM(tmax, GDEBTX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, GDEBTX(tmax,simcur)) / SUM(tmin, GDEBTX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

GDPMPXPP(simcur)$((SUM(tmin, GDPMPX(tmin,simcur))>0 AND SUM(tmax, GDPMPX(tmax,simcur))>0) 
  OR (SUM(tmin, GDPMPX(tmin,simcur))<0 AND SUM(tmax, GDPMPX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, GDPMPX(tmax,simcur)) / SUM(tmin, GDPMPX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

GOVRECABSXPP(acgovrec,simcur)$((SUM(tmin, GOVRECABSX(acgovrec,tmin,simcur))>0 AND SUM(tmax, GOVRECABSX(acgovrec,tmax,simcur))>0) 
  OR (SUM(tmin, GOVRECABSX(acgovrec,tmin,simcur))<0 AND SUM(tmax, GOVRECABSX(acgovrec,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, GOVRECABSX(acgovrec,tmax,simcur)) / SUM(tmin, GOVRECABSX(acgovrec,tmin,simcur)) ) ** (1/tsolnb) - 1 );

GOVRECGDPXPP(acgovrec,simcur)$((SUM(tmin, GOVRECGDPX(acgovrec,tmin,simcur))>0 AND SUM(tmax, GOVRECGDPX(acgovrec,tmax,simcur))>0) 
  OR (SUM(tmin, GOVRECGDPX(acgovrec,tmin,simcur))<0 AND SUM(tmax, GOVRECGDPX(acgovrec,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, GOVRECGDPX(acgovrec,tmax,simcur)) / SUM(tmin, GOVRECGDPX(acgovrec,tmin,simcur)) ) ** (1/tsolnb) - 1 );

GOVSPNDABSXPP(acgovspnd,simcur)$((SUM(tmin, GOVSPNDABSX(acgovspnd,tmin,simcur))>0 AND SUM(tmax, GOVSPNDABSX(acgovspnd,tmax,simcur))>0) 
  OR (SUM(tmin, GOVSPNDABSX(acgovspnd,tmin,simcur))<0 AND SUM(tmax, GOVSPNDABSX(acgovspnd,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, GOVSPNDABSX(acgovspnd,tmax,simcur)) / SUM(tmin, GOVSPNDABSX(acgovspnd,tmin,simcur)) ) ** (1/tsolnb) - 1 );

GOVSPNDGDPXPP(acgovspnd,simcur)$((SUM(tmin, GOVSPNDGDPX(acgovspnd,tmin,simcur))>0 AND SUM(tmax, GOVSPNDGDPX(acgovspnd,tmax,simcur))>0) 
  OR (SUM(tmin, GOVSPNDGDPX(acgovspnd,tmin,simcur))<0 AND SUM(tmax, GOVSPNDGDPX(acgovspnd,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, GOVSPNDGDPX(acgovspnd,tmax,simcur)) / SUM(tmin, GOVSPNDGDPX(acgovspnd,tmin,simcur)) ) ** (1/tsolnb) - 1 );

GPRIMDEFXPP(simcur)$((SUM(tmin, GPRIMDEFX(tmin,simcur))>0 AND SUM(tmax, GPRIMDEFX(tmax,simcur))>0) 
  OR (SUM(tmin, GPRIMDEFX(tmin,simcur))<0 AND SUM(tmax, GPRIMDEFX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, GPRIMDEFX(tmax,simcur)) / SUM(tmin, GPRIMDEFX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

IADJXPP(ins2,simcur)$((SUM(tmin, IADJX(ins2,tmin,simcur))>0 AND SUM(tmax, IADJX(ins2,tmax,simcur))>0) 
  OR (SUM(tmin, IADJX(ins2,tmin,simcur))<0 AND SUM(tmax, IADJX(ins2,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, IADJX(ins2,tmax,simcur)) / SUM(tmin, IADJX(ins2,tmin,simcur)) ) ** (1/tsolnb) - 1 );

INVVALXPP(simcur)$((SUM(tmin, INVVALX(tmin,simcur))>0 AND SUM(tmax, INVVALX(tmax,simcur))>0) 
  OR (SUM(tmin, INVVALX(tmin,simcur))<0 AND SUM(tmax, INVVALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, INVVALX(tmax,simcur)) / SUM(tmin, INVVALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

INVVALFXPP(simcur)$((SUM(tmin, INVVALFX(tmin,simcur))>0 AND SUM(tmax, INVVALFX(tmax,simcur))>0) 
  OR (SUM(tmin, INVVALFX(tmin,simcur))<0 AND SUM(tmax, INVVALFX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, INVVALFX(tmax,simcur)) / SUM(tmin, INVVALFX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

INVVALGXPP(simcur)$((SUM(tmin, INVVALGX(tmin,simcur))>0 AND SUM(tmax, INVVALGX(tmax,simcur))>0) 
  OR (SUM(tmin, INVVALGX(tmin,simcur))<0 AND SUM(tmax, INVVALGX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, INVVALGX(tmax,simcur)) / SUM(tmin, INVVALGX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

ISCALXPP(ac,simcur)$((SUM(tmin, ISCALX(ac,tmin,simcur))>0 AND SUM(tmax, ISCALX(ac,tmax,simcur))>0) 
  OR (SUM(tmin, ISCALX(ac,tmin,simcur))<0 AND SUM(tmax, ISCALX(ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, ISCALX(ac,tmax,simcur)) / SUM(tmin, ISCALX(ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

LABPARTRATXPP(simcur)$((SUM(tmin, LABPARTRATX(tmin,simcur))>0 AND SUM(tmax, LABPARTRATX(tmax,simcur))>0) 
  OR (SUM(tmin, LABPARTRATX(tmin,simcur))<0 AND SUM(tmax, LABPARTRATX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, LABPARTRATX(tmax,simcur)) / SUM(tmin, LABPARTRATX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

MPSXPP(ins,simcur)$((SUM(tmin, MPSX(ins,tmin,simcur))>0 AND SUM(tmax, MPSX(ins,tmax,simcur))>0) 
  OR (SUM(tmin, MPSX(ins,tmin,simcur))<0 AND SUM(tmax, MPSX(ins,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, MPSX(ins,tmax,simcur)) / SUM(tmin, MPSX(ins,tmin,simcur)) ) ** (1/tsolnb) - 1 );

MPSADJXPP(simcur)$((SUM(tmin, MPSADJX(tmin,simcur))>0 AND SUM(tmax, MPSADJX(tmax,simcur))>0) 
  OR (SUM(tmin, MPSADJX(tmin,simcur))<0 AND SUM(tmax, MPSADJX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, MPSADJX(tmax,simcur)) / SUM(tmin, MPSADJX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

MPSSCALXPP(simcur)$((SUM(tmin, MPSSCALX(tmin,simcur))>0 AND SUM(tmax, MPSSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, MPSSCALX(tmin,simcur))<0 AND SUM(tmax, MPSSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, MPSSCALX(tmax,simcur)) / SUM(tmin, MPSSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

NDFGXPP(simcur)$((SUM(tmin, NDFGX(tmin,simcur))>0 AND SUM(tmax, NDFGX(tmax,simcur))>0) 
  OR (SUM(tmin, NDFGX(tmin,simcur))<0 AND SUM(tmax, NDFGX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, NDFGX(tmax,simcur)) / SUM(tmin, NDFGX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

NFFGXPP(simcur)$((SUM(tmin, NFFGX(tmin,simcur))>0 AND SUM(tmax, NFFGX(tmax,simcur))>0) 
  OR (SUM(tmin, NFFGX(tmin,simcur))<0 AND SUM(tmax, NFFGX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, NFFGX(tmax,simcur)) / SUM(tmin, NFFGX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

NFFINSXPP(simcur)$((SUM(tmin, NFFINSX(tmin,simcur))>0 AND SUM(tmax, NFFINSX(tmax,simcur))>0) 
  OR (SUM(tmin, NFFINSX(tmin,simcur))<0 AND SUM(tmax, NFFINSX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, NFFINSX(tmax,simcur)) / SUM(tmin, NFFINSX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

NFFINSSCALXPP(simcur)$((SUM(tmin, NFFINSSCALX(tmin,simcur))>0 AND SUM(tmax, NFFINSSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, NFFINSSCALX(tmin,simcur))<0 AND SUM(tmax, NFFINSSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, NFFINSSCALX(tmax,simcur)) / SUM(tmin, NFFINSSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

NGOVPAYABSXPP(acngovpay,simcur)$((SUM(tmin, NGOVPAYABSX(acngovpay,tmin,simcur))>0 AND SUM(tmax, NGOVPAYABSX(acngovpay,tmax,simcur))>0) 
  OR (SUM(tmin, NGOVPAYABSX(acngovpay,tmin,simcur))<0 AND SUM(tmax, NGOVPAYABSX(acngovpay,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, NGOVPAYABSX(acngovpay,tmax,simcur)) / SUM(tmin, NGOVPAYABSX(acngovpay,tmin,simcur)) ) ** (1/tsolnb) - 1 );

NGOVPAYGDPXPP(acngovpay,simcur)$((SUM(tmin, NGOVPAYGDPX(acngovpay,tmin,simcur))>0 AND SUM(tmax, NGOVPAYGDPX(acngovpay,tmax,simcur))>0) 
  OR (SUM(tmin, NGOVPAYGDPX(acngovpay,tmin,simcur))<0 AND SUM(tmax, NGOVPAYGDPX(acngovpay,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, NGOVPAYGDPX(acngovpay,tmax,simcur)) / SUM(tmin, NGOVPAYGDPX(acngovpay,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PAXPP(a,simcur)$((SUM(tmin, PAX(a,tmin,simcur))>0 AND SUM(tmax, PAX(a,tmax,simcur))>0) 
  OR (SUM(tmin, PAX(a,tmin,simcur))<0 AND SUM(tmax, PAX(a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PAX(a,tmax,simcur)) / SUM(tmin, PAX(a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PDDXPP(c,simcur)$((SUM(tmin, PDDX(c,tmin,simcur))>0 AND SUM(tmax, PDDX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PDDX(c,tmin,simcur))<0 AND SUM(tmax, PDDX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PDDX(c,tmax,simcur)) / SUM(tmin, PDDX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PDSXPP(c,simcur)$((SUM(tmin, PDSX(c,tmin,simcur))>0 AND SUM(tmax, PDSX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PDSX(c,tmin,simcur))<0 AND SUM(tmax, PDSX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PDSX(c,tmax,simcur)) / SUM(tmin, PDSX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PEXPP(c,simcur)$((SUM(tmin, PEX(c,tmin,simcur))>0 AND SUM(tmax, PEX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PEX(c,tmin,simcur))<0 AND SUM(tmax, PEX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PEX(c,tmax,simcur)) / SUM(tmin, PEX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PKXPP(fcap,simcur)$((SUM(tmin, PKX(fcap,tmin,simcur))>0 AND SUM(tmax, PKX(fcap,tmax,simcur))>0) 
  OR (SUM(tmin, PKX(fcap,tmin,simcur))<0 AND SUM(tmax, PKX(fcap,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PKX(fcap,tmax,simcur)) / SUM(tmin, PKX(fcap,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PMXPP(c,simcur)$((SUM(tmin, PMX(c,tmin,simcur))>0 AND SUM(tmax, PMX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PMX(c,tmin,simcur))<0 AND SUM(tmax, PMX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PMX(c,tmax,simcur)) / SUM(tmin, PMX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PQDXPP(c,ac,simcur)$((SUM(tmin, PQDX(c,ac,tmin,simcur))>0 AND SUM(tmax, PQDX(c,ac,tmax,simcur))>0) 
  OR (SUM(tmin, PQDX(c,ac,tmin,simcur))<0 AND SUM(tmax, PQDX(c,ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PQDX(c,ac,tmax,simcur)) / SUM(tmin, PQDX(c,ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PQSXPP(c,simcur)$((SUM(tmin, PQSX(c,tmin,simcur))>0 AND SUM(tmax, PQSX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PQSX(c,tmin,simcur))<0 AND SUM(tmax, PQSX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PQSX(c,tmax,simcur)) / SUM(tmin, PQSX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PVAXPP(a,simcur)$((SUM(tmin, PVAX(a,tmin,simcur))>0 AND SUM(tmax, PVAX(a,tmax,simcur))>0) 
  OR (SUM(tmin, PVAX(a,tmin,simcur))<0 AND SUM(tmax, PVAX(a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PVAX(a,tmax,simcur)) / SUM(tmin, PVAX(a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

PXXPP(c,simcur)$((SUM(tmin, PXX(c,tmin,simcur))>0 AND SUM(tmax, PXX(c,tmax,simcur))>0) 
  OR (SUM(tmin, PXX(c,tmin,simcur))<0 AND SUM(tmax, PXX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, PXX(c,tmax,simcur)) / SUM(tmin, PXX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QAXPP(a,simcur)$((SUM(tmin, QAX(a,tmin,simcur))>0 AND SUM(tmax, QAX(a,tmax,simcur))>0) 
  OR (SUM(tmin, QAX(a,tmin,simcur))<0 AND SUM(tmax, QAX(a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QAX(a,tmax,simcur)) / SUM(tmin, QAX(a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QDXPP(c,simcur)$((SUM(tmin, QDX(c,tmin,simcur))>0 AND SUM(tmax, QDX(c,tmax,simcur))>0) 
  OR (SUM(tmin, QDX(c,tmin,simcur))<0 AND SUM(tmax, QDX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QDX(c,tmax,simcur)) / SUM(tmin, QDX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QEXPP(c,simcur)$((SUM(tmin, QEX(c,tmin,simcur))>0 AND SUM(tmax, QEX(c,tmax,simcur))>0) 
  OR (SUM(tmin, QEX(c,tmin,simcur))<0 AND SUM(tmax, QEX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QEX(c,tmax,simcur)) / SUM(tmin, QEX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QFXPP(ac,acp,simcur)$((SUM(tmin, QFX(ac,acp,tmin,simcur))>0 AND SUM(tmax, QFX(ac,acp,tmax,simcur))>0) 
  OR (SUM(tmin, QFX(ac,acp,tmin,simcur))<0 AND SUM(tmax, QFX(ac,acp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QFX(ac,acp,tmax,simcur)) / SUM(tmin, QFX(ac,acp,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QFHENDXPP(h,f,simcur)$((SUM(tmin, QFHENDX(h,f,tmin,simcur))>0 AND SUM(tmax, QFHENDX(h,f,tmax,simcur))>0) 
  OR (SUM(tmin, QFHENDX(h,f,tmin,simcur))<0 AND SUM(tmax, QFHENDX(h,f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QFHENDX(h,f,tmax,simcur)) / SUM(tmin, QFHENDX(h,f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QFHENDSCALXPP(f,simcur)$((SUM(tmin, QFHENDSCALX(f,tmin,simcur))>0 AND SUM(tmax, QFHENDSCALX(f,tmax,simcur))>0) 
  OR (SUM(tmin, QFHENDSCALX(f,tmin,simcur))<0 AND SUM(tmax, QFHENDSCALX(f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QFHENDSCALX(f,tmax,simcur)) / SUM(tmin, QFHENDSCALX(f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QFINSXPP(ins,ac,simcur)$((SUM(tmin, QFINSX(ins,ac,tmin,simcur))>0 AND SUM(tmax, QFINSX(ins,ac,tmax,simcur))>0) 
  OR (SUM(tmin, QFINSX(ins,ac,tmin,simcur))<0 AND SUM(tmax, QFINSX(ins,ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QFINSX(ins,ac,tmax,simcur)) / SUM(tmin, QFINSX(ins,ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QFINSSCALXPP(f,simcur)$((SUM(tmin, QFINSSCALX(f,tmin,simcur))>0 AND SUM(tmax, QFINSSCALX(f,tmax,simcur))>0) 
  OR (SUM(tmin, QFINSSCALX(f,tmin,simcur))<0 AND SUM(tmax, QFINSSCALX(f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QFINSSCALX(f,tmax,simcur)) / SUM(tmin, QFINSSCALX(f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QFSXPP(f,simcur)$((SUM(tmin, QFSX(f,tmin,simcur))>0 AND SUM(tmax, QFSX(f,tmax,simcur))>0) 
  OR (SUM(tmin, QFSX(f,tmin,simcur))<0 AND SUM(tmax, QFSX(f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QFSX(f,tmax,simcur)) / SUM(tmin, QFSX(f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QGXPP(c,simcur)$((SUM(tmin, QGX(c,tmin,simcur))>0 AND SUM(tmax, QGX(c,tmax,simcur))>0) 
  OR (SUM(tmin, QGX(c,tmin,simcur))<0 AND SUM(tmax, QGX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QGX(c,tmax,simcur)) / SUM(tmin, QGX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QGSCALXPP(simcur)$((SUM(tmin, QGSCALX(tmin,simcur))>0 AND SUM(tmax, QGSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, QGSCALX(tmin,simcur))<0 AND SUM(tmax, QGSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QGSCALX(tmax,simcur)) / SUM(tmin, QGSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

QHXPP(ac,h,simcur)$((SUM(tmin, QHX(ac,h,tmin,simcur))>0 AND SUM(tmax, QHX(ac,h,tmax,simcur))>0) 
  OR (SUM(tmin, QHX(ac,h,tmin,simcur))<0 AND SUM(tmax, QHX(ac,h,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QHX(ac,h,tmax,simcur)) / SUM(tmin, QHX(ac,h,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QINTXPP(c,a,simcur)$((SUM(tmin, QINTX(c,a,tmin,simcur))>0 AND SUM(tmax, QINTX(c,a,tmax,simcur))>0) 
  OR (SUM(tmin, QINTX(c,a,tmin,simcur))<0 AND SUM(tmax, QINTX(c,a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QINTX(c,a,tmax,simcur)) / SUM(tmin, QINTX(c,a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QINVXPP(c,simcur)$((SUM(tmin, QINVX(c,tmin,simcur))>0 AND SUM(tmax, QINVX(c,tmax,simcur))>0) 
  OR (SUM(tmin, QINVX(c,tmin,simcur))<0 AND SUM(tmax, QINVX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QINVX(c,tmax,simcur)) / SUM(tmin, QINVX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QLABSCALXPP(simcur)$((SUM(tmin, QLABSCALX(tmin,simcur))>0 AND SUM(tmax, QLABSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, QLABSCALX(tmin,simcur))<0 AND SUM(tmax, QLABSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QLABSCALX(tmax,simcur)) / SUM(tmin, QLABSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

QMXPP(c,simcur)$((SUM(tmin, QMX(c,tmin,simcur))>0 AND SUM(tmax, QMX(c,tmax,simcur))>0) 
  OR (SUM(tmin, QMX(c,tmin,simcur))<0 AND SUM(tmax, QMX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QMX(c,tmax,simcur)) / SUM(tmin, QMX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QNGOXPP(c,insngo,simcur)$((SUM(tmin, QNGOX(c,insngo,tmin,simcur))>0 AND SUM(tmax, QNGOX(c,insngo,tmax,simcur))>0) 
  OR (SUM(tmin, QNGOX(c,insngo,tmin,simcur))<0 AND SUM(tmax, QNGOX(c,insngo,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QNGOX(c,insngo,tmax,simcur)) / SUM(tmin, QNGOX(c,insngo,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QNGOSCALXPP(insngo,simcur)$((SUM(tmin, QNGOSCALX(insngo,tmin,simcur))>0 AND SUM(tmax, QNGOSCALX(insngo,tmax,simcur))>0) 
  OR (SUM(tmin, QNGOSCALX(insngo,tmin,simcur))<0 AND SUM(tmax, QNGOSCALX(insngo,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QNGOSCALX(insngo,tmax,simcur)) / SUM(tmin, QNGOSCALX(insngo,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QQXPP(c,simcur)$((SUM(tmin, QQX(c,tmin,simcur))>0 AND SUM(tmax, QQX(c,tmax,simcur))>0) 
  OR (SUM(tmin, QQX(c,tmin,simcur))<0 AND SUM(tmax, QQX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QQX(c,tmax,simcur)) / SUM(tmin, QQX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QTXPP(c,simcur)$((SUM(tmin, QTX(c,tmin,simcur))>0 AND SUM(tmax, QTX(c,tmax,simcur))>0) 
  OR (SUM(tmin, QTX(c,tmin,simcur))<0 AND SUM(tmax, QTX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QTX(c,tmax,simcur)) / SUM(tmin, QTX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QTRSTXPP(c,instrst,simcur)$((SUM(tmin, QTRSTX(c,instrst,tmin,simcur))>0 AND SUM(tmax, QTRSTX(c,instrst,tmax,simcur))>0) 
  OR (SUM(tmin, QTRSTX(c,instrst,tmin,simcur))<0 AND SUM(tmax, QTRSTX(c,instrst,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QTRSTX(c,instrst,tmax,simcur)) / SUM(tmin, QTRSTX(c,instrst,tmin,simcur)) ) ** (1/tsolnb) - 1 );

QTRSTSCALXPP(simcur)$((SUM(tmin, QTRSTSCALX(tmin,simcur))>0 AND SUM(tmax, QTRSTSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, QTRSTSCALX(tmin,simcur))<0 AND SUM(tmax, QTRSTSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QTRSTSCALX(tmax,simcur)) / SUM(tmin, QTRSTSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

QXXPP(c,simcur)$((SUM(tmin, QXX(c,tmin,simcur))>0 AND SUM(tmax, QXX(c,tmax,simcur))>0) 
  OR (SUM(tmin, QXX(c,tmin,simcur))<0 AND SUM(tmax, QXX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, QXX(c,tmax,simcur)) / SUM(tmin, QXX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

RBTVATXPP(a,simcur)$((SUM(tmin, RBTVATX(a,tmin,simcur))>0 AND SUM(tmax, RBTVATX(a,tmax,simcur))>0) 
  OR (SUM(tmin, RBTVATX(a,tmin,simcur))<0 AND SUM(tmax, RBTVATX(a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, RBTVATX(a,tmax,simcur)) / SUM(tmin, RBTVATX(a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

REXRXPP(simcur)$((SUM(tmin, REXRX(tmin,simcur))>0 AND SUM(tmax, REXRX(tmax,simcur))>0) 
  OR (SUM(tmin, REXRX(tmin,simcur))<0 AND SUM(tmax, REXRX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, REXRX(tmax,simcur)) / SUM(tmin, REXRX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

RGDPFCXPP(simcur)$((SUM(tmin, RGDPFCX(tmin,simcur))>0 AND SUM(tmax, RGDPFCX(tmax,simcur))>0) 
  OR (SUM(tmin, RGDPFCX(tmin,simcur))<0 AND SUM(tmax, RGDPFCX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, RGDPFCX(tmax,simcur)) / SUM(tmin, RGDPFCX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

RGDPMPXPP(simcur)$((SUM(tmin, RGDPMPX(tmin,simcur))>0 AND SUM(tmax, RGDPMPX(tmax,simcur))>0) 
  OR (SUM(tmin, RGDPMPX(tmin,simcur))<0 AND SUM(tmax, RGDPMPX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, RGDPMPX(tmax,simcur)) / SUM(tmin, RGDPMPX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

RGPRIMDEFXPP(simcur)$((SUM(tmin, RGPRIMDEFX(tmin,simcur))>0 AND SUM(tmax, RGPRIMDEFX(tmax,simcur))>0) 
  OR (SUM(tmin, RGPRIMDEFX(tmin,simcur))<0 AND SUM(tmax, RGPRIMDEFX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, RGPRIMDEFX(tmax,simcur)) / SUM(tmin, RGPRIMDEFX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

RNDFGXPP(simcur)$((SUM(tmin, RNDFGX(tmin,simcur))>0 AND SUM(tmax, RNDFGX(tmax,simcur))>0) 
  OR (SUM(tmin, RNDFGX(tmin,simcur))<0 AND SUM(tmax, RNDFGX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, RNDFGX(tmax,simcur)) / SUM(tmin, RNDFGX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

SAVXPP(ac,simcur)$((SUM(tmin, SAVX(ac,tmin,simcur))>0 AND SUM(tmax, SAVX(ac,tmax,simcur))>0) 
  OR (SUM(tmin, SAVX(ac,tmin,simcur))<0 AND SUM(tmax, SAVX(ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, SAVX(ac,tmax,simcur)) / SUM(tmin, SAVX(ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

SAVFXPP(simcur)$((SUM(tmin, SAVFX(tmin,simcur))>0 AND SUM(tmax, SAVFX(tmax,simcur))>0) 
  OR (SUM(tmin, SAVFX(tmin,simcur))<0 AND SUM(tmax, SAVFX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, SAVFX(tmax,simcur)) / SUM(tmin, SAVFX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

SHIFXPP(ins,f,simcur)$((SUM(tmin, SHIFX(ins,f,tmin,simcur))>0 AND SUM(tmax, SHIFX(ins,f,tmax,simcur))>0) 
  OR (SUM(tmin, SHIFX(ins,f,tmin,simcur))<0 AND SUM(tmax, SHIFX(ins,f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, SHIFX(ins,f,tmax,simcur)) / SUM(tmin, SHIFX(ins,f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

SUBCXPP(c,ac,simcur)$((SUM(tmin, SUBCX(c,ac,tmin,simcur))>0 AND SUM(tmax, SUBCX(c,ac,tmax,simcur))>0) 
  OR (SUM(tmin, SUBCX(c,ac,tmin,simcur))<0 AND SUM(tmax, SUBCX(c,ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, SUBCX(c,ac,tmax,simcur)) / SUM(tmin, SUBCX(c,ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

SUBCSCALXPP(simcur)$((SUM(tmin, SUBCSCALX(tmin,simcur))>0 AND SUM(tmax, SUBCSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, SUBCSCALX(tmin,simcur))<0 AND SUM(tmax, SUBCSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, SUBCSCALX(tmax,simcur)) / SUM(tmin, SUBCSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

SUBCTXPP(simcur)$((SUM(tmin, SUBCTX(tmin,simcur))>0 AND SUM(tmax, SUBCTX(tmax,simcur))>0) 
  OR (SUM(tmin, SUBCTX(tmin,simcur))<0 AND SUM(tmax, SUBCTX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, SUBCTX(tmax,simcur)) / SUM(tmin, SUBCTX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TAXPP(a,simcur)$((SUM(tmin, TAX(a,tmin,simcur))>0 AND SUM(tmax, TAX(a,tmax,simcur))>0) 
  OR (SUM(tmin, TAX(a,tmin,simcur))<0 AND SUM(tmax, TAX(a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TAX(a,tmax,simcur)) / SUM(tmin, TAX(a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TASCALXPP(simcur)$((SUM(tmin, TASCALX(tmin,simcur))>0 AND SUM(tmax, TASCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TASCALX(tmin,simcur))<0 AND SUM(tmax, TASCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TASCALX(tmax,simcur)) / SUM(tmin, TASCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TEXPP(c,simcur)$((SUM(tmin, TEX(c,tmin,simcur))>0 AND SUM(tmax, TEX(c,tmax,simcur))>0) 
  OR (SUM(tmin, TEX(c,tmin,simcur))<0 AND SUM(tmax, TEX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TEX(c,tmax,simcur)) / SUM(tmin, TEX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TESCALXPP(simcur)$((SUM(tmin, TESCALX(tmin,simcur))>0 AND SUM(tmax, TESCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TESCALX(tmin,simcur))<0 AND SUM(tmax, TESCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TESCALX(tmax,simcur)) / SUM(tmin, TESCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TFXPP(f,simcur)$((SUM(tmin, TFX(f,tmin,simcur))>0 AND SUM(tmax, TFX(f,tmax,simcur))>0) 
  OR (SUM(tmin, TFX(f,tmin,simcur))<0 AND SUM(tmax, TFX(f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TFX(f,tmax,simcur)) / SUM(tmin, TFX(f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TFAXPP(f,a,simcur)$((SUM(tmin, TFAX(f,a,tmin,simcur))>0 AND SUM(tmax, TFAX(f,a,tmax,simcur))>0) 
  OR (SUM(tmin, TFAX(f,a,tmin,simcur))<0 AND SUM(tmax, TFAX(f,a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TFAX(f,a,tmax,simcur)) / SUM(tmin, TFAX(f,a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TFASCALXPP(simcur)$((SUM(tmin, TFASCALX(tmin,simcur))>0 AND SUM(tmax, TFASCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TFASCALX(tmin,simcur))<0 AND SUM(tmax, TFASCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TFASCALX(tmax,simcur)) / SUM(tmin, TFASCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TFPXPP(a,simcur)$((SUM(tmin, TFPX(a,tmin,simcur))>0 AND SUM(tmax, TFPX(a,tmax,simcur))>0) 
  OR (SUM(tmin, TFPX(a,tmin,simcur))<0 AND SUM(tmax, TFPX(a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TFPX(a,tmax,simcur)) / SUM(tmin, TFPX(a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TFPSCALXPP(simcur)$((SUM(tmin, TFPSCALX(tmin,simcur))>0 AND SUM(tmax, TFPSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TFPSCALX(tmin,simcur))<0 AND SUM(tmax, TFPSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TFPSCALX(tmax,simcur)) / SUM(tmin, TFPSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TFSCALXPP(simcur)$((SUM(tmin, TFSCALX(tmin,simcur))>0 AND SUM(tmax, TFSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TFSCALX(tmin,simcur))<0 AND SUM(tmax, TFSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TFSCALX(tmax,simcur)) / SUM(tmin, TFSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TMXPP(c,simcur)$((SUM(tmin, TMX(c,tmin,simcur))>0 AND SUM(tmax, TMX(c,tmax,simcur))>0) 
  OR (SUM(tmin, TMX(c,tmin,simcur))<0 AND SUM(tmax, TMX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TMX(c,tmax,simcur)) / SUM(tmin, TMX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TMSCALXPP(simcur)$((SUM(tmin, TMSCALX(tmin,simcur))>0 AND SUM(tmax, TMSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TMSCALX(tmin,simcur))<0 AND SUM(tmax, TMSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TMSCALX(tmax,simcur)) / SUM(tmin, TMSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TQXPP(c,simcur)$((SUM(tmin, TQX(c,tmin,simcur))>0 AND SUM(tmax, TQX(c,tmax,simcur))>0) 
  OR (SUM(tmin, TQX(c,tmin,simcur))<0 AND SUM(tmax, TQX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TQX(c,tmax,simcur)) / SUM(tmin, TQX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TQSCALXPP(simcur)$((SUM(tmin, TQSCALX(tmin,simcur))>0 AND SUM(tmax, TQSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TQSCALX(tmin,simcur))<0 AND SUM(tmax, TQSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TQSCALX(tmax,simcur)) / SUM(tmin, TQSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TRDGDPXPP(simcur)$((SUM(tmin, TRDGDPX(tmin,simcur))>0 AND SUM(tmax, TRDGDPX(tmax,simcur))>0) 
  OR (SUM(tmin, TRDGDPX(tmin,simcur))<0 AND SUM(tmax, TRDGDPX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TRDGDPX(tmax,simcur)) / SUM(tmin, TRDGDPX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TRIIXPP(ins,insp,simcur)$((SUM(tmin, TRIIX(ins,insp,tmin,simcur))>0 AND SUM(tmax, TRIIX(ins,insp,tmax,simcur))>0) 
  OR (SUM(tmin, TRIIX(ins,insp,tmin,simcur))<0 AND SUM(tmax, TRIIX(ins,insp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TRIIX(ins,insp,tmax,simcur)) / SUM(tmin, TRIIX(ins,insp,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TRNSFRXPP(ac,acp,simcur)$((SUM(tmin, TRNSFRX(ac,acp,tmin,simcur))>0 AND SUM(tmax, TRNSFRX(ac,acp,tmax,simcur))>0) 
  OR (SUM(tmin, TRNSFRX(ac,acp,tmin,simcur))<0 AND SUM(tmax, TRNSFRX(ac,acp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TRNSFRX(ac,acp,tmax,simcur)) / SUM(tmin, TRNSFRX(ac,acp,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TRNSFRSCALXPP(ac,simcur)$((SUM(tmin, TRNSFRSCALX(ac,tmin,simcur))>0 AND SUM(tmax, TRNSFRSCALX(ac,tmax,simcur))>0) 
  OR (SUM(tmin, TRNSFRSCALX(ac,tmin,simcur))<0 AND SUM(tmax, TRNSFRSCALX(ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TRNSFRSCALX(ac,tmax,simcur)) / SUM(tmin, TRNSFRSCALX(ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TRSMRECXPP(simcur)$((SUM(tmin, TRSMRECX(tmin,simcur))>0 AND SUM(tmax, TRSMRECX(tmax,simcur))>0) 
  OR (SUM(tmin, TRSMRECX(tmin,simcur))<0 AND SUM(tmax, TRSMRECX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TRSMRECX(tmax,simcur)) / SUM(tmin, TRSMRECX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TVACXPP(c,ac,simcur)$((SUM(tmin, TVACX(c,ac,tmin,simcur))>0 AND SUM(tmax, TVACX(c,ac,tmax,simcur))>0) 
  OR (SUM(tmin, TVACX(c,ac,tmin,simcur))<0 AND SUM(tmax, TVACX(c,ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TVACX(c,ac,tmax,simcur)) / SUM(tmin, TVACX(c,ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TVACSCALXPP(simcur)$((SUM(tmin, TVACSCALX(tmin,simcur))>0 AND SUM(tmax, TVACSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TVACSCALX(tmin,simcur))<0 AND SUM(tmax, TVACSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TVACSCALX(tmax,simcur)) / SUM(tmin, TVACSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

TYXPP(ins,simcur)$((SUM(tmin, TYX(ins,tmin,simcur))>0 AND SUM(tmax, TYX(ins,tmax,simcur))>0) 
  OR (SUM(tmin, TYX(ins,tmin,simcur))<0 AND SUM(tmax, TYX(ins,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TYX(ins,tmax,simcur)) / SUM(tmin, TYX(ins,tmin,simcur)) ) ** (1/tsolnb) - 1 );

TYSCALXPP(simcur)$((SUM(tmin, TYSCALX(tmin,simcur))>0 AND SUM(tmax, TYSCALX(tmax,simcur))>0) 
  OR (SUM(tmin, TYSCALX(tmin,simcur))<0 AND SUM(tmax, TYSCALX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, TYSCALX(tmax,simcur)) / SUM(tmin, TYSCALX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

UERATXPP(ac,simcur)$((SUM(tmin, UERATX(ac,tmin,simcur))>0 AND SUM(tmax, UERATX(ac,tmax,simcur))>0) 
  OR (SUM(tmin, UERATX(ac,tmin,simcur))<0 AND SUM(tmax, UERATX(ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, UERATX(ac,tmax,simcur)) / SUM(tmin, UERATX(ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

WALRASXPP(simcur)$((SUM(tmin, WALRASX(tmin,simcur))>0 AND SUM(tmax, WALRASX(tmax,simcur))>0) 
  OR (SUM(tmin, WALRASX(tmin,simcur))<0 AND SUM(tmax, WALRASX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, WALRASX(tmax,simcur)) / SUM(tmin, WALRASX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

WFXPP(f,simcur)$((SUM(tmin, WFX(f,tmin,simcur))>0 AND SUM(tmax, WFX(f,tmax,simcur))>0) 
  OR (SUM(tmin, WFX(f,tmin,simcur))<0 AND SUM(tmax, WFX(f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, WFX(f,tmax,simcur)) / SUM(tmin, WFX(f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

WFAXPP(f,a,simcur)$((SUM(tmin, WFAX(f,a,tmin,simcur))>0 AND SUM(tmax, WFAX(f,a,tmax,simcur))>0) 
  OR (SUM(tmin, WFAX(f,a,tmin,simcur))<0 AND SUM(tmax, WFAX(f,a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, WFAX(f,a,tmax,simcur)) / SUM(tmin, WFAX(f,a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

WFAVGXPP(ac,simcur)$((SUM(tmin, WFAVGX(ac,tmin,simcur))>0 AND SUM(tmax, WFAVGX(ac,tmax,simcur))>0) 
  OR (SUM(tmin, WFAVGX(ac,tmin,simcur))<0 AND SUM(tmax, WFAVGX(ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, WFAVGX(ac,tmax,simcur)) / SUM(tmin, WFAVGX(ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

WFDISTXPP(f,a,simcur)$((SUM(tmin, WFDISTX(f,a,tmin,simcur))>0 AND SUM(tmax, WFDISTX(f,a,tmax,simcur))>0) 
  OR (SUM(tmin, WFDISTX(f,a,tmin,simcur))<0 AND SUM(tmax, WFDISTX(f,a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, WFDISTX(f,a,tmax,simcur)) / SUM(tmin, WFDISTX(f,a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

YFXPP(f,simcur)$((SUM(tmin, YFX(f,tmin,simcur))>0 AND SUM(tmax, YFX(f,tmax,simcur))>0) 
  OR (SUM(tmin, YFX(f,tmin,simcur))<0 AND SUM(tmax, YFX(f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YFX(f,tmax,simcur)) / SUM(tmin, YFX(f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

YGXPP(simcur)$((SUM(tmin, YGX(tmin,simcur))>0 AND SUM(tmax, YGX(tmax,simcur))>0) 
  OR (SUM(tmin, YGX(tmin,simcur))<0 AND SUM(tmax, YGX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YGX(tmax,simcur)) / SUM(tmin, YGX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

YIXPP(ins,simcur)$((SUM(tmin, YIX(ins,tmin,simcur))>0 AND SUM(tmax, YIX(ins,tmax,simcur))>0) 
  OR (SUM(tmin, YIX(ins,tmin,simcur))<0 AND SUM(tmax, YIX(ins,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YIX(ins,tmax,simcur)) / SUM(tmin, YIX(ins,tmin,simcur)) ) ** (1/tsolnb) - 1 );

YIFXPP(ins,f,simcur)$((SUM(tmin, YIFX(ins,f,tmin,simcur))>0 AND SUM(tmax, YIFX(ins,f,tmax,simcur))>0) 
  OR (SUM(tmin, YIFX(ins,f,tmin,simcur))<0 AND SUM(tmax, YIFX(ins,f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YIFX(ins,f,tmax,simcur)) / SUM(tmin, YIFX(ins,f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

YTAXIMPXPP(simcur)$((SUM(tmin, YTAXIMPX(tmin,simcur))>0 AND SUM(tmax, YTAXIMPX(tmax,simcur))>0) 
  OR (SUM(tmin, YTAXIMPX(tmin,simcur))<0 AND SUM(tmax, YTAXIMPX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YTAXIMPX(tmax,simcur)) / SUM(tmin, YTAXIMPX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

YTAXEXPXPP(simcur)$((SUM(tmin, YTAXEXPX(tmin,simcur))>0 AND SUM(tmax, YTAXEXPX(tmax,simcur))>0) 
  OR (SUM(tmin, YTAXEXPX(tmin,simcur))<0 AND SUM(tmax, YTAXEXPX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YTAXEXPX(tmax,simcur)) / SUM(tmin, YTAXEXPX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

YTAXVATXPP(simcur)$((SUM(tmin, YTAXVATX(tmin,simcur))>0 AND SUM(tmax, YTAXVATX(tmax,simcur))>0) 
  OR (SUM(tmin, YTAXVATX(tmin,simcur))<0 AND SUM(tmax, YTAXVATX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, YTAXVATX(tmax,simcur)) / SUM(tmin, YTAXVATX(tmin,simcur)) ) ** (1/tsolnb) - 1 );



* other reports

popXPP(ac,simcur)$((SUM(tmin, popX(ac,tmin,simcur))>0 AND SUM(tmax, popX(ac,tmax,simcur))>0) 
  OR (SUM(tmin, popX(ac,tmin,simcur))<0 AND SUM(tmax, popX(ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, popX(ac,tmax,simcur)) / SUM(tmin, popX(ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

pweXPP(c,simcur)$((SUM(tmin, pweX(c,tmin,simcur))>0 AND SUM(tmax, pweX(c,tmax,simcur))>0) 
  OR (SUM(tmin, pweX(c,tmin,simcur))<0 AND SUM(tmax, pweX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, pweX(c,tmax,simcur)) / SUM(tmin, pweX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

pwmXPP(c,simcur)$((SUM(tmin, pwmX(c,tmin,simcur))>0 AND SUM(tmax, pwmX(c,tmax,simcur))>0) 
  OR (SUM(tmin, pwmX(c,tmin,simcur))<0 AND SUM(tmax, pwmX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, pwmX(c,tmax,simcur)) / SUM(tmin, pwmX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

*qa2XPP(a,simcur)$((SUM(tmin, qa2X(a,tmin,simcur))>0 AND SUM(tmax, qa2X(a,tmax,simcur))>0) 
*  OR (SUM(tmin, qa2X(a,tmin,simcur))<0 AND SUM(tmax, qa2X(a,tmax,simcur))<0)) = 
*  100 * ( ( SUM(tmax, qa2X(a,tmax,simcur)) / SUM(tmin, qa2X(a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

qgbXPP(c,simcur)$((SUM(tmin, qgbX(c,tmin,simcur))>0 AND SUM(tmax, qgbX(c,tmax,simcur))>0) 
  OR (SUM(tmin, qgbX(c,tmin,simcur))<0 AND SUM(tmax, qgbX(c,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, qgbX(c,tmax,simcur)) / SUM(tmin, qgbX(c,tmin,simcur)) ) ** (1/tsolnb) - 1 );

qhpcXPP(ac,simcur)$((SUM(tmin, qhpcX(ac,tmin,simcur))>0 AND SUM(tmax, qhpcX(ac,tmax,simcur))>0) 
  OR (SUM(tmin, qhpcX(ac,tmin,simcur))<0 AND SUM(tmax, qhpcX(ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, qhpcX(ac,tmax,simcur)) / SUM(tmin, qhpcX(ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

yhpcXPP(ac,simcur)$((SUM(tmin, yhpcX(ac,tmin,simcur))>0 AND SUM(tmax, yhpcX(ac,tmax,simcur))>0) 
  OR (SUM(tmin, yhpcX(ac,tmin,simcur))<0 AND SUM(tmax, yhpcX(ac,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, yhpcX(ac,tmax,simcur)) / SUM(tmin, yhpcX(ac,tmin,simcur)) ) ** (1/tsolnb) - 1 );

rsavgXPP(simcur)$((SUM(tmin, rsavgX(tmin,simcur))>0 AND SUM(tmax, rsavgX(tmax,simcur))>0) 
  OR (SUM(tmin, rsavgX(tmin,simcur))<0 AND SUM(tmax, rsavgX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, rsavgX(tmax,simcur)) / SUM(tmin, rsavgX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

savgXPP(simcur)$((SUM(tmin, savgX(tmin,simcur))>0 AND SUM(tmax, savgX(tmax,simcur))>0) 
  OR (SUM(tmin, savgX(tmin,simcur))<0 AND SUM(tmax, savgX(tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, savgX(tmax,simcur)) / SUM(tmin, savgX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

*tfpaXPP(a,simcur)$((SUM(tmin, tfpaX(a,tmin,simcur))>0 AND SUM(tmax, tfpaX(a,tmax,simcur))>0) 
*  OR (SUM(tmin, tfpaX(a,tmin,simcur))<0 AND SUM(tmax, tfpaX(a,tmax,simcur))<0)) = 
*  100 * ( ( SUM(tmax, tfpaX(a,tmax,simcur)) / SUM(tmin, tfpaX(a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

*tfpatotXPP(simcur)$((SUM(tmin, tfpatotX(tmin,simcur))>0 AND SUM(tmax, tfpatotX(tmax,simcur))>0) 
*  OR (SUM(tmin, tfpatotX(tmin,simcur))<0 AND SUM(tmax, tfpatotX(tmax,simcur))<0)) = 
*  100 * ( ( SUM(tmax, tfpatotX(tmax,simcur)) / SUM(tmin, tfpatotX(tmin,simcur)) ) ** (1/tsolnb) - 1 );

wfarealXPP(f,a,simcur)$((SUM(tmin, wfarealX(f,a,tmin,simcur))>0 AND SUM(tmax, wfarealX(f,a,tmax,simcur))>0) 
  OR (SUM(tmin, wfarealX(f,a,tmin,simcur))<0 AND SUM(tmax, wfarealX(f,a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, wfarealX(f,a,tmax,simcur)) / SUM(tmin, wfarealX(f,a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

wfrealXPP(f,simcur)$((SUM(tmin, wfrealX(f,tmin,simcur))>0 AND SUM(tmax, wfrealX(f,tmax,simcur))>0) 
  OR (SUM(tmin, wfrealX(f,tmin,simcur))<0 AND SUM(tmax, wfrealX(f,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, wfrealX(f,tmax,simcur)) / SUM(tmin, wfrealX(f,tmin,simcur)) ) ** (1/tsolnb) - 1 );

shvatrbtXPP(c,a,simcur)$((SUM(tmin, shvatrbtX(c,a,tmin,simcur))>0 AND SUM(tmax, shvatrbtX(c,a,tmax,simcur))>0) 
  OR (SUM(tmin, shvatrbtX(c,a,tmin,simcur))<0 AND SUM(tmax, shvatrbtX(c,a,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, shvatrbtX(c,a,tmax,simcur)) / SUM(tmin, shvatrbtX(c,a,tmin,simcur)) ) ** (1/tsolnb) - 1 );

gdpindicXPP(igdp,kgdp,simcur)$((SUM(tmin, gdpindic(igdp,kgdp,tmin,simcur))>0 AND SUM(tmax, gdpindic(igdp,kgdp,tmax,simcur))>0) 
  OR (SUM(tmin, gdpindic(igdp,kgdp,tmin,simcur))<0 AND SUM(tmax, gdpindic(igdp,kgdp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, gdpindic(igdp,kgdp,tmax,simcur)) / SUM(tmin, gdpindic(igdp,kgdp,tmin,simcur)) ) ** (1/tsolnb) - 1 );

gdpexpindicXPP(maccol,kgdp,simcur)$((SUM(tmin, gdpexpindic(maccol,kgdp,tmin,simcur))>0 AND SUM(tmax, gdpexpindic(maccol,kgdp,tmax,simcur))>0) 
  OR (SUM(tmin, gdpexpindic(maccol,kgdp,tmin,simcur))<0 AND SUM(tmax, gdpexpindic(maccol,kgdp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, gdpexpindic(maccol,kgdp,tmax,simcur)) / SUM(tmin, gdpexpindic(maccol,kgdp,tmin,simcur)) ) ** (1/tsolnb) - 1 );

sectorindicXPP(ac,sectorcol3,kgdp,simcur)$((SUM(tmin, sectorindic(ac,sectorcol3,kgdp,tmin,simcur))>0 AND SUM(tmax, sectorindic(ac,sectorcol3,kgdp,tmax,simcur))>0) 
  OR (SUM(tmin, sectorindic(ac,sectorcol3,kgdp,tmin,simcur))<0 AND SUM(tmax, sectorindic(ac,sectorcol3,kgdp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, sectorindic(ac,sectorcol3,kgdp,tmax,simcur)) / SUM(tmin, sectorindic(ac,sectorcol3,kgdp,tmin,simcur)) ) ** (1/tsolnb) - 1 );

sectorindic2XPP(acrep,sectorcol3,kgdp,simcur)$((SUM(tmin, sectorindic2(acrep,sectorcol3,kgdp,tmin,simcur))>0 AND SUM(tmax, sectorindic2(acrep,sectorcol3,kgdp,tmax,simcur))>0) 
  OR (SUM(tmin, sectorindic2(acrep,sectorcol3,kgdp,tmin,simcur))<0 AND SUM(tmax, sectorindic2(acrep,sectorcol3,kgdp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, sectorindic2(acrep,sectorcol3,kgdp,tmax,simcur)) / SUM(tmin, sectorindic2(acrep,sectorcol3,kgdp,tmin,simcur)) ) ** (1/tsolnb) - 1 );

emiindicXPP(emicol,ac,acp,simcur)$((SUM(tmin, emiindic(emicol,ac,acp,tmin,simcur))>0 AND SUM(tmax, emiindic(emicol,ac,acp,tmax,simcur))>0) 
  OR (SUM(tmin, emiindic(emicol,ac,acp,tmin,simcur))<0 AND SUM(tmax, emiindic(emicol,ac,acp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, emiindic(emicol,ac,acp,tmax,simcur)) / SUM(tmin, emiindic(emicol,ac,acp,tmin,simcur)) ) ** (1/tsolnb) - 1 );

emiindic2XPP(emicol,acrep,acrepp,simcur)$((SUM(tmin, emiindic2(emicol,acrep,acrepp,tmin,simcur))>0 AND SUM(tmax, emiindic2(emicol,acrep,acrepp,tmax,simcur))>0) 
  OR (SUM(tmin, emiindic2(emicol,acrep,acrepp,tmin,simcur))<0 AND SUM(tmax, emiindic2(emicol,acrep,acrepp,tmax,simcur))<0)) = 
  100 * ( ( SUM(tmax, emiindic2(emicol,acrep,acrepp,tmax,simcur)) / SUM(tmin, emiindic2(emicol,acrep,acrepp,tmin,simcur)) ) ** (1/tsolnb) - 1 );





DISPLAY "#### END: repperc-xpp.inc";