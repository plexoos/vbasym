
#include <getopt.h>
#include <cstdlib>

#include "VbAnaOptions.h"


using namespace std;


/** */
VbAnaOptions::VbAnaOptions() : AnaOptions(),
   fOptions("Allowed options"), fOptionsValues(),
   fFitSinePhase(0), fFitSineOffset(0)
{
   // Declare the supported options
   fOptions.add_options()
      ("help,h", "Print help message")
      ("filelist,f",      po::value<string>(&fListName), "Name of the file with input list")
      ("max-events,n",    po::value<uint32_t>(&fMaxEventsUser)->default_value(0), "Maximum number of events to process")
      ("fit-sine-phase",  po::value<double>(&fFitSinePhase)->default_value(M_PI_2)->implicit_value(DBL_MAX), "Value for phase")
      ("fit-sine-offset", po::value<double>(&fFitSineOffset)->default_value(0)->implicit_value(DBL_MAX), "Value for offset")
   ;

   fOutFileName = "vbana";
}


/** */
string VbAnaOptions::GetRootFileName() const { return GetResultsDir() + "/hist/" + fOutFileName + GetSuffix() + ".root"; }
double VbAnaOptions::GetFitSinePhase() const { return fFitSinePhase; }
double VbAnaOptions::GetFitSineOffset() const { return fFitSineOffset; }


/** */
void VbAnaOptions::ProcessOptions(int argc, char **argv)
{
   po::store(po::parse_command_line(argc, argv, fOptions), fOptionsValues);
   po::notify(fOptionsValues);

   if (fOptionsValues.count("help"))
   {
      cout << fOptions << endl;
      exit(EXIT_FAILURE);
   }

   Info("ProcessOptions", "User provided options:");

   if (fOptionsValues.count("filelist"))
   {
      cout << "filelist: " << fListName << endl;
      ifstream file_list(GetListFileName().c_str());
      if (!file_list.good()) {
         Error("VbAnaOptions", "Filelist \"%s\" does not exist", GetListFileName().c_str());
         exit(EXIT_FAILURE);
      }
   } else {
      Error("VbAnaOptions", "Filelist not set");
      exit(EXIT_FAILURE);
   }

   if (fOptionsValues.count("max-events"))
   {
      cout << "max-events: " << fMaxEventsUser << endl;
   }

   if (fOptionsValues.count("fit-sine-phase"))
   {
      cout << "fit-sine-phase: " << fFitSinePhase << endl;
   }

   if (fOptionsValues.count("fit-sine-offset"))
   {
      cout << "fit-sine-offset: " << fFitSineOffset << endl;
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
