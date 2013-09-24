#include <math.h>

#include "AsymHContainer.h"

#include "TF1.h"
#include "TF2.h"

#include "utils/utils.h"
#include "utils/MultiGraph.h"

#include "AsymCalculator.h"
#include "WBosEvent.h"


ClassImp(AsymHContainer)

using namespace std;


/** Default constructor. */
AsymHContainer::AsymHContainer() : PlotHelper()
{
   BookHists();
}


AsymHContainer::AsymHContainer(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


/** Default destructor. */
AsymHContainer::~AsymHContainer()
{
}


/** */
void AsymHContainer::BookHists()
{
   string shName;
   TH1*   hist;
   rh::MultiGraph* mg;

   DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();
   for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
   {
      string sDblSpinState = AsString(*iDSS);

      shName = "hLeptonPhiVsEta_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonPhiVsEta_b2_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 2, -1.2, 1.2, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonPhiVsPt_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton P_{T}; Lepton #phi;", 10, 0, 50, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hWBosonPhiVsEta_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; W Boson #eta; W Boson #phi;", 6, -6, 6, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hWBosonPhiVsPt_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; W Boson P_{T}; W Boson #phi;", 10, 0, 10, 8, -M_PI, M_PI);
      hist->SetOption("colz");
   }

   SingleSpinStateSetIter iSSS = gSingleSpinStateSet.begin();
   for ( ; iSSS!=gSingleSpinStateSet.end(); ++iSSS) 
   {
      string sSnglSpinState = AsString(*iSSS);

      shName = "hLeptonPhiVsEta_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonPhiVsEta_b2_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 2, -1.2, 1.2, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonPhi_EtaProj_" + sSnglSpinState;
      o[shName] = hist = new TH1I(shName.c_str(), "; Lepton #phi; Events;", 8, -M_PI, M_PI);
      hist->SetOption("E1 GRIDX GRIDY");

      shName = "hLeptonPhiVsPt_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton P_{T}; Lepton #phi;", 10, 0, 50, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonPhi_PtProj_" + sSnglSpinState;
      o[shName] = hist = new TH1I(shName.c_str(), "; Lepton #phi; Events;", 8, -M_PI, M_PI);
      hist->SetOption("E1 GRIDX GRIDY");

      shName = "hWBosonPhiVsEta_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; W Boson #eta; W Boson #phi;", 6, -6, 6, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hWBosonPhiVsPt_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; W Boson P_{T}; W Boson #phi;", 10, 0, 10, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hWBosonPhi_PtProj_" + sSnglSpinState;
      o[shName] = hist = new TH1I(shName.c_str(), "; W Boson #phi; Events;", 8, -M_PI, M_PI);
      hist->SetOption("E1 GRIDX GRIDY");
   }

   BeamIdSetIter iBeam = gBeams.begin();
   for ( ; iBeam!=gBeams.end(); ++iBeam) 
   {
      string sBeam = AsString(*iBeam);

      // Lepton asymmetry plots
      shName = "hLeptonAsymVsPhi_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Lepton #phi; Asym;", 8, -M_PI, M_PI);
      hist->SetOption("E1 GRIDX GRIDY");

      shName = "hLeptonAsymVsPhiVsEta_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      // The same as above but with different binning
      shName = "hLeptonAsymVsPhiVsEta_b2_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; Lepton #eta; Lepton #phi;", 2, -1.2, 1.2, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      // Lepton asymmetry amplitude vs eta
      shName = "hLeptonAsymAmpVsEta_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Lepton #eta; Asym Amp.;", 3, -1.5, 1.5);
      hist->SetOption("E1 GRIDX GRIDY");
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

      // The same as the previous one but with different binning
      shName = "hLeptonAsymAmpVsEta_b2_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Lepton #eta; Asym Amp.;", 2, -1.2, 1.2);
      hist->SetOption("E1 GRIDX GRIDY");
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

      // Multigraph container with graphs for individual slices/bins
      shName = "mgrLeptonAsymVsPhi_EtaBins_" + sBeam;
      o[shName] = mg = new rh::MultiGraph(shName, shName);
      hist = new TH1F(shName.c_str(), "; Lepton #phi; Asym.;", 1, -M_PI, M_PI);
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);
      mg->SetHistogram((TH1F*) hist);

      // Lepton asymmetry vs p_T
      shName = "hLeptonAsymVsPhiVsPt_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; Lepton P_{T}; Lepton #phi;", 10, 0, 50, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      // Lepton asymmetry amplitude vs p_T
      shName = "hLeptonAsymAmpVsPt_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Lepton P_{T}; Asym Amp.;", 10, 0, 50);
      hist->SetOption("E1 GRIDX GRIDY");
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

      // Multigraph container with graphs for individual slices/bins
      shName = "mgrLeptonAsymVsPhi_PtBins_" + sBeam;
      o[shName] = mg = new rh::MultiGraph(shName, shName);
      hist = new TH1F(shName.c_str(), "; Lepton #phi; Asym.;", 1, -M_PI, M_PI);
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);
      mg->SetHistogram((TH1F*) hist);

      // W asymmetry plots
      shName = "hWBosonAsymVsPhiVsEta_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; W Boson #eta; W Boson #phi;", 6, -6, 6, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hWBosonAsymAmpVsEta_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; W Boson #eta; Asym Amp.;", 6, -6, 6);
      hist->SetOption("E1 GRIDX GRIDY");
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

      // Multigraph container with graphs for individual slices/bins
      shName = "mgrWBosonAsymVsPhi_EtaBins_" + sBeam;
      o[shName] = mg = new rh::MultiGraph(shName, shName);
      hist = new TH1F(shName.c_str(), "; W Boson #phi; Asym.;", 1, -M_PI, M_PI);
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);
      mg->SetHistogram((TH1F*) hist);

      shName = "hWBosonAsymVsPhiVsPt_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; W Boson P_{T}; W Boson #phi;", 10, 0, 10, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hWBosonAsymAmpVsPt_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; W Boson P_{T}; Asym Amp.;", 10, 0, 10);
      hist->SetOption("E1 GRIDX GRIDY");
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

      // Multigraph container with graphs for individual slices/bins
      shName = "mgrWBosonAsymVsPhi_PtBins_" + sBeam;
      o[shName] = mg = new rh::MultiGraph(shName, shName);
      hist = new TH1F(shName.c_str(), "; W Boson #phi; Asym.;", 1, -M_PI, M_PI);
      hist->GetYaxis()->SetRangeUser(-0.5, 0.5);
      mg->SetHistogram((TH1F*) hist);
   }

   shName = "hLeptonAsymAmpVsEta_";
   o[shName] = hist = new TH1D(shName.c_str(), "; Lepton #eta; Asym Amp.;", 3, -1.5, 1.5);
   hist->SetOption("E1 GRIDX GRIDY");
   hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

   // The same as above but with different binning
   shName = "hLeptonAsymAmpVsEta_b2_";
   o[shName] = hist = new TH1D(shName.c_str(), "; Lepton #eta; Asym Amp.;", 2, -1.2, 1.2);
   hist->SetOption("E1 GRIDX GRIDY");
   hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

   shName = "hLeptonAsymAmpVsPt_";
   o[shName] = hist = new TH1D(shName.c_str(), "; Lepton P_{T}; Asym Amp.;", 10, 0, 50);
   hist->SetOption("E1 GRIDX GRIDY");
   hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

   shName = "hWBosonAsymAmpVsEta_";
   o[shName] = hist = new TH1D(shName.c_str(), "; W Boson #eta; Asym Amp.;", 6, -6, 6);
   hist->SetOption("E1 GRIDX GRIDY");
   hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

   shName = "hWBosonAsymAmpVsPt_";
   o[shName] = hist = new TH1D(shName.c_str(), "; W Boson P_{T}; Asym Amp.;", 10, 0, 10);
   hist->SetOption("E1 GRIDX GRIDY");
   hist->GetYaxis()->SetRangeUser(-0.5, 0.5);

   fDir->cd();
}


