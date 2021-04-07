
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

int main(int argc, char** argv)
{
   // TRandom is lousy.  Use TRandom3 instead.
   delete gRandom;
   gRandom = (TRandom*) new TRandom3;

   const int kTrials = 100000;
   DVec vec1, vec2;
   for (int loop = 0; loop < kTrials; ++loop)
   {
      // The assumption:
      // There are two measurements, one uses 200 pb^-1, the other
      // uses 350 pb^-1.  Both measurements share 150 pb^-1, so one
      // has 50 pb^-1 that isn't overlap, and the other has 200 pb^-1.
      // The ppbar and corr uncertainties apply in the same way to all
      // three pieces.  The uncorrelated part applies separately.
      double unique200  = 50;   // 200 pb^-1 unique lumi
      double overlap    = 150;  // overlap lumi
      double unique350  = 200;  // 350 pb^01 unique lumi
      // Error due to uncertainty on inelastic ppbar cross section (4.0%)
      double ppbar      = gRandom->Gaus (0., 0.040);
      // Correlated error due to luminosity detector (3.9%)
      double corr       = gRandom->Gaus (0., 0.039);
      // Uncorrelated errors due to luminosity detector (1.7%)
      double uncorr200  = gRandom->Gaus (0., 0.017);
      double uncorrOver = gRandom->Gaus (0., 0.017);
      double uncorr350  = gRandom->Gaus (0., 0.017);
      // multiply each piece by its error
      unique200 *= (1 + ppbar + corr + uncorr200);
      overlap   *= (1 + ppbar + corr + uncorrOver);
      unique350 *= (1 + ppbar + corr + uncorr350);
      vec1.push_back( unique200 + overlap );
      vec2.push_back( unique350 + overlap );

      // // If we want to figure out the MINIMUM correlation two
      // // measurements could have, then we want to use the following
      // // code.
      // double ppbar   = gRandom->Gaus (0., 0.040);
      // double corr    = gRandom->Gaus (0., 0.039);
      // double uncorr1 = gRandom->Gaus (0., 0.017);
      // double uncorr2 = gRandom->Gaus (0., 0.017);
      // double val1 = 1 + ppbar + corr + uncorr1;
      // double val2 = 1 + ppbar + corr + uncorr2; 
      // vec1.push_back (val1);
      // vec2.push_back (val2);

   } // for loop

   ////////////////////
   // Output Results //
   ////////////////////
   double mean1, rms1, mean2, rms2;
   calcMeanRMS (mean1, rms1, vec1);
   calcMeanRMS (mean2, rms2, vec2);
   cout << "PE Mean +- RMS:" << endl
        << "  1: " << Form ("%.3f", mean1) 
        << " +- "   << Form ("%.3f", rms1)  << endl
        << "  2: " << Form ("%.3f", mean2) 
        << " +- "   << Form ("%.3f", rms2)  << endl;
   cout << "PE Correlations:" << endl
        << "  1 vs. 2: " 
        << Form ("%.3f", correlationCoeff (vec1, vec2) ) << endl
        << endl;
   return 0;
}
