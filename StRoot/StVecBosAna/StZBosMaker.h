// $Id: StZBosMaker.h,v 1.2 2012/10/12 19:30:01 smirnovd Exp $

#ifndef STAR_StZBosMaker
#define STAR_StZBosMaker

/*!
 *
 * \class  StZBosMaker
 * \author Ross
 * \date   December 2011
 * \brief  uses tree from W-algo to find Zs
 *
 *
 *
 */

#include "StMaker.h"
#include "StVecBosMaker.h"

class StMuDstMaker;
class WeventDisplay;
class WeveEleTrack;


class StZBosMaker : public StMaker
{
private:
   // parameters
   float par_nearTotEtFracZ;
   float par_clusterEtZ;
   float par_delPhi12;
   float par_minMassZ;
   float par_maxMassZ;

   enum {mxHA = 100};

   StVecBosMaker *wMK;      // W-algo maker with all data
   StMuDstMaker  *muMK;
   TObjArray     *HList;    // histograms
   TH1           *hA[mxHA];

   void initHistos();
   void find_Z_boson();
   void findEndcap_Z_boson();

public:
   StZBosMaker(const char *name = "2011Zalgo");
   virtual       ~StZBosMaker() {};
   virtual Int_t  Init();
   virtual Int_t  Make();
   void setHList(TObjArray *x)             { HList = x;}
   void attachWalgoMaker(StVecBosMaker *mk) { wMK = mk;}
   void attachMuMaker(StMuDstMaker *mk)    { muMK = mk;}
   void setNearEtFrac(float x)             { par_nearTotEtFracZ = x; return;}
   void setClusterMinEt(float x)           { par_clusterEtZ = x; return;}
   void setPhi12Min(float x)               { par_delPhi12 = x; return;}
   void setMinZMass(float x)               { par_minMassZ = x; return;}
   void setMaxZMass(float x)               { par_maxMassZ = x; return;}
   void printJan(WeveEleTrack *T);
   virtual Int_t InitRun(int runnumber);   // Overload empty StMaker::InitRun
   virtual Int_t FinishRun(int runnumber); // Overload empty StMaker::FinishRun

   // Displayed on session exit, leave it as-is please ...
   virtual const char *GetCVS() const {
      static const char cvs[] = "";
      return cvs;
   }

   ClassDef(StZBosMaker, 0)
};

#endif
