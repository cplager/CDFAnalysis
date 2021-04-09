
#include "Fitter.h"

#include "TROOT.h"
#include "TStyle.h"

#include "RooRandom.h"
#include "Roo1DTable.h"

#include "RooFitResult.h"
#include "RooGaussian.h"

void fitData(double lumi = 1.492) {
  
  //Set the luminosity
  Fitter::TemplateInfo::globalInitialization(lumi);

  //Define the binning
  // Fitter::TemplateInfo::setBinning("2j","muPt_2j",20,20,120);
  Fitter::TemplateInfo::setBinning("3j","muPt_3j",8,30,190);
  Fitter::TemplateInfo::setBinning("4j","muPt_[>=4]j",8,30,190);

  //Make a list of templates to use in the fit
  std::cout << "Initializing list of templates..." << std::endl;
  std::vector<Fitter::TemplateInfo *> templateList;

 

  //---TTbar Template
  Fitter::TemplateInfo *topFitTemplate = new Fitter::TemplateInfo("ttbar",
								  "templates/ttbar/sqwatTop_ttbar_42X.root",
								  3701947,0.1575,true,kRed+1);
  //topFitTemplate->setScaleFactor(0.965);
  templateList.push_back(topFitTemplate);
  
  //---W+Jets Template
  Fitter::TemplateInfo *wFitTemplate = new Fitter::TemplateInfo("WJets",
								"templates/wjets/sqwatTop_wjets_42X.root",
								81352581,31.314,true,kGreen-3);

  templateList.push_back(wFitTemplate);

  // ---Z+Jets Template
   Fitter::TemplateInfo *zFitTemplate = new Fitter::TemplateInfo("ZJets",
                                                             "templates/zjets/sqwatTop_zjets_42X.root",
                                                             36277961,3.048,true,kAzure-2);
   zFitTemplate->setConstraint(0.3,wFitTemplate);
  templateList.push_back(zFitTemplate);


  //---Single top template
  Fitter::TemplateInfo *stFitTemplate = new Fitter::TemplateInfo("SingleTop",
  							 "templates/singletop/sqwatTop_singleTop_tChan_42X.root",
  							 3900171,0.32442*0.063,true,kMagenta); //Was kAzure-12
  stFitTemplate->addTemplates("templates/singletop/sqwatTop_singleTop_tWChan_42X.root",
                           814390,0.0106);
  stFitTemplate->addTemplates("templates/singletop/sqwatTop_singleTop_sChan_42X.root",
                           259971,0.32442*0.0046);
  stFitTemplate->addTemplates("templates/singletop/sqwatTop_singleTbar_tChan_42X.root",
                           1944826,0.32442*0.063);
  stFitTemplate->addTemplates("templates/singletop/sqwatTop_singleTbar_tWChan_42X.root",
                           809984,0.0106);
  stFitTemplate->addTemplates("templates/singletop/sqwatTop_singleTbar_sChan_42X.root",
                           137980,0.32442*0.0046);
  stFitTemplate->setConstraint(0.3);
  templateList.push_back(stFitTemplate);

//   //---tW single top template
//   Fitter::TemplateInfo *stFitTemplate = new Fitter::TemplateInfo("SingleTop",
// 								 "templates/singletop/shyftPretag_singleTop_merged_38X.root",
// 								 1,1.0,true,kMagenta); //ccn: change this color
//   stFitTemplate->setConstraint(0.3);
//   templateList.push_back(stFitTemplate);
 

  //---QCD Template
  Fitter::TemplateInfo *qcdFitTemplate = new Fitter::TemplateInfo("QCD",
                                                                  "templates/qcd_iso/sqwatTop_qcd_42X.root",
                                                                  25064715,2*84.6793,true,kYellow);
  topFitTemplate->setScaleFactor(1);
  qcdFitTemplate->swapShapes("templates/qcd_shape/sqwatTop_qcdMC_pfi20_Mu15_42X.root");
  templateList.push_back(qcdFitTemplate);
  
  // cout<< "QCD scale Factor " << qcdFitTemplate->getScaleFactor() << endl;
//   //---QCD Template
//   Fitter::TemplateInfo *qcdFitTemplate = new Fitter::TemplateInfo("QCD",
//                                                                 "templates/qcd_mc_iso/shyftPretag_qcdIsoMC_38X.root",
// 								  28904866,84.6793*2,true,kYellow);
//   //qcdFitTemplate->swapShapes("templates/qcd_noiso/shyftPretag_qcdNonIso_38X.root");
//   qcdFitTemplate->swapShapes("templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD.root");
//   templateList.push_back(qcdFitTemplate);

 //----------------------------------------------------------------------------------------------

  std::cout << "Creating PDF..." << std::endl;
  RooSimultaneous fitPDF("fitPDF","Fit PDF",*(Fitter::TemplateInfo::getJetBins()));

  Fitter::createPDF(&fitPDF, templateList);

  //Load data
  std::cout << "Loading data..." << std::endl;
  Fitter::TemplateInfo *data = new Fitter::TemplateInfo("Data",
                                                        "templates/data/sqwatTop_data_42X.root",
                                                        0,0,true,kBlack);

  Fitter::plotStack("Muon p_{T}",templateList,true,"muPt_beforeFit",data);
  Fitter::plotTemplates("Muon p_{T}", templateList,true,"muPt");

  Fitter::printNEvts(templateList);

  RooArgSet fitConstraints("fitConstraints");
  fitConstraints.add(zFitTemplate->getConstraints());
  fitConstraints.add(stFitTemplate->getConstraints());

  RooDataHist *dataHist = Fitter::convertTemplateInfo(*data);

  dataHist->table(RooArgSet(*(Fitter::TemplateInfo::getVarBins()),*(Fitter::TemplateInfo::getJetBins())))->Print("v");

  RooFitResult *result = fitPDF.fitTo(*dataHist,Minos(true),ExternalConstraints(fitConstraints),Save());

  result->Print("v");

  //Try plotting the fit results
  Fitter::plotStack("Muon p_{T}",templateList,true,"muPt_afterFit",data);

  Fitter::printNEvts(templateList,data,result);
 


  //Now add some plots to draw after the fit!

}
