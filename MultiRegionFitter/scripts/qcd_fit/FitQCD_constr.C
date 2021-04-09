{

  //*****************
  // Fit QCD fraction
  //*****************

  gROOT->LoadMacro("root_macros/TDistribution.cc");

  // inputs to the scripts - read from input file
//   int jetBin = 2;
//   int nTags = 12;//negative == pre-tag
//   TString plot_name = "qcd_fit_results/qcd_2j_12tag";
  
  TString jetBin = "5";
  TString nTags = "1";
  TString plot_name = "qcd_fit/results/qcd_" + jetBin + "j_" + nTags + "tag";
  bool singleTopQcdVeto = 1;
  if (singleTopQcdVeto) plot_name += "veto";

  TString ytitle = "";
  bool make_legend = true;

     TString tagger = ""; TString tagger_ind = "T"; // empty means tight
  //  TString tagger = "L"; TString tagger_ind = "L"; // empty means tight
  //  TString tagger = "U"; TString tagger_ind = "U"; // empty means tight

  // overlay Nplots from different root files 
  const int Nplots = 11;

  TString LLSF = "2.676*lep.Eff*";// p0-17
  TString Bprob ="1*"; // event tagging probability
  if (nTags != "pre")
    Bprob = "evt.bProb" +  nTags + tagger_ind + "*";
 
  // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


  bool stack = true; // stack the MC ( do not forget that "0" is data)
  bool norm = false; // switch to normalize the histograms to the same area
  bool doKS = false; // calculate KS probabilities


  int NBINS =120;
  if (nTags != "pre") NBINS /= 2;
  if (nTags == "2") NBINS /= 2;

  bool setMinMax = false;
  float minHist = 0.; 
  float maxHist = 200.;

  TString tree_name = "nt";

  ////////////////////// VARIABLE //////////////////////////////////////////////

  
  // TString var = "evt.nJets";  TString xtitle = "Jet Multiplicity"; 
    TString var = "min(evt.Met, 199.9)";  TString xtitle = "Missing E_{T}, GeV"; 
   //   TString var = "bjet1.KaNN";  TString xtitle = "Karlsruhe NN Flavor Separator"; 
  //  TString var = "bjet1.bTagProb*bjet1.bTagErr";  TString xtitle = "b-Tag Probability"; 

  //////////////////////////////  CONDITION ///////////////////////////////////

    TString condition = "evt.isGoodSiRun ";
    if (singleTopQcdVeto) condition += " && evt.passedQCDveto  ";

    if ("5" == jetBin)
      condition = " evt.nJets >= 5 && " + condition;
    else 
      condition = " evt.nJets == " + jetBin + " && " + condition;

    TString condition_data = condition;
    TString condition_qcd = condition;
    if (nTags != "pre"){
      condition_data = " evt.n" + tagger_ind + "posTags == " + nTags + " && " + condition; 
      if (nTags == "12")
	condition_data = " evt.n" + tagger_ind + "posTags >= 1 && " + condition; 
    }

    //alpgen weighting
    condition = "evt.AlpgenW*(" + condition + ")";

    TString MCcondition = Bprob + LLSF + condition;
    
    cout << "Plotting " << var << endl;
    cout << "Event Selection = " << condition_data << endl;
    cout << "MC: " << MCcondition << endl;


  //////////////////////////////////  SAMPLES TO USE /////////////////////////////////

  TDistribution template[Nplots];  
   for (int ind = 0; ind < Nplots; ind++){
    template[ind].var = var;
    template[ind].condition = condition;
  }

  int ind=0;
  template[ind].sample = "data"; 
  template[ind].legentry = "data (2.7 fb^{-1})";   
  template[ind].marker_style = 8;
  template[ind].error = "e";
  template[ind++].condition = condition_data;
 

  template[ind].sample = "wlf"; 
  template[ind].legentry = "W+q#bar{q}";   
  template[ind].line_color = 30;
  template[ind].line_width = 2;
  //  template[ind].fill_color = 30;
   template[ind].marker_style = 8;
   template[ind].marker_color = 4;
   template[ind++].condition = MCcondition;

   template[ind].sample = "wbb"; 
   template[ind].legentry = "W+b#bar{b}";   
   template[ind].line_color = 51;
   //  template[ind].fill_color = 51;
   template[ind].line_width = 2;
   template[ind].marker_color = 51;
   template[ind++].condition =  MCcondition;

   template[ind].sample = "wcc"; 
   template[ind].legentry = "W+c#bar{c}";   
   template[ind].line_color = 4;
   // template[ind].fill_color = 4;
   template[ind].line_width = 2;
   template[ind].marker_color = 4;
   template[ind++].condition =     MCcondition;

   template[ind].sample = "wc"; 
   template[ind].legentry = "W+c";   
   template[ind].line_color = 65;
   // template[ind].fill_color = 65;
   template[ind].line_width = 2;
   template[ind].marker_color = 65;
   template[ind++].condition =     MCcondition;

   template[ind].sample = "top"; 
   template[ind].legentry = "t#bar{t}";   
   template[ind].line_color = 2;
   //  template[ind].fill_color = 2;
   template[ind].line_width = 2;
   template[ind].marker_color = 2;
   template[ind++].condition = MCcondition;

   template[ind].sample = "stop"; 
   template[ind].legentry = "Single Top";   
   template[ind].line_color = 102;
   //  template[ind].fill_color = 102;
   template[ind].line_width = 2;
   template[ind].marker_color = 102;
   template[ind++].condition = MCcondition;


   template[ind].sample = "vv"; 
   template[ind].legentry = "Diboson";   
   template[ind].line_color = 105;
   // template[ind].fill_color = 105;
   template[ind].line_width = 2;
   template[ind].marker_color = 105;
   template[ind++].condition = MCcondition;


   template[ind].sample = "zlf"; 
   template[ind].legentry = "Z+LF";   
   template[ind].line_color = 103;
   //  template[ind].fill_color = 103;
   template[ind].line_width = 2;
   template[ind].marker_color = 103;
   template[ind++].condition = MCcondition;

   template[ind].sample = "zhf"; 
   template[ind].legentry = "Z+HF";   
   template[ind].line_color = 209;
   //   template[ind].fill_color = 209;
   template[ind].line_width = 2;
   template[ind].marker_color = 209;
   template[ind++].condition = MCcondition;

   template[ind].sample = "anti"; 
   template[ind].legentry = "QCD";   
   template[ind].line_color = 117;
   // template[ind].fill_color = 117;
   template[ind].line_width = 2;
   template[ind].marker_color = 117;
   template[ind++].condition = condition_qcd; // no tag requirement 

   // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
   // @@ ========  MAGIC ====@@@@@@ //
   // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
   gROOT->Macro("root_macros/Overlay_pretag.C");
 
   // ****************************************
   // **** PREPARE FOR FIT  ***
   // ***************************************

   const int Nfit = 3; // # of fitted histograms
  // fit is done for w+jets + (ewk+top) and qcd: 3 contributions in total

   TH1F *hdata = (TH1F*)(template[0].h)->Clone("dat");

   TH1F *hmc[Nfit];
   TString fit_name[Nfit] = {"w+jets", "ewk+top", "qcd" };
   bool constr[Nfit] = { 0, 1, 0};
   double error[Nfit] = {0. , 0.1, 0.}; // have an effect only for constrained fractions

   int fit_assoc[Nplots];
   bool fit_init[Nfit] = {0, 0, 0};
  for (int i=0; i<Nplots; i++){//intialize fit_assoc
    if (template[i].sample == "wlf" || template[i].sample == "wbb" || template[i].sample == "wcc" || template[i].sample == "wc")
      fit_assoc[i] = 0;
    else if (template[i].sample == "top" || template[i].sample == "stop" || template[i].sample == "vv" 
	     || template[i].sample == "zlf"  || template[i].sample == "zhf")
      fit_assoc[i] = 1;
    else if (template[i].sample == "anti")
      fit_assoc[i] = 2;
  }// end assoc loop


  for (int i=1; i<Nplots; i++){//intialize histograms used in the fit    
    cout << "i = " << i << "; fit_assoc = " << fit_assoc[i] << endl ; //"; pointer: " << hmc[ fit_assoc[i] ] << endl;
    if (! fit_init [ fit_assoc[i] ]){
      hmc[ fit_assoc[i] ] = (TH1F*)(template[i].h)->Clone(fit_name[ fit_assoc[i] ]);
      fit_init [ fit_assoc[i] ] = 1;
    }
    else                                 
      hmc[ fit_assoc[i] ]->Add(template[i].h);
  }


  gROOT->Macro("root_macros/Fit_wConstr.C");

  c1->SaveAs(plot_name + ".gif");
  c1->SaveAs(plot_name + ".eps");
  // c1->SaveAs(plot_name + ".C");
  gROOT->ProcessLine(".! epstopdf " + plot_name + ".eps");

  //gApplication->Terminate();

}
