// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <algorithm>

// Root include files
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom3.h"

// CLP include files
#include "RootFileCollection/OptionUtils.h"
#include "CLPBlue/CLPTypeDef.hh"
#include "CLPBlue/CLPBlue.hh"
#include "CLPBlue/CLPUtils.hh"
#include "CLPBlue/dout.hh"
#include "dumpSTL.icc"

using namespace std;

// forward declarations
void localInitialize();


// Global variables (ugh!) for command line options
// (file)
string gFilename = "test2.txt";

// Switch to tell us which sets of files to load (which)

int main (int argc, char** argv)
{
   localInitialize();
   OptionUtils::parseArguments (argc, argv);   
   CLPBlue blue;
   // Tell blue not to worry about extra measurement numbers
   blue.setIgnoreLoadWarnings (true);  
   blue.readFromFile( gFilename );
   double mean, error;
   blue.calcIterativeBlueAverage(mean, error);
   cout << endl << blue << endl;
}

void localInitialize()
{
   using namespace OptionUtils;
   // option name is case insensitive.  Capitalization of the strings
   // is for readability only.
   addOptionKey ("file",   gFilename);
}
