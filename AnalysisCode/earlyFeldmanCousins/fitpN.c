Double_t funky (Double_t *x, Double_t *par);

void fitpN()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.1);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   for (int loop = 0; loop <= 3; ++loop)
   {
      TCanvas *c1 = new TCanvas;
      cout << endl << endl <<  "p" <<  loop << endl;
      char line[100], eps[100], eps2[100];
      sprintf (line, "p%d.txt", loop);
      sprintf (eps, "p%d.eps", loop);
      sprintf (eps2, "p%d_2.eps", loop);
      TGraph points(line);
      points.SetMarkerStyle(20);
      points.SetMarkerColor(kRed);
      points.Fit("pol9", "W" , "", 0.20, 0.32);
      points.Fit("pol9", "W+", "", 0.32, 0.60);
      points.Fit("pol9", "W+", "", 0.60, 1.00);
      points.Draw("AP");
      c1->Print(eps);
   }
}

