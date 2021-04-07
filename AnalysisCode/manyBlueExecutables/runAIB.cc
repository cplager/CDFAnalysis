// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>

#include "TClonesArray.h"
#include "TTree.h"
#include "TSystem.h"
#include "TRandom3.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TMath.h"

#include "RootFileCollection/TreeFileInterface.h"
#include "RootFileCollection/TFINamespace.h"
#include "RootFileCollection/OptionUtils.h"
#include "RootFileCollection/dout.h"
#include "CLPBlue/CLPAsymBlue.hh"


using namespace std;
using namespace OptionUtils;
using namespace TFINamespace;

// Order of analyses in AIB
enum
{
   kLF,
   kME,
   kNN,
   kNumMeas,
};

#include "UnbiasArrays.icc"

// Names of branches
const string kTrueBeta = "trueBeta";
const string kMeasLF   = "measLF";  
const string kPosLFerr = "posLFErr";
const string kNegLFerr = "negLFErr";
const string kMeasME   = "measME";  
const string kPosMEerr = "posMEErr";
const string kNegMEerr = "negMEErr";
const string kMeasNN   = "measNN";  
const string kPosNNerr = "posNNErr";
const string kNegNNerr = "negNNErr";

// Polluting the gLobal namespace - gLobal variables
// Note, name in parenthesis is command-line variable name.

// Don't iterate upper and lower BLUE, but just use central average
// (usecentral)
bool   gUseCentralBlue = true;
// only use statistical uncertainties (onlystat)
bool   gOnlyStat = false;
// unbias the systematic result (unbias)
int    gUnbias   = 0;
// which unbias method (whichunbias)
int    gWhichUnbias = -1;
// truncate shape Gaussians (trunk)
bool   gTruncateGaussians = false;
// random seed (seed)
int    gSeed = 1;
// unbias values for LF, ME, and NN (unbiasXX)
double gUnbiasLF = 0.;
double gUnbiasME = 0.;
double gUnbiasNN = 0.;
// mean for error calculation cutoff
double gMinMeanForErrors = -20;
// show data only
bool gDataOnly = false;
// aib file
string gAibFile = "stc.aib";

// forward declarations
double pullValue (double measured, double trueValue,
                  double uppErr,   double lowErr);
void copyUnbiasArray (double target[kNumRows][kNumMeas],
                      const double source[kNumRows][kNumMeas]);
void localInitialize();

