
#include "vbana.h"

#include "TChain.h"

#include "StRoot/StVecBosAna/VecBosEvent.h"
#include "StRoot/StVecBosAna/VecBosRootFile.h"

#include "utils/utils.h"

using namespace std;


int main(int argc, char *argv[])
{
   setbuf(stdout, NULL);

   //Int_t maxEventsUser = -1;
   Int_t maxEventsUser = 10000;

   TChain *vbChain = new TChain("t", "A tree with VecBosEvent events");

<<<<<<< vbana.cc
   // vbChain->Add("/star/data05/scratch/fazio/wtree_run11_cut05/MC_list_Wp_2012_*.wtree.root");
   vbChain->Add("/star/data05/scratch/fazio/wtree_run11_cut05/R*.wtree.root");

   int numTotalEvents = vbChain->GetEntries();

   if (numTotalEvents <= 0) {
      Fatal("main", "Invalid number of events in input chain: %d", numTotalEvents);
   }

   numTotalEvents = (maxEventsUser > 0 && maxEventsUser < numTotalEvents) ? maxEventsUser : numTotalEvents;

   string histFileName = "vbana.root";

   Info("main", "numTotalEvents: %d", numTotalEvents);
   Info("main", "histFileName:   %s", histFileName.c_str());

   VecBosRootFile  vecBosRootFile(histFileName.c_str(), "recreate"); 
   VecBosEvent    *vecBosEvent = new VecBosEvent();

   vbChain->SetBranchAddress("e", &vecBosEvent);

   TStopwatch stopwatch;

   for (UInt_t iEvent=1; iEvent<=(UInt_t) numTotalEvents; iEvent++)
   {
      if ( iEvent % UInt_t(numTotalEvents*0.10) == 0 )
      {
         printf("\n");
         Info("main", "Analyzing event %d", iEvent);
      }

      vbChain->GetEntry(iEvent-1);
      //vecBosEvent->Print();
      vecBosRootFile.Fill(*vecBosEvent);
   }

   vecBosRootFile.Print();
   vecBosRootFile.Close();
}
