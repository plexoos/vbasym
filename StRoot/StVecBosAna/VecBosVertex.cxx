
#include "VecBosVertex.h"


void VecBosVertex::clear()
{
   id                = -999;
   z                 = -999;
   funnyRank         = -9999;
   rank              = -9999;
   nEEMCMatch        = -999;
   eleTrack.clear();
}


void VecBosVertex::print(int flag)
{
   printf(" Vertex ID=%d Z=%.1f cm  nTrack=%d\n", id, z, eleTrack.size());
   for (uint i = 0; i < eleTrack.size(); i++)
      eleTrack[i].print();
}
