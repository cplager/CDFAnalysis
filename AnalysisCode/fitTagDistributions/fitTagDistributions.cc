// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstdlib>

// Root include files
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLine.h"

// CLP include files
#include "RootFileCollection/OptionUtils.h"
#include "CLPTemplateFitter/CLPTemplateFitter.hh"
#include "CLPBlue/CLPBlue.hh"
#include "CLPBlue/CLPUtils.hh"
#include "CLPBlue/dout.hh"

using namespace std;
using namespace OptionUtils;
using namespace CLPUtilNamespace;

typedef vector< int > IVec;

// Global variables (ugh!) for command line options

// Switch to tell us which sets of files to load (which)
int gWhich = 0;
// Minuit verbose level (mvl)
int gMinuitVerboseLevel = -1;
// number of trials for PEs (numtrials)
int gNumTrials = 10000;
// turn on debug statements (debug)
bool gDebugFlag = false;
// output name (output)
string gOutputName = "jettag";
// use Ulrich's templates (ulrich)
bool gUlrich = true;
// use the oldstyle templates (oldstyle)
bool gOldStyle = false;
// let 1st jet bin float in all fits
bool gFloatJet1 = false;
// keep the fourth jet bin (keep4th)
bool gKeep4thJet = false;
// Exclude a jet bin (nojet1, nojet2, nojet3);
bool gNoJet1 = false;
bool gNoJet2 = false;
bool gNoJet3 = false;
// Exclude a tagger (noLoose, noTight, noUltra)
bool gNoLoose = false;
bool gNoTight = false;
bool gNoUltra = false;
// By default, all three flavors will float independently.  To fix
// bottom and charm together, use the heavy flavor mode (HF).  To fix
// all flavors, use the no heavy flavor mode (noHF).  If both are set,
// HF will be used.
bool gUseHF = false;
bool gNoHF  = false;
// names of the template files (dataName, bottomName, charmName, lightName)
string gDataName   = "rootfiles/data_1.2_4jets_noveto.root";
string gBottomName = "rootfiles/tags_bottom_4jets_noveto.root";
string gCharmName  = "rootfiles/tags_charm_4jets_noveto.root"; 
string gLightName  = "rootfiles/tags_light_4jets_noveto.root"; 
string gUlrichName = "rootfiles/harvard.root";
// which results should be printed
bool gShowResults = true;
bool gShowCovar   = false;
bool gShowCorr    = true;
// which plots to make
bool gBeforePlot   = false;
bool gAfterPlot    = true;
bool gRatioPlot    = true;
bool gResidualPlot = true;

// forward declarations
void localInitialize();
void finishInitialization();
void printHist (TH1F *hPtr);
bool drawLines (int bins, double max, bool useLogScale = true, 
                double min = 0.);
bool setupBinEdges (int   bins,
                    IVec &lowerVec,
                    IVec &upperVec,
                    int  &numJets,
                    int  &numTaggers,
                    int  &taggerOffset);
void debugISet (const string &tag, const BinNormClass::ISet &iset);
void calcMeanRMS (double &mean, double &rms, double sum, double sum2,
                  int trials);

