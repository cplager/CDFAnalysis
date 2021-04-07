// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "AnalysisNamespace/AnalysisNamespace.hh"
#include "RootFileCollection/OptionUtils.h"

using namespace std;
using namespace OptionUtils;

// Variable defs
TreeFileInterface AnalysisNamespace::ns_tfi;
string       AnalysisNamespace::ns_fileName;
string       AnalysisNamespace::ns_listName;
string       AnalysisNamespace::ns_outputName       = "output.root";
string       AnalysisNamespace::ns_goodRunListName;
int          AnalysisNamespace::ns_numWanted        = 10;
int          AnalysisNamespace::ns_section          = 0;
int          AnalysisNamespace::ns_totalSection     = 0;
int          AnalysisNamespace::ns_skipFirstNevents = 0;
int          AnalysisNamespace::ns_oneEveryNevents  = 0;
int          AnalysisNamespace::ns_cafJID           = 0;
int          AnalysisNamespace::ns_tfiVerbose       = 1;
int          AnalysisNamespace::ns_decoderVerbose   = 1;
bool         AnalysisNamespace::ns_useSam           = false;
GoodRunClass AnalysisNamespace::ns_goodRunObject;

void
AnalysisNamespace::setupDefaultOptions()
{
   addOptionKey ("file",           ns_fileName);
   addOptionKey ("list",           ns_listName);
   addOptionKey ("output",         ns_outputName);
   addOptionKey ("goodrunlist",    ns_goodRunListName);
   addOptionKey ("num",            ns_numWanted);
   addOptionKey ("section",        ns_section);  
   addOptionKey ("totalsection",   ns_totalSection);  
   addOptionKey ("skipfirst",      ns_skipFirstNevents);
   addOptionKey ("1in",            ns_oneEveryNevents);
   addOptionKey ("usesam",         ns_useSam);
   addOptionKey ("tfiverbose",     ns_tfiVerbose);
   addOptionKey ("decoderverbose", ns_decoderVerbose);
}

void
AnalysisNamespace::setupTFI (const TString &treename,
                             const string &outputNameTag,
                             bool useTopNtupleDecoder)
{
   ns_tfi.setTreeName (treename);
   ns_tfi.setTopNtupleDecoder (useTopNtupleDecoder);
   if ( ns_useSam)
   {
#ifdef __NO_TSAM__
      // Are we trying to access TSam when it is disabled?
      cerr << "topSkim: Can not use TSam with __NO_TSAM__"
           << endl;
      assert(0);
#endif
      // tell the file colleciton to use Sam
      ns_tfi.setCollectionType( RootFileCollection::kSam );
   } // if using sam 
   else
   {
      // tell the file collection tot use filelists
      ns_tfi.setCollectionType( RootFileCollection::kFileList );
      // Are we set?
      if (0 == ns_listName.length() && 0 == ns_fileName.length())
      {
         // we have to have an input list and an output root file
         cerr << "You must specify an input list root file." 
              << endl << "Use 'list=bla.list' arguments."
              << endl;
         exit(0);
      }
      if (ns_listName.length()) 
      {
         ns_tfi.addFilesFromList (ns_listName);
      } else if (ns_fileName.length()) {
         ns_tfi.addFile (ns_fileName);
      } else {
         assert(0);
      }
      if (ns_section && ns_totalSection)
      {
         ns_tfi.setSegment (ns_section, ns_totalSection);
      }
   } // if not using sam
   // Set the number of wanted events if different than 0
   if (ns_numWanted)
   {
      ns_tfi.setEventsWanted (ns_numWanted);
   }

   //////////////////////////////////////
   // Modify Output Name Appropriately //
   //////////////////////////////////////
   ns_outputName = removeEnding (ns_outputName, ".root");
   // add on the tag we were passed in
   ns_outputName += outputNameTag;
   if (ns_section)
   {
      ns_outputName += Form ("_sec%03d", ns_section);
   }
   if (ns_cafJID)
   {
      ns_outputName += Form ("_jid%03d", ns_cafJID);
   }
   if (ns_numWanted)
   {
      ns_outputName += Form ("_n%d", ns_numWanted);
   }
   if (ns_oneEveryNevents)
   {
      ns_outputName += Form ("_1in%d", ns_oneEveryNevents);
   }
   ns_outputName += ".root";

   ////////////////////////////
   // Finish Any Other Setup //
   ////////////////////////////
   // setup good run list
   if (ns_goodRunListName.length())
   {
      ns_goodRunObject.loadfile (ns_goodRunListName);
   }
   if (ns_oneEveryNevents)
   {
      ns_tfi.set1inEveryN (ns_oneEveryNevents);
   }
   if (ns_tfiVerbose)
   {
      ns_tfi.setVerbose (true);
   } else {
      ns_tfi.setVerbose (false);
   }
   TopNtupleDecoder::setVerboseMode (ns_decoderVerbose);
}

bool
AnalysisNamespace::finalTFIsetup()
{
   // If you are going to skip events, this needs to be AFTER telling
   // the TFI which branches we want hooked up.
   if (ns_skipFirstNevents && ! ns_tfi.skipNevents (ns_skipFirstNevents))
   {
      // nothing left
      return false;
   }
   return true;
}
