/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "WBosEventHContainer.h"

#include "TF1.h"

#include "TF2.h"

#include "WBosEvent.h"


ClassImp(WBosEventHContainer)

using namespace std;


/** Default constructor. */
WBosEventHContainer::WBosEventHContainer() : PlotHelper()
{
   BookHists();
}


WBosEventHContainer::WBosEventHContainer(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


/** Default destructor. */
WBosEventHContainer::~WBosEventHContainer()
{
}


/** */
void WBosEventHContainer::BookHists()
{
   string shName;
   TH1*   hist;

   fDir->cd();

   o["hNumCandidateTracks"] = hist = new TH1I("hNumCandidateTracks", "; Num. of Candidate Tracks; Events", 10, 0, 10);
   hist->SetOption("hist GRIDX");

   o["hNumTracksWithBCluster"] = hist = new TH1I("hNumTracksWithBCluster", "; Num. of Tracks with Barrel Cluster; Events", 5, 0, 5);
   hist->SetOption("hist GRIDX");

   o["hNumTracksWithBCluster2"] = hist = new TH1I("hNumTracksWithBCluster2", "; Num. of Tracks with Barrel Cluster; Events", 5, 0, 5);
   hist->SetOption("hist GRIDX");

   o["hJetRecoilPt"]               = hist = new TH1F("hJetRecoilPt", "Recoil from Jets; Jet-based Recoil P_{T}; Events", 40, 0, 40);

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

   o["hPtBalanceFromJets"]        = hist = new TH1F("hPtBalanceFromJets", "Jets; P_{T}-balance from tracks; P_{T};", 40, 0, 60);
   o["hPtBalanceCosPhiFromJets"]  = hist = new TH1F("hPtBalanceCosPhiFromJets", "Jets; P_{T}-balance cos(#phi); P_{T};", 40, -100, 100);
   o["hPhiBalanceCosPhiFromJets"] = hist = new TH1F("hPhiBalanceCosPhiFromJets", "Jets; P_{T}-balance; #phi;", 40, -TMath::Pi(),TMath::Pi());
   o["hBalanceDeltaPhiFromJets"]  = hist = new TH1F("hBalanceDeltaPhiFromJets", "Jets; #Delta #phi;", 40, -TMath::Pi(), TMath::Pi());
   o["hPtBalanceJetsVsElecEt"]    = hist = new TH2F("hPtBalanceJetsVsElecEt","Jets; E_{T}^{electron}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
   hist->SetOption("colz LOGZ");
}


/** */
void WBosEventHContainer::Fill(ProtoEvent &ev)
{
   WBosEvent& event = (WBosEvent&) ev;

   ((TH1*) o["hJetRecoilPt"])->Fill(event.GetJetRecoil().Pt());
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
     ((TH2*) o["hPtBalanceTracksNeutralsVsElecEt"])->Fill( event.GetElectronP3().Pt(), event.mPtBalanceCosPhiFromTracks);
   }

   ((TH1*) o["hPtBalanceFromTracks"])->Fill(event.mP3BalanceFromTracks2.Pt());
   ((TH1*) o["hPtBalanceCosPhiFromTracks"])->Fill(event.mPtBalanceCosPhiFromTracks2);
   ((TH1*) o["hPhiBalanceCosPhiFromTracks"])->Fill(event.mP3BalanceFromTracks2.Phi());
   ((TH1*) o["hBalanceDeltaPhiFromTracks"])->Fill(event.mBalanceDeltaPhiFromTracks2);
   if (event.mTracksCandidate.size() > 0) {
     ((TH2*) o["hPtBalanceTracksVsElecEt"])->Fill(event.GetElectronP3().Pt(), event.mPtBalanceCosPhiFromTracks2);
   }

   ((TH1*) o["hPtBalanceFromJets"])->Fill(event.mP3BalanceFromJets.Pt());
   ((TH1*) o["hPtBalanceCosPhiFromJets"])->Fill(event.mPtBalanceCosPhiFromJets);
   ((TH1*) o["hPhiBalanceCosPhiFromJets"])->Fill(event.mP3BalanceFromJets.Phi());
   ((TH1*) o["hBalanceDeltaPhiFromJets"])->Fill(event.mBalanceDeltaPhiFromJets);
   if (event.mTracksCandidate.size() > 0) {
     ((TH2*) o["hPtBalanceJetsVsElecEt"])->Fill(event.GetElectronP3().Pt(),event.mPtBalanceCosPhiFromJets);
   }
}


/** */
void WBosEventHContainer::FillDerived()
{
   Info("FillDerived()", "Called");
}


/** */
void WBosEventHContainer::PostFill()
{
   Info("PostFill", "Called");
}