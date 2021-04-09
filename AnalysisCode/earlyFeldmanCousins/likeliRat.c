// -*- C++ -*-
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "CLPHisto.h"
#include "StringUtils.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h> // for sprintf
#include <vector>
#include <assert.h>
#include "CLPTypeDef.h"
#include "dout.h"

using namespace std;

//typedef vector< Double_t > DVec;
//typedef map< int, DVec > IDVecMap;

const double kMaxError = 0.00001;
const double kVerySmall = 1e-10;
const int kSix   = 6;
const int kEight = 8;
const unsigned int kGaussSize_old = 8;
const unsigned int kGaussSize = 10;
const double kBiggestB = 2.5;
double zero = 0;
bool g_gifOnly = false;
const bool debug = false;


IDVecMap gaussMap;

// b100 Gaussian parameters
DVec b100_gaussVec;

// parameters to get bBest for a given bMeasured
DVec bBestVec;

// parameters to get P(bMeas | bBest)
DVec PbMeas_bBestVec;

// parameters to get peak in likelihood ratio given bBest
DVec peakLR_bBestVec;
 
DVec g_vec;
Double_t *g_pointless = 0;
Double_t g_b0 = 0.50;

CLPHisto areaHist, likeliRatHist;
bool g_filled = false;

// the likelihood ratio at x.  the '*par' argument is so likeliRat
// can be used as a function of a TF1
Double_t likeliRat(Double_t *x, Double_t *par = g_pointless);

// Given a value of x, returns the best b
Double_t bBest(Double_t x);

// Double bifircated Gaussian at x with parameters passed in to params
Double_t splitGauss(Double_t x, const DVec &params = g_vec);

// calculate the histogram that stores the integrated value of the
// bifricated Gaussian
void calculateIntegrated();

// loads information from file
void loadfile (const char filename[]);

// converts a vector of strings into a vector of doubles
void strings2vector (SVec &sList, DVec &retval, int offset = 1);

// Given the 'size' ]0, 1[
Double_t findFCinterval (Double_t size, Double_t &r, 
                         DVec &xValues, DVec &areas);
Double_t PbGivenb0 (Double_t x, const DVec &params = g_vec);
Double_t PbGivenbBest (Double_t x);
Double_t polynomial (Double_t x, const DVec &params);

// after loading the file, run the puppy
void findAllFCIntervals();

// The area of the distribution with a likelihood ratio >= r
Double_t areaGivenLikeliRat (Double_t r, DVec &xValues, DVec &areaVec);


void findAllFCIntervals()
{
   for (IDVecMapIter iter = gaussMap.begin();
        iter != gaussMap.end();
        ++iter)
   {
      double r;
      g_filled = false;
      int intB = iter->first;
      cout << "fitting " << intB << endl;
      g_vec = iter->second;
      g_b0 = intB / 100.;
      TCanvas *c1 = new TCanvas;
      char name[100], shortname[100];
      sprintf (name, "Likelihood Ratio for b0 = %.2f", g_b0);
      sprintf (shortname, "likratfor%d", intB);
      TH2F h2(shortname, name, 2, 0, 2.3, 2, 0, 1.1);
      h2.Draw();
      TF1 *likeR = new TF1("likeliRat", likeliRat, 0, 2.3, 0);
      likeR->SetMaximum(1.1);
      likeR->Draw("same");      
      char line[100];
      sprintf (line, "images/sys_likerat%03d.eps", intB);
      cout << endl << endl;
      c1->Print(line);
      if (g_gifOnly) continue;
      DVec xValues, areaVec;
      const int kSize = 3;
      const double kPercents[kSize] = {0.6828, 0.90, 0.95};
      for (int loop = 0; loop < kSize; ++loop)
      {
         double area = findFCinterval (kPercents[loop], r, xValues, areaVec);
         cout << "b(" << kPercents[loop] * 100 << "): " << intB 
              << "  "<< area << " (" << r << ") " 
              << xValues << " :: " << areaVec << endl;
         cout << "R: ";
         for (unsigned int loop = 0; loop < xValues.size(); ++loop)
         {
            double b = xValues[loop];
            cout << likeliRat(&b) << " ";
         }
      cout << endl;
      } // for loop
   } // for iter
} // findAllFCIntervals

void strings2vector (SVec &sList, DVec &retval, int offset)
{
   retval.clear();
   for (unsigned int loop = offset; loop < sList.size(); ++loop)
   {
      retval.push_back ( atof( sList[loop].c_str() ) );
   } // for loop
}

