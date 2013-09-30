
#include "ZBosEvent.h"

#include "utils/utils.h"

ClassImp(ZBosEvent)

using namespace std;


const float ZBosEvent::sMinElectronPtLight = 15;
const float ZBosEvent::sMinElectronPtHard  = 25;
const float ZBosEvent::sMinNeutrinoPt      = 18;
const float ZBosEvent::sMinZEleCandPt      = 15; // S.Fazio 30Sep2013

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

   // Make sure an isolated track exists
   if (mTracksCandidate.size() < 1) return;

   VecBosTrack &trackCand1  = **mTracksCandidate.begin();

   mCand1P3 = trackCand1.GetP3EScaled();
   mP4Cand1.SetPxPyPzE(mCand1P3.Px(), mCand1P3.Py(), mCand1P3.Pz(), trackCand1.mCluster2x2.mEnergy);
   mP4Cand2.SetPxPyPzE(0, 0, 0, 0);

   //mP4Cand1.SetE(trackCand1.mCluster2x2.mEnergy)
   VecBosTrack trackCand2;

   // Loop over candidates. 
   VecBosTrackPtrSetIter iTrackCand = mTracksCandidate.begin();
   for (; iTrackCand != mTracksCandidate.end(); ++iTrackCand) {
      VecBosTrack &cand = **iTrackCand;

      if (cand == trackCand1) continue;

      if (cand.mStMuTrack->charge() != trackCand1.mStMuTrack->charge()) {
	// If the sign of the second track is different from the sign 
	// of the first track pick it as Z second electron candidate. 
	trackCand2 = cand;
        mCand2P3 = trackCand2.GetP3EScaled();
        mP4Cand2.SetPxPyPzE(mCand2P3.Px(), mCand2P3.Py(), mCand2P3.Pz(), trackCand1.mCluster2x2.mEnergy);
        //mP4Cand2.SetE(trackCand1.mCluster2x2.mEnergy)
        break;
      }  else {
        //Info("Print", "this is no Z event ");
        mCand2P3 = -99;
	break;
      }
   }

   CalcZBosP4();
}


void ZBosEvent::Clear(const Option_t*)
{
   VecBosEvent::Clear();
}


bool ZBosEvent::PassedCutZBos(float minElePt) const
{
   if ( HasCandidateEle() && 
        mCand1P3.Pt() >= sMinZEleCandPt && 
        mCand2P3.Pt() >= sMinZEleCandPt )
   {
      return true;
   }

   return false;
}

void ZBosEvent::CalcZBosP4()
{
   mP4ZBoson = mP4Cand1 + mP4Cand2;
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
