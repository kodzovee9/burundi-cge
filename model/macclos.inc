* macclos.inc

DISPLAY "#### START: macclos.inc";

$ONTEXT
*HL
IF(SUM(tp, rgdpfcsim(sim,tp)),
  TFPSCAL.LO(t) = -INF;
  TFPSCAL.UP(t) = +INF;
  RGDPFC.FX(t)  = rgdpfcsim(sim,t);
  RGDPFC.FX(t)  = rgdpfcsim(sim,t); 
ELSE 
  RGDPFC.LO(t) = -INF;
  RGDPFC.UP(t) = +INF;
  TFPSCAL.FX(t) = TFPSCAL0(t);
);
$OFFTEXT  


* GOVRECRULE ========================================================

* not yet: trgovngov

* transfers from row to gov
LOOP(trgovrow,
  IF(govrecrulesim(sim,trgovrow) EQ 1,
    TRNSFRSCAL.FX('trgovrow',t) = TRNSFRSCAL0('trgovrow',t);
    trnsfrb(insgov,insrow,t)$(NOT govrecgrwsim(sim,trgovrow,t)) = trnsfrb0(insgov,insrow,t);
    trnsfrb(insgov,insrow,t)$govrecgrwsim(sim,trgovrow,t)       = trnsfrb00(insgov,insrow)*govrecindexsim(sim,trgovrow,t);
    GOVRECGDP.LO(trgovrow,t) = -INF;
    GOVRECGDP.UP(trgovrow,t) = +INF;
    GOVRECABS.LO(trgovrow,t) = -INF;
    GOVRECABS.UP(trgovrow,t) = +INF;
    
  );
  IF(govrecrulesim(sim,trgovrow) EQ 2,
    TRNSFRSCAL.LO(trgovrow,t) = -INF;
    TRNSFRSCAL.UP(trgovrow,t) = +INF;
    GOVRECGDP.FX(trgovrow,t)$(NOT govrecgdpsim(sim,trgovrow,t)) = GOVRECGDP0(trgovrow,t);
    GOVRECGDP.FX(trgovrow,t)$govrecgdpsim(sim,trgovrow,t)       = govrecgdpsim(sim,trgovrow,t)*GOVRECGDP00(trgovrow)/SUM(tmin, govrecgdpsim(sim,trgovrow,tmin));
    GOVRECABS.LO(trgovrow,t) = -INF;
    GOVRECABS.UP(trgovrow,t) = +INF;

  );
  IF(govrecrulesim(sim,trgovrow) EQ 3,
    TRNSFRSCAL.LO(trgovrow,t) = -INF;
    TRNSFRSCAL.UP(trgovrow,t) = +INF;
    GOVRECGDP.LO(trgovrow,t) = -INF;
    GOVRECGDP.UP(trgovrow,t) = +INF;
    GOVRECABS.FX(trgovrow,t)$(NOT govrecabssim(sim,trgovrow,t)) = GOVRECABS0(trgovrow,t);
    GOVRECABS.FX(trgovrow,t)$govrecabssim(sim,trgovrow,t)       = govrecabssim(sim,trgovrow,t)*GOVRECABS00(trgovrow)/SUM(tmin, govrecabssim(sim,trgovrow,tmin));
  );
  
);  
    
    
* rate of direct tax on dom inst ins
LOOP(taxdir,
  IF(govrecrulesim(sim,taxdir) EQ 1,
    TYSCAL.FX(t) = TYSCAL0(t);
    tyb(insdng,t)$(NOT taxratesim(sim,taxdir,insdng,t)) = tyb0(insdng,t);
    tyb(insdng,t)$taxratesim(sim,taxdir,insdng,t)       = tyb00(insdng)*taxratesim(sim,taxdir,insdng,t)/SUM(tmin, taxratesim(sim,taxdir,insdng,tmin));
    GOVRECGDP.LO(taxdir,t) = -INF;
    GOVRECGDP.UP(taxdir,t) = +INF;
    GOVRECABS.LO(taxdir,t) = -INF;
    GOVRECABS.UP(taxdir,t) = +INF;
  );
  IF(govrecrulesim(sim,taxdir) EQ 2,
    TYSCAL.LO(t) = -INF;
    TYSCAL.UP(t) = +INF;
    GOVRECGDP.FX(taxdir,t)$(NOT govrecgdpsim(sim,taxdir,t)) = GOVRECGDP0(taxdir,t);
    GOVRECGDP.FX(taxdir,t)$(govrecgdpsim(sim,taxdir,t)) = govrecgdpsim(sim,taxdir,t)*GOVRECGDP00(taxdir)/SUM(tmin, govrecgdpsim(sim,taxdir,tmin));
    GOVRECABS.LO(taxdir,t) = -INF;
    GOVRECABS.UP(taxdir,t) = +INF;
  );
  IF(govrecrulesim(sim,taxdir) EQ 3,
    TYSCAL.LO(t) = -INF;
    TYSCAL.UP(t) = +INF;
    GOVRECGDP.LO(taxdir,t) = -INF;
    GOVRECGDP.UP(taxdir,t) = +INF;    
    GOVRECABS.FX(taxdir,t)$(NOT govrecabssim(sim,taxdir,t)) = GOVRECABS0(taxdir,t);
    GOVRECABS.FX(taxdir,t)$(govrecabssim(sim,taxdir,t)) = govrecabssim(sim,taxdir,t)*GOVRECABS00(taxdir)/SUM(tmin, govrecabssim(sim,taxdir,tmin));
  );
  
);  

* rate of tax on producer gross output value
LOOP(taxact,
  IF(govrecrulesim(sim,taxact) EQ 1,
    TASCAL.FX(t) = TASCAL0(t);  
    tab(a,t)$(NOT taxratesim(sim,taxact,a,t)) = tab0(a,t);
    tab(a,t)$taxratesim(sim,taxact,a,t)       = tab00(a)*taxratesim(sim,taxact,a,t)/SUM(tmin, taxratesim(sim,taxact,a,tmin));
    GOVRECGDP.LO(taxact,t) = -INF;
    GOVRECGDP.UP(taxact,t) = +INF;
    GOVRECABS.LO(taxact,t) = -INF;
    GOVRECABS.UP(taxact,t) = +INF;    
  );
  IF(govrecrulesim(sim,taxact) EQ 2,
    TASCAL.LO(t) = -INF;
    TASCAL.UP(t) = +INF;
    GOVRECGDP.FX(taxact,t)$(NOT govrecgdpsim(sim,taxact,t)) = GOVRECGDP0(taxact,t);
    GOVRECGDP.FX(taxact,t)$(govrecgdpsim(sim,taxact,t)) = govrecgdpsim(sim,taxact,t)*GOVRECGDP00(taxact)/SUM(tmin, govrecgdpsim(sim,taxact,tmin));
    GOVRECABS.LO(taxact,t) = -INF;
    GOVRECABS.UP(taxact,t) = +INF;
  );
  IF(govrecrulesim(sim,taxact) EQ 3,
    TASCAL.LO(t) = -INF;
    TASCAL.UP(t) = +INF;
    GOVRECGDP.LO(taxact,t) = -INF;
    GOVRECGDP.UP(taxact,t) = +INF;
    GOVRECABS.FX(taxact,t)$(NOT govrecabssim(sim,taxact,t)) = GOVRECABS0(taxact,t);
    GOVRECABS.FX(taxact,t)$(govrecabssim(sim,taxact,t)) = govrecabssim(sim,taxact,t)*GOVRECABS00(taxact)/SUM(tmin, govrecabssim(sim,taxact,tmin));
  );  
);
  
