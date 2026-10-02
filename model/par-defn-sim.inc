* par-defn-sim.inc

DISPLAY "#### START: par-defn-sim.inc";



* start: impose simulations values for exogenous variables

teb(c,t)$(NOT tebsim(sim,c,t) AND NOT teb2sim(sim,c,t)) = teb0(c,t);
teb(c,t)$tebsim(sim,c,t)                                = teb0(c,t)*tebsim(sim,c,t);
teb(c,t)$teb2sim(sim,c,t)                               = teb2sim(sim,c,t);

tmb(c,t)$(NOT tmbsim(sim,c,t) AND NOT tmb2sim(sim,c,t)) = tmb0(c,t);
tmb(c,t)$tmbsim(sim,c,t)                                = tmb0(c,t)*tmbsim(sim,c,t);
tmb(c,t)$tmb2sim(sim,c,t)                               = tmb2sim(sim,c,t);

tqb(c,t)$(NOT tqbsim(sim,c,t) AND NOT tqb2sim(sim,c,t)) = tqb0(c,t);
tqb(c,t)$tqbsim(sim,c,t)                                = tqb0(c,t)*tqbsim(sim,c,t);
tqb(c,t)$tqb2sim(sim,c,t)                               = tqb2sim(sim,c,t);

tab(a,t)$(NOT tabsim(sim,a,t) AND NOT tab2sim(sim,a,t)) = tab0(a,t);
tab(a,t)$tabsim(sim,a,t)                                = tab0(a,t)*tabsim(sim,a,t);
tab(a,t)$tab2sim(sim,a,t)                               = tab2sim(sim,a,t);

tfab(f,a,t)$(NOT tfabsim(sim,f,a,t) AND NOT tfab2sim(sim,f,a,t)) = tfab0(f,a,t);
tfab(f,a,t)$tfabsim(sim,f,a,t)                                 = tfab0(f,a,t)*tfabsim(sim,f,a,t);
tfab(f,a,t)$tfab2sim(sim,f,a,t)                                = tfab2sim(sim,f,a,t);

tvacb(c,ac,t)$(NOT tvacbsim(sim,c,ac,t) AND NOT tvacb2sim(sim,c,ac,t)) = tvacb0(c,ac,t);
tvacb(c,ac,t)$tvacbsim(sim,c,ac,t)                                  = tvacb0(c,ac,t)*tvacbsim(sim,c,ac,t);
tvacb(c,ac,t)$tvacb2sim(sim,c,ac,t)                                 = tvacb2sim(sim,c,ac,t);

* if at least one element of [insdng] in ty01sim(sim,insdng), use ty01sim to define ty01
ty01(insdng,t)$(NOT SUM(insdngp, ty01sim(sim,insdngp,t))) = ty010(insdng,t);
ty01(insdng,t)$(SUM(insdngp, ty01sim(sim,insdngp,t)))     = ty01sim(sim,insdng,t);


tyb(insdng,t)$(NOT tybsim(sim,insdng,t) AND NOT tyb2sim(sim,insdng,t)) = tyb0(insdng,t);
tyb(insdng,t)$tybsim(sim,insdng,t)                                     = tyb0(insdng,t)*tybsim(sim,insdng,t);
tyb(insdng,t)$tyb2sim(sim,insdng,t)                                    = tyb2sim(sim,insdng,t);

* if at least one element of [f] in tf01sim(sim,f), use tf01sim to define tf01
tf01(f,t)$(NOT SUM(fp, tf01sim(sim,fp,t))) = tf010(f,t);
tf01(f,t)$(SUM(fp, tf01sim(sim,fp,t)))     = tf01sim(sim,f,t);


tfb(f,t)$(NOT tfbsim(sim,f,t) AND NOT tfb2sim(sim,f,t)) = tfb0(f,t);
tfb(f,t)$tfbsim(sim,f,t)                                = tfb0(f,t)*tfbsim(sim,f,t);
tfb(f,t)$tfb2sim(sim,f,t)                               = tfb2sim(sim,f,t);


* if at least one element of [a] in ta01sim(sim,a), use ta01sim to define ta01
ta01(a)$(NOT SUM(ap, ta01sim(sim,ap))) = ta010(a);
ta01(a)$(SUM(ap, ta01sim(sim,ap)))     = ta01sim(sim,a);

