#ifndef Globals_h
#define Globals_h

#include "StEmcUtil/geometry/StEmcGeom.h"

#include "WanaConst.h"
#include "WEventCluster.h"
#include "VecBosEvent.h" // for WeveBEMC


extern StEmcGeom  *gBTowGeom;
extern StEmcGeom  *mBSmdGeom[mxBSmd];
extern TVector3    gBCalTowerCoords[mxBtow];               // vs. tower ID
extern TVector3    mBSmdStripCoords[mxBSmd][mxBStrips];    // vs. strip ID
extern int         gMapBTowEtaPhiBin2Id[mxBTetaBin * mxBTphiBin];  // vs. (iEta, iPhi)


bool        ConvertEtaPhi2Bins(float etaF, float phiF, int &kEta, int &kPhi);
WeveCluster FindMaxBTow2x2(VecBosEvent &vbEvent, int iEta, int iPhi, float zVert);
WeveCluster SumBtowPatch(VecBosEvent &vbEvent, int iEta, int iPhi, int Leta, int  Lphi, float zVert);

#endif
