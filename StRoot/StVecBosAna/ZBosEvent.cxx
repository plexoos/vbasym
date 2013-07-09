
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


TVector3 ZBosEvent::GetVecBosonP3() const
{
   return -1*(GetElectronCandidateP3()); // XXX wrong, needs to be fixed
}


void ZBosEvent::Clear(const Option_t*)
{
   VecBosEvent::Clear();
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
