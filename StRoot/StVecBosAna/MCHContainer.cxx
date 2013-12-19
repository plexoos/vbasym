#include "MCHContainer.h"

#include "TF1.h"
#include "TProfile.h"
#include "TText.h"

#include "WBosEvent.h"
#include "WBosMcEvent.h"
#include "Globals.h"

#include "utils/H1I.h"
#include "utils/H1F.h"
#include "utils/H2I.h"
#include "utils/H2F.h"

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

   o["hWBosonE"]   = new rh::H1F("hWBosonE", "; W energy [GeV]; Events", 100, 20., 200., "hist GRIDX");
   o["hWBosonPz"]  = new rh::H1F("hWBosonPz", "; P^{W}_{z} [GeV/c]; Events", 50, -80., 80., "hist GRIDX");
   o["hWBosonPt"]  = new rh::H1F("hWBosonPt", "; P^{W}_{T} [GeV/c]; Events", 40, 0., 40., "hist GRIDX");
   o["hWBosonPt_zoomin"] = new rh::H1F("hWBosonPt_zoomin", "; P^{W}_{T} [GeV/c]; Events", 20, 0., 10., "hist GRIDX");
   o["hWBosonEta"] = new rh::H1F("hWBosonEta", "; W pseudo-rapidity #eta; Events", 60, -6, 6, "hist GRIDX");

   o["hNeutrinoE"]  = new rh::H1I("hNeutrinoE", "; #nu energy [GeV]; Events", 50, 0., 200., "hist GRIDX");
   o["hNeutrinoPx"] = new rh::H1I("hNeutrinoPx", "; P^{#nu}_{x} [GeV/c]; Events", 100, -150., 150., "hist GRIDX");
   o["hNeutrinoPy"] = new rh::H1I("hNeutrinoPy", "; P^{#nu}_{y} [GeV/c]; Events", 100, -150., 150., "hist GRIDX");
   o["hNeutrinoPz"] = new rh::H1I("hNeutrinoPz", "; P^{#nu}_{z} [GeV/c]; Events", 100, -150., 150., "hist GRIDX");
   o["hNeutrinoPt"] = new rh::H1I("hNeutrinoPt", "; P^{#nu}_{T} [GeV/c]; Events", 100, 0., 150., "hist GRIDX");

   o["hLeptonE"]  = new rh::H1I("hLeptonE", "; electron energy [GeV]; Events", 80, 0., 80., "hist GRIDX");
   o["hLeptonPx"] = new rh::H1I("hLeptonPx", "; P^{#nu}_{x} [GeV/c]; Events", 80, -80., 80., "hist GRIDX");
   o["hLeptonPy"] = new rh::H1I("hLeptonPy", "; P^{#nu}_{y} [GeV/c]; Events", 80, -80., 80., "hist GRIDX");
   o["hLeptonPz"] = new rh::H1I("hLeptonPz", "; P^{#nu}_{z} [GeV/c]; Events", 80, -80., 80., "hist GRIDX");
   o["hLeptonPt"] = new rh::H1I("hLeptonPt", "; P^{#nu}_{T} [GeV/c]; Events", 80, 0., 80., "hist GRIDX");

   o["hRecoVsGenLeptonPhi"] = new rh::H2I("hRecoVsGenLeptonPhi", "; Gen. Lepton #phi; Reco. Lepton #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI, "colz LOGZ");
   o["hRecoVsGenLeptonPt"]  = new rh::H2I("hRecoVsGenLeptonPt",  "; Gen. Lepton P_{T}, GeV; Reco. Lepton P_{T}, GeV", 50, 10, 60, 50, 10, 60, "colz LOGZ");
   o["hRecoVsGenLeptonPz"]  = new rh::H2I("hRecoVsGenLeptonPz",  "; Gen. Lepton P_{z}, GeV; Reco. Lepton P_{z}, GeV", 50, -50, 50, 50, -50, 50, "colz LOGZ");
   o["hRecoVsGenLeptonEta"] = new rh::H2I("hRecoVsGenLeptonEta", "; Gen. Lepton #eta; Reco. Lepton #eta", 50, -2, 2, 50, -2, 2, "colz LOGZ");

   o["hRecoVsGenNeutrinoPhi"] = new rh::H2I("hRecoVsGenNeutrinoPhi", "; Gen. Neutrino #phi; Reco. Neutrino #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI, "colz LOGZ");
   o["hRecoVsGenNeutrinoPt"]  = new rh::H2I("hRecoVsGenNeutrinoPt",  "; Gen. Neutrino P_{T}, GeV; Reco. Neutrino P_{T}, GeV", 50, 10, 60, 50, 10, 60, "colz LOGZ");
   o["hRecoVsGenNeutrinoPz"]  = new rh::H2I("hRecoVsGenNeutrinoPz",  "; Gen. Neutrino P_{z}, GeV; Reco. Neutrino P_{z}, GeV", 50, -70, 70, 50, -70, 70, "colz LOGZ");
   o["hRecoVsGenNeutrinoPz_c"]  = new rh::H2I("hRecoVsGenNeutrinoPz_c",  "; Gen. Neutrino P_{z}, GeV; Reco. Neutrino P_{z}, GeV", 50, -70, 70, 50, -70, 70, "colz LOGZ");
   o["hRecoVsGenNeutrinoPz_f"]  = new rh::H2I("hRecoVsGenNeutrinoPz_f",  "; Gen. Neutrino P_{z}, GeV; Reco. Neutrino P_{z}, GeV", 50, -70, 70, 50, -70, 70, "colz LOGZ");
   o["hRecoVsGenNeutrinoEta"] = new rh::H2I("hRecoVsGenNeutrinoEta", "; Gen. Neutrino #eta; Reco. Neutrino #eta", 50, -2, 2, 50, -2, 2, "colz LOGZ");

   o["hRecoVsGenWBosonPhi"] = new rh::H2I("hRecoVsGenWBosonPhi", "; Gen. W Boson #phi; Reco. W Boson #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI, "colz LOGZ");
   o["hRecoVsGenWBosonPt"]  = new rh::H2I("hRecoVsGenWBosonPt",  "; Gen. W Boson P_{T}, GeV; Reco. W Boson P_{T}, GeV", 50, 0, 25, 50, 0, 25, "colz LOGZ");
   o["hRecoVsGenWBosonPz"]  = new rh::H2I("hRecoVsGenWBosonPz",  "; Gen. W Boson P_{z}, GeV; Reco. W Boson P_{z}, GeV", 50, -80, 80, 50, -80, 80, "colz LOGZ");
   o["hRecoVsGenWBosonEta"] = new rh::H2I("hRecoVsGenWBosonEta", "; Gen. W Boson #eta; Reco. W Boson #eta", 50, -6, 6, 50, -6, 6, "colz LOGZ");

   // W recoil momentum components
   o["hGenRecoilE"]  = new rh::H1I("hGenRecoilE", "; Gen. W Recoil E, GeV; Events", 100, 0., 200., "hist GRIDX");
   o["hGenRecoilPz"] = new rh::H1I("hGenRecoilPz", "; Gen. W Recoil p_{z}, GeV; Events", 100, -160., 160., "hist GRIDX");
   o["hGenRecoilPt"] = new rh::H1I("hGenRecoilPt",  "; Gen. W Recoil p_{T}, GeV; Events", 50, 0., 25., "hist GRIDX");
   o["hGenRecoilPhi"] = new rh::H1I("hGenRecoilPhi", "; Gen. W Recoil #phi; Events", 50, -M_PI, M_PI, "hist GRIDX");

   // Recoil in acceptance
   o["hRecInAccEnergy"]    = new rh::H1I("hRecInAccEenergy", "; Recoil energy [GeV]; Events", 100, 0., 200., "hist GRIDX");
   o["hRecInAccMomentumX"] = new rh::H1I("hRecInAccMomentumX", "; P^{Recoil}_{x} [GeV/c]; Events", 100, -160., 160., "hist GRIDX");
   o["hRecInAccMomentumY"] = new rh::H1I("hRecInAccMomentumY", "; P^{Recoil}_{y} [GeV/c]; Events", 100, -160., 160., "hist GRIDX");
   o["hRecInAccMomentumZ"] = new rh::H1I("hRecInAccMomentumZ", "; P^{Recoil}_{z} [GeV/c]; Events", 100, -160., 160., "hist GRIDX");
   o["hRecoilInAcceptPt"]  = new rh::H1I("hRecoilInAcceptPt", "; P^{Recoil}_{t} [GeV/c]; Events", 50, 0., 50., "hist GRIDX");

   // Recoil outside of detector acceptance
   o["hRecOutAccEnergy"]    = new rh::H1I("hRecOutAccEenergy", "; Recoil energy [GeV]; Events", 100, 0., 200., "hist GRIDX");
   o["hRecOutAccMomentumX"] = new rh::H1I("hRecOutAccMomentumX", "; P^{Recoil}_{x} [GeV/c]; Events", 100, -160., 160., "hist GRIDX");
   o["hRecOutAccMomentumY"] = new rh::H1I("hRecOutAccMomentumY", "; P^{Recoil}_{y} [GeV/c]; Events", 100, -160., 160., "hist GRIDX");
   o["hRecOutAccMomentumZ"] = new rh::H1I("hRecOutAccMomentumZ", "; P^{Recoil}_{z} [GeV/c]; Events", 100, -160., 160., "hist GRIDX");
   o["hRecOutAccPt"]        = new rh::H1I("hRecOutAccPt", "; P^{Recoil}_{t} [GeV/c]; Events", 50, 0., 50., "hist GRIDX");

   o["hGenRecoilVsWBosonPt"]       = new rh::H2I("hGenRecoilVsWBosonPt", "; Gen. W Boson p_{T}; Gen. W Recoil p_{T};", 50, 0, 25, 50, 0, 25, "colz LOGZ");
   o["hGenRecoilInAccVsWBosonPt"]  = new rh::H2I("hGenRecoilInAccVsWBosonPt",  "; Gen. W Boson p_{T}; Gen. W Recoil p_{T} In Accept.;", 50, 0, 25, 50, 0, 25, "colz LOGZ");
   o["hGenRecoilOutAccVsWBosonPt"] = new rh::H2I("hGenRecoilOutAccVsWBosonPt", "; Gen. W Boson p_{T}; Gen. W Recoil p_{T} Out of Accept.;", 50, 0, 25, 50, 0, 25, "colz LOGZ");
   o["hGenRecoilOutVsInAccPt"]     = new rh::H2I("hGenRecoilOutVsInAccPt", "; Gen. W Recoil p_{T} In Accept.; Gen. W Recoil p_{T} Out of Accept.;", 50, 0, 25, 50, 0, 25, "colz LOGZ");
   o["hGenRecoilInOutDeltaPhi"]    = new rh::H1I("hGenRecoilInOutDeltaPhi", "; Gen. W Recoil #Delta#phi(In, Out) ; Events;", 30, -M_PI, M_PI, "hist GRIDX GRIDY");

   o["hJetRecoilPtVsWBosonPt"]   = new rh::H2I("hJetRecoilPtVsWBosonPt", "; W Boson P_{T}; Jet-based Recoil P_{T}", 50, 0., 50., 50, 0., 50., "colz LOGZ");
   o["hJetRecoilPt_GenOverReco"] = new rh::H2F("hJetRecoilPt_GenOverReco", ";Jet-based Recoil P_{T}; Correction factor",40, 0, 40, 50, 0, 20, "colz LOGZ");
   o["hJetRecoilPt_GenOverReco_zoomin"] = new rh::H2F("hJetRecoilPt_GenOverReco_zoomin", ";Jet-based Recoil P_{T}; Correction factor",20, 0, 10, 50, 0, 20, "colz LOGZ");
   o["hJetRecoilPhiVsWBosonPhi"] = new rh::H2I("hJetRecoilPhiVsWBosonPhi", "; W Boson #phi; Jet-based Recoil #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI, "colz LOGZ");

   o["hTrackRecoilPtVsWBosonPt"]    = new rh::H2I("hTrackRecoilPtVsWBosonPt", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50., "colz LOGZ");
   o["hTrackRecoilPhiVsWBosonPhi"]  = new rh::H2I("hTrackRecoilPhiVsWBosonPhi", "; W Boson #phi; Track-based Recoil #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI, "colz LOGZ");
   o["hTrackRecoilTpcPtVsWBosonPt"] = new rh::H2I("hTrackRecoilTpcPtVsWBosonPt", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50., "colz LOGZ");
   o["hTrackRecoilTpcNeutralsPtVsWBosonPt"] = new rh::H2I("hTrackRecoilTpcNeutralsPtVsWBosonPt", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50., "colz LOGZ");
   o["hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt"] = new rh::H2I("hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt", "; W Boson P_{T}; Corrected Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50., "colz LOGZ");
   o["hTrackRecoilTpcNeutralsPtVsWBosonPt_copy"] = new rh::H2I("hTrackRecoilTpcNeutralsPtVsWBosonPt_copy", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50., "colz LOGZ");
   o["hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy"] = new rh::H2I("hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy", "; W Boson P_{T}; Corrected Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50., "colz LOGZ");
   o["hTrackRecoilTpcNeutralsPt_GenOverReco"] = new rh::H2F("hTrackRecoilTpcNeutralsPt_GenOverReco", ";Track-based Recoil P_{T}; Correction factor",40, 0, 40, 50, 0, 20, "colz LOGZ");
   o["hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin"] = new rh::H2F("hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin", "; Track-based Recoil P_{T}; Correction factor",20, 0, 10, 50, 0, 20, "colz LOGZ");

   o["hJetRecoilPtRelDiff"]   = new rh::H1F("hJetRecoilPtRelDiff", "; Jet-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2., "hist");
   o["hTrackRecoilPtRelDiff"] = new rh::H1F("hTrackRecoilPtRelDiff", "; Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2., "hist");
   o["hTrackRecoilTpcNeutralsPtRelDiff"] = new rh::H1F("hTrackRecoilTpcNeutralsPtRelDiff", "; Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2., "hist");
   o["hJetTrackRecoilPtRelDiff"] = new rh::H1F("hJetTrackRecoilPtRelDiff", "; Jet- Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2., "hist");
   o["hJetTrackTpcNeutralsRecoilPtRelDiff"] = new rh::H1F("hJetTrackTpcNeutralsRecoilPtRelDiff", "; Jet- Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2., "hist");
}


/**
 * Fill histograms using information about the Monte-Carlo event.
 */
void MCHContainer::Fill(ProtoEvent &ev)
{
   WBosEvent& event = (WBosEvent&) ev;
   WBosMcEvent* mcEvent = (WBosMcEvent*) event.mMcEvent;

   ((TH1*) o["hWBosonE"]) ->Fill(mcEvent->mP4WBoson.E());
   ((TH1*) o["hWBosonPz"])->Fill(mcEvent->mP4WBoson.Pz());
   ((TH1*) o["hWBosonPt"])->Fill(mcEvent->mP4WBoson.Pt());
   ((TH1*) o["hWBosonPt_zoomin"])->Fill(mcEvent->mP4WBoson.Pt());

   if (mcEvent->mP4WBoson.Mag() > 0)
      ((TH1*) o["hWBosonEta"])->Fill(mcEvent->mP4WBoson.Eta());
   else
      ((TH1*) o["hWBosonEta"])->Fill(1e5);

   ((TH1*) o["hNeutrinoE"]) ->Fill(mcEvent->mP4Neutrino.E());
   ((TH1*) o["hNeutrinoPx"])->Fill(mcEvent->mP4Neutrino.Px());
   ((TH1*) o["hNeutrinoPy"])->Fill(mcEvent->mP4Neutrino.Py());
   ((TH1*) o["hNeutrinoPz"])->Fill(mcEvent->mP4Neutrino.Pz());
   ((TH1*) o["hNeutrinoPt"])->Fill(mcEvent->mP4Neutrino.Pz());

   ((TH1*) o["hLeptonE"]) ->Fill(mcEvent->mP4Lepton.E());
   ((TH1*) o["hLeptonPx"])->Fill(mcEvent->mP4Lepton.Px());
   ((TH1*) o["hLeptonPy"])->Fill(mcEvent->mP4Lepton.Py());
   ((TH1*) o["hLeptonPz"])->Fill(mcEvent->mP4Lepton.Pz());
   ((TH1*) o["hLeptonPt"])->Fill(mcEvent->mP4Lepton.Pt());

   ((TH1*) o["hRecoVsGenLeptonPhi"])  ->Fill(mcEvent->mP4Lepton.Phi(),   event.GetElectronP3().Phi());
   ((TH1*) o["hRecoVsGenLeptonPt"])   ->Fill(mcEvent->mP4Lepton.Pt(),    event.GetElectronP3().Pt());
   ((TH1*) o["hRecoVsGenLeptonPz"])   ->Fill(mcEvent->mP4Lepton.Pz(),    event.GetElectronP3().Pz());
   ((TH1*) o["hRecoVsGenLeptonEta"])  ->Fill(mcEvent->mP4Lepton.Eta(),   event.GetElectronP3().Eta());

   ((TH1*) o["hRecoVsGenNeutrinoPhi"])->Fill(mcEvent->mP4Neutrino.Phi(), event.GetNeutrinoP3().Phi());
   ((TH1*) o["hRecoVsGenNeutrinoPt"]) ->Fill(mcEvent->mP4Neutrino.Pt(),  event.GetNeutrinoP3().Pt());
   ((TH1*) o["hRecoVsGenNeutrinoPz"]) ->Fill(mcEvent->mP4Neutrino.Pz(),  event.GetNeutrinoP3().Pz());

   if ( fabs(mcEvent->mP4Neutrino.Pz() - event.GetNeutrinoP3().Pz()) <
        fabs(mcEvent->mP4Neutrino.Pz() - event.GetNeutrinoP3Other().Pz()) )
   {
      ((TH1*) o["hRecoVsGenNeutrinoPz_c"]) ->Fill(mcEvent->mP4Neutrino.Pz(),  event.GetNeutrinoP3().Pz());
      ((TH1*) o["hRecoVsGenNeutrinoPz_f"]) ->Fill(mcEvent->mP4Neutrino.Pz(),  event.GetNeutrinoP3Other().Pz());
   } else {
      ((TH1*) o["hRecoVsGenNeutrinoPz_c"]) ->Fill(mcEvent->mP4Neutrino.Pz(),  event.GetNeutrinoP3Other().Pz());
      ((TH1*) o["hRecoVsGenNeutrinoPz_f"]) ->Fill(mcEvent->mP4Neutrino.Pz(),  event.GetNeutrinoP3().Pz());
   }

   ((TH1*) o["hRecoVsGenNeutrinoEta"])->Fill(mcEvent->mP4Neutrino.Eta(), event.GetNeutrinoP3().Eta());

   ((TH1*) o["hRecoVsGenWBosonPhi"])  ->Fill(mcEvent->mP4WBoson.Phi(),   event.GetVecBosonP3().Phi());
   ((TH1*) o["hRecoVsGenWBosonPt"])   ->Fill(mcEvent->mP4WBoson.Pt(),    event.GetVecBosonP3().Pt());
   ((TH1*) o["hRecoVsGenWBosonPz"])   ->Fill(mcEvent->mP4WBoson.Pz(),    event.GetVecBosonP3().Pz());
   ((TH1*) o["hRecoVsGenWBosonEta"])  ->Fill(mcEvent->mP4WBoson.Eta(),   event.GetVecBosonP3().Eta());

   ((TH1*) o["hGenRecoilE"]) ->Fill(mcEvent->mP4Recoil.E());
   ((TH1*) o["hGenRecoilPz"])->Fill(mcEvent->mP4Recoil.Pz());
   ((TH1*) o["hGenRecoilPt"])->Fill(mcEvent->mP4Recoil.Pt());
   ((TH1*) o["hGenRecoilPhi"])->Fill(mcEvent->mP4Recoil.Phi());

   ((TH1*) o["hRecInAccEnergy"])    ->Fill(mcEvent->mP4RecoilInAccept.E());
   ((TH1*) o["hRecInAccMomentumX"]) ->Fill(mcEvent->mP4RecoilInAccept.Px());
   ((TH1*) o["hRecInAccMomentumY"]) ->Fill(mcEvent->mP4RecoilInAccept.Py());
   ((TH1*) o["hRecInAccMomentumZ"]) ->Fill(mcEvent->mP4RecoilInAccept.Pz());
   ((TH1*) o["hRecoilInAcceptPt"])  ->Fill(mcEvent->mP4RecoilInAccept.Pt());
   ((TH1*) o["hRecOutAccEnergy"])   ->Fill(mcEvent->mP4RecoilOutAccept.E());
   ((TH1*) o["hRecOutAccMomentumX"])->Fill(mcEvent->mP4RecoilOutAccept.Px());
   ((TH1*) o["hRecOutAccMomentumY"])->Fill(mcEvent->mP4RecoilOutAccept.Py());
   ((TH1*) o["hRecOutAccMomentumZ"])->Fill(mcEvent->mP4RecoilOutAccept.Pz());
   ((TH1*) o["hRecOutAccPt"])       ->Fill(mcEvent->mP4RecoilOutAccept.Pt());

   ((TH1*) o["hGenRecoilVsWBosonPt"])->Fill(mcEvent->mP4WBoson.Pt(), mcEvent->mP4Recoil.Pt());
   ((TH1*) o["hGenRecoilInAccVsWBosonPt"])->Fill(mcEvent->mP4WBoson.Pt(), mcEvent->mP4RecoilInAccept.Pt());
   ((TH1*) o["hGenRecoilOutAccVsWBosonPt"])->Fill(mcEvent->mP4WBoson.Pt(), mcEvent->mP4RecoilOutAccept.Pt());
   ((TH1*) o["hGenRecoilOutVsInAccPt"])->Fill(mcEvent->mP4RecoilInAccept.Pt(), mcEvent->mP4RecoilOutAccept.Pt());
   ((TH1*) o["hGenRecoilInOutDeltaPhi"])->Fill(mcEvent->mP4RecoilInAccept.DeltaPhi( mcEvent->mP4RecoilOutAccept ));

   ((TH2*) o["hJetRecoilPtVsWBosonPt"])         ->Fill(mcEvent->mP4WBoson.Pt(),  event.GetJetRecoil().Pt());
   ((TH2*) o["hJetRecoilPhiVsWBosonPhi"])       ->Fill(mcEvent->mP4WBoson.Phi(), event.GetJetRecoil().Phi());
   ((TH2*) o["hJetRecoilPt_GenOverReco"])       ->Fill(event.GetJetRecoil().Pt(), mcEvent->mP4WBoson.Pt()/event.GetJetRecoil().Pt());
   ((TH2*) o["hJetRecoilPt_GenOverReco_zoomin"])->Fill(event.GetJetRecoil().Pt(), mcEvent->mP4WBoson.Pt()/event.GetJetRecoil().Pt());

   ((TH2*) o["hTrackRecoilPtVsWBosonPt"])                    ->Fill(mcEvent->mP4WBoson.Pt(),  event.GetTrackRecoil().Pt());
   ((TH2*) o["hTrackRecoilPhiVsWBosonPhi"])                  ->Fill(mcEvent->mP4WBoson.Phi(), event.GetTrackRecoil().Phi());
   ((TH2*) o["hTrackRecoilTpcPtVsWBosonPt"])                 ->Fill(mcEvent->mP4WBoson.Pt(),  event.mP3TrackRecoilTpc.Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPtVsWBosonPt"])         ->Fill(mcEvent->mP4WBoson.Pt(),  event.GetTrackRecoilTpcNeutrals().Pt());
   //((TH2*) o["hTrackRecoilTpcNeutralsPtVsWBosonPt"])         ->Fill(event.GetTrackRecoilTpcNeutrals().Pt(), mcEvent->mP4WBoson.Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt"])->Fill(mcEvent->mP4WBoson.Pt(),  event.GetTrackRecoilTpcNeutralsCorrected().Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPtVsWBosonPt_copy"])         ->Fill(mcEvent->mP4WBoson.Pt(),  event.GetTrackRecoilTpcNeutrals().Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy"])->Fill(mcEvent->mP4WBoson.Pt(),  event.GetTrackRecoilTpcNeutralsCorrected().Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPt_GenOverReco"])       ->Fill(event.GetTrackRecoilTpcNeutrals().Pt(), mcEvent->mP4WBoson.Pt()/event.GetTrackRecoilTpcNeutrals().Pt());
   ((TH2*) o["hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin"])->Fill(event.GetTrackRecoilTpcNeutrals().Pt(), mcEvent->mP4WBoson.Pt()/event.GetTrackRecoilTpcNeutrals().Pt());

   Double_t recoilRelDiff = (event.GetJetRecoil().Pt() - mcEvent->mP4WBoson.Pt())/mcEvent->mP4WBoson.Pt();
   ((TH1*) o["hJetRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoil().Pt() - mcEvent->mP4WBoson.Pt())/mcEvent->mP4WBoson.Pt();
   ((TH1*) o["hTrackRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoilTpcNeutrals().Pt() - mcEvent->mP4WBoson.Pt())/mcEvent->mP4WBoson.Pt();
   ((TH1*) o["hTrackRecoilTpcNeutralsPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoil().Pt() - event.GetJetRecoil().Pt())/event.GetJetRecoil().Pt();
   ((TH1*) o["hJetTrackRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoilTpcNeutrals().Pt() - event.GetJetRecoil().Pt())/event.GetJetRecoil().Pt();
   ((TH1*) o["hJetTrackTpcNeutralsRecoilPtRelDiff"]) ->Fill( recoilRelDiff );
}


/** */
void MCHContainer::PostFill()
{
   Info("PostFill", "Called");

   // Fit the correction curve
   TH2* hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin = (TH2*) o["hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin"];
   o["hRecoilCorrection"] = (TProfile*) hTrackRecoilTpcNeutralsPt_GenOverReco_zoomin->ProfileX("hRecoilCorrection");
   TProfile* hRecoilCorrection = (TProfile*) o["hRecoilCorrection"];
   hRecoilCorrection->Print("all");

   if (hRecoilCorrection->Integral() ) {
      hRecoilCorrection->Fit("pol3", "W +", "", 0, 5);  // The W option helps with low stat when error bar is 0
      hRecoilCorrection->Fit("pol0", "W +", "", 5, 10);
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

   char textFrac[10];
   sprintf(textFrac, "f = %5.3f", (integralAboveLow-integralAboveHigh)/integral);

   hRecoVsGenWBosonPz->GetListOfFunctions()->Add(funcLow.Clone());
   hRecoVsGenWBosonPz->GetListOfFunctions()->Add(funcHigh.Clone());
   TText *text = new TText(0.5, 0.92, textFrac);
   text->SetNDC(true);
   hRecoVsGenWBosonPz->GetListOfFunctions()->Add(text);
}
