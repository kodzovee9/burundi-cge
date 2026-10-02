* irrmodel.inc


DISPLAY "#### START: irrmodel.inc";


*START: IRR model====================================================

*Computing the internal rate of return (irr) for investments in capital 
*stock f. Note: the irr is the interest at which the present value of 
*net income over the life time of an investment in f is zero.

SET
  tirr  time periods for irr calculation
    /1*100/
;

PARAMETERS
  va00(a)        base-year VA in activity a
  va0(a,tirr)    VA in activity a in tirr (not adjusted for investment impact)
  vagrw          annual growth in real VA (GDP at fc during simulation period)
  tsolnb2
;
 
va00(a)$SUM(fcapg, mpcapgov_af(a,fcapg)) = SUM(f, SAM(f,a));
DISPLAY va00;

tsolnb2 = SUM(t$tmax(t), ORD(t)) - SUM(t$tmin(t), ORD(t));
tsolnb2$(dmod=0) = 1;
DISPLAY tsolnb2;

vagrw = ( SUM(tmax, RGDPFC0(tmax)) / SUM(tmin, RGDPFC0(tmin)) ) ** (1/tsolnb2) - 1;
DISPLAY vagrw;

va0(a,tirr)$va00(a) = va00(a)*(1 + vagrw)**(ORD(tirr) - 1);
DISPLAY va0;

PARAMETER
  invincf(fcapg,ac,tirr)  income (benefit) from f in activity a in tirr
;

* investment income in years after the first (stock is not available in the first year)
invincf(fcapg,a,tirr)$(ORD(tirr) GT 1) = 
  mpcapgov_af(a,fcapg)*((1 + vagrw)*(1 - deprcap(fcapg)))**(ORD(tirr) - 2);
  
invincf(fcapg,'total',tirr) = SUM(a, invincf(fcapg,a,tirr));
DISPLAY invincf;


PARAMETER
  invcostf(fcapg,tirr)  cost per unit of cap stock f in tirr
;
* in the base year the cost of one unit of capital is 1
invcostf(fcapg,tirr)$(mpcapgov(fcapg) AND (ORD(tirr) EQ 1)) = 1;
DISPLAY invcostf;

* in subsequent years, the cost is the amount of the supporting activity a needed
* per unit of remaining capital stock)

invcostf(fcapg,tirr)$(mpcapgov(fcapg) AND (ORD(tirr) GT 1)) =
  0.04
* [cost per unit of remaining capital stock] 
* (4% (here equal to 0.04 LCU) estimate based on Fay and Yepes (2003) and Perrotti and Sanches (2011))
  
  * (1 - deprcap(fcapg))**(ORD(tirr) - 2);
* TIMES [remaining share of capital stock f (1 in year 2, lower in later years due to depreciation]


* Note: The capital stock starts to depreciate in year 3.


DISPLAY invcostf, mpcapgov;



PARAMETER
  invnetincf(fcapg,tirr) net investment income for f in tirr
;
invnetincf(fcapg,tirr) =  invincf(fcapg,'total',tirr) -  invcostf(fcapg,tirr);

DISPLAY invnetincf, invincf, invcostf;

PARAMETER
 invnetinc(tirr) net income from investment (parameter for irrmodel)
 ;

VARIABLE
 IRR    internal rate of return of investment in f
 NPV    NPV of investment in f 
 NPVSQ  objective variable (NPV squared)

EQUATIONS
 EQ_IRRDEF     definition of internal rate of return of investment in f
 EQ_NPVSQDEF   minimand -- definition of NPV squared
; 

EQ_IRRDEF..
  NPV =E= SUM(tirr, (invnetinc(tirr)/((1 + IRR)**ORD(tirr))));  

EQ_NPVSQDEF.. 
  NPVSQ =E= SQR(NPV); 

  

MODEL 
 irrmodel  model for BOTE calcualtion of the irr for government investments (NLP version
 /
 EQ_IRRDEF
 /

;

PARAMETERS
* parameters for reports
  irr00(fcapg)       internal rate of return of investment in f
  npv00(fcapg)	        net present value at optimal solution for IRR model (should be zero)
  irrsolution(*)    status of solver & model + number of redefined variables for IRR model
;

PARAMETER 
  irrinitval(fcapg)  initial value (+0.01 or -0.01) based on NPV for IRR=0
; 
irrinitval(fcapg)$(SUM(tirr, invnetincf(fcapg,tirr)) GT 0) = +0.01;
irrinitval(fcapg)$(SUM(tirr, invnetincf(fcapg,tirr)) LT 0) = -0.01; 
irrinitval(fcapg)$(SUM(tirr, invnetincf(fcapg,tirr)) EQ 0) = 0;  
DISPLAY irrinitval; 

 
LOOP(fcapg$(mpcapgov(fcapg) GT 0.01 AND dmod NE 0),

  IRR.L           =  irrinitval(fcapg);
  invnetinc(tirr) = invnetincf(fcapg,tirr);
  NPV.FX = 0;

  DISPLAY IRR.LO, IRR.UP, IRR.L; 

  SOLVE IRRMODEL USING CNS;
 
  irr00(fcapg)   = IRR.L;
  npv00(fcapg)   = NPV.L;

  irrsolution('modelstat') = IRRMODEL.MODELSTAT;
  irrsolution('solvestat') = IRRMODEL.SOLVESTAT;

  DISPLAY irrsolution, npv00, irr00;
);  
$EXIT

 ABORT$((irrsolution('modelstat') GT 2) OR (irrsolution('solvestat') NE 1))
"Error in solution for the IRR model."
"Suggested action: Review information about model status, solver"
"status, as well as the data used (mpcapgov, mtfp, and SAM, and" 
"data for activity producting services related to each capital" 
"stock with non-zero value for mpcapgov)."; 

ABORT$(ABS(NPV.L) GT 1.0E-6) 
"Error in model for IRR computation since the NPV is not zero."
"Suggested action: Review the data used (mpcapgov, mtfp, and SAM"
"data for activity producting services related to each capital"
"stock with non-zero value for mpcapgov)."; 


DISPLAY irrsolution, npv00, irr00;
                                );
*END: IRR model======================================================



DISPLAY "#### END: irrmodel.inc";