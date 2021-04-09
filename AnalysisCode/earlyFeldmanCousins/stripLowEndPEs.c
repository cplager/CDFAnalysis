#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include <math.h>
#include <iostream>
#include <stdio.h> // for sprintf
#include <vector>

using namespace std;

const int kBins = 20;

void stripLowEndPEs()
{
   vector< TH1F > histVec;
   TFile *file = new TFile ("all.root");
   if (! file->IsOpen())
   {
      return;
   }
   for (int loop = 2; loop <= 25; ++loop)
   {
      char reg[100], shortname[100];
      sprintf (reg, "Rgen%03d", loop);
      sprintf (shortname, "Rgen%03d_short", loop);
      TH1F *histPtr = (TH1F*)file->Get(reg);
      if (histPtr) {
          cout << endl << endl << endl << "Using " << reg << endl;
      } else {
         cout << "Couldn't find " << reg << endl;
         continue;
      }
      TH1F hist(shortname, shortname, kBins, 0., kBins / 100.);
      for (int bin = 1; bin <= kBins; ++bin)
      {
         float value = histPtr->GetBinContent(bin);
         hist.SetBinContent(bin, value);
      }
      histVec.push_back(hist);
   } // for loop
   unsigned int size = histVec.size();
   TFile newfile ("all_short.root", "recreate");
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      histVec[loop].Write();
   } // for loop

}
