
#include "Fitter.h"
#include "ZJetsTemplate.h"

#include "TROOT.h"
#include "TStyle.h"

#include "RooRandom.h"
#include "Roo1DTable.h"

#include "RooFitResult.h"
#include "RooGaussian.h"

void fitData(double lumi = 0.002961) {
  
  //Set the luminosity
  Fitter::TemplateInfo::globalInitialization(lumi);

  //Define the binning
  Fitter::TemplateInfo::setBinning("2j","htLep_2j",56,0,280);
  Fitter::TemplateInfo::setBinning("3j","muEta_3j",20,-2.1,2.1);
  Fitter::TemplateInfo::setBinning("4j","muEta_[>=4]j",20,-2.1,2.1);

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
  templateList.push_back(qcdTemplate);
  
  std::cout << "Creating PDF..." << std::endl;
  RooSimultaneous fitPDF("fitPDF","Fit PDF",*(Fitter::TemplateInfo::getJetBins()));

  Fitter::createPDF(&fitPDF, templateList);

  //Load data
  std::cout << "Loading data..." << std::endl;
  Fitter::TemplateInfo *data = new Fitter::TemplateInfo("Data",
                                                        "templates/shyftPretag_all.root",
                                                        0,0,true,kBlack);

  Fitter::plotStack("Muon p_{T}",templateList,true,"muPt_beforeFit",data);
  Fitter::plotTemplates("Muon p_{T}", templateList,true,"muPt");

  Fitter::printNEvts(templateList);

  RooDataHist *dataHist = Fitter::convertTemplateInfo(*data);

  dataHist->table(RooArgSet(*(Fitter::TemplateInfo::getVarBins()),*(Fitter::TemplateInfo::getJetBins())))->Print("v");

  RooFitResult *result = fitPDF.fitTo(*dataHist,Minos(true),ExternalConstraints(zTemplate->getConstraints()),Save());

  result->Print("v");

  //Try plotting the fit results
  Fitter::plotStack("Muon p_{T}",templateList,true,"muPt_afterFit",data);

  Fitter::printNEvts(templateList,data,result);

}
