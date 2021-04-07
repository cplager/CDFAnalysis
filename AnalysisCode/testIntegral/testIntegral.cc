// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
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
#include "CLPTemplateFitter/CLPTrapezoid.hh"
#include "CLPTemplateFitter/dout.hh"
#include "RootFileCollection/OptionUtils.h"

using namespace std;

void localInitialize();

// mode (mode)
int gMode = 3;
// limit percent (percent)
double gPercent = 0.95;
// x value of peak (xpeak)
double gXpeak = 0.9;

int main (int argc, char** argv)
{
   // parce command line arguments
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);
   gROOT->SetStyle("Plain");
   CLPTrapezoid::Vec curve;
   for (double xval = 0; xval <= 1.0001; xval += 0.01)
   {
      if (0 == gMode)
         curve.push_back( CLPTrapezoid (xval, std::max (0., xval)) );
      else if (1 == gMode)
         curve.push_back( CLPTrapezoid (xval, std::max (0., 1. - xval)) );
      else if (2 == gMode)
         curve.push_back( CLPTrapezoid (xval, std::max (0., 
                                                        std::min(1 - xval,
                                                                 xval)) ));
      else
      {
         double height;
         if (xval > gXpeak) 
         {
            // negative slope side
            height = (1 - xval) / (1 - gXpeak);
         } else {
            // positive slope side
            height = xval / gXpeak;
         }
         curve.push_back ( CLPTrapezoid (xval, std::max (0., height) ) );
      }

   } // for xval;
   double upper, lower;
   //cout << curve << endl;
   CLPTrapezoid::limits (curve, upper, lower, gPercent);
   // cout << curve << endl;
   cout << "l " << lower << " u " << upper << endl;
   if (3 == gMode)
   {
      double slope1 = 2 / gXpeak;
      double slope2 = 2 / (1 - gXpeak);
      double x1 = lower;
      double x2 = 1 - upper;
      double y1 = x1 * slope1;
      double y2 = x2 * slope2;
      double area = 0.5 * (x1 * y1 + x2 * y2);
      cout << " x1 " << x1 << " y1 " << y1
           << " x2 " << x2 << " y2 " << y1
           << " area " << area << endl;
   }
   return 0;
}

void localInitialize()
{
   OptionUtils::addOptionKey ("mode",    gMode);
   OptionUtils::addOptionKey ("percent", gPercent);
   OptionUtils::addOptionKey ("xpeak",   gXpeak,
                              "X value of peak");
}
