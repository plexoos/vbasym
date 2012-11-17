
#include "VecBosTrack.h"

#include "VecBosEvent.h"
#include "VecBosVertex.h"
#include "Globals.h"


ClassImp(VecBosTrack)

using namespace std;


VecBosTrack::VecBosTrack() : TObject(), mEvent(0), mType(kUNKNOWN), mHelix(), mVertex(0),
   mVec3AtDca(), mVec3AtBTow(), mCoorAtBTow()
{
   clear();
}


void VecBosTrack::print(int flag)
{
   if (prMuTrack == 0) {  printf("   Track NULL pointer???\n"); return; }

   printf("   Track glPT=%.1f GeV/c   isMatch2Cl=%d, nearTotET=%.1f, awayTotET=%.1f mVec3AtDcaT=%.1f\n",
          glMuTrack->pt(), isMatch2Cl, nearTotET, awayTotET, mVec3AtDca.Pt());

   mMatchedTower.print(flag);
   mCluster2x2.print(flag);

   TVector3 D = mMatchedTower.R - mCluster2x2.position;
   printf("                XYZ(track-mCluster2x2):  |3D dist|=%.1fcm  delZ=%.1fcm\n", D.Mag(), D.z());
   printf("     4x4 :"); mCluster4x4.print(flag);
   printf("     nearET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n", nearTpcPT, nearEmcET, nearBtowET, nearEtowET, nearTotET);
   printf("     awayET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n", awayTpcPT, awayEmcET, awayBtowET, awayEtowET, awayTotET);
}


void VecBosTrack::Process()
{
   // Good track must come from a good vertex
   if (!mVertex || !mVertex->IsGood()) {
      mType = kBAD;
      return;
   }

   mType = kGOOD; // this track has a good vertex

   //if (mVecBosEvent->l2bitET && rank > 0 && primaryTrack->flag() == 301)
      //XXX:ds:if (secID == 20) continue; //poorly calibrated sector for Run 9+11+12?
      //XXX:ds:if (mTpcFilter[secID - 1].accept(primaryTrack) == false) continue;

   //if (mVecBosEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)
      //XXX:ds:if ( mTpcFilterE[secID - 1].accept(primaryTrack) == false) continue;

   //XXX:ds:if (!barrelTrack && !endcapTrack) continue;

   // Look for high Pt electron candidates
   if ( prMuTrack->pt() >= 1.0 && prMuTrack->flag() == 301 )
   {
      mType |= kBARREL;
      MatchTrack2BtowCluster();
   }
   else if ( prMuTrack->pt() >= 1.0 && prMuTrack->flag() == 311 )
   {
      mType |= kENDCAP;
      //ExtendTrack2Endcap();
   }
}


void VecBosTrack::clear()
{
   mEvent               = 0;
   mType                = kUNKNOWN;
   isMatch2Cl           = false;
   mMatchedTower.clear();
   glMuTrack            = 0;
   prMuTrack            = 0;
   //mHelix               = St
   mVertex              = 0;
   mCluster2x2.clear();
   mCluster4x4.clear();
   mVec3AtDca           = TVector3(0, 0, 0);
   mVec3AtBTow          = TVector3(0, 0, 0);
   mCoorAtBTow          = TVector3(0, 0, 0);
   awayTpcPT            = 0;
   nearTpcPT            = 0;
   nearTotET            = 0;
   awayTotET            = 0;
   nearEmcET            = 0;
   awayEmcET            = 0;
   nearBtowET           = 0;
   awayBtowET           = 0;
   nearEtowET           = 0;
   awayEtowET           = 0;
   smallNearTpcPT       = 0;
   nearTotET_noEEMC     = 0;
   awayTotET_noEEMC     = 0;
   ptBalance            = TVector3(0, 0, 0);
   ptBalance_noEEMC     = TVector3(0, 0, 0);
   sPtBalance           = 0;
   sPtBalance_noEEMC    = 0;
   hadronicRecoil       = TVector3(0, 0, 0);

   memset(esmdGlobStrip, -999, sizeof(esmdGlobStrip));
   memset(esmdDca, -999., sizeof(esmdDca));
   memset(esmdDcaGlob, -999., sizeof(esmdDcaGlob));
   memset(esmdE, 0., sizeof(esmdE));
   memset(esmdNhit, 0, sizeof(esmdNhit));
   memset(esmdShowerCentroid, 999., sizeof(esmdShowerCentroid));
   memset(esmdShowerWidth, 999., sizeof(esmdShowerWidth));

   esmdXPcentroid = TVector3(0, 0, 0);
}


