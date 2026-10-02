*bdi2019-sim2.inc

DISPLAY "#### START: bdi2019-sim2.inc";    




$ONMULTI
SET
  t1925(t)
  /
  2019*2025
  /
  
  t1926(t)
  /2019*2026/
  
  t2640(t)
  /
  2026*2040
  /

  t1930(t)
  /
  2019*2030
  /
  
  t3140(t)
  /
  2031*2040
  /
  
  t4050(t)
  /
  2040*2050
  /
  
  t19(t)
  /2019/
  
  t25(t)
  /2025/
  
  t26(t)
  /2026/

  t27(t)
  /2027/

  t28(t)
  /2028/

  t40(t)
  /2040/

  t2740(t)
  /2027*2040/

  t2940(t)
  /2029*2040/
  

 sim
  /
  base

  mquota
  exrodev
  50p
  20p
  10p  
  uni2

  uni
  inf-all
  inf
  hd
  min

  hd-LIC90
  hd-LIC99
  hd-LIC80pc
  hd-LIC90pc
  hd-LIC99pc
  inf-db
  inf-fb

  uni+inf-all
  uni+inf
  uni+hd
  uni+min
  
  uni+inf+hd
  combi  

  combi-min
  combi-fg
  combi-ef  
  
  hd+tfphd

  combi-min+
  combi+
  combi+2
* combi-min+
  /

*SIMCUR=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 
 simcur(sim)
  /
  base
*$ONTEXT
  uni
  uni+inf
  uni+inf+hd
  combi
* combi-min+
  combi+
* combi+2
  
*  hd
*  min
*  inf-all
*  inf
*  combi-min

* uni+hd  
* hd+tfphd
* combi-fg
* combi-ef
*$OFFTEXT


*=-=-=-=-=-=-=-=-=
* hd-LIC90
* hd-LIC99
* hd-LIC80pc
* hd-LIC90pc
* hd-LIC99pc
** mquota
** exrodev
*  50p
*  20p
*  10p  
** uni2
** inf-db
** inf-fb
*  uni+min
* uni+inf-all


  /


* start: financing source

  simtdir(sim)
  /
  mquota
  exrodev
  
  50p
  20p
  10p  
  uni2  
  uni
  
  min
  
  inf-all
  inf
  

  hd  
  hd-LIC90
  hd-LIC99

  hd-LIC80pc
  hd-LIC90pc
  hd-LIC99pc

  uni+inf-all
  uni+inf
  uni+hd
  uni+min
  uni+inf+hd
   
  combi
  combi+
  combi+2
  combi-min
  combi-min+
  
  hd+tfphd
  /
 

 
 simgcon(sim)
  /
  combi-ef  
  /

  simtindir(sim)
  /
  /

  simdbor(sim)
  /
  inf-db
  /

  simfbor(sim)
  /
  inf-fb
  /

  simftr(sim)
  /
  combi-fg
  /

  simtcap(sim)
  /
  /

* end: financing source
 


 
 siminfall(sim)
  /
  inf-all
  uni+inf-all
  /
 
  siminftrg(sim)
  /
  inf
  uni+inf
  uni+inf+hd
 
  inf-db
  inf-fb

   
  combi
  combi+
  combi+2
  combi-min
  combi-fg
  combi-ef    
  combi-min+

  /
 
  simhciLIC90(sim)
  /
  hd-LIC90
  /

  simhciLIC99(sim)
  /
  hd-LIC99
  /

  simhciLIC80pc(sim)
  /
  hd-LIC80pc


  

  /

  simhciLIC90pc(sim)
  /
  hd-LIC90pc
  
  hd
  uni+hd
  uni+inf+hd


  combi
  combi+
  combi+2
  combi-min
  combi-fg
  combi-ef    
  combi-min+
 /

  simhciLIC99pc(sim)
  /
  hd-LIC99pc
  /

  simmin200p(sim)
  /
  combi
  combi-min+
  combi+
  /

  simmin100p(sim)
  /
  min
