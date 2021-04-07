// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <algorithm>

// Root include files
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TText.h"
#include "TTree.h"
#include "TRandom3.h"

// CLP include files
#include "RootFileCollection/OptionUtils.h"
#include "CLPTemplateFitter/CLPTemplateFitter.hh"
#include "CLPTemplateFitter/CLPTrapezoid.hh"
#include "CLPTemplateFitter/dout.hh"

#include "Chi2FitterNamespace.hh"
#include "dumpSTL.icc"

using namespace std;
using namespace OptionUtils;
using namespace Chi2FitterNamespace;

// forward declarations
void localInitialize();
void rescale (const CLPTrapezoid::Vec &input,
              CLPTrapezoid::Vec &output,
              double scale = 1.);
void setAxisLabels (TH1F *histPtr);
void corporate_id();

// Global variables (ugh!) for command line options

// Number of PEs (numpes)
int    gNumPEs = 0;
// seed (seed)
int    gSeed = 0; 
// true bf (truebf)
double gTrueBF = 0.;
// force true JES (forceJES)
double gForceTrueJes = 0.;
// makeplots (makeplots)
bool   gMakePlots = false;
// output filename (output)
string gOutputFilename = "rootfiles/PEs";
// fit output filename (fitoutput)
string gFitOutputName  = "";
// do data fit (dodata)
bool   gDoData = false;
// throw from Alpgen KQFac = 2 (throwalp=1) or KQFac=0.5 (throwalp=-1)
int    gThrowAlpgen = 0;
// run only ideal PE
bool   gIdealPE = false;

