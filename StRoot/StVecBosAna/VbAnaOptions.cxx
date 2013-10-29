
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
      ("help", "produce help message")
      ("fit-sine-phase",  po::value<double>(&mFitSinePhase)->default_value(M_PI_2)->implicit_value(DBL_MAX), "Value for phase")
      ("fit-sine-offset", po::value<double>(&mFitSineOffset)->default_value(0)->implicit_value(DBL_MAX), "Value for offset")
   ;
}


/** */
void VbAnaOptions::ProcessOptions(int argc, char **argv)
{
   po::store(po::parse_command_line(argc, argv, mOptions), mOptionsValues);
   po::notify(mOptionsValues);

   if (mOptionsValues.count("fit-sine-phase"))
   {
      cout << "fit-sine-phase: " << mOptionsValues["fit-sine-phase"].as<double>() << ", "
         << mFitSinePhase << endl;
   }

   if (mOptionsValues.count("fit-sine-offset"))
   {
      cout << "fit-sine-offset: " << mOptionsValues["fit-sine-offset"].as<double>() << ", "
         << mFitSineOffset << endl;
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
