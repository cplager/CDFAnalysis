void loadfits ()
{ 
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.25);        // height of box
   gStyle->SetStatW(0.1);         // width of box 
   if ( !gSystem->CompileMacro("CLPTypeDef.cc", "k")  ) return;
   if ( !gSystem->CompileMacro("CLPHisto.cc", "k")    ) return;
   if ( !gSystem->CompileMacro("fitPEdists.c", "k") ) return;
   fitPEdists();
}