* rate of sales tax
LOOP(taxcom,
  IF(govrecrulesim(sim,taxcom) EQ 1,
    TQSCAL.FX(t) = TQSCAL0(t);  
    tqb(c,t)$(NOT taxratesim(sim,taxcom,c,t)) = tqb0(c,t);
    tqb(c,t)$taxratesim(sim,taxcom,c,t)       = tqb00(c)*taxratesim(sim,taxcom,c,t)/SUM(tmin, taxratesim(sim,taxcom,c,tmin));
    GOVRECGDP.LO(taxcom,t) = -INF;
    GOVRECGDP.UP(taxcom,t) = +INF;
    GOVRECABS.LO(taxcom,t) = -INF;
    GOVRECABS.UP(taxcom,t) = +INF;
  );
  IF(govrecrulesim(sim,taxcom) EQ 2,
    TQSCAL.LO(t) = -INF;
    TQSCAL.UP(t) = +INF;
    GOVRECGDP.FX(taxcom,t)$(NOT govrecgdpsim(sim,taxcom,t)) = GOVRECGDP0(taxcom,t);
    GOVRECGDP.FX(taxcom,t)$(govrecgdpsim(sim,taxcom,t))     = govrecgdpsim(sim,taxcom,t)*GOVRECGDP00(taxcom)/SUM(tmin, govrecgdpsim(sim,taxcom,tmin));
    GOVRECABS.LO(taxcom,t) = -INF;
    GOVRECABS.UP(taxcom,t) = +INF;
  );
  IF(govrecrulesim(sim,taxcom) EQ 3,
    TQSCAL.LO(t) = -INF;
    TQSCAL.UP(t) = +INF;
    GOVRECGDP.LO(taxcom,t) = -INF;
    GOVRECGDP.UP(taxcom,t) = +INF;    
    GOVRECABS.FX(taxcom,t)$(NOT govrecabssim(sim,taxcom,t)) = GOVRECABS0(taxcom,t);
    GOVRECABS.FX(taxcom,t)$(govrecabssim(sim,taxcom,t))     = govrecabssim(sim,taxcom,t)*GOVRECABS00(taxcom)/SUM(tmin, govrecabssim(sim,taxcom,tmin));
  );
  
);  

* rate of direct tax on factors (soc sec tax)
LOOP(taxfac,
  IF(govrecrulesim(sim,taxfac) EQ 1,
    TFSCAL.FX(t) = TFSCAL0(t);  
    tfb(f,t)$(NOT taxratesim(sim,taxfac,f,t)) = tfb0(f,t);
    tfb(f,t)$taxratesim(sim,taxfac,f,t)       = tfb00(f)*taxratesim(sim,taxfac,f,t)/SUM(tmin, taxratesim(sim,taxfac,f,tmin));
    GOVRECGDP.LO(taxfac,t) = -INF;
    GOVRECGDP.UP(taxfac,t) = +INF;
    GOVRECABS.LO(taxfac,t) = -INF;
    GOVRECABS.UP(taxfac,t) = +INF;
  );
  IF(govrecrulesim(sim,taxfac) EQ 2,
    TFSCAL.LO(t) = -INF;
    TFSCAL.UP(t) = +INF;
    GOVRECGDP.FX(taxfac,t)$(NOT govrecgdpsim(sim,taxfac,t)) = GOVRECGDP0(taxfac,t);
    GOVRECGDP.FX(taxfac,t)$(govrecgdpsim(sim,taxfac,t)) = govrecgdpsim(sim,taxfac,t)*GOVRECGDP00(taxfac)/SUM(tmin, govrecgdpsim(sim,taxfac,tmin));
    GOVRECABS.LO(taxfac,t) = -INF;
    GOVRECABS.UP(taxfac,t) = +INF;
  );
  IF(govrecrulesim(sim,taxfac) EQ 3,
    TFSCAL.LO(t) = -INF;
    TFSCAL.UP(t) = +INF;
    GOVRECGDP.LO(taxfac,t) = -INF;
    GOVRECGDP.UP(taxfac,t) = +INF;
    GOVRECABS.FX(taxfac,t)$(NOT govrecabssim(sim,taxfac,t)) = GOVRECABS0(taxfac,t);
    GOVRECABS.FX(taxfac,t)$(govrecabssim(sim,taxfac,t)) = govrecabssim(sim,taxfac,t)*GOVRECABS00(taxfac)/SUM(tmin, govrecabssim(sim,taxfac,tmin));

  );
  
); 

* export tax rate for commodity c
LOOP(taxexp,
  IF(govrecrulesim(sim,taxexp) EQ 1,
    TESCAL.FX(t) = TESCAL0(t);  
    teb(c,t)$(NOT taxratesim(sim,taxexp,c,t) AND NOT taxrate2sim(sim,taxexp,c,t)) = teb0(c,t);
    teb(c,t)$taxratesim(sim,taxexp,c,t)       = teb00(c)*taxratesim(sim,taxexp,c,t)/SUM(tmin, taxratesim(sim,taxexp,c,tmin));
    teb(c,t)$taxrate2sim(sim,taxexp,c,t)      = taxrate2sim(sim,taxexp,c,t);
    GOVRECGDP.LO(taxexp,t) = -INF;
    GOVRECGDP.UP(taxexp,t) = +INF;
    GOVRECABS.LO(taxexp,t) = -INF;
    GOVRECABS.UP(taxexp,t) = +INF;
  );
  IF(govrecrulesim(sim,taxexp) EQ 2,
    TESCAL.LO(t) = -INF;
    TESCAL.UP(t) = +INF;
    GOVRECGDP.FX(taxexp,t)$(NOT govrecgdpsim(sim,taxexp,t)) = GOVRECGDP0(taxexp,t);
    GOVRECGDP.FX(taxexp,t)$(govrecgdpsim(sim,taxexp,t))     = govrecgdpsim(sim,taxexp,t)*GOVRECGDP00(taxexp)/SUM(tmin, govrecgdpsim(sim,taxexp,tmin));
    GOVRECABS.LO(taxexp,t) = -INF;
    GOVRECABS.UP(taxexp,t) = +INF;
  );
  IF(govrecrulesim(sim,taxexp) EQ 3,
    TESCAL.LO(t) = -INF;
    TESCAL.UP(t) = +INF;
    GOVRECGDP.LO(taxexp,t) = -INF;
    GOVRECGDP.UP(taxexp,t) = +INF;
    GOVRECABS.FX(taxexp,t)$(NOT govrecabssim(sim,taxexp,t)) = GOVRECABS0(taxexp,t);
    GOVRECABS.FX(taxexp,t)$(govrecabssim(sim,taxexp,t))     = govrecabssim(sim,taxexp,t)*GOVRECABS00(taxexp)/SUM(tmin, govrecabssim(sim,taxexp,tmin));
  );  
);  



* import tariff rate for commodity c
LOOP(taximp,
  IF(govrecrulesim(sim,taximp) EQ 1,
    TMSCAL.FX(t) = TMSCAL0(t);  
    tmb(c,t)$(NOT taxratesim(sim,taximp,c,t) AND NOT taxrate2sim(sim,taximp,c,t)) = tmb0(c,t);
    tmb(c,t)$taxratesim(sim,taximp,c,t)       = tmb00(c)*taxratesim(sim,taximp,c,t)/SUM(tmin, taxratesim(sim,taximp,c,tmin));
    tmb(c,t)$taxrate2sim(sim,taximp,c,t)      = taxrate2sim(sim,taximp,c,t);
    GOVRECGDP.LO(taximp,t) = -INF;
    GOVRECGDP.UP(taximp,t) = +INF;
    GOVRECABS.LO(taximp,t) = -INF;
    GOVRECABS.UP(taximp,t) = +INF;
  );
  IF(govrecrulesim(sim,taximp) EQ 2,
    TMSCAL.LO(t) = -INF;
    TMSCAL.UP(t) = +INF;
    GOVRECGDP.FX(taximp,t)$(NOT govrecgdpsim(sim,taximp,t)) = GOVRECGDP0(taximp,t);
    GOVRECGDP.FX(taximp,t)$(govrecgdpsim(sim,taximp,t))     = govrecgdpsim(sim,taximp,t)*GOVRECGDP00(taximp)/SUM(tmin, govrecgdpsim(sim,taximp,tmin));
    GOVRECABS.LO(taximp,t) = -INF;
    GOVRECABS.UP(taximp,t) = +INF;
  );
  IF(govrecrulesim(sim,taximp) EQ 3,
    TMSCAL.LO(t) = -INF;
    TMSCAL.UP(t) = +INF;
    GOVRECGDP.LO(taximp,t) = -INF;
    GOVRECGDP.UP(taximp,t) = +INF;
    GOVRECABS.FX(taximp,t)$(NOT govrecabssim(sim,taximp,t)) = GOVRECABS0(taximp,t);
    GOVRECABS.FX(taximp,t)$(govrecabssim(sim,taximp,t))     = govrecabssim(sim,taximp,t)*GOVRECABS00(taximp)/SUM(tmin, govrecabssim(sim,taximp,tmin));
  );
  
);  

