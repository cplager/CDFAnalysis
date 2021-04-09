// This file contains a set of basic helper functions to simplify
// doing NJets vs X fits
//
//  Authors:
//     Kevin Lannon (2010/05/24)

#ifndef NJETS2DHELPER
#define NJETS2DHELPER

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "TObject.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "THStack.h"
#include "TIterator.h"
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooRealConstant.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooBinning.h"
#include "Roo1DTable.h"
#include "RooNumber.h"
#include "RooGaussian.h"


#include <iostream>
#include <vector>
#include <map>

using namespace RooFit ;

namespace Fitter {

  //If you just want to use the class, then take a look at the information (especially comments) below:
  class BinInfo;

  //Utility class for managing the templates
  class TemplateInfo {
    
  public:

    // ***Call this before making any TemplateInfo objects to set some common features***
    //We're just going to hold a pointer to the jet bins, so be careful not to delete it later.
    //We don't take ownership of the jet bin object here.  (You still have to delete it if necessary.)
    //Luminosities are in fb^-1.
    static void globalInitialization(double lumi);
    
    //Set up any rebinning you'd like to have happen with the template histograms
    // ***Must be called prior to making any TemplateInfo objects since it wouldn't make
    //sense to change binnings mid-stream.***
    //If you don't specify a rebinning, then the only "rebinning" that happens is to move
    //overflows and underflows inside the histogram.
    static void setBinning(TString binName, TString histName, int nBins, double xLow, double xHigh);

    //Define a template including all the necessary information to properly normalize
    //the template for the values we expect (i.e. nGenEvents, crossSection, and lumi).
    //fixedNjets = true means that relative to one another, the jet bins don't vary.
    //fixedNjets = false means that the jet bins normalizations float relative to one another.
    TemplateInfo(const TString &name, 
                 const TString &inFileName, 
                 int nGenEvents, double crossSection, bool fixedNJets, int color);

    //Empty constructor
    TemplateInfo(const TString &name = "none");
    //Copy construtor
    TemplateInfo(const TemplateInfo &t);
    //Assignment
    TemplateInfo &operator=(const TemplateInfo &rhs);

    //If there is a scale factor (say on the muon trigger efficiency, etc.
    //you can set it here.
    //Note:  Call this before you set constraints, or they might be off in the case of floating Njets templates...
    void setScaleFactor(double sf);

    //This returns the scale factor variable for you
    const RooRealVar &getScaleFactor() const { return scaleFactor;}

    //This is for swapping out the template shapes (while keeping the normalizations from the
    //original sample
    void swapShapes(const TString &fileName);

    //Set a constraint on this parameter.  If the optional TemplateInfo * argument
    //is provided, constrain this template relative to another template.
    //Note:  Frac = 0.3 means constrain the parameter to 30% of the default value
    //When constraining relative to another template, frac = 0.3 means constrain the ratio to 30% of the default
    //ratio.
    //Finally, when doing a relative constraint to another template the two templates must be of the same type
    //(either both fixed or both floating Njets) or else an error will be generated.
    virtual void setConstraint(double frac, TemplateInfo *refTempl = 0);

    //Functions to return information about the constraints
    RooArgSet &getConstraints() {return parConstraints;}
    RooArgSet &getConstrainedPars() {return constrainedPars;}

    //Merge additional shapes from another file
    void addTemplates(const TString &inFileName, 
                      int nGenEvents, double crossSection);

    //The destructor...
    virtual ~TemplateInfo();

    //Gets the luminosity to which these templates correspond. 
    static double getLuminosity() { return lumi;}

    //Functions to figure out what is the largest number of histogram 
    //bins for the template in any jet bin, and the binning information for any 
    //jet bins.
    static RooCategory* getJetBins() {return jetBin;}
    static RooCategory* getVarBins() {return varBin;}
    static int maxNBins() {return maxBins;}
    static TString getHistName(const TString jetBinName);

    //Information about this template
    TString name;
    int nGenEvts;
    double crossSection;
    bool fixedNJets;
    RooArgSet fitPars;
    std::map<TString,TH1 *> histMap;
    int color;

    //These are constrained parameters
    RooArgSet constrainedPars;
    RooArgSet parConstraints;

  private:

    //Used to hold the effects of acceptance or effeciency SF
    RooRealVar scaleFactor;

    //Used for rebinning the templates, if desired.
    static std::map<TString,BinInfo> rebinInfo;
    static bool lastBinInclusive;
    static bool initialized;
    static bool alreadyMade;
    static void rebin(std::map<TString,TH1 *> *map);
    static double lumi;
    static RooCategory *jetBin;
    static RooCategory *varBin;
    static int maxBins;

  };


  //Functions for setting up the fit and looking at the results
  void createPDF(RooSimultaneous *pdf, 
                 const std::vector<TemplateInfo *> &templateList);

  //Plots the templates (unit normalized) for each jet bin
  void plotTemplates(const TString &xTitle, const std::vector<TemplateInfo *> &templates, 
                     bool save = false, const char *plotLabel = 0);

  //Makes a stack plot normalized according to the current values
  //of the fit parameters.
  void plotStack(const TString &xTitle, std::vector<TemplateInfo *> templates, 
                 bool save = false, const char *plotLabel = 0, TemplateInfo *data = 0);

  //Handy function to print out the number of events in each jet bin
  //Pass in a non-null pointer to data in order to include data in the table
  //Pass in a non-null pointer to the fit result in order to put fit errors on the table.
  void printNEvts(std::vector<TemplateInfo *> templates,  
                  TemplateInfo *data = 0, const RooFitResult *fitResult = 0);
  

  //Convert a RooDataHist of NJets vs Bin into a TemplateInfo object with histograms
  //You should delete the TemplateInfo object where you're done with it.
  TemplateInfo *convertRooDataHist(const RooDataHist &data);


  //Convert a TemplateInfo object into a RooDataHist
  //You should delete the RooDataHist when you're done with it.
  RooDataHist *convertTemplateInfo(const RooDataHist &data);

  //Saves the templates to a histogram file
  void saveTemplates(TString fileName, const std::vector<TemplateInfo *> &templates, 
                     bool normalized = false);


  //----------------------------------------------
  //Everything below here is for the internal implementation.
  //You don't have to look here unless you want to get into the guts of what's going on.


  //Utility class for managing binning information
  class BinInfo {

  public:
    TString name;
    TString histName;
    int nBins;
    double xLow;
    double xHigh;

    BinInfo() : name(""), histName(""), nBins(0), xLow(0), xHigh(0) {}

    BinInfo(TString bName, TString hName, int nB, double xLo, double xHi) :
      name (bName), histName(hName), nBins(nB), xLow(xLo), xHigh(xHi) {}

  };
  
  //To convert from pb to fb or fb^-1 to pb^-1
  const double lumiConvFactor = 1e6;  


  //Implementation for TemplateInfo functions
  //Will be set to true the first time the constructor is called.
  
  std::map<TString,BinInfo> TemplateInfo::rebinInfo = std::map<TString,BinInfo>();

  bool TemplateInfo::lastBinInclusive = false;

  bool TemplateInfo::initialized = false;

  bool TemplateInfo::alreadyMade = false;  

  double TemplateInfo::lumi = 0.0;

  RooCategory *TemplateInfo::jetBin = 0;
  RooCategory *TemplateInfo::varBin = 0;

  int TemplateInfo::maxBins = -999;

  void TemplateInfo::globalInitialization(double l) {

    if (alreadyMade) {
      std::cerr << "You should call this function before instantiating any TemplateInfo objects."
                << std::endl;
      exit(2);
    }

    lumi = l;
    jetBin = new RooCategory("jetBin","jetBin");

    varBin = new RooCategory("varBin","varBin");


    initialized = true;
  }

