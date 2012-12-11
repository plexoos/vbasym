#include <limits>
//#include <math.h>

#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include <TString.h>
#include <StMessMgr.h>
#include <StThreeVectorF.hh>

#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include <StMuDSTMaker/COMMON/StMuTriggerIdCollection.h>
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include <StMuDSTMaker/COMMON/StMuTrack.h>
#include <StMuDSTMaker/COMMON/StMuPrimaryVertex.h>
#include "StTriggerUtilities/L2Emulator/L2wAlgo/L2wResult2009.h"

#include "StEmcUtil/database/StBemcTables.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include "StEEmcUtil/StEEmcSmd/EEmcSmdGeom.h"

#include "StEmcRawMaker/defines.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"

#include "StSpinPool/StJets/StJet.h"
#include "StSpinPool/StJets/StJets.h"
#include "StJetMaker/StJetMaker.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
#include "StJetMaker/StJetReader.h"
#include "StJetMaker/StJetSkimEventMaker.h"

#include "Globals.h"
#include "WeventDisplay.h"
#include "StVecBosMaker.h"

ClassImp(StVecBosMaker)


/** */
StVecBosMaker::StVecBosMaker(const char *name, VecBosRootFile *vbFile): StMaker(name),
   mStMuDstMaker(0), mStJetReader(0), mVecBosRootFile(vbFile),
   mJetTreeBranchName(), mJetTreeBranchNameNoEndcap(),
   mJets(0), mVecBosEvent(0), mWtree(0),
   mNumInputEvents(0), mNumTrigEvents(0), mNumAcceptedEvents(0),
   Tfirst(numeric_limits<int>::max()), Tlast(numeric_limits<int>::min()),
   mParETOWScale(1.0), mParBTOWScale(1.0)   // for old the Endcap geometr you need ~1.3
{
   mStMuDstMaker = (StMuDstMaker*) GetMaker("MuDst");

   coreTitle = name;

   if (!mStMuDstMaker) { // load tree if no MuDst
      Info("StVecBosMaker", "MuDst maker is not defined. Creating W tree");
      mTreeChain = new TChain("mWtree", "W candidate events");
      index = 0;
   }

   // must have either MuDst or W tree
   assert(mStMuDstMaker || mTreeChain);

   mStJetReader = (StJetReader*) GetMaker("JetReader");

   if (!mStJetReader && mTreeChain) { // jet chain for reading W tree
      mJetTreeChain = new TChain("jet", "Jet Tree");
      indexJet = 0;
   }

   if (!mJetTreeChain && !mStJetReader)
      LOG_WARN << GetName() << Form("::constructor() NO JETS , W-algo is not working properly, continue") << endm;

   // preset or clear some params
   par_l2bwTrgID  = 0;
   parE_l2ewTrgID = 0;

   setHList(0);
   setHListTpc(0);
   setMC(0);

   // MC trigger simulator
   par_l0emulAdcThresh          = 30;
   par_l2emulSeedThresh         = 5.0;
   par_l2emulClusterThresh      = 12.0;

   // vertex
   mMinNumPileupVertices        = 3;    // to reject events w/o TPC, lower it for MC
   mCutVertexZ                  = 100;  // (cm)

   // towers
   par_kSigPed                  = 3;    // rawADC-ped cut off
   par_AdcThres                 = 8;    // ADC threshold to avoid correlated noise
   par_maxADC                   = 200.; // (adc chan) on the highest tower in events

   // Barrel params
   mMinBClusterEnergy           = 14.;  // GeV/c 2x2 cluster ET
   mMinBClusterEnergyIsoRatio   = 0.95; // ET ratio 2x2/4x4 cluster
   par_nearTotEtFrac            = 0.88; // ratio 2x2/near Tot ET
   mMinBTrackEta                = -1.5; // bracket acceptance
   mMaxBTrackEta                = 1.5;  // bracket acceptance
   par_ptBalance                = 14.;  // GeV, ele cluster vector + jet sum vector
   par_nFitPts                  = 15;   // hits on the track
   par_trackRin                 = 90;   // cm
   par_trackRout                = 160;  // cm
   par_highET                   = 25.;  // (GeV), cut-off for final Barrel W-cluster

   // Endcap Algo
   parE_trackEtaMin             = 0.7;  // avoid bad extrapolation to ESMD
   parE_clustET                 = 14.;  // (GeV/c) 2x1 cluster ET
   mMinEClusterEnergyIsoRatio   = 0.90; // ET ratio 2x2/4x4 cluster
   parE_nearTotEtFrac           = 0.85; // ratio 2x2/near Tot ET
   parE_delR3D                  = 10.;  // cm, dist between projected track and center of cluster
   mMinETrackEta                = 0.7;  // bracket acceptance
   mMaxETrackEta                = 2.5;  // bracket acceptance
   parE_ptBalance               = 14.;  // (GeV), ele cluster vector + jet sum vector
   //... track
   parE_nFitPts                 = 5;    // hits on the track
   parE_nHitFrac                = 0.51;
   parE_trackRin                = 120;
   parE_trackRout               = 70;   // cm
   mMinETrackPt                 = 7.;   // GeV
   parE_nSmdStrip               = 20;
   parE_highET                  = 25.;  // (GeV), cut-off for final Endcap W-cluster

   mRunNo                       = 0;
   nRun                         = 0;
   hbxIdeal                     = 0;

   // irrelevant for W analysis
   par_DsmThres                 = 31;   // only for monitoring
   parE_DsmThres                = 31;   // only for monitoring
   par_maxDisplEve              = 1;    // # of displayed selected events

   use_gains_file               = 0;
}


Int_t StVecBosMaker::Init()
{
   if (mStMuDstMaker) {
      //only need DB tables for MuDst analysis
      mBarrelTables = new StBemcTables();
      mDbE = (StEEmcDb*) GetDataSet("StEEmcDb");
      assert(mDbE);
   }
   else { // not used
      //setup for reading in tree
      //mVecBosEvent = new VecBosEvent();
      //mTreeChain-> SetBranchAddress("mVecBosEvent", &mVecBosEvent);
   }

   gBTowGeom       = StEmcGeom::instance("bemc");
   mBSmdGeom[kBSE] = StEmcGeom::instance("bsmde");
   mBSmdGeom[kBSP] = StEmcGeom::instance("bsmdp");

   mGeomEmc = new EEmcGeomSimple();
   mGeomSmd = EEmcSmdGeom::instance();
   initGeom();

   wDisaply = new WeventDisplay(this, par_maxDisplEve);

   if (isMC) { // load vertex reweighting histo
      // TString filename="/star/u/stevens4/wAnalysis/efficXsec/zVertReweight.root";
      TString filename="/star/u/fazio/offline/users/fazio/vbasym/zVertReweight.root";
      //TFile* reweightFile = new TFile(filename);
      //cout<<"Re-weighting vertex z distribution with '"<<nameReweight<<"' histo from file "<<endl<<filename<<endl;
      //hReweight = (TH1F*) reweightFile->Get(nameReweight);

      mMinNumPileupVertices = 1;
   }

   // tree only written during MuDst analysis
   if (mStMuDstMaker) {
      mTreeFile = new TFile(mTreeName, "recreate");
      mTreeFile->cd();

      mWtree = new TTree("t", "mWtree");
      mVecBosEvent = new VecBosEvent();
      mWtree->Branch("e", "VecBosEvent", &mVecBosEvent);
   }

   assert(HList);
   mVecBosRootFile->cd();
   initHistos();
   initEHistos();

   return StMaker::Init();
}


/** */
Int_t StVecBosMaker::InitRun(int runNo)
{
   Info("InitRun", "Called for run %d", runNo);

   //if (!isMC) assert(mRunNo == 0); // to prevent run merging - it was not tested

   if (mStMuDstMaker) {
      mBarrelTables->loadTables(this);
      mRunNo = runNo;
   }
   //else {
   //   mRunNo = mVecBosEvent->runNo;
   //}

   // barrel algo params
   //LOG_INFO << Form("::InitRun(%d) %s done\n" \
   //   "Barrel W-algo params: trigID L2BW=%d isMC=%d\n" \
   //   "TPC: nPileupVert>%d, vertex |Z|<%.1fcm, primEleTrack: nFit>%d, hitFrac>%.2f Rin<%.1fcm, Rout>%.1fcm, PT>%.1fGeV/c\n" \
   //   "BTOW ADC: kSigPed=%d AdcThr>%d maxAdc>%.0f clustET>%.1f GeV  ET2x2/ET4x4>%0.2f  ET2x2/mP3InNearCone.Pt()>%0.2f\n" \
   //   "dist(track-clust)<%.1fcm, nearDelR<%.1f\n" \
   //   "W selection highET>%.1f awayDelPhi<%.1frad  ptBalance>%.1fGeV  %.1f<leptonEta<%.1f ",
   //   mRunNo, coreTitle.Data(), par_l2bwTrgID, isMC,
   //   mMinNumPileupVertices, mCutVertexZ,
   //   par_nFitPts, par_trackRin,  par_trackRout, mVecBosEvent->mMinBTrackPt,
   //   par_kSigPed, par_AdcThres, par_maxADC, mMinBClusterEnergy, mMinBClusterEnergyIsoRatio, par_nearTotEtFrac,
   //   mVecBosEvent->mMaxTrackClusterDist, mVecBosEvent->mTrackIsoDeltaR,
   //   par_highET, mVecBosEvent->mTrackIsoDeltaPhi, par_ptBalance, mMinBTrackEta, mMaxBTrackEta
   //) << endm;

   // endcap algo params
   //cout << Form("\n" \
   //   "Endcap W-algo params: trigID: L2EW=%d isMC=%d\n" \
   //   "TPC: nPileupVert>%d, vertex |Z|<%.1fcm, primEleTrack: nFit>%d, hitFrac>%.2f Rin<%.1fcm, Rout>%.1fcm, PT>%.1fGeV/c\n" \
   //   "ETOW ADC: kSigPed=%d AdcThr>%d maxAdc>%.0f clustET>%.1f GeV  ET2x1/ET4x4>%0.2f  ET2x1/mP3InNearCone.Pt()>%0.2f\n" \
   //   "dist(track-clust)<%.1fcm, nearDelR<%.1f\n" \
   //   "W selection highET>%.1f awayDelPhi<%.1frad  ptBalance>%.1fGeV ",
   //   parE_l2ewTrgID, isMC,
   //   mMinNumPileupVertices, mCutVertexZ,
   //   parE_nFitPts, parE_nHitFrac, parE_trackRin, parE_trackRout, mMinETrackPt,
   //   par_kSigPed, par_AdcThres, par_maxADC, parE_clustET, mMinEClusterEnergyIsoRatio, parE_nearTotEtFrac,
   //   parE_delR3D, mVecBosEvent->mTrackIsoDeltaR,
   //   par_highET, mVecBosEvent->mTrackIsoDeltaPhi, parE_ptBalance
   //) << endl;

   cout << Form("\n EtowScaleFact=%.2f  BtowScaleFacor=%.2f" , mParETOWScale, mParBTOWScale) << endl;

   if (mStMuDstMaker) {

      // initialization of TPC cuts is run dependent (only MuDst analysis)
      for (int iSector = 1; iSector <= mxTpcSec; iSector++)
      {
         float Rin   = par_trackRin;
         float Rout  = par_trackRout;
         float RinE  = parE_trackRin;
         float RoutE = parE_trackRout;

         // Rin changes

         //Run 9 (final)
         if (iSector == 4  && mRunNo >= 10090089) Rin = 125.;
         if (iSector == 11 && mRunNo >= 10083013) Rin = 125.;
         if (iSector == 15 && mRunNo >= 10088096 && mRunNo <= 10090112 ) Rin = 125.;

         //Run 11 ?? XXX:ds:

         //Run 12 (not final, need to identify where electronics died, JS)
         if ((iSector == 5 || iSector == 6 || iSector == 7 || iSector == 21) && (mRunNo >= 13000000 || mRunNo / 100 == 3) )
            Rin = 125.; // all have dead inner padrows, run#300+nn is for M-C generated by Jan

         // Rout changes

         //Run 9 (final)
         if (iSector == 5 && mRunNo >= 10098029) Rout = 140.;
         if (iSector == 6 ) Rout = 140.;
         if (iSector == 20 && mRunNo >= 10095120 && mRunNo <= 10099078 ) Rout = 140.;

         //Run 11 ?? XXX

         //Run 12 ??
         mTpcFilter[iSector-1].setCuts(par_nFitPts, mVecBosEvent->mMinTrackHitFrac, Rin, Rout);
         mTpcFilter[iSector-1].init("iSector", iSector, HListTpc, true);

         mTpcFilterE[iSector-1].setCuts(parE_nFitPts, parE_nHitFrac, RinE, RoutE);
         mTpcFilterE[iSector-1].init("secEemcTr", iSector, HListTpc, false);
      }
   }

   // spinDB monitoring
   if (mStMuDstMaker && spinDb) {
      char txt[1000], txt0[100];
      sprintf(txt0, "bxIdeal%d", nRun);
      sprintf(txt,  "intended fill pattern  R%d-%d vs. bXing; %s", runNo, nRun, spinDb->getV124comment());
      nRun++;

      Tfirst = int(2e9);
      Tlast  = -Tfirst;

      hbxIdeal = new TH1F(txt0, txt, 128, -0.5, 127.5);
      hbxIdeal->SetFillColor(kYellow);
      HList->Add(hbxIdeal);

      spinDb->print(0); // 0=short, 1=huge

      for (int bx = 0; bx < 120; bx++) {
         if (spinDb->isBXfilledUsingInternalBX(bx))
            hbxIdeal->Fill(bx);
      }
   }

   return kStOK;
}


