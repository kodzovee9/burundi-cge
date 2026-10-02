* repbcie.gms


SET
  t 
  /2016*2030, avg2530/
  
  trep(t) 
*  /2025, 2026, 2030/
  /2025, 2030/

  tminrep(t)

  tmax(t)

  merged_set
  sim
  simcur(sim)

  kgdp
  sectorcol
  sectorcol3
  demcol
  maccol
  bopcol

  
  ac
  acrep
  povcol(ac)
  
  cntry
  /
  Guatemala
  El_Salvador
  Honduras
  Nicaragua
  Costa_Rica	
  Panama
  Republica_Dominicana	
  Mexico
  /


;

ALIAS (ac,acp), (t,tp);

$ONMULTI

* the following elements are part of all applications
$GDXIN sim.gdx

$LOADDC tminrep
$LOADDC tmax
$LOADDC ac
$LOADDC acrep
$LOADDC maccol
$LOADDC kgdp
$LOADDC sectorcol
$LOADDC sectorcol3
$LOADDC demcol
$LOADDC bopcol
$LOADDC sim
$LOADDC simcur


$GDXIN reppov.gdx
$LOADDC ac=povcol
$LOADDC povcol

SET sim /2024/;
$OFFMULTI




PARAMETER
* base-year
  sectorstruc200tmp(merged_set,acrep,sectorcol)
  sectorstruc200(cntry,acrep,sectorcol)
 
  facdemstruc200tmp(merged_set,acrep,acrep)                 
  facdemstruc200(cntry,acrep,ac) 
  
  demstruc200tmp(merged_set,acrep,demcol)         
  demstruc200(cntry,acrep,demcol)        
  
  demstruc200tmp(merged_set,acrep,demcol)         
  demstruc200(cntry,acrep,demcol)        

  bopindic00tmp(merged_set,bopcol,kgdp)
  bopindic00(bopcol,cntry)

* macro  
  macrealyy(merged_set,sim,maccol,t)
  macgrowth(merged_set,maccol,sim)
  macrealxp(merged_set,sim,maccol,t)
  
* meso
  qvagrowth2(merged_set,acrep,sim)  
  qvarealxp2(merged_set,sim,acrep,t) 
  qhpcgrowth(merged_set,*,sim)
  qhpcrealyy(merged_set,sim,*,t)

  employxp2tmp(merged_set,sim,acrep,t)
  employtotxp(cntry,sim,t)                  empleo total (cambio % respecto base)
  unempratetottmp(merged_set,t,sim)
  unempratetot(cntry,t,sim)
  
* poverty  
  fgt0tmp(merged_set,*,sim)  
  fgt0(cntry,ac,sim)  
  fgt0yytmp(merged_set,sim,*,t)  
  poptotXtmp(merged_set,t,sim)
  
* report
  sectorstruc2tmp(merged_set,acrep,sectorcol,t,sim)       
  sectorindic2tmp(merged_set,acrep,sectorcol3,kgdp,t,sim)

  qhpcX(merged_set,*,t,sim)  
  
;

$GDXIN merged-repbaseyr2

$LOADDC merged_set = Merged_set_1

$LOADDC bopindic00tmp=bopindic00
$LOADDC sectorstruc200tmp=sectorstruc200
$LOADDC facdemstruc200tmp=facdemstruc200
$LOADDC demstruc200tmp=demstruc200



$GDXIN merged-repmacro2

$ONMULTI
$LOADDC merged_set = Merged_set_1
$OFFMULTI
$LOADDC macrealyy
$LOADDC macgrowth
$LOADDC macrealxp


$GDXIN merged-repmeso2

$ONMULTI
$LOADDC merged_set = Merged_set_1
$OFFMULTI

$LOADDC employxp2tmp=employxp2
$LOADDC unempratetottmp=unempratetot

*$LOADDC qvagrowth2
$LOADDC qvarealxp2
$LOADDC qhpcgrowth
$LOADDC qhpcrealyy

