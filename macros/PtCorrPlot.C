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
   TString outPath       = "~/vbasym_results/plots/4prelim";
   TString outPathPaper  = "~/vbasym_results/plots/paper";

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
   //TFile *fileData          = TFile::Open(inPath + "run11_pp_transverse_-w_--jpm_0.5_--run_11_vbana.root");
   //TFile *fileData          = TFile::Open(inPathNew + "run11_pp_transverse_-w_--jpm_0.5_--tpm_0.2_--run_11/hist/vbana.root");
   TFile *fileData          = TFile::Open(inPathNew + "run11_pp_transverse/hist/vbana.root");
   //TFile *fileMCWp          = TFile::Open(inPath + "run11_mc_Wp2enu.lis_-m_-w_--jpm_0.5_vbana.root");
   ////TFile *fileMCWp        = TFile::Open(inPathNew + "run11_mc_Wp2enu.lis_-m_-w_--jpm_0.5_--tpm_0.2/hist/vbana_tpm02.root");
   TFile *fileMCWp          = TFile::Open(inPathNew + "run11_mc_Wp2enu.lis/hist/vbana.root");
   TFile *fileMCWp_ZDChigh  = TFile::Open(inPathNew + "run11_mc_Wp2enu_ZDCrate_gt_90k.lis/hist/vbana.root");
   TFile *fileMCWp_TuneA    = TFile::Open(inPathNew + "run11_mc_Wp2enu_tuneA.lis/hist/vbana.root");
   TFile *fileMCWpOld       = TFile::Open(inPath + "run11_mc_Wp2enu.lis_-m_-w_--jpm_0.5_vbana_OldStylePtCorr.root");
   TFile *fileMCWm          = TFile::Open(inPathNew + "run11_mc_Wm2enu.lis/hist/vbana.root");

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
   TH1 *hd_Wp_WBosonPz                     = (TH1*) fileData->Get("event_wp/hWBosonPz");
   TH1 *hd_Wm_PtReco                       = (TH1*) fileData->Get("event_wm/hTrackRecoilWithNeutralsPt");
   TH1 *hd_Wm_PtReco_zoomin                = (TH1*) fileData->Get("event_wm/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hd_Wm_PtRecoCorrected              = (TH1*) fileData->Get("event_wm/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hd_Wm_PtRecoCorrected_zoomin       = (TH1*) fileData->Get("event_wm/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hd_Wm_WBosonPt                     = (TH1*) fileData->Get("event_wm/hWBosonPt");
   TH1 *hd_Wm_WBosonPt_zoomin              = (TH1*) fileData->Get("event_wm/hWBosonPt_zoomin");
   TH1 *hd_Wm_WBosonPz                     = (TH1*) fileData->Get("event_wm/hWBosonPz");

   TH1 *hWp_PtReco                         = (TH1*) fileMCWp->Get("event_w/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_PtReco_zoomin                  = (TH1*) fileMCWp->Get("event_w/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWp_PtRecoCorrected                = (TH1*) fileMCWp->Get("event_w/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWp_PtRecoCorrected_zoomin         = (TH1*) fileMCWp->Get("event_w/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWm_PtReco                         = (TH1*) fileMCWm->Get("event_w/hTrackRecoilWithNeutralsPt");
   TH1 *hWm_PtReco_zoomin                  = (TH1*) fileMCWm->Get("event_w/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWm_PtRecoCorrected                = (TH1*) fileMCWm->Get("event_w/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWm_PtRecoCorrected_zoomin         = (TH1*) fileMCWm->Get("event_w/hTrackRecoilWithNeutralsPtCorrected_zoomin");

   TH1 *hWp_Wp_PtReco                      = (TH1*) fileMCWp->Get("event_wp/hTrackRecoilWithNeutralsPt");
   TH1 *hWp_Wp_PtReco_zoomin               = (TH1*) fileMCWp->Get("event_wp/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWp_Wp_PtRecoCorrected             = (TH1*) fileMCWp->Get("event_wp/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWp_Wp_PtRecoCorrected_zoomin      = (TH1*) fileMCWp->Get("event_wp/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWp_Wp_WBosonPt                    = (TH1*) fileMCWp->Get("event_wp/hWBosonPt");
   TH1 *hWp_Wp_WBosonPt_zoomin             = (TH1*) fileMCWp->Get("event_wp/hWBosonPt_zoomin");
   TH1 *hWp_Wp_WBosonPz                    = (TH1*) fileMCWp->Get("event_wp/hWBosonPz");
   TH1 *hWp_Wp_WBosonPzVsRapidity          = (TH1*) fileMCWp->Get("event_wp/hWBosonPzVsRapidity");

   TH1 *hWm_Wm_PtReco                      = (TH1*) fileMCWm->Get("event_wm/hTrackRecoilWithNeutralsPt");
   TH1 *hWm_Wm_PtReco_zoomin               = (TH1*) fileMCWm->Get("event_wm/hTrackRecoilWithNeutralsPt_zoomin");
   TH1 *hWm_Wm_PtRecoCorrected             = (TH1*) fileMCWm->Get("event_wm/hTrackRecoilWithNeutralsPtCorrected");
   TH1 *hWm_Wm_PtRecoCorrected_zoomin      = (TH1*) fileMCWm->Get("event_wm/hTrackRecoilWithNeutralsPtCorrected_zoomin");
   TH1 *hWm_Wm_WBosonPt                    = (TH1*) fileMCWm->Get("event_wm/hWBosonPt");
   TH1 *hWm_Wm_WBosonPt_zoomin             = (TH1*) fileMCWm->Get("event_wm/hWBosonPt_zoomin");
   TH1 *hWm_Wm_WBosonPz                    = (TH1*) fileMCWm->Get("event_wm/hWBosonPz");
   TH1 *hWm_Wm_WBosonPzVsRapidity          = (TH1*) fileMCWm->Get("event_wm/hWBosonPzVsRapidity");

   TH1 *hWp_PtWGen                         = (TH1*) fileMCWp->Get("event_mc/hWBosonPt");
   TH1 *hWp_PtWGen_zoomin                  = (TH1*) fileMCWp->Get("event_mc/hWBosonPt_zoomin");
   TH1 *hWp_hRecoVsGenWBosonRap            = (TH1*) fileMCWp->Get("event_mc/hRecoVsGenWBosonRap");
   TH1 *hWm_PtWGen                         = (TH1*) fileMCWm->Get("event_mc/hWBosonPt");
   TH1 *hWm_PtWGen_zoomin                  = (TH1*) fileMCWm->Get("event_mc/hWBosonPt_zoomin");
   TH1 *hWm_hRecoVsGenWBosonRap            = (TH1*) fileMCWm->Get("event_mc/hRecoVsGenWBosonRap");

   TH2 *hWp_PtRecoil_vs_PtWGen             = (TH2 *)fileMCWp->Get("event_mc/hTrackRecoilTpcNeutralsPtVsWBosonPt_copy");
   TH2 *hWp_PtRecoilCorr_vs_PtWGen         = (TH2 *)fileMCWp->Get("event_mc/hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy");
   TH2 *hWp_PtRecoil_vs_Pt_GenOverReco     = (TH2 *)fileMCWp->Get("event_mc/hTrackRecoilTpcNeutralsPt_GenOverReco");
   TH1 *hWp_PhiRecoil_vs_PhiWGen           = (TH1 *)fileMCWp->Get("event_mc/hTrackRecoilPhiVsWBosonPhi");
   TH2 *hWp_An_evol_ZK_Vs_PtGen            = (TH2 *)fileMCWp->Get("event_mc/hAn_evol_ZK_Vs_PtGen");
   TH2 *hWp_An_evol_ZK_Vs_PtRec            = (TH2 *)fileMCWp->Get("event_mc/hAn_evol_ZK_Vs_PtRec");
   TH2 *hWp_An_noevo_ZK_Vs_PtGen           = (TH2 *)fileMCWp->Get("event_mc/hAn_noevo_ZK_Vs_PtGen");
   TH2 *hWp_An_noevo_ZK_Vs_PtRec           = (TH2 *)fileMCWp->Get("event_mc/hAn_noevo_ZK_Vs_PtRec");

   TH2 *hWm_PtRecoil_vs_Pt_GenOverReco     = (TH2 *)fileMCWm->Get("event_mc/hTrackRecoilTpcNeutralsPt_GenOverReco");
   TH2 *hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco   = (TH2 *)fileMCWp_ZDChigh->Get("event_mc/hTrackRecoilTpcNeutralsPt_GenOverReco");
   TH2 *hWp_TuneA_PtRecoil_vs_Pt_GenOverReco     = (TH2 *)fileMCWp_TuneA->Get("event_mc/hTrackRecoilTpcNeutralsPt_GenOverReco");

   TH2 *hWm_PtRecoil_vs_PtWGen             = (TH2 *)fileMCWm->Get("event_mc/hTrackRecoilTpcNeutralsPtVsWBosonPt_copy");
   TH2 *hWm_PtRecoilCorr_vs_PtWGen         = (TH2 *)fileMCWm->Get("event_mc/hTrackRecoilTpcNeutralsPtCorrectedVsWBosonPt_copy");
   TH1 *hWm_PhiRecoil_vs_PhiWGen           = (TH1 *)fileMCWm->Get("event_mc/hTrackRecoilPhiVsWBosonPhi");
   TH2 *hWm_An_evol_ZK_Vs_PtGen            = (TH2 *)fileMCWm->Get("event_mc/hAn_evol_ZK_Vs_PtGen");
   TH2 *hWm_An_evol_ZK_Vs_PtRec            = (TH2 *)fileMCWm->Get("event_mc/hAn_evol_ZK_Vs_PtRec");
   TH2 *hWm_An_noevo_ZK_Vs_PtGen           = (TH2 *)fileMCWm->Get("event_mc/hAn_noevo_ZK_Vs_PtGen");
   TH2 *hWm_An_noevo_ZK_Vs_PtRec           = (TH2 *)fileMCWm->Get("event_mc/hAn_noevo_ZK_Vs_PtRec");
   TH2 *hWm_An_noevo_ZK_Vs_PtGen_zoomin    = (TH2 *)fileMCWm->Get("event_mc/hAn_noevo_ZK_Vs_PtGen_zoomin");
   TH2 *hWm_An_noevo_ZK_Vs_PtRec_zoomin    = (TH2 *)fileMCWm->Get("event_mc/hAn_noevo_ZK_Vs_PtRec_zoomin");
   TH2 *hWm_An_evol_ZK_Vs_RapGen           = (TH2 *)fileMCWm->Get("event_mc/hAn_evol_ZK_Vs_RapGen");
   TH2 *hWm_An_evol_ZK_Vs_RapRec           = (TH2 *)fileMCWm->Get("event_mc/hAn_evol_ZK_Vs_RapRec");
   TH1 *hWm_ResolutionRap                  = (TH1 *)fileMCWm->Get("event_mc/hResolutionRap");
   TH1 *hWm_ResolutionRap_bin1             = (TH1 *)fileMCWm->Get("event_mc/hResolutionRap_bin1");
   TH1 *hWm_ResolutionRap_bin2             = (TH1 *)fileMCWm->Get("event_mc/hResolutionRap_bin2");
   TH1 *hWm_ResolutionRap_bin3             = (TH1 *)fileMCWm->Get("event_mc/hResolutionRap_bin3");
   TH1 *hWm_ResolutionRap_bin4             = (TH1 *)fileMCWm->Get("event_mc/hResolutionRap_bin4");


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

   TCanvas *c2   = new TCanvas("c2", "W+ RhicBOS 500 GeV", 400, 400);

   TGraph *g1       = new TGraph("./curves/0304002.Wptcurve.txt","%lg %lg");   
   Double_t int_g1  =  integrateGraph(g1); // the method Integral() not yet implemented on TGraph in ROOT4STAR  
   Double_t int_hWp_PtRecoCorrected_zoomin    =  hWp_PtRecoCorrected_zoomin  -> Integral(); 
   float scale_g1   = int_hWp_PtRecoCorrected_zoomin / int_g1;
   scaleGraph(g1, 0.5*scale_g1);

   c2->cd();
   hWp_PtReco_zoomin -> GetYaxis() -> SetTitleOffset(1.8);
   hWp_PtReco_zoomin -> GetXaxis() -> SetTitle("Recoil P_{T}");
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


   TCanvas *c2_Wm   = new TCanvas("c2_Wm", "W- RhicBOS 500 GeV", 400, 400);

   TGraph *g1_Wm       = new TGraph("./curves/0304002.Wptcurve.txt","%lg %lg");   
   Double_t int_g1_Wm  =  integrateGraph(g1_Wm); // the method Integral() not yet implemented on TGraph in ROOT4STAR  
   Double_t int_hWm_PtRecoCorrected_zoomin    =  hWm_PtRecoCorrected_zoomin  -> Integral(); 
   float scale_g1_Wm   = int_hWm_PtRecoCorrected_zoomin / int_g1_Wm;
   scaleGraph(g1_Wm, 0.5*scale_g1_Wm);

   c2_Wm->cd();
   hWm_PtReco_zoomin -> GetYaxis() -> SetTitleOffset(1.8);
   hWm_PtReco_zoomin -> GetXaxis() -> SetTitle("Recoil P_{T}");
   hWm_PtReco_zoomin -> GetYaxis() -> SetTitle("Events (W^{-})");
   hWm_PtRecoCorrected_zoomin ->SetFillStyle(3448);
   hWm_PtRecoCorrected_zoomin ->SetFillColor(kGreen);
   hWm_PtReco_zoomin          ->Draw();
   hWm_PtRecoCorrected_zoomin ->Draw("same");
   g1_Wm                           -> SetLineColor(kBlue); 
   g1_Wm                           -> SetLineWidth(3); 
   g1_Wm                           -> Draw("same");
   TLegend *leg1_Wm = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg1_Wm -> AddEntry(hWm_PtReco_zoomin,"Before P_{T} correction", "F");
   leg1_Wm -> AddEntry(hWm_PtRecoCorrected_zoomin,"After P_{T} correction", "F" );
   leg1_Wm -> AddEntry(g1_Wm,"RhicBOS 500 GeV", "l");
   leg1_Wm -> Draw();


   c2_Wm->SaveAs(outPath + "/plot_PtCorr_Wm.png");
   c2_Wm->SaveAs(outPath + "/plot_PtCorr_Wm.eps");


   TCanvas *c2b   = new TCanvas("c2b", "W+", 400, 400);

   c2b->cd();
   hWp_PtReco_zoomin ->GetYaxis()-> SetTitleOffset(1.8);
   hWp_PtReco_zoomin -> GetXaxis() -> SetTitle("Recoil P_{T}");
   hWp_PtRecoCorrected_zoomin -> SetFillStyle(3448);
   hWp_PtRecoCorrected_zoomin -> SetLineColor(kGreen);
   hWp_PtRecoCorrected_zoomin -> SetFillColor(kGreen);
   hWp_PtReco_zoomin          -> Draw();
   //hWp_PtWGen_zoomin          ->SetFillStyle(3490);
   hWp_PtWGen_zoomin          -> SetFillStyle(0);
   //hWp_PtWGen_zoomin          ->SetFillColor(kRed);
   hWp_PtWGen_zoomin          -> SetLineColor(kRed);
   hWp_PtWGen_zoomin          -> SetLineWidth(3); 
   hWp_PtRecoCorrected_zoomin -> Draw("same");
   hWp_PtWGen_zoomin          -> Draw("same");
   TLegend *leg2 = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg2 -> AddEntry(hWp_PtReco_zoomin,"Before P_{T} correction", "F");
   leg2 -> AddEntry(hWp_PtRecoCorrected_zoomin,"After P_{T} correction", "F" );
   leg2 -> AddEntry(hWp_PtWGen_zoomin,"PHYTHIA Generated", "L");
   leg2 -> Draw();


   c2b->SaveAs(outPath + "/plot_PtCorr2.png");
   c2b->SaveAs(outPath + "/plot_PtCorr2.eps");


   TCanvas *c2b_Wm   = new TCanvas("c2b_Wm", "W-", 400, 400);

   c2b_Wm->cd();
   hWm_PtReco_zoomin -> GetYaxis() -> SetTitleOffset(1.8);
   hWm_PtReco_zoomin -> GetXaxis() -> SetTitle("Recoil P_{T}");
   hWm_PtReco_zoomin -> GetYaxis() -> SetTitle("Events (W^{-})");
   hWm_PtRecoCorrected_zoomin -> SetFillStyle(3448);
   hWm_PtRecoCorrected_zoomin -> SetLineColor(kGreen);
   hWm_PtRecoCorrected_zoomin -> SetFillColor(kGreen);
   hWm_PtReco_zoomin          -> Draw();
   //hWm_PtWGen_zoomin           ->SetFillStyle(3490);
   hWm_PtWGen_zoomin          -> SetFillStyle(0);
   //hWm_PtWGen_zoomin          ->SetFillColor(kRed);
   hWm_PtWGen_zoomin          -> SetLineColor(kRed);
   hWm_PtWGen_zoomin          -> SetLineWidth(3); 
   hWm_PtRecoCorrected_zoomin -> Draw("same");
   hWm_PtWGen_zoomin          -> Draw("same");
   TLegend *leg2_Wm = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg2_Wm -> AddEntry(hWm_PtReco_zoomin,"Before P_{T} correction", "F");
   leg2_Wm -> AddEntry(hWm_PtRecoCorrected_zoomin,"After P_{T} correction", "F" );
   leg2_Wm -> AddEntry(hWm_PtWGen_zoomin,"PHYTHIA Generated", "L");
   leg2_Wm -> Draw();


   c2b_Wm->SaveAs(outPath + "/plot_PtCorr2_Wm.png");
   c2b_Wm->SaveAs(outPath + "/plot_PtCorr2_Wm.eps");




   TCanvas *c2b_paper   = new TCanvas("c2b_paper", "W+ Paper Fig", 400, 400);

   c2b_paper-> cd();
   c2b_paper-> SetGrid(0,0);
   //hWp_PtReco_zoomin ->GetYaxis()->SetTitleOffset(1.8);
   //hWp_PtReco_zoomin -> GetXaxis() -> SetTitle("Recoil P_{T}");
   //hWp_PtRecoCorrected_zoomin ->SetFillStyle(3448);
   //hWp_PtRecoCorrected_zoomin ->SetLineColor(kGreen);
   //hWp_PtRecoCorrected_zoomin ->SetFillColor(kGreen);
   hWp_PtReco_zoomin          -> SetTitle("");
   hWp_PtReco_zoomin          -> SetStats(0);
   hWp_PtReco_zoomin          -> Draw();
   //hWp_PtWGen_zoomin           ->SetFillStyle(0);
   //hWp_PtWGen_zoomin           ->SetLineColor(kRed);
   hWp_PtRecoCorrected_zoomin -> Draw("same");
   hWp_PtWGen_zoomin          -> Draw("same"); 
   g1                         -> Draw("same");
   TLegend *leg2_paper = new TLegend(0.45, 0.6, 0.75, 0.9);
   leg2_paper -> AddEntry(hWp_PtReco_zoomin,"Before P_{T} correction", "F");
   leg2_paper -> AddEntry(hWp_PtRecoCorrected_zoomin,"After P_{T} correction", "F" );
   leg2_paper -> AddEntry(hWp_PtWGen_zoomin,"PHYTHIA Generated", "L");;
   leg2_paper -> AddEntry(g1,"RhicBOS 500 GeV", "L");
   leg2_paper -> Draw();


   c2b_paper->SaveAs(outPathPaper + "/PaperPlot_PtCorr.png");
   c2b_paper->SaveAs(outPathPaper + "/PaperPlot_PtCorr.eps");

  
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
   //hPtRecoilBinCenter -> Draw("P same");
   hPtRecoilBinMean   -> SetMarkerStyle(20);
   hPtRecoilBinMean   -> SetMarkerColor(kGreen);
   //hPtRecoilBinMean   -> Draw("P same");

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
  
   TLegend *leg4 = new TLegend(0.4, 0.7, 0.75, 0.9);
   leg4 -> AddEntry(hd_Wp_WBosonPt_zoomin_1,"STAR 2011 tran.", "P");
   leg4 -> AddEntry(hWp_Wp_WBosonPt_zoomin_1,"PHYTHIA", "F");
   leg4 -> Draw();


   c4->SaveAs(outPath + "/plot_DataMc_WpPt_zoomin.png");
   c4->SaveAs(outPath + "/plot_DataMc_WpPt_zoomin.eps");


   TCanvas *c4_Wm   = new TCanvas("c4_Wm", "", 400, 400);

   TH1F * hd_Wm_WBosonPt_zoomin_1 = (TH1F *) hd_Wm_WBosonPt_zoomin->Clone(" hd_Wm_WBosonPt_zoomin_1");

   TH1F * hWm_Wm_WBosonPt_zoomin_1 = (TH1F *) hWm_Wm_WBosonPt_zoomin->Clone(" hWm_Wm_WBosonPt_zoomin_1");

   // Normalize to the integral:
   hWm_Wm_WBosonPt_zoomin_1->Scale(hd_Wm_WBosonPt_zoomin_1->Integral()/hWm_Wm_WBosonPt_zoomin_1->Integral());

   // Normalize to the luminosity
   //hWm_Wm_WBosonPt_zoomin_1->Scale(scaleWm);

   c4_Wm->cd();
   hd_Wm_WBosonPt_zoomin_1    ->Rebin(2);
   hd_Wm_WBosonPt_zoomin_1    ->GetYaxis()->SetTitleOffset(1.8);

   hWm_Wm_WBosonPt_zoomin_1   ->Rebin(2);
   hWm_Wm_WBosonPt_zoomin_1   ->Draw();
   hd_Wm_WBosonPt_zoomin_1    ->SetMarkerStyle(20);
   hd_Wm_WBosonPt_zoomin_1    ->SetTitle("W- sample");
   hd_Wm_WBosonPt_zoomin_1    ->Draw("E0");
   hWm_Wm_WBosonPt_zoomin_1   ->Draw("same");
   hd_Wm_WBosonPt_zoomin_1    ->Draw("E0 same");
  
   TLegend *leg4_Wm = new TLegend(0.4, 0.7, 0.75, 0.9);
   leg4_Wm -> AddEntry(hd_Wm_WBosonPt_zoomin_1,"STAR 2011 tran.", "P");
   leg4_Wm -> AddEntry(hWm_Wm_WBosonPt_zoomin_1,"PHYTHIA", "F");
   leg4_Wm -> Draw();


   c4_Wm->SaveAs(outPath + "/plot_DataMc_WmPt_zoomin.png");
   c4_Wm->SaveAs(outPath + "/plot_DataMc_WmPt_zoomin.eps");


   TCanvas *c4II   = new TCanvas("c4II", "", 400, 400);

   TH1F * hd_Wp_WBosonPt_1 = (TH1F *) hd_Wp_WBosonPt->Clone(" hd_Wp_WBosonPt_1");

   TH1F * hWp_Wp_WBosonPt_1 = (TH1F *) hWp_Wp_WBosonPt->Clone(" hWp_Wp_WBosonPt_1");

   // Normalize to the integral:
   hWp_Wp_WBosonPt_1->Scale(hd_Wp_WBosonPt_1->Integral()/hWp_Wp_WBosonPt_1->Integral());

   // Normalize to the luminosity
   //hWp_Wp_WBosonPt_1->Scale(scaleWp);

   c4II->cd();
   hd_Wp_WBosonPt_1    ->Rebin(2);
   hd_Wp_WBosonPt_1    ->GetYaxis()->SetTitleOffset(1.8);

   hWp_Wp_WBosonPt_1   ->Rebin(2);
   hWp_Wp_WBosonPt_1   ->Draw();
   hd_Wp_WBosonPt_1    ->SetMarkerStyle(20);
   hd_Wp_WBosonPt_1    ->SetTitle("W+ sample");
   hd_Wp_WBosonPt_1    ->Draw("E0");
   hWp_Wp_WBosonPt_1   ->Draw("same");
   hd_Wp_WBosonPt_1    ->Draw("E0 same");
  
   TLegend *leg4II = new TLegend(0.4, 0.7, 0.75, 0.9);
   leg4II -> AddEntry(hd_Wp_WBosonPt_1,"STAR 2011 tran.", "P");
   leg4II -> AddEntry(hWp_Wp_WBosonPt_1,"PHYTHIA", "F");
   leg4II -> Draw();


   c4II->SaveAs(outPath + "/plot_DataMc_WpPt.png");
   c4II->SaveAs(outPath + "/plot_DataMc_WpPt.eps");


   TCanvas *c4II_Wm   = new TCanvas("c4II_Wm", "", 400, 400);

   TH1F * hd_Wm_WBosonPt_1 = (TH1F *) hd_Wm_WBosonPt->Clone(" hd_Wm_WBosonPt_1");

   TH1F * hWm_Wm_WBosonPt_1 = (TH1F *) hWm_Wm_WBosonPt->Clone(" hWm_Wm_WBosonPt_1");

   // Normalize to the integral:
   hWm_Wm_WBosonPt_1->Scale(hd_Wm_WBosonPt_1->Integral()/hWm_Wm_WBosonPt_1->Integral());

   // Normalize to the luminosity
   //hWm_Wm_WBosonPt_1->Scale(scaleWm);

   c4II_Wm->cd();
   hd_Wm_WBosonPt_1    ->Rebin(2);
   hd_Wm_WBosonPt_1    ->GetYaxis()->SetTitleOffset(1.8);

   hWm_Wm_WBosonPt_1   ->Rebin(2);
   hWm_Wm_WBosonPt_1   ->Draw();
   hd_Wm_WBosonPt_1    ->SetMarkerStyle(20);
   hd_Wm_WBosonPt_1    ->SetTitle("W- sample");
   hd_Wm_WBosonPt_1    ->Draw("E0");
   hWm_Wm_WBosonPt_1   ->Draw("same");
   hd_Wm_WBosonPt_1    ->Draw("E0 same");
  
   TLegend *leg4II_Wm = new TLegend(0.4, 0.7, 0.75, 0.9);
   leg4II_Wm -> AddEntry(hd_Wm_WBosonPt_1,"STAR 2011 tran.", "P");
   leg4II_Wm -> AddEntry(hWm_Wm_WBosonPt_1,"PHYTHIA", "F");
   leg4II_Wm -> Draw();


   c4II_Wm->SaveAs(outPath + "/plot_DataMc_WmPt.png");
   c4II_Wm->SaveAs(outPath + "/plot_DataMc_WmPt.eps");


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
   //TLegend *leg4b = new TLegend(0.4, 0.7, 0.75, 0.9);
   leg4b -> AddEntry(hd_Wp_PtReco_zoomin_1,"STAR 2011 tran.", "P");
   leg4b -> AddEntry(hWp_Wp_PtReco_zoomin_1,"PHYTHIA", "F");
   leg4b -> SetTextSize(0.03);
   leg4b -> Draw();


   c4b->SaveAs(outPath + "/plot_DataMc_WpPt_nocorr.png");
   c4b->SaveAs(outPath + "/plot_DataMc_WpPt_nocorr.eps");
  

   TCanvas *c4c   = new TCanvas("c4c", "", 400, 400);

   TH1F * hd_Wp_WBosonPz_1 = (TH1F *) hd_Wp_WBosonPz->Clone(" hd_Wp_WBosonPz_1");

   TH1F * hWp_Wp_WBosonPz_1 = (TH1F *) hWp_Wp_WBosonPz->Clone(" hWp_Wp_WBosonPz_1");

   // Normalize to the integral:
   hWp_Wp_WBosonPz_1->Scale(hd_Wp_WBosonPz_1->Integral()/hWp_Wp_WBosonPz_1->Integral());

   // Normalize to the luminosity
   //hWp_Wp_WBosonPz_1->Scale(scaleWp);

   c4c->cd();
   hd_Wp_WBosonPz_1    ->Rebin(2);
   hd_Wp_WBosonPz_1    ->GetYaxis()->SetTitleOffset(1.8);

   hWp_Wp_WBosonPz_1   ->Rebin(2);
   hWp_Wp_WBosonPz_1   ->Draw();
   hd_Wp_WBosonPz_1    ->SetMarkerStyle(20);
   hd_Wp_WBosonPz_1    ->SetTitle("W+ sample"); 
   //hd_Wp_WBosonPz_1    -> SetAxisRange(0, 160, "Y");
   hd_Wp_WBosonPz_1    ->Draw("E0");
   hWp_Wp_WBosonPz_1   ->Draw("same");
   hd_Wp_WBosonPz_1    ->Draw("E0 same");
  
   TLegend *leg4c = new TLegend(0.5, 0.75, 0.75, 0.9); 
   //TLegend *leg4c = new TLegend(0.47, 0.7, 0.75, 0.9);
   leg4c -> AddEntry(hd_Wp_WBosonPz_1,"STAR 2011 tran.", "P");
   leg4c -> AddEntry(hWp_Wp_WBosonPz_1,"PHYTHIA", "F");
   leg4c -> SetTextSize(0.03);
   leg4c -> Draw();


   c4c->SaveAs(outPath + "/plot_DataMc_WpPz.png");
   c4c->SaveAs(outPath + "/plot_DataMc_WpPz.eps");
  

   TCanvas *c4c_Wm   = new TCanvas("c4c_Wm", "", 400, 400);

   TH1F * hd_Wm_WBosonPz_1 = (TH1F *) hd_Wm_WBosonPz->Clone(" hd_Wm_WBosonPz_1");

   TH1F * hWm_Wm_WBosonPz_1 = (TH1F *) hWm_Wm_WBosonPz->Clone(" hWm_Wm_WBosonPz_1");

   // Normalize to the integral:
   hWm_Wm_WBosonPz_1->Scale(hd_Wm_WBosonPz_1->Integral()/hWm_Wm_WBosonPz_1->Integral());

   // Normalize to the luminosity
   //hWm_Wm_WBosonPz_1->Scale(scaleWp);

   c4c_Wm->cd();
   hd_Wm_WBosonPz_1    ->Rebin(2);
   hd_Wm_WBosonPz_1    ->GetYaxis()->SetTitleOffset(1.8);

   hWm_Wm_WBosonPz_1   ->Rebin(2);
   hWm_Wm_WBosonPz_1   ->Draw();
   hd_Wm_WBosonPz_1    ->SetMarkerStyle(20);
   hd_Wm_WBosonPz_1    ->SetTitle("W- sample"); 
   //hd_Wm_WBosonPz_1    -> SetAxisRange(0, 160, "Y");
   hd_Wm_WBosonPz_1    ->Draw("E0");
   hWm_Wm_WBosonPz_1   ->Draw("same");
   hd_Wm_WBosonPz_1    ->Draw("E0 same");
  
   TLegend *leg4c_Wm = new TLegend(0.5, 0.75, 0.75, 0.9); 
   leg4c_Wm -> AddEntry(hd_Wm_WBosonPz_1,"STAR 2011 tran.", "P");
   leg4c_Wm -> AddEntry(hWm_Wm_WBosonPz_1,"PHYTHIA", "F");
   leg4c_Wm -> SetTextSize(0.03);
   leg4c_Wm -> Draw();


   c4c_Wm->SaveAs(outPath + "/plot_DataMc_WmPz.png");
   c4c_Wm->SaveAs(outPath + "/plot_DataMc_WmPz.eps");


   TCanvas *cPtCorrFact   = new TCanvas("cPtCorrFact", "W+ Pt correction factor", 800, 500);

   cPtCorrFact -> cd();
   cPtCorrFact -> SetLogz(1);
   hWp_PtRecoil_vs_Pt_GenOverReco  -> GetXaxis() -> SetTitle("Recoil P_{T}");
   hWp_PtRecoil_vs_Pt_GenOverReco  -> SetStats(0);
   hWp_PtRecoil_vs_Pt_GenOverReco  -> Draw();

   cPtCorrFact->SaveAs(outPath + "/plot_PtCorrFactor.png");
   cPtCorrFact->SaveAs(outPath + "/plot_PtCorrFactor.eps");


   TCanvas *cPtCorrFact_Wm   = new TCanvas("cPtCorrFact_Wm", "W- Pt correction factor", 800, 500);

   cPtCorrFact_Wm -> cd();
   cPtCorrFact_Wm -> SetLogz(1);
   hWm_PtRecoil_vs_Pt_GenOverReco  -> SetTitle("W^{-}");
   hWm_PtRecoil_vs_Pt_GenOverReco  -> GetXaxis() -> SetTitle("Recoil P_{T}");
   hWm_PtRecoil_vs_Pt_GenOverReco  -> SetStats(0);
   hWm_PtRecoil_vs_Pt_GenOverReco  -> Draw();

   cPtCorrFact_Wm->SaveAs(outPath + "/plot_PtCorrFactor_Wm.png");
   cPtCorrFact_Wm->SaveAs(outPath + "/plot_PtCorrFactor_Wm.eps");


   TCanvas *cPtCorrFact_ZDChigh   = new TCanvas("cPtCorrFact_ZDChigh", "W+ Pt correction factor for ZDC rate > 90k", 800, 500);

   cPtCorrFact_ZDChigh -> cd();
   cPtCorrFact_ZDChigh -> SetLogz(1);
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco  -> SetTitle("W^{+} - ZDC rate > 90k");
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco  -> GetXaxis() -> SetTitle("Recoil P_{T}");
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco  -> SetStats(0);
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco  -> Draw();

   cPtCorrFact_ZDChigh->SaveAs(outPath + "/plot_PtCorrFactor_ZDChigh.png");
   cPtCorrFact_ZDChigh->SaveAs(outPath + "/plot_PtCorrFactor_ZDChigh.eps");


   TCanvas *cPtCorrFact_TuneA   = new TCanvas("cPtCorrFact_TuneA", "W+ Pt correction factor for Tune A", 800, 500);

   cPtCorrFact_TuneA -> cd();
   cPtCorrFact_TuneA -> SetLogz(1);
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco  -> SetTitle("W^{+} - Tune A");
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco  -> GetXaxis() -> SetTitle("Recoil P_{T}");
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco  -> SetStats(0);
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco  -> Draw();

   cPtCorrFact_TuneA->SaveAs(outPath + "/plot_PtCorrFactor_TuneA.png");
   cPtCorrFact_TuneA->SaveAs(outPath + "/plot_PtCorrFactor_TuneA.eps");


   TCanvas *cPtCorrFact_pfx   = new TCanvas("cPtCorrFact_pfx", "Pt correction factor", 800, 500);

   cPtCorrFact_pfx -> cd();
   hWp_PtRecoil_vs_Pt_GenOverReco -> ProfileX("hWp_PtRecoil_vs_Pt_GenOverReco_pfx",1,-1,"s");
   hWm_PtRecoil_vs_Pt_GenOverReco -> ProfileX("hWm_PtRecoil_vs_Pt_GenOverReco_pfx",1,-1,"s");
   hWp_PtRecoil_vs_Pt_GenOverReco_pfx -> SetTitle("; Recoil P_{T}; Correction Factor");
   hWp_PtRecoil_vs_Pt_GenOverReco_pfx -> SetMarkerStyle(20);
   hWp_PtRecoil_vs_Pt_GenOverReco_pfx -> SetMarkerColor(kRed);
   hWm_PtRecoil_vs_Pt_GenOverReco_pfx -> SetMarkerStyle(20);
   hWm_PtRecoil_vs_Pt_GenOverReco_pfx -> SetMarkerColor(kBlue);

   hWp_PtRecoil_vs_Pt_GenOverReco_pfx -> Draw("E");
   hWm_PtRecoil_vs_Pt_GenOverReco_pfx -> Draw("Esame");
  
   TLegend *legPtCorrFact_pfx = new TLegend(0.5, 0.75, 0.75, 0.9); 
   legPtCorrFact_pfx -> AddEntry(hWp_PtRecoil_vs_Pt_GenOverReco_pfx,"W^{+}", "P");
   legPtCorrFact_pfx -> AddEntry(hWm_PtRecoil_vs_Pt_GenOverReco_pfx,"W^{-}", "P");
   //legPtCorrFact_pfx -> SetTextSize(0.03);
   legPtCorrFact_pfx -> Draw();;

   cPtCorrFact_pfx->SaveAs(outPath + "/plot_PtCorrFactor_ProfileX.png");
   cPtCorrFact_pfx->SaveAs(outPath + "/plot_PtCorrFactor_ProfileX.eps");


   TCanvas *cPtCorrFact_ZDChigh_pfx   = new TCanvas("cPtCorrFact_ZDChigh_pfx", "Pt correction factor ZDC rate > 90k", 800, 500);

   cPtCorrFact_ZDChigh_pfx -> cd();
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco -> ProfileX("hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco_pfx",1,-1,"s");
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco_pfx -> SetTitle("; Recoil P_{T}; Correction Factor");
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco_pfx -> SetMarkerStyle(20);
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco_pfx -> SetMarkerColor(kGreen);

   hWp_PtRecoil_vs_Pt_GenOverReco_pfx         -> Draw("E");
   hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco_pfx -> Draw("Esame");
  
   TLegend *legPtCorrFact_ZDChigh_pfx = new TLegend(0.5, 0.75, 0.75, 0.9); 
   legPtCorrFact_ZDChigh_pfx -> AddEntry(hWp_ZDChigh_PtRecoil_vs_Pt_GenOverReco_pfx,"W^{+}, ZDC rate > 90k ", "P");
   legPtCorrFact_ZDChigh_pfx -> AddEntry(hWp_PtRecoil_vs_Pt_GenOverReco_pfx,"W^{+}", "P");
   legPtCorrFact_ZDChigh_pfx -> Draw();;

   cPtCorrFact_ZDChigh_pfx->SaveAs(outPath + "/plot_PtCorrFactor_ZDChigh_ProfileX.png");
   cPtCorrFact_ZDChigh_pfx->SaveAs(outPath + "/plot_PtCorrFactor_ZDChigh_ProfileX.eps");


   TCanvas *cPtCorrFact_TuneA_pfx   = new TCanvas("cPtCorrFact_TuneA_pfx", "Pt correction factor Tune A", 800, 500);

   cPtCorrFact_TuneA_pfx -> cd();
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco -> ProfileX("hWp_TuneA_PtRecoil_vs_Pt_GenOverReco_pfx",1,-1,"s");
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco_pfx -> SetTitle("; Recoil P_{T}; Correction Factor");
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco_pfx -> SetMarkerStyle(20);
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco_pfx -> SetMarkerColor(kGreen);

   hWp_PtRecoil_vs_Pt_GenOverReco_pfx         -> Draw("E");
   hWp_TuneA_PtRecoil_vs_Pt_GenOverReco_pfx -> Draw("Esame");
  
   TLegend *legPtCorrFact_TuneA_pfx = new TLegend(0.5, 0.75, 0.75, 0.9); 
   legPtCorrFact_TuneA_pfx -> AddEntry(hWp_TuneA_PtRecoil_vs_Pt_GenOverReco_pfx,"W^{+}, Tune A ", "P");
   legPtCorrFact_TuneA_pfx -> AddEntry(hWp_PtRecoil_vs_Pt_GenOverReco_pfx,"W^{+}, Perugia 0 ", "P");
   legPtCorrFact_TuneA_pfx -> Draw();;

   cPtCorrFact_TuneA_pfx->SaveAs(outPath + "/plot_PtCorrFactor_TuneA_ProfileX.png");
   cPtCorrFact_TuneA_pfx->SaveAs(outPath + "/plot_PtCorrFactor_TuneA_ProfileX.eps");


   TCanvas *c4d   = new TCanvas("c4d", "W Rapidity vs Pz", 700, 500);

   c4d->cd();
   hWp_Wp_WBosonPzVsRapidity  -> SetStats(0);
   hWp_Wp_WBosonPzVsRapidity  -> Draw();
  
   TLine *redLineDash = new TLine(-50, -1, -50, 1);
   redLineDash -> SetLineStyle(2);
   redLineDash -> SetLineColor(kRed);
   redLineDash -> SetLineWidth(3);
   redLineDash -> Draw();
   redLineDash -> DrawLine(50, -1, 50, 1);
   redLineDash -> DrawLine(-60, -0.6, 60, -0.6);
   redLineDash -> DrawLine(-60, 0.6, 60, 0.6);

   c4d->SaveAs(outPath + "/plot_MCWp_PzVsRap.png");
   c4d->SaveAs(outPath + "/plot_MCWp_PzVsRap.eps");


   TCanvas *cRecVsGenRap   = new TCanvas("cRecVsGenRap", "Reco Vs Gen Wp-Rapidity", 700, 500);

   cRecVsGenRap -> cd();
   cRecVsGenRap -> SetGrid(0,0);
   //cRecVsGenRap -> SetLogz(1);
   hWp_hRecoVsGenWBosonRap->Draw();
   //hWp_hRecoVsGenWBosonRap->GetYaxis()->SetTitleOffset(1.8);
   hWp_hRecoVsGenWBosonRap->SetStats(0);

   cRecVsGenRap->SaveAs(outPath + "/hRecoVsGenWBosonRap.png");
   cRecVsGenRap->SaveAs(outPath + "/hRecoVsGenWBosonRap.eps");


   TCanvas *c5   = new TCanvas("c5", "An prediction with evol.", 400, 400);

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

   TCanvas *c5bis   = new TCanvas("c5bis", "An prediction with evol.", 400, 400);

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


   TCanvas *c5c   = new TCanvas("c5c", "An prediction no evol.", 400, 400);

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

   TCanvas *c5d   = new TCanvas("c5d", "An prediction no evol.", 400, 400);

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


   TCanvas *c6   = new TCanvas("c6", "Wm An prediction with evol.", 500, 500);

   //hWm_An_evol_ZK_Vs_PtGen ->ProfileX("hWm_An_evol_ZK_Vs_PtGen_pfx"); 
   //hWm_An_evol_ZK_Vs_PtRec ->ProfileX("hWm_An_evol_ZK_Vs_PtRec_pfx"); 

   hWm_An_evol_ZK_Vs_PtGen ->ProjectionY("hWm_An_evol_ZK_Vs_PtGen_pjy1", 1, 1);
   hWm_An_evol_ZK_Vs_PtGen ->ProjectionY("hWm_An_evol_ZK_Vs_PtGen_pjy2", 2, 2); 
   hWm_An_evol_ZK_Vs_PtGen ->ProjectionY("hWm_An_evol_ZK_Vs_PtGen_pjy3", 3, 3);  
   hWm_An_evol_ZK_Vs_PtGen ->ProjectionY("hWm_An_evol_ZK_Vs_PtGen_pjy4", 4, 4);  
   hWm_An_evol_ZK_Vs_PtGen ->ProjectionY("hWm_An_evol_ZK_Vs_PtGen_pjy5", 5, 5);  
   hWm_An_evol_ZK_Vs_PtGen ->ProjectionY("hWm_An_evol_ZK_Vs_PtGen_pjy6", 6, 6);  
   hWm_An_evol_ZK_Vs_PtGen ->ProjectionY("hWm_An_evol_ZK_Vs_PtGen_pjy7", 7, 7);  

   hWm_An_evol_ZK_Vs_PtRec ->ProjectionY("hWm_An_evol_ZK_Vs_PtRec_pjy1", 1, 1); 
   hWm_An_evol_ZK_Vs_PtRec ->ProjectionY("hWm_An_evol_ZK_Vs_PtRec_pjy2", 2, 2); 
   hWm_An_evol_ZK_Vs_PtRec ->ProjectionY("hWm_An_evol_ZK_Vs_PtRec_pjy3", 3, 3);  
   hWm_An_evol_ZK_Vs_PtRec ->ProjectionY("hWm_An_evol_ZK_Vs_PtRec_pjy4", 4, 4);  
   hWm_An_evol_ZK_Vs_PtRec ->ProjectionY("hWm_An_evol_ZK_Vs_PtRec_pjy5", 5, 5);  
   hWm_An_evol_ZK_Vs_PtRec ->ProjectionY("hWm_An_evol_ZK_Vs_PtRec_pjy6", 6, 6);  
   hWm_An_evol_ZK_Vs_PtRec ->ProjectionY("hWm_An_evol_ZK_Vs_PtRec_pjy7", 7, 7);


   //c6->Divide(1, 2);

   c6->cd();
   hWm_An_evol_ZK_Vs_PtGen->Draw();
   hWm_An_evol_ZK_Vs_PtGen->GetYaxis()->SetTitleOffset(1.8);
   hWm_An_evol_ZK_Vs_PtGen->SetStats(0);
   hWm_An_evol_ZK_Vs_PtGen->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{GEN}; A_{N};");
   //c6_2->cd();
   //hWm_An_evol_ZK_Vs_PtGen_pfx->Draw();
   //hWm_An_evol_ZK_Vs_PtGen_pfx->GetYaxis()->SetTitleOffset(2.);
   //hWm_An_evol_ZK_Vs_PtGen_pfx->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{GEN}; A_{N}");

   c6->SaveAs(outPath + "/plot_Wm_An_evol_ZK.png");
   c6->SaveAs(outPath + "/plot_Wm_An_evol_ZK.eps");

   TCanvas *c6bis   = new TCanvas("c6bis", "Wm An prediction with evol.", 500, 500);

   //c6bis->Divide(1, 2);

   c6bis->cd();
   hWm_An_evol_ZK_Vs_PtRec->Draw();
   hWm_An_evol_ZK_Vs_PtRec->GetYaxis()->SetTitleOffset(1.8);
   hWm_An_evol_ZK_Vs_PtRec->SetStats(0);
   hWm_An_evol_ZK_Vs_PtRec->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{REC}; A_{N};");
   //c6bis_2->cd();
   //hWm_An_evol_ZK_Vs_PtRec_pfx->Draw();
   //hWm_An_evol_ZK_Vs_PtRec_pfx->GetYaxis()->SetTitleOffset(2.);
   //hWm_An_evol_ZK_Vs_PtRec_pfx->SetTitle("Prediction inlcudes evolution; W^{-} P_{T}^{REC}; A_{N}");

   c6bis->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Ptrec.png");
   c6bis->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Ptrec.eps");


   TCanvas *c6c   = new TCanvas("c6c", "Wm An prediction no evol.", 500, 500);

   //hWm_An_noevo_ZK_Vs_PtGen ->ProfileX("hWm_An_noevo_ZK_Vs_PtGen_pfx"); 
   //hWm_An_noevo_ZK_Vs_PtRec ->ProfileX("hWm_An_noevo_ZK_Vs_PtRec_pfx");
   //hWm_An_noevo_ZK_Vs_PtGen_zoomin ->ProfileX("hWm_An_noevo_ZK_Vs_PtGen_zoomin_pfx"); 
   //hWm_An_noevo_ZK_Vs_PtRec_zoomin ->ProfileX("hWm_An_noevo_ZK_Vs_PtRec_zoomin_pfx"); 

   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy1", 1, 1);
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy2", 2, 2); 
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy3", 3, 3);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy4", 4, 4);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy5", 5, 5);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy6", 6, 6);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy7", 7, 7);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy8", 8, 8);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy9", 9, 9);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy10", 10, 10);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy11", 11, 11);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy12", 12, 12);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy13", 13, 13);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy14", 14, 14);  
   hWm_An_noevo_ZK_Vs_PtGen ->ProjectionY("hWm_An_noevo_ZK_Vs_PtGen_pjy15", 15, 15);

   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy1", 1, 1); 
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy2", 2, 2); 
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy3", 3, 3);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy4", 4, 4);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy5", 5, 5);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy6", 6, 6);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy7", 7, 7);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy8", 8, 8);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy9", 9, 9);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy10", 10, 10);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy11", 11, 11);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy12", 12, 12);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy13", 13, 13);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy14", 14, 14);  
   hWm_An_noevo_ZK_Vs_PtRec ->ProjectionY("hWm_An_noevo_ZK_Vs_PtRec_pjy15", 15, 15);


   c6c->cd();
   hWm_An_noevo_ZK_Vs_PtGen->Draw();
   hWm_An_noevo_ZK_Vs_PtGen->GetYaxis()->SetTitleOffset(1.8);
   hWm_An_noevo_ZK_Vs_PtGen->SetStats(0);
   hWm_An_noevo_ZK_Vs_PtGen->SetTitle("Prediction no evolution; W^{-} P_{T}^{GEN}; A_{N};");

   c6c->SaveAs(outPath + "/plot_Wm_An_noevo_ZK.png");
   c6c->SaveAs(outPath + "/plot_Wm_An_noevo_ZK.eps");

   TCanvas *c6d   = new TCanvas("c6d", "Wm An prediction no evol.", 500, 500);

   c6d->cd();
   hWm_An_noevo_ZK_Vs_PtRec->Draw();
   hWm_An_noevo_ZK_Vs_PtRec->GetYaxis()->SetTitleOffset(1.8);
   hWm_An_noevo_ZK_Vs_PtRec->SetStats(0);
   hWm_An_noevo_ZK_Vs_PtRec->SetTitle("Prediction no evolution; W^{-} P_{T}^{REC}; A_{N};");

   c6d->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Ptrec.png");
   c6d->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Ptrec.eps");

    
   Print_Projections_evol(outPath); 
   CalcSyst_evol(outPath); 
   //Print_Projections_noevo(outPath);
   //CalcSyst_noevo();


   TCanvas *c8a   = new TCanvas("c8a", "Wm An prediction no evol.", 500, 500);


   hWm_An_evol_ZK_Vs_RapGen ->ProjectionY("hWm_An_evol_ZK_Vs_RapGen_pjy1", 1, 1);
   hWm_An_evol_ZK_Vs_RapGen ->ProjectionY("hWm_An_evol_ZK_Vs_RapGen_pjy2", 2, 2); 
   hWm_An_evol_ZK_Vs_RapGen ->ProjectionY("hWm_An_evol_ZK_Vs_RapGen_pjy3", 3, 3);  
   //hWm_An_evol_ZK_Vs_RapGen ->ProjectionY("hWm_An_evol_ZK_Vs_RapGen_pjy4", 4, 4);

   hWm_An_evol_ZK_Vs_RapRec ->ProjectionY("hWm_An_evol_ZK_Vs_RapRec_pjy1", 1, 1); 
   hWm_An_evol_ZK_Vs_RapRec ->ProjectionY("hWm_An_evol_ZK_Vs_RapRec_pjy2", 2, 2); 
   hWm_An_evol_ZK_Vs_RapRec ->ProjectionY("hWm_An_evol_ZK_Vs_RapRec_pjy3", 3, 3);  
   //hWm_An_evol_ZK_Vs_RapRec ->ProjectionY("hWm_An_evol_ZK_Vs_RapRec_pjy4", 4, 4);


   c8a->cd();
   hWm_An_evol_ZK_Vs_RapGen->Draw();
   hWm_An_evol_ZK_Vs_RapGen->GetYaxis()->SetTitleOffset(1.8);
   hWm_An_evol_ZK_Vs_RapGen->SetStats(0);
   hWm_An_evol_ZK_Vs_RapGen->SetTitle("Prediction with evolution; W^{-} P_{T}^{GEN}; A_{N};");

   c8a->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Rapgen.png");
   c8a->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Rapgen.eps");


   TCanvas *c8b   = new TCanvas("c8b", "Wm An Vs Rapidity prediction with evol.", 500, 500);

   c8b->cd();
   hWm_An_evol_ZK_Vs_RapRec->Draw();
   hWm_An_evol_ZK_Vs_RapRec->GetYaxis()->SetTitleOffset(1.8);
   hWm_An_evol_ZK_Vs_RapRec->SetStats(0);
   hWm_An_evol_ZK_Vs_RapRec->SetTitle("Prediction with evolution; W^{-} y^{REC}; A_{N};");

   c8b->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Raprec.png");
   c8b->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Raprec.eps");


   Print_Projections_evol_Rapidity(outPath);
   CalcSyst_evol_Rapidity(outPath); 
     
}


