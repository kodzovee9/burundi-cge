* diagnostics-sol.inc

DISPLAY "#### START: diagnostics-sol.inc";

* CNS
$ONTEXT
IF ((GEM.MODELSTAT NE 16),
  ABORT 'error solving CGE5_ext; MODELSTAT <> 16';
);
$OFFTEXT

* MCP
*$ONTEXT
IF ((GEM.MODELSTAT NE 1),
  ABORT 'error solving CGE5_ext; MODELSTAT <> 1';
);
*$OFFTEXT


* check walras law
IF ((ABS(WALRAS.L(tsol))>0.0001),
  ABORT 'error solving CGE5_ext; WALRAS <> 0';
);


DISPLAY "#### END: diagnostics-sol.inc";