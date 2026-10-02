* imv2-default.inc

$IF %NonIMv2%==1 $EXIT

SET sim /base/;

* use base scenario to set non-base scenarios default rules for gov spending, gov receipts, and non-gov payments
govspndrulesim('base',acgovspnd) = 1;
govrecrulesim('base',acgovrec) = 1;
ngovpayrulesim('base',acngovpay) = 1;