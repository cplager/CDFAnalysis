
#include "Fitter.h"
#include "ZJetsTemplate.h"

#include "TROOT.h"
#include "TStyle.h"

#include "RooRandom.h"
#include "Roo1DTable.h"

void onePE(double lumi = 0.02) {

  //Set the luminosity
  Fitter::TemplateInfo::globalInitialization(lumi);

  //Define the binning
  Fitter::TemplateInfo::setBinning("2j","muPt_2j",20,20,120);
  Fitter::TemplateInfo::setBinning("3j","muPt_3j",10,20,120);
  Fitter::TemplateInfo::setBinning("4j","muPt_[>=4]j",10,20,120);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> templateList;

  //---TTbar Template
  Fitter::TemplateInfo *topTemplate = new Fitter::TemplateInfo("ttbar",
                                                               "templates/shyftPretag_ttbar.root",
                                                               1483404,0.1575,true,kRed+1);
  templateList.push_back(topTemplate);
  
  //---W+Jets Template
  Fitter::TemplateInfo *wTemplate = new Fitter::TemplateInfo("WJets",
                                                             "templates/shyftPretag_wjets.root",
                                                             10068895,31.314,true,kGreen-3);

  templateList.push_back(wTemplate);

  //---Z+Jets Template
  Fitter::ZJetsTemplateInfo *zTemplate = new Fitter::ZJetsTemplateInfo("ZJets",
                                                                       "templates/shyftPretag_zjets.root",
                                                                       1084921,3.048,kAzure-2,wTemplate);
  templateList.push_back(zTemplate);

  //---QCD Template
  Fitter::TemplateInfo *qcdTemplate = new Fitter::TemplateInfo("QCD",
                                                               "templates/shyftPretag_qcd.root",
                                                               4377187,79.688,false,kYellow);
  qcdTemplate->swapShapes("templates/shyftPretag_qcd_noiso.root");  
  templateList.push_back(qcdTemplate);
  
  std::cout << "Creating PDF..." << std::endl;
  RooSimultaneous fitPDF("fitPDF","Fit PDF",*(Fitter::TemplateInfo::getJetBins()));

  Fitter::createPDF(&fitPDF, templateList);


  //Generate a random dataset
  double nExpGen = fitPDF.expectedEvents(RooArgSet(*(Fitter::TemplateInfo::getVarBins()),*(Fitter::TemplateInfo::getJetBins())));
  int nEvt = RooRandom::randomGenerator()->Poisson(nExpGen);

  RooDataHist *peData = fitPDF.generateBinned(RooArgSet(*(Fitter::TemplateInfo::getVarBins()),*(Fitter::TemplateInfo::getJetBins())),nEvt);
  
  std::cout << "Generated pseudo-data:" << std::endl;
  peData->table(RooArgSet(*(Fitter::TemplateInfo::getVarBins()),*(Fitter::TemplateInfo::getJetBins())))->Print("v");

  //Try generating a set of histograms from the pseudo-data
  Fitter::TemplateInfo *data = Fitter::convertRooDataHist(*peData);

  plotStack("Muon p_{T}",templateList,true,"beforeFit",data);

  //Now run the fit
  RooFitResult *result = fitPDF.fitTo(*peData,Minos(true),ExternalConstraints(zTemplate->getConstraints()),Save());
  result->Print("v");

  //Try plotting the fit results
  plotStack("Muon p_{T}",templateList,true,"afterFit",data);


}
