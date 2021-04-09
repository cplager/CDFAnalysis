// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>

// Root include files
#include "TSystem.h"

// CLP include files
#include "CLPTemplateFitter/CLPTemplateFitter.hh"

using namespace std;

int main(int argc, char** argv)
{
   cout << __LINE__ << endl;
   TH1F *data  = new TH1F ("data",  "data",  3, 0.5, 3.5);   
   TH1F *temp1 = new TH1F ("temp1", "temp1", 3, 0.5, 3.5);   
   TH1F *temp2 = new TH1F ("temp2", "temp2", 3, 0.5, 3.5);   
   // data 3 1 1
   data->Fill (1, 3);
   data->Fill (2, 1);
   data->Fill (3, 2);
   // temp1 1 0 0
   temp1->Fill (1, 1);
   // temp2 1 1 1
   temp2->Fill (1, 1);
   temp2->Fill (2, 1);
   temp2->Fill (3, 1);
   CLPTemplateFitter fitter;
   fitter.addData (data);
   fitter.addTemplate ("temp1", temp1, 1.);
   fitter.addTemplate ("temp2", temp2, 1.);
   BinNormClass::ISet tempSet, binSet;
   tempSet.insert (1);
   binSet.insert (3);
   cout << __LINE__ << endl;
   fitter.addBinNorm ("thirdbin", tempSet, binSet, 2);
   cout << __LINE__ << endl;
   fitter.fit();
}
