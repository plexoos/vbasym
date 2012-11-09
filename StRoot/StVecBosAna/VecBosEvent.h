#ifndef VecBosEvent_h
#define VecBosEvent_h

#include <set>

#include "TVector3.h"
#include "TH1.h"

#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"

#include "StSpinPool/StJets/StJet.h"
#include "StSpinPool/StJets/StJets.h"

#include "WanaConst.h"
#include "VecBosVertex.h"
#include "utils/ProtoEvent.h"


// W-reco event container
class WeveBEMC   // info about BEMC
{
public:
   // raw BTOW/BPRS hits
   int   tileIn[mxBTile]; // 0 if no data
   float adcTile[mxBTile][mxBtow];
   float eneTile[mxBTile][mxBtow];
   int   statTile[mxBTile][mxBtow];
   float maxAdc;
   int   maxHtDsm;

   //raw BSMD hits, both planes
   float adcBsmd[mxBSmd][mxBStrips];
   int   statBsmd[mxBSmd][mxBStrips];

   void clear() {
      memset(adcTile, 0, sizeof(adcTile));
      memset(eneTile, 0, sizeof(eneTile));
      memset(statTile, -1, sizeof(statTile)); // default all dead
      memset(tileIn, 0, sizeof(tileIn)); // detector was On/Off
      memset(adcBsmd, 0, sizeof(adcBsmd));
      memset(statBsmd, -1, sizeof(statBsmd)); // default all dead
      maxAdc   = 0;
      maxHtDsm = -1;
   }

   void print( int flag = 0) {
      printf(" BTOW tower ADC>500 list: ");
      for (int i = 0; i < mxBtow; i++) {
         if (adcTile[kBTow][i] < 500) continue;
         int id = i + 1;
         printf("id=%d adc=%.1f ene=%.1f;  ", id, adcTile[kBTow][i], eneTile[kBTow][i]);
      }    printf("\n");

      printf(" BSMDE tower ADC>200 list: ");

      for (int i = 0; i < mxBStrips; i++) {
         if (adcBsmd[ kBSE][i] < 200) continue;
         int id = i + 1;
         int module = 1 + i / 150;
         printf("id=%d mod=%d adc=%.1f ;  ", id, module, adcBsmd[ kBSE][i]);
      }

      printf("\n");
      printf(" BTOW maxAdc=%.1f  maxHtDsm=%d\n", maxAdc, maxHtDsm);

      if (flag & 1) {
         for (int i = 0; i < 120; i++) {
            int id = 7 + i * 40;
            if (i % 10 == 0) printf("\n  softID=%4d adc format  BTOW:BPRS=  ", id);
            printf("%.0f : %.0f, ", adcTile[kBTow][id - 1], adcTile[kBPrs][id - 1]);
         }
         printf("\n");
      }
   }

   ClassDef(WeveBEMC, 2);
};


class WeveETOW   // info about ETOW
{
public:
   //raw ETOW hit
   int etowIn;
   float adc[mxEtowSec *mxEtowSub][mxEtowEta]; //[phibin][etabin]
   float ene[mxEtowSec *mxEtowSub][mxEtowEta];
   int stat[mxEtowSec *mxEtowSub][mxEtowEta];
   float maxAdc;
   int maxSec, maxSub, maxEta;
   int maxHtDsm;

   void clear() {
      memset(adc, 0, sizeof(adc));
      memset(ene, 0, sizeof(ene));
      memset(stat, -1, sizeof(stat)); // default all dead
      maxAdc = 0;
      maxSec = maxSub = maxEta = 0;
      maxHtDsm = -1;
   }

   ClassDef(WeveETOW, 2);
};


class WeveEPRS   // info about EPRS
{
public:
   //raw EPRS hit
   int eprsIn;
   float adc[mxEtowSec *mxEtowSub][mxEtowEta][mxPrs]; //[phibin][etabin][layer]
   float ene[mxEtowSec *mxEtowSub][mxEtowEta][mxPrs];
   int stat[mxEtowSec *mxEtowSub][mxEtowEta][mxPrs];