/** */
void AsymHContainer::Fill(ProtoEvent &ev)
{
   WBosEvent& event = (WBosEvent&) ev;

   if ( gDoubleSpinStateSet.find((EDoubleSpinState) event.mSpinPattern4Bits) == gDoubleSpinStateSet.end())
      return;

   EDoubleSpinState dblSpinState = (EDoubleSpinState) event.mSpinPattern4Bits;

   string sDblSpinState = AsString( dblSpinState );

   if (event.mTracksCandidate.size() > 0)
   {
      TVector3 eleCandidate = event.GetElectronP3();
      string shName;

      shName = "hLeptonPhiVsEta_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(eleCandidate.Eta(), eleCandidate.Phi());

      shName = "hLeptonPhiVsEta_b2_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(eleCandidate.Eta(), eleCandidate.Phi());

      shName = "hLeptonPhiVsPt_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(eleCandidate.Pt(), eleCandidate.Phi());

      TVector3 wBoson = event.GetVecBosonP3();

      shName = "hWBosonPhiVsPt_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(wBoson.Pt(), wBoson.Phi());

      shName = "hWBosonPhiVsEta_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(wBoson.Eta(), wBoson.Phi());
   }
}


/** */
void AsymHContainer::FillDerived()
{
   Info("FillDerived()", "Called");

   string shName;

   SingleSpinStateSetIter iSSS = gSingleSpinStateSet.begin();
   for ( ; iSSS!=gSingleSpinStateSet.end(); ++iSSS) 
   {
      ESingleSpinState sss  = *iSSS;
      string sSnglSpinState = AsString(sss);

      TH2* hLeptonPhiVsEta_sngl    = (TH2*) o["hLeptonPhiVsEta_"    + sSnglSpinState];
      TH2* hLeptonPhiVsEta_b2_sngl = (TH2*) o["hLeptonPhiVsEta_b2_" + sSnglSpinState];
      TH1* hLeptonPhi_EtaProj_sngl = (TH1*) o["hLeptonPhi_EtaProj_" + sSnglSpinState];
      TH2* hLeptonPhiVsPt_sngl     = (TH2*) o["hLeptonPhiVsPt_"     + sSnglSpinState];
      TH1* hLeptonPhi_PtProj_sngl  = (TH1*) o["hLeptonPhi_PtProj_"  + sSnglSpinState];
      TH2* hWBosonPhiVsEta_sngl    = (TH2*) o["hWBosonPhiVsEta_"    + sSnglSpinState];
      //TH1* hWBosonPhi_EtaProj_sngl = (TH1*) o["hWBosonPhi_EtaProj_" + sSnglSpinState];
      TH2* hWBosonPhiVsPt_sngl     = (TH2*) o["hWBosonPhiVsPt_"     + sSnglSpinState];
      TH1* hWBosonPhi_PtProj_sngl  = (TH1*) o["hWBosonPhi_PtProj_"  + sSnglSpinState];

      DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();
      for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
      {
         EDoubleSpinState dss = *iDSS;
         string sDblSpinState = AsString(dss);

         TH2* hLeptonPhiVsEta_dbl    = (TH2*) o["hLeptonPhiVsEta_"    + sDblSpinState];
         TH2* hLeptonPhiVsEta_b2_dbl = (TH2*) o["hLeptonPhiVsEta_b2_" + sDblSpinState];
         TH2* hLeptonPhiVsPt_dbl     = (TH2*) o["hLeptonPhiVsPt_"     + sDblSpinState];
         TH2* hWBosonPhiVsEta_dbl    = (TH2*) o["hWBosonPhiVsEta_"    + sDblSpinState];
         TH2* hWBosonPhiVsPt_dbl     = (TH2*) o["hWBosonPhiVsPt_"     + sDblSpinState];

         if (dss & sss) {
            hLeptonPhiVsEta_sngl->Add(hLeptonPhiVsEta_dbl);
            hLeptonPhiVsEta_b2_sngl->Add(hLeptonPhiVsEta_b2_dbl);
            hLeptonPhiVsPt_sngl->Add(hLeptonPhiVsPt_dbl);
            hWBosonPhiVsEta_sngl->Add(hWBosonPhiVsEta_dbl);
            hWBosonPhiVsPt_sngl->Add(hWBosonPhiVsPt_dbl);
         }
      }

      // Create projections on phi axis
      TH1D* hProjTmp = hLeptonPhiVsEta_sngl->ProjectionY();
      utils::CopyBinContentError(hProjTmp, hLeptonPhi_EtaProj_sngl);

      hProjTmp = hLeptonPhiVsPt_sngl->ProjectionY();
      utils::CopyBinContentError(hProjTmp, hLeptonPhi_PtProj_sngl);

      hProjTmp = hWBosonPhiVsPt_sngl->ProjectionY();
      utils::CopyBinContentError(hProjTmp, hWBosonPhi_PtProj_sngl);
   }
}


