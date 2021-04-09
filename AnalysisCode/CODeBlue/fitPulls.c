{
gROOT->SetStyle("Plain");
gStyle->SetOptFit(0011);
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box 
TString prefix = "041118_";
//TString suffix = "_bk_acc";
TString suffix = "_acc";
//TString suffix = "_bk";
//TString suffix = "";
TFile a("output" + suffix + ".root");
TTree *tree = (TTree *) gROOT->FindObject("comb");
const int kSize = 14;
TString array[kSize] = 
{
   "dilPull", 
   "hadPull", 
   "cod1Pull", 
   "secvtxPull", 
   "nnfakePull", 
   "cod2Pull", 
   "blue1Pull", 
   "blue1aPull", 
   "blue1bPull",
   "blue2Pull", 
   "blue2aPull",
   "asym1Pull",
   "asym2Pull",
   "asym2aPull"
};
TString title[kSize] = 
{
   "Dilepton Pull Distribution",
   "All Hadronic Pull Distribution",
   "COD1 Combination Pull Distribution",
   "SecVtx Pull Distribution",
   "Fake NN Pull Distribution",
   "COD2 Combination Pull Distribution",
   "Blue1 correct errors Pull Distribution",
   "Blue1 non-iterative Pull Distribution",
   "Blue1 Sqrt Error Pull Distribution",
   "Blue2 correct errors Pull Distribution",
   "Blue2 non-iterative Pull Distribution",
   "Asymmetric Pull Distribution",
   "Asymmetric Pull Distribution",
   "Uncorrected Asymmetric Pull Distribution"
};
TString axis[kSize] = {
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution",
   "Pull Distribution"
};
// loop
for (Int_t loop = 0; loop < kSize; ++loop)
{
   cout << endl << endl << array[loop] << endl;
   TString looseCuts = array[loop] + " > -9000";
   TString tightCuts = "abs(" + array[loop] + ") <= 1.0";
   int tight = tree->Draw(array[loop], tightCuts);
   int loose = tree->Draw(array[loop], looseCuts);
   cout << "coverage: " << 100 * tight / (double) loose << endl;
   TH1F *temp = htemp->Clone();
   temp->SetName(array[loop]);
   temp->Fit ("gaus", "", "", 
              temp->GetXaxis()->GetXmin(),
              temp->GetXaxis()->GetXmax());
   temp->SetTitle(title[loop]);
   temp->GetXaxis()->SetTitle(axis[loop]);
   temp->Draw();
   c1->Print("eps/" + prefix + array[loop] + suffix + ".eps");            
} // for loop
}
