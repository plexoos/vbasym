
#include "VecBosJet.h"

#include "VecBosEvent.h"


VecBosJet::VecBosJet() : StJet(), mEvent(0), mVertexId(-1), mMinZDistToVertex(-1)
{
}


VecBosJet::VecBosJet(StJet &stJet) :
   StJet(stJet.E(), stJet.Px(), stJet.Py(), stJet.Pz(), stJet.nCell, stJet.charge),
   mEvent(0), mVertexId(-1), mMinZDistToVertex(-1)
{
  nTracks   = stJet.nTracks;
  nBtowers  = stJet.nBtowers;
  nEtowers  = stJet.nEtowers;
  tpcEtSum  = stJet.tpcEtSum;
  btowEtSum = stJet.btowEtSum;
  etowEtSum = stJet.etowEtSum;
  zVertex   = stJet.zVertex;
  jetEt     = Et();
  jetPt     = Pt();
  jetEta    = Eta();
  jetPhi    = Phi();
}


VecBosJet::~VecBosJet()
{
   //Info("~VecBosJet()", "this: %x", this);
}


/** */
void VecBosJet::Process()
{
   if (!mEvent) return;

   // The only info about jet vertex is its z position. Use it to match vertex to jet
   VecBosVertex *closestVertex = 0;

   VecBosVertexPtrSetIter iVertex = mEvent->mVertices.begin();
   for ( ; iVertex != mEvent->mVertices.end(); ++iVertex)
   {
      VecBosVertex &vertex = **iVertex;
      Double_t deltaZ = fabs(zVertex - vertex.mPosition.Z());

      if (mMinZDistToVertex < 0 || deltaZ < mMinZDistToVertex) {
         mMinZDistToVertex = deltaZ;
         closestVertex = *iVertex;
      }
   }

   if (closestVertex && mMinZDistToVertex < VecBosEvent::sMaxVertexJetDeltaZ) {
      SetVertexId(closestVertex->GetId());
   }
}


void VecBosJet::Print(const Option_t* opt) const
{
   Info("Print(const Option_t* opt)", "Jet mVertexId=%d, zVertex=%.1f cm, nTracks=%d\n", mVertexId, zVertex, nTracks);
}
