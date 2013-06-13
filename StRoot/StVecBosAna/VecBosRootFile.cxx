
#include "VecBosRootFile.h"

#include <climits>
#include <sstream>

#include "TROOT.h"
#include "TStyle.h"

#include "EventHContainer.h"
#include "JetHContainer.h"
#include "TrackHContainer.h"
#include "VertexHContainer.h"
#include "KinemaHContainer.h"
#include "MCHContainer.h"
#include "VecBosEvent.h"
#include "VecBosTrack.h"
#include "VecBosVertex.h"

#include "utils/utils.h"

ClassImp(VecBosRootFile)

using namespace std;


VecBosRootFile::VecBosRootFile() : TFile(),
   fHists(0), fHistCuts(),
   fMinFill(UINT_MAX), fMaxFill(0),
   fMinTime(UINT_MAX), fMaxTime(0),
   fIsMc(kFALSE)
{
   //gROOT->SetMacroPath("./:~/rootmacros/:");
   //gROOT->Macro("styles/style_vbana.C");

   BookHists();
}


VecBosRootFile::VecBosRootFile(const char *fname, Option_t *option, Bool_t isMc, const char *ftitle, Int_t compress) :
   TFile(fname, option, ftitle, compress),
   fHists(0), fHistCuts(),
   fMinFill(UINT_MAX), fMaxFill(0),
   fMinTime(UINT_MAX), fMaxTime(0),
   fIsMc(isMc)
{
   printf("Created ROOT file: %s\n", GetName());

   //gROOT->SetMacroPath("./:~/rootmacros/:");
   //gROOT->Macro("styles/style_vbana.C");

   BookHists();
}


VecBosRootFile::~VecBosRootFile()
{
   //if (fHists) { delete fHists; fHists = 0; }
}


