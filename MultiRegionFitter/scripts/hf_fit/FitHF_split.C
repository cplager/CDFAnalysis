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

  TString jetBin = "1"; //1, 2, 3, 4, 5
  TString nTags = "1";// 1,2,12
  TString systDesc = "_btagm1";       // for shifted JES, b-tag, etc. Make sure it begins with _ if not null
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
  // TString var = "ANN.3j";  TString xtitle =  tagger_desc + "ANN";   float minHist = 0.;  float maxHist = 1.; TString var_desc = "ANN" ; 
  
  // 1 tag KIT
     TString var = "bjet" + tagger + "1.KaNNcorr";  TString xtitle =  tagger_desc + "KIT Flavor Separator";   float minHist = -1.;  float maxHist = 1.; TString var_desc = "KIT" ; 

  // for 2 tags, use the average
  //    TString var = "(bjet" + tagger + "1.KaNNcorr + bjet" + tagger + "2.KaNNcorr) / 2";  TString xtitle =  tagger_desc + "KIT Flavor Separator";   float minHist = -1.;  float maxHist = 1.; TString var_desc = "KIT" ;

  //    TString var = "min(19., bjet" + tagger + "1.KNN.NtrackSig3)";  TString xtitle =  tagger_desc + " Num tracks Sig3";   float minHist = 0.;  float maxHist = 20.; TString var_desc = tagger + "NtrackSig3" ; 

  // SecVtx Mass - only Loose tags
  //   TString var = "min(4.39, bjet" + tagger + "1.Mvtx)";  TString xtitle = tagger_desc + "SECVTX Mass";  float minHist = 0.;   float maxHist = 4.4; TString var_desc = "LooseSecvtxMass" ; NBINS = 22;

  // TString var = "bjet" + tagger + "1.L2d*bjet" + tagger + "1.Mvtx/bjet" + tagger + "1.PtVtx";  TString xtitle = tagger_desc + " c#tau";  float minHist =0.;   float maxHist = 0.4; TString var_desc = tagger + "ctau" ; 

  //  TString var = "bjet" + tagger + "1.KNN.signD0sig1";  TString xtitle = tagger_desc + " signD0sig1";  float minHist = -50.;   float maxHist = 50.;  NBINS =100;  TString var_desc = tagger + "signD0sig1" ; 

  //TString var = "bjet1.KNN.signD02";  TString xtitle = tagger_desc + " signD02";  float minHist = -0.2;   float maxHist = 0.2;  NBINS =100;  TString var_desc = tagger + "signD02" ; 

  //  TString var = "min(39.9,bjet1.PtVtx)";  TString xtitle = tagger_desc + " P_{T,vtx}";  float minHist = 0.;   float maxHist = 40.;
  //  TString var = "bjet1.L2d";  TString xtitle = tagger_desc + " L2d";  float minHist = 0.;   float maxHist = 3.;

  //   TString var = "bjet1.KNN.NtrackSig3";  TString xtitle = tagger_desc + " NtrackSig3";  float minHist = 2.;   float maxHist = 16.;  NBINS =14;   TString var_desc = tagger + "NtrackSig3" ; 

  // TString var = "bjet1.KNN.Ntrk";  TString xtitle = tagger_desc + " Ntracks";  float minHist = 2.;   float maxHist = 16.; NBINS=14;

  //TString var = "bjet1.KNN.signD03";  TString xtitle = tagger_desc + " signD03";  float minHist = -0.2;   float maxHist = 0.2;  NBINS =20;  TString var_desc = tagger + "signD03" ; 

  //TString var = "min(0.4,bjet" + tagger + "1.KNN.ctau)";  TString xtitle = tagger_desc + " c#tau";  float minHist =0.;   float maxHist = 0.4; TString var_desc = tagger_desc + "c#tau" ; 

   //  TString var = "evt.dR12";  TString xtitle = "#Delta R(j1,j2)";  float minHist = 0.;   float maxHist = 5.0; TString var_desc = tagger + "DeltaR12" ; 

  // TString var = "abs(jet1.Eta-jet2.Eta)";  TString xtitle = "#Delta #eta(j1,j2)";  float minHist = 0.;   float maxHist = 2.5; TString var_desc = tagger + "DeltaEta12" ; 

  //TString var = "jet1.Et+jet2.Et";  TString xtitle = "#Sum jet E_{T}";  float minHist = 0.;   float maxHist = 300; TString var_desc = tagger + "Sum jetEt" ; 

  // TString var = "DeltaPhi(jet1.Phi,lep.Phi)";  TString xtitle = "#Delta #Phi(j,lep)";  float minHist = 0.;   float maxHist = 3.5; TString var_desc = tagger + "DeltaPhiJL" ; 

  //  TString var = "jet1.Eta";  TString xtitle = "#Eta(j1)";  float minHist = -2.5;   float maxHist = 2.5; TString var_desc = tagger + "Etaj1" ; 

  // TString var_desc = "KIT" ; //bjet" + tagger + "1.Mvtx";//"bjet1.Mvtx"; //var; //"ctau" ;//var;

