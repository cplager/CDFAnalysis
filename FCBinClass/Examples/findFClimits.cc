// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLine.h"
#include "FCBinClass/FCBinClass.hh"
#include "FCBinClass/dout.hh"

using namespace std;

int main(int argc, char** argv)
{
   // setup interal variables based on command line arguments
   FCBinClass::parseCommandLineArguments (argc, argv);
   // After EVERYTHING (but trueValue) is set,
   // we initialize the class
   FCBinClass::initialize();   
   // Note: AFter this point, if you were to update a background or an
   // acceptance, but NOT the number of dimensions or one of the 'max'
   // variables, you can/need to do a 'FCBinClass::updateAll();'

   // The 'find...Limit' functions use the default confidence limit
   // (sm_confidenceLevel) unless otherwise set
   double upper;
   double lower = -1;
   if (FCBinClass::kRootFeldmanCousinsMode == FCBinClass::mode())
   {
      upper = FCBinClass::findTFCUpperLimit ();
      cout << "Root's Feldman Cousins ";
   } else if (FCBinClass::kFeldmanCousinsMode == FCBinClass::mode()) 
   {
      upper = FCBinClass::findUpperLimit ();
      lower = FCBinClass::findLowerLimit ();
      cout << "lower " << lower << endl;
      cout << "Feldman Cousins ";
   } else {
      upper = FCBinClass::BayesUpperLimit ();
      cout << "Bayesian ";
      
   }
   cout << "limit: " << Form ("%0.3f", upper) << endl; 
   if (lower >= 0)
   {
      cout << "lower: " << Form ("%0.3f", lower) << endl; 
   }
   return 0;
}