  void TemplateInfo::setBinning(TString bName, TString hName, int nBins, double xLow, double xHigh) {

    if (alreadyMade) {
      std::cerr << "You cannot change the binning once you start making templates.  Please make"
                << " sure that all \"setBinning\" calls come before you make any \"TemplateInfo\" objects"
                << std::endl;
      exit(2);
    }

    jetBin->defineType(bName);

    rebinInfo[bName] = BinInfo(bName,hName,nBins,xLow,xHigh);

    if (hName.Contains(">=")) lastBinInclusive = true;

  }

  TemplateInfo::TemplateInfo(const TString &nm, const TString &inFileName, 
                             int nGE, double xs,
                             bool fixNJ, int c):
    name (nm), nGenEvts(nGE), 
    crossSection(xs), fixedNJets(fixNJ), fitPars(name+"_FitPars"),
    color(c), scaleFactor(name+"ScaleFactor",name+"ScaleFactor",1.0) {

    if (!initialized) {
      std::cerr << "Before initializing any TemplateInfo objects, you need to call"
                << " the globalInitialization function." << std::endl;
      exit(2);
    }
        
    alreadyMade = true;

    std::cout << "-->Getting templates for " << name 
              << " from file " << inFileName << std::endl;
    
    TFile *histFile = TFile::Open(inFileName);
    if (!histFile) {
      std::cerr << "Error!  Cannot open file " << inFileName << std::endl;
      exit(2);
    }

    //Make a copy of the templates
    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;

    while ((iJetBin = (RooCatType *)jetIter->Next())) {

      std::cout << "  ----->JetBin = " << iJetBin->GetName() << std::endl;

      BinInfo &binInfo = rebinInfo[iJetBin->GetName()];

      TString hName = binInfo.histName;

      std::cout << "  -----> hName = " << hName << std::endl;

      TH1 *hist = 0;

      //Check to see if this histogram is for an inclusive jet bin
      if (hName.Contains("[>=")) {

        std::cout << "  -----> Inclusive Histogram!..." << std::endl;

        //We have to sum over a number of histograms
        TString nameTemplate = hName;

        //Extract the starting number
        int ind1 = nameTemplate.Index("[>=") + 3;
        int ind2 = nameTemplate.Index("]");
        int iNum = atoi(nameTemplate(ind1,ind2-ind1).Data());
       
        //Replace "[>=N]" with %d
        nameTemplate.Replace(ind1-3,ind2-ind1+4,"%d");

        TString hNameReplaced = Form(nameTemplate,iNum);

        std::cout << "  -----> nameTemplate = " << nameTemplate << ", iNum = " << iNum 
                  << ", hNameReplaced = " << hNameReplaced << std::endl;

        //Grab the first histogram
        TObject *obj = histFile->Get(hNameReplaced);
        if (!obj) {
          std::cerr << "In TemplateInfo constructor: Cannot find histogram " 
                    << hNameReplaced
                    << " in file " << histFile
                    << " for " << name << " template." << std::endl
                    << "Note:  Inclusive >= N histogram adding..." 
                    << std::endl;
          exit(2);
        }

        //Make a copy
        TString clName = hNameReplaced + name;
        hist = (TH1 *)obj->Clone(clName);

        ++iNum;
        TString hName2 = Form(nameTemplate,iNum);
        TH1 *hist2 = (TH1 *)histFile->Get(hName2);
        while (hist2) {
          std::cout << "  -----> iNum = " << iNum 
                    << ", hName2 = " << hName2 << std::endl;
          hist->Add(hist2);
          ++iNum;
          hName2 = Form(nameTemplate,iNum);
          hist2 = (TH1 *)histFile->Get(hName2);
        }
      } else {

        std:: cout << "  -----> Getting " << hName << "..." << std::endl;
        TObject *obj = histFile->Get(hName);
        if (!obj) {
          std::cerr << "In TemplateInfo constructor: Cannot find histogram " << hName
                    << " in file " << histFile
                    << " for " << name << " template." << std::endl;
          exit(2);
        }

        //Make a copy
        TString clName = hName + name;
        hist = (TH1 *)obj->Clone(clName);
      }

      hist->SetDirectory(0);
      histMap[iJetBin->GetName()] = hist;
    }

    //Done accessing the histogram file
    histFile->Close();
    delete histFile;
    histFile = 0;

    //Now let's do any fixing up of bins that I need to do
    rebin(&histMap);

    //Finally, define the fit parameters related to this template
    if (fixedNJets) {
    
      TString parName = "xs";
      parName += name;
      fitPars.addClone(RooRealVar(parName,parName,
                                  crossSection,
//                                   0,20*crossSection));
                                  -10*crossSection,20*crossSection));
    } else {

      for (std::map<TString,TH1 *>::iterator jetBinIter = histMap.begin();
           jetBinIter != histMap.end(); ++jetBinIter) {

        const TString &jetBinName = jetBinIter->first;
        const TH1 *hist = jetBinIter->second;

        double numInBin = hist->Integral();
        if (numInBin > 0) { //Only do for filled bins

          TString safeName = jetBinName;
          if (safeName.Contains(">=")) {
            safeName.Remove(safeName.Index(">="),2);
          }

          TString parName = "num";
          parName += name;
          parName += safeName;
          double sf = scaleFactor.getVal();
          double nExp = numInBin*sf*crossSection*lumi*Fitter::lumiConvFactor/nGenEvts;
          fitPars.addClone(RooRealVar(parName,parName,nExp, 
                                      -100*nExp,100*nExp));
        }
      }
      
    }
      
    fitPars.Print("V");

  }

  TemplateInfo::TemplateInfo(const TString &nm) :
    name (nm), nGenEvts(0), 
    crossSection(0), fixedNJets(false), fitPars(name+"_FitPars"),
    color(0), scaleFactor(name+"ScaleFactor",name+"ScaleFactor",1.0) {

    if (!initialized) {
      std::cerr << "Before initializing any TemplateInfo objects, you need to call"
                << " the globalInitialization function." << std::endl;
      exit(2);
    }

    alreadyMade = true;

    //Make a copy of the templates
    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;

    while ((iJetBin = (RooCatType *)jetIter->Next())) {        
      //Get binning information
      std::map<TString,BinInfo>::iterator rebinIter = rebinInfo.find(iJetBin->GetName());
      if (rebinIter == rebinInfo.end()) {
        std::cout << "Error: Missing bin info in TemplateInfo()!" << std::endl
                  << "Aborting!" << std::endl;
        exit(2);
      }
      const BinInfo &binInfo = rebinIter->second;
      int nBin = binInfo.nBins;
      double xMin = binInfo.xLow;
      double xMax = binInfo.xHigh;

      TString hName = binInfo.histName;

      //Check to see if this histogram is for an inclusive jet bin
      if (hName.Contains("[>=")) {

        //We have to sum over a number of histograms
        TString nameTemplate = hName;

        //Extract the starting number
        int ind1 = nameTemplate.Index("[>=") + 3;
        int ind2 = nameTemplate.Index("]");
        int iNum = atoi(nameTemplate(ind1,ind2-ind1).Data());
       
        //Replace "[>=N]" with %d
        nameTemplate.Replace(ind1-3,ind2-ind1+4,"%d");

        hName = Form(nameTemplate,iNum);
      }

      TH1 *hist = new TH1F(hName,hName,nBin,xMin,xMax);
      hist->SetDirectory(0);
      histMap[iJetBin->GetName()] = hist;
    }

    //No fit parameters
  }

