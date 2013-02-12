
#include "VecBosVertex.h"

#include "VecBosEvent.h"


VecBosVertex::VecBosVertex() : TObject(), mEvent(0), mType(kUNKNOWN), mId(-1),
   mIdMuDst(-1), z(-1), mRank(-1), mRankLog(-1), nEEMCMatch(-1), mPosition(),
   mTracks(), eleTrack(), prTrList()
{
}


VecBosVertex::VecBosVertex(StMuPrimaryVertex &stMuVertex) : TObject(),
   mEvent(0), mType(kUNKNOWN), mId(-1), mIdMuDst(stMuVertex.idTruth()), z(0),
   mRank(-1), mRankLog(-1), nEEMCMatch(stMuVertex.nEEMCMatch()), mPosition(),
   eleTrack(), prTrList()
{
   z     = stMuVertex.position().z();
   mRank = stMuVertex.ranking();

   if (mRank > 1e6)    mRankLog = log(mRank - 1e6) + 10;
   else if (mRank > 0) mRankLog = log(mRank);
   else                mRankLog = log(mRank + 1e6) - 10;

   SetPosition(stMuVertex.position());
}


void VecBosVertex::SetPosition(const StThreeVectorF &vec)
{
   mPosition.SetX(vec.x());
   mPosition.SetY(vec.y());
   mPosition.SetZ(vec.z());
}


/** Checks the vertex paramteres against predefined cuts. */
void VecBosVertex::Process()
{
   //if ( (mRank > 0 || nEEMCMatch > 0) && fabs(mPosition.z()) <= 100)
   if ( mRank > 0 && fabs(mPosition.z()) <= 100)
   {
      mType = kGOOD;
   } else {
      mType = kBAD;
   }
}


void VecBosVertex::clear()
{
   mEvent     = 0;
   mType      = kUNKNOWN;
   mId        = -1;
   mIdMuDst   = -1;
   z          = -999;
   mRank      = -9999;
   mRankLog   = -9999;
   nEEMCMatch = -999;
   mTracks.clear();
   eleTrack.clear();
}


void VecBosVertex::Print(const Option_t* opt) const
{
   Info("Print(const Option_t* opt)", " Vertex mId=%d Z=%.1f cm  nTrack=%d\n", mId, z, mTracks.size());

   VecBosTrackPtrSetConstIter iTrack = mTracks.begin();
   for ( ; iTrack!=mTracks.end(); ++iTrack)
   {
      VecBosTrack &track = **iTrack;
      track.print();
   }
}
