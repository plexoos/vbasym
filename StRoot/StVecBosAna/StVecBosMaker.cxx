#include <limits>
#include <math.h>

#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TString.h>
#include <StMessMgr.h>
#include <StThreeVectorF.hh>

//MuDst
#include <StMuDSTMaker/COMMON/StMuDstMaker.h>
#include <StMuDSTMaker/COMMON/StMuDst.h>
#include <StMuDSTMaker/COMMON/StMuEvent.h>

#include "StEmcUtil/database/StBemcTables.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include "StEEmcUtil/StEEmcSmd/EEmcSmdGeom.h"

#include "StEmcRawMaker/defines.h"
#include <StMuDSTMaker/COMMON/StMuPrimaryVertex.h>

//jets
#include "StSpinPool/StJets/StJet.h"
#include "StSpinPool/StJets/StJets.h"
#include "StJetMaker/StJetMaker.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
#include "StJetMaker/StJetReader.h"
#include "StJetMaker/StJetSkimEventMaker.h"

#include "WeventDisplay.h"
#include "StVecBosMaker.h"

ClassImp(StVecBosMaker)


/** */
StVecBosMaker::StVecBosMaker(const char *name): StMaker(name),
   mStMuDstMaker(0),
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

   nInpEve = nTrigEve = nAccEve = 0;

   // MC trigger simulator
   par_l0emulAdcThresh     = 30;
   par_l2emulSeedThresh    = 5.0;
   par_l2emulClusterThresh = 12.0;

   // vertex
   par_minPileupVert       = 3;    // to reject events w/o TPC, lower it for MC
   par_vertexZ             = 100;  // (cm)

   // towers
   par_kSigPed             = 3;    // rawADC-ped cut off
   par_AdcThres            = 8;    // ADC threshold to avoid correlated noise
   par_maxADC              = 200.; // (adc chan) on the highest tower in events

   //... Barrel Algo
   par_clustET             = 14.;  // (GeV/c) 2x2 cluster ET
   par_clustFrac24         = 0.95; // ET ratio 2x2/4x4 cluster
   par_nearTotEtFrac       = 0.88; // ratio 2x2/near Tot ET
   par_delR3D              = 7.;   // cm, dist between projected track and center of cluster
   par_leptonEtaLow        = -1.5; // bracket acceptance
   par_leptonEtaHigh       = 1.5;  // bracket acceptance
   par_ptBalance           = 14.;  // (GeV), ele cluster vector + jet sum vector
   //... track
   par_nFitPts             = 15;   // hits on the track
   par_nHitFrac            = 0.51;
   par_trackRin            = 90;   // cm
   par_trackRout           = 160;  // cm
   par_trackPt             = 10.;  // GeV
   par_highET              = 25.;  // (GeV), cut-off for final Barrel W-cluster

   //... Endcap Algo
   parE_trackEtaMin        = 0.7;  // avoid bad extrapolation to ESMD
   parE_clustET            = 14.;  // (GeV/c) 2x1 cluster ET
   parE_clustFrac24        = 0.90; // ET ratio 2x2/4x4 cluster
   parE_nearTotEtFrac      = 0.85; // ratio 2x2/near Tot ET
   parE_delR3D             = 10.;  // cm, dist between projected track and center of cluster
   parE_leptonEtaLow       = 0.7;  // bracket acceptance
   parE_leptonEtaHigh      = 2.5;  // bracket acceptance
   parE_ptBalance          = 14.;  // (GeV), ele cluster vector + jet sum vector
   //... track
   parE_nFitPts            = 5;    // hits on the track
   parE_nHitFrac           = 0.51;
   parE_trackRin           = 120;
   parE_trackRout          = 70;   // cm
   parE_trackPt            = 7.;   // GeV
   parE_nSmdStrip          = 20;
   parE_highET             = 25.;  // (GeV), cut-off for final Endcap W-cluster

   //... search for W's
   par_nearDeltaR          = 0.7;  // (~rad) near-cone size
   par_awayDeltaPhi        = 0.7;  // (rad) away-'cone' size

   mRunNo                  = 0;
   nRun                    = 0;
   hbxIdeal                = 0;

   // irrelevant for W analysis
   par_DsmThres            = 31;   // only for monitoring
   parE_DsmThres           = 31;   // only for monitoring
   par_maxDisplEve         = 1;    // # of displayed selected events

   use_gains_file          = 0;
}


Int_t StVecBosMaker::Init()
{
   assert(HList);
   initHistos();
   initEHistos();

   if (mStMuDstMaker) {
      //only need DB tables for MuDst analysis
      mBarrelTables = new StBemcTables();
      mDbE = (StEEmcDb*) GetDataSet("StEEmcDb");
      assert(mDbE);
   }
   else {
      //setup for reading in tree
      mWEvent = new WEvent();
      mTreeChain-> SetBranchAddress("mWEvent", &mWEvent);
   }

   mBtowGeom       = StEmcGeom::instance("bemc");
   mBSmdGeom[kBSE] = StEmcGeom::instance("bsmde");
   mBSmdGeom[kBSP] = StEmcGeom::instance("bsmdp");

   mGeomEmc = new EEmcGeomSimple();
   mGeomSmd = EEmcSmdGeom::instance();
   initGeom();

   wDisaply = new WeventDisplay(this, par_maxDisplEve);

   if (isMC) par_minPileupVert = 1;

   // tree only written during MuDst analysis
   if (mStMuDstMaker) {
      mTreeFile = new TFile(mTreeName, "recreate");
      mTreeFile->cd();

      mWEvent = new WEvent();
      mWtree  = new TTree("mWtree", "W candidate Events");
      mWtree->Branch("mWEvent", "WEvent", &mWEvent);
   }

   return StMaker::Init();
}


