{
gROOT->SetStyle("Plain");
gStyle->SetStatH(0.1);          // height of box
gStyle->SetStatW(0.1);         // width of box 
TString prefix = "041118_";
TString statOnly = "";
TString total = "_bk_acc";
TString back  = "_bk";
//TString suffix = statOnly;
TString suffix = back;
TFile a("output" + suffix + ".root");
TString title;
if (suffix == total)
{
   title = "COD and Asymmetric BLUE Asymmetry (total errors)";
} else if (suffix = back)
{
   title = "COD and Asymmetric BLUE Asymmetry (stat + back errors)";
} else  {
   title = "COD and Asymmetric BLUE Asymmetry (stat errors only)";
}
TTree *tree = (TTree *) gROOT->FindObject("comb");
TH1F cod ("cod", "cod", 100, 1.0, 1.15);
TH1F asym ("asym", "asym", 100, 1.0, 1.15);
tree->Draw("cod2Perr/cod2Nerr >> cod");
tree->Draw("asym2Perr/asym2Nerr >> asym");
asym.SetTitle(title);
cod.SetTitle(title);
asym.GetXaxis()->SetTitle("Asymmetry");
asym.SetLineColor(kBlue);
cod.SetLineColor(kRed);
if (suffix == total)
{
   cod.Draw();
   asym.Draw("same");
} else {
   asym.Draw();
   cod.Draw("same");
}
c1->Print("eps/" + prefix + "cod_asym_asymmetry" + suffix + ".eps");
}