* rate of VAT
LOOP(taxvatc,
  IF(govrecrulesim(sim,taxvatc) EQ 1,
    TVACSCAL.FX(t) = TVACSCAL0(t);  
    tvacb(c,ac,t)$(NOT taxratesim(sim,taxvatc,c,t) AND taxrate2sim(sim,taxvatc,c,t)) = tvacb0(c,ac,t);
    tvacb(c,ac,t)$taxratesim(sim,taxvatc,c,t)  = tvacb00(c,ac)*taxratesim(sim,taxvatc,c,t)/SUM(tmin, taxratesim(sim,taxvatc,c,tmin));
    tvacb(c,ac,t)$taxrate2sim(sim,taxvatc,c,t) = taxrate2sim(sim,taxvatc,c,t);
    tvacb(c,ac,t)$taxrate3sim(sim,taxvatc,c,t) = tvacb0(c,ac,t) + taxrate3sim(sim,taxvatc,c,t);




    GOVRECGDP.LO(taxvatc,t) = -INF;
    GOVRECGDP.UP(taxvatc,t) = +INF;
    GOVRECABS.LO(taxvatc,t) = -INF;
    GOVRECABS.UP(taxvatc,t) = +INF;
  );
  IF(govrecrulesim(sim,taxvatc) EQ 2,
    TVACSCAL.LO(t) = -INF;
    TVACSCAL.UP(t) = +INF;
    GOVRECGDP.FX(taxvatc,t)$(NOT govrecgdpsim(sim,taxvatc,t)) = GOVRECGDP0(taxvatc,t);
    GOVRECGDP.FX(taxvatc,t)$(govrecgdpsim(sim,taxvatc,t))     = govrecgdpsim(sim,taxvatc,t)*GOVRECGDP00(taxvatc)/SUM(tmin, govrecgdpsim(sim,taxvatc,tmin));
    GOVRECABS.LO(taxvatc,t) = -INF;
    GOVRECABS.UP(taxvatc,t) = +INF;
  );
  IF(govrecrulesim(sim,taxvatc) EQ 3,
    TVACSCAL.LO(t) = -INF;
    TVACSCAL.UP(t) = +INF;
    GOVRECGDP.LO(taxvatc,t) = -INF;
    GOVRECGDP.UP(taxvatc,t) = +INF;
    GOVRECABS.FX(taxvatc,t)$(NOT govrecabssim(sim,taxvatc,t)) = GOVRECABS0(taxvatc,t);
    GOVRECABS.FX(taxvatc,t)$(govrecabssim(sim,taxvatc,t))     = govrecabssim(sim,taxvatc,t)*GOVRECABS00(taxvatc)/SUM(tmin, govrecabssim(sim,taxvatc,tmin));

  );
  
);  




* rate of tax on factor use
TFASCAL.FX(t) = TFASCAL0(t);



* net foreign financing
LOOP(netforfingov,
  IF(govrecrulesim(sim,netforfingov) EQ 1,
    NFFG.FX(t) = NFFG0(t);
    GOVRECGDP.LO(netforfingov,t) = -INF;
    GOVRECGDP.UP(netforfingov,t) = +INF;
    GOVRECABS.LO(netforfingov,t) = -INF;
    GOVRECABS.UP(netforfingov,t) = +INF;
  );
  IF(govrecrulesim(sim,netforfingov) EQ 2,
    NFFG.LO(t) = -INF;
    NFFG.UP(t) = +INF;
    GOVRECGDP.FX(netforfingov,t)$(NOT govrecgdpsim(sim,netforfingov,t)) = GOVRECGDP0(netforfingov,t);
    GOVRECGDP.FX(netforfingov,t)$govrecgdpsim(sim,netforfingov,t)       = govrecgdpsim(sim,netforfingov,t)*GOVRECGDP00(netforfingov)/SUM(tmin, govrecgdpsim(sim,netforfingov,tmin));
    GOVRECABS.LO(netforfingov,t) = -INF;
    GOVRECABS.UP(netforfingov,t) = +INF;
  );
  IF(govrecrulesim(sim,netforfingov) EQ 3,
    NFFG.LO(t) = -INF;
    NFFG.UP(t) = +INF;
    GOVRECGDP.LO(netforfingov,t) = -INF;
    GOVRECGDP.UP(netforfingov,t) = +INF;
    GOVRECABS.FX(netforfingov,t)$(NOT govrecabssim(sim,netforfingov,t)) = GOVRECABS0(netforfingov,t);
    GOVRECABS.FX(netforfingov,t)$govrecabssim(sim,netforfingov,t)       = govrecabssim(sim,netforfingov,t)*GOVRECABS00(netforfingov)/SUM(tmin, govrecabssim(sim,netforfingov,tmin));
  );  
);  

* net domestic financing
LOOP(netdomfin,
  IF(govrecrulesim(sim,netdomfin) EQ 1,
    RNDFG.FX(t) = RNDFG0(t);
    GOVRECGDP.LO(netdomfin,t) = -INF;
    GOVRECGDP.UP(netdomfin,t) = +INF;
    GOVRECABS.LO(netdomfin,t) = -INF;
    GOVRECABS.UP(netdomfin,t) = +INF;
  );
  IF(govrecrulesim(sim,netdomfin) EQ 2,
    RNDFG.LO(t) = -INF;
    RNDFG.UP(t) = +INF;
    GOVRECGDP.FX(netdomfin,t)$(NOT govrecgdpsim(sim,netdomfin,t)) = GOVRECGDP0(netdomfin,t);
    GOVRECGDP.FX(netdomfin,t)$govrecgdpsim(sim,netdomfin,t)       = govrecgdpsim(sim,netdomfin,t)*GOVRECGDP00(netdomfin)/SUM(tmin, govrecgdpsim(sim,netdomfin,tmin));
    GOVRECABS.LO(netdomfin,t) = -INF;
    GOVRECABS.UP(netdomfin,t) = +INF;
  );
  IF(govrecrulesim(sim,netdomfin) EQ 3,
    RNDFG.LO(t) = -INF;
    RNDFG.UP(t) = +INF;
    GOVRECGDP.LO(netdomfin,t) = -INF;
    GOVRECGDP.UP(netdomfin,t) = +INF;
    GOVRECABS.FX(netdomfin,t)$(NOT govrecabssim(sim,netdomfin,t)) = GOVRECABS0(netdomfin,t);
    GOVRECABS.FX(netdomfin,t)$govrecabssim(sim,netdomfin,t)       = govrecabssim(sim,netdomfin,t)*GOVRECABS00(netdomfin)/SUM(tmin, govrecabssim(sim,netdomfin,tmin));

  );
);  






* GOVSPNDRULE =======================================================