int main (int argc, char** argv)
{
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);
   finishInitialization();
   if (gOldStyle) gUlrichName = "rootfiles/oldstyle.root";

   gROOT->SetStyle("Plain");   

   ////////////////////////
   // Release the Hounds //
   ////////////////////////
   CLPTemplateFitter fitter;
   fitter.setMinuitVerboseLevel (gMinuitVerboseLevel);

   /////////////////////
   // Load Histograms //
   /////////////////////
   // data
   TFile *dataFile    = 0;
   TH1F  *data        = 0;
   // light
   TFile *lightFile   = 0;
   TH1F  *light       = 0;
   int    lightIndex  = 0; 
   // charm
   TFile *charmFile   = 0;
   TH1F  *charm       = 0;
   int    charmIndex  = 0;
   // bottom
   TFile *bottomFile  = 0;
   TH1F  *bottom      = 0;
   int    bottomIndex = 0;
   // top
   TH1F  *smttbar     = 0;
   int    ttbarIndex  = 0;
   // Ulrich's file pointer
   TFile *ulrichFile  = 0;

   if (gUlrich)
   {
      ulrichFile  = TFile::Open (gUlrichName.c_str());
      // data
      data        = (TH1F*) ulrichFile->Get ("htemp_data");
      fitter.addData (data);
      // light
      light       = (TH1F*) ulrichFile->Get ("htemp_zjets");
      lightIndex  = fitter.addTemplate ("light", light, 1.6);
      // charm
      charm       = (TH1F*) ulrichFile->Get ("htemp_zcc");
      charmIndex  = fitter.addTemplate ("charm", charm, 1.6);
      // bottom
      bottom      = (TH1F*) ulrichFile->Get ("htemp_zbb");
      bottomIndex = fitter.addTemplate ("bottom", bottom, 1.6);
      // smttbar
      smttbar     = (TH1F*) ulrichFile->Get ("htemp_smtt");
      ttbarIndex  = fitter.addTemplate ("smttbar", smttbar, 1.);      
      fitter.fixParameter ("smttbar");
   } else {
      // data
      dataFile    = TFile::Open (gDataName.c_str());
      data        = (TH1F*) dataFile->Get ("tags_data");
      fitter.addData (data);
      // light           
      lightFile   = TFile::Open (gLightName.c_str());
      light       = (TH1F*) lightFile->Get ("tags_light");
      lightIndex  = fitter.addTemplate ("light", light, 13.);   
      // charm           
      charmFile   = TFile::Open (gCharmName.c_str());
      charm       = (TH1F*) charmFile->Get ("tags_charm");
      charmIndex  = fitter.addTemplate ("charm", charm, 13.);
      // bottom
      bottomFile  = TFile::Open (gBottomName.c_str());
      bottom      = (TH1F*) bottomFile->Get ("tags_bottom");
      bottomIndex = fitter.addTemplate ("bottom", bottom, 13.);
      // smttbar
      ulrichFile  = TFile::Open (gUlrichName.c_str());
      smttbar     = (TH1F*) ulrichFile->Get ("htemp_smtt");
      ttbarIndex  = fitter.addTemplate ("smttbar", smttbar, 1.);      
      fitter.fixParameter ("smttbar");
   }
   // Use the histograms
   data->SetMarkerStyle (20);
   light->SetFillColor (kRed);
   charm->SetFillColor (kGreen);
   bottom->SetFillColor (kBlue);
   smttbar->SetFillColor (kYellow);

   // Figure out which taggers we're using, etc.
   float dataMax = data->GetMaximum();
   int   bins    = data->GetNbinsX();
   int   numJets, numTaggers, taggerOffset;
   IVec  lowerVec, upperVec;
   if (! setupBinEdges (bins, lowerVec, upperVec, 
                        numJets, numTaggers, taggerOffset) )
   {
      return 1;
   } // if problem setting up bin edges
   int taggersUsed = numTaggers;

   ////////////////////
   // Template ISets //
   ////////////////////
   BinNormClass::ISet hfTempSet, zjetTempSet;
   // HF template sets
   hfTempSet.insert (charmIndex );
   hfTempSet.insert (bottomIndex);
   // All template sets
   zjetTempSet.insert (lightIndex );
   zjetTempSet.insert (charmIndex );
   zjetTempSet.insert (bottomIndex);

   ///////////////
   // Bin ISets //
   ///////////////

   // all bin set
   BinNormClass::ISet allBinSet;
   BinNormClass::insertRangeIntoSet (allBinSet, 1, bins);

   // tagger bin sets
   BinNormClass::ISet looseBinSet, tightBinSet, ultraBinSet;
   BinNormClass::insertRangeIntoSet (looseBinSet,
                                     (1 - 1) * taggerOffset + 1,
                                     (1    ) * taggerOffset);
   BinNormClass::insertRangeIntoSet (tightBinSet,
                                     (2 - 1) * taggerOffset + 1,
                                     (2    ) * taggerOffset);
   BinNormClass::insertRangeIntoSet (ultraBinSet,
                                     (3 - 1) * taggerOffset + 1,
                                     (3    ) * taggerOffset);

   // Jet bin sets
   BinNormClass::ISet bin1Set, bin2Set, bin3Set, bin4Set;
   for (int tagger = 0; tagger < numTaggers; ++tagger)
   {
      int offset = tagger * taggerOffset;
      // 1 jet bin sets
      BinNormClass::insertRangeIntoSet (bin1Set,  
                                        offset + lowerVec[ 1 - 1 ],
                                        offset + upperVec[ 1 - 1 ]);
      // 2 jet bin sets
      BinNormClass::insertRangeIntoSet (bin2Set,                 
                                        offset + lowerVec[ 2 - 1 ],
                                        offset + upperVec[ 2 - 1 ]);
      // 3 jet bin sets
      BinNormClass::insertRangeIntoSet (bin3Set,                 
                                        offset + lowerVec[ 3 - 1 ],
                                        offset + upperVec[ 3 - 1 ]);
      // 4 jet bin sets
      // we want this set to be empty UNLESS there really are 4 jets
      if (numJets >= 4)
      {
         BinNormClass::insertRangeIntoSet (bin4Set,                 
                                           offset + lowerVec[ 4 - 1 ],
                                           offset + upperVec[ 4 - 1 ]);
      } // if 4 or more jets
   } // for tagger

   if (gDebugFlag)
   {
      // template sets
      debugISet ("hfTempSet  ",  hfTempSet);
      debugISet ("zjetTempSet ",  zjetTempSet);
      // bin sets
      debugISet ("allBinSet  ",  allBinSet);
      debugISet ("looseBinSet",  looseBinSet);
      debugISet ("tightBinSet",  tightBinSet);
      debugISet ("ultraBinSet",  ultraBinSet);
      debugISet ("bin1Set    ",  bin1Set);
      debugISet ("bin2Set    ",  bin2Set);
      debugISet ("bin3Set    ",  bin3Set);
      debugISet ("bin4Set    ",  bin4Set);
   }

   // Define the binNormClasses
   BinNormClass binNorm1  (zjetTempSet, bin1Set,   1.);
   BinNormClass binNorm2  (zjetTempSet, bin2Set,   1.);
   BinNormClass binNorm3  (zjetTempSet, bin3Set,   1.);
   BinNormClass binNorm4  (zjetTempSet, bin4Set,   1.);
   BinNormClass hfBinNorm (hfTempSet,   allBinSet, 1.0);

   ////////////////////////
   // Set Fitter Options //
   ////////////////////////

   cout << "Fitter Configuration:" << endl
        << "  - " << numJets << " jet bins" << endl
        << "  - " << numTaggers << " taggers present" << endl;
   
   if (gUlrich)
   {
      cout << "  - Using Ulrich's templates" << endl;
      gOutputName += "_Ulrich";
      if (gOldStyle) 
      {
         gOutputName += "_oldStyle";
      cout << "    - Old Style HFOR" << endl;
      } // old style
   } // if gUlrich
   
   // Do we want to float combined bottom and charm HF fractions?
   if (gUseHF)
   {
      cout << "  - Combined HF fit" << endl;
      // let combined HF float
      fitter.addBinNorm ("HF", hfBinNorm);
      if ( gFloatJet1)
      {
         // Let 1 jet bin float.
         cout << "  - floating 1 jet bin" << endl;
         fitter.addBinNorm ("OneJet", binNorm1);
      }
      // just to be sure
      gNoHF = false;
      // fix the individual flavors
      //fitter.fixParameter ("light");
      fitter.fixParameter ("charm");
      fitter.fixParameter ("bottom");
      gOutputName += "_hf";
   }
   // Or no HF fractions floating?
   else if (gNoHF) 
   {
      cout << "  - No flavor fractions floating." << endl;
      // fix the individual flavors
      fitter.fixParameter ("light");
      fitter.fixParameter ("charm");
      fitter.fixParameter ("bottom");
      // let 1 jet bin float
      if (! gFloatJet1)
      {
         fitter.addBinNorm ("OneJet", binNorm1);
      }
      gOutputName += "_nohf";
   } 
   else {
      cout << "  - All flavor fractions floating" << endl;
   }

   // Keep the 4th jet bins? (probably not a good idea unless there's
   // data there)
   if (gKeep4thJet)
   {
      cout << "  - Not excluding 4 jet bins" << endl;
      gOutputName += "_4jetbin";
   } else {
      fitter.removeBinsFromFit (bin4Set);
   }

   // do we want to not fit the loose tagger
   if (gNoLoose)
   {
      cout << "  - Excluding loose tagger from fit" << endl;
      fitter.removeBinsFromFit (looseBinSet);
      --taggersUsed;
      gOutputName += "_noloose";
   }
   // do we want to not fit the tight tagger
   if (gNoTight)
   {
      cout << "  - Excluding tight tagger from fit" << endl;
      fitter.removeBinsFromFit (tightBinSet);
      --taggersUsed;
      gOutputName += "_notight";
   }
   // do we want to not fit the ultra tagger
   if (gNoUltra)
   {
      cout << "  - Excluding ultra tagger from fit" << endl;
      fitter.removeBinsFromFit (ultraBinSet);
      if (numTaggers >= 3)
      {
         --taggersUsed;
      }
      gOutputName += "_noultra";
   }

   if (gFloatJet1)
   {
      // If we are letting all three flavors float, we do not (in
      // general) want to let 'OneJet' float as well as this is now
      // redundant.
      cout << "  - Forcing 1st jet bin normalization to float" << endl;
      fitter.addBinNorm ("OneJet", binNorm1);
   }
   fitter.addBinNorm ("TwoJet",   binNorm2);
   fitter.addBinNorm ("ThreeJet", binNorm3);
   fitter.addBinNorm ("FourJet",  binNorm4);
   fitter.fixParameter ("FourJet");
   
   cout << endl;

   // We never fit the 4 jet bins (there isn't any data there to fit
   // too anyway, but just to be sure)
   
   
   //////////////////
   // //////////// //
   // // FIT!!! // //
   // //////////// //
   //////////////////

   fitter.fit();
   if (gShowResults) fitter.outputFitResults();   
   if (gShowCovar)   fitter.outputCovarianceMatrix();
   if (gShowCorr)    fitter.outputCorrelationMatrix();

   ///////////////////////////////////////////////
   // ///////////////////////////////////////// //
   // // Calculate 4th Jet Bin Tagging Rates // //
   // ///////////////////////////////////////// //
   ///////////////////////////////////////////////

   // get name-index map
   CLPTemplateFitter::SIMap paramIndexMap =
      fitter.getNameIndexMap();
   // get 'TwoJet' and 'ThreeJet' indicies
   int twoJetIndex = -1, threeJetIndex = -1, fourJetIndex = -1;
   CLPTemplateFitter::SIMapConstIter simIter = paramIndexMap.find ("TwoJet");
   if (paramIndexMap.end() == simIter)
   {
      cerr << "Can not find 'TwoJet'.  Aborting." << endl;
      return 1;
   } else {
      twoJetIndex = simIter->second;
   }
   simIter = paramIndexMap.find ("ThreeJet");
   if (paramIndexMap.end() == simIter)
   {
      cerr << "Can not find 'ThreeJet'.  Aborting." << endl;
      return 1;
   } else {
      threeJetIndex = simIter->second;
   }
   simIter = paramIndexMap.find ("FourJet");
   if (paramIndexMap.end() == simIter)
   {
      cerr << "Can not find 'FourJet'.  Aborting." << endl;
      return 1;
   } else {
      fourJetIndex = simIter->second;
   }
   // get parameter values
   CLPTemplateFitter::DVec paramVec;
   fitter.fillParamVec (paramVec);
   // get covariance matrix
   CLPTemplateFitter::SVec paramNamesVec;
   TMatrix covarMat = fitter.getCovarianceMatrix (paramNamesVec);
   int size = covarMat.GetNrows();
   // The errors on the covariance matrix are too small if we used
   // multiple taggers.  Blow them up accordingly
   covarMat *= (double) taggersUsed;
   // make the 'indexVec' which holds the mapping between place in
   // covariance matrix and place in parameter values.
   CLPTemplateFitter::IVec indexVec;
   TVectorD meanTVecD (size);
   int index = 0;
   for (CLPTemplateFitter::SVecConstIter iter = paramNamesVec.begin();
        paramNamesVec.end() != iter;
        ++iter)
   {
      CLPTemplateFitter::SIMapConstIter mapIter =
         paramIndexMap.find( *iter );
      if (paramIndexMap.end() == mapIter)
      {
         // we didn't find it
         cerr << "Could not find '" << *iter << "'.  Aborting."
              << endl;
         return 1;
      }
      indexVec.push_back( mapIter->second );
      meanTVecD( index++ ) = paramVec[ mapIter->second ];
   } // for iter
   if (0)
   {
      for (CLPTemplateFitter::SIMapConstIter iter = paramIndexMap.begin();
           paramIndexMap.end() != iter;
           ++iter)
      {
         cout << setw(2) << iter->second << " : "
              << iter->first << endl;
      } // for iter
      cout << "parameters: " << paramVec << endl;
      cout << "covMat" << endl << covarMat << endl;
      for (CLPTemplateFitter::SVecConstIter iter = paramNamesVec.begin();
           paramNamesVec.end() != iter;
           ++iter)
      {
         cout << *iter << endl;
      } // for iter
      cout << endl;
   }

   TMatrixD sqrtCovMat (size, size);
   sqrtMatrix (sqrtCovMat, covarMat);
   double fourJet3_sum = 0, fourJet3_sum2 = 0;
   double fourJet23_sum = 0, fourJet23_sum2 = 0;
   // setup sumVec and sum2Vec;
   CLPTemplateFitter::DVec sumVec, sum2Vec;
   for (int tagger = 0; tagger < 3 * numTaggers; ++tagger)
   {
      sumVec.push_back (0.);
      sum2Vec.push_back (0.);
   } // for tagger
   CLPTemplateFitter::DVec jetVec, jet2Vec;
   for (int loop = 0; loop < 3; ++loop)
   {
      jetVec.push_back (0.);
      jet2Vec.push_back (0.);
   }
   int jetbin = 4;
   int lower = lowerVec[ jetbin - 1 ];
   int upper = upperVec[ jetbin - 1 ];
   for (int loop = 0; loop < gNumTrials; ++loop)
   {
      TVectorD retTVecD (size);
      gaussCorrRand (retTVecD, sqrtCovMat, meanTVecD);
      for (int index = 0; index < size; ++index)
      {
         paramVec[ indexVec[ index ] ] = retTVecD (index);
      } // for index
      double fourJetNorm3  = 0;
      double fourJetNorm23 = 0;
      for (int tagger = 0; tagger < numTaggers; ++tagger)
      {
         int offset = tagger * taggerOffset;
         double denominator   = 0;
         double denominator3  = 0;
         double denominator23 = 0;
         double numerator     = 0;
         double numerator3    = 0;
         double numerator23   = 0;
         
         double twoJet   = paramVec [twoJetIndex];
         double threeJet = paramVec [threeJetIndex];
         fourJetNorm3  = threeJet;
         fourJetNorm23 =  (2 * threeJet - twoJet);
         for (int bin = lower; bin <= upper; ++bin)
         {
            // unscaled
            paramVec[fourJetIndex] = 1;
            double content 
               = fitter.getTotalBinContent (bin + offset, paramVec);
            // 3 scalled
            paramVec[fourJetIndex] = fourJetNorm3;
            double content3 
               = fitter.getTotalBinContent (bin + offset, paramVec);
            // 23 scalled
            paramVec[fourJetIndex] = fourJetNorm23;
            double content23 
               = fitter.getTotalBinContent (bin + offset, paramVec);
            denominator   += content;
            denominator3  += content3;
            denominator23 += content23;
            // don't count the very first bin
            if (bin > lower)
            {
               // Not the first bin
               numerator   += content;
               numerator3  += content3;
               numerator23 += content23;
            }
         } // for bin
         // Look at jet 4 bin yields, but only for one of the taggers
         // (as it's going to be the same for all of the taggers)
         if (0 == tagger)
         {
            jetVec[0]  += denominator;  
            jetVec[1]  += denominator3; 
            jetVec[2]  += denominator23;
            jet2Vec[0] += denominator   * denominator;  
            jet2Vec[1] += denominator3  * denominator3; 
            jet2Vec[2] += denominator23 * denominator23;
            double twoJet   = paramVec [twoJetIndex];
            double threeJet = paramVec [threeJetIndex];
            double fourJet3  = denominator * threeJet;
            double fourJet23 =  denominator *  (2 * threeJet - twoJet);
            fourJet3_sum   += fourJet3;
            fourJet3_sum2  += fourJet3  * fourJet3;
            fourJet23_sum  += fourJet23;
            fourJet23_sum2 += fourJet23 * fourJet23;
         } // if loose tagger
         double tagRate   = numerator   / denominator;
         double tagRate3  = numerator3  / denominator3;
         double tagRate23 = numerator23 / denominator23;
         sumVec [3 * tagger    ] += tagRate;  
         sumVec [3 * tagger + 1] += tagRate3; 
         sumVec [3 * tagger + 2] += tagRate23;
         sum2Vec[3 * tagger    ] += tagRate   * tagRate;  
         sum2Vec[3 * tagger + 1] += tagRate3  * tagRate3; 
         sum2Vec[3 * tagger + 2] += tagRate23 * tagRate23;
      } // for tagger
   } // for loop

   if (gNumTrials)
   {
      for (int tagger = 0; tagger < 3 * numTaggers; ++tagger)
      {
         string taggername;         
         if (0 == tagger / 3) taggername = "loose";
         if (1 == tagger / 3) taggername = "tight";
         if (2 == tagger / 3) taggername = "ultra";
         if (0 == tagger % 3) taggername += " unscaled   ";
         if (1 == tagger % 3) taggername += " 3 scaled   ";
         if (2 == tagger % 3) taggername += " 23 unscaled";
         double mean, rms;
         calcMeanRMS (mean, rms, sumVec[tagger], sum2Vec[tagger], gNumTrials);
         cout << taggername << " tag rate:   "
              <<  Form ("%5.2f%% +- %.2f%%", 100 * mean, 100 * rms) << endl;
      } // for tagger
      cout << endl;
      for (int loop = 0; loop < 3; ++loop)
      {
         string jetname;
         if (0 == loop) jetname = "unscaled    ";
         if (1 == loop) jetname = "3 scaled    ";
         if (2 == loop) jetname = "23 unscaled ";
         double mean, rms;
         calcMeanRMS (mean, rms, jetVec[loop], jet2Vec[loop], gNumTrials);
         cout << jetname << " jet count:       "
              <<  Form ("%5.2f +- %.2f", mean, rms) << endl;
      } // for loop
   } // if gNumTrials

   //////////////////////
   // //////////////// //
   // // Make Plots // //
   // //////////////// //
   //////////////////////

   // general style options
   gStyle->SetOptStat(0000);
   gStyle->SetOptLogy();

   ////////////
   // Before //
   ////////////
   if (gBeforePlot)
   {
      TCanvas *c1 = new TCanvas;
      THStack *beforeStack = new THStack ("before", "Before Templates");
      TH1F *lightBefore  = fitter.updatedHistogram ("light" , false);
      TH1F *charmBefore  = fitter.updatedHistogram ("charm" , false);
      TH1F *bottomBefore = fitter.updatedHistogram ("bottom", false);
      TH1F *ttbarBefore  = fitter.updatedHistogram ("smttbar", false);
      beforeStack->Add (lightBefore );
      beforeStack->Add (charmBefore );
      beforeStack->Add (bottomBefore);
      beforeStack->Add (ttbarBefore );
      float stackMax = beforeStack->GetMaximum();
      if (dataMax > stackMax)
      {
         beforeStack->SetMaximum (dataMax);
      }
      beforeStack->Draw("hist");
      double ymax = beforeStack->GetMaximum() * 1000;
      beforeStack->SetMaximum (ymax);
      drawLines (bins, ymax);
      data->Draw("P SAME");
      c1->Print( Form ( "images/%s_before.eps", gOutputName.c_str() ) );
      delete c1;
   } // if making 'before' plots

   ///////////
   // After //
   ///////////
   if (gAfterPlot)
   {
      TCanvas *c1 = new TCanvas;
      THStack *afterStack = new THStack ("after", "After Templates");
      TH1F *lightAfter  = fitter.updatedHistogram ("light" );
      TH1F *charmAfter  = fitter.updatedHistogram ("charm" );
      TH1F *bottomAfter = fitter.updatedHistogram ("bottom");
      TH1F *ttbarAfter  = fitter.updatedHistogram ("smttbar");
      // lightAfter->SetFillColor (kRed);
      // charmAfter->SetFillColor (kGreen);
      // bottomAfter->SetFillColor (kBlue);
      // ttbarAfter->SetFillColor (kYellow);
      afterStack->Add (lightAfter );
      afterStack->Add (charmAfter );
      afterStack->Add (bottomAfter);
      afterStack->Add (ttbarAfter );
      double stackMax = afterStack->GetMaximum();
      if (dataMax > stackMax)
      {
         afterStack->SetMaximum (dataMax);
      }
      double ymax = afterStack->GetMaximum() * 1000;
      afterStack->SetMaximum (ymax);
      afterStack->Draw("hist");
      drawLines (bins, ymax);
      data->SetMarkerStyle(20);
      data->SetMarkerSize(1.0);
      data->Draw("P SAME hist");
      c1->Print( Form ( "images/%s_after.eps", gOutputName.c_str() ) );
      delete c1;      
   } // if making 'after' plots

   ///////////
   // Ratio //
   ///////////
   if (gRatioPlot)
   {
      gStyle->SetOptLogy(0);
      TCanvas *c1 = new TCanvas;
      TH1F *ratio = fitter.ratioHistogram();
      ratio->SetMarkerStyle (20);
      double ymax = ratio->GetMaximum();
      ymax *= 2.;
      ratio->SetMaximum (ymax);
      ratio->SetMarkerStyle(20);
      ratio->SetMarkerSize (1.0);
      ratio->Draw();
      TAxis *axis = ratio->GetXaxis();
      double lower = axis->GetXmin();
      double upper = axis->GetXmax();
      TLine *horLine = new TLine (lower, 1, upper, 1);
      horLine->SetLineColor (kBlue);
      horLine->Draw ("same");
      drawLines (bins, ymax, false);
      c1->Print( Form ( "images/%s_ratio.eps", gOutputName.c_str() ) );
      delete c1;
   } // if making 'ratio' plots

   //////////////
   // Residual //
   //////////////
   if (gResidualPlot)
   {
      gStyle->SetOptLogy(0);
      TCanvas *c1 = new TCanvas;
      TH1F *residual = fitter.residualHistogram();
      residual->SetMarkerStyle (20);
      double ymax = residual->GetMaximum();
      double ymin = residual->GetMinimum();
      if (ymin < 0)
      {
         ymin *= 1.1;
      }
      residual->SetMinimum (ymin);
      ymax = 2 * (ymax - ymin) + ymin;
      //ymax *= 2.;
      residual->SetMaximum (ymax);
      residual->Draw("P");
      TAxis *axis = residual->GetXaxis();
      double lower = axis->GetXmin();
      double upper = axis->GetXmax();
      TLine *horLine = new TLine (lower, 0, upper, 0);
      horLine->SetLineColor (kBlue);
      horLine->Draw ("same");
      drawLines (bins, ymax, false, ymin);
      c1->Print( Form ( "images/%s_residual.eps", gOutputName.c_str() ) );
      delete c1;
   } // if making 'residual' plots
}

