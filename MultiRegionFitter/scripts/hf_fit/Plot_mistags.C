{
  gROOT->LoadMacro("root_macros/TDistribution.cc");

  //**********
  // This script can be used for plotting event variables and NOT for hf content fitting
      // HF K-factor is assumed 1, all LF jets are called mistags
  //***********

      bool plot_only = 0; 
  bool plot_data = 1;
  bool make_legend = 1;

  TString jetBin = "1";//1, 2, 3
  TString nTags = "1";// 1,2,12

  //TAGGERS
  TString tagger = "";   TString tagger_ind = "T"; TString tagger_desc = "";
  //   TString tagger = "L";   TString tagger_ind = "L";  TString tagger_desc = " Loose ";
  //     TString tagger = "U";   TString tagger_ind = "U";  TString tagger_desc = " UltraTight ";
  TString MetCut = "25";// 20-60
  bool singleTopQcdVeto = 1;  //0,1
  int NBINS =32;  

  TString ytitle = "# Events";
 
  // VARIABLES
  //   TString var = "ANN.3j";  TString xtitle = "ANN"; bool setMinMax = true; float minHist = 0.; float maxHist = 1.; TString var_desc = "ANN";
  //   TString var = "min(fit.Mass,499.9)";  TString xtitle = "M_{reco}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 500.; TString var_desc = "mass";
  // TString var = "min(evt.Ht, 799.9)";  TString xtitle = "H_{T}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 800.; TString var_desc = "Ht";
    TString var = "min(lep.Pt,249.)";  TString xtitle = "Lepton P_{T}";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 250; TString var_desc = "LepPt" ;   int NBINS = 25;
  //TString var = "min(jet1.Et,319.)";  TString xtitle = "Leading Jet E_{T}";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 320; TString var_desc = tagger + "LeadJetEt" ;  int NBINS = 32;
  //TString var = "jet1.deltaPhiMet";  TString xtitle = "#Delta #phi (jet, met)";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 3.15; TString var_desc = tagger + "dPhijet1met" ;  int NBINS = 32;
  // TString var = "min(evt.Met, 199.9)";  TString xtitle = "Missing E_{T}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 200.; TString var_desc = "met"; int NBINS = 20;
  //  TString var = "evt.dR12";  TString xtitle = "#Delta R(j1,j2)";  float minHist = 0.;   float maxHist = 5.0; TString var_desc = tagger + "DeltaR12" ; 
  // TString var = "abs(jet1.Eta-jet2.Eta)";  TString xtitle = "#Delta #eta(j1,j2)";  float minHist = 0.;   float maxHist = 2.5; TString var_desc = tagger + "DeltaEta12" ; 
  //   TString var = "jet1.Et";  TString xtitle = "Leading Jet E_{T}";  float minHist = 0.;   float maxHist = 320; TString var_desc = tagger + "LeadJetEt" ; 
  // TString var = "jet2.Et";  TString xtitle = "Second Jet E_{T}";  float minHist = 0.;   float maxHist = 300; TString var_desc = tagger + "SecondJetEt" ; 
 //TString var = "jet1.Et+jet2.Et";  TString xtitle = "#Sum jet E_{T}";  float minHist = 0.;   float maxHist = 300; TString var_desc = tagger + "Sum jetEt" ; 
  // TString var = "DeltaPhi(jet1.Phi,jet2.Phi)";  TString xtitle = "#Delta #Phi(j,j)";  float minHist = 0.;   float maxHist = 3.5; TString var_desc = tagger + "DeltaPhiJJ" ; 
  // TString var = "DeltaPhi(jet1.Phi,lep.Phi)";  TString xtitle = "#Delta #Phi(j,lep)";  float minHist = 0.;   float maxHist = 3.5; TString var_desc = tagger + "DeltaPhiJL" ; 
  //  TString var = "jet1.Eta";  TString xtitle = "#Eta(j1)";  float minHist = -2.5;   float maxHist = 2.5; TString var_desc = tagger + "Etaj1" ; 
  TString plot_name = "hf_fit/results/" + var_desc +"_" + jetBin +"j_" + nTags + "tag" + tagger + "_met" + MetCut + "_nbins" + NBINS;
  if (singleTopQcdVeto) plot_name += "_veto";

 //***** End: Inputs

  // overlay Nplots from different root files 
  const int Nplots = 36;

  TString LLSF = "2.676*lep.Eff*"; 
  TString ZLF = "1.4*";
  TString ZHF = "2.0*";
  TString qcdSF ="1.*";
  TString WSF ="1.35*";

  // qcdSF is obtained from met fit
  // WSF is obtained from pre-tag fit, aka M2-like
  // M3 assumes a common WSF=1.35 obtained from 0 -jet bin
  if (jetBin == "1") {
    if (nTags == "1" && tagger_ind == "T"){
      if (singleTopQcdVeto)   	qcdSF = "0.0707343*";
      else                                   qcdSF = "0.0369115*";
    }

  }  else if (jetBin == "2") {
    
    if (nTags == "12" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "0.293537*";
      else                                qcdSF = "0.0930628*";
    }

    if (nTags == "1" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "0.285361*";
    }

    if (nTags == "2" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "0.00443142*";
    }


  } else if (jetBin == "3") {


    if (nTags == "12" && tagger_ind == "T"){
      if (singleTopQcdVeto){   
	    qcdSF = "0.26001*";
      } else { // no qcd veto
	qcdSF = "0.146688*";
      }
    }
    if (nTags == "1" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "0.287186*";
    }
    
    if (nTags == "2" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "2.767473e-10*";
    }
    

 } else if (jetBin == "4") {

    if (nTags == "12" && tagger_ind == "T")
      qcdSF = "0.147438*";

    if (nTags == "1" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "0.140266*";
    }
    
    if (nTags == "2" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "4.74e-09*";
    }

    if (nTags == "12" && tagger_ind == "L")
      qcdSF = "0.343974*";
    
  } else if (jetBin == "45") {
    
    if (nTags == "12" && tagger_ind == "T")
      qcdSF = "0.195786*";
    
    if (nTags == "12" && tagger_ind == "L")
      qcdSF = "0.36033*";
    
  } else if (jetBin == "5") {
    if (nTags == "12" && tagger_ind == "T")
      qcdSF = "1.42289e-08*";

    if (nTags == "1" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "0.146018*";
    }
    
    if (nTags == "2" && tagger_ind == "T"){     
      if (singleTopQcdVeto)   qcdSF = "2.17827e-08*";
    }
  }

  // 0- jet bin sf
  // WSF = "1.35*";

 TString LSF = "1*"; 
  TString Bprob ="1*"; // event tagging probability
  if (nTags != "pre")
    Bprob = "evt.bProb" +  nTags + tagger_ind + "*";

  bool stack = 1; //false; // stack the MC ( do not forget that "0" is data)

  bool norm = 0; //1; //false; // switch to normalize the histograms to the same area
  bool normToData = 0; //1; //false; // normalize to "0" histogram

  //  if (plot_only){ norm = 1; normToData = 1; }
  bool doKS = false; // calculate KS probabilities

  bool setMinMax = true;

  TString tree_name = "nt";

  //////////////////////////////  EVENT SELECTION ///////////////////////////////////

  TString condition = "evt.isGoodSiRun"; 
  // condition = " ANN.3j < 0.5 && " + condition;
  condition += (" && " + var + " > -999");
  
  //  TString condition_qcd = condition;
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
  
  TString condition_data = condition;
  
  if (nTags == "12")
    condition_data += " && evt.n" + tagger_ind + "posTags >= 1  "; 
  else 
    condition_data += " && evt.n" + tagger_ind + "posTags == " + nTags; 
  
  cout << "Plotting " << var << endl;
  cout << "Event Selection = " << condition_data << endl;
  
  
  TString bottom = "evt." + tagger_ind + "matchB && "; 
  TString charm = "evt." + tagger_ind + "matchC && "; 
  TString HF = " ( evt." + tagger_ind + "matchB || evt." + tagger_ind + "matchC ) && ";
  TString LF = "  ( !evt." + tagger_ind + "matchB && !evt." + tagger_ind + "matchC ) && ";
  
  TString bjet1 = "(bjet" + tagger + "1.flavor == 5 && bjet" + tagger + "2.flavor < 0) && ";
  TString cjet1 = "(bjet" + tagger + "1.flavor == 4 && bjet" + tagger + "2.flavor < 0) && ";
  TString qjet1 = "(bjet" + tagger + "1.flavor == 1 && bjet" + tagger + "2.flavor < 0) && ";
  TString bb    = "(bjet" + tagger + "1.flavor == 5 && bjet" + tagger + "2.flavor == 5) && ";
  TString cc    = "(bjet" + tagger + "1.flavor == 4 && bjet" + tagger + "2.flavor == 4) && ";
  TString qq    = "(bjet" + tagger + "1.flavor == 1 && bjet" + tagger + "2.flavor == 1) && ";
  TString bc    = "( (bjet" + tagger + "1.flavor == 5 && bjet" + tagger + "2.flavor == 4) || (bjet"
    + tagger + "1.flavor == 4 && bjet" + tagger + "2.flavor == 5) ) && ";
  TString bq    = "( (bjet" + tagger + "1.flavor == 5 && bjet" + tagger + "2.flavor == 1) || (bjet"
    + tagger + "1.flavor == 1 && bjet" + tagger + "2.flavor == 5) ) && ";
  TString cq    = "( (bjet" + tagger + "1.flavor == 4 && bjet" + tagger + "2.flavor == 1) || (bjet"
    + tagger + "1.flavor == 1 && bjet" + tagger + "2.flavor == 4) ) && ";
  
  
  
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
  template[ind++].condition = condition_data;
  
  template[ind].sample = "anti"; 
  template[ind].legentry = "QCD";   
  template[ind].line_color = 1; //117;
  template[ind].fill_color = 33;
  template[ind].line_width = 1;
  template[ind].marker_color = 117;
  template[ind++].condition = qcdSF + "(" + condition_data + ")"; 
  
  template[ind].sample = "stop"; 
  template[ind].legentry = "EW";   
  template[ind].line_color = 89; // = 102;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "vv"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 105;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "zlf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 103;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "zhf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 209;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + HF +condition + ")"; 
  // EW
  
  //Mistags
  template[ind].sample = "stop"; 
  template[ind].legentry = "W + q#bar{q}"; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF   + Bprob + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "vv"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF   + Bprob + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "zlf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF  + Bprob + LLSF + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "zhf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF  + Bprob + LLSF + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  // ind = 10
  template[ind].sample = "top"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF  + Bprob + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wbb"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF  + Bprob + LLSF + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wcc"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF  + Bprob + LLSF + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wc"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF  + Bprob + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wlf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF  + Bprob + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 
  // end Mistags
  
  template[ind].sample = "wc"; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  
  // Wcc
  template[ind].sample = "wlf"; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 62;
  template[ind].line_width = 1;
  template[ind].fill_color = 62;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")";

  template[ind].sample = "wbb"; 
  template[ind].legentry = "";  
  template[ind].line_color = 62;
  template[ind].fill_color = 62;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  
  template[ind].sample = "wcc"; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 62;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  // Wcc
  
  // Wbb
  template[ind].sample = "wlf"; 
  template[ind].legentry = "W+ b#bar{b}"; 
  template[ind].line_color = 56;
  template[ind].line_width = 1;
  template[ind].fill_color = 56;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  // ind = 20
  template[ind].sample = "wc"; 
  template[ind].legentry = "";  
  template[ind].line_color = 56;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  
  template[ind].sample = "wcc"; 
  template[ind].legentry = "";  
  template[ind].line_color = 56;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  
  template[ind].sample = "wbb"; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")";
  // Wbb
  
  template[ind].sample = "top"; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + HF + condition + ")"; 
  

  // Single tag split templates

  // EW_b
  template[ind].sample = "stop"; 
  template[ind].legentry = "EW_b";   
  template[ind].line_color = 89; // = 102;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition + ")"; 
  
  template[ind].sample = "vv"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 105;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bjet1 + condition + ")"; 
 
  template[ind].sample = "zlf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 103;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bjet1 + condition + ")"; 
  template[ind].sample = "zhf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bjet1 + condition + ")";
  // end EW_b

  // EW_c
  template[ind].sample = "stop"; 
  template[ind].legentry = "EW_c";   
  template[ind].line_color = 89; // = 102;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition + ")"; 

  template[ind].sample = "vv"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 105;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + cjet1 +condition + ")"; 
  
  // ind = 30
  template[ind].sample = "zlf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 103;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cjet1 +condition + ")"; 
  
  template[ind].sample = "zhf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cjet1 +condition + ")"; 
  // end EW_c

  // EW_q
  template[ind].sample = "stop"; 
  template[ind].legentry = "EW_q";   
  template[ind].line_color = 89; // = 102;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + qjet1 +condition + ")"; 

  template[ind].sample = "vv"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 105;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + qjet1 +condition + ")"; 
  
  template[ind].sample = "zlf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 103;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + qjet1 +condition + ")"; 
  
  template[ind].sample = "zhf"; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + qjet1 +condition + ")"; 
  // end EW_q 35





  
  
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
  // @@ ========  MAGIC ====@@@@@@ //
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
  gROOT->Macro("root_macros/Overlay.C");
  

  if (!plot_only){
    cout.precision(3);
    const int Nfit = 26; // # of fitted histograms
    double param[Nfit];
    
    TH1F *hdata = (TH1F*)(template[0].h)->Clone("dat");
    
    TH1F *hmc[Nfit];
    TString fit_name[Nfit] = {"Data", "top", "Wbb", "Wcc", "Wc", "Wqq", "EW", "QCD",
			      "top_b", "top_c", "top_q", "Wbb_b", "Wbb_c", "Wbb_q",
			      "Wcc_b", "Wcc_c", "Wcc_q", "Wc_b", "Wc_c", "Wc_q",
			      "Wqq_b", "Wqq_c", "Wqq_q", "EW_b", "EW_c", "EW_q"};
    
    int fit_assoc[Nplots];
    bool fit_init[Nfit] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i=0; i<Nplots; i++){ //intialize fit_assoc
      if (0 == i)
	fit_assoc[i] = 0;
      else if (1 == i)
	fit_assoc[i] = 7;
      else if (i<=5)
	fit_assoc[i] = 6;
      else if (i<=14)
	fit_assoc[i] = 5;
      else if (i== 15)
	fit_assoc[i] = 4;
      else if  (i>=16 &&  i <= 18)
	fit_assoc[i] = 3;
      else if  (i>=19 &&  i <= 22)
	fit_assoc[i] = 2;
      else if (i == 23)
	fit_assoc[i] = 1;
      else if (i <= 27)
	fit_assoc[i] = 23;
      else if (i <= 31)
	fit_assoc[i] = 24;
      else if (i <= 35)
	fit_assoc[i] = 25;

    } // end assoc loop
    
    for (int i=1; i<Nplots; i++) { //intialize histograms used in the fit    
      //  cout << "i = " << i << "; fit_assoc = " << fit_assoc[i] << endl ; //"; pointer: " << hmc[ fit_assoc[i] ] << endl;
      if (! fit_init [ fit_assoc[i] ]){
	hmc[ fit_assoc[i] ] = (TH1F*)(template[i].h)->Clone(fit_name[ fit_assoc[i] ]);
	fit_init [ fit_assoc[i] ] = 1;
      }
      else
	hmc[ fit_assoc[i] ]->Add(template[i].h);
    }
    cout << "Filled histograms" << endl;
    
    TFile ffit("fit_inputs/" + var_desc + "_" + jetBin + "j_" + nTags + "tag_mistags.root", "recreate");
    cout << "About to write histos" << endl;
    hdata->Write();
    for (int i = 1; i < Nfit; i++)
      hmc[i]->Write();
    ffit->Close();
    cout << "Wrote histos" << endl;
    cout.precision(5);
  } // if plot_only
  





  
  if ( template[0].h->Integral() != 0){
    float Fqcd = template[1].h->Integral() / template[0].h->Integral();
    cout << "Fqcd: " << Fqcd << endl;
  }
  
  float N_EW = 0;
  float N_qcd = 0;
  float N_data = 0;
  float N_stop = 0;
  float N_top = 0;
  float N_vv = 0;
  float N_zhf = 0;
  float N_zlf = 0;
  float N_wc = 0;
  float N_wbb = 0;
  float N_wcc = 0;
  float N_mistags = 0;
  for (int i=0; i<Nplots; i++){
    if  (i == 1)      N_qcd +=  template[i].h->Integral();
    else if  (i == 0)     N_data +=  template[i].h->Integral();
    else if  (i == 2)       N_top +=  template[i].h->Integral();
    else if  (i == 4)       N_vv +=  template[i].h->Integral();
    else if  (i == 3)       N_stop +=  template[i].h->Integral();
    else if  (i == 5)       N_zlf +=  template[i].h->Integral();
    else if  (i == 6)       N_zhf +=  template[i].h->Integral();
    else if  (i >= 7 && i <= 15)       N_mistags +=  template[i].h->Integral();
    else if  (i == 16)       N_wc +=  template[i].h->Integral();
    else if  (i >= 17 && i <= 19)       N_wcc +=  template[i].h->Integral();
    else if  (i >= 20 && i <= 23)       N_wbb +=  template[i].h->Integral();
    else cout << "Split single tags" << endl;
  }
  
  N_EW = N_vv + N_stop + N_zlf + N_zhf;
  
  if (N_qcd != 0)
    cout << " correction to QCD SF = " << (double) N_data*Fqcd / N_qcd << endl;
  
  cout << "qcd & " << N_qcd << endl;
  cout << "stop(hf) & " << N_stop << endl;
  cout << "top(hf)  & " << N_top << endl;
  cout << "vv(hf) & " << N_vv << endl;
  cout << "zhf (hf)& " << N_zhf << endl;
  cout << "zlf(hf) & " << N_zlf << endl;
  cout << "wc & " << N_wc << endl;
  cout << "wbb & " << N_wbb << endl;
  cout << "wcc & " << N_wcc << endl;
  cout << "mistags & " << N_mistags << endl;
  cout << "EW & " << N_EW << endl;
  cout << " ---------------------------------- " << endl;
  cout << "Total & " << N_qcd+N_stop+N_top+N_vv+N_zhf+N_zlf+N_wc+N_wbb+N_wcc+N_mistags << endl;
  cout << "data & " << N_data << endl;
  cout << endl;
  c1->SaveAs(plot_name + ".gif");
  c1->SaveAs(plot_name + ".eps");
  gROOT->ProcessLine(".! epstopdf " + plot_name + ".eps");
  
  
  c1->SetLogy();
  c1->SaveAs(plot_name + "_log.gif");
  c1->SaveAs(plot_name + "_log.eps");
  gROOT->ProcessLine(".! epstopdf " + plot_name + "_log.eps");
  
  //   gApplication->Terminate();
}
