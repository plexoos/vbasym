#include <StMuDSTMaker/COMMON/StMuDstMaker.h>
#include <StMuDSTMaker/COMMON/StMuDst.h>
#include <StMuDSTMaker/COMMON/StMuTriggerIdCollection.h>
#include <StMuDSTMaker/COMMON/StMuEvent.h>
#include <StMuDSTMaker/COMMON/StMuTrack.h>
#include <StMuDSTMaker/COMMON/StMuPrimaryVertex.h>

#include "StEmcRawMaker/defines.h"
#include "StEmcUtil/database/StBemcTables.h"

#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"

#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"

#include "StVecBosMaker.h"

// returns non-zero on abort
int StVecBosMaker::accessBarrelTrig()
{ //{{{
   if (isMC) {
      /*
        When the trigger emulator is ready, this should hook into that
        instead of the two functions used below.  For now, check that is passes both
        L0 and L2, and set the l2bitET flag to true if so.
      */

      //if (!passes_L0()) return -1;
      if (!passes_L2()) return -2;

      hA[0]->Fill("L2bwET", 1.);

      mWEvent->l2bitET = true;
      return 0; // we haven't set everything, but it should be good enough for simu.
   }

   StMuEvent *muEve = mStMuDstMaker->muDst()->event();

   //collect info for the luminosity monitor
   int highestT = 0;
   int highestM = 0;

   for (int m = 0; m < 300; m++) {
      int myT = muEve->emcTriggerDetector().highTower(m);

      if  (myT > highestT) {
         highestT = myT;
         highestM = m;
      }
   }

   int highestPhi, tempPhi, tempEta;
   int awaySum[16];
   int totalSum = 0;

   for (int i = 0; i < 16; i++) awaySum[i] = 0;

   patchToEtaPhi(highestM, &tempEta, &highestPhi);

   for (int m = 0; m < 300; m++) {
      int myT = muEve->emcTriggerDetector().highTower(m);
      patchToEtaPhi(m, &tempEta, &tempPhi);

      for (int away_width = 0; away_width < 16; away_width++)
         if ((highestPhi + 30 - tempPhi) % 30 > (15 - away_width) && (highestPhi + 30 - tempPhi) % 30 < (15 + away_width)) {
            //printf("==> adding %d to awaySum",myT);
            awaySum[away_width] += myT;
         }

      totalSum += myT;
   }

   for (int i = 0; i < 16; i++)  mWEvent->trigAwaySum[i] = awaySum[i];

   mWEvent->trigTotalSum = totalSum;

   StMuTriggerIdCollection *tic = &(muEve->triggerIdCollection());
   assert(tic);
   const StTriggerId &l1 = tic->l1();
   vector<unsigned int> idL = l1.triggerIds();

   //printf("nTrig=%d, trigID: ",idL.size());
   for (unsigned int i = 0; i < idL.size(); i++) {
      char txt[100];
      sprintf(txt, "%d", idL[i]);
      //printf("%d, ",idL[i]);
      hA[1]->Fill(txt, 1.);
   }

   //printf("\n isTrg=%d trgId=%d\n",tic->nominal().isTrigger(par_l2bwTrgID),par_l2bwTrgID);

   //get bX info
   StL0Trigger *trig = &(muEve->l0Trigger());
   mWEvent->bx48 = trig->bunchCrossingId();
   mWEvent->bx7 = trig->bunchCrossingId7bit(mRunNo);

   // store spin info
   int bxStar48 = -2, bxStar7 = -2, spin4 = -2;

   if (spinDb && spinDb->isValid() && // all 3 DB records exist
         spinDb->isPolDirLong()) {  // you do not want mix Long & Trans by accident
      bxStar48 = spinDb->BXstarUsingBX48(mWEvent->bx48);
      bxStar7  = spinDb->BXstarUsingBX7(mWEvent->bx7);
      spin4    = spinDb->spin4usingBX48(mWEvent->bx48);
   }

   mWEvent->bxStar48 = bxStar48;
   mWEvent->bxStar7 = bxStar7;
   mWEvent->spin4 = spin4;

   //check trigger ID
   if (!tic->nominal().isTrigger(par_l2bwTrgID)) return -2;

   hA[0]->Fill("L2bwId", 1.);

   TArrayI &l2Array = muEve->L2Result();
   LOG_DEBUG << Form("AccessL2Decision() from regular muDst: L2Ar-size=%d", l2Array.GetSize()) << endm;
   unsigned int *l2res = (unsigned int *)l2Array.GetArray();
   const int BEMCW_off = 20; // valid only for 2009 & 2011 run
   L2wResult2009 *l2algo = ( L2wResult2009 *) &l2res[BEMCW_off];

#if 0

   if (l2algo->trigger == 0) return -3;

   printf(" L2-jet online results below:\n");

   for (int k = 0; k < 64; k++)
      if (l2res[k]) printf("k=%2d  val=0x%04x\n", k, l2res[k]);

   L2wResult2009_print(l2algo);
#endif

   mWEvent->l2bitET = (l2algo->trigger & 2) > 0; // bit1=ET>thr
   mWEvent->l2bitRnd = (l2algo->trigger & 1) > 0; // bit0=rnd,

   if ( (mWEvent->l2bitRnd || mWEvent->l2bitET) == 0) return -3; // L2W-algo did not accept this event

   hA[0]->Fill("L2bwBits", 1.); // confirmation bits were set properly

   if (mWEvent->l2bitRnd) {
      hA[0]->Fill("L2bwRnd", 1.);

      for (int m = 0; m < 300; m++) {
         int val = muEve->emcTriggerDetector().highTower(m);
         hA[7]->Fill(val);
      }

      hA[61]->Fill(mWEvent->bx7);
   }

   if (!mWEvent->l2bitET)  return -3; // drop L2W-random accepts

   if (mWEvent->l2bitET) hA[0]->Fill("L2bwET", 1.);

   //.... only monitor below ....
   hA[2]->Fill(mWEvent->bx48);
   hA[3]->Fill(mWEvent->bx7);

   // access L0-HT data
   int mxVal = -1;

   for (int m = 0; m < 300; m++)	{
      int val = muEve->emcTriggerDetector().highTower(m);

      if (mxVal < val) mxVal = val;

      if (mWEvent->l2bitET) hA[6]->Fill(val);

      if (val < par_DsmThres) continue;

      if (mWEvent->l2bitET) hA[8]->Fill(m);

      //printf("Fired L0 HT m=%d val=%d\n",m,val);
   }

   mWEvent->bemc.maxHtDsm = mxVal;
   return 0;
} //}}}


