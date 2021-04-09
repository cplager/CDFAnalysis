{
gROOT->SetStyle("Plain");
gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box 

TF1 fun1 ("fun1", "pol3", 2, 15);
TF1 fun2 ("fun2", "pol3", 2, 15);
fun1.SetParameters(2.08497e+00, 3.06526e-02, -2.09833e-04, 1.82777e-06);
fun2.SetParameters(2.09008e+00,  2.78397e-02,  1.84048e-04, -1.42778e-05);
fun1.Draw("AL");
fun2.SetLineColor(kRed);
fun2.Draw("L");
}
