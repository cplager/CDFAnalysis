void loadfindBbest ()
{ 
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.1);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   if ( !gSystem->CompileMacro("CLPTypeDef.cc", "k")  ) return;
   if ( !gSystem->CompileMacro("CLPHisto.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("findBbest.c", "k") ) return;
   double bBest, PbBest;
   for (int loop = 40; loop <= 100; ++loop)
   {
      findPbBest (loop / 100., bBest, PbBest);
   }
}