// returns non-zero on abort
int StVecBosMaker::accessVertex()
{ //{{{
   int nInpPrimV = mStMuDstMaker->muDst()->numberOfPrimaryVertices();

   if (nInpPrimV < par_minPileupVert) return -1;

   //separate histos for barrel and endcap triggers
   if (mWEvent->l2bitET)  hA[0]->Fill("tpcOn", 1.);
   if (mWEvent->l2EbitET) hE[0]->Fill("tpcOn", 1.);

   int nVer  = 0;
   int nVerR = 0;

   for (int iv = 0; iv < nInpPrimV; iv++)
   {
      StMuPrimaryVertex *V = mStMuDstMaker->muDst()->primaryVertex(iv);
      assert(V);
      mStMuDstMaker->muDst()->setVertexIndex(iv);

      float rank   = V->ranking();
      float funnyR = 999;

      if (rank > 1e6)    funnyR = log(rank - 1e6) + 10;
      else if (rank > 0) funnyR = log(rank);
      else               funnyR = log(rank + 1e6) - 10;

      if (mWEvent->l2bitET)  hA[10]->Fill(funnyR);
      if (mWEvent->l2EbitET) hE[10]->Fill(funnyR);

      //keep some neg. rank vertices for endcap if matched to ETOW
      if (rank <= 0 && V->nEEMCMatch() <= 0) continue;

      const StThreeVectorF &r = V->position();

      // StThreeVectorF &er=V->posError();
      if (mWEvent->l2bitET && rank > 0) hA[11]->Fill(r.z());

      if (mWEvent->l2EbitET) hE[11]->Fill(r.z());

      nVer++; // count valid vertices

      if (fabs(r.z()) > par_vertexZ) continue;

      if (rank > 0) nVerR++; //count vertices with rank>0

      WEventVertex wv;
      wv.id = iv;
      wv.z = r.z();
      wv.rank = rank;
      wv.funnyRank = funnyR;
      wv.nEEMCMatch = V->nEEMCMatch();
      mWEvent->vertex.push_back(wv);
   }

   if (nVer <= 0) return -2;

   if (mWEvent->l2bitET && nVerR > 0) {
      hA[0]->Fill("primVert", 1.);
      hA[4]->Fill(mWEvent->bx48);
      hA[5]->Fill(mWEvent->bx7);
   }

   if (mWEvent->l2EbitET) {
      hE[0]->Fill("primVert", 1.);
      hE[4]->Fill(mWEvent->bx48);
      hE[5]->Fill(mWEvent->bx7);
   }

   // access L0-HT data
   StMuEvent *muEve = mStMuDstMaker->muDst()->event();

   for (int m = 0; m < 300; m++)	{
      int val = muEve->emcTriggerDetector().highTower(m);

      if (val < par_DsmThres) continue;

      if (mWEvent->l2bitET && nVerR > 0) hA[9]->Fill(m);
   }

   for (int m = 0; m < 90; m++)	{
      int val = muEve->emcTriggerDetector().highTowerEndcap(m);

      if (val < parE_DsmThres) continue;

      if (mWEvent->l2EbitET) hE[9]->Fill(m);
   }

   if (mWEvent->l2bitET) hA[12]->Fill(nVerR);

   if (mWEvent->l2EbitET) hE[12]->Fill(nVer);

   if (mWEvent->vertex.size() <= 0) return -3;

   if (mWEvent->l2bitET && nVerR > 0) hA[0]->Fill("vertZ", 1.);

   if (mWEvent->l2EbitET) hE[0]->Fill("vertZ", 1.);

   return 0;
} //}}}



