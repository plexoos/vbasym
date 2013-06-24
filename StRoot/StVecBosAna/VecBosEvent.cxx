#include <math.h>

#include "VecBosEvent.h"

#include "utils/utils.h"

ClassImp(VecBosEvent)

using namespace std;


VecBosEvent::VecBosEvent() : ProtoEvent(),
   mStMuDst(0),
   mSpinPattern4Bits(-1),
   mSpinDirection(-1),
   mCpuTimeEventAna(0), mCpuTimeHistFill(0),
   mMuDstNumGTracks(0), mMuDstNumVertices(0), mMuDstNumPTracks(0), mMuDstNumOTracks(0),
   mNumGoodVertices(0), mNumGoodTracks(0), mNumBTracks(0), mNumETracks(0), mNumIsolatedTracks(0), mNumCandidateTracks(0),
   mStJets(0), mJets(), mJetsRecoil(), mJetsWithIsoTrack(),
   mVertices(),
   mTracks(),
   mTracksCandidate(),
   mWEvent(0),
   mP4JetTotal(), mP4JetFirst(), mP4JetRecoil(), mP3TrackRecoilTpc(), mP3TrackRecoilTow(),
   mP3TrackRecoilNeutrals(), mP3TrackRecoilTpcNeutrals(),
   mP3RecoilFromTracks(),
   mPtKfactor(0), mPtTrackRecoilWithNeutralsCorrected(0),
   mMinVertexDeltaZ(-1),
   mP3BalanceFromTracks(),
   mCandElecP3AtDca(),
   mCandElecP3EScaled(),
   mBalanceDeltaPhiFromTracks(0),
   mLumiEff(0)
{
   //clear();
}


const float VecBosEvent::sMinTrackIsoDeltaR    = 0.7;  // was 0.7
const float VecBosEvent::sMinTrackIsoDeltaPhi  = 0.7;
const float VecBosEvent::sMaxVertexJetDeltaZ   = 1;    // distance between jet and track z coord, cm
const float VecBosEvent::sMaxTrackJetDeltaZ    = 3;    // distance between jet and track z coord, cm
const float VecBosEvent::sMinBTrackPt          = 10;
const float VecBosEvent::sMinTrackHitFrac      = 0.51;
const float VecBosEvent::sMinClusterEnergyFrac = 0.90; // was 0.88


VecBosEvent::~VecBosEvent()
{
   //Info("~VecBosEvent()", "this: %x", this);

   while (!mJets.empty()) delete *mJets.begin(), mJets.erase(mJets.begin());
   //mJets.clear(); // unnecessary?
   mJetsRecoil.clear();
   mJetsWithIsoTrack.clear();

   while (!mTracks.empty()) delete *mTracks.begin(), mTracks.erase(mTracks.begin());
   //mTracks.clear(); // unnecessary?
   mTracksCandidate.clear();

   while (!mVertices.empty()) delete *mVertices.begin(), mVertices.erase(mVertices.begin());
   //mVertices.clear(); // unnecessary?

   if (mWEvent) delete mWEvent;
   mWEvent = 0;
}


VecBosVertex *VecBosEvent::AddVertex(StMuPrimaryVertex &stMuVertex)
{
   VecBosVertex *vbVertex = new VecBosVertex(stMuVertex);
   vbVertex->mEvent = this;
   mVertices.insert(vbVertex);
   return vbVertex;
}


void VecBosEvent::AddVertex(VecBosVertex *vbVertex)
{
   vbVertex->mEvent = this;
   mVertices.insert(vbVertex);
}


void VecBosEvent::AddTrack(StMuTrack *stMuTrack, VecBosVertex *vbVertex)
{
   //if (vbVertex) Info("AddTrack", "vbVertex != 0");
   //else          Info("AddTrack", "vbVertex == 0");

   VecBosTrack *vbTrack = new VecBosTrack();

   vbTrack->mEvent     = this;
   vbTrack->prMuTrack = stMuTrack;
   vbTrack->glMuTrack  = stMuTrack->globalTrack();
   vbTrack->mVertex    = vbVertex;
   vbTrack->mP3AtDca   = TVector3(stMuTrack->p().x(), stMuTrack->p().y(), stMuTrack->p().z());

   mTracks.insert(vbTrack);

   if (vbVertex) {
      vbVertex->prTrList.push_back(stMuTrack);
      //vbVertex->eleTrack.push_back(*vbTrack);
      vbVertex->mTracks.insert(vbTrack);
   }
}


