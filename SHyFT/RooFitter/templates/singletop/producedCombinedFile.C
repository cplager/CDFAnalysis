void mergeTemplates () {

  //////////// Declare Variables ///////////////////////

  TString Tag = "_2btag";
  TString ToptChanFileName   = "sqwatTop_singleTop_tChan_53X"+Tag+".root";
  TString ToptWChanFileName  = "sqwatTop_singleTop_tWChan_53X"+Tag+".root";
  TString TopsChanFileName   = "sqwatTop_singleTop_sChan_53X"+Tag+".root";
  TString TbartChanFileName  = "sqwatTop_singleTbar_tChan_53X"+Tag+".root";
  TString TbartWChanFileName = "sqwatTop_singleTbar_tWChan_53X"+Tag+".root";
  TString TbarsChanFileName  = "sqwatTop_singleTbar_sChan_53X"+Tag+".root";
  TString outputName         = "sqwatTop_singleTop_t_53X"+Tag+".root";
  
  // xsec in nb
  double T_tChanXsec  = 0.0563;
  double T_tWChanXsec = 0.0111;
  double T_sChanXsec = 0.00379;
  double Tbar_tChanXsec  = 0.0307;
  double Tbar_tWChanXsec = 0.0111;
  double Tbar_sChanXsec = 0.00176;


  // Summer 12 MC Ngen 53X 
  //  double ToptChanNGen   = 3758227;
  //double ToptWChanNGen  = 497658;
  //double TopsChanNGen   = 259961;
  //double TbartChanNGen  = 1935072;
  //double TbartWChanNGen = 493460;
  //double TbarsChanNGen  = 139974; 


  double ToptChanNGen   = 3754544;
  double ToptWChanNGen  = 496918;
  double TopsChanNGen   = 259657;
  double TbartChanNGen  = 1932776;
  double TbartWChanNGen = 492779;
  double TbarsChanNGen  = 139835; 

  //Summer 11 MC for single top has W-> had&lep (don't need BR)
   double branchRatio = 1;
  // double branchRatio = 0.32442;




   
  //////////////////////////////////////////////////


  TFile * fileTopTChan   = new TFile (ToptChanFileName.Data());
  TFile * fileTopTwChan  = new TFile (ToptWChanFileName.Data());
  TFile * fileTopSChan   = new TFile (TopsChanFileName.Data());
  TFile * fileTbarTChan  = new TFile (TbartChanFileName.Data());
  TFile * fileTbarTwChan = new TFile (TbartWChanFileName.Data());
  TFile * fileTbarSChan  = new TFile (TbarsChanFileName.Data());
  TFile * fileOutput     = new TFile (outputName.Data(), "RECREATE");
  

  TKey *key = 0;
  TIter nextKey(fileTopTChan->GetListOfKeys());

  int numHistos = 0;

  
  while ( key = (TKey *) nextKey() ) {

    

    TString histName = key->GetName();
    TString certainName = histName + ";1";

    cout << "================= Histo # " << numHistos << " ============ " << endl;
    
    cout << "Histogram name is " << histName << ", with namecycle "
         << certainName << endl;

    TH1 * ToptTemp   = (TH1*) fileTopTChan->Get(certainName)->Clone();
    TH1 * ToptwTemp  = (TH1*) fileTopTwChan->Get(certainName)->Clone();
    TH1 * TopsTemp   = (TH1*) fileTopSChan->Get(certainName)->Clone();
    TH1 * TbartTemp   = (TH1*) fileTbarTChan->Get(certainName)->Clone();
    TH1 * TbartwTemp  = (TH1*) fileTbarTwChan->Get(certainName)->Clone();
    TH1 * TbarsTemp   = (TH1*) fileTbarSChan->Get(certainName)->Clone();

    cout << "top tw num entries            =     " << ToptwTemp->GetEntries() << endl;
    cout << "top tw num entries            =     " << ToptwTemp->Integral() << endl;
    cout << "top t  num entries            =     " << ToptTemp->GetEntries() << endl;
    cout << "top s  num entries            =     " << TopsTemp->GetEntries() << endl;
    cout << "tbar tw num entries           =     " << TbartwTemp->GetEntries() << endl;
    cout << "tbar t  num entries           =     " << TbartTemp->GetEntries() << endl;
    cout << "tbar s  num entries           =     " << TbarsTemp->GetEntries() << endl;


    ToptwTemp->Scale(branchRatio * T_tWChanXsec / ToptWChanNGen);
    ToptTemp->Scale( branchRatio * T_tChanXsec / ToptChanNGen );
    TopsTemp->Scale( branchRatio * T_sChanXsec / TopsChanNGen );
    TbartwTemp->Scale(branchRatio * Tbar_tWChanXsec / TbartWChanNGen);
    TbartTemp->Scale( branchRatio * Tbar_tChanXsec / TbartChanNGen );
    TbarsTemp->Scale( branchRatio * Tbar_sChanXsec / TbarsChanNGen );


    TH1 * comboTemp = (TH1*) ToptwTemp->Clone();
    comboTemp->SetDirectory (fileOutput);

    
    comboTemp->Add(ToptTemp);
    comboTemp->Add(TopsTemp);
    comboTemp->Add(TbartwTemp);
    comboTemp->Add(TbartTemp);
    comboTemp->Add(TbarsTemp);

    cout << "combo num entries       =     " << comboTemp->GetEntries() << endl;

    
    cout << "top tw num integral            =     " << ToptwTemp->Integral() << endl;
    cout << "top t  num integral            =     " << ToptTemp->Integral() << endl;
    cout << "top s  num integral            =     " << TopsTemp->Integral() << endl;
    cout << "tbar tw num integral           =     " << TbartwTemp->Integral() << endl;
    cout << "tbar t  num integral           =     " << TbartTemp->Integral() << endl;
    cout << "tbar s  num integral           =     " << TbarsTemp->Integral() << endl;
    cout << "combo num integral        =     " << comboTemp->Integral() << endl;

    
    numHistos++;

    //clean up
    delete ToptwTemp;
    delete ToptTemp;
    delete TopsTemp;
    delete TbartwTemp;
    delete TbartTemp;
    delete TbarsTemp;

  }

  cout << "\n\nRan over " << numHistos << ", saving file " << endl;


  cout << "\n\n-------- Instructions ---------" << endl
       << "To use the file " << outputName.Data()  
       << " in the fitter, please enter: " << endl
       << "cross section = 1.0 " << endl
       << "ngen = 1.0\n\n\n" << endl;


  
  fileOutput->Write();
  fileOutput->Close();
  


}