int StVecBosMaker::accessTracks()  // return non-zero on abort
{ //{{{
   int nTrOK = 0;

   // printf("\n nInp=%d eveID=%d nPVer=%d nAnyV= %d\n",nInpEve,mStMuDstMaker->muDst()->event()->eventId(),mWEvent->vertex.size(),mStMuDstMaker->muDst()->numberOfPrimaryVertices());
   for (uint iv = 0; iv < mWEvent->vertex.size(); iv++) {
      uint vertID = mWEvent->vertex[iv].id;
      assert(vertID < mStMuDstMaker->muDst()->numberOfPrimaryVertices());
      assert(vertID >= 0);
      StMuPrimaryVertex *V = mStMuDstMaker->muDst()->primaryVertex(vertID);
      assert(V);
      mStMuDstMaker->muDst()->setVertexIndex(vertID);
      float rank = V->ranking();
      assert(rank > 0 || (rank < 0 && V->nEEMCMatch()));
      Int_t nPrimTrAll = mStMuDstMaker->muDst()->GetNPrimaryTrack();

      for (int itr = 0; itr < nPrimTrAll; itr++) {
         StMuTrack *prTr = mStMuDstMaker->muDst()->primaryTracks(itr);

         if (prTr->flag() <= 0) continue;

         const StMuTrack *glTr = prTr->globalTrack();

         if (glTr == 0) continue; // see the reason at the end of this method

         //keep list of all tracks for TPC cone sum in tree ana
         mWEvent->vertex[iv].prTrList.push_back(prTr);
         StThreeVectorF ro = glTr->lastPoint();

         // TPC+prim vertex tracks and short EEMC tracks
         if (prTr->flag() != 301 && prTr->flag() != 311) continue;

         if (mWEvent->l2bitET && rank > 0 && prTr->flag() == 301)
            hA[20]->Fill("flag", 1.);

         if (mWEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)
            hE[20]->Fill("flag", 1.);

         float pt = prTr->pt();

         if (pt < 1.0) continue;

         if (mWEvent->l2bitET && rank > 0 && prTr->flag() == 301)
            hA[20]->Fill("pt1", 1.);

         if (mWEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)
            hE[20]->Fill("pt1", 1.);

         //accepted tracks......
         float hitFrac = 1.*prTr->nHitsFit() / prTr->nHitsPoss();
         StThreeVectorF ri = glTr->firstPoint();
         /* Victor: in reality mChiSqXY is a normal Xi2 for track and mChiSqZ is Xi2 of fit to  primary vertex   */
         float globChi2dof = glTr->chi2();
         float dedx = prTr->dEdx() * 1e6;

         //barrel algo track monitors
         if (mWEvent->l2bitET && rank > 0 && prTr->flag() == 301) {
            hA[21]->Fill(prTr->nHitsFit());
            hA[22]->Fill(hitFrac);
            hA[23]->Fill(ri.perp());
            hA[24]->Fill(ro.perp());

            //TPC sector dependent filter
            int secID = WtpcFilter::getTpcSec(ro.phi(), ro.pseudoRapidity());

            if (mTpcFilter[secID - 1].accept(prTr) == false) continue;

            if (secID == 20) continue; //poorly calibrated sector for Run 9+11+12?

            hA[25]->Fill(glTr->p().perp());

            if (glTr->charge() < 0) hA[27]->Fill(glTr->p().perp());

            hA[29]->Fill(pt);

            if (prTr->charge() < 0)hA[30]->Fill(pt);

            hA[26]->Fill(ro.pseudoRapidity(), ro.phi());

            if (pt > 5) //estimate TPC inefficiency in data
               hA[57]->Fill(ro.pseudoRapidity(), ro.phi());

            hA[35]->Fill(globChi2dof);

            // monitor chi2 for east/west TPC separately
            if (ri.z() > 0 && ro.z() > 0)  hA[58]->Fill(globChi2dof);

            if (ri.z() < 0 && ro.z() < 0)  hA[59]->Fill(globChi2dof);

            hA[36]->Fill(globChi2dof, ro.pseudoRapidity());
            hA[28]->Fill(prTr->p().mag(), dedx);

            if (pt > 10)
               hA[197]->Fill(ro.pseudoRapidity(), ro.phi());

            hA[198]->Fill(ro.pseudoRapidity(), prTr->pt());
         }

         //endcap algo track monitors
         if (mWEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin) {
            hE[20]->Fill("#eta>0.7", 1.);
            hE[21]->Fill(prTr->nHitsFit());
            hE[22]->Fill(hitFrac);
            hE[23]->Fill(ri.perp());
            hE[24]->Fill(ro.perp());

            // TPC sector dependent filter
            int secID = WtpcFilter::getTpcSec(ro.phi(), ro.pseudoRapidity());

            if ( mTpcFilterE[secID - 1].accept(prTr) == false) continue;

            hE[25]->Fill(glTr->p().perp());

            if (glTr->charge() < 0)hE[27]->Fill(glTr->p().perp());

            hE[29]->Fill(pt);

            if (prTr->charge() < 0)hE[30]->Fill(pt);

            hE[26]->Fill(ro.pseudoRapidity(), ro.phi());

            if (pt > 5) //estimate TPC inefficiency in data
               hE[57]->Fill(ro.pseudoRapidity(), ro.phi());

            hE[35]->Fill(globChi2dof);
            hE[36]->Fill(globChi2dof, ro.pseudoRapidity());
            hE[28]->Fill(prTr->p().mag(), dedx);
         }


         bool barrelTrack = (mWEvent->l2bitET && rank > 0 && prTr->flag() == 301 && pt > par_trackPt);

         if (barrelTrack) hA[20]->Fill("ptOK", 1.); //good barrel candidate

         bool endcapTrack = (mWEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin && pt > parE_trackPt);

         if (endcapTrack) hE[20]->Fill("ptOK", 1.); //good endcap candidate

         if (!barrelTrack && !endcapTrack) continue;

         //keep all tracks in one container
         nTrOK++;
         WeveEleTrack wTr;

         wTr.prMuTrack = prTr;
         wTr.glMuTrack = glTr;
         StThreeVectorF prPvect = prTr->p();
         wTr.primP = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

         mWEvent->vertex[iv].eleTrack.push_back(wTr);
      }// loop over tracks
   }// loop over vertices

   if (nTrOK <= 0) return -1;

   if (mWEvent->l2bitET) hA[0]->Fill("Pt10", 1.);

   if (mWEvent->l2EbitET) hE[0]->Fill("Pt10", 1.);

   return 0;
} //}}}

