#ifndef WJETSTEMPLATE
#define WJETSTEMPLATE

#include "Fitter.h"

#include "RooGaussian.h"

using namespace RooFit;

namespace Fitter {

  class WJetsTemplateInfo : public TemplateInfo {

  public:

    WJetsTemplateInfo(const TString &name, 
                 const TString &inFileName, 
                 int nGenEvents, double crossSection, int color);

    RooArgSet &getConstraints() {return parConstraints;}
    RooArgSet &getConstrainedPars() {return constrainedPars;}

    //These are the real fit parameters
    RooArgSet extraPars;
    RooArgSet constrainedPars;
    RooArgSet parConstraints;

    static const double a0Mean, a0Sig, a1Mean, a1Sig, a2Mean, a2Sig;

  };

  const double WJetsTemplateInfo::a0Mean = 1.61e-1;
  const double WJetsTemplateInfo::a0Sig  = 5.44e-2;
//   const double WJetsTemplateInfo::a0Sig  = 4.0e-2;
//   const double WJetsTemplateInfo::a0Sig  = 1.0e-2;
  const double WJetsTemplateInfo::a1Mean = 3.75e-2;
  const double WJetsTemplateInfo::a1Sig  = 2.39e-2;
//   const double WJetsTemplateInfo::a1Sig  = 1.0e-2;
//   const double WJetsTemplateInfo::a1Sig  = 3.0e-3;
  const double WJetsTemplateInfo::a2Mean =-1.05e-2;
  const double WJetsTemplateInfo::a2Sig  = 3.65e-2;
//   const double WJetsTemplateInfo::a2Sig  = 2.00e-2;
//   const double WJetsTemplateInfo::a2Sig  = 3.00e-3;


  WJetsTemplateInfo::WJetsTemplateInfo(const TString &nm, const TString &inFileName, 
                                       int nGE, double xs,int c):
    TemplateInfo(nm,inFileName,nGE,xs,false,c) {
    
    //Make the actual fit parameters
    TString parName = "xs";
    parName += name;
    RooRealVar *xsVar = (RooRealVar *)extraPars.addClone(RooRealVar(parName,parName,
                                                               crossSection,
                                                               -10*crossSection,20*crossSection));

    RooRealVar *a0 = (RooRealVar *)constrainedPars.addClone(RooRealVar("a0","a0",
                                                               a0Mean,
                                                               a0Mean-20*a0Sig,a0Mean+20*a0Sig));

    RooRealVar *a1 = (RooRealVar *)constrainedPars.addClone(RooRealVar("a1","a1",
                                                               a1Mean,
                                                               a1Mean-20*a1Sig,a1Mean+20*a1Sig));
    
    RooRealVar *a2 = (RooRealVar *)constrainedPars.addClone(RooRealVar("a2","a2",
                                                               a2Mean,
                                                               a2Mean-20*a2Sig,a2Mean+20*a2Sig));


    //Remove the standard "fit parameters (number of events in each jet bin) and replace with
    //the calculated fit parameters
    fitPars.removeAll();

    double nTotal = 0;
    for (std::map<TString,TH1 *>::iterator jetBinIter = histMap.begin();
         jetBinIter != histMap.end(); ++jetBinIter) {
      
      const TH1 *hist = jetBinIter->second;
      nTotal += hist->Integral();

    }

    for (std::map<TString,TH1 *>::iterator jetBinIter = histMap.begin();
         jetBinIter != histMap.end(); ++jetBinIter) {
      
      const TString &jetBinName = jetBinIter->first;

      int iJet = jetBin->lookupType(jetBinName)->getVal();

      //Actually define the variable string
      double factor = lumi*Fitter::lumiConvFactor*nTotal/nGenEvts;
      TString formulaStr = "@0*";
      formulaStr += factor;

      for (int i = 1; i <= iJet; ++i) {

        //Transform into the variable space used by the Chebyshev Polynomial...
        double x = -1 + 2*((double)i+0.5)/6;
        double x2 = 2*x*x-1;
        formulaStr += "*(@1 + @2*";
        formulaStr += x;
        formulaStr += " + @3*";
        formulaStr += x2;
        formulaStr += ")";
      }

      if (!jetBinName.Contains(">=")) {
        double x = -1 + 2*((double)(iJet+1)+0.5)/6;
        double x2 = 2*x*x-1;
        formulaStr += "*(1-@1 - @2*";
        formulaStr += x;
        formulaStr += " - @3*";
        formulaStr += x2;
        formulaStr += ")";
      }

      TString safeName = jetBinName;
      if (safeName.Contains(">=")) {
        safeName.Remove(safeName.Index(">="),2);
      }
      
      TString parName = "num";
      parName += name;
      parName += safeName;
      fitPars.addClone(RooFormulaVar(parName,parName,formulaStr,RooArgList(*xsVar,*a0,*a1,*a2)));

    }

    RooGaussian *a0Constr = 
      new RooGaussian("a0Constr","Constraint on a0",*a0,RooConst(a0Mean),RooConst(a0Sig));
    parConstraints.addOwned(*a0Constr);
    
    RooGaussian *a1Constr = 
      new RooGaussian("a1Constr","Constraint on a1",*a1,RooConst(a1Mean),RooConst(a1Sig));
    parConstraints.addOwned(*a1Constr);

    RooGaussian *a2Constr = 
      new RooGaussian("a2Constr","Constraint on a2",*a2,RooConst(a2Mean),RooConst(a2Sig));
    parConstraints.addOwned(*a2Constr);


  }

}

#endif
