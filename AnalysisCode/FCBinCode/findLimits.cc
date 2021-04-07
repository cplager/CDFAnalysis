// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStyle.h"
#include "RootFileCollection/OptionUtils.h"
#include "FCBinClass/FCBinClass.hh"
#include "FCBinClass/dout.hh"

using namespace std;

int main(int argc, char** argv)
{
   FCBinClass::FCBinFirstCall();
   OptionUtils::parseArguments (argc, argv);
   //OptionUtils::setVariablesFromFile ("options.txt");
   // After EVERYTHING (but trueValue) is set,
   // we initialize the class
   FCBinClass::initialize();   
   // Note: AFter this point, if you were to update a background or an
   // acceptance, but NOT the number of dimensions or one of the 'max'
   // variables, you can/need to do a 'FCBinClass::updateAll();'

   // FCBinClass::testDifferences();

   if (FCBinClass::makePlots())
   {
      gROOT->SetStyle("Plain");
      gStyle->SetOptStat(0000);
   }
   
   // The 'find...Limit' functions use the default confidence limit
   // (sm_confidenceLevel) unless otherwise set
   double upper;
   if (FCBinClass::kRootFeldmanCousinsMode == FCBinClass::mode())
   {
      upper = FCBinClass::findTFCUpperLimit ();
   } else if (FCBinClass::kFeldmanCousinsMode == FCBinClass::mode()) 
   {
      upper = FCBinClass::findUpperLimit();
      if (FCBinClass::makePlots() && (1 == FCBinClass::numDim()))
      {
         FCBinClass::setTrueValue (upper);
         FCBinClass::setFastMode (true);
         TH1F* fast = FCBinClass::getPEhist();
         fast->SetLineColor (kBlue);
         FCBinClass::setFastMode (false);
         TH1F* slow = FCBinClass::getPEhist("peHistFast");
         slow->SetLineColor (kRed);
         TCanvas *c1 = new TCanvas;
         slow->Draw();
         fast->Draw("SAME");
         string name = "numObsv.eps";
         c1->Print(name.c_str());         
      }
   } else {
      FCBinClass::setFastMode (true);
      upper = FCBinClass::BayesUpperLimit ();
      cout << "Upper Limit " << FCBinClass::limitModeName() << " : "
           << Form ("%.4f", upper) << endl;
      FCBinClass::setFastMode (false);
      upper = FCBinClass::BayesUpperLimit ();
      cout << "Upper Limit " << FCBinClass::limitModeName() << " : "
           << Form ("%.4f", upper) << endl;      
      if (FCBinClass::makePlots())
      {
         FCBinClass::setUserDouble (0, 1);
         TGraph *slow = FCBinClass::probabilityGraph();
         slow->SetMarkerStyle(20);
         slow->SetMarkerSize(0.1);
         slow->SetMarkerColor(kRed);
         slow->SetLineColor (kRed);
         FCBinClass::setFastMode (true);
         TGraph *fast = FCBinClass::probabilityGraph (true);
         fast->SetMarkerStyle(20);
         fast->SetMarkerSize(0.1);
         fast->SetMarkerColor(kBlue);
         fast->SetLineColor (kBlue);
         TCanvas *c1 = new TCanvas;
         slow->Draw("ALP");
         fast->Draw("LP SAME");
         string name = "likely.eps";
         c1->Print(name.c_str());
      } // if make plots
      if (FCBinClass::userBool(0))
      {
         FCBinClass::testDifferences();
      }
      return 0;
   } // else
   cout << "Upper Limit " << FCBinClass::limitModeName() << " : "
        << Form ("%.4f", upper) << endl;
   return 0;
}