/* from Pibero:
   It looks like your global track is null. See this post:

   http://www.star.bnl.gov/HyperNews-star/get/mudst/53.html

   My reading of this hypernews says its just the way ITTF/MuDst
   works. You can get a good primary track, but its global track
   fails the chi2 fit. So the primary track is kept in the MuDst
   but the global track is dropped. I would suggest you skip those
   rare primary tracks that have no global tracks, that way you
   still use most of the tracks in the MuDst. You don't need to
   skip the entire event, just that track. I guess the down side
   is you couldn't make a global DCA cut on those rare tracks, right?
   I guess you could also request S&C to change ITTF/MuDst not to drop
   the global track for every good primary track regardless of chi2.
*/



/* $STAR/StRoot/StEvent/StTrack.h
 *  mFlag=zxyy, where  z = 1 for pile up track in TPC (otherwise 0)
 *                     x indicates the detectors included in the fit and
 *                    yy indicates the status of the fit.
 *  Positive mFlag values are good fits, negative values are bad fits.
 *
 *  The first digit indicates which detectors were used in the refit:
 *
 *      x=1 -> TPC only
 *      x=3 -> TPC       + primary vertex
 *      x=5 -> SVT + TPC
 *      x=6 -> SVT + TPC + primary vertex
 *      x=7 -> FTPC only
 *      x=8 -> FTPC      + primary
 *      x=9 -> TPC beam background tracks
 *
 *  The last two digits indicate the status of the refit:
 *       = +x01 -> good track
 *
 *      = -x01 -> Bad fit, outlier removal eliminated too many points
 *      = -x02 -> Bad fit, not enough points to fit
 *      = -x03 -> Bad fit, too many fit iterations
 *      = -x04 -> Bad Fit, too many outlier removal iterations
 *      = -x06 -> Bad fit, outlier could not be identified
 *      = -x10 -> Bad fit, not enough points to start
 *
 *      = +x11 -> Short track pointing to EEMC
*/