* transfers from gov to non-gov
LOOP(trngovgov,
  IF(govspndrulesim(sim,trngovgov) EQ 1,
    TRNSFRSCAL.FX('trngovgov',t) = TRNSFRSCAL0('trngovgov',t);

    trnsfrb(insdng,insgov,t)$(NOT govspndgrwsim(sim,trngovgov,t)) = trnsfrb0(insdng,insgov,t);
    trnsfrb(insdng,insgov,t)$govspndgrwsim(sim,trngovgov,t)       = trnsfrb00(insdng,insgov)*govspndindexsim(sim,trngovgov,t);
    
    GOVSPNDGDP.LO(trngovgov,t) = -INF;
    GOVSPNDGDP.UP(trngovgov,t) = +INF;
    GOVSPNDABS.LO(trngovgov,t) = -INF;
    GOVSPNDABS.UP(trngovgov,t) = +INF;
  );
  IF(govspndrulesim(sim,trngovgov) EQ 2,
    TRNSFRSCAL.LO('trngovgov',t) = -INF;
    TRNSFRSCAL.UP('trngovgov',t) = +INF;
    GOVSPNDGDP.FX(trngovgov,t)$(NOT govspndgdpsim(sim,trngovgov,t)) = GOVSPNDGDP0(trngovgov,t);
    GOVSPNDGDP.FX(trngovgov,t)$govspndgdpsim(sim,trngovgov,t)       = govspndgdpsim(sim,trngovgov,t)*GOVSPNDGDP00(trngovgov)/SUM(tmin, govspndgdpsim(sim,trngovgov,tmin));
    GOVSPNDABS.LO(trngovgov,t) = -INF;
    GOVSPNDABS.UP(trngovgov,t) = +INF;
  );
  
  IF(govspndrulesim(sim,trngovgov) EQ 3,
    TRNSFRSCAL.LO('trngovgov',t) = -INF;
    TRNSFRSCAL.UP('trngovgov',t) = +INF;
    GOVSPNDGDP.LO(trngovgov,t) = -INF;
    GOVSPNDGDP.UP(trngovgov,t) = +INF;
    GOVSPNDABS.FX(trngovgov,t)$(NOT govspndabssim(sim,trngovgov,t)) = GOVSPNDABS0(trngovgov,t);
    GOVSPNDABS.FX(trngovgov,t)$govspndabssim(sim,trngovgov,t)       = govspndabssim(sim,trngovgov,t)*GOVSPNDABS00(trngovgov)/SUM(tmin, govspndabssim(sim,trngovgov,tmin));

  );
  
);  

* transfers from gov to row
LOOP(trrowgov,
  IF(govspndrulesim(sim,trrowgov) EQ 1,
    TRNSFRSCAL.FX('trrowgov',t) = TRNSFRSCAL0('trrowgov',t);

    trnsfrb(insrow,insgov,t)$(NOT govspndgrwsim(sim,trrowgov,t)) = trnsfrb0(insrow,insgov,t);
    trnsfrb(insrow,insgov,t)$govspndgrwsim(sim,trrowgov,t)       = trnsfrb00(insrow,insgov)*govspndindexsim(sim,trrowgov,t);
    
    GOVSPNDGDP.LO(trrowgov,t) = -INF;
    GOVSPNDGDP.UP(trrowgov,t) = +INF;
    GOVSPNDABS.LO(trrowgov,t) = -INF;
    GOVSPNDABS.UP(trrowgov,t) = +INF;    
  );
  IF(govspndrulesim(sim,trrowgov) EQ 2,
    TRNSFRSCAL.LO('trrowgov',t) = -INF;
    TRNSFRSCAL.UP('trrowgov',t) = +INF;
    GOVSPNDGDP.FX(trrowgov,t)$(NOT govspndgdpsim(sim,trrowgov,t)) = GOVSPNDGDP0(trrowgov,t);
    GOVSPNDGDP.FX(trrowgov,t)$govspndgdpsim(sim,trrowgov,t)       = govspndgdpsim(sim,trrowgov,t)*GOVSPNDGDP00(trrowgov)/SUM(tmin, govspndgdpsim(sim,trrowgov,tmin));
    GOVSPNDABS.LO(trrowgov,t) = -INF;
    GOVSPNDABS.UP(trrowgov,t) = +INF;
  );

  IF(govspndrulesim(sim,trrowgov) EQ 3,
    TRNSFRSCAL.LO('trrowgov',t) = -INF;
    TRNSFRSCAL.UP('trrowgov',t) = +INF;
    GOVSPNDGDP.LO(trrowgov,t) = -INF;
    GOVSPNDGDP.UP(trrowgov,t) = +INF;    
    GOVSPNDABS.FX(trrowgov,t)$(NOT govspndabssim(sim,trrowgov,t)) = GOVSPNDABS0(trrowgov,t);
    GOVSPNDABS.FX(trrowgov,t)$govspndabssim(sim,trrowgov,t)       = govspndabssim(sim,trrowgov,t)*GOVSPNDABS00(trrowgov)/SUM(tmin, govspndabssim(sim,trrowgov,tmin));
  );

);  

* government investment
LOOP(fcapgz,
  IF(govspndrulesim(sim,fcapgz) EQ 1,
    ISCAL.FX(fcapgz,t) = ISCAL0(fcapgz,t);
*    qinvdestb(fcapgz,t)$(NOT govspndgrwsim(sim,fcapgz,t)) = qinvdestb0(fcapgz,t);
*    qinvdestb(fcapgz,t)$govspndgrwsim(sim,fcapgz,t)       = qinvdestb00(fcapgz)*govspndindexsim(sim,fcapgz,t);
    GOVSPNDGDP.LO(fcapgz,t) = -INF;
    GOVSPNDGDP.UP(fcapgz,t) = +INF;
    GOVSPNDABS.LO(fcapgz,t) = -INF;
    GOVSPNDABS.UP(fcapgz,t) = +INF;    
  );
  IF (govspndrulesim(sim,fcapgz) EQ 2,
    ISCAL.LO(fcapgz,t) = -INF;
    ISCAL.UP(fcapgz,t) = +INF;
    GOVSPNDGDP.FX(fcapgz,t)$(NOT govspndgdpsim(sim,fcapgz,t)) = GOVSPNDGDP0(fcapgz,t);
    GOVSPNDGDP.FX(fcapgz,t)$govspndgdpsim(sim,fcapgz,t)       = govspndgdpsim(sim,fcapgz,t)*GOVSPNDGDP00(fcapgz)/SUM(tmin, govspndgdpsim(sim,fcapgz,tmin));
    GOVSPNDABS.LO(fcapgz,t) = -INF;
    GOVSPNDABS.UP(fcapgz,t) = +INF;    
  );

  IF (govspndrulesim(sim,fcapgz) EQ 3,
    ISCAL.LO(fcapgz,t) = -INF;
    ISCAL.UP(fcapgz,t) = +INF;
    GOVSPNDGDP.LO(fcapgz,t) = -INF;
    GOVSPNDGDP.UP(fcapgz,t) = +INF;    
    GOVSPNDABS.FX(fcapgz,t)$(NOT govspndabssim(sim,fcapgz,t)) = GOVSPNDABS0(fcapgz,t);
    GOVSPNDABS.FX(fcapgz,t)$govspndabssim(sim,fcapgz,t)       = govspndabssim(sim,fcapgz,t)*GOVSPNDABS00(fcapgz)/SUM(tmin, govspndabssim(sim,fcapgz,tmin));
  );

);

* gov consumption
LOOP(congov,
  IF(govspndrulesim(sim,congov) EQ 1,
    QGSCAL.FX(t) = QGSCAL0(t);
    qgb(c,t)$(NOT govspndgrwsim(sim,congov,t)) = qgb0(c,t);
    qgb(c,t)$govspndgrwsim(sim,congov,t)       = qgb00(c)*govspndindexsim(sim,congov,t);
    GOVSPNDGDP.LO(congov,t) = -INF;
    GOVSPNDGDP.UP(congov,t) = +INF;
    GOVSPNDABS.LO(congov,t) = -INF;
    GOVSPNDABS.UP(congov,t) = +INF;    
  );
  IF (govspndrulesim(sim,congov) EQ 2,
    QGSCAL.LO(t) = -INF;
    QGSCAL.UP(t) = +INF;
    GOVSPNDGDP.FX(congov,t)$(NOT govspndgdpsim(sim,congov,t)) = GOVSPNDGDP0(congov,t);
    GOVSPNDGDP.FX(congov,t)$govspndgdpsim(sim,congov,t)       = govspndgdpsim(sim,congov,t)*GOVSPNDGDP00(congov)/SUM(tmin, govspndgdpsim(sim,congov,tmin));
    GOVSPNDABS.LO(congov,t) = -INF;
    GOVSPNDABS.UP(congov,t) = +INF;   
  );

  IF (govspndrulesim(sim,congov) EQ 3,
    ISCAL.LO(fcapgz,t) = -INF;
    ISCAL.UP(fcapgz,t) = +INF;
    GOVSPNDGDP.LO(congov,t) = -INF;
    GOVSPNDGDP.UP(congov,t) = +INF;    
    GOVSPNDABS.FX(congov,t)$(NOT govspndabssim(sim,congov,t)) = GOVSPNDABS0(congov,t);
    GOVSPNDABS.FX(congov,t)$govspndabssim(sim,congov,t)       = govspndabssim(sim,congov,t)*GOVSPNDABS00(congov)/SUM(tmin, govspndabssim(sim,congov,tmin));
  );

);  