subcb(c,d,t)$(NOT subcbsim(sim,c,t)) = subcb0(c,d,t);
subcb(c,d,t)$subcbsim(sim,c,t)       = subcb0(c,d,t)*subcbsim(sim,c,t);
subcb(c,d,t)$subcb2sim(sim,c,d,t)    = subcb2sim(sim,c,d,t);


PWE.FX(c,t)$(NOT ced(c) AND NOT PWESIM(sim,c,t)) = PWE0(c,t);
PWE.FX(c,t)$(NOT ced(c) AND PWESIM(sim,c,t))     = PWE0(c,t)*PWESIM(sim,c,t);

PWM.FX(c,t)$(NOT PWMSIM(sim,c,t))                = PWM0(c,t);
PWM.FX(c,t)$PWMSIM(sim,c,t)                      = PWM0(c,t)*PWMSIM(sim,c,t);

qeb(c,t)$((ced(c) OR cesexog(c)) AND NOT qebsim(sim,c,t)) = qeb0(c,t);
qeb(c,t)$((ced(c) OR cesexog(c)) AND qebsim(sim,c,t))     = qeb0(c,t)*qebsim(sim,c,t);


qmbar(c,t)$(NOT qmbarsim(sim,c,t)) = qmbar0(c,t);
qmbar(c,t)$(qmbarsim(sim,c,t))     = qmbar0(c,t)*qmbarsim(sim,c,t);

REXR.FX(t)$(NOT rexrsim(sim,t) AND rowclossim(sim,t)=4) = REXR0(t);
REXR.FX(t)$(rexrsim(sim,t) AND rowclossim(sim,t)=4)     = REXR0(t)*rexrsim(sim,t);

PREXR.FX(t)$(NOT prexrsim(sim,t) AND rowclossim(sim,t)=1) = PREXR0(t);
PREXR.FX(t)$(prexrsim(sim,t) AND rowclossim(sim,t)=1)     = PREXR0(t)*prexrsim(sim,t);


*HL-Start

*Need for diagnostic on pweindexsim and pwmindexsim?
* pwe(c,t)$(NOT SUM(tp, pweindexsim(sim,c,tp))) = pwe0(c,t);
* pwe(c,t)$SUM(tp, pweindexsim(sim,c,tp)) = pwe00(c)*pweindexsim(sim,c,t);

* pwm(c,t)$(NOT SUM(tp, pwmindexsim(sim,c,tp))) = pwm0(c,t);
* pwm(c,t)$SUM(tp, pwmindexsim(sim,c,tp)) = pwm00(c)*pwmindexsim(sim,c,t);


*HL-End

* if at least one element of invg in qgc01sim, use qgc01sim to define qgc01
qgc01(c,t)$(NOT SUM((cp,tp), qgc01sim(sim,cp,tp))) = qgc010(c,t);
qgc01(c,t)$(SUM((cp,tp), qgc01sim(sim,cp,tp)))     = qgc01sim(sim,c,t);

qgb(c,t)$(NOT qgbsim(sim,c,t)) = qgb0(c,t);
qgb(c,t)$qgbsim(sim,c,t)       = qgb0(c,t)*qgbsim(sim,c,t);

dqg(c,t)$(NOT dqgsim(sim,c,t)) = dqg0(c,t);
dqg(c,t)$dqgsim(sim,c,t)       = dqgsim(sim,c,t)/scaling('samsol');


* start: translate shock using dkinsbgdp0sim to shock using ddkinssim

ddkinssim(sim,ins2,fcap,t)$ddkinsgdp0sim(sim,ins2,fcap,t) = GDPMP0(t)*scaling('samsol')*ddkinsgdp0sim(sim,ins2,fcap,t)/PK0(fcap,t);

* end: translate shock using dkinsbgdp0sim to shock using ddkinssim

dkinsb(ins2,fcap,t)$(NOT dkinsbsim(sim,ins2,fcap,t)) = dkinsb0(ins2,fcap,t);
dkinsb(ins2,fcap,t)$dkinsbsim(sim,ins2,fcap,t)       = dkinsb0(ins2,fcap,t)*dkinsbsim(sim,ins2,fcap,t);

ddkins(ins2,fcap,t)$(NOT ddkinssim(sim,ins2,fcap,t)) = ddkins0(ins2,fcap,t);
ddkins(ins2,fcap,t)$ddkinssim(sim,ins2,fcap,t) = ddkinssim(sim,ins2,fcap,t)/scaling('samsol');


