#ifndef EventW_h
#define EventW_h


#include "TLorentzVector.h"
#include "TObject.h"

#include "PyEvent.h"

class WEvent : public TObject
{
public:

   TLorentzVector W;
   TLorentzVector WReco;
   TLorentzVector l;
   TLorentzVector nu;
   TLorentzVector q;
   TLorentzVector recoil;
   TLorentzVector recoilInAccept;

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

   void SetVectors();
   void RecoW();
   void CalcRecoil(PyEvent &pyEvent);
   void CalcEventEnergyRatio(PyEvent &pyEvent);

	ClassDef( WEvent, 1 )
};

#endif