  TemplateInfo::TemplateInfo(const TemplateInfo &t) :
    name (t.name), nGenEvts(t.nGenEvts), 
    crossSection(t.crossSection), fixedNJets(t.fixedNJets), fitPars(name+"_FitPars"),
    color(t.color), scaleFactor(t.scaleFactor) {

    if (!initialized) {
      std::cerr << "Before initializing any TemplateInfo objects, you need to call"
                << " the globalInitialization function." << std::endl;
      exit(2);
    }

    alreadyMade = true;

    //Make a copy of the templates
    for (std::map<TString, TH1 *>::const_iterator iter = t.histMap.begin();
         iter != t.histMap.end(); ++iter) {

      TH1 *hist = (TH1 *)iter->second->Clone();
      hist->SetDirectory(0);
      histMap[iter->first] = hist;
    }

    //Copy the fit parameters
    TIterator *fpIter = t.fitPars.createIterator();
    RooAbsReal *iFitPar = 0;

    while ((iFitPar = (RooAbsReal *)fpIter->Next())) {
      fitPars.addClone(*((RooAbsReal *)iFitPar->Clone()));
    }
    
  }

  TemplateInfo &TemplateInfo::operator=(const TemplateInfo &rhs) {

    name = rhs.name;
    nGenEvts = rhs.nGenEvts;
    crossSection = rhs.crossSection;
    fixedNJets = rhs.fixedNJets;
    fitPars = RooArgSet(name+"_FitPars");
    color = rhs.color;
    scaleFactor.setVal(rhs.scaleFactor.getVal());

    //Make a copy of the templates
    for (std::map<TString, TH1 *>::const_iterator iter = rhs.histMap.begin();
         iter != rhs.histMap.end(); ++iter) {

      TH1 *hist = (TH1 *)iter->second->Clone();
      hist->SetDirectory(0);
      histMap[iter->first] = hist;
    }

    //Copy the fit parameters
    TIterator *fpIter = rhs.fitPars.createIterator();
    RooAbsReal *iFitPar = 0;

    while ((iFitPar = (RooAbsReal *)fpIter->Next())) {
      fitPars.addClone(*((RooAbsReal *)iFitPar->Clone()));
    }
    
    return *this;

  }

  void TemplateInfo::setScaleFactor(double sf) {

    scaleFactor.setVal(sf);
    if (! fixedNJets) {
      //Copy the fit parameters
      TIterator *fpIter = fitPars.createIterator();
      RooAbsReal *iFitPar = 0;
      
      while ((iFitPar = (RooAbsReal *)fpIter->Next())) {
        //Check to see if this is a RooRealVal that can have its value set
        RooRealVar *realPar = dynamic_cast<RooRealVar *>(iFitPar);
        if (realPar) realPar->setVal(sf*realPar->getVal());
      }

    }
  }
      

  void TemplateInfo::swapShapes(const TString &fileName) {

    TFile *histFile = TFile::Open(fileName);
    if (!histFile) {
      std::cerr << "Error!  Cannot open file " << fileName << std::endl;
      exit(2);
    }

    //Make a copy of the templates
    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;
      
    while ((iJetBin = (RooCatType *)jetIter->Next())) {

      BinInfo &binInfo = rebinInfo[iJetBin->GetName()];

      TString hName = binInfo.histName;

      TH1 *hist = 0;

      //Check to see if this histogram is for an inclusive jet bin
      if (hName.Contains("[>=")) {
        
        //We have to sum over a number of histograms
        TString nameTemplate = hName;

        //Extract the starting number
        int ind1 = nameTemplate.Index("[>=") + 3;
        int ind2 = nameTemplate.Index("]");
        int iNum = atoi(nameTemplate(ind1,ind2-ind1).Data());
       
        //Replace "[>=N]" with %d
        nameTemplate.Replace(ind1-3,ind2-ind1+4,"%d");

        TString hNameReplaced = Form(nameTemplate,iNum);

        //Grab the first histogram
        TObject *obj = histFile->Get(hNameReplaced);
        if (!obj) {
          std::cerr << "In TemplateInfo constructor: Cannot find histogram " 
                    << hNameReplaced
                    << " in file " << histFile
                    << " for " << name << " template." << std::endl
                    << "Note:  Inclusive >= N histogram adding..." 
                    << std::endl;
          exit(2);
        }

        //Make a copy
        TString clName = hNameReplaced + name;
        hist = (TH1 *)obj->Clone(clName);

        ++iNum;
        TString hName2 = Form(nameTemplate,iNum);
        TH1 *hist2 = (TH1 *)histFile->Get(hName2);
        while (hist2) {
          hist->Add(hist2);
          ++iNum;
          hName2 = Form(nameTemplate,iNum);
          hist2 = (TH1 *)histFile->Get(hName2);
        }
      } else {

        TObject *obj = histFile->Get(hName);
        if (!obj) {
          std::cerr << "In TemplateInfo constructor: Cannot find histogram " << hName
                    << " in file " << histFile
                    << " for " << name << " template." << std::endl;
          exit(2);
        }

        //Make a copy
        TString clName = hName + name;
        hist = (TH1 *)obj->Clone(clName);
      }
    
      hist->SetDirectory(0);

      // JMS addition
      // rescale incoming histos
      // to match norms of old histos
      // this comes up if you are swapping shapes
      // for something that has a cross section
      double orignalNorm = histMap[iJetBin->GetName()]->Integral();
      if (hist->Integral() > 0){
        hist->Scale(orignalNorm/hist->Integral());
      } else {
        hist->Scale(0.);
      }
      
      histMap[iJetBin->GetName()] = hist;
    }

    //Done accessing the histogram file
    histFile->Close();
    delete histFile;
    histFile = 0;

    //Rebin the templates
    rebin(&histMap);
    
  }

