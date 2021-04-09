void loadfcbands3()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptStat(0);
   if ( !gSystem->CompileMacro("StringUtils.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("fcbands3.c", "k")    ) return;
   fcbands("fc_syst.description");
}
