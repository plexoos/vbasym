
#include "WBosEvent.h"

#include "utils/utils.h"

ClassImp(WBosEvent)

using namespace std;


const float WBosEvent::sMinElectronPtLight = 15;
const float WBosEvent::sMinElectronPtHard  = 25;
const float WBosEvent::sMinNeutrinoPt      = 18;


WBosEvent::WBosEvent() : VecBosEvent()
{
}


WBosEvent::~WBosEvent()
{
}


VecBosTrack& WBosEvent::GetElectronCandidate() const
{
   return *(*mTracksCandidate.begin());
}


TVector3 WBosEvent::GetElectronCandidateP3() const
{
   return (*mTracksCandidate.begin())->GetP3EScaled();
}


TVector3 WBosEvent::GetMissingEnergyP3() const
{
   //if (mTracksCandidate.size() < 1) {
   //   Warning("GetMissingEnergyP3", "No track/lepton candidate: Cannot calculate P3Balance");
   //   return TVector3();
   //}

   return -1*(GetTrackRecoilTpcNeutrals() + GetElectronCandidateP3());
   //return mP3TrackRecoilTow + GetElectronCandidateP3();
   //return mP4JetRecoil + GetElectronCandidateP3();
}


TVector3 WBosEvent::GetVecBosonP3() const
{
   return -1*(GetElectronCandidateP3() + GetMissingEnergyP3());
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
   return PassedCutWBos(minElePt) && GetElectronCandidate().GetChargeSign() > 0;
}


bool WBosEvent::PassedCutWBosMinus(float minElePt) const
{
   return PassedCutWBos(minElePt) && GetElectronCandidate().GetChargeSign() < 0;
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