/** */
Int_t StVecBosMaker::InitRun(int runNo)
{
   LOG_INFO << Form("::InitRun(%d) start", runNo) << endm;

   //if (!isMC) assert(mRunNo == 0); // to prevent run merging - it was not tested

   if (mStMuDstMaker) {
      mBarrelTables->loadTables(this);
      mRunNo = runNo;
   }
   else {
      mRunNo = mWEvent->runNo;
   }

   // barrel algo params
   LOG_INFO << Form("::InitRun(%d) %s done\n" \
      "Barrel W-algo params: trigID L2BW=%d isMC=%d\n" \
      "TPC: nPileupVert>%d, vertex |Z|<%.1fcm, primEleTrack: nFit>%d, hitFrac>%.2f Rin<%.1fcm, Rout>%.1fcm, PT>%.1fGeV/c\n" \
      "BTOW ADC: kSigPed=%d AdcThr>%d maxAdc>%.0f clustET>%.1f GeV  ET2x2/ET4x4>%0.2f  ET2x2/nearTotET>%0.2f\n" \
      "dist(track-clust)<%.1fcm, nearDelR<%.1f\n" \
      "W selection highET>%.1f awayDelPhi<%.1frad  ptBalance>%.1fGeV  %.1f<leptonEta<%.1f ",
      mRunNo, coreTitle.Data(), par_l2bwTrgID, isMC,
      par_minPileupVert, par_vertexZ,
      par_nFitPts, par_nHitFrac,  par_trackRin,  par_trackRout, par_trackPt,
      par_kSigPed, par_AdcThres, par_maxADC, par_clustET, par_clustFrac24, par_nearTotEtFrac,
      par_delR3D, par_nearDeltaR,
      par_highET, par_awayDeltaPhi, par_ptBalance, par_leptonEtaLow, par_leptonEtaHigh
   ) << endm;

   // endcap algo params
   cout << Form("\n" \
      "Endcap W-algo params: trigID: L2EW=%d isMC=%d\n" \
      "TPC: nPileupVert>%d, vertex |Z|<%.1fcm, primEleTrack: nFit>%d, hitFrac>%.2f Rin<%.1fcm, Rout>%.1fcm, PT>%.1fGeV/c\n" \
      "ETOW ADC: kSigPed=%d AdcThr>%d maxAdc>%.0f clustET>%.1f GeV  ET2x1/ET4x4>%0.2f  ET2x1/nearTotET>%0.2f\n" \
      "dist(track-clust)<%.1fcm, nearDelR<%.1f\n" \
      "W selection highET>%.1f awayDelPhi<%.1frad  ptBalance>%.1fGeV ",
      parE_l2ewTrgID, isMC,
      par_minPileupVert, par_vertexZ,
      parE_nFitPts, parE_nHitFrac, parE_trackRin, parE_trackRout, parE_trackPt,
      par_kSigPed, par_AdcThres, par_maxADC, parE_clustET, parE_clustFrac24, parE_nearTotEtFrac,
      parE_delR3D, par_nearDeltaR,
      par_highET, par_awayDeltaPhi, parE_ptBalance
   ) << endl;

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
         mTpcFilter[iSector-1].setCuts(par_nFitPts, par_nHitFrac, Rin, Rout);
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
   mWEvent->clear();
}


//
Int_t StVecBosMaker::Make()
{
   nInpEve++;

   if (mStMuDstMaker) // standard MuDst analysis
   {
      mWEvent->id      = mStMuDstMaker->muDst()->event()->eventId();
      mWEvent->runNo   = mStMuDstMaker->muDst()->event()->runId();
      mWEvent->time    = mStMuDstMaker->muDst()->event()->eventInfo().time();
      mWEvent->zdcRate = mStMuDstMaker->muDst()->event()->runInfo().zdcCoincidenceRate();

      int T = mWEvent->time;

      Tlast  = (Tlast  < T) ? T : Tlast;
      Tfirst = (Tfirst > T) ? T : Tfirst;

      const char *afile = mStMuDstMaker->GetFile();

      if (nInpEve % 200 == 1)
         Info("Make", "muDst nEve: inp=%d trig=%d accpt=%d\n" \
              "daqFile: %s\n", nInpEve, nTrigEve, nAccEve, afile);

      hA[0]->Fill("inp", 1.);
      hE[0]->Fill("inp", 1.);

      int btowStat = ReadBTOWInfo(); // get energy in BTOW
      int etowStat = ReadETOWInfo(); // get energy in ETOW

      int btrig    = ReadBarrelTrigInfo();
      int etrig    = ReadEndcapTrigInfo();

      if ( btrig && etrig )  { mWtree->Fill(); return kStOK; } //skip event w/o valid trig ID

      nTrigEve++;

      if (ReadVertexInfo()) {
         Info("Make", "Found vertex info");
         fillTowHit(false); // fill 2D tower "hit" histos for _no_ vertex and L2BW trigger (beam background analysis, remove any time JS)
         mWtree->Fill();
         return kStOK;      // skip event w/o ~any reasonable vertex
      } else {
         Info("Make", "Not found vertex info");
      }

      // fill 2D tower "hit" histos for vertex found and L2BW trigger (beam background analysis, remove any time JS)
      fillTowHit(true);

      // add plots for QCD normalization
      FillNormHists();

      if (accessTracks()) { mWtree->Fill(); return kStOK; } //skip event w/o ~any highPt track

      accessBSMD(); // get energy in BSMD
      accessESMD(); // get energy in ESMD
      accessEPRS(); // get energy in EPRS

      mWtree->Fill(); //write all events w/ pt>10 track to tree

      if (mWEvent->l2bitET  && mWEvent->bemc.tileIn[0] == 1) hA[0]->Fill("B-in", 1.0);
      if (mWEvent->l2EbitET && mWEvent->etow.etowIn == 1)    hE[0]->Fill("E-in", 1.0);
      if (mWEvent->l2bitET  && !btowStat)                    hA[0]->Fill("B200", 1.0);
      if (mWEvent->l2EbitET && !etowStat)                    hE[0]->Fill("E200", 1.0);

      if ( btowStat && etowStat ) return kStOK; //skip event w/o energy in BTOW && ETOW

      if (mStJetReader) { // just QA plots for jets
         mJets = GetJets(mJetTreeBranch); //get input jet info

         hA[119]->Fill(mNJets); // save jet count in this event

         for (int i_jet = 0; i_jet < mNJets; ++i_jet) {
            StJet *jet     = GetJet(i_jet);
            float  jet_pt  = jet->Pt();
            float  jet_eta = jet->Eta();
            float  jet_phi = jet->Phi();
            hA[117]->Fill(jet_eta, jet_phi);
            hA[118]->Fill(jet_pt);
         }
      }
   }
   else { // analysis of W tree

      if (getEvent(index++, indexJet++) == kStEOF)
         return kStEOF; //get next event from W and jet tree

      //allow for manual scale adjustment of BTOW energy (careful!)
      for (int i = 0; i < 4800; i++)
         mWEvent->bemc.eneTile[0][i] *= mParBTOWScale;

      if (nInpEve % 200 == 1) printf("\n-----in---- %s, W-Tree  nEve: inp=%d \n", GetName(), nInpEve); //,nTrigEve, nAccEve,afile);

      //fill some bins in muStatEve histos for checks
      hA[0]->Fill("inp", 1.);
      hE[0]->Fill("inp", 1.);

      //fill trigger bins for counter histos
      if (mWEvent->l2bitET)   hA[0]->Fill("L2bwET", 1.);
      if (mWEvent->l2bitRnd)  hA[0]->Fill("L2bwRnd", 1.);
      if (mWEvent->l2EbitET)  hE[0]->Fill("L2ewET", 1.);
      if (mWEvent->l2EbitRnd) hE[0]->Fill("L2ewRnd", 1.);

      if (!mWEvent->l2bitET && !mWEvent->l2EbitET) return kStOK; //skip event w/o valid trig ID

      nTrigEve++;

      // fill tpc bins
      int nVerR = 0;
      int nTrOK = 0;

      for (uint iv = 0; iv < mWEvent->vertex.size(); iv++) {
         if (mWEvent->vertex[iv].rank > 0)            nVerR++;
         if (mWEvent->vertex[iv].eleTrack.size() > 0) nTrOK++;
      }

      if (mWEvent->l2bitET  && nVerR > 0) hA[0]->Fill("vertZ", 1.);
      if (mWEvent->l2EbitET && mWEvent->vertex.size() > 0) hE[0]->Fill("vertZ", 1.);
      if (mWEvent->l2bitET  && nTrOK > 0) hA[0]->Fill("Pt10", 1.);
      if (mWEvent->l2EbitET && nTrOK > 0) hE[0]->Fill("Pt10", 1.);

      if (nTrOK <= 0) return kStOK;

      //fill some B/ETOW bins
      if (mWEvent->l2bitET  && mWEvent->bemc.tileIn[0] == 1)      hA[0]->Fill("B-in", 1.0);
      if (mWEvent->l2EbitET && mWEvent->etow.etowIn == 1)         hE[0]->Fill("E-in", 1.0);
      if (mWEvent->l2bitET  && mWEvent->bemc.maxAdc > par_maxADC) hA[0]->Fill("B200", 1.0);
      if (mWEvent->l2EbitET && mWEvent->etow.maxAdc > par_maxADC) hE[0]->Fill("E200", 1.0);

      if (mWEvent->bemc.maxAdc < par_maxADC && mWEvent->etow.maxAdc < par_maxADC) return kStOK; //skip event w/o energy in BTOW && ETOW

      if (mJetTreeChain) { // just QA plots for jets
         mJets = GetJetsTreeAnalysis(mJetTreeBranch); //get input jet info

         for (int i_jet = 0; i_jet < mNJets; ++i_jet) {
            StJet *jet     = GetJet(i_jet);
            float  jet_pt  = jet->Pt();
            float  jet_eta = jet->Eta();
            float  jet_phi = jet->Phi();
            hA[117]->Fill(jet_eta, jet_phi);
            hA[118]->Fill(jet_pt);
         }
      }
   }

   // find barrel candidates
   extendTrack2Barrel();
   int noMatchedBtowCluster = matchTrack2BtowCluster();

   // find endcap candidates
   extendTrack2Endcap();
   int noMatchedEtowCluster = matchTrack2EtowCluster();

   if (noMatchedBtowCluster && noMatchedEtowCluster) return kStOK; //no matched BTOW or ETOW clusters

   nAccEve++;

   // now it starts to get interesting, process every track on the list  till the end
   findNearJet();
   findAwayJet();

   if (mStJetReader || mJetTreeChain) {
      CalcPtBalance();
      //CalcMissingET();

      if (!noMatchedBtowCluster) tag_Z_boson();
   }

   // endcap specific analysis
   if (!noMatchedEtowCluster) {
      analyzeESMD();
      analyzeEPRS();
   }

   if (!noMatchedBtowCluster) find_W_boson();
   if (!noMatchedEtowCluster) findEndcap_W_boson();

   if (nAccEve < 2 || nAccEve % 1000 == 1 ) mWEvent->print(0x0, isMC);

   return kStOK;
}


