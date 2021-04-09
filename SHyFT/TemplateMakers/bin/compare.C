void compare(){

 TCanvas*canv = new TCanvas("nJets", "nJets", 500, 500);
  canv -> cd();
  nJetsPlus -> Draw();
  double IntegralNJetsPlus = nJetsPlus -> Integral();
  cout << "Integral nJetsPlus = " << IntegralNJetsPlus << endl;

  nJetsMinus -> SetLineColor(2);
  nJetsMinus -> Draw("same");
  double IntegralNJetsMinus = nJetsMinus -> Integral();
  cout << "Integral nJetsMinus = " << IntegralNJetsMinus << endl;
  canv -> SaveAs("nJets_compare.png");
  

 TCanvas*canv = new TCanvas("ht", "ht", 500, 500);
  canv -> cd();
  htPlus -> Draw();
  double IntegralHTPlus = htPlus -> Integral();
  cout << "Integral htPlus = " << IntegralHTPlus << endl;

  htMinus -> SetLineColor(2);
  htMinus -> Draw("same");
  double IntegralHTMinus = htMinus -> Integral();
  cout << "Integral htMinus = " << IntegralHTMinus << endl;
  canv -> SaveAs("ht_compare.png");
  

 TCanvas*canv = new TCanvas("met", "met", 500, 500);
  canv -> cd();  
  metPlus -> Draw();
  double IntegralMetPlus = metPlus -> Integral();
  cout << "Integral metPlus = " << IntegralMetPlus << endl;

  metMinus -> SetLineColor(2);
  metMinus -> Draw("same");
  double IntegralMetMinus = metMinus -> Integral();
  cout << "Integral metMinus = " << IntegralMetMinus << endl;
  canv -> SaveAs("met_compare.png");
  

 TCanvas*canv = new TCanvas("muPt", "muPt", 500, 500);
  canv -> cd();
  muPtPlus -> Draw();
  double IntegralMuPtPlus = muPtPlus -> Integral();
  cout << "Integral muPtPlus = " << IntegralMuPtPlus << endl;

  muPtMinus -> SetLineColor(2);
  muPtMinus -> Draw("same");
  double IntegralMuPtMinus = muPtMinus -> Integral();
  cout << "Integral muPtMinus = " << IntegralMuPtMinus << endl;
  canv -> SaveAs("muPt_compare.png");
  
 TCanvas*canv = new TCanvas("WTransverseMass", "WTransverseMass", 500, 500);
  canv -> cd();
  mTPlus -> Draw();
  double IntegralmTPlus = mTPlus -> Integral(); 
  cout << "Integral mTPlus = " << IntegralmTPlus << endl;
 
  mTMinus -> SetLineColor(2);
  mTMinus -> Draw("same");
  double IntegralmTMinus = mTMinus -> Integral(); 
  cout << "Integral mTMinus = " << IntegralmTMinus << endl;
  canv -> SaveAs("mT_compare.png");

  
 TCanvas*canv = new TCanvas("WTransverseMass_1j", "WTransverseMass_1j", 500, 500);
  canv -> cd();
  mTPlus_1j -> Draw();
  double IntegralmTPlus_1j = mTPlus_1j -> Integral(); 
  cout << "Integral mTPlus_1j = " << IntegralmTPlus_1j << endl;
  
  mTMinus_1j -> SetLineColor(2);
  mTMinus_1j -> Draw("same");
  double IntegralmTMinus_1j = mTMinus_1j -> Integral(); 
  cout << "Integral mTMinus_1j = " << IntegralmTMinus_1j << endl;
  canv -> SaveAs("mT_1j_compare.png");

 
 TCanvas*canv = new TCanvas("WTransverseMass_2j", "WTransverseMass_2j", 500, 500);
  canv -> cd();
  mTPlus_2j -> Draw();
  double IntegralmTPlus_2j = mTPlus_2j -> Integral(); 
  cout << "Integral mTPlus_2j = " << IntegralmTPlus_2j << endl;
 
  mTMinus_2j -> SetLineColor(2);
  mTMinus_2j -> Draw("same");
  double IntegralmTMinus_2j = mTMinus_2j -> Integral(); 
  cout << "Integral mTMinus_2j = " << IntegralmTMinus_2j << endl;
  canv -> SaveAs("mT_2j_compare.png");


 TCanvas*canv = new TCanvas("WTransverseMass_3j", "WTransverseMass_3j", 500, 500);
  canv -> cd();
  mTPlus_3j -> Draw();
  double IntegralmTPlus_3j = mTPlus_3j -> Integral();
  cout << "IntegralmTPlus_3j =  " << IntegralmTPlus_3j << endl;
 
  mTMinus_3j -> SetLineColor(2);
  mTMinus_3j -> Draw("same");
  double IntegralmTMinus_3j = mTMinus_3j -> Integral(); 
  cout << "Integral mTMinus_3j = " << IntegralmTMinus_3j << endl;
  canv -> SaveAs("mT_3j_compare.png");


 TCanvas*canv = new TCanvas("WTransverseMass_4j", "WTransverseMass_4j", 500, 500);
  canv -> cd();
  mTPlus_4j -> Draw();
  double IntegralmTPlus_4j = mTPlus_4j -> Integral();
  cout << "IntegralmTPlus_4j =  " << IntegralmTPlus_4j << endl;
 
  mTMinus_4j -> SetLineColor(2);
  mTMinus_4j -> Draw("same");
  double IntegralmTMinus_4j = mTMinus_4j -> Integral(); 
  cout << "Integral mTMinus_4j = " << IntegralmTMinus_4j << endl;
  canv -> SaveAs("mT_4j_compare.png");


 TCanvas*canv = new TCanvas("WTransverseMass_3morej", "WTransverseMass_3morej", 500, 500);
  canv -> cd();
  TH1F *mTPlus_4j = mTPlus_4j;
  TH1F *mTPlus_sumj = mTPlus_3j;
  mTPlus_sumj -> Add(mTPlus_4j, 1);
  TH1F *mTPlus_5j = mTPlus_5j;
  mTPlus_sumj -> Add(mTPlus_5j, 1);
  mTPlus_sumj -> Draw();
  double IntegralmTPlusSum = mTPlus_sumj -> Integral(); 
  cout << "IntegralmTPlus >= 3 jets = " << IntegralmTPlusSum << endl;
  
  TH1F *mTMinus_4j = mTMinus_4j;
  TH1F *mTMinus_sumj = mTMinus_3j;
  mTMinus_sumj -> Add(mTMinus_4j, 1);
  TH1F *mTMinus_5j = mTMinus_5j;
  mTMinus_sumj -> Add(mTMinus_5j, 1);
  mTMinus_sumj -> SetLineColor(2);   
  mTMinus_sumj -> Draw("same");
  double IntegralmTMinusSum = mTMinus_sumj -> Integral(); 
  cout << "IntegralmTMinus >= 3 jets = " << IntegralmTMinusSum << endl;
  canv -> SaveAs("mT_3morej_compare.png");
}



