#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
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

void makeTable(double lumi=0){

  /////// if you don't give a specific lumi, this code will print out
  //////     the event yield in the templates. 
  /////// If you do give a lumi e.g. 36.1, it will print out 
  //////     the event yields mormalized to this lumi

  //////////////////constants
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

  ///////////// ///////////////////////
  std::cout << "Initializing list of NJets histograms..." << std::endl;
  std::vector<TH1 *> histList;

  //ttbar sample
  std::cout << "Opening ttbar file..." << std::endl;
  TFile *topFile = TFile::Open("../RooFitter/templates/ttbar/shyftPretag_ttbar_38X.root");
  TH1 *ttbarNJets = (TH1 *)topFile->Get("nJets");
  if ( lumi != 0 ){ ttbarNJets->Scale(lumi*xsttbar/nGenttbar);  }
  histList.push_back(ttbarNJets);

  //single Top t channel sample
  std::cout << "Opening single Top t channel file..." << std::endl;
  TFile *stFile = TFile::Open("../RooFitter/templates/singletop/shyftPretag_singleTop_t_38X.root");
  TH1 *stNJets = (TH1 *)stFile->Get("nJets");
  if ( lumi != 0 ){ stNJets->Scale(lumi*xsSTopt/nGenSTopt);  }
  histList.push_back(stNJets);

  //single Top tW channel sample
  std::cout << "Opening single Top tW channel file..." << std::endl;
  TFile *stWFile = TFile::Open("../RooFitter/templates/singletop/shyftPretag_singleTop_tW_38X.root");
  TH1 *stWNJets = (TH1 *)stWFile->Get("nJets");
  if ( lumi != 0 ){ stWNJets->Scale(lumi*xsSToptW/nGenSToptW);  }
  histList.push_back(stWNJets);

  //wjets sample
  std::cout << "Opening wjets file..." << std::endl;
  TFile *wFile = TFile::Open("../RooFitter/templates/wjets/shyftPretag_wjets_38X.root");
  TH1 *wNJets = (TH1 *)wFile->Get("nJets");
  if ( lumi != 0 ){ wNJets->Scale(lumi*xsWJets/nGenWJets);  }
  histList.push_back(wNJets);

  //zjets sample
  std::cout << "Opening zjets file..." << std::endl;
  TFile *zFile = TFile::Open("../RooFitter/templates/zjets/shyftPretag_zjets_38X.root");
  TH1 *zNJets = (TH1 *)zFile->Get("nJets");
  if ( lumi != 0 ){ zNJets->Scale(lumi*xsZJets/nGenZJets);  }
  histList.push_back(zNJets);

  //QCD sample
  std::cout << "Opening QCD file..." << std::endl;
  TFile *qcdFile = TFile::Open("../RooFitter/templates/qcd_mc_iso/shyftPretag_qcdIsoMC_38X.root");
  TH1 *qcdNJets = (TH1 *)qcdFile->Get("nJets");
  qcdNJets->Scale(2);   /// twice the amount of QCD
  if ( lumi != 0 ){ qcdNJets->Scale(lumi*xsqcd/nGenqcd);  }
  histList.push_back(qcdNJets);

  //Data sample
  std::cout << "Opening Data file..." << std::endl;
  TFile *dataFile = TFile::Open("../RooFitter/templates/data/shyftPretag_data_38X.root");
  TH1 *dataNJets = (TH1 *)dataFile->Get("nJets");


  //////////////////////// make the table
  std::cout << "-----------------start making table----------------------" <<std::endl;
  cout << '\n'
       << '\n' ;

  ////////// cout the output to a file
  TString filename = "";
  streambuf* coutBuf = cout.rdbuf();
  if(lumi==0){ filename = "table_EventYield.txt" ; }
  else { filename = "table_EventYield_Norm.txt" ;  }
  ofstream of(filename) ; 
  streambuf* fileBuf = of.rdbuf();
  cout.rdbuf(fileBuf);

  ////////////////////
  cout << setiosflags(ios::fixed)
       << setprecision(0) 
       << left ;
  
  cout << setw(11) << "Cut" << " & " << setw(11) << "$t\\bar{t}$" << " & " 
       << setw(11) << "$t$-chan" << " & " << setw(11) << "$tW$" << " & " 
       << setw(11) << "$W$+Jets" << " & " << setw(11) << "$Z$+Jets" << " & " 
       << setw(11) << "QCD" << " & " << setw(11) << "Sum MC" << " & " << setw(11) << "Data" << "\\" << "\\" << endl; 
  
  for (int ijet=0;ijet<5;++ijet){
    
    TString cutname = "";
    if (ijet < 2){
      cutname = "==" ; cutname += ijet ; cutname += "Jet" ;
    }
    else if (ijet != 4){
      cutname = "==" ; cutname += ijet ; cutname += "Jets" ;
    }else {
      cutname = "$\\geq$" ; cutname += ijet ; cutname += "Jets" ;
    }
    cout << setw(11) << cutname << " & " ;
    
    int sum = 0;
    int num = 0;
    int numData =0;

    for (std::vector<TH1 *>::const_iterator tIter = histList.begin();
	 tIter != histList.end(); ++tIter){
      
      TH1 *hist = *tIter;
    
      if (ijet != 4 ){
	num = hist->GetBinContent(ijet+1);
        numData = dataNJets->GetBinContent(ijet+1);
      }
      else {
	num = hist->GetBinContent(ijet+1)+hist->GetBinContent(ijet+2);      
	numData = dataNJets->GetBinContent(ijet+1) + dataNJets->GetBinContent(ijet+2);
      }
      sum += num;
      cout << setw(11) << num  << " & " ;
      
    }
    
    cout << setw(11) << sum << " & " ;
    cout << setw(11) << numData << " \\" << "\\"<< std::endl;
    
  } 
  
  /////print out current date when the table is made
  struct tm *current;
  time_t now;
  now = time(NULL);
  current = localtime(&now);
  int year = current->tm_year +1900 ;
  int month = current->tm_mon +1 ;
  int day = current->tm_mday ;
  cout << '\n'
       << '\n';
  cout << "this table is made on: " <<  year << "-" << month << "-" << day << endl; 

  ////////////
  of.flush();
  of.close();
  cout.rdbuf(coutBuf);

  cout << "Done, all info is in " << filename << endl;

  
}
