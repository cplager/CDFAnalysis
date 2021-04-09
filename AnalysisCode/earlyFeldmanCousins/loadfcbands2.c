void loadfcbands2()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptStat(0);
   if ( !gSystem->CompileMacro("StringUtils.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("fcbands2.c", "k")    ) return;
   fcbands2();
}
