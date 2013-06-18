#ifndef Globals_h
#define Globals_h

#include <set>

#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StSpinPool/StJets/StJet.h"

#include "WanaConst.h"
#include "WEventCluster.h"


enum ESpinState        {kSPIN_DOWN = -1, kSPIN_NULL = 0, kSPIN_UP = +1};
enum EBeamId           {kBLUE_BEAM = 1,  kYELLOW_BEAM = 2, kUNKNOWN_BEAM};
enum EBeamSpinState    {kBLUE_UP = 4, kBLUE_DOWN = 8, kYELLOW_UP = 1, kYELLOW_DOWN = 2};
enum ESingleSpinState  {kBU_Y0 = 4, kBD_Y0 = 8, kB0_YU = 1, kB0_YD = 2};
enum EDoubleSpinState  {kBU_YU = 5, kBU_YD = 6, kBD_YU = 9, kBD_YD = 10};

typedef std::set<EBeamId>            BeamIdSet;
typedef BeamIdSet::iterator          BeamIdSetIter;

typedef std::set<ESingleSpinState>   SingleSpinStateSet;
typedef SingleSpinStateSet::iterator SingleSpinStateSetIter;

typedef std::set<EDoubleSpinState>   DoubleSpinStateSet;
typedef DoubleSpinStateSet::iterator DoubleSpinStateSetIter;

inline bool operator==(const StJet& lhs, const StJet& rhs) { return (TLorentzVector) lhs == (TLorentzVector) rhs; }
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

extern EBeamId            aBeams[2];
extern BeamIdSet          gBeams;
extern ESingleSpinState   aSingleSpinStates[4];
extern SingleSpinStateSet gSingleSpinStateSet;
extern EDoubleSpinState   aDoubleSpinStates[4];
extern DoubleSpinStateSet gDoubleSpinStateSet;


bool        ConvertEtaPhi2Bins(float etaF, float phiF, int &kEta, int &kPhi);
void        PatchToEtaPhi(int patch, int *eta, int *phi);
std::string AsString(EBeamId beamId);
std::string AsString(EBeamId beamId, ESpinState spinState);
std::string AsString(ESingleSpinState dss);
std::string AsString(EDoubleSpinState dss);

#endif
