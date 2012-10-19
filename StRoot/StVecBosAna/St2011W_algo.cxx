#include "TF1.h"

#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include "StEEmcUtil/StEEmcSmd/EEmcSmdGeom.h"
#include "WeventDisplay.h"
#include "StSpinPool/StJets/StJet.h"
#include "StSpinPool/StJets/TowerToJetIndex.h"

#include "StVecBosMaker.h"


void StVecBosMaker::find_W_boson()
{
   if (!mWEvent->l2bitET) return;

   //printf("========= find_W_boson() \n");
   int nNoNear = 0, nNoAway = 0, nEta1 = 0, nGoldW = 0, nGoldWp = 0, nGoldWn = 0;

   //remove events tagged as Zs
   if (mWEvent->zTag) return;

   // search for  Ws
   for (uint iv = 0; iv < mWEvent->mVertices.size(); iv++)
   {
      WEventVertex &vertex = mWEvent->mVertices[iv];
      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         WeveEleTrack &track = vertex.eleTrack[it];
         if (track.pointTower.id <= 0) continue; //skip endcap towers
         if (track.isMatch2Cl == false) continue;

         assert(track.cluster.nTower > 0); // internal logical error
         assert(track.nearTotET > 0);      // internal logical error

         // make cut on lepton eta
         if (track.primP.Eta() < mCutTrackEtaMin || track.primP.Eta() > mCutTrackEtaMax) continue;

         hA[20]->Fill("eta1", 1.);
         nEta1++;

         //signal plots w/o EEMC in veto
         if (track.cluster.ET / track.nearTotET_noEEMC > par_nearTotEtFrac) {
            if (track.sPtBalance_noEEMC > par_ptBalance ) { // only signed ptBalance cut
               hA[140]->Fill(track.cluster.ET);
               hA[240]->Fill(track.prMuTrack->eta(), track.cluster.ET);

               if (track.prMuTrack->charge() < 0) {
                  hA[184 + 3]->Fill(track.cluster.ET);
               } else if (track.prMuTrack->charge() > 0) {
                  hA[184 + 4]->Fill(track.cluster.ET);
               }
            }
         }

         // fill plot for background
         if (track.cluster.ET > par_highET) {
            if (track.prMuTrack->charge() > 0)      hA[251]->Fill(track.cluster.ET / track.nearTotET, track.sPtBalance);
            else if (track.prMuTrack->charge() < 0) hA[252]->Fill(track.cluster.ET / track.nearTotET, track.sPtBalance);
         }

         if (track.cluster.ET / track.nearTotET < par_nearTotEtFrac) continue; // too large nearET

         hA[20]->Fill("noNear", 1.);
         nNoNear++;
         hA[112]->Fill(track.cluster.ET);
         hA[50]->Fill(track.awayTpcPT);
         hA[51]->Fill(track.awayBtowET);
         hA[54]->Fill(track.awayTotET);
         hA[52]->Fill(track.cluster.ET, track.awayTotET);
         hA[53]->Fill(track.cluster.ET, track.awayEmcET);
         hA[55]->Fill(track.awayEtowET);
         hA[60]->Fill(track.cluster.ET, track.awayTpcPT);

         hA[132]->Fill(track.cluster.ET, track.ptBalance.Perp());
         hA[133]->Fill(track.awayTotET, track.ptBalance.Perp());
         hA[134]->Fill(track.cluster.ET, track.sPtBalance);
         hA[135]->Fill(track.awayTotET, track.sPtBalance);
         hA[209]->Fill(track.cluster.position.PseudoRapidity(), track.cluster.ET);

         for (int i = 0; i <= 20; i++) {
            //  float awayTot_cut = 10.+2.*((float) i);
            for (int j = 0; j <= 20; j++) {
               float pTBal_cut = 5. + ((float) j);
               if (track.sPtBalance < pTBal_cut) {
                  if (track.prMuTrack->charge() < 0) {
                     hA[142 + i]->Fill(track.cluster.ET, j);
                  }
                  else if (track.prMuTrack->charge() > 0) {
                     hA[163 + i]->Fill(track.cluster.ET, j);
                  }
               }
            }
         }

         //plots for backg sub yield
         if (track.sPtBalance > par_ptBalance ) {
            hA[136]->Fill(track.cluster.ET);//signal
            hA[241]->Fill(track.prMuTrack->eta(), track.cluster.ET);
            hA[62]->Fill(track.pointTower.iEta , track.cluster.energy);
            if (track.prMuTrack->charge() < 0) {
               hA[184 + 1]->Fill(track.cluster.ET);
            }
            else if (track.prMuTrack->charge() > 0) {
               hA[184 + 2]->Fill(track.cluster.ET);
            }
         }
         else {
            hA[137]->Fill(track.cluster.ET);//background
            if (track.prMuTrack->charge() < 0) {
               hA[184 + 5]->Fill(track.cluster.ET);
            }
            else if (track.prMuTrack->charge() > 0) {
               hA[184 + 6]->Fill(track.cluster.ET);
            }
            hA[202]->Fill(track.cluster.position.PseudoRapidity(), track.prMuTrack->pt());
            hA[204]->Fill(track.cluster.position.PseudoRapidity(), track.cluster.energy / track.prMuTrack->p().mag());
         }

         if (track.sPtBalance > par_ptBalance) {
            Info("find_W_boson", "WWWWWWWWWWWWWWWWWWWWW  Barrel");
            wDisaply->exportEvent( "WB", vertex, track, iv);
            mWEvent->print();
         }

         // put final W cut here
         if (track.sPtBalance < par_ptBalance)  continue;

         hA[20]->Fill("noAway", 1.0);
         nNoAway++;

         //::::::::::::::::::::::::::::::::::::::::::::::::
         //:::::accepted W events for x-section :::::::::::
         //::::::::::::::::::::::::::::::::::::::::::::::::

         hA[113]->Fill( track.cluster.ET);//for Joe

         hA[90]->Fill( track.cluster.ET);
         hA[92]->Fill( track.cluster.ET, track.glMuTrack->dEdx() * 1e6);
         //hA[93]->Fill( track.cluster.ET,track.glMuTrack->dca(vertex.id).mag());
         int k = 0;
         if (track.prMuTrack->charge() < 0) k = 1;
         hA[94 + k]->Fill( track.cluster.ET, track.glMuTrack->dcaD());
         // h95 used above

         // plots to investigate east/west yield diff
         hA[200]->Fill(track.cluster.position.PseudoRapidity(), track.cluster.ET);
         hA[201]->Fill(track.cluster.position.PseudoRapidity(), track.prMuTrack->pt());
         hA[203]->Fill(track.cluster.position.PseudoRapidity(), track.cluster.energy / track.prMuTrack->p().mag());
         hA[205]->Fill(track.prMuTrack->lastPoint().pseudoRapidity(), track.prMuTrack->lastPoint().phi());

         // Q/pT plot
         hA[100]->Fill(track.cluster.ET, track.glMuTrack->charge() / track.glMuTrack->pt());
         hA[101]->Fill(track.cluster.ET, track.prMuTrack->charge() / track.prMuTrack->pt());

         if (track.cluster.ET < par_highET) continue; // very likely Ws

         hA[91]->Fill(track.cluster.position.PseudoRapidity(), track.cluster.position.Phi());
         hA[96]->Fill(vertex.id);
         hA[97]->Fill(vertex.funnyRank);
         hA[98]->Fill(vertex.z);
         hA[99]->Fill(track.prMuTrack->eta());
         hA[190 + k]->Fill(track.prMuTrack->eta(), track.cluster.ET);

         hA[20]->Fill("goldW", 1.);
         nGoldW++;

         if (track.prMuTrack->charge() > 0)      nGoldWp++;
         else if (track.prMuTrack->charge() < 0) nGoldWn++;
      } // loop over tracks
   } // loop over vertices

   if (nNoNear > 0) hA[0]->Fill("noNear", 1.);
   if (nNoAway > 0) hA[0]->Fill("noAway", 1.);
   if (nEta1   > 0) hA[0]->Fill("eta1",   1.);
   if (nGoldW  > 0) hA[0]->Fill("goldW",  1.);
   if (nGoldWp > 0) hA[0]->Fill("goldW+", 1.);
   if (nGoldWn > 0) hA[0]->Fill("goldW-", 1.);
}


