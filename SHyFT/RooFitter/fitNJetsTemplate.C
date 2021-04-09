#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooPlot.h"
#include "RooArgList.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooMCStudy.h"
#include "RooRealConstant.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TPad.h"

using namespace RooFit ;


void fitNJetsTemplate(double lumi = 0.01, int npe = 5000) {

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
  std::cout << "Expected top = " << nTopExp << std::endl;


  //Define the x-axis variable.
  RooRealVar nJets("nJets","nJets",topHist->GetXaxis()->GetXmin(),
                   topHist->GetXaxis()->GetXmax());
  nJets.setBins(topHist->GetNbinsX());

  //Turn it into the Top PDF
  RooDataHist topDH("top","N_{Jets} for Top",RooArgList(nJets),topHist);
  RooHistPdf topPDF("topPDF","Top PDF",nJets,topDH);

  //Get the wjets file
  TFile *wjetsFile = TFile::Open("templates/shyftPretag_ISOCUTwjets_36X.root");
  TH1 *wjetsHist = (TH1 *)wjetsFile->Get("nJets")->Clone("wjetsHist");
  double nWJetsExp = wjetsHist->Integral()*wjetsXS*lumi*convFactor/nWJetsGen;
  std::cout << "Expected W+Jets = " << nWJetsExp << std::endl;

  //Turn it into the Top PDF
  RooDataHist wjetsDH("wjets","N_{Jets} for W+Jets",RooArgList(nJets),wjetsHist);
  RooHistPdf wjetsPDF("wjetsPDF","W+Jets PDF",nJets,wjetsDH);

  //Now, make a two component PDF
  double nTotal = nTopExp + nWJetsExp;
  RooRealVar nTop("nTop","N_{Top}",nTopExp,-3.0*nTopExp,3.*nTotal);
  RooRealVar nWJets("nWJets","N_{W+Jets}",nWJetsExp,
                        -3.0*nWJetsExp,3.*nTotal);

  RooAddPdf model("model","S+B",RooArgList(topPDF,wjetsPDF),RooArgList(nTop,nWJets));

//   //Make just one PE
//   RooDataHist *dataHist = model.generateBinned(nJets, Verbose(kTRUE));

//   model.fitTo(*dataHist);

//   TCanvas *canv = new TCanvas("canv","Canvas",500,500);
//   canv->cd();
//   RooPlot *frame = nJets.frame();
//   dataHist->plotOn(frame);
//   model.plotOn(frame);
//   frame->Draw();
  
  //Do some pseudo-experiments
  RooMCStudy* mcstudy = new RooMCStudy(model,nJets,Binned(kTRUE),Silence(),Extended(),
                                       FitOptions(Save(kTRUE),PrintEvalErrors(-1),Minos(kTRUE),Strategy(2))) ;

  //A module used to calculate extra variables
  //   SHyFTMCSModule shyftMCSMod(RooArgList(nTop,nWJets));
  //   mcstudy->addModule(shyftMCSMod);

  // Generate and fit npe samples of Poisson(nExpected) events
  mcstudy->generateAndFit(npe) ;
    
//   // Make plots of the distributions of mean, the error on mean and the pull of mean
//   RooPlot* frame1 = mcstudy->plotParam(nTop,Bins(40)) ;
//   RooPlot* frame2 = mcstudy->plotError(nTop,Bins(40)) ;
//   RooPlot* frame3 = mcstudy->plotPull(nTop,Bins(40),FitGauss(kTRUE)) ;


//   // Plot distribution of minimized likelihood
//   RooPlot* frame4 = mcstudy->plotNLL(Bins(40)) ;

//   // Draw all plots on a canvas
//   gROOT->SetStyle("Plain");
//   gStyle->SetPalette(1) ;
//   gStyle->SetOptStat(0) ;
//   TCanvas* canv = new TCanvas("canv","Pseudo-Experiments",600,600) ;
//   canv->Divide(2,2) ;

//   canv->cd(1); 
//   gPad->SetLeftMargin(0.15); 
//   frame1->GetYaxis()->SetTitleOffset(1.4); 
//   frame1->Draw() ;

//   canv->cd(2);
//   gPad->SetLeftMargin(0.15);
//   frame2->GetYaxis()->SetTitleOffset(1.4); 
//   frame2->Draw() ;

//   canv->cd(3);
//   gPad->SetLeftMargin(0.15); 
//   frame3->GetYaxis()->SetTitleOffset(1.4); 
//   frame3->Draw() ;
  
//   canv->cd(4); 
//   gPad->SetLeftMargin(0.15); 
//   frame4->GetYaxis()->SetTitleOffset(1.4); 
//   frame4->Draw() ;
  

//   TCanvas *canv2 = new TCanvas("canv2","testCanv",500,500);
//   canv2->cd();
//   RooRealVar nTop_err("nTop_err","nTop_err",0.);
//   RooFormulaVar fracErr("fracErr","Fractional Error","nTop_err/nTop",RooArgList(nTop_err,nTop));
//   RooDataSet fitResults(mcstudy->fitParDataSet(),"fitResults");
//   fitResults.addColumn(fracErr);
//   TH1 *fracErrHist = mcstudy->fitParDataSet().createHistogram("nTop_fracErr");
//   fracErrHist->Draw();


  std::cout << std::endl << "Dataset parameters:  ";
  mcstudy->fitParDataSet().printArgs(std::cout);
  std::cout << std::endl;
  


  //Save the PE results to a tree
  TFile *outFile = TFile::Open(Form("fitNJetsTemplate_PE_lumi%.4f.root",lumi),"RECREATE");
  outFile->cd();
  const TTree *peTree = mcstudy->fitParDataSet().tree();
  peTree->Write();
  outFile->Close();


}