//
int StVecBosMaker::accessBTOW()
{ //{{{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      gMessMgr->Warning() << "No EMC data for this event" << endm;    return -4;
   }

   int ibp = kBTow; // my index for tower & preshower set to BTOW
   int jBP = BTOW; // official BTOW detector ID
   int n5 = 0, n0 = 0, n1 = 0, n2 = 0, n3 = 0, n4 = 0;
   int maxID = 0;
   double maxADC = 0, adcSum = 0;

   for (int softID = 1; softID <= mxBtow ; softID++)
   {
      float rawAdc = emc->getTowerADC(softID);

      if (rawAdc == 0) n0++;

      int statPed, statOfl, statGain;
      mBarrelTables->getStatus(jBP, softID, statPed, "pedestal");
      mBarrelTables->getStatus(jBP, softID, statOfl);
      mBarrelTables->getStatus(jBP, softID, statGain, "calib");

      if (statPed != 1) {
         mWEvent->bemc.statTile[ibp][softID - 1] = 1;
         n1++; continue;
      }

      if (statOfl != 1) {
         mWEvent->bemc.statTile[ibp][softID - 1] = 2;
         n2++; continue;
      }

      if (statGain != 1) {
         mWEvent->bemc.statTile[ibp][softID - 1] = 4;
         n3++; continue;
      }

      mWEvent->bemc.statTile[ibp][softID - 1] = 0 ;

      float ped, sigPed, gain;
      int capID = 0; // just one value for btow

      mBarrelTables->getPedestal(jBP, softID, capID, ped, sigPed);
      mBarrelTables->getCalib(jBP, softID, 1, gain);

      if (use_gains_file == 1) {
         gain = gains_BTOW[softID];
      }

      //printf("id=%d gain=%f\n",softID,gain);

      //method for shifting energy scale
      gain = gain * par_btowScale; //(default is par_btowScale=1)

      float adc = rawAdc - ped;

      if (adc > 0) n4++;
      if (adc < par_kSigPed * sigPed) continue;
      if (adc < par_AdcThres)         continue;

      n5++;
      mWEvent->bemc.adcTile[ibp][softID - 1] = adc;
      mWEvent->bemc.eneTile[ibp][softID - 1] = adc * gain;

      if (maxADC < adc) { maxID = softID; maxADC = adc;}

      adcSum += adc;
   }

   //printf("NNN %d %d %d %d %d %d id=%d\n",n0,n1,n2,n3,n4,n5,maxID);
   if (n0 == mxBtow) return -1 ; // BTOW was not present in this events

   mWEvent->bemc.tileIn[ibp] = 1; //tag usable data

   if (nInpEve % 5000 == 1) {
      LOG_INFO << Form("unpackMuBTOW() dataIn=%d, nBbad: ped=%d stat=%d gain=%d ; nAdc: %d>0, %d>thres\n    maxADC=%.0f softID=%d adcSum=%.0f",
                       mWEvent->bemc.tileIn[ibp], n1, n2, n3, n4, n5,
                       maxADC, maxID, adcSum
                      ) << endm;
   }

   hA[31]->Fill(maxADC);
   hA[32]->Fill(adcSum);

   mWEvent->bemc.maxAdc = maxADC;

   if (maxID <= 2400) hA[195]->Fill(maxADC);
   else hA[196]->Fill(maxADC);

   if (maxADC < par_maxADC)  return -2 ; // not enough energy

   return 0;
} //}}}


