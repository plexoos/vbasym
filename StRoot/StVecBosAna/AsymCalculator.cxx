#include "AsymCalculator.h"

#include <math.h>

#include "TF1.h"
#include "TF2.h"
#include "TMath.h"

#include "utils/utils.h"


ClassImp(AsymCalculator)

using namespace std;


/**
 * Takes two histograms binned in asimuthal angle phi for spin up and down.
 * Returns a hist hChAsym filled with asym values for each valid bin/channel.
 */
void AsymCalculator::CalcAsimAsym(TH1I &hUp, TH1I &hDown, TH1D &hAsym)
{
   //if (!hAsym) {
   //   hAsym= new TH1D("hAsym", "hAsym", hUp.GetNbinsX(), hUp.GetXaxis()->GetXmin(), hUp.GetXaxis()->GetXmax());
   //}

   for (int iAsimBin=1; iAsimBin<=hUp.GetNbinsX(); iAsimBin++)
   {
      UInt_t totalCountsUp   = 1;
      UInt_t totalCountsDown = 1;

      Double_t phi1   = hUp.GetBinCenter(iAsimBin);

      // Loop to get total counts (luminosity)
      //for (int iAsimBin2=1; iAsimBin2<=hUp.GetNbinsX(); iAsimBin2++)
      //{
      //   double phi2  = hUp.GetBinCenter(iAsimBin2);
      //   double dPhi  = fabs(phi1 - phi2);
      //   double phase = fmod(dPhi, M_PI);

      //   //printf("iAsimBin, iAsimBin2, phi1, phi2, phase: %d, %d, %f, %f, %f\n", iAsimBin, iAsimBin2, phi1, phi2, phase);

      //   // Calculate luminosity. This strip and ones in cross geometry are excluded.
      //   //if (!AsymCalculator::ExcludeStrip(iAsimBin-1, iAsimBin2-1))
      //   if ( phase != 0 )
      //   {
      //      totalCountsUp   += hUp.GetBinContent(iAsimBin2);
      //      totalCountsDown += hDown.GetBinContent(iAsimBin2);
      //   }
      //}

      UInt_t nCountsUp   = hUp.GetBinContent(iAsimBin);
      UInt_t nCountsDown = hDown.GetBinContent(iAsimBin);

      //printf("nCountsUp, nCountsDown, totalCountsUp, totalCountsDown: %d, %d, %d, %d\n", nCountsUp, nCountsDown, totalCountsUp, totalCountsDown);

      // Calculate raw asymmetries for the i-th bin
      if ( totalCountsUp && totalCountsDown && nCountsUp + nCountsDown )
      {
         // Calculate Asym and dAsym
         ValErrPair chAsym = CalcAsym(nCountsUp, nCountsDown, totalCountsUp, totalCountsDown);

         // Assign large error, i.e. 100% when there is no Up or Down counts
         if (nCountsUp <= 0 || nCountsDown <= 0)
            chAsym.second = 1;

         hAsym.SetBinContent(iAsimBin, chAsym.first);
         hAsym.SetBinError(iAsimBin,   chAsym.second);
      }
   }
}


/** */
void AsymCalculator::CalcAsimAsym(TH2I &hUp, TH2I &hDown, TH2D &hAsym)
{
   for (int iBinX=1; iBinX<=hAsym.GetNbinsX(); iBinX++)
   {
      TH1I *hUpSlice = (TH1I*) hUp.ProjectionY("hUpSlice", iBinX, iBinX);
      TH1I *hDwSlice = (TH1I*) hDown.ProjectionY("hDwSlice", iBinX, iBinX);

      TH1D hAsymSlice("hAsymSlice", "hAsymSlice", hUpSlice->GetNbinsX(), hUpSlice->GetXaxis()->GetXmin(), hUpSlice->GetXaxis()->GetXmax());

      // Check if there are events in the histograms
      if (!hUpSlice->Integral() && !hDwSlice->Integral()) continue;

      CalcAsimAsym(*hUpSlice, *hDwSlice, hAsymSlice);

      for (int iBinY=1; iBinY<=hAsym.GetNbinsY(); iBinY++)
      {
         Double_t asymVal = hAsymSlice.GetBinContent(iBinY);
         Double_t asymErr = hAsymSlice.GetBinError(iBinY);

         hAsym.SetBinContent(iBinX, iBinY, asymVal);
         hAsym.SetBinError(iBinX, iBinY, asymErr);
      }
   }
}


