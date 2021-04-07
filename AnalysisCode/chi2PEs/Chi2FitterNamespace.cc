// -*- C++ -*-

#include "TCanvas.h"
#include "TFile.h"

#include "Chi2FitterNamespace.hh"
#include "CLPTemplateFitter/dout.hh"
#include "CLPTemplateMorph/CLPTemplateMorph.hh"
#include "RootFileCollection/OptionUtils.h"

using namespace std;

//////////////////////////
// Variable Definitions //
//////////////////////////

CLPTemplateFitter Chi2FitterNamespace::ns_fitter;

TH1F* Chi2FitterNamespace::ns_dataHPtr     = 0;
TH1F* Chi2FitterNamespace::ns_fcncWZHPtr   = 0;
TH1F* Chi2FitterNamespace::ns_fcncZZHPtr   = 0;
TH1F* Chi2FitterNamespace::ns_zjet1HPtr    = 0;
TH1F* Chi2FitterNamespace::ns_zjet2HPtr    = 0;
TH1F* Chi2FitterNamespace::ns_zjet3HPtr    = 0;
TH1F* Chi2FitterNamespace::ns_smttbarHPtr  = 0;
TH1F* Chi2FitterNamespace::ns_dibosonHPtr  = 0;
TH1F* Chi2FitterNamespace::ns_alp20Zjet1HPtr = 0;
TH1F* Chi2FitterNamespace::ns_alp20Zjet2HPtr = 0;
TH1F* Chi2FitterNamespace::ns_alp20Zjet3HPtr = 0;
TH1F* Chi2FitterNamespace::ns_alp05Zjet1HPtr = 0;
TH1F* Chi2FitterNamespace::ns_alp05Zjet2HPtr = 0;
TH1F* Chi2FitterNamespace::ns_alp05Zjet3HPtr = 0;

int Chi2FitterNamespace::ns_dataIndex           = -1;
int Chi2FitterNamespace::ns_fcncWZIndex         = -1;
int Chi2FitterNamespace::ns_fcncZZIndex         = -1;
int Chi2FitterNamespace::ns_zjet1Index          = -1;
int Chi2FitterNamespace::ns_zjet2Index          = -1;
int Chi2FitterNamespace::ns_zjet3Index          = -1;
int Chi2FitterNamespace::ns_smttbarIndex        = -1;
int Chi2FitterNamespace::ns_dibosonIndex        = -1;
int Chi2FitterNamespace::ns_fcncBFIndex         = -1;
int Chi2FitterNamespace::ns_jesIndex            = -1;
int Chi2FitterNamespace::ns_signalZjetNormIndex = -1;
int Chi2FitterNamespace::ns_signalZjetTagIndex  = -1;


RunAccept Chi2FitterNamespace::ns_wzAcc;
RunAccept Chi2FitterNamespace::ns_zzAcc;
RunFraction Chi2FitterNamespace::ns_signalZjetNormToid;
TagFraction Chi2FitterNamespace::ns_zjetTagFracToid;

int    Chi2FitterNamespace::ns_histNumber            = 4;
int    Chi2FitterNamespace::ns_minuitVerbose         = -1;
int    Chi2FitterNamespace::ns_numBins1              = 0;
int    Chi2FitterNamespace::ns_numBins2              = 0;
int    Chi2FitterNamespace::ns_numBins3              = 0;
int    Chi2FitterNamespace::ns_rebin1                = 10;
int    Chi2FitterNamespace::ns_rebin2                = 4;
int    Chi2FitterNamespace::ns_rebin3                = 4;
int    Chi2FitterNamespace::ns_numDefaultBins        = 100;
int    Chi2FitterNamespace::ns_whichTemplates        = -5;
int    Chi2FitterNamespace::ns_verbose               = 0;
bool   Chi2FitterNamespace::ns_2fb                   = true;
bool   Chi2FitterNamespace::ns_constrain             = false; 
bool   Chi2FitterNamespace::ns_makePlots             = false;
bool   Chi2FitterNamespace::ns_renormalizeAll        = false;
bool   Chi2FitterNamespace::ns_useNewData            = false;
bool   Chi2FitterNamespace::ns_ignoreJes             = false;
bool   Chi2FitterNamespace::ns_doMinos               = false;
bool   Chi2FitterNamespace::ns_throwAccSyst          = true;
bool   Chi2FitterNamespace::ns_170GeV                = false;
bool   Chi2FitterNamespace::ns_as175GeV              = true;
double Chi2FitterNamespace::ns_constraintPercent     = 0.20;
double Chi2FitterNamespace::ns_antiBack              = 0;
double Chi2FitterNamespace::ns_tagBack               = 0;
double Chi2FitterNamespace::ns_constrJes             = 0;
double Chi2FitterNamespace::ns_minExpectedBinContent = 0.01;
double Chi2FitterNamespace::ns_constrainZjet1        = 0.;
double Chi2FitterNamespace::ns_constrainZjet2        = 0.;
double Chi2FitterNamespace::ns_constrainZjet3        = 0.;
double Chi2FitterNamespace::ns_signalFactor          = 0.;

double Chi2FitterNamespace::ns_data1Integral     = 0.;
double Chi2FitterNamespace::ns_data2Integral     = 0.;
double Chi2FitterNamespace::ns_data3Integral     = 0.;
int    Chi2FitterNamespace::ns_totalBins         = 0;

//////////////////////////
// Function Definitions //
//////////////////////////

double
Chi2FitterNamespace::scanBF()
{
   TFile *output = TFile::Open ("red.root", "recreate");
   TH1F *peHist = (TH1F*) ns_fitter.getData()->Clone ("pe");
   peHist->SetDirectory (output);
   ns_fitter.resetMinuitParameters();
   ns_fitter.fixParameter (kFcncBFName);
   for (double bf = -.35; bf < 0; bf += 0.01)
   {
      ns_fitter.resetMinuitParameters();
      ns_fitter.setParameter (kFcncBFName, bf);
      fitBF (bf);
      double logLikeli = ns_fitter.logLikelihoodValue();
      dout << Form ("%5.2f  %f", bf, logLikeli) << endl;
      ns_fitter.outputFitResults();
      TH1F *totalHPtr = ns_fitter.updatedHistogram();
      TString name = Form ("bfm%.0f", fabs(100*bf));
      cout << "name " << name << endl;
      totalHPtr->SetName (name);
      totalHPtr->SetDirectory (output);
   }
   output->Write();
   ns_fitter.releaseParameter (kFcncBFName);
   return -1;
}

