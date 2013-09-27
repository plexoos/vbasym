
#include "ZBosEvent.h"

#include "utils/utils.h"

ClassImp(ZBosEvent)

using namespace std;


const float ZBosEvent::sMinElectronPtLight = 15;
const float ZBosEvent::sMinElectronPtHard  = 25;
const float ZBosEvent::sMinNeutrinoPt      = 18;


ZBosEvent::ZBosEvent() : VecBosEvent(), mZBosMass(91.1876), mCand1P3(), mCand2P3()
{
}


ZBosEvent::~ZBosEvent()
{
}


VecBosTrack& ZBosEvent::GetElectronCandidate() const
{
   return *(*mTracksCandidate.begin());
}

TVector3 ZBosEvent::GetCandidate1_P3() const { return mCand1P3; }
TVector3 ZBosEvent::GetCandidate2_P3() const { return mCand2P3; }


TVector3 ZBosEvent::GetVecBosonP3() const { return GetCandidate1_P3() + GetCandidate2_P3(); }


/**
 * The primary method to identify and reconstruct the event with a Z boson.
 */
void ZBosEvent::Process()
{
   VecBosEvent::ProcessZ0();

   //here I have to loop over candidates. 

   // Make sure an isolated track exists
   if (mTracksCandidate.size() < 1) return;

   VecBosTrack &trackCand1  = **mTracksCandidate.begin();
   VecBosTrack trackCand2;

   VecBosTrackPtrSetIter iTrackCand = mTracksCandidate.begin();
   for (; iTrackCand != mTracksCandidate.end(); ++iTrackCand) {
      VecBosTrack &cand = **iTrackCand;

      if (cand == trackCand1) continue;

      if (cand.mStMuTrack->charge() != trackCand1.mStMuTrack->charge()) {
	// If the sign of the second track is different from the sign 
	// of the first track pick it as Z candidate otherwise gor further 
	trackCand2 = cand;
        break;
      }  else {
        //Info("Print", "this is no Z event ");
	break;
      }
   }

   mCand1P3 = trackCand1.GetP3EScaled();

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
