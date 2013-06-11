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
#include "VecBosEvent.h"


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
   }

   DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();
   for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
   {
      string sDblSpinState = AsString(*iDSS);

      shName = "hLeptonEtaVsPhi_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonPtVsPhi_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton P_{T}; Lepton #phi;", 10, 0, 50, 8, -M_PI, M_PI);
      hist->SetOption("colz");
   }

   SingleSpinStateSetIter iSSS = gSingleSpinStateSet.begin();
   for ( ; iSSS!=gSingleSpinStateSet.end(); ++iSSS) 
   {
      string sSnglSpinState = AsString(*iSSS);

      shName = "hLeptonEtaVsPhi_" + sSnglSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
      hist->SetOption("colz");

      shName = "hLeptonPhi_EtaProj_" + sSnglSpinState;
      o[shName] = hist = new TH1I(shName.c_str(), "; Lepton #phi; Events;", 8, -M_PI, M_PI);
      hist->SetOption("E1 GRIDX GRIDY");

      shName = "hLeptonPtVsPhi_" + sSnglSpinState;
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
   VecBosEvent& event = (VecBosEvent&) ev;

   if ( gDoubleSpinStateSet.find((EDoubleSpinState) event.mSpinPattern4Bits) == gDoubleSpinStateSet.end())
      return;

   EDoubleSpinState dblSpinState = (EDoubleSpinState) event.mSpinPattern4Bits;

   string sDblSpinState = AsString( dblSpinState );

   if (event.mTracksCandidate.size() > 0)
   {
      TVector3 eleCandidate = event.GetElectronCandidate();

      string shName = "hLeptonEtaVsPhi_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(eleCandidate.Eta(), eleCandidate.Phi());

      shName = "hLeptonPtVsPhi_" + sDblSpinState;
      ((TH2*) o[shName])->Fill(eleCandidate.Pt(), eleCandidate.Phi());
   }
}


/** */
void AsymHContainer::FillDerived()
{
   Info("FillDerived()", "Called");

   string shName;
   //TH1*   hist;

   SingleSpinStateSetIter iSSS = gSingleSpinStateSet.begin();
   for ( ; iSSS!=gSingleSpinStateSet.end(); ++iSSS) 
   {
      ESingleSpinState sss  = *iSSS;
      string sSnglSpinState = AsString(sss);

      TH2* hLeptonEtaVsPhi_sngl    = (TH2*) o["hLeptonEtaVsPhi_"    + sSnglSpinState];
      TH2* hLeptonPtVsPhi_sngl     = (TH2*) o["hLeptonPtVsPhi_"     + sSnglSpinState];
      TH1* hLeptonPhi_EtaProj_sngl = (TH1*) o["hLeptonPhi_EtaProj_" + sSnglSpinState];
      TH1* hLeptonPhi_PtProj_sngl  = (TH1*) o["hLeptonPhi_PtProj_"  + sSnglSpinState];

      DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();
      for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
      {
         EDoubleSpinState dss = *iDSS;
         string sDblSpinState = AsString(dss);

         TH2* hLeptonEtaVsPhi_dbl = (TH2*) o["hLeptonEtaVsPhi_" + sDblSpinState];
         TH2* hLeptonPtVsPhi_dbl  = (TH2*) o["hLeptonPtVsPhi_" + sDblSpinState];

         if (dss & sss) { 
            hLeptonEtaVsPhi_sngl->Add(hLeptonEtaVsPhi_dbl);
            hLeptonPtVsPhi_sngl->Add(hLeptonPtVsPhi_dbl);
         }
      }

      // Create projections on phi axis

      TH1D* hProjTmp = hLeptonEtaVsPhi_sngl->ProjectionY();
      utils::CopyBinContentError(hProjTmp, hLeptonPhi_EtaProj_sngl);

      hProjTmp = hLeptonPtVsPhi_sngl->ProjectionY();
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

   AsymCalculator::CalcAsimAsym(*hLeptonPhi_BU, *hLeptonPhi_BD, hLeptonAsymVsPhi_BLU);

   TF1 fitFunc("fitFunc", "[0] + [1]*sin(x + [2])", -M_PI, M_PI);

   hLeptonAsymVsPhi_BLU->Fit(&fitFunc);

   TH1I* hLeptonPhi_YU        = (TH1I*) o["hLeptonPhi_EtaProj_0u"];
   TH1I* hLeptonPhi_YD        = (TH1I*) o["hLeptonPhi_EtaProj_0d"];
   TH1D* hLeptonAsymVsPhi_YEL = (TH1D*) o["hLeptonAsymVsPhi_YEL"];

   AsymCalculator::CalcAsimAsym(*hLeptonPhi_YU, *hLeptonPhi_YD, hLeptonAsymVsPhi_YEL);

   hLeptonAsymVsPhi_YEL->Fit(&fitFunc);
}