void StVecBosMaker::tag_Z_boson()
{
   float par_jetPt = 10.;
   float lowMass   = 70.;
   float highMass  = 140.;
   //select specific jet-type
   mJets = GetJets(mJetTreeBranch);
   if (mJetTreeChain) mJets = GetJetsTreeAnalysis(mJetTreeBranch);

   //form invariant mass from lepton candidate and jet
   //vertex loop
   for (uint iv = 0; iv < mWEvent->mVertices.size(); iv++)
   {
      WEventVertex &vertex = mWEvent->mVertices[iv];
      for (uint it = 0; it < vertex.eleTrack.size(); it++) // select track
      {
         WeveEleTrack &T1 = vertex.eleTrack[it];
         if (T1.isMatch2Cl == false) continue;
         assert(T1.cluster.nTower > 0); // internal logical error
         assert(T1.nearTotET > 0); // internal logical error

         if (T1.cluster.ET / T1.nearTotET < par_nearTotEtFrac) continue; // too large nearET

         //match lepton candidate with jet
         TLorentzVector jetVec;
         for (uint iJet = 0; iJet < mWEvent->mNJets; iJet++) { //jet loop
            jetVec = *((StJet*) mJets->At(iJet));
            if (jetVec.Pt() < par_jetPt) continue; //remove low pt jets

            //electron like cut on jets
            StJet *jet = GetJet(iJet);  float maxCluster = 0.;
            int totTowers = jet->nBtowers + jet->nEtowers;
            for (int itow = 0; itow < totTowers; itow++) { //loop over towers
               if (jet->tower(itow)->detectorId() == 13) //drop endcap towers
                  continue;

               int towerId = jet->tower(itow)->towerId();
               //find highest 2x2 BTOW cluster in jet
               TVector3 pos = positionBtow[towerId - 1];

               int iEta, iPhi;
               if ( !ConvertEtaPhi2Bins(pos.Eta(), pos.Phi(), iEta, iPhi) ) continue;

               float cluster = maxBtow2x2(iEta, iPhi, jet->zVertex).ET;
               if (cluster > maxCluster) maxCluster = cluster;
            }

            TVector3 jetVec3(jetVec.X(), jetVec.Y(), jetVec.Z());
            if (jetVec3.DeltaR(T1.primP) < par_nearDeltaR)
               continue;//skip jets in candidate phi isolation'cone'

            // form invM
            float e1 = T1.cluster.energy;
            TVector3 p1 = T1.primP; p1.SetMag(e1);
            TLorentzVector ele1(p1, e1); //lepton candidate 4- mom
            TLorentzVector sum = ele1 + jetVec;
            float invM = sqrt(sum * sum);
            if (maxCluster / jet->jetPt < 0.5) continue;
            if (invM > lowMass && invM < highMass)
               mWEvent->zTag = true;
         }
      }
   }
}


