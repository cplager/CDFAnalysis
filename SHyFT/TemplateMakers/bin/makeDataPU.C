

void makeDataPU(TString meanFileName, TString outFileName) {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TFile *meanFile = TFile::Open(meanFileName);
  TH1 *meanHist = (TH1 *)meanFile->Get("pileup")->Clone("meanHist");

  std::cout << "Mean histogram integral:" << meanHist->Integral() << std::endl;

  TFile *mikeOrig = TFile::Open("data_pileup_160404-180252.root");
  TH1 *mikeOrigHist = (TH1 *)mikeOrig->Get("pileup")->Clone("mikeOrigHist");
  mikeOrigHist->SetLineColor(2);

  std::cout << "Original (from Mike) histogram integral:" << mikeOrigHist->Integral() << std::endl;

  //Now we want to integrate this histogram convoluted with the
  //appropriate Poisson
  TH1F *obsHist = new TH1F("obsHist","Pileup Interaction Distribution",50,-0.5,49.5);
  obsHist->SetLineColor(3);

  for (int iBin = 1; iBin <= meanHist->GetNbinsX(); ++iBin) {

    double mean = meanHist->GetBinCenter(iBin);
    double nEntries = meanHist->GetBinContent(iBin);

    if (nEntries > 0) {
      for (int nInt = 0; nInt < 50; ++nInt) {
        obsHist->Fill(nInt,nEntries*TMath::PoissonI(nInt,mean));
      }
    }

  }


  std::cout << "My observed integral:" << obsHist->Integral() << std::endl;

  TFile *outFile = TFile::Open(outFileName,"RECREATE");
  outFile->cd();
  obsHist->Write("pileup");

  TCanvas *canv = new TCanvas("canv","canv",500,500);
  canv->cd();

  mikeOrigHist->Scale(1./mikeOrigHist->Integral());
  obsHist->Scale(1./obsHist->Integral());

  mikeOrigHist->Draw();
  obsHist->Draw("SAME");

  TLegend *leg = new TLegend(0.32,0.8,0.9,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(mikeOrigHist,"Default from Mike","l");
  leg->AddEntry(obsHist,"My version","l");
  leg->Draw();

  
}
    

