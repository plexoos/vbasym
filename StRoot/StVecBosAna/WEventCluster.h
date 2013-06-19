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

   WeveCluster();
   ~WeveCluster();

   void clear();
   void print(int flag=0) const;

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

   void clear();
   void print(int flag = 0) const;

   ClassDef(WevePointTower, 1);
};

#endif
