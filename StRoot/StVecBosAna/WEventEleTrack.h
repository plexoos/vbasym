#ifndef WeveEleTrack_h
#define WeveEleTrack_h

#include <vector>

#include "TVector3.h"

#include <StMuDSTMaker/COMMON/StMuTrack.h>

#include "WanaConst.h"
#include "WEventCluster.h"


class WeveEleTrack;

//typedef std::vector<WeveEleTrack> VBTrackVec;
//typedef VBTrackVec::iterator      VBTrackVecIter;


// Track info
class WeveEleTrack
{
public:

   int              isMatch2Cl;         // result of cuts
   WevePointTower   pointTower;
   const StMuTrack *glMuTrack;
   const StMuTrack *prMuTrack;
   WeveCluster      mCluster2x2;
   WeveCluster      mCluster4x4;
   TVector3         primP;              // primary momentum vector
   float            nearTpcPT;
   float            nearEmcET;
   float            nearBtowET;
   float            nearEtowET;
   float            smallNearTpcPT;     // (GeV/c) around prim track direction
   float            awayTpcPT;
   float            awayEmcET;
   float            awayBtowET;
   float            awayEtowET;         // (GeV/c) opposite in phi to prim track direction
   float            nearTotET;
   float            awayTotET;
   float            nearTotET_noEEMC;
   float            awayTotET_noEEMC;   // (GeV) for nearCone 10 GeV is subtracted to avoid double counting
   TVector3         ptBalance;
   TVector3         ptBalance_noEEMC;
   float            sPtBalance;
   float            sPtBalance_noEEMC;   // signed pT balance (GeV/c)
   TVector3         hadronicRecoil;

   // esmd shower info
   int      hitSector;
   int      esmdGlobStrip[mxEsmdPlane];
   float    esmdShower[mxEsmdPlane][41];
   float    esmdDca[mxEsmdPlane];
   float    esmdDcaGlob[mxEsmdPlane];
   float    esmdE[mxEsmdPlane];
   int      esmdNhit[mxEsmdPlane];
   float    esmdShowerCentroid[mxEsmdPlane];
   float    esmdShowerWidth[mxEsmdPlane];
   TVector3 esmdXPcentroid;

   WeveEleTrack();

   void clear();
   void print( int flag = 0);
   TVector3 CalcDistanceToMatchedCluster();


   ClassDef(WeveEleTrack, 1);
};

typedef std::vector<WeveEleTrack> VBTrackVec;
typedef VBTrackVec::iterator      VBTrackVecIter;

#endif
