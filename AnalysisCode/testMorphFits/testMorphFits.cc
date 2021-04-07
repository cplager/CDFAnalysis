// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cmath>

// Root include files
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TH2F.h"
#include "TLegend.h"

// CLP include files
#include "CLPTemplateFitter/CLPTemplateFitter.hh"
#include "CLPTemplateMorph/CLPTemplateMorph.hh"
#include "CLPTemplateMorph/dout.hh"
#include "RootFileCollection/OptionUtils.h"

using namespace std;

void localInitialize();

// number of bins (bins)
int gBins = 100;
// factor of extra bins (increase)
int gIncrease = 10;
// mean and width of 'data' gaussian (mean, width)
double gMean  = 37;
double gWidth =  7.2;
// (minverb)
int gMinuitVerbose = -1;
// remove points (removepts)
bool gRemovePoints = false;
// smooth beginning of delta CDFs (smooth)
bool gSmooth = false;

int main (int argc, char** argv)
{
   // parce command line arguments
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.3);          // height of box
   gStyle->SetStatW(0.15);         // width of box 
   // default histogram
   TH1F gaus45_5  ("gaus45_5",  "gaus45_5",  gIncrease * gBins, 0, 100);
   //  wide histogram
   TH1F gaus45_10 ("gaus45_10", "gaus45_10", gIncrease * gBins, 0, 100);
   TH1F gaus50_10 ("gaus50_10", "gaus50_10", gIncrease * gBins, 0, 100);
   // left and right shifted histograms
   TH1F gaus30_5  ("gaus30_5",  "gaus30_5",  gIncrease * gBins, 0, 100);
   TH1F gaus60_5  ("gaus60_5",  "gaus60_5",  gIncrease * gBins, 0, 100);
   // our "data" histogram
   TH1F dataHist  ("dataHist",  "dataHist" , gBins, 0, 100);
   delete gRandom;
   gRandom = new TRandom3;
   for (int loop = 0; loop < 1000000; ++loop)
   {
      // default
      gaus45_5.Fill ( gRandom->Gaus (45., 5.) );
      // left and right shifted
      gaus30_5.Fill ( gRandom->Gaus (30., 5.) );
      gaus60_5.Fill ( gRandom->Gaus (60., 5.) ); 
      // wider
      gaus45_10.Fill( gRandom->Gaus (45., 10.) );
      gaus50_10.Fill( gRandom->Gaus (50. + 0.25, 10.) );
      dataHist.Fill ( gRandom->Gaus (gMean, gWidth) );
   } // for loop
   CLPTemplateMorph morph ("morph", "morph", gBins, 0, 100);
   morph.addDefaultTemplate (&gaus45_5);
   morph.addMorphVariable ("center", 45.);
   morph.addMorphVariable ("width", 5.);
   // add center templates
   morph.addMorphTemplate ("center", 30, &gaus30_5);
   morph.addMorphTemplate ("center", 60, &gaus60_5);
   // add width template
   morph.addMorphTemplate ("width", 10, &gaus45_10);

   TCanvas *c1 = new TCanvas;

   // CLPTemplateFitter fitter;
   // fitter.setMinuitVerboseLevel (gMinuitVerbose);
   // fitter.addData (&dataHist);
   // fitter.addTemplate ("gauss", &morph, 1);
   // BinNormClass centerNorm (45.), widthNorm (5.);
   // fitter.addBinNorm ("center", centerNorm);
   // fitter.addBinNorm ("width", widthNorm);
   // fitter.fit();
   // fitter.outputFitResults();
   // TH1F *afterHptr = fitter.updatedHistogram ("gauss");
   // afterHptr->SetLineColor (kRed);
   // dataHist.SetMarkerStyle(20);
   // afterHptr->Draw();
   // dataHist.Draw("same p hist");
   // c1->Print ( Form ("gaus_%.1f_%.1f.eps", gMean, gWidth) );
   gStyle->SetOptStat (0);
   gaus45_5.SetTitle ("Templates");
   gaus45_5.SetLineWidth(2);
   gaus30_5.SetLineWidth(2);
   gaus45_10.SetLineWidth(2);
   gaus45_5. Rebin (gIncrease);
   gaus30_5. Rebin (gIncrease);
   gaus45_10.Rebin (gIncrease);
   gaus30_5.SetLineColor(kBlue);
   gaus45_10.SetLineColor(kRed);
   gaus45_5.Draw();
   gaus30_5.Draw("same");
   gaus45_10.Draw("same");
   TLegend legend (0.60, 0.50, 0.90, 0.80);
   legend.SetFillStyle(4000); // transparent
   legend.AddEntry (&gaus45_5, "45 #pm 5", "l");
   legend.AddEntry (&gaus30_5, "30 #pm 5", "l");
   legend.AddEntry (&gaus45_10, "45 #pm 10", "l");
   legend.Draw("same");
   // c1->Print ("templates.eps");
   TH1F square ("square", "square", gIncrease * gBins, 0, 100);
   TH1F circle ("circle", "circle", gIncrease * gBins, 0, 100);
   TH1F gauss  ("gauss",  "gauss",  gIncrease * gBins, 0, 100);
   double mean = gIncrease * gBins / 2.;
   double width = gIncrease * gBins / 10.;
   double max = 4000;
   //double gaussMax = max * (100 / sqrt(2.));
   double gaussMax = 280 * max / 3.14159265;
   double prevValue = max * TMath::Erf (- mean / width);
   for (int bin = 1; bin <= gIncrease * gBins; ++bin)
   {
      double value = (bin - mean) / width;
      double intValue = gaussMax * TMath::Erf (value);
      gauss.SetBinContent (bin, std::max (0., intValue - prevValue));
      prevValue = intValue;
   } // for bin
   int left = 20 * gIncrease - 1;
   int right = 80 * gIncrease + 1;
   for (int bin = left; bin <= right; ++bin)
   {
      double angle = 3.14126535 * (bin - left) / (right - left);
      circle.SetBinContent (bin, max * sin (angle));
   }
   left = 20 * gIncrease + 1; right = 80 * gIncrease;
   for (int bin = left; bin <= right; ++bin)
   {
      square.SetBinContent (bin, max);      
   }
   CLPTemplateMorph cirSqu ("cirSqu", "cirSqu", gBins, 0, 100);
   cirSqu.setDontRemovePoints (! gRemovePoints);
   cirSqu.setSmoothDeltas (gSmooth);
   cirSqu.addDefaultTemplate (&square);  
   cirSqu.addMorphVariable ("shape", 0.);
   cirSqu.addMorphTemplate ("shape", 1., &circle);
   cirSqu.addMorphTemplate ("shape", 2., &gauss);
   // CLPTemplateMorph::TGraphPtrVec tgraphPtrVec;
   // cirSqu.allTGraphs (tgraphPtrVec);
   // int index = 0;
   // for (CLPTemplateMorph::TGraphPtrVecConstIter iter = tgraphPtrVec.begin();
   //      tgraphPtrVec.end() != iter;
   //      ++iter)
   // {
   //    TCanvas *c2 = new TCanvas;
   //    (*iter)->Draw("ALP");
   //    c2->Print( Form ("bincont%d.eps", ++index) );
   //    delete c2;
   // } // for iter
   CLPTemplateMorph::DVec paramVec;
   paramVec.push_back(0);
   for (double value = 0; value <= 2.01; value += 0.05)
   {
      paramVec[0] = value;
      cirSqu.setParamVec (paramVec);
      cirSqu.SetTitle ( Form( "Square - Circle - Gaussian %.2f", value) );
      cirSqu.Draw();
      c1->Print (Form ("cirSqu_%.2f.eps", value));
   }
   // cout << cirSqu << endl;
   square.Rebin (gIncrease);
   circle.Rebin (gIncrease);
   gauss.Rebin  (gIncrease);
   gaus50_10.Rebin (gIncrease);
   circle.SetLineColor (kRed);
   gauss.SetLineColor  (kBlue);
   square.Draw();
   circle.Draw("same");
   gauss.Draw("same");
   gaus50_10.Draw("same");
   c1->Print ("sqauare-circle-gauss.eps");
   return 0;
}

void localInitialize()
{
   OptionUtils::addOptionKey ("bins",      gBins);
   OptionUtils::addOptionKey ("increase",  gIncrease);
   OptionUtils::addOptionKey ("mean",      gMean);
   OptionUtils::addOptionKey ("width",     gWidth);
   OptionUtils::addOptionKey ("minverb",   gMinuitVerbose);
   OptionUtils::addOptionKey ("removepts", gRemovePoints);
   OptionUtils::addOptionKey ("smooth",    gSmooth);
   // OptionUtils::addOptionKey ("",     g);
   
}
