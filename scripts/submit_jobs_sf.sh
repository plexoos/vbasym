#!/bin/sh
mxEve=500000
codePath=/star/u/fazio/offline/users/fazio/vbasym
inpPath=/star/data05/scratch/fazio/lists_run11
outPath=/star/data05/scratch/fazio/
STAR_VER=SL11d

export $STAR_VER

echo outPath  = $outPath
echo inpPath  = $inpPath
echo STAR_VER = $STAR_VER

#for runNumber in `cat run_list_11_nfs.txt`
#for runNumber in `cat run_list_11_all.txt`
#for runNumber in `cat run_list_11_test.txt`
for runNumber in `cat $codePath/runlists/run_list_11_test_short.txt`
#for runNumber in `cat run_list_11_all_reduced2_test.txt`
#for runNumber in `cat run_list_11_all_reduced2.txt`
do
   echo submitting job for runNumber = $runNumber
   star-submit-template -template run11_job_template_sf.xml -entities  n1=$mxEve,outPath=$outPath,codePath=$codePath,inpPath=$inpPath,runNumber=$runNumber,STAR_VER=$STAR_VER
done
 
