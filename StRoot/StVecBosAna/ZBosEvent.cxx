
#include "ZBosEvent.h"

#include "utils/utils.h"

ClassImp(ZBosEvent)

using namespace std;


const float ZBosEvent::sMinElectronPtLight = 15;
const float ZBosEvent::sMinElectronPtHard  = 25;
const float ZBosEvent::sMinNeutrinoPt      = 18;


ZBosEvent::ZBosEvent() : VecBosEvent()
{
}


ZBosEvent::~ZBosEvent()
{
}


VecBosTrack& ZBosEvent::GetElectronCandidate() const
{
   return *(*mTracksCandidate.begin());
}


TVector3 ZBosEvent::GetElectronCandidateP3() const
{
   return (*mTracksCandidate.begin())->GetP3EScaled();
}


TVector3 ZBosEvent::GetMissingEnergyP3() const
{
   //if (mTracksCandidate.size() < 1) {
   //   Warning("GetMissingEnergyP3", "No track/lepton candidate: Cannot calculate P3Balance");
   //   return TVector3();
   //}

   return -1*(GetTrackRecoilTpcNeutrals() + GetElectronCandidateP3());
   //return mP3TrackRecoilTow + GetElectronCandidateP3();
   //return mP4JetRecoil + GetElectronCandidateP3();
}


TVector3 ZBosEvent::GetVecBosonP3() const
{
   return -1*(GetElectronCandidateP3() + GetMissingEnergyP3());
}


bool ZBosEvent::PassedCutWBos(float minElePt) const
{
   if ( HasCandidateEle() && mPtBalanceCosPhiFromTracks >= sMinNeutrinoPt &&
        (*mTracksCandidate.begin())->GetP3EScaled().Pt() >= minElePt)
   {
      return true;
   }

   return false;
}


bool ZBosEvent::PassedCutWBosPlus(float minElePt) const
{
   return PassedCutWBos(minElePt) && GetElectronCandidate().GetChargeSign() > 0;
}


bool ZBosEvent::PassedCutWBosMinus(float minElePt) const
{
   return PassedCutWBos(minElePt) && GetElectronCandidate().GetChargeSign() < 0;
}


bool ZBosEvent::PassedCutQcdBkg(float minElePt) const
{
   if ( HasCandidateEle() && mPtBalanceCosPhiFromTracks < sMinNeutrinoPt &&
        (*mTracksCandidate.begin())->GetP3EScaled().Pt() >= minElePt)
   {
      return true;
   }

   return false;
}
