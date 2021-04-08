// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLine.h"
#include "FCBinClass/FCBinClass.hh"
#include "FCBinClass/dout.hh"

using namespace std;

int main(int argc, char** argv)
{
   FCBinClass::parseCommandLineArguments (argc, argv);
   // After EVERYTHING (but trueValue) is set,
   // we initialize the class
   FCBinClass::initialize();   
   // Note: AFter this point, if you were to update a background or an
   // acceptance, but NOT the number of dimensions or one of the 'max'
   // variables, you can/need to do a 'FCBinClass::updateAll();'
   // O.k. set the true value to 0 so we only have background
   float start =  9.0;
   float end   = 10.98;
   float step  =  0.1;
   if (FCBinClass::userDouble(0) > 0. && FCBinClass::userDouble(1) > 0.)
   {
      start = FCBinClass::userDouble(0);
      end   = FCBinClass::userDouble(1);
   }
   if (FCBinClass::userDouble(2) > 0.)
   {
      step = FCBinClass::userDouble(2);
   }
   if (start >= end)
   {
      cerr << "invalid parameters" << endl;
      return 1;
   }
   string name = "plot";
   if (FCBinClass::userString(0).length())
   {
      name = FCBinClass::userString(0);
   }
   name += Form("_%g_to_%g_by_%g", start, end, step);
   if (!FCBinClass::fastMode())
   {
      name += Form ("_%dpes", FCBinClass::numPEs());
   }
   name += ".eps";
   cout << name << endl;
   TGraph* graph = FCBinClass::getTGraph (start, end, step);
   TCanvas *c1 = new TCanvas;
   graph->SetMarkerStyle(22);
   graph->Draw("AP");
   c1->Print(name.c_str());
   return 0;
}
