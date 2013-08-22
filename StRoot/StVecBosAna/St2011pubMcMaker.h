#ifndef St2011pubMcMaker_h
#define St2011pubMcMaker_h

#include "TH2.h"
#include "TVector3.h"


#include "StMaker.h"


class StVecBosMaker;

/**
 *
 */
class St2011pubMcMaker : public StMaker
{
public:

   St2011pubMcMaker(const char *name = "2011pubMc");
   virtual       ~St2011pubMcMaker();
   virtual Int_t  Init();
   virtual Int_t  Make();
   void setHList(TObjArray *x) {HList = x;}

   void AttachWalgoMaker(StVecBosMaker *mk) { wMK = mk;}

   virtual Int_t InitRun  (int runumber) {return 0;}; // Overload empty StMaker::InitRun
   virtual Int_t FinishRun(int runumber) {return 0;}; // Overload empty StMaker::FinishRun

private:

   StVecBosMaker *wMK; // W-algo maker with all data

   // histograms
   TObjArray *HList;
   enum {mxHA = 128}; TH1 *hA[mxHA];

   void initHistos();
   void doWanalysis();
   void doWefficiency();
   bool doMCanalysis();

   TVector3 mWP;
   TVector3 mNeutrinoP;
   TVector3 mElectronP;
   TVector3 mVertex;


   ClassDef(St2011pubMcMaker, 0)
};

#endif
