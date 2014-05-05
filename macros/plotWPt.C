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


void plotWPt()
{

   //gROOT->Reset();

   // Load the libraries:
   //gROOT->Macro("/star/u/fazio/offline/users/fazio/vbasym/macros/loadLibs.C");

   //TString inPath  = "/star/u/fazio/vbasym/macros";
   TString outPath = "../../vbasym_plots/";

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

// Chain Root Trees:

  TChain WpMCTuneAChain("t");
  TChain WpMCPerugiaChain("t");

  //WpMCTuneAChain.Add("/star/data05/scratch/fazio/runlists/run11_mc_Wp2enu.lis_-m_--jpm_0.5/tree/run11_mc_Wp2enu_*_tree.root");
   WpMCTuneAChain.Add("/star/data05/scratch/fazio/runlists/run11_mc_Wp2enu.lis_-m_--jpm_0.5/tree/run11_mc_Wp2enu_00_tree.root"); 
   WpMCTuneAChain.Add("/star/data05/scratch/fazio/runlists/run11_mc_Wp2enu.lis_-m_--jpm_0.5/tree/run11_mc_Wp2enu_01_tree.root"); 
   WpMCTuneAChain.Add("/star/data05/scratch/fazio/runlists/run11_mc_Wp2enu.lis_-m_--jpm_0.5/tree/run11_mc_Wp2enu_02_tree.root"); 
  
   WpMCPerugiaChain.Add("/star/data05/scratch/fazio/runlists/run12_mc_Wp2enu.lis_-m_--jpm_0.5/tree/run12_mc_Wp2enu_00_tree.root");  
   WpMCPerugiaChain.Add("/star/data05/scratch/fazio/runlists/run12_mc_Wp2enu.lis_-m_--jpm_0.5/tree/run12_mc_Wp2enu_01_tree.root");  
   WpMCPerugiaChain.Add("/star/data05/scratch/fazio/runlists/run12_mc_Wp2enu.lis_-m_--jpm_0.5/tree/run12_mc_Wp2enu_02_tree.root"); 
   WpMCPerugiaChain.Add("/star/data05/scratch/fazio/runlists/run12_mc_Wp2enu.lis_-m_--jpm_0.5/tree/run12_mc_Wp2enu_03_tree.root");


TH1F *htuneA_WPt     = new TH1F("htuneA_WPt","TUNE A kt=1; P^{W}_{T} (GeV); events",40,0.,20);
TH1F *hperugia0_WPt     = new TH1F("hperugia0_WPt","Perugia0 510GeV; P^{W}_{T} (GeV); events",40,0.,20);


 WpMCTuneAChain.Project("htuneA_WPt","e.mWEvent.mP4WBoson.Pt()"); 
 WpMCPerugiaChain.Project("hperugia0_WPt","e.mWEvent.mP4WBoson.Pt()"); 

 TGraph *g1 = new TGraph("./curves/0304002.Wptcurve.txt","%lg %lg");
 TGraph *g2 = new TGraph("./curves/Altarelli.530GeV.txt","%lg %lg");
 TGraph *g3 = new TGraph("./curves/UA1.530GeV.txt","%lg %lg");

 TFile fpyTunes("./curves/histos_pyTunes.root");
 TH1F *htuneA_kt1ckin1_WPt   = (TH1F*)fpyTunes.Get("W_kt1ckin1_Pt");
 TH1F *htuneA_kt1ckin5_WPt   = (TH1F*)fpyTunes.Get("W_kt1ckin5_Pt");
 TH1F *htuneA_kt1ckin10_WPt  = (TH1F*)fpyTunes.Get("W_kt1ckin10_Pt");
 TH1F *hperugia0_ckin1_WPt  = (TH1F*)fpyTunes.Get("W_P0_Pt");
 TH1F *hperugia0_ckin5_WPt  = (TH1F*)fpyTunes.Get("W_P0b_Pt");


   Double_t int_htuneA_WPt            =  htuneA_WPt     -> Integral();
   Double_t int_hperugia0_WPt         =  hperugia0_WPt  -> Integral();
   Double_t int_hperugia0_ckin1_WPt   =  hperugia0_ckin1_WPt  -> Integral();
   Double_t int_hperugia0_ckin5_WPt   =  hperugia0_ckin5_WPt  -> Integral();
   Double_t int_g1                    =  integrateGraph(g1); // the method Integral() not yet implemented on TGraph in ROOT4STAR  
   Double_t int_g2                    =  integrateGraph(g2);
   Double_t int_g3                    =  integrateGraph(g3);
   //Double_t int_g1 = g1-> Integral(5, 6);
   Double_t int_tuneA_kt1ckin1_WPt    =  htuneA_kt1ckin1_WPt  -> Integral(); 
   Double_t int_tuneA_kt1ckin5_WPt    =  htuneA_kt1ckin5_WPt  -> Integral();  
   Double_t int_tuneA_kt1ckin10_WPt   =  htuneA_kt1ckin10_WPt -> Integral(); 

   float scaleTA_hperugia0        = int_htuneA_WPt / int_hperugia0_WPt;
   float scaleTA_hperugia0ckin1   = int_htuneA_WPt / int_hperugia0_ckin1_WPt;
   float scaleTA_hperugia0ckin5   = int_htuneA_WPt / int_hperugia0_ckin5_WPt;
   float scaleTA_TAkt1ckin1       = int_htuneA_WPt / int_tuneA_kt1ckin1_WPt;
   float scaleTA_TAkt1ckin5       = int_htuneA_WPt / int_tuneA_kt1ckin5_WPt;
   float scaleTA_TAkt1ckin10      = int_htuneA_WPt / int_tuneA_kt1ckin10_WPt;
   float scaleTA_g1               = int_htuneA_WPt / int_g1;
   float scaleTA_g2               = int_htuneA_WPt / int_g2;
   float scaleTA_g3               = int_htuneA_WPt / int_g3;

   
   cout << "Integral of TUNE A = " <<  int_htuneA_WPt << endl;
   cout << "Integral of RhicBOS 500 GeV = " <<  int_g1 << endl;
   cout << "Scale of RhicBOS 500 GeV = " <<  scaleTA_g1 << endl;
   cout << "Integral of Altarelli et al. 530 GeV = " <<  int_g2 << endl;
   cout << "Scale of Altarelli et al. 530 GeV = " <<  scaleTA_g2 << endl;
   cout << "Integral of UA1 530 GeV = " <<  int_g3 << endl;
   cout << "Scale of UA1 530 GeV = " <<  scaleTA_g3 << endl;
   

   TH1F   *hperugia0_WPt_1 = (TH1F *)hperugia0_WPt->Clone("hperugia0_WPt_1");
   TH1F   *htuneA_kt1ckin1_WPt_1 = (TH1F *)htuneA_kt1ckin1_WPt->Clone("htuneA_kt1ckin1_WPt_1");
   TGraph *g1_1 = (TGraph *)g1->Clone("g1_1");
   TGraph *g2_1 = (TGraph *)g2->Clone("g2_1");
   TGraph *g3_1 = (TGraph *)g3->Clone("g3_1");

   hperugia0_WPt ->Scale(scaleTA_hperugia0);
   hperugia0_ckin1_WPt ->Scale(scaleTA_hperugia0ckin1);
   hperugia0_ckin5_WPt ->Scale(scaleTA_hperugia0ckin5);
   htuneA_kt1ckin1_WPt  ->Scale(scaleTA_TAkt1ckin1);
   htuneA_kt1ckin5_WPt  ->Scale(scaleTA_TAkt1ckin5);
   htuneA_kt1ckin10_WPt ->Scale(scaleTA_TAkt1ckin10);
   scaleGraph(g1, 0.50*scaleTA_g1);
   //scaleGraph(g1, 15);
   scaleGraph(g2, 0.05*scaleTA_g2);
   scaleGraph(g3, 0.40*scaleTA_g3);

   TCanvas *c1 = new TCanvas("c1", "", 600, 900);
   TH2F *fr1 = new TH2F("fr1","",100,0,20,100,0,22); 
         fr1 ->SetNameTitle("fr1", "RhicBOS 500 GeV");
   TH2F *fr2 = new TH2F("fr2","",100,0,20,100,0,0.13); 
         fr2 ->SetNameTitle("fr2", "Altarelli et al. 530 GeV"); 
   TH2F *fr3 = new TH2F("fr3","",100,0,20,100,0,0.11); 
         fr3 ->SetNameTitle("fr3", "UA1 530 GeV"); 

   c1->Divide(1,5);

   c1_1->cd();
   htuneA_WPt ->Draw();
   //g1 ->Scale();
   //g1 ->Draw("same");

   c1_2->cd();
   hperugia0_WPt_1 ->Draw();
   //g1 ->Scale();
   //g1 ->Draw("same");

   c1_3->cd();
   fr1->Draw();
   g1_1 ->Draw("same");
   //g1 ->Draw("AL");

   c1_4->cd();
   fr2->Draw();
   g2_1 ->Draw("same"); 
   g2_1 ->SetLineColor(kRed); 

   c1_5->cd();
   fr3->Draw();
   g3_1 ->Draw("same*"); 
   //g3 ->Draw("A*"); 
   g3_1 ->SetMarkerColor(kBlue);

   c1->Print(outPath + "/plot_WPT_1.png");  



   TCanvas *c2 = new TCanvas("c2", "", 600, 600);

   c2->cd();
   TLegend *leg1 = new TLegend(0.4, 0.5, 0.75, 0.9);
   leg1 -> AddEntry(htuneA_WPt,"PY. TUNE A kt=1 ckin(3)=0.5", "l");
   leg1 -> AddEntry(htuneA_kt1ckin1_WPt,"PY. TUNE A kt=1 ckin(3)=1.0", "l");
   leg1 -> AddEntry(htuneA_kt1ckin5_WPt,"PY. TUNE A kt=1 ckin(3)=5.0", "l");
   leg1 -> AddEntry(htuneA_kt1ckin10_WPt,"PY. TUNE A kt=1 ckin(3)=10.0", "l");
   leg1 -> AddEntry(hperugia0_ckin1_WPt,"PY. Perugia0 kt=2 ckin(3)=1", "l");
   leg1 -> AddEntry(hperugia0_ckin5_WPt,"PY. Perugia0 kt=2 ckin(3)=5", "l");
   leg1 -> AddEntry(hperugia0_WPt,"PY. Perugia0 kt=2 ckin(3)=10", "l");
   leg1 -> AddEntry(g1,"RhicBOS 500 GeV","l");
   leg1 -> AddEntry(g2,"Altarelli et al. 530 GeV","l");
   leg1 -> AddEntry(g3,"UA1 530 GeV","p");

   htuneA_WPt    -> SetStats(0);
   htuneA_WPt    -> SetTitle("PYTHIA tuning");
   htuneA_WPt    -> GetYaxis()-> SetTitleOffset(1.8);
   htuneA_WPt    -> Draw();  
   hperugia0_WPt -> SetStats(0);
   hperugia0_WPt -> SetLineColor(kGreen); 
   hperugia0_WPt -> Draw("same");
   g1            -> SetLineColor(kBlue); 
   g1            -> Draw("same");
   g2            -> SetLineColor(kRed); 
   g2            -> Draw("same");
   g3            -> SetMarkerColor(kBlue); 
   g3            -> Draw("same*");
   htuneA_kt1ckin1_WPt  -> SetStats(0);
   htuneA_kt1ckin1_WPt  -> SetLineColor(1); 
   htuneA_kt1ckin1_WPt  -> SetLineStyle(2);
   htuneA_kt1ckin1_WPt  -> Draw("same");
   htuneA_kt1ckin5_WPt  -> SetStats(0);
   htuneA_kt1ckin5_WPt  -> SetLineColor(1); 
   htuneA_kt1ckin5_WPt  -> SetLineStyle(3);
   htuneA_kt1ckin5_WPt  -> Draw("same");
   htuneA_kt1ckin10_WPt -> SetStats(0);
   htuneA_kt1ckin10_WPt -> SetLineColor(1); 
   htuneA_kt1ckin10_WPt -> SetLineStyle(5);
   htuneA_kt1ckin10_WPt -> Draw("same");
   hperugia0_ckin1_WPt  -> SetStats(0);
   hperugia0_ckin1_WPt  -> SetLineColor(kGreen); 
   hperugia0_ckin1_WPt  -> SetLineStyle(2);
   hperugia0_ckin1_WPt  -> Draw("same");
   hperugia0_ckin5_WPt  -> SetStats(0);
   hperugia0_ckin5_WPt  -> SetLineColor(kGreen); 
   hperugia0_ckin5_WPt  -> SetLineStyle(3);
   hperugia0_ckin5_WPt  -> Draw("same");
   leg1          -> Draw();

   c2->Print(outPath + "/plot_WPT_2.png");  

}