   void clear() {
      memset(adc, 0, sizeof(adc));
      memset(ene, 0, sizeof(ene));
      memset(stat, -1, sizeof(stat)); // default all dead
   }

   ClassDef(WeveEPRS, 1);
};


class WeveESMD   // info about ESMD
{
public:
   //raw ESMD hit
   int   esmdIn;
   float adc[mxEtowSec][mxEsmdPlane][mxEsmdStrip]; //[phibin][etabin]
   float ene[mxEtowSec][mxEsmdPlane][mxEsmdStrip];
   int   stat[mxEtowSec][mxEsmdPlane][mxEsmdStrip];

   void clear() {
      memset(adc, 0, sizeof(adc));
      memset(ene, 0, sizeof(ene));
      memset(stat, -1, sizeof(stat)); // default all dead
   }

   ClassDef(WeveESMD, 1);
};


inline bool operator==(const StJet& lhs, const StJet& rhs) { return (TLorentzVector) lhs == (TLorentzVector) lhs; }
inline bool operator!=(const StJet& lhs, const StJet& rhs) { return !operator==(lhs,rhs); } 
inline bool operator< (const StJet& lhs, const StJet& rhs) { return lhs.E() < rhs.E(); }
inline bool operator> (const StJet& lhs, const StJet& rhs) { return  operator< (rhs,lhs); } 
inline bool operator<=(const StJet& lhs, const StJet& rhs) { return !operator> (lhs,rhs); } 
inline bool operator>=(const StJet& lhs, const StJet& rhs) { return !operator< (lhs,rhs); }


struct CompareStJets
{
   bool operator()(const StJet* lhs, const StJet* rhs) const { return (*lhs) < (*rhs); }
};


typedef std::set<StJet*, CompareStJets>   StJetPtrSet;
typedef StJetPtrSet::iterator             StJetPtrSetIter;
typedef StJetPtrSet::const_iterator       StJetPtrSetConstIter;


class VecBosEvent : public ProtoEvent
{
public:

   int                l2bitET;
   int                l2bitRnd;
   int                l2EbitET;
   int                l2EbitRnd;
   int                trigAwaySum[16];   //for lumi
   int                trigTotalSum;      //for lumi
   int                id;                // eventID
   int                runNo;
   int                time;
   float              zdcRate;
   int                bx7;
   int                bx48;              // raw from muDst
   int                bxStar7;
   int                bxStar48;
   int                spin4;             // using spinDb or -1 if failed
   bool               zTag;
   StJets            *mStJets;          //!
   StJets            *mStJetsNoEndcap;  //! jets noEEMC
   WeveBEMC           bemc;
   WeveETOW           etow;
   WeveEPRS           eprs;
   WeveESMD           esmd;
   StJetPtrSet        mJets;
   VecBosVertexVec    mVertices;
   VecBosTrackVec     mTracks;
   VecBosTrackPtrSet  mLeptonBTracks;   // Set of lepton track candidates, i.e. good tracks with energy in barrel
   VecBosTrackPtrSet  mLeptonETracks;   // Set of lepton track candidates, i.e. good tracks with energy in endcap

   VecBosEvent();

   VecBosVertex*       AddVertex(StMuPrimaryVertex &stMuVertex);
   void                AddVertex(VecBosVertex &vbVertex);
   void                AddTrack(StMuTrack *stMuTrack, VecBosVertex *vbVertex=0);
   void                AddStJets(StJets *stJets, StJets *stJetsNoEndcap);
   TClonesArray*       GetJets();
   TClonesArray*       GetJetsNoEndcap();
   UInt_t              GetNumJets();
   UInt_t              GetNumJetsNoEndcap();
   UInt_t              GetNumVertices();
   UInt_t              GetNumTracks();
   UInt_t              GetNumTracksWithBCluster();
   UInt_t              GetNumTracksWithBCluster2();
   bool                HasGoodVertex();
   bool                HasGoodTrack();
   void                Process();
   void                clear();
   void                print(int flag = 0, int isMC = 0);
   void                getGmt_day_hour(int &yyyymmdd, int &hhmmss);

   ClassDef(VecBosEvent, 2);
};

#endif
