{
gROOT->SetStyle("Plain");
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box 
TString prefix = "041117_";
//TString suffix = "";
TString suffix = "_bk_acc";
TFile a("output" + suffix + ".root");
TTree *tree = (TTree *) gROOT->FindObject("comb");
const int kSize = 8;
//  // statistical uncertainties only
//  double lower[kSize] = {-.12, -.12, -.12, 0.5, 0.5, -0.5};
//  double upper[kSize] = { .12,  .12,  .12, 1.5, 1.5,  0.5};
// full systematic uncertainties
double lower[kSize] = {-.12, -.12, -.12, 0.7, 0.7, -0.5, -0.0, -0.0};
double upper[kSize] = { .12,  .12,  .12, 2.0, 2.0,  0.5,  0.3,  0.3};

TString name[kSize + 2] = 
{
   "blue2Val-cod2Val",
   "blue2aVal-cod2Val",
   "blue2Val-blue2aVal",
   "blue2Err",
   "cod2Err",
   "blue2Err-cod2Err",
   "cod2Perr-asym2Perr",
   "cod2Nerr-asym2Nerr",
}

TString array[kSize + 2] = 
{
   "2*(blue2Val-cod2Val)/(blue2Val+cod2Val)",
   "2*(-blue2aVal+cod2Val)/(blue2aVal+cod2Val)",
   "2*(blue2Val-blue2aVal)/(blue2Val+blue2aVal)",
   "blue2Err",
   "cod2Err",
   "blue2Err-cod2Err",
   "(cod2Perr-asym2Perr)/cod2Nerr",
   "(cod2Nerr-asym2Nerr)/cod2Nerr",
};

TString title[kSize + 2] = 
{
   "Relative difference between Iterative BLUE and COD",
   "Relative difference between COD and Non-Iterative BLUE",
   "Relative difference between Iterative and Non-Iterative BLUE",
   "Iterative BLUE Errors",
   "COD Errors",
   "Difference between BLUE and COD errors",
   "Difference between BLUE and COD Positive Errors",
   "Difference between BLUE and COD Negative Errors",

};

TString axisX[kSize + 2] = 
{
   "Relative difference",
   "Relative difference",
   "Relative difference", 
   "Cross Section Error (pb)",
   "Cross Section Error (pb)",
   "Cross Section Error (pb)",
   "Cross Section Error (pb)",
   "Cross Section Error (pb)"
};

for (Int_t loop = 0; loop < kSize; ++loop)
{
   // cout << "starting" << endl;
   TH1F hist("hist", "hist", 100, lower[loop], upper[loop]);
   // cout << "Drawing " << array[loop] << endl;
   tree->Draw(array[loop]+ ">>hist");
   // cout << "Setting Title " << title[loop] << endl;
   hist->SetTitle(title[loop]);
   // cout << "setting axis " << axisX[loop] << endl;
   hist->GetXaxis()->SetTitle(axisX[loop]);
   // cout << "saving" << endl;
   c1->Print("eps/" + prefix + name[loop] + suffix + ".eps");            
   // cout << "loop " << loop << endl;
} // for loop
// cout << "done" << endl;
}
