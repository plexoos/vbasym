/* How to use this macro.
To run w/o jets

root4star -b -q 'analyzeMuDst.C(2e3,"st_W_12037041_raw_1400001.MuDst.root",0,0,5,7,"/star/data13/Magellan/reco/pp500_production_2011/ReversedFullField/P10k/2011/12037041/")'

To  produce jets
root4star -b -q 'analyzeMuDst.C(2e3,"st_W_12037041_raw_1400001.MuDst.root",0,1,5,7,"/star/data13/Magellan/reco/pp500_production_2011/ReversedFullField/P10k/2011/12037041/","/star/institutions/anl/balewski/jetTemp/")'

*/

#include <string>
#include <cstdlib>
#include <cassert>

#include "stana.h"

#include "TObject.h"
#include "TString.h"
#include "TStopwatch.h"

#include "St_base/StMessMgr.h"
#include "StChain.h"
#include "StEEmcDbMaker/StEEmcDbMaker.h"
#include "StMcEventMaker/StMcEventMaker.h"
#include "StEmcSimulatorMaker/StEmcSimulatorMaker.h"
#include "StEEmcSimulatorMaker/StEEmcSlowMaker.h"
#include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"
#include "StIOMaker/StIOMaker.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "St_db_Maker/St_db_Maker.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"


#include "StTriggerUtilities/StTriggerSimuMaker.h"
#include "StTriggerUtilities/Eemc/StEemcTriggerSimu.h"
#include "StTriggerUtilities/Bemc/StBemcTriggerSimu.h"
#include "StJetMaker/StBET4pMaker.h"
#include "StJetMaker/StJetMaker.h"
#include "StJetMaker/towers/StjTowerEnergyCorrectionForTracksFraction.h"

#include "StJetMaker/StppAnaPars.h"
#include "StJetFinder/StConePars.h"
#include "StJetMaker/StJetReader.h"

#include "StVecBosAna/St2011pubMcMaker.h"
#include "StVecBosAna/StVecBosMaker.h"
#include "StVecBosAna/StZBosMaker.h"
#include "StVecBosAna/St2011pubWanaMaker.h"
#include "StVecBosAna/StVecBosSpinMaker.h"
#include "StVecBosAna/AnaInfo.h"
#include "StVecBosAna/St2011WlumiMaker.h"
#include "StVecBosAna/VecBosRootFile.h"

#include "utils/utils.h"

using namespace std;


int analyzeMuDst(
   UInt_t   maxEventsUser       = 0,
   string   inMuDstFileListName = "st_W_12037063_raw_1380001_1201.MuDst.root",
   bool     isMC                = false,   // 0 = run9-data  200 = new MC w/ EEss in BFC
   int      useJetFinder        = 0,       // 0 - no jets = badWalgo; 1 generate jet trees; 2 read jet trees
   int      idL2BWtrg           = 0,       // offline Ids  needed for real data
   int      idL2EWtrg           = 0,       // run 9 L2EW
   // make those below  empty for scheduler
   string   muDir               = "",
   string   jetDir              = "",
   string   histDir             = "",
   string   wtreeDir            = "",
   bool     spinSort            = true,
   bool     findZ               = false,
   int      geant               = false
);


int main(int argc, char *argv[])
{
   setbuf(stdout, NULL);

   AnaInfo anaInfo;
   anaInfo.ProcessOptions(argc, argv);
   anaInfo.VerifyOptions();

   int  useJetFinder = anaInfo.fDoReconstructJets ? 1 : 2;
   bool isMC         = anaInfo.fIsMc; 

   return analyzeMuDst(anaInfo.fMaxEventsUser, anaInfo.GetListName(), isMC, useJetFinder, 320801, 320851);
}