* combi
* combi+
  combi+2
  uni+min
  /

  simmin75p(sim)
  /
* min
* combi-min
* combi
* combi-fg
* combi-ef  
  /
  
  simmin50p(sim)
  /
*  min
* combi-min
*  combi
* combi-fg
* combi-ef  
  /
 

  simuni(sim)
  /
  uni
  uni+inf-all
  uni+inf
  uni+hd
  uni+min
  uni+inf+hd

   
  combi
  combi+
  combi+2
  combi-min
  combi-fg
  combi-ef  
  combi-min+
  /  
;  



$OFFMULTI

*simcur(sim) = NO;
 simcur('base') = YES;


* start: closure and rules

govclossim('base',tmin) = 1;

govclossim(simtdir,tmin) = 1;
govclossim(simgcon,tmin) = 4;
govclossim(simtindir,tmin) = 9;
govclossim(simfbor,tmin) = 3;
govclossim(simdbor,tmin) = 2;
govclossim(simtcap,tmin) = 10;
govclossim(simftr,tmin) = 11;

* select elements in f with endogenous income tax rate
tf01sim(simtcap,'f-capprv',t) = 1;

siclossim('base') = 1;

govrecrulesim('base',acgovrec) = 1;

govspndrulesim('base',acgovspnd) = 1;

ngovpayrulesim('base',acngovpay) = 1;

rowclossim(simcur,tmin) = 1;
rowclossim('base',tmin) = 2;

* start: mquota

PARAMETER
  scalqmbar(t)
  /
  2026    0.2 
  2027    0.4
  2028    0.6
  2029    0.8
  2030    1.0
  2031    1.2
  2032    1.4
  2033    1.6
  2034    1.8
  2035    2.0
  2036    2.2
  2037    2.4
  2038    2.6
  2039    2.8
  2040    3.0
$ONTEXT
  2041    3.2
  2042    3.4
  2043    3.6
  2044    3.8
  2045    4.0
  2046    4.2
  2047    4.4
  2048    4.6
  2049    4.8
  2050    5.0  
$OFFTEXT
  /
;

* note: at some point, the quota might become non-binding
qmbarsim('mquota',cmbar,t)$t1925(t) = 1;
qmbarsim('mquota',cmbar,t)$t2640(t) = 1 + scalqmbar(t);
        

* end: mquotaplus


* start: exrodev

PARAMETER
  scalrexr(t)
  /
  2026    0.2 
  2027    0.4
  2028    0.6
  2029    0.8
  2030    1.0
  2031    1.0
  2032    1.0
  2033    1.0
  2034    1.0
  2035    1.0
  2036    1.0
  2037    1.0
  2038    1.0
  2039    1.0
  2040    1.0
$ONTEXT  
  2041    1.0
  2042    1.0
  2043    1.0
  2044    1.0
  2045    1.0
  2046    1.0
  2047    1.0
  2048    1.0
  2049    1.0
  2050    1.0  
$OFFTEXT  
  /
;


rexrsim('exrodev',t)$t1925(t) = 1;
rexrsim('exrodev',t)$t2640(t) = 1 + 0.10*scalrexr(t);

* end: exrodev

* start: exrun



* 50% in exchange rate premium
prexrsim('50p',t)$(tsol(t) AND t1925(t)) = 1;
prexrsim('50p',t)$(tsol(t) AND t26(t))   = (1 + (PREXR0(t)-1)*0.75) / PREXR0(t);
prexrsim('50p',t)$(tsol(t) AND t2740(t)) = (1 + (PREXR0(t)-1)*0.5) / PREXR0(t);

* 80% reduction in exchange rate premium
prexrsim('20p',t)$(tsol(t) AND t1925(t)) = 1;
prexrsim('20p',t)$(tsol(t) AND t26(t)) = (1 + (PREXR0(t)-1)*0.6) / PREXR0(t);
prexrsim('20p',t)$(tsol(t) AND t2740(t)) = (1 + (PREXR0(t)-1)*0.2) / PREXR0(t);

