{

  //*****************
  // Determine qcd SF supplied to HF fitting machinery
  //*****************

  gROOT->LoadMacro("root_macros/TDistribution.cc");
  
  TString jetBin = "5";
  TString nTags = "2"; // not pre!
  //  TString plot_name = "qcd_fit/results/qcd_" + jetBin + "j_" + nTags + "tag";
  TString MetCut = "25";// 20-60
  bool singleTopQcdVeto = 1;
  TString ytitle = "# Events";
  bool make_legend = true;

   TString tagger = ""; TString tagger_ind = "T"; // empty means tight
  // TString tagger = "L"; TString tagger_ind = "L"; // empty means tight
  //  TString tagger = "U"; TString tagger_ind = "U"; // empty means tight

  // overlay Nplots from different root files 
  const int Nplots = 2;

  TString LLSF = "2.676*lep.Eff*";// p0-17
  TString Bprob ="1*"; // event tagging probability
  if (nTags != "pre")
    Bprob = "evt.bProb" +  nTags + tagger_ind + "*";
 
  // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


  bool stack = true; // stack the MC ( do not forget that "0" is data)
  bool norm = false; // switch to normalize the histograms to the same area
  bool doKS = false; // calculate KS probabilities


  int NBINS =120;

  bool setMinMax = false;
  float minHist = 0.; 
  float maxHist = 200.;

  TString tree_name = "nt";

  ////////////////////// VARIABLE //////////////////////////////////////////////

 
   TString var = "min(evt.Met, 199.9)";  TString xtitle = "Missing E_{T}, GeV"; 

  //////////////////////////////  CONDITION ///////////////////////////////////

    TString condition = "evt.isGoodSiRun ";
    if ("5" == jetBin) 
      condition = " evt.nJets >= 5 && " + condition;      
    else
      condition = " evt.nJets == " + jetBin + " && " + condition;

    if (singleTopQcdVeto) condition += " && evt.passedQCDveto  ";
    TString condition_tag = condition;
    TString condition_evSel = condition;

    //if (singleTopQcdVeto) condition_evSel += " && evt.passedQCDveto  ";

    condition_evSel = " evt.Met > " + MetCut + " && " + condition_evSel;

    condition_tag = " evt.n" + tagger_ind + "posTags == " + nTags + " && " + condition; 

    cout << "condition_tag = " << condition_tag << endl;
    cout << "condition_evSel = " << condition_evSel << endl;

  //////////////////////////////////  SAMPLES TO USE /////////////////////////////////

  TDistribution template[Nplots];  
   for (int ind = 0; ind < Nplots; ind++){
    template[ind].var = var;
    template[ind].condition = condition;
  }

  int ind=0;
  template[ind].sample = "anti"; 
  template[ind].legentry = "qcd tag";   
  template[ind].marker_style = 8;
  template[ind].error = "e";
  template[ind++].condition = condition_tag; // tag, but no MET

  template[ind].sample = "anti"; 
  template[ind].legentry = "qcd_evSel";   
  template[ind].line_color = 30;
  template[ind].line_width = 2;
   template[ind].marker_style = 8;
   template[ind].marker_color = 4;
   template[ind++].condition = condition_evSel;


   // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
   // @@ ========  MAGIC ====@@@@@@ //
   // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
   gROOT->Macro("root_macros/Overlay_pretag.C");
 
   float N_qcd_tag =  template[0].h->Integral();
   float N_qcd_evSel =  template[1].h->Integral();

   cout << endl << "Additional qcd SF for HF fit = " << N_qcd_evSel / N_qcd_tag <<  endl << endl;


//   c1->SaveAs(plot_name + ".gif");
//   c1->SaveAs(plot_name + ".eps");
//   // c1->SaveAs(plot_name + ".C");
//   gROOT->ProcessLine(".! epstopdf " + plot_name + ".eps");

 gApplication->Terminate();

}
