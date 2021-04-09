#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "CLPHisto.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdio.h> // for sprintf
#include <assert.h>

using namespace std;

const double kMaxError = 0.0001;

Double_t b50_gauss[8]  = {3.15786e+03, 4.46832e-01, 1.19294e-01, 1.38139e-01, 
                          4.75722e-01, 5.88579e-01, 9.99999e-01, 1.58044e+00};
Double_t b100_gauss[8] = {2.36022e+03, 9.54886e-01, 1.48475e-01, 1.13842e-01, 
                          2.92967e-11, 1.14140e-01, 1.16595e+00, 2.00780e+00};
 
Double_t *g_dPtr = b50_gauss;
Double_t g_b0 = 0.50;
Double_t g_bIdeal = 0.4468;

CLPHisto areaHist;
bool filled = false;

Double_t likeliRat(Double_t *x, Double_t *par = g_dPtr);
Double_t bBest(Double_t *x);
Double_t splitGauss(Double_t *x, Double_t *par);
void calculateIntegrated();
Double_t findFCinterval (Double_t size, Double_t &xLower, Double_t &xHigher);

Double_t likeliRat(Double_t *x, Double_t *par)
{
   Double_t bGivenb0 = splitGauss (x, g_dPtr);
   Double_t bBst = bBest(x);
   assert (bBst > 0); // just to make sure
   const double P0 =  7.85225e+03;
   const double P1 = -2.49030e+04;
   const double P2 =  5.84744e+04;
   const double P3 = -7.81668e+04;
   const double P4 =  5.37288e+04;
   const double P5 = -1.46157e+04;
   double v = bBst;
   Double_t bGivenbBest;
   // are we in the physical region?
   if (bBst < 1)
   {
      // Yep, just use the fit of p (b | b best)
      bGivenbBest = P0 + P1 * v + P2 * v * v + P3 * v * v * v
         + P4 * v * v * v * v + P5 * v * v * v * v * v;
   } else {
      // NOT physical.  Use p (b | b = 1.0)
      bGivenbBest = splitGauss (x, b100_gauss);
   }
   if (bGivenbBest > 0)
   {
      return bGivenb0 / bGivenbBest;
   } else {
      return 0;
   }
}

// Returns 
Double_t bBest(Double_t *x)
{
   double v = x[0];
   const double bBestP0 =  5.16022e-02;
   const double bBestP1 =  1.02742e+00;
   const double bBestP2 = -3.84807e-01;
   const double bBestP3 =  1.32905e+00;
   const double bBestP4 = -1.60638e+00;
   const double bBestP5 =  6.22797e-01;
   return bBestP0 + bBestP1 * v + bBestP2 * v * v + bBestP3 * v * v * v
      + bBestP4 * v * v * v * v + bBestP5 * v * v * v * v * v;
}

void calculateIntegrated()
{
   double lower = 0;
   double upper = 2.5;
   int bins = 1000;
   double stepsize = (upper - lower) / bins;
   double halfstep = stepsize / 2;
   CLPHisto dist (bins + 1, lower - halfstep, upper + halfstep);
   // let's fill this puppy
   for (int loop = 0; loop < bins; ++loop)
   {
      double x = lower + loop * stepsize;
      double weight = splitGauss (&x, g_dPtr);
      dist.fill (x, weight);
   } // for loop
   areaHist = dist.integrate();
   filled = true;
}

Double_t findFCinterval (Double_t size, Double_t &x1, Double_t &x2)
{
   if (! filled)
   {
      calculateIntegrated();
   }
   ///////////////////////////////////////////////////////////////
   // A couple thoughts before we start.                        //
   // We are in the regime where R peaks at bBest (b0) and is   //
   // strictly increasing up to that point and strictly         //
   // decreasing after that point.  This means that searching   //
   // is a lot easier than it could be.                         //
   //                                                           //
   // Start with an x1 a bit lower than bBest (b0).  We         //
   // then find x2 such that R(x1) = R(x2). Given               //
   // this range, we check to see what percentage of the        //
   // pseudo-experiments falls in this range.                   //
   //                                                           //
   // If the percentage is too small, we move x1 lower and      //
   // start over.  If the percentage is too big, we move x1     //
   // up.                                                       //
   //                                                           //
   // We will keep doing this until we get close enough.        //
   ///////////////////////////////////////////////////////////////
   // initialize defaults
   for (int loop = 0; loop <= 100; ++loop)
   {
      if (0 == loop % 10)
      {
         //cout << endl << setw(2) << loop << ") ";
      }
      //double x = loop / 100.;
      //cout << likeliRat (&x) << " ";
   }
   //cout << endl;
   double lower1 = 0;    // smaller X1
   double lower2 = 2.5;  // larger X2
   double lowerArea = 1;
   double upper1 = g_bIdeal - 0.02; // larger X1
   double upper2 = g_bIdeal + 0.02; // smaller X2
   double upperArea = 0;
   double area = 1;
   // loop until the area is close enough to what we want
   while (fabs(area - size) > kMaxError)
   {
      x1 = (lower1 + upper1) / 2;
      x2 = (lower2 + upper2) / 2;
      //cout << endl << endl << "area " << area 
      //     << " x1 " << x1 << " x2 " << x2 << endl;
      // loop until |R(x1) - R(x2)| is small enough
      double diffR = likeliRat(&x2) - likeliRat(&x1);
      // use previous information about the range
      double bigger2 = lower2;
      double smaller2 = upper2;
      double oldDiffR = 0;
      //cout << "bigger2 " << bigger2 << " smaller2 " << smaller2 << endl;
      while (fabs(diffR) > kMaxError)
      {
         //cout << "  x1 " << x1 << " x2 " << x2 << " diff " << diffR << endl;
         //cout << "  R(x1) " << likeliRat(&x1) << " R(x2) " 
         //     << likeliRat(&x2) << endl;
         // O.K., we're not close enough.  Are we too big or too small
         if (diffR > 0)
         {
            // R(x2) > R(x1).  This means that we need to increase x2
            smaller2 = x2;
            x2 = (x2 + bigger2) / 2; 
            //cout << "  increasing x2 from " << smaller2 << " to " << x2
            //      << endl << endl;
         } else {
            // R(x2) < R(x1).  We need to decrease x2
            bigger2 = x2;
            x2 = (x2 + smaller2) / 2; 
            //cout << "  decreasing x2 from " << bigger2 << " to " << x2
            //     << endl << endl;
         }
         assert (fabs(diffR - oldDiffR) > kMaxError);
         oldDiffR = diffR;
         diffR = likeliRat(&x2) - likeliRat(&x1);
      } // while diffR too big
      // get the area of the pair
      area = areaHist.interpolate(x2) - areaHist.interpolate(x1);
      // rearrange end points accordingly
      if (area > size)
      {
         // area too big
         // this means we nead to increase X1
         lower1 = x1;
         lower2 = x2;
         lowerArea = area;
      } else {
         // area too small
         // decrease X1
         upper1 = x1;
         upper2 = x2;
         upperArea = area;
      }
   } // while error too big
   return area;
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
