//to run in root: .L swapQCDShape.C
//swapQCDShape()
void swapQCDShape(){

  TString QCDtemplateFileName = "~/scratch0/SHyFT/CMSSW_4_2_4/src/SHyFT/RooFitter/templates/qcd_iso/sqwatTop_qcd_42X_2btag.root";
  TString QCDshapeFileName    = "~/scratch0/SHyFT/CMSSW_4_2_4/src/SHyFT/RooFitter/templates/qcd_shape/sqwatTop_nonIsoData_42X_2btag.root";
  TString outputName          = "sqwatTop_qcd_Model_42X_2btag.root";

  TFile *fileTemplate  = new TFile(QCDtemplateFileName.Data());
  TFile *fileShape     = new TFile(QCDshapeFileName.Data());
  TFile * fileOutput     = new TFile (outputName.Data(), "RECREATE");


  TKey *key = 0;
  TIter nextKey(fileTemplate->GetListOfKeys());

  int numHistos = 0;
  double tempNorm = 0;
  double shapeNorm = 0;
  
   while ( key = (TKey *) nextKey() ) {

    TString histName = key->GetName();
    TString certainName = histName + ";";

    cout << "================= Histo # " << numHistos << " ============ " << endl;
    
    cout << "Histogram name is " << histName << ", with namecycle "
         << certainName << endl;
 
    TH1 * tempHist  = (TH1*)   fileTemplate->Get(certainName)->Clone();
    tempNorm = tempHist->Integral();
    cout << " template norm: " <<  tempNorm << endl;
    
    TH1 * modelHist = (TH1*)   fileShape->Get(certainName)->Clone();
    shapeNorm = modelHist->Integral();
    cout << " original shape norm: " <<  modelHist->Integral() << endl;

    //Rescale the shape to the template normalization
    modelHist->Scale(tempNorm/shapeNorm);
    cout << " QCD Model norm: " <<  modelHist->Integral() << endl;
 
    
    
    modelHist->SetDirectory (fileOutput);

    delete tempHist;

    
    shapeNorm = 0;
    tempNorm = 0;
    numHistos += 1;
   }//end while loop over keys

   cout << "\n\n-------- Instructions ---------" << endl
        << "To use the file " << outputName.Data()  
        << " in the fitter or plot drawer " <<endl
        << " use xsec and Ngen from the original template " << endl;
      

 fileOutput->Write();
 fileOutput->Close();


} //end swapQCDShape
