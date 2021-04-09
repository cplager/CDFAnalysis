void mergeTemplates () {

  //////////// Declare Variables ///////////////////////

  TString Tag = "_2btag_jesdown";
  TString HadFileName = "sqwatTop_ttbar_had_53X"+Tag+".root";
  TString SemFileName = "sqwatTop_ttbar_semilep_53X"+Tag+".root";
  TString DilFileName = "sqwatTop_ttbar_dilep_53X"+Tag+".root";
  TString outputName  = "sqwatTop_ttbar_53X_comb"+Tag+".root";

  //xsec in nb
  // double ttXSec = 0.225;
  double ttXSec = 0.234;
   
  bool debug = true;
  double lumi = 5282000;  //lumi only used for debugging
 
  
  //branching ratios
  double hadBR = 0.438;
  double semBR = 0.457;
  double dilBR = 0.105;


  //53X MC
 //  double NGenHad = 31223821;
//   double NGenSem = 25424818;
//   double NGenDil = 12119013;

   //53X MC
  double NGenHad = 30997580;
  double NGenSem = 25165429;
  double NGenDil = 12063533;
   
  //////////////////////////////////////////////////

  TFile *fileHadronic    = new TFile (HadFileName.Data());
  TFile *fileSemiLep     = new TFile (SemFileName.Data());
  TFile *fileDilepton    = new TFile (DilFileName.Data());
  TFile *fileOutput     = new TFile (outputName.Data(), "RECREATE");


  TKey *key = 0;
  TIter nextKey(fileSemiLep->GetListOfKeys());

  int numHistos = 0;

  
  while ( key = (TKey *) nextKey() ) {

  TString histName = key->GetName();
  TString certainName = histName + ";1";

   //For debugging 
  //TString histName = "h_muPt_4j";
  //TString certainName = "h_muPt_4j";

    cout << "================= Histo # " << numHistos << " ============ " << endl;
    
    cout << "Histogram name is " << histName << ", with namecycle "
         << certainName << endl;

    TH1 * HadTemp     = (TH1*) fileHadronic->Get(certainName)->Clone();
    TH1 * SemiLepTemp = (TH1*) fileSemiLep->Get(certainName)->Clone();
    TH1 * DilepTemp   = (TH1*) fileDilepton->Get(certainName)->Clone();

  
    if(debug == true){
      cout << "intial event in template histo " << endl;
      cout << "hadronic events            =     " << HadTemp->GetEntries() << endl;
      cout << "semilep  events            =     " << SemiLepTemp->GetEntries() << endl;
      cout << "dilep    events            =     " << DilepTemp->GetEntries() << endl;
      cout << "intial integral in template histo " << endl;
      cout << "hadronic integral            =     " << HadTemp->Integral() << endl;
      cout << "semilep  integral            =     " << SemiLepTemp->Integral() << endl;
      cout << "dilep    integral            =     " << DilepTemp->Integral() << endl;
    }
    
    HadTemp->Scale((ttXSec * hadBR)/NGenHad);
    if(debug == true){
      cout << "Scale semilep by: (ttXSec x semBR)/(NgenSem)  = (" << ttXSec << " x "
           << semBR << ")/NgenSem = " << endl << "(" <<  ttXSec*semBR << ")/"
           << NGenSem << " = " << ttXSec*semBR/NGenSem << endl;
    }
    SemiLepTemp->Scale((ttXSec * semBR)/NGenSem);
    DilepTemp->Scale((ttXSec * dilBR)/NGenDil);
    

    TH1 * comboTemp = (TH1*) SemiLepTemp->Clone();
    comboTemp->SetDirectory (fileOutput);
    
    comboTemp->Add(HadTemp);
    comboTemp->Add(DilepTemp);

   
    
    cout << "combo num entries       =     " << comboTemp->GetEntries() << endl;
    cout << "final event in template histo " << endl;
    cout << "hadronic events            =     " << HadTemp->GetEntries() << endl;
    cout << "semilep  events            =     " << SemiLepTemp->GetEntries() << endl;
    cout << "dilep    events            =     " << DilepTemp->GetEntries() << endl;
    cout << "final integral in template histo " << endl;
    cout << "hadronic integral            =     " << HadTemp->Integral() << endl;
    cout << "semilep  integral            =     " << SemiLepTemp->Integral() << endl;
    cout << "dilep    integral            =     " << DilepTemp->Integral() << endl;
    cout << "combo    integral        =     " << comboTemp->Integral() << endl;

     if(debug == true){
       cout << "Expected TTbar in " << (lumi/1000000) << " fb^-1: " << comboTemp->Integral()*lumi << endl; 
       cout << "         hadronic : " << HadTemp->Integral()*lumi << endl;
       cout << "         semilept : " << SemiLepTemp->Integral()*lumi << endl;
       cout << "         dilepton : " << DilepTemp->Integral()*lumi << endl; 

     }
    
    numHistos++;

    //clean up
    delete HadTemp;
    delete SemiLepTemp;
    delete DilepTemp;

    }//end while

  cout << "\n\nRan over " << numHistos << ", saving file " << endl;


  cout << "\n\n-------- Instructions ---------" << endl
       << "To use the file " << outputName.Data()  
       << " in the fitter, please enter: "
       << "cross section = 1.0 " << endl
       << "ngen = 1.0\n\n\n" << endl;


  
  fileOutput->Write();
  fileOutput->Close();
  


}
