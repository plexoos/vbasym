#ifndef VecBosEventInfo_h
#define VecBosEventInfo_h


#include "Globals.h"


// W-reco event container
// info about BEMC
class WeveBEMC
{
public:
   // raw BTOW/BPRS hits
   int   tileIn[mxBTile];           //! 0 if no data
   float adcTile[mxBTile][mxBtow];  //!
   float eneTile[mxBTile][mxBtow];
   int   statTile[mxBTile][mxBtow]; //!
   float maxAdc;                    //!
   int   maxHtDsm;                  //!

   //raw BSMD hits, both planes
   float adcBsmd[mxBSmd][mxBStrips];  //!
   int   statBsmd[mxBSmd][mxBStrips]; //!

   void clear();
   void print(int flag = 0);

   ClassDef(WeveBEMC, 2);
};


class WeveETOW   // info about ETOW
{
public:
   //raw ETOW hit
   int   etowIn;
   float adc[mxEtowSec*mxEtowSub][mxEtowEta]; //[phibin][etabin]
   float ene[mxEtowSec*mxEtowSub][mxEtowEta];
   int   stat[mxEtowSec*mxEtowSub][mxEtowEta];
   float maxAdc;
   int   maxSec, maxSub, maxEta;
   int   maxHtDsm;

   void clear() {
      memset(adc, 0, sizeof(adc));
      memset(ene, 0, sizeof(ene));
      memset(stat, -1, sizeof(stat)); // default all dead
      maxAdc = 0;
      maxSec = maxSub = maxEta = 0;
      maxHtDsm = -1;
   }

   ClassDef(WeveETOW, 2);
};


class WeveEPRS   // info about EPRS
{
public:
   //raw EPRS hit
   int   eprsIn;
   float adc[mxEtowSec*mxEtowSub][mxEtowEta][mxPrs]; //[phibin][etabin][layer]
   float ene[mxEtowSec*mxEtowSub][mxEtowEta][mxPrs];
   int   stat[mxEtowSec*mxEtowSub][mxEtowEta][mxPrs];

   void clear() {
      memset(adc, 0, sizeof(adc));
      memset(ene, 0, sizeof(ene));
      memset(stat, -1, sizeof(stat)); // default all dead
   }

   ClassDef(WeveEPRS, 1);
};


class WeveESMD   // info about ESMD
{
public:
   //raw ESMD hit
   int   esmdIn;
   float adc[mxEtowSec][mxEsmdPlane][mxEsmdStrip]; //[phibin][etabin]
   float ene[mxEtowSec][mxEsmdPlane][mxEsmdStrip];
   int   stat[mxEtowSec][mxEsmdPlane][mxEsmdStrip];

   void clear() {
      memset(adc, 0, sizeof(adc));
      memset(ene, 0, sizeof(ene));
      memset(stat, -1, sizeof(stat)); // default all dead
   }

   ClassDef(WeveESMD, 1);
};

#endif
