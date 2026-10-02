* repdebtbor.inc

DISPLAY "#### START: repdebtbor.inc";


PARAMETERS
 gborxx(t,sim)     net gov domestic borrowing from ac
 gdebteoyxx(t,sim) net gov domestic debt (end-of-year) owed to ac
 gdebtxx(t,sim)    net gov domestic debt (avg beg & end yr) owed to ac
 gborgdpxx(t,sim)  net gov domestic borrowing from ac (share not % of GDP)
 gdebtgdpxx(t,sim) net gov domestic debt (avg beg & end yr) owed to ac (share not % of GDP)
 ndfgsourcegdp(*,t,sim) net gov domestic financing by source (share not % of GDP)

 fborxx(ac,t,sim)     net foreign borrowing by ac
 fdebteoyxx(ac,t,sim) net foreign debt (end-of-year) owed by ac
 fdebtxx(ac,t,sim)    net foreign debt (avg beg & end yr) owed by ac
 fborgdpxx(ac,t,sim)  net foreign borrowing by ac (share % of GDP)
 fdebtgdpxx(ac,t,sim) net foreign debt (avg beg & end yr) owed by ac (share not % of GDP)
 ; 
 
$ONTEXT
 gborxx(ac,sim,t)     = 0; 
 gdebteoyxx(ac,sim,t) = 0;      
 gdebtxx(ac,sim,t)    = 0;      
 gborgdpxx(ac,sim,t)  = 0;      
 gdebtgdpxx(ac,sim,t) = 0;      
 ndfgsourcegdp(sim,ac,t) = 0;   
 fborxx(ac,sim,t)     = 0;      
 fdebteoyxx(ac,sim,t) = 0;      
 fdebtxx(ac,sim,t)    = 0;      
 fborgdpxx(ac,sim,t)  = 0;      
 fdebtgdpxx(ac,sim,t) = 0;      
$OFFTEXT

DISPLAY 
"Note: Domestic and foreign debt and borrowing data are computed in the file repdebtbor.inc, included in rep.gms."
 datagdebt01, datafdebt01;




$ONTEXT

Derivation of the definitions of gborx and fborx (below):

Notation: B = borrowing, F = financing; D(t) = debt at end-of-year t
Recall that F(t) = B(t) - I(t) where I(t) = interest payment

0. I(t) = i(t)*[D(t) + D(t-1)]/2 = i(t)*0.5*[D(t) + D(t-1)]
[int pay in t] = [int rate]*[avg of debts at end of t and end of t-1]

1. B(t) = F(t) + i(t)*0.5*[D(t) + D(t-1)]
[borrowing] = [financing] + int pay]  

2. D(t) = D(t-1) + B(t)
[debt at end of t] = [debt at end of t-1] + [borrowing in t]

Using 2 to substitute for D(t) in 1:
(3) B(t) = F(t) + i(t)*[D(t-1) + B(t) + D(t-1)]/2

(3) B(t) = F(t) + i(t)*0.5*[D(t-1) + B(t) + D(t-1)]
         = F(t) + i(t)*0.5*2*D(t-1) + i(t)*0.5*B(t) 
		 = F(t) + i(t)*D(t-1) + 0.5*i(t)*B(t) 
 

Solving for B(t):

  B(t) - 0.5*i(t)*B(t) = F(t) + i(t)*D(t-1)

  B(t)*[1 - 0.5*i(t)] = F(t) + i(t)*D(t-1)

  B(t) = [F(t) + i(t)*D(t-1)] / [1 - 0.5*i(t)]
  
Above:   
(2) D(t) = D(t-1) + B(t)

Computing average debt (average of end-of-year debts in t and t-1):
   DAVG(t) = 0.5*[D(t) + D(t-1)]

