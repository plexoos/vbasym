#include "WBosEvent.h"
#include "WBosMcEvent.h"

#include "utils/utils.h"

ClassImp(WBosEvent)

using namespace std;


bool WBosEvent::sUseOtherSolution = false;
const float WBosEvent::sMinElectronPtLight = 15;
const float WBosEvent::sMinElectronPtHard  = 25;
const float WBosEvent::sMinNeutrinoPt      = 18;


WBosEvent::WBosEvent(float minTrackPt, bool otherSolution) : VecBosEvent()
   , mWBosMass(80.385)
   , mElectronP3()
   , mNeutrinoP3()
   , mNeutrinoP3Other()
{
   VecBosEvent::sMinRecoilTrackPt = minTrackPt;
   sUseOtherSolution = otherSolution;
}


VecBosTrack& WBosEvent::GetElectronTrack() const
{
   return *(*mTracksCandidate.begin());
}


TVector3 WBosEvent::GetElectronP3() const { return mElectronP3; }
TVector3 WBosEvent::GetNeutrinoP3() const { return mNeutrinoP3; }
TVector3 WBosEvent::GetNeutrinoP3Other() const { return mNeutrinoP3Other; }


TVector3 WBosEvent::CalcMissingEnergyP3() const
{
   //return -1*(mP3TrackRecoilTpcNeutrals + mElectronP3);
   return -1*(mP3TrackRecoilTpcNeutralsCorrected + mElectronP3);

   // Other definitions
   //return -1*(mP3TrackRecoilTow + mElectronP3);
   //return -1*(mP4JetRecoil + mElectronP3);
}


TVector3 WBosEvent::CalcSignedPtBalance() const
{
   return mPtBalanceCosPhiFromTracks;
}


TVector3 WBosEvent::GetVecBosonP3() const { return mElectronP3 + (sUseOtherSolution ? mNeutrinoP3Other : mNeutrinoP3); }


TLorentzVector WBosEvent::GetVecBosonP4() const
{
   TLorentzVector wBosonP4;
   wBosonP4.SetVectM(GetVecBosonP3(), mWBosMass);
   return wBosonP4;
}


/**
 * The primary method to identify and reconstruct the event with a W boson.
 */
void WBosEvent::Process()
{
   VecBosEvent::Process();
   ProcessPersistent();
}


void WBosEvent::ProcessPersistent()
{
   VecBosEvent::ProcessPersistent();

   // Proceed only if this is a W event, i.e. it conforms to W event signature
   //if ( !PassedCutWBos() ) return;
   if ( !HasCandidateEle() ) return;

   mElectronP3 = GetElectronTrack().GetP3EScaled();
   mNeutrinoP3 = CalcMissingEnergyP3(); // here we set only x and y components, and reconstruct the z one later
   mNeutrinoP3Other = mNeutrinoP3;

   ReconstructNeutrinoZ();
}


void WBosEvent::ProcessMC()
{
   fIsMc = true;
   StMcEvent *stMcEvent = (StMcEvent *) StMaker::GetChain()->GetDataSet("StMcEvent");
   assert(stMcEvent);

   mMcEvent = new WBosMcEvent();
   ((WBosMcEvent*) mMcEvent)->CalcRecoil(*stMcEvent);
}


void WBosEvent::Clear(const Option_t* opt)
{
   VecBosEvent::Clear();
   mElectronP3.SetXYZ(0, 0, 0);
   mNeutrinoP3.SetXYZ(0, 0, 0);
}


void WBosEvent::Print(const Option_t* opt) const
{
   Info("Print", ":");
   //VecBosEvent::Print(opt);
   mElectronP3.Print();
   mNeutrinoP3.Print();
}


bool WBosEvent::PassedCutWBos(float minElePt) const
{
   if ( HasCandidateEle() &&
        mPtBalanceCosPhiFromTracks >= sMinNeutrinoPt &&
        //mNeutrinoP3.Pt() >= sMinNeutrinoPt &&
        mElectronP3.Pt() >= minElePt)
   {
      return true;
   }

   return false;
}


bool WBosEvent::PassedCutWBosPlus(float minElePt) const
{
   return PassedCutWBos(minElePt) && GetElectronTrack().GetChargeSign() > 0;
}


bool WBosEvent::PassedCutWBosMinus(float minElePt) const
{
   return PassedCutWBos(minElePt) && GetElectronTrack().GetChargeSign() < 0;
}


bool WBosEvent::PassedCutQcdBkg(float minElePt) const
{
   if ( HasCandidateEle() && mPtBalanceCosPhiFromTracks < sMinNeutrinoPt &&
        mElectronP3.Pt() >= minElePt)
   {
      return true;
   }

   return false;
}


void WBosEvent::ReconstructNeutrinoZ()
{
   double A = mWBosMass*mWBosMass/2 + mElectronP3.Px() * mNeutrinoP3.Px() + mElectronP3.Py() * mNeutrinoP3.Py();
   double a = mElectronP3.Pt() * mElectronP3.Pt();
   double b = -2 * A * mElectronP3.Pz();
   double c = mNeutrinoP3.Pt() * mNeutrinoP3.Pt() * mElectronP3.Mag() * mElectronP3.Mag() - A*A;

   double d = b*b - 4*a*c;
   if (d < 0 ) {
      Warning("ReconstructNeutrinoZ", "Problem reconstructing neutrino, d=%f will be set to 0", d);
      Info("ReconstructNeutrinoZ", "mElectronP3.Pt: %f, mElectronP3.Pz: %f, mNeutrinoP3.Pt: %f, A: %f", mElectronP3.Pt(), mElectronP3.Pz(), mNeutrinoP3.Pt(), A);
      d = 0;
   }
   double p_nu_z1  = (-b + sqrt(d) ) / 2 / a;
   double p_nu_z2  = (-b - sqrt(d) ) / 2 / a;

   if (fabs(p_nu_z1) < fabs(p_nu_z2)) {
      mNeutrinoP3.SetZ(p_nu_z1);
      mNeutrinoP3Other.SetZ(p_nu_z2);
   } else {
      mNeutrinoP3.SetZ(p_nu_z2);
      mNeutrinoP3Other.SetZ(p_nu_z1);
   }
}


void WBosEvent::Streamer(TBuffer &R__b)
{
   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(WBosEvent::Class(), this);
   }
   else {
      R__b.WriteClassBuffer(WBosEvent::Class(), this);
   }
}