Int_t StVecBosMaker::Finish()
{
   if (mStMuDstMaker) {
      LOG_INFO << endl << "Output tree file " << mTreeName << endl << endl;
      mTreeFile->Write();
      mTreeFile->Close();

      if (hbxIdeal) {
         char txt[1000];
         sprintf(txt, "events T= %d %d", Tfirst, Tlast);
         printf("Finish run=%d , events time range %s\n", mRunNo, txt);
         hbxIdeal->GetYaxis()->SetTitle(txt);
      }
   }

   return StMaker::Finish();
}


Int_t StVecBosMaker::FinishRun(int runNo)
{
   LOG_INFO << Form("::FinishRun(%d)", runNo) << endm;
   return kStOK;
}


void StVecBosMaker::Clear(const Option_t *)
{
   mVecBosEvent->clear();
   //delete mVecBosEvent;
   //mVecBosEvent = 0;
}


//
Int_t StVecBosMaker::Make()
{
   // Create new event and connect it to the tree
   //mVecBosEvent = new VecBosEvent();
   //mWtree->Branch("mVecBosEvent", "VecBosEvent", &mVecBosEvent);

   mNumInputEvents++;
   cout << endl;
   Info("Make()", "Called for event %d", mNumInputEvents);
   //printf("isMC = %d\n", isMC);

   if (isMC) {
      //mVecBosEvent->addMC(); 
      //mVecBosEvent->McAnalysis(); 
      mVecBosEvent->CalcRecoil();   
   }

   // standard MuDst analysis
   if (!mStMuDstMaker || !mStJetReader) return kStOK; // We need both makers for proper analysis

   mVecBosEvent->id      = mStMuDstMaker->muDst()->event()->eventId();
   mVecBosEvent->runNo   = mStMuDstMaker->muDst()->event()->runId();
   mVecBosEvent->time    = mStMuDstMaker->muDst()->event()->eventInfo().time();
   mVecBosEvent->zdcRate = mStMuDstMaker->muDst()->event()->runInfo().zdcCoincidenceRate();

   mVecBosEvent->SetStMuDst(mStMuDstMaker->muDst());

   int T = mVecBosEvent->time;

   Tlast  = (Tlast  < T) ? T : Tlast;
   Tfirst = (Tfirst > T) ? T : Tfirst;

   const char *afile = mStMuDstMaker->GetFile();

   if (mNumInputEvents % 200 == 0)
      Info("Make()", "nEve: inp=%d, trig=%d, accpt=%d, daqFile: %s\n", mNumInputEvents, mNumTrigEvents, mNumAcceptedEvents, afile);

   //hA[0]->Fill("inp", 1.);
   //hE[0]->Fill("inp", 1.);

   // First access calorimeter data
   int btowStat = ReadMuDstBTOW(); // get energy in BTOW
   int etowStat = ReadMuDstETOW(); // get energy in ETOW

   // Skip entire event if no energy in BTOW && ETOW
   if ( btowStat != 0 && etowStat != 0 ) {
      Info("Make()", "No energy in neither BTOW nor ETOW. Skipping event...");
      return kStOK;
   }

   int btrig    = ReadMuDstBarrelTrig();
   int etrig    = ReadMuDstEndcapTrig();

   // Skip entire event if no valid trig ID
   if ( btrig != 0 && etrig != 0 ) {
      Info("Make()", "No trigger bit in neither BTOW nor ETOW. Skipping event...");
      //mWtree->Fill();
      return kStOK;
   }

   mNumTrigEvents++;

   // Access other detectors and save info in the event
   ReadMuDstBSMD(); // get energy in BSMD
   ReadMuDstESMD(); // get energy in ESMD
   ReadMuDstEPRS(); // get energy in EPRS

   // Save all vertices from MuDst into event. Add tracks in the event. See the
   // function for the cuts imposed on the track quality
   ReadMuDstVerticesTracks();
   ReadMuDstJets();   // Get input jet info

   mVecBosEvent->Process();
   mVecBosRootFile->Fill(*mVecBosEvent);
   mWtree->Fill(); // write event to tree

   //if (mVecBosEvent->HasGoodVertex())
   //   mVecBosRootFile->Fill(*mVecBosEvent, kCUT_VERTICES_GOOD);

   //XXX:ds: if (mVecBosEvent->GetNumVertices())
   //XXX:ds:    FillTowHit(true);  // fill 2D tower "hit" histos for vertex found and L2BW trigger (beam background analysis, remove any time JS)
   //XXX:ds: else
   //XXX:ds:    FillTowHit(false); // fill 2D tower "hit" histos for _no_ vertex and L2BW trigger (beam background analysis, remove any time JS)

   //XXX:ds: FillNormHists();  // add plots for QCD normalization

   //XXX:ds: // Skip event w/o high Pt tracks
   //XXX:ds: if (mVecBosEvent->GetNumTracks() <= 0) {
   //XXX:ds:    Info("Make()", "No tracks found in the event. Skipping...");
   //XXX:ds:    return kStOK;
   //XXX:ds: }

   //XXX:ds: if (mVecBosEvent->l2bitET  && mVecBosEvent->bemc.tileIn[0] == 1) hA[0]->Fill("B-in", 1.0);
   //XXX:ds: if (mVecBosEvent->l2EbitET && mVecBosEvent->etow.etowIn == 1)    hE[0]->Fill("E-in", 1.0);
   //XXX:ds: if (mVecBosEvent->l2bitET  && !btowStat)                         hA[0]->Fill("B200", 1.0);
   //XXX:ds: if (mVecBosEvent->l2EbitET && !etowStat)                         hE[0]->Fill("E200", 1.0);

   //hA[116]->Fill(mVecBosEvent->mNJets);

   //for (uint iJet=0; iJet<mVecBosEvent->GetNumJets(); ++iJet)
   //{
   //   StJet *jet     = GetJet(iJet);
   //   float  jet_pt  = jet->Pt();
   //   float  jet_eta = jet->Eta();
   //   float  jet_phi = jet->Phi();

   //   hA[117]->Fill(jet_eta, jet_phi);
   //   hA[118]->Fill(jet_pt);
   //}

   // Add tracks to the event and atch tracks to energy clusters in the barrel
   // and endcap
   //bool hasMatchedTrack2BCluster = MatchTrack2BtowCluster();
   //bool hasMatchedTrack2ECluster = MatchTrack2EtowCluster();

   //if (!hasMatchedTrack2BCluster && !hasMatchedTrack2ECluster) return kStOK; //no matched BTOW or ETOW clusters

   mNumAcceptedEvents++;

   // Add info to the event
   CalcPtBalance();
   //CalcMissingET();

   // endcap specific analysis
   //if (hasMatchedTrack2ECluster) {
   //   analyzeESMD();
   //   //analyzeEPRS(); // not implemented
   //}

   // Fill final histograms
   //if (hasMatchedTrack2BCluster) {
   //   FindWBoson();
   //   FindZBoson();
   //}

   //if (hasMatchedTrack2ECluster) FindWBosonEndcap();

   //mVecBosRootFile->Fill(*mVecBosEvent, kCUT_CUT);

   if (mNumAcceptedEvents < 2 || mNumAcceptedEvents % 1000 == 1 ) mVecBosEvent->Print(0x0, isMC);

   return kStOK;
}


void StVecBosMaker::initGeom()
{
   // BTOW
   memset(gMapBTowEtaPhiBin2Id, 0, sizeof(gMapBTowEtaPhiBin2Id));

   // end of loop over towers
   for (int towerId = 1; towerId <= mxBtow; towerId++)
   {
      // querry BTOW geom
      int m, e, s;
      gBTowGeom->getBin(towerId, m, e, s);

      float eta, phi;
      gBTowGeom->getEta(m, e, eta);
      gBTowGeom->getPhi(m, s, phi); // -pi <= phi < pi

      int iEta, iPhi;
      assert(ConvertEtaPhi2Bins(eta, phi, iEta, iPhi)); // tower must be localized at the known position

      int IJ = iEta + iPhi * mxBTetaBin;
      assert(gMapBTowEtaPhiBin2Id[IJ] == 0); // avoid overlaping mapping
      gMapBTowEtaPhiBin2Id[IJ] = towerId;

      float x, y, z;
      assert( gBTowGeom->getXYZ(towerId, x, y, z) == 0);
      gBCalTowerCoords[towerId - 1] = TVector3(x, y, z);
   }

   // BSMD-E, -P
   for (int iep = 0; iep < mxBSmd; iep++) {
      for (int towerId = 1; towerId <= mxBStrips; towerId++) {
         float x, y, z;
         assert( mBSmdGeom[iep]->getXYZ(towerId, x, y, z) == 0);
         mBSmdStripCoords[iep][towerId - 1] = TVector3(x, y, z);
      }
   }

   // ETOW
   for (int isec = 0; isec < mxEtowSec; isec++) {
      for (int isub = 0; isub < mxEtowSub; isub++) {
         for (int ieta = 0; ieta < mxEtowEta; ieta++) {
            gETowCoords[isec * mxEtowSub + isub][ieta] = mGeomEmc->getTowerCenter(isec, isub, ieta);
         }
      }
   }
}


// intended for normalization of filtered QCD MC
void StVecBosMaker::FillNormHists()
{
   // fill max BTOW clustET vs z-vertex distribution for events with positive
   // rank vertex
   if (mVecBosEvent->l2bitET && mVecBosEvent->GetNumVertices() > 0 && mVecBosEvent->mVertices[0].mRank > 0)
   {
      float maxBtowET = 0;

      for (int i = 0; i < mxBtow; i++)
      {
         if (mVecBosEvent->bemc.statTile[0][i] != 0) continue; //zero means good

         int   ieta = -1;
         int   iphi = -1;
         float etaF = gBCalTowerCoords[i].Eta();
         float phiF = gBCalTowerCoords[i].Phi();

         ConvertEtaPhi2Bins(etaF, phiF, ieta, iphi);
         WeveCluster c = mVecBosEvent->FindMaxBTow2x2(ieta, iphi, mVecBosEvent->mVertices[0].z);

         if (c.ET > maxBtowET) maxBtowET = c.ET;
      }

      hA[13]->Fill(mVecBosEvent->mVertices[0].z, maxBtowET);
   }

   // fill max ETOW towerET vs z-vertex distribution for events with positive
   // rank vertex
   if (mVecBosEvent->l2EbitET && mVecBosEvent->GetNumVertices() > 0 && mVecBosEvent->mVertices[0].mRank > 0)
   {
      float maxEtowET = 0;

      for (int isec = 0; isec < mxEtowSec; isec++) {
         for (int isub = 0; isub < mxEtowSub; isub++) {
            for (int ieta = 0; ieta < mxEtowEta; ieta++) {
               if (mVecBosEvent->etow.stat[isec * mxEtowSub + isub][ieta] == 0) {
                  WeveCluster c = sumEtowPatch(ieta, isec * mxEtowSub + isub, 1, 1, mVecBosEvent->mVertices[0].z);

                  if (c.ET > maxEtowET) maxEtowET = c.ET;
               }
            }
         }
      }

      hE[13]->Fill(mVecBosEvent->mVertices[0].z, maxEtowET);
   }
}


/** */
void StVecBosMaker::ReadMuDstJets()
{
   //if (mStJetReader == 0) {
   //   mVecBosEvent->mNJets = 0;
   //   return 0;
   //}

   StJets* stJets         = GetStJets(mJetTreeBranchName);
   StJets* stJetsNoEndcap = GetStJets(mJetTreeBranchNameNoEndcap);

   mVecBosEvent->AddStJets(stJets, stJetsNoEndcap);

   //if (mStJetReader->getStJets(branchName)->eventId() != mVecBosEvent->id)
   if (stJets->eventId() != mVecBosEvent->id)    Error("ReadMuDstJets", "Jet and W event ids do not match: %12d, %12d", stJets->eventId(), mVecBosEvent->id);
   if (stJets->runId()   != mVecBosEvent->runNo) Error("ReadMuDstJets", "Jet and W run ids do not match: %12d, %12d", stJets->runId(), mVecBosEvent->runNo);

   if (stJetsNoEndcap->eventId() != mVecBosEvent->id)    Error("ReadMuDstJets", "Jet and W event ids do not match: %12d, %12d (no_endcap branch)", stJetsNoEndcap->eventId(), mVecBosEvent->id);
   if (stJetsNoEndcap->runId()   != mVecBosEvent->runNo) Error("ReadMuDstJets", "Jet and W run ids do not match: %12d, %12d (no_endcap branch)", stJetsNoEndcap->runId(), mVecBosEvent->runNo);

   //mVecBosEvent->mNJets = stJets->nJets();
}


//StJets* StVecBosMaker::GetStJets(int i) const
//{
//  TBranch* branch = (TBranch*) mStJetReader->tree()->GetListOfBranches()->At(i);
//  return branch ? *(StJets**)branch->GetAddress() : 0;
//}


StJets* StVecBosMaker::GetStJets(const char* bname) const
{
  TBranch* branch = mStJetReader->tree()->GetBranch(bname);
  return branch ? *(StJets**) branch->GetAddress() : 0;
}


// Below is only used for Tree analysis
TClonesArray* StVecBosMaker::GetJetsTreeAnalysis(TString branchName)
{
   //if (mJetTreeChain == 0) {
   //   mVecBosEvent->GetNumJets() = 0;
   //   return 0;
   //}

   //cout<<"looking for matching jet event"<<endl;

   StJets *jetTmp = GetStJetsFromTree(branchName);

   while (jetTmp->eventId() != mVecBosEvent->id || jetTmp->runId() != mVecBosEvent->runNo) {
      mJetTreeChain->GetEntry(indexJet++);
      jetTmp = GetStJetsFromTree(branchName);
   }

   //cout<<"found matching jet event"<<endl;

   assert(jetTmp->eventId() == mVecBosEvent->id);
   assert(jetTmp->runId() == mVecBosEvent->runNo);
   //mVecBosEvent->mNJets = jetTmp->nJets();
   return jetTmp->jets();
}


