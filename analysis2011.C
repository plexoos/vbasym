void analysis2011()
{
   std::cout.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed

   //gROOT->Reset();

   // Load the libraries:
   //gROOT->Macro("/star/u/fazio/offline/users/fazio/vbasym/macros/loadLibs.C");

   //TString inPath  = "/star/u/fazio/offline/users/fazio/vbasym/";
   TString inPath  = "~/vbasym_results/root_hists/";
   TString outPath = "~/vbasym_results/plots/";

   //Styles:
   gStyle->SetPadBottomMargin(0.15);
   //gStyle->SetPadLeftMargin(0.13);
   gStyle->SetPadLeftMargin(0.17);
   gStyle->SetStatFontSize(0.05);
   gStyle->SetOptStat("mruoi");
   gStyle->SetPadRightMargin(0.25);
   gStyle->SetMarkerStyle(20);

   //float lumiDataTot       = 24.28; // pb-1
   float lumiDataTot       = 24.42; // pb-1
   //float lumiDataEff       = 23.86; // pb-1
   float lumiDataEff       = 23.99; // pb-1
   float lumiMC_Z          = 955.15; // pb-1
   float lumiMC_WpToTauTau = 2136.49; // pb-1
   float lumiMC_WmToTauTau = 1995.34; // pb-1

   float scaleZ    = lumiDataEff / lumiMC_Z;
   float scaleWptt = lumiDataEff / lumiMC_WpToTauTau;
   float scaleWmtt = lumiDataEff / lumiMC_WmToTauTau;

   cout.precision(3);
   cout << "*****************************************" << endl;
   cout << "SCALE FACTORS" << endl;
   cout << "Z0 lumi scale factor =" << scaleZ << endl;
   cout << "Wp->tt lumi scale factor =" << scaleWptt << endl;
   cout << "Wm->tt lumi scale factor =" << scaleWmtt << endl;
   cout << "*****************************************" << endl;

   // open histogram files
   TFile *fileData   = TFile::Open(inPath + "vbana_cut05_data_final.root");
   TFile *fileMCWp   = TFile::Open(inPath + "vbana_cut05_mc_wp.root");
   TFile *fileMCWm   = TFile::Open(inPath + "vbana_cut05_mc_wm.root");
   TFile *fileMCQCD  = TFile::Open(inPath + "vbana_cut05_mc_qcd.root");
   TFile *fileMCZ    = TFile::Open(inPath + "vbana_cut05_mc_z_to_ee.root");
   TFile *fileMCWptt = TFile::Open(inPath + "vbana_cut05_mc_wp_to_tt.root");
   TFile *fileMCWmtt = TFile::Open(inPath + "vbana_cut05_mc_wm_to_tt.root");


   // W total (W+ + W-)
   TH1 *hd_PtLepTPC     = (TH1*) fileData->Get("track_cand_pass_final_QEToPT/hTrackPt");
   TH1 *hd_PtLep        = (TH1*) fileData->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hZ_PtLep        = (TH1*) fileMCZ->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWp_PtLep       = (TH1*) fileMCWp->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWm_PtLep       = (TH1*) fileMCWm->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWptt_PtLep     = (TH1*) fileMCWptt->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWmtt_PtLep     = (TH1*) fileMCWmtt->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hQ_PtLep        = (TH1*) fileMCQCD->Get("track_cand_pass_final_QEToPT/hEcalScaledPt");

   TH1 *hd_PtLepTPCPt15 = (TH1*) fileData->Get("track_cand_pass_final_QEToPT_Pt>15/hTrackPt");
   TH1 *hd_PtLepPt15    = (TH1*) fileData->Get("track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hZ_PtLepPt15    = (TH1*) fileMCZ->Get("track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWp_PtLepPt15   = (TH1*) fileMCWp->Get("track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWm_PtLepPt15   = (TH1*) fileMCWm->Get("track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWptt_PtLepPt15 = (TH1*) fileMCWptt->Get("track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWmtt_PtLepPt15 = (TH1*) fileMCWmtt->Get("track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hQ_PtLepPt15    = (TH1*) fileMCQCD->Get("track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");

   TH1 *hd_PtLepQCD     = (TH1*) fileData->Get("track_cand_pass_qcd_QEToPT/hEcalScaledPt");
   TH1 *hd_PtLepQCDPt15 = (TH1*) fileData->Get("track_cand_pass_qcd_QEToPT_Pt>15/hEcalScaledPt");

   hd_PtLep->Rebin(3);  // 3 GeV per bin (originally 1 GeV)
   hZ_PtLep->Rebin(3);
   hWp_PtLep->Rebin(3);
   hWm_PtLep->Rebin(3);
   hWptt_PtLep->Rebin(3);
   hWmtt_PtLep->Rebin(3);
   hQ_PtLep->Rebin(3);
   hd_PtLepQCD->Rebin(3);
   hd_PtLepPt15->Rebin(3);
   hZ_PtLepPt15->Rebin(3);
   hWp_PtLepPt15->Rebin(3);
   hWm_PtLepPt15->Rebin(3);
   hWptt_PtLepPt15->Rebin(3);
   hWmtt_PtLepPt15->Rebin(3);
   hQ_PtLepPt15->Rebin(3);
   hd_PtLepQCDPt15->Rebin(3);


   TH2 *hd_QxEtoPt_Vs_Et = (TH2 *)fileData->Get("track_cand_pass_final/hQxEtoPt_Vs_Et_PrimaryTrack");
   cout << hd_QxEtoPt_Vs_Et->ClassName() << endl;
   TH2 *hd_QxEtoPt_Vs_Et_passCut = (TH2 *)fileData->Get("track_cand_pass_final_QEToPT/hQxEtoPt_Vs_Et_PrimaryTrack");


   TH2 *hd_PtNeutrinoVsEle        = (TH2*) fileData->Get("event_pass_final_QEToPT/hPtBalanceTracksNeutralsVsElecEt");
   TH2 *hd_PtNeutrinoVsEleNocut   = (TH2*) fileData->Get("event_has_candidate_pt>15/hPtBalanceTracksNeutralsVsElecEt");
   TH1 *hd_PtWReco                = (TH1*) fileData->Get("event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt");

   TH1 *hWp_PtWReco               = (TH1*) fileMCWp->Get("event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_PtWReco_zoomin        = (TH1*) fileMCWp->Get("event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWp_PtWGen                = (TH1*) fileMCWp->Get("event_mc/hWBosonPt");
   TH1 *hWp_PtWGen_zoomin         = (TH1*) fileMCWp->Get("event_mc/hWBosonPt_zoomin");
   TH1 *hWp_PtWGenOverReco        = (TH1*) hWp_PtWReco->Clone("hWp_PtWGenOverReco");
   TH1 *hWp_PtWGenOverReco_zoomin = (TH1*) hWp_PtWReco_zoomin->Clone("hWp_PtWGenOverReco_zoomin");

   hWp_PtWGenOverReco->Divide(hWp_PtWGen, hWp_PtWReco);
   hWp_PtWGenOverReco_zoomin->Divide(hWp_PtWGen_zoomin, hWp_PtWReco_zoomin);

   TH1 *hd_PtWRecoCorrected = (TH1 *)hd_PtWReco->Clone("hd_PtWRecoCorrected");
   hd_PtWRecoCorrected->Multiply(hd_PtWReco, hWp_PtWGenOverReco, 1, 1);

   TH1 *hWm_PtWReco = (TH1*) fileMCWm->Get("event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt");
   TH1 *hWm_PtWGen  = (TH1*) fileMCWm->Get("event_mc/hWBosonPt");

   TH1 *hWm_PtWRecoOverGen = (TH1*) hWm_PtWReco->Clone("hWm_PtWRecoOverGen");
   hWm_PtWRecoOverGen->Divide(hWm_PtWReco, hWm_PtWGen, 1, 1);

   TH1 *hWp_PtRecoil_vs_PtWGen = (TH1 *)fileMCWp->Get("event_mc_pass_final_QEToPT/hTrackRecoilTpcNeutralsPtVsWBosonPt");

   TH1 *hWp_PhiRecoil_vs_PhiWGen = (TH1 *)fileMCWp->Get("event_mc_pass_final_QEToPT/hTrackRecoilPhiVsWBosonPhi");


   // W+
   TH1 *hd_Wp_PtLep        = (TH1 *)fileData->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hZ_Wp_PtLep        = (TH1 *)fileMCZ->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWp_Wp_PtLep       = (TH1 *)fileMCWp->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWm_Wp_PtLep       = (TH1 *)fileMCWm->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWptt_Wp_PtLep     = (TH1 *)fileMCWptt->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hQ_Wp_PtLep        = (TH1 *)fileMCQCD->Get("W+_track_cand_pass_final_QEToPT/hEcalScaledPt");

   TH1 *hd_Wp_PtLepPt15    = (TH1 *)fileData->Get("W+_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hZ_Wp_PtLepPt15    = (TH1 *)fileMCZ->Get("W+_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWp_Wp_PtLepPt15   = (TH1 *)fileMCWp->Get("W+_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWm_Wp_PtLepPt15   = (TH1 *)fileMCWm->Get("W+_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWptt_Wp_PtLepPt15 = (TH1 *)fileMCWptt->Get("W+_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hQ_Wp_PtLepPt15    = (TH1 *)fileMCQCD->Get("W+_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");

   TH1 *hd_Wp_PtLepQCD     = (TH1 *)fileData->Get("W+_track_cand_pass_qcd_QEToPT/hEcalScaledPt");
   TH1 *hd_Wp_PtLepQCDPt15 = (TH1 *)fileData->Get("W+_track_cand_pass_qcd_QEToPT_Pt>15/hEcalScaledPt");

   hd_Wp_PtLep->Rebin(3);
   hZ_Wp_PtLep->Rebin(3);
   hWp_Wp_PtLep->Rebin(3);
   hWm_Wp_PtLep->Rebin(3);
   hWptt_Wp_PtLep->Rebin(3);
   hQ_Wp_PtLep->Rebin(3);
   hd_Wp_PtLepQCD->Rebin(3);
   hd_Wp_PtLepPt15->Rebin(3);
   hZ_Wp_PtLepPt15->Rebin(3);
   hWp_Wp_PtLepPt15->Rebin(3);
   hWm_Wp_PtLepPt15->Rebin(3);
   hWptt_Wp_PtLepPt15->Rebin(3);
   hQ_Wp_PtLepPt15->Rebin(3);
   hd_Wp_PtLepQCDPt15->Rebin(3);

   TH1 *hd_Wp_PtWReco                = (TH1*) fileData->Get("W+_event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_Wp_PtWReco               = (TH1*) fileMCWp->Get("W+_event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_Wp_PtWReco_zoomin        = (TH1*) fileMCWp->Get("W+_event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt_zoomin");

   TH1 *hWp_Wp_PtWGenOverReco        = (TH1*) hWp_Wp_PtWReco->Clone("hWp_Wp_PtWGenOverReco");
   TH1 *hWp_Wp_PtWGenOverReco_zoomin = (TH1*) hWp_Wp_PtWReco_zoomin->Clone("hWp_Wp_PtWGenOverReco_zoomin");

   hWp_Wp_PtWGenOverReco->Divide(hWp_PtWGen, hWp_Wp_PtWReco, 1, 1);
   hWp_Wp_PtWGenOverReco_zoomin->Divide(hWp_PtWGen_zoomin, hWp_Wp_PtWReco_zoomin, 1, 1);

   TH1 *hd_Wp_PtWRecoCorrected = (TH1*) hd_Wp_PtWReco->Clone("hd_Wp_PtWRecoCorrected");
   hd_Wp_PtWRecoCorrected->Multiply(hd_Wp_PtWReco, hWp_Wp_PtWGenOverReco, 1, 1);
   hd_Wp_PtWRecoCorrected->Scale(hd_Wp_PtWReco->Integral()/hd_Wp_PtWRecoCorrected->Integral());

   // W-
   TH1 *hd_Wm_PtLep        = (TH1 *)fileData->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hZ_Wm_PtLep        = (TH1 *)fileMCZ->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWp_Wm_PtLep       = (TH1 *)fileMCWp->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWm_Wm_PtLep       = (TH1 *)fileMCWm->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hWmtt_Wm_PtLep     = (TH1 *)fileMCWmtt->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");
   TH1 *hQ_Wm_PtLep        = (TH1 *)fileMCQCD->Get("W-_track_cand_pass_final_QEToPT/hEcalScaledPt");

   TH1 *hd_Wm_PtLepPt15    = (TH1 *)fileData->Get("W-_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hZ_Wm_PtLepPt15    = (TH1 *)fileMCZ->Get("W-_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWp_Wm_PtLepPt15   = (TH1 *)fileMCWp->Get("W-_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWm_Wm_PtLepPt15   = (TH1 *)fileMCWm->Get("W-_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hWmtt_Wm_PtLepPt15 = (TH1 *)fileMCWmtt->Get("W-_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");
   TH1 *hQ_Wm_PtLepPt15    = (TH1 *)fileMCQCD->Get("W-_track_cand_pass_final_QEToPT_Pt>15/hEcalScaledPt");

   TH1 *hd_Wm_PtLepQCD     = (TH1 *)fileData->Get("W-_track_cand_pass_qcd_QEToPT/hEcalScaledPt");
   TH1 *hd_Wm_PtLepQCDPt15 = (TH1 *)fileData->Get("W-_track_cand_pass_qcd_QEToPT_Pt>15/hEcalScaledPt");

   hd_Wm_PtLep->Rebin(3);
   hZ_Wm_PtLep->Rebin(3);
   hWp_Wm_PtLep->Rebin(3);
   hWm_Wm_PtLep->Rebin(3);
   hWmtt_Wm_PtLep->Rebin(3);
   hQ_Wm_PtLep->Rebin(3);
   hd_Wm_PtLepQCD->Rebin(3);
   hd_Wm_PtLepPt15->Rebin(3);
   hZ_Wm_PtLepPt15->Rebin(3);
   hWp_Wm_PtLepPt15->Rebin(3);
   hWm_Wm_PtLepPt15->Rebin(3);
   hWmtt_Wm_PtLepPt15->Rebin(3);
   hQ_Wm_PtLepPt15->Rebin(3);
   hd_Wm_PtLepQCDPt15->Rebin(3);

   TH1 *hd_Wm_PtWReco = (TH1 *)fileData->Get("W-_event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt");

   TH1 *hWm_Wm_PtWReco = (TH1 *)fileMCWm->Get("W-_event_pass_final_QEToPT/hTrackRecoilWithNeutralsPt");

   TH1 *hWm_Wm_PtWGenOverReco = (TH1 *)hWm_Wm_PtWReco->Clone("hWm_Wm_PtWGenOverReco");
   hWm_Wm_PtWGenOverReco->Divide(hWm_PtWGen, hWm_Wm_PtWReco, 1, 1);

   TH1 *hd_Wm_PtWRecoCorrected = (TH1 *)hd_Wm_PtWReco->Clone("hd_Wm_PtWRecoCorrected");
   hd_Wm_PtWRecoCorrected->Multiply(hd_Wm_PtWReco, hWm_Wm_PtWGenOverReco, 1, 1);
   hd_Wm_PtWRecoCorrected->Scale(hd_Wm_PtWReco->Integral()/hd_Wm_PtWRecoCorrected->Integral());


   //-----------------------------------------------------------------------------------------------------------------

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
   hd_PtLepTPCPt15->GetXaxis()->SetTitle("P_{T}^{lep}(TPC)");
   hd_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hZ_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWptt_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWmtt_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hQ_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hd_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hZ_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWp_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWm_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWptt_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hQ_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hd_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hZ_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWp_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWm_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hWmtt_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");
   hQ_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{lep}");

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
   hd_PtLepTPCPt15->GetYaxis()->SetTitle("events");
   hd_PtLepPt15->GetYaxis()->SetTitle("events");
   hZ_PtLepPt15->GetYaxis()->SetTitle("events");
   hWp_PtLepPt15->GetYaxis()->SetTitle("events");
   hWm_PtLepPt15->GetYaxis()->SetTitle("events");
   hWptt_PtLepPt15->GetYaxis()->SetTitle("events");
   hWmtt_PtLepPt15->GetYaxis()->SetTitle("events");
   hQ_PtLepPt15->GetYaxis()->SetTitle("events");
   hd_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hZ_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hWp_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hWm_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hWptt_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hQ_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hd_Wm_PtLepPt15->GetYaxis()->SetTitle("events");
   hZ_Wm_PtLepPt15->GetYaxis()->SetTitle("events");
   hWp_Wm_PtLepPt15->GetYaxis()->SetTitle("events");
   hWm_Wm_PtLepPt15->GetYaxis()->SetTitle("events");
   hWmtt_Wm_PtLepPt15->GetYaxis()->SetTitle("events");
   hQ_Wm_PtLepPt15->GetYaxis()->SetTitle("events");

   hd_PtLepTPC->SetNameTitle("hd_PtLepTPC", "Data");
   hd_PtLep->SetNameTitle("hd_PtLep", "Data");
   hd_Wp_PtLep->SetNameTitle("hd_Wp_PtLep", "Data (W+)");
   hd_Wm_PtLep->SetNameTitle("hd_Wm_PtLep", "Data (W-)");
   hd_PtLepTPCPt15->SetNameTitle("hd_PtLepTPCPt15", "Data");
   hd_PtLepPt15->SetNameTitle("hd_PtLepPt15", "Data");
   hd_Wp_PtLepPt15->SetNameTitle("hd_Wp_PtLepPt15", "Data (W+)");
   hd_Wm_PtLepPt15->SetNameTitle("hd_Wm_PtLepPt15", "Data (W-)");

   hd_PtLepTPC->SetTitleOffset(1.3, "y");
   hd_PtLep->SetTitleOffset(1.3, "y");
   hd_Wp_PtLep->SetTitleOffset(1.3, "y");
   hd_Wm_PtLep->SetTitleOffset(1.3, "y");
   hd_PtLepTPCPt15->SetTitleOffset(1.3, "y");
   hd_PtLepPt15->SetTitleOffset(1.3, "y");
   hd_Wp_PtLepPt15->SetTitleOffset(1.3, "y");
   hd_Wm_PtLepPt15->SetTitleOffset(1.3, "y");

   TH1F *hd_PtLep_1 = (TH1F *)hd_PtLep->Clone("hd_PtLep_1");
   TH1F *hZ_PtLep_1 = (TH1F *)hZ_PtLep->Clone("hZ_PtLep_1");
   TH1F *hWptt_PtLep_1 = (TH1F *)hWptt_PtLep->Clone("hWptt_PtLep_1");
   TH1F *hWmtt_PtLep_1 = (TH1F *)hWmtt_PtLep->Clone("hWmtt_PtLep_1");
   TH1F *hd_Wp_PtLep_1 = (TH1F *)hd_Wp_PtLep->Clone("hd_Wp_PtLep_1");
   TH1F *hZ_Wp_PtLep_1 = (TH1F *)hZ_Wp_PtLep->Clone("hZ_Wp_PtLep_1");
   TH1F *hWptt_Wp_PtLep_1 = (TH1F *)hWptt_Wp_PtLep->Clone("hWptt_Wp_PtLep_1");
   TH1F *hd_Wm_PtLep_1 = (TH1F *)hd_Wm_PtLep->Clone("hd_Wm_PtLep_1");
   TH1F *hZ_Wm_PtLep_1 = (TH1F *)hZ_Wm_PtLep->Clone("hZ_Wm_PtLep_1");
   TH1F *hWmtt_Wm_PtLep_1 = (TH1F *)hWmtt_Wm_PtLep->Clone("hWmtt_Wm_PtLep_1");

   TH1F *hd_PtLepPt15_1 = (TH1F *)hd_PtLepPt15->Clone("hd_PtLepPt15_1");
   TH1F *hZ_PtLepPt15_1 = (TH1F *)hZ_PtLepPt15->Clone("hZ_PtLepPt15_1");
   TH1F *hWptt_PtLepPt15_1 = (TH1F *)hWptt_PtLepPt15->Clone("hWptt_PtLepPt15_1");
   TH1F *hWmtt_PtLepPt15_1 = (TH1F *)hWmtt_PtLepPt15->Clone("hWmtt_PtLepPt15_1");
   TH1F *hd_Wp_PtLepPt15_1 = (TH1F *)hd_Wp_PtLepPt15->Clone("hd_Wp_PtLepPt15_1");
   TH1F *hZ_Wp_PtLepPt15_1 = (TH1F *)hZ_Wp_PtLepPt15->Clone("hZ_Wp_PtLepPt15_1");
   TH1F *hWptt_Wp_PtLepPt15_1 = (TH1F *)hWptt_Wp_PtLepPt15->Clone("hWptt_Wp_PtLepPt15_1");
   TH1F *hd_Wm_PtLepPt15_1 = (TH1F *)hd_Wm_PtLepPt15->Clone("hd_Wm_PtLepPt15_1");
   TH1F *hZ_Wm_PtLepPt15_1 = (TH1F *)hZ_Wm_PtLepPt15->Clone("hZ_Wm_PtLepPt15_1");
   TH1F *hWmtt_Wm_PtLepPt15_1 = (TH1F *)hWmtt_Wm_PtLepPt15->Clone("hWmtt_Wm_PtLepPt15_1");


   TCanvas *c1 = new TCanvas("c1", "", 800, 800);
   c1->Divide(3, 3);

   c1_1->cd();
   hd_PtLepTPC->Draw();
   c1_2->cd();
   hd_PtLep_1->SetNameTitle("hd_PtLep", "Data");
   hd_PtLep_1->Draw();
   c1_3->cd();
   hZ_PtLep_1->SetNameTitle("hZ_PtLep", "Z^{0}->ee");
   hZ_PtLep_1->Draw();
   c1_4->cd();
   hWp_PtLep->SetNameTitle("hWp_PtLep", "W^{+}->e#nu_{e}");
   hWp_PtLep->Draw();
   c1_5->cd();
   hWm_PtLep->SetNameTitle("hWp_PtLep", "W^{-}->e#nu_{e}");
   hWm_PtLep->Draw();
   c1_6->cd();
   hWptt_PtLep_1->SetNameTitle("hWp_PtLep", "W^{+}->#tau#nu_{#tau}");
   hWptt_PtLep_1->Draw();
   c1_7->cd();
   hWmtt_PtLep_1->SetNameTitle("hWp_PtLep", "W^{-}->#tau#nu_{#tau}");
   hWmtt_PtLep_1->Draw();
   c1_8->cd();
   hQ_PtLep->SetNameTitle("hQ_PtLep", "QCD");
   hQ_PtLep->Draw();

   //c1->Print(outPath + "/plot_1.eps");
   c1->Print(outPath + "/plot_1.png");


   TCanvas *c1b = new TCanvas("c1b", "", 800, 800);
   c1b->Divide(3, 3);

   c1b_1->cd();
   hd_PtLepTPCPt15->Draw();
   c1b_2->cd();
   hd_PtLepPt15_1->SetNameTitle("hd_PtLepPt15", "Data");
   hd_PtLepPt15_1->Draw();
   c1b_3->cd();
   hZ_PtLepPt15_1->SetNameTitle("hZ_PtLepPt15", "Z^{0}->ee");
   hZ_PtLepPt15_1->Draw();
   c1b_4->cd();
   hWp_PtLepPt15->SetNameTitle("hWp_PtLepPt15", "W^{+}->e#nu_{e}");
   hWp_PtLepPt15->Draw();
   c1b_5->cd();
   hWm_PtLepPt15->SetNameTitle("hWp_PtLepPt15", "W^{-}->e#nu_{e}");
   hWm_PtLepPt15->Draw();
   c1b_6->cd();
   hWptt_PtLepPt15_1->SetNameTitle("hWp_PtLepPt15", "W^{+}->#tau#nu_{#tau}");
   hWptt_PtLepPt15_1->Draw();
   c1b_7->cd();
   hWmtt_PtLepPt15_1->SetNameTitle("hWp_PtLepPt15", "W^{-}->#tau#nu_{#tau}");
   hWmtt_PtLepPt15_1->Draw();
   c1b_8->cd();
   hQ_PtLepPt15->SetNameTitle("hQ_PtLepPt15", "QCD");
   hQ_PtLepPt15->Draw();

   //c1b->Print(outPath + "/plot_1b.eps");
   c1b->Print(outPath + "/plot_1b.png");



   TCanvas *c2 = new TCanvas("c2", "W TOTAL sample", 800, 800);

   hZ_PtLep->Scale(scaleZ);
   hWptt_PtLep->Scale(scaleWptt);
   hWmtt_PtLep->Scale(scaleWmtt);
   hd_PtLep->Add(hd_PtLep, hZ_PtLep, 1, -1);
   hd_PtLep->Add(hd_PtLep, hWptt_PtLep, 1, -1);
   hd_PtLep->Add(hd_PtLep, hWmtt_PtLep, 1, -1);

   TH1F *hZ_PtLep_2 = (TH1F *)hZ_PtLep->Clone("hZ_PtLep_2");
   TH1F *hWptt_PtLep_2 = (TH1F *)hWptt_PtLep->Clone("hWptt_PtLep_2");
   TH1F *hWmtt_PtLep_2 = (TH1F *)hWmtt_PtLep->Clone("hWmtt_PtLep_2");
   TH1F *hd_PtLep_2 = (TH1F *)hd_PtLep->Clone("hd_PtLep_2");

   c2->Divide(2, 2);
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
   hZ_PtLep_2->SetNameTitle("hZ_PtLep_2", "Z^{0}->ee - norm. to data lumi");
   hZ_PtLep_2->Draw();

   c2_3->cd();
   c2_3->SetLogy(1);
   hWptt_PtLep_2-> SetStats(1);
   hWptt_PtLep_2->SetNameTitle("hWptt_PtLep_2", "W^{+}->#tau#nu_{#tau} - norm. to data lumi");
   hWptt_PtLep_2-> SetFillColor(kRed);
   hWptt_PtLep_2->Draw();

   c2_4->cd();
   c2_4->SetLogy(1);
   hWmtt_PtLep_2-> SetStats(1);
   hWmtt_PtLep_2->SetNameTitle("hWmtt_PtLep_2", "W^{-}->#tau#nu_{#tau} - norm. to data lumi");
   hWmtt_PtLep_2-> SetFillColor(kBlue);
   hWmtt_PtLep_2->Draw();


   //c2->Print(outPath + "/plot_2.eps");
   c2->Print(outPath + "/plot_2.png");


   TCanvas *c2b = new TCanvas("c2b", "W TOTAL sample", 800, 800);

   hZ_PtLepPt15->Scale(scaleZ);
   hWptt_PtLepPt15->Scale(scaleWptt);
   hWmtt_PtLepPt15->Scale(scaleWmtt);
   hd_PtLepPt15->Add(hd_PtLepPt15, hZ_PtLepPt15, 1, -1);
   hd_PtLepPt15->Add(hd_PtLepPt15, hWptt_PtLepPt15, 1, -1);
   hd_PtLepPt15->Add(hd_PtLepPt15, hWmtt_PtLepPt15, 1, -1);

   TH1F *hZ_PtLepPt15_2 = (TH1F *)hZ_PtLepPt15->Clone("hZ_PtLepPt15_2");
   TH1F *hWptt_PtLepPt15_2 = (TH1F *)hWptt_PtLepPt15->Clone("hWptt_PtLepPt15_2");
   TH1F *hWmtt_PtLepPt15_2 = (TH1F *)hWmtt_PtLepPt15->Clone("hWmtt_PtLepPt15_2");
   TH1F *hd_PtLepPt15_2 = (TH1F *)hd_PtLepPt15->Clone("hd_PtLepPt15_2");

   c2b->Divide(2, 2);
   c2b_1->cd();
   c2b_1->SetLogy(1);
   hZ_PtLepPt15_2-> SetFillStyle(3448);
   hZ_PtLepPt15_2-> SetFillColor(kGreen);
   hd_PtLepPt15_1->Draw();
   hZ_PtLepPt15_2-> SetStats(0);
   hWptt_PtLepPt15_2-> SetFillStyle(3446);
   hWptt_PtLepPt15_2-> SetFillColor(kRed);
   hWptt_PtLepPt15_2-> SetStats(0);
   hWptt_PtLepPt15_2->Draw("same");
   hWmtt_PtLepPt15_2-> SetFillColor(kBlue);
   hWmtt_PtLepPt15_2-> SetStats(0);
   hZ_PtLepPt15_2->Draw("same");
   hWmtt_PtLepPt15_2->Draw("same");

   c2b_2->cd();
   c2b_2->SetLogy(1);
   hZ_PtLepPt15_2-> SetStats(1);
   hZ_PtLepPt15_2->SetNameTitle("hZ_PtLepPt15_2", "Z^{0}->ee - norm. to data lumi");
   hZ_PtLepPt15_2->Draw();

   c2b_3->cd();
   c2b_3->SetLogy(1);
   hWptt_PtLepPt15_2-> SetStats(1);
   hWptt_PtLepPt15_2->SetNameTitle("hWptt_PtLepPt15_2", "W^{+}->#tau#nu_{#tau} - norm. to data lumi");
   hWptt_PtLepPt15_2-> SetFillColor(kRed);
   hWptt_PtLepPt15_2->Draw();

   c2b_4->cd();
   c2b_4->SetLogy(1);
   hWmtt_PtLepPt15_2-> SetStats(1);
   hWmtt_PtLepPt15_2->SetNameTitle("hWmtt_PtLepPt15_2", "W^{-}->#tau#nu_{#tau} - norm. to data lumi");
   hWmtt_PtLepPt15_2-> SetFillColor(kBlue);
   hWmtt_PtLepPt15_2->Draw();


   //c2b->Print(outPath + "/plot_2b.eps");
   c2b->Print(outPath + "/plot_2b.png");


   TCanvas *c2c = new TCanvas("c2c", "W TOTAL - QCD data driven - PtEle > 15 GeV", 800, 400);


   TH1F *hd_PtLepQCDPt15_1 = (TH1F *)hd_PtLepQCDPt15->Clone("hd_PtLepQCDPt15_1");

   Double_t bin0 =  hd_PtLepPt15_2-> GetBinContent(0);
   Double_t bin1 =  hd_PtLepPt15_2-> GetBinContent(1);
   Double_t bin2 =  hd_PtLepPt15_2-> GetBinContent(2);
   Double_t bin3 =  hd_PtLepPt15_2-> GetBinContent(3);
   Double_t bin4 =  hd_PtLepPt15_2-> GetBinContent(4);
   Double_t bin5 =  hd_PtLepPt15_2-> GetBinContent(5);
   Double_t bin6 =  hd_PtLepPt15_2-> GetBinContent(6);
   Double_t bin7 =  hd_PtLepPt15_2-> GetBinContent(7);
   Double_t bin8 =  hd_PtLepPt15_2-> GetBinContent(8);
   Double_t bin9 =  hd_PtLepPt15_2-> GetBinContent(9);
   Double_t bin10 =  hd_PtLepPt15_2-> GetBinContent(10);
   Double_t bin11 =  hd_PtLepPt15_2-> GetBinContent(11);
   Double_t bin12 =  hd_PtLepPt15_2-> GetBinContent(12);
   Double_t bin13 =  hd_PtLepPt15_2-> GetBinContent(13);
   Double_t bin14 =  hd_PtLepPt15_2-> GetBinContent(14);
   Double_t bin15 =  hd_PtLepPt15_2-> GetBinContent(15);
   Double_t bin16 =  hd_PtLepPt15_2-> GetBinContent(16);
   Double_t bin17 =  hd_PtLepPt15_2-> GetBinContent(17);
   Double_t bin18 =  hd_PtLepPt15_2-> GetBinContent(18);
   Double_t bin19 =  hd_PtLepPt15_2-> GetBinContent(19);
   Double_t bin20 =  hd_PtLepPt15_2-> GetBinContent(20);
   Double_t bin21 =  hd_PtLepPt15_2-> GetBinContent(21);

   cout << "PtLep QCD bin0 = " <<  bin 0 << endl;
   cout << "PtLep QCD bin1 = " <<  bin 1 << endl;
   cout << "PtLep QCD bin2 = " <<  bin 2 << endl;
   cout << "PtLep QCD bin3 = " <<  bin 3 << endl;
   cout << "PtLep QCD bin4 = " <<  bin 4 << endl;
   cout << "PtLep QCD bin5 = " <<  bin 5 << endl;
   cout << "PtLep QCD bin6 = " <<  bin 6 << endl;
   cout << "PtLep QCD bin7 = " <<  bin 7 << endl;
   cout << "PtLep QCD bin8 = " <<  bin 8 << endl;
   cout << "PtLep QCD bin9 = " <<  bin 9 << endl;
   cout << "PtLep QCD bin10 = " <<  bin 10 << endl;
   cout << "PtLep QCD bin11 = " <<  bin 11 << endl;
   cout << "PtLep QCD bin12 = " <<  bin 12 << endl;
   cout << "PtLep QCD bin13 = " <<  bin 13 << endl;
   cout << "PtLep QCD bin14 = " <<  bin 14 << endl;
   cout << "PtLep QCD bin15 = " <<  bin 15 << endl;
   cout << "PtLep QCD bin16 = " <<  bin 16 << endl;
   cout << "PtLep QCD bin17 = " <<  bin 17 << endl;
   cout << "PtLep QCD bin18 = " <<  bin 18 << endl;
   cout << "PtLep QCD bin19 = " <<  bin 19 << endl;
   cout << "PtLep QCD bin20 = " <<  bin 20 << endl;
   cout << "PtLep QCD bin21 = " <<  bin 21 << endl;

   // Double_t integral15_18=  hd_PtLepPt15_2-> Integral(15,18);

   // Double_t integralQCD15_18= hd_PtLepQCDPt15-> Integral(15,18);

   Double_t integral5_6 =  hd_PtLepPt15_2-> Integral(5, 6);

   Double_t integralQCD5_6 = hd_PtLepQCDPt15-> Integral(5, 6);

   float scaleQCD = integral5_6 / integralQCD5_6;

   cout << "Integral of bin content in the first bin for Signal = " <<  integral5_6 << endl;

   cout << "Integral of bin content in the first bin for QCD = " <<  integralQCD5_6 << endl;

   cout << "QCD scale factor= " << scaleQCD << endl;

   hd_PtLepQCDPt15->Scale(scaleQCD);

   TH1F *hd_PtLepQCDPt15_2 = (TH1F *)hd_PtLepQCDPt15->Clone("hd_PtLepQCDPt15_2");

   hd_PtLepPt15->Add(hd_PtLepPt15, hd_PtLepQCDPt15, 1, -1);

   TH1F *hd_PtLepPt15_3 = (TH1F *)hd_PtLepPt15->Clone("hd_PtLepPt15_3");
   hd_PtLepPt15_3->SetNameTitle("hd_PtLepPt15_3", "data- Z^{0}->ee, W^{+-}->#tau#nu_{#tau} and QCD subtracted");


   c2c->Divide(2, 1);

   c2c_1->cd();
   // hd_PtLepPt15_2-> SetMinimum(30.);
   hd_PtLepPt15_2->Draw();
   hd_PtLepQCDPt15_2-> SetFillStyle(3448);
   hd_PtLepQCDPt15_2-> SetFillColor(kMagenta);
   hd_PtLepQCDPt15_2->Draw("same");
   c2c_2->cd();
   hd_PtLepPt15_3->Draw();


   //c2c->Print(outPath + "/plot_2c.eps");
   c2c->Print(outPath + "/plot_2c.png");


   TCanvas *c2d = new TCanvas("c2d", "W TOTAL - QCD data driven", 800, 400);


   TH1F *hd_PtLepQCD_1 = (TH1F *)hd_PtLepQCD->Clone("hd_PtLepQCD_1");

   hd_PtLepQCD->Scale(scaleQCD);

   TH1F *hd_PtLepQCD_2 = (TH1F *)hd_PtLepQCD->Clone("hd_PtLepQCD_2");

   hd_PtLep->Add(hd_PtLep, hd_PtLepQCD, 1, -1);

   TH1F *hd_PtLep_3 = (TH1F *)hd_PtLep->Clone("hd_PtLep_3");
   hd_PtLep_3->SetNameTitle("hd_PtLep_3", "data- Z^{0}->ee, W^{+-}->#tau#nu_{#tau} and QCD subtracted");


   c2d->Divide(2, 1);

   c2d_1->cd();
   hd_PtLep_2->Draw();
   hd_PtLepQCD_2-> SetFillStyle(3448);
   hd_PtLepQCD_2-> SetFillColor(kMagenta);
   hd_PtLepQCD_2->Draw("same");
   c2d_2->cd();
   hd_PtLep_3->Draw();


   //c2d->Print(outPath + "/plot_2d.eps");
   c2d->Print(outPath + "/plot_2d.png");


   TCanvas *c3 = new TCanvas("c3", "W TOTAL sample summary", 800, 400);

   c3->Divide(2, 1);

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
   hd_PtLepQCD_2->Draw("same");

   c3_2->cd();
   c3_2->SetLogy(0);
   // hd_PtLep_2->SetNameTitle("hd_PtLep","data- Z^{0}->ee and W^{+-}->#tau#nu_{#tau} subtracted");
   hd_PtLep_3->SetMarkerStyle(20);
   hd_PtLep_3->Draw("E0");

   //c3->Print(outPath + "/plot_3.eps");
   c3->Print(outPath + "/plot_3.png");



   TCanvas *c3c = new TCanvas("c3c", "W TOTAL sample summary - PtEle > 15 GeV", 800, 400);

   c3c->Divide(2, 1);

   c3c_1->cd();
   c3c_1->SetLogy(0);
   hZ_PtLepPt15_2-> SetFillStyle(3448);
   hZ_PtLepPt15_2-> SetFillColor(kGreen);
   hd_PtLepPt15_1->Draw();
   hZ_PtLepPt15_2-> SetStats(0);
   hWptt_PtLepPt15_2-> SetFillStyle(3446);
   hWptt_PtLepPt15_2-> SetFillColor(kRed);
   hWptt_PtLepPt15_2-> SetStats(0);
   hWptt_PtLepPt15_2->Draw("same");
   hWmtt_PtLepPt15_2-> SetFillColor(kBlue);
   hWmtt_PtLepPt15_2-> SetStats(0);
   hZ_PtLepPt15_2->Draw("same");
   hWmtt_PtLepPt15_2->Draw("same");
   hd_PtLepQCDPt15_2->Draw("same");

   c3c_2->cd();
   c3c_2->SetLogy(0);
   //hd_PtLepPt15_2->SetNameTitle("hd_PtLepPt15","data- Z^{0}->ee and W^{+-}->#tau#nu_{#tau} subtracted");
   hd_PtLepPt15_3->SetMarkerStyle(20);
   hd_PtLepPt15_3->Draw("E0");

   //c3c->Print(outPath + "/plot_3c.eps");
   c3c->Print(outPath + "/plot_3c.png");


   // W+ sample

   TCanvas *c4 = new TCanvas("c4", "W+ sample", 800, 800);

   hZ_Wp_PtLep->Scale(scaleZ);
   hWptt_Wp_PtLep->Scale(scaleWptt);
   hd_Wp_PtLep->Add(hd_Wp_PtLep, hZ_Wp_PtLep, 1, -1);
   hd_Wp_PtLep->Add(hd_Wp_PtLep, hWptt_Wp_PtLep, 1, -1);

   TH1F *hZ_Wp_PtLep_2 = (TH1F *)hZ_Wp_PtLep->Clone("hZ_Wp_PtLep_2");
   TH1F *hWptt_Wp_PtLep_2 = (TH1F *)hWptt_Wp_PtLep->Clone("hWptt_Wp_PtLep_2");
   TH1F *hd_Wp_PtLep_2 = (TH1F *)hd_Wp_PtLep->Clone("hd_Wp_PtLep_2");

   c4->Divide(2, 2);
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
   hZ_Wp_PtLep_2->SetNameTitle("hZ_Wp_PtLep_2", "Z^{0}->ee - norm. to data lumi");
   hZ_Wp_PtLep_2->Draw();

   c4_3->cd();
   c4_3->SetLogy(1);
   hWptt_Wp_PtLep_2-> SetStats(1);
   hWptt_Wp_PtLep_2->SetNameTitle("hWptt_Wp_PtLep_2", "W^{+}->#tau#nu_{#tau} - norm. to data lumi");
   hWptt_Wp_PtLep_2-> SetFillColor(kRed);
   hWptt_Wp_PtLep_2->Draw();


   //c4->Print(outPath + "/plot_4.eps");
   c4->Print(outPath + "/plot_4.png");


   TCanvas *c4b = new TCanvas("c4b", "W+ sample - PtEle > 15 GeV", 800, 800);

   hZ_Wp_PtLepPt15->Scale(scaleZ);
   hWptt_Wp_PtLepPt15->Scale(scaleWptt);
   hd_Wp_PtLepPt15->Add(hd_Wp_PtLepPt15, hZ_Wp_PtLepPt15, 1, -1);
   hd_Wp_PtLepPt15->Add(hd_Wp_PtLepPt15, hWptt_Wp_PtLepPt15, 1, -1);

   TH1F *hZ_Wp_PtLepPt15_2 = (TH1F *)hZ_Wp_PtLepPt15->Clone("hZ_Wp_PtLepPt15_2");
   TH1F *hWptt_Wp_PtLepPt15_2 = (TH1F *)hWptt_Wp_PtLepPt15->Clone("hWptt_Wp_PtLepPt15_2");
   TH1F *hd_Wp_PtLepPt15_2 = (TH1F *)hd_Wp_PtLepPt15->Clone("hd_Wp_PtLepPt15_2");

   c4b->Divide(2, 2);
   c4b_1->cd();
   c4b_1->SetLogy(1);
   hZ_Wp_PtLepPt15_2-> SetFillStyle(3448);
   hZ_Wp_PtLepPt15_2-> SetFillColor(kGreen);
   hd_Wp_PtLepPt15_1->Draw();
   hZ_Wp_PtLepPt15_2-> SetStats(0);
   hWptt_Wp_PtLepPt15_2-> SetFillStyle(3446);
   hWptt_Wp_PtLepPt15_2-> SetFillColor(kRed);
   hWptt_Wp_PtLepPt15_2-> SetStats(0);
   hWptt_Wp_PtLepPt15_2->Draw("same");
   hZ_Wp_PtLepPt15_2->Draw("same");

   c4b_2->cd();
   c4b_2->SetLogy(1);
   hZ_Wp_PtLepPt15_2-> SetStats(1);
   hZ_Wp_PtLepPt15_2->SetNameTitle("hZ_Wp_PtLepPt15_2", "Z^{0}->ee - norm. to data lumi");
   hZ_Wp_PtLepPt15_2->Draw();

   c4b_3->cd();
   c4b_3->SetLogy(1);
   hWptt_Wp_PtLepPt15_2-> SetStats(1);
   hWptt_Wp_PtLepPt15_2->SetNameTitle("hWptt_Wp_PtLepPt15_2", "W^{+}->#tau#nu_{#tau} - norm. to data lumi");
   hWptt_Wp_PtLepPt15_2-> SetFillColor(kRed);
   hWptt_Wp_PtLepPt15_2->Draw();


   //c4b->Print(outPath + "/plot_4b.eps");
   c4b->Print(outPath + "/plot_4b.png");


   TCanvas *c4c = new TCanvas("c4c", "W+ sample - QCD data driven - PtEle > 15 GeV", 800, 400);


   TH1F *hd_Wp_PtLepQCDPt15_1 = (TH1F *)hd_Wp_PtLepQCDPt15->Clone("hd_Wp_PtLepQCDPt15_1");

   // Double_t integral_Wp_15_18=  hd_Wp_PtLepPt15_2-> Integral(15,18);

   // Double_t integral_Wp_QCD15_18= hd_Wp_PtLepQCDPt15-> Integral(15,18);

   Double_t integral_Wp_5_6 =  hd_Wp_PtLepPt15_2-> Integral(5, 6);

   Double_t integral_Wp_QCD5_6 = hd_Wp_PtLepQCDPt15-> Integral(5, 6);

   float scale_Wp_QCD = integral_Wp_5_6 / integral_Wp_QCD5_6;

   cout << "W+, Integral of bin content in the first bin for Signal = " <<  integral_Wp_5_6 << endl;

   cout << "W+, Integral of bin content in the first bin for QCD = " <<  integral_Wp_QCD5_6 << endl;

   cout << "W+, QCD scale factor= " << scale_Wp_QCD << endl;

   hd_Wp_PtLepQCDPt15->Scale(scale_Wp_QCD);

   TH1F *hd_Wp_PtLepQCDPt15_2 = (TH1F *)hd_Wp_PtLepQCDPt15->Clone("hd_Wp_PtLepQCDPt15_2");

   hd_Wp_PtLepPt15->Add(hd_Wp_PtLepPt15, hd_Wp_PtLepQCDPt15, 1, -1);

   TH1F *hd_Wp_PtLepPt15_3 = (TH1F *)hd_Wp_PtLepPt15->Clone("hd_Wp_PtLepPt15_3");
   hd_Wp_PtLepPt15_3->SetNameTitle("hd_Wp_PtLepPt15_3", "data- Z^{0}->ee, W^{+}->#tau#nu_{#tau} and QCD subtracted");


   c4c->Divide(2, 1);

   c4c_1->cd();
   // hd_Wp_PtLepPt15_2-> SetMinimum(30.);
   hd_Wp_PtLepPt15_2->Draw();
   hd_Wp_PtLepQCDPt15_2-> SetFillStyle(3448);
   hd_Wp_PtLepQCDPt15_2-> SetFillColor(kMagenta);
   hd_Wp_PtLepQCDPt15_2->Draw("same");
   c4c_2->cd();
   hd_Wp_PtLepPt15_3->Draw();


   //c4c->Print(outPath + "/plot_4c.eps");
   c4c->Print(outPath + "/plot_4c.png");


   TCanvas *c4d = new TCanvas("c4d", "W+ sample - QCD data driven", 800, 400);


   TH1F *hd_Wp_PtLepQCD_1 = (TH1F *)hd_Wp_PtLepQCD->Clone("hd_Wp_PtLepQCD_1");

   hd_Wp_PtLepQCD->Scale(scale_Wp_QCD);

   TH1F *hd_Wp_PtLepQCD_2 = (TH1F *)hd_Wp_PtLepQCD->Clone("hd_Wp_PtLepQCD_2");

   hd_Wp_PtLep->Add(hd_Wp_PtLep, hd_Wp_PtLepQCD, 1, -1);

   TH1F *hd_Wp_PtLep_3 = (TH1F *)hd_Wp_PtLep->Clone("hd_Wp_PtLep_3");
   hd_Wp_PtLep_3->SetNameTitle("hd_Wp_PtLep_3", "data- Z^{0}->ee, W^{+}->#tau#nu_{#tau} and QCD subtracted");


   c4d->Divide(2, 1);

   c4d_1->cd();
   hd_Wp_PtLep_2->Draw();
   hd_Wp_PtLepQCD_2-> SetFillStyle(3448);
   hd_Wp_PtLepQCD_2-> SetFillColor(kMagenta);
   hd_Wp_PtLepQCD_2->Draw("same");
   c4d_2->cd();
   hd_Wp_PtLep_3->Draw();


   //c4d->Print(outPath + "/plot_4d.eps");
   c4d->Print(outPath + "/plot_4d.png");


   TCanvas *c5 = new TCanvas("c5", "", 800, 400);

   c5-> SetTitle("W+ sample summary");
   c5->Divide(2, 1);

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
   // hd_Wp_PtLep_2->SetNameTitle("hd_Wp_PtLep","data (W+) - [Z^{0}->ee and W^{+}->#tau#nu_{#tau} subtracted]");
   hd_Wp_PtLep_3->SetMarkerStyle(20);
   hd_Wp_PtLep_3->Draw("E0");

   //c5->Print(outPath + "/plot_5.eps");
   c5->Print(outPath + "/plot_5.png");


   TCanvas *c5b = new TCanvas("c5b", "", 800, 400);

   c5b-> SetTitle("W+ sample summary - PtEle > 15 GeV");
   c5b->Divide(2, 1);

   c5b_1->cd();
   c5b_1->SetLogy(0);
   hZ_Wp_PtLepPt15_2-> SetFillStyle(3448);
   hZ_Wp_PtLepPt15_2-> SetFillColor(kGreen);
   hd_Wp_PtLepPt15_1->Draw();
   hZ_Wp_PtLepPt15_2-> SetStats(0);
   hWptt_Wp_PtLepPt15_2-> SetFillStyle(3446);
   hWptt_Wp_PtLepPt15_2-> SetFillColor(kRed);
   hWptt_Wp_PtLepPt15_2-> SetStats(0);
   hWptt_Wp_PtLepPt15_2->Draw("same");
   hZ_Wp_PtLepPt15_2->Draw("same");

   c5b_2->cd();
   c5b_2->SetLogy(0);
   //hd_Wp_PtLepPt15_2->SetNameTitle("hd_Wp_PtLepPt15","data (W+) - [Z^{0}->ee and W^{+}->#tau#nu_{#tau} subtracted]");
   hd_Wp_PtLepPt15_3->SetMarkerStyle(20);
   hd_Wp_PtLepPt15_3->Draw("E0");

   //c5b->Print(outPath + "/plot_5b.eps");
   c5b->Print(outPath + "/plot_5b.png");


   // W- sample

   TCanvas *c6 = new TCanvas("c6", "", 800, 800);

   hZ_Wm_PtLep->Scale(scaleZ);
   hWmtt_Wm_PtLep->Scale(scaleWmtt);
   hd_Wm_PtLep->Add(hd_Wm_PtLep, hZ_Wm_PtLep, 1, -1);
   hd_Wm_PtLep->Add(hd_Wm_PtLep, hWmtt_Wm_PtLep, 1, -1);

   TH1F *hZ_Wm_PtLep_2 = (TH1F *)hZ_Wm_PtLep->Clone("hZ_Wm_PtLep_2");
   TH1F *hWmtt_Wm_PtLep_2 = (TH1F *)hWmtt_Wm_PtLep->Clone("hWmtt_Wm_PtLep_2");
   TH1F *hd_Wm_PtLep_2 = (TH1F *)hd_Wm_PtLep->Clone("hd_Wm_PtLep_2");

   c6-> SetTitle("W- sample");
   c6->Divide(2, 2);
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

   hZ_Wm_PtLep_2->SetNameTitle("hZ_Wm_PtLep_2", "Z^{0}->ee - norm. to data lumi");
   hZ_Wm_PtLep_2->Draw();

   c6_3->cd();
   c6_3->SetLogy(1);
   hWmtt_Wm_PtLep_2-> SetStats(1);
   hWmtt_Wm_PtLep_2->SetNameTitle("hWptt_Wm_PtLep_2", "W^{-}->#tau#nu_{#tau} - norm. to data lumi");
   hWmtt_Wm_PtLep_2-> SetFillColor(kRed);
   hWmtt_Wm_PtLep_2->Draw();


   //c6->Print(outPath + "/plot_6.eps");
   c6->Print(outPath + "/plot_6.png");

   TCanvas *c6b = new TCanvas("c6b", "", 800, 800);

   hZ_Wm_PtLepPt15->Scale(scaleZ);
   hWmtt_Wm_PtLepPt15->Scale(scaleWmtt);
   hd_Wm_PtLepPt15->Add(hd_Wm_PtLepPt15, hZ_Wm_PtLepPt15, 1, -1);
   hd_Wm_PtLepPt15->Add(hd_Wm_PtLepPt15, hWmtt_Wm_PtLepPt15, 1, -1);

   TH1F *hZ_Wm_PtLepPt15_2 = (TH1F *)hZ_Wm_PtLepPt15->Clone("hZ_Wm_PtLepPt15_2");
   TH1F *hWmtt_Wm_PtLepPt15_2 = (TH1F *)hWmtt_Wm_PtLepPt15->Clone("hWmtt_Wm_PtLepPt15_2");
   TH1F *hd_Wm_PtLepPt15_2 = (TH1F *)hd_Wm_PtLepPt15->Clone("hd_Wm_PtLepPt15_2");

   c6b-> SetTitle("W- sample");
   c6b->Divide(2, 2);
   c6b_1->cd();
   c6b_1->SetLogy(1);
   hZ_Wm_PtLepPt15_2-> SetFillStyle(3448);
   hZ_Wm_PtLepPt15_2-> SetFillColor(kGreen);
   hd_Wm_PtLepPt15_1->Draw();
   hZ_Wm_PtLepPt15_2-> SetStats(0);
   hWmtt_Wm_PtLepPt15_2-> SetFillStyle(3446);
   hWmtt_Wm_PtLepPt15_2-> SetFillColor(kRed);
   hWmtt_Wm_PtLepPt15_2-> SetStats(0);
   hWmtt_Wm_PtLepPt15_2->Draw("same");
   hZ_Wm_PtLepPt15_2->Draw("same");

   c6b_2->cd();
   c6b_2->SetLogy(1);
   hZ_Wm_PtLepPt15_2-> SetStats(1);

   hZ_Wm_PtLepPt15_2->SetNameTitle("hZ_Wm_PtLepPt15_2", "Z^{0}->ee - norm. to data lumi");
   hZ_Wm_PtLepPt15_2->Draw();

   c6b_3->cd();
   c6b_3->SetLogy(1);
   hWmtt_Wm_PtLepPt15_2-> SetStats(1);
   hWmtt_Wm_PtLepPt15_2->SetNameTitle("hWptt_Wm_PtLepPt15_2", "W^{-}->#tau#nu_{#tau} - norm. to data lumi");
   hWmtt_Wm_PtLepPt15_2-> SetFillColor(kRed);
   hWmtt_Wm_PtLepPt15_2->Draw();


   //c6b->Print(outPath + "/plot_6b.eps");
   c6b->Print(outPath + "/plot_6b.png");


   TCanvas *c6c = new TCanvas("c6c", "W- sample - QCD data driven - PtEle > 15 GeV", 800, 400);


   TH1F *hd_Wm_PtLepQCDPt15_1 = (TH1F *)hd_Wm_PtLepQCDPt15->Clone("hd_Wm_PtLepQCDPt15_1");

   // Double_t integral_Wm_15_18=  hd_Wm_PtLepPt15_2-> Integral(15,18);

   // Double_t integral_Wm_QCD15_18= hd_Wm_PtLepQCDPt15-> Integral(15,18);

   Double_t integral_Wm_5_6 =  hd_Wm_PtLepPt15_2-> Integral(5, 6);

   Double_t integral_Wm_QCD5_6 = hd_Wm_PtLepQCDPt15-> Integral(5, 6);

   float scale_Wm_QCD = integral_Wm_5_6 / integral_Wm_QCD5_6;

   cout << "W-, Integral of bin content in the first bin for Signal = " <<  integral_Wm_5_6 << endl;

   cout << "W-, Integral of bin content in the first bin for QCD = " <<  integral_Wm_QCD5_6 << endl;

   cout << "W-, QCD scale factor= " << scale_Wm_QCD << endl;

   hd_Wm_PtLepQCDPt15->Scale(scale_Wm_QCD);

   TH1F *hd_Wm_PtLepQCDPt15_2 = (TH1F *)hd_Wm_PtLepQCDPt15->Clone("hd_Wm_PtLepQCDPt15_2");

   hd_Wm_PtLepPt15->Add(hd_Wm_PtLepPt15, hd_Wm_PtLepQCDPt15, 1, -1);

   TH1F *hd_Wm_PtLepPt15_3 = (TH1F *)hd_Wm_PtLepPt15->Clone("hd_Wm_PtLepPt15_3");
   hd_Wm_PtLepPt15_3->SetNameTitle("hd_Wm_PtLepPt15_3", "data- Z^{0}->ee, W^{-}->#tau#nu_{#tau} and QCD subtracted");


   c6c->Divide(2, 1);

   c6c_1->cd();
   // hd_Wp_PtLepPt15_2-> SetMinimum(30.);
   hd_Wm_PtLepPt15_2->Draw();
   hd_Wm_PtLepQCDPt15_2-> SetFillStyle(3448);
   hd_Wm_PtLepQCDPt15_2-> SetFillColor(kMagenta);
   hd_Wm_PtLepQCDPt15_2->Draw("same");
   c6c_2->cd();
   hd_Wm_PtLepPt15_3->Draw();


   //c6c->Print(outPath + "/plot_6c.eps");
   c6c->Print(outPath + "/plot_6c.png");


   TCanvas *c6d = new TCanvas("c6d", "W- sample - QCD data driven", 800, 400);


   TH1F *hd_Wm_PtLepQCD_1 = (TH1F *)hd_Wm_PtLepQCD->Clone("hd_Wm_PtLepQCD_1");

   hd_Wm_PtLepQCD->Scale(scale_Wm_QCD);

   TH1F *hd_Wm_PtLepQCD_2 = (TH1F *)hd_Wm_PtLepQCD->Clone("hd_Wm_PtLepQCD_2");

   hd_Wm_PtLep->Add(hd_Wm_PtLep, hd_Wm_PtLepQCD, 1, -1);

   TH1F *hd_Wm_PtLep_3 = (TH1F *)hd_Wm_PtLep->Clone("hd_Wm_PtLep_3");
   hd_Wm_PtLep_3->SetNameTitle("hd_Wm_PtLep_3", "data- Z^{0}->ee, W^{-}->#tau#nu_{#tau} and QCD subtracted");


   c6d->Divide(2, 1);

   c6d_1->cd();
   hd_Wm_PtLep_2->Draw();
   hd_Wm_PtLepQCD_2-> SetFillStyle(3448);
   hd_Wm_PtLepQCD_2-> SetFillColor(kMagenta);
   hd_Wm_PtLepQCD_2->Draw("same");
   c6d_2->cd();
   hd_Wm_PtLep_3->Draw();


   //c6d->Print(outPath + "/plot_6d.eps");
   c6d->Print(outPath + "/plot_6d.png");


   TCanvas *c7 = new TCanvas("c7", "", 800, 400);

   c7-> SetTitle("W- sample summary ");
   c7->Divide(2, 1);

   c7_1->cd();
   c7_1->SetLogy(0);
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
   c7_2->SetLogy(0);
   // hd_Wm_PtLep_2->SetNameTitle("hd_Wm_PtLep","data (W-) - [Z^{0}->ee and W^{-}->#tau#nu_{#tau} subtracted]");
   hd_Wm_PtLep_3->SetMarkerStyle(20);
   hd_Wm_PtLep_3->Draw("E0");

   //c7->Print(outPath + "/plot_7.eps");
   c7->Print(outPath + "/plot_7.png");


   TCanvas *c7b = new TCanvas("c7b", "", 800, 400);

   c7b-> SetTitle("W- sample summary - PtEle > 15 GeV");
   c7b->Divide(2, 1);

   c7b_1->cd();
   c7b_1->SetLogy(0);
   hZ_Wm_PtLepPt15_2-> SetFillStyle(3448);
   hZ_Wm_PtLepPt15_2-> SetFillColor(kGreen);
   hd_Wm_PtLepPt15_1->Draw();
   hZ_Wm_PtLepPt15_2-> SetStats(0);
   hWmtt_Wm_PtLepPt15_2-> SetFillStyle(3446);
   hWmtt_Wm_PtLepPt15_2-> SetFillColor(kRed);
   hWmtt_Wm_PtLepPt15_2-> SetStats(0);
   hZ_Wm_PtLepPt15_2->Draw("same");
   hWmtt_Wm_PtLepPt15_2->Draw("same");

   c7b_2->cd();
   c7b_2->SetLogy(0);
   //hd_Wm_PtLepPt15_2->SetNameTitle("hd_Wm_PtLepPt15","data (W-) - [Z^{0}->ee and W^{-}->#tau#nu_{#tau} subtracted]");
   hd_Wm_PtLepPt15_3->SetMarkerStyle(20);
   hd_Wm_PtLepPt15_3->Draw("E0");

   //c7b->Print(outPath + "/plot_7b.eps");
   c7b->Print(outPath + "/plot_7b.png");


   // Calculate Signal/Bkgnd ratios and their errors:
   float hd_PtLep_1_integral           = hd_PtLep_1->Integral();
   float hZ_PtLep_2_integral           = hZ_PtLep_2->Integral();
   float hZ_PtLep_1_integral           = hZ_PtLep_1->Integral();
   float hZ_PtLep_2_integral_err       = scaleZ * sqrt(hZ_PtLep_1_integral);
   float hWptt_PtLep_2_integral        = hWptt_PtLep_2->Integral();
   float hWptt_PtLep_1_integral        = hWptt_PtLep_1->Integral();
   float hWptt_PtLep_2_integral_err    = scaleWptt * sqrt(hWptt_PtLep_1_integral);
   float hWmtt_PtLep_2_integral        = hWmtt_PtLep_2->Integral();
   float hWmtt_PtLep_1_integral        = hWmtt_PtLep_1->Integral();
   float hWmtt_PtLep_2_integral_err    = scaleWmtt * sqrt(hWmtt_PtLep_1_integral);
   float hd_PtLepQCD_2_integral        = hd_PtLepQCD_2->Integral();
   float hd_PtLepQCD_1_integral        = hd_PtLepQCD_1->Integral();
   float hd_PtLepQCD_2_integral_err    = scaleQCD * sqrt(hd_PtLepQCD_1_integral);
   float hZ_PtLep_2_BoS                = hZ_PtLep_2_integral / hd_PtLep_1_integral;
   float hZ_PtLep_2_BoS_err            = hZ_PtLep_2_integral_err / hd_PtLep_1_integral;
   float hWptt_PtLep_2_BoS             = hWptt_PtLep_2_integral / hd_PtLep_1_integral;
   float hWptt_PtLep_2_BoS_err         = hWptt_PtLep_2_integral_err / hd_PtLep_1_integral;
   float hWmtt_PtLep_2_BoS             = hWmtt_PtLep_2_integral / hd_PtLep_1_integral;
   float hWmtt_PtLep_2_BoS_err         = hWmtt_PtLep_2_integral_err / hd_PtLep_1_integral;
   float hd_PtLepQCD_2_BoS             = hd_PtLepQCD_2_integral / hd_PtLep_1_integral;
   float hd_PtLepQCD_2_BoS_err         = hd_PtLepQCD_2_integral_err / hd_PtLep_1_integral;

   float hd_Wp_PtLep_1_integral        = hd_Wp_PtLep_1->Integral();
   float hZ_Wp_PtLep_2_integral        = hZ_Wp_PtLep_2->Integral();
   float hZ_Wp_PtLep_1_integral        = hZ_Wp_PtLep_1->Integral();
   float hZ_Wp_PtLep_2_integral_err    = scaleZ * sqrt(hZ_Wp_PtLep_1_integral);
   float hWptt_Wp_PtLep_2_integral     = hWptt_Wp_PtLep_2->Integral();
   float hWptt_Wp_PtLep_1_integral     = hWptt_Wp_PtLep_1->Integral();
   float hWptt_Wp_PtLep_2_integral_err = scaleWptt * sqrt(hWptt_Wp_PtLep_1_integral);
   float hd_Wp_PtLepQCD_2_integral     = hd_Wp_PtLepQCD_2->Integral();
   float hd_Wp_PtLepQCD_1_integral     = hd_Wp_PtLepQCD_1->Integral();
   float hd_Wp_PtLepQCD_2_integral_err = scale_Wp_QCD * sqrt(hd_Wp_PtLepQCD_1_integral);
   float hZ_Wp_PtLep_2_BoS             = hZ_Wp_PtLep_2_integral / hd_Wp_PtLep_1_integral;
   float hZ_Wp_PtLep_2_BoS_err         = hZ_Wp_PtLep_2_integral_err / hd_Wp_PtLep_1_integral;
   float hWptt_Wp_PtLep_2_BoS          = hWptt_Wp_PtLep_2_integral / hd_Wp_PtLep_1_integral;
   float hWptt_Wp_PtLep_2_BoS_err      = hWptt_Wp_PtLep_2_integral_err / hd_Wp_PtLep_1_integral;
   float hd_Wp_PtLepQCD_2_BoS          = hd_Wp_PtLepQCD_2_integral / hd_Wp_PtLep_1_integral;
   float hd_Wp_PtLepQCD_2_BoS_err      = hd_Wp_PtLepQCD_2_integral_err / hd_Wp_PtLep_1_integral;

   float hd_Wm_PtLep_1_integral        = hd_Wm_PtLep_1->Integral();
   float hZ_Wm_PtLep_2_integral        = hZ_Wm_PtLep_2->Integral();
   float hZ_Wm_PtLep_1_integral        = hZ_Wm_PtLep_1->Integral();
   float hZ_Wm_PtLep_2_integral_err    = scaleZ * sqrt(hZ_Wm_PtLep_1_integral);
   float hWmtt_Wm_PtLep_2_integral     = hWmtt_Wm_PtLep_2->Integral();
   float hWmtt_Wm_PtLep_1_integral     = hWmtt_Wm_PtLep_1->Integral();
   float hWmtt_Wm_PtLep_2_integral_err = scaleWmtt * sqrt(hWmtt_Wm_PtLep_1_integral);
   float hd_Wm_PtLepQCD_2_integral     = hd_Wm_PtLepQCD_2->Integral();
   float hd_Wm_PtLepQCD_1_integral     = hd_Wm_PtLepQCD_1->Integral();
   float hd_Wm_PtLepQCD_2_integral_err = scale_Wm_QCD * sqrt(hd_Wm_PtLepQCD_1_integral);
   float hZ_Wm_PtLep_2_BoS             = hZ_Wm_PtLep_2_integral / hd_Wm_PtLep_1_integral;
   float hZ_Wm_PtLep_2_BoS_err         = hZ_Wm_PtLep_2_integral_err / hd_Wm_PtLep_1_integral;
   float hWmtt_Wm_PtLep_2_BoS          = hWmtt_Wm_PtLep_2_integral / hd_Wm_PtLep_1_integral;
   float hWmtt_Wm_PtLep_2_BoS_err      = hWmtt_Wm_PtLep_2_integral_err / hd_Wm_PtLep_1_integral;
   float hd_Wm_PtLepQCD_2_BoS          = hd_Wm_PtLepQCD_2_integral / hd_Wm_PtLep_1_integral;
   float hd_Wm_PtLepQCD_2_BoS_err      = hd_Wm_PtLepQCD_2_integral_err / hd_Wm_PtLep_1_integral;


   cout.precision(2);

   cout << "************************************************ " << endl;
   cout << "------------ " << endl;
   cout << "TOTAL SAMPLE " << endl;
   cout << "------------ " << endl;
   cout << "Signal events    = " << hd_PtLep_1_integral << endl;
   cout << "Z0 -> ee events  = " << hZ_PtLep_2_integral << " +- " << hZ_PtLep_2_integral_err << endl;
   cout << "Z0 -> ee B/S     = " << 100 * hZ_PtLep_2_BoS << "%" << " +- " << 100 * hZ_PtLep_2_BoS_err << "%" << endl;
   cout << "W+ -> tau events = " << hWptt_PtLep_2_integral << " +- " << hWptt_PtLep_2_integral_err << endl;
   cout << "W+ -> tau B/S    = " << 100 * hWptt_PtLep_2_BoS << "%" << " +- " << 100 * hWptt_PtLep_2_BoS_err << "%" << endl;
   cout << "W- -> tau events = " << hWmtt_PtLep_2_integral << " +- " << hWmtt_PtLep_2_integral_err << endl;
   cout << "W- -> tau B/S    = " << 100 * hWmtt_PtLep_2_BoS << "%" << " +- " << 100 * hWmtt_PtLep_2_BoS_err << "%" << endl;
   cout << "QCD events       = " << hd_PtLepQCD_2_integral << " +- " << hd_PtLepQCD_2_integral_err << endl;
   cout << "QCD B/S          = " << 100 * hd_PtLepQCD_2_BoS << "%" << " +- " << 100 * hd_PtLepQCD_2_BoS_err << "%" << endl;

   cout << " " << endl;
   cout << " " << endl;

   cout << "---------------------- " << endl;
   cout << "POSITIVE CHARGE SAMPLE " << endl;
   cout << "---------------------- " << endl;
   cout << "Signal events    = " << hd_Wp_PtLep_1_integral << endl;
   cout << "Z0 -> ee events  = " << hZ_Wp_PtLep_2_integral << " +- " << hZ_Wp_PtLep_2_integral_err << endl;
   cout << "Z0 -> ee B/S     = " << 100 * hZ_Wp_PtLep_2_BoS << "%" << " +- " << 100 * hZ_Wp_PtLep_2_BoS_err << "%" << endl;
   cout << "W+ -> tau events = " << hWptt_Wp_PtLep_2_integral << " +- " << hWptt_Wp_PtLep_2_integral_err << endl;
   cout << "W+ -> tau B/S    = " << 100 * hWptt_Wp_PtLep_2_BoS << "%" << " +- " << 100 * hWptt_Wp_PtLep_2_BoS_err << "%" << endl;
   cout << "QCD events       = " << hd_Wp_PtLepQCD_2_integral << " +- " << hd_Wp_PtLepQCD_2_integral_err << endl;
   cout << "QCD B/S          = " << 100 * hd_Wp_PtLepQCD_2_BoS << "%" << " +- " << 100 * hd_Wp_PtLepQCD_2_BoS_err << "%" << endl;

   cout << " " << endl;
   cout << " " << endl;

   cout << "---------------------- " << endl;
   cout << "NEGATIVE CHARGE SAMPLE " << endl;
   cout << "---------------------- " << endl;
   cout << "Signal events    = " << hd_Wm_PtLep_1_integral << endl;
   cout << "Z0 -> ee events  = " << hZ_Wm_PtLep_2_integral << " +- " << hZ_Wm_PtLep_2_integral_err << endl;
   cout << "Z0 -> ee B/S     = " << 100 * hZ_Wm_PtLep_2_BoS << "%" << " +- " << 100 * hZ_Wm_PtLep_2_BoS_err << "%" << endl;
   cout << "W- -> tau events = " << hWmtt_Wm_PtLep_2_integral << " +- " << hWmtt_Wm_PtLep_2_integral_err << endl;
   cout << "W- -> tau B/S    = " << 100 * hWmtt_Wm_PtLep_2_BoS << "%" << " +- " << 100 * hWmtt_Wm_PtLep_2_BoS_err << "%" << endl;
   cout << "QCD events       = " << hd_Wm_PtLepQCD_2_integral << " +- " << hd_Wm_PtLepQCD_2_integral_err << endl;
   cout << "QCD B/S          = " << 100 * hd_Wm_PtLepQCD_2_BoS << "%" << " +- " << 100 * hd_Wm_PtLepQCD_2_BoS_err << "%" << endl;
   cout << "************************************************ " << endl;



   // plot the Q over Pt distributions:


   TCanvas *c9 = new TCanvas("c9", "DATA QEt/Pt", 800, 400);

   c9->Divide(2, 1);

   c9_1->cd();
   hd_QxEtoPt_Vs_Et->Draw();
   //TLine *line1 = new TLine(0,0.4,c8_1->GetFrame()->GetX2(),0.4);
   TLine *line1 = new TLine(0, 0.4, 80, 0.4);
   line1->SetLineColor(kRed);
   line1->SetLineWidth(2);
   line1->Draw();
   line1->DrawLine(0, -0.4, 80, -0.4);
   line1->DrawLine(0, 1.8, 80, 1.8);
   line1->DrawLine(0, -1.4, 80, -1.4);
   c9_2->cd();
   hd_QxEtoPt_Vs_Et_passCut->Draw();

   //c9->Print(outPath + "/plot_9.eps");
   c9->Print(outPath + "/plot_9.png");


   TCanvas *c10 = new TCanvas("c10", "", 800, 400);

   c10-> SetTitle("Reco vs Gen");

   c10->Divide(2, 1);
   //c10->SetLeftMargin(0.2);

   c10_1->cd();
   //c10_1->SetLeftMargin(0.8);
   hWp_PtWGen->SetNameTitle("hWp_PtWGen", "Generated W+");
   hWp_PtWGen->GetYaxis()->SetTitleOffset(1.8);
   hWp_PtWGen->SetFillColor(kYellow);
   hWp_PtWGen->Draw();

   c10_2->cd();
   hWp_PtWReco->SetNameTitle("hWp_PtWReco", "Reconstructed W+");
   hWp_PtWReco->GetXaxis()->SetTitle("Tracks-based recoil P_{T} [GeV/c]");
   hWp_PtWReco->GetYaxis()->SetTitleOffset(1.8);
   hWp_PtWReco->SetFillColor(kYellow);
   hWp_PtWReco->Draw();

   //c10->Print(outPath + "/plot_10.eps");
   c10->Print(outPath + "/plot_10.png");



   TCanvas *c10b = new TCanvas("c10b", "", 800, 400);

   c10b-> SetTitle("Reco vs Gen");

   c10b->Divide(2, 1);

   c10b_1->cd();
   hWm_PtWGen->SetNameTitle("hWm_PtWGen", "Generated W-");
   hWm_PtWGen->GetYaxis()->SetTitleOffset(1.8);
   hWm_PtWGen->Draw();

   c10b_2->cd();
   hWm_PtWReco->SetNameTitle("hWm_PtWReco", "Reconstructed W-");
   hWm_PtWReco->GetXaxis()->SetTitle("Tracks-based recoil P_{T} [GeV/c]");
   hWm_PtWReco->GetYaxis()->SetTitleOffset(1.8);
   hWm_PtWReco->Draw();

   //c10b->Print(outPath + "/plot_10b.eps");
   c10b->Print(outPath + "/plot_10b.png");

   TCanvas *c10c = new TCanvas("c10c", "", 800, 400);

   c10c-> SetTitle("Reco vs Gen");

   c10c->Divide(2, 1);

   c10c_1->cd();
   c10c_1->SetLogz(1);
   hWp_PtRecoil_vs_PtWGen->GetYaxis()->SetTitleOffset(1.1);
   hWp_PtRecoil_vs_PtWGen-> SetStats(0);
   hWp_PtRecoil_vs_PtWGen->Draw();

   c10c_2->cd();
   c10c_2->SetLogz(1);
   hWp_PhiRecoil_vs_PhiWGen-> SetStats(0);
   hWp_PhiRecoil_vs_PhiWGen->Draw();

   //c10c->Print(outPath + "/plot_10c.eps");
   c10c->Print(outPath + "/plot_10c.png");



   TCanvas *c10d = new TCanvas("c10d", "", 800, 400);

   c10d-> SetTitle("Reco vs Gen");

   c10d->Divide(2, 1);

   c10d_1->cd();
   c10d_1->SetLogz(1);
   hWp_Wp_PtWGenOverReco->GetYaxis()->SetTitleOffset(1.1);
   hWp_Wp_PtWGenOverReco->SetTitle("W+ sample - MC Correction; Tracks-based recoil P_{T}, GeV/c; W P_{T}/Recoil P_{T}");
   hWp_Wp_PtWGenOverReco->Draw("E1");
   //hWp_PtRecoil_vs_PtWGen->GetYaxis()->SetTitleOffset(1.1);
   //hWp_PtRecoil_vs_PtWGen->SetStats(0);
   //hWp_PtRecoil_vs_PtWGen->Draw();

   c10d_2->cd();
   hd_Wp_PtWRecoCorrected->SetFillStyle(3448);
   hd_Wp_PtWRecoCorrected->SetFillColor(kGreen);
   //hd_Wp_PtWRecoCorrected->Draw();
   hd_Wp_PtWReco->SetTitle("Data");
   hd_Wp_PtWReco->Draw();
   hd_Wp_PtWRecoCorrected->Draw("same");

   //c10d->Print(outPath + "/plot_10d.eps");
   c10d->Print(outPath + "/plot_10d.png");



   TCanvas *c10di = new TCanvas("c10di", "", 800, 400);

   hWp_Wp_PtWGenOverReco_zoomin->Draw("E1");
   c10di->SaveAs(outPath + "/plot_10di.png");



   TCanvas *c10e = new TCanvas("c10e", "", 800, 400);

   c10e-> SetTitle("Reco vs Gen");

   c10e->Divide(2, 1);

   c10e_1->cd();
   c10e_1->SetLogz(1);
   hWm_Wm_PtWGenOverReco->GetYaxis()->SetTitleOffset(1.1);
   hWm_Wm_PtWGenOverReco->SetTitle("W- sample - MC Correction; Tracks-based recoil P_{T}, GeV/c; W P_{T}/Recoil P_{T}");
   hWm_Wm_PtWGenOverReco->Draw("E1");
   c10e_2->cd();
   hd_Wm_PtWReco->SetNameTitle("hd_Wm_PtWReco", "Data");
   //hd_Wm_PtWReco->Draw();
   hd_Wm_PtWRecoCorrected->SetFillStyle(3448);
   hd_Wm_PtWRecoCorrected->SetFillColor(kGreen);
   //hd_Wm_PtWRecoCorrected->Draw();
   hd_Wm_PtWReco->Draw();
   hd_Wm_PtWRecoCorrected->Draw("same");

   //c10e->Print(outPath + "/plot_10e.eps");
   c10e->Print(outPath + "/plot_10e.png");


   TCanvas *c11 = new TCanvas("c11", "", 800, 400);

   c11->SetTitle("DATA - Neutrino vs electron");

   c11->Divide(2, 1);

   c11_1->cd();
   c11_1->SetLogz(1);
   hd_PtNeutrinoVsEleNocut->GetYaxis()->SetTitleOffset(1.1);
   hd_PtNeutrinoVsEleNocut-> SetStats(0);
   hd_PtNeutrinoVsEleNocut->Draw();
   line1->DrawLine(0, 18, 60, 18);
   line1->DrawLine(25, -40, 25, 60);

   c11_2->cd();
   c11_2->SetLogz(1);
   hd_PtNeutrinoVsEle->GetYaxis()->SetTitleOffset(1.1);
   hd_PtNeutrinoVsEle-> SetStats(0);
   hd_PtNeutrinoVsEle->Draw();

   //c11->Print(outPath + "/plot_11.eps");
   c11->Print(outPath + "/plot_11.png");

}
