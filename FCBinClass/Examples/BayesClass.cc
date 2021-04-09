// -*- C++ -*-
// C++ standard include files
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <vector>
// Root include files
#include "TH1F.h"
#include "TMath.h"
#include "TMinuit.h"
#include "TCanvas.h"
#include "TF1.h"
// User include files
#include "BayesClass.h"

using namespace std;

// static data members;
double BayesClass::sm_back = 4.0;
double BayesClass::sm_backErr = 0.3;
int    BayesClass::sm_numObsv = 9;
double BayesClass::sm_intLimit = 10.;
double BayesClass::sm_stepSize = 0.1;
double BayesClass::sm_confid = 0.95;
bool   BayesClass::sm_fast = true;
double BayesClass::sm_sigma = 10.;
double BayesClass::sm_acclum = 1;
double BayesClass::sm_acclumErr = 1;
double BayesClass::sm_back2 = 4.0;
double BayesClass::sm_backErr2 = 0.3;
double BayesClass::sm_acclum2 = 1;
double BayesClass::sm_acclumErr2 = 1;
int    BayesClass::sm_numObsv2 = 1;
int    BayesClass::sm_numDim = 1;

// Constructors
BayesClass::BayesClass() 
{
}

void BayesClass::find_bayes_limit() 
{

  char   formula[200];
  double total;
  double value;
  double integral = 0.;
  
  if (sm_fast) {  
  
    sprintf (formula,"TMath::Poisson(%d, (x*%f) + %f)", 
             sm_numObsv, sm_acclum, sm_back);
    //printf("%s\n",formula);
    TF1* poisson = new TF1 ("poisson",formula,0,sm_intLimit);
    poisson->Draw();    
    total = poisson->Integral(0,sm_intLimit);  
    //printf("limit %f, total %f\n",sm_intLimit,total);
    setSigma (sm_intLimit); 
    
    while ( integral < 1 - sm_confid && total > 0) {
        value = poisson->Integral( sm_sigma, sm_intLimit );
        integral = value / total;
        printf("Sigma %f, integral %f\n",sm_sigma,integral);
        setSigma (sm_sigma - sm_stepSize);
    }
    setSigma (sm_sigma + 2*sm_stepSize);
    printf("< %f pb\n", sm_sigma);
  } else {
  
    // Make poisson distribution
    double nBins = sm_intLimit / sm_stepSize;
    TH1F* poisson = new TH1F("poisson","P(n_obsv|n_exp)",
                             nBins,-sm_stepSize/2,sm_intLimit-(sm_stepSize/2));
    double y_value;
    for (float i=0; i<sm_intLimit; i+=sm_stepSize) {
      y_value = find_max_P_obsv( i, sm_numDim );
      //printf(">>>>>>>>>>>>>> Results from Minuit: %f, %f <<<<<<<<<<<<<<<\n",
      //       i, y_value);
      poisson->Fill (i, y_value);
    }
    TCanvas* c1 = new TCanvas;
    poisson->Draw();

    // Find limit
    total = poisson->Integral (1, nBins);
    
    int s_bin;
    for (int i=nBins-1; i>0; --i) {
      value = poisson->Integral (i, nBins);
      integral = value / total;
      //printf("Sigma %f, integral %f\n",i / nBins * sm_intLimit, integral);
      
      s_bin = i;
      if ( integral > 1 - sm_confid ) break;
    }
    
    setSigma ( (s_bin+1) / nBins * sm_intLimit );
    printf(" < %f pb\n", sm_sigma);
    
    
  }   

}

