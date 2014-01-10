#include <Riostream.h>
#include <iostream.h>
#include <fstream>
#include <sstream>

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

   TString inPath        = "/star/institutions/bnl_me/fazio/vbana_out/";
   TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString inPathScratch = "/star/data05/scratch/fazio/stana_out/runlists/";
   //TString inPathNew     = $VBASYM_RESULTS_DIR;
   TString outPath       = "~/vbasym_results/plots/";

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

   // open histogram files
   //TFile *fileData      = TFile::Open(inPath + "run11_pp_transverse_-w_--jpm_0.5_--run_11_vbana.root");
   //TFile *fileData      = TFile::Open(inPathNew + "run11_pp_transverse_-w_--jpm_0.5_--tpm_0.2_--run_11/hist/vbana.root");
   TFile *fileData      = TFile::Open(inPathScratch + "run11_pp_transverse/hist/vbana.root");
   //TFile *fileMCWp      = TFile::Open(inPath + "run11_mc_Wp2enu.lis_-m_-w_--jpm_0.5_vbana.root");
   ////TFile *fileMCWp      = TFile::Open(inPathNew + "run11_mc_Wp2enu.lis_-m_-w_--jpm_0.5_--tpm_0.2/hist/vbana_tpm02.root");
   TFile *fileMCWp      = TFile::Open(inPathScratch + "run11_mc_Wp2enu.lis/hist/vbana.root");
   TFile *fileMCWpOld   = TFile::Open(inPath + "run11_mc_Wp2enu.lis_-m_-w_--jpm_0.5_vbana_OldStylePtCorr.root");
   TFile *fileMCWm      = TFile::Open(inPathScratch + "run11_mc_Wm2enu.lis/hist/vbana.root");

//*** CREATE ROOT FILE FOR CURVES FROM ASCII FILE ***
   // open text files
   //TFile *fileCurvesWp_evol  = TFile::Open(inPathCurves + "ZK_evolution_Wp_asymmetry.txt");

   //Convert from a Unix pathname to a local pathname. E.g. from /user/root to \user\root
   TString dirCurves = gSystem->UnixPathName(gInterpreter->GetCurrentMacroName());
   dirCurves.ReplaceAll("PtCorrPlot.C","");
   dirCurves.ReplaceAll("/./","/");
   TString inPathCurves  = dirCurves + "curves/";
   cout << " dirCurves = " << dirCurves.Data() << endl; 
   cout << " inPathCurves = " << inPathCurves << endl; 

   ifstream inCurves;
   inCurves.open(Form("%sZK_evolution_Wp_asymmetry.txt",inPathCurves.Data()));
   //inCurves.open(Form("%sZK_noevo_Wp_asymmetry.txt",inPathCurves.Data()));
   //inCurves.open(Form("%sZK_evolution_Wm_asymmetry.txt",inPathCurves.Data()));
   //inCurves.open(Form("%sZK_noevo_Wm_asymmetry.txt",inPathCurves.Data()));

   //FILE *fp = fopen("/direct/star+u/fazio/vbasym/macros/curves/ZK_evolution_Wp_asymmetry.txt","r");
   //Int_t ncols;
   Float_t x,y,z;
   Float_t y,pt,an;
   Int_t nlines = 0;
   TFile   *fAnEvol  = new TFile("./ZK_evolution_Wp_asymmetry.root","RECREATE");
   TH3F    *hAnEvol  = new TH3F("hAnEvol","A_N prediction with evolution",100,-4,4,100,0,25,100,0,0.2);
   TNtuple *ntAnEvol = new TNtuple("ntAnEvol","A_N prediction with evolution","y:pt:an");

     while (1) {
       //ncols = fscanf(fp,"%f %f %f",&x, &y, &z);
       //printf("x=%8f, y=%8f, z=%8.15f\n",x,y,z);
      inCurves >> y >> pt >> an;
      //if (nlines < 4) printf("y=%8f, pt=%8f, an=%8.15f\n",y,pt,an);
      if (!inCurves.good()) break;
      if (nlines < 4) printf("y=%8f, pt=%8f, an=%8.15f\n",y,pt,an);
      hAnEvol  ->Fill(y,pt,an);
      ntAnEvol ->Fill(y,pt,an);
      nlines++;
   }
   printf(" found %d points\n",nlines);

   inCurves.close();

   fAnEvol ->Write();

   TCanvas *c0   = new TCanvas("c0", "", 500, 500);
   c0-> cd();
   ntAnEvol->Draw("an:pt:y");

   // Test
   ntAnEvol->Draw("an:y:pt", "y>= 1.35 && y < 1.45 && pt == 5.000");
   ntAnEvol->Draw("an >> h1", "y>= 1.35 && y < 1.45 && pt == 5.000");
   Double_t mean=h1->GetMean();
     cout << "mean = " << mean << endl;
     delete h1;