void VecBosEvent::AddStJets(StJets *stJets, StJets *stJetsNoEndcap)
{
   mStJets         = stJets;
   mStJetsNoEndcap = stJetsNoEndcap;

   TClonesArray *jets = mStJets->jets();
   TIter         jetsIter(jets);
   jetsIter.Reset();

   while ( StJet *stJet = (StJet *) jetsIter() )
   {
      VecBosJet *vbJet = new VecBosJet(*stJet);
      vbJet->mEvent = this;
      mJets.insert(vbJet);
   }
}


TClonesArray *VecBosEvent::GetStJets()            { return mStJets         ? mStJets->jets() : 0; }
TClonesArray *VecBosEvent::GetStJetsNoEndcap()    { return mStJetsNoEndcap ? mStJetsNoEndcap->jets() : 0; }
UInt_t        VecBosEvent::GetNumStJets()         { return mStJets         ? mStJets->nJets() : 0; }
UInt_t        VecBosEvent::GetNumStJetsNoEndcap() { return mStJetsNoEndcap ? mStJetsNoEndcap->nJets() : 0; }


UInt_t VecBosEvent::GetNumTracksWithBCluster()
{
   UInt_t nTracks = 0;

   VecBosVertexPtrSetIter iVertex = mVertices.begin();
   for ( ; iVertex != mVertices.end(); ++iVertex) {
      VecBosVertex       &vertex = **iVertex;
      VecBosTrackVecIter  iTrack = vertex.eleTrack.begin();

      for ( ; iTrack != vertex.eleTrack.end(); ++iTrack) {
         if (iTrack->isMatch2Cl) nTracks++;
      }
   }

   return nTracks;
}


bool VecBosEvent::PassedCutExceptPtBal() const
{
   if ( mTracksCandidate.size() > 0 &&
        (*mTracksCandidate.begin())->GetP3EScaled().Pt() >= VecBosTrack::sMinCandidateTrackClusterE)
   {
      return true;
   }

   return false;
}


bool VecBosEvent::PassedCutFinal() const
{
   //if (mTracksCandidate.size() > 0 && GetMissingEnergy().Pt() > 18
   if ( mTracksCandidate.size() > 0 && mPtBalanceCosPhiFromTracks > 18 &&
        (*mTracksCandidate.begin())->GetP3EScaled().Pt() >= VecBosTrack::sMinCandidateTrackClusterE)
   {
      return true;
   }

   return false;
}


