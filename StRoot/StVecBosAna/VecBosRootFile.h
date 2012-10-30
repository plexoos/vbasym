#ifndef VecBosRootFile_h
#define VecBosRootFile_h

#include <map>
#include <set>
#include <string>

#include "TFile.h"

#include "utils/PlotHelper.h"


enum ECut {kCUT_UNKNOWN, kCUT_NOCUT,
           kCUT_VERTICES, kCUT_VERTICES_GOOD,
           kCUT_TRACKS, kCUT_TRACKS_GOOD,
           kCUT_BARREL, kCUT_ENDCAP, kCUT_CUT,
           kCUT_ELERACK};

typedef std::map<ECut, std::set<PlotHelper*> >   Cut2PlotHelperMap;
typedef Cut2PlotHelperMap::iterator              Cut2PlotHelperMapIter;


class VecBosRootFile : public TFile
{
protected:

   PlotHelper        *fHists;
   Cut2PlotHelperMap  fHistCuts;
   Float_t            fMinFill;
   Float_t            fMaxFill;
   time_t             fMinTime;
   time_t             fMaxTime;
   //FILE              *fFilePhp;      //!

public:

   VecBosRootFile();
   VecBosRootFile(const char* fname, Option_t* option = "", const char* ftitle = "", Int_t compress = 1);
   ~VecBosRootFile();

   using TObject::SaveAs;

   PlotHelper* GetHists();
   void SetHists(PlotHelper &hists);
   void Fill(ProtoEvent &ev);
   void Fill(ProtoEvent &ev, ECut cut);
   //void SetAnaGlobResult(AnaGlobResult *agr);
   void SaveAs(std::string pattern="^.*$", std::string dir=".");
   //void UpdMinMax(EventConfig &mm);
	void UpdMinMaxFill(UInt_t fillId);
	void UpdMinMaxTime(time_t time);
   void Print(const Option_t* opt="") const;
   void PrintAsPhp(FILE *f=stdout) const;
   void Close(Option_t* option = "");

private:

   void BookHists();


   ClassDef(VecBosRootFile, 1)
};
  
#endif
