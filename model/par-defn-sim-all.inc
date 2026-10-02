* par-defn-sim-all.inc


DISPLAY "#### START: par-defn-sim-all.inc";    

qebsim(sim,c2,t)$qebsim(sim,'all',t) = qebsim(sim,'all',t);
qebsim(sim,'all',t)$qebsim(sim,'all',t) = 0;



$EXIT



* tfabsim
tfabsim(sim,f,a,t)$tfabsim(sim,f,'all',t) = tfabsim(sim,f,'all',t);
tfabsim(sim,f,'all',t)$tfabsim(sim,f,'all',t) = 0;


* tfasim
dtfasim(sim,f,a,t)$dtfasim(sim,f,'all',t) = dtfasim(sim,f,'all',t);
dtfasim(sim,f,'all',t)$dtfasim(sim,f,'all',t) = 0;

* mtfpsim
mtfpsim(sim,a,fcap)$mtfpsim(sim,'all',fcap)  = mtfpsim(sim,'all',fcap);  
mtfpsim(sim,'all',fcap) = 0;

* fprdabsim
fprdabsim(sim,fc,a,t)$fprdabsim(sim,fc,'all',t) = fprdabsim(sim,fc,'all',t);
fprdabsim(sim,fc,'all',t) = 0;

* tvacacsim
tvacsim(sim,call,ac,t)$tvacsim(sim,'all',ac,t) = tvacsim(sim,'all',ac,t);
tvacsim(sim,call,a,t)$tvacsim(sim,call,'act',t) = tvacsim(sim,'all',a,t);


* tvacac2sim
tvac2sim(sim,c,ac,t)$tvac2sim(sim,'all',ac,t) = tvac2sim(sim,'all',ac,t);
tvac2sim(sim,c,a,t)$tvac2sim(sim,c,'act',t) = tvac2sim(sim,c,'act',t);

* tstarsim
tstarsim(sim,call,r2,t)$tstarsim(sim,call,'all',t) = tstarsim(sim,call,'all',t);
tstarsim(sim,call,'all',t)$tstarsim(sim,call,'all',t) = 0;

tstarsim(sim,call,r2,t)$tstarsim(sim,'all',r2,t) = tstarsim(sim,'all',r2,t);
tstarsim(sim,'all',r2,t)$tstarsim(sim,'all',r2,t) = 0;

* tstar2sim
tstar2sim(sim,call,r2,t)$tstar2sim(sim,call,'all',t) = tstar2sim(sim,call,'all',t);
tstar2sim(sim,call,'all',t)$tstar2sim(sim,call,'all',t) = 0;

tstar2sim(sim,call,r2,t)$tstar2sim(sim,'all',r2,t) = tstar2sim(sim,'all',r2,t);
tstar2sim(sim,'all',r2,t)$tstar2sim(sim,'all',r2,t) = 0;

* tmsim
tmsim(sim,call,r2,t)$tmsim(sim,call,'all',t) = tmsim(sim,call,'all',t);
tmsim(sim,call,'all',t)$tmsim(sim,call,'all',t) = 0;

tmsim(sim,call,r2,t)$tmsim(sim,'all',r2,t) = tmsim(sim,'all',r2,t);
tmsim(sim,'all',r2,t)$tmsim(sim,'all',r2,t) = 0;

* tm2sim
tm2sim(sim,call,r2,t)$tm2sim(sim,call,'all',t) = tm2sim(sim,call,'all',t);
tm2sim(sim,call,'all',t)$tm2sim(sim,call,'all',t) = 0;

tm2sim(sim,call,r2,t)$tm2sim(sim,'all',r2,t) = tm2sim(sim,'all',r2,t);
tm2sim(sim,'all',r2,t)$tm2sim(sim,'all',r2,t) = 0;

* tesim
tesim(sim,call,r2,t)$tesim(sim,call,'all',t) = tesim(sim,call,'all',t);
tesim(sim,call,'all',t)$tesim(sim,call,'all',t) = 0;

