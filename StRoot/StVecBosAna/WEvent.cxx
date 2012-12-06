
#include "WEvent.h"

#include "utils/utils.h"

#include "StMcEvent/StMcVertex.hh"
#include "StarClassLibrary/StParticleDefinition.hh"


ClassImp(WEvent)


using namespace std;


/** */
WEvent::WEvent() : TObject(), mWBosonP4(), mLeptonP4(), mNeutrinoP4(),
   mTotalP4(), mRecoilP4(), mRecoilInAcceptP4(), mRecoilOutAcceptP4(),
   fLeptonGen(0), fLeptonIndex(0), fNeutrinoIndex(0), fEnergyRatio(0),
   fPzRatio(0), fPtRatio(0), fPtCorr(0), fPtCorrAngle(0), fPzRatioInOut(0),
   fPtRatioInOut(0)
{
}


/** */
TLorentzVector WEvent::RecoW()
{
   return mLeptonP4 + mNeutrinoP4;
}


/** */
void WEvent::CalcRecoil(PyEvent &pyEvent)
{
   Double_t eneTotal = 0, eneAccept = 0;
   TLorentzVector recoilPOutAccept;

   mTotalP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilInAcceptP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilOutAcceptP4.SetPxPyPzE(0, 0, 0, 0);

   vector<track>::const_iterator iParticle    = pyEvent.tracks.begin();
   vector<track>::const_iterator lastParticle = pyEvent.tracks.end();

   for ( ; iParticle!=lastParticle; ++iParticle)
   {
      if (iParticle->KS != 1) continue;
      if (iParticle->index == fLeptonIndex || iParticle->index == fNeutrinoIndex) continue;
      //if (iParticle->index != fLeptonIndex && iParticle->index != fNeutrinoIndex) continue;

      TLorentzVector particleP4(iParticle->px, iParticle->py, iParticle->pz, iParticle->E);

      mRecoilP4   += particleP4;
      eneTotal += particleP4.E();

      //if (particleP4.Eta() > -1 && particleP4.Eta() < 2)
      if (particleP4.Eta() > -2.4 && particleP4.Eta() < 2.4) {
         eneAccept          += particleP4.E();
         mRecoilInAcceptP4  += particleP4;
      } else {
         mRecoilOutAcceptP4 += particleP4;
      }
   }

   //printf("%f, %f\t", mRecoilInAcceptP4.Px(), mRecoilP4.Px());
   //printf("%f, %f\n", mRecoilInAcceptP4.Pz(), mRecoilP4.Pz());

   fEnergyRatio  = eneAccept/eneTotal;
   fPzRatio      = mRecoilInAcceptP4.Pz()/mRecoilP4.Pz();
   fPtRatio      = mRecoilInAcceptP4.Pt()/mRecoilP4.Pt();
   //fPzRatioInOut = (mRecoilInAcceptP4 + mRecoilOutAcceptP4).Pz()/mRecoilP4.Pz();
   //fPtRatioInOut = (mRecoilInAcceptP4 + mRecoilOutAcceptP4).Pt()/mRecoilP4.Pt();

   //mRecoilInAcceptP4.SetPz(0);
   //mRecoilP4.SetPz(0);

   //fPtCorr       = (mRecoilP4.Pt() - mRecoilInAcceptP4.Pt())/mRecoilInAcceptP4.Pt();
   // XXX:ds: z componennt should be removed
   fPtCorrAngle  = mRecoilInAcceptP4.Angle(mRecoilP4.Vect());


   fPtCorr       = mRecoilP4.Pt()/mRecoilInAcceptP4.Pt();
}


