
#include "WEventCluster.h"


WeveCluster::WeveCluster() { clear(); }


WeveCluster::~WeveCluster()
{ }


void WeveCluster::print(int flag) const
{
   printf("   Cluster ET=%.1f E=%.1f GeV, sumAdc=%.0f nTw=%d iEta=%d, iPhi=%d XYZ(%.0f,%.0f,%.0f)cm\n",
      ET, energy, adcSum, nTower, iEta, iPhi, position.x(), position.y(), position.z());
}


void WeveCluster::clear()
{
   position = TVector3(0, 0, 0);
   ET = energy = adcSum = 0;
   nTower = 0;
   iEta = iPhi = 999;
}


void WevePointTower::clear()
{
   id    = 0;
   R     = TVector3(0, 0, 0);
   Rglob = TVector3(0, 0, 0);
   iEta  = iPhi = 9999;
}


void WevePointTower::print(int flag) const
{
   printf("\n");
   Info("print(int flag)", "");
   printf("     pointed tower ID=%d; L2index: iEta=%d iPhi=%d; XYZ=(%.0f,%.0f,%.0f)cm\n",
          id, iEta, iPhi, R.x(), R.y(), R.z());
}
