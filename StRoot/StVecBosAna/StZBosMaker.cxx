
#include "StZBosMaker.h"


ClassImp(StZBosMaker)


StZBosMaker::StZBosMaker(const char *name): StMaker(name)
{
   wMK   = 0;
   muMK  = 0;
   HList = 0;
}


Int_t StZBosMaker::Init()
{
   assert(wMK);
   assert(HList);
   initHistos();
   return StMaker::Init();
}


Int_t StZBosMaker::InitRun(int runumber)
{
   LOG_INFO << Form("::InitRun(%d) done, Z-algo params: nearTotEtFrac=%.2f,  clusterEt=%.1f GeV, delPhi12>%.2f rad, Zmass in[%.1f,%.1f]\n",
                    runumber, par_nearTotEtFracZ, par_clusterEtZ, par_delPhi12, par_minMassZ, par_maxMassZ) << endm;
   return 0;
}


Int_t StZBosMaker::FinishRun(int runnumber)
{
   return 0;
}


Int_t StZBosMaker::Make()
{

   //fill various histograms and arrays with event data
   find_Z_boson();
   findEndcap_Z_boson();

   return kStOK;
}


void StZBosMaker::printJan(VecBosTrack *T)
{
   int ibp = kBTow;
   WevePointTower poiTw = T->mMatchedTower;
   WeveCluster cl = T->mCluster2x2;
   int id = poiTw.id;
   float adc = wMK->mVecBosEvent->bemc.adcTile[ibp][id - 1];
   float frac = adc / 4096 * 60 / cl.ET;
   printf("Ztower Q=%d pointTw: id=%d ADC=%.0f  2x2ET=%.1f frac=%.2f\n", T->prMuTrack->charge(), id, adc, cl.ET, frac);
}


