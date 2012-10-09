
#include "WEvent.h"

ClassImp(WEvent)


using namespace std;


/** */
WEvent::WEvent() : TObject(), W(), l(), nu(), q(), recoil(), recoilInAccept(), fLeptonGen(0),
fLeptonIndex(0), fNeutrinoIndex(0), fEnergyRatio(0), fPzRatio(0), fPtRatio(0), fPtCorr(0), fPtCorrAngle(0),
fPzRatioInOut(0), fPtRatioInOut(0)
{//{{{
}//}}}


/** */
void WEvent::SetVectors()
{//{{{
}//}}}


/** */
void WEvent::RecoW()
{//{{{
   WReco = l + nu;
}//}}}


/** */
void WEvent::CalcRecoil(PyEvent &pyEvent)
{//{{{
   recoil.SetPxPyPzE(0, 0, 0, 0);
   recoilInAccept.SetPxPyPzE(0, 0, 0, 0);

   vector<track>::const_iterator iParticle;
   vector<track>::const_iterator firstParticle = pyEvent.tracks.begin();
   vector<track>::const_iterator lastParticle  = pyEvent.tracks.end();

   for (iParticle=firstParticle; iParticle!=lastParticle; ++iParticle)
   {
      if (iParticle->KS != 1) continue;
      if (iParticle->index == fLeptonIndex || iParticle->index == fNeutrinoIndex) continue;
      //if (iParticle->index != fLeptonIndex && iParticle->index != fNeutrinoIndex) continue;

      TLorentzVector hadr(iParticle->px, iParticle->py, iParticle->pz, iParticle->E);

      recoil += hadr;

      //if (iParticle->eta > -1 && iParticle->eta < 2)
      if (iParticle->eta > -2.4 && iParticle->eta < 2.4) {
         recoilInAccept += hadr;
      }
   }

}//}}}


/** */
void WEvent::CalcEventEnergyRatio(PyEvent &pyEvent)
{//{{{
   Double_t eneTotal = 0, eneAccept = 0;
   TLorentzVector recoilPTotal;
   TLorentzVector recoilPInAccept;
   TLorentzVector recoilPOutAccept;

   vector<track>::const_iterator iParticle;
   vector<track>::const_iterator firstParticle = pyEvent.tracks.begin();
   vector<track>::const_iterator lastParticle  = pyEvent.tracks.end();

   for (iParticle=firstParticle; iParticle!=lastParticle; ++iParticle)
   {
      if (iParticle->KS != 1) continue;
      if (iParticle->index == fLeptonIndex || iParticle->index == fNeutrinoIndex) continue;

      TLorentzVector particleP(iParticle->px, iParticle->py, iParticle->pz, iParticle->E);

      eneTotal     += iParticle->E;
      recoilPTotal += particleP;

      //if (iParticle->eta > -1 && iParticle->eta < 2)
      if (iParticle->eta > -2.4 && iParticle->eta < 2.4) {
         eneAccept       += iParticle->E;
         recoilPInAccept += particleP;
      } else {
         recoilPOutAccept += particleP;
      }
   }

   //printf("%f, %f\t", recoilPInAccept.Px(), recoilPTotal.Px());
   //printf("%f, %f\n", recoilPInAccept.Pz(), recoilPTotal.Pz());

   fEnergyRatio  = eneAccept/eneTotal;
   fPzRatio      = recoilPInAccept.Pz()/recoilPTotal.Pz();
   fPtRatio      = recoilPInAccept.Pt()/recoilPTotal.Pt();
   fPzRatioInOut = (recoilPInAccept + recoilPOutAccept).Pz()/recoilPTotal.Pz();
   fPtRatioInOut = (recoilPInAccept + recoilPOutAccept).Pt()/recoilPTotal.Pt();

   recoilPInAccept.SetPz(0);
   recoilPTotal.SetPz(0);

   fPtCorr       = (recoilPTotal.Pt() - recoilPInAccept.Pt())/recoilPInAccept.Pt();
   fPtCorrAngle  = recoilPInAccept.Angle(recoilPTotal.Vect());
}//}}}
