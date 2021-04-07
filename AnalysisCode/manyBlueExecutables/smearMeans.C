#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

Double_t splitGauss(Double_t *x, Double_t *par);


void smearMeans (TString filename = "")
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit (111);
   gStyle->SetStatH(0.4);          // height of box
   gStyle->SetStatW(0.2);         // width of box 
   TF1 *func = new TF1("splitGauss",splitGauss, -.1, .1, 8);
   // Sets initial values and parameter names
   func->SetParameters(900, 0.07, 0.33, 0.33, 0.66, 0.66, 0.5, 0.5);
   func->SetParNames("Constant",         // 0
                     "Max_bValue",       // 1
                     "Negative_Sigma1",  // 2
                     "Positive_Sigma1",  // 3
                     "Negative_Sigma2",  // 4
                     "Positive_Sigma2",  // 5
                     "Negative_frac",    // 6
                     "Positive_frac"     // 7
      );
   func->SetParLimits (2, 0, 1);
   func->SetParLimits (3, 0, 1);
   func->SetParLimits (4, 0, 1);
   func->SetParLimits (5, 0, 1);
   func->SetParLimits (6, 0, 1);
   func->SetParLimits (7, 0, 1);

   for (int loop = 6; loop <= 15; ++loop)
   {
      TCanvas c1;
      int minor = loop % 10;
      int major = loop / 10;
      double height, center;
      TFile *filePtr = TFile::Open( Form("rootfiles/hists_%d_%d.root", 
                                         major, minor) );
      if (filename.Length())
      {
         loop = 20;
         filePtr = TFile::Open (filename);
      }
      cout << endl << endl << "beta " << loop / 10. << endl;
      TH1F *smearLF = (TH1F*) filePtr->Get("smearLF");
      smearLF->SetLineColor (kRed);
      smearLF->SetLineWidth (2);
      cout << "LF smear      " << smearLF->GetMean() << endl;
      smearLF->Fit("splitGauss", "W");
      cout << "LF fit smear  " << func->GetParameter(1) << endl;      
      smearLF->Fit("splitGauss");
      cout << "LF fit smear2 " << func->GetParameter(1) << endl;      
      smearLF->Draw();
      height = func->GetParameter (0);
      center = func->GetParameter (1);
      TLine lfLine (center, 0, center, height);
      lfLine.SetLineStyle(2);
      lfLine.Draw("same");
      c1.Print( Form ("eps/LF_%02d.eps", loop) );
      TH1F *smearME = (TH1F*) filePtr->Get("smearME");
      smearME->SetLineColor (kRed);
      smearME->SetLineWidth (2);
      cout << "ME smear      " << smearME->GetMean() << endl;
      smearME->Fit("splitGauss", "W");
      cout << "ME fit smear  " << func->GetParameter(1) << endl;      
      smearME->Fit("splitGauss");
      cout << "ME fit smear2 " << func->GetParameter(1) << endl;      
      smearME->Draw();
      height = func->GetParameter (0);
      center = func->GetParameter (1);
      TLine meLine (center, 0, center, height);
      meLine.SetLineStyle(2);
      meLine.Draw("same");
      c1.Print( Form ("eps/ME_%02d.eps", loop) );
      TH1F *smearNN = (TH1F*) filePtr->Get("smearNN");
      smearNN->SetLineColor (kRed);
      smearNN->SetLineWidth (2);
      cout << "NN smear      " << smearNN->GetMean() << endl;
      smearNN->Fit("splitGauss", "W");
      cout << "NN fit smear  " << func->GetParameter(1) << endl;      
      smearNN->Fit("splitGauss");
      cout << "NN fit smear2 " << func->GetParameter(1) << endl;      
      smearNN->Draw();
      height = func->GetParameter (0);
      center = func->GetParameter (1);
      TLine nnLine (center, 0, center, height);
      nnLine.SetLineStyle(2);
      nnLine.Draw("same");
      c1.Print( Form ("eps/NN_%02d.eps", loop) );
      delete filePtr;
   } // for loop
}

Double_t splitGauss(Double_t *x, Double_t *par)
{
   double value = x[0];   
   double norm   = par[0];
   double arg    = value - par[1];
   double sigma1 = par[2]; // negative sigma1
   double sigma2 = par[4]; // negative sigma2
   double frac   = par[6]; // negative sigma fraction
   if (arg > 0)
   {
      sigma1 = par[2 + 1]; // positive sigma1
      sigma2 = par[4 + 1]; // positive sigma2
      frac   = par[6 + 1]; // positive sigma fraction
   }
   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 *arg * arg / (sigma1 * sigma1))
      + norm * (1 - frac) * TMath::Exp( -0.5 *arg * arg / (sigma2 * sigma2));
   return fitval;
}
