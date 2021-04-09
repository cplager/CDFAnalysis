// -*- C++ -*-
// Standard include files
#include <math.h>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
// CLP include files
#include "XsecComb.h"
#include "CLPUtils.h"
#include "dout.h"
// Root include files
#include "TSystem.h"
#include "TRandom.h"

#include "CLPUtils.h"

using namespace std;

int main(int argc, char** argv)
{
   double true_xsec = 6.7;
   int trials = 1000;
   if (argc >= 2)
   {
      double temp = atof( argv[1] );
      if (temp > 0)
      {
         true_xsec = temp;
      }
   }
   if (argc >= 3)
   {
      int temp = atoi( argv[2] );
      if (temp > 0)
      {
         trials = temp;
      }      
   }   
   double accNN = 0.0665;
   double accSV = 0.03032; 
   double backNN_xsec = 428.0 / 200.;
   double backSV_xsec =  13.8 / 200.;
   double lumi = 200.;
   // the background of the SecVtx is defined to be a subset of the
   // NN analysis
   double backSV_exp = backSV_xsec * lumi;
   double backNN_exp = backNN_xsec * lumi;
   double backExtra_exp = backNN_exp - backSV_exp;
   DVec soSVvec, sbSVvec, soNNvec, sbNNvec;
   DVec backSVvec, backNNvec, backSV2vec, backNN2vec;
   DVec bxSVvec, bxNNvec;
   for (int loop = 0; loop < trials; ++loop)
   {
      //  // Don't do this for checking correlations
      //  double true_xsec = gRandom->Gaus(7., 1.);
      // the signal of the SecVtx is defined to be a subset of the
      // NN analysis
      double sigSV_exp = true_xsec * accSV * lumi;
      double sigNN_exp = true_xsec * accNN * lumi;
      double sigExtra_exp = sigNN_exp - sigSV_exp;
      int sigSV = gRandom->Poisson(sigSV_exp);
      int sigNN = gRandom->Poisson(sigExtra_exp) + sigSV;
      int backSV = gRandom->Poisson(backSV_exp);
      int backNN = gRandom->Poisson(backExtra_exp) + backSV;      
      int totalSV = sigSV + backSV;
      int totalNN = sigNN + backNN;
      // signal only SecVtx cross section
      double xsecSVso = (sigSV) / (accSV * lumi);
      soSVvec.push_back( xsecSVso );
      // signal only Neural net cross section
      double xsecNNso = (sigNN) / (accNN * lumi);
      soNNvec.push_back( xsecNNso );
      // signal + background SecVtx cross section
      double xsecSVsb = (totalSV - backSV_exp) / (accSV * lumi);
      sbSVvec.push_back( xsecSVsb );
      // signal + background Neural net cross section
      double xsecNNsb = (totalNN - backNN_exp) / (accNN * lumi);
      sbNNvec.push_back( xsecNNsb );
      // backgrounds only
      double svSmear = gRandom->Gaus (1., .143);
      double nnSmear = gRandom->Gaus (1., .044);
      int backSV2 = gRandom->Poisson(backSV_exp * svSmear);
      int backNN2 = gRandom->Poisson(backExtra_exp * nnSmear) + backSV2;      
      backSVvec.push_back( backSV );
      backNNvec.push_back( backNN );
      backSV2vec.push_back( backSV2 );
      backNN2vec.push_back( backNN2 );
      bxSVvec.push_back( (backSV2 - backSV_exp) / (accSV * lumi)  );
      bxNNvec.push_back( (backNN2 - backNN_exp) / (accNN * lumi)  );
   } // for loop
   cout << "correlation for " << trials << "  xsec: " << true_xsec << endl
        << "signal only           " 
        << correlationCoeff (soSVvec, soNNvec) << endl
        << "signal + background   " 
        << correlationCoeff (sbSVvec, sbNNvec) << endl
        << "sb SV + so NN         " 
        << correlationCoeff (sbSVvec, soNNvec) << endl
        << "background: SV vs NN  "
        << correlationCoeff (backSVvec, backNNvec) << endl
        << "background: SV vs NN  "
        << correlationCoeff (backSV2vec, backNN2vec) 
        << " : with systematic error" << endl
        << "backerr: SV vs NN     "
        << correlationCoeff (bxSVvec, bxNNvec) << endl;
}