* 90% reduction in exchange rate premium
prexrsim('10p',t)$(tsol(t) AND t1925(t)) = 1;
prexrsim('10p',t)$(tsol(t) AND t26(t)) = (1 + (PREXR0(t)-1)*0.55) / PREXR0(t);
prexrsim('10p',t)$(tsol(t) AND t2740(t)) = (1 + (PREXR0(t)-1)*0.1) / PREXR0(t);

* 100% reduction in exchange rate premium
prexrsim('uni2',t)$(tsol(t) AND t1925(t)) = 1;
prexrsim('uni2',t)$(tsol(t) AND t26(t)) = (1 + (PREXR0(t)-1)*0.5) / PREXR0(t);
prexrsim('uni2',t)$(tsol(t) AND t2740(t)) = (1 + (PREXR0(t)-1)*0) / PREXR0(t);

prexrsim(simuni,t)$(tsol(t) AND t1925(t)) = 1;
prexrsim(simuni,t)$(tsol(t) AND t26(t)) = (1 + (PREXR0(t)-1)*0.5) / PREXR0(t);
prexrsim(simuni,t)$(tsol(t) AND t2740(t)) = (1 + (PREXR0(t)-1)*0) / PREXR0(t);

qmbarsim(simuni,c,t)$(cmbar(c) AND tsol(t) AND t1925(t)) = 1;
qmbarsim(simuni,c,t)$(cmbar(c) AND tsol(t) AND t26(t)) = 45;
qmbarsim(simuni,c,t)$(cmbar(c) AND tsol(t) AND t2740(t)) = 45;

* end: exrun


* start: min

PARAMETER
  scalmin(t)
  /
$ONTEXT    
  2026	0.05
  2027	0.1
  2028	0.15
  2029	0.2
  2030	0.25
  2031	0.3
  2032	0.35
  2033	0.4
  2034	0.45
  2035	0.5
  2036	0.55
  2037	0.6
  2038	0.65
  2039	0.7
  2040	0.75
  2041	0.8
  2042	0.85
  2043	0.9
  2044	0.95
  2045	1
  2046	1
  2047	1
  2048	1
  2049	1
  2050	1
$OFFTEXT
  2026 0.1
  2027 0.2
  2028 0.3
  2029 0.4
  2030 0.5
  2031 0.6
  2032 0.7
  2033 0.8
  2034 0.9
  2035 1
  2036 1
  2037 1
  2038 1
  2039 1
  2040 1
  /
;

 qfssim(simmin200p,'f-nrmin',t)$t1925(t) = 1;
 qfssim(simmin200p,'f-nrmin',t)$t2640(t) = 1 + 2.0*scalmin(t);

 qfssim(simmin100p,'f-nrmin',t)$t1925(t) = 1;
 qfssim(simmin100p,'f-nrmin',t)$t2640(t) = 1 + 1.0*scalmin(t);

 qfssim(simmin50p,'f-nrmin',t)$t1925(t) = 1;
 qfssim(simmin50p,'f-nrmin',t)$t2640(t) = 1 + 0.5*scalmin(t);

 qfssim(simmin75p,'f-nrmin',t)$t1925(t) = 1;
 qfssim(simmin75p,'f-nrmin',t)$t2640(t) = 1 + 0.75*scalmin(t);


* end: min


* start: inf scenarios


PARAMETER
  scalfacinf(t)
  /
$ONTEXT  
  2026    0.2 
  2027    0.4
  2028    0.6
  2029    0.8
  2030    1.0
  2031    1.0
  2032    1.0
  2033    1.0
  2034    1.0
  2035    1.0
  2036    1.0
  2037    1.0
  2038    1.0
  2039    1.0
  2040    1.0
  2041    1.0
  2042    1.0
  2043    1.0
  2044    1.0
  2045    1.0
  2046    0.8
  2047    0.6
  2048    0.4
  2049    0.2
  2050    0.0
