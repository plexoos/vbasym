#ifndef WEventVertex_h
#define WEventVertex_h

#include <StMuDSTMaker/COMMON/StMuTrack.h>

#include "WEventEleTrack.h"


// info about vertex
class WEventVertex
{
public:
   int   id;           // as store do muDst list
   float z;            // cm
   float rank;
   float funnyRank;
   int   nEEMCMatch;   //# of matched endcap towers

   vector<WeveEleTrack>  eleTrack;
   vector<StMuTrack*>    prTrList;

   void clear() {
      id = -999; z = -999; funnyRank = -9999; rank = -9999;
      eleTrack.clear(); nEEMCMatch = -999;
   }

   void print( int flag = 0) {
      printf(" Vertex ID=%d Z=%.1f cm  nTrack=%d\n", id, z, eleTrack.size());
      for (uint i = 0; i < eleTrack.size(); i++)
         eleTrack[i].print();
   }

   ClassDef(WEventVertex, 1);
};

#endif
