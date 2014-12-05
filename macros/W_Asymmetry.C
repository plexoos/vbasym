#include <vector>

void W_Asymmetry()
{

   gStyle -> SetPadBottomMargin(0.15);
   gStyle -> SetPadLeftMargin(0.17);
   gStyle -> SetMarkerStyle(20);
   gStyle -> SetOptDate(0);
   gStyle -> SetErrorX();
   gStyle -> SetEndErrorSize(4);
   gStyle -> SetHistLineWidth(2);

   TString inPathNew     = "/star/institutions/bnl_me/fazio/stana_out/runlists/";
   TString inPathScratch = "/star/data05/scratch/fazio/stana_out/runlists/";
   TString outPath       = "~/vbasym_results/plots/4prelim";

   TFile *fileData         = TFile::Open(inPathNew + "run11_pp_transverse/hist/vbana.root");
   TFile *fileDataZ0       = TFile::Open(inPathNew + "run11_pp_transverse_zboson/hist/vbana.root");
   TFile *fileMCWm         = TFile::Open(inPathNew + "run11_mc_Wm2enu.lis/hist/vbana.root");
   TFile *fileSystRap      = TFile::Open("./histSysRap.root");
   TFile *fileSystGMeanRap = TFile::Open("./histSysGMeanRap.root");
   TFile *fileSystPt       = TFile::Open("./histSysPt.root");
   TFile *fileSystMeanPt   = TFile::Open("./histMeanSysPt.root");

   // take the histograms

   TH1 *hd_Wp_AsymAmpSqrtVsEta          = (TH1*) fileData->Get("asym/asym_wp_phys/hWBosonAsymAmpVsEta_");
   TH1 *hd_Wp_AsymAmpSqrtVsRap          = (TH1*) fileData->Get("asym/asym_wp_phys/hWBosonAsymAmpVsRap_");
   TH1 *hd_Wp_AsymAmpSqrtVsPt           = (TH1*) fileData->Get("asym/asym_wp_phys/hWBosonAsymAmpVsPt_");
   TH1 *hd_Wm_AsymAmpSqrtVsEta          = (TH1*) fileData->Get("asym/asym_wm_phys/hWBosonAsymAmpVsEta_");
   TH1 *hd_Wm_AsymAmpSqrtVsRap          = (TH1*) fileData->Get("asym/asym_wm_phys/hWBosonAsymAmpVsRap_");
   TH1 *hd_Wm_AsymAmpSqrtVsPt           = (TH1*) fileData->Get("asym/asym_wm_phys/hWBosonAsymAmpVsPt_");
   TH1 *hd_Z0_AsymAmpSqrtVsEta          = (TH1*) fileDataZ0->Get("asym/asym_z_phys/hZBosonAsymAmpVsEta_");
   TH1 *hd_Z0_AsymAmpSqrtVsRap          = (TH1*) fileDataZ0->Get("asym/asym_z_phys/hZBosonAsymAmpVsRap_");
   TH1 *hd_Z0_AsymAmpSqrtVsPt           = (TH1*) fileDataZ0->Get("asym/asym_z_phys/hZBosonAsymAmpVsPt_");
   TH1 *hd_Z0_Rap_2016Pr                = (TH1*) fileDataZ0->Get("event_z/hZ0_Rapidity_2016Pr");
   TH1 *hd_Z0_Pt_2016Pr                 = (TH1*) fileDataZ0->Get("event_z/hZ0_Pt_2016Pr");
   // take the histograms for the systematics 
   TH1 *hSysPt                          = (TH1*) fileSystPt->Get("hSystematics_evol");
   TH1 *hSysMeanPt                      = (TH1*) fileSystMeanPt->Get("hSystematicsMean_evol");
   TH1 *hSysRap                         = (TH1*) fileSystRap->Get("hSystematics_evol");
   TH1 *hSysRapGMean                    = (TH1*) fileSystGMeanRap->Get("hSystematicsGMean_evol");
   TH1 *hSysRapPR                       = (TH1*) fileMCWm->Get("event_mc/hSystematics_evol_PR");
   TH1 *hSysRapMR                       = (TH1*) fileMCWm->Get("event_mc/hSystematics_evol_MR");

   // Clone the histograms with only statistical errors
   TH1D *hd_Wp_AsymAmpSqrtVsRap_clone = (TH1D*)hd_Wp_AsymAmpSqrtVsRap -> Clone("hd_Wp_AsymAmpSqrtVsRap_clone");
   TH1D *hd_Wp_AsymAmpSqrtVsPt_clone  = (TH1D*)hd_Wp_AsymAmpSqrtVsPt  -> Clone("hd_Wp_AsymAmpSqrtVsPt_clone");
   TH1D *hd_Wm_AsymAmpSqrtVsRap_clone = (TH1D*)hd_Wm_AsymAmpSqrtVsRap -> Clone("hd_Wm_AsymAmpSqrtVsRap_clone");
   TH1D *hd_Wm_AsymAmpSqrtVsPt_clone  = (TH1D*)hd_Wm_AsymAmpSqrtVsPt  -> Clone("hd_Wm_AsymAmpSqrtVsPt_clone");


   // Add the systematic uncertainties

   //hSysRapGMean -> SetMarkerSize(2);  
   hSysRapPR -> SetMarkerSize(2);  
   hSysRapMR -> SetMarkerSize(2);  

   const Int_t nRapBins =  hd_Wp_AsymAmpSqrtVsRap->GetNbinsX();
   std::vector<Double_t> sysRapGMean(nRapBins); // vector of relative systematics of An vs W-Rapidity
   //std::vector<Double_t> sysRap(nRapBins); // vector of relative systematics of An vs W-Rapidity
   std::vector<Double_t> sysRapPR(nRapBins); // vector of relative POSITIVE systematics of An vs W-Rapidity 
   std::vector<Double_t> sysRapMR(nRapBins); // vector of relative NEGATIVE systematics of An vs W-Rapidity 

   for (int i = 1; i <= nRapBins; ++i) {
     sysRapGMean[i-1] =  hSysRapGMean -> GetBinContent(i);
     //sysRap[i-1] =  hSysRap -> GetBinContent(i);
     sysRapPR[i-1] =  hSysRapPR -> GetBinContent(i);
     sysRapPR[i-1] =  hSysRapMR -> GetBinContent(i);
   }

   const Int_t nPtBins =  hd_Wp_AsymAmpSqrtVsPt->GetNbinsX();
   std::vector<Double_t> sysPt(nPtBins); // vector of relative systematics of An vs W-Pt

   for (int i = 1; i <= nPtBins; ++i) {
     //sysPt[i-1] =  hSysPt -> GetBinContent(i);
     sysPt[i-1] =  hSysMeanPt -> GetBinContent(i);
   }


   //TGraphAsymmErrors* gd_Wp_AsymAmpSqrtVsRap = new TGraphAsymmErrors(nRapBins);

   for (int i = 1; i <= nRapBins; ++i) { // loop over Rapidity bins
     Double_t AsymBin = hd_Wp_AsymAmpSqrtVsRap->GetBinContent(i);
     cout  << "W+ Rapidity-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wp_AsymAmpSqrtVsRap->GetBinError(i);
     cout  << "W+ Rapidity-Bin " << i << " errStatBin= " << errStatBin << endl;
     cout  << "W+ Rapidity-Bin " << i << " Relative syst.= " << sysRapGMean[i-1] << "%" << endl;
     Double_t errSysBin = fabs(AsymBin * sysRapGMean[i-1]/100);
     cout  << "W+ Rapidity-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     cout  << "W+ Rapidity-Bin " << i << " errBin= " << errBin << endl;
     hd_Wp_AsymAmpSqrtVsRap -> SetBinError(i, errBin);
   } // for

   for (int i = 1; i <= hd_Wp_AsymAmpSqrtVsPt->GetNbinsX(); ++i) { // loop over Pt bins
     Double_t AsymBin = hd_Wp_AsymAmpSqrtVsPt->GetBinContent(i);
     cout  << "W+ Pt-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wp_AsymAmpSqrtVsPt->GetBinError(i);
     cout  << "W+ Pt-Bin " << i << " errStatBin= " << errStatBin << endl;
     cout  << "W+ Pt-Bin " << i << " Relative syst.= " << sysPt[i-1] << "%" << endl;
     Double_t errSysBin;
     errSysBin = fabs(AsymBin * sysPt[i-1]/100);
     cout  << "W+ Pt-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin;
     errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     cout  << "W+ Pt-Bin " << i << " errBin= " << errBin << endl;
     hd_Wp_AsymAmpSqrtVsPt->SetBinError(i, errBin);
   }

   for (int i = 1; i <= nRapBins; ++i) { // loop over Rapidity bins 
     Double_t AsymBin = hd_Wm_AsymAmpSqrtVsRap->GetBinContent(i);
     cout  << "W- Rapidity-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wm_AsymAmpSqrtVsRap->GetBinError(i);
     cout  << "W- Rapidity-Bin " << i << " errStatBin= " << errStatBin << endl;
     cout  << "W- Rapidity-Bin " << i << " Relativ syst.= " << sysRapGMean[i-1] << "%" << endl;
     Double_t errSysBin = fabs(AsymBin * sysRapGMean[i-1]/100);
     cout  << "W- Rapidity-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     cout  << "W- Rapidity-Bin " << i << " errBin= " << errBin << endl;
     hd_Wm_AsymAmpSqrtVsRap->SetBinError(i, errBin);
   }

   for (int i = 1; i <= hd_Wm_AsymAmpSqrtVsPt->GetNbinsX(); ++i) { // loop over Pt bins 
     Double_t AsymBin = hd_Wm_AsymAmpSqrtVsPt->GetBinContent(i);
     cout  << "W- Pt-Bin " << i << " AsymBin= " << AsymBin << endl;
     Double_t errStatBin = hd_Wm_AsymAmpSqrtVsPt->GetBinError(i);
     cout  << "W- Pt-Bin " << i << " errStatBin= " << errStatBin << endl;
     //cout  << "W- Pt-Bin " << i << " Relativ syst.= " << sysPt[i-1]*100 << "%" << endl;
     cout  << "W- Pt-Bin " << i << " Relativ syst.= " << sysPt[i-1] << "%" << endl;
     Double_t errSysBin;
     //errSysBin = fabs(AsymBin * sysPt[i-1]);
     errSysBin = fabs(AsymBin * sysPt[i-1]/100);
     cout  << "W- Pt-Bin " << i << " errSysBin= " << errSysBin << endl;
     Double_t errBin;
     errBin = sqrt(pow(errStatBin,2) + pow(errSysBin,2)) ;  // sum stat. and sys. errors in quadrature
     cout  << "W- Pt-Bin " << i << " errBin= " << errBin << endl;
     hd_Wm_AsymAmpSqrtVsPt->SetBinError(i, errBin);
   }


   // Clone the histograms with errors bars rescaled to 2016 and values set to zero. 
   TH1D *hd_Wp_AsymAmpSqrtVsRap_2016Pr = (TH1D*)hd_Wp_AsymAmpSqrtVsRap -> Clone("hd_Wp_AsymAmpSqrtVsRap_2016Pr");
   TH1D *hd_Wp_AsymAmpSqrtVsPt_2016Pr  = (TH1D*)hd_Wp_AsymAmpSqrtVsPt  -> Clone("hd_Wp_AsymAmpSqrtVsPt_2016Pr");
   TH1D *hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb = (TH1D*)hd_Wp_AsymAmpSqrtVsRap -> Clone("hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb");
   TH1D *hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb  = (TH1D*)hd_Wp_AsymAmpSqrtVsPt  -> Clone("hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb");
   TH1D *hd_Wm_AsymAmpSqrtVsRap_2016Pr = (TH1D*)hd_Wm_AsymAmpSqrtVsRap -> Clone("hd_Wm_AsymAmpSqrtVsRap_2016Pr");
   TH1D *hd_Wm_AsymAmpSqrtVsPt_2016Pr  = (TH1D*)hd_Wm_AsymAmpSqrtVsPt  -> Clone("hd_Wm_AsymAmpSqrtVsPt_2016Pr");

   Double_t shiftRap = 0.03;
   Double_t shiftPt = 0.18;
   Double_t xBinsRap[4];
     xBinsRap[0] = -0.6  + shiftRap;
     xBinsRap[1] = -0.2  + shiftRap;
     xBinsRap[2] = 0.2   + shiftRap;
     xBinsRap[3] = 0.6   + shiftRap;
   Double_t xBinsPt[8]       = {0.5+shiftPt, 1+shiftPt, 2+shiftPt, 3+shiftPt, 4+shiftPt, 5+shiftPt, 6+shiftPt, 10+shiftPt}; 
   /*
   Double_t xBinsRap_wp_400pb[4];
     xBinsRap_wp_400pb[0] = -0.6  + (2*shiftRap);
     xBinsRap_wp_400pb[1] = -0.2  + (2*shiftRap);
     xBinsRap_wp_400pb[2] = 0.2   + (2*shiftRap);
     xBinsRap_wp_400pb[3] = 0.6   + (2*shiftRap);
   Double_t xBinsPt_wp_400pb[8] = {0.5+2*shiftPt, 1+2*shiftPt, 2+2*shiftPt, 3+2*shiftPt, 4+2*shiftPt, 5+2*shiftPt, 6+2*shiftPt, 10+2*shiftPt};
   Double_t xBinsRap_wm_400pb[4];
     xBinsRap_wm_400pb[0] = -0.6  + 3*shiftRap;
     xBinsRap_wm_400pb[1] = -0.2  + 3*shiftRap;
     xBinsRap_wm_400pb[2] = 0.2   + 3*shiftRap;
     xBinsRap_wm_400pb[3] = 0.6   + 3*shiftRap;
   Double_t xBinsPt_wm_400pb[8] = {0.5+3*shiftPt, 1+3*shiftPt, 2+3*shiftPt, 3+3*shiftPt, 4+3*shiftPt, 5+3*shiftPt, 6+3*shiftPt, 10+3*shiftPt};
   */
   TH1D *hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift = new TH1D("hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift", "", 3, xBinsRap); //shift to make it visible
   TH1D *hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift  = new TH1D("hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift", "", 7, xBinsPt); //shift to make it visible

   //TH1D *hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb_shift = new TH1D("hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb_shift", "", 3, xBinsRap_wp_400pb); //shift to make it visible
   //TH1D *hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb_shift  = new TH1D("hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb_shift", "", 7, xBinsPt_wp_400pb); //shift to make it visible
   TH1D *hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift = new TH1D("hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift", "", 3, xBinsRap); //shift to make it visible
   TH1D *hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift  = new TH1D("hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift", "", 7, xBinsPt); //shift to make it visible

   Double_t projFactor2016 = 5;
   Double_t projFactor2016_400pb = 0.4444444;

   for (int i = 1; i <= hd_Wp_AsymAmpSqrtVsRap_2016Pr->GetNbinsX(); ++i) { // loop over Rapidity bins 
     hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetBinContent(i, 0);
     hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetBinContent(i, 0);
     Double_t errBin = hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetBinError(i);
     Double_t errProj2016Bin       = errBin / projFactor2016;
     Double_t errProj2016Bin_400pb = errProj2016Bin / projFactor2016_400pb;
     hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetBinError(i, errProj2016Bin);
     hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetBinError(i, errProj2016Bin_400pb);
   }

   for (int i = 1; i <= hd_Wp_AsymAmpSqrtVsPt_2016Pr->GetNbinsX(); ++i) { // loop over Pt bins 
     hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetBinContent(i, 0);
     hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetBinContent(i, 0);
     Double_t errBin = hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetBinError(i);
     Double_t errProj2016Bin = errBin / projFactor2016;
     Double_t errProj2016Bin_400pb = errProj2016Bin / projFactor2016_400pb;
     hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetBinError(i, errProj2016Bin);
     hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetBinError(i, errProj2016Bin_400pb);
   }

   for (int i = 1; i <= hd_Wm_AsymAmpSqrtVsRap_2016Pr->GetNbinsX(); ++i) { // loop over Rapidity bins 
     hd_Wm_AsymAmpSqrtVsRap_2016Pr       -> SetBinContent(i, 0);
     hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetBinContent(i, 0);
     hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetBinContent(i, 0);
     Double_t errBin = hd_Wm_AsymAmpSqrtVsRap_2016Pr -> GetBinError(i);
     Double_t errProj2016Bin = errBin / projFactor2016;
     Double_t errProj2016Bin_400pb = errProj2016Bin / projFactor2016_400pb;
     hd_Wm_AsymAmpSqrtVsRap_2016Pr       -> SetBinError(i, errProj2016Bin);
     hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetBinError(i, errProj2016Bin);
     hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetBinError(i, errProj2016Bin_400pb);
   }

   for (int i = 1; i <= hd_Wm_AsymAmpSqrtVsPt_2016Pr->GetNbinsX(); ++i) { // loop over Pt bins 
     hd_Wm_AsymAmpSqrtVsPt_2016Pr       -> SetBinContent(i, 0); 
     hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> SetBinContent(i, 0);
     hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetBinContent(i, 0);
     Double_t errBin = hd_Wm_AsymAmpSqrtVsPt_2016Pr -> GetBinError(i);
     Double_t errProj2016Bin = errBin / projFactor2016;
     Double_t errProj2016Bin_400pb = errProj2016Bin / projFactor2016_400pb;
     hd_Wm_AsymAmpSqrtVsPt_2016Pr       -> SetBinError(i, errProj2016Bin);
     hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> SetBinError(i, errProj2016Bin);
     hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetBinError(i, errProj2016Bin_400pb);
   }


   TH1D *hd_Z0_AsymAmpSqrtVsRap_2016Pr = (TH1D*)hd_Z0_Rap_2016Pr  -> Clone("hd_Z0_AsymAmpSqrtVsRap_2016Pr");
   TH1D *hd_Z0_AsymAmpSqrtVsPt_2016Pr  = (TH1D*)hd_Z0_Pt_2016Pr   -> Clone("hd_Z0_AsymAmpSqrtVsPt_2016Pr");
   TH1D *hd_Z0_AsymAmpSqrtVsRap_2016Pr_400pb = (TH1D*)hd_Z0_Rap_2016Pr  -> Clone("hd_Z0_AsymAmpSqrtVsRap_2016Pr_400pb");
   TH1D *hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb  = (TH1D*)hd_Z0_Pt_2016Pr   -> Clone("hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb");

   TH1D *hd_Z0_Asym_2016Pr_oneBin = (TH1D*)hd_Z0_AsymAmpSqrtVsRap -> Clone("hd_Z0_Asym_2016Pr_oneBin");

   Double_t xBinsPt_Z0Pr[4]  = {0, 5, 10, 25};
   Double_t xBinsRap_Z0Pr[4] = {-0.6, -0.2, 0.2, 0.6};
   //Double_t asymScaleFromYelds = 2.;
   Double_t projFactor2016_Z0 = 2.5;
   Double_t projFactor2016_Z0_oneBin = 5.;

   for (int i = 1; i <= hd_Z0_Asym_2016Pr_oneBin->GetNbinsX(); ++i) {
     hd_Z0_Asym_2016Pr_oneBin   -> SetBinContent(1, 0); 
     Double_t errBin = hd_Z0_Asym_2016Pr_oneBin -> GetBinError(i);
     Double_t Bin = hd_Z0_Asym_2016Pr_oneBin -> GetBinContent(i);
     cout << "Z0 ONE BIN = " << Bin << endl;
     cout << "Z0 ONE BIN errBin = " << errBin << endl;
     Double_t errProj2016Bin1 = errBin / projFactor2016_Z0_oneBin;
     cout << "Z0 ONE BIN error = " << errProj2016Bin1 << endl;
     hd_Z0_Asym_2016Pr_oneBin   -> SetBinError(i, errProj2016Bin1);
   }

   for (int i = 0; i <= hd_Z0_AsymAmpSqrtVsRap_2016Pr->GetNbinsX(); ++i) { // loop over Rapidity bins  
     Double_t binVal = hd_Z0_AsymAmpSqrtVsRap_2016Pr -> GetBinContent(i);
     Double_t errBin = hd_Z0_AsymAmpSqrtVsRap_2016Pr -> GetBinError(i);
     cout << "Z0 Rap binVal = " << binVal << endl;
     cout << "Z0 Rap errBin = " << errBin << endl;
     //Double_t errProj2016Bin = (errBin/projFactor2016) * asymScaleFromYelds;
     Double_t errProj2016Bin = errBin / projFactor2016_Z0;
     Double_t errProj2016Bin_400pb = errProj2016Bin / projFactor2016_400pb;
     cout << "Z0 Rap error = " << errProj2016Bin << endl;
     Double_t relError = errProj2016Bin / binVal;
     Double_t relError_400pb = errProj2016Bin_400pb / binVal;
     cout << "Z0 Rap Relative error = " << relError << endl;
     hd_Z0_AsymAmpSqrtVsRap_2016Pr       -> SetBinContent(i, 0); 
     hd_Z0_AsymAmpSqrtVsRap_2016Pr_400pb -> SetBinContent(i, 0);
     hd_Z0_AsymAmpSqrtVsRap_2016Pr       -> SetBinError(i, relError);
     hd_Z0_AsymAmpSqrtVsRap_2016Pr_400pb -> SetBinError(i, relError_400pb);
   }

   for (int i = 0; i <= hd_Z0_AsymAmpSqrtVsPt_2016Pr->GetNbinsX(); ++i) { // loop over Pt bins  
     Double_t binVal = hd_Z0_AsymAmpSqrtVsPt_2016Pr -> GetBinContent(i);
     Double_t errBin = hd_Z0_AsymAmpSqrtVsPt_2016Pr -> GetBinError(i);
     cout << "Z0 Rt binVal = " << binVal << endl;
     cout << "Z0 Pt errBin = " << errBin << endl;
     //Double_t errProj2016Bin = (errBin/projFactor2016) * asymScaleFromYelds;;
     Double_t errProj2016Bin = errBin / projFactor2016_Z0;
     Double_t errProj2016Bin_400pb = errProj2016Bin / projFactor2016_400pb;
     cout << "Z0 Pt error = " << errProj2016Bin << endl;
     if (binVal != 0) Double_t relError = errProj2016Bin / binVal;
     if (binVal != 0) Double_t relError_400pb = errProj2016Bin_400pb / binVal;
     cout << "Z0 Pt Relative error = " << relError << endl;
     hd_Z0_AsymAmpSqrtVsPt_2016Pr       -> SetBinContent(i, 0);  
     hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb -> SetBinContent(i, 0);
     hd_Z0_AsymAmpSqrtVsPt_2016Pr       -> SetBinError(i, relError);
     hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb -> SetBinError(i, relError_400pb);
   }

  // Plot the asymmetries: 

 
   TLine *line1 = new TLine(-0.6, 0, 10, 0);
   //line1->SetLineColor(kRed);
   line1->SetLineWidth(2);
   line1->SetLineStyle(2);

   //Double_t xLeg = 0.2;
   Double_t xLeg = 0.43;
   Double_t yLeg = 0.17;
   TLatex *textSTAR = new TLatex(xLeg, yLeg, "STAR p-p 500 GeV #intL = 25 pb^{-1}");
   textSTAR -> SetNDC(); 
   textSTAR -> SetTextSize(0.035);
   TLatex *textWp = new TLatex(0.2, 0.83, "W^{+} #rightarrow l^{+} #nu");
   textWp -> SetNDC(); 
   textWp -> SetTextFont(32);
   textWp -> SetTextSize(0.07);
   TLatex *textWm = new TLatex(0.2, 0.83, "W^{-} #rightarrow l^{-} #nu");
   textWm -> SetNDC(); 
   textWm -> SetTextFont(32);
   textWm -> SetTextSize(0.07);
   TLatex *textArrow = new TLatex(0.18, 0.83, "#uparrow 106%");
   textArrow -> SetNDC(); 
   textArrow -> SetTextFont(32);
   textArrow -> SetTextSize(0.15);
   textArrow -> SetTextColor(kRed);
   TLatex *textZ0 = new TLatex(0.2, 0.83, "Z^{0} #rightarrow l^{+} l^{-}");
   textZ0 -> SetNDC(); 
   textZ0 -> SetTextFont(32);
   textZ0 -> SetTextSize(0.07);
   TLatex *textPtLim = new TLatex(xLeg, yLeg-0.05, "0.5 < P_{T} < 7 GeV");
   textPtLim -> SetNDC(); 
   textPtLim -> SetTextFont(32);
   textPtLim -> SetTextSize(0.04);
   TLatex *textRapLim = new TLatex(xLeg, yLeg-0.05, "|y| < 1");
   textRapLim -> SetNDC(); 
   textRapLim -> SetTextFont(32);
   textRapLim -> SetTextSize(0.04);
   TLatex *textLSys = new TLatex(xLeg,  yLeg-0.1, "3.4% beam pol. uncertainty not shown");
   textLSys -> SetNDC(); 
   textLSys -> SetTextFont(32);
   textLSys -> SetTextSize(0.04);
   TLatex *textStarPrel = new TLatex(0.55, 0.83, "STAR preliminary");
   textStarPrel -> SetNDC(); 
   textStarPrel -> SetTextColor(kRed); 
   textStarPrel -> SetTextFont(32);
   textStarPrel -> SetTextSize(0.06);
   TLatex *textStarPrel_1w = new TLatex(0.6, 0.83, "STAR preliminary");
   textStarPrel_1w -> SetNDC(); 
   textStarPrel_1w -> SetTextColor(kRed); 
   textStarPrel_1w -> SetTextFont(32);
   textStarPrel_1w -> SetTextSize(0.04);
   //TLatex *textStarProj = new TLatex(0.25, 0.8, "STAR projection \\int");
   TLatex *textStarProj = new TLatex(0.25, 0.8, "STAR projection");
   textStarProj -> SetNDC(); 
   textStarProj -> SetTextColor(kRed); 
   textStarProj -> SetTextFont(32);
   textStarProj -> SetTextSize(0.06);
   //TLatex *textStarProjBis = new TLatex(0.6, 0.8, "L_{DEL} = 900 pb^{-1}");
   TLatex *textStarProjBis = new TLatex(0.6, 0.8, "");
   textStarProjBis -> SetNDC(); 
   textStarProjBis -> SetTextColor(kRed); 
   textStarProjBis -> SetTextFont(32);
   textStarProjBis -> SetTextSize(0.06);
   //TLatex *textStarProj_1w = new TLatex(0.25, 0.83, "STAR projection \\int");
   TLatex *textStarProj_1w = new TLatex(0.25, 0.83, "STAR projection");
   textStarProj_1w -> SetNDC(); 
   textStarProj_1w -> SetTextColor(kRed); 
   textStarProj_1w -> SetTextFont(32);
   textStarProj_1w -> SetTextSize(0.04);;
   //TLatex *textStarProjBis_1w = new TLatex(0.54, 0.83, "L_{DEL} = 900 pb^{-1}");
   TLatex *textStarProjBis_1w = new TLatex(0.54, 0.83, "");
   textStarProjBis_1w -> SetNDC(); 
   textStarProjBis_1w -> SetTextColor(kRed); 
   textStarProjBis_1w -> SetTextFont(32);
   textStarProjBis_1w -> SetTextSize(0.04);
   TText *textLetter = new TText(0.2, 0.07, "(a)");
   textLetter -> SetNDC();
   textLetter -> SetTextColor(kRed); 
   textLetter -> SetTextFont(32);
   textLetter -> SetTextSize(0.06);
   TText *textLetter_1w = new TText(0.82, 0.31, "(e)");
   textLetter_1w -> SetNDC();
   textLetter_1w -> SetTextColor(kRed); 
   textLetter_1w -> SetTextFont(32);
   textLetter_1w -> SetTextSize(0.06);
  
  TCanvas *cWpAmpEta   = new TCanvas("cWpAmpEta", "An Wp Eta", 600, 600);

  cWpAmpEta -> cd();
  hd_Wp_AsymAmpSqrtVsEta -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsEta -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsEta -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsEta -> SetTitle("; #eta^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsEta -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  //hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textPtLim);
  //hd_Wp_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textLSys);
  hd_Wp_AsymAmpSqrtVsEta -> Draw();
  textPtLim -> DrawLatex(0.2, 0.82, "STAR p-p 500 GeV #int L = 25 pb^{-1}");
  textSTAR  -> DrawLatex(0.2, 0.77, "0.5 < P_{T} < 7 GeV");
  textLSys  -> DrawLatex(0.2, 0.72, "3.4% beam pol. uncertainty not shown");
  textWp    -> DrawLatex(0.2, 0.2, "W^{+} #rightarrow l^{+} #nu");
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsEta -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsEta -> GetXaxis() -> GetXmax(), 0);

  cWpAmpEta->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsEta.png");
  cWpAmpEta->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsEta.eps");
 
  TCanvas *cWpAmpRap   = new TCanvas("cWpAmpRap", "An Wp Rapidity", 600, 650);

  //TPad*    upperPad = new TPad("upperPad", "upperPad", .005, .7525, .995, .995);
  TPad*    upperPad = new TPad("upperPad", "upperPad", .005, .25, .995, .995);
  TPad*    lowerPad = new TPad("lowerPad", "lowerPad", .005, .005, .995, .25);
  upperPad -> SetBottomMargin(0.0);
  lowerPad -> SetTopMargin(0.0);
  //upperPad -> SetBottomMargin(0.017);
  //lowerPad -> SetTopMargin(0.03);
  lowerPad -> SetBottomMargin(0.35);
  upperPad->Draw();
  lowerPad->Draw();

  //cWpAmpRap -> Divide(1,2);
  //cWpAmpRap -> cd(1);
  upperPad->cd();
  //gPad -> SetBottomMargin(0.1);
  gPad-> Modified();
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsRap -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsRap -> SetTitle("; y^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textWp);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  //hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textStarPrel);
  //hd_Wp_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLetter);
  hd_Wp_AsymAmpSqrtVsRap -> Draw("E0");
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetLabelFont(62);
  hd_Wp_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleFont(62);
  hd_Wp_AsymAmpSqrtVsRap_clone -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsRap_clone -> Draw("same E1");
  textLetter -> Draw();
  textWp -> Draw();
  textStarPrel -> Draw();
  //Double_t xMin =  hd_Wp_AsymAmpSqrtVsRap_clone  -> GetXaxis() -> GetXmin();
  //Double_d xMax = hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax;
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax(), 0);

  //cWpAmpRap -> cd(2);
  lowerPad->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  TH2F *frameSysRap = new TH2F("frameSysRap","",5,-0.6,0.6,5,0,11);
  //TH2F *frameSysRap = new TH2F("frameSysRap","",5,-0.6,0.6,5,-12,12);
  frameSysRap -> SetStats(0); 
  frameSysRap -> SetTitle("; y^{W}; syst.(%)");
  //frameSysRap -> GetXaxis() -> SetTitleOffset(0.7) 
  frameSysRap -> GetXaxis() -> SetNdivisions(8); 
  frameSysRap -> GetXaxis() -> SetLabelSize(0.15); 
  frameSysRap -> GetXaxis() -> SetLabelOffset(0.03);  
  frameSysRap -> GetYaxis() -> SetNdivisions(8); 
  frameSysRap -> GetYaxis() -> SetLabelSize(0.1); ;
  frameSysRap -> SetTitleSize(0.2, "x");
  frameSysRap -> SetTitleOffset(0.78, "x"); 
  frameSysRap -> SetTitleSize(0.12, "Y");
  frameSysRap -> SetTitleOffset(0.5, "Y"); 
  frameSysRap -> Draw(); 
  //hSysRapPR   -> SetTitle("; y^{W}; syst.(%)");
  //hSysRapPR   -> GetXaxis() -> SetTitleOffset(0.7);
  //hSysRapPR   -> GetXaxis() -> SetNdivisions(8);
  //hSysRapPR   -> GetXaxis() -> SetLabelSize(0.15);
  //hSysRapPR   -> GetYaxis() -> SetTitleOffset(0.5);
  //hSysRapPR   -> GetYaxis() -> SetNdivisions(8);
  //hSysRapPR   -> GetYaxis() -> SetLabelSize(0.1);
  //hSysRapPR   -> SetTitleSize(0.2, "X");
  //hSysRapPR   -> SetTitleSize(0.12, "Y");
  hSysRapGMean   -> Draw("same P");
  //hSysRapPR   -> Draw("same P");
  //hSysRapMR   -> Draw("same P");

  //cWpAmpRap -> Update();  
  cWpAmpRap->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsRap.png");
  cWpAmpRap->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsRap.eps");
  
  TCanvas *cWpAmpPt   = new TCanvas("cWpAmpPt", "An Wp Pt", 600, 650);

  TPad*    upperPad_WpPt = new TPad("upperPad_WpPt", "upperPad_WpPt", .005, .25, .995, .995);
  TPad*    lowerPad_WpPt = new TPad("lowerPad_WpPt", "lowerPad_WpPt", .005, .005, .995, .25);
  upperPad_WpPt -> SetBottomMargin(0.0);
  lowerPad_WpPt -> SetTopMargin(0.0);
  //upperPad_WpPt -> SetBottomMargin(0.017);
  //lowerPad_WpPt -> SetTopMargin(0.017);
  lowerPad_WpPt -> SetBottomMargin(0.35);
  upperPad_WpPt -> Draw();
  lowerPad_WpPt -> Draw();

  //cWpAmpPt -> cd();
  upperPad_WpPt->cd();
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsPt -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textWp);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textRapLim);
  hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textLSys);
  //hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textStarPrel);
  //hd_Wp_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textLetter);
  hd_Wp_AsymAmpSqrtVsPt -> Draw("E0");
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetLabelFont(62);
  hd_Wp_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleFont(62);
  hd_Wp_AsymAmpSqrtVsPt_clone -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt_clone -> Draw("same E1");
  textLetter -> DrawText(0.2, 0.07, "(c)");
  textWp -> Draw();
  textStarPrel -> Draw();
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmax(), 0);

  lowerPad_WpPt->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysMeanPt   -> SetTitle("; P_{T}^{W}; syst.(%)");
  hSysMeanPt   -> GetXaxis() -> SetRangeUser(0, 9.99);
  hSysMeanPt   -> GetXaxis() -> SetTitleOffset(0.75);
  hSysMeanPt   -> GetXaxis() -> SetNdivisions(10);
  hSysMeanPt   -> GetXaxis() -> SetLabelSize(0.15);
  hSysMeanPt   -> GetYaxis() -> SetTitleOffset(0.5);
  hSysMeanPt   -> GetYaxis() -> SetNdivisions(8);
  hSysMeanPt   -> GetYaxis() -> SetLabelSize(0.1);
  hSysMeanPt   -> GetYaxis() -> SetRangeUser(0, 27);
  hSysMeanPt   -> SetBinContent(1,0); // mask the syst. for the first Pt bin
  hSysMeanPt   -> SetTitleSize(0.2, "X");
  hSysMeanPt   -> SetTitleSize(0.12, "Y");
  hSysMeanPt   -> Draw("P");
  //TArrow *arrow = new TArrow(hSysMeanPt->GetBinCenter(1), 5, hSysMeanPt->GetBinCenter(1), 20, 0.15, "|>");
  //arrow -> SetAngle(40);
  //arrow -> SetLineWidth(2);
  //arrow -> Draw("same");
  textArrow ->  Draw();

  cWpAmpPt -> Update();  
  cWpAmpPt->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsPt.png");
  cWpAmpPt->SaveAs(outPath + "/hd_Wp_AsymAmpSqrtVsPt.eps");
  
  TCanvas *cWmAmpEta   = new TCanvas("cWmAmpEta", "An Wm Eta", 600, 600);

  cWmAmpEta -> cd();
  hd_Wm_AsymAmpSqrtVsEta -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsEta -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsEta -> SetTitle("; #eta^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsEta -> SetMarkerStyle(20);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textWm);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textPtLim);
  //hd_Wm_AsymAmpSqrtVsEta -> GetListOfFunctions() -> Add(textLSys);
  hd_Wm_AsymAmpSqrtVsEta -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wm_AsymAmpSqrtVsEta -> Draw();
  textPtLim -> DrawLatex(0.2, 0.82, "STAR p-p 500 GeV #intL = 25 pb^{-1}");
  textSTAR  -> DrawLatex(0.2, 0.77, "0.5 < P_{T} < 7 GeV");
  textLSys  -> DrawLatex(0.2, 0.72, "3.4% beam pol. uncertainty not shown");
  textWm ->  DrawLatex(0.2, 0.2, "W^{-} #rightarrow l^{-} #nu");
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsEta -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsEta -> GetXaxis() -> GetXmax(), 0);

  cWmAmpEta->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsEta.png");
  cWmAmpEta->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsEta.eps");

  TCanvas *cWmAmpRap   = new TCanvas("cWmAmpRap", "An Wm Rapidity", 600, 650);

  TPad*    upperPad_WmRap = new TPad("upperPad_WmRap", "upperPad_WmRap", .005, .25, .995, .995);
  TPad*    lowerPad_WmRap = new TPad("lowerPad_WmRap", "lowerPad_WmRap", .005, .005, .995, .25);
  upperPad_WmRap -> SetBottomMargin(0.0);
  lowerPad_WmRap -> SetTopMargin(0.0);
  //upperPad_WmRap -> SetBottomMargin(0.017);
  //lowerPad_WmRap -> SetTopMargin(0.03);
  lowerPad_WmRap -> SetBottomMargin(0.35);
  upperPad_WmRap -> Draw();
  lowerPad_WmRap -> Draw();

  //cWmAmpRap -> cd();
  upperPad_WmRap->cd();
  gPad-> Modified();
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap -> SetTitle("; y^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsRap -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textWm);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  //hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textStarPrel);
  //hd_Wm_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLetter);
  hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Wm_AsymAmpSqrtVsRap -> Draw("E0");
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsRap_clone -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap_clone -> Draw("same E1");
  textLetter -> DrawText(0.2, 0.07, "(b)");
  textWm -> Draw();
  textStarPrel -> Draw();
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax(), 0);

  lowerPad_WmRap->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  frameSysRap -> Draw(); 
  //hSysRapPR   -> SetTitle("; y^{W}; syst.(%)");
  //hSysRapPR   -> GetXaxis() -> SetTitleOffset(0.7);
  //hSysRapPR   -> GetXaxis() -> SetNdivisions(8);
  //hSysRapPR   -> GetXaxis() -> SetLabelSize(0.15);
  //hSysRapPR   -> GetYaxis() -> SetTitleOffset(0.5);
  //hSysRapPR   -> GetYaxis() -> SetNdivisions(8);
  //hSysRapPR   -> GetYaxis() -> SetLabelSize(0.1);
  //hSysRapPR   -> SetTitleSize(0.2, "X");
  //hSysRapPR   -> SetTitleSize(0.12, "Y");
  hSysRapGMean   -> Draw("same P");
  //hSysRapPR   -> Draw("same P");
  //hSysRapMR   -> Draw("same P");

  cWpAmpRap -> Update();  
  cWmAmpRap->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsRap.png");
  cWmAmpRap->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsRap.eps");

  TCanvas *cWmAmpPt   = new TCanvas("cWmAmpPt", "An Wm Pt", 600, 650);

  TPad*    upperPad_WmPt = new TPad("upperPad_WmPt", "upperPad_WmPt", .005, .25, .995, .995);
  TPad*    lowerPad_WmPt = new TPad("lowerPad_WmPt", "lowerPad_WmPt", .005, .005, .995, .25);
  upperPad_WmPt -> SetBottomMargin(0.0);
  lowerPad_WmPt -> SetTopMargin(0.0);
  //upperPad_WmPt -> SetBottomMargin(0.017);
  //lowerPad_WmPt -> SetTopMargin(0.017);
  lowerPad_WmPt -> SetBottomMargin(0.35);
  upperPad_WmPt -> Draw();
  lowerPad_WmPt -> Draw();

  //cWmAmpPt -> cd();
  upperPad_WmPt->cd();
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsPt -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textSTAR);
  //hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textWm);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textRapLim);
  hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textLSys);
  //hd_Wm_AsymAmpSqrtVsPt -> GetListOfFunctions() -> Add(textStarPrel);
  hd_Wm_AsymAmpSqrtVsPt -> Draw("E0");
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsPt -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsPt_clone -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt_clone -> Draw("same E1");
  textLetter -> DrawText(0.2, 0.07, "(d)");
  textWm -> Draw();
  textStarPrel -> Draw();
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmax(), 0);

  lowerPad_WmPt->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysMeanPt   -> SetTitle("; P_{T}^{W}; syst.(%)");
  hSysMeanPt   -> GetXaxis() -> SetRangeUser(0, 9.99);
  hSysMeanPt   -> GetXaxis() -> SetTitleOffset(0.75);
  hSysMeanPt   -> GetXaxis() -> SetNdivisions(10);
  hSysMeanPt   -> GetXaxis() -> SetLabelSize(0.15);
  hSysMeanPt   -> GetYaxis() -> SetTitleOffset(0.5);
  hSysMeanPt   -> GetYaxis() -> SetNdivisions(8);
  hSysMeanPt   -> GetYaxis() -> SetLabelSize(0.1);
  hSysMeanPt   -> SetTitleSize(0.2, "X");
  hSysMeanPt   -> SetTitleSize(0.12, "Y");
  hSysMeanPt   -> Draw("P");
  textArrow    -> Draw();

  cWmAmpPt -> Update();  
  cWmAmpPt->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsPt.png");
  cWmAmpPt->SaveAs(outPath + "/hd_Wm_AsymAmpSqrtVsPt.eps");


  TCanvas *cWAmpPt   = new TCanvas("cWAmpPt", "An vs Pt", 600, 650);

  TPad*    upperPad_WPt = new TPad("upperPad_WPt", "upperPad_WPt", .005, .25, .995, .995);
  TPad*    lowerPad_WPt = new TPad("lowerPad_WPt", "lowerPad_WPt", .005, .005, .995, .25);
  upperPad_WPt -> SetBottomMargin(0.0);
  lowerPad_WPt -> SetTopMargin(0.0);
  lowerPad_WPt -> SetBottomMargin(0.35);
  upperPad_WPt -> Draw();
  lowerPad_WPt -> Draw();

  upperPad_WPt->cd();
  gPad -> SetGrid(0,0);
  gPad -> Modified();
  hd_Wm_AsymAmpSqrtVsPt -> SetMarkerColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt -> SetMarkerSize(1.3);
  hd_Wm_AsymAmpSqrtVsPt -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsPt -> Draw("E0");
  hd_Wm_AsymAmpSqrtVsPt_clone -> SetMarkerColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt_clone -> SetMarkerSize(1.3);
  hd_Wm_AsymAmpSqrtVsPt_clone -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt_clone -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsPt_clone -> Draw("same E1");
  hd_Wp_AsymAmpSqrtVsPt -> SetMarkerColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt -> SetMarkerSize(1.3);
  hd_Wp_AsymAmpSqrtVsPt -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsPt -> Draw("same E0");
  hd_Wp_AsymAmpSqrtVsPt_clone -> SetMarkerColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt_clone -> SetMarkerSize(1.3);
  hd_Wp_AsymAmpSqrtVsPt_clone -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt_clone -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsPt_clone -> Draw("same E1");
  //textLetter -> DrawText(0.2, 0.07, "(d)");
  textStarPrel -> DrawLatex(0.2, 0.83, "STAR preliminary");
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsPt -> GetXaxis() -> GetXmax(), 0);

  TLegend *leg_hSysMean = new TLegend(0.6, 0.7, 0.95, 0.9);
  leg_hSysMean -> SetFillColor(0);
  leg_hSysMean -> SetFillStyle(0);
  leg_hSysMean -> SetLineColor(0);
  leg_hSysMean -> SetBorderSize(0);
  leg_hSysMean -> AddEntry(hd_Wp_AsymAmpSqrtVsPt,"W^{+} #rightarrow l^{+} #nu", "p");
  leg_hSysMean -> AddEntry(hd_Wm_AsymAmpSqrtVsPt,"W^{-} #rightarrow l^{-} #nu", "p");
  leg_hSysMean -> Draw();


  lowerPad_WPt->cd();
  gPad -> SetGrid(0);
  gPad -> Modified();
  hSysMeanPt   -> SetMarkerColor(1);
  hSysMeanPt   -> Draw("P");
  textArrow -> SetTextColor(kBlack);
  textArrow    -> Draw();

  cWAmpPt -> Update();  
  cWAmpPt->SaveAs(outPath + "/hd_W_AsymAmpSqrtVsPt.png");
  cWAmpPt->SaveAs(outPath + "/hd_W_AsymAmpSqrtVsPt.eps");



  TCanvas *cWAmpRap   = new TCanvas("cWAmpRap", "An vs Rap", 600, 650);

  TPad*    upperPad_WRap = new TPad("upperPad_WRap", "upperPad_WRap", .005, .25, .995, .995);
  TPad*    lowerPad_WRap = new TPad("lowerPad_WRap", "lowerPad_WRap", .005, .005, .995, .25);
  upperPad_WRap -> SetBottomMargin(0.0);
  lowerPad_WRap -> SetTopMargin(0.0);
  lowerPad_WRap -> SetBottomMargin(0.35);
  upperPad_WRap -> Draw();
  lowerPad_WRap -> Draw();

  upperPad_WRap->cd();
  gPad -> SetGrid(0,0);
  gPad -> Modified();
  hd_Wm_AsymAmpSqrtVsRap -> SetMarkerColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap -> SetMarkerSize(1.3);
  hd_Wm_AsymAmpSqrtVsRap -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsRap -> Draw("E0");
  hd_Wm_AsymAmpSqrtVsRap_clone -> SetMarkerColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap_clone -> SetMarkerSize(1.3);
  hd_Wm_AsymAmpSqrtVsRap_clone -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap_clone -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsRap_clone -> Draw("same E1");
  hd_Wp_AsymAmpSqrtVsRap -> SetMarkerColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap -> SetMarkerSize(1.3);
  hd_Wp_AsymAmpSqrtVsRap -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsRap -> Draw("same E0");
  hd_Wp_AsymAmpSqrtVsRap_clone -> SetMarkerColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap_clone -> SetMarkerSize(1.3);
  hd_Wp_AsymAmpSqrtVsRap_clone -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap_clone -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsRap_clone -> Draw("same E1");
  //textLetter -> DrawText(0.2, 0.07, "(d)");
  textStarPrel -> DrawLatex(0.2, 0.83, "STAR preliminary");
  line1 -> DrawLine(hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmin(), 0, hd_Wm_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax(), 0);

  leg_hSysMean -> Draw();


  lowerPad_WRap->cd();
  gPad -> SetGrid(0);
  gPad -> Modified();
  frameSysRap  -> Draw(); 
  hSysRapGMean -> SetMarkerColor(1);
  hSysRapGMean -> Draw("same P");

  cWAmpRap -> Update();  
  cWAmpRap -> SaveAs(outPath + "/hd_W_AsymAmpSqrtVsRap.png");
  cWAmpRap -> SaveAs(outPath + "/hd_W_AsymAmpSqrtVsRap.eps");


  TCanvas *cZ0AmpRap   = new TCanvas("cZ0AmpRap", "An Z0 Rapidity", 600, 600);


  cZ0AmpRap -> cd();
  hd_Z0_AsymAmpSqrtVsRap -> GetYaxis() -> SetTitleOffset(1.6);
  hd_Z0_AsymAmpSqrtVsRap -> SetStats(0);
  hd_Z0_AsymAmpSqrtVsRap -> SetTitle("; y^{Z^{0}}; A_{N}");
  hd_Z0_AsymAmpSqrtVsRap -> SetMarkerStyle(20);
  hd_Z0_AsymAmpSqrtVsRap -> SetMarkerColor(kBlue);
  hd_Z0_AsymAmpSqrtVsRap -> SetMarkerSize(2);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textSTAR);
  hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textZ0);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textPtLim);
  //hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLSys);
  //hd_Z0_AsymAmpSqrtVsRap -> GetXaxis() -> SetRangeUser(-1., 0.99);
  hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textStarPrel_1w);
  hd_Z0_AsymAmpSqrtVsRap -> GetListOfFunctions() -> Add(textLetter_1w);
  hd_Z0_AsymAmpSqrtVsRap -> Draw("E1");
  line1 -> DrawLine(hd_Z0_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmin(), 0, hd_Z0_AsymAmpSqrtVsRap -> GetXaxis() -> GetXmax(), 0);

  textSTAR  -> DrawLatex(0.2, 0.3, "STAR p-p 500 GeV #intL = 25 pb^{-1}");
  textPtLim -> DrawLatex(0.2, 0.25, "0 < P_{T} < 25 GeV");
  textLSys  -> DrawLatex(0.2, 0.2, "3.4% beam pol. uncertainty not shown");
  //textLetter_1w -> DrawText(0.2, 0.07, "(e)");
  //hd_Z0_AsymAmpSqrtVsRap -> GetYaxis() -> SetRangeUser(-1.49, 1.49);


  cZ0AmpRap->SaveAs(outPath + "/hd_Z0_AsymAmpSqrtVsRap.png");
  cZ0AmpRap->SaveAs(outPath + "/hd_Z0_AsymAmpSqrtVsRap.eps");


  TCanvas *cWAmpProjRap   = new TCanvas("cWAmpProjRap", "W An 2016 projection Rapidity", 600, 650);

  gStyle -> SetErrorX(0);

  TPad*    upperPad_WRapProj = new TPad("upperPad_WRapProj", "upperPad_WRapProj", .005, .25, .995, .995);
  TPad*    lowerPad_WRapProj = new TPad("lowerPad_WRapProj", "lowerPad_WRapProj", .005, .005, .995, .25);
  upperPad_WRapProj -> SetBottomMargin(0.0);
  lowerPad_WRapProj -> SetTopMargin(0.0);
  lowerPad_WRapProj -> SetBottomMargin(0.35);
  upperPad_WRapProj -> Draw();
  lowerPad_WRapProj -> Draw();

  upperPad_WRapProj->cd();
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetMarkerColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> Draw("E1");

  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetTitle("; y^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetMarkerStyle(20);
  //hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> GetListOfFunctions() -> Add(textStarProj);
  //hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> GetListOfFunctions() -> Add(textStarProjBis);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetMarkerColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> Draw("same E1");

  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetTitle("; y^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetMarkerStyle(20);
  //hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> GetListOfFunctions() -> Add(textStarProj);
  //hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> GetListOfFunctions() -> Add(textStarProjBis);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> GetYaxis() -> SetLabelFont(62);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> GetYaxis() -> SetTitleFont(62);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetMarkerColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> SetLineStyle(2);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> Draw("same E1");

  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetTitle("; y^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetMarkerStyle(20);
  //hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> GetListOfFunctions() -> Add(textStarProj);
  //hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> GetListOfFunctions() -> Add(textStarProjBis);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetMarkerStyle(20);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetMarkerColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> SetLineStyle(2);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> Draw("same E1");

 
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmax(), 0);
 
  TLegend *leg1 = new TLegend(0.5, 0.005, 0.9, 0.31);
  leg1 -> SetFillColor(0);
  leg1 -> SetFillStyle(0);
  leg1 -> SetLineColor(0);
  leg1 -> SetBorderSize(0);
  leg1 -> SetTextSize(0.035);
  //leg1 -> AddEntry(hd_Wp_AsymAmpSqrtVsRap_2016Pr,"W^{+} stat. uncertainty", "lp");
  //leg1 -> AddEntry(hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift,"W^{-} stat. uncertainty", "lp");
  leg1 -> AddEntry(hd_Wp_AsymAmpSqrtVsRap_2016Pr,"W^{+} - \\int L(del.) = 900 pb^{-1}", "lp");
  leg1 -> AddEntry(hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb,"W^{+} - \\int L(del.) = 400 pb^{-1}", "lp");
  leg1 -> AddEntry(hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift,"W^{-} - \\int L(del.) = 900 pb^{-1}", "lp");
  leg1 -> AddEntry(hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift,"W^{-} - \\int L(del.) = 400 pb^{-1}", "lp");
  leg1  -> Draw();

  lowerPad_WRapProj->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  frameSysRap  -> Draw(); 
  hSysRapGMean -> Draw("same P");

  cWAmpProjRap -> Update();  
  cWAmpProjRap->SaveAs(outPath + "/hd_WAsym2016ProjRap.png");
  cWAmpProjRap->SaveAs(outPath + "/hd_WAsym2016ProjRap.eps");


  TCanvas *cWAmpProjRapZoom   = new TCanvas("cWAmpProjRapZoom", "W An 2016 projection Rapidity Zoom", 600, 650);

  TPad*    upperPad_WRapProjZoom = new TPad("upperPad_WRapProjZoom", "upperPad_WRapProjZoom", .005, .25, .995, .995);
  TPad*    lowerPad_WRapProjZoom = new TPad("lowerPad_WRapProjZoom", "lowerPad_WRapProjZoom", .005, .005, .995, .25);
  upperPad_WRapProjZoom -> SetBottomMargin(0.0);
  lowerPad_WRapProjZoom -> SetTopMargin(0.0);
  lowerPad_WRapProjZoom -> SetBottomMargin(0.35);
  upperPad_WRapProjZoom -> Draw();
  lowerPad_WRapProjZoom -> Draw();

  upperPad_WRapProjZoom->cd();
  upperPad_WRapProjZoom-> SetGrid(0,0);
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> Draw("E1");
  hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetRangeUser(-0.49, 0.49);
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_shift -> Draw("same E1");
  hd_Wp_AsymAmpSqrtVsRap_2016Pr_400pb -> Draw("same E1");
  hd_Wm_AsymAmpSqrtVsRap_2016Pr_400pb_shift -> Draw("same E1");
 
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmax(), 0);
  leg1  -> Draw();

  lowerPad_WRapProjZoom->cd();
  gPad -> SetGrid(0);
  gPad -> Modified();
  frameSysRap  -> Draw(); 
  hSysRapGMean -> Draw("same P");

  cWAmpProjRapZoom -> Update();  
  cWAmpProjRapZoom->SaveAs(outPath + "/hd_WAsym2016ProjRap_zoom.png");
  cWAmpProjRapZoom->SaveAs(outPath + "/hd_WAsym2016ProjRap_zoom.eps");


  TCanvas *cWAmpProjPt   = new TCanvas("cWAmpProjPt", "W An 2016 projection Pt", 600, 650);

  TPad*    upperPad_WPtProj = new TPad("upperPad_WPtProj", "upperPad_WPtProj", .005, .25, .995, .995);
  TPad*    lowerPad_WPtProj = new TPad("lowerPad_WPtProj", "lowerPad_WPtProj", .005, .005, .995, .25);
  upperPad_WPtProj -> SetBottomMargin(0.0);
  lowerPad_WPtProj -> SetTopMargin(0.0);
  lowerPad_WPtProj -> SetBottomMargin(0.35);
  upperPad_WPtProj -> Draw();
  lowerPad_WPtProj -> Draw();

  upperPad_WPtProj->cd();
  upperPad_WPtProj-> SetGrid(0,0);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetRangeUser(-1.49, 1.49);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetMarkerStyle(20);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetMarkerSize(1);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetMarkerColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> Draw("E1");

  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> SetMarkerStyle(20);
  //hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> GetListOfFunctions() -> Add(textStarProj);
  //hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> GetListOfFunctions() -> Add(textStarProjBis);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> SetMarkerColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> Draw("same E1");

  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetStats(0);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetMarkerStyle(20);
  //hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> GetListOfFunctions() -> Add(textStarProj);
  //hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> GetListOfFunctions() -> Add(textStarProjBis);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> GetYaxis() -> SetLabelFont(62);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> GetYaxis() -> SetTitleFont(62);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetMarkerColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetLineColor(kRed);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetLineWidth(2);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> SetLineStyle(2);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> Draw("same E1");

  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> GetXaxis() -> SetLabelOffset(1.3);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetStats(0);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetTitle("; P_{T}^{W}; A_{N}");
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetMarkerStyle(20);
  //hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> GetListOfFunctions() -> Add(textStarProj);
  //hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> GetListOfFunctions() -> Add(textStarProjBis);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> GetYaxis() -> SetLabelFont(62);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> GetYaxis() -> SetTitleFont(62);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetMarkerColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetLineColor(kBlue);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetLineWidth(2);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> SetLineStyle(2);
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> Draw("same E1");
 
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmax(), 0);
  leg1  -> Draw();

  lowerPad_WPtProj->cd();
  gPad -> SetGrid(0);
  gPad-> Modified();
  hSysMeanPt   -> Draw("P");
  textArrow    -> Draw();

  cWAmpProjPt -> Update();  
  cWAmpProjPt->SaveAs(outPath + "/hd_WAsym2016ProjPt.png");
  cWAmpProjPt->SaveAs(outPath + "/hd_WAsym2016ProjPt.eps");


  TCanvas *cWAmpProjPtZoom   = new TCanvas("cWAmpProjPtZoom", "W An 2016 projection Pt Zoom", 600, 650);

  TPad*    upperPad_WPtProjZoom = new TPad("upperPad_WPtProjZoom", "upperPad_WPtProjZoom", .005, .25, .995, .995);
  TPad*    lowerPad_WPtProjZoom = new TPad("lowerPad_WPtProjZoom", "lowerPad_WPtProjZoom", .005, .005, .995, .25);
  upperPad_WPtProjZoom -> SetBottomMargin(0.0);
  lowerPad_WPtProjZoom -> SetTopMargin(0.0);
  lowerPad_WPtProjZoom -> SetBottomMargin(0.35);
  upperPad_WPtProjZoom -> Draw();
  lowerPad_WPtProjZoom -> Draw();

  upperPad_WPtProjZoom->cd();
  upperPad_WPtProjZoom-> SetGrid(0,0);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetRangeUser(-0.49, 0.49);
  hd_Wp_AsymAmpSqrtVsPt_2016Pr -> Draw("E1");

  hd_Wm_AsymAmpSqrtVsPt_2016Pr_shift -> Draw("same E1");
  hd_Wp_AsymAmpSqrtVsPt_2016Pr_400pb -> Draw("same E1");
  hd_Wm_AsymAmpSqrtVsPt_2016Pr_400pb_shift -> Draw("same E1");
 
  line1 -> DrawLine(hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Wp_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmax(), 0);
  leg1  -> Draw();

  lowerPad_WPtProjZoom->cd();
  gPad -> SetGrid(0);
  gPad -> Modified();
  hSysMeanPt   -> Draw("P");
  textArrow    -> Draw();

  cWAmpProjPtZoom -> Update();  
  cWAmpProjPtZoom->SaveAs(outPath + "/hd_WAsym2016ProjPt_zoom.png");
  cWAmpProjPtZoom->SaveAs(outPath + "/hd_WAsym2016ProjPt_zoom.eps");


  TCanvas *cZ0AmpProjPt   = new TCanvas("cZ0AmpProjPt", "Z0 An 2016 projection Pt", 600, 650);

  cZ0AmpProjPt -> SetGrid(0,0);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr -> SetStats(0);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr -> SetTitle("; P_{T}^{Z}; A_{N}");
  //hd_Z0_AsymAmpSqrtVsPt_2016Pr -> GetListOfFunctions() -> Add(textStarProj_1w);
  //hd_Z0_AsymAmpSqrtVsPt_2016Pr -> GetListOfFunctions() -> Add(textStarProjBis_1w);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr -> GetYaxis() -> SetRangeUser(-0.49, 0.49);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr -> SetMarkerStyle(20);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr -> SetLineWidth(2);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr -> Draw("E1");

  //hd_Z0_Pt_2016Pr -> SetMarkerStyle(20);
  //hd_Z0_Pt_2016Pr -> Draw("E1");
  hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb -> SetMarkerStyle(20);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb -> SetLineWidth(2);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb -> SetLineStyle(2);
  hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb -> Draw("same E1");
 
  line1 -> DrawLine(hd_Z0_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Z0_AsymAmpSqrtVsPt_2016Pr -> GetXaxis() -> GetXmax(), 0);

  TLegend *leg2 = new TLegend(0.2, 0.15, 0.7, 0.34);
  leg2 -> SetFillColor(0);
  leg2 -> SetFillStyle(0);
  leg2 -> SetLineColor(0);
  leg2 -> SetBorderSize(0);
  //leg2 -> AddEntry(hd_Z0_AsymAmpSqrtVsPt_2016Pr,"Z^{0} stat. uncertainty", "lp");
  leg2 -> AddEntry(hd_Z0_AsymAmpSqrtVsPt_2016Pr,"Z^{0} - \\int L(del.) = 900 pb^{-1}", "lp");
  leg2 -> AddEntry(hd_Z0_AsymAmpSqrtVsPt_2016Pr_400pb,"Z^{0} - \\int L(del.) = 400 pb^{-1}", "lp");
  leg2  -> Draw();

  cZ0AmpProjPt -> Update();  
  cZ0AmpProjPt->SaveAs(outPath + "/hd_Z0Asym2016ProjPt.png");
  cZ0AmpProjPt->SaveAs(outPath + "/hd_Z0Asym2016ProjPt.eps");


  TCanvas *cZ0AmpProjRap   = new TCanvas("cZ0AmpProjRap", "Z0 An 2016 projection Rapidity", 600, 650);

  cZ0AmpProjRap -> SetGrid(0,0);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr -> SetStats(0);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr -> SetTitle("; y^{Z}; A_{N}");
  //hd_Z0_AsymAmpSqrtVsRap_2016Pr -> GetListOfFunctions() -> Add(textStarProj_1w);
  //hd_Z0_AsymAmpSqrtVsRap_2016Pr -> GetListOfFunctions() -> Add(textStarProjBis_1w);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr -> GetYaxis() -> SetRangeUser(-0.49, 0.49);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr -> SetMarkerStyle(20);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr -> SetLineWidth(2);

  //hd_Z0_Rap_2016Pr -> SetMarkerStyle(20);
  //hd_Z0_Rap_2016Pr -> Draw("E1");
  hd_Z0_AsymAmpSqrtVsRap_2016Pr -> Draw("E1");

  hd_Z0_AsymAmpSqrtVsRap_2016Pr_400pb -> SetMarkerStyle(20);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr_400pb -> SetLineWidth(2);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr_400pb -> SetLineStyle(2);
  hd_Z0_AsymAmpSqrtVsRap_2016Pr_400pb -> Draw("same E1");
 
  line1 -> DrawLine(hd_Z0_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmin(), 0, hd_Z0_AsymAmpSqrtVsRap_2016Pr -> GetXaxis() -> GetXmax(), 0);

  leg2  -> Draw();

  cZ0AmpProjRap -> Update();  
  cZ0AmpProjRap->SaveAs(outPath + "/hd_Z0Asym2016ProjRap.png");
  cZ0AmpProjRap->SaveAs(outPath + "/hd_Z0Asym2016ProjRap.eps");


  TCanvas *cZ0AmpProjRap_oneBin   = new TCanvas("cZ0AmpProjRap_oneBin", "Z0 An 2016 projection Rapidity", 600, 650);

  hd_Z0_Asym_2016Pr_oneBin -> GetYaxis() -> SetTitleOffset(1.3);
  hd_Z0_Asym_2016Pr_oneBin -> SetStats(0);
  hd_Z0_Asym_2016Pr_oneBin -> SetTitle("; y^{Z}; A_{N}");
  //hd_Z0_Asym_2016Pr_oneBin -> GetListOfFunctions() -> Add(textStarProj_1w);
  //hd_Z0_Asym_2016Pr_oneBin -> GetListOfFunctions() -> Add(textStarProjBis_1w);
  hd_Z0_Asym_2016Pr_oneBin -> GetYaxis() -> SetRangeUser(-0.49, 0.49);
  hd_Z0_Asym_2016Pr_oneBin -> SetMarkerStyle(20);
  hd_Z0_Asym_2016Pr_oneBin -> SetLineWidth(2);

  hd_Z0_Asym_2016Pr_oneBin -> Draw("E1");
 
  line1 -> DrawLine(hd_Z0_Asym_2016Pr_oneBin -> GetXaxis() -> GetXmin(), 0, hd_Z0_Asym_2016Pr_oneBin -> GetXaxis() -> GetXmax(), 0);

  TLegend *leg2 = new TLegend(0.2, 0.15, 0.7, 0.25);
  leg2 -> SetFillColor(0);
  leg2 -> SetFillStyle(0);
  leg2 -> SetLineColor(0);
  leg2 -> SetBorderSize(0);
  leg2 -> AddEntry(hd_Z0_Asym_2016Pr_oneBin,"Z^{0} stat. uncertainty", "lp");
  leg2  -> Draw();

  cZ0AmpProjRap_oneBin -> Update();  
  cZ0AmpProjRap_oneBin -> SaveAs(outPath + "/hd_Z0Asym2016ProjRap_oneBin.png");
  cZ0AmpProjRap_oneBin -> SaveAs(outPath + "/hd_Z0Asym2016ProjRap_oneBin.eps");

}
