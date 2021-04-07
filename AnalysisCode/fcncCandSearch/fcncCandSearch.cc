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
// minimum number of jets (minjet)
int gMinJets       = 0;
// Use tracks as second Z leg (usetracks)
bool gUseTracks    = true;
// only keep candidates with Z mass (76,106) (tight)
bool gTightZ       = true;
// don't check charge (noq)
bool gNoQ          = false; // false means check the charge
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
// allow exactly 2 leptons only (only2)
bool gAllowOnly2   = true;
// allow exactly 1 candidate Z per event (1cand)
bool gExactly1Cand = false;
// don't calculate chi2 (nochi2)
bool gNoChi2       = false;
// take only events with chi2 > x (minchi2)
double gChi2Cut    = 0;
// which chi2 to use (whichchi2);
int gWhichChi2     = 23;
// reweight based on filename (reweight)
bool gReweight     = false;
// no hepg processing (nohepg)
bool gNoHepg       = true;
// keep track of HEPG classification that passes 4 jet cuts (hepgclass)
bool gDoHepgClass = false;
// source file name (file)
string gFileName;
// source list name (list)
string gListName;
// output name (output)
string gOutputName;   
// good run listname (goodrunlist)
string gGoodRunListName;

// Temporary, for debugging

int gKFmatches   = 0;
int gOldMatches  = 0;
int gSameMatches = 0;
int gHEPGmatches = 0;

// forward declarations
void initialize();
void calculateChi2ForCandidate (CLPCandidate &cand);