$GDXIN merged-reppov2

$ONMULTI
$LOADDC merged_set = Merged_set_1
$OFFMULTI

$LOADDC fgt0tmp=fgt0
$LOADDC fgt0yytmp=fgt0yy
$LOADDC poptotXtmp=poptotX

$GDXIN merged-report

$ONMULTI
$LOADDC merged_set = Merged_set_1
$OFFMULTI

$LOADDC sectorstruc2tmp=sectorstruc2
$LOADDC sectorindic2tmp=sectorindic2



$LOADDC qhpcX




* start: estructura sectorial

SET 
  c3 /c3-agr, c3-ind, c3-ser/
  
  mapacrepc3(c3,acrep)
  /
  c3-agr   . (c2-agr)
  c3-ind   . (c2-min, c2-food, c2-tex, c2-othman, c2-util, c2-cns)
  c3-ser   . (c2-trd, c2-trns, c2-hotelrest, c2-otrser, c2-bus, c2-admpub)
  /

  mapcntrytmin(merged_set,t)
  /
report-cri2018v2.2018
report-dom2016v2.2016
report-gtm2018v2.2018
report-hnd2018v2.2018
report-nic2018v2.2018
report-pan2016v2.2016
report-slv2017v2.2017
report-mex2018bcie.2018

  /
  
  mapcntry(merged_set,cntry)
  /
report-cri2018v2.Costa_Rica
report-dom2016v2.Republica_Dominicana
report-gtm2018v2.Guatemala
report-hnd2018v2.Honduras
report-nic2018v2.Nicaragua
report-pan2016v2.Panama
report-slv2017v2.El_Salvador
report-mex2018bcie.Mexico


repbaseyr2-cri2018v2.Costa_Rica
repbaseyr2-dom2016v2.Republica_Dominicana
repbaseyr2-gtm2018v2.Guatemala
repbaseyr2-hnd2018v2.Honduras
repbaseyr2-nic2018v2.Nicaragua
repbaseyr2-pan2016v2.Panama
repbaseyr2-slv2017v2.El_Salvador
repbaseyr2-mex2018bcie.Mexico

repmacro2-cri2018v2.Costa_Rica
repmacro2-dom2016v2.Republica_Dominicana
repmacro2-gtm2018v2.Guatemala
repmacro2-hnd2018v2.Honduras
repmacro2-nic2018v2.Nicaragua
repmacro2-pan2016v2.Panama
repmacro2-slv2017v2.El_Salvador
repmacro2-mex2018bcie.Mexico

repmeso2-cri2018v2.Costa_Rica
repmeso2-dom2016v2.Republica_Dominicana
repmeso2-gtm2018v2.Guatemala
repmeso2-hnd2018v2.Honduras
repmeso2-nic2018v2.Nicaragua
repmeso2-pan2016v2.Panama
repmeso2-slv2017v2.El_Salvador
repmeso2-mex2018bcie.Mexico

reppov2-cri2018v2.Costa_Rica
reppov2-dom2016v2.Republica_Dominicana
reppov2-gtm2018v2.Guatemala
reppov2-hnd2018v2.Honduras
reppov2-nic2018v2.Nicaragua
reppov2-pan2016v2.Panama
reppov2-slv2017v2.El_Salvador
reppov2-mex2018bcie.Mexico

  /

;

ALIAS (c3,c3p);

PARAMETER
  vashrnom(merged_set,c3,t,sim)
  vareal(merged_set,c3,t,sim)
  varealXP(merged_set,c3,t,sim)
  vashrreal(merged_set,c3,t,sim)
  vashrrealsel(merged_set,c3,*,sim)
;

vashrnom(merged_set,c3,t,sim) = SUM(acrep$mapacrepc3(c3,acrep), sectorstruc2tmp(merged_set,acrep,'vashr',t,sim));

