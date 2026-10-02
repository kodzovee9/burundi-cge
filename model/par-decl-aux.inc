* par-decl-aux.inc

DISPLAY "#### START: par-decl-aux.inc";

* START: Parameters for initial variable values - declaration =======



* level endogenous variables (name of variable  + X)  
* % deviation from baseline result (name of variable + XP)
* average % annual growth rates bt tmin and tmax (name of variable + XPP)
* average % annual growth rates bt tminrep and tmaxrep (name of variable + XPPREP)
* % annual growth from preceding period (name of variable + XPY)


PARAMETER
  PREXR0(t)    exchange rate premium (ref0)
  PREXR00    exchange rate premium (base-yr)
  PREXRX(t,sim)    exchange rate premium (level)
  PREXRXP(t,sim)    exchange rate premium (% deviation from base in t)
  PREXRXPP(sim)    exchange rate premium (avg annual % growth tmin – tmax)
  PREXRXPPREP(sim)    exchange rate premium (avg annual % growth tminrep – tmaxrep)
  PREXRXPY(t,sim)    exchange rate premium (annual % growth from t-1)

  YPREXRT0(c,t)    total rents from exchage rate premium in commodity c (ref0)
  YPREXRT00(c)    total rents from exchage rate premium in commodity c (base-yr)
  YPREXRTX(c,t,sim)    total rents from exchage rate premium in commodity c (level)
  YPREXRTXP(c,t,sim)    total rents from exchage rate premium in commodity c (% deviation from base in t)
  YPREXRTXPP(c,sim)    total rents from exchage rate premium in commodity c (avg annual % growth tmin – tmax)
  YPREXRTXPPREP(c,sim)    total rents from exchage rate premium in commodity c (avg annual % growth tminrep – tmaxrep)
  YPREXRTXPY(c,t,sim)    total rents from exchage rate premium in commodity c (annual % growth from t-1)

  YPREXR0(c,ac,t)    rents from exchage rate premium in commodity c to demander ac (ref0)
  YPREXR00(c,ac)    rents from exchage rate premium in commodity c to demander ac (base-yr)
  YPREXRX(c,ac,t,sim)    rents from exchage rate premium in commodity c to demander ac (level)
  YPREXRXP(c,ac,t,sim)    rents from exchage rate premium in commodity c to demander ac (% deviation from base in t)
  YPREXRXPP(c,ac,sim)    rents from exchage rate premium in commodity c to demander ac (avg annual % growth tmin – tmax)
  YPREXRXPPREP(c,ac,sim)    rents from exchage rate premium in commodity c to demander ac (avg annual % growth tminrep – tmaxrep)
  YPREXRXPY(c,ac,t,sim)    rents from exchage rate premium in commodity c to demander ac (annual % growth from t-1)

  PRQMBAR0(c,t)    unit rent of import quota on commodity c (ref0)
  PRQMBAR00(c)    unit rent of import quota on commodity c (base-yr)
  PRQMBARX(c,t,sim)    unit rent of import quota on commodity c (level)
  PRQMBARXP(c,t,sim)    unit rent of import quota on commodity c (% deviation from base in t)
  PRQMBARXPP(c,sim)    unit rent of import quota on commodity c (avg annual % growth tmin – tmax)
  PRQMBARXPPREP(c,sim)    unit rent of import quota on commodity c (avg annual % growth tminrep – tmaxrep)
  PRQMBARXPY(c,t,sim)    unit rent of import quota on commodity c (annual % growth from t-1)

  YPRQMBART0(c,t)    total import quota rents from commodity c (ref0)
  YPRQMBART00(c)    total import quota rents from commodity c (base-yr)
  YPRQMBARTX(c,t,sim)    total import quota rents from commodity c (level)
  YPRQMBARTXP(c,t,sim)    total import quota rents from commodity c (% deviation from base in t)
  YPRQMBARTXPP(c,sim)    total import quota rents from commodity c (avg annual % growth tmin – tmax)
  YPRQMBARTXPPREP(c,sim)    total import quota rents from commodity c (avg annual % growth tminrep – tmaxrep)
  YPRQMBARTXPY(c,t,sim)    total import quota rents from commodity c (annual % growth from t-1)

  YPRQMBAR0(c,ac,t)    import quota rents from commodity c to demander ac (ref0)
  YPRQMBAR00(c,ac)    import quota rents from commodity c to demander ac (base-yr)
  YPRQMBARX(c,ac,t,sim)    import quota rents from commodity c to demander ac (level)
  YPRQMBARXP(c,ac,t,sim)    import quota rents from commodity c to demander ac (% deviation from base in t)
  YPRQMBARXPP(c,ac,sim)    import quota rents from commodity c to demander ac (avg annual % growth tmin – tmax)
  YPRQMBARXPPREP(c,ac,sim)    import quota rents from commodity c to demander ac (avg annual % growth tminrep – tmaxrep)
  YPRQMBARXPY(c,ac,t,sim)    import quota rents from commodity c to demander ac (annual % growth from t-1)

  FPRDASCAL00    scaling factor factor-specific productivity
  FPRDASCAL0(t)  scaling factor factor-specific productivity

  PWE0(c,t)    export price for c (foreign currency) (ref0)
  PWE00(c)    export price for c (foreign currency) (base-yr)
  PWEX(c,t,sim)    export price for c (foreign currency) (level)
  PWEXP(c,t,sim)    export price for c (foreign currency) (% deviation from base in t)
  PWEXPP(c,sim)    export price for c (foreign currency) (avg annual % growth tmin – tmax)
  PWEXPPREP(c,sim)    export price for c (foreign currency) (avg annual % growth tminrep – tmaxrep)
  PWEXPY(c,t,sim)    export price for c (foreign currency) (annual % growth from t-1)

  PWM0(c,t)    import price for c (foreign currency) (ref0)
  PWM00(c)    import price for c (foreign currency) (base-yr)
  PWMX(c,t,sim)    import price for c (foreign currency) (level)
  PWMXP(c,t,sim)    import price for c (foreign currency) (% deviation from base in t)
  PWMXPP(c,sim)    import price for c (foreign currency) (avg annual % growth tmin – tmax)
  PWMXPPREP(c,sim)    import price for c (foreign currency) (avg annual % growth tminrep – tmaxrep)
  PWMXPY(c,t,sim)    import price for c (foreign currency) (annual % growth from t-1)


  QEMI0(ghg,ac,acp,t)    emissions of from commodity c by emitter ac (ref0)
  QEMI00(ghg,ac,acp)    emissions of from commodity c by emitter ac (base-yr)
  QEMIX(ac,ac,acp,t,sim)    emissions of from commodity c by emitter ac (level)
  QEMIXP(ac,ac,acp,t,sim)    emissions of from commodity c by emitter ac (% deviation from base in t)
  QEMIXPP(ac,ac,acp,sim)    emissions of from commodity c by emitter ac (avg annual % growth tmin – tmax)
  QEMIXPPREP(ac,ac,acp,sim)    emissions of from commodity c by emitter ac (avg annual % growth tminrep – tmaxrep)
  QEMIXPY(ac,ac,acp,t,sim)    emissions of from commodity c by emitter ac (annual % growth from t-1)

  QXAC0(a,c,t)    quantity of ouput of commodity c from activity a (ref0)
  QXAC00(a,c)    quantity of ouput of commodity c from activity a (base-yr)
  QXACX(a,c,t,sim)    quantity of ouput of commodity c from activity a (level)
  QXACXP(a,c,t,sim)    quantity of ouput of commodity c from activity a (% deviation from base in t)
  QXACXPP(a,c,sim)    quantity of ouput of commodity c from activity a (avg annual % growth tmin – tmax)
  QXACXPPREP(a,c,sim)    quantity of ouput of commodity c from activity a (avg annual % growth tminrep – tmaxrep)
  QXACXPY(a,c,t,sim)    quantity of ouput of commodity c from activity a (annual % growth from t-1)

  PXAC0(a,c,t)    price of commodity c from activity a (ref0)
  PXAC00(a,c)    price of commodity c from activity a (base-yr)
  PXACX(a,c,t,sim)    price of commodity c from activity a (level)
  PXACXP(a,c,t,sim)    price of commodity c from activity a (% deviation from base in t)
  PXACXPP(a,c,sim)    price of commodity c from activity a (avg annual % growth tmin – tmax)
  PXACXPPREP(a,c,sim)    price of commodity c from activity a (avg annual % growth tminrep – tmaxrep)
  PXACXPY(a,c,t,sim)    price of commodity c from activity a (annual % growth from t-1)



  ABSNOM0(t)    total nominal absorption (ref0)
  ABSNOM00    total nominal absorption (base-yr)
  ABSNOMX(t,sim)    total nominal absorption (level)
  ABSNOMXP(t,sim)    total nominal absorption (% deviation from base in t)
  ABSNOMXPP(sim)    total nominal absorption (avg annual % growth tmin – tmax)
  ABSNOMXPPREP(sim)    total nominal absorption (avg annual % growth tminrep – tmaxrep)
  ABSNOMXPY(t,sim)    total nominal absorption (annual % growth from t-1)

  CPI0(t)    consumer price index (ref0)
  CPI00    consumer price index (base-yr)
  CPIX(t,sim)    consumer price index (level)
  CPIXP(t,sim)    consumer price index (% deviation from base in t)
  CPIXPP(sim)    consumer price index (avg annual % growth tmin – tmax)
  CPIXPPREP(sim)    consumer price index (avg annual % growth tminrep – tmaxrep)
  CPIXPY(t,sim)    consumer price index (annual % growth from t-1)

  DKA0(f,a,t)    non-gov investment by destination (ref0)
  DKA00(f,a)    non-gov investment by destination (base-yr)
  DKAX(f,a,t,sim)    non-gov investment by destination (level)
  DKAXP(f,a,t,sim)    non-gov investment by destination (% deviation from base in t)
  DKAXPP(f,a,sim)    non-gov investment by destination (avg annual % growth tmin – tmax)
  DKAXPPREP(f,a,sim)    non-gov investment by destination (avg annual % growth tminrep – tmaxrep)
  DKAXPY(f,a,t,sim)    non-gov investment by destination (annual % growth from t-1)

  DKINS0(ins2,f,t)    investment by institution i in capital stock f (ref0)
  DKINS00(ins2,f)    investment by institution i in capital stock f (base-yr)
  DKINSX(ins2,f,t,sim)    investment by institution i in capital stock f (level)
  DKINSXP(ins2,f,t,sim)    investment by institution i in capital stock f (% deviation from base in t)
  DKINSXPP(ins2,f,sim)    investment by institution i in capital stock f (avg annual % growth tmin – tmax)
  DKINSXPPREP(ins2,f,sim)    investment by institution i in capital stock f (avg annual % growth tminrep – tmaxrep)
  DKINSXPY(ins2,f,t,sim)    investment by institution i in capital stock f (annual % growth from t-1)

  DPI0(t)    index for domestic producer prices (PDS-based) (ref0)
  DPI00    index for domestic producer prices (PDS-based) (base-yr)
  DPIX(t,sim)    index for domestic producer prices (PDS-based) (level)
  DPIXP(t,sim)    index for domestic producer prices (PDS-based) (% deviation from base in t)
  DPIXPP(sim)    index for domestic producer prices (PDS-based) (avg annual % growth tmin – tmax)
  DPIXPPREP(sim)    index for domestic producer prices (PDS-based) (avg annual % growth tminrep – tmaxrep)
  DPIXPY(t,sim)    index for domestic producer prices (PDS-based) (annual % growth from t-1)

  EG0(t)    total current government expenditure (ref0)
  EG00    total current government expenditure (base-yr)
  EGX(t,sim)    total current government expenditure (level)
  EGXP(t,sim)    total current government expenditure (% deviation from base in t)
  EGXPP(sim)    total current government expenditure (avg annual % growth tmin – tmax)
  EGXPPREP(sim)    total current government expenditure (avg annual % growth tminrep – tmaxrep)
  EGXPY(t,sim)    total current government expenditure (annual % growth from t-1)

  EH0(h,t)    household consumption expenditure (ref0)
  EH00(h)    household consumption expenditure (base-yr)
  EHX(h,t,sim)    household consumption expenditure (level)
  EHXP(h,t,sim)    household consumption expenditure (% deviation from base in t)
  EHXPP(h,sim)    household consumption expenditure (avg annual % growth tmin – tmax)
  EHXPPREP(h,sim)    household consumption expenditure (avg annual % growth tminrep – tmaxrep)
  EHXPY(h,t,sim)    household consumption expenditure (annual % growth from t-1)

  EXR0(t)    exchange rate (dom. currency per unit of for. currency) (ref0)
  EXR00    exchange rate (dom. currency per unit of for. currency) (base-yr)
  EXRX(t,sim)    exchange rate (dom. currency per unit of for. currency) (level)
  EXRXP(t,sim)    exchange rate (dom. currency per unit of for. currency) (% deviation from base in t)
  EXRXPP(sim)    exchange rate (dom. currency per unit of for. currency) (avg annual % growth tmin – tmax)
  EXRXPPREP(sim)    exchange rate (dom. currency per unit of for. currency) (avg annual % growth tminrep – tmaxrep)
  EXRXPY(t,sim)    exchange rate (dom. currency per unit of for. currency) (annual % growth from t-1)

  FBOR0(ins2,t)    foreign borrowing inst insd (ref0)
  FBOR00(ins2)    foreign borrowing inst insd (base-yr)
  FBORX(ins2,t,sim)    foreign borrowing inst insd (level)
  FBORXP(ins2,t,sim)    foreign borrowing inst insd (% deviation from base in t)
  FBORXPP(ins2,sim)    foreign borrowing inst insd (avg annual % growth tmin – tmax)
  FBORXPPREP(ins2,sim)    foreign borrowing inst insd (avg annual % growth tminrep – tmaxrep)
  FBORXPY(ins2,t,sim)    foreign borrowing inst insd (annual % growth from t-1)

  FDEBT0(ins2,t)    foreign debt stock inst insd (ref0)
  FDEBT00(ins2)    foreign debt stock inst insd (base-yr)
  FDEBTX(ins2,t,sim)    foreign debt stock inst insd (level)
  FDEBTXP(ins2,t,sim)    foreign debt stock inst insd (% deviation from base in t)
  FDEBTXPP(ins2,sim)    foreign debt stock inst insd (avg annual % growth tmin – tmax)
  FDEBTXPPREP(ins2,sim)    foreign debt stock inst insd (avg annual % growth tminrep – tmaxrep)
  FDEBTXPY(ins2,t,sim)    foreign debt stock inst insd (annual % growth from t-1)

  FDISCAL0(t)    scaling factor for FDI (ref0)
  FDISCAL00    scaling factor for FDI (base-yr)
  FDISCALX(t,sim)    scaling factor for FDI (level)
  FDISCALXP(t,sim)    scaling factor for FDI (% deviation from base in t)
  FDISCALXPP(sim)    scaling factor for FDI (avg annual % growth tmin – tmax)
  FDISCALXPPREP(sim)    scaling factor for FDI (avg annual % growth tminrep – tmaxrep)
  FDISCALXPY(t,sim)    scaling factor for FDI (annual % growth from t-1)

  FPRDA0(f,a,t)    productivity term for factor f in act a (ref0)
  FPRDA00(f,a)    productivity term for factor f in act a (base-yr)
  FPRDAX(f,a,t,sim)    productivity term for factor f in act a (level)
  FPRDAXP(f,a,t,sim)    productivity term for factor f in act a (% deviation from base in t)
  FPRDAXPP(f,a,sim)    productivity term for factor f in act a (avg annual % growth tmin – tmax)
  FPRDAXPPREP(f,a,sim)    productivity term for factor f in act a (avg annual % growth tminrep – tmaxrep)
  FPRDAXPY(f,a,t,sim)    productivity term for factor f in act a (annual % growth from t-1)

  GBOR0(t)    domestic government borrowing (ref0)
  GBOR00    domestic government borrowing (base-yr)
  GBORX(t,sim)    domestic government borrowing (level)
  GBORXP(t,sim)    domestic government borrowing (% deviation from base in t)
  GBORXPP(sim)    domestic government borrowing (avg annual % growth tmin – tmax)
  GBORXPPREP(sim)    domestic government borrowing (avg annual % growth tminrep – tmaxrep)
  GBORXPY(t,sim)    domestic government borrowing (annual % growth from t-1)

  GDEBT0(t)    domestic government debt stock (ref0)
  GDEBT00    domestic government debt stock (base-yr)
  GDEBTX(t,sim)    domestic government debt stock (level)
  GDEBTXP(t,sim)    domestic government debt stock (% deviation from base in t)
  GDEBTXPP(sim)    domestic government debt stock (avg annual % growth tmin – tmax)
  GDEBTXPPREP(sim)    domestic government debt stock (avg annual % growth tminrep – tmaxrep)
  GDEBTXPY(t,sim)    domestic government debt stock (annual % growth from t-1)

  GDPMP0(t)    nominal GDP at market prices (ref0)
  GDPMP00    nominal GDP at market prices (base-yr)
  GDPMPX(t,sim)    nominal GDP at market prices (level)
  GDPMPXP(t,sim)    nominal GDP at market prices (% deviation from base in t)
  GDPMPXPP(sim)    nominal GDP at market prices (avg annual % growth tmin – tmax)
  GDPMPXPPREP(sim)    nominal GDP at market prices (avg annual % growth tminrep – tmaxrep)
  GDPMPXPY(t,sim)    nominal GDP at market prices (annual % growth from t-1)

  GOVRECABS0(acgovrec,t)    absorption shr for government receipt of ac (ref0)
  GOVRECABS00(acgovrec)    absorption shr for government receipt of ac (base-yr)
  GOVRECABSX(acgovrec,t,sim)    absorption shr for government receipt of ac (level)
  GOVRECABSXP(acgovrec,t,sim)    absorption shr for government receipt of ac (% deviation from base in t)
  GOVRECABSXPP(acgovrec,sim)    absorption shr for government receipt of ac (avg annual % growth tmin – tmax)
  GOVRECABSXPPREP(acgovrec,sim)    absorption shr for government receipt of ac (avg annual % growth tminrep – tmaxrep)
  GOVRECABSXPY(acgovrec,t,sim)    absorption shr for government receipt of ac (annual % growth from t-1)

  GOVRECGDP0(acgovrec,t)    GDP shr for government receipt acgovrec (ref0)
  GOVRECGDP00(acgovrec)    GDP shr for government receipt acgovrec (base-yr)
  GOVRECGDPX(acgovrec,t,sim)    GDP shr for government receipt acgovrec (level)
  GOVRECGDPXP(acgovrec,t,sim)    GDP shr for government receipt acgovrec (% deviation from base in t)
  GOVRECGDPXPP(acgovrec,sim)    GDP shr for government receipt acgovrec (avg annual % growth tmin – tmax)
  GOVRECGDPXPPREP(acgovrec,sim)    GDP shr for government receipt acgovrec (avg annual % growth tminrep – tmaxrep)
  GOVRECGDPXPY(acgovrec,t,sim)    GDP shr for government receipt acgovrec (annual % growth from t-1)

  GOVSPNDABS0(acgovspnd,t)    absorption shr for government receipt of ac (ref0)
  GOVSPNDABS00(acgovspnd)    absorption shr for government receipt of ac (base-yr)
  GOVSPNDABSX(acgovspnd,t,sim)    absorption shr for government receipt of ac (level)
  GOVSPNDABSXP(acgovspnd,t,sim)    absorption shr for government receipt of ac (% deviation from base in t)
  GOVSPNDABSXPP(acgovspnd,sim)    absorption shr for government receipt of ac (avg annual % growth tmin – tmax)
  GOVSPNDABSXPPREP(acgovspnd,sim)    absorption shr for government receipt of ac (avg annual % growth tminrep – tmaxrep)
  GOVSPNDABSXPY(acgovspnd,t,sim)    absorption shr for government receipt of ac (annual % growth from t-1)

  GOVSPNDGDP0(acgovspnd,t)    GDP shr for government spending acgovspnd (ref0)
  GOVSPNDGDP00(ac)    GDP shr for government spending acgovspnd (base-yr)
  GOVSPNDGDPX(acgovspnd,t,sim)    GDP shr for government spending acgovspnd (level)
  GOVSPNDGDPXP(acgovspnd,t,sim)    GDP shr for government spending acgovspnd (% deviation from base in t)
  GOVSPNDGDPXPP(acgovspnd,sim)    GDP shr for government spending acgovspnd (avg annual % growth tmin – tmax)
  GOVSPNDGDPXPPREP(acgovspnd,sim)    GDP shr for government spending acgovspnd (avg annual % growth tminrep – tmaxrep)
  GOVSPNDGDPXPY(acgovspnd,t,sim)    GDP shr for government spending acgovspnd (annual % growth from t-1)

  GPRIMDEF0(t)    government primary deficit (ref0)
  GPRIMDEF00    government primary deficit (base-yr)
  GPRIMDEFX(t,sim)    government primary deficit (level)
  GPRIMDEFXP(t,sim)    government primary deficit (% deviation from base in t)
  GPRIMDEFXPP(sim)    government primary deficit (avg annual % growth tmin – tmax)
  GPRIMDEFXPPREP(sim)    government primary deficit (avg annual % growth tminrep – tmaxrep)
  GPRIMDEFXPY(t,sim)    government primary deficit (annual % growth from t-1)

  IADJ0(ins2,t)    scaling factor for non-gov and gov investment (ref0)
  IADJ00(ins2)    scaling factor for non-gov and gov investment (base-yr)
  IADJX(ins2,t,sim)    scaling factor for non-gov and gov investment (level)
  IADJXP(ins2,t,sim)    scaling factor for non-gov and gov investment (% deviation from base in t)
  IADJXPP(ins2,sim)    scaling factor for non-gov and gov investment (avg annual % growth tmin – tmax)
  IADJXPPREP(ins2,sim)    scaling factor for non-gov and gov investment (avg annual % growth tminrep – tmaxrep)
  IADJXPY(ins2,t,sim)    scaling factor for non-gov and gov investment (annual % growth from t-1)

  INVVAL0(t)    domestic non-government investment (ref0)
  INVVAL00     domestic non-government investment (base-yr)
  INVVALX(t,sim)    domestic non-government investment (level)
  INVVALXP(t,sim)    domestic non-government investment (% deviation from base in t)
  INVVALXPP(sim)    domestic non-government investment (avg annual % growth tmin – tmax)
  INVVALXPPREP(sim)    domestic non-government investment (avg annual % growth tminrep – tmaxrep)
  INVVALXPY(t,sim)    domestic non-government investment (annual % growth from t-1)

  INVVALF0(t)    FDI value (in FCU) (ref0)
  INVVALF00    FDI value (in FCU) (base-yr)
  INVVALFX(t,sim)    FDI value (in FCU) (level)
  INVVALFXP(t,sim)    FDI value (in FCU) (% deviation from base in t)
  INVVALFXPP(sim)    FDI value (in FCU) (avg annual % growth tmin – tmax)
  INVVALFXPPREP(sim)    FDI value (in FCU) (avg annual % growth tminrep – tmaxrep)
  INVVALFXPY(t,sim)    FDI value (in FCU) (annual % growth from t-1)

  INVVALG0(t)    government investment (ref0)
  INVVALG00    government investment (base-yr)
  INVVALGX(t,sim)    government investment (level)
  INVVALGXP(t,sim)    government investment (% deviation from base in t)
  INVVALGXPP(sim)    government investment (avg annual % growth tmin – tmax)
  INVVALGXPPREP(sim)    government investment (avg annual % growth tminrep – tmaxrep)
  INVVALGXPY(t,sim)    government investment (annual % growth from t-1)

  ISCAL0(ac,t)    investment scaling factor (for fixed capital formation) (ref0)
  ISCAL00(ac)    investment scaling factor (for fixed capital formation) (base-yr)
  ISCALX(ac,t,sim)    investment scaling factor (for fixed capital formation) (level)
  ISCALXP(ac,t,sim)    investment scaling factor (for fixed capital formation) (% deviation from base in t)
  ISCALXPP(ac,sim)    investment scaling factor (for fixed capital formation) (avg annual % growth tmin – tmax)
  ISCALXPPREP(ac,sim)    investment scaling factor (for fixed capital formation) (avg annual % growth tminrep – tmaxrep)
  ISCALXPY(ac,t,sim)    investment scaling factor (for fixed capital formation) (annual % growth from t-1)

  LABPARTRAT0(t)    ratio between labor force and population at labor force age (ref0)
  LABPARTRAT00    ratio between labor force and population at labor force age (base-yr)
  LABPARTRATX(t,sim)    ratio between labor force and population at labor force age (level)
  LABPARTRATXP(t,sim)    ratio between labor force and population at labor force age (% deviation from base in t)
  LABPARTRATXPP(sim)    ratio between labor force and population at labor force age (avg annual % growth tmin – tmax)
  LABPARTRATXPPREP(sim)    ratio between labor force and population at labor force age (avg annual % growth tminrep – tmaxrep)
  LABPARTRATXPY(t,sim)    ratio between labor force and population at labor force age (annual % growth from t-1)

  MPS0(ins,t)    marginal propensity to save for dom non-gov inst insdng (ref0)
  MPS00(ins)    marginal propensity to save for dom non-gov inst insdng (base-yr)
  MPSX(ins,t,sim)    marginal propensity to save for dom non-gov inst insdng (level)
  MPSXP(ins,t,sim)    marginal propensity to save for dom non-gov inst insdng (% deviation from base in t)
  MPSXPP(ins,sim)    marginal propensity to save for dom non-gov inst insdng (avg annual % growth tmin – tmax)
  MPSXPPREP(ins,sim)    marginal propensity to save for dom non-gov inst insdng (avg annual % growth tminrep – tmaxrep)
  MPSXPY(ins,t,sim)    marginal propensity to save for dom non-gov inst insdng (annual % growth from t-1)

  MPSADJ0(t)    change in marginal propensity to save for dom non-gov inst (ref0)
  MPSADJ00    change in marginal propensity to save for dom non-gov inst (base-yr)
  MPSADJX(t,sim)    change in marginal propensity to save for dom non-gov inst (level)
  MPSADJXP(t,sim)    change in marginal propensity to save for dom non-gov inst (% deviation from base in t)
  MPSADJXPP(sim)    change in marginal propensity to save for dom non-gov inst (avg annual % growth tmin – tmax)
  MPSADJXPPREP(sim)    change in marginal propensity to save for dom non-gov inst (avg annual % growth tminrep – tmaxrep)
  MPSADJXPY(t,sim)    change in marginal propensity to save for dom non-gov inst (annual % growth from t-1)

  MPSSCAL0(t)    savings rate scaling factor (ref0)
  MPSSCAL00    savings rate scaling factor (base-yr)
  MPSSCALX(t,sim)    savings rate scaling factor (level)
  MPSSCALXP(t,sim)    savings rate scaling factor (% deviation from base in t)
  MPSSCALXPP(sim)    savings rate scaling factor (avg annual % growth tmin – tmax)
  MPSSCALXPPREP(sim)    savings rate scaling factor (avg annual % growth tminrep – tmaxrep)
  MPSSCALXPY(t,sim)    savings rate scaling factor (annual % growth from t-1)

  NDFG0(t)    net domestic financing (difference between net domestic borrowing and interest payments) (LCU) (ref0)
  NDFG00    net domestic financing (difference between net domestic borrowing and interest payments) (LCU) (base-yr)
  NDFGX(t,sim)    net domestic financing (difference between net domestic borrowing and interest payments) (LCU) (level)
  NDFGXP(t,sim)    net domestic financing (difference between net domestic borrowing and interest payments) (LCU) (% deviation from base in t)
  NDFGXPP(sim)    net domestic financing (difference between net domestic borrowing and interest payments) (LCU) (avg annual % growth tmin – tmax)
  NDFGXPPREP(sim)    net domestic financing (difference between net domestic borrowing and interest payments) (LCU) (avg annual % growth tminrep – tmaxrep)
  NDFGXPY(t,sim)    net domestic financing (difference between net domestic borrowing and interest payments) (LCU) (annual % growth from t-1)

  NFFG0(t)    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU) (ref0)
  NFFG00    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU) (base-yr)
  NFFGX(t,sim)    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU) (level)
  NFFGXP(t,sim)    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU) (% deviation from base in t)
  NFFGXPP(sim)    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU) (avg annual % growth tmin – tmax)
  NFFGXPPREP(sim)    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU) (avg annual % growth tminrep – tmaxrep)
  NFFGXPY(t,sim)    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU) (annual % growth from t-1)

  NFFINS0(t)    net foreign financing dom non-gov inst (FCU) (ref0)
  NFFINS00    net foreign financing dom non-gov inst (FCU) (base-yr)
  NFFINSX(t,sim)    net foreign financing dom non-gov inst (FCU) (level)
  NFFINSXP(t,sim)    net foreign financing dom non-gov inst (FCU) (% deviation from base in t)
  NFFINSXPP(sim)    net foreign financing dom non-gov inst (FCU) (avg annual % growth tmin – tmax)
  NFFINSXPPREP(sim)    net foreign financing dom non-gov inst (FCU) (avg annual % growth tminrep – tmaxrep)
  NFFINSXPY(t,sim)    net foreign financing dom non-gov inst (FCU) (annual % growth from t-1)

  NFFINSSCAL0(t)    scaling factor for net foreign financing dom non-gov inst (FCU) (ref0)
  NFFINSSCAL00    scaling factor for net foreign financing dom non-gov inst (FCU) (base-yr)
  NFFINSSCALX(t,sim)    scaling factor for net foreign financing dom non-gov inst (FCU) (level)
  NFFINSSCALXP(t,sim)    scaling factor for net foreign financing dom non-gov inst (FCU) (% deviation from base in t)
  NFFINSSCALXPP(sim)    scaling factor for net foreign financing dom non-gov inst (FCU) (avg annual % growth tmin – tmax)
  NFFINSSCALXPPREP(sim)    scaling factor for net foreign financing dom non-gov inst (FCU) (avg annual % growth tminrep – tmaxrep)
  NFFINSSCALXPY(t,sim)    scaling factor for net foreign financing dom non-gov inst (FCU) (annual % growth from t-1)

  NGOVPAYABS0(acngovpay,t)    absorption shr for government receipt of ac (ref0)
  NGOVPAYABS00(acngovpay)    absorption shr for government receipt of ac (base-yr)
  NGOVPAYABSX(acngovpay,t,sim)    absorption shr for government receipt of ac (level)
  NGOVPAYABSXP(acngovpay,t,sim)    absorption shr for government receipt of ac (% deviation from base in t)
  NGOVPAYABSXPP(acngovpay,sim)    absorption shr for government receipt of ac (avg annual % growth tmin – tmax)
  NGOVPAYABSXPPREP(acngovpay,sim)    absorption shr for government receipt of ac (avg annual % growth tminrep – tmaxrep)
  NGOVPAYABSXPY(acngovpay,t,sim)    absorption shr for government receipt of ac (annual % growth from t-1)

  NGOVPAYGDP0(acngovpay,t)    GDP shr for non-government payment acngovpay (ref0)
  NGOVPAYGDP00(ac)    GDP shr for non-government payment acngovpay (base-yr)
  NGOVPAYGDPX(acngovpay,t,sim)    GDP shr for non-government payment acngovpay (level)
  NGOVPAYGDPXP(acngovpay,t,sim)    GDP shr for non-government payment acngovpay (% deviation from base in t)
  NGOVPAYGDPXPP(acngovpay,sim)    GDP shr for non-government payment acngovpay (avg annual % growth tmin – tmax)
  NGOVPAYGDPXPPREP(acngovpay,sim)    GDP shr for non-government payment acngovpay (avg annual % growth tminrep – tmaxrep)
  NGOVPAYGDPXPY(acngovpay,t,sim)    GDP shr for non-government payment acngovpay (annual % growth from t-1)

  PA0(a,t)    output price of activity a (ref0)
  PA00(a)    output price of activity a (base-yr)
  PAX(a,t,sim)    output price of activity a (level)
  PAXP(a,t,sim)    output price of activity a (% deviation from base in t)
  PAXPP(a,sim)    output price of activity a (avg annual % growth tmin – tmax)
  PAXPPREP(a,sim)    output price of activity a (avg annual % growth tminrep – tmaxrep)
  PAXPY(a,t,sim)    output price of activity a (annual % growth from t-1)

  PDD0(c,t)    demand price for comm c produced and sold domestically (ref0)
  PDD00(c)    demand price for comm c produced and sold domestically (base-yr)
  PDDX(c,t,sim)    demand price for comm c produced and sold domestically (level)
  PDDXP(c,t,sim)    demand price for comm c produced and sold domestically (% deviation from base in t)
  PDDXPP(c,sim)    demand price for comm c produced and sold domestically (avg annual % growth tmin – tmax)
  PDDXPPREP(c,sim)    demand price for comm c produced and sold domestically (avg annual % growth tminrep – tmaxrep)
  PDDXPY(c,t,sim)    demand price for comm c produced and sold domestically (annual % growth from t-1)

  PDS0(c,t)    supply price for comm c produced and sold domestically (ref0)
  PDS00(c)    supply price for comm c produced and sold domestically (base-yr)
  PDSX(c,t,sim)    supply price for comm c produced and sold domestically (level)
  PDSXP(c,t,sim)    supply price for comm c produced and sold domestically (% deviation from base in t)
  PDSXPP(c,sim)    supply price for comm c produced and sold domestically (avg annual % growth tmin – tmax)
  PDSXPPREP(c,sim)    supply price for comm c produced and sold domestically (avg annual % growth tminrep – tmaxrep)
  PDSXPY(c,t,sim)    supply price for comm c produced and sold domestically (annual % growth from t-1)

  PE0(c,t)    export price for c (domestic currency) (ref0)
  PE00(c)    export price for c (domestic currency) (base-yr)
  PEX(c,t,sim)    export price for c (domestic currency) (level)
  PEXP(c,t,sim)    export price for c (domestic currency) (% deviation from base in t)
  PEXPP(c,sim)    export price for c (domestic currency) (avg annual % growth tmin – tmax)
  PEXPPREP(c,sim)    export price for c (domestic currency) (avg annual % growth tminrep – tmaxrep)
  PEXPY(c,t,sim)    export price for c (domestic currency) (annual % growth from t-1)

  PK0(fcap,t)    replacement cost of capital inv (ref0)
  PK00(fcap)    replacement cost of capital inv (base-yr)
  PKX(fcap,t,sim)    replacement cost of capital inv (level)
  PKXP(fcap,t,sim)    replacement cost of capital inv (% deviation from base in t)
  PKXPP(fcap,sim)    replacement cost of capital inv (avg annual % growth tmin – tmax)
  PKXPPREP(fcap,sim)    replacement cost of capital inv (avg annual % growth tminrep – tmaxrep)
  PKXPY(fcap,t,sim)    replacement cost of capital inv (annual % growth from t-1)

  PM0(c,t)    import price for c (domestic currency) (ref0)
  PM00(c)    import price for c (domestic currency) (base-yr)
  PMX(c,t,sim)    import price for c (domestic currency) (level)
  PMXP(c,t,sim)    import price for c (domestic currency) (% deviation from base in t)
  PMXPP(c,sim)    import price for c (domestic currency) (avg annual % growth tmin – tmax)
  PMXPPREP(c,sim)    import price for c (domestic currency) (avg annual % growth tminrep – tmaxrep)
  PMXPY(c,t,sim)    import price for c (domestic currency) (annual % growth from t-1)

  PQD0(c,ac,t)    composite commodity demand price for c demanded by ac (ref0)
  PQD00(c,ac)    composite commodity demand price for c demanded by ac (base-yr)
  PQDX(c,ac,t,sim)    composite commodity demand price for c demanded by ac (level)
  PQDXP(c,ac,t,sim)    composite commodity demand price for c demanded by ac (% deviation from base in t)
  PQDXPP(c,ac,sim)    composite commodity demand price for c demanded by ac (avg annual % growth tmin – tmax)
  PQDXPPREP(c,ac,sim)    composite commodity demand price for c demanded by ac (avg annual % growth tminrep – tmaxrep)
  PQDXPY(c,ac,t,sim)    composite commodity demand price for c demanded by ac (annual % growth from t-1)

  PQS0(c,t)    composite commodity supply price for c (ref0)
  PQS00(c)    composite commodity supply price for c (base-yr)
  PQSX(c,t,sim)    composite commodity supply price for c (level)
  PQSXP(c,t,sim)    composite commodity supply price for c (% deviation from base in t)
  PQSXPP(c,sim)    composite commodity supply price for c (avg annual % growth tmin – tmax)
  PQSXPPREP(c,sim)    composite commodity supply price for c (avg annual % growth tminrep – tmaxrep)
  PQSXPY(c,t,sim)    composite commodity supply price for c (annual % growth from t-1)

  PVA0(a,t)    value-added price for activity a (ref0)
  PVA00(a)    value-added price for activity a (base-yr)
  PVAX(a,t,sim)    value-added price for activity a (level)
  PVAXP(a,t,sim)    value-added price for activity a (% deviation from base in t)
  PVAXPP(a,sim)    value-added price for activity a (avg annual % growth tmin – tmax)
  PVAXPPREP(a,sim)    value-added price for activity a (avg annual % growth tminrep – tmaxrep)
  PVAXPY(a,t,sim)    value-added price for activity a (annual % growth from t-1)

  PX0(c,t)    producer price for commodity c (ref0)
  PX00(c)    producer price for commodity c (base-yr)
  PXX(c,t,sim)    producer price for commodity c (level)
  PXXP(c,t,sim)    producer price for commodity c (% deviation from base in t)
  PXXPP(c,sim)    producer price for commodity c (avg annual % growth tmin – tmax)
  PXXPPREP(c,sim)    producer price for commodity c (avg annual % growth tminrep – tmaxrep)
  PXXPY(c,t,sim)    producer price for commodity c (annual % growth from t-1)

  QA0(a,t)    level of activity a (ref0)
  QA00(a)    level of activity a (base-yr)
  QAX(a,t,sim)    level of activity a (level)
  QAXP(a,t,sim)    level of activity a (% deviation from base in t)
  QAXPP(a,sim)    level of activity a (avg annual % growth tmin – tmax)
  QAXPPREP(a,sim)    level of activity a (avg annual % growth tminrep – tmaxrep)
  QAXPY(a,t,sim)    level of activity a (annual % growth from t-1)

  QD0(c,t)    quantity sold domestically of domestic output c (ref0)
  QD00(c)    quantity sold domestically of domestic output c (base-yr)
  QDX(c,t,sim)    quantity sold domestically of domestic output c (level)
  QDXP(c,t,sim)    quantity sold domestically of domestic output c (% deviation from base in t)
  QDXPP(c,sim)    quantity sold domestically of domestic output c (avg annual % growth tmin – tmax)
  QDXPPREP(c,sim)    quantity sold domestically of domestic output c (avg annual % growth tminrep – tmaxrep)
  QDXPY(c,t,sim)    quantity sold domestically of domestic output c (annual % growth from t-1)

  QE0(c,t)    quantity of exports for commodity c (ref0)
  QE00(c)    quantity of exports for commodity c (base-yr)
  QEX(c,t,sim)    quantity of exports for commodity c (level)
  QEXP(c,t,sim)    quantity of exports for commodity c (% deviation from base in t)
  QEXPP(c,sim)    quantity of exports for commodity c (avg annual % growth tmin – tmax)
  QEXPPREP(c,sim)    quantity of exports for commodity c (avg annual % growth tminrep – tmaxrep)
  QEXPY(c,t,sim)    quantity of exports for commodity c (annual % growth from t-1)

  QF0(f,a,t)    quantity demanded of factor f from activity a (ref0)
  QF00(f,a)    quantity demanded of factor f from activity a (base-yr)
  QFX(ac,ac,t,sim)    quantity demanded of factor f from activity a (level)
  QFXP(ac,ac,t,sim)    quantity demanded of factor f from activity a (% deviation from base in t)
  QFXPP(ac,ac,sim)    quantity demanded of factor f from activity a (avg annual % growth tmin – tmax)
  QFXPPREP(ac,ac,sim)    quantity demanded of factor f from activity a (avg annual % growth tminrep – tmaxrep)
  QFXPY(ac,ac,t,sim)    quantity demanded of factor f from activity a (annual % growth from t-1)

  QFHEND0(h,f,t)    end of period real endowment of factor ac for household h (ref0)
  QFHEND00(h,f)    end of period real endowment of factor ac for household h (base-yr)
  QFHENDX(h,f,t,sim)    end of period real endowment of factor ac for household h (level)
  QFHENDXP(h,f,t,sim)    end of period real endowment of factor ac for household h (% deviation from base in t)
  QFHENDXPP(h,f,sim)    end of period real endowment of factor ac for household h (avg annual % growth tmin – tmax)
  QFHENDXPPREP(h,f,sim)    end of period real endowment of factor ac for household h (avg annual % growth tminrep – tmaxrep)
  QFHENDXPY(h,f,t,sim)    end of period real endowment of factor ac for household h (annual % growth from t-1)

  QFHENDSCAL0(f,t)    scaling factor for end of period real endowment of factor ac for household h (ref0)
  QFHENDSCAL00(f)    scaling factor for end of period real endowment of factor ac for household h (base-yr)
  QFHENDSCALX(f,t,sim)    scaling factor for end of period real endowment of factor ac for household h (level)
  QFHENDSCALXP(f,t,sim)    scaling factor for end of period real endowment of factor ac for household h (% deviation from base in t)
  QFHENDSCALXPP(f,sim)    scaling factor for end of period real endowment of factor ac for household h (avg annual % growth tmin – tmax)
  QFHENDSCALXPPREP(f,sim)    scaling factor for end of period real endowment of factor ac for household h (avg annual % growth tminrep – tmaxrep)
  QFHENDSCALXPY(f,t,sim)    scaling factor for end of period real endowment of factor ac for household h (annual % growth from t-1)

  QFINS0(ins,ac,t)    real endowment of factor ac for institution ins (ref0)
  QFINS00(ins,ac)    real endowment of factor ac for institution ins (base-yr)
  QFINSX(ins,ac,t,sim)    real endowment of factor ac for institution ins (level)
  QFINSXP(ins,ac,t,sim)    real endowment of factor ac for institution ins (% deviation from base in t)
  QFINSXPP(ins,ac,sim)    real endowment of factor ac for institution ins (avg annual % growth tmin – tmax)
  QFINSXPPREP(ins,ac,sim)    real endowment of factor ac for institution ins (avg annual % growth tminrep – tmaxrep)
  QFINSXPY(ins,ac,t,sim)    real endowment of factor ac for institution ins (annual % growth from t-1)

  QFINSSCAL0(f,t)    scaling factor for factor endowments (ref0)
  QFINSSCAL00(f)    scaling factor for factor endowments (base-yr)
  QFINSSCALX(f,t,sim)    scaling factor for factor endowments (level)
  QFINSSCALXP(f,t,sim)    scaling factor for factor endowments (% deviation from base in t)
  QFINSSCALXPP(f,sim)    scaling factor for factor endowments (avg annual % growth tmin – tmax)
  QFINSSCALXPPREP(f,sim)    scaling factor for factor endowments (avg annual % growth tminrep – tmaxrep)
  QFINSSCALXPY(f,t,sim)    scaling factor for factor endowments (annual % growth from t-1)

  QFS0(f,t)    supply of factor f (ref0)
  QFS00(f)    supply of factor f (base-yr)
  QFSX(f,t,sim)    supply of factor f (level)
  QFSXP(f,t,sim)    supply of factor f (% deviation from base in t)
  QFSXPP(f,sim)    supply of factor f (avg annual % growth tmin – tmax)
  QFSXPPREP(f,sim)    supply of factor f (avg annual % growth tminrep – tmaxrep)
  QFSXPY(f,t,sim)    supply of factor f (annual % growth from t-1)

  QG0(c,t)    quantity of government demand for commodity c (ref0)
  QG00(c)    quantity of government demand for commodity c (base-yr)
  QGX(c,t,sim)    quantity of government demand for commodity c (level)
  QGXP(c,t,sim)    quantity of government demand for commodity c (% deviation from base in t)
  QGXPP(c,sim)    quantity of government demand for commodity c (avg annual % growth tmin – tmax)
  QGXPPREP(c,sim)    quantity of government demand for commodity c (avg annual % growth tminrep – tmaxrep)
  QGXPY(c,t,sim)    quantity of government demand for commodity c (annual % growth from t-1)

  QGSCAL0(t)    government demand scaling factor (ref0)
  QGSCAL00    government demand scaling factor (base-yr)
  QGSCALX(t,sim)    government demand scaling factor (level)
  QGSCALXP(t,sim)    government demand scaling factor (% deviation from base in t)
  QGSCALXPP(sim)    government demand scaling factor (avg annual % growth tmin – tmax)
  QGSCALXPPREP(sim)    government demand scaling factor (avg annual % growth tminrep – tmaxrep)
  QGSCALXPY(t,sim)    government demand scaling factor (annual % growth from t-1)

  QH0(ac,h,t)    quantity consumed of commodity c by household h (ref0)
  QH00(ac,h)    quantity consumed of commodity c by household h (base-yr)
  QHX(ac,h,t,sim)    quantity consumed of commodity c by household h (level)
  QHXP(ac,h,t,sim)    quantity consumed of commodity c by household h (% deviation from base in t)
  QHXPP(ac,h,sim)    quantity consumed of commodity c by household h (avg annual % growth tmin – tmax)
  QHXPPREP(ac,h,sim)    quantity consumed of commodity c by household h (avg annual % growth tminrep – tmaxrep)
  QHXPY(ac,h,t,sim)    quantity consumed of commodity c by household h (annual % growth from t-1)

  QINT0(c,a,t)    quantity of commodity c as intermediate input to activity a (ref0)
  QINT00(c,a)    quantity of commodity c as intermediate input to activity a (base-yr)
  QINTX(c,a,t,sim)    quantity of commodity c as intermediate input to activity a (level)
  QINTXP(c,a,t,sim)    quantity of commodity c as intermediate input to activity a (% deviation from base in t)
  QINTXPP(c,a,sim)    quantity of commodity c as intermediate input to activity a (avg annual % growth tmin – tmax)
  QINTXPPREP(c,a,sim)    quantity of commodity c as intermediate input to activity a (avg annual % growth tminrep – tmaxrep)
  QINTXPY(c,a,t,sim)    quantity of commodity c as intermediate input to activity a (annual % growth from t-1)

  QINV0(c,t)    quantity of investment demand for commodity c (ref0)
  QINV00(c)    quantity of investment demand for commodity c (base-yr)
  QINVX(c,t,sim)    quantity of investment demand for commodity c (level)
  QINVXP(c,t,sim)    quantity of investment demand for commodity c (% deviation from base in t)
  QINVXPP(c,sim)    quantity of investment demand for commodity c (avg annual % growth tmin – tmax)
  QINVXPPREP(c,sim)    quantity of investment demand for commodity c (avg annual % growth tminrep – tmaxrep)
  QINVXPY(c,t,sim)    quantity of investment demand for commodity c (annual % growth from t-1)

  QLABSCAL0(t)    scaling factor for labor force part'on rate targeting (ref0)
  QLABSCAL00    scaling factor for labor force part'on rate targeting (base-yr)
  QLABSCALX(t,sim)    scaling factor for labor force part'on rate targeting (level)
  QLABSCALXP(t,sim)    scaling factor for labor force part'on rate targeting (% deviation from base in t)
  QLABSCALXPP(sim)    scaling factor for labor force part'on rate targeting (avg annual % growth tmin – tmax)
  QLABSCALXPPREP(sim)    scaling factor for labor force part'on rate targeting (avg annual % growth tminrep – tmaxrep)
  QLABSCALXPY(t,sim)    scaling factor for labor force part'on rate targeting (annual % growth from t-1)

  QM0(c,t)    quantity of imports of commodity c (ref0)
  QM00(c)    quantity of imports of commodity c (base-yr)
  QMX(c,t,sim)    quantity of imports of commodity c (level)
  QMXP(c,t,sim)    quantity of imports of commodity c (% deviation from base in t)
  QMXPP(c,sim)    quantity of imports of commodity c (avg annual % growth tmin – tmax)
  QMXPPREP(c,sim)    quantity of imports of commodity c (avg annual % growth tminrep – tmaxrep)
  QMXPY(c,t,sim)    quantity of imports of commodity c (annual % growth from t-1)

  QNGO0(c,insngo,t)    quantity consumed of commodity c by NGO insngo (ref0)
  QNGO00(c,insngo)    quantity consumed of commodity c by NGO insngo (base-yr)
  QNGOX(c,insngo,t,sim)    quantity consumed of commodity c by NGO insngo (level)
  QNGOXP(c,insngo,t,sim)    quantity consumed of commodity c by NGO insngo (% deviation from base in t)
  QNGOXPP(c,insngo,sim)    quantity consumed of commodity c by NGO insngo (avg annual % growth tmin – tmax)
  QNGOXPPREP(c,insngo,sim)    quantity consumed of commodity c by NGO insngo (avg annual % growth tminrep – tmaxrep)
  QNGOXPY(c,insngo,t,sim)    quantity consumed of commodity c by NGO insngo (annual % growth from t-1)

  QNGOSCAL0(insngo,t)    scaling factor for NGO consumption (ref0)
  QNGOSCAL00(insngo)    scaling factor for NGO consumption (base-yr)
  QNGOSCALX(insngo,t,sim)    scaling factor for NGO consumption (level)
  QNGOSCALXP(insngo,t,sim)    scaling factor for NGO consumption (% deviation from base in t)
  QNGOSCALXPP(insngo,sim)    scaling factor for NGO consumption (avg annual % growth tmin – tmax)
  QNGOSCALXPPREP(insngo,sim)    scaling factor for NGO consumption (avg annual % growth tminrep – tmaxrep)
  QNGOSCALXPY(insngo,t,sim)    scaling factor for NGO consumption (annual % growth from t-1)

  QQ0(c,t)    quantity of goods supplied domestically (composite supply) (ref0)
  QQ00(c)    quantity of goods supplied domestically (composite supply) (base-yr)
  QQX(c,t,sim)    quantity of goods supplied domestically (composite supply) (level)
  QQXP(c,t,sim)    quantity of goods supplied domestically (composite supply) (% deviation from base in t)
  QQXPP(c,sim)    quantity of goods supplied domestically (composite supply) (avg annual % growth tmin – tmax)
  QQXPPREP(c,sim)    quantity of goods supplied domestically (composite supply) (avg annual % growth tminrep – tmaxrep)
  QQXPY(c,t,sim)    quantity of goods supplied domestically (composite supply) (annual % growth from t-1)

  QT0(c,t)    quantity of trade and transport demand for commodity c (ref0)
  QT00(c)    quantity of trade and transport demand for commodity c (base-yr)
  QTX(c,t,sim)    quantity of trade and transport demand for commodity c (level)
  QTXP(c,t,sim)    quantity of trade and transport demand for commodity c (% deviation from base in t)
  QTXPP(c,sim)    quantity of trade and transport demand for commodity c (avg annual % growth tmin – tmax)
  QTXPPREP(c,sim)    quantity of trade and transport demand for commodity c (avg annual % growth tminrep – tmaxrep)
  QTXPY(c,t,sim)    quantity of trade and transport demand for commodity c (annual % growth from t-1)

  QTRST0(c,instrst,t)    quantity consumed of commodity c by tourist instrst (ref0)
  QTRST00(c,instrst)    quantity consumed of commodity c by tourist instrst (base-yr)
  QTRSTX(c,instrst,t,sim)    quantity consumed of commodity c by tourist instrst (level)
  QTRSTXP(c,instrst,t,sim)    quantity consumed of commodity c by tourist instrst (% deviation from base in t)
  QTRSTXPP(c,instrst,sim)    quantity consumed of commodity c by tourist instrst (avg annual % growth tmin – tmax)
  QTRSTXPPREP(c,instrst,sim)    quantity consumed of commodity c by tourist instrst (avg annual % growth tminrep – tmaxrep)
  QTRSTXPY(c,instrst,t,sim)    quantity consumed of commodity c by tourist instrst (annual % growth from t-1)

  QTRSTSCAL0(t)    scaling factor for tourist consumption (ref0)
  QTRSTSCAL00    scaling factor for tourist consumption (base-yr)
  QTRSTSCALX(t,sim)    scaling factor for tourist consumption (level)
  QTRSTSCALXP(t,sim)    scaling factor for tourist consumption (% deviation from base in t)
  QTRSTSCALXPP(sim)    scaling factor for tourist consumption (avg annual % growth tmin – tmax)
  QTRSTSCALXPPREP(sim)    scaling factor for tourist consumption (avg annual % growth tminrep – tmaxrep)
  QTRSTSCALXPY(t,sim)    scaling factor for tourist consumption (annual % growth from t-1)

  QX0(c,t)    quantity of domestic output of commodity c (ref0)
  QX00(c)    quantity of domestic output of commodity c (base-yr)
  QXX(c,t,sim)    quantity of domestic output of commodity c (level)
  QXXP(c,t,sim)    quantity of domestic output of commodity c (% deviation from base in t)
  QXXPP(c,sim)    quantity of domestic output of commodity c (avg annual % growth tmin – tmax)
  QXXPPREP(c,sim)    quantity of domestic output of commodity c (avg annual % growth tminrep – tmaxrep)
  QXXPY(c,t,sim)    quantity of domestic output of commodity c (annual % growth from t-1)

  RBTVAT0(a,t)    value added tax rebate to activities (ref0)
  RBTVAT00(a)    value added tax rebate to activities (base-yr)
  RBTVATX(a,t,sim)    value added tax rebate to activities (level)
  RBTVATXP(a,t,sim)    value added tax rebate to activities (% deviation from base in t)
  RBTVATXPP(a,sim)    value added tax rebate to activities (avg annual % growth tmin – tmax)
  RBTVATXPPREP(a,sim)    value added tax rebate to activities (avg annual % growth tminrep – tmaxrep)
  RBTVATXPY(a,t,sim)    value added tax rebate to activities (annual % growth from t-1)

  REXR0(t)    real exchange rate (ref0)
  REXR00    real exchange rate (base-yr)
  REXRX(t,sim)    real exchange rate (level)
  REXRXP(t,sim)    real exchange rate (% deviation from base in t)
  REXRXPP(sim)    real exchange rate (avg annual % growth tmin – tmax)
  REXRXPPREP(sim)    real exchange rate (avg annual % growth tminrep – tmaxrep)
  REXRXPY(t,sim)    real exchange rate (annual % growth from t-1)

  RGDPFC0(t)    real GDP at factor cost (at constant base-year prices) (ref0)
  RGDPFC00    real GDP at factor cost (at constant base-year prices) (base-yr)
  RGDPFCX(t,sim)    real GDP at factor cost (at constant base-year prices) (level)
  RGDPFCXP(t,sim)    real GDP at factor cost (at constant base-year prices) (% deviation from base in t)
  RGDPFCXPP(sim)    real GDP at factor cost (at constant base-year prices) (avg annual % growth tmin – tmax)
  RGDPFCXPPREP(sim)    real GDP at factor cost (at constant base-year prices) (avg annual % growth tminrep – tmaxrep)
  RGDPFCXPY(t,sim)    real GDP at factor cost (at constant base-year prices) (annual % growth from t-1)

  RGDPMP0(t)    real GDP at market prices (at constant base-year prices) (ref0)
  RGDPMP00    real GDP at market prices (at constant base-year prices) (base-yr)
  RGDPMPX(t,sim)    real GDP at market prices (at constant base-year prices) (level)
  RGDPMPXP(t,sim)    real GDP at market prices (at constant base-year prices) (% deviation from base in t)
  RGDPMPXPP(sim)    real GDP at market prices (at constant base-year prices) (avg annual % growth tmin – tmax)
  RGDPMPXPPREP(sim)    real GDP at market prices (at constant base-year prices) (avg annual % growth tminrep – tmaxrep)
  RGDPMPXPY(t,sim)    real GDP at market prices (at constant base-year prices) (annual % growth from t-1)

  RGPRIMDEF0(t)    real government deficit (ref0)
  RGPRIMDEF00    real government deficit (base-yr)
  RGPRIMDEFX(t,sim)    real government deficit (level)
  RGPRIMDEFXP(t,sim)    real government deficit (% deviation from base in t)
  RGPRIMDEFXPP(sim)    real government deficit (avg annual % growth tmin – tmax)
  RGPRIMDEFXPPREP(sim)    real government deficit (avg annual % growth tminrep – tmaxrep)
  RGPRIMDEFXPY(t,sim)    real government deficit (annual % growth from t-1)

  RNDFG0(t)    real gov net domestic financing (ref0)
  RNDFG00    real gov net domestic financing (base-yr)
  RNDFGX(t,sim)    real gov net domestic financing (level)
  RNDFGXP(t,sim)    real gov net domestic financing (% deviation from base in t)
  RNDFGXPP(sim)    real gov net domestic financing (avg annual % growth tmin – tmax)
  RNDFGXPPREP(sim)    real gov net domestic financing (avg annual % growth tminrep – tmaxrep)
  RNDFGXPY(t,sim)    real gov net domestic financing (annual % growth from t-1)

  SAV0(ac,t)        savings of (domestic non-government) institution insdng (ref0)
  SAV00(ac)         savings of (domestic non-government) institution insdng (base-yr)
  SAVX(ac,t,sim)    savings of (domestic non-government) institution insdng (level)
  SAVXP(ac,t,sim)   savings of (domestic non-government) institution insdng (% deviation from base in t)
  SAVXPP(ac,sim)    savings of (domestic non-government) institution insdng (avg annual % growth tmin – tmax)
  SAVXPPREP(ac,sim) savings of (domestic non-government) institution insdng (avg annual % growth tminrep – tmaxrep)
  SAVXPY(ac,t,sim)  savings of (domestic non-government) institution insdng (annual % growth from t-1)

  SAVF0(t)    foreign savings (foreign currency) (ref0)
  SAVF00    foreign savings (foreign currency) (base-yr)
  SAVFX(t,sim)    foreign savings (foreign currency) (level)
  SAVFXP(t,sim)    foreign savings (foreign currency) (% deviation from base in t)
  SAVFXPP(sim)    foreign savings (foreign currency) (avg annual % growth tmin – tmax)
  SAVFXPPREP(sim)    foreign savings (foreign currency) (avg annual % growth tminrep – tmaxrep)
  SAVFXPY(t,sim)    foreign savings (foreign currency) (annual % growth from t-1)

  SHIF0(ins,f,t)    share for inst ins in the income of factor f (ref0)
  SHIF00(ins,f)    share for inst ins in the income of factor f (base-yr)
  SHIFX(ins,f,t,sim)    share for inst ins in the income of factor f (level)
  SHIFXP(ins,f,t,sim)    share for inst ins in the income of factor f (% deviation from base in t)
  SHIFXPP(ins,f,sim)    share for inst ins in the income of factor f (avg annual % growth tmin – tmax)
  SHIFXPPREP(ins,f,sim)    share for inst ins in the income of factor f (avg annual % growth tminrep – tmaxrep)
  SHIFXPY(ins,f,t,sim)    share for inst ins in the income of factor f (annual % growth from t-1)

  SUBC0(c,ac,t)    subsidy rate for demander ac on comm c (ref0)
  SUBC00(c,ac)    subsidy rate for demander ac on comm c (base-yr)
  SUBCX(c,ac,t,sim)    subsidy rate for demander ac on comm c (level)
  SUBCXP(c,ac,t,sim)    subsidy rate for demander ac on comm c (% deviation from base in t)
  SUBCXPP(c,ac,sim)    subsidy rate for demander ac on comm c (avg annual % growth tmin – tmax)
  SUBCXPPREP(c,ac,sim)    subsidy rate for demander ac on comm c (avg annual % growth tminrep – tmaxrep)
  SUBCXPY(c,ac,t,sim)    subsidy rate for demander ac on comm c (annual % growth from t-1)

  SUBCSCAL0(t)    subsidy rate scaling factor (ref0)
  SUBCSCAL00    subsidy rate scaling factor (base-yr)
  SUBCSCALX(t,sim)    subsidy rate scaling factor (level)
  SUBCSCALXP(t,sim)    subsidy rate scaling factor (% deviation from base in t)
  SUBCSCALXPP(sim)    subsidy rate scaling factor (avg annual % growth tmin – tmax)
  SUBCSCALXPPREP(sim)    subsidy rate scaling factor (avg annual % growth tminrep – tmaxrep)
  SUBCSCALXPY(t,sim)    subsidy rate scaling factor (annual % growth from t-1)

  SUBCT0(t)    total commodity subsidy spnd (ref0)
  SUBCT00    total commodity subsidy spnd (base-yr)
  SUBCTX(t,sim)    total commodity subsidy spnd (level)
  SUBCTXP(t,sim)    total commodity subsidy spnd (% deviation from base in t)
  SUBCTXPP(sim)    total commodity subsidy spnd (avg annual % growth tmin – tmax)
  SUBCTXPPREP(sim)    total commodity subsidy spnd (avg annual % growth tminrep – tmaxrep)
  SUBCTXPY(t,sim)    total commodity subsidy spnd (annual % growth from t-1)

  TA0(a,t)    rate of tax on producer gross output value (ref0)
  TA00(a)    rate of tax on producer gross output value (base-yr)
  TAX(a,t,sim)    rate of tax on producer gross output value (level)
  TAXP(a,t,sim)    rate of tax on producer gross output value (% deviation from base in t)
  TAXPP(a,sim)    rate of tax on producer gross output value (avg annual % growth tmin – tmax)
  TAXPPREP(a,sim)    rate of tax on producer gross output value (avg annual % growth tminrep – tmaxrep)
  TAXPY(a,t,sim)    rate of tax on producer gross output value (annual % growth from t-1)

  TASCAL0(t)    scaling factor for rate of tax on producer gross output value (ref0)
  TASCAL00    scaling factor for rate of tax on producer gross output value (base-yr)
  TASCALX(t,sim)    scaling factor for rate of tax on producer gross output value (level)
  TASCALXP(t,sim)    scaling factor for rate of tax on producer gross output value (% deviation from base in t)
  TASCALXPP(sim)    scaling factor for rate of tax on producer gross output value (avg annual % growth tmin – tmax)
  TASCALXPPREP(sim)    scaling factor for rate of tax on producer gross output value (avg annual % growth tminrep – tmaxrep)
  TASCALXPY(t,sim)    scaling factor for rate of tax on producer gross output value (annual % growth from t-1)

  TE0(c,t)    export tax rate for commodity c (ref0)
  TE00(c)    export tax rate for commodity c (base-yr)
  TEX(c,t,sim)    export tax rate for commodity c (level)
  TEXP(c,t,sim)    export tax rate for commodity c (% deviation from base in t)
  TEXPP(c,sim)    export tax rate for commodity c (avg annual % growth tmin – tmax)
  TEXPPREP(c,sim)    export tax rate for commodity c (avg annual % growth tminrep – tmaxrep)
  TEXPY(c,t,sim)    export tax rate for commodity c (annual % growth from t-1)

  TESCAL0(t)    scaling factor for export tax rate for commodity c (ref0)
  TESCAL00    scaling factor for export tax rate for commodity c (base-yr)
  TESCALX(t,sim)    scaling factor for export tax rate for commodity c (level)
  TESCALXP(t,sim)    scaling factor for export tax rate for commodity c (% deviation from base in t)
  TESCALXPP(sim)    scaling factor for export tax rate for commodity c (avg annual % growth tmin – tmax)
  TESCALXPPREP(sim)    scaling factor for export tax rate for commodity c (avg annual % growth tminrep – tmaxrep)
  TESCALXPY(t,sim)    scaling factor for export tax rate for commodity c (annual % growth from t-1)

  TF0(f,t)    rate of direct tax on factors (soc sec tax) (ref0)
  TF00(f)    rate of direct tax on factors (soc sec tax) (base-yr)
  TFX(f,t,sim)    rate of direct tax on factors (soc sec tax) (level)
  TFXP(f,t,sim)    rate of direct tax on factors (soc sec tax) (% deviation from base in t)
  TFXPP(f,sim)    rate of direct tax on factors (soc sec tax) (avg annual % growth tmin – tmax)
  TFXPPREP(f,sim)    rate of direct tax on factors (soc sec tax) (avg annual % growth tminrep – tmaxrep)
  TFXPY(f,t,sim)    rate of direct tax on factors (soc sec tax) (annual % growth from t-1)

  TFA0(f,a,t)    rate of tax on factor f use by activity a (ref0)
  TFA00(f,a)    rate of tax on factor f use by activity a (base-yr)
  TFAX(f,a,t,sim)    rate of tax on factor f use by activity a (level)
  TFAXP(f,a,t,sim)    rate of tax on factor f use by activity a (% deviation from base in t)
  TFAXPP(f,a,sim)    rate of tax on factor f use by activity a (avg annual % growth tmin – tmax)
  TFAXPPREP(f,a,sim)    rate of tax on factor f use by activity a (avg annual % growth tminrep – tmaxrep)
  TFAXPY(f,a,t,sim)    rate of tax on factor f use by activity a (annual % growth from t-1)

  TFASCAL0(t)    scaling factor rate of tax on factor f use by activity a (ref0)
  TFASCAL00    scaling factor rate of tax on factor f use by activity a (base-yr)
  TFASCALX(t,sim)    scaling factor rate of tax on factor f use by activity a (level)
  TFASCALXP(t,sim)    scaling factor rate of tax on factor f use by activity a (% deviation from base in t)
  TFASCALXPP(sim)    scaling factor rate of tax on factor f use by activity a (avg annual % growth tmin – tmax)
  TFASCALXPPREP(sim)    scaling factor rate of tax on factor f use by activity a (avg annual % growth tminrep – tmaxrep)
  TFASCALXPY(t,sim)    scaling factor rate of tax on factor f use by activity a (annual % growth from t-1)

  TFP0(a,t)    sectoral TFP index (ref0)
  TFP00(a)    sectoral TFP index (base-yr)
  TFPX(a,t,sim)    sectoral TFP index (level)
  TFPXP(a,t,sim)    sectoral TFP index (% deviation from base in t)
  TFPXPP(a,sim)    sectoral TFP index (avg annual % growth tmin – tmax)
  TFPXPPREP(a,sim)    sectoral TFP index (avg annual % growth tminrep – tmaxrep)
  TFPXPY(a,t,sim)    sectoral TFP index (annual % growth from t-1)

  TFPSCAL0(t)    tfp in calibration run (ref0)
  TFPSCAL00    tfp in calibration run (base-yr)
  TFPSCALX(t,sim)    tfp in calibration run (level)
  TFPSCALXP(t,sim)    tfp in calibration run (% deviation from base in t)
  TFPSCALXPP(sim)    tfp in calibration run (avg annual % growth tmin – tmax)
  TFPSCALXPPREP(sim)    tfp in calibration run (avg annual % growth tminrep – tmaxrep)
  TFPSCALXPY(t,sim)    tfp in calibration run (annual % growth from t-1)

  TFSCAL0(t)    scaling factor for rate of direct tax on factors (soc sec tax) (ref0)
  TFSCAL00    scaling factor for rate of direct tax on factors (soc sec tax) (base-yr)
  TFSCALX(t,sim)    scaling factor for rate of direct tax on factors (soc sec tax) (level)
  TFSCALXP(t,sim)    scaling factor for rate of direct tax on factors (soc sec tax) (% deviation from base in t)
  TFSCALXPP(sim)    scaling factor for rate of direct tax on factors (soc sec tax) (avg annual % growth tmin – tmax)
  TFSCALXPPREP(sim)    scaling factor for rate of direct tax on factors (soc sec tax) (avg annual % growth tminrep – tmaxrep)
  TFSCALXPY(t,sim)    scaling factor for rate of direct tax on factors (soc sec tax) (annual % growth from t-1)

  TM0(c,t)    import tariff rate for commodity c (ref0)
  TM00(c)    import tariff rate for commodity c (base-yr)
  TMX(c,t,sim)    import tariff rate for commodity c (level)
  TMXP(c,t,sim)    import tariff rate for commodity c (% deviation from base in t)
  TMXPP(c,sim)    import tariff rate for commodity c (avg annual % growth tmin – tmax)
  TMXPPREP(c,sim)    import tariff rate for commodity c (avg annual % growth tminrep – tmaxrep)
  TMXPY(c,t,sim)    import tariff rate for commodity c (annual % growth from t-1)

  TMSCAL0(t)    scaling factor for import tariff rate for commodity c (ref0)
  TMSCAL00    scaling factor for import tariff rate for commodity c (base-yr)
  TMSCALX(t,sim)    scaling factor for import tariff rate for commodity c (level)
  TMSCALXP(t,sim)    scaling factor for import tariff rate for commodity c (% deviation from base in t)
  TMSCALXPP(sim)    scaling factor for import tariff rate for commodity c (avg annual % growth tmin – tmax)
  TMSCALXPPREP(sim)    scaling factor for import tariff rate for commodity c (avg annual % growth tminrep – tmaxrep)
  TMSCALXPY(t,sim)    scaling factor for import tariff rate for commodity c (annual % growth from t-1)

  TQ0(c,t)    rate of sales tax (ref0)
  TQ00(c)    rate of sales tax (base-yr)
  TQX(c,t,sim)    rate of sales tax (level)
  TQXP(c,t,sim)    rate of sales tax (% deviation from base in t)
  TQXPP(c,sim)    rate of sales tax (avg annual % growth tmin – tmax)
  TQXPPREP(c,sim)    rate of sales tax (avg annual % growth tminrep – tmaxrep)
  TQXPY(c,t,sim)    rate of sales tax (annual % growth from t-1)

  TQSCAL0(t)    scaling factor for rate of sales tax (ref0)
  TQSCAL00    scaling factor for rate of sales tax (base-yr)
  TQSCALX(t,sim)    scaling factor for rate of sales tax (level)
  TQSCALXP(t,sim)    scaling factor for rate of sales tax (% deviation from base in t)
  TQSCALXPP(sim)    scaling factor for rate of sales tax (avg annual % growth tmin – tmax)
  TQSCALXPPREP(sim)    scaling factor for rate of sales tax (avg annual % growth tminrep – tmaxrep)
  TQSCALXPY(t,sim)    scaling factor for rate of sales tax (annual % growth from t-1)

  TRDGDP0(t)    foreign trade (exports+imports) (GDP shr) (ref0)
  TRDGDP00    foreign trade (exports+imports) (GDP shr) (base-yr)
  TRDGDPX(t,sim)    foreign trade (exports+imports) (GDP shr) (level)
  TRDGDPXP(t,sim)    foreign trade (exports+imports) (GDP shr) (% deviation from base in t)
  TRDGDPXPP(sim)    foreign trade (exports+imports) (GDP shr) (avg annual % growth tmin – tmax)
  TRDGDPXPPREP(sim)    foreign trade (exports+imports) (GDP shr) (avg annual % growth tminrep – tmaxrep)
  TRDGDPXPY(t,sim)    foreign trade (exports+imports) (GDP shr) (annual % growth from t-1)

  TRII0(ins,insp,t)    transfers to inst ins from dom inst insdng (ref0)
  TRII00(ins,insp)    transfers to inst ins from dom inst insdng (base-yr)
  TRIIX(ins,insp,t,sim)    transfers to inst ins from dom inst insdng (level)
  TRIIXP(ins,insp,t,sim)    transfers to inst ins from dom inst insdng (% deviation from base in t)
  TRIIXPP(ins,insp,sim)    transfers to inst ins from dom inst insdng (avg annual % growth tmin – tmax)
  TRIIXPPREP(ins,insp,sim)    transfers to inst ins from dom inst insdng (avg annual % growth tminrep – tmaxrep)
  TRIIXPY(ins,insp,t,sim)    transfers to inst ins from dom inst insdng (annual % growth from t-1)

  TRNSFR0(ac,acp,t)    transfers from insp or factorp to ins or factor (ref0)
  TRNSFR00(ac,acp)    transfers from insp or factorp to ins or factor (base-yr)
  TRNSFRX(ac,acp,t,sim)    transfers from insp or factorp to ins or factor (level)
  TRNSFRXP(ac,acp,t,sim)    transfers from insp or factorp to ins or factor (% deviation from base in t)
  TRNSFRXPP(ac,acp,sim)    transfers from insp or factorp to ins or factor (avg annual % growth tmin – tmax)
  TRNSFRXPPREP(ac,acp,sim)    transfers from insp or factorp to ins or factor (avg annual % growth tminrep – tmaxrep)
  TRNSFRXPY(ac,acp,t,sim)    transfers from insp or factorp to ins or factor (annual % growth from t-1)

  TRNSFRSCAL0(ac,t)    scaling factor for transfers from insp to ins or factor (ref0)
  TRNSFRSCAL00(ac)    scaling factor for transfers from insp to ins or factor (base-yr)
  TRNSFRSCALX(ac,t,sim)    scaling factor for transfers from insp to ins or factor (level)
  TRNSFRSCALXP(ac,t,sim)    scaling factor for transfers from insp to ins or factor (% deviation from base in t)
  TRNSFRSCALXPP(ac,sim)    scaling factor for transfers from insp to ins or factor (avg annual % growth tmin – tmax)
  TRNSFRSCALXPPREP(ac,sim)    scaling factor for transfers from insp to ins or factor (avg annual % growth tminrep – tmaxrep)
  TRNSFRSCALXPY(ac,t,sim)    scaling factor for transfers from insp to ins or factor (annual % growth from t-1)

  TRSMREC0(t)    total tourism receipts (FCU) (ref0)
  TRSMREC00    total tourism receipts (FCU) (base-yr)
  TRSMRECX(t,sim)    total tourism receipts (FCU) (level)
  TRSMRECXP(t,sim)    total tourism receipts (FCU) (% deviation from base in t)
  TRSMRECXPP(sim)    total tourism receipts (FCU) (avg annual % growth tmin – tmax)
  TRSMRECXPPREP(sim)    total tourism receipts (FCU) (avg annual % growth tminrep – tmaxrep)
  TRSMRECXPY(t,sim)    total tourism receipts (FCU) (annual % growth from t-1)

  TVAC0(c,ac,t)    rate of value added tax commodity c demander ac (ref0)
  TVAC00(c,ac)    rate of value added tax commodity c demander ac (base-yr)
  TVACX(c,ac,t,sim)    rate of value added tax commodity c demander ac (level)
  TVACXP(c,ac,t,sim)    rate of value added tax commodity c demander ac (% deviation from base in t)
  TVACXPP(c,ac,sim)    rate of value added tax commodity c demander ac (avg annual % growth tmin – tmax)
  TVACXPPREP(c,ac,sim)    rate of value added tax commodity c demander ac (avg annual % growth tminrep – tmaxrep)
  TVACXPY(c,ac,t,sim)    rate of value added tax commodity c demander ac (annual % growth from t-1)

  TVACSCAL0(t)    TVAC scaling factor (ref0)
  TVACSCAL00    TVAC scaling factor (base-yr)
  TVACSCALX(t,sim)    TVAC scaling factor (level)
  TVACSCALXP(t,sim)    TVAC scaling factor (% deviation from base in t)
  TVACSCALXPP(sim)    TVAC scaling factor (avg annual % growth tmin – tmax)
  TVACSCALXPPREP(sim)    TVAC scaling factor (avg annual % growth tminrep – tmaxrep)
  TVACSCALXPY(t,sim)    TVAC scaling factor (annual % growth from t-1)

  TY0(ins,t)    rate of direct tax on dom inst ins (ref0)
  TY00(ins)    rate of direct tax on dom inst ins (base-yr)
  TYX(ins,t,sim)    rate of direct tax on dom inst ins (level)
  TYXP(ins,t,sim)    rate of direct tax on dom inst ins (% deviation from base in t)
  TYXPP(ins,sim)    rate of direct tax on dom inst ins (avg annual % growth tmin – tmax)
  TYXPPREP(ins,sim)    rate of direct tax on dom inst ins (avg annual % growth tminrep – tmaxrep)
  TYXPY(ins,t,sim)    rate of direct tax on dom inst ins (annual % growth from t-1)

  TYSCAL0(t)    scaling factor for rate of direct tax (ref0)
  TYSCAL00    scaling factor for rate of direct tax (base-yr)
  TYSCALX(t,sim)    scaling factor for rate of direct tax (level)
  TYSCALXP(t,sim)    scaling factor for rate of direct tax (% deviation from base in t)
  TYSCALXPP(sim)    scaling factor for rate of direct tax (avg annual % growth tmin – tmax)
  TYSCALXPPREP(sim)    scaling factor for rate of direct tax (avg annual % growth tminrep – tmaxrep)
  TYSCALXPY(t,sim)    scaling factor for rate of direct tax (annual % growth from t-1)

  UERAT0(f,t)    unemployment rate for factor f (ref0)
  UERAT00(f)    unemployment rate for factor f (base-yr)
  UERATX(ac,t,sim)    unemployment rate for factor f (level)
  UERATXP(ac,t,sim)    unemployment rate for factor f (% deviation from base in t)
  UERATXPP(ac,sim)    unemployment rate for factor f (avg annual % growth tmin – tmax)
  UERATXPPREP(ac,sim)    unemployment rate for factor f (avg annual % growth tminrep – tmaxrep)
  UERATXPY(ac,t,sim)    unemployment rate for factor f (annual % growth from t-1)

  WALRAS0(t)    dummy variable (zero at equilibrium) (ref0)
  WALRAS00    dummy variable (zero at equilibrium) (base-yr)
  WALRASX(t,sim)    dummy variable (zero at equilibrium) (level)
  WALRASXP(t,sim)    dummy variable (zero at equilibrium) (% deviation from base in t)
  WALRASXPP(sim)    dummy variable (zero at equilibrium) (avg annual % growth tmin – tmax)
  WALRASXPPREP(sim)    dummy variable (zero at equilibrium) (avg annual % growth tminrep – tmaxrep)
  WALRASXPY(t,sim)    dummy variable (zero at equilibrium) (annual % growth from t-1)

  WF0(f,t)    average price of factor f (ref0)
  WF00(f)    average price of factor f (base-yr)
  WFX(f,t,sim)    average price of factor f (level)
  WFXP(f,t,sim)    average price of factor f (% deviation from base in t)
  WFXPP(f,sim)    average price of factor f (avg annual % growth tmin – tmax)
  WFXPPREP(f,sim)    average price of factor f (avg annual % growth tminrep – tmaxrep)
  WFXPY(f,t,sim)    average price of factor f (annual % growth from t-1)

  WFA0(f,a,t)    wage for factor f in activity a (ref0)
  WFA00(f,a)    wage for factor f in activity a (base-yr)
  WFAX(f,a,t,sim)    wage for factor f in activity a (level)
  WFAXP(f,a,t,sim)    wage for factor f in activity a (% deviation from base in t)
  WFAXPP(f,a,sim)    wage for factor f in activity a (avg annual % growth tmin – tmax)
  WFAXPPREP(f,a,sim)    wage for factor f in activity a (avg annual % growth tminrep – tmaxrep)
  WFAXPY(f,a,t,sim)    wage for factor f in activity a (annual % growth from t-1)

  WFAVG0(f,t)    average remuneration of factor f (ref0)
  WFAVG00(f)    average remuneration of factor f (base-yr)
  WFAVGX(ac,t,sim)    average remuneration of factor f (level)
  WFAVGXP(ac,t,sim)    average remuneration of factor f (% deviation from base in t)
  WFAVGXPP(ac,sim)    average remuneration of factor f (avg annual % growth tmin – tmax)
  WFAVGXPPREP(ac,sim)    average remuneration of factor f (avg annual % growth tminrep – tmaxrep)
  WFAVGXPY(ac,t,sim)    average remuneration of factor f (annual % growth from t-1)

  WFDIST0(f,a,t)    wage distortion factor for factor f in activity a (ref0)
  WFDIST00(f,a)    wage distortion factor for factor f in activity a (base-yr)
  WFDISTX(f,a,t,sim)    wage distortion factor for factor f in activity a (level)
  WFDISTXP(f,a,t,sim)    wage distortion factor for factor f in activity a (% deviation from base in t)
  WFDISTXPP(f,a,sim)    wage distortion factor for factor f in activity a (avg annual % growth tmin – tmax)
  WFDISTXPPREP(f,a,sim)    wage distortion factor for factor f in activity a (avg annual % growth tminrep – tmaxrep)
  WFDISTXPY(f,a,t,sim)    wage distortion factor for factor f in activity a (annual % growth from t-1)

  YF0(f,t)    income of factor f (ref0)
  YF00(f)    income of factor f (base-yr)
  YFX(f,t,sim)    income of factor f (level)
  YFXP(f,t,sim)    income of factor f (% deviation from base in t)
  YFXPP(f,sim)    income of factor f (avg annual % growth tmin – tmax)
  YFXPPREP(f,sim)    income of factor f (avg annual % growth tminrep – tmaxrep)
  YFXPY(f,t,sim)    income of factor f (annual % growth from t-1)

  YG0(t)    government revenue (ref0)
  YG00    government revenue (base-yr)
  YGX(t,sim)    government revenue (level)
  YGXP(t,sim)    government revenue (% deviation from base in t)
  YGXPP(sim)    government revenue (avg annual % growth tmin – tmax)
  YGXPPREP(sim)    government revenue (avg annual % growth tminrep – tmaxrep)
  YGXPY(t,sim)    government revenue (annual % growth from t-1)

  YI0(ins,t)    income of (domestic non-government) institution insdng (ref0)
  YI00(ins)    income of (domestic non-government) institution insdng (base-yr)
  YIX(ins,t,sim)    income of (domestic non-government) institution insdng (level)
  YIXP(ins,t,sim)    income of (domestic non-government) institution insdng (% deviation from base in t)
  YIXPP(ins,sim)    income of (domestic non-government) institution insdng (avg annual % growth tmin – tmax)
  YIXPPREP(ins,sim)    income of (domestic non-government) institution insdng (avg annual % growth tminrep – tmaxrep)
  YIXPY(ins,t,sim)    income of (domestic non-government) institution insdng (annual % growth from t-1)

  YIF0(ins,f,t)    income of institution ins from factor f (ref0)
  YIF00(ins,f)    income of institution ins from factor f (base-yr)
  YIFX(ins,f,t,sim)    income of institution ins from factor f (level)
  YIFXP(ins,f,t,sim)    income of institution ins from factor f (% deviation from base in t)
  YIFXPP(ins,f,sim)    income of institution ins from factor f (avg annual % growth tmin – tmax)
  YIFXPPREP(ins,f,sim)    income of institution ins from factor f (avg annual % growth tminrep – tmaxrep)
  YIFXPY(ins,f,t,sim)    income of institution ins from factor f (annual % growth from t-1)

  YTAXIMP0(t)    total import tax collection (ref0)
  YTAXIMP00    total import tax collection (base-yr)
  YTAXIMPX(t,sim)    total import tax collection (level)
  YTAXIMPXP(t,sim)    total import tax collection (% deviation from base in t)
  YTAXIMPXPP(sim)    total import tax collection (avg annual % growth tmin – tmax)
  YTAXIMPXPPREP(sim)    total import tax collection (avg annual % growth tminrep – tmaxrep)
  YTAXIMPXPY(t,sim)    total import tax collection (annual % growth from t-1)

  YTAXEXP0(t)    total export tax collection (ref0)
  YTAXEXP00    total export tax collection (base-yr)
  YTAXEXPX(t,sim)    total export tax collection (level)
  YTAXEXPXP(t,sim)    total export tax collection (% deviation from base in t)
  YTAXEXPXPP(sim)    total export tax collection (avg annual % growth tmin – tmax)
  YTAXEXPXPPREP(sim)    total export tax collection (avg annual % growth tminrep – tmaxrep)
  YTAXEXPXPY(t,sim)    total export tax collection (annual % growth from t-1)

  YTAXVAT0(t)    total VAT tax collection (ref0)
  YTAXVAT00    total VAT tax collection (base-yr)
  YTAXVATX(t,sim)    total VAT tax collection (level)
  YTAXVATXP(t,sim)    total VAT tax collection (% deviation from base in t)
  YTAXVATXPP(sim)    total VAT tax collection (avg annual % growth tmin – tmax)
  YTAXVATXPPREP(sim)    total VAT tax collection (avg annual % growth tminrep – tmaxrep)
  YTAXVATXPY(t,sim)    total VAT tax collection (annual % growth from t-1)



  iemi00(ghg,ac,ac)                    emissions per unit demanded of commodity c by emitter ac
  iemi(ghg,ac,ac,t)                    emissions per unit demanded of commodity c by emitter ac


