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

enum {
   kLF,
   kME,
   kNN,
};

// forward declarations
double calculateProbability (double rho);
void generateThreeSets (int trials, const TMatrixD& sqrtCovMat,
                        DVec &vecLF, DVec &vecME, DVec &vecNN);
void readThreeSetsFromFile (string filename,
                            DVec &vecLF, DVec &vecME, DVec &vecNN);
void outputMeanEtc (const DVec &vecLF, const DVec &vecME, const DVec &vecNN);

int main(int argc, char** argv)
{
   const bool kGenRandomNumbers = true;
   const bool kUsePEnumbers = false;

   // TRandom is lousy.  Use TRandom3 instead.
   delete gRandom;
   gRandom = (TRandom*) new TRandom3;
   
   double meanLF   =  0.10;  // Likelyhood Fraction
   double meanME   =  0.98;  // Matrix Element
   double meanNN   = -0.25;  // Neural Net
   double sigmaLF  =  0.526;
   double sigmaME  =  0.488;
   double sigmaNN  =  0.44;
   double rhoLF_ME =  0.532; // LF vs ME
   double rhoLF_NN =  0.667; // LF vs NN
   double rhoME_NN =  0.662; // ME vs NN
   
   if (kUsePEnumbers)
   {
      sigmaLF  =  0.469;
      sigmaME  =  0.414;
      sigmaNN  =  0.418;
      rhoLF_ME =  0.609; // LF vs ME
      rhoLF_NN =  0.667; // LF vs NN
      rhoME_NN =  0.662; // ME vs NN
   }

   if (kGenRandomNumbers)
   {
      cout << "Generating PEs from correlation coeffiencts" << endl;
   } else {
      cout << "Reading PEs from file" << endl;
   }

   cout << "BLUE settings: " << endl
        << "  LF: " << Form ("%6.3f +- %.3f", meanLF, sigmaLF) << endl
        << "  ME: " << Form ("%6.3f +- %.3f", meanME, sigmaME) << endl
        << "  NN: " << Form ("%6.3f +- %.3f", meanNN, sigmaNN) << endl
        << "  LF versus ME correlation: " << Form ("%.3f", rhoLF_ME) << endl
        << "  LF versus NN correlation: " << Form ("%.3f", rhoLF_NN) << endl
        << "  ME versus NN correlation: " << Form ("%.3f", rhoME_NN) << endl
        << endl;

   ////////////////
   // Setup BLUE //
   ////////////////
   const int kThree   = 3; // how many measurements (for all three)
   const int kTwo     = 2; // when doing only two measurements
   const int kNumErr  = 1; // how many error categories
   const int kErr     = 0; // which error 

   // This may seem like overkill, but I want to setup enums for EACH
   // blue calculation (I'm sure that if I don't, I'll mess it up).
   enum {
      kLF_LFME,
      kME_LFME,
   };
   enum {
      kLF_LFNN,
      kNN_LFNN,
   };
   enum {
      kME_MENN,
      kNN_MENN,
   };
   // Create my four BLUE objects
   CLPBlue blueAll  (kThree, kNumErr);
   CLPBlue blueLFME (kTwo,   kNumErr);
   CLPBlue blueLFNN (kTwo,   kNumErr);
   CLPBlue blueMENN (kTwo,   kNumErr);
   // Set the errors for all blue calculations (for now, we are using
   // the same errors for all PEs
   // blueAll
   blueAll.setErrorFraction  (kLF, kErr, sigmaLF);
   blueAll.setErrorFraction  (kME, kErr, sigmaME);
   blueAll.setErrorFraction  (kNN, kErr, sigmaNN);
   // blueLFME
   blueLFME.setErrorFraction (kLF_LFME, kErr, sigmaLF);
   blueLFME.setErrorFraction (kME_LFME, kErr, sigmaME);
   // blueLFNN
   blueLFNN.setErrorFraction (kLF_LFNN, kErr, sigmaLF);
   blueLFNN.setErrorFraction (kNN_LFNN, kErr, sigmaNN);
   // blueMENN
   blueMENN.setErrorFraction (kME_MENN, kErr, sigmaME);
   blueMENN.setErrorFraction (kNN_MENN, kErr, sigmaNN);
   // Set the correlations for all blue calculations.  These will
   // always be the same for each PE.
   // blueAll
   blueAll.setCorrelation  (kLF, kME, kErr, rhoLF_ME);
   blueAll.setCorrelation  (kLF, kNN, kErr, rhoLF_NN);
   blueAll.setCorrelation  (kME, kNN, kErr, rhoME_NN);
   // blueLFME
   blueLFME.setCorrelation (kLF_LFME, kME_LFME, kErr, rhoLF_ME);
   // blueLFNN
   blueLFNN.setCorrelation (kLF_LFNN, kNN_LFNN, kErr, rhoLF_NN);
   // blueMENN
   blueMENN.setCorrelation (kME_MENN, kNN_MENN, kErr, rhoME_NN);
   
   // the following few lines are only true for the data
   // blueAll
   blueAll.setMean (kLF, meanLF);
   blueAll.setMean (kME, meanME);
   blueAll.setMean (kNN, meanNN);
   // blueLFME
   blueLFME.setMean (kLF_LFME, meanLF);
   blueLFME.setMean (kME_LFME, meanME);
   // blueLFNN
   blueLFNN.setMean (kLF_LFNN, meanLF);
   blueLFNN.setMean (kNN_LFNN, meanNN);
   // blueMENN
   blueMENN.setMean (kME_MENN, meanME);
   blueMENN.setMean (kNN_MENN, meanNN);
   // Calculate the average (and chi^2)
   blueAll.calcBlueAverage();
   blueLFME.calcBlueAverage();
   blueLFNN.calcBlueAverage();
   blueMENN.calcBlueAverage();
   double chi2_threshold_all  = blueAll.chi2();
   double chi2_threshold_LFME = blueLFME.chi2();
   double chi2_threshold_LFNN = blueLFNN.chi2();
   double chi2_threshold_MENN = blueMENN.chi2();

   DVec vecLF, vecME, vecNN;
   ///////////////////////////////////////////////
   // Setup Correlated Random Number Generation //
   ///////////////////////////////////////////////
   TMatrixD covMat (kThree, kThree);
   // diagonal terms
   covMat (kLF, kLF) = sigmaLF * sigmaLF;
   covMat (kME, kME) = sigmaME * sigmaME;
   covMat (kNN, kNN) = sigmaNN * sigmaNN;
   // off-diagonal terms
   covMat (kLF, kNN) = covMat (kNN, kLF) = rhoLF_NN * sigmaNN * sigmaLF;
   covMat (kLF, kME) = covMat (kME, kLF) = rhoLF_ME * sigmaME * sigmaLF;
   covMat (kME, kNN) = covMat (kNN, kME) = rhoME_NN * sigmaNN * sigmaME;
   TMatrixD sqrtCovMat (kThree, kThree);
   sqrtMatrix (sqrtCovMat, covMat);
   // what the random numbers should be centered around
   TVectorD meanVec(kThree); 
   for (int loop = 0; loop < kThree; ++loop)
   {
      meanVec (loop) = 1; // in Beta units, we want them centered at 1
   } // for loop
   int trials = 100000;
   if (kGenRandomNumbers)
   {
      generateThreeSets (trials, sqrtCovMat, vecLF, vecME, vecNN);
   } else {
      readThreeSetsFromFile ("PEs.txt", vecLF, vecME, vecNN);
   }

   // check the mean, rms, and correlations
   outputMeanEtc (vecLF, vecME, vecNN);


   ///////////////
   // Check PEs //
   ///////////////
   trials = (int) vecLF.size();
   int worse_all  = 0;
   int worse_LFME = 0;
   int worse_LFNN = 0;
   int worse_MENN = 0;
   for (int loop = 0; loop < trials; ++loop)
   {
      // blueAll
      blueAll.setMean( kLF, vecLF[loop] );
      blueAll.setMean( kME, vecME[loop] );
      blueAll.setMean( kNN, vecNN[loop] );
      // blueLFME
      blueLFME.setMean( kLF_LFME, vecLF[loop] );
      blueLFME.setMean( kME_LFME, vecME[loop] );
      // blueLFNN
      blueLFNN.setMean( kLF_LFNN, vecLF[loop] );
      blueLFNN.setMean( kNN_LFNN, vecNN[loop] );
      // blueMENN
      blueMENN.setMean( kME_MENN, vecME[loop] );
      blueMENN.setMean( kNN_MENN, vecNN[loop] );
      // Calculate average and chi^2
      blueAll.calcBlueAverage();
      blueLFME.calcBlueAverage();
      blueLFNN.calcBlueAverage();
      blueMENN.calcBlueAverage();
      double chi2_all  = blueAll.chi2();
      double chi2_LFME = blueLFME.chi2();
      double chi2_LFNN = blueLFNN.chi2();
      double chi2_MENN = blueMENN.chi2();
      // Check to see if this value is worse than the threshold
      if (chi2_all  > chi2_threshold_all)   ++worse_all;
      if (chi2_LFME > chi2_threshold_LFME)  ++worse_LFME;
      if (chi2_LFNN > chi2_threshold_LFNN)  ++worse_LFNN;
      if (chi2_MENN > chi2_threshold_MENN)  ++worse_MENN;
   } // for loop
   cout << "Consistency: (" << trials << " trials)" << endl
        << "  All:      " 
        << Form ("%6.3f%%",  100. * ( (double) worse_all  / trials) ) << endl
        << "  LF vs ME: " 
        << Form ("%6.3f%%",  100. * ( (double) worse_LFME / trials) ) << endl
        << "  LF vs NN: " 
        << Form ("%6.3f%%",  100. * ( (double) worse_LFNN / trials) ) << endl
        << "  ME vs NN: " 
        << Form ("%6.3f%%",  100. * ( (double) worse_MENN / trials) ) << endl
        << endl;
   return 0;
}

