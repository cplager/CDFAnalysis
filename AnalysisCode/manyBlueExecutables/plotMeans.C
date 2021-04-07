#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

Double_t splitGauss(Double_t *x, Double_t *par);


void plotMeans()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit (111);
   TF1 *func = new TF1("splitGauss",splitGauss, 0, 1, 8);
   // Sets initial values and parameter names
   func->SetParameters(900, 0.07, 0.33, 0.33);
   func->SetParNames("Constant",         // 0
                     "Max_bValue",       // 1
                     "Negative_Sigma1",  // 2
                     "Positive_Sigma1"   // 3
      );

   for (int loop = 6; loop <= 15; ++loop)
   {
      TCanvas c1;
      float beta = loop / 10.;
      cout << endl << endl << "beta " << beta << endl;
      int minor = loop % 10;
      int major = loop / 10;
      TFile *filePtr = TFile::Open( Form("rootfiles/hists_%d_%d.root", 
                                         major, minor) );
      TH1F *meanLF = (TH1F*) filePtr->Get("meanLF");
      meanLF->Fit("splitGauss", "W");
      meanLF->Draw();
      c1.Print( Form ("eps/LF_%02d.eps", loop) );
      cout << "LF syst     " << meanLF->GetMean() << endl;
      cout << "LF fit syst " << func->GetParameter(1) << endl;      
      TH1F *meanME = (TH1F*) filePtr->Get("meanME");
      meanME->Fit("splitGauss", "W");
      meanME->Draw();
      c1.Print( Form ("eps/ME_%02d.eps", loop) );
      cout << "ME syst     " << meanME->GetMean() << endl;
      cout << "ME fit syst " << func->GetParameter(1) << endl;
      TH1F *meanNN = (TH1F*) filePtr->Get("meanNN");
      meanNN->Fit("splitGauss", "W");
      meanNN->Draw();
      c1.Print( Form ("eps/NN_%02d.eps", loop) );
      cout << "NN syst     " << meanME->GetMean() << endl;
      cout << "NN fit syst " << func->GetParameter(1) << endl;
      delete filePtr;
      TFile *filePtr = TFile::Open( Form("rootfiles/hists_%d_%d_nosyst.root", 
                                         major, minor) );
      TH1F *meanLF = (TH1F*) filePtr->Get("meanLF");
      meanLF->Fit("splitGauss", "W");
      meanLF->Draw();
      c1.Print( Form ("eps/LFno_%02d.eps", loop) );
      cout << "LF no syst     " << meanLF->GetMean() << endl;
      cout << "LF no fit syst " << func->GetParameter(1) << endl;      
      TH1F *meanME = (TH1F*) filePtr->Get("meanME");
      meanME->Fit("splitGauss", "W");
      meanME->Draw();
      c1.Print( Form ("eps/MEno_%02d.eps", loop) );
      cout << "ME no syst     " << meanME->GetMean() << endl;
      cout << "ME no fit syst " << func->GetParameter(1) << endl;
      TH1F *meanNN = (TH1F*) filePtr->Get("meanNN");
      meanNN->Fit("splitGauss", "W");
      meanNN->Draw();
      c1.Print( Form ("eps/NNno_%02d.eps", loop) );
      cout << "NN no syst     " << meanME->GetMean() << endl;
      cout << "NN no fit syst " << func->GetParameter(1) << endl;
      delete filePtr;
   } // for loop
}

Double_t splitGauss(Double_t *x, Double_t *par)
{
   double value = x[0];   
   double norm   = par[0];
   double arg    = value - par[1];
   double sigma1 = par[2]; // negative sigma1
   if (arg > 0)
   {
      sigma1 = par[2 + 1]; // positive sigma1
   }
   Double_t fitval = 
      norm * TMath::Exp( -0.5 *arg * arg / (sigma1 * sigma1));
   return fitval;
}
