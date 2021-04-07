// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstdlib>

// Root include files
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLine.h"
#include "TLegend.h"
#include "TText.h"

// CLP include files
#include "RootFileCollection/OptionUtils.h"
#include "CLPTemplateFitter/CLPTemplateFitter.hh"
#include "FCBinClass/FCBinClass.hh"
#include "CLPBlue/dout.hh"

using namespace std;
using namespace OptionUtils;

// forward declarations
void localInitialize();
void calcMeanRMS (double &mean, double &rms, double sum, double sum2,
                  int trials);
void rescale (const CLPTemplateFitter::DPairSet &input,
              CLPTemplateFitter::DPairSet &output,
              double scale = 1.);

// Assumes that outputPtr goes from 0.5 to N + 0.5
void addToHistogram (TH1F *outputPtr, TH1F *inputPtr,  
                     int offset, int rebin, float scale = 1.);

void addError (TH1F *outptuPtr, TH1F *inputPtr,
               double error1, double error2, int middle);

void corporate_id();

// Global variables (ugh!) for command line options
// which histogram to grab (histnumber)
int gHistNumber = 4; // 1 or 4
// (minverb)
int gMinuitVerbose = -1;
// value of BF to set to plot (plotbf)
double gPlotBF = 0.;
// fix the value of the antitag and tag background (antiback, tagback)
double gAntiBack = 0.;
double gTagBack  = 0.;
// do limit scan (doscan)
bool gDoScan = false;
// output root name (output)
string gOutputName = "";
// rebin the tagged and anti histograms (rebin1, rebin2)
int gRebin1 = 10;
int gRebin2 = 4;
// make extra plots (plot)
bool gMakePlot = false;
// fit chicago distributions (chicago)
bool gChicago   = false;
bool gChicagoZZ = false;
// float a single background shape (singleback)
bool gSingleBack = false;
// use 2 fb^-1 (2fb)
bool g2fb = true;
// choose which templates to use (whichtemp)
int gWhichTemplates = 0;


// Switch to tell us which sets of files to load (which)