void generateThreeSets (int trials, const TMatrixD& sqrtCovMat,
                        DVec &vecLF, DVec &vecME, DVec &vecNN)
{
   const int kThree = 3;
   vecLF.clear();
   vecME.clear();
   vecNN.clear();
   TVectorD meanVec(kThree); 
   for (int loop = 0; loop < kThree; ++loop)
   {
      meanVec (loop) = 1; // in Beta units, we want them centered at 1
   } // for loop
   for (int loop = 0; loop < trials; ++loop)
   {
      TVectorD retVec (kThree);
      gaussCorrRand (retVec, sqrtCovMat, meanVec);
      vecLF.push_back( retVec (kLF) );
      vecME.push_back( retVec (kME) );
      vecNN.push_back( retVec (kNN) );
   } // for loop 
}

void readThreeSetsFromFile (string filename,
                            DVec &vecLF, DVec &vecME, DVec &vecNN)
{
   vecLF.clear();
   vecME.clear();
   vecNN.clear();
   ifstream source (filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "Can not open '" << filename << "' for reading.  Aborting."
           << endl;
      return;
   } // if opening file failed
   string line;
   while ( getline (source, line) )
   {
      SVec words;
      split (words, line);
      // are there at least three words?
      if (words.size() < 3)
      {
         // don't bother with this line
         continue;
      } // if we don't have enough words
      double me = atof ( words[0].c_str() );
      double nn = atof ( words[1].c_str() );
      double lf = atof ( words[2].c_str() );
      if (me || nn || lf)
      {
         // at least one of these is not zero
         vecLF.push_back( lf );
         vecME.push_back( me );
         vecNN.push_back( nn );
      } // if one non-zero
   } // while getline
}

void outputMeanEtc (const DVec &vecLF, const DVec &vecME, const DVec &vecNN)
{
   double meanLF, rmsLF, meanME, rmsME, meanNN, rmsNN;
   calcMeanRMS (meanLF, rmsLF, vecLF);
   calcMeanRMS (meanME, rmsME, vecME);
   calcMeanRMS (meanNN, rmsNN, vecNN);
   cout << "PE Mean +- RMS:" << endl
        << "  LF: " << Form ("%.3f", meanLF) 
        << " +- "   << Form ("%.3f", rmsLF)  << endl
        << "  ME: " << Form ("%.3f", meanME) 
        << " +- "   << Form ("%.3f", rmsME)  << endl
        << "  NN: " << Form ("%.3f", meanNN) 
        << " +- "   << Form ("%.3f", rmsNN)  << endl << endl;
   cout << "PE Correlations:" << endl
        << "  LF vs. ME: " 
        << Form ("%.3f", correlationCoeff (vecLF, vecME) ) << endl
        << "  LF vs. NN: " 
        << Form ("%.3f", correlationCoeff (vecLF, vecNN) ) << endl
        << "  ME vs. NN: " 
        << Form ("%.3f", correlationCoeff (vecME, vecNN) ) << endl << endl;
}
