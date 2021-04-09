#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include <math.h>
#include <iostream>
#include <stdio.h> // for sprintf

using namespace std;

void hcentFits();
Double_t splitGauss(Double_t *x, Double_t *par);


void hcentFits()
{
   TCanvas c1;
   const double kDeltaEpsilon = 0.436;
   for (int loop = 20; loop <= 100; ++loop)
   //for (int loop = 52; loop <= 52; ++loop)
   {
      char filename[100], epsname[100], gifname[100];
      sprintf (filename, "root_files/fc_%d.root", loop);
      sprintf (epsname, "images/hcentFit_%03d.eps", loop);
      sprintf (gifname, "images/hcentFit_%03d.gif", loop);
      TFile *file = new TFile (filename);
      if (! file->IsOpen())
      {
         continue;
      }
      cout << endl << endl << endl << "Using " << filename << endl;
      TH1F *hist = (TH1F*)file->Get("hcent");
      int bins = hist->GetNbinsX();
      double lower = hist->GetXaxis()->GetXmin() / kDeltaEpsilon;
      double upper = hist->GetXaxis()->GetXmax() / kDeltaEpsilon;
      char shortname[100], longname[100];
      sprintf (shortname, "bDist%d", loop);
      sprintf (longname, "b Disttribution for %.2f", loop/100.);
      TH1F *bHist = new TH1F (shortname, longname, bins, lower, upper);
      for (Int_t innerLoop = 0; innerLoop <= bins + 1; ++innerLoop)
      {
         Double_t contents = hist->GetBinContent(innerLoop);
         bHist->SetBinContent(innerLoop, contents);
      } // for innerLoop
      double mean = bHist->GetMean();
      cout << "mean " << mean << endl;
      TF1 *func = new TF1("splitGauss",splitGauss, 0, 1, 8);
      // Sets initial values and parameter names
      func->SetParameters(3000, loop / 100., 0.1, 0.1, 0.5, 0.5, 3, 3);
      func->SetParNames("Constant",         // 0
                        "Max_bValue",       // 1
                        "Negative_Sigma1",  // 2
                        "Positive_Sigma1",  // 3
                        "Negative_Frac",    // 4
                        "Positive_Frac",    // 5
                        "Negative_SigFrac", // 6
                        "Positive_SigFrac"  // 7
         );
      func->SetParLimits(4, 0, 1);
      func->SetParLimits(5, 0, 1);
      func->SetParLimits(7, 0.2, 5);
      func->SetParLimits(6, 0.2, 5);
      // Fit histogram in range defined by function
      bHist->Fit("splitGauss","W");

      bHist->Draw();
      c1.Print(epsname);
      delete func;
      delete bHist;
      //c1->Print(gifname);
      file->Close();
      delete file;
      //delete hist;
   } // for loop
}

Double_t splitGauss(Double_t *x, Double_t *par)
{
   double value = x[0];   
   double norm   = par[0];
   double arg    = value - par[1];
   double sigma1 = par[2]; // negative sigma1
   double frac   = par[4]; // negative fraction
   double sigma2 = sigma1 * par[6]; // negative sigma2
   if (arg > 0)
   {
      sigma1 = par[2 + 1]; // positive sigma1
      frac   = par[4 + 1]; // positive fraction
      sigma2 = sigma1 * par[6 + 1]; // positive sigma2
   }
   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 *arg * arg / (sigma1 * sigma1)) + 
      norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2));
   return fitval;
}
