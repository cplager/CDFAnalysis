void loadfcbands()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptStat(0);
   if ( !gSystem->CompileMacro("fcbands.c", "k")    ) return;
   fcbands();
}
