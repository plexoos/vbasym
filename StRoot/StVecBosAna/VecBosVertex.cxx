
#include "VecBosVertex.h"


VecBosVertex::VecBosVertex() : TObject(), mType(kUNKNOWN), id(-1), z(-1), mRank(-1), mRankLog(-1),
   nEEMCMatch(-1), mPosition(), eleTrack(), prTrList()
{
}


VecBosVertex::VecBosVertex(StMuPrimaryVertex &stMuVertex) : TObject(),
   mType(kUNKNOWN), id(stMuVertex.idTruth()), z(0), mRank(-1), mRankLog(-1),
   nEEMCMatch(stMuVertex.nEEMCMatch()), mPosition(), eleTrack(), prTrList()
{
   z          = stMuVertex.position().z();
   mRank      = stMuVertex.ranking();
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
bool VecBosVertex::IsGood() const
{
   if (mType == kGOOD) return true;
   return false;
}


void VecBosVertex::Process()
{
   if ( (mRank > 0 || nEEMCMatch > 0) && fabs(mPosition.z()) <= 100)
   {
      mType = kGOOD;
   } else {
      mType = kBAD;
   }
}


void VecBosVertex::clear()
{
   mType      = kUNKNOWN;
   id         = -999;
   z          = -999;
   mRank      = -9999;
   mRankLog   = -9999;
   nEEMCMatch = -999;
   eleTrack.clear();
}


void VecBosVertex::print(int flag)
{
   printf(" Vertex ID=%d Z=%.1f cm  nTrack=%d\n", id, z, eleTrack.size());
   for (uint i = 0; i < eleTrack.size(); i++)
      eleTrack[i].print();
}
