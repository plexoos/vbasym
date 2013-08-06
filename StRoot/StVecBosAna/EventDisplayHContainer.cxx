/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "EventDisplayHContainer.h"

#include "TF1.h"
#include "TF2.h"

#include "VecBosEvent.h"
#include "EventDisplayHists.h"
#include "TrackHContainer.h"


ClassImp(EventDisplayHContainer)

using namespace std;


/** Default constructor. */
EventDisplayHContainer::EventDisplayHContainer() : PlotHelper()
{
   BookHists();
}


EventDisplayHContainer::EventDisplayHContainer(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


/** Default destructor. */
EventDisplayHContainer::~EventDisplayHContainer()
{
}


/** */
void EventDisplayHContainer::BookHists()
{
   string shName;
   TH1*   hist;

   fDir->cd();
}


/** */
void EventDisplayHContainer::Fill(ProtoEvent &ev)
{
   PlotHelper *ph_sub = GetEventDisplayHists(ev);

   if (ph_sub)
      ph_sub->Fill(ev);
}


/** */
PlotHelper *EventDisplayHContainer::GetEventDisplayHists(const ProtoEvent &ev)
{
   VecBosEvent& vbEvent = (VecBosEvent&) ev;

   int runId   = vbEvent.GetRunId();
   int eventId = vbEvent.GetEventId();

   // Set individual sub dirs
   string dirName(17, ' ');
   sprintf(&dirName[0], "r%08d_%08d", runId, eventId);

   PlotHelper *ph;
   PlotHelperMapIter isubdir = d.find(dirName);

   if ( isubdir == d.end()) { // if dir not found create a new one
      TDirectoryFile *tdir = new TDirectoryFile(dirName.c_str(), dirName.c_str(), "", fDir);
      ph = new EventDisplayHists(tdir, vbEvent);
      d[dirName] = ph;
   } else {
      ph = isubdir->second;
   }

   return ph;
}
