#include <Riostream.h>
#include <iostream.h>
#include <fstream>
#include <sstream>


void polarization()
{
  //fitPol();
  //readTables();
  calcPolLumiReweight();
}

void fitPol()
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
 

 // PLOT
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


void readTables()
{
 // This code is used to contract (by hand) tables of luminosity and polarization for each fill
 
 // Open Luminosity files:
 Float_t run, dummy, dummy2, fill, prescale;
 Float_t lumi;
 Float_t fillPol, bluePolAveFill, yellowPolAveFill;

 ifstream inFileLumi;
 inFileLumi.open(Form("%slumi-by-run-and-fill.txt","./curves/"));
 inFileLumi.ignore(10000,'\n'); //ignores the first 10k caracters on the first line - skips the line header contents!

 ifstream inFilePol;
 inFilePol.open(Form("%spolarization-by-fill.txt","./curves/"));
 inFilePol.ignore(10000,'\n');

 Int_t nlines    = 0;
 Int_t nFills    = 0;
 Int_t fillCurr = 0;
 Float_t fillLumiTot = 0;
 Float_t fillLumiTotPrevious = 0;
 Float_t fillPrevious = 0;
 Float_t lumiTot = 0;

 while (1) {
   inFileLumi >> run >> dummy >> dummy2 >> fill >> lumi >> prescale;
   if (!inFileLumi.good()) break;
   //if (nlines < 5) printf("tun=%8i, dummy=%8i, dummy2=%8i, fill=%8i, lumi=%8.15f, prescale=%8i\n", run, dummy, dummy2, fill, lumi, prescale);

   lumiTot += lumi;

   if (fill != fillCurr) {
     fillCurr = fill;
     fillLumiTot = lumi;

     if (fillPrevious > 0) {
       /*
       while (fillPol == fillPrevious) {
         inFilePol >> fillPol >> dummy >> dummy >> bluePolAveFill >> dummy >> dummy >> dummy >> dummy >> dummy >> yellowPolAveFill >> dummy >> dummy >> dummy >> dummy >> dummy;
         if (!inFilePol.good()) break;
         if (fillPol == fillPrevious)  printf("fill=%6i bluePolAve=%8f yellowPolAve=%8f\n", fillPol, bluePolAveFill, yellowPolAveFill);
       }
       */
       printf("fill=%6i fillLumi=%8.15f \n", fillPrevious, fillLumiTotPrevious);
       fillPrevious = fillCurr;
       fillLumiTotPrevious = fillLumiTot;
       nFills++;
     }

   } else {
     fillLumiTot += lumi;
     fillPrevious = fillCurr;
     fillLumiTotPrevious = fillLumiTot;
   } 

   //if (nlines < 50) printf("fillCurr=%8i, lumi=%8.15f, fillLumiTot=%8.15f, lumiTot=%8.15f\n", fillCurr, lumi, fillLumiTot, lumiTot);
   //printf("fillCurr=%8i, lumi=%8.15f, fillLumiTot=%8.15f, lumiTot=%8.15f\n", fillCurr, lumi, fillLumiTot, lumiTot);
   //if (nlines < 50) printf("fillCurr=%8i, fillLumiTot=%8.15f \n", fillCurr, fillLumiTot);

   nlines++;
 }
 nFills = nFills +1; //add the LAST fill of the file to the total number
 printf("fill=%6i fillLumi=%8.15f \n", fillPrevious, fillLumiTotPrevious); // prints the LAST fill and lumiTot of the file
 printf(" found %d points\n",nlines);
 printf(" found %d fills\n",nFills);
 printf(" total luminosity is %f pb-1 \n",lumiTot);

 inFileLumi.close();

 
 nlines = 0;
 while (1) {
   inFilePol >> fillPol >> dummy >> dummy >> bluePolAveFill >> dummy >> dummy >> dummy >> dummy >> dummy >> yellowPolAveFill >> dummy >> dummy >> dummy >> dummy >> dummy;
   if (!inFilePol.good()) break;
   printf("fill=%6i bluePolAve=%8f yellowPolAve=%8f\n", fillPol, bluePolAveFill, yellowPolAveFill);
   nlines++;
 }

 printf(" found %d fills in lumi file\n",nlines);


}


void calcPolLumiReweight()
{

 // Open tables of luminosity and beam-polarization per fill:
 ifstream inFileLumiPol;
 inFileLumiPol.open(Form("%srun11_transv_fill_lumi_pol.txt","./curves/"));
 inFileLumiPol.ignore(10000,'\n'); //ignores the first 10k caracters on the first line - skips the line header contents!

 Float_t fill, lumi;
 Float_t bluePolAveFill, yellowPolAveFill;
 Int_t nlines    = 0;
 Float_t numBlue = 0;
 Float_t numYellow = 0;
 Float_t lumiTot = 0;
 Float_t polAveBlue, polAveYellow;

 while (1) {
   inFileLumiPol >> fill >> lumi >> bluePolAveFill >> yellowPolAveFill;
   if (!inFileLumiPol.good()) break;
   printf("fill=%6i fillLumi=%8.15f bluePolAve=%8f yellowPolAve=%8f\n", fill, lumi, bluePolAveFill, yellowPolAveFill);
   nlines++;
   numBlue   += lumi * bluePolAveFill;   
   numYellow += lumi * yellowPolAveFill;  
   lumiTot   += lumi; 
   //printf("numBlue=%8f denBlue=%8f\n", numBlue, lumiTot);
 }

 polAveBlue   =  numBlue / lumiTot;
 polAveYellow =  numYellow / lumiTot;
 printf(" found %d lines\n",nlines);
 printf(" total luminosity is %f pb-1 \n",lumiTot);
 printf(" The average polarization (lumi-weightet) for the BLUE beam is= %f % \n",polAveBlue);
 printf(" The average polarization (lumi-weightet) for the YELLOW beam is= %f % \n",polAveYellow);

 inFileLumiPol.close();

}
