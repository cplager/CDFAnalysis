#include <iostream>
#include "TMinuit.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TFile.h"


using namespace std;

const int Nfitted = 5;

static int Nmax = 5;

TH1F *fit_hmc[Nfitted];
TH1F *fit_hdata;

// Set starting values, step sizes and lower/upper limits for parameters
static Double_t vstart[Nfitted];
static Double_t step[Nfitted]; 
static Double_t minpar[Nfitted];
static Double_t maxpar[Nfitted];

void SetMax(int Nset);
void SetHisto(TH1F *hdata, TH1F *hmc[Nfitted]);
void SetConstraints(Double_t *minV = 0, Double_t *maxV = 0, Double_t *startV = 0, Double_t *stepV = 0);
void PerformFit(double *param, double *errors);
void Likelihood(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);


void SetMax(int Nset){
  Nmax = Nset;
}


void SetHisto(TH1F *hdata, TH1F *hmc[Nfitted]){

  fit_hdata = (TH1F*) hdata->Clone();
  for (int i=0; i < Nmax; i++)
    fit_hmc[i] = (TH1F*) hmc[i]->Clone();

}

void SetConstraints(Double_t *minV, Double_t *maxV, Double_t *startV, Double_t *stepV){

  for (int i=0; i < Nmax; i++){

    if (startV)
      vstart[i] = *(startV+i);
    else 
      vstart[i] = 1.0;

    if (stepV)
      step[i] = *(stepV+i);
    else 
      step[i] = 0.001;

    if (minV)
      minpar[i] = *(minV+i);
    else 
      minpar[i] = 0.0;

    if (maxV)
      maxpar[i] = *(maxV+i);
    else 
      maxpar[i] = 5.0;

  }  

}//SetConstraints


void PerformFit(double *param, double *errors) {

  double fit_param[Nfitted];
  double fit_err[Nfitted];
  //setup fitter
  TMinuit *mnp = new TMinuit(Nmax);//initialize TMinuit with a maximum of 5 params
   mnp->SetFCN(Likelihood);

   Double_t arglist[20];
   Int_t ierflag = 0;

   // Defining parameter errors (UP)
   //1: default value (normal for chisquared)
   //0.5 for negative log likelihood
    arglist[0] = 1; // we don't set 0.5 since the likelihood is already mutiplied by a factor of 2 in FCN
    mnp->mnexcm("SET ERR", arglist ,1,ierflag);

    mnp->SetPrintLevel(-1);
    //set Minuit print level
    // printlevel = -1  quiet (also suppresse all warnings)
    //            =  0  normal
    //            =  1  verbose
    //    arglist[0] = -1;
    //setting print out
    //-1: no output except from SHOW commands 
    //0: minimum output (no starting values or intermediate results) 
     //1: default value, normal output 
     //2: additional output giving intermediate results. 
     //3: maximum output, showing progress of minimizations.
    // mnp->mnexcm("SET PRINT", arglist, 1, ierflag);
    //Supresses Minuit warning messages
    //      mnp->mnexcm("SET NOW",arglist,1,ierflag);

      arglist[0] = 2; 
      //setting strategy
      //0: economize function calls
      //2: waste function calls in order to be sure that all values are precise
      mnp->mnexcm("SET STRATEGY", arglist, 1, ierflag);	
      
      //Supresses Minuit warning messages
      mnp->mnexcm("SET NOW",arglist,1,ierflag);
  
    //The inverse of SET GRAdient, 
      //instructs Minuit not to use the first derivatives calculated by the user in FCN.
      mnp->mnexcm("SET NOG",arglist,1,ierflag);

 

      for (int i=0; i < Nmax; i++)
	mnp->mnparm(i,fit_hmc[i]->GetName(), vstart[i], step[i], minpar[i],maxpar[i],ierflag);
      
      //Mimization (Max Steps and Tolerance (0.001*Tolerance*error(0.5));
      arglist[0] = 500;
      arglist[1] = 1.;
      //Perform a function minimization using the simplex method of Nelder and Mead. 
      //Minimization terminates either when the function has been called (approximately)
      //arglist[0]=500 times, or when the estimated vertical distance to minimum (EDM) 
      //is less than tolerance: arglist[1]=1. 
      //The default value of [tolerance] is 0.1*UP (see SET ERR).
      mnp->mnexcm("SIMPLEX", arglist ,2,ierflag);
      //Causes minimization of the function by the method of Migrad, 
      //the most efficient and complete single method, recommended for general functions 
      mnp->mnexcm("MIGRAD", arglist ,2,ierflag);  
      // Do the MINOS Errors
      mnp->mnexcm("MINOS", arglist, 2, ierflag);
      cout << "Number of function calls in MINUIT: " << mnp->fNfcn << endl;

      // Print results
      Double_t amin,edm,errdef;
      Int_t nvpar,nparx,icstat;
      mnp->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
      mnp->mnprin(4,amin);    
//       cout << "amin = " << amin << endl;
   
      for (int i=0; i < Nmax; i++){
	mnp->GetParameter(i,fit_param[i],fit_err[i]);
	cout << fit_hmc[i]->GetName() << " = " << fit_param[i] << " +/- " << fit_err[i] << endl;
	*(param+i) = fit_param[i];
	*(errors+i) = fit_err[i];
      }

      //   param = fit_param;
      // MINOS ERRORS:
      //   mnp->mnerrs(i,errorSigFrac[0],errorSigFrac[1],eparb,gcc);

      // plot
      //     fit_hdata->Draw();


}//Perform Fit




