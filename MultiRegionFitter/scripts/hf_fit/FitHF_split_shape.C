{

  //*****************
  // Purpose: Create fit inputs for kfactor Fit: kfactor/Fit_Kfactor.C 
    // all jets matched to LF are called mistags
    // therefore requires a proper qcd and LF scale factors
    // obtained from scripts: qcdSF_4HFfit X Fit_wConstr 
    // and LSF_4HFfit
  //******************
  gROOT->LoadMacro("root_macros/TDistribution.cc");

  //***********
  // Inputs
  //***********

      bool plot_only = 0; 
  bool make_legend = true;

  TString jetBin = "2"; //1, 2, 3, 4, 5
  TString nTags = "2";// 1,2,12
  TString systDesc = "";       // for shifted JES, b-tag, etc. Make sure it begins with _ if not null
                               // and that the corresponding files have this tag as well

  //TAGGERS
   TString tagger = "";   TString tagger_ind = "T"; TString tagger_desc = "Tight";
  // TString tagger = "L";   TString tagger_ind = "L";  TString tagger_desc = "Loose";
  //   TString tagger = "U";   TString tagger_ind = "U";  TString tagger_desc = "UltraTight";
  TString MetCut = "25";// 20-60
  bool singleTopQcdVeto = 1;  //0,1
  int NBINS =20;  

  TString ytitle = "# Events";

  // VARIABLES
  
  // 1 tag KIT
  TString var = "bjet" + tagger + "1.KaNNcorr";  TString xtitle =  tagger_desc + "KIT Flavor Separator";   float minHist = -1.;  float maxHist = 1.; TString var_desc = "KIT" ; 
    
  // for 2 tags, use the average
  if (nTags == 2) {
    var = "(bjet" + tagger + "1.KaNNcorr + bjet" + tagger + "2.KaNNcorr) / 2";
  }
   

  // SecVtx Mass - only Loose tags
  //   TString var = "min(4.39, bjet" + tagger + "1.Mvtx)";  TString xtitle = tagger_desc + "SECVTX Mass";  float minHist = 0.;   float maxHist = 4.4; TString var_desc = "LooseSecvtxMass" ; NBINS = 22;

     
  // 1 tag KIT, uncorrected
  /*TString var = "bjet" + tagger + "1.KaNN";  TString xtitle =  tagger_desc + "KIT Flavor Separator";   float minHist = -1.;  float maxHist = 1.; TString var_desc = "KIT_uncorr" ; 

  // for 2 tags, use the average
  if (nTags == 2) {
    var = "(bjet" + tagger + "1.KaNN + bjet" + tagger + "2.KaNN) / 2";
  }
  */


  TString plot_name = "hf_fit/results/" + var_desc + "_split" + systDesc + "_" + jetBin +"j_" + nTags + "tag" + tagger + "_met" + MetCut + "_nbins" + NBINS;
  if (singleTopQcdVeto) plot_name += "_veto";

  //***** End: Inputs

  // overlay Nplots from different root files 
      const int Nplots = 140;

  TString LLSF = "2.676*lep.Eff*"; 
  // use this for LSF anyway
  TString WSF = "1.35*";
  TString ZLF = "1.4*";
  TString ZHF = "2.0*";

  TString qcdSF = "";  // (qcdSF_4HFfit X Fit_wConstr )
  TString LSF = ""; // LSF_4HFfit

  if (jetBin == "1" && tagger_ind == "T") {
    qcdSF = "4.4386*0.0707343*";
    LSF = "1.38263*";
  } else if (jetBin == "1" && tagger_ind == "L") {
    qcdSF = "0.145716*0.107003*";
    LSF = "1.22921*";
  } else if (jetBin == "1" && tagger_ind == "U") {
    qcdSF = "0.779061*0.0243146*";
    LSF = "1.06019*";
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "4.6129*0.285361*"; 
    LSF = "1.33045*"; 
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "0.20028*0.218136*"; 
    LSF = "1.16276*"; 
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "0.986207*0.0896162*"; 
    LSF = "0.945689*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "286*0.00562856*"; 
    LSF = "3.7247*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "4.68852*0.00851716*"; 
    LSF = "1.57309*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "35.75*0.00184308*"; 
    LSF = "2.8243*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "4.52941*0.287186*"; 
    LSF = "1.28678*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "0.28*0.614599*"; 
    LSF = "1.18005*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "1.24194*0.0784804*"; 
    LSF = "1.05963*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "0*1.21568e-07*"; 
    LSF = "1.58079*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "5.1333*4.46547e-09*"; 
    LSF = "1.60104*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "38.5*1.89621e-08*"; 
    LSF = "0.439978*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "1.75758*0.14526*"; 
    LSF = "1.3478*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "1.16*0.350817*"; 
    LSF = "1.23416*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "2.9*9.44046e-09*"; 
    LSF = "1.10999*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "19.3333*6.81147e-10*"; 
    LSF = "1.98572*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "11.6*3.61481e-08*"; 
    LSF = "1.49252*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "58*2.42987e-10*"; 
    LSF = "0.921002*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "1.85714*0.295103*"; 
    LSF = "1.29417*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "2*0.212022*"; 
    LSF = "1.15329*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "12*1.31753e-09*"; 
    LSF = "0.971391*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "0*2.01568e-10*"; 
    LSF = "2.35918*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "0*1.70928e-10*"; 
    LSF = "1.47517*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "0*1.08137e-11*"; 
    LSF = "5.04432*"; 
  }

  TString Bprob = "1*";
  if (nTags != "pre")
    Bprob = "evt.bProb" +  nTags + tagger_ind + "*"; // change the last part to mn* for -1 sigma, pl*
     
                                                  // for +1 sigma, or * for no shift
  LSF = "1*";

  bool stack = 1; //false; // stack the MC ( do not forget that "0" is data)

  bool norm = 0; //1; //false; // switch to normalize the histograms to the same area
  bool normToData = 0; //1; //false; // normalize to "0" histogram

  //  if (plot_only){ norm = 1; normToData = 1; }
  bool doKS = false; // calculate KS probabilities

  bool setMinMax = true;

  TString tree_name = "nt";

  //////////////////////////////  EVENT SELECTION ///////////////////////////////////

  TString condition = "evt.isGoodSiRun "; //&& ANN.3j < 0.6 "; 
  //  if (jetBin == "3") condition += " && ANN.3j < 0.5 ";
  condition += (" && " + var + " > -999");

    if (singleTopQcdVeto) condition += " && evt.passedQCDveto  ";

    // jetBin
    if ("5" == jetBin) 
      condition = "  evt.nJets >= 5 && " + condition;
    else 
      condition = "  evt.nJets ==  " + jetBin + " && " + condition;

    TString condition_qcd = condition;

    // MetCut 
    condition = " evt.Met > " + MetCut + " && " + condition; // don't want QCD to have MET cut


    TString condition_data = condition;
    condition_data += " && evt.n" + tagger_ind + "posTags == " + nTags; 
    condition_qcd += " && evt.n" + tagger_ind + "posTags == " + nTags; 

 cout << "Plotting " << var << endl;
 cout << "Event Selection = " << condition_data << endl;

 TString bottom = "evt." + tagger_ind + "matchB && "; 
 TString charm = "evt." + tagger_ind + "matchC && "; 
 TString evtbottom = "evt.matchBottom && ";
 TString evtcharm = "( evt.matchCharm && !evt.matchBottom) && ";
 TString evtLF = "( !evt.matchBottom && !evt.matchCharm ) && ";
 TString evtnoB = "(!evt.matchBottom) && ";
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

 TString HF = " ( evt." + tagger_ind + "matchB || evt." + tagger_ind + "matchC ) && " ;
 TString LF = "  ( !evt." + tagger_ind + "matchB && !evt." + tagger_ind + "matchC ) && " ;

  //////////////////////////////////  SAMPLES TO USE /////////////////////////////////

  TDistribution template[Nplots];  
  for (int ind = 0; ind < Nplots; ind++) {
    template[ind].var = var;
    template[ind].condition = condition;
  }

  cout << "Starting the templates" << endl;

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
  template[ind].marker_color = 33;
  template[ind++].condition = qcdSF + "(" + condition_qcd + ")"; 
  
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW";   
  template[ind].line_color = 89; // = 102;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + HF + condition_data + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 105;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + HF + condition_data + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 103;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + HF + condition_data + ")"; 
  
  // ind = 5
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 209;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + HF + condition_data + ")"; 
  
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 89;
  template[ind].line_width = 1;
  template[ind].fill_color = 89;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 89;
  template[ind].line_width = 1;
  template[ind].fill_color = 89;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 89;
  template[ind].line_width = 1;
  template[ind].fill_color = 89;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF + LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 89;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF + LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 
  // EW

  // Wqq ind = 10
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W + q#bar{q}"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  // Wc
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + LLSF + Bprob + "evt.AlpgenW*(" + charm + condition_data + ")"; 
  
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 
  // Wc

  // Wcc
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 62;
  template[ind].line_width = 1;
  template[ind].fill_color = 62;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + charm + condition_data + ")";

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  // ind = 15
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 62;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition_data + ")"; 
  // Wcc

  // Wbb
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ b#bar{b}"; 
  template[ind].line_color = 56;
  template[ind].line_width = 1;
  template[ind].fill_color = 56;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition_data + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 56;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition_data + ")"; 
  
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 56;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition_data + ")"; 
  
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  // ind = 20
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 51;
  template[ind++].condition =  WSF + LLSF + Bprob + "evt.AlpgenW*(" + HF + condition_data + ")"; 
  // Wbb

  // Top
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 2; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + HF + condition_data + ")"; 

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 2;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 


  // this is where all the templates get split. First for 1 tag, then for 2.

  // EW_b 23
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_b";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition_data + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition_data + ")"; 
 
  // ind = 25
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" +  bjet1 + condition_data + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + bjet1 + condition_data + ")"; 
  // end EW_b

  // EW_c
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_c";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition_data + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition_data + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + cjet1 + condition_data + ")"; 
  
  // ind = 30
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + cjet1 + condition_data + ")"; 
  // end EW_c

  // EW_q
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_q";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + qjet1 + condition_data + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + qjet1 + condition_data + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + LLSF + "evt.AlpgenW*(" + qjet1 + condition_data + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + LLSF + "evt.AlpgenW*(" + qjet1 + condition_data + ")"; 
  // end EW_q

  // Wqq_b ind = 35
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_b"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + evtLF + bjet1 + condition_data + ")";

  // Wqq_c
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_c"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + evtLF + cjet1 + condition_data + ")";

  // Wqq_q
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_q"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + evtLF + qjet1 + condition_data + ")";

  // Wc_b
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_b";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bjet1 + condition_data + ")"; 

  // Wc_c
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cjet1 + condition_data + ")"; 

  // Wc_q ind = 40
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_q";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + LSF + Bprob + "evt.AlpgenW*(" + evtnoB + qjet1 + condition_data + ")"; 

  // Wcc_b
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_b"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bjet1 + condition_data + ")"; 

  // Wcc_c
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_c"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cjet1 + condition_data + ")"; 

  // Wcc_q
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_q"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + qjet1 + condition_data + ")"; 

  // Wbb_b
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_b"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition_data + ")"; 

  // Wbb_c ind = 45
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_c"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition_data + ")"; 

  // Wbb_q 
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_q"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + qjet1 + condition_data + ")"; 

  // Top_b
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_b";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition_data + ")"; 

  // Top_c
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_c";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition_data + ")"; 

  // Top_q ind = 49
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_q";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + qjet1 + condition_data + ")"; 


  // Now do the double tags
  // EW_bb ind = 50
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_bb";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bb + condition_data + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bb + condition_data + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + bb + condition_data + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + bb + condition_data + ")"; 
  // end EW_bb
  
  // EW_bc 
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_bc";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bc + condition_data + ")"; 
  
  // ind = 55
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bc + condition_data + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + bc + condition_data + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + bc + condition_data + ")"; 
  // end EW_bc

  // EW_bq
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_bq";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bq + condition_data + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bq + condition_data + ")"; 
 
  // ind = 60
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + bq + condition_data + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + bq + condition_data + ")"; 
  // end EW_bq

  // EW_cc
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_cc";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cc + condition_data + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cc + condition_data + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + cc + condition_data + ")"; 

  // ind = 65
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + cc + condition_data + ")"; 
  // end EW_cc
  
  // EW_cq
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_cq";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cq + condition_data + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cq + condition_data + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + cq + condition_data + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + Bprob + LLSF + "evt.AlpgenW*(" + cq + condition_data + ")"; 
  // end EW_cq
  
  // EW_qq ind = 70
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW_q";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + LSF + Bprob + "evt.AlpgenW*(" + qq + condition_data + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition = LLSF + LSF + Bprob + "evt.AlpgenW*(" + qq + condition_data + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + LSF + LLSF + "evt.AlpgenW*(" + qq + condition_data + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + LSF + LLSF + "evt.AlpgenW*(" + qq + condition_data + ")"; 
  // end EW_qq

  // Wqq_bb
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_bb"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + LSF + "evt.AlpgenW*(" + evtLF + bb + condition_data + ")"; 

  // Wqq_bc ind = 75
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_bc"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + LSF + "evt.AlpgenW*(" + evtLF + bc + condition_data + ")"; 

  // Wqq_bq
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_bq"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + LSF + "evt.AlpgenW*(" + evtLF + bq + condition_data + ")"; 

  // Wqq_cc
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_cc"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + LSF + "evt.AlpgenW*(" + evtLF + cc + condition_data + ")"; 

  // Wqq_cq
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_cq"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + LSF + "evt.AlpgenW*(" + evtLF + cq + condition_data + ")"; 

  // Wqq_qq
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_qq"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + LSF + "evt.AlpgenW*(" + evtLF + qq + condition_data + ")"; 

  // Wc_bb ind = 80
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_bb";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bb + condition_data + ")"; 

  // Wc_bc
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_bc";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bc + condition_data + ")"; 

  // Wc_bq
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_bq";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bq + condition_data + ")"; 

  // Wc_cc
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_cc";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cc + condition_data + ")"; 

  // Wc_cq
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_cq";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cq + condition_data + ")"; 

  // Wc_qq ind = 85
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_qq";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + qq + condition_data + ")"; 
  // Wc_qq

  // Wcc_bb
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_bb"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bb + condition_data + ")"; 

  // Wcc_bc 
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_bc"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bc + condition_data + ")"; 


  // Wcc_bq
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_bq"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + bq + condition_data + ")"; 

  // Wcc_cc
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_cc"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cc + condition_data + ")"; 

  // Wcc_cq ind = 90
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_cq"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + cq + condition_data + ")"; 

  // Wcc_qq
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_qq"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtnoB + qq + condition_data + ")"; 
  // Wcc_qq

  // Wbb_bb 
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_bb"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bb + condition_data + ")"; 

  // Wbb_bc
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_bc"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bc + condition_data + ")"; 

  // Wbb_bq
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_bq"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bq + condition_data + ")"; 

  // Wbb_cc ind = 95
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_cc"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cc + condition_data + ")"; 

  // Wbb_cq
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_cq"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cq + condition_data + ")"; 

  // Wbb_qq 
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_qq"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + qq + condition_data + ")"; 

  // Top_bb
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_bb";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bb + condition_data + ")"; 

  // Top_bc
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_bc";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bc + condition_data + ")"; 

  // Top_bq ind = 100
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_bq";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bq + condition_data + ")"; 

  // Top_cc
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_cc";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cc + condition_data + ")"; 

  // Top_cq
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_cq";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cq + condition_data + ")"; 

  // Top_qq ind = 103
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_qq";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + qq + condition_data + ")"; 


  // Wlf that belongs in Wbb or Wcc
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bottom + condition_data + ")"; 

  // 105
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + charm + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + LF + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + bottom + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + charm + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + LF + condition_data + ")"; 

  // 110
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bb + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bc + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bq + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cc + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cq + condition_data + ")"; 

  // 115
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + qq + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + bb + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + bc + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + bq + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + cc + condition_data + ")"; 

  // 120
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + cq + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtcharm + qq + condition_data + ")"; 

  // Wcc and Wc that belongs in Wbb
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bottom + condition_data + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + charm + condition_data + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + LF + condition_data + ")"; 

  // 125
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bottom + condition_data + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + charm + condition_data + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + LF + condition_data + ")"; 

  // Wcc that belongs in Wbb
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bb + condition_data + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bc + condition_data + ")"; 

  // 130
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bq + condition_data + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cc + condition_data + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cq + condition_data + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + qq + condition_data + ")"; 

  // Wc that belongs in Wbb
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bb + condition_data + ")"; 

  // 135
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bc + condition_data + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + bq + condition_data + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cc + condition_data + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + cq + condition_data + ")"; 

  // 139
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 4;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + evtbottom + qq + condition_data + ")"; 







  if (ind  < Nplots ){
    cout << "ERROR: number of templates = " << ind << " is less than " << Nplots << endl;
    gApplication->Terminate();
  }
  
   // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
   // @@ ======  MAGIC ======@@@@@@ //
   // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
   gROOT->Macro("root_macros/Overlay.C");

   cout << "Overlay done" << endl;
   if (!plot_only){
     cout.precision(3);

     // ****************************************
     // **** PREPARE FOR FIT  ***
     // ***************************************

     cout << "Preparing for the fit" << endl;
     const int Nfit = 62; // # of fitted histograms
     double param[Nfit];

     TH1F *hdata = (TH1F*)(template[0].h)->Clone("dat");
     cout << "Cloned the data template" << endl;
     TH1F *hmc[Nfit];
     TString fit_name[Nfit] = {"Data", "top", "Wbb", "Wcc", "Wc", "Wqq", "EW", "QCD",                     // 7
			       "EW_b", "EW_c", "EW_q", "Wqq_b", "Wqq_c", "Wqq_q", "Wc_b", "Wc_c", "Wc_q", // 16
			       "Wcc_b", "Wcc_c", "Wcc_q", "Wbb_b", "Wbb_c", "Wbb_q",                      // 22
			       "top_b", "top_c", "top_q",                                                 // 25
			       "EW_bb", "EW_bc", "EW_bq", "EW_cc", "EW_cq", "EW_qq",                      // 31
			       "Wqq_bb", "Wqq_bc", "Wqq_bq", "Wqq_cc", "Wqq_cq", "Wqq_qq",                // 37
			       "Wc_bb", "Wc_bc", "Wc_bq", "Wc_cc", "Wc_cq", "Wc_qq",                      // 43
			       "Wcc_bb", "Wcc_bc", "Wcc_bq", "Wcc_cc", "Wcc_cq", "Wcc_qq",                // 49
			       "Wbb_bb", "Wbb_bc", "Wbb_bq", "Wbb_cc", "Wbb_cq", "Wbb_qq",                // 55
			       "top_bb", "top_bc", "top_bq", "top_cc", "top_cq", "top_qq"};               // 61
								   


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
	 fit_assoc[i] = 7;
      else if (i <= 9)
	fit_assoc[i] = 6;
      else if (10 == i)
	fit_assoc[i] = 5;
      else if (i <= 12)
	fit_assoc[i] = 4;
      else if (i >= 13 && i <= 15)
	fit_assoc[i] = 3;
      else if (i >= 16 && i <= 20)
	fit_assoc[i] = 2;
      else if (i >= 21 && i <= 22)
	fit_assoc[i] = 1;

       // Begin split tempaltes, single tag
       else if (i >= 23 && i <= 26)
	 fit_assoc[i] = 8;
       else if (i >= 27 && i <= 30) 
	 fit_assoc[i] = 9;
       else if (i >= 31 && i <= 34)
	 fit_assoc[i] = 10; // EW_q
       else if (i == 35)
	 fit_assoc[i] = 11; // i - 24
       else if (i == 36)
	 fit_assoc[i] = 12;
       else if (i == 37)
	 fit_assoc[i] = 13;
       else if (i == 38)
	 fit_assoc[i] = 14;
       else if (i == 39)
	 fit_assoc[i] = 15;
       else if (i == 40)
	 fit_assoc[i] = 16;
       else if (i == 41)
	 fit_assoc[i] = 17;
       else if (i == 42)
	 fit_assoc[i] = 18;
       else if (i == 43)
	 fit_assoc[i] = 19;
       else if (i == 44)
	 fit_assoc[i] = 20;
       else if (i == 45)
	 fit_assoc[i] = 21;
       else if (i == 46)
	 fit_assoc[i] = 22;
       else if (i == 47)
	 fit_assoc[i] = 23;
       else if (i == 48)
	 fit_assoc[i] = 24;
       else if (i == 49)
	 fit_assoc[i] = 25;

       // Begin split templates, double tags
       else if (i >= 50 && i <= 53)
	 fit_assoc[i] = 26;
       else if (i >= 54 && i <= 57)
	 fit_assoc[i] = 27;
       else if (i >= 58 && i <= 61)
	 fit_assoc[i] = 28;
       else if (i >= 62 && i <= 65)
	 fit_assoc[i] = 29;
       else if (i >= 66 && i <= 69)
	 fit_assoc[i] = 30;
       else if (i >= 70 && i <= 73)
	 fit_assoc[i] = 31; // EW_qq
       else if (i == 74)
	 fit_assoc[i] = 32; // i - 42
       else if (i == 75)
	 fit_assoc[i] = 33;
       else if (i == 76)
	 fit_assoc[i] = 34;
       else if (i == 77)
	 fit_assoc[i] = 35;
       else if (i == 78)
	 fit_assoc[i] = 36;
       else if (i == 79)
	 fit_assoc[i] = 37;
       else if (i == 80)
	 fit_assoc[i] = 38;
       else if (i == 81)
	 fit_assoc[i] = 39;
       else if (i == 82)
	 fit_assoc[i] = 40;
       else if (i == 83)
	 fit_assoc[i] = 41;
       else if (i == 84)
	 fit_assoc[i] = 42;
       else if (i == 85)
	 fit_assoc[i] = 43;
       else if (i == 86)
	 fit_assoc[i] = 44;
       else if (i == 87)
	 fit_assoc[i] = 45;
       else if (i == 88)
	 fit_assoc[i] = 46;
       else if (i == 89)
	 fit_assoc[i] = 47;
       else if (i == 90)
	 fit_assoc[i] = 48;
       else if (i == 91)
	 fit_assoc[i] = 49;
       else if (i == 92)
	 fit_assoc[i] = 50;
       else if (i == 93)
	 fit_assoc[i] = 51;
       else if (i == 94)
	 fit_assoc[i] = 52;
       else if (i == 95)
	 fit_assoc[i] = 53;
       else if (i == 96)
	 fit_assoc[i] = 54;
       else if (i == 97)
	 fit_assoc[i] = 55;
       else if (i == 98)
	 fit_assoc[i] = 56;
       else if (i == 99)
	 fit_assoc[i] = 57;
       else if (i == 100)
	 fit_assoc[i] = 58;
       else if (i == 101)
	 fit_assoc[i] = 59;
       else if (i == 102)
	 fit_assoc[i] = 60;
       else if (i == 103)
	 fit_assoc[i] = 61;

       else if (i == 104)
	 fit_assoc[i] = 20;
       else if (i == 105)
	 fit_assoc[i] = 21;
       else if (i == 106)
	 fit_assoc[i] = 22;
       else if (i == 107)
	 fit_assoc[i] = 17;
       else if (i == 108)
	 fit_assoc[i] = 18;
       else if (i == 109)
	 fit_assoc[i] = 19;

       else if (i == 110)
	 fit_assoc[i] = 50;
       else if (i == 111)
	 fit_assoc[i] = 51;
       else if (i == 112)
	 fit_assoc[i] = 52;
       else if (i == 113)
	 fit_assoc[i] = 53;
       else if (i == 114)
	 fit_assoc[i] = 54;
       else if (i == 115)
	 fit_assoc[i] = 55;

       else if (i == 116)
	 fit_assoc[i] = 44;
       else if (i == 117)
	 fit_assoc[i] = 45;
       else if (i == 118)
	 fit_assoc[i] = 46;
       else if (i == 119)
	 fit_assoc[i] = 47;
       else if (i == 120)
	 fit_assoc[i] = 48;
       else if (i == 121)
	 fit_assoc[i] = 49;

       else if (i == 122)
	 fit_assoc[i] = 20;
       else if (i == 123)
	 fit_assoc[i] = 21;
       else if (i == 124)
	 fit_assoc[i] = 22;
       else if (i == 125)
	 fit_assoc[i] = 20;
       else if (i == 126)
	 fit_assoc[i] = 21;
       else if (i == 127)
	 fit_assoc[i] = 22;

       else if (i == 128)
	 fit_assoc[i] = 50;
       else if (i == 129)
	 fit_assoc[i] = 51;
       else if (i == 130)
	 fit_assoc[i] = 52;
       else if (i == 131)
	 fit_assoc[i] = 53;
       else if (i == 132)
	 fit_assoc[i] = 54;
       else if (i == 133)
	 fit_assoc[i] = 55;

       else if (i == 134)
	 fit_assoc[i] = 50;
       else if (i == 135)
	 fit_assoc[i] = 51;
       else if (i == 136)
	 fit_assoc[i] = 52;
       else if (i == 137)
	 fit_assoc[i] = 53;
       else if (i == 138)
	 fit_assoc[i] = 54;
       else if (i == 139)
	 fit_assoc[i] = 55;



     }// end assoc loop

     cout << "Done initializing fit_assoc" << endl;
     cout << "Nplots = " << Nplots << endl;
  for (int i = 1; i < Nplots; i++){//intialize histograms used in the fit   
    //  cout << "i = " << i << "; fit_assoc = " << fit_assoc[i] << endl ; //"; pointer: " << hmc[ fit_assoc[i] ] << endl;
    if (! fit_init [ fit_assoc[i] ]){
      hmc[ fit_assoc[i] ] = (TH1F*)(template[i].h)->Clone(fit_name[ fit_assoc[i] ]);
      fit_init [ fit_assoc[i] ] = 1;
    }
    else                                 
      hmc[ fit_assoc[i] ]->Add(template[i].h);
  }
  

  TFile ffit("fit_inputs/" + var_desc + systDesc + "_shape_" + jetBin + "j_" + nTags + "tag.root","recreate");
  hdata->Write();
  for (int i = 1; i < Nfit; i++)
    hmc[i]->Write();
  ffit->Close();

//      cout << "Value fl_data = Value(" << fit_val[0] << ", " << fit_err[0] << "); " << endl;
//      cout << "Value fb_data = Value(" << fit_val[1] << ", " << fit_err[1] << "); " << endl;
//      cout << "Value fc_data = Value(" << fit_val[2] << ", " << fit_err[2] << "); " << endl;
   }

   c1->SaveAs(plot_name + ".gif");
   c1->SaveAs(plot_name + ".eps");
   gROOT->ProcessLine(".! epstopdf " + plot_name + ".eps");
   
   //   gApplication->Terminate();


}
