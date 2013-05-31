/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TrackHContainer.h"

#include "TF1.h"

#include "TF2.h"

#include "VecBosEvent.h"


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

   o["hTrackVbType"] = hist = new TH1I("hTrackVbType", "; Track vbType; Num. of Tracks", 40, 0, 40);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackEta"] = hist = new TH1I("hTrackEta", "; Track #eta; Num. of Tracks", 60, -3, 3);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackPhi"] = hist = new TH1I("hTrackPhi", "; Track #phi; Num. of Tracks", 60, -M_PI, M_PI);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackEtaAtBTow"] = hist = new TH1I("hTrackEtaAtBTow", "; Track #eta at BTOW; Num. of Tracks", 60, -3, 3);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackPhiAtBTow"] = hist = new TH1I("hTrackPhiAtBTow", "; Track #phi at BTOW; Num. of Tracks", 60, -M_PI, M_PI);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackPt"] = hist = new TH1F("hTrackPt", "; Track P_T; Num. of Tracks", 80, 0, 80);
   hist->SetOption("hist GRIDX GRIDY XY");

   o["hEcalScaledPt"] = hist = new TH1F("hEcalScaledPt", "; P_T; Num. of Tracks", 80, 0, 80);
   hist->SetOption("hist GRIDX GRIDY XY");

   o["hTrackEOverP"] = hist = new TH1I("hTrackEOverP", "; E/P; Num. of Tracks", 50, 0, 2);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackHitsFit"] = hist = new TH1I("hTrackHitsFit", "; Track Num. of Fit Hits; Num. of Tracks", 50, 0, 50);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackHitsPoss"] = hist = new TH1I("hTrackHitsPoss", "; Track Num. of Possible Hits; Num. of Tracks", 50, 0, 50);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackBTowerId"] = hist = new TH1I("hTrackBTowerId", "; Track Extrapolated Barrel Tower Id; Num. of Tracks", 4800, 0, 4800);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackCluster2x2E"] = hist = new TH1I("hTrackCluster2x2E", "; Barrel Cluster Energy; Num. of Tracks", 70, 0, 70);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackCluster2x2Et"] = hist = new TH1I("hTrackCluster2x2Et", "; Barrel Cluster E_T; Num. of Tracks", 70, 0, 70);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackCluster4x4E"] = hist = new TH1I("hTrackCluster4x4E", "; Barrel Cluster (4x4) Energy; Num. of Tracks", 70, 0, 70);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackCluster4x4Et"] = hist = new TH1I("hTrackCluster4x4Et", "; Barrel Cluster (4x4) E_T; Num. of Tracks", 70, 0, 70);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackBClusterEnergyIsoRatio"] = hist = new TH1I("hTrackBClusterEnergyIsoRatio", "; Barrel Cluster Energy Iso Ratio; Num. of Tracks", 55, 0, 1.1);
   hist->SetOption("hist GRIDX GRIDY");

   o["hTrackClusterEnergyFrac"] = hist = new TH1I("hTrackClusterEnergyFrac", "; Isolation, Cluster Energy Frac.; Num. of Tracks", 55, 0, 1.1);
   hist->SetOption("hist GRIDX GRIDY XY");

   o["hChargePrimaryTrack"] = hist = new TH1I("hChargePrimaryTrack", "; Charge of the primary track; Num. of Tracks", 10, -2, 2);
   o["hQoPt_PrimaryTrack"] = hist = new TH1F("hQoPt_PrimaryTrack", "; Q/P_{T} of the primary track; Num. of Tracks", 50, -0.1, 0.1);

   o["hQoPt_Vs_Et_PrimaryTrack"] = hist = new TH2F("hQoPt_Vs_Et_PrimaryTrack", "Primary track; E_{T} 2x2 cluster; Q/P_{T}", 50, 0, 80, 50, -0.1, 0.1);
   hist->SetOption("colz LOGZ");

   o["hQxEtoPt_Vs_Et_PrimaryTrack"] = hist = new TH2F("hQxEtoPt_Vs_Et_PrimaryTrack", "Primary track; E_{T} 2x2 cluster; Q(TPC)*E_{T}(EMC)/P_{T}(TPC)", 50, 0, 80, 50, -3, 3);
   hist->SetOption("colz LOGZ");

   o["hTrackDistanceToCluster"] = hist = new TH1I("hTrackDistanceToCluster", "; Distance(Track-Cluster), cm; Num. of Tracks", 50, 0, 50);

   o["hMinDeltaRToJet"] = hist = new TH1I("hMinDeltaRToJet", "; ; Num. of Tracks", 40, 0, 4);
   hist->SetOption("hist GRIDX GRIDY");
}


