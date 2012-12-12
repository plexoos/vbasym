#include <math.h>

#include "VecBosEvent.h"

#include "utils/utils.h"

ClassImp(VecBosEvent)

using namespace std;


VecBosEvent::VecBosEvent() : ProtoEvent(),
   mStMuDst(0),
   mNumGoodVertices(0), mNumGoodTracks(0), mNumBTracks(0), mNumETracks(0), mNumIsolatedTracks(0),
   mStJets(0), mJets(),
   mVertices(),
   mTracks(),
   mTracksIsolated(),
   mWEvent(0),
   mP4JetTotal(), mP4JetFirst(),
   mP3RecoilFromTracks(),
   mHadronicRecoilEta(0),
   mHadronicRecoilPt(0),
   mPtKfactor(0),
   mMaxTrackClusterDist (7),
   mTrackIsoDeltaR      (0.7),
   mTrackIsoDeltaPhi    (0.7),
   mMinBTrackPt         (10),
   mMinTrackHitFrac     (0.51),
   //mMinClusterEnergyFrac(0.88),
   mMinClusterEnergyFrac(0.80),
   mMaxEnergyInOppsCone (30)
{
   clear();
}


VecBosVertex *VecBosEvent::AddVertex(StMuPrimaryVertex &stMuVertex)
{
   VecBosVertex vbVertex(stMuVertex);
   vbVertex.mEvent = this;
   mVertices.push_back(vbVertex);
   return &mVertices.back();
}


void VecBosEvent::AddVertex(VecBosVertex &vbVertex)
{
   vbVertex.mEvent = this;
   mVertices.push_back(vbVertex);
}


void VecBosEvent::AddTrack(StMuTrack *stMuTrack, VecBosVertex *vbVertex)
{
   //if (vbVertex) Info("AddTrack", "vbVertex != 0");
   //else          Info("AddTrack", "vbVertex == 0");

   StThreeVectorF prPvect = stMuTrack->p();

   VecBosTrack vbTrack;

   vbTrack.mEvent     = this;
   vbTrack.prMuTrack  = stMuTrack;
   vbTrack.glMuTrack  = stMuTrack->globalTrack();
   vbTrack.mVertex    = vbVertex;
   vbTrack.mP3AtDca   = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

   if (vbVertex) {
      vbVertex->prTrList.push_back(stMuTrack);
      vbVertex->eleTrack.push_back(vbTrack);
   }

   mTracks.push_back(vbTrack);
}


void VecBosEvent::AddStJets(StJets *stJets, StJets *stJetsNoEndcap)
{
   mStJets         = stJets;
   mStJetsNoEndcap = stJetsNoEndcap;

   TClonesArray *jets = mStJets->jets();
   TIter         jetsIter(jets);
   jetsIter.Reset();

   while ( StJet *stJet = (StJet *) jetsIter() ) {
      mJets.insert(stJet);
   }
}


TClonesArray *VecBosEvent::GetJets()            { return mStJets         ? mStJets->jets() : 0; }
TClonesArray *VecBosEvent::GetJetsNoEndcap()    { return mStJetsNoEndcap ? mStJetsNoEndcap->jets() : 0; }
UInt_t        VecBosEvent::GetNumJets()         { return mStJets         ? mStJets->nJets() : 0; }
UInt_t        VecBosEvent::GetNumJetsNoEndcap() { return mStJetsNoEndcap ? mStJetsNoEndcap->nJets() : 0; }


UInt_t VecBosEvent::GetNumTracksWithBCluster()
{
   UInt_t nTracks = 0;
   VecBosVertexVecIter iVertex = mVertices.begin();

   for ( ; iVertex != mVertices.end(); ++iVertex) {
      VecBosTrackVecIter iTrack = iVertex->eleTrack.begin();

      for ( ; iTrack != iVertex->eleTrack.end(); ++iTrack) {
         if (iTrack->isMatch2Cl) nTracks++;
      }
   }

   return nTracks;
}