void
Chi2FitterNamespace::fitBF (double startingBF, bool reset)
{
   static CLPTemplateFitter::DVec morphParamVec (1, 0);
   static CLPTemplateFitter::DVec paramVec (ns_fitter.size(), 0);
   static bool firstTime = true;
   static CLPTemplateMorph::PtrVec morphPtrVec;
   static double numWanted1, numWanted2, numWanted3;
   static const TH1F* dataHPtr;
   if (firstTime)
   {
      firstTime = false;
      // Get the morphs back as morphs
      CLPTemplateMorph *zjet1Ptr = 
         dynamic_cast < CLPTemplateMorph* > ( ns_zjet1HPtr );
      CLPTemplateMorph *zjet2Ptr = 
         dynamic_cast < CLPTemplateMorph* > ( ns_zjet2HPtr );
      CLPTemplateMorph *zjet3Ptr = 
         dynamic_cast < CLPTemplateMorph* > ( ns_zjet3HPtr );
      CLPTemplateMorph *fcncWZPtr = 
         dynamic_cast < CLPTemplateMorph* > ( ns_fcncWZHPtr );
      CLPTemplateMorph *fcncZZPtr = 
         dynamic_cast < CLPTemplateMorph* > ( ns_fcncZZHPtr );
      CLPTemplateMorph *smttbarPtr = 
         dynamic_cast < CLPTemplateMorph* > ( ns_smttbarHPtr );
      CLPTemplateMorph *dibosonPtr = 
         dynamic_cast < CLPTemplateMorph* > ( ns_dibosonHPtr );
      morphPtrVec.push_back (zjet1Ptr);
      morphPtrVec.push_back (zjet2Ptr);
      morphPtrVec.push_back (zjet3Ptr);
      morphPtrVec.push_back (fcncWZPtr);
      morphPtrVec.push_back (fcncZZPtr);
      if (smttbarPtr) morphPtrVec.push_back (smttbarPtr);
      if (dibosonPtr) morphPtrVec.push_back (dibosonPtr);
   }
   if (firstTime || reset)
   {
      dataHPtr = ns_fitter.getData();
      numWanted1 = dataHPtr->Integral (1, ns_numBins1);
      numWanted2 = dataHPtr->Integral (ns_numBins1 + 1, 
                                       ns_numBins1 + ns_numBins2);
      numWanted3 = dataHPtr->Integral (ns_numBins1 + ns_numBins2 + 1,
                                       ns_numBins1 + ns_numBins2 + 
                                       ns_numBins3);
      CLPTemplateMorph::setParamVecs (morphPtrVec, morphParamVec);
   }
   paramVec.at( ns_fcncWZIndex  ) =  1.;
   paramVec.at( ns_fcncZZIndex  ) =  1.;
   paramVec.at( ns_fcncBFIndex  ) =  startingBF;
   paramVec.at( ns_zjet1Index   ) =  0.;
   paramVec.at( ns_zjet2Index   ) =  0.; 
   paramVec.at( ns_zjet3Index   ) =  0.;
   paramVec.at( ns_smttbarIndex ) =  1.;      
   paramVec.at( ns_dibosonIndex ) =  1.;      
   paramVec.at( ns_jesIndex     ) =  0.;
   TH1F *totalHPtr = ns_fitter.updatedHistogram (paramVec);
   totalHPtr->Integral (1, ns_numBins1);
   double zjet1 = 
      numWanted1 - totalHPtr->Integral (1, ns_numBins1);
   double zjet2 = 
      numWanted2 - totalHPtr->Integral (ns_numBins1 + 1, 
                                        ns_numBins1 + ns_numBins2);
   double zjet3 = 
      numWanted3 - totalHPtr->Integral (ns_numBins1 + ns_numBins2 + 1,
                                        ns_numBins1 + ns_numBins2 +
                                        ns_numBins3);
   delete totalHPtr;
   if (ns_whichTemplates <= -5)
   {
      // tag fraction is easy
      double tagFraction = zjet1 / (zjet1 + zjet2);
      // signal to control fraction is not so easy.
      double fractionWanted = (zjet1 + zjet2) / zjet3;
      // o.k.  I thought I was being clever in having the fractions
      // take a unit Gaussian as input.  Unfortunately, this means
      // that I will have trouble inverting this function.  In this
      // case, I think I will assume this function is linear and then
      // solve for the most likely value (remember that we are only
      // seeding the fit, so if I'm close but off it doesn't matter).
      double zero = ns_signalZjetNormToid (1., 0., 0.);
      double one  = ns_signalZjetNormToid (1., 1., 0.);
      double slope = one - zero;
      assert (slope);
      double fracValue = (fractionWanted - zero) / slope;
      ns_fitter.setParameter (kZjetFractionName, fracValue);
      ns_fitter.setParameter (kZjetTagFracName,  tagFraction);
   } else {
      ns_fitter.setParameter (kZjet1Name, zjet1);
      ns_fitter.setParameter (kZjet2Name, zjet2);
   }
   ns_fitter.setParameter (kZjet3Name, zjet3);
   // paramVec.at( ns_zjet1Index   ) =  zjet1;
   // paramVec.at( ns_zjet2Index   ) =  zjet2; 
   // paramVec.at( ns_zjet3Index   ) =  zjet3;
   // double before = ns_fitter.logLikelihoodValue (paramVec);
   ns_fitter.fit();
   // double after  = ns_fitter.logLikelihoodValue ();
}

