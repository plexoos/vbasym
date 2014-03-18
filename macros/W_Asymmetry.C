#include <vector>

void W_Asymmetry()
{

   gStyle->SetPadBottomMargin(0.15);
   gStyle->SetPadLeftMargin(0.17);
   gStyle->SetMarkerStyle(20);
   gStyle->SetOptDate(0);
   //gStyle->SetErrorX(0);

   TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString inPathScratch = "/star/data05/scratch/fazio/stana_out/runlists/";
   TString outPath       = "~/vbasym_results/plots/4prelim";

   TFile *fileData      = TFile::Open(inPathNew + "run11_pp_transverse/hist/vbana.root");
   TFile *fileDataZ0    = TFile::Open(inPathNew + "run11_pp_transverse_zboson/hist/vbana.root");
   TFile *fileSystRap   = TFile::Open("./histSysRap.root");
   TFile *fileSystPt    = TFile::Open("./histSysPt.root");

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
   TH1 *hSysRap                         = (TH1*) fileSystRap->Get("hSystematics_evol");
   TH1 *hSysPt                          = (TH1*) fileSystPt->Get("hSystematics_evol");

   // Clone the histograms with only statistical errors
   TH1D *hd_Wp_AsymAmpSqrtVsRap_clone = (TH1D*)hd_Wp_AsymAmpSqrtVsRap -> Clone("hd_Wp_AsymAmpSqrtVsRap_clone");
   TH1D *hd_Wp_AsymAmpSqrtVsPt_clone  = (TH1D*)hd_Wp_AsymAmpSqrtVsPt  -> Clone("hd_Wp_AsymAmpSqrtVsPt_clone");
   TH1D *hd_Wm_AsymAmpSqrtVsRap_clone = (TH1D*)hd_Wm_AsymAmpSqrtVsRap -> Clone("hd_Wm_AsymAmpSqrtVsRap_clone");
   TH1D *hd_Wm_AsymAmpSqrtVsPt_clone  = (TH1D*)hd_Wm_AsymAmpSqrtVsPt  -> Clone("hd_Wm_AsymAmpSqrtVsPt_clone");


   // Add the systematic uncertainties

   //float sysRap[6] = { 0., 0.18, 0.32, 0.24, 0.13, 0. }; // vector of relative systematics of An vs W-Rapidity  
   //float sysPt[10] = { 0.49, 0.078, 0.026, 0.012, 0.029, 0.0079, 0.033, 0.066, 0.093, 0.097}; // vector of relative systematics of An vs W-Pt

   const Int_t nRapBins =  hd_Wp_AsymAmpSqrtVsRap->GetNbinsX();
   std::vector<Double_t> sysRap(nRapBins); // vector of relative systematics of An vs W-Rapidity

   for (int i = 1; i <= nRapBins; ++i) {
     sysRap[i-1] =  hSysRap -> GetBinContent(i);
   }

   const Int_t nPtBins =  hd_Wp_AsymAmpSqrtVsPt->GetNbinsX();
   std::vector<Double_t> sysPt(nPtBins); // vector of relative systematics of An vs W-Pt

   for (int i = 1; i <= nPtBins; ++i) {
     sysPt[i-1] =  hSysPt -> GetBinContent(i);
   }

   for (int i = 1; i <= hd_Wp_AsymAmpSqrtVsRap->GetNbinsX(); ++i) { // in general 6 bins...
     Double_t AsymBin = hd_Wp_AsymAmpSqrtVsRap->GetBinContent(i);
     cout  << "W+ Rapidity-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wp_AsymAmpSqrtVsRap->GetBinError(i);
     cout  << "W+ Rapidity-Bin " << i << " errStatBin= " << errStatBin << endl;
     //cout  << "W+ Rapidity-Bin " << i << " Relativ syst.= " << sysRap[i-1]*100 << "%" << endl;
     cout  << "W+ Rapidity-Bin " << i << " Relativ syst.= " << sysRap[i-1] << "%" << endl;
     Double_t errSysBin;
     //errSysBin = fabs(AsymBin * sysRap[i-1]);
     errSysBin = fabs(AsymBin * sysRap[i-1]/100);
     cout  << "W+ Rapidity-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin;
     errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     //Double_t errBin= sqrt(errStatBin**2 + errSysBin**2);  // sum stat. and sys. errors in quadrature
     cout  << "W+ Rapidity-Bin " << i << " errBin= " << errBin << endl;
     hd_Wp_AsymAmpSqrtVsRap->SetBinError(i, errBin);
   }

   for (int i = 1; i <= hd_Wp_AsymAmpSqrtVsPt->GetNbinsX(); ++i) { // in general 10 bins...
     Double_t AsymBin = hd_Wp_AsymAmpSqrtVsPt->GetBinContent(i);
     cout  << "W+ Pt-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wp_AsymAmpSqrtVsPt->GetBinError(i);
     cout  << "W+ Pt-Bin " << i << " errStatBin= " << errStatBin << endl;
     ////cout  << "W+ Pt-Bin " << i << " Relativ syst.= " << sysPt[i-1]*100 << "%" << endl;
     cout  << "W+ Pt-Bin " << i << " Relativ syst.= " << sysPt[i-1] << "%" << endl;
     Double_t errSysBin;
     ////errSysBin = fabs(AsymBin * sysPt[i-1]);
     errSysBin = fabs(AsymBin * sysPt[i-1]/100);
     cout  << "W+ Pt-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin;
     errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     //Double_t errBin= sqrt(errStatBin**2 + errSysBin**2);  // sum stat. and sys. errors in quadrature
     cout  << "W+ Pt-Bin " << i << " errBin= " << errBin << endl;
     hd_Wp_AsymAmpSqrtVsPt->SetBinError(i, errBin);
   }

   for (int i = 1; i <= hd_Wm_AsymAmpSqrtVsRap->GetNbinsX(); ++i) { // in general 6 bins...
     Double_t AsymBin = hd_Wm_AsymAmpSqrtVsRap->GetBinContent(i);
     cout  << "W- Rapidity-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wm_AsymAmpSqrtVsRap->GetBinError(i);
     cout  << "W- Rapidity-Bin " << i << " errStatBin= " << errStatBin << endl;
     //cout  << "W- Rapidity-Bin " << i << " Relativ syst.= " << sysRap[i-1]*100 << "%" << endl;
     cout  << "W- Rapidity-Bin " << i << " Relativ syst.= " << sysRap[i-1] << "%" << endl;
     Double_t errSysBin;
     //errSysBin = fabs(AsymBin * sysRap[i-1]);
     errSysBin = fabs(AsymBin * sysRap[i-1]/100);
     cout  << "W- Rapidity-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin;
     errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     //Double_t errBin= sqrt(errStatBin**2 + errSysBin**2);  // sum stat. and sys. errors in quadrature
     cout  << "W- Rapidity-Bin " << i << " errBin= " << errBin << endl;
     hd_Wm_AsymAmpSqrtVsRap->SetBinError(i, errBin);
   }

   for (int i = 1; i <= hd_Wm_AsymAmpSqrtVsPt->GetNbinsX(); ++i) { // in general 10 bins...
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
     //Double_t errBin= sqrt(errStatBin**2 + errSysBin**2);  // sum stat. and sys. errors in quadrature
     cout  << "W- Pt-Bin " << i << " errBin= " << errBin << endl;
     hd_Wm_AsymAmpSqrtVsPt->SetBinError(i, errBin);
   }


  // Plot the asymmetries: 

  
   TLegend *leg1 = new TLegend(0.15, 0.8, 0.66, 0.9);
   leg1 -> SetFillColor(0);
   leg1 -> SetFillStyle(0);
   leg1 -> SetLineColor(0);
   leg1 -> SetBorderSize(0);
   leg1 -> AddEntry(hd_Wp_AsymAmpSqrtVsEta,"STAR p-p 500 GeV - L = 25 pb^{-1}", "P");

   TLatex *textSTAR = new TLatex(0.2, 0.85, "STAR p-p 500 GeV - L = 25 pb^{-1}");
   textSTAR -> SetNDC(); 
   //textSTAR -> SetTextFont(32); 
   textSTAR -> SetTextSize(0.035);
   TLatex *textWp = new TLatex(0.2, 0.07, "W^{+} #rightarrow l^{+} #nu");
   textWp -> SetNDC(); 
   textWp -> SetTextFont(32);
   textWp -> SetTextSize(0.07);
   TLatex *textWm = new TLatex(0.2, 0.07, "W^{-} #rightarrow l^{-} #nu");
   textWm -> SetNDC(); 
   textWm -> SetTextFont(32);
   textWm -> SetTextSize(0.07);
   TLatex *textZ0 = new TLatex(0.2, 0.8, "Z^{0} #rightarrow l^{+} l^{-}");
   textZ0 -> SetNDC(); 
   textZ0 -> SetTextFont(32);
   textZ0 -> SetTextSize(0.07);
   TLatex *textLumi = new TLatex(0.2, 0.8, "L = 25 pb^{-1}");
   textLumi -> SetNDC(); 
   textLumi -> SetTextFont(32); 
   textLumi -> SetTextSize(0.04);
   TLatex *textPtLim = new TLatex(0.2, 0.8, "0.5 < P_{T} < 7 GeV");
   textPtLim -> SetNDC(); 
   textPtLim -> SetTextFont(32);
   textPtLim -> SetTextSize(0.04);
   TLatex *textRapLim = new TLatex(0.2, 0.8, "|y| < 1");
   textRapLim -> SetNDC(); 
   textRapLim -> SetTextFont(32);
   textRapLim -> SetTextSize(0.04);
   TLatex *textLSys = new TLatex(0.2, 0.75, "Overall pol. syst. = 3.4%");
   textLSys -> SetNDC(); 
   textLSys -> SetTextFont(32);
   textLSys -> SetTextSize(0.04);
  
  TCanvas *cWpAmpEta   = new TCanvas("cWpAmpEta", "An Wp Eta", 600, 600);

  cWpAmpEta -> cd();
  hd_Wp_AsymAmpSqrtVsEta -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsEta -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsEta -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsEta -> SetTitle("Asymmetry amplitude; #eta^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsEta -> GetYaxis() -> SetRangeUser(-1.5, 1.5);
  hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textWp);
  //hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textLumi);
  hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textLSys);
  hd_Wp_AsymAmpSqrtVsEta -> Draw();
  textWp ->  DrawLatex(0.2, 0.2, "W^{+} #rightarrow l^{+} #nu");
  //leg1 -> Draw();

  cWpAmpEta->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsEta.png");
  cWpAmpEta->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsEta.eps");
 
  TCanvas *cWpAmpRap   = new TCanvas("cWpAmpRap", "An Wp Rapidity", 600, 650);

  //TPad*    upperPad = new TPad("upperPad", "upperPad", .005, .7525, .995, .995);
  TPad*    upperPad = new TPad("upperPad", "upperPad", .005, .25, .995, .995);
  TPad*    lowerPad = new TPad("lowerPad", "lowerPad", .005, .005, .995, .25);
  upperPad -> SetBottomMargin(0.017);
  lowerPad -> SetTopMargin(0.017);
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
  hd_Wp_AsymAmpSqrtVsRap -> SetTitle("Asymmetry amplitude; y^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textWp);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  hd_Wp_AsymAmpSqrtVsRap -> Draw("E0");
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.5, 1.5);
  hd_Wp_AsymAmpSqrtVsRap_clone -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsRap_clone -> Draw("same E1");
  //leg1 -> Draw();

  //cWpAmpRap -> cd(2);
  lowerPad->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysRap   -> SetTitle("; y^{W}; syst.(%)");
  hSysRap   -> GetXaxis() -> SetTitleOffset(0.7);
  hSysRap   -> GetXaxis() -> SetNdivisions(8);
  hSysRap   -> GetXaxis() -> SetLabelSize(0.15);
  hSysRap   -> GetYaxis() -> SetTitleOffset(0.5);
  hSysRap   -> GetYaxis() -> SetNdivisions(8);
  hSysRap   -> GetYaxis() -> SetLabelSize(0.1);
  hSysRap   -> SetTitleSize(0.2, "X");
  hSysRap   -> SetTitleSize(0.12, "Y");
  hSysRap   -> Draw("PL");

  //cWpAmpRap -> Update();  
  cWpAmpRap->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsRap.png");
  cWpAmpRap->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsRap.eps");
  
  TCanvas *cWpAmpPt   = new TCanvas("cWpAmpPt", "An Wp Pt", 600, 650);

  TPad*    upperPad_WpPt = new TPad("upperPad_WpPt", "upperPad_WpPt", .005, .25, .995, .995);
  TPad*    lowerPad_WpPt = new TPad("lowerPad_WpPt", "lowerPad_WpPt", .005, .005, .995, .25);
  upperPad_WpPt -> SetBottomMargin(0.017);
  lowerPad_WpPt -> SetTopMargin(0.017);
  lowerPad_WpPt -> SetBottomMargin(0.35);
  upperPad_WpPt -> Draw();
  lowerPad_WpPt -> Draw();

  //cWpAmpPt -> cd();
  upperPad_WpPt->cd();
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsPt -> SetTitle("Asymmetry amplitude; P_{T}^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textSTAR);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textWp);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textRapLim);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textLSys);
  hd_Wp_AsymAmpSqrtVsPt -> Draw("E0");
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetRangeUser(-1.5, 1.5);
  hd_Wp_AsymAmpSqrtVsPt_clone -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt_clone -> Draw("same E1");
  //leg1 -> Draw();

  lowerPad_WpPt->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysPt   -> SetTitle("; P_{T}^{W}; syst.(%)");
  hSysPt   -> GetXaxis() -> SetRangeUser(0, 9.99);
  hSysPt   -> GetXaxis() -> SetTitleOffset(0.75);
  hSysPt   -> GetXaxis() -> SetNdivisions(10);
  hSysPt   -> GetXaxis() -> SetLabelSize(0.15);
  hSysPt   -> GetYaxis() -> SetTitleOffset(0.5);
  hSysPt   -> GetYaxis() -> SetNdivisions(8);
  hSysPt   -> GetYaxis() -> SetLabelSize(0.1);
  hSysPt   -> SetTitleSize(0.2, "X");
  hSysPt   -> SetTitleSize(0.12, "Y");
  hSysPt   -> Draw("PL");

  cWpAmpPt -> Update();  
  cWpAmpPt->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsPt.png");
  cWpAmpPt->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsPt.eps");
  
  TCanvas *cWmAmpEta   = new TCanvas("cWmAmpEta", "An Wm Eta", 600, 600);

  cWmAmpEta -> cd();
  hd_Wm_AsymAmpSqrtVsEta -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsEta -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsEta -> SetTitle("Asymmetry amplitude; #eta^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsEta -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textWm);
  hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsEta -> GetYaxis() -> SetRangeUser(-1.5, 1.5);
  hd_Wm_AsymAmpSqrtVsEta -> Draw();
  textWm ->  DrawLatex(0.2, 0.2, "W^{-} #rightarrow l^{-} #nu");
  //leg1 -> Draw();

  cWmAmpEta->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsEta.png");
  cWmAmpEta->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsEta.eps");

  TCanvas *cWmAmpRap   = new TCanvas("cWmAmpRap", "An Wm Rapidity", 600, 650);

  TPad*    upperPad_WmRap = new TPad("upperPad_WmRap", "upperPad_WmRap", .005, .25, .995, .995);
  TPad*    lowerPad_WmRap = new TPad("lowerPad_WmRap", "lowerPad_WmRap", .005, .005, .995, .25);
  upperPad_WmRap -> SetBottomMargin(0.017);
  lowerPad_WmRap -> SetTopMargin(0.017);
  lowerPad_WmRap -> SetBottomMargin(0.35);
  upperPad_WmRap -> Draw();
  lowerPad_WmRap -> Draw();

  //cWmAmpRap -> cd();
  upperPad_WmRap->cd();
  gPad-> Modified();
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap -> SetTitle("Asymmetry amplitude; y^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsRap -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textWm);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Wm_AsymAmpSqrtVsRap -> Draw("E0");
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.5, 1.5);
  hd_Wm_AsymAmpSqrtVsRap_clone -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap_clone -> Draw("same E1");
  //leg1 -> Draw();

  lowerPad_WmRap->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysRap   -> SetTitle("; y^{W}; syst.(%)");
  hSysRap   -> GetXaxis() -> SetTitleOffset(0.7);
  hSysRap   -> GetXaxis() -> SetNdivisions(8);
  hSysRap   -> GetXaxis() -> SetLabelSize(0.15);
  hSysRap   -> GetYaxis() -> SetTitleOffset(0.5);
  hSysRap   -> GetYaxis() -> SetNdivisions(8);
  hSysRap   -> GetYaxis() -> SetLabelSize(0.1);
  hSysRap   -> SetTitleSize(0.2, "X");
  hSysRap   -> SetTitleSize(0.12, "Y");
  hSysRap   -> Draw("PL");

  cWpAmpRap -> Update();  
  cWmAmpRap->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsRap.png");
  cWmAmpRap->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsRap.eps");

  TCanvas *cWmAmpPt   = new TCanvas("cWmAmpPt", "An Wm Pt", 600, 650);

  TPad*    upperPad_WmPt = new TPad("upperPad_WmPt", "upperPad_WmPt", .005, .25, .995, .995);
  TPad*    lowerPad_WmPt = new TPad("lowerPad_WmPt", "lowerPad_WmPt", .005, .005, .995, .25);
  upperPad_WmPt -> SetBottomMargin(0.017);
  lowerPad_WmPt -> SetTopMargin(0.017);
  lowerPad_WmPt -> SetBottomMargin(0.35);
  upperPad_WmPt -> Draw();
  lowerPad_WmPt -> Draw();

  //cWmAmpPt -> cd();
  upperPad_WmPt->cd();
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt -> SetTitle("Asymmetry amplitude; P_{T}^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsPt -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textSTAR);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textWm);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsPt -> Draw("E0");
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetRangeUser(-1.5, 1.5);
  hd_Wm_AsymAmpSqrtVsPt_clone -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt_clone -> Draw("same E1");
  //leg1 -> Draw();

  lowerPad_WmPt->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysPt   -> SetTitle("; P_{T}^{W}; syst.(%)");
  hSysPt   -> GetXaxis() -> SetRangeUser(0, 9.99);
  hSysPt   -> GetXaxis() -> SetTitleOffset(0.75);
  hSysPt   -> GetXaxis() -> SetNdivisions(10);
  hSysPt   -> GetXaxis() -> SetLabelSize(0.15);
  hSysPt   -> GetYaxis() -> SetTitleOffset(0.5);
  hSysPt   -> GetYaxis() -> SetNdivisions(8);
  hSysPt   -> GetYaxis() -> SetLabelSize(0.1);
  hSysPt   -> SetTitleSize(0.2, "X");
  hSysPt   -> SetTitleSize(0.12, "Y");
  hSysPt   -> Draw("PL");

  cWpAmpPt -> Update();  
  cWmAmpPt->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsPt.png");
  cWmAmpPt->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsPt.eps");


  TCanvas *cZ0AmpRap   = new TCanvas("cZ0AmpRap", "An Z0 Rapidity", 600, 600);


  cZ0AmpRap -> cd();
  hd_Z0_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleOffset(1.6);
  hd_Z0_AsymAmpSqrtVsRap -> SetStats(0);
  hd_Z0_AsymAmpSqrtVsRap -> SetTitle("Asymmetry amplitude; y^{Z^{0}}; A_{N}");
  hd_Z0_AsymAmpSqrtVsRap -> SetMarkerStyle(20);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textZ0);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  //hd_Z0_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Z0_AsymAmpSqrtVsRap -> Draw("E1");

  textSTAR  -> DrawLatex(0.2, 0.3, "STAR p-p 500 GeV - L = 25 pb^{-1}");
  textPtLim -> DrawLatex(0.2, 0.25, "0 < P_{T} < 7 GeV");
  textLSys  -> DrawLatex(0.2, 0.2, "Overall pol. syst. = 3.4%");
  //hd_Z0_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.5, 1.5);

  cZ0AmpRap->SaveAs(outPath + "/hd_Z0_AsymAmpSqrtVsRap.png");
  cZ0AmpRap->SaveAs(outPath + "/hd_Z0_AsymAmpSqrtVsRap.eps");

}
