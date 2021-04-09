void fitBbest()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.1);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   gStyle->SetStatY(0.75);
   // b vs max (max independant)
   TGraph maxb("maxb.txt");
   maxb.SetMarkerStyle(20);
   maxb.SetMarkerColor(kRed);
   maxb.Fit("pol5");
   maxb.Draw("AP");
   c1->Print("maxb.eps");
   // max vs b
   TCanvas c2;
   TGraph bmax("bmax.txt");
   bmax.SetMarkerStyle(20);
   bmax.SetMarkerColor(kRed);
   bmax.Fit("pol5");
   bmax.Draw("AP");
   c2.Print("bmax.eps");   
   // mean vs b
   TCanvas c3;
   TGraph bmean("bmean.txt");
   bmean.SetMarkerStyle(20);
   bmean.SetMarkerColor(kRed);
   bmean.Fit("pol5");
   bmean.Draw("AP");
   c3.Print("bmean.eps");   
   gStyle->SetStatY(0.95);
   // bconst
   TCanvas c4;
   TGraph bconst("bconst.txt");
   bconst.SetMarkerStyle(20);
   bconst.SetMarkerColor(kRed);
   bconst.Fit("pol5");
   bconst.Draw("AP");
   c4.Print("bconst.eps");

   //   // bnegsig
   //   TCanvas c10;
   //   TGraph bnegsig("bnegsig.txt");
   //   bnegsig.SetMarkerStyle(20);
   //   bnegsig.SetMarkerColor(kRed);
   ////   bnegsig.Fit("pol5");
   //   bnegsig.Draw("AP");
   //   c10.Print("bnegsig.eps");
   //   // bpossig
   //   TCanvas c5;
   //   TGraph bpossig("bpossig.txt");
   //   bpossig.SetMarkerStyle(20);
   //   bpossig.SetMarkerColor(kRed);
   ////   bpossig.Fit("pol5");
   //   bpossig.Draw("AP");
   //   c5.Print("bpossig.eps");
   //   // bnegfrac
   //   TCanvas c6;
   //   TGraph bnegfrac("bnegfrac.txt");
   //   bnegfrac.SetMarkerStyle(20);
   //   bnegfrac.SetMarkerColor(kRed);
   ////   bnegfrac.Fit("pol5");
   //   bnegfrac.Draw("AP");
   //   c6.Print("bnegfrac.eps");
   //   // bposfrac
   //   TCanvas c7;
   //   TGraph bposfrac("bposfrac.txt");
   //   bposfrac.SetMarkerStyle(20);
   //   bposfrac.SetMarkerColor(kRed);
   ////   bposfrac.Fit("pol5");
   //   bposfrac.Draw("AP");
   //   c7.Print("bposfrac.eps");
   //   // bnegSF
   //   TCanvas c8;
   //   TGraph bnegSF("bnegSF.txt");
   //   bnegSF.SetMarkerStyle(20);
   //   bnegSF.SetMarkerColor(kRed);
   ////   bnegSF.Fit("pol5");
   //   bnegSF.Draw("AP");
   //   c8.Print("bnegSF.eps");
   //   // bposSF
   //   TCanvas c9;
   //   TGraph bposSF("bposSF.txt");
   //   bposSF.SetMarkerStyle(20);
   //   bposSF.SetMarkerColor(kRed);
   ////   bposSF.Fit("pol5");
   //   bposSF.Draw("AP");
   //   c9.Print("bposSF.eps");
}
