{
gROOT->SetStyle("Plain");
gStyle->SetOptStat(0);
gStyle->SetOptFit(0);
//gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box 

TH2F *h2 = new TH2F ("h2", "Feldman Cousins Bands for R", 
                     2, 0, 100, 2, 0, 2.5);
TCanvas *c1 = new TCanvas;
h2->Draw();

//// FC 68b_2 interval
//cout << endl << endl << "68b_2" << endl;
//TGraph fc68b_2("fc68b_2.txt");
//fc68b_2.SetMarkerStyle(20);
//fc68b_2.SetMarkerColor(kGreen);
//fc68b_2.Fit("pol4");
//fc68b_2.Draw("P");

// FC 68a_1 interval
cout << endl << endl << "68a_1" << endl;
TGraph fc68a_1("fc68a_1.txt");
fc68a_1.SetMarkerStyle(20);
fc68a_1.SetMarkerColor(kBlue);
fc68a_1.Fit("pol7");
fc68a_1.Draw("P");

// FC 68a_2 interval
cout << endl << endl << "68a_2" << endl;
TGraph fc68a_2("fc68a_2.txt");
fc68a_2.SetMarkerStyle(20);
fc68a_2.SetMarkerColor(kBlue);
fc68a_2.Fit("pol7");
fc68a_2.Draw("P");


// FC 95_1 interval
cout << endl << endl << "95_1" << endl;
TGraph fc95_1("fc95_1.txt");
fc95_1.SetMarkerStyle(20);
fc95_1.SetMarkerColor(kRed);
fc95_1.Fit("pol7", "", "", 20, 100);
fc95_1.Draw("P");

// FC 95_2 interval
cout << endl << endl << "95_2" << endl;
TGraph fc95_2("fc95_2.txt");
fc95_2.SetMarkerStyle(20);
fc95_2.SetMarkerColor(kRed);
cout << "First try" << endl;
fc95_2.Fit("pol3", "", "", 2, 41);
cout << "second try" << endl;
fc95_2.Fit("pol3", "+", "", 41, 80);
fc95_2.Fit("pol4", "+", "", 78, 95.5);
fc95_2.Draw("P");

c1->Print("images/fc_bands.eps");

}

