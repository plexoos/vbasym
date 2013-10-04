#ifndef VecBosAsymRootFile_h
#define VecBosAsymRootFile_h

#include <map>
#include <set>
#include <string>

#include "VecBosRootFile.h"

#include "utils/PlotHelper.h"



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


   ClassDef(VecBosAsymRootFile, 1)
};

#endif
