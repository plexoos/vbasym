
void loadLibs()
{
   gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
   loadSharedLibraries();
   gSystem->Load("StJets");
   gSystem->Load(".sl53_gcc432/lib/libutils.so");
   gSystem->Load(".sl53_gcc432/lib/libStVecBosAna.so");

}

