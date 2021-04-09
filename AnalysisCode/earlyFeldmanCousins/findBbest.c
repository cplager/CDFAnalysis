// -*- C++ -*-
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMinuit.h"
#include "CLPHisto.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdio.h> // for sprintf
#include <vector>
#include <assert.h>

using namespace std;

// typedefs
typedef vector< Double_t > DVec;

// global variables
bool initialized = false;

double limitsArray[3] = {0.32, 0.60, 1.50}; // 1.5 instead of 1.0 for room
DVec limits (limitsArray, limitsArray + 3);
DVecVec p0vecvec, p1vecvec, p2vecvec, p3vecvec;

// p0
Double_t p0_32[10] = {-2.38412e+02, 1.18314e+05, -1.20558e+06, 6.30470e+06, -1.96745e+07, 3.86334e+07, -4.81619e+07, 3.70878e+07, -1.61853e+07, 1.90568e+06};

Double_t p0_60[10] = {3.47517e+04, -2.02332e+05, -3.33914e+05, 6.33351e+06, -2.23170e+07, 3.76207e+07, -4.00252e+07, 4.44166e+07, -4.80486e+07, 2.37741e+07};

Double_t p0_100[10] = {2.66070e+03, 1.08959e+05, -1.20201e+06, 6.31486e+06, -1.96740e+07, 3.86173e+07, -4.81877e+07, 3.70779e+07, -1.60417e+07, 2.98598e+06};

// p1
Double_t p1_32[10] = {8.70303e+04, -1.45445e+06, 1.06722e+07, -4.48314e+07, 1.18783e+08, -2.05796e+08, 2.32163e+08, -1.66304e+08, 6.88750e+07, 8.14759e+06};

Double_t p1_60[10] = {4.27964e+03, -7.43481e+05, 9.21309e+06, -4.74950e+07, 1.28495e+08, -1.99887e+08, 2.08000e+08, -2.08812e+08, 1.99823e+08, -9.33542e+07};

Double_t p1_100[10] = {2.25973e+05, -2.18238e+06, 1.17713e+07, -4.46766e+07, 1.17543e+08, -2.06120e+08, 2.33964e+08, -1.64695e+08, 6.53472e+07, -1.11780e+07};

// p2
Double_t p2_32[10] = {-3.18591e+06, 4.40640e+07, -2.74222e+08, 9.61916e+08, -2.07665e+09, 2.84559e+09, -2.44858e+09, 1.26818e+09, -3.46510e+08, -1.06811e+08};

Double_t p2_60[10] = {-5.98138e+06, 6.91975e+07, -3.40902e+08, 9.57085e+08, -1.87632e+09, 2.92860e+09, -3.05874e+09, 6.91491e+08, 2.03376e+09, -1.48500e+09};

Double_t p2_100[10] = {-3.37899e+06, 4.47037e+07, -2.74461e+08, 9.61271e+08, -2.07682e+09, 2.84541e+09, -2.45112e+09, 1.26151e+09, -3.41095e+08, 3.39544e+07};

// p3
Double_t p3_32[10] = {5.71292e+07, -9.36339e+08, 6.83503e+09, -2.87969e+10, 7.71963e+10, -1.36235e+11, 1.56005e+11, -1.15618e+11, 4.99191e+10, 3.13454e+10};

Double_t p3_60[10] = {-7.21084e+07, 2.79416e+08, 3.41077e+09, -2.86869e+10, 8.76151e+10, -1.32826e+11, 1.24265e+11, -1.40973e+11, 1.74722e+11, -9.31994e+10};

Double_t p3_100[10] = {4.68366e+07, -9.03856e+08, 6.81684e+09, -2.88428e+10, 7.72222e+10, -1.35952e+11, 1.56887e+11, -1.14101e+11, 4.73883e+10, -8.56172e+09};

// from bMax to b0 -> b0 (bMax)
double meas2gen[6] = {-4.09870e-02,  1.87481e+00, -3.68378e+00,  
                         7.26694e+00, -6.77391e+00,  2.40776e+00};
DVec meas2genVec(meas2gen, meas2gen + 6);
 
// from b0 to bMax -> bMax (b0)
double gen2meas[6] ={4.21282e-02,  2.30502e-01,  2.88533e+00, 
                        -5.17765e+00,  4.46259e+00, -1.48498e+00};
DVec gen2measVec(gen2meas, gen2meas + 6);

double g_bMeas;

// functions
Double_t polynomial (Double_t x, const DVec &params);
void findPbBest (double bmeas, double &bBest, double &PbBest);
Double_t PbMeasGivenB (double bmeas, double bgen);

