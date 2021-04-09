void load2 ()
{ 
   gROOT->SetStyle("Plain");
   gStyle->SetOptStat("");
   if ( !gSystem->CompileMacro("CLPTypeDef.cc", "k")  ) return;
   if ( !gSystem->CompileMacro("CLPHisto.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("MakeTH1F.cc", "k") ) return;
   // 025
   TH1F *h025 = MakeTH1F ("limits_0.025.txt", "2.5",
                          "2.5% limit as a function of b");
   if (!h025) {
      return;
   }
   h025->SetMarkerStyle(20);
   h025->Draw("P");
   c1->Print ("images/025.eps");
   // 1586
   TH1F *h1586 = MakeTH1F ("limits_0.1586.txt", "15.86",
                          "15.86% limit as a function of b");
   if (!h1586) {
      return;
   }
   h1586->SetMarkerStyle(20);
   h1586->Draw("P");
   c1->Print ("images/1586.eps");
   // 8414
   TH1F *h8414 = MakeTH1F ("limits_0.8414.txt", "84.14",
                          "84.14% limit as a function of b");
   if (!h8414) {
      return;
   }
   h8414->SetMarkerStyle(20);
   h8414->Draw("P");
   c1->Print ("images/8414.eps");
   // 975
   TH1F *h975 = MakeTH1F ("limits_0.975.txt", "97.5",
                          "97.5% limit as a function of b");
   if (!h975) {
      return;
   }
   h975->SetMarkerStyle(20);
   h975->Draw("P");
   c1->Print ("images/975.eps");
}
