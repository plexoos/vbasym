#include <vector>

void Isolation_plots()
{

   gStyle -> SetPadBottomMargin(0.15);
   gStyle -> SetPadLeftMargin(0.17);
   gStyle -> SetMarkerStyle(20);
   gStyle -> SetOptDate(0);
   gStyle -> SetErrorX();
   gStyle -> SetEndErrorSize(4);
   gStyle -> SetHistLineWidth(2);
   gStyle -> SetOptStat("emr");
   gStyle -> SetStatX(0.4);
   gStyle -> SetStatY(0.9);

   //TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString inPath_TSSA   = "/star/institutions/bnl_me/fazio/WeakBosonTSSA_run11/runlists/";
   //TString inPathScratch = "/star/data05/scratch/fazio/stana_out/runlists/";
   //TString outPath       = "~/vbasym_results/plots/postPrelim";
   TString outPath       = "~/plots4GPC/";

   TFile *fileData             = TFile::Open(inPath_TSSA + "run11_pp_transverse/hist/vbana.root");
   TFile *fileMCWp             = TFile::Open(inPath_TSSA + "run11_mc_Wp2enu.lis/hist/vbana.root");
   TFile *fileData_Iso02       = TFile::Open(inPath_TSSA + "run11_pp_transverse/hist/vbana_Iso02.root");
   TFile *fileMCWp_Iso08       = TFile::Open(inPath_TSSA + "run11_mc_Wp2enu.lis/hist/vbana_Iso08.root");
   TFile *fileData_NoIso       = TFile::Open(inPath_TSSA + "run11_pp_transverse/hist/vbana_NoIso.root");
   TFile *fileMCWp_NoIso       = TFile::Open(inPath_TSSA + "run11_mc_Wp2enu.lis/hist/vbana_NoIso.root");

   // take the histograms

   TH1 *hd_W_TrackClusterEnergyFrac       = (TH1*) fileData_NoIso -> Get("tracks.cut_w/hTrackClusterEnergyFrac");
   hd_W_TrackClusterEnergyFrac            -> SetTitleOffset(1.5, "Y");
   hd_W_TrackClusterEnergyFrac            -> SetTitle(";Energy Cone Isolation");
   TH1 *hd_W_TrackClusterETFrac           = (TH1*) fileData_NoIso -> Get("tracks.cut_w/hTrackClusterETFrac");
   hd_W_TrackClusterETFrac                -> SetTitleOffset(1.5, "Y");
   hd_W_TrackClusterETFrac                -> SetTitle(";E_{T} Cone Isolation");
   TH1 *hd_W_TrackClusterEnergyFrac_noEleP       = (TH1*) fileData_NoIso -> Get("tracks.cut_w/hTrackClusterEnergyFrac_noEleP");
   hd_W_TrackClusterEnergyFrac_noEleP            -> SetTitleOffset(1.5, "Y");
   hd_W_TrackClusterEnergyFrac_noEleP            -> SetTitle(";Energy Cone Isolation");
   TH1 *hd_W_TrackBClusterEnergyIsoRatio  = (TH1*) fileData_NoIso -> Get("tracks.cut_w/hTrackBClusterEnergyIsoRatio");
   hd_W_TrackBClusterEnergyIsoRatio       -> SetTitleOffset(1.5, "Y");
   hd_W_TrackBClusterEnergyIsoRatio       -> SetTitle(";E_{T}^{2x2}/E_{T}^{4x4}");
   TH1 *hd_W_TrackBClusterEnergyIsoRatio_allCuts  = (TH1*) fileData -> Get("tracks.cut_w/hTrackBClusterEnergyIsoRatio");
   hd_W_TrackBClusterEnergyIsoRatio_allCuts       -> SetTitleOffset(1.5, "Y");
   hd_W_TrackBClusterEnergyIsoRatio_allCuts       -> SetTitle(";E_{T}^{2x2}/E_{T}^{4x4}");

   TH1 *hWp_W_TrackClusterEnergyFrac      = (TH1*) fileMCWp_NoIso -> Get("tracks.cut_w/hTrackClusterEnergyFrac");
   hWp_W_TrackClusterEnergyFrac           -> SetTitleOffset(1.5, "Y");
   hWp_W_TrackClusterEnergyFrac           -> SetTitle(";Energy Cone Isolation");
   TH1 *hWp_W_TrackClusterEnergyFrac_noEleP      = (TH1*) fileMCWp_NoIso -> Get("tracks.cut_w/hTrackClusterEnergyFrac_noEleP");
   hWp_W_TrackClusterEnergyFrac_noEleP           -> SetTitleOffset(1.5, "Y");
   hWp_W_TrackClusterEnergyFrac_noEleP           -> SetTitle(";Energy Cone Isolation");
   TH1 *hWp_W_TrackClusterETFrac          = (TH1*) fileMCWp_NoIso -> Get("tracks.cut_w/hTrackClusterETFrac");
   hWp_W_TrackClusterETFrac               -> SetTitleOffset(1.5, "Y");
   hWp_W_TrackClusterETFrac               -> SetTitle(";E_{T} Cone Isolation");
   TH1 *hWp_W_TrackBClusterEnergyIsoRatio = (TH1*) fileMCWp_NoIso -> Get("tracks.cut_w/hTrackBClusterEnergyIsoRatio");
   hWp_W_TrackBClusterEnergyIsoRatio      -> SetTitleOffset(1.5, "Y");
   hWp_W_TrackBClusterEnergyIsoRatio      -> SetTitle(";E_{T}^{2x2}/E_{T}^{4x4}");
   TH1 *hWp_W_TrackBClusterEnergyIsoRatio_allCuts = (TH1*) fileMCWp -> Get("tracks.cut_w/hTrackBClusterEnergyIsoRatio");
   hWp_W_TrackBClusterEnergyIsoRatio_allCuts      -> SetTitleOffset(1.5, "Y");
   hWp_W_TrackBClusterEnergyIsoRatio_allCuts      -> SetTitle(";E_{T}^{2x2}/E_{T}^{4x4}");


  Double_t hd_W_TrackClusterEnergyFrac_yMax              = hd_W_TrackClusterEnergyFrac  -> GetMaximum();
  Double_t hWp_W_TrackClusterEnergyFrac_yMax             = hWp_W_TrackClusterEnergyFrac -> GetMaximum();
  Double_t hWp_W_TrackClusterEnergyFrac_ScaleFactor      = hd_W_TrackClusterEnergyFrac_yMax / hWp_W_TrackClusterEnergyFrac_yMax ;
  //cout << "hWp_W_TrackClusterEnergyFrac_ScaleFactor =  " << hWp_W_TrackClusterEnergyFrac_ScaleFactor << endl;
  hWp_W_TrackClusterEnergyFrac -> Scale(hWp_W_TrackClusterEnergyFrac_ScaleFactor);
  hWp_W_TrackClusterEnergyFrac -> SetFillColor(kRed); 
  Double_t hd_W_TrackClusterEnergyFrac_noEleP_yMax              = hd_W_TrackClusterEnergyFrac_noEleP  -> GetMaximum();
  Double_t hWp_W_TrackClusterEnergyFrac_noEleP_yMax             = hWp_W_TrackClusterEnergyFrac_noEleP -> GetMaximum();
  Double_t hWp_W_TrackClusterEnergyFrac_noEleP_ScaleFactor      = hd_W_TrackClusterEnergyFrac_noEleP_yMax / hWp_W_TrackClusterEnergyFrac_noEleP_yMax ;
  hWp_W_TrackClusterEnergyFrac_noEleP -> Scale(hWp_W_TrackClusterEnergyFrac_noEleP_ScaleFactor);
  hWp_W_TrackClusterEnergyFrac_noEleP -> SetFillColor(kRed); 
  Double_t hd_W_TrackClusterETFrac_yMax                  = hd_W_TrackClusterETFrac  -> GetMaximum();
  Double_t hWp_W_TrackClusterETFrac_yMax                 = hWp_W_TrackClusterETFrac -> GetMaximum();
  Double_t hWp_W_TrackClusterETFrac_ScaleFactor          = hd_W_TrackClusterETFrac_yMax / hWp_W_TrackClusterETFrac_yMax ;
  hWp_W_TrackClusterETFrac -> Scale(hWp_W_TrackClusterETFrac_ScaleFactor);
  hWp_W_TrackClusterETFrac -> SetFillColor(kRed); 
  Double_t hd_W_TrackBClusterEnergyIsoRatio_yMax         = hd_W_TrackBClusterEnergyIsoRatio  -> GetMaximum();
  Double_t hWp_W_TrackBClusterEnergyIsoRatio_yMax        = hWp_W_TrackBClusterEnergyIsoRatio -> GetMaximum();
  Double_t hWp_W_TrackBClusterEnergyIsoRatio_ScaleFactor = hd_W_TrackBClusterEnergyIsoRatio_yMax / hWp_W_TrackBClusterEnergyIsoRatio_yMax ;
  hWp_W_TrackBClusterEnergyIsoRatio -> Scale(hWp_W_TrackBClusterEnergyIsoRatio_ScaleFactor);
  hWp_W_TrackBClusterEnergyIsoRatio -> SetFillColor(kRed); 
  Double_t hd_W_TrackBClusterEnergyIsoRatio_allCuts_yMax         = hd_W_TrackBClusterEnergyIsoRatio_allCuts  -> GetMaximum();
  Double_t hWp_W_TrackBClusterEnergyIsoRatio_allCuts_yMax        = hWp_W_TrackBClusterEnergyIsoRatio_allCuts -> GetMaximum();
  Double_t hWp_W_TrackBClusterEnergyIsoRatio_allCuts_ScaleFactor = hd_W_TrackBClusterEnergyIsoRatio_allCuts_yMax / hWp_W_TrackBClusterEnergyIsoRatio_allCuts_yMax ;
  hWp_W_TrackBClusterEnergyIsoRatio_allCuts -> Scale(hWp_W_TrackBClusterEnergyIsoRatio_allCuts_ScaleFactor);
  hWp_W_TrackBClusterEnergyIsoRatio_allCuts -> SetFillColor(kRed);

  TLine *lineJ = new TLine(0.88, 0, 0.88, hd_W_TrackClusterEnergyFrac_yMax);
  lineJ -> SetLineColor(kBlue);
  lineJ -> SetLineWidth(3);
  lineJ -> SetLineStyle(7);
  TLine *lineS = new TLine(0.9, 0, 0.9, hd_W_TrackClusterEnergyFrac_yMax);
  lineS -> SetLineColor(kGreen);
  lineS -> SetLineWidth(3);
  lineS -> SetLineStyle(7);


  // do the plots: 

  TCanvas *cEneFrac   = new TCanvas("cEneFrac", "ISOLATION - Energy Fraction", 600, 600);

  //cEneFrac -> Divide(1,2);
  /*
  Int_t    binMax = hd_W_TrackClusterEnergyFrac -> GetMaximumBin(); 
  Double_t binMaxVal = hd_W_TrackClusterEnergyFrac -> GetBinContent(binMax);
  cout << "binMax  =  " << binMax << endl; 
  cout << "binMaxVal  =  " << binMaxVal << endl; 
  */
  TLegend *leg_cEneFrac = new TLegend(0.2, 0.3, 0.6, 0.6);
  leg_cEneFrac -> SetFillColor(0);
  leg_cEneFrac -> SetFillStyle(0);
  leg_cEneFrac -> SetLineColor(0);
  leg_cEneFrac -> SetBorderSize(0);
  leg_cEneFrac -> AddEntry(hd_W_TrackClusterEnergyFrac, "Data", "F");
  leg_cEneFrac -> AddEntry(hWp_W_TrackClusterEnergyFrac, "MC", "F");
  //leg_cEneFrac -> AddEntry(lineJ, "Justin's cut", "L");
  leg_cEneFrac -> AddEntry(lineS, "Sal's cut", "L");

  cEneFrac -> cd();
  gPad -> SetGrid(0,0);
  gPad -> Modified();
  hd_W_TrackClusterEnergyFrac  -> Draw();
  hWp_W_TrackClusterEnergyFrac -> Draw("same");
  //lineJ -> Draw();
  lineS -> Draw();
  leg_cEneFrac -> Draw();
  /*
  cEneFrac_2 -> cd();
  gPad -> SetGrid(0,0);
  hWp_W_TrackClusterEnergyFrac -> Draw();
  lineJ -> DrawLine(0.88, 0, 0.88, hWp_W_TrackClusterEnergyFrac_yMax);
  lineS -> DrawLine(0.9, 0, 0.9, hWp_W_TrackClusterEnergyFrac_yMax);
  */

  cEneFrac -> SaveAs(outPath + "W_TrackClusterEnergyFrac.png");
  cEneFrac -> SaveAs(outPath + "W_TrackClusterEnergyFrac.eps");


  TCanvas *cEneFrac_noEleP   = new TCanvas("cEneFrac_noEleP", "ISOLATION - Energy Fraction (no electron track)", 600, 600);

  TLegend *leg_cEneFrac_noEleP = new TLegend(0.2, 0.3, 0.6, 0.6);
  leg_cEneFrac_noEleP -> SetFillColor(0);
  leg_cEneFrac_noEleP -> SetFillStyle(0);
  leg_cEneFrac_noEleP -> SetLineColor(0);
  leg_cEneFrac_noEleP -> SetBorderSize(0);
  leg_cEneFrac_noEleP -> AddEntry(hd_W_TrackClusterEnergyFrac, "Data", "F");
  leg_cEneFrac_noEleP -> AddEntry(hWp_W_TrackClusterEnergyFrac, "MC", "F");
  leg_cEneFrac_noEleP -> AddEntry(lineJ, "Justin's cut", "L");
  //leg_cEneFrac_noEleP -> AddEntry(lineS, "Sal's cut", "L");

  cEneFrac_noEleP -> cd();
  gPad -> SetGrid(0,0);
  gPad -> Modified();
  hd_W_TrackClusterEnergyFrac_noEleP  -> Draw();
  hWp_W_TrackClusterEnergyFrac_noEleP -> Draw("same");
  lineJ -> DrawLine(0.88, 0, 0.88, hd_W_TrackClusterEnergyFrac_noEleP_yMax);
  //lineS -> Draw();
  leg_cEneFrac_noEleP -> Draw();

  cEneFrac_noEleP -> SaveAs(outPath + "W_TrackClusterEnergyFrac_noEleP.png");
  cEneFrac_noEleP -> SaveAs(outPath + "W_TrackClusterEnergyFrac_noEleP.eps");


  TCanvas *cETFrac   = new TCanvas("cETFrac", "ISOLATION - ET Fraction", 600, 600);

  TLegend *leg_cETFrac = new TLegend(0.2, 0.3, 0.6, 0.6);
  leg_cETFrac -> SetFillColor(0);
  leg_cETFrac -> SetFillStyle(0);
  leg_cETFrac -> SetLineColor(0);
  leg_cETFrac -> SetBorderSize(0);
  leg_cETFrac -> AddEntry(hd_W_TrackClusterETFrac, "Data", "F");
  leg_cETFrac -> AddEntry(hWp_W_TrackClusterETFrac, "MC", "F");
  //leg_cETFrac -> AddEntry(lineJ, "Justin's cut", "L");
  leg_cETFrac -> AddEntry(lineS, "Sal's cut", "L");

  cETFrac -> cd();
  gPad -> SetGrid(0,0);
  gPad -> Modified();
  hd_W_TrackClusterETFrac  -> Draw();
  hWp_W_TrackClusterETFrac -> Draw("same");
  //lineJ -> Draw();
  lineS -> Draw();
  leg_cETFrac -> Draw();

  cETFrac -> SaveAs(outPath + "W_TrackClusterETFrac.png");
  cETFrac -> SaveAs(outPath + "W_TrackClusterETFrac.eps");


  TCanvas *c2x2_4x4_ET   = new TCanvas("c2x2_4x4_ET", "ISOLATION - 2x2_ET/4x4_ET", 600, 600);

  TLegend *leg_c2x2_4x4_ET = new TLegend(0.2, 0.3, 0.6, 0.6);
  leg_c2x2_4x4_ET -> SetFillColor(0);
  leg_c2x2_4x4_ET -> SetFillStyle(0);
  leg_c2x2_4x4_ET -> SetLineColor(0);
  leg_c2x2_4x4_ET -> SetBorderSize(0);
  leg_c2x2_4x4_ET -> AddEntry(hd_W_TrackClusterETFrac, "Data", "F");
  leg_c2x2_4x4_ET -> AddEntry(hWp_W_TrackClusterETFrac, "MC", "F");
  leg_c2x2_4x4_ET -> AddEntry(lineJ, "Justin's cut", "L");

  c2x2_4x4_ET -> cd();
  gPad  -> SetGrid(0,0);
  gPad  -> Modified();
  hd_W_TrackBClusterEnergyIsoRatio  -> Draw();
  hWp_W_TrackBClusterEnergyIsoRatio -> Draw("same");
  lineJ -> DrawLine(0.95, 0, 0.95, hd_W_TrackBClusterEnergyIsoRatio_yMax);
  leg_cETFrac -> Draw();

  c2x2_4x4_ET -> SaveAs(outPath + "W_2x2over4x4_ET_IsoRatio.png");
  c2x2_4x4_ET -> SaveAs(outPath + "W_2x2over4x4_ET_IsoRatio.eps");


  TCanvas *c2x2_4x4_ET_allCuts   = new TCanvas("c2x2_4x4_ET_allCuts", "ISOLATION - 2x2_ET/4x4_ET - All cuts", 600, 600);

  c2x2_4x4_ET_allCuts -> cd();
  gPad  -> SetGrid(0,0);
  gPad  -> Modified();
  hd_W_TrackBClusterEnergyIsoRatio_allCuts  -> Draw();
  hWp_W_TrackBClusterEnergyIsoRatio_allCuts -> Draw("same");
  lineJ -> DrawLine(0.95, 0, 0.95, hd_W_TrackBClusterEnergyIsoRatio_allCuts_yMax);
  leg_cETFrac -> Draw();

  c2x2_4x4_ET_allCuts -> SaveAs(outPath + "W_2x2over4x4_ET_IsoRatio_allCuts.png");
  c2x2_4x4_ET_allCuts -> SaveAs(outPath + "W_2x2over4x4_ET_IsoRatio_allCuts.eps");
}
