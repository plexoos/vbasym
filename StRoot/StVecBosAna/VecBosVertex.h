#ifndef VecBosVertex_h
#define VecBosVertex_h

#include <vector>

//#include "TObject.h"

#include "StThreeVectorF.hh"

#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"

#include "VecBosTrack.h"


class VecBosEvent;


// Info about vertex
class VecBosVertex : public TObject
{
public:

   enum EVertexType {kUNKNOWN=0x0000, kBAD=0x1000, kGOOD=0x0001};

   VecBosEvent        *mEvent;       //!
   EVertexType         mType;
   int                 id;           // as store do muDst list
   float               z;            //! cm
   float               mRank;
   float               mRankLog;
   int                 nEEMCMatch;   // # of matched endcap towers
   TVector3            mPosition;
   VecBosTrackVec      eleTrack;
   vector<StMuTrack*>  prTrList;     //!

   VecBosVertex();
   VecBosVertex(StMuPrimaryVertex &stMuVertex);
   void SetPosition(const StThreeVectorF &vec);
   bool IsGood() const { return (mType & kGOOD) == kGOOD ? true : false; }
   void Process();
   void clear();
   virtual void Print(const Option_t* opt="") const;

   ClassDef(VecBosVertex, 1);
};

typedef std::vector<VecBosVertex>   VecBosVertexVec;
typedef VecBosVertexVec::iterator   VecBosVertexVecIter;

#endif
