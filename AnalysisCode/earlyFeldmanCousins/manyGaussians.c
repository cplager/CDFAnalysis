#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TLine.h"
#include "CLPTypeDef.h"
#include <math.h>
#include <iostream>
#include <stdio.h> // for sprintf

using namespace std;

// global variables
double g66Array[8] = {2.70443e+03, 6.23008e-01, 1.46007e-01, 1.65495e-01, 1.35940e-04, 6.76540e-01, 9.99979e-01, 1.50649e+00};
DVec g66Vec( g66Array, g66Array + 8);
double g67Array[8] = {2.68686e+03, 6.33120e-01, 1.47165e-01, 1.70939e-01, 7.37206e-01, 7.48476e-01, 9.99997e-01, 1.53177e+00}; 
DVec g67Vec( g67Array, g67Array + 8);
double g68Array[8] = {2.67006e+03, 6.41230e-01, 1.46798e-01, 1.76558e-01, 9.87226e-01, 7.94914e-01, 9.99995e-01, 1.54326e+00}; 
DVec g68Vec( g68Array, g68Array + 8);
double g69Array[8] = {2.65852e+03, 6.51998e-01, 1.48336e-01, 1.73151e-01, 2.97902e-04, 7.44342e-01, 9.99993e-01, 1.52169e+00}; 
DVec g69Vec( g69Array, g69Array + 8);
double g70Array[8] = {2.64491e+03, 6.62158e-01, 1.49465e-01, 1.71332e-01, 9.96639e-01, 7.03335e-01, 1.00002e+00, 1.50286e+00} ;
DVec g70Vec( g70Array, g70Array + 8);


// function definitions
void manyGaussians();
Double_t gauss66(Double_t *x, Double_t *par);
Double_t gauss67(Double_t *x, Double_t *par);
Double_t gauss68(Double_t *x, Double_t *par);
Double_t gauss69(Double_t *x, Double_t *par);
Double_t gauss70(Double_t *x, Double_t *par);
// Double bifircated Gaussian at x with parameters passed in to params
Double_t splitGauss(Double_t x, const DVec &params);


void manyGaussians()
{
   TCanvas *c1 = new TCanvas;
   TH2F *h2 = new TH2F ("h2", "Some Title Here", 
                        2, 0, 1.2, 2, 0, 4000);
   TF1 *func67 = new TF1("gauss67", gauss67, 0, 1.2, 0);
   TF1 *func68 = new TF1("gauss68", gauss68, 0, 1.2, 0);
   TF1 *func69 = new TF1("gauss69", gauss69, 0, 1.2, 0);
   TF1 *func70 = new TF1("gauss70", gauss70, 0, 1.2, 0);
   double b = 0.662;
   double p67 = splitGauss (b, g67Vec);
   double p68 = splitGauss (b, g68Vec);
   double p69 = splitGauss (b, g69Vec);
   double p70 = splitGauss (b, g70Vec);
   TLine line67(b, 0, b, p67);
   TLine line68(b, 0, b, p68);
   TLine line69(b, 0, b, p69);
   TLine line70(b, 0, b, p70);
   cout << "67: " << p67 << endl
        << "68: " << p68 << endl
        << "69: " << p69 << endl
        << "70: " << p70 << endl;
   func67->SetLineColor(kBlue  );
   func68->SetLineColor(kYellow);
   func69->SetLineColor(kGreen );
   func70->SetLineColor(kRed   );
   line67.SetLineColor(kBlue  );
   line68.SetLineColor(kYellow);
   line69.SetLineColor(kGreen );
   line70.SetLineColor(kRed   );
   h2->Draw();
   func70->Draw("same");
   line70.Draw();
   c1->Print("try1.eps");
   //func69->Draw("same");
   //line69.Draw();
   //c1->Print("try2.eps");
   //func68->Draw("same");
   //line68.Draw();
   //c1->Print("try3.eps");
   func67->Draw("same");
   line67.Draw();
   c1->Print("try4.eps");
}

Double_t gauss66(Double_t *x, Double_t *par)
{
   return splitGauss (*x, g66Vec);
}

Double_t gauss67(Double_t *x, Double_t *par)
{
   return splitGauss (*x, g67Vec);
}

Double_t gauss68(Double_t *x, Double_t *par)
{
   return splitGauss (*x, g68Vec);
}

Double_t gauss69(Double_t *x, Double_t *par)
{
   return splitGauss (*x, g69Vec);
}

Double_t gauss70(Double_t *x, Double_t *par)
{
   return splitGauss (*x, g70Vec);
}

Double_t splitGauss(Double_t x, const DVec &params)
{
   double norm   = params[0];
   double arg    = x - params[1];
   double sigma1 = params[2]; // negative sigma1
   double frac   = params[4]; // negative fraction
   double sigma2 = sigma1 * params[6]; // negative sigma2
   if (arg > 0)
   {
      sigma1 = params[2 + 1]; // positive sigma1
      frac   = params[4 + 1]; // positive fraction
      sigma2 = sigma1 * params[6 + 1]; // positive sigma2
   }
   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1)) 
      + norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2));
   return fitval;
}



