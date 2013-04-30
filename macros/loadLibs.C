
void loadLibs()
{
   gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
   loadSharedLibraries();
   gSystem->Load("StJets");
   gSystem->Load("~/rootmacros/utils/libutils.so");
   gSystem->Load("~/offline/users/fazio/vbasym/.sl53_gcc432/lib/libStVecBosAna.so");

}
