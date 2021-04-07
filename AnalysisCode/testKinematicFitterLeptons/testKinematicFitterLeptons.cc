// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"

#include "time.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>

#include "CLPClasses/CLPClasses.h"
#include "KinematicFitter/KinematicFitter.hh"
#include "KinematicFitter/dout.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;


// number of events (numwanted/howmany)
int gNumWanted     = 10;
// source file name (file)
string gFilename   = "silly2.root";
// target file name (target)
string gTarget     = "output.root";
// verbose mode (verbose)
bool gVerbose      = false;
// use M_P basis (mass)
bool gUseMassBasis = true;
// which CLP mode to use (clp)
int gCLPmode       = 1;
// number of iterations (iter)
int gNumIterations = 0;
// use fast covar mode (covar)
int gCovarMode     = 1;
// CLP scale everything (everything)
bool gScaleEverything = false;
// use the Z width (usezwidth)
bool gUseZwidth = true;
// Z width to use (width)
double gZwidth  = 2.495;
// error resolution (deltae)
double gRelDeltaE  = 0.035;
// degrees of freedom (dof)
int gDof = 1;
// use Breit Wigner width (breit)
bool gUseBreitWigner = true;
// use the aleph bug (aleph)
bool gUseAlephBug    = false;

void initializeOptionsMap();

