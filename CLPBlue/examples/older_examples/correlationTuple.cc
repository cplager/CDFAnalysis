// -*- C++ -*-
// Standard include files
#include <math.h>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <stdio.h>
// CLP include files
#include "XsecComb.h"
#include "CLPBlue.h"
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
   bool doAccptErrors = false;
   bool doBkgndErrors = false;
   TString extension = "";
   for (int loop = 2; loop <= argc; ++loop)
   {
      TString arg = argv[loop - 1];
      if (arg.BeginsWith("back", TString::kIgnoreCase))
      {
         doBkgndErrors = true;
         continue;
      }
      if (arg.BeginsWith("acc", TString::kIgnoreCase))
      {
         doAccptErrors = true;
         continue;
      }
      //  // convert from TString to string
      //  string sarg = (const char*) arg;
      //  cout << "sarg " << sarg << endl;
   }

   double accept, acceptRecip;
   TTree *treePtr = new TTree("random", "Random pieces");
   // cross sections
   treePtr->Branch("accept"     , &accept     , "accept/D"     );
   treePtr->Branch("acceptRecip", &acceptRecip, "acceptRecip/D");

   //////////////////////
   // Let's go play... //
   //////////////////////
   for (int loop = 1; loop <= 100000; ++loop)
   {
      if (0 == loop % 1000)
      { 
         cout << ".";
         cout.flush();
      }
      //////////////////////////////
      // Throw all random numbers //
      //////////////////////////////
      accept = gRandom->Gaus (1.0, 0.08);
      if (accept > 0)
      {
         acceptRecip = 1 / accept;
      } else {
         acceptRecip = 0;
      }
      // You worked this hard, you might as well record
      // everything
      treePtr->Fill();
   } // for loop
   cout << endl << "done" << endl;
   
   // write tree
   TString filename = "random" + extension + ".root";
   TFile file (filename, "RECREATE", "hist file");
   treePtr->Write();
   cout << "Wrote " << filename << endl;
   file.Close();
}
