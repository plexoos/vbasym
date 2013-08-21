#ifndef AsymCalculator_h
#define AsymCalculator_h

#include <string>

#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"

#include "utils/ValErrPair.h"


/**
 * Currently just a set of static methods to calculate various asymmetries from histogrmas provided by the user.
 */
class AsymCalculator
{
public:

   static void   CalcAsimAsym(TH1I &hUp, TH1I &hDown, TH1D &hAsym);
   static void   CalcAsimAsym(TH2I &h2DetCounts_up, TH2I &h2DetCounts_down, TH2D &hAsym);
   static void   FitAsimAsym(TH1D &hAsym);
   static void   FitAsimAsym(TH2D &hAsym, TH1D &hAsymAmplitude);
   static void   CombineAsimAsym(TH1D &hAsymBlu, TH1D &hAsymYel, TH1D &hAsymComb);

   static ValErrPair CalcAsym(Double_t A, Double_t B, Double_t totalA=1, Double_t totalB=1);
   static ValErrPair CalcAsymSqrtFormula(Double_t A, Double_t B, Double_t C, Double_t D);

   ClassDef(AsymCalculator, 1)
};

#endif
