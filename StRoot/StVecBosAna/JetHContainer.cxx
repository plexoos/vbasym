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

   o["hNumTracksWithBCluster"] = hist = new TH1I("hNumTracksWithBCluster", "; Num. of Tracks with Barrel Cluster; Events", 5, 0, 5);
   hist->SetOption("hist GRIDX");
}


/** */
void JetHContainer::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;

   VecBosTrackVecIter iJet = event.mTracks.begin();

   for ( ; iJet!=event.mTracks.end(); ++iJet)
   {
      //Fill(*iJet);
   }
}


/** */
//void JetHContainer::Fill(VecBosTrack &track)
//{
//   ((TH1*) o["hTrackFlag"])->Fill(track.prMuTrack->flag());
//
//   //printf("hasMatchedCluster: %d\n", track.isMatch2Cl);
//}


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
