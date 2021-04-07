// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include "time.h"
#include "CLPClasses/dout.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include "CLPClasses/CLPClasses.h"
#include "KinematicFitter/KinematicFitter.hh"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;

// Global Variables (ugh..) 
// Note, name in parenthesis is command-line variable name.
// e.g. 'which=12 usetracks=1' will set gWhich = 12 and gUseTracks = false

// Which file to grab (which)
int gWhich         = 13; //smttbar
// number of events (numwanted/howmany)
int gNumWanted     = 10;
// current section (section)
int gSection       = 0;
// which entry to start at (start)
int gStartEntry    = 0;
// chiSq cutoff (chisq)
double gMaxChiSq   = 20.0;
// W width (wwid)
double gW_width    = 10 * GEV;
// Count number of B tag jets only (count)
bool gCountOnly    = false;
// use goodRunList (goodrun)
bool gUseGoodRun   = true;
// print debug statements (debug)
int gLocalDebug    = 0;
// more debugging (scoby)
int gScobyDebug    = 0;
// prints out a count once every (numper)
int gNumPer        = 100;
// source file name (file)
string gFileName;
// source list name (list)
string gListName;
// target name (target)
string gTargetName = "../Tuples/histogram";

// order used for permutation
enum {
   kB1Index,
   kB2Index,
   kW11Index,
   kW12Index,
   kW21Index,
   kW22Index
};

// forward declarations
void initialize();

// returns true if this passes all cuts (or if alwaysFit is true)
bool processPermutation (CLPPermutation &perm,
                         const IVec &vec,
                         const CLPJet::JetList &jetList,
                         int numLooseTags, int numTightTags,
                         bool alwaysFit = false);