Computing interest payment
   
  I(t) = i(t)*DAVG(t) 
       = i(t)*0.5*[D(t) + D(t-1)] 
	   = i(t)*0.5*[D(t-1) + B(t) + D(t-1)] 
	   = i(t)*0.5*[2D(t-1) + B(t)] 
	   = i(t)*D(t-1) + 0.5*B(t)] 
	   = i(t)*D(t-1) + 0.5*i(t)*B(t)	   

-------------------------
Check: F(t) = B(t) - I(t) ==> B(t) = F(t) + I(t)
-------------------------

RHS1 (1st term on RHS of starting point): As noted above:
B(t) = [F(t) + i(t)*D(t-1)] / [1 - 0.5*i(t)]

RHS2 (2nd term on RHS of starting point): Using the definition of I(t) 
I(t) = i(t)*D(t-1) + 0.5*i(t)*B(t)	 
     = i(t)*D(t-1) + 0.5*i(t)*[F(t) + i(t)*D(t-1) + 0.5*i(t)*B(t)]
	 

F(t) = RHS1 - RHS2
F(t) = [F(t) + i(t)*D(t-1)] / [1 - 0.5*i(t)] 
       - i(t)*D(t-1) - 0.5*i(t)*[F(t) + i(t)*D(t-1) + 0.5*i(t)*B(t)]
????

  
For government domestic financing, only a fraction of the financing
[1 - shrndfgmssim) is treated as borrowing. The rest is treated as
financing from the monetary system, which does not add to domestic 
government debt.

Thus, for the government:
 S(t) = seignorage
 
 S(t) = F(t)*shrndfgmssim(t)
 B(t) = F(t) - S(t) + 
????


$OFFTEXT

DISPLAY 'data before calculation', debt00, ndfgx, nffgx, nffinsx;


*Defining foreign and domestic financing parameters========

PARAMETER
 nffxx(ins2,sim,t)   net foreign financing by institution ins 
*Note: One parameter is created for all foreign financing (gov and non-gov). 
 ;
 nffxx('govz',sim,t) = NFFGX(t,sim);
 nffxx('ngovz',sim,t) = NFFINSX(t,sim);
DISPLAY nffxx;

PARAMETER
 ndfgxx(t,sim)  net gov financing from domestic ins ac
 ;
 ndfgxx(t,sim) = NDFGX(t,sim);
DISPLAY ndfgxx;

*Defining interest rate  parameters=================
*gintratsim(sim,ins,t) real interest rate on domestic government debt to ins (in insdng)
*fintratsim(sim,ins,t) real interest rate on foreign debt for ins (in insd)

*If no other data provided for gintratsim, data for gintrat0 is used
gintratsim(t,simcur)$(debt00('ngovz','govz') AND (NOT gintratsim(t,simcur))) = gintrat0(t);

*If no other data provided for fintratsim, data for fintrat0 is used
fintratsim(ins2,t,simcur)$(debt00('rowz',ins2) AND (NOT fintratsim(ins2,t,simcur))) = fintrat0(ins2,t);

DISPLAY fintrat0, fintratsim, gintrat0, gintratsim, t, debt00;


 
 
*Defining share of financing from monetary system==========

PARAMETERS
*shrndfgms0(t)       shr of gov net dom financing via monetary system
 shrndfgmssim(t,sim) shr of gov net dom financing via monetary system
 ;
 
 shrndfgmssim(t,sim) = 0;
 
*Net domestic non-bank financing = borrowing - interest 
*If no data for shrndfgms0 for non-base year, use base-year data (if any)
 shrndfgms0(t)$(NOT shrndfgms0(t)) = SUM(tmin, shrndfgms0(tmin));
 

*shrndfgmssim(sim,t1) shr of gov net dom financing via monetary system
*If no data for shrndfgms0 for non-base year, use base-year data (if any)
 shrndfgms0(t)$((NOT shrndfgms0(t)) AND tsol(t)) = SUM(tmin, shrndfgms0(tmin));
  
