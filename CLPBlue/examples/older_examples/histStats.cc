#include "TSystem.h"
#include "TH1.h"
#include "TH1F.h"
#include "CLPTypeDef.h"
#include <iostream>
#include <vector>
#include "histStats.h"

using namespace std;

const Double_t kSixteen    = 0.1586;
const Double_t kFifty      = 0.50;
const Double_t kEightyfour = 0.8414;

void histStats (TH1 *hPtr, Double_t &sixteen, 
                Double_t &fifty, Double_t &eightyfour)
{
   Double_t lower = hPtr->GetXaxis()->GetXmin();
   Double_t upper = hPtr->GetXaxis()->GetXmax();
   sixteen = fifty = eightyfour = lower - 1;
   Int_t nBins = hPtr->GetNbinsX();
   Double_t perBin = (upper - lower) / nBins;
   vector< Double_t > binList;
   Double_t sum = 0;
   // Get the contents of the histogram
   // 0 and nBins + 1 are the under/overflow bins   
   for (Int_t loop = 0; loop <= nBins + 1; ++loop)
   {
      Double_t contents = hPtr->GetBinContent(loop);
      sum += contents;
      binList.push_back( contents );
   } // for loop
   // sum over it again and see where you get 16, 50 and 84 percentile
   Double_t prevPercent = 0;
   Double_t localSum = 0;
   // don't do overflow this time
   for (Int_t bin = 0; bin <= nBins; ++bin)
   {
      localSum += binList[bin];
      Double_t currPercent = localSum / (Double_t) sum;
      // sixteen
      if ((prevPercent < kSixteen) && (kSixteen <= currPercent))
      {
         // we found the bin
         Double_t xLow = lower + (bin - 1) * perBin;
         // how far in between xLow and xHigh should this be
         Double_t percent  = (kSixteen - prevPercent) /
            (currPercent - prevPercent);
         sixteen = xLow + percent * perBin;
      }
      // fifty
      if ((prevPercent < kFifty) && (kFifty <= currPercent))
      {
         // we found the bin
         Double_t xLow = lower + (bin - 1) * perBin;
         // how far in between xLow and xHigh should this be
         Double_t percent  = (kFifty - prevPercent) /
            (currPercent - prevPercent);
         fifty = xLow + percent * perBin;
      }
      // eightyfour
      if ((prevPercent < kEightyfour) && (kEightyfour <= currPercent))
      {
         // we found the bin
         Double_t xLow = lower + (bin - 1) * perBin;
         // how far in between xLow and xHigh should this be
         Double_t percent  = (kEightyfour - prevPercent) /
            (currPercent - prevPercent);
         eightyfour = xLow + percent * perBin;
      }
      prevPercent = currPercent;
   }
}
