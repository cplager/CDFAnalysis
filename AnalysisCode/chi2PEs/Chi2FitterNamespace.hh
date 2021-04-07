// -*- C++ -*-

#if !defined(Chi2FitterNamespace_HH)
#define Chi2FitterNamespace_HH

#include "TH1F.h"

#include "CLPTemplateFitter/CLPTemplateFitter.hh"

#include "RunAccept.hh"
#include "RunFraction.hh"
#include "TagFraction.hh"

namespace Chi2FitterNamespace
{
   //////////////////////////////
   // //////////////////////// //
   // // Enums and Typedefs // //
   // //////////////////////// //
   //////////////////////////////

   enum
   {
      kFCNC_WZ,
      kFCNC_ZZ,
      kZjets,
      kSMTTbar,
      kDiboson,
      kNumFlavors,
   };

   enum
   {
      kTag,
      kAntiTag,
      kControl,
      kNumTypes,
   };
   
   /////////////////////
   // /////////////// //
   // // Variables // //
   // /////////////// //
   /////////////////////

   // The fitter
   extern CLPTemplateFitter ns_fitter;

   // the templates
   extern TH1F* ns_dataHPtr;
   extern TH1F* ns_fcncWZHPtr;
   extern TH1F* ns_fcncZZHPtr;
   extern TH1F* ns_zjet1HPtr;
   extern TH1F* ns_zjet2HPtr;
   extern TH1F* ns_zjet3HPtr;
   extern TH1F* ns_smttbarHPtr;
   extern TH1F* ns_dibosonHPtr;
   // For sanity checks
   extern TH1F* ns_alp20Zjet1HPtr;
   extern TH1F* ns_alp20Zjet2HPtr;
   extern TH1F* ns_alp20Zjet3HPtr;
   extern TH1F* ns_alp05Zjet1HPtr;
   extern TH1F* ns_alp05Zjet2HPtr;
   extern TH1F* ns_alp05Zjet3HPtr;
   

   // fitter indicies of the templates
   extern int ns_dataIndex;
   extern int ns_fcncWZIndex;
   extern int ns_fcncZZIndex;
   extern int ns_zjet1Index;
   extern int ns_zjet2Index;
   extern int ns_zjet3Index;
   extern int ns_smttbarIndex;
   extern int ns_dibosonIndex;
   extern int ns_fcncBFIndex;
   extern int ns_jesIndex;
   extern int ns_signalZjetNormIndex;
   extern int ns_signalZjetTagIndex;

   // template names
   const std::string kFcncWZName  = "Awz"; 
   const std::string kFcncZZName  = "Azz";
   const std::string kZjet1Name   = "zjet1";
   const std::string kZjet2Name   = "zjet2";
   const std::string kZjet3Name   = "zjet3";
   const std::string kSmttbarName = "smttbar";
   const std::string kDibosonName = "diboson";
   const std::string kFcncBFName  = "fcncBF";
   const std::string kJesName     = "jes";
   const std::string kZjetFractionName = "zjetFrac";
   const std::string kZjetTagFracName  = "zjetTag";

   // Functionoids describing running acceptances
   extern RunAccept ns_wzAcc;
   extern RunAccept ns_zzAcc;

   // Functionoids describing signal region Z + jet normalizations
   extern RunFraction ns_signalZjetNormToid;
   extern TagFraction ns_zjetTagFracToid;

   // command line options
   extern int    ns_histNumber;
   extern int    ns_minuitVerbose;   
   extern int    ns_numBins1;
   extern int    ns_numBins2;
   extern int    ns_numBins3;
   extern int    ns_rebin1;
   extern int    ns_rebin2;
   extern int    ns_rebin3;
   extern int    ns_numDefaultBins;
   extern int    ns_whichTemplates;
   extern int    ns_verbose;
   extern bool   ns_2fb;
   extern bool   ns_constrain;
   extern bool   ns_makePlots;
   extern bool   ns_renormalizeAll;
   extern bool   ns_useNewData;
   extern bool   ns_ignoreJes;
   extern bool   ns_doMinos;
   extern bool   ns_throwAccSyst;
   extern bool   ns_170GeV;
   extern bool   ns_as175GeV;
   extern double ns_constraintPercent;
   extern double ns_antiBack;
   extern double ns_tagBack;
   extern double ns_constrJes;
   extern double ns_minExpectedBinContent;
   extern double ns_constrainZjet1;
   extern double ns_constrainZjet2;
   extern double ns_constrainZjet3;
   extern double ns_signalFactor;

   // Bookkeeping variables
   extern double ns_data1Integral;
   extern double ns_data2Integral;
   extern double ns_data3Integral;
   extern int    ns_totalBins;

   ////////////////////////////////
   // ////////////////////////// //
   // // Function Definitions // //
   // ////////////////////////// //
   ////////////////////////////////

   // hook up this namespaces OptionUtils
   void setupOptions();

   // scan the branching fraction manually
   double scanBF();

   // Fit the branching fraction.
   void fitBF (double startingBF = 0., bool reset = false);

   // initialize the fitter according to the mode flag
   void initializeFitter();
   void _loadTemplates();
   void _loadMorphingTemplates();
   void _hookupFitter();
   void _makeEverythingPretty();

   // Assumes that outputPtr goes from 0.5 to N + 0.5
   void addToHistogram (TH1F *outputPtr, TH1F *inputPtr,  
                        int offset, int rebin, float scale = 1.);
};

#endif // Chi2FitterNamespace_HH