*If no other data provided for shrndfgsim, data for shrndfgms0 is used
 shrndfgmssim(t,simcur)$(NOT shrndfgmssim(t,simcur)) = shrndfgms0(t);

DISPLAY shrndfgmssim;


*Defining share of financing from monetary system==========

PARAMETER   
 gdebtboyxx(t,sim) gov domestic debt (beginning-of-year) owed to ac
 fdebtboyxx(ins2,t,sim) for debt of institution ac (beginning-of-year)
 ;
 
PARAMETER
 gdebt0101  gov debt to ins Jan 1 of tmin
 gdebt1231  gov debt to ins Dec 31 of tmin
 infl           inflation rate during base year
 infl05         inflation rate during each half of base year
 ;

PARAMETER
 ndfgms(t,sim) net domestic financing from monetary system
 ;
 

IF(datagdebt01,
*START: Domestic borrowing, debt, and interest ======================

*gborxx(ac,sim,t1)     gov domestic borrowing from ac
*gdebteoyxx(ac,sim,t1) gov domestic debt (end-of-year) owed to ac
*gdebtxx(ac,sim,t1)    gov domestic debt (avg beg & end yr) owed to ac
*gborgdpxx(ac,sim,t1)  gov domestic borrowing from ac (decimal share of GDP not %)
*gdebtgdpxx(ac,sim,t1) gov domestic debt (avg beg & end yr) owed to ac (decimal share of GDP not %)
*ndfgsourcegdp(sim,*,t1) gov net domestic financing from the monetary system (decimal share of GDP not %)
 
*tmin======================================================
*Input data for tmin:


 infl = 125.6040026/124.6547393 - 1;
*End-of-period CPI inflation 2018/2019
*Source: WDI
 infl05 = (1 + infl)**0.5 - 1;
DISPLAY infl05;

*Nominal debts
*Note: debt00 is treated as end-of-year debt
 gdebt1231 = debt00('ngovz','govz'); 
 gdebt0101 = 11262; 
*Source: jor2019-data.xlsx, sheet debt00 from 
*        WDI
DISPLAY gdebt0101, gdebt1231;

*Real debts at mid-year price level
 gdebtboyxx(tmin,simcur) = gdebt0101*(1 + infl05);
 gdebteoyxx(tmin,simcur) = gdebt1231/(1 + infl05); 
DISPLAY 'debt for tmin', gdebtboyxx, gdebteoyxx;

*Computing borrowing as difference between beg and end of year debts at mid-year price level
 gborxx(tmin,simcur) = gdebteoyxx(tmin,simcur) - gdebtboyxx(tmin,simcur);
 
*Computing average deflated debt for reporting purposes 
 gdebtxx(tmin,simcur)$gdebteoyxx(tmin,simcur)
  = 0.5*(gdebtboyxx(tmin,simcur) + gdebteoyxx(tmin,simcur));
*!!!	
DISPLAY '1', gborxx, gdebteoyxx, gdebtboyxx;
 

 
*Loop (after tmin)=========================================

LOOP((t,simcur)$((NOT tmin(t)) AND tsol(t)),

 gborxx(t,simcur) 
  = ((1 - shrndfgmssim(t,simcur))*ndfgxx(t,simcur) 
      + gintratsim(t,simcur)*gdebteoyxx(t-1,simcur))
                                /(1 - 0.5*gintratsim(t,simcur));
 gdebteoyxx(t,simcur) 
   = gdebteoyxx(t-1,simcur) + gborxx(t,simcur);         ); 

  
*Debt calc: beg of year and avg of beg and end of each year for t > tmin===

 gdebtboyxx(t,simcur)$(NOT tmin(t)) = gdebteoyxx(t-1,simcur);

 gdebtxx(t,simcur)
  $((NOT tmin(t)) AND tsol(t) AND gdebteoyxx(t,simcur))
  = 0.5*(gdebteoyxx(t,simcur) + gdebteoyxx(t-1,simcur));

 ndfgms(t,sim)$(simcur(sim) and trep(t))
  = shrndfgmssim(t,sim)*ndfgxx(t,sim);
  
DISPLAY '2', gborxx, ndfgms, ndfgxx, gdebtxx;  

 
*GDP shares for borrowing and debt=========================

 gborgdpxx(t,simcur)$gborxx(t,simcur)
  = gborxx(t,simcur)/GDPMPX(t,simcur);
 
 gdebtgdpxx(t,simcur)$gdebtxx(t,simcur)
  = gdebtxx(t,simcur)/GDPMPX(t,simcur);

*Domestic financing by source (share of GDP)================

 ndfgsourcegdp('ms',t,simcur)$tsol(t) = shrndfgmssim(t,simcur)*ndfgxx(t,simcur) / GDPMPX(t,simcur);

 ndfgsourcegdp('non-ms',t,simcur)$tsol(t)
  = (1 - shrndfgmssim(t,simcur))*ndfgxx(t,simcur)
    /GDPMPX(t,simcur);
	
 ndfgsourcegdp('total',t,simcur)$tsol(t)
  = ndfgsourcegdp('ms',t,simcur) + ndfgsourcegdp('non-ms',t,simcur);

  
*END: Domestic borrowing, debt, and interest ========================
                                                                      );


                                                                      
