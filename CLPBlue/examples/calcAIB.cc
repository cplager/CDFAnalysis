// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <cmath>

#include "TClonesArray.h"
#include "TTree.h"
#include "TSystem.h"
#include "TRandom3.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TMath.h"
#include "TString.h"

#include "CLPBlue/CLPAsymBlue.hh"


using namespace std;

int main(int argc, char** argv)
{
   if (argc < 2)
   {
      cout << "Usage: " << argv[0] << " bla.aib" << endl;
      exit(0);
   }
   bool statOnly = false;
   for (int index = 2; index < argc; ++index)
   {
      TString option (argv[index]);
      option.ToLower();
      if ("statonly" == option)
      {
         statOnly = true;
         break;
      }
      // if we're still here, complain
      cerr << "Do not understand option '" << option << "'.  Aborting."
           << endl;
      exit(1);
   }
   CLPAsymBlue aib;
   aib.readFromFile (argv[1]);
   if (statOnly)
   {
      aib.setNumErrors(1);
      cout << "Stat only" << endl;
   }

   double mean, upper, lower;
   aib.calcIterativeBlueAverage (mean, upper, lower);
   cout << aib << endl;
}