void
Chi2FitterNamespace::initializeFitter()
{
   if (ns_whichTemplates <= -2)
   {
      ns_useNewData = true;
   }
   _loadTemplates();
   _makeEverythingPretty();
   _hookupFitter();
   ns_fitter.setDoMinos (ns_doMinos);
}

void
Chi2FitterNamespace::_loadTemplates()
{
   // The histograms inside are indexed from 0 to 5 with the following cuts:
   // 0: weight*(njets>=4)
   // 1: weight*(njets>=4)*wloose
   // 2: weight*(njets>=4)*(1-wloose)
   // 3: weight*(njets>=4)*(Et1>40)*(Et2>30)*(Et3>20)*(Et4>15)*(mt>200)
   // 4: weight*(njets>=4)*wloose*(Et1>40)*(Et2>30)*(Et3>20)*(Et4>15)*(mt>200)
   // 5: weight*(njets>=4)*(1-wloose)*(Et1>40)*(Et2>30)*(Et3>20)*(Et4>15)*
   //                      (mt>200) 
   double wzArray[8] = { 73.80, 40.30,  33.50,  66.44, 
                         37.13, 29.31 , 27.85,  21.98};
   double zzArray[8] = {247.08, 96.82, 150.26, 221.95, 
                        88.26, 133.69,  66.20, 100.27};

   ns_numBins1 = 100 / ns_rebin1;
   ns_numBins2 = 100 / ns_rebin2;
   ns_numBins3 = 100 / ns_rebin3;
   ns_totalBins = ns_numBins1 + ns_numBins2;
   TString histname1 = Form ("hsum_chi_%d", ns_histNumber);
   TString histname2 = Form ("hsum_chi_%d", ns_histNumber + 1);   

   //////////
   // Data //
   //////////
   double lumiFactor = 1.;
   if (ns_useNewData)
   {
      TFile *dataFile = TFile::Open ("histograms/data_19_080115.root");
      if (ns_170GeV && ! ns_as175GeV)
      {
         dataFile = TFile::Open ("histograms/20080317_data_170.root");
      }
      assert (dataFile);
      lumiFactor = 1.9 / 1.12;
      TH1F *data1 = (TH1F*) dataFile->Get("data_tagC");
      TH1F *data2 = (TH1F*) dataFile->Get("data_antiC");
      TH1F *data3 = (TH1F*) dataFile->Get("data_contC");
      ns_data1Integral = data1->Integral();
      ns_data2Integral = data2->Integral();
      if (ns_whichTemplates <= -2)
      {
         ns_totalBins = ns_numBins1 + ns_numBins2 + ns_numBins3;
         ns_data3Integral = data3->Integral();

      }
      ns_dataHPtr = new TH1F ("data", "data", ns_totalBins, 
                              0.5, 0.5 + ns_totalBins);
      addToHistogram (ns_dataHPtr, data1, 0, 10 * ns_rebin1);
      addToHistogram (ns_dataHPtr, data2, ns_numBins1, 10 * ns_rebin2);
      if (ns_whichTemplates <= -2)
      {
         addToHistogram (ns_dataHPtr, data3, ns_numBins1 + ns_numBins2, 
                         10 *ns_rebin3);
      }
   } else {
      TFile *dataFile = 0;
      if (ns_2fb)
      {
         dataFile = TFile::Open ("histograms/data_1.7invfb.root");
         lumiFactor = 1.73 / 1.12;
      } else {
         dataFile = TFile::Open ("histograms/data_112.root");
      }
      TH1F *data1 = (TH1F*) dataFile->Get(histname1);
      TH1F *data2 = (TH1F*) dataFile->Get(histname2);
      ns_data1Integral = data1->Integral();
      ns_data2Integral = data2->Integral();
      // data
      ns_dataHPtr = new TH1F ("data", "data", ns_totalBins, 
                              0.5, 0.5 + ns_totalBins);
      addToHistogram (ns_dataHPtr, data1, 0, ns_rebin1);
      addToHistogram (ns_dataHPtr, data2, ns_numBins1, ns_rebin2);
   }

   TFile *zjetFile = 0;
   TFile *AwzFile  = 0;
   TFile *AzzFile  = 0;
   if (0 == ns_whichTemplates)
   {
      // Old templates
      zjetFile = TFile::Open ("histograms/zjets.root");
      AwzFile  = TFile::Open ("histograms/Awz.root");
      AzzFile  = TFile::Open ("histograms/Azz.root");
   } else if (1 == ns_whichTemplates) 
   {
      // new templates
      zjetFile = TFile::Open ("histograms/background_1.7invfb.root");
      AwzFile  = TFile::Open ("histograms/signal_1.7invfb.root");
      AzzFile  = TFile::Open ("histograms/zczc_1.7invfb.root");
   } else if (2 == ns_whichTemplates) 
   {
      // new templates JES up
      zjetFile = TFile::Open ("histograms/background_1.7_jes_up.root");
      AwzFile  = TFile::Open ("histograms/signal_1.7_jes_up.root");
      AzzFile  = TFile::Open ("histograms/zczc_1.7_jes_up.root");
   } else if (3 == ns_whichTemplates) 
   {
      // new templates JES down
      zjetFile = TFile::Open ("histograms/background_1.7_jes_down.root");
      AwzFile  = TFile::Open ("histograms/signal_1.7_jes_down.root");
      AzzFile  = TFile::Open ("histograms/zczc_1.7_jes_down.root");
   } else if (ns_whichTemplates < 0)
   {
      _loadMorphingTemplates();
      return;
   }

   //////////////
   // Z + Jets //
   //////////////
   TH1F *zjet1a = (TH1F*) zjetFile->Get(histname1);
   TH1F *zjet2a = (TH1F*) zjetFile->Get(histname2);

   ///////////////////
   // WZ Acceptance //
   ///////////////////
   TH1F *Awz1 = (TH1F*) AwzFile->Get(histname1);
   TH1F *Awz2 = (TH1F*) AwzFile->Get(histname2);

   ///////////////////
   // ZZ Acceptance //
   ///////////////////
   TH1F *Azz1 = (TH1F*) AzzFile->Get(histname1);
   TH1F *Azz2 = (TH1F*) AzzFile->Get(histname2);

   double Awz1Norm = wzArray[ns_histNumber];
   double Awz2Norm = wzArray[ns_histNumber + 1];
   double Azz1Norm = 0.589 * zzArray[ns_histNumber];
   double Azz2Norm = 0.589 * zzArray[ns_histNumber + 1];

   Awz1Norm *= lumiFactor;
   Awz2Norm *= lumiFactor;
   Azz1Norm *= lumiFactor;
   Azz2Norm *= lumiFactor;

   /////////////////////////////
   // Assemble the histograms //
   /////////////////////////////
   // background
   ns_zjet1HPtr = new TH1F ("zjet1", "zjet1", ns_totalBins, 
                            0.5, 0.5 + ns_totalBins);
   ns_zjet2HPtr = new TH1F ("zjet2", "zjet2", ns_totalBins, 
                            0.5, 0.5 + ns_totalBins);
   addToHistogram (ns_zjet1HPtr, zjet1a, 0,        ns_rebin1, 
                   1. / zjet1a->Integral());
   addToHistogram (ns_zjet2HPtr, zjet2a, ns_numBins1, ns_rebin2, 
                   1. / zjet2a->Integral());
   TH1F *zjet  = new TH1F ("zjet", "zjet", ns_totalBins, 
                           0.5, 0.5 + ns_totalBins);
   double norm = zjet1a->Integral() + zjet2a->Integral();
   addToHistogram (zjet,  zjet1a, 0,        ns_rebin1, 1. / norm);
   addToHistogram (zjet,  zjet2a, ns_numBins1, ns_rebin2, 1. / norm);
   // WZ acceptance
   ns_fcncWZHPtr = new TH1F ("Awz", "Awz", ns_totalBins, 
                             0.5, 0.5 + ns_totalBins);
   addToHistogram (ns_fcncWZHPtr, Awz1, 0,        ns_rebin1, 
                   Awz1Norm / Awz1->Integral());
   addToHistogram (ns_fcncWZHPtr, Awz2, ns_numBins1, ns_rebin2, 
                   Awz2Norm / Awz2->Integral());
   // ZZ acceptance
   ns_fcncZZHPtr = new TH1F ("Azz", "Azz", ns_totalBins, 
                             0.5, 0.5 + ns_totalBins);
   addToHistogram (ns_fcncZZHPtr, Azz1, 0,        ns_rebin1, 
                   Azz1Norm / Azz1->Integral());
   addToHistogram (ns_fcncZZHPtr, Azz2, ns_numBins1, ns_rebin2, 
                   Azz2Norm / Azz2->Integral());
}

