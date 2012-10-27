#ifndef StVecBosMaker_h
#define StVecBosMaker_h

/*!
 *
 * \class  StVecBosMaker
 * \brief  muDst based extraction of W-signal from pp500 data from 2011
 *
 */

#include "StMaker.h"

#include <TChain.h>
#include <TRandom3.h>

#include "VecBosEvent.h"
#include "WtpcFilter.h"
#include "VecBosRootFile.h"


class  TObjArray;
class  TH1I;
class  TH2I;
class  StMuDstMaker;
class  StEmcGeom;
class  StEmcDecoder;
class  StBemcTables;
class  StEEmcDb;
class  StSpinDbMaker;
class  EEmcGeomSimple;
class  EEmcSmdGeom;
class  WeventDisplay;
class  St2011pubWanaMaker;
class  St2011pubSpinMaker;
class  St2011pubMcMaker;

class  StJetReader;
class  StJets;
class  StJet;
class  TClonesArray;
class  TTree;

class StVecBosMaker : public StMaker
{
   friend class WeventDisplay;
   friend class St2011pubWanaMaker;
   friend class St2011pubSpinMaker;
   friend class St2011pubMcMaker;
   friend class St2011WlumiMaker;
   friend class StZBosMaker;

private:
  
   StMuDstMaker   *mStMuDstMaker;
   StJetReader    *mStJetReader;
   VecBosRootFile *mVecBosRootFile;
   TString         mJetTreeBranch;
   TString         mJetTreeBranch_noEEMC;
   TClonesArray   *mJets;
   VecBosEvent         *mVecBosEvent;
   TTree          *mWtree;
   TString         mTreeName;
   TFile          *mTreeFile;
   WeventDisplay  *wDisaply;
   WtpcFilter      mTpcFilter[mxTpcSec];    //allows sector dependent filter
   WtpcFilter      mTpcFilterE[mxTpcSec];   //allows sector dependent filter for endcap tracks
   int             nInpEve;
   int             mNumTrigEvents;
   int             nAccEve;                 // event counters
   int             mRunNo;
   int             nRun;
   int             isMC;                    //0 for real data
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
   float par_nHitFrac, par_trackRin,  par_trackRout, par_trackPt;
   float parE_nHitFrac, parE_trackRin,  parE_trackRout, parE_trackPt;

   int   par_kSigPed, par_AdcThres;
   float par_maxADC, mMinBClusterEnergy, parE_clustET;
   float mMinBClusterEnergyIsoRatio, par_nearTotEtFrac;
   float mMinEClusterEnergyIsoRatio, parE_nearTotEtFrac;
   float par_nearDeltaR, par_awayDeltaPhi;
   float mMaxTrackClusterDist, parE_delR3D, par_highET, parE_highET,  par_ptBalance, parE_ptBalance;
   float mCutTrackEtaMin, mCutTrackEtaMax, parE_leptonEtaLow, parE_leptonEtaHigh; //bracket acceptance
   float parE_trackEtaMin;
   int   parE_nSmdStrip;

   float mParETOWScale;
   float mParBTOWScale;

   char *gains_file;
   int   use_gains_file;
   float gains_BTOW[4801];

   TString coreTitle;

  TH1F* hReweight; char* nameReweight;

public: // to overwrite default params from .C macro

   void setVertexCuts(float zm, int npv) {
      mCutVertexZ = zm; mMinNumPileupVertices = npv;
   }
   void setEleTrackCuts(int nfp, int hfr, float rin, float rout, float mpt) {
      par_nFitPts = nfp;  par_nHitFrac = hfr;
      par_trackRin = rin;  par_trackRout = rout; par_trackPt = mpt;
   }
   void setWbosonCuts(float a, float fr2,  float bal, float etaLow, float etaHigh) {
      par_highET = a; par_nearTotEtFrac = fr2;  par_ptBalance = bal;  mCutTrackEtaMin = etaLow; mCutTrackEtaMax = etaHigh;
   }
   void setE_WbosonCuts(float a, float fr2,  float bal, float etaLow, float etaHigh) {
      parE_highET = a; parE_nearTotEtFrac = fr2;  parE_ptBalance = bal;  parE_leptonEtaLow = etaLow; parE_leptonEtaHigh = etaHigh;
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
      mJetTreeBranch = jetTreeBranch; mJetTreeBranch_noEEMC = jetTreeBranch_noEEMC;
   }

   void setGainsFile(char *x) {gains_file = x; use_gains_file = 1;}
   void setTreeName(TString x) { mTreeName = x; }
   void setNameReweight(char* x) {nameReweight=x;}

private:

   //.... not used in the algo
   int par_DsmThres, parE_DsmThres;
   int par_maxDisplEve;

