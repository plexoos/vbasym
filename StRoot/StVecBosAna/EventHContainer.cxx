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

   o["hNumTracksWithBCluster2"] = hist = new TH1I("hNumTracksWithBCluster2", "; Num. of Tracks with Barrel Cluster; Events", 5, 0, 5);
   hist->SetOption("hist GRIDX");

   o["hJetRecoilPt"]               = hist = new TH1F("hJetRecoilPt", "Recoil from Jets; Jet-based Recoil P_{T}; Events", 40, 0, 40);

   o["hJetRecoilPt_zoomin"]        = hist = new TH1F("hJetRecoilPt_zoomin", "Recoil from Jets; Jet-based Recoil P_{T}; Events", 20, 0, 10);

   o["hTrackRecoilPt"]             = hist = new TH1F("hTrackRecoilPt", "Recoil from Tracks: TPC+TOW; Track-based Recoil P_{T}; Events;", 40, 0, 40);

   o["hTrackRecoilTpcPt"]          = hist = new TH1F("hTrackRecoilTpcPt", "Recoil from Tracks: TPC only; Track-based Recoil P_{T}; Events", 40, 0, 40);

   o["hTrackRecoilWithNeutralsPt"] = hist = new TH1F("hTrackRecoilWithNeutralsPt", "Recoil from Tracks: TPC+emCal (also trackless clusters) ; Track-based Recoil P_{T}; Events", 40, 0, 40);

   o["hTrackRecoilWithNeutralsPtCorrected"] = hist = new TH1F("hTrackRecoilWithNeutralsPtCorrected", "Recoil from Tracks: TPC+emCal (CORRECTED) ; Track-based Recoil P_{T}; Events", 40, 0, 40);

   o["hTrackRecoilWithNeutralsPt_zoomin"] = hist = new TH1F("hTrackRecoilWithNeutralsPt_zoomin", "Recoil from Tracks: TPC+emCal (also trackless clusters) ; Track-based Recoil P_{T}; Events", 20, 0, 10);

   o["hTrackRecoilWithNeutralsPtCorrected_zoomin"] = hist = new TH1F("hTrackRecoilWithNeutralsPtCorrected_zoomin", "Recoil from Tracks: TPC+emCal (CORRECTED) ; Track-based Recoil P_{T}; Events", 20, 0, 10);

   o["hTrackRecoillUntrackedClustersPt"]    = hist = new TH1F("hTrackRecoillUntrackedClustersPt", "Recoil from tracks; P_{T};", 40, 0, 40);
   o["hPtBalanceFromTracksNeutrals"]        = hist = new TH1F("hPtBalanceFromTracksNeutrals", "P_{T}-balance from tracks; P_{T};", 40, 0, 60);
   o["hPtBalanceCosPhiFromTracksNeutrals"]  = hist = new TH1F("hPtBalanceCosPhiFromTracksNeutrals", "P_{T}-balance cos(#phi); P_{T};", 40, -100, 100);
   o["hPhiBalanceCosPhiFromTracksNeutrals"] = hist = new TH1F("hPhiBalanceCosPhiFromTracksNeutrals", "P_{T}-balance; #phi;", 40, -TMath::Pi(),TMath::Pi());
   o["hBalanceDeltaPhiFromTracksNeutrals"]  = hist = new TH1F("hBalanceDeltaPhiFromTracksNeutrals", "; #Delta #phi;", 40, -TMath::Pi(), TMath::Pi());
   o["hPtBalanceTracksNeutralsVsElecEt"]    = hist = new TH2F("hPtBalanceTracksNeutralsVsElecEt","; E_{T}^{electron}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
   hist->SetOption("colz LOGZ");

   o["hPtBalanceFromTracks"]        = hist = new TH1F("hPtBalanceFromTracks", "P_{T}-balance from tracks; P_{T};", 40, 0, 60);
   o["hPtBalanceCosPhiFromTracks"]  = hist = new TH1F("hPtBalanceCosPhiFromTracks", "P_{T}-balance cos(#phi); P_{T};", 40, -100, 100);
   o["hPhiBalanceCosPhiFromTracks"] = hist = new TH1F("hPhiBalanceCosPhiFromTracks", "P_{T}-balance; #phi;", 40, -TMath::Pi(),TMath::Pi());
   o["hBalanceDeltaPhiFromTracks"]  = hist = new TH1F("hBalanceDeltaPhiFromTracks", "; #Delta #phi;", 40, -TMath::Pi(), TMath::Pi());
   o["hPtBalanceTracksVsElecEt"]    = hist = new TH2F("hPtBalanceTracksVsElecEt","; E_{T}^{electron}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
   hist->SetOption("colz LOGZ");

   o["hPtBalanceFromJets"]          = hist = new TH1F("hPtBalanceFromJets", "Jets; P_{T}-balance from tracks; P_{T};", 40, 0, 60);
   o["hPtBalanceCosPhiFromJets"]    = hist = new TH1F("hPtBalanceCosPhiFromJets", "Jets; P_{T}-balance cos(#phi); P_{T};", 40, -100, 100);
   o["hPhiBalanceCosPhiFromJets"]   = hist = new TH1F("hPhiBalanceCosPhiFromJets", "Jets; P_{T}-balance; #phi;", 40, -TMath::Pi(),TMath::Pi());
   o["hBalanceDeltaPhiFromJets"]    = hist = new TH1F("hBalanceDeltaPhiFromJets", "Jets; #Delta #phi;", 40, -TMath::Pi(), TMath::Pi());
   o["hPtBalanceJetsVsElecEt"]      = hist = new TH2F("hPtBalanceJetsVsElecEt","Jets; E_{T}^{electron}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
   hist->SetOption("colz LOGZ");


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
   //((TH1*) o["hNumTracksWithBCluster2"])->Fill(event.GetNumTracksWithBCluster2());

   ((TH1*) o["hJetRecoilPt"])->Fill(event.GetJetRecoil().Pt());
   ((TH1*) o["hJetRecoilPt_zoomin"])->Fill(event.GetJetRecoil().Pt());
   ((TH1*) o["hTrackRecoilPt"])->Fill(event.GetTrackRecoil().Pt());
   ((TH1*) o["hTrackRecoilTpcPt"])->Fill(event.mP3TrackRecoilTpc.Pt());
   ((TH1*) o["hTrackRecoilWithNeutralsPt"])->Fill(event.GetTrackRecoilTpcNeutrals().Pt());
   ((TH1*) o["hTrackRecoilWithNeutralsPtCorrected"])->Fill(event.mPtTrackRecoilWithNeutralsCorrected);
   ((TH1*) o["hTrackRecoilWithNeutralsPt_zoomin"])->Fill(event.GetTrackRecoilTpcNeutrals().Pt());
   ((TH1*) o["hTrackRecoilWithNeutralsPtCorrected_zoomin"])->Fill(event.mPtTrackRecoilWithNeutralsCorrected);
   //((TH1*) o["hTrackRecoilUntrackedClustersPt"])->Fill(event.GetTrackRecoilNeutrals().Pt());

   ((TH1*) o["hPtBalanceFromTracksNeutrals"])->Fill(event.mP3BalanceFromTracks.Pt());
   ((TH1*) o["hPtBalanceCosPhiFromTracksNeutrals"])->Fill(event.mPtBalanceCosPhiFromTracks);
   ((TH1*) o["hPhiBalanceCosPhiFromTracksNeutrals"])->Fill(event.mP3BalanceFromTracks.Phi());
   ((TH1*) o["hBalanceDeltaPhiFromTracksNeutrals"])->Fill(event.mBalanceDeltaPhiFromTracks);
   if (event.mTracksCandidate.size() > 0) {
     ((TH2*) o["hPtBalanceTracksNeutralsVsElecEt"])->Fill( event.GetElectronCandidateP3().Pt(), event.mPtBalanceCosPhiFromTracks);
   }

   ((TH1*) o["hPtBalanceFromTracks"])->Fill(event.mP3BalanceFromTracks2.Pt());
   ((TH1*) o["hPtBalanceCosPhiFromTracks"])->Fill(event.mPtBalanceCosPhiFromTracks2);
   ((TH1*) o["hPhiBalanceCosPhiFromTracks"])->Fill(event.mP3BalanceFromTracks2.Phi());
   ((TH1*) o["hBalanceDeltaPhiFromTracks"])->Fill(event.mBalanceDeltaPhiFromTracks2);
   if (event.mTracksCandidate.size() > 0) {
     ((TH2*) o["hPtBalanceTracksVsElecEt"])->Fill(event.GetElectronCandidateP3().Pt(), event.mPtBalanceCosPhiFromTracks2);
   }

   ((TH1*) o["hPtBalanceFromJets"])->Fill(event.mP3BalanceFromJets.Pt());
   ((TH1*) o["hPtBalanceCosPhiFromJets"])->Fill(event.mPtBalanceCosPhiFromJets);
   ((TH1*) o["hPhiBalanceCosPhiFromJets"])->Fill(event.mP3BalanceFromJets.Phi());
   ((TH1*) o["hBalanceDeltaPhiFromJets"])->Fill(event.mBalanceDeltaPhiFromJets);
   if (event.mTracksCandidate.size() > 0) {
     ((TH2*) o["hPtBalanceJetsVsElecEt"])->Fill(event.GetElectronCandidateP3().Pt(),event.mPtBalanceCosPhiFromJets);
   }

   d["tracks"]->Fill(ev);
}


void EventHContainer::FillTracks(ProtoEvent &ev)
{
   WBosEvent& event = (WBosEvent&) ev;
}


///** */
//void EventHContainer::FillDerived()
//{
//   Info("FillDerived()", "Called");
//}
//
//
///** */
//void EventHContainer::PostFill()
//{
//   Info("PostFill", "Called");
//}
