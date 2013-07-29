/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "EventDisplayHists.h"

#include "TF1.h"
#include "TF2.h"

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

   o["hBTowEnergy"] = hist = new TH2F("hBTowEnergy", "; #phi, Tower Id; #eta, Tower Id; Energy", 40, 0.5, 40.5, 120, 0.5, 120.5);
   hist->SetOption("COLZ");
}


/** */
void EventDisplayHists::Fill(ProtoEvent &ev)
{
   WBosEvent& wbEvent = (WBosEvent&) ev;

   TH2*  hBTowEnergy = ((TH2*) o["hBTowEnergy"]);
   Int_t softId;

   for (int iEtaBin=1; iEtaBin<=hBTowEnergy->GetNbinsX(); iEtaBin++)
   {
      for (int iPhiBin=1; iPhiBin<=hBTowEnergy->GetNbinsY(); iPhiBin++)
      {
         Double_t eta_c = hBTowEnergy->GetXaxis()->GetBinCenter(iEtaBin);
         Double_t phi_c = hBTowEnergy->GetYaxis()->GetBinCenter(iPhiBin);

         gBTowGeom->getId((Float_t) phi_c, (Float_t) eta_c, softId);
         //hBTowEnergy->SetBinContent(iEtaBin, iPhiBin, wbEvent.bemc.adcTile[kBTow][softId - 1]);
      }
   }
}
