#ifndef STAR_StZBosMaker
#define STAR_StZBosMaker

/*!
 * \brief  uses tree from W-algo to find Zs
 *
 */

#include "StMaker.h"
#include "StVecBosMaker.h"


class StMuDstMaker;
class WeventDisplay;
class VecBosTrack;


class StZBosMaker : public StMaker
{

private:

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
   void FindZBosBarrel();
   void FindZBosEndcap();

public:

   StZBosMaker(const char *name = "2011Zalgo");
   virtual       ~StZBosMaker() {};
   virtual Int_t  Init();
   virtual Int_t  Make();
   virtual Int_t InitRun(int runnumber);   // Overload empty StMaker::InitRun
   virtual Int_t FinishRun(int runnumber); // Overload empty StMaker::FinishRun
   virtual const char *GetCVS() const
   {
      static const char cvs[] = "";
      return cvs;
   }

   void setHList(TObjArray *x)              { HList = x; }
   void AttachWalgoMaker(StVecBosMaker *mk) { wMK = mk; }
   void attachMuMaker(StMuDstMaker *mk)     { muMK = mk; }
   void setNearEtFrac(float x)              { par_nearTotEtFracZ = x; }
   void setClusterMinEt(float x)            { par_clusterEtZ = x; }
   void setPhi12Min(float x)                { par_delPhi12 = x; }
   void setMinZMass(float x)                { par_minMassZ = x; }
   void setMaxZMass(float x)                { par_maxMassZ = x; }
   void printJan(VecBosTrack *T);

   ClassDef(StZBosMaker, 0)
};

#endif
