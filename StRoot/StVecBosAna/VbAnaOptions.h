#ifndef VbAnaOptions_h
#define VbAnaOptions_h

#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <sys/stat.h>

#include "AnaOptions.h"
#include "Globals.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
namespace po = boost::program_options;


/**
 * Processes and controls user options provided in the command line.
 */ 
class VbAnaOptions : public AnaInfo
{
public:

   // Flag options
   enum Flag   {FLAG_COPY              = 0x10000400,
                FLAG_READ_FROM_DB      = 0x10000100,
                FLAG_UPDATE_DB         = 0x10000200, FLAG_NO_UPDATE_DB = 0x90000200,
                FLAG_USE_DB            = 0x10000300,
                FLAG_CREATE_THUMBS     = 0x10000800};

   // Flag options
   enum Option {OPTION_POL_ID          = 0x20004000,
                OPTION_SET_CALIB       = 0x20003000,
                OPTION_SET_CALIB_ALPHA = 0x20001000,
                OPTION_SET_CALIB_DL    = 0x20002000,
                OPTION_DET_DISABLE     = 0x20000001,
                OPTION_SUFFIX          = 0x20000002,
                OPTION_JETS_PT_MIN     = 0x20000003,
                OPTION_RHIC_RUN_ID     = 0x20000004,
                OPTION_TRACKS_PT_MIN   = 0x20000005};

public:

   VbAnaOptions();

   virtual void ProcessOptions(int argc, char **argv);
   virtual void VerifyOptions();
   void         Print(const Option_t* opt="") const;
   void         PrintAsPhp(FILE *f=stdout) const;
   virtual void PrintUsage();

   double mFitSinePhase;
   double mFitSineOffset;

protected:

   po::options_description mOptions;
   po::variables_map mOptionsValues;

};

#endif
