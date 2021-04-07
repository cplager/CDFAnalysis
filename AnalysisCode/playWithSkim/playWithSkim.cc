// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"

#include "time.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>
#include <algorithm>
#include <cstdlib>
#include "CLPClasses/CLPClasses.h"
#include "CLPClasses/dout.h"
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
// verbose (verbose)
bool gVerbose      = true;
// file name (file)
string gFilename;
// output name (output)
string gOutput = "output.root";

// forward declarations
void initialize();

int main(int argc, char** argv)
{
   initialize();
   parseArguments (argc, argv);
   cout << "## numWanted " << gNumWanted << endl;
   const int kNum = (int) CLPJet::kNumTagTypes;
   TH1F *hfTagHist [kNum];
   TH1F *hfUnHist  [kNum];
   TH1F *lfHist    [kNum];

   int hfTag = 0, hfUn = 0, lf = 0;

   const string kTaggingNames[kNum] =
   {
      "Loose",
      "Tight",
      "Ultra"
   };

   for (int loop = 0; loop < kNum; ++loop)
   {
      string hftName = Form ("hfTag%s", kTaggingNames[loop].c_str());
      string hfuName = Form ("hfUn%s",  kTaggingNames[loop].c_str());
      string lfName  = Form ("lf%s",    kTaggingNames[loop].c_str());
      hfTagHist[loop] = new TH1F (hftName.c_str(), hftName.c_str(), 50, 0, 1);
      hfUnHist[loop]  = new TH1F (hfuName.c_str(), hfuName.c_str(), 50, 0, 1);
      lfHist[loop]    = new TH1F (lfName.c_str(), lfName.c_str(), 50, 0, 1);
   } // for loop

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
   if (0 == gFilename.length())
   {
      cout << "invalid value of which " << gWhich << endl;
      exit(0);
   }


   ////////////////////
   // Initialization //
   ////////////////////


   TreeFileInterface tfi ("CLPTree");
   tfi.addFile (gFilename);
   if (gNumWanted)
   {
      tfi.setEventsWanted (gNumWanted);
   }

   tfi.addBranch < int >    ( kEventBranch   );
   tfi.addBranch < int >    ( kRunBranch     );
   tfi.addBranch < int >    ( kSectionBranch );
   tfi.addBranch < int >    ( "numVtxs"      );
   tfi.addBranch < double > ( kWeightBranch  );
   tfi.addTcaBranch ( kElectronBranch, kElectronClassName );
   tfi.addTcaBranch ( kMuonBranch    , kMuonClassName     );
   tfi.addTcaBranch ( kJetBranch     , kJetClassName      );
   tfi.addTcaBranch ( kTrackBranch   , kTrackClassName    );
   

   cout << "Here we go" << endl;
   while (tfi.getNextEvent())
   {
      // check run
      if (gVerbose)
         cout << endl << endl 
              << " " << kRunBranch << " "  << tfi.value< int > (kRunBranch) 
              << " " << kEventBranch << " " << tfi.value< int > (kEventBranch)
              << " weight " << tfi.value< double > (kWeightBranch)
              << " numVtxs " << tfi.value< int > ("numVtxs")
              << " " << kSectionBranch << " " 
              << tfi.value< int >(kSectionBranch)
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
            int heavyFlavor = jetPtr->heavyFlavor();
            if (gVerbose)  cout << "  " << heavyFlavor;
            for (int typeInt = (int) CLPJet::kLoose;
                 typeInt < (int) CLPJet::kNumTagTypes;
                 ++typeInt)
            {
               CLPJet::SVXTagType type = (CLPJet::SVXTagType) typeInt;
               TH1F *histPtr = 0;
               if (heavyFlavor)
               {
                  if (jetPtr->isBTagged (type))
                  {
                     // tagged HF
                     histPtr = hfTagHist[typeInt];
                     ++hfTag;
                  } else {
                     // UNtagged hf
                     histPtr = hfUnHist[typeInt];
                     ++hfUn;
                  }
               } else {
                  // light flavor
                  histPtr = lfHist[typeInt];
                  ++lf;
               }
               assert (histPtr);
               histPtr->Fill ( jetPtr->tagProb (type) );
               int tag = jetPtr->bTag (type);
               if (gVerbose) 
                  cout << " (" << setw(10) << tag << ", "
                       << Form("%5.3f", jetPtr->tagProb (type)) << ")";
            }
            if (gVerbose) cout << endl;
         
         } // for jetLoop
      } // if use jets
   } // for entryIndex

   if (gVerbose) cout << "hfTag " << hfTag 
                      << " hfUn " << hfUn << " lf " << lf << endl;

   TFile *outputPtr = TFile::Open (gOutput.c_str(), "recreate");
   if (! outputPtr)
   {
      cerr << "Crap" << endl;
   }
   for (int loop = 0; loop < kNum; ++loop)
   {
      hfTagHist[loop]->Write();
      hfUnHist[loop]->Write(); 
      lfHist[loop]->Write();   
   } // for loop

   return 0;
}

void initialize()
{
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["num"]            = (void*) &gNumWanted;
   ns_BoolMap["pem"]           = (void*) &gUsePem;
   ns_BoolMap["electron"]      = (void*) &gUseElectrons;
   ns_BoolMap["muon"]          = (void*) &gUseMuons;
   ns_BoolMap["track"]         = (void*) &gUseTracks;
   ns_BoolMap["jet"]           = (void*) &gUseJets;
   ns_BoolMap["verbose"]       = (void*) &gVerbose;
   ns_StringMap["file"]        = (void*) &gFilename;
   ns_StringMap["output"]      = (void*) &gOutput;
}
