double integrateGraph(TGraph* graph, int first=0, int last=-1) {

if (first < 0) first = 0;
   Int_t fNpoints = graph->GetN();
   if (last < 0) last = fNpoints - 1;
   if (last >= graph->GetN()) last = fNpoints - 1;
   if (first >= last) return 0;
   Int_t np = last - first + 1;
   Double_t sum = 0.0;
   double* fY = graph->GetY();
   double* fX = graph->GetX();
   //for(Int_t i=first;i<=last;i++) {
   //   Int_t j = first + (i-first+1)%np;
   //   sum += TMath::Abs(fX[i]*fY[j]);
   //   sum -= TMath::Abs(fY[i]*fX[j]);
   //}
   for (Int_t i = first; i <= last; i++) {
      Int_t j = first + (i - first + 1) % np;
      sum += (fY[i] + fY[j]) * (fX[j] - fX[i]);
   }
   return 0.5 * TMath::Abs(sum);

}


double scaleGraph(TGraph* graph, float scale=1, int first=0, int last=-1) {

if (first < 0) first = 0;
   Int_t fNpoints = graph->GetN();
   if (last < 0) last = fNpoints - 1;
   if (last >= graph->GetN()) last = fNpoints - 1;
   if (first >= last) return 0;
   double* fX = graph->GetX();
   double* fY = graph->GetY();
   //vector<double> fYsc(fNpoints, 0.); // creates a vector with fNpoints and initializes it to 0
   //cout << fX << " " << fY << " " << fYsc << endl;
   //cout << fX << " " << fY << endl;

   for (Int_t i = first; i <= last; i++) {
      fY[i] *= scale;
      graph->SetPoint(i,fX[i],fY[i]);
   }

}