  void TemplateInfo::setConstraint(double frac, TemplateInfo *refTempl) {

    if (parConstraints.getSize() != 0) {
      //We can't have two different sets of constraints!
      std::cerr << "ERROR!  You cannot call TemplateInfo::setConstraint() multiple times for the same template!"
                << "  -->Template name = " << name << std::endl;
      parConstraints.Print("V");
      exit(2);
    }

    //Now, check whether this is an absolute or relative constraint
    if (refTempl) {
      //Relative constraint
      
      //Check that both templates are of the same type
      if (fixedNJets != refTempl->fixedNJets) {
        std::cerr << "ERROR can't constrain this template to a template with a different type of NJets " << std::endl
                  << "distribution (fixed versus floating)" << std::endl
                  << "  --> This template: fixedNJets = " << fixedNJets << std::endl
                  << "  --> Reference template: fixedNJets = " << refTempl->fixedNJets << std::endl;
        exit(2);
      }

      if (fixedNJets) {

        //Now, let's try to do something a little fancy.  Let's suppose that
        //we want to fit for the ratio of this Template to the ratio
        //Step 1:  Get this template's and the reference's cross section parameters
        RooAbsReal *thisXSPar = (RooAbsReal *)fitPars.first();
        RooAbsReal *refXSPar = (RooAbsReal *)refTempl->fitPars.first();

        //Next, figure out what the MC ratio of these is
        double ratio = thisXSPar->getVal()/refXSPar->getVal();

        //Now make a new fit parameter for the ratio
        TString rName = "ratio";
        rName += name;
        rName += "_";
        rName += refTempl->name;
        RooRealVar *ratioPar = new RooRealVar(rName,rName,ratio,-0.5,10*ratio);
        constrainedPars.addOwned(*ratioPar);

        //Next, we need a "formula" variable to relate the ratio to the this XS:
        TString parName = thisXSPar->GetName();
        TString parTitle = thisXSPar->GetTitle();

        //Get replace old parameter with new
        fitPars.remove(*thisXSPar);
        thisXSPar = new RooFormulaVar(parName,parTitle,"@0*@1",
                                 RooArgList(*ratioPar,*refXSPar));

        //Now, make this the fit parameter instead of original one
        //Magically, now the cross section be a function of the ratio.
        fitPars.addOwned(*thisXSPar);

        //Instead, constrain ratio of W to Z to be within 30% of SM prediction.
        RooGaussian *rConstraint =
          new RooGaussian(rName+"Constr",rName+"Constr",*ratioPar, RooConst(ratio), RooConst(frac*ratio));
      
        parConstraints.addOwned(*rConstraint);

      } else {

        for (std::map<TString,TH1 *>::iterator jetBinIter = histMap.begin();
             jetBinIter != histMap.end(); ++jetBinIter) {
        
          const TString &jetBinName = jetBinIter->first;
        
          TString safeName = jetBinName;
          if (safeName.Contains(">=")) {
            safeName.Remove(safeName.Index(">="),2);
          }
        
          
          //Now, let's try to do something a little fancy.  Let's suppose that
          //we want to fit for the ratio of this template to reference instead.
          //Step 1:  Get this template's and the reference's cross section parameters
          TString parNameThis = "num";
          parNameThis += name;
          parNameThis += safeName;
          RooAbsReal *thisNumPar = (RooAbsReal *)fitPars.find(parNameThis);
          TString parNameRef = "num";
          parNameRef += refTempl->name;
          parNameRef += safeName;
          RooAbsReal *refNumPar = (RooAbsReal *)refTempl->fitPars.find(parNameRef);
        
          //Next, figure out what the MC ratio of these is
          double ratio = thisNumPar->getVal()/refNumPar->getVal();
        
          //Now make a new fit parameter for the ratio
          TString rName = "ratio";
          rName += safeName;
          rName += "_";
          rName += name;
          rName += "_";
          rName += refTempl->name;
          RooRealVar *ratioPar = new RooRealVar(rName,rName,ratio,-0.5,10*ratio);
          constrainedPars.addOwned(*ratioPar);
        
          //Next, we need a "formula" variable to relate the ratio to the this Number:
          TString parName = thisNumPar->GetName();
          TString parTitle = thisNumPar->GetTitle();
          
          //Get replace old parameter with new
          fitPars.remove(*thisNumPar);
          thisNumPar = new RooFormulaVar(parName,parTitle,"@0*@1",
                                         RooArgList(*ratioPar,*refNumPar));

          //Now, make this the fit parameter instead of original one
          //Magically, now the cross section be a function of the ratio.
          fitPars.addOwned(*thisNumPar);

          //Instead, constrain ratio of W to Z to be within 30% of SM prediction.
          RooGaussian *rConstraint =
            new RooGaussian(rName+"Constr",rName+"Constr",*ratioPar, RooConst(ratio), RooConst(frac*ratio));
          
          parConstraints.addOwned(*rConstraint);
        }
      }

    } else {

      //Absolute constraint.  Again handle fixed Njets and floating Njets cases
      if (fixedNJets) {

        //Add the cross section to the list of constrained parameters
        RooAbsReal *thisXSPar = (RooAbsReal *)fitPars.first();
        constrainedPars.add(*thisXSPar);

        TString constrName = thisXSPar->GetName();
        constrName += "Constr";
        RooGaussian *xsConstr =
          new RooGaussian(constrName,constrName,*thisXSPar, 
                          RooConst(thisXSPar->getVal()), RooConst(frac*thisXSPar->getVal()));
      
        parConstraints.addOwned(*xsConstr);

      } else {

        for (std::map<TString,TH1 *>::iterator jetBinIter = histMap.begin();
             jetBinIter != histMap.end(); ++jetBinIter) {
        
          const TString &jetBinName = jetBinIter->first;
        
          TString safeName = jetBinName;
          if (safeName.Contains(">=")) {
            safeName.Remove(safeName.Index(">="),2);
          }

          TString parName = "num";
          parName += name;
          parName += safeName;
          RooAbsReal *thisNumPar = (RooAbsReal *)fitPars.find(parName);
          constrainedPars.add(*thisNumPar);

          TString constrName = parName;
          constrName += "Constr";
          RooGaussian *numConstr =
          new RooGaussian(constrName,constrName,*thisNumPar, 
                          RooConst(thisNumPar->getVal()), RooConst(frac*thisNumPar->getVal()));
      
          parConstraints.addOwned(*numConstr);

        }
      }
    }

  }      

