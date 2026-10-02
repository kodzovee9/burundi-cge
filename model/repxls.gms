* rep.gms


$ONECHO > taskout-baseyr.txt
  par = sectorstruc00       rng = sectorstruc00!a3       rdim=1 cdim=1
  par = bopindic00          rng = bopindic00!a3          rdim=1 cdim=1
  par = gdpindic00          rng = gdpindic00!a3          rdim=1 cdim=1
  par = fiscalindic00       rng = fiscalindic00!a3       rdim=1 cdim=1
  par = taxstruc00          rng = taxstruc00!a3          rdim=1 cdim=1
  par = demstruc00          rng = demstruc00!a3          rdim=1 cdim=1
  par = incomestruc00       rng = incomestruc00!a3       rdim=1 cdim=1 
  par = facdemstruc00       rng = facdemstruc00!a3       rdim=1 cdim=1
  par = spndstruc00         rng = spndstruc00!a3         rdim=1 cdim=1
  par = coststruc00         rng = coststruc00!a3         rdim=1 cdim=1
  par = incomedistindic00   rng = incomedistindic00!A3   rdim=1 cdim=1
$OFFECHO            


$ONECHO > taskout-macro.txt


  par = macgrowth            rng = macgrowth!a3            rdim=1 cdim=1 
  par = macgrowthyy          rng = macgrowthyy!a3          rdim=2 cdim=1
                             
  par = govgdp               rng = govgdp!a3               rdim=1 cdim=1
  par = govgdpyy             rng = govgdpyy!a3             rdim=2 cdim=1
                             
  par = rowgdp               rng = rowgdp!a3               rdim=1 cdim=1
  par = rowgdpyy             rng = rowgdpyy!a3             rdim=2 cdim=1
                             
  par = macgdp               rng = macgdp!a3               rdim=1 cdim=1
  par = macgdpyy             rng = macgdpyy!a3             rdim=2 cdim=1
                             
  par = macreal              rng = macreal!a3              rdim=1 cdim=1
  par = macrealyy            rng = macrealyy!a3            rdim=2 cdim=1
  par = macrealxp            rng = macrealxp!a3            rdim=2 cdim=1

  par = govnom               rng = govnom!a3               rdim=1 cdim=1
  par = govnomyy             rng = govnomyy!a3             rdim=2 cdim=1


  
  
$OFFECHO            





$ONECHO > taskout-meso.txt



  par = qegrowth            rng = qegrowth!a3            rdim=1 cdim=1
  par = qegrowthyy          rng = qegrowthyy!a3          rdim=2 cdim=1
  par = qerealxp            rng = qerealxp!a3            rdim=2 cdim=1
                           
  par = qmgrowth            rng = qmgrowth!a3            rdim=1 cdim=1
  par = qmgrowthyy          rng = qmgrowthyy!a3          rdim=2 cdim=1
  par = qmrealxp            rng = qmrealxp!a3            rdim=2 cdim=1
                            
  par = qdgrowth            rng = qdgrowth!a3            rdim=1 cdim=1
  par = qdgrowthyy          rng = qdgrowthyy!a3          rdim=2 cdim=1
  par = qdrealxp            rng = qdrealxp!a3            rdim=2 cdim=1
                            
  par = qvagrowth           rng = qvagrowth!a3           rdim=1 cdim=1
  par = qvagrowthyy         rng = qvagrowthyy!a3         rdim=2 cdim=1
  par = qvarealxp           rng = qvarealxp!a3           rdim=2 cdim=1
  par = qvarealyy           rng = qvarealyy!a3           rdim=2 cdim=1
  
  par = qvagrowth2           rng = qvagrowth2!a3           rdim=1 cdim=1
  par = qvagrowthyy2         rng = qvagrowthyy2!a3         rdim=2 cdim=1
  par = qvarealxp2           rng = qvarealxp2!a3           rdim=2 cdim=1
  par = qvarealyy2           rng = qvarealyy2!a3           rdim=2 cdim=1
 
  par = qggrowth            rng = qggrowth!a3            rdim=1 cdim=1
  par = qggrowthyy          rng = qggrowthyy!a3          rdim=2 cdim=1
  par = qgrealxp            rng = qgrealxp!a3            rdim=2 cdim=1
                            
  par = qhgrowth            rng = qhgrowth!a3            rdim=2 cdim=1 
  par = qhgrowthyy          rng = qhgrowthyy!a3          rdim=3 cdim=1
  par = qhrealxp            rng = qhrealxp!a3            rdim=3 cdim=1
 
  par = employgrowth        rng = employgrowth!a3        rdim=1 cdim=1
  par = employgrowthyy      rng = employgrowthyy!a3      rdim=2 cdim=1
  par = employxp            rng = employxp!a3            rdim=2 cdim=1
                            
  par = wagegrowth          rng = wagegrowth!a3          rdim=1 cdim=1 
  par = wagegrowthyy        rng = wagegrowthyy!a3        rdim=2 cdim=1
  par = wagexp              rng = wagexp!a3              rdim=2 cdim=1
  
  par = capgdprep            rng = capgdprep!a3          rdim=1 cdim=1
  par = capgdpyy             rng = capgdpyy!a3           rdim=2 cdim=1
  par = caprealyy            rng = caprealyy!a3          rdim=2 cdim=1
  par = caprealxp            rng = caprealxp!a3          rdim=2 cdim=1
  
$OFFECHO            


$ONECHO > taskout-pov.txt

  par = fgt0                rng = fgt0!a3                rdim=1 cdim=1
  par = fgt1                rng = fgt1!a3                rdim=1 cdim=1
  par = fgt2                rng = fgt2!a3                rdim=1 cdim=1
                                                        
  par = fgt0yy              rng = fgt0yy!a3              rdim=2 cdim=1
  par = fgt1yy              rng = fgt1yy!a3              rdim=2 cdim=1
  par = fgt2yy              rng = fgt2yy!a3              rdim=2 cdim=1

  
  par = ginirep             rng = ginirep!a3             rdim=1 cdim=1
  par = ginirepyy           rng = ginirepyy!a3           rdim=2 cdim=1

$OFFECHO          




EXECUTE "xlstalk.exe -C repbaseyr.xlsx";
EXECUTE "gdxxrw repbaseyr2.gdx o=repbaseyr.xlsx @taskout-baseyr.txt";
*EXECUTE "xlstalk -O repbaseyr.xlsx";

EXECUTE "xlstalk.exe -C repmacro.xlsx";
EXECUTE "gdxxrw repmacro2.gdx o=repmacro.xlsx @taskout-macro.txt";
*EXECUTE "xlstalk -O repmacro.xlsx";

EXECUTE "xlstalk.exe -C repmeso.xlsx";
EXECUTE "gdxxrw repmeso2.gdx o=repmeso.xlsx @taskout-meso.txt";
*EXECUTE "xlstalk -O repmeso.xlsx";

$ONTEXT
EXECUTE "xlstalk.exe -C reppov.xlsx";
EXECUTE "gdxxrw reppov2.gdx o=reppov.xlsx @taskout-pov.txt";
*EXECUTE "xlstalk -O reppov.xlsx";
$OFFTEXT