void Likelihood(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
// npar - number of currently variable parameters
// gin - optional vector of first derivatives
// f  - calculated function value
// par - vector of constant and variable parameters
// iflag - indicated what is to be calculated

//calculate Likelihood 
  f = 0;

// Loop over the bins and determine the contribution to from each bin.
  int NBINS = fit_hdata->GetNbinsX();
  for(int bin=1; bin<=NBINS; bin++) {

     double mu = 0; // predicted number of events in this bin
     for (int i=0; i<Nmax; i++)
       mu += par[i]*fit_hmc[i]->GetBinContent(bin);

     if (mu < 1.e-9) mu = 1.e-9;

     int n = fit_hdata->GetBinContent(bin); // observed number of events in this bin
      
    // Likelihood contribution from this bin	
     f -= (n*TMath::Log(mu) - mu);
   }// loop over bins

   f *= 2;
   
}//LikelihoodFCN

double DrawLikelihood(double *param, TH2F *hL, int Nm, int x, int y){ //, double *xmin, double *ymin){
// parse parameters and specify which parameters used in likelihood drawing

  double f_min = 0;
//   double x_min = 0;
//   double y_min = 0;

  // Loop over bins of 2D_L histogram
  int NbinsX = hL->GetNbinsX();
  int NbinsY = hL->GetNbinsY();

 TAxis *xaxis = hL->GetXaxis();
 float Xmin = xaxis->GetXmin();
 float Xmax = xaxis->GetXmax();

 TAxis *yaxis = hL->GetYaxis();
 float Ymin = yaxis->GetXmin();
 float Ymax = yaxis->GetXmax();

  for (int binX=1; binX <=NbinsX; binX++){
    for (int binY=1; binY <=NbinsY; binY++){
      
      double f =0;

      float value_x  = Xmin + (Xmax - Xmin) / NbinsX * binX;
      float value_y  = Ymin + (Ymax - Ymin) / NbinsY * binY;

    
      // Loop over the bins and determine the contribution to from each bin.
      int NBINS = fit_hdata->GetNbinsX();
      for(int bin=1; bin<=NBINS; bin++) {
	
	double mu = 0; // predicted number of events in this bin
	for (int i=0; i<Nm; i++){
	  float param_mc = param[i];
 	  if (i==x) param_mc = value_x;
 	  if (i==y) param_mc = value_y;
	  mu += param_mc*fit_hmc[i]->GetBinContent(bin);

	  //	  cout << "fit_hmc[" << i << "](" << bin << ") = " << fit_hmc[i]->GetBinContent(bin) << endl;
	}

	if (mu < 1.e-9) mu = 1.e-9;
	
	int n = fit_hdata->GetBinContent(bin); // observed number of events in this bin
     
	// Likelihood contribution from this bin	
	f -= (n*TMath::Log(mu) - mu);
	//	cout << "n = " << n << "; mu = " << mu << "; f = " << f << endl;
      }// loop over bins
      
      f *= 2;

 //      if ( f_min < f) {
// 	f_min = f;
// 	x_min = value_x;
// 	y_min = value_y;
//       }
      f_min = min(f_min, f);
      //    cout << "f(" << value_x  << ", " << value_y << ") = " << f << endl;
      hL->SetBinContent(binX, binY, f);

    }//binY
  }//binX

  return f_min;

}


