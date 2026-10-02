* mod.gms


DISPLAY "#### START: mod.gms";

* declaration of auxiliary parameters
$INCLUDE par-decl-aux.inc


* ASSIGNMENTS FOR PARAMETERS AND VARIABLES ==========================

*START: Parameters and variables - definition ==========================



* Diagnosis BEFORE Definitions
$INCLUDE diagnostics-data.inc


* start: growth rates and growth indices

popgrw(acpop,t)$(NOT tmin(t) AND tsol(t)) = pop0(acpop,t)/pop0(acpop,t-1) - 1;
DISPLAY popgrw;

pop00(acpop) = SUM(tmin, pop0(acpop,tmin));
pop(acpop,t) = pop0(acpop,t);
DISPLAY pop00;




IF (dmod=2,
  popgrw(acpop,t)$(NOT tmin(t)) = ssgrw('qlab');
  qfacgrw(f,t)$(NOT tmin(t) AND flab(f)) = ssgrw('qlab');
  qfacgrw(f,t)$(NOT tmin(t) AND NOT flab(f)) = ssgrw('gdp');
  gdpgrw(t)$(NOT tmin(t)) = ssgrw('gdp');
  govrecgrw0(acgovrec,t) = 0;
  govspndgrw0(acgovspnd,t) = 0;
  ngovpaygrw0(acngovpay,t) = 0;

* note:
* we know that grwrat = ((1 + popgrwrat)*(1 + tfpgrwrat)) - 1;
* then, compute fprdgrw so that ssgrw('gdp') = ssgrw('qlab') + fprdgrw

  fprdgrw(f,t)$flab(f) = (1+ssgrw('gdp'))/(1+ssgrw('qlab')) - 1;
* implied factor prod growth
  ssgrw('prd') = (1+ssgrw('gdp'))/(1+ssgrw('qlab')) - 1;

);


* index(tbas) = 1
gdpindex(tmin) = 1;
popindex(acpop,tmin) = 1;
qfacindex(f,tmin) = 1;
fprdindex(f,tmin) = 1;
prdindex(tmin) = 1;
agelabindex(tmin) = 1;
govspndindex0(acgovspnd,tmin) = 1;
govrecindex0(acgovrec,tmin) = 1;
ngovpayindex0(acngovpay,tmin) = 1;

govspndgrw0(acgovspnd,t)$(tsol(t) AND NOT SUM(tp, govspndgrw0(acgovspnd,tp))) = gdpgrw(t);
govrecgrw0(acgovrec,t)$(tsol(t) AND NOT SUM(tp, govrecgrw0(acgovrec,tp))) = gdpgrw(t);
ngovpaygrw0(acngovpay,t)$(tsol(t) AND NOT SUM(tp, ngovpaygrw0(acngovpay,tp))) = gdpgrw(t);

* if qfacgrw is empty, calculate qfacgrw for labor based on qlabins0
qfacgrw(flab,t)$(NOT SUM((flabp,tp), qfacgrw(flabp,tp)) AND SUM(ins, qlabins0(ins,flab,t-1))) = 
  SUM(ins, qlabins0(ins,flab,t)) / SUM(ins, qlabins0(ins,flab,t-1)) -1;


LOOP(t$(NOT tmin(t) AND tsol(t)),
  gdpindex(t) = gdpindex(t-1)*(1 + gdpgrw(t));
  popindex(acpop,t) = popindex(acpop,t-1)*(1 + popgrw(acpop,t));
  qfacindex(f,t) = qfacindex(f,t-1) * (1+qfacgrw(f,t));
  fprdindex(f,t) = fprdindex(f,t-1) * (1+fprdgrw(f,t));
  prdindex(t) = prdindex(t-1) * (1+ssgrw('prd'));
  agelabindex(t) = agelabindex(t-1)*(1 + popgrw('agelab',t));


  govspndindex0(acgovspnd,t) = govspndindex0(acgovspnd,t-1)*(1+govspndgrw0(acgovspnd,t));
  govrecindex0(acgovrec,t)   = govrecindex0(acgovrec,t-1)*(1+govrecgrw0(acgovrec,t));
  ngovpayindex0(acngovpay,t) = ngovpayindex0(acngovpay,t-1)*(1+ngovpaygrw0(acngovpay,t));

);
DISPLAY gdpindex, popindex, qfacindex, fprdindex, ngovpayindex0;

pop0(acpop,t)$(dmod=2) = pop00(acpop)*popindex(acpop,t);
* overwrite pop when dmod=2
pop(acpop,t)$(dmod=2) = pop0(acpop,t);

* end: growth rates and growth indices



* start: average growth rate for gdp

PARAMETER
  gdpgrwavg           gdp average growth rate
  agelabgrwavg        population in labor force age average growth rate
  tsolnb2
;

tsolnb2 = SUM(t$tmax(t), ORD(t)) - SUM(t$tmin(t), ORD(t));

gdpgrwavg$((SUM(tmin, gdpindex(tmin))>0 AND SUM(tmax, gdpindex(tmax))>0)
  OR (SUM(tmin, gdpindex(tmin))<0 AND SUM(tmax, gdpindex(tmax))<0)) =
  ( ( SUM(tmax, gdpindex(tmax)) / SUM(tmin, gdpindex(tmin)) ) ** (1/tsolnb2) - 1 );

agelabgrwavg$((SUM(tmin, agelabindex(tmin))>0 AND SUM(tmax, agelabindex(tmax))>0)
  OR (SUM(tmin, agelabindex(tmin))<0 AND SUM(tmax, agelabindex(tmax))<0)) =
  ( ( SUM(tmax, agelabindex(tmax)) / SUM(tmin, agelabindex(tmin)) ) ** (1/tsolnb2) - 1 );

* end: average growth rate for gdp




* Prices

PA00(a) = 1;
PA0(a,t)$tsol(t) = PA00(a);

PX00(c) = 1;
PX0(c,t)$tsol(t) = 1;

PDS00(c) = 1;
PDS0(c,t)$tsol(t) = 1;

PE00(c)$SUM(insrow, SAM(c,insrow)) = 1;
PE0(c,t)$tsol(t) = PE00(c);
PM00(c)$SUM(insrow, SAM(insrow,c)) = 1;
PM0(c,t)$tsol(t) = PM00(c);

EXR00 = 1;
EXR0(t)$tsol(t) = EXR00;



* start: exchange rate premiun

PARAMETER
  prratexr00;
prratexr00 = SUM(c, SAM('prexr',c)) / SUM(c, (1-shrom000(c))*SAM('row',c) - (1-shroe000(c))*SAM(c,'row'));



PREXR00 = 1 + prratexr00;
prexrindex0(t)$(SUM(tp, prexrindex0(tp)) EQ 0) = 1;
prexrindex(t)$tsol(t) = prexrindex0(t);
PREXR0(t)$tsol(t) = PREXR00*prexrindex0(t);
DISPLAY PREXR0, PREXR00, prexrindex0;


YPREXRT00(c) = SAM('prexr',c);
YPREXRT0(c,t)$tsol(t) = YPREXRT00(c);

* allocation of rents

* assumption: same allocation of exr premium for all commodities
*shryprexr00(c,insd) = SUM(cp, SAM(cp,insd)) / SUM((cp,insdp), SAM(cp,insdp));
* following assumption works as long as there is a single private capital factor
shryprexr00(c,fcapng) = SAM(fcapng,'prexr')/SAM('total','prexr');
shryprexr0(c,ac,t)$tsol(t) = shryprexr00(c,ac);
shryprexr(c,ac,t) = shryprexr0(c,ac,t);


YPREXR00(c,ac) = shryprexr00(c,ac)*YPREXRT00(c);
YPREXR0(c,ac,t)$tsol(t) = YPREXR00(c,ac);


PARAMETER
  errshryprexr00(c)   if =UNDF total shryprexr00 NE 1
;
errshryprexr00(c)$(ABS(SUM(acnt, shryprexr00(c,acnt)) - 1) > 1e-10) = 1/0;
DISPLAY errshryprexr00;

shroe00(c) = shroe000(c);
shroe0(c,t)$tsol(t) = shroe00(c);
shroe(c,t) = shroe0(c,t);

shrom00(c) = shrom000(c);
shrom0(c,t)$tsol(t) = shrom00(c);
shrom(c,t) = shrom0(c,t);


PARAMETER
  yprexrte(c)
  yprexrtm(c)
  gapyprexrt(c)
;

yprexrte(c) = (1-shroe00(c))*(PREXR00-1)*SAM(c,'row');
yprexrtm(c) = (1-shrom00(c))*(PREXR00-1)*SAM('row',c);
gapyprexrt(c) = -yprexrte(c) + yprexrtm(c) - SAM('prexr',c);


* end: exchange rate premiun






* QA, QE, QM, QX, QD, QQ

*QA00(a) = SAM(a,'total') / PA00(a);
* note: need to deduct SAM(a,'prqmbar') because if transposed has negative sign
QA00(a) = [SAM(a,'total')-SAM(a,'prqmbar')] / PA00(a);
QA0(a,t)$tsol(t) = QA00(a)*gdpindex(t);

* exported quantity is computed as exporters income divided by the domestic
* price of exports
QE00(c)$(SUM(insrow, SAM(c,insrow))) = ( SUM(insrow, SAM(c,insrow)) - SUM(taxexp, SAM(taxexp,c)) - SUM(tace, SAM(tace,c)) + yprexrte(c) ) / PE00(c);
QE0(c,t)$tsol(t) = QE00(c)*gdpindex(t);



* imported quantity is computed as payment from demanders before sales tax
* (import value + tariffs) divided by the domestic price of imports
QM00(c)$(SUM(insrow, SAM(insrow,c))) = ( SUM(insrow, SAM(insrow,c)) + SUM(taximp, SAM(taximp,c)) + SAM('prqmbar',c) + SUM(tacm, SAM(tacm,c)) + yprexrtm(c) ) / PM00(c);
QM0(c,t)$tsol(t) = QM00(c)*gdpindex(t);

QX00(c) = SUM(a, SAM(a,c)) / PX00(c);
QX0(c,t)$tsol(t) = QX00(c)*gdpindex(t);
QD00(c) = QX00(c) - QE00(c);
QD0(c,t)$tsol(t) = QD00(c)*gdpindex(t);
QQ00(c) = QD00(c) + QM00(c);
QQ0(c,t)$tsol(t) = QQ00(c)*gdpindex(t);
DISPLAY QA0, QE0, QM0, QX0, QD0, QQ0;



PXAC00(a,c)$(SAM(a,c)) = 1;
PXAC0(a,c,t)$tsol(t) = PXAC00(a,c);

QXAC00(a,c)$(SAM(a,c)) = SAM(a,c)/PXAC00(a,c);
QXAC0(a,c,t)$tsol(t) = QXAC00(a,c)*gdpindex(t);

* Demand Price for Domestic Commodity

* demander payment divided by quantity bought
PDD00(c)$QD00(c)= ( PDS00(c)*QD00(c) + SUM(tacd, SAM(tacd,c)) ) / QD00(c);
PDD0(c,t)$tsol(t) = PDD00(c);
DISPLAY PDD0;

* demander price of composite commodity c = total payment (including
* sales tax) divided by quantity
PQS00(c)$QQ00(c) = ( PM00(c)*QM00(c) + PDD00(c)*QD00(c) ) / QQ00(c);
PQS0(c,t)$tsol(t) = PQS00(c);
DISPLAY PQS0;


* rate of sales tax  = tax divided by pre-sales-tax payment from demanders
* (i.e., base excludes the tax value)
TQ00(c)$QQ00(c) = SUM(taxcom, SAM(taxcom,c)) / ( PDD00(c)*QD00(c) + PM00(c)*QM00(c) );
TQ0(c,t)$tsol(t) = TQ00(c);
tqb00(c) = TQ00(c);
tqb0(c,t) = TQ0(c,t);
tqb(c,t) = tqb0(c,t);
TQSCAL00 = 1;
TQSCAL0(t)$tsol(t) = 1;

* VAT rate on c for demander acnt.
* [VAT rate for taxed demander acnt] = [total VAT on c]
* DIVIDED BY [total pay for c for taxed demanders excl. VAT]
TVAC00(c,acnt)$mtaxvatc(c,acnt) = SUM(taxvatc, SAM(taxvatc,c)) /
  ( SUM(acntp$mtaxvatc(c,acntp), SAM(c,acntp)) - SUM(taxvatc, SAM(taxvatc,c)) );
* switch from inv to fcap
TVAC00(c,fcap) = SUM(inv$mfcapinv(fcap,inv), TVAC00(c,inv));
TVAC00(c,inv) = 0;

TVAC0(c,ac,t)$tsol(t) = TVAC00(c,ac);
tvacb(c,ac,t) = TVAC0(c,ac,t);
tvacb00(c,ac) = TVAC00(c,ac);
tvacb0(c,ac,t) = tvacb(c,ac,t);
TVACSCAL0(t)          = 1;
DISPLAY TVAC0;


* commodity subsidy rate
* note: It is assumed that the same subsidy rate applies to all demanders
* singled out by the mapping msubcom

PARAMETER
  vatpay(c,ac)
  subsidy(c,ac)
;
vatpay(c,d)$mtaxvatc(c,d) = SUM(taxvatc, SAM(taxvatc,c)) * SAM(c,d)/SUM(dp$mtaxvatc(c,dp), SAM(c,dp));
vatpay(c,'total') = SUM(d, vatpay(c,d));

subsidy(c,d)$msubcom(c,d) = -SUM(subcom, SAM(subcom,c)) * SAM(c,d)/SUM(dp$msubcom(c,dp), SAM(c,dp));
subsidy(c,'total') = SUM(d$msubcom(c,d), subsidy(c,d));
* [subsidy rate] = [subsidy] DIVIDED BY [total demander pay + subsidy - demander vat pay]
SUBC00(c,d)$msubcom(c,d) = -SUM(subcom, SAM(subcom,c)) /
  ( SUM(dp$msubcom(c,dp), SAM(c,dp) + subsidy(c,dp) - vatpay(c,dp)) );
* switch from inv to fcap
SUBC00(c,fcap) = SUM(inv$mfcapinv(fcap,inv), SUBC00(c,inv));
SUBC00(c,inv) = 0;
SUBC0(c,ac,t)$tsol(t) = SUBC00(c,ac);
subcb(c,ac,t) = SUBC0(c,ac,t);
subcb00(c,ac) = SUBC00(c,ac);
subcb0(c,ac,t) = subcb(c,ac,t);
SUBCSCAL0(t) = 1;
DISPLAY SUBC0;



PQD00(c,d)$SAM(c,d) = PQS00(c)*(1+TQ00(c))*(1+TVAC00(c,d))*(1-SUBC00(c,d));
PQD00(c,fcap)$SUM(inv$mfcapinv(fcap,inv), SAM(c,inv)) = PQS00(c)*(1+TQ00(c))*(1+TVAC00(c,fcap))*(1-SUBC00(c,fcap));
PQD00(c,'row') = 0;
PQD0(c,d,t)$tsol(t) = PQD00(c,d);

PARAMETER errpqd00(c,ac);
errpqd00(c,d)$(PQD00(c,d)<0) = 1/0;
DISPLAY errpqd00;

$ONTEXT
* demander price of composite commodity c = total payment (including
* sales tax) divided by quantity
PQD00(c,acnt)$SAM(c,acnt) = PQS00(c) * (1 + TQ00(c));
PQD0(c,ac,t)$tsol(t) = PQD00(c,ac);
DISPLAY PQD0;
$OFFTEXT




* start: adapt SAM to add VAT rebate --------------------------------

* note that SAM(c,a)/PQD00(c,a) = QINT00(c,a)
* tax collection on intermediate input for is (1-SUBC(c,a,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,a,t)*QINT(c,a,t))
* note that tax base includes SUBC AND TQ
RBTVAT00(a) = SUM(c$PQD00(c,a), SAM(c,a)/PQD00(c,a)*PQS00(c)*(1-SUBC00(c,a))*(1+TQ00(c))*TVAC00(c,a));
RBTVAT0(a,t)$tsol(t) = RBTVAT00(a)*gdpindex(t);
DISPLAY RBTVAT0;

shrbtvat00(c,a) = 1;
shrbtvat0(c,a,t)$tsol(t) = shrbtvat00(c,a);
shrbtvat(c,a,t) = shrbtvat0(c,a,t);

* adapt SAM
SAM('rebate-vat',a)     = -RBTVAT00(a);
SAM(taxact,a)      = SAM(taxact,a) + RBTVAT00(a);
SAM(insgov,taxact)  = SAM(insgov,taxact) + SUM(a, RBTVAT00(a));
SAM('rebate-vat','gov') = SUM(a, RBTVAT00(a));

* check sam consistency
SAM('total',ac) = SUM(acntp, SAM(acntp,ac));
SAM(ac,'total') = SUM(acntp, SAM(ac,acntp));
sambalchk(acnt) = SUM(acntp, SAM(acnt, acntp)) - SUM(acntp, SAM(acntp,acnt));
DISPLAY sambalchk;




* end: adapt SAM to add VAT rebate ----------------------------------




* rate of producer (activity) tax = tax divided by output value (i.e., tax
* base includes the tax value)
TA00(a) = SUM(taxact, SAM(taxact,a)) / (PA00(a)*QA00(a));
TA0(a,t)$tsol(t) = TA00(a);
tab00(a) = TA00(a);
tab0(a,t) = TA0(a,t);
tab(a,t) = tab0(a,t);
TASCAL00 = 0;
TASCAL0(t)$tsol(t) = TASCAL00;
ta010(a) = 1;
ta01(a) = ta010(a);
DISPLAY TA0;

* rate of tax on factor use
TFA00(f,a)$SAM(f,a) =
  SUM(taxfacact$mtaxfa(taxfacact,f), SAM(taxfacact,a)) / SAM(f,a);
TFA0(f,a,t)$tsol(t) = TFA00(f,a);
tfab(f,a,t) = TFA0(f,a,t);
tfab00(f,a) = TFA00(f,a);
tfab0(f,a,t) = tfab(f,a,t);
TFASCAL00 = 1;
TFASCAL0(t)$tsol(t) = TFASCAL00;
DISPLAY TFA0;






* Trade and Transport Margins

PARAMETERS
  shctd(c)    share of commy ct in trans services for domestic sales
  shctm(c)    share of commy ct in trans services for imports
  shcte(c)    share of commy ct in trans services for exports
;

shctd(ct) = SUM(tacd, SAM(ct,tacd)/SAM('total',tacd));
shctm(ct) = SUM(tacm, SAM(ct,tacm)/SAM('total',tacm));
shcte(ct) = SUM(tace, SAM(ct,tace)/SAM('total',tace));
DISPLAY shctd, shctm, shcte;

* transactions input coefficients
icd(ct,c)$(shctd(ct) AND QD00(c)) = (shctd(ct)*SUM(tacd, SAM(tacd,c)/PQD00(ct,tacd))) / QD00(c);
icm(ct,c)$(shctm(ct) AND QM00(c)) = (shctm(ct)*SUM(tacm, SAM(tacm,c)/PQD00(ct,tacm))) / QM00(c);
ice(ct,c)$(shcte(ct) AND QE00(c)) = (shcte(ct)*SUM(tace, SAM(tace,c)/PQD00(ct,tace))) / QE00(c);

QT00(ct) = SUM(tacd, SAM(ct,tacd)/PQD00(ct,tacd)) + SUM(tace, SAM(ct,tace)/PQD00(ct,tace)) + SUM(tacm, SAM(ct,tacm)/PQD00(ct,tacm));
QT0(ct,t)$tsol(t) = QT00(ct)*gdpindex(t);
DISPLAY QT0;

* Consumer Price Index

cwts(c,h) = SAM(c,h) / SUM((cp,hp), SAM(cp,hp));
* CPI = sum of over prices and households weighted by consumption shares
CPI00 = SUM((c,h), cwts(c,h) * PQD00(c,h));
CPI0(t)$tsol(t) = CPI00;
DISPLAY cwts, CPI0;

* Index for Domestic Producer Prices

dwts(c) = ( SUM(a, SAM(a,c)) - SUM(insrow, SAM(c,insrow)) ) /
          (  SUM(cp, SUM(a, SAM(a,cp)) - SUM(insrow, SAM(cp,insrow))) );
DPI00    = SUM(c, dwts(c)*PDS00(c));
DPI0(t)$tsol(t) = DPI00;
DISPLAY dwts, DPI0;

DISPLAY rexrindex0; 

* real exchange rate
REXR00 = EXR00/DPI00;
REXR0(t)$tsol(t) = REXR00;
* default 
rexrindex0(t)$(SUM(tp, rexrindex0(tp)) EQ 0) = 1;
rexrindex(t)$tsol(t) = rexrindex0(t);
REXR0(t)$(tsol(t) AND dmod EQ 2) = REXR00;
REXR0(t)$(tsol(t) AND dmod NE 2) = REXR00*rexrindex(t);
DISPLAY REXR00, rexrindex, rexrindex0; 
*$EXIT
* Value Added

* the value-added price is the factor payments per unit of activity
* MC-2018-10-08
* note the use of NOT fleo in the definition of PVA
*!! to do: adjust reports of GDP at FC
*PVA00(a) = [ SUM(f$(NOT fleo(f) AND fva(f)), SAM(f,a) + SUM(taxfacact$mtaxfa(taxfacact,f), SAM(taxfacact,a))) ] / ( [SAM('total',a)] / PA00(a) );
PVA00(a) = [ SUM(f$(NOT fleo(f) AND fva(f)), SAM(f,a) + SUM(taxfacact$mtaxfa(taxfacact,f), SAM(taxfacact,a))) ] / ( [SAM('total',a)-SAM(a,'prqmbar')] / PA00(a) );
PVA0(a,t)$tsol(t) = PVA00(a);
DISPLAY PVA0;


* Intermediate Inputs

* qnty of intermediate demand for c from a = sam payment divided by price
QINT00(c,a)$SAM(c,a) = SAM(c,a) / PQD00(c,a);
QINT0(c,a,t) = QINT00(c,a)*gdpindex(t);
DISPLAY QINT0;

ica00(c,a) = QINT00(c,a) / QA00(a);
ica0(c,a,t)$tsol(t) = ica00(c,a);
ica(c,a,t) = ica0(c,a,t);
DISPLAY ica;

* Factor Employment and Prices


fprdab00(f,a)$fva(f) = 1;

*HL20251102
*fprdab0(f,a,t)  = fprdab00(f,a)*fprdindex(f,t);
 fprdab0(f,a,t)  = fprdab00(f,a);
 
 fprdab0(f3,a,tmin) =  fprdab00(f3,a)

$ONTEXT
LOOP(t$(NOT tmin(t)),
*fprdab0(f3,a,t) = fprdab0(f3,a,t-1)*(1 - 0.01);

*1
 fprdab0(f3,a,t) = fprdab0(f3,a,t-1)*(1 - 0.01);

*2 
 fprdab0('f-labm-n',a,t) = fprdab0('f-labm-n',a,t-1)*(1 + 0.00001);
 fprdab0('f-labm-p',a,t) = fprdab0('f-labm-p',a,t-1)*(1 + 0.00001);
 fprdab0('f-labm-s',a,t) = fprdab0('f-labm-s',a,t-1)*(1 + 0.01);
 fprdab0('f-labm-t',a,t) = fprdab0('f-labm-t',a,t-1)*(1 + 0.01);
 fprdab0('f-labf-n',a,t) = fprdab0('f-labf-n',a,t-1)*(1 + 0.00001);
 fprdab0('f-labf-p',a,t) = fprdab0('f-labf-p',a,t-1)*(1 + 0.00001);
 fprdab0('f-labf-s',a,t) = fprdab0('f-labf-s',a,t-1)*(1 + 0.01);
 fprdab0('f-labf-t',a,t) = fprdab0('f-labf-t',a,t-1)*(1 + 0.01);

*3 
 fprdab0('f-labm-n',a,t) = fprdab0('f-labm-n',a,t-1)*(1 + 0.00001);
 fprdab0('f-labm-p',a,t) = fprdab0('f-labm-p',a,t-1)*(1 + 0.00001);
 fprdab0('f-labm-s',a,t) = fprdab0('f-labm-s',a,t-1)*(1 + 0.01);
 fprdab0('f-labm-t',a,t) = fprdab0('f-labm-t',a,t-1)*(1 + 0.01);
 fprdab0('f-labf-n',a,t) = fprdab0('f-labf-n',a,t-1)*(1 + 0.00001);
 fprdab0('f-labf-p',a,t) = fprdab0('f-labf-p',a,t-1)*(1 + 0.00001);
 fprdab0('f-labf-s',a,t) = fprdab0('f-labf-s',a,t-1)*(1 + 0.01);
 fprdab0('f-labf-t',a,t) = fprdab0('f-labf-t',a,t-1)*(1 + 0.01);
 );
$OFFTEXT
DISPLAY fprdab0; 
 
*$EXIT
fprdab(f,a,t) = fprdab0(f,a,t);
FPRDA00(f,a)$fva(f)  = 1;
FPRDA0(f,a,t) = fprdab0(f,a,t);


* start: factor supply and demand baseyr

UERAT00(f)$fuendog(f)  = unemp(f,'UERAT00');
UERAT0(f,t)$tsol(t) = UERAT00(f);
eta_wf(f)$fuendog(f) = unemp(f,'eta_wf');



* factor demand for factors without physical quantities:
* quantity = value (assuming that wage is one)
QF00(f,a)$(NOT qfbase(f,a)) = SAM(f,a);

* factor demand for factors without physical quantities:
QF00(f,a)$(qfbase(f,a)) = qfbase(f,a);

