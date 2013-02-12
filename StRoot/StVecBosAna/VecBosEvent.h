#ifndef VecBosEvent_h
#define VecBosEvent_h

#include "TVector3.h"
#include "TH1.h"
#include "TLorentzVector.h"

#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"

#include "StSpinPool/StJets/StJets.h"
#include "StSpinPool/StSpinDbMaker/cstructs/spinConstDB.hh"
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
#include "VecBosJet.h"
#include "WEvent.h"

#include "utils/ProtoEvent.h"

class StJet;


class VecBosEvent : public ProtoEvent
{
protected:

   StMuDst           *mStMuDst;          //!

public:

   int                 l2bitET;
   int                 l2bitRnd;
   int                 l2EbitET;
   int                 l2EbitRnd;
   int                 trigAwaySum[16];   // for lumi
   int                 trigTotalSum;      // for lumi
   int                 id;                // eventID
   int                 runNo;
   int                 time;
   float               zdcRate;
   int                 bx7;               //!
   int                 bx48;              //! raw from muDst
   int                 bxStar7;           //!
   int                 bxStar48;          //!
   int                 mSpinPattern4Bits; // using spinDb or -1 if failed
	int                 mSpinDirection;    // use spinDbEnum to interpret the value
   bool                zTag;
   Double_t            mCpuTimeEventAna;
   Double_t            mCpuTimeHistFill;
   UShort_t            mMuDstNumGTracks;
   UShort_t            mMuDstNumVertices;
   UShort_t            mMuDstNumPTracks;
   UShort_t            mMuDstNumOTracks;
   UShort_t            mNumGoodVertices;
   UShort_t            mNumGoodTracks;
   UShort_t            mNumBTracks;
   UShort_t            mNumETracks;
   UShort_t            mNumIsolatedTracks;
   UShort_t            mNumCandidateTracks;
   StJets             *mStJets;            //!
   StJets             *mStJetsNoEndcap;    //! jets noEEMC
   WeveBEMC            bemc;               //!
   WeveETOW            etow;               //!
   WeveEPRS            eprs;               //!
   WeveESMD            esmd;               //!
   VecBosJetPtrSet     mJets;              // owns jets
   VecBosJetPtrSet     mJetsRecoil;        //
   VecBosJetPtrSet     mJetsWithIsoTrack;  //
   VecBosVertexPtrSet  mVertices;
   VecBosTrackPtrSet   mTracks;            // owns tracks
   VecBosTrackPtrSet   mTracksCandidate;
   WEvent             *mWEvent;
   TLorentzVector      mP4JetTotal;
   TLorentzVector      mP4JetFirst;
   TLorentzVector      mP4JetRecoil;
   TVector3            mP3TrackRecoilTpc;
   TVector3            mP3TrackRecoilTow;
   TVector3            mP3RecoilFromTracks;
   Double_t            mPtKfactor;
   Float_t             mMinVertexDeltaZ;            // min distance along z between vertices
   TVector3            mP3BalanceFromTracks;
   Double_t            mPtBalanceFromTracks;
   Double_t            mBalanceDeltaPhiFromTracks; 
   Double_t            mPtBalanceCosPhiFromTracks; 

   static const float mTrackIsoDeltaR;       //! (rad) near-cone size
   static const float mTrackIsoDeltaPhi;     //! (rad) away-'cone' size, approx. 40 deg.
   static const float mMaxVertexJetDeltaZ;   //! distance between jet and vertex z coord, cm
   static const float mMaxTrackJetDeltaZ;    //! distance between jet and track z coord, cm
   static const float mMinTrackPt;           //!
   static const float mMinBTrackPt;          //!
   static const float mMinTrackHitFrac;      //!
   static const float mMinClusterEnergyFrac; //!

   VecBosEvent();

   void           SetStMuDst(StMuDst *stMuDst) { mStMuDst = stMuDst; }
   VecBosVertex  *AddVertex(StMuPrimaryVertex &stMuVertex);
   void           AddVertex(VecBosVertex *vbVertex);
   void           AddTrack(StMuTrack *stMuTrack, VecBosVertex *vbVertex = 0);
   void           AddStJets(StJets *stJets, StJets *stJetsNoEndcap);
   TClonesArray  *GetStJets();
   TClonesArray  *GetStJetsNoEndcap();
   UInt_t         GetNumStJets();
   UInt_t         GetNumStJetsNoEndcap();
   UInt_t         GetNumVertices()        const { return mVertices.size(); }
   UInt_t         GetNumTracks()          const { return mTracks.size(); }
   UShort_t       GetNumGoodVertices()    const { return mNumGoodVertices; }
   UShort_t       GetNumGoodTracks()      const { return mNumGoodTracks; }
   UShort_t       GetNumBTracks()         const { return mNumBTracks; }
   UShort_t       GetNumETracks()         const { return mNumETracks; }
   UShort_t       GetNumIsolatedTracks()  const { return mNumIsolatedTracks; }
   UShort_t       GetNumCandidateTracks() const { return mNumCandidateTracks; }
   UInt_t         GetNumTracksWithBCluster();
   void           SetCpuTimeEventAna(Double_t time) { mCpuTimeEventAna = time; }
   void           SetCpuTimeHistFill(Double_t time) { mCpuTimeHistFill = time; }
   bool           HasGoodVertex()        const { return mNumGoodVertices    > 0 ? true : false; } // Checks if at least one good vertex exist in the event
   bool           HasGoodTrack()         const { return mNumGoodTracks      > 0 ? true : false; }
   bool           HasIsolatedTrack()     const { return mNumIsolatedTracks  > 0 ? true : false; }
   bool           HasCandidateTrack()    const { return mNumCandidateTracks > 0 ? true : false; }

   void           Process();
   void           ProcessMC();
   void           MCanalysis();
   bool           IsRecoilJet(VecBosJet *vbJet) const;
   void           clear();
   void           Print(int flag = 0, int isMC = 0);
   void           getGmt_day_hour(int &yyyymmdd, int &hhmmss);

   WeveCluster    FindMaxBTow2x2(int iEta, int iPhi, float zVert);
   WeveCluster    SumBTowPatch  (int iEta, int iPhi, int Leta, int  Lphi, float zVert);
   TVector3       CalcP3InConeBTow(VecBosTrack *vbTrack, UShort_t cone1d2d = 2, Float_t scale = 1);
   TVector3       CalcP3InConeETow(VecBosTrack *vbTrack, UShort_t cone1d2d = 2, Float_t scale = 1);
   TVector3       CalcP3InConeTpc (VecBosTrack *vbTrack, UShort_t cone1d2d = 2, Float_t scale = 1);

private:

   void           ProcessJets();
   void           CalcRecoilFromTracks();
   void           CalcRecoilFromTracks2();

   ClassDef(VecBosEvent, 2);
};

#endif
