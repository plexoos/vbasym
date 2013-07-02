#!/bin/sh

RUNLIST=$1
CODE_DIR=~/vbasym/
OUT_DIR=~/stana_out/
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

mkdir -p $OUT_DIR/list_run11_cut05
mkdir -p $OUT_DIR/log_run11_cut05
mkdir -p $OUT_DIR/jets_run11_cut05
mkdir -p $OUT_DIR/data_run11_cut05
mkdir -p $OUT_DIR/jets_run11_cut05
mkdir -p $OUT_DIR/wtree_run11_cut05
mkdir -p $OUT_DIR/lumi_run11_cut05

for RUN_ID in `cat $RUNLIST`
do
   echo
   echo "Submitting job for RUN_ID = " $RUN_ID
   star-submit-template -template $CODE_DIR/scripts/run11_job_template.xml -entities OUT_DIR=$OUT_DIR,codePath=$CODE_DIR,RUN_ID=$RUN_ID,STAR_VER=$STAR_VER,STANA_OPTIONS=$STANA_OPTIONS
   echo
done
