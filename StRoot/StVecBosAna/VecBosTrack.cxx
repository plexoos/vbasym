
#include "WEventEleTrack.h"


ClassImp(VecBosTrack)

using namespace std;


VecBosTrack::VecBosTrack()
{
   clear();
}


void VecBosTrack::print(int flag)
{
   if (prMuTrack == 0) {  printf("   Track NULL pointer???\n"); return;}
   printf("   Track glPT=%.1f GeV/c   isMatch2Cl=%d, nearTotET=%.1f, awayTotET=%.1f primPT=%.1f\n",
          glMuTrack->pt(), isMatch2Cl, nearTotET, awayTotET, primP.Pt());
   pointTower.print(flag);
   mCluster2x2.print(flag);
   TVector3 D = pointTower.R - mCluster2x2.position;
   printf("                XYZ(track-mCluster2x2):  |3D dist|=%.1fcm  delZ=%.1fcm\n", D.Mag(), D.z());
   printf("     4x4 :"); mCluster4x4.print(flag);
   printf("     nearET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n", nearTpcPT, nearEmcET, nearBtowET, nearEtowET, nearTotET);
   printf("     awayET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n", awayTpcPT, awayEmcET, awayBtowET, awayEtowET, awayTotET);
}


void VecBosTrack::clear()
{
   pointTower.clear();
   mCluster2x2.clear();
   mCluster4x4.clear();
   isMatch2Cl = false;
   primP = TVector3(0, 0, 0);
   prMuTrack = glMuTrack = 0;
   awayTpcPT = nearTpcPT = nearTotET = awayTotET = nearEmcET = awayEmcET = 0;
   nearBtowET = awayBtowET = nearEtowET = awayEtowET = smallNearTpcPT = 0;
   nearTotET_noEEMC = awayTotET_noEEMC = 0;

   ptBalance = TVector3(0, 0, 0);
   ptBalance_noEEMC = TVector3(0, 0, 0);
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


TVector3 VecBosTrack::CalcDistanceToMatchedCluster()
{
   return pointTower.R - mCluster2x2.position;
}
