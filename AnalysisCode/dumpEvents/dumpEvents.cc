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
#include <stdlib.h>
#include "CLPClasses/CLPClasses.h"
#include "AnalysisNamespace/AnalysisNamespace.hh"
#include "RootFileCollection/TreeFileInterface.h"
#include "RootFileCollection/TopTreeNamespace.h"
#include "RootFileCollection/OptionUtils.h"
using namespace std;
using namespace CLPFunctions;
using namespace TopTreeNamespace;
using namespace AnalysisNamespace;
using namespace CLPNamespace;

// Global Variables (ugh..) 
// Note, name in parenthesis is command-line variable name.
// e.g. 'which=12 usetracks=1' will set gWhich = 12 and gUseTracks = false

// Which file to grab (which)
int gWhich         = 0;
// number of events (numwanted/howmany)
int gNumWanted     = 10;
// show electrons (electron)
bool gShowElectrons = false; 
// show muons (muon)
bool gShowMuons = false;
// show tracks (track)
bool gShowTracks = false;
// show only good leptons (onlygood)
bool gOnlyGoodLeptons = false;
// show jets (jet)
bool gShowJets = false;
// scan hepg bank for different decays
bool gScanHepg = false;
// show hepg (hepg)
bool gShowHepg = false;
// check heavy flavor (heavy)
bool gHeavy = false;
// treat as heavy flavor sample (isheavy)
bool gIsHeavyFlavorSample = false;
// debugging (debug)
bool gDebugMode = false;
// use entry index instead of run/event number (entry)
bool gUseEntry = false;
// print reconstructed only if matched (ifmatched)
bool gOnlyIfMatched = false;
// show all leptons (lepton)
bool gShowAllLeptons = false;
// show all entries (showall)
bool gShowAllEntries = true;

// forward declarations
void localInitialize();

