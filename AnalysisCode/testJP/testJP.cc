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
#include "TLine.h"

// CLP include files
#include "JointPhysics/JPLumiReweighting.hh"
#include "JointPhysics/dumpSTL.icc"
#include "RootFileCollection/OptionUtils.h"

using namespace std;

// forward declarations
void localInitialize();

// global variables

// number of PEs (numPEs)
int gNumPes = 0;

// weight of P10-12 MC (1012MC)
double g1012MC = 0.10;

// weight of new MC (newmc)
double gNewMC = 0.30;

// weight of p13 data (p13)
double gP13 = 1.75;

// weight of golden data (golden)
double gGolden = 0.75;

// string name (prefix)
string gPrefix = "weights";

const double kP13Total = 800;

int main (int argc, char** argv)
{
   // Silly root things
   delete gRandom;
   gRandom = new TRandom3;
   gROOT->SetStyle ("Plain");
   gStyle->SetOptStat(0);
   // parce command line arguments
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);
   JPLumiReweighting reweight;
   reweight.loadDataInfo ("data.profile");
   reweight.loadMCInfo   ("mc.profile");
   reweight.setMCIntLumForPeriod ("MC10-12", g1012MC);
   reweight.setMCIntLumForPeriod ("goldenMC", gNewMC);
   reweight.setDataIntLumForPeriod ("P13-17", gP13 * kP13Total);
   reweight.setDataIntLumForPeriod ("Golden", gGolden * kP13Total);
   reweight.recalculate();
   // reweight.setSeedFromFilename ("one/two/three");
   string title = Form ("%s_p13-%0.1f_g-%0.1f_10mc-%0.1f_newmc-%0.1f",
                        gPrefix.c_str(), gP13, gGolden, g1012MC, gNewMC);
   TH1F *weights = JPLumiReweighting::TH1FfromDVec (reweight.weightVec(),
                                                    "weights",
                                                    title.c_str());
   TCanvas c1;
   weights->Draw();
   TLine *line = new TLine (0, 1, JPLumiReweighting::maxInteractions() + 1, 1);
   line->SetLineColor (kRed);
   line->SetLineWidth (2);
   line->SetLineStyle (2);
   line->Draw("same");
   c1.Print( (title + ".eps").c_str() );
   // TH1F *data = 
   //    JPLumiReweighting::TH1FfromDVec (reweight.dataNumIntProfile(),
   //                                     "dataNInt");
   // data->Draw();
   // c1.Print ("data.eps");
   cout << endl << "weights: ";
   dumpSTL (reweight.weightVec());
   cout << endl << endl;
   cout << "data: ";
   dumpSTL (reweight.dataNumIntProfile());
   cout << endl;
   
   if (! gNumPes)
   {
      return 0;
   }

   
   TH1F *mcNumIntHistPtr = 
      JPLumiReweighting::TH1FfromDVec (reweight.mcNumIntProfile(),
                                       "mcNumInt");
   for (int pesLoop = 0; pesLoop < gNumPes; ++pesLoop)
   {
      int numInt = (int) mcNumIntHistPtr->GetRandom();
      reweight.weight (numInt);
   } // for pesLoop
   cout << endl << endl << reweight << endl;
   return 0;
}

void localInitialize()
{
   OptionUtils::addOptionKey ("numPEs", gNumPes);
   OptionUtils::addOptionKey ("1012MC", g1012MC);
   OptionUtils::addOptionKey ("newMC",  gNewMC);
   OptionUtils::addOptionKey ("P13",    gP13);
   OptionUtils::addOptionKey ("golden", gGolden);
   OptionUtils::addOptionKey ("prefix", gPrefix);   
}
