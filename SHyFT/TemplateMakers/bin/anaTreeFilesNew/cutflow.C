//----------------------------------------------------------------------------
//
// This Macro will take an arbitray number of samples, for an arbitrary number
//  of cuts, make histograms at each cut stage, as well as create twiki and
//  laTeX formatted cutflow tables estimating the number of events for a 
//  user defined integrated luminosity.  The variables to make cuts and 
//  histograms must be defined in the anaTrees that this code runs over.  
//
//
// To Run, with cmsenv, in the same directory as this file and the anaTrees:
//  root -l
//  .L cutflow.C++
//  eventyield(IntLumi, bool anaTree is from muSkim, bool Sequential Cuts)
//
//----------------------------------------------------------------------------


#include <cmath>      //necessary for absolute function fabs()     
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include "math.h"
#include <fstream>
#include <stdio.h>

//Root includes                                   
#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include "TAxis.h"

//--------------------------------------------------------------------------
// Classes

//Utility class for managing binning information
class sampleInfo {
  
public:
  TString name;
  TString twikiN;
  TString fname;
  TString ofname;
  double xs;
  double nGen;
   
  sampleInfo() : name(""), twikiN(""), fname(""), ofname(""), xs(0), nGen(0) {}
  
  sampleInfo(TString bName, TString tName, TString filename, TString outFilename, double xsMC, double num_gen) :
    name (bName), twikiN(tName), fname(filename), ofname(outFilename), xs(xsMC), nGen(num_gen) {}
  
};

// Utility Class for Handling Variable info
class varInfo {

public:
  TString var;
  TString hName;
  TString hTitle;
  int nBins;
  int min;
  int max;
  
  varInfo(): var(""), hName(""), hTitle(""), nBins(0), min(0), max(0) {}

  varInfo(TString varName, TString histName, TString histTitle, int numBins, int binMin, int binMax) : 
  var(varName), hName(histName), hTitle(histTitle), nBins(numBins), min(binMin), max(binMax){}

};

// Utility Class for Handling Cut info
class cutInfo {

public:
  TString humName;
  TString laName;
  TString twikName;
  TString cut;
    
  cutInfo(): humName(""), laName(""), twikName(""), cut("") {}

  cutInfo(TString humanName, TString latexName, TString twikiName, TString cutType):
    humName(humanName), laName(latexName), twikName(twikiName), cut(cutType){}

};



//----------------------------------------------------------------------------
// Functions
//
//
//   addHists:    This function adds histograms weighted by xs, 
//                and rescales to sum addList hists entries,
//                which is sum of events that pass that cut.
//
//   fom25:       This returns a figure of merit, using Mass of WW system
//                = sqrt( sumOverBins( S^2/(B+S+delS^2+delB^2) ) )
//
//   printTable:  This function prints, to file, the cutflow table for each 
//                signal sample, at each cut, in laTeX format
//
//   twikiTable:  This function prints, to file, the cutflow table for each
//                signal sample, at each cut, in twiki format
//
//   dataTable:   This function prints the cutflow table for data only
//
//   efficiencyTable:  This function plots the efficiency of each cut
//                     relative to the efficiency of the first cut,
//                     which is typically set to include all events in
//                     the anaTree
//
//----------------------------------------------------------------------------


void addHists(std::vector<sampleInfo*> addList, sampleInfo* dest, std::vector<varInfo*> varList, TString cutNum, bool isMuon, bool cutSeq){

  // Setup Output file
  TString ofName = dest->ofname;
  ofName += "_";
  ofName += cutNum;
  if(!cutSeq) ofName += "_NonSeq";
  if( isMuon ) ofName += ".root";
  else ofName += "_ele.root";
  TFile *outFile = new TFile(ofName, "RECREATE");

  // Loop over variables to plot
  for(std::vector<varInfo*>::const_iterator tIter1 = varList.begin(); 
      tIter1 != varList.end(); tIter1++){
     
    // Pass a pointer to variable info
    const varInfo *v = *tIter1;

    // Declare new Hist and set output directory
    TH1F *newHist = new TH1F(v->hName, v->hTitle, v->nBins, v->min, v->max);
    newHist->SetDirectory(outFile);

    // Loop over samples to grab histogram for this variable
    for(std::vector<sampleInfo*>::const_iterator tIter2 = addList.begin(); 
        tIter2 !=addList.end(); tIter2++){
  
      // Pass a pointer to call sample info
      const sampleInfo *t1 = *tIter2;
      
      // Get file info about where the hists are stored (output file) 
      TString fnameTemp = t1->ofname;
      fnameTemp += "_";
      fnameTemp += cutNum;
      if (!cutSeq) fnameTemp += "_NonSeq";
      if (isMuon) fnameTemp += ".root" ;
      else fnameTemp += "_ele.root" ;
      TFile *f = new TFile(fnameTemp);
      
      // Grab hist from file
      TH1F *hist = (TH1F*)f->Get(v->hName);      
       
      // Scale histogram by efficiency and xs
      hist->Scale(t1->xs/t1->nGen);

      // Add Scaled histogram to the running sum
      newHist->Add( hist, 1 );

      // Close output file
      f->Close();
      
    } // end sample list loop
      
    // Set histogram name
    newHist->SetName(v->hName); 
   
    // Write summed hist to output file
    outFile->Write();    

  } //end varList loop

  // Close output file
  outFile->Close();

  return;
}


//---------------------------------------------------------------------------


double fom25(const sampleInfo *sig, sampleInfo* allBg, varInfo* Mlvjj, TString cutNum, bool isMuon, bool cutSeq){

  // fom25 = sqrt( sumOverBins( S^2/(S+B+delS^2+delB^2) ) )
  double fom25=0;
  
  // Get Signal File containing histograms
  TString sigFName = sig->ofname;
  sigFName += "_";
  sigFName += cutNum;
  if( !cutSeq ) sigFName += "_NonSeq";
  if( isMuon ) sigFName += ".root";
  else sigFName += "_ele.root";
  
  // Open file and grab histogram of Mass of WW system
  TFile *sigF = new TFile(sigFName);
  TH1F *sigHist = (TH1F*) sigF->Get(Mlvjj->hName);

  // Get Sum of bg file
  TString bgFName = allBg->ofname;
  bgFName += "_";
  bgFName += cutNum;
  if( !cutSeq ) bgFName += "_NonSeq";
  if( isMuon ) bgFName += ".root";
  else bgFName += "_ele.root";
  
  // Open file and grab histogram of Mass of WW system
  TFile *bgF = new TFile(bgFName);
  TH1F *bgHist = (TH1F*) bgF->Get(Mlvjj->hName);

  // Loop over bins to compute fom25
  double sigVal=0, sigErr=0, bgVal=0, bgErr=0;
  for(int i=1; i<= Mlvjj->nBins; i++){
    sigVal = sigHist->GetBinContent(i);
    sigVal *= sig->xs/sig->nGen;
    sigErr = sigHist->GetBinError(i);
    sigErr *= sig->xs/sig->nGen;
 
    bgVal  = bgHist->GetBinContent(i);
    bgVal  *= allBg->xs/allBg->nGen;
    bgErr  = bgHist->GetBinError(i);
    bgErr *= allBg->xs/allBg->nGen;    

    if((sigVal!=0) && (bgVal!=0) )fom25 += (pow(sigVal,2)/(sigVal + bgVal + pow(sigErr,2) + pow(bgErr,2) ));
  }
  
  // Complete calculation
  double fom25final = sqrt(fom25);

  // Close files
  sigF->Close();
  bgF->Close();

  return fom25final;
}