void StVecBosMaker::CalcPtBalance()
{
   for (uint iv = 0; iv < mWEvent->mVertices.size(); iv++)
   {
      WEventVertex &vertex = mWEvent->mVertices[iv];

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         WeveEleTrack &track = vertex.eleTrack[it];

         if (track.isMatch2Cl == false) continue;

         // Loop over branch with EEMC
         mJets = GetJets(mJetTreeBranch);
         if (mJetTreeChain) mJets = GetJetsTreeAnalysis(mJetTreeBranch);

         // Add up all jets outside of nearDeltaR cone around the electron track
         for (uint iJet = 0; iJet<mWEvent->mNJets; iJet++) { //loop over jets
            StJet *jet = GetJet(iJet);
            TVector3 jetVec; //vector for jet momentum
            jetVec.SetPtEtaPhi(jet->Pt(), jet->Eta(), jet->Phi());

            if (jetVec.DeltaR(track.primP) > par_nearDeltaR)
               track.ptBalance += jetVec;
         }

         TVector3 clustPt(track.primP.X(), track.primP.Y(), 0);
         clustPt.SetMag(track.cluster.ET);

         // Add electron energy. XXX:ds: Why is the energy transverse only?
         track.ptBalance  += clustPt;
         track.sPtBalance  = track.ptBalance.Perp();

         if (track.ptBalance.Dot(clustPt) < 0)
            track.sPtBalance *= -1.;

         // Loop over branch without EEMC
         mJets = GetJets(mJetTreeBranch_noEEMC);
         if (mJetTreeChain) mJets = GetJetsTreeAnalysis(mJetTreeBranch_noEEMC);

         for (uint iJet = 0; iJet < mWEvent->mNJets; iJet++) { //loop over jets
            StJet *jet = GetJet(iJet);
            TVector3 jetVec; //vector for jet momentum
            jetVec.SetPtEtaPhi(jet->Pt(), jet->Eta(), jet->Phi());

            if (jetVec.DeltaR(track.primP) > par_nearDeltaR)
               track.ptBalance_noEEMC += jetVec;
         }

         track.ptBalance_noEEMC += clustPt;
         track.sPtBalance_noEEMC = track.ptBalance_noEEMC.Perp();

         if (track.ptBalance_noEEMC.Dot(clustPt) < 0)
            track.sPtBalance_noEEMC *= -1.;
      } // end of loop over tracks
   } // end of loop over vertices
}


