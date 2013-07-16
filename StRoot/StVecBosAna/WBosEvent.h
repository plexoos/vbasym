#ifndef WBosEvent_h
#define WBosEvent_h

#include "TVector3.h"

#include "VecBosEvent.h"
#include "VecBosTrack.h"



class WBosEvent : public VecBosEvent
{

public:

   WBosEvent();
   ~WBosEvent();

   VecBosTrack& GetElectronTrack() const;
   TVector3     GetElectronP3() const;
   TVector3     GetNeutrinoP3() const;
   TVector3     GetMissingEnergyP3() const;
   TVector3     GetVecBosonP3() const;
   virtual void Process();
   virtual void ProcessPersistent();
   virtual void Clear(const Option_t* opt="");
   bool         PassedCutWBos(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosPlus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosMinus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutQcdBkg(float minElePt=sMinElectronPtLight) const;

   static const float sMinElectronPtLight; //
   static const float sMinElectronPtHard;  //
   static const float sMinNeutrinoPt;      //

protected:

   void      ReconstructNeutrinoZ();

   float     mWBosMass;   //
   TVector3  mElectronP3; //
   TVector3  mNeutrinoP3; //

   ClassDef(WBosEvent, 1);
};

#endif