int main(int argc, char** argv)
{
   initialize();
   parseArguments (argc, argv);
   GoodRunClass withSili ("goodrun_qcd_si.list");
   SelectRunEventClass select("runevent.list");
   cout << "numWanted " << gNumWanted << endl;

   ///////////////////////
   // Set Defaults Here //
   ///////////////////////   
   CLPJet::setDefaultCorrectionType (CLPJet::kLevel5);
   //CLP3Vector::setOutput3Mode( CLP3Vector::kPtEtaPhiOutputMode);
   CLPCandidate::setOutputType (CLPCandidate::kEverything);
   enum {
      kInput = 0,
      kOutput,
   };
   // Order in HEPG container
   enum {
      kHepgB1Index = 0,
      kHepgW11Index,
      kHepgW12Index,
      kHepgB2Index,
      kHepgW21Index,
      kHepgW22Index,
   };

   enum {
      kIsHEPGcomb = 0,
      kIsUnmatchedComb,
      kIsMatchedComb,
   };
   bool noHepgReconstruction = false;
   if (0 == gWhich) 
   {
      // do nothing
      //noHepgReconstruction = true;
   } 
   else if (11 == gWhich) 
   {
      // SM ttbar
      //gFileName = "../Tuples/clptree_smttbar_branch_100000.root";
      gFileName = "/cdf/scratch/scoby/skim/clptree_smttbar2_branch_10000.root";
      gTargetName = "../Tuples/allhad2";
      //noHepgReconstruction = true; // don't do HEPG event reconstruction
   } 
   else if (12 == gWhich)
   {
      // SM ttbar ignoring HEPG banks
      gFileName ="/cdf/scratch/scoby/skim/all_ttopel_nt5_4j_branch_10000.root";
      gTargetName = "../Tuples/cand_smttbar2_allHad";
      noHepgReconstruction = true;
   }
   else if (13 == gWhich) 
   { 
      // SM ttbar with HEPG banks
      //gFileName ="/cdf/scratch/scoby/skim/all_ttopel_nt5_4j_branch_10000.root";
      gFileName ="../Tuples/clptree_ttopel_decode_allhad_n21000.root";
      gTargetName = "../Tuples/cand_allhad_smttbar";
   }
   else if (14 == gWhich)
   {
      //gFileName = "/cdf/scratch/scoby/skim/clptree_6p_100000.root";
      gFileName = "../data/clptree_6p_100000.root";
      gTargetName = "../Tuples/cand_smQCD6p_allhad";
      noHepgReconstruction = true;
   }
   else if (15 == gWhich)
   {
      gTargetName += "_customNoHEPG";
      noHepgReconstruction = true;
   }


   if ( (0 == gFileName.length() && 0 == gListName.length())  || 
        0 == gTargetName.length() )
   {
      cout << "invalid value of which " << gWhich << endl
           << "Use 'which', 'file', 'list', and/or 'target' options."
           << endl;
      exit(0);
   }


   ////////////////////
   // Initialization //
   ////////////////////
   // Let's change the output name depending on which options are set
   // (to avoid future problems...)

   // Put this in a 'do while (0)' loop so we can easily break out
   do
   {
      if (gCountOnly)
      {
         gTargetName += "_count";
         gNumPer = 1000;
         break;
      }
      if (gMaxChiSq)
      {
         gTargetName += Form ("_chiSq%.0f",  gMaxChiSq);
      }
      if (gW_width)
      {
         gTargetName += Form ("_wwid%d", (int) gW_width);
      }
      if (gSection)
      {
         gTargetName += Form ("_sec%03d", gSection);
      }
      if (gStartEntry)
      {
         gTargetName += Form ("_start%d", gStartEntry);
      }
      if (gNumWanted)
      {
         gTargetName += Form ("_n%d", gNumWanted);
      }
   } while (false);
   gTargetName += ".root";
  

   cout << "From: " << gFileName << endl
        << "To:   " << gTargetName << endl;

   // skim file
   TFile *targetPtr = new TFile ( gTargetName.c_str(), "RECREATE" ); 

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
   TClonesArray *elecTcaPtr  = new TClonesArray("CLPElectron", 10);
   TClonesArray *muonTcaPtr  = new TClonesArray("CLPMuon", 10);
   TClonesArray *trackTcaPtr = new TClonesArray("CLPTrack", 10);
   TClonesArray *jetTcaPtr   = new TClonesArray("CLPJet", 10);   
   TClonesArray &elecTca     = *elecTcaPtr;
   TClonesArray &muonTca     = *muonTcaPtr;
   TClonesArray &trackTca    = *trackTcaPtr;
   TClonesArray &jetTca      = *jetTcaPtr;
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
   Int_t runNum;    
   Int_t eventNum;
   Int_t runSection;
   Int_t numElecs;
   Int_t numMuons;
   Int_t numTracks;
   Int_t numJets; 
   Double_t weight;
   chainPtr->SetBranchAddress("runNum",   &runNum);
   chainPtr->SetBranchAddress("eventNum", &eventNum);
   chainPtr->SetBranchAddress("runSection", &runSection);
   chainPtr->SetBranchAddress("weight",   &weight);
   chainPtr->SetBranchAddress("numElecs", &numElecs);
   chainPtr->SetBranchAddress("numMuons", &numMuons);
   chainPtr->SetBranchAddress("numTracks", &numTracks);
   chainPtr->SetBranchAddress("numJets ", &numJets);
   // HEPG container
   CLPHEPGCont *contPtr      = 0;
   bool isSignalMC = chainPtr->GetBranchStatus("hepg");
   if (noHepgReconstruction)
   {
      isSignalMC = false;
   }
   if (isSignalMC)
   {
      contPtr = new CLPHEPGCont;
      cout << "Processing HEPG" << endl;
      // hepg container
      chainPtr->SetBranchAddress ("hepg", &contPtr);
   } 
   else
   {
      cout << "Not Processing HEPG bank" << endl;
   }

   
   //////////////////
   // Output Trees //
   //////////////////
   // Reconstructedcandidate tree
   CLPCandidate *candPtr = new CLPCandidate;
   TTree *candTreePtr 
      = new TTree ("candTree", "Candidates for ttbar all-hadronic decay");
   candTreePtr->Branch    ("candidate", "CLPCandidate", &candPtr);
   // HEPG matched candidate tree
   CLPCandidate *matchedCandPtr = new CLPCandidate;
   TTree *matchedTreePtr 
      = new TTree ("matchedTree", "Candidates for ttbar all-hadronic decay");
   matchedTreePtr->Branch ("matched",   "CLPCandidate", &matchedCandPtr); 
   TH1F *looseCountHist = 0;
   TH1F *tightCountHist = 0;
   if (gCountOnly)
   {
      looseCountHist = new TH1F ("looseCount", "Loose Btag Jets", 6, -0.5, 5.5);
      tightCountHist = new TH1F ("tightCount", "Tight Btag Jets", 6, -0.5, 5.5);
   } // if counting only
  
   ////////////////////
   // Start the loop //
   ////////////////////
   Int_t nentries = Int_t(chainPtr->GetEntries());
   cout << "Possible entries: " << nentries << endl;
   if ((gNumWanted > 0) && (gNumWanted + gStartEntry < nentries))
   {
      nentries = gNumWanted + gStartEntry;    
   }
   cout << " We are looping over " << nentries - gStartEntry 
        << " events." << endl;
   
   for (Int_t entryIndex = gStartEntry; entryIndex < nentries; ++entryIndex) 
   {
      // check run
      chainPtr->GetEntry(entryIndex);

      if ((entryIndex + 1) % gNumPer == 0) cout << entryIndex + 1 << endl;
  
      if (gUseGoodRun && !withSili.isGoodRun (runNum, runSection) )
      {
         continue;
      }
 
      ///////////////
      // Electrons //
      ///////////////
      CLPElectron::ElectronList elecList;
      int numTCAelecs = elecTca.GetEntries();
      for (int elecLoop = 0; elecLoop < numTCAelecs; ++elecLoop)
      {
         CLPElectron *elecPtr = (CLPElectron*) elecTca.At(elecLoop);
         if ( elecPtr->isaTCE() || elecPtr->isaPHX() )
         {
            CLPCuts::ElectronResult result = CLPCuts::electronCuts(*elecPtr);
            // tight cuts
            if ( result.none() )
            {
               elecList.push_back( *elecPtr );
            } 
         } // is a cem
      } // for elecLoop
   
      ///////////
      // Muons //
      ///////////
      CLPMuon::MuonList muonList;
      int numTCAmuons = muonTca.GetEntries();
      for (int muonLoop = 0; muonLoop < numTCAmuons; ++muonLoop)
      {
         CLPMuon *muonPtr = (CLPMuon*) muonTca.At(muonLoop);
         CLPCuts::MuonResult result = CLPCuts::muonCuts( *muonPtr);
         // for now, we are only allowing tight leptons
         if ( result.none() && muonPtr->isaTightLepton())
         {
            muonList.push_back( *muonPtr );
         }
      } // for muonLoop
   
      ////////////
      // Tracks //
      ////////////
      CLPTrack::TrackList trackList;
      int numTCAtracks = trackTca.GetEntries();
      for (int trackLoop = 0; trackLoop < numTCAtracks; ++trackLoop)
      {
         CLPTrack *trackPtr = (CLPTrack*) trackTca.At(trackLoop);
         CLPCuts::TrackResult result = CLPCuts::trackCuts( *trackPtr);
         if ( result.none() )
         {
            trackList.push_back( *trackPtr );
         } //if it passed all cuts
      } // for trackLoop

      // reject event if there are any leptons
      if (elecList.size() + muonList.size() + trackList.size())
      {
         // lepton veto
         continue;
      }

      //////////
      // Jets //
      //////////
      CLPJet::JetList jetList;
      int numGoodMatchedJets = 0;
      int numLooseTags = 0;
      int numTightTags = 0;
      int numTCAjets = jetTca.GetEntries();
      for (int jetLoop = 0; jetLoop < numTCAjets; ++jetLoop)
      {
         CLPJet *jetPtr = (CLPJet*) jetTca.At(jetLoop);
         CLPCuts::JetResult result = CLPCuts::jetCuts( *jetPtr);
         if ( result.none() )
         {
            jetList.push_back( *jetPtr );
            if (jetPtr->isBTagged (true))
            {
               ++numLooseTags;
            }
            if (jetPtr->isBTagged (false))
            {
               ++numTightTags;
            }
            if (jetPtr->isTagged())
            {
               ++numGoodMatchedJets;
            }     
         }
      } // for jetLoop

      // Don't bother unless we have at least 6 jets
      if (jetList.size() < 6)
      {
         // not enough jets
         continue;
      }

      if (gCountOnly)
      {
         looseCountHist->Fill (numLooseTags);
         tightCountHist->Fill (numTightTags);
         continue;
      }

      //////////////////////////////////////////
      // Sort the jets from high Et to low Et //
      //////////////////////////////////////////

      // since we want them in decending order, we use a little trick
      // and sort 'rbegin' to 'rend' instead of 'begin' to 'end'.
      sort (jetList.rbegin(), jetList.rend());


      ////////////////////
      /// HEPG matching //
      ////////////////////
      IVec reMatchedIndexVec;
      if ( contPtr )  // equivalent to isSignalMC
      {
         if ( contPtr->topSMdecayOk_allhad() )
         {
            // re-index HEPG Matching
            contPtr->resetJetHEPGMatching ( jetList );
            if ( contPtr->allPossibleRecon() )           
            {
               IVec matchedIndexVec;
               reMatchedIndexVec.clear();

               //int count = 0;
               int size = contPtr->size();
               for (int loop = 0; loop < size; ++loop)
               {
                  CLPHEPGObject object = contPtr->object (loop);
                  int reconIndex = object.taggedToReconIndex();
                  if (reconIndex >= 0)
                  {
                     matchedIndexVec.push_back(reconIndex);
                  } // if reconstructed
               } // loop over HEPG container

               // I want to get the matched jetList indicies
               int matchedB1index  = matchedIndexVec[kHepgB1Index];
               int matchedB2index  = matchedIndexVec[kHepgB2Index];
               int matchedW11index = matchedIndexVec[kHepgW11Index];
               int matchedW12index = matchedIndexVec[kHepgW12Index];
               int matchedW21index = matchedIndexVec[kHepgW21Index];
               int matchedW22index = matchedIndexVec[kHepgW22Index];
               
               // reorder the matchedIndexVec
               if (matchedB1index < matchedB2index)
               {
                  reMatchedIndexVec.push_back(matchedB1index);
                  reMatchedIndexVec.push_back(matchedB2index);
               }
               else
               {
                  reMatchedIndexVec.push_back(matchedB2index);
                  reMatchedIndexVec.push_back(matchedB1index);
                  // swap the W indicies
                  std::swap (matchedW11index, matchedW21index);
                  std::swap (matchedW12index, matchedW22index);
               }
               if (matchedW11index < matchedW12index)
               {
                  reMatchedIndexVec.push_back(matchedW11index);
                  reMatchedIndexVec.push_back(matchedW12index);
               }
               else
               {
                  reMatchedIndexVec.push_back(matchedW12index);
                  reMatchedIndexVec.push_back(matchedW11index);
               }
               if (matchedW21index < matchedW22index)
               {
                  reMatchedIndexVec.push_back(matchedW21index);
                  reMatchedIndexVec.push_back(matchedW22index);
               }
               else
               {
                  reMatchedIndexVec.push_back(matchedW22index);
                  reMatchedIndexVec.push_back(matchedW21index);
               }

               // // we now have everything in the order we want.  Let's
               // // use it and see what we get
               CLPPermutation perm;
               perm.setIsHEPGMatched (true);
               processPermutation (perm,
                                   reMatchedIndexVec,
                                   jetList,
                                   numLooseTags, numTightTags,
                                   true);
               perm.setType(0);
               matchedCandPtr->Clear();
               matchedCandPtr->setRunNumber (runNum);
               matchedCandPtr->setEventNumber (eventNum);
               matchedCandPtr->setRunSection (runSection);
               matchedCandPtr->setEntryIndex (entryIndex);
               matchedCandPtr->addJets( jetList );
               matchedCandPtr->addPermutation( perm );
               matchedTreePtr->Fill();
               
            } // if HEPG containter is completely matched
         } // if allhad decay
      } // if signal MC (contPtr>

      // Using combinations
      CLPPermutation::PermList permList;
      
      int from = (int) jetList.size();
      // Don't bother looping over more than top 8 jets
      if (from > 8)
      {
         from = 8;
      }
      int choose = 6;
      CombEngine engine (from, choose);

      int numPerm = engine.numPermutations();

      for (int permIndex = 0; permIndex < numPerm; ++permIndex)
      {
         IVec vec = engine.permutation (permIndex); 
 
         // Check to make sure this isn't a duplicate
         if (vec[kB1Index]  > vec[kB2Index]  ||
             vec[kW11Index] > vec[kW12Index] ||
             vec[kW21Index] > vec[kW22Index])
         {
            // not good enough
            continue;
         }
         
         CLPPermutation perm;
         // set the combination member data in the combLoop
         perm.setFromAndChoose (from, choose);
         perm.setCombIndex (permIndex);
         perm.setIsHEPGMatched (false); 
         if ( vec == reMatchedIndexVec ) 
         {
            perm.setIsHEPGMatched(true);
         }
         if ( processPermutation (perm, vec, jetList, 
                                 numLooseTags, numTightTags) )
         {
            permList.push_back( perm );
         } // if we take this one
         
      } // for permIndex

      int permListSize = (int) permList.size();
      // Sort and rank the perms by chiSq
      sort (permList.begin(), permList.end());
      for (int rank=0; rank < permListSize; ++rank)
      {
         permList[rank].setRank(0, rank);
      }
      // do the local rankings
      for (int which = 1; which <= 2; ++which)
      {
         bool checkLoose = true;
         if (2 == which)
         {
            checkLoose = false;
         }

         // Fill the map
         CLPPermutation::PermMap permMap;
         for (int rank=0; rank < permListSize; ++rank)
         {
            CLPPermutation::PermPair key = 
               permList[rank].BtagIndexPair (checkLoose);
            permMap[key].push_back( &(permList[rank]) );
         } // for rank  

         // loop over each key and set local ranks.  Since the main
         // list goes in order from best rank to worst rank, every
         // keyed list is in the same order.
         for (CLPPermutation::PermMapIter iter = permMap.begin();
              permMap.end() != iter;
              ++iter)
         {
            CLPPermutation::PermPtrList &vec = iter->second;
            int size = (int) vec.size();
            for (int rank = 0; rank < size; ++rank)
            {
               (*vec[rank]).setRank (which, rank);
            } // for rank
         } // for iter
      } // for which

      // // debugging
      // for (int rank=0; rank < permListSize; ++rank)
      // {
      //    if (permList[rank].isHEPGMatched())
      //    {
      //       cout << "size " << permListSize << endl
      //            << permList[rank] << endl;
      //    }
      // } // for rank;

      if ( permList.size() )
      {
         // fill the output trees
         candPtr->Clear();
         candPtr->setRunNumber (runNum);
         candPtr->setEventNumber (eventNum);
         candPtr->setRunSection (runSection);
         candPtr->setEntryIndex (entryIndex);
         candPtr->addJets( jetList );
         candPtr->addPermutations( permList );
         candTreePtr->Fill();
      }

   } // for entryIndex
   targetPtr->Write();
   cout << "Finished gracefully." << endl;
   return 0;
}