void VecBosEvent::Process()
{
   mMuDstNumVertices = mStMuDst->primaryVertices()->GetEntriesFast();
   mMuDstNumGTracks  = mStMuDst->globalTracks()->GetEntriesFast();
   mMuDstNumPTracks  = mStMuDst->primaryTracks()->GetEntriesFast();
   mMuDstNumOTracks  = mStMuDst->otherTracks()->GetEntriesFast();

   UShort_t vertexId = 0;

   VecBosVertexPtrSetIter iVertex = mVertices.begin();
   for ( ; iVertex != mVertices.end(); ++iVertex, vertexId++)
   {
      VecBosVertex &vertex = **iVertex;
      vertex.Process();
      vertex.mId = vertexId;

      if ( !vertex.IsGood()) continue;

      mNumGoodVertices++;

      VecBosVertexPtrSetIter iVertex2 = iVertex; // initialize with the current one
      ++iVertex2; // advance to the next one
      for ( ; iVertex2 != mVertices.end(); ++iVertex2)
      {
         VecBosVertex &vertex2 = **iVertex2;
         if ( !vertex2.IsGood()) continue;

         Double_t deltaZ = fabs(vertex.mPosition.Z() - vertex2.mPosition.Z());

         if (deltaZ < mMinVertexDeltaZ || mMinVertexDeltaZ < 0)
            mMinVertexDeltaZ = deltaZ;
      }
   }

   // Process tracks
   //Info("Process", "Process tracks");
   VecBosTrackPtrSetIter iTrack = mTracks.begin();
   for ( ; iTrack != mTracks.end(); ++iTrack)
   {
      VecBosTrack &track = **iTrack;
      track.Process();

      if (track.IsGood())   mNumGoodTracks++;
      if (track.IsBTrack()) mNumBTracks++;
      if (track.IsETrack()) mNumETracks++;
      if (track.IsIsolated())
      {
         //Info("Process()", "Iso track found");
         mNumIsolatedTracks++;

         if ( track.IsUnBalanced() ) track.FindClosestJet(mJets);
      }

      if ( track.IsCandidate() ) {
         mNumCandidateTracks++;
         mTracksCandidate.insert(*iTrack);

         if ( track.IsInJet() ) {
            //Info("Process()", "Track is a candidate and within jet: %f <= %f. Saving jet...", track.mMinDeltaRToJet, sMinTrackIsoDeltaR);
            //utils::PrintTLorentzVector((TLorentzVector&) *track->mJet);
            mJetsWithIsoTrack.insert(track.mJet);
         }
      }
   }

   ProcessJets();
   CalcRecoilFromTracks();


   // Calculate the Pt balance as the vector sum: pt elec + pt recoil
   if  (mTracksCandidate.size() == 1)
   {

      // Correct the recoil via MC 

     //      mPtTrackRecoilWithNeutralsCorrected = mP3TrackRecoilTpcNeutrals.Pt() * (0.196 + 0.4606*mP3TrackRecoilTpcNeutrals.Pt() - 0.02518*(pow(mP3TrackRecoilTpcNeutrals.Pt(), 2)));

     if (mP3TrackRecoilTpcNeutrals.Pt() < 5) 
     {
       Double_t corrFact = 5.471 - 2.97 * mP3TrackRecoilTpcNeutrals.Pt() + 0.734 * (pow(mP3TrackRecoilTpcNeutrals.Pt(), 2)) - 0.0631 * (pow(mP3TrackRecoilTpcNeutrals.Pt(), 3));
       mPtTrackRecoilWithNeutralsCorrected = mP3TrackRecoilTpcNeutrals.Pt() * corrFact ;   
     }
     else if  (mP3TrackRecoilTpcNeutrals.Pt() >= 5) 
     {
       mPtTrackRecoilWithNeutralsCorrected = mP3TrackRecoilTpcNeutrals.Pt() * 1.15;
     }

      TVector3 mP3JetRecoil;
      mP3JetRecoil.SetXYZ(mP4JetRecoil.Px(), mP4JetRecoil.Py(), mP4JetRecoil.Pz());

      mP3BalanceFromJets            = mP3JetRecoil + (*mTracksCandidate.begin())->GetP3EScaled();
      mBalanceDeltaPhiFromJets      = (*mTracksCandidate.begin())->GetP3EScaled().DeltaPhi(mP3BalanceFromJets);
      mPtBalanceCosPhiFromJets      = mP3BalanceFromJets.Pt() * cos(mBalanceDeltaPhiFromJets) ;

      mP3BalanceFromTracks          = mP3TrackRecoilTpcNeutrals + (*mTracksCandidate.begin())->GetP3EScaled();
      mBalanceDeltaPhiFromTracks    = (*mTracksCandidate.begin())->mP3AtDca.DeltaPhi(mP3BalanceFromTracks);
      mPtBalanceCosPhiFromTracks    = mP3BalanceFromTracks.Pt() * cos(mBalanceDeltaPhiFromTracks) ;

      //mP3BalanceFromTracks2       = mP3TrackRecoilTow + (*mTracksCandidate.begin())->mP3AtDca;
      mP3BalanceFromTracks2         = mP3TrackRecoilTpc + (*mTracksCandidate.begin())->GetP3EScaled();
      //mBalanceDeltaPhiFromTracks2 = (*mTracksCandidate.begin())->GetP3EScaled().DeltaPhi(mP3TrackRecoilTow);
      mBalanceDeltaPhiFromTracks2   = (*mTracksCandidate.begin())->mP3AtDca.DeltaPhi(mP3BalanceFromTracks2);
      mPtBalanceCosPhiFromTracks2   = mP3BalanceFromTracks2.Pt() * cos(mBalanceDeltaPhiFromTracks2) ;

      mCandElecP3AtDca              = (*mTracksCandidate.begin())->GetP3AtDca();
      mCandElecP3EScaled            = (*mTracksCandidate.begin())->GetP3EScaled();
   }
}


