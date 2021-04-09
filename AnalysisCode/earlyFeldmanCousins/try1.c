{
   double b0min = 0.0;
   double b0max = 1.0;
   double bMeasMin = 0;
   double bMeasMax = 2.5;
   TCanvas *c1 = new TCanvas;
   TH2F *h2 = new TH2F ("h2", "Feldman Cousins Bands for R", 
                        2, bMeasMin, bMeasMax, 2, b0min, b0max);
   h2->Draw();
   double zerod = 0;
   TGraphErrors *point = 
      new TGraphErrors(1, &zerod, &zerod, &zerod, &zerod);
   point->SetMarkerColor(2);
   point->SetMarkerStyle(20);
   cout << "starting legend" << endl;
   TLegend *legend = new TLegend (0.5, 0.2, 0.8, 0.5);
   legend->AddEntry(point, "Most Likely Value");
   cout << "drawing legend" << endl;
   legend->Draw();
   cout << "drew lengend" << endl;
   h2->Draw("same");
   cout << "printing" << endl;
   c1->Print("try1.eps");
   cout << "printed" << endl;
}
