#!/bin/sh
#
# Requires at least one argument:
#
# submit_jobs.sh RUNLIST [STANA_OPTIONS]
#
# RUNLIST is the name of the runlist file in $VBASYM_DIR/runlist/
# STANA_OPTIONS is a list of arguments to be passed to the stana executable
#
# Usage example:
#
# submit_jobs.sh run11_pp_transverse -z -o -m -n 1000
#
# Current usage tips:
#
# The --jets option passed to stana must be the last one in the STANA_OPTIONS
# list
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

RUNLIST=$1
STANA_OPTIONS=("$@")
unset STANA_OPTIONS[0]

STANA_OPTIONS=`echo ${STANA_OPTIONS[*]} | sed 's/ /_/g'`

case $STANA_OPTIONS in
*"-r12"* | *"-r_12"* | *"--run=12"*) RUN_PERIOD=12 ;;
*"-r13"* | *"-r_13"* | *"--run=13"*) RUN_PERIOD=13 ;;
*) RUN_PERIOD=11 ;;
esac

echo
echo RUNLIST       = $RUNLIST
echo VBASYM_DIR    = $VBASYM_DIR
echo OUT_DIR       = $VBASYM_RESULTS_DIR/$RUNLIST
echo STAR_VER      = $STAR_VERSION
echo STANA_OPTIONS = $STANA_OPTIONS
echo RUN_PERIOD    = $RUN_PERIOD
echo

mkdir -p $VBASYM_RESULTS_DIR/$RUNLIST/lists
mkdir -p $VBASYM_RESULTS_DIR/$RUNLIST/log
mkdir -p $VBASYM_RESULTS_DIR/$RUNLIST/jets
mkdir -p $VBASYM_RESULTS_DIR/$RUNLIST/hist
mkdir -p $VBASYM_RESULTS_DIR/$RUNLIST/tree
mkdir -p $VBASYM_RESULTS_DIR/$RUNLIST/lumi

for RUN_ID in `cat $VBASYM_DIR/runlists/$RUNLIST`
do
   echo
   echo "Submitting job for RUN_ID =" $RUN_ID
   star-submit-template -template $VBASYM_DIR/scripts/run${RUN_PERIOD}_job_template.xml \
   -entities OUT_DIR=$VBASYM_RESULTS_DIR,CODE_DIR=$VBASYM_DIR,RUN_ID=$RUN_ID,STAR_VER=$STAR_VERSION,STAR_HOST_SYS=$STAR_HOST_SYS,STANA_OPTIONS=$STANA_OPTIONS
   echo
   sleep 1
done
