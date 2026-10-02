* bdi2019-data.inc

DISPLAY "#### START: bdi2019-data.inc";    

* Read data from excel file
*$IF NOT SET NonIMv2 $CALL GDXXRW user-files\bdi2019-data.xlsx index=layout!A1
$CALL 'GDXXRW i=bdi2019-data.xlsx o=bdi2019-data.gdx index=layout!A1'
$GDXIN bdi2019-data.gdx


* start: IMv2

$LOADDC t
$LOADDC t2=tsol

$LOADDC tminrep
$LOADDC tmaxrep

* end: IMv2



DISPLAY "#### END: bdi2019-data.inc";    