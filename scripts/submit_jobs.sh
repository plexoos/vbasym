#!/bin/sh
mxEve=500000
codePath=/star/u/smirnovd/vbasym_cvs/
inpPath=/star/u/smirnovd/vbasym_results/lists
outPath=/star/u/smirnovd/vbasym_results/
STAR_VER=SL11d

export $STAR_VER

echo outPath  = $outPath
echo inpPath  = $inpPath
echo STAR_VER = $STAR_VER

#for runNumber in `cat run_list_11_nfs.txt`
#for runNumber in `cat run_list_11_all.txt`
#for runNumber in `cat run_list_11_test.txt`
#for runNumber in `cat run_list_11_all_reduced2_test.txt`
#for runNumber in `cat run_list_11_all_reduced2.txt`
for runNumber in `cat runlists/run11_test_short`
do
   echo submitting job for runNumber = $runNumber
   star-submit-template -template scripts/run11_job_template.xml -entities  n1=$mxEve,outPath=$outPath,codePath=$codePath,inpPath=$inpPath,runNumber=$runNumber,STAR_VER=$STAR_VER
done
