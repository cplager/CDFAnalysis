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
#include "KinematicFitter/KinematicFitter.hh"

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
// which entry to start at (start)
int gStartEntry    = 0;
// source file name (file)
string gFileName;
// source list name (list)
string gListName;

// Temporary, for debugging
CLPHEPGCont *gContPtr = 0;
int gKFmatches   = 0;
int gOldMatches  = 0;
int gSameMatches = 0;
int gHEPGmatches = 0;

// forward declarations
void initialize();
void calculateChi2ForCandidate (CLPCandidate &cand,
                                bool hepgMatchOnly = false);

int main(int argc, char** argv)
{
   initialize();
   parseArguments (argc, argv);
   cout << "numWanted " << gNumWanted << endl;

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

   if ( (0 == gFileName.length() && 0 == gListName.length()))
   {
      cout << "invalid value of which " << gWhich << endl
           << "Use 'which', 'file', 'list', and/or 'target' options."
           << endl;
      exit(0);
   }


   ////////////////////
   // Initialization //
   ////////////////////
   ////////////////
   // Input Tree //
   ////////////////
   TChain *chainPtr = (TChain*) 0;
   if (gFileName.length())
   {
      chainPtr = chainOneFile (gFileName, "CLPTree");
   } else {
      chainPtr = chainFromList (gListName, "CLPTree");
   }
   if (! chainPtr)
   {
      cerr << "Couldn't get a TChain from either " << gFileName
           << " or " << gListName << ". Aborting." << endl;
      return 0;                                                
   }
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
   CLPHEPGCont *contPtr = new CLPHEPGCont; // defined above
   // for debugging
   gContPtr = contPtr;
   // counters
   Int_t runNum;    
   Int_t eventNum;
   Int_t runSection;
   Int_t numElecs;
   Int_t numMuons;
   Int_t numTracks;
   Int_t numJets; 
   Double_t weight;
   // start hooking things up
   // tclones
   chainPtr->GetBranch("electrons")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("electrons", &elecTcaPtr);
   chainPtr->GetBranch("muons")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("muons", &muonTcaPtr);
   chainPtr->GetBranch("tracks")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("tracks", &trackTcaPtr);
   chainPtr->GetBranch("jets")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("jets", &jetTcaPtr);
   // simple variables (e.g., ints, doubles, etc)
   chainPtr->SetBranchAddress("runNum",   &runNum);
   chainPtr->SetBranchAddress("eventNum", &eventNum);
   chainPtr->SetBranchAddress("runSection", &runSection);
   chainPtr->SetBranchAddress("weight",   &weight);
   chainPtr->SetBranchAddress("numElecs", &numElecs);
   chainPtr->SetBranchAddress("numMuons", &numMuons);
   chainPtr->SetBranchAddress("numTracks", &numTracks);
   chainPtr->SetBranchAddress("numJets ", &numJets);

   Int_t nentries = Int_t(chainPtr->GetEntries());
   cout << "Possible entries: " << nentries << endl;
   if ((gNumWanted > 0) && (gNumWanted + gStartEntry < nentries))
   {
      nentries = gNumWanted + gStartEntry;    
   }
   cout << " We are looping over " << nentries << " events." << endl;
   int actualEntries = 0;
   CLPElectron::ElectronList elecList;
   CLPMuon::MuonList muonList;
   CLPTrack::TrackList trackList;
   CLPJet::JetList jetList;


   int numMatched = 0;
   int numComplete = 0;
   int numTCE = 0, numPHX = 0, numCMX = 0, numCMUP = 0;
   int topTCE = 0, topPHX = 0, topCMX = 0, topCMUP = 0;
   int elecProblems = 0, muonProblems = 0;
   for (Int_t entryIndex = gStartEntry; entryIndex < nentries; ++entryIndex) 
   {
      // check run
      chainPtr->GetEntry(entryIndex);
      ++actualEntries;

      //cout << endl << endl << "entry " << entryIndex 
      //     << " run " << runNum << " event " << eventNum << endl;
   
      ///////////////
      // Electrons //
      ///////////////
      elecList.clear();
      int numTCAelecs = elecTca.GetEntries();
      for (int elecLoop = 0; elecLoop < numTCAelecs; ++elecLoop)
      {
         CLPElectron *elecPtr = (CLPElectron*) elecTca.At(elecLoop);
         int topCode = CLPChargedParticle::kNone_CutCode;
         if (CLPChargedParticle::kTCEM_CutCode == elecPtr->cutCode())
         {
            ++topTCE;
            topCode = CLPChargedParticle::kTCEM_CutCode;
         } else if (CLPChargedParticle::kTPHX_CutCode == elecPtr->cutCode())
         {
            ++topPHX;
            topCode = CLPChargedParticle::kTPHX_CutCode;
         }
         int myCode = CLPChargedParticle::kNone_CutCode;
         if ( ! elecPtr->passesCuts())
         {
            if (CLPChargedParticle::kNone_CutCode != topCode)
            {
               if (CLPChargedParticle::kTPHX_CutCode == topCode)
               {
                  cout << endl << endl << "entry " << entryIndex 
                       << " run " << runNum << " event " << eventNum << endl;
                  CLPCuts::ElectronResult result = 
                     CLPCuts::electronCuts(*elecPtr);
                  CLPCuts::decodeElectronCuts (result);
                  cout << elecPtr->index() 
                       << " eta: " << elecPtr->eta() << endl
                       << " " << elecPtr->pes2dEta() << endl;
               }
               ++elecProblems;
            }
            continue;
         }
         if ( elecPtr->isaTCE() )
         {
            ++numTCE;
            myCode = CLPChargedParticle::kTCEM_CutCode;
         } else if ( elecPtr->isaPHX() )
         {
            ++numPHX;
            myCode = CLPChargedParticle::kTPHX_CutCode;
         }
         if (myCode != topCode)
         {
            ++elecProblems;
         }
      } // for elecLoop
   
      ///////////
      // Muons //
      ///////////
      muonList.clear();
      int numTCAmuons = muonTca.GetEntries();
      for (int muonLoop = 0; muonLoop < numTCAmuons; ++muonLoop)
      {
         CLPMuon *muonPtr = (CLPMuon*) muonTca.At(muonLoop);
         int topCode = CLPChargedParticle::kNone_CutCode;
         if (CLPChargedParticle::kTCMX_CutCode == muonPtr->cutCode())
         {
            ++topCMX;
            topCode = CLPChargedParticle::kTCMX_CutCode;
         } else if (CLPChargedParticle::kTCMUP_CutCode == muonPtr->cutCode())
         {
            ++topCMUP;
            topCode = CLPChargedParticle::kTCMUP_CutCode;
         }
         int myCode = CLPChargedParticle::kNone_CutCode;
         if ( ! muonPtr->passesCuts())
         {
            if (CLPChargedParticle::kNone_CutCode != topCode)
            {
               cout << endl << endl << "entry " << entryIndex 
                    << " run " << runNum << " event " << eventNum << endl;
               CLPCuts::MuonResult result = 
                  CLPCuts::muonCuts(*muonPtr);
               cout << "  Muon: " << muonPtr->index() << " ";
               CLPCuts::decodeMuonCuts (result);
               ++muonProblems;
            }
            continue;
         }
         if ( muonPtr->isaCMX() )
         {
            ++numCMX;
            myCode = CLPChargedParticle::kTCMX_CutCode;
         } else  if ( muonPtr->isaCMUP() )
         {
            ++numCMUP;
            myCode = CLPChargedParticle::kTCMUP_CutCode;
         } 
         if (myCode != topCode && myCode)
         {
            cout << endl << endl << "entry ME ONLY " << entryIndex 
                 << " run " << runNum << " event " << eventNum << endl
                 << "  rho " << muonPtr->COTexitRadius() << endl;
            ++muonProblems;
         }
      } // for muonLoop
   } // for entryIndex
   cout << "TCE  " << setw(4) << topTCE  << " " << setw(4) << numTCE  << endl;
   cout << "PHX  " << setw(4) << topPHX  << " " << setw(4) << numPHX  << endl;
   cout << "CMX  " << setw(4) << topCMX  << " " << setw(4) << numCMX  << endl;
   cout << "CMUP " << setw(4) << topCMUP << " " << setw(4) << numCMUP << endl;
   cout << "Problems " << elecProblems << " " << muonProblems << endl;
   return 0;
}

//////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////// //
// //    *****************     Subroutines      *****************    // //
// //////////////////////////////////////////////////////////////////// //
//////////////////////////////////////////////////////////////////////////


void initialize()
{
   // Int maps
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["num"]            = (void*) &gNumWanted;
   ns_IntMap["start"]          = (void*) &gStartEntry;
   // Bool maps
   // String maps
   ns_StringMap["file"]        = (void*) &gFileName;
   ns_StringMap["list"]        = (void*) &gListName;
}

