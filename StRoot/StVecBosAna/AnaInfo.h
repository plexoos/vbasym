#ifndef AnaInfo_h
#define AnaInfo_h

#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <sys/stat.h>

#include "TObject.h"
#include "TSystem.h"

#include "Globals.h"


/**
 * Processes and controls user options provided in the command line.
 */ 
class AnaInfo : public TObject
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

   // Various histogramming and running modes
   enum Mode   {MODE_GRAPH             = 0x02000000, MODE_NO_GRAPH     = 0x82000000};

public:

   std::string      fSuffix;            // Additional unique identifier for analysis job
   uint64_t         fModes;
   bool             fDoReconstructJets;
   float            fTracksPtMin;
   float            fJetPtMin;
   unsigned short   fRhicRunId;         // RHIC run id, e.g. 9, 11, 12, 13, ...
   bool             fIsMc;
   BosonType        fBosonType;
   time_t           fAnaDateTime;       // Date/time when data analysis started
   double           fAnaTimeReal;       // Time in seconds to process input raw file
   double           fAnaTimeCpu;        // Time in seconds to process input raw file
   Str2StrMap       fAsymEnv;
   uint32_t         fMaxEventsUser;
   FILE            *fFileMeasInfo;      //!
   FILE            *fFileStdLog;        //!
   std::string      fFileStdLogName;
   bool             fFlagCopyResults;
   bool             fFlagUpdateDb;
   UserGroup_t      fUserGroup;

public:

   AnaInfo();
   virtual ~AnaInfo();

   std::string  GetSuffix() const;
   std::string  GetResultsDir() const;
   std::string  GetOutDir() const;
   std::string  GetImageDir() const;
   virtual std::string  GetAnaInfoFileName() const;
   std::string  GetStdLogFileName() const;
   std::string  GetRootFileName() const;
   FILE*        GetAnaInfoFile() const;
   virtual void ProcessOptions(int argc, char **argv);
   virtual void VerifyOptions();
   void         SetListName(std::string listName) { fListName = listName; }
   std::string  GetListName() { return fListName; }
   void         Print(const Option_t* opt="") const;
   void         PrintAsPhp(FILE *f=stdout) const;
   virtual void PrintUsage();

   bool         HasGraphBit() const;

protected:

   std::string fListName;
   std::string fOutputName;

   void MakeOutDir();

   ClassDef(AnaInfo, 1)
};

#endif
