* bdi2019-data2.inc

DISPLAY "#### START: bdi2019-data2.inc";    


capest=2; 

* start: agg to avoid problems when adding exr premmium

* complete mapaggreg
mapaggreg(ac,ac)$(NOT SUM(acp, mapaggreg(ac,acp))) = YES;

PARAMETER
  samdummy(ac,acp) dummy SAM
;
samdummy(ac,acp) = SAM(ac,acp);
SAM(ac,acp)  = 0;

SAM(acnt,acntp) = SUM((ac,acp)$(mapaggreg(ac,acnt)$mapaggreg(acp,acntp)), samdummy(ac,acp));

* end: agg to avoid problems when adding exr premmium




*fprdgrw(flab,t) = -qfacgrw(flab,t);
*fprdgrw(flab,t) = -qfacgrw(flab,t)/2;

* start: change place for change in international reserves

SAM('cap-fin','cap-ngov') = SAM('cap-row','cap-ngov');
SAM('cap-row','cap-fin') = SAM('cap-row','cap-ngov');
SAM('cap-row','cap-ngov') = 0;

PARAMETER
  balchk(ac)       to check sam balance
;


* check sam consistency
SAM('total',acnt) = SUM(acntp, SAM(acntp,acnt));
SAM(acnt,'total') = SUM(acntp, SAM(acnt,acntp));
balchk(acnt) = SAM('total',acnt) - SAM(acnt,'total');
DISPLAY balchk;

* end: change place for change in international reserves



* start: consider tax-com to refpet as tax-imp
$ONTEXT
SAM('tax-imp','c-refpet') = SAM('tax-imp','c-refpet') + SAM('tax-com','c-refpet');
SAM('gov','tax-imp') = SAM('gov','tax-imp') + SAM('tax-com','c-refpet');
SAM('gov','tax-com') = SAM('gov','tax-com') - SAM('tax-com','c-refpet');
SAM('tax-com','c-refpet') = 0;

* check sam consistency
SAM('total',acnt) = SUM(acntp, SAM(acntp,acnt));
SAM(acnt,'total') = SUM(acntp, SAM(acnt,acntp));
balchk(acnt) = SAM('total',acnt) - SAM(acnt,'total');
DISPLAY balchk;
$OFFTEXT

* end: consider tax-com to refpet as tax-imp






* start: change prv capital payments from admpub to gov capital payments

SAM('f-capgov','a-admpub') = SAM('f-capprv','a-admpub');
SAM('f-capprv','f-capgov') = SAM('f-capprv','a-admpub');
SAM('f-capprv','a-admpub') = 0;


* check sam consistency
SAM('total',acnt) = SUM(acntp, SAM(acntp,acnt));
SAM(acnt,'total') = SUM(acntp, SAM(acnt,acntp));
balchk(acnt) = SAM('total',acnt) - SAM(acnt,'total');
DISPLAY balchk;

* end: change prv capital payments from admpub to gov capital payments




* start: add import quota rent

$ONTEXT

SET 
  demdomac(ac)
;
ALIAS (demdomac,demdomacp);

demdomac(a) = YES;
demdomac(h) = YES;
demdomac('gov') = YES;
demdomac('inv-prv') = YES;
demdomac('inv-gov') = YES;
demdomac('dstk') = YES;

PARAMETER
  shryprqmbar000(c,ac)
;

shryprqmbar000(c,demdomac) = SAM(c,demdomac) / SUM(demdomacp, SAM(c,demdomacp));




PARAMETER
  prqmbar000 /0.25/
;


*SAM('prqmbar','c-refpet') = prqmbar000*SAM('row','c-refpet');
SAM('prqmbar',cmbar) = prqmbar000*SAM('row',cmbar);

*SAM(demdomac,'prqmbar') = shryprqmbar000('c-refpet',demdomac)*SAM('prqmbar','c-refpet');
SAM(demdomac,'prqmbar') = SUM(cmbar, shryprqmbar000(cmbar,demdomac)*SAM('prqmbar',cmbar));

*SAM('c-refpet',demdomac) = SAM('c-refpet',demdomac) + SAM(demdomac,'prqmbar');
SAM(cmbar,demdomac) = SAM(cmbar,demdomac) + shryprqmbar000(cmbar,demdomac)*SAM('prqmbar',cmbar);



* adjust invest
SAM('cap-ngov','prqmbar') = SAM('inv-prv','prqmbar') + SAM('inv-gov','prqmbar') + SAM('dstk','prqmbar');
SAM('inv-prv','cap-ngov') = SAM('inv-prv','cap-ngov') + SAM('inv-prv','prqmbar');
*!! need to adjust if inv-gov also demands refpet
SAM('inv-gov','cap-ngov') = SAM('inv-gov','cap-ngov') + SAM('inv-gov','prqmbar');
SAM('dstk','cap-ngov') = SAM('dstk','cap-ngov') + SAM('dstk','prqmbar');

SAM('inv-prv','prqmbar') = 0;
SAM('inv-gov','prqmbar') = 0;
SAM('dstk','prqmbar') = 0;



* check sam consistency
SAM('total',acnt) = SUM(acntp, SAM(acntp,acnt));
SAM(acnt,'total') = SUM(acntp, SAM(acnt,acntp));
balchk(acnt) = SAM('total',acnt) - SAM(acnt,'total');
DISPLAY balchk;

$OFFTEXT


* end: add import quota rents






* start: add rents from exchage rate premium

SET mapac(a,c)
/
a-agr           .c-agr
a-min           .c-min
a-food          .c-food
a-texwapp       .c-texwapp
a-woodpaper     .c-woodpaper
a-chemplast     .c-chemplast
a-prodminnmet   .c-prodminnmet
a-met           .c-met
                
