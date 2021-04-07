#define SkimFCNC_cxx
// CLP start
// "Additional" headers -- not from MakeTree command in root
#include "time.h"
#include "CLPClasses/dout.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "JetUser/JetEnergyCorrections.hh"
#include "TString.h" // For Evelyn's 'skipFiascoEvents'
#include "CLPClasses/CLPClasses.h"

#include "SkimFCNC.h"
#include "TH2.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TAuthenticate.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;

#include "FromSkimFCNC.C_include"

extern bool hasGoodZs (TClonesArray &elecTca, TClonesArray &muonTca, 
                       TClonesArray &trackTca, TClonesArray &jetTca);

// Jet Correction Constants
const Int_t kJCLevel_five = 5;
const Int_t kJCLevel_seven = 7;
const Int_t kJCNumVerticies = 1;
const Int_t kJCConeSize = 0;
const Int_t kJCVersion = 5;
const Int_t kJCSysCode = 0;
const Int_t kJCiMode_Data = 1;
const Int_t kJCiMode_MC = 0;
// CLP end

void SkimFCNC::Loop(Int_t numWanted, bool debug)
{
   ////////////////////
   // Initialization //
   ////////////////////

   // Set ntuple type
   setNtupleType(CLPNamespace::kTopNtuple);
   //CLP3Vector::setOutput3Mode( CLP3Vector::kStandardOutputMode );

   // boolean variables
   bool isSignalMC      = isUserBitOn(0);
   bool variableWeights = isUserBitOn(1);
   bool require4jets    = isUserBitOn(2);
   bool matchHepgZs     = isUserBitOn(3);

   if (matchHepgZs)
   {
      cout << "Z MC, no?" << endl;
   }

   if (variableWeights)  
   {
      cout << "VARIABLE WEIGHTS" << endl;
   }

   // use jet corrections level 7 for comparisons
   // 0 for Level 5, 1 for Level 7
   CLPJet::setDefaultCorrectionType (1);
   
   // set jet correction version
   Int_t imode;
   if (CLPFunctions::isData())
   {
      imode = kJCiMode_Data;
      cout << "Skiming as DATA" << endl;
   } else {
      imode = kJCiMode_MC;
      cout << "Skiming as MC" << endl;
   }
   cout << "imode " << imode << endl;

   if (fChain == 0) return;

   Int_t nentries = Int_t(fChain->GetEntriesFast());
   cout << "Possible entries: " << nentries << endl;
   if ((numWanted > 0) && (numWanted < nentries))
   {
      nentries = numWanted;    
   }
   cout << " We are looping over " << nentries << " events." << endl;

   // TCloneArrays
   TClonesArray *elecTcaPtr  = 
      new TClonesArray(CLPNamespace::kElectronClassName, 20);
   TClonesArray *muonTcaPtr  = 
      new TClonesArray(CLPNamespace::kMuonClassName, 20);
   TClonesArray *trackTcaPtr  = 
      new TClonesArray(CLPNamespace::kTrackClassName, 20);
   TClonesArray *jetTcaPtr   = 
      new TClonesArray(CLPNamespace::kJetClassName,  20);
   TClonesArray &elecTca  = *elecTcaPtr;
   TClonesArray &muonTca  = *muonTcaPtr;
   TClonesArray &trackTca = *trackTcaPtr;
   TClonesArray &jetTca   = *jetTcaPtr;
   elecTca.BypassStreamer(true);
   muonTca.BypassStreamer(true);
   trackTca.BypassStreamer(true);
   jetTca.BypassStreamer(true);
   // Various counters
   Int_t runNum;    
   Int_t eventNum;
   Int_t runSection;
   Int_t numVtxs;
   Double_t weight = 1;
   Int_t numElecs;
   Int_t numMuons;
   Int_t numTracks;
   Int_t numJets; 
   // single variables
   CLPHEPGCont *contPtr = new CLPHEPGCont;

   // open file for output
   string name = userString(0);
   if (0 == name.length())
   {
      name = "/cdf/scratch/cplager/fcnc/skim.root";
   }
   cout << "Skimming to " << name << endl;

   TFile file (name.c_str(), "RECREATE");
   // setup the tree
   TTree tree ("CLPTree", "CLPClasses Skim");
   // counters
   tree.Branch ("runNum",     &runNum,     "runNum/I");
   tree.Branch ("eventNum",   &eventNum,   "eventNum/I");
   tree.Branch ("runSection", &runSection, "runSection/I");
   tree.Branch ("numVtxs", &numVtxs, "numVtxs/I");
   tree.Branch ("weight",     &weight,     "weight/D");
   tree.Branch ("numElecs",   &numElecs,   "numElecs/I");
   tree.Branch ("numMuons",   &numMuons,   "numMuons/I");
   tree.Branch ("numTracks",  &numTracks,  "numTracks/I");
   tree.Branch ("numJets ",   &numJets ,   "numJets/I");
   // TCAs
   tree.Branch ("electrons", &elecTcaPtr);
   tree.Branch ("muons",     &muonTcaPtr);
   tree.Branch ("tracks",    &trackTcaPtr);
   tree.Branch ("jets",      &jetTcaPtr);
   if (isSignalMC || matchHepgZs)
   {
      // single objects
      tree.Branch ("hepg",   "CLPHEPGCont",  &contPtr);
   }
   for (Int_t entryIndex=0; entryIndex < nentries; ++entryIndex) 
   {
      bool takeEvent = true;

      if (debug) cout << "entry " << entryIndex << endl;

      if (entryIndex % 100 == 0) cout << entryIndex << endl;

      ///////////////////////////
      // Get event from ntuple //
      ///////////////////////////
      Int_t loadResult = LoadTree(entryIndex);
      // apparently, if loadResult is negative, everything is
      // screwed, so you might as well quit now
      if (loadResult < 0) 
      {
         // Sorry, eh.
         break;
      }


      if (debug) cout << "loaded entry" << endl;
      
      // check run
      fChain->GetEntry(entryIndex);


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
      runNum     = CLPNamespace::ns_runNumber   = evt_runNumber[0];
      eventNum   = CLPNamespace::ns_eventNumber = evt_eventNumber[0];
      runSection = evt_runSection[0];
      numVtxs = numZVtxs;
      Int_t numNTelecs  = numEmObjs;      
      Int_t numNTmuons  = numMuObjs;
      Int_t numNTtracks = numOfflTracks;
      Int_t numNTjets   = numJetObjs;
      // Start with the Missing Et calculated in the top ntuple
      CLP3Vector met3Vec = 
         CLP3Vector::fromEtaPhiMag (0., // (eta) since only trans component
                                    summary_fmuoMetPhi[0],
                                    summary_fmuoMet[0]);

      //                                                                //
      //                                                                //
      ////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////

      ///////////////
      // Electrons //
      ///////////////
      elecTca.Clear();
      numElecs = 0;
      for (int elecLoop = 0; elecLoop < numNTelecs; ++elecLoop)
      {
         CLPElectron electron = clpElectron(elecLoop);
         if (electron.isUndefined())
         {
            // don't bother with electrons that aren't in one of
            // our classifications
            continue;
         }
         new (elecTca[numElecs++]) CLPElectron( electron );         
      } // for elecLoop

      ///////////
      // Muons //
      ///////////
      muonTca.Clear();
      numMuons = 0;
      for (int muonLoop = 0; muonLoop < numNTmuons; ++muonLoop)
      {
         CLPMuon muon = clpMuon(muonLoop);
         if (muon.isUndefined())
         {
            // don't bother with muons that aren't in one of
            // our classifications
            continue;
         }
         new (muonTca[numMuons++]) CLPMuon( muon );         
      } // for muonLoop


      ////////////
      // Tracks //
      ////////////
      trackTca.Clear();
      numTracks = 0;
      for (int trackLoop = 0; trackLoop < numNTtracks; ++trackLoop)
      {
         CLPTrack track = clpTrack(trackLoop);
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
         new (trackTca[numTracks++]) CLPTrack( track );         
      } // for trackLoop



      //////////
      // Jets //
      //////////
      jetTca.Clear();
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
      for (int jetLoop = 0; jetLoop < numNTjets; ++jetLoop)
      {
         CLPJet jet = clpJet(jetLoop, jetCorrections, jetCorrections7);
         if (jet.isUndefined())
         {
            // don't bother with jets that aren't in one of
            // our classifications
            continue;
         }
         if ((jet.Et() > 13 * GEV) &&
             (fabs(jet.eta()) < 2.5) )
         {
            ++numGoodJets;
         }
         if (fabs(jet.eta()) < 2.5)
         {
            ++numOkJets;
         }
         new (jetTca[numJets++]) CLPJet( jet );         
      } // for jetLoop

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
         fillHEPGCont (*contPtr);
         doHEPGMatching (*contPtr, elecTca, muonTca, jetTca, trackTca);
      } // if signal MC

      if (matchHepgZs)
      {
         fillHEPGContWithZs (*contPtr);
         doHEPGMatching (*contPtr, elecTca, muonTca, jetTca, trackTca);
      } // match HEPG Zs
    
      // O.k. We're still here.
      // Should we check the filename to figure out a weight?
      if (variableWeights)
      {
         TString filename = fChain->GetCurrentFile()->GetName();
         if ( filename.Contains ("etop0m") )      { weight =   3.43341E-01; } 
         else if ( filename.Contains ("etop1m") ) { weight =   4.41129E-02; } 
         else if ( filename.Contains ("etop2m") ) { weight =   8.46244E-03; } 
         else if ( filename.Contains ("etop3m") ) { weight =   1.56866E-03; } 
         else if ( filename.Contains ("etop4m") ) { weight =   2.76659E-04; } 
         else if ( filename.Contains ("etop5m") ) { weight =   7.55815E-05; } 
         else if ( filename.Contains ("etop0q") ) { weight =   3.36803E-01; } 
         else if ( filename.Contains ("etop1q") ) { weight =   4.40564E-02; } 
         else if ( filename.Contains ("etop2q") ) { weight =   8.60918E-03; } 
         else if ( filename.Contains ("etop3q") ) { weight =   1.58224E-03; } 
         else if ( filename.Contains ("etop4q") ) { weight =   2.76953E-04; } 
         else if ( filename.Contains ("etop5q") ) { weight =   7.55055E-05; } 
         else { weight = 0.; }
         //if ( filename.Contains("1p_") )
         //{
         //   weight = 0.527;
         //} else if ( filename.Contains("2p_") )
         //{
         //   weight = 0.2283;
         //} else if ( filename.Contains("3p_") )
         //{
         //   weight = 0.1895;
         //} else if ( filename.Contains("4p_") )
         //{
         //   weight = 0.1078;
         //} else {
         //   weight = 0.00;
         //}
      } // if variable weights
      if (CLPFunctions::isUserBitOn(4))
      {
         if (! hasGoodZs(elecTca, muonTca, trackTca, jetTca))
         {
            continue;
         }
      }
      tree.Fill();
   } // for entryIndex
   file.Write();
}
