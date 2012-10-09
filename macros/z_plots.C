void z_plots() {

// Load the library:
// .L ../zana_mc/libzana.so

// open data files:
TFile *fileZ = TFile::Open("/eicdata/eic0004/PYTHIA/pp/Z0_pythia.pp.250x250.1Mevents.ckin3=1.kt=0.5.root");
//TFile *fileW = TFile::Open("/eicdata/eic0004/PYTHIA/pp/pythia.pp.250x250.1Mevents.ckin3=1.kt=0.5.root");
TFile *fileW = TFile::Open("/eicdata/eic0004/PYTHIA/pp/W_24Aug_1Mgen.root");

// Define style
gStyle->SetPaperSize(20,20);
gStyle->SetTitleSize(0.06);
gStyle->SetPadBottomMargin(0.15);
gStyle->SetPadLeftMargin(0.15);

fileZ->cd();
//pyTree.Show(0);
//zTree.Show(0);


/*
// Create histograms
 TH1F *Z_theta           = new TH1F("Z_theta","polar angle;#theta_{Z}",80,0,TMath::Pi());
 TH1F *Z_pseudorapidity  = new TH1F("Z_pseudorapidity","pseudo-rapidity;#eta_{Z}",80,-6,6);
 TH1F *Z_mass            = new TH1F("Z_mass","Z mass;M_{Z}",80,60,120);
 TH1F *l1_angle          = new TH1F("l1_angle","lepton angle;#theta_{lep}",80,0,TMath::Pi());
 TH1F *l2_angle          = new TH1F("l2_angle","lepton angle;#theta_{lep}",80,0,TMath::Pi());
 TH1F *l1_pseudorapidity = new TH1F("l1_pseudorapidity","lepton pseudo-rapidity;#eta_{lep}",80,-6,6);
 TH1F *l2_pseudorapidity = new TH1F("l2_pseudorapidity","lepton pseudo-rapidity;#eta_{lep}",80,-6,6);

// Fill histograms
TString etaz("-log(tan(zEvent.ZReco.Theta()/2.))");
TString etal1("-log(tan(zEvent.l1.Theta()/2.))"); 
TString etal2("-log(tan(zEvent.l2.Theta()/2.))"); 

 zTree->Project("Z_theta","zEvent.ZReco.Theta()");  
 zTree->Project("Z_pseudorapidity",etaz);   
 zTree->Project("Z_mass","zEvent.ZReco.M()"); 
 zTree->Project("l1_angle","zEvent.l1.Theta()");  
 zTree->Project("l2_angle","zEvent.l2.Theta()"); 
 zTree->Project("l1_pseudorapidity",etal1);  
 zTree->Project("l2_pseudorapidity",etal2); 

// Plot histograms

 TCanvas *c1 = new TCanvas("c1","test_Z",800,800);
c1->Divide(2,3);


c1_1->cd();
 Z_mass->Draw();
 c1_1->SetBottomMargin(0.15);
c1_2->cd();
 Z_theta->Draw();
 c1_2->SetBottomMargin(0.15);
c1_3->cd();
 Z_pseudorapidity->Draw();
 c1_3->SetBottomMargin(0.15);
c1_4->cd();
 TLegend* leg_lepton_angle = new TLegend(0.5, 0.3, 0.3, 0.5);
 l1_angle->Draw(); 
 l2_angle->Draw("same");
 l2_angle->SetLineColor(kRed);
   leg_lepton_angle->AddEntry(l1_angle,"l1");
   leg_lepton_angle->AddEntry(l2_angle,"l2");
   leg_lepton_angle->Draw();
 // c1_4->SetBottomMargin(0.15);
c1_5->cd(); 
l1_pseudorapidity->Draw(); 
l2_pseudorapidity->Draw("same");
l2_pseudorapidity->SetLineColor(kRed);
 gPad->Update();
 //cout<<c1_5->GetFrame()->GetY2()<<endl;
 TLine *staraccl = new TLine(-1,0,-1,c1_5->GetFrame()->GetY2());
 TLine *staraccu = new TLine(1,0,1,c1_5->GetFrame()->GetY2());
staraccl->Draw(); 
staraccu->Draw(); 


 c1->Print("./plots/Z_rapidity.eps");

*/
//-------------------------------------------------------------------

//pyTree.Show(0);
//wTree.Show(0);

fileW->cd();
// Create histograms
 TH1F *W_theta = new TH1F("W_theta","polar angle;#theta_{W}",80,0,TMath::Pi()); 
 TH1F *W_pseudorapidity = new TH1F("W_pseudorapidity","pseudo-rapidity;#eta_{W}",80,-6,6);
 TH1F *W_pseudorapidity2 = new TH1F("W_pseudorapidity2","pseudo-rapidity;#eta_{W}",80,-6,6); 
 TH1F *W_rapidity = new TH1F("W_rapidity","rapidity (|#eta_{el}|<1);y_{W}",80,-2,2); 
 TH1F *Wm_rapidity = new TH1F("Wm_rapidity","rapidity (|#eta_{el}|<1);y_{W^{-}}",4,-0.5,1.5); 
 TH1F *Wp_rapidity = new TH1F("Wp_rapidity","rapidity (|#eta_{el}|<1);y_{W^{+}}",8,-2.0,2.0);
 TH1F *lep_charge = new TH1F("lep_charge","lepton charge (|#eta_{el}|<1);Q",4,-2.0,2.0); 
 TH1F *W_mass = new TH1F("W_mass","W mass;M_{W}",80,60,120);  
 TH1F *l_angle = new TH1F("l_angle","lepton angle;#theta_{lep}",80,0,TMath::Pi());
 TH1F *nu_angle = new TH1F("nu_angle","lepton angle;#theta_{lep}",80,0,TMath::Pi());
 TH1F *l_pseudorapidity = new TH1F("l_pseudorapidity","lepton pseudo-rapidity;#eta_{lep}",80,-6,6); 
 TH1F *nu_pseudorapidity = new TH1F("nu_pseudorapidity","lepton pseudo-rapidity;#eta_{lep}",80,-6,6); 
 TH1F *recoil_tracks_pseudorapidity = new TH1F("recoil_tracks_pseudorapidity","recoil tracks pseudo-rapidity;#eta_{track}",80,-6,6);  
 TH1F *recoil_fracEnergy_acceptance = new TH1F("recoil_fracEnergy_acceptance","Energy fraction of recoil particle within acceptance (|#eta|<1)",80,0,0.5);   
 TH2F *frac_ptacceptance_vs_ptrecoil = new TH2F("frac_ptacceptance_vs_ptrecoil","relative Pt correction;Pt_{recoil};[Pt(acc)-Pt(true)]/Pt(true)",50,0,10,50,-5,15);    
 TH2F *frac_ptacceptance_vs_ptelectron = new TH2F("frac_ptacceptance_vs_ptelectron","relative Pt correction;Pt_{ele}",10,0,60,10,-5,15); 
 TH2F *deltaangle_vs_ptrecoil = new TH2F("deltaangle_vs_ptrecoil",";Pt_{recoil} (rad)",40,0,60,40,0,TMath::Pi()); 
 TH2F *deltaangle_vs_ptelectron = new TH2F("deltaangle_vs_ptelectron",";Pt_{ele} (rad)",40,0,60,40,0,TMath::Pi()); 

// Fill histograms
TString etaw("-log(tan(wEvent.WReco.Theta()/2.))");
TString etal("-log(tan(wEvent.l.Theta()/2.))"); 
TString etanu("-log(tan(wEvent.nu.Theta()/2.))"); 
//TString etal("0.5*log( (wEvent.l.E() + wEvent.l.Pz())/(wEvent.l.E() - wEvent.l.Pz()) )"); 
//TString etanu("0.5*log( (wEvent.nu.E() + wEvent.nu.Pz())/(wEvent.nu.E() - wEvent.nu.Pz()) )"); 

 wTree->Project("W_theta","wEvent.WReco.Theta()");  
 wTree->Project("lep_charge","wEvent.fLeptonSign","abs("+etal+")<1.");  
 wTree->Project("W_pseudorapidity",etaw);   
 wTree->Project("W_pseudorapidity2",etaw,"abs("+etal+")<1.");   
 wTree->Project("W_rapidity","wEvent.WReco.Rapidity()","abs("+etal+")<1.");  
 wTree->Project("Wm_rapidity","wEvent.WReco.Rapidity()","abs("+etal+")<1. && wEvent.fLeptonSign<0.");    
 wTree->Project("Wp_rapidity","wEvent.WReco.Rapidity()","abs("+etal+")<1. && wEvent.fLeptonSign>0.");   
 wTree->Project("W_mass","wEvent.WReco.M()");
 wTree->Project("l_angle","wEvent.l.Theta()");  
 wTree->Project("nu_angle","wEvent.nu.Theta()"); 
 wTree->Project("l_pseudorapidity",etal);  
 wTree->Project("nu_pseudorapidity",etanu); 
 pyTree->Project("recoil_tracks_pseudorapidity","tracks.eta","abs(tracks.id)!=11 && abs(tracks.id)!=12");   
 wTree->Project("recoil_fracEnergy_acceptance","wEvent.fEnergyRatio");
 wTree->Project("frac_ptacceptance_vs_ptrecoil","fPtCorr:wTree.wEvent.recoilInAccept.Pt()");  
 wTree->Project("frac_ptacceptance_vs_ptelectron","fPtCorr:wTree.wEvent.l.Pt()"); 
 wTree->Project("deltaangle_vs_ptrecoil","fPtCorrAngle:wTree.wEvent.recoilInAccept.Pt()"); 
 wTree->Project("deltaangle_vs_ptelectron","fPtCorrAngle:wTree.wEvent.l.Pt()"); 


// Plot histograms

// /*

 TCanvas *c2 = new TCanvas("c2","test_W",800,800);
c2->Divide(2,3);
 
c2_1->cd();
 W_mass->Draw();
 // c2_1->SetBottomMargin(0.15);
c2_2->cd();
 W_theta->Draw();
 // c1_2->SetBottomMargin(0.15);
c2_3->cd();
 W_pseudorapidity->Draw();
 W_pseudorapidity2->Draw("same");
 W_pseudorapidity2->SetLineColor(kRed);
 TLegend* leg_Wrapidity = new TLegend(0.6, 0.6, 0.4, 0.8);
   leg_Wrapidity->AddEntry(W_pseudorapidity,"no cuts");
   leg_Wrapidity->AddEntry(W_pseudorapidity2,"|#eta_{lep}|<1");
   leg_Wrapidity->Draw();
 // c1_3->SetBottomMargin(0.15);
c2_4->cd();
 TLegend* leg2_lepton_angle = new TLegend(0.5, 0.3, 0.3, 0.5);
 l_angle->Draw(); 
 nu_angle->Draw("same");
 nu_angle->SetLineColor(kRed);
   leg2_lepton_angle->AddEntry(l_angle,"electon");
   leg2_lepton_angle->AddEntry(nu_angle,"neutrino");
   leg2_lepton_angle->Draw();
 // c1_4->SetBottomMargin(0.15);
c2_5->cd(); 
l_pseudorapidity->Draw(); 
nu_pseudorapidity->Draw("same");
nu_pseudorapidity->SetLineColor(kRed);
 gPad->Update();
 //cout<<c2_5->GetFrame()->GetY2()<<endl;
 TLine *staraccl = new TLine(-1,0,-1,c2_5->GetFrame()->GetY2());
 TLine *staraccu = new TLine(1,0,1,c2_5->GetFrame()->GetY2());
staraccl->Draw(); 
staraccu->Draw(); 
c2_6->cd();
recoil_tracks_pseudorapidity->Draw();
 gPad->Update();
 TLine *staraccl = new TLine(-1,c2_6->GetFrame()->GetY1(),-1,c2_6->GetFrame()->GetY2());
 TLine *staraccu = new TLine(1,c2_6->GetFrame()->GetY1(),1,c2_6->GetFrame()->GetY2());
staraccl->Draw(); 
staraccu->Draw(); 

 c2->Print("./plots/W_rapidity.eps");

 TCanvas *c3 = new TCanvas("c3","test_W",800,800);

 recoil_fracEnergy_acceptance->Draw();
 c3->SetLogy(1); 
 c3->Print("./plots/recoil_energy_frac.eps");

 TCanvas *c4 = new TCanvas("c4","test_W",800,800);
 c4->Divide(2,2);

 c4_1->cd();
 frac_ptacceptance_vs_ptrecoil->ProfileX()->Draw();
 c4_2->cd();
 TH1* profX_frac_ptacceptance_vs_ptelectron = frac_ptacceptance_vs_ptelectron->ProfileX();
 //profX_frac_ptacceptance_vs_ptelectron->SetTitle(";;|#phi_{acc}-#phi_{rec}| (rad)");
 profX_frac_ptacceptance_vs_ptelectron->Draw();
 c4_3->cd();
 deltaangle_vs_ptrecoil->GetYaxis()->SetTitle("Y axis title");
 TH1* profX_deltaangle_vs_ptrecoil = deltaangle_vs_ptrecoil->ProfileX();
 profX_deltaangle_vs_ptrecoil->GetYaxis()->SetTitle("|#phi_{acc}-#phi_{rec}| (rad)");
 profX_deltaangle_vs_ptrecoil->Draw();
 c4_4->cd();
 TH1* profX_deltaangle_vs_ptelectron = deltaangle_vs_ptelectron->ProfileX();
 profX_deltaangle_vs_ptelectron->GetYaxis()->SetTitle("|#phi_{acc}-#phi_{rec}| (rad)");
 profX_deltaangle_vs_ptelectron->Draw();


 c4->Print("./plots/recoil_corrections.eps");
 
 //*/

 TCanvas *c5 = new TCanvas("c5","test_W",800,800);
 c5->Divide(2,2);

c5_1->cd();
 W_rapidity->Draw();
 //W_rapidity->GetYaxis()->SetTitle("Y axis title");
c5_2->cd();
 lep_charge->Draw();
c5_3->cd();
 Wm_rapidity->Draw();
c5_4->cd();
 Wp_rapidity->Draw();

 c5->Print("./plots/Wpm_rap.eps");

}