/** */
void AsymCalculator::FitAsimAsym(TH1D &hAsym)
{
   // A way to check the histogram is empty
   double integral = hAsym.Integral();

   if (integral == 0) return;

   TF1 fitFunc("fitFunc", "[0] + [1]*sin(x + [2])", -M_PI, M_PI);
   fitFunc.SetParNames("Offset", "Amplitude", "Phase");
   hAsym.Fit(&fitFunc);
}


/** */
void AsymCalculator::FitAsimAsym(TH2D &hAsym, TH1D &hAsymAmplitude)
{
   for (int iBinX=1; iBinX<=hAsym.GetNbinsX(); iBinX++)
   {
      TH1D *hAsymSlice = (TH1D*) hAsym.ProjectionY("hAsymSlice", iBinX, iBinX);

      FitAsimAsym(*hAsymSlice);

      TF1* fitFunc = (TF1*) hAsymSlice->GetListOfFunctions()->FindObject("fitFunc");

      if (fitFunc)
      {
         Double_t val = fitFunc->GetParameter(1);
         Double_t err = fitFunc->GetParError(1);

         hAsymAmplitude.SetBinContent(iBinX, val);
         hAsymAmplitude.SetBinError(iBinX, err);
      }
   }
}


/**
 * Takes two one-dimensional histograms with asymmetries for the two beams and properly combines them. All three
 * histograms must have the same structure, i.e. the number of bins and the axis ranges are assumed to be the same.
 */
void AsymCalculator::CombineAsimAsym(TH1D &hAsymBlu, TH1D &hAsymYel, TH1D &hAsymComb)
{
   // Flip the sign of the yellow beam asymmetry. Include underflow and overflow bins but don't change the bin errors
   for (int iBin=0; iBin<=hAsymYel.GetNbinsX()+1; iBin++)
   {
      hAsymYel.SetBinContent( iBin, -1*hAsymYel.GetBinContent(iBin) );
   }

   utils::AverageIgnoreEmptyBins(&hAsymBlu, &hAsymYel, &hAsymComb);
}


/**
 * Calculates the asymmetry for two values A and B using the simple formula 
 * asym = (A - R * B) / (A * + R * B), where R = totalA/totalB. The correction R can be applied to
 * correct for an a-priori known differences in A and B.
 */
ValErrPair AsymCalculator::CalcAsym(Double_t A, Double_t B, Double_t totalA, Double_t totalB)
{
   Double_t R = 0;

   if (totalB) R = totalA/totalB;

   Double_t denom1  = A+R*B;
   Double_t denom2  = A+B;
   Double_t asym    = 0;
   Double_t asymErr = 0;

   if ( denom1 && denom2 ) {
      asym    = (A-R*B)/denom1;
      asymErr = sqrt(4*B*B*A + 4*A*A*B)/denom2/denom2;
   }

   ValErrPair result(asym, asymErr);

   return result;
}


/** 
 * Calculates the asymmetry using the so called square root formula.
 */
ValErrPair AsymCalculator::CalcAsymSqrtFormula(Double_t A, Double_t B, Double_t C, Double_t D)
{
   Double_t asym, asymErr;
   Double_t denom = sqrt(A*B) + sqrt(C*D);

   if ( denom ) {
      asym    = (sqrt(A*B) - sqrt(C*D))/denom;
      asymErr =  sqrt(A*B*(C+D) + C*D*(A+B))/denom/denom;
   } else {
      asym    =  0;
      asymErr = -1; // set to -1 to indicate invalid result
   }

   ValErrPair result(asym, asymErr);

   return result;
}
