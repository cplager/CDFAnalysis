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
#include <cmath>
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
// verbose flag (verbose)
int gVerbose = 0;

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
   setupTFI ("CLPTree");

   // Tell the TFI which ALL branches you want MAY want to use (if you
   // don't tell it know, you won't be able to access them later).
   // TCA branches
   ns_tfi.addTcaBranch ( kElectronBranch, kElectronClassName );
   ns_tfi.addTcaBranch ( kMuonBranch    , kMuonClassName     );
   ns_tfi.addTcaBranch ( kJetBranch     , kJetClassName      );
   // Class branches
   ns_tfi.addClassBranch< CLP3Vector > (kMetBranch);
   // Simple object branches
   ns_tfi.addBranch< int >    (kRunBranch);
   ns_tfi.addBranch< int >    (kEventBranch);
   ns_tfi.addBranch< int >    (kTopEventBranch);
   ns_tfi.addBranch< int >    (kNumDilLepBranch);
   ns_tfi.addBranch< double > (kJetZ0Branch);

   // Do any final TFI setup that needs to be done (e.g. skipping the
   // first N events) This needs to be done AFTER setting the
   // branches, etc and should be the last thing right before the
   // 'while (ns_tfi.getNextEvent())'
   finalTFIsetup();

   if (gVerbose)
   {
      cout << "## Entry Run   Event  Lep   Met     njets" << endl
           << "# acc Met 0.01" << endl;
   }

   enum {
      kThree,
      kFour,
      kNumBins
   };
   enum {
      kSingle,
      kDouble,
      kNumTags
   };
   enum {
      kLoose,
      kTight,
      kTightLooseTaggers
   };

   int numPassed = 0;
   int numSeen = 0;
   int counts[kNumBins][kTightLooseTaggers][kNumTags] = {{{}}};
   while ( ns_tfi.getNextEvent() )
   {
      ++numSeen;
      // Simple objects
      ns_runNumber   = ns_tfi.value< int >    (kRunBranch);
      ns_eventNumber = ns_tfi.value< int >    (kEventBranch);
      ns_topEvent    = ns_tfi.value< int >    (kTopEventBranch);
      ns_numDil      = ns_tfi.value< int >    (kNumDilLepBranch);
      ns_jetZ0       = ns_tfi.value< double > (kJetZ0Branch);
      if (0 && gVerbose)
      {
         cout << endl << endl
              << setw(6) << ns_runNumber
              << setw(8) << ns_eventNumber << endl;
      }
      // Class pointers
      ns_met3VecPtr  = ns_tfi.classPtr< CLP3Vector > (kMetBranch);
      
      if (0 && gVerbose) cout << "met " << *ns_met3VecPtr 
                              << " " << ns_met3VecPtr->transMag() << endl;
      CLPJet::JetList jetList;
      if (! passesLJfilter (ns_tfi.tcaPtr (kJetBranch),
                            ns_tfi.tcaPtr (kElectronBranch),
                            ns_tfi.tcaPtr (kMuonBranch),
                            jetList))
      {
         continue;
      }
      
      int numLoose = 0, numTight = 0;
      int numgoodjets = (int) jetList.size();
      for (int jetLoop = 0; jetLoop < numgoodjets; ++jetLoop)
      {
         if (jetList[jetLoop].isBTagged (CLPJet::kLoose))
         {
            ++numLoose;
         }
         if (jetList[jetLoop].isBTagged (CLPJet::kTight))
         {
            ++numTight;
         }
      } // for jetLoop
      
      for (int threeFour = kThree; threeFour <= kFour; ++threeFour)
      {
         int threeFourThreshold = 3 + threeFour;
         if (0) cout << "tFT " << threeFourThreshold << endl;
         if (numgoodjets < threeFourThreshold)
         {
            continue;
         }
         for (int tightLoose = kLoose; tightLoose <= kTight; ++tightLoose)
         {
            int numTag = numLoose;
            if (0) cout << "  numTag: ";
            if (kTight == tightLoose)
            {
               if (0) cout << "(tight) ";
               numTag = numTight;
            }
            if (0) cout << numTag << endl;
            for (int singDoub = kSingle; singDoub <= kDouble; ++singDoub)
            {
               int singDoubThreshold = singDoub + 1;
               if (0) cout << "    sDT " << singDoubThreshold << endl;
               if (numTag < singDoubThreshold)
               {
                  continue;
               }
               // IF we're still here, count it, baby
               if (0) cout << "         IT!" << endl;
               ++counts[threeFour][tightLoose][singDoub];
            } // for singDoub
         } //  for tightLoose
      } // for threeFour

      if (gVerbose)
      {
         cout << setw(5) << numSeen - 1 << "  "
              << setw(6) << ns_runNumber
              << setw(8) << ns_eventNumber;
         if (ns_leptonPtr->isaElectron())
         {
            cout << "   e" << Form ("%-2d", ns_leptonPtr->index());
         } 
         else if (ns_leptonPtr->isaMuon())
         {
            cout << "   m" << Form ("%-2d", ns_leptonPtr->index());
         }
         cout << Form (" %6.2f", ns_met3VecPtr->transMag())
              << "   " << jetList.size() 
              << "   " << numLoose << "  " << numTight << endl;
      } // if verbose

      ++numPassed;
   } // while getNextEvent
   cout << numPassed << " out of " << numSeen<< endl;
      
   for (int threeFour = kThree; threeFour <= kFour; ++threeFour)
   {
      if (kThree == threeFour)
      {
         cout << "Three or more jets" << endl;
      } else {
         cout << "Four or more jets" << endl;
      }
      for (int tightLoose = kLoose; tightLoose <= kTight; ++tightLoose)
      {
         if (kLoose == tightLoose)
         {
            cout << "  Loose Tagger" << endl;
         } else {
            cout << "  Tight Tagger" << endl;
         }
         for (int singDoub = kSingle; singDoub <= kDouble; ++singDoub)
         {
            if (kSingle == singDoub)
            {
               cout << "    >= 1 tag ";
            } else {
               cout << "    >= 2 tag ";
            }
            // IF we're still here, count it, baby
            cout << Form ("%5d", counts[threeFour][tightLoose][singDoub])
                 << Form ("  %.3f%%", counts[threeFour][tightLoose][singDoub] /
                          (double) numSeen * 100)
                 << endl;
         } // for singDoub
      } //  for tightLoose
   } // for threeFour
   return 0;
}

void localInitialize()
{
   // addOptionKey ("string", gVariable);
   addOptionKey ("verbose", gVerbose);
}

