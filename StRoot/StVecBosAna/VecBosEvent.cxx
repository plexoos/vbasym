#include "VecBosEvent.h"
//#include "StVecBosMaker.h"

//need these to get MC record
//#include "tables/St_g2t_tpc_hit_Table.h"
//#include "StMcEventMaker/StMcEventMaker.h"
//#include "StMcEvent/StMcEvent.hh"
//#include "StMcEvent/StMcVertex.hh"
//#include "StMcEvent/StMcTrack.hh"

ClassImp(VecBosEvent)

using namespace std;


VecBosEvent::VecBosEvent() : ProtoEvent(),
   mStMuDst(0),
   mMuDstNumGTracks(0), mMuDstNumVertices(0), mMuDstNumPTracks(0), mMuDstNumOTracks(0),
   mNumGoodVertices(0), mNumGoodTracks(0), mNumBTracks(0), mNumETracks(0),
   mStJets(0),
   mJets(),
   mVertices(),
   mTracks(),
   mLeptonBTracks(), mLeptonETracks(),
   mMaxTrackClusterDist(7),
   mTrackIsoDeltaR     (0.7),
   mTrackIsoDeltaPhi   (0.7),
   mMinBTrackPt        (10)
   //mMcEvent            ()
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
   vbTrack.mVec3AtDca = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

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


UInt_t VecBosEvent::GetNumTracksWithBCluster2()
{
   return mLeptonBTracks.size();
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

   VecBosTrackVecIter iTrack = mTracks.begin();
   for ( ; iTrack != mTracks.end(); ++iTrack) {
      iTrack->Process();

      if (iTrack->IsGood())   mNumGoodTracks++;
      if (iTrack->IsBTrack()) mNumBTracks++;
      if (iTrack->IsETrack()) mNumETracks++;
   }
}


void VecBosEvent::addMC()
{
   StMcEvent *mMcEvent = 0;
   mMcEvent = (StMcEvent *) StMaker::GetChain()->GetDataSet("StMcEvent");
   assert(mMcEvent);

   StMcVertex *V = mMcEvent->primaryVertex();
   mVertex = TVector3(V->position().x(), V->position().y(), V->position().z());

   uint i = 1;
   int found = 0;

   while (found < 2 && i < mMcEvent->tracks().size())
   { //loop tracks
      StMcTrack *mcTrack = mMcEvent->tracks()[i];
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
         assert(T.nearTotET > 0); // internal logical error

         //if (T.mCluster2x2.ET / T.nearTotET < wMK->par_nearTotEtFrac) continue; // too large nearET
         if (T.awayTotET > 30.) continue; // too large awayET , Jan
         //Full W cuts applied at this point

         hadronicRecoilEta = T.hadronicRecoil.Eta();

         //hadronic recoil and correlations with W from pythia
         TVector3 hadronicPt(T.hadronicRecoil.X(), T.hadronicRecoil.Y(), 0); //transverse momentum vector

         hadronicRecoilPt = hadronicPt.Perp();
      }
   }
}


void VecBosEvent::CalcRecoil()
{
   RecoilEneTotal   =  0;
   RecoilEneInAcc   =  0;
   RecoilEneOutAcc  =  0;

   StMcEvent *mMcEvent = 0;

   mMcEvent = (StMcEvent *) StMaker::GetChain()->GetDataSet("StMcEvent");
   assert(mMcEvent);

   uint i = 0;
   //int found = 0;
   while (i < mMcEvent->tracks().size())
   { //loop tracks
      StMcTrack *mcTrack = mMcEvent->tracks()[i];
      int pdgId = mcTrack->pdgId();
      //int key = mcTrack->key();

      //if (key = 1) {
      if (abs(pdgId) != 11 && abs(pdgId) != 12) {
         hadr = mcTrack->fourMomentum();

         // TLorentzVector hadr(mcTrack->fourmomentum().px(), mcTrack->fourMomentum().py(), mcTrack->fourMomentum().pz(), mcTrack->fourMomentum().e());
         RecoilEneTotal += mcTrack->energy();
         recoil         += hadr;

         //if (iParticle->eta > -1 && iParticle->eta < 2)
         if (hadr.pseudoRapidity() > -2.4 && hadr.pseudoRapidity() < 2.4) {
            RecoilEneInAcc += mcTrack->energy();
            recoilInAccept += hadr;
         }
         else {
            RecoilEneOutAcc   += mcTrack->energy();
            recoilOutAccept += hadr;
         }
      }
      //}
      i++;
   }

   fEnergyRatio  = recoilInAccept.e() / recoil.e();
   fPzRatio      = recoilInAccept.pz() / recoil.pz();
   fPtRatio      = recoilInAccept.perp() / recoil.perp();
   fPzRatioInOut = (recoilInAccept + recoilOutAccept).pz() / recoil.pz();
   fPtRatioInOut = (recoilInAccept + recoilOutAccept).perp() / recoil.perp();
}