/** */
void WEvent::CalcRecoil(StMcEvent &stMcEvent)
{
   Info("CalcRecoil(StMcEvent &stMcEvent)", "Called...");

   Double_t eneTotal  = 0;
   Double_t eneAccept = 0;
   TLorentzVector recoilPOutAccept;

   mTotalP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilInAcceptP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilOutAcceptP4.SetPxPyPzE(0, 0, 0, 0);

   StMcVertex *primVertex = stMcEvent.primaryVertex();

   if (!primVertex) {
      Warning("CalcRecoil(StMcEvent &stMcEvent)", "Primary vertex not found");
      return;
   }

   //cout << "primVertex:" << endl;
   //primVertex->Print();
   //cout << endl;

   // Loop over vertices
   //StSPtrVecMcVertex::const_iterator iVertex    = stMcEvent.vertices().begin();
   //StSPtrVecMcVertex::const_iterator lastVertex = stMcEvent.vertices().end();

   //cout << "vertices:" << endl;
   //for ( ; iVertex!=lastVertex; ++iVertex)
   //{
   //   const StMcVertex *stMcVertex = *iVertex;
   //   stMcVertex->Print();
   //}
   //cout << endl;

   // Loop over tracks
   std::vector<StMcTrack*, std::allocator<StMcTrack*> >::const_iterator iParticle     = stMcEvent.tracks().begin();
   std::vector<StMcTrack*, std::allocator<StMcTrack*> >::const_iterator lastParticle  = stMcEvent.tracks().end();

   //for ( UInt_t iTrack = 0 ; iParticle!=lastParticle; ++iParticle, ++iTrack)
   for ( ; iParticle!=lastParticle; ++iParticle)
   {
      const StMcTrack *mcTrack = *iParticle;

      //Info("CalcRecoil(StMcEvent &stMcEvent)", "iTrack = %d, mcTrack = %x, partDef = %x", iTrack, mcTrack, mcTrack->particleDefinition());

      if (!mcTrack) {
         Warning("CalcRecoil(StMcEvent &stMcEvent)", "StMcTrack is not valid");
         continue;
      }

      //cout << endl;
      //cout << *mcTrack << endl;
      //cout << endl;

      // Pure MC particles
      TLorentzVector particleP4(mcTrack->fourMomentum().px(), mcTrack->fourMomentum().py(), mcTrack->fourMomentum().pz(), mcTrack->fourMomentum().e());

      // Consider pure MC particles
      if (mcTrack->key() == 0 && mcTrack->parent())
      {
         int pdgId = mcTrack->pdgId();

         if (abs(pdgId) == 24)
            mWBosonP4   = particleP4;
         else if (abs(pdgId) == 11 && abs(mcTrack->parent()->pdgId()) == 24)
            mLeptonP4   = particleP4;
         else if (abs(pdgId) == 12 && abs(mcTrack->parent()->pdgId()) == 24)
            mNeutrinoP4 = particleP4;
      }

      const StMcVertex *currVertex = mcTrack->startVertex();

      if (!currVertex) continue;
      //if (currVertex != primVertex) continue;
      if (currVertex->geantProcess() != 0) continue; // only pythia particles/decays

      mTotalP4 += particleP4;
      eneTotal += particleP4.E();

      //if (particleP4.Eta() > -1 && particleP4.Eta() < 2)
      if (particleP4.Eta() > -2.4 && particleP4.Eta() < 2.4) {
         eneAccept          += particleP4.E();
         mRecoilInAcceptP4  += particleP4;
      } else {
         mRecoilOutAcceptP4 += particleP4;
      }
   }

   //cout << "mTotalP4:" << endl;
   //utils::PrintTLorentzVector(mTotalP4);
   //cout << endl;

   mRecoilP4  = mTotalP4;
   mRecoilP4 -= mLeptonP4;
   mRecoilP4 -= mNeutrinoP4;

   //printf("%f, %f\t", mRecoilInAcceptP4.Px(), mRecoilP4.Px());
   //printf("%f, %f\n", mRecoilInAcceptP4.Pz(), mRecoilP4.Pz());

   fEnergyRatio  = eneAccept/eneTotal;
   fPzRatio      = mRecoilInAcceptP4.Pz()/mRecoilP4.Pz();
   fPtRatio      = mRecoilInAcceptP4.Pt()/mRecoilP4.Pt();
   fPzRatioInOut = (mRecoilInAcceptP4 + mRecoilOutAcceptP4).Pz()/mRecoilP4.Pz();
   fPtRatioInOut = (mRecoilInAcceptP4 + mRecoilOutAcceptP4).Pt()/mRecoilP4.Pt();

   //mRecoilInAcceptP4.SetPz(0);
   //mRecoilP4.SetPz(0);

   fPtCorr       = (mRecoilP4.Pt() - mRecoilInAcceptP4.Pt())/mRecoilInAcceptP4.Pt();
   // XXX:ds: z componennt should be removed
   fPtCorrAngle  = mRecoilInAcceptP4.Angle(mRecoilP4.Vect());
}
