
#include "VecBosTrack.h"

#include "VecBosEvent.h"
#include "VecBosVertex.h"
#include "Globals.h"


ClassImp(VecBosTrack)

using namespace std;


VecBosTrack::VecBosTrack() : TObject(), mEvent(0), mType(kUNKNOWN), mHelix(), mVertex(0), mStJet(0),
   mP3AtDca(), mP3AtBTow(), mCoorAtBTow(),
   mP3InNearCone(), mP3InNearConeTow(), mP3InNearConeBTow(), mP3InNearConeETow(), mP3InNearConeNoETow(), mP3InNearConeTpc(),
   mP3InOppsCone(), mP3InOppsConeTow(), mP3InOppsConeBTow(), mP3InOppsConeETow(), mP3InOppsConeNoETow(), mP3InOppsConeTpc(),
   awayTpcPT       (0),
   awayEmcET       (0),
   awayBtowET      (0),
   awayEtowET      (0),
   awayTotET       (0),
   smallNearTpcPT  (0),
   awayTotET_noEEMC(0),
   mNumTracksInNearCone(0),
   ptBalance(), ptBalance_noEEMC(), sPtBalance(0), sPtBalance_noEEMC(0), hadronicRecoil(),
   mMinDeltaRToJet(-1) // nonsense value
{
   clear();
}


const float VecBosTrack::mMinPt = 20;
const float VecBosTrack::mMaxEnergyInOppsCone = 40; // was 30 GeV


//bool VecBosTrack::IsCandidate() const { return (IsUnBalanced() && !IsInJet() && mP3AtDca.Pt() >= mMinPt); }
//bool VecBosTrack::IsCandidate() const { return (IsUnBalanced() && mP3AtDca.Pt() >= mMinPt); }
bool VecBosTrack::IsCandidate() const { return (IsUnBalanced() && mCluster2x2.energy >= 20); }


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
      CalcEnergyInNearCone();
   }
   else if ( prMuTrack->pt() >= 1.0 && prMuTrack->flag() == 311 )
   {
      mType |= kENDCAP;
      //ExtendTrack2Endcap();
   }
}


StJet* VecBosTrack::FindClosestJet(StJetPtrSet &jets)
{
   StJet *closestJet = 0;

   // Find the min distance between the track and the closest jet
   StJetPtrSetConstIter iJet = jets.begin();
   for ( ; iJet != jets.end(); ++iJet)
   {
      StJet *stJet = *iJet;

      Double_t deltaR = stJet->Vect().DeltaR(mP3AtDca);
      //Info("FindClosestJet()", "deltaR: %f ", deltaR);

      if (mMinDeltaRToJet < 0 || deltaR < mMinDeltaRToJet) {
         mMinDeltaRToJet = deltaR;
         closestJet = stJet;
         //Info("FindClosestJet()", "New closest jet found: mMinDeltaRToJet = %f", mMinDeltaRToJet);
      }
   }

   if (closestJet && mMinDeltaRToJet <= mEvent->mTrackIsoDeltaR)
   {
      //Info("FindClosestJet()", "Track is within jet: %f <= %f. Returning jet...", mMinDeltaRToJet, mEvent->mTrackIsoDeltaR);
      mType |= VecBosTrack::kIN_JET;
      mStJet = closestJet;
      return closestJet;
   }
   
   return 0;
}


void VecBosTrack::clear()
{
   mEvent                 = 0;
   mType                  = kUNKNOWN;
   isMatch2Cl             = false;
   mMatchedTower.clear();
   glMuTrack              = 0;
   prMuTrack              = 0;
   //mHelix               = St
   mVertex                = 0;
   mStJet                 = 0;
   mCluster2x2.clear();
   mCluster4x4.clear();
   mP3AtDca               = TVector3(0, 0, 0);
   mP3AtBTow              = TVector3(0, 0, 0);
   mCoorAtBTow            = TVector3(0, 0, 0);
   mP3InNearCone          = TVector3(0, 0, 0);
   mP3InNearConeTow       = TVector3(0, 0, 0);
   mP3InNearConeBTow      = TVector3(0, 0, 0);
   mP3InNearConeETow      = TVector3(0, 0, 0);
   mP3InNearConeNoETow    = TVector3(0, 0, 0);
   mP3InNearConeTpc       = TVector3(0, 0, 0);
   mP3InOppsCone          = TVector3(0, 0, 0);
   mP3InOppsConeTow       = TVector3(0, 0, 0);
   mP3InOppsConeBTow      = TVector3(0, 0, 0);
   mP3InOppsConeETow      = TVector3(0, 0, 0);
   mP3InOppsConeNoETow    = TVector3(0, 0, 0);
   mP3InOppsConeTpc       = TVector3(0, 0, 0);
   awayTpcPT              = 0;
   awayEmcET              = 0;
   awayBtowET             = 0;
   awayEtowET             = 0;
   awayTotET              = 0;
   smallNearTpcPT         = 0;
   awayTotET_noEEMC       = 0;
   mNumTracksInNearCone   = 0;
   ptBalance              = TVector3(0, 0, 0);
   ptBalance_noEEMC       = TVector3(0, 0, 0);
   sPtBalance             = 0;
   sPtBalance_noEEMC      = 0;
   hadronicRecoil         = TVector3(0, 0, 0);
   mMinDeltaRToJet        = -1;

   memset(esmdGlobStrip, -999, sizeof(esmdGlobStrip));
   memset(esmdDca, -999., sizeof(esmdDca));
   memset(esmdDcaGlob, -999., sizeof(esmdDcaGlob));
   memset(esmdE, 0., sizeof(esmdE));
   memset(esmdNhit, 0, sizeof(esmdNhit));
   memset(esmdShowerCentroid, 999., sizeof(esmdShowerCentroid));
   memset(esmdShowerWidth, 999., sizeof(esmdShowerWidth));

   esmdXPcentroid = TVector3(0, 0, 0);
}