mpsb(insdng,t)$(NOT mpsbsim(sim,insdng,t)) = mpsb0(insdng,t);
mpsb(insdng,t)$mpsbsim(sim,insdng,t)       = mpsb0(insdng,t)*mpsbsim(sim,insdng,t);

dmpsb(insdng,t)$(NOT dmpsbsim(sim,insdng,t)) = dmpsb0(insdng,t);
dmpsb(insdng,t)$dmpsbsim(sim,insdng,t)       = dmpsbsim(sim,insdng,t);



*SAVF.FX(t)$(NOT FSAVSIM(sim,t) AND rowclossim(sim)=1) = SAVF0(t);
*SAVF.FX(t)$(FSAVSIM(sim,t) AND rowclossim(sim)=1)     = SAVF0(t)*FSAVSIM(sim,t);

trnsfrb(ac,ins,t)$(NOT trnsfrbsim(sim,ac,ins,t)) = trnsfrb0(ac,ins,t);
trnsfrb(ac,ins,t)$trnsfrbsim(sim,ac,ins,t)  = trnsfrb0(ac,ins,t)*trnsfrbsim(sim,ac,ins,t);

dtrnsfr(ac,ins,t)$(NOT dtrnsfrsim(sim,ac,ins,t)) = dtrnsfr0(ac,ins,t);
dtrnsfr(ac,ins,t)$dtrnsfrsim(sim,ac,ins,t) = dtrnsfrsim(sim,ac,ins,t)/scaling('samsol');



*invvalfb(t)$SUM(tp, fdiindexsim(sim,tp))                    = SUM(tmin, invvalfb0(tmin))*fdiindexsim(sim,t);
*invvalfb(t)$(NOT SUM(tp, fdiindexsim(sim,tp)))              = invvalfb0(t);
 
 

tfpexog(a,t)$(NOT tfpexogsim(sim,a,t)) = tfpexog0(a,t);
tfpexog(a,t)$tfpexogsim(sim,a,t)       = tfpexog0(a,t)*tfpexogsim(sim,a,t);

fprdab(f,a,t)$(NOT fprdbsim(sim,f,t) AND NOT fprdabsim(sim,f,a,t)) = fprdab0(f,a,t);
fprdab(f,a,t)$fprdbsim(sim,f,t)       = fprdab0(f,a,t)*fprdbsim(sim,f,t);
fprdab(f,a,t)$fprdabsim(sim,f,a,t)    = fprdab0(f,a,t)*fprdabsim(sim,f,a,t);


CPI.FX(t)$(NOT CPISIM(sim,t) AND numerairesim(sim)=1) = CPI0(t);
CPI.FX(t)$(CPISIM(sim,t) AND numerairesim(sim)=1)     = CPI0(t)*CPISIM(sim,t);



* start: translate shock using invvalfbgdp0sim to shock using invvalfb2sim

*invvalfb2sim(sim,t)$invvalfbgdp0sim(sim,t) = GDPMP0(t)*scaling('samsol')*SUM(fcapng, invshr(fcapng,'rowz',t)*invvalfbgdp0sim(sim,t)/PK0(fcapng,t));
invvalfb2sim(sim,t)$invvalfbgdp0sim(sim,t) = GDPMP0(t)*scaling('samsol')*invvalfbgdp0sim(sim,t)/EXR0(t);

* end: translate shock using invvalfbgdp0sim to shock using invvalfb2sim

invvalfb(t)$(NOT invvalfbsim(sim,t) AND NOT invvalfb2sim(sim,t)) = invvalfb0(t);
invvalfb(t)$invvalfbsim(sim,t)                                   = invvalfb0(t)*invvalfbsim(sim,t);
invvalfb(t)$invvalfb2sim(sim,t)                                  = invvalfb0(t) + invvalfb2sim(sim,t)/scaling('samsol');


dkafdi(f,a,t)$(NOT dkafdisim(sim,f,a,t) AND NOT dkafdi2sim(sim,f,a,t)) = dkafdi0(f,a,t);
dkafdi(f,a,t)$dkafdisim(sim,f,a,t) = dkafdi0(f,a,t)*dkafdisim(sim,f,a,t);
dkafdi(f,a,t)$dkafdi2sim(sim,f,a,t) = dkafdi0(f,a,t) + dkafdi2sim(sim,f,a,t)/scaling('samsol');;


