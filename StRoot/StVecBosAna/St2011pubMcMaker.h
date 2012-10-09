// $Id: St2011pubMcMaker.h,v 1.1 2012/10/09 15:21:20 smirnovd Exp $
//
//*-- Author :  Justin Stevens, IUCF


#ifndef STAR_St2011pubMcMaker
#define STAR_St2011pubMcMaker

/*!
 *
 * \class  St2011pubMcMaker
 * \author Justin Stevens, IUCF
 * \date   September 2009
 * \brief  maker to retrieve info from geant.root files for
 *         comparison with reco quantities from MC
 *
 * This commented block at the top of the header file is considered as
 * the class description to be present on the this class Web page.
 *
 *
 */

#ifndef StMaker_H
#include "StMaker.h"
#endif
#include <TH2.h>
#include <TVector3.h>
class StVecBosMaker;

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



   /// Displayed on session exit, leave it as-is please ...
   virtual const char *GetCVS() const {
      static const char cvs[] = "Tag $Name:  $ $Id: St2011pubMcMaker.h,v 1.1 2012/10/09 15:21:20 smirnovd Exp $ built "__DATE__" "__TIME__ ;
      return cvs;
   }

   ClassDef(St2011pubMcMaker, 0)  //StAF chain virtual base class for Makers
};

#endif


// $Log: St2011pubMcMaker.h,v $
// Revision 1.1  2012/10/09 15:21:20  smirnovd
// *** empty log message ***
//
// Revision 1.1  2011/02/10 20:33:25  balewski
// start
//