void StVecBosMaker::fillTowHit(bool vert)
{ //{{{
   if (!mWEvent->l2bitET) return; //only barrel triggers

   //find highest rank vertex
   float maxRank = 0; uint maxRankId = 0;

   for (uint iv = 0; iv < mWEvent->vertex.size(); iv++) {
      float rank = mWEvent->vertex[iv].rank;

      if (rank < 0) continue;

      if (rank > maxRank) {
         maxRank = rank;
         maxRankId = iv;
      }
   }

   int bx7 = mWEvent->bx7; int bxBin = -1;

   if (bx7 >= 0 && bx7 < 30)
      bxBin = 0;
   else if (bx7 < 40)
      bxBin = 1;
   else if (bx7 < 110)
      bxBin = 2;
   else if (bx7 < 120)
      bxBin = 3;

   float Rcylinder = mBtowGeom->Radius(), Rcylinder2 = Rcylinder * Rcylinder;

   //loop barrel towers and fill histo
   for (int i = 0; i < mxBtow; i++) {
      float adc = mWEvent->bemc.adcTile[kBTow][i];
      bool fillAdc = false;

      if (adc > 10) fillAdc = true; //~150 MeV threshold for tower firing

      if (vert) {
         if (fillAdc) hA[215 + bxBin]->Fill(positionBtow[i].Eta(), positionBtow[i].Phi());

         float ene = mWEvent->bemc.eneTile[kBTow][i];
         float delZ = positionBtow[i].z() - mWEvent->vertex[maxRankId].z;
         float e2et = Rcylinder / sqrt(Rcylinder2 + delZ * delZ);
         float ET = ene * e2et;

         if (ET > 2.0) hA[219 + bxBin]->Fill(positionBtow[i].Eta(), positionBtow[i].Phi());
      }
      else if (fillAdc) hA[223 + bxBin]->Fill(positionBtow[i].Eta(), positionBtow[i].Phi());
   }

   //some lower threshold plots
   for (int isec = 0; isec < mxEtowSec; isec++) {
      for (int isub = 0; isub < mxEtowSub; isub++) {
         for (int ieta = 0; ieta < mxEtowEta; ieta++) {
            int iPhi = isec * mxEtowSub + isub;
            float adc = mWEvent->etow.adc[iPhi][ieta];
            bool fillAdc = false;

            if (adc > 10) fillAdc = true; //~150 MeV threshold for tower firing

            if (vert) {
               if (fillAdc) hA[227 + bxBin]->Fill(ieta, iPhi);

               float ene = mWEvent->etow.ene[iPhi][ieta];
               float delZ = positionEtow[iPhi][ieta].z() - mWEvent->vertex[maxRankId].z;
               float Rxy = positionEtow[iPhi][ieta].Perp();
               float e2et = Rxy / sqrt(Rxy * Rxy + delZ * delZ);
               float ET = ene * e2et;

               if (ET > 2.0) hA[231 + bxBin]->Fill(ieta, iPhi);
            }
            else if (fillAdc) hA[235 + bxBin]->Fill(ieta, iPhi);
         }
      }
   }
} //}}}