int main (int argc, char** argv)
{
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);
   // The histograms inside are indexed from 0 to 5 with the following cuts:
   // 0: weight*(njets>=4)
   // 1: weight*(njets>=4)*wloose
   // 2: weight*(njets>=4)*(1-wloose)
   // 3: weight*(njets>=4)*(Et1>40)*(Et2>30)*(Et3>20)*(Et4>15)*(mt>200)
   // 4: weight*(njets>=4)*wloose*(Et1>40)*(Et2>30)*(Et3>20)*(Et4>15)*(mt>200)
   // 5: weight*(njets>=4)*(1-wloose)*(Et1>40)*(Et2>30)*(Et3>20)*(Et4>15)*
   //                      (mt>200) 
   double wzArray[8] = { 73.80, 40.30,  33.50,  66.44, 
                         37.13, 29.31 , 27.85,  21.98};
   double zzArray[8] = {247.08, 96.82, 150.26, 221.95, 
                        88.26, 133.69,  66.20, 100.27};

   const double kChicagoFraction = (8.8 / 15.95) * (1.27 / 1.12);
   //const double kChicagoFraction = 1.;
   int numbins1 = 100 / gRebin1;
   int numbins2 = 100 / gRebin2;
   int totalbins = numbins1 + numbins2;
   double denommu1 = -1.358;
   double denommu2 =  0.378;
   // gROOT->SetStyle("Plain");   
   corporate_id();
   gStyle->SetOptStat(0000);
   CLPTemplateFitter fitter;
   fitter.setMinuitVerboseLevel (gMinuitVerbose);
   TString histname1 = Form ("hsum_chi_%d", gHistNumber);
   TString histname2 = Form ("hsum_chi_%d", gHistNumber + 1);

   //////////
   // Data //
   //////////
   TFile *dataFile = 0;
   double lumiFactor = 1.;
   if (g2fb)
   {
      dataFile = TFile::Open ("histograms/data_1.7invfb.root");
      lumiFactor = 1.7 / 1.12;
   } else {
      dataFile = TFile::Open ("histograms/data_112.root");
   }
   TH1F *data1 = (TH1F*) dataFile->Get(histname1);
   TH1F *data2 = (TH1F*) dataFile->Get(histname2);

   TFile *zjetFile = 0;
   TFile *AwzFile  = 0;
   TFile *AzzFile  = 0;
   if (0 == gWhichTemplates)
   {
      // Old templates
      zjetFile = TFile::Open ("histograms/zjets.root");
      AwzFile  = TFile::Open ("histograms/Awz.root");
      AzzFile  = TFile::Open ("histograms/Azz.root");
   } else if (1 == gWhichTemplates) 
   {
      // new templates
      zjetFile = TFile::Open ("histograms/background_1.7invfb.root");
      AwzFile  = TFile::Open ("histograms/signal_1.7invfb.root");
      AzzFile  = TFile::Open ("histograms/zczc_1.7invfb.root");
   } else if (2 == gWhichTemplates) 
   {
      // new templates JES up
      zjetFile = TFile::Open ("histograms/background_1.7_jes_up.root");
      AwzFile  = TFile::Open ("histograms/signal_1.7_jes_up.root");
      AzzFile  = TFile::Open ("histograms/zczc_1.7_jes_up.root");
   } else if (3 == gWhichTemplates) 
   {
      // new templates JES down
      zjetFile = TFile::Open ("histograms/background_1.7_jes_down.root");
      AwzFile  = TFile::Open ("histograms/signal_1.7_jes_down.root");
      AzzFile  = TFile::Open ("histograms/zczc_1.7_jes_down.root");
   }


   //////////////
   // Z + Jets //
   //////////////
   TH1F *zjet1a = (TH1F*) zjetFile->Get(histname1);
   TH1F *zjet2a = (TH1F*) zjetFile->Get(histname2);

   ///////////////////
   // WZ Acceptance //
   ///////////////////
   TH1F *Awz1 = (TH1F*) AwzFile->Get(histname1);
   TH1F *Awz2 = (TH1F*) AwzFile->Get(histname2);

   ///////////////////
   // ZZ Acceptance //
   ///////////////////
   TH1F *Azz1 = (TH1F*) AzzFile->Get(histname1);
   TH1F *Azz2 = (TH1F*) AzzFile->Get(histname2);

   double Awz1Norm = wzArray[gHistNumber];
   double Awz2Norm = wzArray[gHistNumber + 1];
   double Azz1Norm = 0.589 * zzArray[gHistNumber];
   double Azz2Norm = 0.589 * zzArray[gHistNumber + 1];

   /////////////
   // Chicago //
   /////////////
   if (gChicago)
   {
      denommu1 = -1.10; 
      denommu2 =  0.130;
      gRebin1 = 1;
      gRebin2 = 1;
      TFile *chicagoFile = TFile::Open ("histograms/chicago.root");
      // electrons first, then muons
      data1  = (TH1F*) chicagoFile->Get ("dataElec");
      data2  = (TH1F*) chicagoFile->Get ("dataMuon");
      zjet1a = (TH1F*) chicagoFile->Get ("backElec");
      zjet2a = (TH1F*) chicagoFile->Get ("backMuon");
      Awz1   = (TH1F*) chicagoFile->Get ("fcncElec");
      Awz2   = (TH1F*) chicagoFile->Get ("fcncMuon");
      Azz1   = (TH1F*) Awz1->Clone("Azz1");
      Azz2   = (TH1F*) Awz2->Clone("Azz2");
      numbins1 = numbins2 = data1->GetNbinsX();
      totalbins = numbins1 / gRebin1 + numbins2 / gRebin2;
      double elecFrac = Awz1->Integral() / 
         (Awz1->Integral() + Awz2->Integral());
      double chicagoAwz = wzArray[1] * kChicagoFraction;
      double chicagoAzz = zzArray[1] * kChicagoFraction * 0.589;
      Awz1Norm = elecFrac       * chicagoAwz;
      Awz2Norm = (1 - elecFrac) * chicagoAwz;
      Azz1Norm = elecFrac       * chicagoAzz;
      Azz2Norm = (1 - elecFrac) * chicagoAzz;
   }

   Awz1Norm *= lumiFactor;
   Awz2Norm *= lumiFactor;
   Azz1Norm *= lumiFactor;
   Azz2Norm *= lumiFactor;

   /////////////////////////////
   // Assemble the histograms //
   /////////////////////////////
   // data
   TH1F *data = new TH1F ("data", "data", totalbins, 0.5, 0.5 + totalbins);
   addToHistogram (data, data1, 0, gRebin1);
   addToHistogram (data, data2, numbins1, gRebin2);
   // background
   TH1F *zjet1 = new TH1F ("zjet1", "zjet1", totalbins, 0.5, 0.5 + totalbins);
   TH1F *zjet2 = new TH1F ("zjet2", "zjet2", totalbins, 0.5, 0.5 + totalbins);
   addToHistogram (zjet1, zjet1a, 0,        gRebin1, 1. / zjet1a->Integral());
   addToHistogram (zjet2, zjet2a, numbins1, gRebin2, 1. / zjet2a->Integral());
   TH1F *zjet  = new TH1F ("zjet", "zjet", totalbins, 0.5, 0.5 + totalbins);
   double norm = zjet1a->Integral() + zjet2a->Integral();
   addToHistogram (zjet,  zjet1a, 0,        gRebin1, 1. / norm);
   addToHistogram (zjet,  zjet2a, numbins1, gRebin2, 1. / norm);
   // WZ acceptance
   TH1F *Awz = new TH1F ("Awz", "Awz", totalbins, 0.5, 0.5 + totalbins);
   addToHistogram (Awz, Awz1, 0,        gRebin1, 
                   Awz1Norm / Awz1->Integral());
   addToHistogram (Awz, Awz2, numbins1, gRebin2, 
                   Awz2Norm / Awz2->Integral());
   // ZZ acceptance
   TH1F *Azz = new TH1F ("Azz", "Azz", totalbins, 0.5, 0.5 + totalbins);
   addToHistogram (Azz, Azz1, 0,        gRebin1, 
                   Azz1Norm / Azz1->Integral());
   addToHistogram (Azz, Azz2, numbins1, gRebin2, 
                   Azz2Norm / Azz2->Integral());

   //////////////////////
   // Setup the colors //
   //////////////////////
   // data
   data->SetMarkerStyle (20);
   data->SetMarkerSize(1.5);
   data->SetMarkerColor (26);
   // backgrond
   zjet->SetFillColor (38);
   zjet->SetLineColor (kBlack);
   zjet->SetLineWidth (2);
   zjet1->SetFillColor (38);
   zjet1->SetLineColor (kBlack);
   zjet1->SetLineWidth (2);
   zjet2->SetFillColor (kYellow);
   zjet2->SetLineColor (kBlack);
   zjet2->SetLineWidth (2);
   // FCNC acceptance
   Awz->SetFillColor (kRed);
   Awz->SetLineColor (kBlack);
   Awz->SetLineWidth (2);
   Azz->SetFillColor (kRed);
   Azz->SetLineColor (kRed);
   Azz->SetLineWidth (2);

   //////////////////////
   // Setup the fitter //
   //////////////////////
   fitter.addData (data);
   int zjet1Index = 0; int zjet2Index = 0;
   if (! gSingleBack)
   {
      zjet1Index = fitter.addTemplate ("zjet1", zjet1, data1->Integral());
      zjet2Index = fitter.addTemplate ("zjet2", zjet2, data2->Integral());
   } else {
      zjet1Index = fitter.addTemplate ("zjet", zjet, 
                                       data1->Integral() + data2->Integral());
   }
   if (gTagBack)
   {
      fitter.setParameter ("zjet1", gTagBack);
      fitter.fixParameter ("zjet1");
   }
   if (gAntiBack)
   {
      fitter.setParameter ("zjet2", gAntiBack);
      fitter.fixParameter ("zjet2");
   }
   int AwzIndex = fitter.addTemplate ("Awz", Awz, 1); 
   int AzzIndex = fitter.addTemplate ("Azz", Azz, 1);
   fitter.fixParameter ("Awz");
   fitter.fixParameter ("Azz");

   /////////////
   // BinNorm //
   /////////////
   BinNormClass::ISet allBinSet, fcncTempSet;
   fcncTempSet.insert (AwzIndex);
   fcncTempSet.insert (AzzIndex);
   BinNormClass::insertRangeIntoSet (allBinSet, 1, totalbins);
   BinNormClass fcncNorm (fcncTempSet, allBinSet, 0.06);
   fcncNorm.setFunctionParam (AwzIndex, 0,  2.);
   fcncNorm.setFunctionParam (AwzIndex, 1, -2.);
   fcncNorm.setFunctionParam (AwzIndex, 3, denommu1);
   fcncNorm.setFunctionParam (AwzIndex, 4, denommu2);   
   fcncNorm.setFunctionParam (AzzIndex, 1,  1.);
   fcncNorm.setFunctionParam (AzzIndex, 3, denommu1);
   fcncNorm.setFunctionParam (AzzIndex, 4, denommu2);   
   fitter.addBinNorm ("fcncBF", fcncNorm);
   //cout << "fitter: " << endl << fitter << endl;
   CLPTemplateFitter::DVec paramVec;
   paramVec.push_back (6);
   paramVec.push_back (8);
   paramVec.push_back (1);
   paramVec.push_back (1);
   paramVec.push_back (0.06);
   fitter._initializeMinuit();
   double value = CLPTemplateFitter::minimizeFcnValue (paramVec);
   cout << "value " << value << endl;
   //return 0;
   fitter.fit();
   fitter.outputFitResults();
   // fitter.fitEverythingBut ("fcncBF", -0.01);
   // fitter.outputFitResults();

   /////////////////////
   // After Fit Plots //
   /////////////////////
   if (gMakePlot)
   {
      TH1F *AwzAfter1   = fitter.updatedHistogram ("Awz");
      TH1F *AzzAfter1   = fitter.updatedHistogram ("Azz");
      AwzAfter1->Add (AzzAfter1);
      double fcnccount1 = AwzAfter1->Integral();
      if (gChicago)
      {
         AwzAfter1->SetTitle ("Fit to Top Mass Distributions (Best Fit)");
      } else {
         AwzAfter1->SetTitle ("Fit to Mass #chi^{2} Distributions (Best Fit)");
      }
      TH1F *fcnc = (TH1F*) AwzAfter1->Clone("fcnc");
      TH1F *zjet1After1 = 0, *zjet2After1 = 0, *zjetAfter1 = 0;
      if (gSingleBack)
      {
         zjetAfter1  = fitter.updatedHistogram ("zjet");
         AwzAfter1->Add (zjetAfter1);
      } else {
         zjet1After1 = fitter.updatedHistogram ("zjet1");
         zjet2After1 = fitter.updatedHistogram ("zjet2");
         AwzAfter1->Add (zjet1After1);
         AwzAfter1->Add (zjet2After1);
      }
      double ymax = AwzAfter1->GetMaximum();
      double datamax = data->GetMaximum();
      if (datamax > ymax) ymax = datamax;
      ymax *= 1.8;
      AwzAfter1->SetMaximum( ymax);
      TCanvas *c1 = new TCanvas;
      TAxis *aPtr = AwzAfter1->GetXaxis();
      if (! gChicago)
      {
         aPtr->SetBinLabel ( 20 / gRebin1,  "2");
         aPtr->SetBinLabel ( 40 / gRebin1,  "4");
         aPtr->SetBinLabel ( 60 / gRebin1,  "6");
         aPtr->SetBinLabel ( 80 / gRebin1,  "8");
         // aPtr->SetBinLabel (100 / gRebin1, "10");
         aPtr->SetBinLabel (numbins1 +  20 / gRebin2,  "2");
         aPtr->SetBinLabel (numbins1 +  40 / gRebin2,  "4");
         aPtr->SetBinLabel (numbins1 +  60 / gRebin2,  "6");
         aPtr->SetBinLabel (numbins1 +  80 / gRebin2,  "8");
         aPtr->SetBinLabel (numbins1 + 100 / gRebin2, "10");
      }
      aPtr->SetTitle ("#sqrt{#chi^{2}}");
      AwzAfter1->Draw("hist");
      if (gSingleBack)
      {
         zjetAfter1->Draw("hist same");
      } else {
         zjet1After1->Draw("hist same");
         zjet2After1->Draw("hist same");
      }
      data->Draw("same p hist");
      TLine *vert = new TLine (numbins1 + 0.5, -0. * ymax, numbins1 + 0.5, ymax);
      vert->SetLineColor(kBlack);
      vert->SetLineWidth(2);
      vert->Draw();
      TLine *horz = new TLine ( 0.5, 0, 0.5 + totalbins, 0);
      horz->SetLineColor(kBlack);
      horz->SetLineWidth(2);
      horz->Draw(); 
      TText *tagged = 0, *antitagged = 0;
      if (gSingleBack)
      {
         tagged     = new TText (numbins1 / 2.,          ymax * 0.95, 
                                 "Electrons");
         antitagged = new TText (numbins1 + numbins2/2., ymax * 0.95, 
                                 "Muons");
      } else {
         tagged     = new TText (numbins1 / 2.,          ymax * 0.95, 
                                 "Tagged");
         antitagged = new TText (numbins1 + numbins2/2., ymax * 0.95, 
                                 "Anti-tagged");
      }
      tagged->SetTextColor (1);
      tagged->SetTextSize(0.05);
      tagged->SetTextAlign(22);
      antitagged->SetTextColor (1);
      antitagged->SetTextSize(0.05);
      antitagged->SetTextAlign(22);
      tagged->Draw("same");
      antitagged->Draw("same");
      TLegend *legend = new TLegend (0.55, 0.6, 0.85, 0.8); // x1, y1, x2, y2
      legend->SetFillColor(kWhite);
      legend->SetLineColor(kBlack);
      legend->SetLineWidth(2);
      legend->SetBorderSize(1);
      legend->SetTextFont (132);
      legend->AddEntry (data,        "Data", "p");
      legend->AddEntry (AwzAfter1,   "FCNC", "f");
      if (gSingleBack)
      {
         legend->AddEntry (zjetAfter1, "Background", "f");
      } else {
         legend->AddEntry (zjet1After1, "Tagged Background", "f");
         legend->AddEntry (zjet2After1, "Anti-tagged Background", "f");
      }
      legend->Draw("same");
      c1->Print ( "templateBest.eps" );

      //////////////////////////
      // More After Fit Plots //
      //////////////////////////
      fitter.fitEverythingBut ("fcncBF", gPlotBF);
      fitter.outputFitResults();
      TH1F *AwzAfter2   = fitter.updatedHistogram ("Awz");
      TH1F *AzzAfter2   = fitter.updatedHistogram ("Azz");
      TH1F *zjet1After2 = 0, *zjet2After2 = 0, *zjetAfter2 = 0;
      if (gSingleBack)
      {
         zjetAfter2  = fitter.updatedHistogram ("zjet");
         AwzAfter2->Add (zjetAfter2);
      } else {
         zjet1After2 = fitter.updatedHistogram ("zjet1");
         zjet2After2 = fitter.updatedHistogram ("zjet2");
         AwzAfter2->Add (zjet1After2);
         AwzAfter2->Add (zjet2After2);
      }
      cout << "Awz " << AwzAfter2->Integral()
           << " Azz " << AzzAfter2->Integral() << endl;
      AwzAfter2->Add (AzzAfter2);
      double fcnccount = AwzAfter2->Integral();
      if (gChicago)
      {
         AwzAfter2->SetTitle ("Top Mass Distributions");
      } else {
         AwzAfter2->SetTitle ( Form ("Mass #chi^{2} Fit (%.1f%% UL)", 
                                     100 * gPlotBF) );
      }
      double supermax = ymax * 0.8;
      AwzAfter2->SetMaximum( supermax );
      aPtr = AwzAfter2->GetXaxis();
      if (! gChicago)
      {
         aPtr->SetBinLabel ( 20 / gRebin1,  "2");
         aPtr->SetBinLabel ( 40 / gRebin1,  "4");
         aPtr->SetBinLabel ( 60 / gRebin1,  "6");
         aPtr->SetBinLabel ( 80 / gRebin1,  "8");
         //aPtr->SetBinLabel (100 / gRebin1, "10");
         aPtr->SetBinLabel (numbins1 +  20 / gRebin2,  "2");
         aPtr->SetBinLabel (numbins1 +  40 / gRebin2,  "4");
         aPtr->SetBinLabel (numbins1 +  60 / gRebin2,  "6");
         aPtr->SetBinLabel (numbins1 +  80 / gRebin2,  "8");
         aPtr->SetBinLabel (numbins1 + 100 / gRebin2, "10"); 
         aPtr->SetTitle ("#sqrt{#chi^{2}}");
      }
      AwzAfter2->Draw("hist");
      if (gSingleBack)
      {
         zjetAfter2->Draw("hist same");
      } else {
         zjet1After2->Draw("hist same");
         zjet2After2->Draw("hist same");
      }
      fcnc->SetFillStyle(0);
      fcnc->SetLineColor (kRed);
      fcnc->Scale(3);
      if (! gPlotBF)
      {
         fcnc->Draw("same hist");
      }
      data->Draw("same p hist");
      TH1F *dataToo = (TH1F*) data->Clone("data2");
      dataToo->SetMarkerColor(kBlack);
      dataToo->SetMarkerStyle(4);
      dataToo->Draw("same p hist");
      vert = new TLine (numbins1 + 0.5, -0. * supermax, numbins1 + 0.5, 
                        supermax);
      vert->SetLineColor(kBlack);
      vert->SetLineWidth(2);
      vert->Draw();
      horz->Draw();
      if (gSingleBack)
      {
         tagged     = new TText (numbins1 / 2.,          ymax * 0.55, 
                                 "Electrons");
         antitagged = new TText (numbins1 + numbins2/2., ymax * 0.55, 
                                 "Muons");
      } else {
         tagged     = new TText (numbins1 / 2.,          ymax * 0.55, 
                                 "Tagged");
         antitagged = new TText (numbins1 + numbins2/2., ymax * 0.55, 
                                 "Anti-tagged");
      }
      tagged->SetTextColor (1);
      tagged->SetTextSize(0.05);
      tagged->SetTextAlign(22);
      antitagged->SetTextColor (1);
      antitagged->SetTextSize(0.05);
      antitagged->SetTextAlign(22);
      tagged->Draw("same");
      antitagged->Draw("same");
      // TText *preliminary = new TText (numbins1 + numbins2 / 2.,  ymax * 1.1, 
      //                                 "CDF II Preliminary 1.12 fb^-1");
      // preliminary->SetTextSize(0.05);
      // preliminary->SetTextAlign(22);
      // preliminary->Draw("same");
      legend = new TLegend (0.65, 0.7, 0.9, 0.9); // x1, y1, x2, y2
      legend->SetFillColor(kWhite);
      legend->SetLineColor(kBlack);
      legend->SetLineWidth(2);
      legend->SetTextFont (132);
      legend->SetBorderSize(1);
      legend->AddEntry (data,        "Data", "p");
      legend->AddEntry (AwzAfter2,   "FCNC", "f");
      if (gSingleBack)
      {
         legend->AddEntry (zjetAfter2, "Background", "f");
      } else
      {
         legend->AddEntry (zjet1After2, "Tagged Background", "f");
         legend->AddEntry (zjet2After2, "Anti-tagged Background ", "f");
      }
      legend->Draw("same");
      c1->Print ( "template.eps" );
      cout << "1 " << fcnccount1 << " " << fcnccount 
           << " data " << data->Integral()<< endl;
   } // if make extra plot 
   if (gDoScan)
   {
      fitter.fitEverythingBut ("fcncBF", -0.01);
      CLPTemplateFitter::DPairSet pairSet, expPairSet;
      fitter.scanVariable (pairSet, "fcncBF", 0, 0.30, 120);   
      rescale (pairSet, expPairSet);
      double ninetyfive = FCBinClass::BayesUpperLimit (expPairSet);
      cout << "95% " << ninetyfive << endl;
   }
}