$OFFTEXT  
  2026    0.2 
  2027    0.4
  2028    0.6
  2029    0.8
  2030    1.0
  2031    1.0
  2032    1.0
  2033    1.0
  2034    1.0
  2035    1.0
  2036    0.8
  2037    0.6
  2038    0.4
  2039    0.2
  2040    0.0
  /
  
;  

* change in real investment
 ddkinssim(siminfall,'govz','f-capgov',t)$tsol(t) = GDPMP0(t)*scaling('samsol')*0.02*scalfacinf(t)/PK0('f-capgov',t);
*ddkinssim(siminfall,'govz','f-capgov',t)$tsol(t) = GDPMP0(t)*scaling('samsol')*0.01*scalfacinf(t)/PK0('f-capgov',t);
*ddkinssim(siminfall,'govz','f-capgov',t)$tsol(t) = GDPMP0(t)*scaling('samsol')*0.005*scalfacinf(t)/PK0('f-capgov',t);

* depreciation
ddkinssim(siminfall,'govz','f-capgov',t)$t4050(t) = SUM(tp$(ORD(tp)<ORD(t)), ddkinssim(siminfall,'govz','f-capgov',tp))*deprcap('f-capgov');


* start: MP capital
*mpcapgovsim(siminfall,'f-capgov') = 0.45;
*mpcapgovsim(siminfall,'f-capgov') = 0.30;
 mpcapgovsim(siminfall,'f-capgov') = 0.65;

* targeted activities
mtfpsim(siminfall,a,'f-capgov') = 1;

* end: MP capital



* change in real investment
ddkinssim(siminftrg,'govz','f-capgov',t)$tsol(t) = GDPMP0(t)*scaling('samsol')*0.02*scalfacinf(t)/PK0('f-capgov',t);

* depreciation
ddkinssim(siminftrg,'govz','f-capgov',t)$t4050(t) = SUM(tp$(ORD(tp)<ORD(t)), ddkinssim(siminftrg,'govz','f-capgov',tp))*deprcap('f-capgov');


* start: MP capital
*mpcapgovsim(siminftrg,'f-capgov') = 0.45;
 mpcapgovsim(siminftrg,'f-capgov') = 0.65;

* targeted activities
SET atrg(a)
/
a-agr
a-food
a-texwapp
a-woodpaper
a-chemplast
a-prodminnmet
a-met
a-oman
a-elect
a-water
a-construc
a-transp

/
;
mtfpsim(siminftrg,atrg,'f-capgov') = 1;





* end: inf scenarios




* start: HD spending and labor productivity shocks


$ONTEXT

* HCI low income countries

. summ hci if lic01==1 & time==2020, detail

            Human Capital Index (HCI) (scale 0-1)
                        [HD.HCI.OVRL]
-------------------------------------------------------------
      Percentiles      Smallest
 1%     .2916319       .2916319
 5%     .2957073       .2997828
10%     .3031518       .3065208       Obs                  20
25%     .3186419       .3156852       Sum of wgt.          20

50%     .3751148                      Mean           .3642434
                        Largest       Std. dev.      .0418991
75%     .3890808       .4002841
90%     .4174056       .4132406       Variance       .0017555
95%     .4267193       .4215706       Skewness      -.3167104
99%      .431868        .431868       Kurtosis       1.991021


Burundi hci 2020 =  .3861706    

* source: WDI

$OFFTEXT


PARAMETER
  hciprct( * ) HCI percentile data
  /
  bdi        .3861706  
  lic80th    .3961376
  lic90th    .4174056  
  lic99th    .431868  
  /
;

PARAMETER
  scalfachci(t) scaling factor for HCI-driven change in labor productivity
  /
