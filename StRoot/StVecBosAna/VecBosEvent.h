#ifndef VecBosEvent_h
#define VecBosEvent_h

#include "TVector3.h"
#include "TH1.h"
#include "TLorentzVector.h"

#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"

#include "StSpinPool/StJets/StJets.h"
#include "tables/St_g2t_tpc_hit_Table.h"
#include "StMcEventMaker/StMcEventMaker.h"
#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcVertex.hh"
#include "StMcEvent/StMcTrack.hh"

#include "Globals.h"
#include "WanaConst.h"
#include "VecBosEventInfo.h"
#include "VecBosVertex.h"
#include "VecBosTrack.h"
#include "WEvent.h"

#include "utils/ProtoEvent.h"


class VecBosEvent : public ProtoEvent
{
protected:

   StMuDst           *mStMuDst; //!

public:

   int                l2bitET;
   int                l2bitRnd;
   int                l2EbitET;
   int                l2EbitRnd;
   int                trigAwaySum[16];   // for lumi
   int                trigTotalSum;      // for lumi
   int                id;                // eventID
   int                runNo;
   int                time;
   float              zdcRate;
   int                bx7;               //!
   int                bx48;              //! raw from muDst
   int                bxStar7;           //!
   int                bxStar48;          //!
   int                spin4;             //! using spinDb or -1 if failed
   bool               zTag;
   UShort_t           mMuDstNumGTracks;
   UShort_t           mMuDstNumVertices;
   UShort_t           mMuDstNumPTracks;
   UShort_t           mMuDstNumOTracks;
   UShort_t           mNumGoodVertices;
   UShort_t           mNumGoodTracks;
   UShort_t           mNumBTracks;
   UShort_t           mNumETracks;
   UShort_t           mNumIsolatedTracks;
   StJets            *mStJets;          //!
   StJets            *mStJetsNoEndcap;  //! jets noEEMC
   WeveBEMC           bemc;             //!
   WeveETOW           etow;             //!
   WeveEPRS           eprs;             //!
   WeveESMD           esmd;             //!
   StJetPtrSet        mJets;            //
   VecBosVertexVec    mVertices;
   VecBosTrackVec     mTracks;
   VecBosTrackPtrVec  mTracksIsolated;
   WEvent            *mWEvent;
   TLorentzVector     mP4JetTotal;
   TLorentzVector     mP4JetFirst;
   TVector3           mP3RecoilFromTracks;
   Double_t           mHadRecoilFromTracksEta;
   Double_t           mHadRecoilFromTracksPt;
   Double_t           mHadronicRecoilEta;
   Double_t           mHadronicRecoilPt;
   Double_t           mPtKfactor;

   float mMaxTrackClusterDist;  //! cm, dist between projected track and center of cluster
   float mTrackIsoDeltaR;       //! (rad) near-cone size
   float mTrackIsoDeltaPhi;     //! (rad) away-'cone' size, approx. 40 deg.
   float mMinBTrackPt;          //!
   float mMinTrackHitFrac;      //!
   float mMinClusterEnergyFrac; //!
   float mMaxEnergyInOppsCone;  //!

   VecBosEvent();

   void           SetStMuDst(StMuDst *stMuDst) { mStMuDst = stMuDst; }
   VecBosVertex*  AddVertex(StMuPrimaryVertex &stMuVertex);
   void           AddVertex(VecBosVertex &vbVertex);
   void           AddTrack(StMuTrack *stMuTrack, VecBosVertex *vbVertex=0);
   void           AddStJets(StJets *stJets, StJets *stJetsNoEndcap);
   TClonesArray*  GetJets();
   TClonesArray*  GetJetsNoEndcap();
   UInt_t         GetNumJets();
   UInt_t         GetNumJetsNoEndcap();
   UInt_t         GetNumVertices()       { return mVertices.size(); }
   UInt_t         GetNumTracks()         { return mTracks.size(); }
   UShort_t       GetNumGoodVertices()   { return mNumGoodVertices; }
   UShort_t       GetNumGoodTracks()     { return mNumGoodTracks; }
   UShort_t       GetNumBTracks()        { return mNumBTracks; }
   UShort_t       GetNumETracks()        { return mNumETracks; }
   UShort_t       GetNumIsolatedTracks() { return mNumIsolatedTracks; }
   UInt_t         GetNumTracksWithBCluster();
   bool           HasGoodVertex()        { return mNumGoodVertices   > 0 ? true : false; } // Checks if at least one good vertex exist in the event
   bool           HasGoodTrack()         { return mNumGoodTracks     > 0 ? true : false; }
   bool           HasIsolatedTrack()     { return mNumIsolatedTracks > 0 ? true : false; }

   void           Process();
   void           ProcessMC();
   void           CalcRecoilFromTracks();
   void           MCanalysis();
   void           clear();
   void           Print(int flag = 0, int isMC = 0);
   void           getGmt_day_hour(int &yyyymmdd, int &hhmmss);

   WeveCluster    FindMaxBTow2x2(int iEta, int iPhi, float zVert);
   WeveCluster    SumBTowPatch  (int iEta, int iPhi, int Leta, int  Lphi, float zVert);
   TVector3       CalcP3InConeTpc(VecBosTrack *vbTrack, UShort_t cone1d2d=2, Float_t scale=1);
   TVector3       CalcP3InConeBTow(VecBosTrack *vbTrack, UShort_t cone1d2d=2, Float_t scale=1);
   TVector3       CalcP3InConeETow(VecBosTrack *vbTrack, UShort_t cone1d2d=2, Float_t scale=1);
   //float          SumETowCone   (float zVert, TVector3 refAxis, int flag);
   //float          SumTpcCone    (int vertID, TVector3 refAxis, int flag, int pointTowId);

   ClassDef(VecBosEvent, 2);
};

#endif
