
#include "VecBosTrack.h"

#include "VecBosEvent.h"
#include "VecBosVertex.h"
#include "Globals.h"


ClassImp(VecBosTrack)

using namespace std;


VecBosTrack::VecBosTrack() : TObject(), mEvent(0), mType(kUNKNOWN), mVertex(0)
{
   clear();
}


bool VecBosTrack::IsGood()   const { return (mType & kGOOD)   == kGOOD   ? true : false; }
bool VecBosTrack::IsBTrack() const { return (mType & kBARREL) == kBARREL ? true : false; }
bool VecBosTrack::IsETrack() const { return (mType & kENDCAP) == kENDCAP ? true : false; }


void VecBosTrack::print(int flag)
{
   if (prMuTrack == 0) {  printf("   Track NULL pointer???\n"); return;}
   printf("   Track glPT=%.1f GeV/c   isMatch2Cl=%d, nearTotET=%.1f, awayTotET=%.1f primPT=%.1f\n",
          glMuTrack->pt(), isMatch2Cl, nearTotET, awayTotET, primP.Pt());
   pointTower.print(flag);
   mCluster2x2.print(flag);
   TVector3 D = pointTower.R - mCluster2x2.position;
   printf("                XYZ(track-mCluster2x2):  |3D dist|=%.1fcm  delZ=%.1fcm\n", D.Mag(), D.z());
   printf("     4x4 :"); mCluster4x4.print(flag);
   printf("     nearET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n", nearTpcPT, nearEmcET, nearBtowET, nearEtowET, nearTotET);
   printf("     awayET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n", awayTpcPT, awayEmcET, awayBtowET, awayEtowET, awayTotET);
}


void VecBosTrack::Process()
{
   // Good track must come from a good vertex
   if (!mVertex || !mVertex->IsGood()) {
      mType = kBAD;
      return;
   }

   if ( prMuTrack->pt() >= 1.0)
   {
      if ( prMuTrack->flag() == 301 )
         mType = kBARREL;
      else if ( prMuTrack->flag() == 311 )
         mType = kENDCAP;
      else
         mType = kBAD;
   } else
      mType = kBAD;

   //if (mVecBosEvent->l2bitET && rank > 0 && primaryTrack->flag() == 301)
      //XXX:ds:if (secID == 20) continue; //poorly calibrated sector for Run 9+11+12?
      //XXX:ds:if (mTpcFilter[secID - 1].accept(primaryTrack) == false) continue;

   //if (mVecBosEvent->l2EbitET && ro.pseudoRapidity() > parE_trackEtaMin)
      //XXX:ds:if ( mTpcFilterE[secID - 1].accept(primaryTrack) == false) continue;

   //XXX:ds:if (!barrelTrack && !endcapTrack) continue;

   if (IsBTrack()) MatchTrack2BtowCluster();
   //if (IsETrack()) ExtendTrack2Endcap();
}


void VecBosTrack::clear()
{
   mEvent               = 0;
   mType                = kUNKNOWN;
   isMatch2Cl           = false;
   pointTower.clear();
   glMuTrack            = 0;
   prMuTrack            = 0;
   mVertex              = 0;
   mCluster2x2.clear();
   mCluster4x4.clear();
   primP                = TVector3(0, 0, 0);
   awayTpcPT            = 0;
   nearTpcPT            = 0;
   nearTotET            = 0;
   awayTotET            = 0;
   nearEmcET            = 0;
   awayEmcET            = 0;
   nearBtowET           = 0;
   awayBtowET           = 0;
   nearEtowET           = 0;
   awayEtowET           = 0;
   smallNearTpcPT       = 0;
   nearTotET_noEEMC     = 0;
   awayTotET_noEEMC     = 0;
   ptBalance            = TVector3(0, 0, 0);
   ptBalance_noEEMC     = TVector3(0, 0, 0);
   sPtBalance           = 0;
   sPtBalance_noEEMC    = 0;
   hadronicRecoil       = TVector3(0, 0, 0);

   memset(esmdGlobStrip, -999, sizeof(esmdGlobStrip));
   memset(esmdDca, -999., sizeof(esmdDca));
   memset(esmdDcaGlob, -999., sizeof(esmdDcaGlob));
   memset(esmdE, 0., sizeof(esmdE));
   memset(esmdNhit, 0, sizeof(esmdNhit));
   memset(esmdShowerCentroid, 999., sizeof(esmdShowerCentroid));
   memset(esmdShowerWidth, 999., sizeof(esmdShowerWidth));

   esmdXPcentroid = TVector3(0, 0, 0);
}


TVector3 VecBosTrack::CalcDistanceToMatchedCluster()
{
   return pointTower.R - mCluster2x2.position;
}