$ONTEXT  
2026	0.05
2027	0.1
2028	0.15
2029	0.2
2030	0.25
2031	0.3
2032	0.35
2033	0.4
2034	0.45
2035	0.5
2036	0.55
2037	0.6
2038	0.65
2039	0.7
2040	0.75
2041	0.8
2042	0.85
2043	0.9
2044	0.95
2045	1
2046	1
2047	1
2048	1
2049	1
2050	1
$OFFTEXT
2031	0.1
2032	0.2
2033	0.3
2034	0.4
2035	0.5
2036	0.6
2037	0.7
2038	0.8
2039	0.9
2040	1
 / 

  scalfacgspnd(t)
  /
  2026  0.1
  2027  0.2
  2028  0.3
  2029  0.4
  2030  0.5
  2031  0.6
  2032  0.7
  2033  0.8
  2034  0.9
  2035  1
  2037  1
  2038  1
  2039  1
  2040  1
  / 
 
 ;

*increase labor productivity to close the gap wrt 90th percentile HCI for LICs 
 fprdabsim(simhciLIC80pc,flab,a,t)$(t1930(t) AND tsol(t)) = 1;
 fprdabsim(simhciLIC80pc,flab,a,t)$(t3140(t) AND tsol(t)) = (1 + ((hciprct('lic80th')/hciprct('bdi') - 1)/1)*scalfachci(t));

 fprdabsim(simhciLIC90,flab,a,t)$(t1930(t) AND tsol(t)) = 1;
 fprdabsim(simhciLIC90,flab,a,t)$(t3140(t) AND tsol(t)) = (1 + ((hciprct('lic90th')/hciprct('bdi') - 1)/1)*scalfachci(t));

 fprdabsim(simhciLIC90pc,flab,a,t)$(t1930(t) AND tsol(t)) = 1;
 fprdabsim(simhciLIC90pc,flab,a,t)$(t3140(t) AND tsol(t)) = (1 + ((hciprct('lic90th')/hciprct('bdi') - 1)/1)*scalfachci(t));

*increase labor productivity to close the gap wrt 99th percentile HCI for LICs 
 fprdabsim(simhciLIC99,flab,a,t)$(t1930(t) AND tsol(t)) = 1;
 fprdabsim(simhciLIC99,flab,a,t)$(t3140(t) AND tsol(t)) = (1 + ((hciprct('lic99th')/hciprct('bdi') - 1)/1)*scalfachci(t));

 fprdabsim(simhciLIC99pc,flab,a,t)$(t1930(t) AND tsol(t)) = 1;
 fprdabsim(simhciLIC99pc,flab,a,t)$(t3140(t) AND tsol(t)) = (1 + ((hciprct('lic99th')/hciprct('bdi') - 1)/1)*scalfachci(t));



*!! to do: also health
PARAMETERS
  gapgovspndedu50lmic    gap spending on edu bt Burundi & median for LMIC
  gapgovspndedu90lic    gap spending on edu bt Burundi & 90th percentile for LIC
  gapgovspndedu99lic    gap spending on edu bt Burundi & 99th percentile for LIC

  gapgovspndhealth50lmic    gap spending on edu bt Burundi & median for LMIC
  gapgovspndhealth90lic    gap spending on health bt Burundi & 90th percentile for LIC
  gapgovspndhealth99lic    gap spending on health bt Burundi & 99th percentile for LIC
;

* gov spending gap as % of GDP] 
*   = [educ GDP shares for Xth %-ile] MINUS [educ GDP shares for Burundi]

gapgovspndedu50lmic = (3.597 - 5.347)/100;
gapgovspndedu90lic = (6.286 - 5.347)/100;
gapgovspndedu99lic = (7.44701 - 5.347)/100;

gapgovspndhealth50lmic = (1.904403 - 2.275172)/100;  
gapgovspndhealth90lic = (2.258316 - 2.275172)/100;     
gapgovspndhealth99lic = (2.516317 - 2.275172)/100;    

