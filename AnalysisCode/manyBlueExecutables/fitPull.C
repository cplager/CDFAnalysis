b#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"

Double_t splitGauss(Double_t *x, Double_t *par);


void fitPull (TString filename = "rootfiles/hists_0_7_fitpullUnbias_trunk.root",
              TString paramName = "pull",
              TString title     = "pull",
              TString fitname   = "gaus")
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
   func->FixParameter (1, 0.);
   // func->SetParLimits (2, 0, 1);
   // func->SetParLimits (3, 0, 1);
   // func->SetParLimits (4, 0, 1);
   // func->SetParLimits (5, 0, 1);
   // func->SetParLimits (6, 0, 1);
   // func->SetParLimits (7, 0, 1);

   TCanvas c1;
   double height, center;
   TFile *filePtr = TFile::Open( filename );
   TH1F *smear = (TH1F*) filePtr->Get(paramName);
   smear->SetTitle(title);
   smear->SetLineColor (kBlue);
   smear->SetLineWidth (2);
   smear->Fit(fitname);
   smear->Draw();
   TF1 *funcPtr = smear->GetFunction (fitname);
   height = funcPtr->GetParameter (0);
   center = funcPtr->GetParameter (1);
   TLine lfLine (center, 0, center, height);
   lfLine.SetLineStyle(2);
   lfLine.Draw("same");
   c1.Print( "eps/pull.eps" );
   cout << " smear     " << smear->GetMean() << endl;
   cout << " fit smear " << funcPtr->GetParameter(1) << endl;      
   delete filePtr;
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