void VecBosEvent::Process()
{
   mMuDstNumVertices = mStMuDst->primaryVertices()->GetEntriesFast();
   mMuDstNumGTracks  = mStMuDst->globalTracks()->GetEntriesFast();
   mMuDstNumPTracks  = mStMuDst->primaryTracks()->GetEntriesFast();
   mMuDstNumOTracks  = mStMuDst->otherTracks()->GetEntriesFast();

   VecBosVertexVecIter iVertex = mVertices.begin();
   for ( ; iVertex != mVertices.end(); ++iVertex) {
      iVertex->Process();

      if (iVertex->IsGood()) mNumGoodVertices++;
   }

   // Process tracks
   VecBosTrackVecIter iTrack = mTracks.begin();
   for ( ; iTrack != mTracks.end(); ++iTrack) {
      iTrack->Process();

      if (iTrack->IsGood())     mNumGoodTracks++;
      if (iTrack->IsBTrack())   mNumBTracks++;
      if (iTrack->IsETrack())   mNumETracks++;
      //if (iTrack->IsIsolated()) mNumIsolatedTracks++;

      // Consider tracks with clusters only
      if (!iTrack->HasCluster()) continue;

      // Find the min distance between the track and the closest jet

      StJetPtrSetConstIter iJet = mJets.begin();
      for ( ; iJet != mJets.end(); ++iJet) {
         StJet *stJet = *iJet;

         Double_t deltaR = stJet->Vect().DeltaR(iTrack->mP3AtDca);
         //Info("Process()", "Iso track found: %f ", deltaR);

         // Find the distance to the closest track to this jet
         if (deltaR < iTrack->mMinDeltaR) iTrack->mMinDeltaR = deltaR;
      }

      if (iTrack->mMinDeltaR > mTrackIsoDeltaR) {
         iTrack->mType |= VecBosTrack::kISOLATED;
         mNumIsolatedTracks++;
         mTracksIsolated.push_back(*iTrack);
      }

      //if (mMinDeltaR > mTrackIsoDeltaR) {
      //   //Info("Process", "recoil accepted: %f > %f", mMinDeltaR, mTrackIsoDeltaR);
      //   //utils::PrintTLorentzVector(*stJet);
      //   //mP4JetRecoil += *stJet;
      //   //utils::PrintTLorentzVector(mP4JetRecoil);
      //} else {
      //   //mJetsIsolated.insert(stJet);
      //}
   }

   // Process jets
   StJetPtrSetConstIter iJet = mJets.begin();
   mP4JetFirst = *iJet ? **iJet : TLorentzVector();

   for ( ; iJet != mJets.end(); ++iJet) {
      StJet *stJet = *iJet;
      //Info("Process", "total jet");
      //utils::PrintTLorentzVector(*stJet);
      mP4JetTotal += *stJet;
      //utils::PrintTLorentzVector(mP4JetTotal);
   }
}


void VecBosEvent::ProcessMC()
{
   //Info("ProcessMC()", "test...");

   StMcEvent *mcEvent = (StMcEvent *) StMaker::GetChain()->GetDataSet("StMcEvent");
   assert(mcEvent);

   mWEvent->CalcRecoil(*mcEvent);
}


void VecBosEvent::CalcRecoilFromTracks()
{
   //loop over tracks with a good vertex
   TVector3 recoil;
   VecBosTrackVecIter iTrack = mTracks.begin();
   for (; iTrack !=  mTracks.end(); ++iTrack)
   {
      //// iTrack->Process();
      if (iTrack->IsGood() == false) continue;      // Track has a good vertex
      if (iTrack->IsIsolated() == true) continue;   // Track is not the electro
      //if(iTrack->HasCluster() == false) continue;  // Track points to a cluster

      TVector3 TrackP3 = iTrack->mP3AtDca;

      //....process TPC tracks

      recoil += TrackP3;
   }

   mP3RecoilFromTracks = recoil;
   mHadronicRecoilEta  = mP3RecoilFromTracks.Eta();
   mHadronicRecoilPt   = mP3RecoilFromTracks.Perp();
}


