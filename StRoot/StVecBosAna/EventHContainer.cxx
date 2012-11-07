/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "EventHContainer.h"

#include "TF1.h"

#include "VecBosEvent.h"


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

   o["hNumJets"] = hist = new TH1I("hNumJets", "; Num. of Jets; Events", 20, 0, 20);
   hist->SetOption("hist GRIDX");

   o["hNumVertices"] = hist = new TH1I("hNumVertices", "; Num. of Vertices; Events", 10, 0, 10);
   hist->SetOption("hist GRIDX");

   o["hNumTracks"] = hist = new TH1I("hNumTracks", "; Num. of Tracks; Events", 50, 0, 150);
   hist->SetOption("hist GRIDX");

   o["hNumTracksWithBCluster"] = hist = new TH1I("hNumTracksWithBCluster", "; Num. of Tracks with Barrel Cluster; Events", 5, 0, 5);
   hist->SetOption("hist GRIDX");

   o["hNumTracksWithBCluster2"] = hist = new TH1I("hNumTracksWithBCluster2", "; Num. of Tracks with Barrel Cluster; Events", 5, 0, 5);
   hist->SetOption("hist GRIDX");

   //shName = "hMassFitChi2ByChannel";
   //o[shName] = new TH1F(shName.c_str(), shName.c_str(), N_SILICON_CHANNELS, 0.5, N_SILICON_CHANNELS+0.5);
   //((TH1*) o[shName])->SetTitle("; Channel Id; #chi^{2};");
   //((TH1*) o[shName])->SetOption("hist GRIDX");

   //shName = "hMassFitMeanByChannel";
   //o[shName] = new TH1F(shName.c_str(), shName.c_str(), N_SILICON_CHANNELS, 0.5, N_SILICON_CHANNELS+0.5);
   //((TH1*) o[shName])->SetTitle("; Channel Id; Mean Mass;");
   //((TH1*) o[shName])->SetOption("E1 GRIDX");

   //shName = "hMassFitSigmaByChannel";
   //o[shName] = new TH1F(shName.c_str(), shName.c_str(), N_SILICON_CHANNELS, 0.5, N_SILICON_CHANNELS+0.5);
   //((TH1*) o[shName])->SetTitle("; Channel Id; Mass Width;");
   //((TH1*) o[shName])->SetOption("E1 GRIDX");

   //DrawObjContainer        *oc;
   //DrawObjContainerMapIter  isubdir;

   //ChannelSetIter iCh = gMeasInfo->fSiliconChannels.begin();

   //for (; iCh!=gMeasInfo->fSiliconChannels.end(); ++iCh) {

   //   string sChId("  ");
   //   sprintf(&sChId[0], "%02d", *iCh);

   //   string dName = "channel" + sChId;

   //   isubdir = d.find(dName);

   //   if ( isubdir == d.end()) { // if dir not found
   //      oc = new DrawObjContainer();
   //      oc->fDir = new TDirectoryFile(dName.c_str(), dName.c_str(), "", fDir);
   //   } else {
   //      oc = isubdir->second;
   //   }

   //   oc->fDir->cd();

   //   shName = "hPseudoMass_ch" + sChId;
   //   hist = new TH1F(shName.c_str(), shName.c_str(), 50, 0, 20);
   //   hist->SetTitle("; Mass; Events;");
   //   hist->SetOption("E1");
   //   oc->o[shName] = hist;
   //   fhPseudoMass_ch[*iCh-1] = hist;

   //   // If this is a new directory then we need to add it to the list
   //   if ( isubdir == d.end()) {
   //      d[dName] = oc;
   //   }
   //}
}


/** */
void EventHContainer::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;

   ((TH1*) o["hNumJets"])->Fill(event.GetNumJets());
   ((TH1*) o["hNumVertices"])->Fill(event.mVertices.size());
   ((TH1*) o["hNumTracks"])->Fill(event.GetNumTracks());
   ((TH1*) o["hNumTracksWithBCluster"])->Fill(event.GetNumTracksWithBCluster());
   ((TH1*) o["hNumTracksWithBCluster2"])->Fill(event.GetNumTracksWithBCluster2());
}


