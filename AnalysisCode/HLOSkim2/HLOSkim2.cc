// -*- C++ -*-
#include <time.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"

#include "CLPClasses/CLPClasses.h"
#include "HighLevelObjects/HighLevelObjects.hh"
#include "TopMods/CollectionAndViewTypeFinder.hh"
#include "JetUser/JetEnergyCorrections.hh"
#include "AnalysisNamespace/AnalysisNamespace.hh"
#include "RootFileCollection/TreeFileInterface.h"
#include "RootFileCollection/TopTreeNamespace.h"
#include "RootFileCollection/OptionUtils.h"
#include "CLPClasses/dout.h"
#include "CLP_HLO_SkimUtils.h"

using namespace std;
using namespace TopTreeNamespace;
using namespace AnalysisNamespace;
using namespace CLPFunctions;
using namespace CLP_HLO_SkimUtils;

// Jet Correction Constants
const Int_t kJCLevel_five = 5;
const Int_t kJCLevel_seven = 7;
//int coneSize = 1; // 0=0.4, 1=0.7 and 2=1.0
const Int_t kJCConeSize_04 = 0;
const Int_t kJCConeSize_07 = 1;
const Int_t kJCConeSize_10 = 2;
const Int_t kJCVersion = 5;
const Int_t kJCSysCode = 0;
const Int_t kJCiMode_Data = 1;
const Int_t kJCiMode_MC = 0;

// Polluting the gLobal namespace - gLobal variables
// Note, name in parenthesis is command-line variable name.
// e.g. 'which=12 usetracks=1' will set gWhich = 12 and gUseTracks = false

// Which file to grab (which)
int    gWhich           = 0;
// require at least N jets (njets)
int    gNjets           = 0;
// maximum output size in megabytes (maxtree)
int    gMaxTreeSize     = 500;
// require all hadronic cuts (allhad)
bool   gRequireAllHad   = false;
// Do signal HEPG processing (hepg)
bool   gDoHepg          = false;
// force MC (forcemc) - Needed for Gen 5 MC
bool   gForceMC         = false;
// check bottom content (bottom)
bool   gBottom          = false;
// check charm content (charm)
bool   gCharm           = false;
// check light content (light)
bool   gLight           = false;
// check heavy flavor content (heavy)
bool   gHeavy           = false;
// require 4 jets (4jets)
bool   gRequire4Jets    = false;
// require good Z (goodz)
bool   gRequireGoodZ    = false;
// use variable weights (weights)
bool   gVariableWeights = false;
// use a single weight  (useweight)
double gUseWeight       = 1.;
// apply signal veto in skim (veto)
bool   gSignalVeto      = false;
// use old (2004) mistag parameterization (oldmis)
// defined in CLP_HLO_SkimUtils

// forward declarations
void localInitialize();
void setupRootFileTags (string &rootFileTags);
void setWeight (const TString &filename, double &weight);

