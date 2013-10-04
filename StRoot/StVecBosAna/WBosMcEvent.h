#ifndef WBosMcEvent_h
#define WBosMcEvent_h

#include "TLorentzVector.h"
#include "TObject.h"

#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcTrack.hh"

#include "VecBosMcEvent.h"
#include "src/PyEvent.h"


class WBosMcEvent : public VecBosMcEvent
{
public:

   TLorentzVector mP4WBoson;
   TLorentzVector mP4Lepton;
   TLorentzVector mP4Neutrino;
   TLorentzVector mP4Total;
   TLorentzVector mP4Recoil;
   TLorentzVector mP4RecoilInAccept;
   TLorentzVector mP4RecoilOutAccept;

   UShort_t       fLeptonGen;
   UShort_t       fLeptonSign;
   UShort_t       fLeptonIndex;
   UShort_t       fNeutrinoIndex;

   Double_t       fEnergyRatio;
   Double_t       fPzRatio;
   Double_t       fPtRatio;
   Double_t       fPtCorr;
   Double_t       fPtCorrAngle;
   Double_t       fPzRatioInOut;
   Double_t       fPtRatioInOut;

   WBosMcEvent();

   TLorentzVector CalcRecoP4WBoson();
   TLorentzVector GetRecoilInOutAccept() { return mP4RecoilInAccept + mP4RecoilOutAccept; }
   void           CalcRecoil(PyEvent &pyEvent);
   void           CalcRecoil(StMcEvent &stMcEvent);

	ClassDef( WBosMcEvent, 1 )
};

#endif
