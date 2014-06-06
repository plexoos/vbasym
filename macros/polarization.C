void polarization()
{

   TString outPath    = "~/vbasym_results/plots/4prelim";

// Plot the polarization average for yellow and blue beams in 2011 transverse fills

 TGraphErrors *gPolBlue = new TGraphErrors("./curves/polarization-by-fill.txt","%lg %*lg %*lg %lg %lg");
 gPolBlue -> SetTitle("Yellow; fill; <P>"); 
 gPolBlue -> SetMarkerStyle(20); 
 gPolBlue -> SetMarkerColor(kBlue); 

 TGraphErrors *gPolYellow = new TGraphErrors("./curves/polarization-by-fill.txt","%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg %lg");
 gPolYellow -> SetTitle("Yellow; fill; <P>"); 
 gPolYellow -> SetMarkerStyle(20);
 gPolYellow -> SetMarkerColor(kYellow); 

 TCanvas *cPol = new TCanvas("cPol", "", 600, 600);
 cPol -> Divide(1,2);
 
 cPol_1 -> cd();
 gPolBlue -> Draw("AP");
 gPolBlue -> Fit("pol0");

 cPol_2 -> cd();
 gPolYellow -> Draw("AP");
 gPolYellow -> Fit("pol0");

 cPol -> Print(outPath + "/plot_polarization.png");  
 cPol -> Print(outPath + "/plot_polarization.eps");  

}
