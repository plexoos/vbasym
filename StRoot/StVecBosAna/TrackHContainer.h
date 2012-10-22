/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef TrackHContainer_h
#define TrackHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"


/**
 *
 */
class TrackHContainer : public PlotHelper
{
private:

   //TH1* fhPseudoMass_ch[N_SILICON_CHANNELS];


public:

   TrackHContainer();
   TrackHContainer(TDirectory *dir);
   ~TrackHContainer();

   using PlotHelper::FillDerived;
   using PlotHelper::PostFill;

   void Fill(ProtoEvent &ev);
   void FillDerived();
   void PostFill();

private:

   void BookHists();


   ClassDef(TrackHContainer, 1)
};

#endif
