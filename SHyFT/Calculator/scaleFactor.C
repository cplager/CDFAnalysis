#include "TROOT.h"
#include "TString.h"
#include "TIterator.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include "math.h"
#include <time.h>
#include <fstream>

//Utility class for managing binning information
class sampleInfo {
  
public:
  TString name;
  double xsMC;
  double xsFit;
  double error;
   
  sampleInfo() : name(""), xsMC(0), xsFit(0), error(0) {}
  
  sampleInfo(TString bName, double bMC, double bFit, double bErr) :
    name (bName), xsMC(bMC), xsFit(bFit), error(bErr) {}
  
};

void scalefactor( double SF_Muon =  0.965,  double wBR =  0.9818){

  //////////input constants
  //  double SF_Muon =  0.965 ; // muon scale factor
  //  double wBR =  0.9818 ;   // w Branching Ratio

  //////////////
  std::cout << "Initializing list of samples..." << std::endl;
  cout << '\n'
       << '\n';

  ////////// cout the output to a file
  streambuf* coutBuf = cout.rdbuf();
  ofstream of("table_SF.txt");    //----------
  streambuf* fileBuf = of.rdbuf();
  cout.rdbuf(fileBuf);

  ////////////////////////////////////////////////////////////////
  /////////     -------------- how to use this code --------------
  ///////// 1. input all the fitting results here
  //////       e.g.  sampleInfo( sampleName, xsMC, xsFit, error)
  ////////  2. input muon scale factor and W branching Ratio when you call the function
  //////////////////////////////////////////////////////
  std::vector<sampleInfo *> sampleList;

  sampleInfo *sample1 = new sampleInfo("$t\\bar{t}$", 1.5750e-01, 1.4801e-01, 1.99e-02 );
  sampleList.push_back(sample1);

  sampleInfo *sample2 = new sampleInfo("$W$+jets", 3.1314e+01, 3.8241e+01, 3.37e+00 );
  sampleList.push_back(sample2);
  
  sampleInfo *sample3 = new sampleInfo("$Z$+jets", 9.7337e-02, 9.8467e-02, 2.91e-02 );
  sampleList.push_back(sample3);
  
  sampleInfo *sample4 = new sampleInfo("Single Top", 1.0000e+00, 1.0114e+00, 3.00e-01 );
  sampleList.push_back(sample4);
  
  sampleInfo *sample5 = new sampleInfo("QCD", 0.5*1.6936e+02, 1.8993e+02, 3.78e+01 );
  sampleList.push_back(sample5);    /// qcd xs_MC has to be divided by 2

  /////////////
  cout << "%--------check if you got all the input numbers right-------------" << endl;
  cout << "% " << left <<setw(10) << "name" 
       << " & " <<setw(10) << "MC xs "
       << " & " <<setw(10) << "fitted xs "
       << " & " <<setw(10) << "error " 
       << " \\"<< "\\"  << endl;

  for (std::vector<sampleInfo *>::const_iterator tIter1 = sampleList.begin();
       tIter1 != sampleList.end(); ++tIter1) {
    
    const sampleInfo *t1 = *tIter1;
    
    TString name1 = t1->name;
    double mc1 = t1->xsMC;
    double fit1 = t1->xsFit;
    double fiterr1 = t1->error;
      
      cout << "% "  <<setw(10) << name1 
	   << " & " <<setw(10) << mc1 
	   << " & " <<setw(10) << fit1 
	   << " & " <<setw(10) << fiterr1 << " \\"<< "\\"  << endl;
    
  }

  cout << '\n'
       << '\n';

  ////----------------------------------------------------
  //// print out some tex content first
  cout << "\\begin{table}" << endl
       << "\\begin{center}" << endl
       << "\\begin{tabular}{|l|l|}" << endl
       << "\\hline" << endl
       << "Sample  & Scale Factor "<<"\\"<<"\\" << endl
       << "\\hline" << endl;
  
  ///////////// print out the table
  for (std::vector<sampleInfo *>::const_iterator tIter = sampleList.begin();
       tIter != sampleList.end(); ++tIter) {
    
    const sampleInfo *t = *tIter;
    
    TString name = t->name;
    double mc = t->xsMC;
    double fit = t->xsFit;
    double fiterr = t->error;
    
    double meas = 0;
    double err = 0;
    double ratio = 0;
    double errRatio = 0;

    if ( name == "$t\\bar{t}$" ){
      meas = fit * (1/SF_Muon) * (1/wBR);
      err = fiterr * (1/SF_Muon) * (1/wBR);
    }else if ( name == "$Z$+jets" ){
      meas = fit * (1/SF_Muon) * (sample2->xsFit) * (1/sample2->xsMC); 
      err = fiterr * (1/SF_Muon) * (sample2->xsFit) * (1/sample2->xsMC);
    }else {
      meas = fit * (1/SF_Muon) ;
      err = fiterr * (1/SF_Muon) ;
    }
    
    ratio = meas/mc ;
    errRatio = err/mc ;
    
    cout << setiosflags(ios::fixed)
	 << setprecision(2) ;

    cout <<setw(15) << name << " & " << ratio << " $\\pm " << errRatio <<  " $ "  << "\\"<< "\\"  << endl;
  }
  
  ///// print out some tex content again
  cout << "\\hline" << endl
       << "\\end{tabular}" << endl
       << "\\caption{Scale factors extracted from the unoptimized $3,\\geq 4$" << endl
       << "fit.  These scale factors have  been adjusted for the" << endl
       << "results of the muon trigger, ID, and isolation efficiency" << endl
       << "measurement.}" << endl
       << "\\label{tab:scale_factors}" << endl
       << "\\end{center}" << endl
       << "\\end{table}" << endl;
  
  cout << '\n' 
       << '\n' 
       << '\n' ;
 
 /////print out current date when the table is made
  struct tm *current;
  time_t now;
  now = time(NULL);
  //  time(&now);
  current = localtime(&now);
  int year = current->tm_year +1900 ;
  int month = current->tm_mon +1 ;
  int day = current->tm_mday ;
  cout << '\n'
       << '\n';
  cout << "this table is made on: " <<  year << "-" << month << "-" << day << endl; 

  ////////////////
  of.flush();
  of.close();
  cout.rdbuf(coutBuf);

  cout << "Done, all info is in table_SF.txt" << endl;
  
}
