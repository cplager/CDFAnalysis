// -*- C++ -*-
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TPolyLine.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h> // for sprintf
#include <vector>
#include <assert.h>

using namespace std;

typedef vector< double > DVec;

// constants
const double kB0min = 0.4;
const double kB0max = 1.0;
const double kBmeasMin = 0;
const double kBmeasMax = 2.3;
const int kNumSteps = 60;
const double kStepSize = (kB0max - kB0min) / kNumSteps;

// (gulp) Global variables
double fc68_1[8] = {-3.48567e+00,  3.80316e-01, -1.72619e-02,  4.41700e-04, 
                    -6.75026e-06,  6.19012e-08, -3.16941e-10,  7.02287e-13};
DVec fc68_1vec(fc68_1, fc68_1 + 8);

double fc68_2[8] = {-1.32282e+02,  1.49107e+01, -7.08823e-01,  1.84765e-02, 
                    -2.85081e-04,  2.60479e-06, -1.30569e-08,  2.77162e-11};
DVec fc68_2vec(fc68_2, fc68_2 + 8);

double fc95_1[8] = {-8.40689e+00,  1.04503e+00, -5.44648e-02,  1.54831e-03, 
                    -2.57464e-05,  2.50435e-07, -1.31933e-09,  2.90539e-12};
DVec fc95_1vec(fc95_1, fc95_1 + 8);

double fc95_2[10] = {-5.50797e+00,  2.92351e-01, -2.90537e-03, -3.14604e-05, 
                      2.33888e-07,  5.71804e-09,  2.05776e-11, -6.02203e-13, 
                     -8.69851e-15,  8.63572e-17};
DVec fc95_2vec(fc95_2, fc95_2 + 10);

Double_t bBestArray[6] = { 3.48416e-01, -2.14897e+00,  1.17845e+01, 
                          -2.11178e+01,  1.83049e+01, -6.16090e+00};
DVec bBestvec(bBestArray, bBestArray + 6);

Double_t polynomial (Double_t x, const DVec &params);
void fcbands();
Double_t bBestFunc (Double_t *x, Double_t *par);

Double_t bBestFunc (Double_t *x, Double_t *par)
{
   double retval = polynomial (*x, bBestvec);
   cout << "x " << *x << " rv " << retval << endl;
   return retval;
}


void fcbands()
{
   TCanvas *c1 = new TCanvas;
   TH2F *h2 = new TH2F ("h2", "Feldman Cousins Bands for R", 
                        2, kBmeasMin, kBmeasMax, 2, kB0min, kB0max);
   h2->Draw();
   double x[2 * kNumSteps + 3], y[2 * kNumSteps + 3];
   // go up fc95_1 first
   for (int loop = 0; loop <= kNumSteps; ++loop)
   {
      double b0 = kB0min + loop * kStepSize;
      double bmeas = polynomial(b0 * 100, fc95_1vec);
      if (bmeas > kBmeasMax)
      {
         bmeas = kBmeasMax;
      } else if (bmeas < kBmeasMin)
      {
         bmeas = kBmeasMin;
      }
      x[loop] = bmeas;
      y[loop] = b0;
   }// for loop 0..kNumSteps
   
   // now down fc95_2
   for (int loop = 0; loop <= kNumSteps; ++loop)
   {
      double b0 = kB0max - loop * kStepSize;
      double bmeas = polynomial(b0 * 100, fc95_2vec);
      if (bmeas > kBmeasMax)
      {
         bmeas = kBmeasMax;
      } else if (bmeas < kBmeasMin)
      {
         bmeas = kBmeasMin;
      }
      x[loop + kNumSteps + 1] = bmeas;
      y[loop + kNumSteps + 1] = b0;
   } // for loop 0..kNumSteps
   // close the loop
   y[2 * kNumSteps + 2] = y[0];
   x[2 * kNumSteps + 2] = x[0];
   TPolyLine *fc95Line = new TPolyLine(2 * kNumSteps + 3, x, y);
   fc95Line->SetFillColor(2);
   fc95Line->SetFillStyle(3004); // page 151 of 4_08 Users Guide
   fc95Line->SetLineColor(2);
   fc95Line->SetLineWidth(0);
   fc95Line->Draw("f");
   fc95Line->Draw();
   
   //double x[2 * kNumSteps + 3], y[2 * kNumSteps + 3];
   // go up fc68_1 first
   for (int loop = 0; loop <= kNumSteps; ++loop)
   {
      double b0 = kB0min + loop * kStepSize;
      double bmeas = polynomial(b0 * 100, fc68_1vec);
      if (bmeas > kBmeasMax)
      {
         bmeas = kBmeasMax;
      } else if (bmeas < kBmeasMin)
      {
         bmeas = kBmeasMin;
      }
      x[loop] = bmeas;
      y[loop] = b0;
   }// for loop 0..kNumSteps
   
   // now down fc68_2
   for (int loop = 0; loop <= kNumSteps; ++loop)
   {
      double b0 = kB0max - loop * kStepSize;
      double bmeas = polynomial(b0 * 100, fc68_2vec);
      if (bmeas > kBmeasMax)
      {
         bmeas = kBmeasMax;
      } else if (bmeas < kBmeasMin)
      {
         bmeas = kBmeasMin;
      }
      x[loop + kNumSteps + 1] = bmeas;
      y[loop + kNumSteps + 1] = b0;
   } // for loop 0..kNumSteps
   // close the loop
   y[2 * kNumSteps + 2] = y[0];
   x[2 * kNumSteps + 2] = x[0];
   TPolyLine *fc68Line = new TPolyLine(2 * kNumSteps + 3, x, y);
   fc68Line->SetFillColor(3);
   fc68Line->SetFillStyle(3005); // page 151 of 4_08 Users Guide
   fc68Line->SetLineColor(3);
   fc68Line->SetLineWidth(0);
   fc68Line->Draw("f");
   fc68Line->Draw();
   TF1 *bBest = new TF1 ("bBest", bBestFunc, kBmeasMin, 1.02, 0);
   bBest->Draw("same");
   c1->Print("FC.eps");
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