void StVecBosMaker::initGeom()
{
   //...... BTOW ...........
   memset(mapBtowIJ2ID, 0, sizeof(mapBtowIJ2ID));

   for (int softID = 1; softID <= mxBtow; softID++) {
      // querry BTOW geom
      int m, e, s;
      mBtowGeom->getBin(softID, m, e, s);

      float etaF, phiF;
      mBtowGeom->getEta(m, e, etaF);
      mBtowGeom->getPhi(m, s, phiF); // -pi <= phi < pi

      int iEta, iPhi;
      assert(L2algoEtaPhi2IJ(etaF, phiF, iEta, iPhi) == 0); // tower must be localized at the known position

      int IJ = iEta + iPhi * mxBTetaBin;
      assert(mapBtowIJ2ID[IJ] == 0); // avoid overlaping mapping
      mapBtowIJ2ID[IJ] = softID;

      Float_t x, y, z;
      assert( mBtowGeom->getXYZ(softID, x, y, z) == 0);
      positionBtow[softID - 1] = TVector3(x, y, z);
   } // end of loop over towers

   //...... BSMD-E, -P ...........
   for (int iep = 0; iep < mxBSmd; iep++) {
      for (int softID = 1; softID <= mxBStrips; softID++) {
         Float_t x, y, z;
         assert( mBSmdGeom[iep]->getXYZ(softID, x, y, z) == 0);
         positionBsmd[iep][softID - 1] = TVector3(x, y, z);
      } // end of loop over towers
   }

   //...... ETOW .............
   for (int isec = 0; isec < mxEtowSec; isec++) {
      for (int isub = 0; isub < mxEtowSub; isub++) {
         for (int ieta = 0; ieta < mxEtowEta; ieta++) {
            positionEtow[isec * mxEtowSub + isub][ieta] = mGeomEmc->getTowerCenter(isec, isub, ieta);
         }
      }
   }
}


// returns error code
int StVecBosMaker::L2algoEtaPhi2IJ(float etaF, float phiF, int &iEta, int &iPhi)
{
   if ( phiF < 0) phiF += 2*M_PI; // I want phi in [0,2Pi]

   if (fabs(etaF) >= 0.99) return -1;

   int kEta = 1 + (int)((etaF + 1.) / 0.05);
   iPhi = 24 - (int)( phiF / M_PI * 60.);

   if (iPhi < 0) iPhi += 120;

   // convention:  iPhi=[0,119], kEta=[1,40]
   iEta = kEta - 1;

   //printf("IJ=%d %d\n",iEta,iPhi);
   if (iEta < 0 || iEta >= mxBTetaBin) return -2;
   if (iPhi < 0 || iPhi >= mxBTphiBin) return -3;

   return 0;
}


// intended for normalization of filtered QCD MC
void StVecBosMaker::FillNormHists()
{
   // fill max BTOW clustET vs z-vertex distribution for events with positive
   // rank vertex
   if (mWEvent->l2bitET && mWEvent->vertex.size() > 0 && mWEvent->vertex[0].rank > 0)
   {
      float maxBtowET = 0;

      for (int i = 0; i < mxBtow; i++) {
         if (mWEvent->bemc.statTile[0][i] != 0) continue; //zero means good

         int   ieta = -1;
         int   iphi = -1;
         float etaF = positionBtow[i].Eta();
         float phiF = positionBtow[i].Phi();

         L2algoEtaPhi2IJ(etaF, phiF, ieta, iphi);
         WeveCluster c = maxBtow2x2(ieta, iphi, mWEvent->vertex[0].z);

         if (c.ET > maxBtowET) maxBtowET = c.ET;
      }

      hA[13]->Fill(mWEvent->vertex[0].z, maxBtowET);
   }

   // fill max ETOW towerET vs z-vertex distribution for events with positive
   // rank vertex
   if (mWEvent->l2EbitET && mWEvent->vertex.size() > 0 && mWEvent->vertex[0].rank > 0)
   {
      float maxEtowET = 0;

      for (int isec = 0; isec < mxEtowSec; isec++) {
         for (int isub = 0; isub < mxEtowSub; isub++) {
            for (int ieta = 0; ieta < mxEtowEta; ieta++) {
               if (mWEvent->etow.stat[isec * mxEtowSub + isub][ieta] == 0) {
                  WeveCluster c = sumEtowPatch(ieta, isec * mxEtowSub + isub, 1, 1, mWEvent->vertex[0].z);

                  if (c.ET > maxEtowET) maxEtowET = c.ET;
               }
            }
         }
      }

      hE[13]->Fill(mWEvent->vertex[0].z, maxEtowET);
   }
}


