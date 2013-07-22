#!/bin/sh
#
# Takes one argument:
#
# RUNLIST is the name of the runlist file in $CODE_DIR/runlist/
#

RUNLIST=$1
CODE_DIR=~/vbasym/
#STAR_VER=SL11d
STAR_VER=SL12c
#STANA_OPTIONS=$2
#STANA_OPTIONS="--jpm_0.5_-n1000_--jets"
#STANA_OPTIONS="--jpm_3.5_--jets"
STANA_OPTIONS="--jpm_0.5"
OUT_DIR=~/stana_out/${RUNLIST}_${STANA_OPTIONS}

echo
echo RUNLIST       = $RUNLIST
echo CODE_DIR      = $CODE_DIR
echo OUT_DIR       = $OUT_DIR
echo STAR_VER      = $STAR_VER
echo STANA_OPTIONS = $STANA_OPTIONS
echo

mkdir -p $OUT_DIR/lists
mkdir -p $OUT_DIR/log
mkdir -p $OUT_DIR/jets
mkdir -p $OUT_DIR/hist
mkdir -p $OUT_DIR/tree
mkdir -p $OUT_DIR/lumi

for RUN_ID in `cat $CODE_DIR/runlists/$RUNLIST`
do
   echo
   echo "Submitting job for RUN_ID =" $RUN_ID
   star-submit-template -template $CODE_DIR/scripts/run11_job_template.xml -entities OUT_DIR=$OUT_DIR,CODE_DIR=$CODE_DIR,RUN_ID=$RUN_ID,STAR_VER=$STAR_VER,STANA_OPTIONS=$STANA_OPTIONS
   echo
   sleep 3
done
