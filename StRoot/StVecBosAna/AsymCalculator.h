
/**
 * Header file for AsymCalc
 * Author   : Itaru Nakagawa
 *          : Dmitri Smirnov
 *
 * Creation : 02/25/2006         
 */

#ifndef AsymCalculator_h
#define AsymCalculator_h

#include <string>

#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"

#include "utils/ValErrPair.h"



class AsymCalculator
{
public:

/*
   TGraphErrors* CalcBunchAsymDet(TH2 &hDetVsBunchId_ss, TH2 &hDetVsBunchId, DetLRSet detSet, TGraphErrors *gr=0);
   ValErrMap     CalcDetAsymSqrtFormula    (TH1 &hUp, TH1 &hDown, DetLRSet detSet);
   void          CalcBunchAsymSqrtFormula(DrawObjContainer *oc);
   void          CalcDelimAsymSqrtFormula(DrawObjContainer *oc);
   void          CalcStripAsymmetryByProfile(DrawObjContainer *oc);
   void          CalcKinEnergyAChAsym(DrawObjContainer *oc);
*/
   static void   CalcAsimAsym(TH1I &hUp, TH1I &hDown, TH1D &hAsym);
   static void   CalcAsimAsym(TH2I &h2DetCounts_up, TH2I &h2DetCounts_down, TH2D &hAsym);
   static void   FitAsimAsym(TH1D &hAsym);
   static void   FitAsimAsym(TH2D &hAsym, TH1D &hAsymAmplitude);
   static void   CombineAsimAsym(TH1D &hAsymBlu, TH1D &hAsymYel, TH1D &hAsymComb);
   //ValErrMap     CalcDetAsymSqrtFormula    (TH1 &hUp, TH1 &hDown, DetLRSet detSet);

   static ValErrPair CalcAsym(Double_t A, Double_t B, Double_t totalA, Double_t totalB);
   static ValErrPair CalcAsymSqrtFormula(Double_t A, Double_t B, Double_t C, Double_t D);
   //static bool       ExcludeAsimBins();

   ClassDef(AsymCalculator, 1)
};

#endif
