void backgrounds2011()
{
   std::cout.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed

   //gROOT->Reset();

   // Load the libraries:
   //gROOT->Macro("/star/u/fazio/offline/users/fazio/vbasym/macros/loadLibs.C");

   TString inPath        = "/star/institutions/bnl_me/fazio/vbana_out/";
   TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString outPath       = "~/vbasym_results/plots/2011/";
   //TString outPath       = "~/vbasym_results/plots/4prelim";
   TString outPathPaper  = "~/vbasym_results/plots/paper";

   //Styles:
   gStyle->SetPadBottomMargin(0.15);
   //gStyle->SetPadLeftMargin(0.13);
   gStyle->SetPadLeftMargin(0.17);
   gStyle->SetStatFontSize(0.05);
   gStyle->SetOptStat("mruoi");
   gStyle->SetPadRightMargin(0.25);
   gStyle->SetMarkerStyle(20);
   gStyle->SetStatX(0.99);
   gStyle->SetOptFit(1);
   gStyle->SetOptDate(0);


   float lumiDataTot       = 24.42; // pb-1
   //float lumiDataEff       = lumiDataTot; // just a test
   //float lumiDataEff       = 23.99; // pb-1
   float lumiDataEff       = 26.1; // pb-1  //TEST
   //float lumiMC_Z          = 1621;  // pb-1
   float lumiMC_Z          = 1802;  // pb-1 // s.f. 30 Jan 2015
   float lumiMC_Wp         = 1925;  // pb-1
   float lumiMC_Wm         = 1872;  // pb-1
   float lumiMC_WpToTauNu = 1945;  // pb-1
   //float lumiMC_WmToTauTau = 1802;  // pb-1
   float lumiMC_WmToTauNu = 1403; // pb-1  // s.f. 30 Jan 2015


   float scaleZ    = lumiDataEff / lumiMC_Z;
   float scaleWptt = lumiDataEff / lumiMC_WpToTauNu;
   float scaleWmtt = lumiDataEff / lumiMC_WmToTauNu;
   float scaleWp   = lumiDataEff / lumiMC_Wp;
   float scaleWm   = lumiDataEff / lumiMC_Wm;

   cout.precision(3);
   cout << "*****************************************" << endl;
   cout << "SCALE FACTORS" << endl;
   cout << "Z0 lumi scale factor =" << scaleZ << endl;
   cout << "Wp->tv lumi scale factor =" << scaleWptt << endl;
   cout << "Wm->tv lumi scale factor =" << scaleWmtt << endl;
   cout << "Wp->ev lumi scale factor =" << scaleWp << endl;
   cout << "Wm->ev lumi scale factor =" << scaleWm << endl;
   cout << "*****************************************" << endl;

   // open histogram files
   TFile *fileData   = TFile::Open(inPathNew + "run11_pp_transverse/hist/vbana_oldstyle.root");
   TFile *fileDataNew   = TFile::Open(inPathNew + "run11_pp_transverse/hist/vbana.root");
   TFile *fileMCWp   = TFile::Open(inPathNew + "run11_mc_Wp2enu.lis/hist/vbana_oldstyle.root");
   TFile *fileMCWpNew   = TFile::Open(inPathNew + "run11_mc_Wp2enu.lis/hist/vbana.root");
   TFile *fileMCWm   = TFile::Open(inPathNew + "run11_mc_Wm2enu.lis/hist/vbana_oldstyle.root");
   TFile *fileMCWmNew   = TFile::Open(inPathNew + "run11_mc_Wm2enu.lis/hist/vbana.root");
   TFile *fileMCZ    = TFile::Open(inPathNew + "run11_mc_Z02ee.lis/hist/vbana_oldstyle.root");
   TFile *fileMCZNew    = TFile::Open(inPathNew + "run11_mc_Z02ee.lis/hist/vbana.root");
   TFile *fileMCWptt = TFile::Open(inPathNew + "run11_mc_Wp2taunu.lis/hist/vbana_oldstyle.root");
   TFile *fileMCWpttNew = TFile::Open(inPathNew + "run11_mc_Wp2taunu.lis/hist/vbana.root");
   TFile *fileMCWmtt = TFile::Open(inPathNew + "run11_mc_Wm2taunu.lis/hist/vbana_oldstyle.root");
   TFile *fileMCWmttNew = TFile::Open(inPathNew + "run11_mc_Wm2taunu.lis/hist/vbana.root");

// W+
   TH1 *hd_Wp_PtLep        = (TH1 *)fileData   -> Get("W+_event_pass_wbos/hCandidateTrackEScaledPt");
   TH1 *hZ_Wp_PtLep        = (TH1 *)fileMCZ    -> Get("W+_event_pass_wbos/hCandidateTrackEScaledPt");
   TH1 *hWp_Wp_PtLep       = (TH1 *)fileMCWp   -> Get("W+_event_pass_wbos/hCandidateTrackEScaledPt");
   TH1 *hWptt_Wp_PtLep     = (TH1 *)fileMCWptt -> Get("W+_event_pass_wbos/hCandidateTrackEScaledPt");

   TH1 *hd_Wp_PtLepPt15    = (TH1*) fileData   -> Get("W+_event_pass_wbos_pt>15/hCandidateTrackEScaledPt");
   TH1 *hZ_Wp_PtLepPt15    = (TH1*) fileMCZ    -> Get("W+_event_pass_wbos_pt>15/hCandidateTrackEScaledPt");
   TH1 *hWp_Wp_PtLepPt15   = (TH1*) fileMCWp   -> Get("W+_event_pass_wbos_pt>15/hCandidateTrackEScaledPt");
   TH1 *hWptt_Wp_PtLepPt15 = (TH1*) fileMCWptt -> Get("W+_event_pass_wbos_pt>15/hCandidateTrackEScaledPt");

   TH1 *hd_Wp_PtLepQCD     = (TH1 *)fileData   -> Get("W+_event_pass_qcd/hCandidateTrackEScaledPt");
   TH1 *hd_Wp_PtLepQCDPt15 = (TH1 *)fileData   -> Get("W+_event_pass_qcd_pt>15/hCandidateTrackEScaledPt");

   //TH1 *hd_Wd_WBosonPt_zoomin  = (TH1*) fileDataNew -> Get("event_wm/hWBosonPt_zoomin");
   //TH1 *hWp_Wp_WBosonPt_zoomin = (TH1*) fileMCWp->Get("event_wp/hWBosonPt_zoomin");

   hd_Wp_PtLep->Rebin(3);
   hZ_Wp_PtLep->Rebin(3);
   hWp_Wp_PtLep->Rebin(3);
   hWptt_Wp_PtLep->Rebin(3);
   hd_Wp_PtLepQCD->Rebin(3);
   hd_Wp_PtLepPt15->Rebin(3);
   hZ_Wp_PtLepPt15->Rebin(3);
   hWp_Wp_PtLepPt15->Rebin(3);
   hWptt_Wp_PtLepPt15->Rebin(3);
   hd_Wp_PtLepQCDPt15->Rebin(3);

   TH1 *hd_Wp_PtWReco                 = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt");
   TH1 *hd_Wp_PtWReco_zoomin          = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWp_Wp_PtWReco                = (TH1*) fileMCWp->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_Wp_PtWReco_zoomin         = (TH1*) fileMCWp->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWptt_Wp_PtWReco_zoomin       = (TH1*) fileMCWptt->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");

   TH1 *hd_Wp_PtWRecoCorrected           = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hd_Wp_PtWRecoCorrected_zoomin    = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_Wp_PtWRecoCorrected_zoomin   = (TH1*) fileMCWp->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWptt_Wp_PtWRecoCorrected_zoomin = (TH1*) fileMCWptt->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");

   TH2 *hWp_RecoVsGenLeptonPt          = (TH2*) fileMCWp->Get("event_mc/hRecoVsGenLeptonPt");


// W-
   TH1 *hd_Wm_PtLep        = (TH1 *)fileData  ->Get("W-_event_pass_wbos/hCandidateTrackEScaledPt");
   TH1 *hZ_Wm_PtLep        = (TH1 *)fileMCZ   ->Get("W-_event_pass_wbos/hCandidateTrackEScaledPt");
   TH1 *hWm_Wm_PtLep       = (TH1 *)fileMCWm  ->Get("W-_event_pass_wbos/hCandidateTrackEScaledPt");
   TH1 *hWmtt_Wm_PtLep     = (TH1 *)fileMCWmtt->Get("W-_event_pass_wbos/hCandidateTrackEScaledPt");

   TH1 *hd_Wm_PtLepPt15    = (TH1*) fileData  ->Get("W-_event_pass_wbos_pt>15/hCandidateTrackEScaledPt");
   TH1 *hZ_Wm_PtLepPt15    = (TH1*) fileMCZ   ->Get("W-_event_pass_wbos_pt>15/hCandidateTrackEScaledPt");
   TH1 *hWm_Wm_PtLepPt15   = (TH1*) fileMCWm  ->Get("W-_event_pass_wbos_pt>15/hCandidateTrackEScaledPt");
   TH1 *hWmtt_Wm_PtLepPt15 = (TH1*) fileMCWmtt->Get("W-_event_pass_wbos_pt>15/hCandidateTrackEScaledPt");

   //TH1 *hd_Wm_PtLepQCD     = (TH1 *)fileData  ->Get("W-_track_cand_pass_qcd/hEcalScaledPt"); //CHECK THIS!!!!!!!!!
   TH1 *hd_Wm_PtLepQCD     = (TH1 *)fileData  ->Get("W-_event_pass_qcd/hCandidateTrackEScaledPt"); //CHECK THIS!!!!!!!!!
   TH1 *hd_Wm_PtLepQCDPt15 = (TH1*) fileData  ->Get("W-_event_pass_qcd_pt>15/hCandidateTrackEScaledPt");

   hd_Wm_PtLep->Rebin(3);
   hZ_Wm_PtLep->Rebin(3);
   hWm_Wm_PtLep->Rebin(3);
   hWmtt_Wm_PtLep->Rebin(3);
   hd_Wm_PtLepQCD->Rebin(3);
   hd_Wm_PtLepPt15->Rebin(3);
   hZ_Wm_PtLepPt15->Rebin(3);
   hWm_Wm_PtLepPt15->Rebin(3);
   hWmtt_Wm_PtLepPt15->Rebin(3);
   hd_Wm_PtLepQCDPt15->Rebin(3);

   TH1 *hd_Wm_PtWReco                = (TH1 *)fileData->Get("W-_event_pass_wbos/hTrackRecoilWithNeutralsPt");
   TH1 *hd_Wm_PtWReco_zoomin         = (TH1*) fileData->Get("W-_event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");

   TH1 *hWm_Wm_PtWReco               = (TH1 *)fileMCWm->Get("W-_event_pass_wbos/hTrackRecoilWithNeutralsPt");
   TH1 *hWm_Wm_PtWReco_zoomin        = (TH1*) fileMCWm->Get("W-_event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");

   TH1 *hd_Wm_PtWRecoCorrected       = (TH1*) fileData->Get("W-_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hd_Wm_PtWRecoCorrected_zoomin= (TH1*) fileData->Get("W-_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");


   //-----------------------------------------------------------------------------------------------------------------

   hd_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hZ_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hWp_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hWptt_Wp_PtLep->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hd_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hZ_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hWm_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hWmtt_Wm_PtLep->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hd_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hZ_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hWp_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hWptt_Wp_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hd_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hZ_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hWm_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");
   hWmtt_Wm_PtLepPt15->GetXaxis()->SetTitle("P_{T}^{e} (GeV/c)");

   hd_Wp_PtLep->GetYaxis()->SetTitle("events");
   hZ_Wp_PtLep->GetYaxis()->SetTitle("events");
   hWp_Wp_PtLep->GetYaxis()->SetTitle("events");
   hWptt_Wp_PtLep->GetYaxis()->SetTitle("events");
   hd_Wm_PtLep->GetYaxis()->SetTitle("events");
   hZ_Wm_PtLep->GetYaxis()->SetTitle("events");
   hWm_Wm_PtLep->GetYaxis()->SetTitle("events");
   hWmtt_Wm_PtLep->GetYaxis()->SetTitle("events");
   hd_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hZ_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hWp_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hWptt_Wp_PtLepPt15->GetYaxis()->SetTitle("events");
   hd_Wm_PtLepPt15->GetYaxis()->SetTitle("events");
   hZ_Wm_PtLepPt15->GetYaxis()->SetTitle("events");
   hWm_Wm_PtLepPt15->GetYaxis()->SetTitle("events");
   hWmtt_Wm_PtLepPt15->GetYaxis()->SetTitle("events");

   hd_Wp_PtLep->SetNameTitle("hd_Wp_PtLep", "Run11 (W+)");
   hd_Wm_PtLep->SetNameTitle("hd_Wm_PtLep", "Run11 (W-)");
   hd_Wp_PtLepPt15->SetNameTitle("hd_Wp_PtLepPt15", "Data (W+)");
   hd_Wm_PtLepPt15->SetNameTitle("hd_Wm_PtLepPt15", "Data (W-)");

   hd_Wp_PtLep->SetTitleOffset(1.3, "y");
   hd_Wm_PtLep->SetTitleOffset(1.3, "y");
   hd_Wp_PtLepPt15->SetTitleOffset(1.3, "y");
   hd_Wm_PtLepPt15->SetTitleOffset(1.3, "y");

   TH1F *hd_Wp_PtLep_1 = (TH1F *)hd_Wp_PtLep->Clone("hd_Wp_PtLep_1");
   TH1F *hZ_Wp_PtLep_1 = (TH1F *)hZ_Wp_PtLep->Clone("hZ_Wp_PtLep_1");
   TH1F *hWptt_Wp_PtLep_1 = (TH1F *)hWptt_Wp_PtLep->Clone("hWptt_Wp_PtLep_1");
   TH1F *hd_Wm_PtLep_1 = (TH1F *)hd_Wm_PtLep->Clone("hd_Wm_PtLep_1");
   TH1F *hZ_Wm_PtLep_1 = (TH1F *)hZ_Wm_PtLep->Clone("hZ_Wm_PtLep_1");
   TH1F *hWmtt_Wm_PtLep_1 = (TH1F *)hWmtt_Wm_PtLep->Clone("hWmtt_Wm_PtLep_1");

   TH1F *hd_Wp_PtLepPt15_1 = (TH1F *)hd_Wp_PtLepPt15->Clone("hd_Wp_PtLepPt15_1");
   TH1F *hZ_Wp_PtLepPt15_1 = (TH1F *)hZ_Wp_PtLepPt15->Clone("hZ_Wp_PtLepPt15_1");
   TH1F *hWptt_Wp_PtLepPt15_1 = (TH1F *)hWptt_Wp_PtLepPt15->Clone("hWptt_Wp_PtLepPt15_1");
   TH1F *hd_Wm_PtLepPt15_1 = (TH1F *)hd_Wm_PtLepPt15->Clone("hd_Wm_PtLepPt15_1");
   TH1F *hZ_Wm_PtLepPt15_1 = (TH1F *)hZ_Wm_PtLepPt15->Clone("hZ_Wm_PtLepPt15_1");
   TH1F *hWmtt_Wm_PtLepPt15_1 = (TH1F *)hWmtt_Wm_PtLepPt15->Clone("hWmtt_Wm_PtLepPt15_1");


   // W+ sample

   TCanvas *c4 = new TCanvas("c4", "W+ sample", 800, 800);

   TH1F *hWp_Wp_PtLep_2 = (TH1F *)hWp_Wp_PtLep -> Clone("hWp_Wp_PtLep_2");

   hZ_Wp_PtLep    -> Scale(scaleZ);
   hWptt_Wp_PtLep -> Scale(scaleWptt);
   hWp_Wp_PtLep_2 -> Scale(scaleWp);
   hWp_Wp_PtLep_2 -> Add(hWp_Wp_PtLep_2, hZ_Wp_PtLep, 1, 1); // W->ev MC (+ W->tn & Z->ee backgrounds) 
   hWp_Wp_PtLep_2 -> Add(hWp_Wp_PtLep_2, hWptt_Wp_PtLep, 1, 1);
   hd_Wp_PtLep    -> Add(hd_Wp_PtLep, hZ_Wp_PtLep, 1, -1);
   hd_Wp_PtLep    -> Add(hd_Wp_PtLep, hWptt_Wp_PtLep, 1, -1);

   TH1F *hZ_Wp_PtLep_2    = (TH1F *)hZ_Wp_PtLep    -> Clone("hZ_Wp_PtLep_2");
   TH1F *hWptt_Wp_PtLep_2 = (TH1F *)hWptt_Wp_PtLep -> Clone("hWptt_Wp_PtLep_2");
   TH1F *hd_Wp_PtLep_2    = (TH1F *)hd_Wp_PtLep    -> Clone("hd_Wp_PtLep_2");

   //c4->Divide(2, 2);

   TLegend *leg4 = new TLegend(0.5, 0.65, 0.75, 0.9);
   leg4 -> AddEntry(hd_Wp_PtLep_1,"STAR data", "P");
   leg4 -> AddEntry(hWp_Wp_PtLep_2,"PYTHIA  W^{+}-> e#nu_{e} + Bkgd ", "L" );
   leg4 -> AddEntry(hWptt_Wp_PtLep_2,"PYTHIA W^{+}-> #tau#nu_{#tau} ", "F" );
   leg4 -> AddEntry(hZ_Wp_PtLep_2,"PYTHIA Z^{0}-> e^{+}e^{-}", "F");

   c4 -> SetGrid(0,0);
   c4 -> cd();
   c4 -> SetLogy(1);
   hd_Wp_PtLep_1    -> SetLineWidth(2);
   hd_Wp_PtLep_1    -> SetMarkerStyle(20);
   hZ_Wp_PtLep_2    -> SetFillStyle(3448);
   hZ_Wp_PtLep_2    -> SetFillColor(kGreen);
   hd_Wp_PtLep_1    -> Draw("e");
   hZ_Wp_PtLep_2    -> SetStats(0);
   hWptt_Wp_PtLep_2 -> SetFillStyle(3446);
   hWptt_Wp_PtLep_2 -> SetFillColor(kRed);
   hWptt_Wp_PtLep_2 -> SetStats(0);
   hWptt_Wp_PtLep_2 -> Draw("same");
   hZ_Wp_PtLep_2    -> Draw("same");
   hWp_Wp_PtLep_2   -> SetFillStyle(0);
   hWp_Wp_PtLep_2   -> SetLineStyle(7);
   hWp_Wp_PtLep_2   -> SetLineWidth(2);
   hWp_Wp_PtLep_2   -> SetLineColor(kRed);
   hWp_Wp_PtLep_2   -> Draw("same");
   leg4 -> Draw();

   c4->Print(outPath + "/plot_4.eps");
   c4->Print(outPath + "/plot_4.png");


   TCanvas *c4b = new TCanvas("c4b", "W+ sample - PtEle > 15 GeV", 800, 800);

   TH1F *hWp_Wp_PtLepPt15_2 = (TH1F *)hWp_Wp_PtLepPt15 -> Clone("hWp_Wp_PtLepPt15_2");

   hZ_Wp_PtLepPt15    -> Scale(scaleZ);
   hWptt_Wp_PtLepPt15 -> Scale(scaleWptt);
   hWp_Wp_PtLepPt15_2 -> Scale(scaleWp);
   hWp_Wp_PtLepPt15_2 -> Add(hWp_Wp_PtLepPt15_2, hZ_Wp_PtLepPt15, 1, 1); // W->ev MC (+ W->tn & Z->ee backgrounds)
   hWp_Wp_PtLepPt15_2 -> Add(hWp_Wp_PtLepPt15_2, hWptt_Wp_PtLepPt15, 1, 1);
   hd_Wp_PtLepPt15    -> Add(hd_Wp_PtLepPt15, hZ_Wp_PtLepPt15, 1, -1);
   hd_Wp_PtLepPt15    -> Add(hd_Wp_PtLepPt15, hWptt_Wp_PtLepPt15, 1, -1);

   TH1F *hZ_Wp_PtLepPt15_2    = (TH1F *)hZ_Wp_PtLepPt15    -> Clone("hZ_Wp_PtLepPt15_2");
   TH1F *hWptt_Wp_PtLepPt15_2 = (TH1F *)hWptt_Wp_PtLepPt15 -> Clone("hWptt_Wp_PtLepPt15_2");
   TH1F *hd_Wp_PtLepPt15_2    = (TH1F *)hd_Wp_PtLepPt15    -> Clone("hd_Wp_PtLepPt15_2");

   //c4b->Divide(2, 2);

   c4b -> SetGrid(0,0);
   c4b -> cd();
   c4b -> SetLogy(1);
   hd_Wp_PtLepPt15_1    -> SetLineWidth(2);
   hd_Wp_PtLepPt15_1    -> SetMarkerStyle(20);
   hZ_Wp_PtLepPt15_2    -> SetFillStyle(3448);
   hZ_Wp_PtLepPt15_2    -> SetFillColor(kGreen);
   hd_Wp_PtLepPt15_1    -> Draw("e");
   hZ_Wp_PtLepPt15_2    -> SetStats(0);
   hWptt_Wp_PtLepPt15_2 -> SetFillStyle(3446);
   hWptt_Wp_PtLepPt15_2 -> SetFillColor(kRed);
   hWptt_Wp_PtLepPt15_2 -> SetStats(0);
   hWptt_Wp_PtLepPt15_2 -> Draw("same");
   hZ_Wp_PtLepPt15_2    -> Draw("same");
   hWp_Wp_PtLepPt15_2   -> SetFillStyle(0);
   hWp_Wp_PtLepPt15_2   -> SetLineStyle(7);
   hWp_Wp_PtLepPt15_2   -> SetLineWidth(2);
   hWp_Wp_PtLepPt15_2   -> SetLineColor(kRed);
   hWp_Wp_PtLepPt15_2   -> Draw("same");
   leg4 -> Draw();
   /*
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
   */

   c4b->Print(outPath + "/plot_4b.eps");
   c4b->Print(outPath + "/plot_4b.png");


   TCanvas *c4c = new TCanvas("c4c", "W+ sample - QCD data driven - PtEle > 15 GeV", 800, 400);


   // TH1F *hd_Wp_PtLepQCDPt15_1 = (TH1F *)hd_Wp_PtLepQCDPt15->Clone("hd_Wp_PtLepQCDPt15_1");

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

   TLegend *leg4c = new TLegend(0.46, 0.70, 0.75, 0.9);
   leg4c -> AddEntry(hd_Wp_PtLepPt15_2,"STAR data - W/Z background", "F");
   leg4c -> AddEntry(hd_Wp_PtLepQCDPt15_2,"data driven QCD", "F" );

   c4c_1->cd();
   // hd_Wp_PtLepPt15_2-> SetMinimum(30.);
   hd_Wp_PtLepPt15_2   -> Draw();
   hd_Wp_PtLepQCDPt15_2-> SetFillStyle(3448);
   hd_Wp_PtLepQCDPt15_2-> SetFillColor(kMagenta);
   hd_Wp_PtLepQCDPt15_2-> Draw("same");
   leg4c               -> Draw();

   c4c_2->cd();
   hd_Wp_PtLepPt15_3   -> Draw();


   c4c->Print(outPath + "/plot_4c.eps");
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


   c4d->Print(outPath + "/plot_4d.eps");
   c4d->Print(outPath + "/plot_4d.png");


   TCanvas *c5 = new TCanvas("c5", "", 800, 400);

   c5-> SetTitle("W+ sample summary");
   c5->Divide(2, 1);


   TLegend *leg5 = new TLegend(0.47, 0.6, 0.75, 0.9);
   leg5 -> AddEntry(hd_Wp_PtLep_1,"STAR data", "F");
   leg5 -> AddEntry(hWptt_Wp_PtLep_2,"PYTHIA W^{+}-> #tau#nu_{#tau} ", "F" );
   leg5 -> AddEntry(hZ_Wp_PtLep_2,"PYTHIA Z^{0}-> e^{+}e^{-}", "F");;
   leg5 -> AddEntry(hd_Wp_PtLepQCD_2,"data driven QCD", "F");

   c5_1->cd();
   c5_1->SetLogy(0);
   hZ_Wp_PtLep_2   -> SetFillStyle(3448);
   hZ_Wp_PtLep_2   -> SetFillColor(kGreen);
   hd_Wp_PtLep_1   -> Draw();
   hZ_Wp_PtLep_2   -> SetStats(0);
   hWptt_Wp_PtLep_2-> SetFillStyle(3446);
   hWptt_Wp_PtLep_2-> SetFillColor(kRed);
   hWptt_Wp_PtLep_2-> SetStats(0);
   hWptt_Wp_PtLep_2-> Draw("same");
   hZ_Wp_PtLep_2   -> Draw("same");
   hd_Wp_PtLepQCD_2-> Draw("same");
   leg5            -> Draw();

   c5_2->cd();
   c5_2->SetLogy(0);
   // hd_Wp_PtLep_2->SetNameTitle("hd_Wp_PtLep","data (W+) - [Z^{0}->ee and W^{+}->#tau#nu_{#tau} subtracted]");
   hd_Wp_PtLep_3->SetMarkerStyle(20);
   hd_Wp_PtLep_3->Draw("E0");

   c5->Print(outPath + "/plot_5.eps");
   c5->Print(outPath + "/plot_5.png");


   TCanvas *c5b = new TCanvas("c5b", "", 800, 400);

   c5b -> SetTitle("W+ sample summary - PtEle > 15 GeV");
   c5b -> Divide(2, 1);

   c5b_1 -> cd();
   c5b_1 -> SetLogy(0);

   hd_Wp_PtLepPt15_1    -> Draw(); // data - (No bkgd subtraction)

   hZ_Wp_PtLepPt15_2    -> SetFillStyle(3448);
   hZ_Wp_PtLepPt15_2    -> SetFillColor(kGreen);
   hZ_Wp_PtLepPt15_2    -> SetStats(0);
   hZ_Wp_PtLepPt15_2    -> Draw("same");

   hWptt_Wp_PtLepPt15_2 -> SetFillStyle(3446);
   hWptt_Wp_PtLepPt15_2 -> SetFillColor(kRed);
   hWptt_Wp_PtLepPt15_2 -> SetStats(0);
   hWptt_Wp_PtLepPt15_2 -> Draw("same");

   hd_Wp_PtLepQCDPt15_2 -> Draw("same");

   c5b_2->cd();
   c5b_2->SetLogy(0);
   //hd_Wp_PtLepPt15_2->SetNameTitle("hd_Wp_PtLepPt15","data (W+) - [Z^{0}->ee and W^{+}->#tau#nu_{#tau} subtracted]");
   hd_Wp_PtLepPt15_3 -> SetMarkerStyle(20);
   hd_Wp_PtLepPt15_3 -> Draw("E0");  // data - (All bkgd subtracted)

   c5b->Print(outPath + "/plot_5b.eps");
   c5b->Print(outPath + "/plot_5b.png");


   // W- sample

   TCanvas *c6 = new TCanvas("c6", "W- sample", 800, 800);

   TH1F *hWm_Wm_PtLep_2 = (TH1F *)hWm_Wm_PtLep -> Clone("hWm_Wm_PtLep_2");

   hZ_Wm_PtLep    -> Scale(scaleZ);
   hWmtt_Wm_PtLep -> Scale(scaleWmtt);
   hWm_Wm_PtLep_2 -> Scale(scaleWm);
   hWm_Wm_PtLep_2 -> Add(hWm_Wm_PtLep_2, hZ_Wm_PtLep, 1, 1);
   hWm_Wm_PtLep_2 -> Add(hWm_Wm_PtLep_2, hWmtt_Wm_PtLep, 1, 1);
   hd_Wm_PtLep    -> Add(hd_Wm_PtLep, hZ_Wm_PtLep, 1, -1);
   hd_Wm_PtLep    -> Add(hd_Wm_PtLep, hWmtt_Wm_PtLep, 1, -1);

   TH1F *hZ_Wm_PtLep_2    = (TH1F *)hZ_Wm_PtLep    -> Clone("hZ_Wm_PtLep_2");
   TH1F *hWmtt_Wm_PtLep_2 = (TH1F *)hWmtt_Wm_PtLep -> Clone("hWmtt_Wm_PtLep_2");
   TH1F *hd_Wm_PtLep_2    = (TH1F *)hd_Wm_PtLep    -> Clone("hd_Wm_PtLep_2");

   TLegend *leg6 = new TLegend(0.5, 0.6, 0.75, 0.9);
   leg6 -> AddEntry(hd_Wm_PtLep_1,"STAR data", "P");
   leg6 -> AddEntry(hWm_Wm_PtLep_2,"PYTHIA  W^{-}-> e#nu_{e} + Bkgd ", "L" );
   leg6 -> AddEntry(hWmtt_Wm_PtLep_2,"PYTHIA W^{-}-> #tau#nu_{#tau} ", "F" );
   leg6 -> AddEntry(hZ_Wm_PtLep_2,"PYTHIA Z^{0}-> e^{+}e^{-}", "F");

   //c6->Divide(2, 2);

   c6 -> SetGrid(0,0);
   c6 -> cd();
   c6 -> SetLogy(1);
   hd_Wm_PtLep_1   -> SetLineWidth(2);
   hd_Wm_PtLep_1   -> SetMarkerStyle(20);
   hZ_Wm_PtLep_2   -> SetFillStyle(3448);
   hZ_Wm_PtLep_2   -> SetFillColor(kGreen);
   hd_Wm_PtLep_1   -> Draw("e");
   hZ_Wm_PtLep_2   -> SetStats(0);
   hWmtt_Wm_PtLep_2-> SetFillStyle(3446);
   hWmtt_Wm_PtLep_2-> SetFillColor(kRed);
   hWmtt_Wm_PtLep_2-> SetStats(0);
   hWmtt_Wm_PtLep_2-> Draw("same");
   hZ_Wm_PtLep_2   -> Draw("same");
   hWm_Wm_PtLep_2  -> SetFillStyle(0);
   hWm_Wm_PtLep_2  -> SetLineStyle(7);
   hWm_Wm_PtLep_2  -> SetLineWidth(2);
   hWm_Wm_PtLep_2  -> SetLineColor(kRed);
   hWm_Wm_PtLep_2  -> Draw("same");
   leg6            -> Draw();

   c6->Print(outPath + "/plot_6.eps");
   c6->Print(outPath + "/plot_6.png");

   TCanvas *c6b = new TCanvas("c6b", "W- sample - PtEle > 15 GeV", 800, 800);

   TH1F *hWm_Wm_PtLepPt15_2 = (TH1F *)hWm_Wm_PtLepPt15 -> Clone("hWm_Wm_PtLepPt15_2");

   hZ_Wm_PtLepPt15    -> Scale(scaleZ);
   hWmtt_Wm_PtLepPt15 -> Scale(scaleWmtt);
   hWm_Wm_PtLepPt15_2 -> Scale(scaleWm);
   hWm_Wm_PtLepPt15_2 -> Add(hWm_Wm_PtLepPt15_2, hZ_Wm_PtLepPt15, 1, 1);
   hWm_Wm_PtLepPt15_2 -> Add(hWm_Wm_PtLepPt15_2, hWmtt_Wm_PtLepPt15, 1, 1);
   hd_Wm_PtLepPt15    -> Add(hd_Wm_PtLepPt15, hZ_Wm_PtLepPt15, 1, -1);
   hd_Wm_PtLepPt15    -> Add(hd_Wm_PtLepPt15, hWmtt_Wm_PtLepPt15, 1, -1);

   TH1F *hZ_Wm_PtLepPt15_2    = (TH1F *)hZ_Wm_PtLepPt15    -> Clone("hZ_Wm_PtLepPt15_2");
   TH1F *hWmtt_Wm_PtLepPt15_2 = (TH1F *)hWmtt_Wm_PtLepPt15 -> Clone("hWmtt_Wm_PtLepPt15_2");
   TH1F *hd_Wm_PtLepPt15_2    = (TH1F *)hd_Wm_PtLepPt15    -> Clone("hd_Wm_PtLepPt15_2");

   //c6b->Divide(2, 2);

   c6b -> SetGrid(0,0);
   c6b -> cd();
   c6b -> SetLogy(1);
   hd_Wm_PtLepPt15_1    -> SetLineWidth(2);
   hd_Wm_PtLepPt15_1    -> SetMarkerStyle(20);
   hZ_Wm_PtLepPt15_2    -> SetFillStyle(3448);
   hZ_Wm_PtLepPt15_2    -> SetFillColor(kGreen);
   hd_Wm_PtLepPt15_1    -> Draw("e");
   hZ_Wm_PtLepPt15_2    -> SetStats(0);
   hWmtt_Wm_PtLepPt15_2 -> SetFillStyle(3446);
   hWmtt_Wm_PtLepPt15_2 -> SetFillColor(kRed);
   hWmtt_Wm_PtLepPt15_2 -> SetStats(0);
   hWmtt_Wm_PtLepPt15_2 -> Draw("same");
   hZ_Wm_PtLepPt15_2    -> Draw("same");
   hWm_Wm_PtLepPt15_2   -> SetFillStyle(0);
   hWm_Wm_PtLepPt15_2   -> SetLineStyle(7);
   hWm_Wm_PtLepPt15_2   -> SetLineWidth(2);
   hWm_Wm_PtLepPt15_2   -> SetLineColor(kRed);
   hWm_Wm_PtLepPt15_2   -> Draw("same");
   leg6                 -> Draw();
   /*
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
   */

   c6b->Print(outPath + "/plot_6b.eps");
   c6b->Print(outPath + "/plot_6b.png");


   TCanvas *c6c = new TCanvas("c6c", "W- sample - QCD data driven - PtEle > 15 GeV", 800, 400);


   TH1F *hd_Wm_PtLepQCDPt15_1 = (TH1F *)hd_Wm_PtLepQCDPt15->Clone("hd_Wm_PtLepQCDPt15_1");

   // Double_t integral_Wm_15_18=  hd_Wm_PtLepPt15_2-> Integral(15,18);

   // Double_t integral_Wm_QCD15_18= hd_Wm_PtLepQCDPt15-> Integral(15,18);

   Double_t integral_Wm_5_6    = hd_Wm_PtLepPt15_2  -> Integral(5, 6);

   Double_t integral_Wm_QCD5_6 = hd_Wm_PtLepQCDPt15 -> Integral(5, 6);

   float scale_Wm_QCD = integral_Wm_5_6 / integral_Wm_QCD5_6;

   cout << "W-, Integral of bin content in the first bin for Signal = " <<  integral_Wm_5_6 << endl;

   cout << "W-, Integral of bin content in the first bin for QCD = " <<  integral_Wm_QCD5_6 << endl;

   cout << "W-, QCD scale factor= " << scale_Wm_QCD << endl;

   hd_Wm_PtLepQCDPt15 -> Scale(scale_Wm_QCD);

   TH1F *hd_Wm_PtLepQCDPt15_2 = (TH1F *)hd_Wm_PtLepQCDPt15->Clone("hd_Wm_PtLepQCDPt15_2");

   hd_Wm_PtLepPt15    -> Add(hd_Wm_PtLepPt15, hd_Wm_PtLepQCDPt15, 1, -1);

   TH1F *hd_Wm_PtLepPt15_3 = (TH1F *)hd_Wm_PtLepPt15 -> Clone("hd_Wm_PtLepPt15_3");
   hd_Wm_PtLepPt15_3  -> SetNameTitle("hd_Wm_PtLepPt15_3", "data- Z^{0}->ee, W^{-}->#tau#nu_{#tau} and QCD subtracted");


   c6c->Divide(2, 1);

   TLegend *leg6c = new TLegend(0.46, 0.70, 0.75, 0.9);
   leg6c -> AddEntry(hd_Wm_PtLepPt15_2,"STAR data - W/Z backgrounds", "F");
   leg6c -> AddEntry(hd_Wm_PtLepQCDPt15_2,"data driven QCD", "F" );

   c6c_1->cd();
   hd_Wm_PtLepPt15_2    -> Draw(); // data - W and Z bkgd subtracted
   hd_Wm_PtLepQCDPt15_2 -> SetFillStyle(3448);
   hd_Wm_PtLepQCDPt15_2 -> SetFillColor(kMagenta);
   hd_Wm_PtLepQCDPt15_2 -> Draw("same");
   leg6c                -> Draw();

   c6c_2->cd();
   hd_Wm_PtLepPt15_3->Draw();


   c6c->Print(outPath + "/plot_6c.eps");
   c6c->Print(outPath + "/plot_6c.png");


   TCanvas *c6d = new TCanvas("c6d", "W- sample - QCD data driven", 800, 400);


   TH1F *hd_Wm_PtLepQCD_1 = (TH1F *)hd_Wm_PtLepQCD -> Clone("hd_Wm_PtLepQCD_1");

   hd_Wm_PtLepQCD -> Scale(scale_Wm_QCD);

   TH1F *hd_Wm_PtLepQCD_2 = (TH1F *)hd_Wm_PtLepQCD -> Clone("hd_Wm_PtLepQCD_2");

   hd_Wm_PtLep    -> Add(hd_Wm_PtLep, hd_Wm_PtLepQCD, 1, -1);

   TH1F *hd_Wm_PtLep_3 = (TH1F *)hd_Wm_PtLep -> Clone("hd_Wm_PtLep_3");
   hd_Wm_PtLep_3  -> SetNameTitle("hd_Wm_PtLep_3", "data- Z^{0}->ee, W^{-}->#tau#nu_{#tau} and QCD subtracted");


   c6d->Divide(2, 1);

   c6d_1 -> cd();
   hd_Wm_PtLep_2    -> Draw();
   hd_Wm_PtLepQCD_2 -> SetFillStyle(3448);
   hd_Wm_PtLepQCD_2 -> SetFillColor(kMagenta);
   hd_Wm_PtLepQCD_2 -> Draw("same");
   c6d_2 -> cd();
   hd_Wm_PtLep_3    -> Draw();


   c6d->Print(outPath + "/plot_6d.eps");
   c6d->Print(outPath + "/plot_6d.png");


   TCanvas *c7 = new TCanvas("c7", "", 800, 400);

   c7-> SetTitle("W- sample summary ");
   c7->Divide(2, 1);

   TLegend *leg7 = new TLegend(0.47, 0.6, 0.75, 0.9);
   leg7 -> AddEntry(hd_Wm_PtLep_1,"STAR data", "F");
   leg7 -> AddEntry(hWmtt_Wm_PtLep_2,"PYTHIA W^{-}-> #tau#nu_{#tau} ", "F" );
   leg7 -> AddEntry(hZ_Wm_PtLep_2,"PYTHIA Z^{0}-> e^{+}e^{-}", "F");;
   leg7 -> AddEntry(hd_Wm_PtLepQCD_2,"data driven QCD", "F");

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
   hd_Wm_PtLepQCD_2->Draw("same");
   leg7            ->Draw();

   c7_2->cd();
   c7_2->SetLogy(0);
   // hd_Wm_PtLep_2->SetNameTitle("hd_Wm_PtLep","data (W-) - [Z^{0}->ee and W^{-}->#tau#nu_{#tau} subtracted]");
   hd_Wm_PtLep_3->SetMarkerStyle(20);
   hd_Wm_PtLep_3->Draw("E0");

   c7->Print(outPath + "/plot_7.eps");
   c7->Print(outPath + "/plot_7.png");


   TCanvas *c7b = new TCanvas("c7b", "", 800, 400);

   c7b -> SetTitle("W- sample summary - PtEle > 15 GeV");
   c7b -> Divide(2, 1);

   c7b_1 -> cd();
   c7b_1 -> SetLogy(0);

   hd_Wm_PtLepPt15_1    -> Draw(); // data - (No bkgd subtraction)

   hZ_Wm_PtLepPt15_2    -> SetFillStyle(3448);
   hZ_Wm_PtLepPt15_2    -> SetFillColor(kGreen);
   hZ_Wm_PtLepPt15_2    -> SetStats(0);
   hZ_Wm_PtLepPt15_2    -> Draw("same");

   hWmtt_Wm_PtLepPt15_2 -> SetFillStyle(3446);
   hWmtt_Wm_PtLepPt15_2 -> SetFillColor(kRed);
   hWmtt_Wm_PtLepPt15_2 -> SetStats(0);
   hWmtt_Wm_PtLepPt15_2 -> Draw("same");

   hd_Wm_PtLepQCDPt15_2 -> Draw("same");

   c7b_2->cd();
   c7b_2->SetLogy(0);
   //hd_Wm_PtLepPt15_2->SetNameTitle("hd_Wm_PtLepPt15","data (W-) - [Z^{0}->ee and W^{-}->#tau#nu_{#tau} subtracted]");
   hd_Wm_PtLepPt15_3 -> SetMarkerStyle(20);
   hd_Wm_PtLepPt15_3 -> Draw("E0");  // data - (All bkgd subtracted)

   c7b->Print(outPath + "/plot_7b.eps");
   c7b->Print(outPath + "/plot_7b.png");

   //TCanvas *c8TEST = new TCanvas("c8TEST", "Data/MC summary - PAPER FIGURE", 500, 400);
   //hd_Wp_PtLep_1 -> Draw("e");

   TCanvas *c8paper = new TCanvas("c8paper", "Data/MC summary - PAPER FIGURE", 500, 400);

   TH1F *hWp_Wp_PtLep_3 = (TH1F *)hWp_Wp_PtLep_2 -> Clone("hWp_Wp_PtLep_3");
   hWp_Wp_PtLep_3   -> SetFillStyle(0);
   hWp_Wp_PtLep_3   -> SetLineStyle(7);
   hWp_Wp_PtLep_3   -> SetLineWidth(2);
   hWp_Wp_PtLep_3   -> Add(hWp_Wp_PtLep_3, hd_Wp_PtLepQCD_2, 1, 1); // W->ev MC (+ W->tn & Z->ee & QCD backgrounds) 

   TH1F *hWm_Wm_PtLep_3 = (TH1F *)hWm_Wm_PtLep_2 -> Clone("hWm_Wm_PtLep_3");
   hWm_Wm_PtLep_3   -> SetFillStyle(0);
   hWm_Wm_PtLep_3   -> SetLineStyle(7);
   hWm_Wm_PtLep_3   -> SetLineWidth(2);
   hWm_Wm_PtLep_3   -> Add(hWm_Wm_PtLep_3, hd_Wm_PtLepQCD_2, 1, 1); // W->ev MC (+ W->tn & Z->ee & QCD backgrounds) 

   hWptt_Wp_PtLep_2 -> SetFillStyle(3305);
   hZ_Wp_PtLep_2    -> SetFillStyle(3395);
   hd_Wp_PtLepQCD_2 -> SetFillStyle(3744);
   hWmtt_Wm_PtLep_2 -> SetFillStyle(3305);
   hZ_Wm_PtLep_2    -> SetFillStyle(3395);
   hd_Wm_PtLepQCD_2 -> SetFillStyle(3744);

   //c8paper -> Divide(2, 1);
   c8paper -> Divide(1, 2);

   TLatex *textWp = new TLatex(0.15, 0.8, "W^{+} #rightarrow e^{+} #nu_{e}");
   textWp -> SetNDC(); 
   textWp -> SetTextFont(32);
   textWp -> SetTextSize(0.11);
   TLatex *textWm = new TLatex(0.15, 0.8+0.1, "W^{-} #rightarrow e^{-} #nu_{e}");
   textWm -> SetNDC(); 
   textWm -> SetTextFont(32);
   textWm -> SetTextSize(0.11);

   TLegend *leg9 = new TLegend(0.59, 0.45, 0.96, 0.89);
   leg9 -> SetFillColor(kWhite);
   leg9 -> SetFillStyle(0);
   leg9 -> SetLineColor(0);
   leg9 -> SetBorderSize(0);
   leg9 -> AddEntry(hd_Wp_PtLep_1,"STAR data", "P");
   leg9 -> AddEntry(hWp_Wp_PtLep_3,"PYTHIA MC + Bg ", "L");
   leg9 -> AddEntry(hWptt_Wp_PtLep_2,"PYTHIA W^{#pm}-> #tau#nu_{#tau} ", "F" );
   leg9 -> AddEntry(hZ_Wp_PtLep_2,"PYTHIA Z^{0}-> e^{+}e^{-}", "F");
   leg9 -> AddEntry(hd_Wp_PtLepQCD_2,"data driven QCD", "F");

   TLegend *leg9b = new TLegend(0.51, 0.6, 0.9, 0.9);
   leg9b -> SetFillColor(kWhite);
   leg9b -> SetFillStyle(0);
   leg9b -> SetLineColor(0);
   leg9b -> SetBorderSize(0);
   leg9b -> AddEntry(hd_Wm_PtLep_1,"STAR data", "P");
   leg9b -> AddEntry(hWm_Wm_PtLep_3,"PYTHIA MC + Bg ", "L");
   leg9b -> AddEntry(hWmtt_Wm_PtLep_2,"PYTHIA W^{-}-> #tau#nu_{#tau} ", "F" );
   leg9b -> AddEntry(hZ_Wm_PtLep_2,"PYTHIA Z^{0}-> e^{+}e^{-}", "F");;
   leg9b -> AddEntry(hd_Wm_PtLepQCD_2,"data driven QCD", "F");

   c8paper_1 -> cd();
   c8paper_1 -> SetLeftMargin(0.1);
   c8paper_1 -> SetRightMargin(0.1);
   c8paper_1 -> SetBottomMargin(0.01);
   c8paper_1 -> SetGrid(0,0);
   c8paper_1 -> SetLogy(1);
   hd_Wp_PtLep_1   -> GetYaxis() -> SetTitleOffset(0.6);
   hd_Wp_PtLep_1   -> GetXaxis() -> SetRangeUser(20, 70);
   hd_Wp_PtLep_1   -> SetStats(0);
   //hd_Wp_PtLep_1   -> SetNameTitle("hd_Wp_PtLep_1","W^{+}->e^{+}#nu_{e}");
   hd_Wp_PtLep_1   -> SetNameTitle("hd_Wp_PtLep_1","");
   hd_Wp_PtLep_1   -> SetTitleSize(0.07, "XY");
   hd_Wp_PtLep_1   -> SetLabelSize(0.07, "XY");
   hd_Wp_PtLep_1   -> SetLabelOffset(2.0, "X");
   Double_t max = hd_Wp_PtLep_1   -> GetMaximum();
   max = max *5 ;
   hd_Wp_PtLep_1   -> SetMaximum(max);
   hd_Wp_PtLep_1   -> Draw("e");
   hWptt_Wp_PtLep_2-> Draw("same");
   hZ_Wp_PtLep_2   -> Draw("same");
   hZ_Wp_PtLep_2   -> Draw("same");
   hd_Wp_PtLepQCD_2-> SetFillColor(kBlue);
   hd_Wp_PtLepQCD_2-> Draw("same");
   hWp_Wp_PtLep_3  -> Draw("same");
   hd_Wp_PtLep_1   -> Draw("sameaxis");
   textWp          -> Draw();
   leg9            -> Draw();

   c8paper_2 -> cd();
   c8paper_2 -> SetLeftMargin(0.1);
   c8paper_2 -> SetRightMargin(0.1);
   c8paper_2 -> SetTopMargin(0.0);
   c8paper_2 -> SetBottomMargin(0.15);
   c8paper_2 -> SetGrid(0,0);
   c8paper_2 -> SetLogy(1);
   hd_Wm_PtLep_1   -> GetYaxis() -> SetTitleOffset(0.6);
   hd_Wm_PtLep_1   -> GetXaxis() -> SetRangeUser(20, 70);
   hd_Wm_PtLep_1   -> SetStats(0);
   //hd_Wm_PtLep_1   -> SetNameTitle("hd_Wm_PtLep_1","W^{-}->e^{-}#nu_{e}");
   hd_Wm_PtLep_1   -> SetNameTitle("hd_Wm_PtLep_1","");
   hd_Wm_PtLep_1   -> SetTitleSize(0.07, "XY");
   hd_Wm_PtLep_1   -> SetLabelSize(0.07, "XY");
   hd_Wm_PtLep_1   -> Draw("e");
   hWmtt_Wm_PtLep_2-> Draw("same");
   hZ_Wm_PtLep_2   -> Draw("same");
   hZ_Wm_PtLep_2   -> Draw("same");
   hd_Wm_PtLepQCD_2-> SetFillColor(kBlue);
   hd_Wm_PtLepQCD_2-> Draw("same");
   hWm_Wm_PtLep_3  -> Draw("same");
   hd_Wm_PtLep_1   -> Draw("sameaxis");
   textWm          -> Draw();
   //leg9b           -> Draw();

   c8paper -> Print(outPathPaper + "/PaperPlot_DataMC.eps");
   c8paper -> Print(outPathPaper + "/PaperPlot_DataMC.png");


   TCanvas *c8paper15 = new TCanvas("c8paper15", "Data/MC summary - PAPER FIGURE PtEle > 15 GeV", 500, 400);
 
   TLine *line1 = new TLine(25, 0, 25, 100);
   line1->SetLineColor(1);
   line1->SetLineWidth(2);
   line1->SetLineStyle(5);

   TH1F *hWp_Wp_PtLepPt15_3 = (TH1F *)hWp_Wp_PtLepPt15_2 -> Clone("hWp_Wp_PtLepPt15_3");
   hWp_Wp_PtLepPt15_3   -> SetFillStyle(0);
   hWp_Wp_PtLepPt15_3   -> SetLineStyle(7);
   hWp_Wp_PtLepPt15_3   -> SetLineWidth(2);
   hWp_Wp_PtLepPt15_3   -> Add(hWp_Wp_PtLepPt15_3, hd_Wp_PtLepQCDPt15_2, 1, 1); // W->ev MC (+ W->tn & Z->ee & QCD backgrounds) 

   TH1F *hWm_Wm_PtLepPt15_3 = (TH1F *)hWm_Wm_PtLepPt15_2 -> Clone("hWm_Wm_PtLepPt15_3");
   hWm_Wm_PtLepPt15_3   -> SetFillStyle(0);
   hWm_Wm_PtLepPt15_3   -> SetLineStyle(7);
   hWm_Wm_PtLepPt15_3   -> SetLineWidth(2);
   hWm_Wm_PtLepPt15_3   -> Add(hWm_Wm_PtLepPt15_3, hd_Wm_PtLepQCDPt15_2, 1, 1); // W->ev MC (+ W->tn & Z->ee & QCD backgrounds) 

   hWptt_Wp_PtLepPt15_2 -> SetFillStyle(3305);
   hZ_Wp_PtLepPt15_2    -> SetFillStyle(3395);
   hd_Wp_PtLepQCDPt15_2 -> SetFillStyle(3744);
   hWmtt_Wm_PtLepPt15_2 -> SetFillStyle(3305);
   hZ_Wm_PtLepPt15_2    -> SetFillStyle(3395);
   hd_Wm_PtLepQCDPt15_2 -> SetFillStyle(3744);

   c8paper15 -> Divide(1, 2);

   TLatex *textWp = new TLatex(0.33, 0.79, "W^{+} #rightarrow e^{+} #nu_{e}");
   textWp -> SetNDC(); 
   textWp -> SetTextFont(32);
   textWp -> SetTextSize(0.12);
   TLatex *textWm = new TLatex(0.33, 0.79+0.1, "W^{-} #rightarrow e^{-} #nu_{e}");
   textWm -> SetNDC(); 
   textWm -> SetTextFont(32);
   textWm -> SetTextSize(0.12);

   c8paper15_1 -> cd();
   c8paper15_1 -> SetLeftMargin(0.1);
   c8paper15_1 -> SetRightMargin(0.1);
   c8paper15_1 -> SetBottomMargin(0.01);
   c8paper15_1 -> SetGrid(0,0);
   c8paper15_1 -> SetLogy(1);
   hd_Wp_PtLepPt15_1   -> GetYaxis() -> SetTitleOffset(0.6);
   hd_Wp_PtLepPt15_1   -> GetXaxis() -> SetRangeUser(10, 70);
   hWp_Wp_PtLepPt15_3  -> GetXaxis() -> SetRangeUser(15, 70);
   hd_Wp_PtLepPt15_1   -> SetStats(0);
   hd_Wp_PtLepPt15_1   -> SetNameTitle("hd_Wp_PtLepPt15_1","");
   hd_Wp_PtLepPt15_1   -> SetTitleSize(0.07, "XY");
   hd_Wp_PtLepPt15_1   -> SetLabelSize(0.07, "XY");
   hd_Wp_PtLepPt15_1   -> SetLabelOffset(2.0, "X");
   Double_t max = hd_Wp_PtLepPt15_1   -> GetMaximum();
   max = max *5;
   hd_Wp_PtLepPt15_1   -> SetMaximum(max);
   hd_Wp_PtLepPt15_1   -> Draw("e");
   hWptt_Wp_PtLepPt15_2-> Draw("same");
   hZ_Wp_PtLepPt15_2   -> Draw("same");
   hZ_Wp_PtLepPt15_2   -> Draw("same");
   hd_Wp_PtLepQCDPt15_2-> SetFillColor(kBlue);
   hd_Wp_PtLepQCDPt15_2-> Draw("same");
   hWp_Wp_PtLepPt15_3  -> Draw("same");
   line1 -> DrawLine(25, 0, 25, hd_Wp_PtLepPt15_1   -> GetMaximum());
   //line1 -> DrawLine(25, 0, 25, 1000);
   hd_Wp_PtLepPt15_1   -> Draw("sameaxis");
   textWp          -> Draw();
   leg9            -> Draw();

   c8paper15_2 -> cd();
   c8paper15_2 -> SetLeftMargin(0.1);
   c8paper15_2 -> SetRightMargin(0.1);
   c8paper15_2 -> SetTopMargin(0.0);
   c8paper15_2 -> SetBottomMargin(0.15);
   c8paper15_2 -> SetGrid(0,0);
   c8paper15_2 -> SetLogy(1);
   hd_Wm_PtLepPt15_1   -> GetYaxis() -> SetTitleOffset(0.6);
   hd_Wm_PtLepPt15_1   -> GetXaxis() -> SetRangeUser(10, 70);
   hWm_Wm_PtLepPt15_3  -> GetXaxis() -> SetRangeUser(15, 70);
   hd_Wm_PtLepPt15_1   -> SetStats(0);
   hd_Wm_PtLepPt15_1   -> SetNameTitle("hd_Wm_PtLepPt15_1","");
   hd_Wm_PtLepPt15_1   -> SetTitleSize(0.07, "XY");
   hd_Wm_PtLepPt15_1   -> SetLabelSize(0.07, "XY");
   Double_t max_wm = hd_Wm_PtLepPt15_1   -> GetMaximum();
   max_wm = max_wm *5;
   hd_Wm_PtLepPt15_1   -> SetMaximum(max_wm);
   hd_Wm_PtLepPt15_1   -> Draw("e");
   hWmtt_Wm_PtLepPt15_2-> Draw("same");
   hZ_Wm_PtLepPt15_2   -> Draw("same");
   hZ_Wm_PtLepPt15_2   -> Draw("same");
   hd_Wm_PtLepQCDPt15_2-> SetFillColor(kBlue);
   hd_Wm_PtLepQCDPt15_2-> Draw("same");
   hWm_Wm_PtLepPt15_3  -> Draw("same");
   line1 -> DrawLine(25, 0, 25, hd_Wm_PtLepPt15_1   -> GetMaximum());
   hd_Wm_PtLepPt15_1   -> Draw("sameaxis");
   textWm          -> Draw();

   // Write the output of the vectors in Paper Fig. 2 (PAER FIGURE ptEle > 15 GeV)
   cout << " " << endl;
   cout << "----------------- PAPER FIGURE 2 UP ----------------" << endl;
   cout << " W+ -> e v " << endl;
   for(Int_t i=1; i <= hd_Wp_PtLepPt15_1 -> GetNbinsX(); i++) {
     Double_t eval_x   = hd_Wp_PtLepPt15_1     -> GetBinCenter(i);
     Double_t data_y   = hd_Wp_PtLepPt15_1     -> GetBinContent(i);
     Double_t data_error_y   = hd_Wp_PtLepPt15_1 -> GetBinError(i);
     Double_t WpBg_y   = hWp_Wp_PtLepPt15_3    -> GetBinContent(i);
     Double_t Wptt_y   = hWptt_Wp_PtLepPt15_2  -> GetBinContent(i);
     Double_t Z_Wp_y   = hZ_Wp_PtLepPt15_2     -> GetBinContent(i);
     Double_t QCD_Wp_y = hd_Wp_PtLepQCDPt15_2  -> GetBinContent(i);
     cout << "bin= " << i << " -> " << "PtEle = " << eval_x <<
       "; #Events(STAR data) = " << data_y << " +- " << data_error_y <<
       "; #Events(PYTHIA MC + Bg) = " << WpBg_y <<
       "; #Events(PYTHIA Wp to tt) = " << Wptt_y <<
       "; #Events(PYTHIA Z0 to ee) = " << Z_Wp_y <<
       "; #Events(data driven QCD) = " << QCD_Wp_y << endl;
   }
   cout << "----------------- PAPER FIGURE 2 DOWN ----------------" << endl;
   cout << " W- -> e v " << endl;
   for(Int_t i=1; i <= hd_Wm_PtLepPt15_1 -> GetNbinsX(); i++) {
     Double_t eval_x   = hd_Wm_PtLepPt15_1     -> GetBinCenter(i);
     Double_t data_y   = hd_Wm_PtLepPt15_1     -> GetBinContent(i);
     Double_t data_error_y   = hd_Wm_PtLepPt15_1 -> GetBinError(i);
     Double_t WpBg_y   = hWm_Wm_PtLepPt15_3    -> GetBinContent(i);
     Double_t Wptt_y   = hWmtt_Wm_PtLepPt15_2  -> GetBinContent(i);
     Double_t Z_Wp_y   = hZ_Wm_PtLepPt15_2     -> GetBinContent(i);
     Double_t QCD_Wp_y = hd_Wm_PtLepQCDPt15_2  -> GetBinContent(i);
     cout << "bin= " << i << " -> " << "PtEle = " << eval_x <<
       "; #Events(STAR data) = " << data_y << " +- " << data_error_y <<
       "; #Events(PYTHIA MC + Bg) = " << WpBg_y <<
       "; #Events(PYTHIA Wp to tt) = " << Wptt_y <<
       "; #Events(PYTHIA Z0 to ee) = " << Z_Wp_y <<
       "; #Events(data driven QCD) = " << QCD_Wp_y << endl;
   }
   cout << "-------------------------------------------------" << endl;
   cout << " " << endl;

   c8paper15 -> Print(outPathPaper + "/PaperPlot_DataMC_Pt15.eps");
   c8paper15 -> Print(outPathPaper + "/PaperPlot_DataMC_Pt15.png");


   // Calculate Signal/Bkgnd ratios and their errors:

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

   TCanvas *c9paper   = new TCanvas("c9paper", "W reconstructed Pt", 400, 800);

   // Normalize to the integral:
   //hWp_Wp_PtWRecoCorrected_zoomin -> Scale(hd_Wp_PtWReco_zoomin->Integral()/hWp_Wp_PtWReco_zoomin->Integral());

   // Normalize to the luminosity
   hWp_Wp_PtWRecoCorrected_zoomin   -> Scale(scaleWp);
   hWptt_Wp_PtWRecoCorrected_zoomin -> Scale(scaleWptt);

   c9paper   -> Divide(1,3);
   c9paper_1 -> cd();
   c9paper_1 -> SetLogy(1);
   hd_Wp_PtWRecoCorrected_zoomin    ->Rebin(2);
   //hd_Wp_PtWRecoCorrected_zoomin    ->SetMinimum(0);
   hd_Wp_PtWRecoCorrected_zoomin    ->GetYaxis()->SetTitleOffset(1.8);

   hWp_Wp_PtWRecoCorrected_zoomin   ->Rebin(2);
   hWp_Wp_PtWRecoCorrected_zoomin   -> SetFillStyle(0);
   hWp_Wp_PtWRecoCorrected_zoomin   -> SetLineStyle(7);
   hWp_Wp_PtWRecoCorrected_zoomin   -> SetLineColor(kRed);
   hWp_Wp_PtWRecoCorrected_zoomin   -> SetLineWidth(2);
   hWptt_Wp_PtWRecoCorrected_zoomin ->Rebin(2);
   hWptt_Wp_PtWRecoCorrected_zoomin-> SetFillStyle(3446);
   hWptt_Wp_PtWRecoCorrected_zoomin ->SetFillColor(kRed);
   hd_Wp_PtWRecoCorrected_zoomin    ->SetMarkerStyle(20);
   hd_Wp_PtWRecoCorrected_zoomin    ->SetTitle("W+ sample");
   hd_Wp_PtWRecoCorrected_zoomin    ->Draw("E0");
   hWp_Wp_PtWRecoCorrected_zoomin   ->Draw("same");
   hWptt_Wp_PtWRecoCorrected_zoomin ->Draw("same");
   hd_Wp_PtWRecoCorrected_zoomin    ->Draw("E0 same");

   c9paper_2->cd();
   hd_Wp_PtWRecoCorrected_zoomin -> Draw("E0");
   hWptt_Wp_PtWRecoCorrected_zoomin -> Draw("same");

   c9paper_3->cd();
   hWptt_Wp_PtWRecoCorrected_zoomin -> Draw("");
 
   TLegend *leg8 = new TLegend(0.4, 0.7, 0.75, 0.9);
   leg8 -> SetFillColor(kWhite);
   //leg9b -> AddEntry(hd_Wp_PtLep_1,"STAR data", "P");
   //leg9b -> AddEntry(hWptt_Wp_PtLep_2,"PYTHIA W^{-}-> #tau#nu_{#tau} ", "F" );
   //leg9b -> AddEntry(hZ_Wp_PtLep_2,"PYTHIA Z^{0}-> e^{+}e^{-}", "F");
   //leg9b -> AddEntry(hd_Wp_PtLepQCD_2,"data driven QCD", "F");

   leg8 -> AddEntry(hd_Wp_PtWRecoCorrected_zoomin,"STAR data", "P");
   leg8 -> AddEntry(hWp_Wp_PtWRecoCorrected_zoomin,"PYTHIA  W^{+}-> e#nu_{e} + Bg", "L");
   leg8 -> AddEntry(hWptt_Wp_PtWRecoCorrected_zoomin,"PHYTHIA W^{+}-> #tau#nu_{#tau}", "F");
   leg8 -> Draw();


   //c9paper -> SaveAs(outPath + "/PaperPlot_DataMc_WpPt_zoomin.png");
   //c9paper -> SaveAs(outPath + "/PaperPlot_DataMc_WpPt_zoomin.eps");

}
