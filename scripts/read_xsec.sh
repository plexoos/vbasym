#!/bin/sh
#
# Returns the total cross section from each of the PYTHIA log files and calculates the arithmetic averadge

# path for the W+ -> ev
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Wplus_enu_20131001/P11id.SL11d/log

# path for the W+ -> tv
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Wplus_taunu_20131001/P11id.SL11d/log

# path for the W- -> ev
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Wminus_enu_20131001/P11id.SL11d/log

# path for the W- -> tv
LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Wminus_taunu_20131001/P11id.SL11d/log

# path for the Z0 -> ee
#LOGFILES_DIR=/star/data20/embedding/wEmbedding2011_transverse/Perugia0_ckin3_eq_10/Z_eplus_eminus_inter_20131001/P11id.SL11d/log

echo
echo LOGFILES_DIR    = $LOGFILES_DIR
echo

# uncomment grep for the Ws
grep "I   2 f + fbar' -> W+/-            I" $LOGFILES_DIR/R*.log | awk '{printf("%f\n", $13);}'  > xsec

# uncomment grep for the Z0
#grep "I   1 f + fbar ->" $LOGFILES_DIR/R*.log | awk '{printf("%f\n", $13);}'  > xsec


awk '{ print $1, k+=$1, sum+=1, av=10^(-8)*k/sum}  END {printf("The averadge cross-section is: %e (mb) \n",  av)}' < xsec

grep "NUMBER OF EVENTS PROCESSED" $LOGFILES_DIR/R*.log | awk '{printf("%f\n", $13);}'  > xsec

grep "NUMBER OF EVENTS PROCESSED" $LOGFILES_DIR/R*.log | awk '{evt+=$8} END {printf("The processed number of events is: %d \n", evt);}'

/bin/rm xsec

