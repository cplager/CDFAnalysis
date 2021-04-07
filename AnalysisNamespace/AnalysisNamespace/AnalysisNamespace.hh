// -*- C++ -*-

#if !defined(ANALYSIS_NAMESPACE_HH)
#define ANALYSIS_NAMESPACE_HH

#include <string>
#include "RootFileCollection/TreeFileInterface.h"
#include "CLPClasses/CLPClasses.h"
#include "TString.h"
//#include <math.h>

namespace AnalysisNamespace
{
   /////////////////////
   // /////////////// //
   // // Variables // //
   // /////////////// //
   /////////////////////

   // Our TreeFileInterface
   extern TreeFileInterface ns_tfi;

   // Strings describing input/output
   extern std::string ns_fileName;
   extern std::string ns_listName;
   extern std::string ns_outputName;
   extern std::string ns_goodRunListName;

   // Integers, doubles, and bools (oy my)
   extern int  ns_numWanted;
   extern int  ns_section;
   extern int  ns_totalSection;
   extern int  ns_skipFirstNevents;
   extern int  ns_oneEveryNevents;
   extern int  ns_cafJID;
   extern int  ns_tfiVerbose;
   extern int  ns_decoderVerbose;
   extern bool ns_useSam;

   // other objects
   extern GoodRunClass ns_goodRunObject;

   ////////////////////////////////
   // ////////////////////////// //
   // // Function Definitions // //
   // ////////////////////////// //
   ////////////////////////////////

   // sets up default options (e.g., list, file, etc).
   void setupDefaultOptions();

   // sets up the TFI, output file name, etc.
   void setupTFI (const TString &treename, 
                  const std::string &outputNameTag = "",
                  bool useTopNtupleDecoder = false);

   // done after adding branches, etc.
   bool finalTFIsetup ();
};

#endif // ANALYSIS_NAMESPACE_H
