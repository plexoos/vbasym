/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "EventDisplayHists.h"

#include "TF1.h"
#include "TF2.h"
#include "TMarker.h"

#include "WBosEvent.h"


ClassImp(EventDisplayHists)

using namespace std;


/** Default constructor. */
EventDisplayHists::EventDisplayHists() : PlotHelper()
{
   BookHists();
}


EventDisplayHists::EventDisplayHists(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


EventDisplayHists::EventDisplayHists(TDirectory *dir, const VecBosEvent &ev) : PlotHelper(dir)
{
   BookHists(ev);
}


/** Default destructor. */
EventDisplayHists::~EventDisplayHists()
{
}


/** */
void EventDisplayHists::BookHists()
{
}


/** */
void EventDisplayHists::BookHists(const VecBosEvent &ev)
{
   string shName;
   TH1*   hist;

   fDir->cd();

   //o["hBTowEnergy"] = hist = new TH2F("hBTowEnergy", "; #phi, Tower Id; #eta, Tower Id; Energy", 40, 0.5, 40.5, 120, 0.5, 120.5);
   o["hBTowEnergy"] = hist = new TH2F("hBTowEnergy", "; #eta, Tower Id; #phi, Tower Id; Energy", 40, -1, 1, 120, 0, 2*M_PI);
   hist->SetOption("COLZ");
}


/** */
void EventDisplayHists::Fill(ProtoEvent &ev)
{
   WBosEvent& wbEvent = (WBosEvent&) ev;

   TH2*  hBTowEnergy = ((TH2*) o["hBTowEnergy"]);
   Int_t softId, m, e, s;

   for (int iEtaBin=1; iEtaBin<=hBTowEnergy->GetNbinsX(); iEtaBin++)
   {
      for (int iPhiBin=1; iPhiBin<=hBTowEnergy->GetNbinsY(); iPhiBin++)
      {
         Double_t eta_c = hBTowEnergy->GetXaxis()->GetBinCenter(iEtaBin);
         Double_t phi_c = hBTowEnergy->GetYaxis()->GetBinCenter(iPhiBin);

         gBTowGeom->getId((Float_t) phi_c, (Float_t) eta_c, softId);
         //gBTowGeom->getBin((Float_t) phi_c, (Float_t) eta_c, m, e, s);
         //printf("phi_c: %f, eta_c: %f, softId: %d, m: %d, e: %d, s: %d\n", phi_c, eta_c, softId, m, e, s);
         hBTowEnergy->SetBinContent(iEtaBin, iPhiBin, wbEvent.bemc.eneTile[kBTow][softId - 1]);
      }
   }

   const VecBosTrack  &eleTrack = wbEvent.GetElectronTrack();
   const VecBosVertex *vbVertex = wbEvent.GetVertexById(eleTrack.GetVertexId());

   eleTrack.Print("electron");
   vbVertex->Print();

   //TVector3 eleP3(eleTrack.GetCoordAtBTow() + vbVertex->mPosition);
   TVector3 eleP3(eleTrack.GetCoordAtBTow() );

   //TMarker *marker = new TMarker(wbEvent.GetElectronP3().Eta(), wbEvent.GetElectronP3().Phi(), 29);
   TMarker *marker = new TMarker(eleP3.Eta(), fmod(2*M_PI + eleP3.Phi(), 2*M_PI), 30);
   marker->SetMarkerColor(kRed);
   marker->SetMarkerSize(4);
 
   hBTowEnergy->GetListOfFunctions()->SetOwner(kTRUE);
   hBTowEnergy->GetListOfFunctions()->Add(marker);

   TVector3 cluP3( eleTrack.mCluster2x2.position );

   TMarker *markerCluster = new TMarker(cluP3.Eta(), fmod(2*M_PI + cluP3.Phi(), 2*M_PI), 30);
   markerCluster->SetMarkerColor(kBlue);
   markerCluster->SetMarkerSize(4);
   hBTowEnergy->GetListOfFunctions()->Add(markerCluster);
}