void VecBosEvent::MCanalysis()
{
   ////CalcRecoilFromTracks();
   // Lets now do the MC analysis running trough the entire selection
   // run through W cuts to fill other histos............
   //   for (uint iv = 0; iv < mVertices.size(); iv++)
   //   {
   //      VecBosVertex &V = mVertices[iv];

   //      for (uint it = 0; it < V.eleTrack.size(); it++)
   //      {
   //         VecBosTrack &T = V.eleTrack[it];
   ////if (T.isMatch2Cl == false) continue;
   //         assert(T.mCluster2x2.nTower > 0); // internal logical error
   //assert(T.nearTotET > 0); // internal logical error

   //if (T.mCluster2x2.ET / T.nearTotET < wMK->par_nearTotEtFrac) continue; // too large nearET
   ////if (T.awayTotET > 30.) continue; // too large awayET , Jan

   //Full W cuts applied at this point

   mHadronicRecoilEta = mP3RecoilFromTracks.Eta();

   //hadronic recoil and correlations with W from pythia
   //         TVector3 hadronicPt(T.hadronicRecoil.X(), T.hadronicRecoil.Y(), 0); //transverse momentum vector

   mHadronicRecoilPt = mP3RecoilFromTracks.Perp();
   //    }
   //   }

   //   mPtKfactor = mWEvent->mRecoilP4.Pt()/mHadronicRecoilPt;
}


/*
void VecBosEvent::addMC()
{
   StMcEvent *mcEvent = (StMcEvent *) StMaker::GetChain()->GetDataSet("StMcEvent");
   assert(mcEvent);

   StMcVertex *V = mcEvent->primaryVertex();
   mVertex = TVector3(V->position().x(), V->position().y(), V->position().z());

   uint i = 1;
   int found = 0;

   while (found < 2 && i < mcEvent->tracks().size())
   { //loop tracks
      StMcTrack *mcTrack = mcEvent->tracks()[i];
      int pdgId = mcTrack->pdgId();
      //float pt=mcTrack->pt();
      //LOG_INFO<<"pdgId "<<pdgId<<" pt "<<pt<<" pz "<<mcTrack->momentum().z()<<endm;

      if (pdgId == 11 || pdgId == -11) //select e+ and e-
      {
         if (abs(mcTrack->parent()->pdgId()) == 24 )
         {
            pElectron = mcTrack->momentum();
            //LOG_INFO<<"pdgId "<<pdgId<<" pt "<<pt<<" pz "<<mcTrack->momentum().z()<<endm;
            pW = mcTrack->parent()->momentum();
            eW = mcTrack->parent()->energy();
            //LOG_INFO<<"pdgId "<<mcTrack->parent()->pdgId()<<" pt "<<mcTrack->parent()->pt()<<" pz "<<mcTrack->parent()->momentum().z()<<endm;
            found++;
         }
      }

      if (pdgId == 12 || pdgId == -12) { //select neutrino
         if (abs(mcTrack->parent()->pdgId()) == 24 ) {
            pNeutrino = mcTrack->momentum();
            //LOG_INFO<<"pdgId "<<pdgId<<" pt "<<pt<<" pz "<<mcTrack->momentum().z()<<endm;
            pW = mcTrack->parent()->momentum();
            eW = mcTrack->parent()->energy();
            //LOG_INFO<<"pdgId "<<mcTrack->parent()->pdgId()<<" pt "<<mcTrack->parent()->pt()<<" pz "<<mcTrack->parent()->momentum().z()<<endm;
            found++;
         }
      }
      i++;
   }

   if (found != 2) return;

   mWP        = TVector3(pW.x(), pW.y(), pW.z());
   mNeutrinoP = TVector3(pNeutrino.x(), pNeutrino.y(), pNeutrino.z());
   mElectronP = TVector3(pElectron.x(), pElectron.y(), pElectron.z());

   TVector3 diff = mWP - mNeutrinoP - mElectronP;

   if (diff.Mag() > 0.0001) //should get exactly right
      LOG_INFO << "\n \n W+e+nu vector sum =" << diff.Mag() << endm;

   if (mElectronP.Mag() < 0.0001)
      LOG_INFO << "\n \n no lepton track =" << endm;

   //calculate x1 and x2 from W rapidity
   rapW = 0.5 * log((eW + mWP.Z()) / (eW - mWP.Z()));
   //float mw2sqs = 80.4 / 500.;
   //float x1 = mw2sqs * exp(rapW);
   //float x2 = mw2sqs * exp(-rapW);
}
*/