int main(int argc, char** argv)
{
   // Parse Command Line Arguments          
   // Decide Which Files and How Many Events
   setupDefaultOptions(); // hook up standard options
   localInitialize();
   parseArguments (argc, argv); // OptionUtils::

   // Root initializations
   gROOT->SetStyle("Plain");
   delete gRandom; gRandom = new TRandom3;
   gRandom->SetSeed (gSeed);

   string tag = ""; // any additional tags you want added on an output
                    // filename
   
   bool unbias = false;
   double array[kNumRows][kNumMeas];
   if (gUnbias)
   {
      if (gUnbias >= kFirstRow && gUnbias < kFirstRow + kNumRows)
      {
         unbias = true;
         gOnlyStat = false;
         gUnbias -= kFirstRow;
      }
      bool dontOverride = false;
      switch (gWhichUnbias)
      {
         case -1:
            cout << "Using command line values" << endl;
            tag += "_command";
            dontOverride = true;
            unbias = true;
            gOnlyStat = false;
            break;
         case 0:
            copyUnbiasArray (array, kMeanUnbias);
            cout << "Using Mean Unbias" << endl;
            tag += "_meanUnbias";
            break;
         case 1:
            copyUnbiasArray (array, kFitMeanUnbias);
            cout << "Using Fit Mean Unbias" << endl;
            tag += "_fitmeanUnbias";
            break;
         case 2:
            copyUnbiasArray (array, kPullLowerUnbias);
            cout << "Using Pull Unbias" << endl;
            tag += "_pullUnbias";
            break;
         case 3:
            copyUnbiasArray (array, kFitPullLowerUnbias);
            cout << "Using Fit Pull Unbias" << endl;
            tag += "_fitpullUnbias";
            break;
         case 4:
            copyUnbiasArray (array, kPullAveUnbias);
            cout << "Using Average Pull Unbias" << endl;
            tag += "_avepullUnbias";
            break;
         case 5:
            copyUnbiasArray (array, kFitPullAveUnbias);
            cout << "Using Average Fit Pull Unbias" << endl;
            tag += "_avefitpullUnbias";
            break;
         case 6:
            copyUnbiasArray (array, kPullUpperUnbias);
            cout << "Using Upper Pull Unbias" << endl;
            tag += "_upperpullUnbias";
            break;
         case 7:
            copyUnbiasArray (array, kFitPullUpperUnbias);
            cout << "Using Upper Fit Pull Unbias" << endl;
            tag += "_upperfitpullUnbias";
            break;
      }
      assert (array);
      if (! dontOverride)
      {
         gUnbiasLF = array[gUnbias][kLF];
         gUnbiasME = array[gUnbias][kME];
         gUnbiasNN = array[gUnbias][kNN];
      }
      cout << "unbiasing with " 
           << gUnbiasLF << " "
           << gUnbiasME << " "
           << gUnbiasNN << endl;
   } // if gUnbias
   if (gOnlyStat)          tag += "_nosyst";
   if (gTruncateGaussians) tag += "_trunk";
   tag += Form ("_s%03d", gSeed);

   CLPAsymBlue aib;
   aib.setIgnoreLoadWarnings (true);
   aib.setUpperLowerErrorsOnly (gUseCentralBlue);
   aib.setMinimumMeanForErrors (gMinMeanForErrors);
   aib.readFromFile (gAibFile);
   double valLF = aib.indMean (kLF);
   double valME = aib.indMean (kME);
   double valNN = aib.indMean (kNN);
   if (gOnlyStat)
   {
      aib.setNumErrors (1);
   }

   double mean, upper, lower;
   aib.calcIterativeBlueAverage (mean, upper, lower);
   double chi2 = aib.chi2();
   if (gDataOnly)
   {
      cout << aib << endl
           << "chi2 " << chi2 << endl;
      return 0;
   }

   // tell it which systematic uncertainties are only rate
   set< int > rateOnlySet;
   rateOnlySet.insert (5);


   // setup the TFI to use the top tree and TopNtupleDecoder
   setupTFI ("STCtree", tag);

   ////////////////////////
   // Setup TCA Branches //
   ////////////////////////

   // Tell 'ns_tfi' which branches we want access to.
   ns_tfi.addBranch< float > (kTrueBeta);
   ns_tfi.addBranch< float > (kMeasLF  );
   ns_tfi.addBranch< float > (kMeasME  );
   ns_tfi.addBranch< float > (kMeasNN  );
   ns_tfi.addBranch< float > (kPosLFerr);
   ns_tfi.addBranch< float > (kPosMEerr);
   ns_tfi.addBranch< float > (kPosNNerr);
   ns_tfi.addBranch< float > (kNegLFerr);
   ns_tfi.addBranch< float > (kNegMEerr);
   ns_tfi.addBranch< float > (kNegNNerr);

   /////////////////////////////////////
   // Open File and Define Histograms //
   /////////////////////////////////////   
   TFile *filePtr = new TFile (ns_outputName.c_str(), "RECREATE");
   if (! filePtr->IsOpen()) {
      cerr << "Did not successfully open " << ns_outputName 
           << " for output.  Aborting." << endl;
      return 0;      
   } else {
      cout << "Opened " << ns_outputName << " for output." << endl;
   }
   TH1F *pullPtr    = new TH1F ("pull",    "pull",     50, -5.0, 5.0);
   TH1F *pullSymPtr = new TH1F ("pullSym", "pullSym",  50, -5.0, 5.0);
   TH1F *pullLFptr  = new TH1F ("pullLF",  "pullLF",   50, -5.0, 5.0);
   TH1F *pullMEptr  = new TH1F ("pullME",  "pullME",   50, -5.0, 5.0);
   TH1F *pullNNptr  = new TH1F ("pullNN",  "pullNN",   50, -5.0, 5.0);
   TH1F *meanPtr    = new TH1F ("mean",    "mean",     50, -1.0, 2.0);
   TH1F *meanLFptr  = new TH1F ("meanLF",  "meanLF",   50, -1.0, 2.0);
   TH1F *meanMEptr  = new TH1F ("meanME",  "meanME",   50, -1.0, 2.0);
   TH1F *meanNNptr  = new TH1F ("meanNN",  "meanNN",   50, -1.0, 2.0);
   TH1F *smearLFptr = new TH1F ("smearLF", "smearLF", 100, -1.0, 1.0);
   TH1F *smearMEptr = new TH1F ("smearME", "smearME", 100, -1.0, 1.0);
   TH1F *smearNNptr = new TH1F ("smearNN", "smearNN", 100, -1.0, 1.0);

   TH1F *shiftsHPtrs[CLPBlue::kMaxErrors + 1] = {};
   TH1F *smearsHPtrs[CLPBlue::kMaxErrors + 1] = {};

   CLPBlue::UInt numErr = aib.numErrors();
   for (CLPBlue::UInt errIndex = 1; errIndex < numErr; ++errIndex)
   {
      shiftsHPtrs[errIndex] = new TH1F (Form ("shift%02d", errIndex),
                                        Form ("shift%02d", errIndex),
                                        50, -5., 5.);
      smearsHPtrs[errIndex] = new TH1F (Form ("smear%02d", errIndex),
                                        Form ("smear%02d", errIndex),
                                        50, -1., 1.);
   } // for errIndex

   int numEvents     = 0;
   int numChi2       = 0;
   int numLesser     = 0;
   int numGreater    = 0;
   int numLFgreater  = 0;
   int numMEgreater  = 0;
   int numNNgreater  = 0;
   int numGreater1   = 0; 
   int numLFgreater1 = 0;
   int numMEgreater1 = 0;
   int numNNgreater1 = 0;

   while (ns_tfi.getNextEvent())
   {
      // Get the measured means
      double peLF = ns_tfi.value< float > (kMeasLF);
      double peME = ns_tfi.value< float > (kMeasME);
      double peNN = ns_tfi.value< float > (kMeasNN);
      // Throw systematic uncertainties
      double blurLF = 0., blurME = 0., blurNN = 0.;
      for (CLPBlue::UInt errIndex = 1; errIndex < numErr; ++errIndex)
      {
         double shift = gRandom->Gaus (0., 1.); // unit Gaussian random
         while (gTruncateGaussians &&
                rateOnlySet.end() == rateOnlySet.find (errIndex) &&
                fabs (shift) > 1)
         {
            shift = gRandom->Gaus (0., 1.);
         }
         double LFerr, MEerr, NNerr;
         if (shift >= 0)
         {
            // get upper error
            LFerr = aib.upperError (kLF, errIndex, peLF);
            MEerr = aib.upperError (kME, errIndex, peME);
            NNerr = aib.upperError (kNN, errIndex, peNN);
         } else {
            // get lower error
            LFerr = aib.lowerError (kLF, errIndex, peLF);
            MEerr = aib.lowerError (kME, errIndex, peME);
            NNerr = aib.lowerError (kNN, errIndex, peNN);
         }
         blurLF += LFerr * shift;
         blurME += MEerr * shift;
         blurNN += NNerr * shift;
         shiftsHPtrs[errIndex]->Fill (shift);
         smearsHPtrs[errIndex]->Fill ( shift * (LFerr + MEerr + NNerr) );
      } // for errIndex      
      double smearedLF = peLF + blurLF;
      double smearedME = peME + blurME;
      double smearedNN = peNN + blurNN;
      smearLFptr->Fill (blurLF);
      smearMEptr->Fill (blurME);
      smearNNptr->Fill (blurNN);
      if (unbias)
      {
         assert (array);
         smearedLF -= gUnbiasLF;
         smearedME -= gUnbiasME;
         smearedNN -= gUnbiasNN;
      }
      aib.setMean (kLF, smearedLF);
      aib.setMean (kME, smearedME);
      aib.setMean (kNN, smearedNN);
      double peMean, peUpper, peLower;
      aib.calcIterativeBlueAverage (peMean, peUpper, peLower);

      // check consistencies
      if (aib.chi2() < chi2) ++numChi2;
      if (peMean < mean)     ++numLesser;
      if (peMean > mean)     ++numGreater;
      if (smearedLF > valLF) ++numLFgreater;
      if (smearedME > valME) ++numMEgreater;
      if (smearedNN > valNN) ++numNNgreater;
      if (peMean > 1)        ++numGreater1;
      if (smearedLF > 1)     ++numLFgreater1;
      if (smearedME > 1)     ++numMEgreater1;
      if (smearedNN > 1)     ++numNNgreater1;
      ++numEvents;

      if (0 == numEvents % 10000)
      {
         cout << "."; cout.flush();
      }
      if (0 == numEvents % 500000)
      {
         cout << " " << numEvents / 1.e6 <<  endl;
      }

      double trueBeta = ns_tfi.value< float > (kTrueBeta);
      pullPtr->Fill( pullValue (peMean, trueBeta, peUpper, peLower) );
      double aveErr = (peUpper + peLower) / 2.;
      pullSymPtr->Fill( pullValue (peMean, trueBeta, aveErr, aveErr) );
      pullLFptr->Fill( pullValue ( smearedLF, trueBeta, 
                                   ns_tfi.value< float > (kPosLFerr), 
                                   ns_tfi.value< float > (kNegLFerr) ) ); 
      pullMEptr->Fill( pullValue ( smearedME, trueBeta, 
                                   ns_tfi.value< float > (kPosMEerr), 
                                   ns_tfi.value< float > (kNegMEerr) ) ); 
      pullNNptr->Fill( pullValue ( smearedNN, trueBeta, 
                                   ns_tfi.value< float > (kPosNNerr), 
                                   ns_tfi.value< float > (kNegNNerr) ) ); 
      meanPtr->Fill (peMean);
      meanLFptr->Fill (smearedLF);
      meanMEptr->Fill (smearedME);
      meanNNptr->Fill (smearedNN);
   } // while get next event
   cout << endl;
   filePtr->cd();
   filePtr->Write();
   delete filePtr;

   // output statistics
   cout << "num:     " << setw (10) << numEvents << endl
        << "chi2:    " << setw (10) << numChi2 
        << Form (" - %6.2f%%", 100. * numChi2 / numEvents)
        << endl
        << "lesser:  " << setw (10) << numLesser
        << Form (" - %6.2f%%", 100. * numLesser / numEvents);
   if ( (double) numLesser / numEvents < 0.20)
   {

      cout << Form (" - %6.3f sigma",  sqrt(2.) * 
                    TMath::ErfcInverse(2. * numLesser / numEvents));
           // << endl
           // << Form ("%29s - %6.3f sigma two sided", " ", sqrt (2.) *
           //          TMath::ErfcInverse(4. * numLesser / numEvents));      
   }
   cout << endl
        << "Data:" << endl
        << "greater: " << setw (10) << numGreater
        << Form (" - %6.2f%%", 100. * numGreater / numEvents)
        << Form (" - %6.3f sigma",  sqrt(2.) * 
                 TMath::ErfcInverse(2. * numGreater / numEvents)) 
        << endl
        << "  LF:    " << setw (10) << numLFgreater
        << Form (" - %6.2f%%", 100. * numLFgreater / numEvents)
        << Form (" - %6.3f sigma",  sqrt(2.) * 
                 TMath::ErfcInverse(2. * numLFgreater / numEvents)) 
        << endl
        << "  ME:    " << setw (10) << numMEgreater
        << Form (" - %6.2f%%", 100. * numMEgreater / numEvents)
        << Form (" - %6.3f sigma",  sqrt(2.) * 
                 TMath::ErfcInverse(2. * numMEgreater / numEvents)) 
        << endl
        << "  NN:    " << setw (10) << numNNgreater
        << Form (" - %6.2f%%", 100. * numNNgreater / numEvents)
        << Form (" - %6.3f sigma",  sqrt(2.) * 
                 TMath::ErfcInverse(2. * numNNgreater / numEvents)) 
        << endl
        << "Expected:" << endl
        << "greater1:" << setw (10) << numGreater1
        << Form (" - %6.2f%%", 100. * numGreater1 / numEvents)
        << Form (" - %6.3f sigma",  sqrt(2.) * 
                 TMath::ErfcInverse(2. * numGreater1 / numEvents))
        << endl
        << "  LF1:   " << setw (10) << numLFgreater1
        << Form (" - %6.2f%%", 100. * numLFgreater1 / numEvents)
        << Form (" - %6.3f sigma",  sqrt(2.) * 
                 TMath::ErfcInverse(2. * numLFgreater1 / numEvents)) 
        << endl
        << "  ME1:   " << setw (10) << numMEgreater1
        << Form (" - %6.2f%%", 100. * numMEgreater1 / numEvents)
        << Form (" - %6.3f sigma",  sqrt(2.) * 
                 TMath::ErfcInverse(2. * numMEgreater1 / numEvents)) 
        << endl
        << "  NN1:   " << setw (10) << numNNgreater1
        << Form (" - %6.2f%%", 100. * numNNgreater1 / numEvents)
        << Form (" - %6.3f sigma",  sqrt(2.) * 
                 TMath::ErfcInverse(2. * numNNgreater1 / numEvents)) 
        << endl
        << endl;
}

