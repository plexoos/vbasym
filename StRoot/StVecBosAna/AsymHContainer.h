/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef AsymHContainer_h
#define AsymHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"


/**
 *
 */
class AsymHContainer : public PlotHelper
{
public:

   AsymHContainer();
   AsymHContainer(TDirectory *dir);

   using PlotHelper::FillDerived;
   using PlotHelper::PostFill;

   void Fill(ProtoEvent &ev);
   void FillDerived();
   void PostFill();

private:

   void BookHists();


   ClassDef(AsymHContainer, 1)
};

#endif
