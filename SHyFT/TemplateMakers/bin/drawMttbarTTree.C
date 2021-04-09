drawMttbarTTree(){
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TFile * anaFile = new TFile ("ntuple.ttbarFall424.root"); 
  anaFile->cd();
  TTree * anaTree = (TTree *) anaFile->Get("NTuple");
  //   anaFile2->cd();
  //   TTree * anaTree2 = (TTree *) anaFile->Get("NTuple");

// TString outputName      = "mttbarTree_hists.root";
 TString outputName      = "ttbar.histFall424.root";
 TFile * fileOutput          = new TFile (outputName.Data(), "RECREATE");

 bool drawTruth = true;
 // pick if you want to draw or write an outFile
 bool drawNow = false;

 bool reNorm = false;
 double Lumi = 5000;
 double ttbarXsec = 157.5;
 double ttbarNgen = 3701947;
 

 ////---------------------------------------------------------------------------------------

 //Mttbar Truth Distribution
 if(drawTruth ==true){
   TCanvas * cMttbarTrue = new TCanvas ("cMttbarTrue", "Mttbar");

   //specialized binning
   int nBins = 19;
   double binRange[20]= {0, 350, 410, 470, 530, 590, 650, 710, 770, 830, 890, 950, 1010, 1070, 1130, 1190, 1250, 1350, 1500, 3500};
   //must call this h_plot or unfolder gets mad
   TH1F * h_plot    = new TH1F ("h_plot", "True Mttbar", nBins, binRange);
   h_plot->SetLineColor(2);
   h_plot->SetLineWidth(2);
   //anaTree->Draw("truthMtt >> h_plot", "",  "");
   anaTree->Draw("reconMtt >> h_plot", "recon_nJets == 4",  ""); 
   h_plot->Scale(Lumi*ttbarXsec/ttbarNgen);
   if(drawNow == false){
   h_plot->SetDirectory (fileOutput);
   }
   }

    //write the outFile
    if(drawNow == false){
       fileOutput->Write();
       fileOutput->Close();
    }
    
}//end drawMttbarTTree
