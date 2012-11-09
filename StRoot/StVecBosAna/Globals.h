#ifndef Globals_h
#define Globals_h

#include "StEmcUtil/geometry/StEmcGeom.h"

#include "WanaConst.h"

extern StEmcGeom *mBtowGeom;
extern int        mapBtowIJ2ID[mxBTetaBin * mxBTphiBin];  // vs. (iEta, iPhi)

bool ConvertEtaPhi2Bins(float etaF, float phiF, int &kEta, int &kPhi);

#endif