vareal(merged_set,c3,t,sim) = SUM(acrep$mapacrepc3(c3,acrep), sectorindic2tmp(merged_set,acrep,'VA','Real',t,sim));
varealXP(merged_set,c3,t,simcur)$vareal(merged_set,c3,t,'base') = 100*(vareal(merged_set,c3,t,simcur)/vareal(merged_set,c3,t,'base') - 1);

vashrreal(merged_set,c3,t,sim)$SUM(c3p, vareal(merged_set,c3,t,sim)) = 
  100*vareal(merged_set,c3,t,sim)/SUM(c3p, vareal(merged_set,c3p,t,sim));

vashrrealsel(merged_set,c3,'t-min',sim) = SUM(t$mapcntrytmin(merged_set,t), vashrreal(merged_set,c3,t,sim));
vashrrealsel(merged_set,c3,'t-max',sim) = vashrreal(merged_set,c3,'2030',sim);



* end: estructura sectorial


* start: base-yr
 
bopindic00(bopcol,cntry) = SUM(merged_set$mapcntry(merged_set,cntry), bopindic00tmp(merged_set,bopcol,'GDPshr'));
sectorstruc200(cntry,acrep,sectorcol) = SUM(merged_set$mapcntry(merged_set,cntry), sectorstruc200tmp(merged_set,acrep,sectorcol));


*facdemstruc200(cntry,ac,acp)      = SUM(merged_set$mapcntry(merged_set,cntry), facdemstruc200tmp(merged_set,ac,acp));                 
*demstruc200(cntry,ac,demcol)       = SUM(merged_set$mapcntry(merged_set,cntry), demstruc200tmp(merged_set,ac,demcol));        



* end: base-yr



* start: number of periods with non-base NE to base

SET
  trep2(t)
;

trep2(t)$(ORD(t)>=SUM(tp$tminrep(tp), ORD(tp)+1) AND ORD(t)<=SUM(tp$tmax(tp), ORD(tp))) = YES;

  
PARAMETER
  trep2nb

;
trep2nb = CARD(trep2);
DISPLAY trep2nb;



* end: number of periods with non-base NE to base



* start: macro indicators

PARAMETER
  prvconXP(cntry,sim,t)          consumo privado (cambio % respecto base)
  prvfixinvXP(cntry,sim,t)       inversion privada (cambio % respecto base)
  gdpfcXP(cntry,sim,t)       PIB CF (cambio % respecto base)
  gdpmpXP(cntry,sim,t)       PIB PM (cambio % respecto base)

  gdpfcgrowth(cntry,sim)     PIB CF (tasa crecimiento anual promedio %)
  gdpmpgrowth(cntry,sim)     PIB PM (tasa crecimiento anual promedio %)

  gdpfcYY(cntry,sim,t)       PIB CF (nivel)
  gdpmpYY(cntry,sim,t)       PIB PM (nivel)

  
;


prvconXP(cntry,sim,t)$trep(t) = SUM(merged_set$mapcntry(merged_set,cntry), macrealxp(merged_set,sim,'PrvCon',t));
prvconXP(cntry,sim,'avg2530') = SUM(merged_set$mapcntry(merged_set,cntry), SUM(trep2, macrealxp(merged_set,sim,'PrvCon',trep2)))/trep2nb;

prvfixinvXP(cntry,sim,t)$trep(t) = SUM(merged_set$mapcntry(merged_set,cntry), macrealxp(merged_set,sim,'PrvFixInv',t));
prvfixinvXP(cntry,sim,'avg2530') = SUM(merged_set$mapcntry(merged_set,cntry), SUM(trep2, macrealxp(merged_set,sim,'PrvFixInv',trep2)))/trep2nb;