void StVecBosMaker::CalcMissingET()
{
   for (uint iv = 0; iv < mWEvent->mVertices.size(); iv++)
   {
      WEventVertex &vertex = mWEvent->mVertices[iv];
      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         WeveEleTrack &track = vertex.eleTrack[it];

         if (track.isMatch2Cl == false) continue;

         // Loop over branch with EEMC
         mJets = GetJets(mJetTreeBranch);
         if (mJetTreeChain) mJets = GetJetsTreeAnalysis(mJetTreeBranch);

         // Add up all jets outside of nearDeltaR cone around the electron track
         for (uint iJet = 0; iJet<mWEvent->mNJets; iJet++) { //loop over jets
            StJet *jet = GetJet(iJet);
            TVector3 jetVec; //vector for jet momentum
            jetVec.SetPtEtaPhi(jet->Pt(), jet->Eta(), jet->Phi());

            if (jetVec.DeltaR(track.primP) > par_nearDeltaR)
               track.ptBalance += jetVec;
         }

         TVector3 clustPt(track.primP.X(), track.primP.Y(), 0);
         clustPt.SetMag(track.cluster.ET);

         // Add electron energy. XXX:ds: Why is the energy transverse only?
         track.ptBalance  += clustPt;
         track.sPtBalance  = track.ptBalance.Perp();

         if (track.ptBalance.Dot(clustPt) < 0)
            track.sPtBalance *= -1.;

         // Loop over branch without EEMC
         mJets = GetJets(mJetTreeBranch_noEEMC);
         if (mJetTreeChain) mJets = GetJetsTreeAnalysis(mJetTreeBranch_noEEMC);

         for (uint iJet = 0; iJet < mWEvent->mNJets; iJet++) { //loop over jets
            StJet *jet = GetJet(iJet);
            TVector3 jetVec; //vector for jet momentum
            jetVec.SetPtEtaPhi(jet->Pt(), jet->Eta(), jet->Phi());

            if (jetVec.DeltaR(track.primP) > par_nearDeltaR)
               track.ptBalance_noEEMC += jetVec;
         }

         track.ptBalance_noEEMC += clustPt;
         track.sPtBalance_noEEMC = track.ptBalance_noEEMC.Perp();

         if (track.ptBalance_noEEMC.Dot(clustPt) < 0)
            track.sPtBalance_noEEMC *= -1.;
      } // end of loop over tracks
   } // end of loop over vertices
}


