
#include "VecBosAsymRootFile.h"

#include <climits>
#include <sstream>

#include "TROOT.h"
#include "TStyle.h"

#include "AsymHContainer.h"
#include "MCHContainer.h"
#include "VecBosEvent.h"
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

   fHists->d["asym"] = ph = new AsymHContainer(new TDirectoryFile("asym", "asym", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["asym_final"] = ph = new AsymHContainer(new TDirectoryFile("asym_final", "asym_final", "", this));
   fHistCuts[kCUT_EVENT_PASS_FINAL].insert(ph);

   if (!fIsMc) return;

   this->cd();
}


/** */
void VecBosAsymRootFile::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;
   //VecBosEvent *event = (VecBosEvent *) &ev;

   Fill(ev, kCUT_EVENT_NOCUT);

   if ( event.PassCutFinal() )
      Fill(ev, kCUT_EVENT_PASS_FINAL);

}


/** */
void VecBosAsymRootFile::Fill(ProtoEvent &ev, ECut cut)
{
   VecBosRootFile::Fill(ev, cut);
}
