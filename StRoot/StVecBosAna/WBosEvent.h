#ifndef WBosEvent_h
#define WBosEvent_h

#include "TVector3.h"
#include "TLorentzVector.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

#include "VecBosEvent.h"
#include "VecBosTrack.h"

#include <sstream>

/**
 * This class extends the data structure and functionality of #VecBosEvent which is saved in the
 * output ROOT tree. The events described by this class are assumed to pass the W boson
 * requirements.
 */
class WBosEvent : public VecBosEvent
{
public:

   WBosEvent(float minTrackPt=0, bool otherSolution=false);

   VecBosTrack& GetElectronTrack() const;
   TVector3     GetElectronP3() const;
   TVector3     GetNeutrinoP3() const;
   TVector3     GetNeutrinoP3Other() const;
   TVector3     CalcMissingEnergyP3() const;
   TVector3     CalcSignedPtBalance() const;
   TVector3     GetVecBosonP3() const;
   TLorentzVector GetVecBosonP4() const;
   void         PredictionAnEvol();
   void         PredictionAn();
   virtual void Process();
   virtual void ProcessPersistent();
   virtual void ProcessMC();
   virtual void Clear(const Option_t* opt="");
   virtual void Print(const Option_t* opt="") const;
   bool         PassedCutWBos(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosPlus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutWBosMinus(float minElePt=sMinElectronPtLight) const;
   bool         PassedCutQcdBkg(float minElePt=sMinElectronPtLight) const;
   //Double_t     An_evol_ZK;

   static bool sUseOtherSolution;          //!< If true calculates the W kinematics using the second choice solution for the z-component
   static const float sMinElectronPtLight; //!
   static const float sMinElectronPtHard;  //!
   static const float sMinNeutrinoPt;      //!

protected:

   void      ReconstructNeutrinoZ();

   float     mWBosMass;   //
   TVector3  mElectronP3; //
   TVector3  mNeutrinoP3; //
   TVector3  mNeutrinoP3Other; //!< Neutrino momentum based on "wrong" p_z solution XXX:ds: is not persistent temporarily
   //float     tracksPt;

   ClassDef(WBosEvent, 1);
};

#endif
