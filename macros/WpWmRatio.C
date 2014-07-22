void WpWmRatio()
{
   TString inPath        = "/star/institutions/bnl_me/fazio/vbana_out/";
   TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString inPathScratch = "/star/data05/scratch/fazio/stana_out/runlists/";
   //TString inPathNew     = $VBASYM_RESULTS_DIR/;
   //TString outPath       = "~/vbasym_results/plots/4prelim";
   TString outPath       = "~/vbasym_results/plots/postPrelim/WpWmRatio";
   cout << "input path" << inPathNew << endl;

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

   // open histogram files
   TFile *fileData      = TFile::Open(inPathNew + "run11_pp_transverse/hist/vbana.root");

   // fill histograms
   TH1 *hd_Wp_WBosonPt                     = (TH1*) fileData->Get("event_wp/hWBosonPt");
   TH1 *hd_Wp_WBosonPt_zoomin              = (TH1*) fileData->Get("event_wp/hWBosonPt_zoomin");
   TH1 *hd_Wp_WBosonPz                     = (TH1*) fileData->Get("event_wp/hWBosonPz");
   TH1 *hd_Wp_WBosonEta                    = (TH1*) fileData->Get("event_wp/hWBosonEta");
   TH1 *hd_Wp_WBosonRapidity               = (TH1*) fileData->Get("event_wp/hWBosonRapidity");

   TH1 *hd_Wm_WBosonPt                     = (TH1*) fileData->Get("event_wm/hWBosonPt");
   TH1 *hd_Wm_WBosonPt_zoomin              = (TH1*) fileData->Get("event_wm/hWBosonPt_zoomin");
   TH1 *hd_Wm_WBosonPz                     = (TH1*) fileData->Get("event_wm/hWBosonPz");
   TH1 *hd_Wm_WBosonEta                    = (TH1*) fileData->Get("event_wm/hWBosonEta");
   TH1 *hd_Wm_WBosonRapidity               = (TH1*) fileData->Get("event_wm/hWBosonRapidity");

   // rebin
   hd_Wp_WBosonEta -> Rebin(3);
   hd_Wm_WBosonEta -> Rebin(3);

   // calculate the ratios
   //int bins = hd_Wp_WBosonEta -> GetBinX();
   //cout << "# bins = " << bins << endl;

   TH1D *hd_WpWmRatio         = (TH1D*) hd_Wp_WBosonEta->Clone("hd_WpWmRatio");
   hd_WpWmRatio  -> Divide(hd_Wp_WBosonEta, hd_Wm_WBosonEta);
   hd_WpWmRatio  -> SetMarkerStyle(20);
   hd_WpWmRatio  -> SetTitle("; #eta; W^{+}/W^{-}");

   // plots
   TCanvas *c0    = new TCanvas("c0", "W versus Eta", 800, 500);
   c0 -> Divide(2,1);
   c0_1 -> cd();
   hd_Wp_WBosonEta -> SetMarkerStyle(20);
   hd_Wp_WBosonEta -> SetTitle("W^{+}");
   hd_Wp_WBosonEta -> Draw("e");
   c0_2 -> cd();
   hd_Wm_WBosonEta -> SetTitle("W^{-}");
   hd_Wm_WBosonEta -> SetMarkerStyle(20);
   hd_Wm_WBosonEta -> Draw("e");

   c0->Print(outPath + "/plot_WEta.eps");
   c0->Print(outPath + "/plot_WEta.png");

   TCanvas *c1    = new TCanvas("c1", "W+/W- versus Eta", 800, 500);
   c1 -> cd();
   hd_WpWmRatio -> Draw("e");

   c1->Print(outPath + "/plot_WpWmRatio_vs_Eta.eps");
   c1->Print(outPath + "/plot_WpWmRatio_vs_Eta.png");
}