a-oman          .c-oman
a-elect         .c-elect
a-water         .c-water
a-construc      .c-construc
a-trade         .c-trade
a-transp        .c-transp
a-hotelrest     .c-hotelrest
a-admpub        .c-admpub
a-edu           .c-edu
a-health        .c-health
a-oser          .c-oser

/
;

PARAMETER
  prexr000 /2.5/
  errcapneg(a)

  
;


SAM('prexr',c) = (1-shrom000(c))*(PREXR000-1)*SAM('row',c)
  - (1-shroe000(c))*(PREXR000-1)*SAM(c,'row');


SAM('f-capprv','prexr') = SUM(c, SAM('prexr',c));



$ONTEXT
SAM(a,c)$mapac(a,c) = SAM(a,c) - SAM('prexr',c);

SAM('f-capprv',a) = SAM('f-capprv',a) - SUM(c$mapac(a,c), SAM('prexr',c));

$OFFTEXT

SET acfin(ac)
/
h-rur
h-urb
gov
inv-prv
inv-gov
dstk
/
;

ALIAS (acfin,acfinp);

PARAMETER
  demshr(c,acfin)
;

demshr(c,acfin) = SAM(c,acfin)/SUM(acfinp, SAM(c,acfinp));

SAM(c,acfin) = SAM(c,acfin) + SAM('prexr',c)*demshr(c,acfin);

SAM('inv-prv','cap-ngov') = SAM('inv-prv','cap-ngov') + SUM(c, SAM('prexr',c)*(demshr(c,'inv-prv')));
SAM('dstk','cap-ngov') = SAM('dstk','cap-ngov') + SUM(c, SAM('prexr',c)*(demshr(c,'dstk')));
SAM('inv-gov','cap-gov') = SAM('inv-gov','cap-gov') + SUM(c, SAM('prexr',c)*(demshr(c,'inv-gov')));


SAM('cap-ngov',h) = SAM('cap-ngov',h) + SAM('cap-ngov',h)/SUM(hp, SAM('cap-ngov',hp)) * [ SUM(c, SAM('prexr',c)*(demshr(c,'inv-prv')+demshr(c,'dstk'))) ]; 
SAM('cap-gov','gov') = SAM('cap-gov','gov') + SUM(c, SAM('prexr',c)*demshr(c,'inv-gov')); 

* check sam consistency
SAM('total',acnt) = SUM(acntp, SAM(acntp,acnt));
SAM(acnt,'total') = SUM(acntp, SAM(acnt,acntp));
balchk(acnt) = SAM('total',acnt) - SAM(acnt,'total');
DISPLAY balchk;

SAM(h,'f-capprv') = SAM(h,'f-capprv') + balchk(h);
SAM('gov','f-capprv') = SAM('gov','f-capprv') + balchk('gov');

* check sam consistency
SAM('total',acnt) = SUM(acntp, SAM(acntp,acnt));
SAM(acnt,'total') = SUM(acntp, SAM(acnt,acntp));
balchk(acnt) = SAM('total',acnt) - SAM(acnt,'total');
DISPLAY balchk;




errcapneg(a)$(SAM('f-capprv',a)<0) = 1/0;



$ONTEXT
  YPREXRT(c,t) =E= (1-shrom(c,t))*(PREXR(t)-1)*EXR(t)*PWM(c,t)*QM(c,t)
    - (1-shroe(c,t))*(PREXR(t)-1)*EXR(t)*PWE(c,t)*QE(c,t);
$OFFTEXT

* to check 


 
PARAMETER
  prratexrc2(c)
  prratexr2
;
 
prratexrc2(c)$SAM('prexr',c) = SAM('prexr',c) / [ (1-shrom000(c))*SAM('row',c) - (1-shroe000(c))*SAM(c,'row') ];
DISPLAY prratexrc2;
prratexr2 = SUM(c, SAM('prexr',c)) / SUM(c, (1-shrom000(c))*SAM('row',c) - (1-shroe000(c))*SAM(c,'row'));
DISPLAY prratexr2; 


 
* end: add rents from exchage rate premium









* start: Frisch param

$ONTEXT
example:
cpi usa 2012 / cpi usa 1970 = 5.917371134					
Costa Rica	2012 GDP per capita (current US$) = 9985.36959
Costa Rica	2012 GDP per capita (current US$) in 1970 USD = 1687.467182
$OFFTEXT

$ONTEXT

PARAMETER 
  yhpc2(ac)
  gdppcdol1970  
  yhpcdol1970(h)
  
  cpiusa2019
  /255.657/
  
  cpiusa1970
  /38.8/
  
  gdppcdol2019
  /4405.487109/
;





gdppcdol1970 = gdppcdol2019/(cpiusa2019/cpiusa1970);

* compute gdp per capita for each hhd in SAM after mapaggreg
yhpc2(h) = SUM(ac$mapaggreg(ac,h), SAM(ac,'total')*scaling('samTo1')/(pop0(ac,'2019')*scaling('qlabTo1')));

yhpc2('total') = SUM(h, SUM(ac$mapaggreg(ac,h), SAM(ac,'total')))*scaling('samTo1') 
  / 
  ( SUM(h, SUM(ac$mapaggreg(ac,h), pop0(ac,'2019')*scaling('qlabTo1'))) );

yhpcdol1970(h) =  yhpc2(h)/yhpc2('total') * gdppcdol1970;

frisch(h) = -36*yhpcdol1970(h)**(-0.36);

$OFFTEXT

* end: Frisch param




DISPLAY "#### END: bdi2019-data2.inc";   