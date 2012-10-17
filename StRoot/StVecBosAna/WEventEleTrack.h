#ifndef WeveEleTrack_h
#define WeveEleTrack_h

#include <StMuDSTMaker/COMMON/StMuTrack.h>

#include "WEventCluster.h"


class WeveEleTrack   // electron track info
{

public:

   int              isMatch2Cl;         // result of cuts
   WevePointTower   pointTower;
   const StMuTrack *glMuTrack;
   const StMuTrack *prMuTrack;
   WeveCluster      cluster;
   WeveCluster      cl4x4;
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

   TVector3 ptBalance;
   TVector3 ptBalance_noEEMC;
   float    sPtBalance;
   float    sPtBalance_noEEMC;   // signed pT balance (GeV/c)

   TVector3 hadronicRecoil;

   //esmd shower info
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

   WeveEleTrack() { clear();}

   void clear() {
      pointTower.clear();
      cluster.clear(); cl4x4.clear();  isMatch2Cl = false;  primP = TVector3(0, 0, 0);
      prMuTrack = glMuTrack = 0;
      awayTpcPT = nearTpcPT = nearTotET = awayTotET = nearEmcET = awayEmcET = 0;
      nearBtowET = awayBtowET = nearEtowET = awayEtowET = smallNearTpcPT = 0;
      nearTotET_noEEMC = awayTotET_noEEMC = 0;

      ptBalance = TVector3(0, 0, 0); ptBalance_noEEMC = TVector3(0, 0, 0);
      sPtBalance = sPtBalance_noEEMC = 0;

      hadronicRecoil = TVector3(0, 0, 0);

      memset(esmdGlobStrip, -999, sizeof(esmdGlobStrip));
      memset(esmdDca, -999., sizeof(esmdDca));
      memset(esmdDcaGlob, -999., sizeof(esmdDcaGlob));
      memset(esmdE, 0., sizeof(esmdE));
      memset(esmdNhit, 0, sizeof(esmdNhit));
      memset(esmdShowerCentroid, 999., sizeof(esmdShowerCentroid));
      memset(esmdShowerWidth, 999., sizeof(esmdShowerWidth));

      esmdXPcentroid = TVector3(0, 0, 0);
   }

   void print( int flag = 0) {
      if (prMuTrack == 0) {  printf("   Track NULL pointer???\n"); return;}
      printf("   Track glPT=%.1f GeV/c   isMatch2Cl=%d, nearTotET=%.1f, awayTotET=%.1f primPT=%.1f\n",
             glMuTrack->pt(), isMatch2Cl, nearTotET, awayTotET, primP.Pt());
      pointTower.print(flag);
      cluster.print(flag);
      TVector3 D = pointTower.R - cluster.position;
      printf("                XYZ(track-cluster):  |3D dist|=%.1fcm  delZ=%.1fcm\n", D.Mag(), D.z());
      printf("     4x4 :"); cl4x4.print(flag);
      printf("     nearET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n", nearTpcPT, nearEmcET, nearBtowET, nearEtowET, nearTotET);
      printf("     awayET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n", awayTpcPT, awayEmcET, awayBtowET, awayEtowET, awayTotET);
   }

   ClassDef(WeveEleTrack, 1);
};

#endif
