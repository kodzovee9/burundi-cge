* diagnostics-data.inc

DISPLAY "#### START: diagnostics-data.inc";


* start: display error for illegal sam payments ---------------------

* to diagnose errors in data
PARAMETER
  samlegal(ac,acp)         parameter with values (of 1) in cells where entries are legal
  errsamflow(ac,acp)       illegal SAM payment from 2nd index to 1st (not in samlegal)
  samlegalneg(ac,acp)      parameter with values (of 1) in cells where NEGATIVE values are legal
  errnegsamflow(ac,acp)    if =UNDF illegal negative SAM payment from 2nd index to 1st (not in samlegalneg)
;

* start: temp for import quota and exr premium

samlegal('prqmbar',c) = YES;
samlegal(insgov,'prqmbar') = YES;
samlegal(a,'prqmbar') = YES;
samlegal(h,'prqmbar') = YES;
samlegal(capinsdng,'prqmbar') = YES;


samlegal('prexr',c) = YES;
samlegal(fcapng,'prexr') = YES;

* end: temp for import quota and exr premium


* defining parameters where SAM entries are legal

samlegal(c,a)       = 1;
samlegal(f,a)       = 1;
samlegal(fcapng,fcapg) = 1;
samlegal(taxact,a)  = 1;
*samlegal(taxfact,a) = 1;

samlegal(a,c)       = 1;
samlegal(taxcom,c)  = 1;
samlegal(taxvatc,c)  = 1;
samlegal(taxfacact,a) = 1;
samlegal(subcom,c) = 1;
samlegal(tacd,c)$(SUM(a, SAM(a,c)) - (SUM(insrow, SAM(c,insrow)) - SUM(taxexp, SAM(taxexp,c)) - SUM(tace, SAM(tace,c)))  GT 0) = 1;

samlegal(tace,c)$SUM(insrow, SAM(c,insrow)) = 1;
samlegal(tacm,c)$SUM(insrow, SAM(insrow,c)) = 1;
samlegal(taximp,c)$SUM(insrow, SAM(insrow,c)) = 1;
samlegal(taxexp,c)$SUM(insrow, SAM(c,insrow)) = 1;
samlegal(insrow,c) = YES;


samlegal(c,tacd)$SUM(cp, samlegal(tacd,cp)) = 1;
samlegal(c,tace)$SUM(cp, samlegal(tace,cp)) = 1;
samlegal(c,tacm)$SUM(cp, samlegal(tacm,cp)) = 1;

samlegal(ins,f)     = 1;
samlegal(taxfac,f)  = 1;

samlegal(c,ins) = 1;
samlegal(f,insrow)   = 1;
samlegal(ins,insp)  = 1;
samlegal(taxdir,insdng) = 1;
samlegal(capins,ins) = 1;

samlegal(invng,capinsdng) = 1;
samlegal(invg,capinsdng) = 1;
samlegal(invng,caprow) = 1;
samlegal(invg,capgov) = 1;
samlegal(invng,capgov) = 1;
samlegal(dstk,capins) = 1;

samlegal(insgov,actax) = 1;
samlegal(insgov,subcom) = 1;


samlegal(c,inv)   = 1;
samlegal(c,dstk) = 1;

samlegal(c,insrow) = 1;

samlegal(capgov,capinsdng) = 1;
samlegal(capinsd,caprow) = 1;

* foreign reserves
samlegal(caprow,capfin) = 1;
samlegal(capfin,capinsdng) = 1;

samlegal(acnt,'total')$(SUM(acntp, samlegal(acnt,acntp)) AND SUM(acntp, samlegal(acntp,acnt))) = 1;
samlegal('total',acnt)$samlegal(acnt,'total') = 1;



* defining parameters where negative SAM entries are legal

samlegalneg('prexr',c) = YES;


samlegalneg(taxact,a) = 1;
samlegalneg(taxexp,c) = 1;
samlegalneg(taximp,c) = 1;
samlegalneg(taxcom,c) = 1; 
*samlegalneg(taxfact,a) = 1;
samlegalneg(subcom,c) = 1; 

samlegalneg(capins,ins) = 1;

samlegalneg(dstk,capins) = 1;

samlegalneg(insgov,actax) = 1;
samlegalneg(insgov,subcom) = 1;

samlegalneg(c,dstk)      = 1;

samlegalneg(capgov,capinsdng) = 1;
samlegalneg(capinsd,caprow) = 1;

