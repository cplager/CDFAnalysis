#include "TROOT.h"
#include "TStyle.h"
#include "TH1.h"
#include "TFile.h"
#include "TMath.h"

#include <TObject.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include "math.h"
#include <time.h>
#include <fstream>

void makeTable(double lumi= 36.1 ){
  
  ///constants
  //xsec in pb
  double BR = 0.3257;
  
  double xsttbar = 157.5;
  double xsSTopt = 64.6 * BR;
  double xsSToptW = 10.6 * BR;
  double xsWJets = 31314;
  double xsZJets = 3048;
  double xsqcd = 84679.3;
  
  double nGenttbar = 1306182;
  double nGenSTopt = 484060;
  double nGenSToptW = 494961;
  double nGenWJets = 14805546;
  double nGenZJets = 2543727;
  double nGenqcd = 28904866;
  
  double nGenttbar1 = nGenttbar;
  double nGenSTopt1 = nGenSTopt;
  double nGenSToptW1 = nGenSToptW;
  double nGenWJets1 = nGenWJets;
  double nGenZJets1 = nGenZJets;
  double nGenqcd1 = nGenqcd;
  

  ///////////// ///////////////////////
  ///// ///loading nominal samples
  
  std::cout << "Initializing list of nominal NJets histograms..." << std::endl;
  std::vector<TH1 *> histList;
  
  //ttbar sample
  std::cout << "Opening ttbar file..." << std::endl;
  TFile *topFile = TFile::Open("../RooFitter/templates/ttbar/shyftPretag_ttbar_38X.root");
  TH1 *ttbarNJets = (TH1 *)topFile->Get("nJets");
  ttbarNJets->Scale(lumi*xsttbar/nGenttbar);  
  histList.push_back(ttbarNJets);

  //single Top t channel sample
  std::cout << "Opening single Top t channel file..." << std::endl;
  TFile *stFile = TFile::Open("../RooFitter/templates/singletop/shyftPretag_singleTop_t_38X.root");
  TH1 *stNJets = (TH1 *)stFile->Get("nJets");
  stNJets->Scale(lumi*xsSTopt/nGenSTopt);  
  histList.push_back(stNJets);

  //single Top tW channel sample
  std::cout << "Opening single Top tW channel file..." << std::endl;
  TFile *stWFile = TFile::Open("../RooFitter/templates/singletop/shyftPretag_singleTop_tW_38X.root");
  TH1 *stWNJets = (TH1 *)stWFile->Get("nJets");
  stWNJets->Scale(lumi*xsSToptW/nGenSToptW); 
  histList.push_back(stWNJets);

  //wjets sample
  std::cout << "Opening wjets file..." << std::endl;
  TFile *wFile = TFile::Open("../RooFitter/templates/wjets/shyftPretag_wjets_38X.root");
  TH1 *wNJets = (TH1 *)wFile->Get("nJets");
  wNJets->Scale(lumi*xsWJets/nGenWJets);  
  histList.push_back(wNJets);

  //zjets sample
  std::cout << "Opening zjets file..." << std::endl;
  TFile *zFile = TFile::Open("../RooFitter/templates/zjets/shyftPretag_zjets_38X.root");
  TH1 *zNJets = (TH1 *)zFile->Get("nJets");
  zNJets->Scale(lumi*xsZJets/nGenZJets); 
  histList.push_back(zNJets);

  //QCD sample
  std::cout << "Opening QCD file..." << std::endl;
  TFile *qcdFile = TFile::Open("../RooFitter/templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD.root");
  TH1 *qcdNJets = (TH1 *)qcdFile->Get("nJets");
  qcdNJets->Scale(lumi*xsqcd/nGenqcd);
  histList.push_back(qcdNJets);

  //////////////////////////////////////
  std::vector<TH1 *> histList1;

  TString  systLabel = "jes-down";
  
  //ttbar systematic sample
  //  std::cout << "Opening ttbar systematic file..." << std::endl;
  TString filename = "../RooFitter/templates/ttbar/shyftPretag_ttbar_38X";
  filename += "_"+systLabel+".root";
  TFile *topFile1 = TFile::Open(filename);
  TH1 *ttbarNJets1 = (TH1*)topFile1->Get("nJets");
  ttbarNJets1->Scale(lumi*xsttbar/nGenttbar1);
  histList1.push_back(ttbarNJets1);
  
  cout << filename << endl;
  
  //single Top t channel systematic sample
  //  std::cout << "Opening single Top t channel systematic file..." << std::endl;
  filename = "../RooFitter/templates/singletop/shyftPretag_tchan_38X";
  filename += "_"+systLabel+".root";
  TFile *stFile1 = TFile::Open(filename);
  TH1 *stNJets1 = (TH1*)stFile1->Get("nJets");
  stNJets1->Scale(lumi*xsSTopt/nGenSTopt1);
  histList1.push_back(stNJets1);
  
  cout << filename << endl;
  
  //single Top tW channel systematic sample
  //  std::cout << "Opening single Top tW channel systematic file..." << std::endl;
  filename = "../RooFitter/templates/singletop/shyftPretag_tW_38X";
  filename += "_"+systLabel+".root";
  TFile *stWFile1 = TFile::Open(filename);
  TH1 *stWNJets1 = (TH1*)stWFile1->Get("nJets");
  stWNJets1->Scale(lumi*xsSToptW/nGenSToptW1);
  histList1.push_back(stWNJets1);
      
  cout << filename << endl;
  
  //wjets systematic sample
  //  std::cout << "Opening wjets systematic file..." << std::endl;
  filename = "../RooFitter/templates/wjets/shyftPretag_wjets_38X";
  filename += "_"+systLabel+".root";
  TFile *wFile1 = TFile::Open(filename);
  TH1 *wNJets1 = (TH1*)wFile1->Get("nJets")->Clone("wNJets");
  wNJets1->Scale(lumi*xsWJets/nGenWJets1);
  histList1.push_back(wNJets1);
  
  cout << filename << endl;
  
  //zjets systematic sample
  //  std::cout << "Opening zjets systematic file..." << std::endl;
  filename = "../RooFitter/templates/zjets/shyftPretag_zjets_38X";
  filename += "_"+systLabel+".root";
  TFile *zFile1 = TFile::Open(filename);
  TH1 *zNJets1 = (TH1*)zFile1->Get("nJets")->Clone("zNJets");
  zNJets1->Scale(lumi*xsZJets/nGenZJets1);
  histList1.push_back(zNJets1);
  
  cout << filename << endl;
   
  //QCD systematic sample
  //  std::cout << "Opening QCD systematic file..." << std::endl;
  filename = "../RooFitter/templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X";
  //  "../RooFitter/templates/qcd_mc_iso/shyftPretag_qcdIsoMC_38X";
  //  "../RooFitter/templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD";
  filename += "_"+systLabel+"_muon-KITQCD.root";
  TFile *qcdFile1 = TFile::Open(filename);
  TH1 *qcdNJets1 = (TH1*)qcdFile1->Get("nJets")->Clone("qcdNJets");
  qcdNJets1->Scale(lumi*xsqcd/nGenqcd1);
  histList1.push_back(qcdNJets1);
    
  cout << filename << endl;

  /////////////////
  systLabel = "jes-up";
    
  //ttbar systematic sample
  //  std::cout << "Opening ttbar systematic file..." << std::endl;
  filename = "../RooFitter/templates/ttbar/shyftPretag_ttbar_38X";
  filename += "_"+systLabel+".root";
  TFile *topFile2 = TFile::Open(filename);
  TH1 *ttbarNJets2 = (TH1*)topFile2->Get("nJets");
  ttbarNJets2->Scale(lumi*xsttbar/nGenttbar1);
  histList1.push_back(ttbarNJets2);
  
  cout << filename << endl;
  
  //single Top t channel systematic sample
  //  std::cout << "Opening single Top t channel systematic file..." << std::endl;
  filename = "../RooFitter/templates/singletop/shyftPretag_tchan_38X";
  filename += "_"+systLabel+".root";
  TFile *stFile2 = TFile::Open(filename);
  TH1 *stNJets2 = (TH1*)stFile2->Get("nJets");
  stNJets2->Scale(lumi*xsSTopt/nGenSTopt1);
  histList1.push_back(stNJets2);
  
  cout << filename << endl;
  
  //single Top tW channel systematic sample
  //  std::cout << "Opening single Top tW channel systematic file..." << std::endl;
  filename = "../RooFitter/templates/singletop/shyftPretag_tW_38X";
  filename += "_"+systLabel+".root";
  TFile *stWFile2 = TFile::Open(filename);
  TH1 *stWNJets2 = (TH1*)stWFile2->Get("nJets");
  stWNJets2->Scale(lumi*xsSToptW/nGenSToptW1);
  histList1.push_back(stWNJets2);
      
  cout << filename << endl;
   
  //wjets systematic sample
  //  std::cout << "Opening wjets systematic file..." << std::endl;
  filename = "../RooFitter/templates/wjets/shyftPretag_wjets_38X";
  filename += "_"+systLabel+".root";
  TFile *wFile2 = TFile::Open(filename);
  TH1 *wNJets2 = (TH1*)wFile2->Get("nJets");
  wNJets2->Scale(lumi*xsWJets/nGenWJets1);
  histList1.push_back(wNJets2);
  
  cout << filename << endl;

  //zjets systematic sample
  //  std::cout << "Opening zjets systematic file..." << std::endl;
  filename = "../RooFitter/templates/zjets/shyftPretag_zjets_38X";
  filename += "_"+systLabel+".root";
  TFile *zFile2 = TFile::Open(filename);
  TH1 *zNJets2 = (TH1*)zFile2->Get("nJets");
  zNJets2->Scale(lumi*xsZJets/nGenZJets1);
  histList1.push_back(zNJets2);
  
  cout << filename << endl;
  
  //QCD systematic sample
  //  std::cout << "Opening QCD systematic file..." << std::endl;
  filename = "../RooFitter/templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X";
    // "../RooFitter/templates/qcd_mc_iso/shyftPretag_qcdIsoMC_38X";
  //  "../RooFitter/templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD";
  filename += "_"+systLabel+"_muon-KITQCD.root";
  TFile *qcdFile2 = TFile::Open(filename);
  TH1 *qcdNJets2 = (TH1*)qcdFile2->Get("nJets");
  qcdNJets2->Scale(lumi*xsqcd/nGenqcd1);
  histList1.push_back(qcdNJets2);
    
  cout << filename << endl;
  
  //  for(unsigned int perro = 0; perro < histList1.size(); ++perro){
  //   cout << "aca fue " << perro << " " << histList1[perro] << endl;
  //  }
  
  
  //////////// make table one
  std::cout << "-----------------start making table----------------------" <<std::endl;

  ////////// cout the output to a file
  streambuf* coutBuf = cout.rdbuf();
  ofstream of("table_JES.txt");    //----------
  streambuf* fileBuf = of.rdbuf();
  cout.rdbuf(fileBuf);

  /// print some tex content first
  cout << "\\begin{table}" << endl;
  cout << "\\begin{center}" << endl;
  cout << "\\begin{tabular}{|r|l|c|c|c|c|c|c|}" << endl;
  cout << "\\hline" << endl;
  cout << "\\multicolumn{8}{|c|}{JES systematic yield change}" << "\\" << "\\" << endl;
  cout << "\\hline" << endl;
  
  ///////// print the table
  cout << setiosflags(ios::fixed)
       << setprecision(1) 
       << left ;
  
  cout << setw(30) << "Cut" << " & " <<  setw(5) << "sys" << " & " 
       << setw(11) << "$t\\bar{t}$" << " & " 
       << setw(11) << "$t$-chan" << " & " << setw(11) << "$tW$" << " & " 
       << setw(11) << "$W$+Jets" << " & " << setw(11) << "$Z$+Jets" << " & " 
       << setw(11) << "QCD" << "\\" << "\\" << endl;   
  cout << "\\hline" << endl;

  for (int ijet=2;ijet<5;++ijet){

    cout << "\\multirow{2}{*}{" ;
    TString cutname = "";
    if (ijet != 4){
      cutname = "== " ; cutname += ijet ; cutname += " Jets}" ;
    }else {
      cutname = "$\\geq$ " ; cutname += ijet ; cutname += " Jets}" ;
    }
    cout << setw(14) << cutname << " & " ;
    
    cout << setw(5) << "JES -" ;   ////---------------

    int size = histList.size();
    double num = 0;
    double num1 = 0;
    double frac = 0;

    for ( int i = 0; i < size; ++i){
      TH1 * hist = (TH1 *) histList[i];
      TH1 * hist1 = (TH1 *) histList1[i];

      if (ijet != 4 ){
	num = hist->GetBinContent(ijet+1);
	num1 = hist1->GetBinContent(ijet+1);
      }
      else {
	num = hist->GetBinContent(ijet+1)+hist->GetBinContent(ijet+2);   
	num1 = hist1->GetBinContent(ijet+1)+hist1->GetBinContent(ijet+2);  
      }
      
      frac = 100*(num1-num)/num;
      
      if (frac > 0){
	cout << " & " << "+" << setw(5) << frac << setw(5) << "\\%"  ;
      }
      else{
	cout << " & " << setw(6) << frac << setw(5) << "\\%" ;
      }
    }
    
    cout << "\\" << "\\"<< endl;

    /////////////
    cout << setw(30) << " " << " & " ;   
    
    cout << setw(5) <<  "JES +" ;   ////---------------
          
    for ( int i = 0; i < size; ++i){
      TH1 * hista = (TH1 *) histList[i];
      TH1 * hista1 = (TH1 *) histList1[i+size];   ////---------------
      
      if (ijet != 4 ){
	num = hista->GetBinContent(ijet+1);
	num1 = hista1->GetBinContent(ijet+1);
      }
      else {
	num = hista->GetBinContent(ijet+1)+hista->GetBinContent(ijet+2);   
	num1 = hista1->GetBinContent(ijet+1)+hista1->GetBinContent(ijet+2);  
      }
      
      frac = 100*(num1-num)/num;
      
      if (frac > 0){
	cout << " & " << "+" << setw(5) << frac << setw(5) << "\\%"  ;
      }
      else{
	cout << " & " << setw(6) << frac << setw(5) <<"\\%" ;
      }
    }
    
    cout << "\\" << "\\"<< std::endl;
   
    
  } 
  
  ////// print out some tex content again
  cout << "\\hline" << endl;
  cout << "\\end{tabular}" << endl;
  cout << "\\caption{Relative yield variation due to JES shift up/down.}" << endl; 
  cout << "\\label{tab:jes_yield_variations}" << endl;
  cout << "\\end{center}" << endl;
  cout << "\\end{table}" << endl;
 
  ////////////
  of.flush();
  of.close();
  cout.rdbuf(coutBuf);
  
  cout << "Done, all info is in table_JES.txt" << endl;

}


//   std::vector<TString > syslist;
//   TString sys1= "jes-up";
//   TString sys2= "jes-down";
//   syslist.push_back(sys1);
  //  syslist.push_back(sys2);
//   for (int j=0; j<syslist.size();++j){
//     TString  systLabel = syslist[j];

//   TList syslabelList;
//   syslabelList.Add(new TObjString("jes-up"));
//   syslabelList.Add(new TObjString("jes-down"));
  //   TIter nextsys(&syslabelList);
//   TObjString *sysObj = 0;
  //   while ((sysObj = (TObjString *)nextsys())) {
//     TString  systLabel = sysObj->String();
      