int main(int argc, char** argv)
{
   initialize();
   parseArguments (argc, argv);
   GoodRunClass grlObj;
   SelectRunEventClass select;
   map< string, int > hepgClassifyMap;
   cout << "numWanted " << gNumWanted << endl;

   if (gGoodRunListName.length())
   {
      grlObj.loadfile (gGoodRunListName);
   }

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

   if ( (0 == gFileName.length() && 0 == gListName.length())  || 
        0 == gOutputName.length() )
   {
      cout << "invalid value of which " << gWhich << endl
           << "Use 'which', 'file', 'list', and/or 'output' options."
           << endl;
      exit(0);
   }

   ////////////////////
   // Initialization //
   ////////////////////
   bool debug = false;
   if (gChi2Cut > 0)
   {
      gMinJets = 4;
   }
   // Let's change the output name depending on which options are set
   // (to avoid future problems...)
   if (gHepgOnly)
   {      
      gOutputName += "_hepg";
      gUseTracks = gTightZ = gNoQ = gVetoSignal = gRunEvent =
         gRunEvent = gBtagOnly = gUsePem = false;
   }
   if (gUseTracks)
   {
      // change output name if we are dealing with tracks
      if (gUseAllTracks)
      {
         gOutputName += "_alltrk";
      } else {
         gOutputName += "_trk";
      }
   }
   if (gTightZ)
   {
      gOutputName += "_tight";
   }
   if (gNoQ)
   {
      gOutputName += "_noQ";
   }
   if (gVetoSignal)
   {
      gOutputName += "_nosig";
   }
   if (gRunEvent)
   {
      gOutputName += "_re";
   }
   if (gBtagOnly)
   {
      gOutputName += "_btag";
   }
   if (gUsePem)
   {
      gOutputName += "_pem";
   }
   if (gAllowOnly2)
   {
      gOutputName += "_only2";
   }
   if (gMinJets)
   {
      gOutputName += Form ("_minjets%d", gMinJets);
   }
   if (gChi2Cut)
   {
      gOutputName += Form ("_minChi2-%.1f-%d", gChi2Cut, gWhichChi2);
   }
   if (gGoodRunListName.length())
   {
      gOutputName += "_withGRL";
   }
   if (gStartEntry)
   {
      gOutputName += Form ("_start%d", gStartEntry);
   }
   if (gNumWanted)
   {
      gOutputName += Form ("_n%d", gNumWanted);
   }
   gOutputName += ".root";
  

   cout << "From: " << gFileName << endl
        << "To:   " << gOutputName << endl;

   // skim file
   TFile *outputPtr = new TFile ( gOutputName.c_str(), "RECREATE" ); 
   if (! outputPtr)
   {
      cerr << "Could not open '" << gOutputName << "' for output.  Aborting."
           << endl;
      exit(1);
   }

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
   TClonesArray &elecTca  = *elecTcaPtr;
   TClonesArray &muonTca  = *muonTcaPtr;
   TClonesArray &trackTca = *trackTcaPtr;
   TClonesArray &jetTca   = *jetTcaPtr;
   CLPHEPGCont  *contPtr  = new CLPHEPGCont;
   // counters
   Int_t runNum;    
   Int_t eventNum;
   Int_t runSection;
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
   bool isSignalMC = chainPtr->GetBranchStatus("hepg");
   if (isSignalMC && ! gNoHepg)
   {
      cout << "Processing HEPG" << endl;
      // hepg container
      chainPtr->SetBranchAddress ("hepg", &contPtr);
   } 
   else
   {
      delete contPtr;
      isSignalMC = false;
      contPtr = 0;
      cout << "Not Processing HEPG bank" << endl;
   }

   //////////////////
   // Output Trees //
   //////////////////
   // candidate tree
   CLPCandidate::CandidateList candList;
   CLPCandidate *candPtr = new CLPCandidate;
   CLPCandidate *matchedCand = new CLPCandidate;
   TTree *candTreePtr 
      = new TTree ("candFCNC", "Candidates for Top FCNC Decays");
   candTreePtr->Branch ("candidate", "CLPCandidate", &candPtr);   
   // matched tree
   TTree *matchTreePtr 
      = new TTree ("matched", "Reconstructed and HEPG matched momentums");
   matchTreePtr->Branch ("cont", "CLPHEPGCont", &contPtr);
   matchTreePtr->Branch ("runNum",   &runNum,   "runNum/I");
   matchTreePtr->Branch ("eventNum", &eventNum, "eventNum/I");
   matchTreePtr->Branch ("runSection", &runSection, "runSection/I");
   // matched candidate only tree
   TTree *matchedCandTreePtr 
      = new TTree ("matchedCand", "Fully matched reconstructed events");
   matchedCandTreePtr->Branch ("candidate", "CLPCandidate", &matchedCand);
   
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
   bool debugThisEvent = false;
   for (Int_t entryIndex = gStartEntry; entryIndex < nentries; ++entryIndex) 
   {
      // check run
      chainPtr->GetEntry(entryIndex);
      ++actualEntries;

      if (! grlObj.isGoodRun (runNum, runSection) )
      {
         continue;
      }

      if (entryIndex % 100 == 0) cout << entryIndex << endl;

      if (gHepgOnly)
      {
         // The values from matchTreePtr are linked directly to the
         // input tree.
         matchTreePtr->Fill();
         continue;
      }

      if (debug) cout << endl << endl << endl << endl << endl 
                      << "loaded entry " << entryIndex << endl;
      if (gRunEvent)
      {
         debugThisEvent = select.isSelected (runNum, eventNum);
         if (debugThisEvent)
         {
            cout << endl << endl << "entry " << entryIndex 
                 << " run " << runNum << " event " << eventNum << endl;
         }
      }
      /////////////////
      // Reweighting //
      /////////////////
      if (gReweight)
      {
         TString filename = chainPtr->GetCurrentFile()->GetName();
         if      ( filename.Contains ("ztop0p") ) weight = 3.71095E-01;
         else if ( filename.Contains ("ztop1p") ) weight = 4.83439E-02;	
         else if ( filename.Contains ("ztop2p") ) weight = 7.76635E-03;	
         else if ( filename.Contains ("ztop3p") ) weight = 1.24884E-03;	
         else if ( filename.Contains ("ztop4p") ) weight = 2.26715E-04;	
         else if ( filename.Contains ("ztop5p") ) weight = 3.53626E-01;	
         else if ( filename.Contains ("ztop6p") ) weight = 4.83439E-02;	
         else if ( filename.Contains ("ztop7p") ) weight = 7.76635E-03;	
         else if ( filename.Contains ("ztop8p") ) weight = 1.23098E-03;	
         else if ( filename.Contains ("ztop9p") ) weight = 2.22024E-04;	
         else if ( filename.Contains ("ztopb0") ) weight = 1.17879E-03;
         else if ( filename.Contains ("ztopb1") ) weight = 3.25914E-04;
         else if ( filename.Contains ("ztopb2") ) weight = 1.51355E-04;
         else if ( filename.Contains ("ztopb5") ) weight = 1.40215E-03;
         else if ( filename.Contains ("ztopb6") ) weight = 3.25190E-04;
         else if ( filename.Contains ("ztopb7") ) weight = 8.61685E-05;
         else if ( filename.Contains ("ztopc0") ) weight = 1.95493E-03;
         else if ( filename.Contains ("ztopc1") ) weight = 5.71273E-04;
         else if ( filename.Contains ("ztopc2") ) weight = 1.95074E-04;
         else if ( filename.Contains ("ztopc5") ) weight = 1.93037E-03;
         else if ( filename.Contains ("ztopc6") ) weight = 5.98706E-04;
         else if ( filename.Contains ("ztopc7") ) weight = 1.82100E-04;
         else   /* if we haven't matched it */    weight = 0.;
      } // reweighting
   
      ///////////////
      // Electrons //
      ///////////////
      elecList.clear();
      int numTCAelecs = elecTca.GetEntries();
      for (int elecLoop = 0; elecLoop < numTCAelecs; ++elecLoop)
      {
         CLPElectron *elecPtr = (CLPElectron*) elecTca.At(elecLoop);
         if ( elecPtr->isaTCE() || elecPtr->isaPHX() || 
              ( gUsePem && elecPtr->isaPEM() ) )
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
      muonList.clear();
      int numTCAmuons = muonTca.GetEntries();
      for (int muonLoop = 0; muonLoop < numTCAmuons; ++muonLoop)
      {
         CLPMuon *muonPtr = (CLPMuon*) muonTca.At(muonLoop);
         CLPCuts::MuonResult result = CLPCuts::muonCuts( *muonPtr);
         // for now, we are only allowing tight leptons
         if ( result.none() && muonPtr->isaTightLepton())
         {
            muonList.push_back( *muonPtr );
         } // if passes cuts
      } // for muonLoop
   
      ////////////
      // Tracks //
      ////////////
      trackList.clear();
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

      // If we are not using all tracks, we should mark which tracks
      // are overlaps of electrons and muons
      if (! gUseAllTracks)
      {
         //lets try to match any tracks to the electrons and muons that
         //have passed the buts above.
         CLPTrack::matchElectron (trackList, elecList); 
         CLPTrack::matchMuon     (trackList, muonList);
      }

      // If we want only two leptons, we should count and see how many
      // we have.
      if (gAllowOnly2)
      {
         // we want at most 2 leptons
         int numLeptons = (int) (elecList.size() + muonList.size());
         // we now want to count tracks, but only if they are NOT
         // matched to leptons
         int size = (int) trackList.size();
         for (int inner = 0; inner < size; ++inner)
         {
            if (! trackList[inner].isMatchedToElectron() &&
                ! trackList[inner].isMatchedToMuon() )
            {
               ++numLeptons;
            }
         } // for inner
         if (numLeptons > 2)
         {
            continue;
         }
      } // if only 2 leptons
      
      //////////
      // Jets //
      //////////
      jetList.clear();
      int numGoodMatchedJets = 0;
      int numTCAjets = jetTca.GetEntries();
      for (int jetLoop = 0; jetLoop < numTCAjets; ++jetLoop)
      {
         CLPJet *jetPtr = (CLPJet*) jetTca.At(jetLoop);
         CLPCuts::JetResult result = CLPCuts::jetCuts( *jetPtr);
         if ( result.none() )
         {
            jetList.push_back( *jetPtr );
            if (jetPtr->isTagged())
            {
               ++numGoodMatchedJets;
            }
         }
      } // for jetLoop

      ///////////////////////////////
      // Remove Jet-Track Overlaps //
      ///////////////////////////////
      CLPJet::JetList tempList;
      int jetSize = (int) jetList.size();
      int trkSize = (int) trackList.size();
      for (int jetIndex = 0; jetIndex < jetSize; ++jetIndex)
      {
         CLP3Vector jetVec = jetList[jetIndex].mom3Vec();
         bool ok = true;
         for (int trkIndex = 0; trkIndex < trkSize; ++trkIndex)
         {
            CLP3Vector trkVec = trackList[trkIndex].mom3Vec();
            if ( jetVec.etaPhiDiff( trkVec ) < 0.4)
            {
               // it's too close to a track
               ok = false;
               break;
            }
         } // for trkIndex
         if (ok) 
         {
            tempList.push_back( jetList[jetIndex] );
         }
      } // for jetIndex
      jetList = tempList;

      if (isSignalMC)
      {
         contPtr->resetJetHEPGMatching (jetList);
      }


      if (gBtagOnly)
      {
         bool anyTagged = false;
         for (int loop = 0; loop < (int)jetList.size(); ++loop)
         {
            if (jetList[loop].isBTagged())
            {
               // cool.  Got one.
               anyTagged = true;
               break;
            }
         } // for loop
         if (! anyTagged)
         {
            // Sorry, eh?  Nothing for us here
            continue;
         }
      } // if b tagged

      //////////////////////////////////////////
      // Sort the jets from high Et to low Et //
      //////////////////////////////////////////

      // since we want them in decending order, we use a little trick
      // and sort 'rbegin' to 'rend' instead of 'begin' to 'end'.
 
      sort (jetList.rbegin(), jetList.rend());

      // veto signal
      if (gVetoSignal)
      {
         // We veto events IFF there are four or more jets
         if ( (int) jetList.size() >= ns_veto_jetCount)
         {
            continue;
         }
      }
  
      /////////////////////////
      // Let's Make Some Z's //
      /////////////////////////
      if (gMinJets > (int)jetList.size() )
      {
         // don't bother
         continue;
      } // minimum jet veto
      ns_mZc_tightZ    = gTightZ;
      ns_mZc_noQ       = gNoQ;
      ns_mZc_useTracks = gUseTracks;
      makeZcandidates (candList, elecList, muonList, trackList);
      
      // Do we want exactly 1 candidate?
      if ( gExactly1Cand )
      {
         int numInZWindow = 0;
         for (CLPCandidate::CandidateListIter iter = candList.begin();
              candList.end() != iter;
              ++iter)
         {
            double Zmass = iter->Zmass();
            if ((Zmass > 76) && (Zmass < 106))
            {
               ++numInZWindow;
            }                  
         } // for iter
         if (1 != numInZWindow)
         {
            // not the right number
            continue;
         }
      } // if exactly 1 candidate wanted

      ///////////////////////////////
      // Check HEPG classification //
      ///////////////////////////////
      if (gDoHepgClass && isSignalMC)
      {
         if (candList.size() && jetList.size() >= 4)
         {
            string classify = contPtr->classify();
            ++hepgClassifyMap[classify]; 
            if ("Zll Wlv" == classify)
            {
               cout << runNum << " " << eventNum << endl;
            }
         }
      }

      // sort candidate list and fill the candidate tree
      candList.sort();
      int rank = 0;
      for (CLPCandidate::CandidateListIter iter = candList.begin();
           iter != candList.end();
           ++iter)
      {
         iter->setRank (++rank);
         iter->setRunNumber (runNum);
         iter->setEventNumber (eventNum);
         iter->setRunSection (runSection);
         iter->setEntryIndex (entryIndex);
         iter->addJets (jetList);
         iter->setWeight (weight);
         int numJets = (int) jetList.size();
         *candPtr = *iter;
         if (numJets >= 4)
         {
            if (! gNoChi2)
            {
               calculateChi2ForCandidate (*candPtr);
               if (gChi2Cut > 0)
               {
                  bool ok = false;
                  CLPPermutation::PermList permlist;
                  candPtr->getPermutations (permlist);
                  for (CLPPermutation::PermList::const_iterator 
                          iter = permlist.begin();
                       permlist.end() != iter;
                       ++iter)
                  {
                     if (iter->type() != gWhichChi2)
                     {
                        continue;
                     }
                     if (iter->chi2() > gChi2Cut)
                     {
                        ok = true;
                     } else {
                        break;
                     }
                  } // for iter
                  if (! ok)
                  {
                     continue;
                  }
               } // if chi2 cut
            } // unless NoChi2
         } // if at least 4 jets
         candTreePtr->Fill();
      } // loop over candidates

      /////////////////////
      // HEPG processing //
      /////////////////////
      if (!isSignalMC)
      {
         // don't bother going any further.
         continue;
      }
      if ("Zll Wqq" != contPtr->classify())
      {
         continue;
      }
      if (! contPtr->allReconMatched(gUseTracks) || 
          numGoodMatchedJets < 4)
      {
         // We don't have a fully matched candidate, so don't bother
         continue;
      }
      ++numMatched;
      matchTreePtr->Fill();
      const int kNumInteresting = 2;
      CLPHEPGObject objArray[kNumInteresting];
      objArray[0] = contPtr->object(CLPHEPGCont::kLepton1);
      objArray[1] = contPtr->object(CLPHEPGCont::kLepton2);
      CLPCandidate cand;
      for (int loop = 0; loop < kNumInteresting; ++loop)
      {
         int reconIndex = objArray[loop].taggedToReconIndex();
         if (reconIndex < 0)
         {
            int trackReconIndex = objArray[loop].taggedToTrackIndex();
            if (trackReconIndex < 0)
            {
               continue;
            }
            // O.k. So, we have an electron or a muon, but its only
            // reconstructed as a track.  But just to further
            // complicate matters, we need to know whether it
            // should be added as a muon track or electronTrack.
            // So...
            CLPTrack *trackPtr = (CLPTrack *) trackTca.At(trackReconIndex);
            if ( CLPObject::kElectron == objArray[loop].reconType() )
            {
               // as an electron
               cand.addTrack (*trackPtr, CLPTrack::kAsElectronMom);
            } else {
               // as a muon
               cand.addTrack (*trackPtr, CLPTrack::kAsMuonMom);
            }
            continue;
         }
         if ( CLPObject::kElectron == objArray[loop].reconType() )
         {
            CLPElectron *elecPtr = (CLPElectron *) elecTca.At(reconIndex);
            cand.addElectron (*elecPtr);
            continue;
         }
         if ( CLPObject::kMuon == objArray[loop].reconType() )
         {
            CLPMuon *muonPtr = (CLPMuon *) muonTca.At(reconIndex);
            cand.addMuon (*muonPtr);
            continue;
         }
      } // for loop
      cand.addJets (jetList);
      cand.setWeight (weight);
      cand.calculate();
      cand.setRunNumber (runNum);
      cand.setEventNumber (eventNum);
      cand.setRunSection (runSection);
      cand.setEntryIndex (entryIndex);
      *matchedCand = cand;
      int numJets = cand.numJets();
      for (int jIndex = 0; jIndex < numJets; ++jIndex)
      {
         //if (! cand.jet(jIndex).isTagged())
         //{
         //   continue;
         //}
         CLP3Vector mom = cand.jet(jIndex).mom3Vec();
         CLP4Vector hepg = cand.jet(jIndex).HEPGMom4Vec();         
      } // for jIndex
      matchedCandTreePtr->Fill();
   } // for entryIndex
   for (map< string, int >::const_iterator iter = hepgClassifyMap.begin();
        hepgClassifyMap.end() != iter;
        ++iter)
   {
      cout << Form ("%14s : %5d",iter->first.c_str(),  iter->second)
           << endl;
   } // for iter
   
   outputPtr->Write();
   return 0;
}

//////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////// //
// //    *****************     Subroutines      *****************    // //
// //////////////////////////////////////////////////////////////////// //
//////////////////////////////////////////////////////////////////////////

void calculateChi2ForCandidate (CLPCandidate &cand)
{
   int numJets = cand.numJets();
   if (numJets < 4)
   {
      // can't do anything
      return;
   }

   // Method 8 only, for now
   for (int method = 9; method <= 13; ++method)
   {
      if (method >= 9 && method <= 12)
      {
         continue;
      }
      double value = sq (cand.best_ttbar_Diff(method));
      CLPPermutation old;
      old.setCombIndex ( cand.bestIndex() );
      old.setFromAndChoose (numJets, 4);
      old.setType (10 + method);
      old.setChi2 (value);
      cand.addPermutation (old);
   }
}

void initialize()
{
   // Double maps
   ns_DoubleMap["minchi2"]     = (void*) &gChi2Cut;
   // Int maps
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_IntMap["num"]            = (void*) &gNumWanted;
   ns_IntMap["howmany"]        = (void*) &gNumWanted;
   ns_IntMap["start"]          = (void*) &gStartEntry;
   ns_IntMap["minjet"]         = (void*) &gMinJets;
   ns_IntMap["whichchi2"]      = (void*) &gWhichChi2;
   // Bool maps
   ns_BoolMap["usetracks"]     = (void*) &gUseTracks;
   ns_BoolMap["usealltracks"]  = (void*) &gUseAllTracks;
   ns_BoolMap["tight"]         = (void*) &gTightZ;
   ns_BoolMap["noq"]           = (void*) &gNoQ;
   ns_BoolMap["hepgonly"]      = (void*) &gHepgOnly;
   ns_BoolMap["veto"]          = (void*) &gVetoSignal;
   ns_BoolMap["runevent"]      = (void*) &gRunEvent;
   ns_BoolMap["btag"]          = (void*) &gBtagOnly;
   ns_BoolMap["pem"]           = (void*) &gUsePem;
   ns_BoolMap["only2"]         = (void*) &gAllowOnly2;
   ns_BoolMap["1cand"]         = (void*) &gExactly1Cand;
   ns_BoolMap["nochi2"]        = (void*) &gNoChi2;
   ns_BoolMap["reweight"]      = (void*) &gReweight;
   ns_BoolMap["nohepg"]        = (void*) &gNoHepg;
   ns_BoolMap["hepgclass"]     = (void*) &gDoHepgClass;
   // String maps
   ns_StringMap["file"]        = (void*) &gFileName;
   ns_StringMap["list"]        = (void*) &gListName;
   ns_StringMap["output"]      = (void*) &gOutputName;
   ns_StringMap["goodrunlist"] = (void*) &gGoodRunListName;
}