/** */
TClonesArray* StVecBosMaker::GetJets(TString branchName)
{
   if (mStJetReader == 0) {
      mNJets = -1;
      return 0;
   }

   //assert(mStJetReader->getStJets(branchName)->eventId() == mWEvent->id);
   if (mStJetReader->getStJets(branchName)->eventId() != mWEvent->id)
      Error("GetJets", "Jet and W event ids do not match: %12d, %12d", mStJetReader->getStJets(branchName)->eventId(), mWEvent->id);

   //assert(mStJetReader->getStJets(branchName)->runId() == mWEvent->runNo);
   if (mStJetReader->getStJets(branchName)->runId() != mWEvent->runNo)
      Error("GetJets", "Jet and W run ids do not match: %12d, %12d", mStJetReader->getStJets(branchName)->runId(), mWEvent->runNo);

   mNJets = mStJetReader->getStJets(branchName)->nJets();

   return mStJetReader->getStJets(branchName)->jets();
}


// Below is only used for Tree analysis
TClonesArray* StVecBosMaker::GetJetsTreeAnalysis(TString branchName)
{
   if (mJetTreeChain == 0) {
      mNJets = -1;
      return 0;
   }

   //cout<<"looking for matching jet event"<<endl;

   StJets *jetTmp = GetStJetsCopy(branchName);

   while (jetTmp->eventId() != mWEvent->id || jetTmp->runId() != mWEvent->runNo) {
      mJetTreeChain->GetEntry(indexJet++);
      jetTmp = GetStJetsCopy(branchName);
   }

   //cout<<"found matching jet event"<<endl;

   assert(jetTmp->eventId() == mWEvent->id);
   assert(jetTmp->runId() == mWEvent->runNo);
   mNJets = jetTmp->nJets();
   return jetTmp->jets();
}


StJets * StVecBosMaker::GetStJetsCopy(TString branchName)
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
int StVecBosMaker::ReadEndcapTrigInfo()
{
   if (isMC) {
     //// if (mWEvent->etow.maxAdc < 10. / 60.*4096) return -1; //L2 is HT
     ////  hE[0]->Fill("L2ewET", 1.);
     ////  mWEvent->l2EbitET = true;
     ////  return 0;
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
   mWEvent->l2EbitET  = (l2weResult2011->trigger & 2) > 0; // bit1=ET>thr
   mWEvent->l2EbitRnd = (l2weResult2011->trigger & 1) > 0; // bit0=rnd,

#if 0
   if (l2weResult2011->trigger == 0) return -3;
   printf(" L2-jet online results below:\n");
   for (int k = 0; k < 64; k++)
      if (trigL2Chunk[k]) printf("k=%2d  val=0x%04x\n", k, trigL2Chunk[k]);
   printf("L2WE_Result 4-bytes: trg bitET=%d,  bitRnd=%d, highets:  ET/GeV=%.2f,  RDO=%d  hex=0x%08x\n", mWEvent->l2EbitET, mWEvent->l2EbitRnd, l2weResult2011->highestEt / 256.*60, l2weResult2011->highestRDO, trigL2Chunk[EEMCW_offset]);
#endif

   // hack to make the code work also for run 9 and early run 12
   // XXX:ds: What about run 11?
   if (mRunNo < 11000111 || mRunNo > 13000000) {
      mWEvent->l2EbitET  = 1;
      mWEvent->l2EbitRnd = 1;
   }

   if ( (mWEvent->l2EbitRnd || mWEvent->l2EbitET) == 0) return -3; // L2W-algo did not accept this event

   hE[0]->Fill("L2ewBits", 1.); // confirmation bits were set properly

   if (mWEvent->l2EbitRnd) {
      hE[0]->Fill("L2ewRnd", 1.);
      for (int m = 0; m < 90; m++) {
         int val = stMuEvent->emcTriggerDetector().highTowerEndcap(m);
         hE[7]->Fill(val);
      }

      hE[61]->Fill(mWEvent->bx7);
   }

   if (!mWEvent->l2EbitET) return -3; // drop L2W-random accepts
   if ( mWEvent->l2EbitET) hE[0]->Fill("L2ewET", 1.);

   // only monitor below
   hE[2]->Fill(mWEvent->bx48);
   hE[3]->Fill(mWEvent->bx7);

   // access L0-HT data
   int mxVal = -1;
   for (int m = 0; m < 90; m++)  {
      int val = stMuEvent->emcTriggerDetector().highTowerEndcap(m);
      if (mxVal < val) mxVal = val;
      if (mWEvent->l2EbitET) hE[6]->Fill(val);
      if (val < parE_DsmThres) continue;
      if (mWEvent->l2EbitET) hE[8]->Fill(m);
      //printf("Fired L0 EHT m=%d val=%d\n",m,val);
   }

   mWEvent->etow.maxHtDsm = mxVal;
   return 0;
}


int StVecBosMaker::ReadETOWInfo()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();
   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;    return -4;
   }

   mWEvent->etow.etowIn = 1; //tag usable ETOW data
   const char *maxIdName = 0;
   double maxADC = 0, adcSum = 0;
   int maxSec = -1, maxSub = -1, maxEta = -1;

   //loop over all towers
   for (int i = 0; i < emc->getNEndcapTowerADC(); i++) {
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

      mWEvent->etow.adc[isec * mxEtowSub + isub][ieta] = adc;

      if (x->gain <= 0) continue; // drop channels w/o gains
      float ene = adc / x->gain;

      //method for shifting energy scale
      ene *= mParETOWScale; //(default is mParETOWScale=1)
      mWEvent->etow.ene[isec * mxEtowSub + isub][ieta] = ene;
      mWEvent->etow.stat[isec * mxEtowSub + isub][ieta] = 0;

      if (maxADC < adc) { maxIdName = x->name; maxADC = adc; maxSec = isec; maxSub = isub; maxEta = ieta;}
      adcSum += adc;

   }

   mWEvent->etow.maxAdc = maxADC;
   mWEvent->etow.maxSec = maxSec;
   mWEvent->etow.maxSub = maxSub;
   mWEvent->etow.maxEta = maxEta;

   hE[31]->Fill(maxADC);
   hE[32]->Fill(adcSum);

   if (maxADC < par_maxADC)  return -2 ; // not enough energy

   return 0;
}


