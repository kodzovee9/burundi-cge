* piecemeal-prm.inc

$IF NOT SET iter $SET iter 004

SET ii /iter0001*iter1000/
PARAMETER scalelas(ii)
$ONTEXT
/
iter0001   0.275
iter0002   0.50
iter0003   0.75
iter0004   1
iter0005   1.5
iter0006   2
iter0007   2.5
iter0008   3
iter0009   3.5
iter0010   4
iter0011   4.5
iter0012   5
iter0013   5.5
iter0014   6
iter0015   6.5
iter0016   7
iter0017   7.5
iter0018   8
iter0019   8.5
iter0020   9
iter0021   9.5
iter0022   10
iter0023   20
iter0024   21
iter0025   22
iter0026   23
iter0027   24
iter0028   25
iter0029   26
iter0030   27
iter0031   28
iter0032   29
iter0033   30
 
/
$OFFTEXT

/
iter0001   0.25
iter0002   0.5
iter0003   1
iter0004   1.5
iter0005   2.05
iter0006   2.5
iter0007   3
iter0008   10

/
;

* alt:
*scalelas(ii) = 0.25*ORD(ii);
DISPLAY scalelas;


* elas centrales
*prodelas2(angdp,f)$prodelas2(angdp,f) = prodelas2(angdp,f)*scalelas('iter%iter%');
*prodelas2(a,f)$prodelas2(a,f) = prodelas2(a,f)*scalelas('iter%iter%');
leselasp(cleif,h) = leselasp(cleif,h)*scalelas('iter%iter%'); 
*conelas1(c,h) = conelas1(c,h)*scalelas('iter%iter%');  
*leselasp('c-caretot',h) = leselasp('c-caretot',h)*scalelas('iter%iter%'); 


*tfpelas(a,invg) = tfpelas(a,invg)/2;

*tfpelas(a,invg) = tfpelas(a,invg)*scalelas('%iter%');

*tradelas(c,'sigma_Q') = 1.05;
*tradelas(c,'sigma_Q') = tradelas(c,'sigma_Q')*scalelas('%iter%');

*tradelas(c,'sigma_X') = 1.05;
*tradelas(c,'sigma_X') = tradelas(c,'sigma_X')*scalelas('%iter%');

*scalelas(ii) = 1*ORD(ii);
*prodelas(a)  = prodelas(a)*scalelas('%iter%');


*tradelas(c,'sigmat') = tradelas(c,'sigmat')*scalelas('%iter%');

*unempdata(f,'eta_wf') = unempdata(f,'eta_wf')*scalelas('%iter%');

*tradelas(c,'eta')    = tradelas(c,'eta')*mu('eta');

*leselas(c,h)         = leselas(c,h)*mu('leselas');
*frisch(h)            = frisch(h)*mu('frisch');
