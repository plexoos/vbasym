#include "WEvent.h"


ClassImp(WEvent)

using namespace std;


WEvent::WEvent() : ProtoEvent(),
   mVertices()
{
   clear();
}


UInt_t WEvent::GetNumVertices()
{
   return mVertices.size();
}


UInt_t WEvent::GetNumTracks()
{
   UInt_t nTracks = 0;
   VBVertexVecIter iVertex = mVertices.begin();

   for ( ; iVertex!=mVertices.end(); ++iVertex)
      nTracks += iVertex->prTrList.size();

   return nTracks;
}


UInt_t WEvent::GetNumTracksWithBCluster()
{
   UInt_t nTracks = 0;
   VBVertexVecIter iVertex = mVertices.begin();

   for ( ; iVertex!=mVertices.end(); ++iVertex)
   {
      //VBTrackVecIter iTrack = iVertex->eleTrack.begin();
      vector<WeveEleTrack>::iterator iTrack = iVertex->eleTrack.begin();

      for ( ; iTrack!=iVertex->eleTrack.end(); ++iTrack)
      {
         if (iTrack->isMatch2Cl) nTracks++;
      }
   }

   return nTracks;
}


void WEvent::clear()
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
   mVertices.clear();
   bemc.clear();
   etow.clear();
   eprs.clear();
   esmd.clear();
}


void WEvent::print(int flag, int isMC)
{
   Info("print", "");

   printf("\nmy W2011event runNo=%d ID=%d  L2Wbits: ET=%d rnd=%d;  muDst: bx7=%d bx48=%d nVert=%d star: Bx7m=%d, Bx48=%d, spin4=%d \n", runNo, id, l2bitET, l2bitRnd, bx7, bx48, mVertices.size(), bxStar7, bxStar48, spin4);
   int  yyyymmdd,  hhmmss; getGmt_day_hour( yyyymmdd,  hhmmss);
   printf("  event time is: day=%d, hour=%d (GMT)\n", yyyymmdd, hhmmss);

   for (uint i = 0; i < mVertices.size(); i++) mVertices[i].print(flag);
   bemc.print(flag);
}


void WEvent::getGmt_day_hour(int &yyyymmdd, int &hhmmss)
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
