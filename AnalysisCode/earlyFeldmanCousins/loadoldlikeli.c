void loadoldlikeli ()
{ 
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.1);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   if ( !gSystem->CompileMacro("CLPTypeDef.cc", "k")  ) return;
   if ( !gSystem->CompileMacro("CLPHisto.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("oldlikeliRat.c", "k") ) return;
   TF1 *likeR = new TF1("likeliRat", likeliRat, 0.4, 0.6, 0);
   likeR->Draw();
   c1->Print("oldlikelirat.eps");
   double x1, x2;
   double area = findFCinterval (0.6828, x1, x2);
   cout << "area " << area << " x1 " << x1 << " x2 " << x2 << endl;
   double area = findFCinterval (0.95, x1, x2);
   cout << "area " << area << " x1 " << x1 << " x2 " << x2 << endl;
}
