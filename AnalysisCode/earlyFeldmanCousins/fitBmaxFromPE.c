void fitBmaxFromPE()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.1);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   gStyle->SetStatY(0.75);
   // bBest(mmeas) vs mmeas
   TCanvas *c1 = new TCanvas;
   cout << endl << endl << "bBest as a function of bMeas" << endl;
   TGraph maxb("txt/bmax_b.txt");
   maxb.SetTitle("bBest vs. bMeasured");
   maxb.SetMarkerStyle(20);
   maxb.SetMarkerColor(kRed);
   maxb.Fit("pol3");
   maxb.Draw("AP");
   TLine unbias (0.0, 0.0, 1, 1);
   unbias.SetLineColor(kGreen);
   unbias.Draw();
   c1->Print("eps/bmax_b.eps");
   // bMeas(bBest)
   c1 = new TCanvas;
   cout << endl << endl << "bMeas as a function of bBest" << endl;
   TGraph bmax("txt/b_bmax.txt");
   bmax.SetTitle("bMeasured vs. bBest");
   bmax.SetMarkerStyle(20);
   bmax.SetMarkerColor(kRed);
   bmax.Fit("pol3");
   bmax.Draw("AP");
   TLine unbias (0.0, 0.0, 1, 1);
   unbias.SetLineColor(kGreen);
   unbias.Draw();
   c1->Print("eps/b_bmax.eps");
   // p(bmeas|bBest)
   cout << endl << endl << "P(bMeas|bBest) as a function of bMeas" << endl;
   c1 = new TCanvas;
   TGraph maxP("txt/bmax_const.txt");
   maxP.SetTitle("P(bMeasured | bBest) vs. bMeasured");
   maxP.SetMarkerStyle(20);
   maxP.SetMarkerColor(kRed);
   maxP.Fit("pol6");
   maxP.Draw("AP");
   c1->Print("eps/bmax_const.eps");   
}