/** Process jets */
void VecBosEvent::ProcessJets()
{
   //Info("Process", "Process jets");

   mP4JetRecoil.SetXYZT(0, 0, 0, 0);

   VecBosJetPtrSetConstIter iJet = mJets.begin();
   mP4JetFirst = *iJet ? **iJet : TLorentzVector();

   for ( ; iJet != mJets.end(); ++iJet)
   {
      VecBosJet *vbJet = *iJet;
      vbJet->Process();
      //printf("\nstJet: ");
      //utils::PrintTLorentzVector(*vbJet);

      mP4JetTotal += *vbJet;

      if ( IsRecoilJet(vbJet) ) {
         mP4JetRecoil += *vbJet;
         mJetsRecoil.insert(vbJet);
      }// else
      //  Info("Process()", "Don't add this jet to recoil");
   }

   //printf("\nmP4JetRecoil: ");
   //utils::PrintTLorentzVector(mP4JetRecoil);
}


void VecBosEvent::ProcessMC()
{
   //Info("ProcessMC()", "test...");

   StMcEvent *mcEvent = (StMcEvent *) StMaker::GetChain()->GetDataSet("StMcEvent");
   assert(mcEvent);

   mWEvent = new WEvent();
   mWEvent->CalcRecoil(*mcEvent);
}


void VecBosEvent::CalcRecoilFromTracks()
{
   //Info("CalcRecoilFromTracks", "");

   // Make sure an isolated track exists
   if (mTracksCandidate.size() < 1) return;

   VecBosTrack &trackCandidate = **mTracksCandidate.begin();
   //printf("\ntrackCandidate: ");
   //trackCandidate.mP3AtDca.Print();

   // To calculate 
   VecBosTrackPtrSetIter iTrack = mTracks.begin();
   for (; iTrack != mTracks.end(); ++iTrack)
   {
      VecBosTrack &track = **iTrack;
      //printf("\ntrack: ");
      //track.mP3AtDca.Print();
      //utils::PrintTLorentzVector(*stJet);

      if ( track.mVertex != trackCandidate.mVertex ) {
         //Info("CalcRecoilFromTracks2", "track.mVertex != trackCandidate.mVertex");
         continue;
      }

      if ( track == trackCandidate ) {
         //Info("CalcRecoilFromTracks2", "track, %x == trackCandidate, %x", track, trackCandidate);
         continue;
      }

      mP3TrackRecoilTpc += track.mP3AtDca;

      if ( track.HasCluster() ) {
         mP3TrackRecoilTow += track.GetP3EScaled();
      }
      else {
         mP3TrackRecoilTow += track.mP3AtDca;
      }
   }

   // Process un-tracked BTOW hits
   for (int iBTow = 0; iBTow < mxBtow; iBTow++)
   {
      float eneTo = bemc.eneTile[kBTow][iBTow];

      if (eneTo <= 0.200) continue; // skip towers with energy below noise

      //printf("tower energy: %f\n", eneTo);

      // Correct BCal tower position to the vertex position
      //TVector3 calP = positionBtow[i] - TVector3(0, 0, vertex.mPosition.Z());
      //TVector3 calP = positionBtow[i] - TVector3(0, 0, trackCandidate.mVertex->mPosition.Z());

      TVector3 towerP = gBCalTowerCoords[iBTow] -  trackCandidate.mVertex->mPosition;
      towerP.SetMag(eneTo); // it is 3D momentum in the event ref frame
      TVector3 towCoord = gBCalTowerCoords[iBTow];
      //printf("tower Pt: %f\n", towerP.Pt());
      //printf("tower Coordinate: X=%f: Y=%f: Z=%f \n", towCoord.X(),towCoord.Y(),towCoord.Z() );

      bool hasMatch            = false;
      bool partOfElecCandidate = false;

      //Check if the tower belongs to the electron 2x2 candidate
      TVector3 distToCluster = (-10, -10, -10);  // nonsense value
      distToCluster = trackCandidate.mCluster2x2.position - towCoord;
      printf("Distance of tower to electron cluster: %f\n", distToCluster.Mag());
      if (distToCluster.Mag() <= 2 * VecBosTrack::sMaxTrackClusterDist) partOfElecCandidate = true;
      //printf("partOfElecCandidate= %d\n", partOfElecCandidate);

      //loop over tracks to and exclude towers with a matching track
      VecBosTrackPtrSetIter iTr = mTracks.begin();
      for ( ; iTr != mTracks.end(); ++iTr)
      {
         VecBosTrack &tr           = **iTr;
         TVector3     trP3         = tr.mP3AtDca;
         TVector3     trCoorAtBTow = tr.GetCoordAtBTow();
         //printf("Track coordinate at  BTower: %f\n", trCoorAtBTow.Mag() );

         if (trCoorAtBTow.Mag() == 0.000000) continue; // track does not extend to barrel
         //printf("Track coordinate at BTower: X=%f Y=%f Z=%f \n", trCoorAtBTow.X(), trCoorAtBTow.Y(), trCoorAtBTow.Z() );

         //TVector3 trCoorAtBTow = (tr.mCoorAtBTow->position.x(),tr.mCoorAtBTow.position.y(),tr.mCoorAtBTow.position.z());

         //if (trP3.DeltaR(towerP)    < 0.1) hasMatch = true;// Checks for a track matching the tower
         //if (hasMatch == true)   continue;
         //if ( mTracks->ExtendTrack2Barrel() == false) continue;

         // spacial separation (track - cluster)
         TVector3 distToTower(-10, -10, -10);  // nonsense value
         distToTower = trCoorAtBTow - towCoord;
         //printf("Distance track to Tower: %f\n", distToTower.Mag());

         if (distToTower.Mag() <= VecBosTrack::sMaxTrackClusterDist) {
            hasMatch = true; // the TPC track maches to the tower
            break;
         }
      }

      //printf("Tower has a match: %d\n",hasMatch);
      if (!hasMatch && !partOfElecCandidate)
      {
         mP3TrackRecoilNeutrals += towerP;
         //printf("Recoil neutrals Pt: %f\n", mP3TrackRecoilNeutrals.Pt());
      }
      //mP3TrackRecoilNeutrals += towerP;

      //double PT = mP3TrackRecoilNeutrals.Pt();
      //printf("PT: %f\n", PT);
      //printf("Recoil neutrals Pt: %f\n", mP3TrackRecoilNeutrals.Pt());
   }

   mP3TrackRecoilTpcNeutrals = mP3TrackRecoilTpc + mP3TrackRecoilNeutrals;
}


