#ifndef VecBosAsymRootFile_h
#define VecBosAsymRootFile_h

#include <map>
#include <set>
#include <string>

#include "VecBosRootFile.h"

#include "utils/PlotHelper.h"


/**
 * With this specialization of the ROOT's TFile we control the structure of the output file. In this
 * class we choose what should be saved (or not) and under what conditions. The decision is made on
 * event-by-event basis by calling the Fill(ProtoEvent &ev) function.
 */
class VecBosAsymRootFile : public VecBosRootFile
{

public:

   VecBosAsymRootFile();
   VecBosAsymRootFile(const char* fname, Option_t* option = "", Bool_t isMc=kFALSE, Bool_t isZ=kFALSE, const char* ftitle = "", Int_t compress = 1);
   ~VecBosAsymRootFile();

   void Fill(ProtoEvent &ev);
   void Fill(ProtoEvent &ev, ECut cut);

private:

   void BookHists();
};

#endif