StJets* StVecBosMaker::GetStJetsFromTree(TString branchName)
{
   TBranch *branch = mJetTreeChain->GetBranch(branchName);
   return branch ? *(StJets **)branch->GetAddress() : 0;
}


Int_t StVecBosMaker::getEvent(Int_t i, Int_t ijet)
{
   Int_t stat    = mTreeChain->GetEntry(i);
   Int_t statJet = mJetTreeChain->GetEntry(ijet);

   if (!stat && !statJet) return kStEOF;

   return kStOK;
}


void StVecBosMaker::chainFile( const Char_t *file )
{
   TString fname = file;
   cout << "Chain W tree files" << endl;

   if ( !fname.Contains("tree.root") ) return;

   cout << "+ " << fname << endl;
   mTreeChain->Add(fname);
}


void StVecBosMaker::chainJetFile( const Char_t *file )
{

   TString fname = file;
   cout << "Chain jet tree files" << endl;

   if ( !fname.Contains("jets_") ) return;

   cout << "+ " << fname << endl;
   mJetTreeChain->Add(fname);
}


// return non-zero on abort
int StVecBosMaker::ReadMuDstEndcapTrig()
{
   if (isMC) {
      if (mVecBosEvent->etow.maxAdc < 10. / 60.*4096)
         return -1; //L2 is HT
      hE[0]->Fill("L2ewET", 1.);
      mVecBosEvent->l2EbitET = true;
      return 0;
   }

   StMuEvent *stMuEvent = mStMuDstMaker->muDst()->event();
   StMuTriggerIdCollection *tic = &(stMuEvent->triggerIdCollection());

   assert(tic);

   const StTriggerId &l1 = tic->l1();
   vector<unsigned int> idL = l1.triggerIds();

   //printf("nTrig=%d, trigID: ",idL.size());
   for (unsigned int i = 0; i < idL.size(); i++) {
      char txt[100];
      sprintf(txt, "%d", idL[i]);
      hE[1]->Fill(txt, 1.);
   }

   //check trigger ID
   if (!tic->nominal().isTrigger(parE_l2ewTrgID)) return -2;
   hE[0]->Fill("L2ewId", 1.);

   // need to get offset for 2011 run for EEMC
   struct  L2weResult2011 {
      unsigned char  trigger;     // bit0=rnd, bit1=ET>thr
      unsigned char  highestEt;   // cluster Et with 60Gev Max.  bits=Et*256/60
      unsigned short highestRDO;
   };

   TArrayI &l2Array = stMuEvent->L2Result();
   LOG_DEBUG << Form("AccessL2Decision() from regular muDst: L2Array-size=%d", l2Array.GetSize()) << endm;

   unsigned int* trigL2Chunk = (unsigned int*) l2Array.GetArray();
   const int EEMCW_offset = 35; // valid only for 2011 run

   L2weResult2011 *l2weResult2011 = (L2weResult2011*) &trigL2Chunk[EEMCW_offset];

   mVecBosEvent->l2EbitET  = (l2weResult2011->trigger & 2) > 0; // bit1=ET>thr
   mVecBosEvent->l2EbitRnd = (l2weResult2011->trigger & 1) > 0; // bit0=rnd,

#if 0
   if (l2weResult2011->trigger == 0) return -3;
   printf(" L2-jet online results below:\n");
   for (int k = 0; k < 64; k++)
      if (trigL2Chunk[k]) printf("k=%2d  val=0x%04x\n", k, trigL2Chunk[k]);
   printf("L2WE_Result 4-bytes: trg bitET=%d,  bitRnd=%d, highets:  ET/GeV=%.2f,  RDO=%d  hex=0x%08x\n", mVecBosEvent->l2EbitET, mVecBosEvent->l2EbitRnd, l2weResult2011->highestEt / 256.*60, l2weResult2011->highestRDO, trigL2Chunk[EEMCW_offset]);
#endif

   // hack to make the code work also for run 9 and early run 12
   // XXX:ds: What about run 11?
   if (mRunNo < 11000111 || mRunNo > 13000000) {
      mVecBosEvent->l2EbitET  = 1;
      mVecBosEvent->l2EbitRnd = 1;
   }

   if ( (mVecBosEvent->l2EbitRnd || mVecBosEvent->l2EbitET) == 0) return -3; // L2W-algo did not accept this event

   hE[0]->Fill("L2ewBits", 1.); // confirmation bits were set properly

   if (mVecBosEvent->l2EbitRnd) {
      hE[0]->Fill("L2ewRnd", 1.);
      for (int m = 0; m < 90; m++) {
         int val = stMuEvent->emcTriggerDetector().highTowerEndcap(m);
         hE[7]->Fill(val);
      }

      hE[61]->Fill(mVecBosEvent->bx7);
   }

   if (!mVecBosEvent->l2EbitET) return -3; // drop L2W-random accepts
   if ( mVecBosEvent->l2EbitET) hE[0]->Fill("L2ewET", 1.);

   // only monitor below
   hE[2]->Fill(mVecBosEvent->bx48);
   hE[3]->Fill(mVecBosEvent->bx7);

   // access L0-HT data
   int mxVal = -1;
   for (int m = 0; m < 90; m++)  {
      int val = stMuEvent->emcTriggerDetector().highTowerEndcap(m);
      if (mxVal < val) mxVal = val;
      if (mVecBosEvent->l2EbitET) hE[6]->Fill(val);
      if (val < parE_DsmThres) continue;
      if (mVecBosEvent->l2EbitET) hE[8]->Fill(m);
      //printf("Fired L0 EHT m=%d val=%d\n",m,val);
   }

   mVecBosEvent->etow.maxHtDsm = mxVal;
   return 0;
}


int StVecBosMaker::ReadMuDstETOW()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;
      return -4;
   }

   mVecBosEvent->etow.etowIn = 1; //tag usable ETOW data
   const char *maxIdName = 0;
   double maxADC =  0, adcSum =  0;
   int    maxSec = -1, maxSub = -1, maxEta = -1;

   //loop over all towers
   for (int i = 0; i < emc->getNEndcapTowerADC(); i++)
   {
      int sec, eta, sub, rawAdc; //muDst  ranges:sec:1-12, sub:1-5, eta:1-12
      emc->getEndcapTowerADC(i, rawAdc, sec, sub, eta);

      const EEmcDbItem *x = mDbE->getTile(sec, 'A' + sub - 1, eta, 'T');
      assert(x); // it should never happened for muDst
      if (x->fail ) continue; // drop not working channels
      int isec = x->sec - 1;
      int isub = x->sub - 'A';
      int ieta = x->eta - 1;

      assert(isec >= 0 && isec < mxEtowSec); // check input is ok
      assert(isub >= 0 && isub < mxEtowSub);
      assert(ieta >= 0 && ieta < mxEtowEta);

      float adc = rawAdc - x->ped; // ped subtracted ADC
      if (adc < par_kSigPed * x->sigPed) continue;

      mVecBosEvent->etow.adc[isec * mxEtowSub + isub][ieta] = adc;

      if (x->gain <= 0) continue; // drop channels w/o gains
      float ene = adc / x->gain;

      //method for shifting energy scale
      ene *= mParETOWScale; //(default is mParETOWScale=1)
      mVecBosEvent->etow.ene[isec * mxEtowSub + isub][ieta] = ene;
      mVecBosEvent->etow.stat[isec * mxEtowSub + isub][ieta] = 0;

      if (maxADC < adc) { maxIdName = x->name; maxADC = adc; maxSec = isec; maxSub = isub; maxEta = ieta;}
      adcSum += adc;

   }

   mVecBosEvent->etow.maxAdc = maxADC;
   mVecBosEvent->etow.maxSec = maxSec;
   mVecBosEvent->etow.maxSub = maxSub;
   mVecBosEvent->etow.maxEta = maxEta;

   hE[31]->Fill(maxADC);
   hE[32]->Fill(adcSum);

   if (maxADC < par_maxADC) return -2 ; // not enough energy

   return 0;
}


void StVecBosMaker::ReadMuDstESMD()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;
   }

   for (char uv = 'U'; uv <= 'V'; uv++)
   {
      int sec, strip;
      int nh = emc->getNEndcapSmdHits(uv);

      for (int i = 0; i < nh; i++) {
         StMuEmcHit *hit = emc->getEndcapSmdHit(uv, i, sec, strip);
         float rawAdc = hit->getAdc();
         const EEmcDbItem *eEmcDbItem = mDbE->getByStrip(sec, uv, strip);
         assert(eEmcDbItem); // it should never happened for muDst

         if (eEmcDbItem->fail )   continue; // drop broken channels
         if (eEmcDbItem->ped < 0) continue; // drop channels without peds

         float adc    = rawAdc - eEmcDbItem->ped; // ped subtracted ADC
         float sigPed = eEmcDbItem->sigPed;

         int isec     = sec - 1;
         int iuv      = eEmcDbItem->plane - 'U';
         int istr     = eEmcDbItem->strip - 1;

         //eEmcDbItem->print(); printf("adc=%f\n",adc);

         assert(isec >= 0 && isec < mxEtowSec); //never trust the input
         assert(iuv  >= 0 && iuv  < mxEsmdPlane);
         assert(istr >= 0 && istr < mxEsmdStrip);

         if (eEmcDbItem->gain <= 0) continue; // drop channels w/o gains
         if (adc < par_kSigPed * sigPed) continue; //drop noise

         mVecBosEvent->esmd.adc[isec][iuv][istr] = adc;
         mVecBosEvent->esmd.ene[isec][iuv][istr] = adc / eEmcDbItem->gain;
      }
   }
}


void StVecBosMaker::ReadMuDstEPRS()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;
   }

   int pNh = emc->getNEndcapPrsHits();

   for (int i = 0; i < pNh; i++) {
      int pre, sec, eta, sub; //muDst  ranges: sec:1-12, sub:1-5, eta:1-12 ,pre:1-3==>pre1/pre2/post

      StMuEmcHit *hit = emc->getEndcapPrsHit(i, sec, sub, eta, pre);
      float rawAdc = hit->getAdc();
      //Db ranges: sec=1-12,sub=A-E,eta=1-12,type=T,P-R ; slow method

      const EEmcDbItem *eEmcDbItem = mDbE->getTile(sec, sub - 1 + 'A', eta, pre - 1 + 'P');
      assert(eEmcDbItem); // it should never happened for muDst
      if (eEmcDbItem->fail ) continue; // drop not working channels

      int isec = eEmcDbItem->sec - 1;
      int isub = eEmcDbItem->sub - 'A';
      int ieta = eEmcDbItem->eta - 1;
      int ipre = pre - 1;
      int iphi = isec * mxEtowSub + isub;

      assert(isec >= 0 && isec < mxEtowSec); // check input is ok
      assert(isub >= 0 && isub < mxEtowSub);
      assert(ieta >= 0 && ieta < mxEtowEta);

      float adc = rawAdc - eEmcDbItem->ped; // ped subtracted ADC
      if (adc < par_kSigPed * eEmcDbItem->sigPed) continue;

      mVecBosEvent->eprs.adc[iphi][ieta][ipre] = adc;

      if (eEmcDbItem->gain <= 0) continue; // drop channels w/o gains

      mVecBosEvent->eprs.ene[isec * mxEtowSub + isub][ieta][ipre] = adc / eEmcDbItem->gain;
      mVecBosEvent->eprs.stat[isec * mxEtowSub + isub][ieta][ipre] = 0;
   }
}


