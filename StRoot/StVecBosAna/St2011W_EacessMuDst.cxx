//MuDst
#include <StMuDSTMaker/COMMON/StMuDstMaker.h>
#include <StMuDSTMaker/COMMON/StMuDst.h>
#include <StMuDSTMaker/COMMON/StMuTriggerIdCollection.h>
#include <StMuDSTMaker/COMMON/StMuEvent.h>
#include <StMuDSTMaker/COMMON/StMuTrack.h>
#include <StMuDSTMaker/COMMON/StMuPrimaryVertex.h>

#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"

#include "StVecBosMaker.h"


//--------------------------------------
int StVecBosMaker::accessEndcapTrig()  // return non-zero on abort
{
   if (isMC) {
      if (mWEvent->etow.maxAdc < 10. / 60.*4096) return -1; //L2 is HT
      hE[0]->Fill("L2ewET", 1.);
      mWEvent->l2EbitET = true;
      return 0;
   }

   StMuEvent *muEve = mMuDstMaker->muDst()->event();
   StMuTriggerIdCollection *tic = &(muEve->triggerIdCollection());
   assert(tic);
   const StTriggerId &l1 = tic->l1();
   vector<unsigned int> idL = l1.triggerIds();

   //printf("nTrig=%d, trigID: ",idL.size());
   for (unsigned int i = 0; i < idL.size(); i++) {
      char txt[100];
      sprintf(txt, "%d", idL[i]);
      hE[1]->Fill(txt, 1.);
   }

   //check trigger ID
   if (!tic->nominal().isTrigger(parE_l2ewTrgID)) return -2;
   hE[0]->Fill("L2ewId", 1.);

   //need to get offset for 2011 run for EEMC, done, hacked by Jan
   struct  L2weResult2011 {
      unsigned char  trigger;     // bit0=rnd, bit1=ET>thr
      unsigned char  highestEt;   // cluster Et with 60Gev Max.  bits=Et*256/60
      unsigned short highestRDO;
   };

   TArrayI &l2Array = muEve->L2Result();
   LOG_DEBUG << Form("AccessL2Decision() from regular muDst: L2Ar-size=%d", l2Array.GetSize()) << endm;
   unsigned int *l2res = (unsigned int *)l2Array.GetArray();
   const int EEMCW_off = 35; // valid only for 2011 run

   L2weResult2011 *l2algo = ( L2weResult2011 *) &l2res[EEMCW_off];
   mWEvent->l2EbitET = (l2algo->trigger & 2) > 0; // bit1=ET>thr
   mWEvent->l2EbitRnd = (l2algo->trigger & 1) > 0; // bit0=rnd,

#if 0
   if (l2algo->trigger == 0) return -3;
   printf(" L2-jet online results below:\n");
   for (int k = 0; k < 64; k++)
      if (l2res[k]) printf("k=%2d  val=0x%04x\n", k, l2res[k]);
   printf("L2WE_Result 4-bytes: trg bitET=%d,  bitRnd=%d, highets:  ET/GeV=%.2f,  RDO=%d  hex=0x%08x\n", mWEvent->l2EbitET, mWEvent->l2EbitRnd, l2algo->highestEt / 256.*60, l2algo->highestRDO, l2res[EEMCW_off]);
#endif

   // XXX:ds hack to make the code work also for run 9 and early run 12
   if (mRunNo < 11000111 || mRunNo > 13000000) {
      mWEvent->l2EbitET = 1;
      mWEvent->l2EbitRnd = 1;
   }

   if ( (mWEvent->l2EbitRnd || mWEvent->l2EbitET) == 0) return -3; // L2W-algo did not accept this event

   hE[0]->Fill("L2ewBits", 1.); // confirmation bits were set properly

   if (mWEvent->l2EbitRnd) {
      hE[0]->Fill("L2ewRnd", 1.);
      for (int m = 0; m < 90; m++) {
         int val = muEve->emcTriggerDetector().highTowerEndcap(m);
         hE[7]->Fill(val);
      }
      hE[61]->Fill(mWEvent->bx7);
   }

   if (!mWEvent->l2EbitET) return -3; // drop L2W-random accepts
   if (mWEvent->l2EbitET) hE[0]->Fill("L2ewET", 1.);

   // only monitor below
   hE[2]->Fill(mWEvent->bx48);
   hE[3]->Fill(mWEvent->bx7);

   // access L0-HT data
   int mxVal = -1;
   for (int m = 0; m < 90; m++)	{
      int val = muEve->emcTriggerDetector().highTowerEndcap(m);
      if (mxVal < val) mxVal = val;
      if (mWEvent->l2EbitET) hE[6]->Fill(val);
      if (val < parE_DsmThres) continue;
      if (mWEvent->l2EbitET) hE[8]->Fill(m);
      //printf("Fired L0 EHT m=%d val=%d\n",m,val);
   }

   mWEvent->etow.maxHtDsm = mxVal;
   return 0;
}


