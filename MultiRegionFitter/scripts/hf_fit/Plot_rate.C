//void Plot_rate() {
{
  gROOT->LoadMacro("root_macros/TDistribution.cc");

  //**********
  // This script can be used for plotting event variables and NOT for hf content fitting
  //***********

  bool plot_only = 0; 
  bool plot_data = 1;
  bool make_legend = true;

  TString jetBin = "5"; // 1, 2, 3
  TString nTags = "2"; // 1, 2, 12
  TString systDesc = "_JESm0.5";

  //TAGGERS
  TString tagger = "";   TString tagger_ind = "T"; TString tagger_desc = "";
  //   TString tagger = "L";   TString tagger_ind = "L";  TString tagger_desc = " Loose ";
  //     TString tagger = "U";   TString tagger_ind = "U";  TString tagger_desc = " UltraTight ";
  TString MetCut = "25";// 20-60
  bool singleTopQcdVeto = 1;  //0,1
  int NBINS = 32;  

  TString ytitle = "# Eevents";
  // VARIABLES
  //   TString var = "ANN.3j";  TString xtitle = "ANN"; bool setMinMax = true; float minHist = 0.; float maxHist = 1.; TString var_desc = "ANN";
  //   TString var = "min(fit.Mass,499.9)";  TString xtitle = "M_{reco}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 500.; TString var_desc = "mass";
  // TString var = "min(evt.Ht, 799.9)";  TString xtitle = "H_{T}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 800.; TString var_desc = "Ht";
  //  TString var = "min(lep.Pt,249.)";  TString xtitle = "Lepton P_{T}";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 250; TString var_desc = "LepPt" ;   int NBINS = 25;
  //TString var = "min(jet1.Et,319.)";  TString xtitle = "Leading Jet E_{T}";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 320; TString var_desc = tagger + "LeadJetEt" ;  int NBINS = 32;
  //TString var = "jet1.deltaPhiMet";  TString xtitle = "#Delta #phi (jet, met)";  bool setMinMax = true;  float minHist = 0.;   float maxHist = 3.15; TString var_desc = tagger + "dPhijet1met" ;  int NBINS = 32;
   TString var = "min(evt.Met, 199.9)";  TString xtitle = "Missing E_{T}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 200.; TString var_desc = "met"; int NBINS = 20;
  //  TString var = "evt.dR12";  TString xtitle = "#Delta R(j1,j2)";  float minHist = 0.;   float maxHist = 5.0; TString var_desc = tagger + "DeltaR12" ; 
  // TString var = "abs(jet1.Eta-jet2.Eta)";  TString xtitle = "#Delta #eta(j1,j2)";  float minHist = 0.;   float maxHist = 2.5; TString var_desc = tagger + "DeltaEta12" ; 
  //   TString var = "jet1.Et";  TString xtitle = "Leading Jet E_{T}";  float minHist = 0.;   float maxHist = 320; TString var_desc = tagger + "LeadJetEt" ; 
  // TString var = "jet2.Et";  TString xtitle = "Second Jet E_{T}";  float minHist = 0.;   float maxHist = 300; TString var_desc = tagger + "SecondJetEt" ; 
 //TString var = "jet1.Et+jet2.Et";  TString xtitle = "#Sum jet E_{T}";  float minHist = 0.;   float maxHist = 300; TString var_desc = tagger + "Sum jetEt" ; 
 //TString var = "DeltaPhi(jet1.Phi,jet2.Phi)";  TString xtitle = "#Delta #Phi(j,j)";  float minHist = 0.;   float maxHist = 3.5; TString var_desc = tagger + "DeltaPhiJJ" ; 
  // TString var = "DeltaPhi(jet1.Phi,lep.Phi)";  TString xtitle = "#Delta #Phi(j,lep)";  float minHist = 0.;   float maxHist = 3.5; TString var_desc = tagger + "DeltaPhiJL" ; 
  //  TString var = "jet1.Eta";  TString xtitle = "#Eta(j1)";  float minHist = -2.5;   float maxHist = 2.5; TString var_desc = tagger + "Etaj1" ; 
  TString plot_name = "plot/results/";
	plot_name += var_desc + systDesc;
	plot_name += "_";
	plot_name += jetBin;
	plot_name += "j_";
	plot_name += nTags;
	plot_name += "tag";
	plot_name += tagger;
	plot_name += "_met";
	plot_name += MetCut;
	plot_name += "_nbins";
	plot_name += NBINS;
	
  if (singleTopQcdVeto) plot_name += "_veto";

 //***** End: Inputs

  // overlay Nplots from different root files 
  const int Nplots = 125;

  TString LLSF = "2.676*lep.Eff*"; 

  TString ZLF = "1.4*";
  TString ZHF = "2.0*";
  TString qcdSF = "0.*";
  TString WSF = "1.35*";

  // qcdSF is obtained from met fit
  if (jetBin == "1" && tagger_ind == "T") {
    qcdSF = "0.0707343*";
  } else if (jetBin == "1" && tagger_ind == "L") {
    qcdSF = "0.107003*";
  } else if (jetBin == "1" && tagger_ind == "U") {
    qcdSF = "0.0243146*";
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "T") {
    qcdSF = "0.285361*"; 
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "0.218136*"; 
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "0.0896162*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "0.00562856*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "0.00851716*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "0.00184308*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "0.287186*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "0.614599*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "0.0784804*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "1.21568e-07*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "4.46547e-09*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "1.89621e-08*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "0.14526*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "0.350817*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "9.44046e-09*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "6.81147e-10*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "3.61481e-08*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "2.42987e-10*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "0.295103*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "0.212022*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "1.31753e-09*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "2.01568e-10*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "1.70928e-10*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "1.08137e-11*"; 
  }

  TString Bprob ="1*"; // event tagging probability
  if (nTags != "pre") {
    Bprob = "evt.bProb" +  nTags + tagger_ind + "*";
    if (systDesc == "_btagm1")
      Bprob = "evt.bProb" +  nTags + tagger_ind + "mn*";
    else if (systDesc == "_btagp1")
      Bprob = "evt.bProb" +  nTags + tagger_ind + "pl*";
  }

  TString Mprob ="1*"; // event mistagging probability
  if (nTags != "pre") {
    Mprob = "evt.bProb" +  nTags + tagger_ind + "*"; // mistag rate normal
    if (systDesc == "_mtagm1") 
      Mprob = "evt.misProb" +  nTags + tagger_ind + "mn*"; // mistag - 20%
    else if (systDesc == "_mtagp1")
      Mprob = "evt.misProb" +  nTags + tagger_ind + "pl*"; // mistag + 20%
  }

  bool stack = 1; //false; // stack the MC ( do not forget that "0" is data)

  bool norm = 0; //1; //false; // switch to normalize the histograms to the same area
  bool normToData = 0; //1; //false; // normalize to "0" histogram

  //  if (plot_only){ norm = 1; normToData = 1; }
  bool doKS = false; // calculate KS probabilities
	  bool doChi2 = false;

  bool setMinMax = true;

  TString tree_name = "nt";
TString histoTitle = "nEvents";	

  //////////////////////////////  EVENT SELECTION ///////////////////////////////////

  TString condition = "evt.isGoodSiRun "; //&& lep.Pt < 200 "; 
  // condition = " ANN.3j < 0.5 && " + condition;
  condition += (" && " + var + " > -999");
  
  //  TString condition_qcd = condition;
  if (singleTopQcdVeto) condition += " && evt.passedQCDveto  ";
  // MetCut 
  condition = " evt.Met > " + MetCut + " && " + condition;
 
  // jetBin
  if (jetBin == "5")
    condition = " evt.nJets >= 5 && " + condition;
  else if (jetBin == "45")
    condition = " evt.nJets >= 4 && " + condition;
  else
    condition = " evt.nJets == " + jetBin + " && " + condition;
  
 TString condition_data = condition;

 if (nTags == "12")
   condition_data += " && evt.n" + tagger_ind + "posTags >= 1  "; 
 else 
   condition_data += " && evt.n" + tagger_ind + "posTags == " + nTags; 

// add to make sure consistency between "K-factor" and "Higgs" ntuples:
 // condition_data += " && evt.Run <= 261005 && lep.Isol < 0.1";// && evt.hasTightLepton";	 
 // condition += " && evt.Run <= 261005 && lep.Isol < 0.1";
	 
 cout << "Plotting " << var << endl;
 cout << "Event Selection = " << condition_data << endl;
 
 
 TString bottom = "evt." + tagger_ind + "matchB && "; 
 TString charm = "evt." + tagger_ind + "matchC && "; 
 TString evtbottom = "evt.matchBottom && ";
 TString evtcharm = "( evt.matchCharm && !evt.matchBottom) && ";
 TString evtLF = "( !evt.matchBottom && !evt.matchCharm ) && ";
 TString evtnoB = "(!evt.matchBottom) && ";
 TString HF = " ( evt." + tagger_ind + "matchB || evt." + tagger_ind + "matchC ) && " ;
 TString LF = "  ( !evt." + tagger_ind + "matchB && !evt." + tagger_ind + "matchC ) && " ;
 
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
  template[ind].fill_color = 14;
  template[ind].line_width = 1;
  template[ind].marker_color = 14;
  template[ind++].condition = qcdSF + "(" + condition + ")"; 
  
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bottom +condition + ")"; 
  
  // 5
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + charm +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 218;
  template[ind].line_width = 1;
  template[ind].marker_color = 218;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bottom +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 218;
  template[ind].line_width = 1;
  template[ind].marker_color = 218;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + charm +condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 210;
  template[ind].line_width = 1;
  template[ind].marker_color = 210;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bottom +condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 210;
  template[ind].line_width = 1;
  template[ind].marker_color = 210;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + charm +condition + ")"; 
  
  // 10
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bottom +condition + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + charm +condition + ")"; 
  
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Mistags"; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + Mprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + Mprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF + Mprob + LLSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  // 15
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF + Mprob + LLSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + Mprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + Mprob + LLSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + Mprob + LLSF + "evt.AlpgenW*(" + evtnoB + LF + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtnoB + LF + condition + ")"; 

  // 20
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtLF + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + charm + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtLF + charm + condition + ")";

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 4;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + charm + condition + ")"; 
  
  // 25
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ b#bar{b}"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtLF + bottom + condition + ")"; 
  
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 51;
  template[ind].fill_color = 51;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bottom + condition + ")"; 
  
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 51;
  template[ind].fill_color = 51;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bottom + condition + ")"; 
  
  // 28
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 51;
  template[ind].line_width = 1;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 


   // double tags
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + bb + condition + ")"; 
  
  // 30
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + bc + condition + ")"; 
  
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 
  
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + cc + condition + ")"; 
  
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + cq + condition + ")"; 
  
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Mprob + "evt.AlpgenW*(" + qq + condition + ")"; 
  

  // single top 35
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bb + condition + ")"; 
  
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bc + condition + ")"; 
  
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 
  
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cc + condition + ")"; 
  
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cq + condition + ")"; 
  
  // 40
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Mprob + "evt.AlpgenW*(" + qq +condition + ")"; 
  

  // diboson
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 218;
  template[ind].line_width = 1;
  template[ind].marker_color = 218;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bb + condition + ")"; 
  
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 218;
  template[ind].line_width = 1;
  template[ind].marker_color = 218;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bc + condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 218;
  template[ind].line_width = 1;
  template[ind].marker_color = 218;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 218;
  template[ind].line_width = 1;
  template[ind].marker_color = 218;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + cc + condition + ")"; 
  
  // 45
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 218;
  template[ind].line_width = 1;
  template[ind].marker_color = 218;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + cq + condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 218;
  template[ind].line_width = 1;
  template[ind].marker_color = 218;
  template[ind++].condition =  LLSF  + Mprob + "evt.AlpgenW*(" + qq + condition + ")"; 
  
  
  // Z+LF
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 210;
  template[ind].line_width = 1;
  template[ind].marker_color = 210;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bb + condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 210;
  template[ind].line_width = 1;
  template[ind].marker_color = 210;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bc + condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 210;
  template[ind].line_width = 1;
  template[ind].marker_color = 210;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bq + condition + ")"; 
  
  // 50
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 210;
  template[ind].line_width = 1;
  template[ind].marker_color = 210;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cc + condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 210;
  template[ind].line_width = 1;
  template[ind].marker_color = 210;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cq + condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 210;
  template[ind].line_width = 1;
  template[ind].marker_color = 210;
  template[ind++].condition = ZHF  + Mprob + LLSF + "evt.AlpgenW*(" + qq + condition + ")"; 
  
  // Z+HF
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bb + condition + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bc + condition + ")"; 
  
  // 55
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bq + condition + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cc + condition + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cq + condition + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Mprob + LLSF + "evt.AlpgenW*(" + qq + condition + ")"; 
  

  // Wc
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bb + condition + ")"; 

  // 60
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bc + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bq + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cc + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cq + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtnoB + qq + condition + ")"; 

  // Wqq 65
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ q#bar{q}"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtLF + bb + condition + ")";

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtLF + bc + condition + ")";

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtLF + bq + condition + ")";

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtLF + cc + condition + ")";

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtLF + cq + condition + ")";

  // 70
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtLF + qq + condition + ")";

  
  // Wbb
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 4;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + bb + condition + ")"; 
   
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 4;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + bc + condition + ")"; 
   
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 4;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 
   
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 4;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cc + condition + ")"; 
   
  // 75
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 4;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cq + condition + ")"; 
   
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 4;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + qq + condition + ")"; 
   
  // Wcc
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bb + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bc + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bq + condition + ")"; 

  // 80
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cc + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cq + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Mprob + "evt.AlpgenW*(" + evtnoB + qq + condition + ")"; 

  // Wlf that belongs in Wbb
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bottom + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + charm + condition + ")"; 

  // 85
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtbottom + LF + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + bottom + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + charm + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtcharm + LF + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bb + condition + ")"; 

  // 90
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bc + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bq + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cc + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cq + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtbottom + qq + condition + ")"; 

  // 95
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + bb + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + bc + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + bq + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + cc + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + cq + condition + ")"; 

  // 100
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtcharm + qq + condition + ")"; 


  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bottom + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + charm + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtbottom + LF + condition + ")"; 


  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bottom + condition + ")"; 

  // 105
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + charm + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtbottom + LF + condition + ")"; 

  // Wcc that belongs in Wbb
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bb + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bc + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bq + condition + ")"; 

  // 110
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cc + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cq + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtbottom + qq + condition + ")"; 

  // Wc that belongs in Wbb
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bb + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bc + condition + ")"; 

  // 115
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bq + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cc + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cq + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Mprob + "evt.AlpgenW*(" + evtbottom + qq + condition + ")"; 


  // These are to fill up all the templates, not really used.
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + HF + condition + ")"; 
  
  // 120
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 206;
  template[ind].line_width = 1;
  template[ind].marker_color = 206;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + HF + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 4;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
   
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF +LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + charm + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + charm + condition + ")"; 

  // 124
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + LF + condition + ")"; 



  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
  // @@ ========  MAGIC ====@@@@@@ //
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
  gROOT->Macro("root_macros/Overlay_tag.C");
  
  
  if ( template[0].h->Integral() != 0){
    float Fqcd = template[1].h->Integral() / template[0].h->Integral();
    cout << "Fqcd: " << Fqcd << endl;
  }
  
  float N_qcd = 0;
  float N_data = 0;
  float N_stop = 0;
  float N_stop_b = 0;
  float N_stop_c = 0;
  float N_stop_q = 0;
  float N_top = 0;
  float N_top_b = 0;
  float N_top_c = 0;
  float N_top_q = 0;
  float N_vv = 0;
  float N_vv_b = 0;
  float N_vv_c = 0;
  float N_vv_q = 0;
  float N_zhf = 0;
  float N_zhf_b = 0;
  float N_zhf_c = 0;
  float N_zhf_q = 0;
  float N_zlf = 0;
  float N_zlf_b = 0;
  float N_zlf_c = 0;
  float N_zlf_q = 0;
  float N_ewk = 0;
  float N_ewk_b = 0;
  float N_ewk_c = 0;
  float N_ewk_q = 0;
  float N_wc = 0;
  float N_wc_b = 0;
  float N_wc_c = 0;
  float N_wc_q = 0;
  float N_wbb = 0;
  float N_wbb_b = 0;
  float N_wbb_c = 0;
  float N_wbb_q = 0;
  float N_wcc = 0;
  float N_wcc_b = 0;
  float N_wcc_c = 0;
  float N_wcc_q = 0;
  float N_wlf = 0;
  float N_wlf_b = 0;
  float N_wlf_c = 0;
  float N_wlf_q = 0;
  
  
  float N_stop_bb = 0;
  float N_stop_bc = 0;
  float N_stop_bq = 0;
  float N_stop_cc = 0;
  float N_stop_cq = 0;
  float N_stop_qq = 0;
  float N_top_bb = 0;
  float N_top_bc = 0;
  float N_top_bq = 0;
  float N_top_cc = 0;
  float N_top_cq = 0;
  float N_top_qq = 0;
  float N_vv_bb = 0;
  float N_vv_bc = 0;
  float N_vv_bq = 0;
  float N_vv_cc = 0;
  float N_vv_cq = 0;
  float N_vv_qq = 0;
  float N_zhf_bb = 0;
  float N_zhf_bc = 0;
  float N_zhf_bq = 0;
  float N_zhf_cc = 0;
  float N_zhf_cq = 0;
  float N_zhf_qq = 0;
  float N_zlf_bb = 0;
  float N_zlf_bc = 0;
  float N_zlf_bq = 0;
  float N_zlf_cc = 0;
  float N_zlf_cq = 0;
  float N_zlf_qq = 0;
  float N_ewk_bb = 0;
  float N_ewk_bc = 0;
  float N_ewk_bq = 0;
  float N_ewk_cc = 0;
  float N_ewk_cq = 0;
  float N_ewk_qq = 0;
  float N_wc_bb = 0;
  float N_wc_bc = 0;
  float N_wc_bq = 0;
  float N_wc_cc = 0;
  float N_wc_cq = 0;
  float N_wc_qq = 0;
  float N_wbb_bb = 0;
  float N_wbb_bc = 0;
  float N_wbb_bq = 0;
  float N_wbb_cc = 0;
  float N_wbb_cq = 0;
  float N_wbb_qq = 0;
  float N_wcc_bb = 0;
  float N_wcc_bc = 0;
  float N_wcc_bq = 0;
  float N_wcc_cc = 0;
  float N_wcc_cq = 0;
  float N_wcc_qq = 0;
  float N_wlf_bb = 0;
  float N_wlf_bc = 0;
  float N_wlf_bq = 0;
  float N_wlf_cc = 0;
  float N_wlf_cq = 0;
  float N_wlf_qq = 0;
  
  float N_mistags = 0;
  for (int i=0; i<Nplots; i++){
    if  (i == 1)      N_qcd +=  template[i].h->Integral();
    else if  (i == 0)     N_data +=  template[i].h->Integral();
    else if  (i == 2)       N_top_b +=  template[i].h->Integral();
    else if  (i == 3)       N_top_c +=  template[i].h->Integral();
    else if  (i == 4)       N_stop_b +=  template[i].h->Integral();
    else if  (i == 5)       N_stop_c +=  template[i].h->Integral();
    else if  (i == 6)       N_vv_b +=  template[i].h->Integral();
    else if  (i == 7)       N_vv_c +=  template[i].h->Integral();
    else if  (i == 8)       N_zhf_b +=  template[i].h->Integral();
    else if  (i == 9)       N_zhf_c +=  template[i].h->Integral();
    else if  (i == 10)       N_zlf_b +=  template[i].h->Integral();
    else if  (i == 11)       N_zlf_c +=  template[i].h->Integral();
    else if  (i == 12)       N_stop_q +=  template[i].h->Integral();
    else if  (i == 13)       N_vv_q +=  template[i].h->Integral();
    else if  (i == 14)       N_zlf_q +=  template[i].h->Integral();
    else if  (i == 15)       N_zhf_q +=  template[i].h->Integral(); 
    else if  (i == 16)       N_top_q +=  template[i].h->Integral();
    else if  (i == 17)       N_wbb_q +=  template[i].h->Integral();
    else if  (i == 18)       N_wcc_q +=  template[i].h->Integral();
    else if  (i == 19)       N_wc_q +=  template[i].h->Integral();
    else if  (i == 20)       N_wlf_q +=  template[i].h->Integral();
    else if  (i == 21)       N_wc_c +=  template[i].h->Integral();
    else if  (i == 22)       N_wlf_c +=  template[i].h->Integral();
    else if  (i == 23)       N_wbb_c +=  template[i].h->Integral();
    else if  (i == 24)       N_wcc_c +=  template[i].h->Integral();
    else if  (i == 25)       N_wlf_b +=  template[i].h->Integral();
    else if  (i == 26)       N_wc_b +=  template[i].h->Integral();
    else if  (i == 27)       N_wcc_b +=  template[i].h->Integral();
    else if  (i == 28)       N_wbb_b +=  template[i].h->Integral();
    
    else if  (i == 29)       N_top_bb +=  template[i].h->Integral();
    else if  (i == 30)       N_top_bc +=  template[i].h->Integral();
    else if  (i == 31)       N_top_bq +=  template[i].h->Integral();
    else if  (i == 32)       N_top_cc +=  template[i].h->Integral();
    else if  (i == 33)       N_top_cq +=  template[i].h->Integral();
    else if  (i == 34)       N_top_qq +=  template[i].h->Integral();
    
    else if  (i == 35)       N_stop_bb +=  template[i].h->Integral();
    else if  (i == 36)       N_stop_bc +=  template[i].h->Integral();
    else if  (i == 37)       N_stop_bq +=  template[i].h->Integral();
    else if  (i == 38)       N_stop_cc +=  template[i].h->Integral();
    else if  (i == 39)       N_stop_cq +=  template[i].h->Integral();
    else if  (i == 40)       N_stop_qq +=  template[i].h->Integral();

    else if  (i == 41)       N_vv_bb +=  template[i].h->Integral();
    else if  (i == 42)       N_vv_bc +=  template[i].h->Integral();
    else if  (i == 43)       N_vv_bq +=  template[i].h->Integral();
    else if  (i == 44)       N_vv_cc +=  template[i].h->Integral();
    else if  (i == 45)       N_vv_cq +=  template[i].h->Integral();
    else if  (i == 46)       N_vv_qq +=  template[i].h->Integral();

    else if  (i == 47)       N_zlf_bb +=  template[i].h->Integral();
    else if  (i == 48)       N_zlf_bc +=  template[i].h->Integral();
    else if  (i == 49)       N_zlf_bq +=  template[i].h->Integral();
    else if  (i == 50)       N_zlf_cc +=  template[i].h->Integral();
    else if  (i == 51)       N_zlf_cq +=  template[i].h->Integral();
    else if  (i == 52)       N_zlf_qq +=  template[i].h->Integral();

    else if  (i == 53)       N_zhf_bb +=  template[i].h->Integral();
    else if  (i == 54)       N_zhf_bc +=  template[i].h->Integral();
    else if  (i == 55)       N_zhf_bq +=  template[i].h->Integral();
    else if  (i == 56)       N_zhf_cc +=  template[i].h->Integral();
    else if  (i == 57)       N_zhf_cq +=  template[i].h->Integral();
    else if  (i == 58)       N_zhf_qq +=  template[i].h->Integral();

    else if  (i == 59)       N_wc_bb +=  template[i].h->Integral();
    else if  (i == 60)       N_wc_bc +=  template[i].h->Integral();
    else if  (i == 61)       N_wc_bq +=  template[i].h->Integral();
    else if  (i == 62)       N_wc_cc +=  template[i].h->Integral();
    else if  (i == 63)       N_wc_cq +=  template[i].h->Integral();
    else if  (i == 64)       N_wc_qq +=  template[i].h->Integral();

    else if  (i == 65)       N_wlf_bb +=  template[i].h->Integral();
    else if  (i == 66)       N_wlf_bc +=  template[i].h->Integral();
    else if  (i == 67)       N_wlf_bq +=  template[i].h->Integral();
    else if  (i == 68)       N_wlf_cc +=  template[i].h->Integral();
    else if  (i == 69)       N_wlf_cq +=  template[i].h->Integral();
    else if  (i == 70)       N_wlf_qq +=  template[i].h->Integral();

    else if  (i == 71)       N_wbb_bb +=  template[i].h->Integral();
    else if  (i == 72)       N_wbb_bc +=  template[i].h->Integral();
    else if  (i == 73)       N_wbb_bq +=  template[i].h->Integral();
    else if  (i == 74)       N_wbb_cc +=  template[i].h->Integral();
    else if  (i == 75)       N_wbb_cq +=  template[i].h->Integral();
    else if  (i == 76)       N_wbb_qq +=  template[i].h->Integral();

    else if  (i == 77)       N_wcc_bb +=  template[i].h->Integral();
    else if  (i == 78)       N_wcc_bc +=  template[i].h->Integral();
    else if  (i == 79)       N_wcc_bq +=  template[i].h->Integral();
    else if  (i == 80)       N_wcc_cc +=  template[i].h->Integral();
    else if  (i == 81)       N_wcc_cq +=  template[i].h->Integral();
    else if  (i == 82)       N_wcc_qq +=  template[i].h->Integral();

    else if  (i == 83)       N_wbb_b +=  template[i].h->Integral();
    else if  (i == 84)       N_wbb_c +=  template[i].h->Integral();
    else if  (i == 85)       N_wbb_q +=  template[i].h->Integral();

    else if  (i == 86)       N_wcc_b +=  template[i].h->Integral();
    else if  (i == 87)       N_wcc_c +=  template[i].h->Integral();
    else if  (i == 88)       N_wcc_q +=  template[i].h->Integral();

    else if  (i == 89)       N_wbb_bb +=  template[i].h->Integral();
    else if  (i == 90)       N_wbb_bc +=  template[i].h->Integral();
    else if  (i == 91)       N_wbb_bq +=  template[i].h->Integral();
    else if  (i == 92)       N_wbb_cc +=  template[i].h->Integral();
    else if  (i == 93)       N_wbb_cq +=  template[i].h->Integral();
    else if  (i == 94)       N_wbb_qq +=  template[i].h->Integral();

    else if  (i == 95)       N_wbb_bb +=  template[i].h->Integral();
    else if  (i == 96)       N_wbb_bc +=  template[i].h->Integral();
    else if  (i == 97)       N_wbb_bq +=  template[i].h->Integral();
    else if  (i == 98)       N_wbb_cc +=  template[i].h->Integral();
    else if  (i == 99)       N_wbb_cq +=  template[i].h->Integral();
    else if  (i == 100)      N_wbb_qq +=  template[i].h->Integral();

    else if  (i == 101)      N_wbb_b +=  template[i].h->Integral();
    else if  (i == 102)      N_wbb_c +=  template[i].h->Integral();
    else if  (i == 103)      N_wbb_q +=  template[i].h->Integral();

    else if  (i == 104)      N_wbb_b +=  template[i].h->Integral();
    else if  (i == 105)      N_wbb_c +=  template[i].h->Integral();
    else if  (i == 106)      N_wbb_q +=  template[i].h->Integral();

    else if  (i == 107)      N_wbb_bb +=  template[i].h->Integral();
    else if  (i == 108)      N_wbb_bc +=  template[i].h->Integral();
    else if  (i == 109)      N_wbb_bq +=  template[i].h->Integral();
    else if  (i == 110)      N_wbb_cc +=  template[i].h->Integral();
    else if  (i == 111)      N_wbb_cq +=  template[i].h->Integral();
    else if  (i == 112)      N_wbb_qq +=  template[i].h->Integral();

    else if  (i == 113)      N_wbb_bb +=  template[i].h->Integral();
    else if  (i == 114)      N_wbb_bc +=  template[i].h->Integral();
    else if  (i == 115)      N_wbb_bq +=  template[i].h->Integral();
    else if  (i == 116)      N_wbb_cc +=  template[i].h->Integral();
    else if  (i == 117)      N_wbb_cq +=  template[i].h->Integral();
    else if  (i == 118)      N_wbb_qq +=  template[i].h->Integral();

  }
  
  N_top = N_top_b + N_top_c + N_top_q;   
  N_stop = N_stop_b + N_stop_c + N_stop_q;   
  N_vv = N_vv_b + N_vv_c + N_vv_q;   
  N_zhf = N_zhf_b + N_zhf_c + N_zhf_q;   
  N_zlf = N_zlf_b + N_zlf_c + N_zlf_q;  
  N_ewk = N_stop+N_vv+N_zhf+N_zlf;
  N_ewk_b = N_stop_b+N_vv_b+N_zhf_b+N_zlf_b;
  N_ewk_c = N_stop_c+N_vv_c+N_zhf_c+N_zlf_c;
  N_ewk_q = N_stop_q+N_vv_q+N_zhf_q+N_zlf_q;
  N_wbb = N_wbb_b + N_wbb_c + N_wbb_q + N_wcc_b + N_wc_b + N_wlf_b;
  N_wcc = N_wcc_c + N_wcc_q + N_wlf_c;
  N_wc = N_wc_c + N_wc_q;
  N_wlf = N_wlf_q;
  
  if (nTags == "2") {
    cout << "Double tags" << endl;
    N_top = N_top_bb + N_top_bc + N_top_bq + N_top_cc + N_top_cq + N_top_qq;
    N_stop = N_stop_bb + N_stop_bc + N_stop_bq + N_stop_cc + N_stop_cq + N_stop_qq;
    N_vv = N_vv_bb + N_vv_bc + N_vv_bq + N_vv_cc + N_vv_cq + N_vv_qq;
    N_zhf = N_zhf_bb + N_zhf_bc + N_zhf_bq + N_zhf_cc + N_zhf_cq + N_zhf_qq;
    N_zlf = N_zlf_bb + N_zlf_bc + N_zlf_bq + N_zlf_cc + N_zlf_cq + N_zlf_qq;
    N_ewk = N_stop+N_vv+N_zhf+N_zlf;
    N_ewk_bb = N_stop_bb + N_vv_bb + N_zhf_bb + N_zlf_bb;
    N_ewk_bc = N_stop_bc + N_vv_bc + N_zhf_bc + N_zlf_bc;
    N_ewk_bq = N_stop_bq + N_vv_bq + N_zhf_bq + N_zlf_bq;
    N_ewk_cc = N_stop_cc + N_vv_cc + N_zhf_cc + N_zlf_cc;
    N_ewk_cq = N_stop_cq + N_vv_cq + N_zhf_cq + N_zlf_cq;
    N_ewk_qq = N_stop_qq + N_vv_qq + N_zhf_qq + N_zlf_qq;
    
    N_wbb = N_wbb_bb + N_wbb_bc + N_wbb_bq + N_wbb_cc + N_wbb_cq + N_wbb_qq 
      + N_wcc_bb + N_wc_bb + N_wlf_bb + N_wcc_bc + N_wc_bc + N_wlf_bc + N_wcc_bq + N_wc_bq + N_wlf_bq;
    N_wcc = N_wcc_cc + N_wcc_cq + N_wcc_qq + N_wlf_cc + N_wlf_cq;
    N_wc = N_wc_cc + N_wc_cq + N_wc_qq;
    N_wlf = N_wlf_qq;
    
  }
  
  
  if (N_qcd != 0)
    cout << " correction to QCD SF = " << (double) N_data*Fqcd / N_qcd << endl;
  
  cout << N_stop_b << "    " << N_vv_b << "   " << N_zlf_b << "   " << N_zhf_b << endl;
  cout << N_stop_c << "    " << N_vv_c << "   " << N_zlf_c << "   " << N_zhf_c << endl;
  cout << N_stop_q << "    " << N_vv_q << "   " << N_zlf_q << "   " << N_zhf_q << endl;
  cout << endl << endl;
  
  if (nTags == "2") {
    cout << N_stop_bb << "    " << N_vv_bb << "   " << N_zlf_bb << "   " << N_zhf_bb << endl;
    cout << N_stop_bc << "    " << N_vv_bc << "   " << N_zlf_bc << "   " << N_zhf_bc << endl;
    cout << N_stop_bq << "    " << N_vv_bq << "   " << N_zlf_bq << "   " << N_zhf_bq << endl;
    cout << N_stop_cc << "    " << N_vv_cc << "   " << N_zlf_cc << "   " << N_zhf_cc << endl;
    cout << N_stop_cq << "    " << N_vv_cq << "   " << N_zlf_cq << "   " << N_zhf_cq << endl;
    cout << N_stop_qq << "    " << N_vv_qq << "   " << N_zlf_qq << "   " << N_zhf_qq << endl;
  }
  
  
  cout << " Data       QCD        EWK       Wqq        Wc       Wcc       Wbb       Top "	<< endl;
  cout << N_data << "    ";
  cout << N_qcd <<  "    ";	   
  cout << N_ewk << "    ";
  cout << N_wlf <<  "    ";
  cout << N_wc << "    ";
  cout << N_wcc <<  "    ";
  cout << N_wbb << "    ";
  cout << N_top <<  endl;
  
  cout << "        ";
  cout << "        ";	   
  cout << N_ewk_b << "    ";
  cout << N_wlf_b <<  "    ";
  cout << N_wc_b << "    ";
  cout << N_wcc_b <<  "    ";
  cout << N_wbb_b << "    ";
  cout << N_top_b <<  endl;
  
  
  cout << "        ";
  cout << "        ";	   
  cout << N_ewk_c << "    ";
  cout << N_wlf_c <<  "    ";
  cout << N_wc_c << "    ";
  cout << N_wcc_c <<  "    ";
  cout << N_wbb_c << "    ";
  cout << N_top_c <<  endl;
  
  
  cout << "        ";
  cout << "        ";	   
  cout << N_ewk_q << "    ";
  cout << N_wlf_q <<  "    ";
  cout << N_wc_q << "    ";
  cout << N_wcc_q <<  "    ";
  cout << N_wbb_q << "    ";
  cout << N_top_q <<  endl;
  
  if (nTags == "2") {
    cout << endl;
    cout << "        ";
    cout << "        ";	   
    cout << N_ewk_bb << "    ";
    cout << N_wlf_bb <<  "    ";
    cout << N_wc_bb << "    ";
    cout << N_wcc_bb <<  "    ";
    cout << N_wbb_bb << "    ";
    cout << N_top_bb <<  endl;
    
    cout << "        ";
    cout << "        ";	   
    cout << N_ewk_bc << "    ";
    cout << N_wlf_bc <<  "    ";
    cout << N_wc_bc << "    ";
    cout << N_wcc_bc <<  "    ";
    cout << N_wbb_bc << "    ";
    cout << N_top_bc <<  endl;
    
    cout << "        ";
    cout << "        ";	   
    cout << N_ewk_bq << "    ";
    cout << N_wlf_bq <<  "    ";
    cout << N_wc_bq << "    ";
    cout << N_wcc_bq <<  "    ";
    cout << N_wbb_bq << "    ";
    cout << N_top_bq <<  endl;
    
    cout << "        ";
    cout << "        ";	   
    cout << N_ewk_cc << "    ";
    cout << N_wlf_cc <<  "    ";
    cout << N_wc_cc << "    ";
    cout << N_wcc_cc <<  "    ";
    cout << N_wbb_cc << "    ";
    cout << N_top_cc <<  endl;
    
    cout << "        ";
    cout << "        ";	   
    cout << N_ewk_cq << "    ";
    cout << N_wlf_cq <<  "    ";
    cout << N_wc_cq << "    ";
    cout << N_wcc_cq <<  "    ";
    cout << N_wbb_cq << "    ";
    cout << N_top_cq <<  endl;
    
    cout << "        ";
    cout << "        ";	   
    cout << N_ewk_qq << "    ";
    cout << N_wlf_qq <<  "    ";
    cout << N_wc_qq << "    ";
    cout << N_wcc_qq <<  "    ";
    cout << N_wbb_qq << "    ";
    cout << N_top_qq <<  endl;
    
  }



 //  cout << "qcd & " << N_qcd << endl;
 //  cout << "stop(hf) & " << N_stop << endl;
 //  cout << "top(hf)  & " << N_top << endl;
 //  cout << "vv(hf) & " << N_vv << endl;
//   cout << "zhf (hf)& " << N_zhf << endl;
//   cout << "zlf(hf) & " << N_zlf << endl;
//   cout << "wc & " << N_wc << endl;
//   cout << "wbb & " << N_wbb << endl;
//   cout << "wcc & " << N_wcc << endl;
//   cout << "mistags & " << N_mistags << endl;
//   cout << " ---------------------------------- " << endl;
//   cout << "Total & " << N_qcd+N_stop+N_top+N_vv+N_zhf+N_zlf+N_wc+N_wbb+N_wcc+N_mistags << endl;
//   cout << "data & " << N_data << endl;


  cout << "Preparing for the fit" << endl;
  const int Nfit = 62; // # of fitted histograms
  double param[Nfit];
  
  TH1F *hdata = (TH1F*)(template[0].h)->Clone("dat");
  cout << "Cloned the data template" << endl;
  TH1F *hmc[Nfit];
  TString fit_name[Nfit] = {"Data", "QCD",                                                             // 1
			    "EW_b", "EW_c", "EW_q", "Wqq_b", "Wqq_c", "Wqq_q", "Wc_b", "Wc_c", "Wc_q", // 10
			    "Wcc_b", "Wcc_c", "Wcc_q", "Wbb_b", "Wbb_c", "Wbb_q",                      // 16
			    "top_b", "top_c", "top_q",                                                 // 19
			    "EW_bb", "EW_bc", "EW_bq", "EW_cc", "EW_cq", "EW_qq",                      // 25
			    "Wqq_bb", "Wqq_bc", "Wqq_bq", "Wqq_cc", "Wqq_cq", "Wqq_qq",                // 31
			    "Wc_bb", "Wc_bc", "Wc_bq", "Wc_cc", "Wc_cq", "Wc_qq",                      // 37
			    "Wcc_bb", "Wcc_bc", "Wcc_bq", "Wcc_cc", "Wcc_cq", "Wcc_qq",                // 43
			    "Wbb_bb", "Wbb_bc", "Wbb_bq", "Wbb_cc", "Wbb_cq", "Wbb_qq",                // 49
			    "top_bb", "top_bc", "top_bq", "top_cc", "top_cq", "top_qq",                // 55
			    "top", "EW", "Wbb", "Wcc", "Wc", "Wqq"};                                   // 61
  
  
  
  int fit_assoc[Nplots];
  cout << "Initializing fit_init" << endl;
  
  bool fit_init[Nfit] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0};
  
  cout << "Initializing fit_assoc" << endl;
  for (int i = 0; i < Nplots; i++) { //intialize fit_assoc
    if (0 == i)
      fit_assoc[i] = 0;
    else if (1 == i)
      fit_assoc[i] = 1;
    else if (2 == i)
      fit_assoc[i] = 17;
    else if (3 == i)
      fit_assoc[i] = 18;
    else if (4 == i)
      fit_assoc[i] = 2;
    else if (5 == i)
      fit_assoc[i] = 3;
    else if (6 == i)
      fit_assoc[i] = 2;
    else if (7 == i)
      fit_assoc[i] = 3;
    else if (8 == i)
      fit_assoc[i] = 2;
    else if (9 == i)
      fit_assoc[i] = 3;
    else if (10 == i)
      fit_assoc[i] = 2;
    else if (11 == i)
      fit_assoc[i] = 3;
    else if (12 == i)
      fit_assoc[i] = 4;
    else if (13 == i)
      fit_assoc[i] = 4;
    else if (14 == i)
      fit_assoc[i] = 4;
    else if (15 == i)
      fit_assoc[i] = 4;
    else if (16 == i)
      fit_assoc[i] = 19;
    else if (17 == i)
      fit_assoc[i] = 16;
    else if (18 == i)
      fit_assoc[i] = 13;
    else if (19 == i)
      fit_assoc[i] = 10;
    else if (20 == i)
      fit_assoc[i] = 7;
    else if (21 == i)
      fit_assoc[i] = 9;
    else if (22 == i)
      fit_assoc[i] = 6;
    else if (23 == i)
      fit_assoc[i] = 15;
    else if (24 == i)
      fit_assoc[i] = 12;
    else if (25 == i)
      fit_assoc[i] = 5;
    else if (26 == i)
      fit_assoc[i] = 8;
    else if (27 == i)
      fit_assoc[i] = 11;
    else if (28 == i)
      fit_assoc[i] = 14;


    // Begin split templates, double tags
    else if (i >= 29 && i <= 34)
      fit_assoc[i] = i + 21;
    else if (i == 35)
      fit_assoc[i] = 20;
    else if (i == 36)
      fit_assoc[i] = 21;
    else if (i == 37)
      fit_assoc[i] = 22;
    else if (i == 38)
      fit_assoc[i] = 23;
    else if (i == 39)
      fit_assoc[i] = 24;
    else if (i == 40)
      fit_assoc[i] = 25;
    else if (i == 41)
      fit_assoc[i] = 20;
    else if (i == 42)
      fit_assoc[i] = 21;
    else if (i == 43)
      fit_assoc[i] = 22;
    else if (i == 44)
      fit_assoc[i] = 23;
    else if (i == 45)
      fit_assoc[i] = 24;
    else if (i == 46)
      fit_assoc[i] = 25;
    else if (i == 47)
      fit_assoc[i] = 20;
    else if (i == 48)
      fit_assoc[i] = 21;
    else if (i == 49)
      fit_assoc[i] = 22;
    else if (i == 50)
      fit_assoc[i] = 23;
    else if (i == 51)
      fit_assoc[i] = 24;
    else if (i == 52)
      fit_assoc[i] = 25;
    else if (i == 53)
      fit_assoc[i] = 20;
    else if (i == 54)
      fit_assoc[i] = 21;
    else if (i == 55)
      fit_assoc[i] = 22;
    else if (i == 56)
      fit_assoc[i] = 23;
    else if (i == 57)
      fit_assoc[i] = 24;
    else if (i == 58)
      fit_assoc[i] = 25;
    else if (i >= 59 && i <= 64)
      fit_assoc[i] = i - 27;
    else if (i >= 65 && i <= 70)
      fit_assoc[i] = i - 39;
    else if (i >= 71 && i <= 76)
      fit_assoc[i] = i - 27;
    else if (i >= 77 && i <= 82)
      fit_assoc[i] = i - 39;
    else if (i >= 70 && i <= 73)
      fit_assoc[i] = 31;

    else if (i == 83)
      fit_assoc[i] = 14;
    else if (i == 84)
      fit_assoc[i] = 15;
    else if (i == 85)
      fit_assoc[i] = 16;
    
    else if (i == 86)
      fit_assoc[i] = 11;
    else if (i == 87)
      fit_assoc[i] = 12;
    else if (i == 88)
      fit_assoc[i] = 13;
    
    else if (i == 89)
      fit_assoc[i] = 44;
    else if (i == 90)
      fit_assoc[i] = 45;
    else if (i == 91)
      fit_assoc[i] = 46;
    else if (i == 92)
      fit_assoc[i] = 47;
    else if (i == 93)
      fit_assoc[i] = 48;
    else if (i == 94)
      fit_assoc[i] = 49;

    else if (i == 95)
      fit_assoc[i] = 38;
    else if (i == 96)
      fit_assoc[i] = 39;
    else if (i == 97)
      fit_assoc[i] = 40;
    else if (i == 98)
      fit_assoc[i] = 41;
    else if (i == 99)
      fit_assoc[i] = 42;
    else if (i == 100)
      fit_assoc[i] = 43;

    else if (i == 101)
      fit_assoc[i] = 14;
    else if (i == 102)
      fit_assoc[i] = 15;
    else if (i == 103)
      fit_assoc[i] = 16;

    else if (i == 104)
      fit_assoc[i] = 14;
    else if (i == 105)
      fit_assoc[i] = 15;
    else if (i == 106)
      fit_assoc[i] = 16;

    else if (i == 107)
      fit_assoc[i] = 44;
    else if (i == 108)
      fit_assoc[i] = 45;
    else if (i == 109)
      fit_assoc[i] = 46;
    else if (i == 110)
      fit_assoc[i] = 47;
    else if (i == 111)
      fit_assoc[i] = 48;
    else if (i == 112)
      fit_assoc[i] = 49;

    else if (i == 113)
      fit_assoc[i] = 44;
    else if (i == 114)
      fit_assoc[i] = 45;
    else if (i == 115)
      fit_assoc[i] = 46;
    else if (i == 116)
      fit_assoc[i] = 47;
    else if (i == 117)
      fit_assoc[i] = 48;
    else if (i == 118)
      fit_assoc[i] = 49;

    else if (i >= 119 && i <= 124)
      fit_assoc[i] = i - 63;

  } // end assoc loop
  
  //cout << "Done initializing fit_assoc" << endl;
  //cout << "Nplots = " << Nplots << endl;
  for (int i = 1; i < Nplots; i++){//intialize histograms used in the fit   
    //cout << "i = " << i << "; fit_assoc = " << fit_assoc[i] << endl ; //"; pointer: " << hmc[ fit_assoc[i] ] << endl;
    if (! fit_init [ fit_assoc[i] ]) {
      //cout << "Cloning" << endl;
      hmc[ fit_assoc[i] ] = (TH1F*)(template[i].h)->Clone(fit_name[ fit_assoc[i] ]);
      fit_init [ fit_assoc[i] ] = 1;
    }
    else                                 
      hmc[ fit_assoc[i] ]->Add(template[i].h);
  }
  
  //cout << "About to write the histos" << endl;
  TFile ffit("fit_inputs/" + var_desc + systDesc + "_rate_" + jetBin + "j_" + nTags + "tag.root","recreate");
  hdata->Write();
  //cout << "Wrote data" << endl;
  for (int i = 1; i < Nfit; i++) {
    hmc[i]->Write();
    //cout << "Wrote histo " << i << endl;
  }
  ffit->Close();
  //cout << "Closed the file" << endl;  
  

  c1->SaveAs(plot_name + ".gif");
  c1->SaveAs(plot_name + ".eps");
  gROOT->ProcessLine(".! epstopdf " + plot_name + ".eps");
  
  
  c1->SetLogy();
  c1->SaveAs(plot_name + "_log.gif");
  c1->SaveAs(plot_name + "_log.eps");
  gROOT->ProcessLine(".! epstopdf " + plot_name + "_log.eps");
  
  gApplication->Terminate();
}