* foreign reserves
samlegalneg(caprow,capfin) = 1;
samlegalneg(capfin,capinsdng) = 1;


* negative totals are permitted if negative entries are permitted.
samlegalneg(ac,'total')$SUM(acnt, samlegalneg(ac,acnt)) = 1;
samlegalneg('total',ac)$SUM(acnt, samlegalneg(acnt,ac)) = 1;

* illegal SAM payments or values
errsamflow(ac,acp)$(SAM(ac,acp) AND (NOT samlegal(ac,acp))) = 1/0;
DISPLAY errsamflow;

SET
  negcell(ac,acp)    the sam payment from acp (column) to ac (row) is negative
;

negcell(ac,acp)$(SAM(ac,acp) LT 0) = YES;

DISPLAY negcell;

errnegsamflow(ac,acp)$( (sam(ac,acp) LT 0) AND (NOT samlegalneg(ac,acp)) ) = 1;

errnegsamflow(ac,acp)$( (SAM(ac,acp) LT 0) AND (NOT samlegalneg(ac,acp)) ) = 1/0;
DISPLAY errnegsamflow;

* end: display error for illegal sam payments -----------------------

* ============
PARAMETER
   errmapaggreg(ac)         if =UNDF error mapping mapaggreg
;

* error mapaggreg
errmapaggreg(ac)$( SUM(acp$mapaggreg(ac,acp), 1) > 1 ) = 1/0;
DISPLAY errmapaggreg;

*!!
* ============
PARAMETER

  errexpprddum(c)             diff bt error expos and produccion
  errexpprd(c)             if =UNDF error expos > produccion
;
errexpprddum(c) = ( SAM(c,'row') - SUM(taxexp, SAM(taxexp,c)) - SUM(tace, SAM(tace,c)) ) - SUM(a, SAM(a,c));
errexpprd(c)$( (SAM(c,'row') - SUM(taxexp, SAM(taxexp,c)) - SUM(tace, SAM(tace,c)) ) > SUM(a, SAM(a,c))) = 1/0;
DISPLAY errexpprd;



* ============
SET
  errmpcapgovmtfp(fcapg)    only one of mpcapgov and mtfp is nonzero for invg
* Note: either both of or none of mpcapgov(f) and SUM(a, mtfp(a,f)) are non-zero
;

LOOP(fcapg$((mpcapgov(fcapg) AND NOT SUM(a, mtfp(a,fcapg))) OR (NOT mpcapgov(fcapg) AND SUM(a, mtfp(a,fcapg)))),
  errmpcapgovmtfp(fcapg)$((mpcapgov(fcapg) AND NOT SUM(a, mtfp(a,fcapg))) OR (NOT mpcapgov(fcapg) AND SUM(a, mtfp(a,fcapg)))) = YES;
  DISPLAY mpcapgov, mtfp, errmpcapgovmtfp;
  ABORT$errmpcapgovmtfp(fcapg)
  "Only one of mpcapgov(inv) and SUM(a, mtfp(a,inv)) is nonzero for inv."
  "Solution: review above displays of errmpcapgovmtfp, mtfp, and      "
  "mpcapgov, and make sure that both parameters are zero  or non-zero "
  "(for mtfp this applies to the sum over all a).                     "
);

* ============
PARAMETER
  errmfcapinv              if =UNDF error maping mfcapinv
;

errmfcapinv(fcap)$(NOT SUM(inv$mfcapinv(fcap,inv), 1)) = 1/0;
DISPLAY errmfcapinv;


* ============
PARAMETER
  errgovinvng1(fcapng)  if =UNDF SAM has no gov invng but has profit income to gov
;

*errgovinvng1(fcapng)$(SUM(insgov, SAM(insgov,fcapng)) AND NOT SUM(invng$mfcapinv(fcapng,invng), SUM(capgov, SAM(invng,capgov)))) = 1/0;
*DISPLAY errgovinvng1; 


* ============
PARAMETER
  errgovinvng2(fcapng)  if =UNDF SAM has gov invng but has no profit income to gov
;

*errgovinvng2(fcapng)$(NOT SUM(insgov, SAM(insgov,fcapng)) AND SUM(invng$mfcapinv(fcapng,invng), SUM(capgov, SAM(invng,capgov)))) = 1/0;
*DISPLAY errgovinvng2; 



* to diagnose missing data ------------------------------------------

