#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <stdio.h> // for sprintf

using namespace std;

void fitbBestFromPE();
Double_t splitGauss(Double_t *x, Double_t *par);


void fitbBestFromPE()
{
   // open the file of histograms.
   TFile *file = new TFile ("all.root");
   if (! file->IsOpen())
   {
      cout << "Couldn't open all.root." << endl;
      return;
   }
   // get one histogram so that we can get the number of bins,
   // min and max
   TH1F *hist = (TH1F*)file->Get("Rgen098");
   if (hist) {
      //cout << endl << "Using " << histname << endl;
   } else {
      cout << "Couldn't find our sample histogram.  Aborting..."<< endl;
      return;
   }

   const int bMeasBins = hist->GetNbinsX();
   const double lower  = hist->GetXaxis()->GetXmin();
   const double upper  = hist->GetXaxis()->GetXmax();
   cout << "bMeasBins " << bMeasBins << " lower " << lower 
        << " upper " << upper << endl;
   const double kNumSteps = bMeasBins;
   const double kStepSize = (upper - lower) / kNumSteps;
   const int    kMinInt = 1;
   //const double kMin = kMinInt / 100.;
   const double kMin = 0.;
   const double kMax = 1.00;
   const double kb0StepSize = 0.01;
   const double kb0HalfStep = kb0StepSize / 2.;
   // 1 should work, but turning it into an int makes 60, not 61
   // so I use 1.001 to be "safe"
   const int    kb0Steps = int (1.001 + (kMax - kMin) / kb0StepSize);
   int minbMeasLoop = bMeasBins;
   int maxbMeasLoop = 1;
   vector< TH1F > histVec;
   bool okay = false;
   cout << "b0steps " << kb0Steps << " min " << kMin - kb0HalfStep
        << " max " << kMax + kb0HalfStep << endl;
   // loop over the number of bins
   // find the minimum and maximum bin such that we're still 
   // inside our [kMin, kMax]
   // Once we're inside our interval, make a histogram for each 
   // value of bMeasLoop and store it in a vector.
   for (int bMeasLoop = 0; bMeasLoop < bMeasBins; ++bMeasLoop)
   {
      // don't forget the 0.5 because we want the middle of the
      // bin, not the lower edge
      double bMeas = lower + (bMeasLoop + 0.5) * kStepSize;
      // are we the minimum?
      if ((bMeas > kMin) && !okay)
      {
         okay = true;
         minbMeasLoop = bMeasLoop;
         cout << "min " << bMeas << " bMeasLoop " << minbMeasLoop << endl;
      }
      // are we too big?
      if (bMeas > kMax)
      {
         okay = false;
         maxbMeasLoop = bMeasLoop - 1;
         double bMeas = lower + (bMeasLoop - 1) * kStepSize;
         cout << "max " << bMeas << " bMeasLoop " << maxbMeasLoop << endl;
         break;
      }
      // if we haven't met the minimum yet, then don't bother
      if (! okay)
      {
         continue;
      }
      cout << "bMeas " << bMeas << endl;
      char shortname[100], longname[100];
      sprintf (shortname, "b0Dist%03d", int (bMeas * 1000));
      sprintf (longname, "b0 Disttribution for bMeas = %.3f", bMeas);
      //cout << "short " << shortname << " long " << longname << endl;
      TH1F hist( shortname, longname, kb0Steps, 
                 kMin - kb0HalfStep, kMax + kb0HalfStep );
      hist.SetMaximum(10000);
      histVec.push_back( hist );
      
   } // for bMeasLoop
   cout << "We have made " << histVec.size() << " histograms." << endl;
   cout << __LINE__ << endl;
   for (int b0loop = kMinInt; b0loop <= 100; ++b0loop)      
   {
      double b0 = b0loop / 100.;
      char histname[100];
      sprintf (histname, "Rgen%03d", b0loop);
      hist = (TH1F*)file->Get(histname);
      if (hist) {
         //cout << endl << "Using " << histname << endl;
      } else {
         cout << "Couldn't find " << histname << endl;
         continue;
      }
      for (Int_t bMeasLoop = minbMeasLoop; 
           bMeasLoop <= maxbMeasLoop; ++bMeasLoop)
      {
         int bmeasBin = bMeasLoop - minbMeasLoop;
         double contents = hist->GetBinContent(bMeasLoop);
         histVec[bmeasBin].Fill (b0, contents);
      } // for bMeasLoop
   }
   //file->Close();
   //delete file;
   // For the hell of it, write out the orthoganol histograms.
   file = new TFile ("allRgen.root", "RECREATE", "root file");
   cout << __LINE__ << endl;
   for (unsigned int loop = 0; loop < histVec.size(); ++loop)
   {
      histVec[loop].Write();
   } // for loop
   file->Close();
   delete file;
   TF1 *func = new TF1("splitGauss",splitGauss,0, 1, 8);
   // Sets initial values and parameter names
   func->SetParNames("Constant",         // 0
                     "Max_bValue",       // 1
                     "Negative_Sigma1",  // 2
                     "Positive_Sigma1",  // 3
                     "Negative_Frac",    // 4
                     "Positive_Frac",    // 5
                     "Negative_SigFrac", // 6
                     "Positive_SigFrac"  // 7
      );
   for (unsigned int loop = 0; loop < histVec.size(); ++loop)
   {
      // don't forget, we want the middle of the bin,
      // not the lower edge.  Add 0.5
      double bmeas = lower + (loop + minbMeasLoop + 0.5) * kStepSize;
      char name[100];
      
      sprintf (name, "images/sys_b0dist%04d.eps", (int) (bmeas * 1000));
      cout << endl << endl << "Fiiting for " << name << ": " << bmeas << endl;
      func->SetParameters(3000, bmeas, 0.1, 0.1, 0.5, 0.5, 3, 3);
      func->SetParLimits(4, 0, 1);
      func->SetParLimits(5, 0, 1);
      func->SetParLimits(7, 0.2, 5);
      func->SetParLimits(6, 0.2, 5);
      histVec[loop].Fit("splitGauss","W");
      // Fit histogram in range defined by function
      TCanvas c1;
      histVec[loop].SetLineColor(kRed);      
      histVec[loop].Draw();
      c1.Print(name);
   }
}

Double_t splitGauss(Double_t *x, Double_t *par)
{
   double value = x[0];   
   double norm   = par[0];
   double arg    = value - par[1];
   double sigma1 = par[2]; // negative sigma1
   double frac   = par[4]; // negative fraction
   double sigma2 = sigma1 * par[6]; // negative sigma2
   double zConst = par[8];
   double zWidth = par[9];
   if (arg > 0)
   {
      sigma1 = par[2 + 1]; // positive sigma1
      frac   = par[4 + 1]; // positive fraction
      sigma2 = sigma1 * par[6 + 1]; // positive sigma2
   }
   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1)) 
      + norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2))
      + zConst * TMath::Exp( -0.5 * value * value / (zWidth * zWidth));
   return fitval;
}