// Returns non-zero on abort
int StVecBosMaker::ReadMuDstBarrelTrig()
{
   if (isMC) {
      // When the trigger emulator is ready, this should hook into that
      // instead of the two functions used below.  For now, check that it passes both
      // L0 and L2, and set the l2bitET flag to true if so.

      //if (!passes_L0()) return -1;
      if (!passes_L2()) return -2;

      hA[0]->Fill("L2bwET", 1.);

      mVecBosEvent->l2bitET = true;
      return 0; // we haven't set everything, but it should be good enough for simu.
   }

   StMuEvent *stMuEvent = mStMuDstMaker->muDst()->event();

   //collect info for the luminosity monitor
   int highestT = 0;
   int highestM = 0;

   // Loop over trigger patches
   for (int m=0; m<300; m++) {
      int myT = stMuEvent->emcTriggerDetector().highTower(m);

      if  (myT > highestT) {
         highestT = myT;
         highestM = m;
      }
   }

   int highestPhi, tempPhi, tempEta;
   int awaySum[16];
   int totalSum = 0;

   memset(awaySum, 0, sizeof(awaySum));

   PatchToEtaPhi(highestM, &tempEta, &highestPhi);

   for (int m = 0; m < 300; m++)
   {
      int myT = stMuEvent->emcTriggerDetector().highTower(m);
      PatchToEtaPhi(m, &tempEta, &tempPhi);

      for (int away_width = 0; away_width < 16; away_width++) {
         if ((highestPhi + 30 - tempPhi) % 30 > (15 - away_width) && (highestPhi + 30 - tempPhi) % 30 < (15 + away_width)) {
            //printf("==> adding %d to awaySum",myT);
            awaySum[away_width] += myT;
         }
      }
      totalSum += myT;
   }

   for (int i = 0; i < 16; i++)
      mVecBosEvent->trigAwaySum[i] = awaySum[i];

   mVecBosEvent->trigTotalSum = totalSum;

   StMuTriggerIdCollection* tic = &(stMuEvent->triggerIdCollection());
   assert(tic);
   const StTriggerId &l1 = tic->l1();
   vector<unsigned int> idL = l1.triggerIds();

   //printf("nTrig=%d, trigID: ",idL.size());
   for (unsigned int i = 0; i < idL.size(); i++) {
      char txt[100];
      sprintf(txt, "%d", idL[i]);
      //printf("%d, ",idL[i]);
      hA[1]->Fill(txt, 1.);
   }

   //printf("\n isTrg=%d trgId=%d\n",tic->nominal().isTrigger(par_l2bwTrgID),par_l2bwTrgID);

   //get bX info
   StL0Trigger *trig = &stMuEvent->l0Trigger();

   mVecBosEvent->bx48 = trig->bunchCrossingId();
   mVecBosEvent->bx7  = trig->bunchCrossingId7bit(mRunNo);

   // store spin info
   int bxStar48 = -2;
   int bxStar7  = -2;
   int spin4    = -2;

   // all 3 DB records exist
   if (spinDb && spinDb->isValid() && spinDb->isPolDirLong())
   {  // you do not want mix Long & Trans by accident
      bxStar48 = spinDb->BXstarUsingBX48(mVecBosEvent->bx48);
      bxStar7  = spinDb->BXstarUsingBX7(mVecBosEvent->bx7);
      spin4    = spinDb->spin4usingBX48(mVecBosEvent->bx48);
   }

   mVecBosEvent->bxStar48 = bxStar48;
   mVecBosEvent->bxStar7  = bxStar7;
   mVecBosEvent->spin4    = spin4;

   // Check trigger ID exists = fired
   if ( !tic->nominal().isTrigger(par_l2bwTrgID) ) return -2;

   hA[0]->Fill("L2bwId", 1.);

   TArrayI &l2Array = stMuEvent->L2Result();
   LOG_DEBUG << Form("AccessL2Decision() from regular muDst: L2Array-size=%d", l2Array.GetSize()) << endm;

   unsigned int* trigL2Chunk = (unsigned int*) l2Array.GetArray();
   const int BEMCW_offset = 20; // valid only for 2009 & 2011 run

   L2wResult2009* l2wResult2009 = (L2wResult2009*) &trigL2Chunk[BEMCW_offset];

#if 0
   if (l2wResult2009->trigger == 0) return -3;

   printf(" L2-jet online results below:\n");

   for (int k = 0; k < 64; k++)
      if (trigL2Chunk[k]) printf("k=%2d  val=0x%04x\n", k, trigL2Chunk[k]);

   L2wResult2009_print(l2wResult2009);
#endif

   mVecBosEvent->l2bitET  = (l2wResult2009->trigger & 2) > 0; // bit1=ET>thr
   mVecBosEvent->l2bitRnd = (l2wResult2009->trigger & 1) > 0; // bit0=rnd

   if ( (mVecBosEvent->l2bitRnd || mVecBosEvent->l2bitET) == 0) return -3; // L2W-algo did not accept this event

   hA[0]->Fill("L2bwBits", 1.); // confirmation bits were set properly

   if (mVecBosEvent->l2bitRnd) {
      hA[0]->Fill("L2bwRnd", 1.);

      for (int m = 0; m < 300; m++) {
         int val = stMuEvent->emcTriggerDetector().highTower(m);
         hA[7]->Fill(val);
      }

      hA[61]->Fill(mVecBosEvent->bx7);
   }

   if (!mVecBosEvent->l2bitET) return -3; // drop L2W-random accepts

   if (mVecBosEvent->l2bitET) hA[0]->Fill("L2bwET", 1.);

   // only monitor below
   hA[2]->Fill(mVecBosEvent->bx48);
   hA[3]->Fill(mVecBosEvent->bx7);

   // access L0-HT data
   int mxVal = -1;

   for (int m = 0; m < 300; m++) {
      int val = stMuEvent->emcTriggerDetector().highTower(m);

      if (mxVal < val) mxVal = val;

      if (mVecBosEvent->l2bitET) hA[6]->Fill(val);

      if (val < par_DsmThres) continue;

      if (mVecBosEvent->l2bitET) hA[8]->Fill(m);

      //printf("Fired L0 HT m=%d val=%d\n",m,val);
   }

   mVecBosEvent->bemc.maxHtDsm = mxVal;
   return 0;
}


/**
 * Saves all vertices. (Before: with (mRank>0 or EEMC matched) && z pos < mCutVertexZ)
 */
void StVecBosMaker::ReadMuDstVerticesTracks()
{
   int numOfPrimaryVertices = mStMuDstMaker->muDst()->numberOfPrimaryVertices();

   // XXX:ds: not sure I understand this cut
   //if (numOfPrimaryVertices < mMinNumPileupVertices) return;

   // separate histos for barrel and endcap triggers
   if (mVecBosEvent->l2bitET)  hA[0]->Fill("tpcOn", 1.);
   if (mVecBosEvent->l2EbitET) hE[0]->Fill("tpcOn", 1.);

   int nVerticesPosRank = 0;

   for (int iVertex=0; iVertex<numOfPrimaryVertices; iVertex++)
   {
      StMuPrimaryVertex *stMuVertex = mStMuDstMaker->muDst()->primaryVertex(iVertex);
      assert(stMuVertex);

      // Select current vertex
      mStMuDstMaker->muDst()->setVertexIndex(iVertex);

      float rank    = stMuVertex->ranking();
      float rankLog = 999;

      if (rank > 1e6)    rankLog = log(rank - 1e6) + 10;
      else if (rank > 0) rankLog = log(rank);
      else               rankLog = log(rank + 1e6) - 10;

      if (mVecBosEvent->l2bitET)  { hA[10]->Fill(rankLog); hA[14]->Fill(rank); }
      if (mVecBosEvent->l2EbitET)   hE[10]->Fill(rankLog);

      // Keep some neg. rank vertices for endcap if matched to ETOW
      // XXX:ds: if (rank <= 0 && stMuVertex->nEEMCMatch() <= 0) continue;

      const StThreeVectorF &vertexPosition = stMuVertex->position();

      // StThreeVectorF &er = stMuVertex->posError();
      if (mVecBosEvent->l2bitET && rank > 0) hA[11]->Fill(vertexPosition.z());
      if (mVecBosEvent->l2EbitET)            hE[11]->Fill(vertexPosition.z());

      // XXX:ds: if (fabs(vertexPosition.z()) > mCutVertexZ) continue;

      if (rank > 0) nVerticesPosRank++; // count vertices with rank>0

      VecBosVertex *vecBosVertex = mVecBosEvent->AddVertex(*stMuVertex);

      ReadMuDstTracks(vecBosVertex);
   }

   if (mVecBosEvent->GetNumVertices() <= 0) return;

   if (mVecBosEvent->l2bitET && nVerticesPosRank > 0) {
      hA[0]->Fill("primVert", 1.);
      hA[4]->Fill(mVecBosEvent->bx48);
      hA[5]->Fill(mVecBosEvent->bx7);
   }

   if (mVecBosEvent->l2EbitET) {
      hE[0]->Fill("primVert", 1.);
      hE[4]->Fill(mVecBosEvent->bx48);
      hE[5]->Fill(mVecBosEvent->bx7);
   }

   // access L0-HT data
   StMuEvent *stMuEvent = mStMuDstMaker->muDst()->event();

   for (int m=0; m<300; m++) {
      int val = stMuEvent->emcTriggerDetector().highTower(m);

      if (val < par_DsmThres) continue;

      if (mVecBosEvent->l2bitET && nVerticesPosRank > 0) hA[9]->Fill(m);
   }

   for (int m=0; m<90; m++) {
      int val = stMuEvent->emcTriggerDetector().highTowerEndcap(m);

      if (val < parE_DsmThres) continue;

      if (mVecBosEvent->l2EbitET) hE[9]->Fill(m);
   }

   if (mVecBosEvent->l2bitET)  hA[12]->Fill(nVerticesPosRank);
   if (mVecBosEvent->l2EbitET) hE[12]->Fill(mVecBosEvent->GetNumVertices());

   if (mVecBosEvent->GetNumVertices() <= 0) return;

   if (mVecBosEvent->l2bitET && nVerticesPosRank > 0) hA[0]->Fill("vertZ", 1.);
   if (mVecBosEvent->l2EbitET)                        hE[0]->Fill("vertZ", 1.);
}


/* from Pibero:
   It looks like your global track is null. See this post:

   http://www.star.bnl.gov/HyperNews-star/get/mudst/53.html

   My reading of this hypernews says its just the way ITTF/MuDst
   works. You can get a good primary track, but its global track
   fails the chi2 fit. So the primary track is kept in the MuDst
   but the global track is dropped. I would suggest you skip those
   rare primary tracks that have no global tracks, that way you
   still use most of the tracks in the MuDst. You don't need to
   skip the entire event, just that track. I guess the down side
   is you couldn't make a global DCA cut on those rare tracks, right?
   I guess you could also request S&C to change ITTF/MuDst not to drop
   the global track for every good primary track regardless of chi2.
*/
/* $STAR/StRoot/StEvent/StTrack.h
 *  mFlag=zxyy, where  z = 1 for pile up track in TPC (otherwise 0)
 *                     x indicates the detectors included in the fit and
 *                    yy indicates the status of the fit.
 *  Positive mFlag values are good fits, negative values are bad fits.
 *
 *  The first digit indicates which detectors were used in the refit:
 *
 *      x=1 -> TPC only
 *      x=3 -> TPC       + primary vertex
 *      x=5 -> SVT + TPC
 *      x=6 -> SVT + TPC + primary vertex
 *      x=7 -> FTPC only
 *      x=8 -> FTPC      + primary
 *      x=9 -> TPC beam background tracks
 *
 *  The last two digits indicate the status of the refit:
 *      = +x01 -> good track
 *
 *      = -x01 -> Bad fit, outlier removal eliminated too many points
 *      = -x02 -> Bad fit, not enough points to fit
 *      = -x03 -> Bad fit, too many fit iterations
 *      = -x04 -> Bad Fit, too many outlier removal iterations
 *      = -x06 -> Bad fit, outlier could not be identified
 *      = -x10 -> Bad fit, not enough points to start
 *
 *      = +x11 -> Short track pointing to EEMC
*/
/**
 * Saves all tracks
 *
 * Before: if
 * (flag > 0) &&
 * (primary track has a global track) &&
 * (flag == 301 || 311) &&
 * track P_T >= 1
 *
 * Tracks which do not pass cuts on individual sectors, that is min number of
 * hits, fract of hits, min and max radius in transverse plane
 *
 */
void StVecBosMaker::ReadMuDstTracks(VecBosVertex* vbVertex)
{
   // printf("\n nInp=%d eveID=%d nPVer=%d nAnyV=
   //        %d\n",mNumInputEvents,mStMuDstMaker->muDst()->event()->eventId(),mVecBosEvent->mVertices.size(),mStMuDstMaker->muDst()->numberOfPrimaryVertices());
   //float rank = vertex->ranking();
   // XXX:ds: assert(rank > 0 || (rank < 0 && vertex->nEEMCMatch()));

   // Get tracks from the current vertex set in ReadMuDstVerticesTracks
   Int_t nPrimaryTracks = mStMuDstMaker->muDst()->GetNPrimaryTrack();

   for (int iTrack=0; iTrack<nPrimaryTracks; iTrack++)
   {
      StMuTrack *primaryTrack = mStMuDstMaker->muDst()->primaryTracks(iTrack);

      mVecBosEvent->AddTrack(primaryTrack, vbVertex);

      //XXX:ds:if (primaryTrack->flag() <= 0) continue;

      const StMuTrack *globalTrack = primaryTrack->globalTrack();

      if (globalTrack == 0) continue; // see the reason at the end of this method

      // Keep list of all tracks for TPC cone sum in tree ana
      //mVecBosEvent->mVertices[iv].prTrList.push_back(primaryTrack);

      StThreeVectorF ro = globalTrack->lastPoint();

      // TPC+prim vertex tracks and short EEMC tracks
      //XXX:ds:if (primaryTrack->flag() != 301 && primaryTrack->flag() != 311) continue;

      if (mVecBosEvent->l2bitET  && vbVertex->mRank > 0 && primaryTrack->flag() == 301) hA[20]->Fill("flag", 1.);
      if (mVecBosEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)             hE[20]->Fill("flag", 1.);

      float pt = primaryTrack->pt();

      //XXX:ds:if (pt < 1.0) continue;

      if (mVecBosEvent->l2bitET  && vbVertex->mRank > 0 && primaryTrack->flag() == 301) hA[20]->Fill("pt1", 1.);
      if (mVecBosEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)             hE[20]->Fill("pt1", 1.);

      // Accepted tracks
      float hitFrac     = 1.*primaryTrack->nHitsFit() / primaryTrack->nHitsPoss();
      // Victor: in reality mChiSqXY is a normal Xi2 for track and mChiSqZ is Xi2 of fit to  primary vertex
      float globChi2dof = globalTrack->chi2();
      float dedx        = primaryTrack->dEdx() * 1e6;

      StThreeVectorF ri = globalTrack->firstPoint();

      // barrel algo track monitors
      if (mVecBosEvent->l2bitET && vbVertex->mRank > 0 && primaryTrack->flag() == 301)
      {
         hA[21]->Fill(primaryTrack->nHitsFit());
         hA[22]->Fill(hitFrac);
         hA[23]->Fill(ri.perp());
         hA[24]->Fill(ro.perp());

         //TPC sector dependent filter
         //XXX:ds: int secID = WtpcFilter::getTpcSec(ro.phi(), ro.pseudoRapidity());
         //XXX:ds: if (secID == 20) continue; //poorly calibrated sector for Run 9+11+12?
         //XXX:ds: if (mTpcFilter[secID - 1].accept(primaryTrack) == false) continue;

         hA[25]->Fill(globalTrack->p().perp());

         if (globalTrack->charge() < 0) hA[27]->Fill(globalTrack->p().perp());

         hA[29]->Fill(pt);

         if (primaryTrack->charge() < 0) hA[30]->Fill(pt);

         hA[26]->Fill(ro.pseudoRapidity(), ro.phi());

         if (pt > 5) hA[57]->Fill(ro.pseudoRapidity(), ro.phi()); // estimate TPC inefficiency in data

         hA[35]->Fill(globChi2dof);

         // monitor chi2 for east/west TPC separately
         if (ri.z() > 0 && ro.z() > 0)  hA[58]->Fill(globChi2dof);
         if (ri.z() < 0 && ro.z() < 0)  hA[59]->Fill(globChi2dof);

         hA[36]->Fill(globChi2dof, ro.pseudoRapidity());
         hA[28]->Fill(primaryTrack->p().mag(), dedx);

         if (pt > 10) hA[197]->Fill(ro.pseudoRapidity(), ro.phi());

         hA[198]->Fill(ro.pseudoRapidity(), primaryTrack->pt());
      }

      // endcap algo track monitors
      if (mVecBosEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)
      {
         hE[20]->Fill("#eta>0.7", 1.);
         hE[21]->Fill(primaryTrack->nHitsFit());
         hE[22]->Fill(hitFrac);
         hE[23]->Fill(ri.perp());
         hE[24]->Fill(ro.perp());

         // TPC sector dependent filter
         //XXX:ds:int secID = WtpcFilter::getTpcSec(ro.phi(), ro.pseudoRapidity());
         //XXX:ds:if ( mTpcFilterE[secID - 1].accept(primaryTrack) == false) continue;

         hE[25]->Fill(globalTrack->p().perp());

         if (globalTrack->charge() < 0) hE[27]->Fill(globalTrack->p().perp());

         hE[29]->Fill(pt);

         if (primaryTrack->charge() < 0) hE[30]->Fill(pt);

         hE[26]->Fill(ro.pseudoRapidity(), ro.phi());

         if (pt > 5) hE[57]->Fill(ro.pseudoRapidity(), ro.phi()); //estimate TPC inefficiency in data

         hE[35]->Fill(globChi2dof);
         hE[36]->Fill(globChi2dof, ro.pseudoRapidity());
         hE[28]->Fill(primaryTrack->p().mag(), dedx);
      }

      bool barrelTrack = (mVecBosEvent->l2bitET && vbVertex->mRank > 0 && primaryTrack->flag() == 301 && pt > mVecBosEvent->mMinBTrackPt);

      if (barrelTrack) hA[20]->Fill("ptOK", 1.); //good barrel candidate

      bool endcapTrack = (mVecBosEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin && pt > mMinETrackPt);

      if (endcapTrack) hE[20]->Fill("ptOK", 1.); //good endcap candidate

      //XXX:ds:if (!barrelTrack && !endcapTrack) continue;
   }

   if (mVecBosEvent->GetNumTracks() <= 0) return;

   if (mVecBosEvent->l2bitET)  hA[0]->Fill("Pt10", 1.);
   if (mVecBosEvent->l2EbitET) hE[0]->Fill("Pt10", 1.);
}


