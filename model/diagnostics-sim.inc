* diagnostics-sim.inc

DISPLAY "#### START: diagnostics-sim.inc";


PARAMETER 
  errpwesim(sim,c)    if =UNDF shock (value not equal to one) in tmin with dyn model
  errpwmsim(sim,c)    if =UNDF shock (value not equal to one) in tmin with dyn model  
;

errpwesim(simcur,c)$(dmod NE 0 AND SUM(tp, pwesim(simcur,c,tp)) AND SUM(tmin, pwesim(simcur,c,tmin)) NE 1) = 1/0;
DISPLAY errpwesim;
errpwmsim(simcur,c)$(dmod NE 0 AND SUM(tp, pwmsim(simcur,c,tp)) AND SUM(tmin, pwmsim(simcur,c,tmin)) NE 1) = 1/0;
DISPLAY errpwmsim;


DISPLAY "#### END: diagnostics-sim.inc";