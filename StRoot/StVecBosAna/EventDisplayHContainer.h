/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef EventDisplayHContainer_h
#define EventDisplayHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"


/**
 *
 */
class EventDisplayHContainer : public PlotHelper
{
private:

   //TH1* fhPseudoMass_ch[N_SILICON_CHANNELS];

public:

   EventDisplayHContainer();
   EventDisplayHContainer(TDirectory *dir);
   ~EventDisplayHContainer();

   void Fill(ProtoEvent &ev);
   //void FillDerived();
   //void FillDerived(PlotHelper &oc);
   //void PostFill();
   //void PostFill(PlotHelper &oc);

private:

	uint32_t fEventCounter; //< Counter for requested event displays

   void BookHists();
   PlotHelper *GetEventDisplayHists(const ProtoEvent &ev);


   ClassDef(EventDisplayHContainer, 1)
};

#endif