int main(int argc, char** argv)
{
   // parce command line arguments
   localInitialize();     // hook up local options
   setupDefaultOptions(); // hook up standard options
   OptionUtils::parseArguments (argc, argv);

   // set default to data
   CLPFunctions::setDataType( CLPNamespace::kData );
   CLP3Vector::setOutput3Mode (CLP3Vector::kPtEtaPhiOutputMode);
   
   // boolean variables
   bool debug           = false;
   bool isSignalMC      = false;
   bool matchHepgZs     = false;
   bool initializedBtag = false;

   if (gDoHepg)
   {
      isSignalMC = true;
   }

   // // set skim name
   // if (111 == gWhich)
   // {
   //    // all signal - Gen 6
   //    isSignalMC  = true; // signal MC
   //    ns_outputName = "/cdf/scratch/cplager/fcnc/all_ttop0f";
   //    ns_listName   = "rootlists/gen6_fcnc.list";
   // }

   Int_t imode;

   ///////////////
   // setup TFI //
   ///////////////
   string rootFileTags;
   setupRootFileTags (rootFileTags);
   setupTFI ("TopTree", rootFileTags, true);

   // Tell the tfi what branches we want
   ns_tfi.addTcaBranch (TopTreeNamespace::kEvtBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kSummaryBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kZvtxsBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kHepgBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kObspBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kElectronBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kMuonBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kOffltrackBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kJetBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kL2CalBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kPrivertexBranch);
   ns_tfi.addTcaBranch (TopTreeNamespace::kTrigInfoBranch);

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
   elecCLPTcaPtr->BypassStreamer(true);
   muonCLPTcaPtr->BypassStreamer(true);
   trackCLPTcaPtr->BypassStreamer(true);
   jetCLPTcaPtr->BypassStreamer(true);
   // Various counters
   Int_t runNum;    
   Int_t eventNum;
   Int_t runSection;
   Int_t numVtxs;
   Int_t topEvent;
   Int_t numElecs;
   Int_t numMuons;
   Int_t numTracks;
   Int_t numJets; 
   Int_t numDilLeptons;
   Int_t trigSumWord;
   UInt_t heavyFlavorWord = 0;
   Double_t weight;
   Double_t jetZ0;
   // single variables
   CLPHEPGCont *contPtr = new CLPHEPGCont;
   CLP3Vector  *met3VecPtr  = new CLP3Vector;

   // open file for output
   cout << "Skimming to " << ns_outputName << endl;

   TFile *filePtr = new TFile (ns_outputName.c_str(), "RECREATE");
   if (! filePtr)
   {
      cerr << "Could not open '" << ns_outputName 
           << "' for output.  Aborting." << endl;
      exit (1);
   }
   // setup the tree
   TTree *outputTreePtr = new TTree ("CLPTree", "CLPClasses Skim");
   // split up file if larger than 500 MB
   const long long kBytesPerMB = 1024 * 1024;
   outputTreePtr->SetMaxTreeSize( gMaxTreeSize * kBytesPerMB );
   
   // counters, etc
   outputTreePtr->Branch (CLPNamespace::kRunBranch,     
                          &runNum,          "runNum/I");
   outputTreePtr->Branch (CLPNamespace::kEventBranch,       
                          &eventNum,        "eventNum/I");
   outputTreePtr->Branch (CLPNamespace::kSectionBranch,   
                          &runSection,      "runSection/I");
   outputTreePtr->Branch (CLPNamespace::kNumVertexBranch, 
                          &numVtxs,         "numVtxs/I");
   outputTreePtr->Branch (CLPNamespace::kWeightBranch,    
                          &weight,          "weight/D");
   outputTreePtr->Branch (CLPNamespace::kTopEventBranch,
                          &topEvent,        "topEvent/I");
   outputTreePtr->Branch (CLPNamespace::kNumDilLepBranch,
                          &numDilLeptons,   "numDilLep/I");
   outputTreePtr->Branch (CLPNamespace::kTrigSumWordBranch,
                          &trigSumWord,     "trigSumWord/I");
   outputTreePtr->Branch (CLPNamespace::kHeavyFlavorBranch,
                          &heavyFlavorWord, "heavyFlavorWord/i");
   outputTreePtr->Branch (CLPNamespace::kJetZ0Branch,
                          &jetZ0,           "jetZ0/D");
   // TCAs
   outputTreePtr->Branch (CLPNamespace::kElectronBranch,  &elecCLPTcaPtr);
   outputTreePtr->Branch (CLPNamespace::kMuonBranch,      &muonCLPTcaPtr);
   outputTreePtr->Branch (CLPNamespace::kTrackBranch,     &trackCLPTcaPtr);
   outputTreePtr->Branch (CLPNamespace::kJetBranch,       &jetCLPTcaPtr);

   if (isSignalMC || matchHepgZs)
   {
      // single objects
      outputTreePtr->Branch (CLPNamespace::kHepgBranch, 
                             kHepgContClassName,  
                             &contPtr);
   }
   outputTreePtr->Branch (CLPNamespace::kMetBranch,
                          k3VectorClassName,
                          &met3VecPtr);

   // setup the defaults
   CLPNamespace::ns_ELETYPE_DefEm           = ELETYPE_DefEm;  
   CLPNamespace::ns_ELETYPE_Phoenix         = ELETYPE_Phoenix;
   CLPNamespace::ns_MUOTYPE_DefMuon         = MUOTYPE_DefMuon;
   CLPNamespace::ns_MUOTYPE_DefStublessMuon = MUOTYPE_DefStublessMuon;
   CLPNamespace::ns_JETTYPE_Jetclu_04_LJ    = JETTYPE_Jetclu_04_LJ;
   CLPNamespace::ns_JETTYPE_Jetclu_07_LJ    = JETTYPE_Jetclu_07_LJ;
   CLPNamespace::ns_OTRKTYPE_DefTrack       = OTRKTYPE_DefTrack;
   CLPNamespace::ns_OTRKTYPE_TrackLepton    = OTRKTYPE_TrackLepton;

   ////////////////////////////////////////////
   // ////////////////////////////////////// //
   // // //////////////////////////////// // //
   // // // ////////////////////////// // // //
   // // // // Start The Large Loop // // // //
   // // // ////////////////////////// // // //
   // // //////////////////////////////// // //
   // ////////////////////////////////////// //
   ////////////////////////////////////////////
   
   int numActual = 0;
   Int_t entryIndex = 0;
   finalTFIsetup();
   while ( ns_tfi.getNextEvent() )
   {
      if (ns_tfi.isNewFile())
      {
         if (ns_debugFlag) cout << " new file" << endl;
         TopNtupleDecoder *decoderPtr =  ns_tfi.getTopDecoder();
         CLPNamespace::ns_ELETYPE_DefEm           
            = decoderPtr->getEleCollType(kDefEmObj);
         CLPNamespace::ns_ELETYPE_Phoenix         
            = decoderPtr->getEleCollType(kPhxEmObj);
         CLPNamespace::ns_MUOTYPE_DefMuon         
            = decoderPtr->getMuoViewType(kDefMuons);
         CLPNamespace::ns_MUOTYPE_DefStublessMuon
            = decoderPtr->getMuoViewType(kDefStublessMuons);
         CLPNamespace::ns_JETTYPE_Jetclu_04_LJ    
            = decoderPtr->getJetCollType(kJetClu04LJ);
         CLPNamespace::ns_JETTYPE_Jetclu_07_LJ    
            = decoderPtr->getJetCollType(kJetClu07LJ);
         CLPNamespace::ns_OTRKTYPE_DefTrack       
            = decoderPtr->getOfflTrkCollType(kDefTrks);
         CLPNamespace::ns_OTRKTYPE_TrackLepton    
            = decoderPtr->getOfflTrkCollType(kTrackLeptons);
         // Is this MC or data?
         if (decoderPtr->isMonteCarlo() || gForceMC)
         {
            // MC
            imode = kJCiMode_MC;
            CLPFunctions::setDataType( CLPNamespace::kMC );
            cout << "Skiming as MC" << endl;
            if (! initializedBtag)
            {
               if (ns_useOldMistag)
               {
                  // 2004 : tight loose
                  initialize2004BTagObjects 
                     ("btag_533_nt_mistag_taggable_all.root", 
                      "btag_533_nt_loose_mistag_taggable_all.root");
               } else {
                  // 2007 : ultra tight loose
                  initialize2007BTagObjects 
                     ("ultratight_withnew0i_taggable.root",
                      "tight_withnew0i_taggable.root",
                      "loose_withnew0i_taggable.root");
               }
               initializedBtag = true;
            } // if b tags not initialized
         } else {
            // Data
            imode = kJCiMode_Data;
            CLPFunctions::setDataType( CLPNamespace::kData );
            cout << "Skiming as DATA" << endl;
         }
      } // if new file      

      ++entryIndex;

      if (entryIndex % 1000 == 0) cout << entryIndex << endl;

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
      evt *evtPtr = (evt*) ns_tfi.tcaPtr(kEvtBranch)->At(0);
      summary *summaryPtr = (summary*) ns_tfi.tcaPtr(kSummaryBranch)->At(0);
      
      runNum     = CLPNamespace::ns_runNumber   = evtPtr->runNumber;
      eventNum   = CLPNamespace::ns_eventNumber = evtPtr->eventNumber;
      runSection = evtPtr->runSection;
      weight     = gUseWeight;
      numVtxs    = 0;
      int numPossibleVtx = ns_tfi.tcaPtr(kZvtxsBranch)->GetEntries();
      for (int loop = 0; loop < numPossibleVtx; ++loop)
      {
         zvtxs *vtxPtr = (zvtxs*) ns_tfi.tcaPtr(kZvtxsBranch)->At(loop);
         if (vtxPtr->quality >= 12)
         {
            ++numVtxs;
         } // if quality 12
      } // for loop
      TClonesArray *electronTcaPtr  
         = ns_tfi.tcaPtr (TopTreeNamespace::kElectronBranch);  
      TClonesArray *muonTcaPtr      
         = ns_tfi.tcaPtr (TopTreeNamespace::kMuonBranch);
      TClonesArray *offltrackTcaPtr 
         = ns_tfi.tcaPtr (TopTreeNamespace::kOffltrackBranch);
      TClonesArray *jetTcaPtr       
         = ns_tfi.tcaPtr (TopTreeNamespace::kJetBranch);
      TClonesArray *hepgTcaPtr      
         = ns_tfi.tcaPtr (TopTreeNamespace::kHepgBranch);
      Int_t numNTelecs  = electronTcaPtr->GetEntries();  
      Int_t numNTmuons  = muonTcaPtr->GetEntries();
      Int_t numNTtracks = offltrackTcaPtr->GetEntries();
      Int_t numNTjets   = jetTcaPtr->GetEntries();
      // Start with the Missing Et calculated in the top ntuple
      *met3VecPtr = 
         CLP3Vector::fromEtaPhiMag (0., // (eta) since only trans component
                                    summaryPtr->fmuoMetPhi,
                                    summaryPtr->fmuoMet);
      topEvent      = summaryPtr->fTopEventClass;
      jetZ0         = summaryPtr->fjetZV;
      numDilLeptons = summaryPtr->fnTightLepton + 
         summaryPtr->fnNonIsoLepton + summaryPtr->fnLooseLepton;
      trigSumWord   = 0;
      if (ns_tfi.GetEntries(TopTreeNamespace::kTrigInfoBranch))
      {
         trigInfo *trigInfoPtr = 
            (trigInfo*) ns_tfi.At(TopTreeNamespace::kTrigInfoBranch, 0);
         trigSumWord = trigInfoPtr->summaryWord;
      }

      //                                                                //
      //                                                                //
      ////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////

      if (debug || ns_debugFlag) cout << endl << endl
                                    << "entry " << entryIndex 
                                    << " run " << runNum
                                    << " event " << eventNum
                                    << endl;

      ////////////////////////
      // Check Heavy Flavor //
      ////////////////////////
      if (gCharm || gLight)
      {
         bool hasBottom, hasCharm;
         hepgHeavyFlavorCheck (hasBottom, hasCharm, hepgTcaPtr);
         // Do we want this event
         if (gCharm)
         {
            // this should have charm and NOT bottom
            if (hasBottom)
            {
               // no solo charm found
               continue;
            }
         } // charm check
         else if (gLight)
         {
            // this should have no heavy flavor
            if (hasCharm || hasBottom)
            {
               continue;
            }
         } // light check
      }

      ///////////////
      // Electrons //
      ///////////////
      elecCLPTcaPtr->Clear();
      numElecs = 0;
      for (int elecLoop = 0; elecLoop < numNTelecs; ++elecLoop)
      {
         CLPElectron myElectron = clpElectron(elecLoop, electronTcaPtr);
         if (myElectron.isUndefined())
         {
            // don't bother with electrons that aren't in one of
            // our classifications
            continue;
         }
         new ( (*elecCLPTcaPtr) [numElecs++] ) CLPElectron( myElectron );
      } // for elecLoop

      ///////////
      // Muons //
      ///////////
      muonCLPTcaPtr->Clear();
      numMuons = 0;
      for (int muonLoop = 0; muonLoop < numNTmuons; ++muonLoop)
      {
         CLPMuon myMuon = clpMuon(muonLoop, muonTcaPtr);
         if (myMuon.isUndefined())
         {
            // don't bother with muons that aren't in one of
            // our classifications
            continue;
         }
         new ( (*muonCLPTcaPtr) [numMuons++] ) CLPMuon( myMuon );         
      } // for muonLoop

      ////////////
      // Tracks //
      ////////////
      trackCLPTcaPtr->Clear();
      numTracks = 0;
      for (int trackLoop = 0; trackLoop < numNTtracks; ++trackLoop)
      {
         CLPTrack myTrack = clpTrack(trackLoop, offltrackTcaPtr);
         if (myTrack.BCPt() < 0 && myTrack.Pt() < 10)
         {
            // this is a useless track (for now) so lets not even
            // store it
            continue;
         }
         if (myTrack.isUndefined())
         {
            // don't bother with tracks that aren't in one of
            // our classifications
            continue;
         }
         new ( (*trackCLPTcaPtr) [numTracks++] ) CLPTrack( myTrack );         
      } // for trackLoop

      //////////
      // Jets //
      //////////
      jetCLPTcaPtr->Clear();
      numJets = 0;
      if (numVtxs < 1)
      {
         numVtxs = 1;
      }
      JetEnergyCorrections *jetCorrArray[kTotalNumJetCorr];
      for (int cone = 0; cone < 2; ++cone)
      {
         int offset = cone * CLPJet::kNumJetCorrections;
         int conesize;
         if (0 == cone)
         {
            conesize = kJCConeSize_04;
         } else {
            conesize = kJCConeSize_07;
         }
         for (int inner = 0; inner < CLPJet::kNumJetCorrections; ++inner)
         {
            int level;
            if (inner % 2)
            {
               // odd, therefore level 7
               level = kJCLevel_seven;
            } else {
               // even, therefore level 5
               level = kJCLevel_five;
            }
            jetCorrArray[offset + inner] = 
               new JetEnergyCorrections ("JetCorrections","JetCorrections",
                                         level, numVtxs,
                                         conesize, kJCVersion,
                                         kJCSysCode, runNum, imode);
            (*jetCorrArray[offset + inner]).setCemCorON (false);
         } // for inner
         // Levels 5 & 7 +1 sigma
         (*jetCorrArray[CLPJet::kLevel5Plus + offset])
            .setTotalSysUncertainties(+1);
         (*jetCorrArray[CLPJet::kLevel5Plus + offset])
            .setRelCorSystematicsMinimal(true);
         (*jetCorrArray[CLPJet::kLevel7Plus + offset])
            .setTotalSysUncertainties(+1);
         (*jetCorrArray[CLPJet::kLevel7Plus + offset])
            .setRelCorSystematicsMinimal(true);
         // Levels 5 & 7 -1 sigma
         (*jetCorrArray[CLPJet::kLevel5Minus + offset])
            .setTotalSysUncertainties(-1);
         (*jetCorrArray[CLPJet::kLevel5Minus + offset])
            .setRelCorSystematicsMinimal(true);
         (*jetCorrArray[CLPJet::kLevel7Minus + offset])
            .setTotalSysUncertainties(-1);
         (*jetCorrArray[CLPJet::kLevel7Minus + offset])
            .setRelCorSystematicsMinimal(true);
      } // for cone
      int numGoodJets = 0;
      int numOkJets  = 0;
      if ( isMC() )
      {
         clearBTagObjects();
      }
      // make a std::vector of the jets
      TVector3Vec jet3vecVec;
      
      for (int jetLoop = 0; jetLoop < numNTjets; ++jetLoop)
      {
         CLPJet myJet = clpJet(jetLoop, jetTcaPtr, jetCorrArray);
         if (myJet.isUndefined())
         {
            // don't bother with jets that aren't in one of
            // our classifications
            continue;
         }
         if ((myJet.Et() > 13 * GEV) &&
             (fabs(myJet.eta()) < 2.5) )
         {
            ++numGoodJets;
         }
         if (fabs(myJet.eta()) < 2.5)
         {
            ++numOkJets;
         }
         new ( (*jetCLPTcaPtr) [numJets++] ) CLPJet( myJet );         
         if (gHeavy)
         {
            CLPCuts::JetResult result = CLPCuts::jetCuts (myJet);
            if (result.none())
            {
               // This jet passes our cuts, so lets take it
               jet3vecVec.push_back( myJet.mom3Vec().tvector3() );
            }
         }
      } // for jetLoop

      if (gHeavy)
      {
         heavyFlavorWord = checkHeavyFlavor(hepgTcaPtr, jet3vecVec);
         if (ns_debug)
         {
            decodeHeavyFlavorWord (heavyFlavorWord);
         }
      }

      // Apply mistag parameterization
      if ( isMC() )
      {
         applyMistagMatricies();
         jetBtagObspAccounting (jetCLPTcaPtr, ns_tfi.tcaPtr (kObspBranch), 
                                runNum);
         if (ns_debugFlag) dumpMistagRates (jetCLPTcaPtr);
      }

      // delete the jet corrections
      for (int loop = 0; loop < kTotalNumJetCorr; ++loop)
      {
         delete jetCorrArray[loop];
      }

      //////////////////////////////////
      // //////////////////////////// //
      // // Do We Want This Event? // //
      // //////////////////////////// //
      //////////////////////////////////

      if (gRequire4Jets)
      {
         // we want three good jets (Et and eta cuts)
         // and fourth jet that passes eta cuts
         if (numGoodJets < 3 || numOkJets < 4)
         {
            // don't want this event
            continue;
         }
      } // if we're requiring at least 4 good jets

      if (gNjets && numGoodJets < gNjets)
      {
         // don't want this event
         continue;
      }

      if (gRequireAllHad)
      {
         if (numGoodJets < 6)
         {
            // sorry, no
            continue;
         }
         int numL2cal = ns_tfi.GetEntries (kL2CalBranch);
         const float kHex2GEV = 0.125; // conversion factor ADC counts to GeV
         float sumEtL2    = 0.;
         // float sumEtL2_15 = 0.;
         for (int l2index = 0; l2index < numL2cal; ++l2index)
         {
            l2Cal* l2CalPtr = (l2Cal*) ns_tfi.At(kL2CalBranch, l2index); 
            if (2 == l2CalPtr->typL2)
            {
               float etL2 = kHex2GEV * (l2CalPtr->etEmL2 + l2CalPtr->etHadL2);
               sumEtL2 += etL2;
               // if (etL2 >= 15 * GEV)
               // {
               //    sumEtL2_15 += etL2;
               // } // if et > 15 GeV
            } // if type 2
         } // for l2index
         if (sumEtL2 < 175 * GEV)
         {
            continue;
         }
      } // if require all hadronic cuts

      // Do we have a good Z requirement?
      if (gRequireGoodZ)
      {
         if (! hasGoodZs(*elecCLPTcaPtr, *muonCLPTcaPtr, 
                         *trackCLPTcaPtr, *jetCLPTcaPtr))
         {
            continue;
         }
      }

      // If this is data, we should take this and stop now
      if (CLPFunctions::isData())
      {
         // Let's fill this and get out
         outputTreePtr->Fill();
         ++numActual;
         continue;
      }

      ///////////////////
      // HEPG Matching //
      // IFF Signal MC //
      ///////////////////
      if (isSignalMC)
      {
         fillHEPGCont (*contPtr, hepgTcaPtr);
      } // if signal MC      
      else if (matchHepgZs)
      {
         fillHEPGContWithZs (*contPtr, hepgTcaPtr);
      } // match HEPG Zs

      if (isSignalMC || matchHepgZs)
      {
         // Match leptons
         doLeptonHEPGMatching (*contPtr, *elecCLPTcaPtr, 
                               *muonCLPTcaPtr, *trackCLPTcaPtr);
         // Match jets
         int numJets = jetCLPTcaPtr->GetEntries();
         for (int outer = 0; outer < CLPJet::kNumberCones; ++outer)
         {
            CLPJet::JetPtrList jetPtrList;            
            CLPJet::JetCone jetCone = (CLPJet::JetCone) outer;
            for (int inner = 0; inner < numJets; ++inner)
            {
               CLPJet *jetPtr = (CLPJet*) jetCLPTcaPtr->At (inner);
               if (jetPtr->jetCone() == jetCone)
               {
                  jetPtrList.push_back (jetPtr);
               }
            } // for inner
            doJetHEPGMatching (*contPtr, jetPtrList);
         } // for outer
      } // if isSignal || matchHepgZs
    
      // O.k. We're still here.
      // Should we check the filename to figure out a weight?
      if (gVariableWeights)
      {
         TString filename = ns_tfi.filename();
         setWeight (filename, weight);
      } // if variable weights
      outputTreePtr->Fill();
      ++numActual;
   } // while getNextEvent

   ///////////////////////////////////////
   //    We've survived the big loop.   //
   // Let's finish this up and go home. //
   ///////////////////////////////////////
   TFile *currentFilePtr = outputTreePtr->GetCurrentFile();
   currentFilePtr->cd();
   outputTreePtr->Write(); 
   currentFilePtr->Close();
   cout << "finished gracefully with " << numActual << " events." << endl;
   return 0;
}

