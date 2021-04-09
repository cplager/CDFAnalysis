{
gROOT->SetStyle("Plain");
//gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box 
TString prefix = "041116_";
TString suffix = "";
TFile a("output" + suffix + ".root");
TTree *tree = (TTree *) gROOT->FindObject("comb");
const int kSize = 8;
TString array[kSize] = 
{
   "cod1Val:blue1Val", 
   "blue1aVal:blue1Val", 
   "blue1bVal:blue1Val",
   "cod1Err:blue1Err",
   "cod2Val:blue2Val", 
   "blue2aVal:cod2Val", 
   "blue2aVal:blue2Val", 
   "cod2Err:blue2Err"
};
TString title[kSize] = 
{
   "COD1 Xsec vs. BLUE1 Xsec",
   "Non-iterative BLUE1 Xsec vs. BLUE1 Xsec",
   "Sqrt Stat errors BLUE1 vs. BLUE1 Xsec",
   "COD1 Errors vs. BLUE1 Errors",
   "COD2 Xsec vs. BLUE2 Xsec",
   "Non-Iterative BLUE2 Xsec vs. COD2 Xsec",
   "Non-iterative BLUE2 Xsec vs. BLUE2 Xsec",
   "COD2 Errors vs. BLUE2 Errors"
};
TString axisX[kSize] = {
   "Blue1 Cross Section (pb-1)",
   "Blue1 Cross Section (pb-1)",
   "Blue1 Cross Section (pb-1)",
   "Blue1 Cross Section Error (pb-1)",
   "Blue2 Cross Section (pb-1)",
   "COD2 Cross Section (pb-1)",
   "Blue2 Cross Section (pb-1)",
   "Blue2 Cross Section Error (pb-1)"
};
TString axisY[kSize] = {
   "COD1 Cross Section (pb-1)",
   "Non-iterative Blue1 Cross Section (pb-1)",
   "Sqrt Stat Errors Blue1 Cross Section (pb-1)",
   "Cod1 Cross Section Error (pb-1)",
   "COD2 Cross Section (pb-2)",
   "Non-Iterative Blue2 Cross Section (pb-1)",
   "Non-iterative Blue2 Cross Section (pb-1)",
   "Cod2 Cross Section Error (pb-1)"
};

for (Int_t loop = 0; loop < kSize; ++loop)
{
   cout << endl << endl << array[loop] << endl;
   tree->Draw(array[loop]);
   htemp->SetTitle(title[loop]);
   htemp->GetXaxis()->SetTitle(axisX[loop]);
   htemp->GetYaxis()->SetTitle(axisY[loop]);
   TString name = array[loop];
   name.ReplaceAll(":", "_vs_");
   c1->Print("eps/" + prefix + name + ".eps");            
   cout << "loop " << loop << endl;
} // for loop
cout << "done" << endl;
}