/** */
void EventHContainer::FillDerived()
{
   Info("FillDerived()", "Called");

   //TH1* hPseudoMass = (TH1*) o["hPseudoMass"];
   //
   //for (UShort_t iCh=1; iCh<=N_SILICON_CHANNELS; iCh++) {

   //   string sChId("  ");
   //   sprintf(&sChId[0], "%02d", iCh);

   //   DrawObjContainer *oc = d.find("channel" + sChId)->second;

   //   TH1* hPseudoMass_ch = (TH1*) oc->o["hPseudoMass_ch" + sChId];
   //   hPseudoMass->Add(hPseudoMass_ch);
   //}
}


/** */
void EventHContainer::PostFill()
{
   Info("PostFill", "Called");

   //// Fit energy slope with an exponential func
   //TF1 *fitfunc = new TF1("fitfunc", "gaus", 9, 14);

   ////fitfunc->SetParNames("slope");
   ////fitfunc->SetParameter(0, 0);
   ////fitfunc->SetParameter(1, 0);
   ////fitfunc->SetParLimits(1, -1, 1);

   //TH1* hPseudoMass = (TH1*) o["hPseudoMass"];
   //
   //if (hPseudoMass->Integral() <= 0) {
   //   Error("PostFill", "Mass distribution is empty");
   //   return;
   //}

   //TFitResultPtr fitres = hPseudoMass->Fit("fitfunc", "M E S R");

   //if ( fitres.Get() && fitres->Ndf()) {
   //   gAnaMeasResult->fFitResPseudoMass = fitres;
   //} else {
   //   Error("PostFill", "Something is wrong with mass fit");
   //   hPseudoMass->GetListOfFunctions()->Clear();
   //}

   //TH1* hMassFitChi2ByChannel  = (TH1*) o["hMassFitChi2ByChannel"];
   //TH1* hMassFitMeanByChannel  = (TH1*) o["hMassFitMeanByChannel"];
   //TH1* hMassFitSigmaByChannel = (TH1*) o["hMassFitSigmaByChannel"];

   //// Fit channel histograms
   //ChannelSetIter iCh = gMeasInfo->fSiliconChannels.begin();

   //for (; iCh!=gMeasInfo->fSiliconChannels.end(); ++iCh) {

   //   string sChId("  ");
   //   sprintf(&sChId[0], "%02d", *iCh);

   //   DrawObjContainer *oc_ch = d.find("channel" + sChId)->second;

   //   TH1* hPseudoMass_ch = (TH1*) oc_ch->o["hPseudoMass_ch" + sChId];
   //
   //   if (hPseudoMass_ch->Integral() <= 0) {
   //      Error("PostFill", "Mass distribution for channel %s is empty", sChId.c_str());
   //      continue;
   //   }

   //   TF1 *fitfunc = new TF1("fitfunc", "gaus", 8, 14);

   //   //fitfunc->SetParNames("slope");
   //   //fitfunc->SetParameter(0, 0);
   //   //fitfunc->SetParameter(1, 0);
   //   //fitfunc->SetParLimits(1, -1, 1);

   //   TFitResultPtr fitres = hPseudoMass_ch->Fit("fitfunc", "M E S R");

   //   if ( fitres.Get() && fitres->Ndf()) {
   //      // check ndf
   //      //if (fitres->Ndf() <= 0) continue;

   //      hMassFitChi2ByChannel ->SetBinContent(*iCh, fitres->Chi2()/fitres->Ndf() );
   //      hMassFitMeanByChannel ->SetBinContent(*iCh, fitres->Value(1) );
   //      hMassFitMeanByChannel ->SetBinError  (*iCh, fitres->FitResult::Error(1) );
   //      hMassFitSigmaByChannel->SetBinContent(*iCh, fitres->Value(2) );
   //      hMassFitSigmaByChannel->SetBinError  (*iCh, fitres->FitResult::Error(2) );
   //   } else {
   //      Error("PostFill", "Something is wrong with mass fit for channel %s", sChId.c_str());
   //      hPseudoMass_ch->GetListOfFunctions()->Clear();
   //   }
   //}
}
