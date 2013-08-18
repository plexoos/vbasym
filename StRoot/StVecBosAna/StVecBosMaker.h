#ifndef StVecBosMaker_h
#define StVecBosMaker_h

#include <stdint.h>

#include "StMaker.h"

#include "TChain.h"
#include "TRandom3.h"

#include "WBosEvent.h"
#include "VecBosVertex.h"
#include "WtpcFilter.h"
#include "VecBosRootFile.h"


class  TObjArray;
class  StMuDstMaker;
class  StBemcTables;
class  StEEmcDb;
class  StSpinDbMaker;
class  EEmcGeomSimple;
class  EEmcSmdGeom;
class  WeventDisplay;
class  St2011pubWanaMaker;
class  StVecBosSpinMaker;
class  St2011pubMcMaker;

class  StJetReader;
class  StJets;
class  StJet;
class  TClonesArray;


/**
 * \class  StVecBosMaker
 *
 * The main analysis maker to process the StMuDstMaker data. The Make() method fills the tree in the output file.
 *
 */
class StVecBosMaker : public StMaker
{
   friend class WeventDisplay;
   friend class St2011pubWanaMaker;
   friend class StVecBosSpinMaker;
   friend class St2011pubMcMaker;
   friend class St2011WlumiMaker;
   friend class StZBosMaker;

public:

   StVecBosMaker(const char *name = "StVecBosMaker", VecBosRootFile *vbFile = 0);
   virtual ~StVecBosMaker() {};

   virtual Int_t Init();
   virtual Int_t Make();
   virtual Int_t Finish();
   virtual Int_t InitRun (int runumber);
   virtual void  Clear(const Option_t* = "");
   virtual Int_t FinishRun(int runumber);

   VecBosEvent* GetVecBosEvent()                { return mVecBosEvent; }
   float        GetMinBClusterEnergyIsoRatio()  { return mMinBClusterEnergyIsoRatio; }
   float        GetNearTotEtFrac()              { return par_nearTotEtFrac; }
   float        GetPtBalance()                  { return par_ptBalance; }
   void         setTrigID(int l2bw, int l2ew)   { par_l2bwTrgID = l2bw; parE_l2ewTrgID = l2ew; }
   void         setHList(TObjArray *x)          { HList    = x; }
   void         setHListTpc(TObjArray *x)       { HListTpc = x; }
   void         setMC(int x)                    { mIsMc = x; }
   void         setMaxDisplayEve(int n)         { par_maxDisplEve = n; }
   void         AttachSpinDb(StSpinDbMaker *mk) { mStSpinDbMaker = mk; }

   // tree analysis
   Int_t getNumberOfEvents() { return mTreeChain->GetEntries(); }

   void setVertexCuts(float zm, int npv) { mCutVertexZ = zm; mMinNumPileupVertices = npv; }
   void setEleTrackCuts(int nfp, int hfr, float rin, float rout, float mpt) { par_nFitPts = nfp; par_trackRin = rin;  par_trackRout = rout; }
   void setWbosonCuts(float a, float fr2,  float bal, float etaLow, float etaHigh) {
      par_highET = a; par_nearTotEtFrac = fr2;  par_ptBalance = bal;  mMinBTrackEta = etaLow; mMaxBTrackEta = etaHigh;
   }
   void setE_WbosonCuts(float a, float fr2,  float bal, float etaLow, float etaHigh) {
      parE_highET = a; parE_nearTotEtFrac = fr2;  parE_ptBalance = bal;  mMinETrackEta = etaLow; mMaxETrackEta = etaHigh;
   }
   void setEmcCuts(int ksp , float madc, float clet, float fr1, float dr) {
      par_kSigPed = ksp; par_maxADC = madc; mMinBClusterEnergy = clet;
      mMinBClusterEnergyIsoRatio = fr1;
   }
   void SetEtowScale(float x) { mParETOWScale = x; }
   void SetBtowScale(float x) { mParBTOWScale = x; }
   void setL0AdcThresh(int x) { par_l0emulAdcThresh = x; }
   void setL2ClusterThresh(float x) { par_l2emulClusterThresh = x; }
   void setL2SeedThresh(float x) { par_l2emulSeedThresh = x; }
   void setJetTreeBranch(TString jetTreeBranch, TString jetTreeBranch_noEEMC) {
      mJetTreeBranchName         = jetTreeBranch;
      mJetTreeBranchNameNoEndcap = jetTreeBranch_noEEMC;
   }

   void setGainsFile(char *x) {gains_file = x; use_gains_file = 1;}
   void SetTreeName(TString x) { mTreeName = x; }
   void setNameReweight(char* x) {nameReweight=x;}

private:
  