void StVecBosMaker::findAwayJet()
{
   // printf("\n******* find AwayJet() nVert=%d\n", mWEvent->mVertices.size());
   //mWEvent->print();
   for (uint iv = 0; iv < mWEvent->mVertices.size(); iv++)
   {
      WEventVertex &vertex = mWEvent->mVertices[iv];

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         WeveEleTrack &track = vertex.eleTrack[it];

         if (track.isMatch2Cl == false) continue;

         // sum opposite in phi EMC components
         track.awayBtowET  = sumBtowCone(vertex.z, -track.primP, 1); // '1' = only cut on delta phi
         track.awayEmcET   = track.awayBtowET;
         track.awayEtowET  = sumEtowCone(vertex.z, -track.primP, 1);
         track.awayEmcET  += track.awayEtowET;

         // add TPC ET
         if (mStMuDstMaker)
            track.awayTpcPT = sumTpcCone(vertex.id, -track.primP, 1, track.pointTower.id);
         else
            track.awayTpcPT = sumTpcConeFromTree(iv, -track.primP, 1, track.pointTower.id);

         track.awayTotET = track.awayEmcET + track.awayTpcPT;
         track.awayTotET_noEEMC = track.awayBtowET + track.awayTpcPT;

         //printf("\n*** in   awayTpc=%.1f awayEmc=%.1f\n  ",track.awayTpcPT,track.awayEmcET); track.print();
      }
   }
}


/**
 * Calculates the energy in the cone around the electron.
 * XXX:ds: The total includes the EMC and TPC energies. Isn't it doble counting?
 */
void StVecBosMaker::findNearJet()
{
   //printf("\n******* findNearJet() nVert=%d\n",mWEvent->mVertices.size());

   for (uint iv = 0; iv < mWEvent->mVertices.size(); iv++)
   {
      WEventVertex &vertex = mWEvent->mVertices[iv];

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         WeveEleTrack &track = vertex.eleTrack[it];
         if (track.isMatch2Cl == false) continue;

         // sum EMC-jet component
         track.nearBtowET  = sumBtowCone(vertex.z, track.primP, 2); // '2'=2D cone
         track.nearEmcET  += track.nearBtowET;
         track.nearEtowET  = sumEtowCone(vertex.z, track.primP, 2);
         track.nearEmcET  += track.nearEtowET;

         // sum TPC-near component
         if (mStMuDstMaker)
            track.nearTpcPT = sumTpcCone(vertex.id, track.primP, 2, track.pointTower.id); // '2'=2D cone
         else
            track.nearTpcPT = sumTpcConeFromTree(iv, track.primP, 2, track.pointTower.id);

         float nearSum = track.nearEmcET + track.nearTpcPT; // XXX:ds: double counting? yes, see correction below

         // fill histos separately for 2 types of events
         if (track.pointTower.id > 0) { //only barrel towers

            // correct for double counting of electron track in near cone rarely primTrPT<10 GeV & globPT>10 - handle this here
            if (track.primP.Pt() > par_trackPt) nearSum -= par_trackPt;
            else                                nearSum -= track.primP.Pt();

            track.nearTotET        = nearSum;
            track.nearTotET_noEEMC = nearSum - track.nearEtowET;
            float nearTotETfrac    = track.cluster.ET / track.nearTotET;

            hA[40]->Fill(track.nearEmcET);
            hA[41]->Fill(track.cluster.ET, track.nearEmcET - track.cluster.ET);
            hA[42]->Fill(nearTotETfrac);
            hA[47]->Fill(track.nearTpcPT);
            hA[48]->Fill(track.nearEmcET, track.nearTpcPT);
            hA[49]->Fill(nearSum);
            hA[250]->Fill(track.cluster.ET, nearTotETfrac);

            // check east/west yield diff
            hA[210]->Fill(track.cluster.position.PseudoRapidity(), track.nearEtowET);

            if (track.cluster.position.PseudoRapidity() > 0) hA[211]->Fill(track.cluster.position.Phi(), track.nearEtowET);
            else hA[212]->Fill(track.cluster.position.Phi(), track.nearEtowET);

         } else if (track.pointTower.id < 0) { //only endcap towers
            // correct for double counting of electron track in near cone rarely primTrPT<10 GeV & globPT>10 - handle this here
            if (track.primP.Pt() > parE_trackPt) nearSum -= parE_trackPt;
            else  nearSum -= track.primP.Pt();
            track.nearTotET = nearSum;
            track.nearTotET_noEEMC = nearSum - track.nearEtowET;
            float nearTotETfrac = track.cluster.ET / track.nearTotET;

            hE[40]->Fill(track.nearEmcET);
            hE[41]->Fill(track.cluster.ET, track.nearEmcET - track.cluster.ET);
            hE[42]->Fill(nearTotETfrac);
            hE[47]->Fill(track.nearTpcPT);
            hE[48]->Fill(track.nearEmcET, track.nearTpcPT);
            hE[49]->Fill(nearSum);
         }
      }
   }
}


