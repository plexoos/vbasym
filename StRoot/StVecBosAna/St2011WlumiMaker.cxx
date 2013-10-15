#include "StVecBosMaker.h"

#include "St2011WlumiMaker.h"


ClassImp(St2011WlumiMaker)


St2011WlumiMaker::St2011WlumiMaker(const char *name): StMaker(name)
{
   wMK   = 0;
   muMK  = 0;
   HList = 0;
}


Int_t St2011WlumiMaker::Init()
{
   assert(wMK);
   assert(muMK);
   assert(HList);
   initHistos();
   return StMaker::Init();
}


Int_t St2011WlumiMaker::InitRun  (int runumber)
{
   towerInfoIsCurrent = false; // make sure we check the tower info on the first event.
   nActiveTowers = 0;
   for (int i = 0; i < 16; i++)  nBHT3[i] = 0;
   nBHT3_software_L0 = 0;
   nBHT3_hardware_L0 = 0;
   for (int i = 0; i < 120; i++)
      for (int j = 0; j < 16; j++)
         nBx[j][i] = 0;

   return 0;
}


Int_t St2011WlumiMaker::FinishRun(int runnumber)
{
   printf("Finishing Run %d (lumi)\n", runnumber);

   char runName[9];
   sprintf(runName, "%d", runnumber);

   float activeFraction = nActiveTowers * 1.0 / 4800.0; //portion of the detector that was working.
   //float effective_lumi;//pb^-1, effective integrated luminosity, given not all the detector is necessarily working.
   //float total_lumi;//pb^-1, total integrated luminosity if the whole detector were working.
   int BHT3prescale = 50; //recorded 1/n events:
   //int BHT3prescale=1;//recorded 1/n events: // S. Fazio 15 May 2013
   int nBHT3triggers;//number of non-background BHT3 events roughly (nBHT3*BHT3prescale);
   float BHT3xs = 520000; //pb . 520nb.
   int nAbortGap1, nAbortGap2; //number of counts in the first and second abort gap.

   //try all possible alignments of the abort gaps to find the right relative alignment,
   //then count the events in those two gaps.
   getAbortGapCounts(0, &nAbortGap1, &nAbortGap2);


   //hA[19]->Fill(runName,nBHT3_software_L0);
   //hA[20]->Fill(runName,nBHT3_hardware_L0);
   //hA[21]->Fill(runName,nAbortGap1);
   //hA[22]->Fill(runName,nAbortGap2);

   nBHT3triggers = (nBHT3_software_L0 - nAbortGap1 * 111.0 / 11.0 - nAbortGap2 * 109.0 / 9.0) * BHT3prescale;

   //hA[23]->Fill(runName,nBHT3triggers);

   effective_lumi = nBHT3triggers * 1.0 / BHT3xs;

   //VecBosEvent *eve=wMK->mVecBosEvent;
   //(eve->mLumiEff) = effective_lumi;

   //hA[24]->Fill(runName,effective_lumi);

   //this value doesn't have any factors of activeFraction, because they cancel out:
   //ntotaltriggers=nBHT3triggers/activeFraction
   //total_lumi=ntotaltriggers/bht3xs
   //effective_lumi=total_lumi*activeFraction

   total_lumi = effective_lumi / activeFraction;
   //hA[16]->Fill(runName,total_lumi);
   //hA[17]->Fill(runName,activeFraction);
   //hA[18]->Fill(runName,nAbortGap1*120/11+nAbortGap2*120/9);
   printf("eff_lumi (pb-1)=%f, activeFraction=%2.2f, nActiveTowers=%d\n", effective_lumi, activeFraction, nActiveTowers);
   printf("total_lumi (pb-1)=%f \n", effective_lumi);

   /*
   for (int i=0;i<16;i++)
     {
       //printf("nBHT3[%d]=%d\n",i,nBHT3[i]);
       hA[25]->Fill(i,nBHT3[i]);
       hA[30+i]->Fill(runName,nBHT3[i]);
       getAbortGapCounts(i,&nAbortGap1,&nAbortGap2);
       hA[50+i]->Fill(runName,nAbortGap1);
       hA[70+i]->Fill(runName,nAbortGap2);
     }
   */

   //TH1F *temp;
   //temp=(TH1F*)(HList->FindObject("muWET"));
   //hA[5]->Add(temp,1.0/activeFraction);//yield scaled by the active fraction


   return 0;
}


Int_t St2011WlumiMaker::Make()
{
   //printf("in %s\n", GetName());
   //hA[0]->Fill("",1.);

   //if we haven't done it yet, calculate the working fraction of the bemc
   if (!towerInfoIsCurrent) getActiveTowers();

   //fill various histograms and arrays with event data
   sortTrigger();

   //  printf("Out of Make\n");
   return kStOK;
}


void St2011WlumiMaker::getActiveTowers()
{
   //count the number of good towers:
   nActiveTowers = 0;
   WeveBEMC *barrel = &(wMK->mVecBosEvent->bemc);
   for (int i = 0; i < 4800; i++)
      if (barrel->statTile[0][i] == 0) //[0]=kBtow
         nActiveTowers++;

   //count good trigger patches?

   if (nActiveTowers > 0)   towerInfoIsCurrent = true;
}


void St2011WlumiMaker::sortTrigger()
{
   int thresh[16];
   thresh[0]  = -1;
   thresh[1]  = 14;
   thresh[2]  = 36;
   thresh[3]  = 56;
   thresh[4]  = 78;
   thresh[5]  = 98;
   thresh[6]  = 119;
   thresh[7]  = 139;
   thresh[8]  = 161;
   thresh[9]  = 181;
   thresh[10] = 201;
   thresh[11] = 222;
   thresh[12] = 243;
   thresh[13] = 263;
   thresh[14] = 283;
   thresh[15] = 306;

   //printf("sortTrigger()\n");
   //has access to whole W-algo-maker data via pointer 'wMK'
   VecBosEvent *weve = wMK->mVecBosEvent;

   if (weve->l2bitET) {
      //printf("ET\n");
      hA[0]->Fill("L2W", 1);
      hA[0]->Fill("L2Wnormal", 1);
   }

   //if (weve->l2bitRnd) {
   //printf("Rnd\n");
   hA[0]->Fill("L2W", 1);
   hA[0]->Fill("L2Wrandom", 1);
   nBHT3_hardware_L0++;
   if (wMK->passes_L0()) {
      nBHT3_software_L0++;
      // hA[1]->Fill(weve->bx7);
      //printf("passes_L0\n");
      for (int i = 0; i < 16; i++) {
         //printf("AwaySum[%d]=%d\n",i,weve->trigAwaySum[i]);
         if (weve->trigAwaySum[i] > thresh[i]) { //for bg test
            //printf("Passes coin(%d)!\n",i);
            nBHT3[i]++;
            //printf("Passes coin(%d)(%d)\n",i,nBHT3[i]);
            nBx[i][weve->bx7]++;
         }
      }
   }
   //}
   //printf("out of sort\n");
}


void St2011WlumiMaker::getAbortGapCounts(int angle, int *n1, int *n2)
{
   //new, simple method:  count only over the last 8 bins of the gap.
   *n1 = 0;
   for (int i = 32; i <= 39; i++)
      *n1 += nBx[angle][i];
   *n2 = 0;
   for (int i = 112; i <= 119; i++)
      *n2 += nBx[angle][i];
}
