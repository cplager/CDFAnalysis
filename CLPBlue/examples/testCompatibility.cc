// -*- C++ -*-

// Standard include files
#include <math.h>
#include <iostream>
#include <assert.h>
#include <stdio.h>

// CLP include files
#include "CLPBlue/CLPBlue.hh"
#include "CLPBlue/CLPUtils.hh"
#include "CLPBlue/dout.hh"

// Root include files
#include "TSystem.h"
#include "TRandom3.h"

using namespace std;
using namespace CLPUtilNamespace;

// forward declarations
double calculateProbability (double rho);


int main(int argc, char** argv)
{
   // TRandom is lousy.  Use TRandom3 instead.
   delete gRandom;
   gRandom = (TRandom*) new TRandom3;
   cout << "#rho   Prob(%)" << endl;
   for (double rho = 0; rho < 0.99; rho += 0.05)
   {
      cout << Form ("%0.2f    ", rho)
           << Form ("%5.2f", calculateProbability (rho)) << endl;
   } // for rho
   double rho = 0.662;
   cout << Form ("%0.2f    ", rho)
        << Form ("%5.2f", calculateProbability (rho)) << endl;
}

double calculateProbability (double rho)
{
   const double meanNN   = 0.00;  // Likelyhood Fraction
   const double sigmaNN  = 0.41;
   const double meanME   = 0.98;  // Matrix Element
   const double sigmaME  = 0.488;
   const double rhoNN_ME = rho; // NN vs ME
   ////////////////
   // Setup BLUE //
   ////////////////
   enum {
      kNN,
      kME,
      kLF,
   };
   const int kNumMeas = 2; // how many measurements
   const int kNumErr  = 1; // how many error categories
   const int kErr     = 0; // which error 
   CLPBlue blue1 (kNumMeas, kNumErr);
   // the following lines are true for all PEs
   blue1.setErrorFraction (kNN, kErr, sigmaNN);
   blue1.setErrorFraction (kME, kErr, sigmaME);
   blue1.setCorrelation   (kNN, kME, kErr, rhoNN_ME);
   // the following few lines are only true for the data
   blue1.setMean (kNN, meanNN);
   blue1.setMean (kME, meanME);
   blue1.calcBlueAverage();
   double chi2_threshold = blue1.chi2();
   //cout << "threshold " << chi2_threshold << endl;

   ///////////////////////////////////////////////
   // Setup Correlated Random Number Generation //
   ///////////////////////////////////////////////
   TMatrixD covMat (kNumMeas, kNumMeas);
   // diagonal terms
   covMat (kNN, kNN) = sigmaNN * sigmaNN;
   covMat (kME, kME) = sigmaME * sigmaME;
   // off-diagonal terms
   covMat (kNN, kME) = covMat (kME, kNN) = rhoNN_ME * sigmaNN * sigmaME;
   TMatrixD sqrtCovMat (kNumMeas, kNumMeas);
   sqrtMatrix (sqrtCovMat, covMat);
   // what the random numbers should be centered around
   TVectorD meanVec(kNumMeas); 
   for (int loop = 0; loop < kNumMeas; ++loop)
   {
      meanVec (loop) = 1; // in Beta units, we want them centered at 1
   } // for loop

   ///////////////////
   // Throw the PEs //
   ///////////////////
   int trials = 100000;
   int worse  = 0;
   for (int outer = 0; outer < trials; ++outer)
   {
      TVectorD retVec(2);
      gaussCorrRand (retVec, sqrtCovMat, meanVec);
      for (int inner = 0; inner < kNumMeas; ++inner)
      {
         blue1.setMean( inner, retVec (inner) );
      } // for inner
      blue1.calcBlueAverage();
      double chi2 = blue1.chi2();
      if (chi2 > chi2_threshold)
      {
         ++worse;
      } 
   } // for outer
   return  100. * ( (double) worse / trials);
}
