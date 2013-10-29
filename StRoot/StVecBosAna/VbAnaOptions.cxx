
#include <getopt.h>
#include <cstdlib>

#include "VbAnaOptions.h"


using namespace std;


/** */
VbAnaOptions::VbAnaOptions() : AnaOptions(),
   fFitSinePhase(0), fFitSineOffset(0),
   mOptions("Allowed options"), mOptionsValues()
{
   // Declare the supported options
   mOptions.add_options()
      ("help,h", "Print help message")
      ("fit-sine-phase",  po::value<double>(&fFitSinePhase)->default_value(M_PI_2)->implicit_value(DBL_MAX), "Value for phase")
      ("fit-sine-offset", po::value<double>(&fFitSineOffset)->default_value(0)->implicit_value(DBL_MAX), "Value for offset")
      ("filelist,f",      po::value<string>(&fListName), "Name of the file with input list")
   ;

   fOutFileName = "vbana";
}


/** */
string VbAnaOptions::GetRootFileName() const { return GetResultsDir() + "/hist/" + fOutFileName + GetSuffix() + ".root"; }


/** */
void VbAnaOptions::ProcessOptions(int argc, char **argv)
{
   po::store(po::parse_command_line(argc, argv, mOptions), mOptionsValues);
   po::notify(mOptionsValues);

   if (mOptionsValues.count("help"))
   {
      cout << mOptions << endl;
      exit(EXIT_FAILURE);
   }

   Info("ProcessOptions", "User provided options:");

   if (mOptionsValues.count("fit-sine-phase"))
   {
      cout << "fit-sine-phase: " << fFitSinePhase << endl;
   }

   if (mOptionsValues.count("fit-sine-offset"))
   {
      cout << "fit-sine-offset: " << fFitSineOffset << endl;
   }

   if (mOptionsValues.count("filelist"))
   {
      cout << "filelist: " << fListName << endl;
   } else {
      Error("VbAnaOptions", "Filelist not set");
      exit(EXIT_FAILURE);
   }

   AnaOptions::VerifyOptions();
}


/** */
void VbAnaOptions::Print(const Option_t* opt) const
{
   Info("Print", "Print members:");
   PrintAsPhp();
}


/** */
void VbAnaOptions::PrintAsPhp(FILE *f) const
{
   AnaOptions::PrintAsPhp(f);
}