double pullValue (double measured, double trueValue,
                  double uppErr, double lowErr)
{
   double error = uppErr;
   if (measured > trueValue)
   {
      error = fabs (lowErr); // just in case the stored lower
                             // uncertainty as negative
   }
   if (! error)
   {
      return -999;
   }
   return (measured - trueValue) / error;
}

void copyUnbiasArray (double target[kNumRows][kNumMeas],
                      const double source[kNumRows][kNumMeas])
{
   for (int outer = 0; outer < kNumRows; ++outer)
   {
      for (int inner = 0; inner < kNumMeas; ++inner)
      {
         target[outer][inner] = source[outer][inner];
      } // for inner
   } // for outer
}


void localInitialize()
{   
   // put options you want here
   addOptionKey ("onlystat",    gOnlyStat);
   addOptionKey ("usecentral",  gUseCentralBlue);
   addOptionKey ("unbias",      gUnbias);
   addOptionKey ("whichunbias", gWhichUnbias,
                 "0 - Mean, 1 - FitMean, 2 - Pull, 3 - FitPull");
   addOptionKey ("trunk",       gTruncateGaussians,
                 "Truncate shape Gaussians");
   addOptionKey ("seed",        gSeed);
   addOptionKey ("unbiasLF",    gUnbiasLF);
   addOptionKey ("unbiasME",    gUnbiasME);
   addOptionKey ("unbiasNN",    gUnbiasNN);
   addOptionKey ("minmean",     gMinMeanForErrors);
   addOptionKey ("dataonly",    gDataOnly);
   addOptionKey ("aibfile",     gAibFile);
}
