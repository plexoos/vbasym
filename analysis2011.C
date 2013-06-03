void analysis2011() {

std::cout.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed

//gROOT->Reset();

// Load the libraries:

//gROOT->Macro("/star/u/fazio/offline/users/fazio/vbasym/macros/loadLibs.C"); 

 TString outPath =  "/star/u/fazio/offline/users/fazio/vbasym_results/plots";


 //Styles:
 gStyle-> SetPadBottomMargin(0.15);
 gStyle-> SetPadLeftMargin(0.13);
 gStyle-> SetStatFontSize(0.05);
 gStyle->SetOptStat("mruoi");


 //float lumiDataTot = 24.28; // pb-1
 float lumiDataTot = 24.42; // pb-1
 //float lumiDataEff = 23.86; // pb-1
 float lumiDataEff = 23.99; // pb-1
 float lumiMC_Z = 955.15; // pb-1
 float lumiMC_WpToTauTau = 2136.49; // pb-1
 float lumiMC_WmToTauTau = 1995.34; // pb-1


 float scaleZ = lumiDataEff/lumiMC_Z;
 float scaleWptt = lumiDataEff/lumiMC_WpToTauTau;
 float scaleWmtt = lumiDataEff/lumiMC_WmToTauTau;

 cout.precision(3);
 cout << "*****************************************" << endl;
 cout << "SCALE FACTORS" << endl;
 cout << "Z0 lumi scale factor =" << scaleZ << endl;
 cout << "Wp->tt lumi scale factor =" << scaleWptt << endl;
 cout << "Wm->tt lumi scale factor =" << scaleWmtt << endl;
 cout << "*****************************************" << endl;

// open histogram files:

 TFile *fileData = TFile::Open("./vbana_cut05_data_final.root");
 TFile *fileMCWp = TFile::Open("./vbana_cut05_mc_wp.root");
 TFile *fileMCWm = TFile::Open("./vbana_cut05_mc_wm.root");
 TFile *fileMCQCD = TFile::Open("./vbana_cut05_mc_qcd.root");
 TFile *fileMCZ = TFile::Open("vbana_cut05_mc_z_to_ee.root");
 TFile *fileMCWptt = TFile::Open("vbana_cut05_mc_wp_to_tt.root");
 TFile *fileMCWmtt = TFile::Open("vbana_cut05_mc_wm_to_tt.root");


 // W total (W+ + W-)
 TH1* hd_PtLepTPC = (TH1*)fileData->Get("track_cand_pass_final_QEToPT/hTrackPt");
 TH1* hd_PtLep = (TH1*)fileData->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hZ_PtLep = (TH1*)fileMCZ->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
 cout << hZ_PtLep->ClassName() << endl;
 TH1* hWp_PtLep = (TH1*)fileMCWp->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWm_PtLep = (TH1*)fileMCWm->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWptt_PtLep = (TH1*)fileMCWptt->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWmtt_PtLep = (TH1*)fileMCWmtt->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hQ_PtLep = (TH1*)fileMCQCD->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");

 TH1* hd_PtLepTPCPt10 = (TH1*)fileData->Get("track_cand_pass_final_QEToPT_Pt>10/hTrackPt");
 TH1* hd_PtLepPt10 = (TH1*)fileData->Get("track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hZ_PtLepPt10 = (TH1*)fileMCZ->Get("track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWp_PtLepPt10 = (TH1*)fileMCWp->Get("track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWm_PtLepPt10 = (TH1*)fileMCWm->Get("track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWptt_PtLepPt10 = (TH1*)fileMCWptt->Get("track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWmtt_PtLepPt10 = (TH1*)fileMCWmtt->Get("track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hQ_PtLepPt10 = (TH1*)fileMCQCD->Get("track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");

 TH1* hd_PtLepQCD = (TH1*)fileData->Get("track_cand_pass_qcd_QEToPT/hEcalScaledPt");

 TH2* hd_QxEtoPt_Vs_Et = (TH2*)fileData->Get("track_cand_pass_final/hQxEtoPt_Vs_Et_PrimaryTrack");
   cout << hd_QxEtoPt_Vs_Et->ClassName() << endl;
 TH2* hd_QxEtoPt_Vs_Et_passCut = (TH2*)fileData->Get("track_cand_pass_final_QEToPT/hQxEtoPt_Vs_Et_PrimaryTrack");



 // W+
 TH1* hd_Wp_PtLep = (TH1*)fileData->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hZ_Wp_PtLep = (TH1*)fileMCZ->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWp_Wp_PtLep = (TH1*)fileMCWp->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWm_Wp_PtLep = (TH1*)fileMCWm->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWptt_Wp_PtLep = (TH1*)fileMCWptt->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hQ_Wp_PtLep = (TH1*)fileMCQCD->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");

 TH1* hd_Wp_PtLepPt10 = (TH1*)fileData->Get("W+_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hZ_Wp_PtLepPt10 = (TH1*)fileMCZ->Get("W+_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWp_Wp_PtLepPt10 = (TH1*)fileMCWp->Get("W+_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWm_Wp_PtLepPt10 = (TH1*)fileMCWm->Get("W+_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWptt_Wp_PtLepPt10 = (TH1*)fileMCWptt->Get("W+_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hQ_Wp_PtLepPt10 = (TH1*)fileMCQCD->Get("W+_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");


 // W-
 TH1* hd_Wm_PtLep = (TH1*)fileData->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hZ_Wm_PtLep = (TH1*)fileMCZ->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWp_Wm_PtLep = (TH1*)fileMCWp->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWm_Wm_PtLep = (TH1*)fileMCWm->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hWmtt_Wm_PtLep = (TH1*)fileMCWmtt->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
 TH1* hQ_Wm_PtLep = (TH1*)fileMCQCD->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");

 TH1* hd_Wm_PtLepPt10 = (TH1*)fileData->Get("W-_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hZ_Wm_PtLepPt10 = (TH1*)fileMCZ->Get("W-_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWp_Wm_PtLepPt10 = (TH1*)fileMCWp->Get("W-_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWm_Wm_PtLepPt10 = (TH1*)fileMCWm->Get("W-_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hWmtt_Wm_PtLepPt10 = (TH1*)fileMCWmtt->Get("W-_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");
 TH1* hQ_Wm_PtLepPt10 = (TH1*)fileMCQCD->Get("W-_track_cand_pass_final_QEToPT_Pt>10/hEcalScaledPt");


 hd_PtLepTPC->GetXaxis()->SetTitle("P_{T}^{lep}(TPC)");
 hd_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hZ_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWp_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWm_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWptt_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWmtt_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hQ_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hd_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hZ_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWp_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWm_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWptt_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hQ_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hd_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hZ_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWp_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWm_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWmtt_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hQ_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{lep}");
 hd_PtLepTPCPt10->GetXaxis()->SetTitle("P_{T}^{lep}(TPC)");
 hd_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hZ_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWp_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWm_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWptt_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWmtt_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hQ_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hd_Wp_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hZ_Wp_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWp_Wp_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWm_Wp_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWptt_Wp_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hQ_Wp_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hd_Wm_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hZ_Wm_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWp_Wm_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWm_Wm_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hWmtt_Wm_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");
 hQ_Wm_PtLepPt10->GetXaxis()->SetTitle("P_{T}^{lep}");

 hd_PtLepTPC->GetYaxis()->SetTitle("events");
 hd_PtLep->GetYaxis()->SetTitle("events");
 hZ_PtLep->GetYaxis()->SetTitle("events");
 hWp_PtLep->GetYaxis()->SetTitle("events");
 hWm_PtLep->GetYaxis()->SetTitle("events");
 hWptt_PtLep->GetYaxis()->SetTitle("events");
 hWmtt_PtLep->GetYaxis()->SetTitle("events");
 hQ_PtLep->GetYaxis()->SetTitle("events");
 hd_Wp_PtLep->GetYaxis()->SetTitle("events");
 hZ_Wp_PtLep->GetYaxis()->SetTitle("events");
 hWp_Wp_PtLep->GetYaxis()->SetTitle("events");
 hWm_Wp_PtLep->GetYaxis()->SetTitle("events");
 hWptt_Wp_PtLep->GetYaxis()->SetTitle("events");
 hQ_Wp_PtLep->GetYaxis()->SetTitle("events");
 hd_Wm_PtLep->GetYaxis()->SetTitle("events");
 hZ_Wm_PtLep->GetYaxis()->SetTitle("events");
 hWp_Wm_PtLep->GetYaxis()->SetTitle("events");
 hWm_Wm_PtLep->GetYaxis()->SetTitle("events");
 hWmtt_Wm_PtLep->GetYaxis()->SetTitle("events");
 hQ_Wm_PtLep->GetYaxis()->SetTitle("events");
 hd_PtLepTPCPt10->GetYaxis()->SetTitle("events");
 hd_PtLepPt10->GetYaxis()->SetTitle("events");
 hZ_PtLepPt10->GetYaxis()->SetTitle("events");
 hWp_PtLepPt10->GetYaxis()->SetTitle("events");
 hWm_PtLepPt10->GetYaxis()->SetTitle("events");
 hWptt_PtLepPt10->GetYaxis()->SetTitle("events");
 hWmtt_PtLepPt10->GetYaxis()->SetTitle("events");
 hQ_PtLepPt10->GetYaxis()->SetTitle("events");
 hd_Wp_PtLepPt10->GetYaxis()->SetTitle("events");
 hZ_Wp_PtLepPt10->GetYaxis()->SetTitle("events");
 hWp_Wp_PtLepPt10->GetYaxis()->SetTitle("events");
 hWm_Wp_PtLepPt10->GetYaxis()->SetTitle("events");
 hWptt_Wp_PtLepPt10->GetYaxis()->SetTitle("events");
 hQ_Wp_PtLepPt10->GetYaxis()->SetTitle("events");
 hd_Wm_PtLepPt10->GetYaxis()->SetTitle("events");
 hZ_Wm_PtLepPt10->GetYaxis()->SetTitle("events");
 hWp_Wm_PtLepPt10->GetYaxis()->SetTitle("events");
 hWm_Wm_PtLepPt10->GetYaxis()->SetTitle("events");
 hWmtt_Wm_PtLepPt10->GetYaxis()->SetTitle("events");
 hQ_Wm_PtLepPt10->GetYaxis()->SetTitle("events");

 hd_PtLepTPC->SetNameTitle("hd_PtLepTPC","Data");
 hd_PtLep->SetNameTitle("hd_PtLep","Data");
 hd_Wp_PtLep->SetNameTitle("hd_Wp_PtLep","Data (W+)");
 hd_Wm_PtLep->SetNameTitle("hd_Wm_PtLep","Data (W-)");
 hd_PtLepTPCPt10->SetNameTitle("hd_PtLepTPCPt10","Data");
 hd_PtLepPt10->SetNameTitle("hd_PtLepPt10","Data");
 hd_Wp_PtLepPt10->SetNameTitle("hd_Wp_PtLepPt10","Data (W+)");
 hd_Wm_PtLepPt10->SetNameTitle("hd_Wm_PtLepPt10","Data (W-)");

 hd_PtLepTPC->SetTitleOffset(1.3,"y");
 hd_PtLep->SetTitleOffset(1.3,"y");
 hd_Wp_PtLep->SetTitleOffset(1.3,"y");
 hd_Wm_PtLep->SetTitleOffset(1.3,"y");
 hd_PtLepTPCPt10->SetTitleOffset(1.3,"y");
 hd_PtLepPt10->SetTitleOffset(1.3,"y");
 hd_Wp_PtLepPt10->SetTitleOffset(1.3,"y");
 hd_Wm_PtLepPt10->SetTitleOffset(1.3,"y");

 TH1F *hd_PtLep_1 = (TH1F*)hd_PtLep->Clone("hd_PtLep_1");
 TH1F *hZ_PtLep_1 = (TH1F*)hZ_PtLep->Clone("hZ_PtLep_1");
 TH1F *hWptt_PtLep_1 = (TH1F*)hWptt_PtLep->Clone("hWptt_PtLep_1");
 TH1F *hWmtt_PtLep_1 = (TH1F*)hWmtt_PtLep->Clone("hWmtt_PtLep_1");
 TH1F *hd_Wp_PtLep_1 = (TH1F*)hd_Wp_PtLep->Clone("hd_Wp_PtLep_1");
 TH1F *hZ_Wp_PtLep_1 = (TH1F*)hZ_Wp_PtLep->Clone("hZ_Wp_PtLep_1");
 TH1F *hWptt_Wp_PtLep_1 = (TH1F*)hWptt_Wp_PtLep->Clone("hWptt_Wp_PtLep_1");
 TH1F *hd_Wm_PtLep_1 = (TH1F*)hd_Wm_PtLep->Clone("hd_Wm_PtLep_1");
 TH1F *hZ_Wm_PtLep_1 = (TH1F*)hZ_Wm_PtLep->Clone("hZ_Wm_PtLep_1");
 TH1F *hWmtt_Wm_PtLep_1 = (TH1F*)hWmtt_Wm_PtLep->Clone("hWmtt_Wm_PtLep_1");

 TH1F *hd_PtLepPt10_1 = (TH1F*)hd_PtLepPt10->Clone("hd_PtLepPt10_1");
 TH1F *hZ_PtLepPt10_1 = (TH1F*)hZ_PtLepPt10->Clone("hZ_PtLepPt10_1");
 TH1F *hWptt_PtLepPt10_1 = (TH1F*)hWptt_PtLepPt10->Clone("hWptt_PtLepPt10_1");
 TH1F *hWmtt_PtLepPt10_1 = (TH1F*)hWmtt_PtLepPt10->Clone("hWmtt_PtLepPt10_1");
 TH1F *hd_Wp_PtLepPt10_1 = (TH1F*)hd_Wp_PtLepPt10->Clone("hd_Wp_PtLepPt10_1");
 TH1F *hZ_Wp_PtLepPt10_1 = (TH1F*)hZ_Wp_PtLepPt10->Clone("hZ_Wp_PtLepPt10_1");
 TH1F *hWptt_Wp_PtLepPt10_1 = (TH1F*)hWptt_Wp_PtLepPt10->Clone("hWptt_Wp_PtLepPt10_1");
 TH1F *hd_Wm_PtLepPt10_1 = (TH1F*)hd_Wm_PtLepPt10->Clone("hd_Wm_PtLepPt10_1");
 TH1F *hZ_Wm_PtLepPt10_1 = (TH1F*)hZ_Wm_PtLepPt10->Clone("hZ_Wm_PtLepPt10_1");
 TH1F *hWmtt_Wm_PtLepPt10_1 = (TH1F*)hWmtt_Wm_PtLepPt10->Clone("hWmtt_Wm_PtLepPt10_1");


 TCanvas *c1 = new TCanvas("c1","",800,800);
 c1->Divide(3,3);

 c1_1->cd();
 hd_PtLepTPC->Draw();
 c1_2->cd();
 hd_PtLep_1->SetNameTitle("hd_PtLep","Data");
 hd_PtLep_1->Draw();
 c1_3->cd();
 hZ_PtLep_1->SetNameTitle("hZ_PtLep","Z^{0}->ee");
 hZ_PtLep_1->Draw();
 c1_4->cd();
 hWp_PtLep->SetNameTitle("hWp_PtLep","W^{+}->ee");
 hWp_PtLep->Draw();
 c1_5->cd();
 hWm_PtLep->SetNameTitle("hWp_PtLep","W^{-}->ee");
 hWm_PtLep->Draw();
 c1_6->cd();
 hWptt_PtLep_1->SetNameTitle("hWp_PtLep","W^{+}->#tau#tau");
 hWptt_PtLep_1->Draw();
 c1_7->cd();
 hWmtt_PtLep_1->SetNameTitle("hWp_PtLep","W^{-}->#tau#tau");
 hWmtt_PtLep_1->Draw();
 c1_8->cd();
 hQ_PtLep->SetNameTitle("hQ_PtLep","QCD");
 hQ_PtLep->Draw();

 c1->Print(outPath + "/plot_1.eps");
 c1->Print(outPath + "/plot_1.png");


 TCanvas *c1b = new TCanvas("c1b","",800,800);
 c1b->Divide(3,3);

 c1b_1->cd();
 hd_PtLepTPCPt10->Draw();
 c1b_2->cd();
 hd_PtLepPt10_1->SetNameTitle("hd_PtLepPt10","Data");
 hd_PtLepPt10_1->Draw();
 c1b_3->cd();
 hZ_PtLepPt10_1->SetNameTitle("hZ_PtLepPt10","Z^{0}->ee");
 hZ_PtLepPt10_1->Draw();
 c1b_4->cd();
 hWp_PtLepPt10->SetNameTitle("hWp_PtLepPt10","W^{+}->ee");
 hWp_PtLepPt10->Draw();
 c1b_5->cd();
 hWm_PtLepPt10->SetNameTitle("hWp_PtLepPt10","W^{-}->ee");
 hWm_PtLepPt10->Draw();
 c1b_6->cd();
 hWptt_PtLepPt10_1->SetNameTitle("hWp_PtLepPt10","W^{+}->#tau#tau");
 hWptt_PtLepPt10_1->Draw();
 c1b_7->cd();
 hWmtt_PtLepPt10_1->SetNameTitle("hWp_PtLepPt10","W^{-}->#tau#tau");
 hWmtt_PtLepPt10_1->Draw();
 c1b_8->cd();
 hQ_PtLepPt10->SetNameTitle("hQ_PtLepPt10","QCD");
 hQ_PtLepPt10->Draw();

 c1b->Print(outPath + "/plot_1b.eps");
 c1b->Print(outPath + "/plot_1b.png");



 TCanvas *c2 = new TCanvas("c2","W TOTAL sample",800,800);

 hZ_PtLep->Scale(scaleZ);
 hWptt_PtLep->Scale(scaleWptt);
 hWmtt_PtLep->Scale(scaleWmtt);
 hd_PtLep->Add(hd_PtLep,hZ_PtLep,1,-1); 
 hd_PtLep->Add(hd_PtLep,hWptt_PtLep,1,-1); 
 hd_PtLep->Add(hd_PtLep,hWmtt_PtLep,1,-1); 

 TH1F *hZ_PtLep_2 = (TH1F*)hZ_PtLep->Clone("hZ_PtLep_2"); 
 TH1F *hWptt_PtLep_2 = (TH1F*)hWptt_PtLep->Clone("hWptt_PtLep_2");
 TH1F *hWmtt_PtLep_2 = (TH1F*)hWmtt_PtLep->Clone("hWmtt_PtLep_2"); 
 TH1F *hd_PtLep_2 = (TH1F*)hd_PtLep->Clone("hd_PtLep_2");

 c2->Divide(2,2);
 c2_1->cd();
 c2_1->SetLogy(1);
 hZ_PtLep_2-> SetFillStyle(3448);
 hZ_PtLep_2-> SetFillColor(kGreen);
 hd_PtLep_1->Draw();
 hZ_PtLep_2-> SetStats(0);
 hWptt_PtLep_2-> SetFillStyle(3446);
 hWptt_PtLep_2-> SetFillColor(kRed);
 hWptt_PtLep_2-> SetStats(0);
 hWptt_PtLep_2->Draw("same");
 hWmtt_PtLep_2-> SetFillColor(kBlue);
 hWmtt_PtLep_2-> SetStats(0);
 hZ_PtLep_2->Draw("same");
 hWmtt_PtLep_2->Draw("same");

 c2_2->cd();
 c2_2->SetLogy(1);
 hZ_PtLep_2-> SetStats(1);
 hZ_PtLep_2->SetNameTitle("hZ_PtLep_2","Z^{0}->ee - norm. to data lumi");
 hZ_PtLep_2->Draw();

 c2_3->cd();
 c2_3->SetLogy(1);
 hWptt_PtLep_2-> SetStats(1);
 hWptt_PtLep_2->SetNameTitle("hWptt_PtLep_2","W^{+}->#tau#tau - norm. to data lumi");
 hWptt_PtLep_2-> SetFillColor(kRed);
 hWptt_PtLep_2->Draw();

 c2_4->cd();
 c2_4->SetLogy(1);
 hWmtt_PtLep_2-> SetStats(1);
 hWmtt_PtLep_2->SetNameTitle("hWmtt_PtLep_2","W^{-}->#tau#tau - norm. to data lumi");
 hWmtt_PtLep_2-> SetFillColor(kBlue);
 hWmtt_PtLep_2->Draw();


 c2->Print(outPath + "/plot_2.eps");
 c2->Print(outPath + "/plot_2.png");


 TCanvas *c2b = new TCanvas("c2b","W TOTAL sample",800,800);

 hZ_PtLepPt10->Scale(scaleZ);
 hWptt_PtLepPt10->Scale(scaleWptt);
 hWmtt_PtLepPt10->Scale(scaleWmtt);
 hd_PtLepPt10->Add(hd_PtLepPt10,hZ_PtLepPt10,1,-1); 
 hd_PtLepPt10->Add(hd_PtLepPt10,hWptt_PtLepPt10,1,-1); 
 hd_PtLepPt10->Add(hd_PtLepPt10,hWmtt_PtLepPt10,1,-1); 

 TH1F *hZ_PtLepPt10_2 = (TH1F*)hZ_PtLepPt10->Clone("hZ_PtLepPt10_2"); 
 TH1F *hWptt_PtLepPt10_2 = (TH1F*)hWptt_PtLepPt10->Clone("hWptt_PtLepPt10_2");
 TH1F *hWmtt_PtLepPt10_2 = (TH1F*)hWmtt_PtLepPt10->Clone("hWmtt_PtLepPt10_2"); 
 TH1F *hd_PtLepPt10_2 = (TH1F*)hd_PtLepPt10->Clone("hd_PtLepPt10_2");

 c2b->Divide(2,2);
 c2b_1->cd();
 c2b_1->SetLogy(1);
 hZ_PtLepPt10_2-> SetFillStyle(3448);
 hZ_PtLepPt10_2-> SetFillColor(kGreen);
 hd_PtLepPt10_1->Draw();
 hZ_PtLepPt10_2-> SetStats(0);
 hWptt_PtLepPt10_2-> SetFillStyle(3446);
 hWptt_PtLepPt10_2-> SetFillColor(kRed);
 hWptt_PtLepPt10_2-> SetStats(0);
 hWptt_PtLepPt10_2->Draw("same");
 hWmtt_PtLepPt10_2-> SetFillColor(kBlue);
 hWmtt_PtLepPt10_2-> SetStats(0);
 hZ_PtLepPt10_2->Draw("same");
 hWmtt_PtLepPt10_2->Draw("same");

 c2b_2->cd();
 c2b_2->SetLogy(1);
 hZ_PtLepPt10_2-> SetStats(1);
 hZ_PtLepPt10_2->SetNameTitle("hZ_PtLepPt10_2","Z^{0}->ee - norm. to data lumi");
 hZ_PtLepPt10_2->Draw();

 c2b_3->cd();
 c2b_3->SetLogy(1);
 hWptt_PtLepPt10_2-> SetStats(1);
 hWptt_PtLepPt10_2->SetNameTitle("hWptt_PtLepPt10_2","W^{+}->#tau#tau - norm. to data lumi");
 hWptt_PtLepPt10_2-> SetFillColor(kRed);
 hWptt_PtLepPt10_2->Draw();

 c2b_4->cd();
 c2b_4->SetLogy(1);
 hWmtt_PtLepPt10_2-> SetStats(1);
 hWmtt_PtLepPt10_2->SetNameTitle("hWmtt_PtLepPt10_2","W^{-}->#tau#tau - norm. to data lumi");
 hWmtt_PtLepPt10_2-> SetFillColor(kBlue);
 hWmtt_PtLepPt10_2->Draw();


 c2b->Print(outPath + "/plot_2b.eps");
 c2b->Print(outPath + "/plot_2b.png");


 TCanvas *c3 = new TCanvas("c3","W TOTAL sample",800,400);

 c3->Divide(2,1);

 c3_1->cd();
 c3_1->SetLogy(0);
 hZ_PtLep_2-> SetFillStyle(3448);
 hZ_PtLep_2-> SetFillColor(kGreen);
 hd_PtLep_1->Draw();
 hZ_PtLep_2-> SetStats(0);
 hWptt_PtLep_2-> SetFillStyle(3446);
 hWptt_PtLep_2-> SetFillColor(kRed);
 hWptt_PtLep_2-> SetStats(0);
 hWptt_PtLep_2->Draw("same");
 hWmtt_PtLep_2-> SetFillColor(kBlue);
 hWmtt_PtLep_2-> SetStats(0);
 hZ_PtLep_2->Draw("same");
 hWmtt_PtLep_2->Draw("same");

 c3_2->cd();
 c3_2->SetLogy(0);
 hd_PtLep_2->SetNameTitle("hd_PtLep","data- Z^{0}->ee and W^{+-}->#tau#tau subtracted");
 hd_PtLep_2->Draw();

 c3->Print(outPath + "/plot_3.eps");
 c3->Print(outPath + "/plot_3.png");


 TCanvas *c3b = new TCanvas("c3b","W TOTAL sample",800,400);

 c3b->Divide(2,1);

 c3b_1->cd();
 c3b_1->SetLogy(0);
 hZ_PtLepPt10_2-> SetFillStyle(3448);
 hZ_PtLepPt10_2-> SetFillColor(kGreen);
 hd_PtLepPt10_1->Draw();
 hZ_PtLepPt10_2-> SetStats(0);
 hWptt_PtLepPt10_2-> SetFillStyle(3446);
 hWptt_PtLepPt10_2-> SetFillColor(kRed);
 hWptt_PtLepPt10_2-> SetStats(0);
 hWptt_PtLepPt10_2->Draw("same");
 hWmtt_PtLepPt10_2-> SetFillColor(kBlue);
 hWmtt_PtLepPt10_2-> SetStats(0);
 hZ_PtLepPt10_2->Draw("same");
 hWmtt_PtLepPt10_2->Draw("same");

 c3b_2->cd();
 c3b_2->SetLogy(0);
 hd_PtLepPt10_2->SetNameTitle("hd_PtLepPt10","data- Z^{0}->ee and W^{+-}->#tau#tau subtracted");
 hd_PtLepPt10_2->Draw();

 c3b->Print(outPath + "/plot_3b.eps");
 c3b->Print(outPath + "/plot_3b.png");


 // W+ sample

 TCanvas *c4 = new TCanvas("c4","",800,800);

 hZ_Wp_PtLep->Scale(scaleZ);
 hWptt_Wp_PtLep->Scale(scaleWptt);
 hd_Wp_PtLep->Add(hd_Wp_PtLep,hZ_Wp_PtLep,1,-1); 
 hd_Wp_PtLep->Add(hd_Wp_PtLep,hWptt_Wp_PtLep,1,-1);

 TH1F *hZ_Wp_PtLep_2 = (TH1F*)hZ_Wp_PtLep->Clone("hZ_Wp_PtLep_2"); 
 TH1F *hWptt_Wp_PtLep_2 = (TH1F*)hWptt_Wp_PtLep->Clone("hWptt_Wp_PtLep_2");
 TH1F *hd_Wp_PtLep_2 = (TH1F*)hd_Wp_PtLep->Clone("hd_Wp_PtLep_2");

 c4-> SetTitle("W+ sample");
 c4->Divide(2,2);
 c4_1->cd();
 c4_1->SetLogy(1);
 hZ_Wp_PtLep_2-> SetFillStyle(3448);
 hZ_Wp_PtLep_2-> SetFillColor(kGreen);
 hd_Wp_PtLep_1->Draw();
 hZ_Wp_PtLep_2-> SetStats(0);
 hWptt_Wp_PtLep_2-> SetFillStyle(3446);
 hWptt_Wp_PtLep_2-> SetFillColor(kRed);
 hWptt_Wp_PtLep_2-> SetStats(0);
 hWptt_Wp_PtLep_2->Draw("same");
 hZ_Wp_PtLep_2->Draw("same");

 c4_2->cd();
 c4_2->SetLogy(1);
 hZ_Wp_PtLep_2-> SetStats(1);
 hZ_Wp_PtLep_2->SetNameTitle("hZ_Wp_PtLep_2","Z^{0}->ee - norm. to data lumi");
 hZ_Wp_PtLep_2->Draw();

 c4_3->cd();
 c4_3->SetLogy(1);
 hWptt_Wp_PtLep_2-> SetStats(1);
 hWptt_Wp_PtLep_2->SetNameTitle("hWptt_Wp_PtLep_2","W^{+}->#tau#tau - norm. to data lumi");
 hWptt_Wp_PtLep_2-> SetFillColor(kRed);
 hWptt_Wp_PtLep_2->Draw();


 c4->Print(outPath + "/plot_4.eps");
 c4->Print(outPath + "/plot_4.png");

 
 TCanvas *c4b = new TCanvas("c4b","",800,800);

 hZ_Wp_PtLepPt10->Scale(scaleZ);
 hWptt_Wp_PtLepPt10->Scale(scaleWptt);
 hd_Wp_PtLepPt10->Add(hd_Wp_PtLepPt10,hZ_Wp_PtLepPt10,1,-1); 
 hd_Wp_PtLepPt10->Add(hd_Wp_PtLepPt10,hWptt_Wp_PtLepPt10,1,-1);

 TH1F *hZ_Wp_PtLepPt10_2 = (TH1F*)hZ_Wp_PtLepPt10->Clone("hZ_Wp_PtLepPt10_2"); 
 TH1F *hWptt_Wp_PtLepPt10_2 = (TH1F*)hWptt_Wp_PtLepPt10->Clone("hWptt_Wp_PtLepPt10_2");
 TH1F *hd_Wp_PtLepPt10_2 = (TH1F*)hd_Wp_PtLepPt10->Clone("hd_Wp_PtLepPt10_2");

 c4b-> SetTitle("W+ sample");
 c4b->Divide(2,2);
 c4b_1->cd();
 c4b_1->SetLogy(1);
 hZ_Wp_PtLepPt10_2-> SetFillStyle(3448);
 hZ_Wp_PtLepPt10_2-> SetFillColor(kGreen);
 hd_Wp_PtLepPt10_1->Draw();
 hZ_Wp_PtLepPt10_2-> SetStats(0);
 hWptt_Wp_PtLepPt10_2-> SetFillStyle(3446);
 hWptt_Wp_PtLepPt10_2-> SetFillColor(kRed);
 hWptt_Wp_PtLepPt10_2-> SetStats(0);
 hWptt_Wp_PtLepPt10_2->Draw("same");
 hZ_Wp_PtLepPt10_2->Draw("same");

 c4b_2->cd();
 c4b_2->SetLogy(1);
 hZ_Wp_PtLepPt10_2-> SetStats(1);
 hZ_Wp_PtLepPt10_2->SetNameTitle("hZ_Wp_PtLepPt10_2","Z^{0}->ee - norm. to data lumi");
 hZ_Wp_PtLepPt10_2->Draw();

 c4b_3->cd();
 c4b_3->SetLogy(1);
 hWptt_Wp_PtLepPt10_2-> SetStats(1);
 hWptt_Wp_PtLepPt10_2->SetNameTitle("hWptt_Wp_PtLepPt10_2","W^{+}->#tau#tau - norm. to data lumi");
 hWptt_Wp_PtLepPt10_2-> SetFillColor(kRed);
 hWptt_Wp_PtLepPt10_2->Draw();


 c4b->Print(outPath + "/plot_4b.eps");
 c4b->Print(outPath + "/plot_4b.png");


 TCanvas *c5 = new TCanvas("c5","",800,400);

 c5-> SetTitle("W+ sample");
 c5->Divide(2,1);

 c5_1->cd();
 c5_1->SetLogy(0);
 hZ_Wp_PtLep_2-> SetFillStyle(3448);
 hZ_Wp_PtLep_2-> SetFillColor(kGreen);
 hd_Wp_PtLep_1->Draw();
 hZ_Wp_PtLep_2-> SetStats(0);
 hWptt_Wp_PtLep_2-> SetFillStyle(3446);
 hWptt_Wp_PtLep_2-> SetFillColor(kRed);
 hWptt_Wp_PtLep_2-> SetStats(0);
 hWptt_Wp_PtLep_2->Draw("same");
 hZ_Wp_PtLep_2->Draw("same");

 c5_2->cd();
 c5_2->SetLogy(0);
 hd_Wp_PtLep_2->SetNameTitle("hd_Wp_PtLep","data (W+) - [Z^{0}->ee and W^{+}->#tau#tau subtracted]");
 hd_Wp_PtLep_2->Draw();

 c5->Print(outPath + "/plot_5.eps");
 c5->Print(outPath + "/plot_5.png");


 TCanvas *c5b = new TCanvas("c5b","",800,400);

 c5b-> SetTitle("W+ sample");
 c5b->Divide(2,1);

 c5b_1->cd();
 c5b_1->SetLogy(0);
 hZ_Wp_PtLepPt10_2-> SetFillStyle(3448);
 hZ_Wp_PtLepPt10_2-> SetFillColor(kGreen);
 hd_Wp_PtLepPt10_1->Draw();
 hZ_Wp_PtLepPt10_2-> SetStats(0);
 hWptt_Wp_PtLepPt10_2-> SetFillStyle(3446);
 hWptt_Wp_PtLepPt10_2-> SetFillColor(kRed);
 hWptt_Wp_PtLepPt10_2-> SetStats(0);
 hWptt_Wp_PtLepPt10_2->Draw("same");
 hZ_Wp_PtLepPt10_2->Draw("same");

 c5b_2->cd();
 c5b_2->SetLogy(0);
 hd_Wp_PtLepPt10_2->SetNameTitle("hd_Wp_PtLepPt10","data (W+) - [Z^{0}->ee and W^{+}->#tau#tau subtracted]");
 hd_Wp_PtLepPt10_2->Draw();

 c5b->Print(outPath + "/plot_5b.eps");
 c5b->Print(outPath + "/plot_5b.png");


 // W- sample

 TCanvas *c6 = new TCanvas("c6","",800,800);

 hZ_Wm_PtLep->Scale(scaleZ);
 hWmtt_Wm_PtLep->Scale(scaleWmtt);
 hd_Wm_PtLep->Add(hd_Wm_PtLep,hZ_Wm_PtLep,1,-1); 
 hd_Wm_PtLep->Add(hd_Wm_PtLep,hWmtt_Wm_PtLep,1,-1);

 TH1F *hZ_Wm_PtLep_2 = (TH1F*)hZ_Wm_PtLep->Clone("hZ_Wm_PtLep_2"); 
 TH1F *hWmtt_Wm_PtLep_2 = (TH1F*)hWmtt_Wm_PtLep->Clone("hWmtt_Wm_PtLep_2");
 TH1F *hd_Wm_PtLep_2 = (TH1F*)hd_Wm_PtLep->Clone("hd_Wm_PtLep_2");

 c6-> SetTitle("W- sample");
 c6->Divide(2,2);
 c6_1->cd();
 c6_1->SetLogy(1);
 hZ_Wm_PtLep_2-> SetFillStyle(3448);
 hZ_Wm_PtLep_2-> SetFillColor(kGreen);
 hd_Wm_PtLep_1->Draw();
 hZ_Wm_PtLep_2-> SetStats(0);
 hWmtt_Wm_PtLep_2-> SetFillStyle(3446);
 hWmtt_Wm_PtLep_2-> SetFillColor(kRed);
 hWmtt_Wm_PtLep_2-> SetStats(0);
 hWmtt_Wm_PtLep_2->Draw("same");
 hZ_Wm_PtLep_2->Draw("same");

 c6_2->cd();
 c6_2->SetLogy(1);
 hZ_Wm_PtLep_2-> SetStats(1);

 hZ_Wm_PtLep_2->SetNameTitle("hZ_Wm_PtLep_2","Z^{0}->ee - norm. to data lumi");
 hZ_Wm_PtLep_2->Draw();

 c6_3->cd();
 c6_3->SetLogy(1);
 hWmtt_Wm_PtLep_2-> SetStats(1);
 hWmtt_Wm_PtLep_2->SetNameTitle("hWptt_Wm_PtLep_2","W^{-}->#tau#tau - norm. to data lumi");
 hWmtt_Wm_PtLep_2-> SetFillColor(kRed);
 hWmtt_Wm_PtLep_2->Draw();


 c6->Print(outPath + "/plot_6.eps");
 c6->Print(outPath + "/plot_6.png");

 TCanvas *c6b = new TCanvas("c6b","",800,800);

 hZ_Wm_PtLepPt10->Scale(scaleZ);
 hWmtt_Wm_PtLepPt10->Scale(scaleWmtt);
 hd_Wm_PtLepPt10->Add(hd_Wm_PtLepPt10,hZ_Wm_PtLepPt10,1,-1); 
 hd_Wm_PtLepPt10->Add(hd_Wm_PtLepPt10,hWmtt_Wm_PtLepPt10,1,-1);

 TH1F *hZ_Wm_PtLepPt10_2 = (TH1F*)hZ_Wm_PtLepPt10->Clone("hZ_Wm_PtLepPt10_2"); 
 TH1F *hWmtt_Wm_PtLepPt10_2 = (TH1F*)hWmtt_Wm_PtLepPt10->Clone("hWmtt_Wm_PtLepPt10_2");
 TH1F *hd_Wm_PtLepPt10_2 = (TH1F*)hd_Wm_PtLepPt10->Clone("hd_Wm_PtLepPt10_2");

 c6b-> SetTitle("W- sample");
 c6b->Divide(2,2);
 c6b_1->cd();
 c6b_1->SetLogy(1);
 hZ_Wm_PtLepPt10_2-> SetFillStyle(3448);
 hZ_Wm_PtLepPt10_2-> SetFillColor(kGreen);
 hd_Wm_PtLepPt10_1->Draw();
 hZ_Wm_PtLepPt10_2-> SetStats(0);
 hWmtt_Wm_PtLepPt10_2-> SetFillStyle(3446);
 hWmtt_Wm_PtLepPt10_2-> SetFillColor(kRed);
 hWmtt_Wm_PtLepPt10_2-> SetStats(0);
 hWmtt_Wm_PtLepPt10_2->Draw("same");
 hZ_Wm_PtLepPt10_2->Draw("same");

 c6b_2->cd();
 c6b_2->SetLogy(1);
 hZ_Wm_PtLepPt10_2-> SetStats(1);

 hZ_Wm_PtLepPt10_2->SetNameTitle("hZ_Wm_PtLepPt10_2","Z^{0}->ee - norm. to data lumi");
 hZ_Wm_PtLepPt10_2->Draw();

 c6b_3->cd();
 c6b_3->SetLogy(1);
 hWmtt_Wm_PtLepPt10_2-> SetStats(1);
 hWmtt_Wm_PtLepPt10_2->SetNameTitle("hWptt_Wm_PtLepPt10_2","W^{-}->#tau#tau - norm. to data lumi");
 hWmtt_Wm_PtLepPt10_2-> SetFillColor(kRed);
 hWmtt_Wm_PtLepPt10_2->Draw();


 c6b->Print(outPath + "/plot_6b.eps");
 c6b->Print(outPath + "/plot_6b.png");


 TCanvas *c7 = new TCanvas("c7","",800,400);

 c7-> SetTitle("W- sample");
 c7->Divide(2,1);

 c7_1->cd(); 
 c7_1->SetLogy(1);
 hZ_Wm_PtLep_2-> SetFillStyle(3448);
 hZ_Wm_PtLep_2-> SetFillColor(kGreen);
 hd_Wm_PtLep_1->Draw();
 hZ_Wm_PtLep_2-> SetStats(0);
 hWmtt_Wm_PtLep_2-> SetFillStyle(3446);
 hWmtt_Wm_PtLep_2-> SetFillColor(kRed);
 hWmtt_Wm_PtLep_2-> SetStats(0);
 hZ_Wm_PtLep_2->Draw("same");
 hWmtt_Wm_PtLep_2->Draw("same");

 c7_2->cd();
 c7_2->SetLogy(1);
 hd_Wm_PtLep_2->SetNameTitle("hd_Wm_PtLep","data (W-) - [Z^{0}->ee and W^{-}->#tau#tau subtracted]");
 hd_Wm_PtLep_2->Draw();

 c7->Print(outPath + "/plot_7.eps");
 c7->Print(outPath + "/plot_7.png");


 TCanvas *c7b = new TCanvas("c7b","",800,400);

 c7b-> SetTitle("W- sample");
 c7b->Divide(2,1);

 c7b_1->cd(); 
 c7b_1->SetLogy(0);
 hZ_Wm_PtLepPt10_2-> SetFillStyle(3448);
 hZ_Wm_PtLepPt10_2-> SetFillColor(kGreen);
 hd_Wm_PtLepPt10_1->Draw();
 hZ_Wm_PtLepPt10_2-> SetStats(0);
 hWmtt_Wm_PtLepPt10_2-> SetFillStyle(3446);
 hWmtt_Wm_PtLepPt10_2-> SetFillColor(kRed);
 hWmtt_Wm_PtLepPt10_2-> SetStats(0);
 hZ_Wm_PtLepPt10_2->Draw("same");
 hWmtt_Wm_PtLepPt10_2->Draw("same");

 c7b_2->cd();
 c7b_2->SetLogy(0);
 hd_Wm_PtLepPt10_2->SetNameTitle("hd_Wm_PtLepPt10","data (W-) - [Z^{0}->ee and W^{-}->#tau#tau subtracted]");
 hd_Wm_PtLepPt10_2->Draw();

 c7b->Print(outPath + "/plot_7b.eps");
 c7b->Print(outPath + "/plot_7b.png");


 // Calculate Signal/Bkgnd ratios and their errors:


 float hd_PtLep_1_integral= hd_PtLep_1->Integral();
 float hZ_PtLep_2_integral= hZ_PtLep_2->Integral();
 float hZ_PtLep_1_integral= hZ_PtLep_1->Integral();
 float hZ_PtLep_2_integral_err= scaleZ*sqrt(hZ_PtLep_1_integral);
 float hWptt_PtLep_2_integral= hWptt_PtLep_2->Integral();
 float hWptt_PtLep_1_integral= hWptt_PtLep_1->Integral();
 float hWptt_PtLep_2_integral_err= scaleWptt*sqrt(hWptt_PtLep_1_integral);
 float hWmtt_PtLep_2_integral= hWmtt_PtLep_2->Integral();
 float hWmtt_PtLep_1_integral= hWmtt_PtLep_1->Integral();
 float hWmtt_PtLep_2_integral_err= scaleWmtt*sqrt(hWmtt_PtLep_1_integral);
 float hZ_PtLep_2_BoS= hZ_PtLep_2_integral/hd_PtLep_1_integral;
 float hZ_PtLep_2_BoS_err= hZ_PtLep_2_integral_err/hd_PtLep_1_integral;
 float hWptt_PtLep_2_BoS= hWptt_PtLep_2_integral/hd_PtLep_1_integral;
 float hWptt_PtLep_2_BoS_err= hWptt_PtLep_2_integral_err/hd_PtLep_1_integral;
 float hWmtt_PtLep_2_BoS= hWmtt_PtLep_2_integral/hd_PtLep_1_integral;
 float hWmtt_PtLep_2_BoS_err= hWmtt_PtLep_2_integral_err/hd_PtLep_1_integral;

 float hd_Wp_PtLep_1_integral= hd_Wp_PtLep_1->Integral();
 float hZ_Wp_PtLep_2_integral= hZ_Wp_PtLep_2->Integral();
 float hZ_Wp_PtLep_1_integral= hZ_Wp_PtLep_1->Integral();
 float hZ_Wp_PtLep_2_integral_err= scaleZ*sqrt(hZ_Wp_PtLep_1_integral);
 float hWptt_Wp_PtLep_2_integral= hWptt_Wp_PtLep_2->Integral();
 float hWptt_Wp_PtLep_1_integral= hWptt_Wp_PtLep_1->Integral();
 float hWptt_Wp_PtLep_2_integral_err= scaleWptt*sqrt(hWptt_Wp_PtLep_1_integral);
 float hZ_Wp_PtLep_2_BoS= hZ_Wp_PtLep_2_integral/hd_Wp_PtLep_1_integral;
 float hZ_Wp_PtLep_2_BoS_err= hZ_Wp_PtLep_2_integral_err/hd_Wp_PtLep_1_integral;
 float hWptt_Wp_PtLep_2_BoS= hWptt_Wp_PtLep_2_integral/hd_Wp_PtLep_1_integral;
 float hWptt_Wp_PtLep_2_BoS_err= hWptt_Wp_PtLep_2_integral_err/hd_Wp_PtLep_1_integral;

 float hd_Wm_PtLep_1_integral= hd_Wm_PtLep_1->Integral();
 float hZ_Wm_PtLep_2_integral= hZ_Wm_PtLep_2->Integral();
 float hZ_Wm_PtLep_1_integral= hZ_Wm_PtLep_1->Integral();
 float hZ_Wm_PtLep_2_integral_err= scaleZ*sqrt(hZ_Wm_PtLep_1_integral);
 float hWmtt_Wm_PtLep_2_integral= hWmtt_Wm_PtLep_2->Integral();
 float hWmtt_Wm_PtLep_1_integral= hWmtt_Wm_PtLep_1->Integral();
 float hWmtt_Wm_PtLep_2_integral_err= scaleWmtt*sqrt(hWmtt_Wm_PtLep_1_integral);
 float hZ_Wm_PtLep_2_BoS= hZ_Wm_PtLep_2_integral/hd_Wm_PtLep_1_integral;
 float hZ_Wm_PtLep_2_BoS_err= hZ_Wm_PtLep_2_integral_err/hd_Wm_PtLep_1_integral;
 float hWmtt_Wm_PtLep_2_BoS= hWmtt_Wm_PtLep_2_integral/hd_Wm_PtLep_1_integral;
 float hWmtt_Wm_PtLep_2_BoS_err= hWmtt_Wm_PtLep_2_integral_err/hd_Wm_PtLep_1_integral;


 cout.precision(2);

 cout << "************************************************ " << endl;
 cout << "------------ " << endl;
 cout << "TOTAL SAMPLE " << endl;
 cout << "------------ " << endl;
 cout << "Signal events    = " << hd_PtLep_1_integral << endl;
 cout << "Z0 -> ee events  = " << hZ_PtLep_2_integral << " +- " << hZ_PtLep_2_integral_err << endl << endl;
 cout << "Z0 -> ee B/S     = " << 100*hZ_PtLep_2_BoS << "%" << " +- " << 100*hZ_PtLep_2_BoS_err << "%" << endl;
 cout << "W+ -> tau events = " << hWptt_PtLep_2_integral << " +- " << hWptt_PtLep_2_integral_err << endl;
 cout << "W+ -> tau B/S    = " << 100*hWptt_PtLep_2_BoS << "%" << " +- " << 100*hWptt_PtLep_2_BoS_err << "%" << endl;
 cout << "W- -> tau events = " << hWmtt_PtLep_2_integral << " +- " << hWmtt_PtLep_2_integral_err << endl;
 cout << "W- -> tau B/S    = " << 100*hWmtt_PtLep_2_BoS << "%" << " +- " << 100*hWmtt_PtLep_2_BoS_err << "%" << endl;

 cout << " " << endl;
 cout << " " << endl;

 cout << "---------------------- " << endl;
 cout << "POSITIVE CHARGE SAMPLE " << endl;
 cout << "---------------------- " << endl;
 cout << "Signal events    = " << hd_Wp_PtLep_1_integral << endl;
 cout << "Z0 -> ee events  = " << hZ_Wp_PtLep_2_integral << " +- " << hZ_Wp_PtLep_2_integral_err << endl << endl;
 cout << "Z0 -> ee B/S     = " << 100*hZ_Wp_PtLep_2_BoS << "%" << " +- " << 100*hZ_Wp_PtLep_2_BoS_err << "%" << endl;
 cout << "W+ -> tau events = " << hWptt_Wp_PtLep_2_integral << " +- " << hWptt_Wp_PtLep_2_integral_err << endl;
 cout << "W+ -> tau B/S    = " << 100*hWptt_Wp_PtLep_2_BoS << "%" << " +- " << 100*hWptt_Wp_PtLep_2_BoS_err << "%" << endl;

 cout << " " << endl;
 cout << " " << endl;

 cout << "---------------------- " << endl;
 cout << "NEGATIVE CHARGE SAMPLE " << endl;
 cout << "---------------------- " << endl;
 cout << "Signal events    = " << hd_Wm_PtLep_1_integral << endl;
 cout << "Z0 -> ee events  = " << hZ_Wm_PtLep_2_integral << " +- " << hZ_Wm_PtLep_2_integral_err << endl << endl;
 cout << "Z0 -> ee B/S     = " << 100*hZ_Wm_PtLep_2_BoS << "%" << " +- " << 100*hZ_Wm_PtLep_2_BoS_err << "%" << endl;
 cout << "W- -> tau events = " << hWmtt_Wm_PtLep_2_integral << " +- " << hWmtt_Wm_PtLep_2_integral_err << endl;
 cout << "W- -> tau B/S    = " << 100*hWmtt_Wm_PtLep_2_BoS << "%" << " +- " << 100*hWmtt_Wm_PtLep_2_BoS_err << "%" << endl;
 cout << "************************************************ " << endl;



 TCanvas *c8 = new TCanvas("c8","W TOTAL - QCD data driven",800,400);

 c8->Divide(2,1);

 c8_1->cd();
 hd_PtLepPt10_2->Draw();
 c8_2->cd();
 hd_PtLepQCD->Draw();

 c8->Print(outPath + "/plot_8.eps");
 c8->Print(outPath + "/plot_8.png");


 // plot the Q over Pt distributions:


 TCanvas *c9 = new TCanvas("c9","",800,400);

 c9-> SetTitle("Q/Pt");
 c9->Divide(2,1);

 c9_1->cd(); 
 hd_QxEtoPt_Vs_Et->Draw();
 //TLine *line1 = new TLine(0,0.4,c8_1->GetFrame()->GetX2(),0.4);
 TLine *line1 = new TLine(0,0.4,80,0.4);
 line1->SetLineColor(kRed);
 line1->SetLineWidth(2);
 line1->Draw();
 line1->DrawLine(0,-0.4,80,-0.4);
 line1->DrawLine(0,1.8,80,1.8);
 line1->DrawLine(0,-1.4,80,-1.4);
 c9_2->cd(); 
 hd_QxEtoPt_Vs_Et_passCut->Draw();

 c9->Print(outPath + "/plot_9.eps");
 c9->Print(outPath + "/plot_9.png");


}