void
Chi2FitterNamespace::_loadMorphingTemplates()
{
   const int kNumJes   = 4;
   const TString types [kNumTypes]  = {"_tag", "_anti", "_cont"};
   const TString jes[kNumJes]      = {"_jesP2_0", "_jesP1_0", 
                                      "_jesM1_0", "_jesM2_0"};
   const double  jesValues[kNumJes] =
      { 2.0, 1.0, -1.0, -2.0 };
   int numBins[kNumTypes] = {ns_numBins1,
                             ns_numBins2,
                             ns_numBins3};
   const TString flavors[kNumFlavors] = {"fcnc_wz", "fcnc_zz", "zjets", 
                                         "smttbar", "diboson"};

   int numTypesUsed = 2;
   if (ns_whichTemplates <= -2)
   {
      numTypesUsed = 3;
   }
   int numFlavorsUsed = kZjets + 1;
   if (ns_whichTemplates <= -3)
   {
      numFlavorsUsed = kNumFlavors;
   }
   //TFile *templateFile = TFile::Open ("histograms/all_templates_v7.root");
   //TFile *templateFile = TFile::Open ("histograms/20080115_all_templates.root");
   TFile *templateFile = TFile::Open ("histograms/20080208_all_templates.root");
   if (ns_170GeV)
   {
      if (ns_as175GeV) 
      {
         templateFile = 
            TFile::Open ("histograms/20080317_all_templates_170_175.root");
      } else {
         templateFile = 
            TFile::Open ("histograms/20080317_all_templates_170.root");
      }
   }
   assert (templateFile);
   /////////////////////////////////////
   // Setup Empty Morphs for Z + Jets //
   /////////////////////////////////////
   TH1F *emptyHistPtrs[kNumTypes] = {};
   CLPTemplateMorph *emptyMorphPtrs[kNumTypes] = {};
   for (int typeIndex = 0; typeIndex < kNumTypes; ++typeIndex)
   {
      TString histName  = Form ("temphist%d",  typeIndex);
      TString morphName = Form ("morphhist%d", typeIndex);
      emptyHistPtrs[typeIndex] = 
         new TH1F (histName, histName, numBins[typeIndex], 0, 10);
      emptyMorphPtrs[typeIndex] =
         new CLPTemplateMorph (morphName, morphName,
                               numBins[typeIndex], 0, 10);
      emptyMorphPtrs[typeIndex]->addDefaultTemplate (emptyHistPtrs[typeIndex]);
   } // for typeIndex
   
   ///////////////////////////
   // Loop over all Flavors //
   ///////////////////////////
   for (int flavorIndex = 0; flavorIndex < numFlavorsUsed; ++flavorIndex)
   {
      TString name = Form ("%sMorph", (const char*) flavors[flavorIndex]);
      CLPTemplateMorph *morphPtr = new CLPTemplateMorph (name, name);
      for (int typeIndex = 0; typeIndex < numTypesUsed; ++typeIndex)
      {
         ///////////////////////
         // Get Default Shape //
         ///////////////////////
         name = Form ("%s%s",
                      (const char*) flavors[flavorIndex],
                      (const char*) types[typeIndex]);
         TString morphName = name + "Morph";
         CLPTemplateMorph *tempMorphPtr = 
            new CLPTemplateMorph (morphName, morphName, 
                                  numBins[typeIndex], 0, 10);
         TH1F *histPtr = (TH1F*) templateFile->Get (name);
         assert (histPtr);
         histPtr->Rebin(10);
         if (kZjets == flavorIndex || ns_renormalizeAll)
         {
            // normalize to unit area
            histPtr->Scale (1. / histPtr->Integral());
         }
         if (ns_signalFactor && 
             (kFCNC_WZ == flavorIndex || kFCNC_ZZ == flavorIndex))
         {
            histPtr->Scale (ns_signalFactor);
            // cout << "scaling " << histPtr->GetName() << " by "
            //      << ns_signalFactor << endl;
         }
         tempMorphPtr->addDefaultTemplate (histPtr);

         ////////////////////
         // Get JES Shapes //
         ////////////////////
         tempMorphPtr->addMorphVariable (kJesName, 0.);
         for (int jesIndex = 0; jesIndex < kNumJes; ++jesIndex)
         {
            TString jesValString = Form ("%0.1f", 
                                         fabs(jesValues[jesIndex]));
            TString jesName = Form ("%s%s%s",
                                    (const char*) flavors[flavorIndex],
                                    (const char*) jes[jesIndex],
                                    (const char*) types[typeIndex]);
            if ("0.5" == jesValString) {
               //cout << "Skipping " << jesName << endl;
               continue;
            }
            if (fabs(jesValues[jesIndex] > 1.5)) continue;
            histPtr = (TH1F*) templateFile->Get (jesName);
            if (kZjets == flavorIndex && kAntiTag == typeIndex)
            {
               TCanvas c1;
               histPtr->Draw();
               //c1.Print( "plots/" + jesName + ".eps");               
            } // debug
            assert (histPtr);
            histPtr->Rebin(10);
            if (ns_signalFactor && 
                (kFCNC_WZ == flavorIndex || kFCNC_ZZ == flavorIndex))
            {
               histPtr->Scale (ns_signalFactor);
               // cout << "scaling " << histPtr->GetName() << " by "
               //      << ns_signalFactor << endl;
            }
            if (kZjets == flavorIndex || ns_renormalizeAll)
            {
               // normalize to unit area
               histPtr->Scale (1. / histPtr->Integral());
            }
            tempMorphPtr->addMorphTemplate (kJesName, 
                                            jesValues[jesIndex],
                                            histPtr);
         } // for jesIndex
         // Since we let the different Z + jet components float
         // independently, we store these in separate pointers.
         // :NOTE: 
         if (kZjets == flavorIndex)
         {
            TString morphName = Form ("zjets%sMorphPiece", 
                                      (const char*) types[typeIndex]);
            CLPTemplateMorph *zjetMorphPtr = 
               new CLPTemplateMorph (morphName, morphName);
            CLPTemplateMorph *morphPtrArray[kNumTypes] =
               {emptyMorphPtrs[0], emptyMorphPtrs[1], emptyMorphPtrs[2]};
            morphPtrArray[typeIndex] = tempMorphPtr;
            for (int inner = 0; inner < numTypesUsed; ++inner)
            {
               zjetMorphPtr->addMorph (*(morphPtrArray[inner]));
            } // for inner

            // assign zjetMorphPtr to the right holder
            switch (typeIndex)
            {
               case kTag:
                  ns_zjet1HPtr = zjetMorphPtr;
                  break;
               case kAntiTag:
                  ns_zjet2HPtr = zjetMorphPtr;
                  break;
               case kControl:
                  ns_zjet3HPtr = zjetMorphPtr;
                  break;
               default:
                  assert (0);
                  break;
            } // switch typeIndex
         } else {
            morphPtr->addMorph (*tempMorphPtr);
         }
      } // for typeIndex
      switch (flavorIndex)
      {
         case kFCNC_WZ:
            ns_fcncWZHPtr = (TH1F*) morphPtr;
            break;
         case kFCNC_ZZ:
            ns_fcncZZHPtr = (TH1F*) morphPtr;
            break;
         case kZjets:
            // we don't use the morphPtr in this case, so get rid of
            // it.
            delete morphPtr;
            break;
         case kSMTTbar:
            ns_smttbarHPtr = (TH1F*) morphPtr;
            break;
         case kDiboson:
            ns_dibosonHPtr = (TH1F*) morphPtr;
            break;
         default:
            assert (0);
            break;
      }
      /////////////////////////////////////
      // Do NOT delete the tempMorphPtrs //
      /////////////////////////////////////

   } // for flavorIndex
   CLPTemplateFitter::DVec paramVec (1, 0);
   for (double jes = -1.5; jes < 1.6; jes += 0.5)
   {
      TCanvas c1;
      CLPTemplateMorph *morphPtr = 
         dynamic_cast< CLPTemplateMorph* > (ns_zjet2HPtr);
      if (! morphPtr) break;
      paramVec.at (0) = jes;
      morphPtr->setParamVec (paramVec);
      morphPtr->Draw();
      //c1.Print( Form ("plots/antijet%.1f.eps", jes) );
   } // for jes

   if (0)
   {
      // load Alpgen templates
      // KFac/QFac 2.0
      ns_alp20Zjet1HPtr = new TH1F ("alp20Z1", "alp20Z1", ns_totalBins,
                                    0.5, 0.5 + ns_totalBins);
      ns_alp20Zjet2HPtr = new TH1F ("alp20Z2", "alp20Z2", ns_totalBins,
                                    0.5, 0.5 + ns_totalBins);
      ns_alp20Zjet3HPtr = new TH1F ("alp20Z3", "alp20Z3", ns_totalBins,
                                  0.5, 0.5 + ns_totalBins);
      TH1F *alp20_1 = (TH1F*) templateFile->Get ("zjets_alpQ2_0_tag");
      TH1F *alp20_2 = (TH1F*) templateFile->Get ("zjets_alpQ2_0_anti");
      TH1F *alp20_3 = (TH1F*) templateFile->Get ("zjets_alpQ2_0_cont");
      addToHistogram (ns_alp20Zjet1HPtr, alp20_1, 0, 
                      10 * ns_rebin1, 1 / alp20_1->Integral());
      addToHistogram (ns_alp20Zjet2HPtr, alp20_2, ns_numBins1, 
                      10 * ns_rebin2, 1 / alp20_2->Integral());
      addToHistogram (ns_alp20Zjet3HPtr, alp20_3, ns_numBins1 + ns_numBins2, 
                      10 * ns_rebin3, 1 / alp20_3->Integral());
      // KFac/QFac 0.5
      ns_alp05Zjet1HPtr = new TH1F ("alp05Z1", "alp05Z1", ns_totalBins,
                                    0.5, 0.5 + ns_totalBins);
      ns_alp05Zjet2HPtr = new TH1F ("alp05Z2", "alp05Z2", ns_totalBins,
                                    0.5, 0.5 + ns_totalBins);
      ns_alp05Zjet3HPtr = new TH1F ("alp05Z3", "alp05Z3", ns_totalBins,
                                  0.5, 0.5 + ns_totalBins);
      TH1F *alp05_1 = (TH1F*) templateFile->Get ("zjets_alpQ0_5_tag");
      TH1F *alp05_2 = (TH1F*) templateFile->Get ("zjets_alpQ0_5_anti");
      TH1F *alp05_3 = (TH1F*) templateFile->Get ("zjets_alpQ0_5_cont");
      addToHistogram (ns_alp05Zjet1HPtr, alp05_1, 0, 
                      10 * ns_rebin1, 1 / alp05_1->Integral());
      addToHistogram (ns_alp05Zjet2HPtr, alp05_2, ns_numBins1, 
                      10 * ns_rebin2, 1 / alp05_2->Integral());
      addToHistogram (ns_alp05Zjet3HPtr, alp05_3, ns_numBins1 + ns_numBins2, 
                      10 * ns_rebin3, 1 / alp05_3->Integral());
   }
}

