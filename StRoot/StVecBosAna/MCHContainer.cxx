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

   o["hWenergy"] = hist = new TH1I("hWenergy", "; W energy [GeV]; Events", 100, 20., 200.);
   hist->SetOption("hist GRIDX");

   o["hWmomentumX"] = hist = new TH1I("hWmomentumX", "; P^{W}_{x} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hWmomentumY"] = hist = new TH1I("hWmomentumY", "; P^{W}_{y} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hWmomentumZ"] = hist = new TH1I("hWmomentumZ", "; P^{W}_{z} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hWpt"] = hist = new TH1I("hWpt", "; P^{W}_{T} [GeV/c]; Events", 100, 0., 160.);
   hist->SetOption("hist GRIDX");

   o["hWrapidity"] = hist = new TH1I("hWrapidity", "; W pseudo-rapidity #eta; Events", 20, -4, 4);
   hist->SetOption("hist GRIDX");

   o["hNeutrinoEnergy"] = hist = new TH1I("hNeutrinoEnergy", "; #nu energy [GeV]; Events", 50, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoMomentumX"] = hist = new TH1I("hNeutrinoMomentumX", "; P^{#nu}_{x} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoMomentumY"] = hist = new TH1I("hNeutrinoMomentumY", "; P^{#nu}_{y} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoMomentumZ"] = hist = new TH1I("hNeutrinoMomentumZ", "; P^{#nu}_{z} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoPt"] = hist = new TH1I("hNeutrinoPt", "; P^{#nu}_{T} [GeV/c]; Events", 100, 0., 150.);
   hist->SetOption("hist GRIDX");

   o["hElectronEnergy"] = hist = new TH1I("hElectronEnergy", "; electron energy [GeV]; Events", 50, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hElectronMomentumX"] = hist = new TH1I("hElectronMomentumX", "; P^{#nu}_{x} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hElectronMomentumY"] = hist = new TH1I("hElectronMomentumY", "; P^{#nu}_{y} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hElectronMomentumZ"] = hist = new TH1I("hElectronMomentumZ", "; P^{#nu}_{z} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hElectronPt"] = hist = new TH1I("hElectronPt", "; P^{#nu}_{T} [GeV/c]; Events", 100, 0., 150.);
   hist->SetOption("hist GRIDX");

   // tracks ID
   //o["hTrackID"] = hist = new TH1I("hTrackID", "; ; Events", 100, 0., 200.);

   // recoil
   o["hRecEnergy"] = hist = new TH1I("hRecEenergy", "; Recoil energy [GeV]; Events", 100, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hRecMomentumX"] = hist = new TH1I("hRecMomentumX", "; P^{Recoil}_{x} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecMomentumY"] = hist = new TH1I("hRecMomentumY", "; P^{Recoil}_{y} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecMomentumZ"] = hist = new TH1I("hRecMomentumZ", "; P^{Recoil}_{z} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");

   //recoil in acceptance
   o["hRecInAccEnergy"] = hist = new TH1I("hRecInAccEenergy", "; Recoil energy [GeV]; Events", 100, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hRecInAccMomentumX"] = hist = new TH1I("hRecInAccMomentumX", "; P^{Recoil}_{x} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecInAccMomentumY"] = hist = new TH1I("hRecInAccMomentumY", "; P^{Recoil}_{y} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecInAccMomentumZ"] = hist = new TH1I("hRecInAccMomentumZ", "; P^{Recoil}_{z} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");

   //recoil outside acceptance

   o["hRecOutAccEnergy"] = hist = new TH1I("hRecOutAccEenergy", "; Recoil energy [GeV]; Events", 100, 0., 200.);
   hist->SetOption("hist GRIDX");
   o["hRecOutAccMomentumX"] = hist = new TH1I("hRecOutAccMomentumX", "; P^{Recoil}_{x} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecOutAccMomentumY"] = hist = new TH1I("hRecOutAccMomentumY", "; P^{Recoil}_{y} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");
   o["hRecOutAccMomentumZ"] = hist = new TH1I("hRecOutAccMomentumZ", "; P^{Recoil}_{z} [GeV/c]; Events", 100, -160., 160.);
   hist->SetOption("hist GRIDX");


   // recoil from tracks geant variables
   o["hHadRecoilFromTracksPx"]  = hist = new TH1I("hHadRecoilFromTracksPx", "; P_{x}^{Recoil} (geant); Events", 100, -50., 50.);
   o["hHadRecoilFromTracksPy"]  = hist = new TH1I("hHadRecoilFromTracksPy", "; P_{y}^{Recoil} (geant); Events", 100, -50., 50.);
   o["hHadRecoilFromTracksPz"]  = hist = new TH1I("hHadRecoilFromTracksPz", "; P_{z}^{Recoil} (geant); Events", 100, -50., 50.);
   o["hHadRecoilFromTracksEta"] = hist = new TH1I("hHadRecoilFromTracksEta", "; #eta^{Recoil} (geant); Events", 100, -8., 8.);
   o["hHadRecoilFromTracksPt"]  = hist = new TH1I("hHadRecoilFromTracksPt", "; P_{T}^{Recoil} (geant); Events", 100, 0., 50.);
   o["hRecoilCorrelPythia"]     = hist = new TH2I("hRecoilCorrelPythia", "; P_{T}^{Recoil} (geant:pythia); P_{T}^{Recoil}", 100, 0., 50., 100, 0., 50.);


   // MC correction variables
   o["hEnergyRatio"] = hist = new TH1I("hEnergyRatio", "; E_{in}^{Recoil}/E_{out}^{Recoil}; Events", 100, 0., 4.);
   hist->SetOption("hist GRIDX");


   // hist->SetOption("hist GRIDX GRIDY");

}


void MCHContainer::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;

   //((TH1*) o["hWenergy"])->Fill(event.eW);
   ((TH1*) o["hWenergy"])->Fill(event.mWEvent->mWBosonP4.E());
   //((TH1*) o["hWmomentumX"])->Fill(event.mWP.x());
   ((TH1*) o["hWmomentumX"])->Fill(event.mWEvent->mWBosonP4.Px());
   //((TH1*) o["hWmomentumY"])->Fill(event.mWP.y());
   ((TH1*) o["hWmomentumY"])->Fill(event.mWEvent->mWBosonP4.Py());
   //((TH1*) o["hWmomentumZ"])->Fill(event.mWP.z());
   ((TH1*) o["hWmomentumZ"])->Fill(event.mWEvent->mWBosonP4.Pz());
   //((TH1*) o["hWpt"])->Fill(event.mWP.Perp());
   ((TH1*) o["hWpt"])->Fill(event.mWEvent->mWBosonP4.Pt());
   //((TH1*) o["hWrapidity"])->Fill(event.rapW)
   ((TH1*) o["hWrapidity"])->Fill(event.mWEvent->mWBosonP4.Eta());;

   ////((TH1*) o["hNeutrinoEnergy"])->Fill(event.eNeutrino);
   ((TH1*) o["hNeutrinoEnergy"])->Fill(event.mWEvent->mNeutrinoP4.E());
   //((TH1*) o["hNeutrinoMomentumX"])->Fill(event.mNeutrinoP.x());
   ((TH1*) o["hNeutrinoMomentumX"])->Fill(event.mWEvent->mNeutrinoP4.Px());
   //((TH1*) o["hNeutrinoMomentumY"])->Fill(event.mNeutrinoP.y());
   ((TH1*) o["hNeutrinoMomentumY"])->Fill(event.mWEvent->mNeutrinoP4.Py());
   //((TH1*) o["hNeutrinoMomentumZ"])->Fill(event.mNeutrinoP.z());
   ((TH1*) o["hNeutrinoMomentumZ"])->Fill(event.mWEvent->mNeutrinoP4.Pz());
   //((TH1*) o["hNeutrinoPt"])->Fill(event.mNeutrinoP.Perp());
   ((TH1*) o["hNeutrinoPt"])->Fill(event.mWEvent->mNeutrinoP4.Pz());

   ((TH1*) o["hElectronEnergy"])->Fill(event.mWEvent->mLeptonP4.E());
   //((TH1*) o["hElectronMomentumX"])->Fill(event.mElectronP.x());
   ((TH1*) o["hElectronMomentumX"])->Fill(event.mWEvent->mLeptonP4.Px());
   //((TH1*) o["hElectronMomentumY"])->Fill(event.mElectronP.y());
   ((TH1*) o["hElectronMomentumY"])->Fill(event.mWEvent->mLeptonP4.Py());
   //((TH1*) o["hElectronMomentumZ"])->Fill(event.mElectronP.z());
   ((TH1*) o["hElectronMomentumZ"])->Fill(event.mWEvent->mLeptonP4.Pz());
   //((TH1*) o["hElectronPt"])->Fill(event.mElectronP.Perp());
   ((TH1*) o["hElectronPt"])->Fill(event.mWEvent->mLeptonP4.Pt());

   // recoil variables
   ((TH1*) o["hRecEnergy"])->Fill(event.mWEvent->mRecoilP4.E());
   ((TH1*) o["hRecMomentumX"])->Fill(event.mWEvent->mRecoilP4.Px());
   ((TH1*) o["hRecMomentumY"])->Fill(event.mWEvent->mRecoilP4.Py());
   ((TH1*) o["hRecMomentumZ"])->Fill(event.mWEvent->mRecoilP4.Pz());
   ((TH1*) o["hRecInAccEnergy"])->Fill(event.mWEvent->mRecoilInAcceptP4.E());
   ((TH1*) o["hRecInAccMomentumX"])->Fill(event.mWEvent->mRecoilInAcceptP4.Px());
   ((TH1*) o["hRecInAccMomentumY"])->Fill(event.mWEvent->mRecoilInAcceptP4.Py());
   ((TH1*) o["hRecInAccMomentumZ"])->Fill(event.mWEvent->mRecoilInAcceptP4.Pz());
   ((TH1*) o["hRecOutAccEnergy"])->Fill(event.mWEvent->mRecoilOutAcceptP4.E());
   ((TH1*) o["hRecOutAccMomentumX"])->Fill(event.mWEvent->mRecoilOutAcceptP4.Px());
   ((TH1*) o["hRecOutAccMomentumY"])->Fill(event.mWEvent->mRecoilOutAcceptP4.Py());
   ((TH1*) o["hRecOutAccMomentumZ"])->Fill(event.mWEvent->mRecoilOutAcceptP4.Pz());

   // recoil from tracks geant variables
   ((TH1*) o["hHadRecoilFromTracksPx"])->Fill(event.mP3RecoilFromTracks.Px(), event.mHadRecoilFromTracksPt != 0);
   ((TH1*) o["hHadRecoilFromTracksPy"])->Fill(event.mP3RecoilFromTracks.Py(), event.mHadRecoilFromTracksPt != 0);
   ((TH1*) o["hHadRecoilFromTracksPz"])->Fill(event.mP3RecoilFromTracks.Pz(), event.mHadRecoilFromTracksPt != 0);
   ((TH1*) o["hHadRecoilFromTracksEta"])->Fill(event.mP3RecoilFromTracks.Eta(), event.mHadRecoilFromTracksPt != 0);
   ((TH1*) o["hHadRecoilFromTracksPt"])->Fill(event.mP3RecoilFromTracks.Pt(), event.mHadRecoilFromTracksPt != 0);
   ((TH2*) o["hRecoilCorrelPythia"])->Fill(event.mP3RecoilFromTracks.Pt(), event.mWEvent->mRecoilP4.Pt());

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