  void TemplateInfo::addTemplates(const TString &inFileName, 
                                  int newGenEvents, double newCrossSection) {

    //OK: I don't want to mess with fixing the constraints if you've already added constraints, 
    //so just issue an error message (with instructions) instead and then end
    if (parConstraints.getSize() != 0) {
      std::cerr << "ERROR: You cannot currently merge templates after having set a constraint." << std::endl
                << "Please go back and call addTemplates() before setConstraint()." << std::endl
                << "--> Template Name: " << name << std::endl;
      parConstraints.Print("V");
      exit(2);
    }

    //Extract a rebinned version of the templates from the file
    std::cout << "Adding additional templates from " << inFileName 
              << " to templates for " << name << std::endl;
    TFile *histFile = TFile::Open(inFileName);
    if (!histFile) {
      std::cerr << "Error!  Cannot open file " << inFileName << std::endl;
      exit(2);
    }

    //A container to temporarily store the histograms to be summed
    std::map<TString,TH1 *> tempHistMap;

    //Make a copy of the templates
    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;

    while ((iJetBin = (RooCatType *)jetIter->Next())) {

      std::cout << "  ----->JetBin = " << iJetBin->GetName() << std::endl;

      BinInfo &binInfo = rebinInfo[iJetBin->GetName()];

      TString hName = binInfo.histName;

      std::cout << "  -----> hName = " << hName << std::endl;

      TH1 *hist = 0;

      //Check to see if this histogram is for an inclusive jet bin
      if (hName.Contains("[>=")) {

        std::cout << "  -----> Inclusive Histogram!..." << std::endl;

        //We have to sum over a number of histograms
        TString nameTemplate = hName;

        //Extract the starting number
        int ind1 = nameTemplate.Index("[>=") + 3;
        int ind2 = nameTemplate.Index("]");
        int iNum = atoi(nameTemplate(ind1,ind2-ind1).Data());
       
        //Replace "[>=N]" with %d
        nameTemplate.Replace(ind1-3,ind2-ind1+4,"%d");

        TString hNameReplaced = Form(nameTemplate,iNum);

        std::cout << "  -----> nameTemplate = " << nameTemplate << ", iNum = " << iNum 
                  << ", hNameReplaced = " << hNameReplaced << std::endl;

        //Grab the first histogram
        TObject *obj = histFile->Get(hNameReplaced);
        if (!obj) {
          std::cerr << "In TemplateInfo constructor: Cannot find histogram " 
                    << hNameReplaced
                    << " in file " << histFile
                    << " for " << name << " template." << std::endl
                    << "Note:  Inclusive >= N histogram adding..." 
                    << std::endl;
          exit(2);
        }

        //Make a copy
        TString clName = hNameReplaced + name;
        hist = (TH1 *)obj->Clone(clName);

        ++iNum;
        TString hName2 = Form(nameTemplate,iNum);
        TH1 *hist2 = (TH1 *)histFile->Get(hName2);
        while (hist2) {
          std::cout << "  -----> iNum = " << iNum 
                    << ", hName2 = " << hName2 << std::endl;
          hist->Add(hist2);
          ++iNum;
          hName2 = Form(nameTemplate,iNum);
          hist2 = (TH1 *)histFile->Get(hName2);
        }
      } else {

        std:: cout << "  -----> Getting " << hName << "..." << std::endl;
        TObject *obj = histFile->Get(hName);
        if (!obj) {
          std::cerr << "In TemplateInfo constructor: Cannot find histogram " << hName
                    << " in file " << histFile
                    << " for " << name << " template." << std::endl;
          exit(2);
        }

        //Make a copy
        TString clName = hName + name;
        hist = (TH1 *)obj->Clone(clName);
      }

      hist->SetDirectory(0);
      tempHistMap[iJetBin->GetName()] = hist;
    }

    //Done accessing the histogram file
    histFile->Close();
    delete histFile;
    histFile = 0;

    //Now let's do any fixing up of bins that I need to do
    rebin(&tempHistMap);

    //Finally, let's go through the histogram
    //for each jet bin and combine templates
    RooCategory *jetBin = TemplateInfo::getJetBins();

    //Loop over the jet bins and assemble the appropriate RooAddPdfs
    jetIter = jetBin->typeIterator();
    iJetBin = 0;

    while ((iJetBin = (RooCatType *)jetIter->Next())) {

      TString jetBinName = iJetBin->GetName();
      TH1 *origHist = histMap[jetBinName];
      origHist->Scale(crossSection/nGenEvts);

      TH1 *newHist = tempHistMap[jetBinName];
      origHist->Add(newHist,newCrossSection/newGenEvents);

      //We don't need this histogram anymore
      delete newHist;
      tempHistMap[jetBinName] = 0;

      if (!fixedNJets) {
        //If this is a floating Njets distribution, we need to
        //to set the correct starting value (will be used for PE's)
        TString parName = "num";
        parName += name;
        parName += iJetBin->GetName();
        
        if (parName.Contains(">=")) {
          parName.Remove(parName.Index(">="),2);
        }
        
        RooAbsReal *num = (RooAbsReal *)fitPars.find(parName);
        //OK, to be safe, I'm going to try to cast this (dynmaically) to a RooRealVar.
        //If that fails, it suggest that this is a derived variable (i.e. RooFormulaVar)
        //that determines its normalization some other way...
        RooRealVar *realNum = dynamic_cast<RooRealVar *>(num);
        if (realNum) {
          realNum->setVal(origHist->Integral()*TemplateInfo::getLuminosity()*lumiConvFactor);
        }
      }
    }

    //Now make sure our fit parameters are all set
    crossSection = 1.0;
    nGenEvts = 1.0;

    if (fixedNJets) {
      RooAbsArg *par = fitPars.first();
      TString newParName = "beta";
      newParName += name;
      par->SetName(newParName);
      par->SetTitle(newParName);
      //OK, to be safe, I'm going to try to cast this (dynmaically) to a RooRealVar.
      //If that fails, it suggest that this is a derived variable (i.e. RooFormulaVar)
      //that determines its normalization some other way...
      RooRealVar *realPar = dynamic_cast<RooRealVar *>(par);
      if (realPar) {
        realPar->setRange(-10,20);
        realPar->setVal(1.0);
      }
    }
    
    fitPars.Print("V");

  }    

  TemplateInfo::~TemplateInfo() {

    //Delete the histogram pointers
    for (std::map<TString,TH1 *>::iterator iter = histMap.begin();
         iter != histMap.end(); ++iter) {
      delete iter->second;
    }

  }

  TString TemplateInfo::getHistName(const TString jetBinName) {

    std::map<TString,BinInfo>::iterator iter = rebinInfo.find(jetBinName);

    if (iter == rebinInfo.end()) {
      return TString("");
    } else {
      BinInfo &bi = iter->second;
      return bi.histName;
    }
  }

  void TemplateInfo::rebin(std::map<TString,TH1 *> *hMapPtr) {

    //To make the code slightly more readable--change from ptr to ref
    std::map<TString,TH1 *> &hMap = *hMapPtr;

    //Loop over the jet bins in this template
    for (std::map<TString,TH1 *>::iterator hIter = hMap.begin();
         hIter != hMap.end(); ++hIter) {

      const TString &jetBinName = hIter->first;
      TH1 *origHist = hIter->second;

      //Some information about the original histogram and binning
      int nBinsOrig = origHist->GetNbinsX();
      double xMinOrig = origHist->GetXaxis()->GetXmin();
      double xMaxOrig = origHist->GetXaxis()->GetXmax();
      double binSizeOrig = (xMaxOrig - xMinOrig)/nBinsOrig;
      double nEntries = origHist->GetEntries();

      //Now prepare the rebinned histogram
      TH1 *hist = (TH1 *)origHist->Clone();
      hist->SetDirectory(0);

      int nBins = nBinsOrig;
      double xMin = xMinOrig;
      double xMax = xMaxOrig;
      double binSize = binSizeOrig;

      //Wipe out the contents of the destination histogram
      hist->Reset();

      //See if we're rebinning this histogram
      std::map<TString,BinInfo>::iterator rebinIter = rebinInfo.find(jetBinName);

      if (rebinIter != rebinInfo.end()) {
        const BinInfo &info = rebinIter->second;
        nBins = info.nBins;
        xMin = info.xLow;
        xMax = info.xHigh;
        binSize = (xMax-xMin)/nBins;

        hist->SetBins(nBins,xMin,xMax);

        std::cout << "---->Rebinning template in jet bin " << jetBinName
                  << " using " << nBins << " bins from " << xMin << " to " << xMax << "." << std::endl;

      } else {

        std::cout << "Error: Binning information not defined for this channel" << std::endl
                  << "  --> jetBinName = " << jetBinName << std::endl
                  << "ABORTING!..." << std::endl;
        exit(2);
      }

      //Make sure we keep track of the maximum number of histogram bins for any jet bin
      maxBins = TMath::Max(maxBins,nBins);

      // Check that this is a valid rebinning that aligns with the
      // original histogram
      
      // JMS update
      // watch out for floating point problems!
      // give yourself a cushion to handle bin boundary issues

      double tolerance = 0.00001;
      
      int firstBinOrig = origHist->FindBin(xMin+tolerance);

      
      if (! ( (xMin < (origHist->GetBinLowEdge(firstBinOrig) + tolerance))
              && (xMin > (origHist->GetBinLowEdge(firstBinOrig) - tolerance))
              )
              ) {
        
        std::cerr << "Bins don't align!.  Requested min = " << xMin
                  << ".  Available min = " << origHist->GetBinLowEdge(firstBinOrig) << std::endl
                  << "Original: nBins = " << nBinsOrig << ", xMin = " << xMinOrig
                  << ", xMax = " << xMaxOrig << std::endl
                  << "Rebinned: nBins = " << nBins << ", xMin = " << xMin
                  << ", xMax = " << xMax << std::endl;
        exit(2);
      }
      
      //Check that the original bin size divides evenly into the new bin size so that we're grouping
      //together integral numbers of bins
      double binRatio = binSize/binSizeOrig;
      int binGroup = (int)binRatio;
      
      if (binRatio - binGroup > 1e-10) {
        std::cerr << "Bins don't align!.  Requested bin size = " << binSize
                  << ".  Original min = " << binSizeOrig << std::endl
        << "Original: nBins = " << nBinsOrig << ", xMin = " << xMinOrig
        << ", xMax = " << xMaxOrig << std::endl
        << "Rebinned: nBins = " << nBins << ", xMin = " << xMin
        << ", xMax = " << xMax << std::endl;
        exit(2);
      }

      //Handle the first bin, including underflow in the original
      double binCont = 0;
      double binSumW2 = 0;

      int origStart = 0; //Include the underflow in original
      //Already have firstBinOrig from above
      int origEnd = firstBinOrig + binGroup;
      for (int iOrigBin = origStart; iOrigBin < origEnd; ++iOrigBin) {
        binCont += origHist->GetBinContent(iOrigBin);
        binSumW2 += origHist->GetBinError(iOrigBin)*origHist->GetBinError(iOrigBin);
      }
      hist->SetBinContent(1,binCont);
      if (hist->GetSumw2N()) hist->SetBinError(1,sqrt(binSumW2));

      //Now do the "bulk" of the distribution
      for (int iNewBin = 2; iNewBin < nBins; ++iNewBin) {

        origStart = origEnd;
        origEnd = origStart + binGroup;

        binCont = 0;
        binSumW2 = 0;

        for (int iOrigBin = origStart; iOrigBin < origEnd; ++iOrigBin) {
          binCont += origHist->GetBinContent(iOrigBin);
          binSumW2 += origHist->GetBinError(iOrigBin)*origHist->GetBinContent(iOrigBin);
        }

        hist->SetBinContent(iNewBin,binCont);
        if (hist->GetSumw2N()) hist->SetBinError(iNewBin,sqrt(binSumW2));
                   
      }

      //Do any remaining bins past the end of the new range (including overflow in original)
      origStart = origEnd;
      origEnd = 2+nBinsOrig; //Include the overflow in the original
      binCont = 0;
      binSumW2 = 0;

      for (int iOrigBin = origStart; iOrigBin < origEnd; ++iOrigBin) {
        binCont += origHist->GetBinContent(iOrigBin);
        binSumW2 += origHist->GetBinError(iOrigBin)*origHist->GetBinContent(iOrigBin);
      }

      hist->SetBinContent(nBins,binCont);
      if (hist->GetSumw2N()) hist->SetBinError(nBins,sqrt(binSumW2));

      //Fix the entries in this histogram so that it matches the original
      hist->SetEntries(nEntries);

      //Replace the original histogram with this new one
      hMap[jetBinName] = hist;

      //We're done with the original histogram.  No need to keep it.
      delete origHist;

    }

  }