PARAMETER
  errinsd                if =UNDF row in insd

  missprodelas(a)        if =UNDF missing prodelas (value added without prodelas)
  missprodelas2(a,f)     if =UNDF missing prodelas2
  misstradelas(c,ac)     if =UNDF missing tradelas (trade without tradelas)
  missleselas(c,h)       if =UNDF missing LES elasticity
  missfrisch(h)          if =UNDF missing Frisch parameter
  missunemp(f,ac)        if =UNDF factor with endog unemp but missing unemp data ac
  missmsubcom(c)        if =UNDF missing msubcom
  errmsubcom(c)          if =UNDF msubcom defined for row 
  
* model does not work for inst with savings and not invest; not enough having financing  
  missinv(insdng)        if =UNDF missing investment for insdng in SAM 
  misspop0(ac,t)         if =UNDF missing population in labor force age

  errqfbase1(f,a)               if =UNDF data in SAM without corresponding data in qfbase
  errqfbase2(f,a)               if =UNDF data in qfbase without corresponding data in SAM
  
  errcapinsdng                  if =UNDF more than one element in set capinsdng
  errmcapins(ins)               if =UNDF error mapping mcapins
  
  missmtaxfa(taxfacact)         if =UNDF mtaxfa is empty but taxfacact is not empty
  
  errmacacrep(ac)           if =UNDF SAM account ac not mapped to acrep
  
  errf3f2f11(f)            if =UNDF factor f in f1 and in (f2 OR f3)
  errf3f2f12(f)            if =UNDF factor f not in f1 and not in f2 and not in f3
  errmf2f1(f2)           if =UNDF factor in f2 but not in mf2f1 and not mf3f2
  errmf3f2(f3)           if =UNDF factor in f3 but not in mf3f2 
  
  errcedcesexog(c)       if =UNDF commodity c in ced and cesexog
  erreta_a(c)            if =UNDF commodity c in ced without negative eta_e
;

errinsd$SUM(ins$(insd(ins) AND insrow(ins)), 1) = 1/0;
DISPLAY errinsd;

missprodelas(a)$(SUM(f, SAM(f,a)) AND NOT prodelas(a)) = 1/0;
DISPLAY missprodelas;

missprodelas2(a,f1)$(SUM(f2$mf2f1(f2,f1), 1) AND NOT prodelas2(a,f1)) = 1/0;
DISPLAY missprodelas2;

misstradelas(c,'sigma_q')$(SAM('row',c) AND SUM(a, SAM(a,c)) AND NOT tradelas(c,'sigma_q')) = 1/0;
misstradelas(c,'sigma_x')$(SAM(c,'row') AND SUM(a, SAM(a,c)) AND NOT tradelas(c,'sigma_x')) = 1/0;
DISPLAY misstradelas;

missleselas(c,h)$(SAM(c,h) AND NOT leselas(c,h)) = 1/0;
DISPLAY missleselas;
missfrisch(h)$(NOT frisch(h)) = 1/0;
DISPLAY missfrisch;

missunemp(f,'UERAT00')$(facclos0(f)=4 AND unemp(f,'UERAT00') = 0) = 1/0;
missunemp(f,'eta_wf')$(facclos0(f)=4 AND unemp(f,'eta_wf') = 0) = 1/0;
DISPLAY missunemp;

missmsubcom(c)$(SUM(subcom, SAM(subcom,c)) NE 0 AND NOT SUM(ac, msubcom(c,ac))) = 1/0;
DISPLAY missmsubcom;

errmsubcom(c)$(SUM(insrow$msubcom(c,insrow), 1)) = 1/0;
DISPLAY errmsubcom;


missinv(insdng)$(SUM(capinsp, SUM(capins$mcapins(capins,insdng), SAM(capinsp,capins))) 
  AND NOT SUM(inv, SUM(capins$mcapins(capins,insdng), SAM(inv,capins)))) = 1/0;
DISPLAY missinv;

*errinvng$(CARD(invng) NE 1) = 1/0;
*DISPLAY errinvng;

*errinvg$(CARD(invg) NE 0 AND CARD(invg) NE 1) = 1/0;           
*DISPLAY errinvg;

errqfbase1(f,a)$(SUM(ap, qfbase(f,ap)) NE 0  AND SAM(f,a) AND NOT qfbase(f,a)) = 1/0;
DISPLAY errqfbase1;

