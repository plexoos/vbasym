
#include "VecBosRootFile.h"

#include <climits>
#include <sstream>

#include "TROOT.h"

#include "EventHContainer.h"
#include "JetHContainer.h"
#include "TrackHContainer.h"
#include "VertexHContainer.h"
#include "KinemaHContainer.h"
#include "MCHContainer.h"
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
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["event_has_jetrecoil"]     = ph = new EventHContainer(new TDirectoryFile("event_has_jetrecoil", "event_has_jetrecoil", "", this));
   fHistCuts[kCUT_EVENT_HAS_JETRECOIL].insert(ph);

   fHists->d["event_has_candidate"]     = ph = new EventHContainer(new TDirectoryFile("event_has_candidate", "event_has_candidate", "", this));
   fHistCuts[kCUT_EVENT_HAS_CANDIDATE_TRACK].insert(ph);

   fHists->d["event_jets"] = ph = new JetHContainer(new TDirectoryFile("event_jets", "event_jets", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["event_jets_has_jetrecoil"] = ph = new JetHContainer(new TDirectoryFile("event_jets_has_jetrecoil", "event_jets_has_jetrecoil", "", this));
   fHistCuts[kCUT_EVENT_HAS_JETRECOIL].insert(ph);

   fHists->d["event_vertices"] = ph = new VertexHContainer(new TDirectoryFile("event_vertices", "event_vertices", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["event_tracks"] = ph = new TrackHContainer(new TDirectoryFile("event_tracks", "event_tracks", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["event_tracks_has_candidate"] = ph = new TrackHContainer(new TDirectoryFile("event_tracks_has_candidate", "event_tracks_has_candidate", "", this));
   fHistCuts[kCUT_EVENT_HAS_CANDIDATE_TRACK].insert(ph);

   //fHists->d["event_mc"] = ph = new MCHContainer(new TDirectoryFile("event_mc", "event_mc", "", this));
   //fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   //fHists->d["event_mc_has_jetrecoil"] = ph = new MCHContainer(new TDirectoryFile("event_mc_has_jetrecoil", "event_mc_has_jetrecoil", "", this));
   //fHistCuts[kCUT_EVENT_HAS_JETRECOIL].insert(ph);

   fHists->d["vertex"]           = ph = new VertexHContainer(new TDirectoryFile("vertex", "vertex", "", this));
   fHists->d["vertex_good"]      = ph = new VertexHContainer(new TDirectoryFile("vertex_good", "vertex_good", "", this));
   fHists->d["track"]            = ph = new TrackHContainer(new TDirectoryFile("track", "track", "", this));
   fHists->d["track_candidates"] = ph = new TrackHContainer(new TDirectoryFile("track_candidates", "track_candidates", "", this));

   //fHists->d["tracks_good"] = ph = new TrackHContainer(new TDirectoryFile("tracks_good", "tracks_good", "", this));
   ////fHistCuts[kCUT_TRACKS_GOOD].insert(ph);

   //fHists->d["tracks_barrel"] = ph = new TrackHContainer(new TDirectoryFile("tracks_barrel", "tracks_barrel", "", this));
   ////fHistCuts[kCUT_BARREL].insert(ph);

   //fHists->d["tracks_endcap"] = ph = new TrackHContainer(new TDirectoryFile("tracks_endcap", "tracks_endcap", "", this));
   ////fHistCuts[kCUT_ENDCAP].insert(ph);

   //fHists->d["kinema"]    = ph = new KinemaHContainer(new TDirectoryFile("kinema", "kinema", "", this));

   this->cd();
}


PlotHelper* VecBosRootFile::GetHists() { return fHists; }
void VecBosRootFile::SetHists(PlotHelper &hists) { fHists = &hists; }


/** */
void VecBosRootFile::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;

   Fill(ev, kCUT_EVENT_NOCUT);

   if ( event.HasJetRecoil() )
      Fill(ev, kCUT_EVENT_HAS_JETRECOIL);

   if ( event.HasCandidateTrack() )
      Fill(ev, kCUT_EVENT_HAS_CANDIDATE_TRACK);

   // Fill vertex histos
   VecBosVertexPtrSetIter iVertex = event.mVertices.begin();
   for ( ; iVertex != event.mVertices.end(); ++iVertex)
   {
      VecBosVertex &vertex = **iVertex;

      ((VertexHContainer*) fHists->d["vertex"])->Fill(vertex);

      if ( !vertex.IsGood() ) continue;

      ((VertexHContainer*) fHists->d["vertex_good"])->Fill(vertex);
   }

   // Fill track histos
   VecBosTrackPtrSetConstIter iTrack = event.mTracks.begin();
   for ( ; iTrack != event.mTracks.end(); ++iTrack)
   {
      VecBosTrack track = **iTrack;

      ((TrackHContainer*) fHists->d["track"])->Fill(track);

      if ( !track.IsCandidate() ) continue;

      ((TrackHContainer*) fHists->d["track_candidates"])->Fill(track);
   }

   //// Save only good tracks
   //VecBosTrackPtrSetIter iTrack = event.mTracks.begin();
   //for ( ; iTrack!=event.mTracks.end(); ++iTrack)
   //{
   //   VecBosTrack &track = **iTrack;
   //   if ( !track.IsGood() ) continue;

   //   ((TrackHContainer*) fHists->d["tracks_good"])->Fill(track);

   //   if ( track.IsBTrack() ) {
   //      ((TrackHContainer*) fHists->d["tracks_barrel"])->Fill(track);
   //   }

   //   if ( track.IsETrack() ) {
   //      ((TrackHContainer*) fHists->d["tracks_endcap"])->Fill(track);
   //   }
   //}
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