/**
 * Returns the sum of all towers withing par_nearDeltaR around the track
 * refAxis.
 */
float StVecBosMaker::sumBtowCone(float zVert, TVector3 refAxis, int flag)
{
   /* flag=1 : only delta phi cut;  flag=2 use 2D cut */
   assert(flag == 1 || flag == 2);
   double ptSum = 0;

   // process BTOW hits
   for (int i=0; i<mxBtow; i++)
   {
      float ene = mWEvent->bemc.eneTile[kBTow][i];
      if (ene <= 0) continue;

      TVector3 primP = positionBtow[i] - TVector3(0, 0, zVert);
      primP.SetMag(ene); // it is 3D momentum in the event ref frame

      if (flag == 1) {
         float deltaPhi = refAxis.DeltaPhi(primP);
         if (fabs(deltaPhi) > par_awayDeltaPhi) continue;
      }
      if (flag == 2) {
         float deltaR = refAxis.DeltaR(primP);
         if (deltaR > par_nearDeltaR) continue;
      }

      ptSum += primP.Perp();
   }

   return ptSum;
}


float StVecBosMaker::sumTpcConeFromTree(int vertID, TVector3 refAxis, int flag, int pointTowId)
{
   // flag=2 use 2D cut, 1= only delta phi

   assert(vertID >= 0);
   assert(vertID < (int)mWEvent->mVertices.size());

   double ptSum = 0;
   WEventVertex &vertex = mWEvent->mVertices[vertID];
   for (uint it = 0; it < vertex.prTrList.size(); it++) {
      StMuTrack *prTr = vertex.prTrList[it];
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
}


/** */
void StVecBosMaker::extendTrack2Barrel()
{
   //printf("******* extendTracks() nVert=%d\n", mWEvent->mVertices.size());
   if (!mWEvent->l2bitET) return; //fire barrel trigger

   // loop over vertices
   for (uint iv = 0; iv < mWEvent->mVertices.size(); iv++)
   {
      WEventVertex &vertex = mWEvent->mVertices[iv];
      if (vertex.rank < 0) continue; //remove vertex for endcap algo only

      // loop over tracks
      for (uint iTrack = 0; iTrack < vertex.eleTrack.size(); iTrack++)
      {
         WeveEleTrack &track = vertex.eleTrack[iTrack];

         if (track.prMuTrack->flag() != 301) continue; //remove track for endcap algo only

         // Apply eta cuts at track level (tree analysis)
         if (track.primP.Eta() < mCutTrackEtaMin || track.primP.Eta() > mCutTrackEtaMax) continue;

         // extrapolate track to the barrel @ R=entrance
         const StPhysicalHelixD TrkHlx = track.prMuTrack->outerHelix();
         float Rcylinder = mBtowGeom->Radius();
         pairD d2 = TrkHlx.pathLength(Rcylinder);
         //printf(" R=%.1f path 1=%f, 2=%f, period=%f, R=%f\n",Rctb,d2.first ,d2.second,TrkHlx.period(),1./TrkHlx.curvature());

         // assert(d2.first  < 0); // propagate backwards
         // assert(d2.second > 0); // propagate forwards
         if (d2.first >= 0 || d2.second <= 0) {
            LOG_WARN << Form("MatchTrk , unexpected solution for track crossing CTB\n" \
                             "d2.first=%f, d2.second=%f, swap them", d2.first, d2.second) << endm;
            float xx  = d2.first;
            d2.first  = d2.second;
            d2.second = xx;
         }

         // extrapolate track to cylinder
         StThreeVectorD posR = TrkHlx.at(d2.second);
         //printf(" punch2 x,y,z=%.1f, %.1f, %.1f, Rxy=%.1f\n",posCTB.x(),posCTB.y(),posCTB.z(),xmagn);
         float eta = posR.pseudoRapidity();
         float phi = posR.phi();

         int iEta, iPhi;
         if ( !ConvertEtaPhi2Bins(eta, phi, iEta, iPhi) ) continue;

         hA[20]->Fill("@B", 1.);
         //printf(" phi=%.0f deg,  eta=%.2f, iEta=%d, iPhi=%d\n",posCTB.phi()/3.1416*180.,posCTB. pseudoRapidity(),iEta, iPhi);
         int towerId = mapBtowIJ2ID[ iEta + iPhi * mxBTetaBin];
         // printf("hit Tower ID=%d\n",towerId);

         track.pointTower.id   = towerId;
         track.pointTower.R    = TVector3(posR.x(), posR.y(), posR.z());
         track.pointTower.iEta = iEta;
         track.pointTower.iPhi = iPhi;
         //track.print();
      }
   }

   hA[0]->Fill("TrB", 1.0);
}


bool StVecBosMaker::matchTrack2BtowCluster()
{
   // printf("******* matchCluster() nVert=%d\n",mWEvent->mVertices.size());
   int   numMatchedTracks = 0;
   float Rcylinder = mBtowGeom->Radius();

   for (uint iv = 0; iv < mWEvent->mVertices.size(); iv++)
   {
      WEventVertex &vertex = mWEvent->mVertices[iv];
      float vertexZ = vertex.z;

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         WeveEleTrack &track = vertex.eleTrack[it];
         if (track.pointTower.id <= 0) continue; // skip endcap towers

         float trackPT = track.prMuTrack->momentum().perp();

         // Choose 2x2 cluster with maximum ET
         track.cluster = maxBtow2x2( track.pointTower.iEta, track.pointTower.iPhi, vertexZ);

         hA[33]->Fill(track.cluster.ET);
         hA[34]->Fill(track.cluster.adcSum, trackPT);
         hA[110]->Fill(track.cluster.ET);

         // Compute surroinding cluster energy
         int iEta = track.cluster.iEta;
         int iPhi = track.cluster.iPhi;
         track.cl4x4 = sumBtowPatch(iEta - 1, iPhi - 1, 4, 4, vertexZ); // needed for lumi monitor

         if (track.cluster.ET < par_clustET) continue; // too low energy

         hA[20] ->Fill("CL", 1.);
         hA[206]->Fill(track.cluster.position.PseudoRapidity(), track.cluster.ET);
         hA[37] ->Fill(track.cl4x4.ET);
         hA[38] ->Fill(track.cluster.energy, track.cl4x4.energy - track.cluster.energy);

         float frac24 = track.cluster.ET / (track.cl4x4.ET);
         hA[39]->Fill(frac24);
         if (frac24 < par_clustFrac24) continue;

         hA[20]->Fill("fr24", 1.);

         // spacial separation (track - cluster)
         TVector3 D = track.pointTower.R - track.cluster.position;

         hA[43]->Fill( track.cluster.energy, D.Mag());
         hA[44]->Fill( track.cluster.position.z(), D.z());

         float delPhi = track.pointTower.R.DeltaPhi(track.cluster.position);
         // printf("aaa %f %f %f   phi=%f\n",D.x(),D.y(),D.z(),delPhi);
         hA[45]->Fill( track.cluster.energy, Rcylinder * delPhi); // wrong?
         hA[46]->Fill( D.Mag());
         hA[199]->Fill(track.cluster.position.PseudoRapidity(), D.Mag());
         hA[207]->Fill(track.cluster.position.PseudoRapidity(), track.cluster.ET);

         if (D.Mag() > par_delR3D) continue;

         track.isMatch2Cl = true; // cluster is matched to TPC track

         hA[20]->Fill("#Delta R", 1.);
         hA[111]->Fill(track.cluster.ET);

         hA[208]->Fill(track.cluster.position.PseudoRapidity(), track.cluster.ET);

         numMatchedTracks++;
      }
   }

   if (!numMatchedTracks) return false;

   hA[0]->Fill("Tr2Cl", 1.0);

   return true;
}


