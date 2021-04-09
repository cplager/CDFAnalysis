#include "TH1.h"
#include "TMath.h"
#include <iostream>

double fixedKS(const TH1 *parent, const TH1 *test) {

  if (parent->GetNbinsX() != test->GetNbinsX()) return -9E20;

  double sumParent = 0.0, sumTest = 0.0;
  double sumW2Parent = 0.0, sumW2Test = 0.0;

  for (int i = 1; i <= parent->GetNbinsX(); ++i) {
    sumParent += parent->GetBinContent(i);
    sumW2Parent += parent->GetBinError(i)*parent->GetBinError(i);

    sumTest += test->GetBinContent(i);
    sumW2Test += test->GetBinError(i)*test->GetBinError(i);
  }

  double sParent = 1/sumParent;
  double sTest = 1/sumTest;

  double eSumParent = sumParent*sumParent/sumW2Parent;
  double eSumTest = sumTest*sumTest/sumW2Test;

  double rSumParent = 0.0, rSumTest = 0.0, maxDiff = -9E20;
  for (int i = 1; i <= parent->GetNbinsX(); ++i) {
    rSumParent += sParent*parent->GetBinContent(i);
    rSumTest += sTest*test->GetBinContent(i);
    double diff = TMath::Abs(rSumParent-rSumTest);
    if (diff > maxDiff) maxDiff = diff;
  }

  //Get the Kolmogorov Probability
  return TMath::KolmogorovProb(maxDiff*TMath::Sqrt(eSumParent*eSumTest/(eSumParent+eSumTest)));

}
