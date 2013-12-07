#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

#include "TMath.h"
#include "TString.h"
#include "TObject.h"
#include "TTree.h"
#include "TSystem.h"
#include "TFile.h"
#include "TVector3.h"

#include "StVecBosAna/PythiaEvent.h"
#include "StVecBosAna/WBosMcEvent.h"

using namespace std;


/** This program creates a root tree generated from a pythia listing. */
int main(int argc, char *argv[])
{
   //string inputFileName = "/eicdata/eic0010/wana_mc_data/pythia_w.txt";
   //string inputFileName = "/eicdata/eic0010/wana_mc_data/pythia_w_2.txt";
   string inputFileName = "/eicdata/eic0004/PYTHIA/pp/pythia.pp.250x250.1Mevents.ckin3=1.kt=0.5.txt";

   if(argc > 1) {
      inputFileName = argv[1];
   }

   ifstream inputFile( inputFileName.c_str(), ios::in );

   // Set 10 Gb for the root file. LL used to force long integer.
   TTree::SetMaxTreeSize( 10LL * 1024LL * 1024LL * 1024LL );

   //Get the input file name, stripping any leading directory path via
   //use of the Basename() method from TSystem.
   TString outName = gSystem->BaseName( inputFileName.c_str() );

   //Remove the existing extension, if there is one.
   if ( outName.Last( '.' ) > -1 ) {
      outName.Replace( outName.Last( '.' ), outName.Length(), "" );
   }

   outName.Append(".root");

   PythiaEvent* pyEvent = new PythiaEvent();
   WBosMcEvent* wBosMcEvent = new WBosMcEvent();

   TFile treefile(outName, "recreate");

   TTree pyTree("pyTree", "pyTree");
   TTree wTree("wTree", "wTree");

   pyTree.Branch("PythiaEvent", &pyEvent, 640000, 99 );
   wTree.Branch("WBosMcEvent", &wBosMcEvent, 640000, 99 );

   bool start   = false; // set a switch for reading data
   bool doboost = false; // set a switch for the boost

   if ( !inputFile.is_open() ) {
      std::cout << "Error! File not found. \n" <<std::endl;
      return 0;
   }

   std::cout << "Opened " << inputFileName << std::endl;

   std::string line;//set a line string to store one line of the data

   const std::string searchPattern1( "K" );//A flag to record the first data
   const std::string searchPattern2( "finished" );//A flag for the end of a event


   int N = 0; // counter for every 1000 events
   const double m = 0.9383;

   //while( inputFile.good() && N < 100)
   while( inputFile.good())
   {
      std::getline( inputFile, line );

      //cout << "l: " << line << endl;

      size_t posit1 = line.find( searchPattern1 );
      size_t posit2 = line.find( searchPattern2 );

      // first pattern to find the start of an event
      if ( posit1 not_eq string::npos ) {
         for( int i(0); i< 2; ++i ) std::getline( inputFile, line );//skip the current line
         std::stringstream ss;
         ss << line;
         ss >> pyEvent->ievent >> pyEvent->ievent  //skip the 1st number
            >> pyEvent->genevent >> pyEvent->subprocess >> pyEvent->nucleon
            >> pyEvent->targetparton >> pyEvent->xtargparton >> pyEvent->beamparton >> pyEvent->xbeamparton
            >> pyEvent->thetabeamprtn >> pyEvent->pt2_hat >> pyEvent->Q2_hat >> pyEvent->nrTracks;

         start = true;
         for ( int i(0); i< 2; ++i ) getline( inputFile, line);
      }

      // second pattern to find the end of an event and push it back to the event tree class
      if ( posit2 not_eq string::npos)
      {
         if ( pyEvent->AcceptWEvent(*wBosMcEvent) ) {
            //cout << "Accepted event" << endl;

            wBosMcEvent->RecoW();
            wBosMcEvent->CalcRecoil(*pyEvent);

            pyTree.Fill();
            wTree.Fill();
         } else {
            //cout << "Rejected event" << endl;
         }

         pyEvent->tracks.clear();
         pyEvent->nParticles = 0;

         if ( !inputFile.eof() ) {
            std::getline( inputFile, line );
            std::stringstream ss;
            ss << line;
            ss >> pyEvent->ievent >> pyEvent->ievent  //skip the 1st number
               >> pyEvent->genevent >> pyEvent->subprocess >> pyEvent->nucleon
               >> pyEvent->targetparton >> pyEvent->xtargparton >> pyEvent->beamparton >> pyEvent->xbeamparton
               >> pyEvent->thetabeamprtn >> pyEvent->pt2_hat >> pyEvent->Q2_hat >> pyEvent->nrTracks;

            for (int i(0); i<2; ++i ) std::getline( inputFile, line );
         }
      }

      if ( !start ) continue;

      track particle( line );

      // get some extra handy particlewise variables
      particle.pt    = sqrt( particle.px*particle.px + particle.py*particle.py );
      particle.p     = sqrt( particle.pt*particle.pt + particle.pz*particle.pz );
      particle.theta = atan2( particle.pt, particle.pz );
      particle.phi   = atan2( particle.py, particle.px);

      if( particle.phi<0 ) particle.phi+=2*TMath::Pi();

      // calculate the rapidity and eta
      particle.rapidity = (1./2.)*log((particle.E+particle.pz)/(particle.E-particle.pz));
      particle.eta      = (1./2.)*log((particle.p+particle.pz)/(particle.p-particle.pz));

      if( particle.KS == 1) pyEvent->nParticles++;

      // boost over then push back to the particles
      pyEvent->tracks.push_back( particle );

      // counter check every 1000 events
      if ( pyEvent->ievent/1000 > N) { N++; cout << 1000*N << "completed" << endl; }
   }

   std::cout << "Found " << pyEvent->ievent << " events" << std::endl;

   pyTree.Write();
   wTree.Write();

   inputFile.close();

   cout << "|~~~~~~~~~~~~~~Completed~~~~~~~~~~~~~~|" << endl;

   return 1;
}
