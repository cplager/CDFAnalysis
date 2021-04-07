// -*- C++ -*-
// Standard include files
#include <math.h>
#include <iostream>
#include <assert.h>
#include <stdio.h>
// CLP include files
#include "XsecComb.h"
#include "CLPAsymBlue.h"
#include "CLPUtils.h"
#include "dout.h"
// Root include files
#include "TSystem.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

using namespace std;

int main(int argc, char** argv)
{
   /////////////////////
   // Parse Arguments //
   /////////////////////
   ////////////////
   // Setup BLUE //
   ////////////////
   CLPAsymBlue blue1;
   blue1.readFromFile( "blue_files/hadDil.asymBlue" );
   double mean, lower, upper;
   blue1.calcBlueAverage(mean, upper, lower);
   cout << "once   " << mean << " +" << upper << " -" << lower << endl;
   blue1.calcIterativeBlueAverage(mean, upper, lower);
   cout << "iter:  " << mean << " +" << upper << " -" << lower << endl;
}
