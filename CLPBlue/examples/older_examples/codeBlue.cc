// -*- C++ -*-
// Standard include files
#include <math.h>
#include <iostream>
#include <assert.h>
#include <stdio.h>
// CLP include files
#include "XsecComb.h"
#include "CLPBlue.h"
#include "CLPAsymBlue.h"
#include "CLPUtils.h"
#include "dout.h"
// Root include files
#include "TSystem.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

using namespace std;

int main(int argc, char** argv)
{
   
   /////////////////////
   // Parse Arguments //
   /////////////////////
   bool doAccptErrors = false;
   bool doBkgndErrors = false;
   TString extension = "";
   for (int loop = 2; loop <= argc; ++loop)
   {
      TString arg = argv[loop - 1];
      if (arg.BeginsWith("back", TString::kIgnoreCase))
      {
         doBkgndErrors = true;
         continue;
      }
      if (arg.BeginsWith("acc", TString::kIgnoreCase))
      {
         doAccptErrors = true;
         continue;
      }
      //  // convert from TString to string
      //  string sarg = (const char*) arg;
      //  cout << "sarg " << sarg << endl;
   }
   cout << "extra modes: ";
   if (doBkgndErrors)
   {
      extension += "_bk";
      cout << "background ";
   }
   if (doAccptErrors)
   {
      extension += "_acc";
      cout << "acceptance ";
   }
   cout << endl;
   ///////////////
   // Setup COD //
   ///////////////
   CLPHisto::setNoMinInterval();
   XsecComb::initializeClass();
   XsecComb dil;
   TString filename = "cod_files/dil" + extension + ".cod";
   dil.initializeWithFile( (const char*) filename );
   XsecComb had;
   filename = "cod_files/had" + extension + ".cod";
   had.initializeWithFile( (const char*) filename );
   XsecComb cod1;
   filename = "cod_files/hadDil" + extension + ".cod";
   cod1.initializeWithFile( (const char*) filename );
   XsecComb secvtx;
   filename = "cod_files/secVtx" + extension + ".cod";
   secvtx.initializeWithFile( (const char*) filename );
   XsecComb nnfake;
   filename = "cod_files/largeLJ" + extension + ".cod";
   nnfake.initializeWithFile( (const char*) filename );
   XsecComb cod2;
   filename = "cod_files/sv_nn" + extension + ".cod";
   cod2.initializeWithFile( (const char*) filename );

   ////////////////
   // Setup BLUE //
   ////////////////
   CLPBlue blue1;
   filename = "blue_files/hadDil_correct" + extension + ".blue";
   blue1.readFromFile( (const char*) filename );
   CLPBlue blue1b;
   filename = "blue_files/hadDil_sqrt"  + extension + ".blue";
   blue1b.readFromFile( (const char*) filename );
   CLPBlue blue2;
   filename = "blue_files/lj" + extension + ".blue";
   blue2.readFromFile( (const char*) filename );

   ///////////////////////////
   // Setup Asymetric BLUE //
   //////////////////////////
   CLPAsymBlue asym1;
   asym1.readFromFile( "blue_files/hadDil.aib" );
   CLPAsymBlue asym2;
   filename = "blue_files/lj" + extension + ".aib";
   asym2.readFromFile( (const char*) filename );

   // assign acceptance * luminosity numbers
   double lumi = XsecComb::getTotalPb();
   // dilepton
   double dilAcc = dil.getAcceptance(0);
   double dilBack = dil.getBackground(0);
   double dilAL = lumi * dilAcc;
   double dilAccErr = 0.135;
   double dilBkErr = 0.28;
   // all hadronic
   double hadAcc = had.getAcceptance(0);
   double hadBack = had.getBackground(0);
   double hadAL = lumi * hadAcc;
   double hadAccErr = 0.08;
   double hadBkErr = 0.065;
   // SecVtx L + J
   double secvtxAcc = secvtx.getAcceptance(0);
   double secvtxBack = secvtx.getBackground(0);
   double secvtxAL = lumi * secvtxAcc;
   double secvtxAccErr = 0.08;
   double secvtxBkErr = 0.143;
   // Neural net
   double nnfakeAcc = nnfake.getAcceptance(0);
   double nnfakeBack = nnfake.getBackground(0);
   double nnfakeAL = lumi * nnfakeAcc;
   double nnfakeAccErr = 0.08;
   // rest (secvtx is a complete subset of neural net)
   double restBkErr = 0.044;
   double restBack = nnfakeBack - secvtxBack;
   double restAL   = nnfakeAL   - secvtxAL;
   // vectors to be used sending new observed results to COD
   DVec vec, vec2;
   vec.push_back(0.);
   vec2.push_back(0.);
   vec2.push_back(0.);

   filename = "output" + extension + ".root";
   TFile file (filename, "RECREATE", "hist file");
   cout << "opened " << filename << endl;
   // root ntuple variables
   double xsec;
   double dilVal, hadVal, cod1Val, secvtxVal, nnfakeVal, cod2Val, 
      blue1Val, blue1aVal, blue1bVal, blue2Val, blue2aVal,
      asym1Val, asym2Val;
   double dilErr, hadErr, cod1Err, secvtxErr, nnfakeErr, cod2Err, 
      blue1Err, blue1aErr, blue1bErr, blue2Err, blue2aErr,
      cod1Perr, cod1Nerr, asym1Perr, asym1Nerr, 
      cod2Perr, cod2Nerr, asym2Perr, asym2Nerr;
   double dilPull, hadPull, cod1Pull, secvtxPull, nnfakePull, cod2Pull, 
      blue1Pull, blue1aPull, blue1bPull, blue2Pull, blue2aPull,
      asym1Pull, asym2Pull;
   // interative() and non-iterative (0) weights respectively
   double dilWght, hadWght, secvtxWght, nnfakeWght, 
      dil0Wght, had0Wght, secvtx0Wght, nnfake0Wght;
   TTree *treePtr = new TTree("comb", "Cross section combination");
   // cross sections
   treePtr->Branch("xsec"       , &xsec       , "xsec/D"       );
   treePtr->Branch("dilVal"     , &dilVal     , "dilVal/D"     );
   treePtr->Branch("hadVal"     , &hadVal     , "hadVal/D"     );
   treePtr->Branch("cod1Val"    , &cod1Val    , "cod1Val/D"    );
   treePtr->Branch("secvtxVal"  , &secvtxVal  , "secvtxVal/D"  );
   treePtr->Branch("nnfakeVal"  , &nnfakeVal  , "nnfakeVal/D"  );
   treePtr->Branch("cod2Val"    , &cod2Val    , "cod2Val/D"    );
   treePtr->Branch("blue1Val"   , &blue1Val   , "blue1Val/D"   );
   treePtr->Branch("blue1aVal"  , &blue1aVal  , "blue1aVal/D"  );
   treePtr->Branch("blue1bVal"  , &blue1bVal  , "blue1bVal/D"  );
   treePtr->Branch("blue2Val"   , &blue2Val   , "blue2Val/D"   );
   treePtr->Branch("blue2aVal"  , &blue2aVal  , "blue2aVal/D"  );
   treePtr->Branch("asym1Val"   , &asym1Val   , "asym1Val/D"   );
   treePtr->Branch("asym2Val"   , &asym2Val   , "asym2Val/D"   );
   // errors                                                   
   treePtr->Branch("dilErr"     , &dilErr     , "dilErr/D"     );
   treePtr->Branch("hadErr"     , &hadErr     , "hadErr/D"     );
   treePtr->Branch("cod1Err"    , &cod1Err    , "cod1Err/D"    );
   treePtr->Branch("cod1Perr"   , &cod1Perr   , "cod1Perr/D"   );
   treePtr->Branch("cod1Nerr"   , &cod1Nerr   , "cod1Nerr/D"   );
   treePtr->Branch("secvtxErr"  , &secvtxErr  , "secvtxErr/D"  );
   treePtr->Branch("nnfakeErr"  , &nnfakeErr  , "nnfakeErr/D"  );
   treePtr->Branch("cod2Err"    , &cod2Err    , "cod2Err/D"    );
   treePtr->Branch("cod2Perr"   , &cod2Perr   , "cod2Perr/D"   );
   treePtr->Branch("cod2Nerr"   , &cod2Nerr   , "cod2Nerr/D"   );
   treePtr->Branch("blue1Err"   , &blue1Err   , "blue1Err/D"   );
   treePtr->Branch("blue1aErr"  , &blue1aErr  , "blue1aErr/D"  );
   treePtr->Branch("blue1bErr"  , &blue1bErr  , "blue1bErr/D"  );
   treePtr->Branch("blue2Err"   , &blue2Err   , "blue2Err/D"   );
   treePtr->Branch("blue2aErr"  , &blue2aErr  , "blue2aErr/D"  );
   treePtr->Branch("asym1Perr"  , &asym1Perr  , "asym1Perr/D"  );
   treePtr->Branch("asym1Nerr"  , &asym1Nerr  , "asym1Nerr/D"  );
   treePtr->Branch("asym2Perr"  , &asym2Perr  , "asym2Perr/D"  );
   treePtr->Branch("asym2Nerr"  , &asym2Nerr  , "asym2Nerr/D"  );
   // pull distributions                                       
   treePtr->Branch("dilPull"    , &dilPull    , "dilPull/D"    );
   treePtr->Branch("hadPull"    , &hadPull    , "hadPull/D"    );
   treePtr->Branch("cod1Pull"   , &cod1Pull   , "cod1Pull/D"   );
   treePtr->Branch("secvtxPull" , &secvtxPull , "secvtxPull/D" );
   treePtr->Branch("nnfakePull" , &nnfakePull , "nnfakePull/D" );
   treePtr->Branch("cod2Pull"   , &cod2Pull   , "cod2Pull/D"   );
   treePtr->Branch("blue1Pull"  , &blue1Pull  , "blue1Pull/D"  );
   treePtr->Branch("blue1aPull" , &blue1aPull , "blue1aPull/D" );
   treePtr->Branch("blue1bPull" , &blue1bPull , "blue1bPull/D" );
   treePtr->Branch("blue2Pull"  , &blue2Pull  , "blue2Pull/D"  );
   treePtr->Branch("blue2aPull" , &blue2aPull , "blue2aPull/D" );
   treePtr->Branch("asym1Pull"  , &asym1Pull  , "asym1Pull/D"  );
   treePtr->Branch("asym2Pull"  , &asym2Pull  , "asym2Pull/D"  );
   // blue weights                                             
   treePtr->Branch("dilWght"    , &dilWght    , "dilWght/D"    );
   treePtr->Branch("hadWght"    , &hadWght    , "hadWght/D"    );
   treePtr->Branch("secvtxWght" , &secvtxWght , "secvtxWght/D" );
   treePtr->Branch("nnfakeWght" , &nnfakeWght , "nnfakeWght/D" );
   treePtr->Branch("dil0Wght"   , &dil0Wght   , "dil0Wght/D"   );
   treePtr->Branch("had0Wght"   , &had0Wght   , "had0Wght/D"   );
   treePtr->Branch("secvtx0Wght", &secvtx0Wght, "secvtx0Wght/D");
   treePtr->Branch("nnfake0Wght", &nnfake0Wght, "nnfake0Wght/D");
   treePtr->Branch("nnfake0Wght", &nnfake0Wght, "nnfake0Wght/D");


   //////////////////////
   // Let's go play... //
   //////////////////////
   for (int loop = 0; loop < 10000; ++loop)
   {
      //////////////////////////////
      // Throw all random numbers //
      //////////////////////////////
      xsec = gRandom->Gaus (6.7, 0.8);
      //xsec = gRandom->Gaus (7., 1.0);
      double dilAccSmear = 1.;
      double hadAccSmear = 1.;
      double secvtxAccSmear = 1.;
      double nnfakeAccSmear = 1.;
      if (doAccptErrors)
      {
         // throw ONE random nubmer
         double accSmear = gRandom->Gaus (0.0, 1.0);
         // use it to make MANY smearings
         dilAccSmear = 1. + accSmear * dilAccErr;
         hadAccSmear = 1. + accSmear * hadAccErr;
         secvtxAccSmear = 1. + accSmear * secvtxAccErr;
         nnfakeAccSmear = 1. + accSmear * nnfakeAccErr;
         // sanity checks
         if (dilAccSmear < 0.01)
         {
            dilAccSmear = 0.01;
            dout << endl;
         }
         if (hadAccSmear < 0.01)
         {
            hadAccSmear = 0.01;
            dout << endl;
         }
         if (secvtxAccSmear < 0.01)
         {
            secvtxAccSmear = 0.01;
            dout << endl;
         }
         if (nnfakeAccSmear < 0.01)
         {
            nnfakeAccSmear = 0.01;
            dout << endl;
         }
      } // do acceptance errors
      double dilBkSmear = 1.;
      double hadBkSmear = 1.;
      double secvtxBkSmear = 1.;
      double restBkSmear = 1.;
      if (doBkgndErrors)
      {
         // Background errors are almost completely UNcorrelated, so
         // Throw MANY different numbers
         dilBkSmear = gRandom->Gaus (1.0, dilBkErr);
         hadBkSmear = gRandom->Gaus (1.0, hadBkErr);
         secvtxBkSmear = gRandom->Gaus (1.0, secvtxBkErr);
         restBkSmear = gRandom->Gaus (1.0, restBkErr);
         if (dilBkSmear < 0.)
         {
            dilBkSmear = 0.;
            dout << endl;
         }
         if (hadBkSmear < 0.)
         {
            hadBkSmear = 0.;
            dout << endl;
         }
         if (secvtxBkSmear < 0.)
         {
            secvtxBkSmear = 0.;
            dout << endl;
         }
         if (restBkSmear < 0.)
         {
            restBkSmear = 0.;
            dout << endl;
         }         
      }
      double dilExp    = dilAL * dilAccSmear * xsec 
         + dilBack * dilBkSmear;
      double dilObs    = gRandom->Poisson(dilExp);
      double hadExp    = hadAL * hadAccSmear * xsec 
         + hadBack * hadBkSmear;
      double hadObs    = gRandom->Poisson(hadExp);
      double secvtxExp = secvtxAL * secvtxAccSmear * xsec 
         + secvtxBack * secvtxBkSmear;
      double secvtxObs = gRandom->Poisson(secvtxExp);
      double restExp   = restAL * nnfakeAccSmear * xsec 
         + restBack * restBkSmear;
      double restObs   = gRandom->Poisson(restExp);
      //double nnfakeExp = secvtxExp + restExp;
      double nnfakeObs = secvtxObs + restObs;

      /////////////////
      // Do Dil only //
      /////////////////
      vec[0] = dilObs;
      dil.setObserved( vec );
      dil.calculate(true); // calulate with errors
      double dilPlus, dilMinus;
      dil.getCnstrntResults (dilVal, dilPlus, dilMinus);
      dilPull = pullValue (xsec, dilVal, dilPlus, dilMinus);
      dilErr = (dilPlus + dilMinus) / 2;

      /////////////////
      // Do Had only //
      /////////////////
      vec[0] = hadObs;
      had.setObserved( vec );
      had.calculate(true); // calulate with errors
      double hadPlus, hadMinus;
      had.getCnstrntResults (hadVal, hadPlus, hadMinus);
      hadPull = pullValue (xsec, hadVal, hadPlus, hadMinus);
      hadErr = (hadPlus + hadMinus) / 2;

      ////////////////////
      // Do Secvtx only //
      ////////////////////
      vec[0] = secvtxObs;
      secvtx.setObserved( vec );
      secvtx.calculate(true); // calulate with errors
      double secvtxPlus, secvtxMinus;
      secvtx.getCnstrntResults (secvtxVal, secvtxPlus, secvtxMinus);
      secvtxPull = pullValue (xsec, secvtxVal, secvtxPlus, secvtxMinus);
      secvtxErr = (secvtxPlus + secvtxMinus) / 2;

      ////////////////////
      // Do Nnfake only //
      ////////////////////
      vec[0] = nnfakeObs;
      nnfake.setObserved( vec );
      nnfake.calculate(true); // calulate with errors
      double nnfakePlus, nnfakeMinus;
      nnfake.getCnstrntResults (nnfakeVal, nnfakePlus, nnfakeMinus);
      nnfakePull = pullValue (xsec, nnfakeVal, nnfakePlus, nnfakeMinus);
      nnfakeErr = (nnfakePlus + nnfakeMinus) / 2;

      /////////////////////////
      // Do COD1 combination //
      /////////////////////////
      vec2[0] = hadObs; // hadron is measurement 0
      vec2[1] = dilObs; // dilepton is measurement 1
      cod1.setObserved( vec2 );
      cod1.calculate(true); // calulate with errors
      cod1.getCnstrntResults (cod1Val, cod1Perr, cod1Nerr);
      cod1Pull = pullValue (xsec, cod1Val, cod1Perr, cod1Nerr);
      cod1Err = (cod1Perr + cod1Nerr) / 2;

      /////////////////////////
      // Do COD2 combination //
      /////////////////////////
      vec2[0] = secvtxObs; // secvtx is measurement 0
      vec2[1] = restObs; // rest is measurement 1
      cod2.setObserved( vec2 );
      cod2.calculate(true); // calulate with errors
      cod2.getCnstrntResults (cod2Val, cod2Perr, cod2Nerr);
      cod2Pull = pullValue (xsec, cod2Val, cod2Perr, cod2Nerr);
      cod2Err = (cod2Perr + cod2Nerr) / 2;

      /////////////////////////
      // Do BLUE combination //
      /////////////////////////
      // blue1
      blue1.setMean (0, hadVal); // hadron is measurement 0
      blue1.setMean (1, dilVal); // dilepton is measurement 1
      blue1.calcBlueAverage(blue1aVal, blue1aErr);
      DVec weights = blue1.getWeights();
      had0Wght = weights[0];
      dil0Wght = weights[1];
      blue1aPull = pullValue (xsec, blue1aVal, blue1aErr);
      blue1.calcIterativeBlueAverage(blue1Val, blue1Err);
      weights = blue1.getWeights();
      hadWght = weights[0];
      dilWght = weights[1];
      blue1Pull = pullValue (xsec, blue1Val, blue1Err);

      // blue1b
      blue1b.setMean (0, hadVal); // hadron is measurement 0
      blue1b.setMean (1, dilVal); // dilepton is measurement 1
      blue1b.calcIterativeBlueAverage(blue1bVal, blue1bErr);
      blue1bPull = pullValue (xsec, blue1bVal, blue1bErr);

      // blue2
      blue2.setMean (0, secvtxVal); // secvtx is measurement 0
      blue2.setMean (1, nnfakeVal); // nnfake is measurement 1
      blue2.calcBlueAverage(blue2aVal, blue2aErr);
      weights = blue2.getWeights();
      secvtx0Wght = weights[0];
      nnfake0Wght = weights[1];
      blue2aPull = pullValue (xsec, blue2aVal, blue2aErr);
      blue2.calcIterativeBlueAverage(blue2Val, blue2Err);
      weights = blue2.getWeights();
      secvtxWght = weights[0];
      nnfakeWght = weights[1];
      blue2Pull = pullValue (xsec, blue2Val, blue2Err);

      ////////////////////////////////////
      // Do Asymmetric BLUE Combination //
      ////////////////////////////////////
      // asym1
      asym1.setMean (0, hadVal); // hadron is measurement 0
      asym1.setMean (1, dilVal); // dilepton is measurement 1
      asym1.calcIterativeBlueAverage(asym1Val, asym1Perr, asym1Nerr);
      asym1Pull = pullValue (xsec, asym1Val, asym1Perr, asym1Nerr);

      // asym2
      asym2.setMean (0, secvtxVal); // secvtx is measurement 0
      asym2.setMean (1, nnfakeVal); // nnfake is measurement 1
      asym2.calcIterativeBlueAverage(asym2Val, asym2Perr, asym2Nerr);
      asym2Pull = pullValue (xsec, asym2Val, asym2Perr, asym2Nerr);

      // You worked this hard, you might as well record
      // everything
      treePtr->Fill();
   } // for loop
   cout << "done" << endl;
   
   // write tree
   treePtr->Write();
   cout << "Wrote " << filename << endl;
   file.Close();
}