//--------------------------------------------------------------------------


void printTable(std::vector<sampleInfo*> sigList, std::vector<sampleInfo*> bgList, sampleInfo* allBg, std::vector<varInfo*> varList, varInfo* Mlvjj, std::vector<cutInfo*> cutList, double lumi, bool isMuon, bool cutSeq){
   
  int sigNum=0;

  FILE *file;
  TString fString = "HtoWWtolvjjCutFlow";
  if( !cutSeq ) fString += "_NonSeq";
  if( isMuon) fString += ".tex";
  else fString += "_ele.tex";
  file = fopen(fString, "w");

  // Print Beginning of laTeX document
  fprintf(file, "\\documentclass{article} \n");
  fprintf(file, "\\usepackage{chngpage} \n");
  fprintf(file, "\n \n \\begin{document} \n \n");
 
  // Section Header
  fprintf(file, "\\section{Estimated Event Yields for %.1f pb$^-$$^1$} \n \n \n", lumi);
  
  // Loop over signals
  for (std::vector<sampleInfo *>::const_iterator tIter1 = sigList.begin();
       tIter1 != sigList.end(); ++tIter1) {

    const sampleInfo *sig = *tIter1; 
   
    // Print Header of Cut Flow Table
    fprintf(file, "\\begin{table}[ht] \n");
    fprintf(file, "\\begin{adjustwidth}{-1.7in}{-1.7in} \n");
    fprintf(file, "  \\centering \n");
    fprintf(file, "  \\noindent \n");
    fprintf(file, "  \\small \n");
    fprintf(file, "    \\caption{Cut Flow Table for H$\\rightarrow$WW$\\rightarrow$ljvv} \n");
    fprintf(file, "    \\label{tab:Table %d} \n", sigNum);
    fprintf(file, "    \\begin{tabular}{|l|c|");
    for(unsigned int i=0; i< bgList.size(); i++) fprintf(file, "c|");
    fprintf(file, "c|c|c|} \\hline \n");
    fprintf(file, "Cut & %s", sig->name.Data());
    for (std::vector<sampleInfo *>::const_iterator tIter2 = bgList.begin(); 
         tIter2 != bgList.end(); ++tIter2) {
       
      // Pointer to grab sample names
      const sampleInfo *t2 = *tIter2;
      TString sampleName = t2->name;
      fprintf(file, " & %s", sampleName.Data());
     
    } 
    fprintf(file, " & Sum B & S/$\\sqrt{B}$ & ${FOM}_{2.5}$");
    fprintf(file, " \\");
    fprintf(file, "\\");
    fprintf(file, " \\hline \n \n");

    // Loop over cuts
    int num=0;
    for (std::vector<cutInfo*>::const_iterator cutIter = cutList.begin(); 
         cutIter != cutList.end(); ++cutIter) {
      // Pass pointer to cut Info
      const cutInfo *cI = *cutIter;

      // TString for cut number
      TString cutNum = "cut"; 
      cutNum += num;

      // Counters to sum background yield and errors
      double sumBgYield=0;
      
      // Get Signal File containing histograms
      TString sigFName = sig->ofname;
      sigFName += "_";
      sigFName += cutNum;
      if( !cutSeq ) sigFName += "_NonSeq";
      if( isMuon ) sigFName += ".root";
      else sigFName += "_ele.root";
      double sigXs = sig->xs;
      double sigNGen = sig->nGen;

      // Open file, grab histogram
      TFile *sigF = new TFile(sigFName);
      TH1F *sigHist = (TH1F*) sigF->Get(varList[0]->hName);
        
      // Get number of events in histogram, calculate expected yield
      double sigEv = sigHist->Integral();
      double sigYield = sigEv*lumi*sigXs/sigNGen;
      
      //Make sure table is laTeX compatible
      fprintf(file,"%s & %.2f", cI->laName.Data(), sigYield);

      // Close signal file
      sigF->Close();

      // Loop of samples for this cut to get their yields
      for (std::vector<sampleInfo *>::const_iterator tIter3 = bgList.begin(); 
           tIter3 != bgList.end(); ++tIter3) {
    
        // Pass pointer to call sample info
        const sampleInfo *t3 = *tIter3;
    
        // Get files containing histograms of bg sample
        TString fnameTemp = t3->ofname;
        fnameTemp += "_";
        fnameTemp += cutNum;
        if (!cutSeq) fnameTemp += "_NonSeq";
        if (isMuon) fnameTemp += ".root" ;
        else fnameTemp += "_ele.root" ;
        double xsTemp = t3->xs;
        double nGenTemp = t3->nGen;
     
        // Open bg file and grab hist
        TFile *f = new TFile(fnameTemp);
        TH1F *hist = (TH1F*) f->Get(varList[0]->hName);
        
        // Get number of events and calc expected yield
        double numEv = hist->Integral();
        double yield = numEv*lumi*xsTemp/nGenTemp;
        sumBgYield += yield;
      
        // LayTeX formatting
        fprintf(file, " & %.2f", yield);
       
        // Close file
        f->Close();  

      }  // end bg loop

      // Print Sum of Backgrounds
      fprintf(file, " & %.2f", sumBgYield);
      
      // Pring S/sqrt(B)
      double SoverRootB = sigYield/sqrt(sumBgYield);
      fprintf(file, " & %.2f", SoverRootB);
      
      // Calc fom25, figure of merit for WW system mass
      double fom = fom25(sig, allBg, Mlvjj, cutNum, isMuon, cutSeq);
      fprintf(file, " & %.2f", fom);
   
      // Print end of line laTeX 
      fprintf(file, " \\");
      fprintf(file, "\\");
      fprintf(file, " \\hline \n \n \n");
      
      // Increment cut counter
      num++;
      
    } // end cut loop

    // Format end of table
    fprintf(file, "    \\end{tabular} \n");
    fprintf(file, "  \\end{adjustwidth} \n");
    fprintf(file, "\\end{table} \n \n \n");
    
 
    // Create Space between tables
    fprintf(file, "\n \n \n \n \n");

    // Increment table number counter
    sigNum++;

  } // end signal loop

  // Print end of laTeX document
  fprintf(file, "\\end{document} \n \n");

  // Close output file
  fclose(file);

  // Tell user which file was output
  cout << "LaTeX Cutflow table written to: " << fString << endl;

  return;
}


