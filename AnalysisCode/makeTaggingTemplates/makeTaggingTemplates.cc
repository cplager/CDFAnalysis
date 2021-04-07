// -*- C++ -*-
#include "CLPClasses/dout.h"
#include "TH1F.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <algorithm>
#include <cstdlib>
#include "CLPClasses/CLPClasses.h"
#include "RootFileCollection/TreeFileInterface.h"
#include "RootFileCollection/TFINamespace.h"
#include "RootFileCollection/OptionUtils.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace TFINamespace;
using namespace OptionUtils;

// (ugh..) Global Variables
// maximum allowed jets (numjets)
int gNumJets = 3;
// number of taggers to use (numtag)
int gNumTaggers = 2;
// verbose mode (verbose)
bool gVerbose = false;
// uses the tagging information as is (usetag)
// instead of using the per jet tagging probability
bool gUseTagInfo = false;
// number of events wanted (num)
int gNumWanted = 10;
// veto events with more jets than 'gNumJets' (jetveto)
int gJetVeto   = true;
// histogram name (histname)
string gHistName = "histname";

// hardwired maximum allowed number of jets
const int kMaxJet = 5;

// forward declarations
void localInitialize();

// fill histogram based off of tagDigVec
void fillHistWithTagDist (TH1F *histPtr, const DVec &tagDistVec,
                          int offset, double eventWeight = 1.);

void coutDVec (const DVec &vec);

