#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "TPad.h"
#include "TMath.h"
#include "TF1.h"

double calcP(double *x, double *par);
double calcNJet(double *x, double *par);

void compNJetsFunc() {

  //The fit results
  double a0 = 2.41073e-01;
  double a1 = 2.33256e-01;
  double a2 = -1.13590e-01;
  double xsWjets = 2.91331e+01;

  int nGenWJets = 10068895;

  double xsttbar = -4.63916e-01;
  int nGenttbar = 1483404;

  double zwRatio0j = 4.89123e-02;
  double zwRatio1j = 8.01661e-02;
  double zwRatio2j = 7.66444e-02;
  double zwRatio3j = 8.20949e-02;
  double zwRatio4j = 8.33696e-02;

  double numQCD0j = 1.30099e+03;
  double numQCD1j = 4.94097e+02;
  double numQCD2j = -1.28496e+01;
  double numQCD3j = -2.63086e+01;
  double numQCD4j = -2.88683e+01;

  double lumi = 0.002961;
  const double lumiConvFactor = 1e6;

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  //Get some histograms
  std::cout << "Opening W+jets file..." << std::endl;
  TFile *wjetsFile = TFile::Open("templates/shyftPretag_wjets.root");
  TH1 *wjetsTemp = (TH1 *)wjetsFile->Get("nJets");
  TH1 *wjetsNJets = new TH1F("wjetsNjets",";N_{Jets};Events",5,-0.5,4.5);
  for (int iBin = 1; iBin <= 4; ++iBin) {
    wjetsNJets->SetBinContent(iBin,wjetsTemp->GetBinContent(iBin));
  }
  wjetsNJets->SetBinContent(5,wjetsTemp->GetBinContent(5)+
                            wjetsTemp->GetBinContent(6));
  double accWjets = wjetsNJets->Integral()/((double)nGenWJets);
  wjetsNJets->Sumw2();
  wjetsNJets->Scale(lumi*lumiConvFactor*xsWjets/((double)nGenWJets));

  std::cout << "Opening ttbar file..." << std::endl;
  TFile *ttbarFile = TFile::Open("templates/shyftPretag_ttbar.root");
  TH1 *ttbarTemp = (TH1 *)ttbarFile->Get("nJets");
  TH1 *ttbarNJets = new TH1F("ttbarNjets",";N_{Jets};Events",5,-0.5,4.5);
  for (int iBin = 1; iBin <= 4; ++iBin) {
    ttbarNJets->SetBinContent(iBin,ttbarTemp->GetBinContent(iBin));
  }
  ttbarNJets->SetBinContent(5,ttbarTemp->GetBinContent(5)+
                            ttbarTemp->GetBinContent(6));
  double accttbar = ttbarNJets->Integral()/((double)nGenttbar);
  ttbarNJets->Sumw2();
  ttbarNJets->Scale(lumi*lumiConvFactor*xsttbar/((double)nGenttbar));

  std::cout << "Opening data file..." << std::endl;
  TFile *dataFile = TFile::Open("templates/shyftPretag_all.root");
  TH1 *dataTemp = (TH1 *)dataFile->Get("nJets");
  TH1 *dataNJets = new TH1F("dataNjets",";N_{Jets};Events",5,-0.5,4.5);
  for (int iBin = 1; iBin <= 4; ++iBin) {
    dataNJets->SetBinContent(iBin,dataTemp->GetBinContent(iBin));
  }
  dataNJets->SetBinContent(5,dataTemp->GetBinContent(5)+
                            dataTemp->GetBinContent(6));

  //Construct some other histograms
  std::cout << "Constructiong QCD histogram" << std::endl;
  TH1 *qcdNJets = new TH1F("qcdNJets",";N_{Jets};Events",5,-0.5,4.5);
  qcdNJets->SetBinContent(1,numQCD0j);
  qcdNJets->SetBinContent(2,numQCD1j);
  qcdNJets->SetBinContent(3,numQCD2j);
  qcdNJets->SetBinContent(4,numQCD3j);
  qcdNJets->SetBinContent(5,numQCD4j);

  std::cout << "Constructiong W+Jets 'Func' histogram" << std::endl;
  TH1 *wjetsNJetsFunc = new TH1F("wjetsNJetsFunc",";N_{Jets};Events",5,-0.5,4.5);
  double par[] = {accWjets*lumi*lumiConvFactor*xsWjets,a0,a1,a2};
  double nj = 0.;
  wjetsNJetsFunc->SetBinContent(1,calcNJet(&nj,par));
  wjetsNJetsFunc->SetBinError(1,0.);
  nj = 1.;
  wjetsNJetsFunc->SetBinContent(2,calcNJet(&nj,par));
  wjetsNJetsFunc->SetBinError(2,0.);
  nj = 2.;
  wjetsNJetsFunc->SetBinContent(3,calcNJet(&nj,par));
  wjetsNJetsFunc->SetBinError(3,0.);
  nj = 3.;
  wjetsNJetsFunc->SetBinContent(4,calcNJet(&nj,par));
  wjetsNJetsFunc->SetBinError(4,0.);
  nj = 4.;
  wjetsNJetsFunc->SetBinContent(5,calcNJet(&nj,par));
  wjetsNJetsFunc->SetBinError(5,0.);

  std::cout << "Constructiong Z+jets histogram" << std::endl;
  TH1 *zjetsNJets = new TH1F("zjetsNJets",";N_{Jets};Events",5,-0.5,4.5);
  zjetsNJets->SetBinContent(1,zwRatio0j*wjetsNJetsFunc->GetBinContent(1));
  zjetsNJets->SetBinContent(2,zwRatio1j*wjetsNJetsFunc->GetBinContent(2));
  zjetsNJets->SetBinContent(3,zwRatio2j*wjetsNJetsFunc->GetBinContent(3));
  zjetsNJets->SetBinContent(4,zwRatio3j*wjetsNJetsFunc->GetBinContent(4));
  zjetsNJets->SetBinContent(5,zwRatio4j*wjetsNJetsFunc->GetBinContent(5));
  

  //First plot: Overlay default W+Jets and fit distribution + Ratio
  std::cout << "Draw on canvas" << std::endl;
  TCanvas *canv1 = new TCanvas("wjetsComp","wjetsComp",500,700);
  canv1->Divide(1,2);

  canv1->cd(1);
  gPad->SetLogy();

  double histMax = 2*TMath::Max(wjetsNJets->GetMaximum(),wjetsNJetsFunc->GetMaximum());
  double histMin = 0.5*TMath::Min(wjetsNJets->GetMinimum(),wjetsNJetsFunc->GetMinimum());

  wjetsNJetsFunc->SetLineColor(2);
  wjetsNJetsFunc->SetMaximum(histMax);
  wjetsNJetsFunc->SetMinimum(histMin);
  wjetsNJetsFunc->Draw();
  wjetsNJets->Draw("SAME");

  TLegend *wjetsLeg = new TLegend(0.6,0.6,0.9,0.9);
  wjetsLeg->SetFillColor(0);
  wjetsLeg->AddEntry(wjetsNJets,"WJets MC","l");
  wjetsLeg->AddEntry(wjetsNJetsFunc,"WJets Fit","l");
  wjetsLeg->Draw();

  canv1->cd(2);
  TH1 *wjetsRatio = (TH1 *)wjetsNJetsFunc->Clone("wjetsRatio");
  wjetsRatio->SetLineColor(1);
  wjetsRatio->SetMaximum(-1111);
  wjetsRatio->GetYaxis()->SetTitle("Fit/MC Ratio");
  wjetsRatio->SetMinimum(0.0);
  wjetsRatio->Divide(wjetsNjets);
  wjetsRatio->Draw();

  canv1->Print(".png");
  

  //Now lets do some stack plots of contributions
  THStack *stack = new THStack("stack",";N_{Jets};Events");
  TH1 *total = new TH1F("total",";N_{Jets};Events",5,-0.5,4.5);
  double stackMin = 9e20;
  qcdNJets->SetLineColor(1);
  qcdNJets->SetFillColor(kYellow);
  stack->Add(qcdNJets,"HIST");
  total->Add(qcdNJets);
  zjetsNJets->SetLineColor(1);
  zjetsNJets->SetFillColor(kAzure-2);
  stack->Add(zjetsNJets,"HIST");
  total->Add(zjetsNJets);
  TH1 *wjetsNJetsFunc2 = (TH1 *)wjetsNJetsFunc->Clone("wjetsNJetsFunc2");
  wjetsNJetsFunc2->SetLineColor(1);
  wjetsNJetsFunc2->SetFillColor(kGreen-3);
  stack->Add(wjetsNJetsFunc2,"HIST");
  total->Add(wjetsNJetsFunc2);
  ttbarNJets->SetLineColor(1);
  ttbarNJets->SetFillColor(kRed+1);
  stack->Add(ttbarNJets,"HIST");
  total->Add(ttbarNJets);

  std::cout << stack->GetMaximum() << "  " << stack->GetMinimum() << std::endl;

  std::cout << ">=4-jet bin:" << std::endl
            << "  QCD = " << qcdNJets->GetBinContent(5) << std::endl
            << "  Z+Jets = " << zjetsNJets->GetBinContent(5) << std::endl
            << "  W+Jets = " << wjetsNJetsFunc2->GetBinContent(5) << std::endl
            << "  Top = " << ttbarNJets->GetBinContent(5) << std::endl
            << "  Total = " << total->GetBinContent(5) << std::endl;

  TLegend *stackLeg = new TLegend(0.6,0.6,0.9,0.9);
  stackLeg->SetFillColor(0);
  stackLeg->AddEntry(ttbarNJets,"t#bar{t}","f");
  stackLeg->AddEntry(wjetsNJetsFunc2,"W+Jets","f");
  stackLeg->AddEntry(zjetsNJets,"Z+Jets","f");
  stackLeg->AddEntry(qcdNJets,"QCD","f");

  TCanvas *canv2 = new TCanvas("stackComp","stackComp",500,700);
  canv2->Divide(1,2);
  canv2->cd(1);
  gPad->SetLogy();

  stack->Draw();
  double stackMax = TMath::Max(stack->GetMaximum(),dataNJets->GetMaximum());
  stack->SetMaximum(stackMax*2);
  stack->Draw();
  dataNJets->SetLineColor(1);
  dataNJets->SetMarkerStyle(20);
  dataNJets->Draw("E0SAME");

  stackLeg->Draw();

  canv2->cd(2);
  TH1 *dataRatio = (TH1 *)dataNJets->Clone("dataRatio");
  dataRatio->Divide(total);
  dataRatio->SetMaximum(-1111);
  double rMinDelta = 1-dataRatio->GetMinimum();
  double rMaxDelta = dataRatio->GetMaximum()-1;
  double delta = 1.1*TMath::Max(rMinDelta,rMaxDelta);
  dataRatio->SetMaximum(1 + delta);
  dataRatio->SetMinimum(1 - delta);
  dataRatio->GetYaxis()->SetTitle("data/Fit Ratio");
  dataRatio->Draw("HIST");

  TLine *l = new TLine();
  l->SetLineStyle(2);
  l->DrawLine(-0.5,1.0,4.5,1.0);

  canv2->Print(".png");

  //Also draw the ratio function
  TCanvas *canv3 = new TCanvas("pFunc","pFunc",500,400);
  canv3->cd();

  TF1 *pFunc = new TF1("pFunc",calcP,0.5,4.5,3);
  pFunc->SetTitle(";N_{jets};P = N_{n}/N_{n-1}");
  pFunc->SetParameter(0,1.61e-1);
  pFunc->SetParameter(1,3.75e-2);
  pFunc->SetParameter(2,-1.05e-2);
  
  pFunc->SetMaximum(0.5);
  pFunc->SetMinimum(0.0);

  pFunc->Draw();

  TF1 *pFunc2 = new TF1("pFunc2",calcP,0.5,4.5,3);
  pFunc2->SetParameter(0,a0);
  pFunc2->SetParameter(1,a1);
  pFunc2->SetParameter(2,a2);

  pFunc2->SetLineColor(2);
  pFunc2->Draw("SAME");

  TLegend *pLeg = new TLegend(0.7,0.75,0.9,0.9);
  pLeg->SetFillColor(0);
  pLeg->AddEntry(pFunc,"Default","l");
  pLeg->AddEntry(pFunc2,"Fit","l");
  pLeg->Draw();

  canv3->Print(".png");

}
      