;

* END: Parameters for initial variable values - declaration =========


* START: Parameters in model and their support parameters - declaration

PARAMETERS

  dkinsb0(ins2,fcap,t)        exog investment by institution i in capital stock f
  dkinsb(ins2,fcap,t)         exog investment by institution i in capital stock f
  ddkins(ins2,fcap,t)         exog investment by institution i in capital stock f
  ddkins0(ins2,fcap,t)        exog investment by institution i in capital stock f

  qfinsb0(ins,ac,t)          exogenous real endowment of factor ac for institution ins
  qfinsb(ins,ac,t)           exogenous real endowment of factor ac for institution ins

  invvalfb0(t)               exog FDI
  invvalfb(t)                exog FDI

  invshr00(f,ins2)            share for capital stock f in investment spending of institution i
  invshr(f,ins2,t)            share for capital stock f in investment spending of institution i

  iadj01(fcap)               0-1 parameter for flexing non-government investment

  qngob(c,insngo,t)          exogenous quantity consumed of commodity c by NGO insngo
  qngob0(c,insngo,t)         exogenous quantity consumed of commodity c by NGO insngo
  qngob00(c,insngo)          exogenous quantity consumed of commodity c by NGO insngo

  qtrstb(c,ac,t)             exogenous quantity consumed of commodity c by tourist instrst
  qtrstb0(c,ac,t)            exogenous quantity consumed of commodity c by tourist instrst
  qtrstb00(c,ac)             exogenous quantity consumed of commodity c by tourist instrst

  mpsb(ins,t)                exogenous marginal propensity to save for dom non-gov inst insdng
  mpsb0(ins,t)               exogenous marginal propensity to save for dom non-gov inst insdng
  mpsb00(ins)                exogenous marginal propensity to save for dom non-gov inst insdng
  dmpsb(ins,t)                exogenous marginal propensity to save for dom non-gov inst insdng
  dmpsb0(ins,t)               exogenous marginal propensity to save for dom non-gov inst insdng
  qgb(c,t)                   exogenous qnty of government demand for commodity c
  qgb0(c,t)                  exogenous qnty of government demand for commodity c
  qgb00(c)                   exogenous qnty of government demand for commodity c
  dqg(c,t)                   exogenous qnty of government demand for commodity c
  dqg0(c,t)                  exogenous qnty of government demand for commodity c

  qgc01(c,t)                 0-1 parameter for flexing government consumption

  ty01(ins,t)                    0-1 parameter for flexing rate of direct tax on dom inst ins
  ty010(ins,t)                   0-1 parameter for flexing rate of direct tax on dom inst ins

  tyb0(ins,t)                base-year rate of direct tax on dom inst ins
  tyb00(ins)                 base-year rate of direct tax on dom inst ins
  tyb(ins,t)                 base-year rate of direct tax on dom inst ins

  tab0(a,t)                  base-year rate of tax on producer gross output value
  tab00(a)                   base-year rate of tax on producer gross output value
  tab(a,t)                   base-year rate of tax on producer gross output value

  ta01(a)                    0-1 parameter for flexing rate of tax on producer gross output value
  ta010(a)                   0-1 parameter for flexing rate of tax on producer gross output value

  tqb0(c,t)                  base-year rate of sales tax
  tqb00(c)                   base-year rate of sales tax
  tqb(c,t)                   base-year rate of sales tax

  tvacb(c,ac,t)              rate of value added tax commodity c demander ac
  tvacb00(c,ac)              rate of value added tax commodity c demander ac
  tvacb0(c,ac,t)             rate of value added tax commodity c demander ac

  subcb(c,ac,t)              subsidy rate for demander ac on comm c
  subcb00(c,ac)              subsidy rate for demander ac on comm c
  subcb0(c,ac,t)             subsidy rate for demander ac on comm c

  tfb0(f,t)                  base-year rate of direct tax on factors (soc sec tax)
  tfb00(f)                   base-year rate of direct tax on factors (soc sec tax)
  tfb(f,t)                   base-year rate of direct tax on factors (soc sec tax)
  tf01(f,t)                    0-1 parameter for flexing rate of direct tax on factors
  tf010(f,t)                   0-1 parameter for flexing rate of direct tax on factors



  teb0(c,t)                  base-year export tax rate for commodity c
  teb00(c)                   base-year export tax rate for commodity c
  teb(c,t)                   base-year export tax rate for commodity c

  tmb0(c,t)                  base-year import tariff rate for commodity c
  tmb00(c)                   base-year import tariff rate for commodity c
  tmb(c,t)                   base-year import tariff rate for commodity c
  tfab0(f,a,t)               base-year rate of tax on factor f use by activity a
  tfab00(f,a)                base-year rate of tax on factor f use by activity a
  tfab(f,a,t)                base-year rate of tax on factor f use by activity a

  shrbtvat(c,a,t)              share of VAT rebate
  shrbtvat00(c,a)              share of VAT rebate
  shrbtvat0(c,a,t)              share of VAT rebate

  trnsfrb0(ac,acp,t)         base-year transfers from insp or factorp to ins or factor
  trnsfrb00(ac,acp)          base-year transfers from insp or factorp to ins or factor
  trnsfrb(ac,acp,t)          base-year transfers from insp or factorp to ins or factor
  trnsfrbX(ac,acp,t,sim)          base-year transfers from insp or factorp to ins or factor

  dtrnsfr(ac,ins,t)    transfers from insp or factorp to ins or factor
  dtrnsfr0(ac,ins,t)   transfers from insp or factorp to ins or factor

  trnsfrpcb00(h,ins)             base transfers per capita
  trnsfrpcb0(h,ins,t)            base transfers per capita
  trnsfrpcb(h,ins,t)             base transfers per capita

  shii00(ins,insp)             share of inst ins in post-tax post-sav income of inst insp
  shii0(ins,insp,t)             share of inst ins in post-tax post-sav income of inst insp
  shii(ins,insp,t)             share of inst ins in post-tax post-sav income of inst insp

  PWE0(c,t)                  export price for c (foreign currency)
  PWE00(c)                   export price for c (foreign currency)
  PWM0(c,t)                  import price for c (foreign currency)
  PWM00(c)                   import price for c (foreign currency)


  fprdab(f,a,t)            constant in definition of productivity of factor f in act a
  fprdab0(f,a,t)           constant in definition of productivity of factor f in act a
  fprdab00(f,a)            constant in definition of productivity of factor f in act a

  fprda01(f,a)   0-1 parameter for flexing factor-specific productivity

  qdstk(c,ins2,t)             changes in inventories
  qdstk0(c,ins2,t)            changes in inventories
  qdstk00(c,ins2)             changes in inventories

  drf00                      change in foreign reserves (FCU)
  drf0(t)                    change in foreign reserves (FCU)
  drf(t)                     change in foreign reserves (FCU)

  icd(c,cp)                  trade and transport input of c per unit of comm cp produced and sold domest
  ice(c,cp)                  trade and transport input of c per unit of comm cp exported
  icm(c,cp)                  trade and transport input of c per unit of comm cp imported

  delta_va(f,a)              share parameter for CES activity production function
  phi_va(a)                  shift parameter for CES activity production function
  phi_va0(a)                 shift parameter for CES activity production function
  sigma_va(a)                elasticity of substitution bt factors
  rho_va(a)                  exponent in the value added production fn for a

  delta2(f,a)                share of prod fn level 2 to factor f in activity a
  phi2(f,a)                  efficiency parameter in the level 2 of production fn for a
  phi20(f,a)                 efficiency parameter in the level 2 of production fn for a
  sigma2(f,a)                elasticity of substitution bt factors level 2 of production fn 
  rho2(f,a)                  exponent in the level 2 of production fn of production fn for a 

  delta3(f,a)                share of prod fn level 3 to factor f in activity a
  phi3(f,a)                  efficiency parameter in the level 3 of production fn for a
  phi30(f,a)                 efficiency parameter in the level 3 of production fn for a
  sigma3(f,a)                elasticity of substitution bt factors level 3 of production fn 
  rho3(f,a)                  exponent in the level 3 of production fn of production fn for a 

