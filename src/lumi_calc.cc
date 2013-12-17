#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TObject.h"
#include "TIterator.h"
#include "TString.h"

#include "utils/utils.h"

using namespace std;


int main(int argc, char *argv[])
{
   float lumiEff;
   float lumiTot;
   float lumiEffSum = 0;
   float lumiTotSum = 0;

   string filelist = "./runlists/run11_pp_transverse";

   TObject *o;
   TIter   *next = new TIter(utils::getFileList(filelist));

   while (next && (o = (*next)()) )
   {
       string runNo    = string(((TObjString*) o)->GetName());

       TString pathLumi =  "/star/data05/scratch/fazio/lumi_run11_cut05/";
       TString lumiEffName = pathLumi;
       lumiEffName += "R" + runNo + "_lumi_effective.txt";
       TString lumiTotalName = pathLumi;
       lumiTotalName += "R" + runNo + "_lumi_total.txt";

       cout << "Opening file: " << lumiEffName << endl;

       ifstream lumie;
       lumie.open (lumiEffName);
       lumie >> lumiEff;
       lumie.close();

       cout << "Opening file: " << lumiTotalName << endl;

       ifstream lumit;
       lumit.open (lumiTotalName);
       lumit >> lumiTot;
       lumit.close();

       lumiEffSum += lumiEff;
       lumiTotSum += lumiTot;

   }

   cout << "Effective luminosity (pb-1): " << lumiEffSum << endl;
   cout << "Total luminosity (pb-1): " << lumiTotSum << endl;

   return EXIT_SUCCESS;
}
