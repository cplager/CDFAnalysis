#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include <math.h>
#include <iostream>
#include <stdio.h> // for sprintf

using namespace std;
const double kCenter = 0.018;
const double kWidth = 0.005;

void fitPEdists();
Double_t splitGauss(Double_t *x, Double_t *par);

void fixParameter(TF1 *func, int parameter)
{
   double value = func->GetParameter(parameter);
   func->FixParameter(parameter, value);
}

void floatParameter(TF1 *func, int param, double lower, double higher)
{
   double value = func->GetParameter(param);
   func->SetParLimits(param, lower, higher);
   func->SetParameter(param, value);
}

void fixNonBump (TF1 *func, int loop = 3)
{
      // fix everything but the Zero_Const
      func->FixParameter(0, 0.); // Constant",        
      func->FixParameter(1, loop/100.); // Max_bValue",      
      func->FixParameter(2, 0.1); // Negative_Sigma1", 
      func->FixParameter(3, 0.1); // Positive_Sigma1", 
      func->FixParameter(4, 0.5); // Negative_Frac",   
      func->FixParameter(5, 0.5); // Positive_Frac",   
      func->FixParameter(6, 3.); // Negative_SigFrac",
      func->FixParameter(7, 3.); // Positive_SigFrac",
      // tell parameter 8 it must be positive
      func->SetParLimits(8, 0, 1e6);
      func->SetParLimits(9, 0.002, 0.008);
      func->SetParLimits(10, 0.01, 0.02);
      func->SetParameter(8, 8e3);
      func->SetParameter(9, kWidth);
      func->SetParameter(10, kCenter);
}

void fixBump (TF1 *func)
{
      floatParameter(func, 0, 0., 1e6);
      floatParameter(func, 1, -0.1, 1.1);
      floatParameter(func, 2, 0., 3.);
      floatParameter(func, 3, 0., 3.);
      floatParameter(func, 4, 0, 1);
      floatParameter(func, 5, 0, 1);
      floatParameter(func, 6, 0.2, 5);
      floatParameter(func, 7, 0.2, 5);
      fixParameter(func, 8);
      fixParameter(func, 9);
      fixParameter(func, 10);
}


void fitPEdists()
{
   TCanvas c1;
   //const double kDeltaEpsilon = 0.436;
   TFile *file = new TFile ("all.root");
   if (! file->IsOpen())
   {
      return;
   }
   for (int loop = 2; loop <= 100; ++loop)
   //for (int loop = 2; loop <= 9; ++loop)
   {
      //if ((loop != 5) & (loop != 37)) continue;
      char histname[100], epsname[100], gifname[100];
      sprintf (histname, "Rgen%03d", loop);
      sprintf (epsname, "images/sysFit_%03d.eps", loop);
      sprintf (gifname, "images/sysFit_%03d.gif", loop);
      TH1F *hist = (TH1F*)file->Get(histname);
      if (hist) {
          cout << endl << endl << endl << "Using " << histname << endl;
      } else {
         cout << "Couldn't find " << histname << endl;
         continue;
      }
      
      //hist->SetMaximum(7000);
      double mean = hist->GetMean();
      cout << "mean " << mean << endl;
      TF1 *func = new TF1("splitGauss",splitGauss,0,1,11);
      // Sets initial values and parameter names
      func->SetParNames("Constant",         //  0
                        "Max_bValue",       //  1
                        "Negative_Sigma1",  //  2
                        "Positive_Sigma1",  //  3
                        "Negative_Frac",    //  4
                        "Positive_Frac",    //  5
                        "Negative_SigFrac", //  6
                        "Positive_SigFrac", //  7
                        "Zero_Const",       //  8
                        "Zero_Width",       //  9
                        "Zero_Center"       // 10
         );
      func->SetNpx(400);
      func->SetLineWidth(1);
      fixNonBump(func, loop);
      hist->Fit("splitGauss","W");      
      fixBump(func);
      hist->Fit("splitGauss","W");      
      hist->Fit("splitGauss","M");      
      hist->SetLineColor(kRed);
      hist->Draw();
      c1.Print(epsname);
      delete func;
      //delete bHist;
      //c1->Print(gifname);
      //delete hist;
   } // for loop
   file->Close();
   delete file;
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
   double zCenter = par[10];
   if (arg > 0)
   {
      sigma1 = par[2 + 1]; // positive sigma1
      frac   = par[4 + 1]; // positive fraction
      sigma2 = sigma1 * par[6 + 1]; // positive sigma2
   }
   double zValue = value - zCenter;
   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1)) 
      + norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2))
      + zConst * TMath::Exp( -0.5 * zValue * zValue / (zWidth * zWidth));
   return fitval;
}















