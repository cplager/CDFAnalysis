#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"

void analyzePE(TString inputFileName, TString outputTag) {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(111110);
  gStyle->SetOptFit(111111);

  //TString inFileName = "qcdPE_";
  //inFileName += var;
  //inFileName += Form("_lumi%.3finvpb.root",lumi*1000);

  TString inFileName = inputFileName;
  
  TFile *inFile = TFile::Open(inFileName);

  TTree *tree = (TTree *)inFile->Get("merged");

  TList vars;
  vars.Add(new TObjString("xsttbar"));
  vars.Add(new TObjString("xsWJets"));
  vars.Add(new TObjString("xsSingleTop"));
  vars.Add(new TObjString("ratioZJets_WJets"));
  // vars.Add(new TObjString("xsZJets"));
  //vars.Add(new TObjString("xsQCD"));
  //vars.Add(new TObjString("numQCD1met"));
  //vars.Add(new TObjString("numQCD2j"));
  //vars.Add(new TObjString("numQCD3j"));
  //vars.Add(new TObjString("numQCD4j"));

  TIter nextVar(&vars);
  TObjString *varObj = 0;

  while ((varObj = (TObjString *)nextVar())) {



    
    TString peVar = varObj->String();

    TCanvas * myCan1 = new TCanvas (peVar+"_"+outputTag+"_PullMeas", peVar, 1100, 700);
    //TCanvas * myCan2 = new TCanvas (peVar+"_"+outputTag+"_MeasTrue", peVar, 1100, 700);
    //TCanvas * myCan3 = new TCanvas (peVar+"_"+outputTag+"_MeasMinusTrueError", peVar, 1100, 700);
    
    
    myCan1->Divide(2,1);
    //myCan2->Divide(2,1);
    //myCan3->Divide(2,1);
    
    
    

    //Turn on the branches in the file
    tree->SetBranchStatus(peVar,1);
    tree->SetBranchStatus(peVar+"_gen",1);
    tree->SetBranchStatus(peVar+"_aerr_lo",1);
    tree->SetBranchStatus(peVar+"_aerr_hi",1);

    std::cout << "---------------------------------------------------" << std::endl
              << "  " << peVar << " Results: " << std::endl;

    TH1 *pullHist = new TH1F("pullHist"+peVar,"Pulls",100,-10,10);


    cout << "Drawing pulls for " << peVar.Data() << endl;
    tree->Draw(Form("(%s-%s_gen)/(%s_aerr_hi)>>+pullHist%s",peVar.Data(),peVar.Data(),peVar.Data(),peVar.Data()),
               Form("%s<%s_gen && %s_aerr_hi > 0",peVar.Data(),peVar.Data(),peVar.Data()),
               "goff");

    tree->Draw(Form("(%s-%s_gen)/(-%s_aerr_lo)>>+pullHist%s",peVar.Data(),peVar.Data(),peVar.Data(),peVar.Data()),
               Form("%s>%s_gen && %s_aerr_lo < 0",peVar.Data(),peVar.Data(),peVar.Data()),
               "goff");

    myCan1->cd(1);
    pullHist->Fit("gaus");

    double maxMeas = tree->GetMaximum(peVar);
    double minMeas = tree->GetMinimum(peVar);    
    TH1 *numHist = new TH1F("numHist"+peVar,"Fit Result",100,minMeas,maxMeas);

    cout << "Drawing the fitted var called " << peVar.Data() << endl;
    //myCan2->cd(1);
    tree->Draw(Form("%s>>+numHist%s",peVar.Data(),peVar.Data()),
               "",
               "goff");

    double maxGen = tree->GetMaximum(peVar+"_gen");
    double minGen = tree->GetMinimum(peVar+"_gen");

    double aveGen = 0.;

    
    //myCan2->cd(2);
    tree->Draw(Form("%s_gen>>+genHist%s",peVar.Data(),peVar.Data()),
               "",
               "goff");

    
    
    
    if (maxGen == minGen) {
      aveGen = minGen; //Or maxGen since they're the same--all same value.
    } else {
      
      TH1 *genHist = new TH1F("genHist"+peVar,"Generated Parameter",100,minGen,maxGen);
 
      cout << "Drawing the gen value called " << peVar.Data() << endl;
      tree->Draw(Form("%s_gen>>+genHist%s",peVar.Data(),peVar.Data()),
               "",
               "goff");
      
      
      aveGen = genHist->GetMean();
    }

    TH1 *measHist = new TH1F("measHist"+peVar,"(Measured - True)/True",100,
                             (minMeas-aveGen)/aveGen,
                             (maxMeas-aveGen)/aveGen);

    cout << "Drawing the measured-true value called " << peVar.Data() << endl;

   
    tree->Draw(Form("(%s-%s_gen)/%s_gen>>+measHist%s",peVar.Data(),peVar.Data(),peVar.Data(),peVar.Data()),
               "",
               "goff");
   
    

    myCan1->cd(2);
    measHist->Fit("gaus");



    
    //myCan3->cd(1);
    tree->Draw(Form("(%s-%s_gen)",peVar.Data(),peVar.Data()),
               "",
               "goff");
    

    //myCan3->cd(2);

    tree->Draw(Form("(%s_aerr_lo)",peVar.Data(),peVar.Data(),peVar.Data(),peVar.Data()),
               "",
               "goff");

    if (outputTag != "" ) myCan1->SaveAs(".png");
    //if (outputTag != "") myCan2->SaveAs(".pdf");
    //if (outputTag != "") myCan3->SaveAs(".pdf");


    
    
    
  }

//   TCanvas * myCan4 = new TCanvas ("ratioZW_"+outputTag+"_RatioGen", peVar, 1100, 700);
//   myCan4->Divide(2,1);
//   myCan4->cd(1);
//   tree->Draw("ratioZW_gen");

//   myCan4->cd(2);
//   tree->Draw("ratioZW - ratioZW_gen");

//   myCan4->SaveAs(".pdf");

  

}
