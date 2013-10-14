#include "VecBosAsymRootFile.h"

#include <climits>
#include <sstream>

#include "TROOT.h"
#include "TStyle.h"

#include "St_db_Maker/St_db_Maker.h"
#include "StEmcUtil/database/StBemcTables.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

#include "AsymHContainer.h"
#include "EventHContainer.h"
#include "EventDisplayHContainer.h"
#include "WBosEventHContainer.h"
#include "MCHContainer.h"
#include "WBosEvent.h"
#include "VecBosTrack.h"
#include "VecBosVertex.h"

#include "utils/utils.h"

using namespace std;


VecBosAsymRootFile::VecBosAsymRootFile() : VecBosRootFile()
{
   BookHists();
}


VecBosAsymRootFile::VecBosAsymRootFile(const char *fname, Option_t *option, Bool_t isMc, Bool_t isZ, const char *ftitle, Int_t compress) :
  VecBosRootFile(fname, option, isMc, isZ, ftitle, compress)
{
   gBTowGeom = StEmcGeom::instance("bemc");
   gBTowGeom->printGeom();
   BookHists();
}


VecBosAsymRootFile::~VecBosAsymRootFile()
{
   //if (fHists) { delete fHists; fHists = 0; }
}


void VecBosAsymRootFile::BookHists()
{
   // Delete histograms created in parent class
   if (fHists) { delete fHists; fHists = 0; }
   fHistCuts.clear();

   PlotHelper *ph;

   fHists = new PlotHelper(this);

   fHists->d["event"] = ph = new EventHContainer(new TDirectoryFile("event", "event", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["event.cut_w"] = ph = new EventHContainer(new TDirectoryFile("event.cut_w", "event.cut_w", "", this));
   fHistCuts[kCUT_EVENT_W].insert(ph);

   fHists->d["event_w"] = ph = new WBosEventHContainer(new TDirectoryFile("event_w", "event_w", "", this));
   fHistCuts[kCUT_EVENT_W].insert(ph);

   fHists->d["event_display"] = ph = new EventDisplayHContainer(new TDirectoryFile("event_display", "event_display", "", this));
   fHistCuts[kCUT_EVENT_W].insert(ph);

   fHists->d["asym_w"] = ph = new AsymHContainer(new TDirectoryFile("asym_w", "asym_w", "", this));
   fHistCuts[kCUT_EVENT_W].insert(ph);

   fHists->d["event_wp"] = ph = new WBosEventHContainer(new TDirectoryFile("event_wp", "event_wp", "", this));
   fHistCuts[kCUT_EVENT_W_PLUS].insert(ph);

   fHists->d["asym_wp"] = ph = new AsymHContainer(new TDirectoryFile("asym_wp", "asym_wp", "", this));
   fHistCuts[kCUT_EVENT_W_PLUS].insert(ph);

   fHists->d["asym_wp_phys"] = ph = new AsymHContainer(new TDirectoryFile("asym_wp_phys", "asym_wp_phys", "", this), kAsymSqrtPhys);
   fHistCuts[kCUT_EVENT_W_PLUS].insert(ph);

   fHists->d["asym_wp_geom"] = ph = new AsymHContainer(new TDirectoryFile("asym_wp_geom", "asym_wp_geom", "", this), kAsymSqrtGeom);
   fHistCuts[kCUT_EVENT_W_PLUS].insert(ph);

   fHists->d["asym_wp_lumi"] = ph = new AsymHContainer(new TDirectoryFile("asym_wp_lumi", "asym_wp_lumi", "", this), kAsymSqrtLumi);
   fHistCuts[kCUT_EVENT_W_PLUS].insert(ph);

   fHists->d["event_wm"] = ph = new WBosEventHContainer(new TDirectoryFile("event_wm", "event_wm", "", this));
   fHistCuts[kCUT_EVENT_W_MINUS].insert(ph);

   fHists->d["asym_wm"] = ph = new AsymHContainer(new TDirectoryFile("asym_wm", "asym_wm", "", this));
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