void VecBosEvent::MCanalysis()
{
   /*
      VecBosTrackPtrVecIter iTrack = mTracksCandidate.begin();
      for (; iTrack !=  mTracksCandidate.end(); ++iTrack)
      {

        //if ((*iTrack)->IsGood() == false) continue;      // Track has a good vertex
        //if ((*iTrack)->IsIsolated() == false) continue;   // Track is not the electro
        //if(iTrack->HasCluster() == false) continue;  // Track points to a cluster

        TVector3 prP3 = (*iTrack)->mP3AtDca;
         if (prP3.Pt() > 27)
         {
           //Full W cuts applied at this point

           //hadronic recoil and correlations with W from pythia
           //TVector3 hadronicPt(T.hadronicRecoil.X(), T.hadronicRecoil.Y(), 0); //transverse momentum vector

           //mPtKfactor = mWEvent->mRecoilP4.Pt()/mHadronicRecoilPt;
         }
      }
   */
}


/** If there is no track candidate than there is no recoil. */
bool VecBosEvent::IsRecoilJet(VecBosJet *vbJet) const
{
   VecBosTrackPtrSetConstIter iTrack = mTracksCandidate.begin();
   for ( ; iTrack != mTracksCandidate.end(); ++iTrack)
   {
      VecBosTrack &track = **iTrack;
      Double_t deltaR = vbJet->Vect().DeltaR( track.mP3AtDca );

      if ( track.GetVertexId() == vbJet->GetVertexId() && deltaR > sMinTrackIsoDeltaR )
         return true;
   }

   return false;
}


