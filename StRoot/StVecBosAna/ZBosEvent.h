#ifndef ZBosEvent_h
#define ZBosEvent_h

#include "TLorentzVector.h"
#include "TVector3.h"

#include "VecBosEvent.h"
#include "VecBosTrack.h"



class ZBosEvent : public VecBosEvent
{

public:

   ZBosEvent();
   ~ZBosEvent();

   VecBosTrack& GetElectronCandidate() const;
   TLorentzVector mP4ZBoson;
   TVector3     GetCandidate1_P3() const;
   TVector3     GetCandidate2_P3() const;
   TVector3     GetVecBosonP3() const;
   virtual void Process();
   virtual void Clear(const Option_t* opt="");
   bool         PassedCutZBos(float minElePt=sMinElectronPtLight) const;

   static const float sMinElectronPtLight; //!
   static const float sMinElectronPtHard;  //!
   static const float sMinNeutrinoPt;      //!
   static const float sMinZEleCandPt;      //! S.Fazio 30Sep2013

protected:

   void      CalcZBosP4();

   float            mZBosMass;   
   TVector3         mCand1P3; 
   TVector3         mCand2P3; 
   TLorentzVector   mP4Cand1; 
   TLorentzVector   mP4Cand2;

   ClassDef(ZBosEvent, 1);
};

#endif
