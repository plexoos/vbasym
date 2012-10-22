/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TrackHContainer.h"

#include "TF1.h"

#include "WEvent.h"


ClassImp(TrackHContainer)

using namespace std;


/** Default constructor. */
TrackHContainer::TrackHContainer() : PlotHelper()
{
   BookHists();
}


TrackHContainer::TrackHContainer(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


/** Default destructor. */
TrackHContainer::~TrackHContainer()
{
}


/** */
void TrackHContainer::BookHists()
{
   string shName;
   TH1*   hist;

   fDir->cd();

   o["hNumTracksWithBCluster"] = hist = new TH1I("hNumTracksWithBCluster", "; Num. of Tracks with Barrel Cluster; Events", 5, 0, 5);
   hist->SetOption("hist GRIDX");

   o["hTrackFlag"] = hist = new TH1I("hTrackFlag", "; Track Flag; Num. of Tracks", 60, 280, 340);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackEta"] = hist = new TH1I("hTrackEta", "; Track #eta; Num. of Tracks", 60, -3, 3);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackPhi"] = hist = new TH1I("hTrackPhi", "; Track #phi; Num. of Tracks", 60, -M_PI, M_PI);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackBTowerId"] = hist = new TH1I("hTrackBTowerId", "; Track Extrapolated Barrel Tower Id; Num. of Tracks", 4800, 0, 4800);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackBClusterEnergy2x2"] = hist = new TH1I("hTrackBClusterEnergy2x2", "; Barrel Cluster Energy; Num. of Tracks", 70, 0, 70);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackBClusterEnergy4x4"] = hist = new TH1I("hTrackBClusterEnergy4x4", "; Barrel Cluster (4x4) Energy; Num. of Tracks", 70, 0, 70);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackBClusterEnergyIsoRatio"] = hist = new TH1I("hTrackBClusterEnergyIsoRatio", "; Barrel Cluster Energy Iso Ratio; Num. of Tracks", 55, 0, 1.1);
   hist->SetOption("hist GRIDX GRIDY");

   o["hChargePrimaryTrack"] = hist = new TH1I("hChargePrimaryTrack", "; Charge of the primary track; Num. of Tracks", 10, -2, 2);
   o["hTrackDistanceToCluster"] = hist = new TH1I("hTrackDistanceToCluster", "; Distance(Track-Cluster), cm; Num. of Tracks", 50, 0, 50);
}


/** */
void TrackHContainer::Fill(ProtoEvent &ev)
{
   WEvent& wEvent = (WEvent&) ev;

   VBVertexVecIter iVertex = wEvent.mVertices.begin();

   for ( ; iVertex!=wEvent.mVertices.end(); ++iVertex)
   {
      //VBTrackVecIter iTrack = iVertex->eleTrack.begin();
      vector<WeveEleTrack>::iterator iTrack = iVertex->eleTrack.begin();

      for ( ; iTrack!=iVertex->eleTrack.end(); ++iTrack)
      {
         ((TH1*) o["hTrackFlag"])->Fill(iTrack->prMuTrack->flag());
         ((TH1*) o["hTrackEta"])->Fill(iTrack->primP.Eta());
         ((TH1*) o["hTrackPhi"])->Fill(iTrack->primP.Phi());
         ((TH1*) o["hTrackBTowerId"])->Fill(iTrack->pointTower.id);
         ((TH1*) o["hTrackBClusterEnergy2x2"])->Fill(iTrack->mCluster2x2.ET);
         ((TH1*) o["hTrackBClusterEnergy4x4"])->Fill(iTrack->mCluster4x4.ET);
         ((TH1*) o["hTrackBClusterEnergyIsoRatio"])->Fill(iTrack->mCluster2x2.ET/iTrack->mCluster4x4.ET);
         ((TH1*) o["hTrackDistanceToCluster"])->Fill(iTrack->CalcDistanceToMatchedCluster().Mag());
         ((TH1*) o["hChargePrimaryTrack"])->Fill(iTrack->prMuTrack->charge());

         //printf("hasMatchedCluster: %d\n", iTrack->isMatch2Cl);
      }
   }
}


/** */
void TrackHContainer::FillDerived()
{
   Info("FillDerived()", "Called");
}


/** */
void TrackHContainer::PostFill()
{
   Info("PostFill", "Called");
}