errqfbase2(f,a)$(SUM(ap, qfbase(f,ap)) NE 0  AND NOT SAM(f,a) AND qfbase(f,a)) = 1/0;
DISPLAY errqfbase2;


misspop0(acpop,tsol)$(pop0(acpop,tsol)=0) = 1/0;
DISPLAY misspop0;





missmtaxfa(taxfacact)$(NOT SUM(f$mtaxfa(taxfacact,f), 1)) = 1/0;
DISPLAY missmtaxfa;


errcapinsdng$(CARD(capinsdng)>1) = 1/0;
DISPLAY capinsdng, errcapinsdng;

errmcapins(ins)$(SUM(capins, SAM(capins,ins)) AND NOT SUM(capins$mcapins(capins,ins), 1)) = 1/0;
DISPLAY errmcapins;

* errmacacrep -------------------------------------------------------

* note: by default, acrep contains 8 elements
errmacacrep(acsam)$(CARD(acrep) > 8 AND NOT SUM(acrep$macacrep(acsam,acrep), 1)) = 1/0;
DISPLAY errmacacrep;



errf3f2f11(f)$(f1(f) AND (f2(f) OR f3(f))) = 1/0;
DISPLAY errf3f2f11;


errf3f2f12(f)$(fncap(f) AND (NOT f1(f) AND NOT f2(f) AND NOT f3(f))) = 1/0;
errf3f2f12(fcapng)$(NOT f1(fcapng) AND NOT f2(fcapng) AND NOT f3(fcapng)) = 1/0;
DISPLAY errf3f2f12;

errmf2f1(f2)$(NOT SUM(f1$mf2f1(f2,f1), 1) AND NOT SUM(f3$mf3f2(f3,f2), 1)) = 1/0;
DISPLAY errmf2f1;

errmf3f2(f3)$(NOT SUM(f2$mf3f2(f3,f2), 1)) = 1/0;
DISPLAY errmf3f2;

errcedcesexog(c)$(ced(c) AND cesexog(c)) = 1/0;
DISPLAY errcedcesexog;

erreta_a(c)$(ced(c) AND tradelas(c,'eta_e')>=0) = 1/0;
DISPLAY erreta_a;



*START: Diagnostics of domestic and foreign debt and interest data===
 
PARAMETER
  errgdebt       inconsistent data for post-calculation of net debt owed by gov to ins(dng)
  errfdebt(ins2) inconsistent data for post-calculation of net debt owed by ins(d) to RoW
;
errgdebt$(debt00('ngovz','govz') AND (NOT SUM(tmin, gintrat0(tmin)))) = 1;
 
errgdebt$((NOT debt00('ngovz','govz')) AND SUM(tmin, gintrat0(tmin))) = 1;
 
errfdebt(ins2)$(debt00('rowz',ins2) AND (NOT SUM(tmin, fintrat0(ins2,tmin)))) = 1;
 
errfdebt(ins2)$((NOT debt00('rowz',ins2)) AND SUM(tmin, fintrat0(ins2,tmin))) = 1;

OPTION errgdebt:0, errfdebt:0;
DISPLAY errgdebt, errfdebt;


LOOP((ins2)$(errgdebt OR errfdebt(ins2)),
DISPLAY debt00, gintrat0, fintrat0, errgdebt, errfdebt;
ABORT$(errgdebt OR errfdebt(ins2))
"For foreign and-or domestic government debt either missing interest data"
"(debt data provided) or missing debt data (interest data provided).     "
"Solution: For the debt type concerned, provide missing data to permit   "
"debt calculation or remove superfluous data and do without debt results."
" " 
);


PARAMETER
 datagdebt01  if 1 (0) enough (not enough data) to compute net gov domestic debt and borrowing
 datafdebt01  if 1 (0) enough (not enough data) to compute net for debt and borrowing
 ;
 datagdebt01 = 0;
 datafdebt01 = 0;
 datagdebt01$(debt00('ngovz','govz') AND SUM((tmin), gintrat0(tmin))) = 1;
 datafdebt01$(SUM((ins2), debt00('rowz',ins2)) AND SUM((ins2,tmin), fintrat0(ins2,tmin)))         = 1;
DISPLAY 
"Note: Domestic and foreign debt and borrowing data are computed in the file repdebtbor.inc, included in rep.gms."
 datagdebt01, datafdebt01, fintrat0;


*END: Diagnostics of domestic and foreign debt and interest data=====




DISPLAY "#### END: diagnostics-data.inc";