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

   VecBosTrack& GetElectronCandidate() const;
   TVector3     GetElectronCandidateP3() const;
   TVector3     GetMissingEnergyP3() const;
   TVector3     GetVecBosonP3() const;
   bool         PassedCutWBos(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosPlus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosMinus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutQcdBkg(float minElePt=sMinElectronPtLight) const;

   static const float sMinElectronPtLight;
   static const float sMinElectronPtHard;
   static const float sMinNeutrinoPt;


   ClassDef(WBosEvent, 1);
};

#endif