/*
void VecBosEvent::McAnalysis()
{
   // run through W cuts to fill other histos............
   for (uint iv = 0; iv < mVertices.size(); iv++)
   {
      VecBosVertex &V = mVertices[iv];

      for (uint it = 0; it < V.eleTrack.size(); it++)
      {
         VecBosTrack &T = V.eleTrack[it];
         if (T.isMatch2Cl == false) continue;
         assert(T.mCluster2x2.nTower > 0); // internal logical error
         assert(T.mP3InNearCone > 0); // internal logical error

         //if (T.mCluster2x2.ET / T.mP3InNearCone < wMK->par_nearTotEtFrac) continue; // too large nearET
         if (T.awayTotET > 30.) continue; // too large awayET , Jan
         //Full W cuts applied at this point

         mHadronicRecoilEta = T.hadronicRecoil.Eta();

         //hadronic recoil and correlations with W from pythia
         TVector3 hadronicPt(T.hadronicRecoil.X(), T.hadronicRecoil.Y(), 0); //transverse momentum vector

         mHadronicRecoilPt = hadronicPt.Perp();
      }
   }
}
*/


/**
 * For a given eta-phi bin considers all combinations of 2x2 bin clusters around it
 * and returns one with the maximum ET.
 */
WeveCluster VecBosEvent::FindMaxBTow2x2(int etaBin, int phiBin, float zVert)
{
   //Info("FindMaxBTow2x2(int etaBin, int phiBin, float zVert)", "seed etaBin=%d phiBin=%d \n",etaBin, phiBin);
   const int L = 2; // size of the summed square

   WeveCluster maxCL;

   // Just 4 cases of 2x2 clusters
   float maxET = 0;

   for (int iEta = etaBin - 1; iEta <= etaBin; iEta++) {
      for (int iPhi = phiBin - 1; iPhi <= phiBin; iPhi++) {
         WeveCluster cluster = SumBTowPatch(iEta, iPhi, L, L, zVert);
         if (maxET > cluster.ET) continue;
         maxET = cluster.ET;
         maxCL = cluster;
         // printf("   newMaxETSum=%.1f etaBin=%d iPhi=%d \n",maxET, I,J);
      }
   }

   //printf(" final inpEve=%d SumET2x2=%.1f \n",nInpEve,maxET);
   return maxCL;
}


WeveCluster VecBosEvent::SumBTowPatch(int etaBin, int phiBin, int etaWidth, int  phiWidth, float zVert)
{
   //printf("eveID=%d btow Square seed etaBin=%d[+%d] phiBin=%d[+%d] zVert=%.0f\n", id, etaBin, etaWidth, phiBin, phiWidth, zVert);
   WeveCluster cluster; // object is small, not to much overhead in creating it
   cluster.iEta = etaBin;
   cluster.iPhi = phiBin;
   TVector3 cluCoord;
   double sumW          = 0;
   float  nomBTowRadius = gBTowGeom->Radius();

   for (int iEta = etaBin; iEta < etaBin + etaWidth; iEta++) { // trim in eta-direction
      if (iEta < 0) continue;
      if (iEta >= mxBTetaBin) continue;

      for (int iPhi = phiBin; iPhi < phiBin + phiWidth; iPhi++) {
         // wrap up in the phi-direction
         int   iPhi_p  = (iPhi + mxBTphiBin) % mxBTphiBin;         // keep it always positive
         int   towerId = gMapBTowEtaPhiBin2Id[ iEta + iPhi_p * mxBTetaBin];
         float energy  = bemc.eneTile[kBTow][towerId - 1];

         //if (L<5) printf("n=%2d  iEta=%d iPhi_p=%d\n",cluster.nTower,iEta,iPhi_p);

         if (energy <= 0) continue; // skip towers w/o energy

         float adc    = bemc.adcTile[kBTow][towerId - 1];
         float delZ   = gBCalTowerCoords[towerId - 1].z() - zVert;
         float cosine = nomBTowRadius / sqrt(nomBTowRadius * nomBTowRadius + delZ * delZ);
         float ET     = energy * cosine;
         float logET  = log10(ET + 0.5);

         cluster.nTower++;
         cluster.energy += energy;
         cluster.ET     += ET;
         cluster.adcSum += adc;

         if (logET > 0) {
            cluCoord += logET * gBCalTowerCoords[towerId - 1]; // (log) energy weighted cluster position
            sumW     += logET;
         }
         // if(etaWidth==2)
         //    printf("etaBin=%d phiBin=%d  ET=%.1f  energy=%.1f   sum=%.1f logET=%f sumW=%f\n",iEta,iPhi,ET,energy,cluster.energy,logET,sumW);
      }

      // printf(" end btowSquare: etaBin=%d  nTw=%d, ET=%.1f adc=%.1f\n",iEta,cluster.nTower,cluster.ET,cluster.adcSum);
      if (sumW > 0)
         cluster.position = (1. / sumW) * cluCoord; // weighted cluster position
      else
         cluster.position = TVector3(0, 0, 999);
   }

   return cluster;
}


