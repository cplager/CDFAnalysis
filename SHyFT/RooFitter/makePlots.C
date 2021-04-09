#include "Fitter.h"
#include "ZJetsTemplate.h"

#include "TROOT.h"
#include "TStyle.h"

void makePlots(double lumi = 0.01) {

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
  templateList.push_back(qcdTemplate);


  //Unit normalized plots
  Fitter::plotTemplates("Muon p_{T}", templateList,true);

  //Stack plots
  Fitter::plotStack("Muon p_{T}",templateList,true);

}