DKA.LO(f,a,t) = -INF;
DKA.UP(f,a,t) = +INF;
dkaexog01(a)$(NOT SUM(ap, dkaexog01sim(sim,ap))) = dkaexog010(a);
dkaexog01(a)$(SUM(ap, dkaexog01sim(sim,ap)))     = dkaexog01sim(sim,a);
DKA.FX(f,a,t)$dkaexog01(a) = DKA0(f,a,t);


qtrstb(c,instrst,t)$(NOT qtrstbsim(sim,c,instrst,t)) = qtrstb0(c,instrst,t);
qtrstb(c,instrst,t)$qtrstbsim(sim,c,instrst,t)       = qtrstb0(c,instrst,t)*qtrstbsim(sim,c,instrst,t);


TRSMREC.FX(t)$(NOT TRSMRECSIM(sim,t) AND ngovpayrulesim(sim,'tourismrec')=1) = TRSMREC0(t);
TRSMREC.FX(t)$(TRSMRECSIM(sim,t) AND ngovpayrulesim(sim,'tourismrec')=1)     = TRSMREC0(t)*TRSMRECSIM(sim,t);


WFDIST.FX(f,a,t)$(fsam(f) AND NOT facclossim(sim,f)=2 AND NOT WFDISTSIM(sim,f,a,t)) = WFDIST0(f,a,t);
WFDIST.FX(f,a,t)$(fsam(f) AND NOT facclossim(sim,f)=2 AND WFDISTSIM(sim,f,a,t))     = WFDIST0(f,a,t)*WFDISTSIM(sim,f,a,t);

pop(acpop,t)$(NOT popsim(sim,acpop,t) AND NOT pop2sim(sim,acpop,t)) = pop0(acpop,t);
pop(acpop,t)$popsim(sim,acpop,t)                                    = pop0(acpop,t)*popsim(sim,acpop,t);
pop(acpop,t)$pop2sim(sim,acpop,t)                                   = pop0(acpop,t) + pop2sim(sim,acpop,t)/scaling('qlabsol');


shrbtvat(c,a,t)$(NOT shrbtvat2sim(sim,c,a,t) AND NOT shrbtvat2sim(sim,c,a,t)) = shrbtvat0(c,a,t);
shrbtvat(c,a,t)$shrbtvatsim(sim,c,a,t)                                        = shrbtvat0(c,a,t)*shrbtvatsim(sim,c,a,t);
shrbtvat(c,a,t)$shrbtvat2sim(sim,c,a,t)                                       = shrbtvat2sim(sim,c,a,t);

ica(c,a,t)$(NOT icasim(sim,c,a,t)) = ica0(c,a,t);
ica(c,a,t)$(icasim(sim,c,a,t))     = ica0(c,a,t)*icasim(sim,c,a,t);


* start: diagnose govclos8
errgovclos8(sim,t)$(govclossim(sim,t)=8 AND SUM(ap, tfp01sim(sim,ap))=0) = 1/0;
DISPLAY errgovclos8;
* end: diagnose govclos8

* if at least one element of [a] in tfp01sim(sim,a), use tfp01sim to define ta01
tfp01(a)$(NOT SUM(ap, tfp01sim(sim,ap))) = tfp010(a);
tfp01(a)$(SUM(ap, tfp01sim(sim,ap)))     = tfp01sim(sim,a);

* start: impose real GDP FC

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

* end: impose real GDP FC



* start: definition of mpk

* reset
mpcapgov(fcap) = 0;
mtfp(a,fcap) = 0;
mpk(a,fcap,t) = 0;

mpcapgov(fcap)$(NOT mpcapgovsim(sim,fcap)) = mpcapgov0(fcap);
mpcapgov(fcap)$(mpcapgovsim(sim,fcap))     = mpcapgovsim(sim,fcap);

* if at least one element of a in mtfpsim, use mtfpsim to define mtfp
mtfp(a,fcap)$(NOT SUM(ap, mtfpsim(sim,ap,fcap))) = mtfp0(a,fcap);
mtfp(a,fcap)$(SUM(ap, mtfpsim(sim,ap,fcap)))     = mtfpsim(sim,a,fcap);