TVector3 VecBosEvent::CalcP3InConeTpc(VecBosTrack *vbTrack, UShort_t cone1d2d, Float_t scale)
{
   TVector3 totalP3InCone;

   if (!vbTrack) return totalP3InCone;

   // Scale P3 momentum of the track
   TVector3 trackP3 = vbTrack->mP3AtDca * scale;

   VecBosTrackVecIter iTrack = mTracks.begin();

   for ( ; iTrack != mTracks.end(); ++iTrack) {
      // Skip tracks from different vertices XXX:ds: Later can consider
      // vertices in some close proximity to this one
      if (iTrack->mVertex != vbTrack->mVertex) continue;

      // Don't count the same track
      //if (&*iTrack == vbTrack) continue;
      // XXX:ds: move this requirement to the track class where appropriate
      //if (iTrack->GetFitHitFrac() < mMinTrackHitFrac) continue;

      if (cone1d2d == 1 && fabs(trackP3.DeltaPhi(iTrack->mP3AtDca)) > mTrackIsoDeltaPhi) continue;
      if (cone1d2d == 2) {
         if (trackP3.DeltaR(iTrack->mP3AtDca) > mTrackIsoDeltaR) continue;
         // Count the number of tracks in the cone
         vbTrack->mNumTracksInNearCone++;
      }

      totalP3InCone += iTrack->mP3AtDca;
   }

   return totalP3InCone;
}


/**
 * Returns the  sum of all towers withing mTrackIsoDeltaR around the track
 * refAxis.
 * flag = 1: only delta phi cut; used for away (out of) cone cut
 * flag = 2: use 2D cut;         used for near cone cut
 */
TVector3 VecBosEvent::CalcP3InConeBTow(VecBosTrack *vbTrack, UShort_t cone1d2d, Float_t scale)
{
   TVector3 totalP3InCone;

   if (!vbTrack) return totalP3InCone;

   // Scale P3 momentum of the track
   TVector3 trackP3 = vbTrack->mP3AtDca * scale;

   // process BTOW hits
   for (int iBTow = 0; iBTow < mxBtow; iBTow++) {
      float energy = bemc.eneTile[kBTow][iBTow];
      if (energy <= 0) continue;

      // Correct BCal tower position to the vertex position
      TVector3 towerCoord = gBCalTowerCoords[iBTow] - vbTrack->mVertex->mPosition;
      towerCoord.SetMag(energy); // it is 3D momentum in the event ref frame

      if (cone1d2d == 1 && fabs(trackP3.DeltaPhi(towerCoord)) > mTrackIsoDeltaPhi) continue;
      if (cone1d2d == 2 &&      trackP3.DeltaR(towerCoord)    > mTrackIsoDeltaR)   continue;

      // XXX:ds: Another bug? The sum should be vector one
      //totalP3InCone += towerCoord.Perp();
      totalP3InCone += towerCoord;
   }

   return totalP3InCone;
}


