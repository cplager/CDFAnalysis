// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include "RootFileCollection/OptionUtils.h"
#include "FCBinClass/FCBinClass.hh"
#include "FCBinClass/dout.hh"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"

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
   double tooBig; // return value for too big value of upper limit
   double RMS;
   TH1F *expectedHistPtr = 0;
   if (FCBinClass::makePlots())
   {
      expectedHistPtr = new TH1F ("Expected", "Expected Limit Distribution",
                                  50, 0, 0.3);
   }
   
   double lim = FCBinClass::findExpectedLimit (tooBig, RMS, expectedHistPtr);
   cout << "Expected Limit " << FCBinClass::limitModeName() << " : "
        << Form ("%.4f", lim) << ", " 
        << Form ("%.4f", tooBig) << endl;
   if (FCBinClass::makePlots())
   {
      gROOT->SetStyle("Plain");
      TCanvas *c1 = new TCanvas;
      expectedHistPtr->Draw();
      c1->Print ("expected.eps");
      TFile *filePtr = TFile::Open("expectedlimitdist.root", "recreate");
      expectedHistPtr->Write();
      filePtr->Close();
   }
}