PARAMETER
 gapgovspnd(*,*)
 ;
 gapgovspnd('edu','50lmic') = gapgovspndedu50lmic;
 gapgovspnd('edu','90lic')  = gapgovspndedu90lic;
 gapgovspnd('edu','99lic')  = gapgovspndedu99lic; 

 gapgovspnd('health','50lmic') = gapgovspndhealth50lmic;
 gapgovspnd('health','90lic')  = gapgovspndhealth90lic;
 gapgovspnd('health','99lic')  = gapgovspndhealth99lic; 
;

DISPLAY gapgovspnd;

*Education
 dqgsim(simhciLIC90,'c-edu',t)$(t2640(t) AND tsol(t) AND gapgovspndedu90lic>0) 
  = gapgovspndedu90lic*scalfacgspnd(t)*GDPMP0(t)*scaling('samsol')/PQD0('c-edu','gov',t);

 dqgsim(simhciLIC99,'c-edu',t)$(t2640(t) AND tsol(t) AND gapgovspndedu99lic>0) 
  = gapgovspndedu99lic*scalfacgspnd(t)*GDPMP0(t)*scaling('samsol')/PQD0('c-edu','gov',t);

*Health
 dqgsim(simhciLIC90,'c-health',t)$(t2640(t) AND tsol(t) AND gapgovspndhealth90lic>0) 
  = gapgovspndhealth90lic*scalfacgspnd(t)*GDPMP0(t)*scaling('samsol')/PQD0('c-health','gov',t);

 dqgsim(simhciLIC99,'c-health',t)$(t2640(t) AND tsol(t) AND gapgovspndhealth99lic>0) 
  = gapgovspndhealth99lic*scalfacgspnd(t)*GDPMP0(t)*scaling('samsol')/PQD0('c-health','gov',t);

DISPLAY dqgsim;



* start: define increase in edu and health spending based on spending pc


$ONTEXT
* education per capita in LICs 

                         gedupc_ppp
-------------------------------------------------------------
      Percentiles      Smallest
 1%     14.68719       14.68719
 5%     14.68719       16.92556
10%     16.92556       18.64719       Obs                  18
25%     33.07692       28.36188       Sum of wgt.          18

50%      53.3336                      Mean           52.15398
                        Largest       Std. dev.      22.71719
75%     69.75416       74.36494
90%     82.37889       77.95513       Variance       516.0705
95%     88.58466       82.37889       Skewness      -.2191483
99%     88.58466       88.58466       Kurtosis       2.022022


Burundi Edu Spending per capita = 50.80674 


* health in LICs (% GDP)

. summ ghealthpc_ppp if lic01==1 & time==2015, detail

                        ghealthpc_ppp
-------------------------------------------------------------
      Percentiles      Smallest
 1%     1.228099       1.228099
 5%     2.366506       3.504914
10%     4.437657         5.3704       Obs                  20
25%     12.48435       9.303607       Sum of wgt.          20

50%     18.33478                      Mean           19.10716
                        Largest       Std. dev.      10.15585
75%     25.63957       29.78422
90%     33.86548       32.94665       Variance       103.1413
95%     36.13198        34.7843       Skewness       .0477346
99%     37.47966       37.47966       Kurtosis       2.286415

Burundi Health Spending per capita = 19.29408 

$OFFTEXT


PARAMETER
  gedupcprct( * ) edu spending per capita percentile data
  /
  bdi         50.80674  
  lic50th     53.3336 
  lic80th     74.36494
  lic90th     82.37889  
  lic99th     88.58466
  /


  ghealthpcprct( * ) health spending per capita percentile data
  /
  bdi        18.33478 
  lic50th    23.79656
  lic80th    28.21539
  lic90th    33.86548   
  lic99th    37.47966 
  /


  scalfacgspndpc(t)
  /
  2026  0.1
  2027  0.2
  2028  0.3
  2029  0.4
  2030  0.5
  2031  0.6
  2032  0.7
  2033  0.8
  2034  0.9
  2035  1
  2036  1  
  2037  1
  2038  1
  2039  1
  2040  1
  /
;



