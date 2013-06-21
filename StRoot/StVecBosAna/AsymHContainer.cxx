/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <math.h>

#include "AsymHContainer.h"

#include "TF1.h"
#include "TF2.h"

#include "utils/utils.h"

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

   BeamIdSetIter iBeam = gBeams.begin();
   for ( ; iBeam!=gBeams.end(); ++iBeam) 
   {
      string sBeam = AsString(*iBeam);

      shName = "hLeptonAsymVsPhi_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Lepton #phi; Asym;", 8, -M_PI, M_PI);
      hist->SetOption("E1 GRIDX GRIDY");

      shName = "hLeptonAsymVsPhiVsEta_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonAsymAmpVsEta_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Lepton #eta; Asym Amp.;", 3, -1.5, 1.5);
      hist->SetOption("E1 GRIDX GRIDY");

      shName = "hLeptonAsymVsPhiVsPt_" + sBeam;
      o[shName] = hist = new TH2D(shName.c_str(), "; Lepton P_{T}; Lepton #phi;", 10, 0, 50, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonAsymAmpVsPt_" + sBeam;
      o[shName] = hist = new TH1D(shName.c_str(), "; Lepton P_{T}; Asym Amp.;", 10, 0, 50);
      hist->SetOption("E1 GRIDX GRIDY");
   }

   DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();
   for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
   {
      string sDblSpinState = AsString(*iDSS);

      shName = "hLeptonPhiVsEta_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonPhiVsPt_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton P_{T}; Lepton #phi;", 10, 0, 50, 8, -M_PI, M_PI);
      hist->SetOption("colz");
   }

   SingleSpinStateSetIter iSSS = gSingleSpinStateSet.begin();
   for ( ; iSSS!=gSingleSpinStateSet.end(); ++iSSS) 
   {
      string sSnglSpinState = AsString(*iSSS);

      shName = "hLeptonPhiVsEta_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
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
   }

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
      TVector3 eleCandidate = event.GetElectronCandidateP3();

      string shName = "hLeptonPhiVsEta_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(eleCandidate.Eta(), eleCandidate.Phi());

      shName = "hLeptonPhiVsPt_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(eleCandidate.Pt(), eleCandidate.Phi());
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
      TH2* hLeptonPhiVsPt_sngl     = (TH2*) o["hLeptonPhiVsPt_"     + sSnglSpinState];
      TH1* hLeptonPhi_EtaProj_sngl = (TH1*) o["hLeptonPhi_EtaProj_" + sSnglSpinState];
      TH1* hLeptonPhi_PtProj_sngl  = (TH1*) o["hLeptonPhi_PtProj_"  + sSnglSpinState];

      DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();
      for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
      {
         EDoubleSpinState dss = *iDSS;
         string sDblSpinState = AsString(dss);

         TH2* hLeptonPhiVsEta_dbl = (TH2*) o["hLeptonPhiVsEta_" + sDblSpinState];
         TH2* hLeptonPhiVsPt_dbl  = (TH2*) o["hLeptonPhiVsPt_" + sDblSpinState];

         if (dss & sss) {
            hLeptonPhiVsEta_sngl->Add(hLeptonPhiVsEta_dbl);
            hLeptonPhiVsPt_sngl->Add(hLeptonPhiVsPt_dbl);
         }
      }

      // Create projections on phi axis
      TH1D* hProjTmp = hLeptonPhiVsEta_sngl->ProjectionY();
      utils::CopyBinContentError(hProjTmp, hLeptonPhi_EtaProj_sngl);

      hProjTmp = hLeptonPhiVsPt_sngl->ProjectionY();
      utils::CopyBinContentError(hProjTmp, hLeptonPhi_PtProj_sngl);
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

      TH2I* hLeptonPhiVsEta_up     = (TH2I*) o["hLeptonPhiVsEta_" + sSpinUp];
      TH2I* hLeptonPhiVsEta_dn     = (TH2I*) o["hLeptonPhiVsEta_" + sSpinDn];
      TH2D* hLeptonAsymVsPhiVsEta_ = (TH2D*) o["hLeptonAsymVsPhiVsEta_" + sBeam];
      TH1D* hLeptonAsymAmpVsEta_   = (TH1D*) o["hLeptonAsymAmpVsEta_" + sBeam];

      AsymCalculator::CalcAsimAsym(*hLeptonPhiVsEta_up, *hLeptonPhiVsEta_dn, *hLeptonAsymVsPhiVsEta_);
      AsymCalculator::FitAsimAsym(*hLeptonAsymVsPhiVsEta_, *hLeptonAsymAmpVsEta_);

      TH2I* hLeptonPhiVsPt_up     = (TH2I*) o["hLeptonPhiVsPt_" + sSpinUp];
      TH2I* hLeptonPhiVsPt_dn     = (TH2I*) o["hLeptonPhiVsPt_" + sSpinDn];
      TH2D* hLeptonAsymVsPhiVsPt_ = (TH2D*) o["hLeptonAsymVsPhiVsPt_" + sBeam];
      TH1D* hLeptonAsymAmpVsPt_   = (TH1D*) o["hLeptonAsymAmpVsPt_" + sBeam];

      AsymCalculator::CalcAsimAsym(*hLeptonPhiVsPt_up, *hLeptonPhiVsPt_dn, *hLeptonAsymVsPhiVsPt_);
      AsymCalculator::FitAsimAsym(*hLeptonAsymVsPhiVsPt_, *hLeptonAsymAmpVsPt_);
   }
}