  //Function for creating a multi-bin template PDF.
  void createPDF(RooSimultaneous *pdf,
                 const std::vector<TemplateInfo *> &templateList) {

    //Set up the RooCategory variable that represents the binning on the "x-axis" of this pdf
    //Note:  Since this fit is explicitly a binned one, binVar represents which bin
    //the data falls in.  For example, if you're fitting NJets vs MET, binVar represents
    //which MET bin the data is in in a given jet bin.
    RooCategory *binVar = TemplateInfo::getVarBins();
    binVar->clearTypes();
    for (int i = 1; i <= TemplateInfo::maxNBins(); ++i) {
      binVar->defineType(Form("Bin %d",i),i);
    }

    RooCategory *jetBin = TemplateInfo::getJetBins();

    //Loop over the jet bins and assemble the appropriate RooAddPdfs
    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;

    while ((iJetBin = (RooCatType *)jetIter->Next())) {

      std::cout << "--> Assembling components for jet bin " 
                << iJetBin->GetName() << std::endl;

      //Create ArgSets for the pdfs and their coefficients
      //For now I'm sticking these on the heap because it looks like
      //RooFit only keeps a pointer/reference to these objects
      //and doesn't make a copy (so they disappear when they go
      //out of scope!)
      RooArgSet *componentPdfs = new RooArgSet(Form("componentPdfs%dJ",iJetBin->getVal()));
      RooArgSet *componentNorms = new RooArgSet(Form("componentNormss%dJ",iJetBin->getVal()));

      //Loop over the samples and produce templates
      for (std::vector<TemplateInfo *>::const_iterator tIter = templateList.begin();
           tIter != templateList.end(); ++tIter) {

        TemplateInfo *t = *tIter;

        std::cout << "----> Adding template " << t->name << "." << std::endl;
        
//         TString nameUpper = t->name;
//         nameUpper.ToUpper();
//         TString jetBinName = (nameUpper == "QCD") ? "1j" : iJetBin->GetName();
//         if (nameUpper == "QCD") std::cout << "QCD Jet bin kludge, using jet bin " << jetBinName << std::endl;
        TString jetBinName = iJetBin->GetName();

        TH1 *varHist = t->histMap[jetBinName];

        if (varHist->Integral() <= 0) continue;

        //Make a histogram template PDF
        std::cout << "----> Constructing histogram template pdf." << std::endl;
        TString dhName = "dataHist_";
        dhName += TemplateInfo::getHistName(jetBinName);
        dhName += "_";
        dhName += t->name;
        RooDataHist *dataHist = new RooDataHist(dhName,dhName,RooArgSet(*binVar));

        TIterator *binIter = binVar->typeIterator();
        RooCatType *binPtr = 0;

        while ((binPtr = (RooCatType *)binIter->Next())) {

          int iBin = binPtr->getVal();
          double wght = varHist->GetBinContent(iBin);
          double err = varHist->GetBinError(iBin);

          binVar->setIndex(iBin);
          dataHist->add(RooArgSet(*binVar),wght,err*err);

        }

        TString pdfName = "pdf";
        pdfName += t->name;
        pdfName += iJetBin->GetName();
        RooHistPdf *templatePDF = new RooHistPdf(pdfName,pdfName,*binVar,*dataHist);


        //Construct the normalization variable for this template in this jet bin
        RooAbsReal *norm = 0;
        std::cout << "----> Hooking up a varible for the normalization in this fit." << std::endl;
        if (t->fixedNJets) {
          
          //This template has just one fit parameter
          RooAbsArg *xs = t->fitPars.first();
          xs->Print();

          const RooRealVar &sf = t->getScaleFactor();
          
          //Figure out the "acceptance" for this jet bin
          double numJetBin = varHist->Integral();
          double acceptance = numJetBin/t->nGenEvts;
          
          TString fName = "num";
          fName += t->name;
          fName += iJetBin->GetName();
          double lumi = TemplateInfo::getLuminosity();
          norm = new RooFormulaVar(fName,fName,
                                   "@0*@1*@2",
                                   RooArgList(RooRealConstant::value(acceptance*lumi*lumiConvFactor),sf,*xs));
          std::cout << "numJetBin = " << numJetBin
                    << ", acceptance = " << acceptance
                    << ", lumi = " << lumi << std::endl;

          
        } else {
          
          //OK, now we need to find the parameter in the RooArgList
          TString parName = "num";
          parName += t->name;
          parName += iJetBin->GetName();

          if (parName.Contains(">=")) {
            parName.Remove(parName.Index(">="),2);
          }

          RooAbsReal *num = (RooAbsReal *)t->fitPars.find(parName);
          norm = num;
          
        }
        
        if (norm) {
          norm->Print();
          std::cout << "Default normalization = " << norm->getVal() << std::endl;
          
          std::cout << "----> Adding fit norm and pdf to template argument lists." << std::endl;
          //Load the pdf and the fit parameter into the appropriate arg lists
          componentPdfs->add(*templatePDF);
          componentNorms->add(*norm);
        }
      }
      
      std::cout << "--> Creating RooAddPdf for this jet bin." << std::endl;
      //Put them all together for one jet bin
      RooAddPdf *jetBinPDF = new RooAddPdf(iJetBin->GetName(),iJetBin->GetName(),
                                           *componentPdfs,*componentNorms);
      
      std::cout << "--> Adding RooAddPdf for this jet bin to global RooSimultaneous pdf." 
                << std::endl;
      //Add it to the RooSimultaneous fit
      pdf->addPdf(*jetBinPDF,iJetBin->GetName());
      
    }

    std::cout << "...Done." << std::endl;
    
    //OK, now we have a PDF.  In principal, we can do fits and pseudo-experiments.
    //Let's check that it looks like we want it too.
    TString gvName = "fitNJetsVsX.gv";
    std::cout << "Writing PDF structure to " << gvName << std::endl
              << "Use Graphviz to view." << std::endl;
    pdf->graphVizTree(gvName," ");
  }