void Print_Projections_evol(TString outPath)
{

   TCanvas *c7a   = new TCanvas("c7a", "Wm An prediction with evol.", 1200, 800);

   //TF1 *fGaus1 = new TF1("fGaus1", "gaus", 0, 0.1);
   //TF1 *fGaus2 = new TF1("fGaus2", "gaus", 0, 0.45);

   c7a-> Divide(2,4);
   //c7a-> Divide(3,5);
   c7a_1->cd();
   hWm_An_evol_ZK_Vs_PtGen_pjy1 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtGen_pjy1 -> SetTitle("PtGen BIN 1: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtGen_pjy1 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_PtGen_pjy1 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtGen_pjy1 -> Fit("fGaus2", "R");
   //hWm_An_evol_ZK_Vs_PtGen_pjy1 -> Fit("gaus");
   hWm_An_evol_ZK_Vs_PtGen_pjy1 -> Draw();
   c7a_2->cd();
   hWm_An_evol_ZK_Vs_PtGen_pjy2 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtGen_pjy2 ->SetTitle("PtGen BIN 2: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtGen_pjy2 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_PtGen_pjy2 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtGen_pjy2 -> Fit("fGaus2", "R");
   hWm_An_evol_ZK_Vs_PtGen_pjy2 -> Draw();
   c7a_3->cd();
   hWm_An_evol_ZK_Vs_PtGen_pjy3 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtGen_pjy3 ->SetTitle("PtGen BIN 3: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtGen_pjy3 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_PtGen_pjy3 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtGen_pjy3 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtGen_pjy3 -> Draw();
   c7a_4->cd();
   hWm_An_evol_ZK_Vs_PtGen_pjy4 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtGen_pjy4 ->SetTitle("PtGen BIN 4: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtGen_pjy4 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_PtGen_pjy4 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtGen_pjy4 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtGen_pjy4 -> Draw();
   c7a_5->cd();
   hWm_An_evol_ZK_Vs_PtGen_pjy5 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtGen_pjy5 ->SetTitle("PtGen BIN 5: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtGen_pjy5 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_PtGen_pjy5 -> SetAxisRange(0, 0.1, "X"); 
   //hWm_An_evol_ZK_Vs_PtGen_pjy5 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtGen_pjy5 -> Draw();
   c7a_6->cd();
   hWm_An_evol_ZK_Vs_PtGen_pjy6 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtGen_pjy6 ->SetTitle("PtGen BIN 6: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtGen_pjy6 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_PtGen_pjy6 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtGen_pjy6 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtGen_pjy6 -> Draw();
   c7a_7->cd();
   hWm_An_evol_ZK_Vs_PtGen_pjy7 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtGen_pjy7 ->SetTitle("PtGen BIN 7: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtGen_pjy7 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_PtGen_pjy7 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtGen_pjy7 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtGen_pjy7 -> Draw();

   c7a->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Vs_PtGen_projs.png");
   c7a->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Vs_PtGen_projs.eps");


   TCanvas *c7b   = new TCanvas("c7b", "Wm An prediction with evol.", 1200, 800);

   c7b-> Divide(2,4);
   //c7b-> Divide(3,5);
   c7b_1->cd();
   hWm_An_evol_ZK_Vs_PtRec_pjy1 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtRec_pjy1 -> SetTitle("PtRec BIN 1: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtRec_pjy1 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_PtRec_pjy1 -> SetAxisRange(0, 0.1, "X");
   hWm_An_evol_ZK_Vs_PtRec_pjy1 -> Draw(""); 
   //hWm_An_evol_ZK_Vs_PtRec_pjy1 -> Fit("fGaus1", "R");
   //hWm_An_evol_ZK_Vs_PtRec_pjy1 -> Fit("gaus");
   c7b_2->cd();
   hWm_An_evol_ZK_Vs_PtRec_pjy2 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtRec_pjy2 -> SetTitle("PtRec BIN 2: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtRec_pjy2 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_PtRec_pjy2 -> SetAxisRange(0, 0.1, "X");
   hWm_An_evol_ZK_Vs_PtRec_pjy2 -> Draw();
   //hWm_An_evol_ZK_Vs_PtRec_pjy2 -> Fit("fGaus1", "R");
   c7b_3->cd();
   hWm_An_evol_ZK_Vs_PtRec_pjy3 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtRec_pjy3 -> SetTitle("PtRec BIN 3: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtRec_pjy3 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_PtRec_pjy3 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtRec_pjy3 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtRec_pjy3 -> Draw();
   c7b_4->cd();
   hWm_An_evol_ZK_Vs_PtRec_pjy4 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtRec_pjy4 -> SetTitle("PtRec BIN 4: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtRec_pjy4 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_PtRec_pjy4 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtRec_pjy4 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtRec_pjy4 -> Draw();
   c7b_5->cd();
   hWm_An_evol_ZK_Vs_PtRec_pjy5 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtRec_pjy5 -> SetTitle("PtRec BIN 5: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtRec_pjy5 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_PtRec_pjy5 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtRec_pjy5 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtRec_pjy5 -> Draw();
   c7b_6->cd();
   hWm_An_evol_ZK_Vs_PtRec_pjy6 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtRec_pjy6 -> SetTitle("PtRec BIN 6: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtRec_pjy6 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_PtRec_pjy6 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtRec_pjy6 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtRec_pjy6 -> Draw();
   c7b_7->cd();
   hWm_An_evol_ZK_Vs_PtRec_pjy7 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_PtRec_pjy7 -> SetTitle("PtRec BIN 7: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_PtRec_pjy7 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_PtRec_pjy7 -> SetAxisRange(0, 0.1, "X");
   //hWm_An_evol_ZK_Vs_PtRec_pjy7 -> Fit("fGaus1", "R");
   hWm_An_evol_ZK_Vs_PtRec_pjy7 -> Draw();

   c7b->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Vs_PtRec_projs.png");
   c7b->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Vs_PtRec_projs.eps");
}


void Print_Projections_noevo(TString outPath)
{

   TCanvas *c7c   = new TCanvas("c7c", "Wm An prediction no evol.", 1200, 800);

   TF1 *fGaus1 = new TF1("fGaus1", "gaus", 0, 0.1);
   TF1 *fGaus2 = new TF1("fGaus2", "gaus", 0, 0.45);

   c7c-> Divide(3,5);
   c7c_1->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> SetTitle("PtGen BIN 1: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> Fit("fGaus2", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy1 -> Draw();
   c7c_2->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 ->SetTitle("PtGen BIN 2: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 -> Fit("fGaus2", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy2 -> Draw();
   c7c_3->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 ->SetTitle("PtGen BIN 3: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy3 -> Draw();
   c7c_4->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 ->SetTitle("PtGen BIN 4: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy4 -> Draw();
   c7c_5->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy5 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy5 ->SetTitle("PtGen BIN 5: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy5 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy5 -> SetAxisRange(0, 0.1, "X"); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy5 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy5 -> Draw();
   c7c_6->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy6 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy6 ->SetTitle("PtGen BIN 6: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy6 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy6 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtGen_pjy6 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy6 -> Draw();
   c7c_7->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy7 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy7 ->SetTitle("PtGen BIN 7: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy7 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy7 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtGen_pjy7 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy7 -> Draw();
   c7c_8->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy8 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy8 ->SetTitle("PtGen BIN 8: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy8 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy8 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtGen_pjy8 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy8 -> Draw();
   c7c_9->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy9 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy9 ->SetTitle("PtGen BIN 9: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy9 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy9 -> SetAxisRange(0, 0.1, "X"); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy9 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy9 -> Draw();
   c7c_10->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy10 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy10 ->SetTitle("PtGen BIN 10: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy10 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy10 -> SetAxisRange(0, 0.1, "X"); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy10 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy10 -> Draw();
   c7c_11->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy11 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy11 ->SetTitle("PtGen BIN 11: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy11 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy11 -> SetAxisRange(0, 0.1, "X"); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy11 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy11 -> Draw();
   c7c_12->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy12 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy12 ->SetTitle("PtGen BIN 12: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy12 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy12 -> SetAxisRange(0, 0.1, "X"); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy12 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy12 -> Draw();
   c7c_13->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy13 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy13 ->SetTitle("PtGen BIN 13: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy13 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy13 -> SetAxisRange(0, 0.1, "X");  
   hWm_An_noevo_ZK_Vs_PtGen_pjy13 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy13 -> Draw();
   c7c_14->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy14 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy14 ->SetTitle("PtGen BIN 14: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy14 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtGen_pjy14 -> SetAxisRange(0, 0.1, "X");  
   hWm_An_noevo_ZK_Vs_PtGen_pjy14 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy14 -> Draw();
   c7c_15->cd();
   hWm_An_noevo_ZK_Vs_PtGen_pjy15 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtGen_pjy15 ->SetTitle("PtGen BIN 15: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtGen_pjy15 -> SetFillColor(kYellow); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy15 -> SetAxisRange(0, 0.1, "X"); 
   hWm_An_noevo_ZK_Vs_PtGen_pjy15 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtGen_pjy15 -> Draw();


   c7c->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Vs_PtGen_projs.png");
   c7c->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Vs_PtGen_projs.eps");


   TCanvas *c7d   = new TCanvas("c7d", "Wm An prediction no evol.", 1200, 800);

   c7d-> Divide(3,5);
   c7d_1->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> SetTitle("PtRec BIN 1: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> Draw(""); 
   hWm_An_noevo_ZK_Vs_PtRec_pjy1 -> Fit("fGaus1", "R");
   c7d_2->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> SetTitle("PtRec BIN 2: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> Draw();
   hWm_An_noevo_ZK_Vs_PtRec_pjy2 -> Fit("fGaus1", "R");
   c7d_3->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> SetTitle("PtRec BIN 3: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy3 -> Draw();
   c7d_4->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> SetTitle("PtRec BIN 4: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy4 -> Draw();
   c7d_5->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy5 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy5 -> SetTitle("PtRec BIN 5: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy5 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy5 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy5 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy5 -> Draw();
   c7d_6->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy6 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy6 -> SetTitle("PtRec BIN 6: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy6 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy6 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy6 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy6 -> Draw();
   c7d_7->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy7 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy7 -> SetTitle("PtRec BIN 7: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy7 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy7 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy7 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy7 -> Draw();
   c7d_8->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy8 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy8 -> SetTitle("PtRec BIN 8: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy8 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy8 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy8 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy8 -> Draw();
   c7d_9->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy9 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy9 -> SetTitle("PtRec BIN 9: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy9 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy9 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy9 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy9 -> Draw();
   c7d_10->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy10 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy10 -> SetTitle("PtRec BIN 10: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy10 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy10 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy10 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy10 -> Draw();
   c7d_11->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy11 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy11 -> SetTitle("PtRec BIN 11: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy11 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy11 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy11 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy11 -> Draw();
   c7d_12->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy12 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy12 -> SetTitle("PtRec BIN 12: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy12 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy12 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy12 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy12 -> Draw();
   c7d_13->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy13 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy13 -> SetTitle("PtRec BIN 13: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy13 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy13 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy13 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy13 -> Draw();
   c7d_14->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy14 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy14 -> SetTitle("PtRec BIN 14: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy14 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy14 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy14 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy14 -> Draw();
   c7d_15->cd();
   hWm_An_noevo_ZK_Vs_PtRec_pjy15 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_noevo_ZK_Vs_PtRec_pjy15 -> SetTitle("PtRec BIN 15: no evolution; W- A_{N}");
   hWm_An_noevo_ZK_Vs_PtRec_pjy15 -> SetFillColor(kYellow);
   hWm_An_noevo_ZK_Vs_PtRec_pjy15 -> SetAxisRange(0, 0.1, "X");
   hWm_An_noevo_ZK_Vs_PtRec_pjy15 -> Fit("fGaus1", "R");
   hWm_An_noevo_ZK_Vs_PtRec_pjy15 -> Draw();


   c7d->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Vs_PtRec_projs.png");
   c7d->SaveAs(outPath + "/plot_Wm_An_noevo_ZK_Vs_PtRec_projs.eps");
}


void CalcSyst_evol(TString outPath)
{
 
   Double_t mean_Gen_evol_pjy1  = hWm_An_evol_ZK_Vs_PtGen_pjy1  -> GetMean();
   Double_t mean_Gen_evol_pjy2  = hWm_An_evol_ZK_Vs_PtGen_pjy2  -> GetMean();
   Double_t mean_Gen_evol_pjy3  = hWm_An_evol_ZK_Vs_PtGen_pjy3  -> GetMean();
   Double_t mean_Gen_evol_pjy4  = hWm_An_evol_ZK_Vs_PtGen_pjy4  -> GetMean();
   Double_t mean_Gen_evol_pjy5  = hWm_An_evol_ZK_Vs_PtGen_pjy5  -> GetMean();
   Double_t mean_Gen_evol_pjy6  = hWm_An_evol_ZK_Vs_PtGen_pjy6  -> GetMean();
   Double_t mean_Gen_evol_pjy7  = hWm_An_evol_ZK_Vs_PtGen_pjy7  -> GetMean(); 

   Double_t mean_Rec_evol_pjy1  = hWm_An_evol_ZK_Vs_PtRec_pjy1  -> GetMean();
   Double_t mean_Rec_evol_pjy2  = hWm_An_evol_ZK_Vs_PtRec_pjy2  -> GetMean();
   Double_t mean_Rec_evol_pjy3  = hWm_An_evol_ZK_Vs_PtRec_pjy3  -> GetMean();
   Double_t mean_Rec_evol_pjy4  = hWm_An_evol_ZK_Vs_PtRec_pjy4  -> GetMean(); 
   Double_t mean_Rec_evol_pjy5  = hWm_An_evol_ZK_Vs_PtRec_pjy5  -> GetMean();
   Double_t mean_Rec_evol_pjy6  = hWm_An_evol_ZK_Vs_PtRec_pjy6  -> GetMean();
   Double_t mean_Rec_evol_pjy7  = hWm_An_evol_ZK_Vs_PtRec_pjy7  -> GetMean(); 

   cout << "mean_Gen_evol_pjy1:  " << mean_Gen_evol_pjy1  << endl; 
   cout << "mean_Gen_evol_pjy2:  " << mean_Gen_evol_pjy2  << endl; 
   cout << "mean_Gen_evol_pjy3:  " << mean_Gen_evol_pjy3  << endl; 
   cout << "mean_Gen_evol_pjy4:  " << mean_Gen_evol_pjy4  << endl;
   cout << "mean_Gen_evol_pjy5:  " << mean_Gen_evol_pjy5  << endl; 
   cout << "mean_Gen_evol_pjy6:  " << mean_Gen_evol_pjy6  << endl; 
   cout << "mean_Gen_evol_pjy7:  " << mean_Gen_evol_pjy7  << endl; 

   cout << "mean_Rec_evol_pjy1:  " << mean_Rec_evol_pjy1  << endl; 
   cout << "mean_Rec_evol_pjy2:  " << mean_Rec_evol_pjy2  << endl; 
   cout << "mean_Rec_evol_pjy3:  " << mean_Rec_evol_pjy3  << endl; 
   cout << "mean_Rec_evol_pjy4:  " << mean_Rec_evol_pjy4  << endl;
   cout << "mean_Rec_evol_pjy5:  " << mean_Rec_evol_pjy5  << endl; 
   cout << "mean_Rec_evol_pjy6:  " << mean_Rec_evol_pjy6  << endl; 
   cout << "mean_Rec_evol_pjy7:  " << mean_Rec_evol_pjy7  << endl; 

   Double_t diff_mean_evol_pjy1  = mean_Rec_evol_pjy1 - mean_Gen_evol_pjy1;
   Double_t diff_mean_evol_pjy2  = mean_Rec_evol_pjy2 - mean_Gen_evol_pjy2;
   Double_t diff_mean_evol_pjy3  = mean_Rec_evol_pjy3 - mean_Gen_evol_pjy3;
   Double_t diff_mean_evol_pjy4  = mean_Rec_evol_pjy4 - mean_Gen_evol_pjy4;
   Double_t diff_mean_evol_pjy5  = mean_Rec_evol_pjy5 - mean_Gen_evol_pjy5;
   Double_t diff_mean_evol_pjy6  = mean_Rec_evol_pjy6 - mean_Gen_evol_pjy6;
   Double_t diff_mean_evol_pjy7  = mean_Rec_evol_pjy7 - mean_Gen_evol_pjy7;
   cout << "diff_mean_evol_pjy1: " << diff_mean_evol_pjy1 << endl; 
   cout << "diff_mean_evol_pjy2: " << diff_mean_evol_pjy2 << endl; 
   cout << "diff_mean_evol_pjy3: " << diff_mean_evol_pjy3 << endl; 
   cout << "diff_mean_evol_pjy4: " << diff_mean_evol_pjy4 << endl; 
   cout << "diff_mean_evol_pjy5: " << diff_mean_evol_pjy5 << endl; 
   cout << "diff_mean_evol_pjy6: " << diff_mean_evol_pjy6 << endl; 
   cout << "diff_mean_evol_pjy7: " << diff_mean_evol_pjy7 << endl; 

   Double_t relDiff_evol_pjy1  = diff_mean_evol_pjy1 / mean_Gen_evol_pjy1;
   Double_t relDiff_evol_pjy2  = diff_mean_evol_pjy2 / mean_Gen_evol_pjy2;
   Double_t relDiff_evol_pjy3  = diff_mean_evol_pjy3 / mean_Gen_evol_pjy3;
   Double_t relDiff_evol_pjy4  = diff_mean_evol_pjy4 / mean_Gen_evol_pjy4;
   Double_t relDiff_evol_pjy5  = diff_mean_evol_pjy5 / mean_Gen_evol_pjy5;
   Double_t relDiff_evol_pjy6  = diff_mean_evol_pjy6 / mean_Gen_evol_pjy6;
   Double_t relDiff_evol_pjy7  = diff_mean_evol_pjy7 / mean_Gen_evol_pjy7;
   cout << "relDiff_evol_pjy1: " << relDiff_evol_pjy1 << endl;  
   cout << "relDiff_evol_pjy2: " << relDiff_evol_pjy2 << endl;  
   cout << "relDiff_evol_pjy3: " << relDiff_evol_pjy3 << endl;  
   cout << "relDiff_evol_pjy4: " << relDiff_evol_pjy4 << endl; 
   cout << "relDiff_evol_pjy5: " << relDiff_evol_pjy5 << endl;  
   cout << "relDiff_evol_pjy6: " << relDiff_evol_pjy6 << endl;  
   cout << "relDiff_evol_pjy7: " << relDiff_evol_pjy7 << endl;


   Double_t rms_Gen_evol_pjy1  = hWm_An_evol_ZK_Vs_PtGen_pjy1  -> GetRMS();
   Double_t rms_Gen_evol_pjy2  = hWm_An_evol_ZK_Vs_PtGen_pjy2  -> GetRMS();
   Double_t rms_Gen_evol_pjy3  = hWm_An_evol_ZK_Vs_PtGen_pjy3  -> GetRMS();
   Double_t rms_Gen_evol_pjy4  = hWm_An_evol_ZK_Vs_PtGen_pjy4  -> GetRMS();
   Double_t rms_Gen_evol_pjy5  = hWm_An_evol_ZK_Vs_PtGen_pjy5  -> GetRMS();
   Double_t rms_Gen_evol_pjy6  = hWm_An_evol_ZK_Vs_PtGen_pjy6  -> GetRMS();
   Double_t rms_Gen_evol_pjy7  = hWm_An_evol_ZK_Vs_PtGen_pjy7  -> GetRMS();
   cout << "rms_Gen_evol_pjy1:  " << rms_Gen_evol_pjy1  << endl; 
   cout << "rms_Gen_evol_pjy2:  " << rms_Gen_evol_pjy2  << endl; 
   cout << "rms_Gen_evol_pjy3:  " << rms_Gen_evol_pjy3  << endl; 
   cout << "rms_Gen_evol_pjy4:  " << rms_Gen_evol_pjy4  << endl; 
   cout << "rms_Gen_evol_pjy5:  " << rms_Gen_evol_pjy5  << endl; 
   cout << "rms_Gen_evol_pjy6:  " << rms_Gen_evol_pjy6  << endl; 
   cout << "rms_Gen_evol_pjy7:  " << rms_Gen_evol_pjy7  << endl;
   Double_t rms_Rec_evol_pjy1  = hWm_An_evol_ZK_Vs_PtRec_pjy1  -> GetRMS();
   Double_t rms_Rec_evol_pjy2  = hWm_An_evol_ZK_Vs_PtRec_pjy2  -> GetRMS();
   Double_t rms_Rec_evol_pjy3  = hWm_An_evol_ZK_Vs_PtRec_pjy3  -> GetRMS();
   Double_t rms_Rec_evol_pjy4  = hWm_An_evol_ZK_Vs_PtRec_pjy4  -> GetRMS();
   Double_t rms_Rec_evol_pjy5  = hWm_An_evol_ZK_Vs_PtRec_pjy5  -> GetRMS();
   Double_t rms_Rec_evol_pjy6  = hWm_An_evol_ZK_Vs_PtRec_pjy6  -> GetRMS();
   Double_t rms_Rec_evol_pjy7  = hWm_An_evol_ZK_Vs_PtRec_pjy7  -> GetRMS();
   cout << "rms_Rec_evol_pjy1:  " << rms_Rec_evol_pjy1  << endl; 
   cout << "rms_Rec_evol_pjy2:  " << rms_Rec_evol_pjy2  << endl; 
   cout << "rms_Rec_evol_pjy3:  " << rms_Rec_evol_pjy3  << endl; 
   cout << "rms_Rec_evol_pjy4:  " << rms_Rec_evol_pjy4  << endl; 
   cout << "rms_Rec_evol_pjy5:  " << rms_Rec_evol_pjy5  << endl; 
   cout << "rms_Rec_evol_pjy6:  " << rms_Rec_evol_pjy6  << endl; 
   cout << "rms_Rec_evol_pjy7:  " << rms_Rec_evol_pjy7  << endl;
   Double_t diff_rms_evol_pjy1  = rms_Rec_evol_pjy1 - rms_Gen_evol_pjy1;
   Double_t diff_rms_evol_pjy2  = rms_Rec_evol_pjy2 - rms_Gen_evol_pjy2;
   Double_t diff_rms_evol_pjy3  = rms_Rec_evol_pjy3 - rms_Gen_evol_pjy3;
   Double_t diff_rms_evol_pjy4  = rms_Rec_evol_pjy4 - rms_Gen_evol_pjy4;
   Double_t diff_rms_evol_pjy5  = rms_Rec_evol_pjy5 - rms_Gen_evol_pjy5;
   Double_t diff_rms_evol_pjy6  = rms_Rec_evol_pjy6 - rms_Gen_evol_pjy6;
   Double_t diff_rms_evol_pjy7  = rms_Rec_evol_pjy7 - rms_Gen_evol_pjy7;
   cout << "diff_rms_evol_pjy1: " << diff_rms_evol_pjy1 << endl; 
   cout << "diff_rms_evol_pjy2: " << diff_rms_evol_pjy2 << endl; 
   cout << "diff_rms_evol_pjy3: " << diff_rms_evol_pjy3 << endl; 
   cout << "diff_rms_evol_pjy4: " << diff_rms_evol_pjy4 << endl; 
   cout << "diff_rms_evol_pjy5: " << diff_rms_evol_pjy5 << endl; 
   cout << "diff_rms_evol_pjy6: " << diff_rms_evol_pjy6 << endl; 
   cout << "diff_rms_evol_pjy7: " << diff_rms_evol_pjy7 << endl;
   Double_t syst_evol_pjy1  = fabs(diff_rms_evol_pjy1) / rms_Gen_evol_pjy1 /2;
   Double_t syst_evol_pjy2  = fabs(diff_rms_evol_pjy2) / rms_Gen_evol_pjy2 /2;
   Double_t syst_evol_pjy3  = fabs(diff_rms_evol_pjy3) / rms_Gen_evol_pjy3 /2;
   Double_t syst_evol_pjy4  = fabs(diff_rms_evol_pjy4) / rms_Gen_evol_pjy4 /2;
   Double_t syst_evol_pjy5  = fabs(diff_rms_evol_pjy5) / rms_Gen_evol_pjy5 /2;
   Double_t syst_evol_pjy6  = fabs(diff_rms_evol_pjy6) / rms_Gen_evol_pjy6 /2;
   Double_t syst_evol_pjy7  = fabs(diff_rms_evol_pjy7) / rms_Gen_evol_pjy7 /2;
   cout << "syst_evol_pjy1: " << syst_evol_pjy1 << endl;  
   cout << "syst_evol_pjy2: " << syst_evol_pjy2 << endl;  
   cout << "syst_evol_pjy3: " << syst_evol_pjy3 << endl;  
   cout << "syst_evol_pjy4: " << syst_evol_pjy4 << endl;  
   cout << "syst_evol_pjy5: " << syst_evol_pjy5 << endl;  
   cout << "syst_evol_pjy6: " << syst_evol_pjy6 << endl;  
   cout << "syst_evol_pjy7: " << syst_evol_pjy7 << endl;


   Double_t xBinsPt[8] = {0.5, 1, 2, 3, 4, 5, 6, 10}; 

   TH1D* hRelativeOffset_evol = new TH1D("hRelativeOffset_evol", "; W P_{T}; Relative difference in the means", 7, xBinsPt);
   hRelativeOffset_evol -> SetBinContent(1, relDiff_evol_pjy1);
   hRelativeOffset_evol -> SetBinContent(2, relDiff_evol_pjy2);
   hRelativeOffset_evol -> SetBinContent(3, relDiff_evol_pjy3);
   hRelativeOffset_evol -> SetBinContent(4, relDiff_evol_pjy4);
   hRelativeOffset_evol -> SetBinContent(5, relDiff_evol_pjy5);
   hRelativeOffset_evol -> SetBinContent(6, relDiff_evol_pjy6);
   hRelativeOffset_evol -> SetBinContent(7, relDiff_evol_pjy7);
   TH1D* hSystematicsMean_evol = new TH1D("hSystematicsMean_evol", "; W P_{T};  systematic uncertainty (%)", 7, xBinsPt);
   hSystematicsMean_evol -> SetBinContent(1, fabs(relDiff_evol_pjy1)*100);
   hSystematicsMean_evol -> SetBinContent(2, fabs(relDiff_evol_pjy2)*100);
   hSystematicsMean_evol -> SetBinContent(3, fabs(relDiff_evol_pjy3)*100);
   hSystematicsMean_evol -> SetBinContent(4, fabs(relDiff_evol_pjy4)*100);
   hSystematicsMean_evol -> SetBinContent(5, fabs(relDiff_evol_pjy5)*100);
   hSystematicsMean_evol -> SetBinContent(6, fabs(relDiff_evol_pjy6)*100);
   hSystematicsMean_evol -> SetBinContent(7, fabs(relDiff_evol_pjy7)*100);

   TH1D* hOffset_evol = new TH1D("hOffset_evol", "; W P_{T}; Difference in the means", 7, xBinsPt);
   hOffset_evol -> SetBinContent(1, diff_mean_evol_pjy1);
   hOffset_evol -> SetBinContent(2, diff_mean_evol_pjy2);
   hOffset_evol -> SetBinContent(3, diff_mean_evol_pjy3);
   hOffset_evol -> SetBinContent(4, diff_mean_evol_pjy4);
   hOffset_evol -> SetBinContent(5, diff_mean_evol_pjy5);
   hOffset_evol -> SetBinContent(6, diff_mean_evol_pjy6);
   hOffset_evol -> SetBinContent(7, diff_mean_evol_pjy7);


   TH1D* hSystematics_evol = new TH1D("hSystematics_evol", "; W P_{T}; systematic uncertainty (%)", 7, xBinsPt);
   hSystematics_evol -> SetBinContent(1, syst_evol_pjy1*100);
   hSystematics_evol -> SetBinContent(2, syst_evol_pjy2*100);
   hSystematics_evol -> SetBinContent(3, syst_evol_pjy3*100);
   hSystematics_evol -> SetBinContent(4, syst_evol_pjy4*100);
   hSystematics_evol -> SetBinContent(5, syst_evol_pjy5*100);
   hSystematics_evol -> SetBinContent(6, syst_evol_pjy6*100);
   hSystematics_evol -> SetBinContent(7, syst_evol_pjy7*100);

   TCanvas *c8   = new TCanvas("c8", "Systematics evol.", 500, 500);
   c8->cd();
   hSystematics_evol -> GetYaxis() -> SetTitleOffset(1.5);
   hSystematics_evol -> SetMarkerStyle(20);
   hSystematics_evol -> SetMarkerColor(kRed);
   hSystematics_evol -> SetStats(0);
   hSystematics_evol -> Draw("P");

   TFile fileSys("histSysPt.root", "recreate"); 
   hSystematics_evol -> Write();

   c8->SaveAs(outPath + "/plot_systematics_evol.png");
   c8->SaveAs(outPath + "/plot_systematics_evol.eps");


   TCanvas *cSysMeanPt   = new TCanvas("cSysMeanPt", "Systematics evol.vs Pt from Means", 500, 500);
   cSysMeanPt->cd();
   hSystematicsMean_evol  -> GetYaxis() -> SetTitleOffset(1.5);
   hSystematicsMean_evol  -> SetMarkerStyle(20);
   hSystematicsMean_evol  -> SetMarkerColor(kRed);
   hSystematicsMean_evol  -> SetStats(0);
   hSystematicsMean_evol  -> Draw("P");

   TFile fileMeanSys("histMeanSysPt.root", "recreate"); 
   hSystematicsMean_evol -> Write(); 

   cSysMeanPt->SaveAs(outPath + "/plot_systematicsMean_evol.png");
   cSysMeanPt->SaveAs(outPath + "/plot_systematicsMean_evol.eps");


   TCanvas *cOffsetPt   = new TCanvas("cOffsetPt", "Offset Vs Pt", 500, 500);

   cOffsetPt -> Divide(1,2);

   cOffsetPt_1->cd();
   hOffset_evol -> GetYaxis() -> SetTitleOffset(1.2);
   hOffset_evol -> SetMarkerStyle(20);
   hOffset_evol -> SetMarkerColor(kRed);
   hOffset_evol -> SetStats(0);
   hOffset_evol -> Draw("P");
   cOffsetPt_2->cd();
   hRelativeOffset_evol -> GetYaxis() -> SetTitleOffset(1.2);
   hRelativeOffset_evol -> SetMarkerStyle(20);
   hRelativeOffset_evol -> SetMarkerColor(kRed);
   hRelativeOffset_evol -> SetStats(0);
   hRelativeOffset_evol -> Draw("P");

   cOffsetPt->SaveAs(outPath + "/plot_Offset_evol_Pt.png");
   cOffsetPt->SaveAs(outPath + "/plot_Offset_evol_Pt.eps");
}


void CalcSyst_noevo()
{
   Double_t rms_Gen_noevo_pjy1  = hWm_An_noevo_ZK_Vs_PtGen_pjy1  -> GetRMS();
   Double_t rms_Gen_noevo_pjy2  = hWm_An_noevo_ZK_Vs_PtGen_pjy2  -> GetRMS();
   Double_t rms_Gen_noevo_pjy3  = hWm_An_noevo_ZK_Vs_PtGen_pjy3  -> GetRMS();
   Double_t rms_Gen_noevo_pjy4  = hWm_An_noevo_ZK_Vs_PtGen_pjy4  -> GetRMS();
   Double_t rms_Gen_noevo_pjy5  = hWm_An_noevo_ZK_Vs_PtGen_pjy5  -> GetRMS();
   Double_t rms_Gen_noevo_pjy6  = hWm_An_noevo_ZK_Vs_PtGen_pjy6  -> GetRMS();
   Double_t rms_Gen_noevo_pjy7  = hWm_An_noevo_ZK_Vs_PtGen_pjy7  -> GetRMS();
   Double_t rms_Gen_noevo_pjy8  = hWm_An_noevo_ZK_Vs_PtGen_pjy8  -> GetRMS();
   Double_t rms_Gen_noevo_pjy9  = hWm_An_noevo_ZK_Vs_PtGen_pjy9  -> GetRMS();
   Double_t rms_Gen_noevo_pjy10 = hWm_An_noevo_ZK_Vs_PtGen_pjy10 -> GetRMS();
   Double_t rms_Gen_noevo_pjy11 = hWm_An_noevo_ZK_Vs_PtGen_pjy11 -> GetRMS();
   Double_t rms_Gen_noevo_pjy12 = hWm_An_noevo_ZK_Vs_PtGen_pjy12 -> GetRMS();
   Double_t rms_Gen_noevo_pjy13 = hWm_An_noevo_ZK_Vs_PtGen_pjy13 -> GetRMS();
   Double_t rms_Gen_noevo_pjy14 = hWm_An_noevo_ZK_Vs_PtGen_pjy14 -> GetRMS();
   Double_t rms_Gen_noevo_pjy15 = hWm_An_noevo_ZK_Vs_PtGen_pjy15 -> GetRMS();
   cout << "rms_Gen_noevo_pjy1:  " << rms_Gen_noevo_pjy1  << endl; 
   cout << "rms_Gen_noevo_pjy2:  " << rms_Gen_noevo_pjy2  << endl; 
   cout << "rms_Gen_noevo_pjy3:  " << rms_Gen_noevo_pjy3  << endl; 
   cout << "rms_Gen_noevo_pjy4:  " << rms_Gen_noevo_pjy4  << endl; 
   cout << "rms_Gen_noevo_pjy5:  " << rms_Gen_noevo_pjy5  << endl; 
   cout << "rms_Gen_noevo_pjy6:  " << rms_Gen_noevo_pjy6  << endl; 
   cout << "rms_Gen_noevo_pjy7:  " << rms_Gen_noevo_pjy7  << endl; 
   cout << "rms_Gen_noevo_pjy8:  " << rms_Gen_noevo_pjy8  << endl; 
   cout << "rms_Gen_noevo_pjy9:  " << rms_Gen_noevo_pjy9  << endl; 
   cout << "rms_Gen_noevo_pjy10: " << rms_Gen_noevo_pjy10 << endl; 
   cout << "rms_Gen_noevo_pjy11: " << rms_Gen_noevo_pjy11 << endl; 
   cout << "rms_Gen_noevo_pjy12: " << rms_Gen_noevo_pjy12 << endl; 
   cout << "rms_Gen_noevo_pjy13: " << rms_Gen_noevo_pjy13 << endl; 
   cout << "rms_Gen_noevo_pjy14: " << rms_Gen_noevo_pjy14 << endl;  
   cout << "rms_Gen_noevo_pjy15: " << rms_Gen_noevo_pjy15 << endl;
   Double_t rms_Rec_noevo_pjy1  = hWm_An_noevo_ZK_Vs_PtRec_pjy1  -> GetRMS();
   Double_t rms_Rec_noevo_pjy2  = hWm_An_noevo_ZK_Vs_PtRec_pjy2  -> GetRMS();
   Double_t rms_Rec_noevo_pjy3  = hWm_An_noevo_ZK_Vs_PtRec_pjy3  -> GetRMS();
   Double_t rms_Rec_noevo_pjy4  = hWm_An_noevo_ZK_Vs_PtRec_pjy4  -> GetRMS();
   Double_t rms_Rec_noevo_pjy5  = hWm_An_noevo_ZK_Vs_PtRec_pjy5  -> GetRMS();
   Double_t rms_Rec_noevo_pjy6  = hWm_An_noevo_ZK_Vs_PtRec_pjy6  -> GetRMS();
   Double_t rms_Rec_noevo_pjy7  = hWm_An_noevo_ZK_Vs_PtRec_pjy7  -> GetRMS();
   Double_t rms_Rec_noevo_pjy8  = hWm_An_noevo_ZK_Vs_PtRec_pjy8  -> GetRMS();
   Double_t rms_Rec_noevo_pjy9  = hWm_An_noevo_ZK_Vs_PtRec_pjy9  -> GetRMS();
   Double_t rms_Rec_noevo_pjy10 = hWm_An_noevo_ZK_Vs_PtRec_pjy10 -> GetRMS();
   Double_t rms_Rec_noevo_pjy11 = hWm_An_noevo_ZK_Vs_PtRec_pjy11 -> GetRMS();
   Double_t rms_Rec_noevo_pjy12 = hWm_An_noevo_ZK_Vs_PtRec_pjy12 -> GetRMS();
   Double_t rms_Rec_noevo_pjy13 = hWm_An_noevo_ZK_Vs_PtRec_pjy13 -> GetRMS();
   Double_t rms_Rec_noevo_pjy14 = hWm_An_noevo_ZK_Vs_PtRec_pjy14 -> GetRMS();
   Double_t rms_Rec_noevo_pjy15 = hWm_An_noevo_ZK_Vs_PtRec_pjy15 -> GetRMS();
   cout << "rms_Rec_noevo_pjy1:  " << rms_Rec_noevo_pjy1  << endl; 
   cout << "rms_Rec_noevo_pjy2:  " << rms_Rec_noevo_pjy2  << endl; 
   cout << "rms_Rec_noevo_pjy3:  " << rms_Rec_noevo_pjy3  << endl; 
   cout << "rms_Rec_noevo_pjy4:  " << rms_Rec_noevo_pjy4  << endl; 
   cout << "rms_Rec_noevo_pjy5:  " << rms_Rec_noevo_pjy5  << endl; 
   cout << "rms_Rec_noevo_pjy6:  " << rms_Rec_noevo_pjy6  << endl; 
   cout << "rms_Rec_noevo_pjy7:  " << rms_Rec_noevo_pjy7  << endl; 
   cout << "rms_Rec_noevo_pjy8:  " << rms_Rec_noevo_pjy8  << endl; 
   cout << "rms_Rec_noevo_pjy9:  " << rms_Rec_noevo_pjy9  << endl; 
   cout << "rms_Rec_noevo_pjy10: " << rms_Rec_noevo_pjy10 << endl; 
   cout << "rms_Rec_noevo_pjy11: " << rms_Rec_noevo_pjy11 << endl; 
   cout << "rms_Rec_noevo_pjy12: " << rms_Rec_noevo_pjy12 << endl; 
   cout << "rms_Rec_noevo_pjy13: " << rms_Rec_noevo_pjy13 << endl; 
   cout << "rms_Rec_noevo_pjy14: " << rms_Rec_noevo_pjy14 << endl;  
   cout << "rms_Rec_noevo_pjy15: " << rms_Rec_noevo_pjy15 << endl; 

   Double_t diff_rms_noevo_pjy1  = rms_Rec_noevo_pjy1 - rms_Gen_noevo_pjy1;
   Double_t diff_rms_noevo_pjy2  = rms_Rec_noevo_pjy2 - rms_Gen_noevo_pjy2;
   Double_t diff_rms_noevo_pjy3  = rms_Rec_noevo_pjy3 - rms_Gen_noevo_pjy3;
   Double_t diff_rms_noevo_pjy4  = rms_Rec_noevo_pjy4 - rms_Gen_noevo_pjy4;
   Double_t diff_rms_noevo_pjy5  = rms_Rec_noevo_pjy5 - rms_Gen_noevo_pjy5;
   Double_t diff_rms_noevo_pjy6  = rms_Rec_noevo_pjy6 - rms_Gen_noevo_pjy6;
   Double_t diff_rms_noevo_pjy7  = rms_Rec_noevo_pjy7 - rms_Gen_noevo_pjy7;
   Double_t diff_rms_noevo_pjy8  = rms_Rec_noevo_pjy8 - rms_Gen_noevo_pjy8;
   Double_t diff_rms_noevo_pjy9  = rms_Rec_noevo_pjy9 - rms_Gen_noevo_pjy9;
   Double_t diff_rms_noevo_pjy10 = rms_Rec_noevo_pjy10 - rms_Gen_noevo_pjy10;
   Double_t diff_rms_noevo_pjy11 = rms_Rec_noevo_pjy11 - rms_Gen_noevo_pjy11;
   Double_t diff_rms_noevo_pjy12 = rms_Rec_noevo_pjy12 - rms_Gen_noevo_pjy12;
   Double_t diff_rms_noevo_pjy13 = rms_Rec_noevo_pjy13 - rms_Gen_noevo_pjy13;
   Double_t diff_rms_noevo_pjy14 = rms_Rec_noevo_pjy14 - rms_Gen_noevo_pjy14;
   Double_t diff_rms_noevo_pjy15 = rms_Rec_noevo_pjy15 - rms_Gen_noevo_pjy15;
   cout << "diff_rms_noevo_pjy1: " << diff_rms_noevo_pjy1 << endl; 
   cout << "diff_rms_noevo_pjy2: " << diff_rms_noevo_pjy2 << endl; 
   cout << "diff_rms_noevo_pjy3: " << diff_rms_noevo_pjy3 << endl; 
   cout << "diff_rms_noevo_pjy4: " << diff_rms_noevo_pjy4 << endl; 
   cout << "diff_rms_noevo_pjy5: " << diff_rms_noevo_pjy5 << endl; 
   cout << "diff_rms_noevo_pjy6: " << diff_rms_noevo_pjy6 << endl; 
   cout << "diff_rms_noevo_pjy7: " << diff_rms_noevo_pjy7 << endl; 
   cout << "diff_rms_noevo_pjy8: " << diff_rms_noevo_pjy8 << endl; 
   cout << "diff_rms_noevo_pjy9: " << diff_rms_noevo_pjy9 << endl; 
   cout << "diff_rms_noevo_pjy10: " << diff_rms_noevo_pjy10 << endl; 
   cout << "diff_rms_noevo_pjy11: " << diff_rms_noevo_pjy11 << endl; 
   cout << "diff_rms_noevo_pjy12: " << diff_rms_noevo_pjy12 << endl; 
   cout << "diff_rms_noevo_pjy13: " << diff_rms_noevo_pjy13 << endl; 
   cout << "diff_rms_noevo_pjy14: " << diff_rms_noevo_pjy14 << endl; 
   cout << "diff_rms_noevo_pjy15: " << diff_rms_noevo_pjy15 << endl; 

   Double_t syst_noevo_pjy1  = fabs(diff_rms_noevo_pjy1) / rms_Gen_noevo_pjy1;
   Double_t syst_noevo_pjy2  = fabs(diff_rms_noevo_pjy2) / rms_Gen_noevo_pjy2;
   Double_t syst_noevo_pjy3  = fabs(diff_rms_noevo_pjy3) / rms_Gen_noevo_pjy3;
   Double_t syst_noevo_pjy4  = fabs(diff_rms_noevo_pjy4) / rms_Gen_noevo_pjy4;
   Double_t syst_noevo_pjy5  = fabs(diff_rms_noevo_pjy5) / rms_Gen_noevo_pjy5;
   Double_t syst_noevo_pjy6  = fabs(diff_rms_noevo_pjy6) / rms_Gen_noevo_pjy6;
   Double_t syst_noevo_pjy7  = fabs(diff_rms_noevo_pjy7) / rms_Gen_noevo_pjy7;
   Double_t syst_noevo_pjy8  = fabs(diff_rms_noevo_pjy8) / rms_Gen_noevo_pjy8;
   Double_t syst_noevo_pjy9  = fabs(diff_rms_noevo_pjy9) / rms_Gen_noevo_pjy9;
   Double_t syst_noevo_pjy10 = fabs(diff_rms_noevo_pjy10) / rms_Gen_noevo_pjy10;
   Double_t syst_noevo_pjy11 = fabs(diff_rms_noevo_pjy11) / rms_Gen_noevo_pjy11;
   Double_t syst_noevo_pjy12 = fabs(diff_rms_noevo_pjy12) / rms_Gen_noevo_pjy12;
   Double_t syst_noevo_pjy13 = fabs(diff_rms_noevo_pjy13) / rms_Gen_noevo_pjy13;
   Double_t syst_noevo_pjy14 = fabs(diff_rms_noevo_pjy14) / rms_Gen_noevo_pjy14;
   Double_t syst_noevo_pjy15 = fabs(diff_rms_noevo_pjy15) / rms_Gen_noevo_pjy15; 
   cout << "syst_noevo_pjy1: " << syst_noevo_pjy1 << endl;  
   cout << "syst_noevo_pjy2: " << syst_noevo_pjy2 << endl;  
   cout << "syst_noevo_pjy3: " << syst_noevo_pjy3 << endl;  
   cout << "syst_noevo_pjy4: " << syst_noevo_pjy4 << endl;  
   cout << "syst_noevo_pjy5: " << syst_noevo_pjy5 << endl;  
   cout << "syst_noevo_pjy6: " << syst_noevo_pjy6 << endl;  
   cout << "syst_noevo_pjy7: " << syst_noevo_pjy7 << endl;  
   cout << "syst_noevo_pjy8: " << syst_noevo_pjy8 << endl;  
   cout << "syst_noevo_pjy9: " << syst_noevo_pjy9 << endl;  
   cout << "syst_noevo_pjy10: " << syst_noevo_pjy10 << endl;  
   cout << "syst_noevo_pjy11: " << syst_noevo_pjy11 << endl;  
   cout << "syst_noevo_pjy12: " << syst_noevo_pjy12 << endl;  
   cout << "syst_noevo_pjy13: " << syst_noevo_pjy13 << endl;  
   cout << "syst_noevo_pjy14: " << syst_noevo_pjy14 << endl;  
   cout << "syst_noevo_pjy15: " << syst_noevo_pjy15 << endl; 
}



void Print_Projections_evol_Rapidity(TString outPath)
{

   TCanvas *c9a   = new TCanvas("c9a", "Wm An prediction with evol. Vs Rapidity", 800, 800);

   c9a-> Divide(2,2);
   c9a_1->cd();
   hWm_An_evol_ZK_Vs_RapGen_pjy1 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_RapGen_pjy1 -> SetTitle("RapGen BIN 1: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_RapGen_pjy1 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_RapGen_pjy1 -> SetAxisRange(0, 0.1, "X");
   hWm_An_evol_ZK_Vs_RapGen_pjy1 -> Rebin(2);
   hWm_An_evol_ZK_Vs_RapGen_pjy1 -> Fit("gaus");
   hWm_An_evol_ZK_Vs_RapGen_pjy1 -> GetXaxis() -> SetNdivisions(4);
   hWm_An_evol_ZK_Vs_RapGen_pjy1 -> Draw();
   c9a_2->cd();
   hWm_An_evol_ZK_Vs_RapGen_pjy2 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_RapGen_pjy2 ->SetTitle("RapGen BIN 2: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_RapGen_pjy2 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_RapGen_pjy2 -> SetAxisRange(0, 0.1, "X");
   hWm_An_evol_ZK_Vs_RapGen_pjy2 -> Rebin(2);
   hWm_An_evol_ZK_Vs_RapGen_pjy2 -> Fit("gaus");
   hWm_An_evol_ZK_Vs_RapGen_pjy2 -> GetXaxis() -> SetNdivisions(4);
   hWm_An_evol_ZK_Vs_RapGen_pjy2 -> Draw();
   c9a_3->cd();
   hWm_An_evol_ZK_Vs_RapGen_pjy3 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_RapGen_pjy3 ->SetTitle("RapGen BIN 3: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_RapGen_pjy3 -> SetFillColor(kYellow); 
   //hWm_An_evol_ZK_Vs_RapGen_pjy3 -> SetAxisRange(0, 0.1, "X");
   hWm_An_evol_ZK_Vs_RapGen_pjy3 -> Rebin(2);
   //hWm_An_evol_ZK_Vs_RapGen_pjy3 -> Fit("gaus", "", "", 0, 0.03);
   hWm_An_evol_ZK_Vs_RapGen_pjy3 -> Fit("gaus");
   hWm_An_evol_ZK_Vs_RapGen_pjy3 -> GetXaxis() -> SetNdivisions(4);
   hWm_An_evol_ZK_Vs_RapGen_pjy3 -> Draw();
   c9a_4->cd();

   c9a->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Vs_RapGen_projs.png");
   c9a->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Vs_RapGen_projs.eps");


   TCanvas *c9b   = new TCanvas("c9b", "Wm An prediction with evol. Vs Rapidity", 800, 800);

   c9b-> Divide(2,2);
   c9b_1->cd();
   hWm_An_evol_ZK_Vs_RapRec_pjy1 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_RapRec_pjy1 -> SetTitle("RapRec BIN 1: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_RapRec_pjy1 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_RapRec_pjy1 -> SetAxisRange(0, 0.1, "X");
   hWm_An_evol_ZK_Vs_RapRec_pjy1 -> Rebin(2);
   //hWm_An_evol_ZK_Vs_RapRec_pjy1 -> Fit("gaus", "", "", 0, 0.015);
   hWm_An_evol_ZK_Vs_RapRec_pjy1 -> Fit("gaus");
   hWm_An_evol_ZK_Vs_RapRec_pjy1 -> GetXaxis() -> SetNdivisions(4);
   hWm_An_evol_ZK_Vs_RapRec_pjy1 -> LabelsOption("v", "X");
   hWm_An_evol_ZK_Vs_RapRec_pjy1 -> Draw(""); 
   c9b_2->cd();
   hWm_An_evol_ZK_Vs_RapRec_pjy2 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_RapRec_pjy2 -> SetTitle("RapRec BIN 2: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_RapRec_pjy2 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_RapRec_pjy2 -> SetAxisRange(0, 0.1, "X");
   hWm_An_evol_ZK_Vs_RapRec_pjy2 -> Rebin(2);
   //hWm_An_evol_ZK_Vs_RapRec_pjy2 -> Fit("gaus", "", "", 0, 0.018);
   hWm_An_evol_ZK_Vs_RapRec_pjy2 -> Fit("gaus");
   hWm_An_evol_ZK_Vs_RapRec_pjy2 -> GetXaxis() -> SetNdivisions(4);
   hWm_An_evol_ZK_Vs_RapRec_pjy2 -> Draw();
   c9b_3->cd();
   hWm_An_evol_ZK_Vs_RapRec_pjy3 -> GetXaxis() -> SetTitleOffset(1.2);
   hWm_An_evol_ZK_Vs_RapRec_pjy3 -> SetTitle("RapRec BIN 3: with evolution; W- A_{N}");
   hWm_An_evol_ZK_Vs_RapRec_pjy3 -> SetFillColor(kYellow);
   //hWm_An_evol_ZK_Vs_RapRec_pjy3 -> SetAxisRange(0, 0.1, "X");
   hWm_An_evol_ZK_Vs_RapRec_pjy3 -> Rebin(2);
   //hWm_An_evol_ZK_Vs_RapRec_pjy3 -> Fit("gaus", "", "", 0, 0.030);
   hWm_An_evol_ZK_Vs_RapRec_pjy3 -> Fit("gaus");
   hWm_An_evol_ZK_Vs_RapRec_pjy3 -> GetXaxis() -> SetNdivisions(4);
   hWm_An_evol_ZK_Vs_RapRec_pjy3 -> Draw();

   c9b->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Vs_RapRec_projs.png");
   c9b->SaveAs(outPath + "/plot_Wm_An_evol_ZK_Vs_RapRec_projs.eps");
}



void CalcSyst_evol_Rapidity(TString outPath)
{
  /*
   Double_t rms_Gen_evol_pjy1  = hWm_An_evol_ZK_Vs_RapGen_pjy1  -> GetRMS();
   Double_t rms_Gen_evol_pjy2  = hWm_An_evol_ZK_Vs_RapGen_pjy2  -> GetRMS();
   Double_t rms_Gen_evol_pjy3  = hWm_An_evol_ZK_Vs_RapGen_pjy3  -> GetRMS();
   Double_t rms_Gen_evol_pjy4  = hWm_An_evol_ZK_Vs_RapGen_pjy4  -> GetRMS();
   cout << "rms_Gen_evol_pjy1:  " << rms_Gen_evol_pjy1  << endl; 
   cout << "rms_Gen_evol_pjy2:  " << rms_Gen_evol_pjy2  << endl; 
   cout << "rms_Gen_evol_pjy3:  " << rms_Gen_evol_pjy3  << endl; 
   cout << "rms_Gen_evol_pjy4:  " << rms_Gen_evol_pjy4  << endl;
   Double_t rms_Rec_evol_pjy1  = hWm_An_evol_ZK_Vs_RapRec_pjy1  -> GetRMS();
   Double_t rms_Rec_evol_pjy2  = hWm_An_evol_ZK_Vs_RapRec_pjy2  -> GetRMS();
   Double_t rms_Rec_evol_pjy3  = hWm_An_evol_ZK_Vs_RapRec_pjy3  -> GetRMS();
   Double_t rms_Rec_evol_pjy4  = hWm_An_evol_ZK_Vs_RapRec_pjy4  -> GetRMS();
   cout << "rms_Rec_evol_pjy1:  " << rms_Rec_evol_pjy1  << endl; 
   cout << "rms_Rec_evol_pjy2:  " << rms_Rec_evol_pjy2  << endl; 
   cout << "rms_Rec_evol_pjy3:  " << rms_Rec_evol_pjy3  << endl; 
   cout << "rms_Rec_evol_pjy4:  " << rms_Rec_evol_pjy4  << endl; 

   Double_t diff_rms_evol_pjy1  = rms_Rec_evol_pjy1 - rms_Gen_evol_pjy1;
   Double_t diff_rms_evol_pjy2  = rms_Rec_evol_pjy2 - rms_Gen_evol_pjy2;
   Double_t diff_rms_evol_pjy3  = rms_Rec_evol_pjy3 - rms_Gen_evol_pjy3;
   Double_t diff_rms_evol_pjy4  = rms_Rec_evol_pjy4 - rms_Gen_evol_pjy4;
   cout << "diff_rms_evol_pjy1: " << diff_rms_evol_pjy1 << endl; 
   cout << "diff_rms_evol_pjy2: " << diff_rms_evol_pjy2 << endl; 
   cout << "diff_rms_evol_pjy3: " << diff_rms_evol_pjy3 << endl; 
   cout << "diff_rms_evol_pjy4: " << diff_rms_evol_pjy4 << endl; 

   Double_t syst_evol_pjy1  = fabs(diff_rms_evol_pjy1) / rms_Gen_evol_pjy1;
   Double_t syst_evol_pjy2  = fabs(diff_rms_evol_pjy2) / rms_Gen_evol_pjy2;
   Double_t syst_evol_pjy3  = fabs(diff_rms_evol_pjy3) / rms_Gen_evol_pjy3;
   Double_t syst_evol_pjy4  = fabs(diff_rms_evol_pjy4) / rms_Gen_evol_pjy4;
   cout << "syst_evol_pjy1: " << syst_evol_pjy1 << endl;  
   cout << "syst_evol_pjy2: " << syst_evol_pjy2 << endl;  
   cout << "syst_evol_pjy3: " << syst_evol_pjy3 << endl;  
   cout << "syst_evol_pjy4: " << syst_evol_pjy4 << endl; 
   */ 
   Double_t mean_Gen_evol_pjy1  = hWm_An_evol_ZK_Vs_RapGen_pjy1  -> GetMean();
   Double_t mean_Gen_evol_pjy2  = hWm_An_evol_ZK_Vs_RapGen_pjy2  -> GetMean();
   Double_t mean_Gen_evol_pjy3  = hWm_An_evol_ZK_Vs_RapGen_pjy3  -> GetMean();
   //Double_t mean_Gen_evol_pjy4  = hWm_An_evol_ZK_Vs_RapGen_pjy4  -> GetMean(); 

   Double_t mean_Rec_evol_pjy1  = hWm_An_evol_ZK_Vs_RapRec_pjy1  -> GetMean();
   Double_t mean_Rec_evol_pjy2  = hWm_An_evol_ZK_Vs_RapRec_pjy2  -> GetMean();
   Double_t mean_Rec_evol_pjy3  = hWm_An_evol_ZK_Vs_RapRec_pjy3  -> GetMean();
   //Double_t mean_Rec_evol_pjy4  = hWm_An_evol_ZK_Vs_RapRec_pjy4  -> GetMean(); 

   cout << "Rpidity: mean_Gen_evol_pjy1:  " << mean_Gen_evol_pjy1  << endl; 
   cout << "Rpidity: mean_Gen_evol_pjy2:  " << mean_Gen_evol_pjy2  << endl; 
   cout << "Rpidity: mean_Gen_evol_pjy3:  " << mean_Gen_evol_pjy3  << endl; 
   //cout << "Rpidity: mean_Gen_evol_pjy4:  " << mean_Gen_evol_pjy4  << endl;

   cout << "Rpidity: mean_Rec_evol_pjy1:  " << mean_Rec_evol_pjy1  << endl; 
   cout << "Rpidity: mean_Rec_evol_pjy2:  " << mean_Rec_evol_pjy2  << endl; 
   cout << "Rpidity: mean_Rec_evol_pjy3:  " << mean_Rec_evol_pjy3  << endl; 
   //cout << "Rpidity: mean_Rec_evol_pjy4:  " << mean_Rec_evol_pjy4  << endl;

   Double_t diff_mean_evol_pjy1  = mean_Rec_evol_pjy1 - mean_Gen_evol_pjy1;
   Double_t diff_mean_evol_pjy2  = mean_Rec_evol_pjy2 - mean_Gen_evol_pjy2;
   Double_t diff_mean_evol_pjy3  = mean_Rec_evol_pjy3 - mean_Gen_evol_pjy3;
   //Double_t diff_mean_evol_pjy4  = mean_Rec_evol_pjy4 - mean_Gen_evol_pjy4;
   cout << "Rpidity: diff_mean_evol_pjy1: " << diff_mean_evol_pjy1 << endl; 
   cout << "Rpidity: diff_mean_evol_pjy2: " << diff_mean_evol_pjy2 << endl; 
   cout << "Rpidity: diff_mean_evol_pjy3: " << diff_mean_evol_pjy3 << endl; 
   //cout << "Rpidity: diff_mean_evol_pjy4: " << diff_mean_evol_pjy4 << endl; 

   Double_t relDiff_evol_pjy1  = diff_mean_evol_pjy1 / mean_Gen_evol_pjy1;
   Double_t relDiff_evol_pjy2  = diff_mean_evol_pjy2 / mean_Gen_evol_pjy2;
   Double_t relDiff_evol_pjy3  = diff_mean_evol_pjy3 / mean_Gen_evol_pjy3;
   //Double_t relDiff_evol_pjy4  = diff_mean_evol_pjy4 / mean_Gen_evol_pjy4;
   cout << "Rpidity: relDiff_evol_pjy1: " << relDiff_evol_pjy1 << endl;  
   cout << "Rpidity: relDiff_evol_pjy2: " << relDiff_evol_pjy2 << endl;  
   cout << "Rpidity: relDiff_evol_pjy3: " << relDiff_evol_pjy3 << endl;  
   //cout << "Rpidity: relDiff_evol_pjy4: " << relDiff_evol_pjy4 << endl; 


   //Double_t xBinsRap[5] = {-0.6, -0.25, 0, 0.25, 0.6};
   static const int RapBins = 3;
   Double_t xBinsRap[RapBins+1];
   if (RapBins == 4) {
     xBinsRap[0] = -0.6;
     xBinsRap[1] = -0.25;
     xBinsRap[2] = 0;
     xBinsRap[3] = 0.25;
     xBinsRap[4] = 0.6;
   } else if (RapBins == 3){
     xBinsRap[0] = -0.6;
     xBinsRap[1] = -0.2;
     xBinsRap[2] = 0.2;
     xBinsRap[3] = 0.6;
   }

   TH1D* hRelativeOffset_evol = new TH1D("hRelativeOffset_evol", "; y; Relative difference in the means", RapBins, xBinsRap);
   hRelativeOffset_evol -> SetBinContent(1, relDiff_evol_pjy1);
   hRelativeOffset_evol -> SetBinContent(2, relDiff_evol_pjy2);
   hRelativeOffset_evol -> SetBinContent(3, relDiff_evol_pjy3);
   //hRelativeOffset_evol -> SetBinContent(4, relDiff_evol_pjy4);
  
   TH1D* hOffset_evol = new TH1D("hOffset_evol", "; y; Difference in the means", RapBins, xBinsRap);
   hOffset_evol -> SetBinContent(1, diff_mean_evol_pjy1);
   hOffset_evol -> SetBinContent(2, diff_mean_evol_pjy2);
   hOffset_evol -> SetBinContent(3, diff_mean_evol_pjy3);
   //hOffset_evol -> SetBinContent(4, diff_mean_evol_pjy4);

   TF1 *f1g = hWm_An_evol_ZK_Vs_RapGen_pjy1->GetFunction("gaus");
   double gmean_Gen_evol_pjy1 = f1g->GetParameter(1);
   double sigma_Gen_evol_pjy1 = f1g->GetParameter(2);
   TF1 *f2g = hWm_An_evol_ZK_Vs_RapGen_pjy2->GetFunction("gaus");
   double gmean_Gen_evol_pjy2 = f2g->GetParameter(1);
   double sigma_Gen_evol_pjy2 = f2g->GetParameter(2);
   TF1 *f3g = hWm_An_evol_ZK_Vs_RapGen_pjy3->GetFunction("gaus");
   double gmean_Gen_evol_pjy3 = f3g->GetParameter(1);
   double sigma_Gen_evol_pjy3 = f3g->GetParameter(2);
   //TF1 *f4g = hWm_An_evol_ZK_Vs_RapGen_pjy4->GetFunction("gaus");
   //double sigma_Gen_evol_pjy4 = f4g->GetParameter(2);
   cout << "Rpidity: gmean_Gen_evol_pjy1:  " << gmean_Gen_evol_pjy1  << endl; 
   cout << "Rpidity: gmean_Gen_evol_pjy2:  " << gmean_Gen_evol_pjy2  << endl; 
   cout << "Rpidity: gmean_Gen_evol_pjy3:  " << gmean_Gen_evol_pjy3  << endl; 
   cout << "Rpidity: sigma_Gen_evol_pjy1:  " << sigma_Gen_evol_pjy1  << endl; 
   cout << "Rpidity: sigma_Gen_evol_pjy2:  " << sigma_Gen_evol_pjy2  << endl; 
   cout << "Rpidity: sigma_Gen_evol_pjy3:  " << sigma_Gen_evol_pjy3  << endl; 
   //cout << "Rpidity: sigma_Gen_evol_pjy4:  " << sigma_Gen_evol_pjy4  << endl;
   TF1 *f1r = hWm_An_evol_ZK_Vs_RapRec_pjy1->GetFunction("gaus");
   double gmean_Rec_evol_pjy1 = f1r->GetParameter(1);
   double sigma_Rec_evol_pjy1 = f1r->GetParameter(2);
   TF1 *f2r = hWm_An_evol_ZK_Vs_RapRec_pjy2->GetFunction("gaus");
   double gmean_Rec_evol_pjy2 = f2r->GetParameter(1);
   double sigma_Rec_evol_pjy2 = f2r->GetParameter(2);
   TF1 *f3r = hWm_An_evol_ZK_Vs_RapRec_pjy3->GetFunction("gaus");
   double gmean_Rec_evol_pjy3 = f3r->GetParameter(1);
   double sigma_Rec_evol_pjy3 = f3r->GetParameter(2);
   //TF1 *f4r = hWm_An_evol_ZK_Vs_RapRec_pjy4->GetFunction("gaus");
   //double sigma_Rec_evol_pjy4 = f4r->GetParameter(2);
   cout << "Rpidity: gmean_Rec_evol_pjy1:  " << gmean_Rec_evol_pjy1  << endl;
   cout << "Rpidity: gmean_Rec_evol_pjy2:  " << gmean_Rec_evol_pjy2  << endl;
   cout << "Rpidity: gmean_Rec_evol_pjy3:  " << gmean_Rec_evol_pjy3  << endl;
   cout << "Rpidity: sigma_Rec_evol_pjy1:  " << sigma_Rec_evol_pjy1  << endl; 
   cout << "Rpidity: sigma_Rec_evol_pjy2:  " << sigma_Rec_evol_pjy2  << endl; 
   cout << "Rpidity: sigma_Rec_evol_pjy3:  " << sigma_Rec_evol_pjy3  << endl; 
   //cout << "Rpidity: sigma_Rec_evol_pjy4:  " << sigma_Rec_evol_pjy4  << endl; 

   Double_t diff_gmean_evol_pjy1  = gmean_Rec_evol_pjy1 - gmean_Gen_evol_pjy1;
   Double_t diff_gmean_evol_pjy2  = gmean_Rec_evol_pjy2 - gmean_Gen_evol_pjy2;
   Double_t diff_gmean_evol_pjy3  = gmean_Rec_evol_pjy3 - gmean_Gen_evol_pjy3;
   Double_t diff_sigma_evol_pjy1  = sigma_Rec_evol_pjy1 - sigma_Gen_evol_pjy1;
   Double_t diff_sigma_evol_pjy2  = sigma_Rec_evol_pjy2 - sigma_Gen_evol_pjy2;
   Double_t diff_sigma_evol_pjy3  = sigma_Rec_evol_pjy3 - sigma_Gen_evol_pjy3;
   //Double_t diff_sigma_evol_pjy4  = sigma_Rec_evol_pjy4 - sigma_Gen_evol_pjy4;
   cout << "Rpidity: diff_gmean_evol_pjy1: " << diff_gmean_evol_pjy1 << endl; 
   cout << "Rpidity: diff_gmean_evol_pjy2: " << diff_gmean_evol_pjy2 << endl; 
   cout << "Rpidity: diff_gmean_evol_pjy3: " << diff_gmean_evol_pjy3 << endl; 
   cout << "Rpidity: diff_sigma_evol_pjy1: " << diff_sigma_evol_pjy1 << endl; 
   cout << "Rpidity: diff_sigma_evol_pjy2: " << diff_sigma_evol_pjy2 << endl; 
   cout << "Rpidity: diff_sigma_evol_pjy3: " << diff_sigma_evol_pjy3 << endl; 
   //cout << "Rpidity: diff_sigma_evol_pjy4: " << diff_sigma_evol_pjy4 << endl; 

   Double_t systFromGMean_evol_pjy1  = fabs(diff_gmean_evol_pjy1) / gmean_Gen_evol_pjy1 ;
   Double_t systFromGMean_evol_pjy2  = fabs(diff_gmean_evol_pjy2) / gmean_Gen_evol_pjy2 ;
   Double_t systFromGMean_evol_pjy3  = fabs(diff_gmean_evol_pjy3) / gmean_Gen_evol_pjy3 ;
   Double_t syst_evol_pjy1  = fabs(diff_sigma_evol_pjy1) / sigma_Gen_evol_pjy1 ;
   Double_t syst_evol_pjy2  = fabs(diff_sigma_evol_pjy2) / sigma_Gen_evol_pjy2 ;
   Double_t syst_evol_pjy3  = fabs(diff_sigma_evol_pjy3) / sigma_Gen_evol_pjy3 ;
   //Double_t syst_evol_pjy4  = fabs(diff_sigma_evol_pjy4) / sigma_Gen_evol_pjy4 ;
   cout << "Rpidity: systFromGMean_evol_pjy1: " << systFromGMean_evol_pjy1 << endl;  
   cout << "Rpidity: systFromGMean_evol_pjy2: " << systFromGMean_evol_pjy2 << endl;  
   cout << "Rpidity: systFromGMean_evol_pjy3: " << systFromGMean_evol_pjy3 << endl;  
   cout << "Rpidity: syst_evol_pjy1: " << syst_evol_pjy1 << endl;  
   cout << "Rpidity: syst_evol_pjy2: " << syst_evol_pjy2 << endl;  
   cout << "Rpidity: syst_evol_pjy3: " << syst_evol_pjy3 << endl;  
   //cout << "Rpidity: syst_evol_pjy4: " << syst_evol_pjy4 << endl; 

 
   TH1D* hSystematicsGMean_evol = new TH1D("hSystematicsGMean_evol", "; y; systematic uncertainty (%)", RapBins, xBinsRap);
   hSystematicsGMean_evol -> SetBinContent(1, systFromGMean_evol_pjy1*100);
   hSystematicsGMean_evol -> SetBinContent(2, systFromGMean_evol_pjy2*100);
   hSystematicsGMean_evol -> SetBinContent(3, systFromGMean_evol_pjy3*100);

   TH1D* hSystematics_evol = new TH1D("hSystematics_evol", "; y; systematic uncertainty (%)", RapBins, xBinsRap);
   hSystematics_evol -> SetBinContent(1, syst_evol_pjy1*100);
   hSystematics_evol -> SetBinContent(2, syst_evol_pjy2*100);
   hSystematics_evol -> SetBinContent(3, syst_evol_pjy3*100);
   //hSystematics_evol -> SetBinContent(4, syst_evol_pjy4*100);


   TCanvas *c10   = new TCanvas("c10", "Systematics evol. Vs Rapidity", 500, 500);
   c10->cd();
   hSystematics_evol -> GetYaxis() -> SetTitleOffset(1.5);
   hSystematics_evol -> SetMarkerStyle(20);
   hSystematics_evol -> SetMarkerColor(kRed);
   hSystematics_evol -> SetStats(0);
   hSystematics_evol -> Draw("P");

   TFile fileSys("histSysRap.root", "recreate"); 
   hSystematics_evol -> Write(); 
   //fileSys -> Write(hSystematics_evol);

   c10->SaveAs(outPath + "/plot_systematics_evol_Rap.png");
   c10->SaveAs(outPath + "/plot_systematics_evol_Rap.eps");


   TCanvas *cSysFromGMean   = new TCanvas("cSysFromGMean", "Systematics (From GMean) evol. Vs Rapidity", 500, 500);
   cSysFromGMean->cd();
   hSystematicsGMean_evol -> GetYaxis() -> SetTitleOffset(1.5);
   hSystematicsGMean_evol -> SetMarkerStyle(20);
   hSystematicsGMean_evol -> SetMarkerColor(kRed);
   hSystematicsGMean_evol -> SetStats(0);
   hSystematicsGMean_evol -> Draw("P");

   TFile fileSysGMean("histSysGMeanRap.root", "recreate"); 
   hSystematicsGMean_evol -> Write(); 

   cSysFromGMean->SaveAs(outPath + "/plot_systematicsGMean_evol_Rap.png");
   cSysFromGMean->SaveAs(outPath + "/plot_systematicsGMean_evol_Rap.eps");


   TCanvas *cOffsetRap   = new TCanvas("cOffsetRap", "Offset Vs Rapidity", 500, 500);

   cOffsetRap -> Divide(1,2);

   cOffsetRap_1->cd();
   hOffset_evol -> GetYaxis() -> SetTitleOffset(1.2);
   hOffset_evol -> SetMarkerStyle(20);
   hOffset_evol -> SetMarkerColor(kRed);
   hOffset_evol -> SetStats(0);
   hOffset_evol -> Draw("P");
   cOffsetRap_2->cd();
   hRelativeOffset_evol -> GetYaxis() -> SetTitleOffset(1.2);
   hRelativeOffset_evol -> SetMarkerStyle(20);
   hRelativeOffset_evol -> SetMarkerColor(kRed);
   hRelativeOffset_evol -> SetStats(0);
   hRelativeOffset_evol -> Draw("P");

   cOffsetRap->SaveAs(outPath + "/plot_Offset_evol_Rap.png");
   cOffsetRap->SaveAs(outPath + "/plot_Offset_evol_Rap.eps");
}
