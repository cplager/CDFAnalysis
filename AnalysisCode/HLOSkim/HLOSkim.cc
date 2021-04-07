// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TChain.h"
#include "TTree.h"

#include "time.h"
#include "CLPClasses/dout.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include "CLPClasses/CLPClasses.h"
#include "HighLevelObjects/HighLevelObjects.hh"
#include "JetUser/JetEnergyCorrections.hh"
#include "SkimUtils.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;


// Jet Correction Constants
const Int_t kJCLevel_five = 5;
const Int_t kJCLevel_seven = 7;
const Int_t kJCConeSize = 0;
const Int_t kJCVersion = 5;
const Int_t kJCSysCode = 0;
const Int_t kJCiMode_Data = 1;
const Int_t kJCiMode_MC = 0;

// Polluting the gLobal namespace - gLobal variables
// Note, name in parenthesis is command-line variable name.
// e.g. 'which=12 usetracks=1' will set gWhich = 12 and gUseTracks = false

// Which file to grab (which)
int gWhich         = 0;
// number of events (numwanted/howmany)
int gNumWanted     = 10;
// input list of root files (list)
string gListName;
// input root file (file) - instead of list 
string gFileName;
// output root filename (output)
string gOutputName;

// Ints for bools:
//  1 -> true
//  0 -> false
// -1 -> leave as default
// turn on signal veto (veto)
int gSignalVeto = -1; // negative means do not overwride
// require 4 or more good jets (fourjets)
int gRequire4jets = -1;
// require good Z candidate (goodz)
int gRequireGoodZ = -1;


// forward declarations
void initialize();

