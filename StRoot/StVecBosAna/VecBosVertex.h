#ifndef VecBosVertex_h
#define VecBosVertex_h

#include <vector>

#include "StThreeVectorF.hh"

#include "StMuDSTMaker/COMMON/StMuTrack.h"

#include "VecBosTrack.h"



// Info about vertex
class VecBosVertex
{
public:
   int      id;           // as store do muDst list
   float    z;            // cm
   float    mRank;
   float    mRankLog;
   int      nEEMCMatch;   //# of matched endcap towers
   TVector3 mPosition;

   VecBosTrackVec      eleTrack;
   vector<StMuTrack*>  prTrList;

   VecBosVertex();
   void SetPosition(const StThreeVectorF &vec);
   bool IsGood() const;
   void clear();
   void print(int flag=0);

   ClassDef(VecBosVertex, 1);
};

typedef std::vector<VecBosVertex>   VecBosVertexVec;
typedef VecBosVertexVec::iterator   VecBosVertexVecIter;

#endif
