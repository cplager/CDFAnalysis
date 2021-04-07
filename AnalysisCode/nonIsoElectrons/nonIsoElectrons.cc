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

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;


// Global Variables (ugh..) 
// Note, name in parenthesis is command-line variable name.
// e.g. 'which=12 usetracks=1' will set gWhich = 12 and gUseTracks = false

// Which file to grab (which)
int gWhich         = 11;
// number of events (numwanted/howmany)
int gNumWanted     = 10;
// Use tracks as second Z leg (usetracks)
bool gUseTracks    = true;
// only keep candidates with Z mass (76,106) and 4 jets (tightzjets)
bool gTightZjets   = true;
// don't check charge (noq)
bool gNoQ          = false;
// use all tracks regardless of whether it is matched to a lepton or
// not (usealltracks)
bool gUseAllTracks = false;
// Do HEPG only processing (hepgonly)
bool gHepgOnly     = false;
// use signal veto (vetosignal)
bool gVetoSignal   = false;
// use run and event list debugging (runevent)
bool gRunEvent     = false;
// only accept candidates with at least one loose secvtx tag (btag)
bool gBtagOnly     = false;
// allow PEM electrons (pem)
bool gUsePem       = false;

// forward declarations
void initialize();

int main(int argc, char** argv)
{
   initialize();
   parseArguments (argc, argv);
   SelectRunEventClass select("runevent.list");
   cout << "numWanted " << gNumWanted << endl;
   //CLP3Vector::setOutput3Mode( CLP3Vector::kStandardOutputMode);
   CLPCandidate::setOutputType (CLPCandidate::kEverything);
   enum {
      kInput = 0,
      kOutput,
   };

   string sourceName;
   string targetName;   
   if (11 == gWhich) 
   {
      // all signal
      sourceName = "/cdf/scratch/cplager/fcnc/all_signal_nt5.root";
      targetName = "noniso.root";
   } 
   else 
   {
      cout << "invalid value of which " << gWhich << endl;
      exit(0);
   }


   ////////////////////
   // Initialization //
   ////////////////////
   cout << "From: " << sourceName << endl
        << "To:   " << targetName << endl;

   // skim file
   TFile file   ( sourceName.c_str() );
   TFile target ( targetName.c_str(), "RECREATE" ); 

   ////////////////
   // Input Tree //
   ////////////////
   // setup my variables
   // TClones stuff
   TClonesArray *elecTcaPtr = new TClonesArray("CLPElectron", 10);
   TClonesArray *muonTcaPtr = new TClonesArray("CLPMuon", 10);
   TClonesArray *trackTcaPtr = new TClonesArray("CLPTrack", 10);
   TClonesArray *jetTcaPtr  = new TClonesArray("CLPJet", 10);   
   TClonesArray &elecTca = *elecTcaPtr;
   TClonesArray &muonTca = *muonTcaPtr;
   TClonesArray &trackTca = *trackTcaPtr;
   TClonesArray &jetTca  = *jetTcaPtr;
   // counters
   Int_t runNum;    
   Int_t eventNum;  
   Int_t numElecs;
   Int_t numMuons;
   Int_t numTracks;
   Int_t numJets; 
   Double_t weight;
   // find the tree
   TTree *tree = (TTree*) file.Get("CLPTree");
   if (0 == tree)
   {
      // something bad has happened
      cerr << "Can't get CLPTree.  Sorry, eh." << endl;
      return 0;
   }
   // start hooking things up
   // tclones
   tree->GetBranch("electrons")->SetAutoDelete(false);
   tree->SetBranchAddress("electrons", &elecTcaPtr);
   tree->GetBranch("muons")->SetAutoDelete(false);
   tree->SetBranchAddress("muons", &muonTcaPtr);
   tree->GetBranch("tracks")->SetAutoDelete(false);
   tree->SetBranchAddress("tracks", &trackTcaPtr);
   tree->GetBranch("jets")->SetAutoDelete(false);
   tree->SetBranchAddress("jets", &jetTcaPtr);
   // simple variables (e.g., ints, doubles, etc)
   tree->SetBranchAddress("runNum",   &runNum);
   tree->SetBranchAddress("eventNum", &eventNum);
   tree->SetBranchAddress("weight",   &weight);
   tree->SetBranchAddress("numElecs", &numElecs);
   tree->SetBranchAddress("numMuons", &numMuons);
   tree->SetBranchAddress("numTracks", &numTracks);
   tree->SetBranchAddress("numJets ", &numJets);

   TH1F jetHist ("numjets", "Number of Jets", 8, -0.5, 7.5);
   TH1F nonIsoHist ("numnoniso", "Number of Isolated Electrons", 4, -0.5, 3.5);
   TH1F collisionsHist ("collisions", "Number of Non-Iso-Jet Collisions",
                        4, -0.5, 3.5);
   TH1F matchHist ("match", "HEPG Match ID", 12, -1.5, 10.5);

   Int_t nentries = Int_t(tree->GetEntriesFast());
   cout << "Possible entries: " << nentries << endl;
   if ((gNumWanted > 0) && (gNumWanted < nentries))
   {
      nentries = gNumWanted;    
   }
   cout << " We are looping over " << nentries << " events." << endl;
   int actualEntries = 0;
   CLPElectron::ElectronList elecList;
   CLPJet::JetList jetList;

   CLPCuts::ElectronResult nonIsolated;
   nonIsolated.set(CLPCuts::kIsolation_elec);

   int totalMatched = 0, totalRejected = 0;
   for (Int_t entryIndex = 0; entryIndex < nentries; ++entryIndex) 
   {
      // check run
      tree->GetEntry(entryIndex);

      if (++actualEntries % 10000 == 0) cout << actualEntries << endl;
   
      ///////////////
      // Electrons //
      ///////////////
      elecList.clear();
      int numMatched = 0;
      bool plusHepg = false, minusHepg = false;
      int numTCAelecs = elecTca.GetEntriesFast();
      if (numTCAelecs != numElecs)
      {
         cout << "electrons " << numTCAelecs << " != "
              << numElecs << endl;
      }
      for (int elecLoop = 0; elecLoop < numTCAelecs; ++elecLoop)
      {
         CLPElectron *elecPtr = (CLPElectron*) elecTca.At(elecLoop);
         if ( elecPtr->isaTCE() || elecPtr->isaPHX() || 
              ( gUsePem && elecPtr->isaPEM() ) )
         {
            // Don't bother if the non-isolated lepton is from the Z
            if (elecPtr->isTagged())
            {
               ++numMatched;
               if (elecPtr->isaTCE())
               {
                  if (1 == elecPtr->sign())
                  {
                     plusHepg = true;
                  } else {
                     minusHepg = true;
                  }
               }
               continue;
            }
            CLPCuts::ElectronResult result = CLPCuts::electronCuts(*elecPtr);
            // did this fail the isolation cut
            if (result == nonIsolated)
            {
               
               elecList.push_back( *elecPtr );
            } 
         } // is a non-isolated electron
      } // for elecLoop
      nonIsoHist.Fill( (int)elecList.size() );

      if (! numMatched)
      {
         continue;
      }

      // if we don't have two HEPG matches, we need to do the tracks
      if (1 == numMatched)
      {
         int numTCAtracks = trackTca.GetEntriesFast();
         for (int trackLoop = 0; trackLoop < numTCAtracks; ++trackLoop)
         {
            CLPTrack *trackPtr = (CLPTrack*) trackTca.At(trackLoop);
            if (1 == trackPtr->sign())
            {
               if (! plusHepg)
               {
                  plusHepg = true;
                  ++numMatched;
                  break;
               }
            } else {
               if (! minusHepg)
               {
                  minusHepg = true;
                  ++numMatched;
                  break;
               }
            }
         } // for trackLoop
      }

      //////////
      // Jets //
      //////////
      int hepgJet = 0;
      jetList.clear();
      int numTCAjets = jetTca.GetEntriesFast();
      if (numTCAjets != numJets)
      {
         cout << "jets " << numTCAjets << " != "
              << numJets << endl;
      }
      for (int jetLoop = 0; jetLoop < numTCAjets; ++jetLoop)
      {
         CLPJet *jetPtr = (CLPJet*) jetTca.At(jetLoop);
         CLPCuts::JetResult result = CLPCuts::jetCuts( *jetPtr);
         if (jetPtr->isTagged())
         {
            ++hepgJet;
         }
         if ( result.none() )
         {
            jetList.push_back( *jetPtr );
         }
      } // for jetLoop
      jetHist.Fill( (int)jetList.size() );

      /////////////////////////////////
      // Study Jet-Electron Overlaps //
      /////////////////////////////////
      bool allMatched = false;
      if (numMatched >= 2 && hepgJet >= 4)
      {
         allMatched = true;
         ++totalMatched;
      }
      int collisions = 0;
      int jetSize = (int) jetList.size();
      int elecSize = (int) elecList.size();
      for (int jetIndex = 0; jetIndex < jetSize; ++jetIndex)
      {
         CLP3Vector jetVec = jetList[jetIndex].mom3Vec();
         for (int elecIndex = 0; elecIndex < elecSize; ++elecIndex)
         {
            CLP3Vector elecVec = elecList[elecIndex].mom3Vec();
            if ( jetVec.etaPhiDiff( elecVec ) < 0.4)
            {
               // it's too close to a elec
               ++collisions;
               if (allMatched)
               {
                  ++totalRejected;
               }
               matchHist.Fill ( jetList[jetIndex].taggedToHEPGIndex() );
            } // if within 0.4
         } // for elecIndex
      } // for jetIndex
      collisionsHist.Fill(collisions);
   } // for entryIndex
   target.Write();
   cout << "totalMatched  " << totalMatched << endl
        << "totalRejected " << totalRejected << endl;
   return 0;
}

void initialize()
{
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_IntMap["howmany"]        = (void*) &gNumWanted;
   ns_BoolMap["usetracks"]     = (void*) &gUseTracks;
   ns_BoolMap["usealltracks"]  = (void*) &gUseAllTracks;
   ns_BoolMap["tightzjets"]    = (void*) &gTightZjets;
   ns_BoolMap["noq"]           = (void*) &gNoQ;
   ns_BoolMap["hepgonly"]      = (void*) &gHepgOnly;
   ns_BoolMap["vetosignal"]    = (void*) &gVetoSignal;
   ns_BoolMap["runevent"]      = (void*) &gRunEvent;
   ns_BoolMap["btag"]          = (void*) &gBtagOnly;
   ns_BoolMap["pem"]           = (void*) &gUsePem;
}