/** */
void AsymHContainer::PostFill()
{
   Info("PostFill", "Called");

   TH1I* hLeptonPhi_BU        = (TH1I*) o["hLeptonPhi_EtaProj_u0"];
   TH1I* hLeptonPhi_BD        = (TH1I*) o["hLeptonPhi_EtaProj_d0"];
   TH1D* hLeptonAsymVsPhi_BLU = (TH1D*) o["hLeptonAsymVsPhi_BLU"];

   AsymCalculator::CalcAsimAsym(*hLeptonPhi_BU, *hLeptonPhi_BD, *hLeptonAsymVsPhi_BLU);
   AsymCalculator::FitAsimAsym(*hLeptonAsymVsPhi_BLU);

   TH1I* hLeptonPhi_YU        = (TH1I*) o["hLeptonPhi_EtaProj_0u"];
   TH1I* hLeptonPhi_YD        = (TH1I*) o["hLeptonPhi_EtaProj_0d"];
   TH1D* hLeptonAsymVsPhi_YEL = (TH1D*) o["hLeptonAsymVsPhi_YEL"];

   AsymCalculator::CalcAsimAsym(*hLeptonPhi_YU, *hLeptonPhi_YD, *hLeptonAsymVsPhi_YEL);
   AsymCalculator::FitAsimAsym(*hLeptonAsymVsPhi_YEL);

   BeamIdSetIter iBeam = gBeams.begin();
   for ( ; iBeam!=gBeams.end(); ++iBeam) 
   {
      string sBeam   = AsString(*iBeam);
      string sSpinUp = AsString(*iBeam, kSPIN_UP);
      string sSpinDn = AsString(*iBeam, kSPIN_DOWN);

      // Lepton asymmetry vs. lepton eta for iBeam
      TH2I* hLeptonPhiVsEta_up     = (TH2I*) o["hLeptonPhiVsEta_" + sSpinUp];
      TH2I* hLeptonPhiVsEta_dn     = (TH2I*) o["hLeptonPhiVsEta_" + sSpinDn];
      TH2D* hLeptonAsymVsPhiVsEta_ = (TH2D*) o["hLeptonAsymVsPhiVsEta_" + sBeam];
      TH1D* hLeptonAsymAmpVsEta_   = (TH1D*) o["hLeptonAsymAmpVsEta_" + sBeam];
      rh::MultiGraph* mgrLeptonAsymVsPhi_EtaBins_ = (rh::MultiGraph*) o["mgrLeptonAsymVsPhi_EtaBins_" + sBeam];

      AsymCalculator::CalcAsimAsym(*hLeptonPhiVsEta_up, *hLeptonPhiVsEta_dn, *hLeptonAsymVsPhiVsEta_);
      AsymCalculator::FitAsimAsym(*hLeptonAsymVsPhiVsEta_, *hLeptonAsymAmpVsEta_, mgrLeptonAsymVsPhi_EtaBins_);

      // The same as above but with different binning
      TH2I* hLeptonPhiVsEta_b2_up     = (TH2I*) o["hLeptonPhiVsEta_b2_" + sSpinUp];
      TH2I* hLeptonPhiVsEta_b2_dn     = (TH2I*) o["hLeptonPhiVsEta_b2_" + sSpinDn];
      TH2D* hLeptonAsymVsPhiVsEta_b2_ = (TH2D*) o["hLeptonAsymVsPhiVsEta_b2_" + sBeam];
      TH1D* hLeptonAsymAmpVsEta_b2_   = (TH1D*) o["hLeptonAsymAmpVsEta_b2_"   + sBeam];

      AsymCalculator::CalcAsimAsym(*hLeptonPhiVsEta_b2_up, *hLeptonPhiVsEta_b2_dn, *hLeptonAsymVsPhiVsEta_b2_);
      AsymCalculator::FitAsimAsym(*hLeptonAsymVsPhiVsEta_b2_, *hLeptonAsymAmpVsEta_b2_);

      // Lepton asymmetry vs. lepton p_T for iBeam
      TH2I* hLeptonPhiVsPt_up     = (TH2I*) o["hLeptonPhiVsPt_" + sSpinUp];
      TH2I* hLeptonPhiVsPt_dn     = (TH2I*) o["hLeptonPhiVsPt_" + sSpinDn];
      TH2D* hLeptonAsymVsPhiVsPt_ = (TH2D*) o["hLeptonAsymVsPhiVsPt_" + sBeam];
      TH1D* hLeptonAsymAmpVsPt_   = (TH1D*) o["hLeptonAsymAmpVsPt_" + sBeam];
      rh::MultiGraph* mgrLeptonAsymVsPhi_PtBins_ = (rh::MultiGraph*) o["mgrLeptonAsymVsPhi_PtBins_" + sBeam];

      AsymCalculator::CalcAsimAsym(*hLeptonPhiVsPt_up, *hLeptonPhiVsPt_dn, *hLeptonAsymVsPhiVsPt_);
      AsymCalculator::FitAsimAsym(*hLeptonAsymVsPhiVsPt_, *hLeptonAsymAmpVsPt_, mgrLeptonAsymVsPhi_PtBins_);

      // W asymmetry vs. boson eta for iBeam
      TH2I* hWBosonPhiVsEta_up     = (TH2I*) o["hWBosonPhiVsEta_" + sSpinUp];
      TH2I* hWBosonPhiVsEta_dn     = (TH2I*) o["hWBosonPhiVsEta_" + sSpinDn];
      TH2D* hWBosonAsymVsPhiVsEta_ = (TH2D*) o["hWBosonAsymVsPhiVsEta_" + sBeam];
      TH1D* hWBosonAsymAmpVsEta_   = (TH1D*) o["hWBosonAsymAmpVsEta_" + sBeam];
      rh::MultiGraph* mgrWBosonAsymVsPhi_EtaBins_ = (rh::MultiGraph*) o["mgrWBosonAsymVsPhi_EtaBins_" + sBeam];

      AsymCalculator::CalcAsimAsym(*hWBosonPhiVsEta_up, *hWBosonPhiVsEta_dn, *hWBosonAsymVsPhiVsEta_);
      AsymCalculator::FitAsimAsym(*hWBosonAsymVsPhiVsEta_, *hWBosonAsymAmpVsEta_, mgrWBosonAsymVsPhi_EtaBins_);

      // W asymmetry vs. boson p_T for iBeam
      TH2I* hWBosonPhiVsPt_up     = (TH2I*) o["hWBosonPhiVsPt_" + sSpinUp];
      TH2I* hWBosonPhiVsPt_dn     = (TH2I*) o["hWBosonPhiVsPt_" + sSpinDn];
      TH2D* hWBosonAsymVsPhiVsPt_ = (TH2D*) o["hWBosonAsymVsPhiVsPt_" + sBeam];
      TH1D* hWBosonAsymAmpVsPt_   = (TH1D*) o["hWBosonAsymAmpVsPt_" + sBeam];
      rh::MultiGraph* mgrWBosonAsymVsPhi_PtBins_ = (rh::MultiGraph*) o["mgrWBosonAsymVsPhi_PtBins_" + sBeam];

      AsymCalculator::CalcAsimAsym(*hWBosonPhiVsPt_up, *hWBosonPhiVsPt_dn, *hWBosonAsymVsPhiVsPt_);
      AsymCalculator::FitAsimAsym(*hWBosonAsymVsPhiVsPt_, *hWBosonAsymAmpVsPt_, mgrWBosonAsymVsPhi_PtBins_);
   }

   // The lepton asymmetry first
   TH1D* hLeptonAsymAmpVsPt_BLU = (TH1D*) o["hLeptonAsymAmpVsPt_BLU"];
   TH1D* hLeptonAsymAmpVsPt_YEL = (TH1D*) o["hLeptonAsymAmpVsPt_YEL"];
   TH1D* hLeptonAsymAmpVsPt_    = (TH1D*) o["hLeptonAsymAmpVsPt_"];
   AsymCalculator::CombineAsimAsym(*hLeptonAsymAmpVsPt_BLU, *hLeptonAsymAmpVsPt_YEL, *hLeptonAsymAmpVsPt_);

   TH1D* hLeptonAsymAmpVsEta_BLU = (TH1D*) o["hLeptonAsymAmpVsEta_BLU"];
   TH1D* hLeptonAsymAmpVsEta_YEL = (TH1D*) o["hLeptonAsymAmpVsEta_YEL"];
   TH1D* hLeptonAsymAmpVsEta_    = (TH1D*) o["hLeptonAsymAmpVsEta_"];
   AsymCalculator::CombineAsimAsym(*hLeptonAsymAmpVsEta_BLU, *hLeptonAsymAmpVsEta_YEL, *hLeptonAsymAmpVsEta_, true);

   // The same as above but with different binning
   TH1D* hLeptonAsymAmpVsEta_b2_BLU = (TH1D*) o["hLeptonAsymAmpVsEta_b2_BLU"];
   TH1D* hLeptonAsymAmpVsEta_b2_YEL = (TH1D*) o["hLeptonAsymAmpVsEta_b2_YEL"];
   TH1D* hLeptonAsymAmpVsEta_b2_    = (TH1D*) o["hLeptonAsymAmpVsEta_b2_"];
   AsymCalculator::CombineAsimAsym(*hLeptonAsymAmpVsEta_b2_BLU, *hLeptonAsymAmpVsEta_b2_YEL, *hLeptonAsymAmpVsEta_b2_, true);

   // The boson asymmetry is next
   TH1D* hWBosonAsymAmpVsPt_BLU = (TH1D*) o["hWBosonAsymAmpVsPt_BLU"];
   TH1D* hWBosonAsymAmpVsPt_YEL = (TH1D*) o["hWBosonAsymAmpVsPt_YEL"];
   TH1D* hWBosonAsymAmpVsPt_    = (TH1D*) o["hWBosonAsymAmpVsPt_"];
   AsymCalculator::CombineAsimAsym(*hWBosonAsymAmpVsPt_BLU, *hWBosonAsymAmpVsPt_YEL, *hWBosonAsymAmpVsPt_);

   TH1D* hWBosonAsymAmpVsEta_BLU     = (TH1D*) o["hWBosonAsymAmpVsEta_BLU"];
   TH1D* hWBosonAsymAmpVsEta_YEL     = (TH1D*) o["hWBosonAsymAmpVsEta_YEL"];
   TH1D* hWBosonAsymAmpVsEta_        = (TH1D*) o["hWBosonAsymAmpVsEta_"];
   AsymCalculator::CombineAsimAsym(*hWBosonAsymAmpVsEta_BLU, *hWBosonAsymAmpVsEta_YEL, *hWBosonAsymAmpVsEta_, true);

   // This is mainly to cross check that the asymmetry sign flip works
   TH1D* hWBosonAsymAmpVsEta_YEL_rev = new TH1D(*hWBosonAsymAmpVsEta_YEL);
   o["hWBosonAsymAmpVsEta_YEL_rev"] = hWBosonAsymAmpVsEta_YEL_rev;
   hWBosonAsymAmpVsEta_YEL_rev->SetName("hWBosonAsymAmpVsEta_YEL_rev");
   utils::CopyReversedBinContentError(hWBosonAsymAmpVsEta_YEL, hWBosonAsymAmpVsEta_YEL_rev);
}