//---------------------------------------------------------------------------


void twikiTable(std::vector<sampleInfo*> twikiList, std::vector<varInfo*> varList, std::vector<cutInfo*> cutList, double lumi, bool isMuon, bool cutSeq){
   
  // Open file to write Twiki cut flow
  FILE *file;
  TString fString = "twikiCutFlow";
  if( !cutSeq ) fString += "_NonSeq";
  if( isMuon) fString += ".txt";
  else fString += "_ele.txt";
  file = fopen(fString, "w");

  // Section Header
  fprintf(file, "| "); 
  if( isMuon) fprintf(file, "*muons*");
  else fprintf(file, "*electrons*");
  
  // Loop over signals to create header of twiki table
  for (std::vector<sampleInfo *>::const_iterator tIter1 = twikiList.begin();
       tIter1 != twikiList.end(); ++tIter1) {

    const sampleInfo *samp = *tIter1; 
    fprintf(file, " | %s", samp->twikiN.Data());
 
  }
  fprintf(file, " | *S/sqrt(B)* | \n");

  
  // Loop over cuts
  int num=0;  
  for (std::vector<cutInfo*>::const_iterator cutIter = cutList.begin(); 
       cutIter != cutList.end(); ++cutIter) {
    // Pass pointer to cut Info
    const cutInfo *cI = *cutIter;
    
    // Print Cut Description 
    fprintf(file, "| %s | ", cI->twikName.Data() );

    // Counters to sum background yield and errors
    double sigYield=0;
    double allBgYield=0;      

    // Loop over samples to compute event yield
    for (std::vector<sampleInfo *>::const_iterator tIter2 = twikiList.begin(); 
         tIter2 != twikiList.end(); ++tIter2) {
       
      // Pass pointer to sample info
      const sampleInfo *t2 = *tIter2;

      // TString for cut number
      TString cutNum = "cut"; 
      cutNum += num;

      // Get files containing histograms 
      TString fnameTemp = t2->ofname;
      fnameTemp += "_";
      fnameTemp += cutNum;
      if (!cutSeq) fnameTemp += "_NonSeq";
      if (isMuon) fnameTemp += ".root" ;
      else fnameTemp += "_ele.root" ;
      double xsTemp = t2->xs;
      double nGenTemp = t2->nGen;
     
      // Open sample file and grab hist
      TFile *f = new TFile(fnameTemp);
      TH1F *hist = (TH1F*) f->Get(varList[0]->hName);
        
      // Get number of events and calc expected yield
      double numEv = hist->Integral();
      double yield = numEv*lumi*xsTemp/nGenTemp;
      if( t2->twikiN == "*H(500)*" ) sigYield = yield;            
      if( t2->twikiN == "*B*" ) allBgYield = yield;

      // Twiki formatting
      fprintf(file, "%.2f | ", yield);
       
      // Close file
      f->Close();  

    }  // sampleList loop

    // Print S/sqrt(B)
    double SoverRootB = sigYield/sqrt(allBgYield);
    fprintf(file, "%.2f | \n", SoverRootB);
      
    // Increment cut number
    num++;

  } // end cut loop

  // Close output file
  fclose(file);

  // Tell user which file was output
  cout << "Twiki cutflow table written to: " << fString << endl;

  return;
}


//----------------------------------------------------------------------------


void dataTable(sampleInfo *data, std::vector<varInfo*> varList, std::vector<cutInfo*> cutList, bool isMuon, bool cutSeq){

  //Open file to write to
  FILE *file;
  TString fString = "dataCutFlow";
  if( !cutSeq) fString += "_NonSeq";
  if( isMuon) fString += ".tex";
  else fString += "_ele.tex";
  file = fopen(fString, "w");

  // Print Beginning of laTeX document
  fprintf(file, "\\documentclass{article} \n");
  fprintf(file, "\\usepackage{chngpage} \n");
  fprintf(file, "\n \n \\begin{document} \n \n");
  
  // Section Header
  fprintf(file, "\\section{Event Yields for Data} \n \n \n");

  // Print Header of Cut Flow Table
  fprintf(file, "\\begin{table}[ht] \n");
  fprintf(file, "\\begin{adjustwidth}{-1.5in}{-1.5in} \n");
  fprintf(file, "  \\centering \n");
  fprintf(file, "  \\noindent \n");
  fprintf(file, "  \\small \n");
  fprintf(file, "    \\caption{Cut Flow Table for H$\\rightarrow$WW$\\rightarrow$ljvv Data} \n");
  fprintf(file, "    \\label{tab:Table 1} \n");
  fprintf(file, "    \\begin{tabular}{|l|c|} \\hline \n");
  fprintf(file, "    Cut Description & Event Yield");
  fprintf(file, " \\");
  fprintf(file, "\\");
  fprintf(file, " \\hline \n \n");

  // Loop over cuts
  int num=0;  
  for (std::vector<cutInfo*>::const_iterator cutIter = cutList.begin(); 
       cutIter != cutList.end(); ++cutIter) {
    // Pass pointer to cut Info
    const cutInfo *cI = *cutIter;

    // Get files containing histograms of data sample
    TString fnameTemp = data->ofname;
    fnameTemp += "_cut";
    fnameTemp += num;
    if (!cutSeq) fnameTemp += "_NonSeq";
    if (isMuon) fnameTemp += ".root" ;
    else fnameTemp += "_ele.root" ;
  
    // Open bg file and grab hist
    TFile *f = new TFile(fnameTemp);
    TH1F *hist = (TH1F*) f->Get(varList[0]->hName);
        
    // Get number of events and calc expected yield
    double numEv = hist->GetEntries();
          
    // LayTeX formatting
    fprintf(file,"%s & %.2f", cI->laName.Data(), numEv);
   
    // Create space between cuts in table
    fprintf(file, " \\");
    fprintf(file, "\\");
    fprintf(file, " \\hline \n \n");  

    // Close dataFile
    f->Close();

    // Increment cut counter
    num++;

  } // end cut list

  // Format end of table
  fprintf(file, "    \\end{tabular} \n");
  fprintf(file, "  \\end{adjustwidth} \n");
  fprintf(file, "\\end{table} \n \n \n");
    
 
  // Create Space between tables
  fprintf(file, "\n \n \n \n \n");

  // End laTex Document
  fprintf(file, "\\end{document}");

  // Close output file
  fclose(file);

  // Tell user which file was output
  cout << "LaTeX cutflow table for data written to: " << fString << endl;

  return;
}


