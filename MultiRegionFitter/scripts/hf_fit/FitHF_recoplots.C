{

  //**************
    // Purpose: Create fit inputs for kfactor Fit: kfactor/Fit_Kfactor.C 
    // all jets matched to LF are called mistags
    // therefore requires a proper qcd and LF scale factors
    // obtained from scripts: qcdSF_4HFfit X Fit_wConstr 
    // and LSF_4HFfit
    //***************
    gROOT->LoadMacro("root_macros/TDistribution.cc");
  
  //**********
      // Inputs
      //**********
	  
  bool plot_only = 0; 
  bool make_legend = true;
  
  
  TString jetBin = "1";//1, 2, 3, 4, 5
  TString nTags = "1";// 1,2,12
  TString systDesc = ""; // for shifted JES, b-tag, etc. Make sure it begins with _ if not null
                               // and that the corresponding files have this tag as well
                               // if _btag*, make sure that the variable on line 185 corresponds to this tag
  
  //TAGGERS
   TString tagger = "";   TString tagger_ind = "T"; TString tagger_desc = "Tight";
  // TString tagger = "L";   TString tagger_ind = "L";  TString tagger_desc = "Loose";
  // TString tagger = "U";   TString tagger_ind = "U";  TString tagger_desc = "UltraTight";
  TString MetCut = "25";// 20-60
  bool singleTopQcdVeto = 1;  //0,1
  int NBINS = 20;  
  
  
  // VARIABLES
  TString var = "evt.Met";  TString xtitle = "MET [GeV]"; float minHist = 25.;   float maxHist = 125.;        TString var_desc = tagger + "MET" ; NBINS = 25; TString ytitle = "# Events / [4 GeV]"; 

  // TString var = "evt.Ht";  TString xtitle = "H_{T} [GeV]"; float minHist = 25.;   float maxHist = 125.; TString var_desc = tagger + "HT" ; NBINS = 50; TString ytitle = "# Events / [2 GeV]";

  // Lepton Plots
  //  TString var = "lep.Pt";  TString xtitle = "Lepton P_{T} [GeV]"; float minHist = 20.;   float maxHist = 100.;  TString var_desc = tagger + "LeptonPt" ;   NBINS = 40; TString ytitle = "# Events / [2 GeV]";

  //  TString var = "lep.Eta";  TString xtitle = "Lepton #eta"; float minHist = -2.5;   float maxHist = 2.5;  NBINS = 25;  TString var_desc = tagger + "LeptonEta" ; TString ytitle = "# Events";

  //  TString var = "lep.Z0";  TString xtitle = "Lepton z0 [cm]"; float minHist = -50;   float maxHist = 50;  NBINS = 50;  TString var_desc = tagger + "Leptonz0" ; TString ytitle = "# Events / [2 cm]";

  //  TString var = "lep.TrkChi2";  TString xtitle = "#chi^{2} of lepton"; float minHist = 0.;   float maxHist = 3.;  NBINS = 30;  TString var_desc = tagger + "lepChi2"; TString ytitle = "# Events";

  //  TString var = "lep.mTW";  TString xtitle = "M_{T} of W [GeV]"; float minHist = 0.;   float maxHist = 200.;  NBINS = 50;  TString var_desc = tagger + "mTW"; TString ytitle = "# Events / [4 GeV]"; MetCut = 0;


  // Jet plots
  //  TString var = "jet1.Eta";  TString xtitle = "#eta(j1)";  float minHist = -2.5;   float maxHist = 2.5; TString var_desc = tagger + "Etaj1" ; NBINS = 25; TString ytitle = "# Events";

  //  TString var = "jet2.Eta";  TString xtitle = "#eta(j2)";  float minHist = -2.5;   float maxHist = 2.5; TString var_desc = tagger + "Etaj2" ; NBINS = 25; TString ytitle = "# Events";

  //  TString var = "jet3.Eta";  TString xtitle = "#eta(j3)";  float minHist = -2.5;   float maxHist = 2.5; TString var_desc = tagger + "Etaj3" ; NBINS = 25; TString ytitle = "# Events";

  // TString var = "jet4.Eta";  TString xtitle = "#eta(j4)";  float minHist = -2.5;   float maxHist = 2.5; TString var_desc = tagger + "Etaj4" ; NBINS = 25; TString ytitle = "# Events";

  // TString var = "jet1.Et";  TString xtitle = "E_{T}(j1) [GeV]";  float minHist = 20.;   float maxHist = 220; TString var_desc = tagger + "Etj1" ; NBINS = 40; TString ytitle = "# Events / [5 GeV]";

  //  TString var = "jet2.Et";  TString xtitle = "E_{T}(j2) [GeV]";  float minHist = 20.;   float maxHist = 220; TString var_desc = tagger + "Etj2"; NBINS = 40; TString ytitle = "# Events / [5 GeV]";

  //  TString var = "jet3.Et";  TString xtitle = "E_{T}(j3) [GeV]";  float minHist = 20.;   float maxHist = 220; TString var_desc = tagger + "Etj3"; NBINS = 40; TString ytitle = "# Events / [5 GeV]";

  //  TString var = "jet4.Et";  TString xtitle = "E_{T}(j4) [GeV]";  float minHist = 20.;   float maxHist = 220; TString var_desc = tagger + "Etj4"; NBINS = 40; TString ytitle = "# Events / [5 GeV]";

  //  TString var = "jet1.Phi";  TString xtitle = "#phi(j1)";  float minHist = 0.;   float maxHist = 2*TMath::Pi(); TString var_desc = tagger + "Phij1"; NBINS = 20; TString ytitle = "# Events";

  //  TString var = "jet2.Phi";  TString xtitle = "#phi(j2)";  float minHist = 0.;   float maxHist = 2*TMath::Pi(); TString var_desc = tagger + "Phij2"; NBINS = 20; TString ytitle = "# Events";

  //  TString var = "jet3.Phi";  TString xtitle = "#phi(j3)";  float minHist = 0.;   float maxHist = 2*TMath::Pi(); TString var_desc = tagger + "Phij3"; NBINS = 20; TString ytitle = "# Events";

  //  TString var = "jet4.Phi";  TString xtitle = "#phi(j4)";  float minHist = 0.;   float maxHist = 2*TMath::Pi(); TString var_desc = tagger + "Phij4"; NBINS = 20; TString ytitle = "# Events";


  // Vertex Plots
  //  TString var = "bjet" + tagger + "1.L2d*bjet" + tagger + "1.Mvtx/bjet" + tagger + "1.PtVtx";  TString xtitle = "c#tau [cm]";  float minHist =0.;   float maxHist = 0.2; TString var_desc = tagger + "ctau" ; NBINS = 40;

  //  TString var = "bjet" + tagger + "1.L2d";  TString xtitle = "L_{2d}(v1) [cm]";  float minHist = 0.;   float maxHist = 1.6; TString var_desc = tagger + "L2dv1" ; NBINS = 40;  TString ytitle = "# Events / [0.04 cm]";

  //  TString var = "bjet" + tagger + "2.L2d";  TString xtitle = "L_{2d}(v2) [cm]";  float minHist = 0.;   float maxHist = 1.6; TString var_desc = tagger + "L2dv2" ; NBINS = 40;  TString ytitle = "# Events / [0.04 cm]";

  //  TString var = "bjet" + tagger + "1.Mvtx";  TString xtitle = "Mass(v1) [GeV]";  float minHist = 0.;   float maxHist = 5; TString var_desc = tagger + "VtxMassv1" ; NBINS = 25;  TString ytitle = "# Events / [0.2 GeV]";

  //  TString var = "bjet" + tagger + "2.Mvtx";  TString xtitle = "Mass(v2) [GeV]";  float minHist = 0.;   float maxHist = 5; TString var_desc = tagger + "VtxMassv2" ; NBINS = 25;  TString ytitle = "# Events / [0.2 GeV]";

  //   TString var = "bjet" + tagger + "1.PtVtx";  TString xtitle = "p_{T}(v1) [GeV]";  float minHist = 0.;   float maxHist = 100; TString var_desc = tagger + "VtxPtv1" ; NBINS = 25;  TString ytitle = "# Events / [4 GeV]";

  //   TString var = "bjet" + tagger + "2.PtVtx";  TString xtitle = "p_{T}(v2) [GeV]";  float minHist = 0.;   float maxHist = 100; TString var_desc = tagger + "VtxPtv2" ; NBINS = 25;  TString ytitle = "# Events / [4 GeV]";





  //  TString var = "evt.dR12";  TString xtitle = "#Delta R(j1,j2)";  float minHist = 0.;   float maxHist = 5.0; TString var_desc = tagger + "DeltaR12" ; 
  
  // TString var = "abs(jet1.Eta-jet2.Eta)";  TString xtitle = "#Delta #eta(j1,j2)";  float minHist = 0.;   float maxHist = 2.5; TString var_desc = tagger + "DeltaEta12" ; 
  
  //TString var = "jet1.Et+jet2.Et";  TString xtitle = "#Sum jet E_{T} [GeV]";  float minHist = 0.;   float maxHist = 300; TString var_desc = tagger + "Sum jetEt" ; 
  
  // TString var = "DeltaPhi(jet1.Phi,lep.Phi)";  TString xtitle = "#Delta #Phi(j,lep)";  float minHist = 0.;   float maxHist = 3.5; TString var_desc = tagger + "DeltaPhiJL" ; 
  

  // KIT / SecvtxMass variables
  //  TString var = "bjet" + tagger + "1.KNN.ctau";  TString xtitle = "c#tau(j1) [cm]";  float minHist = 0.;   float maxHist = 0.2; TString var_desc = tagger + "ctauj1_FS" ; NBINS = 40; TString ytitle = "# Events / [0.005 cm]";

  //  TString var = "bjet" + tagger + "2.KNN.ctau";  TString xtitle = "c#tau(j2) [cm]";  float minHist = 0.;   float maxHist = 0.2; TString var_desc = tagger + "ctauj2_FS" ; NBINS = 40; TString ytitle = "# Events / [0.005 cm]";

  //  TString var = "bjet" + tagger + "1.KNN.Ntrk";  TString xtitle = "N_{trk}(j1)";  float minHist = -0.5;   float maxHist = 20.5; TString var_desc = tagger + "Ntrkj1_FS" ; NBINS = 21; TString ytitle = "# Events";

  //  TString var = "bjet" + tagger + "2.KNN.Ntrk";  TString xtitle = "N_{trk}(j2)";  float minHist = -0.5;   float maxHist = 20.5; TString var_desc = tagger + "Ntrkj2_FS" ; NBINS = 21; TString ytitle = "# Events";










  TString plot_name = "hf_fit/results/" + var_desc + systDesc + "_" + jetBin +"j_" + nTags + "tag" + tagger + "_reco";
  if (singleTopQcdVeto) plot_name += "_veto";
  
  //***** End: Inputs ------//
  
  // overlay Nplots from different root files 
  const int Nplots = 24;
  
  TString LLSF = "2.676*lep.Eff*";
  // use this for LSF anyway
  TString WSF = "1.35*";
  TString WHF = "1.5*";
  TString ZLF = "1.4*";
  TString ZHF = "2.0*";
  TString WBBFIT = "1.62*";
  TString WCCFIT = "1.63*";
  TString WCFIT = "1.57*";
  TString WQQFIT = "1.14*";
  TString TOPFIT = "1.05*";
  TString EWFIT = "0.99*";
  TString QCDFIT = "0.99*";
  
  
  TString qcdSF = "";  // (qcdSF_4HFfit X Fit_wConstr )
  TString LSF = "1*"; // LSF_4HFfit
  
  if (jetBin == "1" && tagger_ind == "T"){
    qcdSF = "1*0.0710666*";
    //qcdSF = "0.241585*0.0710666*";
    //   LSF = "1.38134*";
  } else if (jetBin == "1" && tagger_ind == "L"){
    qcdSF = "1*0.107003*";
    //qcdSF = "0.145716*0.107003*";
    //  LSF = "1.22921*";
  } else if (jetBin == "1" && tagger_ind == "U"){
    qcdSF = "1*0.0243146*";
    //qcdSF = "0.779061*0.0243146*";
    //  LSF = "1.06019*";
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "1*0.286776*"; 
    //qcdSF = "0.318131*0.286776*"; 
    //  LSF = "1.33085*"; 
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "1*0.218136*"; 
    //qcdSF = "0.20028*0.218136*"; 
    //   LSF = "1.16276*"; 
  } else if (jetBin == "2" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "1*0.0896162*"; 
    //qcdSF = "0.986207*0.0896162*"; 
    //   LSF = "0.945689*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "1*0.00562856*"; 
    // qcdSF = "9.53333*0.00562856*"; 
    //   LSF = "3.71939*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "1*0.00851716*"; 
    //qcdSF = "4.68852*0.00851716*"; 
    //   LSF = "1.57309*"; 
  } else if (jetBin == "2" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "1*0.00184308*"; 
    //qcdSF = "35.75*0.00184308*"; 
    //   LSF = "2.8243*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "1*0.296376*"; 
    //qcdSF = "0.442529*0.296376*"; 
    //   LSF = "1.29184*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "1*0.614599*"; 
    //qcdSF = "0.28*0.614599*"; 
    //   LSF = "1.18005*"; 
  } else if (jetBin == "3" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "1*0.0784804*"; 
    //qcdSF = "1.24194*0.0784804*"; 
    //   LSF = "1.05963*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "1*1.21568e-07*"; 
    //qcdSF = "12.8333*1.21568e-07*"; 
    //   LSF = "1.58271*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "1*4.46547e-09*";
    // qcdSF = "5.1333*4.46547e-09*";
    //   LSF = "1.60104*"; 
  } else if (jetBin == "3" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "1*1.89621e-08*"; 
    //qcdSF = "38.5*1.89621e-08*"; 
    //   LSF = "0.439978*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "1*0.14526*"; 
    //qcdSF = "1.75758*0.14526*"; 
    //   LSF = "1.35387*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "1*0.350817*"; 
    //qcdSF = "1.16*0.350817*"; 
    //   LSF = "1.23416*"; 
  } else if (jetBin == "4" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "1*9.44046e-09*"; 
    //qcdSF = "2.9*9.44046e-09*"; 
    //   LSF = "1.10999*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "1*6.81147e-10*"; 
    //qcdSF = "19.3333*6.81147e-10*"; 
    //   LSF = "1.98906*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "1*3.61481e-08*"; 
    //qcdSF = "11.6*3.61481e-08*"; 
    //   LSF = "1.49252*"; 
  } else if (jetBin == "4" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "1*2.42987e-10*"; 
    //qcdSF = "58*2.42987e-10*"; 
    //   LSF = "0.921002*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "T"){
    qcdSF = "1*0.139088*"; 
    //qcdSF = "3*0.139088*"; 
    //   LSF = "1.29701*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "L"){
    qcdSF = "1*0.212022*"; 
    //qcdSF = "2*0.212022*"; 
    //   LSF = "1.15329*"; 
  } else if (jetBin == "5" && nTags == "1" && tagger_ind == "U"){
    qcdSF = "1*1.31753e-09*"; 
    //qcdSF = "12*1.31753e-09*"; 
    //   LSF = "0.971391*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "T"){
    qcdSF = "1*3.52158e-10*"; 
    //qcdSF = "0*3.52158e-10*"; 
    //  LSF = "2.34008*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "L"){
    qcdSF = "1*1.70928e-10*"; 
    //qcdSF = "0*1.70928e-10*"; 
    //   LSF = "1.47517*"; 
  } else if (jetBin == "5" && nTags == "2" && tagger_ind == "U"){
    qcdSF = "1*1.08137e-11*";
    // qcdSF = "0*1.08137e-11*";
    //   LSF = "5.04432*"; 
  }
  
  
  
  TString Bprob = "1*";
  if (nTags != "pre")
    Bprob = "evt.bProb" +  nTags + tagger_ind + "*"; // change the last part to mn* for -1 sigma, pl*
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

  // MetCut 
  condition = " evt.Met > " + MetCut + " && " + condition;
  
  // jetBin
  if (jetBin == "5") { // want to do >= 5 jets
    condition = "  evt.nJets ==  " + jetBin + " && " + condition;
  } else {
    condition = "  evt.nJets ==  " + jetBin + " && " + condition;
  }

  TString condition_data = condition;
  if (nTags != "12") {
    condition_data += " && evt.n" + tagger_ind + "posTags == " + nTags; 
  } else {
    condition_data += " && (evt.n" + tagger_ind + "posTags == 1 || evt.n" + tagger_ind + "posTags == 2)";
  }
  
  TString condition_qcd = condition;

  cout << "Plotting " << var << endl;
  cout << "Event Selection = " << condition_data << endl;
  cout << "QCD Selection = " << condition_qcd << endl;
  cout << "MC Selection = " << condition << endl;


  TString bottom = "evt." + tagger_ind + "matchB && "; 
  TString charm = "evt." + tagger_ind + "matchC && "; 
  TString HF = " ( evt." + tagger_ind + "matchB || evt." + tagger_ind + "matchC ) && " ;
  TString LF = "  ( !evt." + tagger_ind + "matchB && !evt." + tagger_ind + "matchC ) && " ;
  
  //////////////////////////////////  SAMPLES TO USE /////////////////////////////////
  
  TDistribution template[Nplots];  
  for (int ind = 0; ind < Nplots; ind++) {
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
  template[ind].marker_color = 33;
  template[ind++].condition = qcdSF + QCDFIT + "(" + condition_qcd + ")"; 

  // EW
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "EW";   
  template[ind].line_color = 89; // = 102;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 102;
  template[ind++].condition = LLSF + EWFIT + Bprob + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 105;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 105;
  template[ind++].condition =  LLSF + EWFIT  + Bprob + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 89; // = 103;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 103;
  template[ind++].condition = ZHF + EWFIT  + Bprob + LLSF + "evt.AlpgenW*(" + HF +condition + ")"; 
  
  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = "";   
  template[ind].line_color = 1; // = 209;
  template[ind].fill_color = 89;
  template[ind].line_width = 1;
  template[ind].marker_color = 209;
  template[ind++].condition = ZHF + EWFIT  + Bprob + LLSF + "evt.AlpgenW*(" + HF +condition + ")"; 
  // end EW

  //Mistags = W+LF
  template[ind].sample = "stop" + systDesc; 
  template[ind].legentry = "W + q#bar{q}"; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + WQQFIT + LSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "vv" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF  + WQQFIT   + LSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "zlf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF + WQQFIT   + LLSF + Bprob + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "zhf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF + WQQFIT   + LLSF + Bprob + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + WQQFIT   + LSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WHF + WSF + WQQFIT  + Bprob  + LLSF + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WHF + WSF + WQQFIT  + Bprob  + LLSF + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 69;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + WQQFIT  + Bprob  + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = ""; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 69;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + WQQFIT  + Bprob  + LSF + "evt.AlpgenW*(" + LF + condition + ")"; 
  // end Mistags = W+LF

  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "W + c";  
  template[ind].line_color = 1; // = 65;
  template[ind].fill_color = 65;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + WCFIT + LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 

  // Wcc
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W + c#bar{c}"; 
  template[ind].line_color = 62;
  template[ind].line_width = 1;
  template[ind].fill_color = 62;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + WCCFIT + LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")";

  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 62;
  template[ind].fill_color = 62;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + WCCFIT +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 62;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + WCCFIT +LLSF + Bprob + "evt.AlpgenW*(" + charm + condition + ")"; 
  // end Wcc
  
  // Wbb
  template[ind].sample = "wlf" + systDesc; 
  template[ind].legentry = "W + b#bar{b}"; 
  template[ind].line_color = 56;
  template[ind].line_width = 1;
  template[ind].fill_color = 56;
  template[ind].marker_style = 8;
  template[ind].marker_color = 51;
  template[ind++].condition = WSF + WBBFIT + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  
  template[ind].sample = "wc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 56;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + WBBFIT +LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  
  template[ind].sample = "wcc" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 56;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 4;
  template[ind++].condition =  WSF + WBBFIT +LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  
  template[ind].sample = "wbb" + systDesc; 
  template[ind].legentry = "";  
  template[ind].line_color = 1;
  template[ind].fill_color = 56;
  template[ind].line_width = 1;
  template[ind].marker_color = 51;
  template[ind++].condition =  WSF + WBBFIT + LLSF + Bprob + "evt.AlpgenW*(" + bottom + condition + ")"; 
  // end Wbb

  template[ind].sample = "top" + systDesc; 
  template[ind].legentry = "t#bar{t}";   
  template[ind].line_color = 1; // = 2;
  template[ind].fill_color = 2;
  template[ind].line_width = 1;
  template[ind].marker_color = 2;
  template[ind++].condition =  LLSF + TOPFIT + Bprob + "evt.AlpgenW*(" + HF + condition + ")"; 








  //   if (ind  < Nplots ){
  //      cout << "ERROR: number of templates = " << ind << " is less than " << Nplots << endl;
  //      //  gApplication->Terminate();
  //      //  }
  
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
  // @@ ======  MAGIC ======@@@@@@ //
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
  gROOT->Macro("root_macros/Overlay.C");
  
  if (!plot_only){
    cout.precision(3);
    
    // ****************************************
    // **** PREPARE FOR FIT  ***
    // ***************************************
    
    const int Nfit = 8; // # of fitted histograms
    double param[Nfit];
    
    TH1F *hdata = (TH1F*)(template[0].h)->Clone("dat");
    
    TH1F *hmc[Nfit];
    TString fit_name[Nfit] = {"Data", "Top", "Wbb", "Wcc", "Wc", "Wqq", "EW", "QCD"};
    
    int fit_assoc[Nplots];
    int temp1;
    bool fit_init[Nfit] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i=0; i<Nplots; i++) { //intialize fit_assoc
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
    
    TFile ffit("fit_inputs/" + var_desc + systDesc + "_" + jetBin + "j_" + nTags + "tag.root", "recreate");
    hdata->Write();
    for (int i = 1; i < Nfit; i++)
      hmc[i]->Write();
    ffit->Close();
  } // if plot_only


    
  c1->SaveAs(plot_name + ".gif");
  c1->SaveAs(plot_name + ".eps");
  gROOT->ProcessLine(".! epstopdf " + plot_name + ".eps");
  
  //   gApplication->Terminate();
  
}
