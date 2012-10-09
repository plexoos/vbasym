#!/bin/sh
mxEve=500000
codePath=/star/u/smirnovd/wana_head/
#inpPath=/star/u/smirnovd/lists
#inpPath=/star/data05/scratch/smirnovd/lists
#outPath=/star/data05/scratch/smirnovd/
inpPath=/star/u/smirnovd/wana_test/lists
outPath=/star/u/smirnovd/wana_test/
STAR_VER=SL11d

export $STAR_VER

echo outPath  = $outPath
echo inpPath  = $inpPath
echo STAR_VER = $STAR_VER

#for runNumber in `cat run_list_11_nfs.txt`
#for runNumber in `cat run_list_11_all.txt`
#for runNumber in `cat run_list_11_test.txt`
for runNumber in `cat runlists/run11_test_short`
#for runNumber in `cat run_list_11_all_reduced2_test.txt`
#for runNumber in `cat run_list_11_all_reduced2.txt`
do
   echo submitting job for runNumber = $runNumber
   star-submit-template -template scripts/run11_job_template.xml -entities  n1=$mxEve,outPath=$outPath,codePath=$codePath,inpPath=$inpPath,runNumber=$runNumber,STAR_VER=$STAR_VER
done
 