* MC-2018-10-08
  ifa(f,a,t)                   factor f per unit of output (Leontief VA production function)
  ifa0(f,a,t)                  factor f per unit of output (Leontief VA production function)

  theta(a,c)                 yield of output c per unit of activity a

* domestic production aggregation function
  delta_ac(a,c)         share parameter for domestic commodity aggregation fn
  phi_ac(c)             shift parameter for domestic commodity aggregation fn
  rho_ac(c)             domestic commodity aggregation function exponent
  sigma_ac(c)           output aggregation elasticity for commodity c

  ica00(c,a)                    intermediate input c per unit of aggregate intermediate
  ica0(c,a,t)                   intermediate input c per unit of aggregate intermediate
  ica(c,a,t)                    intermediate input c per unit of aggregate intermediate

  beta(c,h)                  marginal share of hhd cons on com c
  gamma(c,h,t)               per capita subsist cons of com c for hhd h
  gamma00(c,h)               per capita subsist cons of com c for hhd h

  alpha_sav(insdng,t)        intercept savings function
  alpha_sav00(insdng)        intercept savings function


  delta_m(c)                 Armington function share parameter for imports commodity c
  delta_dd(c)                Armington function share parameter for domestic commodity c
  phi_q(c)                   Armington function shift parameter for commodity c
  sigma_q(c)                 elasticity of substitution bt. dom goods and imports for c
  rho_q(c)                   Armington function exponent for commodity c

  delta_e(c)                 CET function share parameter for exports commodity c
  delta_ds(c)                CET function share parameter for domestic commodity c
  phi_x(c)                   CET function shift parameter for commodity c
  sigma_x(c)                 elasticity of transformation bt. dom sales and exports for c
  rho_x(c)                   CET function exponent for commodity c

  scaldelta_ds(c,t)          scaling factor for delta_ds
  scaldelta_e(c,t)           scaling factor for delta_e
  scalphi_x(c,t)             scaling factor for phi_x



  cwts(c,h)                  weight of commodity c in the CPI
  dwts(c)                    domestic sales price weights

  eta_wf(f)                  elas of real wage with respect to unemployment rate

  qeb00(c)                   export demand for c if pwe = pwse (world price for substitutes)
  qeb0(c,t)                  export demand for c if pwe = pwse (world price for substitutes)
  qeb(c,t)                   export demand for c if pwe = pwse (world price for substitutes)
  pwse00(c)                  world price for commodity c substitutes in export markets
  pwse0(c,t)                 world price for commodity c substitutes in export markets
  pwse(c,t)                  world price for commodity c substitutes in export markets
  eta_e(c)                   constant price elasticity of export demand for c (< 0)