void loadfile (const char filename[])
{
   ifstream source(filename, ios::in);
   if (! source)
   {
      cerr << "file " << filename << " could not be opened" << endl;
      assert(0);
   }
   cerr << "Loading file " << filename << endl;
   string line;
   while (getline (source, line))
   {
      SVec sList;
      DVec values;
      split (sList, line);
      int size = sList.size();
      if (size < 1)
      {
         continue;
      }
      if ("gif_only" == sList[0])
      {
         g_gifOnly = true;
         continue;
      }
      if (size < 2)
      {
         continue;
      }
      // At least two items
      if ("best_measured" == sList[0])
      {
         if (debug) cout << "best_measured " << bBestVec << endl;
         strings2vector (sList, bBestVec);
         if (debug) cout << "after         " << bBestVec << endl;
         continue;
      }
      if ("Pmeas_best" == sList[0])
      {
         if (debug) cout << "Pmeas_best " << PbMeas_bBestVec << endl;
         strings2vector (sList, PbMeas_bBestVec);
         if (debug) cout << "after      " << PbMeas_bBestVec << endl;
         continue;
      }
      if ("peakLR" == sList[0])
      {
         if (debug) cout << "peakLR_bBestVec " << peakLR_bBestVec << endl;
         strings2vector (sList, peakLR_bBestVec);
         if (debug) cout << "after           " << peakLR_bBestVec << endl;
         continue;
      }
      if (size < 3)
      {
         continue;
      }
      if ("+" == sList[0])
      {
         int b = atoi (sList[1].c_str());
         DVec tempVector;
         strings2vector (sList, tempVector, 2);
         if (tempVector.size() == kGaussSize_old)
         {
            for (unsigned int loop = kGaussSize_old + 1; 
                 loop <= kGaussSize; ++loop)
            {
               tempVector.push_back( 0. );
            }
         }
         if (tempVector.size() == kGaussSize)
         {
            if (debug) cout << "b " << b << " : " <<  tempVector << endl;
            gaussMap[b] = tempVector;
         } else {
            cout << "Gauss size for b " << b << " : " 
                 << tempVector.size()
                 << " doesn't match : " << kGaussSize << endl;
         } // else if tempVector o.k.
      }
      // at least three items
   } // while getline
   // we need 100
   if (gaussMap[100].size())
   {
      b100_gaussVec = gaussMap[100];
   } else {
      cout << "No 100 loaded" << endl;
      assert(0);
   }
   // make sure all important vectors are filled
   assert (b100_gaussVec.size());
   assert (bBestVec.size());
   assert (PbMeas_bBestVec.size());
   assert (peakLR_bBestVec.size());
}

Double_t likeliRat(Double_t *x, Double_t *par)
{
   Double_t pb_b0 = PbGivenb0 (x[0]);
   Double_t pb_best = PbGivenbBest (x[0]);
   // if we have a negative X, make sure the likeliratio dies to 
   // 0 VERY quickly
   if (x[0] < 0)
   {
      double arg = -1 * x[0] * x[0] * 1e10;
      return TMath::Exp(arg) * likeliRat(&zero, par);
   } else if (pb_best > 0) {
      double retval = pb_b0 / pb_best;
      if ((x[0] <= 1) || (retval < 1))
      {
         return retval;
      } else {
         return 1;
      }
   } else {
      // Mathematically, this shouldn't happen
      // However, to the precision that we are keeping track
      // it can.  It just means that we are REALLY far out there
      return 0;
   }
}

Double_t PbGivenb0 (Double_t x, const DVec &params)
{
   return splitGauss (x, params);
}

Double_t PbGivenbBest (Double_t x)
{
   Double_t bBst = bBest(x);
   // since our 
   if (x > 1.01) 
   {
      bBst = 1.01;
   }
   if ( (bBst <= 0) || (x < -0.02) )
   {
      bBst = 0;
   }
   // are we in the physical region?
   if (bBst > 1)
   {
      // NOT physical.  Use p (b | b = 1.0)
      return splitGauss (x, b100_gaussVec);
   } else if (bBest < 0)
   {
      // NOT Physical
      // Since our current case is only looking at the upper half of 
      // the possible regions, we don't have to worry about this.
      // Nominally, we should be returning
      // P (b | b = 0.0)
      return 0;
   } else 
   {
      // Yep, just use the fit of p (b | b best)
      return polynomial (x, PbMeas_bBestVec);
   }
}

// Returns 
Double_t bBest(Double_t x)
{
   double retval = polynomial (x, bBestVec);
   
   return retval;
}

Double_t polynomial (Double_t x, const DVec &params)
{
   Double_t xToTheN = 1.;
   Double_t total = 0.;
   unsigned int size = params.size();
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      total += xToTheN * params[loop];
      xToTheN *= x;
   }
   return total;
}