//***************************************************

   // fill histograms

   TH1 *hd_Wp_PtReco                       = (TH1*) fileData->Get("event_wp/hTrackRecoilWithNeutralsPt");
   TH1 *hd_Wp_PtReco_zoomin                = (TH1*) fileData->Get("event_wp/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hd_Wp_PtRecoCorrected              = (TH1*) fileData->Get("event_wp/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hd_Wp_PtRecoCorrected_zoomin       = (TH1*) fileData->Get("event_wp/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hd_Wp_WBosonPt                     = (TH1*) fileData->Get("event_wp/hWBosonPt");
   TH1 *hd_Wp_WBosonPt_zoomin              = (TH1*) fileData->Get("event_wp/hWBosonPt_zoomin");
   TH1 *hWp_PtReco                         = (TH1*) fileMCWp->Get("event_w/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_PtReco_zoomin                  = (TH1*) fileMCWp->Get("event_w/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWp_PtRecoCorrected                = (TH1*) fileMCWp->Get("event_w/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWp_PtRecoCorrected_zoomin         = (TH1*) fileMCWp->Get("event_w/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_Wp_PtReco                      = (TH1*) fileMCWp->Get("event_wp/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_Wp_PtReco_zoomin               = (TH1*) fileMCWp->Get("event_wp/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWp_Wp_PtRecoCorrected             = (TH1*) fileMCWp->Get("event_wp/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWp_Wp_PtRecoCorrected_zoomin      = (TH1*) fileMCWp->Get("event_wp/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_Wp_WBosonPt                    = (TH1*) fileMCWp->Get("event_wp/hWBosonPt");
   TH1 *hWp_Wp_WBosonPt_zoomin             = (TH1*) fileMCWp->Get("event_wp/hWBosonPt_zoomin");
   TH1 *hWp_PtWGen                         = (TH1*) fileMCWp->Get("event_mc/hWBosonPt");
   TH1 *hWp_PtWGen_zoomin                  = (TH1*) fileMCWp->Get("event_mc/hWBosonPt_zoomin");

   TH2 *hWp_PtRecoil_vs_PtWGen             = (TH2 *)fileMCWp->Get("event_mc/hTrackRecoilTpcNeutralsPtVsWBosonPt_copy");
   TH2 *hWp_PtRecoilCorr_vs_PtWGen         = (TH2 *)fileMCWp->Get("event_mc/hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy");
   TH1 *hWp_PhiRecoil_vs_PhiWGen           = (TH1 *)fileMCWp->Get("event_mc/hTrackRecoilPhiVsWBosonPhi");
   TH2 *hWp_An_evol_ZK_Vs_PtGen            = (TH2 *)fileMCWp->Get("event_mc/hAn_evol_ZK_Vs_PtGen");
   TH2 *hWp_An_evol_ZK_Vs_PtRec            = (TH2 *)fileMCWp->Get("event_mc/hAn_evol_ZK_Vs_PtRec");
   TH2 *hWp_An_noevo_ZK_Vs_PtGen           = (TH2 *)fileMCWp->Get("event_mc/hAn_noevo_ZK_Vs_PtGen");
   TH2 *hWp_An_noevo_ZK_Vs_PtRec           = (TH2 *)fileMCWp->Get("event_mc/hAn_noevo_ZK_Vs_PtRec");

   TH2 *hWm_PtRecoil_vs_PtWGen             = (TH2 *)fileMCWm->Get("event_mc/hTrackRecoilTpcNeutralsPtVsWBosonPt_copy");
   TH2 *hWm_PtRecoilCorr_vs_PtWGen         = (TH2 *)fileMCWm->Get("event_mc/hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy");
   TH1 *hWm_PhiRecoil_vs_PhiWGen           = (TH1 *)fileMCWm->Get("event_mc/hTrackRecoilPhiVsWBosonPhi");
   TH2 *hWm_An_evol_ZK_Vs_PtGen            = (TH2 *)fileMCWm->Get("event_mc/hAn_evol_ZK_Vs_PtGen");
   TH2 *hWm_An_evol_ZK_Vs_PtRec            = (TH2 *)fileMCWm->Get("event_mc/hAn_evol_ZK_Vs_PtRec");
   TH2 *hWm_An_noevo_ZK_Vs_PtGen           = (TH2 *)fileMCWm->Get("event_mc/hAn_noevo_ZK_Vs_PtGen");
   TH2 *hWm_An_noevo_ZK_Vs_PtRec           = (TH2 *)fileMCWm->Get("event_mc/hAn_noevo_ZK_Vs_PtRec");
   TH2 *hWm_An_noevo_ZK_Vs_PtGen_zoomin    = (TH2 *)fileMCWm->Get("event_mc/hAn_noevo_ZK_Vs_PtGen_zoomin");
   TH2 *hWm_An_noevo_ZK_Vs_PtRec_zoomin    = (TH2 *)fileMCWm->Get("event_mc/hAn_noevo_ZK_Vs_PtRec_zoomin");

   hWp_PtReco_zoomin                ->SetFillColor(kYellow);
   hWp_Wp_PtReco_zoomin             ->SetFillColor(kYellow);
   hWp_Wp_PtRecoCorrected_zoomin    ->SetFillColor(kYellow);
   hWp_Wp_WBosonPt_zoomin           ->SetFillColor(kYellow);
   hWp_PtWGen_zoomin                ->SetFillColor(kYellow);

/*
   TH1 *hd_Wp_PtReco                       = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt");
   TH1 *hd_Wp_PtReco_zoomin                = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hd_Wp_PtRecoCorrected              = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hd_Wp_PtRecoCorrected_zoomin       = (TH1*) fileData->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_PtReco                         = (TH1*) fileMCWp->Get("event_pass_wbos/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_PtReco_zoomin                  = (TH1*) fileMCWp->Get("event_pass_wbos/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWp_PtRecoCorrected                = (TH1*) fileMCWp->Get("event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWp_PtRecoCorrected_zoomin         = (TH1*) fileMCWp->Get("event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_Wp_PtRecoCorrected                      = (TH1*) fileMCWp->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWp_Wp_PtRecoCorrected_zoomin               = (TH1*) fileMCWp->Get("W+_event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   //TH1 *hWp_PtRecoCorrectedOldStyle        = (TH1*) fileMCWpOld->Get("event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected");
   //TH1 *hWp_PtRecoCorrectedOldStyle_zoomin = (TH1*) fileMCWpOld->Get("event_pass_wbos/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_PtWGen                          = (TH1*) fileMCWp->Get("event_mc_pass_wbos/hWBosonPt");
   TH1 *hWp_PtWGen_zoomin                   = (TH1*) fileMCWp->Get("event_mc_pass_wbos/hWBosonPt_zoomin");

   TH2 *hWp_PtRecoil_vs_PtWGen              = (TH2 *)fileMCWp->Get("event_mc_pass_wbos/hTrackRecoilTpcNeutralsPtVsWBosonPt_copy");
   TH2 *hWp_PtRecoilCorr_vs_PtWGen          = (TH2 *)fileMCWp->Get("event_mc_pass_wbos/hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy");

   TH1 *hWp_PhiRecoil_vs_PhiWGen            = (TH1 *)fileMCWp->Get("event_mc_pass_wbos/hTrackRecoilPhiVsWBosonPhi");
*/

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
   Double_t int_hWp_PtRecoCorrected_zoomin    =  hWp_PtRecoCorrected_zoomin  -> Integral(); 
   float scale_g1   = int_hWp_PtRecoCorrected_zoomin / int_g1;
   scaleGraph(g1, 0.5*scale_g1);

   c2->cd();
   hWp_PtReco_zoomin ->GetYaxis()->SetTitleOffset(1.8);
   hWp_PtRecoCorrected_zoomin ->SetFillStyle(3448);
   hWp_PtRecoCorrected_zoomin ->SetFillColor(kGreen);
   //hWp_PtRecoCorrected_zoomin ->Draw();
   hWp_PtReco_zoomin          ->Draw();
   hWp_PtRecoCorrected_zoomin ->Draw("same");
   g1                            -> SetLineColor(kBlue); 
   g1                            -> SetLineWidth(3); 
   g1                            -> Draw("same");
   TLegend *leg1 = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg1 -> AddEntry(hWp_PtReco_zoomin,"Before P_{T} correction", "F");
   leg1 -> AddEntry(hWp_PtRecoCorrected_zoomin,"After P_{T} correction", "F" );
   leg1 -> AddEntry(g1,"RhicBOS 500 GeV", "l");
   leg1 -> Draw();


   c2->SaveAs(outPath + "/plot_PtCorr.png");
   c2->SaveAs(outPath + "/plot_PtCorr.eps");


   TCanvas *c2b   = new TCanvas("c2b", "", 400, 400);

   c2b->cd();
   hWp_PtReco_zoomin ->GetYaxis()->SetTitleOffset(1.8);
   hWp_PtRecoCorrected_zoomin ->SetFillStyle(3448);
   hWp_PtRecoCorrected_zoomin ->SetLineColor(kGreen);
   hWp_PtRecoCorrected_zoomin ->SetFillColor(kGreen);
   hWp_PtReco_zoomin          ->Draw();
   //hWp_PtWGen_zoomin           ->SetFillStyle(3490);
   hWp_PtWGen_zoomin           ->SetFillStyle(0);
   //hWp_PtWGen_zoomin           ->SetFillColor(kRed);
   hWp_PtWGen_zoomin           ->SetLineColor(kRed);
   hWp_PtRecoCorrected_zoomin ->Draw("same");
   hWp_PtWGen_zoomin           ->Draw("same");
   TLegend *leg2 = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg2 -> AddEntry(hWp_PtReco_zoomin,"Before P_{T} correction", "F");
   leg2 -> AddEntry(hWp_PtRecoCorrected_zoomin,"After P_{T} correction", "F" );
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

   TH1F * hd_Wp_WBosonPt_zoomin_1 = (TH1F *) hd_Wp_WBosonPt_zoomin->Clone(" hd_Wp_WBosonPt_zoomin_1");

   TH1F * hWp_Wp_WBosonPt_zoomin_1 = (TH1F *) hWp_Wp_WBosonPt_zoomin->Clone(" hWp_Wp_WBosonPt_zoomin_1");

   // Normalize to the integral:
   hWp_Wp_WBosonPt_zoomin_1->Scale(hd_Wp_WBosonPt_zoomin_1->Integral()/hWp_Wp_WBosonPt_zoomin_1->Integral());

   // Normalize to the luminosity
   //hWp_Wp_WBosonPt_zoomin_1->Scale(scaleWp);

   c4->cd();
   hd_Wp_WBosonPt_zoomin_1    ->Rebin(2);
   hd_Wp_WBosonPt_zoomin_1    ->GetYaxis()->SetTitleOffset(1.8);

   hWp_Wp_WBosonPt_zoomin_1   ->Rebin(2);
   hWp_Wp_WBosonPt_zoomin_1   ->Draw();
   hd_Wp_WBosonPt_zoomin_1    ->SetMarkerStyle(20);
   hd_Wp_WBosonPt_zoomin_1    ->SetTitle("W+ sample");
   hd_Wp_WBosonPt_zoomin_1    ->Draw("E0");
   hWp_Wp_WBosonPt_zoomin_1   ->Draw("same");
   hd_Wp_WBosonPt_zoomin_1    ->Draw("E0 same");
  
   TLegend *leg4 = new TLegend(0.5, 0.75, 0.75, 0.9);
   leg4 -> AddEntry(hd_Wp_WBosonPt_zoomin_1,"STAR run11 tran.", "P");
   leg4 -> AddEntry(hWp_Wp_WBosonPt_zoomin_1,"PHYTHIA", "F");
   leg4 -> Draw();


   c4->SaveAs(outPath + "/plot_DataMc_WpPt.png");
   c4->SaveAs(outPath + "/plot_DataMc_WpPt.eps");;


   TCanvas *c4b   = new TCanvas("c4b", "", 400, 400);

   TH1F * hd_Wp_PtReco_zoomin_1  = (TH1F *) hd_Wp_PtReco_zoomin -> Clone("hd_Wp_PtReco_zoomin_1");

   TH1F * hWp_Wp_PtReco_zoomin_1 = (TH1F *) hWp_Wp_PtReco_zoomin-> Clone("hWp_Wp_PtReco_zoomin_1");

   // Normalize to the integral:
   hWp_Wp_PtReco_zoomin_1->Scale(hd_Wp_PtReco_zoomin_1->Integral()/hWp_Wp_PtReco_zoomin_1->Integral());

   // Normalize to the luminosity
   //hWp_Wp_PtReco_zoomin_1->Scale(scaleWp);

   c4b->cd();
   hd_Wp_PtReco_zoomin_1    ->Rebin(2);
   hd_Wp_PtReco_zoomin_1    ->GetYaxis()->SetTitleOffset(1.8);

   hWp_Wp_PtReco_zoomin_1   ->Rebin(2);
   hWp_Wp_PtReco_zoomin_1   ->Draw();
   hd_Wp_PtReco_zoomin_1    ->SetMarkerStyle(20);
   hd_Wp_PtReco_zoomin_1    ->SetTitle("W+ sample - Without W-Pt correction");
   hd_Wp_PtReco_zoomin_1    ->Draw("E0");
   hWp_Wp_PtReco_zoomin_1   ->Draw("same");
   hd_Wp_PtReco_zoomin_1    ->Draw("E0 same");
  
   TLegend *leg4b = new TLegend(0.5, 0.75, 0.75, 0.9);
   leg4b -> AddEntry(hd_Wp_PtReco_zoomin_1,"STAR run11 tran.", "P");
   leg4b -> AddEntry(hWp_Wp_PtReco_zoomin_1,"PHYTHIA", "F");
   leg4 -> Draw();


   c4b->SaveAs(outPath + "/plot_DataMc_WpPt_nocorr.png");
   c4b->SaveAs(outPath + "/plot_DataMc_WpPt_nocorr.eps");
  


   TCanvas *c5   = new TCanvas("c5", "An predicction with evol.", 400, 400);

   hWp_An_evol_ZK_Vs_PtGen ->ProfileX("hWp_An_evol_ZK_Vs_PtGen_pfx"); 
   hWp_An_evol_ZK_Vs_PtRec ->ProfileX("hWp_An_evol_ZK_Vs_PtRec_pfx"); 


   c5->Divide(1, 2);

   c5_1->cd();
   hWp_An_evol_ZK_Vs_PtGen->Draw();
   hWp_An_evol_ZK_Vs_PtGen->GetYaxis()->SetTitleOffset(2.);
   hWp_An_evol_ZK_Vs_PtGen->SetTitle("Prediction inlcudes evolution; W^{+} P_{T}^{GEN}; A_{N}");
   //hWp_An_evol_ZK_Vs_PtGen_pfx->Draw();
   //hWp_An_evol_ZK_Vs_PtGen_pfx->GetYaxis()->SetTitleOffset(2.);
   //hWp_An_evol_ZK_Vs_PtGen_pfx->SetTitle("Prediction inlcudes evolution; W^{+} P_{T}^{GEN}; A_{N}");
   c5_2->cd();
   hWp_An_evol_ZK_Vs_PtGen_pfx->Draw();
   hWp_An_evol_ZK_Vs_PtGen_pfx->GetYaxis()->SetTitleOffset(2.);
   hWp_An_evol_ZK_Vs_PtGen_pfx->SetTitle("Prediction inlcudes evolution; W^{+} P_{T}^{GEN}; A_{N}");
   //hWp_An_evol_ZK_Vs_PtRec_pfx->Draw();
   //hWp_An_evol_ZK_Vs_PtRec_pfx->GetYaxis()->SetTitleOffset(2.);
   //hWp_An_evol_ZK_Vs_PtRec_pfx->SetTitle("Prediction inlcudes evolution; W^{+} P_{T}^{REC}; A_{N}");

   c5->SaveAs(outPath + "/plot_Wp_An_evol_ZK.png");
   c5->SaveAs(outPath + "/plot_Wp_An_evol_ZK.eps");

   TCanvas *c5bis   = new TCanvas("c5bis", "An predicction with evol.", 400, 400);

   c5bis->Divide(1, 2);

   c5bis_1->cd();
   hWp_An_evol_ZK_Vs_PtRec->Draw();
   hWp_An_evol_ZK_Vs_PtRec->GetYaxis()->SetTitleOffset(2.);
   hWp_An_evol_ZK_Vs_PtRec->SetTitle("Prediction inlcudes evolution; W^{+} P_{T}^{REC}; A_{N}");
   c5bis_2->cd();
   hWp_An_evol_ZK_Vs_PtRec_pfx->Draw();
   hWp_An_evol_ZK_Vs_PtRec_pfx->GetYaxis()->SetTitleOffset(2.);
   hWp_An_evol_ZK_Vs_PtRec_pfx->SetTitle("Prediction inlcudes evolution; W^{+} P_{T}^{REC}; A_{N}");

   c5bis->SaveAs(outPath + "/plot_Wp_An_evol_ZK_Ptrec.png");
   c5bis->SaveAs(outPath + "/plot_Wp_An_evol_ZK_Ptrec.eps");


   TCanvas *c5c   = new TCanvas("c5c", "An predicction no evol.", 400, 400);

   hWp_An_noevo_ZK_Vs_PtGen ->ProfileX("hWp_An_noevo_ZK_Vs_PtGen_pfx"); 
   hWp_An_noevo_ZK_Vs_PtRec ->ProfileX("hWp_An_noevo_ZK_Vs_PtRec_pfx"); 


   c5c->Divide(1, 2);

   c5c_1->cd();
   hWp_An_noevo_ZK_Vs_PtGen->Draw();
   hWp_An_noevo_ZK_Vs_PtGen->GetYaxis()->SetTitleOffset(2.);
   hWp_An_noevo_ZK_Vs_PtGen->SetTitle("Prediction no evolution; W^{+} P_{T}^{GEN}; A_{N}");
   c5c_2->cd();
   hWp_An_noevo_ZK_Vs_PtGen_pfx->Draw();
   hWp_An_noevo_ZK_Vs_PtGen_pfx->GetYaxis()->SetTitleOffset(2.);
   hWp_An_noevo_ZK_Vs_PtGen_pfx->SetTitle("Prediction no evolution; W^{+} P_{T}^{GEN}; A_{N}");

   c5c->SaveAs(outPath + "/plot_Wp_An_noevo_ZK.png");
   c5c->SaveAs(outPath + "/plot_Wp_An_noevo_ZK.eps");

   TCanvas *c5d   = new TCanvas("c5d", "An predicction no evol.", 400, 400);

   c5d->Divide(1, 2);

   c5d_1->cd();
   hWp_An_noevo_ZK_Vs_PtRec->Draw();
   hWp_An_noevo_ZK_Vs_PtRec->GetYaxis()->SetTitleOffset(2.);
   hWp_An_noevo_ZK_Vs_PtRec->SetTitle("Prediction no evolution; W^{+} P_{T}^{REC}; A_{N}");
   c5d_2->cd();
   hWp_An_noevo_ZK_Vs_PtRec_pfx->Draw();
   hWp_An_noevo_ZK_Vs_PtRec_pfx->GetYaxis()->SetTitleOffset(2.);
   hWp_An_noevo_ZK_Vs_PtRec_pfx->SetTitle("Prediction inlcudes evolution; W^{+} P_{T}^{REC}; A_{N}");

   c5d->SaveAs(outPath + "/plot_Wp_An_noevo_ZK_Ptrec.png");
   c5d->SaveAs(outPath + "/plot_Wp_An_noevo_ZK_Ptrec.eps");


   TCanvas *c6   = new TCanvas("c6", "An prediction with evol.", 400, 400);

   hWm_An_evol_ZK_Vs_PtGen ->ProfileX("hWm_An_evol_ZK_Vs_PtGen_pfx"); 
   hWm_An_evol_ZK_Vs_PtRec ->ProfileX("hWm_An_evol_ZK_Vs_PtRec_pfx"); 


   c6->Divide(1, 2);

   c6_1->cd();
   hWm_An_evol_ZK_Vs_PtGen->Draw();
   hWm_An_evol_ZK_Vs_PtGen->GetYaxis()->SetTitleOffset(2.);
   hWm_An_evol_ZK_Vs_PtGen->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{GEN}; A_{N}");
   //hWm_An_evol_ZK_Vs_PtGen_pfx->Draw();
   //hWm_An_evol_ZK_Vs_PtGen_pfx->GetYaxis()->SetTitleOffset(2.);
   //hWm_An_evol_ZK_Vs_PtGen_pfx->SetTitle("Prediction inlcudes evolution; W P_{T}^{GEN}; A_{N}");
   c6_2->cd();
   hWm_An_evol_ZK_Vs_PtGen_pfx->Draw();
   hWm_An_evol_ZK_Vs_PtGen_pfx->GetYaxis()->SetTitleOffset(2.);
   hWm_An_evol_ZK_Vs_PtGen_pfx->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{GEN}; A_{N}");
   //hWm_An_evol_ZK_Vs_PtRec_pfx->Draw();
   //hWm_An_evol_ZK_Vs_PtRec_pfx->GetYaxis()->SetTitleOffset(2.);
   //hWm_An_evol_ZK_Vs_PtRec_pfx->SetTitle("Prediction inlcudes evolution; W P_{T}^{REC}; A_{N}");

   c6->SaveAs(outPath + "/plot_Wm_An_evol_ZK.png");
   c6->SaveAs(outPath + "/plot_Wm_An_evol_ZK.eps");

   TCanvas *c6bis   = new TCanvas("c6bis", "An prediction with evol.", 400, 400);

   c6bis->Divide(1, 2);

   c6bis_1->cd();
   hWm_An_evol_ZK_Vs_PtRec->Draw();
   hWm_An_evol_ZK_Vs_PtRec->GetYaxis()->SetTitleOffset(2.);
   hWm_An_evol_ZK_Vs_PtRec->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{REC}; A_{N}");
   c6bis_2->cd();
   hWm_An_evol_ZK_Vs_PtRec_pfx->Draw();
   hWm_An_evol_ZK_Vs_PtRec_pfx->GetYaxis()->SetTitleOffset(2.);
   hWm_An_evol_ZK_Vs_PtRec_pfx->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{REC}; A_{N}");

   c6bis->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Ptrec.png");
   c6bis->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Ptrec.eps");


   TCanvas *c6c   = new TCanvas("c6c", "An predicction no evol.", 500, 500);

   hWm_An_noevo_ZK_Vs_PtGen ->ProfileX("hWm_An_noevo_ZK_Vs_PtGen_pfx"); 
   hWm_An_noevo_ZK_Vs_PtRec ->ProfileX("hWm_An_noevo_ZK_Vs_PtRec_pfx");
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy1", 1, 1); 
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy2", 2, 2); 
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy3", 3, 3);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy4", 4, 4); 
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy1", 1, 1); 
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy2", 2, 2); 
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy3", 3, 3);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy4", 4, 4); 
   hWm_An_noevo_ZK_Vs_PtGen_zoomin ->ProfileX("hWm_An_noevo_ZK_Vs_PtGen_zoomin_pfx"); 
   hWm_An_noevo_ZK_Vs_PtRec_zoomin ->ProfileX("hWm_An_noevo_ZK_Vs_PtRec_zoomin_pfx"); 


   //c6c->Divide(1, 2);

   c6c->cd();
   hWm_An_noevo_ZK_Vs_PtGen->Draw();
   hWm_An_noevo_ZK_Vs_PtGen->GetYaxis()->SetTitleOffset(1.8);
   hWm_An_noevo_ZK_Vs_PtGen->SetTitle("Prediction no evolution; W^{-} P_{T}^{GEN}; A_{N}");
   ////c6c_2->cd();
   ////hWm_An_noevo_ZK_Vs_PtGen_pfx->Draw();
   ////hWm_An_noevo_ZK_Vs_PtGen_pfx->GetYaxis()->SetTitleOffset(1.8);
   ////hWm_An_noevo_ZK_Vs_PtGen_pfx->SetTitle("Prediction no evolution; W^{-} P_{T}^{GEN}; A_{N}");
   //c6c_3->cd();
   //hWm_An_noevo_ZK_Vs_PtGen_zoomin->Draw();
   //hWm_An_noevo_ZK_Vs_PtGen_zoomin->GetYaxis()->SetTitleOffset(2.);
   //hWm_An_noevo_ZK_Vs_PtGen_zoomin->SetTitle("Prediction no evolution; W^{-} P_{T}^{GEN}; A_{N}");
   //c6c_4->cd();
   //hWm_An_noevo_ZK_Vs_PtGen_zoomin_pfx->Draw();
   //hWm_An_noevo_ZK_Vs_PtGen_zoomin_pfx->GetYaxis()->SetTitleOffset(2.);
   //hWm_An_noevo_ZK_Vs_PtGen_zoomin_pfx->SetTitle("Prediction no evolution; W^{-} P_{T}^{GEN}; A_{N}");

   c6c->SaveAs(outPath + "/plot_Wm_An_noevo_ZK.png");
   c6c->SaveAs(outPath + "/plot_Wm_An_noevo_ZK.eps");

   TCanvas *c6d   = new TCanvas("c6d", "An prediction no evol.", 500, 500);

   //c6d->Divide(1, 2);

   c6d->cd();
   hWm_An_noevo_ZK_Vs_PtRec->Draw();
   hWm_An_noevo_ZK_Vs_PtRec->GetYaxis()->SetTitleOffset(1.8);
   hWm_An_noevo_ZK_Vs_PtRec->SetTitle("Prediction no evolution; W^{-} P_{T}^{REC}; A_{N}");
   ////c6d_2->cd();
   ////hWm_An_noevo_ZK_Vs_PtRec_pfx->Draw();
   ////hWm_An_noevo_ZK_Vs_PtRec_pfx->GetYaxis()->SetTitleOffset(1.8);
   ////hWm_An_noevo_ZK_Vs_PtRec_pfx->SetTitle("Prediction no evolution; W^{-} P_{T}^{REC}; A_{N}");
   //c6d_3->cd();
   //hWm_An_noevo_ZK_Vs_PtRec_zoomin->Draw();
   //hWm_An_noevo_ZK_Vs_PtRec_zoomin->GetYaxis()->SetTitleOffset(2.);
   //hWm_An_noevo_ZK_Vs_PtRec_zoomin->SetTitle("Prediction no evolution; W^{-} P_{T}^{REC}; A_{N}");
   //c6d_4->cd();
   //hWm_An_noevo_ZK_Vs_PtRec_zoomin_pfx->Draw();
   //hWm_An_noevo_ZK_Vs_PtRec_zoomin_pfx->GetYaxis()->SetTitleOffset(2.);
   //hWm_An_noevo_ZK_Vs_PtRec_zoomin_pfx->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{REC}; A_{N}");

   c6d->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Ptrec.png");
   c6d->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Ptrec.eps");


   TCanvas *c7   = new TCanvas("c7", "An prediction no evol.", 1200, 800);

   c7-> Divide(2,2);
   c7_1->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> SetTitle("PtGen BIN 1: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> Draw();
   c7_2->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 ->SetTitle("PtGen BIN 2: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 -> Draw();
   c7_3->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 ->SetTitle("PtGen BIN 3: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 -> Draw();
   c7_4->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 ->SetTitle("PtGen BIN 4: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 -> Draw();


   c7->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Vs_PtGen_projs.png");
   c7->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Vs_PtGen_projs.eps");


   TCanvas *c7b   = new TCanvas("c7b", "An prediction no evol.", 1200, 800);

   c7b-> Divide(2,2);
   c7b_1->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> SetTitle("PtRec BIN 1: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> Draw();
   c7b_2->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 ->SetTitle("PtRec BIN 2: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> Draw();
   c7b_3->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 ->SetTitle("PtRec BIN 3: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> Draw();
   c7b_4->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 ->SetTitle("PtRec BIN 4: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> Draw();


   c7b->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Vs_PtRec_projs.png");
   c7b->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Vs_PtRec_projs.eps");
}
