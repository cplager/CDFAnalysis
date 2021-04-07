// -*- C++ -*-
#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TLine.h"
#include "TText.h"

using namespace std;

bool drawLines (int bins, double max, bool useLogScale, double min = 0)
{
   int offsets[6] = {0, 2, 5, 8, 11, 14};
   // do we understand the number of bins
   int numTaggers = 0;
   int numJets = 0;
   if (16 == bins)
   {
      numJets = 3;
      numTaggers = 2;
   } else if (22 == bins)
   {
      numJets = 4;
      numTaggers = 2;
   } else if (24 == bins)
   {
      numJets = 3;
      numTaggers = 3;
   } else if (33 == bins)
   {
      numJets = 4;
      numTaggers = 3;
   } else if (18 == bins)
   {
      numJets = 3;
      numTaggers = 2;
      offsets[3] = 9;
   } else {
      // don't know what we're doing
      cout << "How can you have " << bins << " bins?  Aborting." << endl;
      return false;
   }
   ////////////////
   // Draw Lines //
   ////////////////
   double hmax = max / 30;
   double upperHmax = hmax * 5;
   double lowerHmax = hmax / 5;
   if (! useLogScale)
   {
      hmax      = 0.9   * (max - min) + min;
      upperHmax = 0.945 * (max - min) + min;
      lowerHmax = 0.855 * (max - min) + min;
   }
   double end     = numTaggers * offsets[numJets] - 0.5;
   // Main horizontal line
   TLine *horz = new TLine (-0.5, hmax, end, hmax);  horz->Draw("same");
   // Main vertical lines
   for (int tag = 1; tag < numTaggers; ++tag)
   {
      double middle = tag * offsets[numJets] - 0.5;
      TLine *vert = new TLine (middle, min, middle, max); 
      vert->SetLineWidth(2);vert->Draw("same");
   }
   // vertical jet bin lines
   for (int jet = 1; jet < numJets; ++jet)
   {
      for (int tag = 1; tag <= numTaggers; ++tag)
      {
         double xcoord = offsets[jet]  + offsets[numJets] * (tag - 1)
            - 0.5;
         TLine *vert = new TLine (xcoord, min, xcoord, hmax); vert->Draw("same");
      } // for tag
   } // for jet
   
   //////////////////
   // Write Labels //
   //////////////////
   for (int tag = 1; tag <= numTaggers; ++tag)
   {
      TString tagger;
      double taggerOffset = (tag - 1) * offsets[numJets];
      double middle = offsets[numJets] / 2. - 0.5;
      if (1 == tag)
      {
         tagger = "Loose Tagger";
      } else if (2 == tag)
      {
         tagger = "Tight Tagger";
      } else if (3 == tag)
      {
         tagger = "Ultra Tagger";
      }
      TText *tagLabel = new TText (middle + taggerOffset, upperHmax,
                                   (const char*) tagger);
      tagLabel->SetTextSize (0.10 / numTaggers);
      tagLabel->SetTextAlign (22);
      tagLabel->Draw();
      double textsize = 0.25 / (numTaggers * numJets);
      for (int jet = 1; jet <= numJets; ++jet)
      {
         double left  = offsets[jet - 1] - 0.5;
         double right = offsets[jet]     - 0.5;
         double xcoord = (left + right) / 2;
         TString jetString = Form ("%d Jets", jet);
         if (1 == jet)
         {
            jetString = "1 Jet";
         }
         TText *jetLabel = new TText (xcoord + taggerOffset, lowerHmax,
                                      (const char*) jetString);
         jetLabel->SetTextSize (textsize);
         jetLabel->SetTextAlign (22);
         jetLabel->Draw();      
      } // for jet
   } // for tag
   return true;
}