double BayesClass::find_max_P_obsv( double sigma, int numDim ) 
{
  setSigma (sigma);
  setNumDim(numDim);

  TMinuit *ptrMinuit = new TMinuit(numDim*2);
  ptrMinuit->SetFCN(_fcn);
  ptrMinuit->SetPrintLevel(-1);
  double arglist[10];
  int ierflg = 0;
  
  double acc_shift, back_shift, parErr;
  double acc_shift2, back_shift2;  
  double acc_low_limit  = 0;
  double back_low_limit = 0;
  double acc_low_limit2  = 0;
  double back_low_limit2 = 0;  
  if (sm_acclumErr) acc_low_limit  = -sm_acclum / sm_acclumErr;
  if (sm_backErr)   back_low_limit = -sm_back   / sm_backErr;
  if (2 == numDim)
  {
    if (sm_acclumErr2) acc_low_limit2  = -sm_acclum2 / sm_acclumErr2;
    if (sm_backErr2)   back_low_limit2 = -sm_back2   / sm_backErr2;    
  }
  
  //printf("A -limit %f, B-limit %f\n",acc_low_limit,back_low_limit);
  ptrMinuit->mncler();
  ptrMinuit->mnparm (0, "acc_shift",  0, 0.1, acc_low_limit,  10, ierflg);
  ptrMinuit->mnparm (1, "back_shift", 0, 0.1, back_low_limit, 10, ierflg);
  if (2 == numDim)
  {
    ptrMinuit->mnparm (2, "acc_shift2",  0, 0.1, acc_low_limit2,  10, ierflg);
    ptrMinuit->mnparm (3, "back_shift2", 0, 0.1, back_low_limit2, 10, ierflg);    
  }
  ptrMinuit->mnexcm ("SIM", arglist, 0, ierflg);
  ptrMinuit->mnexcm ("MIG", arglist, 0, ierflg);
  ptrMinuit->GetParameter (0, acc_shift,  parErr);
  ptrMinuit->GetParameter (1, back_shift, parErr);
  double acclum = sm_acclum + sm_acclumErr * acc_shift;
  double back   = sm_back + sm_backErr * back_shift;
  
  //printf("sigma %f,acc_shift %f, back_shift %f, Poisson %f\n",
  //        sm_sigma,acc_shift,back_shift,
  //TMath::Poisson(sm_numObsv, (sm_sigma * accept * sm_lumi) + back));
  
  if (1 == numDim)
  {
  return TMath::Poisson(sm_numObsv, (sm_sigma * acclum) + back) *
          exp ( - pow(acc_shift, 2) / 2 ) * exp ( - pow(back_shift, 2) / 2 );
  }  
  if (2 == numDim)
  {
    ptrMinuit->GetParameter(2, acc_shift2,  parErr);
    ptrMinuit->GetParameter(3, back_shift2, parErr);
    double acclum2 = sm_acclum2 + sm_acclumErr2 * acc_shift2;
    double back2   = sm_back2 + sm_backErr2 * back_shift2;  
    return (TMath::Poisson(sm_numObsv, (sm_sigma * acclum) + back) *
            exp ( - pow(acc_shift, 2) / 2 ) * exp ( - pow(back_shift, 2) / 2 ) *
	   TMath::Poisson(sm_numObsv2, (sm_sigma * acclum2) + back2) *
	    exp ( - pow(acc_shift2, 2) / 2 ) * exp ( - pow(back_shift2, 2) / 2));
  }
  
  printf("Error, %d dimensions not allowed !!!\n",numDim);
  return -1;
}

void BayesClass::_fcn(int &npar, double* grad, double &chisq, double *par,
                      int iflag) {

  chisq = 0;
  
  double accShift = par[0];
  double backShift = par[1];
  double acclum = sm_acclum + sm_acclumErr * accShift;
  double back = sm_back + sm_backErr * backShift;
  // Acceptance and background must be positive
  if (acclum < 0 || back < 0) chisq = 999;
  else if (1 == sm_numDim) 
  {
    chisq = -1*TMath::Poisson(sm_numObsv, sm_sigma * 
            acclum + back) *
            exp ( - pow(accShift, 2) / 2) * exp ( - pow(backShift, 2) / 2);
  }
  else if (2 == sm_numDim) {
    double accShift2 = par[2];
    double backShift2 = par[3];
    double acclum2 = sm_acclum2 + sm_acclumErr2 * accShift2;
    double back2 = sm_back2 + sm_backErr2 * backShift2; 
    
    if (acclum2 < 0 || back2 < 0) chisq = 999;
    else
    { 
      chisq = -1*TMath::Poisson(sm_numObsv, sm_sigma * 
              acclum + back) *
              exp ( - pow(accShift, 2) / 2) * exp ( - pow(backShift, 2) / 2) *
	      TMath::Poisson(sm_numObsv2, sm_sigma * 
              acclum2 + back2) *
              exp ( - pow(accShift2, 2) / 2) * exp ( - pow(backShift2, 2) / 2);  
    }
  }
  else chisq = 999;
}