//________________________________________________
int StVecBosMaker::accessETOW()
{

   StMuEmcCollection *emc = mMuDstMaker->muDst()->muEmcCollection();
   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;    return -4;
   }

   mWEvent->etow.etowIn = 1; //tag usable ETOW data
   const char *maxIdName = 0;
   double maxADC = 0, adcSum = 0;
   int maxSec = -1, maxSub = -1, maxEta = -1;

   //loop over all towers
   for (int i = 0; i < emc->getNEndcapTowerADC(); i++) {
      int sec, eta, sub, rawAdc; //muDst  ranges:sec:1-12, sub:1-5, eta:1-12
      emc->getEndcapTowerADC(i, rawAdc, sec, sub, eta);

      const EEmcDbItem *x = mDbE->getTile(sec, 'A' + sub - 1, eta, 'T');
      assert(x); // it should never happened for muDst
      if (x->fail ) continue; // drop not working channels
      int isec = x->sec - 1;
      int isub = x->sub - 'A';
      int ieta = x->eta - 1;

      assert(isec >= 0 && isec < mxEtowSec); // check input is ok
      assert(isub >= 0 && isub < mxEtowSub);
      assert(ieta >= 0 && ieta < mxEtowEta);

      float adc = rawAdc - x->ped; // ped subtracted ADC
      if (adc < par_kSigPed * x->sigPed) continue;

      mWEvent->etow.adc[isec * mxEtowSub + isub][ieta] = adc;

      if (x->gain <= 0) continue; // drop channels w/o gains
      float ene = adc / x->gain;

      //method for shifting energy scale
      ene *= par_etowScale; //(default is par_etowScale=1)
      mWEvent->etow.ene[isec * mxEtowSub + isub][ieta] = ene;
      mWEvent->etow.stat[isec * mxEtowSub + isub][ieta] = 0;

      if (maxADC < adc) { maxIdName = x->name; maxADC = adc; maxSec = isec; maxSub = isub; maxEta = ieta;}
      adcSum += adc;

   }

   mWEvent->etow.maxAdc = maxADC;
   mWEvent->etow.maxSec = maxSec; mWEvent->etow.maxSub = maxSub; mWEvent->etow.maxEta = maxEta;
   hE[31]->Fill(maxADC);
   hE[32]->Fill(adcSum);

   if (maxADC < par_maxADC)  return -2 ; // not enough energy

   return 0;
}


//________________________________________________
void StVecBosMaker::accessEPRS()
{
   StMuEmcCollection *emc = mMuDstMaker->muDst()->muEmcCollection();
   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;
   }

   int i ;
   int pNh = emc->getNEndcapPrsHits();
   for (i = 0; i < pNh; i++) {
      int pre, sec, eta, sub; //muDst  ranges: sec:1-12, sub:1-5, eta:1-12 ,pre:1-3==>pre1/pre2/post

      StMuEmcHit *hit = emc->getEndcapPrsHit(i, sec, sub, eta, pre);
      float rawAdc = hit->getAdc();
      //Db ranges: sec=1-12,sub=A-E,eta=1-12,type=T,P-R ; slow method
      const EEmcDbItem *x = mDbE->getTile(sec, sub - 1 + 'A', eta, pre - 1 + 'P');
      assert(x); // it should never happened for muDst
      if (x->fail ) continue; // drop not working channels

      int isec = x->sec - 1;
      int isub = x->sub - 'A';
      int ieta = x->eta - 1;
      int ipre = pre - 1;
      int iphi = isec * mxEtowSub + isub;

      assert(isec >= 0 && isec < mxEtowSec); // check input is ok
      assert(isub >= 0 && isub < mxEtowSub);
      assert(ieta >= 0 && ieta < mxEtowEta);

      float adc = rawAdc - x->ped; // ped subtracted ADC
      if (adc < par_kSigPed * x->sigPed) continue;

      mWEvent->eprs.adc[iphi][ieta][ipre] = adc;

      if (x->gain <= 0) continue; // drop channels w/o gains

      mWEvent->eprs.ene[isec * mxEtowSub + isub][ieta][ipre] = adc / x->gain;
      mWEvent->eprs.stat[isec * mxEtowSub + isub][ieta][ipre] = 0;

   }
}


//________________________________________________
void StVecBosMaker::accessESMD()
{
   StMuEmcCollection *emc = mMuDstMaker->muDst()->muEmcCollection();
   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;
   }

   char uv = 'U';
   for (uv = 'U'; uv <= 'V'; uv++) {
      int sec, strip;
      int nh = emc->getNEndcapSmdHits(uv);
      int i;
      for (i = 0; i < nh; i++) {
         StMuEmcHit *hit = emc->getEndcapSmdHit(uv, i, sec, strip);
         float rawAdc = hit->getAdc();
         const EEmcDbItem *x = mDbE->getByStrip(sec, uv, strip);
         assert(x); // it should never happened for muDst

         if (x->fail ) continue; // drop broken channels
         if (x->ped < 0) continue; // drop channels without peds

         float adc = rawAdc - x->ped; // ped subtracted ADC

         float sigPed = x->sigPed;
         //  x->print(); printf("adc=%f\n",adc);
         int isec = sec - 1;
         int iuv = x->plane - 'U';
         int istr = x->strip - 1;

         assert(isec >= 0 && isec < mxEtowSec); //never trust the input
         assert(iuv >= 0 && iuv < mxEsmdPlane);
         assert(istr >= 0 && istr < mxEsmdStrip);
         if (x->gain <= 0)continue; // drop channels w/o gains
         if (adc < par_kSigPed * sigPed) continue; //drop noise

         mWEvent->esmd.adc[isec][iuv][istr] = adc;
         mWEvent->esmd.ene[isec][iuv][istr] = adc / x->gain;
      }
   }
}
