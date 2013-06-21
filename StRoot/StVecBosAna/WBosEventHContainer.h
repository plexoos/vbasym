/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef WBosEventHContainer_h
#define WBosEventHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"


/**
 *
 */
class WBosEventHContainer : public PlotHelper
{
private:

   //TH1* fhPseudoMass_ch[N_SILICON_CHANNELS];


public:

   WBosEventHContainer();
   WBosEventHContainer(TDirectory *dir);
   ~WBosEventHContainer();

   using PlotHelper::FillDerived;
   using PlotHelper::PostFill;

   void Fill(ProtoEvent &ev);
   void FillDerived();
   void PostFill();

private:

   void BookHists();


   ClassDef(WBosEventHContainer, 1)
};

#endif
