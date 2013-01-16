
#include "WEvent.h"

#include "utils/utils.h"

#include "StMcEvent/StMcVertex.hh"
#include "StarClassLibrary/StParticleDefinition.hh"


ClassImp(WEvent)


using namespace std;


/** */
WEvent::WEvent() : TObject(), mP4WBoson(), mP4Lepton(), mP4Neutrino(),
   mP4Total(), mP4Recoil(), mP4RecoilInAccept(), mP4RecoilOutAccept(),
   fLeptonGen(0), fLeptonIndex(0), fNeutrinoIndex(0), fEnergyRatio(0),
   fPzRatio(0), fPtRatio(0), fPtCorr(0), fPtCorrAngle(0), fPzRatioInOut(0),
   fPtRatioInOut(0)
{
}


/** */
TLorentzVector WEvent::CalcRecoP4WBoson()
{
   return mP4Lepton + mP4Neutrino;
}


/** */
void WEvent::CalcRecoil(PyEvent &pyEvent)
{
   Double_t eneTotal = 0, eneAccept = 0;
   TLorentzVector recoilPOutAccept;

   mP4Total.SetPxPyPzE(0, 0, 0, 0);
   mP4Recoil.SetPxPyPzE(0, 0, 0, 0);
   mP4RecoilInAccept.SetPxPyPzE(0, 0, 0, 0);
   mP4RecoilOutAccept.SetPxPyPzE(0, 0, 0, 0);

   vector<track>::const_iterator iParticle    = pyEvent.tracks.begin();
   vector<track>::const_iterator lastParticle = pyEvent.tracks.end();

   for ( ; iParticle!=lastParticle; ++iParticle)
   {
      if (iParticle->KS != 1) continue;
      if (iParticle->index == fLeptonIndex || iParticle->index == fNeutrinoIndex) continue;
      //if (iParticle->index != fLeptonIndex && iParticle->index != fNeutrinoIndex) continue;

      TLorentzVector particleP4(iParticle->px, iParticle->py, iParticle->pz, iParticle->E);

      mP4Recoil   += particleP4;
      eneTotal += particleP4.E();

      //if (particleP4.Eta() > -1 && particleP4.Eta() < 2)
      if (particleP4.Eta() > -2.4 && particleP4.Eta() < 2.4) {
         eneAccept          += particleP4.E();
         mP4RecoilInAccept  += particleP4;
      } else {
         mP4RecoilOutAccept += particleP4;
      }
   }

   //printf("%f, %f\t", mP4RecoilInAccept.Px(), mP4Recoil.Px());
   //printf("%f, %f\n", mP4RecoilInAccept.Pz(), mP4Recoil.Pz());

   fEnergyRatio  = eneAccept/eneTotal;
   fPzRatio      = mP4RecoilInAccept.Pz()/mP4Recoil.Pz();
   fPtRatio      = mP4RecoilInAccept.Pt()/mP4Recoil.Pt();
   //fPzRatioInOut = (mP4RecoilInAccept + mP4RecoilOutAccept).Pz()/mP4Recoil.Pz();
   //fPtRatioInOut = (mP4RecoilInAccept + mP4RecoilOutAccept).Pt()/mP4Recoil.Pt();

   //mP4RecoilInAccept.SetPz(0);
   //mP4Recoil.SetPz(0);

   //fPtCorr       = (mP4Recoil.Pt() - mP4RecoilInAccept.Pt())/mP4RecoilInAccept.Pt();
   // XXX:ds: z componennt should be removed
   fPtCorrAngle  = mP4RecoilInAccept.Angle(mP4Recoil.Vect());


   fPtCorr       = mP4Recoil.Pt()/mP4RecoilInAccept.Pt();
}


/** */
void WEvent::CalcRecoil(StMcEvent &stMcEvent)
{
   //Info("CalcRecoil(StMcEvent &stMcEvent)", "Called...");

   Double_t eneTotal  = 0;
   Double_t eneAccept = 0;
   TLorentzVector recoilPOutAccept;

   mP4Total.SetPxPyPzE(0, 0, 0, 0);
   mP4Recoil.SetPxPyPzE(0, 0, 0, 0);
   mP4RecoilInAccept.SetPxPyPzE(0, 0, 0, 0);
   mP4RecoilOutAccept.SetPxPyPzE(0, 0, 0, 0);

   StMcVertex *primVertex = stMcEvent.primaryVertex();

   if (!primVertex) {
      Warning("CalcRecoil(StMcEvent &stMcEvent)", "Primary vertex not found");
      return;
   }


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
            mP4WBoson   = particleP4;
         else if (abs(pdgId) == 11 && abs(mcTrack->parent()->pdgId()) == 24)
            mP4Lepton   = particleP4;
         else if (abs(pdgId) == 12 && abs(mcTrack->parent()->pdgId()) == 24)
            mP4Neutrino = particleP4;
      }

      const StMcVertex *currVertex = mcTrack->startVertex();

      if (!currVertex) continue;
      //if (currVertex != primVertex) continue;
      if (currVertex->geantProcess() != 0) continue; // only pythia particles/decays

      mP4Total += particleP4;
      eneTotal += particleP4.E();

      //if (particleP4.Eta() > -2.4 && particleP4.Eta() < 2.4)
      if (particleP4.Eta() > -1.1 && particleP4.Eta() < 2.0)
      {
         eneAccept          += particleP4.E();
         mP4RecoilInAccept  += particleP4;
      } else {
         mP4RecoilOutAccept += particleP4;
      }
   }

   //cout << "mP4Total:" << endl;
   //utils::PrintTLorentzVector(mP4Total);
   //cout << endl;

   mP4Recoil  = mP4Total;
   mP4Recoil -= mP4Lepton;
   mP4Recoil -= mP4Neutrino;

   if (mP4Lepton.Eta() > -1.1 && mP4Lepton.Eta() < 2.0)
      mP4RecoilInAccept -= mP4Lepton;
   else                                                
      mP4RecoilOutAccept -= mP4Lepton;

   if (mP4Neutrino.Eta() > -1.1 && mP4Neutrino.Eta() < 2.0)
      mP4RecoilInAccept -= mP4Neutrino;
   else 
      mP4RecoilOutAccept -= mP4Neutrino;

   //printf("%f, %f\t", mP4RecoilInAccept.Px(), mP4Recoil.Px());
   //printf("%f, %f\n", mP4RecoilInAccept.Pz(), mP4Recoil.Pz());

   fEnergyRatio  = eneAccept/eneTotal;
   fPzRatio      = mP4RecoilInAccept.Pz()/mP4Recoil.Pz();
   fPtRatio      = mP4RecoilInAccept.Pt()/mP4Recoil.Pt();
   fPzRatioInOut = (mP4RecoilInAccept + mP4RecoilOutAccept).Pz()/mP4Recoil.Pz();
   fPtRatioInOut = (mP4RecoilInAccept + mP4RecoilOutAccept).Pt()/mP4Recoil.Pt();

   //mP4RecoilInAccept.SetPz(0);
   //mP4Recoil.SetPz(0);

   fPtCorr       = (mP4Recoil.Pt() - mP4RecoilInAccept.Pt())/mP4RecoilInAccept.Pt();
   // XXX:ds: z componennt should be removed
   fPtCorrAngle  = mP4RecoilInAccept.Angle(mP4Recoil.Vect());
}
