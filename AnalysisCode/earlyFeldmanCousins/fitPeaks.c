{
gROOT->SetStyle("Plain");
gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box 
for (int loop = 0; loop <= 100; ++loop)
{
   TCanvas *c1 = new TCanvas;
   cout << endl << endl <<  "b: " <<  loop / 100. << endl;
   char line[100], eps[100];
   sprintf (line, "txt/tmp%d.txt", loop);
   sprintf (eps, "txt/tmp%d.eps", loop);
   TGraph points(line);
   if (! points.GetN())
   {
      continue;
   }
   points.SetMarkerStyle(20);
   points.SetMarkerColor(kRed);
   points.Fit("pol3");
   points.Draw("AP");
   c1->Print(eps);
}
}