gdpfcXP(cntry,sim,t)$trep(t) = SUM(merged_set$mapcntry(merged_set,cntry), macrealxp(merged_set,sim,'GDPFC',t));
gdpfcXP(cntry,sim,'avg2530') = SUM(merged_set$mapcntry(merged_set,cntry), SUM(trep2, macrealxp(merged_set,sim,'GDPFC',trep2)))/trep2nb;

gdpmpXP(cntry,sim,t)$trep(t) = SUM(merged_set$mapcntry(merged_set,cntry), macrealxp(merged_set,sim,'GDPMP',t));
gdpmpXP(cntry,sim,'avg2530') = SUM(merged_set$mapcntry(merged_set,cntry), SUM(trep2, macrealxp(merged_set,sim,'GDPMP',trep2)))/trep2nb;

gdpfcgrowth(cntry,sim) = SUM(merged_set$mapcntry(merged_set,cntry), macgrowth(merged_set,'GDPFC',sim));
gdpmpgrowth(cntry,sim) = SUM(merged_set$mapcntry(merged_set,cntry), macgrowth(merged_set,'GDPMP',sim));

gdpfcYY(cntry,sim,t) = SUM(merged_set$mapcntry(merged_set,cntry), macrealyy(merged_set,sim,'GDPFC',t));
gdpmpYY(cntry,sim,t) = SUM(merged_set$mapcntry(merged_set,cntry), macrealyy(merged_set,sim,'GDPMP',t));
 


* end: macro indicators




* start: mercado de trabajo

employtotxp(cntry,sim,t)$trep(t) = SUM(merged_set$mapcntry(merged_set,cntry), employxp2tmp(merged_set,sim,'total3',t));
employtotxp(cntry,sim,'avg2530') = SUM(merged_set$mapcntry(merged_set,cntry), SUM(trep2, employxp2tmp(merged_set,sim,'total3',trep2)))/trep2nb;


unempratetot(cntry,t,sim) = SUM(merged_set$mapcntry(merged_set,cntry), unempratetottmp(merged_set,t,sim));
  
PARAMETER
  unempratetotXP(cntry,sim,t)    tasa de desempleo y subempleo combinada (cambio % respecto base)
  unempratetotX(cntry,sim,t)  tasa de desempleo y subempleo combinada (%)
;

unempratetotX(cntry,sim,t)$trep(t) = unempratetot(cntry,t,sim);
unempratetotXP(cntry,simcur,t)$(trep(t)) = 100*(unempratetot(cntry,t,simcur)/unempratetot(cntry,t,'base') - 1);
  
* end: mercado de trabajo


* start: pobreza

fgt0yytmp(merged_set,sim,'rural',t) = fgt0yytmp(merged_set,sim,'h-rur',t);
fgt0yytmp(merged_set,sim,'urban',t) = fgt0yytmp(merged_set,sim,'h-urb',t);

fgt0yytmp(merged_set,sim,'h-rur',t)=0;
fgt0yytmp(merged_set,sim,'h-urb',t)=0;




PARAMETER   
  qhpcp0xtt(merged_set,ac,*,t,sim)
  fgt0xp(merged_set,sim,povcol,t)
  fgt0totXP(cntry,sim,t)                 tasa de pobreza (cambio % respecto base)
  
  fgt0totX(cntry,sim,t)                 tasa de pobreza (%)
  
  poptotX(cntry,t,sim)
  povnumb(cntry,t,sim)
  
;
qhpcp0xtt(merged_set,povcol,'p0',t,simcur)$(trep(t)) = fgt0yytmp(merged_set,simcur,povcol,t); 
qhpcp0xtt(merged_set,povcol,'QHPC',t,simcur)$(trep(t)) = qhpcX(merged_set,povcol,t,simcur);

fgt0xp(merged_set,simcur,povcol,t)$fgt0yytmp(merged_set,'base',povcol,t) = 
  100*(fgt0yytmp(merged_set,simcur,povcol,t)/fgt0yytmp(merged_set,'base',povcol,t) - 1);

