/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "JetHContainer.h"

#include "TF1.h"

#include "VecBosEvent.h"


ClassImp(JetHContainer)

using namespace std;


/** Default constructor. */
JetHContainer::JetHContainer() : PlotHelper()
{
   BookHists();
}


JetHContainer::JetHContainer(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


/** Default destructor. */
JetHContainer::~JetHContainer()
{
}


/** */
void JetHContainer::BookHists()
{
   string shName;
   TH1*   hist;

   fDir->cd();

   o["hJetEta"] = hist = new TH1I("hJetEta", "; Jet #eta; Num. of Jets", 60, -3, 3);
   hist->SetOption("hist GRIDX GRIDY");

   o["hJetPhi"] = hist = new TH1I("hJetPhi", "; Jet #phi; Num. of Jets", 60, -M_PI, M_PI);
   hist->SetOption("hist GRIDX GRIDY");

   o["hJetPt"] = hist = new TH1I("hJetPt", "; Jet P_T; Num. of Jets", 60, 0, 60);
   hist->SetOption("hist GRIDX GRIDY XY");
}


/** */
void JetHContainer::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;

   StJetPtrSetIter iJet = event.mJets.begin();

   for ( ; iJet!=event.mJets.end(); ++iJet)
   {
      Fill(**iJet);
   }
}


/** */
void JetHContainer::Fill(StJet &stJet)
{
   ((TH1*) o["hJetEta"])->Fill(stJet.Eta());
   ((TH1*) o["hJetPhi"])->Fill(stJet.Phi());
   ((TH1*) o["hJetPt"])->Fill(stJet.Pt());

   //printf("hasMatchedCluster: %d\n", stJet.isMatch2Cl);
}


/** */
void JetHContainer::FillDerived()
{
   Info("FillDerived()", "Called");
}


/** */
void JetHContainer::PostFill()
{
   Info("PostFill", "Called");
}
