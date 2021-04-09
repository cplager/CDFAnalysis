void loadlikeli ()
{ 
   gROOT->SetStyle("Plain");
   gStyle->SetOptStat(0);
   gStyle->SetStatH(0.1);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   if ( !gSystem->CompileMacro("CLPTypeDef.cc", "k")  ) return;
   if ( !gSystem->CompileMacro("CLPHisto.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("StringUtils.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("likeliRat.c", "k")    ) return;
   //TF1 *likeR = new TF1("likeliRat", likeliRat, 0., 2., 0);
   //likeR->Draw();
   //c1->Print("likelirat.eps");
   loadfile ("sys_new.fc");
   findAllFCIntervals();

   //double x1, x2;
   //double area = findFCinterval (0.6828, x1, x2);
   //cout << "area " << area << " x1 " << x1 << " x2 " << x2 << endl;
   //double area = findFCinterval (0.95, x1, x2);
   //cout << "area " << area << " x1 " << x1 << " x2 " << x2 << endl;
}