$EXIT

                                                                      
                                                                      
IF(datafdebt01,
*START: Foreign borrowing, debt, and interest =======================

*fborxx(ac,sim,t)     foreign borrowing by ac
*fdebteoyxx(ac,sim,t) foreign debt (end-of-year) owed by ac
*fdebtxx(ac,sim,t)    foreign debt (avg beg & end yr) owed by ac
*fborgdpxx(ac,sim,t)  foreign borrowing by ac (share % of GDP)
*fdebtgdpxx(ac,sim,t) foreign debt (avg beg & end yr) owed by ac (decimal share of GDP not %)


*tmin======================================================



* fborxx(insd,simcur,tmin) 
*  = (nffxx(insd,simcur,tmin) + fintratsim(simcur,insd,tmin)*SUM(insrow, debt00(insrow,insd)))/(1 - 0.5*fintratsim(simcur,insd,tmin));
  
*fdebteoyxx(insd,simcur,tmin) = SUM(insrow, debt00(insrow,insd)) + fborxx(insd,simcur,tmin);
*Instead: fdebteoyxx = debt00(row,insd);
*Also, need to provide fdebtboyxx for tmin to compute fborxx for tmin

*debt00 is the end-of-year debt:
   fdebteoyxx(insd,simcur,tmin) = SUM(insrow, debt00(insrow,insd));

*fdebtboyxx(ac,sim,t) for debt of institution ac(beginning-of-year)
 ;
*HL. Placeholder data.
*Beginning-of-year debt is defined as:
*[End-of-year debt] MINUS the net foreign financing (ignoring 
*interest payments).

 fdebtboyxx(insd,simcur,tmin)$fdebteoyxx(insd,simcur,tmin)
  = fdebteoyxx(insd,simcur,tmin) - nffxx(insd,simcur,tmin);

 fborxx(insd,simcur,tmin) = fdebteoyxx(insd,simcur,tmin) - fdebtboyxx(insd,simcur,tmin);

  
DISPLAY fborxx, fdebteoyxx, fdebtboyxx;

 
*Loop (after tmin)=========================================

LOOP((insd,simcur,t)$((NOT tmin(t)) AND tsol(t)),

 fborxx(insd,simcur,t) 
  = (nffxx(insd,simcur,t) 
      + fintratsim(simcur,insd,t)*fdebteoyxx(insd,simcur,t-1))
                                /(1 - 0.5*fintratsim(simcur,insd,t));

 fdebteoyxx(insd,simcur,t) 
   = fdebteoyxx(insd,simcur,t-1) + fborxx(insd,simcur,t);
 ); 

 
*Avg of debt at beginning and end of each year for tmin====

 fdebtxx(insd,simcur,tmin)$fdebteoyxx(insd,simcur,tmin)
* = 0.5*(SUM(insrow, debt00(insrow,insd)) + fdebteoyxx(insd,simcur,tmin));
*HL. Switched to new data point fdebtboyxx instead of debt00
 = 0.5*(fdebtboyxx(insd,simcur,tmin) + fdebteoyxx(insd,simcur,tmin));

  
*Avg of debt at beginning and end of each year for t > tmin===

 fdebtxx(insd,simcur,t)$((NOT tmin(t)) AND fdebteoyxx(insd,simcur,t))
  = 0.5*(fdebteoyxx(insd,simcur,t) + fdebteoyxx(insd,simcur,t-1));

  
*Totals for borrowing and debt=============================

 fborxx('total',simcur,t)     = SUM(insd, fborxx(insd,simcur,t));
 fdebteoyxx('total',simcur,t) = SUM(insd, fdebteoyxx(insd,simcur,t));
 fdebtxx('total',simcur,t)    = SUM(insd, fdebtxx(insd,simcur,t));
  
  
*GDP shares for borrowing and debt=========================

 fborgdpxx(ac,simcur,t)$fborxx(ac,simcur,t)
  = fborxx(ac,simcur,t)*EXRX(simcur,t)/GDPMPX(simcur,t);
 
 fdebtgdpxx(ac,simcur,t)$fdebtxx(ac,simcur,t)
  = fdebtxx(ac,simcur,t)*EXRX(simcur,t)/GDPMPX(simcur,t);

*END: Foreign borrowing, debt, and interest =========================
                                                                     );
	
	
