#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"

void analyzePE2D(TString inFileName) {

  gROOT->SetStyle("Plain");
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111111);

  TFile *inFile = TFile::Open(inFileName);

  TTree *tree = (TTree *)inFile->Get("merged");
  
  TList vars;
  vars.Add(new TObjString("xsttbar"));
  vars.Add(new TObjString("xsWJets"));
  vars.Add(new TObjString("numQCD2j"));
  vars.Add(new TObjString("numQCD3j"));
  vars.Add(new TObjString("numQCD4j"));

  TIter nextVar(&vars);
  TObjString *varObj = 0;

  while ((varObj = (TObjString *)nextVar())) {

    TString peVar = varObj->String();

    //Turn on the branches in the file
    tree->SetBranchStatus(peVar,1);
    tree->SetBranchStatus(peVar+"_gen",1);
    tree->SetBranchStatus(peVar+"_aerr_lo",1);
    tree->SetBranchStatus(peVar+"_aerr_hi",1);

    std::cout << "---------------------------------------------------" << std::endl
              << "  " << peVar << " Results: " << std::endl;

    TH1 *pullHist = new TH1F("pullHist"+peVar,"Pulls",100,-15,15);

    tree->Draw(Form("(%s-%s_gen)/(%s_aerr_hi)>>+pullHist%s",peVar.Data(),peVar.Data(),peVar.Data(),peVar.Data()),
               Form("%s<%s_gen && %s_aerr_hi > 0",peVar.Data(),peVar.Data(),peVar.Data()),"goff");

    tree->Draw(Form("(%s-%s_gen)/(-%s_aerr_lo)>>+pullHist%s",peVar.Data(),peVar.Data(),peVar.Data(),peVar.Data()),
               Form("%s>%s_gen && %s_aerr_lo < 0",peVar.Data(),peVar.Data(),peVar.Data()),"goff");


    double maxMeas = tree->GetMaximum(peVar);
    double minMeas = tree->GetMinimum(peVar);
    TH1 *numHist = new TH1F("numHist"+peVar,"Fit Result",100,minMeas,maxMeas);
    tree->Draw(Form("%s>>+numHist%s",peVar.Data(),peVar.Data()),"","goff");

    double maxGen = tree->GetMaximum(peVar+"_gen");
    double minGen = tree->GetMinimum(peVar+"_gen");

    double aveGen = 0.;

    if (maxGen == minGen) {
      aveGen = minGen; //Or maxGen since they're the same--all same value.
    } else {
      TH1 *genHist = new TH1F("genHist"+peVar,"Generated Parameter",100,minGen,maxGen);
      tree->Draw(Form("%s_gen>>+genHist%s",peVar.Data(),peVar.Data()),"","goff");
    }

    TH1 *measHist = new TH1F("measHist"+peVar,"(Measured - True)/True",100,
                             (minMeas-aveGen)/aveGen,
                             (maxMeas-aveGen)/aveGen);

    tree->Draw(Form("(%s-%s_gen)/%s_gen>>+measHist%s",peVar.Data(),peVar.Data(),peVar.Data(),peVar.Data()),
               Form("%s_gen != 0. ",peVar.Data()),"goff");


    double maxErr = tree->GetMaximum(peVar+"_aerr_hi");
    double minErr = tree->GetMinimum(peVar+"_aerr_lo");

    const double errHistScale = 1.1;

    TH1 *errHiHist = new TH1F("errHiHist"+peVar,"Fractional Error",100,
                            errHistScale*minErr/aveGen,errHistScale*maxErr/aveGen);
    errHiHist->SetStats(kFALSE);
    tree->Draw(Form("%s_aerr_hi/%s>>+errHiHist%s",peVar.Data(),peVar.Data(),peVar.Data()),"","goff");
    const double prob = 0.5;
    double medianHi = 0.;
    errHiHist->GetQuantiles(1,&medianHi,&prob);
    

    TH1 *errLoHist = new TH1F("errLoHist"+peVar,"Fractional Error",100,
                              errHistScale*minErr/aveGen,errHistScale*maxErr/aveGen);
    errLoHist->SetStats(kFALSE);
    errLoHist->SetLineColor(2);
    tree->Draw(Form("%s_aerr_lo/%s>>+errLoHist%s",peVar.Data(),peVar.Data(),peVar.Data()),"","goff");
    double medianLo = 0.;
    errLoHist->GetQuantiles(1,&medianLo,&prob);

    double errMax = TMath::Max(errHiHist->GetMaximum(),errLoHist->GetMaximum());
    errHiHist->SetMaximum(1.2*errMax);
    errLoHist->SetMaximum(1.2*errMax);
  

    TString canvName = "pePlot_";
    canvName += peVar;

    TCanvas *canv = new TCanvas(canvName,canvName,800,400);
    canv->Divide(2,1,0.01,0.01);
//     canv->Divide(2,2,0.01,0.01);

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

    TLatex *text = new TLatex();
    text->SetNDC();
    text->DrawLatex(0.15,0.8,peVar);


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
    
//     canv->cd(3);
//     errHiHist->Draw();
//     errLoHist->Draw("SAME");
    std::cout << "Fractional error:" << std::endl
              << "  Median (Lo): " << medianLo << std::endl
              << "  Median (Hi): " << medianHi << std::endl;

//     canv->cd(4);
//     numHist->Draw();
//     TArrow *arrow = new TArrow();
//     arrow->SetLineWidth(2);
//     arrow->DrawArrow(aveGen,numHist->GetMaximum()*.5,aveGen,0.0,0.02,">");
    std::cout << "Fitted Number of Events:" << std::endl
              << "  Mean Fit Result: " << numHist->GetMean() << std::endl
              << "  True Value: " << aveGen << std::endl;

    canv->Print(".png");
  }

}
