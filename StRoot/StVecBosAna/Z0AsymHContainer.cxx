#include <math.h>

#include "Z0AsymHContainer.h"

#include "TF1.h"
#include "TF2.h"

#include "utils/utils.h"
#include "utils/MultiGraph.h"
#include "utils/H1D.h"
#include "utils/H2I.h"
#include "utils/H2D.h"

#include "AsymCalculator.h"
#include "ZBosEvent.h"
//#include "WBosEvent.h"


ClassImp(Z0AsymHContainer)

using namespace std;


/** Default constructor. */
Z0AsymHContainer::Z0AsymHContainer() : PlotHelper()
{
   BookHists();
}


Z0AsymHContainer::Z0AsymHContainer(TDirectory *dir, EAsymType asymType) : PlotHelper(dir), mAsymType(asymType)
{
   BookHists();
}


/** */
void Z0AsymHContainer::BookHists()
{
   string shName;
   TH1*   hist, *h;
   rh::MultiGraph* mg;

   DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();
   for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
   {
      string sDblSpinState = AsString(*iDSS);

      shName = "hZBosonPhiVsEta_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Z Boson #eta; Z Boson #phi;", 1, -6, 6, 3, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hZBosonPhiVsPt_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Z Boson P_{T}; Z Boson #phi;", 1, 0, 25, 3, -M_PI, M_PI);
      hist->SetOption("colz");
   }

   SingleSpinStateSetIter iSSS = gSingleSpinStateSet.begin();
   for ( ; iSSS!=gSingleSpinStateSet.end(); ++iSSS) 
   {
      string sSnglSpinState = AsString(*iSSS);

      shName = "hZBosonPhiVsEta_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Z Boson #eta; Z Boson #phi;", 1, -6, 6, 3, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hZBosonPhiVsPt_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Z Boson P_{T}; Z Boson #phi;", 1, 0, 25, 3, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hZBosonPhi_PtProj_" + sSnglSpinState;
      o[shName] = hist = new TH1I(shName.c_str(), "; Z Boson #phi; Events;", 3, -M_PI, M_PI);
      hist->SetOption("E1 GRIDX GRIDY");
   }

   BeamIdSetIter iBeam = gBeams.begin();
   for ( ; iBeam!=gBeams.end(); ++iBeam) 
   {
      string sBeam = AsString(*iBeam);

      // Z asymmetry plots
      shName = "hZBosonAsymVsPhiVsEta_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; Z Boson #eta; Z Boson #phi;", 1, -6, 6, 3, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hZBosonAsymAmpVsEta_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Z Boson #eta; Asym Amp.;", 1, -6, 6);
      hist->SetOption("E1 GRIDX GRIDY");
      hist->GetYaxis()->SetRangeUser(-1.5, 1.5);

      // Multigraph container with graphs for individual slices/bins
      shName = "mgrZBosonAsymVsPhi_EtaBins_" + sBeam;
      o[shName] = mg = new rh::MultiGraph(shName, shName);
      hist = new TH1F(shName.c_str(), "; Z Boson #phi; Asym.;", 1, -M_PI, M_PI);
      hist->GetYaxis()->SetRangeUser(-1.5, 1.5);
      mg->SetHistogram((TH1F*) hist);

      shName = "hZBosonAsymVsPhiVsPt_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; Z Boson P_{T}; Z Boson #phi;", 1, 0, 25, 3, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hZBosonAsymAmpVsPt_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Z Boson P_{T}; Asym Amp.;", 1, 0, 25);
      hist->SetOption("E1 GRIDX GRIDY");
      hist->GetYaxis()->SetRangeUser(-1.5, 1.5);

      // Multigraph container with graphs for individual slices/bins
      shName = "mgrZBosonAsymVsPhi_PtBins_" + sBeam;
      o[shName] = mg = new rh::MultiGraph(shName, shName);
      hist = new TH1F(shName.c_str(), "; Z Boson #phi; Asym.;", 1, -M_PI, M_PI);
      hist->GetYaxis()->SetRangeUser(-1.5, 1.5);
      mg->SetHistogram((TH1F*) hist);
   }

   shName = "hZBosonAsymAmpVsEta_";
   o[shName] = hist = new TH1D(shName.c_str(), "; Z Boson #eta; Asym Amp.;", 1, -6, 6);
   hist->SetOption("E1 GRIDX GRIDY");
   hist->GetYaxis()->SetRangeUser(-1.5, 1.5);

   shName = "hZBosonAsymAmpVsPt_";
   o[shName] = hist = new TH1D(shName.c_str(), "; Z Boson P_{T}; Asym Amp.;", 1, 0, 25);
   hist->SetOption("E1 GRIDX GRIDY");
   hist->GetYaxis()->SetRangeUser(-1.5, 1.5);

   fDir->cd();
}


/** */
void Z0AsymHContainer::Fill(ProtoEvent &ev)
{
   ZBosEvent& event = (ZBosEvent&) ev;

   if ( gDoubleSpinStateSet.find((EDoubleSpinState) event.mSpinPattern4Bits) == gDoubleSpinStateSet.end())
      return;

   EDoubleSpinState dblSpinState = (EDoubleSpinState) event.mSpinPattern4Bits;

   string sDblSpinState = AsString( dblSpinState );

   if (event.mTracksCandidate.size() > 1)
   {
      TLorentzVector zBoson = event.GetVecBosonP4();
      string shName;

      shName = "hZBosonPhiVsPt_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(zBoson.Pt(), zBoson.Phi());

      shName = "hZBosonPhiVsEta_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(zBoson.Eta(), zBoson.Phi());
   }
}


