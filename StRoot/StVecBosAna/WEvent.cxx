
#include "WEvent.h"

#include "StMcEvent/StMcVertex.hh"
#include "StarClassLibrary/StParticleDefinition.hh"


ClassImp(WEvent)


using namespace std;


/** */
WEvent::WEvent() : TObject(), mWBosonP4(), mLeptonP4(), mNeutrinoP4(), mTotalP4(), mRecoilP4(), mRecoilInAcceptP4(), mRecoilOutAcceptP4(), fLeptonGen(0),
fLeptonIndex(0), fNeutrinoIndex(0), fEnergyRatio(0), fPzRatio(0), fPtRatio(0), fPtCorr(0), fPtCorrAngle(0),
fPzRatioInOut(0), fPtRatioInOut(0)
{
}


/** */
void WEvent::SetVectors()
{
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

      TLorentzVector particleP(iParticle->px, iParticle->py, iParticle->pz, iParticle->E);

      mRecoilP4   += particleP;
      eneTotal += particleP.E();

      //if (particleP.Eta() > -1 && particleP.Eta() < 2)
      if (particleP.Eta() > -2.4 && particleP.Eta() < 2.4) {
         eneAccept       += particleP.E();
         mRecoilInAcceptP4  += particleP;
      } else {
         mRecoilOutAcceptP4 += particleP;
      }
   }

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


/** */
void WEvent::CalcRecoil(StMcEvent &stMcEvent)
{
   //Info("CalcRecoil(StMcEvent &stMcEvent)", "test...");

   Double_t eneTotal  = 0;
   Double_t eneAccept = 0;
   TLorentzVector recoilPOutAccept;

   mTotalP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilInAcceptP4.SetPxPyPzE(0, 0, 0, 0);
   mRecoilOutAcceptP4.SetPxPyPzE(0, 0, 0, 0);

   StMcVertex *primVertex = stMcEvent.primaryVertex();

   if (!primVertex) return;

   std::vector<StMcTrack*, std::allocator<StMcTrack*> >::const_iterator iParticle     = stMcEvent.tracks().begin();
   std::vector<StMcTrack*, std::allocator<StMcTrack*> >::const_iterator lastParticle  = stMcEvent.tracks().end();

   for ( UInt_t iTrack = 0 ; iParticle!=lastParticle; ++iParticle, ++iTrack)
   {
      const StMcTrack *mcTrack = *iParticle;

      //Info("CalcRecoil(StMcEvent &stMcEvent)", "iTrack = %d, mcTrack = %x, partDef = %x", iTrack, mcTrack, mcTrack->particleDefinition());

      if (!mcTrack) continue;

      // Consider pure MC particles
      if (!mcTrack->key() && mcTrack->parent())
      {
         int pdgId = mcTrack->pdgId();

         if (abs(pdgId) == 24)
            mWBosonP4.SetPxPyPzE(mcTrack->fourMomentum().px(), mcTrack->fourMomentum().py(), mcTrack->fourMomentum().pz(), mcTrack->fourMomentum().e());
         else if (abs(pdgId) == 11 && abs(mcTrack->parent()->pdgId()) == 24)
            mLeptonP4.SetPxPyPzE(mcTrack->fourMomentum().px(), mcTrack->fourMomentum().py(), mcTrack->fourMomentum().pz(), mcTrack->fourMomentum().e());
         else if (abs(pdgId) == 12 && abs(mcTrack->parent()->pdgId()) == 24)
            mNeutrinoP4.SetPxPyPzE(mcTrack->fourMomentum().px(), mcTrack->fourMomentum().py(), mcTrack->fourMomentum().pz(), mcTrack->fourMomentum().e());
      }

      const StMcVertex *currVertex = mcTrack->startVertex();

      if (!currVertex) continue;
      if (currVertex != primVertex) continue;

      // Pure MC particles
      TLorentzVector particleP(mcTrack->fourMomentum().px(), mcTrack->fourMomentum().py(), mcTrack->fourMomentum().pz(), mcTrack->fourMomentum().e());

      mTotalP4 += particleP;

      //mRecoilP4   += particleP;
      eneTotal += particleP.E();

      //if (particleP.Eta() > -1 && particleP.Eta() < 2)
      if (particleP.Eta() > -2.4 && particleP.Eta() < 2.4) {
         eneAccept       += particleP.E();
         mRecoilInAcceptP4  += particleP;
      } else {
         mRecoilOutAcceptP4 += particleP;
      }
   }

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