int StVecBosMaker::ReadMuDstBTOW()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      gMessMgr->Warning() << "No EMC data for this event" << endm;
      return -4;
   }

   int ibp = kBTow; // my index for tower & preshower set to BTOW
   int n5  = 0, n0 = 0, n1 = 0, n2 = 0, n3 = 0, n4 = 0;
   int maxID = 0;
   double maxADC = 0, adcSum = 0;

   for (int softID=1; softID<=mxBtow; softID++)
   {
      float rawAdc = emc->getTowerADC(softID);

      if (rawAdc == 0) n0++;

      int statPed, statOfl, statGain;

      mBarrelTables->getStatus(BTOW, softID, statPed, "pedestal"); // official BTOW detector ID
      mBarrelTables->getStatus(BTOW, softID, statOfl);
      mBarrelTables->getStatus(BTOW, softID, statGain, "calib");

      if (statPed  != 1) { mVecBosEvent->bemc.statTile[ibp][softID - 1] = 1; n1++; continue; }
      if (statOfl  != 1) { mVecBosEvent->bemc.statTile[ibp][softID - 1] = 2; n2++; continue; }
      if (statGain != 1) { mVecBosEvent->bemc.statTile[ibp][softID - 1] = 4; n3++; continue; }

      mVecBosEvent->bemc.statTile[ibp][softID - 1] = 0 ;

      float ped, sigPed, gain;
      int capID = 0; // just one value for btow

      mBarrelTables->getPedestal(BTOW, softID, capID, ped, sigPed);
      mBarrelTables->getCalib(BTOW, softID, 1, gain);

      //if (use_gains_file == 1) {
      //   gain = gains_BTOW[softID];
      //}

      //printf("id=%d gain=%f\n",softID,gain);

      // method for shifting energy scale
      gain = gain * mParBTOWScale; //(default is mParBTOWScale=1)

      float adc = rawAdc - ped;

      if (adc > 0) n4++;
      if (adc < par_kSigPed * sigPed) continue;
      if (adc < par_AdcThres)         continue;

      n5++;
      mVecBosEvent->bemc.adcTile[ibp][softID - 1] = adc;
      mVecBosEvent->bemc.eneTile[ibp][softID - 1] = adc * gain;

      if (maxADC < adc) { maxID = softID; maxADC = adc;}

      adcSum += adc;
   }

   //printf("NNN %d %d %d %d %d %d id=%d\n",n0,n1,n2,n3,n4,n5,maxID);
   if (n0 == mxBtow) return -1 ; // BTOW was not present in this events

   mVecBosEvent->bemc.tileIn[ibp] = 1; //tag usable data

   if (mNumInputEvents % 5000 == 1) {
      LOG_INFO << Form("unpackMuBTOW() dataIn=%d, nBbad: ped=%d stat=%d gain=%d ; nAdc: %d>0, %d>thres\n    maxADC=%.0f softID=%d adcSum=%.0f",
                       mVecBosEvent->bemc.tileIn[ibp], n1, n2, n3, n4, n5,
                       maxADC, maxID, adcSum
                      ) << endm;
   }

   hA[31]->Fill(maxADC);
   hA[32]->Fill(adcSum);

   mVecBosEvent->bemc.maxAdc = maxADC;

   if (maxID <= 2400) hA[195]->Fill(maxADC);
   else               hA[196]->Fill(maxADC);

   if (maxADC < par_maxADC) return -2 ; // not enough energy

   return 0;
}


void StVecBosMaker::FillTowHit(bool hasVertices)
{
   if (!mVecBosEvent->l2bitET) return; //only barrel triggers

   //find highest rank vertex
   float maxRank   = 0;
   uint  maxRankId = 0;

   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++)
   {
      float rank = mVecBosEvent->mVertices[iv].mRank;

      if (rank < 0) continue;

      if (rank > maxRank) {
         maxRank   = rank;
         maxRankId = iv;
      }
   }

   int bx7   = mVecBosEvent->bx7;
   int bxBin = -1;

   if (bx7 >= 0 && bx7 < 30) bxBin = 0;
   else if (bx7 < 40)        bxBin = 1;
   else if (bx7 < 110)       bxBin = 2;
   else if (bx7 < 120)       bxBin = 3;

   float Rcylinder  = gBTowGeom->Radius();
   float Rcylinder2 = Rcylinder*Rcylinder;

   //loop barrel towers and fill histo
   for (int i = 0; i < mxBtow; i++)
   {
      float adc     = mVecBosEvent->bemc.adcTile[kBTow][i];
      bool  fillAdc = false;

      if (adc > 10) fillAdc = true; //~150 MeV threshold for tower firing

      if (hasVertices) {
         if (fillAdc) hA[215 + bxBin]->Fill(gBCalTowerCoords[i].Eta(), gBCalTowerCoords[i].Phi());

         float ene  = mVecBosEvent->bemc.eneTile[kBTow][i];
         float delZ = gBCalTowerCoords[i].z() - mVecBosEvent->mVertices[maxRankId].z;
         float e2et = Rcylinder / sqrt(Rcylinder2 + delZ * delZ);
         float ET   = ene * e2et;

         if (ET > 2.0)  hA[219 + bxBin]->Fill(gBCalTowerCoords[i].Eta(), gBCalTowerCoords[i].Phi());
      }
      else if (fillAdc) hA[223 + bxBin]->Fill(gBCalTowerCoords[i].Eta(), gBCalTowerCoords[i].Phi());
   }

   //some lower threshold plots
   for (int isec = 0; isec < mxEtowSec; isec++)
   {
      for (int isub = 0; isub < mxEtowSub; isub++)
      {
         for (int ieta = 0; ieta < mxEtowEta; ieta++)
         {
            int   iPhi    = isec * mxEtowSub + isub;
            float adc     = mVecBosEvent->etow.adc[iPhi][ieta];
            bool  fillAdc = false;

            if (adc > 10) fillAdc = true; //~150 MeV threshold for tower firing

            if (hasVertices) {
               if (fillAdc) hA[227 + bxBin]->Fill(ieta, iPhi);

               float ene  = mVecBosEvent->etow.ene[iPhi][ieta];
               float delZ = gETowCoords[iPhi][ieta].z() - mVecBosEvent->mVertices[maxRankId].z;
               float Rxy  = gETowCoords[iPhi][ieta].Perp();
               float e2et = Rxy / sqrt(Rxy * Rxy + delZ * delZ);
               float ET   = ene * e2et;

               if (ET > 2.0) hA[231 + bxBin]->Fill(ieta, iPhi);
            }
            else if (fillAdc) hA[235 + bxBin]->Fill(ieta, iPhi);
         }
      }
   }
}


void StVecBosMaker::ReadMuDstBSMD()
{
   const char cPlane[mxBSmd] = {'E', 'P'};

   // Access to muDst
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      gMessMgr->Warning() << "No EMC data for this muDst event" << endm;    return;
   }

   // BSMD
   for (int iEP = bsmde; iEP <= bsmdp; iEP++)
   { // official BSMD plane IDs
      int iep = iEP - 3;
      assert(bsmde == 3); // what a hack
      int nh = emc->getNSmdHits(iEP);
      //printf("muDst BSMD-%c nHit=%d\n",cPlane[iep],nh);
      int n5 = 0, n1 = 0, n2 = 0, n3 = 0, n4 = 0;

      for (int i = 0; i < nh; i++)
      {
         StMuEmcHit *hit = emc->getSmdHit(i, iEP);
         float  adc = hit->getAdc();
         int softID = hit->getId();

         int statPed, statOfl, statGain;
         mBarrelTables->getStatus(iEP, softID, statPed, "pedestal");
         mBarrelTables->getStatus(iEP, softID, statOfl);
         mBarrelTables->getStatus(iEP, softID, statGain, "calib");

         if (statPed != 1) { mVecBosEvent->bemc.statBsmd[iep][softID - 1] = 1; n1++; continue; }
         if (statOfl != 1) { mVecBosEvent->bemc.statBsmd[iep][softID - 1] = 2; n2++; continue; }
         if (statGain < 1 || statGain > 19) { mVecBosEvent->bemc.statBsmd[iep][softID - 1] = 4; n3++; continue; }

         float pedRes, sigPed, gain;
         int capID = 0; // just one value for ped residua in pp500, 2009 run

         mBarrelTables->getPedestal(iEP, softID, capID, pedRes, sigPed);
         mBarrelTables->getCalib(iEP, softID, 1, gain);

         if (isMC) { // overwrite it based on genat DE & private calibration
            float par_bsmdAbsGain = 6e6; // tmp arbitrary absolute calib of bsmd, was 3e6
            float  de = hit->getEnergy();// Geant energy deposit (GeV)
            adc = de * par_bsmdAbsGain;
         }
         else { // correct for pedestal residua
            adc -= pedRes;

            if (adc > 0) n4++;
            if (adc < par_kSigPed * sigPed) continue;
         }

         n5++;
         assert(softID >= 1);      assert(softID <= mxBStrips);
         int id0 = softID - 1;
         mVecBosEvent->bemc.adcBsmd[ iep][id0] = adc;
         hA[70 + 10 * iep]->Fill(adc);

         //if(mNumInputEvents<3 || i <20 )printf("  i=%d, smd%c id=%d, m=%d adc=%.3f pedRes=%.1f, sigP=%.1f stat: O=%d P=%d G=%d  gain=%.2f\n",i,cPlane[iep],softID,1+id0/150,adc,pedRes,sigPed, statOfl,statPed,statGain, gain);
      }

      if (mNumTrigEvents % 5000 == 1) {
         LOG_INFO << Form("unpackMuBSMD-%c() nBbad: ped=%d stat=%d gain=%d ; nAdc: %d>0, %d>thres", cPlane[iep], n1, n2, n3, n4, n5) << endm;
      }
   }
}


/**
    In 2011, L2W fed off the BHT3 L0 trigger, which required a single
    high tower to have an ADC of greater than 30.  This is the default
    threshold, but can be set from the macro if a different value is
    needed.
*/
bool StVecBosMaker::passes_L0()
{
   StMuEvent *stMuEvent = mStMuDstMaker->muDst()->event();

   for (int m = 0; m < 300; m++)
      if (stMuEvent->emcTriggerDetector().highTower(m) > par_l0emulAdcThresh) return true;

   return false;
}