bool processPermutation (CLPPermutation &perm,
                         const IVec &vec,
                         const CLPJet::JetList &jetList,
                         int numLooseTags, int numTightTags,
                         bool alwaysFit)
{
   static bool firstTime = true;
   static KinematicFitter kinFitter;
   static int combW1c = 0;
   static int combB1c = 0;
   static int combTop1c = 0;
   static int combW2c = 0;
   static int combB2c = 0;
   static int combTop2c = 0;
   static int combConstr_W1 = 0;
   static int combConstr_W2 = 0;
   static int combConstr_top = 0;

   // If this is the first time this function is called, setup the
   // Kinematic Fitter
   if (firstTime)
   {
      firstTime = false;
      combW1c   = kinFitter.createVirtualParticle();
      combB1c   = kinFitter.createVirtualParticle();
      combTop1c = kinFitter.createSuperVirtualParticle (combW1c, combB1c);
      combW2c   = kinFitter.createVirtualParticle();
      combB2c   = kinFitter.createVirtualParticle();
      combTop2c = kinFitter.createSuperVirtualParticle (combW2c, combB2c);
      // Put Mass constraints on the virtual particles 
      // The Ws
      combConstr_W1 = kinFitter.addSingleMassConstraint (combW1c,  kWmass);
      combConstr_W2 = kinFitter.addSingleMassConstraint (combW2c,  kWmass);
      // Now, the tops - Note we will not have all three active at the
      // same time.
      //    - Equal mass constraint
      // see-saw between two top values
      combConstr_top = 
         kinFitter.addEqualMassConstraint (combTop1c, combTop2c);
      // save the current constraints as the defaults.
      kinFitter.saveCurrentConstraintsAsDefault();
      kinFitter.setCovarianceMode( KinematicFitter::kRecalcCovarMode );
      kinFitter.setNumIterations( 100 );
      kinFitter.setDefaultParticleType (KinematicFitter::kMassiveJet);
      kinFitter.setMaxChi2 (1e10);
      kinFitter.setDebugLevel (gScobyDebug);
   } // if first time

   // now prepare to run the kinematic fitter
   CLP4Vector b1  
      = jetList[ vec[ kB1Index] ].mom4Vec  (CLPJet::kLevel5,
                                            CLPJet::kBottomJetScaleFactor); 
   CLP4Vector b2      
      = jetList[ vec[ kB2Index] ].mom4Vec  (CLPJet::kLevel5,
                                            CLPJet::kBottomJetScaleFactor); 
   CLP4Vector W11     
      = jetList[ vec[ kW11Index] ].mom4Vec (CLPJet::kLevel5,
                                            CLPJet::kLightJetScaleFactor);
   CLP4Vector W12     
      = jetList[ vec[ kW12Index] ].mom4Vec (CLPJet::kLevel5,
                                            CLPJet::kLightJetScaleFactor);
   CLP4Vector W21     
      = jetList[ vec[ kW21Index] ].mom4Vec (CLPJet::kLevel5,
                                            CLPJet::kLightJetScaleFactor);
   CLP4Vector W22     
      = jetList[ vec[ kW22Index] ].mom4Vec (CLPJet::kLevel5,
                                            CLPJet::kLightJetScaleFactor);
   // check the W masses
   CLP4Vector W1 = W11 + W12;         
   CLP4Vector W2 = W21 + W22;    
   double W1mass = W1.mass();
   double W2mass = W2.mass();
   double wMassDiff = fabs (W1mass - kWmass);
   // if 2nd difference is bigger, take that instead
   if (fabs (W2mass - kWmass) > wMassDiff)
   {
      wMassDiff = fabs (W2mass - kWmass);
   }
   if ( wMassDiff > gW_width )
   {
      if (! alwaysFit)
      {
         return false;
      }
   }

   // calculate the errors if we're still here
   double dEb1 = jetList[kB1Index].bottomJetAbsoluteWidth_TM();
   double dEb2 = jetList[kB2Index].bottomJetAbsoluteWidth_TM();
   double dE11 = jetList[kW11Index].lightJetAbsoluteWidth_TM();
   double dE12 = jetList[kW12Index].lightJetAbsoluteWidth_TM();
   double dE21 = jetList[kW21Index].lightJetAbsoluteWidth_TM();
   double dE22 = jetList[kW22Index].lightJetAbsoluteWidth_TM();

   //////////////////////////////////
   // Here comes Kinematic Fitting //
   //////////////////////////////////

   // reset the constraints in case we played with them last iteration
   kinFitter.resetConstraints();

   // clear the input particles from last event
   kinFitter.clearAllInputParticles();

   // make the TLorentzVectors from the CLP4Vectors 
   // from (CLP4Vector) reconB1, reconW11, reconW2, etc...
   TLorentzVector b1vec   = b1.tlorentz();
   TLorentzVector w1d1vec = W11.tlorentz();
   TLorentzVector w1d2vec = W12.tlorentz();
   TLorentzVector b2vec   = b2.tlorentz();
   TLorentzVector w2d1vec = W21.tlorentz();
   TLorentzVector w2d2vec = W22.tlorentz();

   // and add the particles, with errors
   int b1int   = kinFitter.addInputParticle ( b1vec,   dEb1 );
   int w1d1int = kinFitter.addInputParticle ( w1d1vec, dE11 );
   int w1d2int = kinFitter.addInputParticle ( w1d2vec, dE12 );
   int b2int   = kinFitter.addInputParticle ( b2vec,   dEb2 );
   int w2d1int = kinFitter.addInputParticle ( w2d1vec, dE21 );
   int w2d2int = kinFitter.addInputParticle ( w2d2vec, dE22 );

   // update the created particles
   kinFitter.updateVirtualParticle (combW1c, w1d1int, w1d2int);
   kinFitter.updateVirtualParticle (combW2c, w2d1int, w2d2int);
   kinFitter.updateVirtualParticle (combB1c, b1int);
   kinFitter.updateVirtualParticle (combB2c, b2int);

   // take the chisq
   double chi2 = kinFitter.minimizeChi2();
   if (chi2 > 20 && ! alwaysFit)
   {
      return false;
   }

   // Get the modified vectors
   KinematicFitter::FourVectorVec modVec;
   kinFitter.fillAllVirtualMomentumVector(modVec,
                                           KinematicFitter::kModified);
   // After the minimization
   TLorentzVector w1AfterVec    = modVec  [combW1c];
   TLorentzVector w2AfterVec    = modVec  [combW2c];
   TLorentzVector top1AfterVec  = modVec  [combTop1c];
   TLorentzVector top2AfterVec  = modVec  [combTop2c];  

   //KinematicFitter::FourVectorVec origVec;
   // kinFitter.fillAllVirtualMomentumVector(origVec,
   //                                         KinematicFitter::kOriginal);
   // // The values before the chi^2 minimization
   // TLorentzVector w1BeforeVec   = origVec [combW1c];
   // TLorentzVector w2BeforeVec   = origVec [combW2c];
   // TLorentzVector top1BeforeVec = origVec [combTop1c];
   // TLorentzVector top2BeforeVec = origVec [combTop2c];

   double topMass1 = top1AfterVec.M();
   double topMass2 = top2AfterVec.M();

   double aveTopMass  = (topMass1 + topMass2) / 2;

   // set some more stuff
   perm.setChi2  (chi2);
   perm.setMass  (aveTopMass);
   perm.setMass2 (wMassDiff);
   perm.setType  (0);

   /////////////////////
   // Check B Tagging //
   /////////////////////
   assert (6 == vec.size());
   int numLooseBs = 0, numTightBs = 0;
   int numLooseWs = 0, numTightWs = 0;
   IVec looseTagVec, tightTagVec;
   
   for (int loop = 0; loop < 6; ++loop)
   {
      const CLPJet &jet = jetList[ vec[loop] ];
      // check loose tag
      if (jet.isBTagged(true))
      {
         if (loop < 2)
         {
            // a B jet
            ++numLooseBs;
         } else {
            ++numLooseWs;
         }
         looseTagVec.push_back (loop);
      }
      // check tight tag
      if (jet.isBTagged(false))
      {
         if (loop < 2)
         {
            // a B jet
            ++numTightBs;
         } else {
            ++numTightWs;
         }
         tightTagVec.push_back (loop);
      }
   } // for loop

   for (int which = 1; which <= 2; ++which)
   {
      int numTags;
      int numBs, numWs;
      int offset;
      IVec tagVec;
      if (1 == which)
      {
         // loose
         numTags = numLooseTags;
         numBs   = numLooseBs;
         numWs   = numLooseWs;
         offset  = 0;
         tagVec  = looseTagVec;
      } else {
         // tight
         numTags = numTightTags;
         numBs   = numTightBs;
         numWs   = numTightWs;
         offset  = 2;
         tagVec  = tightTagVec;
      } // else if
      
      int status = 0;
      if (numBs)
      {
         status |= 0x1;
      }
      if (numWs)
      {
         status |= 0x2;
      }
      // Should we mark the status as "bad"
      // 1) We have more than 2 b tags
      // 2) We have a tag outside our 6 jets
      if ( (numTags > 2) ||
           (numTags != numBs + numWs) )
      {
         status |= CLPPermutation::kOtherJetTagged;
      } // if something bad
      else {
         // nothing too bad

         // Let's figure out which jets are tagged so we can let the
         // permuation know.
         int size = (int) tagVec.size();
         assert (size <= 2);
         for (int inner = 0; inner < size; ++inner)
         {
            perm.setBtagIndex (inner + offset, tagVec[inner]);
         } // for inner
      }
      if (1 == which)
      {
         // loose
         perm.setLooseBtagStatus (status);
         perm.setNumLooseBtagJets (numTags);
      } else {
         // tight
         perm.setTightBtagStatus (status);
         perm.setNumTightBtagJets (numTags);
      }

   } // for which

   int ind = 0;

   ///////////////////////////
   // After KinematicFitter //
   ///////////////////////////
   // ind == 0
   CLP4Vector 
      bottom1a ( kinFitter.particleMom ( b1int ) ),
      bottom2a ( kinFitter.particleMom ( b2int ) );
   perm.setUserDouble (ind++, std::min (bottom1a.transMag(), 
                                        bottom2a.transMag() ) );
   perm.setUserDouble (ind++, std::min (bottom1a.mag(), 
                                        bottom2a.mag() ) );
   perm.setUserDouble (ind++, bottom1a.cosAngle   (bottom2a) );
   perm.setUserDouble (ind++, bottom1a.etaPhiDiff (bottom2a) );
   CLP4Vector 
      wboson1a ( kinFitter.virtualParticleMom (combW1c) ),
      wboson2a ( kinFitter.virtualParticleMom (combW2c) );
   perm.setUserDouble (ind++, std::min (wboson1a.transMag(), 
                                        wboson1a.transMag()) );
   perm.setUserDouble (ind++, std::min (wboson1a.mag(), 
                                        wboson1a.mag()) );
   perm.setUserDouble (ind++, wboson1a.cosAngle   (wboson2a) );
   perm.setUserDouble (ind++, wboson1a.etaPhiDiff (wboson2a) );
   CLP3Vector::V3List afterV3vec;
   afterV3vec.push_back(bottom1a);
   afterV3vec.push_back(bottom2a);
   afterV3vec.push_back(wboson1a);
   afterV3vec.push_back(wboson2a);
   double sphericity, aplanarity;
   CLPFunctions::sphericityAplanarity (sphericity, aplanarity, afterV3vec);
   perm.setUserDouble (ind++, sphericity);
   perm.setUserDouble (ind++, aplanarity);
   perm.setUserDouble (ind++, CLPFunctions::centrality (afterV3vec) );

   ////////////////////////////
   // Before KinematicFitter //
   ////////////////////////////
   // ind == 11
   CLP4Vector 
      bottom1b ( kinFitter.particleMom ( b1int, 
                                         KinematicFitter::kOriginal ) ),
      bottom2b ( kinFitter.particleMom ( b2int, 
                                         KinematicFitter::kOriginal ) );
   CLP4Vector 
      wboson1b ( kinFitter.virtualParticleMom (combW1c, 
                                               KinematicFitter::kOriginal) ),
      wboson2b ( kinFitter.virtualParticleMom (combW2c, 
                                               KinematicFitter::kOriginal) );
   perm.setUserDouble (ind++, std::min (bottom1b.transMag(), 
                                        bottom2b.transMag() ) );
   perm.setUserDouble (ind++, std::min (bottom1b.mag(), 
                                        bottom2b.mag() ) );
   perm.setUserDouble (ind++, bottom1b.cosAngle   (bottom2b) );
   perm.setUserDouble (ind++, bottom1b.etaPhiDiff (bottom2b) );

   ////////////
   // Global //
   ////////////
   CLP3Vector::V3List allV3vec;
   int size = (int) jetList.size();
   for (int inner = 0; inner < size; ++inner)
   {
      allV3vec.push_back
         (jetList[inner].mom4Vec(CLPJet::kLevel5,
                                 CLPJet::kBottomJetScaleFactor) );
   } // for inner
   CLPFunctions::sphericityAplanarity (sphericity, aplanarity, allV3vec);
   perm.setUserDouble (ind++, sphericity);
   perm.setUserDouble (ind++, aplanarity);
   perm.setUserDouble (ind++, CLPFunctions::centrality (allV3vec) );
   
   ///////////////////
   // HEPG Versions //
   ///////////////////
   // Is everything tagged?
   if (perm.isHEPGMatched())
   {
   // ind == 15
      CLP4Vector 
         bottom1h ( jetList[ vec[ kB1Index]  ].HEPGMom4Vec() ),
         bottom2h ( jetList[ vec[ kB2Index]  ].HEPGMom4Vec() );
      perm.setUserDouble (ind++, std::min (bottom1h.transMag(), 
                                           bottom2h.transMag() ) );
      perm.setUserDouble (ind++, std::min (bottom1h.mag(), 
                                           bottom2h.mag() ) );
      perm.setUserDouble (ind++, bottom1h.cosAngle   (bottom2h) );
      perm.setUserDouble (ind++, bottom1h.etaPhiDiff (bottom2h) );
      CLP4Vector
         w11h ( jetList[ vec[ kW11Index] ].HEPGMom4Vec() ),
         w12h ( jetList[ vec[ kW12Index] ].HEPGMom4Vec() ),
         w21h ( jetList[ vec[ kW21Index] ].HEPGMom4Vec() ),
         w22h ( jetList[ vec[ kW22Index] ].HEPGMom4Vec() );
      CLP4Vector 
         wboson1h = w11h + w12h,
         wboson2h = w21h + w22h;
      perm.setUserDouble (ind++, std::min (wboson1h.transMag(), 
                                           wboson1h.transMag()) );
      perm.setUserDouble (ind++, std::min (wboson1h.mag(), 
                                           wboson1h.mag()) );
      perm.setUserDouble (ind++, wboson1h.cosAngle   (wboson2h) );
      perm.setUserDouble (ind++, wboson1h.etaPhiDiff (wboson2h) );
   } // if hepg matched
   // ind == 23
   assert (ind <= CLPPermutation::kNumUserDoubles);
   return true;
}

void initialize()
{
   // Int maps
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_IntMap["num"]            = (void*) &gNumWanted;
   ns_IntMap["howmany"]        = (void*) &gNumWanted;
   ns_IntMap["start"]          = (void*) &gStartEntry;
   ns_IntMap["debug"]          = (void*) &gLocalDebug;
   ns_IntMap["section"]        = (void*) &gSection;
   ns_IntMap["scoby"]          = (void*) &gScobyDebug;
   ns_IntMap["numper"]         = (void*) &gNumPer;

   // Double map entries
   ns_DoubleMap["wwid"]        = (void*) &gW_width;
   ns_DoubleMap["chisq"]       = (void*) &gMaxChiSq;

   // Bool maps
   ns_BoolMap["count"]         = (void*) &gCountOnly;

   // String maps
   ns_StringMap["file"]        = (void*) &gFileName;
   ns_StringMap["list"]        = (void*) &gListName;
   ns_StringMap["target"]      = (void*) &gTargetName;
}

