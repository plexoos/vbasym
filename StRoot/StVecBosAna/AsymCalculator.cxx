#include "AsymCalculator.h"

#include <math.h>

#include "TF1.h"
#include "TF2.h"
#include "TMath.h"

#include "utils/utils.h"


ClassImp(AsymCalculator)

using namespace std;


/**
 * Calculates the left-right asymmetry for a given set of detectors detSet. All
 * the counts are given by the hDetVsBunchId_ss histogram, and the
 * normalization is calculated from all the counts in the hDetVsBunchId
 * histogram.
 */
/*
TGraphErrors* AsymCalculator::CalcBunchAsymDet(TH2 &hDetVsBunchId_ss, TH2 &hDetVsBunchId, DetLRSet detSet, TGraphErrors *gr)
{
   // Calculate detector luminosities by taking sum over R/L detectors and all
   // bunches
   int totalL = 1;
   int totalR = 1;

   DetLRSet::iterator iDetPair = detSet.begin();

   for ( ; iDetPair!=detSet.end(); ++iDetPair) {
      totalL += hDetVsBunchId.Integral(1, N_BUNCHES, iDetPair->first,  iDetPair->first);
      totalR += hDetVsBunchId.Integral(1, N_BUNCHES, iDetPair->second, iDetPair->second);
      //printf("totals: %d, %d\n", totalL, totalR);
   }

   if (!gr)
      gr = new TGraphErrors();

   // Main bunch loop to calculate asymmetry bunch by bunch
   for (Int_t iBunch=1; iBunch<=hDetVsBunchId_ss.GetNbinsX(); iBunch++) {

      UInt_t countsL = 0;
      UInt_t countsR = 0;

      for (iDetPair=detSet.begin() ; iDetPair!=detSet.end(); ++iDetPair)
      {
         countsL += hDetVsBunchId_ss.GetBinContent(iBunch, iDetPair->first);
         countsR += hDetVsBunchId_ss.GetBinContent(iBunch, iDetPair->second);
      }

      //ValErrPair bunchAsym = CalcAsym(countsR, countsL, totalR, totalL);
      ValErrPair bunchAsym = CalcAsym(countsL, countsR, totalL, totalR);

      // skip if nothing was calculated
      if (!bunchAsym.first && !bunchAsym.second) continue;

      Int_t nPoints = gr->GetN();
      gr->SetPoint(nPoints, iBunch, bunchAsym.first);
      gr->SetPointError(nPoints, 0, bunchAsym.second);
   }

   return gr;
}
*/


/**
 * Calculates three asymmetries using the square root formula from two
 * histogram containing counts per detector.
 */
/*
ValErrMap AsymCalculator::CalcDetAsymSqrtFormula(TH1 &hUp, TH1 &hDown, DetLRSet detSet)
{
   Double_t LU = 0, RU = 0, LD = 0, RD = 0;

   DetLRSet::iterator iDetPair = detSet.begin();

   for ( ; iDetPair!=detSet.end(); ++iDetPair) {
      LU += hUp.GetBinContent(iDetPair->first);
      RU += hUp.GetBinContent(iDetPair->second);
      LD += hDown.GetBinContent(iDetPair->first);
      RD += hDown.GetBinContent(iDetPair->second);
   }

   ValErrMap results;

   results["geom"] = CalcAsymSqrtFormula(RU, RD, LD, LU);
   results["lumi"] = CalcAsymSqrtFormula(RU, LU, RD, LD);
   results["phys"] = CalcAsymSqrtFormula(RU, LD, RD, LU);

   return results;
}
*/


