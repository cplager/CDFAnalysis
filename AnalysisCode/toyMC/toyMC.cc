// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>

#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TRandom3.h"
#include "time.h"

#include "CLPClasses/CLPClasses.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;


// number of events (numwanted/howmany)
int gNumWanted     = 10;
// target file name (target)
string gTarget     = "toymc";
// use Breit Wigner width (breit)
bool gUseBreitWigner = false;
// verbose mode (verbose)
bool gVerbose      = false;
// Z width to use (zwidth)
double gZwidth  = 2.5;
// error resolution
double gRelDeltaE  = 0.035;


void initializeOptionsMap();
void genZZ (TClonesArray *genp,  TClonesArray *reco, 
            double sigmaEoverE, double sigmaPhi, double sigmaTheta);

int main(int argc, char** argv)
{
   initializeOptionsMap();
   parseArguments (argc, argv);
   double sigmaPhi    = 0.0;
   double sigmaTheta  = 0.0;

   // update output name
   if (gUseBreitWigner)
   {
      gTarget += Form ("_breit%.1f", gZwidth);
   } else {
      gTarget += Form ("_gauss%.1f", gZwidth);
   }
   gTarget += Form ("_dE%.1f",  gRelDeltaE * 100.);
   gTarget += ".root";

   TFile *targetPtr = new TFile ( gTarget.c_str(), "RECREATE" );
   int runNum = 0;
   int eventNum = 1;

   //////////////////
   // Output Trees //
   //////////////////
   TClonesArray* reconVecTcaPtr = new TClonesArray ("TLorentzVector", 4);
   TClonesArray* hepgVecTcaPtr  = new TClonesArray ("TLorentzVector", 4);
   TTree* outputTreePtr = new TTree ("T4Vecs", "Six TLorentz Vectors");
   outputTreePtr->Branch ("runNum",     &runNum,     "runNum/I");
   outputTreePtr->Branch ("eventNum",   &eventNum,   "eventNum/I");
   outputTreePtr->Branch ("recon", &reconVecTcaPtr);
   outputTreePtr->Branch ("hepg",  &hepgVecTcaPtr);

   for (int loop = 0; loop < gNumWanted; ++loop)
   {
      genZZ(hepgVecTcaPtr, reconVecTcaPtr,
            gRelDeltaE, sigmaPhi, sigmaTheta);
      outputTreePtr->Fill();
   } // for loop
   targetPtr->Write();
   return 0;
}