OPTION 
gborxx:3:1:1, gdebteoyxx:3:1:1, gdebtxx:3:1:1, gborgdpxx:3:1:1, gdebtgdpxx:3:1:1
fborxx:3:1:1, fdebteoyxx:3:1:1, fdebtxx:3:1:1, fborgdpxx:3:1:1, fdebtgdpxx:3:1:1; 

DISPLAY 
 '4', ndfgxx, gborxx, gdebtboyxx, gdebteoyxx, gdebtxx, 
 nffxx, fborxx, fdebtboyxx, fdebteoyxx, fdebtxx,
 gborgdpxx, fborgdpxx, gdebtgdpxx, fdebtgdpxx
 ndfgsourcegdp
; 
$ONTEXT
$OFFTEXT

*==========================================================
*Solvency indicators=======================================
*==========================================================
$ONTEXT

An institution or a nation may be defined as solvent if it has enough current income to
1. repay debts; and
2. meet debt obligations (incl. interest payments)


With the aim of capturing changes in solvency, we have defined the following indicators
using simulation results:

1. for domestic institutions (government and non-government:
   a. ratio between interest payments and current incomes
   b. ratio between net debts (foreign and domestic) and current income

2. for nation
   a. ratio between net foreign debt and GDP
   b. ratio between total net interest payments and BoP current account receipts

For a discussion of solvency, see:
Barth, Richard, and William Hemphill. 2000. Financial Programming and 
Policy: The Casee of Turkey.
IMF Institute, International Monetary Fund, esp. pp. 76 and 133.
$OFFTEXT


SET
*isolv(ac)  indicators of solvency
 isolv      indicators of solvency
  /
 int-y-rat	       ratio bt interest paid and inc -- for insd and nation
 ddebt-y-rat	   ratio bt dom debt (in LCU) and inc -- for gov
 ddebt-GDP-rat	   ratio bt dom debt (in LCU) and GDP -- for gov
 fdebt-y-rat	   ratio bt for debt (in LCU) and inc -- for insd
 fdebt-yforex-rat  ratio bt for debt (in LCU) and inc -- for nation 
 fdebt-GDP-rat     ratio bt for debt (in LCU) and GDP -- for nation
 totnetdebt-y-rat  ratio bt total net debt (in LCU) and inc -- for insd
 fint-yforex-rat   ratio bt for interest payment and for exch income
 /

PARAMETERS
 solvency(ac,isolv,sim,t)  solvency indicators for domestic institutions and nation (%)
 ;

PARAMETERS
 intpay( *,acp,acpp,sim,t) net interest pay to inst ac from inst acp of type acpp by sim and time
 ;
 
 intpay('domestic',insdng,insgov,simcur,t)$gdebtxx(insdng,simcur,t)
  = gintratsim(simcur,insdng,t)*gdebtxx(insdng,simcur,t);
  
 intpay('foreign',insrow,insd,simcur,t)$fdebtxx(insd,simcur,t)
  = fintratsim(simcur,insd,t)*fdebtxx(insd,simcur,t);

 intpay('total',ins,insp,simcur,t) 
  = intpay('domestic',ins,insp,simcur,t) + intpay('foreign',ins,insp,simcur,t);
  

OPTION intpay:1:3:1;  
DISPLAY intpay, gintratsim, fintratsim;

*=====
*For domestic institutions (government and non-government) and nation:
*Ratio between net interest payments and current income (in %). For nation,
*current income = total current for exchange earnings.

*[interest-income ratio for insd] 
*     = 100*[net interest payments for insd]/[current income of insd]
 solvency(insd,'int-y-rat',simcur,t)$tsol(t)
  = 100*SUM(ins, 
    intpay('total',ins,insd,simcur,t) - intpay('total',insd,ins,simcur,t))
	/(simsam(simcur,t,insd,'total')/scaling('sam'));

*[interest-income ratio for nation] 
*     = 100*[net interest payments to RoW]/[current foreign exchange income of nation]
 solvency('nation','fint-yforex-rat',simcur,t)$tsol(t)
  = 100*SUM((insrow,insd), intpay('total',insrow,insd,simcur,t))
  / (SUM(insrow, simsam(simcur,t,'total',insrow) - SUM(caprow, simsam(simcur,t,caprow,insrow)))
    /scaling('sam'));

*=====
*For government:
*Ratio between domestic debt and current revenue (in %)

 solvency(insgov,'ddebt-y-rat',simcur,t)$tsol(t)
 = 100*gdebtxx('total',simcur,t) / (simsam(simcur,t,'total',insgov)/scaling('sam'));

*=====
*For government:
*Ratio between domestic debt and (nominal) GDP (in %)

solvency(insgov,'ddebt-GDP-rat',simcur,t)$tsol(t)
 = 100*gdebtxx('total',simcur,t) / GDPMPX(simcur,t);

*=====
*For domestic institutions (government and non-government) and nation:
*Ratio between foreign debt and current income (in %). For nation,
*current income = total current for exchange earnings.

solvency(insd,'fdebt-y-rat',simcur,t)$tsol(t)
 = 100*fdebtxx(insd,simcur,t)*EXRX(simcur,t) / (simsam(simcur,t,'total',insd)/scaling('sam'));

solvency('nation','fdebt-yforex-rat',simcur,t)$tsol(t)
 = 100*SUM(insd, fdebtxx(insd,simcur,t)*EXRX(simcur,t))
  / (SUM(insrow, simsam(simcur,t,'total',insrow) - SUM(caprow, simsam(simcur,t,caprow,insrow)))
     /scaling('sam'));

*=====
*For nation:
*Ratio between foreign debt and (nominal) GDP (in %).

solvency(insd,'fdebt-GDP-rat',simcur,t)$tsol(t)
 = 100*fdebtxx(insd,simcur,t)*EXRX(simcur,t) / GDPMPX(simcur,t);

solvency('nation','fdebt-GDP-rat',simcur,t)$tsol(t)
 = 100*SUM(insd, fdebtxx(insd,simcur,t)*EXRX(simcur,t)) / GDPMPX(simcur,t);

*=====
*For domestic institutions (government and non-government):
*Ratio between total net debt (foreign and domestic) and current income (in %)

solvency(insdng,'totnetdebt-y-rat',simcur,t)$tsol(t)
 = 100*(fdebtxx(insdng,simcur,t)*EXRX(simcur,t) - gdebtxx(insdng,simcur,t))
  / (simsam(simcur,t,'total',insdng)/scaling('sam'));

solvency(insgov,'totnetdebt-y-rat',simcur,t)$tsol(t)
 = 100*(fdebtxx(insgov,simcur,t)*EXRX(simcur,t) + SUM(insdng, gdebtxx(insdng,simcur,t)))
  / (simsam(simcur,t,'total',insgov)/scaling('sam'));
  
*========
OPTION solvency:3:2:1;
DISPLAY solvency;

*$OFFTEXT

$ONTEXT
gdebteoyxx('total',sim,t)  net gov domestic debt (end-of-year) owed to ac; also for 'hhd'
ndfgxx('total',sim,t)  net gov financing from domestic ins ac (bn LE)
fborxx(total/hhd/gov)  net foreign borrowing by ac

gdebtxx(total/hhd,sim,t)  net gov domestic debt (avg beg & end yr) owed to ac

fdebteoyxx(total/gov/hhd,sim,t)  net foreign debt (end-of-year) owed by ac
fdebtxx(total/gov/hhd,sim,t)  net foreign debt (avg beg & end yr) owed by ac

intpay('total',*,*,sim,t)  net interest pay to inst ac from inst acp of type acpp by sim and time
hhd.gov
hhd.gov
hhd.gov
row.hhd
row.hhd
row.hhd
row.gov
row.gov
row.gov


Formulas
--------
Notation: 
B(t) = borrowing, 
F(t) = financing; 
D(t) = debt at end-of-year t
I(t) = interest payment
i(t) = interest rate

Relationships:
F(t) = B(t) - I(t)
I(t) = i(t)*0.5*[D(t) + D(t-1)]
D(t) = D(t-1) + B(t)


!!!

 ndfgxx('total',sim,t)        = SUM(insdng, ndfgxx(insdng,sim,t));
 gborxx('total',simcur,t)     = SUM(insdng, gborxx(insdng,simcur,t));
 gdebteoyxx('total',simcur,t) = SUM(insdng, gdebteoyxx(insdng,simcur,t));
 gdebtxx('total',simcur,t)    = SUM(insdng, gdebtxx(insdng,simcur,t));

 nffxx(insgov,sim,t) = nffgx(sim,t);
 nffxx(insdng,sim,t) = nffinsx(insdng,sim,t);

*fborxx(ac,sim,t)     foreign borrowing by ac
*fdebteoyxx(ac,sim,t) foreign debt (end-of-year) owed by ac
*fdebtxx(ac,sim,t)    foreign debt (avg beg & end yr) owed by ac
 
  intpay('foreign',insrow,insd,simcur,t)$fdebtxx(insd,simcur,t)
 
$OFFTEXT

 SET
  acfinind
  /
  financing
  financing-prv
  financing-ms
  borrowing
  intpay
  fdebtboy  
  gdebtboy  
  fdebteoy
  gdebteoy
  fdebtavg
  gdebtavg
  / 
  ;

PARAMETER
 govforfin(sim,acfinind,t) government foreign financing and debt data by sim - item - year (LCU)
 ;
 govforfin(sim,'financing',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insgov, nffxx(insgov,sim,t));

 govforfin(sim,'borrowing',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insgov, fborxx(insgov,sim,t));

 govforfin(sim,'fdebteoy',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insgov, fdebteoyxx(insgov,sim,t));

 govforfin(sim,'fdebtavg',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insgov, fdebtxx(insgov,sim,t));

 govforfin(sim,'intpay',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM((insrow,insgov), intpay('foreign',insrow,insgov,sim,t));

  govforfin(sim,'fdebtboy',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insgov, fdebteoyxx(insgov,sim,t-1));
  
DISPLAY govforfin;
  
  
PARAMETER
 prvforfin(sim,acfinind,t) government foreign financing and debt data by sim - item - year
 ;
 prvforfin(sim,'financing',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insdng, nffxx(insdng,sim,t));

 prvforfin(sim,'borrowing',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insdng, fborxx(insdng,sim,t));

 prvforfin(sim,'fdebteoy',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insdng, fdebteoyxx(insdng,sim,t));

 prvforfin(sim,'fdebtavg',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM(insdng, fdebtxx(insdng,sim,t));

 prvforfin(sim,'intpay',t)$(simcur(sim) and trep(t)) 
  = EXRX(sim,t)*SUM((insrow,insdng), intpay('foreign',insrow,insdng,sim,t));

 prvforfin(sim,'fdebtboy',t)$(simcur(sim) and trep(t))  
  = EXRX(sim,t)*SUM(insdng, fdebteoyxx(insdng,sim,t-1));
  
DISPLAY prvforfin;

 ndfgxx('total',sim,t)        = SUM(insdng, ndfgxx(insdng,sim,t));
 gborxx('total',simcur,t)     = SUM(insdng, gborxx(insdng,simcur,t));
 gdebteoyxx('total',simcur,t) = SUM(insdng, gdebteoyxx(insdng,simcur,t));
 gdebtxx('total',simcur,t)    = SUM(insdng, gdebtxx(insdng,simcur,t));
  

PARAMETER
 govdomfin(sim,acfinind,t) government domestic financing and debt data by sim - item - year
 ;
 govdomfin(sim,'financing-prv',t)$(simcur(sim) and trep(t)) 
  = ndfgxx('total',sim,t) - ndfgms(sim,t);

 govdomfin(sim,'financing-ms',t)$(simcur(sim) and trep(t)) 
  = ndfgms(sim,t);

 govdomfin(sim,'borrowing',t)$(simcur(sim) and trep(t)) 
  = gborxx('total',sim,t);

 govdomfin(sim,'gdebteoy',t)$(simcur(sim) and trep(t)) 
  = gdebteoyxx('total',sim,t);

 govdomfin(sim,'gdebtavg',t)$(simcur(sim) and trep(t)) 
  = gdebtxx('total',sim,t);
  
 govdomfin(sim,'intpay',t)$(simcur(sim) and trep(t)) 
  = SUM((insdng,insgov), intpay('domestic',insdng,insgov,sim,t));

 govdomfin(sim,'gdebtboy',t)$(simcur(sim) and trep(t))  
  = gdebteoyxx('total',sim,t-1);
  
DISPLAY govdomfin, gdpmpx, gdpindic;	
  
  
PARAMETER
 govforfingdp(sim,acfinind,t) gov foreign financing & debt data by sim - item - year
 prvforfingdp(sim,acfinind,t) gov foreign financing & debt data by sim - item - year
 govdomfingdp(sim,acfinind,t) gov domestic financing & debt data by sim - item - year
 ;

 govforfingdp(sim,acfinind,t)$(trep(t) AND GDPMPX(sim,t))
  = 100*govforfin(sim,acfinind,t)/GDPMPX(sim,t);
  
 prvforfingdp(sim,acfinind,t)$(trep(t) AND GDPMPX(sim,t))
  = 100*prvforfin(sim,acfinind,t)/GDPMPX(sim,t);
 
 govdomfingdp(sim,acfinind,t)$(trep(t) AND GDPMPX(sim,t))
 = 100*govdomfin(sim,acfinind,t)/GDPMPX(sim,t);
 
DISPLAY govforfingdp, prvforfingdp, govdomfingdp
  
DISPLAY "#### END: repdebtbor.inc";

