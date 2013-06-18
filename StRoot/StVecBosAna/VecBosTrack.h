#ifndef VecBosTrack_h
#define VecBosTrack_h

#include <vector>
#include <set>

#include "TObject.h"
#include "TVector3.h"

#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StarClassLibrary/StPhysicalHelix.hh"

#include "Globals.h"
#include "WanaConst.h"
#include "WEventCluster.h"
#include "VecBosJet.h"


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
                    kHAS_CLUSTER = 0x0100,                     // has cluster above a threshold
                    kISOLATED    = 0x0200,                     // cluster/cone energy > 0.88
                    kIN_JET      = 0x0400,                     // track belongs to a jet
                    kUNBALANCED  = 0x0800,                     // does not have much energy in opposite direction
                    kCANDIDATE   = 0x1000                      // kHAS_CLUSTER && kISOLATED && !kIN_JET && !kBALANCED && Pt > XX GeV
                   };

   VecBosEvent            *mEvent;             //!
   UInt_t                  mVbType;
   Bool_t                  isMatch2Cl;         // result of cuts
   WevePointTower          mMatchedTower;
   const StMuTrack        *glMuTrack;          //!
   StMuTrack              *prMuTrack;          //
   StPhysicalHelixD        mHelix;             //!
   const VecBosVertex     *mVertex;            //! pointer to mother vertex
   Short_t                 mVertexId;          // mId of the mother vertex
   VecBosJet              *mJet;               //! pointer to mother/closest jet if exists
   WeveCluster             mCluster2x2;
   WeveCluster             mCluster4x4;
   TVector3                mP3AtDca;           // primary momentum vector
   TVector3                mP3AtBTow;          // momentum vector
   TVector3                mCoorAtBTow;        //
   TVector3                mP3InNearCone;
   TVector3                mP3InNearConeTow;
   TVector3                mP3InNearConeBTow;
   TVector3                mP3InNearConeETow;
   TVector3                mP3InNearConeNoETow;
   TVector3                mP3InNearConeTpc;
   TVector3                mP3InOppsCone;
   TVector3                mP3InOppsConeTow;
   TVector3                mP3InOppsConeBTow;
   TVector3                mP3InOppsConeETow;
   TVector3                mP3InOppsConeNoETow;
   TVector3                mP3InOppsConeTpc;
   float                   smallNearTpcPT;     //! GeV/c, around prim track direction
   float                   awayTpcPT;          //!
   float                   awayEmcET;          //!
   float                   awayBtowET;         //!
   float                   awayEtowET;         //! GeV/c, opposite in phi to prim track direction
   float                   awayTotET;          //!
   float                   awayTotET_noEEMC;   //! GeV, for nearCone 10 GeV is subtracted to avoid double counting
   UShort_t                mNumTracksInNearCone;
   TVector3                hadronicRecoil;     //!
   TVector3                ptBalance;          //!
   TVector3                ptBalance_noEEMC;   //!
   float                   sPtBalance;         //!
   float                   sPtBalance_noEEMC;  //! signed pT balance (GeV/c)
   float                   mMinDeltaZToJet;    // Min distance to a jet
   float                   mMinDeltaRToJet;    // Min distance to a jet
   TVector3                mDistToCluster;     // Distance to cluster

   // esmd shower info
   int                     hitSector; //!
   int                     esmdGlobStrip[mxEsmdPlane]; //!
   float                   esmdShower[mxEsmdPlane][41]; //!
   float                   esmdDca[mxEsmdPlane]; //!
   float                   esmdDcaGlob[mxEsmdPlane]; //!
   float                   esmdE[mxEsmdPlane]; //!
   int                     esmdNhit[mxEsmdPlane]; //!
   float                   esmdShowerCentroid[mxEsmdPlane]; //!
   float                   esmdShowerWidth[mxEsmdPlane];    //!
   TVector3                esmdXPcentroid;                  //!

   static const float      sMaxTrackClusterDist;            //! cm, dist between projected track and center of cluster
   static const float      sMinPt;                          //!
   static const float      sMaxEnergyInOppsCone;            //!
   static const float      sMinCandidateTrackClusterE;      //!

   VecBosTrack();
   ~VecBosTrack();

   bool        IsGood()       const { return (mVbType & kGOOD)   == kGOOD   ? true : false; }
   bool        IsBTrack()     const { return (mVbType & kBARREL) == kBARREL ? true : false; }
   bool        IsETrack()     const { return (mVbType & kENDCAP) == kENDCAP ? true : false; }
   bool        HasCluster()   const { return (mVbType & kHAS_CLUSTER) == kHAS_CLUSTER ? true : false; }
   bool        IsIsolated()   const { return (mVbType & kISOLATED) == kISOLATED ? true : false; }
   bool        IsInJet()      const { return (mVbType & kIN_JET) == kIN_JET ? true : false; }
   bool        IsUnBalanced() const { return (mVbType & kUNBALANCED) == kUNBALANCED ? true : false; }
   bool        IsCandidate()  const;
   //bool        HasBarrelMatched();
   //bool        HasEndcapMatched();
   void        Process();
   TVector3    GetP3AtDca()            const { return mP3AtDca; }
   TVector3    GetP3EScaled()          const { return mP3AtDca * ((Double_t) mCluster2x2.energy / mP3AtDca.Mag()); }
   float       GetFitHitFrac()         const { return float(prMuTrack->nHitsFit()) / prMuTrack->nHitsPoss(); }
   float       GetClusterEnergyFrac()  const { return (mCluster2x2.energy + mP3AtDca.Mag()) / mP3InNearConeNoETow.Mag(); }
   float       GetClusterETFrac()      const { return (mCluster2x2.ET     + mP3AtDca.Pt())  / mP3InNearConeNoETow.Perp(); }
   TVector3    GetDistanceToCluster()  const { return mDistToCluster; }
   TVector3    CalcDistanceToCluster() const { return mCoorAtBTow - mCluster2x2.position; }
   TVector3    GetCoordAtBTow() const { return mCoorAtBTow; }
   Short_t     GetVertexId()           const { return mVertexId; }
   void        SetVertexId(Short_t vId) { mVertexId = vId; }
   VecBosJet*  FindClosestJet(VecBosJetPtrSet &jets);
   void        clear();
   void        print(int opt=0) const;