int main(int argc, char** argv)
{
   ////////////////////////////////////////////////////////
   // ////////////////////////////////////////////////// //
   // // //////////////////////////////////////////// // //
   // // // Parse Command Line Arguments           // // //
   // // // Decide Which Files and How Many Events // // //
   // // //////////////////////////////////////////// // //
   // ////////////////////////////////////////////////// //
   ////////////////////////////////////////////////////////
   initialize();
   parseArguments (argc, argv);
   // boolean variables (I should move these to global variables so I
   // can control them from the command line)
   bool debug           = false;
   bool isSignalMC      = false;
   bool variableWeights = false;
   bool require4jets    = false;
   bool matchHepgZs     = false;
   bool requireGoodZ    = false;
   bool signalVeto      = false;
   // set skim name
   CLPFunctions::setDataType( CLPNamespace::kMC ); // MC by default
   if (0 == gWhich)
   {
      // we aren't going to do anything because we are getting the
      // names from the command line.
   } else if (11 == gWhich)
   {
      // all signal via rootd
      isSignalMC = true; // signal MC
      gOutputName = "/cdf/scratch/cplager/fcnc/clpskim_fcnc";
      gListName = "lists/all_signal_rootd.list";
   } else if (16 == gWhich)
   {
      // Alpgen + Herwig V2
      variableWeights = true; // weights according to filename
      matchHepgZs = true; // find and match HEPG Zs
      gOutputName =  "/cdf/scratch/cplager/fcnc/clpskim_znjet_alpher2";
      gListName = "lists/Znjet_alpher2.list";
   } else if (51 == gWhich)
   {
      // Ingyin's flat SM ttbar
      isSignalMC = true; // signal MC
      gOutputName =  "/cdf/scratch/cplager/fcnc/clpskim_flat_smttbar";
      gListName = "lists/flat_smttbar.list";
   } else if (201 == gWhich)
   {
      // Electron data
      CLPFunctions::setDataType( CLPNamespace::kData ); // Data, not MC
      requireGoodZ = true; // require good Z
      signalVeto = true; // signal veto
      gOutputName =  "/cdf/scratch/cplager/fcnc/clpskim_bhel0d";
      gListName = "lists/bhel0d.list";
   } else if (202 == gWhich)
   {
      // Muon data
      CLPFunctions::setDataType( CLPNamespace::kData ); // Data, not MC
      requireGoodZ = true; // require good Z
      signalVeto = true; // signal veto
      gOutputName =  "/cdf/scratch/cplager/fcnc/clpskim_bhmu0d";
      gListName = "lists/bhmu0d.list";
   }

   // Are we set?
   if ((0 == gListName.length() && 0 == gFileName.length()) || 
       0 == gOutputName.length())
   {
      // we have to have an input list and an output root file
      cerr << "You must specify both an input list and output root file." 
           << endl << "Use 'list=bla.list output=bla' arguments."
           << endl;
      exit(0);
   }

   ////////////////////////
   // Override Defaults? //
   ////////////////////////
   if (gSignalVeto >= 0)
   {
      if (gSignalVeto)
      {
         signalVeto = true;
      } else {
         signalVeto = false;
      }
   } // override signal veto flag
   if (gRequire4jets >= 0)
   {
      if (gRequire4jets)
      {
         require4jets = true;
      } else {
         require4jets = false;
      }
   } // override signal veto flag
   if (gRequireGoodZ >= 0)
   {
      if (gRequireGoodZ)
      {
         requireGoodZ = true;
      } else {
         requireGoodZ = false;
      }
   } // override signal veto flag

   // Z finding requirements
   ns_mZc_tightZ    = false;
   ns_mZc_noQ       = false;
   ns_mZc_useTracks = true;

   Int_t imode;
   if (CLPFunctions::isData())
   {
      imode = kJCiMode_Data;
      gOutputName += "_data";
      cout << "Skiming as DATA" << endl;
   } else {
      imode = kJCiMode_MC;
      gOutputName += "_mc";
      cout << "Skiming as MC" << endl;
   }
   cout << "imode " << imode << endl;

   // modify the output root file appropriately
   if (require4jets)
   {
      gOutputName += "_4j";
   }
   if (requireGoodZ)
   {
      gOutputName += "_goodZ";
   }
   if (signalVeto)
   {
      gOutputName += Form ("_nosig%d-%.0f", ns_veto_jetCount, ns_veto_jetEt);
   }
   if (gNumWanted)
   {
      gOutputName += Form ("_%d", gNumWanted);
   }

   gOutputName += ".root";
   
   CLPFunctions::setUserString(0, gOutputName);

   cout << "NumWanted " << gNumWanted << endl << "list " << gListName << endl;


   ////////////////////////////////////////
   // ////////////////////////////////// //
   // // //////////////////////////// // //
   // // // Setup Input Tree Chain // // //
   // // //////////////////////////// // //
   // ////////////////////////////////// //
   ////////////////////////////////////////
   TChain *chainPtr;
   if (gListName.length()) 
   {
      chainPtr = chainFromList ( gListName.c_str() );
   } else {
      chainPtr = chainOneFile  ( gFileName.c_str() );
   }
   if (0 == chainPtr)
   {
      // something bad has happened
      cerr << "Can't get TopTree.  Sorry, eh." << endl;
      return 0;
   }

   // start hooking things up
   vector< TString > branchNameVec;

   // evt
   TClonesArray *evtTcaPtr = new TClonesArray("evt", 1);
   chainPtr->GetBranch("evt")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("evt", &evtTcaPtr);
   branchNameVec.push_back("evt");

   // summary
   TClonesArray *summaryTcaPtr = new TClonesArray("summary", 1);
   chainPtr->GetBranch("summary")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("summary", &summaryTcaPtr);
   branchNameVec.push_back("summary");

   // zvtxs
   TClonesArray *zvtxsTcaPtr = new TClonesArray("zvtxs", 1);
   chainPtr->GetBranch("zvtxs")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("zvtxs", &zvtxsTcaPtr);
   branchNameVec.push_back("zvtxs");

   // hepg
   TClonesArray *hepgTcaPtr = new TClonesArray("hepg", 1);
   chainPtr->GetBranch("hepg")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("hepg", &hepgTcaPtr);
   branchNameVec.push_back("hepg");

   // electron
   TClonesArray *electronTcaPtr = new TClonesArray("electron", 1);
   chainPtr->GetBranch("electron")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("electron", &electronTcaPtr);
   branchNameVec.push_back("electron");

   // muon
   TClonesArray *muonTcaPtr = new TClonesArray("muon", 1);
   chainPtr->GetBranch("muon")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("muon", &muonTcaPtr);
   branchNameVec.push_back("muon");

   // offltrack
   TClonesArray *offltrackTcaPtr = new TClonesArray("offltrack", 1);
   chainPtr->GetBranch("offltrack")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("offltrack", &offltrackTcaPtr);
   branchNameVec.push_back("offltrack");

   // jet
   TClonesArray *jetTcaPtr = new TClonesArray("jet", 1);
   chainPtr->GetBranch("jet")->SetAutoDelete(false);
   chainPtr->SetBranchAddress("jet", &jetTcaPtr);
   branchNameVec.push_back("jet");

   ///////////////////////////////////
   // ///////////////////////////// //
   // // /////////////////////// // //
   // // // Setup Output Tree // // //
   // // /////////////////////// // //
   // ///////////////////////////// //
   ///////////////////////////////////

   // TCloneArrays
   TClonesArray *elecCLPTcaPtr  = 
      new TClonesArray(CLPNamespace::kElectronClassName, 20);
   TClonesArray *muonCLPTcaPtr  = 
      new TClonesArray(CLPNamespace::kMuonClassName, 20);
   TClonesArray *trackCLPTcaPtr  = 
      new TClonesArray(CLPNamespace::kTrackClassName, 20);
   TClonesArray *jetCLPTcaPtr   = 
      new TClonesArray(CLPNamespace::kJetClassName,  20);
   TClonesArray &elecCLPTca  = *elecCLPTcaPtr;
   TClonesArray &muonCLPTca  = *muonCLPTcaPtr;
   TClonesArray &trackCLPTca = *trackCLPTcaPtr;
   TClonesArray &jetCLPTca   = *jetCLPTcaPtr;
   elecCLPTca.BypassStreamer(true);
   muonCLPTca.BypassStreamer(true);
   trackCLPTca.BypassStreamer(true);
   jetCLPTca.BypassStreamer(true);
   // Various counters
   Int_t runNum;    
   Int_t eventNum;
   Int_t runSection;
   Int_t numVtxs;
   Int_t entryIndex;
   Double_t weight = 1;
   Int_t numElecs;
   Int_t numMuons;
   Int_t numTracks;
   Int_t numJets; 
   // single variables
   CLPHEPGCont *contPtr = new CLPHEPGCont;

   // open file for output
   cout << "Skimming to " << gOutputName << endl;

   TFile *filePtr = new TFile (gOutputName.c_str(), "RECREATE");
   // To make sure the file is open and so g++ stops complaining that
   // I don't use this variable
   if (! filePtr->IsOpen())
   {
      cerr << "Did not successfully open " << gOutputName 
           << " for output.  Aborting." << endl;
      return 0;
   }

   // setup the tree
   TTree *treePtr = new TTree ("CLPTree", "CLPClasses Skim");
   // counters
   treePtr->Branch ("runNum",     &runNum,     "runNum/I");
   treePtr->Branch ("eventNum",   &eventNum,   "eventNum/I");
   treePtr->Branch ("runSection", &runSection, "runSection/I");
   treePtr->Branch ("numVtxs",    &numVtxs,    "numVtxs/I");
   treePtr->Branch ("entryIndex", &entryIndex, "entryIndex/I");
   treePtr->Branch ("weight",     &weight,     "weight/D");
   treePtr->Branch ("numElecs",   &numElecs,   "numElecs/I");
   treePtr->Branch ("numMuons",   &numMuons,   "numMuons/I");
   treePtr->Branch ("numTracks",  &numTracks,  "numTracks/I");
   treePtr->Branch ("numJets ",   &numJets ,   "numJets/I");
   // TCAs
   treePtr->Branch ("electrons", &elecCLPTcaPtr);
   treePtr->Branch ("muons",     &muonCLPTcaPtr);
   treePtr->Branch ("tracks",    &trackCLPTcaPtr);
   treePtr->Branch ("jets",      &jetCLPTcaPtr);

   if (isSignalMC || matchHepgZs)
   {
      // single objects
      treePtr->Branch ("hepg",   "CLPHEPGCont",  &contPtr);
   }

   // split up file if larger than 500 MB
   treePtr->SetMaxTreeSize( 524288000 );


   ////////////////////////////////////////////
   // ////////////////////////////////////// //
   // // //////////////////////////////// // //
   // // // ////////////////////////// // // //
   // // // // Start The Large Loop // // // //
   // // // ////////////////////////// // // //
   // // //////////////////////////////// // //
   // ////////////////////////////////////// //
   ////////////////////////////////////////////

   Int_t nentries = Int_t(chainPtr->GetEntries());
   cout << "Possible entries: " << nentries << endl;
   if ((gNumWanted > 0) && (gNumWanted < nentries))
   {
      nentries = gNumWanted;    
   }
   cout << " We are looping over " << nentries << " events." << endl;
   int numActual = 0;
   for (entryIndex = 0; entryIndex < nentries; ++entryIndex) 
   {
      bool takeEvent = true;

      if (debug) cout << "entry " << entryIndex << endl;

      if (entryIndex % 100 == 0) cout << entryIndex << endl;

      ///////////////////////////
      // Get event from ntuple //
      ///////////////////////////
      // check run
      int status = 0;
      int size = (int) branchNameVec.size();
      int iFlag = chainPtr->LoadTree (entryIndex);
      if (iFlag < 0)
      {
         // If iFlag < 0, there's nothing left
         break;
      }
      for (int loop = 0; loop < size; ++loop)
      {
         status += chainPtr->GetBranch 
            ((const char*) branchNameVec[loop])->GetEntry(iFlag);
      }
      if (0 == status)
      {
         // we didn't get anything.  Let's break out now
         //continue;
      }
      ////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////
      //      ////////////////////////////////////////////////////      //
      //      //       TopNtuple Specific Variables             //      //
      //      // By trying to rename as many TopNtuple specific //      //
      //      // variables here as possible, it will make it    //      //
      //      // easier to use this code on other ntuples.      //      //
      //      ////////////////////////////////////////////////////      //


      // Check to see if event is flagged as a cosmic
      //  if ((summary_fTopEventClass[0] & 0x20) != 0)
      //  {
      //     isCosmic = true;
      //  }
      evt *evtPtr = (evt*) evtTcaPtr->At(0);
      summary *summaryPtr = (summary*) summaryTcaPtr->At(0);
      
      //cout << "run " << evtPtr->runNumber << " event " << evtPtr->eventNumber
      //     << endl;
      //continue;
      runNum     = CLPNamespace::ns_runNumber   = evtPtr->runNumber;
      eventNum   = CLPNamespace::ns_eventNumber = evtPtr->eventNumber;
      runSection = evtPtr->runSection;
      // calculate the number of quality 12 verticies      
      numVtxs    = 0;
      int numPossibleVtx = zvtxsTcaPtr->GetEntries();
      for (int loop = 0; loop < numPossibleVtx; ++loop)
      {
         zvtxs *vtxPtr = (zvtxs*) zvtxsTcaPtr->At(loop);
         if (vtxPtr->quality >= 12)
         {
            ++numVtxs;
         } // if quality 12
      } // for loop
      Int_t numNTelecs  = electronTcaPtr->GetEntries();  
      Int_t numNTmuons  = muonTcaPtr->GetEntries();
      Int_t numNTtracks = offltrackTcaPtr->GetEntries();
      Int_t numNTjets   = jetTcaPtr->GetEntries();
      // Start with the Missing Et calculated in the top ntuple
      CLP3Vector met3Vec = 
         CLP3Vector::fromEtaPhiMag (0., // (eta) since only trans component
                                    summaryPtr->fmuoMetPhi,
                                    summaryPtr->fmuoMet);

      //                                                                //
      //                                                                //
      ////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////

      ///////////////
      // Electrons //
      ///////////////
      elecCLPTca.Clear();
      numElecs = 0;
      for (int elecLoop = 0; elecLoop < numNTelecs; ++elecLoop)
      {
         CLPElectron electron = clpElectron(elecLoop, electronTcaPtr);
         if (electron.isUndefined())
         {
            // don't bother with electrons that aren't in one of
            // our classifications
            continue;
         }
         new (elecCLPTca[numElecs++]) CLPElectron( electron );         
      } // for elecLoop

      ///////////
      // Muons //
      ///////////
      muonCLPTca.Clear();
      numMuons = 0;
      for (int muonLoop = 0; muonLoop < numNTmuons; ++muonLoop)
      {
         CLPMuon muon = clpMuon(muonLoop, muonTcaPtr);
         if (muon.isUndefined())
         {
            // don't bother with muons that aren't in one of
            // our classifications
            continue;
         }
         new (muonCLPTca[numMuons++]) CLPMuon( muon );         
      } // for muonLoop


      ////////////
      // Tracks //
      ////////////
      trackCLPTca.Clear();
      CLPTrack::TrackList trackList;
      numTracks = 0;
      for (int trackLoop = 0; trackLoop < numNTtracks; ++trackLoop)
      {
         CLPTrack track = clpTrack(trackLoop, offltrackTcaPtr);
         if (track.BCPt() < 0 && track.Pt() < 10)
         {
            // this is a useless track (for now) so lets not even
            // store it
            continue;
         }
         if (track.isUndefined())
         {
            // don't bother with tracks that aren't in one of
            // our classifications
            continue;
         }
         if (signalVeto)
         {
            // we need to keep track of tracks that pass all cuts
            CLPCuts::TrackResult result = CLPCuts::trackCuts (track);
            if ( result.none() )
            {
               trackList.push_back( track );
            }
         }
         new (trackCLPTca[numTracks++]) CLPTrack( track );         
      } // for trackLoop



      //////////
      // Jets //
      //////////
      jetCLPTca.Clear();
      numJets = 0;
      if (numVtxs < 1)
      {
         numVtxs = 1;
      }
      JetEnergyCorrections jetCorrections("JetCorrections","JetCorrections",
                                          kJCLevel_five, numVtxs,
                                          kJCConeSize, kJCVersion,
                                          kJCSysCode, runNum, imode);      
      jetCorrections.setCemCorON(false);
      JetEnergyCorrections jetCorrections7("JetCorrections","JetCorrections",
                                           kJCLevel_seven, numVtxs,
                                           kJCConeSize, kJCVersion,
                                           kJCSysCode, runNum, imode);      
      jetCorrections7.setCemCorON(false);
      int numGoodJets = 0;
      int numOkJets  = 0;
      float maxEt = 0;
      for (int jetLoop = 0; jetLoop < numNTjets; ++jetLoop)
      {
         CLPJet jet = clpJet(jetLoop, jetTcaPtr, 
                             jetCorrections, jetCorrections7);
         if (jet.isUndefined())
         {
            // don't bother with jets that aren't in one of
            // our classifications
            continue;
         }
         if (jet.Et() > maxEt)
         {
            maxEt = jet.Et();
         } // maxEt
         CLPCuts::JetResult result = CLPCuts::jetCuts (jet);
         if ( result.none() )
         {
            if (signalVeto)
            {
               // we need to keep track of jets/track overlaps
               CLP3Vector jetVec = jet.mom3Vec();         
               bool matchedToTrack = false;
               for (int trkIndex = 0; trkIndex < (int) trackList.size(); 
                    ++trkIndex)
               {
                  CLP3Vector trkVec = trackList[trkIndex].mom3Vec();
                  if ( jetVec.etaPhiDiff( trkVec ) < 0.4)
                  {
                     // it's too close to a track
                     matchedToTrack = true;
                     break;
                  }
               } // for trkIndex
               if (! matchedToTrack)
               {
                  ++numGoodJets;
               }
            }
            else {
               ++numGoodJets;
            }
         }
         if (fabs(jet.eta()) < 2.5)
         {
            ++numOkJets;
         }
         new (jetCLPTca[numJets++]) CLPJet( jet );         
      } // for jetLoop

      if (signalVeto)
      {
         // We don't want this event if there are too many jet AND the
         // leading jet Et is too large
         if ((numGoodJets >= ns_veto_jetCount) &&
             (maxEt       >= ns_veto_jetEt))
         {
            takeEvent = false;
         }
      }

      if (require4jets)
      {
         // we want three good jets (Et and eta cuts)
         // and fourth jet that passes eta cuts
         if (numGoodJets < 3 || numOkJets < 4)
         {
            takeEvent = false;
         }
      } // if we're requiring at least 4 good jets

      // We're almost home
      if (! takeEvent)
      {
         // we don't want this event
         continue;
      }

      ///////////////////
      // HEPG Matching //
      // IFF Signal MC //
      ///////////////////
      if (isSignalMC)
      {
         fillHEPGCont (*contPtr, hepgTcaPtr);
         doHEPGMatching (*contPtr, elecCLPTca, muonCLPTca, jetCLPTca, trackCLPTca);
      } // if signal MC
      
      if (matchHepgZs)
      {
         fillHEPGContWithZs (*contPtr, hepgTcaPtr);
         doHEPGMatching (*contPtr, elecCLPTca, muonCLPTca, jetCLPTca, trackCLPTca);
      } // match HEPG Zs
    
      // O.k. We're still here.
      // Should we check the filename to figure out a weight?
      if (variableWeights)
      {
         TString filename = chainPtr->GetCurrentFile()->GetName();
         // Muons
         if (      filename.Contains ("etop0m") ) { weight =   3.43341E-01; } 
         else if ( filename.Contains ("etop1m") ) { weight =   4.41129E-02; } 
         else if ( filename.Contains ("etop2m") ) { weight =   8.46244E-03; } 
         else if ( filename.Contains ("etop3m") ) { weight =   1.56866E-03; } 
         else if ( filename.Contains ("etop4m") ) { weight =   2.76659E-04; } 
         else if ( filename.Contains ("etop5m") ) { weight =   7.55815E-05; } 
         // Electrons
         else if ( filename.Contains ("etop0q") ) { weight =   3.36803E-01; } 
         else if ( filename.Contains ("etop1q") ) { weight =   4.40564E-02; } 
         else if ( filename.Contains ("etop2q") ) { weight =   8.60918E-03; } 
         else if ( filename.Contains ("etop3q") ) { weight =   1.58224E-03; } 
         else if ( filename.Contains ("etop4q") ) { weight =   2.76953E-04; } 
         else if ( filename.Contains ("etop5q") ) { weight =   7.55055E-05; } 
         else { weight = 0.; }
      } // if variable weights
      if (requireGoodZ)
      {
         ///////////////////////////////////////////
         // Build lists of particles passing cuts //
         ///////////////////////////////////////////
         CLPElectron::ElectronList elecList;
         int numTCAelecs = elecCLPTca.GetEntries();
         for (int elecLoop = 0; elecLoop < numTCAelecs; ++elecLoop)
         {
            CLPElectron *elecPtr = (CLPElectron*) elecCLPTca.At(elecLoop);
            CLPCuts::ElectronResult result = CLPCuts::electronCuts(*elecPtr);
            // tight cuts
            if ( result.none() )
            {
               elecList.push_back( *elecPtr );
            } 
         } // for elecLoop
         CLPMuon::MuonList muonList;
         int numTCAmuons = muonCLPTca.GetEntries();
         for (int muonLoop = 0; muonLoop < numTCAmuons; ++muonLoop)
         {
            CLPMuon *muonPtr = (CLPMuon*) muonCLPTca.At(muonLoop);
            CLPCuts::MuonResult result = CLPCuts::muonCuts(*muonPtr);
            // tight cuts
            if ( result.none() )
            {
               muonList.push_back( *muonPtr );
            } 
         } // for muonLoop
         CLPTrack::TrackList trackList;         
         int numTCAtracks = trackCLPTca.GetEntries();
         for (int trackLoop = 0; trackLoop < numTCAtracks; ++trackLoop)
         {
            CLPTrack *trackPtr = (CLPTrack*) trackCLPTca.At(trackLoop);
            CLPCuts::TrackResult result = CLPCuts::trackCuts(*trackPtr);
            // tight cuts
            if ( result.none() )
            {
               trackList.push_back( *trackPtr );
            } 
         } // for trackLoop
         CLPCandidate::CandidateList candList;
         makeZcandidates (candList, elecList, muonList, trackList);
         if (! candList.size() )
         {
            // no Z candidates
            continue;
         }
      } // if good Z
      treePtr->Fill();
      ++numActual;
   } // for entryIndex
   treePtr->Write();
   cout << "finished gracefully with " << numActual << " events." << endl;
   return 0;
}

void initialize()
{
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_IntMap["num"]            = (void*) &gNumWanted;
   ns_IntMap["howmany"]        = (void*) &gNumWanted;
   ns_IntMap["veto"]           = (void*) &gSignalVeto;
   ns_IntMap["fourjets"]       = (void*) &gRequire4jets;
   ns_IntMap["goodz"]          = (void*) &gRequireGoodZ;
   ns_StringMap["list"]        = (void*) &gListName;
   ns_StringMap["file"]        = (void*) &gFileName;
   ns_StringMap["output"]      = (void*) &gOutputName;
}