/** */
void Z0AsymHContainer::FillDerived()
{
   Info("FillDerived()", "Called");

   string shName;

   SingleSpinStateSetIter iSSS = gSingleSpinStateSet.begin();
   for ( ; iSSS!=gSingleSpinStateSet.end(); ++iSSS) 
   {
      ESingleSpinState sss  = *iSSS;
      string sSnglSpinState = AsString(sss);

      TH2* hZBosonPhiVsEta_sngl    = (TH2*) o["hZBosonPhiVsEta_"    + sSnglSpinState];
      TH2* hZBosonPhiVsPt_sngl     = (TH2*) o["hZBosonPhiVsPt_"     + sSnglSpinState];
      TH1* hZBosonPhi_PtProj_sngl  = (TH1*) o["hZBosonPhi_PtProj_"  + sSnglSpinState];

      DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();
      for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
      {
         EDoubleSpinState dss = *iDSS;
         string sDblSpinState = AsString(dss);

         TH2* hZBosonPhiVsEta_dbl    = (TH2*) o["hZBosonPhiVsEta_"    + sDblSpinState];
         TH2* hZBosonPhiVsPt_dbl     = (TH2*) o["hZBosonPhiVsPt_"     + sDblSpinState];

         if (dss & sss) {
            hZBosonPhiVsEta_sngl->Add(hZBosonPhiVsEta_dbl);
            hZBosonPhiVsPt_sngl->Add(hZBosonPhiVsPt_dbl);
         }
      }

      // Create projections on phi axis
      TH1D* hProjTmp = hZBosonPhiVsPt_sngl->ProjectionY();
      utils::CopyBinContentError(hProjTmp, hZBosonPhi_PtProj_sngl);
   }   
}


/** */
void Z0AsymHContainer::PostFill()
{
   Info("PostFill", "Called");

   AsymCalculator::sAsymType = mAsymType;

   BeamIdSetIter iBeam = gBeams.begin();
   for ( ; iBeam!=gBeams.end(); ++iBeam) 
   {
      string sBeam   = AsString(*iBeam);
      string sSpinUp = AsString(*iBeam, kSPIN_UP);
      string sSpinDn = AsString(*iBeam, kSPIN_DOWN);

      // Z asymmetry vs. boson eta for iBeam
      TH2I* hZBosonPhiVsEta_up     = (TH2I*) o["hZBosonPhiVsEta_" + sSpinUp];
      TH2I* hZBosonPhiVsEta_dn     = (TH2I*) o["hZBosonPhiVsEta_" + sSpinDn];
      TH2D* hZBosonAsymVsPhiVsEta_ = (TH2D*) o["hZBosonAsymVsPhiVsEta_" + sBeam];
      TH1D* hZBosonAsymAmpVsEta_   = (TH1D*) o["hZBosonAsymAmpVsEta_" + sBeam];
      rh::MultiGraph* mgrZBosonAsymVsPhi_EtaBins_ = (rh::MultiGraph*) o["mgrZBosonAsymVsPhi_EtaBins_" + sBeam];

      AsymCalculator::CalcAsimAsym(*hZBosonPhiVsEta_up, *hZBosonPhiVsEta_dn, *hZBosonAsymVsPhiVsEta_);
      AsymCalculator::FitAsimAsym(*hZBosonAsymVsPhiVsEta_, *hZBosonAsymAmpVsEta_, mgrZBosonAsymVsPhi_EtaBins_);

      // Z asymmetry vs. boson p_T for iBeam
      TH2I* hZBosonPhiVsPt_up     = (TH2I*) o["hZBosonPhiVsPt_" + sSpinUp];
      TH2I* hZBosonPhiVsPt_dn     = (TH2I*) o["hZBosonPhiVsPt_" + sSpinDn];
      TH2D* hZBosonAsymVsPhiVsPt_ = (TH2D*) o["hZBosonAsymVsPhiVsPt_" + sBeam];
      TH1D* hZBosonAsymAmpVsPt_   = (TH1D*) o["hZBosonAsymAmpVsPt_" + sBeam];
      rh::MultiGraph* mgrZBosonAsymVsPhi_PtBins_ = (rh::MultiGraph*) o["mgrZBosonAsymVsPhi_PtBins_" + sBeam];

      AsymCalculator::CalcAsimAsym(*hZBosonPhiVsPt_up, *hZBosonPhiVsPt_dn, *hZBosonAsymVsPhiVsPt_);
      AsymCalculator::FitAsimAsym(*hZBosonAsymVsPhiVsPt_, *hZBosonAsymAmpVsPt_, mgrZBosonAsymVsPhi_PtBins_);
   }

   // The Z boson asymmetry 
   TH1D* hZBosonAsymAmpVsPt_BLU  = (TH1D*) o["hZBosonAsymAmpVsPt_BLU"];
   TH1D* hZBosonAsymAmpVsPt_YEL  = (TH1D*) o["hZBosonAsymAmpVsPt_YEL"];
   TH1D* hZBosonAsymAmpVsPt_     = (TH1D*) o["hZBosonAsymAmpVsPt_"];
   AsymCalculator::CombineAsimAsym(*hZBosonAsymAmpVsPt_BLU, *hZBosonAsymAmpVsPt_YEL, *hZBosonAsymAmpVsPt_);

   TH1D* hZBosonAsymAmpVsEta_BLU = (TH1D*) o["hZBosonAsymAmpVsEta_BLU"];
   TH1D* hZBosonAsymAmpVsEta_YEL = (TH1D*) o["hZBosonAsymAmpVsEta_YEL"];
   TH1D* hZBosonAsymAmpVsEta_    = (TH1D*) o["hZBosonAsymAmpVsEta_"];
   AsymCalculator::CombineAsimAsym(*hZBosonAsymAmpVsEta_BLU, *hZBosonAsymAmpVsEta_YEL, *hZBosonAsymAmpVsEta_, true);

}
