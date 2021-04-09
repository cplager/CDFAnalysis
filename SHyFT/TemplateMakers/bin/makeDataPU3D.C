#include "TMath.h"
#include "TH1.h"
#include "TH3.h"
#include "TFile.h"
#include "TStyle.h"
#include "TROOT.h"

#include <iostream>

void makeDataPU3D(TString mcFileName,
                  TString meanFileName, 
                  TString outFileName) {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TFile *mcFile = TFile::Open(mcFileName);
  TH3 *mcHist = (TH3 *)mcFile->Get("nInteractions3D")->Clone("mcHist");

  TFile *meanFile = TFile::Open(meanFileName);
  TH1 *meanHist = (TH1 *)meanFile->Get("pileup")->Clone("meanHist");

  //Now we want to integrate this histogram convoluted with the
  //appropriate Poisson
  TH3 *obsHist = (TH3 *)mcHist->Clone("obsHist");
  obsHist->Reset();

  std::cout << "Building the 3D observed histogram..." << std::endl;
  for (int iBin = 1; iBin <= meanHist->GetNbinsX(); ++iBin) {

    std::cout << "--> Calculating for bin " << iBin << std::endl;

    double mean = meanHist->GetBinCenter(iBin);
    double nEntries = meanHist->GetBinContent(iBin);

    if (nEntries > 0) {
      for (int nIntM1 = 0; nIntM1 < 50; ++nIntM1) {
        for (int nInt0 = 0; nInt0 < 50; ++nInt0) {
          for (int nIntP1 = 0; nIntP1 < 50; ++nIntP1) {
            double weight = TMath::PoissonI(nIntM1,mean)*
              TMath::PoissonI(nInt0,mean)*TMath::PoissonI(nIntP1,mean);
            obsHist->Fill(nIntM1,nInt0,nIntP1,nEntries*weight);
          }
        }
      }
    }

  }

  //Now, calculate the weights
  obsHist->Scale(1./obsHist->Integral());
  mcHist->Scale(1./mcHist->Integral());
  TH3 *weights = (TH3 *)obsHist->Clone("weights");
  weights->Divide(mcHist);


  TFile *outFile = TFile::Open(outFileName,"RECREATE");
  outFile->cd();
  obsHist->Write("DHist");
  mcHist->Write("MHist");
  weights->Write("WHist");
  
  
}
    

