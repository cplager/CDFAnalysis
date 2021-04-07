// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstdlib>

// Root include files
#include "TSystem.h"
#include "TRandom3.h"

// CLP include files
#include "CLPBlue/CLPBlue.hh"
#include "CLPBlue/CLPUtils.hh"
#include "CLPBlue/dout.hh"
#include "RootFileCollection/OptionUtils.h"

using namespace std;
using namespace CLPUtilNamespace;
using namespace OptionUtils;

enum {
   kLF,
   kME,
   kNN,
};

const int kMaxAnalyses = 10;

// global control variables
// filename (filename)
string gFilename = "comp4d_6x6_allSys.txt";
// analysis names (name0..name9)
string gAnalysisNames  [kMaxAnalyses] = 
{"ana0", "ana1", "ana2", "ana3", "ana4", 
 "ana5", "ana6", "ana7", "ana8", "ana9"};
// analysis results (result0..result9)
double gAnalysisValues [kMaxAnalyses] = {};
// number of analysis (num)
int gNumAnalyses = 4;
// Rethrow PEs (rethrow)
bool gRethrow = false;
// Number of PEs to generate (pes)
int  gNumPEs = 100000;

// forward declarations
void initializeOptionMap();
void generateNSets (const TMatrixD& sqrtCovMat, DVecVec &allPEs);