/** */
/*
void AsymCalculator::CalcBunchAsymSqrtFormula(DrawObjContainer *oc)
{
   TH2* hDetVsBunchId_up   = (TH2*) oc->o["hDetVsBunchId_up"];
   TH2* hDetVsBunchId_down = (TH2*) oc->o["hDetVsBunchId_down"];

   // Combine all bunches by making a projection on the Y axis
   TH1D* hDetCounts_up   = hDetVsBunchId_up->ProjectionY();
   TH1D* hDetCounts_down = hDetVsBunchId_down->ProjectionY();

   gAnaMeasResult->fAsymX90  = CalcDetAsymX90SqrtFormula (*hDetCounts_up, *hDetCounts_down);
   gAnaMeasResult->fAsymX45  = CalcDetAsymX45SqrtFormula (*hDetCounts_up, *hDetCounts_down);
   //gAnaMeasResult->fAsymX45T = CalcDetAsymX45TSqrtFormula(*hDetCounts_up, *hDetCounts_down);
   gAnaMeasResult->fAsymX45T = CalcDetAsymSqrtFormula(*hDetCounts_up, *hDetCounts_down, X45TDets);
   gAnaMeasResult->fAsymX45B = CalcDetAsymX45BSqrtFormula(*hDetCounts_up, *hDetCounts_down);
   gAnaMeasResult->fAsymY45  = CalcDetAsymY45SqrtFormula (*hDetCounts_up, *hDetCounts_down);

}
*/


/** */
/*
void AsymCalculator::CalcDelimAsymSqrtFormula(DrawObjContainer *oc)
{
   TH2* hDetVsDelim_up    = (TH2*) oc->o["hDetVsDelim_up"];
   TH2* hDetVsDelim_down  = (TH2*) oc->o["hDetVsDelim_down"];
   TH2* hAsymVsDelim4Det  = (TH2*) oc->o["hAsymVsDelim4Det"];

   for (int iDelim=1; iDelim<=hAsymVsDelim4Det->GetNbinsX(); iDelim++)
   {
      TH1I *hDetCounts_up   = (TH1I*) hDetVsDelim_up  ->ProjectionY("hDetCounts_up",   iDelim, iDelim);
      TH1I *hDetCounts_down = (TH1I*) hDetVsDelim_down->ProjectionY("hDetCounts_down", iDelim, iDelim);

      // Check if there are events in the histograms
      if (!hDetCounts_up->Integral() && !hDetCounts_down->Integral()) continue;

      ValErrMap asymX90 = CalcDetAsymX90SqrtFormula(*hDetCounts_up, *hDetCounts_down);
      ValErrMap asymX45 = CalcDetAsymX45SqrtFormula(*hDetCounts_up, *hDetCounts_down);
      //ValErrMap asymY45 = CalcDetAsymY45SqrtFormula(*hDetCounts_up, *hDetCounts_down);

      asymX45["phys"].first  = TMath::Sqrt(2)*asymX45["phys"].first;
      asymX45["phys"].second = TMath::Sqrt(2)*asymX45["phys"].second;
      //cout << "test: " << asymX90["phys"] << ", " << asymX45["phys"] << endl;
      ValErrPair asymPhys = utils::CalcWeightedAvrgErr(asymX90["phys"], asymX45["phys"]);
      //cout << asymPhys << endl;

      hAsymVsDelim4Det->SetBinContent(iDelim, asymPhys.first);
      hAsymVsDelim4Det->SetBinError(iDelim, asymPhys.second);
   }

}
*/


