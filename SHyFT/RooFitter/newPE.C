#include "Fitter.h"
#include "WJetsTemplate.h"
#include "ZJetsTemplate.h"
#include "VaryParModule.h"

#include "RooMCStudy.h"
#include "RooDataSet.h"
#include "RooMsgService.h"


#include "TTree.h"

void newPE(double lumi = 0.02, int nPE = 2000, TString systLabel="") {

  //Set the luminosity
  Fitter::TemplateInfo::globalInitialization(lumi);

  //Define the binning
  Fitter::TemplateInfo::setBinning("0j","met_0j",50,20,120);
  Fitter::TemplateInfo::setBinning("1j","met_1j",25,20,120);
  Fitter::TemplateInfo::setBinning("2j","met_2j",20,20,120);
  Fitter::TemplateInfo::setBinning("3j","met_3j",10,20,120);
  Fitter::TemplateInfo::setBinning("4j","met_[>=4]j",10,20,120);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> fitTemplateList;

  //---TTbar Template
  Fitter::TemplateInfo *topFitTemplate = new Fitter::TemplateInfo("ttbar",
                                                               "templates/shyftPretag_ttbar.root",
                                                               1483404,0.1575,true,kRed+1);
  fitTemplateList.push_back(topFitTemplate);
  
  //---W+Jets Template
  Fitter::WJetsTemplateInfo *wFitTemplate = new Fitter::WJetsTemplateInfo("WJets",
                                                             "templates/shyftPretag_wjets.root",
                                                             10068895,31.314,kGreen-3);

  fitTemplateList.push_back(wFitTemplate);

  //---Z+Jets Template
  Fitter::ZJetsTemplateInfo *zFitTemplate = new Fitter::ZJetsTemplateInfo("ZJets",
                                                                       "templates/shyftPretag_zjets.root",
                                                                       1084921,3.048,kAzure-2,wFitTemplate);
  fitTemplateList.push_back(zFitTemplate);

  //---QCD Template
  Fitter::TemplateInfo *qcdFitTemplate = new Fitter::TemplateInfo("QCD",
                                                               "templates/shyftPretag_qcd.root",
                                                               4377187,79.688,false,kYellow);
  qcdFitTemplate->swapShapes("templates/shyftPretag_qcd_noiso.root");  
  fitTemplateList.push_back(qcdFitTemplate);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> genTemplateList;

  //---TTbar Template
  TString fileName = "templates/shyftPretag_ttbar";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *topGenTemplate = new Fitter::TemplateInfo("ttbar",
                                                               fileName,
                                                               1483404,0.1575,true,kRed+1);
  genTemplateList.push_back(topGenTemplate);
  
  //---W+Jets Template
  fileName = "templates/shyftPretag_wjets";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *wGenTemplate = new Fitter::TemplateInfo("WJets",
                                                             fileName,
                                                             10068895,31.314,true,kGreen-3);

  genTemplateList.push_back(wGenTemplate);

  //---Z+Jets Template
  fileName = "templates/shyftPretag_zjets";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::ZJetsTemplateInfo *zGenTemplate = new Fitter::ZJetsTemplateInfo("ZJets",
                                                                       fileName,
                                                                       1084921,3.048,kAzure-2,wGenTemplate);
  genTemplateList.push_back(zGenTemplate);

  //---QCD Template
  fileName = "templates/shyftPretag_qcd";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *qcdGenTemplate = new Fitter::TemplateInfo("QCD",
                                                               fileName,
                                                               4377187,79.688,false,kYellow);
  fileName = "templates/shyftPretag_qcd_noiso";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  qcdGenTemplate->swapShapes(fileName);  
  genTemplateList.push_back(qcdGenTemplate);
  
  std::cout << "Creating Fit PDF..." << std::endl;
  RooSimultaneous fitPDF("fitPDF","Fit PDF",*(Fitter::TemplateInfo::getJetBins()));
  Fitter::createPDF(&fitPDF, fitTemplateList);

  std::cout << "Creating Gen PDF..." << std::endl;
  RooSimultaneous genPDF("genPDF","Gen PDF",*(Fitter::TemplateInfo::getJetBins()));
  Fitter::createPDF(&genPDF, genTemplateList);

  RooArgSet fitConstraints("fitConstraints");
  fitConstraints.add(wFitTemplate->getConstraints());
  fitConstraints.add(zFitTemplate->getConstraints());

  RooArgSet genConstraints("genConstraints");
  genConstraints.add(zGenTemplate->getConstraints());

  //Do some pseudo-experiments
  RooMCStudy* mcstudy = new RooMCStudy(genPDF,
                                       RooArgSet(*(Fitter::TemplateInfo::getVarBins()),*(Fitter::TemplateInfo::getJetBins())),
                                       FitModel(fitPDF),
                                       Binned(kTRUE),Silence(),Extended(),
                                       ExternalConstraints(fitConstraints),
                                       FitOptions(Save(kTRUE), PrintLevel(-1),
                                                  PrintEvalErrors(-1),Minos(kTRUE))) ;

  VaryParModule varyPMod(genConstraints,mcstudy);
  mcstudy->addModule(varyPMod);

  // Generate and fit npe samples of Poisson(nExpected) events

  //Try to suppress error messages
  //   RooMsgService::instance().setStreamStatus(0,false);
  //   RooMsgService::instance().setStreamStatus(1,false);
  for (int iStream = 0; iStream < RooMsgService::instance().numStreams(); ++iStream) {
    RooMsgService::instance().getStream(iStream).removeTopic(InputArguments);
  }

  mcstudy->generateAndFit(nPE) ;

  //Save the PE results to a tree
  TString outFileName = "newPE_met";
  if (systLabel != "") {
    outFileName += "_";
    outFileName += systLabel;
  }
  outFileName += Form("_lumi%.4f_num%d.root",lumi,nPE);
  TFile *outFile = TFile::Open(outFileName,"RECREATE");
  outFile->cd();
  const TTree *peTree = mcstudy->fitParDataSet().tree();
  peTree->Write();
  outFile->Close();

  if (systLabel == "") {
    std::cout << "**************************************" << std::endl
              << "**                                  **" << std::endl
              << "** No Syst shift was applied.       **" << std::endl
              << "** If you want a Syst shift, please **" << std::endl
              << "** set systLabel != \"\"              **" << std::endl
              << "**                                  **" << std::endl              
              << "**************************************" << std::endl;
  }


}
