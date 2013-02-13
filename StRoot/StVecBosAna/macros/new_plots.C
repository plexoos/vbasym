void new_plots() {

// Load the libraries:

  gROOT->Macro("/star/u/fazio/offline/users/fazio/vbasym/macros/loadLibs.C"); 

// Delete the histos:
//   h*

// open data files:

//TFile *fileZ = TFile::Open("/eicdata/eic0004/PYTHIA/pp/Z0_pythia.pp.250x250.1Mevents.ckin3=1.kt=0.5.root");

//TFile *fileW = TFile::Open("/eicdata/eic0004/PYTHIA/pp/W_24Aug_1Mgen.root");


// Chain Root Trees:

  TChain WpMCChain("t");
  TChain WmMCChain("t");
  TChain WDataChain("t");

   WpMCChain.Add("/star/data05/scratch/fazio/wtree_run11/MC_list_Wp_2012_*.wtree.root");
   WmMCChain.Add("/star/data05/scratch/fazio/wtree_run11/MC_list_Wm_2012_*.wtree.root");

   // WpMCChain.Add("/star/u/fazio/offline/users/fazio/vbasym/MC_list_Wp_2012_00.wtree.root");
   WDataChain.Add("/star/data05/scratch/fazio/wtree_run11/R*.wtree.root");
   //   WmMCChain.Add("/star/u/fazio/offline/users/fazio/vbasym/MC_list_Wm_2012_*.wtree.root");

// Set a max number of entries:
   WpMCChain.SetMaxEntryLoop(10000);
   WmMCChain.SetMaxEntryLoop(10000);
//................................


  gStyle->SetOptStat(10);
  gStyle->SetMarkerStyle(20);
  gStyle->SetFillColor(kYellow);
// Plot the shit you want:


//  MCChain.Draw("l2bitET");
  //MCChain.Draw("e.mP4JetRecoil.Pt():e.mWEvent.mP4WBoson.Pt()>>h1");
  //h1->Draw("colz"); 

 TH2F *h1 = new TH2F("h1","W^{+} - Recoil from jets;Pt^{Gen}_{W} (rad);Pt^{Reco}_{W} (rad)",40,0,60,40,0,85);  
 TH2F *h1_2 = new TH2F("h1_2","W^{+} - Recoil from jets - TOTAL;Pt^{Gen}_{W} (rad);Pt^{Reco}_{W} (rad)",40,0,60,40,0,85);
 TH2F *h1_3 = new TH2F("h1_3","W^{-} - Recoil from jets;Pt^{Gen}_{W} (rad);Pt^{Reco}_{W} (rad)",40,0,60,40,0,85);  
 TH2F *h1_4 = new TH2F("h1_4","W^{-} - Recoil from jets - TOTAL;Pt^{Gen}_{W} (rad);Pt^{Reco}_{W} (rad)",40,0,60,40,0,85); 
 WpMCChain.Project("h1","e.mP4JetRecoil.Pt():e.mWEvent.mP4WBoson.Pt()"); 
 WpMCChain.Project("h1_2","e.mP4JetTotal.Pt():e.mWEvent.mP4WBoson.Pt()");
 WmMCChain.Project("h1_3","e.mP4JetRecoil.Pt():e.mWEvent.mP4WBoson.Pt()"); 
 WmMCChain.Project("h1_4","e.mP4JetTotal.Pt():e.mWEvent.mP4WBoson.Pt()");
 TCanvas *c1 = new TCanvas("c1","Recoil From Jets",500,500);
 c1->Divide(2,2);
 c1_1->SetLogz(1);
 c1_1->cd();
 h1->Draw("colz");
 // gPad->Update();
 // TPaveStats* stats = dynamic_cast<TPaveStats*>(h1->FindObject("stats"));
 // stats->SetX1NDC(0.8);
 // stats->SetX2NDC(0.6);
 // h1->Draw("colz");
 c1_2->SetLogz(1);
 c1_2->cd();
 h1_2->Draw("colz");
 c1_3->SetLogz(1);
 c1_3->cd();
 h1_3->Draw("colz");
 c1_4->SetLogz(1);
 c1_4->cd();
 h1_4->Draw("colz");

 c1->Print("./plots/h1.eps");

 TH2F *h2 = new TH2F("h2","W^{+} - Recoil from jets;#phi^{Gen}_{W} (rad);#phi^{Reco}_{W} (rad)",40,-TMath::Pi(),TMath::Pi(),40,-TMath::Pi(),TMath::Pi());
 TH2F *h2_2 = new TH2F("h2_2","W^{+} - Recoil from jets - TOTAL;#phi^{Gen}_{W} (rad);#phi^{Reco}_{W} (rad)",40,-TMath::Pi(),TMath::Pi(),40,-TMath::Pi(),TMath::Pi());
 TH2F *h2_3 = new TH2F("h2_3","W^{-} - Recoil from jets;#phi^{Gen}_{W} (rad);#phi^{Reco}_{W} (rad)",40,-TMath::Pi(),TMath::Pi(),40,-TMath::Pi(),TMath::Pi());
 TH2F *h2_4 = new TH2F("h2_4","W^{-} - Recoil from jets - TOTAL;#phi^{Gen}_{W} (rad);#phi^{Reco}_{W} (rad)",40,-TMath::Pi(),TMath::Pi(),40,-TMath::Pi(),TMath::Pi()); 
 WpMCChain.Project("h2","e.mP4JetRecoil.Phi():e.mWEvent.mP4WBoson.Phi()","e.mP4JetRecoil.Pt() > 0");
 WpMCChain.Project("h2_2","e.mP4JetTotal.Phi():e.mWEvent.mP4WBoson.Phi()","e.mP4JetRecoil.Pt() > 0"); 
 WmMCChain.Project("h2_3","e.mP4JetRecoil.Phi():e.mWEvent.mP4WBoson.Phi()","e.mP4JetRecoil.Pt() > 0");
 WmMCChain.Project("h2_4","e.mP4JetTotal.Phi():e.mWEvent.mP4WBoson.Phi()","e.mP4JetRecoil.Pt() > 0");
 TCanvas *c2 = new TCanvas("c2","Recoil From Jets",500,500);
 c2->Divide(2,2);
 c2_1->cd();
 c2_1->SetLogz(1);
 h2->Draw("colz");
 c2_2->cd();
 c2_2->SetLogz(1);
 h2_2->Draw("colz");
 c2_3->cd();
 c2_3->SetLogz(1);
 h2_3->Draw("colz");
 c2_4->cd();
 c2_4->SetLogz(1);
 h2_4->Draw("colz");

 c2->Print("./plots/h2.eps");


 TH2F *h3_1 = new TH2F("h3_1","W^{+} - Recoil from tracks;Pt^{Gen}_{W} (rad);Pt^{Reco}_{W} (rad)",40,0,40,40,0,40); 
 TH2F *h3_2 = new TH2F("h3_2","W^{-} - Recoil from tracks;Pt^{Gen}_{W} (rad);Pt^{Reco}_{W} (rad)",40,0,40,40,0,40); 
 WpMCChain.Project("h3_1","e.mP3RecoilFromTracks.Pt():e.mWEvent.mP4WBoson.Pt()","e.mP3RecoilFromTracks.Pt() > 2."); 
 WmMCChain.Project("h3_2","e.mP3RecoilFromTracks.Pt():e.mWEvent.mP4WBoson.Pt()","e.mP3RecoilFromTracks.Pt() > 2.");
 TCanvas *c3 = new TCanvas("c3","Recoil From Tracks",500,500);
 c3->Divide(1,2);
 c3_1->SetLogz(1);
 c3_1->cd();
 h3_1->Draw("colz");
 c3_2->SetLogz(1);
 c3_2->cd();
 h3_2->Draw("colz");

 c3->Print("./plots/h3.eps");



 TH2F *h4_1 = new TH2F("h4_1","W^{+} - Recoil from tracks;#phi^{Gen}_{W} (rad);#phi^{Reco}_{W} (rad)",40,-TMath::Pi(),TMath::Pi(),40,-TMath::Pi(),TMath::Pi());
 TH2F *h4_2 = new TH2F("h4_2","W^{-} - Recoil from tracks;#phi^{Gen}_{W} (rad);#phi^{Reco}_{W} (rad)",40,-TMath::Pi(),TMath::Pi(),40,-TMath::Pi(),TMath::Pi());
 WpMCChain.Project("h4_1","e.mP3RecoilFromTracks.Phi():e.mWEvent.mP4WBoson.Phi()","e.mP3RecoilFromTracks.Pt() > 2.");
 WmMCChain.Project("h4_2","e.mP3RecoilFromTracks.Phi():e.mWEvent.mP4WBoson.Phi()","e.mP3RecoilFromTracks.Pt() > 2.");
 TCanvas *c4 = new TCanvas("c4","Recoil From Tracks",500,500);
 c4->Divide(1,2);
 c4_1->cd();
 c4_1->SetLogz(1);
 h4_1->Draw("colz");
 c4_2->cd();
 c4_2->SetLogz(1);
 h4_2->Draw("colz");

 c4->Print("./plots/h4.eps");



 TH1F *h5_1 = new TH1F("h5_1","Isolated Electron; P^{ele}_{T} (GeV);",40,0.,100);
 WpMCChain.Project("h5_1","e.mTracksCandidate.mP3AtDca.Pt()");

 TH1F *h5_2 = new TH1F("h5_2","Isolated Electron - data; P^{ele}_{T} (GeV);",40,0.,100);
 WDataChain.Project("h5_2","e.mTracksCandidate.mP3AtDca.Pt()");
 TCanvas *c5 = new TCanvas("c5","Isolated electron",500,500);

 c5->Divide(1,2);
 c5_1->cd();
 h5_1->Draw();
 TLine *line5 = new TLine(15,0,15,c5->GetFrame()->GetY2());
 // 7000);//c5->GetFrame()->GetY2());
 line5->SetLineColor(kRed);
 line5->Draw();
 c5_2->cd();
 h5_2->Draw("e0");
 c5->Print("./plots/h5.eps");
 


 TH1F *h7_1 = new TH1F("h7_1","Recoil from tracks; P_{T};",40,0.,45.);
 WpMCChain.Project("h7_1","e.mHadRecoilFromTracksPt","e.mHadRecoilFromTracksPt > 0");

 TH1F *h7_2 = new TH1F("h7_2","electron candidate; P_{T};",40,0.,60.);
 WpMCChain.Project("h7_2","e.mTracksCandidate.mP3AtDca.Pt()","");

 TH1F *h7_3 = new TH1F("h7_3","P_{T} balance from tracks; P_{T};",40,0.,60.);
 WpMCChain.Project("h7_3","e.mP3BalanceFromTracks.Pt()","e.mP3BalanceFromTracks.Pt() > 0");

 TH1F *h7_4 = new TH1F("h7_4","P_{T}-balance cos(#phi); P_{T};",40,-100.,100.);
 WpMCChain.Project("h7_4","e.mPtBalanceCosPhiFromTracks","e.mP3BalanceFromTracks.Pt() > 0");

 TH1F *h7_5 = new TH1F("h7_5","; #Delta #phi;",40,-TMath::Pi(),TMath::Pi());
 WpMCChain.Project("h7_5","e.mBalanceDeltaPhiFromTracks","e.mP3BalanceFromTracks.Pt() > 0");



 TCanvas *c7 = new TCanvas("c7","Pt balance - MC",600,600);
 //gStyle->SetOptStat(0);

 c7->Divide(2,2);
 c7_1->cd();
 h7_1->Draw();
 c7_2->cd();
 h7_2->Draw();
 c7_3->cd();
 h7_3->Draw();
 c7_4->cd();
 h7_4->Draw();

 c7->Print("./plots/h7.eps");

 TCanvas *c7bis = new TCanvas("c7bis","Pt balance - MC",600,600);
 c7bis->Divide(1,2);
 c7bis_1->cd();
 h7_3->Draw();
 c7bis_2->cd();
 h7_5->Draw();

 c7bis->Print("./plots/h7bis.eps");



 TH1F *h7d_1 = new TH1F("h7d_1","Recoil from tracks - data; P_{T};",40,0.,45.);
 WDataChain.Project("h7d_1","e.mHadRecoilFromTracksPt","e.mHadRecoilFromTracksPt > 0");
 // WDataChain.Project("d7_1","e.mP3RecoilFromTracks.Pt()","e.mP3RecoilFromTracks.Pt() > 0");

 TH1F *h7d_2 = new TH1F("h7d_2","Isolated Electron - data; P^{ele}_{T} (GeV);",40,0.,100);
 WDataChain.Project("h7d_2","e.mTracksCandidate.mP3AtDca.Pt()");

 // TH1F *h7d_3 = new TH1F("h7d_3","P_{T} balance from tracks - data; P_{T};",40,0.,60.);
 // WDataChain.Project("h7d_3","e.mP3BalanceFromTracks.Pt()","e.mP3BalanceFromTracks.Pt() > 0");

 TH1F *h7d_4 = new TH1F("h7d_4","P_{T}-balance cos(#phi) - data; P_{T};",40,-100.,100.);
 WDataChain.Project("h7d_4","e.mPtBalanceCosPhiFromTracks","e.mHadRecoilFromTracksPt > 0");

 TH1F *h7d_5 = new TH1F("h7d_5","data; #Delta #phi;",40,-TMath::Pi(),TMath::Pi());
 WDataChain.Project("h7d_5","e.mBalanceDeltaPhiFromTracks","e.mHadRecoilFromTracksPt > 0");

 TCanvas *d7 = new TCanvas("d7","Pt balance data",600,600);

  d7->Divide(2,2);
  d7_1->cd();
  h7d_1->Draw("e0");
  d7_2->cd();
  h7d_2->Draw("e0");
  d7_3->cd();
  // h7d_3->Draw("e0");
  d7_4->cd();
  h7d_4->Draw("e0");

 d7->Print("./plots/h7d.eps");

 TCanvas *d7b = new TCanvas("d7b","Pt balance data",600,600);
  d7b->Divide(1,2);
  d7b_1->cd();
  // h7d_3->Draw("e0");
  d7b_2->cd();
  h7d_5->Draw("e0");

 d7b->Print("./plots/h7dbis.eps");

// */

 
 TH1F *h8_1 = new TH1F("h8_1","electron candidate - ratio 2x2/4x4; fraction: cluster 2x2/4x4 ET;",40,0.,1.);
 WpMCChain.Project("h8_1","e.mTracksCandidate.mCluster2x2.ET/e.mTracksCandidate.mP3InNearConeBTow.Pt()","");
 TH2F *h8_2 = new TH2F("h8_2","; P_{T}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
 WpMCChain.Project("h8_2","e.mPtBalanceCosPhiFromTracks:e.mTracksCandidate.mCluster2x2.ET","e.mTracksCandidate.mCluster2x2.ET > 20");

 TCanvas *c8 = new TCanvas("c8","",500,500);
 //gStyle->SetOptStat(0);
 c8->Divide(1,2);
 c8_1->cd();
 h8_1->SetFillColor(kRed);
 h8_1->Draw();
 // gStyle->SetOptStat(10);
 c8_2->cd();
 //h8_2->SetStat(10);
 h8_2->Draw("colz");

 c8->Print("./plots/h8.eps");
 
 TH1F *h8d_1 = new TH1F("h8d_1","electron candidate - ratio 2x2/4x4 - data; fraction: cluster 2x2/4x4 ET;",40,0.,1.);
 WDataChain.Project("h8d_1","e.mTracksCandidate.mCluster2x2.ET/e.mTracksCandidate.mP3InNearConeBTow.Pt()","");
 TH2F *h8d_2 = new TH2F("h8d_2","; P_{T}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
 WDataChain.Project("h8d_2","e.mPtBalanceCosPhiFromTracks:e.mTracksCandidate.mCluster2x2.ET");
 TH2F *h8d_3 = new TH2F("h8d_3","2x2 cluster E_{T} > 20 GeV; P_{T}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
 WDataChain.Project("h8d_3","e.mPtBalanceCosPhiFromTracks:e.mTracksCandidate.mCluster2x2.ET","e.mTracksCandidate.mCluster2x2.ET > 20");

 TH2F *h8d_4 = new TH2F("h8d_4","2x2 cluster E_{T} > 20 GeV && P_{T}^{recoil}>0; P_{T}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
 WDataChain.Project("h8d_4","e.mPtBalanceCosPhiFromTracks:e.mTracksCandidate.mCluster2x2.ET","e.mTracksCandidate.mCluster2x2.ET > 20 && e.mHadRecoilFromTracksPt > 0");

 TH2F *h8d_5 = new TH2F("h8d_5","2x2 cluster E_{T} > 20 GeV && P_{T}^{recoil}>0 && P_{T}^{ele}>15 GeV; P_{T}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
 WDataChain.Project("h8d_5","e.mPtBalanceCosPhiFromTracks:e.mTracksCandidate.mCluster2x2.ET","e.mTracksCandidate.mCluster2x2.ET > 20 && e.mHadRecoilFromTracksPt > 0 && e.mTracksCandidate.mP3AtDca.Pt() > 15");

 TH2F *h8d_6 = new TH2F("h8d_6","2x2 cluster E_{T} > 20 GeV && P_{T}^{recoil}>0 && P_{T}^{ele}>18 GeV && Isolation; P_{T}; P_{T}-balance cos(#phi)",40,0.,60.,40,-40,60);
 WDataChain.Project("h8d_6","e.mPtBalanceCosPhiFromTracks:e.mTracksCandidate.mCluster2x2.ET","e.mTracksCandidate.mCluster2x2.ET > 20 && e.mHadRecoilFromTracksPt > 0 && e.mTracksCandidate.mP3AtDca.Pt() > 18 ");

 TCanvas *c8d = new TCanvas("c8d","",500,700);

 //gStyle->SetOptStat(0);
 c8d->Divide(2,3);
 c8d_1->cd();
 h8d_1->SetFillColor(kRed);
 h8d_1->Draw("e0");
 TLine *lh8d_1 = new TLine(0.9,0,0.9,2000);
 lh8d_1->SetLineColor(kRed);
 lh8d_1->Draw();

 // gStyle->SetOptStat(10);
 c8d_2->cd();
 //h8_2->SetStat(10);
 h8d_2->Draw("colz");
 c8d_3->cd();
 h8d_3->Draw("colz");
 c8d_4->cd();
 h8d_4->Draw("colz");
 c8d_5->cd();
 h8d_5->Draw("colz");
 c8d_6->cd();
 h8d_6->Draw("colz");

 c8d->Print("./plots/h8d.eps");
 

}