float StVecBosMaker::sumTpcCone(int vertID, TVector3 refAxis, int flag, int pointTowId)
{ //{{{
   // flag=2 use 2D cut, 1= only delta phi

   // printf("******* sumTpcCone, flag=%d eveId=%d vertID=%d  eta0=%.2f phi0/rad=%.2f  \n",flag,mWEvent->id,vertID,refAxis.PseudoRapidity() ,refAxis.Phi());

   assert(vertID >= 0);
   assert(vertID < (int) mStMuDstMaker->muDst()->numberOfPrimaryVertices());

   StMuPrimaryVertex *V = mStMuDstMaker->muDst()->primaryVertex(vertID);
   assert(V);
   mStMuDstMaker->muDst()->setVertexIndex(vertID);
   float rank = V->ranking();
   assert(rank > 0 || (rank < 0 && V->nEEMCMatch()));
   double ptSum = 0;
   Int_t nPrimTrAll = mStMuDstMaker->muDst()->GetNPrimaryTrack();

   for (int itr = 0; itr < nPrimTrAll; itr++) {
      StMuTrack *prTr = mStMuDstMaker->muDst()->primaryTracks(itr);

      if (prTr->flag() <= 0) continue;

      if (prTr->flag() != 301 && pointTowId > 0) continue; // TPC-only regular tracks for barrel candidate

      if (prTr->flag() != 301 && prTr->flag() != 311 && pointTowId < 0) continue; // TPC regular and short EEMC tracks for endcap candidate

      float hitFrac = 1.*prTr->nHitsFit() / prTr->nHitsPoss();

      if (hitFrac < par_nHitFrac) continue;

      StThreeVectorF prPvect = prTr->p();
      TVector3 primP = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

      // printf(" prTrID=%4d  prTrEta=%.3f prTrPhi/deg=%.1f prPT=%.1f  nFitPts=%d\n", prTr->id(),prTr->eta(),prTr->phi()/3.1416*180.,prTr->pt(),prTr->nHitsFit());
      if (flag == 1) {
         float deltaPhi = refAxis.DeltaPhi(primP);

         if (fabs(deltaPhi) > par_awayDeltaPhi) continue;
      }

      if (flag == 2) {
         float deltaR = refAxis.DeltaR(primP);

         if (deltaR > par_nearDeltaR) continue;
      }

      float pT = prTr->pt();
      //    printf(" passed pt=%.1f\n",pT);

      //separate quench for barrel and endcap candidates
      if (pT > par_trackPt && pointTowId > 0) ptSum += par_trackPt;
      else if (pT > parE_trackPt && pointTowId < 0) ptSum += parE_trackPt;
      else  ptSum += pT;
   }

   return ptSum;
} //}}}