/**
    In 2011, the L2W trigger required a 2x2 patch of barrel towers
    where one tower has more than 5.0GeV and the sum of all four is
    E_T>12.0GeV.  These thresholds are the defaults, but can be set
    from the macro if a different value is needed.
*/
bool StVecBosMaker::passes_L2()
{
   for (int i = 0; i < mxBtow; i++)
   {
      // Zero means good
      if (mVecBosEvent->bemc.statTile[0][i] != 0 || mVecBosEvent->bemc.eneTile[0][i] <= par_l2emulSeedThresh)
         continue;

      int   ieta = -1;
      int   iphi = -1;
      float etaF = gBCalTowerCoords[i].Eta();
      float phiF = gBCalTowerCoords[i].Phi();

      ConvertEtaPhi2Bins(etaF, phiF, ieta, iphi);
      WeveCluster c = mVecBosEvent->FindMaxBTow2x2(ieta, iphi, 0);

      if (c.ET > par_l2emulClusterThresh) return true;
   }

   return false;
}


void StVecBosMaker::FindWBoson()
{
   if (!mVecBosEvent->l2bitET) return;

   //printf("========= FindWBoson() \n");
   int nNoNear = 0, nNoAway = 0, nEta1 = 0, nGoldW = 0, nGoldWp = 0, nGoldWn = 0;

   //remove events tagged as Zs
   if (mVecBosEvent->zTag) return;

   // search for Ws
   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++)
   {
      VecBosVertex &vertex = mVecBosEvent->mVertices[iv];
      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         VecBosTrack &track = vertex.eleTrack[it];
         if (track.mMatchedTower.id <= 0) continue; //skip endcap towers
         if (track.isMatch2Cl == false) continue;

         assert(track.mCluster2x2.nTower > 0); // internal logical error
         assert(track.mP3InNearCone.Pt() > 0);      // internal logical error

         // make cut on lepton eta
         if (track.mP3AtDca.Eta() < mMinBTrackEta || track.mP3AtDca.Eta() > mMaxBTrackEta) continue;

         hA[20]->Fill("eta1", 1.);
         nEta1++;

         //signal plots w/o EEMC in veto
         if (track.mCluster2x2.ET / track.mP3InNearConeNoETow.Pt() > par_nearTotEtFrac) {
            if (track.sPtBalance_noEEMC > par_ptBalance ) { // only signed ptBalance cut
               hA[140]->Fill(track.mCluster2x2.ET);
               hA[240]->Fill(track.prMuTrack->eta(), track.mCluster2x2.ET);

               if (track.prMuTrack->charge() < 0) {
                  hA[184 + 3]->Fill(track.mCluster2x2.ET);
               } else if (track.prMuTrack->charge() > 0) {
                  hA[184 + 4]->Fill(track.mCluster2x2.ET);
               }
            }
         }

         // fill plot for background
         if (track.mCluster2x2.ET > par_highET) {
            if (track.prMuTrack->charge() > 0)      hA[251]->Fill(track.mCluster2x2.ET / track.mP3InNearCone.Pt(), track.sPtBalance);
            else if (track.prMuTrack->charge() < 0) hA[252]->Fill(track.mCluster2x2.ET / track.mP3InNearCone.Pt(), track.sPtBalance);
         }

         if (track.mCluster2x2.ET / track.mP3InNearCone.Pt() < par_nearTotEtFrac) continue; // too large nearET

         hA[20]->Fill("noNear", 1.);
         nNoNear++;
         hA[112]->Fill(track.mCluster2x2.ET);
         hA[50]->Fill(track.awayTpcPT);
         hA[51]->Fill(track.awayBtowET);
         hA[54]->Fill(track.awayTotET);
         hA[52]->Fill(track.mCluster2x2.ET, track.awayTotET);
         hA[53]->Fill(track.mCluster2x2.ET, track.awayEmcET);
         hA[55]->Fill(track.awayEtowET);
         hA[60]->Fill(track.mCluster2x2.ET, track.awayTpcPT);

         hA[132]->Fill(track.mCluster2x2.ET, track.ptBalance.Perp());
         hA[133]->Fill(track.awayTotET, track.ptBalance.Perp());
         hA[134]->Fill(track.mCluster2x2.ET, track.sPtBalance);
         hA[135]->Fill(track.awayTotET, track.sPtBalance);
         hA[209]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.ET);

         for (int i = 0; i <= 20; i++) {
            //  float awayTot_cut = 10.+2.*((float) i);
            for (int j = 0; j <= 20; j++) {
               float pTBal_cut = 5. + ((float) j);
               if (track.sPtBalance < pTBal_cut) {
                  if (track.prMuTrack->charge() < 0) {
                     hA[142 + i]->Fill(track.mCluster2x2.ET, j);
                  }
                  else if (track.prMuTrack->charge() > 0) {
                     hA[163 + i]->Fill(track.mCluster2x2.ET, j);
                  }
               }
            }
         }

         //plots for backg sub yield
         if (track.sPtBalance > par_ptBalance ) {
            hA[136]->Fill(track.mCluster2x2.ET);//signal
            hA[241]->Fill(track.prMuTrack->eta(), track.mCluster2x2.ET);
            hA[62]->Fill(track.mMatchedTower.iEta , track.mCluster2x2.energy);
            if (track.prMuTrack->charge() < 0) {
               hA[184 + 1]->Fill(track.mCluster2x2.ET);
            }
            else if (track.prMuTrack->charge() > 0) {
               hA[184 + 2]->Fill(track.mCluster2x2.ET);
            }
         }
         else {
            hA[137]->Fill(track.mCluster2x2.ET);//background
            if (track.prMuTrack->charge() < 0) {
               hA[184 + 5]->Fill(track.mCluster2x2.ET);
            }
            else if (track.prMuTrack->charge() > 0) {
               hA[184 + 6]->Fill(track.mCluster2x2.ET);
            }
            hA[202]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.prMuTrack->pt());
            hA[204]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.energy / track.prMuTrack->p().mag());
         }

         if (track.sPtBalance > par_ptBalance) {
            Info("FindWBoson", "WWWWWWWWWWWWWWWWWWWWW  Barrel");
            wDisaply->exportEvent( "WB", vertex, track, iv);
            mVecBosEvent->Print();
         }

         // put final W cut here
         if (track.sPtBalance < par_ptBalance)  continue;

         hA[20]->Fill("noAway", 1.0);
         nNoAway++;

         //::::::::::::::::::::::::::::::::::::::::::::::::
         //:::::accepted W events for x-section :::::::::::
         //::::::::::::::::::::::::::::::::::::::::::::::::

         hA[113]->Fill( track.mCluster2x2.ET);//for Joe

         hA[90]->Fill( track.mCluster2x2.ET);
         hA[92]->Fill( track.mCluster2x2.ET, track.glMuTrack->dEdx() * 1e6);
         //hA[93]->Fill( track.mCluster2x2.ET,track.glMuTrack->dca(vertex.id).mag());
         int k = 0;
         if (track.prMuTrack->charge() < 0) k = 1;
         hA[94 + k]->Fill( track.mCluster2x2.ET, track.glMuTrack->dcaD());
         // h95 used above

         // plots to investigate east/west yield diff
         hA[200]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.ET);
         hA[201]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.prMuTrack->pt());
         hA[203]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.energy / track.prMuTrack->p().mag());
         hA[205]->Fill(track.prMuTrack->lastPoint().pseudoRapidity(), track.prMuTrack->lastPoint().phi());

         // Q/pT plot
         hA[100]->Fill(track.mCluster2x2.ET, track.glMuTrack->charge() / track.glMuTrack->pt());
         hA[101]->Fill(track.mCluster2x2.ET, track.prMuTrack->charge() / track.prMuTrack->pt());

         if (track.mCluster2x2.ET < par_highET) continue; // very likely Ws

         hA[91]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.position.Phi());
         hA[96]->Fill(vertex.id);
         hA[97]->Fill(vertex.mRankLog);
         hA[98]->Fill(vertex.z);
         hA[99]->Fill(track.prMuTrack->eta());
         hA[190 + k]->Fill(track.prMuTrack->eta(), track.mCluster2x2.ET);

         hA[20]->Fill("goldW", 1.);
         nGoldW++;

         if (track.prMuTrack->charge() > 0)      nGoldWp++;
         else if (track.prMuTrack->charge() < 0) nGoldWn++;
      } // loop over tracks
   } // loop over vertices

   if (nNoNear > 0) hA[0]->Fill("noNear", 1.);
   if (nNoAway > 0) hA[0]->Fill("noAway", 1.);
   if (nEta1   > 0) hA[0]->Fill("eta1",   1.);
   if (nGoldW  > 0) hA[0]->Fill("goldW",  1.);
   if (nGoldWp > 0) hA[0]->Fill("goldW+", 1.);
   if (nGoldWn > 0) hA[0]->Fill("goldW-", 1.);
}


void StVecBosMaker::FindZBoson()
{
   float minJetPt = 10.;
   float lowMass  = 70.;
   float highMass = 140.;

   // Form invariant mass from lepton candidate and jet
   VecBosTrackVecIter iTrack = mVecBosEvent->mTracks.begin();

   for ( ; iTrack!=mVecBosEvent->mTracks.end(); ++iTrack)
   {
      VecBosTrack &track = *iTrack;
      if ( !track.isMatch2Cl ) continue;

      assert(track.mCluster2x2.nTower > 0); // internal logical error
      assert(track.mP3InNearCone.Pt() > 0); // internal logical error

      if (track.mCluster2x2.ET / track.mP3InNearCone.Pt() < par_nearTotEtFrac) continue; // too large nearET

      //match lepton candidate with jet
      //for (uint iJet = 0; iJet < mVecBosEvent->mNJets; iJet++) //jet loop

      StJetPtrSetIter iJet = mVecBosEvent->mJets.begin();

      for ( ; iJet!=mVecBosEvent->mJets.end(); ++iJet)
      {
         StJet *stJet = *iJet;
         if (stJet->Pt() < minJetPt) continue; // ignore low pt jets

         //electron like cut on jets
         //StJet *jet = GetJet(stJet);
         float maxClusterET = 0.;
         int   totTowers  = stJet->nBtowers + stJet->nEtowers;

         for (int itow = 0; itow < totTowers; itow++) // loop over towers
         {
            // Drop endcap towers
            if (stJet->tower(itow)->detectorId() == kEndcapEmcTowerId) continue;

            int towerId = stJet->tower(itow)->towerId();
            //find highest 2x2 BTOW cluster in jet
            TVector3 pos = gBCalTowerCoords[towerId - 1];

            int iEta, iPhi;
            if ( !ConvertEtaPhi2Bins(pos.Eta(), pos.Phi(), iEta, iPhi) ) continue;

            float clusterET = mVecBosEvent->FindMaxBTow2x2(iEta, iPhi, stJet->zVertex).ET;
            if (clusterET > maxClusterET) maxClusterET = clusterET;
         }

         TVector3 jetVec3(stJet->X(), stJet->Y(), stJet->Z());

         if (jetVec3.DeltaR(track.mP3AtDca) < mVecBosEvent->mTrackIsoDeltaR) continue; // skip jets in candidate phi isolation cone

         // form invariant mass
         float    e1 = track.mCluster2x2.energy;
         TVector3 p1 = track.mP3AtDca;
         p1.SetMag(e1);
         TLorentzVector ele1(p1, e1); //lepton candidate 4-momentum
         TLorentzVector sum = ele1 + *stJet;

         float invM = sqrt(sum * sum);

         if (maxClusterET / stJet->jetPt < 0.5) continue;
         if (invM > lowMass && invM < highMass)
            mVecBosEvent->zTag = true;
      }
   }
}


void StVecBosMaker::CalcPtBalance()
{
   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++)
   {
      VecBosVertex &vertex = mVecBosEvent->mVertices[iv];

      for (uint iTrack = 0; iTrack < vertex.eleTrack.size(); iTrack++)
      {
         VecBosTrack &track = vertex.eleTrack[iTrack];

         if (track.isMatch2Cl == false) continue;

         // Loop over branch with EEMC
         //mJets = GetJets(mJetTreeBranchName);
         //if (mJetTreeChain) mJets = GetJetsTreeAnalysis(mJetTreeBranchName);

         // Add up all jets outside of nearDeltaR cone around the electron track
         // Loop over jets
         for (uint iJet = 0; iJet<mVecBosEvent->GetNumJets(); iJet++)
         {
            StJet *jet = GetJet(iJet);
            TVector3 jetVec; //vector for jet momentum
            jetVec.SetPtEtaPhi(jet->Pt(), jet->Eta(), jet->Phi());

            if (jetVec.DeltaR(track.mP3AtDca) > mVecBosEvent->mTrackIsoDeltaR)
               track.ptBalance += jetVec;
         }

         TVector3 clustPt(track.mP3AtDca.X(), track.mP3AtDca.Y(), 0);
         clustPt.SetMag(track.mCluster2x2.ET);

         // Add electron energy. XXX:ds: Why is the energy transverse only?
         track.ptBalance  += clustPt;
         track.sPtBalance  = track.ptBalance.Perp();

         if (track.ptBalance.Dot(clustPt) < 0)
            track.sPtBalance *= -1.;

         // Loop over branch without EEMC
         //mJets = GetJets(mJetTreeBranchNameNoEndcap);
         //if (mJetTreeChain) mJets = GetJetsTreeAnalysis(mJetTreeBranchNameNoEndcap);

         // loop over jets
         for (uint iJet = 0; iJet < mVecBosEvent->GetNumJetsNoEndcap(); iJet++)
         {
            StJet *jet = GetJet(iJet); // XXX:ds: Need different access method for noendcap jets
            TVector3 jetVec; //vector for jet momentum
            jetVec.SetPtEtaPhi(jet->Pt(), jet->Eta(), jet->Phi());

            if (jetVec.DeltaR(track.mP3AtDca) > mVecBosEvent->mTrackIsoDeltaR)
               track.ptBalance_noEEMC += jetVec;
         }

         track.ptBalance_noEEMC += clustPt;
         track.sPtBalance_noEEMC = track.ptBalance_noEEMC.Perp();

         if (track.ptBalance_noEEMC.Dot(clustPt) < 0)
            track.sPtBalance_noEEMC *= -1.;
      }
   }
}