//TString var = "bjet" + tagger + "1.KaNNcorr + bjet" + tagger + "2.KaNNcorr";  TString xtitle =  tagger_desc + "KIT Flavor Separator";   float minHist = -2.;  float maxHist = 2.; TString var_desc = tagger + "KIT" ; 

  TString plot_name = "hf_fit/results/" + var_desc + "_split" + systDesc + "_" + jetBin +"j_" + nTags + "tag" + tagger + "_met" + MetCut + "_nbins" + NBINS;
  if (singleTopQcdVeto) plot_name += "_veto";

 //***** End: Inputs

  // overlay Nplots from different root files 
  const int Nplots = 105;

  TString LLSF = "2.676*lep.Eff*"; 
  // use this for LSF anyway
  TString WSF = "1.35*";
  TString ZLF = "1.4*";
  TString ZHF = "2.0*";

  TString qcdSF = "";  // (qcdSF_4HFfit X Fit_wConstr )
  TString LSF = ""; // LSF_4HFfit


if (jetBin == "1" && tagger_ind == "T"){
    qcdSF = "4.4386*0.0707343*";
    LSF = "1.38134*";
  } else if (jetBin == "1" && tagger_ind == "L"){
    qcdSF = "0.145716*0.107003*";
    LSF = "1.22921*";
  } else if (jetBin == "1" && tagger_ind == "U"){
    qcdSF = "0.779061*0.0243146*";
    LSF = "1.06019*";
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "4.6129*0.285361*"; 
    LSF = "1.33085*"; 
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "0.20028*0.218136*"; 
    LSF = "1.16276*"; 
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "0.986207*0.0896162*"; 
    LSF = "0.945689*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "286*0.00562856*"; 
    LSF = "3.71939*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "4.68852*0.00851716*"; 
    LSF = "1.57309*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "35.75*0.00184308*"; 
    LSF = "2.8243*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "4.52941*0.287186*"; 
    LSF = "1.29184*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "0.28*0.614599*"; 
    LSF = "1.18005*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "1.24194*0.0784804*"; 
    LSF = "1.05963*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "0*1.21568e-07*"; 
    LSF = "1.58271*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "5.1333*4.46547e-09*"; 
    LSF = "1.60104*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "38.5*1.89621e-08*"; 
    LSF = "0.439978*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "1.75758*0.14526*"; 
    LSF = "1.35387*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "1.16*0.350817*"; 
    LSF = "1.23416*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "2.9*9.44046e-09*"; 
    LSF = "1.10999*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "19.3333*6.81147e-10*"; 
    LSF = "1.98906*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "11.6*3.61481e-08*"; 
    LSF = "1.49252*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "58*2.42987e-10*"; 
    LSF = "0.921002*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "3*0.139088*"; 
    LSF = "1.29701*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "2*0.212022*"; 
    LSF = "1.15329*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "12*1.31753e-09*"; 
    LSF = "0.971391*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "0*3.52158e-10*"; 
    LSF = "2.34008*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "0*1.70928e-10*"; 
    LSF = "1.47517*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "0*1.08137e-11*"; 
    LSF = "5.04432*"; 
  }


  TString Bprob = "1*";
  if (nTags != "pre")
    Bprob = "evt.bProb" +  nTags + tagger_ind + "mn*"; // change the last part to mn* for -1 sigma, pl*
                                                       // for +1 sigma, or * for no shift

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
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 105;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 103;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  //Mistags = W+LF
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "W + q#bar{q}"; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF     + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF     + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF    + LLSF + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF    + LLSF + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  // i = 10
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF    + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF    + LLSF + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF    + LLSF + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + LF + condition_data + ")"; 
  // end Mistags = W+LF


  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ c#bar{c}"; 
  template[ind].line_color = 62;
  template[ind].line_width = 1;
  template[ind].fill_color = 62;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")";

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 62;
  template[ind].fill_color = 62;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 62;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+ b#bar{b}"; 
  template[ind].line_color = 56;
  template[ind].line_width = 1;
  template[ind].fill_color = 56;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 

  // i = 20
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 56;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 56;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 51;
  template[ind++].condition =  WSF + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  // End Wbb

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + HF + condition + ")"; 

  cout << "About to split into single tags" << endl;

  // this is where all the templates get split. First for 1 tag, then for 2.



  // EW_b 24
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_b";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bjet1 +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_b";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bjet1 +condition + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_b";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bjet1 +condition + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bjet1 +condition + ")"; 
  // end EW_b

  // EW_c
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_c";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cjet1 +condition + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_c";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + cjet1 +condition + ")"; 
  
  // ind = 30
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_c";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cjet1 +condition + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cjet1 +condition + ")"; 
  // end EW_c

  // EW_q
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_q";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + qjet1 +condition + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_q";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + qjet1 +condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_q";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + qjet1 +condition + ")"; 
  
  // 35
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + qjet1 +condition + ")"; 
  // end EW_q

  // W+qq
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_b"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + bjet1 + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_c"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + cjet1 + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_q"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + qjet1 + condition_data + ")";

  // W+c
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_b";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition + ")"; 

  // ind = 40
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_q";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + qjet1 + condition + ")"; 

  // W+cc
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_b"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_c"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_q"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + qjet1 + condition + ")"; 

  // W+bb
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_b"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_c"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_q"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + qjet1 + condition + ")"; 


  // Top
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_b";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + bjet1 + condition + ")"; 

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_c";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + cjet1 + condition + ")"; 

  // ind = 50
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_q";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + qjet1 + condition + ")"; 

  cout << "About to do double tags" << endl;
  // Now do double tags - this gets messy

  // EW_bb
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_bb";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bb +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_bb";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bb +condition + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_bb";   
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bb +condition + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bb +condition + ")"; 
  // end EW_bb
  
  // EW_bc 55
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_bc";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bc +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_bc";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bc +condition + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_bc";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bc +condition + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bc +condition + ")"; 
  // end EW_bc


  // EW_bq
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_bq";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 
  
  // ind = 60
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_bq";
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_bq";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bq +condition + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + bq +condition + ")"; 
  // end EW_bq

  
  // EW_cc
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_cc";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cc +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_cc";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + cc +condition + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_cc";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cc +condition + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cc +condition + ")"; 
  // end EW_cc
  
  // EW_cq
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_cq";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + cq +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_cq";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + cq +condition + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_cq";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cq +condition + ")"; 

  // ind = 70
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + cq +condition + ")"; 
  // end EW_cq
  
  // EW_qq
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "Single Top_qq";   
  template[ind].line_color = 1; // = 102;
  template[ind].fill_color = 102;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + qq +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "Diboson_qq";   
  template[ind].line_color = 1; // = 105;
  template[ind].fill_color = 105;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF  + Bprob + "evt.AlpgenW*(" + qq +condition + ")"; 
 
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "Z+jets_qq";
  template[ind].line_color = 1; // = 103;
  template[ind].fill_color = 103;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + qq +condition + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 209;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF  + Bprob + LLSF + "evt.AlpgenW*(" + qq +condition + ")"; 
  // end EW_qq

  
  // W+qq
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_bb"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + bb + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_bc"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + bc + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_bq"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + bq + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_cc"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + cc + condition_data + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_cq"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + cq + condition_data + ")"; 

  // ind = 80
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W+q#bar{q}_qq"; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF    + LSF + "evt.AlpgenW*(" + qq + condition_data + ")"; 
  // End W+qq


  // W+c
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_bb";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + bb + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_bc";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + bc + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_bq";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_cc";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + cc + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_cq";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + cq + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W+c_qq";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF +LLSF + Bprob + "evt.AlpgenW*(" + qq + condition + ")"; 
  // End W+c

  // W+cc
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_bb"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bb + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_bc"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bc + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_bq"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 

  // ind = 90
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_cc"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cc + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_cq"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cq + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "W+c#bar{c}_qq"; 
  template[ind].line_color = 4;
  template[ind].line_width = 1;
  template[ind].fill_color = 4;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + qq + condition + ")"; 
  // End W+cc

  // W+bb
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_bb"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bb + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_bc"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bc + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_bq"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_cc"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cc + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_cq"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + cq + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "W+b#bar{b}_qq"; 
  template[ind].line_color = 51;
  template[ind].line_width = 1;
  template[ind].fill_color = 51;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + qq + condition + ")"; 


  // Top
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_bb";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + bb + condition + ")"; 

  // ind = 100
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_bc";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + bc + condition + ")"; 

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_bq";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + bq + condition + ")"; 

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_cc";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + cc + condition + ")"; 

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_cq";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + cq + condition + ")"; 

  // ind = 104
  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}_qq";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + Bprob + "evt.AlpgenW*(" + qq + condition + ")"; 

  cout << "Got through all the templates" << endl;

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
     TString fit_name[Nfit] = {"Data", "top", "Wbb", "Wcc", "Wc", "Wqq", "EW", "QCD",                        // 8
			       "EW_b", "EW_c", "EW_q", "Wqq_b", "Wqq_c", "Wqq_q", "Wc_b", "Wc_c", "Wc_q",    // 17
			       "Wcc_b", "Wcc_c", "Wcc_q", "Wbb_b", "Wbb_c", "Wbb_q",                         // 23
			       "top_b", "top_c", "top_q",                                                    // 26
			       "EW_bb", "EW_bc", "EW_bq", "EW_cc", "EW_cq", "EW_qq",                         // 32
			       "Wqq_bb", "Wqq_bc", "Wqq_bq", "Wqq_cc", "Wqq_cq", "Wqq_qq",                   // 38
			       "Wc_bb", "Wc_bc", "Wc_bq", "Wc_cc", "Wc_cq", "Wc_qq",                         // 44
			       "Wcc_bb", "Wcc_bc", "Wcc_bq", "Wcc_cc", "Wcc_cq", "Wcc_qq",                   // 50
			       "Wbb_bb", "Wbb_bc", "Wbb_bq", "Wbb_cc", "Wbb_cq", "Wbb_qq",                   // 56
			       "top_bb", "top_bc", "top_bq", "top_cc", "top_cq", "top_qq"};                  // 62
								   


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
     for (int i = 0; i < Nplots; i++){ //intialize fit_assoc
       if (0 == i)
	 fit_assoc[i] = 0;
       else if (1 == i)
	 fit_assoc[i] = 7;
       else if (i <= 5)
	 fit_assoc[i] = 6;
       else if (i <= 14)
	 fit_assoc[i] = 5;
       else if (15 == i)
	 fit_assoc[i] = 4;
       else if (i >= 16 && i <= 18)
	 fit_assoc[i] = 3;
       else if (i >= 19 && i <= 22)
	 fit_assoc[i] = 2;
       else if (23 == i)
	 fit_assoc[i] = 1;

       // Begin split tempaltes, single tag
       else if (i >= 24 && i <= 27)
	 fit_assoc[i] = 8;
       else if (i >= 28 && i <= 31) 
	 fit_assoc[i] = 9;
       else if (i >= 32 && i <= 35)
	 fit_assoc[i] = 10;
       else if (i >= 36 && i <= 50)
	 fit_assoc[i] = i - 25;

       // Begin split templates, double tags
       else if (i >= 51 && i <= 54)
	 fit_assoc[i] = 26;
       else if (i >= 55 && i <= 58)
	 fit_assoc[i] = 27;
       else if (i >= 59 && i <= 62)
	 fit_assoc[i] = 28;
       else if (i >= 63 && i <= 66)
	 fit_assoc[i] = 29;
       else if (i >= 67 && i <= 70)
	 fit_assoc[i] = 30;
       else if (i >= 71 && i <= 74)
	 fit_assoc[i] = 31;
       else if (i >= 75 && i <= 104)
	 fit_assoc[i] = i - 43;
       
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
  

  TFile ffit("fit_inputs/" + var_desc + "_split" + systDesc + "_" + jetBin + "j_" + nTags + "tag.root","recreate");
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