/** */
void VecBosTrack::ExtendTrack2Barrel()
{
   //printf("******* extendTracks() nVert=%d\n", mVecBosEvent->mVertices.size());
   //if (!mVecBosEvent->l2bitET) return; //fire barrel trigger

   // Apply eta cuts at track level (tree analysis)
   //if (track.primP.Eta() < mMinBTrackEta || track.primP.Eta() > mMaxBTrackEta) continue;

   // extrapolate track to the barrel @ R=entrance
   const StPhysicalHelixD trkHelix  = prMuTrack->outerHelix();
   float                  Rcylinder = mBtowGeom->Radius();
   pairD                  d2        = trkHelix.pathLength(Rcylinder);
   //printf(" R=%.1f path 1=%f, 2=%f, period=%f, R=%f\n",Rctb,d2.first ,d2.second,trkHelix.period(),1./trkHelix.curvature());

   // assert(d2.first  < 0); // propagate backwards
   // assert(d2.second > 0); // propagate forwards
   if (d2.first >= 0 || d2.second <= 0) {
      Info("ExtendTrack2Barrel", "MatchTrk , unexpected solution for track crossing CTB\n" \
                                 "d2.first=%f, d2.second=%f, swap them", d2.first, d2.second);
      float xx  = d2.first;
      d2.first  = d2.second;
      d2.second = xx;
   }

   // extrapolate track to cylinder
   StThreeVectorD posR = trkHelix.at(d2.second);
   //printf(" punch2 x,y,z=%.1f, %.1f, %.1f, Rxy=%.1f\n",posCTB.x(),posCTB.y(),posCTB.z(),xmagn);
   float eta = posR.pseudoRapidity();
   float phi = posR.phi();

   int iEta, iPhi;
   if ( !ConvertEtaPhi2Bins(eta, phi, iEta, iPhi) ) return;

   //printf(" phi=%.0f deg,  eta=%.2f, iEta=%d, iPhi=%d\n",posCTB.phi()/3.1416*180.,posCTB. pseudoRapidity(),iEta, iPhi);
   // printf("hit Tower ID=%d\n",towerId);

   pointTower.id   = mapBtowIJ2ID[ iEta + iPhi * mxBTetaBin];
   pointTower.R    = TVector3(posR.x(), posR.y(), posR.z());
   pointTower.iEta = iEta;
   pointTower.iPhi = iPhi;
   //track.print();
}


void VecBosTrack::MatchTrack2BtowCluster()
{
   ExtendTrack2Barrel();

   if (pointTower.id <= 0) return; // skip endcap towers

   //printf("******* matchCluster() nVert=%d\n",mVecBosEvent->mVertices.size());
   float Rcylinder = mBtowGeom->Radius();

   float trackPT = prMuTrack->momentum().perp();

   // Choose 2x2 cluster with maximum ET
   mCluster2x2 = FindMaxBTow2x2(*mEvent, pointTower.iEta, pointTower.iPhi, mVertex->z);

   //hA[33] ->Fill(mCluster2x2.ET);
   //hA[34] ->Fill(mCluster2x2.adcSum, trackPT);
   //hA[110]->Fill(mCluster2x2.ET);

   // Compute surroinding cluster energy
   int iEta = mCluster2x2.iEta;
   int iPhi = mCluster2x2.iPhi;

   mCluster4x4 = SumBtowPatch(*mEvent, iEta - 1, iPhi - 1, 4, 4, mVertex->z); // needed for lumi monitor

   //if (mCluster2x2.ET < mMinBClusterEnergy) continue; // too low energy

   //hA[20] ->Fill("CL", 1.);
   //hA[206]->Fill(mCluster2x2.position.PseudoRapidity(), mCluster2x2.ET);
   //hA[37] ->Fill(mCluster4x4.ET);
   //hA[38] ->Fill(mCluster2x2.energy, mCluster4x4.energy - mCluster2x2.energy);

   //float frac24 = mCluster2x2.ET / mCluster4x4.ET;

   //hA[39]->Fill(frac24);

   //if (frac24 < mMinBClusterEnergyIsoRatio) continue;

   //hA[20]->Fill("fr24", 1.);

   // spacial separation (track - cluster)
   TVector3 D = pointTower.R - mCluster2x2.position;

   //hA[43]->Fill(mCluster2x2.energy,       D.Mag());
   //hA[44]->Fill(mCluster2x2.position.z(), D.z());

   float delPhi = pointTower.R.DeltaPhi(mCluster2x2.position);

   // printf("aaa %f %f %f   phi=%f\n",D.x(),D.y(),D.z(),delPhi);
   //hA[45]->Fill( mCluster2x2.energy, Rcylinder * delPhi); // wrong?
   //hA[46]->Fill( D.Mag());
   //hA[199]->Fill(mCluster2x2.position.PseudoRapidity(), D.Mag());
   //hA[207]->Fill(mCluster2x2.position.PseudoRapidity(), mCluster2x2.ET);

   if (D.Mag() <= mEvent->mMaxTrackClusterDist) {

      isMatch2Cl = true; // cluster is matched to TPC track
      //mVecBosEvent->mLeptonBTracks.insert(&track);
   }

   //hA[20]->Fill("#Delta R", 1.);
   //hA[111]->Fill(track.mCluster2x2.ET);
   //hA[208]->Fill(track.mCluster2x2.position.PseudoRapidity(), track.mCluster2x2.ET);
   //hA[0]->Fill("Tr2Cl", 1.0);
}