void StVecBosMaker::CalcMissingET()
{
}


// Deprecated
/*
float StVecBosMaker::SumTpcConeFromTree(int vertID, TVector3 refAxis, int flag, int pointTowId)
{
   // flag=2 use 2D cut, 1= only delta phi

   assert(vertID >= 0);
   assert(vertID < (int)mVecBosEvent->mVertices.size());

   double ptSum = 0;
   VecBosVertex &vertex = mVecBosEvent->mVertices[vertID];
   for (uint it = 0; it < vertex.prTrList.size(); it++) {
      StMuTrack *prTr = vertex.prTrList[it];
      if (prTr->flag() <= 0) continue;
      if (prTr->flag() != 301 && pointTowId > 0) continue; // TPC-only regular tracks for barrel candidate
      if (prTr->flag() != 301 && prTr->flag() != 311 && pointTowId < 0) continue; // TPC regular and short EEMC tracks for endcap candidate
      float hitFrac = 1.*prTr->nHitsFit() / prTr->nHitsPoss();
      if (hitFrac < mVecBosEvent->mMinTrackHitFrac) continue;
      StThreeVectorF prPvect = prTr->p();
      TVector3 mP3AtDca = TVector3(prPvect.x(), prPvect.y(), prPvect.z());
      // printf(" prTrID=%4d  prTrEta=%.3f prTrPhi/deg=%.1f prPT=%.1f  nFitPts=%d\n", prTr->id(),prTr->eta(),prTr->phi()/3.1416*180.,prTr->pt(),prTr->nHitsFit());
      if (flag == 1) {
         float deltaPhi = refAxis.DeltaPhi(mP3AtDca);
         if (fabs(deltaPhi) > mVecBosEvent->mTrackIsoDeltaPhi) continue;
      }
      if (flag == 2) {
         float deltaR = refAxis.DeltaR(mP3AtDca);
         if (deltaR > mVecBosEvent->mTrackIsoDeltaR) continue;

      }
      float pT = prTr->pt();
      //    printf(" passed pt=%.1f\n",pT);

      //separate quench for barrel and endcap candidates
      if (pT > mVecBosEvent->mMinBTrackPt && pointTowId > 0) ptSum += mVecBosEvent->mMinBTrackPt;
      else if (pT > mMinETrackPt && pointTowId < 0) ptSum += mMinETrackPt;
      else  ptSum += pT;
   }
   return ptSum;
}
*/


// return # of extended tracks
int StVecBosMaker::ExtendTrack2Endcap()
{
   //printf("******* extendTracksEndcap() nVert=%d\n",mVecBosEvent.vertex.size());
   if (!mVecBosEvent->l2EbitET) return 0; //fire endcap trigger

   double parE_zSMD = mGeomEmc->getZSMD(); // (cm), smd depth
   int nTrE = 0;

   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++) 
   {
      VecBosVertex &V = mVecBosEvent->mVertices[iv];
      for (uint it = 0; it < V.eleTrack.size(); it++) 
      {
         VecBosTrack &T = V.eleTrack[it];

         if (T.prMuTrack->eta() < parE_trackEtaMin) continue; // to avoid extrapolation nonsense

         // Do eta sorting at track level (tree analysis)
         if (T.mP3AtDca.Eta() < mMinETrackEta || T.mP3AtDca.Eta() > mMaxETrackEta) continue;

         // Extrapolate track to the disk perpendicular to the z-axis
         const StPhysicalHelixD trkHlx       = T.prMuTrack->outerHelix();
         StThreeVectorD         diskPosition = StThreeVectorD(0, 0, parE_zSMD);
         StThreeVectorD         diskNormal   = StThreeVectorD(0, 0, 1);

         //path length at intersection with plane
         double path = trkHlx.pathLength(diskPosition, diskNormal);

         StThreeVectorD r = trkHlx.at(path);
         float periodL = trkHlx.period();

         if (periodL < 2 * path) {
            printf(" Warn, long path fac=%.1f ", path / periodL);
            printf(" punchEEMC1 x,y,z=%.1f, %.1f, %.1f path=%.1f period=%.1f\n", r.x(), r.y(), r.z(), path, periodL);
         }

         //printf("hitR xyz=%f %f %f, detEta=%f\n",r.x(),r.y(),r.z(),eta);
         hE[69]->Fill(r.x(), r.y());

         int isec, isubSec, ietaBin;
         Float_t epsPhi, epsEta;
         TVector3 rCross(r.x(), r.y(), r.z());
         bool inEtow = mGeomEmc->getTower(rCross, isec, isubSec, ietaBin, epsPhi, epsEta);
         if (!inEtow) continue;
         hE[20]->Fill("@E", 1.);
         //printf("trk points EEMC tower isec=%d isub=%d ieta=%d epsPhi=%f epsEta=%f  trkPT=%f\n", isec,isubSec,ietaBin,epsPhi,epsEta,T.prMuTrack->pt());

         nTrE++;
         T.mMatchedTower.id   = -999; //set negative for endcap towers
         T.mMatchedTower.R    = rCross;
         T.mMatchedTower.iEta = ietaBin;
         T.mMatchedTower.iPhi = isec * mxEtowSub + isubSec;

         //find global track extrapolation (for ESMD analysis)
         const StPhysicalHelixD trkHlxGlob = T.glMuTrack->outerHelix();
         double pathGlob = trkHlxGlob.pathLength(diskPosition, diskNormal);

         StThreeVectorD rGlob = trkHlxGlob.at(pathGlob);
         float periodLGlob    = trkHlxGlob.period();

         if (periodLGlob < 2 * pathGlob) {
            printf(" Warn, long path Global fac=%.1f ", pathGlob / periodLGlob);
            printf(" punchEEMC1 x,y,z=%.1f, %.1f, %.1f path=%.1f period=%.1f\n", r.x(), r.y(), r.z(), pathGlob, periodLGlob);
         }
         TVector3 rCrossGlob(rGlob.x(), rGlob.y(), rGlob.z());
         T.mMatchedTower.Rglob = rCrossGlob;
      }
   }

   if (nTrE <= 0) return -1;
   hE[0]->Fill("TrE", 1.0);

   return 0;
}


bool StVecBosMaker::MatchTrack2EtowCluster()
{
   // find endcap candidates
   ExtendTrack2Endcap();

   //printf("******* matchEtowCluster() nVert=%d\n",mVecBosEvent.vertex.size());
   if (!mVecBosEvent->l2EbitET) return false;

   int numMatchedTracks = 0;

   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++)
   {
      VecBosVertex &vertex = mVecBosEvent->mVertices[iv];
      float vertexZ = vertex.z;

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         VecBosTrack &track = vertex.eleTrack[it];
         if (track.mMatchedTower.id >= 0) continue; //skip barrel towers

         float trackPT = track.prMuTrack->momentum().perp();

         //need to decide on 2x2 or 2x1 for cluster size
         track.mCluster2x2 = maxEtow2x2(track.mMatchedTower.iEta, track.mMatchedTower.iPhi, vertexZ);

         hE[33]->Fill(track.mCluster2x2.ET);
         hE[34]->Fill(track.mCluster2x2.adcSum, trackPT);
         hE[110]->Fill(track.mCluster2x2.ET);

         // Compute surrounding mCluster2x2 energy
         int iEta = track.mCluster2x2.iEta;
         int iPhi = track.mCluster2x2.iPhi;
         track.mCluster4x4 = sumEtowPatch(iEta - 1, iPhi - 1, 4, 4, vertexZ);

         if (track.mCluster2x2.ET < parE_clustET) continue; // too low energy

         hE[20]->Fill("CL", 1.);
         hE[37]->Fill(track.mCluster4x4.ET);
         hE[38]->Fill(track.mCluster2x2.energy, track.mCluster4x4.energy - track.mCluster2x2.energy);

         float frac24 = track.mCluster2x2.ET / (track.mCluster4x4.ET);

         hE[39]->Fill(frac24);

         if (frac24 < mMinEClusterEnergyIsoRatio) continue;

         hE[20]->Fill("fr24", 1.);

         // set logE weighted cluster position vector at SMD z depth
         float newMag = mGeomEmc->getZSMD() / TMath::Cos(track.mCluster2x2.position.Theta());
         track.mCluster2x2.position.SetMag(newMag);

         // spacial separation (track - cluster) only use 2D X-Y distance for endcap (ie. D.Perp())
         TVector3 D = track.mMatchedTower.R - track.mCluster2x2.position;
         hE[43]->Fill(track.mCluster2x2.energy, D.Perp());

         float delPhi = track.mMatchedTower.R.DeltaPhi(track.mCluster2x2.position);
         float Rxy    = track.mCluster2x2.position.Perp();

         hE[44]->Fill( track.mCluster2x2.position.Phi(), Rxy * delPhi);
         hE[45]->Fill( track.mCluster2x2.energy, Rxy * delPhi); // wrong?
         hE[46]->Fill( D.Perp());

         if (D.Perp() > mVecBosEvent->mMaxTrackClusterDist) continue;

         track.isMatch2Cl = true; // cluster is matched to TPC track

         hE[20]->Fill("#Delta R", 1.);
         hE[111]->Fill( track.mCluster2x2.ET);

         numMatchedTracks++;
      }
   }

   if (numMatchedTracks <= 0) return false;

   hE[0]->Fill("Tr2Cl", 1.0);

   return true;
}


void StVecBosMaker::FindWBosonEndcap()
{
   if (!mVecBosEvent->l2EbitET) return;

   //printf("========= FindWBosonEndcap() \n");
   int nNoNear = 0, nNoAway = 0, nGoldW = 0;

   // search for  Ws ............
   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++) 
   {
      VecBosVertex &V = mVecBosEvent->mVertices[iv];
      for (uint it = 0; it < V.eleTrack.size(); it++) 
      {
         VecBosTrack &T = V.eleTrack[it];
         if (T.mMatchedTower.id >= 0) continue; //skip barrel towers
         if (T.isMatch2Cl == false) continue;
         assert(T.mCluster2x2.nTower > 0); // internal logical error
         // XXX:ds: assert(T.mP3InNearCone.Pt() > 0); // internal logical error
         if (T.mP3InNearCone.Pt() < 0) continue; // internal logical error

         //signal plots w/o EEMC in veto
         if (T.mCluster2x2.ET / T.mP3InNearConeNoETow.Pt() > parE_nearTotEtFrac) {
            if (T.sPtBalance_noEEMC > parE_ptBalance ) { //only signed ptBalance cut
               hE[140]->Fill(T.mCluster2x2.ET);
               if (T.prMuTrack->charge() < 0) {
                  hE[184 + 3]->Fill(T.mCluster2x2.ET);
               }
               else if (T.prMuTrack->charge() > 0) {
                  hE[184 + 4]->Fill(T.mCluster2x2.ET);
               }
            }
         }

         if (T.mCluster2x2.ET / T.mP3InNearCone.Pt() < parE_nearTotEtFrac) continue; // too large nearET

         hE[20]->Fill("noNear", 1.);
         nNoNear++;
         hE[112]->Fill(T.mCluster2x2.ET); // for Joe
         hE[50]->Fill(T.awayTpcPT);
         hE[51]->Fill(T.awayBtowET);
         hE[54]->Fill(T.awayTotET);
         hE[52]->Fill(T.mCluster2x2.ET, T.awayTotET);
         hE[53]->Fill(T.mCluster2x2.ET, T.awayEmcET);
         hE[55]->Fill(T.awayEtowET);
         hE[60]->Fill(T.mCluster2x2.ET, T.awayTpcPT);

         hE[132]->Fill(T.mCluster2x2.ET, T.ptBalance.Perp());
         hE[133]->Fill(T.awayTotET, T.ptBalance.Perp());
         hE[134]->Fill(T.mCluster2x2.ET, T.sPtBalance);
         hE[135]->Fill(T.awayTotET, T.sPtBalance);

         //plots for backg sub yield
         if (T.sPtBalance > parE_ptBalance ) {
            hE[136]->Fill(T.mCluster2x2.ET);//signal
            hE[62]->Fill(T.mMatchedTower.iEta , T.mCluster2x2.energy);
            if (T.prMuTrack->charge() < 0) {
               hE[184 + 1]->Fill(T.mCluster2x2.ET);
            }
            else if (T.prMuTrack->charge() > 0) {
               hE[184 + 2]->Fill(T.mCluster2x2.ET);
            }
         }
         else {
            hE[137]->Fill(T.mCluster2x2.ET);//background
            if (T.prMuTrack->charge() < 0) {
               hE[184 + 5]->Fill(T.mCluster2x2.ET);
            }
            else if (T.prMuTrack->charge() > 0) {
               hE[184 + 6]->Fill(T.mCluster2x2.ET);
            }
         }

         if (T.sPtBalance > parE_ptBalance) { /***************************/
            printf("\n WWWWWWWWWWWWWWWWWWWWW  Endcap \n");
            wDisaply->exportEvent( "WE", V, T, iv);
            mVecBosEvent->Print();
         }/***************************/

         //put final W cut here
         if (T.sPtBalance < parE_ptBalance)  continue;
         //::::::::::::::::::::::::::::::::::::::::::::::::
         //:::::accepted W events for x-section :::::::::::
         //::::::::::::::::::::::::::::::::::::::::::::::::

         hE[20]->Fill("noAway", 1.0);
         nNoAway++;
         hE[113]->Fill( T.mCluster2x2.ET);//for Joe

         hE[90]->Fill( T.mCluster2x2.ET);
         hE[92]->Fill( T.mCluster2x2.ET, T.glMuTrack->dEdx() * 1e6);
         //hE[93]->Fill( T.mCluster2x2.ET,T.glMuTrack->dca().mag());
         int k = 0; if (T.prMuTrack->charge() < 0) k = 1;
         hE[94 + k]->Fill( T.mCluster2x2.ET, T.glMuTrack->dcaD());
         // h95 used above

         // do charge sign plot
         float ET = T.mCluster2x2.ET;
         const StMuTrack *glTr = T.glMuTrack; assert(glTr);
         const StMuTrack *prTr = T.prMuTrack; assert(prTr);
         float g_chrg = glTr->charge();
         float p_chrg = prTr->charge();
         hE[200]->Fill(ET, g_chrg / glTr->pt());
         hE[201]->Fill(ET, p_chrg / prTr->pt());

         if (T.mCluster2x2.ET < par_highET) continue; // very likely Ws
         hE[91]->Fill(T.mCluster2x2.position.PseudoRapidity(), T.mCluster2x2.position.Phi());
         hE[96]->Fill(V.id);
         hE[97]->Fill(V.mRankLog);
         hE[98]->Fill(V.z);
         hE[99]->Fill( T.prMuTrack->eta());
         hE[100]->Fill(T.mMatchedTower.R.X(), T.mMatchedTower.R.Y());
         hE[190 + k]->Fill(T.prMuTrack->eta(), T.mCluster2x2.ET);
         hE[20]->Fill("goldW", 1.);
         nGoldW++;

      }// loop over tracks
   }// loop over vertices
   if (nNoNear > 0) hE[0]->Fill("noNear", 1.);
   if (nNoAway > 0) hE[0]->Fill("noAway", 1.);
   if (nGoldW > 0)
      hE[0]->Fill("goldW", 1.);

}