void
Chi2FitterNamespace::_hookupFitter()
{
   // These come from the ratio of LJ acceptances.
   // from ( -1.358, 0.378), to (-1.3645, 0.3840) 
   double denommu1 = -1.358; // -1.3645;
   double denommu2 =  0.378; //  0.3840;
   if (ns_170GeV)
   {
      denommu1 = -1.2655;
      denommu2 =  0.2863;
   }
   // Add Data
   ns_fitter.setMinuitVerboseLevel (ns_minuitVerbose);
   ns_fitter.addData (ns_dataHPtr);
   ns_fitter.setMinExpectedBinContent(ns_minExpectedBinContent);
   //////////////
   // Z + Jets //
   //////////////
   if (ns_whichTemplates <= -5)
   {
      ////////////////////////////////////////////////////////////////
      // Signal Region Z + Jets Are Tied To Control Region Z + Jets //
      ////////////////////////////////////////////////////////////////
      // add the first two Z + jet templates and fix them to unit area
      ns_zjet1Index = ns_fitter.addTemplate (kZjet1Name, ns_zjet1HPtr,
                                             1., 0.9, 1.1, 0.01);
      ns_zjet2Index = ns_fitter.addTemplate (kZjet2Name, ns_zjet2HPtr,
                                            1., 0.9, 1.1, 0.01);
      ns_fitter.fixParameter (kZjet1Name);
      ns_fitter.fixParameter (kZjet2Name);
      // Add the control region Z + jets
      if (ns_constrainZjet3)
      {
         double constraint3 = ns_constraintPercent * ns_constrainZjet3;
         ns_zjet3Index = ns_fitter.addTemplate (kZjet3Name, ns_zjet3HPtr, 
                                                ns_constrainZjet3,
                                                ns_constrainZjet3, 
                                                constraint3, -300, 300, 0.1);
      } else {
         ns_zjet3Index = ns_fitter.addTemplate (kZjet3Name, ns_zjet3HPtr, 
                                                ns_data3Integral,
                                                0., 0., -300., 300., 0.1);
      }
      // Start setting up the two functiods now.  We won't be able to
      // finish them until we have added everything else.
   } else {
      ///////////////////////////////////////////////
      // Three Z + Jet Regions Float Independently //
      ///////////////////////////////////////////////
      double constraint1;
      double constraint2;
      double constraint3;
      if (ns_constraintPercent)
      {
         constraint1 = ns_constraintPercent * ns_constrainZjet1;
         constraint2 = ns_constraintPercent * ns_constrainZjet2;
         constraint3 = ns_constraintPercent * ns_constrainZjet3;
      } else {
         constraint1 = sqrt( ns_constrainZjet1 );
         constraint2 = sqrt( ns_constrainZjet2 );
         constraint3 = sqrt( ns_constrainZjet3 );
      }
      if (ns_constrainZjet1)
      {
         ns_zjet1Index = ns_fitter.addTemplate (kZjet1Name, ns_zjet1HPtr, 
                                                ns_constrainZjet1,
                                                ns_constrainZjet1, 
                                                constraint1, -100., 100., 0.1);
      } else {
         ns_zjet1Index = ns_fitter.addTemplate (kZjet1Name, ns_zjet1HPtr, 
                                                ns_data1Integral,
                                                0., 0., -100., 100., 0.1);
      }
      if (ns_constrainZjet2)
      {
         ns_zjet2Index = ns_fitter.addTemplate (kZjet2Name, ns_zjet2HPtr, 
                                                ns_constrainZjet2,
                                                ns_constrainZjet2, 
                                                constraint2, -200., 200., 0.1);
      } else {
         ns_zjet2Index = ns_fitter.addTemplate (kZjet2Name, ns_zjet2HPtr, 
                                                ns_data2Integral,
                                                0., 0., -200., 200, 0.1);
      }
      // control region
      if (ns_whichTemplates <= -2)
      {
         if (ns_constrainZjet3)
         {
            ns_zjet3Index = ns_fitter.addTemplate (kZjet3Name, ns_zjet3HPtr, 
                                                   ns_constrainZjet3,
                                                   ns_constrainZjet3, 
                                                   constraint3, -300, 300, 0.1);
         } else {
            ns_zjet3Index = ns_fitter.addTemplate (kZjet3Name, ns_zjet3HPtr, 
                                                   ns_data3Integral,
                                                   0., 0., -300., 300., 0.1);
         }
      } // if we have the control region
      if (ns_tagBack)
      {
         ns_fitter.setParameter (kZjet1Name, ns_tagBack);
         ns_fitter.fixParameter (kZjet1Name);
      }
      if (ns_antiBack)
      {
         ns_fitter.setParameter (kZjet2Name, ns_antiBack);
         ns_fitter.fixParameter (kZjet2Name);
      }
   } // three Z + jets regions float independently.

   ////////////////////////
   // SM Top and Diboson //
   ////////////////////////
   if (ns_whichTemplates <= -3)
   {
      assert (ns_smttbarHPtr && ns_dibosonHPtr);
      ns_smttbarIndex = ns_fitter.addTemplate (kSmttbarName,
                                               ns_smttbarHPtr, 1, 
                                               0.9, 1.1, 0.01);
      ns_dibosonIndex = ns_fitter.addTemplate (kDibosonName,
                                               ns_dibosonHPtr, 1, 
                                               0.9, 1.1, 0.01);
      ns_fitter.fixParameter (kSmttbarName);
      ns_fitter.fixParameter (kDibosonName);
   }

   //////////
   // FCNC //
   //////////
   ns_fcncWZIndex = ns_fitter.addTemplate (kFcncWZName, 
                                           ns_fcncWZHPtr, 1,
                                           0., 0., 0.9, 1.2, 0.01); 
   ns_fcncZZIndex = ns_fitter.addTemplate (kFcncZZName, 
                                           ns_fcncZZHPtr, 1,
                                           0., 0., 0.9, 1.2, 0.01);
   ns_fitter.fixParameter (kFcncWZName);
   ns_fitter.fixParameter (kFcncZZName);

   //////////////
   // BinNorms //
   //////////////
   // Running acceptance
   BinNormClass::ISet allBinSet, fcncTempSet;
   fcncTempSet.insert (ns_fcncWZIndex);
   fcncTempSet.insert (ns_fcncZZIndex);
   BinNormClass::insertRangeIntoSet (allBinSet, 1, ns_totalBins);
   BinNormClass fcncNorm (fcncTempSet, allBinSet, 0.06, -1., 2., 0.01);
   RunAccept::DVec paramVec;
   paramVec.push_back ( 2.);        // 0
   paramVec.push_back (-2.);        // 1
   paramVec.push_back ( 1.);        // 2
   paramVec.push_back ( denommu1 ); // 3
   paramVec.push_back ( denommu2 ); // 4
   ns_wzAcc.setParamVec (paramVec);
   paramVec.at(0) = 0.;
   paramVec.at(1) = 1.;
   ns_zzAcc.setParamVec (paramVec);
   fcncNorm.setFunctoid (ns_fcncWZIndex, ns_wzAcc);
   fcncNorm.setFunctoid (ns_fcncZZIndex, ns_zzAcc);
   ns_fcncBFIndex = ns_fitter.addBinNorm (kFcncBFName, fcncNorm);

   // JES
   BinNormClass::ISet emptySet;
   BinNormClass jesNorm (0., -20., 20., 0.1);   
   if (ns_whichTemplates < 0 || ns_constrJes > 0)
   {
      ns_jesIndex = ns_fitter.addBinNorm (kJesName, jesNorm, 0, ns_constrJes);
      if (ns_ignoreJes)
      {
         ns_fitter.fixParameter (kJesName);
      }
   }

   if (ns_whichTemplates <= -5)
   {
      BinNormClass::ISet signalZjetTempSet, signalZjetBinSet;
      // Only for the first two templates and for the first two
      // regions (tag and anti-tag);
      signalZjetTempSet.insert (ns_zjet1Index);
      signalZjetTempSet.insert (ns_zjet2Index);
      BinNormClass::insertRangeIntoSet (signalZjetBinSet, 1, 
                                        ns_numBins1 + ns_numBins2);
      // Signal to control region normalization
      int nextIndex = ns_fitter.size(); // not very eloquent, but should
                                        // be safe.
      int jesIndex = ns_fitter.parameterIndex (kJesName);
      ns_signalZjetNormToid.setControlIndex  (ns_zjet3Index);
      ns_signalZjetNormToid.setJesIndex      (jesIndex);
      ns_signalZjetNormToid.setFractionIndex (nextIndex);
      if (ns_constraintPercent)
      {
         ns_signalZjetNormToid.setFractionError (ns_constraintPercent);
      }
      // tell the BinNorm which templates and bins it should worry about
      BinNormClass fractionNorm (signalZjetTempSet, signalZjetBinSet,
                                 0, -10., 10., 0.1);
      fractionNorm.setFunctoid (ns_zjet1Index, ns_signalZjetNormToid);
      fractionNorm.setFunctoid (ns_zjet2Index, ns_signalZjetNormToid);
      if (ns_constraintPercent)
      {
         // add with a unit Gaussian constraint
         ns_signalZjetNormIndex = 
            ns_fitter.addBinNorm (kZjetFractionName, fractionNorm, 0, 1);
      } else {
         // add with no constraint
         ns_signalZjetNormIndex = 
            ns_fitter.addBinNorm (kZjetFractionName, fractionNorm);
      }
      assert (nextIndex == ns_signalZjetNormIndex + ns_fitter.numTemplates());
      // tag and anti-tag rates
      nextIndex = ns_fitter.size(); // not very eloquent, but should
                                    // be safe.
      BinNormClass::ISet antitagBinsSet;
      BinNormClass::insertRangeIntoSet (antitagBinsSet, 
                                        ns_numBins1 + 1, 
                                        ns_numBins1 + ns_numBins2);
      ns_zjetTagFracToid.setNegBinsSet   (antitagBinsSet);
      ns_zjetTagFracToid.setTagFracIndex (nextIndex);
      BinNormClass tagNorm (signalZjetTempSet, signalZjetBinSet,
                            0, -1., 2., 0.01);
      tagNorm.setFunctoid (ns_zjet1Index, ns_zjetTagFracToid);
      tagNorm.setFunctoid (ns_zjet2Index, ns_zjetTagFracToid);
      ns_signalZjetTagIndex =
         ns_fitter.addBinNorm (kZjetTagFracName, tagNorm);
      assert (nextIndex == ns_signalZjetTagIndex + ns_fitter.numTemplates());
      
   } // -5 or less
   if (ns_verbose)
   {
      cout << "fitter" << endl << ns_fitter << endl;
   }
   
   ns_fitter._initializeMinuit();
   
}

