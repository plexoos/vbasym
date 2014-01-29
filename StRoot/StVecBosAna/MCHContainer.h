/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef MCHContainer_h
#define MCHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"

#include "VecBosTrack.h"


/**
 *
 */
class MCHContainer : public PlotHelper
{
private:

   //TH1* fhPseudoMass_ch[N_SILICON_CHANNELS];


public:

   MCHContainer();
   MCHContainer(TDirectory *dir, Int_t);

   void Fill(ProtoEvent &ev);
   void PostFill();
   void PostFill(PlotHelper &oc) {}

private:

   void BookHists();


   ClassDef(MCHContainer, 1)
};

#endif
