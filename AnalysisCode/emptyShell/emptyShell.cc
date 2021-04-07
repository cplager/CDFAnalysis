// -*- C++ -*-
#include "CLPClasses/dout.h"
#include "TH1F.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include "CLPClasses/CLPClasses.h"
#include "RootFileCollection/TreeFileInterface.h"
#include "AnalysisNamespace/AnalysisNamespace.hh"
#include "RootFileCollection/OptionUtils.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace AnalysisNamespace;
using namespace OptionUtils;

// global variables

// forward declarations
void localInitialize();

int main(int argc, char** argv)
{
   localInitialize();     // hook up local options
   setupDefaultOptions(); // hook up standard options
   OptionUtils::parseArguments (argc, argv);
   ///////////////
   // setup TFI //
   ///////////////
   const string kCountBranch = "count";
   setupTFI ("CountTree");

   // Tell the TFI which ALL branches you want MAY want to use (if you
   // don't tell it know, you won't be able to access them later).
   ns_tfi.addBranch< int > (kCountBranch, "/I");   

   // Do any final TFI setup that needs to be done after setting the
   // branches, etc (e.g. skipping the first N events).  This should
   // be the last thing right before the 'while
   // (ns_tfi.getNextEvent())'
   finalTFIsetup();

   TFile *filePtr = TFile::Open ("bla.root", "RECREATE");
   if (! filePtr)
   {
      cerr << "Could not open file for output" << endl;
      exit(1);
   }
   ns_tfi.setOutputFile (filePtr);
   TTree *treePtr = ns_tfi.addTTree ("sillyTree");
   int count;
   ns_tfi.addBranchObject ("count2", "/I", &count);
   ns_tfi.addBranchToTree ("sillyTree", "count");
   ns_tfi.addBranchToTree ("sillyTree", "count2");
   cerr << "about to start" << endl;
   while ( ns_tfi.getNextEvent() )
   {
      count = ns_tfi.value< int > (kCountBranch) + 1;
      cout << "count " 
           << ns_tfi.value< int > (kCountBranch)
           << endl;
      treePtr->Fill();
   } // while getNextEvent
   return 0;
}

void localInitialize()
{
   // addOptionKey ("string", gVariable);
}