* overwrite for gov capital
QF00(fcapg,a) = 0;


* start: check labor endowments and labor supplies (by definition, must be equal)

PARAMETER
  errgapqlab100(f)  
  errgapqlab200(f)
  
  errgapqlab00(f)
;

errgapqlab100(f) = SUM(a, qfbase(f,a))/(1-unemp(f,'UERAT00'));
errgapqlab200(f) = SUM((h,tmin), qlabins0(h,f,tmin));

errgapqlab00(f)$(flab(f) AND errgapqlab200(f) AND ABS(errgapqlab100(f) - errgapqlab200(f))>1e-6) = 1/0;

DISPLAY errgapqlab00;

* end: check labor endowments and labor supplies (by definition, must be equal)


* factor supply is total quantity demanded + unemployment
QFS00(f)$fncap(f) = SUM(a, QF00(f,a)) / (1-UERAT00(f));


* start: using netprfrat

* alt 1: estimate of initial capital stock when base gdp growth is exogenous and using net profit rate
* change to units of capital assuming a (netprfrat*100)% return
IF (capest=2,
  netprfrat(fcap) = dinam('netprfrat');
  QF00(fcapng,a)$(dmod NE 2) = SAM(fcapng,a) / (netprfrat(fcapng) + deprcap(fcapng));
  QFS00(fcapng)$(dmod NE 2) = SUM(a, QF00(fcapng,a));
);

* end: using netprfrat

* start: using gdpgrw or agelabgrw rates to estimate initial capital stock

$ONTEXT
MICHAEL BERLEMANN and JAN-ERIK WESSELHÖFT. 2014. Estimating Aggregate Capital
Stocks Using the Perpetual Inventory Method – A Survey of Previous
Implementations and New Empirical Evidence for 103 Countries. Review of
Economics, 65.
$OFFTEXT


* alt 1: estimate of initial capital stock when base gdp growth is exogenous and average gdp growth rate
* note: use average gdp growth rate or average population in labor force age growth rate
IF (capest=1,
  QFS00(fcapng)$(dmod NE 2) =
    SUM(invng$mfcapinv(fcapng,invng), SUM(c$SAM(c,invng), SAM(c,invng)/PQD00(c,fcapng))) / ( gdpgrwavg+deprcap(fcapng) );
*  SUM(invng$mfcapinv(fcapng,invng), SUM(c$SAM(c,invng), SAM(c,invng)/PQD00(c,fcapng))) / ( agelabgrwavg+deprcap(fcapng) );

* sectoral capital stock in use
  QF00(f,a)$(fcap(f) AND dmod NE 2 AND SAM(f,a) AND fva(f)) = SAM(f,a) / SUM(ap, SAM(f,ap)) * QFS00(f);
);

* end: using gdpgrw or agelabgrw rates to estimate initial capital stock




* alt 2: estimate of initial capital stock under the assumption of steady state
* gov capital stocks are defined below with other factor endowments
QFS00(fcapng)$(dmod=2) = SUM((c,invng)$(SAM(c,invng) AND mfcapinv(fcapng,invng)), SAM(c,invng)/PQD00(c,fcapng)) / ( ssgrw('gdp') + deprcap(fcapng) );
QF00(fcapng,a)$(dmod=2) = SAM(fcapng,a) / SUM(ap, SAM(fcapng,ap)) * QFS00(fcapng);

QF0(f,a,t)$tsol(t) = QF00(f,a)*qfacindex(f,t);
QFS0(f,t)$tsol(t) = QFS00(f)*qfacindex(f,t);
DISPLAY QFS0;


* end: factor supply and demand baseyr


PARAMETERS
  wfadum(f,a)        wage for factor f in activity a (only for calibration)
  costgap(f,a)    gap calibrated factor cost - SAM value (should be zero)
;

* activity-specific wage: payment divided by quantity
wfadum(f,a)$QF00(f,a) = SAM(f,a)/QF00(f,a);

* average wage by f: total payment divided by total quantity
WF00(f)$(fsam(f) AND fva(f)) = SUM(a, SAM(f,a))/SUM(a, QF00(f,a));
WF0(f,t)$tsol(t) = WF00(f)*fprdindex(f,t);

* wage distortion factors: activity-specific wage divided by avg wage
WFDIST00(f,a)$(fsam(f) AND fva(f)) = wfadum(f,a) / WF00(f);
WFDIST0(f,a,t)$tsol(t) = WFDIST00(f,a);

* Checking calibration
costgap(f,a) = WF00(f)*WFDIST00(f,a)*QF00(f,a) - SAM(f,a);
DISPLAY wfadum, costgap;

* wfa is inclusive of tax on factor use
WFA00(f,a) = WF00(f)*WFDIST00(f,a)*(1+TFA00(f,a));
WFA0(f,a,t)$tsol(t) = WF0(f,t)*WFDIST0(f,a,t)*(1+TFA0(f,a,t));

* initial wages and demands for non-SAM factors (aggregates of SAM factors)
* note that TFA is only levied on SAM factors
WF00(f)$((f1(f) OR f2(f)) AND fnsam(f)) = 1;
WF0(f,t)$(tsol(t) AND (f1(f) OR f2(f)) AND fnsam(f)) = WF00(f)*fprdindex(f,t);
WFDIST00(f,a)$((f1(f) OR f2(f)) AND fnsam(f)) = 1;
WFDIST0(f,a,t)$(tsol(t) AND (f1(f) OR f2(f)) AND fnsam(f)) = WFDIST00(f,a);
WFA00(f,a)$((f1(f) OR f2(f)) AND fnsam(f)) = WF00(f)*WFDIST00(f,a);
WFA0(f,a,t)$(tsol(t) AND (f1(f) OR f2(f)) AND fnsam(f)) = WF0(f,t)*WFDIST0(f,a,t);



QF00(f2,a)$(fnsam(f2) AND WFA00(f2,a)) = SUM(f3$mf3f2(f3,f2), SAM(f3,a)
  + SUM(taxfacact$mtaxfa(taxfacact,f3), SAM(taxfacact,a)))
    / WFA00(f2,a);

QF00(f1,a)$(fnsam(f1) AND WFA00(f1,a)) = SUM(f2$mf2f1(f2,f1), WFA00(f2,a)*QF00(f2,a)
  + SUM(taxfacact$mtaxfa(taxfacact,f2), SAM(taxfacact,a)))
    / WFA00(f1,a);

QF0(f,a,t)$(tsol(t) AND (f1(f) OR f2(f)) AND fnsam(f)) = QF00(f,a)*qfacindex(f,t);



* Factor Incomes
YF00(f) = SUM(acnt, SAM(f,acnt));
YF0(f,t)$tsol(t) = YF00(f)*gdpindex(t);

TF00(f)$YF00(f) = SUM(taxfac, SAM(taxfac,f)) / YF00(f);
TF0(f,t)$tsol(t) = TF00(f);
tfb00(f) = TF00(f);
tfb0(f,t) = TF0(f,t);
tfb(f,t) = tfb0(f,t);
TFSCAL00 = 0;
TFSCAL0(t)$tsol(t) = TFSCAL00;
tf010(f,t)$tsol(t) = 1;
tf01(f,t) = tf010(f,t);

* start: gov capital factors

PARAMETER
  shfcapga(fcapg,a)    share of government capital income in output value
;

shfcapga(fcapg,a) = SAM(fcapg,a)/(PA00(a)*QA00(a));

PARAMETER
  gapyfcapg(fcapg)
;

gapyfcapg(fcapg) = SUM(a, shfcapga(fcapg,a)*PA00(a)*QA00(a)) - YF00(fcapg);
DISPLAY gapyfcapg;



* end: gov capital factors


* Factor Endowments

* note that RoW is excluded from SHIF00, in order to decouple capital income
* to RoW from FDI and corresponding capital stock(s)
*SHIF00(insd,f)$( fsam(f) AND SAM('total',f) - SUM(taxfac, SAM(taxfac,f)) - SUM(insrow, SAM(insrow,f)) ) =
*  SAM(insd,f) / ( SAM('total',f) - SUM(taxfac, SAM(taxfac,f)) - SUM(insrow, SAM(insrow,f)) );
*!! changed formulation:
SHIF00(ins,f)$( fsam(f) AND fva(f) AND SAM('total',f) - SUM(taxfac, SAM(taxfac,f)) ) =
  SAM(ins,f) / ( SAM('total',f) - SUM(taxfac, SAM(taxfac,f)) );

* government capital stocks
*!! to do: allow for alternatives formulas for dmod NE 2
QFINS00(insgov,fcapg) = SUM(invg$mfcapinv(fcapg,invg), SUM(c$SAM(c,invg), SAM(c,invg)/PQD00(c,fcapg))) / (ssgrw('gdp') + deprcap(fcapg));
QFINS0(insgov,fcapg,t)$tsol(t) = QFINS00(insgov,fcapg)*gdpindex(t);

QFINSSCAL00(f) = 1;
QFINSSCAL0(f,t)$tsol(t) = QFINSSCAL00(f);



* to compute capital endowments need to include RoW in auxiliary version shif
* parameter; this is used when dmod NE 2
*!! not needed
*PARAMETER shifdum(ins,f);
*shifdum(ins,f)$( fcap(f) AND fsam(f) AND SAM('total',f) - SUM(taxfac, SAM(taxfac,f)) ) =
*  SAM(ins,f) / ( SAM('total',f) - SUM(taxfac, SAM(taxfac,f)) );

* non-capital and non-labor
QFINS00(ins,f)$(fncap(f) AND NOT flab(f) AND fsam(f) AND fva(f)) = SHIF00(ins,f)*QFS00(f);
QFINS00(ins,f)$(flab(f) AND fsam(f) AND fva(f)) = SHIF00(ins,f)*QFS00(f);
* overwrite for labor in case data is available in qlabins0
QFINS00(ins,f)$(flab(f) AND fsam(f) AND fva(f) AND SUM(tmin, qlabins0(ins,f,tmin))) = SUM(tmin, qlabins0(ins,f,tmin));

* start: check consistency between SHIF00 and QFINS00 for labor

PARAMETER
  gapshif00(h,f)
  errgapshif00(h,f)
;

gapshif00(h,f)$(flab(f) AND SUM(tmin, qlabins0(h,f,tmin))) = SUM(tmin, qlabins0(h,f,tmin))/(SUM((hp,tmin), qlabins0(hp,f,tmin))) - SHIF00(h,f);
errgapshif00(h,f)$(ABS(gapshif00(h,f)>1e-6)) = 1/0;
DISPLAY errgapshif00;

* end: check consistency between SHIF00 and QFINS00 for labor

* note: fcap and fva is an alternative to fcapng
QFINS00(ins,f)$(dmod NE 2 AND fcap(f) AND fva(f) AND fsam(f)) = SHIF00(ins,f)*QFS00(f);
* note that when dmod=2 RoW capital stock is zero, given the need to link its
* evolution with income shares, which are zero for RoW
QFINS00(ins,f)$(dmod EQ 2 AND fcap(f) AND fva(f)  AND fsam(f)) = SHIF00(ins,f)*QFS00(f);




* START: calibration income dist RH =================================


* for non-capital factors, SHIF is fixed at the values here defined for SHIF0

* shifassump assumption for shif
* exogenous at base shares fo non-hhd insd; exogenous and pop-scaled for hhds
* SHIF exogenous at BASE-YEAR shares for all insnh and pop-scaled for hhds
SHIF0(insnh,f,t)$(fsam(f) AND fva(f) AND tsol(t)) = SHIF00(insnh,f);

SHIF0(h,f,t)$(fsam(f) AND fva(f) AND tsol(t) AND SAM(h,f)) =
  SUM(hp, SHIF00(hp,f)) * (SHIF00(h,f)*pop0(h,t)/SUM(tmin, pop0(h,tmin))) /
    SUM(hp, SHIF00(hp,f)*pop0(hp,t)/SUM(tmin, pop0(hp,tmin)));

QFINS0(ins,f,t)$(fsam(f) AND fva(f) AND tsol(t)) = QFS0(f,t)*SHIF0(ins,f,t);
qfinsb0(ins,f,t) = QFINS0(ins,f,t);
qfinsb(ins,f,t) = qfins0(ins,f,t);

PARAMETER
  shifgap(f,t)
;
shifgap(f,t)$tsol(t) = SUM(insd, SHIF00(insd,f)) - SUM(insd, SHIF0(insd,f,t))
DISPLAY shifgap;

* check on total for hhd shares
LOOP((f,t)$SUM(insd, SHIF0(insd,f,t)),
  ABORT$(ABS(shifgap(f,t)) GT 1.0E-4)
    "Gap for shif calc above cutoff."
    "Review calc and displays."
);

* overwrite for households and labor using labor endowment projections
SHIF0(h,f,t)$(tsol(t) AND flab(f)) = QFINS0(h,f,t)/SUM(hp$SHIF00(hp,f), QFINS0(hp,f,t)) * SUM(hp, SHIF00(hp,f));



* END: calibration income dist RH ===================================

* labor force participation rate
LABPARTRAT00 = SUM((ins,flab), QFINS00(ins,flab))/pop00('agelab');
PARAMETER errlabpartrat00;
errlabpartrat00$(LABPARTRAT00>1) = 1/0;
DISPLAY errlabpartrat00;
LABPARTRAT0(t)$(tsol(t) AND (dmod=2 OR NOT SUM(tp, LABPARTRAT0(tp)))) = LABPARTRAT00;
LABPARTRAT0(t)$(tsol(t) AND SUM(tp, LABPARTRAT0(tp)) AND dmod NE 2) = LABPARTRAT00 * LABPARTRAT0(t)/SUM(tmin, LABPARTRAT0(tmin));

QLABSCAL00 = 1;
QLABSCAL0(t)$tsol(t) = QLABSCAL00;



* adjust QFS0(f,t); this is relevant when SAM has more than one household
QFS0(f,t) = SUM(ins, QFINS0(ins,f,t));

* check factor supplies
PARAMETER gapfac(f,t);
gapfac(f,t) = QFS0(f,t) - SUM(ins, QFINS0(ins,f,t));


* used for capital redistribution when pop growth rates differ among households
QFHEND0(h,f,t) = QFINS0(h,f,t);
QFHENDSCAL0(fcapng,t) = 1;


YIF00(ins,f) = SHIF00(ins,f)*YF00(f)*(1-TF00(f));
YIF0(ins,f,t)$tsol(t) = YIF00(ins,f)*gdpindex(t);
DISPLAY SHIF0, YIF0;

* Households

QH00(c,h)$SAM(c,h) = SAM(c,h) / PQD00(c,h);
QH0(c,h,t)$tsol(t) = QH00(c,h)*gdpindex(t);

EH00(h)   = SUM(c, SAM(c,h));
EH0(h,t)$tsol(t) = EH00(h)*gdpindex(t);

* NGOs
QNGO00(c,insngo)$SAM(c,insngo) = SAM(c,insngo)/PQD00(c,insngo);
QNGO0(c,insngo,t)$tsol(t) = QNGO00(c,insngo)*gdpindex(t);
qngob00(c,insngo) = QNGO00(c,insngo);
qngob0(c,insngo,t) = QNGO0(c,insngo,t);
qngob(c,insngo,t) = qngob0(c,insngo,t);
QNGOSCAL00(insngo)$SUM(c, SAM(c,insngo)) = 1;
QNGOSCAL0(insngo,t)$tsol(t) = QNGOSCAL00(insngo);

* Foreign Tourists
QTRST00(c,instrst)$SAM(c,instrst) = SAM(c,instrst)/PQD00(c,instrst);
QTRST0(c,instrst,t)$tsol(t) = QTRST00(c,instrst)*gdpindex(t);
qtrstb00(c,instrst) = QTRST00(c,instrst);
qtrstb0(c,instrst,t) = QTRST0(c,instrst,t);
qtrstb(c,instrst,t) = qtrstb0(c,instrst,t);
QTRSTSCAL00$SUM((c,instrst), SAM(c,instrst)) = 1;
QTRSTSCAL0(t)$tsol(t) = QTRSTSCAL00;
* tourism receipts (FCU)
TRSMREC00 = SUM((c,instrst), SAM(c,instrst))/EXR00;

*HL-Start
*TRSMREC0(t)$tsol(t) = TRSMREC00*gdpindex(t);
 TRSMREC0(t)$tsol(t) = TRSMREC00*ngovpayindex0('tourismrec',t);
DISPLAY TRSMREC0;
*HL-End


* Domesitc Non-Gov Inst

YI00(insdng) = SAM(insdng,'total');
YI0(insdng,t)$tsol(t) = YI00(insdng)*gdpindex(t);
TY00(insdng)  = SUM(taxdir, SAM(taxdir,insdng)) / YI00(insdng);
TY0(insdng,t)$tsol(t) = TY00(insdng);
tyb00(insdng) = TY00(insdng);
tyb0(insdng,t) = TY0(insdng,t);
tyb(insdng,t) = tyb0(insdng,t);
TYSCAL00 = 0;
TYSCAL0(t)$tsol(t) = TYSCAL00;
ty010(insdng,t)$tsol(t) = 1;

*HL20250510
*ty010('ent',t)$tsol(t) = 0;

ty01(insdng,t) = ty010(insdng,t);

DISPLAY QH0, EH0, YI0, TY0;

* Savings-Investment

QINV00(c)$SUM(inv, SAM(c,inv)) = SUM(inv$SAM(c,inv), SAM(c,inv)/SUM(fcap$mfcapinv(fcap,inv), PQD00(c,fcap)));
QINV0(c,t)$tsol(t) = QINV00(c)*gdpindex(t);

PARAMETER dstkcomp(c);
dstkcomp(c) = SUM(dstk, SAM(c,dstk)/SUM(cp, SAM(cp,dstk)));

qdstk00(c,'ngovz')$SUM(dstk, SAM(c,dstk))  = SUM(dstk, SUM(capinsng, SAM(dstk,capinsng))*dstkcomp(c)/PQD00(c,dstk));
qdstk00(c,'govz')$SUM(dstk, SAM(c,dstk))  = SUM(dstk, SUM(capgov, SAM(dstk,capgov))*dstkcomp(c)/PQD00(c,dstk));
qdstk0(c,ins2,t)$tsol(t) = qdstk00(c,ins2)*gdpindex(t);
qdstk(c,ins2,t) = qdstk0(c,ins2,t);

SAV00(insdng) = SUM(capinsdng$mcapins(capinsdng,insdng), SAM(capinsdng,insdng));
SAV0(insdng,t)$tsol(t) = SAV00(insdng)*gdpindex(t);

* sav: SAV = alpha_SAV*CPI + MPS*YI*(1-ty)

MPS00(insdng) = MPS000(insdng);

* calibracion con MPS00 NE 0
alpha_sav00(insdng)$(MPS00(insdng)) =
  (1/CPI00) * ( SAV00(insdng) - MPS00(insdng)*(YI00(insdng)*(1-TY00(insdng))) );

* calibracion con MPS00 EQ 0
MPS00(insdng)$(NOT MPS00(insdng)) = SAV00(insdng) / ( YI00(insdng)*(1-TY00(insdng)) );
MPS0(insdng,t)$tsol(t) = MPS00(insdng);

alpha_sav00(insdng)$(NOT MPS00(insdng)) = 0;
alpha_SAV(insdng,t) = alpha_sav00(insdng)*gdpindex(t);
DISPLAY MPS00, alpha_sav00;



*MPS00(insdng)   = SAV00(insdng) / ( (1 - TY00(insdng))*YI00(insdng) );
*MPS0(insdng,t)$tsol(t) = MPS00(insdng);
mpsb00(insdng) = MPS00(insdng);
mpsb0(insdng,t)$tsol(t) = mpsb00(insdng);
mpsb(insdng,t) = mpsb0(insdng,t);
dmpsb0(ins,t) = 0;
dmpsb(ins,t) = dmpsb0(ins,t);
MPSSCAL00        = 1;
MPSSCAL0(t)$tsol(t) = MPSSCAL00;
MPSADJ00 = 0;
MPSADJ0(t)$tsol(t) = MPSADJ00;
DISPLAY QINV0, qdstk, MPS0, SAV0;
savadj01(insdng)$(NOT SUM(insdngp, savadj01(insdngp))) = 1;
DISPLAY savadj01;


* foreign savings
SAVF00 = SUM((caprow,insrow), SAM(caprow,insrow)) / EXR00;
SAVF0(t)$tsol(t) = SAVF00*gdpindex(t);

* Government

* total tax collection

YTAXIMP00             = SUM((insgov,taximp), SAM(insgov,taximp));
YTAXIMP0(t)$tsol(t)   = YTAXIMP00*gdpindex(t);

YTAXEXP00             = SUM((insgov,taxexp), SAM(insgov,taxexp));
YTAXEXP0(t)$tsol(t)   = YTAXEXP00*gdpindex(t);
DISPLAY YTAXIMP0, YTAXEXP0;

YTAXVAT00 = SUM((insgov,taxvatc), SAM(insgov,taxvatc));
YTAXVAT0(t)$tsol(t) = YTAXVAT00*gdpindex(t);

SUBCT00 = -SUM((insgov,subcom), SAM(insgov,subcom));
SUBCT0(t)$tsol(t) = SUBCT00*gdpindex(t);



EG00 = SUM(insgov, SAM('total',insgov) - SUM(capgov, SAM(capgov,insgov))) - SUM((insgov,subcom), SAM(insgov,subcom));
EG0(t)$tsol(t) = EG00*gdpindex(t);

INVVALG00 = SUM((inv,capgov), SAM(inv,capgov)) + SUM((dstk,capgov), SAM(dstk,capgov));
INVVALG0(t)$tsol(t) = INVVALG00*gdpindex(t);

YG00 = SUM(insgov, SAM(insgov,'total')) - SUM((insgov,subcom), SAM(insgov,subcom));
YG0(t)$tsol(t) = YG00*gdpindex(t);

GPRIMDEF00 = EG00 + INVVALG00 - YG00;
GPRIMDEF0(t)$tsol(t) = GPRIMDEF00*gdpindex(t);

RGPRIMDEF00 = GPRIMDEF00/CPI00;
RGPRIMDEF0(t)$tsol(t) = GPRIMDEF0(t)/CPI0(t);

QG00(c)$SUM(insgov, SAM(c,insgov)) = SUM(insgov, SAM(c,insgov)/PQD00(c,insgov));
QG0(c,t)$tsol(t) = QG00(c)*gdpindex(t);

qgb00(c) = QG00(c);
qgb0(c,t)$tsol(t) = qgb00(c)*gdpindex(t);
qgb(c,t) = qgb0(c,t);
qgc010(c,t)$(NOT SUM(cp, qgc010(c,t)) AND QG00(c)) = 1;
qgc01(c,t) = qgc010(c,t);
dqg0(c,t) = 0;
dqg(c,t) = dqg0(c,t);

QGSCAL00 = 0;
QGSCAL0(t)$tsol(t) = QGSCAL00;
DISPLAY EG0, YG0, QG0;


* Transfers

TRNSFR00(insdng,insgov) = SAM(insdng,insgov) / CPI00;
TRNSFR00(insrow,insgov)  = SAM(insrow,insgov) / EXR00;
TRNSFR00(insd,insrow)   = SAM(insd,insrow) / EXR00;
TRNSFR00(f,insrow)      = SAM(f,insrow) / EXR00;
*!! to del
*TRNSFR00(insrow,f)      = SAM(insrow,f) / EXR00;
TRNSFR0(ac,acp,t)$tsol(t) = TRNSFR00(ac,acp)*gdpindex(t);
trnsfrb00(ac,acp) = TRNSFR00(ac,acp);
trnsfrb0(ac,acp,t) = TRNSFR0(ac,acp,t);
trnsfrb(ac,acp,t) = trnsfrb0(ac,acp,t);

dtrnsfr0(ac,ins,t) = 0;
dtrnsfr(ac,ins,t) = dtrnsfr0(ac,ins,t);

trnsfrpcb00(h,insgov) = TRNSFR00(h,insgov)/pop00(h);
trnsfrpcb0(h,insgov,t)$tsol(t) = trnsfrpcb00(h,insgov);

trnsfrpcb00(h,insrow) = TRNSFR00(h,insrow)/pop00(h);
trnsfrpcb0(h,insrow,t)$tsol(t) = trnsfrpcb00(h,insrow);

* overwrite for dmod=2
trnsfrpcb0(h,ins,t)$(tsol(t) AND dmod=2) = trnsfrpcb00(h,ins)*prdindex(t);

trnsfrpcb(h,ins,t) = trnsfrpcb0(h,ins,t);



TRNSFRSCAL00(actrnsfr) = 1;
TRNSFRSCAL0(actrnsfr,t)$tsol(t) = TRNSFRSCAL00(actrnsfr);

TRII00(ins,insdng) = SAM(ins,insdng);
TRII0(ins,insdng,t)$tsol(t) = TRII00(ins,insdng)*gdpindex(t);
shii00(ins,insdng)$TRII00(ins,insdng)  = TRII00(ins,insdng) / (YI00(insdng)*(1-TY00(insdng)) - SAV00(insdng));
shii0(ins,insdng,t)$tsol(t) = shii00(ins,insdng);
shii(ins,insdng,t) = shii0(ins,insdng,t);
DISPLAY TRNSFR0, TRII0, shii;

* start: scale shii for hoseholds-ent based on population growth

shii0(h,insdngnh,t) = shii00(h,insdngnh)*pop(h,t)/pop00(h);
* re-scale
shii0(h,insdngnh,t)$tsol(t) = shii0(h,insdngnh,t)/SUM(hp, shii0(hp,insdngnh,t));
shii(h,insdngnh,t) = shii0(h,insdngnh,t);

