
#include "VecBosAsymRootFile.h"

#include <climits>
#include <sstream>

#include "TROOT.h"
#include "TStyle.h"

#include "AsymHContainer.h"
#include "EventHContainer.h"
#include "WBosEventHContainer.h"
#include "MCHContainer.h"
#include "WBosEvent.h"
#include "VecBosTrack.h"
#include "VecBosVertex.h"

#include "utils/utils.h"

ClassImp(VecBosAsymRootFile)

using namespace std;


VecBosAsymRootFile::VecBosAsymRootFile() : VecBosRootFile()
{
   BookHists();
}


VecBosAsymRootFile::VecBosAsymRootFile(const char *fname, Option_t *option, Bool_t isMc, const char *ftitle, Int_t compress) :
   VecBosRootFile(fname, option, isMc, ftitle, compress)
{
   BookHists();
}


VecBosAsymRootFile::~VecBosAsymRootFile()
{
   //if (fHists) { delete fHists; fHists = 0; }
}


void VecBosAsymRootFile::BookHists()
{
   // delete histograms created in parent class
   if (fHists) { delete fHists; fHists = 0; }
   fHistCuts.clear();
   //this->Delete("*;*");
   //this->GetListOfKeys()->Delete();

   PlotHelper *ph;

   fHists = new PlotHelper(this);

   fHists->d["event"] = ph = new EventHContainer(new TDirectoryFile("event", "event", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["asym"] = ph = new AsymHContainer(new TDirectoryFile("asym", "asym", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["event_w"] = ph = new WBosEventHContainer(new TDirectoryFile("event_w", "event_w", "", this));
   fHistCuts[kCUT_EVENT_W].insert(ph);

   fHists->d["asym_w"] = ph = new AsymHContainer(new TDirectoryFile("asym_w", "asym_w", "", this));
   fHistCuts[kCUT_EVENT_W].insert(ph);

   fHists->d["event_w_plus"] = ph = new WBosEventHContainer(new TDirectoryFile("event_w_plus", "event_w_plus", "", this));
   fHistCuts[kCUT_EVENT_W_PLUS].insert(ph);

   fHists->d["asym_w_plus"] = ph = new AsymHContainer(new TDirectoryFile("asym_w_plus", "asym_w_plus", "", this));
   fHistCuts[kCUT_EVENT_W_PLUS].insert(ph);

   fHists->d["event_w_minus"] = ph = new WBosEventHContainer(new TDirectoryFile("event_w_minus", "event_w_minus", "", this));
   fHistCuts[kCUT_EVENT_W_MINUS].insert(ph);

   fHists->d["asym_w_minus"] = ph = new AsymHContainer(new TDirectoryFile("asym_w_minus", "asym_w_minus", "", this));
   fHistCuts[kCUT_EVENT_W_MINUS].insert(ph);

   if (!fIsMc) return;

   fHists->d["event_mc"] = ph = new MCHContainer(new TDirectoryFile("event_mc", "event_mc", "", this));
   fHistCuts[kCUT_EVENT_W].insert(ph);

   this->cd();
}


/** */
void VecBosAsymRootFile::Fill(ProtoEvent &ev)
{
   WBosEvent& w_event = (WBosEvent&) ev;

   Fill(ev, kCUT_EVENT_NOCUT);

   if ( w_event.PassedCutWBos(WBosEvent::sMinElectronPtHard) )
   {
      Fill(ev, kCUT_EVENT_W);

      if ( w_event.PassedCutWBosPlus(WBosEvent::sMinElectronPtHard) ) 
         Fill(ev, kCUT_EVENT_W_PLUS);

      if ( w_event.PassedCutWBosMinus(WBosEvent::sMinElectronPtHard) )
         Fill(ev, kCUT_EVENT_W_MINUS);
   }
}


/** */
void VecBosAsymRootFile::Fill(ProtoEvent &ev, ECut cut)
{
   VecBosRootFile::Fill(ev, cut);
}