double calcP(double *x, double *par) {

  //This function only changes at integers
  int nJet = TMath::Nint(x[0]);

  //Convert into something that goes from -1 to 1:
  double x2 = -1 + 2*((double)nJet+0.5)/6;

  double p = par[0] + par[1]*x2 + par[2]*(2*x2*x2-1);

//   std::cout << "   --->calcP = " << p << std::endl;
  
  return p;

}

double calcNJet(double *x, double *par) {

  //Convert the jet bin to an integrer
  int nJet = TMath::Nint(x[0]);
  if (nJet < 0 || nJet > 4) return 0.;

  double N0 = par[0];

  double *polyPars = &par[1];

//   std::cout << "calcNJet: x[0] = " << x[0] 
//             << ", nJet = " << nJet;
//   for (int i = 0; i < 4; ++i) 
//     std::cout << ", par[" << i << "] = "
//               << par[i];
//   std::cout << std::endl;

  double num = N0;
//   std::cout << "--> Initial N0 = " << num << std::endl;

  double nJetD = 1.;
  for (int iJet = 1; iJet <= nJet; ++iJet, nJetD += 1.0) {
//     std::cout << "--->Before calcNJet" << std::endl;
    num *= calcP(&nJetD,polyPars);
//     std::cout << "----> num = " << num << std::endl;
  }
  if (nJet < 4) {
    num *= (1 - calcP(&nJetD,polyPars));
//     std::cout << "*---> num = " << num << std::endl;
  }

//   std::cout << "Final: x[0] = " << x[0]
//             << ", nJet = " << nJet
//             << ", num = " << num 
//             << std::endl;

  return num;

}
