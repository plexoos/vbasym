#!/bin/sh
#
# Takes one argument: RUN_FILE_LIST
#
# RUN_FILE_LIST is the name of the runlist file in $VBASYM_DIR/runlist/
#

RUN_FILE_LIST=$1
STANA_OPTIONS=("$@")
unset STANA_OPTIONS[0]

STANA_OPTIONS=`echo ${STANA_OPTIONS[*]} | sed 's/ /_/g'`

case $STANA_OPTIONS in
*"-r12"* | *"-r_12"* | *"--run=12"*) RUN_PERIOD=12 ;;
*"-r13"* | *"-r_13"* | *"--run=13"*) RUN_PERIOD=13 ;;
*) RUN_PERIOD=11 ;;
esac

#FILELIST=$1
#CODE_DIR=~/vbasym/
#STAR_VER=SL11d
#STAR_VER=SL12c
#STANA_OPTIONS=$2
#STANA_OPTIONS="-m_--jpm_0.5_-n1000_--jets"
#STANA_OPTIONS="-m_-w_--jpm_0.5_--jets"
#STANA_OPTIONS="-m_-w_--jpm_0.5"
#OUT_DIR=/star/institutions/bnl_me/fazio/stana_out/runlists/${FILELIST}_${STANA_OPTIONS}

echo
echo RUN_FILE_LIST = $RUN_FILE_LIST
echo VBASYM_DIR    = $VBASYM_DIR
echo OUT_DIR       = $VBASYM_RESULTS_DIR/$RUN_FILE_LIST
echo STAR_VER      = $STAR_VERSION
echo STANA_OPTIONS = $STANA_OPTIONS
echo RUN_PERIOD    = $RUN_PERIOD
echo

mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/lists
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/log
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/jets
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/hist
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/tree
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/lumi

for JOB_RUN_FILE_NAME in `cat $VBASYM_DIR/runlists/$RUN_FILE_LIST`
do
   echo
   echo "Submitting job for JOB_RUN_FILE_NAME =" $JOB_RUN_FILE_NAME
   star-submit-template -template $VBASYM_DIR/scripts/run${RUN_PERIOD}_job_template_mc.xml \
      -entities OUT_DIR=$VBASYM_RESULTS_DIR/$RUN_FILE_LIST,CODE_DIR=$VBASYM_DIR,JOB_RUN_FILE_NAME=$JOB_RUN_FILE_NAME,STAR_VER=$STAR_VERSION,STAR_HOST_SYS=$STAR_HOST_SYS,STANA_OPTIONS=$STANA_OPTIONS
   echo
   sleep 1
done
