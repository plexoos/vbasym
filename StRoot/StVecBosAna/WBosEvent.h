#ifndef WBosEvent_h
#define WBosEvent_h

#include "TVector3.h"

#include "VecBosEvent.h"


/**
 * This class extends the data structure and functionality of #VecBosEvent which is saved in the
 * output ROOT tree. The events described by this class are assumed to pass the W boson
 * requirements.
 */
class WBosEvent : public VecBosEvent
{

public:

   WBosEvent();

   VecBosTrack& GetElectronTrack() const;
   TVector3     GetElectronP3() const;
   TVector3     GetNeutrinoP3() const;
   TVector3     CalcMissingEnergyP3() const;
   TVector3     CalcSignedPtBalance() const;
   TVector3     GetVecBosonP3() const;
   virtual void Process();
   virtual void ProcessPersistent();
   virtual void ProcessMC();
   virtual void Clear(const Option_t* opt="");
   virtual void Print(const Option_t* opt="") const;
   bool         PassedCutWBos(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosPlus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosMinus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutQcdBkg(float minElePt=sMinElectronPtLight) const;

   static const float sMinElectronPtLight; //!
   static const float sMinElectronPtHard;  //!
   static const float sMinNeutrinoPt;      //!

protected:

   void      ReconstructNeutrinoZ();

   float     mWBosMass;   //
   TVector3  mElectronP3; //
   TVector3  mNeutrinoP3; //
   TVector3  mNeutrinoP3Other; //!< Neutrino momentum based on "wrong" p_z solution XXX:ds: is not persistent temporarily

   ClassDef(WBosEvent, 1);
};

#endif
