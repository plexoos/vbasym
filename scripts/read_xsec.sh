#!/bin/sh
#
# Returns the total cross section from each of the PYTHIA log files and calculates the arithmetic averadge

# path for the W+ -> ev
# 2011
##LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Wplus_enu_20131001/P11id.SL11d/log
##LOGFILES_DIR=/star/data20/embedding/wEmbedding2011_transverse/Perugia0_ckin3_eq_10/Wplus_enu_20131001/P11id.SL11d/log
LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Perugia0_ckin3_eq_10/Wplus_enu_20131001/P11id.SL11d/log
# 2012
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/2012/Wplus-enu_100_20131001/log

# path for the W+ -> tv
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Perugia0_ckin3_eq_10/Wplus_taunu_20131001/P11id.SL11d/log
# 2012
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/2012/Wplus-taunu_100_20131001/log

# path for the W- -> ev
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Perugia0_ckin3_eq_10/Wminus_enu_20131001/P11id.SL11d/log
# 2012
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/2012/Wminus-enu_100_20131001/log

# path for the W- -> tv
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Perugia0_ckin3_eq_10/Wminus_taunu_20131001/P11id.SL11d/log
# 2012
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/2012/Wminus-taunu_100_20131001/log

# path for the Z0 -> ee
##LOGFILES_DIR=/star/data20/embedding/wEmbedding2011_transverse/Perugia0_ckin3_eq_10/Z_eplus_eminus_inter_20131001/P11id.SL11d/log
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/Perugia0_ckin3_eq_10/Z_eplus_eminus_inter_20131001_new/P11id.SL11d/log
# 2012
#LOGFILES_DIR=/star/institutions/bnl_me/fazio/stana_out/logf/2012/Z-eplus-eminus-inter_100_20131001/log

echo
echo LOGFILES_DIR    = $LOGFILES_DIR
echo

# uncomment grep for the Ws
grep "I   2 f + fbar' -> W+/-            I" $LOGFILES_DIR/R*.log | awk '{printf("%f\n", $13);}'  > xsec
# 2012
#grep "I   2 f + fbar' -> W+/-            I" $LOGFILES_DIR/*.log | awk '{printf("%s\n", $13);}'  > xsec

# uncomment grep for the Z0
#grep "I   1 f + fbar ->" $LOGFILES_DIR/*.log | awk '{printf("%s\n", $13);}'  > xsec


#awk '{ print $1, k+=$1, sum+=1, av=10^(-8)*k/sum}  END {printf("The averadge cross-section is: %e (mb) \n",  av)}' < xsec
awk '{ if($1 >1.2) 
          printf( "%s, %e, %i, %g\n",$1, k+=10^(-8)*$1, sum+=1, av=k/sum);
       else if($1 <1.2)
          printf( "%s, %e, %i, %g\n",$1, k+=10^(-7)*$1, sum+=1, av=k/sum);}
  END {printf("The averadge cross-section is: %e (mb) \n",  av)}' < xsec

#grep "NUMBER OF EVENTS PROCESSED" $LOGFILES_DIR/*.log | awk '{printf("%f\n", $13);}'  > xsec

grep "NUMBER OF EVENTS PROCESSED" $LOGFILES_DIR/*.log | awk '{evt+=$8} END {printf("The processed number of events is: %d \n", evt);}'
##grep "NUMBER OF EVENTS PROCESSED" $LOGFILES_DIR/*.log | awk '{print $8, evt+=$8} END {printf("The processed number of events is: %d \n", evt);}'

/bin/rm xsec

