/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "MCHContainer.h"

#include "TF1.h"

#include "VecBosEvent.h"
#include "StVecBosMaker.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

//need these to get MC record
#include "tables/St_g2t_tpc_hit_Table.h"
#include "StMcEventMaker/StMcEventMaker.h"
#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcVertex.hh"
#include "StMcEvent/StMcTrack.hh"

#include "Globals.h"

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


/** Default destructor. */
MCHContainer::~MCHContainer()
{
}


/** */
void MCHContainer::BookHists()
{
   string shName;
   TH1*   hist;

   fDir->cd();

   o["hWBosonE"]                  = hist = new TH1I("hWBosonE", "; W energy [GeV]; Events", 100, 20., 200.);
   hist->SetOption("hist GRIDX");
   o["hWBosonPx"]                 = hist = new TH1I("hWBosonPx", "; P^{W}_{x} [GeV/c]; Events", 80, -40., 40.);
   hist->SetOption("hist GRIDX");
   o["hWBosonPy"]                 = hist = new TH1I("hWBosonPy", "; P^{W}_{y} [GeV/c]; Events", 80, -40., 40.);
   hist->SetOption("hist GRIDX");
   o["hWBosonPz"]                 = hist = new TH1I("hWBosonPz", "; P^{W}_{z} [GeV/c]; Events", 80, -40., 40.);
   hist->SetOption("hist GRIDX");
   o["hWBosonPt"]                 = hist = new TH1I("hWBosonPt", "; P^{W}_{T} [GeV/c]; Events", 40, 0., 40.);
   hist->SetOption("hist GRIDX");
   o["hWBosonEta"]                = hist = new TH1I("hWBosonEta", "; W pseudo-rapidity #eta; Events", 60, -6, 6);
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

   o["hLeptonE"]                  = hist = new TH1I("hLeptonE", "; electron energy [GeV]; Events", 50, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hLeptonPx"]                 = hist = new TH1I("hLeptonPx", "; P^{#nu}_{x} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hLeptonPy"]                 = hist = new TH1I("hLeptonPy", "; P^{#nu}_{y} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hLeptonPz"]                 = hist = new TH1I("hLeptonPz", "; P^{#nu}_{z} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hLeptonPt"]                 = hist = new TH1I("hLeptonPt", "; P^{#nu}_{T} [GeV/c]; Events", 100, 0., 150.);
   hist->SetOption("hist GRIDX");

   o["hTrackPhiVsLeptonPhi"]      = hist = new TH2I("hTrackPhiVsLeptonPhi", "; Lepton #phi; Track #phi", 50, -M_PI, M_PI, 50, -M_PI, M_PI);
   hist->SetOption("colz");

   o["hTrackPtVsLeptonPt"]        = hist = new TH2I("hTrackPtVsLeptonPt", "; Lepton P_{T}; Track P_{T}", 50, 0, 50, 50, 0, 50);
   hist->SetOption("colz");

   // tracks ID
   //o["hTrackID"]                = hist = new TH1I("hTrackID", "; ; Events", 100, 0., 200.);

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
   o["hRecoilInAcceptPt"]               = hist = new TH1I("hRecoilInAcceptPt", "; P^{Recoil}_{t} [GeV/c]; Events", 50, 0., 50.);
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

   // recoil from tracks geant variables
   o["hHadRecoilFromTracksPx"]   = hist = new TH1I("hHadRecoilFromTracksPx", "; P_{x}^{Recoil} (geant); Events", 100, -50., 50.);
   o["hHadRecoilFromTracksPy"]   = hist = new TH1I("hHadRecoilFromTracksPy", "; P_{y}^{Recoil} (geant); Events", 100, -50., 50.);
   o["hHadRecoilFromTracksPz"]   = hist = new TH1I("hHadRecoilFromTracksPz", "; P_{z}^{Recoil} (geant); Events", 100, -50., 50.);
   o["hHadRecoilFromTracksEta"]  = hist = new TH1I("hHadRecoilFromTracksEta", "; #eta^{Recoil} (geant); Events", 100, -8., 8.);
   o["hHadRecoilFromTracksPt"]   = hist = new TH1I("hHadRecoilFromTracksPt", "; P_{T}^{Recoil} (geant); Events", 100, 0., 50.);
   o["hRecoilCorrelPythia"]      = hist = new TH2I("hRecoilCorrelPythia", "; P_{T}^{Recoil} (geant:pythia); P_{T}^{Recoil}", 100, 0., 50., 100, 0., 50.);

   o["hJetRecoilPtVsWBosonPt"]   = hist = new TH2I("hJetRecoilPtVsWBosonPt", "; W Boson P_{T}; Jet-based Recoil P_{T}", 50, 0., 50., 50, 0., 50.);
   hist->SetOption("colz");

   o["hTrackRecoilPtVsWBosonPt"]    = hist = new TH2I("hTrackRecoilPtVsWBosonPt", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50.);
   hist->SetOption("colz");

   o["hTrackRecoilTpcPtVsWBosonPt"] = hist = new TH2I("hTrackRecoilTpcPtVsWBosonPt", "; W Boson P_{T}; Track-based Recoil P_{T}", 50, 0., 50., 50, 0., 50.);
   hist->SetOption("colz");

   o["hJetRecoilPtRelDiff"]   = hist = new TH1F("hJetRecoilPtRelDiff", "; Jet-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2.);
   hist->SetOption("hist");

   o["hTrackRecoilPtRelDiff"] = hist = new TH1F("hTrackRecoilPtRelDiff", "; Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2.);
   hist->SetOption("hist");

   o["hJetTrackRecoilPtRelDiff"] = hist = new TH1F("hJetTrackRecoilPtRelDiff", "; Jet- Track-based Recoil P_{T}, Rel. Diff.; ", 50, -2, 2.);
   hist->SetOption("hist");

   // MC correction variables
   o["hEnergyRatio"] = hist = new TH1I("hEnergyRatio", "; E_{in}^{Recoil}/E_{out}^{Recoil}; Events", 100, 0., 4.);
   hist->SetOption("hist GRIDX");

   // hist->SetOption("hist GRIDX GRIDY");
}


void MCHContainer::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;

   ((TH1*) o["hWBosonE"])->Fill(event.mWEvent->mP4WBoson.E());
   ((TH1*) o["hWBosonPx"])->Fill(event.mWEvent->mP4WBoson.Px());
   ((TH1*) o["hWBosonPy"])->Fill(event.mWEvent->mP4WBoson.Py());
   ((TH1*) o["hWBosonPz"])->Fill(event.mWEvent->mP4WBoson.Pz());
   ((TH1*) o["hWBosonPt"])->Fill(event.mWEvent->mP4WBoson.Pt());

   if (event.mWEvent->mP4WBoson.Mag() > 0)
      ((TH1*) o["hWBosonEta"])->Fill(event.mWEvent->mP4WBoson.Eta());
   else
      ((TH1*) o["hWBosonEta"])->Fill(1e5);

   ((TH1*) o["hNeutrinoE"])->Fill(event.mWEvent->mP4Neutrino.E());
   ((TH1*) o["hNeutrinoPx"])->Fill(event.mWEvent->mP4Neutrino.Px());
   ((TH1*) o["hNeutrinoPy"])->Fill(event.mWEvent->mP4Neutrino.Py());
   ((TH1*) o["hNeutrinoPz"])->Fill(event.mWEvent->mP4Neutrino.Pz());
   ((TH1*) o["hNeutrinoPt"])->Fill(event.mWEvent->mP4Neutrino.Pz());

   ((TH1*) o["hLeptonE"])->Fill(event.mWEvent->mP4Lepton.E());
   ((TH1*) o["hLeptonPx"])->Fill(event.mWEvent->mP4Lepton.Px());
   ((TH1*) o["hLeptonPy"])->Fill(event.mWEvent->mP4Lepton.Py());
   ((TH1*) o["hLeptonPz"])->Fill(event.mWEvent->mP4Lepton.Pz());
   ((TH1*) o["hLeptonPt"])->Fill(event.mWEvent->mP4Lepton.Pt());

   if (event.mTracksCandidate.size() > 0) {
      ((TH1*) o["hTrackPhiVsLeptonPhi"])->Fill(event.mWEvent->mP4Lepton.Phi(), (*event.mTracksCandidate.begin())->mP3AtDca.Phi());
      ((TH1*) o["hTrackPtVsLeptonPt"])  ->Fill(event.mWEvent->mP4Lepton.Pt(),  (*event.mTracksCandidate.begin())->mP3AtDca.Pt());
   }

   // recoil variables
   ((TH1*) o["hRecoilE"])->Fill(event.mWEvent->mP4Recoil.E());
   ((TH1*) o["hRecoilPx"])->Fill(event.mWEvent->mP4Recoil.Px());
   ((TH1*) o["hRecoilPy"])->Fill(event.mWEvent->mP4Recoil.Py());
   ((TH1*) o["hRecoilPz"])->Fill(event.mWEvent->mP4Recoil.Pz());
   ((TH1*) o["hRecInAccEnergy"])->Fill(event.mWEvent->mP4RecoilInAccept.E());
   ((TH1*) o["hRecInAccMomentumX"])->Fill(event.mWEvent->mP4RecoilInAccept.Px());
   ((TH1*) o["hRecInAccMomentumY"])->Fill(event.mWEvent->mP4RecoilInAccept.Py());
   ((TH1*) o["hRecInAccMomentumZ"])->Fill(event.mWEvent->mP4RecoilInAccept.Pz());
   ((TH1*) o["hRecoilInAcceptPt"])->Fill(event.mWEvent->mP4RecoilInAccept.Pt());
   ((TH1*) o["hRecOutAccEnergy"])->Fill(event.mWEvent->mP4RecoilOutAccept.E());
   ((TH1*) o["hRecOutAccMomentumX"])->Fill(event.mWEvent->mP4RecoilOutAccept.Px());
   ((TH1*) o["hRecOutAccMomentumY"])->Fill(event.mWEvent->mP4RecoilOutAccept.Py());
   ((TH1*) o["hRecOutAccMomentumZ"])->Fill(event.mWEvent->mP4RecoilOutAccept.Pz());
   ((TH1*) o["hRecOutAccPt"])->Fill(event.mWEvent->mP4RecoilOutAccept.Pt());

   // recoil from tracks geant variables
   ((TH1*) o["hHadRecoilFromTracksPx"])->Fill(event.mP3RecoilFromTracks.Px());
   ((TH1*) o["hHadRecoilFromTracksPy"])->Fill(event.mP3RecoilFromTracks.Py());
   ((TH1*) o["hHadRecoilFromTracksPz"])->Fill(event.mP3RecoilFromTracks.Pz());

   if (event.mP3RecoilFromTracks.Mag() > 0)
      ((TH1*) o["hHadRecoilFromTracksEta"])->Fill(event.mP3RecoilFromTracks.Eta());
   else
      ((TH1*) o["hHadRecoilFromTracksEta"])->Fill(1e5);

   ((TH1*) o["hHadRecoilFromTracksPt"])->Fill(event.mP3RecoilFromTracks.Pt());
   ((TH1*) o["hRecoilCorrelPythia"])->Fill(event.mP3RecoilFromTracks.Pt(), event.mWEvent->mP4Recoil.Pt());

   ((TH1*) o["hJetRecoilPtVsWBosonPt"])     ->Fill(event.mWEvent->mP4WBoson.Pt(), event.GetJetRecoil().Pt());
   ((TH1*) o["hTrackRecoilPtVsWBosonPt"])   ->Fill(event.mWEvent->mP4WBoson.Pt(), event.GetTrackRecoil().Pt());
   ((TH1*) o["hTrackRecoilTpcPtVsWBosonPt"])->Fill(event.mWEvent->mP4WBoson.Pt(), event.mP3TrackRecoilTpc.Pt());

   Double_t recoilRelDiff = (event.GetJetRecoil().Pt() - event.mWEvent->mP4WBoson.Pt())/event.mWEvent->mP4WBoson.Pt();
   ((TH1*) o["hJetRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoil().Pt() - event.mWEvent->mP4WBoson.Pt())/event.mWEvent->mP4WBoson.Pt();
   ((TH1*) o["hTrackRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   recoilRelDiff = (event.GetTrackRecoil().Pt() - event.GetJetRecoil().Pt())/event.GetJetRecoil().Pt();
   ((TH1*) o["hJetTrackRecoilPtRelDiff"]) ->Fill( recoilRelDiff );

   // MC correction variablel
   //((TH1*) o["hEnergyRatio"])->Fill(event.mWEvent->fEnergyRatio);
   //((TH1*) o["hTrackID"])->Fill(event.trackId);
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
}
