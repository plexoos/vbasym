#ifndef CalTower_h
#define CalTower_h


#include "TObject.h"
#include "TVector3.h"


/**
 * Matches calorimeter tower index to a track pointing to the tower.
 */
class CalTower : public TObject
{
public:
   TVector3 R;       // extrapolated position of primary track
   TVector3 Rglob;   // extrapolated position of global track
   int      id;      // BTOW tower id (!= 0), not used for ETOW
   int      iEta;
   int      iPhi;    // eta x phi bin using L2 indexing convention

   UShort_t GetEtaBin() { return iEta; }
   UShort_t GetPhiBin() { return iPhi; }
   virtual void Clear(const Option_t* opt="");
   virtual void Print(const Option_t* opt="") const;

   ClassDef(CalTower, 1);
};

#endif
