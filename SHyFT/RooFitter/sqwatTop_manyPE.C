#include "Fitter.h"
#include "ZJetsTemplate.h"
#include "VaryParModule.h"

#include "RooMCStudy.h"
#include "RooDataSet.h"
#include "RooMsgService.h"


#include "TTree.h"

void manyPE(double lumi = 5.088, int nPE = 2000,TString systLabel="") {
  
  bool combineVjets = true;
   
  //Set the luminosity
  Fitter::TemplateInfo::globalInitialization(lumi);

  //Define the binning
   Fitter::TemplateInfo::setBinning("3j","h_muM3_geq3j",24,0,1200);
   //  Fitter::TemplateInfo::setBinning("4j","h_muM3_4j",25,0,1200);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> fitTemplateList;

  //---TTbar Template
  Fitter::TemplateInfo *topFitTemplate = new Fitter::TemplateInfo("ttbar",
								  "templates/ttbar/sqwatTop_ttbar_53X_fit.root",
                                   7619888,0.2252,true,kRed+1);
  fitTemplateList.push_back(topFitTemplate);
  
  //---W+Jets Template
   Fitter::TemplateInfo *wFitTemplate = new Fitter::TemplateInfo("WJets",
								"templates/wjets/sqwatTop_wjetsTot_53X_fit.root",
								57398085,36.257,true,kGreen-3);

  wFitTemplate->setConstraint(0.3);
  // fitTemplateList.push_back(wFitTemplate);

  //---Z+Jets Template
  Fitter::TemplateInfo *zFitTemplate = new Fitter::TemplateInfo("ZJets",
                                 "templates/zjets/sqwatTop_zjets_53X_fit.root",
                                   30461028,3.503,true,kAzure-2);
 
  zFitTemplate->setConstraint(0.3,wFitTemplate);
  // fitTemplateList.push_back(zFitTemplate);

  if(combineVjets == false){
    fitTemplateList.push_back(wFitTemplate);
    fitTemplateList.push_back(zFitTemplate);
  }
 
    //---V+Jets Template
    Fitter::TemplateInfo *vFitTemplate = new Fitter::TemplateInfo("VJets",
								"templates/wjets/sqwatTop_wjetsTot_53X_fit.root",
								57398085,36.257,true,kCyan+2);
   
    //Tessa add Z back in!
    // vFitTemplate->addTemplates("templates/zjets/sqwatTop_zjets_53X_fit.root",
    //                           30461028,3.503);
    vFitTemplate->setConstraint(0.3);
    if(combineVjets == true){
      fitTemplateList.push_back(vFitTemplate);
    }

    
   // ---Single top template
  Fitter::TemplateInfo *stFitTemplate = new Fitter::TemplateInfo("SingleTop",
  							 "templates/singletop/sqwatTop_singleTop_t_53X_fit.root",
                              1,1,true,kMagenta);
 
  stFitTemplate->setConstraint(0.3);
 fitTemplateList.push_back(stFitTemplate);
 


  // //---QCD Template
//    Fitter::TemplateInfo *qcdFitTemplate = new Fitter::TemplateInfo("QCD",
//                                                                   "templates/qcd_iso/sqwatTop_qcd_53X_fit.root",
//                                                                   25064715,2*84.6793,true,kYellow);

//   qcdFitTemplate->swapShapes("templates/qcd_shape/sqwatTop_qcdMC_pfi20_Mu15_53X_fit.root");
//   fitTemplateList.push_back(qcdFitTemplate);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> genTemplateList;

  //---TTbar Template
  TString fileName = "templates/ttbar/sqwatTop_ttbar_53X_fit";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *topGenTemplate = new Fitter::TemplateInfo("ttbar",
                                                               fileName,
                                                               7619888,0.2252,true,kRed+1);
  genTemplateList.push_back(topGenTemplate);
  
  //---W+Jets Template
  fileName = "templates/wjets/sqwatTop_wjetsTot_53X_fit";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *wGenTemplate = new Fitter::TemplateInfo("WJets",
                                                             fileName,
                                                             57398085,36.257,true,kGreen-3);

  
  wGenTemplate->setConstraint(0.3);
  //---Z+Jets Template
  fileName =  "templates/zjets/sqwatTop_zjets_53X_fit";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *zGenTemplate = new Fitter::TemplateInfo("ZJets",
                                                                fileName,
                                                                30461028,3.503,true,kAzure-2);
  zGenTemplate->setConstraint(0.3,wGenTemplate);
  
  if(combineVjets == false){
    genTemplateList.push_back(wGenTemplate);
    genTemplateList.push_back(zGenTemplate);
  }

 //---V+Jets Template
  fileName = "templates/wjets/sqwatTop_wjetsTot_53X_fit";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *vGenTemplate = new Fitter::TemplateInfo("VJets",
                                                             fileName,
                                                             57398085,36.257,true,kGreen-3);
  
  fileName = "templates/zjets/sqwatTop_zjets_53X_fit";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  //Tessa add Z back in!
  // vGenTemplate->addTemplates(fileName,30461028,3.503);

  vGenTemplate->setConstraint(0.3);
  if(combineVjets == true){
    genTemplateList.push_back(vGenTemplate);
  }

  
//   //---Single Top Template
  fileName = "templates/singletop/sqwatTop_singleTop_t_53X_fit";
  if (systLabel != "") {
    fileName += "_";
    fileName += systLabel;
  }
  fileName += ".root";
  Fitter::TemplateInfo *stGenTemplate = new Fitter::TemplateInfo("SingleTop",
                                                                 fileName,
                                                                 1,1,true,kMagenta);

  stGenTemplate->setConstraint(0.3);
  genTemplateList.push_back(stGenTemplate);
  

  std::cout << "Creating Fit PDF..." << std::endl;
  RooSimultaneous fitPDF("fitPDF","Fit PDF",*(Fitter::TemplateInfo::getJetBins()));
  Fitter::createPDF(&fitPDF, fitTemplateList);

  std::cout << "Creating Gen PDF..." << std::endl;
  RooSimultaneous genPDF("genPDF","Gen PDF",*(Fitter::TemplateInfo::getJetBins()));
  Fitter::createPDF(&genPDF, genTemplateList);

  RooArgSet fitConstraints("fitConstraints");
  if(combineVjets == false){
    fitConstraints.add(zFitTemplate->getConstraints());
    fitConstraints.add(wFitTemplate->getConstraints());
  }else{
    fitConstraints.add(vFitTemplate->getConstraints());
  }
  fitConstraints.add(stFitTemplate->getConstraints());
  
 
  RooArgSet genConstraints("genConstraints");
  if(combineVjets == false){
    genConstraints.add(zGenTemplate->getConstraints());
    genConstraints.add(wGenTemplate->getConstraints());
  }else{
    genConstraints.add(vFitTemplate->getConstraints());
  }
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
  TString outFileName = "pe_M3";
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
