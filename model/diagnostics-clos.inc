* diagnostics-clos.inc

DISPLAY "#### START: diagnostics-clos.inc";


* start: diagnose rowclossim before imposing defaults

* note: currently, cannot move this code section to diagnostics-clos.inc

PARAMETER
  errrowclossim(sim,t)    rowclossim for some post-base yr but not for t
;

errrowclossim(sim,t)$(tsol(t) AND SUM(tp$tnmin(tp), rowclossim(sim,tp)) AND (NOT rowclossim(sim,t))) = 1/0;
DISPLAY errrowclossim;

* If rowclossim exists for sim-tmin but not for sim-t, use the value for sim-tmin
rowclossim(sim,t)$(SUM(tmin, rowclossim(sim,tmin)) AND (NOT rowclossim(sim,t))) = SUM(tmin, rowclossim(sim,tmin));


* end: diagnose rowclossim before imposing defaults





PARAMETER

  errfacclos(sim,f)          if =UNDF error missing facclossim
  errnumeraire(sim)          if =UNDF error missing numerairesim
  errgovclos(sim,t)            if =UNDF error missing govclossim
  errsiclos(sim)             if =UNDF error missing siclossim
  errrowclos(sim,t)            if =UNDF error missing rowclossim
  
  err2facclos(sim,f)         if =UNDF invalid value for facclossim
  err2numeraire(sim)         if =UNDF invalid value for numerairesim
  err2govclos(sim,t)           if =UNDF invalid value for govclossim
  err2siclos(sim)            if =UNDF invalid value for siclossim
  err2rowclos(sim,t)           if =UNDF invalid value for rowclossim

  errtrfacrow(sim)           if =UNDF ngovpayrule for trfacrow =2 with all relevant trnsfrb=0
  errtrrowfac(sim)           if =UNDF ngovpayrule for trrowfac =2 with all relevant trnsfrb=0

 

  errtrrowgov(sim)           if =UNDF govspndrule for trrowgov =2 with all relevant trnsfrb=0
  
  errgovsirowclos(sim,t)       if =UNDF govclos=4 and (siclossim NE 3 or rowclos NE 3) -- cannot use SAVF for more than one clos and need SAVF to clear sav-inv
  errsirowclos(sim,t)          if =UNDF siclos=4 and rowclos EQ 2 -- cannot use NFFINS for more than one clos
  errgovrowclos(sim,t)          if =UNDF govclos=3 and rowclos EQ 3 -- cannot use NFFG for more than one clos
  errcombirowclos(sim,t)       if =UNDF rowclos=3 and govclos NE 4 and siclos NE 3 
  errsigovclos(sim)          if =UNDF govclos=4 and siclos=3 -- cannot use SAVF for gov and sav-inv
  
  
  
  errgovrecrule(sim,acgovrec)       if =UNDF value not valid for govrecrule
  errgovspndrule(sim,acgovspnd)     if =UNDF value not valid for govspndrule
  errngovpayrule(sim,acngovpay)     if =UNDF value not valid for ngovpayrule

  errgovspndrule2(sim,acgovspnd)     if =UNDF rule=2 without corresponding gov spending in SAM (switch to 1)
  errngovpayrule2(sim,acngovpay)     if =UNDF rule=2 without corresponding non-gov payment in SAM (switch to 1)

  errgovspndrule3(sim,acgovspnd,t)     if =UNDF rule=2 for fcapg and govclos=5 (government investment)
  
  errgovclos8(sim,t)                    if =UNDF govclos=8 and tfp01sim is empty (see par-defn-sim.inc)
  
  errgovclos8ref0                   if =UNDF govclos=8 and dmod=1 for ref0
;

* missing closure rules

errfacclos(sim,f)$(fsam(f) AND fva(f) AND NOT facclossim(sim,f)) = 1/0;
DISPLAY errfacclos;

errnumeraire(sim)$(NOT numerairesim(sim)) = 1/0;
DISPLAY errnumeraire;

errgovclos(sim,t)$(NOT govclossim(sim,t)) = 1/0;
DISPLAY errgovclos;

errsiclos(sim)$(NOT siclossim(sim)) = 1/0;
DISPLAY errsiclos;

errrowclos(sim,t)$(NOT rowclossim(sim,t)) = 1/0;
DISPLAY errrowclos;

* invalid closure rules

err2facclos(sim,f)$(fsam(f) AND fva(f) AND facclossim(sim,f) NE 1 AND facclossim(sim,f) NE 2 AND facclossim(sim,f) NE 3 AND facclossim(sim,f) NE 4) = 1/0;
DISPLAY err2facclos;

err2numeraire(sim)$(numerairesim(sim) NE 1 AND numerairesim(sim) NE 2 AND numerairesim(sim) NE 3) = 1/0;
DISPLAY err2numeraire;