tesim(sim,call,r2,t)$tmsim(sim,'all',r2,t) = tesim(sim,'all',r2,t);
tesim(sim,'all',r2,t)$tesim(sim,'all',r2,t) = 0;


* pwesim
pwesim(sim,call,r2,t)$pwesim(sim,'all',r2,t) = pwesim(sim,'all',r2,t);
pwesim(sim,'all',r2,t)$pwesim(sim,'all',r2,t) = 0;

* pwmsim
pwmsim(sim,call,r2,t)$pwmsim(sim,'all',r2,t) = pwmsim(sim,'all',r2,t);
pwmsim(sim,'all',r2,t)$pwmsim(sim,'all',r2,t) = 0;


* qtrstbsim
qtrstbsim(sim,call,instrst,t)$qtrstbsim(sim,'all',instrst,t) = qtrstbsim(sim,'all',instrst,t);
qtrstbsim(sim,'all',instrst,t)$qtrstbsim(sim,'all',instrst,t) = 0;

* qtrstb2sim
qtrstb2sim(sim,call,instrst,t)$qtrstb2sim(sim,'all',instrst,t) = qtrstb2sim(sim,'all',instrst,t);
qtrstb2sim(sim,'all',instrst,t)$qtrstb2sim(sim,'all',instrst,t) = 0;



* govspndrulesim
govspndrulesim(sim,acgovspndall)$govspndrulesim(sim,'all') = govspndrulesim(sim,'all');
govspndrulesim(sim,'all')$govspndrulesim(sim,'all') = 0;
  
* govspndindexsim  
govspndindexsim(sim,acgovspndall,t)$govspndindexsim(sim,'all',t) = govspndindexsim(sim,'all',t);
govspndindexsim(sim,'all',t)$govspndindexsim(sim,'all',t) = 0;
  
* govspndgdpsim  
govspndgdpsim(sim,acgovspndall,t)$govspndgdpsim(sim,'all',t) = govspndgdpsim(sim,'all',t);
govspndgdpsim(sim,'all',t)$govspndgdpsim(sim,'all',t) = 0;

* govrecrulesim
govrecrulesim(sim,acgovrecall)$govrecrulesim(sim,'all') = govrecrulesim(sim,'all');
govrecrulesim(sim,'all')$govrecrulesim(sim,'all') = 0;
  
* govrecindexsim  
govrecindexsim(sim,acgovrecall,t)$govrecindexsim(sim,'all',t) = govrecindexsim(sim,'all',t);
govrecindexsim(sim,'all',t)$govrecindexsim(sim,'all',t) = 0;
  
* govrecgdpsim  
govrecgdpsim(sim,acgovrecall,t)$govrecgdpsim(sim,'all',t) = govrecgdpsim(sim,'all',t);
govrecgdpsim(sim,'all',t)$govrecgdpsim(sim,'all',t) = 0;


* ngovpayrulesim
ngovpayrulesim(sim,acngovpayall)$ngovpayrulesim(sim,'all') = ngovpayrulesim(sim,'all');
ngovpayrulesim(sim,'all')$ngovpayrulesim(sim,'all') = 0;
  
* ngovpayindexsim  
ngovpayindexsim(sim,acngovpayall,t)$ngovpayindexsim(sim,'all',t) = ngovpayindexsim(sim,'all',t);
ngovpayindexsim(sim,'all',t)$ngovpayindexsim(sim,'all',t) = 0;
  
* ngovpaygdpsim  
ngovpaygdpsim(sim,acngovpayall,t)$ngovpaygdpsim(sim,'all',t) = ngovpaygdpsim(sim,'all',t);
ngovpaygdpsim(sim,'all',t)$ngovpaygdpsim(sim,'all',t) = 0;



DISPLAY "#### END: par-defn-sim-all.inc";    