void StVecBosMaker::analyzeESMD()
{
   if (!mVecBosEvent->l2EbitET) return;
   //Info("analyzeESMD", "");

   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++)
   {
      VecBosVertex &V = mVecBosEvent->mVertices[iv];
      for (uint it = 0; it < V.eleTrack.size(); it++) 
      {
         VecBosTrack &T = V.eleTrack[it];
         if (T.mMatchedTower.id >= 0) continue; //skip barrel towers
         if (T.isMatch2Cl == false) continue;

         assert(T.mCluster2x2.nTower > 0); // internal logical error

         // Id of strips pointed by prim and glob tracks in each plane
         int hitStrip[2]     = { -1, -1};
         int hitStripGlob[2] = { -1, -1};

         // Initialize shower shape histograms
         TH1F *esmdShowerHist[2];
         esmdShowerHist[0] = new TH1F(Form("esmdU%d", mVecBosEvent->id), "esmdU", 41, -10.25, 10.25);
         esmdShowerHist[1] = new TH1F(Form("esmdV%d", mVecBosEvent->id), "esmdV", 41, -10.25, 10.25);

         // Loop over planes
         for (int iuv = 0; iuv < 2; iuv++)
         {
            Float_t dca; //primary extrapolation to smd plane
            const StructEEmcStrip *stripPtr = mGeomSmd->getDca2Strip(iuv, T.mMatchedTower.R, &dca); // find pointed strip

            if (!stripPtr) {cout << "No Strip found" << endl; continue;}
            if (fabs(dca) > 0.5 /*cm*/) {cout << "DCA to big" << endl; continue;}

            Float_t dcaGlob; //global extrapolation to smd plane
            const StructEEmcStrip *stripPtrGlob = mGeomSmd->getDca2Strip(iuv, T.mMatchedTower.Rglob, &dcaGlob); // find pointed strip

            int maxStripId = -1; float maxE = -1;

            int stripId  = stripPtr->stripStructId.stripId;
            int sectorId = stripPtr->stripStructId.sectorId;

            T.hitSector          = sectorId;
            T.esmdGlobStrip[iuv] = stripPtrGlob->stripStructId.stripId - stripId;
            T.esmdDca[iuv]       = dca;
            T.esmdDcaGlob[iuv]   = dcaGlob;

            hitStrip[iuv]     = stripId;
            hitStripGlob[iuv] = stripPtrGlob->stripStructId.stripId;

            // set integration range for smd energy
            int str1 = stripId - parE_nSmdStrip; if (str1 < 1) str1 = 1;
            int str2 = stripId + parE_nSmdStrip; if (str2 > 288) str2 = 288;

            for (int istrip = str1; istrip <= str2; istrip++)
            {
               float ene = mVecBosEvent->esmd.ene[sectorId - 1][iuv][istrip - 1] * 1e3;
               esmdShowerHist[iuv]->SetBinContent(istrip - stripId + parE_nSmdStrip + 1, ene);
               T.esmdShower[iuv][istrip - stripId + parE_nSmdStrip] = ene;
               if (ene > maxE) { maxStripId = istrip; maxE = ene; }
               if (ene > 0) {
                  T.esmdE[iuv] += ene;
                  T.esmdNhit[iuv]++;
               }
            }

            // fit shower shape and fill shower properties
            TF1 *f = new TF1("f", "gaus", -5., 5.);
            f->SetParameter(1, 0);
            esmdShowerHist[iuv]->Fit(f, "RQ", "RQ", -5., 5.);
            T.esmdShowerCentroid[iuv] = f->GetParameter(1);
            T.esmdShowerWidth[iuv] = f->GetParameter(2);

            //get shower x-point from hitStrip + centroid of fit
            T.esmdXPcentroid = mGeomSmd->getIntersection(T.hitSector - 1, hitStrip[0] - 1 + (int)T.esmdShowerCentroid[0], hitStrip[1] - 1 + (int)T.esmdShowerCentroid[1]);
         }
      }
   }
}


void StVecBosMaker::analyzeEPRS()
{
   if (!mVecBosEvent->l2EbitET) return;
   // Info("analyzeEPRS");

   for (uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++) 
   {
      VecBosVertex &V = mVecBosEvent->mVertices[iv];
      for (uint it = 0; it < V.eleTrack.size(); it++) 
      {
         VecBosTrack &T = V.eleTrack[it];
         if (T.mMatchedTower.id >= 0) continue; // skip barrel towers
         if (T.isMatch2Cl == false) continue;
         assert(T.mCluster2x2.nTower > 0); // internal logical error

         //do some clustering of EPRS deposits and plot histos
      }
   }
}


WeveCluster StVecBosMaker::maxEtow2x1(int iEta, int iPhi, float zVert)
{
   //printf("   maxEtow2x1  seed iEta=%d iPhi=%d \n",iEta, iPhi);

   WeveCluster maxCL;
   // just 4 cases of 2x1 clusters
   float maxET = 0;
   int I0 = iEta - 1;
   int J0 = iPhi - 1;
   for (int I = I0; I <= I0 + 1; I++) { // try along eta dir
      WeveCluster CL = sumEtowPatch(I, iPhi, 2, 1, zVert);
      if (maxET > CL.ET) continue;
      maxET = CL.ET;
      maxCL = CL;
      //printf(" maxEtow2x1 A  newMaxETSum=%.1f iEta=%d iPhi=%d \n",maxET, I,iPhi);
   }

   for (int J = J0; J <= J0 + 1; J++) { // try along phi dir
      WeveCluster CL = sumEtowPatch(iEta, J, 1, 2, zVert);
      if (maxET > CL.ET) continue;
      maxET = CL.ET;
      maxCL = CL;
      //printf(" maxEtow2x1 B  newMaxETSum=%.1f iEta=%d iPhi=%d \n",maxET,iEta,J);
   }
   //printf(" final inpEve=%d SumET2x2=%.1f \n",mNumInputEvents,maxET);
   return maxCL;
}


WeveCluster StVecBosMaker::maxEtow2x2(int iEta, int iPhi, float zVert)
{
   //printf("   maxEtow2x1  seed iEta=%d iPhi=%d \n",iEta, iPhi);
   const int L = 2; // size of the summed square

   WeveCluster maxCL;
   // just 4 cases of 2x1 clusters
   float maxET = 0;
   int I0 = iEta - 1;
   int J0 = iPhi - 1;
   for (int I = I0; I <= I0 + 1; I++) {
      for (int J = J0; J <= J0 + 1; J++) {
         WeveCluster CL = sumEtowPatch(I, J, L, L, zVert);
         if (maxET > CL.ET) continue;
         maxET = CL.ET;
         maxCL = CL;
         //printf(" maxEtow2x2 A  newMaxETSum=%.1f iEta=%d iPhi=%d \n",maxET, I,iPhi);
      }
   }// 4 combinations done

   //printf(" final inpEve=%d SumET2x2=%.1f \n",mNumInputEvents,maxET);
   return maxCL;
}


WeveCluster StVecBosMaker::sumEtowPatch(int iEta, int iPhi, int Leta, int  Lphi, float zVert)
{
   //printf("     eveID=%d etowPatch seed iEta=%d[+%d] iPhi=%d[+%d] zVert=%.0f \n",mVecBosEvent.id,iEta,Leta, iPhi,Lphi,zVert);
   WeveCluster CL; // object is small, not to much overhead in creating it
   CL.iEta = iEta;
   CL.iPhi = iPhi;
   TVector3 R;
   double sumW = 0;

   for (int i = iEta; i < iEta + Leta; i++) { // trim in eta-direction
      if (i < 0) continue;
      if (i >= mxEtowEta) continue;
      for (int j = iPhi; j < iPhi + Lphi; j++) { // wrap up in the phi-direction
         int jj = (j + mxEtowPhiBin) % mxEtowPhiBin; // keep it always positive
         //if(L<5) printf("n=%2d  i=%d jj=%d\n",CL.nTower,i,jj);

         float ene = mVecBosEvent->etow.ene[jj][i];
         if (ene <= 0) continue; // skip towers w/o energy
         float adc   = mVecBosEvent->etow.adc[jj][i];
         float delZ  = gETowCoords[jj][i].z() - zVert;
         float Rxy   = gETowCoords[jj][i].Perp();
         float e2et  = Rxy / sqrt(Rxy * Rxy + delZ * delZ);
         float ET    = ene * e2et;
         float logET = log10(ET + 0.5);
         CL.nTower++;
         CL.energy += ene;
         CL.ET += ET;
         CL.adcSum += adc;
         if (logET > 0) {
            R += logET * gETowCoords[jj][i];
            sumW += logET;
         }
      }
      //printf("      in etowPatch: iEta=%d  nTw=%d, ET=%.1f adcSum=%.1f\n",i,CL.nTower,CL.ET,CL.adcSum);
      if (sumW > 0) {
         CL.position = 1. / sumW * R; // weighted cluster position
      }
      else {
         CL.position = TVector3(0, 0, 999);
      }
   }
   return CL;
}


//________________________________________________
//________________________________________________

void
StVecBosMaker::hadronicRecoil(){ //add up all vector pt outside of 'nearJet' region to get 'hadronic recoil' pt vector 

  for(uint iv = 0; iv < mVecBosEvent->mVertices.size(); iv++) 
  {
    VecBosVertex &vertex = mVecBosEvent->mVertices[iv];
    for(uint it = 0; it < vertex.eleTrack.size(); it++) 
    {
      VecBosTrack &track = vertex.eleTrack[it];
      if(track.isMatch2Cl == false) continue;
      
      TVector3 recoil;
      
      //.... process BTOW hits
      for(int i = 0; i < mxBtow; i++) 
      {
	float ene = mVecBosEvent->bemc.eneTile[kBTow][i];
	if(ene <= 0) continue;

	TVector3 primP = positionBtow[i]-TVector3(0,0,vertex.z);
	primP.SetMag(ene); // it is 3D momentum in the event ref frame

	float deltaR = primP.DeltaR(primP);        
	if(deltaR < mVecBosEvent->mTrackIsoDeltaR) continue;
	recoil += primP;
      }

      //....process ETOW hits
      for(int iphi = 0; iphi < mxEtowPhiBin; iphi++)
      {
	for(int ieta = 0; ieta < mxEtowEta; ieta++)
        {
	  float ene = mVecBosEvent->etow.ene[iphi][ieta];
	  if(ene <= 0) continue; //skip towers with no energy
 
	  TVector3 primP = positionEtow[iphi][ieta] - TVector3(0,0,vertex.z);
	  primP.SetMag(ene); // it is 3D momentum in the event ref frame

	  float deltaR = primP.DeltaR(primP);        
	  if(deltaR < mVecBosEvent->mTrackIsoDeltaR) continue;
	  recoil += primP;
	}
      }    

      //....process TPC tracks
      int vertID = vertex.id;
      assert(vertID >= 0);
      assert(vertID < (int)mStMuDstMaker->muDst()->numberOfPrimaryVertices());
      
      StMuPrimaryVertex* prV = mStMuDstMaker->muDst()->primaryVertex(vertID);
      assert(prV);	  
      mStMuDstMaker->muDst()->setVertexIndex(vertID);

      float rank = prV->ranking();
      assert(rank > 0);

      Int_t nPrimTrAll = mStMuDstMaker->muDst()->GetNPrimaryTrack();

      for(int itr=0;itr<nPrimTrAll;itr++) 
      {
	StMuTrack *prTr = mStMuDstMaker->muDst()->primaryTracks(itr);
	if(prTr->flag() <= 0) continue;
	if(prTr->flag() != 301) continue;// TPC-only regular tracks

	float hitFrac = 1.*prTr->nHitsFit()/prTr->nHitsPoss();
	if(hitFrac < par_nHitFrac) continue;

	StThreeVectorF prPvect = prTr->p();

	TVector3 primP = TVector3(prPvect.x(),prPvect.y(),prPvect.z());

	float deltaR = primP.DeltaR(primP);
	if(deltaR <  mVecBosEvent->mTrackIsoDeltaR) continue;
	if(primP.Perp() < 0.15) continue; //lower threshold on pT < 150 MeV
	recoil += primP;	
      }
      
      track.hadronicRecoil = recoil;

    }
  }
}