PARAMETER
  gapshii(insd,t)
;
gapshii(insdngnh,t)$tsol(t) = 1 - SUM(h, shii0(h,insdngnh,t));

* end: scale shii for hoseholds-ent based on population growth



* International Trade

* start: import quota

qmbarindex0(c,t)$(cmbar(c) AND SUM(cp$cmbar(cp), qmbarindex0(cp,t)) EQ 0) = 1;
qmbarindex(c,t)$cmbar(c) = qmbarindex0(c,t);

qmbar00(c)$cmbar(c) = QM00(c);
qmbar0(c,t)$tsol(t) = qmbar00(c)*qmbarindex(c,t);
qmbar(c,t) = qmbar0(c,t);

PRQMBAR00(c)$SAM('prqmbar',c) = SAM('prqmbar',c) / SAM('row',c);
PRQMBAR0(c,t)$tsol(t) = PRQMBAR00(c);

YPRQMBART00(c) = SAM('prqmbar',c);
YPRQMBART0(c,t)$tsol(t) = YPRQMBART00(c);

* end: import quota


TE00(c)$(SUM(insrow, SAM(c,insrow))) = SUM(taxexp, SAM(taxexp,c)) / SUM(insrow, SAM(c,insrow));
TE0(c,t)$tsol(t) = TE00(c);
teb(c,t) = TE0(c,t);
teb00(c) = TE00(c);
teb0(c,t) = teb(c,t);

TESCAL00 = 1;
TESCAL0(t)$tsol(t) = 1;
PWE00(c) = ( PE00(c) + SUM((ct,tace), PQD00(ct,tace)*ice(ct,c)) ) / ( (1 - TE00(c))*( (1-shroe00(c))*EXR00*PREXR00 + shroe00(c)*EXR00 ) );
PWE0(c,t)$tsol(t) = PWE00(c);

PARAMETER
  gapexp(c)
;
gapexp(c) = PWE00(c)*EXR00*QE00(c) - SAM(c,'row');


* constant elasticity export demand function

qeb00(c)$(ced(c) OR cesexog(c)) = QE00(c);
qeb0(c,t)$tsol(t) = qeb00(c)*gdpindex(t);;
qeb(c,t) = qeb0(c,t);

pwse00(c)$ced(c) = PWE00(c);
pwse(c,t)$tsol(t) = pwse00(c);
pwse0(c,t) = pwse(c,t);

eta_e(c) = tradelas(c,'eta_e');


* under ss assumption, pwe does not change
pweindex(c,t)$(dmod=2) = 0;


*HL-Start
*Some diagnostic may be needed to make sure that, if data is provided for one t,
*then data has been provided for all t.
PWE0(c,t)$(tsol(t) AND SUM(tp, pweindex(c,tp))) = PWE00(c)*pweindex(c,t);
*HL-End

*!! note yprexrtm in the formula for tm00
TM00(c)$(SUM(insrow, SAM(insrow,c))) = SUM(taximp, SAM(taximp,c)) / [SUM(insrow, SAM(insrow,c)) + yprexrtm(c)];
TM0(c,t)$tsol(t) = TM00(c);
tmb(c,t) = TM0(c,t);
tmb00(c) = TM00(c);
tmb0(c,t) = tmb(c,t);
TMSCAL00 = 1;
TMSCAL0(t)$tsol(t) = 1;
*PWM00(c) = ( PM00(c) - SUM((ct,tacm), PQD00(ct,tacm)*icm(ct,c)) ) / ( (1 + TM00(c) + PRQMBAR00(c))*EXR00 );
PWM00(c) = ( PM00(c) - SUM((ct,tacm), PQD00(ct,tacm)*icm(ct,c)) ) / ( (1 + TM00(c) + PRQMBAR00(c)) * ( (1-shrom00(c))*EXR00*PREXR00 + shrom00(c)*EXR00 ) );
PWM0(c,t)$tsol(t) = pwm00(c);

PARAMETER
  gapimp(c)
;
gapimp(c) = PWM00(c)*EXR00*QM00(c) - SAM('row',c);


* unser ss assumption, pwm does not change
pwmindex(c,t)$(dmod=2) = 0;
*HL-Start
*Some diagnostic may be needed to make sure that, if data is provided for one t,
*then data has been provided for all t.
 pwm0(c,t)$(tsol(t) AND SUM(tp, pwmindex(c,tp))) = pwm00(c)*pwmindex(c,t);
*HL-End


DISPLAY TE0, pwe0, TM0, pwm0;


* Dynamics

kappa = dinam('kappa');

WFAVG00(f)$(fsam(f) AND fva(f)) = SUM(a, QF00(f,a)*WFA00(f,a)) / SUM(ap, QF00(f,ap));
WFAVG0(f,t)$tsol(t) = WFAVG00(f)*fprdindex(f,t);
DISPLAY WFAVG0;

capcomp(fcap,c)$SUM(inv$mfcapinv(fcap,inv), SAM(c,inv)) =
  SUM(inv$mfcapinv(fcap,inv), SAM(c,inv)/PQD00(c,fcap) / ( SUM(cp$SAM(cp,inv), SAM(cp,inv)/PQD00(cp,fcap)) ));
DISPLAY capcomp;

PK00(fcap) = SUM(c, capcomp(fcap,c)*PQD00(c,fcap));
PK0(fcap,t)$tsol(t) = PK00(fcap);

DKINS00('ngovz',fcap) = SUM(inv$mfcapinv(fcap,inv), SUM(capinsdng, SAM(inv,capinsdng)))/PK00(fcap);
DKINS00('govz',fcap) =  SUM(inv$mfcapinv(fcap,inv), SUM(capgov, SAM(inv,capgov)))/PK00(fcap);
DKINS00('rowz',fcap) =  SUM(inv$mfcapinv(fcap,inv), SUM(caprow, SAM(inv,caprow)))/PK00(fcap);

DKINS0(ins2,fcap,t)$tsol(t) = DKINS00(ins2,fcap)*gdpindex(t);


dkinsb0(ins2,fcap,t) = DKINS0(ins2,fcap,t);
dkinsb(ins2,fcap,t) = dkinsb0(ins2,fcap,t);
ddkins0(ins2,fcap,t) = 0;
ddkins(ins2,fcap,t) = ddkins0(ins2,fcap,t);

PARAMETER dkinsgap(ins2,f);
dkinsgap('ngovz',fcap) = DKINS00('ngovz',fcap) - SUM(inv$mfcapinv(fcap,inv), SUM(capinsng, SAM(inv,capinsng)/PK00(fcap)));
dkinsgap('govz',fcap) = DKINS00('govz',fcap) - SUM(inv$mfcapinv(fcap,inv), SUM(capgov, SAM(inv,capgov)/PK00(fcap)));
DISPLAY dkinsgap;



INVVALF00 = SUM(invng, SUM(caprow, SAM(invng,caprow)))/EXR00;

*HL-Start

*INVVALF0(t)$tsol(t) = invvalf00*gdpindex(t);
 INVVALF0(tmin)      = invvalf00;

LOOP(t$(tsol(t) AND (NOT tmin(t))),
 INVVALF0(t)$tsol(t) = invvalf0(t-1)*(1 + ngovpaygrw0('fdi',t)); );
DISPLAY INVVALF0;

*HL-End


invvalfb0(t) = INVVALF0(t);
invvalfb(t) = invvalfb0(t);
FDISCAL00 = 1;
FDISCAL0(t)$tsol(t) = FDISCAL00;

invshr00(fcap,'rowz') = SUM(inv$mfcapinv(fcap,inv), SUM(caprow, SAM(inv,caprow)));
invshr00(fcap,ins2)$SUM(fcapp, invshr00(fcapp,ins2)) = invshr00(fcap,ins2)/SUM(fcapp, invshr00(fcapp,ins2));
*!! not used
* default invshr00
*invshr00(fcapng,'rowz')$(SUM(fcapp, invshr00(fcapp,'rowz'))=0) = 1/CARD(fcapng);
invshr(fcap,ins2,t) = invshr00(fcap,ins2);

ISCAL00(fcap)      = 1;
ISCAL0(fcap,t)$tsol(t) = ISCAL00(fcap);
IADJ00(ins2)  = 0;
IADJ0(ins2,t)$tsol(t) = IADJ00(ins2);
iadj010(fcap)$(NOT SUM(fcapp, iadj010(fcapp))) = 1;
iadj01(fcap) = iadj010(fcap);


* domestic non-government investment
INVVAL00 = SUM(inv, SUM(capinsdng, SAM(inv,capinsdng)))
  + SUM(dstk, SUM(capinsdng, SAM(dstk,capinsdng)));
INVVAL0(t)$tsol(t) = INVVAL00*gdpindex(t);



* investment by destination invng
DKA00(fcapng,a) = SUM(ins2, DKINS00(ins2,fcapng)) * QF00(fcapng,a)/SUM(ap, QF00(fcapng,ap));
DKA0(fcapng,a,t) = DKA00(fcapng,a)*gdpindex(t);


* National Accounts Macro Aggregates

* real gdp factor cost
RGDPFC00
  = SUM(a, PVA00(a)*QA00(a))
     + SUM((f,a)$fleo(f), WFA00(f,a)*QF00(f,a));
RGDPFC0(t)$tsol(t) = RGDPFC00*gdpindex(t);
DISPLAY RGDPFC0, RGDPFC00, gdpindex;
TFPSCAL00 = 0;
TFPSCAL0(t)$tsol(t) = TFPSCAL00;
TFP00(a) = 1;
TFP0(a,t)$tsol(t) = TFP00(a);
tfpexog0(a,t)$(tsol(t) AND NOT SUM((ap,tp), tfpexog0(ap,tp))) = 1;
tfpexog(a,t) = tfpexog0(a,t);
DISPLAY tfpexog, tfpexog0;
tfp010(a)$(NOT SUM(ap, tfp010(ap))) = 1;
tfp01(a) = tfp010(a);

FPRDASCAL00 = 0;
FPRDASCAL0(t)$tsol(t) = FPRDASCAL00;
fprda01(f,a)$flab(f) = 1;

GDPMP00 = SUM((c,h), PQD00(c,h)*QH00(c,h))
  + SUM((c,insngo), PQD00(c,insngo)*QNGO00(c,insngo))
  + SUM((c,fcapng), PQD00(c,fcapng)*capcomp(fcapng,c)*SUM(ins2, DKINS00(ins2,fcapng)))
  + SUM((c,fcapg), PQD00(c,fcapg)*capcomp(fcapg,c)*SUM(ins2, DKINS00(ins2,fcapg)))
  + SUM((c,dstk), PQD00(c,dstk)*SUM(ins2, qdstk00(c,ins2)))
  + SUM((c,insgov), PQD00(c,insgov)*QG00(c))
  + SUM(c, EXR00*pwe00(c)*QE00(c))
  + SUM((c,instrst), PQD00(c,instrst)*QTRST00(c,instrst))
  - SUM(c, EXR00*pwm00(c)*QM00(c));
GDPMP0(t)$tsol(t) = GDPMP00*gdpindex(t);
DISPLAY GDPMP0;

RGDPMP00 = GDPMP00;
RGDPMP0(t)$tsol(t) = RGDPMP00*gdpindex(t);

TRDGDP00 = ( SUM(c, EXR00*pwe00(c)*QE00(c)) + SUM(c, EXR00*pwm00(c)*QM00(c)) ) / RGDPMP00;
TRDGDP0(t)$tsol(t) = TRDGDP00;

ABSNOM00 = SUM((c,h), PQD00(c,h)*QH00(c,h))
  + SUM((c,insngo), PQD00(c,insngo)*QNGO00(c,insngo))
  + SUM((c,fcapng), PQD00(c,fcapng)*capcomp(fcapng,c)*SUM(ins2, DKINS00(ins2,fcapng)))
  + SUM((c,fcapg), PQD00(c,fcapg)*capcomp(fcapg,c)*SUM(ins2, DKINS00(ins2,fcapg)))
  + SUM((c,dstk), PQD00(c,dstk)*SUM(ins2,qdstk00(c,ins2)))
  + SUM((c,insgov), PQD00(c,insgov)*QG00(c));
ABSNOM0(t)$tsol(t) = ABSNOM00*gdpindex(t);
DISPLAY ABSNOM0;

* Government Net Domestic Financing

NDFG00 = SUM((capgov,capinsd), SAM(capgov,capinsd));
NDFG0(t)$tsol(t) = NDFG00*gdpindex(t);

RNDFG00 = NDFG00/CPI00;
RNDFG0(t)$tsol(t) =  NDFG0(t)/CPI0(t);

* Foreign Financing

NFFG00 = SUM((capgov,caprow), SAM(capgov,caprow));
NFFG0(t)$tsol(t) = NFFG00*gdpindex(t);
DISPLAY NFFG0;

PARAMETER diff;
diff = GPRIMDEF00 - NDFG00 - EXR00*NFFG00;
DISPLAY diff;


NFFINS00 = SUM((capinsdng,caprow), SAM(capinsdng,caprow));
NFFINS0(t)$tsol(t) = NFFINS00*gdpindex(t);
nffinsbar00 = NFFINS00;
nffinsbar0(t) = NFFINS0(t);
nffinsbar(t) = nffinsbar0(t);
DISPLAY NFFINS0;

NFFINSSCAL00 = 1;
NFFINSSCAL0(t)$tsol(t) = NFFINSSCAL00;


* Change in Foreign Reserves

drf00 = SUM((caprow,capfin), SAM(caprow,capfin));
drf0(t) = drf00*gdpindex(t);
drf(t) = drf0(t);

diff = SAVF00 - NFFINS00*EXR00 - NFFG00*EXR00 - invvalf00*EXR00 + drf00;
DISPLAY diff;


* Investment by Institution

PARAMETER
  savshr(ins)     share of ins in savings
  ndfgshr(ins)    share of ins in ndfg
  ndfggap(ins)
  gbadj00(ins)    adjustment factor for government borrowing by ins
  gbadj(ins,t)    adjustment factor for government borrowing by ins

  drfadj00(ins)   adjustment factor for foreign reserves
  drfadj(ins,t)   adjustment factor for foreign reserves
;

ndfgshr(insdng)$SUM((capgov,capinsdngp), SAM(capgov,capinsdngp)) =
  SUM(capgov, SUM(capinsdng$mcapins(capinsdng,insdng), SAM(capgov,capinsdng))) / SUM((capgov,capinsdngp), SAM(capgov,capinsdngp));
DISPLAY ndfgshr;
savshr(insdng) = SUM(capinsdng$mcapins(capinsdng,insdng), SAM(capinsdng,insdng)) / SUM((capinsdng,insdngp), SAM(capinsdng,insdngp));
DISPLAY savshr;

gbadj00(insdng)$savshr(insdng) = ndfgshr(insdng)/savshr(insdng);
gbadj(insdng,t) = gbadj00(insdng);
gbadj(insdng,t)$(NOT dmod=2 AND NOT tmin(t) AND tsol(t)) = 1;
ndfggap(insdng) = NDFG00*savshr(insdng)*gbadj00(insdng) - SUM(capgov, SUM(capinsdng$mcapins(capinsdng,insdng), SAM(capgov,capinsdng)));
DISPLAY gbadj00, ndfggap;

drfadj00(insdng)$SUM((capfin,capinsdngp), SAM(capfin,capinsdngp)) =
  SUM(capfin, SUM(capinsdng$mcapins(capinsdng,insdng), SAM(capfin,capinsdng))) / SUM((capfin,capinsdngp), SAM(capfin,capinsdngp));
drfadj00(insdng)$savshr(insdng) = drfadj00(insdng)/savshr(insdng);
drfadj(insdng,t) = drfadj00(insdng);
drfadj(insdng,t)$(NOT dmod=2 AND NOT tmin(t) AND tsol(t)) = 1;
DISPLAY drfadj00;


PARAMETER gapinvest;
gapinvest = INVVAL00
  - [ SUM(insdng, SAV00(insdng))
  + NFFINS00*EXR00
  - (NDFG00 + drf00) ];
DISPLAY gapinvest;



* Shares of GDP

* Government Receipts

GOVRECGDP00(taxact)  = SUM(a, SAM(taxact,a));
GOVRECGDP00(taxcom)  = SUM(c, SAM(taxcom,c));
GOVRECGDP00(taxdir)  = SUM(insdng, SAM(taxdir,insdng));
GOVRECGDP00(taximp)  = SUM(c, SAM(taximp,c));
GOVRECGDP00(taxexp)  = SUM(c, SAM(taxexp,c));
GOVRECGDP00(taxfac)  = SUM(f, SAM(taxfac,f));
GOVRECGDP00(taxvatc) = SUM(c, SAM(taxvatc,c));
GOVRECGDP00(trgovngov) = SUM((insgov,insdng), SAM(insgov,insdng));
GOVRECGDP00(trgovrow) = SUM(insgov, SAM(insgov,'row'));
GOVRECGDP00(netdomfin) = NDFG00;
GOVRECGDP00(netforfingov) = EXR00*NFFG00;

GOVRECGDP00(acgovrec) = GOVRECGDP00(acgovrec)/GDPMP00;

* dmod=2, overwrite with zeros
GOVRECGDP0(acgovrec,t)$(dmod EQ 2) = 0;

* rescale data in GOVRECGDP0 excluding tmin
GOVRECGDP0(acgovrec,t)$(tsol(t) AND GOVRECGDP0(acgovrec,t) AND SUM(tmin, GOVRECGDP0(acgovrec,tmin))) =
  GOVRECGDP0(acgovrec,t)*GOVRECGDP00(acgovrec)/SUM(tmin, GOVRECGDP0(acgovrec,tmin));
* when GOVRECGDP0 is empty, impose baseyr values
GOVRECGDP0(acgovrec,t)$(tsol(t) AND NOT GOVRECGDP0(acgovrec,t)) = GOVRECGDP00(acgovrec);


* Government Spending

GOVSPNDGDP00(fcapg) = SUM(c, SUM(invg$mfcapinv(fcapg,invg), SAM(c,invg)));
GOVSPNDGDP00(congov) = SUM((c,insgov), SAM(c,insgov));
GOVSPNDGDP00(trngovgov) = SUM((insdng,insgov), SAM(insdng,insgov));
GOVSPNDGDP00(trrowgov) = SUM((insrow,insgov), SAM(insrow,insgov));
GOVSPNDGDP00(subcom) = -SUM(c, SAM(subcom,c));

GOVSPNDGDP00(acgovspnd) = GOVSPNDGDP00(acgovspnd)/GDPMP00;

* dmod=2, overwrite with zeros
GOVSPNDGDP0(acgovspnd,t)$(dmod EQ 2) = 0;

* rescale data in GOVSPNDGDP0 excluding tmin
GOVSPNDGDP0(acgovspnd,t)$(tsol(t) AND GOVSPNDGDP0(acgovspnd,t) AND SUM(tmin, GOVSPNDGDP0(acgovspnd,tmin))) =
  GOVSPNDGDP0(acgovspnd,t)*GOVSPNDGDP00(acgovspnd)/SUM(tmin, GOVSPNDGDP0(acgovspnd,tmin));
* when GOVSPNDGDP0 is empty, impose baseyr values
GOVSPNDGDP0(acgovspnd,t)$(tsol(t) AND NOT GOVSPNDGDP0(acgovspnd,t)) = GOVSPNDGDP00(acgovspnd);


* Non-Government Payments

NGOVPAYGDP00(trngovrow) = SUM((insdng,insrow), SAM(insdng,insrow));
NGOVPAYGDP00(trrowngov) = SUM((insrow,insdng), SAM(insrow,insdng));
NGOVPAYGDP00(trfacrow) = SUM((f,insrow), SAM(f,insrow));
NGOVPAYGDP00(trrowfac) = SUM((insrow,f), SAM(insrow,f));
NGOVPAYGDP00(savngov) = SUM((capinsdng,insdng), SAM(capinsdng,insdng));
NGOVPAYGDP00(netforfinngov) = EXR00*NFFINS00;
NGOVPAYGDP00(fcapng) = SUM(invng$mfcapinv(fcapng,invng), SUM(capinsdng, SAM(invng,capinsdng)));
NGOVPAYGDP00(fdi) = SUM((invng,caprow), SAM(invng,caprow));
NGOVPAYGDP00(tourismrec) = SUM((c,instrst), SAM(c,instrst));
NGOVPAYGDP00(acngovpay) = NGOVPAYGDP00(acngovpay)/GDPMP00;

* dmod=2, overwrite with zeros
NGOVPAYGDP0(acngovpay,t)$(dmod EQ 2) = 0;


* rescale data in NGOVPAYGDP0 excluding tmin
NGOVPAYGDP0(acngovpay,t)$(tsol(t) AND NGOVPAYGDP0(acngovpay,t) AND SUM(tmin, NGOVPAYGDP0(acngovpay,tmin))) =
  NGOVPAYGDP0(acngovpay,t)*NGOVPAYGDP00(acngovpay)/SUM(tmin, NGOVPAYGDP0(acngovpay,tmin));
* when NGOVPAYGDP0 is empty, impose baseyr values
NGOVPAYGDP0(acngovpay,t)$(tsol(t) AND NOT NGOVPAYGDP0(acngovpay,t)) = NGOVPAYGDP00(acngovpay);


* Shares of Absorption

* Government Receipts

GOVRECABS00(acgovrec) = GOVRECGDP00(acgovrec)/GDPMP00*ABSNOM00;
* dmod=2, overwrite with zeros
GOVRECABS0(acgovrec,t)$(dmod EQ 2) = 0;

* rescale data in GOVRECABS0 excluding tmin
GOVRECABS0(acgovrec,t)$(tsol(t) AND GOVRECABS0(acgovrec,t) AND SUM(tmin, GOVRECABS0(acgovrec,tmin))) =
  GOVRECABS0(acgovrec,t)*GOVRECGDP00(acgovrec)/SUM(tmin, GOVRECABS0(acgovrec,tmin));
* when GOVRECABS0 is empty, impose baseyr values
GOVRECABS0(acgovrec,t)$(tsol(t) AND NOT GOVRECABS0(acgovrec,t)) = GOVRECABS00(acgovrec);

* Government Spending

GOVSPNDABS00(acgovspnd) = GOVSPNDGDP00(acgovspnd)/GDPMP00*ABSNOM00;

* if dmod=2, overwrite with zeros
GOVSPNDABS0(acgovspnd,t)$(dmod EQ 2) = 0;

* rescale data in GOVSPNDABS0 excluding tmin
GOVSPNDABS0(acgovspnd,t)$(tsol(t) AND GOVSPNDABS0(acgovspnd,t) AND SUM(tmin, GOVSPNDABS0(acgovspnd,tmin))) =
  GOVSPNDABS0(acgovspnd,t)*GOVSPNDGDP00(acgovspnd)/SUM(tmin, GOVSPNDABS0(acgovspnd,tmin));
* when GOVSPNDABS0 is empty, impose baseyr values
GOVSPNDABS0(acgovspnd,t)$(tsol(t) AND NOT GOVSPNDABS0(acgovspnd,t)) = GOVSPNDABS00(acgovspnd);
* if dmod=2, overwrite using baseyr for all simulation periods
GOVSPNDABS0(acgovspnd,t)$(dmod EQ 2 AND tsol(t)) = GOVSPNDABS00(acgovspnd);


* Non-Government Payments

NGOVPAYABS00(acngovpay) = NGOVPAYGDP00(acngovpay)/GDPMP00*ABSNOM00;

* dmod=2, overwrite with zeros
NGOVPAYABS0(acngovpay,t)$(dmod=2) = 0;

* rescale data in NGOVPAYABS0 excluding tmin
NGOVPAYABS0(acngovpay,t)$(tsol(t) AND NGOVPAYABS0(acngovpay,t) AND SUM(tmin, NGOVPAYABS0(acngovpay,tmin))) =
  NGOVPAYABS0(acngovpay,t)*NGOVPAYABS00(acngovpay)/SUM(tmin, NGOVPAYABS0(acngovpay,tmin));
* when NGOVPAYABS0 is empty, impose baseyr values
NGOVPAYABS0(acngovpay,t)$(tsol(t) AND NOT NGOVPAYABS0(acngovpay,t)) = NGOVPAYABS00(acngovpay);


* Debt Stocks

gintrat00 = SUM(tmin, gintrat0(tmin));
gintrat(t)$((dmod=0 OR dmod=1) AND tsol(t)) = gintrat0(t);
gintrat(t)$(dmod=2) = gintrat00;
fintrat00(ins2) = SUM(tmin, fintrat0(ins2,tmin));
fintrat(ins2,t)$((dmod=0 OR dmod=1) AND tsol(t)) = fintrat0(ins2,t);
fintrat(ins2,t)$(dmod=2) = fintrat00(ins2);

* dmod NE 2
GDEBT00$(dmod=0 OR dmod=1) = debt00('ngovz','govz');
DISPLAY GDEBT00;
FDEBT00('ngovz')$(dmod=0 OR dmod=1) = debt00('rowz','ngovz');
FDEBT00('govz')$(dmod=0 OR dmod=1) = debt00('rowz','govz');
DISPLAY FDEBT00;


* in case dmod=2, overwrite debt00
$ONTEXT

steady state implies that
  GDEBT = (NDFG - intrat*GDEBT)/ssgrw
  GDEBT = NDFG/ssgrw - intrat*GDEBT/ssgrw
  GDEBT*(1+intrat/ssgrw) = NDFG/ssgrw
  GDEBT*(ssgrw+intrat)/ssgrw = NDFG/ssgrw
  GDEBT = NDFG/(ssgrw+intrat)