fgt0totxp(cntry,sim,t)$trep(t) = SUM(merged_set$mapcntry(merged_set,cntry), fgt0xp(merged_set,sim,'nation',t));
fgt0totxp(cntry,sim,'avg2530') = SUM(merged_set$mapcntry(merged_set,cntry), SUM(trep2, fgt0xp(merged_set,sim,'nation',trep2)))/trep2nb;

fgt0totX(cntry,sim,t)$trep(t) = SUM(merged_set$mapcntry(merged_set,cntry), fgt0yytmp(merged_set,sim,'nation',t));


poptotX(cntry,t,sim) = SUM(merged_set$mapcntry(merged_set,cntry), poptotXtmp(merged_set,t,sim));
povnumb(cntry,t,sim) = poptotX(cntry,t,sim) * SUM(merged_set$mapcntry(merged_set,cntry), fgt0yytmp(merged_set,sim,'nation',t))/100;

* end: pobreza



* start: sectoriales

PARAMETER
  qvaXP(cntry,acrep,sim,t)          produccion (cambio % respecto base)
;




qvaXP(cntry,acrep,sim,t)$trep(t) = SUM(merged_set$mapcntry(merged_set,cntry), qvarealxp2(merged_set,sim,acrep,t)); 



* end: sectoriales



* save GDX file
EXECUTE_UNLOAD 'repbcie-merged.gdx',
  bopindic00
  prvconXP
  prvfixinvXP
  
  gdpfcXP  
  gdpfcgrowth
  gdpfcYY

  gdpmpXP  
  gdpmpgrowth
  gdpmpYY

  employtotxp
  unempratetotX
  unempratetotXP
  fgt0totX
  fgt0totxp
  
  qvaXP

  povnumb
  
  macrealxp
;

