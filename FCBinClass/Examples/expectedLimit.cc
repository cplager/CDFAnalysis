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
   FCBinClass::parseCommandLineArguments (argc, argv);
   // After EVERYTHING (but trueValue) is set,
   // we initialize the class
   FCBinClass::initialize();   
   // Note: AFter this point, if you were to update a background or an
   // acceptance, but NOT the number of dimensions or one of the 'max'
   // variables, you can/need to do a 'FCBinClass::updateAll();'
   // O.k. set the true value to 0 so we only have background
   FCBinClass::setTrueValue(0.);
   if (FCBinClass::userBool(0))
   {
      FCBinClass::setMode (FCBinClass::kFeldmanCousinsMode);
      double fcTooBig;
      double fcLim = FCBinClass::findExpectedLimit (fcTooBig);
      FCBinClass::setMode (FCBinClass::kBayesMode);
      double bayesTooBig;
      double bayesLim = FCBinClass::findExpectedLimit (bayesTooBig);
      cout << "Expected Limit: FC (" 
           << Form ("%.4f", fcLim) << ", " 
           << Form ("%.4f", fcTooBig) << ") Bayes (" 
           << Form ("%.4f", bayesLim) << ", " 
           << Form ("%.4f", bayesTooBig) << ")" 
           << endl;
      
   } else {
      double tooBig; // return value for too big value of upper limit
      double lim = FCBinClass::findExpectedLimit (tooBig);
      cout << "Expected Limit: " << Form ("%.4f", lim) << ", " 
           << Form ("%.4f", tooBig) << endl;
   }
}