/** */
void TrackHContainer::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;

   VecBosTrackPtrSetIter iTrack = event.mTracks.begin();

   for ( ; iTrack!=event.mTracks.end(); ++iTrack)
   {
      Fill(**iTrack);
   }
}


/** */
void TrackHContainer::Fill(VecBosTrack &track)
{
   ((TH1*) o["hTrackFlag"])->Fill(track.prMuTrack->flag());
   ((TH1*) o["hTrackVbType"])->Fill(track.mVbType);
   ((TH1*) o["hTrackEta"])->Fill(track.mP3AtDca.Eta());
   ((TH1*) o["hTrackPhi"])->Fill(track.mP3AtDca.Phi());

   if (track.IsBTrack()) {
      //printf("", mP3AtBTow);
      //Info("Fill", "XXX");
      //printf("mVbType: %x\n", track.mVbType);
      //track.mP3AtDca.Print();
      //track.mP3AtBTow.Print();
      ((TH1*) o["hTrackEtaAtBTow"])->Fill(track.mCoorAtBTow.Eta());
      ((TH1*) o["hTrackPhiAtBTow"])->Fill(track.mCoorAtBTow.Phi());
   }

   ((TH1*) o["hTrackPt"])->Fill(track.mP3AtDca.Pt());
   ((TH1*) o["hEcalScaledPt"])->Fill(track.GetP3EScaled().Pt());
   ((TH1*) o["hTrackEOverP"])->Fill(track.mCluster2x2.energy/track.mP3AtDca.Mag());
   ((TH1*) o["hTrackHitsFit"])->Fill(track.prMuTrack->nHitsFit());
   ((TH1*) o["hTrackHitsPoss"])->Fill(track.prMuTrack->nHitsPoss());
   ((TH1*) o["hTrackBTowerId"])->Fill(track.mMatchedTower.id);
   ((TH1*) o["hTrackCluster2x2E"])->Fill(track.mCluster2x2.energy);
   ((TH1*) o["hTrackCluster2x2Et"])->Fill(track.mCluster2x2.ET);
   ((TH1*) o["hTrackCluster4x4E"])->Fill(track.mCluster4x4.energy);
   ((TH1*) o["hTrackCluster4x4Et"])->Fill(track.mCluster4x4.ET);
   ((TH1*) o["hTrackBClusterEnergyIsoRatio"])->Fill(track.mCluster2x2.ET/track.mCluster4x4.ET);
   ((TH1*) o["hTrackClusterEnergyFrac"])->Fill(track.GetClusterEnergyFrac());
   ((TH1*) o["hTrackDistanceToCluster"])->Fill(track.CalcDistanceToCluster().Mag());
   ((TH1*) o["hChargePrimaryTrack"])->Fill(track.prMuTrack->charge());
   ((TH1*) o["hQoPt_PrimaryTrack"])->Fill(track.prMuTrack->charge()/track.GetP3EScaled().Pt());
   //  ((TH2*) o["hQoPt_Vs_Et_PrimaryTrack"])->Fill(track.mCluster2x2.ET, (track.prMuTrack->charge()/track.GetP3EScaled().Pt()));
   ((TH2*) o["hQoPt_Vs_Et_PrimaryTrack"])->Fill(track.mCluster2x2.ET, (track.prMuTrack->charge()/track.mP3AtDca.Pt()));
   ((TH2*) o["hQxEtoPt_Vs_Et_PrimaryTrack"])->Fill(track.mCluster2x2.ET, (track.prMuTrack->charge()*track.mCluster2x2.ET)/track.mP3AtDca.Pt());

   ((TH1*) o["hMinDeltaRToJet"])->Fill(track.mMinDeltaRToJet);

   //printf("hasMatchedCluster: %d\n", track.isMatch2Cl);
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