void VecBosRootFile::BookHists()
{
   PlotHelper *ph;

   fHists = new PlotHelper(this);

   fHists->d["event"] = ph = new EventHContainer(new TDirectoryFile("event", "event", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["event_has_jetrecoil"] = ph = new EventHContainer(new TDirectoryFile("event_has_jetrecoil", "event_has_jetrecoil", "", this));
   fHistCuts[kCUT_EVENT_HAS_JETRECOIL].insert(ph);

   fHists->d["event_has_trackrecoil"] = ph = new EventHContainer(new TDirectoryFile("event_has_trackrecoil", "event_has_trackrecoil", "", this));
   fHistCuts[kCUT_EVENT_HAS_TRACKRECOIL].insert(ph);

   fHists->d["event_has_candidate"] = ph = new EventHContainer(new TDirectoryFile("event_has_candidate", "event_has_candidate", "", this));
   fHistCuts[kCUT_EVENT_HAS_CANDIDATE_TRACK].insert(ph);

   fHists->d["event_has_candidate_pt>15"] = ph = new EventHContainer(new TDirectoryFile("event_has_candidate_pt>15", "event_has_candidate_pt>15", "", this));
   fHistCuts[kCUT_EVENT_HAS_CANDIDATE_TRACK_PT15].insert(ph);

   fHists->d["event_pass_final_QEToPT_pt>15"] = ph = new EventHContainer(new TDirectoryFile("event_pass_final_QEToPT_pt>15", "event_pass_final_QEToPT_pt>15", "", this));
   fHistCuts[kCUT_EVENT_PASS_FINAL_QET_PT15].insert(ph);

   fHists->d["event_pass_final"] = ph = new EventHContainer(new TDirectoryFile("event_pass_final", "event_pass_final", "", this));
   fHistCuts[kCUT_EVENT_PASS_FINAL].insert(ph);

   fHists->d["event_pass_final_QEToPT"]     = ph = new EventHContainer(new TDirectoryFile("event_pass_final_QEToPT", "event_pass_final_QEToPT", "", this));
   fHistCuts[kCUT_EVENT_PASS_FINAL_QET].insert(ph);

   fHists->d["W+_event_pass_final_QEToPT"]     = ph = new EventHContainer(new TDirectoryFile("W+_event_pass_final_QEToPT", "W+_event_pass_final_QEToPT", "", this));
   fHistCuts[kCUT_POSITIVE_EVENT_PASS_FINAL_QET].insert(ph);

   fHists->d["W-_event_pass_final_QEToPT"]     = ph = new EventHContainer(new TDirectoryFile("W-_event_pass_final_QEToPT", "W-_event_pass_final_QEToPT", "", this));
   fHistCuts[kCUT_NEGATIVE_EVENT_PASS_FINAL_QET].insert(ph);

   fHists->d["event_pass_qcd_QEToPT"]     = ph = new EventHContainer(new TDirectoryFile("event_pass_qcd_QEToPT", "event_pass_qcd_QEToPT", "", this));
   fHistCuts[kCUT_EVENT_PASS_QCD_QET].insert(ph);

   fHists->d["event_pass_qcd_QEToPT_pt>15"]     = ph = new EventHContainer(new TDirectoryFile("event_pass_qcd_QEToPT_pt>15", "event_pass_qcd_QEToPT_pt>15", "", this));
   fHistCuts[kCUT_EVENT_PASS_QCD_QET_PT15].insert(ph);

   fHists->d["event_has_jetrecoil_pass_final"] = ph = new EventHContainer(new TDirectoryFile("event_has_jetrecoil_pass_final", "event_has_jetrecoil_pass_final", "", this));
   fHistCuts[kCUT_EVENT_PASS_FINAL].insert(ph);

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

   fHists->d["event_tracks_pass_final"] = ph = new TrackHContainer(new TDirectoryFile("event_tracks_pass_final", "event_tracks_pass_final", "", this));
   fHistCuts[kCUT_EVENT_PASS_FINAL].insert(ph);

   fHists->d["vertex"]           = ph = new VertexHContainer(new TDirectoryFile("vertex", "vertex", "", this));

   fHists->d["vertex_good"]      = ph = new VertexHContainer(new TDirectoryFile("vertex_good", "vertex_good", "", this));

   fHists->d["track"]            = ph = new TrackHContainer(new TDirectoryFile("track", "track", "", this));

   fHists->d["track_candidates"] = ph = new TrackHContainer(new TDirectoryFile("track_candidates", "track_candidates", "", this));

   fHists->d["track_cand_pass_final"] = ph = new TrackHContainer(new TDirectoryFile("track_cand_pass_final", "track_cand_pass_final", "", this));

   fHists->d["track_cand_pass_final_QEToPT"] = ph = new TrackHContainer(new TDirectoryFile("track_cand_pass_final_QEToPT", "track_cand_pass_final_QEToPT", "", this));

   fHists->d["track_cand_pass_final_QEToPT_Pt>15"] = ph = new TrackHContainer(new TDirectoryFile("track_cand_pass_final_QEToPT_Pt>15", "track_cand_pass_final_QEToPT_Pt>15", "", this));

   fHists->d["W+_track_cand_pass_final"] = ph = new TrackHContainer(new TDirectoryFile("W+_track_cand_pass_final", "W+_track_cand_pass_final", "", this));

   fHists->d["W-_track_cand_pass_final"] = ph = new TrackHContainer(new TDirectoryFile("W-_track_cand_pass_final", "W-_track_cand_pass_final", "", this));

   fHists->d["W+_track_cand_pass_final_QEToPT"] = ph = new TrackHContainer(new TDirectoryFile("W+_track_cand_pass_final_QEToPT", "W+_track_cand_pass_final_QEToPT", "", this));

   fHists->d["W-_track_cand_pass_final_QEToPT"] = ph = new TrackHContainer(new TDirectoryFile("W-_track_cand_pass_final_QEToPT", "W-_track_cand_pass_final_QEToPT", "", this));

   fHists->d["W+_track_cand_pass_final_QEToPT_Pt>15"] = ph = new TrackHContainer(new TDirectoryFile("W+_track_cand_pass_final_QEToPT_Pt>15", "W+_track_cand_pass_final_QEToPT_Pt>15", "", this));

   fHists->d["W-_track_cand_pass_final_QEToPT_Pt>15"] = ph = new TrackHContainer(new TDirectoryFile("W-_track_cand_pass_final_QEToPT_Pt>15", "W-_track_cand_pass_final_QEToPT_Pt>15", "", this));

   fHists->d["track_cand_pass_qcd_QEToPT"] = ph = new TrackHContainer(new TDirectoryFile("track_cand_pass_qcd_QEToPT", "track_cand_pass_qcd_QEToPT", "", this));

   fHists->d["track_cand_pass_qcd_QEToPT_Pt>15"] = ph = new TrackHContainer(new TDirectoryFile("track_cand_pass_qcd_QEToPT_Pt>15", "track_cand_pass_qcd_QEToPT_Pt>15", "", this));

   fHists->d["W+_track_cand_pass_qcd_QEToPT"] = ph = new TrackHContainer(new TDirectoryFile("W+_track_cand_pass_qcd_QEToPT", "W+_track_cand_pass_qcd_QEToPT", "", this));

   fHists->d["W+_track_cand_pass_qcd_QEToPT_Pt>15"] = ph = new TrackHContainer(new TDirectoryFile("W+_track_cand_pass_qcd_QEToPT_Pt>15", "W+_track_cand_pass_qcd_QEToPT_Pt>15", "", this));

   fHists->d["W-_track_cand_pass_qcd_QEToPT"] = ph = new TrackHContainer(new TDirectoryFile("W-_track_cand_pass_qcd_QEToPT", "W-_track_cand_pass_qcd_QEToPT", "", this));

   fHists->d["W-_track_cand_pass_qcd_QEToPT_Pt>15"] = ph = new TrackHContainer(new TDirectoryFile("W-_track_cand_pass_qcd_QEToPT_Pt>15", "W-_track_cand_pass_qcd_QEToPT_Pt>15", "", this));


   if (!fIsMc) return;

   fHists->d["event_mc"] = ph = new MCHContainer(new TDirectoryFile("event_mc", "event_mc", "", this));
   fHistCuts[kCUT_EVENT_NOCUT].insert(ph);

   fHists->d["event_mc_has_jetrecoil"] = ph = new MCHContainer(new TDirectoryFile("event_mc_has_jetrecoil", "event_mc_has_jetrecoil", "", this));
   fHistCuts[kCUT_EVENT_HAS_JETRECOIL].insert(ph);

   fHists->d["event_mc_has_trackrecoil"] = ph = new MCHContainer(new TDirectoryFile("event_mc_has_trackrecoil", "event_mc_has_trackrecoil", "", this));
   fHistCuts[kCUT_EVENT_HAS_TRACKRECOIL].insert(ph);

   fHists->d["event_mc_pass_final_QEToPT"] = ph = new MCHContainer(new TDirectoryFile("event_mc_pass_final_QEToPT", "event_mc_pass_final_QEToPT", "", this));
   fHistCuts[kCUT_EVENT_PASS_FINAL_QET].insert(ph);

   this->cd();
}


PlotHelper *VecBosRootFile::GetHists() { return fHists; }
void VecBosRootFile::SetHists(PlotHelper &hists) { fHists = &hists; }


/** */
void VecBosRootFile::Fill(ProtoEvent &ev)
{
   //VecBosEvent& event = (VecBosEvent&) ev;
   VecBosEvent *event = (VecBosEvent *) &ev;

   Fill(ev, kCUT_EVENT_NOCUT);

   if ( event->HasJetRecoil() )
   {
      Fill(ev, kCUT_EVENT_HAS_JETRECOIL);
      //((EventHContainer*) fHists->d["event_has_jetrecoil"])->Fill(ev);

      if ( event->PassCutFinal() )
         ((EventHContainer *) fHists->d["event_has_jetrecoil_pass_final"])->Fill(ev);
         //((JetHContainer*) fHists->d["event_jets_has_jetrecoil_pass_final"])->Fill(ev);
   }

   if ( event->mP3TrackRecoilTpcNeutrals.Mag() > 0)
      Fill(ev, kCUT_EVENT_HAS_TRACKRECOIL);

   if ( event->HasCandidateTrack() )
      Fill(ev, kCUT_EVENT_HAS_CANDIDATE_TRACK);

   if ( event->HasCandidateTrack() && event->mCandElecP3EScaled.Pt() > 15)
      Fill(ev, kCUT_EVENT_HAS_CANDIDATE_TRACK_PT15);

   if ( event->PassCutFinal() )
      Fill(ev, kCUT_EVENT_PASS_FINAL);


   // Fill vertex histos
   VecBosVertexPtrSetIter iVertex = event->mVertices.begin();
   for ( ; iVertex != event->mVertices.end(); ++iVertex) {
      VecBosVertex &vertex = **iVertex;

      ((VertexHContainer *) fHists->d["vertex"])->Fill(vertex);

      if ( !vertex.IsGood() ) continue;

      ((VertexHContainer *) fHists->d["vertex_good"])->Fill(vertex);
   }

   // Fill track histos
   VecBosTrackPtrSetConstIter iTrack = event->mTracks.begin();
   for ( ; iTrack != event->mTracks.end(); ++iTrack)
   {
      VecBosTrack &track = **iTrack;

      ((TrackHContainer *) fHists->d["track"])->Fill(track);

      if ( !track.IsCandidate() ) continue;

      ((TrackHContainer *) fHists->d["track_candidates"])->Fill(track);


      if ( event->mCandElecP3EScaled.Pt() > 15 && event->mPtBalanceCosPhiFromTracks > 18) // Pass final with Pt>15
      {
         if ( abs( (track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) >= 0.4 &&
              abs( (track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) <= 1.8 )
         {
             Fill(ev, kCUT_EVENT_PASS_FINAL_QET_PT15);

             ((TrackHContainer*) fHists->d["track_cand_pass_final_QEToPT_Pt>15"])->Fill(track); 

            if ( track.prMuTrack->charge() > 0.)
               ((TrackHContainer*) fHists->d["W+_track_cand_pass_final_QEToPT_Pt>15"])->Fill(track);
            if ( track.prMuTrack->charge() < 0.)   
               ((TrackHContainer*) fHists->d["W-_track_cand_pass_final_QEToPT_Pt>15"])->Fill(track); 
	 }
      }


      if ( event->mCandElecP3EScaled.Pt() > 15 && event->mPtBalanceCosPhiFromTracks <= 18) // Pass QCD with Pt>15
      {
         if ( abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) >= 0.4 &&
              abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) <= 1.8 )
         {
            Fill(ev, kCUT_EVENT_PASS_QCD_QET_PT15);

            ((TrackHContainer*) fHists->d["track_cand_pass_qcd_QEToPT_Pt>15"])->Fill(track); 

            if ( track.prMuTrack->charge() > 0.)
               ((TrackHContainer*) fHists->d["W+_track_cand_pass_qcd_QEToPT_Pt>15"])->Fill(track);
            if ( track.prMuTrack->charge() < 0.)   
               ((TrackHContainer*) fHists->d["W-_track_cand_pass_qcd_QEToPT_Pt>15"])->Fill(track); 
	 }
      }

      ////      if ( !event->PassCutsExceptedPtBal() && event->mCandElecP3EScaled.Pt() > 25 ) continue; // Pass Pt>25 GeV


      if ( event->mCandElecP3EScaled.Pt() > 25 && event->mPtBalanceCosPhiFromTracks <= 18 ) // select QCD events
      { 

         if ( abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) >= 0.4 &&
              abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) <= 1.8 )
         {
            Fill(ev, kCUT_EVENT_PASS_QCD_QET);

            ((TrackHContainer *) fHists->d["track_cand_pass_qcd_QEToPT"])->Fill(track);

            if ( track.prMuTrack->charge() > 0.)
               ((TrackHContainer*) fHists->d["W+_track_cand_pass_qcd_QEToPT"])->Fill(track);
            if ( track.prMuTrack->charge() < 0.) 
               ((TrackHContainer*) fHists->d["W-_track_cand_pass_qcd_QEToPT"])->Fill(track); 
   	 }
      } // end QCD events


      //     if ( !event->PassCutFinal() ) continue;

      if ( event->mCandElecP3EScaled.Pt() > 25 && event->mPtBalanceCosPhiFromTracks > 18 ) 
      { 


        ((TrackHContainer *) fHists->d["track_cand_pass_final"])->Fill(track);

        if ( track.prMuTrack->charge() > 0.)
           ((TrackHContainer *) fHists->d["W+_track_cand_pass_final"])->Fill(track);
        if ( track.prMuTrack->charge() < 0.)
           ((TrackHContainer *) fHists->d["W-_track_cand_pass_final"])->Fill(track);

        if ( abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) >= 0.4 &&
                abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) <= 1.8 )  //all cuts applied 
        {
            Fill(ev, kCUT_EVENT_PASS_FINAL_QET);

            ((TrackHContainer *) fHists->d["track_cand_pass_final_QEToPT"])->Fill(track);

            if ( track.prMuTrack->charge() > 0.)
	    {
	       Fill(ev, kCUT_POSITIVE_EVENT_PASS_FINAL_QET);
               ((TrackHContainer*) fHists->d["W+_track_cand_pass_final_QEToPT"])->Fill(track);
	    }
            if ( track.prMuTrack->charge() < 0.) 
	    { 
               Fill(ev, kCUT_NEGATIVE_EVENT_PASS_FINAL_QET);  
               ((TrackHContainer*) fHists->d["W-_track_cand_pass_final_QEToPT"])->Fill(track);
	    }
	} 
      }

	/*
      if ( event->mCandElecP3EScaled.Pt() > 25 && event->mPtBalanceCosPhiFromTracks > 18 ) 
      {

         if ( abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) >= 0.4 &&
                abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) <= 1.8 )  //all cuts applied 
         {
            Fill(ev, kCUT_EVENT_PASS_FINAL_QET);

            ((TrackHContainer *) fHists->d["track_cand_pass_final_QEToPT"])->Fill(track);

            if ( track.prMuTrack->charge() > 0.)
	    {
	       Fill(ev, kCUT_POSITIVE_EVENT_PASS_FINAL_QET);
               ((TrackHContainer*) fHists->d["W+_track_cand_pass_final_QEToPT"])->Fill(track);
	    }
            if ( track.prMuTrack->charge() < 0.) 
	    { 
               Fill(ev, kCUT_NEGATIVE_EVENT_PASS_FINAL_QET);  
               ((TrackHContainer*) fHists->d["W-_track_cand_pass_final_QEToPT"])->Fill(track);
	    }
	 } 
      }

     
      if ( event->mCandElecP3EScaled.Pt() > 25 && event->mPtBalanceCosPhiFromTracks > 18 ) 
      {

         if ( abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) >= 0.4 &&
                abs((track.prMuTrack->charge()*track.mCluster2x2.ET) / track.mP3AtDca.Pt()) <= 1.8 )  //all cuts applied 
         {

            if ( track.prMuTrack->charge() > 0.)
	       Fill(ev, kCUT_POSITIVE_EVENT_PASS_FINAL_QET);
            if ( track.prMuTrack->charge() < 0.) 
	       Fill(ev, kCUT_NEGATIVE_EVENT_PASS_FINAL_QET); 
	 } 
      }
      */

   } // end loop over tracks


   //// Save only good tracks
   //VecBosTrackPtrSetIter iTrack = event->mTracks.begin();
   //for ( ; iTrack!=event->mTracks.end(); ++iTrack)
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

   for ( ; hi != hists.end(); ++hi) {
      (*hi)->Fill(ev);
   }
}


/** */
void VecBosRootFile::FillDerived()
{
   Info("FillDerived", "Called");

   fHists->FillDerived();
}


/** */
void VecBosRootFile::PostFill()
{
   Info("PostFill", "Called");

   fHists->PostFill();
}



/** */
void VecBosRootFile::SaveAs(string pattern, string dir)
{
   TCanvas canvas("canvas", "canvas", 1000, 600);
   canvas.UseCurrentStyle();

   stringstream ssSignature("");

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
void VecBosRootFile::Print(const Option_t *opt) const
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
void VecBosRootFile::Close(Option_t *option)
{
   fHists->Write();
   Info("Close", "%s", GetName());
   TFile::Close(option);
}
