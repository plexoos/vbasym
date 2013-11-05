#ifndef Z0AsymHContainer_h
#define Z0AsymHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"

#include "Globals.h"


/**
 * A container to hold histograms with calculated asymmetries of Z0 boson and its decay products.
 */
class Z0AsymHContainer : public PlotHelper
{
public:

   Z0AsymHContainer();
   Z0AsymHContainer(TDirectory *dir, EAsymType asymType=kAsymPlain);

   using PlotHelper::FillDerived;
   using PlotHelper::PostFill;

   void Fill(ProtoEvent &ev);
   void FillDerived();
   void PostFill();

protected:

   EAsymType mAsymType; ///< Type of the asymmetry calculated in this containter

private:

   void BookHists();


   ClassDef(Z0AsymHContainer, 1)
};

#endif
