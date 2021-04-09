{
gROOT->SetStyle("Plain");
gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box 
cout << endl << endl << "Probability bBest vs bMeas" << endl;
TCanvas *c1 = new TCanvas;
TGraph bprob("bProbBbest.txt");
bprob.SetMarkerStyle(20);
bprob.SetMarkerColor(kRed);
bprob.Fit("pol5");
bprob.Draw("AP");
c1->Print("bProbBbest.eps");
gStyle->SetStatY(0.75);
cout << endl << endl << "bBest vs bMeas" << endl;
c1 = new TCanvas;
//TGraph maxb("maxb.txt");
//maxb.SetMarkerStyle(21);
//maxb.SetMarkerColor(kGreen);
//maxb.Draw("AP");
TGraph bbest("bBbest.txt");
bbest.SetMarkerStyle(20);
bbest.SetMarkerColor(kRed);
bbest.Fit("pol5");
bbest.Draw("AP");
//maxb.Fit("pol5");
TF1 *line = new TF1("line", "[0] + [1] * x", 0.0, 1.1);
line->FixParameter(0, 0.);
line->FixParameter(1, 1.);
line->SetLineColor(46);
line->Draw("same");
c1->Print("bBbest.eps");
cout << endl << endl << "bMeas vs bBest" << endl;
c1 = new TCanvas;
//TGraph maxb("maxb.txt");
//maxb.SetMarkerStyle(21);
//maxb.SetMarkerColor(kGreen);
//maxb.Draw("AP");
TGraph bestb("bbestb.txt");
bestb.SetMarkerStyle(20);
bestb.SetMarkerColor(kRed);
bestb.Fit("pol5");
bestb.Draw("AP");
//maxb.Fit("pol5");
c1->Print("bbestb.eps");
}