* subsidy rate for commodity c
SUBCSCAL.FX(t) = 1;
LOOP(subcom,
  IF(govspndrulesim(sim,subcom) EQ 1,
    SUBCSCAL.FX(t) = SUBCSCAL0(t);  
    subcb(c,d,t) = subcb0(c,d,t);
    GOVSPNDGDP.LO(subcom,t) = -INF;
    GOVSPNDGDP.UP(subcom,t) = +INF;
    GOVSPNDABS.LO(subcom,t) = -INF;
    GOVSPNDABS.UP(subcom,t) = +INF;
  );
  IF(govspndrulesim(sim,subcom) EQ 2,
    SUBCSCAL.LO(t) = -INF;
    SUBCSCAL.UP(t) = +INF;
    GOVSPNDGDP.FX(subcom,t)$(NOT govspndgdpsim(sim,subcom,t)) = GOVSPNDGDP0(subcom,t);
    GOVSPNDGDP.FX(subcom,t)$(govspndgdpsim(sim,subcom,t))     = govspndgdpsim(sim,subcom,t)*GOVSPNDGDP00(subcom)/SUM(tmin, govspndgdpsim(sim,subcom,tmin));
    GOVSPNDABS.LO(subcom,t) = -INF;
    GOVSPNDABS.UP(subcom,t) = +INF;
  );
  IF(govspndrulesim(sim,subcom) EQ 3,
    SUBCSCAL.LO(t) = -INF;
    SUBCSCAL.UP(t) = +INF;
    GOVSPNDGDP.LO(subcom,t) = -INF;
    GOVSPNDGDP.UP(subcom,t) = +INF;
    GOVSPNDABS.FX(subcom,t)$(NOT govspndabssim(sim,subcom,t)) = GOVSPNDABS0(subcom,t);
    GOVSPNDABS.FX(subcom,t)$(govspndabssim(sim,subcom,t))     = govspndabssim(sim,subcom,t)*GOVSPNDABS00(subcom)/SUM(tmin, govspndabssim(sim,subcom,tmin));
  );
  
);  




* NGOVPAYRULE =======================================================

* transfers from row to non-gov
LOOP(trngovrow,
  IF(ngovpayrulesim(sim,trngovrow) EQ 1,
    TRNSFRSCAL.FX('trngovrow',t) = TRNSFRSCAL0('trngovrow',t);

    trnsfrb(insdng,insrow,t)$(NOT ngovpaygrwsim(sim,trngovrow,t)) = trnsfrb0(insdng,insrow,t);
    trnsfrb(insdng,insrow,t)$ngovpaygrwsim(sim,trngovrow,t)       = trnsfrb00(insdng,insrow)*ngovpayindexsim(sim,trngovrow,t);
    
    NGOVPAYGDP.LO(trngovrow,t) = -INF;
    NGOVPAYGDP.UP(trngovrow,t) = +INF;
    NGOVPAYABS.LO(trngovrow,t) = -INF;
    NGOVPAYABS.UP(trngovrow,t) = +INF;    
  );
  IF(ngovpayrulesim(sim,trngovrow) EQ 2,
    TRNSFRSCAL.LO('trngovrow',t) = -INF;
    TRNSFRSCAL.UP('trngovrow',t) = +INF;
    NGOVPAYGDP.FX(trngovrow,t)$(NOT ngovpaygdpsim(sim,trngovrow,t)) = NGOVPAYGDP0(trngovrow,t);
    NGOVPAYGDP.FX(trngovrow,t)$ngovpaygdpsim(sim,trngovrow,t)       = ngovpaygdpsim(sim,trngovrow,t)*NGOVPAYGDP00(trngovrow)/SUM(tmin, ngovpaygdpsim(sim,trngovrow,tmin));
    NGOVPAYABS.LO(trngovrow,t) = -INF;
    NGOVPAYABS.UP(trngovrow,t) = +INF;    
  );
  
  IF(ngovpayrulesim(sim,trngovrow) EQ 3,
    TRNSFRSCAL.LO('trngovrow',t) = -INF;
    TRNSFRSCAL.UP('trngovrow',t) = +INF;
    NGOVPAYGDP.LO(trngovrow,t) = -INF;
    NGOVPAYGDP.UP(trngovrow,t) = +INF;    
    NGOVPAYABS.FX(trngovrow,t)$(NOT ngovpayabssim(sim,trngovrow,t)) = NGOVPAYABS0(trngovrow,t);
    NGOVPAYABS.FX(trngovrow,t)$ngovpayabssim(sim,trngovrow,t)       = ngovpayabssim(sim,trngovrow,t)*NGOVPAYABS00(trngovrow)/SUM(tmin, ngovpayabssim(sim,trngovrow,tmin));
  );  
);  


* transfers from row to factors
LOOP(trfacrow,
  IF(ngovpayrulesim(sim,trfacrow) EQ 1,
    TRNSFRSCAL.FX('trfacrow',t) = TRNSFRSCAL0('trfacrow',t);

    trnsfrb(f,insrow,t)$(NOT ngovpaygrwsim(sim,trfacrow,t)) = trnsfrb0(f,insrow,t);
    trnsfrb(f,insrow,t)$ngovpaygrwsim(sim,trfacrow,t)       = trnsfrb00(f,insrow)*ngovpayindexsim(sim,trfacrow,t);
    
    NGOVPAYGDP.LO(trfacrow,t) = -INF;
    NGOVPAYGDP.UP(trfacrow,t) = +INF;
    NGOVPAYABS.LO(trfacrow,t) = -INF;
    NGOVPAYABS.UP(trfacrow,t) = +INF;    
  );
  IF(ngovpayrulesim(sim,trfacrow) EQ 2,
    TRNSFRSCAL.LO('trfacrow',t) = -INF;
    TRNSFRSCAL.UP('trfacrow',t) = +INF;
    NGOVPAYGDP.FX(trfacrow,t)$(NOT ngovpaygdpsim(sim,trfacrow,t)) = NGOVPAYGDP0(trfacrow,t);
    NGOVPAYGDP.FX(trfacrow,t)$ngovpaygdpsim(sim,trfacrow,t)       = ngovpaygdpsim(sim,trfacrow,t)*NGOVPAYGDP00(trfacrow)/SUM(tmin, ngovpaygdpsim(sim,trfacrow,tmin));
    NGOVPAYABS.LO(trfacrow,t) = -INF;
    NGOVPAYABS.UP(trfacrow,t) = +INF;
  );
  
  IF(ngovpayrulesim(sim,trfacrow) EQ 3,
    TRNSFRSCAL.LO('trfacrow',t) = -INF;
    TRNSFRSCAL.UP('trfacrow',t) = +INF;
    NGOVPAYGDP.LO(trfacrow,t) = -INF;
    NGOVPAYGDP.UP(trfacrow,t) = +INF;
    NGOVPAYABS.FX(trfacrow,t)$(NOT ngovpayabssim(sim,trfacrow,t)) = NGOVPAYABS0(trfacrow,t);
    NGOVPAYABS.FX(trfacrow,t)$ngovpayabssim(sim,trfacrow,t)       = ngovpayabssim(sim,trfacrow,t)*NGOVPAYABS00(trfacrow)/SUM(tmin, ngovpayabssim(sim,trfacrow,tmin));

  );
  
);  