$OFFTEXT

GDEBT00$(dmod=2) = NDFG00/(ssgrw('gdp') - gintrat00);
DISPLAY GDEBT00;
FDEBT00('govz')$(dmod=2) = NFFG00/(ssgrw('gdp') - fintrat00('govz'));
FDEBT00('ngovz')$(dmod=2) = NFFINS00/(ssgrw('gdp') - fintrat00('ngovz'));
DISPLAY FDEBT00;

GDEBT0(t)$tsol(t) = GDEBT00*gdpindex(t);
FDEBT0(ins2,t)$tsol(t) = FDEBT00(ins2)*gdpindex(t);

GBOR00 = NDFG00 + gintrat00*GDEBT00;
GBOR0(t)$tsol(t) = GBOR00*gdpindex(t);
DISPLAY GBOR0;

FBOR00('govz') = NFFG00 + fintrat00('govz')*FDEBT00('govz');
FBOR00('ngovz') = NFFINS00 + fintrat00('ngovz')*FDEBT00('ngovz');
FBOR0(ins2,t)$tsol(t) = FBOR00(ins2)*gdpindex(t);
DISPLAY FBOR0;



* activate the following sentence to debug
*$EXIT


*### Calibration Production and Consumption

* Production

*theta(a,c) = ( SAM(a,c) / PX00(c) ) / QA00(a);
theta(a,c) = QXAC00(a,c) / QA00(a);

* aggregation of domestic output from different activities

sigma_ac(c) = 2;
rho_ac(c)   = 1/sigma_ac(c) - 1;
delta_ac(a,c)$QXAC00(a,c) = QXAC00(a,c)**(1/sigma_ac(c)) * PXAC00(a,c) /
  ( SUM(ap, QXAC00(ap,c)**(1/sigma_ac(c)) * PXAC00(ap,c)) );
phi_ac(c)$QX00(c) = QX00(c) / SUM(a, delta_ac(a,c) * QXAC00(a,c)**(-rho_ac(c))) ** (-1/rho_ac(c));
DISPLAY delta_ac, phi_ac;


* Value Added

* Level 1

* factors in CES
sigma_va(a) = prodelas(a);
rho_va(a)   = 1/sigma_va(a) - 1;
* MC-2018-10-08
* note the use of fleo(f) and NOT fleo(f)
delta_va(f,a)$(f1(f) AND NOT fleo(f) AND QF00(f,a)) = QF00(f,a)**(1/sigma_va(a))*WFA00(f,a) /
  ( SUM(fp$(f1(fp) AND NOT fleo(fp)), QF00(fp,a)**(1/sigma_va(a))*WFA00(fp,a)) );
phi_va(a)$(QA00(a) AND SUM(f, SAM(f,a))) = QA00(a) / SUM(f$(f1(f) AND NOT fleo(f)), delta_va(f,a)*QF00(f,a)**(-rho_va(a))) ** (-1/rho_va(a));
phi_va0(a) = phi_va(a);

DISPLAY theta, delta_va, phi_va;


* factors in Leontief

* start: MC-2018-10-08

ifa0(f,a,t)$(fleo(f) AND QF00(f,a)) = QF00(f,a)/QA00(a);
ifa(f,a,t) = ifa0(f,a,t);
DISPLAY ifa;

* end: MC-2018-10-08



* start: check factor demands level 1

PARAMETER qfchk(f,a), qfgap(f,a);
qfchk(f,a)$(NOT fleo(f) AND f1(f) AND QF00(f,a)) = ( PVA00(a)/WFA00(f,a) )**sigma_va(a) *
  delta_va(f,a)**sigma_va(a) * (TFP00(a)*phi_va(a))**(sigma_va(a)-1) * QA00(a) * FPRDA00(f,a)**(sigma_va(a)-1);


qfchk(f,a)$(fleo(f) AND SAM(f,a)) = SUM(tmin, ifa(f,a,tmin))*QA00(a);
qfgap(f,a)$f1(f) = QF00(f,a)-qfchk(f,a);

* end: check factor demands level 1


* Level 2

sigma2(f1,a) = prodelas2(a,f1);
rho2(f1,a)$(sigma2(f1,a)) = 1/sigma2(f1,a) - 1;
 
* production fn for factors at level 2 
LOOP((f1,a),
  delta2(f2,a)$(mf2f1(f2,f1) AND QF00(f2,a)) = QF00(f2,a)**(1/sigma2(f1,a))*WFA00(f2,a) /
    ( SUM(f2p, QF00(f2p,a)**(1/sigma2(f1,a))*WFA00(f2p,a)) );
);	
phi2(f1,a)$(fnsam(f1) AND QF00(f1,a)) = QF00(f1,a) / 
	SUM(f2$mf2f1(f2,f1), delta2(f2,a)*QF00(f2,a)**(-rho2(f1,a))) ** (-1/rho2(f1,a));
phi20(f1,a) = phi2(f1,a);

* start: check factor demands level 2

PARAMETER 
  qfchk2(f,a)
  qfgap2(f,a)
;
qfchk2(f1,a)$fnsam(f1) = phi2(f1,a) * 
    SUM(f2$mf2f1(f2,f1), delta2(f2,a)*QF00(f2,a)**(-rho2(f1,a))) ** (-1/rho2(f1,a));

qfchk2(f2,a)$QF00(f2,a) = (SUM(f1$mf2f1(f2,f1), WFA00(f1,a))/WFA00(f2,a))**SUM(f1$mf2f1(f2,f1), sigma2(f1,a)) * 
  delta2(f2,a)**SUM(f1$mf2f1(f2,f1), sigma2(f1,a)) * 
  SUM(f1$(mf2f1(f2,f1)), phi2(f1,a))**(SUM(f1$mf2f1(f2,f1), sigma2(f1,a))-1) * 
  SUM(f1$mf2f1(f2,f1), QF00(f1,a));

qfgap2(f,a)$f2(f) = QF00(f,a)-qfchk2(f,a);

DISPLAY qfgap2;  


* end: check factor demands level 2



* Level 3

sigma3(f2,a) = prodelas3(a,f2);
rho3(f2,a)$(sigma3(f2,a)) = 1/sigma3(f2,a) - 1;


* production fn for factors at level 3
LOOP((f2,a),
  delta3(f3,a)$(mf3f2(f3,f2) AND QF00(f3,a)) = QF00(f3,a)**(1/sigma3(f2,a))*WFA00(f3,a) /
    ( SUM(f3p$mf3f2(f3p,f2), QF00(f3p,a)**(1/sigma3(f2,a))*WFA00(f3p,a)) );
);

phi3(f2,a)$(fnsam(f2) AND QF00(f2,a)) = QF00(f2,a) / 
	SUM(f3$mf3f2(f3,f2), delta3(f3,a)*QF00(f3,a)**(-rho3(f2,a))) ** (-1/rho3(f2,a));
phi30(f2,a) = phi3(f2,a);

* start: check factor demands level 3

PARAMETER 
  qfchk3(f,a)
  qfgap3(f,a)
;
qfchk3(f2,a)$fnsam(f2) = phi3(f2,a) * 
    SUM(f3$mf3f2(f3,f2), delta3(f3,a)*QF00(f3,a)**(-rho3(f2,a))) ** (-1/rho3(f2,a));

qfchk3(f3,a)$QF00(f3,a) = (SUM(f2$mf3f2(f3,f2), WFA00(f2,a))/WFA00(f3,a))**SUM(f2$mf3f2(f3,f2), sigma3(f2,a)) * 
  delta3(f3,a)**SUM(f2$mf3f2(f3,f2), sigma3(f2,a)) * 
  SUM(f2$(mf3f2(f3,f2)), phi3(f2,a))**(SUM(f2$mf3f2(f3,f2), sigma3(f2,a))-1) * 
  SUM(f2$mf3f2(f3,f2), QF00(f2,a));

qfgap3(f,a)$f3(f) = QF00(f,a)-qfchk3(f,a);

DISPLAY qfgap3;  


* end: check factor demands level 3



* International Trade

sigma_x(c) = tradelas(c,'sigma_x');
sigma_q(c) = tradelas(c,'sigma_q');

rho_x(c)$sigma_x(c) = 1/sigma_x(c) + 1;
rho_q(c)$sigma_q(c) = 1/sigma_q(c) - 1;

* CET x

delta_e(c)$(QD00(c)>0 AND QE00(c)>0) = PE00(c) * QE00(c)**(-1/sigma_x(c)) /
  ( PE00(c) * QE00(c)**(-1/sigma_x(c)) + PDS00(c) * QD00(c)**(-1/sigma_x(c)) );

delta_ds(c)$(QD00(c)>0 AND QE00(c)>0) = PDS00(c) * QD00(c)**(-1/sigma_x(c)) /
  ( PE00(c) * QE00(c)**(-1/sigma_x(c)) + PDS00(c) * QD00(c)**(-1/sigma_x(c)) );

phi_x(c)$(QD00(c)>0 AND QE00(c)>0) = QX00(c) /
  ( delta_e(c)*QE00(c)**rho_x(c) + delta_ds(c)*QD00(c)**rho_x(c) ) ** (1/rho_x(c));
DISPLAY delta_e, delta_ds, phi_x;

scaldelta_ds(c,t)$delta_ds(c) = 1;
scaldelta_e(c,t)$delta_e(c)   = 1;
scalphi_x(c,t)$phi_x(c)       = 1;



* Armington q
delta_m(c)$(QD00(c)>0 AND QM00(c)>0) = PM00(c) * QM00(c)**(1/sigma_q(c)) /
  ( PM00(c) * QM00(c)**(1/sigma_q(c)) + PDD00(c) * QD00(c)**(1/sigma_q(c)) );

delta_dd(c)$(QD00(c)>0 AND QM00(c)>0) = PDD00(c) * QD00(c)**(1/sigma_q(c)) /
  ( PM00(c) * QM00(c)**(1/sigma_q(c)) + PDD00(c) * QD00(c)**(1/sigma_q(c)) );

phi_q(c)$(QD00(c)>0 AND QM00(c)>0) = QQ00(c) /
  ( delta_m(c)*QM00(c)**(-rho_q(c)) + delta_dd(c)*QD00(c)**(-rho_q(c)) ) ** (-1/rho_q(c));
DISPLAY delta_m, delta_dd, phi_q;



* Consumption

PARAMETERS
  budshr(c,h)    budget share for commodity c and household h
  budshrchk(h)   check que budget shares suman uno
  elaschk(h)     check que elasticidades-ingreso satisfacen agregacion engel
  leselas0(c,h)  raw expenditure elasticities (before adjustment)
;

budshr(c,h)  = PQD00(c,h)*QH00(c,h) / SUM(cp, PQD00(cp,h)*QH00(cp,h));
budshrchk(h) = SUM(c, budshr(c,h));
elaschk(h)   = SUM(c, budshr(c,h)*leselas(c,h));
DISPLAY budshr, budshrchk, elaschk;

leselas0(c,h) = leselas(c,h);
leselas(c,h) = leselas(c,h)/elaschk(h);
elaschk(h)   = sum(c, budshr(c,h)*leselas(c,h));
DISPLAY leselas, elaschk;

beta(c,h) = budshr(c,h)*leselas(c,h);
DISPLAY beta;

gamma00(c,h)$budshr(c,h) = QH00(c,h)
  + (beta(c,h)/PQD00(c,h)) * (EH00(h)/frisch(h));
gamma00(c,h) = gamma00(c,h)/pop00(h);
gamma(c,h,t)$(dmod=0 OR dmod=1) = gamma00(c,h);
* gamma is per capita minium consumption; thus, use prdindex instead of gdpindex
*gamma(c,h,t)$(dmod=2) = gamma00(c,h)*gdpindex(t);
gamma(c,h,t)$(dmod=2) = gamma00(c,h)*prdindex(t);

DISPLAY gamma;

* check parametros LES

PARAMETERS
  frisch2(h)          definicion alt. de Frisch -- ratio consumo y consumo supernumerario
  leschk(h)           check definicion parametros LES (mensaje error si error)
  supernum(h)         ingreso supernumerario LES

  leselasp(h,c,cp)    price elasticity bt c and cp for h (with c and cp labeled by source)
* leselasp defines cross-price elasticities when c is different from cp and
* own-price elasticities when c and cp refer to the same commodity
* Source: Dervis, de Melo and Robinson. 1982. General Equilibrium Models for
* Development Policy. Cambridge University Press, p. 483.
;

supernum(h) = EH00(h) - SUM(c, gamma00(c,h)*pop00(h)*PQD00(c,h));
frisch2(h)  = -EH00(h)/supernum(h);
leschk(h)$(ABS(frisch(h) - frisch2(h)) GT 0.00000001) = 1/0;
DISPLAY supernum, frisch, frisch2, leschk;

* cross-price elasticities

leselasp(h,c,cp)$((ORD(c) NE ORD(cp)) AND leselas(c,h) AND leselas(cp,h)) =
  -leselas(c,h) * PQD00(cp,h)*gamma00(cp,h)*pop00(h) / SUM(cpp, PQD00(cpp,h)*QH00(cpp,h));

* own-price elasticities

leselasp(h,c,c) =
  -leselas(c,h) * ( PQD00(c,h)*gamma00(c,h)*pop00(h) / SUM(cp, PQD00(cp,h)*QH00(cp,h)) - 1/frisch(h) );

DISPLAY leselasp;



* start: computation of tfpelas wrt infra
* based on MAMS treatment

$ONTEXT
*===========================================
NOTE. Just below:
elas = CAP*mpcapgov / GDP
 ==>
elas = mpcapgov / (GDP/CAP)
 ==>
elas = (dGDP/dCAP) / (GDP/CAP)
 i.e., the standard definition of elasticity.

*=====================

This is a brief explanation with simplified notation (inter alia
suppressing the subscript for the capital stock, f, that causes the TFP
(GDP) increase) of the computations that are done below:
  mpcapgova(a)        = tfp01a(a)*mpcapgov*[gdp(a)/gdp('total')];
*[MP of gov cap in a] = [scaling factor]*[mp PEDISR unit of cap]*[gdp shr for a]

Scaling initial values so that their sum will equal mpcapgov (needed unless the
scaling factor is 1 for all activities, in which case it leaves outcome unchanged):
  mpcapgova(a) = mpcapgova(a)*mpcapgov/SUM(ap, mpcapgova(ap));

Computing elasticity by activity
  elasa(a)      = cap*mpcapgova(a) / gdp(a);

Verifying that the aggregate change in GDP per unit of extra capital in the
base year is as intended given the above elasticity definition
(which is rearranged and summed over a):
  mpcapgov = SUM(a, elasa(a)*gdp(a))/cap;

