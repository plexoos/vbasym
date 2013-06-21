/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef EventHContainer_h
#define EventHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"


/**
 *
 */
class EventHContainer : public PlotHelper
{
private:

   //TH1* fhPseudoMass_ch[N_SILICON_CHANNELS];


public:

   EventHContainer();
   EventHContainer(TDirectory *dir);
   ~EventHContainer();

   void Fill(ProtoEvent &ev);
   void FillTracks(ProtoEvent &ev);
   //void FillDerived();
   //void FillDerived(PlotHelper &oc);
   //void PostFill();
   //void PostFill(PlotHelper &oc);

private:

   void BookHists();


   ClassDef(EventHContainer, 1)
};

#endif