* transfers from factors to row
LOOP(trrowfac,
  IF(ngovpayrulesim(sim,trrowfac) EQ 1,
    TRNSFRSCAL.FX('trrowfac',t) = TRNSFRSCAL0('trrowfac',t);

    trnsfrb(insrow,f,t)$(NOT ngovpaygrwsim(sim,trrowfac,t)) = trnsfrb0(insrow,f,t);
    trnsfrb(insrow,f,t)$ngovpaygrwsim(sim,trrowfac,t)       = trnsfrb00(insrow,f)*ngovpayindexsim(sim,trrowfac,t);
    
    NGOVPAYGDP.LO(trrowfac,t) = -INF;
    NGOVPAYGDP.UP(trrowfac,t) = +INF;
    NGOVPAYABS.LO(trrowfac,t) = -INF;
    NGOVPAYABS.UP(trrowfac,t) = +INF;    
  );
  IF(ngovpayrulesim(sim,trrowfac) EQ 2,
    TRNSFRSCAL.LO('trrowfac',t) = -INF;
    TRNSFRSCAL.UP('trrowfac',t) = +INF;
    NGOVPAYGDP.FX(trrowfac,t)$(NOT ngovpaygdpsim(sim,trrowfac,t)) = NGOVPAYGDP0(trrowfac,t);
    NGOVPAYGDP.FX(trrowfac,t)$ngovpaygdpsim(sim,trrowfac,t)       = ngovpaygdpsim(sim,trrowfac,t)*NGOVPAYGDP00(trrowfac)/SUM(tmin, ngovpaygdpsim(sim,trrowfac,tmin));
    NGOVPAYABS.LO(trrowfac,t) = -INF;
    NGOVPAYABS.UP(trrowfac,t) = +INF;    
  );
  
  IF(ngovpayrulesim(sim,trrowfac) EQ 3,
    TRNSFRSCAL.LO('trrowfac',t) = -INF;
    TRNSFRSCAL.UP('trrowfac',t) = +INF;
    NGOVPAYGDP.LO(trrowfac,t) = -INF;
    NGOVPAYGDP.UP(trrowfac,t) = +INF;
    NGOVPAYABS.FX(trrowfac,t)$(NOT ngovpayabssim(sim,trrowfac,t)) = NGOVPAYABS0(trrowfac,t);
    NGOVPAYABS.FX(trrowfac,t)$ngovpayabssim(sim,trrowfac,t)       = ngovpayabssim(sim,trrowfac,t)*NGOVPAYABS00(trrowfac)/SUM(tmin, ngovpayabssim(sim,trrowfac,tmin));

  );
  
);  



*!! not yet available
*trrowfac

*!! not yet available
*trrowngov

*!! to think: move?
MPSADJ.FX(t) = MPSADJ0(t);

$ONTEXT
* non-gov savings
LOOP(savngov,
  IF(ngovpayrulesim(sim,savngov) EQ 1,
    MPSSCAL.FX(t) = MPSSCAL0(t);
* not yeat  
*    mpsb(insdng,t)$(NOT ngovpaygrwsim(sim,savngov,t)) = mpsb0(insdng,t);
*    mpsb(insdng,t)$ngovpaygrwsim(sim,savngov,t)       = mpsb00(insdng)*mpsbarsim(sim,indsng,t)/SUM(tmin, mpsbarsim(sim,indsng,tmin));

    NGOVPAYGDP.LO(savngov,t) = -INF;
    NGOVPAYGDP.UP(savngov,t) = +INF;
  );
  IF(ngovpayrulesim(sim,savngov) EQ 2,
    MPSSCAL.LO(t) = -INF;
    MPSSCAL.UP(t) = +INF;
    NGOVPAYGDP.FX(savngov,t)$(NOT ngovpaygdpsim(sim,savngov,t)) = NGOVPAYGDP0(savngov,t);
    NGOVPAYGDP.FX(savngov,t)$ngovpaygdpsim(sim,savngov,t)       = ngovpaygdpsim(sim,savngov,t)*NGOVPAYGDP00(savngov)/SUM(tmin, ngovpaygdpsim(sim,savngov,tmin));
    
  );
);  
$OFFTEXT


* non-gov net foreign financing
LOOP(netforfinngov,
  IF(ngovpayrulesim(sim,netforfinngov) EQ 1,
    NFFINSSCAL.FX(t) = NFFINSSCAL0(t);
    NGOVPAYGDP.LO(netforfinngov,t) = -INF;
    NGOVPAYGDP.UP(netforfinngov,t) = +INF;
    NGOVPAYABS.LO(netforfinngov,t) = -INF;
    NGOVPAYABS.UP(netforfinngov,t) = +INF;    
  );
  IF(ngovpayrulesim(sim,netforfinngov) EQ 2,
    NFFINSSCAL.LO(t) = -INF;
    NFFINSSCAL.UP(t) = +INF;
    NGOVPAYGDP.FX(netforfinngov,t)$(NOT ngovpaygdpsim(sim,netforfinngov,t)) = NGOVPAYGDP0(netforfinngov,t);
    NGOVPAYGDP.FX(netforfinngov,t)$ngovpaygdpsim(sim,netforfinngov,t)       = ngovpaygdpsim(sim,netforfinngov,t)*NGOVPAYGDP00(netforfinngov)/SUM(tmin, ngovpaygdpsim(sim,netforfinngov,tmin));
    NGOVPAYABS.LO(netforfinngov,t) = -INF;
    NGOVPAYABS.UP(netforfinngov,t) = +INF; 
  );
  
  IF(ngovpayrulesim(sim,netforfinngov) EQ 3,
    NFFINSSCAL.LO(t) = -INF;
    NFFINSSCAL.UP(t) = +INF;
    NGOVPAYGDP.LO(netforfinngov,t) = -INF;
    NGOVPAYGDP.UP(netforfinngov,t) = +INF;    
    NGOVPAYABS.FX(netforfinngov,t)$(NOT ngovpayabssim(sim,netforfinngov,t)) = NGOVPAYABS0(netforfinngov,t);
    NGOVPAYABS.FX(netforfinngov,t)$ngovpayabssim(sim,netforfinngov,t)       = ngovpayabssim(sim,netforfinngov,t)*NGOVPAYABS00(netforfinngov)/SUM(tmin, ngovpayabssim(sim,netforfinngov,tmin));
  );  
  
);  



* non-government investment
LOOP(fcapngz,
  IF(ngovpayrulesim(sim,fcapngz) EQ 1,
    ISCAL.FX(fcapngz,t) = ISCAL0(fcapngz,t);
    NGOVPAYGDP.LO(fcapngz,t) = -INF;
    NGOVPAYGDP.UP(fcapngz,t) = +INF;
    NGOVPAYABS.LO(fcapngz,t) = -INF;
    NGOVPAYABS.UP(fcapngz,t) = +INF;
  );
  IF(ngovpayrulesim(sim,fcapngz) EQ 2,
    ISCAL.LO(fcapngz,t) = -INF;
    ISCAL.UP(fcapngz,t) = +INF;
    NGOVPAYGDP.FX(fcapngz,t)$(NOT ngovpaygdpsim(sim,fcapngz,t)) = NGOVPAYGDP0(fcapngz,t);
    NGOVPAYGDP.FX(fcapngz,t)$ngovpaygdpsim(sim,fcapngz,t)       = ngovpaygdpsim(sim,fcapngz,t)*NGOVPAYGDP00(fcapngz)/SUM(tmin, ngovpaygdpsim(sim,fcapngz,tmin));
    NGOVPAYABS.LO(fcapngz,t) = -INF;
    NGOVPAYABS.UP(fcapngz,t) = +INF;
  );
  
  IF(ngovpayrulesim(sim,fcapngz) EQ 3,
    ISCAL.LO(fcapngz,t) = -INF;
    ISCAL.UP(fcapngz,t) = +INF;
    NGOVPAYGDP.LO(fcapngz,t) = -INF;
    NGOVPAYGDP.UP(fcapngz,t) = +INF;
    NGOVPAYABS.FX(fcapngz,t)$(NOT ngovpayabssim(sim,fcapngz,t)) = NGOVPAYABS0(fcapngz,t);
    NGOVPAYABS.FX(fcapngz,t)$ngovpayabssim(sim,fcapngz,t)       = ngovpayabssim(sim,fcapngz,t)*NGOVPAYABS00(fcapngz)/SUM(tmin, ngovpayabssim(sim,fcapngz,tmin));
  );
  
);  

