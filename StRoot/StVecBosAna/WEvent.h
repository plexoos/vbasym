#ifndef EventW_h
#define EventW_h

#include "TLorentzVector.h"
#include "TObject.h"

#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcTrack.hh"

#include "PyEvent.h"

class WEvent : public TObject
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

   WEvent();

   TLorentzVector CalcRecoP4WBoson();
   TLorentzVector GetRecoilInOutAccept() { return mP4RecoilInAccept + mP4RecoilOutAccept; }
   void           CalcRecoil(PyEvent &pyEvent);
   void           CalcRecoil(StMcEvent &stMcEvent);

	ClassDef( WEvent, 1 )
};

#endif