// void DrawLikelihood(double *param, TH2F *hL, int Nm, int x, int y, float *x_array, float *y_array, float *res_array){
// // parse parameters and specify which parameters used in likelihood drawing

//   int arr_ind = 0;
//   float max_x = 0;
//   float max_y = 0;
//   float res_max = 0;

//   // Loop over bins of 2D_L histogram
//   int NbinsX = hL->GetNbinsX();
//   int NbinsY = hL->GetNbinsY();

//  TAxis *xaxis = hL->GetXaxis();
//  float Xmin = xaxis->GetXmin();
//  float Xmax = xaxis->GetXmax();

//  TAxis *yaxis = hL->GetYaxis();
//  float Ymin = yaxis->GetXmin();
//  float Ymax = yaxis->GetXmax();

//   for (int binX=1; binX <=NbinsX; binX++){
//     for (int binY=1; binY <=NbinsY; binY++){
      
//       double f =0;

//       float value_x  = Xmin + (Xmax - Xmin) / NbinsX * binX;
//       float value_y  = Ymin + (Ymax - Ymin) / NbinsY * binY;
//       x_array[arr_ind] = value_x;
//       y_array[arr_ind] = value_y;
  


//       // Loop over the bins and determine the contribution to from each bin.
//       int NBINS = fit_hdata->GetNbinsX();
//       for(int bin=1; bin<=NBINS; bin++) {
	
// 	double mu = 0; // predicted number of events in this bin
// 	for (int i=0; i<Nm; i++){
// 	  float param_mc = param[i];
//  	  if (i==x) param_mc = value_x;
//  	  if (i==y) param_mc = value_y;
// 	  mu += param_mc*fit_hmc[i]->GetBinContent(bin);

// 	  //	  cout << "fit_hmc[" << i << "](" << bin << ") = " << fit_hmc[i]->GetBinContent(bin) << endl;
// 	}

// 	if (mu < 1.e-9) mu = 1.e-9;
	
// 	int n = fit_hdata->GetBinContent(bin); // observed number of events in this bin
     
// 	// Likelihood contribution from this bin	
// 	f -= (n*TMath::Log(mu) - mu);
// 	//	cout << "n = " << n << "; mu = " << mu << "; f = " << f << endl;
//       }// loop over bins
      
//       f *= 2;

//       //    cout << "f(" << value_x  << ", " << value_y << ") = " << f << endl;
//       hL->SetBinContent(binX, binY, -f);
//       res_array[arr_ind] = -f;
//       arr_ind++;
      
//       if (res_max > f){
// 	max_x = value_x;
// 	max_y = value_y;
// 	res_max = -f;
// 	//	cout << "(" << max_x << ", " << max_y << ") = " << res_max << endl;
//       }

//     }//binY
//   }//binX

//   cout << "(max_x, max_y) = (" << max_x << ", " << max_y << ")" << endl;
//   cout << "res_max = " << res_max << endl;

// }
