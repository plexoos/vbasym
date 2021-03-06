#include <string>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char *argv[])
{
   float evtCount;
   float eventsSum = 0;
   string stringfname;

   //string filelist = "./runlists/run11_transverse_embed_gt100evt_test";
   //cout << "Opening file: " << filelist << endl;

   ifstream infile;
   //infile.open ("./runlists/run11_transverse_embed_gt500evt_evtcount2");
   infile.open ("~vbasym/runlists/run11_transverse_embed_gt500evt_evtcount");

   if(!infile)
   {
       cout << "Can't open file" << endl;
       exit(EXIT_FAILURE);
   }

   // while (!infile.eof())
   while (infile >> stringfname >> evtCount) // second coulmn overrides the first
   {
      //infile >> evtCount;
      //infile.close();
      eventsSum += evtCount;
   }

   cout << "Number of events: " << eventsSum << endl;
}
