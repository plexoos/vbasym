
#include "VecBosVertex.h"


VecBosVertex::VecBosVertex() : mType(kUNKNOWN), id(-1), z(-1), mRank(-1), mRankLog(-1),
   nEEMCMatch(-1), mPosition(), eleTrack(), prTrList()
{
}


void VecBosVertex::SetPosition(const StThreeVectorF &vec)
{
   mPosition.SetX(vec.x());
   mPosition.SetY(vec.y());
   mPosition.SetZ(vec.z());
}


/** Checks the vertex paramteres against predefined cuts. */
bool VecBosVertex::IsGood()
{
   if (mType == kGOOD) return true;

   if ( (mRank > 0 || nEEMCMatch > 0) && fabs(mPosition.z()) <= 100)
   {
      mType = kGOOD;
      return true;
   }

   mType = kBAD;
   return false;
}


bool VecBosVertex::IsGood() const
{
   if (mType == kGOOD) return true;
   return false;
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
