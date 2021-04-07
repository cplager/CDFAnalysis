// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
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

using namespace std;
using namespace CLPUtilNamespace;

///////////////////////////////////////////////////////////
// Constants, Global Variables, and Forward Declarations //
///////////////////////////////////////////////////////////

enum {
   kDT,
   kME,
   kNN,
};

const double kSMxsec = 2.9;

// Generates three sets of correlated Gaussian random numbers
void generateThreeSets (int trials, const TMatrixD& sqrtCovMat,
                        DVec &vecDT, DVec &vecME, DVec &vecNN);

// Outputs the mean, RMS, and correlations of the PEs
void outputMeanEtc (const DVec &vecDT, const DVec &vecME, const DVec &vecNN);

int main(int argc, char** argv)
{
   bool increaseLumi = false;
   // TRandom is lousy.  Use TRandom3 instead.
   delete gRandom;
   gRandom = (TRandom*) new TRandom3;

   // Numbers from D0's Wine and Cheese talk (pgs 52 and 53)
   // http://www-d0.fnal.gov/Run2Physics/WWW/results/prelim/TOP/T39/wine_and_cheese.pdf
   double xsecDT  = 4.9; // Decision Tree
   double xsecME  = 4.6; // Matrix Element
   double xsecNN  = 5.0; // Neural Net
   double sigmaDT = 1.4; 
   double sigmaME = 1.5; 
   double sigmaNN = 1.9; 
   double rhoDT_ME  = 0.39; // DT vs ME
   double rhoDT_NN  = 0.57; // DT vs NN
   double rhoME_NN  = 0.29; // ME vs NN

   // See what happens if we increase D0's luminosity by 12%
   if (increaseLumi)
   {
      // Increase luminosity by 12%
      cout << "Increasing Luminosity by 12%" << endl;
      xsecDT  /= 1.12;
      xsecME  /= 1.12;
      xsecNN  /= 1.12;
      sigmaDT /= 1.12;
      sigmaME /= 1.12;
      sigmaNN /= 1.12;
   }

   // Output the current settings.
   cout << "Settings: " << endl
        << "  DT: " << Form ("%6.3f +- %.3f", xsecDT, sigmaDT) << endl
        << "  ME: " << Form ("%6.3f +- %.3f", xsecME, sigmaME) << endl
        << "  NN: " << Form ("%6.3f +- %.3f", xsecNN, sigmaNN) << endl
        << "  DT versus ME correlation: " << Form ("%.3f", rhoDT_ME) << endl
        << "  DT versus NN correlation: " << Form ("%.3f", rhoDT_NN) << endl
        << "  ME versus NN correlation: " << Form ("%.3f", rhoME_NN) << endl
        << endl;

   //////////////////
   // Generate PEs //
   //////////////////   
   TMatrixD covMat (3, 3);
   covMat (kDT, kDT) = sigmaDT * sigmaDT;
   covMat (kME, kME) = sigmaME * sigmaME;
   covMat (kNN, kNN) = sigmaNN * sigmaNN;
   covMat (kDT, kME) = covMat (kME, kDT) =  rhoDT_ME * sigmaDT * sigmaME;
   covMat (kDT, kNN) = covMat (kNN, kDT) =  rhoDT_NN * sigmaDT * sigmaNN;
   covMat (kME, kNN) = covMat (kNN, kME) =  rhoME_NN * sigmaME * sigmaNN;
   TMatrixD sqrtCovMat (3, 3);
   sqrtMatrix (sqrtCovMat, covMat);
   int trials = 100000;
   DVec vecDT, vecME, vecNN;
   generateThreeSets (trials, sqrtCovMat, vecDT, vecME, vecNN);
   // output mean, RMS, and correlation of generated PEs
   outputMeanEtc (vecDT, vecME, vecNN);

   // Setup our comparison.  We are checking to see how often a PE
   // will have all three analyses fluctuate as high (or higher) as
   // they did in the data.
   DVec compareVec;
   compareVec.push_back ((xsecDT - kSMxsec) / sigmaDT); 
   compareVec.push_back ((xsecME - kSMxsec) / sigmaME);
   compareVec.push_back ((xsecNN - kSMxsec) / sigmaNN);
   sort (compareVec.begin(), compareVec.end());
   int worse = 0;
   // Make sure we get trials again in case we load the vectors from
   // some strange place (e.g., a file)
   trials = (int) vecDT.size();
   for (int loop = 0; loop < trials; ++loop)
   {
      DVec sigmaVec;
      sigmaVec.push_back ( (vecDT[loop] - kSMxsec) / sigmaDT);
      sigmaVec.push_back ( (vecME[loop] - kSMxsec) / sigmaME);
      sigmaVec.push_back ( (vecNN[loop] - kSMxsec) / sigmaNN);
      sort (sigmaVec.begin(), sigmaVec.end());
      // Are all of the values big enough?
      bool allBigEnough = true;
      for (int inner = 0; inner < 3; ++inner)
      {
         if (compareVec[inner] > sigmaVec[inner])
         {
            allBigEnough = false;
            continue;
         }
      }
      if (allBigEnough)
      {
         ++worse;
      }
   } // for loop
   cout << Form ("Percent Consistent with SM: %.3f%%",  
                 100. * (double) worse / (double) trials)
        << endl;
   return 0;
}

void generateThreeSets (int trials, const TMatrixD& sqrtCovMat,
                        DVec &vecDT, DVec &vecME, DVec &vecNN)
{
   const int kThree = 3;
   vecDT.clear();
   vecME.clear();
   vecNN.clear();
   TVectorD meanVec(kThree); 
   for (int loop = 0; loop < kThree; ++loop)
   {
      meanVec (loop) = kSMxsec; // centered around SM xsec
   } // for loop
   for (int loop = 0; loop < trials; ++loop)
   {
      TVectorD retVec (kThree);
      gaussCorrRand (retVec, sqrtCovMat, meanVec);
      vecDT.push_back( retVec (kDT) );
      vecME.push_back( retVec (kME) );
      vecNN.push_back( retVec (kNN) );
   } // for loop 
}

void outputMeanEtc (const DVec &vecDT, const DVec &vecME, const DVec &vecNN)
{
   double meanDT, rmsDT, meanME, rmsME, meanNN, rmsNN;
   calcMeanRMS (meanDT, rmsDT, vecDT);
   calcMeanRMS (meanME, rmsME, vecME);
   calcMeanRMS (meanNN, rmsNN, vecNN);
   cout << "PE Mean +- RMS:" << endl
        << "  DT: " << Form ("%.3f", meanDT) 
        << " +- "   << Form ("%.3f", rmsDT)  << endl
        << "  ME: " << Form ("%.3f", meanME) 
        << " +- "   << Form ("%.3f", rmsME)  << endl
        << "  NN: " << Form ("%.3f", meanNN) 
        << " +- "   << Form ("%.3f", rmsNN)  << endl << endl;
   cout << "PE Correlations:" << endl
        << "  DT vs. ME: " 
        << Form ("%.3f", correlationCoeff (vecDT, vecME) ) << endl
        << "  DT vs. NN: " 
        << Form ("%.3f", correlationCoeff (vecDT, vecNN) ) << endl
        << "  ME vs. NN: " 
        << Form ("%.3f", correlationCoeff (vecME, vecNN) ) << endl << endl;
}
