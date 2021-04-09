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

//Utility class for managing systematic sample information
class SysInfo {
  
public:
  TString name;
  double uncertUP;
  double uncertDOWN;
  bool symmetric;
  
  SysInfo() : name(""), uncertUP(0), uncertDOWN(0), symmetric(false) {}
  
  SysInfo(TString bName, double xUp, double xDown, bool symmetry) :
    name (bName), uncertUP(xUp), uncertDOWN(xDown), symmetric(symmetry) {}
  
};

/////////////////////
void calculate( double stat = 12,double lumi = 11, double xsfit = 1.4801e-01 ){
  // I dropped "%" for all the numbers just to make it easier

  //////////
  std::cout << "Initializing list of systematics..." << std::endl;
  cout << '\n' 
       << '\n' ;

  ////////// cout the output to a file
  streambuf* coutBuf = cout.rdbuf();
  ofstream of("table_sysUncert.txt");    //----------
  streambuf* fileBuf = of.rdbuf();
  cout.rdbuf(fileBuf);
    
  /////////////////////////////////////////////////////////////////
  /////////     -------------- how to use this code --------------
  ///////// 1. you need to input all the systematic uncertainties below
  ////////    e.g. SysInfo( name , uncertUP, uncertDOWN, symmetric?)
  ///////// 2. input muon scale factor and W branching Ratio
  ///////// 3. input statistic and lumi uncertainty when you call the function
  ////////////////////////////////////////////////////////////////
  double SF_Muon =  0.965 ;
  double wBR =  0.9818 ;   // Branching Ratio

  std::vector<SysInfo *> sysList;

  //////////// asymmetric types (print two uncerts)
  SysInfo *sys1 = new SysInfo("JES", 18.8, -18.4, false);
  sysList.push_back(sys1);

  SysInfo *sys2 = new SysInfo("JER", 1.7, -0.8,false);
  sysList.push_back(sys2);
  
  SysInfo *sys3 = new SysInfo("scale up/down ($t\\bar{t}$)", 12.9, -7.9,false);
  sysList.push_back(sys3);
  
  SysInfo *sys4 = new SysInfo("scale up/down (W/Z+Jets)", 6.9, -6.9,false);
  sysList.push_back(sys4);
  
  SysInfo *sys5 = new SysInfo("matching up/down ($t\\bar{t}$)", 2.6, -2.7,false);
  sysList.push_back(sys5);
  
  SysInfo *sys6 = new SysInfo("matching up/down (W/Z+Jets)", 9, -9,false);
  sysList.push_back(sys6);

  //////////// symmetric types (print only one uncert)
  SysInfo *sys12 = new SysInfo("PileUp", 4.2, -4.2,true);
  sysList.push_back(sys12);

  SysInfo *sys7 = new SysInfo("PDF", 3, -3,true);
  sysList.push_back(sys7);

  SysInfo *sys8 = new SysInfo("ISR/FSR", 4.6, -4.6,true);
  sysList.push_back(sys8);

  SysInfo *sys9 = new SysInfo("QCD estimate", 3, -3,true);
  sysList.push_back(sys9);

  SysInfo *sys10 = new SysInfo("Muon Trigger/ID/Iso SF", 3, -3,true);
  sysList.push_back(sys10);

  SysInfo *sy11 = new SysInfo("$W$ Branching Fraction", 1, -1,true);
  sysList.push_back(sy11);


  ////// let's start printing the table
  /////// print out some tex content first
 
  cout << "\\begin{table}" << endl
       << "\\begin{center}" << endl
       << "\\begin{tabular}{|l|c|c|}" << endl
       << "\\hline" << endl
       << "\\multicolumn{3}{|c|}{Summary Of Uncertainties For 3, $\\geq$ 4 jets}" << "\\" <<"\\" << endl
       << "\\hline" << endl
       << "Source   & Relative Uncertainty & Stat + Sys Combined "<< "\\" << "\\" << endl
       << "\\hline" << endl;

  ////////////////////
  double sqSumUP = 0;
  double sqSumDOWN = 0;
  for (std::vector<SysInfo *>::const_iterator tIter = sysList.begin();
       tIter != sysList.end(); ++tIter) {
    
    const SysInfo *t = *tIter;
    
    TString sysname = t->name;
    double sysUP = t->uncertUP;
    double sysDOWN = t->uncertDOWN;
    bool sysSYM = t->symmetric;
   
    sqSumUP += sysUP*sysUP;
    sqSumDOWN += sysDOWN*sysDOWN;

    double combineUP = sqrt(stat*stat + sysUP*sysUP);
    double combineDOWN =  sqrt(stat*stat + sysDOWN*sysDOWN);

    cout <<setiosflags(ios::fixed) 
	 << setprecision(1) ; 

    if (!sysSYM) {
      cout << left <<setw(35) <<sysname << " & " << "${+" << sysUP << "}/{"  << sysDOWN << "}$" << "\\%" << " & " << "${+"  <<combineUP << "}/{-"  << combineDOWN << "}$" << "\\% " << "\\" << "\\" << endl;
    }else {
      cout << setw(35) <<sysname << " & " << "$\\pm "  << sysUP <<  "$" << "\\%" << " & "  << "$\\pm "  << combineUP <<  "$" << "\\% " << "\\"<< "\\"  << endl;
    }
  }

  /////////// the second part of the table

  double totalSysUP = sqrt(sqSumUP);
  double totalSysDOWN = sqrt(sqSumDOWN);
  double totalUP = sqrt(sqSumUP + stat*stat);
  double totalDOWN = sqrt(sqSumDOWN + stat*stat);
  cout << "\\hline" << endl
       <<setw(35) << "Total Systematic" << " & " << "${+"  << totalSysUP << "}/{-" << totalSysDOWN << "}$" << "\\%" << " & " << "- " << "\\"<< "\\" <<endl
       << "\\hline" << endl
       << setw(35) <<"Statistical" << " & " << stat <<"\\%" << " & " << "- " << "\\"<< "\\"  <<  endl
       << "\\hline" << endl
       <<setw(35) << "Total Uncert = Stat + Sys" <<" & " << "${+"  << totalUP << "}/{-"  << totalDOWN << "}$" << "\\%"<< " & " << "- " << "\\"<< "\\"  <<endl
       << "\\hline" << endl
       << setw(35) <<"Lumi (quoted separately)" << " & "<< lumi << "\\%" << " & " <<"- " <<"\\"<< "\\"  <<  endl;

  ///////// print out some tex content again
  cout << "\\hline" << endl;
  cout << "\\end{tabular}" << endl
       << "\\caption{Summary of uncertainties from various sources. These uncertainties" << endl
       << "were evaluated using only events with 3 and $\\geq$ 4 jets.}" << endl
       << "\\label{tab:sys_summary_optimized}" << endl
       << "\\end{center}" << endl
       << "\\end{table}" << endl;

  cout << '\n'
       << '\n'
       << '\n'
       << '\n';
 
  /////////////////////////////////////////////////////////
  ////  print out the final equation for measured crossSection
  /////////////////////////////////////////////////////////
//   double SF_Muon =  0.965 ;
//   double wBR =  0.9818 ;   // Branching Ratio
  //  double xsfit = 1.4788e-01 ;       

  double meas = xsfit*1000/(SF_Muon*wBR) ;
  double statF = stat * meas/100 ;
  double sysUPF = totalSysUP * meas/100 ;
  double sysDOWNF = totalSysDOWN * meas/100 ;
  double lumiF = lumi * meas/100 ;

  cout << "\\begin{equation}" << endl;
  cout << "\\sigma (t\\bar{t}) = " << meas <<" \\pm " << statF <<" (\\rm stat.) ^{+" << sysUPF <<"}_{-"<< sysDOWNF <<"} (\\rm syst.)  \\pm " << lumiF <<" (\\rm lumi.) ~ \\rm pb." << endl;
  cout << "\\end{equation}" << endl;

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

  cout << "Done, all info is in table_sysUncert.txt" << endl;
}
