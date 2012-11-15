
#include <math.h>

#include "Globals.h"

#include "TVector3.h"


StEmcGeom  *gBTowGeom;
StEmcGeom  *mBSmdGeom[mxBSmd];
TVector3    gBCalTowerCoords[mxBtow];               // vs. tower ID
TVector3    mBSmdStripCoords[mxBSmd][mxBStrips];    // vs. strip ID
int         gMapBTowEtaPhiBin2Id[mxBTetaBin * mxBTphiBin];  // vs. (iEta, iPhi)


/**
 * Converts eta-phi real pair into (XXX:ds: preshower???) eta-phi bins.
 * Input eta range is from -0.99 to 0.99 only.
 * Returns true on success, false otherwise.
 */
bool ConvertEtaPhi2Bins(float eta, float phi, int &iEta, int &iPhi)
{
   if (phi < 0) phi += 2*M_PI; // I want phi in [0,2Pi]

   if (fabs(eta) >= 0.99) return false;

   // convention:  iPhi=[0,119], kEta=[1,40]
   int kEta = 1 + (int)((eta + 1.) / 0.05);
   iEta = kEta - 1;

   iPhi  = 24 - (int)( phi/M_PI/2 * 120.);
   iPhi += (iPhi < 0) ? 120 : 0;

   //printf("IJ=%d %d\n",iEta,iPhi);
   if (iEta < 0 || iEta >= mxBTetaBin) return false;
   if (iPhi < 0 || iPhi >= mxBTphiBin) return false;

   return true;
}


/**
 * For a given eta-phi bin considers all combinations of 2x2 bin clusters around it
 * and returns one with the maximum ET.
 */
WeveCluster FindMaxBTow2x2(VecBosEvent &vbEvent, int etaBin, int phiBin, float zVert)
{
   //printf("FindMaxBTow2x2  seed etaBin=%d phiBin=%d \n",etaBin, phiBin);
   const int L = 2; // size of the summed square

   WeveCluster maxCL;

   // just 4 cases of 2x2 clusters
   float maxET = 0;

   for (int iEta=etaBin-1; iEta<=etaBin; iEta++)
   {
      for (int iPhi=phiBin-1; iPhi<=phiBin; iPhi++)
      {
         WeveCluster cluster = SumBtowPatch(vbEvent, iEta, iPhi, L, L, zVert);
         if (maxET > cluster.ET) continue;
         maxET = cluster.ET;
         maxCL = cluster;
         // printf("   newMaxETSum=%.1f etaBin=%d iPhi=%d \n",maxET, I,J);
      }
   }

   //printf(" final inpEve=%d SumET2x2=%.1f \n",nInpEve,maxET);
   return maxCL;
}


WeveCluster SumBtowPatch(VecBosEvent &vbEvent, int etaBin, int phiBin, int etaWidth, int  phiWidth, float zVert)
{
   //printf("  eveID=%d btowSquare seed etaBin=%d[+%d] phiBin=%d[+%d] zVert=%.0f \n",mVecBosEvent->id,etaBin,etaWidth, phiBin,phiWidth,zVert);
   WeveCluster cluster; // object is small, not to much overhead in creating it
   cluster.iEta = etaBin;
   cluster.iPhi = phiBin;
   TVector3 R;
   double sumW = 0;
   float Rcylinder  = gBTowGeom->Radius();
   float Rcylinder2 = Rcylinder *Rcylinder;

   for (int iEta = etaBin; iEta < etaBin + etaWidth; iEta++) // trim in eta-direction
   {
      if (iEta < 0) continue;
      if (iEta >= mxBTetaBin) continue;

      for (int iPhi = phiBin; iPhi < phiBin + phiWidth; iPhi++)
      {
         // wrap up in the phi-direction
         int   iPhi_p  = (iPhi + mxBTphiBin) % mxBTphiBin;         // keep it always positive
         int   towerId = gMapBTowEtaPhiBin2Id[ iEta + iPhi_p*mxBTetaBin];
         float energy  = vbEvent.bemc.eneTile[kBTow][towerId - 1];

         //if (L<5) printf("n=%2d  iEta=%d iPhi_p=%d\n",cluster.nTower,iEta,iPhi_p);

         if (energy <= 0) continue; // skip towers w/o energy

         float adc    = vbEvent.bemc.adcTile[kBTow][towerId - 1];
         float delZ   = gBCalTowerCoords[towerId - 1].z() - zVert;
         float cosine = Rcylinder / sqrt(Rcylinder2 + delZ *delZ);
         float ET     = energy * cosine;
         float logET  = log10(ET + 0.5);

         cluster.nTower++;
         cluster.energy += energy;
         cluster.ET     += ET;
         cluster.adcSum += adc;

         if (logET > 0) {
            R    += logET * gBCalTowerCoords[towerId - 1];
            sumW += logET;
         }
         // if(etaWidth==2)
         //    printf("etaBin=%d phiBin=%d  ET=%.1f  energy=%.1f   sum=%.1f logET=%f sumW=%f\n",iEta,iPhi,ET,energy,cluster.energy,logET,sumW);
      }

      // printf(" end btowSquare: etaBin=%d  nTw=%d, ET=%.1f adc=%.1f\n",iEta,cluster.nTower,cluster.ET,cluster.adcSum);
      if (sumW > 0)
         cluster.position = 1. / sumW * R; // weighted cluster position
      else
         cluster.position = TVector3(0, 0, 999);
   }

   return cluster;
}
