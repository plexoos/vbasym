/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "MCHContainer.h"

#include "TF1.h"
#include "TProfile.h"
#include "TText.h"

#include "WBosEvent.h"
#include "StVecBosMaker.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

//need these to get MC record
#include "tables/St_g2t_tpc_hit_Table.h"
#include "StMcEventMaker/StMcEventMaker.h"
#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcVertex.hh"
#include "StMcEvent/StMcTrack.hh"

#include "Globals.h"

#include "utils/H2I.h"

ClassImp(MCHContainer)

using namespace std;


/** Default constructor. */
MCHContainer::MCHContainer() : PlotHelper()
{
   BookHists();
}


MCHContainer::MCHContainer(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


/** */
void MCHContainer::BookHists()
{
   string shName;
   TH1*   hist;

   fDir->cd();

   o["hWBosonE"]                  = hist = new TH1F("hWBosonE", "; W energy [GeV]; Events", 100, 20., 200.);
   hist->SetOption("hist GRIDX");
   o["hWBosonPz"]                 = hist = new TH1F("hWBosonPz", "; P^{W}_{z} [GeV/c]; Events", 80, -40., 40.);
   hist->SetOption("hist GRIDX");
   o["hWBosonPt"]                 = hist = new TH1F("hWBosonPt", "; P^{W}_{T} [GeV/c]; Events", 40, 0., 40.);
   o["hWBosonPt_zoomin"]          = hist = new TH1F("hWBosonPt_zoomin", "; P^{W}_{T} [GeV/c]; Events", 20, 0., 10.);
   hist->SetOption("hist GRIDX");
   o["hWBosonEta"]                = hist = new TH1F("hWBosonEta", "; W pseudo-rapidity #eta; Events", 60, -6, 6);
   hist->SetOption("hist GRIDX");

   o["hNeutrinoE"]           = hist = new TH1I("hNeutrinoE", "; #nu energy [GeV]; Events", 50, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoPx"]        = hist = new TH1I("hNeutrinoPx", "; P^{#nu}_{x} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoPy"]        = hist = new TH1I("hNeutrinoPy", "; P^{#nu}_{y} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoPz"]        = hist = new TH1I("hNeutrinoPz", "; P^{#nu}_{z} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoPt"]               = hist = new TH1I("hNeutrinoPt", "; P^{#nu}_{T} [GeV/c]; Events", 100, 0., 150.);
   hist->SetOption("hist GRIDX");

   o["hLeptonE"]                  = hist = new TH1I("hLeptonE", "; electron energy [GeV]; Events", 80, 0., 80.);
   hist->SetOption("hist GRIDX");
   o["hLeptonPx"]                 = hist = new TH1I("hLeptonPx", "; P^{#nu}_{x} [GeV/c]; Events", 80, -80., 80.);
   hist->SetOption("hist GRIDX");
   o["hLeptonPy"]                 = hist = new TH1I("hLeptonPy", "; P^{#nu}_{y} [GeV/c]; Events", 80, -80., 80.);
   hist->SetOption("hist GRIDX");
   o["hLeptonPz"]                 = hist = new TH1I("hLeptonPz", "; P^{#nu}_{z} [GeV/c]; Events", 80, -80., 80.);
   hist->SetOption("hist GRIDX");
   o["hLeptonPt"]                 = hist = new TH1I("hLeptonPt", "; P^{#nu}_{T} [GeV/c]; Events", 80, 0., 80.);
   hist->SetOption("hist GRIDX");

   o["hRecoVsGenLeptonPhi"]   = hist = new TH2I("hRecoVsGenLeptonPhi", "; Gen. Lepton #phi; Reco. Lepton #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenLeptonPt"]    = hist = new TH2I("hRecoVsGenLeptonPt",  "; Gen. Lepton P_{T}; Reco. Lepton P_{T}", 50, 10, 60, 50, 10, 60);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenLeptonPz"]    = hist = new TH2I("hRecoVsGenLeptonPz",  "; Gen. Lepton P_{z}; Reco. Lepton P_{z}", 50, -50, 50, 50, -50, 50);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenLeptonEta"]   = hist = new TH2I("hRecoVsGenLeptonEta", "; Gen. Lepton #eta; Reco. Lepton #eta", 50, -2, 2, 50, -2, 2);
   hist->SetOption("colz LOGZ");

   o["hRecoVsGenNeutrinoPhi"] = hist = new TH2I("hRecoVsGenNeutrinoPhi", "; Gen. Neutrino #phi; Reco. Neutrino #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenNeutrinoPt"]  = hist = new TH2I("hRecoVsGenNeutrinoPt",  "; Gen. Neutrino P_{T}; Reco. Neutrino P_{T}", 50, 10, 60, 50, 10, 60);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenNeutrinoPz"]  = hist = new TH2I("hRecoVsGenNeutrinoPz",  "; Gen. Neutrino P_{z}; Reco. Neutrino P_{z}", 50, -70, 70, 50, -70, 70);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenNeutrinoEta"] = hist = new TH2I("hRecoVsGenNeutrinoEta", "; Gen. Neutrino #eta; Reco. Neutrino #eta", 50, -2, 2, 50, -2, 2);
   hist->SetOption("colz LOGZ");

   o["hRecoVsGenWBosonPhi"] = hist = new TH2I("hRecoVsGenWBosonPhi", "; Gen. W Boson #phi; Reco. W Boson #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenWBosonPt"]  = hist = new TH2I("hRecoVsGenWBosonPt",  "; Gen. W Boson P_{T}; Reco. W Boson P_{T}", 50, 0, 25, 50, 0, 25);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenWBosonPz"]  = hist = new TH2I("hRecoVsGenWBosonPz",  "; Gen. W Boson P_{z}; Reco. W Boson P_{z}", 50, -80, 80, 50, -80, 80);
   hist->SetOption("colz LOGZ");
   o["hRecoVsGenWBosonEta"] = hist = new TH2I("hRecoVsGenWBosonEta", "; Gen. W Boson #eta; Reco. W Boson #eta", 50, -6, 6, 50, -6, 6);
   hist->SetOption("colz LOGZ");

   // recoil
   o["hRecoilE"]                  = hist = new TH1I("hRecoilE", "; Recoil energy [GeV]; Events", 100, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hRecoilPx"]                 = hist = new TH1I("hRecoilPx", "; P^{Recoil}_{x} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecoilPy"]                 = hist = new TH1I("hRecoilPy", "; P^{Recoil}_{y} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecoilPz"]                 = hist = new TH1I("hRecoilPz", "; P^{Recoil}_{z} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecoilPt"]                 = hist = new TH1I("hRecoilPt", "; P^{Recoil}_{t} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");

   //recoil in acceptance
   o["hRecInAccEnergy"]           = hist = new TH1I("hRecInAccEenergy", "; Recoil energy [GeV]; Events", 100, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hRecInAccMomentumX"]        = hist = new TH1I("hRecInAccMomentumX", "; P^{Recoil}_{x} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecInAccMomentumY"]        = hist = new TH1I("hRecInAccMomentumY", "; P^{Recoil}_{y} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecInAccMomentumZ"]        = hist = new TH1I("hRecInAccMomentumZ", "; P^{Recoil}_{z} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecoilInAcceptPt"]         = hist = new TH1I("hRecoilInAcceptPt", "; P^{Recoil}_{t} [GeV/c]; Events", 50, 0., 50.);
   hist->SetOption("hist GRIDX");

   //recoil outside of detector acceptance
   o["hRecOutAccEnergy"]          = hist = new TH1I("hRecOutAccEenergy", "; Recoil energy [GeV]; Events", 100, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hRecOutAccMomentumX"]       = hist = new TH1I("hRecOutAccMomentumX", "; P^{Recoil}_{x} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecOutAccMomentumY"]       = hist = new TH1I("hRecOutAccMomentumY", "; P^{Recoil}_{y} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecOutAccMomentumZ"]       = hist = new TH1I("hRecOutAccMomentumZ", "; P^{Recoil}_{z} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecOutAccPt"]              = hist = new TH1I("hRecOutAccPt", "; P^{Recoil}_{t} [GeV/c]; Events", 50, 0., 50.);
   hist->SetOption("hist GRIDX");

   o["hJetRecoilPtVsWBosonPt"]      = hist = new TH2I("hJetRecoilPtVsWBosonPt", "; W Boson P_{T}; Jet-based Recoil P_{T}", 50, 0., 50., 50, 0., 50.);
   hist->SetOption("colz LOGZ");

   o["hJetRecoilPt_GenOverReco"]   = hist = new TH2F("hJetRecoilPt_GenOverReco", ";Jet-based Recoil P_{T}; Correction factor",40, 0, 40, 50, 0, 20);
   hist->SetOption("colz LOGZ");

   o["hJetRecoilPt_GenOverReco_zoomin"]   = hist = new TH2F("hJetRecoilPt_GenOverReco_zoomin", ";Jet-based Recoil P_{T}; Correction factor",20, 0, 10, 50, 0, 20);
   hist->SetOption("colz LOGZ");

   o["hJetRecoilPhiVsWBosonPhi"]    = hist = new TH2I("hJetRecoilPhiVsWBosonPhi", "; W Boson #phi; Jet-based Recoil #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI);
   hist->SetOption("colz LOGZ");

   o["hTrackRecoilPtVsWBosonPt"]    = hist = new TH2I("hTrackRecoilPtVsWBosonPt", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50.);
   hist->SetOption("colz LOGZ");

   o["hTrackRecoilPhiVsWBosonPhi"]  = hist = new TH2I("hTrackRecoilPhiVsWBosonPhi", "; W Boson #phi; Track-based Recoil #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI);
   hist->SetOption("colz LOGZ");

   o["hTrackRecoilTpcPtVsWBosonPt"] = hist = new TH2I("hTrackRecoilTpcPtVsWBosonPt", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50.);
   hist->SetOption("colz LOGZ");

   o["hTrackRecoilTpcNeutralsPtVsWBosonPt"] = hist = new TH2I("hTrackRecoilTpcNeutralsPtVsWBosonPt", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50.);
   hist->SetOption("colz LOGZ");
   o["hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt"] = hist = new TH2I("hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt", "; W Boson P_{T}; Corrected Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50.);
   hist->SetOption("colz LOGZ");

   o["hTrackRecoilTpcNeutralsPt_GenOverReco"]   = hist = new TH2F("hTrackRecoilTpcNeutralsPt_GenOverReco", ";Track-based Recoil P_{T}; Correction factor",40, 0, 40, 50, 0, 20);
   hist->SetOption("colz LOGZ");

   o["hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin"]   = hist = new TH2F("hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin", "; Track-based Recoil P_{T}; Correction factor",20, 0, 10, 50, 0, 20 );
   hist->SetOption("colz LOGZ");

   o["hJetRecoilPtRelDiff"]   = hist = new TH1F("hJetRecoilPtRelDiff", "; Jet-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2.);
   hist->SetOption("hist");

   o["hTrackRecoilPtRelDiff"] = hist = new TH1F("hTrackRecoilPtRelDiff", "; Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2.);
   hist->SetOption("hist");

   o["hTrackRecoilTpcNeutralsPtRelDiff"] = hist = new TH1F("hTrackRecoilTpcNeutralsPtRelDiff", "; Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2.);
   hist->SetOption("hist");

   o["hJetTrackRecoilPtRelDiff"] = hist = new TH1F("hJetTrackRecoilPtRelDiff", "; Jet- Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2.);
   hist->SetOption("hist");

   o["hJetTrackTpcNeutralsRecoilPtRelDiff"] = hist = new TH1F("hJetTrackTpcNeutralsRecoilPtRelDiff", "; Jet- Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2.);
   hist->SetOption("hist");
}


void MCHContainer::Fill(ProtoEvent &ev)
{
   WBosEvent& event = (WBosEvent&) ev;

   ((TH1*) o["hWBosonE"]) ->Fill(event.mWEvent->mP4WBoson.E());
   ((TH1*) o["hWBosonPz"])->Fill(event.mWEvent->mP4WBoson.Pz());
   ((TH1*) o["hWBosonPt"])->Fill(event.mWEvent->mP4WBoson.Pt());
   ((TH1*) o["hWBosonPt_zoomin"])->Fill(event.mWEvent->mP4WBoson.Pt());

   if (event.mWEvent->mP4WBoson.Mag() > 0)
      ((TH1*) o["hWBosonEta"])->Fill(event.mWEvent->mP4WBoson.Eta());
   else
      ((TH1*) o["hWBosonEta"])->Fill(1e5);

   ((TH1*) o["hNeutrinoE"]) ->Fill(event.mWEvent->mP4Neutrino.E());
   ((TH1*) o["hNeutrinoPx"])->Fill(event.mWEvent->mP4Neutrino.Px());
   ((TH1*) o["hNeutrinoPy"])->Fill(event.mWEvent->mP4Neutrino.Py());
   ((TH1*) o["hNeutrinoPz"])->Fill(event.mWEvent->mP4Neutrino.Pz());
   ((TH1*) o["hNeutrinoPt"])->Fill(event.mWEvent->mP4Neutrino.Pz());

   ((TH1*) o["hLeptonE"]) ->Fill(event.mWEvent->mP4Lepton.E());
   ((TH1*) o["hLeptonPx"])->Fill(event.mWEvent->mP4Lepton.Px());
   ((TH1*) o["hLeptonPy"])->Fill(event.mWEvent->mP4Lepton.Py());
   ((TH1*) o["hLeptonPz"])->Fill(event.mWEvent->mP4Lepton.Pz());
   ((TH1*) o["hLeptonPt"])->Fill(event.mWEvent->mP4Lepton.Pt());

   ((TH1*) o["hRecoVsGenLeptonPhi"])  ->Fill(event.mWEvent->mP4Lepton.Phi(),   event.GetElectronP3().Phi());
   ((TH1*) o["hRecoVsGenLeptonPt"])   ->Fill(event.mWEvent->mP4Lepton.Pt(),    event.GetElectronP3().Pt());
   ((TH1*) o["hRecoVsGenLeptonPz"])   ->Fill(event.mWEvent->mP4Lepton.Pz(),    event.GetElectronP3().Pz());
   ((TH1*) o["hRecoVsGenLeptonEta"])  ->Fill(event.mWEvent->mP4Lepton.Eta(),   event.GetElectronP3().Eta());

   ((TH1*) o["hRecoVsGenNeutrinoPhi"])->Fill(event.mWEvent->mP4Neutrino.Phi(), event.GetNeutrinoP3().Phi());
   ((TH1*) o["hRecoVsGenNeutrinoPt"]) ->Fill(event.mWEvent->mP4Neutrino.Pt(),  event.GetNeutrinoP3().Pt());
   ((TH1*) o["hRecoVsGenNeutrinoPz"]) ->Fill(event.mWEvent->mP4Neutrino.Pz(),  event.GetNeutrinoP3().Pz());
   ((TH1*) o["hRecoVsGenNeutrinoEta"])->Fill(event.mWEvent->mP4Neutrino.Eta(), event.GetNeutrinoP3().Eta());

   ((TH1*) o["hRecoVsGenWBosonPhi"])  ->Fill(event.mWEvent->mP4WBoson.Phi(),   event.GetVecBosonP3().Phi());
   ((TH1*) o["hRecoVsGenWBosonPt"])   ->Fill(event.mWEvent->mP4WBoson.Pt(),    event.GetVecBosonP3().Pt());
   ((TH1*) o["hRecoVsGenWBosonPz"])   ->Fill(event.mWEvent->mP4WBoson.Pz(),    event.GetVecBosonP3().Pz());
   ((TH1*) o["hRecoVsGenWBosonEta"])  ->Fill(event.mWEvent->mP4WBoson.Eta(),   event.GetVecBosonP3().Eta());

   // recoil variables
   ((TH1*) o["hRecoilE"])->Fill(event.mWEvent->mP4Recoil.E());
   ((TH1*) o["hRecoilPx"])->Fill(event.mWEvent->mP4Recoil.Px());
   ((TH1*) o["hRecoilPy"])->Fill(event.mWEvent->mP4Recoil.Py());
   ((TH1*) o["hRecoilPz"])->Fill(event.mWEvent->mP4Recoil.Pz());
   ((TH1*) o["hRecInAccEnergy"])->Fill(event.mWEvent->mP4RecoilInAccept.E());
   ((TH1*) o["hRecInAccMomentumX"]) ->Fill(event.mWEvent->mP4RecoilInAccept.Px());
   ((TH1*) o["hRecInAccMomentumY"]) ->Fill(event.mWEvent->mP4RecoilInAccept.Py());
   ((TH1*) o["hRecInAccMomentumZ"]) ->Fill(event.mWEvent->mP4RecoilInAccept.Pz());
   ((TH1*) o["hRecoilInAcceptPt"])  ->Fill(event.mWEvent->mP4RecoilInAccept.Pt());
   ((TH1*) o["hRecOutAccEnergy"])   ->Fill(event.mWEvent->mP4RecoilOutAccept.E());
   ((TH1*) o["hRecOutAccMomentumX"])->Fill(event.mWEvent->mP4RecoilOutAccept.Px());
   ((TH1*) o["hRecOutAccMomentumY"])->Fill(event.mWEvent->mP4RecoilOutAccept.Py());
   ((TH1*) o["hRecOutAccMomentumZ"])->Fill(event.mWEvent->mP4RecoilOutAccept.Pz());
   ((TH1*) o["hRecOutAccPt"])       ->Fill(event.mWEvent->mP4RecoilOutAccept.Pt());

   ((TH2*) o["hJetRecoilPtVsWBosonPt"])                      ->Fill(event.mWEvent->mP4WBoson.Pt(),  event.GetJetRecoil().Pt());
   ((TH2*) o["hJetRecoilPhiVsWBosonPhi"])                    ->Fill(event.mWEvent->mP4WBoson.Phi(), event.GetJetRecoil().Phi());
   ((TH2*) o["hJetRecoilPt_GenOverReco"])                    ->Fill(event.GetJetRecoil().Pt(), event.mWEvent->mP4WBoson.Pt()/event.GetJetRecoil().Pt());
   ((TH2*) o["hJetRecoilPt_GenOverReco_zoomin"])             ->Fill(event.GetJetRecoil().Pt(), event.mWEvent->mP4WBoson.Pt()/event.GetJetRecoil().Pt());
   ((TH2*) o["hTrackRecoilPtVsWBosonPt"])                    ->Fill(event.mWEvent->mP4WBoson.Pt(),  event.GetTrackRecoil().Pt());
   ((TH2*) o["hTrackRecoilPhiVsWBosonPhi"])                  ->Fill(event.mWEvent->mP4WBoson.Phi(), event.GetTrackRecoil().Phi());
   ((TH2*) o["hTrackRecoilTpcPtVsWBosonPt"])                 ->Fill(event.mWEvent->mP4WBoson.Pt(),  event.mP3TrackRecoilTpc.Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPtVsWBosonPt"])         ->Fill(event.mWEvent->mP4WBoson.Pt(),  event.GetTrackRecoilTpcNeutrals().Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt"])->Fill(event.mWEvent->mP4WBoson.Pt(),  event.GetTrackRecoilTpcNeutralsCorrected().Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPt_GenOverReco"])       ->Fill(event.GetTrackRecoilTpcNeutrals().Pt(), event.mWEvent->mP4WBoson.Pt()/event.GetTrackRecoilTpcNeutrals().Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin"])->Fill(event.GetTrackRecoilTpcNeutrals().Pt(), event.mWEvent->mP4WBoson.Pt()/event.GetTrackRecoilTpcNeutrals().Pt());

   Double_t recoilRelDiff = (event.GetJetRecoil().Pt() - event.mWEvent->mP4WBoson.Pt())/event.mWEvent->mP4WBoson.Pt();
   ((TH1*) o["hJetRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoil().Pt() - event.mWEvent->mP4WBoson.Pt())/event.mWEvent->mP4WBoson.Pt();
   ((TH1*) o["hTrackRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoilTpcNeutrals().Pt() - event.mWEvent->mP4WBoson.Pt())/event.mWEvent->mP4WBoson.Pt();
   ((TH1*) o["hTrackRecoilTpcNeutralsPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoil().Pt() - event.GetJetRecoil().Pt())/event.GetJetRecoil().Pt();
   ((TH1*) o["hJetTrackRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoilTpcNeutrals().Pt() - event.GetJetRecoil().Pt())/event.GetJetRecoil().Pt();
   ((TH1*) o["hJetTrackTpcNeutralsRecoilPtRelDiff"]) ->Fill( recoilRelDiff );
}


/** */
void MCHContainer::FillDerived()
{
   Info("FillDerived()", "Called");
}


/** */
void MCHContainer::PostFill()
{
   Info("PostFill", "Called");

   // Fit the correction curve
   TH2* hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin = (TH2*) o["hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin"];

   o["hRecoilCorrection"] = (TProfile*) hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin->ProfileX("hRecoilCorrection", 0, 10);
   TProfile* hRecoilCorrection = (TProfile*) o["hRecoilCorrection"];

   if (hRecoilCorrection->Integral() ) {
      hRecoilCorrection->Fit("pol3", "+", "", 0, 5);
      hRecoilCorrection->Fit("pol0", "+", "", 5, 10);
   }

   // Fit the means
   TH2I* hRecoilVsWBosonPt = (TH2I*) o["hTrackRecoilTpcNeutralsPtVsWBosonPt"];
   TProfile* hRecoilVsWBosonPt_pfx = hRecoilVsWBosonPt->ProfileX();
   TF1 fitFunc("fitFunc", "[0] + [1]*x", hRecoilVsWBosonPt_pfx->GetXaxis()->GetXmin(), hRecoilVsWBosonPt_pfx->GetXaxis()->GetXmax());
   fitFunc.SetParNames("Offset", "Slope");
   hRecoilVsWBosonPt_pfx->Fit(&fitFunc);
   hRecoilVsWBosonPt->GetListOfFunctions()->Add(hRecoilVsWBosonPt_pfx->Clone(), "same");

   // Fit the means of the corrected recoil p_T
   hRecoilVsWBosonPt = (TH2I*) o["hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt"];
   hRecoilVsWBosonPt_pfx = hRecoilVsWBosonPt->ProfileX();
   TF1 fitFuncCorrected("fitFuncCorrected", "[0] + [1]*x", hRecoilVsWBosonPt_pfx->GetXaxis()->GetXmin(), hRecoilVsWBosonPt_pfx->GetXaxis()->GetXmax());
   fitFuncCorrected.SetParNames("Offset", "Slope");
   hRecoilVsWBosonPt_pfx->Fit(&fitFuncCorrected);
   hRecoilVsWBosonPt->GetListOfFunctions()->Add(hRecoilVsWBosonPt_pfx->Clone(), "same");

   // Calculate the fraction of missreconstructed events
   rh::H2I* hRecoVsGenWBosonPz = (rh::H2I*) o["hRecoVsGenWBosonPz"];

   TF1 funcLow ("funcLow", "-30 + x", hRecoVsGenWBosonPz->GetXaxis()->GetXmin(), hRecoVsGenWBosonPz->GetXaxis()->GetXmax());
   TF1 funcHigh("funcHigh", "30 + x", hRecoVsGenWBosonPz->GetXaxis()->GetXmin(), hRecoVsGenWBosonPz->GetXaxis()->GetXmax());

   double integralAboveLow  = hRecoVsGenWBosonPz->CalcIntegralAbove(funcLow);
   double integralAboveHigh = hRecoVsGenWBosonPz->CalcIntegralAbove(funcHigh);
   double integral = hRecoVsGenWBosonPz->Integral();

   printf("integralAboveLow: %f, integralAboveHigh: %f\n", integralAboveLow, integralAboveHigh);
   //printf("integralAboveLow: %f, integralAboveHigh: \n", integralAboveLow);

   char textFrac[10];
   sprintf(textFrac, "f = %5.3f", (integralAboveLow-integralAboveHigh)/integral);

   hRecoVsGenWBosonPz->GetListOfFunctions()->Add(funcLow.Clone());
   hRecoVsGenWBosonPz->GetListOfFunctions()->Add(funcHigh.Clone());
   TText *text = new TText(0.5, 0.92, textFrac);
   text->SetNDC(true);
   hRecoVsGenWBosonPz->GetListOfFunctions()->Add(text);
}
