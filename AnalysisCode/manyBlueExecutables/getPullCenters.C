#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

Double_t splitGauss(Double_t *x, Double_t *par);

void getPullCenters (TString filename = "", TString fitname = "gaus")
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit (111);
   gStyle->SetStatH(0.4);          // height of box
   gStyle->SetStatW(0.2);         // width of box 
   TF1 *func = new TF1("splitGauss",splitGauss, -.1, .1, 4);
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
      double height, center;
      int minor = loop % 10;
      int major = loop / 10;
      TFile *filePtr = TFile::Open( Form("rootfiles/hists_%d_%d.root", 
                                         major, minor) );
      if (filename.Length())
      {
         loop = 20;
         filePtr = TFile::Open (filename);
      }
      float beta = loop / 10.;
      cout << endl << endl << "beta " << beta << endl;

      // LF
      TH1F *pullLF = (TH1F*) filePtr->Get("pullLF");
      pullLF->Fit(fitname, "", "", pullLF->GetMean() - 2.5, 
                  pullLF->GetMean() + 2.5);
      pullLF->Draw();
      func = pullLF->GetFunction(fitname);
      height = func->GetParameter (0);
      center = func->GetParameter (1);
      TLine lfLine (center, 0, center, height);
      lfLine.SetLineStyle(2);
      lfLine.Draw("same");
      c1.Print( Form ("eps/LF_%02d.eps", loop) );
      cout << "LF syst     " << pullLF->GetMean() << endl;
      cout << "LF fit syst " 
           << pullLF->GetFunction(fitname)->GetParameter(1) << endl;      

      // ME
      TH1F *pullME = (TH1F*) filePtr->Get("pullME");
      pullME->Fit(fitname, "", "", pullME->GetMean() - 2.5, 
                  pullME->GetMean() + 2.5);
      pullME->Draw();
      func = pullME->GetFunction(fitname);
      height = func->GetParameter (0);
      center = func->GetParameter (1);
      TLine meLine (center, 0, center, height);
      meLine.SetLineStyle(2);
      meLine.Draw("same");
      c1.Print( Form ("eps/ME_%02d.eps", loop) );
      cout << "ME syst     " << pullME->GetMean() << endl;
      cout << "ME fit syst " 
           << pullME->GetFunction(fitname)->GetParameter(1) << endl;

      // NN
      TH1F *pullNN = (TH1F*) filePtr->Get("pullNN");
      pullNN->Fit(fitname, "", "", pullNN->GetMean() - 2.5, 
                  pullNN->GetMean() + 2.5);
      pullNN->Draw();
      func = pullNN->GetFunction(fitname);
      height = func->GetParameter (0);
      center = func->GetParameter (1);
      TLine nnLine (center, 0, center, height);
      nnLine.SetLineStyle(2);
      nnLine.Draw("same");
      c1.Print( Form ("eps/NN_%02d.eps", loop) );
      cout << "NN syst     " << pullNN->GetMean() << endl;
      cout << "NN fit syst " 
           << pullNN->GetFunction(fitname)->GetParameter(1) << endl;
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
      norm * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1));

   return fitval;
}