   TStopwatch      mStopWatch;
   StMuDstMaker   *mStMuDstMaker;
   StJetReader    *mStJetReader;
   VecBosRootFile *mVecBosRootFile;
   TString         mJetTreeBranchName;
   TString         mJetTreeBranchNameNoEndcap;
   TClonesArray   *mJets;
   WBosEvent      *mVecBosEvent;
   TTree          *mVecBosTree;
   TString         mTreeName;
   TFile          *mTreeFile;
   WeventDisplay  *mEventDisplay;
   WtpcFilter      mTpcFilter[mxTpcSec];    //allows sector dependent filter
   WtpcFilter      mTpcFilterE[mxTpcSec];   //allows sector dependent filter for endcap tracks
   int             mNumInputEvents;
   int             mNumTrigEvents;
   int             mNumAcceptedEvents;      // event counters
   int             mRunNo;
   int             nRun;
   int             mIsMc;                   // 0 for real data
   int             Tfirst;
   int             Tlast;

   // internal params
   int   par_l0emulAdcThresh;
   float par_l2emulSeedThresh, par_l2emulClusterThresh;

   int   par_l2bwTrgID;
   int   parE_l2ewTrgID;
   float mCutVertexZ;
   int   mMinNumPileupVertices;

   int   par_nFitPts, parE_nFitPts;
   float par_nHitFrac, par_trackRin,  par_trackRout;
   float parE_nHitFrac, parE_trackRin,  parE_trackRout, mMinETrackPt;

   int   par_kSigPed, par_AdcThres;
   float par_maxADC, mMinBClusterEnergy, parE_clustET;
   float mMinBClusterEnergyIsoRatio, par_nearTotEtFrac;
   float mMinEClusterEnergyIsoRatio, parE_nearTotEtFrac;
   float parE_delR3D, par_highET, parE_highET,  par_ptBalance, parE_ptBalance;
   float mMinBTrackEta, mMaxBTrackEta, mMinETrackEta, mMaxETrackEta; //bracket acceptance
   float parE_trackEtaMin;
   int   parE_nSmdStrip;

   float mParETOWScale;
   float mParBTOWScale;

   char *gains_file;
   int   use_gains_file;
   float gains_BTOW[4801];

   TString coreTitle;
   TH1F* hReweight;
   char* nameReweight;

   // not used in the algo
   int par_DsmThres, parE_DsmThres;
   int par_maxDisplEve;

   StBemcTables *mBarrelTables;              // used to access EMC status and ped info
   TVector3 positionBtow[mxBtow];            // vs. tower ID
   TVector3 positionBsmd[mxBSmd][mxBStrips]; // vs. strip ID       
   TVector3 positionEtow[mxEtowSec*mxEtowSub][mxEtowEta];  

   StEEmcDb       *mDbE;            // access to EEMC database
   StSpinDbMaker  *mStSpinDbMaker;  // access spin information
   EEmcGeomSimple *mGeomEmc;        // access to EEMC geometry
   EEmcSmdGeom    *mGeomSmd;        // access to ESMD geometry

   int   ReadMuDstBTOW();
   int   ReadMuDstETOW();
   int   ReadMuDstBarrelTrig();
   int   ReadMuDstEndcapTrig();
   void  ReadMuDstBSMD();
   void  ReadMuDstESMD();
   void  ReadMuDstEPRS();
   void  ReadMuDstVerticesTracks();
   void  ReadMuDstTracks(VecBosVertex* vbVertex);
   void  ReadMuDstJets();
   void  FillTowHit(bool hasVertices);
   void  FillNormHists();
   void  AnalyzeESMD();
   void  AnalyzeEPRS();

   bool  passes_L0();
   bool  passes_L2();
   void  FindWBoson();
   void  FindWBosonEndcap();
   void  FindZBoson();
   void  FindNearJet();
   void  CalcPtBalance();
   void  esmdAnalysis();

   StJets*       GetStJets(const char* bname) const;  // to make it backward compatible with SL09g
   StJets*       GetStJetsFromTree(TString branchName);
   StJet*        GetJet(int i) { return (StJet*) mVecBosEvent->mStJets->jets()->At(i); }
   TClonesArray* GetJets(TString branchName);
   TClonesArray* GetJetsTreeAnalysis(TString branchName);

   //float       SumTpcConeFromTree(int vertID, TVector3 refAxis, int flag, int pointTowId); //uses track vector saved in tree

   // histograms
   enum {mxHA = 300};
   enum {mxHE = 300};
   TObjArray *HList;
   TObjArray *HListTpc;
   TH1       *hA[mxHA];
   TH1       *hE[mxHE];
   TH1       *hbxIdeal;

   void initHistos();
   void initEHistos();
   void InitGeom();

protected:

   Int_t   index;
   Int_t   indexJet;
   TChain *mTreeChain;
   TChain *mJetTreeChain;

   // Displayed on session exit, leave it as-is please ...
   virtual const char *GetCVS() const {
      static const char cvs[] = "";
      return cvs;
   }

   ClassDef(StVecBosMaker, 0)
};

#endif
