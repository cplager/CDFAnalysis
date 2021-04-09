//void Overlay_tag(int Nplots) {
{
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
  // @@ ========  MAGIC ====@@@@@@ //
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
  
  TString file_end = ".root";
  
  int Nentries[Nplots]; // save Nentries before renormalization
  double Integr[Nplots]; // save Integral before renormalization 
  
  TString filenames[11];
  TFile files[11];
  /*
  // Open all the files first:
    for (int k = 0; k < 7; k++) {
    filenames[k] = template[k].sample + file_end;
  }
  
  for (int l = 12; l < 16; l++) {
    filenames[l - 5] = template[l].sample + file_end;
  }
  */
  // Open all the files first: recoplots.C, Q^2, FitHF.C, jete
  for (int k = 0; k <= 2; k++) {
    filenames[k] = template[k].sample + file_end;
  }
  
  for (int l = 5; l <= 6; l++) {
    filenames[l - 2] = template[l].sample + file_end;
  }

  for (int l = 9; l <= 10; l++) {
    filenames[l - 4] = template[l].sample + file_end;
  }

  for (int l = 17; l <= 20; l++) {
    filenames[l - 10] = template[l].sample + file_end;
  }
  

  for (int m = 0; m < 11; m++) {
    cout << filenames[m] << endl;
    files[m] = new TFile(filenames[m]);
  }
  
  //define common range of the histogram
  float minX = minHist; 
  float maxX = maxHist; 
  
  if (!setMinMax){
    minX = 100000.; 
    maxX = -998.; 
  }
  
  int temp = -1;
  for (int i = 0; i < Nplots; i++){
    template[i].filename = template[i].sample + file_end;
    
    // Match this filename to the filenames[] array
    for (int m = 0; m < 11; m++) {
      if (template[i].filename == filenames[m]) {
	temp = m;
	break;
      }
    }
    template[i].f = files[temp];
    //template[i].f = new TFile(template[i].filename);
    (template[i].f).Get(tree_name);

    cout << "i = " << i << ", file = " << template[i].filename << ", sample = " << template[i].sample << endl;
    cout << "condition = " << template[i].condition << endl;

    template[i].tree = (TTree*)(template[i].f).Get(tree_name);
    template[i].tree->Draw(template[i].var + " >> hdump",template[i].condition);
    TAxis *x = (TAxis*) hdump->GetXaxis();
    
    if (!setMinMax){
      minX = TMath::Min( x->GetXmin(), minX);
      maxX = TMath::Max( x->GetXmax(), maxX);
      //   cout << "minX= " << minX << "; maxX = " << maxX << endl;
    }
    //(template[i].f) ->Close();
  }
  
  if (doKS){
    // Do the KS test
    int NBINS_KS = 10000;
    TH1F *hks[Nplots];
    for (int i=0; i<Nplots; i++){
      //   hks[i] = new TH1F(template[i].sample+template[i].var,template[i].sample+template[i].var,NBINS_KS,minX,maxX);
      //    template[i].tree->Draw(template[i].var + " >> " + template[i].sample+template[i].var,template[i].condition);
      hks[i] = new TH1F(template[i].sample,template[i].sample,NBINS_KS,minX,maxX);
      template[i].tree->Draw(template[i].var + " >> " + template[i].sample,template[i].condition);
    }
    
    double KSd[Nplots][Nplots];
    double KSp[Nplots][Nplots];
    double sqrtMu[Nplots][Nplots];
    double N[i];
    
    for (int i=0; i<Nplots; i++){
      N[i] = hks[i]->GetEntries();
      cout << template[i].sample << ":  Mean = " << hks[i]->GetMean() << "; RMS = " <<  hks[i]->GetRMS() << endl;
    }
    cout << "============= VAR = " << var << " ================== " << endl;
    for (int i=0; i<Nplots; i++){
      for (int j=i+1; j<Nplots; j++){
	KSd[i][j] = hks[i]->KolmogorovTest(hks[j],"M");
	sqrtMu[i][j] = TMath::Sqrt(N[i]*N[j]/(N[i]+N[j]));
	KSp[i][j] = TMath::KolmogorovProb( KSd[i][j]*(sqrtMu[i][j] + 0.12 + 0.11/ sqrtMu[i][j]));
	cout << template[i].sample << ":" << template[j].sample << " -- KSdist = " << KSd[i][j] << " -- KSprob = " << KSp[i][j] << endl;
      }
    }
    
  }  //end KS test 
  
  THStack hs(var,var);
  
  //for (int i = Nplots; i >0; i--){
  for (int i = 0; i < Nplots; i++){
    template[i].h = new TH1F(template[i].sample,template[i].sample,NBINS,minX,maxX);
    //cout << "i = " << i << "; sample = " << template[i].sample << "; minX = " << minX << "; maxX = " << maxX << endl;
    template[i].tree->Draw(template[i].var + " >> " + template[i].sample,template[i].condition);
    if (template[i].line_color) template[i].h->SetLineColor(template[i].line_color);
    if (template[i].line_width) template[i].h->SetLineWidth(template[i].line_width);
    if (template[i].fill_color)  template[i].h->SetFillColor(template[i].fill_color);
    if (template[i].marker_color)  template[i].h->SetMarkerColor(template[i].marker_color);
    if (template[i].marker_style)  template[i].h->SetMarkerStyle(template[i].marker_style);
    
    Nentries[i] = template[i].h->GetEntries();
    Integr[i] =  template[i].h->Integral();
    cout << template[i].sample << ": Nentries " << Nentries[i] << flush;
    cout << ";        Integral " << Integr[i] << endl;
    

    if (template[i].renorm != 1) {
      cout << "Doing renormalization" << endl;
      if (Integr[i] != 0) {
	template[i].h->Scale(template[i].renorm / Integr[i]);
      }
    }

    //    cout << template[i].sample << ": Nentries " << template[i].h->GetEntries() << flush;
    //    cout << ";        Integral " << template[i].h->Integral() << endl;;
    
    if (i!=0) // i==0 is data
      hs.Add(template[i].h);
  }
  
  
  if (norm){ // normalize histograms to the same area
    float Integ[Nplots];
    float NormInteg = 1000.;
    for (int i=0; i<Nplots; i++){
      Integ[i] = (template[i].h)->Integral();
      if (normToData && i==0) NormInteg = Integ[0];
      (template[i].h)->Scale( NormInteg / Integ[i] );      
      //     cout << "Integ[" << i << "] = " << Integ[i] << endl;
    }
  }//norm
  
  // draw first the histogram with larger maximum 
  float max[Nplots]; 
  TAxis *xt[Nplots];
  TAxis *yt[Nplots];
  int first = 0;
  float totMax = 0.;
  for (int i=0; i<Nplots; i++){
    max[i] = (template[i].h)->GetMaximum();
    if (max[i] > totMax){
      totMax =  max[i]; first = i;
    }
    xt[i] = (TAxis*)  (template[i].h)->GetXaxis();
    xt[i]->SetTitle(xtitle);
    // (template[i].h)->SetTitle();
    
    yt[i] = (TAxis*)  (template[i].h)->GetYaxis();
    yt[i]->SetTitle(ytitle);
    (template[i].h)->SetTitle("nEvents");
    
    //     cout << "max[" << i << "] = " << max[i] << "; totMax = " << totMax << endl;
  }
  
  
  //   TAxis *xs =  (TAxis*) hs.GetXaxis();
  //  TAxis *ys =  (TAxis*) hs.GetYaxis();
  hs.SetTitle();
  
  
  if (stack) {
    float max_stack = hs.GetMaximum();
    float max_data = template[0].h->GetMaximum();
    
    
    if (max_data > max_stack)
      hs.SetMaximum(max_data);
    
    
    hs.Draw();
    //       TH1F *hline = hs.GetHistogram();
    //       hline->SetLineWidth(template[Nplots-1].line_width);
    //       hline->Draw("same");
    template[0].h->Draw("esame");
    
  } else { // not stack
    
    template[first].h->Draw(template[first].error);
    
    for (int i=0; i<Nplots; i++){
      if (i!=first)
	template[i].h->Draw(template[i].error+"same");
    }
    
    //esthetics: draw not-filled histograms over filled ones
    for (int i=0; i<Nplots; i++){
      if (!template[i].fill_color)
	template[i].h->Draw(template[i].error+"same");
    }
    
  }// not stack
  
  gPad->RedrawAxis();
  
  if (make_legend) {
    float legSize = 1- Nplots*0.05;
    float Ncorr = Nplots;
    TLegend *leg = new TLegend(0.75,legSize,0.995,0.995); 
    leg->AddEntry(template[0].h,template[0].legentry,"pl"); // Add legend entry for data first
    for (int i = Nplots - 1; i > 0; i--){
      //for (int i = 0; i > Nplots; i++){
      TString opt = "";
      if (template[i].error=="e") opt+="pl";
      if (template[i].fill_color) opt+="f";
      else if (template[i].line_color) opt+="l";
      //       if (template[i].fill_color) opt+="f";
      if (template[i].legentry != "") leg->AddEntry(template[i].h,template[i].legentry,opt);
      else                                    Ncorr --;  
    }
    
    leg->SetY1(1-Ncorr*0.05);
    leg->SetBorderSize(1);
    leg->SetFillColor(10);
    leg->Draw();
  }
  
  // Close the files we opened
  //for (int n = 0; n < 11; n++)
  //files[n].Close();
}