int analyzeMuDst(UInt_t maxEventsUser, string inMuDstFileListName, bool isMC,
   int useJetFinder, int idL2BWtrg, int idL2EWtrg, string muDir, string jetDir,
   string histDir, string wtreeDir, bool spinSort, bool findZ, int geant)
{
   string eemcSetupPath = "/afs/rhic.bnl.gov/star/users/kocolosk/public/StarTrigSimuSetup/";

   if (isMC && useJetFinder == 2) geant = true;

   if (isMC) spinSort = false;

   string inputPathFile = inMuDstFileListName;

   size_t iLastSlash = inputPathFile.find_last_of("/");

   string inputPath("");

   if (iLastSlash != string::npos)
      inputPath = inputPathFile.substr(0, iLastSlash);
   else inputPath = "";

   string inputFile = inputPathFile.substr(iLastSlash + 1);

   printf("Input inputPath: %s\n", inputPath.c_str());
   printf("Input inputFile: %s\n", inputFile.c_str());
   printf("Input inMuDstFileListName: %s\n", inMuDstFileListName.c_str());

   TString outF = inputFile;
   outF = outF.ReplaceAll(".MuDst.root", "");
   outF = outF.ReplaceAll(".lis", "");

   TString histFileName = histDir + outF + ".wana.hist.root";

   cout << "Output histo file " << histFileName << endl;

   VecBosRootFile vecBosRootFile(histFileName, "recreate"); 

   printf("Output file: %s\n", outF.Data());
   printf("TRIG ID: L2BW=%d, L2EW=%d, isMC=%d, useJetFinder=%d\n", idL2BWtrg, idL2EWtrg, isMC, useJetFinder );

   // Logger business
   gMessMgr->SwitchOff("D");
   gMessMgr->SwitchOn("I");

   cout << " loading done " << endl;

   // create chain
   StChain *stChain = new StChain("StChain");

   // Create "global" histogram storage array
   TObjArray *HList    = new TObjArray();
   TObjArray *HListTpc = new TObjArray();

   if (isMC && useJetFinder == 2)
   {
      // get geant file
      StIOMaker *stIOMaker = new StIOMaker();

      TObject *o;
      TIter   *next = new TIter(utils::getFileList(inMuDstFileListName.c_str()));

      while (next && (o = (*next)()) )
      {
         TString geantFileName = TString(((TObjString*) o)->GetName());
         geantFileName.ReplaceAll("eve_mu", "eve_geant");
         geantFileName.ReplaceAll("MuDst", "geant");

         Info("analyzeMuDst(...)", "Added geantFileName: %s", geantFileName.Data());
         stIOMaker->SetFile(geantFileName.Data());
      }

      stIOMaker->SetIOMode("r");
      stIOMaker->SetBranch("*", 0, "1");            // deactivate all branches
      stIOMaker->SetBranch("geantBranch", 0, "r");  // activate geant Branch
      stIOMaker->SetBranch("minimcBranch", 0, "r"); // activate geant Branch
   }

   // Now we add Makers to the chain...
   int maxFiles = 100;

   StMuDstMaker *stMuDstMaker = new StMuDstMaker(0, 0, "", inMuDstFileListName.c_str(), ".", maxFiles);
   stMuDstMaker->SetStatus("*", 0);
   stMuDstMaker->SetStatus("MuEvent", 1);
   stMuDstMaker->SetStatus("EmcTow", 1);
   stMuDstMaker->SetStatus("EmcSmde", 1);
   stMuDstMaker->SetStatus("EmcSmdp", 1);
   stMuDstMaker->SetStatus("PrimaryVertices", 1);
   stMuDstMaker->SetStatus("GlobalTracks", 1);
   stMuDstMaker->SetStatus("PrimaryTracks", 1);

   TChain *stMuDstMakerChain = stMuDstMaker->chain();

   assert(stMuDstMakerChain);

   int numTotalEvents = (int) stMuDstMakerChain->GetEntries();

   Info("analyzeMuDst(...)", "stMuDstMakerChain->ls()");
   stMuDstMakerChain->ls();

   if (numTotalEvents < 0) {
      Error("analyzeMuDst(...)", "Invalid number of events %d", numTotalEvents);
      return -1;
   }

   printf("Total number of events in muDst chain: %d\n", numTotalEvents);

   // For EEMC need full db access:
   St_db_Maker *stDbMaker = new St_db_Maker("StarDb", "MySQL:StarDb", "MySQL:StarDb", "$STAR/StarDb");

   if (isMC) {
      //stDbMaker->SetMaxEntryTime(20101215, 0); // keep the same DB snapshot as used in BFC for embedding
      stDbMaker->SetFlavor("Wbose2", "bsmdpCalib");
      stDbMaker->SetFlavor("Wbose2", "bsmdeCalib");
   }
   else { // embedding samples
      // run 11 data ???
      stDbMaker->SetFlavor("Wbose2", "bsmdeCalib"); // Willie's abs gains E-plane, run 9
      stDbMaker->SetFlavor("Wbose2", "bsmdpCalib"); // P-plane
      stDbMaker->SetFlavor("sim",    "bemcCalib");  // use ideal gains for real data
      stDbMaker->SetFlavor("sim",    "eemcPMTcal"); // use ideal gains for 2011 real data as well
   }

   // Load EEMC database
   StEEmcDbMaker *stEEmcDbMaker = new StEEmcDbMaker("eemcDb");

#if 0 // drop abs lumi for now
   if (!isMC && strstr(inMuDstFileListName, "fillListPhys")) {
      StTriggerFilterMaker *filterMaker = new StTriggerFilterMaker;
      filterMaker->addTrigger(230420); // AJP
      filterMaker->addTrigger(230411); // JP2
      filterMaker->addTrigger(bht3ID); // regular W -> e+ analysis
   }
#endif

   if (isMC && useJetFinder == 2)
   {
      StMcEventMaker *mcEventMaker = new StMcEventMaker();
      mcEventMaker->doPrintEventInfo  = false;
      mcEventMaker->doPrintMemoryInfo = false;

      //don't need geant for trigger simu
      //BEMC simulator:
      StEmcSimulatorMaker *emcSim = new StEmcSimulatorMaker(); // use this instead to "redo" converstion from geant->adc
      emcSim->setCalibSpread(kBarrelEmcTowerId, 0.15);         // spread gains by 15%
      emcSim->setCheckStatus(kBarrelEmcTowerId, false);
      emcSim->setMakeFullDetector(kBarrelEmcTowerId, true);
      emcSim->setDoZeroSuppression(kBarrelEmcTowerId, false);

      StEmcADCtoEMaker *stEmcADCtoEMaker = new StEmcADCtoEMaker();    // for real data this sets calibration and status
      stEmcADCtoEMaker->saveAllStEvent(true);

      // EEMC simulator:
      //StEEmcDbMaker   *stEEmcDbMaker = new StEEmcDbMaker("eemcDb"); // already added
      StEEmcSlowMaker *slowSim       = new StEEmcSlowMaker("slowSim");
      //slowSim->setSamplingFraction(0.0384); // effectively scales all Tower energies with a factor of 1.3 (for old private filtered simu only!)
      slowSim->setAddPed(true);
      slowSim->setSmearPed(true);

      //Get TriggerMaker
      StTriggerSimuMaker *simuTrig = new StTriggerSimuMaker("StarTrigSimu");
      assert(simuTrig);
      simuTrig->setHList(HList);
      //simuTrig->setMC(isMC); // must be before individual detectors, to be passed
      simuTrig->setMC(2); // must be before individual detectors, to be passed 
      simuTrig->useBbc();
      simuTrig->useEemc(0); // default=0: just process ADC, 1,2: comp w/trgData, see .
      assert(simuTrig->eemc);
      //simuTrig->eemc->setSetupPath((char *) eemcSetupPath.c_str());
      simuTrig->useBemc();
      simuTrig->bemc->setConfig(2);
   }

   // Jet finder code
   TString jetFile = jetDir + "jets_" + outF + ".root";
   cout << "BEGIN: Jet finder/reader on jetFile=\"" << jetFile << "\"" << endl;

   if (useJetFinder == 1) {
      // Makers for cluster finding
      StEmcADCtoEMaker *stEmcADCtoEMaker = new StEmcADCtoEMaker(); // for real data this sets calibration and status

      // here we also tag whether or not to do the swap:
      bool doTowerSwapFix   = true;
      bool use2003TowerCuts = false;
      bool use2006TowerCuts = true;

      // 4p maker using 100% tower energy correction
      StBET4pMaker* stBET4pMakerFrac100 = new StBET4pMaker("BET4pMakerFrac100", stMuDstMaker, doTowerSwapFix, new StjTowerEnergyCorrectionForTracksFraction(1.0));
      stBET4pMakerFrac100->setUse2003Cuts(use2003TowerCuts);
      stBET4pMakerFrac100->setUseEndcap(true);
      stBET4pMakerFrac100->setUse2006Cuts(use2006TowerCuts);

      // 4p maker using 100% tower energy correction (no endcap)
      StBET4pMaker* stBET4pMakerFrac100_noEEMC = new StBET4pMaker("BET4pMakerFrac100_noEEMC", stMuDstMaker, doTowerSwapFix, new StjTowerEnergyCorrectionForTracksFraction(1.0));
      stBET4pMakerFrac100_noEEMC->setUse2003Cuts(use2003TowerCuts);
      stBET4pMakerFrac100_noEEMC->setUseEndcap(false);
      stBET4pMakerFrac100_noEEMC->setUse2006Cuts(use2006TowerCuts);

      // Instantiate the stJetMaker and SkimEventMaker
      StJetMaker* stJetMaker = new StJetMaker("stJetMaker", stMuDstMaker, jetFile);
      //StJetSkimEventMaker* skimEventMaker = new StJetSkimEventMaker("StJetSkimEventMaker", stMuDstMaker,outSkimFile);

      // set the analysis cuts: (see StJetMaker/StppJetAnalyzer.h -> class StppAnaPars )
      StppAnaPars* stppAnaPars = new StppAnaPars();
      stppAnaPars->setFlagMin(0);     // track->flag() > 0
      stppAnaPars->setNhits(12);      // track->nHitsFit()>12
      stppAnaPars->setCutPtMin(0.2);  // track->pt() > 0.2
      stppAnaPars->setAbsEtaMax(2.0); // abs(track->eta())<1.6
      stppAnaPars->setJetPtMin(3.5);
      stppAnaPars->setJetEtaMax(100.0);
      stppAnaPars->setJetEtaMin(0);
      stppAnaPars->setJetNmin(0);

      // Setup the cone finder (See StJetFinder/StConeJetFinder.h -> class StConePars)
      StConePars* stConePars = new StConePars();
      stConePars->setGridSpacing(105, -3.0, 3.0, 120, -TMath::Pi(), TMath::Pi());  //include EEMC
      stConePars->setConeRadius(0.7);    // default=0.7
      stConePars->setSeedEtMin(0.5);
      stConePars->setAssocEtMin(0.1);
      stConePars->setSplitFraction(0.5); // default=0.5. if 0.3 less split?
      stConePars->setPerformMinimization(true);
      stConePars->setAddMidpoints(true);
      stConePars->setRequireStableMidpoints(true);
      stConePars->setDoSplitMerge(true);
      stConePars->setDebug(false);

      stJetMaker->addAnalyzer(stppAnaPars, stConePars, stBET4pMakerFrac100, "ConeJets12_100"); //100% subtraction
      stJetMaker->addAnalyzer(stppAnaPars, stConePars, stBET4pMakerFrac100_noEEMC, "ConeJets12_100_noEEMC"); //100% subtraction (no Endcap)

      Info("analyzeMuDst(...)", "stChain->ls");
      stChain->ls(3);
      Info("analyzeMuDst(...)", "stChain->Init()");
      stChain->Init();

      int nProcEvents = 0;
      int t1 = time(0);
      TStopwatch stopwatch;

      for (UInt_t iev=1; iev<=numTotalEvents; iev++)
      {
         printf("\n");
         Info("analyzeMuDst(...)", "Analyzing event %d", iev);

         if (maxEventsUser > 0 && nProcEvents >= maxEventsUser) break;

         stChain->Clear();
         int stat = stChain->Make();

         if (stat != kStOk && stat != kStSkip) break; // EOF or input error

         nProcEvents++;
      }

      cout << "run " << inMuDstFileListName << " maxEventsUser = " << nProcEvents << " total ";
      stopwatch.Print();
      printf("******************************************\n");

      int t2 = time(0);

      if (t2 == t1) t2 = t1 + 1;

      float tMnt = (t2 - t1) / 60.;
      float rate = 1.*nProcEvents / (t2 - t1);

      printf("Jets sorting done %d of maxEventsUser = %d, CPU rate= %.1f Hz, total time %.1f minute(s) \n\n", nProcEvents, numTotalEvents, rate, tMnt);
      cout << "END: jet finder " << endl;

      stChain->Finish();
      delete stChain;

      return 1;
   } 
   // the jet reconstruction ends here


   if (useJetFinder == 2) {
      cout << "Configure to read jet trees " << endl;
      StJetReader* stJetReader = new StJetReader();
      stJetReader->InitFile(jetFile);
   }

   StSpinDbMaker *stSpinDbMaker = new StSpinDbMaker("stSpinDbMaker");
   StVecBosMaker *stVecBosMaker = new StVecBosMaker("StVecBosMaker", &vecBosRootFile);

   if (spinSort)
   {
      //stSpinDbMaker = new StSpinDbMaker("stSpinDbMaker");
      stVecBosMaker->AttachSpinDb(stSpinDbMaker);

      enum {mxSM = 5}; // to study eta-cuts, drop Q/PT cut

      StVecBosSpinMaker *stVecBosSpinMaker[mxSM];

      for (int kk = 0; kk < mxSM; kk++)
      {
         char ttx[100];
         sprintf(ttx, "%cspin", 'A' + kk);
         printf("add spinMaker %s %d \n", ttx, kk);
         stVecBosSpinMaker[kk] = new StVecBosSpinMaker(ttx);
         stVecBosSpinMaker[kk]->AttachWalgoMaker(stVecBosMaker);
         //stVecBosSpinMaker[kk]->AttachSpinDb(stSpinDbMaker);
         stVecBosSpinMaker[kk]->SetHList(HList);

         if (kk == 1) stVecBosSpinMaker[kk]->setEta(-1., 0.);
         if (kk == 2) stVecBosSpinMaker[kk]->setEta(0, 1.);
         if (kk == 3) stVecBosSpinMaker[kk]->setQPT(-1); // disable Q/PT cut
         if (kk == 4) stVecBosSpinMaker[kk]->setNoEEMC();
      }
   }

   if (isMC) { // MC specific
      // S.F. - Here version does nothing (just pass .true.), but it was used
      // in 2009 for calling reweighting files that are depending on the MC type and
      // even run dependent
      stVecBosMaker->setMC(isMC); // pass "version" of MC to maker
      //stVecBosMaker->setJetNeutScaleMC(1.0);
      //stVecBosMaker->setJetChrgScaleMC(1.0);
   }
   else { // real data
      stVecBosMaker->setTrigID(idL2BWtrg, idL2EWtrg);
   }

   TString treeFileName = wtreeDir + outF + ".wtree.root";

   stVecBosMaker->SetTreeName(treeFileName);

   if (useJetFinder == 2)
      stVecBosMaker->setJetTreeBranch("ConeJets12_100", "ConeJets12_100_noEEMC"); //select jet tree braches used

   ////stVecBosMaker->setMaxDisplayEve(10); // only first N events will get displayed
   //set energy scale (works for data and MC - be careful!)
   // S.F. - We must be carefull to put correct numbers for run 2011 for both data and MC separately! 
   //stVecBosMaker->setBtowScale(1.0);
   //stVecBosMaker->setEtowScale(1.0);

   // evaluation of result, has full acess to W-algo internal data including
   // overwrite - be careful

   St2011pubWanaMaker* st2011pubWanaMaker = new St2011pubWanaMaker();
   st2011pubWanaMaker->AttachWalgoMaker(stVecBosMaker);

   //Collect all output histograms
   //already defined this above:  TObjArray* HList=new TObjArray;
   stVecBosMaker->setHList(HList);
   stVecBosMaker->setHListTpc(HListTpc);
   st2011pubWanaMaker->setHList(HList);

   //// S.F. - added 16 Oct. 2012 - 
   //// calculate lumi from runs
   //if(!isMC) {
   //  St2011WlumiMaker *WlumiMk = new St2011WlumiMaker("lumi"); 
   //  WlumiMk->AttachWalgoMaker(stVecBosMaker); 
   //  WlumiMk->attachMuMaker(stMuDstMaker);
   //  WlumiMk->setHList(HList);
   //}

   if (isMC && useJetFinder == 2) {
      St2011pubMcMaker *pubMcMk = new St2011pubMcMaker("pubMc");
      pubMcMk->AttachWalgoMaker(stVecBosMaker);
      pubMcMk->setHList(HList);
   }

   if (findZ) {
      StZBosMaker *st2011ZMaker = new StZBosMaker("Z");
      st2011ZMaker->AttachWalgoMaker(stVecBosMaker);
      st2011ZMaker->setHList(HList);
      st2011ZMaker->setNearEtFrac(0.88);
      st2011ZMaker->setClusterMinEt(15);
      st2011ZMaker->setPhi12Min(3.1416 / 2.);
      st2011ZMaker->setMinZMass(73.); // Zmass -20%
      st2011ZMaker->setMaxZMass(114.);// Zmass +20%
   }

   Info("main()", "stChain->ls");
   stChain->ls(3);
   Info("main()", "stChain->Init()");
   stChain->Init();

   int nProcEvents = 0;
   int t1 = time(0);
   TStopwatch stopwatch;

   for (UInt_t iev=1; iev<=numTotalEvents; iev++)
   {
      printf("\n");
      Info("analyzeMuDst(...)", "Analyzing event %d", iev);

      if (maxEventsUser > 0 && nProcEvents >= maxEventsUser) break;

      stChain->Clear();
      int stat = stChain->Make();
      
      if (stat != kStOk && stat != kStSkip) break; // EOF or input error

      nProcEvents++;
   }

   // S.F. - Must work on it--> hang on Dima ;)  
   //  if(!isMC) 
   //    WlumiMk->FinishRun(RunNo);

   stChain->Finish();
   // delete stChain;

   cout << "inMuDstFileListName: " << inMuDstFileListName << ", maxEventsUser = " << nProcEvents << " total " << endl;
   stopwatch.Print();
   printf("****************************************** \n");

   int t2 = time(0);

   if (t2 == t1) t2 = t1 + 1;

   float tMnt = (t2 - t1) / 60.;
   float rate = 1.*nProcEvents / (t2 - t1);

   printf("#sorting %s done %d of maxEventsUser = %d, CPU rate= %.1f Hz, total time %.1f minute(s) \n\n", inMuDstFileListName.c_str(), nProcEvents, numTotalEvents, rate, tMnt);

   if (vecBosRootFile.IsOpen()) {
      TDirectory *old = vecBosRootFile.mkdir("old");
      old->cd();
      HList->ls();
      HList->Write();
      // Write TPC histos to new directory
      TDirectory *tpc = vecBosRootFile.mkdir("tpc");
      tpc->cd();
      // HListTpc->Write();
   }
   else {
      printf("\n Failed to open Histo-file -->%s<, continue\n", histFileName.Data());
   }

   //vecBosRootFile.SaveAs((string) "^.*$", (string) "../vbasym_results/images_test/");
   vecBosRootFile.Print();
   vecBosRootFile.Close();

   //stVecBosMaker->Finish();

   return 2;
}