void StVecBosMaker::accessEPRS()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();
   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;
   }

   int i ;
   int pNh = emc->getNEndcapPrsHits();
   for (i = 0; i < pNh; i++) {
      int pre, sec, eta, sub; //muDst  ranges: sec:1-12, sub:1-5, eta:1-12 ,pre:1-3==>pre1/pre2/post

      StMuEmcHit *hit = emc->getEndcapPrsHit(i, sec, sub, eta, pre);
      float rawAdc = hit->getAdc();
      //Db ranges: sec=1-12,sub=A-E,eta=1-12,type=T,P-R ; slow method

      const EEmcDbItem *x = mDbE->getTile(sec, sub - 1 + 'A', eta, pre - 1 + 'P');
      assert(x); // it should never happened for muDst
      if (x->fail ) continue; // drop not working channels

      int isec = x->sec - 1;
      int isub = x->sub - 'A';
      int ieta = x->eta - 1;
      int ipre = pre - 1;
      int iphi = isec * mxEtowSub + isub;

      assert(isec >= 0 && isec < mxEtowSec); // check input is ok
      assert(isub >= 0 && isub < mxEtowSub);
      assert(ieta >= 0 && ieta < mxEtowEta);

      float adc = rawAdc - x->ped; // ped subtracted ADC
      if (adc < par_kSigPed * x->sigPed) continue;

      mWEvent->eprs.adc[iphi][ieta][ipre] = adc;

      if (x->gain <= 0) continue; // drop channels w/o gains

      mWEvent->eprs.ene[isec * mxEtowSub + isub][ieta][ipre] = adc / x->gain;
      mWEvent->eprs.stat[isec * mxEtowSub + isub][ieta][ipre] = 0;

   }
}


void StVecBosMaker::accessESMD()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      LOG_WARN << "No EMC data for this event" << endm;
   }

   char uv = 'U';
   for (uv = 'U'; uv <= 'V'; uv++) {
      int sec, strip;
      int nh = emc->getNEndcapSmdHits(uv);
      int i;

      for (i = 0; i < nh; i++) {
         StMuEmcHit *hit = emc->getEndcapSmdHit(uv, i, sec, strip);
         float rawAdc = hit->getAdc();
         const EEmcDbItem *x = mDbE->getByStrip(sec, uv, strip);
         assert(x); // it should never happened for muDst

         if (x->fail )   continue; // drop broken channels
         if (x->ped < 0) continue; // drop channels without peds

         float adc    = rawAdc - x->ped; // ped subtracted ADC
         float sigPed = x->sigPed;

         int isec     = sec - 1;
         int iuv      = x->plane - 'U';
         int istr     = x->strip - 1;

         //x->print(); printf("adc=%f\n",adc);

         assert(isec >= 0 && isec < mxEtowSec); //never trust the input
         assert(iuv  >= 0 && iuv  < mxEsmdPlane);
         assert(istr >= 0 && istr < mxEsmdStrip);

         if (x->gain <= 0) continue; // drop channels w/o gains
         if (adc < par_kSigPed * sigPed) continue; //drop noise

         mWEvent->esmd.adc[isec][iuv][istr] = adc;
         mWEvent->esmd.ene[isec][iuv][istr] = adc / x->gain;
      }
   }
}


