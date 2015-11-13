void isolation_NewVsOld()
{
   std::cout.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed

   // Load the libraries:
   //gROOT->Macro("/star/u/fazio/offline/users/fazio/vbasym/macros/loadLibs.C");

   TString inPath        = "/star/institutions/bnl_me/fazio/vbana_out/";
   TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString inPath_TSSA   = "/star/institutions/bnl_me/fazio/WeakBosonTSSA_run11/runlists/";
   TString outPath       = "~/vbasym_results/plots/2011/";
   //TString outPath       = "~/vbasym_results/plots/4prelim";

   //Styles:
   gStyle->SetPadBottomMargin(0.12);
   gStyle->SetPadLeftMargin(0.1);
   gStyle->SetStatFontSize(0.05);
   gStyle->SetOptStat("e");
   gStyle->SetPadRightMargin(0.25);
   gStyle->SetMarkerStyle(20);
   gStyle->SetStatX(0.99);
   gStyle->SetOptFit(1);
   gStyle->SetOptDate(0);

   // open histogram files
   TFile *fileDataOldIso   = TFile::Open(inPath_TSSA + "run11_pp_transverse/hist/vbana_oldIsol_cut088_SL14a.root");
   TFile *fileDataNewIso   = TFile::Open(inPath_TSSA + "run11_pp_transverse/hist/vbana_newIsol_cut090_SL14a.root");


// W total sample (take histograms)
   TH1 *hd_W_OldIso_RunId        = (TH1 *)fileDataOldIso   -> Get("event_w/hRunId");
   TH1 *hd_W_NewIso_RunId        = (TH1 *)fileDataNewIso   -> Get("event_w/hRunId");


   // Do plots 

   TCanvas *c1 = new TCanvas("c1", "Run Id", 800, 800);
 
   c1 -> SetGrid(0,0);
   hd_W_NewIso_RunId -> SetMarkerColor(kRed);
   hd_W_NewIso_RunId -> SetMarkerStyle(20);
   hd_W_NewIso_RunId -> Draw("e");
   hd_W_OldIso_RunId -> SetMarkerColor(kBlue);
   hd_W_OldIso_RunId -> SetMarkerStyle(24);
   hd_W_OldIso_RunId -> Draw("e same");

   TLegend *leg_c1 = new TLegend(0.1, 0.75, 0.5, 0.9);
   leg_c1 -> SetFillColor(0);
   leg_c1 -> SetFillStyle(0);
   leg_c1 -> SetLineColor(0);
   leg_c1 -> SetBorderSize(0);
   leg_c1 -> AddEntry(hd_W_NewIso_RunId,"New Isolation", "p");
   leg_c1 -> AddEntry(hd_W_OldIso_RunId,"Old Isolation", "p");
   leg_c1 -> Draw();


   c1 -> Print(outPath + "/plot_Isolation_NewVsOld_runId.eps");
   c1 -> Print(outPath + "/plot_Isolation_NewVsOld_runId.png");


}
