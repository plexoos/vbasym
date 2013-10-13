
#include <getopt.h>
#include <cstdlib>

#include "VbAnaOptions.h"


using namespace std;


/** */
VbAnaOptions::VbAnaOptions() : AnaInfo(),
   mFitSinePhase(0), mFitSineOffset(0),
   mOptions("Allowed options"), mOptionsValues()
{
   // Declare the supported options.
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
}


/** */
void VbAnaOptions::VerifyOptions()
{
   AnaInfo::VerifyOptions();
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
   fprintf(f, "$rc['fOutputName']                  = \"%s\";\n", fOutputName.c_str());
   fprintf(f, "$rc['fSuffix']                      = \"%s\";\n", fSuffix.c_str());
   fprintf(f, "$rc['fModes']                       = %#010x;\n",  fModes);
   fprintf(f, "$rc['fAnaDateTime']                 = %u;\n",     (UInt_t) fAnaDateTime);
   fprintf(f, "$rc['fAnaTimeReal']                 = %f;\n",     fAnaTimeReal);
   fprintf(f, "$rc['fAnaTimeCpu']                  = %f;\n",     fAnaTimeCpu);

   stringstream ssEnvs("");

   ssEnvs << "array(";

   for (Str2StrMap::const_iterator ienv=fAsymEnv.begin(); ienv!=fAsymEnv.end(); ienv++) {
      ssEnvs << "'" << ienv->first << "'"  << " => " << "\"" << ienv->second << "\"";
      ssEnvs << (ienv != (--fAsymEnv.end()) ? ", " : "");
   }

   ssEnvs << ")";

   fprintf(f, "$rc['fAsymEnv']                     = %s;\n", ssEnvs.str().c_str());

   fprintf(f, "$rc['fFileStdLogName']              = \"%s\";\n", fFileStdLogName.c_str());
   fprintf(f, "$rc['fFlagCopyResults']             = %d;\n", fFlagCopyResults);
   fprintf(f, "$rc['fFlagUpdateDb']                = %d;\n", fFlagUpdateDb);

   fprintf(f, "$rc['fUserGroup_fUser']             = \"%s\";\n", fUserGroup.fUser.Data());
   fprintf(f, "$rc['fUserGroup_fRealName']         = \"%s\";\n", fUserGroup.fRealName.Data());

   fprintf(f, "\n");
}


/** */
void VbAnaOptions::PrintUsage()
{
   AnaInfo::PrintUsage();

   cout << endl;
   cout << " -h, -?                               : Print this help" << endl;
   cout << " -l, --log=[filename]                 : Optional log file to redirect stdout and stderr" << endl;
   cout << " -r, -f, --list                       : Input list file" << endl;
   cout << " -m, --mc                             : Process input as monte-carlo" << endl;
   cout << endl;
}
