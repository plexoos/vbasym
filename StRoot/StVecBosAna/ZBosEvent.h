#ifndef ZBosEvent_h
#define ZBosEvent_h

#include "TVector3.h"

#include "VecBosEvent.h"
#include "VecBosTrack.h"



class ZBosEvent : public VecBosEvent
{

public:

   ZBosEvent();
   ~ZBosEvent();

   VecBosTrack& GetElectronCandidate() const;
   //TVector3     GetElectronCandidateP3() const;
   TVector3     GetCandidate1_P3() const;
   TVector3     GetCandidate2_P3() const;
   TVector3     GetVecBosonP3() const;
   virtual void Process();
   virtual void Clear(const Option_t* opt="");
   bool         PassedCutWBos(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosPlus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosMinus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutQcdBkg(float minElePt=sMinElectronPtLight) const;

   static const float sMinElectronPtLight; //!
   static const float sMinElectronPtHard;  //!
   static const float sMinNeutrinoPt;      //!

protected:

   float     mZBosMass;   
   TVector3  mCand1P3; 
   TVector3  mCand2P3; 

   ClassDef(ZBosEvent, 1);
};

#endif
