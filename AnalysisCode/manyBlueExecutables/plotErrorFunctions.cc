// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

#include "TClonesArray.h"
#include "TTree.h"
#include "TSystem.h"
#include "TRandom3.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "RootFileCollection/TreeFileInterface.h"
#include "RootFileCollection/TFINamespace.h"
#include "RootFileCollection/OptionUtils.h"
#include "RootFileCollection/dout.h"
#include "CLPBlue/CLPAsymBlue.hh"
#include "CLPBlue/CLPTypeDef.hh"

using namespace std;
using namespace OptionUtils;
using namespace TFINamespace;

// Order of analyses in AIB
enum
{
   kLF,
   kME,
   kNN,
   kNumMeas,
};

const int kColors[kNumMeas] = {kRed, kBlue, 8};
const string kNames[kNumMeas] = {"LF", "ME", "NN"};

// Polluting the gLobal namespace - gLobal variables
// Note, name in parenthesis is command-line variable name.

// which uncertainty to plot (which)
int gWhichUncertainty = 1;
// title of plot (title)
string gTitle = "Some Title";
// name of eps file
string gEpsName = "plot.eps";
// minimum X value (minX)
double gMinX = 0.1;
// maximum X value (maxX)
double gMaxX = 2.0;

// forward declarations
void localInitialize();

int main(int argc, char** argv)
{
   // Parse Command Line Arguments          
   // Decide Which Files and How Many Events
   setupDefaultOptions(); // hook up standard options
   localInitialize();
   parseArguments (argc, argv); // OptionUtils::

   string tag = ""; // any additional tags you want added on an output
                    // filename
   gROOT->SetStyle("Plain");
   delete gRandom; gRandom = new TRandom3;

   CLPAsymBlue aib;
   aib.setIgnoreLoadWarnings (true);
   aib.readFromFile ("stc.aib");
   
   int numErr = (int) aib.numErrors();
   if (gWhichUncertainty < 0 || gWhichUncertainty >= numErr)
   {
      cout << "which = " << gWhichUncertainty << " out of bounds." << endl;
      return 1;
   }
   TCanvas c1;
   vector< TF1* > tf1PtrVec;
   double min = 0, max = 0;
   int num = 0;
   for (int measInd = 0; measInd < kNumMeas; ++measInd)
   {
      for (int upperLower = 0; upperLower < 2; ++upperLower)
      {
         DVec paramVec;
         string upLow = "_up";
         if (upperLower)
         {
            aib.getLowerErrorFraction (measInd, gWhichUncertainty, paramVec);
            upLow = "_lo";
         } else {
            aib.getUpperErrorFraction (measInd, gWhichUncertainty, paramVec);
         }
         int dim = (int) paramVec.size() - 1;
         if (dim < 0 || dim > 9)
         {
            cerr << "Parameter Vector is the wrong size.  Aborting."
                 << endl;
            return 2;
         } // if dim wrong size
         if (dim >= 1)
         {
            ++num;
         }
         TF1 *funcPtr = new TF1( (kNames[measInd] + upLow).c_str(),
                                 Form ("pol%d", dim),
                                 gMinX, gMaxX );

         funcPtr->SetParameters( &(paramVec[0]) );
         funcPtr->SetLineColor (kColors[measInd]);
         if (upperLower) funcPtr->SetLineStyle (2);
         double localMax = funcPtr->GetMaximum (gMinX, gMaxX);
         double localMin = funcPtr->GetMinimum (gMinX, gMaxX);
         if (! measInd && ! upperLower)
         {
            min = localMin;
            max = localMax;
         } else {
            // check
            if (localMax > max)
            {
               max = localMax;
            }
            if (localMin < min)
            {
               min = localMin;
            }
         } // else
         tf1PtrVec.push_back( funcPtr );         
      } // for upperLower      
   } // for measInd
   // lets get ready
   double distance = max - min;
   double globMin = min - 0.40 * distance;
   double globMax = max + 0.10 * distance;
   TF1 *firstPtr = tf1PtrVec[0];
   firstPtr->SetMinimum (globMin);
   firstPtr->SetMaximum (globMax);
   firstPtr->SetTitle (gTitle.c_str());
   firstPtr->GetXaxis()->SetTitle ("#beta");
   firstPtr->GetYaxis()->SetTitle ("#beta");
   int size = (int) tf1PtrVec.size();
   TLegend *legendPtr = new TLegend (0.65, 0.42 - 0.05 * num, 0.85, 0.42);
   legendPtr->SetFillStyle (0);
   legendPtr->SetLineColor (kWhite);
   for (int loop = 0; loop < size; ++loop)
   {
      TF1 *funcPtr = tf1PtrVec.at(loop);
      if (1 == funcPtr->GetNpar())
      {
         // don't bother
         continue;
      }
      if (loop)
      {
         funcPtr->Draw("same");
      } else {
         funcPtr->Draw();
      }
      string name = kNames[loop / 2];
      if (loop % 2)
      {
         name += " Lower";
      } else {
         name += " Upper";
      }
      legendPtr->AddEntry (funcPtr, name.c_str(), "l");
   }
   legendPtr->Draw("same");
   c1.Print (gEpsName.c_str());
}

void localInitialize()
{   
   // put options you want here
   addOptionKey ("which", gWhichUncertainty);
   addOptionKey ("title", gTitle);
   addOptionKey ("eps",   gEpsName);
   addOptionKey ("minX",  gMinX);
   addOptionKey ("maxX",  gMaxX);   
}
