void loadbias()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.1);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   gStyle->SetStatY(0.75);
   //gStyle->SetOptStat("");
   if ( !gSystem->CompileMacro("CLPTypeDef.cc", "k")  ) return;
   if ( !gSystem->CompileMacro("CLPHisto.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("MakeTH1F.cc", "k") )    return;
   if ( !gSystem->CompileMacro("doBiasHist.c", "k") )   return;
   doBiasHist();
}
