#!/bin/sh
#
# Returns the total cross section from each of the PYTHIA log files and calculates the arithmetic averadge


LOGFILES_DIR=/star/institutions/bnl_me//fazio/stana_out/logf/Wplus_enu_20131001/P11id.SL11d/log

echo
#echo RUNLIST         = $RUNLIST
echo CODE_DIR        = $CODE_DIR
echo LOGFILES_DIR    = $LOGFILES_DIR
echo


grep "I   2 f + fbar' -> W+/-            I" $LOGFILES_DIR/R*.log | awk '{printf("%f\n", $13);}'  > xsec

awk '{ print $1, k+=$1, sum+=1, av=10^(-8)*k/sum}  END {printf("The averadge cross-section is: %e (mb) \n",  av)}' < xsec

/bin/rm xsec