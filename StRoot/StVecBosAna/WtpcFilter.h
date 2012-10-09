#ifndef W_TPC_FILTER_HH
#define W_TPC_FILTER_HH

#include <TString.h>

class TObjArray;
class TH1;
class StMuTrack;


// Sector dependent TPC track filter
class WtpcFilter
{
public:
   WtpcFilter();
   void init(const char* core, int sec, TObjArray* HListX, bool barrel);
   void setCuts(int x, float y, float r1, float r2) {
      par_nFitPts  = x;
      par_nHitFrac = y;
      par_Rmin     = r1;
      par_Rmax     = r2;
   }

   static int getTpcSec(float phiRad, float etaDet);
   bool accept( const StMuTrack*  prMuTrack);

private:
   TString name;
   int secID;
   int par_nFitPts;
   float par_nHitFrac, par_Rmin, par_Rmax;

   // histograms
   TObjArray* HList;
   enum {mxHA = 8};
   TH1* hA[mxHA];
   void initHistos(bool barrel);

};

#endif
