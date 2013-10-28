
/**
 * This macro can be used to load STAR and ROOT libraries in a interactive ROOT
 * session to work with trees produced by stana.
 */
void loadLibs()
{
   gSystem->Load("~/root-helper/build/utils/libroot-helper-utils-shared.so");

   //gSystem->Load("CDFConesPlugin");
   gSystem->Load("St_base");
   gSystem->Load("StChain");
   gSystem->Load("StarRoot");
   gSystem->Load("St_Tables");
   gSystem->Load("StUtilities");
   gSystem->Load("StTreeMaker");
   gSystem->Load("StIOMaker");
   gSystem->Load("StarClassLibrary");
   gSystem->Load("StTriggerDataMaker");
   gSystem->Load("StBichsel");
   gSystem->Load("StEvent");
   gSystem->Load("StEventUtilities");
   gSystem->Load("StDbLib");
   gSystem->Load("StEmcUtil");
   gSystem->Load("StTofUtil");
   gSystem->Load("StPmdUtil");
   gSystem->Load("StPreEclMaker");
   gSystem->Load("StStrangeMuDstMaker");
   gSystem->Load("StMuDSTMaker");
   gSystem->Load("StDaqLib");
   gSystem->Load("StDAQMaker");
   gSystem->Load("StDetectorDbMaker");
   gSystem->Load("StTpcDb");
   gSystem->Load("StDbUtilities");
   gSystem->Load("StDbBroker");
   gSystem->Load("St_db_Maker");
   gSystem->Load("StEEmcUtil");
   gSystem->Load("StEEmcDbMaker");
   gSystem->Load("StTriggerFilterMaker");
   gSystem->Load("StTriggerUtilities");
   gSystem->Load("StSpinDbMaker");
   gSystem->Load("StEmcRawMaker");
   gSystem->Load("StEmcADCtoEMaker");
   gSystem->Load("StJetSkimEvent");
   gSystem->Load("StJets");
   gSystem->Load("StSpinDbMaker");
   gSystem->Load("StEmcTriggerMaker");
   gSystem->Load("StTriggerUtilities");
   gSystem->Load("StMCAsymMaker");
   gSystem->Load("StRandomSelector");
   gSystem->Load("StJetEvent");
   gSystem->Load("StJetFinder");
   gSystem->Load("StJetMaker");
   gSystem->Load("StMcEvent");
   gSystem->Load("StMcEventMaker");
   gSystem->Load("StEmcSimulatorMaker");
   gSystem->Load("StEEmcSimulatorMaker");
   gSystem->Load("StEpcMaker");

   gSystem->Load("~/vbasym/.sl53_gcc432/lib/libStVecBosAna.so");
}