err2govclos(sim,t)$(
  govclossim(sim,t) NE 1 
  AND govclossim(sim,t) NE 2 
  AND govclossim(sim,t) NE 3 
  AND govclossim(sim,t) NE 4 
  AND govclossim(sim,t) NE 5 
  AND govclossim(sim,t) NE 6 
  AND govclossim(sim,t) NE 7 
  AND govclossim(sim,t) NE 8
  AND govclossim(sim,t) NE 9
  AND govclossim(sim,t) NE 10
  AND govclossim(sim,t) NE 11) = 1/0;
DISPLAY err2govclos;

err2siclos(sim)$(siclossim(sim) NE 1 AND siclossim(sim) NE 2 AND siclossim(sim) NE 3 AND siclossim(sim) NE 4) = 1/0;
DISPLAY err2siclos;

err2rowclos(sim,t)$(rowclossim(sim,t) NE 1 AND rowclossim(sim,t) NE 2 AND rowclossim(sim,t) NE 3 AND rowclossim(sim,t) NE 4) = 1/0;
DISPLAY err2rowclos;

errtrfacrow(sim)$(ngovpayrulesim(sim,'trfacrow')=2 AND SUM((f,insrow), trnsfrb00(f,insrow))=0) = 1/0; 
DISPLAY errtrfacrow;
DISPLAY "suggested solution: switch to ngovpayrulesim(sim,'trfacrow') = 1";

errtrrowfac(sim)$(ngovpayrulesim(sim,'trrowfac')=2 AND SUM((insrow,f), trnsfrb00(insrow,f))=0) = 1/0; 
DISPLAY errtrrowfac;
DISPLAY "suggested solution: switch to ngovpayrulesim(sim,'trrowfac') = 1";



errtrrowgov(sim)$(govspndrulesim(sim,'trrowgov')=2 AND SUM((insrow,insgov), trnsfrb00(insrow,insgov))=0) = 1/0; 
DISPLAY errtrrowgov;
DISPLAY "suggested solution: switch to govspndrulesim(sim,'trrowgov') = 1";


errsirowclos(sim,t)$(siclossim(sim) = 4 AND rowclossim(sim,t) = 2) = 1/0;
DISPLAY errsirowclos;

errgovrowclos(sim,t)$(govclossim(sim,t) = 3 AND rowclossim(sim,t) = 3) = 1/0;
DISPLAY errgovrowclos;


$ONTEXT

* errors in combined closure rules
  
errgovsirowclos(sim)$(govclossim(sim) = 4 AND (siclossim(sim) NE 3 OR rowclossim(sim) NE 3)) = 1/0;
DISPLAY errgovsirowclos;


errcombirowclos(sim)$(rowclossim(sim) = 3 AND govclossim(sim) NE 4 AND siclossim(sim) NE 3) = 1/0;
DISPLAY errcombirowclos;

errsigovclos(sim)$(govclossim(sim)=3 AND siclossim(sim)=4) = 1/0;
DISPLAY errsigovclos;  

$OFFTEXT

errgovrecrule(sim,acgovrec)$(govrecrulesim(sim,acgovrec) NE 1 
  AND govrecrulesim(sim,acgovrec) NE 2 
  AND govrecrulesim(sim,acgovrec) NE 3) = 1/0;
DISPLAY errgovrecrule;

errgovspndrule(sim,acgovspnd)$(govspndrulesim(sim,acgovspnd) NE 1 
  AND govspndrulesim(sim,acgovspnd) NE 2 
  AND govspndrulesim(sim,acgovspnd) NE 3) = 1/0;
DISPLAY errgovspndrule;

errngovpayrule(sim,acngovpay)$(NOT trrowngov(acngovpay) AND NOT trrowfac(acngovpay) 
  AND ngovpayrulesim(sim,acngovpay) NE 1
  AND ngovpayrulesim(sim,acngovpay) NE 2 
  AND ngovpayrulesim(sim,acngovpay) NE 3) = 1/0;
DISPLAY errngovpayrule;


errgovspndrule2(sim,trngovgov)$(NOT SUM((insdng,insgov), SAM(insdng,insgov)) AND govspndrulesim(sim,trngovgov)=2) = 1/0;
DISPLAY errgovspndrule2;

errngovpayrule2(sim,fdi)$(NOT SUM((invng,caprow), SAM(invng,caprow)) AND ngovpayrulesim(sim,fdi)=2) = 1/0;
DISPLAY errngovpayrule2; 

errngovpayrule2(sim,tourismrec)$(NOT SUM((c,instrst), SAM(c,instrst)) AND ngovpayrulesim(sim,tourismrec)=2) = 1/0;
DISPLAY errngovpayrule2; 


errgovspndrule3(sim,fcapgz,t)$(govclossim(sim,t) EQ 5 AND govspndrulesim(sim,fcapgz) NE 1) = 1/0;
DISPLAY errgovspndrule3;
DISPLAY "suggested solution: switch to ngovpayrulesim(sim,fcapgz) = 1";


errgovclos8ref0$(govclos0=8 AND dmod NE 1) = 1/0;



DISPLAY "#### END: diagnostics-clos.inc";