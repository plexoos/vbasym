//------- W-event Display container
#ifndef W_EVENT_Diplay_HH
#define W_EVENT_Display_HH

#include "TBox.h"

#include "WanaConst.h"
#include "WEventVertex.h"


class TH2F;
class TH1F;
class StVecBosMaker;
class TLine;


class WeventDisplay
{
public:
   int           maxEve;
   TH2F         *hTpcET;
   TH2F         *hEmcET;
   TH2F         *hBsmdAdc[mxBSmd];
   TH1F         *hEsmdShower[mxEsmdPlane];
   TH2F         *hEsmdXpt;
   TLine        *etaBL_ln;
   TLine        *etaBR_ln;                   // barrel
   TLine        *etaEL_ln;
   TLine        *etaER_ln;                   // endcap
   TBox         *bxT;
   TBox         *bxE;
   TBox         *bxS;                        // tpc;
   TBox          emc;
   TBox          bsmd;
   StVecBosMaker *wMK;

   WeventDisplay( StVecBosMaker *mk, int mxEv);
   void clear();
   void exportEvent(  const char *tit, WEventVertex myV, WeveEleTrack myTr, int vertexIndex);
   void export2sketchup(  const char *tit, WEventVertex myV, WeveEleTrack myTr);
   void getPrimTracks(  int vertID, int pointTowId);
   void getPrimTracksFromTree(  int vertID, int pointTowId);
   void draw( const char *tit, int eveID, int daqSeq, int runNo,  WEventVertex myV, WeveEleTrack myTr);

};

#endif