  void plotTemplates(const TString &xTitle, const std::vector<TemplateInfo *> &templates, 
                     bool save, const char *plotLabel) {

    //Make sure we have a decent plot style
    gROOT->SetStyle("Plain");
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.07);
    gStyle->SetPadTopMargin(0.07);
    gStyle->SetPadBottomMargin(0.1);
    gStyle->SetOptStat(0);

    RooCategory *jetBin = TemplateInfo::getJetBins();

    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;

    while ((iJetBin = (RooCatType *)jetIter->Next())) {

      //Create a Canvas for the plots.
      TString canvName = "templates";
      if (plotLabel) {
        canvName += "_";
        canvName += plotLabel;
      }
      canvName += "_";
      canvName += iJetBin->GetName();

      if (canvName.Contains(">=")) {
        canvName.Remove(canvName.Index(">="),2);
      }

      TCanvas *canv = new TCanvas(canvName, canvName, 600,600);
      canv->cd();

      TLegend *leg = new TLegend(0.155,0.8,0.92,0.92);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);
      leg->SetNColumns(3);

      //Hang on to the hists so that we can set a common maximum before plotting
      std::vector<TH1 *> hists;
      double histMax = -999.;

      //Loop over the templates and make unit normalized copies of them to plot
      for (std::vector<TemplateInfo *>::const_iterator tIter = templates.begin();
           tIter != templates.end(); ++tIter) {

        TemplateInfo *t = *tIter;

        TH1 *hist = (TH1 *)t->histMap[iJetBin->GetName()]->Clone();
        hist->SetLineColor(t->color);
        hist->SetLineWidth(2);
        hist->SetFillColor(0);
        hist->Scale(1./hist->Integral());
        histMax = TMath::Max(histMax,hist->GetMaximum());
        hist->SetTitle(iJetBin->GetName());
        hist->GetXaxis()->SetTitle(xTitle);
        hist->GetYaxis()->SetTitle("Arb. Norm.");
        hist->GetYaxis()->SetTitleOffset(1.4);
        hists.push_back(hist);

        leg->AddEntry(hist,t->name,"l");

      }


      //Give a little bit of a margin above the plots
      histMax *= 1.3;

      //Now go ahead and draw the templates
      std::vector<TH1 *>::iterator hIter = hists.begin();
      TH1 *hist = *hIter;
      hist->SetMaximum(histMax);
      hist->Draw("HIST");

      for ( ; hIter != hists.end(); ++hIter) {
        hist = *hIter;
        hist->Draw("HISTSAME");
      }

      leg->Draw();

