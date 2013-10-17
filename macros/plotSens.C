#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH2C.h"
#include "TH2F.h"
#include "TF2.h"
#include "TImage.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TStyle.h"


void plotSens()
{
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetTextFont(62);
   gStyle->SetTitleFont(62, "XYZ");
   gStyle->SetTitleSize(0.12, "XYZ");
   gStyle->SetLabelFont(62, "XYZ");
   gStyle->SetLabelSize(0.12, "XYZ");
   
   gStyle->SetEndErrorSize(6);
   
   // W minus
   TImage *img = TImage::Open("curves_img/anapow_w_minus_orig.png");
   const UInt_t  nBinsW = 3;
   Float_t arrSensW_x[nBinsW]  = {-0.25, 0.25, 0.75};
   Float_t arrSensW_y[nBinsW]  = { 0, 0, 0};
   Float_t arrSensW_xe[nBinsW] = { 0.25, 0.25, 0.25};
   Float_t arrSensW_ye[nBinsW] = { 6, 6, 9};
   Float_t arrSensW_ye_neg[nBinsW] = { -6, -6, -9};
   
   // W plus
   //TImage *img = TImage::Open("curves_img/anapow_w_plus_orig.png");
   //const UInt_t nBinsW = 6;
   //Float_t arrSensW_x[nBinsW]  = {-1.25, -0.75, -0.25, 0.25, 0.75, 1.25};
   //Float_t arrSensW_y[nBinsW]  = { 0, 0, 0, 0, 0, 0};
   //Float_t arrSensW_xe[nBinsW] = { 0.25, 0.25, 0.25, 0.25, 0.25, 0.25};
   //Float_t arrSensW_ye[nBinsW] = { 7, 4, 3.6, 3.6, 4, 7};
   //Float_t arrSensW_ye_neg[nBinsW] = { -7, -4, -3.6, -3.6, -4, -7};
   
   // Z
   //TImage *img = TImage::Open("curves_img/anapow_z_orig.png");
   //const UInt_t  nBinsW = 1;
   //Float_t arrSensW_x[nBinsW]  = { 0};
   //Float_t arrSensW_y[nBinsW]  = { 0};
   //Float_t arrSensW_xe[nBinsW] = { 1.4};
   //Float_t arrSensW_ye[nBinsW] = { 10};
   //Float_t arrSensW_ye_neg[nBinsW] = { -10};
   
   TGraphErrors *grSensW      = new TGraphErrors(nBinsW, arrSensW_x, arrSensW_y, arrSensW_xe, arrSensW_ye);
   TGraph       *grSensW_fill = new TGraph(nBinsW*2);
   TGraph       *grSensW_up   = new TGraph(nBinsW, arrSensW_x, arrSensW_ye);
   TGraph       *grSensW_lo   = new TGraph(nBinsW, arrSensW_x, arrSensW_ye);
   
   grSensW_up->Apply(new TF2("scale", "y*0.01"));
   grSensW_lo->Apply(new TF2("invert", "-1*y*0.01"));
   grSensW->Apply(new TF2("scale", "1.08*y*0.01")); // 8% correction is due to 0 < q_T < 3 GeV cut used for the theoretical predictions
   
   for (UInt_t i=0; i<nBinsW; i++) {
      grSensW_fill->SetPoint(i, arrSensW_x[i], arrSensW_ye[i]);
      grSensW_fill->SetPoint(nBinsW+i, arrSensW_x[nBinsW-i-1], arrSensW_ye_neg[nBinsW-i-1]);
   }
   
   
   TCanvas *canvas = new TCanvas("canvas", "canvas", 570, 800);
   //TCanvas *canvas = new TCanvas("canvas", "canvas", 620, 800);
   //canvas->SetFillColor(kRed);
   canvas->SetFrameBorderMode(1);
   canvas->SetFrameBorderSize(3);
   
   //canvas->Update();
   //TPad *pad1 = new TPad("pad1", "This is pad1", 0.00, 0.30, 0.71, 1);
   TPad *pad1 = new TPad("pad1", "This is pad1", 0.00, 0.30, 1, 1);
   //pad1->SetFillColor(kBlue);
   pad1->SetFrameBorderMode(0);
   pad1->SetFrameBorderSize(0);
   pad1->SetMargin(0, 0, 0, 0);
   
   pad1->Draw();
   
   pad1->cd();
   img->Draw();
   
   canvas->cd();
   
   //TPad *pad2 = new TPad("pad2", "This is pad2", 0.00, 0.00, 0.71, 0.30);
   TPad *pad2 = new TPad("pad2", "This is pad2", 0.00, 0.00, 1, 0.30);
   //pad2->SetFillColor(kGreen);
   //pad2->SetFrameBorderMode(1);
   //pad2->SetFrameBorderSize(3);
   pad2->SetTopMargin(0.02);
   pad2->SetLeftMargin(0.165);
   pad2->SetRightMargin(0.027);
   pad2->SetBottomMargin(0.27);
   pad2->Draw();
   
   //canvas->Divide(1, 2, 0.01, 0.01, 0);
   
   //canvas->cd(1);
   //img->Zoom(0, 0, 200, 200);
   //canvas->cd(2);
   
   pad2->cd();
   pad2->SetGridy();
   
   TH1 *dummy = new TH2F("dummy", "dummy", 1, -2, 2, 1, -0.1, 0.1);
   //TH1 *dummy = new TH2F("dummy", "dummy", 1, -1.5, 1.5, 1, -15, 15);
   dummy->UseCurrentStyle();
   dummy->SetTitle(" ;y_{W}; #Delta A_{N}; ");
   dummy->SetNdivisions(505, "Y");
   dummy->SetTitleOffset(0.7, "Y");
   dummy->Draw();
   
   
   //grSensW_fill->SetFillStyle(3013);
   //grSensW_fill->SetFillColor(16);
   //grSensW_fill->Draw("CF");
   
   //grSensW_up->Draw("c");
   //grSensW_lo->Draw("c");
   
   grSensW->SetLineWidth(3);
   grSensW->SetFillStyle(3013);
   grSensW->SetFillColor(16);
   grSensW->Draw("P");
   
   
   canvas->SaveAs("anapow_w_.png");
   //canvas->SaveAs("anapow_z_.png");
   //canvas->Modified();
   //canvas->Update();
}