int main(int argc, char** argv)
{
   initializeOptionsMap();
   parseArguments (argc, argv);
   if (gUseAlephBug)
   {
      KinematicFitter::setUseAlephBug (true);
   }

   double Zwidth = -1;
   if (gUseZwidth) {
      Zwidth = gZwidth;
   }
   ////////////////////
   // Open Root File //
   ////////////////////
   TFile *filePtr = TFile::Open (gFilename.c_str());
   //filePtr->ls();
   TTree *treePtr = (TTree*) filePtr->Get ("T4Vecs");
   if (! treePtr)
   {
      cerr << "Yeah, that worked well." << endl;
      exit(1);
   } 
   TClonesArray *reconTcaPtr = new TClonesArray ("TLorentzVector", 2);
   TClonesArray *hepgTcaPtr  = new TClonesArray ("TLorentzVector", 2);
   treePtr->GetBranch("recon")->SetAutoDelete(false);
   treePtr->SetBranchAddress("recon", &reconTcaPtr);
   treePtr->GetBranch("hepg")->SetAutoDelete(false);
   treePtr->SetBranchAddress("hepg", &hepgTcaPtr);
   Int_t nentries = Int_t(treePtr->GetEntries());
   cout << "Possible entries: " << nentries << endl;
   if (gNumWanted && (gNumWanted < nentries))
   {
      nentries = gNumWanted;    
   }

   //////////////////////
   // Output Root File //
   //////////////////////
   TFile *outputFilePtr = 0;
   if (1 || ! gNumWanted)
   {
      outputFilePtr = TFile::Open (gTarget.c_str(), "RECREATE");
      if (! outputFilePtr->IsOpen())
      {
         cerr << "Couldn't open " << gTarget << " for output." << endl;
         exit(1);
      }
   }
   TH1F *zTtaHist 
      = new TH1F ("zTta", "Z daughter delta theta", 50, -0.02, 0.02);
   TH1F *zPhiHist 
      = new TH1F ("zPhi", "Z daughter delta phi", 50, -0.02, 0.02);
   TH1F *zEngHist 
      = new TH1F ("zEng", "Z daughter delta E / E", 50, -0.4, 0.4);
   TH1F *zMassHist
      = new TH1F ("zMass", "Z Mass", 100, 50, 150);
   TH1F *zMass1Hist
      = new TH1F ("zMass1", "Good Z Mass", 100, 50, 150);
   TH1F *zMass2Hist
      = new TH1F ("zMass2", "Little Z Mass", 100, 50, 150);
   TH1F *zMass3Hist
      = new TH1F ("zMass3", "Bad Z Mass", 100, 50, 150);
   TH1F *probHist
      = new TH1F ("prob", "chi^2 probability", 50, 0., 1.);
   TH1F *chi2Hist
      = new TH1F ("chi2", "chi^2 probability", 50, 0., 10.);

   CLPPermutation *permPtr = new CLPPermutation;
   TTree *permTreePtr = new TTree ("perms", "perms");
   permTreePtr->Branch ("Permutation", "CLPPermutation", &permPtr);

   /////////////////////////////////
   // Holly, The Kinematic Fitter //
   /////////////////////////////////
   KinematicFitter fitter;
   if (gNumIterations > 0)
   {
      fitter.setNumIterations (gNumIterations);
   }
   if (gUseMassBasis)
   {
      fitter.changeBasis (KinematicFitter::kM_P_basis);
   } else {
      fitter.changeBasis (KinematicFitter::kE_P_basis);
   }
   int Zc = fitter.createVirtualParticle();

   KinematicFitter::IntrinsicWidthType widthType;
   if (gUseBreitWigner)
   {
      widthType = KinematicFitter::kBreitWignerWidth;
   } else {
      widthType = KinematicFitter::kGaussianWidth;
   }

   // Put Mass constraints on the virtual particles
   // The Ws
   int constr_Z1;
   constr_Z1 = fitter.addSingleMassConstraint (Zc, 91.188, 
                                                  Zwidth, widthType);
   // save the current constraints as the defaults.
   fitter.saveCurrentConstraintsAsDefault();
   // Fast covariance mode?
   if (1 == gCovarMode)    
   {
      cout << "Fast Covar mode" << endl;
      fitter.setCovarianceMode (KinematicFitter::kFastCovarMode);
   } else if (2 == gCovarMode)    
   {
      cout << "Keep Covar mode" << endl;
      fitter.setCovarianceMode (KinematicFitter::kKeepCovarMode);
   } else if (3 == gCovarMode)
   {
      cout << "Recalc Covar mode" << endl;
      fitter.setCovarianceMode (KinematicFitter::kRecalcCovarMode);
   } else {
      cout << "Normal Covar mode" << endl;
      fitter.setCovarianceMode (KinematicFitter::kNormalCovarMode);
   }

   // variables for RMS
   double sumX = 0., sumX2 = 0.;
   int count = 0;
   
   for (int loop = 0; loop < nentries; ++loop)
   {
      if (false && loop >= 19 && loop <= 21)
      {
         KinematicFitter::setDebugLevel(1);
         cout << endl << endl << endl << endl << endl 
              << "**************************************************" << endl
              << "**************************************************" << endl
              << "**************************************************" << endl
              << "**************************************************" << endl
              << "event " 
              << loop << endl;
      } else {
         KinematicFitter::setDebugLevel(0);
      }
      if (gVerbose)
      {
         cout << endl << endl << "entry " << loop + 1 << endl;
      } 
      if (0 == (loop + 1) % (500))
      {
         cout << loop + 1 << endl;
      }
      treePtr->GetEntry (loop);
      assert ( reconTcaPtr->GetEntries() >= 2 && 
               hepgTcaPtr->GetEntries()  >= 2 );

      TLorentzVector lep1vec    ( *((TLorentzVector*) reconTcaPtr->At(0)) );
      TLorentzVector lep2vec    ( *((TLorentzVector*) reconTcaPtr->At(1)) );
      TLorentzVector lep1hepg   ( *((TLorentzVector*) hepgTcaPtr->At(0))  );
      TLorentzVector lep2hepg   ( *((TLorentzVector*) hepgTcaPtr->At(1))  );

      vector< TLorentzVector > lepVecs, lepHepgs, wVecs, wHepgs;
      // lep vectors
      lepVecs.push_back (lep1vec);
      lepVecs.push_back (lep2vec);
      lepHepgs.push_back (lep1hepg);
      lepHepgs.push_back (lep2hepg);

      for (unsigned int inner = 0; inner < lepVecs.size(); ++inner)
      {
         TLorentzVector &vec  = lepVecs[inner];
         TLorentzVector &hepg = lepHepgs[inner];
         double deltaPhi = vec.DeltaPhi (hepg);
         double deltaE   = (vec.E() - hepg.E()) / hepg.E();
         double tta1 = vec.Theta();
         double tta2 = hepg.Theta();
         zTtaHist->Fill (tta1-tta2);
         zPhiHist->Fill (deltaPhi);
         zEngHist->Fill (deltaE);
      }
      double dE_rel = gRelDeltaE;
      double dTta = 0.0; //012;
      double dPhi = 0.0; //005;
      /////////////////////////////////////
      // Setup the Kinematic Mass Fitter //
      /////////////////////////////////////
      fitter.clearAllInputParticles();
      // Add input particles
      int z1d1, z1d2;
      double dE_z1d1 = dE_rel * lep1vec.E();
      double dE_z1d2 = dE_rel * lep1vec.E();
      z1d1 = fitter.addInputParticle (lep1vec, dE_z1d1, dTta, dPhi);
      z1d2 = fitter.addInputParticle (lep2vec, dE_z1d2, dTta, dPhi);
      // update the Zs
      fitter.updateVirtualParticle (Zc, z1d1, z1d2);
      if (gVerbose) 
      {
         cout << fitter << endl;
      }
      // minimize the chi^2
      TLorentzVector z1beforevec, z1aftervec;
      // Original vectors
      KinematicFitter::FourVectorVec origVec, modVec;
      double chi2 = fitter.minimizeChi2();
      double prob = TMath::Prob (chi2, gDof);
      fitter.fillAllVirtualMomentumVector (origVec,
                                           KinematicFitter::kOriginal);
      fitter.fillAllVirtualMomentumVector (modVec,
                                           KinematicFitter::kModified);
      z1beforevec   = origVec [Zc];
      z1aftervec    = modVec [Zc];
      double zMass = z1aftervec.M();
      permPtr->setMass ( z1aftervec.M()  );
      permPtr->setMass2( z1beforevec.M() );
      permPtr->setChi2 (chi2);
      permPtr->setUserDouble (0, prob);
      permTreePtr->Fill();
      zMassHist->Fill (zMass);
      chi2Hist->Fill(chi2);
      probHist->Fill ( prob );
      if (prob > 0.14)
      {
         zMass1Hist->Fill(zMass);
      } else if (prob < 0.02) 
      {
         zMass3Hist->Fill(zMass);
      } else {
         zMass2Hist->Fill(zMass);
      }
      
      sumX += zMass;
      sumX2 += zMass * zMass;
      ++count;
      double ns1_kf = 
         (fitter.E(z1d1, KinematicFitter::kModified) -
          fitter.E(z1d1, KinematicFitter::kOriginal))
         / dE_z1d1;
      double ns2_kf = 
         (fitter.E(z1d2, KinematicFitter::kModified) -
          fitter.E(z1d2, KinematicFitter::kOriginal))
         / dE_z1d2;
      if (gVerbose)
      {
         cout << "ns1_kf " << ns1_kf << " ns2_kf " << ns2_kf 
              << " chi2 " << ns1_kf * ns1_kf + ns2_kf * ns2_kf
              << " rat " << ns1_kf / ns2_kf
              << " rat2 " << ns1_kf * ns1_kf / (ns2_kf * ns2_kf)
              << endl;
      } // if verbose
      // CLP
      double numsig1 = 0;
      // Z1
      CLP4Vector z1d1clp (lep1vec);
      CLP4Vector z1d2clp (lep2vec);
      if (1 == gCLPmode)
      {
         numsig1 = fix4VectorsForMass (z1d1clp, z1d2clp, 91.188,
                                       dE_z1d1, dE_z1d2);
      } 
      CLP4Vector z1clp   = z1d1clp + z1d2clp;
      if (2 == gCLPmode)
      {
         z1clp.setRestMass (91.188);
      }
      
      // HEPG
      TLorentzVector z1hepg   = lep1hepg + lep2hepg;
      
      if (gVerbose)
      {
         // before
         cout << endl 
              << setw(20) <<  "Before:" 
              << setw(40) <<  "After:" 
              << setw(40) <<  "CLP:" 
              << setw(40) <<  "HEPG:" 
              << endl;
         cout << "Z1d1    " 
              << fitter.particleMom (z1d1, KinematicFitter::kOriginal)
              << setw(4) << " " << fitter.particleMom (z1d1)
              << setw(4) << " " << z1d1clp 
              << setw(4) << " " << lep1hepg << endl;
         cout << "Z1d2    " 
              << fitter.particleMom (z1d2, KinematicFitter::kOriginal)
              << setw(4) << " " << fitter.particleMom (z1d2)
              << setw(4) << " " << z1d2clp 
              << setw(4) << " " << lep2hepg << endl;
         cout << "Z1      " << z1beforevec 
              << setw(4) << " " << z1aftervec 
              << setw(4) << " " << z1clp 
              << setw(4) << " " << z1hepg << endl;
         cout << "Z Mass 1" 
              << setw(15) << " " << z1beforevec.M()
              << setw(32) << " " << z1aftervec.M()
              << setw(32) << " " << z1clp.mass() 
              << setw(32) << " " << z1hepg.M() 
              << endl;
         cout << setw(65) << "chi^2 " << chi2 
              << setw(23);
         cout << " chi2 " << 2 * numsig1 * numsig1 
              << "  ns1 " << numsig1 << endl;
      } // if verbose
   } // for loop
   if (1 || ! gNumWanted)
   {
      zTtaHist->Write();
      zPhiHist->Write();
      zEngHist->Write();
      zMassHist->Write();
      zMass1Hist->Write();
      zMass2Hist->Write();
      zMass3Hist->Write();
      probHist->Write();
      chi2Hist->Write();
      permTreePtr->Write();
   }
   double xbar = sumX / count;
   double x2bar = sumX2 / count;
   cout << "Width " << zMassHist->GetRMS() 
        << " center " << zMassHist->GetMean()
        << " " << sqrt (x2bar - xbar * xbar)
        << endl;
   zMassHist->Fit ("gaus");
   return 0;
}

void initializeOptionsMap()
{
   // Int maps
   ns_IntMap["numwanted"]  = (void*) &gNumWanted;
   ns_IntMap["num"]        = (void*) &gNumWanted;
   ns_IntMap["howmany"]    = (void*) &gNumWanted;
   ns_IntMap["clp"]        = (void*) &gCLPmode;
   ns_IntMap["iter"]       = (void*) &gNumIterations;
   ns_IntMap["covar"]      = (void*) &gCovarMode;
   ns_IntMap["dof"]        = (void*) &gDof;
   // String maps
   ns_StringMap["file"]    = (void*) &gFilename;
   ns_StringMap["target"]  = (void*) &gTarget;
   // Bool maps
   ns_BoolMap["verbose"]   = (void*) &gVerbose;
   ns_BoolMap["mass"]      = (void*) &gUseMassBasis;
   ns_BoolMap["usezwidth"] = (void*) &gUseZwidth;
   ns_BoolMap["breit"]     = (void*) &gUseBreitWigner;
   ns_BoolMap["aleph"]     = (void*) &gUseAlephBug;
   // Double maps
   ns_DoubleMap["width"]   = (void*) &gZwidth;
   ns_DoubleMap["deltae"]  = (void*) &gRelDeltaE;
}