* foreign direct investment
LOOP(fdi,
  IF(ngovpayrulesim(sim,fdi) EQ 1,
    FDISCAL.FX(t) = FDISCAL0(t);
    NGOVPAYGDP.LO(fdi,t) = -INF;
    NGOVPAYGDP.UP(fdi,t) = +INF;
    NGOVPAYABS.LO(fdi,t) = -INF;
    NGOVPAYABS.UP(fdi,t) = +INF;    
  );
  IF(ngovpayrulesim(sim,fdi) EQ 2,
    FDISCAL.LO(t) = -INF;
    FDISCAL.UP(t) = +INF;
    NGOVPAYGDP.FX(fdi,t)$(NOT ngovpaygdpsim(sim,fdi,t)) = NGOVPAYGDP0(fdi,t);
    NGOVPAYGDP.FX(fdi,t)$ngovpaygdpsim(sim,fdi,t)       = ngovpaygdpsim(sim,fdi,t)*NGOVPAYGDP00(fdi)/SUM(tmin, ngovpaygdpsim(sim,fdi,tmin));
    NGOVPAYABS.LO(fdi,t) = -INF;
    NGOVPAYABS.UP(fdi,t) = +INF; 
  );
  
  IF(ngovpayrulesim(sim,fdi) EQ 3,
    FDISCAL.LO(t) = -INF;
    FDISCAL.UP(t) = +INF;
    NGOVPAYGDP.LO(fdi,t) = -INF;
    NGOVPAYGDP.UP(fdi,t) = +INF;
    NGOVPAYABS.FX(fdi,t)$(NOT ngovpayabssim(sim,fdi,t)) = NGOVPAYABS0(fdi,t);
    NGOVPAYABS.FX(fdi,t)$ngovpayabssim(sim,fdi,t)       = ngovpayabssim(sim,fdi,t)*NGOVPAYABS00(fdi)/SUM(tmin, ngovpayabssim(sim,fdi,tmin));

  );  
);     


* foreign tourist spending
LOOP(tourismrec,
  IF(ngovpayrulesim(sim,tourismrec) EQ 1,
    TRSMREC.FX(t)$(NOT ngovpaygrwsim(sim,tourismrec,t)) = TRSMREC0(t);
    TRSMREC.FX(t)$ngovpaygrwsim(sim,tourismrec,t) = TRSMREC00*ngovpayindexsim(sim,tourismrec,t);
    
    NGOVPAYGDP.LO(tourismrec,t) = -INF;
    NGOVPAYGDP.UP(tourismrec,t) = +INF;
    NGOVPAYABS.LO(tourismrec,t) = -INF;
    NGOVPAYABS.UP(tourismrec,t) = +INF;

  );
  IF(ngovpayrulesim(sim,tourismrec) EQ 2,
    TRSMREC.LO(t) = -INF;
    TRSMREC.UP(t) = +INF;

    NGOVPAYGDP.FX(tourismrec,t)$(NOT ngovpaygdpsim(sim,tourismrec,t)) = NGOVPAYGDP0(tourismrec,t);
    NGOVPAYGDP.FX(tourismrec,t)$ngovpaygdpsim(sim,tourismrec,t)       = ngovpaygdpsim(sim,tourismrec,t)*NGOVPAYGDP00(tourismrec)/SUM(tmin, ngovpaygdpsim(sim,tourismrec,tmin));
    NGOVPAYABS.LO(tourismrec,t) = -INF;
    NGOVPAYABS.UP(tourismrec,t) = +INF;
  );

  IF(ngovpayrulesim(sim,tourismrec) EQ 3,
    TRSMREC.LO(t) = -INF;
    TRSMREC.UP(t) = +INF;
    NGOVPAYGDP.LO(tourismrec,t) = -INF;
    NGOVPAYGDP.UP(tourismrec,t) = +INF;

    NGOVPAYABS.FX(tourismrec,t)$(NOT ngovpayabssim(sim,tourismrec,t)) = NGOVPAYABS0(tourismrec,t);
    NGOVPAYABS.FX(tourismrec,t)$ngovpayabssim(sim,tourismrec,t)       = ngovpayabssim(sim,tourismrec,t)*NGOVPAYABS00(tourismrec)/SUM(tmin, ngovpayabssim(sim,tourismrec,tmin));
  );

);     

    
* Numeraire =========================================================

IF(numerairesim(sim) EQ 1,
  CPI.FX(t) = CPI0(t);

  DPI.LO(t) = -INF;
  DPI.UP(t) = +INF;
  DPI.L(t)  = DPI0(t);
  
  EXR.LO(t) = -INF;
  EXR.UP(t) = +INF;
  EXR.L(t)  = DPI0(t);
);

IF(numerairesim(sim) EQ 2,
  DPI.FX(t) = DPI0(t);

  CPI.LO(t) = -INF;
  CPI.UP(t) = +INF;
  CPI.L(t)  = CPI0(t);

  EXR.LO(t) = -INF;
  EXR.UP(t) = +INF;
  EXR.L(t)  = EXR0(t);
);

IF(numerairesim(sim) EQ 3,
  CPI.LO(t) = -INF;
  CPI.UP(t) = +INF;
  CPI.L(t)  = CPI0(t);
  
  DPI.LO(t) = -INF;
  DPI.UP(t) = +INF;
  DPI.L(t)  = DPI0(t);

  EXR.FX(t) = EXR0(t);  
);


* Government ========================================================

* in all cases, fix IADJ for gov; it will be flexed depending on govclos
IADJ.FX('govz',t) = IADJ0('govz',t);

* in all cases except when dcal01 = 1, fix TFPSCAL; it will be flexed depending on govclos
* note: this might need adjustment in case TFSCAL is not used to target gdpgrw
TFPSCAL.FX(t)$(NOT dcal01) = TFPSCAL0(t);

* 1 = direct tax collection clears the government budget
IF(govclossim(sim,t) EQ 1,
* flexed direct tax rate
  TYSCAL.LO(t) = -INF;  
  TYSCAL.UP(t) = +INF;  
  GOVRECGDP.LO(taxdir,t) = -INF;
  GOVRECGDP.UP(taxdir,t) = +INF;  
  GOVRECABS.LO(taxdir,t) = -INF;
  GOVRECABS.UP(taxdir,t) = +INF;    
);

* 2 = net domestic financing clears the government budget
IF(govclossim(sim,t) EQ 2,
* flexed net domestic financing
  RNDFG.LO(t) = -INF;
  RNDFG.UP(t) = +INF;
  
  GOVRECGDP.LO(netdomfin,t) = -INF;
  GOVRECGDP.UP(netdomfin,t) = +INF;   

  GOVRECABS.LO(netdomfin,t) = -INF;
  GOVRECABS.UP(netdomfin,t) = +INF;  
);

* 3 = net foreign financing clears the government budget
IF(govclossim(sim,t) EQ 3,
* flexed net foreign financing
  NFFG.LO(t) = -INF;  
  NFFG.UP(t) = +INF;  
  GOVRECGDP.LO(netforfingov,t) = -INF;
  GOVRECGDP.UP(netforfingov,t) = +INF;    
  GOVRECABS.LO(netforfingov,t) = -INF;
  GOVRECABS.UP(netforfingov,t) = +INF;      
);

