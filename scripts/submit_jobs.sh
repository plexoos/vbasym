#!/bin/sh
#
# Requires at least one argument:
#
# submit_jobs.sh RUN_FILE_LIST [STANA_OPTIONS]
#
# RUN_FILE_LIST is the name of the runlist file in $VBASYM_DIR/runlist/
# STANA_OPTIONS is a list of arguments to be passed to the stana executable
#
# Usage example:
#
# ./submit_jobs.sh run11_mc_Wp2enu.lis -w --mctype 1 --jets
#
# Current usage tips:
#
# STAR_VERSION=SL11d  for Run 11 production P11id
# STAR_VERSION=SL13b  for Run 12 production P13ib
# STAR_VERSION=DEV    for Run 13 production
#
# STANA_OPTIONS="--jpm_0.5_-n1000_--jets"
# STANA_OPTIONS="--jpm_0.5_--run_11_--jets"
# STANA_OPTIONS="--jpm_0.5_--run_11"
# STANA_OPTIONS=""
# STANA_OPTIONS="-z"
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

case $STANA_OPTIONS in
*"--jets"* ) LOG_DIR="log_jets" ;;
*) LOG_DIR="log" ;;
esac

case $STANA_OPTIONS in
*"-m"* ) MC_SFX="_mc" ;;
*) MC_SFX="" ;;
esac

case $STANA_OPTIONS in
*"--mctype_1"* | *"--mctype=1"* | *"--mctype_2"* | *"--mctype=2"* | *"--mctype_3"* | *"--mctype=3"*) MC_SFX="_mc" ;;
*) MC_SFX="" ;;
esac

echo
echo RUN_FILE_LIST = $RUN_FILE_LIST
echo VBASYM_DIR    = $VBASYM_DIR
echo OUT_DIR       = $VBASYM_RESULTS_DIR/$RUN_FILE_LIST
echo STAR_VER      = $STAR_VERSION
echo STANA_OPTIONS = $STANA_OPTIONS
echo RUN_PERIOD    = $RUN_PERIOD
echo LOG_DIR       = $LOG_DIR
echo MC_SFX        = $MC_SFX
echo

mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/lists
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/$LOG_DIR
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/jets
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/hist
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/tree
mkdir -p $VBASYM_RESULTS_DIR/$RUN_FILE_LIST/lumi

for JOB_RUN_FILE_NAME in `cat $VBASYM_DIR/runlists/$RUN_FILE_LIST`
do
   echo
   echo "Submitting job for JOB_RUN_FILE_NAME =" $JOB_RUN_FILE_NAME
   star-submit-template -template $VBASYM_DIR/scripts/run${RUN_PERIOD}_job_template${MC_SFX}.xml \
      -entities OUT_DIR=$VBASYM_RESULTS_DIR/$RUN_FILE_LIST,CODE_DIR=$VBASYM_DIR,JOB_RUN_FILE_NAME=$JOB_RUN_FILE_NAME,STAR_VER=$STAR_VERSION,STAR_HOST_SYS=$STAR_HOST_SYS,STANA_OPTIONS=$STANA_OPTIONS,LOG_DIR=$LOG_DIR
   echo
   sleep 1
done
