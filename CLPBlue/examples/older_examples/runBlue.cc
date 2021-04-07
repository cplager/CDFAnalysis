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
   CLPBlue blue1;
   blue1.readFromFile( argv[1] );
   double mean, error;
   //  blue1.calcBlueAverage(mean, error);
   //  cout << "once   " << mean << " +-" << error << endl;
   blue1.calcIterativeBlueAverage(mean, error);
   cout << "iter:  " << mean << " +-" << error << endl;
   //cout << "blue1 " << endl << blue1 << endl;
}
