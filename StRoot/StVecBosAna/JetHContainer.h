/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef JetHContainer_h
#define JetHContainer_h

#include "TDirectoryFile.h"

#include "utils/PlotHelper.h"
#include "utils/ProtoEvent.h"


/**
 *
 */
class JetHContainer : public PlotHelper
{
private:

   //TH1* fhPseudoMass_ch[N_SILICON_CHANNELS];

public:

   JetHContainer();
   JetHContainer(TDirectory *dir);
   ~JetHContainer();

   using PlotHelper::FillDerived;
   using PlotHelper::PostFill;

   void Fill(ProtoEvent &ev);
   //void Fill(VecBosTrack &track);
   void FillDerived();
   void PostFill();

private:

   void BookHists();


   ClassDef(JetHContainer, 1)
};

#endif
