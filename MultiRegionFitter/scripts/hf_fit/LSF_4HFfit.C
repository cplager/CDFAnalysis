
{


  gROOT->LoadMacro("root_macros/TDistribution.cc");


  //*****************
  // Determine mistag LF SF supplied to HF fitting machinery
  //*****************

      bool plot_only = 0; 
 
  TString jetBin = "5";//1, 2, 3
  TString nTags = "2";// 1,2,12

  //TAGGERS
   TString tagger = "";   TString tagger_ind = "T"; TString tagger_desc = "";
  // TString tagger = "L";   TString tagger_ind = "L";  TString tagger_desc = " Loose ";
  //   TString tagger = "U";   TString tagger_ind = "U";  TString tagger_desc = " UltraTight ";
  TString MetCut = "25";// 20-60
  bool singleTopQcdVeto = 1;  //0,1
  int NBINS =20;  

 
  // VARIABLES
   TString var = "min(evt.Met, 199.9)";  TString xtitle = "Missing E_{T}, GeV"; bool setMinMax = true; float minHist = 0.; float maxHist = 200.; TString var_desc = "met";
 //***** End: Inputs

     // overlay Nplots from different root files 
  const int Nplots = 18;

  TString LLSF = "2.676*lep.Eff*"; 
  TString WSF = "1.5*";
  TString WHF = "1.5*";
  TString ZLF = "1.4*";
  TString ZHF = "2.0*";

  // 2j pretag

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

  TString condition = "evt.isGoodSiRun "; 
  condition += (" && " + var + " > -999");
  
  //  TString condition_qcd = condition;
  if (singleTopQcdVeto) condition += " && evt.passedQCDveto  ";
  // MetCut 
  condition = " evt.Met > " + MetCut + " && " + condition;
 
 // jetBin
  condition = "  evt.nJets ==  " + jetBin + " && " + condition;
  // condition_qcd = "  evt.nJets ==  " + jetBin + " && " + condition_qcd;

 TString condition_tag = condition;

 condition_tag += " && evt.n" + tagger_ind + "posTags == " + nTags; 
 
 
 TString bottom = "evt." + tagger_ind + "matchB && "; 
 TString charm = "evt." + tagger_ind + "matchC && "; 
 TString HF = " ( evt." + tagger_ind + "matchB || evt." + tagger_ind + "matchC ) && " ;
 TString LF = "  ( !evt." + tagger_ind + "matchB && !evt." + tagger_ind + "matchC ) && " ;
 
  //////////////////////////////////  SAMPLES TO USE /////////////////////////////////

  TDistribution template[Nplots];  
  for (int ind = 0; ind < Nplots; ind++){
    template[ind].var = var;
    template[ind].condition = condition;
  }

  int ind = 0;
  //Mistags
  template[ind].sample = "stop"; 
  template[ind].legentry = "Mistags"; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "vv"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "zlf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF  + LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "zhf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF  + LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "top"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wbb"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WHF + WSF  + LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wcc"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WHF + WSF  + LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wc"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WHF + WSF + LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  template[ind].sample = "wlf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + Bprob + "evt.AlpgenW*(" + LF + condition + ")"; 

  // end Mistags


  //Mistags
  template[ind].sample = "stop"; 
  template[ind].legentry = "Mistags"; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

  template[ind].sample = "vv"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

  template[ind].sample = "zlf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF  + LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

  template[ind].sample = "zhf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = ZLF  + LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

  template[ind].sample = "top"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

  template[ind].sample = "wbb"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WHF + WSF  + LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

  template[ind].sample = "wcc"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WHF + WSF  + LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

  template[ind].sample = "wc"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 30;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WHF + WSF + LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

  template[ind].sample = "wlf"; 
  template[ind].legentry = ""; 
  template[ind].line_color = 1;
  template[ind].line_width = 1;
  template[ind].fill_color = 30;
  template[ind].marker_style = 8;
  template[ind].marker_color = 4;
  template[ind++].condition = WSF + LLSF + "evt.AlpgenW*(" + LF + condition_tag + ")"; 

   // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
   // @@ ========  MAGIC ====@@@@@@ //
   // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
   gROOT->Macro("root_macros/Overlay.C");

   float N_pred_mistag = 0;
   float N_tag_lf = 0;

   for (int i=0; i<Nplots; i++){
     if (i < 9) N_pred_mistag +=  template[i].h->Integral();
     else N_tag_lf +=  template[i].h->Integral();
   }
   cout << "  total predicted = " << N_pred_mistag  << endl;
   cout << "  total observed = " << N_tag_lf  << endl;
   cout << endl << "  L SF = " << (double) N_pred_mistag / N_tag_lf << endl << endl;

   
   gApplication->Terminate();


}