* exchange rate premium

  shryprexr00(c,ac)       share of rents from exchage rate premium in commodity c to demander ac
  shryprexr0(c,ac,t)      share of rents from exchage rate premium in commodity c to demander ac
  shryprexr(c,ac,t)       share of rents from exchage rate premium in commodity c to demander ac
  rexrindex(t)             index for (official) exchange rate (dom. currency per unit of for. currency)
  prexrindex(t)                    index for exchange rate premium

  shroe00(c)              share of exports of commodity c at the official exchange rate
  shroe0(c,t)             share of exports of commodity c at the official exchange rate
  shroe(c,t)              share of exports of commodity c at the official exchange rate

  shrom00(c)              share of imports of commodity c at the official exchange rate
  shrom0(c,t)             share of imports of commodity c at the official exchange rate
  shrom(c,t)              share of imports of commodity c at the official exchange rate
  
* import quota 

  qmbar00(c)              import quota volume for commodity c
  qmbar0(c,t)             import quota volume for commodity c 
  qmbar(c,t)              import quota volume for commodity c 
  qmbarindex(c,t)         index for import quota volume for commodity c 


* dynamics
  kappa                      capital mobility parameter

  netprfrat(fcap)            net profit rate used to estimate initial non-gov capital stocks

  pop00(ac)                  base-year population
  pop(ac,t)                  population
  popgrw(ac,t)               population growth rate
  gdpindex(t)                index for GDP
  popindex(ac,t)             index for population
  qfacindex(f,t)             index for factor supply
  fprdindex(f,t)             index for factor-specific productivity
  prdindex(t)                index for productivity
  agelabindex(t)             index for population in labor force age

  capcomp(fcap,c)            qnty of commodity c PEDISR unit of investment in fcap
* tfpexog0(a,t)              exogenous component of sectoral TFP
  tfpexog(a,t)               exogenous component of sectoral TFP
  tfp01(a)                   0-1 par determining (relative) rate of act a TFP growth (for GDP calibration) (0 <= tfp01 <= 1)

  govrecindex0(acgovrec,t)   index for government receipt acgovrec
  govspndindex0(acgovspnd,t) index for government spending acgovspnd
  ngovpayindex0(acngovpay,t) index for non-government payment acngovpay

  nffinsbar00           base-year net foreign financing dom non-gov inst (FCU)
  nffinsbar0(t)          base-year net foreign financing dom non-gov inst (FCU)
  nffinsbar(t)           base-year net foreign financing dom non-gov inst (FCU)

  gintrat00                  real interest rate on domestic government debt
  gintrat(t)                 real interest rate on domestic government debt

  fintrat00(ins2)                  real interest rate on foreign debt for domestic institution insd
  fintrat(ins2,t)             real interest rate on foreign debt for domestic institution insd
;

* END: Parameters in model and their support parameters - declaration




DISPLAY "#### END: par-decl-aux.inc";