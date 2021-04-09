void loadMG ()
{ 
   gROOT->SetStyle("Plain");
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);
   if ( !gSystem->CompileMacro("CLPTypeDef.cc", "k")  ) return;
   if ( !gSystem->CompileMacro("CLPHisto.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("StringUtils.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("manyGaussians.c", "k")    ) return;
   manyGaussians();
}
