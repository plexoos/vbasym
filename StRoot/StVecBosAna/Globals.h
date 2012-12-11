#ifndef Globals_h
#define Globals_h

#include <set>

#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StSpinPool/StJets/StJet.h"

#include "WanaConst.h"
#include "WEventCluster.h"


inline bool operator==(const StJet& lhs, const StJet& rhs) { return (TLorentzVector) lhs == (TLorentzVector) lhs; }
inline bool operator!=(const StJet& lhs, const StJet& rhs) { return !operator==(lhs,rhs); }
inline bool operator< (const StJet& lhs, const StJet& rhs) { return lhs.E() < rhs.E(); }
inline bool operator> (const StJet& lhs, const StJet& rhs) { return  operator< (rhs,lhs); }
inline bool operator<=(const StJet& lhs, const StJet& rhs) { return !operator> (lhs,rhs); }
inline bool operator>=(const StJet& lhs, const StJet& rhs) { return !operator< (lhs,rhs); }


struct CompareStJets
{
   bool operator()(const StJet* lhs, const StJet* rhs) const { return (*lhs) > (*rhs); }
};


typedef std::set<StJet*, CompareStJets>   StJetPtrSet;         // Jets are sorted according to their E
typedef StJetPtrSet::iterator             StJetPtrSetIter;
typedef StJetPtrSet::const_iterator       StJetPtrSetConstIter;


extern StEmcGeom  *gBTowGeom;
extern StEmcGeom  *mBSmdGeom[mxBSmd];
extern TVector3    gBCalTowerCoords[mxBtow];               // vs. tower ID
extern TVector3    mBSmdStripCoords[mxBSmd][mxBStrips];    // vs. strip ID
extern TVector3    gETowCoords[mxEtowSec *mxEtowSub][mxEtowEta];
extern int         gMapBTowEtaPhiBin2Id[mxBTetaBin * mxBTphiBin];  // vs. (iEta, iPhi)


bool ConvertEtaPhi2Bins(float etaF, float phiF, int &kEta, int &kPhi);
void PatchToEtaPhi(int patch, int *eta, int *phi);

#endif
