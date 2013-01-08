#!/bin/sh

RUNLIST=$1
CODE_DIR=/star/u/smirnovd/vbasym/
OUT_DIR=/star/u/smirnovd/vbasym_results/
#STAR_VER=SL11d
STAR_VER=SL12c
STANA_OPTIONS="-j_-n100"
#STANA_OPTIONS="-n100"

export $STAR_VER

echo RUNLIST  = $RUNLIST
echo CODE_DIR = $CODE_DIR
echo OUT_DIR  = $OUT_DIR
echo STAR_VER = $STAR_VER
echo STANA_OPTIONS = $STANA_OPTIONS

for runNumber in `cat $RUNLIST`
do
   echo "Submitting job for runNumber = " $runNumber
   star-submit-template -template scripts/run11_job_template.xml -entities outPath=$OUT_DIR,codePath=$CODE_DIR,runNumber=$runNumber,STAR_VER=$STAR_VER,STANA_OPTIONS=$STANA_OPTIONS
done
