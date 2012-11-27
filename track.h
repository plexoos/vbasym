#ifndef track_h
#define track_h

#include <TObject.h>

class track : public TObject
{

public:

//define the variables for the particle
//	int index, KS, id, mother1, mother2, daughter1, daughter2, process1, process2;
//	double px, py, pz, E, m, x, y, z;
	
	int    index;
	int    id;
	int    KS;
	int    mother;
	int    daughter1;
	int    daughter2;
	double px;
	double py;
	double pz;
	double E;
	double m;
	double x;
	double y;
	double z;

   //derived variables like pt, theta, phi, rapidity...
	double pt;
	double p;
	double theta;
	double phi;
	double rapidity;
	double eta;

   track( const std::string& line1 = "" );

   void print();

	ClassDef(track, 1 )//define a dictionary for the root
};

#endif
