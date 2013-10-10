#ifndef AsymHContainer_h
#define AsymHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"

#include "Globals.h"


/**
 * A container to hold histograms with calculated asymmetries of W boson and its decay products.
 */
class AsymHContainer : public PlotHelper
{
public:

   AsymHContainer();
   AsymHContainer(TDirectory *dir, EAsymType asymType=kAsymPlain);

   using PlotHelper::FillDerived;
   using PlotHelper::PostFill;

   void Fill(ProtoEvent &ev);
   void FillDerived();
   void PostFill();

protected:

   EAsymType mAsymType; ///< Type of the asymmetry calculated in this containter

private:

   void BookHists();


   ClassDef(AsymHContainer, 1)
};

#endif
