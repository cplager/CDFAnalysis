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
#include "TRandom3.h"
#include "TTree.h"

// CLP include files
#include "RootFileCollection/OptionUtils.h"
#include "dumpSTL.icc"

using namespace std;
using namespace OptionUtils;

// forward declarations
void localInitialize();
double evalPoly4 (double mt, double par[4]);
void theoryXsec (int mode, double mass,
                 double &xsec, double &upErr, double &dnErr);

// constants
double kDefaultTopMass = 175.; // GeV
double kAcceptFactor   = 0.00413112;

// Global variables (ugh!) for command line options
// Number of PEs (numpes)
int  gNumPEs = 1000;
// output file (output)
string gOutputName = "ljPEs";
// branching fraction (bf)
double gBF = 0.;
// ratio of acceptances for WZ to WW (rwz) and ZZ to WW (rzz)
double gRwz = 0.3177; // for t -> Zc
double gRzz = 0.0195; // for t -> Zc
// luminosity (lum)
double gLum = 1.9; // fb^-1
// top mass
double gTopMass = kDefaultTopMass;
// theory mode
int gTheoryMode = 0;
// seed
int gSeed = 1;
// acceptance times cross section and percent uncertainty (acc, accPerUnc)
double gAcceptance      = 14.653; // per mille (= pb * fb^-1)
double gAcceptPerUncert = 0.1663;
// background (per fb^-1) and percent uncertainty (back, backPerUnc);
double gBackground    = 16.053; // events per fb^-1
double gBackPerUncert = 0.4033;
// assume t->gamma c (gammac)
bool gGammaC = false;
// assume t->glue c (gluec)
bool gGlueC  = false;


int main (int argc, char** argv)
{
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);
   // TRandom is lousy.  Use TRandom3 instead.
   delete gRandom;
   gRandom = new TRandom3;
   gRandom->SetSeed (gSeed);

   // rescale acceptance for top mass
   double percentDeltaAccept = (gTopMass - kDefaultTopMass) * kAcceptFactor;
   gAcceptance *= 1 + percentDeltaAccept;
   cout << "percentDeltaAccept = " << percentDeltaAccept << endl;

   // are we doing gammaC or glueC
   if (gGammaC)
   {
      gRwz = 0.1833; // for t -> gamma c
      gRzz = 0.0195; // for t -> gamma c
   } else if (gGlueC)
   {
      gRwz = 0.2716; // for t -> g c
      gRzz = 0.0195; // for t -> g c      
   }

   // setup filename
   removeEnding (gOutputName, ".root");
   if (gTheoryMode)
   {
      gOutputName += "_MSTW";
   } else {
      gOutputName += "_CTEQ";
   }   
   gOutputName += Form ("_mass%.1f", gTopMass);
   gOutputName += Form ("_bf%04.1f%%_rwz%.1f%%_rzz%.2f%%_lum%.1f_sec%d_pes%d",
                        100 * gBF, 100 * gRwz, 100 * gRzz, 
                        gLum, gSeed, gNumPEs);
   gOutputName += ".root";
   cout << "Writing to " << gOutputName << endl;

   // get theoretical xsec and uncertainties
   double xsec, xsecUp, xsecDn;
   theoryXsec (gTheoryMode, gTopMass, xsec, xsecUp, xsecDn);
   cout << "mass " << gTopMass << " : " << xsec << " +" << xsecUp
        << " -" << xsecDn << endl;
   
   // make output tree
   int numObs = 0;
   TFile *outputFilePtr = TFile::Open (gOutputName.c_str(), "RECREATE");
   assert (outputFilePtr);
   TTree *treePtr = new TTree ("PEtree", "PEtree");
   treePtr->Branch ("numObs", &numObs, "numObs/i");

   // calculate the basics
   double accLum    = gAcceptance * gLum;
   double accLumUnc = accLum * gAcceptPerUncert;
   double back      = gBackground * gLum;
   double backUnc   = back * gBackPerUncert;

   double percWW = (1 - gBF) * (1 - gBF);
   double percWZ = 2 * (1 - gBF) * gBF;
   double percZZ = gBF * gBF;
   cout << "sum = " << percWW + percWZ + percZZ << endl;
   double factor = 1. * percWW + gRwz * percWZ + gRzz * percZZ;
   int redos = 0;
   for (int peLoop = 0; peLoop < gNumPEs; ++peLoop)
   {
      double xsecShift = gRandom->Gaus (0, 1);
      double peAccLum  = gRandom->Gaus (accLum, accLumUnc);
      double peBack    = gRandom->Gaus (back,   backUnc);
      double lumFact   = gRandom->Gaus (1,      0.059);
      double peXsec = -1.;
      if (xsecShift > 0)
      {
         peXsec = xsec + xsecShift * xsecUp;
      } else {
         peXsec = xsec + xsecShift * xsecDn;
      }
      if (peXsec <= 0 || peAccLum <= 0 || peBack <= 0)
      {
         if (++redos > 0.2 * gNumPEs)
         {
            cerr << "We have more than 20% redos after "
                 << peLoop << " (" << gNumPEs 
                 << ")  events.  Aborting." << endl;
            assert (0);
         }
         --peLoop;
         continue;
      }
      numObs = gRandom->Poisson( lumFact * peXsec * peAccLum * factor + peBack);
      treePtr->Fill();
   } // for peLoop
   cout << "numRedo " << redos << endl;
   outputFilePtr->cd();
   treePtr->Write();
   delete outputFilePtr;
}