/**
 * flag = 1: only delta phi cut; used for away (out of) cone cut
 * flag = 2: use 2D cut;         used for near cone cut
 */
TVector3 VecBosEvent::CalcP3InConeETow(VecBosTrack *vbTrack, UShort_t cone1d2d, Float_t scale)
{
   TVector3 totalP3InCone;

   if (!vbTrack) return totalP3InCone;

   // Scale P3 momentum of the track
   TVector3 trackP3 = vbTrack->mP3AtDca * scale;

   // Loop over all phi bins
   for (int iphi = 0; iphi < mxEtowPhiBin; iphi++) {
      for (int ieta = 0; ieta < mxEtowEta; ieta++) { // sum all eta rings
         float energy = etow.ene[iphi][ieta];
         if (energy <= 0) continue; // skip towers with no energy

         TVector3 towerCoord = gETowCoords[iphi][ieta] - vbTrack->mVertex->mPosition;
         towerCoord.SetMag(energy); // it is 3D momentum in the event ref frame

         if (cone1d2d == 1 && fabs(trackP3.DeltaPhi(towerCoord)) > mTrackIsoDeltaPhi) continue;
         if (cone1d2d == 2 &&      trackP3.DeltaR(towerCoord)    > mTrackIsoDeltaR)   continue;

         // XXX:ds: Another bug? The sum should be vector one
         //ptsum += towerCoord.Perp();
         totalP3InCone += towerCoord;
      }
   }

   return totalP3InCone;
}


/**
 * Returns the sum of all track P_T's inside the cone around the refAxis track.
 *
 * flag=1: only delta phi cut; used for away (out of) cone cut
 * flag=2: use 2D cut;         used for near cone cut
 *
 *
 * XXX:ds: Do we want to count the refAxis P_T?
 * The calculated sum is scalar. We probably want a vector sum.
 */
/*
float VecBosEvent::SumTpcCone(int vertID, TVector3 refAxis, int flag, int pointTowId)
{
   // printf("******* SumTpcCone, flag=%d eveId=%d vertID=%d  eta0=%.2f phi0/rad=%.2f  \n",flag,mVecBosEvent->id,vertID,refAxis.PseudoRapidity() ,refAxis.Phi());
   assert(vertID >= 0);
   assert(vertID < (int) mStMuDstMaker->muDst()->numberOfPrimaryVertices());

   StMuPrimaryVertex *vertex = mStMuDstMaker->muDst()->primaryVertex(vertID);
   assert(vertex);

   // Select current vertex
   mStMuDstMaker->muDst()->setVertexIndex(vertID);

   float rank = vertex->ranking();
   // XXX:ds: assert(rank > 0 || (rank < 0 && vertex->nEEMCMatch()));
   if (rank <= 0 && vertex->nEEMCMatch() <= 0)  return -1;

   double ptSum = 0;
   Int_t nPrimaryTracks = mStMuDstMaker->muDst()->GetNPrimaryTrack();

   for (int iTrack = 0; iTrack < nPrimaryTracks; iTrack++)
   {
      StMuTrack *stMuTrack = mStMuDstMaker->muDst()->primaryTracks(iTrack);

      if (stMuTrack->flag() <= 0) continue;
      if (stMuTrack->flag() != 301 && pointTowId > 0) continue; // TPC-only regular tracks for barrel candidate
      if (stMuTrack->flag() != 301 && stMuTrack->flag() != 311 && pointTowId < 0) continue; // TPC regular and short EEMC tracks for endcap candidate

      float hitFrac = float(stMuTrack->nHitsFit()) / stMuTrack->nHitsPoss();

      if (hitFrac < mMinTrackHitFrac) continue;

      StThreeVectorF prPvect = stMuTrack->p();
      TVector3 vec3AtDca = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

      // printf(" prTrID=%4d  prTrEta=%.3f prTrPhi/deg=%.1f prPT=%.1f  nFitPts=%d\n", stMuTrack->id(),stMuTrack->eta(),stMuTrack->phi()/3.1416*180.,stMuTrack->pt(),stMuTrack->nHitsFit());
      if (flag == 1 && fabs(refAxis.DeltaPhi(vec3AtDca)) > mVecBosEvent->mTrackIsoDeltaPhi) continue;
      if (flag == 2 &&      refAxis.DeltaR(vec3AtDca)    > mVecBosEvent->mTrackIsoDeltaR)   continue;

      float pT = stMuTrack->pt();

      // XXX:ds: Another bug? The sum should be vector one
      // separate quench for barrel and endcap candidates
      if      (pT > mVecBosEvent->mMinBTrackPt && pointTowId > 0) ptSum += mVecBosEvent->mMinBTrackPt;
      else if (pT > mMinETrackPt && pointTowId < 0)               ptSum += mMinETrackPt;
      else  ptSum += pT;
   }

   return ptSum;
}
*/


