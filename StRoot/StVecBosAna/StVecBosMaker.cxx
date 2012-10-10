#include <limits>

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
   mMuDstMaker(0),
   Tfirst(numeric_limits<int>::max()), Tlast(numeric_limits<int>::min())
{
   mMuDstMaker = (StMuDstMaker *) GetMaker("MuDst");

   coreTitle = name;

   if (!mMuDstMaker) { // load tree if no MuDst
      Info("StVecBosMaker", "MuDst maker is not defined. Creating W tree");
      mTreeChain = new TChain("mWtree", "W candidate events");
      index = 0;
   }

   //must have either MuDst or W tree
   assert(mMuDstMaker || mTreeChain);

   mJetReaderMaker = (StJetReader*) GetMaker("JetReader");

   if (!mJetReaderMaker && mTreeChain) { // jet chain for reading W tree
      mJetTreeChain = new TChain("jet", "Jet Tree");
      indexJet = 0;
   }

   if (!mJetTreeChain && !mJetReaderMaker)
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

   setEtowScale(1.0);              // for old the Endcap geometr you need ~1.3
   setBtowScale(1.0);

   mRunNo                  = 0;
   nRun                    = 0;
   hbxIdeal                = 0;

   // irrelevant for W analysis
   par_DsmThres            = 31;   // only for monitoring
   parE_DsmThres           = 31;   // only for monitoring
   par_maxDisplEve         = 1;    // # of displayed selected events

   use_gains_file          = 0;
}