void StZBosMaker::findEndcap_Z_boson()
{
   VecBosEvent *mVecBosEvent = wMK->mVecBosEvent;
   // printf("========= findEndcap_Z_boson() \n");

   hA[50]->Fill("inp", 1.); hA[60]->Fill("inp", 1.);

   // search for Zs
   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++)
   {
      hA[50]->Fill("vert", 1.); hA[60]->Fill("vert", 1.);
      VecBosVertex &V = mVecBosEvent->mVertices[iv];

      // first loop over good barrel tracks
      for (uint it = 0; it < V.eleTrack.size(); it++)
      {
         VecBosTrack &TB = V.eleTrack[it];
         if (TB.mMatchedTower.id <= 0) continue; //skip endcap towers
         if (TB.isMatch2Cl == false) continue;
         assert(TB.mCluster2x2.nTower > 0); // internal logical error
         assert(TB.nearTotET > 0); // internal logical error

         //place cuts on reco track first (both barrel and endcap tracks allowed)
         float isoET1 = TB.mCluster2x2.ET / TB.mCluster4x4.ET;
         hA[51]->Fill(isoET1);
         hA[52]->Fill(TB.mCluster2x2.ET);
         hA[50]->Fill("trB", 1.); hA[60]->Fill("trB", 1.);
         if (TB.mCluster2x2.ET < par_clusterEtZ) continue;
         hA[50]->Fill("etB", 1.); hA[60]->Fill("etB", 1.);

         float fracET1 = TB.mCluster2x2.ET / TB.nearTotET;
         hA[53]->Fill(fracET1);
         if (fracET1 < par_nearTotEtFracZ) continue;
         hA[50]->Fill("conB", 1.); hA[60]->Fill("conB", 1.);

         // 1) try to find candidate track in the endcap
         for (uint it = 0; it < V.eleTrack.size(); it++)
         {
            VecBosTrack &TE = V.eleTrack[it];
            if (TE.mMatchedTower.id >= 0) continue; //skip barrel towers
            if (TE.isMatch2Cl == false) continue;
            assert(TE.mCluster2x2.nTower > 0); // internal logical error
            assert(TE.nearTotET > 0); // internal logical error

            float isoET2 = TE.mCluster2x2.ET / TE.mCluster4x4.ET;
            hA[71]->Fill(isoET2);
            hA[72]->Fill(TE.mCluster2x2.ET);
            hA[70]->Fill("trE", 1.);
            if (TE.mCluster2x2.ET < par_clusterEtZ) continue;
            hA[70]->Fill("etE", 1.);

            float fracET2 = TE.mCluster2x2.ET / TE.nearTotET;
            hA[73]->Fill(fracET2);
            if (fracET2 < par_nearTotEtFracZ) continue;
            hA[70]->Fill("conE", 1.);

            float e1 = TB.mCluster2x2.energy;
            float e2 = TE.mCluster2x2.energy;
            TVector3 p1 = TB.mVec3AtDca; p1.SetMag(e1); //cluster.position;
            TVector3 p2 = TE.mVec3AtDca; p2.SetMag(e2); //cluster.position;

            float del_phi = p1.DeltaPhi(p2);
            //printf("del Phi=%f\n",del_phi);
            float xx = del_phi;
            if (xx < -TMath::Pi() + 1) xx += 2 * TMath::Pi();
            hA[74]->Fill(xx);
            if (fabs(del_phi) < par_delPhi12) continue;
            hA[70]->Fill("phi12", 1.);

            TVector3 psum = p1 + p2;
            float mass2 = (e1 + e2) * (e1 + e2) - (psum.Dot(psum));
            if (mass2 < 1.) continue; // 9GeV^2) should be param, I'm tired today
            hA[70]->Fill("m2", 1.);
            hA[77]->Fill(p1.Eta(), p2.Eta());
            hA[78]->Fill(psum.Eta());
            hA[79]->Fill(psum.Pt());

            float mass = sqrt(mass2);
            int Q1Q2 = TB.prMuTrack->charge() * TE.prMuTrack->charge();
            if (Q1Q2 == 1) { //..  same sign , can't be Z-> e+ e-
               hA[76]->Fill(mass);
               hA[80]->Fill(TE.mCluster2x2.ET, TE.prMuTrack->charge() / TE.prMuTrack->pt());	  continue;
            }

            // now only opposite sign
            hA[70]->Fill("QQ", 1.);
            hA[75]->Fill(mass);
            hA[81]->Fill(TE.mCluster2x2.ET, TE.prMuTrack->charge() / TE.prMuTrack->pt());
         }

         // 2) use highest ET endcap mCluster2x2 with no track requirement
         float maxET = 0;
         WeveCluster maxCluster;
         for (int iEta = 0; iEta < 12; iEta++) { //loop over eta bins
            for (int iPhi = 0; iPhi < 60; iPhi++) { //loop over phi bins

               WeveCluster eclust = wMK->maxEtow2x2(iEta, iPhi, V.z);
               if (eclust.ET < par_clusterEtZ) continue;
               if (maxET > eclust.ET) continue;
               else {
                  maxET = eclust.ET;
                  maxCluster = eclust;
               }
            }
         }

         if (maxCluster.ET <= 1.0) continue; //remove low E clusters

         //apply cuts to max ETOW cluster and isolation sums
         WeveCluster mCluster4x4 = wMK->sumEtowPatch(maxCluster.iEta - 1, maxCluster.iPhi - 1, 4, 4, V.z);
         hA[54]->Fill(maxCluster.ET / mCluster4x4.ET);
         if (maxCluster.ET / mCluster4x4.ET < wMK->mMinEClusterEnergyIsoRatio) continue;
         hA[55]->Fill(maxCluster.ET);
         hA[50]->Fill("trE", 1.);
         if (maxCluster.ET < par_clusterEtZ) continue;
         hA[50]->Fill("etE", 1.);

         //assume poor tracking effic so only towers in nearCone
         float nearBtow = wMK->SumBTowCone(V.z, maxCluster.position, 2);
         float nearEtow = wMK->SumETowCone(V.z, maxCluster.position, 2);
         float nearSum = nearBtow; nearSum += nearEtow;
         hA[56]->Fill(maxCluster.ET / nearSum);
         if (maxCluster.ET / nearSum < wMK->parE_nearTotEtFrac) continue;
         hA[50]->Fill("conE", 1.);

         //add plots of good candidates
         float e1 = TB.mCluster2x2.energy;
         float e2 = maxCluster.energy;
         TVector3 p1 = TB.mVec3AtDca; p1.SetMag(e1);
         TVector3 p2 = maxCluster.position; p2.SetMag(e2);
         float del_phi = p1.DeltaPhi(p2);
         float xx = del_phi;
         if (xx < -TMath::Pi() + 1) xx += 2 * TMath::Pi();
         hA[57]->Fill(xx);
         if (fabs(del_phi) < par_delPhi12) continue;
         hA[50]->Fill("phi12", 1.);
         TVector3 psum = p1 + p2;
         float mass2 = (e1 + e2) * (e1 + e2) - (psum.Dot(psum));
         if (mass2 < 1.) continue;
         hA[50]->Fill("m2", 1.);
         float mass = sqrt(mass2);
         hA[58]->Fill(mass);
         hA[59]->Fill(p1.Eta(), p2.Eta());
         hA[60]->Fill(psum.Eta());
         hA[61]->Fill(psum.Pt());
      }
   }
}


