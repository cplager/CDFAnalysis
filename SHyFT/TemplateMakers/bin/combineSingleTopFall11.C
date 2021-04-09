void combineSingleTopFall11 () {

  //////////// Declare Variables ///////////////////////

  TString ToptChanFileName   = "templatesCondor/merged/shyftPretag_t_tchan.root";
  TString ToptWChanFileName  = "templatesCondor/merged/shyftPretag_t_tWchan.root";
  TString TopsChanFileName   = "templatesCondor/merged/shyftPretag_t_schan.root";
  TString TbartChanFileName  = "templatesCondor/merged/shyftPretag_tbar_tchan.root";
  TString TbartWChanFileName = "templatesCondor/merged/shyftPretag_tbar_tWchan.root";
  TString TbarsChanFileName  = "templatesCondor/merged/shyftPretag_tbar_schan.root";
  TString outputName         = "templatesCondor/merged/shyftPretag_single_top.root";

  // xsec in nb
  double tChanXsec  = 0.063;
  double tWChanXsec = 0.0106;
  double sChanXsec = 0.0046;


  // Fall 11 MC Ngen (after TotalKinFilter): 
  //  https://twiki.cern.ch/twiki/bin/view/CMS/HiggsWWTolnujjCommonPATtuples#Background_Samples
  double ToptChanNGen   = 3891502; 
  double ToptWChanNGen  = 812544;
  double TopsChanNGen   = 259572;
  double TbartChanNGen  = 1939460; 
  double TbartWChanNGen = 322638;
  double TbarsChanNGen  = 137647; 


  //Fall 11 MC for single top has W-> had&lep (don't need BR)
  double branchRatio = 1;
  // double branchRatio = 0.32442;


  //////////////////////////////////////////////////


  TFile * fileTopTChan   = TFile::Open (ToptChanFileName.Data());
  TFile * fileTopTwChan  = TFile::Open (ToptWChanFileName.Data());
  TFile * fileTopSChan   = TFile::Open (TopsChanFileName.Data());
  TFile * fileTbarTChan  = TFile::Open (TbartChanFileName.Data());
  TFile * fileTbarTwChan = TFile::Open (TbartWChanFileName.Data());
  TFile * fileTbarSChan  = TFile::Open (TbarsChanFileName.Data());
  TFile * fileOutput     = TFile::Open (outputName.Data(), "RECREATE");
  

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


    ToptwTemp->Scale(branchRatio * tWChanXsec / ToptWChanNGen);
    ToptTemp->Scale( branchRatio * tChanXsec / ToptChanNGen );
    TopsTemp->Scale( branchRatio * sChanXsec / TopsChanNGen );
    TbartwTemp->Scale(branchRatio * tWChanXsec / TbartWChanNGen);
    TbartTemp->Scale( branchRatio * tChanXsec / TbartChanNGen );
    TbarsTemp->Scale( branchRatio * sChanXsec / TbarsChanNGen );


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
