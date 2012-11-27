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

   //   o["hNeutrinoEnergy"] = hist = new TH1I("hNeutrinoEnergy", "; #nu energy [GeV]; Events", 50, 20., 200.);
   //   hist->SetOption("hist GRIDX");

   o["hNeutrinoMomentumX"] = hist = new TH1I("hNeutrinoMomentumX", "; P^{#nu}_{x} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoMomentumY"] = hist = new TH1I("hNeutrinoMomentumY", "; P^{#nu}_{y} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoMomentumZ"] = hist = new TH1I("hNeutrinoMomentumZ", "; P^{#nu}_{z} [GeV/c]; Events", 100, -150., 150.);
   hist->SetOption("hist GRIDX");
   o["hNeutrinoPt"] = hist = new TH1I("hNeutrinoPt", "; P^{#nu}_{T} [GeV/c]; Events", 100, 0., 150.);
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

   // MC correction variables
   o["hEnergyRatio"] = hist = new TH1I("hEnergyRatio", "; E_{in}^{Recoil}/E_{out}^{Recoil}; Events", 100, 0., 4.);
   hist->SetOption("hist GRIDX");


   // hist->SetOption("hist GRIDX GRIDY");

}


void MCHContainer::Fill(ProtoEvent &ev)
{
  
   VecBosEvent& event = (VecBosEvent&) ev;

   //float rapW; float eW;

   ((TH1*) o["hWenergy"])->Fill(event.eW);
   ((TH1*) o["hWmomentumX"])->Fill(event.mWP.x());
   ((TH1*) o["hWmomentumY"])->Fill(event.mWP.y());
   ((TH1*) o["hWmomentumZ"])->Fill(event.mWP.z());
   ((TH1*) o["hWpt"])->Fill(event.mWP.Perp());
   ((TH1*) o["hWrapidity"])->Fill(event.rapW);

   //((TH1*) o["hNeutrinoEnergy"])->Fill(event.eNeutrino);
   ((TH1*) o["hNeutrinoMomentumX"])->Fill(event.mNeutrinoP.x());
   ((TH1*) o["hNeutrinoMomentumY"])->Fill(event.mNeutrinoP.y());
   ((TH1*) o["hNeutrinoMomentumZ"])->Fill(event.mNeutrinoP.z());
   ((TH1*) o["hNeutrinoPt"])->Fill(event.mNeutrinoP.Perp());

   ((TH1*) o["hElectronMomentumX"])->Fill(event.mElectronP.x());
   ((TH1*) o["hElectronMomentumY"])->Fill(event.mElectronP.y());
   ((TH1*) o["hElectronMomentumZ"])->Fill(event.mElectronP.z());
   ((TH1*) o["hElectronPt"])->Fill(event.mElectronP.Perp());

   // recoil variables
   ((TH1*) o["hRecEnergy"])->Fill(event.recoil.e());
   ((TH1*) o["hRecMomentumX"])->Fill(event.recoil.px());
   ((TH1*) o["hRecMomentumY"])->Fill(event.recoil.py());
   ((TH1*) o["hRecMomentumZ"])->Fill(event.recoil.pz());
   ((TH1*) o["hRecInAccEnergy"])->Fill(event.recoilInAccept.e());
   ((TH1*) o["hRecInAccMomentumX"])->Fill(event.recoilInAccept.px());
   ((TH1*) o["hRecInAccMomentumY"])->Fill(event.recoilInAccept.py());
   ((TH1*) o["hRecInAccMomentumZ"])->Fill(event.recoilInAccept.pz());
   ((TH1*) o["hRecOutAccEnergy"])->Fill(event.recoilOutAccept.e());
   ((TH1*) o["hRecOutAccMomentumX"])->Fill(event.recoilOutAccept.px());
   ((TH1*) o["hRecOutAccMomentumY"])->Fill(event.recoilOutAccept.py());
   ((TH1*) o["hRecOutAccMomentumZ"])->Fill(event.recoilOutAccept.pz());

   // MC correction variablel
   ((TH1*) o["hEnergyRatio"])->Fill(event.fEnergyRatio);
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
