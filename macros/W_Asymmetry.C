#include <vector>

void W_Asymmetry()
{

   gStyle->SetPadBottomMargin(0.15);
   gStyle->SetPadLeftMargin(0.17);
   gStyle->SetMarkerStyle(20);
   gStyle->SetOptDate(0);
   gStyle->SetErrorX();

   TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString inPathScratch = "/star/data05/scratch/fazio/stana_out/runlists/";
   TString outPath       = "~/vbasym_results/plots/4prelim";

   TFile *fileData         = TFile::Open(inPathNew + "run11_pp_transverse/hist/vbana.root");
   TFile *fileDataZ0       = TFile::Open(inPathNew + "run11_pp_transverse_zboson/hist/vbana.root");
   TFile *fileMCWm         = TFile::Open(inPathNew + "run11_mc_Wm2enu.lis/hist/vbana.root");
   TFile *fileSystRap      = TFile::Open("./histSysRap.root");
   TFile *fileSystGMeanRap = TFile::Open("./histSysGMeanRap.root");
   TFile *fileSystPt       = TFile::Open("./histSysPt.root");
   TFile *fileSystMeanPt   = TFile::Open("./histMeanSysPt.root");

   // take the histograms

   TH1 *hd_Wp_AsymAmpSqrtVsEta          = (TH1*) fileData->Get("asym/asym_wp_phys/hWBosonAsymAmpVsEta_");
   TH1 *hd_Wp_AsymAmpSqrtVsRap          = (TH1*) fileData->Get("asym/asym_wp_phys/hWBosonAsymAmpVsRap_");
   TH1 *hd_Wp_AsymAmpSqrtVsPt           = (TH1*) fileData->Get("asym/asym_wp_phys/hWBosonAsymAmpVsPt_");
   TH1 *hd_Wm_AsymAmpSqrtVsEta          = (TH1*) fileData->Get("asym/asym_wm_phys/hWBosonAsymAmpVsEta_");
   TH1 *hd_Wm_AsymAmpSqrtVsRap          = (TH1*) fileData->Get("asym/asym_wm_phys/hWBosonAsymAmpVsRap_");
   TH1 *hd_Wm_AsymAmpSqrtVsPt           = (TH1*) fileData->Get("asym/asym_wm_phys/hWBosonAsymAmpVsPt_");
   TH1 *hd_Z0_AsymAmpSqrtVsEta          = (TH1*) fileDataZ0->Get("asym/asym_z_phys/hZBosonAsymAmpVsEta_");
   TH1 *hd_Z0_AsymAmpSqrtVsRap          = (TH1*) fileDataZ0->Get("asym/asym_z_phys/hZBosonAsymAmpVsRap_");
   TH1 *hd_Z0_AsymAmpSqrtVsPt           = (TH1*) fileDataZ0->Get("asym/asym_z_phys/hZBosonAsymAmpVsPt_");
   // take the histograms for the systematics 
   TH1 *hSysPt                          = (TH1*) fileSystPt->Get("hSystematics_evol");
   TH1 *hSysMeanPt                      = (TH1*) fileSystMeanPt->Get("hSystematicsMean_evol");
   TH1 *hSysRap                         = (TH1*) fileSystRap->Get("hSystematics_evol");
   TH1 *hSysRapGMean                    = (TH1*) fileSystGMeanRap->Get("hSystematicsGMean_evol");
   TH1 *hSysRapPR                       = (TH1*) fileMCWm->Get("event_mc/hSystematics_evol_PR");
   TH1 *hSysRapMR                       = (TH1*) fileMCWm->Get("event_mc/hSystematics_evol_MR");

   // Clone the histograms with only statistical errors
   TH1D *hd_Wp_AsymAmpSqrtVsRap_clone = (TH1D*)hd_Wp_AsymAmpSqrtVsRap -> Clone("hd_Wp_AsymAmpSqrtVsRap_clone");
   TH1D *hd_Wp_AsymAmpSqrtVsPt_clone  = (TH1D*)hd_Wp_AsymAmpSqrtVsPt  -> Clone("hd_Wp_AsymAmpSqrtVsPt_clone");
   TH1D *hd_Wm_AsymAmpSqrtVsRap_clone = (TH1D*)hd_Wm_AsymAmpSqrtVsRap -> Clone("hd_Wm_AsymAmpSqrtVsRap_clone");
   TH1D *hd_Wm_AsymAmpSqrtVsPt_clone  = (TH1D*)hd_Wm_AsymAmpSqrtVsPt  -> Clone("hd_Wm_AsymAmpSqrtVsPt_clone");


   // Add the systematic uncertainties

   //hSysRapGMean -> SetMarkerSize(2);  
   hSysRapPR -> SetMarkerSize(2);  
   hSysRapMR -> SetMarkerSize(2);  

   const Int_t nRapBins =  hd_Wp_AsymAmpSqrtVsRap->GetNbinsX();
   std::vector<Double_t> sysRapGMean(nRapBins); // vector of relative systematics of An vs W-Rapidity
   //std::vector<Double_t> sysRap(nRapBins); // vector of relative systematics of An vs W-Rapidity
   std::vector<Double_t> sysRapPR(nRapBins); // vector of relative POSITIVE systematics of An vs W-Rapidity 
   std::vector<Double_t> sysRapMR(nRapBins); // vector of relative NEGATIVE systematics of An vs W-Rapidity 

   for (int i = 1; i <= nRapBins; ++i) {
     sysRapGMean[i-1] =  hSysRapGMean -> GetBinContent(i);
     //sysRap[i-1] =  hSysRap -> GetBinContent(i);
     sysRapPR[i-1] =  hSysRapPR -> GetBinContent(i);
     sysRapPR[i-1] =  hSysRapMR -> GetBinContent(i);
   }

   const Int_t nPtBins =  hd_Wp_AsymAmpSqrtVsPt->GetNbinsX();
   std::vector<Double_t> sysPt(nPtBins); // vector of relative systematics of An vs W-Pt

   for (int i = 1; i <= nPtBins; ++i) {
     //sysPt[i-1] =  hSysPt -> GetBinContent(i);
     sysPt[i-1] =  hSysMeanPt -> GetBinContent(i);
   }


   //TGraphAsymmErrors* gd_Wp_AsymAmpSqrtVsRap = new TGraphAsymmErrors(nRapBins);

   for (int i = 1; i <= nRapBins; ++i) { // loop over Rapidity bins
     Double_t AsymBin = hd_Wp_AsymAmpSqrtVsRap->GetBinContent(i);
     cout  << "W+ Rapidity-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wp_AsymAmpSqrtVsRap->GetBinError(i);
     cout  << "W+ Rapidity-Bin " << i << " errStatBin= " << errStatBin << endl;
     cout  << "W+ Rapidity-Bin " << i << " Relative syst.= " << sysRapGMean[i-1] << "%" << endl;
     //cout  << "W+ Rapidity-Bin " << i << " Relative positive syst.= " << sysRapPR[i-1] << "%" << endl;
     //cout  << "W+ Rapidity-Bin " << i << " Relative negative syst.= " << sysRapMR[i-1] << "%" << endl;
     Double_t errSysBin = fabs(AsymBin * sysRapGMean[i-1]/100);
     //Double_t errSysBinPR = fabs(AsymBin * sysRapPR[i-1]/100);
     //Double_t errSysBinMR = fabs(AsymBin * sysRapMR[i-1]/100);
     cout  << "W+ Rapidity-Bin " << i << " errSysBin= " << errSysBin << endl;
     //cout  << "W+ Rapidity-Bin " << i << " errSysBinPR= " << errSysBinPR << endl;
     //cout  << "W+ Rapidity-Bin " << i << " errSysBinMR= " << errSysBinMR << endl;
     Double_t errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     //Double_t errBinPR = sqrt(pow(errStatBin,2) + pow(errSysBinPR,2)) ;  // sum stat. and sys. errors in quadrature
     //Double_t errBinMR = sqrt(pow(errStatBin,2) + pow(errSysBinMR,2)) ;  // sum stat. and sys. errors in quadrature
     cout  << "W+ Rapidity-Bin " << i << " errBin= " << errBin << endl;
     //cout  << "W+ Rapidity-Bin " << i << " errBinPR= " << errBinPR << endl;
     //cout  << "W+ Rapidity-Bin " << i << " errBinMR= " << errBinMR << endl;
     hd_Wp_AsymAmpSqrtVsRap -> SetBinError(i, errBin);
     //hd_Wp_AsymAmpSqrtVsRap -> SetBinError(i, errBinPR);
     //Double_t binX = hd_Wp_AsymAmpSqrtVsRap->GetBinCenter(i);
     //gd_Wp_AsymAmpSqrtVsRap -> SetPoint(i, binX, AsymBin)
     //gd_Wp_AsymAmpSqrtVsRap -> SetPointError(i, 0, 0, errBinMR, errBinPR); 
   } // for

   for (int i = 1; i <= hd_Wp_AsymAmpSqrtVsPt->GetNbinsX(); ++i) { // loop over Pt bins
     Double_t AsymBin = hd_Wp_AsymAmpSqrtVsPt->GetBinContent(i);
     cout  << "W+ Pt-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wp_AsymAmpSqrtVsPt->GetBinError(i);
     cout  << "W+ Pt-Bin " << i << " errStatBin= " << errStatBin << endl;
     cout  << "W+ Pt-Bin " << i << " Relative syst.= " << sysPt[i-1] << "%" << endl;
     Double_t errSysBin;
     errSysBin = fabs(AsymBin * sysPt[i-1]/100);
     cout  << "W+ Pt-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin;
     errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     cout  << "W+ Pt-Bin " << i << " errBin= " << errBin << endl;
     hd_Wp_AsymAmpSqrtVsPt->SetBinError(i, errBin);
   }

   for (int i = 1; i <= nRapBins; ++i) { // loop over Rapidity bins 
     Double_t AsymBin = hd_Wm_AsymAmpSqrtVsRap->GetBinContent(i);
     cout  << "W- Rapidity-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wm_AsymAmpSqrtVsRap->GetBinError(i);
     cout  << "W- Rapidity-Bin " << i << " errStatBin= " << errStatBin << endl;
     cout  << "W- Rapidity-Bin " << i << " Relativ syst.= " << sysRapGMean[i-1] << "%" << endl;
     Double_t errSysBin = fabs(AsymBin * sysRapGMean[i-1]/100);
     cout  << "W- Rapidity-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     cout  << "W- Rapidity-Bin " << i << " errBin= " << errBin << endl;
     hd_Wm_AsymAmpSqrtVsRap->SetBinError(i, errBin);
   }

   for (int i = 1; i <= hd_Wm_AsymAmpSqrtVsPt->GetNbinsX(); ++i) { // loop over Pt bins 
     Double_t AsymBin = hd_Wm_AsymAmpSqrtVsPt->GetBinContent(i);
     cout  << "W- Pt-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wm_AsymAmpSqrtVsPt->GetBinError(i);
     cout  << "W- Pt-Bin " << i << " errStatBin= " << errStatBin << endl;
     //cout  << "W- Pt-Bin " << i << " Relativ syst.= " << sysPt[i-1]*100 << "%" << endl;
     cout  << "W- Pt-Bin " << i << " Relativ syst.= " << sysPt[i-1] << "%" << endl;
     Double_t errSysBin;
     //errSysBin = fabs(AsymBin * sysPt[i-1]);
     errSysBin = fabs(AsymBin * sysPt[i-1]/100);
     cout  << "W- Pt-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin;
     errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     cout  << "W- Pt-Bin " << i << " errBin= " << errBin << endl;
     hd_Wm_AsymAmpSqrtVsPt->SetBinError(i, errBin);
   }


   // Clone the histograms with errors bars rescaled to 2016 and values set to zero. 
   TH1D *hd_Wp_AsymAmpSqrtVsRap_2016Pr = (TH1D*)hd_Wp_AsymAmpSqrtVsRap -> Clone("hd_Wp_AsymAmpSqrtVsRap_2016Pr");
   TH1D *hd_Wp_AsymAmpSqrtVsPt_2016Pr  = (TH1D*)hd_Wp_AsymAmpSqrtVsPt  -> Clone("hd_Wp_AsymAmpSqrtVsPt_2016Pr");
   TH1D *hd_Wm_AsymAmpSqrtVsRap_2016Pr = (TH1D*)hd_Wm_AsymAmpSqrtVsRap -> Clone("hd_Wm_AsymAmpSqrtVsRap_2016Pr");
   TH1D *hd_Wm_AsymAmpSqrtVsPt_2016Pr  = (TH1D*)hd_Wm_AsymAmpSqrtVsPt  -> Clone("hd_Wm_AsymAmpSqrtVsPt_2016Pr");
   Double_t projFactor2016 = 5;

   for (int i = 1; i <= hd_Wp_AsymAmpSqrtVsRap_2016Pr->GetNbinsX(); ++i) { // loop over Rapidity bins 
     hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetBinContent(i, 0);
     Double_t errBin = hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetBinError(i);
     Double_t errProj2016Bin = errBin / projFactor2016;
     hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetBinError(i, errProj2016Bin);
   }

   for (int i = 1; i <= hd_Wp_AsymAmpSqrtVsPt_2016Pr->GetNbinsX(); ++i) { // loop over Pt bins 
     hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetBinContent(i, 0);
     Double_t errBin = hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetBinError(i);
     Double_t errProj2016Bin = errBin / projFactor2016;
     hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetBinError(i, errProj2016Bin);
   }

   for (int i = 1; i <= hd_Wm_AsymAmpSqrtVsRap_2016Pr->GetNbinsX(); ++i) { // loop over Rapidity bins 
     hd_Wm_AsymAmpSqrtVsRap_2016Pr -> SetBinContent(i, 0);
     Double_t errBin = hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetBinError(i);
     Double_t errProj2016Bin = errBin / projFactor2016;
     hd_Wm_AsymAmpSqrtVsRap_2016Pr -> SetBinError(i, errProj2016Bin);
   }

   for (int i = 1; i <= hd_Wm_AsymAmpSqrtVsPt_2016Pr->GetNbinsX(); ++i) { // loop over Pt bins 
     hd_Wm_AsymAmpSqrtVsPt_2016Pr -> SetBinContent(i, 0);
     Double_t errBin = hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetBinError(i);
     Double_t errProj2016Bin = errBin / projFactor2016;
     hd_Wm_AsymAmpSqrtVsPt_2016Pr -> SetBinError(i, errProj2016Bin);
   }


  // Plot the asymmetries: 

 
   TLine *line1 = new TLine(-0.6, 0, 10, 0);
   //line1->SetLineColor(kRed);
   line1->SetLineWidth(2);
   line1->SetLineStyle(2);

   //Double_t xLeg = 0.2;
   Double_t xLeg = 0.43;
   Double_t yLeg = 0.17;
   TLatex *textSTAR = new TLatex(xLeg, yLeg, "STAR p-p 500 GeV #intL = 25 pb^{-1}");
   textSTAR -> SetNDC(); 
   textSTAR -> SetTextSize(0.035);
   TLatex *textWp = new TLatex(0.2, 0.83, "W^{+} #rightarrow l^{+} #nu");
   textWp -> SetNDC(); 
   textWp -> SetTextFont(32);
   textWp -> SetTextSize(0.07);
   TLatex *textWm = new TLatex(0.2, 0.83, "W^{-} #rightarrow l^{-} #nu");
   textWm -> SetNDC(); 
   textWm -> SetTextFont(32);
   textWm -> SetTextSize(0.07);
   TLatex *textArrow = new TLatex(0.18, 0.83, "#uparrow 106%");
   textArrow -> SetNDC(); 
   textArrow -> SetTextFont(32);
   textArrow -> SetTextSize(0.15);
   textArrow -> SetTextColor(kRed);
   TLatex *textZ0 = new TLatex(0.2, 0.83, "Z^{0} #rightarrow l^{+} l^{-}");
   textZ0 -> SetNDC(); 
   textZ0 -> SetTextFont(32);
   textZ0 -> SetTextSize(0.07);
   TLatex *textPtLim = new TLatex(xLeg, yLeg-0.05, "0.5 < P_{T} < 7 GeV");
   textPtLim -> SetNDC(); 
   textPtLim -> SetTextFont(32);
   textPtLim -> SetTextSize(0.04);
   TLatex *textRapLim = new TLatex(xLeg, yLeg-0.05, "|y| < 1");
   textRapLim -> SetNDC(); 
   textRapLim -> SetTextFont(32);
   textRapLim -> SetTextSize(0.04);
   TLatex *textLSys = new TLatex(xLeg,  yLeg-0.1, "3.4% beam pol. uncertainty not shown");
   textLSys -> SetNDC(); 
   textLSys -> SetTextFont(32);
   textLSys -> SetTextSize(0.04);
   TLatex *textStarPrel = new TLatex(0.55, 0.83, "STAR preliminary");
   textStarPrel -> SetNDC(); 
   textStarPrel -> SetTextColor(kRed); 
   textStarPrel -> SetTextFont(32);
   textStarPrel -> SetTextSize(0.06);
   TLatex *textStarPrel_1w = new TLatex(0.6, 0.83, "STAR preliminary");
   textStarPrel_1w -> SetNDC(); 
   textStarPrel_1w -> SetTextColor(kRed); 
   textStarPrel_1w -> SetTextFont(32);
   textStarPrel_1w -> SetTextSize(0.04);
   TLatex *textStarProj = new TLatex(0.27, 0.8, "STAR projection #intL = 600 pb^{-1}");
   textStarProj -> SetNDC(); 
   textStarProj -> SetTextColor(kRed); 
   textStarProj -> SetTextFont(32);
   textStarProj -> SetTextSize(0.06);
  
  TCanvas *cWpAmpEta   = new TCanvas("cWpAmpEta", "An Wp Eta", 600, 600);

  cWpAmpEta -> cd();
  hd_Wp_AsymAmpSqrtVsEta -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsEta -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsEta -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsEta -> SetTitle("; #eta^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsEta -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  //hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textPtLim);
  //hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textLSys);
  hd_Wp_AsymAmpSqrtVsEta -> Draw();
  textPtLim -> DrawLatex(0.2, 0.82, "STAR p-p 500 GeV #int L = 25 pb^{-1}");
  textSTAR  -> DrawLatex(0.2, 0.77, "0.5 < P_{T} < 7 GeV");
  textLSys  -> DrawLatex(0.2, 0.72, "3.4% beam pol. uncertainty not shown");
  textWp    -> DrawLatex(0.2, 0.2, "W^{+} #rightarrow l^{+} #nu");
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsEta -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsEta -> GetXaxis() -> GetXmax(), 0);

  cWpAmpEta->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsEta.png");
  cWpAmpEta->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsEta.eps");
 
  TCanvas *cWpAmpRap   = new TCanvas("cWpAmpRap", "An Wp Rapidity", 600, 650);

  //TPad*    upperPad = new TPad("upperPad", "upperPad", .005, .7525, .995, .995);
  TPad*    upperPad = new TPad("upperPad", "upperPad", .005, .25, .995, .995);
  TPad*    lowerPad = new TPad("lowerPad", "lowerPad", .005, .005, .995, .25);
  upperPad -> SetBottomMargin(0.0);
  lowerPad -> SetTopMargin(0.0);
  //upperPad -> SetBottomMargin(0.017);
  //lowerPad -> SetTopMargin(0.03);
  lowerPad -> SetBottomMargin(0.35);
  upperPad->Draw();
  lowerPad->Draw();

  //cWpAmpRap -> Divide(1,2);
  //cWpAmpRap -> cd(1);
  upperPad->cd();
  //gPad -> SetBottomMargin(0.1);
  gPad-> Modified();
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsRap -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsRap -> SetTitle("; y^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textWp);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textStarPrel);
  hd_Wp_AsymAmpSqrtVsRap -> Draw("E0");
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetLabelFont(62);
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleFont(62);
  hd_Wp_AsymAmpSqrtVsRap_clone -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsRap_clone -> Draw("same E1");
  //Double_t xMin =  hd_Wp_AsymAmpSqrtVsRap_clone  -> GetXaxis() -> GetXmin();
  //Double_d xMax = hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax;
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax(), 0);

  //cWpAmpRap -> cd(2);
  lowerPad->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  TH2F *frameSysRap = new TH2F("frameSysRap","",5,-0.6,0.6,5,0,11);
  //TH2F *frameSysRap = new TH2F("frameSysRap","",5,-0.6,0.6,5,-12,12);
  frameSysRap -> SetStats(0); 
  frameSysRap -> SetTitle("; y^{W}; syst.(%)");
  //frameSysRap -> GetXaxis() -> SetTitleOffset(0.7) 
  frameSysRap -> GetXaxis() -> SetNdivisions(8); 
  frameSysRap -> GetXaxis() -> SetLabelSize(0.15); 
  frameSysRap -> GetXaxis() -> SetLabelOffset(0.03);  
  frameSysRap -> GetYaxis() -> SetNdivisions(8); 
  frameSysRap -> GetYaxis() -> SetLabelSize(0.1); ;
  frameSysRap -> SetTitleSize(0.2, "x");
  frameSysRap -> SetTitleOffset(0.78, "x"); 
  frameSysRap -> SetTitleSize(0.12, "Y");
  frameSysRap -> SetTitleOffset(0.5, "Y"); 
  frameSysRap -> Draw(); 
  //hSysRapPR   -> SetTitle("; y^{W}; syst.(%)");
  //hSysRapPR   -> GetXaxis() -> SetTitleOffset(0.7);
  //hSysRapPR   -> GetXaxis() -> SetNdivisions(8);
  //hSysRapPR   -> GetXaxis() -> SetLabelSize(0.15);
  //hSysRapPR   -> GetYaxis() -> SetTitleOffset(0.5);
  //hSysRapPR   -> GetYaxis() -> SetNdivisions(8);
  //hSysRapPR   -> GetYaxis() -> SetLabelSize(0.1);
  //hSysRapPR   -> SetTitleSize(0.2, "X");
  //hSysRapPR   -> SetTitleSize(0.12, "Y");
  hSysRapGMean   -> Draw("same P");
  //hSysRapPR   -> Draw("same P");
  //hSysRapMR   -> Draw("same P");

  //cWpAmpRap -> Update();  
  cWpAmpRap->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsRap.png");
  cWpAmpRap->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsRap.eps");
  
  TCanvas *cWpAmpPt   = new TCanvas("cWpAmpPt", "An Wp Pt", 600, 650);

  TPad*    upperPad_WpPt = new TPad("upperPad_WpPt", "upperPad_WpPt", .005, .25, .995, .995);
  TPad*    lowerPad_WpPt = new TPad("lowerPad_WpPt", "lowerPad_WpPt", .005, .005, .995, .25);
  upperPad_WpPt -> SetBottomMargin(0.0);
  lowerPad_WpPt -> SetTopMargin(0.0);
  //upperPad_WpPt -> SetBottomMargin(0.017);
  //lowerPad_WpPt -> SetTopMargin(0.017);
  lowerPad_WpPt -> SetBottomMargin(0.35);
  upperPad_WpPt -> Draw();
  lowerPad_WpPt -> Draw();

  //cWpAmpPt -> cd();
  upperPad_WpPt->cd();
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsPt -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textSTAR);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textWp);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textRapLim);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textLSys);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textStarPrel);
  hd_Wp_AsymAmpSqrtVsPt -> Draw("E0");
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetLabelFont(62);
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleFont(62);
  hd_Wp_AsymAmpSqrtVsPt_clone -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt_clone -> Draw("same E1");
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmax(), 0);

  lowerPad_WpPt->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysMeanPt   -> SetTitle("; P_{T}^{W}; syst.(%)");
  hSysMeanPt   -> GetXaxis() -> SetRangeUser(0, 9.99);
  hSysMeanPt   -> GetXaxis() -> SetTitleOffset(0.75);
  hSysMeanPt   -> GetXaxis() -> SetNdivisions(10);
  hSysMeanPt   -> GetXaxis() -> SetLabelSize(0.15);
  hSysMeanPt   -> GetYaxis() -> SetTitleOffset(0.5);
  hSysMeanPt   -> GetYaxis() -> SetNdivisions(8);
  hSysMeanPt   -> GetYaxis() -> SetLabelSize(0.1);
  hSysMeanPt   -> GetYaxis() -> SetRangeUser(0, 27);
  hSysMeanPt   -> SetBinContent(1,0); // mask the syst. for the first Pt bin
  hSysMeanPt   -> SetTitleSize(0.2, "X");
  hSysMeanPt   -> SetTitleSize(0.12, "Y");
  hSysMeanPt   -> Draw("P");
  //TArrow *arrow = new TArrow(hSysMeanPt->GetBinCenter(1), 5, hSysMeanPt->GetBinCenter(1), 20, 0.15, "|>");
  //arrow -> SetAngle(40);
  //arrow -> SetLineWidth(2);
  //arrow -> Draw("same");
  textArrow ->  Draw();

  cWpAmpPt -> Update();  
  cWpAmpPt->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsPt.png");
  cWpAmpPt->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsPt.eps");
  
  TCanvas *cWmAmpEta   = new TCanvas("cWmAmpEta", "An Wm Eta", 600, 600);

  cWmAmpEta -> cd();
  hd_Wm_AsymAmpSqrtVsEta -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsEta -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsEta -> SetTitle("; #eta^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsEta -> SetMarkerStyle(20);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textWm);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textPtLim);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsEta -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wm_AsymAmpSqrtVsEta -> Draw();
  textPtLim -> DrawLatex(0.2, 0.82, "STAR p-p 500 GeV #intL = 25 pb^{-1}");
  textSTAR  -> DrawLatex(0.2, 0.77, "0.5 < P_{T} < 7 GeV");
  textLSys  -> DrawLatex(0.2, 0.72, "3.4% beam pol. uncertainty not shown");
  textWm ->  DrawLatex(0.2, 0.2, "W^{-} #rightarrow l^{-} #nu");
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsEta -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsEta -> GetXaxis() -> GetXmax(), 0);

  cWmAmpEta->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsEta.png");
  cWmAmpEta->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsEta.eps");

  TCanvas *cWmAmpRap   = new TCanvas("cWmAmpRap", "An Wm Rapidity", 600, 650);

  TPad*    upperPad_WmRap = new TPad("upperPad_WmRap", "upperPad_WmRap", .005, .25, .995, .995);
  TPad*    lowerPad_WmRap = new TPad("lowerPad_WmRap", "lowerPad_WmRap", .005, .005, .995, .25);
  upperPad_WmRap -> SetBottomMargin(0.0);
  lowerPad_WmRap -> SetTopMargin(0.0);
  //upperPad_WmRap -> SetBottomMargin(0.017);
  //lowerPad_WmRap -> SetTopMargin(0.03);
  lowerPad_WmRap -> SetBottomMargin(0.35);
  upperPad_WmRap -> Draw();
  lowerPad_WmRap -> Draw();

  //cWmAmpRap -> cd();
  upperPad_WmRap->cd();
  gPad-> Modified();
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap -> SetTitle("; y^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsRap -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textWm);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textStarPrel);
  hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Wm_AsymAmpSqrtVsRap -> Draw("E0");
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsRap_clone -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap_clone -> Draw("same E1");
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax(), 0);

  lowerPad_WmRap->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  frameSysRap -> Draw(); 
  //hSysRapPR   -> SetTitle("; y^{W}; syst.(%)");
  //hSysRapPR   -> GetXaxis() -> SetTitleOffset(0.7);
  //hSysRapPR   -> GetXaxis() -> SetNdivisions(8);
  //hSysRapPR   -> GetXaxis() -> SetLabelSize(0.15);
  //hSysRapPR   -> GetYaxis() -> SetTitleOffset(0.5);
  //hSysRapPR   -> GetYaxis() -> SetNdivisions(8);
  //hSysRapPR   -> GetYaxis() -> SetLabelSize(0.1);
  //hSysRapPR   -> SetTitleSize(0.2, "X");
  //hSysRapPR   -> SetTitleSize(0.12, "Y");
  hSysRapGMean   -> Draw("same P");
  //hSysRapPR   -> Draw("same P");
  //hSysRapMR   -> Draw("same P");

  cWpAmpRap -> Update();  
  cWmAmpRap->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsRap.png");
  cWmAmpRap->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsRap.eps");

  TCanvas *cWmAmpPt   = new TCanvas("cWmAmpPt", "An Wm Pt", 600, 650);

  TPad*    upperPad_WmPt = new TPad("upperPad_WmPt", "upperPad_WmPt", .005, .25, .995, .995);
  TPad*    lowerPad_WmPt = new TPad("lowerPad_WmPt", "lowerPad_WmPt", .005, .005, .995, .25);
  upperPad_WmPt -> SetBottomMargin(0.0);
  lowerPad_WmPt -> SetTopMargin(0.0);
  //upperPad_WmPt -> SetBottomMargin(0.017);
  //lowerPad_WmPt -> SetTopMargin(0.017);
  lowerPad_WmPt -> SetBottomMargin(0.35);
  upperPad_WmPt -> Draw();
  lowerPad_WmPt -> Draw();

  //cWmAmpPt -> cd();
  upperPad_WmPt->cd();
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsPt -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textSTAR);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textWm);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textRapLim);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textStarPrel);
  hd_Wm_AsymAmpSqrtVsPt -> Draw("E0");
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsPt_clone -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt_clone -> Draw("same E1");
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmax(), 0);

  lowerPad_WmPt->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysMeanPt   -> SetTitle("; P_{T}^{W}; syst.(%)");
  hSysMeanPt   -> GetXaxis() -> SetRangeUser(0, 9.99);
  hSysMeanPt   -> GetXaxis() -> SetTitleOffset(0.75);
  hSysMeanPt   -> GetXaxis() -> SetNdivisions(10);
  hSysMeanPt   -> GetXaxis() -> SetLabelSize(0.15);
  hSysMeanPt   -> GetYaxis() -> SetTitleOffset(0.5);
  hSysMeanPt   -> GetYaxis() -> SetNdivisions(8);
  hSysMeanPt   -> GetYaxis() -> SetLabelSize(0.1);
  hSysMeanPt   -> SetTitleSize(0.2, "X");
  hSysMeanPt   -> SetTitleSize(0.12, "Y");
  hSysMeanPt   -> Draw("P");
  textArrow    ->  Draw();

  cWmAmpPt -> Update();  
  cWmAmpPt->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsPt.png");
  cWmAmpPt->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsPt.eps");


  TCanvas *cZ0AmpRap   = new TCanvas("cZ0AmpRap", "An Z0 Rapidity", 600, 600);


  cZ0AmpRap -> cd();
  hd_Z0_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleOffset(1.6);
  hd_Z0_AsymAmpSqrtVsRap -> SetStats(0);
  hd_Z0_AsymAmpSqrtVsRap -> SetTitle("; y^{Z^{0}}; A_{N}");
  hd_Z0_AsymAmpSqrtVsRap -> SetMarkerStyle(20);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textZ0);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  //hd_Z0_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textStarPrel_1w);
  hd_Z0_AsymAmpSqrtVsRap -> Draw("E1");
  line1 -> DrawLine(hd_Z0_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmin(), 0, hd_Z0_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax(), 0);

  textSTAR  -> DrawLatex(0.2, 0.3, "STAR p-p 500 GeV #intL = 25 pb^{-1}");
  textPtLim -> DrawLatex(0.2, 0.25, "0 < P_{T} < 25 GeV");
  textLSys  -> DrawLatex(0.2, 0.2, "3.4% beam pol. uncertainty not shown");
  //hd_Z0_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.49, 1.49);


  cZ0AmpRap->SaveAs(outPath + "/hd_Z0_AsymAmpSqrtVsRap.png");
  cZ0AmpRap->SaveAs(outPath + "/hd_Z0_AsymAmpSqrtVsRap.eps");


  TCanvas *cWAmpProjRap   = new TCanvas("cWAmpProjRap", "W An 2016 projection Rapidity", 600, 650);

  gStyle -> SetErrorX(0);

  TPad*    upperPad_WRapProj = new TPad("upperPad_WRapProj", "upperPad_WRapProj", .005, .25, .995, .995);
  TPad*    lowerPad_WRapProj = new TPad("lowerPad_WRapProj", "lowerPad_WRapProj", .005, .005, .995, .25);
  upperPad_WRapProj -> SetBottomMargin(0.0);
  lowerPad_WRapProj -> SetTopMargin(0.0);
  lowerPad_WRapProj -> SetBottomMargin(0.35);
  upperPad_WRapProj -> Draw();
  lowerPad_WRapProj -> Draw();

  upperPad_WRapProj->cd();
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> SetTitle("; y^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> SetMarkerStyle(20);
  //hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetListOfFunctions() -> Add(textWm);
  //hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetListOfFunctions() -> Add(textPtLim);
  //hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetListOfFunctions() -> Add(textStarProj);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> Draw("E1");

  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> Draw("same E1");
 
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmax(), 0);
 
  TLegend *leg1 = new TLegend(0.2, 0.03, 0.7, 0.2);
  leg1 -> SetFillColor(0);
  leg1 -> SetFillStyle(0);
  leg1 -> SetLineColor(0);
  leg1 -> SetBorderSize(0);
  leg1 -> AddEntry(hd_Wp_AsymAmpSqrtVsRap_2016Pr,"W^{+} stat. uncertainty", "l");
  leg1 -> AddEntry(hd_Wm_AsymAmpSqrtVsRap_2016Pr,"W^{-} stat. uncertainty", "l");
  leg1  -> Draw();

  lowerPad_WRapProj->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  frameSysRap  -> Draw(); 
  hSysRapGMean -> Draw("same P");
  textArrow    -> Draw();

  cWAmpProjRap -> Update();  
  cWAmpProjRap->SaveAs(outPath + "/hd_WAsym2016ProjRap.png");
  cWAmpProjRap->SaveAs(outPath + "/hd_WAsym2016ProjRap.eps");


  TCanvas *cWAmpProjRapZoom   = new TCanvas("cWAmpProjRapZoom", "W An 2016 projection Rapidity Zoom", 600, 650);

  TPad*    upperPad_WRapProjZoom = new TPad("upperPad_WRapProjZoom", "upperPad_WRapProjZoom", .005, .25, .995, .995);
  TPad*    lowerPad_WRapProjZoom = new TPad("lowerPad_WRapProjZoom", "lowerPad_WRapProjZoom", .005, .005, .995, .25);
  upperPad_WRapProjZoom -> SetBottomMargin(0.0);
  lowerPad_WRapProjZoom -> SetTopMargin(0.0);
  lowerPad_WRapProjZoom -> SetBottomMargin(0.35);
  upperPad_WRapProjZoom -> Draw();
  lowerPad_WRapProjZoom -> Draw();

  upperPad_WRapProjZoom->cd();
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetRangeUser(-0.49, 0.49);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr -> Draw("E1");

  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> Draw("same E1");
 
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmax(), 0);
  leg1  -> Draw();

  lowerPad_WRapProjZoom->cd();
  gPad -> SetGrid(0);
  gPad -> Modified();
  frameSysRap  -> Draw(); 
  hSysRapGMean -> Draw("same P");
  textArrow    -> Draw();

  cWAmpProjRapZoom -> Update();  
  cWAmpProjRapZoom->SaveAs(outPath + "/hd_WAsym2016ProjRap_zoom.png");
  cWAmpProjRapZoom->SaveAs(outPath + "/hd_WAsym2016ProjRap_zoom.eps");


  TCanvas *cWAmpProjPt   = new TCanvas("cWAmpProjPt", "W An 2016 projection Pt", 600, 650);

  TPad*    upperPad_WPtProj = new TPad("upperPad_WPtProj", "upperPad_WPtProj", .005, .25, .995, .995);
  TPad*    lowerPad_WPtProj = new TPad("lowerPad_WPtProj", "lowerPad_WPtProj", .005, .005, .995, .25);
  upperPad_WPtProj -> SetBottomMargin(0.0);
  lowerPad_WPtProj -> SetTopMargin(0.0);
  lowerPad_WPtProj -> SetBottomMargin(0.35);
  upperPad_WPtProj -> Draw();
  lowerPad_WPtProj -> Draw();

  upperPad_WPtProj->cd();
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> SetMarkerStyle(20);
  //hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetListOfFunctions() -> Add(textWm);
  //hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetListOfFunctions() -> Add(textRapLim);
  //hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetListOfFunctions() -> Add(textStarProj);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> Draw("E1");

  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> Draw("same E1");
 
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmax(), 0);
  leg1  -> Draw();

  lowerPad_WPtProj->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysMeanPt   -> Draw("P");
  textArrow    -> Draw();

  cWAmpProjPt -> Update();  
  cWAmpProjPt->SaveAs(outPath + "/hd_WAsym2016ProjPt.png");
  cWAmpProjPt->SaveAs(outPath + "/hd_WAsym2016ProjPt.eps");


  TCanvas *cWAmpProjPtZoom   = new TCanvas("cWAmpProjPtZoom", "W An 2016 projection Pt Zoom", 600, 650);

  TPad*    upperPad_WPtProjZoom = new TPad("upperPad_WPtProjZoom", "upperPad_WPtProjZoom", .005, .25, .995, .995);
  TPad*    lowerPad_WPtProjZoom = new TPad("lowerPad_WPtProjZoom", "lowerPad_WPtProjZoom", .005, .005, .995, .25);
  upperPad_WPtProjZoom -> SetBottomMargin(0.0);
  lowerPad_WPtProjZoom -> SetTopMargin(0.0);
  lowerPad_WPtProjZoom -> SetBottomMargin(0.35);
  upperPad_WPtProjZoom -> Draw();
  lowerPad_WPtProjZoom -> Draw();

  upperPad_WPtProjZoom->cd();
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetRangeUser(-0.49, 0.49);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr -> Draw("E1");

  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> Draw("same E1");
 
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmax(), 0);
  leg1  -> Draw();

  lowerPad_WPtProjZoom->cd();
  gPad -> SetGrid(0);
  gPad -> Modified();
  hSysMeanPt   -> Draw("P");
  textArrow    -> Draw();

  cWAmpProjPtZoom -> Update();  
  cWAmpProjPtZoom->SaveAs(outPath + "/hd_WAsym2016ProjPt_zoom.png");
  cWAmpProjPtZoom->SaveAs(outPath + "/hd_WAsym2016ProjPt_zoom.eps");

}
