
#include "ZBosEvent.h"

#include "utils/utils.h"

ClassImp(ZBosEvent)

using namespace std;


const float ZBosEvent::sMinElectronPtLight = 15;
const float ZBosEvent::sMinElectronPtHard  = 25;
const float ZBosEvent::sMinNeutrinoPt      = 18;


ZBosEvent::ZBosEvent() : VecBosEvent(), mZBosMass(91.1876), mElectronP3(), mPositronP3()
{
}


ZBosEvent::~ZBosEvent()
{
}


VecBosTrack& ZBosEvent::GetElectronCandidate() const
{
   return *(*mTracksCandidate.begin());
}

TVector3 ZBosEvent::GetElectronP3() const { return mElectronP3; }
TVector3 ZBosEvent::GetPositronP3() const { return mPositronP3; }


//TVector3 ZBosEvent::GetElectronCandidateP3() const
//{
//   return (*mTracksCandidate.begin())->GetP3EScaled();
//}


//TVector3 ZBosEvent::GetVecBosonP3() const
//{
//   return -1*(GetElectronCandidateP3()); // XXX wrong, needs to be fixed
//}

TVector3 ZBosEvent::GetVecBosonP3() const { return GetElectronP3() + GetPositronP3(); }


/**
 * The primary method to identify and reconstruct the event with a Z boson.
 */
void ZBosEvent::Process()
{
   VecBosEvent::Process();
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


void ZBosEvent::Streamer(TBuffer &R__b)
{
   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ZBosEvent::Class(), this);
   }
   else {
      R__b.WriteClassBuffer(ZBosEvent::Class(), this);
   }
}
