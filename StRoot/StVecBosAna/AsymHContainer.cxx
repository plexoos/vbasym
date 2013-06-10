/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <math.h>

#include "AsymHContainer.h"

#include "TF1.h"
#include "TF2.h"

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

   DoubleSpinStateSetIter iDSS = gDoubleSpinStateSet.begin();

   for ( ; iDSS!=gDoubleSpinStateSet.end(); ++iDSS) 
   {
      string sDblSpinState = AsString(*iDSS);

      shName = "hLeptonEtaVsPhi_" + sDblSpinState;
      o[shName] = hist = new TH2I(shName.c_str(), "; Lepton #eta; Lepton #phi;", 3, -1.5, 1.5, 8, -M_PI, M_PI);
      hist->SetOption("colz");
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
   }
}


/** */
void AsymHContainer::FillDerived()
{
   Info("FillDerived()", "Called");

}


/** */
void AsymHContainer::PostFill()
{
   Info("PostFill", "Called");

}
