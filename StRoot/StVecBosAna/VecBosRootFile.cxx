
#include "VecBosRootFile.h"

#include <climits>
#include <sstream>

#include "TROOT.h"

#include "EventHContainer.h"
#include "JetHContainer.h"
#include "TrackHContainer.h"
#include "VertexHContainer.h"
#include "KinemaHContainer.h"
#include "VecBosEvent.h"
#include "VecBosTrack.h"
#include "VecBosVertex.h"


ClassImp(VecBosRootFile)

using namespace std;


VecBosRootFile::VecBosRootFile() : TFile(),
   fHists(0), fHistCuts(),
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
   fHists(0), fHistCuts(),
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
   PlotHelper *ph;

   fHists = new PlotHelper(this);

   fHists->d["event"]     = ph = new EventHContainer(new TDirectoryFile("event", "event", "", this));
   fHistCuts[kCUT_NOCUT].insert(ph);

   fHists->d["event_cut"] = ph = new EventHContainer(new TDirectoryFile("event_cut", "event_cut", "", this));
   fHistCuts[kCUT_CUT].insert(ph);

   fHists->d["jets"] = ph = new JetHContainer(new TDirectoryFile("jets", "jets", "", this));
   fHistCuts[kCUT_NOCUT].insert(ph);

   fHists->d["vertices"] = ph = new VertexHContainer(new TDirectoryFile("vertices", "vertices", "", this));
   fHistCuts[kCUT_NOCUT].insert(ph);

   fHists->d["vertices_good"] = ph = new VertexHContainer(new TDirectoryFile("vertices_good", "vertices_good", "", this));
   //fHistCuts[kCUT_VERTICES_GOOD].insert(ph);

   fHists->d["tracks"] = ph = new TrackHContainer(new TDirectoryFile("tracks", "tracks", "", this));
   fHistCuts[kCUT_NOCUT].insert(ph);

   fHists->d["tracks_good"] = ph = new TrackHContainer(new TDirectoryFile("tracks_good", "tracks_good", "", this));
   //fHistCuts[kCUT_TRACKS_GOOD].insert(ph);

   fHists->d["tracks_barrel"] = ph = new TrackHContainer(new TDirectoryFile("tracks_barrel", "tracks_barrel", "", this));
   //fHistCuts[kCUT_BARREL].insert(ph);

   fHists->d["tracks_endcap"] = ph = new TrackHContainer(new TDirectoryFile("tracks_endcap", "tracks_endcap", "", this));
   //fHistCuts[kCUT_ENDCAP].insert(ph);

   //fHists->d["kinema"]    = ph = new KinemaHContainer(new TDirectoryFile("kinema", "kinema", "", this));

   this->cd();
}


PlotHelper* VecBosRootFile::GetHists() { return fHists; }
void VecBosRootFile::SetHists(PlotHelper &hists) { fHists = &hists; }


/** */
void VecBosRootFile::Fill(ProtoEvent &ev)
{
   // Fill hists on event basis
   //fHists->Fill(ev);
   Fill(ev, kCUT_NOCUT);

   VecBosEvent& event = (VecBosEvent&) ev;

   // Save only good vertices
   VecBosVertexVecIter iVertex = event.mVertices.begin();

   for ( ; iVertex!=event.mVertices.end(); ++iVertex)
   {
      if ( !iVertex->IsGood() ) continue;
      ((VertexHContainer*) fHists->d["vertices_good"])->Fill(*iVertex);
   }

   // Save only good tracks
   VecBosTrackVecIter iTrack = event.mTracks.begin();

   for ( ; iTrack!=event.mTracks.end(); ++iTrack)
   {
      if ( !iTrack->IsGood() ) continue;
      ((TrackHContainer*) fHists->d["tracks_good"])->Fill(*iTrack);

      //if ( !iTrack->HasBarrelMatched() ) {
      //   ((TrackHContainer*) fHists->d["tracks_barrel"])->Fill(*iTrack);
      //}

      //if ( !iTrack->HasEndcapMatched() ) {
      //   ((TrackHContainer*) fHists->d["tracks_endcap"])->Fill(*iTrack);
      //}
   }
}


/** */
void VecBosRootFile::Fill(ProtoEvent &ev, ECut cut)
{
   PlotHelperSet     hists = fHistCuts[cut];
   PlotHelperSetIter hi    = hists.begin();

   for ( ; hi!=hists.end(); ++hi) {
      (*hi)->Fill(ev);
   }
}


/** */
//void VecBosRootFile::SetAnaGlobResult(AnaGlobResult *agr)
//{
//   //fAnaGlobResult = agr;
//}


/** */
void VecBosRootFile::SaveAs(string pattern, string dir)
{
   TCanvas canvas("canvas", "canvas", 1000, 600);
   canvas.UseCurrentStyle();

   stringstream ssSignature("signature not defined");

   char strAnaTime[25];
   time_t currentTime = time(0);
   tm *ltime = localtime( &currentTime );
   strftime(strAnaTime, 25, "%c", ltime);

   //ssSignature << "Fills " << fMinFill << "--" << fMaxFill << ", Analyzed " << strAnaTime;
   //ssSignature << ", Version " << fAnaInfo->fAsymVersion << ", " << fAnaInfo->fUserGroup.fUser;

   fHists->SetSignature(ssSignature.str());
   fHists->SaveAllAs(canvas, pattern, dir);
}


/** */
//void VecBosRootFile::UpdMinMax(EventConfig &mm)
//{
//   UpdMinMaxFill((UInt_t) mm.fMeasInfo->RUNID);
//   UpdMinMaxTime(mm.fMeasInfo->fStartTime);
//}


/** */
void VecBosRootFile::UpdMinMaxFill(UInt_t fillId)
{
   if (fillId < fMinFill ) fMinFill = fillId;
   if (fillId > fMaxFill ) fMaxFill = fillId;
}


/** */
void VecBosRootFile::UpdMinMaxTime(time_t time)
{
   if (time < fMinTime ) fMinTime = time;
   if (time > fMaxTime ) fMaxTime = time;
}


/** */
void VecBosRootFile::Print(const Option_t* opt) const
{
   Info("Print", "Called");
   //PrintAsPhp(fFilePhp);
}


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
   Info("Close", "%s", GetName());
   TFile::Close(option);
}