/**
 * For a given eta-phi bin considers all combinations of 2x2 bin clusters around it
 * and returns one with the maximum ET.
 */
WeveCluster StVecBosMaker::maxBtow2x2(int etaBin, int phiBin, float zVert)
{
   //printf("maxBtow2x2  seed etaBin=%d phiBin=%d \n",etaBin, phiBin);
   const int L = 2; // size of the summed square

   WeveCluster maxCL;

   // just 4 cases of 2x2 clusters
   float maxET = 0;

   for (int iEta=etaBin-1; iEta<=etaBin; iEta++)
   {
      for (int iPhi=phiBin-1; iPhi<=phiBin; iPhi++)
      {
         WeveCluster CL = sumBtowPatch(iEta, iPhi, L, L, zVert);
         if (maxET > CL.ET) continue;
         maxET = CL.ET;
         maxCL = CL;
         // printf("   newMaxETSum=%.1f etaBin=%d iPhi=%d \n",maxET, I,J);
      }
   }

   //printf(" final inpEve=%d SumET2x2=%.1f \n",nInpEve,maxET);
   return maxCL;
}


WeveCluster StVecBosMaker::sumBtowPatch(int etaBin, int phiBin, int etaWidth, int  phiWidth, float zVert)
{
   //printf("  eveID=%d btowSquare seed etaBin=%d[+%d] phiBin=%d[+%d] zVert=%.0f \n",mWEvent->id,etaBin,etaWidth, phiBin,phiWidth,zVert);
   WeveCluster CL; // object is small, not to much overhead in creating it
   CL.iEta = etaBin;
   CL.iPhi = phiBin;
   TVector3 R;
   double sumW = 0;
   float Rcylinder  = mBtowGeom->Radius();
   float Rcylinder2 = Rcylinder *Rcylinder;

   for (int iEta = etaBin; iEta < etaBin + etaWidth; iEta++) // trim in eta-direction
   {
      if (iEta < 0) continue;
      if (iEta >= mxBTetaBin) continue;

      for (int iPhi = phiBin; iPhi < phiBin + phiWidth; iPhi++)
      {
         // wrap up in the phi-direction
         int   iPhi_p = (iPhi + mxBTphiBin) % mxBTphiBin;         // keep it always positive
         int   towerId = mapBtowIJ2ID[ iEta + iPhi_p*mxBTetaBin];
         float energy = mWEvent->bemc.eneTile[kBTow][towerId - 1];

         //if (L<5) printf("n=%2d  iEta=%d iPhi_p=%d\n",CL.nTower,iEta,iPhi_p);

         if (energy <= 0) continue; // skip towers w/o energy

         float adc    = mWEvent->bemc.adcTile[kBTow][towerId - 1];
         float delZ   = positionBtow[towerId - 1].z() - zVert;
         float cosine = Rcylinder / sqrt(Rcylinder2 + delZ *delZ);
         float ET     = energy * cosine;
         float logET  = log10(ET + 0.5);

         CL.nTower++;
         CL.energy += energy;
         CL.ET     += ET;
         CL.adcSum += adc;

         if (logET > 0) {
            R    += logET * positionBtow[towerId - 1];
            sumW += logET;
         }
         // if(etaWidth==2)
         //    printf("etaBin=%d phiBin=%d  ET=%.1f  energy=%.1f   sum=%.1f logET=%f sumW=%f\n",iEta,iPhi,ET,energy,CL.energy,logET,sumW);
      }

      // printf(" end btowSquare: etaBin=%d  nTw=%d, ET=%.1f adc=%.1f\n",iEta,CL.nTower,CL.ET,CL.adcSum);
      if (sumW > 0)
         CL.position = 1. / sumW * R; // weighted cluster position
      else
         CL.position = TVector3(0, 0, 999);
   }

   return CL;
}
