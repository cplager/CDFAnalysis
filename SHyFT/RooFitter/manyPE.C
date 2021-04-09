#include "Fitter.h"
#include "ZJetsTemplate.h"
#include "VaryParModule.h"

#include "RooMCStudy.h"
#include "RooDataSet.h"
#include "RooMsgService.h"


#include "TTree.h"

void manyPE(double lumi = 0.0354, int nPE = 2000,TString systLabel="") {

  //Set the luminosity
  Fitter::TemplateInfo::globalInitialization(lumi);

  //Define the binning
  Fitter::TemplateInfo::setBinning("2j","muPt_2j",20,20,120);
  Fitter::TemplateInfo::setBinning("3j","muPt_3j",10,20,120);
  Fitter::TemplateInfo::setBinning("4j","muPt_[>=4]j",10,20,120);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> fitTemplateList;

  //---TTbar Template
  Fitter::TemplateInfo *topFitTemplate = new Fitter::TemplateInfo("ttbar",
								  "templates/ttbar/shyftPretag_ttbar_38X.root",
								  1306182,0.1575,true,kRed+1);
  fitTemplateList.push_back(topFitTemplate);
  
  //---W+Jets Template
  Fitter::TemplateInfo *wFitTemplate = new Fitter::TemplateInfo("WJets",
								"templates/wjets/shyftPretag_wjets_38X.root",
								14805546,31.314,true,kGreen-3);

  fitTemplateList.push_back(wFitTemplate);

  //---Z+Jets Template
  Fitter::TemplateInfo *zFitTemplate = new Fitter::TemplateInfo("ZJets",
                                                                "templates/zjets/shyftPretag_zjets_38X.root",
                                                                2543727,3.048,true,kAzure-2);
  zFitTemplate->setConstraint(0.3,wFitTemplate);
  fitTemplateList.push_back(zFitTemplate);

  //---Single top template
  Fitter::TemplateInfo *stFitTemplate = new Fitter::TemplateInfo("SingleTop",
								 "templates/singletop/shyftPretag_singleTop_t_38X.root",
								 484060,0.3257*0.0646,true,kMagenta); //Was kAzure-12
  stFitTemplate->addTemplates("templates/singletop/shyftPretag_singleTop_tW_38X.root",
                              494961,0.0106);
  stFitTemplate->setConstraint(0.3);
  fitTemplateList.push_back(stFitTemplate);
 
//   Fitter::TemplateInfo *stFitTemplate = new Fitter::TemplateInfo("SingleTop",
// 								 "templates/singletop/shyftPretag_singleTop_merged_38X.root",
// 								 1.,1.,true,kMagenta); //Was kAzure-12
//   stFitTemplate->setConstraint(0.3);
//   fitTemplateList.push_back(stFitTemplate);

  //---QCD Template
  Fitter::TemplateInfo *qcdFitTemplate = new Fitter::TemplateInfo("QCD",
								  "templates/qcd_mc_iso/shyftPretag_qcdIsoMC_38X.root",
								  28904866,84.6793*2,true,kYellow);
  qcdFitTemplate->swapShapes("templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD.root");
  fitTemplateList.push_back(qcdFitTemplate);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> genTemplateList;

  //---TTbar Template
  TString fileName = "templates/ttbar/shyftPretag_ttbar_38X";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *topGenTemplate = new Fitter::TemplateInfo("ttbar",
                                                               fileName,
                                                               1306182,0.1575,true,kRed+1);
  genTemplateList.push_back(topGenTemplate);
  
  //---W+Jets Template
  fileName = "templates/wjets/shyftPretag_wjets_38X";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *wGenTemplate = new Fitter::TemplateInfo("WJets",
                                                             fileName,
                                                              14805546,31.314,true,kGreen-3);

  genTemplateList.push_back(wGenTemplate);

  //---Z+Jets Template
  fileName =  "templates/zjets/shyftPretag_zjets_38X";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *zGenTemplate = new Fitter::TemplateInfo("ZJets",
                                                                fileName,
                                                                2543727,3.048,true,kAzure-2);
  zGenTemplate->setConstraint(0.3,wGenTemplate);
  genTemplateList.push_back(zGenTemplate);

  //---Single Top Template
  fileName = "templates/singletop/shyftPretag_singleTop_t_38X";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *stGenTemplate = new Fitter::TemplateInfo("SingleTop",
								 fileName,
								 484060,0.3257*0.0646,true,kMagenta);
  fileName = "templates/singletop/shyftPretag_singleTop_tW_38X";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  stGenTemplate->addTemplates(fileName,494961,0.0106);
  stGenTemplate->setConstraint(0.3);
  genTemplateList.push_back(stGenTemplate);

//   //---Single Top Template
//   fileName = "templates/singletop/shyftPretag_singleTop_merged_38X";
//   if (systLabel != "") {
//     fileName += "_";
//     fileName += systLabel;
//   }
//   fileName += ".root";
//   Fitter::TemplateInfo *stGenTemplate = new Fitter::TemplateInfo("SingleTop",
// 								 fileName,
// 								 1.,1.,true,kMagenta);
//   stGenTemplate->setConstraint(0.3);
//   genTemplateList.push_back(stGenTemplate);

  //---QCD Template
  fileName = "templates/qcd_mc_iso/shyftPretag_qcdIsoMC_38X";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *qcdGenTemplate = new Fitter::TemplateInfo("QCD",
                                                               fileName,
                                                               28904866,84.6793*2,true,kYellow);
  fileName = "templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD";
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
  fitConstraints.add(stFitTemplate->getConstraints());

  RooArgSet genConstraints("genConstraints");
  genConstraints.add(zGenTemplate->getConstraints());
  genConstraints.add(stGenTemplate->getConstraints());


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
  TString outFileName = "pe_muPt";
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
