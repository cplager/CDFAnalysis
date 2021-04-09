
#include "Fitter.h"
#include "ZJetsTemplate.h"
#include "VaryParModule.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "THStack.h"

#include "RooRandom.h"
#include "Roo1DTable.h"

#include "RooFitResult.h"
#include "RooGaussian.h"

#include "RooMCStudy.h"
#include "RooDataSet.h"
#include "RooMsgService.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"

void oldPE(double lumi = 0.02, int nPE = 2000,TString systLabel="") {

  //Set the luminosity
  Fitter::TemplateInfo::globalInitialization(lumi);

  //Define the binning
  Fitter::TemplateInfo::setBinning("4j","ht_[>=4]j",10,140,640);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> fitTemplateList;

  //---TTbar Template
  Fitter::TemplateInfo *topFitTemplate = new Fitter::TemplateInfo("ttbar",
                                                               "templates/shyftPretag_ttbar.root",
                                                               1483404,0.1575,true,kRed+1);
  fitTemplateList.push_back(topFitTemplate);
  
  //---W+Jets Template
  Fitter::TemplateInfo *wFitTemplate = new Fitter::TemplateInfo("WJets",
                                                             "templates/shyftPretag_wjets.root",
                                                             10068895,31.314,true,kGreen-3);

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
  fitConstraints.add(zFitTemplate->getConstraints());

  RooArgSet genConstraints("genConstraints");
  genConstraints.add(zGenTemplate->getConstraints());

  //----------------------  QCD Constraint -------------------------------- 
  RooAbsReal *qcdXSPar = (RooAbsReal *)qcdFitTemplate->fitPars.first();
  double qcdCentral = qcdXSPar->getVal();
  double qcdUncert = 0.4 * qcdCentral;
  RooGaussian *qConstraint = new RooGaussian("qConstraint",
                                              "Constraint on QCD",
                                              *qcdXSPar, RooConst(qcdCentral), RooConst(qcdUncert));

  fitConstraints.add(*qConstraint);
  genConstraints.add(*qConstraint);

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


  //Unit normalized plots
  Fitter::plotTemplates ("ht", fitTemplateList,true);

  cout << "======= Event Info For PE Gen Tempaltes ========" << endl;
  
  Fitter::printNEvts (genTemplateList);

  cout << "======= Event Info For Fit Tempaltes ========" << endl;
  
  Fitter::printNEvts (fitTemplateList);
  

  for (int iStream = 0; iStream < RooMsgService::instance().numStreams(); ++iStream) {
    RooMsgService::instance().getStream(iStream).removeTopic(InputArguments);
  }

  //Run PE's
  mcstudy->generateAndFit(nPE) ;

  if (systLabel == "") {
    std::cout << "**************************************" << std::endl
              << "**                                  **" << std::endl
              << "** No SYST shift applied.           **" << std::endl
              << "** If you want a SYST shift, please **" << std::endl
              << "** set systLabel != \"\"              **" << std::endl
              << "**                                  **" << std::endl              
              << "**************************************" << std::endl;
  }

  //Save the PE results to a tree
  TString outFileName = "jesHtPE";
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



  


}
