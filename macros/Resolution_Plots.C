#include <Riostream.h>
#include <iostream.h>
#include <fstream>
#include <sstream>


void Resolution_Plots()
{
   TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString inPathScratch = "/star/data05/scratch/fazio/stana_out/runlists/";
   //TString outPath       = "~/vbasym_results/plots/4prelim";
   TString outPath       = "~/vbasym_results/plots/postPrelim/resolution";


   TFile *fileMCWp      = TFile::Open(inPathNew + "run11_mc_Wp2enu.lis/hist/vbana.root");

   TH1 *hWp_ResolutionPt_bin1           = (TH1 *)fileMCWp->Get("event_mc/hResolution_RecoilInAcc_Pt_bin1");
   TH1 *hWp_ResolutionPt_bin2           = (TH1 *)fileMCWp->Get("event_mc/hResolution_RecoilInAcc_Pt_bin2");
   TH1 *hWp_ResolutionPt_bin3           = (TH1 *)fileMCWp->Get("event_mc/hResolution_RecoilInAcc_Pt_bin3");
   TH1 *hWp_ResolutionPt_bin4           = (TH1 *)fileMCWp->Get("event_mc/hResolution_RecoilInAcc_Pt_bin4");
   TH1 *hWp_ResolutionPt_bin5           = (TH1 *)fileMCWp->Get("event_mc/hResolution_RecoilInAcc_Pt_bin5");
   TH1 *hWp_ResolutionPt_bin6           = (TH1 *)fileMCWp->Get("event_mc/hResolution_RecoilInAcc_Pt_bin6");
   TH1 *hWp_ResolutionPt_bin7           = (TH1 *)fileMCWp->Get("event_mc/hResolution_RecoilInAcc_Pt_bin7");
   TH1 *hWp_AbsoResolutionPt_bin1       = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolution_RecoilInAcc_Pt_bin1");
   TH1 *hWp_AbsoResolutionPt_bin2       = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolution_RecoilInAcc_Pt_bin2");
   TH1 *hWp_AbsoResolutionPt_bin3       = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolution_RecoilInAcc_Pt_bin3");
   TH1 *hWp_AbsoResolutionPt_bin4       = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolution_RecoilInAcc_Pt_bin4");
   TH1 *hWp_AbsoResolutionPt_bin5       = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolution_RecoilInAcc_Pt_bin5");
   TH1 *hWp_AbsoResolutionPt_bin6       = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolution_RecoilInAcc_Pt_bin6");
   TH1 *hWp_AbsoResolutionPt_bin7       = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolution_RecoilInAcc_Pt_bin7");
   TH1 *hWp_ResolutionPtCorr_bin1       = (TH1 *)fileMCWp->Get("event_mc/hResolutionPtCorr_bin1");
   TH1 *hWp_ResolutionPtCorr_bin2       = (TH1 *)fileMCWp->Get("event_mc/hResolutionPtCorr_bin2");
   TH1 *hWp_ResolutionPtCorr_bin3       = (TH1 *)fileMCWp->Get("event_mc/hResolutionPtCorr_bin3");
   TH1 *hWp_ResolutionPtCorr_bin4       = (TH1 *)fileMCWp->Get("event_mc/hResolutionPtCorr_bin4");
   TH1 *hWp_ResolutionPtCorr_bin5       = (TH1 *)fileMCWp->Get("event_mc/hResolutionPtCorr_bin5");
   TH1 *hWp_ResolutionPtCorr_bin6       = (TH1 *)fileMCWp->Get("event_mc/hResolutionPtCorr_bin6");
   TH1 *hWp_ResolutionPtCorr_bin7       = (TH1 *)fileMCWp->Get("event_mc/hResolutionPtCorr_bin7");
   TH1 *hWp_ResolutionRap               = (TH1 *)fileMCWp->Get("event_mc/hResolutionRap");
   TH1 *hWp_ResolutionRap_bin1          = (TH1 *)fileMCWp->Get("event_mc/hResolutionRap_bin1");
   TH1 *hWp_ResolutionRap_bin2          = (TH1 *)fileMCWp->Get("event_mc/hResolutionRap_bin2");
   TH1 *hWp_ResolutionRap_bin3          = (TH1 *)fileMCWp->Get("event_mc/hResolutionRap_bin3");
   TH1 *hWp_AbsoResolutionRap_bin1      = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolutionRap_bin1");
   TH1 *hWp_AbsoResolutionRap_bin2      = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolutionRap_bin2");
   TH1 *hWp_AbsoResolutionRap_bin3      = (TH1 *)fileMCWp->Get("event_mc/hAbsoResolutionRap_bin3");
   TH1 *hWp_hAn_evol_ZK                 = (TH1 *)fileMCWp->Get("event_mc/hAn_evol_ZK");
   TH1 *hWp_hWBosonRapGen               = (TH1 *)fileMCWp->Get("event_mc/hWBosonRap");

   //Styles:
   gStyle->SetPadBottomMargin(0.15);
   gStyle->SetPadLeftMargin(0.17);
   gStyle->SetStatFontSize(0.05);
   gStyle->SetOptStat("mruoi");
   gStyle->SetPadRightMargin(0.25);
   gStyle->SetMarkerStyle(20);
   gStyle->SetStatX(0.99);
   gStyle->SetOptFit(1);
   gStyle->SetOptDate(0);
   gStyle->SetPadGridX(0);
   gStyle->SetPadGridY(0);



   // RAPIDITY ABSOLUTE RESOLUTION 

   TCanvas *cAbsoResoRap  = new TCanvas("cAbsoResoRap", "Absolute resolution in W-Rapidity", 700, 700);
   
   cAbsoResoRap -> Divide(2,2);

   //TF1 *fGaus = new TF1("fGaus", "gaus", -2, 2);
   cAbsoResoRap_1 -> cd();
   hWp_AbsoResolutionRap_bin1 -> Draw();
   hWp_AbsoResolutionRap_bin1 -> Fit("gaus", "","", -0.2, 0.4);
   hWp_AbsoResolutionRap_bin1 -> GetFunction("gaus") -> Draw("same");
   cAbsoResoRap_2 -> cd();
   hWp_AbsoResolutionRap_bin2 -> Fit("gaus", "","", -0.3, 0.3);
   hWp_AbsoResolutionRap_bin2 -> Draw();
   hWp_AbsoResolutionRap_bin2 -> GetFunction("gaus") -> Draw("same");
   cAbsoResoRap_3 -> cd();
   hWp_AbsoResolutionRap_bin3 -> Fit("gaus", "","", -0.4, 0.2);
   hWp_AbsoResolutionRap_bin3 -> Draw();
   hWp_AbsoResolutionRap_bin3 -> GetFunction("gaus") -> Draw("same");


   //***********************

       double mean_AbsoResoBin1  =  hWp_AbsoResolutionRap_bin1 -> GetFunction("gaus") -> GetParameter(1);   
       double mean_AbsoResoBin2  =  hWp_AbsoResolutionRap_bin2 -> GetFunction("gaus") -> GetParameter(1);     
       double mean_AbsoResoBin3  =  hWp_AbsoResolutionRap_bin3 -> GetFunction("gaus") -> GetParameter(1);     
       double sigma_AbsoResoBin1 =  hWp_AbsoResolutionRap_bin1 -> GetFunction("gaus") -> GetParameter(2);   
       double sigma_AbsoResoBin2 =  hWp_AbsoResolutionRap_bin2 -> GetFunction("gaus") -> GetParameter(2);     
       double sigma_AbsoResoBin3 =  hWp_AbsoResolutionRap_bin3 -> GetFunction("gaus") -> GetParameter(2);   
       cout << "mean_AbsoResoBin1 = " << mean_AbsoResoBin1 << endl;   
       cout << "mean_AbsoResoBin2 = " << mean_AbsoResoBin2 << endl;   
       cout << "mean_AbsoResoBin3 = " << mean_AbsoResoBin3 << endl;     
       cout << "sigma_AbsoResoBin1 = " << sigma_AbsoResoBin1 << endl;   
       cout << "sigma_AbsoResoBin2 = " << sigma_AbsoResoBin2 << endl;   
       cout << "sigma_AbsoResoBin3 = " << sigma_AbsoResoBin3 << endl; 

   //***********************

  cAbsoResoRap -> SaveAs(outPath + "/hWp_AbsoResolutionRap.png");
  cAbsoResoRap -> SaveAs(outPath + "/hWp_AbsoResolutionRap.eps");


   // RAPIDITY RESOLUTION 

   TCanvas *cResoRap  = new TCanvas("cResoRap", "Resolution in W-Rapidity", 700, 700);
   
   cResoRap -> Divide(2,2);

   //TF1 *fGaus = new TF1("fGaus", "gaus", -2, 2);
   cResoRap_1 -> cd();
   hWp_ResolutionRap_bin1 -> Draw();
   hWp_ResolutionRap_bin1 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionRap_bin1 -> GetFunction("gaus") -> Draw("same");
   cResoRap_2 -> cd();
   hWp_ResolutionRap_bin2 -> Fit("gaus", "","", -1.5, 1.5);
   hWp_ResolutionRap_bin2 -> Draw();
   hWp_ResolutionRap_bin2 -> GetFunction("gaus") -> Draw("same");
   cResoRap_3 -> cd();
   hWp_ResolutionRap_bin3 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionRap_bin3 -> Draw();
   hWp_ResolutionRap_bin3 -> GetFunction("gaus") -> Draw("same");


   //***********************
       //TF1 *f1g = hWp_An_evol_ZK_Vs_RapGen_pjy1->GetFunction("gaus");

       double mean_ResoBin1  =  hWp_ResolutionRap_bin1 -> GetFunction("gaus") -> GetParameter(1);   
       double mean_ResoBin2  =  hWp_ResolutionRap_bin2 -> GetFunction("gaus") -> GetParameter(1);     
       double mean_ResoBin3  =  hWp_ResolutionRap_bin3 -> GetFunction("gaus") -> GetParameter(1);     
       double sigma_ResoBin1 =  hWp_ResolutionRap_bin1 -> GetFunction("gaus") -> GetParameter(2);   
       double sigma_ResoBin2 =  hWp_ResolutionRap_bin2 -> GetFunction("gaus") -> GetParameter(2);     
       double sigma_ResoBin3 =  hWp_ResolutionRap_bin3 -> GetFunction("gaus") -> GetParameter(2);   
       cout << "mean_ResoBin1 = " << mean_ResoBin1 << endl;   
       cout << "mean_ResoBin2 = " << mean_ResoBin2 << endl;   
       cout << "mean_ResoBin3 = " << mean_ResoBin3 << endl;     
       cout << "sigma_ResoBin1 = " << sigma_ResoBin1 << endl;   
       cout << "sigma_ResoBin2 = " << sigma_ResoBin2 << endl;   
       cout << "sigma_ResoBin3 = " << sigma_ResoBin3 << endl; 

   //***********************

   TH1D *hResolutionRap = new TH1D("hResolutionRap","",3,-0.6,0.6);
       hResolutionRap   -> SetBinContent(1, mean_ResoBin1);
       hResolutionRap   -> SetBinContent(2, mean_ResoBin2);
       hResolutionRap   -> SetBinContent(3, mean_ResoBin3);
       hResolutionRap   -> SetBinError(1, sigma_ResoBin1);
       hResolutionRap   -> SetBinError(2, sigma_ResoBin2);
       hResolutionRap   -> SetBinError(3, sigma_ResoBin3);

   TLine *line0 = new TLine(hResolutionRap -> GetXaxis() -> GetXmin(), 0, hResolutionRap -> GetXaxis() -> GetXmax(), 0);
   line0 -> SetLineWidth(2);
   line0 -> SetLineStyle(2);

   TCanvas *cResoRap_summary  = new TCanvas("cResoRap_summary", "Resolution in W-Rapidity", 700, 700);
   cResoRap_summary -> cd();

   hResolutionRap   -> Draw("e");
   line0 -> Draw();



   // PT ABSOLUTE RESOLUTION BEFORE CORRECTION

   TCanvas *cAbsoResoPt  = new TCanvas("cAbsoResoPt", "Absolute resolution (before correction) in W-Pt", 700, 700);
   
   cAbsoResoPt -> Divide(2,4);

   //TF1 *fGaus = new TF1("fGaus", "gaus", -2, 2);
   cAbsoResoPt_1 -> cd();
   hWp_AbsoResolutionPt_bin1 -> Draw();
   hWp_AbsoResolutionPt_bin1 -> Fit("gaus", "","", -2, 2);
   hWp_AbsoResolutionPt_bin1 -> GetFunction("gaus") -> Draw("same");
   cAbsoResoPt_2 -> cd();
   hWp_AbsoResolutionPt_bin2 -> Fit("gaus", "","", -2, 2);
   hWp_AbsoResolutionPt_bin2 -> Draw();
   hWp_AbsoResolutionPt_bin2 -> GetFunction("gaus") -> Draw("same");
   cAbsoResoPt_3 -> cd();
   hWp_AbsoResolutionPt_bin3 -> Fit("gaus", "","", -2, 2);
   hWp_AbsoResolutionPt_bin3 -> Draw();
   hWp_AbsoResolutionPt_bin3 -> GetFunction("gaus") -> Draw("same");
   cAbsoResoPt_4 -> cd();
   hWp_AbsoResolutionPt_bin4 -> Fit("gaus", "","", -2, 2);
   hWp_AbsoResolutionPt_bin4 -> Draw();
   hWp_AbsoResolutionPt_bin4 -> GetFunction("gaus") -> Draw("same");
   cAbsoResoPt_5 -> cd();
   hWp_AbsoResolutionPt_bin5 -> Fit("gaus", "","", -2, 2);
   hWp_AbsoResolutionPt_bin5 -> Draw();
   hWp_AbsoResolutionPt_bin5 -> GetFunction("gaus") -> Draw("same");
   cAbsoResoPt_6 -> cd();
   hWp_AbsoResolutionPt_bin6 -> Fit("gaus", "","", -2, 2);
   hWp_AbsoResolutionPt_bin6 -> Draw();
   hWp_AbsoResolutionPt_bin6 -> GetFunction("gaus") -> Draw("same");
   cAbsoResoPt_7 -> cd();
   hWp_AbsoResolutionPt_bin7 -> Fit("gaus", "","", -2, 2);
   hWp_AbsoResolutionPt_bin7 -> Draw();
   hWp_AbsoResolutionPt_bin7 -> GetFunction("gaus") -> Draw("same");
  


   //***********************
       //TF1 *f1g = hWp_An_evol_ZK_Vs_PtGen_pjy1->GetFunction("gaus");

       double Ptmean_AbsoResoBin1  =  hWp_AbsoResolutionPt_bin1 -> GetFunction("gaus") -> GetParameter(1);   
       double Ptmean_AbsoResoBin2  =  hWp_AbsoResolutionPt_bin2 -> GetFunction("gaus") -> GetParameter(1);     
       double Ptmean_AbsoResoBin3  =  hWp_AbsoResolutionPt_bin3 -> GetFunction("gaus") -> GetParameter(1);      
       double Ptmean_AbsoResoBin4  =  hWp_AbsoResolutionPt_bin4 -> GetFunction("gaus") -> GetParameter(1);       
       double Ptmean_AbsoResoBin5  =  hWp_AbsoResolutionPt_bin5 -> GetFunction("gaus") -> GetParameter(1);       
       double Ptmean_AbsoResoBin6  =  hWp_AbsoResolutionPt_bin6 -> GetFunction("gaus") -> GetParameter(1);       
       double Ptmean_AbsoResoBin7  =  hWp_AbsoResolutionPt_bin7 -> GetFunction("gaus") -> GetParameter(1);     
       double Ptsigma_AbsoResoBin1 =  hWp_AbsoResolutionPt_bin1 -> GetFunction("gaus") -> GetParameter(2);   
       double Ptsigma_AbsoResoBin2 =  hWp_AbsoResolutionPt_bin2 -> GetFunction("gaus") -> GetParameter(2);     
       double Ptsigma_AbsoResoBin3 =  hWp_AbsoResolutionPt_bin3 -> GetFunction("gaus") -> GetParameter(2);     
       double Ptsigma_AbsoResoBin4 =  hWp_AbsoResolutionPt_bin4 -> GetFunction("gaus") -> GetParameter(2);       
       double Ptsigma_AbsoResoBin5 =  hWp_AbsoResolutionPt_bin5 -> GetFunction("gaus") -> GetParameter(2);       
       double Ptsigma_AbsoResoBin6 =  hWp_AbsoResolutionPt_bin6 -> GetFunction("gaus") -> GetParameter(2);     
       double Ptsigma_AbsoResoBin7 =  hWp_AbsoResolutionPt_bin7 -> GetFunction("gaus") -> GetParameter(2);      
       cout << "Pt mean_AbsoResoBin1 = " << Ptmean_AbsoResoBin1 << endl;   
       cout << "Pt mean_AbsoResoBin2 = " << Ptmean_AbsoResoBin2 << endl;   
       cout << "Pt mean_AbsoResoBin3 = " << Ptmean_AbsoResoBin3 << endl;    
       cout << "Pt mean_AbsoResoBin4 = " << Ptmean_AbsoResoBin4 << endl;     
       cout << "Pt mean_AbsoResoBin5 = " << Ptmean_AbsoResoBin5 << endl;     
       cout << "Pt mean_AbsoResoBin6 = " << Ptmean_AbsoResoBin6 << endl;     
       cout << "Pt mean_AbsoResoBin7 = " << Ptmean_AbsoResoBin7 << endl;     
       cout << "Pt sigma_AbsoResoBin1 = " << Ptsigma_AbsoResoBin1 << endl;   
       cout << "Pt sigma_AbsoResoBin2 = " << Ptsigma_AbsoResoBin2 << endl;   
       cout << "Pt sigma_AbsoResoBin3 = " << Ptsigma_AbsoResoBin3 << endl;   
       cout << "Pt sigma_AbsoResoBin4 = " << Ptsigma_AbsoResoBin4 << endl;   
       cout << "Pt sigma_AbsoResoBin5 = " << Ptsigma_AbsoResoBin5 << endl;   
       cout << "Pt sigma_AbsoResoBin6 = " << Ptsigma_AbsoResoBin6 << endl;   
       cout << "Pt sigma_AbsoResoBin7 = " << Ptsigma_AbsoResoBin7 << endl;

   //***********************

  cAbsoResoPt -> SaveAs(outPath + "/hWp_AbsoResolutionPt.png");
  cAbsoResoPt -> SaveAs(outPath + "/hWp_AbsoResolutionPt.eps");



   // PT RESOLUTION BEFORE CORRECTION

   TCanvas *cResoPt  = new TCanvas("cResoPt", "Resolution (before correction) in W-Pt", 700, 700);
   
   cResoPt -> Divide(2,4);

   //TF1 *fGaus = new TF1("fGaus", "gaus", -2, 2);
   cResoPt_1 -> cd();
   hWp_ResolutionPt_bin1 -> Draw();
   hWp_ResolutionPt_bin1 -> Fit("gaus", "","", -0.5, 0.8);
   hWp_ResolutionPt_bin1 -> GetFunction("gaus") -> Draw("same");
   cResoPt_2 -> cd();
   hWp_ResolutionPt_bin2 -> Fit("gaus", "","", -0.5, 0.8);
   hWp_ResolutionPt_bin2 -> Draw();
   hWp_ResolutionPt_bin2 -> GetFunction("gaus") -> Draw("same");
   cResoPt_3 -> cd();
   hWp_ResolutionPt_bin3 -> Fit("gaus", "","", -0.8, 0.7);
   hWp_ResolutionPt_bin3 -> Draw();
   hWp_ResolutionPt_bin3 -> GetFunction("gaus") -> Draw("same");
   cResoPt_4 -> cd();
   hWp_ResolutionPt_bin4 -> Fit("gaus", "","", -0.8, 0.6);
   hWp_ResolutionPt_bin4 -> Draw();
   hWp_ResolutionPt_bin4 -> GetFunction("gaus") -> Draw("same");
   cResoPt_5 -> cd();
   hWp_ResolutionPt_bin5 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionPt_bin5 -> Draw();
   hWp_ResolutionPt_bin5 -> GetFunction("gaus") -> Draw("same");
   cResoPt_6 -> cd();
   hWp_ResolutionPt_bin6 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionPt_bin6 -> Draw();
   hWp_ResolutionPt_bin6 -> GetFunction("gaus") -> Draw("same");
   cResoPt_7 -> cd();
   hWp_ResolutionPt_bin7 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionPt_bin7 -> Draw();
   hWp_ResolutionPt_bin7 -> GetFunction("gaus") -> Draw("same");
  


   //***********************
       //TF1 *f1g = hWp_An_evol_ZK_Vs_PtGen_pjy1->GetFunction("gaus");

       double Ptmean_ResoBin1  =  hWp_ResolutionPt_bin1 -> GetFunction("gaus") -> GetParameter(1);   
       double Ptmean_ResoBin2  =  hWp_ResolutionPt_bin2 -> GetFunction("gaus") -> GetParameter(1);     
       double Ptmean_ResoBin3  =  hWp_ResolutionPt_bin3 -> GetFunction("gaus") -> GetParameter(1);      
       double Ptmean_ResoBin4  =  hWp_ResolutionPt_bin4 -> GetFunction("gaus") -> GetParameter(1);       
       double Ptmean_ResoBin5  =  hWp_ResolutionPt_bin5 -> GetFunction("gaus") -> GetParameter(1);       
       double Ptmean_ResoBin6  =  hWp_ResolutionPt_bin6 -> GetFunction("gaus") -> GetParameter(1);       
       double Ptmean_ResoBin7  =  hWp_ResolutionPt_bin7 -> GetFunction("gaus") -> GetParameter(1);     
       double Ptsigma_ResoBin1 =  hWp_ResolutionPt_bin1 -> GetFunction("gaus") -> GetParameter(2);   
       double Ptsigma_ResoBin2 =  hWp_ResolutionPt_bin2 -> GetFunction("gaus") -> GetParameter(2);     
       double Ptsigma_ResoBin3 =  hWp_ResolutionPt_bin3 -> GetFunction("gaus") -> GetParameter(2);     
       double Ptsigma_ResoBin4 =  hWp_ResolutionPt_bin4 -> GetFunction("gaus") -> GetParameter(2);       
       double Ptsigma_ResoBin5 =  hWp_ResolutionPt_bin5 -> GetFunction("gaus") -> GetParameter(2);       
       double Ptsigma_ResoBin6 =  hWp_ResolutionPt_bin6 -> GetFunction("gaus") -> GetParameter(2);     
       double Ptsigma_ResoBin7 =  hWp_ResolutionPt_bin7 -> GetFunction("gaus") -> GetParameter(2);      
       cout << "Pt mean_ResoBin1 = " << Ptmean_ResoBin1 << endl;   
       cout << "Pt mean_ResoBin2 = " << Ptmean_ResoBin2 << endl;   
       cout << "Pt mean_ResoBin3 = " << Ptmean_ResoBin3 << endl;    
       cout << "Pt mean_ResoBin4 = " << Ptmean_ResoBin4 << endl;     
       cout << "Pt mean_ResoBin5 = " << Ptmean_ResoBin5 << endl;     
       cout << "Pt mean_ResoBin6 = " << Ptmean_ResoBin6 << endl;     
       cout << "Pt mean_ResoBin7 = " << Ptmean_ResoBin7 << endl;     
       cout << "Pt sigma_ResoBin1 = " << Ptsigma_ResoBin1 << endl;   
       cout << "Pt sigma_ResoBin2 = " << Ptsigma_ResoBin2 << endl;   
       cout << "Pt sigma_ResoBin3 = " << Ptsigma_ResoBin3 << endl;   
       cout << "Pt sigma_ResoBin4 = " << Ptsigma_ResoBin4 << endl;   
       cout << "Pt sigma_ResoBin5 = " << Ptsigma_ResoBin5 << endl;   
       cout << "Pt sigma_ResoBin6 = " << Ptsigma_ResoBin6 << endl;   
       cout << "Pt sigma_ResoBin7 = " << Ptsigma_ResoBin7 << endl;

   //***********************

  cResoPt -> SaveAs(outPath + "/hWp_ResolutionPt.png");
  cResoPt -> SaveAs(outPath + "/hWp_ResolutionPt.eps");


   TH1D *hResolutionPt = new TH1D("hResolutionPt","",7,-0.6,0.6);
       hResolutionPt   -> SetBinContent(1, Ptmean_ResoBin1);
       hResolutionPt   -> SetBinContent(2, Ptmean_ResoBin2);
       hResolutionPt   -> SetBinContent(3, Ptmean_ResoBin3);
       hResolutionPt   -> SetBinContent(4, Ptmean_ResoBin4);
       hResolutionPt   -> SetBinContent(5, Ptmean_ResoBin5);
       hResolutionPt   -> SetBinContent(6, Ptmean_ResoBin6);
       hResolutionPt   -> SetBinContent(7, Ptmean_ResoBin7);
       hResolutionPt   -> SetBinError(1, Ptsigma_ResoBin1);
       hResolutionPt   -> SetBinError(2, Ptsigma_ResoBin2);
       hResolutionPt   -> SetBinError(3, Ptsigma_ResoBin3);
       hResolutionPt   -> SetBinError(4, Ptsigma_ResoBin4);
       hResolutionPt   -> SetBinError(5, Ptsigma_ResoBin5);
       hResolutionPt   -> SetBinError(6, Ptsigma_ResoBin6);
       hResolutionPt   -> SetBinError(7, Ptsigma_ResoBin7);

   TLine *line0 = new TLine(hResolutionPt -> GetXaxis() -> GetXmin(), 0, hResolutionPt -> GetXaxis() -> GetXmax(), 0);
   line0 -> SetLineWidth(2);
   line0 -> SetLineStyle(2);

   TCanvas *cResoPt_summary  = new TCanvas("cResoPt_summary", "Resolution in W-Pt", 700, 700);
   cResoPt_summary -> cd();

   hResolutionPt   -> SetMaximum(1.);
   hResolutionPt   -> SetMinimum(-1.);
   hResolutionPt   -> Draw("e");
   line0 -> Draw();

   cResoPt_summary -> SaveAs(outPath + "/hResoPt_summary.png");
   cResoPt_summary -> SaveAs(outPath + "/hResoPt_summary.eps");



   // PT RESOLUTION AFTER CORRECTION

   TCanvas *cResoPtCorr  = new TCanvas("cResoPtCorr", "Resolution (after correction) in W-Pt", 700, 700);
   
   cResoPtCorr -> Divide(2,4);

   //TF1 *fGaus = new TF1("fGaus", "gaus", -2, 2);
   cResoPtCorr_1 -> cd();
   hWp_ResolutionPtCorr_bin1 -> Draw();
   hWp_ResolutionPtCorr_bin1 -> Fit("gaus", "","", -2., 0.6);
   hWp_ResolutionPtCorr_bin1 -> GetFunction("gaus") -> Draw("same");
   cResoPtCorr_2 -> cd();
   hWp_ResolutionPtCorr_bin2 -> Fit("gaus", "","", -1.5, 0.6);
   hWp_ResolutionPtCorr_bin2 -> Draw();
   hWp_ResolutionPtCorr_bin2 -> GetFunction("gaus") -> Draw("same");
   cResoPtCorr_3 -> cd();
   hWp_ResolutionPtCorr_bin3 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionPtCorr_bin3 -> Draw();
   hWp_ResolutionPtCorr_bin3 -> GetFunction("gaus") -> Draw("same");
   cResoPtCorr_4 -> cd();
   hWp_ResolutionPtCorr_bin4 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionPtCorr_bin4 -> Draw();
   hWp_ResolutionPtCorr_bin4 -> GetFunction("gaus") -> Draw("same");
   cResoPtCorr_5 -> cd();
   hWp_ResolutionPtCorr_bin5 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionPtCorr_bin5 -> Draw();
   hWp_ResolutionPtCorr_bin5 -> GetFunction("gaus") -> Draw("same");
   cResoPtCorr_6 -> cd();
   hWp_ResolutionPtCorr_bin6 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionPtCorr_bin6 -> Draw();
   hWp_ResolutionPtCorr_bin6 -> GetFunction("gaus") -> Draw("same");
   cResoPtCorr_7 -> cd();
   hWp_ResolutionPtCorr_bin7 -> Fit("gaus", "","", -1., 0.6);
   hWp_ResolutionPtCorr_bin7 -> Draw();
   hWp_ResolutionPtCorr_bin7 -> GetFunction("gaus") -> Draw("same");
  


   //***********************
       //TF1 *f1g = hWp_An_evol_ZK_Vs_PtCorrGen_pjy1->GetFunction("gaus");

       double PtCorrmean_ResoBin1  =  hWp_ResolutionPtCorr_bin1 -> GetFunction("gaus") -> GetParameter(1);   
       double PtCorrmean_ResoBin2  =  hWp_ResolutionPtCorr_bin2 -> GetFunction("gaus") -> GetParameter(1);     
       double PtCorrmean_ResoBin3  =  hWp_ResolutionPtCorr_bin3 -> GetFunction("gaus") -> GetParameter(1);      
       double PtCorrmean_ResoBin4  =  hWp_ResolutionPtCorr_bin4 -> GetFunction("gaus") -> GetParameter(1);       
       double PtCorrmean_ResoBin5  =  hWp_ResolutionPtCorr_bin5 -> GetFunction("gaus") -> GetParameter(1);       
       double PtCorrmean_ResoBin6  =  hWp_ResolutionPtCorr_bin6 -> GetFunction("gaus") -> GetParameter(1);       
       double PtCorrmean_ResoBin7  =  hWp_ResolutionPtCorr_bin7 -> GetFunction("gaus") -> GetParameter(1);     
       double PtCorrsigma_ResoBin1 =  hWp_ResolutionPtCorr_bin1 -> GetFunction("gaus") -> GetParameter(2);   
       double PtCorrsigma_ResoBin2 =  hWp_ResolutionPtCorr_bin2 -> GetFunction("gaus") -> GetParameter(2);     
       double PtCorrsigma_ResoBin3 =  hWp_ResolutionPtCorr_bin3 -> GetFunction("gaus") -> GetParameter(2);     
       double PtCorrsigma_ResoBin4 =  hWp_ResolutionPtCorr_bin4 -> GetFunction("gaus") -> GetParameter(2);       
       double PtCorrsigma_ResoBin5 =  hWp_ResolutionPtCorr_bin5 -> GetFunction("gaus") -> GetParameter(2);       
       double PtCorrsigma_ResoBin6 =  hWp_ResolutionPtCorr_bin6 -> GetFunction("gaus") -> GetParameter(2);     
       double PtCorrsigma_ResoBin7 =  hWp_ResolutionPtCorr_bin7 -> GetFunction("gaus") -> GetParameter(2);      
       cout << "PtCorr mean_ResoBin1 = " << PtCorrmean_ResoBin1 << endl;   
       cout << "PtCorr mean_ResoBin2 = " << PtCorrmean_ResoBin2 << endl;   
       cout << "PtCorr mean_ResoBin3 = " << PtCorrmean_ResoBin3 << endl;    
       cout << "PtCorr mean_ResoBin4 = " << PtCorrmean_ResoBin4 << endl;     
       cout << "PtCorr mean_ResoBin5 = " << PtCorrmean_ResoBin5 << endl;     
       cout << "PtCorr mean_ResoBin6 = " << PtCorrmean_ResoBin6 << endl;     
       cout << "PtCorr mean_ResoBin7 = " << PtCorrmean_ResoBin7 << endl;     
       cout << "PtCorr sigma_ResoBin1 = " << PtCorrsigma_ResoBin1 << endl;   
       cout << "PtCorr sigma_ResoBin2 = " << PtCorrsigma_ResoBin2 << endl;   
       cout << "PtCorr sigma_ResoBin3 = " << PtCorrsigma_ResoBin3 << endl;   
       cout << "PtCorr sigma_ResoBin4 = " << PtCorrsigma_ResoBin4 << endl;   
       cout << "PtCorr sigma_ResoBin5 = " << PtCorrsigma_ResoBin5 << endl;   
       cout << "PtCorr sigma_ResoBin6 = " << PtCorrsigma_ResoBin6 << endl;   
       cout << "PtCorr sigma_ResoBin7 = " << PtCorrsigma_ResoBin7 << endl;

   //***********************

   TH1D *hResolutionPtCorr = new TH1D("hResolutionPtCorr","",7,-0.6,0.6);
       hResolutionPtCorr   -> SetBinContent(1, PtCorrmean_ResoBin1);
       hResolutionPtCorr   -> SetBinContent(2, PtCorrmean_ResoBin2);
       hResolutionPtCorr   -> SetBinContent(3, PtCorrmean_ResoBin3);
       hResolutionPtCorr   -> SetBinContent(4, PtCorrmean_ResoBin4);
       hResolutionPtCorr   -> SetBinContent(5, PtCorrmean_ResoBin5);
       hResolutionPtCorr   -> SetBinContent(6, PtCorrmean_ResoBin6);
       hResolutionPtCorr   -> SetBinContent(7, PtCorrmean_ResoBin7);
       hResolutionPtCorr   -> SetBinError(1, PtCorrsigma_ResoBin1);
       hResolutionPtCorr   -> SetBinError(2, PtCorrsigma_ResoBin2);
       hResolutionPtCorr   -> SetBinError(3, PtCorrsigma_ResoBin3);
       hResolutionPtCorr   -> SetBinError(4, PtCorrsigma_ResoBin4);
       hResolutionPtCorr   -> SetBinError(5, PtCorrsigma_ResoBin5);
       hResolutionPtCorr   -> SetBinError(6, PtCorrsigma_ResoBin6);
       hResolutionPtCorr   -> SetBinError(7, PtCorrsigma_ResoBin7);

   TLine *line0 = new TLine(hResolutionPtCorr -> GetXaxis() -> GetXmin(), 0, hResolutionPtCorr -> GetXaxis() -> GetXmax(), 0);
   line0 -> SetLineWidth(2);
   line0 -> SetLineStyle(2);

   TCanvas *cResoPtCorr_summary  = new TCanvas("cResoPtCorr_summary", "Resolution (after correction) in W-Pt", 700, 700);
   cResoPtCorr_summary -> cd();

   hResolutionPtCorr   -> Draw("e");
   line0 -> Draw();

}