void calcMeanRMS (double &mean, double &rms, double sum, double sum2,
                  int trials)
{
   mean = sum / trials;
   double aveX2 = sum2 / trials;
   double mean2 = mean * mean;
   if (aveX2 < mean2)
   {
      rms = 0.;
   } else {
      rms = sqrt (aveX2 - mean * mean);
   }
}

void rescale (const CLPTemplateFitter::DPairSet &input,
              CLPTemplateFitter::DPairSet &output,
              double scale)
{
   output.clear();
   for (CLPTemplateFitter::DPairSetConstIter iter = input.begin();
        input.end() != iter;
        ++iter)
   {
      double xval = iter->first * scale;
      double yval = exp( iter->second * -0.5 );
      output.insert( make_pair( xval, yval ) );
   }
}

void addToHistogram (TH1F *outputPtr, TH1F *inputPtr,  
                     int offset, int rebin, float scale)
{
   int numBins = inputPtr->GetNbinsX();
   for (int bin = 1; bin <= numBins; ++bin)
   {
      int tobin = (bin - 1) / rebin + 1 + offset;
      //cout << setw(2) << bin << " to " << setw(2) << tobin << endl;
      outputPtr->Fill( (float) tobin, scale * inputPtr->GetBinContent(bin) );
   }
   
}