Digression (showing that, by definition the preceding equation should hold:
 mpcapgov = SUM(a, elasa(a)*gdp(a))/cap ==>
 mpcapgov = SUM(a, (cap*mpcapgova(a)/gdp(a)) * gdp(a))/cap ==>
 mpcapgov = (cap/cap)*SUM(a, mpcapgova(a)*gdp(a)/gdp(a))   ==>
 mpcapgov = SUM(a, mpcapgova(a))

The aggregate elasticity:
  elas = cap*mpcapgov/gdp('total');

*===========================================
$OFFTEXT


* auxiliary parameters
PARAMETER
  mpcapgov_af(ac,fcapg)    change in GDP in activity a per unit of change in cap stk inv
  tfpelasqg00(ac,fcapg)    elas of TFP for a w.r.t. to gov cap stock inv (in EQ_TFPDEF)
;

* INITIAL VALUES for change in GDP for activity a per unit of capital
* stock inv:
* Note: [mpcapgov_af(a,inv)] = [rel'e size of impact on a]*[MP of cap stock]*[share of a in VA]
mpcapgov_af(a,fcapg) =
*[mpcapgov_af(a,f)]
* = [rel'e size of impact on a]*[MP of cap stock]*[share of a in VA]
  mtfp(a,fcapg)*mpcapgov(fcapg)*SUM(fp, SAM(fp,a))/SUM((fp,ap), SAM(fp,ap));
DISPLAY mpcapgov_af;

* scaling initial values so that their sum equals given exogenous value (mpcapgov)
mpcapgov_af(a,fcapg)$SUM(ap, mpcapgov_af(ap,fcapg)) = mpcapgov_af(a,fcapg)*mpcapgov(fcapg)/SUM(ap, mpcapgov_af(ap,fcapg));


$ONTEXT
Derivation of below definitions of tfpelasqg00 and mpcapgov_af:
  tfpelasqg = [dVA/dQF]/[VA00/QF00] = MP/[VA00/QF00] = MP*QF00/VA00
==>
  MP = tfpelasqg*VA00/QF00
$OFFTEXT

tfpelasqg00(a,fcapg)$mpcapgov_af(a,fcapg) =
* [tfpelas of VA in act a w.r.t. stock of f(cap)]
* = [MP (as VA) of gov cap f in act a]*[initial stock of gov cap inv]/[initial VA]
  mpcapgov_af(a,fcapg)*SUM(insgov, QFINS00(insgov,fcapg))/SUM(fp, SAM(fp,a));
DISPLAY tfpelasqg00;

* rotal change in GDP per additional unit of capital stock f
mpcapgov_af('total',fcapg)$SUM(insgov, QFINS00(insgov,fcapg)) =
  SUM(a, tfpelasqg00(a,fcapg)*SUM(fp, SAM(fp,a))) / SUM(insgov, QFINS00(insgov,fcapg));


* mpcapgov_af('total') = SUM(a, elasa(a)*gdp(a))/cap;
* Verifying that the total change in GDP PEDISR unit of extra capital
* in the base year is as intended given the above elasticity definition
* (which is rearranged and summed over a):
DISPLAY mpcapgov_af, mpcapgov;

DISPLAY 'mpcapchk', mtfp, mpcapgov, mpcapgov_af, QFINS00, tfpelasqg00;


PARAMETER
  mpcapgovgap(fcapg)    absolute gap bt. mpcapgov_af and mpcapgov by fcapg
;
LOOP(fcapg$(ABS(mpcapgov_af('total',fcapg) - mpcapgov(fcapg)) GT 1.0E-6),
  mpcapgovgap(fcapg) = ABS(mpcapgov_af('total',fcapg) - mpcapgov(fcapg));
  DISPLAY mpcapgov_af,  mpcapgov, mpcapgovgap;
  ABORT$(ABS(mpcapgov_af('total',fcapg) - mpcapgov(fcapg)) GT 1.0E-6)
  "Computed mpcapgov_af('total',f) is not equal to exogenou mpcapgov."
  "Solution: review data and code for compuations.                   "
);

* The aggregate elasticity:
tfpelasqg00('total',fcapg) =
  SUM(insgov, QFINS00(insgov,fcapg))*mpcapgov_af('total',fcapg) / SUM((fp,ap), SAM(fp,ap));
DISPLAY tfpelasqg00;


tfpelas(a,fcapg) = tfpelasqg00(a,fcapg);


* end: computation of tfpelas wrt infra

$ONTEXT
* Compute IRR
$INCLUDE irrmodel.inc
$OFFTEXT


* START: new treatrment of link between selected investments and tfp ----------

PARAMETER

  mpk(ac,fcap,t)          marginal product in act a per unit of additional capital stock fcap
  mpk0(ac,fcap,t)         marginal product in act a per unit of additional capital stock fcap
  mpk00(ac,fcap)          marginal product in act a per unit of additional capital stock fcap
  mpcapgov0(fcap)         marginal product of capital fcap
  mtfp0(a,fcap)           mapping - TFP in activity a affected by capital stock fcap (with rel value indicating strength of effect)

;

mpcapgov0(fcap) = mpcapgov(fcap);
mtfp0(a,fcap) = mtfp(a,fcap);

* raw definition of MP for act a of change in cap stock fcap
* = (aggregate MP of fcap) * (share of a in GDP) * (relative strength of MP for a from fcap)
mpk00(a,fcap)$(mpcapgov0(fcap) AND mtfp0(a,fcap)) =
  mpcapgov0(fcap)* (SUM(fcapp, SAM(fcapp,a))/SUM((fcapp,ap), SAM(fcapp,ap))) * mtfp0(a,fcap);
DISPLAY mpk00;

* scaling MP for a from f so that sum of activity-specific MPs equals aggregate MP
mpk00(a,fcap)$mpk00(a,fcap) = mpk00(a,fcap) * mpcapgov0(fcap)/SUM(ap, mpk00(ap,fcap));
mpk00('total',fcap)      = SUM(a, mpk00(a,fcap));
mpk0(ac,fcap,t) = mpk00(ac,fcap);
mpk(ac,fcap,t)  = mpk0(ac,fcap,t);
DISPLAY mpk00;


* START: diagnostic test on mpcapgov0 and mtfp0 -------------------------------

SET
  errmpcapgovmtfp0(fcap)    one of mpcapgov and mtfp0 is nonzero for fcap (either both or none should be nonzero)
* note: either both of or none of mpcapgov(f) and SUM(a, mtfp0(a,f)) are non-zero
;

errmpcapgovmtfp0(fcap)$((mpcapgov0(fcap) AND NOT SUM(a, mtfp0(a,fcap))) OR (NOT mpcapgov0(fcap) AND SUM(a, mtfp0(a,fcap)))) = YES;
DISPLAY mpcapgov0, mtfp0, errmpcapgovmtfp0;

ABORT$SUM(fcap$errmpcapgovmtfp0(fcap), 1)
"Only one of mpcapgov0(fcap) and SUM(a, mtfp0(a,fcap)) is nonzero for f. "
"Solution: review above displays of errmpcapgovmtfp0, mtfp0, and mpcapgov0, and "
"make sure that both parameters are zero  or non-zero for any given fcap (for "
"mtfp0 this applies to the sum over all a).";

* END: diagnostic test on mpcapgov0 and mtfp0 ---------------------------------


* to do: adjust equation instead of making zero tfpelas
tfpelas(a,fcap) = 0;
PARAMETER
  tfpinfra1 /1/
;

* END: new treatrment of link between selected investments and tfp ------------



* start: emissions

QEMI00(ghg,ac,acp)$acnt(acp) = qemibase(ghg,ac,acp);
DISPLAY QEMI00;

iemi00(ghg,c,a)$QINT00(c,a)      = QEMI00(ghg,c,a)/QINT00(c,a);
iemi00(ghg,f,a)$QF00(f,a)        = QEMI00(ghg,f,a)/QF00(f,a);
iemi00(ghg,ac,a)$(NOT SAM(ac,a) AND QEMI00(ghg,ac,a)) = QEMI00(ghg,ac,a)/QA00(a);

iemi00(ghg,c,h)$QH00(c,h)      = QEMI00(ghg,c,h)/QH00(c,h);
iemi00(ghg,ac,h)$(NOT SAM(ac,h) AND QEMI00(ghg,ac,h)) = QEMI00(ghg,ac,h)/SUM(c, PQD00(c,h)*QH00(c,h));

iemi00(ghg,c,insgov)$QG00(c)               = QEMI00(ghg,c,insgov)/QG00(c);
iemi00(ghg,ac,insgov)$(NOT SAM(ac,insgov) AND QEMI00(ghg,ac,insgov)) = QEMI00(ghg,ac,insgov)/SUM(c, PQD00(c,insgov)*QG00(c));

DISPLAY iemi00;

iemi(ghg,ac,acp,t)$tsol(t) = iemi00(ghg,ac,acp);

QEMI0(ghg,ac,acp,t)$tsol(t) = QEMI00(ghg,ac,acp)*gdpindex(t);

* end: emissions







* start: import quota

* allocation of rents
PARAMETER
  shryprqmbar00(c,ac)
;

shryprqmbar00(c,a) = QINT00(c,a)/(QQ00(c)-QT00(c));
shryprqmbar00(c,insgov) = QG00(c)/(QQ00(c)-QT00(c));
shryprqmbar00(c,h) = QH00(c,h)/(QQ00(c)-QT00(c));
shryprqmbar00(c,'ngovz') = (QINV00(c) + SUM(ins2, qdstk00(c,ins2)))/(QQ00(c)-QT00(c)); 

YPRQMBAR00(c,ac) = shryprqmbar00(c,ac) * YPRQMBART00(c);
YPRQMBAR0(c,ac,t)$tsol(t) = YPRQMBAR00(c,ac);

PARAMETER
  errshryprqmbar00(c)   if =UNDF total shryprqmbar00 NE 1
;

errshryprqmbar00(c)$(ABS(SUM(acnt, shryprqmbar00(c,acnt)) - 1) > 1e-10) = 1/0;
DISPLAY errshryprqmbar00;


* end: import quota


* activate the following sentence to debug
*$EXIT

* END: Parameters and variables - definition ========================
 
* START: Variables - declaration ====================================

VARIABLES
  ABSNOM(t)                  total nominal absorption
  CPI(t)                     consumer price index
  DKA(f,a,t)                 non-gov investment by destination
  DKINS(ins2,f,t)            investment by institution i in capital stock f
  DPI(t)                     index for domestic producer prices (PDS-based)
  EG(t)                      total current government expenditure
  EH(h,t)                    household consumption expenditure
  EXR(t)                     exchange rate (dom. currency per unit of for. currency)
  FBOR(ins2,t)                foreign borrowing inst insd
  FDEBT(ins2,t)               foreign debt stock inst insd
  FDISCAL(t)                 scaling factor for FDI
  FPRDA(f,a,t)               productivity term for factor f in act a
  GBOR(t)                    domestic government borrowing
  GDEBT(t)                   domestic government debt stock
  GDPMP(t)                   nominal GDP at market prices
  GOVRECGDP(acgovrec,t)      GDP shr for government receipt acgovrec
  GOVSPNDGDP(acgovspnd,t)    GDP shr for government spending acgovspnd
  GPRIMDEF(t)                government primary deficit
  IADJ(ins2,t)               scaling factor for non-gov and gov investment
  INVVAL(t)                  domestic non-government investment
  INVVALF(t)                 FDI value (in FCU)
  INVVALG(t)                 government investment
  ISCAL(ac,t)                investment scaling factor (for fixed capital formation)
  MPS(ins,t)                 marginal propensity to save for dom non-gov inst insdng
  MPSADJ(t)                  change in marginal propensity to save for dom non-gov inst
  MPSSCAL(t)                 savings rate scaling factor
  NDFG(t)                    net domestic financing (difference between net domestic borrowing and interest payments) (LCU)
  NFFG(t)                    gov net foreign financing (difference between net foreign borrowing and interest payments) (FCU)
  NFFINS(t)                  net foreign financing dom non-gov inst (FCU)
  NFFINSSCAL(t)              scaling factor for net foreign financing dom non-gov inst (FCU)
  NGOVPAYGDP(acngovpay,t)    GDP shr for non-government payment acngovpay
  PA(a,t)                    output price of activity a
  PDD(c,t)                   demand price for comm c produced and sold domestically
  PDS(c,t)                   supply price for comm c produced and sold domestically
  PE(c,t)                    export price for c (domestic currency)
  PK(fcap,t)                 replacement cost of capital inv
  PM(c,t)                    import price for c (domestic currency)
  PQD(c,ac,t)                composite commodity demand price for c demanded by ac
  PQS(c,t)                   composite commodity supply price for c
  PVA(a,t)                   value-added price for activity a
  PX(c,t)                    producer price for commodity c
  QA(a,t)                    level of activity a
  QD(c,t)                    quantity sold domestically of domestic output c
  QE(c,t)                    quantity of exports for commodity c
  QF(f,a,t)                  quantity demanded of factor f from activity a
  QFHEND(h,f,t)              end of period real endowment of factor ac for household h
  QFHENDSCAL(f,t)            scaling factor for end of period real endowment of factor ac for household h
  QFINS(ins,ac,t)            real endowment of factor ac for institution ins
  QFINSSCAL(f,t)             scaling factor for factor endowments
  QFS(f,t)                   supply of factor f
  QG(c,t)                    quantity of government demand for commodity c
  QGSCAL(t)                  government demand scaling factor
  QH(c,h,t)                  quantity consumed of commodity c by household h
  QINT(c,a,t)                quantity of commodity c as intermediate input to activity a
  QINV(c,t)                  quantity of investment demand for commodity c
  QM(c,t)                    quantity of imports of commodity c
  QNGO(c,insngo,t)           quantity consumed of commodity c by NGO insngo
  QNGOSCAL(insngo,t)         scaling factor for NGO consumption
  QQ(c,t)                    quantity of goods supplied domestically (composite supply)
  QT(c,t)                    quantity of trade and transport demand for commodity c
  QX(c,t)                    quantity of domestic output of commodity c
  REXR(t)                    real exchange rate
  RGDPFC(t)                  real GDP at factor cost (at constant base-year prices)
  
  RGDPPC(t)                  real GDP at factor cost per capita (at constant base-year prices)

  RGDPMP(t)                  real GDP at market prices (at constant base-year prices)
  RGPRIMDEF(t)               real government deficit
  RNDFG(t)                   real gov net domestic financing
  SAV(ins,t)                 savings of (domestic non-government) institution insdng
  SAVF(t)                    foreign savings (foreign currency)
  SHIF(ins,f,t)              share for inst ins in the income of factor f
  SUBC(c,ac,t)               subsidy rate for demander ac on comm c
  SUBCSCAL(t)                subsidy rate scaling factor
  SUBCT(t)                   total commodity subsidy spnd
  TA(a,t)                    rate of tax on producer gross output value
  TASCAL(t)                  scaling factor for rate of tax on producer gross output value
  TE(c,t)                    export tax rate for commodity c
  TESCAL(t)                  scaling factor for export tax rate for commodity c
  TF(f,t)                    rate of direct tax on factors (soc sec tax)
  TFA(f,a,t)                 rate of tax on factor f use by activity a
  TFASCAL(t)                 scaling factor rate of tax on factor f use by activity a
  TFP(a,t)                   sectoral TFP index
  TFPSCAL(t)                 tfp in calibration run
  TFSCAL(t)                  scaling factor for rate of direct tax on factors (soc sec tax)
  TM(c,t)                    import tariff rate for commodity c
  TMSCAL(t)                  scaling factor for import tariff rate for commodity c
  TQ(c,t)                    rate of sales tax
  TQSCAL(t)                  scaling factor for rate of sales tax
  TRDGDP(t)                  foreign trade (exports+imports) (GDP shr)
  TRII(ins,insp,t)           transfers to inst ins from dom inst insdng
  TRNSFR(ac,acp,t)           transfers from insp or factorp to ins or factor
  TRNSFRSCAL(ac,t)           scaling factor for transfers from insp to ins or factor
  TVAC(c,ac,t)               rate of value added tax commodity c demander ac
  TVACSCAL(t)                TVAC scaling factor
  TY(ins,t)                  rate of direct tax on dom inst ins
  TYSCAL(t)                  scaling factor for rate of direct tax
  UERAT(f,t)                 unemployment rate for factor f
  WALRAS(t)                  dummy variable (zero at equilibrium)
  WF(f,t)                    average price of factor f
  WFA(f,a,t)                 wage for factor f in activity a
  WFAVG(f,t)                 average remuneration of factor f
  WFDIST(f,a,t)              wage distortion factor for factor f in activity a
  YF(f,t)                    income of factor f
  YG(t)                      government revenue
  YI(ins,t)                  income of (domestic non-government) institution insdng
  YIF(ins,f,t)               income of institution ins from factor f
  YTAXIMP(t)                 total import tax collection
  YTAXEXP(t)                 total export tax collection
  YTAXVAT(t)                 total VAT tax collection

  LABPARTRAT(t)              ratio between labor force and population at labor force age
  QLABSCAL(t)                scaling factor for labor force part'on rate targeting

  QTRST(c,instrst,t)         quantity consumed of commodity c by tourist instrst
  QTRSTSCAL(t)               scaling factor for tourist consumption

  TRSMREC(t)                 total tourism receipts (FCU)

  GOVRECABS(acgovrec,t)      absorption shr for government receipt of ac
  GOVSPNDABS(acgovspnd,t)    absorption shr for government receipt of ac
  NGOVPAYABS(acngovpay,t)    absorption shr for government receipt of ac

  RBTVAT(a,t)               value added tax rebate to activities

  QXAC(a,c,t)                quantity of ouput of commodity c from activity a
  PXAC(a,c,t)                price of commodity c from activity a

  QEMI(ghg,ac,acp,t)          emissions of from commodity c by emitter ac

  PWE(c,t)                   export price for c (foreign currency)
  PWM(c,t)                   import price for c (foreign currency)

  FPRDASCAL(t)               scaling factor factor-specific productivity

  PREXR(t)              exchange rate premium
  YPREXRT(c,t)           total rents from exchage rate premium in commodity c
  YPREXR(c,ac,t)     rents from exchage rate premium in commodity c to demander ac

  PRQMBAR(c,t)          unit rent of import quota on commodity c
  YPRQMBART(c,t)         total import quota rents from commodity c
  YPRQMBAR(c,ac,t)   import quota rents from commodity c to demander ac

;


* EQUATIONS =========================================================

EQUATIONS

* Production and Factors

  EQ_WFADEF(f,a,t)                 defn wage for factor f in activity a
  EQ_PRODFN(a,t)                   value added production function
  EQ_PRODFN1(a,t)                   value added production function (dcal eq 1)
  EQ_FACDEM(f,a,t)                 demand for factor f from activity a

  EQ_FACDEMLEO(f,a,t)              Leontief demand for factor f from activity a
  EQ_PRODFNCES2(f,a,t)           Production function (level 2)
  EQ_FACDEMCES2(f,a,t)           Demand for factor f from activity a (level 2)
  EQ_PRODFNCES3(f,a,t)          Production function (level 3)
  EQ_FACDEMCES3(f,a,t)          Demand for factor f from activity a (level 3)

  EQ_TFPDEF(a,t)                   defn sectoral TFP
  EQ_FPRDADEF(f,a,t)               defn sectoral factor productivity
  EQ_INTDEM(c,a,t)                 intermediate demand for commodity c from activity a

  EQ_COMPRDFN(a,c,t)          production function for commodity c and activity a
  EQ_OUTAGGFOC(a,c,t)         first-order condition for output aggregation function
  EQ_OUTAGGFN(c,t)            output aggregation function

  EQ_PVADEF(a,t)                   value-added price for activity a
  EQ_PADEF(a,t)                    price for activity a
  EQ_WAGECURVE(f,t)                wage curve
  EQ_FACEQ(f,t)                    market equilibrium condition for factor f
  EQ_YFDEF(f,t)                    factor incomes

* Domestic and Aggregate Foreign Trade

  EQ_ARMING(c,t)                   composite supply (Armington) function for commodity c
  EQ_ARMING2(c,t)                  composite supply for commodities without both dom sales and imports
  EQ_IMPDOMRAT(c,t)                import-domestic demand ratio for commodity c
  EQ_PDDDEF(c,t)                   dem price for comm c produced and sold domestically
  EQ_ABSORB(c,t)                   absorption for commodity c
  EQ_PQDDEF(c,ac,t)                defn dem price for composite comm demanded by ac

  EQ_CET(c,t)                      output transformation (CET) function for commodity c
  EQ_CET2(c,t)                     domestic sales and exports for outputs without both
  EQ_EXPDOMRAT(c,t)                export-domestic supply ratio for commodity c
  EQ_ESUPPLYEXOG(c,t)              exogenous export supply
  EQ_OUTVAL(c,t)                   output value for commodity c


  EQ_PMDEF(c,t)                    import price for commodity c (domestic currency)
  EQ_PEDEF(c,t)                    export price for commodity c (domestic currency)
  EQ_EDEMAND(c,t)                  exports with constant-elasticity demand function (for c in ced)

  EQ_QTDEM(c,t)                    demand for transactions (trade and transport) services

* Current Payments by Domestic Institutions

  EQ_SHIFDEF(insd,f,t)             institutional shares in factor incomes
  EQ_YIFDEF(ins,f,t)               factor incomes to domestic institutions
  EQ_YIDEF(insdng,t)               total incomes of domest non-gov institutions
  EQ_MPSDEF(insdng,t)              marg prop to save for inst ins
  EQ_INSSAVDEF(insdng,t)           savings for domestic non-government institutions
  EQ_TRIIDEF(ins,insdng,t)         inter-institutional transfers
  EQ_EHDEF(h,t)                    household consumption expenditure
  EQ_HHDDEM(c,h,t)                 consumption demand for household h & commodity c
  EQ_ENGODEF(insngo,t)             NGO consumption expenditure
  EQ_NGODEM(c,insngo,t)            consumption demand for NGO insngo and commodity c
  EQ_TRSTDEM(c,instrst,t)          consumption demand for foreign tourist instrst and commodity c
  EQ_TRSMRECDEF(t)                 total international tourism receipts
  EQ_GOVREV(t)                     government revenue
  EQ_TYDEF(ins,t)                  rate of direct tax
  EQ_TFDEF(f,t)                    rate of direct tax on factors (soc sec tax)
  EQ_TADEF(a,t)                    rate of tax on producer gross output value
  EQ_TQDEF(c,t)                    rate of sales tax
  EQ_TEDEF(c,t)                    export tax rate for commodity c
  EQ_TMDEF(c,t)                    import tariff rate for commodity c
  EQ_VATREBATEDEF(a,t)             rebate for VAT on intermediate inputs


  EQ_TVACDEF(c,ac,t)               rate of VAT
  EQ_TFADEF(f,a,t)                 rate of tax on factor use
  EQ_SUBCDEF(c,ac,t)               subsidy rate for commodity c
  EQ_YTAXEXPDEF(t)                 total export tax collection
  EQ_YTARIMPDEF(t)                 total import tax collection
  EQ_YTAXVATDEF(t)                 total VAT tax collection
  EQ_GOVEXP(t)                     government expenditures
  EQ_GOVDEM(c,t)                   government consumption demand
  EQ_SUBCTDEF(t)                   total commodity subsidy spnd

  EQ_TRHROWDEF(h,insrow,t)        transfers from row to households
  EQ_TRINSDNHROWDEF(insd,insrow,t)        transfers from row to domestic non-gov and non-households
  EQ_TRFACROWDEF(ac,ins,t)         transfers from row to factors
  EQ_TRHGOVDEF(h,insgov,t)           transfers from gov to households
  EQ_TRINSDNHGOVDEF(insd,insgov,t)   transfers from gov to domestic non-gov and non-households
  
  EQ_TRROWGOVDEF(ac,ins,t)         transfers from gov to row
  EQ_TRGOVROWDEF(ac,ins,t)         transfers from row to gov
  EQ_TRROWFACDEF(insrow,f,t)       transfers from factors to row

* Investment, System Constraints, and Numéraire

  EQ_GOVPRIMDEF(t)                 government surplus
  EQ_GOVPRIMDEFREALDEF(t)          real government surplus
  EQ_GOVINVCOST(t)                 nominal government investment
  EQ_GOVCAPACC(t)                  government capital account
  EQ_GOVNETDOMFINREAL(t)           defn real government net domestic financing

  EQ_NGOVINVFIN(t)                 non-gov nominal investment value and financing
  EQ_NGOVINVCOST(t)                nominal non-government investment
  EQ_NGOVNETFORFIN(t)              non-gov net foreign financing

  EQ_DKGOVDEF(ins2,fcap,t)         gross change in government capital stocks (GFCF) by gov
  EQ_DKNGOVDEF(ins2,fcap,t)        gross change in non-government capital stocks (GFCF) by insdng
  EQ_DKROWDEF(ins2,fcap,t)         gross change in non-government capital stocks (GFCF) by RoW
  EQ_INVVALFDEF(t)                 foreign direct investment

  EQ_PCAPDEF(fcap,t)               price of capital food
  EQ_INVDEM(c,t)                   investment demand for commodity c
  EQ_CAPACCUMNGOVDOM(ins,f,t)      accumulation of capital by domestic non-hhd inst

  EQ_CAPACCUMNGOVHHD(h,f,t)        accumulation of capital by households
  EQ_CAPREDIST(h,f,t)              redistribution of capital among households due to different pop growth rates
  EQ_CAPREDISTCONST(f,t)           constraint on capital redistribution

  EQ_CAPACCUMNGOVFOR(ins,f,t)      accumulation of capital by RoW
  EQ_CAPACCUMGOV(ins,fcapg,t)      gov capital accumulation
  EQ_LABENDOWDEF(ins,f,t)          institutional endowments of labor factors
  EQ_OTHENDOWDEF(ins,f,t)          institutional endowments of non-labor and non-capital factors
  EQ_FACSUP(f,t)                   factor supplies
  EQ_LABPARTRATDEF(t)              defn labor force participation rate
  EQ_WFAVGDEF(f,t)                 average rate of return
  EQ_NEWCAPALLOC(f,a,t)            investment by destination
  EQ_CAPACCUMACT(f,a,t)            non-gov capital accumulation

  EQ_COMEQ(c,t)                    market equilibrium condition for composite commodity c
  EQ_CURACC(t)                     current account balance for RoW
  EQ_CAPACC(t)                     capital account balance for RoW

  EQ_CPIDEF(t)                     consumer price index
  EQ_DPIDEF(t)                     domestic producer price index
  EQ_REXRDEF(t)                    real exchange rate

* National Accounts

  EQ_GDPREALFCDEF(t)               real GDP factor cost
  EQ_GDPMPDEF(t)                   nominal GDP mp
  EQ_GDPREALMPDEF(t)               real GDP mp
  EQ_GOVRECGDPDEF(acgovrec,t)      ratio between govrec and gdp
  EQ_GOVSPNDGDPDEF(acgovspnd,t)    ratio between govspnd and gdp
  EQ_NGOVPAYGDPDEF(acngovpay,t)    ratio between ngovpay and gdp

  EQ_TRDGDPDEF(t)                  trade-GDP ratio
  EQ_ABSNOMDEF(t)                  nominal absorption
  EQ_GOVRECABSDEF(acgovrec,t)      ratio between govrec and absorption
  EQ_GOVSPNDABSDEF(acgovspnd,t)    ratio between govspnd and absorption
  EQ_NGOVPAYABSDEF(acngovpay,t)    ratio between ngovpay and absorption

* Emissions

  EQ_EMIACTC(ghg,c,a,t)            emissions by comm-activity
  EQ_EMIACTF(ghg,f,a,t)            emissions by factor-activity
  EQ_EMIACT2(ghg,ac,a,t)           emissions by activity not linked to comm
  EQ_EMIHHD(ghg,c,h,t)             emissions by comm-households
  EQ_EMIHHD2(ghg,ac,h,t)           emissions by households not linked to comm
  EQ_EMIGOV(ghg,c,t)               emissions by comm-government
  EQ_EMIGOV2(ghg,ac,t)             emissions by government not linked to comm


* Borrowing + Debt Stocks

  EQ_GOVDOMBOR(t)                  government domestic borrowing
  EQ_GOVDOMDEBT(t)                 government domestic debt stock
  EQ_GOVFORBOR(ins2,t)           government foreign borrowing
  EQ_NGOVFORBOR(ins2,t)          non-government foreign borrowing
  EQ_FORDEBT(ins2,t)               foreign debt

* import quota

  EQ_QMCONST(c,t)     contraint on imports
  EQ_IMPQUOTARENT(c,t)     rents from import quota
  
  EQ_HHDIMPQUOTARENT(c,h,t)   allocation of rents from import quotas to households
  EQ_ACTIMPQUOTARENT(c,a,t)   allocation of rents from import quotas to activities
  EQ_GOVIMPQUOTARENT(c,t)   allocation of rents from import quotas to government
  EQ_INVIMPQUOTARENT(c,t)   allocation of rents from import quotas to non-gov investment

* dual exchange rate
  
  EQ_FOREXRENT(c,t)   rents from forex rationing
  EQ_FOREXRENTALLOC(c,ac,t) allocation of rents from forex rationing to ac

  EQ_PMEXRODEF(c,t)      import price for commodity c (domestic currency using official exchange rate)
  

;

* END: Equations - declaration ======================================

* START: Equations - definition =====================================

PARAMETER 
  dcal01    0-1 parameter to activate or deactivate dynamic calibration /1/
;




*### Production and Factors

* Value Added

EQ_WFADEF(f,a,tcur(t))$(QF00(f,a))..
  WFA(f,a,t) =E= WF(f,t)*WFDIST(f,a,t)*(1+TFA(f,a,t));

* production function for activity a (level 1)

EQ_PRODFN(a,tcur(t))$(NOT dcal01 AND SUM(f, SAM(f,a)))..
* note the use of NOT fleo(f)
  QA(a,t) =E= 
*!!    (1-SUM(c, YPREXR(c,a,t)*YPREXRT(c,t))/(PA(a,t)*QA(a,t))) * 
    TFP(a,t) * phi_va(a) * SUM(f$(f1(f) AND NOT fleo(f)), delta_va(f,a)*(FPRDA(f,a,t)*QF(f,a,t))**(-rho_va(a))) ** (-1/rho_va(a))
    + SUM(fcap$(tfpinfra1 AND mpcapgov(fcap) AND mtfp(a,fcap)),
      (SUM(ins, QFINS(ins,fcap,t)) - SUM(ins, QFINS0(ins,fcap,t)))*mpk(a,fcap,t));

EQ_PRODFN1(a,tcur(t))$(dcal01 AND SUM(f, SAM(f,a)))..
  QA(a,t) =E= 
*!!    (1-SUM(c, YPREXR(c,a,t)*YPREXRT(c,t))/(PA(a,t)*QA(a,t))) * 
    TFP(a,t) * phi_va(a) * SUM(f$(f1(f) AND NOT fleo(f)), delta_va(f,a)*(FPRDA(f,a,t)*QF(f,a,t))**(-rho_va(a))) ** (-1/rho_va(a));

* demand for factor f from activity a (level 1)

EQ_FACDEM(f,a,tcur(t))$(f1(f) AND NOT fleo(f) AND QF00(f,a))..

*  QF(f,a,t) =E= ( PVA(a,t)/WFA(f,a,t) )**sigma_va(a) *
*    delta_va(f,a)**sigma_va(a) * (TFP(a,t)*phi_va(a))**(sigma_va(a)-1) * QA(a,t) * FPRDA(f,a,t)**(sigma_va(a)-1);

  WFA(f,a,t) =E= PVA(a,t) * QA(a,t)
* note the use of NOT fleo(f)
    * SUM(fp$(NOT fleo(fp)), delta_va(fp,a)*(FPRDA(fp,a,t)*QF(fp,a,t))**(-rho_va(a))) ** (-1)
    * delta_va(f,a) * QF(f,a,t)**(-rho_va(a)-1) * FPRDA(f,a,t)**(-rho_va(a));


* Leontief demand for factor f from activity a

EQ_FACDEMLEO(f,a,tcur(t))$(fleo(f) AND QF00(f,a))..
*  QF(f,a,t) =E= ifa(f,a,t)*QA(a,t);
  QF(f,a,t) =E= (ifa(f,a,t) / (1 + fleo01(f,a)*(TFP(a,t)/TFP00(a) - 1))) * QA(a,t);

$ONTEXT
TFP by a and t is a product of:
1. exogenous term
2. calibration scaling term
3. 1 or, if TFP is linked to capital stocks, the ratio bt current and baseyr capital stocks
$OFFTEXT



* production function for activity a (level 2)

EQ_PRODFNCES2(f1,a,tcur(t))$(fnsam(f1) AND QF00(f1,a))..
  QF(f1,a,t) =E= phi2(f1,a) * 
    SUM(f2$mf2f1(f2,f1), delta2(f2,a)*(FPRDA(f2,a,t)*QF(f2,a,t))**(-rho2(f1,a))) ** (-1/rho2(f1,a));

* demand for factor f from activity a (level 2)

EQ_FACDEMCES2(f2,a,tcur(t))$(QF00(f2,a))..
  QF(f2,a,t) =E= (SUM(f1$mf2f1(f2,f1), WFA(f1,a,t))/WFA(f2,a,t))**SUM(f1$mf2f1(f2,f1), sigma2(f1,a)) * 
   delta2(f2,a)**SUM(f1$mf2f1(f2,f1), sigma2(f1,a)) * 
   SUM(f1$mf2f1(f2,f1), phi2(f1,a)**(sigma2(f1,a)-1)) * 
   FPRDA(f2,a,t)**SUM(f1$mf2f1(f2,f1), (sigma2(f1,a)-1)) * 
   SUM(f1$mf2f1(f2,f1), QF(f1,a,t));

* production function for activity a (level 3)

EQ_PRODFNCES3(f2,a,tcur(t))$(fnsam(f2) AND QF00(f2,a))..
  QF(f2,a,t) =E= phi3(f2,a) * 
    SUM(f3$mf3f2(f3,f2), delta3(f3,a)*(FPRDA(f3,a,t)*QF(f3,a,t))**(-rho3(f2,a))) ** (-1/rho3(f2,a));

* demand for factor f from activity a (level 3)

EQ_FACDEMCES3(f3,a,tcur(t))$(QF00(f3,a))..
  QF(f3,a,t) =E= (SUM(f2$mf3f2(f3,f2), WFA(f2,a,t))/WFA(f3,a,t))**SUM(f2$mf3f2(f3,f2), sigma3(f2,a)) * 
    delta3(f3,a)**SUM(f2$mf3f2(f3,f2), sigma3(f2,a)) * 
    SUM(f2$(mf3f2(f3,f2)), phi3(f2,a))**(SUM(f2$mf3f2(f3,f2), sigma3(f2,a))-1) * 
    FPRDA(f3,a,t)**SUM(f2$mf3f2(f3,f2), (sigma3(f2,a)-1)) * 
    SUM(f2$mf3f2(f3,f2), QF(f2,a,t));

* TFP 

EQ_TFPDEF(a,tcur(t))..
  TFP(a,t) =E= tfpexog(a,t) * (1 + TFPSCAL(t)*tfp01(a)) *
  ( 1$(NOT SUM(fcapg, tfpelas(a,fcapg)))
    + (
          ( PROD((insgov,fcapg), ( QFINS(insgov,fcapg,t)/QFINS00(insgov,fcapg)  )**tfpelas(a,fcapg)) )$(dmod=1)
        + ( PROD((insgov,fcapg), ( QFINS(insgov,fcapg,t)/QFINS0(insgov,fcapg,t) )**tfpelas(a,fcapg)) )$(dmod=2 OR dmod=0)
      )$(SUM(fcapg, tfpelas(a,fcapg)))
  ) *

  (TRDGDP(t)/TRDGDP00)**tfpelas(a,'trdgdp');



$ONTEXT
In the current version, FPRDA is always exogenous
$OFFTEXT

EQ_FPRDADEF(f,a,tcur(t))$(QF00(f,a))..
  FPRDA(f,a,t) =E= fprdab(f,a,t)*(1+FPRDASCAL(t)*fprda01(f,a));


* Intermediate Inputs

EQ_INTDEM(c,a,tcur(t))$QINT00(c,a)..
  QINT(c,a,t) =E= ica(c,a,t) * QA(a,t);

* Commodity Production


EQ_COMPRDFN(a,c,tcur(t))$QXAC00(a,c)..
  QXAC(a,c,t) =E= theta(a,c)*QA(a,t);

EQ_OUTAGGFN(c,tcur(t))$QX00(c)..
  QX(c,t) =E= phi_ac(c) * SUM(a, delta_ac(a,c)*QXAC(a,c,t)**(-rho_ac(c)))**(-1/rho_ac(c));

EQ_OUTAGGFOC(a,c,tcur(t))$QXAC00(a,c)..
  QXAC(a,c,t) =E= (PX(c,t)/PXAC(a,c,t))**sigma_ac(c) * delta_ac(a,c)**sigma_ac(c) * phi_ac(c)**(sigma_ac(c)-1) * QX(c,t);


* Production Prices

$ONTEXT
Definition of PVA
$OFFTEXT

* MC-2018-10-08
* note the new term with summation over fleo(f)
EQ_PVADEF(a,tcur(t))..
  PA(a,t)*(1 - TA(a,t) - SUM(fcapg, shfcapga(fcapg,a))) 
  + SUM(c, YPREXR(c,a,t))/QA(a,t)
  + SUM(c, YPRQMBAR(c,a,t))/QA(a,t)
  =E= 
  PVA(a,t) + SUM(f$fleo(f), WFA(f,a,t)*ifa(f,a,t))
    + SUM(c, PQD(c,a,t)*ica(c,a,t))
* VAT rebate
    - RBTVAT(a,t)/QA(a,t)
    ;

$ONTEXT
Definition of PA
$OFFTEXT

EQ_PADEF(a,tcur(t))..
  PA(a,t) =E= SUM(c, theta(a,c) * PXAC(a,c,t));

* Wage Curve

$ONTEXT
Real wage is defined through a wage curve and as a function of an index of factor productivity.
$OFFTEXT

EQ_WAGECURVE(f,tcur(t))$fuendog(f)..
  (WF(f,t)/CPI(t)) =E= (WF00(f)*fprdindex(f,t)/CPI00) * (UERAT(f,t)/UERAT00(f))**eta_wf(f);
* (WF(f,t)/CPI(t)) =E= (WF00(f)               /CPI00) * (UERAT(f,t)/UERAT00(f))**eta_wf(f);

* Factor Markets

$ONTEXT
Factor supply net of unemployment, if any, equals employment (demand).
$OFFTEXT

EQ_FACEQ(f,tcur(t))$(fsam(f) AND fva(f) AND NOT fcap(f))..
  QFS(f,t)*(1-UERAT(f,t)) =E= SUM(a, QF(f,a,t));

* Factor Incomes

$ONTEXT
Income of factor f from domestic and foreign sources.
$OFFTEXT

PARAMETER
  shffp(f,fp)   share of factor fp income transferred  to factor f
;

shffp('f-capprv','f-capgov') = 1;


EQ_YFDEF(f,tcur(t))$(YF00(f) AND fsam(f) AND fva(f))..
  YF(f,t) =E= SUM(a, WF(f,t)*WFDIST(f,a,t)*QF(f,a,t)) 
    + SUM(insrow, TRNSFR(f,insrow,t))*EXR(t)
*    + SUM(insrow, YIF(insrow,f,t)) - SUM(insrow, YIF(insrow,f,t))/PREXR(t)
    + SUM(c, YPREXR(c,f,t))
    + SUM(fcapg, YF(fcapg,t)*shffp(f,fcapg));

EQUATION 
  EQ_YCAPGDEF(fcapg,t)   defn total income to government capital
;

EQ_YCAPGDEF(fcapg,tcur(t))$YF00(fcapg)..
  YF(fcapg,t) =E= SUM(a, shfcapga(fcapg,a)*PA(a,t)*QA(a,t));


*### Domestic and Aggregate Foreign Trade

* Armington Function

$ONTEXT
Domestic composite supply is a function of aggregate imports and domestic sales.
$OFFTEXT

EQ_ARMING(c,tcur(t))$(QD00(c)>0 AND QM00(c)>0)..
  QQ(c,t) =E= phi_q(c) *
    ( delta_m(c)*QM(c,t)**(-rho_q(c)) + delta_dd(c)*QD(c,t)**(-rho_q(c)) ) ** (-1/rho_q(c));

EQ_ARMING2(c,tcur(t))$( (QD00(c)>0 AND QM00(c)=0) OR (QD00(c)=0 AND QM00(c)>0) )..
  QQ(c,t) =E= QD(c,t) + QM(c,t);

EQ_IMPDOMRAT(c,tcur(t))$(QD00(c)>0 AND QM00(c)>0)..
  QM(c,t) / QD(c,t) =E= ( PDD(c,t)/PM(c,t) * delta_m(c)/delta_dd(c) ) ** (1/(1+rho_q(c)));

$ONTEXT
Domestic demander price for domestic output is the supplier price adjusted for
transactions cost.
$OFFTEXT

EQ_PDDDEF(c,tcur(t))$QD00(c)..
  PDD(c,t) =E= PDS(c,t) + SUM((ct,tacd), PQD(ct,tacd,t)*icd(ct,c));

$ONTEXT
Definition of PQS (by c and t), the composite supply price EXclusive of VAT and
commodity subsidies.

Note:this is actually not absorption since it includes intermediate demands.
$OFFTEXT

EQ_ABSORB(c,tcur(t))$QQ00(c)..
  PQS(c,t)*QQ(c,t) =E= PDD(c,t)*QD(c,t) + PM(c,t)*QM(c,t);

$ONTEXT
Definition of PQD, the composite demander price INclusive of VAT and commodity
subsidies.
$OFFTEXT

EQ_PQDDEF(c,d,tcur(t))$PQD00(c,d)..
  PQD(c,d,t) =E= PQS(c,t)*(1 + TQ(c,t))*(1 - SUBC(c,d,t))*(1 + TVAC(c,d,t));

* CET Function

$ONTEXT
Export-side equivalent to EQ_ARMING.
$OFFTEXT

EQ_CET(c,tcur(t))$( QD00(c)>0 AND QE00(c)>0 AND NOT cesexog(c) )..
*  QX(c,t) =E= phi_x(c) *
*    ( delta_e(c)*QE(c,t)**(rho_x(c)) + delta_ds(c)*QD(c,t)**(rho_x(c)) ) ** (1/rho_x(c));
  QX(c,t) =E= (scalphi_x(c,t)*phi_X(c)) *
    ( (scaldelta_e(c,t)*delta_e(c))*QE(c,t)**(rho_x(c)) + (scaldelta_ds(c,t)*delta_ds(c))*QD(c,t)**(rho_x(c)) ) ** (1/rho_x(c));


$ONTEXT
Export-side equivalent to EQ_ARMING2.
$OFFTEXT

EQ_CET2(c,tcur(t))$( (QD00(c)>0 AND QE00(c)=0) OR (QD00(c)=0 AND QE00(c)>0) OR (cesexog(c) AND QE00(c)) )..
  QX(c,t) =E= QD(c,t) + QE(c,t);


$ONTEXT
Export-side equivalent to EQ_IMPDOMRAT.
$OFFTEXT

EQ_EXPDOMRAT(c,tcur(t))$( QD00(c)>0 AND QE00(c)>0 AND NOT cesexog(c) )..
*  QE(c,t) / QD(c,t) =E= ( PE(c,t)/PDS(c,t) * delta_ds(c)/delta_e(c) ) ** (1/(rho_x(c)-1));
  QE(c,t) / QD(c,t) =E= ( PE(c,t)/PDS(c,t) * (scaldelta_ds(c,t)*delta_ds(c))/(scaldelta_e(c,t)*delta_e(c)) ) ** (1/(rho_x(c)-1));
  

* exogenous export supply

EQ_ESUPPLYEXOG(c,tcur(t))$(cesexog(c) AND QE00(c))..     
  QE(c,t) =E= qeb(c,t);

$ONTEXT
Export-side equivalent to EQ_ABSORB.
Definition of PX (by c and t), the producer prices as average of domestic and export prices.
$OFFTEXT

EQ_OUTVAL(c,tcur(t))$QX00(c)..
  PX(c,t) * QX(c,t) =E= PDS(c,t)*QD(c,t) + PE(c,t)*QE(c,t);

* start: import quota


EQ_QMCONST(c,tcur(t))$cmbar(c)..
  qmbar(c,t) =G= QM(c,t);

PRQMBAR.LO(c,t) = 0;
PRQMBAR.FX(c,t)$(NOT cmbar(c)) = 0;

EQ_IMPQUOTARENT(c,tcur(t))$cmbar(c)..
  YPRQMBART(c,t) =E= PRQMBAR(c,t)*PWM(c,t)*EXR(t)*QM(c,t);

YPRQMBART.FX(c,t)$(NOT cmbar(c)) = 0;

EQ_HHDIMPQUOTARENT(c,h,tcur(t))..
  YPRQMBAR(c,h,t) =E= QH(c,h,t)/(QQ(c,t)-QT(c,t)) * YPRQMBART(c,t);
  
EQ_ACTIMPQUOTARENT(c,a,tcur(t))..   
  YPRQMBAR(c,a,t) =E= QINT(c,a,t)/(QQ(c,t)-QT(c,t)) * YPRQMBART(c,t);

EQ_GOVIMPQUOTARENT(c,tcur(t)).. 
  SUM(insgov, YPRQMBAR(c,insgov,t)) =E= QG(c,t)/(QQ(c,t)-QT(c,t)) * YPRQMBART(c,t);

EQ_INVIMPQUOTARENT(c,tcur(t))..   
  YPRQMBAR(c,'ngovz',t) =E= ( QINV(c,t) + SUM(ins2, qdstk(c,ins2,t)) ) / (QQ(c,t)-QT(c,t)) * YPRQMBART(c,t); 


* end: import quota



*### Disaggregated foreign trade




* all exports are valued at the official exchange rate

EQ_FOREXRENT(c,tcur(t))..
  YPREXRT(c,t) =E= (1-shrom(c,t))*(PREXR(t)-1)*EXR(t)*PWM(c,t)*QM(c,t)
    - (1-shroe(c,t))*(PREXR(t)-1)*EXR(t)*PWE(c,t)*QE(c,t);

EQ_FOREXRENTALLOC(c,acnt,tcur(t))$shryprexr00(c,acnt)..
  YPREXR(c,acnt,t) =E= shryprexr(c,acnt,t)*YPREXRT(c,t);

YPREXR.FX(c,ac,t)$(NOT shryprexr00(c,ac)) = 0;

* World Prices

EQ_PMDEF(c,tcur(t))$QM00(c)..
  PM(c,t) =E= (1 + TM(c,t) + PRQMBAR(c,t)) * PWM(c,t) * ( (1-shrom(c,t))*EXR(t)*PREXR(t) + shrom(c,t)*EXR(t) )  
  + SUM((ct,tacm), PQD(ct,tacm,t)*icm(ct,c));

EQ_PEDEF(c,tcur(t))$QE00(c)..
  PE(c,t) =E= (1-TE(c,t))*PWE(c,t)*( (1-shroe(c,t))*EXR(t)*PREXR(t) + shroe(c,t)*EXR(t) ) 
  - SUM((ct,tace), PQD(ct,tace,t)*ice(ct,c));

EQ_EDEMAND(c,tcur(t))$(ced(c) AND QE00(c))..     
   QE(c,t) =E= qeb(c,t) * (PWE(c,t)/pwse(c,t))**eta_e(c);


* Trade and Transport Margins

$ONTEXT
Demand for transactions (trade, transportation) services expressed as a function
of export and import quanitites with maximum disaggregation (cdis,r).
$OFFTEXT

EQ_QTDEM(c,tcur(t))$QT00(c)..
  QT(c,t) =E= SUM(cp, icm(c,cp)*QM(cp,t) + ice(c,cp)*QE(cp,t))
  + SUM(cp, icd(c,cp)*QD(cp,t));

*### Current Payments by Domestic Institutions

EQ_SHIFDEF(insd,f,tcur(t))$SHIF00(insd,f)..
  SHIF(insd,f,t) =E= QFINS(insd,f,t)/SUM(insdp$SHIF00(insdp,f), QFINS(insdp,f,t)) * SUM(insdp, SHIF00(insdp,f));

  SHIF.FX(insrow,f,t) = SHIF0(insrow,f,t);

$ONTEXT
Income of factor f to institution ins (net of direct taxes).
$OFFTEXT

EQ_YIFDEF(ins,f,tcur(t))$YIF00(ins,f)..
*  YIF(insd,f,t) =E= SHIF(insd,f,t) * ( YF(f,t)*(1-TF(f,t)) - SUM(insrow, TRNSFR(insrow,f,t))*EXR(t) );
  YIF(ins,f,t) =E= SHIF(ins,f,t)*YF(f,t)*(1-TF(f,t));

* Domestic Non-Government Institutions

$ONTEXT
Income of domestic non-government institution ins from transfers (from gov,
abroad, and domestic non-government).
$OFFTEXT

EQ_YIDEF(insdng,tcur(t))$YI00(insdng)..
  YI(insdng,t) =E= SUM(f, YIF(insdng,f,t))
    + SUM(insgov, TRNSFR(insdng,insgov,t))*CPI(t)
    + SUM(insrow, TRNSFR(insdng,insrow,t))*EXR(t)
    + SUM(insdngp, TRII(insdng,insdngp,t))
    + SUM(c, YPREXR(c,insdng,t))
    + SUM(c, YPRQMBAR(c,insdng,t))
*    + (1-1/PREXR(t))*SUM(insrow, TRII(insrow,insdng,t));

    ;

$ONTEXT
MPS, scaled.
$OFFTEXT

EQ_MPSDEF(insdng,tcur(t))$MPS00(insdng)..
  MPS(insdng,t) =E= mpsb(insdng,t) * MPSSCAL(t) + MPSADJ(t) + dmpsb(insdng,t);

$ONTEXT
Savings of insdng defined as intercept (CPI-indexed) plus MPS out of disposable
income.
$OFFTEXT

EQ_INSSAVDEF(insdng,tcur(t))$SAV00(insdng)..
  SAV(insdng,t) =E= alpha_sav(insdng,t)*CPI(t) + MPS(insdng,t)*(1 - TY(insdng,t))*YI(insdng,t);

$ONTEXT
Transfers from insdng to any other institution defined as share of disposable
income net of savings.
$OFFTEXT

EQ_TRIIDEF(ins,insdng,tcur(t))$TRII00(ins,insdng)..
  TRII(ins,insdng,t) =E= shii(ins,insdng,t) * (YI(insdng,t)*(1-TY(insdng,t)) - SAV(insdng,t));

* Households

$ONTEXT
Household consumption expenditure defined as disposable income net of savings
and transfers to other institutions.
$OFFTEXT

EQ_EHDEF(h,tcur(t))..
  EH(h,t) =E= (1-TY(h,t)) * YI(h,t) - SAV(h,t) - SUM(ins, TRII(ins,h,t));

$ONTEXT
Household consumption commodity spending on commodity c as the sum of spending
on exogenous quantity (subsistency spending) plus a share of the remaining value
available for consumption spending.
$OFFTEXT

EQ_HHDDEM(c,h,tcur(t))$QH00(c,h)..
  PQD(c,h,t)*QH(c,h,t) =E= PQD(c,h,t)*gamma(c,h,t)*pop(h,t)
  + beta(c,h)*( EH(h,t) - SUM(cp, PQD(cp,h,t)*gamma(cp,h,t)*pop(h,t)) );

* NGOs

$ONTEXT
NGO consumption expenditure (LHS) defined as disposable income net of savings
and transfers to other institutions.
$OFFTEXT

EQ_ENGODEF(insngo,tcur(t))$QNGOSCAL00(insngo)..
  SUM(c, PQD(c,insngo,t)*QNGO(c,insngo,t)) =E=
    (1-TY(insngo,t)) * YI(insngo,t) - SAV(insngo,t) - SUM(ins, TRII(ins,insngo,t));

$ONTEXT
NGO consumption demand is an exogenous term times a scaling factor across all c.
$OFFTEXT

EQ_NGODEM(c,insngo,tcur(t))$QNGO00(c,insngo)..
  QNGO(c,insngo,t) =E= qngob(c,insngo,t)*QNGOSCAL(insngo,t);

* consumption demand for foreign tourist instrst and commodity c

EQ_TRSTDEM(c,instrst,tcur(t))$QTRST00(c,instrst)..
  QTRST(c,instrst,t) =E= qtrstb(c,instrst,t)*QTRSTSCAL(t);

* total international tourism receipts

EQ_TRSMRECDEF(tcur(t))$TRSMREC00..
  TRSMREC(t)*EXR(t) =E= SUM((c,instrst), PQD(c,instrst,t)*QTRST(c,instrst,t));

* Government

$ONTEXT
Gov rev is defined as the sum of indirect and direct taxes, transfers, factor
incomes, and customs union transfers.
$OFFTEXT

EQ_GOVREV(tcur(t))..
  YG(t) =E= SUM(insdng, TY(insdng,t) * YI(insdng,t))
    + SUM(f$(fsam(f) AND fva(f)), TF(f,t)*YF(f,t))
    + SUM(c, TQ(c,t)*PQS(c,t)*QQ(c,t))
    + YTAXVAT(t)
    + YTAXIMP(t)
    + YTAXEXP(t)
    + SUM((f,a)$(fsam(f) AND fva(f)), TFA(f,a,t)*WF(f,t)*WFDIST(f,a,t)*QF(f,a,t))
    + SUM(a, TA(a,t)*PA(a,t)*QA(a,t))
    + EXR(t)*SUM((insgov,insrow), TRNSFR(insgov,insrow,t))
    + SUM((insgov,insdng), TRII(insgov,insdng,t))
    + SUM((insgov,f), YIF(insgov,f,t))
    + SUM((c,insgov), YPRQMBAR(c,insgov,t))
    + SUM((c,insgov), YPREXR(c,insgov,t))
    
    

    
    ;


$ONTEXT
Indirect domestic and trade tax rates defined as an exogenous rate multiplied by
a potentially endogenous scaling parameter (one per tax type).
$OFFTEXT

EQ_TYDEF(ins,tcur(t))..
  TY(ins,t) =E= tyb(ins,t)*(1+ty01(ins,t)*TYSCAL(t));

EQ_TFDEF(f,tcur(t))..
  TF(f,t) =E= tfb(f,t)*(1+tf01(f,t)*TFSCAL(t));

EQ_TADEF(a,tcur(t))..
  TA(a,t) =E= tab(a,t)*(1+ta01(a)*TASCAL(t));

EQ_TQDEF(c,tcur(t))..
  TQ(c,t) =E= tqb(c,t)*TQSCAL(t);

EQ_TEDEF(c,tcur(t))..
  TE(c,t) =E= teb(c,t)*TESCAL(t);

EQ_TMDEF(c,tcur(t))..
  TM(c,t) =E= tmb(c,t)*TMSCAL(t);

EQ_TVACDEF(c,d,tcur(t))..
  TVAC(c,d,t) =E= tvacb(c,d,t)*TVACSCAL(t);

EQ_TFADEF(f,a,tcur(t))$(fsam(f) AND fva(f))..
  TFA(f,a,t) =E= tfab(f,a,t)*TFASCAL(t);


EQ_VATREBATEDEF(a,tcur(t))..
  RBTVAT(a,t) =E= SUM(c, shrbtvat(c,a,t)*TVAC(c,a,t)*PQS(c,t)*(1-SUBC(c,a,t))*(1+TQ(c,t))*QINT(c,a,t));

$ONTEXT
Subsidy rate for demander d and commodity c, potentially scaled.
$OFFTEXT

EQ_SUBCDEF(c,d,tcur(t))..
  SUBC(c,d,t) =E= subcb(c,d,t)*SUBCSCAL(t);

$ONTEXT
Definitions of total tax revenues for each tax type and total subsidy spending;
used in the equations EQ_GOVREV and EQ_GOVEXP.
$OFFTEXT

EQ_YTAXEXPDEF(tcur(t))..
  YTAXEXP(t) =E= 
    SUM(c, TE(c,t)*PWE(c,t)*QE(c,t) * ( (1-shroe(c,t))*EXR(t)*PREXR(t) + shroe(c,t)*EXR(t) ))
;


EQ_YTARIMPDEF(tcur(t))..
  YTAXIMP(t) =E= 
    SUM(c, TM(c,t)*PWM(c,t)*QM(c,t) * ( (1-shrom(c,t))*EXR(t)*PREXR(t) + shrom(c,t)*EXR(t) ));

EQ_YTAXVATDEF(tcur(t))..
  YTAXVAT(t) =E=
      SUM((c,a),         (1-SUBC(c,a,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,a,t)*QINT(c,a,t))
    + SUM((c,h),         (1-SUBC(c,h,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,h,t)*QH(c,h,t))
    + SUM((c,insngo),    (1-SUBC(c,insngo,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,insngo,t)*QNGO(c,insngo,t))
    + SUM((c,instrst),   (1-SUBC(c,instrst,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,instrst,t)*QTRST(c,instrst,t))
    + SUM((c,insgov),    (1-SUBC(c,insgov,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,insgov,t)*QG(c,t))
    + SUM((c,fcap),      (1-SUBC(c,fcap,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,fcap,t)*capcomp(fcap,c)*SUM(ins2, DKINS(ins2,fcap,t)))
    + SUM((c,dstk),      (1-SUBC(c,dstk,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,dstk,t)*SUM(ins2, qdstk(c,ins2,t)))
    + SUM((c,cp,tacm),   (1-SUBC(c,tacm,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,tacm,t)*icm(c,cp)*QM(cp,t))
    + SUM((c,cp,tace),   (1-SUBC(c,tace,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,tace,t)*ice(c,cp)*QE(cp,t))
    + SUM((c,cp,tacd),   (1-SUBC(c,tacd,t))*PQS(c,t)*(1+TQ(c,t))*TVAC(c,tacd,t)*icd(c,cp)*QD(cp,t));

$ONTEXT
Gov expenditures is defined as sum of spending on consumption, transfers, and
subsidies.
$OFFTEXT

EQ_GOVEXP(tcur(t))..
  EG(t) =E= SUM((c,insgov), PQD(c,insgov,t) * QG(c,t))
    + SUM((insdng,insgov), TRNSFR(insdng,insgov,t))*CPI(t)
    + SUM((insrow,insgov), TRNSFR(insrow,insgov,t))*EXR(t)
    + SUBCT(t)
    + SUM(a, RBTVAT(a,t));

$ONTEXT
Gov consumption demand is an exogenous term times a scaling factor across all c.

Alternatives:
1. Exogenous consumption:
   QG(c,t) is defined by qgb; QGSCAL(t) = 0 (fixed); qgc01 is irrelevant

2. scaling of level of QG for any subset of c (1, all relevant c, ..);
   QGSCAL(t) is flexed; qgc01(c,t) is one for all relevant c (one or more)
$OFFTEXT

EQ_GOVDEM(c,tcur(t))$QG00(c)..
  QG(c,t) =E= qgb(c,t) * (1 + qgc01(c,t)*QGSCAL(t)) + dqg(c,t);

$ONTEXT
Gov total spending in commodity subsidies.
$OFFTEXT

EQ_SUBCTDEF(tcur(t))..
  SUBCT(t) =E=
      SUM((c,a),         SUBC(c,a,t)*PQS(c,t)*(1+TQ(c,t))*QINT(c,a,t))
    + SUM((c,h),         SUBC(c,h,t)*PQS(c,t)*(1+TQ(c,t))*QH(c,h,t))
    + SUM((c,insngo),    SUBC(c,insngo,t)*PQS(c,t)*(1+TQ(c,t))*QNGO(c,insngo,t))
    + SUM((c,instrst),   SUBC(c,instrst,t)*PQS(c,t)*(1+TQ(c,t))*QTRST(c,instrst,t))
    + SUM((c,insgov),    SUBC(c,insgov,t)*PQS(c,t)*(1+TQ(c,t))*QG(c,t))
    + SUM((c,fcap),      SUBC(c,fcap,t)*PQS(c,t)*(1+TQ(c,t))*capcomp(fcap,c)*SUM(ins2, DKINS(ins2,fcap,t)))
    + SUM((c,dstk),      SUBC(c,dstk,t)*PQS(c,t)*(1+TQ(c,t))*SUM(ins2, qdstk(c,ins2,t)))
    + SUM((c,cp,tacm),   SUBC(c,tacm,t)*PQS(c,t)*(1+TQ(c,t))*icm(c,cp)*QM(cp,t))
    + SUM((c,cp,tace),   SUBC(c,tace,t)*PQS(c,t)*(1+TQ(c,t))*ice(c,cp)*QE(cp,t))
    + SUM((c,cp,tacd),   SUBC(c,tacd,t)*PQS(c,t)*(1+TQ(c,t))*icd(c,cp)*QD(cp,t));



* Transfers that Follow Rules



$ONTEXT
Transfers from RoW to insdng defined as exogenous value with potential scaling.
(in FCU) following equations same for from gov to RoW; and from RoW to gov.
$OFFTEXT

EQ_TRHROWDEF(h,insrow,tcur(t))$TRNSFR00(h,insrow)..
  TRNSFR(h,insrow,t) =E= trnsfrpcb(h,insrow,t)*pop(h,t)*TRNSFRSCAL('trngovrow',t);

EQ_TRINSDNHROWDEF(insdngnh,insrow,tcur(t))$(TRNSFR00(insdngnh,insrow))..
  TRNSFR(insdngnh,insrow,t) =E= trnsfrb(insdngnh,insrow,t)*TRNSFRSCAL('trngovrow',t);

EQ_TRFACROWDEF(f,insrow,tcur(t))$TRNSFR00(f,insrow)..
  TRNSFR(f,insrow,t) =E= trnsfrb(f,insrow,t)*TRNSFRSCAL('trfacrow',t);

EQ_TRHGOVDEF(h,insgov,tcur(t))$TRNSFR00(h,insgov)..
  TRNSFR(h,insgov,t) =E= trnsfrpcb(h,insgov,t)*pop(h,t)*TRNSFRSCAL('trngovgov',t) + dtrnsfr(h,insgov,t);

EQ_TRINSDNHGOVDEF(insdngnh,insgov,tcur(t))$TRNSFR00(insdngnh,insgov)..
  TRNSFR(insdngnh,insgov,t) =E= trnsfrb(insdngnh,insgov,t)*TRNSFRSCAL('trngovgov',t) + dtrnsfr(insdngnh,insgov,t);

EQ_TRROWGOVDEF(insrow,insgov,tcur(t))$TRNSFR00(insrow,insgov)..
  TRNSFR(insrow,insgov,t) =E= trnsfrb(insrow,insgov,t)*TRNSFRSCAL('trrowgov',t);

EQ_TRGOVROWDEF(insgov,insrow,tcur(t))$TRNSFR00(insgov,insrow)..
  TRNSFR(insgov,insrow,t) =E= trnsfrb(insgov,insrow,t)*TRNSFRSCAL('trgovrow',t);

$ONTEXT
EQ_TRROWFACDEF(insrow,f,tcur(t))$TRNSFR00(insrow,f)..
  TRNSFR(insrow,f,t) =E= (trnsfrb(insrow,f,t)*TRNSFRSCAL('trrowfac',t))$(SUM(ins, SHIF00(ins,f)))
* for factors without domestic owners, all their income net of factor income tax
* is transferred to RoW
  + (YF(f,t)*(1-TF(f,t))/EXR(t))$(NOT SUM(ins, SHIF00(ins,f)));
$OFFTEXT

*### Investment, System Constraints, and Numéraire

EQ_GOVPRIMDEF(tcur(t))..
  GPRIMDEF(t) =E= EG(t) + INVVALG(t) - YG(t);

EQ_GOVPRIMDEFREALDEF(tcur(t))..
  RGPRIMDEF(t)*CPI(t) =E= GPRIMDEF(t);

$ONTEXT
Definition of the nominal value of government investment -- it enters the
government primary surplus definition.
$OFFTEXT

EQ_GOVINVCOST(tcur(t))..
  INVVALG(t) 
*  + SUM(c, YPREXR(c,'govz',t)) 
*  + SUM(c, YPRQMBAR(c,'govz',t))
  =E= 
    SUM(fcap, PK(fcap,t)*DKINS('govz',fcap,t))
    + SUM((c,dstk), PQD(c,dstk,t)*qdstk(c,'govz',t));


$ONTEXT
Financing of primary deficit is the sum of net foreign and net domestic
financing. Both are made up of the difference between net new borrowing and
interest payments.
$OFFTEXT

EQ_GOVCAPACC(tcur(t))..
  GPRIMDEF(t) =E= EXR(t)*NFFG(t) + NDFG(t);

$ONTEXT
Real net domestic financing is defined as the CPI-indexed nominal value.
$OFFTEXT

EQ_GOVNETDOMFINREAL(tcur(t))..
  RNDFG(t) =E= NDFG(t)/CPI(t);

$ONTEXT
Non-gov investment value and its financing.
$OFFTEXT

EQ_NGOVINVFIN(tcur(t))$INVVAL00..
  INVVAL(t) =E=
    SUM(insdng, SAV(insdng,t))
    + NFFINS(t)*EXR(t)
    - ( NDFG(t) + drf(t)*EXR(t) )
    + SUM(c, YPREXR(c,'ngovz',t))
    + SUM(c, YPRQMBAR(c,'ngovz',t));

$ONTEXT
Definition of the nominal value of domestic non-government inst investment -- it
enters the dom non-gov inst capital account.
$OFFTEXT

EQ_NGOVINVCOST(tcur(t))$INVVAL00..
  INVVAL(t) =E= SUM(fcap, PK(fcap,t)*DKINS('ngovz',fcap,t)) + SUM((c,dstk), PQD(c,dstk,t)*qdstk(c,'ngovz',t));


$ONTEXT
Domestic non-gov net financing is defined as an exogenous term times a uniform
scaling parameter.
$OFFTEXT

PARAMETER
  dnffins0(t)   change in net foreign financing dom non-gov inst (FCU)
  dnffins(t)    change in net foreign financing dom non-gov inst (FCU)
;

dnffins0(t) = 0;
dnffins(t) = dnffins0(t);

EQ_NGOVNETFORFIN(tcur(t))..
  NFFINS(t) =E= nffinsbar(t)*NFFINSSCAL(t) + dnffins(t);

* gross change in government capital stocks (GFCF) by gov

EQ_DKGOVDEF('govz',fcap,tcur(t))$DKINS00('govz',fcap)..
  DKINS('govz',fcap,t) =E= (dkinsb('govz',fcap,t)*ISCAL(fcap,t)) * [ 1 + IADJ('govz',t)*iadj01(fcap) ] + ddkins('govz',fcap,t);;

* gross change in non-government capital stocks (GFCF) by insdng

EQ_DKNGOVDEF('ngovz',fcap,tcur(t))$DKINS00('ngovz',fcap)..
  DKINS('ngovz',fcap,t) =E= (dkinsb('ngovz',fcap,t)*ISCAL(fcap,t)) * [ 1 + IADJ('ngovz',t)*iadj01(fcap) ] + ddkins('ngovz',fcap,t);

* gross change in non-government capital stocks (GFCF) by RoW

EQ_DKROWDEF('rowz',fcapng,tcur(t))$DKINS00('rowz',fcapng)..
  DKINS('rowz',fcapng,t) =E= invshr(fcapng,'rowz',t)*INVVALF(t)*EXR(t)/PK(fcapng,t);

* nominal foreign direct investment (FCU)


PARAMETER 
  dkafdi0(f,a,t)       sector-specific fdi
  dkafdi(f,a,t)        sector-specific fdi 
;
dkafdi0(f,a,t) = 0;
dkafdi(f,a,t)$tsol(t) = 0;


EQ_INVVALFDEF(tcur(t))$INVVALF00..
  INVVALF(t) =E= invvalfb(t)*FDISCAL(t) 
    + SUM((fcap,a), dkafdi(fcap,a,t)*PK(fcap,t))/EXR(t);

$ONTEXT
Price per unit of new capital stock (by destination and INVESTMENT type) defined
on the basis of the cap comp matrix and the demander price for investment goods.
$OFFTEXT

EQ_PCAPDEF(fcap,tcur(t))..
  PK(fcap,t) =E= SUM(c, capcomp(fcap,c)*PQD(c,fcap,t));

$ONTEXT
Investment by source commodity c defined as the product of investment by
institutions and the cap comp parameter.
$OFFTEXT

EQ_INVDEM(c,tcur(t))$QINV00(c)..
  QINV(c,t) =E= SUM((ins2,fcap), capcomp(fcap,c)*DKINS(ins2,fcap,t));

* accumulation of non-gov capital by domestic non-household institutions

* in this version, no link between inst investment and capital stocks
EQ_CAPACCUMNGOVDOM(insdnh,fcapng,tcur(t))$(NOT tmin(t) AND QFINS00(insdnh,fcapng))..
  QFINS(insdnh,fcapng,t) =E= QFINS.L(insdnh,fcapng,t-1)*(1-deprcap(fcapng))
*    + SHIF.L(insdnh,fcapng,t-1)*[DKINS.L('ngovz',fcapng,t-1)]$(dmod NE 2)
    + [ SHIF.L(insdnh,fcapng,t-1)*[DKINS.L('ngovz',fcapng,t-1)+DKINS.L('govz',fcapng,t-1)] ]$(dmod NE 2)
    + [ SHIF.L(insdnh,fcapng,t-1)*[DKINS.L('ngovz',fcapng,t-1)+DKINS.L('govz',fcapng,t-1)+DKINS.L('rowz',fcapng,t-1)] ]$(dmod=2);

* tmin
QFINS.FX(ins,fcapng,t)$tmin(t) = QFINS0(ins,fcapng,t);


* start: accumulation of non-gov capital by households

* accumulation of capital by households
* in this version, no link between inst investment and capital stocks

EQ_CAPACCUMNGOVHHD(h,fcapng,tcur(t))$(NOT tmin(t) AND QFINS00(h,fcapng))..
*!! to del
*  QFINS(h,fcapng,t) =E= QFINS.L(h,fcapng,t-1)*(1-deprcap(fcapng))
  QFHEND(h,fcapng,t) =E= QFINS.L(h,fcapng,t-1)*(1-deprcap(fcapng))
*    + SHIF.L(h,fcapng,t-1)*DKINS.L('ngovz',fcapng,t-1);
    + [ SHIF.L(h,fcapng,t-1)*[DKINS.L('ngovz',fcapng,t-1)+DKINS.L('govz',fcapng,t-1)] ]$(dmod NE 2)
    + [ SHIF.L(h,fcapng,t-1)*[DKINS.L('ngovz',fcapng,t-1)+DKINS.L('govz',fcapng,t-1)+DKINS.L('rowz',fcapng,t-1)] ]$(dmod=2);

* redistribution of capital among households due to different pop growth rates

EQ_CAPREDIST(h,fcapng,tcur(t))$(NOT tmin(t) AND QFINS00(h,fcapng))..
* note: pop(h,t)/pop(h,t-1) does not solves with iterlim eq 0
* so, pop growth is excluded if dmod eq 2
  QFINS(h,fcapng,t) =E= [QFHEND(h,fcapng,t) * pop(h,t)/pop(h,t-1) * QFHENDSCAL(fcapng,t)]$(dmod NE 2)
    + [QFHEND(h,fcapng,t) * QFHENDSCAL(fcapng,t)]$(dmod EQ 2);

* constraint on capital redistribution

EQ_CAPREDISTCONST(fcapng,tcur(t))$(NOT tmin(t) AND SUM(h, QFINS00(h,fcapng)))..
  SUM(h, QFINS(h,fcapng,t)) =E= SUM(h, QFHEND(h,fcapng,t));

* tmin
QFHEND.FX(h,fcapng,t)$tmin(t) = QFINS0(h,fcapng,t);
QFHENDSCAL.FX(fcapng,t)$tmin(t) = QFHENDSCAL0(fcapng,t);

* end: accumulation of non-gov capital by households


* accumulation of non-gov capital by RoW

* note: this eq is excluded when dmod=2
EQ_CAPACCUMNGOVFOR(insrow,fcapng,tcur(t))$(NOT tmin(t) AND QFINS00(insrow,fcapng))..
  QFINS(insrow,fcapng,t) =E=
    QFINS.L(insrow,fcapng,t-1)*(1-deprcap(fcapng))
    + DKINS.L('rowz',fcapng,t-1);

* accumulation of gov capital by government

EQ_CAPACCUMGOV(insgov,fcapg,tcur(t))$(NOT tmin(t) AND QFINS00(insgov,fcapg))..
  QFINS(insgov,fcapg,t) =E=
    QFINS.L(insgov,fcapg,t-1)*(1-deprcap(fcapg))
    + SUM(ins2, DKINS.L(ins2,fcapg,t-1));

* tmin
QFINS.FX(insgov,fcapg,t)$tmin(t) = QFINS0(insgov,fcapg,t);

* institutional endowments of factors

EQ_LABENDOWDEF(ins,f,tcur(t))$(QFINS00(ins,f) AND flab(f))..
  QFINS(ins,f,t) =E= qfinsb(ins,f,t)*QFINSSCAL(f,t)*QLABSCAL(t);

EQ_OTHENDOWDEF(ins,f,tcur(t))$(QFINS00(ins,f) AND fncap(f) AND NOT flab(f))..
  QFINS(ins,f,t) =E= qfinsb(ins,f,t)*QFINSSCAL(f,t);

* Factor Supplies

EQ_FACSUP(f,tcur(t))$QFS00(f)..
  QFS(f,t) =E= SUM(ins, QFINS(ins,f,t));

$ONTEXT
If LABPARTRAT is fixed (see the equation EQ_LABPARTRATDEF), then QLABSCAL is
flexible to assure that the constraint on labor-force participation is
satisfied.
$OFFTEXT

* Labor Force Participation Rate

EQ_LABPARTRATDEF(tcur(t))..
  LABPARTRAT(t) =E= SUM((ins,flab), QFINS(ins,flab,t))/pop('agelab',t);


$ONTEXT
Average wage of factor f defined as total income divided by total employment.
$OFFTEXT

EQ_WFAVGDEF(f,tcur(t))$(fsam(f) AND fva(f))..
  WFAVG(f,t) =E= SUM(a, WFA(f,a,t)*QF(f,a,t)) / SUM(a, QF(f,a,t));

$ONTEXT
Change in capital stock fcap by activity a is defined as:
        [total change in fcap (mapped from inv type) TIMES [share of a in fcap]
        TIMES
        [new capital share adjustment: if rent of fcap in a is above (below)
        average, then the share increases (decreases), for kappa >0];
$OFFTEXT

PARAMETER
  dkaexog010(a)       0-1 parameter for selecting activities with exog non-gov investment by destination
  dkaexog01(a)        0-1 parameter for selecting activities with exog non-gov investment by destination

;

dkaexog010(a) = 0;
dkaexog01(a) = dkaexog010(a);

DKA.FX(f,a,t)$dkaexog01(a) = DKA0(f,a,t);

EQ_NEWCAPALLOC(fcap,a,tcur(t))$(fcapng(fcap) AND NOT dkaexog01(a))..
  DKA(fcap,a,t) =E= SUM(ins2, DKINS(ins2,fcap,t))
   * QF(fcap,a,t)/SUM(ap, QF(fcap,ap,t)) *
    ( 1 + kappa * ( WFA(fcap,a,t)/WFAVG(fcap,t) - 1 ) );

$ONTEXT
Employment of capital fcap by act a is employment in the previous period net of
depreciation PLUS addition to fcap allocated to act a.
$OFFTEXT



EQ_CAPACCUMACT(fcap,a,tcur(t))$(fcapng(fcap) AND NOT tmin(t) AND QF00(fcap,a))..
  QF(fcap,a,t) =E= QF.L(fcap,a,t-1)*(1-deprcap(fcap)) + DKA.L(fcap,a,t-1) + dkafdi(fcap,a,t-1);



* Commodity Markets

$ONTEXT
Composite ocmmodity demands equal composite commodity supply.
$OFFTEXT

EQ_COMEQ(c,tcur(t))$QQ00(c)..
  SUM(h, QH(c,h,t))
  + SUM(insngo, QNGO(c,insngo,t))
  + SUM(a, QINT(c,a,t))
  + QINV(c,t)
  + SUM(ins2, qdstk(c,ins2,t))
  + QG(c,t)
  + QT(c,t)
  + SUM(instrst, QTRST(c,instrst,t))
  =E= QQ(c,t);

* Rest of the World

$ONTEXT
Current account of BoP.
  LHS shows inflows (exports, transfers, tariff inflow [if customs union], for savings)
        RHS show outflows (imports, transfers, factor income, tariff outflow (if customs union)
$OFFTEXT

EQ_CURACC(tcur(t))..
  SUM(c, PWE(c,t)*QE(c,t))
    + SUM((c,instrst), PQD(c,instrst,t)*QTRST(c,instrst,t))/EXR(t)
    + SUM((insd,insrow), TRNSFR(insd,insrow,t))
    + SUM((f,insrow), TRNSFR(f,insrow,t))
    + SAVF(t) =E= SUM(c, PWM(c,t)*QM(c,t))
    + SUM((insrow,insgov), TRNSFR(insrow,insgov,t))
*    + SUM((insrow,f), YIF(insrow,f,t))/(EXR(t)*PREXR(t))
    + SUM((insrow,f), YIF(insrow,f,t))/(EXR(t))
*    + SUM((insrow,insdng), TRII(insrow,insdng,t))/(EXR(t)*PREXR(t));
    + SUM((insrow,insdng), TRII(insrow,insdng,t))/(EXR(t));

$ONTEXT
Foreign savings equal to the sum of government and non-governmennt foreign financing.
$OFFTEXT

EQ_CAPACC(tcur(t))..
  SAVF(t) =E= NFFG(t) + NFFINS(t) + INVVALF(t) - drf(t) + WALRAS(t);

* Consumer Price Index

EQ_CPIDEF(tcur(t))..
  SUM((c,h), PQD(c,h,t) * cwts(c,h)) =E= CPI(t);

* Index for Domestic Producer Prices

EQ_DPIDEF(tcur(t))..
  SUM(c, PDS(c,t) * dwts(c)) =E= DPI(t);

* Real Exchange Rate

EQ_REXRDEF(tcur(t))..
  REXR(t) =E= EXR(t) / DPI(t);

* Real GDP at Factor Cost

$ONTEXT
Definitions of real GDP at fc and nominal GDP at mp.
$OFFTEXT

EQ_GDPREALFCDEF(tcur(t))..
  RGDPFC(t) =E= SUM(a, PVA00(a)*QA(a,t))
  + SUM((f,a)$fleo(f), WFA00(f,a)*QF(f,a,t));

* Nominal GDP at Market Prices

EQ_GDPMPDEF(tcur(t))..
  GDPMP(t) =E= SUM((c,h), PQD(c,h,t)*QH(c,h,t))
  + SUM((c,insngo), PQD(c,insngo,t)*QNGO(c,insngo,t))
  + SUM((c,instrst), PQD(c,instrst,t)*QTRST(c,instrst,t))
  + SUM((c,fcap), PQD(c,fcap,t)*capcomp(fcap,c)*SUM(ins2, DKINS(ins2,fcap,t)))
  + SUM((c,dstk), PQD(c,dstk,t)*SUM(ins2, qdstk(c,ins2,t)))
  + SUM((c,insgov), PQD(c,insgov,t)*QG(c,t))
  + SUM(c, EXR(t)*PWE(c,t)*QE(c,t))
  - SUM(c, EXR(t)*PWM(c,t)*QM(c,t));

* Real GDP at Market Prices

EQ_GDPREALMPDEF(tcur(t))..
  RGDPMP(t) =E= SUM((c,h), PQD00(c,h)*QH(c,h,t))
  + SUM((c,insngo), PQD00(c,insngo)*QNGO(c,insngo,t))
  + SUM((c,instrst), PQD00(c,instrst)*QTRST(c,instrst,t))
  + SUM((c,fcap), PQD00(c,fcap)*capcomp(fcap,c)*SUM(ins2, DKINS(ins2,fcap,t)))
  + SUM((c,dstk), PQD00(c,dstk)*SUM(ins2, qdstk(c,ins2,t)))
  + SUM((c,insgov), PQD00(c,insgov)*QG(c,t))
  + SUM(c, EXR00*PWE00(c)*QE(c,t))
  - SUM(c, EXR00*PWM00(c)*QM(c,t));

* start: GDP per capita

PARAMETER
    RGDPPC00                  real GDP at factor cost per capita (at constant base-year prices)
    RGDPPC0(t)                  real GDP at factor cost per capita (at constant base-year prices)
;

PARAMETER
  gdppcgrw(t)    GDP per capita growth rate
  gdppcindex(t)
;

gdppcgrw(t) = 0;

gdppcindex(tmin) = 1;
LOOP(t$(NOT tmin(t) AND tsol(t)),
  gdppcindex(t) = gdppcindex(t-1)*(1 + gdppcgrw(t));
);


RGDPPC00 = RGDPMP00/SUM(h, pop00(h));
RGDPPC0(t)$tsol(t) = RGDPPC00*gdppcindex(t);

EQUATIONS
  EQ_GDPPCREALDEF(t)    real GDP per capita
;

EQ_GDPPCREALDEF(tcur(t))..
  RGDPPC(t)*SUM(h, pop(h,t)) =E= RGDPMP(t);

RGDPPC.L(t) = RGDPPC0(t);

* end: GDP per capita


$ONTEXT
trade-gdp ratio = [real trade (in dom currency)] / [real GDP]
$OFFTEXT


EQ_TRDGDPDEF(tcur(t))..
  TRDGDP(t)*RGDPMP(t) =E= SUM(c, EXR00*pwe00(c)*QE(c,t))
    + SUM(c, EXR00*PWM00(c)*QM(c,t));

EQ_ABSNOMDEF(tcur(t))..
  ABSNOM(t) =E= SUM((c,h), PQD(c,h,t)*QH(c,h,t))
  + SUM((c,insngo), PQD(c,insngo,t)*QNGO(c,insngo,t))
  + SUM((c,fcap), PQD(c,fcap,t)*capcomp(fcap,c)*SUM(ins2, DKINS(ins2,fcap,t)))
  + SUM((c,dstk), PQD(c,dstk,t)*SUM(ins2, qdstk(c,ins2,t)))
  + SUM((c,insgov), PQD(c,insgov,t)*QG(c,t));



$ONTEXT
HL: Definitions of GDP shares for government receipts, government spending,
    and non-gov payments.

HL?? Is there also a need to defined absorption shares.

[MC: I tend to use GDP shares. However, I know you prefer absorption shares, and
for good reasons. Perhaps, version 2 (and GEM more generally) should offer both
alternatives.]

Note that it could be defined using the GDP shares along the following lines (intuitive alt.):
  GOVRECABS(acgovrec,t) =E= GOVRECGDP(acgovrec,t)*ABSNOM(t)/GDPMP(t)
  or (less intuitive):
  GOVRECABS(acgovrec,t)*GDPMP(t) =E= GOVRECGDP(acgovrec,t)*ABSNOM(t)
In either case, one would need a variable for ABSNOM.

[MC: Nice simplification!]

$OFFTEXT

EQ_GOVRECGDPDEF(acgovrec,tcur(t))$GOVRECGDP00(acgovrec)..
  GOVRECGDP(acgovrec,t)*GDPMP(t) =E=
    + SUM(c, TQ(c,t)*PQS(c,t)*QQ(c,t))$taxcom(acgovrec)
    + SUM(a, TA(a,t)*PA(a,t)*QA(a,t))$taxact(acgovrec)
    + SUM(insdng, TY(insdng,t) * YI(insdng,t))$taxdir(acgovrec)
    + YTAXIMP(t)$taximp(acgovrec)
    + YTAXEXP(t)$taxexp(acgovrec)
    + YTAXVAT(t)$taxvatc(acgovrec)
    + SUM(f, TF(f,t)*YF(f,t))$taxfac(acgovrec)
    + SUM((insgov,insdng), TRII(insgov,insdng,t))$trgovngov(acgovrec)
    + (EXR(t)*SUM((insgov,insrow), trnsfr(insgov,insrow,t)))$trgovrow(acgovrec)
    + NDFG(t)$netdomfin(acgovrec)
    + (EXR(t)*NFFG(t))$netforfingov(acgovrec)

    ;
*!!
* to add:
*    + SUM((insgov,f), YIF(insgov,f,t))$trgovfac(acgovrec);



EQ_GOVSPNDGDPDEF(acgovspnd,tcur(t))..
  GOVSPNDGDP(acgovspnd,t)*GDPMP(t) =E=
      (SUM((insdng,insgov), TRNSFR(insdng,insgov,t))*CPI(t))$trngovgov(acgovspnd)
    + (SUM((insrow,insgov), EXR(t)*TRNSFR(insrow,insgov,t)))$trrowgov(acgovspnd)
    + SUM((c,insgov), PQD(c,insgov,t)*QG(c,t))$congov(acgovspnd)
    + SUBCT(t)$subcom(acgovspnd)
    + SUM(fcapg$macgovspnd(acgovspnd,fcapg), (SUM(ins2, DKINS(ins2,fcapg,t))*PK(fcapg,t))$fcapgz(acgovspnd));


EQ_NGOVPAYGDPDEF(acngovpay,tcur(t))..
  NGOVPAYGDP(acngovpay,t)*GDPMP(T) =E=
    SUM((insdng,insrow), TRNSFR(insdng,insrow,t)*EXR(t))$trngovrow(acngovpay)
  + SUM((insrow,insdng), TRII(insrow,insdng,t))$trrowngov(acngovpay)
  + SUM((f,insrow), TRNSFR(f,insrow,t)*EXR(t))$trfacrow(acngovpay)
*  + SUM((insrow,f), TRNSFR(insrow,f,t)*EXR(t))$trrowfac(acngovpay)
  + SUM((insrow,f), YIF(insrow,f,t))$trrowfac(acngovpay)
  + SUM(insdng, SAV(insdng,t))$savngov(acngovpay)
  + (EXR(t)*NFFINS(t))$netforfinngov(acngovpay)
  + SUM(fcapng$macngovpay(acngovpay,fcapng), DKINS('ngovz',fcapng,t)*PK(fcapng,t))$fcapngz(acngovpay)
  + SUM((insrow,fcapng), DKINS('rowz',fcapng,t)*PK(fcapng,t))$fdi(acngovpay)
  + (TRSMREC(t)*EXR(t))$tourismrec(acngovpay);



EQ_GOVRECABSDEF(acgovrec,tcur(t))$GOVRECABS00(acgovrec)..
  GOVRECABS(acgovrec,t) =E= GOVRECGDP(acgovrec,t)/GDPMP(t)*ABSNOM(t);

EQ_GOVSPNDABSDEF(acgovspnd,tcur(t))..
  GOVSPNDABS(acgovspnd,t) =E= GOVSPNDGDP(acgovspnd,t)/GDPMP(t)*ABSNOM(t);

EQ_NGOVPAYABSDEF(acngovpay,tcur(t))..
  NGOVPAYABS(acngovpay,t) =E= NGOVPAYGDP(acngovpay,t)/GDPMP(t)*ABSNOM(t);



* Emissions

EQ_EMIACTC(ghg,c,a,tcur(t))$(QEMI00(ghg,c,a) AND QINT00(c,a))..
  QEMI(ghg,c,a,t) =E= iemi(ghg,c,a,t)*QINT(c,a,t);

EQ_EMIACTF(ghg,f,a,tcur(t))$(QEMI00(ghg,f,a) AND QF00(f,a))..
  QEMI(ghg,f,a,t) =E= iemi(ghg,f,a,t)*QF(f,a,t);

EQ_EMIACT2(ghg,ac,a,tcur(t))$(QEMI00(ghg,ac,a) AND NOT SAM(ac,a))..
  QEMI(ghg,ac,a,t) =E= iemi(ghg,ac,a,t)*QA(a,t);

EQ_EMIHHD(ghg,c,h,tcur(t))$QEMI00(ghg,c,h)..
  QEMI(ghg,c,h,t) =E= iemi(ghg,c,h,t)*QH(c,h,t);

EQ_EMIHHD2(ghg,ac,h,tcur(t))$(QEMI00(ghg,ac,h) AND NOT SAM(ac,h))..
  QEMI(ghg,ac,h,t) =E= iemi(ghg,ac,h,t)*SUM(c, PQD00(c,h)*QH(c,h,t));

EQ_EMIGOV(ghg,c,tcur(t))$SUM(insgov, QEMI00(ghg,c,insgov))..
  SUM(insgov, QEMI(ghg,c,insgov,t)) =E= SUM(insgov, iemi(ghg,c,insgov,t)*QG(c,t));

EQ_EMIGOV2(ghg,ac,tcur(t))$(SUM(insgov, QEMI00(ghg,ac,insgov)) AND NOT SUM(insgov, SAM(ac,insgov)))..
  SUM(insgov, QEMI(ghg,ac,insgov,t)) =E= SUM(insgov, iemi(ghg,ac,insgov,t)*SUM(c , PQD00(c,insgov)*QG(c,t)));







* Debt Stocks
$ONTEXT
Definitions of borrowing and debt for government and non-government, domestic
and foreign.
$OFFTEXT

EQ_GOVDOMBOR(tcur(t))..
  GBOR(t) =E= NDFG(t) + gintrat(t)*GDEBT(t);

EQ_GOVDOMDEBT(tcur(t))$(NOT tmin(t))..
  GDEBT(t) =E= GDEBT.L(t-1) + GBOR.L(t-1);

EQ_GOVFORBOR('govz',tcur(t))..
  FBOR('govz',t) =E= NFFG(t) + fintrat('govz',t)*FDEBT('govz',t);

EQ_NGOVFORBOR('ngovz',tcur(t))..
  FBOR('ngovz',t) =E= NFFINS(t) + fintrat('ngovz',t)*FDEBT('ngovz',t);

EQ_FORDEBT(ins2,tcur(t))$(NOT tmin(t))..
  FDEBT(ins2,t) =E= FDEBT.L(ins2,t-1) + FBOR.L(ins2,t-1);

GDEBT.FX(tmin) = GDEBT00;
FDEBT.FX(ins2,tmin) = FDEBT00(ins2);

* END: Equations - definition =======================================

* START: Model - declaration and definition =========================



MODEL GEM /
  EQ_WFADEF
  EQ_PRODFN
  EQ_PRODFN1
  EQ_FACDEM

  EQ_FACDEMLEO
  EQ_PRODFNCES2
  EQ_FACDEMCES2
  EQ_PRODFNCES3
  EQ_FACDEMCES3

  EQ_TFPDEF
  EQ_FPRDADEF
  EQ_INTDEM

  EQ_COMPRDFN
  EQ_OUTAGGFOC
  EQ_OUTAGGFN

  EQ_PVADEF
  EQ_PADEF
  EQ_WAGECURVE
  EQ_FACEQ
  EQ_YFDEF
  EQ_YCAPGDEF

  EQ_ARMING
  EQ_ARMING2
  EQ_IMPDOMRAT
  EQ_PDDDEF
  EQ_ABSORB
  EQ_PQDDEF
  EQ_CET
  EQ_CET2
  EQ_EXPDOMRAT
  EQ_ESUPPLYEXOG
  EQ_OUTVAL

  EQ_QMCONST.PRQMBAR
  EQ_IMPQUOTARENT
  EQ_HHDIMPQUOTARENT
  EQ_ACTIMPQUOTARENT
  EQ_GOVIMPQUOTARENT
  EQ_INVIMPQUOTARENT

  EQ_FOREXRENT
  EQ_FOREXRENTALLOC

  
  EQ_PMDEF
  EQ_PEDEF
  EQ_EDEMAND

  EQ_QTDEM

  EQ_SHIFDEF
  EQ_YIDEF
  EQ_YIFDEF
  EQ_MPSDEF
  EQ_INSSAVDEF
  EQ_TRIIDEF
  EQ_EHDEF
  EQ_HHDDEM

  EQ_ENGODEF
  EQ_NGODEM
  EQ_TRSTDEM
  EQ_TRSMRECDEF

  EQ_GOVREV
  EQ_TYDEF
  EQ_TFDEF
  EQ_TADEF
  EQ_TQDEF
  EQ_TEDEF
  EQ_TMDEF
  EQ_TVACDEF
  EQ_TFADEF
  EQ_VATREBATEDEF
  EQ_SUBCDEF
  EQ_YTAXEXPDEF
  EQ_YTARIMPDEF
  EQ_YTAXVATDEF
  EQ_GOVEXP
  EQ_GOVDEM
  EQ_SUBCTDEF

  EQ_TRHROWDEF
  EQ_TRINSDNHROWDEF
  
  EQ_TRFACROWDEF

  EQ_TRHGOVDEF
  EQ_TRINSDNHGOVDEF
  
  EQ_TRROWGOVDEF
  EQ_TRGOVROWDEF
*  EQ_TRROWFACDEF
  
  EQ_GOVPRIMDEF
  EQ_GOVPRIMDEFREALDEF
  EQ_GOVINVCOST
  EQ_GOVCAPACC
  EQ_GOVNETDOMFINREAL
  EQ_NGOVINVFIN

  EQ_NGOVINVCOST

  EQ_NGOVNETFORFIN
  EQ_DKGOVDEF
  EQ_DKNGOVDEF
  EQ_DKROWDEF
  EQ_PCAPDEF
  EQ_INVDEM
  EQ_CAPACCUMNGOVDOM
  EQ_CAPACCUMNGOVFOR
  EQ_CAPACCUMGOV
  EQ_LABENDOWDEF
  EQ_OTHENDOWDEF
  EQ_FACSUP
  EQ_LABPARTRATDEF

  EQ_WFAVGDEF
  EQ_NEWCAPALLOC
  EQ_CAPACCUMACT

  EQ_COMEQ
  EQ_CURACC
  EQ_CAPACC
  EQ_CPIDEF
  EQ_DPIDEF
  EQ_REXRDEF

  EQ_GDPREALFCDEF
  EQ_GDPMPDEF
  EQ_GDPREALMPDEF
  EQ_GDPPCREALDEF
  
  EQ_TRDGDPDEF
  EQ_ABSNOMDEF

  EQ_GOVRECGDPDEF
  EQ_GOVSPNDGDPDEF
  EQ_NGOVPAYGDPDEF

  EQ_GOVRECABSDEF
  EQ_GOVSPNDABSDEF
  EQ_NGOVPAYABSDEF

* Emissions
  EQ_EMIACTC
  EQ_EMIACTF
  EQ_EMIACT2
  EQ_EMIHHD
  EQ_EMIHHD2
  EQ_EMIGOV
  EQ_EMIGOV2

  EQ_GOVDOMBOR
  EQ_GOVDOMDEBT
  EQ_GOVFORBOR
  EQ_NGOVFORBOR
  EQ_FORDEBT

  EQ_INVVALFDEF

  EQ_CAPACCUMNGOVHHD
  EQ_CAPREDIST
  EQ_CAPREDISTCONST

/;

* lower limits for variables
PARAMETER lowlim /0.00001/;
$ONTEXT
CPI.LO         = lowlim;
EG.LO          = lowlim;
EH.LO(h)       = lowlim;
PA.LO(a)       = lowlim;
*PDS.LO(c)       = lowlim;
*PDD.LO(c)       = lowlim;
PE.LO(c)       = lowlim;
PM.LO(c)       = lowlim;
PQ.LO(c)       = lowlim;
PVA.LO(a)      = lowlim;
QA.LO(a)       = lowlim;
QD.LO(c)       = lowlim;
QE.LO(c)       = lowlim;
QF.LO(f,a)     = lowlim;
QG.LO(c)       = lowlim;
QH.LO(c,h)     = lowlim;
QINV.LO(c)     = lowlim;
QM.LO(c)       = lowlim;
QQ.LO(c)       = lowlim;
QX.LO(c)       = lowlim;
WF.LO(f)       = lowlim;
WFDIST.LO(f,a) = lowlim;
YF.LO(f)       = lowlim;
YG.LO          = lowlim;
YI.LO(insdng)  = lowlim;
$OFFTEXT

* END: Model - declaration and definition ===========================


* START: Simulation sets - declarations and definitions =============

SET
* simulations
  sim    all simulations
    /
      baseyr       used in reports
      baseyr-pov   used in poverty and ineq reports
      fixgdp-run    model run with fix gdp + flex tfp
      ref0         reference scenario -- flex gdp + fix tfp

    /

  simcur(sim)    current simulations
    /
      fixgdp-run
      ref0
    /

  simfixgdp(sim)    simulations with fix gdp
    /
      fixgdp-run
    /

  simfixtfp(sim)
    /
      ref0
    /

;

* END: Simulation sets - declarations and definitions ===============

* START: Closure rule parameters for simulation - declarations ======


PARAMETER
  facclossim(sim,f)                  closure rule market for factof f in ref0
  numerairesim(sim)                  numeraire in sim
  govclossim(sim,t)                  closure rule government in simulation sim
  siclossim(sim)                     closure rule savings-investment in simulation sim
  rowclossim(sim,t)                    closure rule rest of the world in simulation sim

  govrecrulesim(sim,acgovrec)        rule for government receipts acgovrec
  govrecgrwsim(sim,acgovrec,t)       growth rate government receipts acgovrec
  govrecgdpsim(sim,acgovrec,t)       GDP shr for government receipt acgovrec
  govrecabssim(sim,acgovrec,t)       absorption shr for government receipt acgovrec

  govspndrulesim(sim,acgovspnd)      rule for government spending acgovspnd
  govspndgrwsim(sim,acgovspnd,t)     growth rate government spending acgovspnd
  govspndgdpsim(sim,acgovspnd,t)     GDP shr for government spending acgovspnd
  govspndabssim(sim,acgovspnd,t)     absorption shr for government spending acgovspnd

  ngovpayrulesim(sim,acngovpay)      rule for non-government payment acngovpay
  ngovpaygrwsim(sim,acngovpay,t)     growth rate for non-government payment acngovpay
  ngovpaygdpsim(sim,acngovpay,t)     GDP shr for non-government payment acngovpay
  ngovpayabssim(sim,acngovpay,t)     absorption shr for non-government payment acngovpay

  govrecindexsim(sim,acgovrec,t)     index for government receipt acgovrec
  govspndindexsim(sim,acgovspnd,t)   index for government spending acgovspnd
  ngovpayindexsim(sim,acngovpay,t)   index for non-government payment acngovpay

*HL-Start
  qfinsscalsim(sim,f,t)              scaling factor for real factor endowments
  qfsim(sim,f,a,t)                   demand for factor f in activity a
*HL-End

  taxratesim(sim,acgovrec,ac,t)       rate for tax type ac imposed on acp in t1 (deviation wrt baseyr)
  taxrate2sim(sim,acgovrec,ac,t)      rate for tax type ac imposed on acp in t1 (level)
  taxrate3sim(sim,acgovrec,ac,t)      rate for tax type ac imposed on acp in t1 (level)

;

* END: ُClosure rule parameters for simulation - declarations =======

* START: Closure rule parameters for simulation - definitions =======

* Note: These definitions are defaults and may change as part of
* simulations in sim.gms.


* Government Receipts

* fix rate of tax on factor use for factors not in SAM and in VA
TFA.FX(f,a,t)$(NOT fsam(f) AND fva(f)) = 0;

* Government Payments

govrecrulesim(sim,acgovrec)  = govrecrule0(acgovrec);
govrecgrwsim(sim,acgovrec,t) = govrecgrw0(acgovrec,t);
govrecgdpsim(sim,acgovrec,t) = govrecgdp0(acgovrec,t);
govrecabssim(sim,acgovrec,t) = govrecabs0(acgovrec,t);

govrecindexsim(sim,acgovrec,t)   = govrecindex0(acgovrec,t);

govspndrulesim(sim,acgovspnd)  = govspndrule0(acgovspnd);
govspndgrwsim(sim,acgovspnd,t) = govspndgrw0(acgovspnd,t);
govspndgdpsim(sim,acgovspnd,t) = GOVSPNDGDP0(acgovspnd,t);
govspndabssim(sim,acgovspnd,t) = GOVSPNDABS0(acgovspnd,t);

govspndindexsim(sim,acgovspnd,t) = govspndindex0(acgovspnd,t);

taxratesim(sim,acgovrec,ac,t) = taxrate0(acgovrec,ac,t);
taxrate2sim(sim,acgovrec,ac,t) = taxrate20(acgovrec,ac,t);
taxrate3sim(sim,acgovrec,ac,t) = 0;

* Non-Government Payments

* if siclos0 = 1, need to overwrite selection for ngovpayrule0
ngovpayrule0(fcapngz)$(siclos0=1) = 1;

ngovpayrulesim(sim,acngovpay)   = ngovpayrule0(acngovpay);
ngovpaygrwsim(sim,acngovpay,t)  = ngovpaygrw0(acngovpay,t);
ngovpaygdpsim(sim,acngovpay,t)  = NGOVPAYGDP0(acngovpay,t);
ngovpayabssim(sim,acngovpay,t)  = NGOVPAYABS0(acngovpay,t);

ngovpayindexsim(sim,acngovpay,t) = ngovpayindex0(acngovpay,t);


*HL-Start
*Added to mod.gms since reference to these in macclos.inc and facclos.inc

*qfinsscalsim(sim,f,t)              scaling factor for real factor endowments
 qfinsscalsim(sim,f,t) = 0;

*qfsim(sim,f,a,t)                  quantity demanded of factor f from activity a
 qfsim(sim,f,a,t) = 0;

*HL-End



* Factor Markets

facclossim(sim,f) = facclos0(f);

* Government

govclossim(sim,t) = govclos0;

* Rest of the World

rowclossim(sim,t) = rowclos0(t);

* Savings-Investment

siclossim(sim) = siclos0;

* Numeraire

numerairesim(sim) = numeraire0;

* diagnose model closure
$INCLUDE diagnostics-clos.inc



* END: Closure rule parameters for simulation: default definitions ==

* world prices
PWE.FX(c,t)$(NOT ced(c)) = PWE0(c,t);
PWE.FX(c,t)$(NOT QE00(c) AND ced(c)) = PWE0(c,t);
PWE.LO(c,t)$(QE00(c) AND ced(c)) = -INF;
PWE.UP(c,t)$(QE00(c) AND ced(c)) = +INF;
PWM.FX(c,t) = PWM0(c,t);


* factor productivity
FPRDASCAL.FX(t) = FPRDASCAL0(t);


* START: Variables with zero initial values - fixing at zero ========

* Note: The parts of the declared domains of variables with a zero base value
* are fixed at zero and never part of the model.


DKINS.FX(ins2,f,t)$(DKINS00(ins2,f)=0)          = 0;
FPRDA.FX(f,a,t)$(QF00(f,a)=0)                 = 0;
MPS.FX(ins,t)$(MPS00(ins)=0)                  = 0;
SAV.FX(ins,t)$(SAV00(ins)=0)                  = 0;
PDD.FX(c,t)$(QD00(c)=0)                       = 0;
PDS.FX(c,t)$(QD00(c)=0)                       = 0;
PE.FX(c,t)$(QE00(c)=0)                        = 0;
PM.FX(c,t)$(QM00(c)=0)                        = 0;
PQD.FX(c,ac,t)$(PQD00(c,ac) = 0)              = 0;
PQS.FX(c,t)$(QQ00(c)=0)                       = 0;
QA.FX(a,t)$(QA00(a)=0)                        = 0;
QD.FX(c,t)$(QD00(c)=0)                        = 0;
QE.FX(c,t)$(QE00(c)=0)                        = 0;
QF.FX(f,a,t)$(QF00(f,a)=0)                    = 0;
QFINS.FX(ins,f,t)$(QFINS00(ins,f)=0)          = 0;
QG.FX(c,t)$(QG00(c)=0)                        = 0;
QH.FX(c,h,t)$(QH00(c,h)=0)                    = 0;
QNGO.FX(c,insngo,t)$(QNGO00(c,insngo)=0)      = 0;
QTRST.FX(c,instrst,t)$(QTRST00(c,instrst)=0)  = 0;
TRSMREC.FX(t)$(TRSMREC00=0)                   = 0;
QINT.FX(c,a,t)$(QINT00(c,a)=0)                = 0;
QINV.FX(c,t)$(QINV00(c)=0)                    = 0;
QM.FX(c,t)$(QM00(c)=0)                        = 0;
QQ.FX(c,t)$(QQ00(c)=0)                        = 0;
QT.FX(c,t)$(QT00(c)=0)                        = 0;
QX.FX(c,t)$(QX00(c)=0)                        = 0;
SHIF.FX(ins,f,t)$(SHIF00(ins,f)=0)            = 0;
TRII.FX(ins,insp,t)$(TRII00(ins,insp)=0)      = 0;
TRNSFR.FX(ac,acp,t)$(TRNSFR00(ac,acp)=0)      = 0;
WFDIST.FX(f,a,t)$(QF00(f,a)=0)                = 0;
WFA.FX(f,a,t)$(QF00(f,a)=0)                   = 0;
YF.FX(f,t)$(YF00(f)=0)                        = 0;
YI.FX(ins,t)$(YI00(ins)=0)                    = 0;
YIF.FX(ins,f,t)$(YIF0(ins,f,t)=0)             = 0;
INVVAL.FX(t)$(INVVAL00=0)            = 0;
INVVALF.FX(t)$(INVVALF00=0)                   = 0;
QFHEND.FX(h,f,t)$(QFINS00(h,f)=0)             = 0;

PVA.FX(a,t)$(PVA00(a)=0) = 0; 

* END: Variables with zero initial values - fixing at zero ==========


* START: Solving the model ==========================================


*OPTION CNS = PATH;
OPTION CNS = CONOPT;
OPTION MCP = PATH;

OPTION DOMLIM = 1000;


OPTION LIMROW=1000, LIMCOL=1000;
*OPTION LIMROW=3, LIMCOL=3;

OPTION SOLVELINK = 5;

*GEM.HOLDFIXED=1;
GEM.TOLINFREP = 1e-10;

IF (dmod=2 OR dmod=0,
  GEM.ITERLIM=0;
);


*### SOLUCION 1 ###

* START: FLEX TFPSCAL + FIX GDP ++++++++++++++++++++++++++++++++++++++


* dcal01 = 1 to activate EQ_PRODFN1 and deactivate EQ_PRODFN
dcal01 = 1;



*### flex TFPSCAL
TFPSCAL.LO(t) = -INF;
TFPSCAL.UP(t) = +INF;
FPRDASCAL.FX(t) = FPRDASCAL0(t);
* alt: flex labor-specific productivity
*TFPSCAL.FX(t) = TFPSCAL0(t);
*FPRDASCAL.LO(t) = -INF;
*FPRDASCAL.UP(t) = +INF;



*### fix RGDPFC
RGDPFC.FX(t) = RGDPFC0(t);
RGDPPC.LO(t) = -INF;
RGDPPC.UP(t) = +INF;
*alt: fix RGDPPC
*RGDPFC.LO(t) = -INF;
*RGDPFC.UP(t) = +INF;
*RGDPPC.FX(t) = RGDPPC0(t);

DISPLAY RGDPFC.LO, RGDPFC.UP, RGDPFC.L, RGDPFC0;

* define initial levels for all model variables using 0-parameters
$INCLUDE varinit.inc

* iteration over simfixgdp
LOOP(sim$simfixgdp(sim),

* select closure rule for factor markets
$INCLUDE facclos.inc

  LOOP(t$tsol(t),
* select macro closure rule
$INCLUDE macclos.inc
  );

* iteration over tsol
  LOOP(tsol,

* assign time period tcur (the period for our single-period model)
    tcur(t)    = NO;
    tcur(tsol) = YES;

* valor variables endogenas t = valor variables endogenas t-1
* dmod = 1
    IF (dmod=1,
$INCLUDE varinit-t2.inc
    );

*    SOLVE GEM USING CNS;
    SOLVE GEM USING MCP;


* diagnose solution
$INCLUDE diagnostics-sol.inc

  );

);

* END: FLEX TFPSCAL + FIX GDP ++++++++++++++++++++++++++++++++++++++++

* activate the following sentence to debug
*$EXIT


DISPLAY 'sol1', RGDPFC0, RGDPFC.L;





*### SOLUCION 2 ###

* dcal01 = 0 to deactivate EQ_PRODFNCES1 and activate EQ_PRODFNCES
dcal01 = 0;

* next line relevant for equation EQ_PRODFNCES
QFINS0(ins,fcap,t) = QFINS.L(ins,fcap,t);


* START: MULTI-PASS + FIX TFPSCAL ++++++++++++++++++++++++++++++++++++

*### fix TFPSCAL

* re-define tfpexog0
tfpexog0(a,t) = (1 + TFPSCAL.L(t)*tfp01(a));
tfpexog(a,t) = tfpexog0(a,t);
TFPSCAL.FX(t) = 0;
* alt:
*TFPSCAL.FX(t) = TFPSCAL.L(t);
*FPRDASCAL.FX(t) = FPRDASCAL.L(t);

*### flex GDPREALFC
RGDPFC.LO(t) = -INF;
RGDPFC.UP(t) = +INF;
* alt:
*RGDPPC.LO(t) = -INF;
*RGDPPC.UP(t) = +INF;
 

* iteracion sobre simfixtfp
LOOP(sim$simfixtfp(sim),

* iteracion sobre tsol
  LOOP(tsol,

* assign time period tf (the period for our single-period model)
    tcur(t)    = NO;
    tcur(tsol) = YES;

*   SOLVE GEM USING CNS;
   SOLVE GEM USING MCP;

* diagnosticar solucion
$INCLUDE diagnostics-sol.inc

  );

);

* limite iteraciones = 1000 (default)
GEM.ITERLIM = 100000;
DISPLAY '1', RGDPFC0, RGDPFC.L;
* redefining initial values for 0 parameters using model solution
$INCLUDE par-redefn-0.inc
DISPLAY '2', RGDPFC0, RGDPFC.L;

* END: Solving the model ============================================



DISPLAY inv, invng, invg;

*HL?? checking on sum-up for investment allocation

*[MC: Thanks. I did the same thing a while ago!]

*DKA(fcap,a,t) =E= SUM(invng$mfcapinv(fcap,invng), QINVDEST(invng,t))

PARAMETER
 invchk
 capstockchk
 ;
 invchk('lhs',fcap,t)$tsol(t) = SUM(a, DKA.L(fcap,a,t));
 invchk('rhs',fcapng,t)$tsol(t) = SUM(ins2, DKINS.L(ins2,fcapng,t));
 invchk('gap',fcap,t)$tsol(t) = invchk('lhs',fcap,t) - invchk('rhs',fcap,t);

DISPLAY invchk;
DISPLAY trnsfr.l;

capstockchk('lhs',fcap,t)$tsol(t) = SUM(ins, QFINS.L(ins,fcap,t+1)) - SUM(ins, QFINS.L(ins,fcap,t))*(1-deprcap(fcap));
capstockchk('rhs',fcap,t)$tsol(t) = SUM(ins2, DKINS.L(ins2,fcap,t));
capstockchk('gap',fcap,t)$tsol(t) = capstockchk('lhs',fcap,t) - capstockchk('rhs',fcap,t);



$STOP

*### SOLUCION 3 ###

SET t2030(t)/2020*2030/;
REXR.FX(t)$t2030(t) = REXR0(t)*1.1; 

* iteracion sobre tsol
LOOP(tsol,

* assign time period tf (the period for our single-period model)
  tcur(t)    = NO;
  tcur(tsol) = YES;

*   SOLVE GEM USING CNS;
 SOLVE GEM USING MCP;

* diagnosticar solucion
$INCLUDE diagnostics-sol.inc

);






$EXIT

*$ONTEXT
*### Shock Simulation

* duble the numeraire
CPI.FX(t) = CPI0(t)*2;

* double the labor supply
*QFS.FX('f-lab') = QFS.L('f-lab')*2;

* unilateral tariff elimination
*tm(c) = tm0(c)*.0;

* increase in the world price of imports
*pwm(c,t)$(NOT tmin(t)) = pwm(c,t)*1.25;

SOLVE GEM USING CNS;
DISPLAY QQ.L, QA.L, QF.L, QH.L, PQD.L, PA.L, WF.L, YI.L, QINT.L, QINV.L, trnsfr.L,
        EG.L, SAVF.L, EXR.L, QE.L, QM.L, CPI.L, WFDIST.L, GPRIMDEF.L, TRII.L,
        DPI.L, WALRAS.L;
*$OFFTEXT

DISPLAY ac;

DISPLAY "#### END: mod.gms";