TVector3 VecBosTrack::CalcDistanceToMatchedCluster()
{
   return mCoorAtBTow - mCluster2x2.position;
}


/** */
bool VecBosTrack::ExtendTrack2Barrel()
{
   //printf("******* extendTracks() nVert=%d\n", mVecBosEvent->mVertices.size());
   //if (!mVecBosEvent->l2bitET) return; //fire barrel trigger

   // Apply eta cuts at track level (tree analysis)
   //if (mVec3AtDca.Eta() < mMinBTrackEta || mVec3AtDca.Eta() > mMaxBTrackEta) continue;

   // extrapolate track to the barrel @ R=entrance
   mHelix               = prMuTrack->outerHelix();
   float  nomBTowRadius = gBTowGeom->Radius();
   pairD  segmentLength = mHelix.pathLength(nomBTowRadius); // XXX:ds: Length along the helix from the origin to the intersection point
   //printf(" R=%.1f path 1=%f, 2=%f, period=%f, R=%f\n", Rctb, segmentLength.first, segmentLength.second, mHelix.period(), 1./mHelix.curvature());

   // assert(segmentLength.first  < 0); // propagate backwards
   // assert(segmentLength.second > 0); // propagate forwards
   if (segmentLength.first >= 0 || segmentLength.second <= 0)
   {
      Info("ExtendTrack2Barrel", "MatchTrk , unexpected solution for track crossing CTB\n" \
                                 "segmentLength.first=%f, segmentLength.second=%f, swap them", segmentLength.first, segmentLength.second);
      float xx  = segmentLength.first;
      segmentLength.first  = segmentLength.second;
      segmentLength.second = xx;
   }

   // Extrapolate track to cylinder
   StThreeVectorD posAtBTow = mHelix.at(segmentLength.second);
   //printf(" punch2 x,y,z=%.1f, %.1f, %.1f, Rxy=%.1f\n",posCTB.x(),posCTB.y(),posCTB.z(),xmagn);
   float etaAtBTow   = posAtBTow.pseudoRapidity();
   float phiAtBTow   = posAtBTow.phi();

   mCoorAtBTow.SetXYZ(posAtBTow.x(), posAtBTow.y(), posAtBTow.z());

   //Info("ExtendTrack2Barrel", "XXX");
   //mVec3AtDca.Print();
   //mCoorAtBTow.Print();

   int iEta, iPhi;

   if ( !ConvertEtaPhi2Bins(etaAtBTow, phiAtBTow, iEta, iPhi) ) return false;

   //printf(" phiAtBTow=%.0f deg,  etaAtBTow=%.2f, iEta=%d, iPhi=%d\n", posCTB.phi()/3.1416*180., posCTB. pseudoRapidity(),iEta, iPhi);
   // printf("hit Tower ID=%d\n",towerId);

   mMatchedTower.id   = gMapBTowEtaPhiBin2Id[ iEta + iPhi * mxBTetaBin];
   mMatchedTower.R    = mCoorAtBTow; //TVector3(posAtBTow.x(), posAtBTow.y(), posAtBTow.z());
   mMatchedTower.iEta = iEta;
   mMatchedTower.iPhi = iPhi;
   //print();

   return true;
}


