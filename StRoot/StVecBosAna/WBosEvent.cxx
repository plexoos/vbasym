
#include "WBosEvent.h"

#include "utils/utils.h"

ClassImp(WBosEvent)

using namespace std;


WBosEvent::WBosEvent() : VecBosEvent()
{
}


WBosEvent::~WBosEvent()
{
}


TVector3 WBosEvent::GetElectronCandidate() const
{
   return (*mTracksCandidate.begin())->GetP3EScaled();
}


TVector3 WBosEvent::GetMissingEnergy() const
{
   //if (mTracksCandidate.size() < 1) {
   //   Warning("GetMissingEnergy", "No track/lepton candidate: Cannot calculate P3Balance");
   //   return TVector3();
   //}

   return mP3TrackRecoilTpcNeutrals + GetElectronCandidate();
   //return mP3TrackRecoilTow + GetElectronCandidate();
   //return mP4JetRecoil + GetElectronCandidate();
}


TVector3 WBosEvent::GetVecBoson() const
{
   return -1*(GetElectronCandidate() + GetMissingEnergy());
}