Double_t P0(double bgen);
Double_t P1(double bgen);
Double_t P2(double bgen);
Double_t P3(double bgen);

void initialize()
{
   // p0
   p0vecvec.push_back( DVec (p0_32,  p0_32  + 10) );
   p0vecvec.push_back( DVec (p0_60,  p0_60  + 10) );
   p0vecvec.push_back( DVec (p0_100, p0_100 + 10) );
   // p1
   p1vecvec.push_back( DVec (p1_32,  p1_32  + 10) );
   p1vecvec.push_back( DVec (p1_60,  p1_60  + 10) );
   p1vecvec.push_back( DVec (p1_100, p1_100 + 10) );
   // p2
   p2vecvec.push_back( DVec (p2_32,  p2_32  + 10) );
   p2vecvec.push_back( DVec (p2_60,  p2_60  + 10) );
   p2vecvec.push_back( DVec (p2_100, p2_100 + 10) );
   // p3
   p3vecvec.push_back( DVec (p3_32,  p3_32  + 10) );
   p3vecvec.push_back( DVec (p3_60,  p3_60  + 10) );
   p3vecvec.push_back( DVec (p3_100, p3_100 + 10) );
}

Double_t P0(double bgen)
{
   unsigned which;
   for (which = 0; which < p0vecvec.size(); ++which)
   {
      if (bgen < limits[which])
      {
         break;
      }
   }
   return polynomial (bgen, p0vecvec[which]);
}

Double_t P1(double bgen)
{
   unsigned which;
   for (which = 0; which < p1vecvec.size(); ++which)
   {
      if (bgen < limits[which])
      {
         break;
      }
   }
   return polynomial (bgen, p1vecvec[which]);
}

Double_t P2(double bgen)
{
   unsigned which;
   for (which = 0; which < p2vecvec.size(); ++which)
   {
      if (bgen < limits[which])
      {
         break;
      }
   }
   return polynomial (bgen, p2vecvec[which]);
}

Double_t P3(double bgen)
{
   unsigned which;
   for (which = 0; which < p3vecvec.size(); ++which)
   {
      if (bgen < limits[which])
      {
         break;
      }
   }
   return polynomial (bgen, p3vecvec[which]);
}

Double_t PbMeasGivenB (double bmeas, double bgen)
{
   double bgenPeak = polynomial (bgen, gen2measVec);
   double arg = bmeas - bgenPeak;
   DVec poly;
   // P0
   poly.push_back( P0(bgen) );
   // P1           
   poly.push_back( P1(bgen) );
   // P2           
   poly.push_back( P2(bgen) );
   // P3           
   poly.push_back( P3(bgen) );
   // now, just calculate the final probability
   return polynomial (arg, poly);
}

void fcn(Int_t &npar, Double_t *gin, Double_t &retval, 
         Double_t *par, Int_t iflag)
{
   double bBest = par[0];
   retval = -1 * PbMeasGivenB (g_bMeas, bBest);
}

void findPbBest (double bMeas, double &bBest, double &PbBest)
{
   if (! initialized)
   {
      initialize();
   }
   g_bMeas = bMeas;
   double bGood = polynomial (bMeas, meas2genVec);
   // cout << "Measured " << bMeas << " : good " << bGood * 100 << endl;
   // // scan around from -0.04 to 0.04 around bGood
   // for (double loop = -0.04; loop <= 0.04; loop += 0.005)
   // {
   //    double prob = PbMeasGivenB (bMeas, bGood + loop);
   //    cout << (bGood + loop) * 100 << "  " << prob << endl;
   // } // for loop
   //initialize TMinuit with a maximum of 1 params
   // whether there was an error or not
   Int_t ierflg = 0;
   double arglist[10];
   TMinuit *gMinuit = new TMinuit(1); 
   arglist[0] = -1;
   gMinuit->mnexcm("SET PRINT",arglist,1,ierflg);
   gMinuit->SetFCN(fcn);
   // Set starting values and step sizes for parameters
   // parameter #, name, starting value, step size
   // starting value, ending value, error flag (returned)
   gMinuit->mnparm(0, "b0", bGood, 0.0001, bGood - 0.05, bGood + 0.05, ierflg);
   // Now ready for minimization step
   // Start with simplex
   gMinuit->mnexcm("SIM", arglist, 0, ierflg);
   // Now lets try Migrad
   gMinuit->mnexcm("MIG", arglist, 0, ierflg);
   double err;
   gMinuit->GetParameter(0, bBest, err);
   //cout << "bBest " << bBest << " P " << PbMeasGivenB(bMeas, bBest) << endl;
   cout << bMeas << " " << bBest << " " << PbMeasGivenB(bMeas, bBest) << endl;
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

