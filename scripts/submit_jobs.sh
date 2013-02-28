#!/bin/sh

RUNLIST=$1
#CODE_DIR=/star/u/fazio/offline/users/fazio/vbasym
CODE_DIR=/star/u/smirnovd/vbasym/
OUT_DIR=/star/data05/scratch/fazio
#STAR_VER=SL11d
STAR_VER=SL12c
#STANA_OPTIONS="-j_-n100"
#STANA_OPTIONS="-j"
STANA_OPTIONS=""

export $STAR_VER

echo
echo RUNLIST       = $RUNLIST
echo CODE_DIR      = $CODE_DIR
echo OUT_DIR       = $OUT_DIR
echo STAR_VER      = $STAR_VER
echo STANA_OPTIONS = $STANA_OPTIONS
echo

for runNumber in `cat $RUNLIST`
do
   echo
   echo "Submitting job for runNumber = " $runNumber
   #star-submit-template -template $CODE_DIR/scripts/run11_job_template.xml -entities OUT_DIR=$OUT_DIR,codePath=$CODE_DIR,runNumber=$runNumber,STAR_VER=$STAR_VER,STANA_OPTIONS=$STANA_OPTIONS
   star-submit-template -template $CODE_DIR/scripts/run11_job_template_cut15.xml -entities OUT_DIR=$OUT_DIR,codePath=$CODE_DIR,runNumber=$runNumber,STAR_VER=$STAR_VER,STANA_OPTIONS=$STANA_OPTIONS
   echo
done
