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
                    kHAS_CLUSTER = 0x0100,                     // has cluster above a threshold
                    kISOLATED    = 0x0200,                     // cluster/cone energy > 0.88
                    kMISBALANCED = 0x0400                      // not much energy in opposite direction
                   };

   VecBosEvent            *mEvent;             //!
   UInt_t                  mType;
   int                     isMatch2Cl;         // result of cuts
   WevePointTower          mMatchedTower;
   const StMuTrack        *glMuTrack;          //!
   const StMuTrack        *prMuTrack;          //!
   StPhysicalHelixD        mHelix;             //!
   const VecBosVertex     *mVertex;            //! pointer to mother vertex
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
   float                   smallNearTpcPT;     //!(GeV/c) around prim track direction
   float                   awayTpcPT;          //!
   float                   awayEmcET;          //!
   float                   awayBtowET;         //!
   float                   awayEtowET;         //!(GeV/c) opposite in phi to prim track direction
   float                   awayTotET;          //!
   float                   awayTotET_noEEMC;   //!(GeV) for nearCone 10 GeV is subtracted to avoid double counting
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

   bool      IsGood()     const { return (mType & kGOOD)   == kGOOD   ? true : false; }
   bool      IsBTrack()   const { return (mType & kBARREL) == kBARREL ? true : false; }
   bool      IsETrack()   const { return (mType & kENDCAP) == kENDCAP ? true : false; }
   bool      HasCluster() const { return (mType & kHAS_CLUSTER) == kHAS_CLUSTER ? true : false; }
   bool      IsIsolated() const { return (mType & kISOLATED) == kISOLATED ? true : false; }
   //bool      HasBarrelMatched();
   //bool      HasEndcapMatched();
   void      Process();
   float     GetFitHitFrac() const { return float(prMuTrack->nHitsFit()) / prMuTrack->nHitsPoss(); }
   float     GetClusterEnergyFrac() const { return mCluster2x2.ET / mP3InNearConeNoETow.Pt(); }
   void      clear();
   void      print(int opt=0) const;
   TVector3  CalcDistanceToMatchedCluster();

private:

   bool ExtendTrack2Barrel();
   void MatchTrack2BtowCluster();
   void CalcEnergyInNearCone();
   void FindAwayJet();

   ClassDef(VecBosTrack, 1);
};

typedef std::vector<VecBosTrack>          VecBosTrackVec;
typedef VecBosTrackVec::iterator          VecBosTrackVecIter;
typedef VecBosTrackVec::const_iterator    VecBosTrackVecConstIter;
typedef std::vector<VecBosTrack*>         VecBosTrackPtrVec;
typedef VecBosTrackPtrVec::iterator       VecBosTrackPtrVecIter;
typedef VecBosTrackPtrVec::const_iterator VecBosTrackPtrVecConstIter;
typedef std::set<VecBosTrack*>            VecBosTrackPtrSet;
typedef VecBosTrackPtrSet::iterator       VecBosTrackPtrSetIter;
typedef VecBosTrackPtrSet::const_iterator VecBosTrackPtrSetConstIter;

#endif