      if (save) canv->Print(".png");

    }
  }

  
  void plotStack(const TString &xTitle, std::vector<TemplateInfo *> templates, 
                 bool save, const char *plotLabel, TemplateInfo *data ) {
    
    //Make sure we have a decent plot style
    gROOT->SetStyle("Plain");
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.07);
    gStyle->SetPadTopMargin(0.07);
    gStyle->SetPadBottomMargin(0.1);
    gStyle->SetOptStat(0);

    RooCategory *jetBin = TemplateInfo::getJetBins();

    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;

    while ((iJetBin = (RooCatType *)jetIter->Next())) {

      //Create a Canvas for the plots.
      TString canvName = "stack";
      if (plotLabel) {
        canvName += "_";
        canvName += plotLabel;
      }
      canvName += "_";
      canvName += iJetBin->GetName();
      
      if (canvName.Contains(">=")) {
        canvName.Remove(canvName.Index(">="),2);
      }

      TCanvas *canv = new TCanvas(canvName, canvName, 600,600);
      canv->cd();

      TLegend *leg = new TLegend(0.155,0.8,0.92,0.92);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);
      leg->SetNColumns(3);

      //Hang on to the hists so that we can set a common maximum before plotting
      std::vector<TH1 *> hists;

      //Loop over the templates and make unit normalized copies of them to plot
      for (std::vector<TemplateInfo *>::const_iterator tIter = templates.begin();
           tIter != templates.end(); ++tIter) {

        TemplateInfo *t = *tIter;

        TH1 *hist = (TH1 *)t->histMap[iJetBin->GetName()]->Clone();
        if (hist->Integral() > 0) {
          hist->SetLineColor(1);
          hist->SetLineWidth(1);
          hist->SetFillColor(t->color);
          hist->SetFillStyle(1001);
          
          //Scale according to current fit parameter
          if (t->fixedNJets) {
            
            RooAbsReal *xs = static_cast<RooAbsReal *>(t->fitPars.first());
            const RooRealVar &sf = t->getScaleFactor();
            hist->Scale(sf.getVal()*xs->getVal()*TemplateInfo::getLuminosity()*Fitter::lumiConvFactor/t->nGenEvts);
            
          } else {
            TString parName = "num";
            parName += t->name;
            parName += iJetBin->GetName();
            if (parName.Contains(">=")) {
              parName.Remove(parName.Index(">="),2);
            }
            
            RooAbsReal *num = static_cast<RooAbsReal *>(t->fitPars.find(parName));
            hist->Scale(num->getVal()/hist->Integral());
          }
          
          hist->SetTitle(iJetBin->GetName());
          hists.push_back(hist);

          leg->AddEntry(hist,t->name,"f");
        }
      }

      //Now put the histograms in the stack.  Do it in reverse order so that the order
      //drawn matches the order in the legend
      
      THStack * returnStack = new THStack("stack",iJetBin->GetName());
      
      
      for (std::vector<TH1 *>::reverse_iterator hIter = hists.rbegin(); 
           hIter != hists.rend(); ++hIter) {
        
        TH1 *hist = *hIter;
        if (hist->Integral() > 0) {
          returnStack->Add(hist,"HIST");
        }

      }// end loop over histo iter

      double histMax = returnStack->GetMaximum();

      TH1 *dataHist = 0;
      if (data) {
        dataHist = data->histMap[iJetBin->GetName()];
        dataHist->SetLineColor(1);
        dataHist->SetMarkerColor(1);
        dataHist->SetFillColor(0);
        dataHist->SetLineWidth(2);
        dataHist->SetMarkerStyle(20.);
        
        histMax = TMath::Max(histMax,dataHist->GetMaximum());

        leg->AddEntry(dataHist,"Data","lpe");
      }

      histMax *= 1.3;

      returnStack->SetMaximum(histMax);

      returnStack->Draw();  //Creates the histogram needed for formatting.
      //Fix up some of the histogram formating
      returnStack->GetXaxis()->SetTitle(xTitle);
      returnStack->GetYaxis()->SetTitle("Events");
      returnStack->GetYaxis()->SetTitleOffset(1.4);
      returnStack->Draw();
      if (dataHist) dataHist->Draw("E0SAME");

      leg->Draw();

      if (save) canv->Print(".png");

    }

  }// end plotStack

  void saveTemplates(TString fileName, const std::vector<TemplateInfo *> &templates, bool normalized) {

    TFile *outFile = TFile::Open(fileName, "RECREATE");

    if (!outFile) {
      std::cerr << "Cannot open file " << fileName << " for output.  Will not save histograms."
                << std::endl;
      return;
    }

    outFile->cd();

    for (std::vector<TemplateInfo *>::const_iterator tIter = templates.begin();
         tIter != templates.end(); ++tIter) {

      const TemplateInfo *t = *tIter;

      for (std::map<TString,TH1 *>::const_iterator hIter = t->histMap.begin();
           hIter != t->histMap.end(); ++hIter) {

        TH1 *hist = (TH1 *)hIter->second->Clone();
        if (normalized) {
          //Scale according to current fit parameter
          if (t->fixedNJets) {
            RooAbsReal *xs = static_cast<RooAbsReal *>(t->fitPars.first());
            const RooRealVar &sf = t->getScaleFactor();
            hist->Scale(sf.getVal()*xs->getVal()*TemplateInfo::getLuminosity()*Fitter::lumiConvFactor/t->nGenEvts);
          } else {
            TString parName = "num";
            parName += t->name;
            parName += hIter->first;
            if (parName.Contains(">=")) {
              parName.Remove(parName.Index(">="),2);
            }
            RooAbsReal *num = static_cast<RooAbsReal *>(t->fitPars.find(parName));
            hist->Scale(num->getVal()/hist->Integral());
          }

        }
        
        hist->Write();

      }

    }

  }
    
  TemplateInfo *convertRooDataHist(const RooDataHist &data) {
    
    //Initialize the return object
    //Assumes that the usual initialization and binning has already been set up.
    TemplateInfo *dataTempl = new TemplateInfo();

    //Get access to the data in the DataHist
    RooCategory *jetBin = TemplateInfo::getJetBins();
    RooCategory *bin = TemplateInfo::getVarBins();
    Roo1DTable *table = data.table(RooArgSet(*bin,*jetBin));

    for (std::map<TString, TH1 *>::iterator iter = dataTempl->histMap.begin();
         iter != dataTempl->histMap.end(); ++iter) {

      const TString &jetBinName = iter->first;
      TH1 *hist = iter->second;

      for (int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) {

        TString label = "{Bin ";
        label += iBin;
        label += ";";
        label += jetBinName;
        label += "}";

        hist->SetBinContent(iBin,table->get(label));

      }

    }

    delete table;

    return dataTempl;

  }          

  RooDataHist *convertTemplateInfo(const TemplateInfo &data) {
    
    RooCategory *jetBin = TemplateInfo::getJetBins();
    RooCategory *bin = TemplateInfo::getVarBins();
    RooDataHist *dataHist = new RooDataHist("dataHist","Data",RooArgList(*bin,*jetBin));
    
    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;
    
    while ((iJetBin = (RooCatType *)jetIter->Next())) {

      //Get the histogram with the data
      std::map<TString,TH1 *>::const_iterator hIter = data.histMap.find(iJetBin->GetName());
      if (hIter == data.histMap.end()) {
        std::cerr << "ERROR:  Missing histogram for " << iJetBin->GetName() << std::endl;
        exit(2);
      }

      const TH1 *hist = hIter->second;
      jetBin->setIndex(iJetBin->getVal());

      //Copy in the data
      for (int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) {

        double wght = hist->GetBinContent(iBin);
        double err = hist->GetBinError(iBin);
        
        bin->setIndex(iBin);
        dataHist->add(RooArgSet(*bin,*jetBin),wght,err*err);
        
      }
      
    }
    
    return dataHist;
  }


  void printNEvts(std::vector<TemplateInfo *> templates, 
                  TemplateInfo *data, const RooFitResult *fitResult) {

    //Print with jet bins as columns and the components as rows
    std::cout << std::setw(15) << "Template" << "  ";
    RooCategory *jetBin = TemplateInfo::getJetBins();
    TIterator *jetIter = jetBin->typeIterator();
    RooCatType *iJetBin = 0;
    while ((iJetBin = (RooCatType *)jetIter->Next())) {
      std::cout << std::setw(16) << iJetBin->GetName() << "  ";
    }
    std::cout << std::endl;

    std::cout << "---------------  ";

    for (int i = 0; i < jetBin->numTypes(); ++i)
      std::cout << "----------------  ";
    std::cout << std::endl;

    // This is a well intentioned effort to do the sum with error
    // propagation that I'm going to punt on for now.
//     std::vector<double> binTotal(jetBin.numTypes(),0.);
//     std::vector<RooArgList *> sumList;
//     if (fitResult) {
//       for (int i = 0; i < jetBin.numTypes(); ++) 
//         sumList.push_back(new RooArgList());
//     }
//     TString sumString = "";
//     for (int i = 0; i < templates.size(); ++i) {
//       sumString += "@";
//       sumString += i;
//       if (i != templates.size()-1) sumString += "+";
//     }


    // Top is signal, all else is bkg
    
    double nSignal = 0.0;
    double nBkg = 0.0;

    
    
    for (std::vector<TemplateInfo *>::iterator tIter = templates.begin();
         tIter != templates.end(); ++tIter) {
      

      TemplateInfo *t = *tIter;

      std::cout << std::right << std::setw(15) << t->name << std::left << "  ";

      jetIter->Reset();
      while ((iJetBin = (RooCatType *)jetIter->Next())) {

        TString jetBinName = iJetBin->GetName();
        TH1 *varHist = t->histMap[jetBinName];

        double num = 0.;
        double err = 0.;

        if (t->fixedNJets) {
          
          //This template has just one fit parameter
          RooAbsArg *xs = t->fitPars.first();
          const RooRealVar &sf = t->getScaleFactor();
          
          //Figure out the "acceptance" for this jet bin
          double numJetBin = varHist->Integral();
          double acceptance = numJetBin/t->nGenEvts;
          
          TString fName = "tempNum";
          fName += t->name;
          fName += iJetBin->GetName();
          double lumi = TemplateInfo::getLuminosity();
          RooFormulaVar sum(fName,fName,
                            "@0*@1*@2",
                            RooArgList(RooRealConstant::value(acceptance*lumi*lumiConvFactor),sf,*xs));

          num = sum.getVal();
          if (fitResult) err = sum.getPropagatedError(*fitResult);
          
        } else {
          
          //OK, now we need to find the parameter in the RooArgList
          TString parName = "num";
          parName += t->name;
          parName += iJetBin->GetName();

          if (parName.Contains(">=")) {
            parName.Remove(parName.Index(">="),2);
          }

          RooRealVar *par = (RooRealVar *)t->fitPars.find(parName);

          num = par->getVal();
          err = par->getError();
          
        }
        
        if (fitResult) {
          std::cout << Form("%8.2f+-%6.2f",num,err) << "  ";
        } else {
          if (t->name == "ttbar"){
            nSignal += num;
          } else {
            nBkg += num;
          }
          std::cout << Form("%16.2f",num) << "  ";
        }

      }
      std::cout << std::endl;

    }

    if (data) {

      std::cout << "---------------  ";
      
      for (int i = 0; i < jetBin->numTypes(); ++i)
        std::cout << "----------------  ";
      std::cout << std::endl;
      
      std::cout << "           data  ";

      jetIter->Reset();
      while ((iJetBin = (RooCatType *)jetIter->Next())) {

        TString jetBinName = iJetBin->GetName();
        TH1 *varHist = data->histMap[jetBinName];

        std::cout << Form("%16.2f",varHist->Integral()) << "  ";
      }
      std::cout << std::endl;
    } // end if data

    if (!fitResult) {
      std::cout << "S/B         =     " << nSignal / nBkg << std::endl
                << "S/SQRT(B)   =     " << nSignal / sqrt(nBkg) << std::endl
                << "S/SQRT(S+B) =     " << nSignal / sqrt(nSignal + nBkg) << std::endl;
    }
  }
}

#endif