void VecBosTrack::print(int opt) const
{
   if (prMuTrack == 0) {  printf("   Track NULL pointer???\n"); return; }

   printf("Track glPT=%.1f GeV/c   isMatch2Cl=%d, mP3InNearCone=%.1f, awayTotET=%.1f mP3AtDcaT=%.1f\n",
          glMuTrack->pt(), isMatch2Cl, mP3InNearCone.Pt(), awayTotET, mP3AtDca.Pt());

   mMatchedTower.print(opt);
   mCluster2x2.print(opt);

   TVector3 D = mMatchedTower.R - mCluster2x2.position;

   printf("     XYZ(track-mCluster2x2):  |3D dist|=%.1fcm  delZ=%.1fcm\n", D.Mag(), D.z());
   printf("     4x4 :");
   mCluster4x4.print(opt);
   printf("     nearET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n",
      mP3InNearConeTpc.Pt(), mP3InNearConeTow.Pt(), mP3InNearConeBTow.Pt(), mP3InNearConeETow.Pt(), mP3InNearCone.Pt());
   printf("     awayET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n",
      awayTpcPT, awayEmcET, awayBtowET, awayEtowET, awayTotET);
}


/** */
bool VecBosTrack::ExtendTrack2Barrel()
{
   //printf("******* extendTracks() nVert=%d\n", mVecBosEvent->mVertices.size());
   //if (!mVecBosEvent->l2bitET) return; //fire barrel trigger

   // Apply eta cuts at track level (tree analysis)
   //if (mP3AtDca.Eta() < mMinBTrackEta || mP3AtDca.Eta() > mMaxBTrackEta) continue;

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
   //mP3AtDca.Print();
   //mCoorAtBTow.Print();

   int iEta, iPhi;

   if ( !ConvertEtaPhi2Bins(etaAtBTow, phiAtBTow, iEta, iPhi) ) return false;

   //printf(" phiAtBTow=%.0f deg,  etaAtBTow=%.2f, iEta=%d, iPhi=%d\n", posCTB.phi()/3.1416*180., posCTB. pseudoRapidity(),iEta, iPhi);
   // printf("hit Tower ID=%d\n",towerId);

   mMatchedTower.id   = gMapBTowEtaPhiBin2Id[ iEta + iPhi * mxBTetaBin];
   mMatchedTower.R    = mCoorAtBTow; //TVector3(posAtBTow.x(), posAtBTow.y(), posAtBTow.z());
   mMatchedTower.iEta = iEta;
   mMatchedTower.iPhi = iPhi;
   //Print();

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
   mCluster2x2 = mEvent->FindMaxBTow2x2(mMatchedTower.iEta, mMatchedTower.iPhi, mVertex->z);

   //hA[33] ->Fill(mCluster2x2.ET);
   //hA[34] ->Fill(mCluster2x2.adcSum, trackPT);
   //hA[110]->Fill(mCluster2x2.ET);

   // Compute surroinding cluster energy
   // 4x4 cluster lower left tower
   mCluster4x4 = mEvent->SumBTowPatch(mCluster2x2.iEta - 1, mCluster2x2.iPhi - 1, 4, 4, mVertex->z); // needed for lumi monitor

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
   }

   //hA[20]->Fill("#Delta R", 1.);
   //hA[111]->Fill(mCluster2x2.ET);
   //hA[208]->Fill(mCluster2x2.position.PseudoRapidity(), mCluster2x2.ET);
   //hA[0]->Fill("Tr2Cl", 1.0);
}


/**
 * Calculates the energy in the cone around the track+cluster (electron
 * candidate).
 */
