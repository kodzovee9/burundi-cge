* repbasyr.gms

DISPLAY "#### START: repbasyr.inc";

PARAMETER

  sectorstruc00(ac,sectorcol)        base-year sectoral structure 
  sectorstruc200(acrep,sectorcol)         base-year sectoral structure 

  bopindic00(bopcol,kgdp)            base-year balance of payments
  gdpindic00(igdp,kgdp)              base-year GDP table
  fiscalindic00(fiscalcol,kgdp)      base-year fiscal indicators
  fiscaldetindic00(fiscaldetcol,ac,kgdp)   base-year detailed fiscal indicators
  taxstruc00(ac,taxcol)              base-year tax structure

  demstruc00(ac,demcol)              base-year demand structure
  demstruc200(acrep,demcol)               base-year demand structure

  incomestruc00(ins,ac)              base-year income sources by institution
  incomestruc200(acrep,acrep)              base-year income sources by institution

  facdemstruc00(ac,ac)               base-year factor demand structure by activity
  facdemstruc200(acrep,acrepp)                base-year factor demand structure by activity

  spndstruc00(ac,ac)                 base-year spending structure by institution
  coststruc00(ac,ac)                 base-year cost structure by activity
  incomedistindic00(inccol,ac)       base-year income distribution 

  taxratcom00(c,acgovrec)            base-year tax rates for taxes on commodities
  taxratact00(a,acgovrec)            base-year tax rates for taxes on activities
  subratcom00(c,ac)                  base-year subsidy rates for subsidities on commodities by demander
  taxratins00(ins)                   base-year tax rates for tax on income
  
  labordemstruc00(ac,ac)                 base-year structure of labor demand by sector
  labordemstruc200(acrep,acrep)          base-year structure of labor demand by sector (aggregated activities)
  employsectorstruc00(ac,ac)             base-year sectoral structure of employment
  employsectorstruc200(acrep,acrep)      base-year sectoral structure of employment (aggregated activities)  
  
;

sectorstruc00(ac,sectorcol)   = SUM(tmin, sectorstruc(ac,sectorcol,tmin,'base'));
sectorstruc200(acrep,sectorcol)   = SUM(tmin, sectorstruc2(acrep,sectorcol,tmin,'base'));

bopindic00(bopcol,kgdp)       = SUM(tmin, bopindic(bopcol,kgdp,tmin,'base'));         
gdpindic00(igdp,kgdp)         = SUM(tmin, gdpindic(igdp,kgdp,tmin,'base'));           
fiscalindic00(fiscalcol,kgdp) = SUM(tmin, fiscalindic(fiscalcol,kgdp,tmin,'base'));   
fiscaldetindic00(fiscaldetcol,ac,kgdp) = SUM(tmin, fiscaldetindic(fiscaldetcol,ac,kgdp,tmin,'base'));
taxstruc00(ac,taxcol)         = SUM(tmin, taxstruc(ac,taxcol,tmin,'base'));

demstruc00(ac,demcol)         = SUM(tmin, demstruc('shrTOT',ac,demcol,tmin,'base'));
demstruc200(acrep,demcol)         = SUM(tmin, demstruc2('shrTOT',acrep,demcol,tmin,'base'));

incomestruc00(ins,ac)         = SUM(tmin, incomestruc('INCshr',ins,ac,tmin,'base')); 
incomestruc200(acrepp,acrep)         = SUM(tmin, incomestruc2('INCshr',acrepp,acrep,tmin,'base')); 

facdemstruc00(ac,acp)         = SUM(tmin, facdemstruc(ac,acp,tmin,'base'));
facdemstruc200(acrep,acrepp)         = SUM(tmin, facdemstruc2(acrep,acrepp,tmin,'base'));

spndstruc00(ac,acp)           = SUM(tmin, spndstruc('SPNDshr',ac,acp,tmin,'base'));
coststruc00(ac,acp)           = SUM(tmin, coststruc(ac,acp,tmin,'base')); 
incomedistindic00(inccol,ac)  = SUM(tmin, incomedistindic(inccol,ac,tmin,'base')); 
taxratcom00(c,acgovrec)       = SUM(tmin, taxratcom(c,acgovrec,tmin,'base')); 
taxratact00(a,acgovrec)       = SUM(tmin, taxratact(a,acgovrec,tmin,'base'));  
subratcom00(c,ac)             = SUM(tmin, subratcom(c,ac,tmin,'base'));        
taxratins00(ins)              = SUM(tmin, taxratins(ins,tmin,'base'));


labordemstruc00(ac,acp)            = SUM(tmin, labordemstruc(ac,acp,tmin,'base'));                
labordemstruc200(acrep,acrepp)     = SUM(tmin, labordemstruc2(acrep,acrepp,tmin,'base'));         
employsectorstruc00(ac,acp)        = SUM(tmin, employsectorstruc(ac,acp,tmin,'base'));            
employsectorstruc200(acrep,acrepp) = SUM(tmin, employsectorstruc2(acrep,acrepp,tmin,'base'));     


* save GDX file
$IF %NonIMv2%==1 EXECUTE_UNLOAD 'repbaseyr2-%app%.gdx',
$IF NOT %NonIMv2%==1 EXECUTE_UNLOAD 'repbaseyr2.gdx',
  sectorstruc00
  sectorstruc200
  
  bopindic00
  gdpindic00
  fiscalindic00
  fiscaldetindic00
  taxstruc00
  
  demstruc00
  demstruc200
  
  incomestruc00
  
  facdemstruc00
  facdemstruc200
  
  spndstruc00
  coststruc00
  incomedistindic00
  taxratcom00
  taxratact00
  subratcom00
  taxratins00
  
  labordemstruc00
  labordemstruc200
  employsectorstruc00
  employsectorstruc200
  
;

DISPLAY "#### END: repbasyr.inc";