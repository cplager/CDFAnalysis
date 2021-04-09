{
  gROOT->LoadMacro("root_macros/TDistribution.cc");
  //gROOT->LoadMacro("root_macros/DeltaPhi.C");
  //**********
      // Inputs
  //***********
      
      bool plot_only = 0; 
  bool plot_data = 1;
  bool make_legend = true;

  TString jetBin = "0";//1, 2, 3
  TString nTags = "pre";
  TString MetCut = "25";// 20-60
  bool singleTopQcdVeto = 1;  //0,1
  int NBINS =40;  
  TString systDesc = "_Q2.0"; // for shifted JES, b-tag, etc.

  bool setMinMax = false;
  float minHist = 0.; 
  float maxHist = 4.;
 
  TString ytitle = "# Events";

  // VARIABLES
  //   TString var = "ANN.3j";  TString xtitle = "ANN"; bool setMinMax = true; float minHist = 0.; float maxHist = 1.; TString var_desc = "ANN";
   TString var = "min(lep.Pt,249.)";  TString xtitle = "Lepton P_{T}";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 250; TString var_desc = "LepPt" ;   int NBINS = 25;
  //   TString var = "min(jet1.Et,419.)";  TString xtitle = "Leading Jet E_{T}";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 420; TString var_desc = "LeadJetEt" ;  int NBINS = 32;
  //  TString var = "min(jet2.Et,249.)";  TString xtitle = "Second Jet E_{T}";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 250; TString var_desc = "SecJetEt" ;  int NBINS = 25;
  //   TString var = "min(jet3.Et,149.)";  TString xtitle = "Third Jet E_{T}";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 150; TString var_desc = "ThirdJetEt" ;  int NBINS = 15;
  //   TString var = "min(evt.Met, 199.9)";  TString xtitle = "Missing E_{T}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 200.; TString var_desc = "met";   int NBINS=20;
  //   TString var = "min(evt.Ht,1199.9)";  TString xtitle = "H_{T}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 1200.; TString var_desc = "Ht";
   //     TString var = "min(fit.Mass,499.9)";  TString xtitle = "M_{reco}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 500.; TString var_desc = "mass";
   //   TString var = "evt.dR12";  TString xtitle = "#Delta R(j1,j2)";  float minHist = 0.;   float maxHist = 5.0; TString var_desc = "DeltaR12" ; 
  // TString var = "DeltaPhi(jet1.Phi, jet2.Phi)";  TString xtitle = "#Delta #phi(j1,j2)";  float minHist = 0.;   float maxHist = 3.2; TString var_desc = "DeltaPhi12" ; NBINS = 16;
  //  TString var = "evt.dR12";  TString xtitle = "#Delta R(j1,j2)";  float minHist = 0.;   float maxHist = 5.0; TString var_desc = "DeltaR12" ; 
  // TString var = "abs(jet1.Eta-jet2.Eta)";  TString xtitle = "#Delta #eta(j1,j2)";  float minHist = 0.;   float maxHist = 2.5; TString var_desc = tagger + "DeltaEta12" ; 
  //TString var = "jet1.Et+jet2.Et";  TString xtitle = "#Sum jet E_{T}";  float minHist = 0.;   float maxHist = 300; TString var_desc = tagger + "Sum jetEt" ; 
  // TString var = "DeltaPhi(jet1.Phi,lep.Phi)";  TString xtitle = "#Delta #Phi(j,lep)";  float minHist = 0.;   float maxHist = 3.5; TString var_desc = tagger + "DeltaPhiJL" ; 
  //  TString var = "jet1.Eta";  TString xtitle = "#Eta(j1)";  float minHist = -3.0;   float maxHist = 3.0; TString var_desc = "Etaj1" ; 

   TString plot_name = "plot/results/" + var_desc +"_" + jetBin +"j_pretag" + "_met" + MetCut + "_nbins" + NBINS;
   if (singleTopQcdVeto) plot_name += "_veto";

 //***** End: Inputs

  // overlay Nplots from different root files 
  const int Nplots = 11;

  TString LLSF = "2.676*lep.Eff*"; 
  // TString WSF = "1.5*";
  TString WHF = "1*"; // no need to re-scale HF in pre-tag plots
  TString ZLF = "1.4*";
  TString ZHF = "2.0*";
  TString WSF = "1.35*";
  
  if (systDesc == "_Q0.5") {
    cout << "Q^2 = 0.5" << endl;
    WSF = "1.22*";
  } else if (systDesc == "_Q2.0") {
    cout << "Q^2 = 2.0" << endl;
    WSF = "1.41*";
  }
  TString qcdSF= "1.0*";
  if (jetBin == "1"){
    qcdSF= "4.7162*";
    //    WSF = "1.51706*"; 
  }
  else if (jetBin == "2"){
    qcdSF= "8.09709*";
    //    WSF = "1.52571*"; 
  }
  else if (jetBin == "3"){
    qcdSF= "7.38736*";
    //    WSF = "1.63274*"; 
  }
  
  else if (jetBin == "4"){
    qcdSF= "3.0977*";
    //    WSF = "1.86701*"; 
  }
  else if (jetBin == "45"){
    qcdSF= "3.1036*";
    //    WSF = "1.89846*"; 
  }
  else if (jetBin == "5"){
    qcdSF= "2.33494*";
    //    WSF = "3.17187*"; 
  }
  
 bool stack = 1; //false; // stack the MC ( do not forget that "0" is data)

  bool norm = 0; //1; //false; // switch to normalize the histograms to the same area
  bool normToData = 0; //1; //false; // normalize to "0" histogram

  bool doKS = false; // calculate KS probabilities

  TString tree_name = "nt";

  //////////////////////////////  EVENT SELECTION ///////////////////////////////////

  /*  TString condition;
  // jetBin
  if (jetBin == "5")
    condition = " evt.nJets >= 5";
  else if (jetBin == "45")
    condition = " evt.nJets >= 4";
  else
    condition = " evt.nJets == " + jetBin;
  */
  
  TString condition = "evt.isGoodSiRun "; 
  condition += (" && " + var + " > -999");

  if (singleTopQcdVeto) condition += " && evt.passedQCDveto  ";
  // MetCut 
  condition = " evt.Met > " + MetCut + " && " + condition;
 
 // jetBin
    if (jetBin == "5")
      condition = " evt.nJets >= " + jetBin + " && " + condition;
    else if (jetBin == "45")
      condition = " evt.nJets >= 4 && " + condition;
    else
      condition = " evt.nJets == " + jetBin + " && " + condition;

  
 cout << "Plotting " << var << endl;
 cout << "Event Selection = " << condition << endl;
 
 
  //////////////////////////////////  SAMPLES TO USE /////////////////////////////////

  TDistribution template[Nplots];  
  for (int ind = 0; ind < Nplots; ind++){
    template[ind].var = var;
    template[ind].condition = condition;
  }

  int ind = 0;
  template[ind].sample = "data"; 
  template[ind].legentry = "CDF Data (2.7 fb^{-1})";   
  template[ind].marker_style = 8;
  template[ind].error = "e";
  template[ind++].condition = condition;

  template[ind].sample = "anti"; 
  template[ind].legentry = "QCD";   
  template[ind].line_color = 1; //117;
  template[ind].fill_color = 117;
  template[ind].line_width = 1;
  template[ind].marker_color = 117;
  template[ind++].condition = qcdSF + "(" + condition + ")";
  
  template[ind].sample = "top"; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + "evt.AlpgenW*(" + condition + ")"; 
  
  template[ind].sample = "stop"; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + condition + ")"; 
  
  template[ind].sample = "vv"; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + "evt.AlpgenW*(" + condition + ")"; 
  
  template[ind].sample = "zlf"; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZLF + LLSF + "evt.AlpgenW*(" + condition + ")"; 
  
  template[ind].sample = "zhf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 103;//1; // = 209;
  template[ind].fill_color = 103; //209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + LLSF + "evt.AlpgenW*(" + condition + ")"; 
  
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = ""; //W + c";  
  template[ind].line_color = 30; //1; // = 65;
  template[ind].fill_color = 30; //65;
  template[ind].line_width =  1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WHF + WSF +LLSF + "evt.AlpgenW*(" + condition + ")"; 
  
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = ""; //W+ c#bar{c}"; 
  template[ind].line_color =  30; //1;
  template[ind].fill_color = 30; //4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WHF + WSF +LLSF + "evt.AlpgenW*(" + condition + ")"; 
  
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";//W+ b#bar{b}"; 
  template[ind].line_color =  30; //1;
  template[ind].fill_color = 30; //51;
  template[ind].line_width = 1;
  template[ind].marker_color = 51;
  template[ind++].condition =  WHF + WSF + LLSF + "evt.AlpgenW*(" + condition + ")"; 
  
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+jets";//W+ q#bar{q}"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + condition + ")"; 


  // @@@@@@@@@@@@@@@@@@@@@ //
   // @@ ========  MAGIC ====@@@@@@ //
   // @@@@@@@@@@@@@@@@@@@@@ //
   gROOT->Macro("root_macros/Overlay_pretag.C");


   float Fqcd = template[1].h->Integral() / template[0].h->Integral();
   cout << "Fqcd: " << Fqcd << endl;
   
   //cout << "data bin content: " << template[0].h->GetBinContent(1) << endl;

   float N_Wjets = 0;
   float N_Wlf = 0;
   float N_Wbb = 0;
   float N_Wcc = 0;
   float N_Wc = 0;
   float N_ewktop = 0;
   float N_qcd = 0;
   float N_data = 0;
   for (int i = 0; i < Nplots; i++){
     if (template[i].sample == "wlf")
       N_Wlf +=  template[i].h->Integral();
     if (template[i].sample == "wcc")
       N_Wcc +=  template[i].h->Integral();
     if (template[i].sample == "wbb")
       N_Wbb +=  template[i].h->Integral();
     if (template[i].sample == "wc")
       N_Wc +=  template[i].h->Integral();
     if (template[i].sample == "wlf" || template[i].sample == "wbb" || template[i].sample == "wcc"  || template[i].sample == "wc")
       N_Wjets += template[i].h->Integral();
     if  (template[i].sample == "stop" || template[i].sample == "vv" || template[i].sample == "zlf" || template[i].sample == "zhf")
       N_ewktop += template[i].h->Integral();
     if  (template[i].sample == "anti")
       N_qcd +=  template[i].h->Integral();
     if  (template[i].sample == "data")
       N_data +=  template[i].h->Integral();
   }

   float N_pred = N_data - N_qcd - N_ewktop;
   float Kpretag = 0; //N_pred / N_Wjets; 
   float hf = 0; //(N_Wjets - N_Wlf) / N_Wjets;

   cout << "W + jets: " <<  N_Wjets << endl;
   cout << "W + bb: " <<  N_Wbb << endl;
   cout << "W + cc: " <<  N_Wcc << endl;
   cout << "W + c: " <<  N_Wc << endl;
   cout << "W + LF: " <<  N_Wlf << endl;
   //   cout << "hf: " << hf << endl;
   cout << "Ewk: " <<  N_ewktop << endl;
   cout << "QCD: " <<  N_qcd << endl;
   cout << "Total predicted " <<  N_Wjets + N_ewktop + N_qcd << endl;
   cout << "Observed " <<  N_data << "; N_pred = " << N_pred << endl;
   cout << "W+jets frac  of MC predicted = " <<  N_Wjets / (N_ewktop + N_Wjets) << endl;
   
   cout << " ============== correction to W Kpretag = " << Kpretag << " +/- " << hf*Kpretag << " ========== " << endl;
   //   cout << "The result is valid only if WSF = 1; WSF = " << WSF << endl; 

   //Input to K factor script
   if ( nTags=="pre" )
     cout << "Value Fqcd_pretag = Value(" << Fqcd <<  ", " << 0.30*Fqcd  << "); " << endl;
   else 
     cout << "Value Fqcd_tag = Value(" << Fqcd <<  ", " << 0.30*Fqcd  << "); " << endl;

   cout << " correction to QCD SF = " << (double) N_data*Fqcd / N_qcd << endl;

   c1->SaveAs(plot_name + ".gif");
   c1->SaveAs(plot_name + ".eps");
   gROOT->ProcessLine(".! epstopdf " + plot_name + ".eps");
   
   //   c1->SetLogy();
   //c1->SaveAs(plot_name + "_log.gif");
   //c1->SaveAs(plot_name + "_log.eps");
   //gROOT->ProcessLine(".! epstopdf " + plot_name + "_log.eps");

   //   gApplication->Terminate();


}