//---------------------------------------------------------------------------

void efficiencyTable(std::vector<sampleInfo*> sampleList, std::vector<varInfo*> varList, std::vector<cutInfo*> cutList, bool isMuon, bool cutSeq){

  // Create new root file to store efficiency plots
  TString outFile = "cutEfficiencies";
  if( !cutSeq ) outFile += "_NonSeq";
  if( isMuon ) outFile += ".root";
  else outFile += "_ele.root";
  TFile *effFile = new TFile(outFile, "RECREATE");

  for (std::vector<sampleInfo *>::const_iterator tIter1 = sampleList.begin(); 
         tIter1 != sampleList.end(); ++tIter1) {
       
    // Pass pointer to sample info
    const sampleInfo *t1 = *tIter1;

    // Create a histogram to store cut efficiency
    TString histName = t1->ofname;
    histName.Remove(0,15);
    TH1F *cutEffHist = new TH1F(histName, histName, cutList.size(), 0, cutList.size() );
    cutEffHist->SetDirectory(effFile);

    double base=0;

    // Loop over cuts
    int num=0;  
    for (std::vector<cutInfo*>::const_iterator cutIter = cutList.begin(); 
         cutIter != cutList.end(); ++cutIter) {
   
      // Pass pointer to cut Info
      const cutInfo *cI = *cutIter;

      // TString for cut number
      TString cutNum = "cut"; 
      cutNum += num;

      // Get files containing histograms 
      TString fnameTemp = t1->ofname;
      fnameTemp += "_";
      fnameTemp += cutNum;
      if (!cutSeq) fnameTemp += "_NonSeq";
      if (isMuon) fnameTemp += ".root" ;
      else fnameTemp += "_ele.root" ;
          
      // Open sample file and grab hist
      TFile *f = new TFile(fnameTemp);
      TH1F *hist = (TH1F*) f->Get(varList[0]->hName);

      // Label bin with cut description
      TAxis *xAx = (TAxis*)cutEffHist->GetXaxis();
      xAx->SetBinLabel(num+1, cI->humName.Data() );
      
      // Get number of events and calc cut efficiency
      double numEv = hist->Integral();
      if(num==0) base = numEv;
      double efficiency = numEv/base;

      // Fill Hist
      cutEffHist->Fill(cI->humName.Data(), efficiency);
  
      // Close Input File
      f->Close();

      // Increment cut counter
      num++;

    } // end cut loop
    
  } // end sample loop

  // Write and Close file
  effFile->Write();
  effFile->Close();

  // Describe output to user
  cout << "CutFlow efficiency histrograms stored in: " << outFile << endl;

  return;
}


//---------------------------------------------------------------------------
//
// Main
//
//---------------------------------------------------------------------------

