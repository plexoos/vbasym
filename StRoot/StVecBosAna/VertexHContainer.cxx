/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "VertexHContainer.h"

#include "TF1.h"

#include "VecBosEvent.h"


ClassImp(VertexHContainer)

using namespace std;


/** Default constructor. */
VertexHContainer::VertexHContainer() : PlotHelper()
{
   BookHists();
}


VertexHContainer::VertexHContainer(TDirectory *dir) : PlotHelper(dir)
{
   BookHists();
}


/** Default destructor. */
VertexHContainer::~VertexHContainer()
{
}


/** */
void VertexHContainer::BookHists()
{
   string shName;
   TH1*   hist;

   fDir->cd();

   o["hVertexId"] = hist = new TH1I("hVertexId", "; Vertex Id; Num. of Vertices", 10, 0, 10);
   hist->SetOption("hist GRIDX GRIDY");

   o["hVertexZPosition"] = hist = new TH1I("hVertexZPosition", "; Z Position, cm; Num. of Vertices", 120, 0, 120);
   hist->SetOption("hist GRIDX");

   o["hVertexRank"] = hist = new TH1I("hVertexRank", "; Vertex Rank; Num. of Vertices", 50, -10e7, 10e7);
   hist->SetOption("hist GRIDX GRIDY");

   o["hVertexRankLog"] = hist = new TH1I("hVertexRankLog", "; Vertex LogRank; Num. of Vertices", 50, -100, 100);
   hist->SetOption("hist GRIDX GRIDY");
}


/** */
void VertexHContainer::Fill(ProtoEvent &ev)
{
   VecBosEvent& event = (VecBosEvent&) ev;

   VecBosVertexVecIter iVertex = event.mVertices.begin();

   for ( ; iVertex!=event.mVertices.end(); ++iVertex)
   {
      ((TH1*) o["hVertexId"])->Fill(iVertex->id);
      ((TH1*) o["hVertexZPosition"])->Fill(iVertex->z);
      ((TH1*) o["hVertexRank"])->Fill(iVertex->mRank);
      ((TH1*) o["hVertexRankLog"])->Fill(iVertex->mRankLog);
   }
}


/** */
void VertexHContainer::FillDerived()
{
   Info("FillDerived()", "Called");
}


/** */
void VertexHContainer::PostFill()
{
   Info("PostFill", "Called");
}
