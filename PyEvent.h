#ifndef PyEvent_h
#define PyEvent_h

//This macro provides the event and track class for the tree files

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

#include <TObject.h>

#include "track.h"

class WEvent;


class PyEvent : public TObject {
public:

	std::vector<track> tracks;
	int I;
	int nParticles;//particle number in the final state
	int ievent;	
	int genevent;
	int subprocess;
	int nucleon;
	double targetparton;
	double xtargparton;
	double beamparton;
	double xbeamparton;
	double thetabeamprtn;
	double pt2_hat;
	double Q2_hat;
	int nrTracks;

   Bool_t AcceptWEvent(WEvent& wEvent);

	ClassDef( PyEvent, 1 )
};

#endif
