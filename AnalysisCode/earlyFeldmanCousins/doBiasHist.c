#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include <math.h>
#include <iostream>
#include "MakeTH1F.h"

void doBiasHist ()
{ 
   TCanvas c1;
   TH1F *hist = MakeTH1F ("bHist.txt", "bMax", "bMax vs bTrue");
   if (!hist) {
      return;
   }
   hist->SetMarkerStyle(20);
   hist->Fit("pol1");
   hist->Draw("P");
   TF1 *line = new TF1("line", "[0] + [1] * x", 0.0, 1.0);
   line->FixParameter(0, 0.);
   line->FixParameter(1, 1.);
   line->SetLineColor(46);
   line->Draw("same");
   c1.Print ("images/bMaxBias.eps");
}
