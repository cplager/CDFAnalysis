void load ()
{ 
   gROOT->SetStyle("Plain");
   if ( !gSystem->CompileMacro("CLPTypeDef.cc", "k")  ) return;
   if ( !gSystem->CompileMacro("CLPHisto.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("hcentBands1.c", "k") ) return;
   hcentBands1();
}