int main(int argc, char** argv)
{
   setupDefaultOptions(); // hook up standard options
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);

   // add output file tags
   string tag;
   if (gNumJets)
   {
      tag += Form ("_%djets", gNumJets);
   }
   if (! gJetVeto)
   {
      tag += "_noveto";
   }
   if (3 == gNumTaggers)
   {
      tag += "_ultra";
   }
   if (gNumTaggers < 2 || gNumTaggers > 3)
   {
      cerr << "'numtag' must be 2 or 3." << endl;
      return 1;
   }

   ///////////////
   // setup TFI //
   ///////////////
   const string kCandidateBranch = "candidate";
   setupTFI ("candFCNC", tag);
   ns_tfi.addClassBranch< CLPCandidate > (kCandidateBranch);

   ///////////////////////////
   // Initialize Histograms //
   ///////////////////////////
   int offsets [kMaxJet + 2] = {};
   offsets[1] = 0;
   for (int jetIndex = 2; jetIndex <= kMaxJet + 1; ++jetIndex)
   {
      offsets[jetIndex] = offsets[jetIndex - 1] + std::min (jetIndex, 3);
      if (gVerbose) cout << jetIndex << ") " << offsets[jetIndex] << endl;
   } // for jetindex
   // For display purposes, you can put in an empty bin at the
   // beginning by changing lower to -1;
   int lower = 0; // 0 (for fitting) or -1 (for display)
   int taggerOffset = offsets[gNumJets + 1];
   int upper = gNumTaggers * taggerOffset - 1;
   int numBins = upper - lower + 1;
   // Make histogram to store template
   if (gVerbose)  cout << "numBins " << numBins << " lower " << lower - 0.5 
                       << " upper " << upper + 0.5 << endl;
   TH1F *hPtr = new TH1F (gHistName.c_str(), 
                          Form ("%s Template", gHistName.c_str()), 
                          numBins, lower - 0.5, upper + 0.5);
   TAxis *aPtr = hPtr->GetXaxis();
   for (int jetIndex = 1; jetIndex <= gNumJets; ++jetIndex)
   {
      int offset = 1 + offsets[jetIndex] - lower;
      for (int tagIndex = 0; tagIndex <= std::min (jetIndex, 2); ++tagIndex)
      {
         // As long as we're using words for the labels, I thought I
         // should get the "plurals" right.
         if (1 == tagIndex)
         {
            for (int tagger = 1; tagger <= gNumTaggers; ++tagger)
            {
               aPtr->SetBinLabel (offset + tagIndex + 
                                  taggerOffset * (tagger - 1), "1 tag");
            } // for tagger
         }
         else if ((2 == tagIndex) && (jetIndex > 2))
         {
            for (int tagger = 1; tagger <= gNumTaggers; ++tagger)
            {
               aPtr->SetBinLabel (offset + tagIndex + 
                                  taggerOffset * (tagger - 1), "2+ tags");
            }
         }
         else 
         {
            for (int tagger = 1; tagger <= gNumTaggers; ++tagger)
            {
               aPtr->SetBinLabel (offset + tagIndex + 
                                  taggerOffset * (tagger - 1), 
                                  Form ("%d tags", tagIndex));
            }
         }
      } // for tagIndex;
   } // for jetIndex

   int entryIndex = 0;
   while ( ns_tfi.getNextEvent() )
   {
      CLPCandidate *candPtr 
         = ns_tfi.classPtr< CLPCandidate > (kCandidateBranch);
      // if (! ns_goodRunObject.isGoodRun (candPtr->runNumber(),
      //                                   candPtr->runSection()) )
      // {
      //    continue;
      // }
      if (++entryIndex % 1000 == 0) cout << entryIndex << endl;
      double weight = candPtr->weight();
      int numJets = candPtr->numJets();
      if (numJets > gNumJets)
      {
         // Are we vetoing events like this
         if (gJetVeto)
         {
            continue;
         } else {
            // Do only leading N
            numJets = gNumJets;
         }
      }
      if (gUseTagInfo)
      {
         // we are going to use the tags as is
         int numLoose = 0, numTight = 0;
         for (int jetIndex = 0; jetIndex < numJets; ++jetIndex)
         {
            const CLPJet &jet = candPtr->jet (jetIndex);
            if ( jet.isBTagged( CLPJet::kLoose ) )
            {
               ++numLoose;
            }
            if ( jet.isBTagged( CLPJet::kTight ) )
            {
               ++numTight;
            }
         } // for jetIndex
         hPtr->Fill( offsets[numJets] + numLoose );
         hPtr->Fill( offsets[numJets] + numTight + taggerOffset );
         continue;
      }
      DVec looseTagProb, tightTagProb, ultraTagProb;
      for (int jetIndex = 0; jetIndex < numJets; ++jetIndex)
      {
         const CLPJet &jet = candPtr->jet (jetIndex);
         looseTagProb.push_back( jet.tagProb( CLPJet::kLoose ) );
         tightTagProb.push_back( jet.tagProb( CLPJet::kTight ) );
         if (gNumTaggers < 3)
         {
            continue;
         }
         ultraTagProb.push_back( jet.tagProb( CLPJet::kUltra ) );
      } // for jetIndex
      DVec looseDist, tightDist;
      CombEngine::calcTagDistribution (looseTagProb, looseDist);
      CombEngine::calcTagDistribution (tightTagProb, tightDist);
      fillHistWithTagDist (hPtr, looseDist, offsets[numJets],
                           weight);
      fillHistWithTagDist (hPtr, tightDist, offsets[numJets] + taggerOffset,
                           weight);
      if (gNumTaggers >= 3)
      {
         fillHistWithTagDist (hPtr, tightDist, 
                              offsets[numJets] + 2 * taggerOffset,
                              weight);
      }
   } // while getNextEvent

   TFile *outputFilePtr = TFile::Open (ns_outputName.c_str(), "RECREATE");
   if (! outputFilePtr)
   {
      cerr << "Can't open " << ns_outputName << " for writing." << endl;
      exit (1);
   }
   hPtr->Write();
   outputFilePtr->Close();
   return 0;
}

void fillHistWithTagDist (TH1F *histPtr, const DVec &tagDistVec,
                          int offset, double eventWeight)
{
   double sum2OrMore = 0;
   int jetBin = 0;
   for (DVec::const_iterator iter = tagDistVec.begin();
        tagDistVec.end() != iter;
        ++iter)
   {
      if (gVerbose) cout << "Filling " << *iter 
                         << " at offset " << offset << endl;
      if (jetBin++ < 2)
      {
         // we are filling either jet bin 0 or 1
         histPtr->Fill (offset++, *iter * eventWeight);
      } else {
         sum2OrMore += *iter * eventWeight;
      }
   } // for iter
   // do we need to make a final entry?
   if (sum2OrMore)
   {
      histPtr->Fill (offset, sum2OrMore);
   }
}

void coutDVec (const DVec &vec)
{
   for (CombEngine::DVecConstIter iter = vec.begin();
        vec.end() != iter;
        ++iter)
   {
      cout << *iter << " ";
   }
}

void localInitialize()
{
   addOptionKey ("verbose",  gVerbose);
   addOptionKey ("usetag",   gUseTagInfo);
   addOptionKey ("verbose",  gVerbose);
   addOptionKey ("numjets",  gNumJets);
   addOptionKey ("histname", gHistName);
   addOptionKey ("jetveto",  gJetVeto);
   addOptionKey ("numtag",   gNumTaggers);
}

