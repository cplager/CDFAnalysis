#ifndef ROO_VARYPARMODULE
#define ROO_VARYPARMODULE

#include "RooAbsMCStudyModule.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooMCStudy.h"
#include "RooFitResult.h"
#include "TMinuit.h"

#include <iostream>
#include <map>

class VaryParModule : public RooAbsMCStudyModule {
public:

  VaryParModule(RooArgSet &constraints, RooMCStudy *mcs);
  VaryParModule(const VaryParModule& other) : RooAbsMCStudyModule(other), 
                                            genParData((RooDataSet *)other.genParData->Clone()), genPars(other.genPars) {}
  virtual ~VaryParModule();

  void addConstraints(RooArgSet &c);
  Bool_t initializeInstance();
  Bool_t initializeRun(Int_t nPE);
  Bool_t processBeforeGen(Int_t sample);
  Bool_t processAfterFit(Int_t sample);
  RooDataSet *finalizeRun();

private:
  RooDataSet *genParData;
  RooArgSet genPars;
  RooProdPdf constraints;
  RooAbsPdf::GenSpec *genSpec;
  RooMCStudy *mcStudy;  //I got sick of not having access...

} ;

VaryParModule::VaryParModule(RooArgSet &constr, RooMCStudy *mcs) : 
  RooAbsMCStudyModule("VaryParModule","VaryParModule"), genParData(0), genPars("genPars"), 
  constraints("VaryParConstraints","VaryParConstraints",constr), genSpec(0), mcStudy(mcs) {

  genSpec = constraints.prepareMultiGen(*constraints.getVariables(),
                                        NumEvents(1));

}

VaryParModule::~VaryParModule() { 
  if (genParData) {
    delete genParData; 
    genParData = 0;
  }
  if (genSpec) {
    delete genSpec;
    genSpec = 0;
  }
}



Bool_t VaryParModule::initializeInstance() {

  //Make copies of the generated parameters with "_gen" appended to name.
  TIterator *gIter = genInitParams()->createIterator();
  RooRealVar *var = 0;
  while ((var = (RooRealVar *)gIter->Next())) {
    TString name = var->GetName();
    genPars.addOwned(*((RooRealVar *)var->Clone(name+"_gen")));
  }

  genParData = new RooDataSet("genData","Generated values",genPars);

  return true;

}

Bool_t VaryParModule::initializeRun(Int_t ) {

  genParData->reset();

  return true;

}

Bool_t VaryParModule::processBeforeGen(Int_t) {

  //First, fluctuate the generated parameters according to the PDF's supplied
  int numTry = 0;
  bool problem = false;
  TIterator *gIter = genParams()->createIterator();
  RooRealVar *var = 0;
  
  do {
    problem = false;
    RooAbsData *genParValues = constraints.generate(*genSpec);
    const RooArgSet *vals = genParValues->get();
    gIter->Reset();
    while ((var = (RooRealVar *)gIter->Next())) {     
      RooRealVar *v = (RooRealVar *)vals->find(var->GetName());
      if (v) {
        var->setVal(v->getVal());
      }
    }
    delete genParValues;

    //Check the component pdf coefficients
    TIterator *compIter = genModel()->getComponents()->createIterator();
    RooAbsArg *comp = 0;
    while ((comp = (RooAbsArg *)compIter->Next())) {

      TString compName = comp->GetName();
      if (compName.BeginsWith("num")) {
        
        RooAbsReal *numVar = (RooAbsReal *)comp;
        if (numVar->getVal() < 0) {
          problem = true;
          std::cout << "-->" << compName << " has a negative number of events: We should rethrow!" << std::endl;
        }
      }
    }

    ++numTry;
  } while (problem && numTry < 100);

  if (problem) {
    std::cout << "Unable to find a suitable set of gen model parameter values after " << numTry << " tries." << std::endl
              << "This probably indicates a serious problem.  Abort!" << std::endl;
    exit(2);
  }
  
  //Add data to the genParDatset (I could simplify this by assuming that it never changes, but I won't).
  genPars.removeAll();

  gIter->Reset();
  while ((var = (RooRealVar *)gIter->Next())) {
    TString name = var->GetName();
    genPars.addOwned(*((RooRealVar *)var->Clone(name+"_gen")));
  }
  delete gIter;


  return true;

}

Bool_t VaryParModule::processAfterFit(Int_t) {

  //I don't add here until I know for sure that the fit was successful.
  if (gMinuit->GetStatus() == 0) 
    genParData->add(genPars);
  else {

    std::cout << "Failed fit!" << std::endl;
    genParams()->Print("v");

  }
  

  return true;

}

RooDataSet *VaryParModule::finalizeRun() {

  return genParData;

}



#endif
