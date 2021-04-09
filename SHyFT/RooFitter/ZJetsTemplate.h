#ifndef ZJETSTEMPLATE
#define ZJETSTEMPLATE

#include "Fitter.h"

#include "RooGaussian.h"

using namespace RooFit;

namespace Fitter {

  class ZJetsTemplateInfo : public TemplateInfo {

  public:

    ZJetsTemplateInfo(const TString &name, 
                      const TString &inFileName, 
                      int nGenEvents, double crossSection, int color,
                      const TemplateInfo *wjets);

    RooArgSet &getConstraints() {return parConstraints;}
    RooArgSet &getConstrainedPars() {return constrainedPars;}

    //These are the real fit parameters
    RooArgSet constrainedPars;
    RooArgSet parConstraints;
    

  };

  ZJetsTemplateInfo::ZJetsTemplateInfo(const TString &nm, const TString &inFileName, 
                                       int nGE, double xs,int c, const TemplateInfo *wjets):
    TemplateInfo(nm,inFileName,nGE,xs,wjets->fixedNJets,c) {
    

    if (fixedNJets) {

      //Now, let's try to do something a little fancy.  Let's suppose that
      //we want to fit for the ratio of Z to W instead.
      //Step 1:  Get the Z and the W cross section parameters
      RooAbsReal *zXSPar = (RooAbsReal *)fitPars.first();
      RooAbsReal *wXSPar = (RooAbsReal *)wjets->fitPars.first();

      //Next, figure out what the MC ratio of these is
      double rZW = zXSPar->getVal()/wXSPar->getVal();

      //Now make a new fit parameter for the ratio
      RooRealVar *ratioZW = new RooRealVar("ratioZW","ratioZW",rZW,-0.5,10*rZW);
      constrainedPars.addOwned(*ratioZW);

      //Next, we need a "formula" variable to relate the ratio to the Z XS:
      TString zName = zXSPar->GetName();
      TString zTitle = zXSPar->GetTitle();

      //Get replace old parameter with new
      fitPars.remove(*zXSPar);
      zXSPar = new RooFormulaVar(zName,zTitle,"@0*@1",
                                 RooArgList(*ratioZW,*wXSPar));

      //Now, make this the fit parameter instead of original one
      //Magically, now the cross section be a function of the ratio.
      fitPars.addOwned(*zXSPar);

      //Instead, constrain ratio of W to Z to be within 30% of SM prediction.
      RooGaussian *rConstraint =
        new RooGaussian("rZWConstraint","rZWConstraint",*ratioZW, RooConst(rZW), RooConst(0.3*rZW));
      
      parConstraints.addOwned(*rConstraint);

      

    } else {

      for (std::map<TString,TH1 *>::iterator jetBinIter = histMap.begin();
           jetBinIter != histMap.end(); ++jetBinIter) {
        
        const TString &jetBinName = jetBinIter->first;
        
        TString safeName = jetBinName;
        if (safeName.Contains(">=")) {
          safeName.Remove(safeName.Index(">="),2);
        }
        
        
        //Now, let's try to do something a little fancy.  Let's suppose that
        //we want to fit for the ratio of Z to W instead.
        //Step 1:  Get the Z and the W cross section parameters
        TString parName = "num";
        parName += name;
        parName += safeName;
        RooAbsReal *zXSPar = (RooAbsReal *)fitPars.find(parName);
        TString parNameW = "num";
        parNameW += wjets->name;
        parNameW += safeName;
        RooAbsReal *wXSPar = (RooAbsReal *)wjets->fitPars.find(parNameW);
        
        //Next, figure out what the MC ratio of these is
        double rZW = zXSPar->getVal()/wXSPar->getVal();
        
        //Now make a new fit parameter for the ratio
        TString ratioName = "ratioZW";
        ratioName += safeName;
        RooRealVar *ratioZW = new RooRealVar(ratioName,ratioName,rZW,-0.5,10*rZW);
        constrainedPars.addOwned(*ratioZW);
        
        //Next, we need a "formula" variable to relate the ratio to the Z XS:
        TString zName = zXSPar->GetName();
        TString zTitle = zXSPar->GetTitle();
        
        //Get replace old parameter with new
        fitPars.remove(*zXSPar);
        zXSPar = new RooFormulaVar(zName,zTitle,"@0*@1",
                                   RooArgList(*ratioZW,*wXSPar));
        
        //Now, make this the fit parameter instead of original one
        //Magically, now the cross section be a function of the ratio.
        fitPars.addOwned(*zXSPar);
        
        //Instead, constrain ratio of W to Z to be within 30% of SM prediction.
        TString constrName = "rZWConstraint";
        constrName += safeName;
        RooGaussian *rConstraint =
          new RooGaussian(constrName,constrName,*ratioZW, RooConst(rZW), RooConst(0.3*rZW));
        
        parConstraints.addOwned(*rConstraint);
      }
    }

  }

}

#endif