*Education
 qgbsim(simhciLIC80pc,'c-edu',t)$(t1925(t) AND tsol(t)) = 1;
 qgbsim(simhciLIC80pc,'c-edu',t)$(t2640(t) AND tsol(t)) = 1 + (gedupcprct('lic80th')/gedupcprct('bdi') - 1) * scalfacgspndpc(t);

 qgbsim(simhciLIC90pc,'c-edu',t)$(t1925(t) AND tsol(t)) = 1;
 qgbsim(simhciLIC90pc,'c-edu',t)$(t2640(t) AND tsol(t)) = 1 + (gedupcprct('lic90th')/gedupcprct('bdi') - 1) * scalfacgspndpc(t);

 qgbsim(simhciLIC99pc,'c-edu',t)$(t1925(t) AND tsol(t)) = 1;
 qgbsim(simhciLIC99pc,'c-edu',t)$(t2640(t) AND tsol(t)) = 1 + (gedupcprct('lic99th')/gedupcprct('bdi') - 1) * scalfacgspndpc(t);

*Health
 qgbsim(simhciLIC80pc,'c-health',t)$(t1925(t) AND tsol(t)) = 1;
 qgbsim(simhciLIC80pc,'c-health',t)$(t2640(t) AND tsol(t)) = 1 + (ghealthpcprct('lic80th')/ghealthpcprct('bdi') - 1) * scalfacgspndpc(t);

 qgbsim(simhciLIC90pc,'c-health',t)$(t1925(t) AND tsol(t)) = 1;
 qgbsim(simhciLIC90pc,'c-health',t)$(t2640(t) AND tsol(t)) = 1 + (ghealthpcprct('lic90th')/ghealthpcprct('bdi') - 1) * scalfacgspndpc(t);

 qgbsim(simhciLIC99pc,'c-health',t)$(t1925(t) AND tsol(t)) = 1;
 qgbsim(simhciLIC99pc,'c-health',t)$(t2640(t) AND tsol(t)) = 1 + (ghealthpcprct('lic99th')/ghealthpcprct('bdi') - 1) * scalfacgspndpc(t);


* end: define increase in edu and health spending based on spending pc

* end: HD spending and labor productivity shocks



* start: hd + TFP in HD services

*increase labor productivity to close the gap wrt 90th percentile HCI for LICs 
 fprdabsim('hd+tfphd',flab,a,t)$(t1930(t) AND tsol(t)) = 1;
 fprdabsim('hd+tfphd',flab,a,t)$(t3140(t) AND tsol(t)) = (1 + ((hciprct('lic80th')/hciprct('bdi') - 1)/1)*scalfachci(t));

*Education
 qgbsim('hd+tfphd','c-edu',t)$(t1925(t) AND tsol(t)) = 1;
 qgbsim('hd+tfphd','c-edu',t)$(t2640(t) AND tsol(t)) = 1 + (gedupcprct('lic80th')/gedupcprct('bdi') - 1)*0.5 * scalfacgspndpc(t);

 tfpexogsim('hd+tfphd','a-edu',t)$(t1925(t) AND tsol(t)) = 1;
 tfpexogsim('hd+tfphd','a-edu',t)$(t2640(t) AND tsol(t)) = 1 + (gedupcprct('lic80th')/gedupcprct('bdi') - 1)*0.5 * scalfacgspndpc(t);


*Health
 qgbsim('hd+tfphd','c-health',t)$(t1925(t) AND tsol(t)) = 1;
 qgbsim('hd+tfphd','c-health',t)$(t2640(t) AND tsol(t)) = 1 + (ghealthpcprct('lic80th')/ghealthpcprct('bdi') - 1)*0.5 * scalfacgspndpc(t);

 tfpexogsim('hd+tfphd','a-health',t)$(t1925(t) AND tsol(t)) = 1;
 tfpexogsim('hd+tfphd','a-health',t)$(t2640(t) AND tsol(t)) = 1 + (ghealthpcprct('lic80th')/ghealthpcprct('bdi') - 1)*0.5 * scalfacgspndpc(t);


