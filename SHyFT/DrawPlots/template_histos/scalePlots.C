

void scalePlots (const char * fname) {


  TFile *myFile = new  TFile(fname);

  TFile *output = new TFile ("scaled.root", "RECREATE");


    //First step is to figure out which plots to make.  Make a list of
   //all histograms, and just keep the ones have "_data" in them


  TKey *key = 0;  TIter nextKey(myFile->GetListOfKeys());
  TList plotList;
  //int iSig = -1;


    while (key = (TKey *)nextKey()) {
      TString histName = key->GetName();

      if (histName.EndsWith("j")) {
        TH1 * myClone = (TH1*) key->ReadObj();
        myClone->Sumw2();
        if (histName.EndsWith("0j")){
          myClone->Scale(10.42);
        } else if (histName.EndsWith("1j") ) {
          myClone->Scale(10.44);
        } else if (histName.EndsWith("2j") ) {
          myClone->Scale(10.74);
        } else if (histName.EndsWith("3j") ) {
          myClone->Scale(10.64);
        } else if (histName.EndsWith("4j") ) {
          myClone->Scale(11.69);
        } else if (histName.EndsWith("5j") ) {
          myClone->Scale(12.17);
        }
                              
        myClone->SetDirectory(output);        
      }
        

    }

    output->Write();
    output->Close();
    

  

}