double evalPoly4 (double mt, const double par[4])
{
  double dmt = mt - 171.0;
  return par[0] + dmt * ( par[1] + dmt * ( par[2] + dmt * par[3] ) );
}

void theoryXsec (int mode, double mass,
                 double &xsec, double &upErr, double &dnErr)
{
  const double par_cteq_central[4] = { 7.61, -0.237, 4.38e-3, -6.28e-5 };
  const double par_cteq_scalep[4]  = { 7.90, -0.247, 4.58e-3, -6.61e-5 };
  const double par_cteq_scalem[4]  = { 7.08, -0.221, 4.10e-3, -5.89e-5 };
  const double par_cteq_pdfp[4]    = { 8.14, -0.256, 4.78e-3, -6.91e-5 };
  const double par_cteq_pdfm[4]    = { 7.24, -0.224, 4.11e-3, -5.85e-5 };
  const double par_mstw_central[4] = { 7.93, -0.253, 4.76e-4, -6.92e-5 };
  const double par_mstw_scalep[4]  = { 8.27, -0.264, 5.00e-3, -7.33e-5 };
  const double par_mstw_scalem[4]  = { 7.37, -0.235, 4.44e-3, -6.45e-5 };
  const double par_mstw_pdfp[4]    = { 8.17, -0.261, 4.93e-3, -7.19e-5 };
  const double par_mstw_pdfm[4]    = { 7.73, -0.245, 4.61e-3, -6.68e-5 };
  double scaleUp = 0., scaleDn = 0., pdfUp = 0., pdfDn = 0.;
  if (mode)
  {
     // MSTW
     xsec    =       evalPoly4 (mass, par_mstw_central);
     scaleUp = fabs( evalPoly4 (mass, par_mstw_scalep) - xsec );
     scaleDn = fabs( evalPoly4 (mass, par_mstw_scalem) - xsec );
     pdfUp   = fabs( evalPoly4 (mass, par_mstw_pdfp)   - xsec );
     pdfDn   = fabs( evalPoly4 (mass, par_mstw_pdfm)   - xsec );
  } else {
     // CTEQ
     xsec    =       evalPoly4 (mass, par_cteq_central);
     scaleUp = fabs( evalPoly4 (mass, par_cteq_scalep) - xsec );
     scaleDn = fabs( evalPoly4 (mass, par_cteq_scalem) - xsec );
     pdfUp   = fabs( evalPoly4 (mass, par_cteq_pdfp)   - xsec );
     pdfDn   = fabs( evalPoly4 (mass, par_cteq_pdfm)   - xsec );
  }
  upErr = sqrt( scaleUp * scaleUp + pdfUp * pdfUp );
  dnErr = sqrt( scaleDn * scaleDn + pdfDn * pdfDn );
}



void localInitialize()
{
   // option name is case insensitive.  Capitalization of the strings
   // is for readability only.
   addOptionKey ("numpes",     gNumPEs,
                 "Number of PEs");
   addOptionKey ("output",     gOutputName);
   addOptionKey ("bf",         gBF,   "non t->Wb branching fraction");
   addOptionKey ("rwz",        gRwz);
   addOptionKey ("rzz",        gRzz);
   addOptionKey ("lum",        gLum,  "luminosity");
   addOptionKey ("seed",       gSeed, "random seed");
   addOptionKey ("acc",        gAcceptance, "per mille acceptance");
   addOptionKey ("accPerUnc",  gAcceptPerUncert);
   addOptionKey ("back",       gBackground, "background per fb^-1");
   addOptionKey ("backPerUnc", gBackPerUncert);
   addOptionKey ("gammac",     gGammaC,
                 "Assume gamma charm - overrides gluec, rwz, and rzz changes");
   addOptionKey ("gluec",      gGlueC,
                 "Assume glue charm -  overrides rwz and rzz changes");
   addOptionKey ("topMass",    gTopMass);
   addOptionKey ("theoryMode", gTheoryMode,
                 "0 - CTEQ, 1 - MTSW");
}
