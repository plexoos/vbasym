#ifndef VecBosJet_h
#define VecBosJet_h

#include <vector>
#include <set>

#include "TObject.h"

#include "StThreeVectorF.hh"
#include "StSpinPool/StJets/StJet.h"


class VecBosEvent;


// Info about vertex
class VecBosJet : public StJet
{
public:

   VecBosEvent  *mEvent;             //!
   Short_t       mVertexId;          // mId of the mother vertex
   Float_t       mMinZDistToVertex;  // distance to closest vertex along z

   VecBosJet();
   VecBosJet(StJet &stJet);
   ~VecBosJet();
  
   Short_t  GetVertexId() const { return mVertexId; }
   void     SetVertexId(Short_t vId) { mVertexId = vId; }
   void     Process();
   void     clear();
   virtual void Print(const Option_t* opt="") const;

   ClassDef(VecBosJet, 1);
};


typedef std::vector<VecBosJet>         VecBosJetVec;
typedef VecBosJetVec::iterator         VecBosJetVecIter;
typedef VecBosJetVec::const_iterator   VecBosJetVecConstIter;


inline bool operator==(const VecBosJet& lhs, const VecBosJet& rhs) { return (TLorentzVector) lhs == (TLorentzVector) rhs; }
inline bool operator!=(const VecBosJet& lhs, const VecBosJet& rhs) { return !operator==(lhs,rhs); }
inline bool operator< (const VecBosJet& lhs, const VecBosJet& rhs) { return lhs.E() < rhs.E(); }
inline bool operator> (const VecBosJet& lhs, const VecBosJet& rhs) { return  operator< (rhs,lhs); }
inline bool operator<=(const VecBosJet& lhs, const VecBosJet& rhs) { return !operator> (lhs,rhs); }
inline bool operator>=(const VecBosJet& lhs, const VecBosJet& rhs) { return !operator< (lhs,rhs); }

struct CompareVecBosJet
{
   bool operator()(const VecBosJet& lhs, const VecBosJet& rhs) const { return lhs > rhs; }
};

struct CompareVecBosJetPtr
{
   bool operator()(const VecBosJet* lhs, const VecBosJet* rhs) const { return (*lhs) > (*rhs); }
};

typedef std::set<VecBosJet, CompareVecBosJet>     VecBosJetSet;
typedef VecBosJetSet::iterator                    VecBosJetSetIter;
typedef VecBosJetSet::const_iterator              VecBosJetSetConstIter;

typedef std::set<VecBosJet*, CompareVecBosJetPtr> VecBosJetPtrSet;
typedef VecBosJetPtrSet::iterator                 VecBosJetPtrSetIter;
typedef VecBosJetPtrSet::const_iterator           VecBosJetPtrSetConstIter;

#endif

