* randprm-uniform.inc

* description: sensitivity w.r.t. parameters
* cada prm tiene dist uniforme independiente


* select random values for model parameters

* plantar semilla numeros aletorios
execseed = 1 + gmillisec(jnow);

$SET min 0.5
$SET max 0.5



PARAMETER
  frischMax(h)
  frischMin(h)
  frischRand(h)

  leselasMax(c,h)
  leselasMin(c,h)
  leselasRand(c,h)

  prodelasMax(a)
  prodelasMin(a)
  prodelasRand(a)

  tfpelasMax(a,ac)
  tfpelasMin(a,ac)
  tfpelasRand(a,ac)

  tradelasMax(c,ac)
  tradelasMin(c,ac)
  tradelasRand(c,ac)
    
  wagecurveMax(f)
  wagecurveMin(f)
  wagecurveRand(f)
  
;



prodelasMin(a) = prodelas(a)*(1-%min%);
prodelasMax(a) = prodelas(a)*(1+%max%);


*prodelasMin(a) = 0.15;
*prodelasMax(a) = 1.85;

tradelasMin(c,ac) = tradelas(c,ac)*(1-%min%);
tradelasMax(c,ac) = tradelas(c,ac)*(1+%max%);

tfpelasMin(a,ac) = tfpelas(a,ac)*(1-%min%);
tfpelasMax(a,ac) = tfpelas(a,ac)*(1+%max%);

leselasMin(c,h) = leselas(c,h)*(1-%min%);
leselasMax(c,h) = leselas(c,h)*(1+%max%);

frischMin(h) = frisch(h)*(1-%min%);
frischMax(h) = frisch(h)*(1+%max%);

wagecurveMin(f) = -unemp(f,'eta_wf')*(1-%min%);
wagecurveMax(f) = -unemp(f,'eta_wf')*(1+%max%);



* seleccionar valor parametros aletoriamente
* supuesto: dist uniforme
prodelasRand(a)        = UNIFORM(prodelasMin(a),prodelasMax(a));
tradelasRand(c,ac)     = UNIFORM(tradelasMin(c,ac),tradelasMax(c,ac));
tfpelasRand(a,ac)      = UNIFORM(tfpelasMin(a,ac),tfpelasMax(a,ac));
leselasRand(c,h)      = UNIFORM(leselasMin(c,h),leselasMax(c,h));
frischRand(h)          = UNIFORM(frischMax(h),frischMin(h));
wagecurveRand(f) = UNIFORM(wagecurveMin(f),wagecurveMax(f));

* asignar valor parametros
prodelas(a)$prodelas(a)               = prodelasRand(a);
tradelas(c,ac)$tradelas(c,ac)         = tradelasRand(c,ac);
tfpelas(a,ac)$tfpelas(a,ac)           = tfpelasRand(a,ac); 
leselas(c,h)$leselas(c,h)           = leselasRand(c,h);    
frisch(h)$frisch(h)                   = frischRand(h);       
unemp(f,'eta_wf')$unemp(f,'eta_wf') = -wagecurveRand(f);

$ONTEXT
* elasticities not being used are non-random
prodelas(angdp) = 0;
prodelas3(alei,f) = 0;
tradelas(c,'sigma_q')$(NOT SAM('row',c) AND NOT SUM(a, SAM(a,c))) = 0;
tradelas(c,'sigma_x')$(NOT SAM(c,'row') AND NOT SUM(a, SAM(a,c))) = 0;
$OFFTEXT