void VecBosTrack::CalcEnergyInNearCone()
{
   //Info("CalcEnergyInNearCone()", "nVert=%d\n",mVecBosEvent->mVertices.size());

   if (!HasCluster()) return;

   // sum EMC-jet component. XXX:ds: Note: the track direction is taken at the origin
   mP3InNearConeBTow   = mEvent->CalcP3InConeBTow(this, 2); // '2'=2D cone
   mP3InNearConeETow   = mEvent->CalcP3InConeETow(this, 2); // '2'=2D cone
   mP3InNearConeTpc    = mEvent->CalcP3InConeTpc (this, 2); // '2'=2D cone

   mP3InNearConeTow    = mP3InNearConeBTow + mP3InNearConeETow;
   mP3InNearCone       = mP3InNearConeBTow + mP3InNearConeETow + mP3InNearConeTpc; // XXX:ds: double counting? yes, see correction below
   mP3InNearConeNoETow = mP3InNearConeBTow                     + mP3InNearConeTpc;

   mP3InOppsConeBTow   = mEvent->CalcP3InConeBTow(this, 1, -1); // '1'=1D cone
   mP3InOppsConeETow   = mEvent->CalcP3InConeETow(this, 1, -1); // '1'=1D cone
   mP3InOppsConeTpc    = mEvent->CalcP3InConeTpc (this, 1, -1); // '1'=1D cone

   mP3InOppsConeTow    = mP3InOppsConeBTow + mP3InOppsConeETow;
   mP3InOppsCone       = mP3InOppsConeTow  + mP3InOppsConeTpc; // XXX:ds: double counting? yes, see correction below
   mP3InOppsConeNoETow = mP3InOppsCone     - mP3InOppsConeETow;

   if (GetClusterEnergyFrac() >= mEvent->mMinClusterEnergyFrac)
   {
      mType |= kISOLATED;

      if ( mP3InOppsConeTow.Mag() < mMaxEnergyInOppsCone )
      {
         mType |= kUNBALANCED;
      }
   }
   // else {
      //mEvent->mTracksCandidate.push_back(this);
   //}
   
   // sum TPC-near component
   //if (mStMuDstMaker)
   //else
   //   mP3InNearConeTpc = SumTpcConeFromTree(iv, mP3AtDca, 2, mMatchedTower.id);

   // fill histos separately for 2 types of events
   //if (mMatchedTower.id > 0) { //only barrel towers

   // Correct for double counting of electron track in near cone rarely
   // primTrPT<10 GeV & globPT>10 - handle this here
   //if (mP3AtDca.Pt() > mEvent->mMinBTrackPt) mP3InNearCone -= mEvent->mMinBTrackPt;
   //else                                      mP3InNearCone -= mP3AtDca.Pt();

   //float nearTotETfrac = mCluster2x2.ET / mP3InNearCone;

   //hA[40]->Fill(mP3InNearConeTow);
   //hA[41]->Fill(mCluster2x2.ET, mP3InNearConeTow - mCluster2x2.ET);
   //hA[42]->Fill(nearTotETfrac);
   //hA[47]->Fill(mP3InNearConeTpc);
   //hA[48]->Fill(mP3InNearConeTow, mP3InNearConeTpc);
   //hA[49]->Fill(mP3InNearCone);
   //hA[250]->Fill(mCluster2x2.ET, nearTotETfrac);

   //// check east/west yield diff
   //hA[210]->Fill(mCluster2x2.position.PseudoRapidity(), mP3InNearConeETow);

   //if (mCluster2x2.position.PseudoRapidity() > 0) hA[211]->Fill(mCluster2x2.position.Phi(), mP3InNearConeETow);
   //else                                           hA[212]->Fill(mCluster2x2.position.Phi(), mP3InNearConeETow);

   //} else if (mMatchedTower.id < 0) { //only endcap towers

   //   // Correct for double counting of electron track in near cone rarely primTrPT<10 GeV & globPT>10 - handle this here
   //   if (mP3AtDca.Pt() > mMinETrackPt) mP3InNearCone -= mMinETrackPt;
   //   else                              mP3InNearCone -= mP3AtDca.Pt();

   //   mP3InNearCone       = mP3InNearCone;
   //   mP3InNearConeNoETow = mP3InNearCone - mP3InNearConeETow;
   //   float nearTotETfrac = mCluster2x2.ET / mP3InNearCone;

   //   hE[40]->Fill(mP3InNearConeTow);
   //   hE[41]->Fill(mCluster2x2.ET, mP3InNearConeTow - mCluster2x2.ET);
   //   hE[42]->Fill(nearTotETfrac);
   //   hE[47]->Fill(mP3InNearConeTpc);
   //   hE[48]->Fill(mP3InNearConeTow, mP3InNearConeTpc);
   //   hE[49]->Fill(mP3InNearCone);
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
//         awayBtowET  = mEvent->CalcP3InConeBTow(vertex.z, -mP3AtDca, 1); // '1' = only cut on delta phi
//         awayEtowET  = mEvent->CalcP3InConeETow(vertex.z, -mP3AtDca, 1);
//         awayEmcET   = awayBtowET;
//         awayEmcET  += awayEtowET;
//
//         // add TPC ET
//         //if (mStMuDstMaker)
//            awayTpcPT = mEvent->CalcP3InConeTpc(vertex.id, -mP3AtDca, 1, mMatchedTower.id);
//         //else
//         //   awayTpcPT = SumTpcConeFromTree(iv, -mP3AtDca, 1, mMatchedTower.id);
//
//         awayTotET        = awayEmcET  + awayTpcPT;
//         awayTotET_noEEMC = awayBtowET + awayTpcPT;
//
//         //printf("\n*** in   awayTpc=%.1f awayEmc=%.1f\n  ",awayTpcPT,awayEmcET); print();
//      }
//   }
}