void StVecBosMaker::accessBSMD()
{ //{{{
   const char cPlane[mxBSmd] = {'E', 'P'};

   // Access to muDst
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      gMessMgr->Warning() << "No EMC data for this muDst event" << endm;    return;
   }

   // BSMD
   for (int iEP = bsmde; iEP <= bsmdp; iEP++)
   { // official BSMD plane IDs
      int iep = iEP - 3;
      assert(bsmde == 3); // what a hack
      int nh = emc->getNSmdHits(iEP);
      //printf("muDst BSMD-%c nHit=%d\n",cPlane[iep],nh);
      int n5 = 0, n1 = 0, n2 = 0, n3 = 0, n4 = 0;

      for (int i = 0; i < nh; i++) {
         StMuEmcHit *hit = emc->getSmdHit(i, iEP);
         float  adc = hit->getAdc();
         int softID = hit->getId();

         int statPed, statOfl, statGain;
         mBarrelTables->getStatus(iEP, softID, statPed, "pedestal");
         mBarrelTables->getStatus(iEP, softID, statOfl);
         mBarrelTables->getStatus(iEP, softID, statGain, "calib");

         if (statPed != 1) {
            mWEvent->bemc.statBsmd[iep][softID - 1] = 1;
            n1++; continue;
         }

         if (statOfl != 1) {
            mWEvent->bemc.statBsmd[iep][softID - 1] = 2;
            n2++; continue;
         }

         if (statGain < 1 || statGain > 19) {
            mWEvent->bemc.statBsmd[iep][softID - 1] = 4;
            n3++; continue;
         }

         float pedRes, sigPed, gain;
         int capID = 0; // just one value for ped residua in pp500, 2009 run

         mBarrelTables->getPedestal(iEP, softID, capID, pedRes, sigPed);
         mBarrelTables->getCalib(iEP, softID, 1, gain);

         if (isMC) { // overwrite it based on genat DE & private calibration
            float par_bsmdAbsGain = 6e6; // tmp arbitrary absolute calib of bsmd, was 3e6
            float  de = hit->getEnergy();// Geant energy deposit (GeV)
            adc = de * par_bsmdAbsGain;
         }
         else {   // correct for pedestal residua
            adc -= pedRes;

            if (adc > 0) n4++;
            if (adc < par_kSigPed * sigPed) continue;
         }

         n5++;
         assert(softID >= 1);      assert(softID <= mxBStrips);
         int id0 = softID - 1;
         mWEvent->bemc.adcBsmd[ iep][id0] = adc;
         hA[70 + 10 * iep]->Fill(adc);

         //if(nInpEve<3 || i <20 )printf("  i=%d, smd%c id=%d, m=%d adc=%.3f pedRes=%.1f, sigP=%.1f stat: O=%d P=%d G=%d  gain=%.2f\n",i,cPlane[iep],softID,1+id0/150,adc,pedRes,sigPed, statOfl,statPed,statGain, gain);
      }// end of hit list

      if (nTrigEve % 5000 == 1) {
         LOG_INFO << Form("unpackMuBSMD-%c() nBbad: ped=%d stat=%d gain=%d ; nAdc: %d>0, %d>thres", cPlane[iep], n1, n2, n3, n4, n5) << endm;
      }
   } // end of E-, P-plane loop
} //}}}