///////////////////////
// ///////////////// //
// // Subroutines // //
// ///////////////// //
///////////////////////

void printHist (TH1F *hPtr)
{
   int bins = hPtr->GetNbinsX();
   cout << hPtr->GetName() << " :";
   for (int loop = 1; loop <= bins; ++loop)
   {
      cout << "  " << hPtr->GetBinContent (loop);
   }
   cout << endl;
}

bool setupBinEdges (int   bins,
                    IVec &lowerVec,
                    IVec &upperVec,
                    int  &numJets,
                    int  &numTaggers,
                    int  &taggerOffset)
{
   int offsets[6] = {0, 2, 5, 8, 11, 14};
   // do we understand the number of bins
   numJets = 0;
   numTaggers = 0;
   const int kMaxJet = 4;
   if (16 == bins)
   {
      numJets = 3;
      numTaggers = 2;
   } else if (22 == bins)
   {
      numJets = 4;
      numTaggers = 2;
   } else if (24 == bins)
   {
      numJets = 3;
      numTaggers = 3;
   } else if (33 == bins)
   {
      numJets = 4;
      numTaggers = 3;
   } else if (18 == bins)
   {
      numJets = 3;
      numTaggers = 2;
      offsets[3] = 9;
   } else {
      // don't know what we're doing
      cout << "How can you have " << bins << " bins?  Aborting." << endl;
      return false;
   }
   taggerOffset = bins / numTaggers;
   lowerVec.clear();
   upperVec.clear();
   for (int jet = 0; jet < kMaxJet; ++jet)
   {
      lowerVec.push_back ( offsets [jet    ] + 1 );
      upperVec.push_back ( offsets [jet + 1]     );
   } // for jet
   return true;
}

