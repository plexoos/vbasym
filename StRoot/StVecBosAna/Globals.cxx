
#include <math.h>

#include "Globals.h"

#include "TVector3.h"


using namespace std;


StEmcGeom          *gBTowGeom;
StEmcGeom          *mBSmdGeom[mxBSmd];
TVector3            gBCalTowerCoords[mxBtow];               // vs. tower ID
TVector3            mBSmdStripCoords[mxBSmd][mxBStrips];    // vs. strip ID
TVector3            gETowCoords[mxEtowSec *mxEtowSub][mxEtowEta];
int                 gMapBTowEtaPhiBin2Id[mxBTetaBin * mxBTphiBin];  // vs. (iEta, iPhi)
EDoubleSpinState    aDoubleSpinStates[] = {kBU_YU, kBU_YD, kBD_YU, kBD_YD};
DoubleSpinStateSet  gDoubleSpinStateSet(aDoubleSpinStates, aDoubleSpinStates+4); 


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


/** Converts patch index to eta-phi bins? */
void PatchToEtaPhi(int patch, int *eta, int *phi)
{
   if (patch < 0 || patch > 299) {
      Error("PatchToEtaPhi", "PatchToEtaPhi p=%d, out of range. Eta phi not defined.\n", patch);
      return;
   }

   if (patch < 150) {
      int m = 14 - patch / 10;
      int n = patch % 10;
      *eta = n / 2 + 5;
      *phi = n % 2 + m * 2;
   }
   else {
      int m = 29 - patch / 10;
      int n = patch % 10;
      *eta = 4 - n / 2;
      *phi = 1 - n % 2 + m * 2;
   }
}


string AsString(EDoubleSpinState dss)
{
   switch (dss) {
   case kBU_YU:
	   return "uu";
   case kBU_YD:
	   return "ud";
   case kBD_YU:
	   return "du";
   case kBD_YD:
	   return "dd";
   default:
      return "UNK";
   }
}
