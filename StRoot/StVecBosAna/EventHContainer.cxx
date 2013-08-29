/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "EventHContainer.h"

#include "TF1.h"

#include "TF2.h"

#include "WBosEvent.h"
#include "TrackHContainer.h"


ClassImp(EventHContainer)

using namespace std;


/** Default constructor. */
EventHContainer::EventHContainer() : PlotHelper()
{
   BookHists();
}


EventHContainer::EventHContainer(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


/** Default destructor. */
EventHContainer::~EventHContainer()
{
}


/** */
void EventHContainer::BookHists()
{
   string shName;
   TH1*   hist;

   fDir->cd();

   o["hRunId"] = hist = new TH1I("hRunId", "; Run Id; Events", 20, 0, 20);
   hist->SetOption("hist GRIDX");

   o["hZdcRate"] = hist = new TH1I("hZdcRate", "; ZDC Rate; Events", 50, 129e3, 134e3);
   hist->SetOption("hist GRIDX");

   o["hNumJets"] = hist = new TH1I("hNumJets", "; Num. of Jets; Events", 15, 0, 15);
   hist->SetOption("hist GRIDX");

   o["hNumJetsRecoil"] = hist = new TH1I("hNumJetsRecoil", "; Num. of Jets in Recoil; Events", 15, 0, 15);
   hist->SetOption("hist GRIDX");

   o["hNumJetsWithIsoTrack"] = hist = new TH1I("hNumJetsWithIsoTrack", "; Num. of Jets w/ Iso Track; Events", 15, 0, 15);
   hist->SetOption("hist GRIDX");

   o["hNumVertices"] = hist = new TH1I("hNumVertices", "; Num. of Vertices; Events", 10, 0, 10);
   hist->SetOption("hist GRIDX");

   o["hNumGoodVertices"] = hist = new TH1I("hNumGoodVertices", "; Num. of Good Vertices; Events", 10, 0, 10);
   hist->SetOption("hist GRIDX");

   o["hNumTracks"] = hist = new TH1I("hNumTracks", "; Num. of Tracks; Events", 50, 0, 250);
   hist->SetOption("hist GRIDX");

   o["hNumGoodTracks"] = hist = new TH1I("hNumGoodTracks", "; Num. of Good Tracks; Events", 40, 0, 40);
   hist->SetOption("hist GRIDX");

   o["hNumBTracks"] = hist = new TH1I("hNumBTracks", "; Num. of Barrel Tracks; Events", 40, 0, 40);
   hist->SetOption("hist GRIDX");

   o["hNumETracks"] = hist = new TH1I("hNumETracks", "; Num. of Endcap Tracks; Events", 10, 0, 10);
   hist->SetOption("hist GRIDX");

   o["hNumIsolatedTracks"] = hist = new TH1I("hNumIsolatedTracks", "; Num. of Isolated Tracks; Events", 10, 0, 10);
   hist->SetOption("hist GRIDX");

   o["hNumCandidateTracks"] = hist = new TH1I("hNumCandidateTracks", "; Num. of Candidate Tracks; Events", 10, 0, 10);
   hist->SetOption("hist GRIDX");

   o["hNumTracksWithBCluster"] = hist = new TH1I("hNumTracksWithBCluster", "; Num. of Tracks with Barrel Cluster; Events", 5, 0, 5);
   hist->SetOption("hist GRIDX");

   o["hJetRecoilPt"]               = hist = new TH1F("hJetRecoilPt", "Recoil from Jets; Jet-based Recoil P_{T}; Events", 40, 0, 40);
   o["hJetRecoilPt_zoomin"]        = hist = new TH1F("hJetRecoilPt_zoomin", "Recoil from Jets; Jet-based Recoil P_{T}; Events", 20, 0, 10);
   o["hTrackRecoilPt"]             = hist = new TH1F("hTrackRecoilPt", "Recoil from Tracks: TPC+TOW; Track-based Recoil P_{T}; Events;", 40, 0, 40);
   o["hTrackRecoilTpcPt"]          = hist = new TH1F("hTrackRecoilTpcPt", "Recoil from Tracks: TPC only; Track-based Recoil P_{T}; Events", 40, 0, 40);
   o["hTrackRecoilWithNeutralsPt"] = hist = new TH1F("hTrackRecoilWithNeutralsPt", "Recoil from Tracks: TPC+emCal (also trackless clusters) ; Track-based Recoil P_{T}; Events", 40, 0, 40);
   o["hTrackRecoilWithNeutralsPtCorrected"]        = hist = new TH1F("hTrackRecoilWithNeutralsPtCorrected", "Recoil from Tracks: TPC+emCal (CORRECTED) ; Track-based Recoil P_{T}; Events", 40, 0, 40);
   o["hTrackRecoilWithNeutralsPt_zoomin"]          = hist = new TH1F("hTrackRecoilWithNeutralsPt_zoomin", "Recoil from Tracks: TPC+emCal (also trackless clusters) ; Track-based Recoil P_{T}; Events", 20, 0, 10);
   o["hTrackRecoilWithNeutralsPtCorrected_zoomin"] = hist = new TH1F("hTrackRecoilWithNeutralsPtCorrected_zoomin", "Recoil from Tracks: TPC+emCal (CORRECTED) ; Track-based Recoil P_{T}; Events", 20, 0, 10);

   d["tracks"] = new TrackHContainer(new TDirectoryFile("tracks", "tracks", "", fDir));
}


/** */
void EventHContainer::Fill(ProtoEvent &ev)
{
   WBosEvent& event = (WBosEvent&) ev;

   ((TH1*) o["hRunId"])->Fill(event.runNo);
   ((TH1*) o["hZdcRate"])->Fill(event.zdcRate);
   ((TH1*) o["hNumJets"])->Fill(event.GetNumJets());
   ((TH1*) o["hNumJetsRecoil"])->Fill(event.GetNumJetsRecoil());
   ((TH1*) o["hNumJetsWithIsoTrack"])->Fill(event.GetNumJetsWithIsoTrack());
   ((TH1*) o["hNumVertices"])->Fill(event.GetNumVertices());
   ((TH1*) o["hNumGoodVertices"])->Fill(event.GetNumGoodVertices());
   ((TH1*) o["hNumTracks"])->Fill(event.GetNumTracks());
   ((TH1*) o["hNumGoodTracks"])->Fill(event.GetNumGoodTracks());
   ((TH1*) o["hNumBTracks"])->Fill(event.GetNumBTracks());
   ((TH1*) o["hNumETracks"])->Fill(event.GetNumETracks());
   ((TH1*) o["hNumIsolatedTracks"])->Fill(event.GetNumIsolatedTracks());
   ((TH1*) o["hNumCandidateTracks"])->Fill(event.GetNumCandidateTracks());
   ((TH1*) o["hNumTracksWithBCluster"])->Fill(event.GetNumTracksWithBCluster());

   ((TH1*) o["hJetRecoilPt"])->Fill(event.GetJetRecoil().Pt());
   ((TH1*) o["hJetRecoilPt_zoomin"])->Fill(event.GetJetRecoil().Pt());
   ((TH1*) o["hTrackRecoilPt"])->Fill(event.GetTrackRecoil().Pt());
   ((TH1*) o["hTrackRecoilTpcPt"])->Fill(event.mP3TrackRecoilTpc.Pt());
   ((TH1*) o["hTrackRecoilWithNeutralsPt"])->Fill(event.GetTrackRecoilTpcNeutrals().Pt());
   ((TH1*) o["hTrackRecoilWithNeutralsPtCorrected"])->Fill(event.mPtTrackRecoilWithNeutralsCorrected);
   ((TH1*) o["hTrackRecoilWithNeutralsPt_zoomin"])->Fill(event.GetTrackRecoilTpcNeutrals().Pt());
   ((TH1*) o["hTrackRecoilWithNeutralsPtCorrected_zoomin"])->Fill(event.mPtTrackRecoilWithNeutralsCorrected);

   d["tracks"]->Fill(ev);
}


void EventHContainer::FillTracks(ProtoEvent &ev)
{
   //WBosEvent& event = (WBosEvent&) ev;
}
