#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include <math.h>
#include <iostream>
#include <stdio.h> // for sprintf
#include "CLPHisto.h"

using namespace std;

void hcentBands1()
{
   TCanvas c1;
   const int kSize = 4;
   const double kValues[kSize] = {0.025, 0.1586, 0.8414, 0.975};
   for (int loop = 51; loop <= 100; ++loop)
   {
      char filename[100], epsname[100], gifname[100];
      sprintf (filename, "fc_%d.root", loop);
      sprintf (epsname, "images/hcent_%03d.eps", loop);
      sprintf (gifname, "images/hcent_%03d.gif", loop);
      TFile *file = new TFile (filename);
      if (! file->IsOpen())
      {
         continue;
      }
      TH1F *hist = (TH1F*)file->Get("hcent");
      CLPHisto clpHist (hist);
      CLPHisto integ = clpHist.integrate();
      cout << endl << endl << endl << "b = " << loop / 100. << endl;
      for (int alpha = 0; alpha < kSize; ++alpha)
      {
         DVec xValues;
         integ.find(kValues[alpha], xValues, true);
         if (xValues.size())
         {
            cout << kValues[alpha] << ": " << xValues[0] << endl;
         } else {
            cout << "** No value for " << kValues[alpha] << endl;
         }
      } // for alpha
      
      hist->Draw();
      c1.Print(epsname);
      //c1->Print(gifname);
      file->Close();
      //delete file;
      //delete hist;
   } // for loop
}


