
#include "VecBosTrack.h"

#include "VecBosVertex.h"


ClassImp(VecBosTrack)

using namespace std;


VecBosTrack::VecBosTrack() : mType(kUNKNOWN), mVecBosVertex(0)
{
   clear();
}


bool VecBosTrack::IsGood()
{
   if ( (mType & kGOOD) == kGOOD) return true;

   // Good track must come from a good vertex
   if (!mVecBosVertex || !mVecBosVertex->IsGood()) return false;

   if ( (prMuTrack->flag() == 301 || prMuTrack->flag() == 311) &&
         prMuTrack->pt() < 1.0) return true;

   return false;
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
   mType                = kUNKNOWN;
   isMatch2Cl           = false;
   pointTower.clear();
   glMuTrack            = 0;
   prMuTrack            = 0;
   mVecBosVertex        = 0;
   mCluster2x2.clear();
   mCluster4x4.clear();
   primP                = TVector3(0, 0, 0);
   awayTpcPT            = 0;
   nearTpcPT            = 0;
   nearTotET            = 0;
   awayTotET            = 0;
   nearEmcET            = 0;
   awayEmcET            = 0;
   nearBtowET           = 0;
   awayBtowET           = 0;
   nearEtowET           = 0;
   awayEtowET           = 0;
   smallNearTpcPT       = 0;
   nearTotET_noEEMC     = 0;
   awayTotET_noEEMC     = 0;
   ptBalance            = TVector3(0, 0, 0);
   ptBalance_noEEMC     = TVector3(0, 0, 0);
   sPtBalance           = 0;
   sPtBalance_noEEMC    = 0;
   hadronicRecoil       = TVector3(0, 0, 0);

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
