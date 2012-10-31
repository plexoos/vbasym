#include "VecBosEvent.h"


ClassImp(VecBosEvent)

using namespace std;


VecBosEvent::VecBosEvent() : ProtoEvent(),
   mVertices(),
   mTracks(),
   mLeptonBTracks(), mLeptonETracks()
{
   clear();
}


void VecBosEvent::AddTrack(UInt_t vertexId, StMuTrack* stMuTrack)
{
   mVertices[vertexId].prTrList.push_back(stMuTrack);

   StThreeVectorF prPvect = stMuTrack->p();

   VecBosTrack track;

   track.prMuTrack     = stMuTrack;
   track.glMuTrack     = stMuTrack->globalTrack();
   track.mVecBosVertex = &mVertices[vertexId];
   track.primP         = TVector3(prPvect.x(), prPvect.y(), prPvect.z());

   mTracks.push_back(track);
}


UInt_t VecBosEvent::GetNumVertices()
{
   return mVertices.size();
}


UInt_t VecBosEvent::GetNumTracks()
{
   UInt_t nTracks = 0;
   VecBosVertexVecIter iVertex = mVertices.begin();

   for ( ; iVertex!=mVertices.end(); ++iVertex)
      nTracks += iVertex->prTrList.size();

   return nTracks;
}


UInt_t VecBosEvent::GetNumTracksWithBCluster()
{
   UInt_t nTracks = 0;
   VecBosVertexVecIter iVertex = mVertices.begin();

   for ( ; iVertex!=mVertices.end(); ++iVertex)
   {
      VecBosTrackVecIter iTrack = iVertex->eleTrack.begin();

      for ( ; iTrack!=iVertex->eleTrack.end(); ++iTrack)
      {
         if (iTrack->isMatch2Cl) nTracks++;
      }
   }

   return nTracks;
}


UInt_t VecBosEvent::GetNumTracksWithBCluster2()
{
   return mLeptonBTracks.size();
}


/** Checks if at least one good vertex exist in the event. */
bool VecBosEvent::HasGoodVertex()
{
   VecBosVertexVecIter iVertex = mVertices.begin();

   for ( ; iVertex!=mVertices.end(); ++iVertex)
   {
      if ( iVertex->IsGood() ) return true;
   }

   return false;
}


Bool_t VecBosEvent::HasGoodTrack()
{
   VecBosTrackVecIter iTrack = mTracks.begin();

   for ( ; iTrack!=mTracks.end(); ++iTrack)
   {
      if (iTrack->IsGood() ) return true;
   }

   return false;

   //if (mVecBosEvent->l2bitET && rank > 0 && primaryTrack->flag() == 301)
      //XXX:ds:if (secID == 20) continue; //poorly calibrated sector for Run 9+11+12?
      //XXX:ds:if (mTpcFilter[secID - 1].accept(primaryTrack) == false) continue;

   //if (mVecBosEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)
      //XXX:ds:if ( mTpcFilterE[secID - 1].accept(primaryTrack) == false) continue;

   //XXX:ds:if (!barrelTrack && !endcapTrack) continue;
}


void VecBosEvent::clear()
{
   //Info("clear", "");
   id       = runNo     = time  = 0;
   zdcRate  = 0;
   l2bitET  = l2bitRnd  = 0;
   l2EbitET = l2EbitRnd = 0;
   bx7      = bx48      = -1;
   zTag     = false;
   mNJets   = 0;
   bxStar7  = bxStar48  = spin4 = -1;
   bemc.clear();
   etow.clear();
   eprs.clear();
   esmd.clear();
   mVertices.clear();
   mTracks.clear();
   mLeptonBTracks.clear();
   mLeptonETracks.clear();
}


void VecBosEvent::print(int flag, int isMC)
{
   Info("print", "");

   printf("\nmy W2011event runNo=%d ID=%d  L2Wbits: ET=%d rnd=%d;  muDst: bx7=%d bx48=%d nVert=%d star: Bx7m=%d, Bx48=%d, spin4=%d \n", runNo, id, l2bitET, l2bitRnd, bx7, bx48, mVertices.size(), bxStar7, bxStar48, spin4);
   int  yyyymmdd,  hhmmss; getGmt_day_hour( yyyymmdd,  hhmmss);
   printf("  event time is: day=%d, hour=%d (GMT)\n", yyyymmdd, hhmmss);

   for (uint i = 0; i < mVertices.size(); i++) mVertices[i].print(flag);
   bemc.print(flag);
}


void VecBosEvent::getGmt_day_hour(int &yyyymmdd, int &hhmmss)
{
   time_t rawtime = this->time;
   struct tm *timeinfo = gmtime ( &rawtime );
   char buffer [80];
   strftime (buffer, 80, "%k%M%S", timeinfo);
   //puts (buffer);
   hhmmss = atoi(buffer);
   strftime (buffer, 80, "%G%m%d", timeinfo);
   //puts (buffer);
   yyyymmdd = atoi(buffer);
   //printf("day=%d, hour=%d\n",yyyymmdd,hhmmss);
}
