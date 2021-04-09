{
  float Nplots = 3;
  TH1F* hwbb = new TH1F("hwbb", "KIT Flavor Separator", 40, -1, 1);
  TH1F* hwcc = new TH1F("hwcc", "KIT Flavor Separator", 40, -1, 1);
  TH1F* hwlf = new TH1F("hwlf", "KIT Flavor Separator", 40, -1, 1);

  hwbb ->SetDirectory(0);

  TFile* fwbb = new TFile("wbb.root");
  TFile* fwcc = new TFile("wcc.root");
  TFile* fwlf = new TFile("wlf.root");
  TTree* treewbb;
  TTree* treewcc;
  TTree* treewlf;
  fwbb.GetObject("nt",treewbb);
  fwcc.GetObject("nt",treewcc);
  fwlf.GetObject("nt",treewlf);

  TCanvas* c1 = new TCanvas("c1","KIT",900,600);
  //c1->SetLogy(1);
  treewbb->Draw("bjet1.KaNNcorr>>hwbb", "bjet1.KaNNcorr > -999");
  treewcc->Draw("bjet1.KaNNcorr>>hwcc", "bjet1.KaNNcorr > -999");
  treewlf->Draw("bjet1.KaNNcorr>>hwlf", "bjet1.KaNNcorr > -999");

  hwbb = (TH1F*)gDirectory ->Get("hwbb");
  hwbb ->GetXaxis()->SetTitle("KIT Flavor Separator");
  hwbb ->GetYaxis()->SetTitle("Events / 0.1");
  hwbb ->SetLineColor(kRed);
  hwbb ->SetLineWidth(2);
  hwbb ->SetStats(0);
  hwbb ->SetTitle("KIT");

  hwcc = (TH1F*)gDirectory ->Get("hwcc");
  hwcc ->SetLineColor(kBlue);
  hwcc ->SetLineWidth(2);
  hwcc ->SetStats(0);

  hwlf = (TH1F*)gDirectory ->Get("hwlf");
  hwlf ->SetLineColor(kGreen);
  hwlf ->SetLineWidth(2);
  hwlf ->SetStats(0);

  hwbb ->Scale(hwlf ->GetEntries() / hwbb ->GetEntries());
  hwcc ->Scale(hwlf ->GetEntries() / hwcc ->GetEntries());

  hwbb ->Draw();
  hwcc ->Draw("same");
  hwlf ->Draw("same");

  float legSize = 1 - Nplots*0.05;
  TLegend *leg = new TLegend(0.70, legSize, 0.85, 0.995); 
  leg->AddEntry(hwbb, "W + b#bar{b}", "pl");
  leg->AddEntry(hwcc, "W + c#bar{c}", "pl");
  leg->AddEntry(hwlf, "W + q#bar{q}", "pl");

  leg->SetY1(1-Nplots*0.05);
  leg->SetBorderSize(1);
  leg->SetFillColor(10);
  leg->Draw();
  
  c1 ->SaveAs("KIT_separator.eps");
  c1 ->SaveAs("KIT_separator.png");
}
