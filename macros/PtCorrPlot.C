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


void PtCorrPlot()
{

   //gROOT->Reset();

   // Load the libraries:
   //gROOT->Macro("/star/u/fazio/offline/users/fazio/vbasym/macros/loadLibs.C");

   TString inPath  = "/star/institutions/bnl_me/fazio/vbana_out/";
   TString outPath = "~/vbasym_results/plots/";


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

   // open histogram files
   TFile *fileData      = TFile::Open(inPath + "run11_pp_transverse_-w_--jpm_0.5_--run_11_vbana.root");
   TFile *fileMCWp      = TFile::Open(inPath + "run11_mc_Wp2enu.lis_-m_-w_--jpm_0.5_vbana.root");
   TFile *fileMCWpOld   = TFile::Open(inPath + "run11_mc_Wp2enu.lis_-m_-w_--jpm_0.5_vbana_OldStylePtCorr.root");


   // fill histograms
   TH1 *hd_Wp_PtWReco                       = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt");
   TH1 *hd_Wp_PtWReco_zoomin                = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hd_Wp_PtWRecoCorrected              = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hd_Wp_PtWRecoCorrected_zoomin       = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_PtWReco                         = (TH1*) fileMCWp->Get("event_pass_wbos/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_PtWReco_zoomin                  = (TH1*) fileMCWp->Get("event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWp_PtWRecoCorrected                = (TH1*) fileMCWp->Get("event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWp_PtWRecoCorrected_zoomin         = (TH1*) fileMCWp->Get("event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_Wp_PtWRecoCorrected                      = (TH1*) fileMCWp->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWp_Wp_PtWRecoCorrected_zoomin               = (TH1*) fileMCWp->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   //TH1 *hWp_PtWRecoCorrectedOldStyle        = (TH1*) fileMCWpOld->Get("event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   //TH1 *hWp_PtWRecoCorrectedOldStyle_zoomin = (TH1*) fileMCWpOld->Get("event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_PtWGen                          = (TH1*) fileMCWp->Get("event_mc_pass_wbos/hWBosonPt");
   TH1 *hWp_PtWGen_zoomin                   = (TH1*) fileMCWp->Get("event_mc_pass_wbos/hWBosonPt_zoomin");

   TH2 *hWp_PtRecoil_vs_PtWGen              = (TH2 *)fileMCWp->Get("event_mc_pass_wbos/hTrackRecoilTpcNeutralsPtVsWBosonPt_copy");
   TH2 *hWp_PtRecoilCorr_vs_PtWGen          = (TH2 *)fileMCWp->Get("event_mc_pass_wbos/hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy");

   TH1 *hWp_PhiRecoil_vs_PhiWGen            = (TH1 *)fileMCWp->Get("event_mc_pass_wbos/hTrackRecoilPhiVsWBosonPhi");


   // Luminosities
   float lumiDataTot       = 24.42; // pb-1
   float lumiDataEff       = 23.99; // pb-1
   float lumiMC_Z          = 1621;  // pb-1
   float lumiMC_WpToENU    = 1925;  // pb-1
   float lumiMC_WpToTauTau = 1945;  // pb-1
   float lumiMC_WmToTauTau = 1802;  // pb-1

   // Scale factor for normalization to the luminosity
   float scaleWp = lumiDataTot / lumiMC_WpToENU;


   /*
   TCanvas *c1   = new TCanvas("c1", "", 400, 400);

   TGraph *g1       = new TGraph("./curves/0304002.Wptcurve.txt","%lg %lg");   
   Double_t int_g1  =  integrateGraph(g1); // the method Integral() not yet implemented on TGraph in ROOT4STAR  
   Double_t int_hWp_PtWRecoCorrectedOldStyle_zoomin    =  hWp_PtWRecoCorrectedOldStyle_zoomin  -> Integral(); 
   float scale_g1   = int_hWp_PtWRecoCorrectedOldStyle_zoomin / int_g1;
   scaleGraph(g1, 0.5*scale_g1);

   c1->cd();
   hWp_PtWRecoCorrectedOldStyle_zoomin ->GetYaxis()->SetTitleOffset(1.8);
   hWp_PtWRecoCorrectedOldStyle_zoomin ->SetFillStyle(3448);
   hWp_PtWRecoCorrectedOldStyle_zoomin ->SetFillColor(kGreen);
   hWp_PtWRecoCorrectedOldStyle_zoomin ->Draw();
   hWp_PtWReco_zoomin          ->Draw("same");
   hWp_PtWRecoCorrectedOldStyle_zoomin ->Draw("same");
   g1                            -> SetLineColor(kBlue); 
   g1                            -> SetLineWidth(3); 
   g1                            -> Draw("same");
   TLegend *leg1 = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg1 -> AddEntry(hWp_PtWReco_zoomin,"Before P_{T} correction", "F");
   leg1 -> AddEntry(hWp_PtWRecoCorrectedOldStyle_zoomin,"After P_{T} correction", "F" );
   leg1 -> AddEntry(g1,"RhicBOS 500 GeV", "l");
   leg1 -> Draw();


   c1->SaveAs(outPath + "/plot_PtCorr_OldStyle.png");
   */

   TCanvas *c2   = new TCanvas("c2", "", 400, 400);

   TGraph *g1       = new TGraph("./curves/0304002.Wptcurve.txt","%lg %lg");   
   Double_t int_g1  =  integrateGraph(g1); // the method Integral() not yet implemented on TGraph in ROOT4STAR  
   Double_t int_hWp_PtWRecoCorrected_zoomin    =  hWp_PtWRecoCorrected_zoomin  -> Integral(); 
   float scale_g1   = int_hWp_PtWRecoCorrected_zoomin / int_g1;
   scaleGraph(g1, 0.5*scale_g1);

   c2->cd();
   hWp_PtWReco_zoomin ->GetYaxis()->SetTitleOffset(1.8);
   hWp_PtWRecoCorrected_zoomin ->SetFillStyle(3448);
   hWp_PtWRecoCorrected_zoomin ->SetFillColor(kGreen);
   //hWp_PtWRecoCorrected_zoomin ->Draw();
   hWp_PtWReco_zoomin          ->Draw();
   hWp_PtWRecoCorrected_zoomin ->Draw("same");
   g1                            -> SetLineColor(kBlue); 
   g1                            -> SetLineWidth(3); 
   g1                            -> Draw("same");
   TLegend *leg1 = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg1 -> AddEntry(hWp_PtWReco_zoomin,"Before P_{T} correction", "F");
   leg1 -> AddEntry(hWp_PtWRecoCorrected_zoomin,"After P_{T} correction", "F" );
   leg1 -> AddEntry(g1,"RhicBOS 500 GeV", "l");
   leg1 -> Draw();


   c2->SaveAs(outPath + "/plot_PtCorr.png");
   c2->SaveAs(outPath + "/plot_PtCorr.eps");


   TCanvas *c2b   = new TCanvas("c2b", "", 400, 400);

   c2b->cd();
   hWp_PtWReco_zoomin ->GetYaxis()->SetTitleOffset(1.8);
   hWp_PtWRecoCorrected_zoomin ->SetFillStyle(3448);
   hWp_PtWRecoCorrected_zoomin ->SetLineColor(kGreen);
   hWp_PtWRecoCorrected_zoomin ->SetFillColor(kGreen);
   hWp_PtWReco_zoomin          ->Draw();
   //hWp_PtWGen_zoomin           ->SetFillStyle(3490);
   hWp_PtWGen_zoomin           ->SetFillStyle(0);
   //hWp_PtWGen_zoomin           ->SetFillColor(kRed);
   hWp_PtWGen_zoomin           ->SetLineColor(kRed);
   hWp_PtWRecoCorrected_zoomin ->Draw("same");
   hWp_PtWGen_zoomin           ->Draw("same");
   TLegend *leg2 = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg2 -> AddEntry(hWp_PtWReco_zoomin,"Before P_{T} correction", "F");
   leg2 -> AddEntry(hWp_PtWRecoCorrected_zoomin,"After P_{T} correction", "F" );
   leg2 -> AddEntry(hWp_PtWGen_zoomin,"PHYTHIA Generated", "L");
   leg2 -> Draw();


   c2b->SaveAs(outPath + "/plot_PtCorr2.png");
   c2b->SaveAs(outPath + "/plot_PtCorr2.eps");

  
   TCanvas *c3 = new TCanvas("c3", "", 1000, 400);

   c3-> SetTitle("Reco vs Gen");

   c3->Divide(3, 1);

   c3_1->cd();
   c3_1->SetLogz(1);
   TLine *line1 = new TLine(0, 0, 50, 50);
   line1->SetLineColor(kRed);
   line1->SetLineWidth(2);
   line1->SetLineStyle(2);
   hWp_PtRecoil_vs_PtWGen->GetYaxis()->SetTitleOffset(1.1);
   hWp_PtRecoil_vs_PtWGen-> SetStats(0);
   hWp_PtRecoil_vs_PtWGen->Draw();
   line1->Draw();

   c3_2->cd();
   c3_2->SetLogz(1);
   hWp_PtRecoilCorr_vs_PtWGen->GetYaxis()->SetTitleOffset(1.1);
   hWp_PtRecoilCorr_vs_PtWGen-> SetStats(0);
   hWp_PtRecoilCorr_vs_PtWGen->Draw();
   line1->Draw();

   TH1D* hWp_PtRecoilCorr_vs_PtWGen_py;
   TString name("hWp_PtRecoilCorr_vs_PtWGen_py");
   hWp_PtRecoilCorr_vs_PtWGen_py = hWp_PtRecoilCorr_vs_PtWGen -> ProjectionY(name, 1, 1);
   TH1D* hPtRecoilBinCenter      = (TH1D*) hWp_PtRecoilCorr_vs_PtWGen_py->Clone("hPtRecoilCorrMax");
   TH1D* hPtRecoilBinMean        = (TH1D*) hWp_PtRecoilCorr_vs_PtWGen_py->Clone("hPtRecoilCorrMax");

   Int_t nybins = hWp_PtRecoilCorr_vs_PtWGen->GetNbinsX();
   for (Int_t i = 1; i <= nybins; i++) {
	hWp_PtRecoilCorr_vs_PtWGen_py = hWp_PtRecoilCorr_vs_PtWGen -> ProjectionY(name, i, i);

        cout << "binNumber = " << i << endl;

        Double_t binCenter = hWp_PtRecoilCorr_vs_PtWGen_py -> GetBinCenter(hWp_PtRecoilCorr_vs_PtWGen_py -> GetMaximumBin());
        cout << "binCenter = " << binCenter << endl;

        Double_t binMean   = hWp_PtRecoilCorr_vs_PtWGen_py -> GetMean();
        cout << "binMean = " << binMean << endl;

        hPtRecoilBinCenter -> SetBinContent(i,binCenter);
        hPtRecoilBinMean   -> SetBinContent(i,binMean);
   } 

   hPtRecoilBinCenter -> SetMarkerStyle(20);
   hPtRecoilBinCenter -> SetMarkerColor(kMagenta);
   hPtRecoilBinCenter -> Draw("P same");
   hPtRecoilBinMean   -> SetMarkerStyle(20);
   hPtRecoilBinMean   -> SetMarkerColor(kGreen);
   hPtRecoilBinMean   -> Draw("P same");

   c3_3->cd();
   c3_3->SetLogz(1);
   hWp_PhiRecoil_vs_PhiWGen-> SetStats(0);
   hWp_PhiRecoil_vs_PhiWGen-> Draw();
   
   c3->Print(outPath + "/plot_c3.png");
   c3->Print(outPath + "/plot_c3.eps");


   TCanvas *c4   = new TCanvas("c4", "", 400, 400);

   TH1F * hd_Wp_PtWRecoCorrected_zoomin_1 = (TH1F *) hd_Wp_PtWRecoCorrected_zoomin->Clone(" hd_Wp_PtWRecoCorrected_zoomin_1");

   TH1F * hWp_Wp_PtWRecoCorrected_zoomin_1 = (TH1F *) hWp_Wp_PtWRecoCorrected_zoomin->Clone(" hWp_Wp_PtWRecoCorrected_zoomin_1");

   // Normalize to the integral:
   //hWp_Wp_PtWRecoCorrected_zoomin_1->Scale(hd_Wp_PtWRecoCorrected_zoomin_1->Integral()/hWp_Wp_PtWRecoCorrected_zoomin_1->Integral());

   // Normalize to the luminosity
   hWp_Wp_PtWRecoCorrected_zoomin_1->Scale(scaleWp);

   c4->cd();
   hd_Wp_PtWRecoCorrected_zoomin_1    ->Rebin(2);
   hd_Wp_PtWRecoCorrected_zoomin_1    ->GetYaxis()->SetTitleOffset(1.8);

   hWp_Wp_PtWRecoCorrected_zoomin_1   ->Rebin(2);
   hWp_Wp_PtWRecoCorrected_zoomin_1   ->Draw();
   hd_Wp_PtWRecoCorrected_zoomin_1    ->SetMarkerStyle(20);
   hd_Wp_PtWRecoCorrected_zoomin_1    ->SetTitle("W+ sample");
   hd_Wp_PtWRecoCorrected_zoomin_1    ->Draw("E0");
   hWp_Wp_PtWRecoCorrected_zoomin_1   ->Draw("same");
   hd_Wp_PtWRecoCorrected_zoomin_1    ->Draw("E0 same");
  
   TLegend *leg4 = new TLegend(0.5, 0.75, 0.75, 0.9);
   leg4 -> AddEntry(hd_Wp_PtWRecoCorrected_zoomin_1,"STAR run11 tran.", "P");
   leg4 -> AddEntry(hWp_Wp_PtWRecoCorrected_zoomin_1,"PHYTHIA", "F");
   leg4 -> Draw();


   c4->SaveAs(outPath + "/plot_DataMc_WpPt.png");
   c4->SaveAs(outPath + "/plot_DataMc_WpPt.eps");
  

}