void calculateIntegrated()
{
   double lower = -0.0025;
   double upper = 2.5;
   int bins = 1001;
   double stepsize = (upper - lower) / bins;
   double halfstep = stepsize / 2;
   cout << "global vector " << g_vec << endl;
   CLPHisto dist (bins + 1, lower - halfstep, upper + halfstep);
   likeliRatHist = CLPHisto (bins + 1, lower - halfstep, upper + halfstep);
   // let's fill this puppy
   for (int loop = 0; loop < bins; ++loop)
   {
      double x = lower + loop * stepsize;
      double weight;
      if (x >= 0) 
      {
         weight = splitGauss (x);
      } else {
         weight = 0;
      }
      //cout << loop << ") " << x << " " << weight << endl;
      if (weight < 0)
      {
         weight = 0;
      }
      double lr = likeliRat(&x);
      dist.fill (x, weight);
      likeliRatHist.fill (x, lr);
   } // for loop
   areaHist = dist.integrate();
   g_filled = true;
}

Double_t areaGivenLikeliRat (Double_t r, DVec &xValues, DVec &areaVec)
{
   areaVec.clear();
   likeliRatHist.find(r, xValues);
   unsigned int size = xValues.size();
   unsigned int halfSize = size / 2;
   // is size odd
   if ( halfSize * 2 != size)
   {
      // we have an odd number of matches.  That means
      // that we have an opening of an interval, but not
      // a closing.  Add the largest value on the end to 
      // close the last interval
      xValues.push_back (kBiggestB);
      size = xValues.size();
      halfSize = size / 2;
      assert (halfSize * 2 == size);
   }
   double area = 0.;
   for (unsigned int loop = 0; loop < halfSize; ++loop)
   {
      double x1 = xValues[ 2 * loop     ];      
      double x2 = xValues[ 2 * loop + 1 ];
      double thisArea = areaHist.interpolate(x2, true) 
         - areaHist.interpolate(x1, true);
      area += thisArea;
   }
   for (unsigned int loop = 0; loop < xValues.size() - 1; ++loop)
   {
      double x2 = xValues[ loop + 1];
      double x1 = xValues[ loop    ];
      areaVec.push_back( areaHist.interpolate(x2, true)
                         - areaHist.interpolate(x1, true) );
   }
   // one last error condition to check.  We could get no area because
   // r is too small
   if ((0. == area) && (r < 0.5))
   {
      area = 1.;
   }
   //cout << "r " << r << " area " << area << " " << xValues << endl;
   return area;
}

Double_t findFCinterval (Double_t size, double &r, DVec &xValues,
                         DVec &areaVec)
{
   if (! g_filled)
   {
      cout << "calling calulateIntegrated()" << endl;
      calculateIntegrated();
   }
   ///////////////////////////////////////////////////////////////
   //double likeliRatPeak = polynomial (g_b0, peakLR_bBestVec);
   //double upper1 = likeliRatPeak - 0.02; // larger X1
   //double upper2 = likeliRatPeak + 0.02; // smaller X2
   double rCurrent = 0.80;
   double rStep  = 0.01;
   double rMax   = 0.99;
   double rMin   = 0.01;
   bool problem = false;
   double area = areaGivenLikeliRat (rCurrent, xValues, areaVec);
   double areaDiff = area - size;
   while (fabs(areaDiff) > kMaxError)
   {
      if (areaDiff > 0)
      {
         // area is too big.  We are too far down
         rMin = rCurrent;
         if (rCurrent + 2 * rStep > rMax)
         {
            rStep = (rMax - rCurrent) / 4;
         }
         rCurrent += rStep;
      } else {
         // area is too small 
         rMax = rCurrent;
         if (rCurrent - 2 * rStep < rMin)
         {
            rStep = (rCurrent - rMin) / 4;
         }
         rCurrent -= rStep;
      }
      area = areaGivenLikeliRat (rCurrent, xValues, areaVec);
      areaDiff = area - size;
      if (rStep < kVerySmall)
      {
         cout << "rStep too small " << rStep << endl;
         problem = true;
         break;
      }
   }
   r = rCurrent;
   return area;
}

Double_t splitGauss(Double_t x, const DVec &params)
{
   double norm   = params[0];
   double arg    = x - params[1];
   double sigma1 = params[2]; // negative sigma1
   double frac   = params[4]; // negative fraction
   double sigma2 = sigma1 * params[6]; // negative sigma2
   double zConst = params[8];
   double zWidth = params[9];
   if (arg > 0)
   {
      sigma1 = params[2 + 1]; // positive sigma1
      frac   = params[4 + 1]; // positive fraction
      sigma2 = sigma1 * params[6 + 1]; // positive sigma2
   }
   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1)) 
      + norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2))
      + zConst * TMath::Exp( -0.5 * x * x / (zWidth * zWidth));
   return fitval;
}
