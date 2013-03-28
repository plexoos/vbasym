
#include "vbana.h"

#include "TChain.h"
#include "TObject.h"
#include "TIterator.h"
#include "TROOT.h"
#include "TStreamerInfo.h"

#include "StRoot/StVecBosAna/VecBosEvent.h"
#include "StRoot/StVecBosAna/VecBosRootFile.h"

#include "utils/utils.h"

using namespace std;


int main(int argc, char *argv[])
{
   gROOT->Macro("./styles/style_vbana.C");

   TStopwatch stopwatch;

   setbuf(stdout, NULL);

   Int_t  nMaxUserEvents = -1;
   //Int_t  nMaxUserEvents = 10;
   //Int_t  nMaxUserEvents = 34000; // cut_35
   //Int_t  nMaxUserEvents = 10;
   //Bool_t isMc           = kFALSE;
   Bool_t isMc           = kTRUE;
   //string histFileName   = "vbana_cut15_data_final.root";
   //string histFileName   = "vbana_cut35_data.root";
   //string histFileName   = "vbana_cut35_data_100k.root";
   //string histFileName   = "vbana_cut15_mc.root";
   //string histFileName   = "vbana_cut35_mc_wm.root";
   string histFileName   = "vbana_cut10_mc_wp.root";

        //string filelist       = "./runlists/run11_pp_transverse";
	//string filelist       = "./runlists/run11_all_goodsofar_cut15";
	//string filelist       = "./runlists/run11_all_goodsofar_cut35";
	//string filelist       = "./runlists/run11_all";
	//string filelist       = "./runlists/MC_list_Wm_2012";
	string filelist       = "./runlists/MC_list_Wp_2012";

   Info("main", "nMaxUserEvents: %d", nMaxUserEvents);
   Info("main", "histFileName:   %s", histFileName.c_str());
   Info("main", "isMc:           %d", isMc);

   VecBosRootFile  vecBosRootFile(histFileName.c_str(), "recreate", isMc); 
   VecBosEvent    *vecBosEvent = new VecBosEvent();
   
   TObject *o;
   TIter   *next = new TIter(utils::getFileList(filelist));
   UInt_t   nProcEvents = 0;

   // Loop over the runs and record the time of the last flattop measurement in the fill
   while (next && (o = (*next)()) )
   {
      string fName    = string(((TObjString*) o)->GetName());
      //string fileName = "./R" + fName + ".wtree.root";
      //string fileName = "/star/data05/scratch/fazio/wtree_run11_cut15/";
      string fileName = "/star/data05/scratch/fazio/wtree_run11_cut10/";
      fileName += (!isMc?"R":"") + fName + ".wtree.root";

      TFile *f = new TFile(fileName.c_str(), "READ");

      if (!f) { Error("main", "file not found. Skipping..."); delete f; continue; }
      if ( f->IsZombie() ) { Error("main", "file is zombie %s. Skipping...", fileName.c_str()); f->Close(); delete f; continue; }

      TList *streamerList = f->GetStreamerInfoList();
      TStreamerInfo *streamerInfo = (TStreamerInfo*) streamerList->FindObject("VecBosTrack"); // this is actually class name
      //Int_t ver = streamerInfo->CompareContent();

      //if (ver != 1) {
      //   Error("main", "Wrong versio of VecBosTrack: %d. Skipping...", ver); f->Close(); delete f; 
      //   continue;
      //}

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

      //VecBosEvent    *vecBosEvent = new VecBosEvent();

      vbTree->SetBranchAddress("e", &vecBosEvent);

      //TBranch *eventBranch = vbTree->GetBranch("e");
      //eventBranch->SetAddress(&vecBosEvent);
      //eventBranch->SetAutoDelete(kTRUE);
      //vbTree->SetBranchStatus("e.*.prMuTrack", 0);

      for (UInt_t iEvent=1; iEvent<=(UInt_t) nTreeEvents; iEvent++, nProcEvents++)
      {
         if ( nTreeEvents >= 10 && iEvent % UInt_t(nTreeEvents*0.1) == 0 )
            Info("main", "Analyzing event %d", iEvent);

         vbTree->GetEntry(iEvent-1);
         //vecBosEvent->Print();
         vecBosRootFile.Fill(*vecBosEvent);
      }

      //delete vecBosEvent;
      //delete vbTree; 

      f->Close();
      delete f;

      Info("main", "Proccessed events: %d", nProcEvents);
      stopwatch.Print();
      stopwatch.Continue();
      if (nMaxUserEvents > 0 && nProcEvents >= nMaxUserEvents) break;
   }

   delete vecBosEvent;

   string outDir = "../vbasym_results/" + histFileName;
   vecBosRootFile.SaveAs((string) "^.*$", outDir);
   //vecBosRootFile.SaveAs((string) ".*TrackEOverP.*", outDir);
   vecBosRootFile.Print();
   vecBosRootFile.Close();

   stopwatch.Print();
}
