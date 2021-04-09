void mergeTemplates () {

  //////////// Declare Variables ///////////////////////
  
  TString tagging = "_2btag";
  //  TString HadFileName = "sqwatTop_ttbar_had_53X_sn0_2btag.root";
  //TString SemFileName = "sqwatTop_ttbar_semilep_53X_sn0_2btag.root";
  //TString DilFileName = "sqwatTop_ttbar_dilep_53X_sn0_2btag.root";
  TString outputName  = "sqwatTop_wjets_53X_comb"+tagging+".root";
  TString W1FileName = "sqwatTop_w1_53X"+tagging+".root";
  TString W2FileName = "sqwatTop_w2_53X"+tagging+".root";
  TString W3FileName = "sqwatTop_w3_53X"+tagging+".root";
  TString W4FileName = "sqwatTop_w4_53X"+tagging+".root";
  
  //xsec in nb
  //double ttXSec = 0.225;
  double WMCXSec  = 30.400;
  double WNLOXSec = 36.257;  //or is it 37.509?
  double W1XSec   = 5.400;
  double W2XSec   = 1.750;
  double W3XSec   = 0.519;
  double W4XSec   = 0.214;

  //debugging
  bool debug = true;
  double lumi = 5282000;  //lumi only used for debugging
   
  //branching ratios
  double WBR = 1; 
  
 //  double NGenW1 = 23141598;
//   double NGenW2 = 34044921;
//   double NGenW3 = 15539503;
//   double NGenW4 = 13382803;

  double NGenW1 = 23136036;
  double NGenW2 = 34030565;
  double NGenW3 = 15472440;
  double NGenW4 = 13360967;

  
  //////////////////////////////////////////////////

 
  TFile *fileW1    = new TFile (W1FileName.Data());
  TFile *fileW2    = new TFile (W2FileName.Data());
  TFile *fileW3    = new TFile (W3FileName.Data());
  TFile *fileW4    = new TFile (W4FileName.Data());
  TFile *fileOutput      = new TFile (outputName.Data(), "RECREATE");


  TKey *key = 0;
  TIter nextKey(fileW1->GetListOfKeys());

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

    TH1 * W1Temp     = (TH1*) fileW1->Get(certainName)->Clone();
    TH1 * W2Temp     = (TH1*) fileW2->Get(certainName)->Clone();
    TH1 * W3Temp     = (TH1*) fileW3->Get(certainName)->Clone();
    TH1 * W4Temp     = (TH1*) fileW4->Get(certainName)->Clone();
    

  
    if(debug == true){
     
      cout << "intial integral in template histo " << endl;
      cout << "W1 integral            =     " << W1Temp->Integral() << endl;
      cout << "W2 integral            =     " << W2Temp->Integral() << endl;
      cout << "W3 integral            =     " << W3Temp->Integral() << endl;
      cout << "W4 integral            =     " << W4Temp->Integral() << endl;
    }
    
   
    // if(debug == true){
//       cout << "Scale W1 by: (W1XSec) x (WNLOXSec/WMCXsec) x (1/NGenW1)  = (" << W1XSec << ") x ("
//            << semBR << ")/NgenSem = " << endl << "(" <<  ttXSec*semBR << ")/"
//            << WNLOXSec/WMCXSec << ") x  (1/NGenW1) = " << (W1XSec*WBR*WNLOXSec)/(NGenW1*WMCXSec) << endl;
//     }

  W1Temp->Scale((W1XSec*WBR*WNLOXSec)/(NGenW1*WMCXSec));
  W2Temp->Scale((W2XSec*WBR*WNLOXSec)/(NGenW2*WMCXSec));
  W3Temp->Scale((W3XSec*WBR*WNLOXSec)/(NGenW3*WMCXSec));
  W4Temp->Scale((W4XSec*WBR*WNLOXSec)/(NGenW4*WMCXSec));
    

    TH1 * comboTemp = (TH1*) W1Temp->Clone();
    comboTemp->SetDirectory (fileOutput);
    
    comboTemp->Add(W2Temp);
    comboTemp->Add(W3Temp);
    comboTemp->Add(W4Temp);

   
    

     if(debug == true){
       cout << "final integral in template histo " << endl;
       cout << "W1 integral            =     " << W1Temp->Integral() << endl;
       cout << "W2 integral            =     " << W2Temp->Integral() << endl;
       cout << "W3 integral            =     " << W3Temp->Integral() << endl;
       cout << "W4 integral            =     " << W4Temp->Integral() << endl;
       cout << "Expected TTbar in " << (lumi/1000000) << " fb^-1: " << comboTemp->Integral()*lumi << endl; 
       cout << "         W1 : " << W1Temp->Integral()*lumi << endl;
       cout << "         W2 : " << W2Temp->Integral()*lumi << endl;
       cout << "         W3 : " << W3Temp->Integral()*lumi << endl;
       cout << "         W4 : " << W4Temp->Integral()*lumi << endl;

     }
    
    numHistos++;

    //clean up
    delete W1Temp;
    delete W2Temp;
    delete W3Temp;
    delete W4Temp;

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
