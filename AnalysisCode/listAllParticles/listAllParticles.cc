// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"

#include "time.h"
#include "CLPClasses/dout.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include "CLPClasses/CLPClasses.h"
#include "RootFileCollection/TreeFileInterface.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;

// Global Variables (ugh..) 
// Note, name in parenthesis is command-line variable name.
// e.g. 'which=12 usetracks=1' will set gWhich = 12 and gUseTracks = false

// Which file to grab (which)
int gWhich         = 0;
// number of events (numwanted/howmany)
int gNumWanted     = 10;
// allow PEM electrons (pem)
bool gUsePem       = false;
// check electrons (electron)
bool gUseElectrons = false;
// check muons (muon)
bool gUseMuons     = false;
// check tracks (track)
bool gUseTracks    = false;
// check jets (jet)
bool gUseJets      = true;
// file name (file)
string gFileName;

// forward declarations
void initialize();

int main(int argc, char** argv)
{
   initialize();
   parseArguments (argc, argv);
   cout << "## numWanted " << gNumWanted << endl;

   ///////////////////////
   // Set Defaults Here //
   ///////////////////////   
   CLPJet::setDefaultCorrectionType (CLPJet::kLevel5);
   //CLP3Vector::setOutput3Mode( CLP3Vector::kStandardOutputMode);
   CLPCandidate::setOutputType (CLPCandidate::kEverything);
   enum {
      kInput = 0,
      kOutput,
   };

   if (0 == gWhich) 
   {
      // do nothing
   } 
   if (0 == gFileName.length())
   {
      cout << "invalid value of which " << gWhich << endl;
      exit(0);
   }


   ////////////////////
   // Initialization //
   ////////////////////


   TreeFileInterface tfi ("CLPTree");
   tfi.addFile (gFileName);
   if (gNumWanted)
   {
      tfi.setEventsWanted (gNumWanted);
   }

   const string kEventBranch    = "eventNum";
   const string kRunBranch      = "runNum";
   const string kSectionBranch  = "runSection";
   const string kElectronBranch = "electrons";
   const string kMuonBranch     = "muons";
   const string kJetBranch      = "jets";
   const string kTrackBranch    = "tracks";

   tfi.addBranch < int > (kEventBranch    );
   tfi.addBranch < int > (kRunBranch      );
   tfi.addBranch < int > (kSectionBranch  );
   tfi.addTcaBranch (kElectronBranch, kElectronClassName );
   tfi.addTcaBranch (kMuonBranch    , kMuonClassName     );
   tfi.addTcaBranch (kJetBranch     , kJetClassName      );
   tfi.addTcaBranch (kTrackBranch   , kTrackClassName    );
   

   cout << "Here we go" << endl;
   while (tfi.getNextEvent())
   {
      // check run
      cout << endl << endl 
           << "run " << tfi.value< int > (kRunBranch) 
           << " event " << tfi.value< int > (kEventBranch)
           << endl;
   
      ///////////////
      // Electrons //
      ///////////////
      if (gUseElectrons)
      {
         int numTCAelecs = tfi.GetEntries (kElectronBranch);
         for (int elecLoop = 0; elecLoop < numTCAelecs; ++elecLoop)
         {
            CLPElectron *elecPtr 
               = (CLPElectron*) tfi.At(kElectronBranch, elecLoop);
            if ( elecPtr->isaTCE() || elecPtr->isaPHX() || 
                 ( gUsePem && elecPtr->isaPEM() ) )
            {
               CLPCuts::ElectronResult result = CLPCuts::electronCuts(*elecPtr);
               // tight cuts
               if ( result.none() )
               {
               } 
            } // is an electron
         } // for elecLoop
      } // if use electrons

      ///////////
      // Muons //
      ///////////
      if (gUseMuons)
      {
         int numTCAmuons = tfi.GetEntries (kMuonBranch);
         for (int muonLoop = 0; muonLoop < numTCAmuons; ++muonLoop)
         {
            CLPMuon *muonPtr 
               = (CLPMuon*) tfi.At(kMuonBranch, muonLoop);
            CLPCuts::MuonResult result = CLPCuts::muonCuts( *muonPtr);
            // for now, we are only allowing tight leptons
            if ( result.none() && muonPtr->isaTightLepton())
            {
            }
         } // for muonLoop
      } // if use muons

      ////////////
      // Tracks //
      ////////////
      if (gUseTracks)
      {
         int numTCAtracks = tfi.GetEntries (kTrackBranch);
         for (int trackLoop = 0; trackLoop < numTCAtracks; ++trackLoop)
         {
            CLPTrack *trackPtr 
               = (CLPTrack*) tfi.At(kTrackBranch, trackLoop);
            CLPCuts::TrackResult result = CLPCuts::trackCuts( *trackPtr);
            if ( result.none() )
            {
            } //if it passed all cuts
         } // for trackLoop
      } // if use tracks

      //////////
      // Jets //
      //////////
      if (gUseJets)
      {
         int numTCAjets = tfi.GetEntries (kJetBranch);
         for (int jetLoop = 0; jetLoop < numTCAjets; ++jetLoop)
         {
            CLPJet *jetPtr 
               = (CLPJet*) tfi.At(kJetBranch, jetLoop);
            cout << "  " << jetPtr->heavyFlavor();
            for (int typeInt = (int) CLPJet::kLoose;
                 typeInt < (int) CLPJet::kNumTagTypes;
                 ++typeInt)
            {
               CLPJet::SVXTagType type = (CLPJet::SVXTagType) typeInt;
               int tag = jetPtr->bTag (type);
               cout << " (" << setw(10) << tag << ", "
                    << Form("%5.3f", jetPtr->tagProb (type)) << ")";
            }
            cout << endl;
         
         } // for jetLoop
      } // if use jets

   } // for entryIndex
   return 0;
}

void initialize()
{
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_IntMap["howmany"]        = (void*) &gNumWanted;
   ns_BoolMap["pem"]           = (void*) &gUsePem;
   ns_BoolMap["electron"]      = (void*) &gUseElectrons;
   ns_BoolMap["muon"]          = (void*) &gUseMuons;
   ns_BoolMap["track"]         = (void*) &gUseTracks;
   ns_BoolMap["jet"]           = (void*) &gUseJets;
   ns_StringMap["file"]        = (void*) &gFileName;
}
