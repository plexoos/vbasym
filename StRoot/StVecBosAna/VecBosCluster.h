#ifndef VecBosCluster_h
#define VecBosCluster_h


#include "TLorentzVector.h"
#include "TVector3.h"


// Info BTOW cluster
class VecBosCluster
{
public:
   float    energy;
   float    ET;
   float    adcSum;
   int      nTower;       // with non-zero ADC>kSigPed
   int      iEta, iPhi;   // lower-left corner of the cluster, can be negative, L2-indexing convention
   TVector3 position;     // 3D ln(E) weighted sume of tower positions

   VecBosCluster();
   ~VecBosCluster();

   void clear();
   void print(int flag=0) const;

protected:

   UShort_t       mSize;       //! size of the cluster, i.e. number of towers along each side, e.g. mSizexmSize
   TLorentzVector mTowerBand1; //!
   TLorentzVector mTowerBand2; //!
   TLorentzVector mTowerBand3; //!

   ClassDef(VecBosCluster, 1);
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
