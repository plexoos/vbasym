#include "vbana.h"

#include "TChain.h"
#include "TObject.h"
#include "TIterator.h"
#include "TROOT.h"
#include "TStreamerInfo.h"

#include "StRoot/StVecBosAna/WBosEvent.h"
#include "StRoot/StVecBosAna/VecBosRootFile.h"
#include "StRoot/StVecBosAna/VecBosAsymRootFile.h"

#include "utils/utils.h"

using namespace std;


int main(int argc, char *argv[])
{
   gROOT->Macro("~/rootmacros/styles/style_vbana.C");

   TStopwatch stopwatch;

   setbuf(stdout, NULL);

   //Int_t  nMaxUserEvents = -1; // < 0 means all events
   Int_t  nMaxUserEvents = 1000;

   //Bool_t isMc           = kFALSE;
   Bool_t isMc           = kTRUE;

   //string filelist       = "run11_pp_transverse";
   //string filelist       = "run11_pp_longitudinal";
   //string filelist       = "run12_pp";
   //string filelist       = "MC_list_QCD_2012";
   //string filelist       = "MC_list_Wm_2012";
   //string filelist       = "MC_list_Wp_2012";
   string filelist       = "runlists/run11_mc_Wp2enu.lis";
   //string filelist       = "MC_list_WmToTauTau_2012";
   //string filelist       = "MC_list_WpToTauTau_2012";
   //string filelist       = "MC_list_Ztoee_2012";

   //string stana_options  = "--jpm_0.5_--run_11";
   string stana_options  = "--jpm_0.5";
   stana_options = (isMc ? "-m_" : "") + stana_options;

   //string histFileName = "~/stana_out/" + filelist + "_" + stana_options + "/hist/vbana.root";
   string histFileName = "/star/data05/scratch/fazio/stana_out/" + filelist + "_" + stana_options + "/hist/vbana.root";

   Info("main", "nMaxUserEvents: %d", nMaxUserEvents);
   Info("main", "histFileName:   %s", histFileName.c_str());
   Info("main", "isMc:           %d", isMc);

   VecBosRootFile  vecBosRootFile(histFileName.c_str(), "recreate", isMc);
   //VecBosAsymRootFile  vecBosRootFile(histFileName.c_str(), "recreate", isMc); // to create the symmetry histograms 
   WBosEvent *wBosEvent = new WBosEvent();

   TObject *o;
   TIter   *next = new TIter(utils::getFileList("./runlists/"+filelist));
   UInt_t   nProcEvents = 0;

   // Loop over the runs and record the time of the last flattop measurement in the fill
   while (next && (o = (*next)()) )
   {
      string fName    = string(((TObjString*) o)->GetName());
      //string fileName = "~/stana_out/" + filelist + "_" + stana_options + "/tree/";
      string fileName = "/star/data05/scratch/fazio/stana_out/" + filelist + "_" + stana_options + "/tree/";
      fileName += (isMc ? "" : "R") + fName + "_tree.root";

      TFile *f = new TFile(fileName.c_str(), "READ");

      if (!f) { Error("main", "file not found. Skipping..."); delete f; continue; }
      if ( f->IsZombie() ) { Error("main", "file is zombie %s. Skipping...", fileName.c_str()); f->Close(); delete f; continue; }

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

      vbTree->SetBranchAddress("e", &wBosEvent);

      for (UInt_t iEvent=1; iEvent<=(UInt_t) nTreeEvents; iEvent++, nProcEvents++)
      {
         if ( nTreeEvents >= 10 && iEvent % UInt_t(nTreeEvents*0.1) == 0 )
            Info("main", "Analyzing event %d", iEvent);

         vbTree->GetEntry(iEvent-1);
         //wBosEvent->Print();
         vecBosRootFile.Fill(*wBosEvent);
      }

      f->Close();
      delete f;

      Info("main", "Proccessed events: %d", nProcEvents);
      stopwatch.Print();
      stopwatch.Continue();
      if (nMaxUserEvents > 0 && nProcEvents >= nMaxUserEvents) break;
   }

   delete wBosEvent;

   vecBosRootFile.FillDerived();
   vecBosRootFile.PostFill();

   string outDir = "../vbasym_results/" + filelist;
   //vecBosRootFile.SaveAs((string) "^.*$", outDir);
   //vecBosRootFile.SaveAs((string) ".*TrackEOverP.*", outDir);
   vecBosRootFile.Print();
   vecBosRootFile.Close();

   stopwatch.Print();
}