void 
Chi2FitterNamespace::_makeEverythingPretty()
{
   //////////////////////
   // Setup the colors //
   //////////////////////
   // ns_dataHPtr
   ns_dataHPtr->SetMarkerStyle (20);
   ns_dataHPtr->SetMarkerSize  (1.5);
   ns_dataHPtr->SetMarkerColor (26);
   // backgrond
   // zjet->SetFillColor  (38);
   // zjet->SetLineColor  (kBlack);
   // zjet->SetLineWidth  (2);
   ns_zjet1HPtr->SetFillColor (kYellow);
   ns_zjet1HPtr->SetLineColor (kBlack);
   ns_zjet1HPtr->SetLineWidth (2);
   ns_zjet2HPtr->SetFillColor (kYellow);
   ns_zjet2HPtr->SetLineColor (kBlack);
   ns_zjet2HPtr->SetLineWidth (2);
   if (ns_zjet3HPtr)
   {
      ns_zjet3HPtr->SetFillColor (kYellow);
      ns_zjet3HPtr->SetLineColor (kBlack);
      ns_zjet3HPtr->SetLineWidth (2);
   }
   if (ns_smttbarHPtr && ns_dibosonHPtr)
   {
      ns_smttbarHPtr->SetFillColor (kBlue);
      ns_smttbarHPtr->SetLineColor (kBlack);
      ns_smttbarHPtr->SetLineWidth (2);
      ns_dibosonHPtr->SetFillColor (kGreen);
      ns_dibosonHPtr->SetLineColor (kBlack);
      ns_dibosonHPtr->SetLineWidth (2);
   }
   // FCNC acceptance
   ns_fcncWZHPtr->SetFillColor (kRed);
   ns_fcncWZHPtr->SetLineColor (kBlack);
   ns_fcncWZHPtr->SetLineWidth (2);
   ns_fcncZZHPtr->SetFillColor (kRed);
   ns_fcncZZHPtr->SetLineColor (kRed);
   ns_fcncZZHPtr->SetLineWidth (2);

   // ns_dataHPtr->SetMarkerStyle (20);
   // ns_dataHPtr->SetMarkerSize(1.5);
   // ns_dataHPtr->SetMarkerColor (kBlack);
   // TAxis *aPtr = ns_dataHPtr->GetXaxis();
   // aPtr->SetBinLabel ( 20 / ns_rebin1,  "2");
   // aPtr->SetBinLabel ( 40 / ns_rebin1,  "4");
   // aPtr->SetBinLabel ( 60 / ns_rebin1,  "6");
   // aPtr->SetBinLabel ( 80 / ns_rebin1,  "8");
   // // aPtr->SetBinLabel (100 / ns_rebin1, "10");
   // aPtr->SetBinLabel (numbins1 +  20 / ns_rebin2,  "2");
   // aPtr->SetBinLabel (numbins1 +  40 / ns_rebin2,  "4");
   // aPtr->SetBinLabel (numbins1 +  60 / ns_rebin2,  "6");
   // aPtr->SetBinLabel (numbins1 +  80 / ns_rebin2,  "8");
   // aPtr->SetBinLabel (numbins1 + 100 / ns_rebin2, "10");

}