void genZZ(TClonesArray *genp,  TClonesArray *reco, 
           double sigmaEoverE, double sigmaPhi, double sigmaTheta ) 
{
   // From Elliot Lipeles
   static TRandom3 rnd3;

   // check run
   reco->Clear();
   genp->Clear();

   // Working in the diquark dilepton channel
   TLorentzVector quark1;
   TLorentzVector quark2;
   TLorentzVector lepton1;
   TLorentzVector lepton2;
   TLorentzVector Z1;
   TLorentzVector Z2;
   TLorentzVector ZZ;

   // Setup for Z decay
   // Ref PDG Eq 38.15
   double Zmass1;
   double Zmass2;
   if (gUseBreitWigner)
   {
      Zmass1 = rnd3.BreitWigner (kZmass, gZwidth);
      Zmass2 = rnd3.BreitWigner (kZmass, gZwidth);
   } else {
      // Gaussian
      Zmass1 = rnd3.Gaus (kZmass, gZwidth);
      Zmass2 = rnd3.Gaus (kZmass, gZwidth);
   }
   // assume the daughers are all massless
   double lepton_energy_1 = Zmass1 / 2;
   double lepton_energy_2 = Zmass1 / 2;
   double quark_energy_1  = Zmass2 / 2;
   double quark_energy_2  = Zmass2 / 2;
   double quark_Phi  = 2.0 * PI * rnd3.Rndm();
   double lepton_Phi = 2.0 * PI * rnd3.Rndm();

   // These two variables will be re-used 
   double cos_theta = 0;
   double sin_theta = 0;
   // There is only one momentum value because they are back to back
   // in their rest frame
   double quark_momentum = 0;
   double lepton_momentum = 0;
   double Z_momentum = 0;

   // First, do quarks
   quark_momentum = quark_energy_1;
   cos_theta = 2.0 * rnd3.Rndm()-1.0;
   sin_theta = sqrt ( 1.0 - sq (cos_theta) );
   quark1.SetPxPyPzE (quark_momentum * cos(quark_Phi) * sin_theta, 
                      quark_momentum * sin(quark_Phi) * sin_theta, 
                      quark_momentum * cos_theta, 
                      quark_energy_1);
   quark2.SetPxPyPzE (-quark_momentum * cos(quark_Phi) * sin_theta, 
                      -quark_momentum * sin(quark_Phi) * sin_theta, 
                      -quark_momentum * cos_theta, 
                       quark_energy_2);
 
   // Second, do leptons
   lepton_momentum = lepton_energy_1;
   cos_theta = 2.0 * rnd3.Rndm() - 1.;
   sin_theta = sqrt (1.0-sq (cos_theta));
   lepton1.SetPxPyPzE (lepton_momentum * cos(lepton_Phi) * sin_theta, 
                       lepton_momentum * sin(lepton_Phi) * sin_theta, 
                       lepton_momentum * cos_theta, 
                       lepton_energy_1);
   lepton2.SetPxPyPzE (-lepton_momentum * cos(lepton_Phi) * sin_theta, 
                       -lepton_momentum * sin(lepton_Phi) * sin_theta,
                       -lepton_momentum * cos_theta, 
                        lepton_energy_2);


   // Now it's time to do the Zs
   double ZZ_momentum = sqrt ( sq (rnd3.Exp(50)) + 
                               sq (rnd3.Exp(50)) + 
                               sq (rnd3.Exp(50)) );
   double ZZ_mass      = 2.0 * sqrt (sq (Zmass1+Zmass2) + sq (ZZ_momentum));
   double ZZ_phi       = 2.0 * PI * rnd3.Rndm();
   double ZZ_cos_theta = 2.0 * rnd3.Rndm() - 1.0;
   double ZZ_sin_theta = sqrt (1.0 - sq (ZZ_cos_theta));

   ZZ.SetPxPyPzE (ZZ_momentum * cos(ZZ_phi) * ZZ_sin_theta, 
                  ZZ_momentum * sin(ZZ_phi) * ZZ_sin_theta, 
                  ZZ_momentum * ZZ_cos_theta,
                  sq (Zmass1 + Zmass2) + sq (ZZ_momentum));

   // Decay ZZ to two Zs
   double Z_energy    = sq (ZZ_mass) / (Zmass1 + Zmass2);
   double Z_phi       = 2.0 * PI * rnd3.Rndm();
   double Z_cos_theta = 2.0 * rnd3.Rndm()-1.0;
   double Z_sin_theta = sqrt (1.0-sq (Z_cos_theta));
  
   Z1.SetPxPyPzE (Z_momentum * cos(Z_phi) * Z_sin_theta, 
                  Z_momentum * sin(Z_phi) * Z_sin_theta, 
                  Z_momentum * Z_cos_theta, 
                  Z_energy);
   Z2.SetPxPyPzE (-Z_momentum * cos(Z_phi) * Z_sin_theta,
                  -Z_momentum * sin(Z_phi) * Z_sin_theta,
                  -Z_momentum * Z_cos_theta, 
                   Z_energy);


   // Now time to do the boosting.
   // First into the Z frame
   lepton1.Boost (Z2.BoostVector());
   lepton2.Boost (Z2.BoostVector());
   quark1.Boost  (Z1.BoostVector());
   quark2.Boost  (Z1.BoostVector());
  
   // Now into the ZZ frame
   lepton1.Boost (ZZ.BoostVector());
   lepton2.Boost (ZZ.BoostVector());
   quark1.Boost  (ZZ.BoostVector());
   quark2.Boost  (ZZ.BoostVector());
  

   new ( (*genp)[0] )  TLorentzVector (lepton1);
   new ( (*genp)[1] )  TLorentzVector (lepton2);
   new ( (*genp)[2] )  TLorentzVector (quark1);
   new ( (*genp)[3] )  TLorentzVector (quark2);

   for (int loop = 0; loop < 4; ++loop) 
   {
      TLorentzVector& gen = (*(TLorentzVector*)((*genp)[loop]));
    
      double recoE     = rnd3.Gaus (1, sigmaEoverE) * gen.E();
      double recoPhi   = rnd3.Gaus (0, sigmaPhi   ) + gen.Phi();
      double recoTheta = rnd3.Gaus (0, sigmaTheta ) + gen.Theta();
    
      new ( (*reco)[loop] ) 
         TLorentzVector (recoE * cos(recoPhi) * sin(recoTheta),
                         recoE * sin(recoPhi) * sin(recoTheta),
                         recoE * cos(recoTheta),
                         recoE);
   } // for loop


   lepton1 = (*(TLorentzVector*) ( (*reco)[0] ) );
   lepton2 = (*(TLorentzVector*) ( (*reco)[1] ) );
   quark1  = (*(TLorentzVector*) ( (*reco)[2] ) );
   quark2  = (*(TLorentzVector*) ( (*reco)[3] ) );


   if (gVerbose)
   {
      cout << " Z1->" << (quark1 + quark2).M()  << " Z2->" 
           << (lepton1 + lepton2).M() << endl;
   } // if verbose
} 

void initializeOptionsMap()
{
   // Int maps
   ns_IntMap["numwanted"]  = (void*) &gNumWanted;
   ns_IntMap["num"]        = (void*) &gNumWanted;
   ns_IntMap["howmany"]    = (void*) &gNumWanted;
   // String maps
   ns_StringMap["target"]  = (void*) &gTarget;
   // Bool maps
   ns_BoolMap["verbose"]   = (void*) &gVerbose;
   ns_BoolMap["breit"]     = (void*) &gUseBreitWigner;
   // Double maps
   ns_DoubleMap["zwidth"]  = (void*) &gZwidth;
   ns_DoubleMap["deltae"]  = (void*) &gRelDeltaE;
}
