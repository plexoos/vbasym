#!/bin/sh
mxEve=500000
codePath=/star/u/fazio/offline/users/fazio/vbasym
inpPath=$codePath/runlists
#inpPath=/star/data05/scratch/fazio/lists_run11
outPath=/star/data05/scratch/fazio/
schedMacro=$codePath/scripts
# codePath=/star/u/stevens4/wAnalysis/xSecPaper/gpc/aaa/
# inpPath=/star/institutions/iucf/stevens4/freezer/2011-Wana-SL11b/lists/embed/sl11b/
# outPath=/star/u/stevens4/wAnalysis/xSecPaper/gpc/aaa/
# schedMacro=$codePath/StRoot/StSpinPool/StWalgoB2009/macros/sched/
STAR_VER=SL11d

export $STAR_VER

echo outPath=$outPath= 
echo inpPath=$inpPath=
echo schedMacro=$schedMacro=
echo STAR_VER = $STAR_VER

# mkdir -p $outPath/
mkdir -p $outPath/jets_run11/
mkdir -p $outPath/data_run11/
mkdir -p $outPath/log_run11/
 
#for fillNo in Wplus0 Wplus1 Wplus2 Wplus3 Wplus4 Wplus5 Wplus6 Wplus7 Wplus8 Wplus9 Wplus10 Wminus0 Wminus1 Wminus2 Wminus3 Wminus4 Wminus5 Wminus6 Wminus7 Wminus8 Wminus9 Wminus10; do 
for fillNo in Wplus0 Wplus1 Wplus2 Wplus3 Wplus4 Wplus5 Wplus6 Wplus7 Wplus8 Wplus9 Wplus10; do 
#for fillNo in Wminus0 Wminus1 Wminus2 Wminus3 Wminus4 Wminus5 Wminus6 Wminus7 Wminus8 Wminus9 Wminus10; do 
# for fillNo in Wplus5; do 
    isMC=350

   echo submitting job for fillNo = $fillNo
    star-submit-template -template $schedMacro/singleMCWanaJobTempl.xml -entities  n1=$mxEve,outPath=$outPath,codePath=$codePath,inpPath=$inpPath,fillNo=$fillNo,isMC=$isMC,STAR_VER=$STAR_VER
done

#for fillNo in Wtau0 Wtau1 Wtau2 Wtau3 Wtau4 Wtau5 Wtau6 Wtau7 Wtau8 Wtau9 ; do 
#    isMC=351

#    star-submit-template -template $schedMacro/singleMCWanaJobTempl.xml -entities  n1=$mxEve,outPath=$outPath,codePath=$codePath,inpPath=$inpPath,fillNo=$fillNo,isMC=$isMC
#done

#for fillNo in Ze+e-Interf0 Ze+e-Interf1 Ze+e-Interf2 Ze+e-Interf3 Ze+e-Interf4 Ze+e-Interf5 Ze+e-Interf6 Ze+e-Interf7 Ze+e-Interf8 Ze+e-Interf9 Zany0 Zany1 Zany2 Zany3 Zany4 Zany5 Zany6 Zany7 Zany8 Zany9 ; do 
#    isMC=352


##    star-submit-template -template $schedMacro/singleMCWanaJobTempl.xml -entities  n1=$mxEve,outPath=$outPath,codePath=$codePath,inpPath=$inpPath,fillNo=$fillNo,isMC=$isMC,STAR_VER=$STAR_VER
##done

 
