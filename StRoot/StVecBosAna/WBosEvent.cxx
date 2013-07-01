
#include "WBosEvent.h"

#include "utils/utils.h"

ClassImp(WBosEvent)

using namespace std;


const float WBosEvent::sMinElectronPtLight = 15;
const float WBosEvent::sMinElectronPtHard  = 25;
const float WBosEvent::sMinNeutrinoPt      = 18;


WBosEvent::WBosEvent() : VecBosEvent(), mWBosMass(80.4), mElectronP3(), mNeutrinoP3()
{
}


WBosEvent::~WBosEvent()
{
}


VecBosTrack& WBosEvent::GetElectronTrack() const
{
   return *(*mTracksCandidate.begin());
}


TVector3 WBosEvent::GetElectronP3() const { return (*mTracksCandidate.begin())->GetP3EScaled(); }
TVector3 WBosEvent::GetNeutrinoP3() const { return mNeutrinoP3; }


TVector3 WBosEvent::GetMissingEnergyP3() const
{
   //if (mTracksCandidate.size() < 1) {
   //   Warning("GetMissingEnergyP3", "No track/lepton candidate: Cannot calculate P3Balance");
   //   return TVector3();
   //}

   return -1*(GetTrackRecoilTpcNeutrals() + GetElectronP3());
   //return mP3TrackRecoilTow + GetElectronP3();
   //return mP4JetRecoil + GetElectronP3();
}


TVector3 WBosEvent::GetVecBosonP3() const
{
   return -1*(GetElectronP3() + GetMissingEnergyP3());
}


void WBosEvent::Process()
{
   VecBosEvent::Process();

   // Proceed only if this is a W event, i.e. it conforms to W event signature
   if ( !PassedCutWBos() ) return;

   mElectronP3 = GetElectronP3();
   mNeutrinoP3 = GetMissingEnergyP3(); // we use only x and y components, and reconstruct z

   ReconstructNeutrinoZ();
}


void WBosEvent::Clear(const Option_t*)
{
   VecBosEvent::Clear();
   mElectronP3.SetXYZ(0, 0, 0);
   mNeutrinoP3.SetXYZ(0, 0, 0);
}


void WBosEvent::Streamer(TBuffer &R__b)
{
   //Info("Streamer", "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx ");
   VecBosEvent::Streamer(R__b);

   // Perhaps a temporary solution...
   ReconstructNeutrinoZ();
}


bool WBosEvent::PassedCutWBos(float minElePt) const
{
   if ( HasCandidateEle() && mPtBalanceCosPhiFromTracks >= sMinNeutrinoPt &&
        (*mTracksCandidate.begin())->GetP3EScaled().Pt() >= minElePt)
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
        (*mTracksCandidate.begin())->GetP3EScaled().Pt() >= minElePt)
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
      Warning("ReconstructNeutrinoZ", "Problem reconstructing neutrino d: %f will be set to 0", d);
      d = 0;
   }
   double p_nu_z1  = (-b + sqrt(d) ) / 2 / a;
   double p_nu_z2  = (-b - sqrt(d) ) / 2 / a;

   mNeutrinoP3.SetZ(p_nu_z1 < p_nu_z2 ? p_nu_z1 : p_nu_z2);
}
