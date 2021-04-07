// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include "RootFileCollection/OptionUtils.h"
#include "FCBinClass/FCBinClass.hh"
#include "FCBinClass/dout.hh"

using namespace std;

int main(int argc, char** argv)
{
   FCBinClass::FCBinFirstCall();
   OptionUtils::parseArguments (argc, argv);
   FCBinClass::orDebug( FCBinClass::kScanExpected_debug );
   //OptionUtils::setVariablesFromFile ("options.txt");
   // After EVERYTHING (but trueValue) is set,
   // we initialize the class
   FCBinClass::initialize (FCBinClass::kScanExpectedLimits);

   // Note: AFter this point, if you were to update a background or an
   // acceptance, but NOT the number of dimensions or one of the 'max'
   // variables, you can/need to do a 'FCBinClass::updateAll();'
   FCBinClass::scanExpectedLimits();
}