void VecBosEvent::clear()
{
   //Info("clear", "");
   mStMuDst           = 0;
   id                 = 0;
   runNo              = 0;
   time               = 0;
   zdcRate            = 0;
   l2bitET            = 0;
   l2bitRnd           = 0;
   l2EbitET           = 0;
   l2EbitRnd          = 0;
   bx7                = -1;
   bx48               = -1;
   zTag               = false;
   mMuDstNumVertices  = 0;
   mMuDstNumGTracks   = 0;
   mMuDstNumPTracks   = 0;
   mMuDstNumOTracks   = 0;
   mNumGoodVertices   = 0;
   mNumGoodTracks     = 0;
   mNumBTracks        = 0;
   mNumETracks        = 0;
   mNumIsolatedTracks = 0;
   mStJets            = 0;
   bxStar7            = -1;
   bxStar48           = -1;
   spin4              = -1;
   bemc.clear();
   etow.clear();
   eprs.clear();
   esmd.clear();
   mJets.clear();
   mVertices.clear();
   mTracks.clear();
   mTracksIsolated.clear();
   if (mWEvent) delete mWEvent;
   mWEvent               = new WEvent();
   mP4JetTotal           = TLorentzVector();
   mP4JetFirst           = TLorentzVector();

   mMaxTrackClusterDist  = 7;    // cm
   mTrackIsoDeltaR       = 0.7;  // (rad) near-cone size
   mTrackIsoDeltaPhi     = 0.7;  // (rad) away-'cone' size, approx. 40 deg.
   mMinBTrackPt          = 10.;  // GeV
   mMinTrackHitFrac      = 0.51;
   //mMinClusterEnergyFrac = 0.88;
   mMinClusterEnergyFrac = 0.80;
   mMaxEnergyInOppsCone  = 30;   // GeV
   mP3RecoilFromTracks   = TVector3(0, 0, 0);
   mPtKfactor            = 0;
}


void VecBosEvent::Print(int opt, int isMC)
{
   Info("Print(int opt, int isMC)", "");

   printf("\nmy W2011event runNo=%d ID=%d  L2Wbits: ET=%d rnd=%d;  muDst: bx7=%d bx48=%d nVert=%d star: Bx7m=%d, Bx48=%d, spin4=%d \n", runNo, id, l2bitET, l2bitRnd, bx7, bx48, mVertices.size(), bxStar7, bxStar48, spin4);
   int  yyyymmdd,  hhmmss; getGmt_day_hour( yyyymmdd,  hhmmss);
   printf("  event time is: day=%d, hour=%d (GMT)\n", yyyymmdd, hhmmss);

   for (uint i = 0; i < mVertices.size(); i++) mVertices[i].Print();
   bemc.print(opt);
}


void VecBosEvent::getGmt_day_hour(int &yyyymmdd, int &hhmmss)
{
   time_t rawtime = this->time;
   struct tm *timeinfo = gmtime ( &rawtime );
   char buffer [80];
   strftime (buffer, 80, "%k%M%S", timeinfo);
   //puts (buffer);
   hhmmss = atoi(buffer);
   strftime (buffer, 80, "%G%m%d", timeinfo);
   //puts (buffer);
   yyyymmdd = atoi(buffer);
   //printf("day=%d, hour=%d\n",yyyymmdd,hhmmss);
}
