#ifndef VecBosVertex_h
#define VecBosVertex_h

#include <vector>

#include "StMuDSTMaker/COMMON/StMuTrack.h"

#include "VecBosTrack.h"


class VecBosVertex;

typedef std::vector<VecBosVertex>   VecBosVertexVec;
typedef VecBosVertexVec::iterator   VecBosVertexVecIter;


// Info about vertex
class VecBosVertex
{
public:
   int   id;           // as store do muDst list
   float z;            // cm
   float rank;
   float funnyRank;
   int   nEEMCMatch;   //# of matched endcap towers

   VecBosTrackVec      eleTrack;
   vector<StMuTrack*>  prTrList;

   void clear();
   void print(int flag=0);

   ClassDef(VecBosVertex, 1);
};

#endif
