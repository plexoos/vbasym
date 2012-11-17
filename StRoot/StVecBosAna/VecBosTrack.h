#ifndef VecBosTrack_h
#define VecBosTrack_h

#include <vector>
#include <set>

#include "TObject.h"
#include "TVector3.h"

#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StarClassLibrary/StPhysicalHelix.hh"

#include "WanaConst.h"
#include "WEventCluster.h"


class VecBosEvent;
class VecBosVertex;


// Track info
class VecBosTrack : public TObject
{
public:

   enum ETrackType {kUNKNOWN     = 0x0000,
                    kBAD         = 0x8000,
                    kGOOD        = 0x0001,                     // comes from a good vertex
                    kBARREL      = 0x0010, kENDCAP   = 0x0020, // from reco algo definition
                    kHAS_CLUSTER = 0x0100, // has cluster above a threshold
                    kISOLATED    = 0x0200
                   };

   VecBosEvent            *mEvent;             //!
   UInt_t                  mType;
   int                     isMatch2Cl;         // result of cuts
   WevePointTower          mMatchedTower;
   const StMuTrack        *glMuTrack;          //!
   const StMuTrack        *prMuTrack;          //!
   StPhysicalHelixD        mHelix;
   const VecBosVertex     *mVertex;            //! pointer to mother vertex
   WeveCluster             mCluster2x2;
   WeveCluster             mCluster4x4;
   TVector3                mVec3AtDca;         // primary momentum vector
   TVector3                mVec3AtBTow;        // primary momentum vector
   TVector3                mCoorAtBTow;        // 
   float                   nearTpcPT;
   float                   nearEmcET;
   float                   nearBtowET;
   float                   nearEtowET;
   float                   smallNearTpcPT;     // (GeV/c) around prim track direction
   float                   awayTpcPT;
   float                   awayEmcET;
   float                   awayBtowET;
   float                   awayEtowET;         // (GeV/c) opposite in phi to prim track direction
   float                   nearTotET;
   float                   awayTotET;
   float                   nearTotET_noEEMC;
   float                   awayTotET_noEEMC;   // (GeV) for nearCone 10 GeV is subtracted to avoid double counting
   TVector3                ptBalance;
   TVector3                ptBalance_noEEMC;
   float                   sPtBalance;
   float                   sPtBalance_noEEMC;   // signed pT balance (GeV/c)
   TVector3                hadronicRecoil;

   // esmd shower info
   int                     hitSector;
   int                     esmdGlobStrip[mxEsmdPlane];
   float                   esmdShower[mxEsmdPlane][41];
   float                   esmdDca[mxEsmdPlane];
   float                   esmdDcaGlob[mxEsmdPlane];
   float                   esmdE[mxEsmdPlane];
   int                     esmdNhit[mxEsmdPlane];
   float                   esmdShowerCentroid[mxEsmdPlane];
   float                   esmdShowerWidth[mxEsmdPlane];
   TVector3                esmdXPcentroid;

   VecBosTrack();

   bool IsGood()     const { return (mType & kGOOD)   == kGOOD   ? true : false; }
   bool IsBTrack()   const { return (mType & kBARREL) == kBARREL ? true : false; }
   bool IsETrack()   const { return (mType & kENDCAP) == kENDCAP ? true : false; }
   bool HasCluster() const { return (mType & kHAS_CLUSTER) == kHAS_CLUSTER ? true : false; }
   //bool HasBarrelMatched();
   //bool HasEndcapMatched();
   void Process();
   void clear();
   void print( int flag = 0);
   TVector3 CalcDistanceToMatchedCluster();

private:

   bool ExtendTrack2Barrel();
   void MatchTrack2BtowCluster();
   void FindNearJet();
   void FindAwayJet();

   ClassDef(VecBosTrack, 1);
};

typedef std::vector<VecBosTrack>      VecBosTrackVec;
typedef VecBosTrackVec::iterator      VecBosTrackVecIter;
typedef std::vector<VecBosTrack*>     VecBosTrackPtrVec;
typedef VecBosTrackPtrVec::iterator   VecBosTrackPtrVecIter;
typedef std::set<VecBosTrack*>        VecBosTrackPtrSet;
typedef VecBosTrackPtrSet::iterator   VecBosTrackPtrSetIter;

#endif
