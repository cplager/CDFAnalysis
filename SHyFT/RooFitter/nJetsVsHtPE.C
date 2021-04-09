#include "Fitter.h"
#include "ZJetsTemplate.h"
#include "GenParModule.h"

#include "RooMCStudy.h"
#include "RooDataSet.h"
#include "RooMsgService.h"


#include "TTree.h"

void nJetsVsHtPE(double lumi = 0.02, int nPE = 2000,TString systLabel="") {

  //Set the luminosity
  Fitter::TemplateInfo::globalInitialization(lumi);

  //Define the binning
  Fitter::TemplateInfo::setBinning("1j","ht_2j",20,0,160);
  Fitter::TemplateInfo::setBinning("2j","ht_2j",20,0,160);
  Fitter::TemplateInfo::setBinning("3j","ht_3j",10,0,160);
  Fitter::TemplateInfo::setBinning("4j","ht_4j",8,0,128);
  Fitter::TemplateInfo::setBinning("5j","ht_[>=5]j",8,0,128);

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

  
  std::cout << "Creating Fit PDF..." << std::endl;
  RooSimultaneous fitPDF("fitPDF","Fit PDF",*(Fitter::TemplateInfo::getJetBins()));
  Fitter::createPDF(&fitPDF, fitTemplateList);

  std::cout << "Creating Gen PDF..." << std::endl;
  RooSimultaneous genPDF("genPDF","Gen PDF",*(Fitter::TemplateInfo::getJetBins()));
  Fitter::createPDF(&genPDF, genTemplateList);


  //Do some pseudo-experiments
  RooMCStudy* mcstudy = new RooMCStudy(genPDF,
                                       RooArgSet(*(Fitter::TemplateInfo::getVarBins()),*(Fitter::TemplateInfo::getJetBins())),
                                       FitModel(fitPDF),
                                       Binned(kTRUE),Silence(),Extended(),
                                       FitOptions(Save(kTRUE), PrintLevel(-1),
                                                  PrintEvalErrors(-1),Minos(kTRUE))) ;

  // Generate and fit npe samples of Poisson(nExpected) events

  //Try to suppress error messages
  //   RooMsgService::instance().setStreamStatus(0,false);
  //   RooMsgService::instance().setStreamStatus(1,false);
  for (int iStream = 0; iStream < RooMsgService::instance().numStreams(); ++iStream) {
    RooMsgService::instance().getStream(iStream).removeTopic(InputArguments);
  }

  GenParModule genParMod;
  mcstudy->addModule(genParMod);

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
  TString outFileName = "nJetsVsHtPE";
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
