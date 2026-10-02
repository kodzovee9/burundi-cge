* facclos.inc

DISPLAY "#### START: facclos.inc";



* for labor, fix labor force participation rate
LABPARTRAT.FX(t) = LABPARTRAT0(t);
QLABSCAL.LO(t) = -INF;
QLABSCAL.UP(t) = +INF;
$ONTEXT
* alt:
LABPARTRAT.LO(t) = -INF;
LABPARTRAT.UP(t) = +INF;
QLABSCAL.FX(t) = QLABSCAL0(t);
$OFFTEXT



* to check: facclossim defined for factors in SAM
facclossim(sim,f)$fnsam(f) = 0;

* by definition, factors not in SAM are sector-specfic
WFDIST.LO(f,a,t)$fnsam(f) = -INF;
WFDIST.UP(f,a,t)$fnsam(f) = +INF;

* there is no market for non-natural factors
WF.FX(f,t)$fnsam(f) = WF0(f,t);

  


LOOP(f,

* full employment (fixed UERAT); mobile factor
  IF (facclossim(sim,f) EQ 1,
    WFDIST.FX(f,a,t) = WFDIST0(f,a,t);
    WF.LO(f,t)       = -INF;
    WF.UP(f,t)       = +INF;
    QF.LO(f,a,t)$QF0(f,a,t) = -INF;
    QF.UP(f,a,t)$QF0(f,a,t) = +INF;
*   QFS.FX(f,t)      = QFS0(f,t);

*HL-Start
*   QFINSSCAL.FX(f,t)      = QFINSSCAL0(f,t);
    QFINSSCAL.FX(f,t)$(NOT SUM(tp, QFINSSCALSIM(sim,f,tp))) = QFINSSCAL0(f,t);
    QFINSSCAL.FX(f,t)$SUM(tp, QFINSSCALSIM(sim,f,tp)) = QFINSSCALSIM(sim,f,t);	
*HL-End
	
    UERAT.FX(f,t) = UERAT0(f,t);
  );

* full employment (fixed UERAT); specific factor
  IF (facclossim(sim,f) EQ 2,
    WFDIST.LO(f,a,t)$QF0(f,a,t) = -INF;
    WFDIST.UP(f,a,t)$QF0(f,a,t) = +INF;
    WF.FX(f,t)       = WF0(f,t);

*HL-Start
*   QF.FX(f,a,t)     = QF0(f,a,t);
    QF.FX(f,a,t)$(NOT SUM(tp, QFSIM(sim,f,a,tp))) = QF0(f,a,t);
    QF.FX(f,a,t)$SUM(tp, QFSIM(sim,f,a,tp)) = QFSIM(sim,f,a,t);	
*HL-End

*   QFS.LO(f,t)      = -INF;
*   QFS.UP(f,t)      = +INF;
    QFINSSCAL.LO(f,t)      = -INF;
    QFINSSCAL.UP(f,t)      = +INF;    
    UERAT.FX(f,t) = UERAT0(f,t);
  );

* unemployment assuming exgenous (constant) real wage; mobile factor
  IF (facclossim(sim,f) EQ 3,
    WFDIST.FX(f,a,t) = WFDIST0(f,a,t);
    WF.FX(f,t)       = WF0(f,t);
    QF.LO(f,a,t)$QF0(f,a,t) = -INF;
    QF.UP(f,a,t)$QF0(f,a,t) = +INF;
*    QFS.LO(f,t)      = -INF;
*    QFS.UP(f,t)      = +INF;
    QFINSSCAL.LO(f,t)      = -INF;
    QFINSSCAL.UP(f,t)      = +INF;
    
    UERAT.FX(f,t) = UERAT0(f,t);    
  );

* unemployment assuming wage curve (i.e., negative relation bt real wage and UERAT); mobile factor
  IF (facclossim(sim,f) EQ 4,
    WFDIST.FX(f,a,t) = WFDIST0(f,a,t);
    WF.LO(f,t)       = -INF;
    WF.UP(f,t)       = +INF;
    QF.LO(f,a,t)$QF0(f,a,t) = -INF;
    QF.UP(f,a,t)$QF0(f,a,t) = +INF;
*    QFS.FX(f,t)      = QFS0(f,t);
    QFINSSCAL.FX(f,t)      = QFINSSCAL0(f,t);    
    UERAT.LO(f,t) = -INF;
    UERAT.UP(f,t) = +INF;
  );
  
);  


$ONTEXT
LOOP(f,

* factor movil con pleno empleo
  IF (facclossim(sim,f) EQ 1,
    WFDIST.FX(f,a,t) = WFDIST0(f,a,t);
    WF.LO(f,t)       = -INF;
    WF.UP(f,t)       = +INF;
    QF.LO(f,a,t)$QF00(f,a) = -INF;
    QF.UP(f,a,t)$QF00(f,a) = +INF;
    QFS.FX(f,t)      = QFS0(f,t);
  );

* factor inmóvil con pleno empleo
  IF (facclossim(sim,f) EQ 2,
    WFDIST.LO(f,a,t)$QF00(f,a) = -INF;
    WFDIST.UP(f,a,t)$QF00(f,a) = +INF;
    WF.FX(f,t)       = WF0(f,t);
    QF.FX(f,a,t)     = QF0(f,a,t);
    QFS.LO(f,t)      = -INF;
    QFS.UP(f,t)      = +INF;
  );

* factor móvil con desempleo  
  IF (facclossim(sim,f) EQ 3,
    WFDIST.FX(f,a,t) = WFDIST0(f,a,t);
    WF.FX(f,t)       = WF0(f,t);
    QF.LO(f,a,t)$QF00(f,a) = -INF;
    QF.UP(f,a,t)$QF00(f,a) = +INF;
    QFS.LO(f,t)      = -INF;
    QFS.UP(f,t)      = +INF;
  );
  
);  
$OFFTEXT





* in dynamic model, capital is always sector-specific
WFDIST.LO(fcap(f),a,t)$(dmod NE 0 AND QF00(f,a)) = -INF;
WFDIST.UP(fcap(f),a,t)$(dmod NE 0 AND QF00(f,a)) = +INF;
WF.FX(fcap(f),t)$dmod = WF0(f,t);
QF.FX(fcap(f),a,t)$(dmod NE 0 AND tmin(t)) = QF0(f,a,t);
QF.LO(fcap(f),a,t)$(QF00(f,a) AND dmod NE 0 AND NOT tmin(t)) = -INF;
QF.UP(fcap(f),a,t)$(QF00(f,a) AND dmod NE 0 AND NOT tmin(t)) = +INF;
*QFS.LO(fcap(f),t)$dmod = -INF;
*QFS.UP(fcap(f),t)$dmod = +INF;
* no need to flex QFINSSCAL for capital endowments, given that we have droped factor market eq cond for capital
* note that capital income is dist based on pop shares; not based on saving rates
*QFINSSCAL.LO(fcap(f),t)$dmod = -INF;
*QFINSSCAL.UP(fcap(f),t)$dmod = +INF;


DISPLAY "#### END: facclos.inc";

