* repperc-pprep.inc

DISPLAY "#### START: repperc-xpprep.inc";

PREXRXPPREP(simcur)$((SUM(tminrep, PREXRX(tminrep,simcur))>0 AND SUM(tmaxrep, PREXRX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PREXRX(tminrep,simcur))<0 AND SUM(tmaxrep, PREXRX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PREXRX(tmaxrep,simcur)) / SUM(tminrep, PREXRX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YPREXRTXPPREP(c,simcur)$((SUM(tminrep, YPREXRTX(c,tminrep,simcur))>0 AND SUM(tmaxrep, YPREXRTX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YPREXRTX(c,tminrep,simcur))<0 AND SUM(tmaxrep, YPREXRTX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YPREXRTX(c,tmaxrep,simcur)) / SUM(tminrep, YPREXRTX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YPREXRXPPREP(c,ac,simcur)$((SUM(tminrep, YPREXRX(c,ac,tminrep,simcur))>0 AND SUM(tmaxrep, YPREXRX(c,ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YPREXRX(c,ac,tminrep,simcur))<0 AND SUM(tmaxrep, YPREXRX(c,ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YPREXRX(c,ac,tmaxrep,simcur)) / SUM(tminrep, YPREXRX(c,ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PRQMBARXPPREP(c,simcur)$((SUM(tminrep, PRQMBARX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PRQMBARX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PRQMBARX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PRQMBARX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PRQMBARX(c,tmaxrep,simcur)) / SUM(tminrep, PRQMBARX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YPRQMBARTXPPREP(c,simcur)$((SUM(tminrep, YPRQMBARTX(c,tminrep,simcur))>0 AND SUM(tmaxrep, YPRQMBARTX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YPRQMBARTX(c,tminrep,simcur))<0 AND SUM(tmaxrep, YPRQMBARTX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YPRQMBARTX(c,tmaxrep,simcur)) / SUM(tminrep, YPRQMBARTX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YPRQMBARXPPREP(c,ac,simcur)$((SUM(tminrep, YPRQMBARX(c,ac,tminrep,simcur))>0 AND SUM(tmaxrep, YPRQMBARX(c,ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YPRQMBARX(c,ac,tminrep,simcur))<0 AND SUM(tmaxrep, YPRQMBARX(c,ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YPRQMBARX(c,ac,tmaxrep,simcur)) / SUM(tminrep, YPRQMBARX(c,ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );



PWEXPPREP(c,simcur)$((SUM(tminrep, PWEX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PWEX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PWEX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PWEX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PWEX(c,tmaxrep,simcur)) / SUM(tminrep, PWEX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PWMXPPREP(c,simcur)$((SUM(tminrep, PWMX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PWMX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PWMX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PWMX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PWMX(c,tmaxrep,simcur)) / SUM(tminrep, PWMX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );



QXACXPPREP(a,c,simcur)$((SUM(tminrep, QXACX(a,c,tminrep,simcur))>0 AND SUM(tmaxrep, QXACX(a,c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QXACX(a,c,tminrep,simcur))<0 AND SUM(tmaxrep, QXACX(a,c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QXACX(a,c,tmaxrep,simcur)) / SUM(tminrep, QXACX(a,c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PXACXPPREP(a,c,simcur)$((SUM(tminrep, PXACX(a,c,tminrep,simcur))>0 AND SUM(tmaxrep, PXACX(a,c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PXACX(a,c,tminrep,simcur))<0 AND SUM(tmaxrep, PXACX(a,c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PXACX(a,c,tmaxrep,simcur)) / SUM(tminrep, PXACX(a,c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QEMIXPPREP(acpp,ac,acp,simcur)$((SUM(tminrep, QEMIX(acpp,ac,acp,tminrep,simcur))>0 AND SUM(tmaxrep, QEMIX(acpp,ac,acp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QEMIX(acpp,ac,acp,tminrep,simcur))<0 AND SUM(tmaxrep, QEMIX(acpp,ac,acp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QEMIX(acpp,ac,acp,tmaxrep,simcur)) / SUM(tminrep, QEMIX(acpp,ac,acp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );



ABSNOMXPPREP(simcur)$((SUM(tminrep, ABSNOMX(tminrep,simcur))>0 AND SUM(tmaxrep, ABSNOMX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, ABSNOMX(tminrep,simcur))<0 AND SUM(tmaxrep, ABSNOMX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, ABSNOMX(tmaxrep,simcur)) / SUM(tminrep, ABSNOMX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

CPIXPPREP(simcur)$((SUM(tminrep, CPIX(tminrep,simcur))>0 AND SUM(tmaxrep, CPIX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, CPIX(tminrep,simcur))<0 AND SUM(tmaxrep, CPIX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, CPIX(tmaxrep,simcur)) / SUM(tminrep, CPIX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

DKAXPPREP(f,a,simcur)$((SUM(tminrep, DKAX(f,a,tminrep,simcur))>0 AND SUM(tmaxrep, DKAX(f,a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, DKAX(f,a,tminrep,simcur))<0 AND SUM(tmaxrep, DKAX(f,a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, DKAX(f,a,tmaxrep,simcur)) / SUM(tminrep, DKAX(f,a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

DKINSXPPREP(ins2,f,simcur)$((SUM(tminrep, DKINSX(ins2,f,tminrep,simcur))>0 AND SUM(tmaxrep, DKINSX(ins2,f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, DKINSX(ins2,f,tminrep,simcur))<0 AND SUM(tmaxrep, DKINSX(ins2,f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, DKINSX(ins2,f,tmaxrep,simcur)) / SUM(tminrep, DKINSX(ins2,f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

DPIXPPREP(simcur)$((SUM(tminrep, DPIX(tminrep,simcur))>0 AND SUM(tmaxrep, DPIX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, DPIX(tminrep,simcur))<0 AND SUM(tmaxrep, DPIX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, DPIX(tmaxrep,simcur)) / SUM(tminrep, DPIX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

EGXPPREP(simcur)$((SUM(tminrep, EGX(tminrep,simcur))>0 AND SUM(tmaxrep, EGX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, EGX(tminrep,simcur))<0 AND SUM(tmaxrep, EGX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, EGX(tmaxrep,simcur)) / SUM(tminrep, EGX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

EHXPPREP(h,simcur)$((SUM(tminrep, EHX(h,tminrep,simcur))>0 AND SUM(tmaxrep, EHX(h,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, EHX(h,tminrep,simcur))<0 AND SUM(tmaxrep, EHX(h,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, EHX(h,tmaxrep,simcur)) / SUM(tminrep, EHX(h,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

EXRXPPREP(simcur)$((SUM(tminrep, EXRX(tminrep,simcur))>0 AND SUM(tmaxrep, EXRX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, EXRX(tminrep,simcur))<0 AND SUM(tmaxrep, EXRX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, EXRX(tmaxrep,simcur)) / SUM(tminrep, EXRX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

FBORXPPREP(ins2,simcur)$((SUM(tminrep, FBORX(ins2,tminrep,simcur))>0 AND SUM(tmaxrep, FBORX(ins2,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, FBORX(ins2,tminrep,simcur))<0 AND SUM(tmaxrep, FBORX(ins2,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, FBORX(ins2,tmaxrep,simcur)) / SUM(tminrep, FBORX(ins2,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

FDEBTXPPREP(ins2,simcur)$((SUM(tminrep, FDEBTX(ins2,tminrep,simcur))>0 AND SUM(tmaxrep, FDEBTX(ins2,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, FDEBTX(ins2,tminrep,simcur))<0 AND SUM(tmaxrep, FDEBTX(ins2,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, FDEBTX(ins2,tmaxrep,simcur)) / SUM(tminrep, FDEBTX(ins2,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

FDISCALXPPREP(simcur)$((SUM(tminrep, FDISCALX(tminrep,simcur))>0 AND SUM(tmaxrep, FDISCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, FDISCALX(tminrep,simcur))<0 AND SUM(tmaxrep, FDISCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, FDISCALX(tmaxrep,simcur)) / SUM(tminrep, FDISCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

FPRDAXPPREP(f,a,simcur)$((SUM(tminrep, FPRDAX(f,a,tminrep,simcur))>0 AND SUM(tmaxrep, FPRDAX(f,a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, FPRDAX(f,a,tminrep,simcur))<0 AND SUM(tmaxrep, FPRDAX(f,a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, FPRDAX(f,a,tmaxrep,simcur)) / SUM(tminrep, FPRDAX(f,a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

GBORXPPREP(simcur)$((SUM(tminrep, GBORX(tminrep,simcur))>0 AND SUM(tmaxrep, GBORX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, GBORX(tminrep,simcur))<0 AND SUM(tmaxrep, GBORX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, GBORX(tmaxrep,simcur)) / SUM(tminrep, GBORX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

GDEBTXPPREP(simcur)$((SUM(tminrep, GDEBTX(tminrep,simcur))>0 AND SUM(tmaxrep, GDEBTX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, GDEBTX(tminrep,simcur))<0 AND SUM(tmaxrep, GDEBTX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, GDEBTX(tmaxrep,simcur)) / SUM(tminrep, GDEBTX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

GDPMPXPPREP(simcur)$((SUM(tminrep, GDPMPX(tminrep,simcur))>0 AND SUM(tmaxrep, GDPMPX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, GDPMPX(tminrep,simcur))<0 AND SUM(tmaxrep, GDPMPX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, GDPMPX(tmaxrep,simcur)) / SUM(tminrep, GDPMPX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

GOVRECABSXPPREP(acgovrec,simcur)$((SUM(tminrep, GOVRECABSX(acgovrec,tminrep,simcur))>0 AND SUM(tmaxrep, GOVRECABSX(acgovrec,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, GOVRECABSX(acgovrec,tminrep,simcur))<0 AND SUM(tmaxrep, GOVRECABSX(acgovrec,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, GOVRECABSX(acgovrec,tmaxrep,simcur)) / SUM(tminrep, GOVRECABSX(acgovrec,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

GOVRECGDPXPPREP(acgovrec,simcur)$((SUM(tminrep, GOVRECGDPX(acgovrec,tminrep,simcur))>0 AND SUM(tmaxrep, GOVRECGDPX(acgovrec,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, GOVRECGDPX(acgovrec,tminrep,simcur))<0 AND SUM(tmaxrep, GOVRECGDPX(acgovrec,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, GOVRECGDPX(acgovrec,tmaxrep,simcur)) / SUM(tminrep, GOVRECGDPX(acgovrec,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

GOVSPNDABSXPPREP(acgovspnd,simcur)$((SUM(tminrep, GOVSPNDABSX(acgovspnd,tminrep,simcur))>0 AND SUM(tmaxrep, GOVSPNDABSX(acgovspnd,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, GOVSPNDABSX(acgovspnd,tminrep,simcur))<0 AND SUM(tmaxrep, GOVSPNDABSX(acgovspnd,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, GOVSPNDABSX(acgovspnd,tmaxrep,simcur)) / SUM(tminrep, GOVSPNDABSX(acgovspnd,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

GOVSPNDGDPXPPREP(acgovspnd,simcur)$((SUM(tminrep, GOVSPNDGDPX(acgovspnd,tminrep,simcur))>0 AND SUM(tmaxrep, GOVSPNDGDPX(acgovspnd,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, GOVSPNDGDPX(acgovspnd,tminrep,simcur))<0 AND SUM(tmaxrep, GOVSPNDGDPX(acgovspnd,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, GOVSPNDGDPX(acgovspnd,tmaxrep,simcur)) / SUM(tminrep, GOVSPNDGDPX(acgovspnd,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

GPRIMDEFXPPREP(simcur)$((SUM(tminrep, GPRIMDEFX(tminrep,simcur))>0 AND SUM(tmaxrep, GPRIMDEFX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, GPRIMDEFX(tminrep,simcur))<0 AND SUM(tmaxrep, GPRIMDEFX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, GPRIMDEFX(tmaxrep,simcur)) / SUM(tminrep, GPRIMDEFX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

IADJXPPREP(ins2,simcur)$((SUM(tminrep, IADJX(ins2,tminrep,simcur))>0 AND SUM(tmaxrep, IADJX(ins2,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, IADJX(ins2,tminrep,simcur))<0 AND SUM(tmaxrep, IADJX(ins2,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, IADJX(ins2,tmaxrep,simcur)) / SUM(tminrep, IADJX(ins2,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

INVVALXPPREP(simcur)$((SUM(tminrep, INVVALX(tminrep,simcur))>0 AND SUM(tmaxrep, INVVALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, INVVALX(tminrep,simcur))<0 AND SUM(tmaxrep, INVVALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, INVVALX(tmaxrep,simcur)) / SUM(tminrep, INVVALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

INVVALFXPPREP(simcur)$((SUM(tminrep, INVVALFX(tminrep,simcur))>0 AND SUM(tmaxrep, INVVALFX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, INVVALFX(tminrep,simcur))<0 AND SUM(tmaxrep, INVVALFX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, INVVALFX(tmaxrep,simcur)) / SUM(tminrep, INVVALFX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

INVVALGXPPREP(simcur)$((SUM(tminrep, INVVALGX(tminrep,simcur))>0 AND SUM(tmaxrep, INVVALGX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, INVVALGX(tminrep,simcur))<0 AND SUM(tmaxrep, INVVALGX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, INVVALGX(tmaxrep,simcur)) / SUM(tminrep, INVVALGX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

ISCALXPPREP(ac,simcur)$((SUM(tminrep, ISCALX(ac,tminrep,simcur))>0 AND SUM(tmaxrep, ISCALX(ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, ISCALX(ac,tminrep,simcur))<0 AND SUM(tmaxrep, ISCALX(ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, ISCALX(ac,tmaxrep,simcur)) / SUM(tminrep, ISCALX(ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

LABPARTRATXPPREP(simcur)$((SUM(tminrep, LABPARTRATX(tminrep,simcur))>0 AND SUM(tmaxrep, LABPARTRATX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, LABPARTRATX(tminrep,simcur))<0 AND SUM(tmaxrep, LABPARTRATX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, LABPARTRATX(tmaxrep,simcur)) / SUM(tminrep, LABPARTRATX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

MPSXPPREP(ins,simcur)$((SUM(tminrep, MPSX(ins,tminrep,simcur))>0 AND SUM(tmaxrep, MPSX(ins,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, MPSX(ins,tminrep,simcur))<0 AND SUM(tmaxrep, MPSX(ins,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, MPSX(ins,tmaxrep,simcur)) / SUM(tminrep, MPSX(ins,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

MPSADJXPPREP(simcur)$((SUM(tminrep, MPSADJX(tminrep,simcur))>0 AND SUM(tmaxrep, MPSADJX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, MPSADJX(tminrep,simcur))<0 AND SUM(tmaxrep, MPSADJX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, MPSADJX(tmaxrep,simcur)) / SUM(tminrep, MPSADJX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

MPSSCALXPPREP(simcur)$((SUM(tminrep, MPSSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, MPSSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, MPSSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, MPSSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, MPSSCALX(tmaxrep,simcur)) / SUM(tminrep, MPSSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

NDFGXPPREP(simcur)$((SUM(tminrep, NDFGX(tminrep,simcur))>0 AND SUM(tmaxrep, NDFGX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, NDFGX(tminrep,simcur))<0 AND SUM(tmaxrep, NDFGX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, NDFGX(tmaxrep,simcur)) / SUM(tminrep, NDFGX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

NFFGXPPREP(simcur)$((SUM(tminrep, NFFGX(tminrep,simcur))>0 AND SUM(tmaxrep, NFFGX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, NFFGX(tminrep,simcur))<0 AND SUM(tmaxrep, NFFGX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, NFFGX(tmaxrep,simcur)) / SUM(tminrep, NFFGX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

NFFINSXPPREP(simcur)$((SUM(tminrep, NFFINSX(tminrep,simcur))>0 AND SUM(tmaxrep, NFFINSX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, NFFINSX(tminrep,simcur))<0 AND SUM(tmaxrep, NFFINSX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, NFFINSX(tmaxrep,simcur)) / SUM(tminrep, NFFINSX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

NFFINSSCALXPPREP(simcur)$((SUM(tminrep, NFFINSSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, NFFINSSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, NFFINSSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, NFFINSSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, NFFINSSCALX(tmaxrep,simcur)) / SUM(tminrep, NFFINSSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

NGOVPAYABSXPPREP(acngovpay,simcur)$((SUM(tminrep, NGOVPAYABSX(acngovpay,tminrep,simcur))>0 AND SUM(tmaxrep, NGOVPAYABSX(acngovpay,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, NGOVPAYABSX(acngovpay,tminrep,simcur))<0 AND SUM(tmaxrep, NGOVPAYABSX(acngovpay,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, NGOVPAYABSX(acngovpay,tmaxrep,simcur)) / SUM(tminrep, NGOVPAYABSX(acngovpay,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

NGOVPAYGDPXPPREP(acngovpay,simcur)$((SUM(tminrep, NGOVPAYGDPX(acngovpay,tminrep,simcur))>0 AND SUM(tmaxrep, NGOVPAYGDPX(acngovpay,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, NGOVPAYGDPX(acngovpay,tminrep,simcur))<0 AND SUM(tmaxrep, NGOVPAYGDPX(acngovpay,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, NGOVPAYGDPX(acngovpay,tmaxrep,simcur)) / SUM(tminrep, NGOVPAYGDPX(acngovpay,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PAXPPREP(a,simcur)$((SUM(tminrep, PAX(a,tminrep,simcur))>0 AND SUM(tmaxrep, PAX(a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PAX(a,tminrep,simcur))<0 AND SUM(tmaxrep, PAX(a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PAX(a,tmaxrep,simcur)) / SUM(tminrep, PAX(a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PDDXPPREP(c,simcur)$((SUM(tminrep, PDDX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PDDX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PDDX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PDDX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PDDX(c,tmaxrep,simcur)) / SUM(tminrep, PDDX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PDSXPPREP(c,simcur)$((SUM(tminrep, PDSX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PDSX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PDSX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PDSX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PDSX(c,tmaxrep,simcur)) / SUM(tminrep, PDSX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PEXPPREP(c,simcur)$((SUM(tminrep, PEX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PEX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PEX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PEX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PEX(c,tmaxrep,simcur)) / SUM(tminrep, PEX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PKXPPREP(fcap,simcur)$((SUM(tminrep, PKX(fcap,tminrep,simcur))>0 AND SUM(tmaxrep, PKX(fcap,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PKX(fcap,tminrep,simcur))<0 AND SUM(tmaxrep, PKX(fcap,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PKX(fcap,tmaxrep,simcur)) / SUM(tminrep, PKX(fcap,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PMXPPREP(c,simcur)$((SUM(tminrep, PMX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PMX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PMX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PMX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PMX(c,tmaxrep,simcur)) / SUM(tminrep, PMX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PQDXPPREP(c,ac,simcur)$((SUM(tminrep, PQDX(c,ac,tminrep,simcur))>0 AND SUM(tmaxrep, PQDX(c,ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PQDX(c,ac,tminrep,simcur))<0 AND SUM(tmaxrep, PQDX(c,ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PQDX(c,ac,tmaxrep,simcur)) / SUM(tminrep, PQDX(c,ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PQSXPPREP(c,simcur)$((SUM(tminrep, PQSX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PQSX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PQSX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PQSX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PQSX(c,tmaxrep,simcur)) / SUM(tminrep, PQSX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PVAXPPREP(a,simcur)$((SUM(tminrep, PVAX(a,tminrep,simcur))>0 AND SUM(tmaxrep, PVAX(a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PVAX(a,tminrep,simcur))<0 AND SUM(tmaxrep, PVAX(a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PVAX(a,tmaxrep,simcur)) / SUM(tminrep, PVAX(a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

PXXPPREP(c,simcur)$((SUM(tminrep, PXX(c,tminrep,simcur))>0 AND SUM(tmaxrep, PXX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, PXX(c,tminrep,simcur))<0 AND SUM(tmaxrep, PXX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, PXX(c,tmaxrep,simcur)) / SUM(tminrep, PXX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QAXPPREP(a,simcur)$((SUM(tminrep, QAX(a,tminrep,simcur))>0 AND SUM(tmaxrep, QAX(a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QAX(a,tminrep,simcur))<0 AND SUM(tmaxrep, QAX(a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QAX(a,tmaxrep,simcur)) / SUM(tminrep, QAX(a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QDXPPREP(c,simcur)$((SUM(tminrep, QDX(c,tminrep,simcur))>0 AND SUM(tmaxrep, QDX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QDX(c,tminrep,simcur))<0 AND SUM(tmaxrep, QDX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QDX(c,tmaxrep,simcur)) / SUM(tminrep, QDX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QEXPPREP(c,simcur)$((SUM(tminrep, QEX(c,tminrep,simcur))>0 AND SUM(tmaxrep, QEX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QEX(c,tminrep,simcur))<0 AND SUM(tmaxrep, QEX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QEX(c,tmaxrep,simcur)) / SUM(tminrep, QEX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QFXPPREP(ac,acp,simcur)$((SUM(tminrep, QFX(ac,acp,tminrep,simcur))>0 AND SUM(tmaxrep, QFX(ac,acp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QFX(ac,acp,tminrep,simcur))<0 AND SUM(tmaxrep, QFX(ac,acp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QFX(ac,acp,tmaxrep,simcur)) / SUM(tminrep, QFX(ac,acp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QFHENDXPPREP(h,f,simcur)$((SUM(tminrep, QFHENDX(h,f,tminrep,simcur))>0 AND SUM(tmaxrep, QFHENDX(h,f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QFHENDX(h,f,tminrep,simcur))<0 AND SUM(tmaxrep, QFHENDX(h,f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QFHENDX(h,f,tmaxrep,simcur)) / SUM(tminrep, QFHENDX(h,f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QFHENDSCALXPPREP(f,simcur)$((SUM(tminrep, QFHENDSCALX(f,tminrep,simcur))>0 AND SUM(tmaxrep, QFHENDSCALX(f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QFHENDSCALX(f,tminrep,simcur))<0 AND SUM(tmaxrep, QFHENDSCALX(f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QFHENDSCALX(f,tmaxrep,simcur)) / SUM(tminrep, QFHENDSCALX(f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QFINSXPPREP(ins,ac,simcur)$((SUM(tminrep, QFINSX(ins,ac,tminrep,simcur))>0 AND SUM(tmaxrep, QFINSX(ins,ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QFINSX(ins,ac,tminrep,simcur))<0 AND SUM(tmaxrep, QFINSX(ins,ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QFINSX(ins,ac,tmaxrep,simcur)) / SUM(tminrep, QFINSX(ins,ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QFINSSCALXPPREP(f,simcur)$((SUM(tminrep, QFINSSCALX(f,tminrep,simcur))>0 AND SUM(tmaxrep, QFINSSCALX(f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QFINSSCALX(f,tminrep,simcur))<0 AND SUM(tmaxrep, QFINSSCALX(f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QFINSSCALX(f,tmaxrep,simcur)) / SUM(tminrep, QFINSSCALX(f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QFSXPPREP(f,simcur)$((SUM(tminrep, QFSX(f,tminrep,simcur))>0 AND SUM(tmaxrep, QFSX(f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QFSX(f,tminrep,simcur))<0 AND SUM(tmaxrep, QFSX(f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QFSX(f,tmaxrep,simcur)) / SUM(tminrep, QFSX(f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QGXPPREP(c,simcur)$((SUM(tminrep, QGX(c,tminrep,simcur))>0 AND SUM(tmaxrep, QGX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QGX(c,tminrep,simcur))<0 AND SUM(tmaxrep, QGX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QGX(c,tmaxrep,simcur)) / SUM(tminrep, QGX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QGSCALXPPREP(simcur)$((SUM(tminrep, QGSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, QGSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QGSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, QGSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QGSCALX(tmaxrep,simcur)) / SUM(tminrep, QGSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QHXPPREP(ac,h,simcur)$((SUM(tminrep, QHX(ac,h,tminrep,simcur))>0 AND SUM(tmaxrep, QHX(ac,h,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QHX(ac,h,tminrep,simcur))<0 AND SUM(tmaxrep, QHX(ac,h,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QHX(ac,h,tmaxrep,simcur)) / SUM(tminrep, QHX(ac,h,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QINTXPPREP(c,a,simcur)$((SUM(tminrep, QINTX(c,a,tminrep,simcur))>0 AND SUM(tmaxrep, QINTX(c,a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QINTX(c,a,tminrep,simcur))<0 AND SUM(tmaxrep, QINTX(c,a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QINTX(c,a,tmaxrep,simcur)) / SUM(tminrep, QINTX(c,a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QINVXPPREP(c,simcur)$((SUM(tminrep, QINVX(c,tminrep,simcur))>0 AND SUM(tmaxrep, QINVX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QINVX(c,tminrep,simcur))<0 AND SUM(tmaxrep, QINVX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QINVX(c,tmaxrep,simcur)) / SUM(tminrep, QINVX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QLABSCALXPPREP(simcur)$((SUM(tminrep, QLABSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, QLABSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QLABSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, QLABSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QLABSCALX(tmaxrep,simcur)) / SUM(tminrep, QLABSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QMXPPREP(c,simcur)$((SUM(tminrep, QMX(c,tminrep,simcur))>0 AND SUM(tmaxrep, QMX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QMX(c,tminrep,simcur))<0 AND SUM(tmaxrep, QMX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QMX(c,tmaxrep,simcur)) / SUM(tminrep, QMX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QNGOXPPREP(c,insngo,simcur)$((SUM(tminrep, QNGOX(c,insngo,tminrep,simcur))>0 AND SUM(tmaxrep, QNGOX(c,insngo,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QNGOX(c,insngo,tminrep,simcur))<0 AND SUM(tmaxrep, QNGOX(c,insngo,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QNGOX(c,insngo,tmaxrep,simcur)) / SUM(tminrep, QNGOX(c,insngo,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QNGOSCALXPPREP(insngo,simcur)$((SUM(tminrep, QNGOSCALX(insngo,tminrep,simcur))>0 AND SUM(tmaxrep, QNGOSCALX(insngo,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QNGOSCALX(insngo,tminrep,simcur))<0 AND SUM(tmaxrep, QNGOSCALX(insngo,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QNGOSCALX(insngo,tmaxrep,simcur)) / SUM(tminrep, QNGOSCALX(insngo,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QQXPPREP(c,simcur)$((SUM(tminrep, QQX(c,tminrep,simcur))>0 AND SUM(tmaxrep, QQX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QQX(c,tminrep,simcur))<0 AND SUM(tmaxrep, QQX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QQX(c,tmaxrep,simcur)) / SUM(tminrep, QQX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QTXPPREP(c,simcur)$((SUM(tminrep, QTX(c,tminrep,simcur))>0 AND SUM(tmaxrep, QTX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QTX(c,tminrep,simcur))<0 AND SUM(tmaxrep, QTX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QTX(c,tmaxrep,simcur)) / SUM(tminrep, QTX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QTRSTXPPREP(c,instrst,simcur)$((SUM(tminrep, QTRSTX(c,instrst,tminrep,simcur))>0 AND SUM(tmaxrep, QTRSTX(c,instrst,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QTRSTX(c,instrst,tminrep,simcur))<0 AND SUM(tmaxrep, QTRSTX(c,instrst,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QTRSTX(c,instrst,tmaxrep,simcur)) / SUM(tminrep, QTRSTX(c,instrst,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QTRSTSCALXPPREP(simcur)$((SUM(tminrep, QTRSTSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, QTRSTSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QTRSTSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, QTRSTSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QTRSTSCALX(tmaxrep,simcur)) / SUM(tminrep, QTRSTSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

QXXPPREP(c,simcur)$((SUM(tminrep, QXX(c,tminrep,simcur))>0 AND SUM(tmaxrep, QXX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, QXX(c,tminrep,simcur))<0 AND SUM(tmaxrep, QXX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, QXX(c,tmaxrep,simcur)) / SUM(tminrep, QXX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

RBTVATXPPREP(a,simcur)$((SUM(tminrep, RBTVATX(a,tminrep,simcur))>0 AND SUM(tmaxrep, RBTVATX(a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, RBTVATX(a,tminrep,simcur))<0 AND SUM(tmaxrep, RBTVATX(a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, RBTVATX(a,tmaxrep,simcur)) / SUM(tminrep, RBTVATX(a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

REXRXPPREP(simcur)$((SUM(tminrep, REXRX(tminrep,simcur))>0 AND SUM(tmaxrep, REXRX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, REXRX(tminrep,simcur))<0 AND SUM(tmaxrep, REXRX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, REXRX(tmaxrep,simcur)) / SUM(tminrep, REXRX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

RGDPFCXPPREP(simcur)$((SUM(tminrep, RGDPFCX(tminrep,simcur))>0 AND SUM(tmaxrep, RGDPFCX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, RGDPFCX(tminrep,simcur))<0 AND SUM(tmaxrep, RGDPFCX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, RGDPFCX(tmaxrep,simcur)) / SUM(tminrep, RGDPFCX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

RGDPMPXPPREP(simcur)$((SUM(tminrep, RGDPMPX(tminrep,simcur))>0 AND SUM(tmaxrep, RGDPMPX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, RGDPMPX(tminrep,simcur))<0 AND SUM(tmaxrep, RGDPMPX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, RGDPMPX(tmaxrep,simcur)) / SUM(tminrep, RGDPMPX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

RGPRIMDEFXPPREP(simcur)$((SUM(tminrep, RGPRIMDEFX(tminrep,simcur))>0 AND SUM(tmaxrep, RGPRIMDEFX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, RGPRIMDEFX(tminrep,simcur))<0 AND SUM(tmaxrep, RGPRIMDEFX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, RGPRIMDEFX(tmaxrep,simcur)) / SUM(tminrep, RGPRIMDEFX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

RNDFGXPPREP(simcur)$((SUM(tminrep, RNDFGX(tminrep,simcur))>0 AND SUM(tmaxrep, RNDFGX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, RNDFGX(tminrep,simcur))<0 AND SUM(tmaxrep, RNDFGX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, RNDFGX(tmaxrep,simcur)) / SUM(tminrep, RNDFGX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

SAVXPPREP(ac,simcur)$((SUM(tminrep, SAVX(ac,tminrep,simcur))>0 AND SUM(tmaxrep, SAVX(ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, SAVX(ac,tminrep,simcur))<0 AND SUM(tmaxrep, SAVX(ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, SAVX(ac,tmaxrep,simcur)) / SUM(tminrep, SAVX(ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

SAVFXPPREP(simcur)$((SUM(tminrep, SAVFX(tminrep,simcur))>0 AND SUM(tmaxrep, SAVFX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, SAVFX(tminrep,simcur))<0 AND SUM(tmaxrep, SAVFX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, SAVFX(tmaxrep,simcur)) / SUM(tminrep, SAVFX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

SHIFXPPREP(ins,f,simcur)$((SUM(tminrep, SHIFX(ins,f,tminrep,simcur))>0 AND SUM(tmaxrep, SHIFX(ins,f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, SHIFX(ins,f,tminrep,simcur))<0 AND SUM(tmaxrep, SHIFX(ins,f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, SHIFX(ins,f,tmaxrep,simcur)) / SUM(tminrep, SHIFX(ins,f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

SUBCXPPREP(c,ac,simcur)$((SUM(tminrep, SUBCX(c,ac,tminrep,simcur))>0 AND SUM(tmaxrep, SUBCX(c,ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, SUBCX(c,ac,tminrep,simcur))<0 AND SUM(tmaxrep, SUBCX(c,ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, SUBCX(c,ac,tmaxrep,simcur)) / SUM(tminrep, SUBCX(c,ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

SUBCSCALXPPREP(simcur)$((SUM(tminrep, SUBCSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, SUBCSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, SUBCSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, SUBCSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, SUBCSCALX(tmaxrep,simcur)) / SUM(tminrep, SUBCSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

SUBCTXPPREP(simcur)$((SUM(tminrep, SUBCTX(tminrep,simcur))>0 AND SUM(tmaxrep, SUBCTX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, SUBCTX(tminrep,simcur))<0 AND SUM(tmaxrep, SUBCTX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, SUBCTX(tmaxrep,simcur)) / SUM(tminrep, SUBCTX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TAXPPREP(a,simcur)$((SUM(tminrep, TAX(a,tminrep,simcur))>0 AND SUM(tmaxrep, TAX(a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TAX(a,tminrep,simcur))<0 AND SUM(tmaxrep, TAX(a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TAX(a,tmaxrep,simcur)) / SUM(tminrep, TAX(a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TASCALXPPREP(simcur)$((SUM(tminrep, TASCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TASCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TASCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TASCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TASCALX(tmaxrep,simcur)) / SUM(tminrep, TASCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TEXPPREP(c,simcur)$((SUM(tminrep, TEX(c,tminrep,simcur))>0 AND SUM(tmaxrep, TEX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TEX(c,tminrep,simcur))<0 AND SUM(tmaxrep, TEX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TEX(c,tmaxrep,simcur)) / SUM(tminrep, TEX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TESCALXPPREP(simcur)$((SUM(tminrep, TESCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TESCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TESCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TESCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TESCALX(tmaxrep,simcur)) / SUM(tminrep, TESCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TFXPPREP(f,simcur)$((SUM(tminrep, TFX(f,tminrep,simcur))>0 AND SUM(tmaxrep, TFX(f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TFX(f,tminrep,simcur))<0 AND SUM(tmaxrep, TFX(f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TFX(f,tmaxrep,simcur)) / SUM(tminrep, TFX(f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TFAXPPREP(f,a,simcur)$((SUM(tminrep, TFAX(f,a,tminrep,simcur))>0 AND SUM(tmaxrep, TFAX(f,a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TFAX(f,a,tminrep,simcur))<0 AND SUM(tmaxrep, TFAX(f,a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TFAX(f,a,tmaxrep,simcur)) / SUM(tminrep, TFAX(f,a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TFASCALXPPREP(simcur)$((SUM(tminrep, TFASCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TFASCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TFASCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TFASCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TFASCALX(tmaxrep,simcur)) / SUM(tminrep, TFASCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TFPXPPREP(a,simcur)$((SUM(tminrep, TFPX(a,tminrep,simcur))>0 AND SUM(tmaxrep, TFPX(a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TFPX(a,tminrep,simcur))<0 AND SUM(tmaxrep, TFPX(a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TFPX(a,tmaxrep,simcur)) / SUM(tminrep, TFPX(a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TFPSCALXPPREP(simcur)$((SUM(tminrep, TFPSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TFPSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TFPSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TFPSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TFPSCALX(tmaxrep,simcur)) / SUM(tminrep, TFPSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TFSCALXPPREP(simcur)$((SUM(tminrep, TFSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TFSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TFSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TFSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TFSCALX(tmaxrep,simcur)) / SUM(tminrep, TFSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TMXPPREP(c,simcur)$((SUM(tminrep, TMX(c,tminrep,simcur))>0 AND SUM(tmaxrep, TMX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TMX(c,tminrep,simcur))<0 AND SUM(tmaxrep, TMX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TMX(c,tmaxrep,simcur)) / SUM(tminrep, TMX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TMSCALXPPREP(simcur)$((SUM(tminrep, TMSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TMSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TMSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TMSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TMSCALX(tmaxrep,simcur)) / SUM(tminrep, TMSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TQXPPREP(c,simcur)$((SUM(tminrep, TQX(c,tminrep,simcur))>0 AND SUM(tmaxrep, TQX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TQX(c,tminrep,simcur))<0 AND SUM(tmaxrep, TQX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TQX(c,tmaxrep,simcur)) / SUM(tminrep, TQX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TQSCALXPPREP(simcur)$((SUM(tminrep, TQSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TQSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TQSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TQSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TQSCALX(tmaxrep,simcur)) / SUM(tminrep, TQSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TRDGDPXPPREP(simcur)$((SUM(tminrep, TRDGDPX(tminrep,simcur))>0 AND SUM(tmaxrep, TRDGDPX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TRDGDPX(tminrep,simcur))<0 AND SUM(tmaxrep, TRDGDPX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TRDGDPX(tmaxrep,simcur)) / SUM(tminrep, TRDGDPX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TRIIXPPREP(ins,insp,simcur)$((SUM(tminrep, TRIIX(ins,insp,tminrep,simcur))>0 AND SUM(tmaxrep, TRIIX(ins,insp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TRIIX(ins,insp,tminrep,simcur))<0 AND SUM(tmaxrep, TRIIX(ins,insp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TRIIX(ins,insp,tmaxrep,simcur)) / SUM(tminrep, TRIIX(ins,insp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TRNSFRXPPREP(ac,acp,simcur)$((SUM(tminrep, TRNSFRX(ac,acp,tminrep,simcur))>0 AND SUM(tmaxrep, TRNSFRX(ac,acp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TRNSFRX(ac,acp,tminrep,simcur))<0 AND SUM(tmaxrep, TRNSFRX(ac,acp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TRNSFRX(ac,acp,tmaxrep,simcur)) / SUM(tminrep, TRNSFRX(ac,acp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TRNSFRSCALXPPREP(ac,simcur)$((SUM(tminrep, TRNSFRSCALX(ac,tminrep,simcur))>0 AND SUM(tmaxrep, TRNSFRSCALX(ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TRNSFRSCALX(ac,tminrep,simcur))<0 AND SUM(tmaxrep, TRNSFRSCALX(ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TRNSFRSCALX(ac,tmaxrep,simcur)) / SUM(tminrep, TRNSFRSCALX(ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TRSMRECXPPREP(simcur)$((SUM(tminrep, TRSMRECX(tminrep,simcur))>0 AND SUM(tmaxrep, TRSMRECX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TRSMRECX(tminrep,simcur))<0 AND SUM(tmaxrep, TRSMRECX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TRSMRECX(tmaxrep,simcur)) / SUM(tminrep, TRSMRECX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TVACXPPREP(c,ac,simcur)$((SUM(tminrep, TVACX(c,ac,tminrep,simcur))>0 AND SUM(tmaxrep, TVACX(c,ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TVACX(c,ac,tminrep,simcur))<0 AND SUM(tmaxrep, TVACX(c,ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TVACX(c,ac,tmaxrep,simcur)) / SUM(tminrep, TVACX(c,ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TVACSCALXPPREP(simcur)$((SUM(tminrep, TVACSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TVACSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TVACSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TVACSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TVACSCALX(tmaxrep,simcur)) / SUM(tminrep, TVACSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TYXPPREP(ins,simcur)$((SUM(tminrep, TYX(ins,tminrep,simcur))>0 AND SUM(tmaxrep, TYX(ins,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TYX(ins,tminrep,simcur))<0 AND SUM(tmaxrep, TYX(ins,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TYX(ins,tmaxrep,simcur)) / SUM(tminrep, TYX(ins,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

TYSCALXPPREP(simcur)$((SUM(tminrep, TYSCALX(tminrep,simcur))>0 AND SUM(tmaxrep, TYSCALX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, TYSCALX(tminrep,simcur))<0 AND SUM(tmaxrep, TYSCALX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, TYSCALX(tmaxrep,simcur)) / SUM(tminrep, TYSCALX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

UERATXPPREP(ac,simcur)$((SUM(tminrep, UERATX(ac,tminrep,simcur))>0 AND SUM(tmaxrep, UERATX(ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, UERATX(ac,tminrep,simcur))<0 AND SUM(tmaxrep, UERATX(ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, UERATX(ac,tmaxrep,simcur)) / SUM(tminrep, UERATX(ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

WALRASXPPREP(simcur)$((SUM(tminrep, WALRASX(tminrep,simcur))>0 AND SUM(tmaxrep, WALRASX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, WALRASX(tminrep,simcur))<0 AND SUM(tmaxrep, WALRASX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, WALRASX(tmaxrep,simcur)) / SUM(tminrep, WALRASX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

WFXPPREP(f,simcur)$((SUM(tminrep, WFX(f,tminrep,simcur))>0 AND SUM(tmaxrep, WFX(f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, WFX(f,tminrep,simcur))<0 AND SUM(tmaxrep, WFX(f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, WFX(f,tmaxrep,simcur)) / SUM(tminrep, WFX(f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

WFAXPPREP(f,a,simcur)$((SUM(tminrep, WFAX(f,a,tminrep,simcur))>0 AND SUM(tmaxrep, WFAX(f,a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, WFAX(f,a,tminrep,simcur))<0 AND SUM(tmaxrep, WFAX(f,a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, WFAX(f,a,tmaxrep,simcur)) / SUM(tminrep, WFAX(f,a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

WFAVGXPPREP(ac,simcur)$((SUM(tminrep, WFAVGX(ac,tminrep,simcur))>0 AND SUM(tmaxrep, WFAVGX(ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, WFAVGX(ac,tminrep,simcur))<0 AND SUM(tmaxrep, WFAVGX(ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, WFAVGX(ac,tmaxrep,simcur)) / SUM(tminrep, WFAVGX(ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

WFDISTXPPREP(f,a,simcur)$((SUM(tminrep, WFDISTX(f,a,tminrep,simcur))>0 AND SUM(tmaxrep, WFDISTX(f,a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, WFDISTX(f,a,tminrep,simcur))<0 AND SUM(tmaxrep, WFDISTX(f,a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, WFDISTX(f,a,tmaxrep,simcur)) / SUM(tminrep, WFDISTX(f,a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YFXPPREP(f,simcur)$((SUM(tminrep, YFX(f,tminrep,simcur))>0 AND SUM(tmaxrep, YFX(f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YFX(f,tminrep,simcur))<0 AND SUM(tmaxrep, YFX(f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YFX(f,tmaxrep,simcur)) / SUM(tminrep, YFX(f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YGXPPREP(simcur)$((SUM(tminrep, YGX(tminrep,simcur))>0 AND SUM(tmaxrep, YGX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YGX(tminrep,simcur))<0 AND SUM(tmaxrep, YGX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YGX(tmaxrep,simcur)) / SUM(tminrep, YGX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YIXPPREP(ins,simcur)$((SUM(tminrep, YIX(ins,tminrep,simcur))>0 AND SUM(tmaxrep, YIX(ins,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YIX(ins,tminrep,simcur))<0 AND SUM(tmaxrep, YIX(ins,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YIX(ins,tmaxrep,simcur)) / SUM(tminrep, YIX(ins,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YIFXPPREP(ins,f,simcur)$((SUM(tminrep, YIFX(ins,f,tminrep,simcur))>0 AND SUM(tmaxrep, YIFX(ins,f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YIFX(ins,f,tminrep,simcur))<0 AND SUM(tmaxrep, YIFX(ins,f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YIFX(ins,f,tmaxrep,simcur)) / SUM(tminrep, YIFX(ins,f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YTAXIMPXPPREP(simcur)$((SUM(tminrep, YTAXIMPX(tminrep,simcur))>0 AND SUM(tmaxrep, YTAXIMPX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YTAXIMPX(tminrep,simcur))<0 AND SUM(tmaxrep, YTAXIMPX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YTAXIMPX(tmaxrep,simcur)) / SUM(tminrep, YTAXIMPX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YTAXEXPXPPREP(simcur)$((SUM(tminrep, YTAXEXPX(tminrep,simcur))>0 AND SUM(tmaxrep, YTAXEXPX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YTAXEXPX(tminrep,simcur))<0 AND SUM(tmaxrep, YTAXEXPX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YTAXEXPX(tmaxrep,simcur)) / SUM(tminrep, YTAXEXPX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

YTAXVATXPPREP(simcur)$((SUM(tminrep, YTAXVATX(tminrep,simcur))>0 AND SUM(tmaxrep, YTAXVATX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, YTAXVATX(tminrep,simcur))<0 AND SUM(tmaxrep, YTAXVATX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, YTAXVATX(tmaxrep,simcur)) / SUM(tminrep, YTAXVATX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );


* other reports

popXPPREP(ac,simcur)$((SUM(tminrep, popX(ac,tminrep,simcur))>0 AND SUM(tmaxrep, popX(ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, popX(ac,tminrep,simcur))<0 AND SUM(tmaxrep, popX(ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, popX(ac,tmaxrep,simcur)) / SUM(tminrep, popX(ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

pweXPPREP(c,simcur)$((SUM(tminrep, pweX(c,tminrep,simcur))>0 AND SUM(tmaxrep, pweX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, pweX(c,tminrep,simcur))<0 AND SUM(tmaxrep, pweX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, pweX(c,tmaxrep,simcur)) / SUM(tminrep, pweX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

pwmXPPREP(c,simcur)$((SUM(tminrep, pwmX(c,tminrep,simcur))>0 AND SUM(tmaxrep, pwmX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, pwmX(c,tminrep,simcur))<0 AND SUM(tmaxrep, pwmX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, pwmX(c,tmaxrep,simcur)) / SUM(tminrep, pwmX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

*qa2XPPREP(a,simcur)$((SUM(tminrep, qa2X(a,tminrep,simcur))>0 AND SUM(tmaxrep, qa2X(a,tmaxrep,simcur))>0) 
*  OR (SUM(tminrep, qa2X(a,tminrep,simcur))<0 AND SUM(tmaxrep, qa2X(a,tmaxrep,simcur))<0)) = 
*  100 * ( ( SUM(tmaxrep, qa2X(a,tmaxrep,simcur)) / SUM(tminrep, qa2X(a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

qgbXPPREP(c,simcur)$((SUM(tminrep, qgbX(c,tminrep,simcur))>0 AND SUM(tmaxrep, qgbX(c,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, qgbX(c,tminrep,simcur))<0 AND SUM(tmaxrep, qgbX(c,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, qgbX(c,tmaxrep,simcur)) / SUM(tminrep, qgbX(c,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

qhpcXPPREP(ac,simcur)$((SUM(tminrep, qhpcX(ac,tminrep,simcur))>0 AND SUM(tmaxrep, qhpcX(ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, qhpcX(ac,tminrep,simcur))<0 AND SUM(tmaxrep, qhpcX(ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, qhpcX(ac,tmaxrep,simcur)) / SUM(tminrep, qhpcX(ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

yhpcXPPREP(ac,simcur)$((SUM(tminrep, yhpcX(ac,tminrep,simcur))>0 AND SUM(tmaxrep, yhpcX(ac,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, yhpcX(ac,tminrep,simcur))<0 AND SUM(tmaxrep, yhpcX(ac,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, yhpcX(ac,tmaxrep,simcur)) / SUM(tminrep, yhpcX(ac,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

rsavgXPPREP(simcur)$((SUM(tminrep, rsavgX(tminrep,simcur))>0 AND SUM(tmaxrep, rsavgX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, rsavgX(tminrep,simcur))<0 AND SUM(tmaxrep, rsavgX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, rsavgX(tmaxrep,simcur)) / SUM(tminrep, rsavgX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

savgXPPREP(simcur)$((SUM(tminrep, savgX(tminrep,simcur))>0 AND SUM(tmaxrep, savgX(tmaxrep,simcur))>0) 
  OR (SUM(tminrep, savgX(tminrep,simcur))<0 AND SUM(tmaxrep, savgX(tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, savgX(tmaxrep,simcur)) / SUM(tminrep, savgX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

*tfpaXPPREP(a,simcur)$((SUM(tminrep, tfpaX(a,tminrep,simcur))>0 AND SUM(tmaxrep, tfpaX(a,tmaxrep,simcur))>0) 
*  OR (SUM(tminrep, tfpaX(a,tminrep,simcur))<0 AND SUM(tmaxrep, tfpaX(a,tmaxrep,simcur))<0)) = 
*  100 * ( ( SUM(tmaxrep, tfpaX(a,tmaxrep,simcur)) / SUM(tminrep, tfpaX(a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

*tfpatotXPPREP(simcur)$((SUM(tminrep, tfpatotX(tminrep,simcur))>0 AND SUM(tmaxrep, tfpatotX(tmaxrep,simcur))>0) 
*  OR (SUM(tminrep, tfpatotX(tminrep,simcur))<0 AND SUM(tmaxrep, tfpatotX(tmaxrep,simcur))<0)) = 
*  100 * ( ( SUM(tmaxrep, tfpatotX(tmaxrep,simcur)) / SUM(tminrep, tfpatotX(tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

wfarealXPPREP(f,a,simcur)$((SUM(tminrep, wfarealX(f,a,tminrep,simcur))>0 AND SUM(tmaxrep, wfarealX(f,a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, wfarealX(f,a,tminrep,simcur))<0 AND SUM(tmaxrep, wfarealX(f,a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, wfarealX(f,a,tmaxrep,simcur)) / SUM(tminrep, wfarealX(f,a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

wfrealXPPREP(f,simcur)$((SUM(tminrep, wfrealX(f,tminrep,simcur))>0 AND SUM(tmaxrep, wfrealX(f,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, wfrealX(f,tminrep,simcur))<0 AND SUM(tmaxrep, wfrealX(f,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, wfrealX(f,tmaxrep,simcur)) / SUM(tminrep, wfrealX(f,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

shvatrbtXPPREP(c,a,simcur)$((SUM(tminrep, shvatrbtX(c,a,tminrep,simcur))>0 AND SUM(tmaxrep, shvatrbtX(c,a,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, shvatrbtX(c,a,tminrep,simcur))<0 AND SUM(tmaxrep, shvatrbtX(c,a,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, shvatrbtX(c,a,tmaxrep,simcur)) / SUM(tminrep, shvatrbtX(c,a,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

gdpindicXPPREP(igdp,kgdp,simcur)$((SUM(tminrep, gdpindic(igdp,kgdp,tminrep,simcur))>0 AND SUM(tmaxrep, gdpindic(igdp,kgdp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, gdpindic(igdp,kgdp,tminrep,simcur))<0 AND SUM(tmaxrep, gdpindic(igdp,kgdp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, gdpindic(igdp,kgdp,tmaxrep,simcur)) / SUM(tminrep, gdpindic(igdp,kgdp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

gdpexpindicXPPREP(maccol,kgdp,simcur)$((SUM(tminrep, gdpexpindic(maccol,kgdp,tminrep,simcur))>0 AND SUM(tmaxrep, gdpexpindic(maccol,kgdp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, gdpexpindic(maccol,kgdp,tminrep,simcur))<0 AND SUM(tmaxrep, gdpexpindic(maccol,kgdp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, gdpexpindic(maccol,kgdp,tmaxrep,simcur)) / SUM(tminrep, gdpexpindic(maccol,kgdp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

sectorindicXPPREP(ac,sectorcol3,kgdp,simcur)$((SUM(tminrep, sectorindic(ac,sectorcol3,kgdp,tminrep,simcur))>0 AND SUM(tmaxrep, sectorindic(ac,sectorcol3,kgdp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, sectorindic(ac,sectorcol3,kgdp,tminrep,simcur))<0 AND SUM(tmaxrep, sectorindic(ac,sectorcol3,kgdp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, sectorindic(ac,sectorcol3,kgdp,tmaxrep,simcur)) / SUM(tminrep, sectorindic(ac,sectorcol3,kgdp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

sectorindic2XPPREP(acrep,sectorcol3,kgdp,simcur)$((SUM(tminrep, sectorindic2(acrep,sectorcol3,kgdp,tminrep,simcur))>0 AND SUM(tmaxrep, sectorindic2(acrep,sectorcol3,kgdp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, sectorindic2(acrep,sectorcol3,kgdp,tminrep,simcur))<0 AND SUM(tmaxrep, sectorindic2(acrep,sectorcol3,kgdp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, sectorindic2(acrep,sectorcol3,kgdp,tmaxrep,simcur)) / SUM(tminrep, sectorindic2(acrep,sectorcol3,kgdp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

emiindicXPPREP(emicol,ac,acp,simcur)$((SUM(tminrep, emiindic(emicol,ac,acp,tminrep,simcur))>0 AND SUM(tmaxrep, emiindic(emicol,ac,acp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, emiindic(emicol,ac,acp,tminrep,simcur))<0 AND SUM(tmaxrep, emiindic(emicol,ac,acp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, emiindic(emicol,ac,acp,tmaxrep,simcur)) / SUM(tminrep, emiindic(emicol,ac,acp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

emiindic2XPPREP(emicol,acrep,acrepp,simcur)$((SUM(tminrep, emiindic2(emicol,acrep,acrepp,tminrep,simcur))>0 AND SUM(tmaxrep, emiindic2(emicol,acrep,acrepp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, emiindic2(emicol,acrep,acrepp,tminrep,simcur))<0 AND SUM(tmaxrep, emiindic2(emicol,acrep,acrepp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, emiindic2(emicol,acrep,acrepp,tmaxrep,simcur)) / SUM(tminrep, emiindic2(emicol,acrep,acrepp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

SIMSAMPPREP(ac,acp,simcur)$((SUM(tminrep, SIMSAM(ac,acp,tminrep,simcur))>0 AND SUM(tmaxrep, SIMSAM(ac,acp,tmaxrep,simcur))>0) 
  OR (SUM(tminrep, SIMSAM(ac,acp,tminrep,simcur))<0 AND SUM(tmaxrep, SIMSAM(ac,acp,tmaxrep,simcur))<0)) = 
  100 * ( ( SUM(tmaxrep, SIMSAM(ac,acp,tmaxrep,simcur)) / SUM(tminrep, SIMSAM(ac,acp,tminrep,simcur)) ) ** (1/tsolnbrep) - 1 );

  


DISPLAY "#### END: repperc-xpprep.inc";