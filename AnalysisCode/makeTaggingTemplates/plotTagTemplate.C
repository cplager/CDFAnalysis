// -*- C++ -*-


bool drawLines (int bins, double max, bool useLogScale = true);

#include "drawJetBinLines.C"

void plotTagTemplate (const char filename[], 
                      const char histname[] = "histname",
                      const char output[] = "bla.eps",
                      bool useLog = true)
{
   bool kludge = false;
   gROOT->SetStyle("Plain");
   gStyle->SetOptStat(0000);
   if (useLog) gStyle->SetOptLogy();
   //TCanvas *c1 = new TCanvas();
   TFile *filePtr = TFile::Open(filename);
   if (! filePtr)
   {
      cerr << "Couldn't open " << filename << ".  Aborting" << endl;
      return;
   }
   TH1F *hPtr = (TH1F*) filePtr->Get(histname);
   if (! hPtr)
   {
      cerr << "Couldn't find " << histname << " from file "
           << filename << endl;
      return;
   }
   int bins = hPtr->GetNbinsX();
   double oldmax = hPtr->GetMaximum();
   double max;
   double hmax;
   if (useLog)
   {
      max = oldmax * 1000;
      hmax = oldmax * 30;
   } else {
      max  = oldmax * 1.4;
      hmax = oldmax * 1.1;
   }
   hPtr->SetMaximum (max);
   if (useLog) hPtr->SetMinimum (0.001);
   hPtr->SetLineColor (kBlack);
   hPtr->SetMarkerStyle(20);
   hPtr->SetMarkerColor (kBlack);
   if (kludge)
   {
      hPtr->SetTitle("Light, Charm, and Bottom Templates");
   }
   hPtr->SetLineWidth(2);
   hPtr->Draw();
   if (kludge)
   {
      // I set kludge to true if I want to see all three templates on
      // the same plot.  It's a kludge, but at least the boolean
      // variable is named well.
      TFile *file2 = TFile::Open("charm.root");
      TH1F *h2 = (TH1F*) file2->Get("charm");
      h2->SetLineColor(kRed);
      h2->SetLineWidth(2);
      h2->Draw("same");
      TFile *file3 = TFile::Open("bottom.root");
      TH1F *h3 = (TH1F*) file3->Get("bottom");
      h3->SetLineColor(8);
      h3->SetLineWidth(2);
      h3->Draw("same");
   }
   
   if (! drawLines (bins, max, useLog))
   {
      return;
   }
   c1->Print(output);
}

