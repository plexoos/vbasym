
#include "TObject.h"
#include "TIterator.h"
#include "TROOT.h"
#include "TTree.h"

#include "StRoot/StVecBosAna/AsymCalculator.h"
#include "StRoot/StVecBosAna/WBosEvent.h"
#include "StRoot/StVecBosAna/ZBosEvent.h"
#include "StRoot/StVecBosAna/VecBosRootFile.h"
#include "StRoot/StVecBosAna/WBosRootFile.h"
#include "StRoot/StVecBosAna/ZBosRootFile.h"
#include "StRoot/StVecBosAna/VbAnaOptions.h"

#include "utils/utils.h"

using namespace std;


int main(int argc, char *argv[])
{
   VbAnaOptions vbAnaOptions;
   vbAnaOptions.ProcessOptions(argc, argv);

   gROOT->Macro("~/root-helper/styles/style_vbana.C");

   TStopwatch stopwatch;

   setbuf(stdout, NULL);

   Int_t  nMaxUserEvents = vbAnaOptions.GetMaxEventsUser();

   //Int_t  nMaxUserEvents = -1; // < 0 means all events
   //Int_t  nMaxUserEvents = 3000;

   Bool_t isZ            = (vbAnaOptions.GetBosonType() == kZBoson  ? kTRUE : kFALSE);

   string histFileName   = vbAnaOptions.GetRootFileName();

   Info("main", "nMaxUserEvents: %d", nMaxUserEvents);
   Info("main", "histFileName:   %s", histFileName.c_str());

   AsymCalculator::sVbAnaOptions = &vbAnaOptions;
   VecBosRootFile *vecBosRootFile;
   VecBosEvent    *vecBosEvent;

   if (vbAnaOptions.GetBosonType() == kWBoson)
   {

     if (!vbAnaOptions.GetOldStyle()) {
        vecBosRootFile = new WBosRootFile(histFileName.c_str(), "recreate", vbAnaOptions.McType());
     } else { // old style containers used for background and control plots
        vecBosRootFile = new VecBosRootFile(histFileName.c_str(), "recreate", vbAnaOptions.McType(), isZ);
     }
      vecBosEvent    = new WBosEvent(vbAnaOptions.GetTracksPtMin(), vbAnaOptions.UseOtherSolution() );

   } else if (vbAnaOptions.GetBosonType() == kZBoson)
   {

     if (!vbAnaOptions.GetOldStyle()) {
        vecBosRootFile = new ZBosRootFile(histFileName.c_str(), "recreate", vbAnaOptions.McType());
     } else { // old style containers used for background and control plots
        vecBosRootFile = new VecBosRootFile(histFileName.c_str(), "recreate", vbAnaOptions.McType(), isZ);
     }
      vecBosEvent    = new ZBosEvent();
   }

   TObject *o;
   TIter   *next = new TIter(utils::getFileList( vbAnaOptions.GetListFileName() ));
   UInt_t   nProcEvents = 0;

   // Loop over the runs and record the time of the last flattop measurement in the fill
   while (next && (o = (*next)()) )
   {
      string fName    = string(((TObjString*) o)->GetName());
      string fileName = vbAnaOptions.GetResultsDir() + "/tree/" + fName + "_tree.root";

      TFile *f = new TFile(fileName.c_str(), "READ");

      if (!f) { Error("main", "File not found. Skipping..."); delete f; continue; }
      if ( f->IsZombie() ) { Error("main", "File is zombie %s. Skipping...", fileName.c_str()); f->Close(); delete f; continue; }

      cout << endl;
      Info("main", "Found file: %s", fileName.c_str());

      TTree* vbTree = (TTree*) f->FindObjectAny("t");

      if (!vbTree) { Error("main", "Tree not found. Skipping..."); f->Close(); delete f; continue; }

      int nTreeEvents = vbTree->GetEntries();

      Info("main", "Found tree with nTreeEvents: %d", nTreeEvents);

      if (nTreeEvents <= 0) { Error("main", "Invalid number of events in input tree: %d. Skipping...", nTreeEvents); f->Close(); delete f; continue; }

      if (nMaxUserEvents > 0) {
         Int_t nExtraEvents = nProcEvents + nTreeEvents - nMaxUserEvents;
         nTreeEvents = nExtraEvents > 0 ? nTreeEvents - nExtraEvents : nTreeEvents;
      }

      vbTree->SetBranchAddress("e", &vecBosEvent);

      for (UInt_t iEvent=1; iEvent<=(UInt_t) nTreeEvents; iEvent++, nProcEvents++)
      {
         if ( nTreeEvents >= 10 && iEvent % UInt_t(nTreeEvents*0.1) == 0 )
            Info("main", "Analyzing event %d", iEvent);

         vbTree->GetEntry(iEvent-1);
         //vecBosEvent->Print();
         vecBosRootFile->Fill(*vecBosEvent);
      }

      f->Close();
      delete f;

      Info("main", "Proccessed events: %d", nProcEvents);
      stopwatch.Print();
      stopwatch.Continue();
      if (nMaxUserEvents > 0 && nProcEvents >= nMaxUserEvents) break;
   }

   delete vecBosEvent;

   vecBosRootFile->FillDerived();
   vecBosRootFile->PostFill();

   if ( vbAnaOptions.SaveGraphs() ) {
      vecBosRootFile->SaveAs((string) "^.*$", vbAnaOptions.GetImageDir());
   }

   vecBosRootFile->Print();
   vecBosRootFile->Close();

   stopwatch.Print();

   return EXIT_SUCCESS;
}
