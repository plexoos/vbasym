
#include <math.h>

#include "Globals.h"


StEmcGeom *mBtowGeom;
int        mapBtowIJ2ID[mxBTetaBin * mxBTphiBin];  // vs. (iEta, iPhi)


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