void finishInitialization()
{
   // In any case, we do NOT want to overwrite any names if they have
   // been specified from the command line.
   // Set defaults:
   if (1 == gWhich)
   {
      if (! gDataName.length())
      {
         gDataName   = "rootfiles/tags_data.root";  
      }
      if (! gBottomName.length())
      {
         gBottomName = "rootfiles/tags_bottom.root";
      }
      if (! gCharmName.length())
      {
         gCharmName  = "rootfiles/tags_charm.root"; 
      }
      if (! gLightName.length())
      {
         gLightName  = "rootfiles/tags_light.root"; 
      }
   } // if gWhich = 1
   if (2 == gWhich)
   {
      gDataName   = "rootfiles/tags_data_4jets_noveto.root";      
   }
}

void debugISet (const string &tag, const BinNormClass::ISet &iset)
{
   cout << tag << " : ";
   BinNormClass::outputISet (iset);
   cout << endl;
}

void calcMeanRMS (double &mean, double &rms, double sum, double sum2,
                  int trials)
{
   mean = sum / trials;
   double aveX2 = sum2 / trials;
   double mean2 = mean * mean;
   if (aveX2 < mean2)
   {
      rms = 0.;
   } else {
      rms = sqrt (aveX2 - mean * mean);
   }
}

void localInitialize()
{
   // option name is case insensitive.  Capitalization is for
   // readability only.
   addOptionKey ("which",        gWhich);
   addOptionKey ("mvl",          gMinuitVerboseLevel);
   addOptionKey ("numpes",       gNumTrials);
   addOptionKey ("numtrials",    gNumTrials);
   addOptionKey ("debug",        gDebugFlag);
   addOptionKey ("output",       gOutputName);
   addOptionKey ("ulrich",       gUlrich);
   addOptionKey ("ulrichname",   gUlrichName);
   addOptionKey ("oldstyle",     gOldStyle);
   addOptionKey ("floatjet1",    gFloatJet1);
   addOptionKey ("keep4th",      gKeep4thJet);
   addOptionKey ("nojet1",       gNoJet1);
   addOptionKey ("nojet2",       gNoJet2);
   addOptionKey ("nojet3",       gNoJet3);
   addOptionKey ("noLoose",      gNoLoose);
   addOptionKey ("noTight",      gNoTight);
   addOptionKey ("noUltra",      gNoUltra);
   addOptionKey ("HF",           gUseHF);
   addOptionKey ("noHF",         gNoHF);
   addOptionKey ("dataname",     gDataName);
   addOptionKey ("bottomname",   gBottomName);
   addOptionKey ("charmname",    gCharmName);
   addOptionKey ("lightname",    gLightName);
   addOptionKey ("showResults",  gShowResults);
   addOptionKey ("showCovar",    gShowCovar);
   addOptionKey ("showCorr",     gShowCorr);
   addOptionKey ("beforePlot",   gBeforePlot);
   addOptionKey ("afterPlot",    gAfterPlot);
   addOptionKey ("ratioPlot",    gRatioPlot);
   addOptionKey ("residualPlot", gResidualPlot);
}