void VecBosEvent::CalcPtInConeAround(VecBosTrack *vbTrack)
{
   VecBosTrackVecIter iTrack = mTracks.begin();
   for ( ; iTrack != mTracks.end(); ++iTrack) {
      // Skip tracks from different vertices XXX:ds: Later can consider
      // vertices in close  proximity to this one
      if (iTrack->mVertex != vbTrack->mVertex) continue;

      // Don't count the same track
      if (&*iTrack == vbTrack) continue;
   }
}


void VecBosEvent::clear()
{
   //Info("clear", "");
   mStMuDst          = 0;
   id                = 0;
   runNo             = 0;
   time              = 0;
   zdcRate           = 0;
   l2bitET           = 0;
   l2bitRnd          = 0;
   l2EbitET          = 0;
   l2EbitRnd         = 0;
   bx7               = -1;
   bx48              = -1;
   zTag              = false;
   mMuDstNumVertices = 0;
   mMuDstNumGTracks  = 0;
   mMuDstNumPTracks  = 0;
   mMuDstNumOTracks  = 0;
   mNumGoodVertices  = 0;
   mNumGoodTracks    = 0;
   mNumBTracks       = 0;
   mNumETracks       = 0;
   mStJets           = 0;
   bxStar7           = -1;
   bxStar48          = -1;
   spin4             = -1;
   bemc.clear();
   etow.clear();
   eprs.clear();
   esmd.clear();
   mJets.clear();
   mVertices.clear();
   mTracks.clear();
   mLeptonBTracks.clear();
   mLeptonETracks.clear();
   mMaxTrackClusterDist = 7;
   mTrackIsoDeltaR      = 0.7;  // (rad) near-cone size
   mTrackIsoDeltaPhi    = 0.7;  // (rad) away-'cone' size, approx. 40 deg.
   mMinBTrackPt         = 10.;  // GeV
   recoil.setX(0);
   recoil.setY(0);
   recoil.setZ(0);
   recoil.setPx(0);
   recoil.setPy(0);
   recoil.setPz(0);
   recoil.setE(0);
   recoil.setT(0);
   recoilInAccept.setX(0);
   recoilInAccept.setY(0);
   recoilInAccept.setZ(0);
   recoilInAccept.setPx(0);
   recoilInAccept.setPy(0);
   recoilInAccept.setPz(0);
   recoilInAccept.setE(0);
   recoilInAccept.setT(0);
   recoilInAccept.setX(0);
   recoilInAccept.setY(0);
   recoilInAccept.setZ(0);
   recoilOutAccept.setPx(0);
   recoilOutAccept.setPy(0);
   recoilOutAccept.setPz(0);
   recoilOutAccept.setE(0);
   recoilOutAccept.setT(0);
   fEnergyRatio = 0;
}


void VecBosEvent::print(int flag, int isMC)
{
   Info("print", "");

   printf("\nmy W2011event runNo=%d ID=%d  L2Wbits: ET=%d rnd=%d;  muDst: bx7=%d bx48=%d nVert=%d star: Bx7m=%d, Bx48=%d, spin4=%d \n", runNo, id, l2bitET, l2bitRnd, bx7, bx48, mVertices.size(), bxStar7, bxStar48, spin4);
   int  yyyymmdd,  hhmmss; getGmt_day_hour( yyyymmdd,  hhmmss);
   printf("  event time is: day=%d, hour=%d (GMT)\n", yyyymmdd, hhmmss);

   for (uint i = 0; i < mVertices.size(); i++) mVertices[i].print(flag);
   bemc.print(flag);
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