void addError (TH1F *outputPtr, TH1F *inputPtr,
               double error1, double error2, int middle)
{
   int numbins = inputPtr->GetNbinsX();
   double error = error1;
   for (int bin = 1; bin <= numbins; ++bin)
   {
      if (bin == middle)
      {
         error = error2;
      }
      double cont = outputPtr->GetBinContent (bin);
      double err  = outputPtr->GetBinError (bin);
      cont += inputPtr->GetBinContent (bin);
      err  += inputPtr->GetBinContent (bin) * error;
      outputPtr->SetBinContent (bin, cont);
      outputPtr->SetBinError   (bin, err);
   } // for bin
}


void localInitialize()
{
   // option name is case insensitive.  Capitalization of the strings
   // is for readability only.
   addOptionKey ("histnumber", gHistNumber);
   addOptionKey ("minverb",    gMinuitVerbose);
   addOptionKey ("plotbf",     gPlotBF);
   addOptionKey ("doscan",     gDoScan);
   addOptionKey ("antiback",   gAntiBack);
   addOptionKey ("tagback",    gTagBack);
   addOptionKey ("output",     gOutputName);
   addOptionKey ("rebin1",     gRebin1);
   addOptionKey ("rebin2",     gRebin2);
   addOptionKey ("chicago",    gChicago);
   addOptionKey ("chicagozz",  gChicagoZZ);
   addOptionKey ("plot",       gMakePlot);
   addOptionKey ("singleBack", gSingleBack);
   addOptionKey ("2fb",        g2fb);
   addOptionKey ("whichtemp",  gWhichTemplates,
      "Which template: 0 - old, 1 - new, 2 - JES up, 3 - JES down");
}

