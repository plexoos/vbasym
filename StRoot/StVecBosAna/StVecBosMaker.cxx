#include <limits>
#include <string>

#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TStopwatch.h"
#include "TString.h"

#include "StMessMgr.h"
#include "StThreeVectorF.hh"

#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StTriggerUtilities/L2Emulator/L2wAlgo/L2wResult2009.h"

#include "StEmcUtil/database/StBemcTables.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include "StEEmcUtil/StEEmcSmd/EEmcSmdGeom.h"

#include "StEmcRawMaker/defines.h"

#include "StSpinPool/StJets/StJet.h"
#include "StSpinPool/StJets/StJets.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
#include "StSpinPool/StSpinDbMaker/cstructs/spinConstDB.hh"
#include "StJetMaker/StJetMaker.h"
#include "StJetMaker/StJetReader.h"
#include "StJetMaker/StJetSkimEventMaker.h"

#include "Globals.h"
#include "WBosEvent.h"
#include "ZBosEvent.h"
#include "WeventDisplay.h"
#include "StVecBosMaker.h"

ClassImp(StVecBosMaker)


StVecBosMaker::StVecBosMaker(AnaInfo& anaInfo, const char *name, VecBosRootFile *vbFile): StMaker(name),
   mStopWatch(), mAnaInfo(&anaInfo), mStMuDstMaker(0), mStJetReader(0), mVecBosRootFile(vbFile),
   mJetTreeBranchName(), mJetTreeBranchNameNoEndcap(),
   mJets(0), mVecBosEvent(0), mVecBosTree(0), 
   mNumInputEvents(0), mNumTrigEvents(0), mNumAcceptedEvents(0),
   mRunNo(0), nRun(0), mIsMc(0),
   Tfirst(numeric_limits<int>::max()), Tlast(numeric_limits<int>::min()),
   mL2BarrelTriggerId(0), mL2BarrelTriggerId2(0), mL2EndcapTriggerId(0),
   mParETOWScale(1.0), mParBTOWScale(1.0)   // for old the Endcap geometr you need ~1.3
{
   mStMuDstMaker = (StMuDstMaker*) GetMaker("MuDst");

   if (!mStMuDstMaker) { // load tree if no MuDst
      Info("StVecBosMaker", "MuDst maker is not defined. Creating W tree");
      mTreeChain = new TChain("mVecBosTree", "W candidate events");
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

   setHList(0);
   setHListTpc(0);
   setMC(0);
   setFindZ(0);

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

   // track
   parE_nFitPts                 = 5;    // hits on the track
   parE_nHitFrac                = 0.51;
   parE_trackRin                = 120;
   parE_trackRout               = 70;   // cm
   mMinETrackPt                 = 7.;   // GeV
   parE_nSmdStrip               = 20;
   parE_highET                  = 25.;  // (GeV), cut-off for final Endcap W-cluster

   hbxIdeal                     = 0;

   // irrelevant for W analysis
   par_DsmThres                 = 31;   // only for monitoring
   parE_DsmThres                = 31;   // only for monitoring
   par_maxDisplEve              = 1;    // # of displayed selected events

   use_gains_file               = 0;

   // Year dependent initialization
   if (mAnaInfo->fRhicRunId == 11) {
      mL2BarrelTriggerId  = 320801;
      mL2EndcapTriggerId  = 320851;
   }
   else if (mAnaInfo->fRhicRunId == 12) {
      mL2BarrelTriggerId  = 380209;
      mL2BarrelTriggerId2 = 380219;
      mL2EndcapTriggerId  = 380305;
   }
}


Int_t StVecBosMaker::Init()
{
   if (mStMuDstMaker) {
      //only need DB tables for MuDst analysis
      mBarrelTables = new StBemcTables();
      mDbE = (StEEmcDb*) GetDataSet("StEEmcDb");
      assert(mDbE);
   }

   gBTowGeom       = StEmcGeom::instance("bemc");
   mBSmdGeom[kBSE] = StEmcGeom::instance("bsmde");
   mBSmdGeom[kBSP] = StEmcGeom::instance("bsmdp");

   mGeomEmc = new EEmcGeomSimple();
   mGeomSmd = EEmcSmdGeom::instance();
   InitGeom();

   mEventDisplay = new WeventDisplay(this, par_maxDisplEve);

   if (mIsMc) { // load vertex reweighting histo
      //TString filename="/star/u/stevens4/wAnalysis/efficXsec/zVertReweight.root";
      //TString filename="/star/u/fazio/offline/users/fazio/vbasym/zVertReweight.root";
      //TFile* reweightFile = new TFile(filename);
      //cout<<"Re-weighting vertex z distribution with '"<<nameReweight<<"' histo from file "<<endl<<filename<<endl;
      //hReweight = (TH1F*) reweightFile->Get(nameReweight);

      mMinNumPileupVertices = 1;
   }

   // tree only written during MuDst analysis
   if (mStMuDstMaker) {
      mTreeFile = new TFile(mTreeName, "recreate");
      mTreeFile->cd();
      mVecBosTree = new TTree("t", "mVecBosTree");

      if (mAnaInfo->fBosonType == kWBoson) {
         mVecBosEvent = new WBosEvent();
         mVecBosTree->Branch("e", "WBosEvent", &mVecBosEvent, 128000, 0); // splitlevel=0. very important for custom streamers
      } else if (mAnaInfo->fBosonType == kZBoson) {
         mVecBosEvent = new ZBosEvent();
         mVecBosTree->Branch("e", "ZBosEvent", &mVecBosEvent, 128000, 0); // splitlevel=0. very important for custom streamers
      } else
         Warning("Init", "Unknown type of boson");
   }


   assert(HList);
   mVecBosRootFile->cd();
   InitHistos();
   InitEndcapHistos();

   return StMaker::Init();
}


/** */
Int_t StVecBosMaker::InitRun(int runNo)
{
   Info("InitRun", "Called for run %d", runNo);

   //if (!mIsMc) assert(mRunNo == 0); // to prevent run merging - it was not tested

   if (mStMuDstMaker) {
      mBarrelTables->loadTables(this);
      mRunNo = runNo;
   }

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
         mTpcFilter[iSector-1].setCuts(par_nFitPts, VecBosEvent::sMinTrackHitFrac, Rin, Rout);
         mTpcFilter[iSector-1].init("iSector", iSector, HListTpc, true);

         mTpcFilterE[iSector-1].setCuts(parE_nFitPts, parE_nHitFrac, RinE, RoutE);
         mTpcFilterE[iSector-1].init("secEemcTr", iSector, HListTpc, false);
      }
   }

   // mStSpinDbMaker monitoring
   if (mStMuDstMaker && mStSpinDbMaker)
   {
      char txt[1000], txt0[100];
      sprintf(txt0, "bxIdeal%d", nRun);
      sprintf(txt,  "intended fill pattern  R%d-%d vs. bXing; ", mRunNo, nRun);
      //string str_txt = txt;
      //str_txt += mStSpinDbMaker->getV124comment();
      //Info("InitRun(...)", "v124comment: %s", mStSpinDbMaker->getV124comment());

      nRun++;

      Tfirst = int(2e9);
      Tlast  = -Tfirst;

      hbxIdeal = new TH1F(txt0, txt, 128, -0.5, 127.5);
      hbxIdeal->SetFillColor(kYellow);
      HList->Add(hbxIdeal);

      mStSpinDbMaker->print(0); // 0=short, 1=huge

      for (int bx = 0; bx < 120; bx++) {
         if (mStSpinDbMaker->isBXfilledUsingInternalBX(bx))
            hbxIdeal->Fill(bx);
      }
   }

   return kStOK;
}


Int_t StVecBosMaker::Finish()
{
   if (mStMuDstMaker) {
      cout << endl << "Output tree file " << mTreeName << endl << endl;
      mTreeFile->Write();
      mTreeFile->Close();

      if (hbxIdeal) {
         char txt[1000];
         sprintf(txt, "events T = %d %d", Tfirst, Tlast);
         printf("Finish run=%d, events time range %s\n", mRunNo, txt);
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
   mVecBosEvent->Clear();
   //delete mVecBosEvent;
   //mVecBosEvent = 0;
}


/** Called every event. */
Int_t StVecBosMaker::Make()
{
   mStopWatch.Start(); // restart mStopWatch

   mNumInputEvents++;
   //cout << endl;
   //Info("Make()", "Called for event %d", mNumInputEvents);

   // standard MuDst analysis
   // We need both makers for proper analysis
   if (!mStMuDstMaker || !mStJetReader) {
      mStopWatch.Stop();
      return kStOK;
   }

   mVecBosEvent->InitUsing(mStMuDstMaker);

   int time = mVecBosEvent->time;

   Tlast  = (Tlast  < time) ? time : Tlast;
   Tfirst = (Tfirst > time) ? time : Tfirst;

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
      mStopWatch.Stop();
      return kStOK;
   }

   int btrig = ReadMuDstBarrelTrig();
   int etrig = ReadMuDstEndcapTrig();

   // Skip entire event if no valid trig ID
   if ( btrig != 0 && etrig != 0 ) {
      Info("Make()", "No trigger bit in neither BTOW nor ETOW. Skipping event...");
      mStopWatch.Stop();
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
   ReadMuDstJets(); // Get input jet info

   mVecBosEvent->Process();

   if (mIsMc) {
      mVecBosEvent->ProcessMC();
   }

   mVecBosEvent->SetCpuTimeEventAna( mStopWatch.CpuTime() );

   // Restart stopwatch
   mStopWatch.Continue();

   mVecBosEvent->SetCpuTimeHistFill( mStopWatch.CpuTime() );

   if ( !mVecBosEvent->HasIsolatedTrack() ) {
      return kStOK;
   }

   if (!mIsMc) {
      //m2011WlumiMaker = new St2011WlumiMaker();
   }

   // Write event to tree
   mVecBosEvent->Print();
   mVecBosTree->Fill();

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

   mNumAcceptedEvents++;

   // Add info to the event
   //CalcPtBalance();

   // endcap specific analysis
   //if (hasMatchedTrack2ECluster) {
   //   AnalyzeESMD();
   //   //AnalyzeEPRS(); // not implemented
   //}

   // Fill final histograms
   //if (hasMatchedTrack2BCluster) {
   //   FindWBoson();
   //   FindZBoson();
   //}

   //if (hasMatchedTrack2ECluster) FindWBosonEndcap();

   //mVecBosRootFile->Fill(*mVecBosEvent, kCUT_CUT);

   if (mNumAcceptedEvents == 1 || mNumAcceptedEvents % 1000 == 1 ) {
      Info("Make", "mNumAcceptedEvents: %d", mNumAcceptedEvents);
      mVecBosEvent->Print();
   }

   return kStOK;
}


void StVecBosMaker::InitGeom()
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
         gBSmdStripCoords[iep][towerId - 1] = TVector3(x, y, z);
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
/*
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
         VecBosCluster c = mVecBosEvent->FindMaxBTow2x2(ieta, iphi, mVecBosEvent->mVertices[0].z);

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
                  VecBosCluster c = SumETowPatch(ieta, isec * mxEtowSub + isub, 1, 1, mVecBosEvent->mVertices[0].z);

                  if (c.ET > maxEtowET) maxEtowET = c.ET;
               }
            }
         }
      }

      hE[13]->Fill(mVecBosEvent->mVertices[0].z, maxEtowET);
   }
*/
}


/** */
void StVecBosMaker::ReadMuDstJets()
{
   //if (mStJetReader == 0) {
   //   mVecBosEvent->mNJets = 0;
   //   return;
   //}

   StJets* stJets         = GetStJets(mJetTreeBranchName);
   StJets* stJetsNoEndcap = 0;// XXX GetStJets(mJetTreeBranchNameNoEndcap);

   mVecBosEvent->AddStJets(stJets, stJetsNoEndcap);

   //if (mStJetReader->getStJets(branchName)->eventId() != mVecBosEvent->GetEventId())
   if (stJets->eventId() != mVecBosEvent->GetEventId() || stJets->runId() != mVecBosEvent->GetRunId())
   {
      Error("ReadMuDstJets", "Jet and W run ids do not match: %12d, %12d",   stJets->runId(),   mVecBosEvent->GetRunId());
      Error("ReadMuDstJets", "Jet and W event ids do not match: %12d, %12d", stJets->eventId(), mVecBosEvent->GetEventId());
      Fatal("ReadMuDstJets", "Cannot proceed");
   }

   //if (stJetsNoEndcap->eventId() != mVecBosEvent->GetEventId() || stJetsNoEndcap->runId() != mVecBosEvent->GetRunId())
   //{
   //   Error("ReadMuDstJets", "Jet and W run ids do not match: %12d, %12d (no_endcap branch)",   stJetsNoEndcap->runId(),   mVecBosEvent->GetRunId());
   //   Error("ReadMuDstJets", "Jet and W event ids do not match: %12d, %12d (no_endcap branch)", stJetsNoEndcap->eventId(), mVecBosEvent->GetEventId());
   //   Fatal("ReadMuDstJets", "Cannot proceed");
   //}

   //mVecBosEvent->mNJets = stJets->nJets();
}


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

   while (jetTmp->eventId() != mVecBosEvent->GetEventId() || jetTmp->runId() != mVecBosEvent->GetRunId()) {
      mJetTreeChain->GetEntry(indexJet++);
      jetTmp = GetStJetsFromTree(branchName);
   }

   //cout<<"found matching jet event"<<endl;

   assert(jetTmp->eventId() == mVecBosEvent->GetEventId());
   assert(jetTmp->runId() == mVecBosEvent->GetRunId());
   //mVecBosEvent->mNJets = jetTmp->nJets();
   return jetTmp->jets();
}


