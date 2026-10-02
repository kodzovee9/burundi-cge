* repnpv.gms


* start: NPV using EV and private consumption for difference bt benefit and cost
 

SET
$ONTEXT
  tnpv(t)
  /
  2021*2030
  /
$OFFTEXT  
  tnpv(t)
  
  ir /ir025, ir050, ir075, ir1/
  
  npvcol1 /ev, PrvCon/
  
  npvcol2 /level, gdpshare/
;


tnpv(t)$(t.VAL>=SUM(tp$tminrep(tp), tp.VAL) AND t.VAL<=SUM(tp$tmaxrep(tp), tp.VAL)) = YES;
DISPLAY tnpv;

 
PARAMETER
  intrat(ir)             interest rate  
  /
  ir025    0.025
  ir050    0.05
  ir075    0.075
  ir1      0.1
  /

  disc(ir,t)           discount factor

  invnetinc2(npvcol1,t1,sim) net income from investment
  npv2(npvcol1,npvcol2,ir,sim)     NPV from alt calc (nominal or nominal share in 2019 GDP
;

* using EV as defined in reports
invnetinc2('ev',t,sim)$(tnpv(t) AND simcur(sim)) = 
  1e-15 + ( ev('total',t,sim) - ev('total',t,'base') );

* using private consumption
invnetinc2('PrvCon',t,sim)$(tnpv(t) AND simcur(sim)) = 
  1e-15 + gdpindic('PrvCon','Real',t,sim) - gdpindic('PrvCon','Real',t,'base');
DISPLAY invnetinc2;


*disc(ir,tnpv) = 1 / ( (1 + intrat(ir))**(ORD(tnpv)-1) );
disc(ir,tnpv) = 1 / ( (1 + intrat(ir))**(tnpv.POS-1) );

DISPLAY disc;

npv2(npvcol1,'level',ir,sim)$(simcur(sim) AND NOT simbase(sim))     = SUM(tnpv, invnetinc2(npvcol1,tnpv,sim)*disc(ir,tnpv));
npv2(npvcol1,'gdpshare',ir,sim)$(simcur(sim) AND NOT simbase(sim)) = 
  npv2(npvcol1,'level',ir,sim)/SUM(tmin, gdpindic('GDPMP','Nominal',tmin,sim))*100;
DISPLAY npv2;

* end: NPV using EV and private consumption for difference bt benefit and cost