* end: hd + TFP in HD services


* start: additional tfp shock for combi+ and combi+2

PARAMETER
  scaltfp(t)
  /
$ONTEXT  
  2026 0.1
  2027 0.2
  2028 0.3
  2029 0.4
  2030 0.5
  2031 0.6
  2032 0.7
  2033 0.8
  2034 0.9
  2035 1
  2036 1
  2037 1
  2038 1
  2039 1
  2040 1
$OFFTEXT

*$ONTEXT
  2026	0.066666667
  2027	0.133333333
  2028	0.2
  2029	0.266666667
  2030	0.333333333
  2031	0.4
  2032	0.466666667
  2033	0.533333333
  2034	0.6
  2035	0.666666667
  2036	0.733333333
  2037	0.8
  2038	0.866666667
  2039	0.933333333
  2040	1
*$OFFTEXT

$ONTEXT
  2026 0.5
  2027 1.0
  2028 1.0
  2029 1.0
  2030 1.0
  2031 1.0
  2032 1.0
  2033 1.0
  2034 1.0
  2035 1
  2036 1
  2037 1
  2038 1
  2039 1
  2040 1
$OFFTEXT


  /
;



tfpexogsim('combi+',a,t)$(t1925(t) AND tsol(t)) = 1;
tfpexogsim('combi+',a,t)$(t2640(t) AND tsol(t)) = 1 + 0.175*scaltfp(t);
*HL-20260322
*tfpexogsim('combi+',a,t)$(t2640(t) AND tsol(t)) = 1 + 0.15*scaltfp(t);

*dnffinssim('combi',t)$(t1925(t) AND tsol(t)) = 0;
*dnffinssim('combi',t)$(t2640(t) AND tsol(t)) = -GDPMP0(t)*scaling('samsol')*0.01*scalmin(t)/EXR0(t);

*nffinsbarsim('combi',t)$(t1925(t) AND tsol(t)) = 1;
*nffinsbarsim('combi',t)$(t2640(t) AND tsol(t)) = 1e-15 + 1-scalmin(t);



PARAMETER

  gdpgrwcombi(t)
  /
2020	0.00335
2021	0.03119
2022	0.01827
2023	0.02659
2024	0.03531
2025	0.01866
2026	0.029815497
2027	0.037109827
2028	0.048061792
2029	0.046651448
2030	0.047756555
2031	0.051496882
2032	0.049699763
2033	0.048184076
2034	0.047030351
2035	0.046023307
2036	0.042058926
2037	0.039491819
2038	0.037035356
2039	0.034752705
2040	0.032560728

  /
;  
*gdpgrwsim('combi+2',t)$(t1925(t) AND tsol(t)) = gdpgrw(t);
*gdpgrwsim('combi+2',t)$(t2640(t) AND tsol(t)) = 0.05;


SET 
t1929(t) /2019*2029/
t3040(t) /2030*2040/
;
gdpgrwsim('combi+2',t)$(t1929(t) AND tsol(t)) = gdpgrwcombi(t);
gdpgrwsim('combi+2','2026') = 0.045;
gdpgrwsim('combi+2','2027') = 0.045;
gdpgrwsim('combi+2','2028') = 0.05;
gdpgrwsim('combi+2','2029') = 0.05;
gdpgrwsim('combi+2',t)$(t3040(t) AND tsol(t)) = 0.055;


*simcur(sim) = NO;
*simcur('base') = YES;
*simcur('combi') = YES;
*simcur('combi+') = YES;
*simcur('combi+2') = YES;



* end: additional tfp shock for combi+ and combi+2


 tineqrep(t)$(t19(t) OR t25(t) OR tmaxrep(t)) = YES;	
DISPLAY tineqrep;

 simpovrep(sim)$simcur(sim) = YES;
 simineqrep(sim)$simcur(sim) = YES;
 tpovrep(t)$tsol(t) = YES;

DISPLAY "#### END: bdi2019-sim2.inc";   


