{

   TString var = "KIT";
  // TString var = "NtrackSig3";
  //  TString var = "SecvtxMass";
  //   TString var = "ctau";
  //    TString var = "signD0sig1"; 
  //    TString var = "signD02";

 

     TString tagger = "";
//   TString nJets = "1";
//   TString nTags = "1";


     double minWc = 0.0;//97;//7;//7; //1.0;
     double maxWc = 5.0; //1.03;//; //1.3; //1.3; //01;//5.0;//1.3; //5.0; //1.3;
     TString constr = "";

  double cnstrVal1 = 1.;
  double cnstrErr1 = 0.1; // non-W
  
  double cnstrVal2 = 1.0; 
  double cnstrErr2 = 0.2;// mistags

  const int Nfit = 5;
  double param[Nfit];
  double errors[Nfit];
  TH1F *hmc1[Nfit];
  TH1F *hmc2[Nfit];
  TH1F *hdata1;
  TH1F *hdata2;
  
   gROOT->LoadMacro("root_macros/MyMinuitFit.C+");
   gROOT->LoadMacro("root_macros/fixedKS.C+");
   gROOT->LoadMacro("root_macros/MyChi2.C+");

   TFile ffit1("fit_inputs/fit_" + tagger + var + "_1j_1tag.root");
   TFile ffit2("fit_inputs/fit_" + tagger + var + "_2j_1tag.root");

   TString plot_name = "kfactor/results/Fit_2j_" + tagger + var;

//   //setup histo
   hmc1[0] = (TH1F*)ffit1.Get("non-W");
   hmc1[1] = (TH1F*)ffit1.Get("Mistags");
   hmc1[2] = (TH1F*)ffit1.Get("Wc");
   hmc1[3] = (TH1F*)ffit1.Get("Wcc");
   hmc1[4] = (TH1F*)ffit1.Get("Wbb");

   hdata1 = (TH1F*)ffit1.Get("dat");

   cout << "hmc2" << endl;
   hmc2[0] = (TH1F*)ffit2.Get("non-W");
   hmc2[1] = (TH1F*)ffit2.Get("Mistags");
   hmc2[2] = (TH1F*)ffit2.Get("Wc");
   hmc2[3] = (TH1F*)ffit2.Get("Wcc");
   hmc2[4] = (TH1F*)ffit2.Get("Wbb");

   hdata2 = (TH1F*)ffit2.Get("dat");

   cout << "hmc_fit" << endl;
   TH1F *hmc_fit[Nfit];
   int NBINS = hmc1[0]->GetNbinsX();
   hmc_fit[0] = new TH1F("non-W","non-W",2*NBINS,0.,2.);
   hmc_fit[1] = new TH1F("Mistags","Mistags",2*NBINS,0.,2.);
   hmc_fit[2] = new TH1F("Wc","Wc",2*NBINS,0.,2.);
   hmc_fit[3] = new TH1F("Wcc","Wcc",2*NBINS,0.,2.);
   hmc_fit[4] = new TH1F("Wbb","Wbb",2*NBINS,0.,2.);
   TH1F *hdata = new TH1F("data","data",2*NBINS, 0., 2.);


     for (int bin=1; bin<=NBINS;bin++){
       hdata->SetBinContent(bin,hdata1->GetBinContent(bin));
       hdata->SetBinContent(NBINS+bin,hdata2->GetBinContent(bin));
       for(int i=0; i<Nfit;i++){
	 hmc_fit[i]->SetBinContent(bin, hmc1[i]->GetBinContent(bin));
	 hmc_fit[i]->SetBinContent(NBINS+bin, hmc2[i]->GetBinContent(bin));
     }
   }

   cout << "constr" << endl;


   double constr_min[Nfit] = {cnstrVal1 - cnstrErr1, cnstrVal2 - cnstrErr2, minWc, 0.0, 0.0};
   double constr_max[Nfit] = {cnstrVal1 + cnstrErr1, cnstrVal2 + cnstrErr2, maxWc, 5.0, 5.0};


     SetMax(Nfit);
     SetHisto(hdata,hmc_fit);
     SetConstraints(constr_min, constr_max);

     cout << fixed;
     cout.precision(2);
     PerformFit(param, errors);
     
     hmc_fit[0]->SetFillColor(108);
     hmc_fit[1]->SetFillColor(30);
     hmc_fit[2]->SetFillColor(65);
     hmc_fit[3]->SetFillColor(4);
     hmc_fit[4]->SetFillColor(51);
     hdata->SetMarkerStyle(8);
     
     int Nm = Nfit;


   THStack hs("mc","mc");
   
   TH1F *hsum = (TH1F*) hmc_fit[0]->Clone();
   hsum->Reset();
   
 
   for (int i=0; i<Nm; i++){
     hmc_fit[i]->Scale(param[i]);
     hs.Add(hmc_fit[i]);
     hsum->Add(hmc_fit[i]);
   }
   float max_stack = hs.GetMaximum();
   float max_data = hdata->GetMaximum();
   if (max_data > max_stack)
     hs.SetMaximum(max_data);
   
   
   hs.Draw();
   hdata->Draw("esame");
   
   TAxis *xaxis = hs.GetXaxis();
   TString axis_name = var;
   if (tagger == "L") axis_name = " Loose " + var;
   if (tagger == "U")  axis_name = " Ultratight " + var;
   xaxis->SetTitle(axis_name);
   hs.SetTitle("");//Number of Events");

   //  cout << "KS = " << hdata->KolmogorovTest(hsum,"X") << endl;
   //   cout << "KS = " << fixedKS(hsum, hdata) << endl;
     cout << "Chi2 = " <<  Chi2(Nm, hsum, hdata)  << endl;
    cout << "NDF = " << hdata ->GetNbinsX() -Nm << endl;
    //  double chi2ndf = TMath::Abs( Chi2(hsum,hdata) / ( hdata ->GetNbinsX() -Nm ) );
    double chi2ndf = Chi2(Nm, hsum, hdata) / ( hdata ->GetNbinsX() -Nm ) ;
    cout << "Chi2/NDF = " << chi2ndf << endl;
    chi2ndf *= 100;
    int tmp = (int) chi2ndf;
    if (chi2ndf - tmp >= 0.5) tmp++;
    chi2ndf = (float) tmp / 100;
    TString tmp_string = "";
    tmp_string += chi2ndf;
    tmp = (int) chi2ndf;
    if (chi2ndf - tmp == 0) tmp_string += ".00"; 
    TString chi2_result = "#chi^{2}/NDF = ";
    chi2_result += tmp_string; 

    double ksprob =  fixedKS(hsum, hdata);
    cout << "KS = " << ksprob << endl;
    ksprob *= 100;
    tmp = (int) ksprob;
    if (ksprob - tmp >= 0.5) tmp++;
    ksprob = (float) tmp / 100;
    tmp_string = "       ";
    tmp_string += ksprob;
    tmp = (int) ksprob;
    if (ksprob - tmp == 0) tmp_string += ".00"; 
    TString ks_result = "KS = ";
    ks_result += tmp_string; 


   float legSize = 1- (Nm+1)*0.05;

    TLegend *leg = new TLegend(0.55,legSize,0.995,0.995); 
    for (int i=0; i<Nm; i++){
      TString legname = hmc_fit[i]->GetName();
      if (legname == "Wcc") legname = "Wc#bar{c}";
      if (legname == "Wbb") legname = "Wb#bar{b}";
      //  cout << "size = " << legname.Sizeof() << endl;
 //      if (fitType == "HF"){
// 	if (i==1) legname += " ";
// 	else if (i==2) legname += "     ";
// 	else if (i==3) legname += "    ";
//       } else if (fitType == "ALL"){
// 	if (i==1) legname += " ";
// 	else if (i==2) legname += "        ";
// 	else if (i==3) legname += "   ";
// 	else if (i==4) legname += "      ";
//       }
      param[i] *= 100;
      int tmp = (int) (param[i]);
      if (param[i] - tmp >= 0.5) tmp++;
      param[i] = (float) tmp / 100;
      TString tmp_string = "";
      tmp_string += param[i];
      tmp = (int) param[i];
      if (param[i] - tmp == 0) tmp_string += ".00"; 
      legname += tmp_string;
      legname += " #pm ";
      errors[i] *= 100;
      int tme = (int) errors[i];
      errors[i] = (float) tme / 100;
      legname += errors[i];

      leg->AddEntry(hmc_fit[i], legname,"f");
      //  leg->AddEntry(hmc_fit[i], hmc_fit[i]->GetName() + param[i],"f");
    }

    leg->AddEntry(hdata, chi2_result);
    //  leg->AddEntry(hdata, ks_result);
    leg->SetBorderSize(1);
    leg->SetFillColor(10);
    leg->Draw();
    
  
    c1->SaveAs(plot_name + ".gif");
    c1->SaveAs(plot_name + ".eps");
    gROOT->ProcessLine(".! epstopdf " + plot_name + ".eps");


}