void analysis_Z0_2011()
{


   // Load the libraries:
   //gROOT->Macro("/star/u/fazio/offline/users/fazio/vbasym/macros/loadLibs.C");


   TString inPathOld = "/star/institutions/bnl_me/fazio/vbana_out/";
   TString inPath    = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString outPath   = "~/vbasym_results/plots/4prelim";

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

   // open histogram files
   //TFile *fileData   = TFile::Open(inPath + "run11_pp_transverse_-z_--jpm_0.5_--run_11/hist/vbana.root");
   TFile *fileData   = TFile::Open(inPath + "run11_pp_transverse_zboson/hist/vbana.root");
   //TFile *fileData12 = TFile::Open(inPath + "run12_pp_long_j3_-z_--jpm_0.5_--run_12/hist/vbana.root");
   //TFile *fileMC     = TFile::Open(inPath + "run11_mc_Z02ee.lis_-m_-z_--jpm_0.5/hist/vbana.root");

   TFile *fileDataW  = TFile::Open(inPathOld + "run11_pp_transverse_-w_--jpm_0.5_--run_11_vbana.root");
   TFile *fileMCZ    = TFile::Open(inPath + "run11_mc_Z02ee.lis_zboson/hist/vbana.root");



   // Get histograms from files
   TH1 *hd_Cand1Pt        = (TH1*) fileData->Get("event_z/hCandidate1Pt");
   TH1 *hd_Cand2Pt        = (TH1*) fileData->Get("event_z/hCandidate2Pt");
   TH1 *hd_Cand1Eta       = (TH1*) fileData->Get("event_z/hCandidate1Eta");
   TH1 *hd_Cand2Eta       = (TH1*) fileData->Get("event_z/hCandidate2Eta");
   TH1 *hd_ZBosPt         = (TH1*) fileData->Get("event_z/hZ0_Pt");
   TH1 *hd_ZBosPt_zoomin  = (TH1*) fileData->Get("event_z/hZ0_Pt_zoomin");
   TH1 *hd_ZBosPz         = (TH1*) fileData->Get("event_z/hZ0_Pz");
   TH1 *hd_ZBosPz_zoomin  = (TH1*) fileData->Get("event_z/hZ0_Pz_zoomin");
   //TH1 *hd_ZBosPtRun12    = (TH1*) fileData12->Get("Z0_event_pass_final/hZ0_Pt");
   TH1 *hd_ZBosEta        = (TH1*) fileData->Get("event_z/hZ0_Eta");
   TH1 *hd_ZBosRapidity   = (TH1*) fileData->Get("event_z/hZ0_Rapidity");
   TH1 *hd_ZBosMass       = (TH1*) fileData->Get("event_z/hZ0_MassInv");
   TH1 *hZ_ZBosPt         = (TH1*) fileMCZ ->Get("event_z/hZ0_Pt");
   TH1 *hZ_ZBosPt_zoomin  = (TH1*) fileMCZ ->Get("event_z/hZ0_Pt_zoomin");
   TH1 *hZ_ZBosPz         = (TH1*) fileMCZ ->Get("event_z/hZ0_Pz");
   TH1 *hZ_ZBosPz_zoomin  = (TH1*) fileMCZ ->Get("event_z/hZ0_Pz_zoomin");
   TH1 *hZ_ZBosRapidity   = (TH1*) fileMCZ ->Get("event_z/hZ0_Rapidity");

   /*
   TH1 *hd_Cand1Pt     = (TH1*) fileData->Get("Z0_event_pass_final/hCandidate1Pt");
   TH1 *hd_Cand2Pt     = (TH1*) fileData->Get("Z0_event_pass_final/hCandidate2Pt");
   TH1 *hd_Cand1Eta    = (TH1*) fileData->Get("Z0_event_pass_final/hCandidate1Eta");
   TH1 *hd_Cand2Eta    = (TH1*) fileData->Get("Z0_event_pass_final/hCandidate2Eta");
   TH1 *hd_ZBosPt      = (TH1*) fileData->Get("Z0_event_pass_final/hZ0_Pt");
   TH1 *hd_ZBosPtRun12 = (TH1*) fileData12->Get("Z0_event_pass_final/hZ0_Pt");
   TH1 *hd_ZBosEta     = (TH1*) fileData->Get("Z0_event_pass_final/hZ0_Eta");
   TH1 *hd_ZBosMass    = (TH1*) fileData->Get("Z0_event_pass_final/hZ0_MassInv");
   */

   TH1 *hd_Wp_PtWRecoCorrected          = (TH1*) fileDataW->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hd_Wp_PtWRecoCorrected_zoomin   = (TH1*) fileDataW->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");


 
   TCanvas *c1 = new TCanvas("c1", "", 800, 800);

   c1-> SetTitle("Z0- electron candidates Pt");

   c1->Divide(1, 2);
   
   c1_1->cd();
   hd_Cand1Pt ->GetXaxis()->SetTitle("Lepton candidate 1 -  P_{T} (GeV/c)");
   hd_Cand1Pt ->Rebin(2);
   hd_Cand1Pt ->SetFillColor(kYellow);
   hd_Cand1Pt ->Draw(); 
   c1_2->cd();
   hd_Cand2Pt ->GetXaxis()->SetTitle("Lepton candidate 2 -  P_{T} (GeV/c)");
   hd_Cand2Pt ->Rebin(2);
   hd_Cand2Pt ->SetFillColor(kYellow);
   hd_Cand2Pt ->Draw(); 

   /*
   c1_3->cd();
   t ->Draw("mCand1P3.Pt()", cut1 && cut2); 
   c1_4->cd();
   t ->Draw("mP4Cand2.Pt()", cut1 && cut2); 
   */

   c1->Print(outPath + "/Z0_plot_1.png");


   TCanvas *c1b = new TCanvas("c1b", "", 800, 800);

   c1b-> SetTitle("Z0- electron candidates eta");

   c1b-> Divide(1, 2);
   
   c1b_1->cd();
   hd_Cand1Eta ->GetXaxis()->SetTitle("Lepton candidate 1 -  #eta");
   hd_Cand1Eta ->SetFillColor(kYellow);
   hd_Cand1Eta ->Draw(); 
   c1b_2->cd();
   hd_Cand2Eta ->GetXaxis()->SetTitle("Lepton candidate 2 -  #eta");
   hd_Cand2Eta ->SetFillColor(kYellow);
   hd_Cand2Eta ->Draw(); 

   /*
   c1_3->cd();
   t ->Draw("mCand1P3.Pt()", cut1 && cut2); 
   c1_4->cd();
   t ->Draw("mP4Cand2.Pt()", cut1 && cut2); 
   */

   c1b->Print(outPath + "/Z0_plot_1b.png");
   c1b->Print(outPath + "/Z0_plot_1b.eps");

 
   TCanvas *c2 = new TCanvas("c2", "", 800, 400);

   TGraph   *g1            = new TGraph("./curves/0304002.Wptcurve.txt","%lg %lg");   
   Double_t int_g1         =  integrateGraph(g1); // the method Integral() not yet implemented on TGraph in ROOT4STAR  
   Double_t int_hd_ZBosPt  =  hd_ZBosPt  -> Integral();  
   float    scale_g1       =  int_hd_ZBosPt / int_g1;
   scaleGraph(g1, 1.35*scale_g1);


   c2-> SetTitle("Z0- invariant mass and Pt");
   /*
   c2-> Divide(1, 2);
   
   c2_1->cd();
   hd_ZBosPt   -> SetMarkerStyle(20);
   hd_ZBosPt   -> Rebin(2);
   hd_ZBosPt   -> Draw("E0"); 
   g1          -> SetLineColor(kBlue); 
   g1          -> SetLineWidth(3); 
   */

   c2->cd(); 
   //hd_ZBosMass -> Rebin(3);
   hd_ZBosMass -> SetMarkerStyle(20);
   hd_ZBosMass -> Rebin(2);
   hd_ZBosMass -> Draw("E0");  

   c2->Print(outPath + "/Z0_plot_2.png");  
   c2->Print(outPath + "/Z0_plot_2.eps");  

 
   TCanvas *c3 = new TCanvas("c3", "", 800, 800);

   c3-> SetTitle("Z0- Eta");

   c3-> Divide(1, 2);
   
   c3->cd();
   hd_ZBosEta   -> Rebin(2);
   hd_ZBosEta   -> SetMarkerStyle(20);
   hd_ZBosEta   -> Rebin(2);
   hd_ZBosEta   -> Draw("E0"); 

   c3->Print(outPath + "/Z0_plot_3.png");  
   c3->Print(outPath + "/Z0_plot_3.eps");  

  
   /*
   TCanvas *c4 = new TCanvas("c4", "", 800, 800);

   hd_ZBosPt      -> Sumw2();
   hd_ZBosPtRun12 -> Sumw2();

   TH1F *hd_ZBosPtRun1112 = (TH1F *)hd_ZBosPtRun12->Clone("hd_ZBosPtRun1112");
   hd_ZBosPtRun1112 -> Add(hd_ZBosPt, hd_ZBosPtRun12);

   c4-> SetTitle("Z0-Pt run12");

   c4-> Divide(1, 4);
   
   c4_1->cd();
   hd_ZBosPtRun12   -> SetTitle("Run12");
   hd_ZBosPtRun12   -> SetMarkerStyle(20);
   //hd_ZBosPtRun12   -> SetAxisRange(0, 9, "X");
   hd_ZBosPtRun12   -> Draw("E0"); 
   //g1               -> SetLineColor(kBlue); 
   //g1               -> SetLineWidth(3); 

   c4_2->cd();
   hd_ZBosPtRun1112 -> SetTitle("Run11 + Run12");
   hd_ZBosPtRun1112 -> SetMarkerStyle(20);
   //hd_ZBosPtRun1112 -> SetAxisRange(0, 9, "X");
   hd_ZBosPtRun1112 -> DrawCopy("E0"); 

   c4_3->cd();
   //hd_Wp_PtWRecoCorrected -> SetTitle("W+ run11");
   //hd_Wp_PtWRecoCorrected -> SetMarkerStyle(20);
   //hd_Wp_PtWRecoCorrected -> Draw("E0");
   hd_ZBosPtRun1112 -> SetTitle("Run11 + Run12");
   hd_ZBosPtRun1112 -> SetMarkerStyle(20);
   hd_ZBosPtRun1112 -> SetAxisRange(0, 9, "X");
   hd_ZBosPtRun1112 -> Draw("E0"); 

   c4_4->cd();
   hd_Wp_PtWRecoCorrected_zoomin -> Rebin(2);
   hd_Wp_PtWRecoCorrected_zoomin -> SetTitle("W+ run11");
   hd_Wp_PtWRecoCorrected_zoomin -> SetMarkerStyle(20);
   hd_Wp_PtWRecoCorrected_zoomin -> Draw("E0");
 
   c4->Print(outPath + "/Z0_plot_4.png");  
   c4->Print(outPath + "/Z0_plot_4.eps");  
   */

   TCanvas *c5   = new TCanvas("c5", "", 400, 400);

   TH1F * hd_ZBosPt_1 = (TH1F *) hd_ZBosPt->Clone(" hZ_ZBosPt_1");

   TH1F * hZ_ZBosPt_1 = (TH1F *) hZ_ZBosPt->Clone(" hZ_ZBosPt_1");
   hZ_ZBosPt_1   -> Rebin(2);

   // Normalize to the integral:
   hZ_ZBosPt_1->Scale(hd_ZBosPt_1->Integral()/hZ_ZBosPt_1->Integral());

   c5->cd();
   hZ_ZBosPt_1    ->SetStats(0);
   hZ_ZBosPt_1    ->Draw();

   hZ_ZBosPt_1    ->SetMarkerStyle(20);
   hd_ZBosPt_1    ->Draw("E0 same");
   //hZ_ZBosPt_1    ->Draw("same");
   //hd_ZBosPt_1    ->Draw("E0 same");

  
   TLegend *leg1 = new TLegend(0.45, 0.75, 0.75, 0.9);
   leg1 -> AddEntry(hd_ZBosPt_1,"STAR 2011 tran.", "P");
   leg1 -> AddEntry(hZ_ZBosPt_1,"PHYTHIA", "F");
   leg1 -> Draw();

   c5->SaveAs(outPath + "/plot_DataMc_Z0Pt.png");
   c5->SaveAs(outPath + "/plot_DataMc_Z0Pt.eps");


   TCanvas *c5b   = new TCanvas("c5b", "", 400, 400);

   TH1F * hd_ZBosPz_1 = (TH1F *) hd_ZBosPz->Clone(" hZ_ZBosPz_1");

   TH1F * hZ_ZBosPz_1 = (TH1F *) hZ_ZBosPz->Clone(" hZ_ZBosPz_1");
   hd_ZBosPz_1   -> Rebin(4);
   hZ_ZBosPz_1   -> Rebin(4);

   // Normalize to the integral:
   hZ_ZBosPz_1->Scale(hd_ZBosPz_1->Integral()/hZ_ZBosPz_1->Integral());

   c5b->cd();
   hZ_ZBosPz_1    ->SetStats(0);
   //hZ_ZBosPz_1    ->Draw();

   hd_ZBosPz_1    ->SetMarkerStyle(20);
   hd_ZBosPz_1    ->SetTitle("; Z^{0}-P_{z}; Events");
   hd_ZBosPz_1    ->Draw("E0");
   hZ_ZBosPz_1    ->Draw("same");
   hd_ZBosPz_1    ->Draw("E0 same");

  
   TLegend *legZPz = new TLegend(0.45, 0.75, 0.75, 0.9);
   legZPz -> AddEntry(hd_ZBosPz_1,"STAR 2011 tran.", "P");
   legZPz -> AddEntry(hZ_ZBosPz_1,"PHYTHIA", "F");
   legZPz -> Draw();

   c5b->SaveAs(outPath + "/plot_DataMc_Z0Pz.png");
   c5b->SaveAs(outPath + "/plot_DataMc_Z0Pz.eps");


   TCanvas *c5c   = new TCanvas("c5c", "", 400, 400);

   TH1F * hd_ZBosRapidity_1 = (TH1F *) hd_ZBosRapidity->Clone(" hZ_ZBosRapidity_1");

   TH1F * hZ_ZBosRapidity_1 = (TH1F *) hZ_ZBosRapidity->Clone(" hZ_ZBosRapidity_1");
   //hZ_ZBosRapidity_1   -> Rebin(2);

   // Normalize to the integral:
   hZ_ZBosRapidity_1->Scale(hd_ZBosRapidity_1->Integral()/hZ_ZBosRapidity_1->Integral());

   c5c->cd();
   hZ_ZBosRapidity_1    -> SetStats(0);
   //hZ_ZBosRapidity_1    -> Draw();

   hZ_ZBosRapidity_1    -> SetMarkerStyle(20);
   hd_ZBosRapidity_1    -> SetMarkerStyle(20);
   hd_ZBosRapidity_1    -> SetAxisRange(-1, 0.8, "X");
   hd_ZBosRapidity_1    -> Draw("E0");
   hZ_ZBosRapidity_1    -> Draw("same");
   hd_ZBosRapidity_1    -> Draw("E0 same");

  
   TLegend *leg2 = new TLegend(0.5, 0.75, 0.75, 0.9);
   leg2 -> AddEntry(hd_ZBosRapidity_1,"STAR 2011 tran.", "P");
   leg2 -> AddEntry(hZ_ZBosRapidity_1,"PHYTHIA", "F");
   leg2 -> Draw();

   c5c->SaveAs(outPath + "/plot_DataMc_Z0Rapidity.png");
   c5c->SaveAs(outPath + "/plot_DataMc_Z0Rapidity.eps");

}