   StBemcTables *mBarrelTables;                          //used to access EMC status and ped info
   StEmcGeom    *mBtowGeom;
   StEmcGeom    *mBSmdGeom[mxBSmd];
   int           mapBtowIJ2ID[mxBTetaBin * mxBTphiBin];  // vs. (iEta, iPhi)
   TVector3      positionBtow[mxBtow];                   // vs. tower ID
   TVector3      positionBsmd[mxBSmd][mxBStrips];        // vs. strip ID

   StEEmcDb       *mDbE;       // access to EEMC database
   StSpinDbMaker  *spinDb;     // access spin information
   EEmcGeomSimple *mGeomEmc;   // access to EEMC geometry
   EEmcSmdGeom    *mGeomSmd;   // access to ESMD geometry
   TVector3 positionEtow[mxEtowSec *mxEtowSub][mxEtowEta];

   int   ReadMuDstBTOW();
   int   ReadMuDstETOW();
   int   ReadMuDstBarrelTrig();
   int   ReadMuDstEndcapTrig();
   void  ReadMuDstVertex();
   void  ReadMuDstTrack();
   void  ReadMuDstBSMD();
   void  ReadMuDstESMD();
   void  ReadMuDstEPRS();
   void  FillTowHit(bool hasVertices);
   void  FillNormHists();
   void  analyzeESMD();
   void  analyzeEPRS();

   bool  passes_L0();
   bool  passes_L2();
   void  find_W_boson();
   void  findEndcap_W_boson();
   void  tag_Z_boson();
   void  ExtendTrack2Barrel();
   bool  matchTrack2BtowCluster();
   int   ExtendTrack2Endcap();
   bool  matchTrack2EtowCluster();
   void  findNearJet();
   void  findAwayJet();
   void  CalcPtBalance();
   void  CalcMissingET();
   void  esmdAnalysis();

   // jets
   StJet*        GetJet(int i) {return (StJet *)mJets->At(i);}
   TClonesArray* GetJets(TString branchName);
   StJets*       GetStJets(int i) const;              // to make it backward compatible with SL09g
   StJets*       GetStJets(const char* bname) const;  // to make it backward compatible with SL09g
   StJets*       GetStJetsCopy(TString branchName);
   TClonesArray* GetJetsTreeAnalysis(TString branchName);

   // tools
   float sumTpcCone( int vertID, TVector3 refAxis, int flag, int pointTowId);
   float sumBtowCone( float zVert,  TVector3 refAxis, int flag);
   float sumEtowCone(float zVert, TVector3 refAxis, int flag);
   float sumTpcConeFromTree( int vertID, TVector3 refAxis, int flag, int pointTowId); //uses track vector saved in tree
   WeveCluster maxBtow2x2(int iEta, int iPhi, float zVert);
   WeveCluster sumBtowPatch(int iEta, int iPhi, int Leta, int  Lphi, float zVert);
   WeveCluster maxEtow2x1(int iEta, int iPhi, float zVert);
   WeveCluster maxEtow2x2(int iEta, int iPhi, float zVert);
   WeveCluster sumEtowPatch(int iEta, int iPhi, int Leta, int  Lphi, float zVert);
   void PatchToEtaPhi(int patch, int *eta, int *phi);


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
   void initGeom();
   bool ConvertEtaPhi2Bins(float etaF, float phiF, int &kEta, int &kPhi);

public:

   StVecBosMaker(const char *name = "StVecBosMaker", VecBosRootFile *vbFile = 0);
   virtual  ~StVecBosMaker() {};
   virtual Int_t Init();
   virtual Int_t Make();
   virtual Int_t Finish();

   virtual Int_t InitRun  (int runumber);
   virtual void Clear(const Option_t* = "");
   virtual Int_t FinishRun(int runumber);

   void setTrigID(int l2bw, int l2ew) { par_l2bwTrgID = l2bw; parE_l2ewTrgID = l2ew; }

   void setHList(TObjArray *x) {HList = x;}
   void setHListTpc(TObjArray *x) {HListTpc = x;}
   void setMC(int x) {isMC = x;}
   void setMaxDisplayEve(int n) { par_maxDisplEve = n;}
   void attachSpinDb(StSpinDbMaker *mk) { spinDb = mk;}

   //tree analysis
   void chainFile( const Char_t *name );
   void chainJetFile( const Char_t *name );
   Int_t getNumberOfEvents() { return mTreeChain->GetEntries(); }
   Int_t getEvent(Int_t event, Int_t eventJet);

protected:
   Int_t index, indexJet;
   TChain *mTreeChain;
   TChain *mJetTreeChain;

   /// Displayed on session exit, leave it as-is please ...
   virtual const char *GetCVS() const {
      static const char cvs[] = "";
      return cvs;
   }

   ClassDef(StVecBosMaker, 0)  //StAF chain virtual base class for Makers
};

#endif
