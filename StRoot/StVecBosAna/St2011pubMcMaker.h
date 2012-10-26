#ifndef St2011pubMcMaker_h
#define St2011pubMcMaker_h

#include "TH2.h"
#include "TVector3.h"


//#ifndef StMaker_H
#include "StMaker.h"
//#endif


class StVecBosMaker;

/**
 *
 */
class St2011pubMcMaker : public StMaker
{
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

public:

   St2011pubMcMaker(const char *name = "2011pubMc");
   virtual       ~St2011pubMcMaker();
   virtual Int_t  Init();
   virtual Int_t  Make();
   void setHList(TObjArray *x) {HList = x;}

   void attachWalgoMaker(StVecBosMaker *mk) { wMK = mk;}

   virtual Int_t InitRun  (int runumber) {return 0;}; // Overload empty StMaker::InitRun
   virtual Int_t FinishRun(int runumber) {return 0;}; // Overload empty StMaker::FinishRun

   // Displayed on session exit, leave it as-is please ...
   virtual const char *GetCVS() const {
      static const char cvs[] = "Tag $Name:  $ $Id: St2011pubMcMaker.h,v 1.2 2012/10/26 00:07:44 smirnovd Exp $ built "__DATE__" "__TIME__ ;
      return cvs;
   }

   ClassDef(St2011pubMcMaker, 0)
};

#endif
