* calc-ginihh.inc

* start: define additional mappings for gini calculations

SET 
  hagg(ac)
  /
  h-wnc-all
  h-wc-all
  h-e-all
  /

  hjor(ac)
  /
  h-jor
  /


  mobshagg(obs,hagg)
  mobshjor(obs,hjor)
;

mobshagg(obs,'h-wnc-all')$SUM(hwnc$mobsh(obs,hwnc), 1) = YES;
mobshagg(obs,'h-wc-all')$SUM(hwc$mobsh(obs,hwc), 1) = YES;
mobshagg(obs,'h-e-all')$SUM(he$mobsh(obs,he), 1) = YES;

mobshjor(obs,'h-jor')$SUM(hagg$mobshagg(obs,hagg), 1) = YES;




PARAMETER
  obstot_i2(ac)
  mutot_i2(ac)
;


LOOP(sim$simcur(sim),
    
  LOOP(t$tineqrep(t),

* start: households in SAM

    obstot_i2(h) = SUM(obs$mobsh(obs,h), popwt(obs,t,sim));
    mutot_i2(h) = SUM(obs$mobsh(obs,h), popwt(obs,t,sim)*welfare_i(obs,t,sim)) / obstot_i2(h);

*$ONTEXT
*HL. Calc of gini by h for tmin and simbase (not for by h for other t or other sim)
    gini(h,t,sim)$(tmin(t) AND simbase(sim)) 
     = SUM(obs$mobsh(obs,h), SUM(obsp$mobsh(obsp,h), popwt(obs,t,sim)*popwt(obsp,t,sim)*ABS(welfare_i(obs,t,sim) - welfare_i(obsp,t,sim))));

    gini(h,t,sim)$(tmin(t) AND simbase(sim)) 
     = gini(h,t,sim)/(2*POWER(obstot_i2(h),2)*mutot_i2(h));
*$OFFTEXT
* end: households in SAM



* start: agg households in hagg

    obstot_i2(hagg) = SUM(obs$mobshagg(obs,hagg), popwt(obs,t,sim));
    mutot_i2(hagg) = SUM(obs$mobshagg(obs,hagg), popwt(obs,t,sim)*welfare_i(obs,t,sim)) / obstot_i2(hagg);

*$ONTEXT
    gini(hagg,t,sim)
 	$(tmin(t) AND simbase(sim)) 
     = SUM(obs$mobshagg(obs,hagg), SUM(obsp$mobshagg(obsp,hagg), popwt(obs,t,sim)*popwt(obsp,t,sim)*ABS(welfare_i(obs,t,sim) - welfare_i(obsp,t,sim))));

    gini(hagg,t,sim)
 	$(tmin(t) AND simbase(sim)) 
     = gini(hagg,t,sim)/(2*POWER(obstot_i2(hagg),2)*mutot_i2(hagg));
*$OFFTEXT

* end: agg households in hagg


*$ONTEXT
* start: agg households in hjor

    obstot_i2(hjor) = SUM(obs$mobshjor(obs,hjor), popwt(obs,t,sim));
    mutot_i2(hjor) = SUM(obs$mobshjor(obs,hjor), popwt(obs,t,sim)*welfare_i(obs,t,sim)) / obstot_i2(hjor);

    gini(hjor,t,sim)
	$(tmin(t) AND simbase(sim)) 
     = SUM(obs$mobshjor(obs,hjor), SUM(obsp$mobshjor(obsp,hjor), popwt(obs,t,sim)*popwt(obsp,t,sim)*ABS(welfare_i(obs,t,sim) - welfare_i(obsp,t,sim))));

    gini(hjor,t,sim)
	$(tmin(t) AND simbase(sim)) 
     = gini(hjor,t,sim)/(2*POWER(obstot_i2(hjor),2)*mutot_i2(hjor));

* end: agg households in hjor
*$OFFTEXT

  );
);


* end: define additional mappings for gini calculations

