
void WpWmRatio(bool syQCDp = false, bool syQCDm = false, bool syZ0p = false, bool syZ0m = false)
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
   gStyle->SetTitleSize(0.07,"c");
   gStyle->SetTitleSize(0.08,"X");
   gStyle->SetLabelSize(0.08,"X");
   gStyle -> SetEndErrorSize(4);

   // open histogram files
   TFile *fileData11    = TFile::Open(inPathNew + "run11_pp_transverse/hist/vbana.root");
   TFile *fileMCWp11    = TFile::Open(inPathNew + "run11_mc_Wp2enu.lis/hist/vbana.root");
   TFile *fileMCWm_nc11    = TFile::Open(inPathNew + "run11_mc_Wm2enu_Nocuts.lis/hist/vbana.root");
   TFile *fileMCWp_nc11    = TFile::Open(inPathNew + "run11_mc_Wp2enu_Nocuts.lis/hist/vbana.root");
   TFile *fileMCWm11    = TFile::Open(inPathNew + "run11_mc_Wm2enu.lis/hist/vbana.root");
   TFile *fileMCZ11     = TFile::Open(inPathNew + "run11_mc_Z02ee.lis/hist/vbana.root");
   TFile *fileMCWptt11  = TFile::Open(inPathNew + "run11_mc_Wp2taunu.lis/hist/vbana.root");
   TFile *fileMCWmtt11  = TFile::Open(inPathNew + "run11_mc_Wm2taunu.lis/hist/vbana.root");

   TFile *fileData12    = TFile::Open(inPathNew + "run12_pp_long_j3/hist/vbana.root");
   TFile *fileMCWp12    = TFile::Open(inPathNew + "run12_mc_Wp2enu.lis/hist/vbana.root");
   TFile *fileMCWm_nc12    = TFile::Open(inPathNew + "run12_mc_Wm2enu_Nocuts.lis/hist/vbana.root");
   TFile *fileMCWp_nc12    = TFile::Open(inPathNew + "run12_mc_Wp2enu_Nocuts.lis/hist/vbana.root");
   TFile *fileMCWm12    = TFile::Open(inPathNew + "run12_mc_Wm2enu.lis/hist/vbana.root");
   TFile *fileMCZ12     = TFile::Open(inPathNew + "run12_mc_Z2ee.lis/hist/vbana.root");
   TFile *fileMCWptt12  = TFile::Open(inPathNew + "run12_mc_Wp2tautau.lis/hist/vbana.root");
   TFile *fileMCWmtt12  = TFile::Open(inPathNew + "run12_mc_Wm2tautau.lis/hist/vbana.root");

   TFile *fileMattResults        = TFile::Open(outPath + "/DIS2015-RW_Matt.root");
   TH1   *hDIS2015_Matt_EleEta   =(TH1*) fileMattResults  -> Get("hRW");

   // fill histograms run 11 transverse
   TH1 *hd_ZDCrates                        = (TH1*) fileData11   ->Get("event/hZdcRate");
   TH1 *hd_Wp_WBosonPt                     = (TH1*) fileData11   ->Get("event_wp/hWBosonPt");
   TH1 *hd_Wp_WBosonPt_zoomin              = (TH1*) fileData11   ->Get("event_wp/hWBosonPt_zoomin");
   TH1 *hd_Wp_WBosonPz                     = (TH1*) fileData11   ->Get("event_wp/hWBosonPz");
   TH1 *hd_Wp_WBosonEta                    = (TH1*) fileData11   ->Get("event_wp/hWBosonEta");
   TH1 *hd_Wp_QCDEta                       = (TH1 *)fileData11   ->Get("event_qcd_positive/hWBosonEta");
   TH1 *hWp_Wp_WBosonEta                   = (TH1*) fileMCWp11   ->Get("event_wp/hWBosonEta");
   TH1 *hWp_Wp_Gen_WBosonEta               = (TH1*) fileMCWp11   ->Get("event_mc_wp/hWBosonEta_20bins"); // for systematics
   TH1 *hWp_Gen_WBosonEta                  = (TH1*) fileMCWp_nc11   ->Get("event_mc_nocut/hWBosonEta_20bins");
   TH1 *hZ_Wp_WBosonEta                    = (TH1*) fileMCZ11    ->Get("event_wp/hWBosonEta");
   TH1 *hWptn_Wp_WBosonEta                 = (TH1*) fileMCWptt11 ->Get("event_wp/hWBosonEta");

   TH1 *hd_Wp_QCDRapidity                  = (TH1 *)fileData11   ->Get("event_qcd_positive/hWBosonRapidity_5bins");
   TH1 *hd_Wp_WBosonRapidity               = (TH1*) fileData11   ->Get("event_wp/hWBosonRapidity_5bins");
   TH1 *hWp_Wp_WBosonRapidity              = (TH1*) fileMCWp11   ->Get("event_wp/hWBosonRapidity_5bins");
   TH1 *hWp_Wp_Gen_WBosonRapidity          = (TH1*) fileMCWp11   ->Get("event_mc_wp/hWBosonRap_5bins"); // for systematics
   TH1 *hZ_Wp_WBosonRapidity               = (TH1*) fileMCZ11    ->Get("event_wp/hWBosonRapidity_5bins");
   TH1 *hWptn_Wp_WBosonRapidity            = (TH1*) fileMCWptt11 ->Get("event_wp/hWBosonRapidity_5bins");

   TH1 *hd_Wp_ElectronPt                   = (TH1*) fileData11   ->Get("event_wp/hElectronPt");

   TH1 *hd_Wp_ElectronEta                  = (TH1*) fileData11   ->Get("event_wp/hElectronEta_8bins");
        hd_Wp_ElectronEta        -> SaveAs(outPath + "/4Matt_Wplus_run11_ElectronEta.root");
   TH1 *hd_Wp_QCDElectronEta               = (TH1 *)fileData11   ->Get("event_qcd_positive/hElectronEta_8bins");
   TH1 *hWp_Wp_ElectronEta                 = (TH1*) fileMCWp11   ->Get("event_wp/hElectronEta_8bins");
   TH1 *hWp_Gen_ElectronEta                = (TH1*) fileMCWp_nc11   ->Get("event_mc_nocut/hLeptonEta_8bins");
   TH1 *hZ_Wp_ElectronEta                  = (TH1*) fileMCZ11    ->Get("event_wp/hElectronEta_8bins");
   TH1 *hWptn_Wp_ElectronEta               = (TH1*) fileMCWptt11 ->Get("event_wp/hElectronEta_8bins");
   TH2 *hWp_GenVsRec_ElectronEta           = (TH2*) fileMCWp11   ->Get("event_mc_wp/hRecoVsGenLeptonEta_8bins");

   /*
   TH1 *hd_Wp_ElectronEta                  = (TH1*) fileData11   ->Get("event_wp/hElectronEta");
   TH1 *hd_Wp_QCDElectronEta               = (TH1 *)fileData11   ->Get("event_qcd_positive/hElectronEta");
   TH1 *hWp_Wp_ElectronEta                 = (TH1*) fileMCWp11   ->Get("event_wp/hElectronEta");
   TH1 *hWp_Gen_ElectronEta                = (TH1*) fileMCWp_nc11   ->Get("event_mc_nocut/hLeptonEta");
   TH1 *hZ_Wp_ElectronEta                  = (TH1*) fileMCZ11    ->Get("event_wp/hElectronEta");
   TH1 *hWptn_Wp_ElectronEta               = (TH1*) fileMCWptt11 ->Get("event_wp/hElectronEta");
   TH2 *hWp_GenVsRec_ElectronEta           = (TH2*) fileMCWp11   ->Get("event_mc_wp/hRecoVsGenLeptonEta_10bins");
   */

   TH1 *hd_Wm_WBosonPt                     = (TH1*) fileData11   ->Get("event_wm/hWBosonPt");
   TH1 *hd_Wm_WBosonPt_zoomin              = (TH1*) fileData11   ->Get("event_wm/hWBosonPt_zoomin");
   TH1 *hd_Wm_WBosonPz                     = (TH1*) fileData11   ->Get("event_wm/hWBosonPz");
   TH1 *hd_Wm_WBosonEta                    = (TH1*) fileData11   ->Get("event_wm/hWBosonEta");
   TH1 *hd_Wm_QCDEta                       = (TH1 *)fileData11   ->Get("event_qcd_negative/hWBosonEta");
   TH1 *hWm_Wm_WBosonEta                   = (TH1*) fileMCWm11   ->Get("event_wm/hWBosonEta");
   TH1 *hWm_Wm_Gen_WBosonEta               = (TH1*) fileMCWm11   ->Get("event_mc_wm/hWBosonEta_20bins"); // for systematics
   TH1 *hWm_Gen_WBosonEta                  = (TH1*) fileMCWm_nc11   ->Get("event_mc_nocut/hWBosonEta_20bins");
   TH1 *hZ_Wm_WBosonEta                    = (TH1*) fileMCZ11    ->Get("event_wm/hWBosonEta");
   TH1 *hWmtn_Wm_WBosonEta                 = (TH1*) fileMCWmtt11 ->Get("event_wm/hWBosonEta");

   TH1 *hd_Wm_QCDRapidity                  = (TH1 *)fileData11   ->Get("event_qcd_negative/hWBosonRapidity_5bins");
   TH1 *hd_Wm_WBosonRapidity               = (TH1*) fileData11   ->Get("event_wm/hWBosonRapidity_5bins");
   TH1 *hWm_Wm_WBosonRapidity              = (TH1*) fileMCWm11   ->Get("event_wm/hWBosonRapidity_5bins");
   TH1 *hWm_Wm_Gen_WBosonRapidity          = (TH1*) fileMCWm11   ->Get("event_mc_wm/hWBosonRap_5bins"); // for systematics
   TH1 *hZ_Wm_WBosonRapidity               = (TH1*) fileMCZ11    ->Get("event_wm/hWBosonRapidity_5bins");
   TH1 *hWmtn_Wm_WBosonRapidity            = (TH1*) fileMCWmtt11 ->Get("event_wm/hWBosonRapidity_5bins");

   TH1 *hd_Wm_ElectronPt                   = (TH1*) fileData11   ->Get("event_wm/hElectronPt");

   TH1 *hd_Wm_ElectronEta                  = (TH1*) fileData11   ->Get("event_wm/hElectronEta_8bins");
        hd_Wm_ElectronEta        -> SaveAs(outPath + "/4Matt_Wminus_run11_ElectronEta.root");
   TH1 *hd_Wm_QCDElectronEta               = (TH1 *)fileData11   ->Get("event_qcd_negative/hElectronEta_8bins");
   TH1 *hWm_Wm_ElectronEta                 = (TH1*) fileMCWm11   ->Get("event_wm/hElectronEta_8bins");
   TH1 *hWm_Gen_ElectronEta                = (TH1*) fileMCWm_nc11   ->Get("event_mc_nocut/hLeptonEta_8bins");
   TH1 *hZ_Wm_ElectronEta                  = (TH1*) fileMCZ11    ->Get("event_wm/hElectronEta_8bins");
   TH1 *hWmtn_Wm_ElectronEta               = (TH1*) fileMCWmtt11 ->Get("event_wm/hElectronEta_8bins");
   TH2 *hWm_GenVsRec_ElectronEta           = (TH2*) fileMCWm11   ->Get("event_mc_wm/hRecoVsGenLeptonEta_8bins");

   /*
   TH1 *hd_Wm_ElectronEta                  = (TH1*) fileData11   ->Get("event_wm/hElectronEta");
   TH1 *hd_Wm_QCDElectronEta               = (TH1 *)fileData11   ->Get("event_qcd_negative/hElectronEta");
   TH1 *hWm_Wm_ElectronEta                 = (TH1*) fileMCWm11   ->Get("event_wm/hElectronEta");
   TH1 *hWm_Gen_ElectronEta                = (TH1*) fileMCWm_nc11   ->Get("event_mc_nocut/hLeptonEta");
   TH1 *hZ_Wm_ElectronEta                  = (TH1*) fileMCZ11    ->Get("event_wm/hElectronEta");
   TH1 *hWmtn_Wm_ElectronEta               = (TH1*) fileMCWmtt11 ->Get("event_wm/hElectronEta");
   TH2 *hWm_GenVsRec_ElectronEta           = (TH2*) fileMCWm11   ->Get("event_mc_wm/hRecoVsGenLeptonEta_10bins");
   */

   // fill histograms run 12
   TH1 *hd12_ZDCrates                      = (TH1*) fileData12   ->Get("event/hZdcRate");
   TH1 *hd12_Wp_WBosonPt                   = (TH1*) fileData12   ->Get("event_wp/hWBosonPt");
   TH1 *hd12_Wp_WBosonPt_zoomin            = (TH1*) fileData12   ->Get("event_wp/hWBosonPt_zoomin");
   TH1 *hd12_Wp_WBosonPz                   = (TH1*) fileData12   ->Get("event_wp/hWBosonPz");
   TH1 *hd12_Wp_WBosonEta                  = (TH1*) fileData12   ->Get("event_wp/hWBosonEta");
   TH1 *hd12_Wp_QCDEta                     = (TH1 *)fileData12   ->Get("event_qcd_positive/hWBosonEta");
   TH1 *hWp12_Wp_WBosonEta                 = (TH1*) fileMCWp12   ->Get("event_wp/hWBosonEta");
   TH1 *hWp12_Gen_WBosonEta                = (TH1*) fileMCWp_nc12   ->Get("event_mc_nocut/hWBosonEta_20bins");
   TH1 *hZ12_Wp_WBosonEta                  = (TH1*) fileMCZ12    ->Get("event_wp/hWBosonEta");
   TH1 *hWptn12_Wp_WBosonEta               = (TH1*) fileMCWptt12 ->Get("event_wp/hWBosonEta");

   TH1 *hd12_Wp_QCDRapidity                = (TH1 *)fileData12   ->Get("event_qcd_positive/hWBosonRapidity_5bins");
   TH1 *hd12_Wp_WBosonRapidity             = (TH1*) fileData12   ->Get("event_wp/hWBosonRapidity_5bins");
   TH1 *hWp12_Wp_WBosonRapidity            = (TH1*) fileMCWp12   ->Get("event_wp/hWBosonRapidity_5bins");
   TH1 *hWp12_Wp_Gen_WBosonRapidity        = (TH1*) fileMCWp12   ->Get("event_mc_wp/hWBosonRap_5bins"); // for systematics
   TH1 *hZ12_Wp_WBosonRapidity             = (TH1*) fileMCZ12    ->Get("event_wp/hWBosonRapidity_5bins");
   TH1 *hWptn12_Wp_WBosonRapidity          = (TH1*) fileMCWptt12 ->Get("event_wp/hWBosonRapidity_5bins");

   TH1 *hd12_Wp_ElectronPt                 = (TH1*) fileData12   ->Get("event_wp/hElectronPt");

   TH1 *hd12_Wp_ElectronEta                = (TH1*) fileData12   ->Get("event_wp/hElectronEta_8bins");
        hd12_Wp_ElectronEta        -> SaveAs(outPath + "/4Matt_Wplus_run12_ElectronEta.root");
   TH1 *hd12_Wp_QCDElectronEta             = (TH1 *)fileData12   ->Get("event_qcd_positive/hElectronEta_8bins");
   TH1 *hWp12_Wp_ElectronEta               = (TH1*) fileMCWp12   ->Get("event_wp/hElectronEta_8bins");
   TH1 *hWp12_Gen_ElectronEta              = (TH1*) fileMCWp_nc12   ->Get("event_mc_nocut/hLeptonEta_8bins");
   TH1 *hZ12_Wp_ElectronEta                = (TH1*) fileMCZ12    ->Get("event_wp/hElectronEta_8bins");
   TH1 *hWptn12_Wp_ElectronEta             = (TH1*) fileMCWptt12 ->Get("event_wp/hElectronEta_8bins");
   TH2 *hWp12_GenVsRec_ElectronEta         = (TH2*) fileMCWp12   ->Get("event_mc_wp/hRecoVsGenLeptonEta_8bins");

   /*
   TH1 *hd12_Wp_ElectronEta                = (TH1*) fileData12   ->Get("event_wp/hElectronEta");
   TH1 *hd12_Wp_QCDElectronEta             = (TH1 *)fileData12   ->Get("event_qcd_positive/hElectronEta");
   TH1 *hWp12_Wp_ElectronEta               = (TH1*) fileMCWp12   ->Get("event_wp/hElectronEta");
   TH1 *hWp12_Gen_ElectronEta              = (TH1*) fileMCWp_nc12   ->Get("event_mc_nocut/hLeptonEta");
   TH1 *hZ12_Wp_ElectronEta                = (TH1*) fileMCZ12    ->Get("event_wp/hElectronEta");
   TH1 *hWptn12_Wp_ElectronEta             = (TH1*) fileMCWptt12 ->Get("event_wp/hElectronEta");
   TH2 *hWp12_GenVsRec_ElectronEta         = (TH2*) fileMCWp12   ->Get("event_mc_wp/hRecoVsGenLeptonEta_10bins");
   */

   TH1 *hd12_Wm_WBosonPt                   = (TH1*) fileData12   ->Get("event_wm/hWBosonPt");
   TH1 *hd12_Wm_WBosonPt_zoomin            = (TH1*) fileData12   ->Get("event_wm/hWBosonPt_zoomin");
   TH1 *hd12_Wm_WBosonPz                   = (TH1*) fileData12   ->Get("event_wm/hWBosonPz");
   TH1 *hd12_Wm_WBosonEta                  = (TH1*) fileData12   ->Get("event_wm/hWBosonEta");
   TH1 *hd12_Wm_QCDEta                     = (TH1 *)fileData12   ->Get("event_qcd_negative/hWBosonEta");
   TH1 *hWm12_Wm_WBosonEta                 = (TH1*) fileMCWm12   ->Get("event_wm/hWBosonEta");
   TH1 *hWm12_Gen_WBosonEta                = (TH1*) fileMCWm_nc12   ->Get("event_mc_nocut/hWBosonEta_20bins");
   TH1 *hZ12_Wm_WBosonEta                  = (TH1*) fileMCZ12    ->Get("event_wm/hWBosonEta");
   TH1 *hWmtn12_Wm_WBosonEta               = (TH1*) fileMCWmtt12 ->Get("event_wm/hWBosonEta");

   TH1 *hd12_Wm_QCDRapidity                = (TH1 *)fileData12   ->Get("event_qcd_negative/hWBosonRapidity_5bins");
   TH1 *hd12_Wm_WBosonRapidity             = (TH1*) fileData12   ->Get("event_wm/hWBosonRapidity_5bins");
   TH1 *hWm12_Wm_WBosonRapidity            = (TH1*) fileMCWm12   ->Get("event_wm/hWBosonRapidity_5bins");
   TH1 *hWm12_Wm_Gen_WBosonRapidity        = (TH1*) fileMCWm12   ->Get("event_mc_wm/hWBosonRap_5bins"); // for systematics
   TH1 *hZ12_Wm_WBosonRapidity             = (TH1*) fileMCZ12    ->Get("event_wm/hWBosonRapidity_5bins");
   TH1 *hWmtn12_Wm_WBosonRapidity          = (TH1*) fileMCWmtt12 ->Get("event_wm/hWBosonRapidity_5bins");

   TH1 *hd12_Wm_ElectronPt                 = (TH1*) fileData12   ->Get("event_wm/hElectronPt");

   TH1 *hd12_Wm_ElectronEta                = (TH1*) fileData12   ->Get("event_wm/hElectronEta_8bins");
        hd12_Wm_ElectronEta        -> SaveAs(outPath + "/4Matt_Wminus_run12_ElectronEta.root");
   TH1 *hd12_Wm_QCDElectronEta             = (TH1 *)fileData12   ->Get("event_qcd_negative/hElectronEta_8bins");
   TH1 *hWm12_Wm_ElectronEta               = (TH1*) fileMCWm12   ->Get("event_wm/hElectronEta_8bins");
   TH1 *hWm12_Gen_ElectronEta              = (TH1*) fileMCWm_nc12   ->Get("event_mc_nocut/hLeptonEta_8bins");
   TH1 *hZ12_Wm_ElectronEta                = (TH1*) fileMCZ12    ->Get("event_wm/hElectronEta_8bins");
   TH1 *hWmtn12_Wm_ElectronEta             = (TH1*) fileMCWmtt12 ->Get("event_wm/hElectronEta_8bins");
   TH2 *hWm12_GenVsRec_ElectronEta         = (TH2*) fileMCWm12   ->Get("event_mc_wm/hRecoVsGenLeptonEta_8bins");

   /*
   TH1 *hd12_Wm_ElectronEta                = (TH1*) fileData12   ->Get("event_wm/hElectronEta");
   TH1 *hd12_Wm_QCDElectronEta             = (TH1 *)fileData12   ->Get("event_qcd_negative/hElectronEta");
   TH1 *hWm12_Wm_ElectronEta               = (TH1*) fileMCWm12   ->Get("event_wm/hElectronEta");
   TH1 *hWm12_Gen_ElectronEta              = (TH1*) fileMCWm_nc12   ->Get("event_mc_nocut/hLeptonEta");
   TH1 *hZ12_Wm_ElectronEta                = (TH1*) fileMCZ12    ->Get("event_wm/hElectronEta");
   TH1 *hWmtn12_Wm_ElectronEta             = (TH1*) fileMCWmtt12 ->Get("event_wm/hElectronEta");
   TH2 *hWm12_GenVsRec_ElectronEta         = (TH2*) fileMCWm12   ->Get("event_mc_wm/hRecoVsGenLeptonEta_10bins");
   */

   // rebin
   hd_Wp_WBosonEta        -> Rebin(4);
   hd_Wm_WBosonEta        -> Rebin(4);
   hd_Wp_QCDEta           -> Rebin(4);
   hd_Wm_QCDEta           -> Rebin(4);
   hWp_Wp_WBosonEta       -> Rebin(4);
   hWm_Wm_WBosonEta       -> Rebin(4);
   hWp_Wp_Gen_WBosonEta   -> Rebin(4);
   hWm_Wm_Gen_WBosonEta   -> Rebin(4);
   hWp_Gen_WBosonEta      -> Rebin(4);
   hWm_Gen_WBosonEta      -> Rebin(4);
   hZ_Wp_WBosonEta        -> Rebin(4);
   hZ_Wm_WBosonEta        -> Rebin(4);
   hWptn_Wp_WBosonEta     -> Rebin(4);
   hWmtn_Wm_WBosonEta     -> Rebin(4);
   hd12_Wp_WBosonEta      -> Rebin(4);
   hd12_Wm_WBosonEta      -> Rebin(4);
   hd12_Wp_QCDEta         -> Rebin(4);
   hd12_Wm_QCDEta         -> Rebin(4);
   hWp12_Wp_WBosonEta     -> Rebin(4);
   hWm12_Wm_WBosonEta     -> Rebin(4);
   hWp12_Gen_WBosonEta    -> Rebin(4);
   hWm12_Gen_WBosonEta    -> Rebin(4);
   hZ12_Wp_WBosonEta      -> Rebin(4);
   hZ12_Wm_WBosonEta      -> Rebin(4);
   hWptn12_Wp_WBosonEta   -> Rebin(4);
   hWmtn12_Wm_WBosonEta   -> Rebin(4);

   /*
   hd_Wp_ElectronEta      -> Rebin(2);
   hd_Wm_ElectronEta      -> Rebin(2);
   hd_Wp_QCDElectronEta   -> Rebin(2);
   hd_Wm_QCDElectronEta   -> Rebin(2);
   hWp_Wp_ElectronEta     -> Rebin(2);
   hWm_Wm_ElectronEta     -> Rebin(2);
   hWp_Gen_ElectronEta    -> Rebin(2);
   hWm_Gen_ElectronEta    -> Rebin(2);
   hZ_Wp_ElectronEta      -> Rebin(2);
   hZ_Wm_ElectronEta      -> Rebin(2);
   hWptn_Wp_ElectronEta   -> Rebin(2);
   hWmtn_Wm_ElectronEta   -> Rebin(2);
   hd12_Wp_ElectronEta    -> Rebin(2);
   hd12_Wm_ElectronEta    -> Rebin(2);
   hd12_Wp_QCDElectronEta -> Rebin(2);
   hd12_Wm_QCDElectronEta -> Rebin(2);
   hWp12_Wp_ElectronEta   -> Rebin(2);
   hWm12_Wm_ElectronEta   -> Rebin(2);
   hWp12_Gen_ElectronEta  -> Rebin(2);
   hWm12_Gen_ElectronEta  -> Rebin(2);
   hZ12_Wp_ElectronEta    -> Rebin(2);
   hZ12_Wm_ElectronEta    -> Rebin(2);
   hWptn12_Wp_ElectronEta -> Rebin(2);
   hWmtn12_Wm_ElectronEta -> Rebin(2);
   */

   // EFFICIENCY STUDIES

   // We calculate the efficiency per each bin using MC as: eff= Reco/Gen
   // we want to see if there is any charge dependence

   // Test ------------------ 
   TCanvas *cTest    = new TCanvas("cTest", "Efficiency studies", 800, 800);
   cTest -> Divide(1,4);
   cTest_1 -> cd();
   hWp_GenVsRec_ElectronEta -> SetTitle("Run 11");
   hWp_GenVsRec_ElectronEta -> SetStats(0);
   hWp_GenVsRec_ElectronEta -> Draw(); 
   cTest_2-> cd();
   hWp_Gen_ElectronEta      -> SetTitle("Run 11");
   hWp_Gen_ElectronEta      -> Draw();

   Double_t Wp11_NumRecoInGenEtaBin1 = hWp_GenVsRec_ElectronEta -> GetBinContent(1,1);
   Double_t Wp11_NumGenEtaBin1       = hWp_Gen_ElectronEta      -> GetBinContent(1);
   cout << "Wp11_NumRecoInGenEtaBin1 = " << Wp11_NumRecoInGenEtaBin1 << "   Wp11_NumGenEtaBin1 = " << Wp11_NumGenEtaBin1 << endl;
   Double_t Wp11_NumRecoInGenEtaBin2 = hWp_GenVsRec_ElectronEta -> GetBinContent(2,2);
   Double_t Wp11_NumGenEtaBin2       = hWp_Gen_ElectronEta      -> GetBinContent(2);
   cout << "Wp11_NumRecoInGenEtaBin2 = " << Wp11_NumRecoInGenEtaBin2 << "   Wp11_NumGenEtaBin2 = " << Wp11_NumGenEtaBin2  << endl;
   Double_t Wp11_NumRecoInGenEtaBin3 = hWp_GenVsRec_ElectronEta -> GetBinContent(3,3);
   Double_t Wp11_NumGenEtaBin3       = hWp_Gen_ElectronEta      -> GetBinContent(3);
   cout << "Wp11_NumRecoInGenEtaBin3 = " << Wp11_NumRecoInGenEtaBin3 << "   Wp11_NumGenEtaBin3 = " << Wp11_NumGenEtaBin3  << endl;
   Double_t Wp11_NumRecoInGenEtaBin4 = hWp_GenVsRec_ElectronEta -> GetBinContent(4,4);
   Double_t Wp11_NumGenEtaBin4       = hWp_Gen_ElectronEta      -> GetBinContent(4);
   cout << "Wp11_NumRecoInGenEtaBin4 = " << Wp11_NumRecoInGenEtaBin4 << "   Wp11_NumGenEtaBin4 = " << Wp11_NumGenEtaBin4  << endl;
   Double_t Wp11_NumRecoInGenEtaBin5 = hWp_GenVsRec_ElectronEta -> GetBinContent(5,5);
   Double_t Wp11_NumGenEtaBin5       = hWp_Gen_ElectronEta      -> GetBinContent(5);
   cout << "Wp11_NumRecoInGenEtaBin5 = " << Wp11_NumRecoInGenEtaBin5 << "   Wp11_NumGenEtaBin5 = " << Wp11_NumGenEtaBin5  << endl;
   Double_t Wp11_NumRecoInGenEtaBin6 = hWp_GenVsRec_ElectronEta -> GetBinContent(6,6);
   Double_t Wp11_NumGenEtaBin6       = hWp_Gen_ElectronEta      -> GetBinContent(6);
   cout << "Wp11_NumRecoInGenEtaBin6 = " << Wp11_NumRecoInGenEtaBin6 << "   Wp11_NumGenEtaBin6 = " << Wp11_NumGenEtaBin6  << endl;
   Double_t Wp11_NumRecoInGenEtaBin7 = hWp_GenVsRec_ElectronEta -> GetBinContent(7,7);
   Double_t Wp11_NumGenEtaBin7       = hWp_Gen_ElectronEta      -> GetBinContent(7);
   cout << "Wp11_NumRecoInGenEtaBin7 = " << Wp11_NumRecoInGenEtaBin7 << "   Wp11_NumGenEtaBin7 = " << Wp11_NumGenEtaBin7  << endl;
   Double_t Wp11_NumRecoInGenEtaBin8 = hWp_GenVsRec_ElectronEta -> GetBinContent(8,8);
   Double_t Wp11_NumGenEtaBin8       = hWp_Gen_ElectronEta      -> GetBinContent(8);
   cout << "Wp11_NumRecoInGenEtaBin8 = " << Wp11_NumRecoInGenEtaBin8 << "   Wp11_NumGenEtaBin8 = " << Wp11_NumGenEtaBin8  << endl;

   Double_t Wp11_effGenEtaBin1 = Wp11_NumRecoInGenEtaBin1 / Wp11_NumGenEtaBin1;
   Double_t Wp11_effGenEtaBin2 = Wp11_NumRecoInGenEtaBin2 / Wp11_NumGenEtaBin2;
   Double_t Wp11_effGenEtaBin3 = Wp11_NumRecoInGenEtaBin3 / Wp11_NumGenEtaBin3;
   Double_t Wp11_effGenEtaBin4 = Wp11_NumRecoInGenEtaBin4 / Wp11_NumGenEtaBin4;
   Double_t Wp11_effGenEtaBin5 = Wp11_NumRecoInGenEtaBin5 / Wp11_NumGenEtaBin5;
   Double_t Wp11_effGenEtaBin6 = Wp11_NumRecoInGenEtaBin6 / Wp11_NumGenEtaBin6;
   Double_t Wp11_effGenEtaBin7 = Wp11_NumRecoInGenEtaBin7 / Wp11_NumGenEtaBin7;
   Double_t Wp11_effGenEtaBin8 = Wp11_NumRecoInGenEtaBin8 / Wp11_NumGenEtaBin8;
   cout << "Wp11_effGenEtaBin1 = " << Wp11_effGenEtaBin1 << endl;
   cout << "Wp11_effGenEtaBin2 = " << Wp11_effGenEtaBin2 << endl;
   cout << "Wp11_effGenEtaBin3 = " << Wp11_effGenEtaBin3 << endl;
   cout << "Wp11_effGenEtaBin4 = " << Wp11_effGenEtaBin4 << endl;
   cout << "Wp11_effGenEtaBin5 = " << Wp11_effGenEtaBin5 << endl;
   cout << "Wp11_effGenEtaBin6 = " << Wp11_effGenEtaBin6 << endl;
   cout << "Wp11_effGenEtaBin7 = " << Wp11_effGenEtaBin7 << endl;
   cout << "Wp11_effGenEtaBin8 = " << Wp11_effGenEtaBin8 << endl;
   // calculate uncertainties using the binomial formula
   Double_t Wp11_kBin1 = Wp11_effGenEtaBin1 * Wp11_NumGenEtaBin1;
   Double_t Wp11_NBin1 = Wp11_NumGenEtaBin1;
   Double_t Wp11_effGenEtaBin1_err = (1 / Wp11_NBin1) * sqrt(Wp11_kBin1 * (1 - Wp11_kBin1 / Wp11_NBin1) );
   Double_t Wp11_kBin2 = Wp11_effGenEtaBin2 * Wp11_NumGenEtaBin2;
   Double_t Wp11_NBin2 = Wp11_NumGenEtaBin2;
   Double_t Wp11_effGenEtaBin2_err = (1 / Wp11_NBin2) * sqrt(Wp11_kBin2 * (1 - Wp11_kBin2 / Wp11_NBin2) );
   Double_t Wp11_kBin3 = Wp11_effGenEtaBin3 * Wp11_NumGenEtaBin3;
   Double_t Wp11_NBin3 = Wp11_NumGenEtaBin3;
   Double_t Wp11_effGenEtaBin3_err = (1 / Wp11_NBin3) * sqrt(Wp11_kBin3 * (1 - Wp11_kBin3 / Wp11_NBin3) );
   Double_t Wp11_kBin4 = Wp11_effGenEtaBin4 * Wp11_NumGenEtaBin4;
   Double_t Wp11_NBin4 = Wp11_NumGenEtaBin4;
   Double_t Wp11_effGenEtaBin4_err = (1 / Wp11_NBin4) * sqrt(Wp11_kBin4 * (1 - Wp11_kBin4 / Wp11_NBin4) );
   Double_t Wp11_kBin5 = Wp11_effGenEtaBin5 * Wp11_NumGenEtaBin5;
   Double_t Wp11_NBin5 = Wp11_NumGenEtaBin5;
   Double_t Wp11_effGenEtaBin5_err = (1 / Wp11_NBin5) * sqrt(Wp11_kBin5 * (1 - Wp11_kBin5 / Wp11_NBin5) );
   Double_t Wp11_kBin6 = Wp11_effGenEtaBin6 * Wp11_NumGenEtaBin6;
   Double_t Wp11_NBin6 = Wp11_NumGenEtaBin6;
   Double_t Wp11_effGenEtaBin6_err = (1 / Wp11_NBin6) * sqrt(Wp11_kBin6 * (1 - Wp11_kBin6 / Wp11_NBin6) );
   Double_t Wp11_kBin7 = Wp11_effGenEtaBin7 * Wp11_NumGenEtaBin7;
   Double_t Wp11_NBin7 = Wp11_NumGenEtaBin7;
   Double_t Wp11_effGenEtaBin7_err = (1 / Wp11_NBin7) * sqrt(Wp11_kBin7 * (1 - Wp11_kBin7 / Wp11_NBin7) );
   Double_t Wp11_kBin8 = Wp11_effGenEtaBin8 * Wp11_NumGenEtaBin8;
   Double_t Wp11_NBin8 = Wp11_NumGenEtaBin8;
   Double_t Wp11_effGenEtaBin8_err = (1 / Wp11_NBin8) * sqrt(Wp11_kBin8 * (1 - Wp11_kBin8 / Wp11_NBin8) );
   cout << "Wp11_effGenEtaBin1_err = " << Wp11_effGenEtaBin1_err << endl;
   cout << "Wp11_effGenEtaBin2_err = " << Wp11_effGenEtaBin2_err << endl;
   cout << "Wp11_effGenEtaBin3_err = " << Wp11_effGenEtaBin3_err << endl;
   cout << "Wp11_effGenEtaBin4_err = " << Wp11_effGenEtaBin4_err << endl;
   cout << "Wp11_effGenEtaBin5_err = " << Wp11_effGenEtaBin5_err << endl;
   cout << "Wp11_effGenEtaBin6_err = " << Wp11_effGenEtaBin6_err << endl;
   cout << "Wp11_effGenEtaBin7_err = " << Wp11_effGenEtaBin7_err << endl;
   cout << "Wp11_effGenEtaBin8_err = " << Wp11_effGenEtaBin8_err << endl;


   Double_t Wm11_NumRecoInGenEtaBin1 = hWm_GenVsRec_ElectronEta -> GetBinContent(1,1);
   Double_t Wm11_NumGenEtaBin1       = hWm_Gen_ElectronEta      -> GetBinContent(1);
   cout << "Wm11_NumRecoInGenEtaBin1 = " << Wm11_NumRecoInGenEtaBin1 << "   Wm11_NumGenEtaBin1 = " << Wm11_NumGenEtaBin1 << endl;
   Double_t Wm11_NumRecoInGenEtaBin2 = hWm_GenVsRec_ElectronEta -> GetBinContent(2,2);
   Double_t Wm11_NumGenEtaBin2       = hWm_Gen_ElectronEta      -> GetBinContent(2);
   cout << "Wm11_NumRecoInGenEtaBin2 = " << Wm11_NumRecoInGenEtaBin2 << "   Wm11_NumGenEtaBin2 = " << Wm11_NumGenEtaBin2  << endl;
   Double_t Wm11_NumRecoInGenEtaBin3 = hWm_GenVsRec_ElectronEta -> GetBinContent(3,3);
   Double_t Wm11_NumGenEtaBin3       = hWm_Gen_ElectronEta      -> GetBinContent(3);
   cout << "Wm11_NumRecoInGenEtaBin3 = " << Wm11_NumRecoInGenEtaBin3 << "   Wm11_NumGenEtaBin3 = " << Wm11_NumGenEtaBin3  << endl;
   Double_t Wm11_NumRecoInGenEtaBin4 = hWm_GenVsRec_ElectronEta -> GetBinContent(4,4);
   Double_t Wm11_NumGenEtaBin4       = hWm_Gen_ElectronEta      -> GetBinContent(4);
   cout << "Wm11_NumRecoInGenEtaBin4 = " << Wm11_NumRecoInGenEtaBin4 << "   Wm11_NumGenEtaBin4 = " << Wm11_NumGenEtaBin4  << endl;
   Double_t Wm11_NumRecoInGenEtaBin5 = hWm_GenVsRec_ElectronEta -> GetBinContent(5,5);
   Double_t Wm11_NumGenEtaBin5       = hWm_Gen_ElectronEta      -> GetBinContent(5);
   cout << "Wm11_NumRecoInGenEtaBin5 = " << Wm11_NumRecoInGenEtaBin5 << "   Wm11_NumGenEtaBin5 = " << Wm11_NumGenEtaBin5  << endl;
   Double_t Wm11_NumRecoInGenEtaBin6 = hWm_GenVsRec_ElectronEta -> GetBinContent(6,6);
   Double_t Wm11_NumGenEtaBin6       = hWm_Gen_ElectronEta      -> GetBinContent(6);
   cout << "Wm11_NumRecoInGenEtaBin6 = " << Wm11_NumRecoInGenEtaBin6 << "   Wm11_NumGenEtaBin6 = " << Wm11_NumGenEtaBin6  << endl;
   Double_t Wm11_NumRecoInGenEtaBin7 = hWm_GenVsRec_ElectronEta -> GetBinContent(7,7);
   Double_t Wm11_NumGenEtaBin7       = hWm_Gen_ElectronEta      -> GetBinContent(7);
   cout << "Wm11_NumRecoInGenEtaBin7 = " << Wm11_NumRecoInGenEtaBin7 << "   Wm11_NumGenEtaBin7 = " << Wm11_NumGenEtaBin7  << endl;
   Double_t Wm11_NumRecoInGenEtaBin8 = hWm_GenVsRec_ElectronEta -> GetBinContent(8,8);
   Double_t Wm11_NumGenEtaBin8       = hWm_Gen_ElectronEta      -> GetBinContent(8);
   cout << "Wm11_NumRecoInGenEtaBin8 = " << Wm11_NumRecoInGenEtaBin8 << "   Wm11_NumGenEtaBin8 = " << Wm11_NumGenEtaBin8  << endl;

   Double_t Wm11_effGenEtaBin1 = Wm11_NumRecoInGenEtaBin1 / Wm11_NumGenEtaBin1;
   Double_t Wm11_effGenEtaBin2 = Wm11_NumRecoInGenEtaBin2 / Wm11_NumGenEtaBin2;
   Double_t Wm11_effGenEtaBin3 = Wm11_NumRecoInGenEtaBin3 / Wm11_NumGenEtaBin3;
   Double_t Wm11_effGenEtaBin4 = Wm11_NumRecoInGenEtaBin4 / Wm11_NumGenEtaBin4;
   Double_t Wm11_effGenEtaBin5 = Wm11_NumRecoInGenEtaBin5 / Wm11_NumGenEtaBin5;
   Double_t Wm11_effGenEtaBin6 = Wm11_NumRecoInGenEtaBin6 / Wm11_NumGenEtaBin6;
   Double_t Wm11_effGenEtaBin7 = Wm11_NumRecoInGenEtaBin7 / Wm11_NumGenEtaBin7;
   Double_t Wm11_effGenEtaBin8 = Wm11_NumRecoInGenEtaBin8 / Wm11_NumGenEtaBin8;
   cout << "Wm11_effGenEtaBin1 = " << Wm11_effGenEtaBin1 << endl;
   cout << "Wm11_effGenEtaBin2 = " << Wm11_effGenEtaBin2 << endl;
   cout << "Wm11_effGenEtaBin3 = " << Wm11_effGenEtaBin3 << endl;
   cout << "Wm11_effGenEtaBin4 = " << Wm11_effGenEtaBin4 << endl;
   cout << "Wm11_effGenEtaBin5 = " << Wm11_effGenEtaBin5 << endl;
   cout << "Wm11_effGenEtaBin6 = " << Wm11_effGenEtaBin6 << endl;
   cout << "Wm11_effGenEtaBin7 = " << Wm11_effGenEtaBin7 << endl;
   cout << "Wm11_effGenEtaBin8 = " << Wm11_effGenEtaBin8 << endl;
   // calculate uncertainties using the binomial formula
   Double_t Wm11_kBin1 = Wm11_effGenEtaBin1 * Wm11_NumGenEtaBin1;
   Double_t Wm11_NBin1 = Wm11_NumGenEtaBin1;
   Double_t Wm11_effGenEtaBin1_err = (1 / Wm11_NBin1) * sqrt(Wm11_kBin1 * (1 - Wm11_kBin1 / Wm11_NBin1) );
   Double_t Wm11_kBin2 = Wm11_effGenEtaBin2 * Wm11_NumGenEtaBin2;
   Double_t Wm11_NBin2 = Wm11_NumGenEtaBin2;
   Double_t Wm11_effGenEtaBin2_err = (1 / Wm11_NBin2) * sqrt(Wm11_kBin2 * (1 - Wm11_kBin2 / Wm11_NBin2) );
   Double_t Wm11_kBin3 = Wm11_effGenEtaBin3 * Wm11_NumGenEtaBin3;
   Double_t Wm11_NBin3 = Wm11_NumGenEtaBin3;
   Double_t Wm11_effGenEtaBin3_err = (1 / Wm11_NBin3) * sqrt(Wm11_kBin3 * (1 - Wm11_kBin3 / Wm11_NBin3) );
   Double_t Wm11_kBin4 = Wm11_effGenEtaBin4 * Wm11_NumGenEtaBin4;
   Double_t Wm11_NBin4 = Wm11_NumGenEtaBin4;
   Double_t Wm11_effGenEtaBin4_err = (1 / Wm11_NBin4) * sqrt(Wm11_kBin4 * (1 - Wm11_kBin4 / Wm11_NBin4) );
   Double_t Wm11_kBin5 = Wm11_effGenEtaBin5 * Wm11_NumGenEtaBin5;
   Double_t Wm11_NBin5 = Wm11_NumGenEtaBin5;
   Double_t Wm11_effGenEtaBin5_err = (1 / Wm11_NBin5) * sqrt(Wm11_kBin5 * (1 - Wm11_kBin5 / Wm11_NBin5) );
   Double_t Wm11_kBin6 = Wm11_effGenEtaBin6 * Wm11_NumGenEtaBin6;
   Double_t Wm11_NBin6 = Wm11_NumGenEtaBin6;
   Double_t Wm11_effGenEtaBin6_err = (1 / Wm11_NBin6) * sqrt(Wm11_kBin6 * (1 - Wm11_kBin6 / Wm11_NBin6) );
   Double_t Wm11_kBin7 = Wm11_effGenEtaBin7 * Wm11_NumGenEtaBin7;
   Double_t Wm11_NBin7 = Wm11_NumGenEtaBin7;
   Double_t Wm11_effGenEtaBin7_err = (1 / Wm11_NBin7) * sqrt(Wm11_kBin7 * (1 - Wm11_kBin7 / Wm11_NBin7) );
   Double_t Wm11_kBin8 = Wm11_effGenEtaBin8 * Wm11_NumGenEtaBin8;
   Double_t Wm11_NBin8 = Wm11_NumGenEtaBin8;
   Double_t Wm11_effGenEtaBin8_err = (1 / Wm11_NBin8) * sqrt(Wm11_kBin8 * (1 - Wm11_kBin8 / Wm11_NBin8) );
   cout << "Wm11_effGenEtaBin1_err = " << Wm11_effGenEtaBin1_err << endl;
   cout << "Wm11_effGenEtaBin2_err = " << Wm11_effGenEtaBin2_err << endl;
   cout << "Wm11_effGenEtaBin3_err = " << Wm11_effGenEtaBin3_err << endl;
   cout << "Wm11_effGenEtaBin4_err = " << Wm11_effGenEtaBin4_err << endl;
   cout << "Wm11_effGenEtaBin5_err = " << Wm11_effGenEtaBin5_err << endl;
   cout << "Wm11_effGenEtaBin6_err = " << Wm11_effGenEtaBin6_err << endl;
   cout << "Wm11_effGenEtaBin7_err = " << Wm11_effGenEtaBin7_err << endl;
   cout << "Wm11_effGenEtaBin8_err = " << Wm11_effGenEtaBin8_err << endl;


   Double_t Wp12_NumRecoInGenEtaBin1 = hWp12_GenVsRec_ElectronEta -> GetBinContent(1,1);
   Double_t Wp12_NumGenEtaBin1       = hWp12_Gen_ElectronEta      -> GetBinContent(1);
   cout << "Wp12_NumRecoInGenEtaBin1 = " << Wp12_NumRecoInGenEtaBin1 << "   Wp12_NumGenEtaBin1 = " << Wp12_NumGenEtaBin1 << endl;
   Double_t Wp12_NumRecoInGenEtaBin2 = hWp12_GenVsRec_ElectronEta -> GetBinContent(2,2);
   Double_t Wp12_NumGenEtaBin2       = hWp12_Gen_ElectronEta      -> GetBinContent(2);
   cout << "Wp12_NumRecoInGenEtaBin2 = " << Wp12_NumRecoInGenEtaBin2 << "   Wp12_NumGenEtaBin2 = " << Wp12_NumGenEtaBin2  << endl;
   Double_t Wp12_NumRecoInGenEtaBin3 = hWp12_GenVsRec_ElectronEta -> GetBinContent(3,3);
   Double_t Wp12_NumGenEtaBin3       = hWp12_Gen_ElectronEta      -> GetBinContent(3);
   cout << "Wp12_NumRecoInGenEtaBin3 = " << Wp12_NumRecoInGenEtaBin3 << "   Wp12_NumGenEtaBin3 = " << Wp12_NumGenEtaBin3  << endl;
   Double_t Wp12_NumRecoInGenEtaBin4 = hWp12_GenVsRec_ElectronEta -> GetBinContent(4,4);
   Double_t Wp12_NumGenEtaBin4       = hWp12_Gen_ElectronEta      -> GetBinContent(4);
   cout << "Wp12_NumRecoInGenEtaBin4 = " << Wp12_NumRecoInGenEtaBin4 << "   Wp12_NumGenEtaBin4 = " << Wp12_NumGenEtaBin4  << endl;
   Double_t Wp12_NumRecoInGenEtaBin5 = hWp12_GenVsRec_ElectronEta -> GetBinContent(5,5);
   Double_t Wp12_NumGenEtaBin5       = hWp12_Gen_ElectronEta      -> GetBinContent(5);
   cout << "Wp12_NumRecoInGenEtaBin5 = " << Wp12_NumRecoInGenEtaBin5 << "   Wp12_NumGenEtaBin5 = " << Wp12_NumGenEtaBin5  << endl;
   Double_t Wp12_NumRecoInGenEtaBin6 = hWp12_GenVsRec_ElectronEta -> GetBinContent(6,6);
   Double_t Wp12_NumGenEtaBin6       = hWp12_Gen_ElectronEta      -> GetBinContent(6);
   cout << "Wp12_NumRecoInGenEtaBin6 = " << Wp12_NumRecoInGenEtaBin6 << "   Wp12_NumGenEtaBin6 = " << Wp12_NumGenEtaBin6  << endl;
   Double_t Wp12_NumRecoInGenEtaBin7 = hWp12_GenVsRec_ElectronEta -> GetBinContent(7,7);
   Double_t Wp12_NumGenEtaBin7       = hWp12_Gen_ElectronEta      -> GetBinContent(7);
   cout << "Wp12_NumRecoInGenEtaBin7 = " << Wp12_NumRecoInGenEtaBin7 << "   Wp12_NumGenEtaBin7 = " << Wp12_NumGenEtaBin7  << endl;
   Double_t Wp12_NumRecoInGenEtaBin8 = hWp12_GenVsRec_ElectronEta -> GetBinContent(8,8);
   Double_t Wp12_NumGenEtaBin8       = hWp12_Gen_ElectronEta      -> GetBinContent(8);
   cout << "Wp12_NumRecoInGenEtaBin8 = " << Wp12_NumRecoInGenEtaBin8 << "   Wp12_NumGenEtaBin8 = " << Wp12_NumGenEtaBin8  << endl;

   Double_t Wp12_effGenEtaBin1 = Wp12_NumRecoInGenEtaBin1 / Wp12_NumGenEtaBin1;
   Double_t Wp12_effGenEtaBin2 = Wp12_NumRecoInGenEtaBin2 / Wp12_NumGenEtaBin2;
   Double_t Wp12_effGenEtaBin3 = Wp12_NumRecoInGenEtaBin3 / Wp12_NumGenEtaBin3;
   Double_t Wp12_effGenEtaBin4 = Wp12_NumRecoInGenEtaBin4 / Wp12_NumGenEtaBin4;
   Double_t Wp12_effGenEtaBin5 = Wp12_NumRecoInGenEtaBin5 / Wp12_NumGenEtaBin5;
   Double_t Wp12_effGenEtaBin6 = Wp12_NumRecoInGenEtaBin6 / Wp12_NumGenEtaBin6;
   Double_t Wp12_effGenEtaBin7 = Wp12_NumRecoInGenEtaBin7 / Wp12_NumGenEtaBin7;
   Double_t Wp12_effGenEtaBin8 = Wp12_NumRecoInGenEtaBin8 / Wp12_NumGenEtaBin8;
   cout << "Wp12_effGenEtaBin1 = " << Wp12_effGenEtaBin1 << endl;
   cout << "Wp12_effGenEtaBin2 = " << Wp12_effGenEtaBin2 << endl;
   cout << "Wp12_effGenEtaBin3 = " << Wp12_effGenEtaBin3 << endl;
   cout << "Wp12_effGenEtaBin4 = " << Wp12_effGenEtaBin4 << endl;
   cout << "Wp12_effGenEtaBin5 = " << Wp12_effGenEtaBin5 << endl;
   cout << "Wp12_effGenEtaBin6 = " << Wp12_effGenEtaBin6 << endl;
   cout << "Wp12_effGenEtaBin7 = " << Wp12_effGenEtaBin7 << endl;
   cout << "Wp12_effGenEtaBin8 = " << Wp12_effGenEtaBin8 << endl;
   // calculate uncertainties using the binomial formula
   Double_t Wp12_kBin1 = Wp12_effGenEtaBin1 * Wp12_NumGenEtaBin1;
   Double_t Wp12_NBin1 = Wp12_NumGenEtaBin1;
   Double_t Wp12_effGenEtaBin1_err = (1 / Wp12_NBin1) * sqrt(Wp12_kBin1 * (1 - Wp12_kBin1 / Wp12_NBin1) );
   Double_t Wp12_kBin2 = Wp12_effGenEtaBin2 * Wp12_NumGenEtaBin2;
   Double_t Wp12_NBin2 = Wp12_NumGenEtaBin2;
   Double_t Wp12_effGenEtaBin2_err = (1 / Wp12_NBin2) * sqrt(Wp12_kBin2 * (1 - Wp12_kBin2 / Wp12_NBin2) );
   Double_t Wp12_kBin3 = Wp12_effGenEtaBin3 * Wp12_NumGenEtaBin3;
   Double_t Wp12_NBin3 = Wp12_NumGenEtaBin3;
   Double_t Wp12_effGenEtaBin3_err = (1 / Wp12_NBin3) * sqrt(Wp12_kBin3 * (1 - Wp12_kBin3 / Wp12_NBin3) );
   Double_t Wp12_kBin4 = Wp12_effGenEtaBin4 * Wp12_NumGenEtaBin4;
   Double_t Wp12_NBin4 = Wp12_NumGenEtaBin4;
   Double_t Wp12_effGenEtaBin4_err = (1 / Wp12_NBin4) * sqrt(Wp12_kBin4 * (1 - Wp12_kBin4 / Wp12_NBin4) );
   Double_t Wp12_kBin5 = Wp12_effGenEtaBin5 * Wp12_NumGenEtaBin5;
   Double_t Wp12_NBin5 = Wp12_NumGenEtaBin5;
   Double_t Wp12_effGenEtaBin5_err = (1 / Wp12_NBin5) * sqrt(Wp12_kBin5 * (1 - Wp12_kBin5 / Wp12_NBin5) );
   Double_t Wp12_kBin6 = Wp12_effGenEtaBin6 * Wp12_NumGenEtaBin6;
   Double_t Wp12_NBin6 = Wp12_NumGenEtaBin6;
   Double_t Wp12_effGenEtaBin6_err = (1 / Wp12_NBin6) * sqrt(Wp12_kBin6 * (1 - Wp12_kBin6 / Wp12_NBin6) );
   Double_t Wp12_kBin7 = Wp12_effGenEtaBin7 * Wp12_NumGenEtaBin7;
   Double_t Wp12_NBin7 = Wp12_NumGenEtaBin7;
   Double_t Wp12_effGenEtaBin7_err = (1 / Wp12_NBin7) * sqrt(Wp12_kBin7 * (1 - Wp12_kBin7 / Wp12_NBin7) );
   Double_t Wp12_kBin8 = Wp12_effGenEtaBin8 * Wp12_NumGenEtaBin8;
   Double_t Wp12_NBin8 = Wp12_NumGenEtaBin8;
   Double_t Wp12_effGenEtaBin8_err = (1 / Wp12_NBin8) * sqrt(Wp12_kBin8 * (1 - Wp12_kBin8 / Wp12_NBin8) );
   cout << "Wp12_effGenEtaBin1_err = " << Wp12_effGenEtaBin1_err << endl;
   cout << "Wp12_effGenEtaBin2_err = " << Wp12_effGenEtaBin2_err << endl;
   cout << "Wp12_effGenEtaBin3_err = " << Wp12_effGenEtaBin3_err << endl;
   cout << "Wp12_effGenEtaBin4_err = " << Wp12_effGenEtaBin4_err << endl;
   cout << "Wp12_effGenEtaBin5_err = " << Wp12_effGenEtaBin5_err << endl;
   cout << "Wp12_effGenEtaBin6_err = " << Wp12_effGenEtaBin6_err << endl;
   cout << "Wp12_effGenEtaBin7_err = " << Wp12_effGenEtaBin7_err << endl;
   cout << "Wp12_effGenEtaBin8_err = " << Wp12_effGenEtaBin8_err << endl;

   Double_t Wm12_NumRecoInGenEtaBin1 = hWm12_GenVsRec_ElectronEta -> GetBinContent(1,1);
   Double_t Wm12_NumGenEtaBin1       = hWm12_Gen_ElectronEta      -> GetBinContent(1);
   cout << "Wm12_NumRecoInGenEtaBin1 = " << Wm12_NumRecoInGenEtaBin1 << "   Wm12_NumGenEtaBin1 = " << Wm12_NumGenEtaBin1 << endl;
   Double_t Wm12_NumRecoInGenEtaBin2 = hWm12_GenVsRec_ElectronEta -> GetBinContent(2,2);
   Double_t Wm12_NumGenEtaBin2       = hWm12_Gen_ElectronEta      -> GetBinContent(2);
   cout << "Wm12_NumRecoInGenEtaBin2 = " << Wm12_NumRecoInGenEtaBin2 << "   Wm12_NumGenEtaBin2 = " << Wm12_NumGenEtaBin2  << endl;
   Double_t Wm12_NumRecoInGenEtaBin3 = hWm12_GenVsRec_ElectronEta -> GetBinContent(3,3);
   Double_t Wm12_NumGenEtaBin3       = hWm12_Gen_ElectronEta      -> GetBinContent(3);
   cout << "Wm12_NumRecoInGenEtaBin3 = " << Wm12_NumRecoInGenEtaBin3 << "   Wm12_NumGenEtaBin3 = " << Wm12_NumGenEtaBin3  << endl;
   Double_t Wm12_NumRecoInGenEtaBin4 = hWm12_GenVsRec_ElectronEta -> GetBinContent(4,4);
   Double_t Wm12_NumGenEtaBin4       = hWm12_Gen_ElectronEta      -> GetBinContent(4);
   cout << "Wm12_NumRecoInGenEtaBin4 = " << Wm12_NumRecoInGenEtaBin4 << "   Wm12_NumGenEtaBin4 = " << Wm12_NumGenEtaBin4  << endl;
   Double_t Wm12_NumRecoInGenEtaBin5 = hWm12_GenVsRec_ElectronEta -> GetBinContent(5,5);
   Double_t Wm12_NumGenEtaBin5       = hWm12_Gen_ElectronEta      -> GetBinContent(5);
   cout << "Wm12_NumRecoInGenEtaBin5 = " << Wm12_NumRecoInGenEtaBin5 << "   Wm12_NumGenEtaBin5 = " << Wm12_NumGenEtaBin5  << endl;
   Double_t Wm12_NumRecoInGenEtaBin6 = hWm12_GenVsRec_ElectronEta -> GetBinContent(6,6);
   Double_t Wm12_NumGenEtaBin6       = hWm12_Gen_ElectronEta      -> GetBinContent(6);
   cout << "Wm12_NumRecoInGenEtaBin6 = " << Wm12_NumRecoInGenEtaBin6 << "   Wm12_NumGenEtaBin6 = " << Wm12_NumGenEtaBin6  << endl;
   Double_t Wm12_NumRecoInGenEtaBin7 = hWm12_GenVsRec_ElectronEta -> GetBinContent(7,7);
   Double_t Wm12_NumGenEtaBin7       = hWm12_Gen_ElectronEta      -> GetBinContent(7);
   cout << "Wm12_NumRecoInGenEtaBin7 = " << Wm12_NumRecoInGenEtaBin7 << "   Wm12_NumGenEtaBin7 = " << Wm12_NumGenEtaBin7  << endl;
   Double_t Wm12_NumRecoInGenEtaBin8 = hWm12_GenVsRec_ElectronEta -> GetBinContent(8,8);
   Double_t Wm12_NumGenEtaBin8       = hWm12_Gen_ElectronEta      -> GetBinContent(8);
   cout << "Wm12_NumRecoInGenEtaBin8 = " << Wm12_NumRecoInGenEtaBin8 << "   Wm12_NumGenEtaBin8 = " << Wm12_NumGenEtaBin8  << endl;

   Double_t Wm12_effGenEtaBin1 = Wm12_NumRecoInGenEtaBin1 / Wm12_NumGenEtaBin1;
   Double_t Wm12_effGenEtaBin2 = Wm12_NumRecoInGenEtaBin2 / Wm12_NumGenEtaBin2;
   Double_t Wm12_effGenEtaBin3 = Wm12_NumRecoInGenEtaBin3 / Wm12_NumGenEtaBin3;
   Double_t Wm12_effGenEtaBin4 = Wm12_NumRecoInGenEtaBin4 / Wm12_NumGenEtaBin4;
   Double_t Wm12_effGenEtaBin5 = Wm12_NumRecoInGenEtaBin5 / Wm12_NumGenEtaBin5;
   Double_t Wm12_effGenEtaBin6 = Wm12_NumRecoInGenEtaBin6 / Wm12_NumGenEtaBin6;
   Double_t Wm12_effGenEtaBin7 = Wm12_NumRecoInGenEtaBin7 / Wm12_NumGenEtaBin7;
   Double_t Wm12_effGenEtaBin8 = Wm12_NumRecoInGenEtaBin8 / Wm12_NumGenEtaBin8;
   cout << "Wm12_effGenEtaBin1 = " << Wm12_effGenEtaBin1 << endl;
   cout << "Wm12_effGenEtaBin2 = " << Wm12_effGenEtaBin2 << endl;
   cout << "Wm12_effGenEtaBin3 = " << Wm12_effGenEtaBin3 << endl;
   cout << "Wm12_effGenEtaBin4 = " << Wm12_effGenEtaBin4 << endl;
   cout << "Wm12_effGenEtaBin5 = " << Wm12_effGenEtaBin5 << endl;
   cout << "Wm12_effGenEtaBin6 = " << Wm12_effGenEtaBin6 << endl;
   cout << "Wm12_effGenEtaBin7 = " << Wm12_effGenEtaBin7 << endl;
   cout << "Wm12_effGenEtaBin8 = " << Wm12_effGenEtaBin8 << endl;
   // calculate uncertainties using the binomial formula
   Double_t Wm12_kBin1 = Wm12_effGenEtaBin1 * Wm12_NumGenEtaBin1;
   Double_t Wm12_NBin1 = Wm12_NumGenEtaBin1;
   Double_t Wm12_effGenEtaBin1_err = (1 / Wm12_NBin1) * sqrt(Wm12_kBin1 * (1 - Wm12_kBin1 / Wm12_NBin1) );
   Double_t Wm12_kBin2 = Wm12_effGenEtaBin2 * Wm12_NumGenEtaBin2;
   Double_t Wm12_NBin2 = Wm12_NumGenEtaBin2;
   Double_t Wm12_effGenEtaBin2_err = (1 / Wm12_NBin2) * sqrt(Wm12_kBin2 * (1 - Wm12_kBin2 / Wm12_NBin2) );
   Double_t Wm12_kBin3 = Wm12_effGenEtaBin3 * Wm12_NumGenEtaBin3;
   Double_t Wm12_NBin3 = Wm12_NumGenEtaBin3;
   Double_t Wm12_effGenEtaBin3_err = (1 / Wm12_NBin3) * sqrt(Wm12_kBin3 * (1 - Wm12_kBin3 / Wm12_NBin3) );
   Double_t Wm12_kBin4 = Wm12_effGenEtaBin4 * Wm12_NumGenEtaBin4;
   Double_t Wm12_NBin4 = Wm12_NumGenEtaBin4;
   Double_t Wm12_effGenEtaBin4_err = (1 / Wm12_NBin4) * sqrt(Wm12_kBin4 * (1 - Wm12_kBin4 / Wm12_NBin4) );
   Double_t Wm12_kBin5 = Wm12_effGenEtaBin5 * Wm12_NumGenEtaBin5;
   Double_t Wm12_NBin5 = Wm12_NumGenEtaBin5;
   Double_t Wm12_effGenEtaBin5_err = (1 / Wm12_NBin5) * sqrt(Wm12_kBin5 * (1 - Wm12_kBin5 / Wm12_NBin5) );
   Double_t Wm12_kBin6 = Wm12_effGenEtaBin6 * Wm12_NumGenEtaBin6;
   Double_t Wm12_NBin6 = Wm12_NumGenEtaBin6;
   Double_t Wm12_effGenEtaBin6_err = (1 / Wm12_NBin6) * sqrt(Wm12_kBin6 * (1 - Wm12_kBin6 / Wm12_NBin6) );
   Double_t Wm12_kBin7 = Wm12_effGenEtaBin7 * Wm12_NumGenEtaBin7;
   Double_t Wm12_NBin7 = Wm12_NumGenEtaBin7;
   Double_t Wm12_effGenEtaBin7_err = (1 / Wm12_NBin7) * sqrt(Wm12_kBin7 * (1 - Wm12_kBin7 / Wm12_NBin7) );
   Double_t Wm12_kBin8 = Wm12_effGenEtaBin8 * Wm12_NumGenEtaBin8;
   Double_t Wm12_NBin8 = Wm12_NumGenEtaBin8;
   Double_t Wm12_effGenEtaBin8_err = (1 / Wm12_NBin8) * sqrt(Wm12_kBin8 * (1 - Wm12_kBin8 / Wm12_NBin8) );
   cout << "Wm12_effGenEtaBin1_err = " << Wm12_effGenEtaBin1_err << endl;
   cout << "Wm12_effGenEtaBin2_err = " << Wm12_effGenEtaBin2_err << endl;
   cout << "Wm12_effGenEtaBin3_err = " << Wm12_effGenEtaBin3_err << endl;
   cout << "Wm12_effGenEtaBin4_err = " << Wm12_effGenEtaBin4_err << endl;
   cout << "Wm12_effGenEtaBin5_err = " << Wm12_effGenEtaBin5_err << endl;
   cout << "Wm12_effGenEtaBin6_err = " << Wm12_effGenEtaBin6_err << endl;
   cout << "Wm12_effGenEtaBin7_err = " << Wm12_effGenEtaBin7_err << endl;
   cout << "Wm12_effGenEtaBin8_err = " << Wm12_effGenEtaBin8_err << endl;


   TH1D *hEfficiency_Wp_r11 = (TH1D *)hWp_Wp_ElectronEta -> Clone("hEfficiency_Wp_r11");
   hEfficiency_Wp_r11 -> SetTitle("run11; #eta_{Ele}; Efficiency");
   hEfficiency_Wp_r11 -> SetBinContent(1, Wp11_effGenEtaBin1);
   hEfficiency_Wp_r11 -> SetBinContent(2, Wp11_effGenEtaBin2);
   hEfficiency_Wp_r11 -> SetBinContent(3, Wp11_effGenEtaBin3);
   hEfficiency_Wp_r11 -> SetBinContent(4, Wp11_effGenEtaBin4);
   hEfficiency_Wp_r11 -> SetBinContent(5, Wp11_effGenEtaBin5);
   hEfficiency_Wp_r11 -> SetBinContent(6, Wp11_effGenEtaBin6);
   hEfficiency_Wp_r11 -> SetBinContent(7, Wp11_effGenEtaBin7);
   hEfficiency_Wp_r11 -> SetBinContent(8, Wp11_effGenEtaBin8);
   hEfficiency_Wp_r11 -> SetBinError(1, Wp11_effGenEtaBin1_err);
   hEfficiency_Wp_r11 -> SetBinError(2, Wp11_effGenEtaBin2_err);
   hEfficiency_Wp_r11 -> SetBinError(3, Wp11_effGenEtaBin1_err);
   hEfficiency_Wp_r11 -> SetBinError(4, Wp11_effGenEtaBin4_err);
   hEfficiency_Wp_r11 -> SetBinError(5, Wp11_effGenEtaBin5_err);
   hEfficiency_Wp_r11 -> SetBinError(6, Wp11_effGenEtaBin6_err);
   hEfficiency_Wp_r11 -> SetBinError(7, Wp11_effGenEtaBin7_err);
   hEfficiency_Wp_r11 -> SetBinError(8, Wp11_effGenEtaBin8_err);
   hEfficiency_Wp_r11 -> SetMaximum(1);
   hEfficiency_Wp_r11 -> SetMinimum(0);
   hEfficiency_Wp_r11 -> SetMarkerStyle(20);
   hEfficiency_Wp_r11 -> SetMarkerSize(1);
   hEfficiency_Wp_r11 -> SetMarkerColor(kGreen);

   TH1D *hEfficiency_Wm_r11 = (TH1D *)hWm_Wm_ElectronEta -> Clone("hEfficiency_Wm_r11");
   hEfficiency_Wm_r11 -> SetTitle("run11; #eta_{Ele}; Efficiency");
   hEfficiency_Wm_r11 -> SetBinContent(1, Wm11_effGenEtaBin1);
   hEfficiency_Wm_r11 -> SetBinContent(2, Wm11_effGenEtaBin2);
   hEfficiency_Wm_r11 -> SetBinContent(3, Wm11_effGenEtaBin3);
   hEfficiency_Wm_r11 -> SetBinContent(4, Wm11_effGenEtaBin4);
   hEfficiency_Wm_r11 -> SetBinContent(5, Wm11_effGenEtaBin5);
   hEfficiency_Wm_r11 -> SetBinContent(6, Wm11_effGenEtaBin6);
   hEfficiency_Wm_r11 -> SetBinContent(7, Wm11_effGenEtaBin7);
   hEfficiency_Wm_r11 -> SetBinContent(8, Wm11_effGenEtaBin8);
   hEfficiency_Wm_r11 -> SetBinError(1, Wm11_effGenEtaBin1_err);
   hEfficiency_Wm_r11 -> SetBinError(2, Wm11_effGenEtaBin2_err);
   hEfficiency_Wm_r11 -> SetBinError(3, Wm11_effGenEtaBin3_err);
   hEfficiency_Wm_r11 -> SetBinError(4, Wm11_effGenEtaBin4_err);
   hEfficiency_Wm_r11 -> SetBinError(5, Wm11_effGenEtaBin5_err);
   hEfficiency_Wm_r11 -> SetBinError(6, Wm11_effGenEtaBin6_err);
   hEfficiency_Wm_r11 -> SetBinError(7, Wm11_effGenEtaBin7_err);
   hEfficiency_Wm_r11 -> SetBinError(8, Wm11_effGenEtaBin8_err);
   hEfficiency_Wm_r11 -> SetMaximum(1);
   hEfficiency_Wm_r11 -> SetMinimum(0);
   hEfficiency_Wm_r11 -> SetMarkerStyle(4);
   hEfficiency_Wm_r11 -> SetMarkerSize(1);
   hEfficiency_Wm_r11 -> SetMarkerColor(kGreen);


   TH1D *hEfficiency_Wp_r12 = (TH1D *)hWp12_Wp_ElectronEta -> Clone("hEfficiency_Wp_r12");
   hEfficiency_Wp_r12 -> SetTitle("run12; #eta_{Ele}; Efficiency");
   hEfficiency_Wp_r12 -> SetBinContent(1, Wp12_effGenEtaBin1);
   hEfficiency_Wp_r12 -> SetBinContent(2, Wp12_effGenEtaBin2);
   hEfficiency_Wp_r12 -> SetBinContent(3, Wp12_effGenEtaBin3);
   hEfficiency_Wp_r12 -> SetBinContent(4, Wp12_effGenEtaBin4);
   hEfficiency_Wp_r12 -> SetBinContent(5, Wp12_effGenEtaBin5);
   hEfficiency_Wp_r12 -> SetBinContent(6, Wp12_effGenEtaBin6);
   hEfficiency_Wp_r12 -> SetBinContent(7, Wp12_effGenEtaBin7);
   hEfficiency_Wp_r12 -> SetBinContent(8, Wp12_effGenEtaBin8);
   hEfficiency_Wp_r12 -> SetBinError(1, Wp12_effGenEtaBin1_err);
   hEfficiency_Wp_r12 -> SetBinError(2, Wp12_effGenEtaBin2_err);
   hEfficiency_Wp_r12 -> SetBinError(3, Wp12_effGenEtaBin3_err);
   hEfficiency_Wp_r12 -> SetBinError(4, Wp12_effGenEtaBin4_err);
   hEfficiency_Wp_r12 -> SetBinError(5, Wp12_effGenEtaBin5_err);
   hEfficiency_Wp_r12 -> SetBinError(6, Wp12_effGenEtaBin6_err);
   hEfficiency_Wp_r12 -> SetBinError(7, Wp12_effGenEtaBin7_err);
   hEfficiency_Wp_r12 -> SetBinError(8, Wp12_effGenEtaBin8_err);
   hEfficiency_Wp_r12 -> SetMaximum(1);
   hEfficiency_Wp_r12 -> SetMinimum(0);
   hEfficiency_Wp_r12 -> SetMarkerStyle(20);
   hEfficiency_Wp_r12 -> SetMarkerSize(1);
   hEfficiency_Wp_r12 -> SetMarkerColor(kGreen);

   TH1D *hEfficiency_Wm_r12 = (TH1D *)hWm12_Wm_ElectronEta -> Clone("hEfficiency_Wm_r12");
   hEfficiency_Wm_r12 -> SetTitle("run12; #eta_{Ele}; Efficiency");
   hEfficiency_Wm_r12 -> SetBinContent(1, Wm12_effGenEtaBin1);
   hEfficiency_Wm_r12 -> SetBinContent(2, Wm12_effGenEtaBin2);
   hEfficiency_Wm_r12 -> SetBinContent(3, Wm12_effGenEtaBin3);
   hEfficiency_Wm_r12 -> SetBinContent(4, Wm12_effGenEtaBin4);
   hEfficiency_Wm_r12 -> SetBinContent(5, Wm12_effGenEtaBin5);
   hEfficiency_Wm_r12 -> SetBinContent(6, Wm12_effGenEtaBin6);
   hEfficiency_Wm_r12 -> SetBinContent(7, Wm12_effGenEtaBin7);
   hEfficiency_Wm_r12 -> SetBinContent(8, Wm12_effGenEtaBin8);
   hEfficiency_Wm_r12 -> SetBinError(1, Wm12_effGenEtaBin1_err);
   hEfficiency_Wm_r12 -> SetBinError(2, Wm12_effGenEtaBin2_err);
   hEfficiency_Wm_r12 -> SetBinError(3, Wm12_effGenEtaBin3_err);
   hEfficiency_Wm_r12 -> SetBinError(4, Wm12_effGenEtaBin4_err);
   hEfficiency_Wm_r12 -> SetBinError(5, Wm12_effGenEtaBin5_err);
   hEfficiency_Wm_r12 -> SetBinError(6, Wm12_effGenEtaBin6_err);
   hEfficiency_Wm_r12 -> SetBinError(7, Wm12_effGenEtaBin7_err);
   hEfficiency_Wm_r12 -> SetBinError(8, Wm12_effGenEtaBin8_err);
   hEfficiency_Wm_r12 -> SetMaximum(1);
   hEfficiency_Wm_r12 -> SetMinimum(0);
   hEfficiency_Wm_r12 -> SetMarkerStyle(4);
   hEfficiency_Wm_r12 -> SetMarkerSize(1);
   hEfficiency_Wm_r12 -> SetMarkerColor(kGreen);

   gStyle->SetErrorX(0);

  cTest_3-> cd();
   hEfficiency_Wp_r11        -> DrawCopy("E");
   hEfficiency_Wm_r11        -> DrawCopy("E same");
 
  cTest_4-> cd();
   hEfficiency_Wp_r12        -> DrawCopy("E");
   hEfficiency_Wm_r12        -> DrawCopy("E same");

   cTest -> Print(outPath + "/plot_BinMigTest_EleEta.eps");
   cTest -> Print(outPath + "/plot_BinMigTest_EleEta.png");

   // -----------

   TH1D *hEff_Wp_r11 = (TH1D *)hWp_Wp_ElectronEta -> Clone("hEff_Wp_r11");
   hWp_Wp_ElectronEta   -> Sumw2();
   hWp_Gen_ElectronEta  -> Sumw2();
   hEff_Wp_r11 -> Divide(hWp_Wp_ElectronEta, hWp_Gen_ElectronEta, 1, 1, "B");
   hEff_Wp_r11 -> SetTitle("run11; #eta_{Ele}; Efficiency");
   hEff_Wp_r11 -> SetMaximum(1);
   hEff_Wp_r11 -> SetMinimum(0);
   hEff_Wp_r11 -> SetMarkerStyle(20);
   hEff_Wp_r11 -> SetMarkerColor(kRed);
   //hEff_Wp_r11 -> Divide(hWp_Gen_ElectronEta);
   TH1D *hEff_Wm_r11 = (TH1D *)hWm_Wm_ElectronEta -> Clone("hEff_Wm_r11");
   //hEff_Wm_r11 -> Divide(hWm_Gen_ElectronEta);
   hWm_Wm_ElectronEta   -> Sumw2();
   hWm_Gen_ElectronEta  -> Sumw2();
   hEff_Wm_r11 -> Divide(hWm_Wm_ElectronEta, hWm_Gen_ElectronEta, 1, 1, "B");
   hEff_Wm_r11 -> SetMaximum(1);
   hEff_Wm_r11 -> SetMinimum(0);
   hEff_Wm_r11 -> SetMarkerStyle(4);
   hEff_Wm_r11 -> SetMarkerColor(kBlue);

   TH1D *hEff_Wp_r12 = (TH1D *)hWp12_Wp_ElectronEta -> Clone("hEff_Wp_r12");
   hWp12_Wp_ElectronEta   -> Sumw2();
   hWp12_Gen_ElectronEta  -> Sumw2();
   hEff_Wp_r12 -> Divide(hWp12_Wp_ElectronEta, hWp12_Gen_ElectronEta, 1, 1, "B");
   hEff_Wp_r12 -> SetTitle("run12; #eta_{Ele}; Efficiency");
   hEff_Wp_r12 -> SetMaximum(1);
   hEff_Wp_r12 -> SetMinimum(0);
   hEff_Wp_r12 -> SetMarkerStyle(20);
   hEff_Wp_r12 -> SetMarkerColor(kRed);
   //hEff_Wp_r12 -> Divide(hWp12_Gen_ElectronEta);
   TH1D *hEff_Wm_r12 = (TH1D *)hWm12_Wm_ElectronEta -> Clone("hEff_Wm_r12");
   //hEff_Wm_r12 -> Divide(hWm12_Gen_ElectronEta);
   hWm12_Wm_ElectronEta   -> Sumw2();
   hWm12_Gen_ElectronEta  -> Sumw2();
   hEff_Wm_r12 -> Divide(hWm12_Wm_ElectronEta, hWm12_Gen_ElectronEta, 1, 1, "B");
   hEff_Wm_r12 -> SetMaximum(1);
   hEff_Wm_r12 -> SetMinimum(0);
   hEff_Wm_r12 -> SetMarkerStyle(4);
   hEff_Wm_r12 -> SetMarkerColor(kBlue);


   TLegend *leg_cEffTest = new TLegend(0.75, 0.65, 0.85, 0.99);
   leg_cEffTest -> AddEntry(hEff_Wp_r11,"W+", "P");
   leg_cEffTest -> AddEntry(hEff_Wm_r11,"W-", "P");


   TCanvas *cEffTest    = new TCanvas("cEffTest", "Efficiency studies", 800, 500);
   // this test compares the two ways for calculation the efficiency:
   // 1. by dividing two histograms, one for the generated and one for the recontructed
   // 2. More properly binning in the generated variable and look to how many 
   //    reconstructed events fall in the generated bin

   cEffTest -> Divide(1,2);

   cEffTest_1 -> cd();
   //hEff_Wp_r11               -> GetXaxis() -> SetRange(3, 8);
   hEff_Wp_r11               -> SetStats(0);
   hEff_Wp_r11               -> Draw("e");
   hEff_Wm_r11               -> Draw("e same");
   hEfficiency_Wp_r11        -> SetMarkerSize(0.5);
   hEfficiency_Wp_r11        -> Draw("e same");
   hEfficiency_Wm_r11        -> SetMarkerSize(0.5);
   hEfficiency_Wm_r11        -> Draw("e same");

   leg_cEffTest              -> Draw();

   cEffTest_2 -> cd();
   //hEff_Wp_r12               -> GetXaxis() -> SetRange(3, 8);
   hEff_Wp_r12               -> SetStats(0);
   hEff_Wp_r12               -> Draw("e");
   hEff_Wm_r12               -> Draw("e same");
   hEfficiency_Wp_r12        -> SetMarkerSize(0.5);
   hEfficiency_Wp_r12        -> Draw("e same");
   hEfficiency_Wm_r12        -> SetMarkerSize(0.5);
   hEfficiency_Wm_r12        -> Draw("e same");

   //cEff_3 -> cd();
   //hWm12_Wm_ElectronEta        -> Draw();

   //cEff_4 -> cd();
   //hWm12_Gen_ElectronEta       -> Draw();
   //hEff_Wm_r11               -> Draw("e"); 

   cEffTest -> Print(outPath + "/plot_EfficiencyTest_EleEta.eps");
   cEffTest -> Print(outPath + "/plot_EfficiencyTest_EleEta.png");



   TCanvas *cEffWpWm    = new TCanvas("cEffWpWm", "Efficiency studies", 800, 500);
   // Here we compare the efficiencies for W+ and W- in each period

   hEfficiency_Wp_r11        -> SetMarkerColor(kRed);
   hEfficiency_Wm_r11        -> SetMarkerColor(kRed);
   hEfficiency_Wp_r12        -> SetMarkerColor(kBlue);
   hEfficiency_Wm_r12        -> SetMarkerColor(kBlue);

   TLegend *leg_cEffWpWm_r11 = new TLegend(0.6, 0.65, 0.75, 0.99);
   leg_cEffWpWm_r11 -> AddEntry(hEfficiency_Wp_r11,"W+ Run 11", "P");
   leg_cEffWpWm_r11 -> AddEntry(hEfficiency_Wm_r11,"W- Run 11", "P");

   TLegend *leg_cEffWpWm_r12 = new TLegend(0.6, 0.65, 0.75, 0.99);
   leg_cEffWpWm_r12 -> AddEntry(hEfficiency_Wp_r12,"W+ Run 12", "P");
   leg_cEffWpWm_r12 -> AddEntry(hEfficiency_Wm_r12,"W- Run 12", "P");

   cEffWpWm -> Divide(1,2);

   cEffWpWm_1 -> cd();
   cEffWpWm_1 -> SetTopMargin(0.15);
   cEffWpWm_1 -> SetBottomMargin(0.2);
   cEffWpWm_1 -> SetLeftMargin(0.1);
   //hEfficiency_Wp_r11        -> GetXaxis() -> SetRange(3, 8);
   hEfficiency_Wp_r11        -> SetLabelSize(0.08,"X");
   hEfficiency_Wp_r11        -> SetTitleSize(0.08,"XY");
   hEfficiency_Wp_r11        -> SetTitleOffset(1.,"X");
   hEfficiency_Wp_r11        -> SetTitleOffset(0.5,"Y");
   hEfficiency_Wp_r11        -> SetTitle("Run 11");
   hEfficiency_Wp_r11        -> SetStats(0);
   hEfficiency_Wp_r11        -> SetMarkerSize(1);
   hEfficiency_Wp_r11        -> Draw("e");
   hEfficiency_Wm_r11        -> SetMarkerSize(1);
   hEfficiency_Wm_r11        -> Draw("e same");
   leg_cEffWpWm_r11          -> Draw();

   cEffWpWm_2 -> cd();
   cEffWpWm_2 -> SetTopMargin(0.15);
   cEffWpWm_2 -> SetBottomMargin(0.2);
   cEffWpWm_2 -> SetLeftMargin(0.1);
   //hEfficiency_Wp_r12        -> GetXaxis() -> SetRange(3, 8);
   hEfficiency_Wp_r12        -> SetLabelSize(0.08,"X");
   hEfficiency_Wp_r12        -> SetTitleSize(0.08,"XY");
   hEfficiency_Wp_r12        -> SetTitleOffset(1.,"X");
   hEfficiency_Wp_r12        -> SetTitleOffset(0.5,"Y");
   hEfficiency_Wp_r12        -> SetTitle("Run 12");
   hEfficiency_Wp_r12        -> SetStats(0);
   hEfficiency_Wp_r12        -> SetMarkerSize(1);
   hEfficiency_Wp_r12        -> Draw("e");
   hEfficiency_Wm_r12        -> SetMarkerSize(1);
   hEfficiency_Wm_r12        -> SetLineWidth(2);
   hEfficiency_Wm_r12        -> Draw("e same");
   leg_cEffWpWm_r12               -> Draw();

   cEffWpWm -> Print(outPath + "/plot_EfficiencyWpWm_EleEta.eps");
   cEffWpWm -> Print(outPath + "/plot_EfficiencyWpWm_EleEta.png");



   TCanvas *cEff    = new TCanvas("cEff", "Efficiency studies", 800, 500);
   // Here we compare the efficiencies for run 11 and run 12

   hEfficiency_Wp_r11        -> SetMarkerColor(kRed);
   hEfficiency_Wm_r11        -> SetMarkerColor(kRed);
   hEfficiency_Wp_r12        -> SetMarkerColor(kBlue);
   hEfficiency_Wm_r12        -> SetMarkerColor(kBlue);

   TLegend *leg_cEff_Wp = new TLegend(0.6, 0.65, 0.75, 0.99);
   leg_cEff_Wp -> AddEntry(hEfficiency_Wp_r11,"W+ Run 11", "P");
   leg_cEff_Wp -> AddEntry(hEfficiency_Wp_r12,"W+ Run 12", "P");
   TLegend *leg_cEff_Wm = new TLegend(0.6, 0.65, 0.75, 0.99);
   leg_cEff_Wm -> AddEntry(hEfficiency_Wm_r12,"W- Run 12", "P");
   leg_cEff_Wm -> AddEntry(hEfficiency_Wm_r11,"W- Run 11", "P");

   cEff -> Divide(1,2);

   cEff_1 -> cd();
   cEff_1 -> SetTopMargin(0.15);
   cEff_1 -> SetBottomMargin(0.2);
   cEff_1 -> SetLeftMargin(0.1);
   //hEfficiency_Wp_r11        -> GetXaxis() -> SetRange(3, 8);
   hEfficiency_Wp_r11        -> SetLabelSize(0.08,"X");
   hEfficiency_Wp_r11        -> SetTitleSize(0.08,"XY");
   hEfficiency_Wp_r11        -> SetTitleOffset(1.,"X");
   hEfficiency_Wp_r11        -> SetTitleOffset(0.5,"Y");
   hEfficiency_Wp_r11        -> SetTitle("W+");
   hEfficiency_Wp_r11        -> SetStats(0);
   hEfficiency_Wp_r11        -> SetMarkerSize(1);
   hEfficiency_Wp_r11        -> Draw("e");
   hEfficiency_Wp_r12        -> SetMarkerSize(1);
   hEfficiency_Wp_r12        -> Draw("e same");
   leg_cEff_Wp               -> Draw();

   cEff_2 -> cd();
   cEff_2 -> SetTopMargin(0.15);
   cEff_2 -> SetBottomMargin(0.2);
   cEff_2 -> SetLeftMargin(0.1);
   //hEfficiency_Wm_r11        -> GetXaxis() -> SetRange(3, 8);
   hEfficiency_Wm_r11        -> SetLabelSize(0.08,"X");
   hEfficiency_Wm_r11        -> SetTitleSize(0.08,"XY");
   hEfficiency_Wm_r11        -> SetTitleOffset(1.,"X");
   hEfficiency_Wm_r11        -> SetTitleOffset(0.5,"Y");
   hEfficiency_Wm_r11        -> SetTitle("W-");
   hEfficiency_Wm_r11        -> SetStats(0);
   hEfficiency_Wm_r11        -> SetMarkerSize(1);
   hEfficiency_Wm_r11        -> Draw("e");
   hEfficiency_Wm_r12        -> SetMarkerSize(1);
   hEfficiency_Wm_r12        -> SetLineWidth(2);
   hEfficiency_Wm_r12        -> Draw("e same");
   leg_cEff_Wm               -> Draw();

   cEff -> Print(outPath + "/plot_Efficiency_EleEta.eps");
   cEff -> Print(outPath + "/plot_Efficiency_EleEta.png");



   TCanvas *cEffChargeDiff    = new TCanvas("cEffChargeDiff", "Efficiency studies", 800, 500);
   // Here we plot the difference in the Efficiency between W+ and W- in the same run period

   TH1D *hEffChargeDiff_r11 = (TH1D *)hEfficiency_Wp_r11 -> Clone("EffChargeDiff_r11");
   hEffChargeDiff_r11 -> Add(hEfficiency_Wp_r11, hEfficiency_Wm_r11, 1, -1);
   //hEffChargeDiff_r11 -> GetXaxis() -> SetRange(3, 8);
   TH1D *hEffChargeDiff_r12 = (TH1D *)hEfficiency_Wp_r12 -> Clone("EffChargeDiff_r12");
   hEffChargeDiff_r12 -> Add(hEfficiency_Wp_r12, hEfficiency_Wm_r12, 1, -1);


   cEffChargeDiff -> Divide(1,2);

   cEffChargeDiff -> cd();
   cEffChargeDiff -> SetTopMargin(0.15);
   cEffChargeDiff -> SetBottomMargin(0.2);
   cEffChargeDiff -> SetLeftMargin(0.15);
   hEffChargeDiff_r11        -> SetMaximum(0.1);
   hEffChargeDiff_r11        -> SetMinimum(-0.1);
   hEffChargeDiff_r11        -> SetTitle("Efficiency: charge dependence");
   hEffChargeDiff_r11        -> SetLabelSize(0.06,"X");
   hEffChargeDiff_r11        -> SetTitleSize(0.07,"XY");
   hEffChargeDiff_r11        -> SetTitleOffset(1.,"X");
   hEffChargeDiff_r11        -> SetTitleOffset(0.9,"Y");
   hEffChargeDiff_r11        -> GetYaxis() -> SetTitle("Eff(W+) - Eff(W-)");
   hEffChargeDiff_r11        -> SetStats(0);
   hEffChargeDiff_r11        -> SetMarkerStyle(20);
   hEffChargeDiff_r11        -> SetMarkerSize(1);
   hEffChargeDiff_r11        -> Draw("e1");
   hEffChargeDiff_r12        -> SetMarkerStyle(21);
   hEffChargeDiff_r12        -> SetMarkerSize(1);
   hEffChargeDiff_r12        -> Draw("e1 same");

   TLine *lineZero = new TLine(-1.2, 0, 1.2, 0);
   lineZero->SetLineColor(kBlack);
   lineZero->SetLineWidth(3);
   lineZero->SetLineStyle(2);
   lineZero->Draw();

   TLegend *leg_cEffChargeDiff = new TLegend(0.6, 0.65, 0.75, 0.8);
   leg_cEffChargeDiff -> AddEntry(hEffChargeDiff_r11,"Run 11", "P");
   leg_cEffChargeDiff -> AddEntry(hEffChargeDiff_r12,"Run 12", "P");

   leg_cEffChargeDiff -> Draw();

   cEffChargeDiff -> Print(outPath + "/plot_EffChargeDiff.eps");
   cEffChargeDiff -> Print(outPath + "/plot_EffChargeDiff.png");



   TCanvas *cZDC    = new TCanvas("cZDC", "ZDC rates", 800, 500);
   // Here we compare the efficiencies for run 11 and run 12

   cZDC -> Divide(1,2);

   cZDC_1 -> cd();
   hd_ZDCrates  -> SetTitle("Run 11");
   hd_ZDCrates  -> SetTitleSize(0.07,"XY");
   hd_ZDCrates  -> Draw();

   cZDC_2 -> cd();
   hd12_ZDCrates -> SetTitle("Run 12");
   hd12_ZDCrates -> SetTitleSize(0.07,"XY");
   hd12_ZDCrates -> Draw();

   cZDC -> Print(outPath + "/plot_ZDCrates.eps");
   cZDC -> Print(outPath + "/plot_ZDCrates.png");

   
   // SUBTRACT the BACKGROUNDS

   // first we take the scale factors as in the macro backgrounds20XX.C 
   // The error on the lumi measurement at STAR is ~13%

   float scaleZ_r11        = 0.014;
   float scaleWptn_r11     = 0.013;
   float scaleWmtn_r11     = 0.019;
   float scale_Wp_QCD_r11  = 0.057; // old style value (not used)
   float scale_Wp_QCD_2bnorm_r11  = 0.051;  //for systematic studies
   float scale_Wp_QCD_4bnorm_r11  = 0.067;
   float scale_Wp_QCD_8bnorm_r11  = 0.0107;  //for systematic studies
   float scale_Wm_QCD_r11  = 0.052; // old style value (not used)
   float scale_Wm_QCD_2bnorm_r11  = 0.045;  //for systematic studies
   float scale_Wm_QCD_4bnorm_r11  = 0.059;
   float scale_Wm_QCD_8bnorm_r11  = 0.076;  //for systematic studies

   float scaleZ_r12        = 0.082;
   float scaleWptn_r12     = 0.078;
   float scaleWmtn_r12     = 0.078;
   float scale_Wp_QCD_r12  = 0.070; // old style value (not used)
   float scale_Wp_QCD_2bnorm_r12  = 0.060;  //for systematic studies
   float scale_Wp_QCD_4bnorm_r12  = 0.087;
   float scale_Wp_QCD_8bnorm_r12  = 0.135;  //for systematic studies
   float scale_Wm_QCD_r12  = 0.062; // old style value (not used)
   float scale_Wm_QCD_2bnorm_r12  = 0.055;  //for systematic studies
   float scale_Wm_QCD_4bnorm_r12  = 0.076;
   float scale_Wm_QCD_8bnorm_r12  = 0.107;  //for systematic studies

   scale_Wp_QCD_r11 = scale_Wp_QCD_4bnorm_r11; // normally use "4bnorm" 
   scale_Wm_QCD_r11 = scale_Wm_QCD_4bnorm_r11; 
   scale_Wp_QCD_r12 = scale_Wp_QCD_4bnorm_r12; 
   scale_Wm_QCD_r12 = scale_Wm_QCD_4bnorm_r12; 

   bool systematics = false;

   if (syQCDp) {  // use different scaling for QCD systematics
      cout << "This is for systematics: syQCDp " << endl;
      bool systematics = true;
      scale_Wp_QCD_r11 = scale_Wp_QCD_8bnorm_r11; 
      scale_Wm_QCD_r11 = scale_Wm_QCD_8bnorm_r11; 
      scale_Wp_QCD_r12 = scale_Wp_QCD_8bnorm_r12; 
      scale_Wm_QCD_r12 = scale_Wm_QCD_8bnorm_r12; 
   } else if (syQCDm) {
      cout << "This is for systematics: syQCDm " << endl;
      bool systematics = true;
      scale_Wp_QCD_r11 = scale_Wp_QCD_2bnorm_r11; 
      scale_Wm_QCD_r11 = scale_Wm_QCD_2bnorm_r11; 
      scale_Wp_QCD_r12 = scale_Wp_QCD_2bnorm_r12; 
      scale_Wm_QCD_r12 = scale_Wm_QCD_2bnorm_r12; 
   }


   if (syZ0p) {  // assume Lumi+-13% for Z background systematics 
      cout << "This is for systematics: syZ0p " << endl;
      bool systematics = true;
      scaleZ_r11 = scaleZ_r11 * (1 + 0.13); 
      scaleZ_r12 = scaleZ_r12 * (1 + 0.13); 
   } else if (syZ0m) {
      cout << "This is for systematics: syZ0m " << endl;
      bool systematics = true;
      scaleZ_r11 = scaleZ_r11 * (1 - 0.13); 
      scaleZ_r12 = scaleZ_r12 * (1 - 0.13); 
   }

   cout.precision(3);
   cout << "*****************************************" << endl;
   cout << "SCALE FACTORS" << endl;
   cout << "RUN 2011                       "     << endl;
   cout << "Z0 lumi scale factor - run 11 ="     << scaleZ_r11 << endl;
   cout << "Wp->tv lumi scale factor - run 11 =" << scaleWptn_r11 << endl;
   cout << "Wm->tv lumi scale factor - run 11 =" << scaleWmtn_r11 << endl;
   cout << "W+, QCD scale factor - run 11 ="     << scale_Wp_QCD_r11 << endl;
   cout << "W-, QCD scale factor - run 11 ="     << scale_Wm_QCD_r11 << endl;
   cout << "RUN 2012                       "     << endl;
   cout << "Z0 lumi scale factor - run 12 ="     << scaleZ_r12 << endl;
   cout << "Wp->tv lumi scale factor - run 12 =" << scaleWptn_r12 << endl;
   cout << "Wm->tv lumi scale factor - run 12 =" << scaleWmtn_r12 << endl;
   cout << "W+, QCD scale factor - run 12 ="     << scale_Wp_QCD_r12 << endl;
   cout << "W-, QCD scale factor - run 12 ="     << scale_Wm_QCD_r12 << endl;
   cout << "*****************************************" << endl;


   TH1D *hd_Wp_QCDEta_scaled         = (TH1D *)hd_Wp_QCDEta        -> Clone("hd_Wp_QCDEta_scaled");
   TH1D *hd_Wm_QCDEta_scaled         = (TH1D *)hd_Wm_QCDEta        -> Clone("hd_Wm_QCDEta_scaled");
   TH1D *hZ_Wp_WBosonEta_scaled      = (TH1D *)hZ_Wp_WBosonEta     -> Clone("hZ_Wp_WBosonEta_scaled");
   TH1D *hZ_Wm_WBosonEta_scaled      = (TH1D *)hZ_Wm_WBosonEta     -> Clone("hZ_Wm_WBosonEta_scaled");
   TH1D *hWptn_Wp_WBosonEta_scaled   = (TH1D *)hWptn_Wp_WBosonEta  -> Clone("hWptn_Wp_WBosonEta_scaled");
   TH1D *hWmtn_Wm_WBosonEta_scaled   = (TH1D *)hWmtn_Wm_WBosonEta  -> Clone("hWmtn_Wm_WBosonEta_scaled");

   TH1D *hd_Wp_QCDRapidity_scaled         = (TH1D *)hd_Wp_QCDRapidity        -> Clone("hd_Wp_QCDRapidity_scaled");
   TH1D *hd_Wm_QCDRapidity_scaled         = (TH1D *)hd_Wm_QCDRapidity        -> Clone("hd_Wm_QCDRapidity_scaled");
   TH1D *hZ_Wp_WBosonRapidity_scaled      = (TH1D *)hZ_Wp_WBosonRapidity     -> Clone("hZ_Wp_WBosonRapidity_scaled");
   TH1D *hZ_Wm_WBosonRapidity_scaled      = (TH1D *)hZ_Wm_WBosonRapidity     -> Clone("hZ_Wm_WBosonRapidity_scaled");
   TH1D *hWptn_Wp_WBosonRapidity_scaled   = (TH1D *)hWptn_Wp_WBosonRapidity  -> Clone("hWptn_Wp_WBosonRapidity_scaled");
   TH1D *hWmtn_Wm_WBosonRapidity_scaled   = (TH1D *)hWmtn_Wm_WBosonRapidity  -> Clone("hWmtn_Wm_WBosonRapidity_scaled");

   TH1D *hd_Wp_QCDElectronEta_scaled = (TH1D *)hd_Wp_QCDElectronEta-> Clone("hd_Wp_QCDElectronEta_scaled");
   TH1D *hd_Wm_QCDElectronEta_scaled = (TH1D *)hd_Wm_QCDElectronEta-> Clone("hd_Wm_QCDElectronEta_scaled");
   TH1D *hZ_Wp_ElectronEta_scaled    = (TH1D *)hZ_Wp_ElectronEta   -> Clone("hZ_Wp_ElectronEta_scaled");
   TH1D *hZ_Wm_ElectronEta_scaled    = (TH1D *)hZ_Wm_ElectronEta   -> Clone("hZ_Wm_ElectronEta_scaled");
   TH1D *hWptn_Wp_ElectronEta_scaled = (TH1D *)hWptn_Wp_ElectronEta-> Clone("hWptn_Wp_ElectronEta_scaled");
   TH1D *hWmtn_Wm_ElectronEta_scaled = (TH1D *)hWmtn_Wm_ElectronEta-> Clone("hWmtn_Wm_ElectronEta_scaled");


   TH1D *hd12_Wp_QCDEta_scaled         = (TH1D *)hd12_Wp_QCDEta       -> Clone("hd12_Wp_QCDEta_scaled");
   TH1D *hd12_Wm_QCDEta_scaled         = (TH1D *)hd12_Wm_QCDEta       -> Clone("hd12_Wm_QCDEta_scaled");
   TH1D *hZ12_Wp_WBosonEta_scaled      = (TH1D *)hZ12_Wp_WBosonEta    -> Clone("hZ12_Wp_WBosonEta_scaled");
   TH1D *hZ12_Wm_WBosonEta_scaled      = (TH1D *)hZ12_Wm_WBosonEta    -> Clone("hZ12_Wm_WBosonEta_scaled");
   TH1D *hWptn12_Wp_WBosonEta_scaled   = (TH1D *)hWptn12_Wp_WBosonEta -> Clone("hWptn12_Wp_WBosonEta_scaled");
   TH1D *hWmtn12_Wm_WBosonEta_scaled   = (TH1D *)hWmtn12_Wm_WBosonEta -> Clone("hWmtn12_Wm_WBosonEta_scaled");

   TH1D *hd12_Wp_QCDRapidity_scaled         = (TH1D *)hd12_Wp_QCDRapidity       -> Clone("hd12_Wp_QCDRapidity_scaled");
   TH1D *hd12_Wm_QCDRapidity_scaled         = (TH1D *)hd12_Wm_QCDRapidity       -> Clone("hd12_Wm_QCDRapidity_scaled");
   TH1D *hZ12_Wp_WBosonRapidity_scaled      = (TH1D *)hZ12_Wp_WBosonRapidity    -> Clone("hZ12_Wp_WBosonRapidity_scaled");
   TH1D *hZ12_Wm_WBosonRapidity_scaled      = (TH1D *)hZ12_Wm_WBosonRapidity    -> Clone("hZ12_Wm_WBosonRapidity_scaled");
   TH1D *hWptn12_Wp_WBosonRapidity_scaled   = (TH1D *)hWptn12_Wp_WBosonRapidity -> Clone("hWptn12_Wp_WBosonRapidity_scaled");
   TH1D *hWmtn12_Wm_WBosonRapidity_scaled   = (TH1D *)hWmtn12_Wm_WBosonRapidity -> Clone("hWmtn12_Wm_WBosonRapidity_scaled");

   TH1D *hd12_Wp_QCDElectronEta_scaled = (TH1D *)hd12_Wp_QCDElectronEta-> Clone("hd12_Wp_QCDElectronEta_scaled");
   TH1D *hd12_Wm_QCDElectronEta_scaled = (TH1D *)hd12_Wm_QCDElectronEta-> Clone("hd12_Wm_QCDElectronEta_scaled");
   TH1D *hZ12_Wp_ElectronEta_scaled    = (TH1D *)hZ12_Wp_ElectronEta   -> Clone("hZ12_Wp_ElectronEta_scaled");
   TH1D *hZ12_Wm_ElectronEta_scaled    = (TH1D *)hZ12_Wm_ElectronEta   -> Clone("hZ12_Wm_ElectronEta_scaled");
   TH1D *hWptn12_Wp_ElectronEta_scaled = (TH1D *)hWptn12_Wp_ElectronEta-> Clone("hWptn12_Wp_ElectronEta_scaled");
   TH1D *hWmtn12_Wm_ElectronEta_scaled = (TH1D *)hWmtn12_Wm_ElectronEta-> Clone("hWmtn12_Wm_ElectronEta_scaled");

   hd_Wp_QCDEta_scaled           -> Scale(scale_Wp_QCD_r11); 
   hd_Wm_QCDEta_scaled           -> Scale(scale_Wm_QCD_r11); 
   hZ_Wp_WBosonEta_scaled        -> Scale(scaleZ_r11);
   hZ_Wm_WBosonEta_scaled        -> Scale(scaleZ_r11);
   hWptn_Wp_WBosonEta_scaled     -> Scale(scaleWptn_r11);
   hWmtn_Wm_WBosonEta_scaled     -> Scale(scaleWmtn_r11);

   hd_Wp_QCDRapidity_scaled           -> Scale(scale_Wp_QCD_r11); 
   hd_Wm_QCDRapidity_scaled           -> Scale(scale_Wm_QCD_r11); 
   hZ_Wp_WBosonRapidity_scaled        -> Scale(scaleZ_r11);
   hZ_Wm_WBosonRapidity_scaled        -> Scale(scaleZ_r11);
   hWptn_Wp_WBosonRapidity_scaled     -> Scale(scaleWptn_r11);
   hWmtn_Wm_WBosonRapidity_scaled     -> Scale(scaleWmtn_r11);

   hd_Wp_QCDElectronEta_scaled   -> Scale(scale_Wp_QCD_r11); 
   hd_Wm_QCDElectronEta_scaled   -> Scale(scale_Wm_QCD_r11); 
   hZ_Wp_ElectronEta_scaled      -> Scale(scaleZ_r11);
   hZ_Wm_ElectronEta_scaled      -> Scale(scaleZ_r11);
   hWptn_Wp_ElectronEta_scaled   -> Scale(scaleWptn_r11);
   hWmtn_Wm_ElectronEta_scaled   -> Scale(scaleWmtn_r11);


   hd12_Wp_QCDEta_scaled           -> Scale(scale_Wp_QCD_r12); 
   hd12_Wm_QCDEta_scaled           -> Scale(scale_Wm_QCD_r12); 
   hZ12_Wp_WBosonEta_scaled        -> Scale(scaleZ_r12);
   hZ12_Wm_WBosonEta_scaled        -> Scale(scaleZ_r12);
   hWptn12_Wp_WBosonEta_scaled     -> Scale(scaleWptn_r12);
   hWmtn12_Wm_WBosonEta_scaled     -> Scale(scaleWmtn_r12);

   hd12_Wp_QCDRapidity_scaled           -> Scale(scale_Wp_QCD_r12); 
   hd12_Wm_QCDRapidity_scaled           -> Scale(scale_Wm_QCD_r12); 
   hZ12_Wp_WBosonRapidity_scaled        -> Scale(scaleZ_r12);
   hZ12_Wm_WBosonRapidity_scaled        -> Scale(scaleZ_r12);
   hWptn12_Wp_WBosonRapidity_scaled     -> Scale(scaleWptn_r12);
   hWmtn12_Wm_WBosonRapidity_scaled     -> Scale(scaleWmtn_r12);

   hd12_Wp_QCDElectronEta_scaled   -> Scale(scale_Wp_QCD_r12); 
   hd12_Wm_QCDElectronEta_scaled   -> Scale(scale_Wm_QCD_r12); 
   hZ12_Wp_ElectronEta_scaled      -> Scale(scaleZ_r12);
   hZ12_Wm_ElectronEta_scaled      -> Scale(scaleZ_r12);
   hWptn12_Wp_ElectronEta_scaled   -> Scale(scaleWptn_r12);
   hWmtn12_Wm_ElectronEta_scaled   -> Scale(scaleWmtn_r12);

   // Sum up all the backgorunds
   TH1D *hMC_Wp_WBosonEta_total    = (TH1D *)hWptn_Wp_WBosonEta_scaled    -> Clone("hMC_Wp_WBosonEta_total");
   hMC_Wp_WBosonEta_total -> Add(hZ_Wp_WBosonEta_scaled);
   hMC_Wp_WBosonEta_total -> Add(hd_Wp_QCDEta_scaled);
   TH1D *hMC_Wp_WBosonRapidity_total    = (TH1D *)hWptn_Wp_WBosonRapidity_scaled    -> Clone("hMC_Wp_WBosonRapidity_total");
   hMC_Wp_WBosonRapidity_total -> Add(hZ_Wp_WBosonRapidity_scaled);
   hMC_Wp_WBosonRapidity_total -> Add(hd_Wp_QCDRapidity_scaled);
   TH1D *hMC_Wp_ElectronEta_total  = (TH1D *)hWptn_Wp_ElectronEta_scaled  -> Clone("hMC_Wp_ElectronEta_total");
   hMC_Wp_ElectronEta_total -> Add(hZ_Wp_ElectronEta_scaled);
   hMC_Wp_ElectronEta_total -> Add(hd_Wp_QCDElectronEta_scaled);

   TH1D *hMC_Wm_WBosonEta_total    = (TH1D *)hWmtn_Wm_WBosonEta_scaled    -> Clone("hMC_Wm_WBosonEta_total");
   hMC_Wm_WBosonEta_total -> Add(hZ_Wm_WBosonEta_scaled);
   hMC_Wm_WBosonEta_total -> Add(hd_Wm_QCDEta_scaled);
   TH1D *hMC_Wm_WBosonRapidity_total    = (TH1D *)hWmtn_Wm_WBosonRapidity_scaled    -> Clone("hMC_Wm_WBosonRapidity_total");
   hMC_Wm_WBosonRapidity_total -> Add(hZ_Wm_WBosonRapidity_scaled);
   hMC_Wm_WBosonRapidity_total -> Add(hd_Wm_QCDRapidity_scaled);
   TH1D *hMC_Wm_ElectronEta_total  = (TH1D *)hWmtn_Wm_ElectronEta_scaled  -> Clone("hMC_Wm_ElectronEta_total");
   hMC_Wm_ElectronEta_total -> Add(hZ_Wm_ElectronEta_scaled);
   hMC_Wm_ElectronEta_total -> Add(hd_Wm_QCDElectronEta_scaled);

   TH1D *hMC12_Wp_WBosonEta_total  = (TH1D *)hWptn12_Wp_WBosonEta_scaled  -> Clone("hMC12_Wp_WBosonEta_total");
   hMC12_Wp_WBosonEta_total -> Add(hZ12_Wp_WBosonEta_scaled);
   hMC12_Wp_WBosonEta_total -> Add(hd12_Wp_QCDEta_scaled);
   TH1D *hMC12_Wp_WBosonRapidity_total  = (TH1D *)hWptn12_Wp_WBosonRapidity_scaled  -> Clone("hMC12_Wp_WBosonRapidity_total");
   hMC12_Wp_WBosonRapidity_total -> Add(hZ12_Wp_WBosonRapidity_scaled);
   hMC12_Wp_WBosonRapidity_total -> Add(hd12_Wp_QCDRapidity_scaled);
   TH1D *hMC12_Wp_ElectronEta_total= (TH1D *)hWptn12_Wp_ElectronEta_scaled -> Clone("hMC12_Wp_ElectronEta_total");
   hMC12_Wp_ElectronEta_total -> Add(hZ12_Wp_ElectronEta_scaled);
   hMC12_Wp_ElectronEta_total -> Add(hd12_Wp_QCDElectronEta_scaled);

   TH1D *hMC12_Wm_WBosonEta_total  = (TH1D *)hWmtn12_Wm_WBosonEta_scaled  -> Clone("hMC12_Wm_WBosonEta_total");
   hMC12_Wm_WBosonEta_total -> Add(hZ12_Wm_WBosonEta_scaled);
   hMC12_Wm_WBosonEta_total -> Add(hd12_Wm_QCDEta_scaled);
   TH1D *hMC12_Wm_WBosonRapidity_total  = (TH1D *)hWmtn12_Wm_WBosonRapidity_scaled  -> Clone("hMC12_Wm_WBosonRapidity_total");
   hMC12_Wm_WBosonRapidity_total -> Add(hZ12_Wm_WBosonRapidity_scaled);
   hMC12_Wm_WBosonRapidity_total -> Add(hd12_Wm_QCDRapidity_scaled);
   TH1D *hMC12_Wm_ElectronEta_total= (TH1D *)hWmtn12_Wm_ElectronEta_scaled -> Clone("hMC12_Wm_ElectronEta_total");
   hMC12_Wm_ElectronEta_total -> Add(hZ12_Wm_ElectronEta_scaled);
   hMC12_Wm_ElectronEta_total -> Add(hd12_Wm_QCDElectronEta_scaled);

   // subtract backgrounds from data
   TH1D *hd_Wp_WBosonEta_BGsub   = (TH1D *)hd_Wp_WBosonEta               -> Clone("hd_Wp_WBosonEta_BGsub");
   hd_Wp_WBosonEta_BGsub -> Add(hMC_Wp_WBosonEta_total, -1);
   TH1D *hd_Wp_WBosonRapidity_BGsub   = (TH1D *)hd_Wp_WBosonRapidity     -> Clone("hd_Wp_WBosonRapidity_BGsub");
   hd_Wp_WBosonRapidity_BGsub -> Add(hMC_Wp_WBosonRapidity_total, -1);
   TH1D *hd_Wp_ElectronEta_BGsub = (TH1D *)hd_Wp_ElectronEta             -> Clone("hd_Wp_ElectronEta_BGsub");
   hd_Wp_ElectronEta_BGsub -> Add(hMC_Wp_ElectronEta_total, -1);

   TH1D *hd_Wm_WBosonEta_BGsub   = (TH1D *)hd_Wm_WBosonEta               -> Clone("hd_Wm_WBosonEta_BGsub");
   hd_Wm_WBosonEta_BGsub -> Add(hMC_Wm_WBosonEta_total, -1);
   TH1D *hd_Wm_WBosonRapidity_BGsub   = (TH1D *)hd_Wm_WBosonRapidity     -> Clone("hd_Wm_WBosonRapidity_BGsub");
   hd_Wm_WBosonRapidity_BGsub -> Add(hMC_Wm_WBosonRapidity_total, -1);
   TH1D *hd_Wm_ElectronEta_BGsub = (TH1D *)hd_Wm_ElectronEta             -> Clone("hd_Wm_ElectronEta_BGsub");
   hd_Wm_ElectronEta_BGsub -> Add(hMC_Wm_ElectronEta_total, -1);

   TH1D *hd12_Wp_WBosonEta_BGsub   = (TH1D *)hd12_Wp_WBosonEta           -> Clone("hd12_Wp_WBosonEta_BGsub");
   hd12_Wp_WBosonEta_BGsub -> Add(hMC12_Wp_WBosonEta_total, -1);
   TH1D *hd12_Wp_WBosonRapidity_BGsub   = (TH1D *)hd12_Wp_WBosonRapidity -> Clone("hd12_Wp_WBosonRapidity_BGsub");
   hd12_Wp_WBosonRapidity_BGsub -> Add(hMC12_Wp_WBosonRapidity_total, -1);
   TH1D *hd12_Wp_ElectronEta_BGsub = (TH1D *)hd12_Wp_ElectronEta         -> Clone("hd_Wp_ElectronEta_BGsub");
   hd12_Wp_ElectronEta_BGsub -> Add(hMC12_Wp_ElectronEta_total, -1);

   TH1D *hd12_Wm_WBosonEta_BGsub   = (TH1D *)hd12_Wm_WBosonEta           -> Clone("hd12_Wm_WBosonEta_BGsub");
   hd12_Wm_WBosonEta_BGsub -> Add(hMC12_Wm_WBosonEta_total, -1);
   TH1D *hd12_Wm_WBosonRapidity_BGsub   = (TH1D *)hd12_Wm_WBosonRapidity -> Clone("hd12_Wm_WBosonRapidity_BGsub");
   hd12_Wm_WBosonRapidity_BGsub -> Add(hMC12_Wm_WBosonRapidity_total, -1);
   TH1D *hd12_Wm_ElectronEta_BGsub = (TH1D *)hd12_Wm_ElectronEta         -> Clone("hd_Wp_ElectronEta_BGsub");
   hd12_Wm_ElectronEta_BGsub -> Add(hMC12_Wm_ElectronEta_total, -1);


   TCanvas *WETA_Bg    = new TCanvas("WETA_Bg", "RUN 11 - WBosonEta", 800, 500);

   WETA_Bg -> Divide(2,2);

   WETA_Bg_1 -> cd();
   hd_Wp_WBosonEta        -> SetTitle("run 11 - positive charge");
   hd_Wp_WBosonEta        -> SetMarkerStyle(20);
   hd_Wp_WBosonEta        -> Draw("e");
   hd_Wp_WBosonEta_BGsub  -> SetMarkerColor(kBlue);
   hd_Wp_WBosonEta_BGsub  -> SetMarkerStyle(21);
   hd_Wp_WBosonEta_BGsub  -> Draw("e same");
   WETA_Bg_2 -> cd();   
   hZ_Wp_WBosonEta_scaled -> SetMinimum(0);  
   hZ_Wp_WBosonEta_scaled -> SetMaximum(15);   
   hZ_Wp_WBosonEta_scaled -> SetLineWidth(2); 
   hZ_Wp_WBosonEta_scaled -> SetLineColor(kRed);  
   hZ_Wp_WBosonEta_scaled -> SetFillColor(0);
   hd_Wp_QCDEta_scaled    -> SetLineWidth(2); 
   hd_Wp_QCDEta_scaled    -> SetLineColor(kGreen); 
   hd_Wp_QCDEta_scaled    -> SetFillColor(0);
   hWptn_Wp_WBosonEta_scaled -> SetLineWidth(2);   
   hWptn_Wp_WBosonEta_scaled -> SetLineColor(kBlue);  
   hWptn_Wp_WBosonEta_scaled -> SetFillColor(0); 
   hMC_Wp_WBosonEta_total              -> SetLineWidth(2);  
   hMC_Wp_WBosonEta_total              -> SetLineStyle(7);   
   hMC_Wp_WBosonEta_total              -> SetLineColor(kBlack);   
   hMC_Wp_WBosonEta_total              -> SetFillColor(0);
   hZ_Wp_WBosonEta_scaled    -> SetTitle("run 11 - positive charge"); 
   hZ_Wp_WBosonEta_scaled    -> SetStats(0);   
   hZ_Wp_WBosonEta_scaled    -> Draw();   
   hd_Wp_QCDEta_scaled       -> Draw("same");     
   hWptn_Wp_WBosonEta_scaled -> Draw("same");   
   hMC_Wp_WBosonEta_total    -> Draw("same"); 
 
   TLegend *leg_WETA_Bg = new TLegend(0.75, 0.65, 0.995, 0.99);
   leg_WETA_Bg -> AddEntry(hd_Wp_WBosonEta,"STAR data", "P");
   leg_WETA_Bg -> AddEntry(hd_Wp_WBosonEta_BGsub,"STAR data - Bg", "P");
   leg_WETA_Bg -> AddEntry(hMC_Wp_WBosonEta_total,"TOTAL Bg. ", "L" );
   leg_WETA_Bg -> AddEntry(hWptn_Wp_WBosonEta_scaled,"PYTHIA W-> #tau#nu_{#tau} ", "L" );
   leg_WETA_Bg -> AddEntry(hZ_Wp_WBosonEta_scaled,"PYTHIA Z^{0}-> e^{+}e^{-}", "L");
   leg_WETA_Bg -> AddEntry(hd_Wp_QCDEta_scaled,"Data driven QCD", "L");
   leg_WETA_Bg -> Draw();

   WETA_Bg_3 -> cd();
   hd_Wm_WBosonEta        -> SetTitle("run 11 - negative charge");
   hd_Wm_WBosonEta        -> SetMarkerStyle(20);
   hd_Wm_WBosonEta        -> Draw("e");
   hd_Wm_WBosonEta_BGsub  -> SetMarkerColor(kBlue);
   hd_Wm_WBosonEta_BGsub  -> SetMarkerStyle(21);
   hd_Wm_WBosonEta_BGsub  -> Draw("e same");
   WETA_Bg_4 -> cd();   
   hZ_Wm_WBosonEta_scaled -> SetMinimum(0);  
   hZ_Wm_WBosonEta_scaled -> SetMaximum(10);   
   hZ_Wm_WBosonEta_scaled -> SetLineWidth(2); 
   hZ_Wm_WBosonEta_scaled -> SetLineColor(kRed);  
   hZ_Wm_WBosonEta_scaled -> SetFillColor(0);
   hd_Wm_QCDEta_scaled    -> SetLineWidth(2); 
   hd_Wm_QCDEta_scaled    -> SetLineColor(kGreen); 
   hd_Wm_QCDEta_scaled    -> SetFillColor(0);
   hWmtn_Wm_WBosonEta_scaled -> SetLineWidth(2);   
   hWmtn_Wm_WBosonEta_scaled -> SetLineColor(kBlue);  
   hWmtn_Wm_WBosonEta_scaled -> SetFillColor(0); 
   hMC_Wm_WBosonEta_total              -> SetLineWidth(2);  
   hMC_Wm_WBosonEta_total              -> SetLineStyle(7);   
   hMC_Wm_WBosonEta_total              -> SetLineColor(kBlack);   
   hMC_Wm_WBosonEta_total              -> SetFillColor(0);
   hZ_Wm_WBosonEta_scaled    -> SetTitle("run 11 - negative charge"); 
   hZ_Wm_WBosonEta_scaled    -> SetStats(0);   
   hZ_Wm_WBosonEta_scaled    -> Draw();   
   hd_Wm_QCDEta_scaled       -> Draw("same");     
   hWmtn_Wm_WBosonEta_scaled -> Draw("same");   
   hMC_Wm_WBosonEta_total    -> Draw("same"); 

   WETA_Bg -> Print(outPath + "/plot_r11_BG_vs_WEta.eps");
   WETA_Bg -> Print(outPath + "/plot_r11_BG_vs_WEta.png");


   TCanvas *WRAP_Bg    = new TCanvas("WRAP_Bg", "RUN 11 - WBosonRapidity", 800, 500);

   WRAP_Bg -> Divide(2,2);

   WRAP_Bg_1 -> cd();
   hd_Wp_WBosonRapidity        -> SetTitle("run 11 - positive charge");
   hd_Wp_WBosonRapidity        -> SetMarkerStyle(20);
   hd_Wp_WBosonRapidity        -> Draw("e");
   hd_Wp_WBosonRapidity_BGsub  -> SetMarkerColor(kBlue);
   hd_Wp_WBosonRapidity_BGsub  -> SetMarkerStyle(21);
   hd_Wp_WBosonRapidity_BGsub  -> Draw("e same");
   WRAP_Bg_2 -> cd();   
   hZ_Wp_WBosonRapidity_scaled -> SetMinimum(0);  
   hZ_Wp_WBosonRapidity_scaled -> SetMaximum(15);   
   hZ_Wp_WBosonRapidity_scaled -> SetLineWidth(2); 
   hZ_Wp_WBosonRapidity_scaled -> SetLineColor(kRed);  
   hZ_Wp_WBosonRapidity_scaled -> SetFillColor(0);
   hd_Wp_QCDRapidity_scaled    -> SetLineWidth(2); 
   hd_Wp_QCDRapidity_scaled    -> SetLineColor(kGreen); 
   hd_Wp_QCDRapidity_scaled    -> SetFillColor(0);
   hWptn_Wp_WBosonRapidity_scaled -> SetLineWidth(2);   
   hWptn_Wp_WBosonRapidity_scaled -> SetLineColor(kBlue);  
   hWptn_Wp_WBosonRapidity_scaled -> SetFillColor(0); 
   hMC_Wp_WBosonRapidity_total              -> SetLineWidth(2);  
   hMC_Wp_WBosonRapidity_total              -> SetLineStyle(7);   
   hMC_Wp_WBosonRapidity_total              -> SetLineColor(kBlack);   
   hMC_Wp_WBosonRapidity_total              -> SetFillColor(0);
   hZ_Wp_WBosonRapidity_scaled    -> SetTitle("run 11 - positive charge"); 
   hZ_Wp_WBosonRapidity_scaled    -> SetStats(0);   
   hZ_Wp_WBosonRapidity_scaled    -> Draw();   
   hd_Wp_QCDRapidity_scaled       -> Draw("same");     
   hWptn_Wp_WBosonRapidity_scaled -> Draw("same");   
   hMC_Wp_WBosonRapidity_total    -> Draw("same"); 
 
   TLegend *leg_WRAP_Bg = new TLegend(0.75, 0.65, 0.995, 0.99);
   leg_WRAP_Bg -> AddEntry(hd_Wp_WBosonRapidity,"STAR data", "P");
   leg_WRAP_Bg -> AddEntry(hd_Wp_WBosonRapidity_BGsub,"STAR data - Bg", "P");
   leg_WRAP_Bg -> AddEntry(hMC_Wp_WBosonRapidity_total,"TOTAL Bg. ", "L" );
   leg_WRAP_Bg -> AddEntry(hWptn_Wp_WBosonRapidity_scaled,"PYTHIA W-> #tau#nu_{#tau} ", "L" );
   leg_WRAP_Bg -> AddEntry(hZ_Wp_WBosonRapidity_scaled,"PYTHIA Z^{0}-> e^{+}e^{-}", "L");
   leg_WRAP_Bg -> AddEntry(hd_Wp_QCDRapidity_scaled,"Data driven QCD", "L");
   leg_WRAP_Bg -> Draw();

   WRAP_Bg_3 -> cd();
   hd_Wm_WBosonRapidity        -> SetTitle("run 11 - negative charge");
   hd_Wm_WBosonRapidity        -> SetMarkerStyle(20);
   hd_Wm_WBosonRapidity        -> Draw("e");
   hd_Wm_WBosonRapidity_BGsub  -> SetMarkerColor(kBlue);
   hd_Wm_WBosonRapidity_BGsub  -> SetMarkerStyle(21);
   hd_Wm_WBosonRapidity_BGsub  -> Draw("e same");
   WRAP_Bg_4 -> cd();   
   hZ_Wm_WBosonRapidity_scaled -> SetMinimum(0);  
   hZ_Wm_WBosonRapidity_scaled -> SetMaximum(10);   
   hZ_Wm_WBosonRapidity_scaled -> SetLineWidth(2); 
   hZ_Wm_WBosonRapidity_scaled -> SetLineColor(kRed);  
   hZ_Wm_WBosonRapidity_scaled -> SetFillColor(0);
   hd_Wm_QCDRapidity_scaled    -> SetLineWidth(2); 
   hd_Wm_QCDRapidity_scaled    -> SetLineColor(kGreen); 
   hd_Wm_QCDRapidity_scaled    -> SetFillColor(0);
   hWmtn_Wm_WBosonRapidity_scaled -> SetLineWidth(2);   
   hWmtn_Wm_WBosonRapidity_scaled -> SetLineColor(kBlue);  
   hWmtn_Wm_WBosonRapidity_scaled -> SetFillColor(0); 
   hMC_Wm_WBosonRapidity_total              -> SetLineWidth(2);  
   hMC_Wm_WBosonRapidity_total              -> SetLineStyle(7);   
   hMC_Wm_WBosonRapidity_total              -> SetLineColor(kBlack);   
   hMC_Wm_WBosonRapidity_total              -> SetFillColor(0);
   hZ_Wm_WBosonRapidity_scaled    -> SetTitle("run 11 - negative charge"); 
   hZ_Wm_WBosonRapidity_scaled    -> SetStats(0);   
   hZ_Wm_WBosonRapidity_scaled    -> Draw();   
   hd_Wm_QCDRapidity_scaled       -> Draw("same");     
   hWmtn_Wm_WBosonRapidity_scaled -> Draw("same");   
   hMC_Wm_WBosonRapidity_total    -> Draw("same"); 

   WRAP_Bg -> Print(outPath + "/plot_r11_BG_vs_WRap.eps");
   WRAP_Bg -> Print(outPath + "/plot_r11_BG_vs_WRap.png");


   TCanvas *WETA_Bg12    = new TCanvas("WETA_Bg12", "RUN 12 - WBosonEta", 800, 500);

   WETA_Bg12 -> Divide(2,2);

   WETA_Bg12_1 -> cd();
   hd12_Wp_WBosonEta        -> SetTitle("run 12 - positive charge");
   hd12_Wp_WBosonEta        -> SetMarkerStyle(20);
   hd12_Wp_WBosonEta        -> Draw("e");
   hd12_Wp_WBosonEta_BGsub  -> SetMarkerColor(kBlue);
   hd12_Wp_WBosonEta_BGsub  -> SetMarkerStyle(21);
   hd12_Wp_WBosonEta_BGsub  -> Draw("e same");
   WETA_Bg12_2 -> cd();   
   hZ12_Wp_WBosonEta_scaled -> SetMinimum(0);  
   hZ12_Wp_WBosonEta_scaled -> SetMaximum(50);   
   hZ12_Wp_WBosonEta_scaled -> SetLineWidth(2); 
   hZ12_Wp_WBosonEta_scaled -> SetLineColor(kRed);  
   hZ12_Wp_WBosonEta_scaled -> SetFillColor(0);
   hd12_Wp_QCDEta_scaled    -> SetLineWidth(2); 
   hd12_Wp_QCDEta_scaled    -> SetLineColor(kGreen); 
   hd12_Wp_QCDEta_scaled    -> SetFillColor(0);
   hWptn12_Wp_WBosonEta_scaled -> SetLineWidth(2);   
   hWptn12_Wp_WBosonEta_scaled -> SetLineColor(kBlue);  
   hWptn12_Wp_WBosonEta_scaled -> SetFillColor(0); 
   hMC12_Wp_WBosonEta_total              -> SetLineWidth(2);  
   hMC12_Wp_WBosonEta_total              -> SetLineStyle(7);   
   hMC12_Wp_WBosonEta_total              -> SetLineColor(kBlack);   
   hMC12_Wp_WBosonEta_total              -> SetFillColor(0);
   hZ12_Wp_WBosonEta_scaled    -> SetTitle("run 12 - positive charge"); 
   hZ12_Wp_WBosonEta_scaled    -> SetStats(0);   
   hZ12_Wp_WBosonEta_scaled    -> Draw();   
   hd12_Wp_QCDEta_scaled       -> Draw("same");     
   hWptn12_Wp_WBosonEta_scaled -> Draw("same");   
   hMC12_Wp_WBosonEta_total    -> Draw("same");

   leg_WETA_Bg               -> Draw();

   WETA_Bg12_3 -> cd();
   hd12_Wm_WBosonEta        -> SetTitle("run 12 - negative charge");
   hd12_Wm_WBosonEta        -> SetMarkerStyle(20);
   hd12_Wm_WBosonEta        -> Draw("e");
   hd12_Wm_WBosonEta_BGsub  -> SetMarkerColor(kBlue);
   hd12_Wm_WBosonEta_BGsub  -> SetMarkerStyle(21);
   hd12_Wm_WBosonEta_BGsub  -> Draw("e same");
   WETA_Bg12_4 -> cd();   
   hZ12_Wm_WBosonEta_scaled -> SetMinimum(0);  
   hZ12_Wm_WBosonEta_scaled -> SetMaximum(50);   
   hZ12_Wm_WBosonEta_scaled -> SetLineWidth(2); 
   hZ12_Wm_WBosonEta_scaled -> SetLineColor(kRed);  
   hZ12_Wm_WBosonEta_scaled -> SetFillColor(0);
   hd12_Wm_QCDEta_scaled    -> SetLineWidth(2); 
   hd12_Wm_QCDEta_scaled    -> SetLineColor(kGreen); 
   hd12_Wm_QCDEta_scaled    -> SetFillColor(0);
   hWmtn12_Wm_WBosonEta_scaled -> SetLineWidth(2);   
   hWmtn12_Wm_WBosonEta_scaled -> SetLineColor(kBlue);  
   hWmtn12_Wm_WBosonEta_scaled -> SetFillColor(0); 
   hMC12_Wm_WBosonEta_total    -> SetLineWidth(2);  
   hMC12_Wm_WBosonEta_total    -> SetLineStyle(7);   
   hMC12_Wm_WBosonEta_total    -> SetLineColor(kBlack);   
   hMC12_Wm_WBosonEta_total    -> SetFillColor(0);
   hZ12_Wm_WBosonEta_scaled    -> SetTitle("run 12 - negative charge"); 
   hZ12_Wm_WBosonEta_scaled    -> SetStats(0);   
   hZ12_Wm_WBosonEta_scaled    -> Draw();   
   hd12_Wm_QCDEta_scaled       -> Draw("same");     
   hWmtn12_Wm_WBosonEta_scaled -> Draw("same");   
   hMC12_Wm_WBosonEta_total    -> Draw("same"); 

   WETA_Bg12 -> Print(outPath + "/plot_r12_BG_vs_WEta.eps");
   WETA_Bg12 -> Print(outPath + "/plot_r12_BG_vs_WEta.png");


   TCanvas *WRAP_Bg12    = new TCanvas("WRAP_Bg12", "RUN 12 - WBosonRapidity", 800, 500);

   WRAP_Bg12 -> Divide(2,2);

   WRAP_Bg12_1 -> cd();
   hd12_Wp_WBosonRapidity        -> SetTitle("run 12 - positive charge");
   hd12_Wp_WBosonRapidity        -> SetMarkerStyle(20);
   hd12_Wp_WBosonRapidity        -> Draw("e");
   hd12_Wp_WBosonRapidity_BGsub  -> SetMarkerColor(kBlue);
   hd12_Wp_WBosonRapidity_BGsub  -> SetMarkerStyle(21);
   hd12_Wp_WBosonRapidity_BGsub  -> Draw("e same");
   WRAP_Bg12_2 -> cd();   
   hZ12_Wp_WBosonRapidity_scaled -> SetMinimum(0);  
   hZ12_Wp_WBosonRapidity_scaled -> SetMaximum(50);   
   hZ12_Wp_WBosonRapidity_scaled -> SetLineWidth(2); 
   hZ12_Wp_WBosonRapidity_scaled -> SetLineColor(kRed);  
   hZ12_Wp_WBosonRapidity_scaled -> SetFillColor(0);
   hd12_Wp_QCDRapidity_scaled    -> SetLineWidth(2); 
   hd12_Wp_QCDRapidity_scaled    -> SetLineColor(kGreen); 
   hd12_Wp_QCDRapidity_scaled    -> SetFillColor(0);
   hWptn12_Wp_WBosonRapidity_scaled -> SetLineWidth(2);   
   hWptn12_Wp_WBosonRapidity_scaled -> SetLineColor(kBlue);  
   hWptn12_Wp_WBosonRapidity_scaled -> SetFillColor(0); 
   hMC12_Wp_WBosonRapidity_total              -> SetLineWidth(2);  
   hMC12_Wp_WBosonRapidity_total              -> SetLineStyle(7);   
   hMC12_Wp_WBosonRapidity_total              -> SetLineColor(kBlack);   
   hMC12_Wp_WBosonRapidity_total              -> SetFillColor(0);
   hZ12_Wp_WBosonRapidity_scaled    -> SetTitle("run 12 - positive charge"); 
   hZ12_Wp_WBosonRapidity_scaled    -> SetStats(0);   
   hZ12_Wp_WBosonRapidity_scaled    -> Draw();   
   hd12_Wp_QCDRapidity_scaled       -> Draw("same");     
   hWptn12_Wp_WBosonRapidity_scaled -> Draw("same");   
   hMC12_Wp_WBosonRapidity_total    -> Draw("same");

   leg_WRAP_Bg               -> Draw();

   WRAP_Bg12_3 -> cd();
   hd12_Wm_WBosonRapidity        -> SetTitle("run 12 - negative charge");
   hd12_Wm_WBosonRapidity        -> SetMarkerStyle(20);
   hd12_Wm_WBosonRapidity        -> Draw("e");
   hd12_Wm_WBosonRapidity_BGsub  -> SetMarkerColor(kBlue);
   hd12_Wm_WBosonRapidity_BGsub  -> SetMarkerStyle(21);
   hd12_Wm_WBosonRapidity_BGsub  -> Draw("e same");
   WRAP_Bg12_4 -> cd();   
   hZ12_Wm_WBosonRapidity_scaled -> SetMinimum(0);  
   hZ12_Wm_WBosonRapidity_scaled -> SetMaximum(50);   
   hZ12_Wm_WBosonRapidity_scaled -> SetLineWidth(2); 
   hZ12_Wm_WBosonRapidity_scaled -> SetLineColor(kRed);  
   hZ12_Wm_WBosonRapidity_scaled -> SetFillColor(0);
   hd12_Wm_QCDRapidity_scaled    -> SetLineWidth(2); 
   hd12_Wm_QCDRapidity_scaled    -> SetLineColor(kGreen); 
   hd12_Wm_QCDRapidity_scaled    -> SetFillColor(0);
   hWmtn12_Wm_WBosonRapidity_scaled -> SetLineWidth(2);   
   hWmtn12_Wm_WBosonRapidity_scaled -> SetLineColor(kBlue);  
   hWmtn12_Wm_WBosonRapidity_scaled -> SetFillColor(0); 
   hMC12_Wm_WBosonRapidity_total    -> SetLineWidth(2);  
   hMC12_Wm_WBosonRapidity_total    -> SetLineStyle(7);   
   hMC12_Wm_WBosonRapidity_total    -> SetLineColor(kBlack);   
   hMC12_Wm_WBosonRapidity_total    -> SetFillColor(0);
   hZ12_Wm_WBosonRapidity_scaled    -> SetTitle("run 12 - negative charge"); 
   hZ12_Wm_WBosonRapidity_scaled    -> SetStats(0);   
   hZ12_Wm_WBosonRapidity_scaled    -> Draw();   
   hd12_Wm_QCDRapidity_scaled       -> Draw("same");     
   hWmtn12_Wm_WBosonRapidity_scaled -> Draw("same");   
   hMC12_Wm_WBosonRapidity_total    -> Draw("same"); 

   WRAP_Bg12 -> Print(outPath + "/plot_r12_BG_vs_WRap.eps");
   WRAP_Bg12 -> Print(outPath + "/plot_r12_BG_vs_WRap.png");



   TCanvas *EleETA_Bg    = new TCanvas("EleETA_Bg", "RUN 11 - ElectronEta", 800, 500);

   EleETA_Bg -> Divide(2,2);

   EleETA_Bg_1 -> cd();
   hd_Wp_ElectronEta        -> SetTitle("run 11 - positive charge");
   hd_Wp_ElectronEta        -> SetMarkerStyle(20);
   hd_Wp_ElectronEta        -> Draw("e1");
   hd_Wp_ElectronEta_BGsub  -> SetMarkerColor(kBlue);
   hd_Wp_ElectronEta_BGsub  -> SetMarkerStyle(21);
   hd_Wp_ElectronEta_BGsub  -> Draw("e1 same");
   EleETA_Bg_2 -> cd();   
   hZ_Wp_ElectronEta_scaled -> SetMinimum(0);  
   hZ_Wp_ElectronEta_scaled -> SetMaximum(15);   
   hZ_Wp_ElectronEta_scaled -> SetLineWidth(2); 
   hZ_Wp_ElectronEta_scaled -> SetLineColor(kRed);  
   hZ_Wp_ElectronEta_scaled -> SetFillColor(0);
   hd_Wp_QCDElectronEta_scaled    -> SetLineWidth(2); 
   hd_Wp_QCDElectronEta_scaled    -> SetLineColor(kGreen); 
   hd_Wp_QCDElectronEta_scaled    -> SetFillColor(0);
   hWptn_Wp_ElectronEta_scaled -> SetLineWidth(2);   
   hWptn_Wp_ElectronEta_scaled -> SetLineColor(kBlue);  
   hWptn_Wp_ElectronEta_scaled -> SetFillColor(0); 
   hMC_Wp_ElectronEta_total              -> SetLineWidth(2);  
   hMC_Wp_ElectronEta_total              -> SetLineStyle(7);   
   hMC_Wp_ElectronEta_total              -> SetLineColor(kBlack);   
   hMC_Wp_ElectronEta_total              -> SetFillColor(0);
   hZ_Wp_ElectronEta_scaled    -> SetTitle("run 11 - positive charge"); 
   hZ_Wp_ElectronEta_scaled    -> SetStats(0);   
   hZ_Wp_ElectronEta_scaled    -> Draw();   
   hd_Wp_QCDElectronEta_scaled -> Draw("same");     
   hWptn_Wp_ElectronEta_scaled -> Draw("same");   
   hMC_Wp_ElectronEta_total    -> Draw("same"); 
 
   TLegend *leg_EleETA_Bg = new TLegend(0.75, 0.65, 0.995, 0.99);
   leg_EleETA_Bg -> AddEntry(hd_Wp_ElectronEta,"STAR data", "P");
   leg_EleETA_Bg -> AddEntry(hd_Wp_ElectronEta_BGsub,"STAR data - Bg", "P");
   leg_EleETA_Bg -> AddEntry(hMC_Wp_ElectronEta_total,"TOTAL Bg. ", "L" );
   leg_EleETA_Bg -> AddEntry(hWptn_Wp_ElectronEta_scaled,"PYTHIA W-> #tau#nu_{#tau} ", "L" );
   leg_EleETA_Bg -> AddEntry(hZ_Wp_ElectronEta_scaled,"PYTHIA Z^{0}-> e^{+}e^{-}", "L");
   leg_EleETA_Bg -> AddEntry(hd_Wp_QCDElectronEta_scaled,"Data driven QCD", "L");
   leg_EleETA_Bg -> Draw();

   EleETA_Bg_3 -> cd();
   hd_Wm_ElectronEta        -> SetTitle("run 11 - negative charge");
   hd_Wm_ElectronEta        -> SetMarkerStyle(20);
   //hd_Wm_ElectronEta        -> SetMinimum(0);
   hd_Wm_ElectronEta        -> Draw("e1");
   hd_Wm_ElectronEta_BGsub  -> SetMarkerColor(kBlue);
   hd_Wm_ElectronEta_BGsub  -> SetMarkerStyle(21);
   hd_Wm_ElectronEta_BGsub  -> Draw("e1 same");
   EleETA_Bg_4 -> cd();   
   hZ_Wm_ElectronEta_scaled -> SetMinimum(0);  
   hZ_Wm_ElectronEta_scaled -> SetMaximum(10);   
   hZ_Wm_ElectronEta_scaled -> SetLineWidth(2); 
   hZ_Wm_ElectronEta_scaled -> SetLineColor(kRed);  
   hZ_Wm_ElectronEta_scaled -> SetFillColor(0);
   hd_Wm_QCDElectronEta_scaled    -> SetLineWidth(2); 
   hd_Wm_QCDElectronEta_scaled    -> SetLineColor(kGreen); 
   hd_Wm_QCDElectronEta_scaled    -> SetFillColor(0);
   hWmtn_Wm_ElectronEta_scaled -> SetLineWidth(2);   
   hWmtn_Wm_ElectronEta_scaled -> SetLineColor(kBlue);  
   hWmtn_Wm_ElectronEta_scaled -> SetFillColor(0); 
   hMC_Wm_ElectronEta_total              -> SetLineWidth(2);  
   hMC_Wm_ElectronEta_total              -> SetLineStyle(7);   
   hMC_Wm_ElectronEta_total              -> SetLineColor(kBlack);   
   hMC_Wm_ElectronEta_total              -> SetFillColor(0);
   hZ_Wm_ElectronEta_scaled    -> SetTitle("run 11 - negative charge"); 
   hZ_Wm_ElectronEta_scaled    -> SetStats(0);   
   hZ_Wm_ElectronEta_scaled    -> Draw();   
   hd_Wm_QCDElectronEta_scaled -> Draw("same");     
   hWmtn_Wm_ElectronEta_scaled -> Draw("same");   
   hMC_Wm_ElectronEta_total    -> Draw("same"); 

   EleETA_Bg -> Print(outPath + "/plot_r11_BG_vs_EleEta.eps");
   EleETA_Bg -> Print(outPath + "/plot_r11_BG_vs_EleEta.png");


   TCanvas *EleETA_Bg12    = new TCanvas("EleETA_Bg12", "RUN 12 - ElectronEta", 800, 500);

   EleETA_Bg12 -> Divide(2,2);

   EleETA_Bg12_1 -> cd();
   hd12_Wp_ElectronEta        -> SetTitle("run 12 - positive charge");
   hd12_Wp_ElectronEta        -> SetMarkerStyle(20);
   hd12_Wp_ElectronEta        -> Draw("e1");
   hd12_Wp_ElectronEta_BGsub  -> SetMarkerColor(kBlue);
   hd12_Wp_ElectronEta_BGsub  -> SetMarkerStyle(21);
   hd12_Wp_ElectronEta_BGsub  -> Draw("e1 same");
   EleETA_Bg12_2 -> cd();   
   hZ12_Wp_ElectronEta_scaled -> SetMinimum(0);  
   hZ12_Wp_ElectronEta_scaled -> SetMaximum(50);   
   hZ12_Wp_ElectronEta_scaled -> SetLineWidth(2); 
   hZ12_Wp_ElectronEta_scaled -> SetLineColor(kRed);  
   hZ12_Wp_ElectronEta_scaled -> SetFillColor(0);
   hd12_Wp_QCDElectronEta_scaled    -> SetLineWidth(2); 
   hd12_Wp_QCDElectronEta_scaled    -> SetLineColor(kGreen); 
   hd12_Wp_QCDElectronEta_scaled    -> SetFillColor(0);
   hWptn12_Wp_ElectronEta_scaled -> SetLineWidth(2);   
   hWptn12_Wp_ElectronEta_scaled -> SetLineColor(kBlue);  
   hWptn12_Wp_ElectronEta_scaled -> SetFillColor(0); 
   hMC12_Wp_ElectronEta_total              -> SetLineWidth(2);  
   hMC12_Wp_ElectronEta_total              -> SetLineStyle(7);   
   hMC12_Wp_ElectronEta_total              -> SetLineColor(kBlack);   
   hMC12_Wp_ElectronEta_total              -> SetFillColor(0);
   hZ12_Wp_ElectronEta_scaled    -> SetTitle("run 12 - positive charge"); 
   hZ12_Wp_ElectronEta_scaled    -> SetStats(0);   
   hZ12_Wp_ElectronEta_scaled    -> Draw();   
   hd12_Wp_QCDElectronEta_scaled -> Draw("same");     
   hWptn12_Wp_ElectronEta_scaled -> Draw("same");   
   hMC12_Wp_ElectronEta_total    -> Draw("same");

   leg_EleETA_Bg               -> Draw();

   EleETA_Bg12_3 -> cd();
   hd12_Wm_ElectronEta        -> SetTitle("run 12 - negative charge");
   hd12_Wm_ElectronEta        -> SetMarkerStyle(20);
   hd12_Wm_ElectronEta        -> SetMinimum(50);
   hd12_Wm_ElectronEta        -> Draw("e1");
   hd12_Wm_ElectronEta_BGsub  -> SetMarkerColor(kBlue);
   hd12_Wm_ElectronEta_BGsub  -> SetMarkerStyle(21);
   hd12_Wm_ElectronEta_BGsub  -> Draw("e1 same");
   EleETA_Bg12_4 -> cd();   
   hZ12_Wm_ElectronEta_scaled -> SetMinimum(0);  
   hZ12_Wm_ElectronEta_scaled -> SetMaximum(50);   
   hZ12_Wm_ElectronEta_scaled -> SetLineWidth(2); 
   hZ12_Wm_ElectronEta_scaled -> SetLineColor(kRed);  
   hZ12_Wm_ElectronEta_scaled -> SetFillColor(0);
   hd12_Wm_QCDElectronEta_scaled    -> SetLineWidth(2); 
   hd12_Wm_QCDElectronEta_scaled    -> SetLineColor(kGreen); 
   hd12_Wm_QCDElectronEta_scaled    -> SetFillColor(0);
   hWmtn12_Wm_ElectronEta_scaled -> SetLineWidth(2);   
   hWmtn12_Wm_ElectronEta_scaled -> SetLineColor(kBlue);  
   hWmtn12_Wm_ElectronEta_scaled -> SetFillColor(0); 
   hMC12_Wm_ElectronEta_total    -> SetLineWidth(2);  
   hMC12_Wm_ElectronEta_total    -> SetLineStyle(7);   
   hMC12_Wm_ElectronEta_total    -> SetLineColor(kBlack);   
   hMC12_Wm_ElectronEta_total    -> SetFillColor(0);
   hZ12_Wm_ElectronEta_scaled    -> SetTitle("run 12 - negative charge"); 
   hZ12_Wm_ElectronEta_scaled    -> SetStats(0);   
   hZ12_Wm_ElectronEta_scaled    -> Draw();   
   hd12_Wm_QCDElectronEta_scaled -> Draw("same");     
   hWmtn12_Wm_ElectronEta_scaled -> Draw("same");   
   hMC12_Wm_ElectronEta_total    -> Draw("same"); 

   EleETA_Bg12 -> Print(outPath + "/plot_r12_BG_vs_EleEta.eps");
   EleETA_Bg12 -> Print(outPath + "/plot_r12_BG_vs_EleEta.png");


   // MULTIPLY BY THE EFFICIENCY
   // the formula for the cross section ratio is
   // RW = sig_W+/sig_W- =  [(N - Bg)_W+] / [(N - Bg)_W-] * (Eff_W-/Eff_W+)

   TH1D *hd_Wp_ElectronEta_BGsub_EffCor   = (TH1D *)hd_Wp_ElectronEta_BGsub     -> Clone("hd_Wp_ElectronEta_BGsub_EffCor");
   hd_Wp_ElectronEta_BGsub_EffCor   -> Multiply(hd_Wp_ElectronEta_BGsub, hEfficiency_Wm_r11, 1, 1);

   TH1D *hd_Wm_ElectronEta_BGsub_EffCor   = (TH1D *)hd_Wm_ElectronEta_BGsub     -> Clone("hd_Wm_ElectronEta_BGsub_EffCor");
   hd_Wm_ElectronEta_BGsub_EffCor   -> Multiply(hd_Wm_ElectronEta_BGsub, hEfficiency_Wp_r11, 1, 1);

   TH1D *hd12_Wp_ElectronEta_BGsub_EffCor = (TH1D *)hd12_Wp_ElectronEta_BGsub -> Clone("hd_Wp_ElectronEta_BGsub_EffCor");
   hd12_Wp_ElectronEta_BGsub_EffCor -> Multiply(hd12_Wp_ElectronEta_BGsub, hEfficiency_Wm_r12, 1, 1);

   TH1D *hd12_Wm_ElectronEta_BGsub_EffCor = (TH1D *)hd12_Wm_ElectronEta_BGsub -> Clone("hd_Wp_ElectronEta_BGsub_EffCor");
   hd12_Wm_ElectronEta_BGsub_EffCor -> Multiply(hd12_Wm_ElectronEta_BGsub, hEfficiency_Wp_r12, 1, 1);



   TCanvas *EleETA_Bg_Eff    = new TCanvas("EleETA_Bg_Eff", "RUN 11 - ElectronEta", 800, 500);

   EleETA_Bg_Eff -> Divide(2,2);

   EleETA_Bg_Eff_1 -> cd();
   hd_Wp_ElectronEta        -> SetTitle("run 11 - positive charge");
   hd_Wp_ElectronEta        -> SetMarkerStyle(20);
   hd_Wp_ElectronEta        -> Draw("e");
   hd_Wp_ElectronEta_BGsub  -> SetMarkerColor(kBlue);
   hd_Wp_ElectronEta_BGsub  -> SetMarkerStyle(21);
   hd_Wp_ElectronEta_BGsub  -> Draw("e same");
   hd_Wp_ElectronEta_BGsub_EffCor  -> SetMarkerColor(kRed);
   hd_Wp_ElectronEta_BGsub_EffCor  -> SetMarkerStyle(22);
   hd_Wp_ElectronEta_BGsub_EffCor  -> Draw("e same");
   EleETA_Bg_Eff_2 -> cd();   
   hZ_Wp_ElectronEta_scaled -> SetMinimum(0);  
   hZ_Wp_ElectronEta_scaled -> SetMaximum(15);   
   hZ_Wp_ElectronEta_scaled -> SetLineWidth(2); 
   hZ_Wp_ElectronEta_scaled -> SetLineColor(kRed);  
   hZ_Wp_ElectronEta_scaled -> SetFillColor(0);
   hd_Wp_QCDElectronEta_scaled    -> SetLineWidth(2); 
   hd_Wp_QCDElectronEta_scaled    -> SetLineColor(kGreen); 
   hd_Wp_QCDElectronEta_scaled    -> SetFillColor(0);
   hWptn_Wp_ElectronEta_scaled -> SetLineWidth(2);   
   hWptn_Wp_ElectronEta_scaled -> SetLineColor(kBlue);  
   hWptn_Wp_ElectronEta_scaled -> SetFillColor(0); 
   hMC_Wp_ElectronEta_total              -> SetLineWidth(2);  
   hMC_Wp_ElectronEta_total              -> SetLineStyle(7);   
   hMC_Wp_ElectronEta_total              -> SetLineColor(kBlack);   
   hMC_Wp_ElectronEta_total              -> SetFillColor(0);
   hZ_Wp_ElectronEta_scaled    -> SetTitle("run 11 - positive charge"); 
   hZ_Wp_ElectronEta_scaled    -> SetStats(0);   
   hZ_Wp_ElectronEta_scaled    -> Draw();   
   hd_Wp_QCDElectronEta_scaled -> Draw("same");     
   hWptn_Wp_ElectronEta_scaled -> Draw("same");   
   hMC_Wp_ElectronEta_total    -> Draw("same"); 
 
   TLegend *leg_EleETA_Bg_Eff = new TLegend(0.75, 0.6, 0.995, 0.99);
   leg_EleETA_Bg_Eff -> AddEntry(hd_Wp_ElectronEta,"STAR data", "P");
   leg_EleETA_Bg_Eff -> AddEntry(hd_Wp_ElectronEta_BGsub,"STAR data - Bg", "P");
   leg_EleETA_Bg_Eff -> AddEntry(hd_Wp_ElectronEta_BGsub_EffCor,"data-Bg * Eff.", "P");
   leg_EleETA_Bg_Eff -> AddEntry(hMC_Wp_ElectronEta_total,"TOTAL Bg. ", "L" );
   leg_EleETA_Bg_Eff -> AddEntry(hWptn_Wp_ElectronEta_scaled,"PYTHIA W-> #tau#nu_{#tau} ", "L" );
   leg_EleETA_Bg_Eff -> AddEntry(hZ_Wp_ElectronEta_scaled,"PYTHIA Z^{0}-> e^{+}e^{-}", "L");
   leg_EleETA_Bg_Eff -> AddEntry(hd_Wp_QCDElectronEta_scaled,"Data driven QCD", "L");
   leg_EleETA_Bg_Eff -> Draw();

   EleETA_Bg_Eff_3 -> cd();
   hd_Wm_ElectronEta        -> SetTitle("run 11 - negative charge");
   hd_Wm_ElectronEta        -> SetMarkerStyle(20);
   hd_Wm_ElectronEta        -> Draw("e");
   hd_Wm_ElectronEta_BGsub  -> SetMarkerColor(kBlue);
   hd_Wm_ElectronEta_BGsub  -> SetMarkerStyle(21);
   hd_Wm_ElectronEta_BGsub  -> Draw("e same");
   hd_Wm_ElectronEta_BGsub_EffCor  -> SetMarkerColor(kRed);
   hd_Wm_ElectronEta_BGsub_EffCor  -> SetMarkerStyle(22);
   hd_Wm_ElectronEta_BGsub_EffCor  -> Draw("e same");
   EleETA_Bg_Eff_4 -> cd();   
   hZ_Wm_ElectronEta_scaled -> SetMinimum(0);  
   hZ_Wm_ElectronEta_scaled -> SetMaximum(10);   
   hZ_Wm_ElectronEta_scaled -> SetLineWidth(2); 
   hZ_Wm_ElectronEta_scaled -> SetLineColor(kRed);  
   hZ_Wm_ElectronEta_scaled -> SetFillColor(0);
   hd_Wm_QCDElectronEta_scaled    -> SetLineWidth(2); 
   hd_Wm_QCDElectronEta_scaled    -> SetLineColor(kGreen); 
   hd_Wm_QCDElectronEta_scaled    -> SetFillColor(0);
   hWmtn_Wm_ElectronEta_scaled -> SetLineWidth(2);   
   hWmtn_Wm_ElectronEta_scaled -> SetLineColor(kBlue);  
   hWmtn_Wm_ElectronEta_scaled -> SetFillColor(0); 
   hMC_Wm_ElectronEta_total              -> SetLineWidth(2);  
   hMC_Wm_ElectronEta_total              -> SetLineStyle(7);   
   hMC_Wm_ElectronEta_total              -> SetLineColor(kBlack);   
   hMC_Wm_ElectronEta_total              -> SetFillColor(0);
   hZ_Wm_ElectronEta_scaled    -> SetTitle("run 11 - negative charge"); 
   hZ_Wm_ElectronEta_scaled    -> SetStats(0);   
   hZ_Wm_ElectronEta_scaled    -> Draw();   
   hd_Wm_QCDElectronEta_scaled -> Draw("same");     
   hWmtn_Wm_ElectronEta_scaled -> Draw("same");   
   hMC_Wm_ElectronEta_total    -> Draw("same"); 

   EleETA_Bg_Eff -> Print(outPath + "/plot_r11_BG_EFF_vs_EleEta.eps");
   EleETA_Bg_Eff -> Print(outPath + "/plot_r11_BG_EFF_vs_EleEta.png");


   TCanvas *EleETA_Bg12_Eff    = new TCanvas("EleETA_Bg12_Eff", "RUN 12 - ElectronEta", 800, 500);

   EleETA_Bg12_Eff -> Divide(2,2);

   EleETA_Bg12_Eff_1 -> cd();
   hd12_Wp_ElectronEta        -> SetTitle("run 12 - positive charge");
   hd12_Wp_ElectronEta        -> SetMarkerStyle(20);
   hd12_Wp_ElectronEta        -> Draw("e");
   hd12_Wp_ElectronEta_BGsub  -> SetMarkerColor(kBlue);
   hd12_Wp_ElectronEta_BGsub  -> SetMarkerStyle(21);
   hd12_Wp_ElectronEta_BGsub  -> Draw("e same");
   hd12_Wp_ElectronEta_BGsub_EffCor  -> SetMarkerColor(kRed);
   hd12_Wp_ElectronEta_BGsub_EffCor  -> SetMarkerStyle(22);
   hd12_Wp_ElectronEta_BGsub_EffCor  -> Draw("e same");
   EleETA_Bg12_Eff_2 -> cd();   
   hZ12_Wp_ElectronEta_scaled -> SetMinimum(0);  
   hZ12_Wp_ElectronEta_scaled -> SetMaximum(50);   
   hZ12_Wp_ElectronEta_scaled -> SetLineWidth(2); 
   hZ12_Wp_ElectronEta_scaled -> SetLineColor(kRed);  
   hZ12_Wp_ElectronEta_scaled -> SetFillColor(0);
   hd12_Wp_QCDElectronEta_scaled    -> SetLineWidth(2); 
   hd12_Wp_QCDElectronEta_scaled    -> SetLineColor(kGreen); 
   hd12_Wp_QCDElectronEta_scaled    -> SetFillColor(0);
   hWptn12_Wp_ElectronEta_scaled -> SetLineWidth(2);   
   hWptn12_Wp_ElectronEta_scaled -> SetLineColor(kBlue);  
   hWptn12_Wp_ElectronEta_scaled -> SetFillColor(0); 
   hMC12_Wp_ElectronEta_total              -> SetLineWidth(2);  
   hMC12_Wp_ElectronEta_total              -> SetLineStyle(7);   
   hMC12_Wp_ElectronEta_total              -> SetLineColor(kBlack);   
   hMC12_Wp_ElectronEta_total              -> SetFillColor(0);
   hZ12_Wp_ElectronEta_scaled    -> SetTitle("run 12 - positive charge"); 
   hZ12_Wp_ElectronEta_scaled    -> SetStats(0);   
   hZ12_Wp_ElectronEta_scaled    -> Draw();   
   hd12_Wp_QCDElectronEta_scaled -> Draw("same");     
   hWptn12_Wp_ElectronEta_scaled -> Draw("same");   
   hMC12_Wp_ElectronEta_total    -> Draw("same");

   leg_EleETA_Bg_Eff               -> Draw();

   EleETA_Bg12_Eff_3 -> cd();
   hd12_Wm_ElectronEta        -> SetTitle("run 12 - negative charge");
   hd12_Wm_ElectronEta        -> SetMarkerStyle(20);
   hd12_Wm_ElectronEta        -> Draw("e");
   hd12_Wm_ElectronEta_BGsub  -> SetMarkerColor(kBlue);
   hd12_Wm_ElectronEta_BGsub  -> SetMarkerStyle(21);
   hd12_Wm_ElectronEta_BGsub  -> Draw("e same");
   hd12_Wm_ElectronEta_BGsub_EffCor  -> SetMarkerColor(kRed);
   hd12_Wm_ElectronEta_BGsub_EffCor  -> SetMarkerStyle(22);
   hd12_Wm_ElectronEta_BGsub_EffCor  -> Draw("e same");
   EleETA_Bg12_Eff_4 -> cd();   
   hZ12_Wm_ElectronEta_scaled -> SetMinimum(0);  
   hZ12_Wm_ElectronEta_scaled -> SetMaximum(50);   
   hZ12_Wm_ElectronEta_scaled -> SetLineWidth(2); 
   hZ12_Wm_ElectronEta_scaled -> SetLineColor(kRed);  
   hZ12_Wm_ElectronEta_scaled -> SetFillColor(0);
   hd12_Wm_QCDElectronEta_scaled    -> SetLineWidth(2); 
   hd12_Wm_QCDElectronEta_scaled    -> SetLineColor(kGreen); 
   hd12_Wm_QCDElectronEta_scaled    -> SetFillColor(0);
   hWmtn12_Wm_ElectronEta_scaled -> SetLineWidth(2);   
   hWmtn12_Wm_ElectronEta_scaled -> SetLineColor(kBlue);  
   hWmtn12_Wm_ElectronEta_scaled -> SetFillColor(0); 
   hMC12_Wm_ElectronEta_total    -> SetLineWidth(2);  
   hMC12_Wm_ElectronEta_total    -> SetLineStyle(7);   
   hMC12_Wm_ElectronEta_total    -> SetLineColor(kBlack);   
   hMC12_Wm_ElectronEta_total    -> SetFillColor(0);
   hZ12_Wm_ElectronEta_scaled    -> SetTitle("run 12 - negative charge"); 
   hZ12_Wm_ElectronEta_scaled    -> SetStats(0);   
   hZ12_Wm_ElectronEta_scaled    -> Draw();   
   hd12_Wm_QCDElectronEta_scaled -> Draw("same");     
   hWmtn12_Wm_ElectronEta_scaled -> Draw("same");   
   hMC12_Wm_ElectronEta_total    -> Draw("same"); 

   EleETA_Bg12_Eff -> Print(outPath + "/plot_r12_BG_EFF_vs_EleEta.eps");
   EleETA_Bg12_Eff -> Print(outPath + "/plot_r12_BG_EFF_vs_EleEta.png");

  
   // Calculate the ratios

   TH1D *hd_WpWmRatio                        = (TH1D*) hd_Wp_WBosonEta                -> Clone("hd_WpWmRatio");
   TH1D *hd_WpWmRatio_BGsub                  = (TH1D*) hd_Wp_WBosonEta_BGsub          -> Clone("hd_WpWmRatio_BGsub");
   TH1D *hd_WpWmRatio_vs_WRapidity           = (TH1D*) hd_Wp_WBosonRapidity           -> Clone("hd_WpWmRatio_vs_WRapidity");
   TH1D *hd_WpWmRatio_vs_WRapidity_BGsub     = (TH1D*) hd_Wp_WBosonRapidity_BGsub     -> Clone("hd_WpWmRatio_vs_WRapidity_BGsub");
   TH1D *hd_WpWmRatio_vs_EleEta              = (TH1D*) hd_Wp_ElectronEta              -> Clone("hd_WpWmRatio_vs_EleEta");
   TH1D *hd_WpWmRatio_vs_EleEta_BGsub        = (TH1D*) hd_Wp_ElectronEta_BGsub        -> Clone("hd_WpWmRatio_vs_EleEta_BGsub");
   TH1D *hd_WpWmRatio_vs_EleEta_BGsub_EffCor = (TH1D*) hd_Wp_ElectronEta_BGsub_EffCor -> Clone("hd_WpWmRatio_vs_EleEta_BGsub_EffCor");
   hd_Wp_WBosonEta         -> Sumw2();
   hd_Wm_WBosonEta         -> Sumw2();
   hd_Wp_WBosonEta_BGsub   -> Sumw2();
   hd_Wm_WBosonEta_BGsub   -> Sumw2();
   hd_Wp_WBosonRapidity         -> Sumw2();
   hd_Wm_WBosonRapidity         -> Sumw2();
   hd_Wp_WBosonRapidity_BGsub   -> Sumw2();
   hd_Wm_WBosonRapidity_BGsub   -> Sumw2();
   hd_Wp_ElectronEta       -> Sumw2();
   hd_Wm_ElectronEta       -> Sumw2();
   hd_Wp_ElectronEta_BGsub -> Sumw2();
   hd_Wm_ElectronEta_BGsub -> Sumw2();
   hd_Wp_ElectronEta_BGsub_EffCor -> Sumw2();
   hd_Wm_ElectronEta_BGsub_EffCor -> Sumw2();

   hd_WpWmRatio  -> Divide(hd_Wp_WBosonEta, hd_Wm_WBosonEta);
   hd_WpWmRatio  -> SetMaximum(6);
   hd_WpWmRatio  -> SetMinimum(0);
   hd_WpWmRatio  -> SetMarkerStyle(20);
   hd_WpWmRatio  -> SetTitle("run11; #eta_{W}; W^{+}/W^{-}");

   hd_WpWmRatio_BGsub  -> Divide(hd_Wp_WBosonEta_BGsub, hd_Wm_WBosonEta_BGsub);
   hd_WpWmRatio_BGsub  -> SetMaximum(6);
   hd_WpWmRatio_BGsub  -> SetMinimum(0);
   hd_WpWmRatio_BGsub  -> SetMarkerStyle(20);
   hd_WpWmRatio_BGsub  -> SetTitle("run11 - Bg; #eta_{W}; W^{+}/W^{-}");

   hd_WpWmRatio_vs_WRapidity  -> Divide(hd_Wp_WBosonRapidity, hd_Wm_WBosonRapidity);
   hd_WpWmRatio_vs_WRapidity  -> SetMaximum(6);
   hd_WpWmRatio_vs_WRapidity  -> SetMinimum(0);
   hd_WpWmRatio_vs_WRapidity  -> SetMarkerStyle(20);
   hd_WpWmRatio_vs_WRapidity  -> SetTitle("run11; y_{W}; W^{+}/W^{-}");

   hd_WpWmRatio_vs_WRapidity_BGsub  -> Divide(hd_Wp_WBosonRapidity_BGsub, hd_Wm_WBosonRapidity_BGsub);
   hd_WpWmRatio_vs_WRapidity_BGsub  -> SetMaximum(6);
   hd_WpWmRatio_vs_WRapidity_BGsub  -> SetMinimum(0);
   hd_WpWmRatio_vs_WRapidity_BGsub  -> SetMarkerStyle(20);
   hd_WpWmRatio_vs_WRapidity_BGsub  -> SetTitle("run11 - Bg; y_{W}; W^{+}/W^{-}");

   hd_WpWmRatio_vs_EleEta  -> Divide(hd_Wp_ElectronEta, hd_Wm_ElectronEta);
   hd_WpWmRatio_vs_EleEta  -> SetMaximum(7.5);
   hd_WpWmRatio_vs_EleEta  -> SetMinimum(0);
   hd_WpWmRatio_vs_EleEta  -> SetMarkerStyle(20);
   hd_WpWmRatio_vs_EleEta  -> SetTitle("run11; #eta_{el}; W^{+}/W^{-}");

   hd_WpWmRatio_vs_EleEta_BGsub  -> Divide(hd_Wp_ElectronEta_BGsub, hd_Wm_ElectronEta_BGsub);
   hd_WpWmRatio_vs_EleEta_BGsub  -> SetMaximum(7.5);
   hd_WpWmRatio_vs_EleEta_BGsub  -> SetMinimum(0);
   hd_WpWmRatio_vs_EleEta_BGsub  -> SetMarkerStyle(20);
   hd_WpWmRatio_vs_EleEta_BGsub  -> SetTitle("run11 - Bg; #eta_{el}; W^{+}/W^{-}");

   hd_WpWmRatio_vs_EleEta_BGsub_EffCor  -> Divide(hd_Wp_ElectronEta_BGsub_EffCor, hd_Wm_ElectronEta_BGsub_EffCor);
   hd_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMaximum(7.5);
   hd_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMinimum(0);
   hd_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMarkerStyle(20);
   hd_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetTitle("run11 - Bg * Efficiency; #eta_{el}; W^{+}/W^{-}");

   TH1D *hd12_WpWmRatio                        = (TH1D*) hd12_Wp_WBosonEta                -> Clone("hd12_WpWmRatio");
   TH1D *hd12_WpWmRatio_BGsub                  = (TH1D*) hd12_Wp_WBosonEta_BGsub          -> Clone("hd12_WpWmRatio_BGsub");
   TH1D *hd12_WpWmRatio_vs_WRapidity           = (TH1D*) hd12_Wp_WBosonRapidity           -> Clone("hd12_WpWmRatio_vs_WRapidity");
   TH1D *hd12_WpWmRatio_vs_WRapidity_BGsub     = (TH1D*) hd12_Wp_WBosonRapidity_BGsub     -> Clone("hd12_WpWmRatio_vs_WRapidity_BGsub");
   TH1D *hd12_WpWmRatio_vs_EleEta              = (TH1D*) hd12_Wp_ElectronEta              -> Clone("hd12_WpWmRatio_vs_EleEta");
   TH1D *hd12_WpWmRatio_vs_EleEta_BGsub        = (TH1D*) hd12_Wp_ElectronEta_BGsub        -> Clone("hd12_WpWmRatio_vs_EleEta_BGsub");
   TH1D *hd12_WpWmRatio_vs_EleEta_BGsub_EffCor = (TH1D*) hd12_Wp_ElectronEta_BGsub_EffCor -> Clone("hd_WpWmRatio_vs_EleEta_BGsub_EffCor");
   hd12_Wp_WBosonEta         -> Sumw2();
   hd12_Wm_WBosonEta         -> Sumw2();
   hd12_Wp_WBosonEta_BGsub   -> Sumw2();
   hd12_Wm_WBosonEta_BGsub   -> Sumw2();
   hd12_Wp_WBosonRapidity         -> Sumw2();
   hd12_Wm_WBosonRapidity         -> Sumw2();
   hd12_Wp_WBosonRapidity_BGsub   -> Sumw2();
   hd12_Wm_WBosonRapidity_BGsub   -> Sumw2();
   hd12_Wp_ElectronEta       -> Sumw2();
   hd12_Wm_ElectronEta       -> Sumw2();
   hd12_Wp_ElectronEta_BGsub -> Sumw2();
   hd12_Wm_ElectronEta_BGsub -> Sumw2();
   hd12_Wp_ElectronEta_BGsub_EffCor -> Sumw2();
   hd12_Wm_ElectronEta_BGsub_EffCor -> Sumw2();

   hd12_WpWmRatio  -> Divide(hd12_Wp_WBosonEta, hd12_Wm_WBosonEta);
   hd12_WpWmRatio  -> SetMaximum(6);
   hd12_WpWmRatio  -> SetMinimum(0);
   hd12_WpWmRatio  -> SetMarkerStyle(20);
   hd12_WpWmRatio  -> SetTitle("run 12; #eta_{W}; W^{+}/W^{-}");

   hd12_WpWmRatio_BGsub  -> Divide(hd12_Wp_WBosonEta_BGsub, hd12_Wm_WBosonEta_BGsub);
   hd12_WpWmRatio_BGsub  -> SetMaximum(6);
   hd12_WpWmRatio_BGsub  -> SetMinimum(0);
   hd12_WpWmRatio_BGsub  -> SetMarkerStyle(20);
   hd12_WpWmRatio_BGsub  -> SetTitle("run12 - Bg; #eta_{W}; W^{+}/W^{-}");

   hd12_WpWmRatio_vs_WRapidity  -> Divide(hd12_Wp_WBosonRapidity, hd12_Wm_WBosonRapidity);
   hd12_WpWmRatio_vs_WRapidity  -> SetMaximum(6);
   hd12_WpWmRatio_vs_WRapidity  -> SetMinimum(0);
   hd12_WpWmRatio_vs_WRapidity  -> SetMarkerStyle(20);
   hd12_WpWmRatio_vs_WRapidity  -> SetTitle("run 12; y_{W}; W^{+}/W^{-}");

   hd12_WpWmRatio_vs_WRapidity_BGsub  -> Divide(hd12_Wp_WBosonRapidity_BGsub, hd12_Wm_WBosonRapidity_BGsub);
   hd12_WpWmRatio_vs_WRapidity_BGsub  -> SetMaximum(6);
   hd12_WpWmRatio_vs_WRapidity_BGsub  -> SetMinimum(0);
   hd12_WpWmRatio_vs_WRapidity_BGsub  -> SetMarkerStyle(20);
   hd12_WpWmRatio_vs_WRapidity_BGsub  -> SetTitle("run12 - Bg; y_{W}; W^{+}/W^{-}");

   hd12_WpWmRatio_vs_EleEta  -> Divide(hd12_Wp_ElectronEta, hd12_Wm_ElectronEta);
   hd12_WpWmRatio_vs_EleEta  -> SetMaximum(7.5);
   hd12_WpWmRatio_vs_EleEta  -> SetMinimum(0);
   hd12_WpWmRatio_vs_EleEta  -> SetMarkerStyle(20);
   hd12_WpWmRatio_vs_EleEta  -> SetTitle("run12; #eta_{el}; W^{+}/W^{-}");

   hd12_WpWmRatio_vs_EleEta_BGsub  -> Divide(hd12_Wp_ElectronEta_BGsub, hd12_Wm_ElectronEta_BGsub);
   hd12_WpWmRatio_vs_EleEta_BGsub  -> SetMaximum(7.5);
   hd12_WpWmRatio_vs_EleEta_BGsub  -> SetMinimum(0);
   hd12_WpWmRatio_vs_EleEta_BGsub  -> SetMarkerStyle(20);
   hd12_WpWmRatio_vs_EleEta_BGsub  -> SetTitle("run12 - Bg; #eta_{el}; W^{+}/W^{-}");

   hd12_WpWmRatio_vs_EleEta_BGsub_EffCor  -> Divide(hd12_Wp_ElectronEta_BGsub_EffCor, hd12_Wm_ElectronEta_BGsub_EffCor);
   hd12_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMaximum(7.5);
   hd12_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMinimum(0);
   hd12_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMarkerStyle(20);
   hd12_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetTitle("run11 - Bg * Efficiency; #eta_{el}; W^{+}/W^{-}");


   TH1D *hd1112_Wp_WBosonEta                     = (TH1D*) hd12_Wp_WBosonEta                -> Clone("hd1112_Wp_WBosonEta");
   TH1D *hd1112_Wm_WBosonEta                     = (TH1D*) hd12_Wm_WBosonEta                -> Clone("hd1112_Wm_WBosonEta");  
   TH1D *hd1112_Wp_WBosonEta_BGsub               = (TH1D*) hd12_Wp_WBosonEta_BGsub          -> Clone("hd1112_Wp_WBosonEta_BGsub");
   TH1D *hd1112_Wm_WBosonEta_BGsub               = (TH1D*) hd12_Wm_WBosonEta_BGsub          -> Clone("hd1112_Wm_WBosonEta_BGsub");  

   TH1D *hd1112_Wp_WBosonRapidity                = (TH1D*) hd12_Wp_WBosonRapidity           -> Clone("hd1112_Wp_WBosonRapidity");
   TH1D *hd1112_Wm_WBosonRapidity                = (TH1D*) hd12_Wm_WBosonRapidity           -> Clone("hd1112_Wm_WBosonRapidity");  
   TH1D *hd1112_Wp_WBosonRapidity_BGsub          = (TH1D*) hd12_Wp_WBosonRapidity_BGsub     -> Clone("hd1112_Wp_WBosonRapidity_BGsub");
   TH1D *hd1112_Wm_WBosonRapidity_BGsub          = (TH1D*) hd12_Wm_WBosonRapidity_BGsub     -> Clone("hd1112_Wm_WBosonRapidity_BGsub");   

   TH1D *hd1112_Wp_ElectronEta                   = (TH1D*) hd12_Wp_ElectronEta              -> Clone("hd1112_Wp_ElectronEta");
   TH1D *hd1112_Wm_ElectronEta                   = (TH1D*) hd12_Wm_ElectronEta              -> Clone("hd1112_Wm_ElectronEta");         
   TH1D *hd1112_Wp_ElectronEta_BGsub             = (TH1D*) hd12_Wp_ElectronEta_BGsub        -> Clone("hd1112_Wp_ElectronEta_BGsub");
   TH1D *hd1112_Wm_ElectronEta_BGsub             = (TH1D*) hd12_Wm_ElectronEta_BGsub        -> Clone("hd1112_Wm_ElectronEta_BGsub");           
   TH1D *hd1112_Wp_ElectronEta_BGsub_EffCor      = (TH1D*) hd12_Wp_ElectronEta_BGsub        -> Clone("hd1112_Wp_ElectronEta_BGsub_EffCor");
   TH1D *hd1112_Wm_ElectronEta_BGsub_EffCor      = (TH1D*) hd12_Wm_ElectronEta_BGsub        -> Clone("hd1112_Wm_ElectronEta_BGsub_EffCor");      

   TH1D *hd1112_WpWmRatio                        = (TH1D*) hd12_Wp_WBosonEta                -> Clone("hd1112_WpWmRatio");    
   TH1D *hd1112_WpWmRatio_BGsub                  = (TH1D*) hd12_Wp_WBosonEta_BGsub          -> Clone("hd1112_WpWmRatio_BGsub");    

   TH1D *hd1112_WpWmRatio_vs_WRapidity           = (TH1D*) hd12_Wp_WBosonRapidity           -> Clone("hd1112_WpWmRatio_vs_WRapidity");    
   TH1D *hd1112_WpWmRatio_vs_WRapidity_BGsub     = (TH1D*) hd12_Wp_WBosonRapidity_BGsub     -> Clone("hd1112_WpWmRatio_vs_WRapidity_BGsub");     

   TH1D *hd1112_WpWmRatio_vs_EleEta              = (TH1D*) hd12_Wp_ElectronEta              -> Clone("hd1112_WpWmRatio_vs_EleEta");         
   TH1D *hd1112_WpWmRatio_vs_EleEta_BGsub        = (TH1D*) hd12_Wp_ElectronEta_BGsub        -> Clone("hd1112_WpWmRatio_vs_EleEta_BGsub");
   TH1D *hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor = (TH1D*) hd12_Wp_ElectronEta_BGsub_EffCor -> Clone("hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor");

   hd1112_Wp_WBosonEta   -> Sumw2();
   hd1112_Wm_WBosonEta   -> Sumw2();
   hd1112_Wp_WBosonEta_BGsub   -> Sumw2();
   hd1112_Wm_WBosonEta_BGsub   -> Sumw2();
   hd1112_Wp_WBosonRapidity   -> Sumw2();
   hd1112_Wm_WBosonRapidity   -> Sumw2();
   hd1112_Wp_WBosonRapidity_BGsub   -> Sumw2();
   hd1112_Wm_WBosonRapidity_BGsub   -> Sumw2();
   hd1112_Wp_ElectronEta -> Sumw2();
   hd1112_Wm_ElectronEta -> Sumw2();
   hd1112_Wp_ElectronEta_BGsub -> Sumw2();
   hd1112_Wm_ElectronEta_BGsub -> Sumw2();
   hd1112_Wp_ElectronEta_BGsub_EffCor -> Sumw2();
   hd1112_Wm_ElectronEta_BGsub_EffCor -> Sumw2();

   hd1112_Wp_WBosonEta         -> Add(hd_Wp_WBosonEta, hd12_Wp_WBosonEta);
   hd1112_Wm_WBosonEta         -> Add(hd_Wm_WBosonEta, hd12_Wm_WBosonEta);
   hd1112_Wp_WBosonEta_BGsub   -> Add(hd_Wp_WBosonEta_BGsub, hd12_Wp_WBosonEta_BGsub);
   hd1112_Wm_WBosonEta_BGsub   -> Add(hd_Wm_WBosonEta_BGsub, hd12_Wm_WBosonEta_BGsub);

   hd1112_Wp_WBosonRapidity         -> Add(hd_Wp_WBosonRapidity, hd12_Wp_WBosonRapidity);
   hd1112_Wm_WBosonRapidity         -> Add(hd_Wm_WBosonRapidity, hd12_Wm_WBosonRapidity);
   hd1112_Wp_WBosonRapidity_BGsub   -> Add(hd_Wp_WBosonRapidity_BGsub, hd12_Wp_WBosonRapidity_BGsub);
   hd1112_Wm_WBosonRapidity_BGsub   -> Add(hd_Wm_WBosonRapidity_BGsub, hd12_Wm_WBosonRapidity_BGsub);

   hd1112_Wp_ElectronEta              -> Add(hd_Wp_ElectronEta, hd12_Wp_ElectronEta);
   hd1112_Wm_ElectronEta              -> Add(hd_Wm_ElectronEta, hd12_Wm_ElectronEta);
   hd1112_Wp_ElectronEta_BGsub        -> Add(hd_Wp_ElectronEta_BGsub, hd12_Wp_ElectronEta_BGsub);
   hd1112_Wm_ElectronEta_BGsub        -> Add(hd_Wm_ElectronEta_BGsub, hd12_Wm_ElectronEta_BGsub);
   hd1112_Wp_ElectronEta_BGsub_EffCor -> Add(hd_Wp_ElectronEta_BGsub_EffCor, hd12_Wp_ElectronEta_BGsub_EffCor);
   hd1112_Wm_ElectronEta_BGsub_EffCor -> Add(hd_Wm_ElectronEta_BGsub_EffCor, hd12_Wm_ElectronEta_BGsub_EffCor);

   hd1112_WpWmRatio  -> Divide(hd1112_Wp_WBosonEta, hd1112_Wm_WBosonEta);
   hd1112_WpWmRatio  -> SetMaximum(6);
   hd1112_WpWmRatio  -> SetMinimum(0);
   hd1112_WpWmRatio  -> SetMarkerStyle(20);
   hd1112_WpWmRatio  -> SetTitle("run 11+12; #eta_{W}; W^{+}/W^{-}");

   hd1112_WpWmRatio_BGsub  -> Divide(hd1112_Wp_WBosonEta_BGsub, hd1112_Wm_WBosonEta_BGsub);
   hd1112_WpWmRatio_BGsub  -> SetMaximum(6);
   hd1112_WpWmRatio_BGsub  -> SetMinimum(0);
   hd1112_WpWmRatio_BGsub  -> SetMarkerStyle(20);
   hd1112_WpWmRatio_BGsub  -> SetTitle("run 11+12 - Bg; #eta_{W}; W^{+}/W^{-}");

   hd1112_WpWmRatio_vs_WRapidity  -> Divide(hd1112_Wp_WBosonRapidity, hd1112_Wm_WBosonRapidity);
   hd1112_WpWmRatio_vs_WRapidity  -> SetMaximum(6);
   hd1112_WpWmRatio_vs_WRapidity  -> SetMinimum(0);
   hd1112_WpWmRatio_vs_WRapidity  -> SetMarkerStyle(20);
   hd1112_WpWmRatio_vs_WRapidity  -> SetTitle("run 11+12; y_{W}; W^{+}/W^{-}");

   hd1112_WpWmRatio_vs_WRapidity_BGsub  -> Divide(hd1112_Wp_WBosonRapidity_BGsub, hd1112_Wm_WBosonRapidity_BGsub);
   hd1112_WpWmRatio_vs_WRapidity_BGsub  -> SetMaximum(6);
   hd1112_WpWmRatio_vs_WRapidity_BGsub  -> SetMinimum(0);
   hd1112_WpWmRatio_vs_WRapidity_BGsub  -> SetMarkerStyle(20);
   hd1112_WpWmRatio_vs_WRapidity_BGsub  -> SetTitle("run 11+12 - Bg; y_{W}; W^{+}/W^{-}");

   hd1112_WpWmRatio_vs_EleEta  -> Divide(hd1112_Wp_ElectronEta, hd1112_Wm_ElectronEta);
   hd1112_WpWmRatio_vs_EleEta  -> SetMaximum(7.5);
   hd1112_WpWmRatio_vs_EleEta  -> SetMinimum(0);
   hd1112_WpWmRatio_vs_EleEta  -> SetMarkerStyle(20);
   hd1112_WpWmRatio_vs_EleEta  -> SetTitle("run 11+12; #eta_{el}; W^{+}/W^{-}");

   hd1112_WpWmRatio_vs_EleEta_BGsub  -> Divide(hd1112_Wp_ElectronEta_BGsub, hd1112_Wm_ElectronEta_BGsub);
   hd1112_WpWmRatio_vs_EleEta_BGsub  -> SetMaximum(7.5);
   hd1112_WpWmRatio_vs_EleEta_BGsub  -> SetMinimum(0);
   hd1112_WpWmRatio_vs_EleEta_BGsub  -> SetMarkerStyle(20);
   hd1112_WpWmRatio_vs_EleEta_BGsub  -> SetTitle("run 11+12 - Bg; #eta_{el}; W^{+}/W^{-}");

   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> Divide(hd1112_Wp_ElectronEta_BGsub_EffCor, hd1112_Wm_ElectronEta_BGsub_EffCor);
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMaximum(7.5);
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMinimum(0);
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetMarkerStyle(20);
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SetTitle("run 11+12 - Bg * Efficiency; #eta_{el}; W^{+}/W^{-}");


   /*
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
   */


   // run 11 over run 12 ratios for cross check,
   // they should look flat versus kinematics and 
   // scale as the luminosity
   TH1D *hd_Wp_11o12Ratio_vs_WBosonEta_BGsub       = (TH1D*) hd_Wp_WBosonEta_BGsub          -> Clone("hd_Wp_11o12Ratio_vs_WBosonEta_BGsub");
   TH1D *hd_Wm_11o12Ratio_vs_WBosonEta_BGsub       = (TH1D*) hd_Wm_WBosonEta_BGsub          -> Clone("hd_Wm_11o12Ratio_vs_WBosonEta_BGsub");

   TH1D *hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub  = (TH1D*) hd_Wp_WBosonRapidity_BGsub     -> Clone("hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub");
   TH1D *hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub  = (TH1D*) hd_Wm_WBosonRapidity_BGsub     -> Clone("hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub");

   TH1D *hd_Wp_11o12Ratio_vs_EleEta_BGsub          = (TH1D*) hd_Wp_ElectronEta_BGsub        -> Clone("hd_Wp_11o12Ratio_vs_EleEta_BGsub");
   TH1D *hd_Wm_11o12Ratio_vs_EleEta_BGsub          = (TH1D*) hd_Wm_ElectronEta_BGsub        -> Clone("hd_Wm_11o12Ratio_vs_EleEta_BGsub");
   TH1D *hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor   = (TH1D*) hd_Wp_ElectronEta_BGsub_EffCor -> Clone("hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor");
   TH1D *hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor   = (TH1D*) hd_Wm_ElectronEta_BGsub_EffCor -> Clone("hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor");

   hd_Wp_11o12Ratio_vs_WBosonEta_BGsub  -> Divide(hd_Wp_WBosonEta_BGsub, hd12_Wp_WBosonEta_BGsub);
   hd_Wp_11o12Ratio_vs_WBosonEta_BGsub  -> SetMaximum(1);
   hd_Wp_11o12Ratio_vs_WBosonEta_BGsub  -> SetMinimum(0);
   hd_Wp_11o12Ratio_vs_WBosonEta_BGsub  -> SetMarkerStyle(20);
   hd_Wp_11o12Ratio_vs_WBosonEta_BGsub  -> SetTitleOffset(1.3, "y");
   hd_Wp_11o12Ratio_vs_WBosonEta_BGsub  -> SetTitle("Data - Bg; #eta_{W}; [run11 W^{+}]/[run 12 W^{+}]");

   hd_Wm_11o12Ratio_vs_WBosonEta_BGsub  -> Divide(hd_Wm_WBosonEta_BGsub, hd12_Wm_WBosonEta_BGsub);
   hd_Wm_11o12Ratio_vs_WBosonEta_BGsub  -> SetMaximum(1);
   hd_Wm_11o12Ratio_vs_WBosonEta_BGsub  -> SetMinimum(0);
   hd_Wm_11o12Ratio_vs_WBosonEta_BGsub  -> SetMarkerStyle(20);
   hd_Wm_11o12Ratio_vs_WBosonEta_BGsub  -> SetTitleOffset(1.3, "y");
   hd_Wm_11o12Ratio_vs_WBosonEta_BGsub  -> SetTitle("Data - Bg; #eta_{W}; [run11 W^{-}]/[run 12 W^{-}]");

   hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub  -> Divide(hd_Wp_WBosonRapidity_BGsub, hd12_Wp_WBosonRapidity_BGsub);
   hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetMaximum(1);
   hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetMinimum(0);
   hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetMarkerStyle(20);
   hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetTitleOffset(1.3, "y");
   hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetTitle("Data - Bg; y_{W}; [run11 W^{+}]/[run 12 W^{+}]");

   hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub  -> Divide(hd_Wm_WBosonRapidity_BGsub, hd12_Wm_WBosonRapidity_BGsub);
   hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetMaximum(1);
   hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetMinimum(0);
   hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetMarkerStyle(20);
   hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetTitleOffset(1.3, "y");
   hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub  -> SetTitle("Data - Bg; y_{W}; [run11 W^{-}]/[run 12 W^{-}]");

   hd_Wp_11o12Ratio_vs_EleEta_BGsub  -> Divide(hd_Wp_ElectronEta_BGsub, hd12_Wp_ElectronEta_BGsub);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub  -> SetMaximum(1);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub  -> SetMinimum(0);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub  -> SetMarkerStyle(20);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub  -> SetTitleOffset(1.3, "y");
   hd_Wp_11o12Ratio_vs_EleEta_BGsub  -> SetTitle("Data - Bg; #eta_{el}; [run11 W^{+}]/[run 12 W^{+}]");

   hd_Wm_11o12Ratio_vs_EleEta_BGsub  -> Divide(hd_Wm_ElectronEta_BGsub, hd12_Wm_ElectronEta_BGsub);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub  -> SetMaximum(1);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub  -> SetMinimum(0);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub  -> SetMarkerStyle(20);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub  -> SetTitleOffset(1.3, "y");
   hd_Wm_11o12Ratio_vs_EleEta_BGsub  -> SetTitle("Data - Bg; #eta_{el}; [run11 W^{-}]/[run 12 W^{-}]");

   // It scales as  * (Eff_run12/Eff_run11)
   TH1D *hd_Wp_Eff12oEff11     = (TH1D*) hEfficiency_Wm_r12 -> Clone("hd_Wp_Eff12oEff11");
   hd_Wp_Eff12oEff11 -> Divide(hEfficiency_Wp_r12, hEfficiency_Wp_r11, 1, 1);
   TH1D *hd_Wm_Eff12oEff11     = (TH1D*) hEfficiency_Wm_r12 -> Clone("hd_Wm_Eff12oEff11");
   hd_Wm_Eff12oEff11 -> Divide(hEfficiency_Wm_r12, hEfficiency_Wm_r11, 1, 1);

   hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor  -> Divide(hd_Wp_ElectronEta_BGsub, hd12_Wp_ElectronEta_BGsub);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor  -> Multiply(hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor, hd_Wp_Eff12oEff11, 1, 1);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetMaximum(1);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetMinimum(0);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetMarkerStyle(20);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetTitleOffset(1.3, "y");
   hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetTitle("Data - Bg * Efficiency; #eta_{el}; [run11 W^{+}]/[run 12 W^{+}]");

   hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor  -> Divide(hd_Wm_ElectronEta_BGsub, hd12_Wm_ElectronEta_BGsub);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor  -> Multiply(hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor, hd_Wm_Eff12oEff11, 1, 1);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetMaximum(1);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetMinimum(0);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetMarkerStyle(20);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetTitleOffset(1.3, "y");
   hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor  -> SetTitle("Data - Bg * Efficiency; #eta_{el}; [run11 W^{-}]/[run 12 W^{-}]");


   // Save the histogram files used for systematics
   if (syQCDp) hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SaveAs(outPath + "/syQCDp_hd1112_WpWmRatio_vs_EleEta.root"); 
   if (syQCDm) hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SaveAs(outPath + "/syQCDm_hd1112_WpWmRatio_vs_EleEta.root"); 

   if (syQCDp) hd1112_WpWmRatio_BGsub                   -> SaveAs(outPath + "/syQCDp_hd1112_WpWmRatio_vs_WBosonEta.root"); 
   if (syQCDm) hd1112_WpWmRatio_BGsub                   -> SaveAs(outPath + "/syQCDm_hd1112_WpWmRatio_vs_WBosonEta.root");  

   if (syQCDp) hd1112_WpWmRatio_vs_WRapidity_BGsub -> SaveAs(outPath + "/syQCDp_hd1112_WpWmRatio_vs_WBosonRapidity.root"); 
   if (syQCDm) hd1112_WpWmRatio_vs_WRapidity_BGsub -> SaveAs(outPath + "/syQCDm_hd1112_WpWmRatio_vs_WBosonRapidity.root"); 


   if (syZ0p)  hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SaveAs(outPath + "/syZ0p_hd1112_WpWmRatio_vs_EleEta.root"); 
   if (syZ0m)  hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> SaveAs(outPath + "/syZ0m_hd1112_WpWmRatio_vs_EleEta.root"); 

   if (syZ0p)  hd1112_WpWmRatio_BGsub                   -> SaveAs(outPath + "/syZ0p_hd1112_WpWmRatio_vs_WBosonEta.root"); 
   if (syZ0m)  hd1112_WpWmRatio_BGsub                   -> SaveAs(outPath + "/syZ0m_hd1112_WpWmRatio_vs_WBosonEta.root");  

   if (syZ0p)  hd1112_WpWmRatio_vs_WRapidity_BGsub -> SaveAs(outPath + "/syZ0p_hd1112_WpWmRatio_vs_WBosonRapidity.root"); 
   if (syZ0m)  hd1112_WpWmRatio_vs_WRapidity_BGsub -> SaveAs(outPath + "/syZ0m_hd1112_WpWmRatio_vs_WBosonRapidity.root"); 

   if (systematics) return; // if this is for syst. calc. STOP here 


   // Calculate the systematic uncertainties on QCD background normalization
   TFile *file_syQCDp_EleEta = TFile::Open(outPath + "/syQCDp_hd1112_WpWmRatio_vs_EleEta.root");
   TH1   *syQCDp_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor = (TH1*) file_syQCDp_EleEta -> Get("hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor");

   TFile *file_syQCDm_EleEta = TFile::Open(outPath + "/syQCDm_hd1112_WpWmRatio_vs_EleEta.root");
   TH1   *syQCDm_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor = (TH1*) file_syQCDm_EleEta -> Get("hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor");

   TFile *file_syZ0p_EleEta  = TFile::Open(outPath + "/syZ0p_hd1112_WpWmRatio_vs_EleEta.root");
   TH1   *syZ0p_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor = (TH1*) file_syZ0p_EleEta -> Get("hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor");

   TFile *file_syZ0m_EleEta  = TFile::Open(outPath + "/syZ0m_hd1112_WpWmRatio_vs_EleEta.root");
   TH1   *syZ0m_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor = (TH1*) file_syZ0m_EleEta -> Get("hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor");

   // create graphs with asymmetric uncertainties to include the systematics
   TGraphAsymmErrors *gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor = new TGraphAsymmErrors(hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor);
   gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetTitle(";#eta_{el}; Systematics (%)");
   gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetFillColor(kBlue);
   gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetFillStyle(3001);
   gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetMaximum(3);
   gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetMinimum(-3);
   TGraphAsymmErrors *gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor = new TGraphAsymmErrors(hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor);
   gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetTitle(";#eta_{el}; Systematics (%)");
   gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetFillColor(kBlue);
   gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetFillStyle(3001);
   gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetMaximum(3);
   gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetMinimum(-3);
   //Int_t NPoints = g_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetN();
   TGraphAsymmErrors *g_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor = new TGraphAsymmErrors(hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor);
   g_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetFillColor(kBlue);
   g_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetFillStyle(3001);
   //cout << "NUMBER OF POINTS!!! = " << NPoints << endl;

   TGraphAsymmErrors *gd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   = new TGraphAsymmErrors(hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor);

   for (Int_t bin = 1; bin <= syQCDp_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor->GetNbinsX(); bin++) {
       Double_t syQCDp_EleEta = syQCDp_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetBinContent(bin);
       Double_t syQCDm_EleEta = syQCDm_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetBinContent(bin);
       Double_t syZ0p_EleEta  = syZ0p_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> GetBinContent(bin);
       Double_t syZ0m_EleEta  = syZ0m_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor  -> GetBinContent(bin);
       Double_t EleEta        = hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetBinContent(bin);
       Double_t statEleEta    = hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetBinError(bin);
       Double_t binCenter     = hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetXaxis() -> GetBinCenter(bin);
       syQCDp_EleEta -= EleEta;
       syQCDm_EleEta -= EleEta;
       //cout << "syQCDp_EleEta = " << syQCDp_EleEta << "  syQCDm_EleEta = " << syQCDm_EleEta << endl;
       Double_t syQCD_EleEta = fabs(syQCDp_EleEta); // for the moment we pick syQCDp which is the bigget in mag.
       cout << "W+/W- vs Electron Eta = " << EleEta <<" +- "<< statEleEta << " stat "<< " +- " << syQCD_EleEta << " QCDsys " << endl;

       Double_t syTotLow_EleEta  = sqrt( syQCD_EleEta**2 );
       Double_t syTotHigh_EleEta = sqrt( syQCD_EleEta**2 );

       syZ0p_EleEta -= EleEta;
       syZ0m_EleEta -= EleEta;
       cout << "syZ0p_EleEta = " << syZ0p_EleEta << "  syZ0m_EleEta = " << syZ0m_EleEta << endl;

       //if ( syZ0p_EleEta > 0 ) {
          syTotHigh_EleEta = sqrt( syTotHigh_EleEta**2 + syZ0p_EleEta**2 );
       //} else {
          syTotLow_EleEta  = sqrt( syTotLow_EleEta**2 + syZ0m_EleEta**2 );
       //}

       
       Double_t errTotHigh = sqrt( statEleEta**2 + syTotHigh_EleEta**2 );
       Double_t errTotLow  = sqrt( statEleEta**2 + syTotLow_EleEta**2 );
       //cout << "errTotHigh = " << errTotHigh << "  errTotLow = " << errTotLow << endl;
       Double_t relSyTotHigh_EleEta = (syTotHigh_EleEta / EleEta) *100;
       Double_t relSyTotLow_EleEta  = (syTotLow_EleEta / EleEta) *100;
       cout << "relSyTotHigh_EleEta = " << relSyTotHigh_EleEta << "  relSyTotLow_EleEta = " << relSyTotLow_EleEta << endl;

       Double_t binWidth= hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetBinWidth(bin);
       g_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetPointError(bin-1, binWidth/2, binWidth/2, syTotLow_EleEta, syTotHigh_EleEta);
       gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetPoint(bin-1, binCenter, 0);
       gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetPointError(bin-1, binWidth/2, binWidth/2, relSyTotLow_EleEta, relSyTotHigh_EleEta);

       gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetPoint(bin-1, binCenter, 0);
       gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetPointError(bin-1, binWidth/2, binWidth/2, syTotLow_EleEta, syTotHigh_EleEta);

       gd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SetPointError(bin-1, binWidth/2, binWidth/2, errTotLow, errTotHigh);
   }


   TFile *file_syQCDp_WBosonEta = TFile::Open(outPath + "/syQCDp_hd1112_WpWmRatio_vs_WBosonEta.root");
   TH1   *syQCDp_hd1112_WpWmRatio_vs_WBosonEta_BGsub = (TH1*) file_syQCDp_WBosonEta -> Get("hd1112_WpWmRatio_BGsub");

   TFile *file_syQCDm_WBosonEta = TFile::Open(outPath + "/syQCDm_hd1112_WpWmRatio_vs_WBosonEta.root");
   TH1   *syQCDm_hd1112_WpWmRatio_vs_WBosonEta_BGsub = (TH1*) file_syQCDm_WBosonEta -> Get("hd1112_WpWmRatio_BGsub");

   TFile *file_syZ0p_WBosonEta  = TFile::Open(outPath + "/syZ0p_hd1112_WpWmRatio_vs_WBosonEta.root");
   TH1   *syZ0p_hd1112_WpWmRatio_vs_WBosonEta_BGsub = (TH1*) file_syZ0p_WBosonEta -> Get("hd1112_WpWmRatio_BGsub");

   TFile *file_syZ0m_WBosonEta  = TFile::Open(outPath + "/syZ0m_hd1112_WpWmRatio_vs_WBosonEta.root");
   TH1   *syZ0m_hd1112_WpWmRatio_vs_WBosonEta_BGsub = (TH1*) file_syZ0m_WBosonEta -> Get("hd1112_WpWmRatio_BGsub");

   // create graphs with asymmetric uncertainties to include the systematics
   TGraphAsymmErrors *g_hd1112_WpWmRatio_vs_WBosonEta_BGsub     = new TGraphAsymmErrors(hd1112_WpWmRatio_BGsub);
   g_hd1112_WpWmRatio_vs_WBosonEta_BGsub -> SetFillColor(kBlue);
   //g_hd1112_WpWmRatio_vs_WBosonEta_BGsub -> SetFillStyle(3001);
   g_hd1112_WpWmRatio_vs_WBosonEta_BGsub -> SetFillStyle(3244);

   // for boson reconstruction systematic studies:
   TH1D *hd_WpWmRatio_MCreco_WBosonEta   = (TH1D*) hWp_Wp_WBosonEta        -> Clone("hd_WpWmRatio_MCreco_WBosonEta");
   TH1D *hd_WpWmRatio_MCgen_WBosonEta    = (TH1D*) hWp_Wp_Gen_WBosonEta    -> Clone("hd_WpWmRatio_MCgen_WBosonEta");
   TH1D *h_RelSyMCrec_WBosonEta          = (TH1D*) hWp_Wp_WBosonEta        -> Clone("h_RelSyMCrec_WBosonEta");
   h_RelSyMCrec_WBosonEta   -> SetMarkerStyle(20);
   h_RelSyMCrec_WBosonEta   -> SetMinimum(-0.5);
   h_RelSyMCrec_WBosonEta   -> SetMaximum(0.5);
   h_RelSyMCrec_WBosonEta   -> SetStats(0);
   h_RelSyMCrec_WBosonEta   -> SetTitleOffset(1.2, "Y");
   h_RelSyMCrec_WBosonEta   -> SetTitle("; #eta_{W}; Rel. syst. on boson recon."); 

   hWp_Wp_WBosonEta         -> Sumw2(); // for systematic studies
   hWm_Wm_WBosonEta         -> Sumw2(); // for systematic studies
   hWp_Wp_Gen_WBosonEta     -> Sumw2(); // for systematic studies
   hWm_Wm_Gen_WBosonEta     -> Sumw2(); // for systematic studies

   hd_WpWmRatio_MCreco_WBosonEta  -> Divide(hWp_Wp_WBosonEta, hWm_Wm_WBosonEta);
   hd_WpWmRatio_MCreco_WBosonEta  -> SetMaximum(6);
   hd_WpWmRatio_MCreco_WBosonEta  -> SetMinimum(0);
   hd_WpWmRatio_MCreco_WBosonEta  -> SetMarkerStyle(20);
   hd_WpWmRatio_MCreco_WBosonEta  -> SetMarkerColor(kRed);
   hd_WpWmRatio_MCreco_WBosonEta  -> SetTitle("run11 Monte Carlo (reconstructed); #eta_{W}; W^{+}/W^{-}");

   hd_WpWmRatio_MCgen_WBosonEta   -> Divide(hWp_Wp_Gen_WBosonEta, hWm_Wm_Gen_WBosonEta);
   hd_WpWmRatio_MCgen_WBosonEta   -> SetMaximum(6);
   hd_WpWmRatio_MCgen_WBosonEta   -> SetMinimum(0);
   hd_WpWmRatio_MCgen_WBosonEta   -> SetMarkerStyle(20);
   hd_WpWmRatio_MCgen_WBosonEta   -> SetMarkerColor(kBlue);
   hd_WpWmRatio_MCgen_WBosonEta   -> SetTitle("run11 Monte Carlo (generated); #eta_{W}; W^{+}/W^{-}");

   for (Int_t bin = 1; bin <= syQCDp_hd1112_WpWmRatio_vs_WBosonEta_BGsub->GetNbinsX(); bin++) {
       Double_t syQCDp_WBosonEta = syQCDp_hd1112_WpWmRatio_vs_WBosonEta_BGsub -> GetBinContent(bin);
       Double_t syQCDm_WBosonEta = syQCDm_hd1112_WpWmRatio_vs_WBosonEta_BGsub -> GetBinContent(bin);
       Double_t syZ0p_WBosonEta  = syZ0p_hd1112_WpWmRatio_vs_WBosonEta_BGsub  -> GetBinContent(bin);
       Double_t syZ0m_WBosonEta  = syZ0m_hd1112_WpWmRatio_vs_WBosonEta_BGsub  -> GetBinContent(bin);
       Double_t WBosonEta        = hd1112_WpWmRatio_BGsub -> GetBinContent(bin);
       Double_t statWBosonEta    = hd1112_WpWmRatio_BGsub -> GetBinError(bin);
       syQCDp_WBosonEta -= WBosonEta;
       syQCDm_WBosonEta -= WBosonEta;

       cout << "     " << endl;
       cout << "syQCDp_WBosonEta = " << syQCDp_WBosonEta << "  syQCDm_WBosonEta = " << syQCDm_WBosonEta << endl;

       Double_t syQCD_WBosonEta;
       if ( fabs(syQCDp_WBosonEta) > fabs(syQCDm_WBosonEta) ) { // we pick which one is the largest in mag.
           syQCD_WBosonEta = fabs(syQCDp_WBosonEta);
       }  else {
           syQCD_WBosonEta = fabs(syQCDm_WBosonEta); 
       }

       //cout << "W+/W- vs W-Eta = " << WBosonEta <<" +- "<< statWBosonEta << " stat "<< " +- " << syQCD_WBosonEta << " QCDsys " << endl;
       Double_t syTotLow_WBosonEta  = sqrt( syQCD_WBosonEta**2 );
       Double_t syTotHigh_WBosonEta = sqrt( syQCD_WBosonEta**2 );

       syZ0p_WBosonEta -= WBosonEta;
       syZ0m_WBosonEta -= WBosonEta;
       cout << "syZ0p_WBosonEta = " << syZ0p_WBosonEta << "  syZ0m_WBosonEta = " << syZ0m_WBosonEta << endl;    

       syTotHigh_WBosonEta = sqrt( syTotHigh_WBosonEta**2 + syZ0p_WBosonEta**2 );

       syTotLow_WBosonEta  = sqrt( syTotLow_WBosonEta**2 + syZ0m_WBosonEta**2 );

       Double_t MCrec_WBosonEta = hd_WpWmRatio_MCreco_WBosonEta -> GetBinContent(bin);
       Double_t MCgen_WBosonEta = hd_WpWmRatio_MCgen_WBosonEta  -> GetBinContent(bin);
       Double_t WEta = hd1112_WpWmRatio_BGsub -> GetBinContent(bin);
       Double_t syWRec_WBosonEta = MCgen_WBosonEta - MCrec_WBosonEta; // systematic on W boson Eta reconstruction 
       Double_t relSyWRec_WBosonEta = syWRec_WBosonEta / WEta;
       cout << "Syst. on reconstruction Boson-Eta = " <<  syWRec_WBosonEta << endl;
       h_RelSyMCrec_WBosonEta -> SetBinContent(bin, relSyWRec_WBosonEta);
       h_RelSyMCrec_WBosonEta -> SetBinError(bin, 0);
       if ( syWRec_WBosonEta > 0 ) {
	 syTotHigh_WBosonEta  = sqrt( syTotHigh_WBosonEta**2 + syWRec_WBosonEta**2);
       } else {
	 syTotLow_WBosonEta   = sqrt( syTotLow_WBosonEta**2 + syWRec_WBosonEta**2);
       }

       cout << "W+/W- vs W-Eta = " << WBosonEta <<" +- "<< statWBosonEta << " stat "<< " + " <<  syTotHigh_WBosonEta << " sys " << " - " << syTotLow_WBosonEta << " sys " << endl;
       cout << "     " << endl;

       Double_t binWidth   = hd1112_WpWmRatio_BGsub -> GetBinWidth(bin);
       g_hd1112_WpWmRatio_vs_WBosonEta_BGsub -> SetPointError(bin-1, binWidth/2, binWidth/2, syTotLow_WBosonEta, syTotHigh_WBosonEta);

   }



   TFile *file_syQCDp_WBosonRapidity = TFile::Open(outPath + "/syQCDp_hd1112_WpWmRatio_vs_WBosonRapidity.root");
   TH1   *syQCDp_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub = (TH1*) file_syQCDp_WBosonRapidity -> Get("hd1112_WpWmRatio_vs_WRapidity_BGsub");

   TFile *file_syQCDm_WBosonRapidity = TFile::Open(outPath + "/syQCDm_hd1112_WpWmRatio_vs_WBosonRapidity.root");
   TH1   *syQCDm_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub = (TH1*) file_syQCDm_WBosonRapidity -> Get("hd1112_WpWmRatio_vs_WRapidity_BGsub");

   TFile *file_syZ0p_WBosonRapidity  = TFile::Open(outPath + "/syZ0p_hd1112_WpWmRatio_vs_WBosonRapidity.root");
   TH1   *syZ0p_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub = (TH1*) file_syZ0p_WBosonRapidity -> Get("hd1112_WpWmRatio_vs_WRapidity_BGsub");

   TFile *file_syZ0m_WBosonRapidity  = TFile::Open(outPath + "/syZ0m_hd1112_WpWmRatio_vs_WBosonRapidity.root");
   TH1   *syZ0m_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub = (TH1*) file_syZ0m_WBosonRapidity -> Get("hd1112_WpWmRatio_vs_WRapidity_BGsub");

   // create graphs with asymmetric uncertainties to include the systematics
   TGraphAsymmErrors *gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub = new TGraphAsymmErrors(hd1112_WpWmRatio_vs_WRapidity_BGsub);
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetTitle(";y_{W}; Systematics (%)");
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetFillColor(kBlue);
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetFillStyle(3001);
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetMaximum(5);
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetMinimum(-50);
   TGraphAsymmErrors *g_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub   = new TGraphAsymmErrors(hd1112_WpWmRatio_vs_WRapidity_BGsub);
   g_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetFillColor(kBlue);
   //g_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetFillStyle(3001);
   g_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetFillStyle(3244);

   TGraphAsymmErrors *gd1112_WpWmRatio_vs_WBosonRapidity_BGsub     = new TGraphAsymmErrors(hd1112_WpWmRatio_vs_WRapidity_BGsub);

   // for boson reconstruction systematic studies:
   TH1D *hd_WpWmRatio_MCreco_WBosonRapidity   = (TH1D*) hWp_Wp_WBosonRapidity        -> Clone("hd_WpWmRatio_MCreco_WBosonRapidity");
   TH1D *hd_WpWmRatio_MCgen_WBosonRapidity    = (TH1D*) hWp_Wp_Gen_WBosonRapidity    -> Clone("hd_WpWmRatio_MCgen_WBosonRapidity");
   TH1D *h_RelSyMCrec_WBosonRapidity          = (TH1D*) hWp_Wp_WBosonRapidity        -> Clone("h_RelSyMCrec_WBosonRapidity");
   h_RelSyMCrec_WBosonRapidity   -> SetMarkerStyle(20);
   h_RelSyMCrec_WBosonRapidity   -> SetMinimum(-0.5);
   h_RelSyMCrec_WBosonRapidity   -> SetMaximum(0.5);
   h_RelSyMCrec_WBosonRapidity   -> SetStats(0);
   h_RelSyMCrec_WBosonRapidity   -> SetTitleOffset(1.2, "Y");
   h_RelSyMCrec_WBosonRapidity   -> SetTitle("; y_{W}; Rel. syst. on boson recon."); 

   hWp_Wp_WBosonRapidity         -> Sumw2(); // for systematic studies
   hWm_Wm_WBosonRapidity         -> Sumw2(); // for systematic studies
   hWp_Wp_Gen_WBosonRapidity     -> Sumw2(); // for systematic studies
   hWm_Wm_Gen_WBosonRapidity     -> Sumw2(); // for systematic studies

   hd_WpWmRatio_MCreco_WBosonRapidity  -> Divide(hWp_Wp_WBosonRapidity, hWm_Wm_WBosonRapidity);
   hd_WpWmRatio_MCreco_WBosonRapidity  -> SetMaximum(6);
   hd_WpWmRatio_MCreco_WBosonRapidity  -> SetMinimum(0);
   hd_WpWmRatio_MCreco_WBosonRapidity  -> SetMarkerStyle(20);
   hd_WpWmRatio_MCreco_WBosonRapidity  -> SetMarkerColor(kRed);
   hd_WpWmRatio_MCreco_WBosonRapidity  -> SetTitle("run11 Monte Carlo (reconstructed); y_{W}; W^{+}/W^{-}");

   hd_WpWmRatio_MCgen_WBosonRapidity   -> Divide(hWp_Wp_Gen_WBosonRapidity, hWm_Wm_Gen_WBosonRapidity);
   hd_WpWmRatio_MCgen_WBosonRapidity   -> SetMaximum(6);
   hd_WpWmRatio_MCgen_WBosonRapidity   -> SetMinimum(0);
   hd_WpWmRatio_MCgen_WBosonRapidity   -> SetMarkerStyle(20);
   hd_WpWmRatio_MCgen_WBosonRapidity   -> SetMarkerColor(kBlue);
   hd_WpWmRatio_MCgen_WBosonRapidity   -> SetTitle("run11 Monte Carlo (generated); y_{W}; W^{+}/W^{-}");

   for (Int_t bin = 1; bin <= syQCDp_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub->GetNbinsX(); bin++) {
       Double_t syQCDp_WBosonRapidity = syQCDp_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> GetBinContent(bin);
       Double_t syQCDm_WBosonRapidity = syQCDm_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> GetBinContent(bin);
       Double_t syZ0p_WBosonRapidity  = syZ0p_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub  -> GetBinContent(bin);
       Double_t syZ0m_WBosonRapidity  = syZ0m_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub  -> GetBinContent(bin);
       Double_t WBosonRapidity        = hd1112_WpWmRatio_vs_WRapidity_BGsub -> GetBinContent(bin);
       Double_t statWBosonRapidity    = hd1112_WpWmRatio_vs_WRapidity_BGsub -> GetBinError(bin);
       Double_t binCenter             = hd1112_WpWmRatio_vs_WRapidity_BGsub -> GetXaxis() -> GetBinCenter(bin);
       syQCDp_WBosonRapidity -= WBosonRapidity;
       syQCDm_WBosonRapidity -= WBosonRapidity;

       cout << "     " << endl;
       cout << "syQCDp_WBosonRapidity = " << syQCDp_WBosonRapidity << "  syQCDm_WBosonRapidity = " << syQCDm_WBosonRapidity << endl;

       Double_t syQCD_WBosonRapidity;
       if ( fabs(syQCDp_WBosonRapidity) > fabs(syQCDm_WBosonRapidity) ) { // we pick which one is the largest in mag.
           syQCD_WBosonRapidity = fabs(syQCDp_WBosonRapidity);
       }  else {
           syQCD_WBosonRapidity = fabs(syQCDm_WBosonRapidity); 
       }

       //cout << "W+/W- vs W-Eta = " << WBosonRapidity <<" +- "<< statWBosonRapidity << " stat "<< " +- " << syQCD_WBosonRapidity << " QCDsys " << endl;
       Double_t syTotLow_WBosonRapidity  = sqrt( syQCD_WBosonRapidity**2 );
       Double_t syTotHigh_WBosonRapidity = sqrt( syQCD_WBosonRapidity**2 );

       syZ0p_WBosonRapidity -= WBosonRapidity;
       syZ0m_WBosonRapidity -= WBosonRapidity;
       cout << "syZ0p_WBosonRapidity = " << syZ0p_WBosonRapidity << "  syZ0m_WBosonRapidity = " << syZ0m_WBosonRapidity << endl;

       //if ( syZ0p_WBosonRapidity > 0 ) {
          syTotHigh_WBosonRapidity = sqrt( syTotHigh_WBosonRapidity**2 + syZ0p_WBosonRapidity**2 );
       //} else {
          syTotLow_WBosonRapidity  = sqrt( syTotLow_WBosonRapidity**2 + syZ0m_WBosonRapidity**2 );
       //}

       Double_t MCrec_WBosonRapidity = hd_WpWmRatio_MCreco_WBosonRapidity -> GetBinContent(bin);
       Double_t MCgen_WBosonRapidity = hd_WpWmRatio_MCgen_WBosonRapidity  -> GetBinContent(bin);
       Double_t syWRec_WBosonRapidity = MCgen_WBosonRapidity - MCrec_WBosonRapidity; // systematic on W boson Eta reconstruction 
       Double_t relSyWRec_WBosonRapidity = syWRec_WBosonRapidity / WBosonRapidity;
       cout << "Syst. on reconstruction Boson-Rapidity = " <<  syWRec_WBosonRapidity << endl;
       h_RelSyMCrec_WBosonRapidity -> SetBinContent(bin, relSyWRec_WBosonRapidity);
       h_RelSyMCrec_WBosonRapidity -> SetBinError(bin, 0);
       if ( syWRec_WBosonRapidity > 0 ) {
	 syTotHigh_WBosonRapidity  = sqrt( syTotHigh_WBosonRapidity**2 + syWRec_WBosonRapidity**2);
       } else {
	 syTotLow_WBosonRapidity   = sqrt( syTotLow_WBosonRapidity**2 + syWRec_WBosonRapidity**2);
       }

       cout << "W+/W- vs W-Rap = " << WBosonRapidity <<" +- "<< statWBosonRapidity << " stat "<< " + " <<  syTotHigh_WBosonRapidity << " sys " << " - " << syTotLow_WBosonRapidity << " sys " << endl;
       cout << "     " << endl;

       Double_t errTotHigh = sqrt( statWBosonRapidity**2 + syTotHigh_WBosonRapidity**2 );
       Double_t errTotLow  = sqrt( statWBosonRapidity**2 + syTotLow_WBosonRapidity**2 );
       //cout << "errTotHigh = " << errTotHigh << "  errTotLow = " << errTotLow << endl;
       Double_t relSyTotHigh_WBosonRapidity = (syTotHigh_WBosonRapidity / WBosonRapidity) *100;
       Double_t relSyTotLow_WBosonRapidity  = (syTotLow_WBosonRapidity / WBosonRapidity) *100;
       cout << "relSyTotHigh_WBosonRapidity = " << relSyTotHigh_WBosonRapidity << "  relSyTotLow_WBosonRapidity = " << relSyTotLow_WBosonRapidity << endl;

       Double_t binWidth   = hd1112_WpWmRatio_vs_WRapidity_BGsub -> GetBinWidth(bin);
       g_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetPointError(bin-1, binWidth/2, binWidth/2, syTotLow_WBosonRapidity, syTotHigh_WBosonRapidity);
       gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetPoint(bin-1, binCenter, 0);
       gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> SetPointError(bin-1, binWidth/2, binWidth/2, relSyTotLow_WBosonRapidity, relSyTotHigh_WBosonRapidity);
       gd1112_WpWmRatio_vs_WBosonRapidity_BGsub   -> SetPointError(bin-1, binWidth/2, binWidth/2, errTotLow, errTotHigh);
   }


   //------------------------------------------------------------------------------------
   // plots
   //------------------------------------------------------------------------------------
   /*
   //plots for the W reconstruction systematics
   TCanvas *cWSysEta    = new TCanvas("cWSysEta", "run 11 Monte Carlo: W reconstruction systematics vs W-Eta", 800, 500);

   TLegend *leg_cWSysEta = new TLegend(0.75, 0.75, 0.99, 0.99);
   leg_cWSysEta -> AddEntry(hd_WpWmRatio_MCgen_WBosonEta,"MC Gen","P");
   leg_cWSysEta -> AddEntry(hd_WpWmRatio_MCreco_WBosonEta,"MC Rec","P");

   cWSysEta -> Divide(2,1);

   cWSysEta_1 -> cd();
   hd_WpWmRatio_MCgen_WBosonEta  -> SetStats(0);
   hd_WpWmRatio_MCgen_WBosonEta  -> Draw("e1");
   hd_WpWmRatio_MCreco_WBosonEta -> Draw("e1 same");
   leg_cWSysEta                  -> Draw();

   cWSysEta_2 -> cd();
   h_RelSyMCrec_WBosonEta -> Draw("P");

   cWSysEta -> Print(outPath + "/plot_WpWmRatio_vs_Eta_MC.eps");
   cWSysEta -> Print(outPath + "/plot_WpWmRatio_vs_Eta_MC.png");


   TCanvas *cWSysRap    = new TCanvas("cWSysRap", "run 11 Monte Carlo: W reconstruction systematics vs W-Rapidity", 800, 500);

   TLegend *leg_cWSysRap = new TLegend(0.75, 0.75, 0.99, 0.99);
   leg_cWSysRap -> AddEntry(hd_WpWmRatio_MCgen_WBosonRapidity,"MC Gen","P");
   leg_cWSysRap -> AddEntry(hd_WpWmRatio_MCreco_WBosonRapidity,"MC Rec","P");

   cWSysRap -> Divide(2,1);

   cWSysRap_1 -> cd();
   hd_WpWmRatio_MCgen_WBosonRapidity  -> SetStats(0);
   hd_WpWmRatio_MCgen_WBosonRapidity  -> Draw("e1");
   hd_WpWmRatio_MCreco_WBosonRapidity -> Draw("e1 same");
   leg_cWSysRap                       -> Draw();

   cWSysRap_2 -> cd();
   h_RelSyMCrec_WBosonRapidity -> Draw("P");

   cWSysRap -> Print(outPath + "/plot_WpWmRatio_vs_Rapidity_MC.eps");
   cWSysRap -> Print(outPath + "/plot_WpWmRatio_vs_Rapidity_MC.png");
   //---------------------------------------------------


   TCanvas *c1    = new TCanvas("c1", "run 11: W+/W- versus Eta", 800, 500);
   //c1 -> Divide(2,1);


   TLegend *leg_c1 = new TLegend(0.5, 0.85, 0.75, 0.99);
   leg_c1 -> SetFillColor(kWhite);
   leg_c1 -> AddEntry(hd_WpWmRatio,"STAR data", "P");
   leg_c1 -> AddEntry(hd_WpWmRatio_BGsub,"data - bg", "P");

   c1 -> cd();
   hd_WpWmRatio -> Draw("e1");

   //c1_2 -> cd();
   hd_WpWmRatio_BGsub -> Draw("e1 same");
   leg_c1 -> Draw();

   c1->Print(outPath + "/plot_WpWmRatio_vs_Eta.eps");
   c1->Print(outPath + "/plot_WpWmRatio_vs_Eta.png");


   TCanvas *c1Rap    = new TCanvas("c1Rap", "run 11: W+/W- versus Rapidity", 800, 500);
   //c1Rap -> Divide(2,1);

   c1Rap -> cd();
   hd_WpWmRatio_vs_WRapidity       -> Draw("e1");

   //c1Rap_2 -> cd();
   hd_WpWmRatio_vs_WRapidity_BGsub -> Draw("e1 same");
   leg_c1 -> Draw();

   c1Rap->Print(outPath + "/plot_WpWmRatio_vs_Rap.eps");
   c1Rap->Print(outPath + "/plot_WpWmRatio_vs_Rap.png");


   TCanvas *c1el    = new TCanvas("c1el", "run 11: W+/W- versus Elec-Eta", 800, 500);
   //c1el -> Divide(2,1);
 
   c1el -> cd();
   hd_WpWmRatio_vs_EleEta -> Draw("e1");

   //c1el_2 -> cd();
   //hd_WpWmRatio_vs_EleEta_BGsub        -> GetXaxis() -> SetRange(3, 8);
   hd_WpWmRatio_vs_EleEta_BGsub        -> Draw("e1 same");
   hd_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetMarkerColor(kRed);
   hd_WpWmRatio_vs_EleEta_BGsub_EffCor -> Draw("e1 same");


   TLegend *leg_c1el = new TLegend(0.55, 0.65, 0.75, 0.99);
   leg_c1el -> AddEntry(hd_WpWmRatio_vs_EleEta,"No correction", "P");
   leg_c1el -> AddEntry(hd_WpWmRatio_vs_EleEta_BGsub,"Bg corrected", "P");
   leg_c1el -> AddEntry(hd_WpWmRatio_vs_EleEta_BGsub_EffCor,"Bg+Eff corrected", "P");
   leg_c1el -> Draw();

   c1el->Print(outPath + "/plot_WpWmRatio_vs_ElecEta.eps");
   c1el->Print(outPath + "/plot_WpWmRatio_vs_ElecEta.png");


   TCanvas *c3    = new TCanvas("c3", "run 12: W+/W- versus Eta", 800, 500);
   //c3 -> Divide(2,1);

   c3 -> cd();
   hd12_WpWmRatio       -> Draw("e");

   //c3_2 -> cd();
   hd12_WpWmRatio_BGsub -> Draw("e same");
   leg_c1 -> Draw();

   c3->Print(outPath + "/plot_r12_WpWmRatio_vs_Eta.eps");
   c3->Print(outPath + "/plot_r12_WpWmRatio_vs_Eta.png");


   TCanvas *c3Rap    = new TCanvas("c3Rap", "run 12: W+/W- versus Rapidity", 800, 500);

   c3Rap -> cd();
   hd12_WpWmRatio_vs_WRapidity       -> Draw("e1");

   //c3Rap_2 -> cd();
   hd12_WpWmRatio_vs_WRapidity_BGsub -> Draw("e1 same");
   leg_c1 -> Draw();

   c3Rap->Print(outPath + "/plot_r12_WpWmRatio_vs_Rap.eps");
   c3Rap->Print(outPath + "/plot_r12_WpWmRatio_vs_Rap.png");


   TCanvas *c3el    = new TCanvas("c3el", "run 12: W+/W- versus Elec-Eta", 800, 500);
   //c3el -> Divide(2,1);

   c3el -> cd();
   hd12_WpWmRatio_vs_EleEta -> Draw("e");

   //c3el_2 -> cd();
   //hd12_WpWmRatio_vs_EleEta_BGsub -> GetXaxis() -> SetRange(3, 8);
   hd12_WpWmRatio_vs_EleEta_BGsub        -> Draw("e same");
   hd12_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetMarkerColor(kRed);
   hd12_WpWmRatio_vs_EleEta_BGsub_EffCor -> Draw("e same");

   leg_c1el -> Draw();

   c3el->Print(outPath + "/plot_r12_WpWmRatio_vs_ElecEta.eps");
   c3el->Print(outPath + "/plot_r12_WpWmRatio_vs_ElecEta.png");


   TCanvas *c5    = new TCanvas("c5", "run 11+12: W+/W- versus Eta", 800, 500);
   //c5 -> Divide(2,1);

   c5 -> cd();
   hd1112_WpWmRatio -> Draw("e");

   //c5_2 -> cd();
   hd1112_WpWmRatio_BGsub -> Draw("e");
   leg_c1 -> Draw();

   c5->Print(outPath + "/plot_r11r12_WpWmRatio_vs_Eta.eps");
   c5->Print(outPath + "/plot_r11r12_WpWmRatio_vs_Eta.png");


   TCanvas *c5rap    = new TCanvas("c5rap", "run 11+12: W+/W- versus Rapidity", 800, 500);
   c5rap -> cd();
   hd1112_WpWmRatio_vs_WRapidity       -> Draw("e1");
   hd1112_WpWmRatio_vs_WRapidity_BGsub -> Draw("e1 same");
   leg_c1 -> Draw();

   c5rap->Print(outPath + "/plot_r11r12_WpWmRatio_vs_Rapidity.eps");
   c5rap->Print(outPath + "/plot_r11r12_WpWmRatio_vs_Rapidity.png");


   TCanvas *c5el    = new TCanvas("c5el", "run 11+12: W+/W- versus Elec-Eta", 800, 500);
   // c5el -> Divide(2,1);

   c5el -> cd();
   hd1112_WpWmRatio_vs_EleEta -> Draw("e1");

   //c5el_2 -> cd();

   //hd1112_WpWmRatio_vs_EleEta_BGsub -> GetXaxis() -> SetRange(3, 8);
   hd1112_WpWmRatio_vs_EleEta_BGsub -> Draw("e1 same");
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> SetMarkerColor(kRed);
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> Draw("e1 same");

   leg_c1el -> Draw();

   c5el->Print(outPath + "/plot_r11r12_WpWmRatio_vs_ElecEta.eps");
   c5el->Print(outPath + "/plot_r11r12_WpWmRatio_vs_ElecEta.png");

   */
   // Plot the final version
   TCanvas *c5elFinal   = new TCanvas("c5elFinal", "run 11+12: W+/W- versus Elec-Eta", 800, 800);

  TPad*    upperPad_c5elFinal  = new TPad("upperPad_c5elFinal ", "upperPad_c5elFinal ", .005, .3, .995, .995);
  TPad*    lowerPad_c5elFinal  = new TPad("lowerPad_c5elFinal ", "lowerPad_c5elFinal ", .005, .005, .995, .3);
  //upperPad_c5elFinal  -> SetBottomMargin(0.15);
  //lowerPad_c5elFinal  -> SetTopMargin(0.1);
  ////upperPad_c5elFinal  -> SetBottomMargin(0.0);
  ////lowerPad_c5elFinal  -> SetTopMargin(0.0);
  //lowerPad_c5elFinal  -> SetBottomMargin(0.35);
  //upperPad_c5elFinal  -> Draw();
  //lowerPad_c5elFinal  -> Draw();

   TLatex *textProcess = new TLatex(0.15, 0.85, "p+p #rightarrow W^{#pm}+X #rightarrow e^{#pm}+X");
   textProcess -> SetNDC(); 
   //textProcess -> SetTextFont(32);
   textProcess -> SetTextSize(0.05);

   TLatex *textStarLumi = new TLatex(0.15, 0.8, "#intL = 102 pb^{-1}");
   textStarLumi -> SetNDC(); 
   //textStarLumi -> SetTextColor(kRed); 
   textStarLumi -> SetTextFont(32);
   textStarLumi -> SetTextSize(0.05);

   TLatex *textStarPrel = new TLatex(0.15, 0.78, "STAR Preliminary");
   textStarPrel -> SetNDC(); 
   //textStarPrel -> SetTextColor(kRed); 
   textStarPrel -> SetTextFont(32);
   textStarPrel -> SetTextSize(0.05);

   //TLatex *textStarRootS = new TLatex(0.2, 0.7, "#sqrt{s}=500/510 GeV, #intL = 102 pb^{-1}");
   TLatex *textStarRootS = new TLatex(0.15, 0.7, "#sqrt{s}=500/510 GeV");
   textStarRootS -> SetNDC(); 
   //textStarRootS -> SetTextColor(kRed); 
   textStarRootS -> SetTextFont(32);
   textStarRootS -> SetTextSize(0.05);

   double etaEl[10] = {
     //-1.9,
     //-1.7,
     //-1.5,
     //-1.3,
     //-1.1,
   -0.9,   -0.7,   -0.5,   -0.3,   -0.1,
   0.1,   0.3,   0.5,   0.7,   0.9 };
   //1.1,
   //1.3,
   //1.5,
   //1.7,
   //1.9 }

   double etaEleh[10] = {0.0};

   double etaElel[10] = {0.0};

   double rEl[10] = {
     //0.99,
     //1.19,
     //1.46,
     //1.84,
     //2.38,
   3.06,   3.86,   4.71,   5.44,   5.91,
   5.91,   5.44,   4.71,   3.86,   3.06 };
   //2.38,
   //1.84,
   //1.46,
   //1.19,
   //0.99 };

   double rEleh[10] = {
     //0.20,
     //0.19,
     //0.22,
     //0.26,
     //0.31,
   0.32,   0.46,   0.58,   0.76,   0.90,
   0.90,   0.76,   0.58,   0.46,   0.32 };
   //0.31,
   //0.26,
   //0.22,
   //0.19,
   //0.20 };

   double rElel[10] = {
     //0.19, 0.24,
     //0.24, 0.27, 0.32, 
     0.45, 0.50,
     0.69, 0.86, 0.96, 0.96, 0.86,
     0.69, 0.50, 0.45 }; 
     //0.32, 0.27,
     //0.24, 0.24, 0.19 };

   //0.10    5.91    0.90    0.96
   //0.30    5.44    0.76    0.86
   //0.50    4.71    0.58    0.69
   //0.70    3.86    0.46    0.50
   //0.90    3.06    0.32    0.45
   //1.10    2.38    0.31    0.32
   //1.30    1.84    0.26    0.27
   //1.50    1.46    0.22    0.24
   //1.70    1.19    0.19    0.24
   //1.90    0.99    0.20    0.19
   //2.10    0.87    0.21    0.17
   //2.30    0.77    0.19    0.18
   //2.50    0.76    0.14    0.27


   TGraphAsymmErrors *g_ratio_EleEta_theory = new TGraphAsymmErrors(10, etaEl, rEl, etaElel, etaEleh, rElel, rEleh);
   g_ratio_EleEta_theory -> SetFillColor(kYellow);

   TLegend *leg_c5elFinal = new TLegend(0.45, 0.85, 0.75, 0.99);
   leg_c5elFinal -> SetFillColor(kWhite);
   leg_c5elFinal -> AddEntry(hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor,"STAR prel. 102 pb^{-1}", "P");
   leg_c5elFinal -> AddEntry(g_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor,"Systematic uncertainty", "F");
   leg_c5elFinal -> AddEntry(g_ratio_EleEta_theory,"CT10 NLO", "F");

   c5elFinal -> cd();
   //upperPad_c5elFinal -> cd();
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SetTitle("");
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SetTitleSize(0.07, "XY");
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SetMarkerColor(kRed);
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SetMaximum(7.5);
   //hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SetMinimum(-0.5);
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SetMinimum(0);
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> Draw("p"); 
   g_ratio_EleEta_theory                     -> Draw("3");
   g_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> Draw("2"); //syst errors only
   gd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> Draw("0"); //syst added in quadrature
   //hDIS2015_Matt_EleEta                      -> Draw("e1 same"); // Matt Results (for comparison)
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> Draw("p same");
   leg_c5elFinal -> Draw();

   hd1112_Wp_ElectronEta_BGsub_EffCor        -> SaveAs(outPath + "/4Dima_Wplus_ElectronEta.root");
   hd1112_Wm_ElectronEta_BGsub_EffCor        -> SaveAs(outPath + "/4Dima_Wminus_ElectronEta.root");
   hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SaveAs(outPath + "/DIS2015_WpWmRatio_vs_EleEta.root");
   gd1112_WpWmRatio_vs_EleEta_BGsub_EffCor   -> SaveAs(outPath + "/DIS2015_WpWmRatio_vs_EleEta_SysAdded.root");
   //lowerPad_c5elFinal -> cd();
   //gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> Draw("2"); // absolute syst errors
   //gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetYaxis() -> SetLabelSize(0.06);
   //gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetYaxis() -> SetTitleSize(0.07);
   //gAbsSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetXaxis() -> SetRangeUser(-1, 1);
   ////gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> Draw("2"); // relative syst errors
   ////gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetYaxis() -> SetLabelSize(0.06);
   ////gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetYaxis() -> SetTitleSize(0.07);
   ////gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetXaxis() -> SetRangeUser(-1, 1);
   //////gRelSys_hd1112_WpWmRatio_vs_EleEta_BGsub_EffCor -> GetXaxis() -> SetNdivisions();

   c5elFinal->Print(outPath + "/plot_r11r12_WpWmRatio_vs_ElecEta_final.eps");
   c5elFinal->Print(outPath + "/plot_r11r12_WpWmRatio_vs_ElecEta_final.png");


   //TCanvas *cPIPPO    = new TCanvas("cPIPPO", "PIPPO", 800, 500);
   //cPIPPO -> cd();
   //gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> Draw("a2"); // relative syst errors


   TCanvas *c5EtaBosFinal    = new TCanvas("c5EtaBosFinal", "run 11+12: W+/W- versus WBoson-Eta", 800, 500);


   TLegend *leg_c5EtaBosFinal = new TLegend(0.45, 0.85, 0.75, 0.99);
   leg_c5EtaBosFinal -> SetFillColor(kWhite);
   leg_c5EtaBosFinal -> AddEntry(hd1112_WpWmRatio_BGsub,"STAR prel. 102 pb^{-1}", "P");
   leg_c5EtaBosFinal -> AddEntry(g_hd1112_WpWmRatio_vs_WBosonEta_BGsub,"Systematic uncertainty", "F");

   c5EtaBosFinal -> cd();
   hd1112_WpWmRatio_BGsub                -> SetTitle("");
   hd1112_WpWmRatio_BGsub                -> SetTitleSize(0.07, "XY");
   hd1112_WpWmRatio_BGsub                -> SetMarkerColor(kRed);
   hd1112_WpWmRatio_BGsub                -> Draw("e1");
   g_hd1112_WpWmRatio_vs_WBosonEta_BGsub -> Draw("2");
   hd1112_WpWmRatio_BGsub                -> Draw("e1 same");
   leg_c5EtaBosFinal -> Draw();

   c5EtaBosFinal->Print(outPath + "/plot_r11r12_WpWmRatio_vs_WBosonEta_final.eps");
   c5EtaBosFinal->Print(outPath + "/plot_r11r12_WpWmRatio_vs_WBosonEta_final.png");


   TCanvas *c5RapBosFinal    = new TCanvas("c5RapBosFinal", "run 11+12: W+/W- versus WBoson-Rapidity", 800, 600);
   /*
  TPad*    upperPad_c5RapBosFinal  = new TPad("upperPad_c5RapBosFinal ", "upperPad_c5RapBosFinal ", .005, .3, .995, .995);
  TPad*    lowerPad_c5RapBosFinal  = new TPad("lowerPad_c5RapBosFinal ", "lowerPad_c5RapBosFinal ", .005, .005, .995, .3);
  upperPad_c5RapBosFinal  -> SetBottomMargin(0.15);
  lowerPad_c5RapBosFinal  -> SetTopMargin(0.1);
  lowerPad_c5RapBosFinal  -> SetBottomMargin(0.35);
  upperPad_c5RapBosFinal  -> Draw();
  lowerPad_c5RapBosFinal  -> Draw();
   */
   TVectorD yW(5); yW[0]= -0.48; yW[1]= -0.24; yW[2]= 0.00; yW[3]= 0.24; yW[4]= 0.48;
   TVectorD yWeh(5); yWeh[0]= 0; yWeh[1]= 0; yWeh[2]= 0; yWeh[3]= 0; yWeh[4]= 0;
   TVectorD yWel(5); yWel[0]= 0; yWel[1]= 0; yWel[2]= 0; yWel[3]= 0; yWel[4]= 0;
   TVectorD r(5); r[0]= 2.76; r[1]= 2.65; r[2]= 2.63; r[3]= 2.65; r[4]= 2.76;
   TVectorD reh(5); reh[0]= 0.36; reh[1]= 0.35; reh[2]= 0.36; reh[3]= 0.35; reh[4]= 0.34;
   TVectorD rel(5); rel[0]= 0.41; rel[1]= 0.48; rel[2]= 0.51; rel[3]= 0.48; rel[4]= 0.41;

   TGraphAsymmErrors *g_ratio_WRap_theory = new TGraphAsymmErrors(yW, r, yWel, yWeh, rel, reh);
   g_ratio_WRap_theory -> SetFillColor(kYellow);

   TFile *fileRWunpBBSWRap       = TFile::Open(outPath + "/RW-unp-BBS-WRap.root");
   TH1 *hRW_unp_BBS_WRap         = (TH1*) fileRWunpBBSWRap   ->Get("hRWRebin");
   hRW_unp_BBS_WRap   -> SetLineColor(kRed); 
   hRW_unp_BBS_WRap   -> SetLineStyle(2); 
   hRW_unp_BBS_WRap   -> SetLineWidth(3);

   TFile *fileRWunpCT10WRap       = TFile::Open(outPath + "/RW-unp-CT10-WRap-500.root");
   TH1 *hRW_unp_CT10_WRap         = (TH1*) fileRWunpCT10WRap   ->Get("hRWRebin");
   hRW_unp_CT10_WRap   -> SetLineColor(kCyan); 
   hRW_unp_CT10_WRap   -> SetLineStyle(1); 
   hRW_unp_CT10_WRap   -> SetLineWidth(3);

   gStyle->SetErrorX(0.5);

   c5RapBosFinal -> cd();
   //upperPad_c5RapBosFinal -> cd();
   c5RapBosFinal -> SetGrid(0,0);
   c5RapBosFinal -> SetRightMargin(0.08);
   c5RapBosFinal -> SetLeftMargin(0.12);
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SetTitle("");
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SetYTitle("RW");
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SetTitleSize(0.07, "XY");
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SetMarkerSize(2);
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SetMarkerColor(kBlack);
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SetStats(0);
   //gStyle->SetStatX(0.1);
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SetLineWidth(2);  
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SetMaximum(8);
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> Draw("e1"); 
   g_ratio_WRap_theory                                -> Draw("3"); // theory MCFM-CT10
   g_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub         -> Draw("2"); //syst errors only
   hRW_unp_BBS_WRap                                   -> Draw("L same"); // theory RHICBOS-BBS
   hRW_unp_CT10_WRap                                  -> Draw("L same"); // theory RHICBOS-CT10
   gd1112_WpWmRatio_vs_WBosonRapidity_BGsub           -> SetLineWidth(2);
   //gd1112_WpWmRatio_vs_WBosonRapidity_BGsub           -> Draw("0"); //syst added in quadrature
   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> Draw("e1 same");
   textProcess       -> Draw();
   //textStarLumi      -> Draw();
   textStarPrel      -> Draw();
   textStarRootS     -> Draw();

   //TLegend *leg_c5RapBosFinal = new TLegend(0.45, 0.85, 0.75, 0.99);
   TLegend *leg_c5RapBosFinal = new TLegend(0.6, 0.6, 0.9, 0.88);
   leg_c5RapBosFinal -> SetFillColor(kWhite);
   //leg_c5RapBosFinal -> SetLineColor(kWhite);
   leg_c5RapBosFinal -> SetBorderSize(0);
   leg_c5RapBosFinal -> AddEntry(hd1112_WpWmRatio_vs_WRapidity_BGsub,"STAR #int L= 102pb^{-1}", "LEP");
   leg_c5RapBosFinal -> AddEntry(g_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub,"Systematic uncertainty", "F");
   leg_c5RapBosFinal -> AddEntry(g_ratio_WRap_theory,"MCFM-CT10", "F");
   leg_c5RapBosFinal -> AddEntry( hRW_unp_BBS_WRap,"RHICBOS-BBS", "L");
   leg_c5RapBosFinal -> AddEntry( hRW_unp_CT10_WRap,"RHICBOS-CT10", "L");
   leg_c5RapBosFinal -> Draw();

   hd1112_WpWmRatio_vs_WRapidity_BGsub                -> SaveAs(outPath + "/DIS2015_WpWmRatio_vs_WRapidity.root");
   gd1112_WpWmRatio_vs_WBosonRapidity_BGsub           -> SaveAs(outPath + "/DIS2015_WpWmRatio_vs_WRapidity_SysAdded.root");

   /*
   lowerPad_c5RapBosFinal -> cd();
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> Draw("a2"); // relative syst errors
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> GetYaxis() -> SetLabelSize(0.06);
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> GetYaxis() -> SetTitleSize(0.07);
   gRelSys_hd1112_WpWmRatio_vs_WBosonRapidity_BGsub -> GetXaxis() -> SetRangeUser(-0.6, 0.6);
   */

   c5RapBosFinal->Print(outPath + "/plot_r11r12_WpWmRatio_vs_WBosonRapidity_final.eps");
   c5RapBosFinal->Print(outPath + "/plot_r11r12_WpWmRatio_vs_WBosonRapidity_final.png");

   /*
   // Plot the run11/run12 tests
   TCanvas *c11o12    = new TCanvas("c11o12", "[run 11 W+]/[run 12 W+]", 800, 800);
   c11o12 -> Divide(2,3);

   TF1 *fitPol0  = new TF1("fitPol0","pol0");
   fitPol0 -> SetLineColor(kRed);   
   fitPol0 -> SetLineStyle(7);   

   c11o12_1 -> cd();
   hd_Wp_11o12Ratio_vs_WBosonEta_BGsub -> Fit("fitPol0");
   //hd_Wp_11o12Ratio_vs_WBosonEta_BGsub -> Draw("e");

   c11o12_2 -> cd();

   //hd_Wp_11o12Ratio_vs_EleEta_BGsub    -> GetXaxis() -> SetRange(3, 8);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub    -> Fit("fitPol0");
   //hd_Wp_11o12Ratio_vs_EleEta_BGsub    -> Draw("e");

   c11o12_3 -> cd();
   hd_Wm_11o12Ratio_vs_WBosonEta_BGsub -> Fit("fitPol0");
   //hd_Wm_11o12Ratio_vs_WBosonEta_BGsub -> Draw("e");

   c11o12_4 -> cd();
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub    -> GetXaxis() -> SetRange(3, 8);
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub    -> Fit("fitPol0","","",-1,1);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub    -> Fit("fitPol0");
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub    -> GetFunction("pol0") -> SetLineColor(kRed);
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub    -> GetFunction("pol0") -> SetLineStyle(7);
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub    -> Draw("e");

   c11o12_5 -> cd();
   hd_Wp_11o12Ratio_vs_WBosonRapidity_BGsub -> Fit("fitPol0");

   c11o12_6 -> cd();
   hd_Wm_11o12Ratio_vs_WBosonRapidity_BGsub -> Fit("fitPol0");

   c11o12 -> Print(outPath + "/plot_R11overR12_Ratio.eps");
   c11o12 -> Print(outPath + "/plot_R11overR12_Ratio.png");
  


   TCanvas *c11o12Eff    = new TCanvas("c11o12Eff", "[run 11 W+]/[run 12 W+]", 800, 800);
   c11o12Eff -> Divide(1,2);

   c11o12Eff_1 -> cd();

   //hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor    -> GetXaxis() -> SetRange(3, 8);
   hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor    -> Fit("fitPol0");
   //hd_Wp_11o12Ratio_vs_EleEta_BGsub_EffCor    -> Draw("e");

   c11o12Eff_2 -> cd();
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor    -> GetXaxis() -> SetRange(3, 8);
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor    -> Fit("fitPol0","","",-1,1);
   hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor    -> Fit("fitPol0");
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor    -> GetFunction("pol0") -> SetLineColor(kRed);
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor    -> GetFunction("pol0") -> SetLineStyle(7);
   //hd_Wm_11o12Ratio_vs_EleEta_BGsub_EffCor    -> Draw("e");

   c11o12Eff -> Print(outPath + "/plot_R11overR12_EFF_Ratio.eps");
   c11o12Eff -> Print(outPath + "/plot_R11overR12_EFF_Ratio.png");
   */

   /*
   //-----------------------------------------------------------------------------------------
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

   */

}