private:

   bool ExtendTrack2Barrel();
   void MatchTrack2BtowCluster();
   void CalcEnergyInNearCone();

   ClassDef(VecBosTrack, 1);
};


typedef std::vector<VecBosTrack>          VecBosTrackVec;
typedef VecBosTrackVec::iterator          VecBosTrackVecIter;
typedef VecBosTrackVec::const_iterator    VecBosTrackVecConstIter;

typedef std::vector<VecBosTrack*>         VecBosTrackPtrVec;
typedef VecBosTrackPtrVec::iterator       VecBosTrackPtrVecIter;
typedef VecBosTrackPtrVec::const_iterator VecBosTrackPtrVecConstIter;

inline bool operator==(const VecBosTrack& lhs, const VecBosTrack& rhs) { return (TVector3) lhs.mP3AtDca == (TVector3) rhs.mP3AtDca; }
inline bool operator!=(const VecBosTrack& lhs, const VecBosTrack& rhs) { return !operator==(lhs,rhs); }
inline bool operator< (const VecBosTrack& lhs, const VecBosTrack& rhs) { return lhs.mP3AtDca.Mag() < rhs.mP3AtDca.Mag(); }
inline bool operator> (const VecBosTrack& lhs, const VecBosTrack& rhs) { return  operator< (rhs,lhs); }
inline bool operator<=(const VecBosTrack& lhs, const VecBosTrack& rhs) { return !operator> (lhs,rhs); }
inline bool operator>=(const VecBosTrack& lhs, const VecBosTrack& rhs) { return !operator< (lhs,rhs); }

struct CompareVecBosTrack
{
   bool operator()(const VecBosTrack& lhs, const VecBosTrack& rhs) const { return lhs > rhs; }
};

struct CompareVecBosTrackPtr
{
   bool operator()(const VecBosTrack* lhs, const VecBosTrack* rhs) const { return (*lhs) > (*rhs); }
};

/**
 * Sorting based on the total cluster energy. Therefore valid only for tracks
 * with cluster, i.e. candidates.
 */
struct CompareVecBosCandTrackPtr
{
   bool operator()(const VecBosTrack* lhs, const VecBosTrack* rhs) const
   {
      return lhs > rhs;
   }
};

typedef std::set<VecBosTrack, CompareVecBosTrack>     VecBosTrackSet;
typedef VecBosTrackSet::iterator                      VecBosTrackSetIter;
typedef VecBosTrackSet::const_iterator                VecBosTrackSetConstIter;

typedef std::set<VecBosTrack*, CompareVecBosTrackPtr> VecBosTrackPtrSet;
typedef VecBosTrackPtrSet::iterator                   VecBosTrackPtrSetIter;
typedef VecBosTrackPtrSet::const_iterator             VecBosTrackPtrSetConstIter;

typedef std::set<VecBosTrack*, CompareVecBosCandTrackPtr> VecBosCandTrackPtrSet;
typedef VecBosCandTrackPtrSet::iterator                   VecBosCandTrackPtrSetIter;
typedef VecBosCandTrackPtrSet::const_iterator             VecBosCandTrackPtrSetConstIter;

#endif