void 
Chi2FitterNamespace::addToHistogram (TH1F *outputPtr, TH1F *inputPtr,  
                                     int offset, int rebin, float scale)
{
   int numBins = inputPtr->GetNbinsX();
   for (int bin = 1; bin <= numBins; ++bin)
   {
      int tobin = (bin - 1) / rebin + 1 + offset;
      //cout << setw(2) << bin << " to " << setw(2) << tobin << endl;
      outputPtr->Fill( (float) tobin, scale * inputPtr->GetBinContent(bin) );
   } // for bin   
}

void
Chi2FitterNamespace::setupOptions()
{
   using namespace OptionUtils;
   addOptionKey ("histnumber",   ns_histNumber);
   addOptionKey ("minverb",      ns_minuitVerbose);
   addOptionKey ("rebin1",       ns_rebin1);
   addOptionKey ("rebin2",       ns_rebin2);
   addOptionKey ("2fb",          ns_2fb);
   addOptionKey ("whichtemp",    ns_whichTemplates,
   "Which template: 0 - old, 1   - new, 2 - JES up, 3 - JES down");
   addOptionKey ("verbose",      ns_verbose);
   addOptionKey ("constrain",    ns_constrain);
   addOptionKey ("percent",      ns_constraintPercent);
   addOptionKey ("constz1",      ns_constrainZjet1);
   addOptionKey ("constz2",      ns_constrainZjet2);
   addOptionKey ("constz3",      ns_constrainZjet3);
   addOptionKey ("antiback",     ns_antiBack);
   addOptionKey ("tagback",      ns_tagBack);
   addOptionKey ("constrjes",    ns_constrJes);
   addOptionKey ("usenewdata",   ns_useNewData);
   addOptionKey ("ignorejes",    ns_ignoreJes);
   addOptionKey ("dominos",      ns_doMinos);
   addOptionKey ("accsyst",      ns_throwAccSyst);
   addOptionKey ("170",          ns_170GeV);
   addOptionKey ("as175",        ns_as175GeV);
   addOptionKey ("signalfactor", ns_signalFactor);  
   addOptionKey ("minexpbin",    ns_minExpectedBinContent);
}

