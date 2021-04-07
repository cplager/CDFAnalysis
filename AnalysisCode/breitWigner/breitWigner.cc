// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TRandom3.h"
#include "TMath.h"

#include "CLPClasses/CLPClasses.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;


// number of events (numwanted/howmany)
int gNumWanted     = (int) 1e4;
// target file name (target)
string gTarget     = "dist";
// verbose mode (verbose)
bool gVerbose      = false;
// Z width to use (zwidth)
double gZwidth  = 2.5;
// error resolution
double gRelDeltaE  = 0.035;


void initializeOptionsMap();

// Conversion from Gaussian 'alpha' to BW 'beta'
// *beta  - converted Breit Wigner variable
// *deriv - d(beta)/d(alpha)
// alpha  - Gaussian variable
// sigma  - width of Gaussian distribution,
// gamma  - width of Breit Wigner distribution (same as 'sigma' if negative)
// alpha0 - mean  of Gaussian distribution,
// beta0  - mean  of Breit Wigner distribution
// * - returned values
void toBreitWigner (double &beta, double &deriv, 
                    double alpha, double sigma, double gamma = -1, 
                    double alpha0 = 1, double beta0 = 1);

// returns the alpha value associated with a given beta
double fromBreitWigner (double beta,  double sigma, double gamma = -1, 
                        double alpha0 = 1, double beta0 = 1);


int main(int argc, char** argv)
{
   initializeOptionsMap();
   parseArguments (argc, argv);
   double epsilon = 0.001;

   // update output name
   gTarget += ".root";

   TFile *targetPtr = new TFile ( gTarget.c_str(), "RECREATE" );
   if (! targetPtr->IsOpen())
   {
      cerr << "Sorry Dude" << endl;
      exit (1);
   }

   //////////////////
   // Output Trees //
   //////////////////
   TH1F *gausHist  = new TH1F ("gauss", "Gauss", 400, 0., 2.);
   TH1F *breitHist = new TH1F ("breit", "Breit", 400, 0., 2.);

   // TRandom has problems.  Use TRandom3 instead.
   delete gRandom;
   gRandom = new TRandom3;

   for (int loop = 0; loop < gNumWanted; ++loop)
   {
      double sigma = 0.02;
      double gaus = gRandom->Gaus(1, sigma);
      double breit, deriv;
      toBreitWigner (breit, deriv, gaus, sigma);
      double gausCheck = fromBreitWigner (breit, sigma);
      if ( fabs(gaus - gausCheck) > 0.0001)
      {
         cout << "gaus " << gaus << "  :  " << gausCheck << endl;
      }
      cout << gaus - gausCheck << endl;
      gausHist->Fill  (gaus);
      breitHist->Fill (breit);
      if (0)
      {
         double junk, breitM, breitP;
         toBreitWigner (breitP, junk, gaus + epsilon, 0.1);
         toBreitWigner (breitM, junk, gaus - epsilon, 0.1);
         double deriv2 = (breitP - breitM) / (2 * epsilon);
         cout << "deriv " << deriv << " " << deriv2 << endl;
      } // if 0
   } // for loop
   gausHist->Write();
   breitHist->Write();
   return 0;
}

void toBreitWigner (double &beta, double &deriv, 
                    double alpha, double sigma, double gamma,
                    double alpha0, double beta0)
{
   // These formulas are given in Aleph note 95-111 (page 6).

   // If no gamma (BW width) is given, use sigma (G width)
   if (gamma < 0)
   {
      gamma = sigma;
   }

   // Setup some temporary varables to make our life easier
   double beta02  = sq (beta0);
   double gamma2  = sq (gamma);
   double delta   = sqrt ( 4. * gamma2 * beta02 - pow (gamma, 4.)  );
   double lambda  = TMath::Pi() / 4.
      + 0.5 * atan( (2 * beta02 - gamma2) / delta );
   double erfArg  = (alpha - alpha0) / ( sqrt(2.) * sigma );
   double tanarg  = lambda * TMath::Erf( erfArg ) + atan( gamma2 / delta );
                    
   beta  = sqrt ( beta02 - gamma2 / 2 + (delta / 2) * tan (tanarg) );
   deriv = ( (delta * lambda) / (2 * beta) )
      * ( 1 / ( sqrt( 2 * TMath::Pi() )  * sigma ) )
      * exp ( -1 * erfArg * erfArg )
      * pow ( cos (tanarg), -2);
}

double fromBreitWigner (double beta,  double sigma, double gamma, 
                        double alpha0, double beta0)
{
   if (gamma < 0)
   {
      gamma = sigma;
   }
   double beta2   = sq (beta);
   double beta02  = sq (beta0);
   double gamma2  = sq (gamma);
   double delta   = sqrt ( 4. * gamma2 * beta02 - pow (gamma, 4.)  );
   double lambda  = TMath::Pi() / 4.
      + 0.5 * atan( (2 * beta02 - gamma2) / delta );
   double atanArg = (2 * beta2 - 2 * beta02 + gamma2) / delta;
   double invErfArg = ( atan (atanArg) - atan( gamma2 / delta ) ) / lambda;
   return alpha0 + sqrt(2.) * sigma * TMath::ErfInverse (invErfArg);
}


void initializeOptionsMap()
{
   // Int maps
   ns_IntMap["numwanted"]  = (void*) &gNumWanted;
   ns_IntMap["num"]        = (void*) &gNumWanted;
   ns_IntMap["howmany"]    = (void*) &gNumWanted;
   // String maps
   ns_StringMap["target"]  = (void*) &gTarget;
   // Bool maps
   ns_BoolMap["verbose"]   = (void*) &gVerbose;
   // Double maps
   ns_DoubleMap["zwidth"]  = (void*) &gZwidth;
   ns_DoubleMap["deltae"]  = (void*) &gRelDeltaE;
}