int main (int argc, char** argv)
{
   Chi2FitterNamespace::setupOptions();
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);
   delete gRandom;
   gRandom = new TRandom3;
   gRandom->SetSeed (gSeed);
   removeEnding (gOutputFilename, ".root");
   gOutputFilename += Form ("_s%03d", gSeed);
   gOutputFilename += Form ("_bf%05.2f%%", 100. * gTrueBF);
   if (ns_ignoreJes)      gOutputFilename += "_noJes";
   if (ns_doMinos)        gOutputFilename += "_minos";
   if (! ns_throwAccSyst) gOutputFilename += "_noaccsyst";
   if (gThrowAlpgen > 0)  gOutputFilename += "_throwalp20";
   if (gThrowAlpgen < 0)  gOutputFilename += "_throwalp05";
   if (ns_170GeV)
   {
      gOutputFilename += "_170GeV";
      if (ns_as175GeV) 
      {
         gOutputFilename += "_as175GeV";
      }
   }
   if (ns_constraintPercent)
   {
      gOutputFilename += Form ("_%0.1fconstr", 100 * ns_constraintPercent);
   }
   if (ns_signalFactor)
   {
      gOutputFilename += Form ("_%0.3fsigfact", ns_signalFactor);
   }
   gOutputFilename += Form ("_%d", gNumPEs);
   gOutputFilename += ".root";
   // gROOT->SetStyle("Plain");   
   corporate_id();
   gStyle->SetOptStat(0000);

   if (gIdealPE)
   {
      ns_verbose = true;
   }


   ////////////////
   // Setup Tree //
   ////////////////
   TFile *outputFilePtr = 0;
   TTree *treePtr = 0;
   float measBF   = 0.;
   float errBF    = 0.;
   float scanBF   = 0.;
   float measJES  = 0.;
   float trueJES  = 0.;
   if (gNumPEs > 0)
   {
      outputFilePtr = TFile::Open (gOutputFilename.c_str(), "RECREATE");
      assert (outputFilePtr);
      treePtr = new TTree ("PEtree", "PEtree");
      treePtr->Branch ("measBF",  &measBF,  "measBF/F" );
      treePtr->Branch ("errBF",   &errBF,   "errBF/F"  );
      treePtr->Branch ("scanBF",  &scanBF,  "scanBF/F" );
      treePtr->Branch ("measJES", &measJES, "measJES/F");
      treePtr->Branch ("trueJES", &trueJES, "trueJES/F");
   }

   Chi2FitterNamespace::initializeFitter();

   if (0)
   {
      for (double bf = 0.30; bf >= -0.30; bf -= 0.01)
      {
         cout << bf << " " << ns_wzAcc (bf)  << " " << ns_zzAcc (bf)
              << " : " <<  ns_wzAcc (bf) * ns_fcncWZHPtr->Integral() +
            ns_zzAcc (bf) * ns_fcncZZHPtr->Integral() << endl;
      }
   } // if dumping out positive and negative acceptances

   TCanvas *c1 = new TCanvas;
   ns_fitter.fit();
   if (gDoData)
   {
      ns_fitter.outputFitResults();
      if (ns_whichTemplates <= -3 && gMakePlots)
      {
         // Get all of the templates.
         TH1F *zjet1HPtr  = ns_fitter.updatedHistogram (kZjet1Name);
         TH1F *zjet2HPtr  = ns_fitter.updatedHistogram (kZjet2Name);
         TH1F *zjet3HPtr  = ns_fitter.updatedHistogram (kZjet3Name);
         TH1F *ttbarHPtr  = ns_fitter.updatedHistogram (kSmttbarName);
         TH1F *dibosHPtr  = ns_fitter.updatedHistogram (kDibosonName);
         TH1F *fcncWZHPtr = ns_fitter.updatedHistogram (kFcncWZName);
         TH1F *fcncZZHPtr = ns_fitter.updatedHistogram (kFcncZZName);
         // Start stacking
         // Z + jets
         zjet3HPtr->Add (zjet1HPtr);
         zjet3HPtr->Add (zjet2HPtr);
         // signal
         zjet3HPtr->Add (fcncWZHPtr);
         zjet3HPtr->Add (fcncZZHPtr);
         // diboson
         ttbarHPtr->Add (dibosHPtr);
         // ttbar
         zjet3HPtr->Add (ttbarHPtr);
         double maximum = 1.3 * std::max (zjet3HPtr->GetMaximum(),
                                          ns_dataHPtr->GetMaximum());
         zjet3HPtr->SetMaximum (maximum);
         zjet3HPtr->SetTitle ("Best Fit to Data");
         setAxisLabels (zjet3HPtr);
         zjet3HPtr->Draw();
         ttbarHPtr->Draw("same");
         dibosHPtr->Draw("same");
         ns_dataHPtr->Draw("hist p same");
         TLine *line1 = new TLine (ns_numBins1 + 0.5, 0, 
                                   ns_numBins1 + 0.5, maximum);
         TLine *line2 = new TLine (ns_numBins1 + ns_numBins2 + 0.5, 0, 
                                   ns_numBins1 + ns_numBins2 + 0.5, maximum);
         line1->Draw("same");
         line2->Draw("same");
         c1->Print ("bestfit.eps");
         if (gFitOutputName.length())
         {
            removeEnding (gFitOutputName, ".root");            
            gFitOutputName += ".root";
            TFile *fitOutputFilePtr =
               TFile::Open (gFitOutputName.c_str(), "recreate");
            zjet1HPtr->Write();
            zjet2HPtr->Write();
            zjet3HPtr->Write();
            ttbarHPtr->Write();
            dibosHPtr->Write();
            ns_dataHPtr->Write();
            delete fitOutputFilePtr;
         } // if fit output
      }
   } // if do data
   if (1)
   {
      TH1F *zjet1HPtr  = ns_fitter.updatedHistogram (kZjet1Name);
      TH1F *zjet2HPtr  = ns_fitter.updatedHistogram (kZjet2Name);
      TH1F *zjet3HPtr  = 0;
      if (ns_whichTemplates <= -2)
      {
         zjet3HPtr  = ns_fitter.updatedHistogram (kZjet3Name);
         zjet3HPtr->SetMaximum( 1.3 * zjet3HPtr->GetMaximum());
         zjet3HPtr->SetTitle ("Best Fit");
         zjet3HPtr->Draw();
         zjet2HPtr->Draw("Same");
      } else {
         zjet2HPtr->SetMaximum( 1.5 * zjet2HPtr->GetMaximum());
         zjet2HPtr->SetTitle ("Best Fit");
         zjet2HPtr->Draw();
      }
      zjet1HPtr->Draw("Same");
      ns_dataHPtr->Draw("hist p same");
   } else {
      dout << endl;
   }

   

   CLPTemplateFitter::SIMap nameIndexMap = ns_fitter.getNameIndexMap();
   int fcncWZParamIndex = 
      CLPTemplateFitter::indexOfName (kFcncWZName,  nameIndexMap);
   int fcncZZParamIndex =                           
      CLPTemplateFitter::indexOfName (kFcncZZName,  nameIndexMap);
   int zjet1ParamIndex  =                           
      CLPTemplateFitter::indexOfName (kZjet1Name,   nameIndexMap);
   int zjet2ParamIndex  =                           
      CLPTemplateFitter::indexOfName (kZjet2Name,   nameIndexMap);
   int zjet3ParamIndex  =                           
      CLPTemplateFitter::indexOfName (kZjet3Name,   nameIndexMap);
   int smttbarParamIndex =                          
      CLPTemplateFitter::indexOfName (kSmttbarName, nameIndexMap);
   int dibosonParamIndex =                          
      CLPTemplateFitter::indexOfName (kDibosonName, nameIndexMap);
   int fcncBFParamIndex =                           
      CLPTemplateFitter::indexOfName (kFcncBFName,  nameIndexMap);
   int jesParamIndex    =                           
      CLPTemplateFitter::indexOfName (kJesName,     nameIndexMap);

   CLPTemplateFitter::DVec paramVec (ns_fitter.size(), 0);
   paramVec.at( fcncWZParamIndex  ) =  1.;
   paramVec.at( fcncZZParamIndex  ) =  1.;
   paramVec.at( fcncBFParamIndex  ) =  0.;
   paramVec.at( zjet1ParamIndex   ) =  0.;
   paramVec.at( zjet2ParamIndex   ) =  0.; 
   paramVec.at( zjet3ParamIndex   ) =  0.;
   paramVec.at( smttbarParamIndex ) =  1.;      
   paramVec.at( dibosonParamIndex ) =  1.;      
   paramVec.at( jesParamIndex     ) =  0.;      

   ////////////////////////////
   // Bayesian Limit on Data //
   ////////////////////////////   
   if (0)
   {
      ns_fitter.fitEverythingBut ("fcncBF", -0.01);
      CLPTrapezoid::Vec logLikelihoodCurve;
      ns_fitter.scanVariable (logLikelihoodCurve, "fcncBF", 0, 0.30, 120);   
      CLPTrapezoid::Vec likelihoodCurve;
      rescale (logLikelihoodCurve, likelihoodCurve);
      double upper, lower;
      CLPTrapezoid::limits (likelihoodCurve, upper, lower, 0.95);
      cout << "95% [" << lower << ", " << upper << "]" << endl;
   }

   if (ns_whichTemplates > -2)
   {
      return 0;
   }

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
   assert (zjet1Ptr && zjet2Ptr && zjet3Ptr);
   CLPTemplateMorph::PtrVec morphPtrVec;
   morphPtrVec.push_back (zjet1Ptr);
   morphPtrVec.push_back (zjet2Ptr);
   morphPtrVec.push_back (zjet3Ptr);
   morphPtrVec.push_back (fcncWZPtr);
   morphPtrVec.push_back (fcncZZPtr);
   if (smttbarPtr) morphPtrVec.push_back (smttbarPtr);
   if (dibosonPtr) morphPtrVec.push_back (dibosonPtr);
   CLPTemplateMorph::DVec morphParamVec (1, 0.);
   zjet1Ptr->setParamVec (morphParamVec);
   zjet2Ptr->setParamVec (morphParamVec);
   zjet3Ptr->setParamVec (morphParamVec);

   double num1 = 13;
   double num2 = 53;
   double num3 = 136;
   double numZ1 = num1 
      - ns_smttbarHPtr->Integral (1, ns_numBins1)
      - ns_dibosonHPtr->Integral (1, ns_numBins1);
   double numZ2 = num2
      - ns_smttbarHPtr->Integral (ns_numBins1 + 1, 
                                  ns_numBins1 + ns_numBins2)
      - ns_dibosonHPtr->Integral (ns_numBins1 + 1, 
                                  ns_numBins1 + ns_numBins2);
   double numZ3 = num3
      - ns_smttbarHPtr->Integral (ns_numBins1 + ns_numBins2 + 1,
                                  ns_numBins1 + ns_numBins2 + 
                                  ns_numBins3)
      - ns_dibosonHPtr->Integral (ns_numBins1 + ns_numBins2 + 1,
                                  ns_numBins1 + ns_numBins2 + 
                                  ns_numBins3);

   if (0)
   {
      dout << numZ1 << " " << numZ2 << " " << numZ3 << endl;
   }

   ///////////////
   // Throw PEs //
   ///////////////
   TH1F *peHPtr = (TH1F*) ns_dataHPtr->Clone("PEdata");
   trueJES = 0.;  
   for (int loop = 0; loop < gNumPEs; ++loop)
   {
      if (ns_verbose) 
      {
         cout << endl << endl << "PE " << loop << endl;
      } else if (loop && 0 == loop % 10) {
         cout << loop << endl;
      }
      peHPtr->Reset();
      if (! ns_ignoreJes)
      {
         // We're not ignoring, so throw it
         trueJES = gRandom->Gaus (0, 1);
         if (gThrowAlpgen > 0)
         {
            trueJES = 0.515083;
         } else if (gThrowAlpgen < 0)
         {
            trueJES = -0.10;
         }
         if (gForceTrueJes)
         {
            trueJES = gForceTrueJes;
         }
         if (gIdealPE)
         {
            cout << "true JES " << trueJES << endl;
         }
      }      
      // tell all template morphs about the true JES value
      morphParamVec.at(0) = trueJES;
      CLPTemplateMorph::setParamVecs (morphPtrVec, morphParamVec);

      // how many diboson and ttbar
      int numDiboson = 0;
      int numSMttbar = 0;
      if (dibosonPtr)
      {
         numDiboson = 
            gRandom->Poisson (gRandom->Gaus(1, 0.06) * dibosonPtr->Integral());
         for (int innerLoop = 0; innerLoop < numDiboson; ++innerLoop)
         {
            peHPtr->Fill( dibosonPtr->GetRandom() );
         } // for innerLoop
      }
      if (smttbarPtr)
      {
         numSMttbar = 
            gRandom->Poisson (gRandom->Gaus(1, 0.06) * smttbarPtr->Integral());
         for (int innerLoop = 0; innerLoop < numSMttbar; ++innerLoop)
         {
            peHPtr->Fill( smttbarPtr->GetRandom() );
         } // for innerLoop
      }
      // Decide how many background events in each sample
      // (tagged, anti-tagged, control)
      double tagFraction = gRandom->Gaus (0.15, 0.02);
      double unitGaus    = gRandom->Gaus (0, 1);      
      double signalZjets = ns_signalZjetNormToid (numZ3, unitGaus, trueJES);
      if (tagFraction < 0.02 || tagFraction > 0.98 || signalZjets < 30)
      {
         // Rethrow
         cout << "Rethrow" << endl;
         --loop;
         continue;
      }
      int num1wanted = gRandom->Poisson (tagFraction * signalZjets);
      int num2wanted = gRandom->Poisson ((1 - tagFraction) * signalZjets);
      int num3wanted = gRandom->Poisson (numZ3);
      // Z + jets
      if (0 == gThrowAlpgen)
      {
         for (int innerLoop = 0; innerLoop < num1wanted; ++innerLoop)
            peHPtr->Fill( zjet1Ptr->GetRandom() );
         for (int innerLoop = 0; innerLoop < num2wanted; ++innerLoop)
            peHPtr->Fill( zjet2Ptr->GetRandom() );
         for (int innerLoop = 0; innerLoop < num3wanted; ++innerLoop)
            peHPtr->Fill( zjet3Ptr->GetRandom() );
      } else {
         if (gThrowAlpgen > 0)
         {
            // 2.0
            for (int innerLoop = 0; innerLoop < num1wanted; ++innerLoop)
               peHPtr->Fill( ns_alp20Zjet1HPtr->GetRandom() );
            for (int innerLoop = 0; innerLoop < num2wanted; ++innerLoop)
               peHPtr->Fill( ns_alp20Zjet2HPtr->GetRandom() );
            for (int innerLoop = 0; innerLoop < num3wanted; ++innerLoop)
               peHPtr->Fill( ns_alp20Zjet3HPtr->GetRandom() );
         } else {
            // 0.5
            for (int innerLoop = 0; innerLoop < num1wanted; ++innerLoop)
               peHPtr->Fill( ns_alp05Zjet1HPtr->GetRandom() );
            for (int innerLoop = 0; innerLoop < num2wanted; ++innerLoop)
               peHPtr->Fill( ns_alp05Zjet2HPtr->GetRandom() );
            for (int innerLoop = 0; innerLoop < num3wanted; ++innerLoop)
               peHPtr->Fill( ns_alp05Zjet3HPtr->GetRandom() );
         }
      }
      // FCNC
      if (gTrueBF > 0)
      {
         // First get the total expected signal with no systematic
         // uncertainties.
         TH1F *signalHPtr = new TH1F ("signal", "signal", ns_totalBins, 
                                  0.5, 0.5 + ns_totalBins);
         signalHPtr->Add (fcncWZPtr,  ns_wzAcc (gTrueBF));
         signalHPtr->Add (fcncZZPtr,  ns_zzAcc (gTrueBF));
         // Second, split them up into three pieces
         TH1F *tagSignalHPtr  = (TH1F*) signalHPtr->Clone ("tagSignal");
         TH1F *antiSignalHPtr = (TH1F*) signalHPtr->Clone ("antiSignal");
         TH1F *contSignalHPtr = (TH1F*) signalHPtr->Clone ("contSignal");
         // and zero out the bins that we don't want for each
         for (int bin = 1; bin <= ns_numBins1; ++bin)
         {
            // Keep tagSignal 
            antiSignalHPtr->SetBinContent (bin, 0.);
            contSignalHPtr->SetBinContent (bin, 0.);
         } // for bin
         for (int bin = ns_numBins1 + 1; 
              bin <= ns_numBins1 + ns_numBins2; ++bin)
         {
            // Keep antiSignal 
            tagSignalHPtr->SetBinContent (bin, 0.);
            contSignalHPtr->SetBinContent (bin, 0.);
         } // for bin
         for (int bin = ns_numBins1 + ns_numBins2 + 1; 
              bin <= ns_numBins1 + ns_numBins2 + ns_numBins3; ++bin)
         {
            // Keep contSignal 
            tagSignalHPtr->SetBinContent (bin, 0.);
            antiSignalHPtr->SetBinContent (bin, 0.);
         } // for bin
         // Good.  Now start throwing
         double tagSmear  = 1.;
         double antiSmear = 1.;
         double contSmear = 1.;
         if (ns_throwAccSyst)
         {
            const double kTagCorr  = 0.062;
            const double kAntiCorr = 0.061;
            const double kContCorr = 0.059;
            const double kTagAntiCorr  =  0.072;
            const double kAntiAntiCorr = -0.167;
            const double kContAntiCorr = -0.102;
            double corrShift     = gRandom->Gaus (0, 1);
            double anticorrShift = gRandom->Gaus (0, 1);
            tagSmear  = (1 + corrShift * kTagCorr) 
               * (1 + anticorrShift * kTagAntiCorr);
            antiSmear = (1 + corrShift * kAntiCorr) 
               * (1 + anticorrShift * kAntiAntiCorr);
            contSmear = (1 + corrShift * kContCorr) 
               * (1 + anticorrShift * kContAntiCorr);
         }
         // if systematics, then non-unity smears
         int numTag  = gRandom->Poisson ( tagSmear  * 
                                          tagSignalHPtr->Integral() );
         int numAnti = gRandom->Poisson ( antiSmear * 
                                          antiSignalHPtr->Integral() );
         int numCont = gRandom->Poisson ( contSmear * 
                                          contSignalHPtr->Integral() );
         // tag
         for (int innerLoop = 0; innerLoop < numTag; ++innerLoop)
         {
            peHPtr->Fill( tagSignalHPtr->GetRandom() );            
         } // for innerLoop
         // anti
         for (int innerLoop = 0; innerLoop < numAnti; ++innerLoop)
         {
            peHPtr->Fill( antiSignalHPtr->GetRandom() );            
         } // for innerLoop
         // cont
         for (int innerLoop = 0; innerLoop < numCont; ++innerLoop)
         {
            peHPtr->Fill( contSignalHPtr->GetRandom() );            
         } // for innerLoop
         delete signalHPtr;
         delete tagSignalHPtr;
         delete antiSignalHPtr;
         delete contSignalHPtr;
         // double expectedWZ = ns_wzAcc (gTrueBF) * fcncWZPtr->Integral();
         // double expectedZZ = ns_zzAcc (gTrueBF) * fcncZZPtr->Integral();
         // int numWZ = gRandom->Poisson( expectedWZ );
         // int numZZ = gRandom->Poisson( expectedZZ );
         // for (int innerLoop = 0; innerLoop < numWZ; ++innerLoop)
         // {
         //    peHPtr->Fill( fcncWZPtr->GetRandom() );
         // } // for innerLoop         
         // for (int innerLoop = 0; innerLoop < numZZ; ++innerLoop)
         // {
         //    peHPtr->Fill( fcncZZPtr->GetRandom() );
         // } // for innerLoop     
      }
      // Is this supposed to be an ideal PE?
      if (gIdealPE)
      {
         peHPtr->Reset();
         // add Z + jets
         if (0 == gThrowAlpgen)
         {
            peHPtr->Add ( zjet1Ptr, (double) num1 );
            peHPtr->Add ( zjet2Ptr, (double) num2 );
            peHPtr->Add ( zjet3Ptr, (double) num3 );
         } else {
            if (gThrowAlpgen > 0)
            {
               // 20
               peHPtr->Add ( ns_alp20Zjet1HPtr, (double) num1 );
               peHPtr->Add ( ns_alp20Zjet2HPtr, (double) num2 );
               peHPtr->Add ( ns_alp20Zjet3HPtr, (double) num3 );
            } else {
               // 05
               peHPtr->Add ( ns_alp05Zjet1HPtr, (double) num1 );
               peHPtr->Add ( ns_alp05Zjet2HPtr, (double) num2 );
               peHPtr->Add ( ns_alp05Zjet3HPtr, (double) num3 );
            }
         }
         // add diboson and SM ttbar
         if (dibosonPtr && smttbarPtr)
         {
            peHPtr->Add (dibosonPtr);
            peHPtr->Add (smttbarPtr);
         }
         // add signal
         if (gTrueBF > 0)
         {
            peHPtr->Add (fcncWZPtr, ns_wzAcc (gTrueBF));
            peHPtr->Add (fcncZZPtr, ns_zzAcc (gTrueBF));
         }
      }
      // reset JES before fitting
      morphParamVec.at(0) = 0;
      CLPTemplateMorph::setParamVecs (morphPtrVec, morphParamVec);      
      ns_fitter.addData (peHPtr);
      ns_fitter.resetMinuitParameters();
      fitBF (0., true);
      if (0)
      {
         ns_fitter.outputFitResults();
         dout << ns_fitter.logLikelihoodValue() << endl;
         Chi2FitterNamespace::scanBF();
      }
      if (ns_verbose)
      {
         ns_fitter.outputFitResults();
      }
      CLPTemplateFitter::DVec paramVec;
      ns_fitter.fillParamVec (paramVec);
      measBF  = paramVec.at (fcncBFParamIndex);
      measJES = paramVec.at (jesParamIndex);
      treePtr->Fill();
      if (! gMakePlots)
      {
         continue;
      }
      TH1F *zjet1HPtr  = ns_fitter.updatedHistogram (kZjet1Name);
      TH1F *zjet2HPtr  = ns_fitter.updatedHistogram (kZjet2Name);
      TH1F *zjet3HPtr  = ns_fitter.updatedHistogram (kZjet3Name);
      zjet3HPtr->SetMaximum( 1.1 * std::max (zjet3HPtr->GetMaximum(),
                                             peHPtr->GetMaximum()));
      CLPTemplateFitter::DVec fittedParamVec;
      ns_fitter.fillParamVec (fittedParamVec);
      fittedParamVec.at( jesParamIndex    ) =  0.;      
      TH1F *zjet1_0HPtr  = ns_fitter.updatedHistogram (kZjet1Name, 
                                                       fittedParamVec);
      TH1F *zjet2_0HPtr  = ns_fitter.updatedHistogram (kZjet2Name, 
                                                       fittedParamVec);
      TH1F *zjet3_0HPtr  = ns_fitter.updatedHistogram (kZjet3Name, 
                                                       fittedParamVec);
         
      zjet3_0HPtr->SetMaximum( 1.1 * std::max (zjet3_0HPtr->GetMaximum(),
                                               peHPtr->GetMaximum()));
      zjet1_0HPtr->SetLineColor(kBlue);
      zjet2_0HPtr->SetLineColor(kBlue);
      zjet3_0HPtr->SetLineColor(kBlue);
      zjet1_0HPtr->SetFillStyle(0);
      zjet2_0HPtr->SetFillStyle(0);
      zjet3_0HPtr->SetFillStyle(0);
      setAxisLabels (zjet3_0HPtr);
      zjet3HPtr->Draw();
      zjet1HPtr->Draw("same");
      zjet2HPtr->Draw("same");
      zjet3_0HPtr->Draw("same");
      zjet1_0HPtr->Draw("same");
      zjet2_0HPtr->Draw("same");
      peHPtr->Draw("hist p same");
      c1->Print( Form ("plots/pe%d.eps", loop));
   } // for loop
   if (outputFilePtr)
   {
      outputFilePtr->cd();
      treePtr->Write();
      delete outputFilePtr;
   }
   return 0;
}

