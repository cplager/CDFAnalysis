#include "Fitter.h"
#include "ZJetsTemplate.h"
#include "VaryParModule.h"

#include "RooMCStudy.h"
#include "RooDataSet.h"
#include "RooMsgService.h"
#include "GenParModule.h"

#include "TTree.h"

void simplePEs(double lumi = 0.02, int nPE = 2000,TString systLabel="") {

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
                                                               "templates/shyftPretag_ttbar_PF_Pt25.root",
                                                               1483404,0.1575,true,kRed+1);
  fitTemplateList.push_back(topFitTemplate);
  
  //---W+Jets Template
  Fitter::TemplateInfo *wFitTemplate = new Fitter::TemplateInfo("WJets",
                                                             "templates/shyftPretag_wjets_PF_Pt25.root",
                                                             10068895,31.314,true,kGreen-3);

  fitTemplateList.push_back(wFitTemplate);

  //---QCD Template
  Fitter::TemplateInfo *qcdFitTemplate = new Fitter::TemplateInfo("QCD",
                                                               "templates/shyftPretag_qcd_PF_Pt25.root",
                                                               4377187,2*79.688,false,kYellow);

  
  //qcdFitTemplate->swapShapes("templates/shyftPretag_qcd_noiso.root");  
  fitTemplateList.push_back(qcdFitTemplate);



  
  std::cout << "Creating Fit PDF..." << std::endl;
  RooSimultaneous fitPDF("fitPDF","Fit PDF",*(Fitter::TemplateInfo::getJetBins()));
  Fitter::createPDF(&fitPDF, fitTemplateList);

  //Do some pseudo-experiments

  RooMCStudy* mcstudy = new RooMCStudy(fitPDF,
                                       RooArgSet(*(Fitter::TemplateInfo::getVarBins()),*(Fitter::TemplateInfo::getJetBins())),
                                       Binned(kTRUE),Silence(),Extended(),
                                       FitOptions(Save(kTRUE), PrintLevel(-1),
                                                  PrintEvalErrors(-1),Minos(kTRUE),Strategy(2))) ;

  //VaryParModule varyPMod(genConstraints,mcstudy);
  //mcstudy->addModule(varyPMod);

  GenParModule genpm;

  mcstudy->addModule(genpm);

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
