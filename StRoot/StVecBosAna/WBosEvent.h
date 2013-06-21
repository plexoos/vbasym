#ifndef WBosEvent_h
#define WBosEvent_h

#include "TVector3.h"

#include "VecBosEvent.h"



class WBosEvent : public VecBosEvent
{

public:

   WBosEvent();
   ~WBosEvent();

   TVector3 GetElectronCandidate() const;
   TVector3 GetMissingEnergy() const;
   TVector3 GetVecBoson() const;

   ClassDef(WBosEvent, 1);
};

#endif
