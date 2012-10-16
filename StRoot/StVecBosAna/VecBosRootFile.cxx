
#include "VecBosRootFile.h"

#include <climits>
#include <sstream>

#include "TROOT.h"

#include "KinemaHContainer.h"


ClassImp(VecBosRootFile)

using namespace std;


VecBosRootFile::VecBosRootFile() : TFile(),
   fHists(0),
   fMinFill(UINT_MAX), fMaxFill(0),
   fMinTime(UINT_MAX), fMaxTime(0)
   //fFilePhp(0)
{
   gROOT->SetMacroPath("./:~/rootmacros/:");
   gROOT->Macro("styles/style_asym.C");

   BookHists();
}


VecBosRootFile::VecBosRootFile(const char* fname, Option_t* option, const char* ftitle, Int_t compress) :
   TFile(fname, option, ftitle, compress),
   fHists(0),
   fMinFill(UINT_MAX), fMaxFill(0),
   fMinTime(UINT_MAX), fMaxTime(0)
   //fFilePhp(anaInfo.GetAnaInfoFile())
{
   printf("Created ROOT file: %s\n", GetName());

   gROOT->SetMacroPath("./:~/rootmacros/:");
   gROOT->Macro("styles/style_asym.C");

   BookHists();
}


VecBosRootFile::~VecBosRootFile()
{
   if (!fHists) { delete fHists; fHists = 0; }
}


void VecBosRootFile::BookHists()
{
   fHists = new PlotHelper(this);

   fHists->d["kinema"]      = new KinemaHContainer(new TDirectoryFile("kinema", "kinema", "", this));
   fHists->d["kinema_iso"]  = new KinemaHContainer(new TDirectoryFile("kinema_iso",  "kinema_iso",  "", this));

   this->cd();
}

PlotHelper* VecBosRootFile::GetHists() { return fHists; }
void VecBosRootFile::SetHists(PlotHelper &hists) { fHists = &hists; }


/** */
//void VecBosRootFile::SetAnaGlobResult(AnaGlobResult *agr)
//{
//   //fAnaGlobResult = agr;
//}


/** */
void VecBosRootFile::SaveAs(string pattern, string dir)
{ //{{{

   TCanvas canvas("canvas", "canvas", 1400, 600);
   canvas.UseCurrentStyle();

   stringstream ssSignature("signature not defined");

   char strAnaTime[25];
   time_t currentTime = time(0);
   tm *ltime = localtime( &currentTime );
   strftime(strAnaTime, 25, "%c", ltime);

   ssSignature << "Fills " << fMinFill << "--" << fMaxFill << ", Analyzed " << strAnaTime;
   //ssSignature << ", Version " << fAnaInfo->fAsymVersion << ", " << fAnaInfo->fUserGroup.fUser;

   fHists->SetSignature(ssSignature.str());
   fHists->SaveAllAs(canvas, pattern, dir);
} //}}}


/** */
//void VecBosRootFile::UpdMinMax(EventConfig &mm)
//{ //{{{
//   UpdMinMaxFill((UInt_t) mm.fMeasInfo->RUNID);
//   UpdMinMaxTime(mm.fMeasInfo->fStartTime);
//} //}}}


/** */
void VecBosRootFile::UpdMinMaxFill(UInt_t fillId)
{ //{{{
   if (fillId < fMinFill ) fMinFill = fillId;
   if (fillId > fMaxFill ) fMaxFill = fillId;
} //}}}


/** */
void VecBosRootFile::UpdMinMaxTime(time_t time)
{ //{{{
   if (time < fMinTime ) fMinTime = time;
   if (time > fMaxTime ) fMaxTime = time;
} //}}}


/** */
void VecBosRootFile::Print(const Option_t* opt) const
{ //{{{
   Info("Print", "Called");
   //PrintAsPhp(fFilePhp);
} //}}}


/** */
void VecBosRootFile::PrintAsPhp(FILE *f) const
{
   fprintf(f, "<?php\n");

   fprintf(f, "\n// AnaGlobResult data\n");
   //if (!fAnaGlobResult) {
   //   Error("PrintAsPhp", "fAnaGlobResult not defined");
   //} else {
   //   fAnaGlobResult->PrintAsPhp(f);
   //}

   fprintf(f, "?>\n");
}


/** */
void VecBosRootFile::Close(Option_t* option)
{
   fHists->Write();
   TFile::Close(option);
}
