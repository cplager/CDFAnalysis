// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLine.h"
#include "FCBinClass/FCBinClass.hh"
#include "FCBinClass/dout.hh"

using namespace std;

int main(int argc, char** argv)
{
   // setup interal variables based on command line arguments
   FCBinClass::parseCommandLineArguments (argc, argv);
   // After EVERYTHING (but trueValue) is set,
   // we initialize the class
   FCBinClass::initialize();   
   // Note: AFter this point, if you were to update a background or an
   // acceptance, but NOT the number of dimensions or one of the 'max'
   // variables, you can/need to do a 'FCBinClass::updateAll();'

   // The 'find...Limit' functions use the default confidence limit
   // (sm_confidenceLevel) unless otherwise set
   TGraph graph = FCBinClass::probabilityGraph();
   TCanvas *c1 = new TCanvas;
   graph.SetMarkerStyle(20);
   graph.SetMarkerSize(0.1);
   graph.SetMarkerColor(kRed);
   graph.Draw("AP");
   string name = "likely.eps";
   c1->Print(name.c_str());
   double value = FCBinClass::BayesUpperLimit(0.75);
   cout << "value = " << value << endl << endl;
   return 0;
}