int main (int argc, char** argv)
{
   initializeOptionMap();
   parseArguments (argc, argv);
   double errors [kMaxAnalyses] = {};
   double correlations [kMaxAnalyses][kMaxAnalyses] = {{}};
   assert (gNumAnalyses <= kMaxAnalyses);
   DVecVec allPEs;   

   //////////////////////////
   // Read in PE text file //
   //////////////////////////
   cout << "Number of Analyses: " << gNumAnalyses << endl;
   cout << "File: " << gFilename << endl;
   if ( ! readNColumnsFromFile (gFilename, gNumAnalyses, allPEs))
   {
      cerr << "Could not open file for input.  Aborting." << endl;
      exit (1);
   }
   ////////////////////////////////////
   // Calculate RMS and Correlations //
   ////////////////////////////////////
   for (int outer = 0; outer < gNumAnalyses; ++outer)
   {
      double mean, rms;
      calcMeanRMS (mean, rms, allPEs[outer]);
      errors[outer] = rms;
      for (int inner = outer; inner < gNumAnalyses; ++inner)
      {
         if (inner == outer)
         {
            correlations[outer][inner] = 1.;
            continue;
         } // if the same
         correlations[inner][outer] = correlations[outer][inner] =
            correlationCoeff( allPEs[inner], allPEs[outer] );
      } // for inner
   } // for outer

   //////////////////
   // Rethrow PEs? //
   //////////////////
   if (gRethrow)
   {
      cout << "Rethrowing PEs" << endl;
      TMatrixD covMat (gNumAnalyses, gNumAnalyses);
      for (int outer = 0; outer < gNumAnalyses; ++outer)
      {
         covMat (outer, outer) = errors[outer] * errors[outer];
         for (int inner = outer + 1; inner < gNumAnalyses; ++inner)
         {
            covMat (outer, inner) = covMat (inner, outer) =
               correlations[inner][outer] * errors[inner] * errors[outer];
         } // for inner
      } // for outer
      TMatrixD sqrtCovMat (gNumAnalyses, gNumAnalyses);
      sqrtMatrix (sqrtCovMat, covMat);
      generateNSets (sqrtCovMat, allPEs);
   }
   int numPEs = (int) allPEs[0].size();
   cout << "num PEs: " << numPEs << endl;


   ////////////////////////////////
   // Calculate All Combinations //
   ////////////////////////////////
   IVecVec allCombinations;
   for (int dim = gNumAnalyses; dim >= 2; --dim)
   {
      IVec comb = firstCombination (dim);
      do
      {
         allCombinations.push_back (comb);
      } while (nextCombination (comb, gNumAnalyses));
   }
   int numCombs = (int) allCombinations.size();

   ////////////////
   // Setup BLUE //
   ////////////////
   vector< CLPBlue> blueVec;
   DVec threshholdVec;
   for (int loop = 0; loop < numCombs; ++loop)
   {
      const IVec &comb = allCombinations[loop];
      int size = (int) comb.size();
      const int kNumErrors = 1; // only 1 error for BLUE
      const int kErr       = 0; // which error
      CLPBlue blue (size, kNumErrors);
      for (int outer = 0; outer < size; ++outer)
      {
         int whichOuter = comb[outer];
         blue.setMean (outer, gAnalysisValues[ whichOuter ]);
         blue.setErrorFraction (outer, kErr, errors[ whichOuter ]);
         for (int inner = outer + 1; inner < size; ++inner)
         {
            int whichInner = comb[inner];
            blue.setCorrelation (outer, inner, kErr, 
                                 correlations[ whichOuter ][ whichInner ]);
         } // for inner
      } // for outer
      // Blue is now setup.  Calcuate the average and keep track of
      // the threshold
      blue.calcBlueAverage();
      double threshhold = blue.chi2();
      blueVec.push_back      ( blue );
      threshholdVec.push_back( threshhold );
   } // for loop

   ///////////////
   // Check PEs //
   ///////////////
   IVec countsVec;
   countsVec.resize (numCombs, 0);
   // for now, assume I didn't screw up and all analyses have the same
   // number of PEs.
   for (int peIndex = 0; peIndex < numPEs; ++peIndex)
   {
      // get the PE values
      DVec peValues;
      for (int anIndex = 0; anIndex < gNumAnalyses; ++anIndex)
      {
         peValues.push_back( allPEs[anIndex][peIndex] );
      } // for anIndex
      for (int combIndex = 0; combIndex < numCombs; ++combIndex)
      {
         CLPBlue &blue = blueVec[combIndex];
         const IVec &comb = allCombinations[combIndex];
         int size = (int) comb.size();
         for (int anIndex = 0; anIndex < size; ++anIndex)
         {
            int whichIndex = comb[anIndex];
            blue.setMean (anIndex, peValues[ whichIndex ]);
         }
         blue.calcBlueAverage();
         double chi2 = blue.chi2();
         if (chi2 > threshholdVec[combIndex])
         {
            ++countsVec[combIndex];
         }
      } // for loop
   } // for peIndex

   ////////////////////
   // Output Results //
   ////////////////////
   int maxLength = 0;
   for (int anIndex = 0; anIndex < gNumAnalyses; ++anIndex)
   {
      if ((int)gAnalysisNames[anIndex].length() > maxLength)
      {
         maxLength = (int) gAnalysisNames[anIndex].length();
      }
   } // for anIndex
   cout << endl << "Values:" << endl;
   int max = std::max (maxLength, 6);
   for (int outer = 0; outer < gNumAnalyses; ++outer)
   {
      cout << setw(maxLength) << gAnalysisNames[outer] 
           << ": " 
           << Form ("%6.3f", gAnalysisValues[outer]) << " +- "
           << Form ("%6.3f", errors[outer]) << endl;
   } // for outer
   cout << endl << "Correlations:" << endl;
   cout << setw(max) << " ";
   for (int outer = 0; outer < gNumAnalyses; ++outer)
   {
      cout << setw(max + 1) << gAnalysisNames[outer] << " ";
   }
   cout << endl;
   for (int outer = 0; outer < gNumAnalyses; ++outer)
   {
      cout << setw(max) << gAnalysisNames[outer] << " ";
      for (int inner = 0; inner < gNumAnalyses; ++inner)
      {
         if (inner < outer)
         {
            cout << setw ( max )
                 << " ";
         } else {
            cout << setw (maxLength) 
                 << Form ("%6.3f", correlations[inner][outer]);
         }
         cout << "  ";
      } // for inner
      cout << endl;
   } // for outer
   
   int prevSize = 0;
   for (int combIndex = 0; combIndex < numCombs; ++combIndex)
   {
      const IVec &comb = allCombinations[combIndex];
      int size = (int) comb.size();
      if (size != prevSize)
      {
         prevSize = size;
         cout << endl << "Comparing " << size << " analyses at once:"
              << endl;
      }
      for (int loop = 0; loop < size; ++loop)
      {
         cout << " " << setw (maxLength) << gAnalysisNames[ comb[loop] ];
      } // for loop
      cout << ": " << setw (1 + (gNumAnalyses - size) * (maxLength + 1) ) 
           << " "
           << Form ("%6.2f%% (%d)", 
                            100. * countsVec[combIndex] / numPEs,
                            countsVec[combIndex])
           << endl;
   } // for combIndex
}

void initializeOptionMap()
{
   // simple options
   addOptionKey ("file",    gFilename);
   addOptionKey ("num",     gNumAnalyses);
   addOptionKey ("rethrow", gRethrow);
   addOptionKey ("pes",     gNumPEs);
   // 0..9 options
   for (int loop = 0; loop < kMaxAnalyses; ++loop)
   {
      addOptionKey (Form ("name%d", loop),   gAnalysisNames[loop]);
      addOptionKey (Form ("result%d", loop), gAnalysisValues[loop]);
   } // for loop
}

void generateNSets (const TMatrixD& sqrtCovMat, DVecVec &allPEs)
{
   int numColumns = sqrtCovMat.GetNcols();
   allPEs.clear();
   allPEs.resize (numColumns);
   TVectorD meanVec (numColumns); 
   for (int loop = 0; loop < numColumns; ++loop)
   {
      meanVec (loop) = 1; // in Beta units, we want them centered at 1
   } // for loop
   for (int loop = 0; loop < gNumPEs; ++loop)
   {
      TVectorD retVec (numColumns);
      gaussCorrRand (retVec, sqrtCovMat, meanVec);
      for (int loop = 0; loop < numColumns; ++loop)
      {
         allPEs[loop].push_back( retVec (loop) );
      } // for loop
   } // for loop 
}