//
Int_t StVecBosMaker::Init()
{
   assert(HList);
   initHistos();
   initEHistos();

   if (mMuDstMaker) {
      //only need DB tables for MuDst analysis
      mBarrelTables = new StBemcTables();
      mDbE = (StEEmcDb *)GetDataSet("StEEmcDb");
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

   geomE = new EEmcGeomSimple();
   geoSmd = EEmcSmdGeom::instance();
   initGeom();

   wDisaply = new WeventDisplay(this, par_maxDisplEve);

   if (isMC) par_minPileupVert = 1;

   // tree only written during MuDst analysis
   if (mMuDstMaker) {
      mTreeFile = new TFile(mTreeName, "recreate");
      mTreeFile->cd();

      mWEvent = new WEvent();
      mWtree = new TTree("mWtree", "W candidate Events");
      mWtree->Branch("mWEvent", "WEvent", &mWEvent);
   }

   return StMaker::Init();
}


/** */
Int_t StVecBosMaker::InitRun(int runNo)
{
   LOG_INFO << Form("::InitRun(%d) start", runNo) << endm;

   //if (!isMC) assert(mRunNo == 0); // to prevent run merging - it was not tested

   if (mMuDstMaker) {
      mBarrelTables->loadTables(this );
      mRunNo = runNo;
   }
   else {
      mRunNo = mWEvent->runNo;
   }

   //barrel algo params
   LOG_INFO << Form("::InitRun(%d) %s done\n
      Barrel W-algo params: trigID L2BW=%d isMC=%d\n
      TPC: nPileupVert>%d, vertex |Z|<%.1fcm, primEleTrack: nFit>%d, hitFrac>%.2f Rin<%.1fcm, Rout>%.1fcm, PT>%.1fGeV/c\n
      BTOW ADC: kSigPed=%d AdcThr>%d maxAdc>%.0f clustET>%.1f GeV  ET2x2/ET4x4>%0.2f  ET2x2/nearTotET>%0.2f\n
      dist(track-clust)<%.1fcm, nearDelR<%.1f\n
      W selection highET>%.1f awayDelPhi<%.1frad  ptBalance>%.1fGeV  %.1f<leptonEta<%.1f ",
      mRunNo, coreTitle.Data(), par_l2bwTrgID, isMC,
      par_minPileupVert, par_vertexZ,
      par_nFitPts, par_nHitFrac,  par_trackRin,  par_trackRout, par_trackPt,
      par_kSigPed, par_AdcThres, par_maxADC, par_clustET, par_clustFrac24, par_nearTotEtFrac,
      par_delR3D, par_nearDeltaR,
      par_highET, par_awayDeltaPhi, par_ptBalance, par_leptonEtaLow, par_leptonEtaHigh
      ) << endm;

   //endcap algo params
   cout << Form("\n
   Endcap W-algo params: trigID: L2EW=%d isMC=%d\n
   TPC: nPileupVert>%d, vertex |Z|<%.1fcm, primEleTrack: nFit>%d, hitFrac>%.2f Rin<%.1fcm, Rout>%.1fcm, PT>%.1fGeV/c\n
   ETOW ADC: kSigPed=%d AdcThr>%d maxAdc>%.0f clustET>%.1f GeV  ET2x1/ET4x4>%0.2f  ET2x1/nearTotET>%0.2f\n
   dist(track-clust)<%.1fcm, nearDelR<%.1f\n
   W selection highET>%.1f awayDelPhi<%.1frad  ptBalance>%.1fGeV ",
   parE_l2ewTrgID, isMC,
   par_minPileupVert, par_vertexZ,
   parE_nFitPts, parE_nHitFrac, parE_trackRin, parE_trackRout, parE_trackPt,
   par_kSigPed, par_AdcThres, par_maxADC, parE_clustET, parE_clustFrac24, parE_nearTotEtFrac,
   parE_delR3D, par_nearDeltaR,
   par_highET, par_awayDeltaPhi, parE_ptBalance
   ) << endl;

   cout << Form("\n EtowScaleFact=%.2f  BtowScaleFacor=%.2f" , par_etowScale, par_btowScale) << endl;

   if (mMuDstMaker) {
      // initialization of TPC cuts is run dependent (only MuDst analysis)
      for (int isec = 0; isec < mxTpcSec; isec++)
      {
         int   sec   = isec + 1;
         float Rin   = par_trackRin;
         float Rout  = par_trackRout;
         float RinE  = parE_trackRin;
         float RoutE = parE_trackRout;
         // Rin changes

         //Run 9 (final)
         if (sec == 4  && mRunNo >= 10090089) Rin = 125.;

         if (sec == 11 && mRunNo >= 10083013) Rin = 125.;

         if (sec == 15 && mRunNo >= 10088096 && mRunNo <= 10090112 ) Rin = 125.;

         //Run 11 ??

         //Run 12 (not final, need to identify where electronics died, JS)
         if ((sec == 5 || sec == 6 || sec == 7 || sec == 21) && (mRunNo >= 13000000 || mRunNo / 100 == 3) )
            Rin = 125.; //all have dead inner padrows, run#300+nn is for M-C generated by Jan

         // Rout changes

         //Run 9 (final)
         if (sec == 5 && mRunNo >= 10098029) Rout = 140.;

         if (sec == 6 ) Rout = 140.;

         if (sec == 20 && mRunNo >= 10095120 && mRunNo <= 10099078 ) Rout = 140.;

         //Run 11 ??

         //Run 12 ??
         mTpcFilter[isec].setCuts(par_nFitPts, par_nHitFrac, Rin, Rout);
         mTpcFilter[isec].init("sec", sec, HListTpc, true);

         mTpcFilterE[isec].setCuts(parE_nFitPts, parE_nHitFrac, RinE, RoutE);
         mTpcFilterE[isec].init("secEemcTr", sec, HListTpc, false);
      }
   }

   // spinDB monitoring
   if (mMuDstMaker && spinDb) {
      char txt[1000], txt0[100];
      sprintf(txt0, "bxIdeal%d", nRun);
      sprintf(txt, "intended fill pattern  R%d-%d vs. bXing; %s", runNo, nRun, spinDb->getV124comment());
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


//________________________________________________
Int_t StVecBosMaker::Finish()
{
   if (mMuDstMaker) {
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

//________________________________________________
//________________________________________________
Int_t StVecBosMaker::FinishRun(int runNo)
{
   LOG_INFO << Form("::FinishRun(%d)", runNo) << endm;
   return kStOK;
}

//________________________________________________
//________________________________________________
void StVecBosMaker::Clear(const Option_t *)
{
   mWEvent->clear();
}


//
Int_t StVecBosMaker::Make()
{
   nInpEve++;

   if (mMuDstMaker) { // standard MuDst analysis
      mWEvent->id      = mMuDstMaker->muDst()->event()->eventId();
      mWEvent->runNo   = mMuDstMaker->muDst()->event()->runId();
      mWEvent->time    = mMuDstMaker->muDst()->event()->eventInfo().time();
      mWEvent->zdcRate = mMuDstMaker->muDst()->event()->runInfo().zdcCoincidenceRate();

      int T = mWEvent->time;

      Tlast  = (Tlast  < T) ? T : Tlast;
      Tfirst = (Tfirst > T) ? T : Tfirst;

      const char *afile = mMuDstMaker->GetFile();

      //printf("inpEve=%d eveID=%d daqFile=%s\n",nInpEve, mWEvent->id,afile);
      if (nInpEve % 200 == 1)
         printf("\n-----in---- %s, muDst nEve: inp=%d trig=%d accpt=%d daqFile=%s\n", GetName(), nInpEve, nTrigEve, nAccEve, afile);

      hA[0]->Fill("inp", 1.);
      hE[0]->Fill("inp", 1.);

      int btowStat = accessBTOW(); // get energy in BTOW
      int etowStat = accessETOW(); // get energy in ETOW

      int btrig    = accessBarrelTrig();
      int etrig    = accessEndcapTrig();

      if ( btrig && etrig )  { mWtree->Fill(); return kStOK; } //skip event w/o valid trig ID

      nTrigEve++;

      if (accessVertex()) {
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
      fillNorm();

      if ( accessTracks()) { mWtree->Fill(); return kStOK; } //skip event w/o ~any highPt track

      accessBSMD(); // get energy in BSMD
      accessESMD(); // get energy in ESMD
      accessEPRS(); // get energy in EPRS

      mWtree->Fill(); //write all events w/ pt>10 track to tree

      if (mWEvent->l2bitET  && mWEvent->bemc.tileIn[0] == 1) hA[0]->Fill("B-in", 1.0);
      if (mWEvent->l2EbitET && mWEvent->etow.etowIn == 1)    hE[0]->Fill("E-in", 1.0);
      if (mWEvent->l2bitET  && !btowStat)                    hA[0]->Fill("B200", 1.0);
      if (mWEvent->l2EbitET && !etowStat)                    hE[0]->Fill("E200", 1.0);

      if ( btowStat && etowStat ) return kStOK; //skip event w/o energy in BTOW && ETOW

      if (mJetReaderMaker) { // just QA plots for jets
         mJets = GetJets(mJetTreeBranch); //get input jet info

         for (int i_jet = 0; i_jet < nJets; ++i_jet) {
            StJet *jet    = GetJet(i_jet);
            float jet_pt  = jet->Pt();
            float jet_eta = jet->Eta();
            float jet_phi = jet->Phi();
            hA[117]->Fill(jet_eta, jet_phi);
            hA[118]->Fill(jet_pt);
         }
      }
   }
   else { // analysis of W tree

      if (getEvent(index++, indexJet++) == kStEOF)
         return kStEOF; //get next event from W and jet tree

      //allow for manual scale adjustment of BTOW energy (careful!)
      for (int i = 0; i < 4800; i++) mWEvent->bemc.eneTile[0][i] *= par_btowScale;

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

      //fill tpc bins
      int nVerR = 0; int nTrOK = 0;

      for (uint iv = 0; iv < mWEvent->vertex.size(); iv++) {
         if (mWEvent->vertex[iv].rank > 0)            nVerR++;
         if (mWEvent->vertex[iv].eleTrack.size() > 0) nTrOK++;
      }

      if (mWEvent->l2bitET && nVerR > 0) hA[0]->Fill("vertZ", 1.);
      if (mWEvent->l2EbitET && mWEvent->vertex.size() > 0) hE[0]->Fill("vertZ", 1.);
      if (mWEvent->l2bitET && nTrOK > 0) hA[0]->Fill("Pt10", 1.);
      if (mWEvent->l2EbitET && nTrOK > 0) hE[0]->Fill("Pt10", 1.);

      if (nTrOK <= 0) return kStOK;

      //fill some B/ETOW bins
      if (mWEvent->l2bitET && mWEvent->bemc.tileIn[0] == 1)       hA[0]->Fill("B-in", 1.0);
      if (mWEvent->l2EbitET && mWEvent->etow.etowIn == 1)         hE[0]->Fill("E-in", 1.0);
      if (mWEvent->l2bitET && mWEvent->bemc.maxAdc > par_maxADC)  hA[0]->Fill("B200", 1.0);
      if (mWEvent->l2EbitET && mWEvent->etow.maxAdc > par_maxADC) hE[0]->Fill("E200", 1.0);
      if (mWEvent->bemc.maxAdc < par_maxADC && mWEvent->etow.maxAdc < par_maxADC) return kStOK; //skip event w/o energy in BTOW && ETOW

      if (mJetTreeChain) { // just QA plots for jets
         mJets = GetJetsTreeAnalysis(mJetTreeBranch); //get input jet info

         for (int i_jet = 0; i_jet < nJets; ++i_jet) {
            StJet *jet    = GetJet(i_jet);
            float jet_pt  = jet->Pt();
            float jet_eta = jet->Eta();
            float jet_phi = jet->Phi();
            hA[117]->Fill(jet_eta, jet_phi);
            hA[118]->Fill(jet_pt);
         }
      }
   }

   // find barrel candidates
   extendTrack2Barrel();
   int bmatch = matchTrack2BtowCluster();

   // find endcap candidates
   extendTrack2Endcap();
   int ematch = matchTrack2EtowCluster();

   if (bmatch && ematch) return kStOK; //no matched BTOW or ETOW clusters

   nAccEve++;

   /* now it starts to get interesting, process every track
      on the list  till the end. */
   findNearJet();
   findAwayJet();

   if (mJetReaderMaker || mJetTreeChain) {
      findPtBalance();

      if (!bmatch) tag_Z_boson();
   }

   // endcap specific analysis
   if (!ematch) {
      analyzeESMD();
      analyzeEPRS();
   }

   if (!bmatch) find_W_boson();
   if (!ematch) findEndcap_W_boson();

   if (nAccEve < 2 || nAccEve % 1000 == 1 ) mWEvent->print(0x0, isMC);

   return kStOK;
}


//--------------------------------------
//--------------------------------------
void StVecBosMaker::initGeom()
{
   //...... BTOW ...........
   memset(mapBtowIJ2ID, 0, sizeof(mapBtowIJ2ID));

   for (int softID = 1; softID <= mxBtow; softID++) {
      //........... querry BTOW geom
      int m, e, s;
      mBtowGeom->getBin(softID, m, e, s);
      float etaF, phiF;
      mBtowGeom->getEta(m, e, etaF);
      mBtowGeom->getPhi(m, s, phiF); // -pi <= phi < pi

      int iEta, iPhi;
      assert(L2algoEtaPhi2IJ(etaF, phiF, iEta, iPhi) == 0); // tower must be localized at the known position
      int IJ = iEta + iPhi * mxBTetaBin;
      assert(mapBtowIJ2ID[IJ ] == 0); // avoid overlaping mapping
      mapBtowIJ2ID[IJ ] = softID;

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
            positionEtow[isec * mxEtowSub + isub][ieta] = geomE->getTowerCenter(isec, isub, ieta);
         }
      }
   }
}


//--------------------------------------
//--------------------------------------
// returns error code
int StVecBosMaker::L2algoEtaPhi2IJ(float etaF, float phiF, int &iEta, int &iPhi)
{
   if ( phiF < 0) phiF += 2 * C_PI; // I want phi in [0,2Pi]

   if (fabs(etaF) >= 0.99) return -1;

   int kEta = 1 + (int)((etaF + 1.) / 0.05);
   iPhi = 24 - (int)( phiF / C_PI * 60.);

   if (iPhi < 0) iPhi += 120;

   // convention:  iPhi=[0,119], kEta=[1,40]
   iEta = kEta - 1;

   //printf("IJ=%d %d\n",iEta,iPhi);
   if (iEta < 0 || iEta >= mxBTetaBin) return -2;

   if (iPhi < 0 || iPhi >= mxBTphiBin) return -3;

   return 0;
}


// intended for normalization of filtered QCD MC
void StVecBosMaker::fillNorm()
{
   //fill max BTOW clustET vs z-vertex distribution for events with positive rank vertex
   if (mWEvent->l2bitET && mWEvent->vertex.size() > 0)
   {
      if (mWEvent->vertex[0].rank > 0) {
         float maxBtowET = 0;

         for (int i = 0; i < mxBtow; i++)
            if (mWEvent->bemc.statTile[0][i] == 0) { //zero means good
               int ieta = -1; int iphi = -1;
               float etaF = positionBtow[i].Eta();
               float phiF = positionBtow[i].Phi();
               L2algoEtaPhi2IJ(etaF, phiF, ieta, iphi);
               WeveCluster c = maxBtow2x2(ieta, iphi, mWEvent->vertex[0].z);

               if (c.ET > maxBtowET) maxBtowET = c.ET;
            }

         hA[13]->Fill(mWEvent->vertex[0].z, maxBtowET);
      }
   }

   //fill max ETOW towerET vs z-vertex distribution for events with positive rank vertex
   if (mWEvent->l2EbitET && mWEvent->vertex.size() > 0)
   {
      if (mWEvent->vertex[0].rank > 0) {
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
}


/** */
TClonesArray* StVecBosMaker::GetJets(TString branchName)
{
   if (mJetReaderMaker == 0) {
      nJets = -1;
      return 0;
   }

   //assert(mJetReaderMaker->getStJets(branchName)->eventId() == mWEvent->id);
   if (mJetReaderMaker->getStJets(branchName)->eventId() != mWEvent->id)
      Error("GetJets", "Jet and W event ids do not match: %12d, %12d", mJetReaderMaker->getStJets(branchName)->eventId(), mWEvent->id);

   //assert(mJetReaderMaker->getStJets(branchName)->runId() == mWEvent->runNo);
   if (mJetReaderMaker->getStJets(branchName)->runId() != mWEvent->runNo)
      Error("GetJets", "Jet and W run ids do not match: %12d, %12d", mJetReaderMaker->getStJets(branchName)->runId(), mWEvent->runNo);

   nJets = mJetReaderMaker->getStJets(branchName)->nJets();

   return mJetReaderMaker->getStJets(branchName)->jets();
}


// Below is only used for Tree analysis
TClonesArray * StVecBosMaker::GetJetsTreeAnalysis(TString branchName)
{
   if (mJetTreeChain == 0) {
      nJets = -1;
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
   nJets = jetTmp->nJets();
   return jetTmp->jets();
}

// ----------------------------------------------------------------------------
StJets * StVecBosMaker::GetStJetsCopy(TString branchName)
{
   TBranch *branch = mJetTreeChain->GetBranch(branchName);
   return branch ? *(StJets **)branch->GetAddress() : 0;
}

// ----------------------------------------------------------------------------
Int_t StVecBosMaker::getEvent(Int_t i, Int_t ijet)
{
   Int_t stat    = mTreeChain->GetEntry(i);
   Int_t statJet = mJetTreeChain->GetEntry(ijet);

   if (!stat && !statJet) return kStEOF;

   return kStOK;
}

// ----------------------------------------------------------------------------
void StVecBosMaker::chainFile( const Char_t *file )
{

   TString fname = file;
   cout << "Chain W tree files" << endl;

   if ( !fname.Contains("tree.root") ) return;

   cout << "+ " << fname << endl;
   mTreeChain->Add(fname);
}

// ----------------------------------------------------------------------------
void StVecBosMaker::chainJetFile( const Char_t *file )
{

   TString fname = file;
   cout << "Chain jet tree files" << endl;

   if ( !fname.Contains("jets_") ) return;

   cout << "+ " << fname << endl;
   mJetTreeChain->Add(fname);
}
