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
   TFile *fileData12    = TFile::Open(inPathNew + "run12_pp_long_j3/hist/vbana.root");

   // fill histograms run 11 transverse
   TH1 *hd_Wp_WBosonPt                     = (TH1*) fileData->Get("event_wp/hWBosonPt");
   TH1 *hd_Wp_WBosonPt_zoomin              = (TH1*) fileData->Get("event_wp/hWBosonPt_zoomin");
   TH1 *hd_Wp_WBosonPz                     = (TH1*) fileData->Get("event_wp/hWBosonPz");
   TH1 *hd_Wp_WBosonEta                    = (TH1*) fileData->Get("event_wp/hWBosonEta");
   TH1 *hd_Wp_WBosonRapidity               = (TH1*) fileData->Get("event_wp/hWBosonRapidity");
   TH1 *hd_Wp_ElectronPt                   = (TH1*) fileData->Get("event_wp/hElectronPt");
   TH1 *hd_Wp_ElectronEta                  = (TH1*) fileData->Get("event_wp/hElectronEta");

   TH1 *hd_Wm_WBosonPt                     = (TH1*) fileData->Get("event_wm/hWBosonPt");
   TH1 *hd_Wm_WBosonPt_zoomin              = (TH1*) fileData->Get("event_wm/hWBosonPt_zoomin");
   TH1 *hd_Wm_WBosonPz                     = (TH1*) fileData->Get("event_wm/hWBosonPz");
   TH1 *hd_Wm_WBosonEta                    = (TH1*) fileData->Get("event_wm/hWBosonEta");
   TH1 *hd_Wm_WBosonRapidity               = (TH1*) fileData->Get("event_wm/hWBosonRapidity");
   TH1 *hd_Wm_ElectronPt                   = (TH1*) fileData->Get("event_wm/hElectronPt");
   TH1 *hd_Wm_ElectronEta                  = (TH1*) fileData->Get("event_wm/hElectronEta");

   // fill histograms run 12
   TH1 *hd12_Wp_WBosonPt                   = (TH1*) fileData12->Get("event_wp/hWBosonPt");
   TH1 *hd12_Wp_WBosonPt_zoomin            = (TH1*) fileData12->Get("event_wp/hWBosonPt_zoomin");
   TH1 *hd12_Wp_WBosonPz                   = (TH1*) fileData12->Get("event_wp/hWBosonPz");
   TH1 *hd12_Wp_WBosonEta                  = (TH1*) fileData12->Get("event_wp/hWBosonEta");
   TH1 *hd12_Wp_WBosonRapidity             = (TH1*) fileData12->Get("event_wp/hWBosonRapidity");
   TH1 *hd12_Wp_ElectronPt                 = (TH1*) fileData12->Get("event_wp/hElectronPt");
   TH1 *hd12_Wp_ElectronEta                = (TH1*) fileData12->Get("event_wp/hElectronEta");

   TH1 *hd12_Wm_WBosonPt                   = (TH1*) fileData12->Get("event_wm/hWBosonPt");
   TH1 *hd12_Wm_WBosonPt_zoomin            = (TH1*) fileData12->Get("event_wm/hWBosonPt_zoomin");
   TH1 *hd12_Wm_WBosonPz                   = (TH1*) fileData12->Get("event_wm/hWBosonPz");
   TH1 *hd12_Wm_WBosonEta                  = (TH1*) fileData12->Get("event_wm/hWBosonEta");
   TH1 *hd12_Wm_WBosonRapidity             = (TH1*) fileData12->Get("event_wm/hWBosonRapidity");
   TH1 *hd12_Wm_ElectronPt                 = (TH1*) fileData12->Get("event_wm/hElectronPt");
   TH1 *hd12_Wm_ElectronEta                = (TH1*) fileData12->Get("event_wm/hElectronEta");

   // rebin
   hd_Wp_WBosonEta     -> Rebin(4);
   hd_Wm_WBosonEta     -> Rebin(4);
   hd12_Wp_WBosonEta   -> Rebin(4);
   hd12_Wm_WBosonEta   -> Rebin(4);

   hd_Wp_ElectronEta   -> Rebin(2);
   hd_Wm_ElectronEta   -> Rebin(2);
   hd12_Wp_ElectronEta -> Rebin(2);
   hd12_Wm_ElectronEta -> Rebin(2);

   hd_Wp_WBosonRapidity     -> Rebin(2);
   hd_Wm_WBosonRapidity     -> Rebin(2);
   hd12_Wp_WBosonRapidity   -> Rebin(2);
   hd12_Wm_WBosonRapidity   -> Rebin(2);
   hd_Wp_WBosonRapidity -> GetXaxis() -> SetRange(3, 8);
   hd_Wm_WBosonRapidity -> GetXaxis() -> SetRange(3, 8);
   hd12_Wp_WBosonRapidity -> GetXaxis() -> SetRange(3, 8);
   hd12_Wm_WBosonRapidity -> GetXaxis() -> SetRange(3, 8);
   //hd_Wp_WBosonRapidity -> GetXaxis() -> SetRangeUser(-0.99, 0.99);
   //hd_Wm_WBosonRapidity -> GetXaxis() -> SetRangeUser(-0.99, 0.99);
   //hd12_Wp_WBosonRapidity -> GetXaxis() -> SetRangeUser(-0.99, 0.99);
   //hd12_Wm_WBosonRapidity -> GetXaxis() -> SetRangeUser(-0.99, 0.99);

   // calculate the ratios
   //int bins = hd_Wp_WBosonEta -> GetBinX();
   //cout << "# bins = " << bins << endl;
   TH1D *hd_WpWmRatio           = (TH1D*) hd_Wp_WBosonEta   -> Clone("hd_WpWmRatio");
   TH1D *hd_WpWmRatio_vs_EleEta = (TH1D*) hd_Wp_ElectronEta -> Clone("hd_WpWmRatio_vs_EleEta");
   //TH1D *hd_Wp_WBosonEta_bis  = (TH1D*) hd_Wp_WBosonEta->Clone("hd_Wp_WBosonEta_bis");
   hd_Wp_WBosonEta   -> Sumw2();
   hd_Wm_WBosonEta   -> Sumw2();
   hd_Wp_ElectronEta -> Sumw2();
   hd_Wm_ElectronEta -> Sumw2();
   //hd_Wp_WBosonEta_bis -> Divide(hd_Wm_WBosonEta);
   //hd_WpWmRatio  -> Sumw2();
   hd_WpWmRatio  -> Divide(hd_Wp_WBosonEta, hd_Wm_WBosonEta);
   hd_WpWmRatio  -> SetMaximum(6);
   hd_WpWmRatio  -> SetMinimum(0);
   hd_WpWmRatio  -> SetMarkerStyle(20);
   hd_WpWmRatio  -> SetTitle("run11; #eta_{W}; W^{+}/W^{-}");
   hd_WpWmRatio_vs_EleEta  -> Divide(hd_Wp_ElectronEta, hd_Wm_ElectronEta);
   hd_WpWmRatio_vs_EleEta  -> SetMaximum(6);
   hd_WpWmRatio_vs_EleEta  -> SetMinimum(0);
   hd_WpWmRatio_vs_EleEta  -> SetMarkerStyle(20);
   hd_WpWmRatio_vs_EleEta  -> SetTitle("run11; #eta_{el}; W^{+}/W^{-}");

   TH1D *hd12_WpWmRatio           = (TH1D*) hd12_Wp_WBosonEta   -> Clone("hd12_WpWmRatio");
   TH1D *hd12_WpWmRatio_vs_EleEta = (TH1D*) hd12_Wp_ElectronEta -> Clone("hd12_WpWmRatio_vs_EleEta");
   hd12_Wp_WBosonEta   -> Sumw2();
   hd12_Wm_WBosonEta   -> Sumw2();
   hd12_Wp_ElectronEta -> Sumw2();
   hd12_Wm_ElectronEta -> Sumw2();
   //hd12_WpWmRatio  -> Sumw2();
   hd12_WpWmRatio  -> Divide(hd12_Wp_WBosonEta, hd12_Wm_WBosonEta);
   hd12_WpWmRatio  -> SetMaximum(6);
   hd12_WpWmRatio  -> SetMinimum(0);
   hd12_WpWmRatio  -> SetMarkerStyle(20);
   hd12_WpWmRatio  -> SetTitle("run 12; #eta_{W}; W^{+}/W^{-}");
   hd12_WpWmRatio_vs_EleEta  -> Divide(hd12_Wp_ElectronEta, hd12_Wm_ElectronEta);
   hd12_WpWmRatio_vs_EleEta  -> SetMaximum(6);
   hd12_WpWmRatio_vs_EleEta  -> SetMinimum(0);
   hd12_WpWmRatio_vs_EleEta  -> SetMarkerStyle(20);
   hd12_WpWmRatio_vs_EleEta  -> SetTitle("run12; #eta_{el}; W^{+}/W^{-}");

   TH1D *hd1112_Wp_WBosonEta        = (TH1D*) hd12_Wp_WBosonEta   -> Clone("hd1112_Wp_WBosonEta");
   TH1D *hd1112_Wm_WBosonEta        = (TH1D*) hd12_Wm_WBosonEta   -> Clone("hd1112_Wm_WBosonEta");   
   TH1D *hd1112_Wp_ElectronEta      = (TH1D*) hd12_Wp_ElectronEta -> Clone("hd1112_Wp_ElectronEta");
   TH1D *hd1112_Wm_ElectronEta      = (TH1D*) hd12_Wm_ElectronEta -> Clone("hd1112_Wm_ElectronEta");      
   TH1D *hd1112_WpWmRatio           = (TH1D*) hd12_Wp_WBosonEta   -> Clone("hd1112_WpWmRatio");      
   TH1D *hd1112_WpWmRatio_vs_EleEta = (TH1D*) hd12_Wp_ElectronEta -> Clone("hd1112_WpWmRatio_vs_EleEta");
   hd1112_Wp_WBosonEta   -> Sumw2();
   hd1112_Wm_WBosonEta   -> Sumw2();
   hd1112_Wp_ElectronEta -> Sumw2();
   hd1112_Wm_ElectronEta -> Sumw2();
   hd1112_Wp_WBosonEta   -> Add(hd_Wp_WBosonEta, hd12_Wp_WBosonEta);
   hd1112_Wm_WBosonEta   -> Add(hd_Wm_WBosonEta, hd12_Wm_WBosonEta);
   hd1112_Wp_ElectronEta -> Add(hd_Wp_ElectronEta, hd12_Wp_ElectronEta);
   hd1112_Wm_ElectronEta -> Add(hd_Wm_ElectronEta, hd12_Wm_ElectronEta);
   //hd1112_WpWmRatio  -> Sumw2();
   hd1112_WpWmRatio  -> Divide(hd1112_Wp_WBosonEta, hd1112_Wm_WBosonEta);
   hd1112_WpWmRatio  -> SetMaximum(6);
   hd1112_WpWmRatio  -> SetMinimum(0);
   hd1112_WpWmRatio  -> SetMarkerStyle(20);
   hd1112_WpWmRatio  -> SetTitle("run 11+12; #eta_{W}; W^{+}/W^{-}");
   hd1112_WpWmRatio_vs_EleEta  -> Divide(hd1112_Wp_ElectronEta, hd1112_Wm_ElectronEta);
   hd1112_WpWmRatio_vs_EleEta  -> SetMaximum(6);
   hd1112_WpWmRatio_vs_EleEta  -> SetMinimum(0);
   hd1112_WpWmRatio_vs_EleEta  -> SetMarkerStyle(20);
   hd1112_WpWmRatio_vs_EleEta  -> SetTitle("run 11+12; #eta_{el}; W^{+}/W^{-}");




   TH1D *hd_WpWmRatio_vs_WBosonRapidity  = (TH1D*) hd_Wp_WBosonRapidity   -> Clone("hd_WpWmRatio_vs_WBosonRapidity");
   hd_Wp_WBosonRapidity   -> Sumw2();
   hd_Wm_WBosonRapidity   -> Sumw2();
   hd_WpWmRatio_vs_WBosonRapidity  -> Divide(hd_Wp_WBosonRapidity, hd_Wm_WBosonRapidity);
   hd_WpWmRatio_vs_WBosonRapidity  -> SetMaximum(6);
   hd_WpWmRatio_vs_WBosonRapidity  -> SetMinimum(0);
   hd_WpWmRatio_vs_WBosonRapidity  -> SetMarkerStyle(20);
   hd_WpWmRatio_vs_WBosonRapidity  -> SetTitle("run11; y_{W}; W^{+}/W^{-}");

   TH1D *hd12_WpWmRatio_vs_WBosonRapidity = (TH1D*) hd12_Wp_WBosonRapidity   -> Clone("hd12_WpWmRatio_vs_WBosonRapidity");
   hd12_Wp_WBosonRapidity   -> Sumw2();
   hd12_Wm_WBosonRapidity   -> Sumw2();
   hd12_WpWmRatio_vs_WBosonRapidity  -> Divide(hd12_Wp_WBosonRapidity, hd12_Wm_WBosonRapidity);
   hd12_WpWmRatio_vs_WBosonRapidity  -> SetMaximum(6);
   hd12_WpWmRatio_vs_WBosonRapidity  -> SetMinimum(0);
   hd12_WpWmRatio_vs_WBosonRapidity  -> SetMarkerStyle(20);
   hd12_WpWmRatio_vs_WBosonRapidity  -> SetTitle("run 12; y_{W}; W^{+}/W^{-}");

   TH1D *hd1112_Wp_WBosonRapidity        = (TH1D*) hd12_Wp_WBosonRapidity   -> Clone("hd1112_Wp_WBosonRapidity");
   TH1D *hd1112_Wm_WBosonRapidity        = (TH1D*) hd12_Wm_WBosonRapidity   -> Clone("hd1112_Wm_WBosonRapidity");      
   TH1D *hd1112_WpWmRatio_vs_WBosonRapidity = (TH1D*) hd12_Wp_WBosonRapidity   -> Clone("hd1112_WpWmRatio_vs_WBosonRapidity");
   hd1112_Wp_WBosonRapidity   -> Sumw2();
   hd1112_Wm_WBosonRapidity   -> Sumw2();
   hd1112_Wp_WBosonRapidity   -> Add(hd_Wp_WBosonRapidity, hd12_Wp_WBosonRapidity);
   hd1112_Wm_WBosonRapidity   -> Add(hd_Wm_WBosonRapidity, hd12_Wm_WBosonRapidity);
   hd1112_WpWmRatio_vs_WBosonRapidity  -> Divide(hd1112_Wp_WBosonRapidity, hd1112_Wm_WBosonRapidity);
   hd1112_WpWmRatio_vs_WBosonRapidity  -> SetMaximum(6);
   hd1112_WpWmRatio_vs_WBosonRapidity  -> SetMinimum(0);
   hd1112_WpWmRatio_vs_WBosonRapidity  -> SetMarkerStyle(20);
   hd1112_WpWmRatio_vs_WBosonRapidity  -> SetTitle("run 11+12; y_{W}; W^{+}/W^{-}");

   // plots
   TCanvas *c0    = new TCanvas("c0", "run11: W-Eta", 800, 500);
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

   TCanvas *c1    = new TCanvas("c1", "run 11: W+/W- versus Eta", 800, 500);
   c1 -> cd();
   hd_WpWmRatio -> Draw("e");

   c1->Print(outPath + "/plot_WpWmRatio_vs_Eta.eps");
   c1->Print(outPath + "/plot_WpWmRatio_vs_Eta.png");

   TCanvas *c0el    = new TCanvas("c0el", "run11: Electron-Eta", 800, 500);
   c0el -> Divide(2,1);
   c0el_1 -> cd();
   hd_Wp_ElectronEta -> SetMarkerStyle(20);
   hd_Wp_ElectronEta -> SetTitle("W^{+}");
   hd_Wp_ElectronEta -> Draw("e");
   c0el_2 -> cd();
   hd_Wm_ElectronEta -> SetTitle("W^{-}");
   hd_Wm_ElectronEta -> SetMarkerStyle(20);
   hd_Wm_ElectronEta -> Draw("e");

   c0el->Print(outPath + "/plot_ElecEta.eps");
   c0el->Print(outPath + "/plot_ElecEta.png");

   TCanvas *c1el    = new TCanvas("c1el", "run 11: W+/W- versus Elec-Eta", 800, 500);
   c1el -> cd();
   hd_WpWmRatio_vs_EleEta -> Draw("e");

   c1el->Print(outPath + "/plot_WpWmRatio_vs_ElecEta.eps");
   c1el->Print(outPath + "/plot_WpWmRatio_vs_ElecEta.png");

   TCanvas *c2    = new TCanvas("c2", "run12: W-Eta", 800, 500);
   c2 -> Divide(2,1);
   c2_1 -> cd();
   hd12_Wp_WBosonEta -> SetMarkerStyle(20);
   hd12_Wp_WBosonEta -> SetTitle("W^{+}");
   hd12_Wp_WBosonEta -> Draw("e");
   c2_2 -> cd();
   hd12_Wm_WBosonEta -> SetTitle("W^{-}");
   hd12_Wm_WBosonEta -> SetMarkerStyle(20);
   hd12_Wm_WBosonEta -> Draw("e");

   c2->Print(outPath + "/plot_r12_WEta.eps");
   c2->Print(outPath + "/plot_r12_WEta.png");

   TCanvas *c3    = new TCanvas("c3", "run 12: W+/W- versus Eta", 800, 500);
   c3 -> cd();
   hd12_WpWmRatio -> Draw("e");
   //hd_Wp_WBosonEta_bis ->Draw("e");

   c3->Print(outPath + "/plot_r12_WpWmRatio_vs_Eta.eps");
   c3->Print(outPath + "/plot_r12_WpWmRatio_vs_Eta.png");

   TCanvas *c2el    = new TCanvas("c2el", "run12: Electron-Eta", 800, 500);
   c2el -> Divide(2,1);
   c2el_1 -> cd();
   hd12_Wp_ElectronEta -> SetMarkerStyle(20);
   hd12_Wp_ElectronEta -> SetTitle("W^{+}");
   hd12_Wp_ElectronEta -> Draw("e");
   c2el_2 -> cd();
   hd12_Wm_ElectronEta -> SetTitle("W^{-}");
   hd12_Wm_ElectronEta -> SetMarkerStyle(20);
   hd12_Wm_ElectronEta -> Draw("e");

   c2el->Print(outPath + "/plot_r12_ElecEta.eps");
   c2el->Print(outPath + "/plot_r12_ElecEta.png");

   TCanvas *c3el    = new TCanvas("c3el", "run 12: W+/W- versus Elec-Eta", 800, 500);
   c3el -> cd();
   hd12_WpWmRatio_vs_EleEta -> Draw("e");

   c3el->Print(outPath + "/plot_r12_WpWmRatio_vs_ElecEta.eps");
   c3el->Print(outPath + "/plot_r12_WpWmRatio_vs_ElecEta.png");

   TCanvas *c4    = new TCanvas("c4", "run 11+12: W-Eta", 800, 500);
   c4 -> Divide(2,1);
   c4_1 -> cd();
   hd1112_Wp_WBosonEta -> SetMarkerStyle(20);
   hd1112_Wp_WBosonEta -> SetTitle("W^{+}");
   hd1112_Wp_WBosonEta -> Draw("e");
   c4_2 -> cd();
   hd1112_Wm_WBosonEta -> SetTitle("W^{-}");
   hd1112_Wm_WBosonEta -> SetMarkerStyle(20);
   hd1112_Wm_WBosonEta -> Draw("e");

   c4->Print(outPath + "/plot_r11r12_WEta.eps");
   c4->Print(outPath + "/plot_r11r12_WEta.png");

   TCanvas *c5    = new TCanvas("c5", "run 11+12: W+/W- versus Eta", 800, 500);
   c5 -> cd();
   hd1112_WpWmRatio -> Draw("e");

   c5->Print(outPath + "/plot_r11r12_WpWmRatio_vs_Eta.eps");
   c5->Print(outPath + "/plot_r11r12_WpWmRatio_vs_Eta.png");

   TCanvas *c5rap    = new TCanvas("c5rap", "run 11+12: W+/W- versus Rapidity", 800, 500);
   c5rap -> cd();
   hd1112_WpWmRatio_vs_WBosonRapidity -> Draw("e");

   c5rap->Print(outPath + "/plot_r11r12_WpWmRatio_vs_Rapidity.eps");
   c5rap->Print(outPath + "/plot_r11r12_WpWmRatio_vs_Rapidity.png");

   TCanvas *c4el    = new TCanvas("c4el", "run 11+12: Electron-Eta", 800, 500);
   c4el -> Divide(2,1);
   c4el_1 -> cd();
   hd1112_Wp_ElectronEta -> SetMarkerStyle(20);
   hd1112_Wp_ElectronEta -> SetTitle("W^{+}");
   hd1112_Wp_ElectronEta -> Draw("e");
   c4el_2 -> cd();
   hd1112_Wm_ElectronEta -> SetTitle("W^{-}");
   hd1112_Wm_ElectronEta -> SetMarkerStyle(20);
   hd1112_Wm_ElectronEta -> Draw("e");

   c4el->Print(outPath + "/plot_r11r12_ElecEta.eps");
   c4el->Print(outPath + "/plot_r11r12_ElecEta.png");

   TCanvas *c5el    = new TCanvas("c5el", "run 11+12: W+/W- versus Elec-Eta", 800, 500);
   c5el -> cd();
   hd1112_WpWmRatio_vs_EleEta -> Draw("e");

   c5el->Print(outPath + "/plot_r11r12_WpWmRatio_vs_ElecEta.eps");
   c5el->Print(outPath + "/plot_r11r12_WpWmRatio_vs_ElecEta.png");




   // We make now expected projections of uncertainties including run13 (~300 pb-1)
   // the projection factor (to be divided for) will be sqrt(3.7)
   Double_t projFactor = 1.92;

   TH1D *hd1112run13proj_Wp_WBosonEta        = (TH1D*) hd1112_Wp_WBosonEta        -> Clone("hd1112run13proj_Wp_WBosonEta");
   TH1D *hd1112run13proj_Wm_WBosonEta        = (TH1D*) hd1112_Wm_WBosonEta        -> Clone("hd1112run13proj_Wm_WBosonEta");
   TH1D *hd1112run13proj_Wp_WBosonRapidity   = (TH1D*) hd1112_Wp_WBosonRapidity   -> Clone("hd1112run13proj_Wp_WBosonRapidity");
   TH1D *hd1112run13proj_Wm_WBosonRapidity   = (TH1D*) hd1112_Wm_WBosonRapidity   -> Clone("hd1112run13proj_Wm_WBosonRapidity");
   TH1D *hd1112run13proj_Wp_ElectronEta      = (TH1D*) hd1112_Wp_ElectronEta      -> Clone("hd1112run13proj_Wp_ElectronEta");
   TH1D *hd1112run13proj_Wm_ElectronEta      = (TH1D*) hd1112_Wm_ElectronEta      -> Clone("hd1112run13proj_Wm_ElectronEta");

   for (int i = 1; i <= hd1112run13proj_Wp_WBosonEta->GetNbinsX(); ++i) {
     //hd1112run13proj_Wp_WBosonEta   -> SetBinContent(1, 0); 
     Double_t errBin = hd1112run13proj_Wp_WBosonEta -> GetBinError(i);
     Double_t Bin    = hd1112run13proj_Wp_WBosonEta -> GetBinContent(i);
     cout << "Bin = " << Bin << endl;
     cout << "errBin = " << errBin << endl;
     Double_t errBinProjected = errBin / projFactor;
     cout << "errBinProjected = " << errBinProjected << endl;
     hd1112run13proj_Wp_WBosonEta   -> SetBinError(i, errBinProjected);
   }

   for (int i = 1; i <= hd1112run13proj_Wm_WBosonEta->GetNbinsX(); ++i) {
     Double_t errBin = hd1112run13proj_Wm_WBosonEta -> GetBinError(i);
     Double_t errBinProjected = errBin / projFactor;
     cout << "Wm - errBinProjected = " << errBinProjected << endl;
     hd1112run13proj_Wm_WBosonEta   -> SetBinError(i, errBinProjected);
   }

   for (int i = 1; i <= hd1112run13proj_Wp_WBosonRapidity->GetNbinsX(); ++i) {
     //hd1112run13proj_Wp_WBosonRapidity   -> SetBinContent(1, 0); 
     Double_t errBin = hd1112run13proj_Wp_WBosonRapidity -> GetBinError(i);
     Double_t Bin    = hd1112run13proj_Wp_WBosonRapidity -> GetBinContent(i);
     cout << "Wp WRapidity - Bin = " << Bin << endl;
     cout << "Wp WRapidity - errBin = " << errBin << endl;
     Double_t errBinProjected = errBin / projFactor;
     cout << "Wp WRapidity - errBinProjected = " << errBinProjected << endl;
     hd1112run13proj_Wp_WBosonRapidity   -> SetBinError(i, errBinProjected);
   }

   for (int i = 1; i <= hd1112run13proj_Wm_WBosonRapidity->GetNbinsX(); ++i) {
     Double_t errBin = hd1112run13proj_Wm_WBosonRapidity -> GetBinError(i);
     Double_t errBinProjected = errBin / projFactor;
     cout << "Wm - errBinProjected = " << errBinProjected << endl;
     hd1112run13proj_Wm_WBosonRapidity   -> SetBinError(i, errBinProjected);
   }

   for (int i = 1; i <= hd1112run13proj_Wp_ElectronEta->GetNbinsX(); ++i) {
     Double_t errBin = hd1112run13proj_Wp_ElectronEta -> GetBinError(i);
     Double_t Bin    = hd1112run13proj_Wp_ElectronEta -> GetBinContent(i);
     cout << "Wp elPt - Bin = " << Bin << endl;
     cout << "Wp elPt - errBin = " << errBin << endl;
     Double_t errBinProjected = errBin / projFactor;
     cout << "Wp elPt - errBinProjected = " << errBinProjected << endl;
     hd1112run13proj_Wp_ElectronEta   -> SetBinError(i, errBinProjected);
   }

   for (int i = 1; i <= hd1112run13proj_Wm_ElectronEta->GetNbinsX(); ++i) {
     Double_t errBin = hd1112run13proj_Wm_ElectronEta -> GetBinError(i);
     Double_t errBinProjected = errBin / projFactor;
     cout << "Wm - errBinProjected = " << errBinProjected << endl;
     hd1112run13proj_Wm_ElectronEta   -> SetBinError(i, errBinProjected);
   }


   TLatex *textStarProj = new TLatex(0.25, 0.8, "STAR projection: #intL = 410 pb^{-1}");
   textStarProj -> SetNDC(); 
   textStarProj -> SetTextColor(kRed); 
   textStarProj -> SetTextFont(32);
   textStarProj -> SetTextSize(0.06);

   //TLatex *textSTAR = new TLatex(0.43, 0.17, "STAR p-p 500 GeV #intL = 25 pb^{-1}");
   //textSTAR -> SetNDC(); 
   //textSTAR -> SetTextSize(0.035);

   TLine *line1 = new TLine(-4, 3, 4, 3);
   //line1->SetLineColor(kRed);
   line1->SetLineWidth(2);
   line1->SetLineStyle(2);
   TLine *line2 = new TLine(-0.9, 2, 0.9, 2);
   line2->SetLineWidth(2);
   line2->SetLineStyle(2);

   gStyle->SetErrorX(0);

   TH1D *hd1112run13proj_WpWmRatio_vs_WBosonRapidity = (TH1D*) hd1112run13proj_Wp_WBosonRapidity   -> Clone("hd1112run13proj_WpWmRatio_vs_WBosonRapidity");
   hd1112run13proj_WpWmRatio_vs_WBosonRapidity  -> Divide(hd1112run13proj_Wp_WBosonRapidity, hd1112run13proj_Wm_WBosonRapidity);
   hd1112run13proj_WpWmRatio_vs_WBosonRapidity  -> SetMaximum(6);
   hd1112run13proj_WpWmRatio_vs_WBosonRapidity  -> SetMinimum(0);
   hd1112run13proj_WpWmRatio_vs_WBosonRapidity  -> SetMarkerStyle(20);
   //hd1112run13proj_WpWmRatio_vs_WBosonRapidity  -> SetTitle("run 11+12+run13proj; #eta_{W}; W^{+}/W^{-}");
   hd1112run13proj_WpWmRatio_vs_WBosonRapidity  -> SetTitle("; y_{W}; W^{+}/W^{-}");
   hd1112run13proj_WpWmRatio_vs_WBosonRapidity  -> SetStats(0);

   for (int i = 1; i <= hd1112run13proj_WpWmRatio_vs_WBosonRapidity->GetNbinsX(); ++i) {
     hd1112run13proj_WpWmRatio_vs_WBosonRapidity   -> SetBinContent(i, 2);
   }


   TH1D *hd1112run13proj_WpWmRatio = (TH1D*) hd1112run13proj_Wp_WBosonEta   -> Clone("hd1112run13proj_WpWmRatio");
   hd1112run13proj_WpWmRatio  -> Divide(hd1112run13proj_Wp_WBosonEta, hd1112run13proj_Wm_WBosonEta);
   hd1112run13proj_WpWmRatio  -> SetMaximum(6);
   hd1112run13proj_WpWmRatio  -> SetMinimum(0);
   hd1112run13proj_WpWmRatio  -> SetMarkerStyle(20);
   //hd1112run13proj_WpWmRatio  -> SetTitle("run 11+12+run13proj; #eta_{W}; W^{+}/W^{-}");
   hd1112run13proj_WpWmRatio  -> SetTitle("; #eta_{W}; W^{+}/W^{-}");
   hd1112run13proj_WpWmRatio  -> SetStats(0);

   for (int i = 1; i <= hd1112run13proj_WpWmRatio->GetNbinsX(); ++i) {
     hd1112run13proj_WpWmRatio   -> SetBinContent(i, 3);
   }

   TH1D *hd1112run13proj_WpWmRatio_vs_EleEta = (TH1D*) hd1112run13proj_Wp_ElectronEta   -> Clone("hd1112run13proj_WpWmRatio_vs_EleEta");
   hd1112run13proj_WpWmRatio_vs_EleEta  -> Divide(hd1112run13proj_Wp_ElectronEta, hd1112run13proj_Wm_ElectronEta);
   hd1112run13proj_WpWmRatio_vs_EleEta  -> SetMaximum(6);
   hd1112run13proj_WpWmRatio_vs_EleEta  -> SetMinimum(0);
   hd1112run13proj_WpWmRatio_vs_EleEta  -> SetMarkerStyle(20);
   //hd1112run13proj_WpWmRatio_vs_EleEta  -> SetTitle("run 11+12+run13proj; #eta_{el}; W^{+}/W^{-}");
   hd1112run13proj_WpWmRatio_vs_EleEta  -> SetTitle("; #eta_{el}; W^{+}/W^{-}");
   hd1112run13proj_WpWmRatio_vs_EleEta  -> SetStats(0);

   for (int i = 3; i <= hd1112run13proj_WpWmRatio_vs_EleEta->GetNbinsX()-2; ++i) {
     hd1112run13proj_WpWmRatio_vs_EleEta   -> SetBinContent(i, 3);
   }


   TCanvas *c4run13proj    = new TCanvas("c4run13proj", "run 11+12+run13proj: W-Eta", 800, 500);
   c4run13proj   -> Divide(2,1);
   c4run13proj_1 -> cd();
   hd1112run13proj_Wp_WBosonEta -> SetMarkerStyle(20);
   hd1112run13proj_Wp_WBosonEta -> SetTitle("W^{+}");
   hd1112run13proj_Wp_WBosonEta -> Draw("e");
   c4run13proj_2 -> cd();
   hd1112run13proj_Wm_WBosonEta -> SetTitle("W^{-}");
   hd1112run13proj_Wm_WBosonEta -> SetMarkerStyle(20);
   hd1112run13proj_Wm_WBosonEta -> Draw("e");

   c4run13proj->Print(outPath + "/plot_r11r12run13proj_WEta.eps");
   c4run13proj->Print(outPath + "/plot_r11r12run13proj_WEta.png");

   TCanvas *c5run13proj    = new TCanvas("c5run13proj", "run 11+12+run13proj: W+/W- versus Eta", 800, 500);
   c5run13proj     -> cd();
   c5run13proj     -> SetGrid(0,0);
   hd1112run13proj_WpWmRatio -> Draw("e");
   textStarProj    -> Draw();
   line1           -> Draw();

   c5run13proj->Print(outPath + "/plot_r11r12run13proj_WpWmRatio_vs_Eta.eps");
   c5run13proj->Print(outPath + "/plot_r11r12run13proj_WpWmRatio_vs_Eta.png");

   // PRINT the table of contents
   cout << " " << endl;
   cout << " " << endl;
   cout << "W+/W- 2013 proj. versus boson pseudo-rapidity " << endl;
   cout << "Pseudo-rapidity  " << "ratio  " << "error  " << endl;
   for (int i = 1; i <= hd1112run13proj_WpWmRatio->GetNbinsX(); ++i) {
     //hd1112run13proj_WpWmRatio_vs_EleEta   -> SetBinContent(1, 0); 
     Double_t errBin    = hd1112run13proj_WpWmRatio -> GetBinError(i);
     Double_t Bin       = hd1112run13proj_WpWmRatio -> GetBinContent(i);
     Double_t BinCenter = hd1112run13proj_WpWmRatio -> GetXaxis() -> GetBinCenter(i);
     cout << BinCenter <<"   " << Bin << "   " << errBin << endl;
   }

   TCanvas *c5run13projRapidity    = new TCanvas("c5run13projRapidity", "run 11+12+run13proj: W+/W- versus Rapidity", 800, 500);
   c5run13projRapidity     -> cd();
   c5run13projRapidity     -> SetGrid(0,0);
   hd1112run13proj_WpWmRatio_vs_WBosonRapidity -> Draw("e");
   textStarProj    -> Draw();
   line2           -> Draw();

   c5run13projRapidity->Print(outPath + "/plot_r11r12run13proj_WpWmRatio_vs_Rapidity.eps");
   c5run13projRapidity->Print(outPath + "/plot_r11r12run13proj_WpWmRatio_vs_Rapidity.png");

   // PRINT the table of contents
   cout << " " << endl;
   cout << " " << endl;
   cout << "W+/W- 2013 proj. versus boson Rapidity " << endl;
   cout << "Rapidity  " << "ratio  " << "error  " << endl;
   for (int i = 1; i <= hd1112run13proj_WpWmRatio_vs_WBosonRapidity->GetNbinsX(); ++i) {
     Double_t errBin    = hd1112run13proj_WpWmRatio_vs_WBosonRapidity -> GetBinError(i);
     Double_t Bin       = hd1112run13proj_WpWmRatio_vs_WBosonRapidity -> GetBinContent(i);
     Double_t BinCenter = hd1112run13proj_WpWmRatio_vs_WBosonRapidity -> GetXaxis() -> GetBinCenter(i);
     cout << BinCenter <<"   " << Bin << "   " << errBin << endl;
   }


   TCanvas *c4run13projel    = new TCanvas("c4run13projel", "run 11+12+run13proj: Electron-Eta", 800, 500);
   c4run13projel -> Divide(2,1);
   c4run13projel_1 -> cd();
   hd1112run13proj_Wp_ElectronEta -> SetMarkerStyle(20);
   hd1112run13proj_Wp_ElectronEta -> SetTitle("W^{+}");
   hd1112run13proj_Wp_ElectronEta -> Draw("e");
   c4run13projel_2 -> cd();
   hd1112run13proj_Wm_ElectronEta -> SetTitle("W^{-}");
   hd1112run13proj_Wm_ElectronEta -> SetMarkerStyle(20);
   hd1112run13proj_Wm_ElectronEta -> Draw("e");

   c4run13projel->Print(outPath + "/plot_r11r12run13proj_ElecEta.eps");
   c4run13projel->Print(outPath + "/plot_r11r12run13proj_ElecEta.png");

   TCanvas *c5run13projel    = new TCanvas("c5run13projel", "run 11+12+run13proj: W+/W- versus Elec-Eta", 800, 500);
   c5run13projel -> cd();
   c5run13projel -> SetGrid(0,0);
   hd1112run13proj_WpWmRatio_vs_EleEta -> Draw("e");
   textStarProj    -> Draw();
   line1           -> DrawLine(-2, 3, 2, 3);

   c5run13projel->Print(outPath + "/plot_r11r12run13proj_WpWmRatio_vs_ElecEta.eps");
   c5run13projel->Print(outPath + "/plot_r11r12run13proj_WpWmRatio_vs_ElecEta.png");

   // PRINT the table of contents
   cout << " " << endl;
   cout << " " << endl;
   cout << "W+/W- 2013 proj. versus lepton pseudo-rapidity " << endl;
   cout << "Pseudo-rapidity  " << "ratio  " << "error  " << endl;
   for (int i = 3; i <= hd1112run13proj_WpWmRatio_vs_EleEta->GetNbinsX()-2; ++i) {
     //hd1112run13proj_WpWmRatio_vs_EleEta   -> SetBinContent(1, 0); 
     Double_t errBin    = hd1112run13proj_WpWmRatio_vs_EleEta -> GetBinError(i);
     Double_t Bin       = hd1112run13proj_WpWmRatio_vs_EleEta -> GetBinContent(i);
     Double_t BinCenter = hd1112run13proj_WpWmRatio_vs_EleEta -> GetXaxis() -> GetBinCenter(i);
     cout << BinCenter <<"   " << Bin << "   " << errBin << endl;
   }

}