int main(int argc, char** argv)
{
   // parce command line arguaments
   localInitialize();     // hook up local options
   setupDefaultOptions(); // hook up standard options
   OptionUtils::parseArguments (argc, argv);
   setupTFI ("CLPTree");

   if (gShowAllLeptons)
   {
      gShowTracks = gShowElectrons = gShowMuons = true;
   }

   cout << "addding" 
        << " " << CLPNamespace::kElectronBranch
        << " " << CLPNamespace::kMuonBranch    
        << " " << CLPNamespace::kTrackBranch   
        << " " << CLPNamespace::kJetBranch     
        << endl;
   ns_tfi.addTcaBranch ( CLPNamespace::kElectronBranch, kElectronClassName );
   ns_tfi.addTcaBranch ( CLPNamespace::kMuonBranch    , kMuonClassName     );
   ns_tfi.addTcaBranch ( CLPNamespace::kJetBranch     , kJetClassName      );
   ns_tfi.addTcaBranch ( CLPNamespace::kTrackBranch   , kTrackClassName    );
   if (gShowHepg || gScanHepg)
   {
      ns_tfi.addClassBranch< CLPHEPGCont > (CLPNamespace::kHepgBranch );
   }
   ns_tfi.addBranch< int > (kRunBranch);
   ns_tfi.addBranch< int > (kEventBranch);
   if (gHeavy)
   {
      ns_tfi.addBranch< unsigned int > (kHeavyFlavorBranch);
   }

   ///////////////////////
   // Set Defaults Here //
   ///////////////////////   
   CLPJet::setDefaultCorrectionType (CLPJet::kLevel5);
   CLP3Vector::setOutput3Mode( CLP3Vector::kPtEtaPhiOutputMode );
   //CLP3Vector::setOutput3Mode( CLP3Vector::kStandardOutputMode);
   CLPFunctions::setOutputMode(kStandardOutput);
   SelectRunEventClass select("interesting.list");
   map< string, int > hepgClassifyMap;
   int numRejected = 0;

   // Do any final TFI setup that needs to be done after setting the
   // branches, etc (e.g. skipping the first N events).  This should
   // be the last thing right before the 'while
   // (ns_tfi.getNextEvent())'
   finalTFIsetup();

   int entryIndex = -1;
   while ( ns_tfi.getNextEvent() )
   {
      ++entryIndex;
      ns_runNumber   = ns_tfi.value< int > (kRunBranch);
      ns_eventNumber = ns_tfi.value< int > (kEventBranch);
      if (! gShowAllEntries)
      {
         if (gUseEntry)
         {
            if (! select.isSelected (entryIndex))
            {
               continue;
            }
         } else {
            if (! select.isSelected (ns_runNumber, ns_eventNumber))
            {
               continue;
            }
         }
      } // ! show all entries
      if (gShowElectrons || gShowMuons || gShowTracks || 
          gShowJets || gShowHepg)
      {          
         cout << endl << endl << "entryIndex " << entryIndex
              << " run " << ns_runNumber 
              << " event " << ns_eventNumber << endl;
      }
      // declare lists, etc
      CLPElectron::ElectronList elecList;
      CLPMuon::MuonList muonList;
      CLPTrack::TrackList trackList;
      CLPCandidate::CandidateList candList;
   
      ///////////////
      // Electrons //
      ///////////////
      if (gShowElectrons)
      {
         int numelecs = ns_tfi.GetEntries (CLPNamespace::kElectronBranch);
         cout << "electrons: " << numelecs << endl;
         for (int elecLoop = 0; elecLoop < numelecs; ++elecLoop)
         {
            CLPElectron *elecPtr = 
               (CLPElectron*) ns_tfi.At (CLPNamespace::kElectronBranch, 
                                         elecLoop);
            if (gOnlyIfMatched && ! elecPtr->isTagged())
            {
               continue;
            }
            if (! elecPtr->isaTCE() && !elecPtr->isaPHX())
            {
               continue;
            }
            CLPCuts::ElectronResult result = CLPCuts::electronCuts(*elecPtr);
            if (! gOnlyGoodLeptons || result.none())
            {
               cout << setw(2) << elecLoop << ") " 
                    << * ( (CLPObject*) elecPtr )
                    << endl << "  ";
               CLPCuts::decodeElectronCuts (result);
               cout << endl;
            }
            // we need to make an electron list so that we can tell which
            // tracks are matched to electrons we are using.
            if (result.none())
            {
               elecList.push_back (*elecPtr);
            } // add electron to the list
         } // for elecLoop
      } // if show electrons
   
      ///////////
      // Muons //
      ///////////
      if (gShowMuons)
      {
         int nummuons = ns_tfi.GetEntries (CLPNamespace::kMuonBranch);
         cout << endl << "muons: " << nummuons <<  endl;
         for (int muonLoop = 0; muonLoop < nummuons; ++muonLoop)
         {
            CLPMuon *muonPtr = 
               (CLPMuon*) ns_tfi.At (CLPNamespace::kMuonBranch, muonLoop); 
            //if ( ! muonPtr->isaTightLepton() )
            //{
            //   continue;
            //} // not a CMIO
            if (gOnlyIfMatched && ! muonPtr->isTagged())
            {
               continue;
            }
            CLPCuts::MuonResult result = CLPCuts::muonCuts(*muonPtr);
            if (! gOnlyGoodLeptons || result.none())
            {
               cout << setw(2) << muonLoop << ") " << * ( (CLPObject*) muonPtr )
                    << endl << "  ";
               CLPCuts::decodeMuonCuts (result);
               //muonPtr->calcQuantities();
               cout << endl;
            }
            // we need to make an muon list so that we can tell which
            // tracks are matched to muons we are using.
            if (result.none() && muonPtr->isaTightLepton())
            {
               muonList.push_back (*muonPtr);
            }
         } // for muonLoop
      } // if show muons
   
      ////////////
      // Tracks //
      ////////////
      int numtracks = ns_tfi.GetEntries (CLPNamespace::kTrackBranch);
      if (gShowTracks) cout << endl << "tracks: " << numtracks << endl;
      // first make looseTrackList vector
      for (int trackLoop = 0; trackLoop < numtracks; ++trackLoop)
      {
         CLPTrack *trackPtr = 
            (CLPTrack*) ns_tfi.At (CLPNamespace::kTrackBranch, trackLoop); 
         if (gOnlyIfMatched && ! trackPtr->isTagged())
         {
            continue;
         }
         CLPCuts::TrackResult result = CLPCuts::trackCuts(*trackPtr);
         if (gShowTracks) {
            if (result.none() || !gOnlyGoodLeptons)
            {
               cout << setw(2) << trackLoop << ") " 
                    << * ( (CLPObject*) trackPtr )
                    << endl 
                    << "  ";
               CLPCuts::decodeTrackCuts (result);
            } // show all tracks
         } // show tracks
         if (result.none())
         {
            trackList.push_back( *trackPtr );
         }
      } // for trackLoop

      //////////
      // Jets //
      //////////
      if (gShowJets)
      {
         int numjets = ns_tfi.GetEntries (CLPNamespace::kJetBranch);
         cout << endl << "Jets: " << numjets << endl;
         for (int jetLoop = 0; jetLoop < numjets; ++jetLoop)
         {
            CLPJet *jetPtr = 
               (CLPJet*) ns_tfi.At (CLPNamespace::kJetBranch, jetLoop); 
            if (gOnlyIfMatched && ! jetPtr->isTagged())
            {
               continue;
            }
            cout << setw(2) << jetLoop << ") " << * ( (CLPObject*) jetPtr ) 
                 << endl << "  ";
            int matched = -1;
            CLP3Vector jetVec = jetPtr->mom3Vec();         
            // Is this jet matched to a track?
            for (int trkIndex = 0; trkIndex < (int) trackList.size(); 
                 ++trkIndex)
            {
               CLP3Vector trkVec = trackList[trkIndex].mom3Vec();
               if ( jetVec.etaPhiDiff( trkVec ) < 0.4)
               {
                  // it's too close to a track
                  matched = trkIndex;
                  break;
               }
            } // for trkIndex
            CLPCuts::JetResult result = CLPCuts::jetCuts(*jetPtr);
            CLPCuts::decodeJetCuts (result);
            if (1 + matched)
            {
               cout << "  ** matched to track " << matched << " " << endl;
            }
         } // for jetLoop
      } // if gShowJets

      //////////
      // HEPG //
      //////////
      if (gShowHepg || gScanHepg)
      {
         CLPHEPGCont *contPtr = 
            ns_tfi.classPtr< CLPHEPGCont > (CLPNamespace::kHepgBranch);
         string classify = contPtr->classify();
         ++hepgClassifyMap[classify];
         if (gShowHepg)
         {            
            cout << endl << "HEPG:" << endl << *contPtr << classify << endl;
         }
      }

      if (gHeavy)
      {
         ns_debug = true;
         unsigned int heavyFlavorWord = 
            ns_tfi.value< unsigned int > (kHeavyFlavorBranch);
         acceptableHeavyFlavorContent (heavyFlavorWord,
                                       gIsHeavyFlavorSample,
                                       numRejected);
         // cout << endl << endl << "entryIndex " << entryIndex
         //      << " run " << ns_runNumber 
         //      << " event " << ns_eventNumber << endl;
         // decodeHeavyFlavorWord (heavyFlavorWord);
      }

   } // while get next event
   if (gShowHepg || gScanHepg)
   {
      for (map< string, int >::const_iterator iter = hepgClassifyMap.begin();
           hepgClassifyMap.end() != iter;
           ++iter)
      {
         cout << Form ("%14s : %5d",iter->first.c_str(),  iter->second)
              << endl;
      } // for iter
   }
   if (gHeavy)
   {
      cout << "rejected " << numRejected 
           << " out of " << entryIndex + 1 << endl;
   }
   return 0;
}


void localInitialize()
{
   OptionUtils::addOptionKey ("num",       gNumWanted);
   OptionUtils::addOptionKey ("electron",  gShowElectrons);
   OptionUtils::addOptionKey ("muon",      gShowMuons);
   OptionUtils::addOptionKey ("track",     gShowTracks);
   OptionUtils::addOptionKey ("onlygood",  gOnlyGoodLeptons);
   OptionUtils::addOptionKey ("jet",       gShowJets);
   OptionUtils::addOptionKey ("hepg",      gShowHepg);
   OptionUtils::addOptionKey ("scanhepg",  gScanHepg);
   OptionUtils::addOptionKey ("heavy",     gHeavy);
   OptionUtils::addOptionKey ("isheavy",   gIsHeavyFlavorSample);
   OptionUtils::addOptionKey ("entry",     gUseEntry);
   OptionUtils::addOptionKey ("showall",   gShowAllEntries);
   OptionUtils::addOptionKey ("ifmatched", gOnlyIfMatched);   
   OptionUtils::addOptionKey ("lepton",    gShowAllLeptons);
}