* 4 = government consumption clears the government budget
IF(govclossim(sim,t) EQ 4,
  QGSCAL.LO(t) = -INF;
  QGSCAL.UP(t) = +INF;
  GOVSPNDGDP.LO(congov,t) = -INF;
  GOVSPNDGDP.UP(congov,t) = +INF;
  GOVSPNDABS.LO(congov,t) = -INF;
  GOVSPNDABS.UP(congov,t) = +INF;    
);

* 5 = government investment clears the government budget
IF(govclossim(sim,t) EQ 5,
* note: corresponding govspndrulesim must be eq 1
* note: need only one endogenous variable; thus, added IADJ(govz)
  IADJ.LO('govz',t) = -INF;
  IADJ.UP('govz',t) = +INF;
  GOVSPNDGDP.LO(fcapgz,t) = -INF;
  GOVSPNDGDP.UP(fcapgz,t) = +INF;
  GOVSPNDABS.LO(fcapgz,t) = -INF;
  GOVSPNDABS.UP(fcapgz,t) = +INF;    
);

$ONTEXT
* 6 = indirect commodity subsidies clear the government budget
IF(govclossim(sim,t) EQ 6,
* flexed indirect subsidy rates
  SUBCADJ.LO(t) = -INF;  
  SUBCADJ.UP(t) = +INF;  
  GOVSPNDGDP.LO(subcom,t) = -INF;
  GOVSPNDGDP.UP(subcom,t) = +INF;  
  GOVSPNDABS.LO(subcom,t) = -INF;
  GOVSPNDABS.UP(subcom,t) = +INF;    
);
$OFFTEXT

* 7 = activity tax clears the government budget
IF(govclossim(sim,t) EQ 7,
* flex activity tax rate 
  TASCAL.LO(t) = -INF;
  TASCAL.UP(t) = +INF;
  GOVRECGDP.LO(taxact,t) = -INF;
  GOVRECGDP.UP(taxact,t) = +INF;
  GOVRECABS.LO(taxact,t) = -INF;
  GOVRECABS.UP(taxact,t) = +INF;  
);


* 8 = TFP for selected (government?) activities clears the government budget
IF(govclossim(sim,t) EQ 8,
* flex commodity subsidies
  TFPSCAL.LO(t) = -INF;
  TFPSCAL.UP(t) = +INF;

);

* 9 = sales tax clears the government budget
IF(govclossim(sim,t) EQ 9,
* flex sales tax rate 
  TQSCAL.LO(t) = -INF;
  TQSCAL.UP(t) = +INF;

);


* 10 = direct tax on factors clears the government budget
IF(govclossim(sim,t) EQ 10,
* flex sales tax rate 
  TFSCAL.LO(t) = -INF;
  TFSCAL.UP(t) = +INF;

);

* 11 = foreign transfers clear the government budget
IF(govclossim(sim,t) EQ 11,
  TRNSFRSCAL.LO('trgovrow',t) = -INF;
  TRNSFRSCAL.UP('trgovrow',t) = +INF;
  GOVRECGDP.LO('trgovrow',t) = -INF;
  GOVRECGDP.UP('trgovrow',t) = +INF;
  GOVRECABS.LO('trgovrow',t) = -INF;
  GOVRECABS.UP('trgovrow',t) = +INF;    
);
*

* Rest of World =====================================================


  
IF(rowclossim(sim,t) EQ 1,
* flexed real exchange rate
  REXR.LO(t) = -INF; 
  REXR.UP(t) = +INF;

* fixed exchange rate premium  
  PREXR.FX(t) = PREXR0(t);   
  
);
IF(rowclossim(sim,t) EQ 2,

* fixed real exchange rate
  REXR.FX(t) = REXR0(t); 

* flexed non-gov net foreign financing
  NFFINSSCAL.LO(t) = -INF;
  NFFINSSCAL.UP(t) = +INF;
  NGOVPAYGDP.LO(netforfinngov,t) = -INF;
  NGOVPAYGDP.UP(netforfinngov,t) = +INF;
  NGOVPAYABS.LO(netforfinngov,t) = -INF;
  NGOVPAYABS.UP(netforfinngov,t) = +INF;   

* fixed exchange rate premium  
  PREXR.FX(t) = PREXR0(t);     
);

IF(rowclossim(sim,t) EQ 3,

* fixed real exchange rate
  REXR.FX(t) = REXR0(t); 

* flexed gov net foreign financing
  NFFG.LO(t) = -INF;
  NFFG.UP(t) = +INF;
  GOVRECGDP.LO(netforfingov,t) = -INF;
  GOVRECGDP.UP(netforfingov,t) = +INF;
  GOVRECABS.LO(netforfingov,t) = -INF;
  GOVRECABS.UP(netforfingov,t) = +INF;   

* fixed exchange rate premium  
  PREXR.FX(t) = PREXR0(t);     
);
    
IF(rowclossim(sim,t) EQ 4,

* fixed real exchange rate
  REXR.FX(t) = REXR0(t); 

* flexed gov net foreign financing
  PREXR.LO(t) = -INF;
  PREXR.UP(t) = +INF;

);


* Savings-Investment ================================================

* in all cases, fix IADJ for non-gov; it will be flexed depending on siclos
IADJ.FX('ngovz',t) = IADJ0('ngovz',t);
IADJ.FX('rowz',t) = IADJ0('rowz',t);
MPSSCAL.FX(t) = MPSSCAL0(t);


* 1 = investment clears the savings-investment balance
IF(siclossim(sim) EQ 1,
* flexed real invesment
  IADJ.LO('ngovz',t) = -INF; 
  IADJ.UP('ngovz',t) = +INF;
  NGOVPAYGDP.LO(fcapngz,t) = -INF;
  NGOVPAYGDP.UP(fcapngz,t) = +INF;

  NGOVPAYABS.LO(fcapngz,t) = -INF;
  NGOVPAYABS.UP(fcapngz,t) = +INF;
);

* 2 = insdng savings rate clears the savings-investment balance; scaled MPS
IF(siclossim(sim) EQ 2,
* flexed insdng savings rate
  MPSSCAL.LO(t) = -INF;
  MPSSCAL.UP(t) = +INF;

* for insdng that do not consume, MPS has to be fixed at 1  
*  MPSSCAL.FX(insdng,t)$(MPS00(insdng)=1) = MPSSCAL0(insdng,t);
*  IADJ.LO('ngovz',t)$(MPS00(insdng)=1) = -INF; 
*  IADJ.UP('ngovz',t)$(MPS00(insdng)=1) = +INF;  

*?? delete savngov  
  NGOVPAYGDP.LO(savngov,t) = -INF;
  NGOVPAYGDP.UP(savngov,t) = +INF;

  NGOVPAYABS.LO(savngov,t) = -INF;
  NGOVPAYABS.UP(savngov,t) = +INF;
  
);

* 3 = insdng savings rate clears the savings-investment balance; uniform MPS point change
IF(siclossim(sim) EQ 3,
* flexed insdng savings rate
  MPSADJ.LO(t) = -INF;
  MPSADJ.UP(t) = +INF;
  NGOVPAYGDP.LO(savngov,t) = -INF;
  NGOVPAYGDP.UP(savngov,t) = +INF;
  NGOVPAYABS.LO(savngov,t) = -INF;
  NGOVPAYABS.UP(savngov,t) = +INF;
  
);

* 4 = net foreign financings clears the savings-investment balance
IF(siclossim(sim) EQ 4,
* flexed insdng net foreign financing
  NFFINSSCAL.LO(t) = -INF;
  NFFINSSCAL.UP(t) = +INF;
  
  NGOVPAYGDP.LO(netforfinngov,t) = -INF;
  NGOVPAYGDP.UP(netforfinngov,t) = +INF;
  NGOVPAYABS.LO(netforfinngov,t) = -INF;
  NGOVPAYABS.UP(netforfinngov,t) = +INF;  
);



DISPLAY "#### END: macclos.inc";