$ONECHO > taskout.txt

  htext = "Sheet, Description" rng = Contents!A1

  textid = bopindic00               rng = Contents!A2                   linkID = bopindic00
  text = "Contents"                  rng = bopindic00!A1                link=Contents!A1
  textid = bopindic00               rng = bopindic00!A2                         
  par = bopindic00                  rng = bopindic00!A30               rdim=1 cdim=1


  textid = prvconXP                  rng = Contents!A3                   linkID = prvconXP
  text = "Contents"                  rng = prvconXP!A1                   link=Contents!A1
  textid = prvconXP                  rng = prvconXP!A2                         
  par = prvconXP                     rng = prvconXP!A60                  rdim=2 cdim=1

  textid = prvfixinvXP               rng = Contents!A4                   linkID = prvfixinvXP
  text = "Contents"                  rng = prvfixinvXP!A1                link=Contents!A1
  textid = prvfixinvXP               rng = prvfixinvXP!A2                         
  par = prvfixinvXP                  rng = prvfixinvXP!A60               rdim=2 cdim=1

  textid = gdpfcXP               rng = Contents!A5                   linkID = gdpfcXP
  text = "Contents"                  rng = gdpfcXP!A1                link=Contents!A1
  textid = gdpfcXP               rng = gdpfcXP!A2                         
  par = gdpfcXP                  rng = gdpfcXP!A60               rdim=2 cdim=1

  textid = gdpfcgrowth               rng = Contents!A6                   linkID = gdpfcgrowth
  text = "Contents"                  rng = gdpfcgrowth!A1                link=Contents!A1
  textid = gdpfcgrowth               rng = gdpfcgrowth!A2                         
  par = gdpfcgrowth                  rng = gdpfcgrowth!A3               rdim=1 cdim=1

  textid = gdpfcYY               rng = Contents!A7                   linkID = gdpfcYY
  text = "Contents"                  rng = gdpfcYY!A1                link=Contents!A1
  textid = gdpfcYY               rng = gdpfcYY!A2                         
  par = gdpfcYY                  rng = gdpfcYY!A3               rdim=2 cdim=1

  textid = gdpmpXP               rng = Contents!A8                   linkID = gdpmpXP
  text = "Contents"                  rng = gdpmpXP!A1                link=Contents!A1
  textid = gdpmpXP               rng = gdpmpXP!A2                         
  par = gdpmpXP                  rng = gdpmpXP!A60               rdim=2 cdim=1

  textid = gdpmpgrowth               rng = Contents!A9                   linkID = gdpmpgrowth
  text = "Contents"                  rng = gdpmpgrowth!A1                link=Contents!A1
  textid = gdpmpgrowth               rng = gdpmpgrowth!A2                         
  par = gdpmpgrowth                  rng = gdpmpgrowth!A3               rdim=1 cdim=1

  textid = gdpmpYY               rng = Contents!A10                   linkID = gdpmpYY
  text = "Contents"                  rng = gdpmpYY!A1                link=Contents!A1
  textid = gdpmpYY               rng = gdpmpYY!A2                         
  par = gdpmpYY                  rng = gdpmpYY!A3               rdim=2 cdim=1

  textid = employtotxp               rng = Contents!A11                   linkID = employtotxp
  text = "Contents"                  rng = employtotxp!A1                link=Contents!A1
  textid = employtotxp               rng = employtotxp!A2                         
  par = employtotxp                  rng = employtotxp!A60               rdim=2 cdim=1


  textid = unempratetotX            rng = Contents!A12                   linkID = unempratetotX
  text = "Contents"                  rng = unempratetotX!A1             link=Contents!A1
  textid = unempratetotX            rng = unempratetotX!A2                        
  par = unempratetotX               rng = unempratetotX!A3            rdim=2 cdim=1

  textid = unempratetotXP            rng = Contents!A13                   linkID = unempratetotXP
  text = "Contents"                  rng = unempratetotXP!A1             link=Contents!A1
  textid = unempratetotXP            rng = unempratetotXP!A2                        
  par = unempratetotXP               rng = unempratetotXP!A3            rdim=2 cdim=1


  textid = fgt0totX                 rng = Contents!A14                   linkID = fgt0totX
  text = "Contents"                  rng = fgt0totX!A1                  link=Contents!A1
  textid = fgt0totX                 rng = fgt0totX!A2                         
  par = fgt0totX                    rng = fgt0totX!A30                 rdim=2 cdim=1  

  textid = fgt0totXP                 rng = Contents!A15                   linkID = fgt0totXP
  text = "Contents"                  rng = fgt0totXP!A1                  link=Contents!A1
  textid = fgt0totXP                 rng = fgt0totXP!A2                         
  par = fgt0totXP                    rng = fgt0totXP!A60                 rdim=2 cdim=1  

  textid = povnumb                 rng = Contents!A15                   linkID = povnumb
  text = "Contents"                  rng = povnumb!A1                  link=Contents!A1
  textid = povnumb                 rng = povnumb!A2                         
  par = povnumb                    rng = povnumb!A60                 rdim=2 cdim=1  


  textid = qvaXP                 rng = Contents!A16                   linkID = qvaXP
  text = "Contents"                  rng = qvaXP!A1                  link=Contents!A1
  textid = qvaXP                 rng = qvaXP!A2                         
  par = qvaXP                    rng = qvaXP!A3                 rdim=2 cdim=2  

  textid = macrealxp                 rng = Contents!A16                   linkID = macrealxp
  text = "Contents"                  rng = macrealxp!A1                  link=Contents!A1
  textid = macrealxp                 rng = macrealxp!A2                         
  par = macrealxp                    rng = macrealxp!A3                 rdim=3 cdim=1  



$OFFECHO            


EXECUTE "xlstalk.exe -C repbcie-merged.xlsx";
EXECUTE "gdxxrw repbcie-merged.gdx o=repbcie-merged.xlsx @taskout.txt";
*EXECUTE "xlstalk -O repbcie-merged.xlsx";



