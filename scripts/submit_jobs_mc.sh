#!/bin/sh
#
# Takes one argument:
#
# FILELIST is the name of the runlist file in $CODE_DIR/runlist/
#

FILELIST=$1
CODE_DIR=~/vbasym/
#STAR_VER=SL11d
STAR_VER=SL12c
#STANA_OPTIONS="-m_--jpm_0.5_-n1000_--jets"
STANA_OPTIONS="-m_--jpm_0.5_--jets"
#STANA_OPTIONS="-m_--jpm_0.5"
OUT_DIR=~/stana_out/${FILELIST}_${STANA_OPTIONS}

echo
echo FILELIST      = $FILELIST
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

FILELISTS=(`find ${CODE_DIR}/runlists/ -regextype posix-egrep -regex '^.*/runlists/'${FILELIST}'_[0-9]{2}$' -printf '%f\n'`)

for MY_FILELIST in ${FILELISTS[@]}
do
   echo
   echo "Submitting job for MY_FILELIST =" $MY_FILELIST
   star-submit-template -template $CODE_DIR/scripts/run11_job_template_mc.xml -entities OUT_DIR=$OUT_DIR,CODE_DIR=$CODE_DIR,MY_FILELIST=$MY_FILELIST,STAR_VER=$STAR_VER,STANA_OPTIONS=$STANA_OPTIONS
   echo
done
