#ifndef WEventCluster_h
#define WEventCluster_h


#include "TVector3.h"


// info BTOW cluster
class WeveCluster
{
public:
   float    energy;
   float    ET;
   float    adcSum;
   int      nTower;           // with non-zero ADC>kSigPed
   int      iEta, iPhi;       // lower-left corner of the cluster, can be negative, L2-indexing convention
   //int      iEtaE[4], iPhiE[4]; //index of cluster towers
   TVector3 position; // 3D ln(E) weighted sume of tower positions
   WeveCluster() { clear(); }
   ~WeveCluster() { }

   void clear() {
      position = TVector3(0, 0, 0);
      ET = energy = adcSum = 0;
      nTower = 0;
      iEta = iPhi = 999;
   }

   void print( int flag = 0) const
   {
      printf("     Cluster ET=%.1f E=%.1f GeV, sumAdc=%.0f nTw=%d iEta=%d, iPhi=%d XYZ(%.0f,%.0f,%.0f)cm\n", ET, energy, adcSum, nTower, iEta, iPhi, position.x(), position.y(), position.z());
   }

   ClassDef(WeveCluster, 1);
};


// tower pointed by the track
class WevePointTower : public TObject
{
public:
   TVector3 R;       // extrapolated position of primary track
   TVector3 Rglob;   // extrapolated position of global track
   int      id;      // BTOW tower id (!= 0), not used for ETOW
   int      iEta;
   int      iPhi;    // eta x phi bin using L2 indexing convention

   void clear()
   {
      id    = 0;
      R     = TVector3(0, 0, 0);
      Rglob = TVector3(0, 0, 0);
      iEta  = iPhi = 9999;
   }

   void print(int flag = 0) const
   {
      printf("\n");
      Info("print(int flag)", "");
      printf("     pointed tower ID=%d; L2index: iEta=%d iPhi=%d; XYZ=(%.0f,%.0f,%.0f)cm\n",
             id, iEta, iPhi, R.x(), R.y(), R.z());
   }

   ClassDef(WevePointTower, 1);
};

#endif
