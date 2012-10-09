#ifndef STAR_St2011pubWanaMaker
#define STAR_St2011pubWanaMaker

/*!
 * \class  St2011pubWanaMaker
 * \brief  gathers all results from  W-analysis
 */

class StVecBosMaker;
class StEmcDecoder;

class St2011pubWanaMaker : public StMaker
{
private:
   // parameters
   float  par_highET; // cut of for W 2x2 cluster ET

   StVecBosMaker *wMK; // W-algo maker with all data
   StEmcDecoder  *mMappB;

   // histograms
   TObjArray *HList;
   enum {mxHA = 50};
   TH1  *hA[mxHA];

   void  initHistos();
   void  evalWeleTrackSign();
   void  scanCrateRate();
   void  varyCuts4backgStudy();

public:
   St2011pubWanaMaker(const char *name = "2011pubWana");
   virtual       ~St2011pubWanaMaker() {};
   virtual Int_t  Init();
   virtual Int_t  Make();
   void setHList(TObjArray *x) {HList = x;}

   void attachWalgoMaker(StVecBosMaker *mk) { wMK = mk;}

   virtual Int_t InitRun  (int runumber);
   virtual Int_t FinishRun(int runumber) {return 0;}; // Overload empty StMaker::FinishRun


   /// Displayed on session exit, leave it as-is please ...
   virtual const char *GetCVS() const {
      static const char cvs[] = "Tag $Name:  $ $Id: St2011pubWanaMaker.h,v 1.1 2012/10/09 15:21:20 smirnovd Exp $ built "__DATE__" "__TIME__ ;
      return cvs;
   }

   ClassDef(St2011pubWanaMaker, 0)  //StAF chain virtual base class for Makers
};

#endif
