#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"

void analyzePE(double lumi = 0.01) {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111111);

  //Constants
  const double topXS = 0.1575;
  const double wjetsXS = 31.314;
  const int nTopGen = 1483404;
  const int nWJetsGen = 10068895;
  const double convFactor = 1000000.;  //Conversion for lumi in fb and xs in nb^-1.
  //Get the top file
  TFile *topFile = TFile::Open("templates/shyftPretag_ISOCUTttbar_36X.root");
  TH1 *topHist = (TH1 *)topFile->Get("nJets")->Clone("topHist");
  double nTopExp = topHist->Integral()*topXS*lumi*convFactor/nTopGen;
  std::cout << "nTopExp = " << nTopExp << std::endl;

  TFile *inFile = TFile::Open(Form("fitNJetsTemplate_PE_lumi%.4f.root",lumi));

  TTree *tree = (TTree *)inFile->Get("fitParData_model");

  tree->SetBranchStatus("nTop",1);
  tree->SetBranchStatus("nTop_aerr_lo",1);
  tree->SetBranchStatus("nTop_aerr_hi",1);

  TH1 *pullHist = new TH1F("pullHist","Pulls",100,-5,5);

  tree->Draw(Form("(nTop-%f)/(nTop_aerr_hi)>>+pullHist",nTopExp),
             Form("nTop<%f && nTop_aerr_hi > 0",nTopExp),"goff");

  tree->Draw(Form("(nTop-%f)/(-nTop_aerr_lo)>>+pullHist",nTopExp),
             Form("nTop>%f && nTop_aerr_lo < 0",nTopExp),"goff");

  double maxMeas = tree->GetMaximum("nTop");
  double minMeas = tree->GetMinimum("nTop");

  TH1 *measHist = new TH1F("measHist","(Measured - True)/True",100,
                           (minMeas-nTopExp)/nTopExp,
                           (maxMeas-nTopExp)/nTopExp);

  tree->Draw(Form("(nTop-%f)/%f>>+measHist",nTopExp,nTopExp),
             "","goff");


  double maxErr = tree->GetMaximum("nTop_aerr_hi");
  double minErr = tree->GetMinimum("nTop_aerr_lo");

  const double errHistScale = 2.0;

  TH1 *errHiHist = new TH1F("errHiHist","Fractional Error",100,
                            errHistScale*minErr/nTopExp,errHistScale*maxErr/nTopExp);
  errHiHist->SetStats(kFALSE);
  tree->Draw("nTop_aerr_hi/nTop>>+errHiHist","","goff");
  const double prob = 0.5;
  double medianHi = 0.;
  errHiHist->GetQuantiles(1,&medianHi,&prob);
    

  TH1 *errLoHist = new TH1F("errLoHist","Fractional Error",100,
                            errHistScale*minErr/nTopExp,errHistScale*maxErr/nTopExp);
  errLoHist->SetStats(kFALSE);
  errLoHist->SetLineColor(2);
  tree->Draw("nTop_aerr_lo/nTop>>+errLoHist","","goff");
  double medianLo = 0.;
  errLoHist->GetQuantiles(1,&medianLo,&prob);

  double errMax = TMath::Max(errHiHist->GetMaximum(),errLoHist->GetMaximum());
  errHiHist->SetMaximum(1.2*errMax);
  errLoHist->SetMaximum(1.2*errMax);

  TH1 *numHist = new TH1F("numHist","Fit Result",100,minMeas,maxMeas);
  tree->Draw("nTop>>+numHist","","goff");
  

  TCanvas *canv = new TCanvas(Form("pePlots_lumi%.4f",lumi),Form("pePlots_lumi%.4f",lumi),800,800);
  canv->Divide(2,2,0.01,0.01);

  canv->cd(1);
  pullHist->Fit("gaus");
  canv->Update();
  TLine *line = new TLine();
  line->SetLineStyle(2);
  line->DrawLine(0.,0.,0.,gPad->GetFrame()->GetY2());
  double pullMean = pullHist->GetFunction("gaus")->GetParameter(1);
  double pullMeanErr = pullHist->GetFunction("gaus")->GetParError(1);
  double pullSigma = pullHist->GetFunction("gaus")->GetParameter(2);
  double pullSigmaErr = pullHist->GetFunction("gaus")->GetParError(2);

  std::cout << "Pull info:" << std::endl
            << "Mean       Mean(Err)   Dev (sig)   Sigma      Sig. (Err)   Dev (sig)" << std::endl
            << "--------   ---------   ---------   --------   ----------   ---------" << std::endl
            << Form("%8.4g   %9.4g   %9.4g   %8.4g   %10.4g   %9.4g",
                    pullMean, pullMeanErr, pullMean/pullMeanErr,
                    pullSigma, pullSigmaErr, (pullSigma-1)/pullSigmaErr)
            << std::endl;

  canv->cd(2);
  measHist->Fit("gaus");
  double measMean = measHist->GetFunction("gaus")->GetParameter(1);
  double measMeanErr = measHist->GetFunction("gaus")->GetParError(1);
  double measSigma = measHist->GetFunction("gaus")->GetParameter(2);
  double measSigmaErr = measHist->GetFunction("gaus")->GetParError(2);

  std::cout << "Measured info:" << std::endl
            << "Mean       Mean(Err)   Dev (sig)   Sigma      Sig. (Err)" << std::endl
            << "--------   ---------   ---------   --------   ----------" << std::endl
            << Form("%8.4g   %9.4g   %9.4g   %8.4g   %10.4g",
                    measMean, measMeanErr, measMean/measMeanErr,
                    measSigma, measSigmaErr)
            << std::endl;

  canv->cd(3);
  errHiHist->Draw();
  errLoHist->Draw("SAME");
  std::cout << "Fractional error:" << std::endl
            << "  Median (Lo): " << medianLo << std::endl
            << "  Median (Hi): " << medianHi << std::endl;

  canv->cd(4);
  numHist->Draw();
  TArrow *arrow = new TArrow();
  arrow->SetLineWidth(2);
  arrow->DrawArrow(nTopExp,numHist->GetMaximum()*.5,nTopExp,0.0,0.02,">");
  std::cout << "Fitted Number of Events:" << std::endl
            << "  Mean Fit Result: " << numHist->GetMean() << std::endl
            << "  True Value: " << nTopExp << std::endl;

  canv->Print(".gif");


}