void StZBosMaker::find_Z_boson()
{
   VecBosEvent *mVecBosEvent = wMK->mVecBosEvent;
   // printf("========= find_Z_boson() \n");

   hA[31]->Fill(mVecBosEvent->mVertices.size());
   hA[0]->Fill("inp", 1.);

   // search for  Zs
   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++)
   {
      hA[0]->Fill("vert", 1.);
      VecBosVertex &V = mVecBosEvent->mVertices[iv];
      hA[32]->Fill(V.eleTrack.size());
      if (V.eleTrack.size() < 2) continue;
      hA[0]->Fill("TT", 1.); // at least 2 isolated tracks exist

      //only one Z can come from a vertex, and it should be the highest-energy object
      //hence, the two highest-et clusters should correspond to the z.  Pick those
      //eventually, but for now, just try all of them.
      for (uint it = 0; it < V.eleTrack.size() - 1; it++)
      { // select first track:
         VecBosTrack &T1 = V.eleTrack[it];
         if (T1.mMatchedTower.id <= 0) continue; //skip endcap towers
         if (T1.isMatch2Cl == false) continue;
         assert(T1.mCluster2x2.nTower > 0); // internal logical error
         assert(T1.nearTotET > 0); // internal logical error

         float isoET1 = T1.mCluster2x2.ET / T1.mCluster4x4.ET;
         hA[29]->Fill(isoET1);

         hA[23]->Fill(T1.mCluster2x2.ET);
         hA[0]->Fill("tr1", 1.);
         if (T1.mCluster2x2.ET < par_clusterEtZ) continue;
         hA[0]->Fill("et1", 1.);

         float fracET1 = T1.mCluster2x2.ET / T1.nearTotET;
         hA[24]->Fill(fracET1);
         if (fracET1 < par_nearTotEtFracZ) continue;
         hA[0]->Fill("con1", 1.);

         for (uint it2 = it + 1; it2 < V.eleTrack.size(); it2++)
         { // select second track:
            VecBosTrack &T2 = V.eleTrack[it2];
            if (T2.mMatchedTower.id <= 0) continue; //skip endcap towers
            if (T2.isMatch2Cl == false) continue;
            assert(T2.mCluster2x2.nTower > 0); // internal logical error
            assert(T2.nearTotET > 0); // internal logical error

            float isoET2 = T2.mCluster2x2.ET / T2.mCluster4x4.ET;
            hA[30]->Fill(isoET2);

            hA[25]->Fill(T2.mCluster2x2.ET);
            hA[0]->Fill("tr2", 1.);
            if (T2.mCluster2x2.ET < par_clusterEtZ) continue;
            hA[0]->Fill("et2", 1.);

            float fracET2 = T2.mCluster2x2.ET / T2.nearTotET;
            hA[26]->Fill(fracET2);
            if (fracET2 < par_nearTotEtFracZ) continue;
            hA[0]->Fill("con2", 1.);

            float e1 = T1.mCluster2x2.energy;
            float e2 = T2.mCluster2x2.energy;
            TVector3 p1 = T1.mVec3AtDca; p1.SetMag(e1); //cluster.position;
            TVector3 p2 = T2.mVec3AtDca; p2.SetMag(e2); //cluster.position;

            float del_phi = p1.DeltaPhi(p2);
            //printf("del Phi=%f\n",del_phi);
            float xx = del_phi;
            if (xx < -TMath::Pi() + 1) xx += 2 * TMath::Pi();
            hA[27]->Fill(xx);
            if (fabs(del_phi) < par_delPhi12) continue;
            hA[0]->Fill("phi12", 1.);

            TVector3 psum = p1 + p2;
            float mass2 = (e1 + e2) * (e1 + e2) - (psum.Dot(psum));
            if (mass2 < 1.) continue; // 9GeV^2) should be param, I'm tired today
            hA[0]->Fill("m2", 1.);

            float mass = sqrt(mass2);
            int Q1Q2 = T1.prMuTrack->charge() * T2.prMuTrack->charge();
            if (Q1Q2 == 1) { //..  same sign , can't be Z-> e+ e-
               hA[14]->Fill(mass);
               continue;
            }

            // now only opposite sign
            hA[0]->Fill("QQ", 1.);
            hA[15]->Fill(mass);
            hA[33]->Fill(T1.mCluster2x2.ET, T1.prMuTrack->charge() / T1.prMuTrack->pt());
            hA[33]->Fill(T2.mCluster2x2.ET, T2.prMuTrack->charge() / T2.prMuTrack->pt());
            hA[34]->Fill(T1.mMatchedTower.iEta , T1.mCluster2x2.energy);
            hA[34]->Fill(T2.mMatchedTower.iEta , T2.mCluster2x2.energy);
            hA[35]->Fill(p1.Eta(), p2.Eta());
            hA[36]->Fill(psum.Eta());
            hA[37]->Fill(psum.Pt());

#if 0
            printf("RCC:  Found Z w/ invmass=%f\n", mass);
            printJan(&T1);
            printJan(&T2);


            if (!wMK->isMC || (wMK->isMC && mVecBosEvent->id < 500) ) {
               printf("\n ZZZZZZZZZZZZZZZZZZZ\n");
               if (mass < par_minMassZ)
                  wMK->wDisaply->exportEvent("Zlow", V, T1);
               else
                  wMK->wDisaply->exportEvent("Zgood", V, T1);
               printf("RCC:  Found Z w/ invmass=%f\n", mass);
               mVecBosEvent->print();
            }
#endif

            if (mass < par_minMassZ) continue; //enforce a lower bound
            hA[0]->Fill("Zlow", 1.);

            if (mass > par_maxMassZ) continue; //enforce an upper bound
            hA[0]->Fill("Zhigh", 1.);

            // **** I stoped changes here, Jan

            float fmax1 = T1.mCluster2x2.ET / T1.mCluster4x4.ET;
            float fmax2 = T2.mCluster2x2.ET / T2.mCluster4x4.ET;

            hA[21]->Fill(fmax1, fmax2);
            hA[22]->Fill(T1.mCluster2x2.ET, T2.mCluster2x2.ET);

            hA[1]->Fill(mass);
            hA[2]->Fill(T1.prMuTrack->charge(), T2.prMuTrack->charge());
            hA[3]->Fill(T1.prMuTrack->charge()*T2.prMuTrack->charge());
            hA[4]->Fill(p1.Phi(), p2.Phi());
            hA[5]->Fill(del_phi);
            hA[6]->Fill(mass, T1.prMuTrack->charge() / T1.mVec3AtDca.Perp()*T2.prMuTrack->charge() / T1.mVec3AtDca.Perp());
            hA[7]->Fill(mass, T1.prMuTrack->charge()*T2.prMuTrack->charge());
            hA[8]->Fill(T1.mCluster2x2.ET);
            if (T1.prMuTrack->charge() > 0) {
               hA[9]->Fill(p1.Eta(), p1.Phi());
               hA[10]->Fill(p2.Eta(), p2.Phi());
            }
            else {
               hA[10]->Fill(p1.Eta(), p1.Phi());
               hA[9]->Fill(p2.Eta(), p2.Phi());
            }

            hA[11]->Fill(fmax1, fmax2);
            hA[12]->Fill(T1.mCluster2x2.ET, T2.mCluster2x2.ET);
            hA[13]->Fill(mass, del_phi);
         }
      } // loop over first track
   } // loop over vertices
}
