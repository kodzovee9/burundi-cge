* repbcie2.gms


$INCLUDE repnpv.gms

* start: macro dev

PARAMETER
  macrealyydev(sim,maccol,t)
;


macrealyydev(sim,maccol,t)$simcur(sim) = macrealyy(sim,maccol,t) - macrealyy('base',maccol,t);

* end: macro dev


* start: employdev

PARAMETER 
  employyy(ac,ac,t,sim)
  employyydev(ac,ac,t,sim)
  
;
employyy(ac,acp,t,sim) = QFX(ac,acp,t,sim)*scaling('qlabsolrep');
employyydev(ac,acp,t,sim) = employyy(ac,acp,t,sim) - employyy(ac,acp,t,'base');


* end: employdev

* start: poverty

PARAMETER
  povnb(sim,ac,t)
  povnbdev(sim,ac,t)
;


povnb(sim,h,t) = fgt0yy(sim,h,t)/100*popX(h,t,sim);
povnb(sim,'nation',t) = fgt0yy(sim,'nation',t)/100*SUM(h, popX(h,t,sim));
povnb(sim,'urban',t) = fgt0yy(sim,'urban',t)/100*SUM(h$hurb(h), popX(h,t,sim));
povnb(sim,'rural',t) = fgt0yy(sim,'rural',t)/100*SUM(h$hrur(h), popX(h,t,sim));


povnbdev(simcur,ac,t) = povnb(simcur,ac,t) - povnb('base',ac,t);

* end: poverty





* start: repsumdev

SET
  sumcol
  /
  chg-abs
  chg-rel    change % or pp
  
  PrvConZ
  PrvFixInvZ
  GDPZ
  EmployZ
  Pov-NationalZ
  Pov-RuralZ
  Pov-UrbanZ
  /
  
;

ALIAS (sumcol,sumcolp);

PARAMETER
  repsumdev(sumcol,sumcol,sim,t)        summary report deviation
  repsumdevtsel(sumcol,sumcol,sim,t)    summary report deviation selected years
  repsumlev(sumcol,sim,t)            summary report level
  repsumdevaccum(sumcol,sumcol,sim)     summary report deviation accumulated
;

repsumdev('chg-abs','GDPZ',simcur,t)          = macrealyydev(simcur,'GDPFC',t);
repsumdev('chg-abs','PrvConZ',simcur,t)       = macrealyydev(simcur,'PrvCon',t);
repsumdev('chg-abs','PrvFixInvZ',simcur,t)    = macrealyydev(simcur,'PrvFixInv',t);
repsumdev('chg-abs','EmployZ',simcur,t)       = employyydev('tot-lab','total',t,simcur);
repsumdev('chg-abs','Pov-NationalZ',simcur,t) = povnbdev(simcur,'nation',t);
repsumdev('chg-abs','Pov-RuralZ',simcur,t)    = povnbdev(simcur,'rural',t);
repsumdev('chg-abs','Pov-UrbanZ',simcur,t)    = povnbdev(simcur,'urban',t);


repsumdev('chg-rel','GDPZ',simcur,t)$tnmin(t)          = macrealxp(simcur,'GDPFC',t);
repsumdev('chg-rel','PrvConZ',simcur,t)$tnmin(t)       = macrealxp(simcur,'PrvCon',t);
repsumdev('chg-rel','PrvFixInvZ',simcur,t)             = macrealxp(simcur,'PrvFixInv',t);
repsumdev('chg-rel','EmployZ',simcur,t)$tnmin(t)       = employxp(simcur,'total',t);
repsumdev('chg-rel','Pov-NationalZ',simcur,t)$tnmin(t) = fgt0yy(simcur,'nation',t) - fgt0yy('base','nation',t);
repsumdev('chg-rel','Pov-RuralZ',simcur,t)$tnmin(t)    = fgt0yy(simcur,'rural',t) - fgt0yy('base','rural',t);
repsumdev('chg-rel','Pov-UrbanZ',simcur,t)$tnmin(t)    = fgt0yy(simcur,'urban',t) - fgt0yy('base','urban',t);


repsumdevtsel(sumcol,sumcolp,sim,t)$trep(t) = repsumdev(sumcol,sumcolp,sim,t);

* end: repsumdev


* start: repsumlev

repsumlev('GDPZ',simcur,t)          = macrealyy(simcur,'GDPFC',t);
repsumlev('PrvConZ',simcur,t)       = macrealyy(simcur,'PrvCon',t);
repsumlev('PrvFixInvZ',simcur,t)    = macrealyy(simcur,'PrvFixInv',t);
repsumlev('EmployZ',simcur,t)       = employyydev('tot-lab','total',t,simcur);
repsumlev('Pov-NationalZ',simcur,t) = povnb(simcur,'nation',t);
repsumlev('Pov-RuralZ',simcur,t)    = povnb(simcur,'rural',t);
repsumlev('Pov-UrbanZ',simcur,t)    = povnb(simcur,'urban',t);


* start: repsumlev



* start: repsumdevaccum

repsumdevaccum('chg-abs',sumcol,simcur) = 
  SUM(t$tsol(t), repsumlev(sumcol,simcur,t)) - SUM(t$tsol(t), repsumlev(sumcol,'base',t));


repsumdevaccum('chg-rel',sumcol,simcur)$SUM(t$tsol(t), repsumlev(sumcol,'base',t)) = 
  100 * ( SUM(t$tsol(t), repsumlev(sumcol,simcur,t)) / SUM(t$tsol(t), repsumlev(sumcol,'base',t)) - 1);



* end: repsumdevaccum

 
* save GDX file
$IF %NonIMv2%==1 EXECUTE_UNLOAD 'repbcie2-%app%.gdx',
$IF NOT %NonIMv2%==1 EXECUTE_UNLOAD 'repbcie22.gdx',
  npv2
  disc
  
  repsumdev
  repsumlev
  repsumdevaccum

;  

