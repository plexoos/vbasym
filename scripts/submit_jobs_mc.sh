#!/bin/sh

#RUNLIST_LABEL="MC_list_Wm_2012_"
RUNLIST_LABEL="MC_list_Wp_2012_"
#RUNLIST_LABEL="MC_list_WmToTauTau_2012_"
#RUNLIST_LABEL="MC_list_WpToTauTau_2012_"
#RUNLIST_LABEL="MC_list_QCD_2012_00_"
#RUNLIST_LABEL="MC_list_Ztoee_2012_"

# For Ztoee :
#RUNLIST_IDS=(00 01 02 03 04 05 06 07)

# For QCD :
#RUNLIST_IDS=(00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59)

# For WmToTauNutau :
#RUNLIST_IDS=(00 01 02 03 04 05 06 07 08 09 10 11 12)

# For WpToTauNutau :
#RUNLIST_IDS=(00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41)

# For Wm (to ee):
#RUNLIST_IDS=(00 01 02 03 04 05 06 07 08 09 10 11)

# For Wp (to ee):
#RUNLIST_IDS=(00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36)
RUNLIST_IDS=(07)

CODE_DIR=/star/u/fazio/offline/users/fazio/vbasym/
#CODE_DIR=/star/u/smirnovd/vbasym/
OUT_DIR=/star/data05/scratch/fazio/
#STAR_VER=SL11d
STAR_VER=SL12c
#STANA_OPTIONS="-j_-m_-n100"
STANA_OPTIONS="-j_-m"
#STANA_OPTIONS="-m"

export $STAR_VER

echo
echo RUNLIST_IDS   = ${RUNLIST_IDS[@]}
echo CODE_DIR      = $CODE_DIR
echo OUT_DIR       = $OUT_DIR
echo STAR_VER      = $STAR_VER
echo STANA_OPTIONS = $STANA_OPTIONS
echo

for RUNLIST_ID in ${RUNLIST_IDS[@]}
do
   RUNLIST_ID=${RUNLIST_LABEL}${RUNLIST_ID}
   echo
   echo "Submitting job for RUNLIST_ID = " $RUNLIST_ID
   star-submit-template -template $CODE_DIR/scripts/run11_job_template_mc.xml -entities OUT_DIR=$OUT_DIR,codePath=$CODE_DIR,RUNLIST_ID=$RUNLIST_ID,STAR_VER=$STAR_VER,STANA_OPTIONS=$STANA_OPTIONS
   echo
done