StJets* StVecBosMaker::GetStJetsFromTree(TString branchName)
{
   TBranch *branch = mJetTreeChain->GetBranch(branchName);
   return branch ? *(StJets **)branch->GetAddress() : 0;
}


int StVecBosMaker::ReadMuDstBTOW()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      gMessMgr->Warning() << "No EMC data for this event" << endm;
      return -4;
   }

   // kBTow: index for tower & preshower set to BTOW
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

      if (statPed  != 1) { mVecBosEvent->bemc.statTile[kBTow][softID - 1] = 1; n1++; continue; }
      if (statOfl  != 1) { mVecBosEvent->bemc.statTile[kBTow][softID - 1] = 2; n2++; continue; }
      if (statGain != 1) { mVecBosEvent->bemc.statTile[kBTow][softID - 1] = 4; n3++; continue; }

      mVecBosEvent->bemc.statTile[kBTow][softID - 1] = 0 ;

      float ped, sigPed, gain;
      int capID = 0; // just one value for btow

      mBarrelTables->getPedestal(BTOW, softID, capID, ped, sigPed);
      mBarrelTables->getCalib(BTOW, softID, 1, gain);

      //if (use_gains_file == 1) {
      //   gain = gains_BTOW[softID];
      //}

      //printf("id=%d gain=%f\n", softID, gain);

      // method for shifting energy scale
      gain = gain * mParBTOWScale; //(default is mParBTOWScale=1)

      float adc = rawAdc - ped;

      if (adc > 0) n4++;
      if (adc < par_kSigPed * sigPed) continue;
      if (adc < par_AdcThres)         continue;

      n5++;
      mVecBosEvent->bemc.adcTile[kBTow][softID - 1] = adc;
      mVecBosEvent->bemc.eneTile[kBTow][softID - 1] = adc * gain;

      if (maxADC < adc) { maxID = softID; maxADC = adc;}

      adcSum += adc;
   }

   //printf("NNN %d %d %d %d %d %d id=%d\n",n0,n1,n2,n3,n4,n5,maxID);
   if (n0 == mxBtow) {
      Warning("ReadMuDstBTOW", "Missing BEMC info");
      return -1 ; // BTOW was not present in this events
   }

   mVecBosEvent->bemc.tileIn[kBTow] = 1; //tag usable data

   if (mNumInputEvents % 5000 == 1) {
      LOG_INFO << Form("unpackMuBTOW() dataIn=%d, nBbad: ped=%d stat=%d gain=%d ; nAdc: %d>0, %d>thres\n" \
                       "maxADC=%.0f softID=%d adcSum=%.0f",
                       mVecBosEvent->bemc.tileIn[kBTow], n1, n2, n3, n4, n5,
                       maxADC, maxID, adcSum
                      ) << endm;
   }

   hA[31]->Fill(maxADC);
   hA[32]->Fill(adcSum);

   mVecBosEvent->bemc.maxAdc = maxADC;

   if (maxID <= 2400) hA[195]->Fill(maxADC);
   else               hA[196]->Fill(maxADC);

   if (maxADC < par_maxADC) {
      Warning("ReadMuDstBTOW", "Energy deposit is too small in BEMC");
      return -2 ; // not enough energy
   }

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

   // Loop over all towers
   for (int i = 0; i < emc->getNEndcapTowerADC(); i++)
   {
      int sec, eta, sub, rawAdc; //muDst  ranges:sec:1-12, sub:1-5, eta:1-12
      emc->getEndcapTowerADC(i, rawAdc, sec, sub, eta);

      const EEmcDbItem *eEmcDbItem = mDbE->getTile(sec, 'A' + sub - 1, eta, 'T');
      assert(eEmcDbItem); // it should never happened for muDst

      if (eEmcDbItem->fail ) continue; // drop not working channels

      int isec = eEmcDbItem->sec - 1;
      int isub = eEmcDbItem->sub - 'A';
      int ieta = eEmcDbItem->eta - 1;

      assert(isec >= 0 && isec < mxEtowSec); // check input is ok
      assert(isub >= 0 && isub < mxEtowSub);
      assert(ieta >= 0 && ieta < mxEtowEta);

      float adc = rawAdc - eEmcDbItem->ped; // ped subtracted ADC
      if (adc < par_kSigPed * eEmcDbItem->sigPed) continue;

      mVecBosEvent->etow.adc[isec * mxEtowSub + isub][ieta] = adc;

      if (eEmcDbItem->gain <= 0) continue; // drop channels w/o gains
      float ene = adc / eEmcDbItem->gain;

      //method for shifting energy scale
      ene *= mParETOWScale; //(default is mParETOWScale=1)
      mVecBosEvent->etow.ene[isec * mxEtowSub + isub][ieta] = ene;
      mVecBosEvent->etow.stat[isec * mxEtowSub + isub][ieta] = 0;

      if (maxADC < adc) { maxIdName = eEmcDbItem->name; maxADC = adc; maxSec = isec; maxSub = isub; maxEta = ieta;}
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


// Returns non-zero on abort
int StVecBosMaker::ReadMuDstBarrelTrig()
{
   if (mIsMc) {
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

   StMuTriggerIdCollection* triggerIdCollection = &(stMuEvent->triggerIdCollection());
   assert(triggerIdCollection);
   const StTriggerId &l1 = triggerIdCollection->l1();
   vector<unsigned int> idL = l1.triggerIds();

   //printf("nTrig=%d, trigID: ",idL.size());
   for (unsigned int i = 0; i < idL.size(); i++) {
      char txt[100];
      sprintf(txt, "%d", idL[i]);
      //printf("%d, ",idL[i]);
      hA[1]->Fill(txt, 1.);
   }

   //get bX info
   StL0Trigger *trig = &stMuEvent->l0Trigger();

   mVecBosEvent->bx48 = trig->bunchCrossingId();
   mVecBosEvent->bx7  = trig->bunchCrossingId7bit(mRunNo);

   // all 3 DB records exist
   // you do not want mix Long & Trans by accident
   if ( mStSpinDbMaker && mStSpinDbMaker->isValid() )
   {
      mVecBosEvent->bxStar48          = mStSpinDbMaker->BXstarUsingBX48(mVecBosEvent->bx48);
      mVecBosEvent->bxStar7           = mStSpinDbMaker->BXstarUsingBX7(mVecBosEvent->bx7);
      mVecBosEvent->mSpinPattern4Bits = mStSpinDbMaker->spin4usingBX48(mVecBosEvent->bx48);
      mVecBosEvent->mSpinDirection    = mStSpinDbMaker->isPolDirLong() ? polDirLong :
                                       (mStSpinDbMaker->isPolDirTrans() ? polDirTrans : -1);
   } else {
      Info("ReadMuDstBarrelTrig()", "No valid mStSpinDbMaker");
   }


   // Check trigger ID exists = fired
   if ( (mL2BarrelTriggerId  != 0 && !triggerIdCollection->nominal().isTrigger(mL2BarrelTriggerId)) &&
        (mL2BarrelTriggerId2 != 0 && !triggerIdCollection->nominal().isTrigger(mL2BarrelTriggerId2)) )
   {
      Warning("ReadMuDstBarrelTrig", "Trigger %d (and %d) not found", mL2BarrelTriggerId, mL2BarrelTriggerId2);
      return -2;
   }

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


// return non-zero on abort
int StVecBosMaker::ReadMuDstEndcapTrig()
{
   if (mIsMc) {
      if (mVecBosEvent->etow.maxAdc < 10. / 60.*4096)
         return -1; //L2 is HT
      hE[0]->Fill("L2ewET", 1.);
      mVecBosEvent->l2EbitET = true;
      return 0;
   }

   StMuEvent *stMuEvent = mStMuDstMaker->muDst()->event();
   StMuTriggerIdCollection *triggerIdCollection = &(stMuEvent->triggerIdCollection());

   assert(triggerIdCollection);

   const StTriggerId &l1 = triggerIdCollection->l1();
   vector<unsigned int> idL = l1.triggerIds();

   //printf("nTrig=%d, trigID: ",idL.size());
   for (unsigned int i = 0; i < idL.size(); i++) {
      char txt[100];
      sprintf(txt, "%d", idL[i]);
      hE[1]->Fill(txt, 1.);
   }

   // Check trigger ID
   if (!triggerIdCollection->nominal().isTrigger(mL2EndcapTriggerId)) {
      //Warning("ReadMuDstEndcapTrig", "Trigger %d not found", mL2EndcapTriggerId);
      return -2;
   }

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


void StVecBosMaker::ReadMuDstBSMD()
{
   const char cPlane[mxBSmd] = {'E', 'P'};

   // Access to muDst
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      gMessMgr->Warning() << "No EMC data for this muDst event" << endm;
      return;
   }

   // BSMD
   for (int iEP = bsmde; iEP <= bsmdp; iEP++) // official BSMD plane IDs
   {
      int iep = iEP - 3;
      assert(bsmde == 3); // what a hack
      int nHits = emc->getNSmdHits(iEP);
      //printf("muDst BSMD-%c nHit=%d\n",cPlane[iep],nHits);
      int n5 = 0, n1 = 0, n2 = 0, n3 = 0, n4 = 0;

      for (int i = 0; i < nHits; i++)
      {
         StMuEmcHit *hit = emc->getSmdHit(i, iEP);
         float  adc  = hit->getAdc();
         int stripId = hit->getId();

         int statPed, statOfl, statGain;
         mBarrelTables->getStatus(iEP, stripId, statPed, "pedestal");
         mBarrelTables->getStatus(iEP, stripId, statOfl);
         mBarrelTables->getStatus(iEP, stripId, statGain, "calib");

         if (statPed != 1) { mVecBosEvent->bemc.statBsmd[iep][stripId - 1] = 1; n1++; continue; }
         if (statOfl != 1) { mVecBosEvent->bemc.statBsmd[iep][stripId - 1] = 2; n2++; continue; }
         if (statGain < 1 || statGain > 19) {
            mVecBosEvent->bemc.statBsmd[iep][stripId - 1] = 4; n3++; continue;
         }

         float pedRes, sigPed, gain;
         int capID = 0; // just one value for ped residua in pp500, 2009 run

         mBarrelTables->getPedestal(iEP, stripId, capID, pedRes, sigPed);
         mBarrelTables->getCalib(iEP, stripId, 1, gain);

         if (mIsMc) { // overwrite it based on genat DE & private calibration
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
         assert(stripId >= 1 && stripId <= mxBStrips);
         int id0 = stripId - 1;
         mVecBosEvent->bemc.adcBsmd[iep][id0] = adc;
         hA[70 + 10 * iep]->Fill(adc);

         //if(mNumInputEvents<3 || i <20 )printf("  i=%d, smd%c id=%d, m=%d
         // adc=%.3f pedRes=%.1f, sigP=%.1f stat: O=%d P=%d G=%d
         // gain=%.2f\n",i,cPlane[iep],stripId,1+id0/150,adc,pedRes,sigPed,
         // statOfl,statPed,statGain, gain);
      }

      if (mNumTrigEvents % 5000 == 1) {
         LOG_INFO << Form("unpackMuBSMD-%c() nBbad: ped=%d stat=%d gain=%d ; nAdc: %d>0, %d>thres",
                     cPlane[iep], n1, n2, n3, n4, n5) << endm;
      }
   }
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

      // Read tracks associated with this vertex
      ReadMuDstTracks(vecBosVertex);
   }

   if (mVecBosEvent->GetNumVertices() <= 0) return;

   //if (mVecBosEvent->l2bitET && nVerticesPosRank > 0) {
   //   hA[0]->Fill("primVert", 1.);
   //   hA[4]->Fill(mVecBosEvent->bx48);
   //   hA[5]->Fill(mVecBosEvent->bx7);
   //}

   //if (mVecBosEvent->l2EbitET) {
   //   hE[0]->Fill("primVert", 1.);
   //   hE[4]->Fill(mVecBosEvent->bx48);
   //   hE[5]->Fill(mVecBosEvent->bx7);
   //}

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
 *
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

      bool barrelTrack = (mVecBosEvent->l2bitET && vbVertex->mRank > 0 && primaryTrack->flag() == 301 && pt > mVecBosEvent->sMinBTrackPt);

      if (barrelTrack) hA[20]->Fill("ptOK", 1.); //good barrel candidate

      bool endcapTrack = (mVecBosEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin && pt > mMinETrackPt);

      if (endcapTrack) hE[20]->Fill("ptOK", 1.); //good endcap candidate

      //XXX:ds:if (!barrelTrack && !endcapTrack) continue;
   }

   if (mVecBosEvent->GetNumTracks() <= 0) return;

   if (mVecBosEvent->l2bitET)  hA[0]->Fill("Pt10", 1.);
   if (mVecBosEvent->l2EbitET) hE[0]->Fill("Pt10", 1.);
}


void StVecBosMaker::FillTowHit(bool hasVertices)
{
   if (!mVecBosEvent->l2bitET) return; //only barrel triggers

   //find highest rank vertex
   float maxRank   = 0;
   uint  maxRankId = 0;

   VecBosVertexPtrSetIter iVertex = GetVecBosEvent()->mVertices.begin();

   for (short iv=0 ; iVertex != GetVecBosEvent()->mVertices.end(); ++iVertex, iv++)
   {
      VecBosVertex &vertex = **iVertex;

      float rank = vertex.mRank;

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
         float delZ = 0; // XXX:ds gBCalTowerCoords[i].z() - mVecBosEvent->mVertices[maxRankId].z;
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
               float delZ = 0; // XXX:ds gETowCoords[iPhi][ieta].z() - mVecBosEvent->mVertices[maxRankId].z;
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
      VecBosCluster c = mVecBosEvent->FindMaxBTow2x2(ieta, iphi, 0);

      if (c.ET > par_l2emulClusterThresh) return true;
   }

   return false;
}


void StVecBosMaker::FindWBoson()
{
   if (!mVecBosEvent->l2bitET) return;
   if (mVecBosEvent->zTag) return; // Ignore events tagged as Z events

   int nNoNear = 0, nNoAway = 0, nEta1 = 0, nGoldW = 0, nGoldWp = 0, nGoldWn = 0;

   // search for Ws
   VecBosVertexPtrSetIter iVertex = GetVecBosEvent()->mVertices.begin();

   for (short iv=0 ; iVertex != GetVecBosEvent()->mVertices.end(); ++iVertex, iv++)
   {
      VecBosVertex &vertex = **iVertex;

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         VecBosTrack &track = vertex.eleTrack[it];

         if (track.mMatchedTower.id <= 0) continue; //skip endcap towers
         if (track.HasCluster() == false) continue;

         assert(track.mCluster2x2.nTower > 0); // internal logical error
         assert(track.mP3InNearCone.Pt() > 0);      // internal logical error

         // Make cut on lepton eta
         if (track.mP3AtDca.Eta() < mMinBTrackEta || track.mP3AtDca.Eta() > mMaxBTrackEta) continue;

         hA[20]->Fill("eta1", 1.);
         nEta1++;

         //signal plots w/o EEMC in veto
         if (track.mCluster2x2.ET / track.mP3InNearConeNoETow.Pt() > par_nearTotEtFrac) {
            if (track.sPtBalance_noEEMC > par_ptBalance ) { // only signed ptBalance cut
               hA[140]->Fill(track.mCluster2x2.ET);
               hA[240]->Fill(track.mStMuTrack->eta(), track.mCluster2x2.ET);

               if (track.mStMuTrack->charge() < 0) {
                  hA[184 + 3]->Fill(track.mCluster2x2.ET);
               } else if (track.mStMuTrack->charge() > 0) {
                  hA[184 + 4]->Fill(track.mCluster2x2.ET);
               }
            }
         }

         // fill plot for background
         if (track.mCluster2x2.ET > par_highET) {
            if (track.mStMuTrack->charge() > 0)      hA[251]->Fill(track.mCluster2x2.ET / track.mP3InNearCone.Pt(), track.sPtBalance);
            else if (track.mStMuTrack->charge() < 0) hA[252]->Fill(track.mCluster2x2.ET / track.mP3InNearCone.Pt(), track.sPtBalance);
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
                  if (track.mStMuTrack->charge() < 0) {
                     hA[142 + i]->Fill(track.mCluster2x2.ET, j);
                  }
                  else if (track.mStMuTrack->charge() > 0) {
                     hA[163 + i]->Fill(track.mCluster2x2.ET, j);
                  }
               }
            }
         }

         //plots for backg sub yield
         if (track.sPtBalance > par_ptBalance ) {
            hA[136]->Fill(track.mCluster2x2.ET);//signal
            hA[241]->Fill(track.mStMuTrack->eta(), track.mCluster2x2.ET);
            hA[62]->Fill(track.mMatchedTower.iEta , track.mCluster2x2.mEnergy);
            if (track.mStMuTrack->charge() < 0) {
               hA[184 + 1]->Fill(track.mCluster2x2.ET);
            }
            else if (track.mStMuTrack->charge() > 0) {
               hA[184 + 2]->Fill(track.mCluster2x2.ET);
            }
         }
         else {
            hA[137]->Fill(track.mCluster2x2.ET);//background
            if (track.mStMuTrack->charge() < 0) {
               hA[184 + 5]->Fill(track.mCluster2x2.ET);
            }
            else if (track.mStMuTrack->charge() > 0) {
               hA[184 + 6]->Fill(track.mCluster2x2.ET);
            }
            hA[202]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mStMuTrack->pt());
            hA[204]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.mEnergy / track.mStMuTrack->p().mag());
         }

         if (track.sPtBalance > par_ptBalance) {
            Info("FindWBoson", "WWWWWWWWWWWWWWWWWWWWW  Barrel");
            mEventDisplay->exportEvent("WB", vertex, track, iv);
            mVecBosEvent->Print();
         }

         // put final W cut here
         if (track.sPtBalance < par_ptBalance)  continue;

         hA[20]->Fill("noAway", 1.0);
         nNoAway++;

         // accepted W events for x-section
         hA[113]->Fill(track.mCluster2x2.ET); // for Joe
         hA[90]->Fill (track.mCluster2x2.ET);
         hA[92]->Fill (track.mCluster2x2.ET, track.glMuTrack->dEdx() * 1e6);
         //hA[93]->Fill( track.mCluster2x2.ET,track.glMuTrack->dca(vertex.id).mag());

         int k = 0;
         if (track.mStMuTrack->charge() < 0) k = 1;

         hA[94 + k]->Fill( track.mCluster2x2.ET, track.glMuTrack->dcaD());
         // h95 used above

         // plots to investigate east/west yield diff
         hA[200]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.ET);
         hA[201]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mStMuTrack->pt());
         hA[203]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.mEnergy / track.mStMuTrack->p().mag());
         hA[205]->Fill(track.mStMuTrack->lastPoint().pseudoRapidity(), track.mStMuTrack->lastPoint().phi());

         // Q/pT plot
         hA[100]->Fill(track.mCluster2x2.ET, track.glMuTrack->charge() / track.glMuTrack->pt());
         hA[101]->Fill(track.mCluster2x2.ET, track.mStMuTrack->charge() / track.mStMuTrack->pt());

         if (track.mCluster2x2.ET < par_highET) continue; // very likely Ws

         hA[91]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.position.Phi());
         hA[96]->Fill(vertex.mIdMuDst);
         hA[97]->Fill(vertex.mRankLog);
         hA[98]->Fill(vertex.z);
         hA[99]->Fill(track.mStMuTrack->eta());
         hA[190 + k]->Fill(track.mStMuTrack->eta(), track.mCluster2x2.ET);

         hA[20]->Fill("goldW", 1.);
         nGoldW++;

         if (track.mStMuTrack->charge() > 0)      nGoldWp++;
         else if (track.mStMuTrack->charge() < 0) nGoldWn++;
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
   VecBosTrackPtrSetIter iTrack = mVecBosEvent->mTracks.begin();

   for ( ; iTrack!=mVecBosEvent->mTracks.end(); ++iTrack)
   {
      VecBosTrack &track = **iTrack;
      if ( !track.HasCluster() ) continue;

      assert(track.mCluster2x2.nTower > 0); // internal logical error
      assert(track.mP3InNearCone.Pt() > 0); // internal logical error

      if (track.mCluster2x2.ET / track.mP3InNearCone.Pt() < par_nearTotEtFrac) continue; // too large nearET

      //match lepton candidate with jet
      //for (uint iJet = 0; iJet < mVecBosEvent->mNJets; iJet++) //jet loop

      VecBosJetPtrSetIter iJet = mVecBosEvent->mJets.begin();

      for ( ; iJet!=mVecBosEvent->mJets.end(); ++iJet)
      {
         StJet *stJet = *iJet;
         if (stJet->Pt() < minJetPt) continue; // ignore low pt jets

         //electron like cut on jets
         //StJet *jet = GetJet(stJet);
         float maxClusterET = 0.;
         int   numAllJetTowers  = stJet->nBtowers + stJet->nEtowers;

         for (int itow = 0; itow < numAllJetTowers; itow++) // loop over towers
         {
            // Ignore endcap towers
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

         if (jetVec3.DeltaR(track.mP3AtDca) < mVecBosEvent->sMinTrackIsoDeltaR) continue; // skip jets in candidate phi isolation cone

         // form invariant mass
         float    e1 = track.mCluster2x2.mEnergy;
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
   VecBosVertexPtrSetIter iVertex = GetVecBosEvent()->mVertices.begin();

   for ( ; iVertex != GetVecBosEvent()->mVertices.end(); ++iVertex)
   {
      VecBosVertex &vertex = **iVertex;

      for (uint iTrack = 0; iTrack < vertex.eleTrack.size(); iTrack++)
      {
         VecBosTrack &track = vertex.eleTrack[iTrack];

         if (track.HasCluster() == false) continue;

         // Loop over branch with EEMC
         //mJets = GetJets(mJetTreeBranchName);
         //if (mJetTreeChain) mJets = GetJetsTreeAnalysis(mJetTreeBranchName);

         // Add up all jets outside of nearDeltaR cone around the electron track
         // Loop over jets
         for (uint iJet = 0; iJet<mVecBosEvent->GetNumStJets(); iJet++)
         {
            StJet *jet = GetJet(iJet);
            TVector3 jetVec; //vector for jet momentum
            jetVec.SetPtEtaPhi(jet->Pt(), jet->Eta(), jet->Phi());

            if (jetVec.DeltaR(track.mP3AtDca) > mVecBosEvent->sMinTrackIsoDeltaR)
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
         for (uint iJet = 0; iJet < mVecBosEvent->GetNumStJetsNoEndcap(); iJet++)
         {
            StJet *jet = GetJet(iJet); // XXX:ds: Need different access method for noendcap jets
            TVector3 jetVec; //vector for jet momentum
            jetVec.SetPtEtaPhi(jet->Pt(), jet->Eta(), jet->Phi());

            if (jetVec.DeltaR(track.mP3AtDca) > mVecBosEvent->sMinTrackIsoDeltaR)
               track.ptBalance_noEEMC += jetVec;
         }

         track.ptBalance_noEEMC += clustPt;
         track.sPtBalance_noEEMC = track.ptBalance_noEEMC.Perp();

         if (track.ptBalance_noEEMC.Dot(clustPt) < 0)
            track.sPtBalance_noEEMC *= -1.;
      }
   }
}


void StVecBosMaker::FindWBosonEndcap()
{
   if (!mVecBosEvent->l2EbitET) return;

   //printf("========= FindWBosonEndcap() \n");
   int nNoNear = 0, nNoAway = 0, nGoldW = 0;

   // Search for  Ws
   VecBosVertexPtrSetIter iVertex = GetVecBosEvent()->mVertices.begin();

   for (short iv=0 ; iVertex != GetVecBosEvent()->mVertices.end(); ++iVertex, iv++)
   {
      VecBosVertex &vertex = **iVertex;

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         VecBosTrack &track = vertex.eleTrack[it];
         if (track.mMatchedTower.id >= 0) continue; //skip barrel towers
         if (track.HasCluster() == false) continue;
         assert(track.mCluster2x2.nTower > 0); // internal logical error
         // XXX:ds: assert(track.mP3InNearCone.Pt() > 0); // internal logical error
         if (track.mP3InNearCone.Pt() < 0) continue; // internal logical error

         //signal plots w/o EEMC in veto
         if (track.mCluster2x2.ET / track.mP3InNearConeNoETow.Pt() > parE_nearTotEtFrac) {
            if (track.sPtBalance_noEEMC > parE_ptBalance ) { //only signed ptBalance cut
               hE[140]->Fill(track.mCluster2x2.ET);
               if (track.mStMuTrack->charge() < 0) {
                  hE[184 + 3]->Fill(track.mCluster2x2.ET);
               }
               else if (track.mStMuTrack->charge() > 0) {
                  hE[184 + 4]->Fill(track.mCluster2x2.ET);
               }
            }
         }

         if (track.mCluster2x2.ET / track.mP3InNearCone.Pt() < parE_nearTotEtFrac) continue; // too large nearET

         hE[20]->Fill("noNear", 1.);
         nNoNear++;
         hE[112]->Fill(track.mCluster2x2.ET); // for Joe
         hE[50]->Fill(track.awayTpcPT);
         hE[51]->Fill(track.awayBtowET);
         hE[54]->Fill(track.awayTotET);
         hE[52]->Fill(track.mCluster2x2.ET, track.awayTotET);
         hE[53]->Fill(track.mCluster2x2.ET, track.awayEmcET);
         hE[55]->Fill(track.awayEtowET);
         hE[60]->Fill(track.mCluster2x2.ET, track.awayTpcPT);

         hE[132]->Fill(track.mCluster2x2.ET, track.ptBalance.Perp());
         hE[133]->Fill(track.awayTotET, track.ptBalance.Perp());
         hE[134]->Fill(track.mCluster2x2.ET, track.sPtBalance);
         hE[135]->Fill(track.awayTotET, track.sPtBalance);

         //plots for backg sub yield
         if (track.sPtBalance > parE_ptBalance) {
            hE[136]->Fill(track.mCluster2x2.ET);//signal
            hE[62]->Fill(track.mMatchedTower.iEta , track.mCluster2x2.mEnergy);
            if (track.mStMuTrack->charge() < 0) {
               hE[184 + 1]->Fill(track.mCluster2x2.ET);
            }
            else if (track.mStMuTrack->charge() > 0) {
               hE[184 + 2]->Fill(track.mCluster2x2.ET);
            }
         }
         else {
            hE[137]->Fill(track.mCluster2x2.ET);//background
            if (track.mStMuTrack->charge() < 0) {
               hE[184 + 5]->Fill(track.mCluster2x2.ET);
            }
            else if (track.mStMuTrack->charge() > 0) {
               hE[184 + 6]->Fill(track.mCluster2x2.ET);
            }
         }

         if (track.sPtBalance > parE_ptBalance) {
            Info("FindWBoson", "WWWWWWWWWWWWWWWWWWWWW  Endcap");
            mEventDisplay->exportEvent("WE", vertex, track, iv);
            mVecBosEvent->Print();
         }

         // Put final W cut here
         if (track.sPtBalance < parE_ptBalance)  continue;
         //::::::::::::::::::::::::::::::::::::::::::::::::
         //:::::accepted W events for x-section :::::::::::
         //::::::::::::::::::::::::::::::::::::::::::::::::

         hE[20]->Fill("noAway", 1.0);
         nNoAway++;
         hE[113]->Fill( track.mCluster2x2.ET);//for Joe

         hE[90]->Fill( track.mCluster2x2.ET);
         hE[92]->Fill( track.mCluster2x2.ET, track.glMuTrack->dEdx() * 1e6);
         //hE[93]->Fill( track.mCluster2x2.ET,track.glMuTrack->dca().mag());
         int k = 0; if (track.mStMuTrack->charge() < 0) k = 1;
         hE[94 + k]->Fill( track.mCluster2x2.ET, track.glMuTrack->dcaD());
         // h95 used above

         // do charge sign plot
         float ET = track.mCluster2x2.ET;
         const StMuTrack *glTr = track.glMuTrack; assert(glTr);
         const StMuTrack *prTr = track.mStMuTrack; assert(prTr);
         float g_chrg = glTr->charge();
         float p_chrg = prTr->charge();
         hE[200]->Fill(ET, g_chrg / glTr->pt());
         hE[201]->Fill(ET, p_chrg / prTr->pt());

         if (track.mCluster2x2.ET < par_highET) continue; // very likely Ws
         hE[91]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.position.Phi());
         hE[96]->Fill(vertex.mIdMuDst);
         hE[97]->Fill(vertex.mRankLog);
         hE[98]->Fill(vertex.z);
         hE[99]->Fill( track.mStMuTrack->eta());
         hE[100]->Fill(track.mMatchedTower.R.X(), track.mMatchedTower.R.Y());
         hE[190 + k]->Fill(track.mStMuTrack->eta(), track.mCluster2x2.ET);
         hE[20]->Fill("goldW", 1.);
         nGoldW++;

      } // loop over tracks
   } // loop over vertices

   if (nNoNear > 0) hE[0]->Fill("noNear", 1.);
   if (nNoAway > 0) hE[0]->Fill("noAway", 1.);
   if (nGoldW > 0)
      hE[0]->Fill("goldW", 1.);
}


void StVecBosMaker::AnalyzeESMD()
{
   if (!mVecBosEvent->l2EbitET) return;
   //Info("AnalyzeESMD", "");

   VecBosVertexPtrSetIter iVertex = GetVecBosEvent()->mVertices.begin();

   for (short iv=0 ; iVertex != GetVecBosEvent()->mVertices.end(); ++iVertex, iv++)
   {
      VecBosVertex &vertex = **iVertex;

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         VecBosTrack &T = vertex.eleTrack[it];
         if (T.mMatchedTower.id >= 0) continue; //skip barrel towers
         if (T.HasCluster() == false) continue;

         assert(T.mCluster2x2.nTower > 0); // internal logical error

         // Id of strips pointed by prim and glob tracks in each plane
         int hitStrip[2]     = { -1, -1};
         int hitStripGlob[2] = { -1, -1};

         // Initialize shower shape histograms
         TH1F *esmdShowerHist[2];
         esmdShowerHist[0] = new TH1F(Form("esmdU%d", mVecBosEvent->GetEventId()), "esmdU", 41, -10.25, 10.25);
         esmdShowerHist[1] = new TH1F(Form("esmdV%d", mVecBosEvent->GetEventId()), "esmdV", 41, -10.25, 10.25);

         // Loop over planes
         for (int iuv = 0; iuv < 2; iuv++)
         {
            Float_t dca; //primary extrapolation to smd plane
            const StructEEmcStrip *stripPtr = mGeomSmd->getDca2Strip(iuv, T.mMatchedTower.R, &dca); // find pointed strip

            if (!stripPtr) { cout << "No Strip found" << endl; continue;}
            if (fabs(dca) > 0.5) { cout << "DCA to big" << endl; continue;}  // in cm

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


void StVecBosMaker::AnalyzeEPRS()
{
   if (!mVecBosEvent->l2EbitET) return;
   // Info("AnalyzeEPRS");

   VecBosVertexPtrSetIter iVertex = GetVecBosEvent()->mVertices.begin();

   for (short iv=0 ; iVertex != GetVecBosEvent()->mVertices.end(); ++iVertex, iv++)
   {
      VecBosVertex &vertex = **iVertex;

      for (uint it = 0; it < vertex.eleTrack.size(); it++)
      {
         VecBosTrack &T = vertex.eleTrack[it];
         if (T.mMatchedTower.id >= 0) continue; // skip barrel towers
         if (T.HasCluster() == false) continue;
         assert(T.mCluster2x2.nTower > 0); // internal logical error

         //do some clustering of EPRS deposits and plot histos
      }
   }
}


/**
 * (Deprecated) Creates root histograms for the barrel calorimeter.
 */
void StVecBosMaker::InitHistos()
{
   // data histograms
   memset(hA, 0, sizeof(hA));

   TList *Lx;
   TLine *ln;
   TH1 *h;
   char txt[1000], txt0[100];
   int  nCase = 20;

   hA[0] = h = new TH1F("hEventStat", Form("Barrel W-algo: event count : %s", GetName()), nCase, 0, nCase);
   h->GetXaxis()->SetTitleOffset(0.4);
   h->GetXaxis()->SetLabelSize(0.06);
   h->GetXaxis()->SetTitleSize(0.05);
   h->SetMinimum(0.8);
   h->SetLineColor(kBlue);
   h->SetLineWidth(2);
   h->SetMarkerSize(2);

   char key[][200] = {"inp", "L2bwId", "L2bwBits", "L2bwET", "L2bwRnd", "tpcOn", "primVert", "vertZ", "Pt10",
                      "B-in", "B200", "TrB", "Tr2Cl", "eta1", "noNear", "noAway", "goldW", "goldW+", "goldW-"};

   for (int i = 0; i < 19; i++) h->Fill(key[i], 0.); // preset the order of keys

   hA[1] = h = new TH1F("muInTrg", "mu Barrel W input triggers, WARN: scrambled if many runs are combined by hadd.C; trigID (random order)", nCase, 0, nCase);
   h->GetXaxis()->SetLabelSize(0.06);

   hA[2] = h = new TH1F("mubX48", "L2WB-ET events vs. bXing; bXing= raw bx48", 128, -0.5, 127.5);
   h->SetFillColor(kGreen);

   hA[3] = h = new TH1F("mubX7", "L2WB-ET events vs. bXing; bXing= raw bx7", 128, -0.5, 127.5);
   h->SetFillColor(kBlue);

   hA[4] = h = new TH1F("mubX48v", "L2WB-ET & primVertex  vs. bXing; bXing= raw bx48", 128, -0.5, 127.5);
   hA[5] = h = new TH1F("mubX7v", "L2WB-ET & primVertex; bXing= raw bx7", 128, -0.5, 127.5);
   h->SetFillColor(kBlue);

   // DMS data
   hA[6] = h = new TH1F("muDsm1", "L2WB-ET events DMS spectrum; DSM value", 64, -0.5, 63.5);
   h->SetMinimum(0.8);

   hA[7] = h = new TH1F("muDsm2", "L2WB-Rnd events DMS spectrum; DSM value", 64, -0.5, 63.5);
   h->SetMinimum(0.8);
   sprintf(txt, "L2WB-ET events w/ DMS>%d vs. BTOW TP ID bXing; Hanks' TP ID", par_DsmThres);

   hA[8] = new TH1F("muDsm3", txt, 300, -0.5, 299.5);
   sprintf(txt, "L2WB-ET events w/ DMS>%d & primVertex vs. BTOW TP ID bXing; Hanks' TP ID", par_DsmThres);

   hA[9] = h = new TH1F("muDsm4", txt, 300, -0.5, 299.5);
   h->SetFillColor(kBlue); h->SetLineColor(kBlue);


   // vertex histograms
   hA[10] = new TH1I("hVertexRankLog", "; Vertex Rank, X=Log10(rank)+offset", 150, -9, 25);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, 0, 0, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hA[14] = new TH1I("hVertexRank", "; Vertex Rank", 100, -1000, 1000);


   hA[11] = h = new TH1F("muZv", "L2WB: Z of any vertex w/ rank>0;Z-vertex (cm)", 100, -200, 200);
   Lx = h->GetListOfFunctions();
   ln = new TLine(mCutVertexZ, 0, mCutVertexZ, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);
   ln = new TLine(-mCutVertexZ, 0, -mCutVertexZ, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hA[12] = new TH1F("muNV", "L2WB: # vertices per event, rank>0 & Z in range; # of vertices", 10, 0, 10);

   // intended for QCD MC normalization
   hA[13] = h = new TH2F("BtowMaxCl_Zv",
      "L2WB: BTOW max 2x2 cluster ET vs Z of any vertex w/ rank>0; Z-vertex (cm); max BTOW 2x2 Cluster ET", 100, -100, 100, 100, 0, 100);

   //..... Tracks....
   hA[20] = h = new TH1F("muStatTrk", "Barrel W-algo: track  count; cases", nCase, 0, nCase);
   h->GetXaxis()->SetTitleOffset(0.4);
   h->GetXaxis()->SetLabelSize(0.06);
   h->GetXaxis()->SetTitleSize(0.05);
   h->SetMinimum(0.8);
   h->SetLineColor(kGreen);
   h->SetLineWidth(2);

   char keyT[][200] = {"flag", "pt1", "nHit", "Hfrac", "Rin", "Rout", "ptOK", "@B", "CL", "fr24",
                       "#Delta R", "eta1", "noNear", "noAway", "goldW" };

   for (int i = 0; i < 15; i++) h->Fill(keyT[i], 0.); // preset the order of keys

   hA[21] = h = new TH1F("muTrNfit", "Barrel: primary track in-selection & vertexZ; nFitPoints", 50, 0, 50);
   Lx = h->GetListOfFunctions();
   ln = new TLine(par_nFitPts, 0, par_nFitPts, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hA[22] = h = new TH1F("muTrFitFrac", "Barrel: primary track in-selection & vertexZ; nFit/nPoss ", 50, 0, 1.1);
   Lx = h->GetListOfFunctions();
   ln = new TLine(VecBosEvent::sMinTrackHitFrac, 0, VecBosEvent::sMinTrackHitFrac, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hA[23] = h = new TH1F("muTrRxyIn", "Barrel: primary track first hit  in-selection & vertexZ; Rxy (cm)", 60, 50, 170.);
   Lx = h->GetListOfFunctions();
   ln = new TLine(par_trackRin, 0, par_trackRin, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hA[24] = h = new TH1F("muTrRxyOut", "Barrel: primary track last hit  in-selection & vertexZ; Rxy (cm)", 60, 100, 220.);
   Lx = h->GetListOfFunctions();
   ln = new TLine(par_trackRout, 0, par_trackRout, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hA[25] = h = new TH1F("muTrPt1", "Barrel: global track PT ; track PT (GeV/c)", 160, 0, 80);
   Lx = h->GetListOfFunctions();
   //ln = new TLine(mVecBosEvent->mMinBTrackPt, 0, mVecBosEvent->mMinBTrackPt, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln); h->SetFillColor(kYellow);


   hA[26] = h = new TH2F("muTr2D1", "Barrel: lastHit on track; detector eta ; detector phi (rad)", 100, -1.1, 1.1, 200, -3.2, 3.2);

   hA[27] = h = new TH1F("muTrPt1N", "Barrel: global NEGATIVE track PT; track PT (GeV/c)", 160, 0, 80);
   h->SetFillColor(7);

   hA[28] = h = new TH2F("muTrdEdX", "Barrel: dEdX vs. momentum; track P (GeV); dE/dx (keV)", 20, 0, 10, 100, 0, 10);

   hA[29] = h = new TH1F("muTrPt1Pr", "Barrel: primary  track PT; track PT (GeV/c)", 160, 0, 80);
   hA[30] = h = new TH1F("muTrPt1NPr", "Barrel: primary  NEGATIVE track PT; track PT (GeV/c)", 160, 0, 80);

   // BTOW
   hA[31] = h = new TH1F("muBmaxAdc", "Barrel: BTOW maxADC in event, in-selection; max tower ADC", 200, 0, 4500);
   hA[32] = h = new TH1F("muBtotAdc", "Barrel: BTOW sum of ADC>thres , in-selection;ADC sum/event", 120, 0, 12000.);

   hA[33] = h = new TH1F("muBclET", "matched BTOW 2x2 cluster ET  ;cluster  ET (GeV)", 100, 0, 100);
   ln = new TLine(mMinBClusterEnergy, 0, mMinBClusterEnergy, 1.e6);
   ln->SetLineColor(kRed);
   Lx = h->GetListOfFunctions();
   Lx->Add(ln);

   hA[34] = h = new TH2F("muBclAdcPt", "Barrel: matched  TPC PT vs. 2x2 cluster ADC sum ; cluster (ADC sum); TPC  PT (GeV)", 50, 0, 5000, 75, 0, 150);

   hA[35] = new TH1F("muTrch2", "Barrel: track glob chi2/dof X-Y", 100, 0, 5);
   hA[36] = new TH2F("muTrch2b", "Barrel: track glob chi2/dof; chi2/dof  X-Y; last hit eta", 30, 0, 5., 30, -1, 1);

   // 4x4 cluster
   hA[37] = h = new TH1F("muBclET24", "matched BTOW 4x4 cluster ET; cluster 4x4  ET (GeV)", 100, 0, 100);
   hA[38] = h = new TH2F("muBclE242D", "Barrel: Excess energy in 4x4 cluster vs. 2x2 E;2x2 cluster E (GeV); E(4x4)-E(2x2)  E (GeV)", 50, 0, 80, 50, 0, 60);

   hA[39] = h = new TH1F("muBclET24R", "Barrel: ratio (2x2/4x4) cluster ET; fraction: cluster ET 2x2/ 4x4 ET", 100, 0, 1.2);
   Lx = h->GetListOfFunctions();
   ln = new TLine(mMinBClusterEnergyIsoRatio, 0, mMinBClusterEnergyIsoRatio, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   // jet energy fraction
   hA[40] = h = new TH1F("muBjetET", "Barrel: near 'EM jet' ET ; 'EM jet'  ET (GeV)", 100, 0, 100);

   hA[41] = h = new TH2F("muBclEjetE2D", "Barrel: Excess nearCone ET  vs. 2x2 E;2x2 cluster ET (GeV); ET(cone-2x2) (GeV)", 50, 0, 80, 50, 0, 60);

   hA[42] = h = new TH1F("muBjetETR", "Barrel: ratio (2x2/nearCone) ET ; cluster ET/ near cone ET", 100, 0, 1.2);
   Lx = h->GetListOfFunctions();
   ln = new TLine(par_nearTotEtFrac, 0, par_nearTotEtFrac, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   // track-EMC distance cuts
   hA[43] = h = new TH2F("muBdist1", "Barrel: 3D Distance(track-cluster) vs. 2x2 E;2x2 cluster E (GeV); | distance | (cm)", 50, 0, 80, 50, 0, 25);
   hA[44] = h = new TH2F("muBdist2", "Barrel: #Delta Z   (track-cluster) vs.Z-clust; Z-cluster (cm); #Delta Z (cm)", 100, -300, 300, 40, -20, 20);
   hA[45] = h = new TH2F("muBdist3", "Barrel: R#Delta #phi   (track-cluster) vs. 2x2 E;2x2 cluster E (GeV); R#Delta #phi (cm)", 50, 0, 80, 80, -20, 20);
   hA[46] = h = new TH1F("muBdist4", "Barrel: 3D Distance(track-cluster) vs. 2x2 E;| 3D distance |   (cm)", 100, 0, 50);
   Lx = h->GetListOfFunctions();
   //ln = new TLine(mVecBosEvent->mMaxTrackClusterDist, 0, mVecBosEvent->mMaxTrackClusterDist, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   // continue same side jet veto
   hA[47] = h = new TH1F("muTjetET", "Barrel: quenched near 'TPC jet' PT ; 'TPC jet'  PT (GeV)", 100, 0, 100);
   hA[48] = h = new TH2F("muTjetBjet2D", "Barrel: quenched  near 'TPC jet' vs.  near 'EMC jet' ; EMC jet ET (GeV); TPC PT  (GeV/c)", 50, 0, 80, 50, 0, 60);
   hA[49] = h = new TH1F("muTBjetET", "Barrel: near 'TPC+EMC jet' ET ; jet ET (GeV), no double counting", 100, 0, 100);

   // away-side jet  veto
   hA[50] = h = new TH1F("muTwayET", "Barrel: TPC away-cone PT sum;   PT (GeV)", 100, 0, 100);
   hA[51] = h = new TH1F("muBwayET", "Barrel: BTOW away-cone ET sum;   ET (GeV)", 100, 0, 100);
   hA[52] = h = new TH2F("muTotwayET2D", "Barrel: away TPC+EMC ET sum  vs. 2x2 cluster ET;  2x2 ET (GeV); away ET (GeV)", 50, 0, 100, 150, 0, 100);
   hA[53] = h = new TH2F("muAwayET2Db", "Barrel: away EMC ET sum  vs. 2x2 cluster ET;  2x2 ET (GeV); away EMC ET (GeV)", 50, 0, 100, 80, 0, 80);
   hA[54] = h = new TH1F("muAwayTotEt", "Barrel: away-cone TPC+EMC ET sum ; away ET (GeV)", 200, 0, 100);
   hA[55] = h = new TH1F("muEwayET", "Barrel: ETOW away-cone ET sum;   ET (GeV)", 100, 0, 100); // away side energy

   hA[57] = h = new TH2F("muTr2D1pt5", "Barrel: lastHit on track (pt > 5); detector eta ; detector phi (rad)", 100, -1.1, 1.1, 240, -M_PI, M_PI);
   hA[58] = new TH1F("muTrch2West", "Barrel: track glob chi2/dof  West TPC ; chi2/dof", 100, 0, 5);
   hA[59] = new TH1F("muTrch2East", "Barrel: track glob chi2/dof  East TPC ; chi2/dof", 100, 0, 5);

   // final golden plots
   hA[60] = h = new TH2F("muBclETPt", "Barrel: Awayside TPC PT vs.  isolated cluster 2x2 ET, matched;2x2 cluster ET (GeV) ; Awayside TPC  PT (GeV)", 50, 0, 100, 75, 0, 150);

   hA[61] = new TH1F("mubX7bht", "L2BW-BHT-rnd  events vs. bXing; bXing= raw bx7", 128, -0.5, 127.5); // filled on input
   hA[62] = h = new TH2F("muEne_Deta", "Barrel W: cluster energy vs. detector eta, final selection; barrel eta bin; 2x2 Energy (GeV)", 40, 0, 40, 50, 0, 100);

   // free 63-69

   //..... BSMD ......reserve 2x10= [70:e.... 80:p.... 89]
   const char cPlane[ mxBSmd] = {'E', 'P'};

   for (int iep = 0; iep < mxBSmd; iep++) {
      sprintf(txt0, "muS%cadc1", cPlane[iep]);
      sprintf(txt, "BSMD-%c whole plane ADC; ADC", cPlane[iep]);
      hA[70 + 10 * iep] = h = new TH1F(txt0, txt, 275, 0, 1100);
   } // end of E-,P-planes

   // final Ws
   hA[90] = h = new TH1F("muWET", Form("Barrel W: Final Selection : %s; 2x2 cluster ET (GeV)", GetName()), 100, 0, 100);
   Lx = h->GetListOfFunctions();
   ln = new TLine(par_highET, 0, par_highET, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   sprintf(txt, "Barrel W: Final selection, ET>%.0f GeV 'goldenW'; detector eta ; detector phi (rad)", par_highET);
   hA[91] = new TH2F("muW2D1", txt, 10, -1.0, 1.0, 24, -M_PI, M_PI);

   hA[92] = new TH2F("muWdedx",    "Barrel W: Track dEdx, final selection; 2x2 ET (GeV); dEdx (keV)", 100, 0, 100, 100, 0, 10);
   hA[93] = new TH2F("muWglDca",   "Barrel W: Track glob vertex abs(DCA), final selection ; 2x2 ET (GeV); |DCA| (cm)", 100, 0, 100, 100, 0, 5);
   hA[94] = new TH2F("muWglDcaSP", "Barrel W: Track prim POSITIVE glob signed DCA, final selection; 2x2 ET (GeV); sDCA (cm)", 100, 0, 100, 100, -5, 5);
   hA[95] = new TH2F("muWglDcaSN", "Barrel W: Track prim NEGATIVE glob signed DCA, final selection ; 2x2 ET (GeV); sDCA (cm)", 100, 0, 100, 100, -5, 5);

   sprintf(txt, "Barrel W: Vertex ID, final selection, 2x2 ET>%.0f GeV; vertex ID", par_highET);
   hA[96] = h = new TH1F("muWcar1", txt, 10, -0.5, 9.5);
   h->SetLineColor(kBlue);
   h->SetLineWidth(3);

   sprintf(txt, "Barrel W: Vertex 'funny' rank, final W selection, 2x2 ET>%.0f GeV;  X=Log10(rank)+offset", par_highET);
   hA[97] = new TH1F("muWcar2", txt, 150, -9, 25);

   sprintf(txt, "Barrel W: Vertex Z, final selection, 2x2 ET>%.0f GeV; Z(cm)", par_highET);
   hA[98] = new TH1F("muWcar3", txt, 100, -200, 200);
   hA[99] = h = new TH1F("muWeta", Form("Barrel W: lepton eta final selection %s ; lepton eta", GetName()), 400, -2.0, 2.0);

   //Q/pt plots
   sprintf(txt, "TPC GLOB Q/PT  ; 2x2 cluster ET (GeV); Q/PT");
   hA[100] = h = new TH2F("muChRecPNg", txt, 100, 0., 100., 100, -0.1, 0.1);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, 0, 100, 0);  ln->SetLineColor(kMagenta);  Lx->Add(ln);

   sprintf(txt, "TPC PRIM  Q/PT ; 2x2 cluster ET (GeV); Q/PT");
   hA[101] = h = new TH2F("muChRecPNp", txt, 100, 0., 100., 100, -0.1, 0.1);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, 0, 100, 0);  ln->SetLineColor(kMagenta);  Lx->Add(ln);

   // free 102-109

   // A set of electron ET plots after succesive cuts
   char tt2[][200] = {"max 2x2", "track matched", "no near ET", "no away ET"};
   for (int i = 0; i < 4; i++) {
      sprintf(txt, "Barrel electron candidate, cut=%s; 2x2 ET (GeV)", tt2[i]);
      sprintf(txt0, "muETlive%d", i);
      hA[110 + i] = h = new TH1F(txt0, txt, 100, 0, 100);
   }

   //free 114-131

   //hA[116] = new TH1I("jetCount", "; Num. of Jets; Events", 20, 0, 20);
   hA[117] = new TH2I("jetEtaVsPhi", "; Jet #eta; Jet #phi; Events", 50, -3, 3, 63, -M_PI, M_PI);
   hA[118] = new TH1I("jetPT", "; Jet P_{T}; Events", 100, 0, 100);

   hA[132] = h = new TH2F("ptBalance_clust",   "ptBalance vs cluster ET; 2x2 Cluster ET; ptBalance", 100, 0, 100, 100, 0, 100);
   hA[133] = h = new TH2F("ptBalance_awayTot", "ptBalance vs awayside PT; awayside PT; ptBalance", 100, 0, 100, 100, 0, 100);

   hA[134] = h = new TH2F("musPtBalance_clust", "Barrel: sPtBalance vs cluster ET; 2x2 Cluster ET (GeV); signed Pt balance (GeV)", 100, 0, 100, 100, -100, 100);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, par_ptBalance, 100, par_ptBalance);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hA[135] = h = new TH2F("musPtBalance_awayTot", "Barrel: sPtBalance vs awayside PT; awayside PT; sPtBalance", 100, 0, 100, 100, -100, 100);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, par_ptBalance, 100, par_ptBalance);  ln->SetLineColor(kRed);  Lx->Add(ln);


   hA[136] = h = new TH1F("muclustPtBal", Form("Barrel: PT Balance > %.1f ; 2x2 Cluster ET", par_ptBalance), 100, 0, 100);
   hA[137] = h = new TH1F("muclustPtBal_bckgrd", Form("Barrel: PT Balance < %.1f ; 2x2 Cluster ET", par_ptBalance), 100, 0, 100);
   hA[140] = h = new TH1F("muclustPtBalnoE", Form("Barrel: sPT Balance > %.1f (EEMC not included); 2x2 Cluster ET", par_ptBalance), 100, 0, 100);

   // Histograms added for background subtraction and systematic
   char str[200];
   for (int i = 0; i <= 20; i++) {
      sprintf(str, "neg_failAwaySide_Awayside_pt_bin_%d", i);
      hA[142 + i] = new TH2F(str, str, 100, 0, 100, 21, 0, 21);
   }

   for (int i = 0; i <= 20; i++) {
      sprintf(str, "pos_failAwaySide_Awayside_pt_bin_%d", i);
      hA[163 + i] = new TH2F(str, str, 100, 0, 100, 21, 0, 21);
   }

   hA[184 + 2] = new TH1F("pos_muclustpTbal_wE",   "Barrel: pos_muclustpTbal_wE", 100, 0, 100);
   hA[184 + 1] = new TH1F("neg_muclustpTbal_wE",   "Barrel: neg_muclustpTbal_wE", 100, 0, 100);
   hA[184 + 4] = new TH1F("pos_muclustpTbal_noE",  "Barrel: pos_muclustpTbal_noE", 100, 0, 100);
   hA[184 + 3] = new TH1F("neg_muclustpTbal_noE",  "Barrel: neg_muclustpTbal_noE", 100, 0, 100);
   hA[184 + 6] = new TH1F("pos_muclustpTbal_back", "Barrel: pos_muclustpTbal_back", 100, 0, 100);
   hA[184 + 5] = new TH1F("neg_muclustpTbal_back", "Barrel: neg_muclustpTbal_back", 100, 0, 100);

   hA[190] = h = new TH2F("muclustEt_etaWp", "Barrel W+: 2x2 Cluster ET vs. lepton eta, final selection; lepton eta in LAB; lepton 2x2 Cluster ET (GeV)", 32, -2., 2., 60, 0., 60.);
   hA[191] = h = new TH2F("muclustEt_etaWm", "Barrel W-: 2x2 Cluster ET vs. lepton eta, final selection; lepton eta in LAB; lepton 2x2 Cluster ET (GeV)", 32, -2., 2., 60, 0., 60.);

   hA[195] = h = new TH1F("muEtaLT0_maxTowADC", "max tower ADC for eta < 0", 4096, 0, 4096);
   hA[196] = h = new TH1F("muEtaGT0_maxTowADC", "max tower ADC for eta > 0", 4096, 0, 4096);
   hA[197] = h = new TH2F("muTr2D1pt10", "lastHit on track (pt > 10); detector eta ; detector phi (rad)", 100, -1.1, 1.1, 240, -M_PI, M_PI);
   hA[198] = h = new TH2F("muTrPt_eta", "Track PT vs lastHit eta; detector eta ; prim track pt (GeV)", 100, -1.1, 1.1, 100, 0, 100);
   hA[199] = h = new TH2F("muBdist5", "Barrel: 3D Distance(track-cluster) vs. cluster eta; cluster eta; | distance | (cm)", 100, -1., 1., 50, 0, 25);
   hA[200] = h = new TH2F("muWClustET_eta", "W: 2x2 cluster ET vs. cluster eta; cluster detector eta; 2x2 cluster ET", 100, -1., 1., 60, 0, 60);
   hA[201] = h = new TH2F("muWTrPT_eta", "W: Track PT vs. cluster eta; cluster detector eta; Track PT", 100, -1., 1., 100, 0, 100);
   hA[202] = h = new TH2F("muBTrPT_eta", "Background: Track PT vs. cluster eta; cluster detector eta; Track PT", 100, -1., 1., 100, 0, 100);
   hA[203] = h = new TH2F("muWE2P_eta", "W: E/P vs. cluster eta; cluster detector eta; E/P", 100, -1., 1., 15, 0, 3);
   hA[204] = h = new TH2F("muBE2P_eta", "Background: E/P vs. cluster eta; cluster detector eta; E/P", 100, -1., 1., 15, 0, 3);
   hA[205] = h = new TH2F("muWTr2D1", "lastHit on track (W candidate); detector eta ; detector phi (rad)", 100, -1.1, 1.1, 240, -M_PI, M_PI);
   hA[206] = h = new TH2F("muWClustET_eta_clgt14", "2x2 cluster ET vs. cluster eta (Cluster ET > 14); cluster detector eta; 2x2 cluster ET", 100, -1., 1., 60, 0, 60);
   hA[207] = h = new TH2F("muWClustET_eta_towerIso", "2x2 cluster ET vs. cluster eta (2x2/4x4 frac); cluster detector eta; 2x2 cluster ET", 100, -1., 1., 60, 0, 60);
   hA[208] = h = new TH2F("muWClustET_eta_delR", "2x2 cluster ET vs. cluster eta (track-clust match); cluster detector eta; 2x2 cluster ET", 100, -1., 1., 60, 0, 60);
   hA[209] = h = new TH2F("muWClustET_eta_nearIso", "2x2 cluster ET vs. cluster eta (2x2/near frac); cluster detector eta; 2x2 cluster ET", 100, -1., 1., 60, 0, 60);

   //check ETOW near cone sum
   hA[210] = h = new TH2F("muETOWnearET_eta_tr2cl", "ETOW near cone ET vs. cluster eta; cluster detector eta; ETOW near ET", 100, -1., 1., 20, 0, 20);
   hA[211] = h = new TH2F("muETOWnearET_phi_etaGT0_tr2cl", "ETOW near cone ET vs. cluster phi (cluster eta > 0); cluster detector phi; ETOW near ET", 240, -M_PI, M_PI, 20, 0, 20);
   hA[212] = h = new TH2F("muETOWnearET_phi_etaLT0_tr2cl", "ETOW near cone ET vs. cluster phi (cluster eta < 0); cluster detector phi; ETOW near ET", 240, -M_PI, M_PI, 20, 0, 20);

   //hit towers for beam background tests
   string bx7name[4] = {"[0,30]", "[30,40]", "[40,110]", "[110,120]"};
   for (int i = 0; i < 4; i++)
   {
      hA[215 + i] = h = new TH2F(Form("muBTOW_adcGT10goodVer_7bx%d", i), Form("BTOW tower ADC > 10 (after good vertex found) bx7=%s; detector eta; detector phi", bx7name[i].data()), 40, -1, 1, 120, -M_PI, M_PI);
      hA[219 + i] = h = new TH2F(Form("muBTOW_etGT2goodVer_7bx%d", i), Form("BTOW tower ET > 2.0 GeV (after good vertex found) bx7=%s; detector eta; detector phi", bx7name[i].data()), 40, -1, 1, 120, -M_PI, M_PI);
      hA[223 + i] = h = new TH2F(Form("muBTOW_adcGT10noVer_7bx%d", i), Form("BTOW tower ADC > 10 (no rank>0 vertex found) bx7=%s; detector eta; detector phi", bx7name[i].data()), 40, -1, 1, 120, -M_PI, M_PI);
      hA[227 + i] = h = new TH2F(Form("muETOW_adcGT10goodVer_7bx%d", i), Form("ETOW tower ADC > 10 (after good vertex found) bx7=%s; detector eta bin; detector phi bin", bx7name[i].data()), 12, 0, 12, 60, 0, 60);
      hA[231 + i] = h = new TH2F(Form("muETOW_etGT2goodVer_7bx%d", i), Form("ETOW tower ET > 2.0 GeV (after good vertex found) bx7=%s; detector eta bin; detector phi bin", bx7name[i].data()), 12, 0, 12, 60, 0, 60);
      hA[235 + i] = h = new TH2F(Form("muETOW_adcGT10noVer_7bx%d", i), Form("ETOW tower ADC > 10 (no rank>0 vertex found) bx7=%s; detector eta bin; detector phi bin", bx7name[i].data()), 12, 0, 12, 60, 0, 60);
   }

   hA[240] = h = new TH2F("muclustEt_etaFinal_noE", "Barrel: 2x2 Cluster ET vs. lepton eta, final selection (no EEMC in veto); lepton eta in LAB; lepton 2x2 Cluster ET (GeV)", 32, -2., 2., 60, 0., 60.);
   hA[241] = h = new TH2F("muclustEt_etaFinal", "Barrel: 2x2 Cluster ET vs. lepton eta, final selection; lepton eta in LAB; lepton 2x2 Cluster ET (GeV)", 32, -2., 2., 60, 0., 60.);

   hA[250] = h = new TH2F("muBclEjetE2D_ET", "ratio (2x2/nearCone) ET vs. 2x2 cluster ET; 2x2 cluster ET (GeV); ET(cone-2x2) (GeV)", 100, 0, 100, 100, 0, 1.2);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, par_nearTotEtFrac, 1.e6, par_nearTotEtFrac);
   ln->SetLineColor(kRed);
   Lx->Add(ln);

   hA[251] = h = new TH2F("muSpTbal_isoConePos", "Q+ Signed p_{T} Balance vs. 2x2/nearCone; 2x2/nearCone; Signed p_{T} Balance (GeV)", 110, 0, 1.1, 100, -100, 100);
   hA[252] = h = new TH2F("muSpTbal_isoConeNeg", "Q- Signed p_{T} Balance vs. 2x2/nearCone; 2x2/nearCone; Signed p_{T} Balance (GeV)", 110, 0, 1.1, 100, -100, 100);

   // add histos to the list (if provided)
   for (int i = 0; i < mxHA; i++) {
      if (  hA[i] == 0) continue;
      HList->Add( hA[i]);
   }

   //HList->ls();
   LOG_INFO << Form("%s::InitHistos done", GetName()) << endm;
}


/**
 * (Deprecated) Creates root histograms for the endcap calorimeter.
 */
void StVecBosMaker::InitEndcapHistos()
{
   const float PI = TMath::Pi();

   // data histograms
   memset(hE, 0, sizeof(hE));

   TList *Lx;
   TLine *ln;
   TH1   *h;
   char   txt[1000];
   char   txt0[100];
   int    nCase = 16;

   hE[0] = h = new TH1F("muEStatEve", "Endcap W-algo: event count", nCase, 0, nCase);
   h->GetXaxis()->SetTitleOffset(0.4);  h->GetXaxis()->SetLabelSize(0.06);  h->GetXaxis()->SetTitleSize(0.05); h->SetMinimum(0.8);
   h->SetLineColor(kBlue); h->SetLineWidth(2);
   h->SetMarkerSize(2);//<-- large text

   char key[][200] = {"inp", "L2ewId", "L2ewBits", "L2ewET", "L2ewRnd", "tpcOn", "primVert", "vertZ", "Pt10",
                      "E-in", "E200", "TrE", "Tr2Cl", "noNear", "noAway", "goldW" };

   for (int i = 0; i < 15; i++) h->Fill(key[i], 0.); // preset the order of keys

   hE[1] = h = new TH1F("muEInTrg", "mu Endcap W input triggers, WARN: scrambled if manyruns are combined by hadd.C; trigID (random order)", nCase, 0, nCase);
   h->GetXaxis()->SetLabelSize(0.06);

   hE[2] = h = new TH1F("muEbX48", "L2WE-ET events vs. bXing; bXing= raw bx48", 128, -0.5, 127.5);
   h->SetFillColor(kGreen);

   hE[3] = h = new TH1F("muEbX7", "L2WE-ET events vs. bXing; bXing= raw bx7", 128, -0.5, 127.5);
   h->SetFillColor(kBlue);

   hE[4] = new TH1F("muEbX48v", "L2WE-ET & primVertex  vs. bXing; bXing= raw bx48", 128, -0.5, 127.5);
   hE[5] = h = new TH1F("muEbX7v", "L2WE-ET & primVertex; bXing= raw bx7", 128, -0.5, 127.5);
   h->SetFillColor(kBlue);

   //... DMS data
   hE[6] = h = new TH1F("muEDsm1", "L2WE-ET events DMS spectrum; DSM value", 64, -0.5, 63.5);
   h->SetMinimum(0.8);
   hE[7] = h = new TH1F("muEDsm2", "L2WE-Rnd events DMS spectrum; DSM value", 64, -0.5, 63.5);
   h->SetMinimum(0.8);
   sprintf(txt, "L2WE-ET events w/ DMS>%d vs.ETOW TP ID bXing; Hanks' TP ID", parE_DsmThres);
   hE[8] = new TH1F("muEDsm3", txt, 90, -0.5, 89.5);
   sprintf(txt, "L2WE-ET events w/ DMS>%d & primVertexvs.ETOW TP ID bXing; Hanks' TP ID", parE_DsmThres);
   hE[9] = h = new TH1F("muEDsm4", txt, 90, -0.5, 89.5);
   h->SetFillColor(kBlue); h->SetLineColor(kBlue);

   // vertex histograms
   hE[10] = h = new TH1F("muEVRf", "L2WE: PPV Vertex rank, funny X-axis; X=Log10(rank)+offset", 150, -9, 25);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, 0, 0, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hE[11] = h = new TH1F("muEZv", "L2WE: Z of any vertex w/ rank>0;Z-vertex (cm)", 100, -200, 200);
   Lx = h->GetListOfFunctions();
   ln = new TLine(mCutVertexZ, 0, mCutVertexZ, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);
   ln = new TLine(-mCutVertexZ, 0, -mCutVertexZ, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hE[12] = new TH1F("muENV", "L2WE: # vertices per event, (rank>0 or matched track to ETOW) && Z in range; # of vertices", 10, 0, 10);

   // intended for QCD MC
   hE[13] = h = new TH2F("muEtowMaxTow_Zv", "L2WE: ETOW max tower ET vs Z of any vertex w/ rank>0;Z-vertex (cm); max ETOW tower ET", 100, -100, 100, 100, 0, 100);

   //..... Tracks....
   hE[20] = h = new TH1F("muEStatTrk", "Endcap W-algo: track  count; cases", nCase, 0, nCase);
   h->GetXaxis()->SetTitleOffset(0.4);  h->GetXaxis()->SetLabelSize(0.06);  h->GetXaxis()->SetTitleSize(0.05); h->SetMinimum(0.8);
   h->SetLineColor(kGreen); h->SetLineWidth(2);
   char keyT[][200] = {"flag", "pt1", "#eta>0.7", "nHit", "Hfrac", "Rin", "Rout", "ptOK", "@E", "CL", "fr24",
                       "#Delta R", "noNear", "noAway", "goldW"
                      };

   for (int i = 0; i < 15; i++) h->Fill(keyT[i], 0.); // preset the order of keys


   hE[21] = h = new TH1F("muETrNfit", "Endcap: primary track  in-selection & vertexZ; nFitPoints", 50, 0, 50);
   Lx = h->GetListOfFunctions();
   ln = new TLine(parE_nFitPts, 0, parE_nFitPts, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hE[22] = h = new TH1F("muETrFitFrac", "Endcap: primary track in-selection & vertexZ; nFit/nPoss ", 50, 0, 1.1);
   Lx = h->GetListOfFunctions();
   Lx = h->GetListOfFunctions();
   ln = new TLine(parE_nHitFrac, 0, parE_nHitFrac, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hE[23] = h = new TH1F("muETrRxyIn", "Endcap: primary track first hit  in-selection & vertexZ; Rxy (cm)", 60, 50, 170.);
   Lx = h->GetListOfFunctions();
   ln = new TLine(parE_trackRin, 0, parE_trackRin, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hE[24] = h = new TH1F("muETrRxyOut", "Endcap: primary track last hit  in-selection & vertexZ; Rxy (cm)", 80, 60, 220.);
   Lx = h->GetListOfFunctions();
   ln = new TLine(parE_trackRout, 0, parE_trackRout, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hE[25] = h = new TH1F("muETrPt1", "Endcap: global track PT ; track PT (GeV/c)", 80, 0, 80);
   Lx = h->GetListOfFunctions();
   ln = new TLine(mMinETrackPt, 0, mMinETrackPt, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln); h->SetFillColor(kYellow);


   hE[26] = h = new TH2F("muETr2D1", "Endcap: lastHit on track; detector eta ; detector phi (rad)", 100, -0.2, 1.8, 200, -3.2, 3.2);

   hE[27] = h = new TH1F("muETrPt1N", "Endcap: global NEGATIVE track PT; track PT (GeV/c)", 80, 0, 80);
   h->SetFillColor(7);

   hE[28] = h = new TH2F("muETrdEdX", "Endcap: dEdX vs. momentum; track P (GeV); dE/dx (keV)", 20, 0, 10, 100, 0, 10);


   hE[29] = h = new TH1F("muETrPt1Pr", "Endcap: primary track PT; track PT (GeV/c)", 80, 0, 80);
   hE[30] = h = new TH1F("muETrPt1NPr", "Endcap: primary NEGATIVE track PT; track PT (GeV/c)", 80, 0, 80);


   //..... ETOW .....

   hE[31] = h = new TH1F("muEmaxAdc", "Endcap: ETOW maxADC in event, in-selection; max tower ADC", 200, 0, 4200);
   hE[32] = h = new TH1F("muEtotAdc", "Endcap: ETOW sum of ADC>thres , in-selection;ADC sum/event", 80, 0, 8000.);

   hE[33] = h = new TH1F("muEclET", "matched ETOW 2x2 cluster ET  ;cluster  ET (GeV)", 100, 0, 100);
   Lx = h->GetListOfFunctions();
   ln = new TLine(parE_clustET, 0, parE_clustET, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hE[34] = h = new TH2F("muEclAdcPt", "Endcap: matched  TPC PT vs. ETOW 2x2 cluster ADC sum ; cluster (ADC sum);TPC  PT (GeV)", 50, 0, 5000, 75, 0, 150);
   h->GetXaxis()->SetNdivisions(4);

   hE[35] = new TH1F("muETrch2", "Endcap: track glob chi2/dof X-Y", 100, 0, 5);
   hE[36] = new TH2F("muETrch2b", "Endcap: track glob chi2/dof; chi2/dof  X-Y; last hit eta", 30, 0, 5., 30, -0.2, 1.8);


   // 4x4 cluster
   hE[37] = h = new TH1F("muEclET24", "matched ETOW 4x4 cluster ET ;cluster 4x4  ET (GeV)", 100, 0, 100);
   hE[38] = h = new TH2F("muEclE242D", "Endcap: Excess energy in ETOW 4x4 cluster vs. 2x2 cluster E;2x2 cluster E (GeV); E(4x4)-E(2x2)  E (GeV)", 60, 0, 120, 30, 0, 30);

   hE[39] = h = new TH1F("muEclET24R", "Endcap: ratio (2x2/4x4) cluster ET ; fraction: cluster ET 2x2/ 4x4 ET", 100, 0, 1.2);
   Lx = h->GetListOfFunctions();
   ln = new TLine(mMinEClusterEnergyIsoRatio, 0, mMinEClusterEnergyIsoRatio, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   // jet energy , fraction

   hE[40] = h = new TH1F("muEEMCjetET", "Endcap: near 'EM jet' ET ; 'EM jet'  ET (GeV)", 100, 0, 100);

   hE[41] = h = new TH2F("muEclEMCjetE2D", "Endcap: Excess nearCone ET  vs. 2x2 E;2x2 cluster ET (GeV); ET(cone-2x2) (GeV)", 50, 0, 80, 50, 0, 60);

   hE[42] = h = new TH1F("muETEMCjetETR", "Endcap: ratio (2x2/nearCone) ET ; cluster ET/ near cone ET", 100, 0, 1.2);
   Lx = h->GetListOfFunctions();
   ln = new TLine(parE_nearTotEtFrac, 0, parE_nearTotEtFrac, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);


   // .... track-EMC distance cuts
   hE[43] = h = new TH2F("muEdist1", "Endcap: X-Y Distance(track-ETOW cluster) vs. 2x2 E;2x2 cluster E (GeV); | distance | (cm)", 40, 0, 120, 40, 0, 25);
   hE[44] = h = new TH2F("muEdist2", "Endcap: R#Delta #phi   (track-ETOW cluster) vs.#phi-clust;  .#phi-clust(rad) ;R#Delta #phi (cm)", 100, -3.2, 3.2, 40, -20, 20);
   hE[45] = h = new TH2F("muEdist3", "Endcap: R#Delta #phi   (track-ETOW cluster) vs. 2x2 E;2x2 cluster E (GeV); R#Delta #phi (cm)", 40, 0, 120, 40, -20, 20);
   hE[46] = h = new TH1F("muEdist4", "Endcap: X-Y Distance(track-ETOW cluster) for 2x2 E;| X-Y distance |   (cm)", 100, 0, 50);
   Lx = h->GetListOfFunctions();
   ln = new TLine(parE_delR3D, 0, parE_delR3D, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   //.. continue same side jet veto
   hE[47] = h = new TH1F("muETjetET", "Endcap: quenched near 'TPC jet' PT ; 'TPC jet'  PT (GeV)", 100, 0, 100);
   hE[48] = h = new TH2F("muETjetEMCjet2D", "Endcap: quenched  near 'TPC jet' vs.  near 'EMC jet' ; EMC jet ET (GeV); TPC PT  (GeV/c)", 50, 0, 80, 50, 0, 60);
   hE[49] = h = new TH1F("muETEMCjetET", "Endcap: near 'TPC+EMC jet' ET ; jet ET (GeV), no double counting", 100, 0, 100);

   //.. away-side jet  veto
   hE[50] = h = new TH1F("muETwayET", "Endcap: TPC away-cone PT sum;   PT (GeV)", 100, 0, 100);
   hE[51] = h = new TH1F("muEBwayET", "Endcap: BTOW away-cone ET sum;   ET (GeV)", 100, 0, 100);
   hE[52] = h = new TH2F("muETotwayET2D", "Endcap: away TPC+EMC ET sum  vs. 2x2 ETOW cluster ET;  2x2 ET (GeV); away ET (GeV)", 50, 0, 100, 150, 0, 100);
   hE[53] = h = new TH2F("muEAwayET2Db", "Endcap: away EMC ET sum  vs. 2x2 ETOW cluster ET;  2x2 ET (GeV); away EMC ET (GeV)", 50, 0, 100, 80, 0, 80);
   hE[54] = h = new TH1F("muEAwayTotEt", "Endcap: away-cone TPC+EMC ET sum ; away ET (GeV)", 200, 0, 100);

   hE[55] = h = new TH1F("muEEwayET", "Endcap: ETOW away-cone ET sum;   ET (GeV)", 100, 0, 100); // away side energy

   hE[57] = h = new TH2F("muETr2D1pt5", "Endcap: lastHit on track (pt > 5); detector eta ; detector phi (rad)", 100, -0.2, 1.8, 240, -PI, PI);

   //... final golden plots ....

   hE[60] = h = new TH2F("muEEclETPt", "Endcap: Awayside TPC PT vs.  isolated ETOW 2x2 cluster ET, matched;2x2 cluster ET (GeV) ; Awayside TPC PT (GeV)", 50, 0, 100, 75, 0, 150);

   hE[61] = new TH1F("muEbX7eht", "L2W-EHT-rnd  events vs. bXing; bXing= raw bx7", 128, -0.5, 127.5); // filled on input
   hE[62] = h = new TH2F("muEEne_Deta", "Endcap W: ETOW 2x2 cluster energy vs. detector eta, final selection; endcap eta bin; 2x2 Energy (GeV)", 12, 0, 12, 50, 0, 100);

   hE[69] = h = new TH2F("muEeXY", "Endcap W: Projected track XY at SMD depth;  X (cm); Y (cm)", 100, -280, 280, 100, -280, 280);

   //.... final Ws
   hE[90] = h = new TH1F("muE_WET", "Endcap W: Final selection; 2x2 ETOW cluster ET (GeV)", 100, 0, 100);
   Lx = h->GetListOfFunctions();
   ln = new TLine(parE_highET, 0, parE_highET, 1.e6);  ln->SetLineColor(kRed);  Lx->Add(ln);

   sprintf(txt, "Endcap W: Final selection, ET>%.0f GeV 'goldenW'; detector eta ; detector phi (rad)", parE_highET);
   hE[91] = new TH2F("muE_W2D1", txt, 15, 0.5, 2.0, 12, -PI, PI);

   hE[92] = new TH2F("muE_Wdedx", "Endcap W: Track dEdx, final W selection; 2x2 ET (GeV); dEdx (keV)", 100, 0, 100, 100, 0, 10);
   hE[93] = new TH2F("muE_WglDca", "Endcap W: Track glob vertex abs(DCA), final selection ; 2x2 ET (GeV); |DCA| (cm)", 100, 0, 100, 100, 0, 5);
   hE[94] = new TH2F("muE_WglDcaSP", "Endcap W: Track prim POSITIVE glob signed DCA, final selection; 2x2 ET (GeV); sDCA (cm)", 100, 0, 100, 100, -5, 5);
   hE[95] = new TH2F("muE_WglDcaSN", "Endcap W: Track prim NEGATIVE glob signed DCA, final selection ; 2x2 ET (GeV); sDCA (cm)", 100, 0, 100, 100, -5, 5);

   sprintf(txt, "Endcap W: Vertex ID, final selection, 2x2 ET>%.0f GeV; vertex ID", parE_highET);
   hE[96] = h = new TH1F("muE_Wcar1", txt, 10, -0.5, 9.5);
   h->SetLineColor(kBlue); h->SetLineWidth(3);

   sprintf(txt, "Endcap W: Vertex 'funny' rank, final selection, 2x2 ET>%.0f GeV;  X=Log10(rank)+offset", parE_highET);
   hE[97] = new TH1F("muE_Wcar2", txt, 150, -9, 25);

   sprintf(txt, "Vertex Z ,Endcap W: Vertex Z, final selection 2x2 ET>%.0f GeV; Z(cm)", parE_highET);
   hE[98] = new TH1F("muE_Wcar3", txt, 100, -200, 200);
   hE[99] = h = new TH1F("muE_Weta", "Endcap W: Lepton eta, final selection ; lepton eta", 400, -2.0, 2.0);
   hE[100] = h = new TH2F("muE_WXY", "Endcap W: Projected track XY at SMD depth, final selection; X (cm); Y (cm)", 100, -280, 280, 100, -280, 280);

   // free 101-116
   //..... series of electron ET plots after succesive cuts
   char tt2[][200] = {"max 2x2", "track matched", "no near ET", "no away ET"};
   for (int i = 0; i < 4; i++) {
      sprintf(txt, "Endcap electron candidate, cut=%s; 2x2 ET (GeV)", tt2[i]);
      sprintf(txt0, "muE_ETlive%d", i);
      hE[110 + i] = h = new TH1F(txt0, txt, 100, 0, 100);
   }

   hE[132] = h = new TH2F("muEptBalance_clust", "Endcap: ptBalance vs cluster ET; 2x2 Cluster ET; ptBalance", 100, 0, 100, 100, 0, 100);
   hE[133] = h = new TH2F("muEptBalance_awayTot", "ptBalance vs awayside PT; awayside PT; ptBalance", 100, 0, 100, 100, 0, 100);

   hE[134] = h = new TH2F("muEsPtBalance_clust", "Endcap: sPtBalance vs cluster ET; 2x2 Cluster ET (GeV); signed Pt balance (GeV)", 100, 0, 100, 100, -100, 100);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, parE_ptBalance, 100, parE_ptBalance);  ln->SetLineColor(kRed);  Lx->Add(ln);

   hE[135] = h = new TH2F("muEsPtBalance_awayTot", "Endcap: sPtBalance vs awayside PT; awayside PT; sPtBalance", 100, 0, 100, 100, -100, 100);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, parE_ptBalance, 100, parE_ptBalance);  ln->SetLineColor(kRed);  Lx->Add(ln);


   hE[136] = h = new TH1F("muEclustPtBal", Form("Endcap: PT Balance > %.1f ; 2x2 Cluster ET", parE_ptBalance), 100, 0, 100);
   hE[137] = h = new TH1F("muEclustPtBal_bckgrd", Form("Endcap: PT Balance < %.1f ; 2x2 Cluster ET", parE_ptBalance), 100, 0, 100);
   hE[140] = h = new TH1F("muEclustPtBalnoE", Form("Endcap: sPT Balance > %.1f (EEMC not included); 2x2 Cluster ET", parE_ptBalance), 100, 0, 100);

   hE[184 + 2] = new TH1F("Epos_muEclustpTbal_wE", "Endcap: pos_muEclustpTbal_wE", 100, 0, 100);
   hE[184 + 1] = new TH1F("Eneg_muEclustpTbal_wE", "Endcap: neg_muEclustpTbal_wE", 100, 0, 100);
   hE[184 + 4] = new TH1F("Epos_muEclustpTbal_noE", "Endcap: pos_muEclustpTbal_noE", 100, 0, 100);
   hE[184 + 3] = new TH1F("neg_muEclustpTbal_noE", "Endcap: neg_muEclustpTbal_noE", 100, 0, 100);
   hE[184 + 6] = new TH1F("Epos_muEclustpTbal_back", "Endcap: pos_muEclustpTbal_back", 100, 0, 100);
   hE[184 + 5] = new TH1F("Eneg_muEclustpTbal_back", "Endcap: neg_muEclustpTbal_back", 100, 0, 100);

   hE[190] = h = new TH2F("muEclustEt_etaWp", "Endcap Wp: 2x2 Cluster ET vs. lepton eta, final selection; lepton eta in LAB; lepton 2x2 Cluster ET (GeV)", 32, -2., 2., 60, 0., 60.);
   hE[191] = h = new TH2F("muEclustEt_etaWm", "Endcap Wm: 2x2 Cluster ET vs. lepton eta, final selection; lepton eta in LAB; lepton 2x2 Cluster ET (GeV)", 32, -2., 2., 60, 0., 60.);

   sprintf(txt, "Endcap: TPC GLOB Q/PT  ; ETOW 2x2 cluster ET (GeV); Q/PT");
   hE[200] = h = new TH2F("muEchRecPNg", txt, 100, 0., 100., 100, -0.1, 0.1);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, 0, 100, 0);  ln->SetLineColor(kMagenta);  Lx->Add(ln);

   sprintf(txt, "Endcap: TPC PRIM  Q/PT ; ETOW 2x2 cluster ET (GeV); Q/PT");
   hE[201] = h = new TH2F("muEchRecPNp", txt, 100, 0., 100., 100, -0.1, 0.1);
   Lx = h->GetListOfFunctions();
   ln = new TLine(0, 0, 100, 0);  ln->SetLineColor(kMagenta);  Lx->Add(ln);

   // add histos to the list (if provided)
   for (int i = 0; i < mxHE; i++) {
      if (  hE[i] == 0) continue;
      HList->Add( hE[i]);
   }

   //  HList->ls();
   LOG_INFO << Form("%s::InitEndcapHistos done", GetName()) << endm;
}