bool VecBosEvent::IsRecoilJetWithZVertexCut(VecBosJet *vbJet) const
{
   VecBosTrackPtrSetConstIter iTrack = mTracksCandidate.begin();
   for ( ; iTrack != mTracksCandidate.end(); ++iTrack) {
      Double_t deltaZ = fabs(vbJet->zVertex - (*iTrack)->mVertex->mPosition.Z());

      if (deltaZ > sMaxTrackJetDeltaZ) continue;

      Double_t deltaR = vbJet->Vect().DeltaR((*iTrack)->mP3AtDca);

      if (deltaR > sMinTrackIsoDeltaR) {
         //Info("IsRecoilJetWithZVertexCut(VecBosJet *vbJet)", "deltaR: %f", deltaR);
         //Info("IsRecoilJetWithZVertexCut(VecBosJet *vbJet)", "deltaZ: %f = |%f - %f|", deltaZ, vbJet->zVertex, (*iTrack)->mVertex->mPosition.Z());
         return true;
      }
   }

   return false;
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

   for (int iEta = etaBin - 1; iEta <= etaBin; iEta++)
   {
      for (int iPhi = phiBin - 1; iPhi <= phiBin; iPhi++)
      {
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

   for (int iEta = etaBin; iEta < etaBin + etaWidth; iEta++)
   {
      // trim in eta-direction
      if (iEta < 0 || iEta >= mxBTetaBin) continue;

      for (int iPhi = phiBin; iPhi < phiBin + phiWidth; iPhi++)
      {
         // wrap up in the phi-direction
         int   iPhi_p  = (iPhi + mxBTphiBin) % mxBTphiBin;  // keep it always positive
         int   towerId = gMapBTowEtaPhiBin2Id[ iEta + iPhi_p * mxBTetaBin];
         float energy  = bemc.eneTile[kBTow][towerId - 1];

         //if (L<5) printf("n=%2d  iEta=%d iPhi_p=%d\n",cluster.nTower,iEta,iPhi_p);

         if (energy <= 0) continue; // skip towers w/o energy

         float adc    = bemc.adcTile[kBTow][towerId - 1];
         float deltaZ = gBCalTowerCoords[towerId - 1].z() - zVert;
         float cosine = nomBTowRadius / sqrt(nomBTowRadius * nomBTowRadius + deltaZ * deltaZ);
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


/**
 * Returns the  sum of all towers withing sMinTrackIsoDeltaR around the track
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
   for (int iBTow = 0; iBTow < mxBtow; iBTow++)
   {
      float energy = bemc.eneTile[kBTow][iBTow];
      if (energy <= 0) continue;

      // Correct BCal tower position to the vertex position
      TVector3 towerCoord = gBCalTowerCoords[iBTow] - vbTrack->mVertex->mPosition;
      towerCoord.SetMag(energy); // it is 3D momentum in the event ref frame

      if (cone1d2d == 1 && fabs(trackP3.DeltaPhi(towerCoord)) > sMinTrackIsoDeltaPhi) continue;
      if (cone1d2d == 2 &&      trackP3.DeltaR(towerCoord)    > sMinTrackIsoDeltaR)   continue;

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

         if (cone1d2d == 1 && fabs(trackP3.DeltaPhi(towerCoord)) > sMinTrackIsoDeltaPhi) continue;
         if (cone1d2d == 2 &&      trackP3.DeltaR(towerCoord)    > sMinTrackIsoDeltaR)   continue;

         // XXX:ds: Another bug? The sum should be vector one
         //ptsum += towerCoord.Perp();
         totalP3InCone += towerCoord;
      }
   }

   return totalP3InCone;
}


TVector3 VecBosEvent::CalcP3InConeTpc(VecBosTrack *vbTrack, UShort_t cone1d2d, Float_t scale)
{
   TVector3 totalP3InCone;

   if (!vbTrack) return totalP3InCone;

   // Scale P3 momentum of the track
   TVector3 trackP3 = vbTrack->mP3AtDca * scale;

   VecBosTrackPtrSetIter iTrack = mTracks.begin();
   for ( ; iTrack != mTracks.end(); ++iTrack) {
      VecBosTrack &track = **iTrack;
      // Skip tracks from different vertices XXX:ds: Later can consider
      // vertices in some close proximity to this one
      if (track.mVertex != vbTrack->mVertex) continue;

      // Don't count the same track
      //if (&*track == vbTrack) continue;
      // XXX:ds: move this requirement to the track class where appropriate
      //if (track.GetFitHitFrac() < sMinTrackHitFrac) continue;

      if (cone1d2d == 1 && fabs(trackP3.DeltaPhi(track.mP3AtDca)) > sMinTrackIsoDeltaPhi) continue;
      if (cone1d2d == 2) {
         if (trackP3.DeltaR(track.mP3AtDca) > sMinTrackIsoDeltaR) continue;
         // Count the number of tracks in the cone
         vbTrack->mNumTracksInNearCone++;
      }

      totalP3InCone += track.mP3AtDca;
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

      if (hitFrac < sMinTrackHitFrac) continue;

      StThreeVectorF prPvect = stMuTrack->p();
      TVector3 vec3AtDca = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

      // printf(" prTrID=%4d  prTrEta=%.3f prTrPhi/deg=%.1f prPT=%.1f  nFitPts=%d\n", stMuTrack->id(),stMuTrack->eta(),stMuTrack->phi()/3.1416*180.,stMuTrack->pt(),stMuTrack->nHitsFit());
      if (flag == 1 && fabs(refAxis.DeltaPhi(vec3AtDca)) > mVecBosEvent->sMinTrackIsoDeltaPhi) continue;
      if (flag == 2 &&      refAxis.DeltaR(vec3AtDca)    > mVecBosEvent->sMinTrackIsoDeltaR)   continue;

      float pT = stMuTrack->pt();

      // XXX:ds: Another bug? The sum should be vector one
      // separate quench for barrel and endcap candidates
      if      (pT > mVecBosEvent->sMinBTrackPt && pointTowId > 0) ptSum += mVecBosEvent->sMinBTrackPt;
      else if (pT > mMinETrackPt && pointTowId < 0)               ptSum += mMinETrackPt;
      else  ptSum += pT;
   }

   return ptSum;
}
*/


void VecBosEvent::clear()
{
   //Info("clear", "");
   mStMuDst            = 0;
   id                  = 0;
   runNo               = 0;
   time                = 0;
   zdcRate             = 0;
   l2bitET             = 0;
   l2bitRnd            = 0;
   l2EbitET            = 0;
   l2EbitRnd           = 0;
   bx7                 = -1;
   bx48                = -1;
   zTag                = false;
   mCpuTimeEventAna    = 0;
   mCpuTimeHistFill    = 0;
   mMuDstNumGTracks    = 0;
   mMuDstNumVertices   = 0;
   mMuDstNumPTracks    = 0;
   mMuDstNumOTracks    = 0;
   mNumGoodVertices    = 0;
   mNumGoodTracks      = 0;
   mNumBTracks         = 0;
   mNumETracks         = 0;
   mNumIsolatedTracks  = 0;
   mNumCandidateTracks = 0;
   mStJets             = 0;
   bxStar7             = -1;
   bxStar48            = -1;
   mSpinPattern4Bits   = -1;
   mSpinDirection      = -1;
   bemc.clear();
   etow.clear();
   eprs.clear();
   esmd.clear();

   while (!mJets.empty()) delete *mJets.begin(), mJets.erase(mJets.begin());
   mJets.clear();
   mJetsRecoil.clear();
   mJetsWithIsoTrack.clear();

   while (!mVertices.empty()) delete *mVertices.begin(), mVertices.erase(mVertices.begin());
   mVertices.clear(); // unnecessary?

   while (!mTracks.empty()) (*mTracks.begin())->prMuTrack = 0, delete * mTracks.begin(), mTracks.erase(mTracks.begin());
   mTracks.clear();
   mTracksCandidate.clear();

   if (mWEvent) delete mWEvent;
   mWEvent = 0;
   //mWEvent = new WEvent();

   mP4JetTotal.SetXYZT(0, 0, 0, 0);
   mP4JetFirst.SetXYZT(0, 0, 0, 0);
   mP4JetRecoil.SetXYZT(0, 0, 0, 0);
   mP3TrackRecoilTpc.SetXYZ(0, 0, 0);
   mP3TrackRecoilTow.SetXYZ(0, 0, 0);
   mP3TrackRecoilNeutrals.SetXYZ(0, 0, 0);
   mP3TrackRecoilTpcNeutrals.SetXYZ(0, 0, 0);
   mP3RecoilFromTracks.SetXYZ(0, 0, 0);
   mP3BalanceFromTracks.SetXYZ(0, 0, 0);
   mCandElecP3AtDca.SetXYZ(0, 0, 0);
   mCandElecP3EScaled.SetXYZ(0, 0, 0);
   mPtKfactor                          =  0;      
   mPtTrackRecoilWithNeutralsCorrected =  0;
   mMinVertexDeltaZ                    = -1;
   mLumiEff                            = -0;
}


void VecBosEvent::Print(int opt, int isMC)
{
   printf("\n");
   Info("Print(int opt, int isMC)", "");

   printf("runNo: %d, ID: %d,  L2Wbits: ET=%d rnd=%d;  muDst: bx7=%d bx48=%d\n" \
          "star: Bx7m=%d, Bx48=%d, mSpinPattern4Bits=%d \n",
          runNo, id, l2bitET, l2bitRnd, bx7, bx48, bxStar7, bxStar48, mSpinPattern4Bits);

   int  yyyymmdd,  hhmmss;
   getGmt_day_hour( yyyymmdd,  hhmmss);
   printf("Event time is: day=%08d, hour=%06d (GMT)\n", yyyymmdd, hhmmss);

   Info("Print", "GetNumJets(): %d", GetNumJets());
   Info("Print", "GetNumVertices(): %d", GetNumVertices());
   Info("Print", "GetNumTracks(): %d", GetNumTracks());
   Info("Print", "GetNumCandidateTracks(): %d", GetNumCandidateTracks());
   Info("Print", "mTracksCandidate.size(): %d", mTracksCandidate.size());

   //Info("Print()", "mTracks: ");
   //VecBosTrackPtrSetIter iTrack = mTracks.begin();
   //for ( ; iTrack != mTracks.end(); ++iTrack)
   //{
   //   Info("Print()", "iTrack: %x", *iTrack);
   //}

   //Info("Print()", "mTracksCandidate:");
   //iTrack = mTracksCandidate.begin();
   //for ( ; iTrack != mTracksCandidate.end(); ++iTrack)
   //{
   //   Info("Print()", "iTrack: %x", *iTrack);
   //}

   VecBosVertexPtrSetIter iVertex = mVertices.begin();

   for ( ; iVertex != mVertices.end(); ++iVertex) {
      VecBosVertex &vertex = **iVertex;
      vertex.Print();
   }
   printf("\n");

   //bemc.print(opt);
}


void VecBosEvent::getGmt_day_hour(int &yyyymmdd, int &hhmmss)
{
   time_t rawtime = this->time;
   struct tm *timeinfo = gmtime( &rawtime );
   char buffer [80];
   strftime (buffer, 80, "%k%M%S", timeinfo);
   hhmmss = atoi(buffer);
   strftime (buffer, 80, "%G%m%d", timeinfo);
   yyyymmdd = atoi(buffer);
}


/**
 * Stream an object of class VecBosEvent.
 */
void VecBosEvent::Streamer(TBuffer &R__b)
{
   //Info("Streamer", "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx ");

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(VecBosEvent::Class(), this);

      //Info("Streamer", "this: %x, mTracks.size(): %d, &mWEvent: %x, &mStJets: %x", this, mTracks.size(), mWEvent, mStJets);

      VecBosTrackPtrSetIter iTrack = mTracks.begin();
      for ( ; iTrack != mTracks.end(); ++iTrack)
      {
         VecBosTrack *track = *iTrack;

         //Info("Streamer", "this: %x, prMuTrack: %x", track, track->prMuTrack);

         // Set pointers to candidate tracks
         if ( track->IsCandidate() ) {
            //Info("Streamer", "mTracksCandidate found: %x", track);
            mTracksCandidate.insert(track);
         }

         // Set pointers to tracks from vertex
         VecBosVertexPtrSetIter iVertex = mVertices.begin();
         for ( ; iVertex != mVertices.end(); ++iVertex) {
            VecBosVertex *vbVertex = *iVertex;

            if ( track->GetVertexId() == vbVertex->GetId() )
               vbVertex->mTracks.insert(track);
         }
      }

      mP4JetRecoil.SetXYZT(0, 0, 0, 0);

      VecBosJetPtrSetConstIter iJet = mJets.begin();
      for ( ; iJet != mJets.end(); ++iJet) {
         VecBosJet *vbJet = *iJet;
         //Info("Streamer", "mJets this: %x", vbJet);

         if ( IsRecoilJet(vbJet) ) {
            //Info("Streamer", "Recoil jet found : %x", vbJet);
            mP4JetRecoil += *vbJet;
            mJetsRecoil.insert(vbJet);
         }
      }

      //VecBosVertexPtrSetIter iVertex = mVertices.begin();
      //for ( ; iVertex != mVertices.end(); ++iVertex)
      //{
      //   VecBosVertex *vbVertex = *iVertex;
      //   Info("Streamer", "mVertices this: %x", vbVertex);
      //}

   }
   else {
      //Info("Streamer", "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy ");
      R__b.WriteClassBuffer(VecBosEvent::Class(), this);
   }
}
