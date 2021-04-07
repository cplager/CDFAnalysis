// -*- C++ -*-

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cassert>

#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TError.h"

#include "FCPEs.hh"

#include "JointPhysics/JPUtils.hh"
#include "JointPhysics/dout.hh"
#include "RootFileCollection/OptionUtils.h"

using namespace std;


// global variables

// name of control file
string gControlName = "bffiles.txt";


// function declarations
void localInitialize();
void corporate_id();

int main (int argc, char** argv)
{
   corporate_id();

   // from http://root.cern.ch/phpBB2/viewtopic.php?t=3895
   gErrorIgnoreLevel = 9999;
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);
   FCPEs::initialize (gControlName);
   if (! FCPEs::loadChains())
   {
      cout << "problem" << endl;
      exit (-1);
   }

   // Should we run the stand alone fitter?  If we do, we don't do
   // anything else afterwards.
   if (FCPEs::SAFitFunc().size())
   {
      // Yep.  Here goes...
      FCPEs::StandAloneFitDistribution();
      // Don't bother doing anything else, so stop.
      return 0;
   }

   // string original = "123567893530";
   // string copy = original;
   // FCPEs::replaceAlphaWithBeta (copy, "35", "xyz");
   // cout << original << " : " << copy << endl;


   // If we're still here, then we're going to make pieces of the FC
   // bands.
   if (FCPEs::makeVerticalSlicesBool() && !  FCPEs::makeVerticalSlices())
   {
      cerr << "Make of vertical slice fit failed." << endl;
   }
   if (FCPEs::fitVerticalSlicesBool() && !  FCPEs::fitVerticalSlices() )
   {
      cerr << "Fitting of vertical slice fit failed." << endl;
      exit( -1 );
   }
   if (FCPEs::fitPbestBool() && ! FCPEs::fitPbest())
   {
      cerr << "Pbest fit failed." << endl;
      exit( -1 );
   }
   if (FCPEs::loadPEdistsBool() && ! FCPEs::loadPEdists())
   {
      cerr << "Pbest fit failed." << endl;
      exit( -1 );
   }
   if (FCPEs::makeLikeliRatBool() && ! FCPEs::makeLikeliRats())
   {
      cerr << "makeLikeliRats() failed." << endl;
      exit( -1 );
   }
   if (FCPEs::fitLikeliRatBool() && ! FCPEs::fitLikeliRats())
   {
      cerr << "fitLikeliRats() failed." << endl;
      exit( -1 );
   }
   if (FCPEs::makeAccBandsBool() && ! FCPEs::findAcceptanceBands())
   {
      cerr << "findAcceptanceBands() failed." << endl;
      exit( -1 );
   }
   if (FCPEs::plotLikeliRatBool() && ! FCPEs::plotLikeliRats())
   {
      cerr << "plotLikeliRats() failed." << endl;
      exit( -1 );
   }
   if (FCPEs::plotBandsBool() && ! FCPEs::plotBands())
   {
      cerr << "plotBands() failed." << endl;
      exit( -1 );
   }
   if (FCPEs::fitUpperBandEdgeBool() && ! FCPEs::fitUpperBandEdge())
   {
      cerr << "fitUpperBandEdge() failed." << endl;
      exit( -1 );
   }
   if (FCPEs::plotExpectedLimitBool() && ! FCPEs::plotExpectedLimit())
   {
      cerr << "plotExpectedLimit() failed." << endl;
      exit( -1 );
   }
   FCPEs::terminate();
   return 0;
}

void localInitialize()
{
   using namespace OptionUtils;
   addOptionKey ("control", gControlName,
                 "Name of control file");
}
