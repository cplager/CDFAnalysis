#ifndef ROO_GENPARMODULE
#define ROO_GENPARMODULE

#include "RooAbsMCStudyModule.h"
#include "RooDataSet.h"
#include "RooRealVar.h"

#include <iostream>

class GenParModule : public RooAbsMCStudyModule {
public:

  GenParModule() : RooAbsMCStudyModule("GenParModule","GenParModule"), genParData(0), genPars("genPars") {}
  GenParModule(const GenParModule& other) : RooAbsMCStudyModule(other), 
                                            genParData((RooDataSet *)other.genParData->Clone()), genPars(other.genPars) {}
  virtual ~GenParModule() { if (genParData) delete genParData; genParData = 0;}

  Bool_t initializeInstance();
  Bool_t initializeRun(Int_t nPE);
  Bool_t processBeforeGen(Int_t sample);
  RooDataSet *finalizeRun();

private:
  RooDataSet *genParData;
  RooArgSet genPars;


} ;

Bool_t GenParModule::initializeInstance() {

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

Bool_t GenParModule::initializeRun(Int_t ) {

  genParData->reset();

  return true;

}

Bool_t GenParModule::processBeforeGen(Int_t ) {

  //Add data to the genParDatset (I could simplify this by assuming that it never changes, but I won't).
  genPars.removeAll();

  TIterator *gIter = genInitParams()->createIterator();
  RooRealVar *var = 0;
  while ((var = (RooRealVar *)gIter->Next())) {
    TString name = var->GetName();
    genPars.addOwned(*((RooRealVar *)var->Clone(name+"_gen")));
  }

  genParData->add(genPars);

  return true;

}

RooDataSet *GenParModule::finalizeRun() {

  return genParData;

}



#endif