// returns non-zero on abort
int StVecBosMaker::ReadBarrelTrigInfo()
{
   if (isMC) {
      // When the trigger emulator is ready, this should hook into that
      // instead of the two functions used below.  For now, check that is passes both
      // L0 and L2, and set the l2bitET flag to true if so.

      //if (!passes_L0()) return -1;
      //// if (!passes_L2()) return -2;

      //// hA[0]->Fill("L2bwET", 1.);

      //// mWEvent->l2bitET = true;
      //// return 0; // we haven't set everything, but it should be good enough for simu.
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
      mWEvent->trigAwaySum[i] = awaySum[i];

   mWEvent->trigTotalSum = totalSum;

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
   StL0Trigger *trig = &(stMuEvent->l0Trigger());
   mWEvent->bx48 = trig->bunchCrossingId();
   mWEvent->bx7 = trig->bunchCrossingId7bit(mRunNo);

   // store spin info
   int bxStar48 = -2;
   int bxStar7  = -2;
   int spin4    = -2;

   // all 3 DB records exist
   if (spinDb && spinDb->isValid() && spinDb->isPolDirLong())
   {  // you do not want mix Long & Trans by accident
      bxStar48 = spinDb->BXstarUsingBX48(mWEvent->bx48);
      bxStar7  = spinDb->BXstarUsingBX7(mWEvent->bx7);
      spin4    = spinDb->spin4usingBX48(mWEvent->bx48);
   }

   mWEvent->bxStar48 = bxStar48;
   mWEvent->bxStar7  = bxStar7;
   mWEvent->spin4    = spin4;

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

   mWEvent->l2bitET  = (l2wResult2009->trigger & 2) > 0; // bit1=ET>thr
   mWEvent->l2bitRnd = (l2wResult2009->trigger & 1) > 0; // bit0=rnd

   if ( (mWEvent->l2bitRnd || mWEvent->l2bitET) == 0) return -3; // L2W-algo did not accept this event

   hA[0]->Fill("L2bwBits", 1.); // confirmation bits were set properly

   if (mWEvent->l2bitRnd) {
      hA[0]->Fill("L2bwRnd", 1.);

      for (int m = 0; m < 300; m++) {
         int val = stMuEvent->emcTriggerDetector().highTower(m);
         hA[7]->Fill(val);
      }

      hA[61]->Fill(mWEvent->bx7);
   }

   if (!mWEvent->l2bitET)  return -3; // drop L2W-random accepts

   if (mWEvent->l2bitET) hA[0]->Fill("L2bwET", 1.);

   // only monitor below 
   hA[2]->Fill(mWEvent->bx48);
   hA[3]->Fill(mWEvent->bx7);

   // access L0-HT data
   int mxVal = -1;

   for (int m = 0; m < 300; m++)	{
      int val = stMuEvent->emcTriggerDetector().highTower(m);

      if (mxVal < val) mxVal = val;

      if (mWEvent->l2bitET) hA[6]->Fill(val);

      if (val < par_DsmThres) continue;

      if (mWEvent->l2bitET) hA[8]->Fill(m);

      //printf("Fired L0 HT m=%d val=%d\n",m,val);
   }

   mWEvent->bemc.maxHtDsm = mxVal;
   return 0;
}


// returns non-zero on abort
int StVecBosMaker::ReadVertexInfo()
{
   int nInpPrimV = mStMuDstMaker->muDst()->numberOfPrimaryVertices();

   if (nInpPrimV < par_minPileupVert) return -1;

   // separate histos for barrel and endcap triggers
   if (mWEvent->l2bitET)  hA[0]->Fill("tpcOn", 1.);
   if (mWEvent->l2EbitET) hE[0]->Fill("tpcOn", 1.);

   int nVer  = 0;
   int nVerR = 0;

   for (int iv = 0; iv < nInpPrimV; iv++)
   {
      StMuPrimaryVertex *vertex = mStMuDstMaker->muDst()->primaryVertex(iv);
      assert(vertex);
      mStMuDstMaker->muDst()->setVertexIndex(iv);

      float rank   = vertex->ranking();
      float funnyR = 999;

      if (rank > 1e6)    funnyR = log(rank - 1e6) + 10;
      else if (rank > 0) funnyR = log(rank);
      else               funnyR = log(rank + 1e6) - 10;

      if (mWEvent->l2bitET)  hA[10]->Fill(funnyR);
      if (mWEvent->l2EbitET) hE[10]->Fill(funnyR);

      //keep some neg. rank vertices for endcap if matched to ETOW
      if (rank <= 0 && vertex->nEEMCMatch() <= 0) continue;

      const StThreeVectorF &r = vertex->position();

      // StThreeVectorF &er=vertex->posError();
      if (mWEvent->l2bitET && rank > 0) hA[11]->Fill(r.z());

      if (mWEvent->l2EbitET) hE[11]->Fill(r.z());

      nVer++; // count valid vertices

      if (fabs(r.z()) > par_vertexZ) continue;

      if (rank > 0) nVerR++; //count vertices with rank>0

      WEventVertex wv;
      wv.id         = iv;
      wv.z          = r.z();
      wv.rank       = rank;
      wv.funnyRank  = funnyR;
      wv.nEEMCMatch = vertex->nEEMCMatch();
      mWEvent->vertex.push_back(wv);
   }

   if (nVer <= 0) return -2;

   if (mWEvent->l2bitET && nVerR > 0) {
      hA[0]->Fill("primVert", 1.);
      hA[4]->Fill(mWEvent->bx48);
      hA[5]->Fill(mWEvent->bx7);
   }

   if (mWEvent->l2EbitET) {
      hE[0]->Fill("primVert", 1.);
      hE[4]->Fill(mWEvent->bx48);
      hE[5]->Fill(mWEvent->bx7);
   }

   // access L0-HT data
   StMuEvent *stMuEvent = mStMuDstMaker->muDst()->event();

   for (int m = 0; m < 300; m++)	{
      int val = stMuEvent->emcTriggerDetector().highTower(m);

      if (val < par_DsmThres) continue;

      if (mWEvent->l2bitET && nVerR > 0) hA[9]->Fill(m);
   }

   for (int m = 0; m<90; m++)	{
      int val = stMuEvent->emcTriggerDetector().highTowerEndcap(m);

      if (val < parE_DsmThres) continue;

      if (mWEvent->l2EbitET) hE[9]->Fill(m);
   }

   if (mWEvent->l2bitET)  hA[12]->Fill(nVerR);
   if (mWEvent->l2EbitET) hE[12]->Fill(nVer);

   if (mWEvent->vertex.size() <= 0) return -3;
   if (mWEvent->l2bitET && nVerR > 0) hA[0]->Fill("vertZ", 1.);
   if (mWEvent->l2EbitET) hE[0]->Fill("vertZ", 1.);

   return 0;
}


int StVecBosMaker::accessTracks()  // return non-zero on abort
{ //{{{
   int nTrOK = 0;

   // printf("\n nInp=%d eveID=%d nPVer=%d nAnyV= %d\n",nInpEve,mStMuDstMaker->muDst()->event()->eventId(),mWEvent->vertex.size(),mStMuDstMaker->muDst()->numberOfPrimaryVertices());

   for (uint iv = 0; iv < mWEvent->vertex.size(); iv++)
   {
      uint vertID = mWEvent->vertex[iv].id;
      assert(vertID < mStMuDstMaker->muDst()->numberOfPrimaryVertices());
      assert(vertID >= 0);
      StMuPrimaryVertex *vertex = mStMuDstMaker->muDst()->primaryVertex(vertID);
      assert(vertex);
      mStMuDstMaker->muDst()->setVertexIndex(vertID);
      float rank = vertex->ranking();
      assert(rank > 0 || (rank < 0 && vertex->nEEMCMatch()));
      Int_t nPrimTrAll = mStMuDstMaker->muDst()->GetNPrimaryTrack();

      for (int itr = 0; itr < nPrimTrAll; itr++)
      {
         StMuTrack *prTr = mStMuDstMaker->muDst()->primaryTracks(itr);

         if (prTr->flag() <= 0) continue;

         const StMuTrack *glTr = prTr->globalTrack();

         if (glTr == 0) continue; // see the reason at the end of this method

         //keep list of all tracks for TPC cone sum in tree ana
         mWEvent->vertex[iv].prTrList.push_back(prTr);
         StThreeVectorF ro = glTr->lastPoint();

         // TPC+prim vertex tracks and short EEMC tracks
         if (prTr->flag() != 301 && prTr->flag() != 311) continue;

         if (mWEvent->l2bitET && rank > 0 && prTr->flag() == 301)
            hA[20]->Fill("flag", 1.);

         if (mWEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)
            hE[20]->Fill("flag", 1.);

         float pt = prTr->pt();

         if (pt < 1.0) continue;

         if (mWEvent->l2bitET && rank > 0 && prTr->flag() == 301)
            hA[20]->Fill("pt1", 1.);

         if (mWEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)
            hE[20]->Fill("pt1", 1.);

         //accepted tracks......
         float hitFrac = 1.*prTr->nHitsFit() / prTr->nHitsPoss();
         StThreeVectorF ri = glTr->firstPoint();
         /* Victor: in reality mChiSqXY is a normal Xi2 for track and mChiSqZ is Xi2 of fit to  primary vertex   */
         float globChi2dof = glTr->chi2();
         float dedx = prTr->dEdx() * 1e6;

         //barrel algo track monitors
         if (mWEvent->l2bitET && rank > 0 && prTr->flag() == 301) {
            hA[21]->Fill(prTr->nHitsFit());
            hA[22]->Fill(hitFrac);
            hA[23]->Fill(ri.perp());
            hA[24]->Fill(ro.perp());

            //TPC sector dependent filter
            int secID = WtpcFilter::getTpcSec(ro.phi(), ro.pseudoRapidity());

            if (mTpcFilter[secID - 1].accept(prTr) == false) continue;

            if (secID == 20) continue; //poorly calibrated sector for Run 9+11+12?

            hA[25]->Fill(glTr->p().perp());

            if (glTr->charge() < 0) hA[27]->Fill(glTr->p().perp());

            hA[29]->Fill(pt);

            if (prTr->charge() < 0)hA[30]->Fill(pt);

            hA[26]->Fill(ro.pseudoRapidity(), ro.phi());

            if (pt > 5) //estimate TPC inefficiency in data
               hA[57]->Fill(ro.pseudoRapidity(), ro.phi());

            hA[35]->Fill(globChi2dof);

            // monitor chi2 for east/west TPC separately
            if (ri.z() > 0 && ro.z() > 0)  hA[58]->Fill(globChi2dof);

            if (ri.z() < 0 && ro.z() < 0)  hA[59]->Fill(globChi2dof);

            hA[36]->Fill(globChi2dof, ro.pseudoRapidity());
            hA[28]->Fill(prTr->p().mag(), dedx);

            if (pt > 10)
               hA[197]->Fill(ro.pseudoRapidity(), ro.phi());

            hA[198]->Fill(ro.pseudoRapidity(), prTr->pt());
         }

         //endcap algo track monitors
         if (mWEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin) {
            hE[20]->Fill("#eta>0.7", 1.);
            hE[21]->Fill(prTr->nHitsFit());
            hE[22]->Fill(hitFrac);
            hE[23]->Fill(ri.perp());
            hE[24]->Fill(ro.perp());

            // TPC sector dependent filter
            int secID = WtpcFilter::getTpcSec(ro.phi(), ro.pseudoRapidity());

            if ( mTpcFilterE[secID - 1].accept(prTr) == false) continue;

            hE[25]->Fill(glTr->p().perp());

            if (glTr->charge() < 0)hE[27]->Fill(glTr->p().perp());

            hE[29]->Fill(pt);

            if (prTr->charge() < 0)hE[30]->Fill(pt);

            hE[26]->Fill(ro.pseudoRapidity(), ro.phi());

            if (pt > 5) //estimate TPC inefficiency in data
               hE[57]->Fill(ro.pseudoRapidity(), ro.phi());

            hE[35]->Fill(globChi2dof);
            hE[36]->Fill(globChi2dof, ro.pseudoRapidity());
            hE[28]->Fill(prTr->p().mag(), dedx);
         }


         bool barrelTrack = (mWEvent->l2bitET && rank > 0 && prTr->flag() == 301 && pt > par_trackPt);

         if (barrelTrack) hA[20]->Fill("ptOK", 1.); //good barrel candidate

         bool endcapTrack = (mWEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin && pt > parE_trackPt);

         if (endcapTrack) hE[20]->Fill("ptOK", 1.); //good endcap candidate

         if (!barrelTrack && !endcapTrack) continue;

         //keep all tracks in one container
         nTrOK++;
         WeveEleTrack wTr;

         wTr.prMuTrack = prTr;
         wTr.glMuTrack = glTr;
         StThreeVectorF prPvect = prTr->p();
         wTr.primP = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

         mWEvent->vertex[iv].eleTrack.push_back(wTr);
      } // loop over tracks
   } // loop over vertices

   if (nTrOK <= 0) return -1;

   if (mWEvent->l2bitET)  hA[0]->Fill("Pt10", 1.);
   if (mWEvent->l2EbitET) hE[0]->Fill("Pt10", 1.);

   return 0;
} //}}}


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
 *       = +x01 -> good track
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


int StVecBosMaker::ReadBTOWInfo()
{
   StMuEmcCollection *emc = mStMuDstMaker->muDst()->muEmcCollection();

   if (!emc) {
      gMessMgr->Warning() << "No EMC data for this event" << endm;    return -4;
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

      if (statPed  != 1) { mWEvent->bemc.statTile[ibp][softID - 1] = 1; n1++; continue; }
      if (statOfl  != 1) { mWEvent->bemc.statTile[ibp][softID - 1] = 2; n2++; continue; }
      if (statGain != 1) { mWEvent->bemc.statTile[ibp][softID - 1] = 4; n3++; continue; }

      mWEvent->bemc.statTile[ibp][softID - 1] = 0 ;

      float ped, sigPed, gain;
      int capID = 0; // just one value for btow

      mBarrelTables->getPedestal(BTOW, softID, capID, ped, sigPed);
      mBarrelTables->getCalib(BTOW, softID, 1, gain);

      if (use_gains_file == 1) {
         gain = gains_BTOW[softID];
      }

      //printf("id=%d gain=%f\n",softID,gain);

      //method for shifting energy scale
      gain = gain * mParBTOWScale; //(default is mParBTOWScale=1)

      float adc = rawAdc - ped;

      if (adc > 0) n4++;
      if (adc < par_kSigPed * sigPed) continue;
      if (adc < par_AdcThres)         continue;

      n5++;
      mWEvent->bemc.adcTile[ibp][softID - 1] = adc;
      mWEvent->bemc.eneTile[ibp][softID - 1] = adc * gain;

      if (maxADC < adc) { maxID = softID; maxADC = adc;}

      adcSum += adc;
   }

   //printf("NNN %d %d %d %d %d %d id=%d\n",n0,n1,n2,n3,n4,n5,maxID);
   if (n0 == mxBtow) return -1 ; // BTOW was not present in this events

   mWEvent->bemc.tileIn[ibp] = 1; //tag usable data

   if (nInpEve % 5000 == 1) {
      LOG_INFO << Form("unpackMuBTOW() dataIn=%d, nBbad: ped=%d stat=%d gain=%d ; nAdc: %d>0, %d>thres\n    maxADC=%.0f softID=%d adcSum=%.0f",
                       mWEvent->bemc.tileIn[ibp], n1, n2, n3, n4, n5,
                       maxADC, maxID, adcSum
                      ) << endm;
   }

   hA[31]->Fill(maxADC);
   hA[32]->Fill(adcSum);

   mWEvent->bemc.maxAdc = maxADC;

   if (maxID <= 2400) hA[195]->Fill(maxADC);
   else hA[196]->Fill(maxADC);

   if (maxADC < par_maxADC)  return -2 ; // not enough energy

   return 0;
}


void StVecBosMaker::fillTowHit(bool vert)
{
   if (!mWEvent->l2bitET) return; //only barrel triggers

   //find highest rank vertex
   float maxRank = 0; uint maxRankId = 0;

   for (uint iv = 0; iv < mWEvent->vertex.size(); iv++)
   {
      float rank = mWEvent->vertex[iv].rank;

      if (rank < 0) continue;

      if (rank > maxRank) {
         maxRank = rank;
         maxRankId = iv;
      }
   }

   int bx7 = mWEvent->bx7; int bxBin = -1;

   if (bx7 >= 0 && bx7 < 30) bxBin = 0;
   else if (bx7 < 40)        bxBin = 1;
   else if (bx7 < 110)       bxBin = 2;
   else if (bx7 < 120)       bxBin = 3;

   float Rcylinder  = mBtowGeom->Radius();
   float Rcylinder2 = Rcylinder*Rcylinder;

   //loop barrel towers and fill histo
   for (int i = 0; i < mxBtow; i++) {
      float adc = mWEvent->bemc.adcTile[kBTow][i];
      bool fillAdc = false;

      if (adc > 10) fillAdc = true; //~150 MeV threshold for tower firing

      if (vert) {
         if (fillAdc) hA[215 + bxBin]->Fill(positionBtow[i].Eta(), positionBtow[i].Phi());

         float ene = mWEvent->bemc.eneTile[kBTow][i];
         float delZ = positionBtow[i].z() - mWEvent->vertex[maxRankId].z;
         float e2et = Rcylinder / sqrt(Rcylinder2 + delZ * delZ);
         float ET = ene * e2et;

         if (ET > 2.0) hA[219 + bxBin]->Fill(positionBtow[i].Eta(), positionBtow[i].Phi());
      }
      else if (fillAdc) hA[223 + bxBin]->Fill(positionBtow[i].Eta(), positionBtow[i].Phi());
   }

   //some lower threshold plots
   for (int isec = 0; isec < mxEtowSec; isec++) {
      for (int isub = 0; isub < mxEtowSub; isub++) {
         for (int ieta = 0; ieta < mxEtowEta; ieta++) {
            int iPhi = isec * mxEtowSub + isub;
            float adc = mWEvent->etow.adc[iPhi][ieta];
            bool fillAdc = false;

            if (adc > 10) fillAdc = true; //~150 MeV threshold for tower firing

            if (vert) {
               if (fillAdc) hA[227 + bxBin]->Fill(ieta, iPhi);

               float ene = mWEvent->etow.ene[iPhi][ieta];
               float delZ = positionEtow[iPhi][ieta].z() - mWEvent->vertex[maxRankId].z;
               float Rxy = positionEtow[iPhi][ieta].Perp();
               float e2et = Rxy / sqrt(Rxy * Rxy + delZ * delZ);
               float ET = ene * e2et;

               if (ET > 2.0) hA[231 + bxBin]->Fill(ieta, iPhi);
            }
            else if (fillAdc) hA[235 + bxBin]->Fill(ieta, iPhi);
         }
      }
   }
}


/**
 * Returns the sum of all track P_T's inside the cone around the refAxis track.
 *
 * XXX:ds: Do we want to count the refAxis P_T?
 * The calculated sum is scalar. We probably want a vector sum.
 */
float StVecBosMaker::sumTpcCone(int vertID, TVector3 refAxis, int flag, int pointTowId)
{
   // flag=2 use 2D cut, 1= only delta phi

   // printf("******* sumTpcCone, flag=%d eveId=%d vertID=%d  eta0=%.2f phi0/rad=%.2f  \n",flag,mWEvent->id,vertID,refAxis.PseudoRapidity() ,refAxis.Phi());

   assert(vertID >= 0);
   assert(vertID < (int) mStMuDstMaker->muDst()->numberOfPrimaryVertices());

   StMuPrimaryVertex* vertex = mStMuDstMaker->muDst()->primaryVertex(vertID);
   assert(vertex);
   mStMuDstMaker->muDst()->setVertexIndex(vertID);

   float rank = vertex->ranking();
   assert(rank > 0 || (rank < 0 && vertex->nEEMCMatch()));

   double ptSum = 0;
   Int_t nPrimTrAll = mStMuDstMaker->muDst()->GetNPrimaryTrack();

   for (int itr = 0; itr < nPrimTrAll; itr++) {
      StMuTrack *prTr = mStMuDstMaker->muDst()->primaryTracks(itr);

      if (prTr->flag() <= 0) continue;

      if (prTr->flag() != 301 && pointTowId > 0) continue; // TPC-only regular tracks for barrel candidate

      if (prTr->flag() != 301 && prTr->flag() != 311 && pointTowId < 0) continue; // TPC regular and short EEMC tracks for endcap candidate

      float hitFrac = float(prTr->nHitsFit()) / prTr->nHitsPoss();

      if (hitFrac < par_nHitFrac) continue;

      StThreeVectorF prPvect = prTr->p();
      TVector3 primP = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

      // printf(" prTrID=%4d  prTrEta=%.3f prTrPhi/deg=%.1f prPT=%.1f  nFitPts=%d\n", prTr->id(),prTr->eta(),prTr->phi()/3.1416*180.,prTr->pt(),prTr->nHitsFit());
      if (flag == 1) {
         float deltaPhi = refAxis.DeltaPhi(primP);

         if (fabs(deltaPhi) > par_awayDeltaPhi) continue;
      }

      if (flag == 2) {
         float deltaR = refAxis.DeltaR(primP);

         if (deltaR > par_nearDeltaR) continue;
      }

      float pT = prTr->pt();
      //printf(" passed pt=%.1f\n",pT);

      //separate quench for barrel and endcap candidates
      if      (pT > par_trackPt  && pointTowId > 0) ptSum += par_trackPt;
      else if (pT > parE_trackPt && pointTowId < 0) ptSum += parE_trackPt;
      else  ptSum += pT;
   }

   return ptSum;
}


void StVecBosMaker::accessBSMD()
{ //{{{
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

      for (int i = 0; i < nh; i++) {
         StMuEmcHit *hit = emc->getSmdHit(i, iEP);
         float  adc = hit->getAdc();
         int softID = hit->getId();

         int statPed, statOfl, statGain;
         mBarrelTables->getStatus(iEP, softID, statPed, "pedestal");
         mBarrelTables->getStatus(iEP, softID, statOfl);
         mBarrelTables->getStatus(iEP, softID, statGain, "calib");

         if (statPed != 1) {
            mWEvent->bemc.statBsmd[iep][softID - 1] = 1;
            n1++; continue;
         }

         if (statOfl != 1) {
            mWEvent->bemc.statBsmd[iep][softID - 1] = 2;
            n2++; continue;
         }

         if (statGain < 1 || statGain > 19) {
            mWEvent->bemc.statBsmd[iep][softID - 1] = 4;
            n3++; continue;
         }

         float pedRes, sigPed, gain;
         int capID = 0; // just one value for ped residua in pp500, 2009 run

         mBarrelTables->getPedestal(iEP, softID, capID, pedRes, sigPed);
         mBarrelTables->getCalib(iEP, softID, 1, gain);

         if (isMC) { // overwrite it based on genat DE & private calibration
            float par_bsmdAbsGain = 6e6; // tmp arbitrary absolute calib of bsmd, was 3e6
            float  de = hit->getEnergy();// Geant energy deposit (GeV)
            adc = de * par_bsmdAbsGain;
         }
         else {   // correct for pedestal residua
            adc -= pedRes;

            if (adc > 0) n4++;
            if (adc < par_kSigPed * sigPed) continue;
         }

         n5++;
         assert(softID >= 1);      assert(softID <= mxBStrips);
         int id0 = softID - 1;
         mWEvent->bemc.adcBsmd[ iep][id0] = adc;
         hA[70 + 10 * iep]->Fill(adc);

         //if(nInpEve<3 || i <20 )printf("  i=%d, smd%c id=%d, m=%d adc=%.3f pedRes=%.1f, sigP=%.1f stat: O=%d P=%d G=%d  gain=%.2f\n",i,cPlane[iep],softID,1+id0/150,adc,pedRes,sigPed, statOfl,statPed,statGain, gain);
      }// end of hit list

      if (nTrigEve % 5000 == 1) {
         LOG_INFO << Form("unpackMuBSMD-%c() nBbad: ped=%d stat=%d gain=%d ; nAdc: %d>0, %d>thres", cPlane[iep], n1, n2, n3, n4, n5) << endm;
      }
   } // end of E-, P-plane loop
} //}}}


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
   for (int i = 0; i < mxBtow; i++) {
      //zero means good
      if (mWEvent->bemc.statTile[0][i] != 0 || mWEvent->bemc.eneTile[0][i] <= par_l2emulSeedThresh)
         continue;

      int   ieta = -1;
      int   iphi = -1;
      float etaF = positionBtow[i].Eta();
      float phiF = positionBtow[i].Phi();

      L2algoEtaPhi2IJ(etaF, phiF, ieta, iphi);
      WeveCluster c = maxBtow2x2(ieta, iphi, 0);

      if (c.ET > par_l2emulClusterThresh) return true;
   }

   return false;
}


/** Converts patch index to eta-phi bins? */
void StVecBosMaker::PatchToEtaPhi(int patch, int *eta, int *phi)
{
   if (patch < 0 || patch > 299) {
      Error("PatchToEtaPhi", "PatchToEtaPhi p=%d, out of range. Eta phi not defined.\n", patch);
      return;
   }

   if (patch < 150) {
      int m = 14 - patch / 10;
      int n = patch % 10;
      *eta = n / 2 + 5;
      *phi = n % 2 + m * 2;
   }
   else {
      int m = 29 - patch / 10;
      int n = patch % 10;
      *eta = 4 - n / 2;
      *phi = 1 - n % 2 + m * 2;
   }
}
