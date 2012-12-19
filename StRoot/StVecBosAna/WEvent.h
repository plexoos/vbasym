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

   TLorentzVector mWBosonP4;
   TLorentzVector mLeptonP4;
   TLorentzVector mNeutrinoP4;
   TLorentzVector mTotalP4;
   TLorentzVector mRecoilP4;
   TLorentzVector mRecoilInAcceptP4;
   TLorentzVector mRecoilOutAcceptP4;

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

   TLorentzVector RecoW();
   TLorentzVector GetRecoilInOutAccept() { return mRecoilInAcceptP4 + mRecoilOutAcceptP4; }
   void CalcRecoil(PyEvent &pyEvent);
   void CalcRecoil(StMcEvent &stMcEvent);

	ClassDef( WEvent, 1 )
};

#endif