void VecBosTrack::MatchTrack2BtowCluster()
{
   // Do not proced if the track cannot be extended to barrel
   if ( !ExtendTrack2Barrel() ) return;

   //if (mMatchedTower.id <= 0) return; // skip endcap towers

   //printf("******* matchCluster() nVert=%d\n",mVecBosEvent->mVertices.size());
   float nomBTowRadius = gBTowGeom->Radius();

   //float trackPT = prMuTrack->momentum().perp();

   // Choose 2x2 cluster with maximum ET
   mCluster2x2 = FindMaxBTow2x2(*mEvent, mMatchedTower.iEta, mMatchedTower.iPhi, mVertex->z);

   //hA[33] ->Fill(mCluster2x2.ET);
   //hA[34] ->Fill(mCluster2x2.adcSum, trackPT);
   //hA[110]->Fill(mCluster2x2.ET);

   // Compute surroinding cluster energy
   int iEta = mCluster2x2.iEta;
   int iPhi = mCluster2x2.iPhi;

   mCluster4x4 = SumBTowPatch(*mEvent, iEta - 1, iPhi - 1, 4, 4, mVertex->z); // needed for lumi monitor

   //if (mCluster2x2.ET < mMinBClusterEnergy) continue; // too low energy

   //hA[20] ->Fill("CL", 1.);
   //hA[206]->Fill(mCluster2x2.position.PseudoRapidity(), mCluster2x2.ET);
   //hA[37] ->Fill(mCluster4x4.ET);
   //hA[38] ->Fill(mCluster2x2.energy, mCluster4x4.energy - mCluster2x2.energy);

   //float frac24 = mCluster2x2.ET / mCluster4x4.ET;

   //hA[39]->Fill(frac24);

   //if (frac24 < mMinBClusterEnergyIsoRatio) continue;

   //hA[20]->Fill("fr24", 1.);

   // spacial separation (track - cluster)
   TVector3 delta3D = mCoorAtBTow - mCluster2x2.position;

   //hA[43]->Fill(mCluster2x2.energy,       delta3D.Mag());
   //hA[44]->Fill(mCluster2x2.position.z(), delta3D.z());

   //float deltaPhi = mCoorAtBTow.DeltaPhi(mCluster2x2.position);

   // printf("aaa %f %f %f   phi=%f\n",delta3D.x(),delta3D.y(),delta3D.z(),deltaPhi);
   //hA[45]->Fill( mCluster2x2.energy, nomBTowRadius * deltaPhi); // wrong?
   //hA[46]->Fill( delta3D.Mag());
   //hA[199]->Fill(mCluster2x2.position.PseudoRapidity(), delta3D.Mag());
   //hA[207]->Fill(mCluster2x2.position.PseudoRapidity(), mCluster2x2.ET);

   if (delta3D.Mag() <= mEvent->mMaxTrackClusterDist)
   {
      isMatch2Cl = true; // cluster is matched to TPC track
      mType |= kHAS_CLUSTER;
      //mVecBosEvent->mLeptonBTracks.insert(&track);
   }

   //hA[20]->Fill("#Delta R", 1.);
   //hA[111]->Fill(mCluster2x2.ET);
   //hA[208]->Fill(mCluster2x2.position.PseudoRapidity(), mCluster2x2.ET);
   //hA[0]->Fill("Tr2Cl", 1.0);
}


/**
 * Calculates the energy in the cone around the electron.
 */