void rescale (const CLPTrapezoid::Vec &input,
              CLPTrapezoid::Vec &output,
              double scale)
{
   output.clear();
   for (CLPTrapezoid::VecConstIter iter = input.begin();
        input.end() != iter;
        ++iter)
   {
      double xval = iter->lowerX() * scale;
      double yval = exp( iter->lowerY() * -0.5 );
      output.push_back( CLPTrapezoid ( xval, yval ) );
   }
}

void setAxisLabels (TH1F *histPtr)
{
   TAxis *aPtr = histPtr->GetXaxis();
   aPtr->SetBinLabel ( 20 / ns_rebin1,  "2");
   aPtr->SetBinLabel ( 40 / ns_rebin1,  "4");
   aPtr->SetBinLabel ( 60 / ns_rebin1,  "6");
   aPtr->SetBinLabel ( 80 / ns_rebin1,  "8");
   // aPtr->SetBinLabel (100 / ns_rebin1, "10");
   aPtr->SetBinLabel (ns_numBins1 +  20 / ns_rebin2,  "2");
   aPtr->SetBinLabel (ns_numBins1 +  40 / ns_rebin2,  "4");
   aPtr->SetBinLabel (ns_numBins1 +  60 / ns_rebin2,  "6");
   aPtr->SetBinLabel (ns_numBins1 +  80 / ns_rebin2,  "8");
   aPtr->SetBinLabel (ns_numBins1 + 100 / ns_rebin2, "10");
   aPtr->SetBinLabel (ns_numBins1 + ns_numBins2 +  20 / ns_rebin3,  "2");
   aPtr->SetBinLabel (ns_numBins1 + ns_numBins2 +  40 / ns_rebin3,  "4");
   aPtr->SetBinLabel (ns_numBins1 + ns_numBins2 +  60 / ns_rebin3,  "6");
   aPtr->SetBinLabel (ns_numBins1 + ns_numBins2 +  80 / ns_rebin3,  "8");
   aPtr->SetBinLabel (ns_numBins1 + ns_numBins2 + 100 / ns_rebin3, "10");
 }

void localInitialize()
{
   // option name is case insensitive.  Capitalization of the strings
   // is for readability only.
   addOptionKey ("seed",       gSeed);
   addOptionKey ("numpes",     gNumPEs,
                 "Number of PEs");
   addOptionKey ("makeplots",  gMakePlots,
                 "Whether to make plots or not");
   addOptionKey ("truebf",     gTrueBF);
   addOptionKey ("output",     gOutputFilename);
   addOptionKey ("fitoutput",  gFitOutputName,
                 "root file name for output templates");
   addOptionKey ("dodata",     gDoData,
                 "fit data distribution or not");
   addOptionKey ("throwAlp",   gThrowAlpgen,
                 "Throw from KQFac=2 Alpgen Z + Jets Template");
   addOptionKey ("idealpe",    gIdealPE,
                 "run only ideal PE");
   addOptionKey ("forcejes",   gForceTrueJes);
}