int eventyield(double lumi, bool isMuon, bool cutSeq) {
  
  
  // Declare vector to store variables
  std::vector<varInfo*> varList;
  
  // Declare Variable info and fill varList vector
  //  Make sure first variable declared has all events, if it gets
  //  Divided between Barrel and Endcap, or has events in the 
  //  under/overflow bins, this will give inaccurate estimates in the 
  //  yield table

  // Variables for either muons or electrons
  varInfo *nJets = new varInfo("nJets", "nJets", "Number of Jets", 8, 0, 8);
  varList.push_back(nJets);
  
  varInfo *jet1Eta = new varInfo("jet1Eta", "jet1Eta", "1st leading jet #eta", 50, -3.1416, 3.1416);
  varList.push_back(jet1Eta);

  varInfo *jet1Pt = new varInfo("jet1Pt", "jet1Pt", "1st leading jet pT", 300, 0, 300);
  varList.push_back(jet1Pt);

  varInfo *jet1discrSSV = new varInfo("jet1discrSSV", "jet1discrSSV", "1st leading jet b-tag discriminant (SSV)", 60, -2, 4);
  varList.push_back(jet1discrSSV);

  varInfo *jet2Eta = new varInfo("jet2Eta", "jet2Eta", "2nd leading jet #eta", 50, -3.1416, 3.1416);
  varList.push_back(jet2Eta);

  varInfo *jet2Pt = new varInfo("jet2Pt", "jet2Pt", "2nd leading jet pT", 200, 0, 200);
  varList.push_back(jet2Pt);

  varInfo *jet2discrSSV = new varInfo("jet2discrSSV", "jet2discrSSV", "2nd leading jet b-tag discriminant (SSV)", 60, -2, 4);
  varList.push_back(jet2discrSSV);

  varInfo *dEtaJetJet_an = new varInfo("dEtaJetJet_an", "dEtaJetJet_an", "delta #eta between 1st and 2nd jet", 100, 0, 5);
  varList.push_back(dEtaJetJet_an);

  varInfo *met = new varInfo("met", "met", "missing transverse energy", 200, 0, 200);
  varList.push_back(met);

  varInfo *mT = new varInfo("mT", "mT", "transverse mass of lv system", 200, 0, 200);
  varList.push_back(mT);

  varInfo *m2 = new varInfo("m2_an", "m2_an", "dijet mass of hadronic system", 200, 0, 800);
  varList.push_back(m2);

  varInfo *WjjPt = new varInfo("WjjPt", "WjjPt", "pT of Hadronic W", 300, 0, 300);
  varList.push_back(WjjPt);

  varInfo *ht = new varInfo("ht", "ht", "ht = pT(j1)+pT(j2)+met+pT(lep)", 600, 0, 1200);
  varList.push_back(ht);

  varInfo *jacksonAngle = new varInfo("jacksonAngle", "jacksonAngle", "CosTheta between leading jet in lab frame and dijet CM rest frame", 50, -1, 1);
  varList.push_back(jacksonAngle);

  varInfo *CosTheta_WH = new varInfo("CosTheta_WH", "CosTheta_WH", "CosTheta between leptonic W and H in Higgs rest frame", 50, -1, 1);
  varList.push_back(CosTheta_WH);

  varInfo *CosTheta_j = new varInfo("CosTheta_j", "CosTheta_j", "CosTheta between 1st leading jet (from Higgs frame) and dijet CM rest frame", 50, -1, 1);
  varList.push_back(CosTheta_j);

  varInfo *CosTheta_l = new varInfo("CosTheta_l", "CosTheta_l", "CosTheta between lepton and rest frame of leptonic W", 50, -1, 1);
  varList.push_back(CosTheta_l);
  
  varInfo *Cos_dPhiWH = new varInfo("Cos_dPhiWH", "Cos_dPhiWH", "CosPhi between normals of the leptonic W decay plane and Higgs decay plane", 50, -1, 1);
  varList.push_back(Cos_dPhiWH);

  varInfo *Cos_dPhiWW = new varInfo("Cos_dPhiWW", "Cos_dPhiWW", "CosPhi between normals of W decay planes in Higgs rest frame", 50, -1, 1);
  varList.push_back(Cos_dPhiWW); 

  varInfo *Mlvjj = new varInfo("MWWTop2jets_an", "MWWTop2jets_an", "mass of lvjj system", 1000, 0, 1000);
  varList.push_back(Mlvjj);

  varInfo *pTlvjj = new varInfo("Ptlnujj", "Ptlnujj", "pT of lvjj system", 300, 0, 300);
  varList.push_back(pTlvjj);

  varInfo *dRlepjj = new varInfo("dRlepjj", "dRlepjj", "delta #sqrt(#phi^2 + #eta^2) between leptonic and hadronic system", 120, 0, 12);
  varList.push_back(dRlepjj);


  // Muon only variables
  if( isMuon ){ 
    varInfo *muEta = new varInfo("muEta", "muEta", "muon #eta", 50, -3.1416, 3.1416); 
    varList.push_back(muEta);
 
    varInfo *muPt  = new varInfo("muPt", "muPt", "muon pT", 200, 0, 200);
    varList.push_back(muPt);

    varInfo *trkIso = new varInfo("trkIso", "trkIso", "Tracker Isolation", 6, -0.5, 5.5);
    varList.push_back(trkIso);

    varInfo *ecalIso = new varInfo("ecalIso", "ecalIso", "ECal Isolation", 31, -0.5, 30.5);
    varList.push_back(ecalIso);

    varInfo *hcalIso = new varInfo("hcalIso", "hcalIso", "HCal Isolation", 31, -0.5, 30.5);
    varList.push_back(hcalIso);
  
    varInfo *minDRMuJet = new varInfo("minDRMuJet", "minDRMuJet", "min deltaR between Muon and Leading Jet", 50, 0, 5);
    varList.push_back(minDRMuJet);
 
  } // end muon only variables


  // Electron only variables
  else {
    varInfo *eleEta_EE = new varInfo("eleEta_EE", "eleEta_EE", "electron #eta in End Cap", 50, -3.1416, 3.1416);
    varList.push_back(eleEta_EE);
 
    varInfo *eleEta_EB = new varInfo("eleEta_EB", "eleEta_EB", "electron #eta in Barrel", 50, -3.1416, 3.1416);
    varList.push_back(eleEta_EB);

    varInfo *elePt_EE = new varInfo("elePt_EE", "elePt_EE", "electron pT in End Cap", 200, 0, 200);
    varList.push_back(elePt_EE);

    varInfo *elePt_EB = new varInfo("elePt_EB", "elePt_EB", "electron pT in Barrel", 200, 0, 200);
    varList.push_back(elePt_EB);

    varInfo *relIso_EE = new varInfo("relIso_EE", "relIso_EE", "Relative Isolation in End-Cap", 50, -0.5, 2.5);
    varList.push_back(relIso_EE);
  
    varInfo *relIso_EB = new varInfo("relIso_EB", "relIso_EB", "Relative Isolation in Barrel", 50, -0.5, 2.5);
    varList.push_back(relIso_EB);  

    varInfo *minDREleJet = new varInfo("minDREleJet", "minDREleJet", "min deltaR between electron and leading jet", 50, 0, 5);
    varList.push_back(minDREleJet);

  } // end electron only variables


 
  //------------------------------------------------------------------------


  // Define Cuts

  // Declare vector to store cuts
  std::vector<cutInfo*> cutList;

  // Declare string to hold total Cuts
  TString cuts = "";


  // Define cut0, all events in skim preselection
  if( isMuon ){ 
    cutInfo *cut0 = new cutInfo("pre-selection", "Preselection", "pre-selection", "muPt>=0");
    cutList.push_back(cut0);
  }
  else{
    cutInfo *cut0 = new cutInfo("pre-selection", "Preselection", "pre-selection", "elePt_EE>=0 || elePt_EB>=0"); 
    cutList.push_back(cut0);
  }


  // Define Cut1a, nJets==2
  cutInfo *cut1a = new cutInfo("Exactly 2 Jets", "nJets==2", "nJets==2", "nJets==2");
  cutList.push_back(cut1a);

  
  // Define Cut1b, Leading Jet pT>30
  cutInfo *cut1b = new cutInfo("Lead Jet pT>30", "jet1 p$_T$$>$30", "jet1 pT > 30", "jet1Pt>30");
  cutList.push_back(cut1b);  


  // Define Cut1c, Trailing Jet pT>30
  cutInfo *cut1c = new cutInfo("2nd Jet pT>30", "jet2 p$_T$$>$30", "jet2 pT > 30", "jet2Pt>30");
  cutList.push_back(cut1c);


  // Define Cut1d, Lead Jet eta < 2.4
  cutInfo *cut1d = new cutInfo("Lead Jet eta<2.4", "jet1 $\\eta$$<$2.4", "&#124;jet1&eta;&#124; < 2.4", "jet1Eta<2.4 && jet1Eta>-2.4");
  cutList.push_back(cut1d);


  // Define Cut1e, Trailing Jet eta < 2.4
  cutInfo *cut1e = new cutInfo("2nd Jet eta<2.4", "jet2 $\\eta$$<$2.4", "&#124;jet2&eta;&#124; < 2.4", "jet2Eta<2.4 && jet2Eta>-2.4");
  cutList.push_back(cut1e);


  // Define Cut1f, Missing Transverse Energy > 25
  cutInfo *cut1f = new cutInfo("MET>25", "met$>$25", "met > 25", "met>25");
  cutList.push_back(cut1f);


  // Define Cut1g, delta R between lead Jet and lepton
  if( isMuon ){
    cutInfo *cut1g = new cutInfo("delR(jet1,muon)", "min $\\Delta$R MuJet1$>$0.5", "min &Delta;R MuJet > 0.5", "minDRMuJet>0.5");
    cutList.push_back(cut1g);
  }
  else{
    cutInfo *cut1g = new cutInfo("delR(jet1,ele)", "min $\\Delta$R EleJet1$>$0.5", "min &Delta;R EleJet > 0.5", "minDREleJet>0.5");
    cutList.push_back(cut1g);
  }


  // Define Cut1h, lepton pT
  if( isMuon ){
    cutInfo *cut1h = new cutInfo("muon pT>25", "muon p$_T$$>$25", "muon pT > 25", "muPt>25");
    cutList.push_back(cut1h);
  }
  else{
    cutInfo *cut1h = new cutInfo("ele pT>30", "ele p$_T$$>$30", "ele pT > 30", "(elePt_EE>30 || elePt_EB>30)");
    cutList.push_back(cut1h);
  }


  // Define Cut1i, lepton eta
  if( isMuon ){
    cutInfo *cut1i = new cutInfo("muon eta<2.1", "muon $\\eta$$<$2.1", "&#124;muon&eta;&#124; < 2.1", "muEta<2.1 && muEta>-2.1");
    cutList.push_back(cut1i);
  }
  else{
    cutInfo *cut1i = new cutInfo("ele eta<2.5", "ele $\\eta$$<$2.5", "&#124;ele&eta;&#124; < 2.5", "( (eleEta_EE<2.5 && eleEta_EE>-2.5)||(eleEta_EB<2.5 && eleEta_EB>-2.5) )");
    cutList.push_back(cut1i);
  }


  // Define Cut1j, lepton Isolation
  if( isMuon ){
    cutInfo *cut1j = new cutInfo("muon Isolation", "muon Isolation", "muon Isolation", "trkIso<1 && ecalIso<1 && hcalIso<1");
    cutList.push_back(cut1j);
  }
  else{
    cutInfo *cut1j = new cutInfo("ele Isolation", "ele Isolation", "ele Isolation", "( (relIso_EE<0.075 && relIso_EE>-100)||(relIso_EB<0.125 && relIso_EB>-100) )");
    cutList.push_back(cut1j);
  }


  // Define Cut2, Baseline with b-tagging
  cutInfo *cut2 = new cutInfo("bTags==0", "Baseline (with b-tag)", "base selection (with b-tag cut)", "nBTagsSSV==0"); 
  cutList.push_back(cut2); 


  // Define Cut3, Transverse mass of leptonic W
  cutInfo *cut3 = new cutInfo("lep W mT>40", "M$_T$ Lep$W$$>$40", "M<sub>T</sub>(W<sub>l&nu;</sub>) > 40", "mT>40");
  cutList.push_back(cut3); 


  // Define Cut4, delta eta between jet1 and jet2
  cutInfo *cut4 = new cutInfo("delEta(jet1,jet2)", "$\\Delta$$\\eta$(j1,j2)$<$1.5", "&#124;&Delta;&eta;(Jet1,Jet2)&#124; < 1.5", "dEtaJetJet_an<1.5 && dEtaJetJet_an>-1.5");
  cutList.push_back(cut4);


  // Define Cut5, pT(jet1+jet2)
  cutInfo *cut5 = new cutInfo("HadW pT>40", "Had$W$ p$_T$$>$40","pT(jet1+jet2) > 40", "WjjPt>40");
  cutList.push_back(cut5);


  // Define Cut6, pT(jet2)/m(jet1+jet2)
  cutInfo *cut6 = new cutInfo("0.3<pT(jet2)/MHadW<0.7", "0.3$<$j2p$_T$/MHad$W$$<$0.7", "0.3 < pT(jet2)/m(jet1+jet2) < 0.7", "(jet2Pt/m2_an)>0.3 && (jet2Pt/m2_an)<0.7");
  cutList.push_back(cut6);


  // Define Cut7, chi2
  //cutInfo *cut7 = new cutInfo("chi2", "$\chi$$^2$$<$10", "chi2(kinematic fit) < 10", "chi2<10");
  //cutList.push_back(cut7); 
  

  // Define Cut8, mJJ
  cutInfo *cut8 = new cutInfo("65<diJetMass<95", "65$<$diJetM$<$95", "65 GeV < mJJ < 95 GeV", "m2_an>65 && m2_an<95");
  cutList.push_back(cut8);


  // Define cut9, jackson angle
  cutInfo *cut9 = new cutInfo("JacksonAngle", "-0.6$<$JacksonAngle$<$0.8", "-0.6 < cosJacksonAngle(j, j) < 0.8", "jacksonAngle>-0.6 && jacksonAngle<0.8");
  cutList.push_back(cut9);
 

 
  //-----------------------------------------------------------------


  // Declare and load MC samples; unit of cross section, xs,  is pb
  
  // Declare vector to store all samples
  std::vector<sampleInfo *> sampleList;
 
  // Declare vectors to store signals and backgrounds, for cutflow table
  std::vector<sampleInfo*> sigList;
  std::vector<sampleInfo*> bgList;
  std::vector<sampleInfo*> twikiList;

  // Signal MCs
  sampleInfo *sig130 = new sampleInfo("sig M130","*H(130)*", "anaTree_HWW130", "SQWaT_template_HWW130", 14.12, 109986 );
  sampleList.push_back(sig130);
  sigList.push_back(sig130);  

  sampleInfo *sig140 = new sampleInfo("sig M140","*H(140)*", "anaTree_HWW140", "SQWaT_template_HWW140", 12.13, 109993 );
  sampleList.push_back(sig140);
  sigList.push_back(sig140);
  
  sampleInfo *sig150 = new sampleInfo("sig M150","*H(150)*", "anaTree_HWW150", "SQWaT_template_HWW150", 10.50, 109987 );
  sampleList.push_back(sig150);
  sigList.push_back(sig150);
  
  sampleInfo *sig160 = new sampleInfo("sig M160","*H(160)*", "anaTree_HWW160", "SQWaT_template_HWW160", 9.08, 109990 );
  sampleList.push_back(sig160);
  sigList.push_back(sig160);  

  sampleInfo *sig170 = new sampleInfo("sig M170", "*H(170)*", "anaTree_HWW170", "SQWaT_template_HWW170", 7.729, 109989 );
  sampleList.push_back(sig170);
  sigList.push_back(sig170);  

  sampleInfo *sig180 = new sampleInfo("sig M180","*H(180)*", "anaTree_HWW180", "SQWaT_template_HWW180", 6.739 , 109992 );
  sampleList.push_back(sig180);
  sigList.push_back(sig180);
  
  sampleInfo *sig190 = new sampleInfo("sig M190","*H(190)*", "anaTree_HWW190", "SQWaT_template_HWW180", 5.896 , 109988 );
  sampleList.push_back(sig190);
  sigList.push_back(sig190);  

  sampleInfo *sig250 = new sampleInfo("sig M250","*H(250)*", "anaTree_HWW250", "SQWaT_template_HWW250", 3.312 , 109990 );
  sampleList.push_back(sig250);
  sigList.push_back(sig250);
  
  sampleInfo *sig300 = new sampleInfo("sig M300","*H(300)*", "anaTree_HWW300", "SQWaT_template_HWW300", 2.422 , 106432 );
  sampleList.push_back(sig300);
  sigList.push_back(sig300);  
  twikiList.push_back(sig300);

  sampleInfo *sig350 = new sampleInfo("sig M350","*H(350)*","anaTree_HWW350", "SQWaT_template_HWW350", 2.306, 109635);
  sampleList.push_back(sig350);
  sigList.push_back(sig350);

  sampleInfo *sig400 = new sampleInfo("sig 400","*H(400)*","anaTree_HWW400", "SQWaT_template_HWW400", 2.032 , 109991);
  sampleList.push_back(sig400);
  sigList.push_back(sig400);  
  twikiList.push_back(sig400);

  sampleInfo *sig450 = new sampleInfo("sig M450","*H(450)*","anaTree_HWW450", "SQWaT_template_HWW450",  1.360, 109978);
  sampleList.push_back(sig450);
  sigList.push_back(sig450);

  sampleInfo *sig500 = new sampleInfo("sig M500","*H(500)*","anaTree_HWW500", "SQWaT_template_HWW500", 0.849, 109982);
  sampleList.push_back(sig500);
  sigList.push_back(sig500);
  twikiList.push_back(sig500);

  sampleInfo *sig550 = new sampleInfo("sig M550","*H(550)*", "anaTree_HWW550", "SQWaT_template_HWW550", 0.528, 106419);
  sampleList.push_back(sig550);
  sigList.push_back(sig550);

 

  //----------------------------------------------------------------------


  // Declare and load MC background samples

  sampleInfo *ww = new sampleInfo("$WW$", "*WW*","anaTree_ww", "SQWaT_template_WWany", 43.0, 2061760);
  sampleList.push_back(ww);
  bgList.push_back(ww);
  twikiList.push_back(ww);

  sampleInfo *wjets = new sampleInfo("$W$+jets","*W+jets*","anaTree_wjets", "SQWaT_template_wjets", 3.1314e+04, 15110974);
  sampleList.push_back(wjets);
  bgList.push_back(wjets);
  twikiList.push_back(wjets);

  sampleInfo *zjets = new sampleInfo("$Z$+jets","*Z+jets*","anaTree_zjets", "SQWaT_template_zjets", 3.048e+03, 2229439);
  sampleList.push_back(zjets);
  bgList.push_back(zjets);
  twikiList.push_back(zjets);  

  sampleInfo *ttbar = new sampleInfo("$t\\bar{t}$","*ttbar*","anaTree_ttbar", "SQWaT_template_ttbar", 1.5750e+02, 1064208 );
  sampleList.push_back(ttbar);
  bgList.push_back(ttbar);
  twikiList.push_back(ttbar);  

  
  //----------------------------------------------------------------------

  // Need to add together histograms for Single Top
  //  Add indvidual channel sampleInfo pointers to an addList
  std::vector<sampleInfo*> addList_sTop;

  sampleInfo *sTopS  = new sampleInfo("$t$ s chan", "*Single Top (s-channel)*", "anaTree_singleTop_s", "SQWaT_template_singletop_s", 1.50, 494967);
  sampleList.push_back(sTopS);
  //bgList.push_back(sTopS);
  addList_sTop.push_back(sTopS);  
  twikiList.push_back(sTopS);

  sampleInfo *sTopT  = new sampleInfo("$t$ t chan", "*Single Top (t-channel)*","anaTree_singleTop_t", "SQWaT_template_singletop_t", 21.00, 484060);
  sampleList.push_back(sTopT);
  //bgList.push_back(sTopT);
  addList_sTop.push_back(sTopT);  
  twikiList.push_back(sTopT);

  sampleInfo *sTopTW = new sampleInfo("$t$ tW chan", "*Single Top (tw-channel)*","anaTree_singleTop_tW", "SQWaT_template_singletop_tw", 10.60, 489417);
  sampleList.push_back(sTopTW);
  //bgList.push_back(sTopTW);   
  addList_sTop.push_back(sTopTW);
  twikiList.push_back(sTopTW);  

  // Create a pointer of sample info to summed files
  sampleInfo *sTop = new sampleInfo("$t$", "*Single Top*", "", "SQWaT_template_singletop", 1, 1); 
  bgList.push_back(sTop); 
  
 
  // Also need to add QCD electron samples
  std::vector<sampleInfo*> addList_qcd;
  sampleInfo *qcdEle=NULL;
  
  if( isMuon ) {
    sampleInfo *qcd = new sampleInfo("QCD", "*QCD*", "anaTree_qcd", "SQWaT_template_qcd", 84700.00 , 29434562);
    sampleList.push_back(qcd);
    bgList.push_back(qcd);
    twikiList.push_back(qcd);
  }
  else {
    sampleInfo *qcdEm20to30 = new sampleInfo("QCD", "*QCD (em 20to30)*","anaTree_qcd_em20to30", "SQWaT_template_qcd_em20to30", 2450000.00, 36061246);
    sampleList.push_back(qcdEm20to30);
    //bgList.push_back(qcdEm20to30);
    addList_qcd.push_back(qcdEm20to30);

    sampleInfo *qcdEm80to170 = new sampleInfo("QCD", "*QCD (em 80to170)*", "anaTree_qcd_em80to170", "SQWaT_template_qcd_em80to170", 140000.00, 8069591);
    sampleList.push_back(qcdEm80to170);
    //bgList.push_back(qcdEm80to170);
    addList_qcd.push_back(qcdEm80to170);
    
    sampleInfo *qcdBCtoE20to30 = new sampleInfo("QCD", "*QCD (BCtoE 20to30)*","anaTree_qcd_bctoe20to30", "SQWaT_template_qcd_bctoe20to30",132000, 2243439);
    sampleList.push_back(qcdBCtoE20to30);
    //bgList.push_back(qcdBCtoE20to30);
    addList_qcd.push_back(qcdBCtoE20to30);

    sampleInfo *qcdBCtoE30to80 = new sampleInfo("QCD", "*QCD (BCtoE 30to80)*", "anaTree_qcd_bctoe30to80", "SQWaT_template_qcd_bctoe30to80",137000,1995502);
    sampleList.push_back(qcdBCtoE30to80);
    //bgList.push_back(qcdBCtoE30to80);
    addList_qcd.push_back(qcdBCtoE30to80);

    sampleInfo *qcdBCtoE80to170 = new sampleInfo("QCD", "*QCD (BCtoE 80to170)*","anaTree_qcd_bctoe80to170", "SQWaT_template_qcd_bctoe80to170", 9360, 1043390);
    sampleList.push_back(qcdBCtoE80to170);
    //bgList.push_back(qcdBCtoE80to170);
    addList_qcd.push_back(qcdBCtoE80to170);
    
    // Create a pointer of sample info to summed files
    qcdEle = new sampleInfo("QCD", "*QCD*", "", "SQWaT_template_qcd", 1, 1);
    bgList.push_back(qcdEle);
    twikiList.push_back(qcdEle);
  }

  // Sample Info to store sum of all backgrounds
  sampleInfo *allBg = new sampleInfo("Sum of Backgrounds", "*B*", "", "SQWaT_template_sumBg", 1, 1);
  twikiList.push_back(allBg);


  //-------------------------------------------------------------------------

  
  // SampleInfo for Data
  sampleInfo *data;
  if( isMuon) {
    data = new sampleInfo("2010-11 Muon Data", "*muon data*","anaTree_data", "SQWaT_template_data", 1, 1);
    sampleList.push_back(data);
  }
  else {
    data = new sampleInfo("2010-11 Electron Data", "*electron data*", "anaTree_data", "SQWaT_template_data", 1, 1);
    sampleList.push_back(data);
  }  


  //-------------------------------------------------------------------------


  // Create Histograms at each cut level
 
  // Loop over cuts
  int num=0; //counter
  for(std::vector<cutInfo*>::const_iterator cutIter = cutList.begin(); 
      cutIter !=cutList.end(); cutIter++){
    
    // Pass pointer on cutInfo
    const cutInfo *cI = *cutIter;
  
    // Create a string to describe which cut we're on 
    TString cutNum = "cut";
    cutNum += num;

    // Print status
    cout << "Processing: " << cutNum << ": " << endl;
    TString effCut = "";
    if( cutSeq && num!=0 && num!=1 ) effCut += " && ";
    effCut += cI->cut;
    cout << effCut << endl << endl;

    // Loop over samples
    for(std::vector<sampleInfo*>::const_iterator tIter1 = sampleList.begin(); 
        tIter1 !=sampleList.end(); tIter1++){
  
      // Pass a pointer to call sample info
      const sampleInfo *t1 = *tIter1;
  
      // Get sample input file info
      TString fnameTemp = t1->fname;
      if (isMuon) fnameTemp += ".root" ;
      else fnameTemp += "_ele.root" ;
      TFile *f = new TFile(fnameTemp);
       
      // Get sample output file info to store histograms
      TString outfNameTemp = t1->ofname;
      outfNameTemp += "_";
      outfNameTemp += cutNum;
      if (!cutSeq) outfNameTemp += "_NonSeq";
      if (isMuon) outfNameTemp += ".root" ;
      else outfNameTemp += "_ele.root" ;
      TFile *of = new TFile(outfNameTemp, "RECREATE");
      
      // Get anatree from input file
      TTree *anaTree = (TTree*)f->Get("anaTree");
      
      // Setup TSting for all cuts
      if (t1->fname=="anaTree_data") cuts = "1*(";
      else cuts = "weight*(";
      if( num==0 ) cuts += cI->cut;
      else{
        if(cutSeq){
          for(int i=1; i<=num; i++){
            if(i!=1) cuts += " && ";
            cuts += cutList[i]->cut; 
          }
        }
        else cuts += cI->cut;
      }
      cuts += ")";
      //cout << cuts << endl << endl;
      
      // Loop over variables in each sample 
      for(std::vector<varInfo*>::const_iterator tIter2 = varList.begin(); 
          tIter2 != varList.end(); tIter2++){
     
        // Pass a pointer to variable info
        const varInfo *v = *tIter2;
  
        // Declare histogram for that variable
        TH1F *hist = new TH1F(v->hName, v->hTitle, v->nBins, v->min, v->max);
        hist->SetTitle(v->hTitle);
        
        // Declare string of variable and cuts to pass to anatree
        TString arg = v->var + " >> " + v->hName;
        
        // Draw histogram with appropriate cuts
        anaTree->Draw(arg, cuts, "goff");
        
        // Grab histogram from the aether put in output file
        TH1F *newHist = (TH1F*) gDirectory->Get(v->hName);
        newHist->SetTitle(v->hTitle);
        newHist->SetDirectory(of);
        newHist->Sumw2();      

      } // end variable loop
    
      // Write and close output file
      of->Write();
      of->Close();
  
      // Close input file
      f->Close();

    } // end sample loop
        

    //---------------------------------------------------------------------


    // Add histograms for single top, qcd, and all backgrounds
    addHists( addList_sTop, sTop,  varList, cutNum, isMuon, cutSeq);    
    if( !isMuon ) addHists( addList_qcd, qcdEle, varList,cutNum,isMuon,cutSeq);
    addHists( bgList,       allBg, varList, cutNum, isMuon, cutSeq);

    //---------------------------------------------------------------------
 
    // Increase cut number counter
    num++;

  } // end cut loop


  //------------------------------------------------------------------------


  // Print Cut Flow tables
  cout << "Printing Cut Flow Tables" << endl;
  
  printTable(sigList, bgList, allBg, varList, Mlvjj, cutList, lumi, isMuon, cutSeq);  
  twikiTable(twikiList, varList, cutList, lumi, isMuon, cutSeq);
  dataTable(data, varList, cutList, isMuon, cutSeq);

  // Create cutflow Efficiency histograms
  cout << endl << "Processing cut flow efficiency histograms" << endl;
  efficiencyTable(sampleList, varList, cutList, isMuon, cutSeq);



  return 0;

}