void VecBosTrack::FindNearJet()
{
   //printf("\n******* FindNearJet() nVert=%d\n",mVecBosEvent->mVertices.size());

   if (!isMatch2Cl) return;

   // sum EMC-jet component. XXX:ds: Note: the track direction is taken at the origin
   nearBtowET  = SumBTowCone(*mEvent, mVertex->z, mVec3AtDca, 2); // '2'=2D cone
   //nearEtowET  = SumETowCone(mVertex->z, mVec3AtDca, 2);
   nearEmcET  += nearBtowET;
   nearEmcET  += nearEtowET;

   // sum TPC-near component
   //if (mStMuDstMaker)
//      nearTpcPT = SumTpcCone(mVertex->id, mVec3AtDca, 2, mMatchedTower.id); // '2'=2D cone
   //else
   //   nearTpcPT = SumTpcConeFromTree(iv, mVec3AtDca, 2, mMatchedTower.id);

   float nearSum = nearEmcET + nearTpcPT; // XXX:ds: double counting? yes, see correction below

   // fill histos separately for 2 types of events
   //if (mMatchedTower.id > 0) { //only barrel towers

   // Correct for double counting of electron track in near cone rarely primTrPT<10 GeV & globPT>10 - handle this here
   if (mVec3AtDca.Pt() > mEvent->mMinBTrackPt) nearSum -= mEvent->mMinBTrackPt;
   else                                        nearSum -= mVec3AtDca.Pt();

   nearTotET        = nearSum;
   nearTotET_noEEMC = nearSum - nearEtowET;
   float nearTotETfrac    = mCluster2x2.ET / nearTotET;

   //hA[40]->Fill(nearEmcET);
   //hA[41]->Fill(mCluster2x2.ET, nearEmcET - mCluster2x2.ET);
   //hA[42]->Fill(nearTotETfrac);
   //hA[47]->Fill(nearTpcPT);
   //hA[48]->Fill(nearEmcET, nearTpcPT);
   //hA[49]->Fill(nearSum);
   //hA[250]->Fill(mCluster2x2.ET, nearTotETfrac);

   //// check east/west yield diff
   //hA[210]->Fill(mCluster2x2.position.PseudoRapidity(), nearEtowET);

   //if (mCluster2x2.position.PseudoRapidity() > 0) hA[211]->Fill(mCluster2x2.position.Phi(), nearEtowET);
   //else                                           hA[212]->Fill(mCluster2x2.position.Phi(), nearEtowET);

   //} else if (mMatchedTower.id < 0) { //only endcap towers

   //   // Correct for double counting of electron track in near cone rarely primTrPT<10 GeV & globPT>10 - handle this here
   //   if (mVec3AtDca.Pt() > mMinETrackPt) nearSum -= mMinETrackPt;
   //   else                                 nearSum -= mVec3AtDca.Pt();

   //   nearTotET        = nearSum;
   //   nearTotET_noEEMC = nearSum - nearEtowET;
   //   float nearTotETfrac    = mCluster2x2.ET / nearTotET;

   //   hE[40]->Fill(nearEmcET);
   //   hE[41]->Fill(mCluster2x2.ET, nearEmcET - mCluster2x2.ET);
   //   hE[42]->Fill(nearTotETfrac);
   //   hE[47]->Fill(nearTpcPT);
   //   hE[48]->Fill(nearEmcET, nearTpcPT);
   //   hE[49]->Fill(nearSum);
   //}
}


void VecBosTrack::FindAwayJet()
{
//   // printf("\n******* find AwayJet() nVert=%d\n", mVecBosEvent->mVertices.size());
//   //mVecBosEvent->print();
//   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++)
//   {
//      VecBosVertex &vertex = mVecBosEvent->mVertices[iv];
//
//      for (uint iTrack = 0; iTrack < vertex.eleTrack.size(); iTrack++)
//      {
//         VecBosTrack &track = vertex.eleTrack[iTrack];
//
//         if (!isMatch2Cl) continue;
//
//         // sum opposite in phi EMC components
//         awayBtowET  = SumBTowCone(vertex.z, -mVec3AtDca, 1); // '1' = only cut on delta phi
//         awayEtowET  = SumETowCone(vertex.z, -mVec3AtDca, 1);
//         awayEmcET   = awayBtowET;
//         awayEmcET  += awayEtowET;
//
//         // add TPC ET
//         //if (mStMuDstMaker)
//            awayTpcPT = SumTpcCone(vertex.id, -mVec3AtDca, 1, mMatchedTower.id);
//         //else
//         //   awayTpcPT = SumTpcConeFromTree(iv, -mVec3AtDca, 1, mMatchedTower.id);
//
//         awayTotET        = awayEmcET  + awayTpcPT;
//         awayTotET_noEEMC = awayBtowET + awayTpcPT;
//
//         //printf("\n*** in   awayTpc=%.1f awayEmc=%.1f\n  ",awayTpcPT,awayEmcET); print();
//      }
//   }
}
