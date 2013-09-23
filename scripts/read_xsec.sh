#!/bin/sh
#
# Returns the total cross section from each of the PYTHIA log files and calculates the arithmetic averadge

# path for the Ws
#LOGFILES_DIR=/star/institutions/bnl_me//fazio/stana_out/logf/Wplus_enu_20131001/P11id.SL11d/log

# path for the Z0
LOGFILES_DIR=/star/data20/embedding/wEmbedding2011_transverse/Perugia0_ckin3_eq_10/Z_eplus_eminus_inter_20131001/P11id.SL11d/log

echo
echo LOGFILES_DIR    = $LOGFILES_DIR
echo

# uncomment grep for the Ws
#grep "I   2 f + fbar' -> W+/-            I" $LOGFILES_DIR/R*.log | awk '{printf("%f\n", $13);}'  > xsec

# uncomment grep for the Z0
grep "I   1 f + fbar ->" $LOGFILES_DIR/R*.log | awk '{printf("%f\n", $13);}'  > xsec


awk '{ print $1, k+=$1, sum+=1, av=10^(-8)*k/sum}  END {printf("The averadge cross-section is: %e (mb) \n",  av)}' < xsec

/bin/rm xsec