void setupRootFileTags (string &rootFileTags)
{
   // modify the output root file appropriately
   if (gRequire4Jets)
   {
      rootFileTags += "_4j";
   }
   if (ns_useOldMistag)
   {
      rootFileTags += "_oldmistag";
   }
   if (gRequireGoodZ)
   {
      rootFileTags += "_goodZ";
   }
   if (gSignalVeto)
   {
      rootFileTags += "_nosig";
   }
   if (gNjets)
   {
      rootFileTags += Form ("_njet%d", gNjets);
   }
   if (gRequireAllHad)
   {
      rootFileTags += "_allhad";
   }
   if (gBottom)
   {
      rootFileTags += "_bottom";
   } else if (gCharm)
   {
      rootFileTags += "_charm";
   } else if (gLight)
   {
      rootFileTags += "_light";
   }
   if (gHeavy)
   {
      rootFileTags += "_heavy";
   }
}

void setWeight (const TString &filename, double &weight)
{
   weight = 0.;
   if      ( filename.Contains ("ztop0p") ) weight = 3.79200E-01;
   else if ( filename.Contains ("ztop1p") ) weight = 5.18400E-02;
   else if ( filename.Contains ("ztop2p") ) weight = 8.32800E-03;
   else if ( filename.Contains ("ztop3p") ) weight = 1.32000E-03;
   else if ( filename.Contains ("ztop4p") ) weight = 2.38080E-04;
   else if ( filename.Contains ("ztop5p") ) weight = 3.79200E-01;
   else if ( filename.Contains ("ztop6p") ) weight = 5.18400E-02;
   else if ( filename.Contains ("ztop7p") ) weight = 8.32800E-03;
   else if ( filename.Contains ("ztop8p") ) weight = 1.32000E-03;
   else if ( filename.Contains ("ztop9p") ) weight = 2.38080E-04;
   else if ( filename.Contains ("ztopb0") ) weight = 1.22640E-03;
   else if ( filename.Contains ("ztopb1") ) weight = 3.21600E-04;
   else if ( filename.Contains ("ztopb2") ) weight = 9.24000E-05;
   else if ( filename.Contains ("ztopb5") ) weight = 1.22640E-03;
   else if ( filename.Contains ("ztopb6") ) weight = 3.21600E-04;
   else if ( filename.Contains ("ztopb7") ) weight = 9.24000E-05;
   else if ( filename.Contains ("ztopc0") ) weight = 1.85143E-03;
   else if ( filename.Contains ("ztopc1") ) weight = 5.67429E-04;
   else if ( filename.Contains ("ztopc2") ) weight = 1.83429E-04;
   else if ( filename.Contains ("ztopc5") ) weight = 1.85143E-03;
   else if ( filename.Contains ("ztopc6") ) weight = 5.67429E-04;
   else if ( filename.Contains ("ztopc7") ) weight = 1.83429E-04;
 }

void localInitialize()
{
   OptionUtils::addOptionKey ("which",     gWhich);
   OptionUtils::addOptionKey ("njets",     gNjets);
   OptionUtils::addOptionKey ("maxtree",   gMaxTreeSize);
   OptionUtils::addOptionKey ("hepg",      gDoHepg);
   OptionUtils::addOptionKey ("forcemc",   gForceMC);
   OptionUtils::addOptionKey ("allhad",    gRequireAllHad);
   OptionUtils::addOptionKey ("bottom",    gBottom);
   OptionUtils::addOptionKey ("charm",     gCharm);
   OptionUtils::addOptionKey ("light",     gLight);
   OptionUtils::addOptionKey ("heavy",     gHeavy);
   OptionUtils::addOptionKey ("4jets",     gRequire4Jets);
   OptionUtils::addOptionKey ("goodz",     gRequireGoodZ);
   OptionUtils::addOptionKey ("weights",   gVariableWeights);
   OptionUtils::addOptionKey ("useweight", gUseWeight);  
   OptionUtils::addOptionKey ("veto",      gSignalVeto);
   OptionUtils::addOptionKey ("oldmis",    ns_useOldMistag);
   OptionUtils::addOptionKey ("debug",     ns_debugFlag);
}