* raw definition of MP for act a of change in cap stock fcap
* = (aggregate MP of fcap) * (share of a in GDP) * (relative strength of MP for a from fcap) 
mpksim(sim,a,fcap,t)$(mpcapgov(fcap) AND mtfp(a,fcap)) = 
  mpcapgov(fcap) * (SUM(f, SAM(f,a))/SUM((f,ap), SAM(f,ap))) * mtfp(a,fcap);
DISPLAY mpk00;

* scaling MP for a from f so that sum of activity-specific MPs equals aggregate MP
* if at least one element of a in mtfpsim, use mpksim to define mpk
mpk(a,fcap,t)$(tsol(t) AND NOT SUM(ap, mpksim(sim,ap,fcap,t))) = mpk0(a,fcap,t);
mpk(a,fcap,t)$(tsol(t) AND SUM(ap, mpksim(sim,ap,fcap,t)))     = mpksim(sim,a,fcap,t)/SUM(ap, mpksim(sim,ap,fcap,t)) * mpcapgov(fcap);
mpk('total',fcap,t) = SUM(a, mpk(a,fcap,t));



* end: definition of mpk


qfinsb(ins,f,t)$(NOT qfssim(sim,f,t)) = qfinsb0(ins,f,t);
qfinsb(ins,f,t)$(qfssim(sim,f,t))     = qfinsb0(ins,f,t)*qfssim(sim,f,t);



* start: re-calibrate CET


delta_esim(sim,c,t)$(delta_e(c) AND tsol(t) AND qeqdratsim(sim,c,t))   = 1 / ( (PDS0(c,t)/PE0(c,t))*(qeqdratsim(sim,c,t)*QE0(c,t)/QD0(c,t))**(1/sigma_x(c)) + 1 );
delta_dssim(sim,c,t)$(delta_DS(c) AND tsol(t) AND qeqdratsim(sim,c,t)) = 1 - delta_esim(sim,c,t);

* change phi_xsim so that QE0 and QD0 with new delta_e and delta_ds are consistent with QX0
phi_xsim(sim,c,t)$(phi_x(c) AND tsol(t) AND qeqdratsim(sim,c,t)) = 
  QX0(c,t) / ( delta_esim(sim,c,t)*QE0(c,t)**rho_x(c) + delta_dssim(sim,c,t)*QD0(c,t)**rho_x(c) ) ** (1/rho_x(c));

phi_xsim(sim,c,t)$(delta_e(c) AND tsol(t) AND NOT qeqdratsim(sim,c,t)) = phi_x(c);
delta_esim(sim,c,t)$(delta_e(c) AND tsol(t) AND NOT qeqdratsim(sim,c,t)) = delta_e(c);
delta_dssim(sim,c,t)$(delta_e(c) AND tsol(t) AND NOT qeqdratsim(sim,c,t)) = delta_ds(c);

* assumption: in ref0 scaldelta_ds, scaldelta_e y scalphi_x are always equal to 1
scaldelta_ds(c,t)$delta_ds(c) = delta_dssim(sim,c,t)/delta_ds(c);
scaldelta_e(c,t)$delta_e(c)   = delta_esim(sim,c,t)/delta_e(c);
scalphi_x(c,t)$phi_x(c)       = phi_xsim(sim,c,t)/phi_x(c);


* end: re-calibrate CET  


NFFG.FX(t)$(govclossim(sim,t) NE 3 AND govrecrulesim(sim,'netforfingov') EQ 1 AND NOT NFFGSIM(sim,t)) = NFFG0(t);
NFFG.FX(t)$(govclossim(sim,t) NE 3 AND govrecrulesim(sim,'netforfingov') EQ 1 AND NFFGSIM(sim,t))     = NFFG0(t) + NFFGSIM(sim,t)/scaling('samsol');

dnffins(t)$(NOT dnffinssim(sim,t)) = dnffins0(t);
dnffins(t)$dnffinssim(sim,t) = dnffinssim(sim,t);

nffinsbar(t)$(NOT nffinsbarsim(sim,t)) = nffinsbar0(t);
nffinsbar(t)$nffinsbarsim(sim,t) = nffinsbar0(t)*nffinsbarsim(sim,t);

* end: impose simulations values for exogenous variables







DISPLAY "#### END: par-defn-sim.inc";