/** */
/*
void AsymCalculator::CalcStripAsymmetryByProfile(DrawObjContainer *oc)
{
   // Calculate asymmetries for each target position
   TH2* hChVsDelim_up   = (TH2*) oc->o["hChVsDelim_up"];
   TH2* hChVsDelim_down = (TH2*) oc->o["hChVsDelim_down"];
   TH1* hAsymVsDelim4Ch = (TH1*) oc->o["hAsymVsDelim4Ch"];

   // this is where the output will go
   //TH1 *hPolarProfile = (TH1*) gAsymRoot->fHists->d["profile"]->o["hPolarProfile"];

   if (!hChVsDelim_up || !hChVsDelim_down || !hAsymVsDelim4Ch) {
      Error("AsymCalculator::CalcStripAsymmetryByProfile", "No proper histograms available to calculate asymmetry. Skipping...");
      return;
   }


   for (Int_t iDelim=1; iDelim<=hAsymVsDelim4Ch->GetNbinsX(); iDelim++)
   {
      TH1I *hUp   = (TH1I*) hChVsDelim_up  ->ProjectionY("hUp",   iDelim, iDelim);
      TH1I *hDown = (TH1I*) hChVsDelim_down->ProjectionY("hDown", iDelim, iDelim);

      // Check if there are events in the histograms
      if (!hUp->Integral() && !hDown->Integral()) continue;

      TH1D* hChAsym = CalcAsimAsym(*hUp, *hDown);

      TFitResultPtr fitres = FitChAsymSine(*hChAsym);

      if (fitres.Get()) {
         Double_t val = fitres->Value(0);
         Double_t err = fitres->FitResult::Error(0);
         //printf("val err: %f, %f\n", val, err);

         hAsymVsDelim4Ch->SetBinContent(iDelim, val);
         hAsymVsDelim4Ch->SetBinError(iDelim, err);

         //hPolarProfile->SetBinContent(iDelim, val/gAnaMeasResult->A_N[1]);
         //hPolarProfile->SetBinError(iDelim, err/gAnaMeasResult->A_N[1]);
      } else {
         Error("AsymCalculator::CalcStripAsymmetryByProfile", "Fit error in time bin (delim) %d", iDelim);
      }

      delete hChAsym;
   }

   //for(Int_t i=0; i<gNDelimeters; i++)
   //{
   //   CalcStripAsymmetry(100+i);
   //   //printf("i, p: %d, %f\n", i, gAnaMeasResult->sinphi[100+i].P[0]);
   //   hPolarProfile->SetBinContent(i+1, gAnaMeasResult->sinphi[100+i].P[0]);
   //   hPolarProfile->SetBinError(i+1, gAnaMeasResult->sinphi[100+i].P[1]);
   //}
}
*/


/** */
/*
void AsymCalculator::CalcKinEnergyAChAsym(DrawObjContainer *oc)
{
   TH2 *hChVsKinE_Up      = (TH2*) oc->o["hChVsKinEnergyA_up"];
   TH2 *hChVsKinE_Down    = (TH2*) oc->o["hChVsKinEnergyA_down"];
   TH1 *hKinEnergyAChAsym = (TH1*) oc->o["hKinEnergyAChAsym"];

   for (Int_t iKinE=1; iKinE<=hKinEnergyAChAsym->GetNbinsX(); iKinE++)
   {
      TH1I *hUp   = (TH1I*) hChVsKinE_Up  ->ProjectionY("hUp",   iKinE, iKinE);
      TH1I *hDown = (TH1I*) hChVsKinE_Down->ProjectionY("hDown", iKinE, iKinE);

      // Check if there are events in the histograms
      if (!hUp->Integral() && !hDown->Integral()) continue;

      TH1D* hChAsym = CalcAsimAsym(*hUp, *hDown);

      TFitResultPtr fitres = FitChAsymSine(*hChAsym);

      if (fitres.Get()) {
         Double_t val = fitres->Value(0);
         Double_t err = fitres->FitResult::Error(0);
         //printf("val err: %f, %f\n", val, err);

         hKinEnergyAChAsym->SetBinContent(iKinE, val);
         hKinEnergyAChAsym->SetBinError(iKinE, err);
      } else {
         Error("AsymCalculator::CalcKinEnergyAChAsym", "Fit error in energy bin %d", iKinE);
      }
   }
}
*/


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

         hAsym.SetBinContent(iAsimBin, chAsym.first);
         hAsym.SetBinError(iAsimBin,   chAsym.second);
      }
   }

   //return hAsym;
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
   TF1 fitFunc("fitFunc", "[0] + [1]*sin(x + [2])", -M_PI, M_PI);
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



// Description : calculate Asymmetry
// Input       : int a, int b, int totalA, int btot
// Return      : float Asym, float dAsym
//
// Asym = (A - R * B) / (A + R * B), where R = totalA/totalB
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


/** */
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
