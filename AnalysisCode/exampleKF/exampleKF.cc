// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <cassert>

#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"

#include "KinematicFitter/KinematicFitter.hh"
#include "KinematicFitter/dout.h"

using namespace std;

// number of events (numwanted/howmany)
int gNumWanted     = 10;
// source file name (file)
string gFilename   = "six_4vectors.root";
// target file name (target)
string gTarget     = "output.root";
// verbose mode (verbose)
bool gVerbose      = true;
// constrain first W only (wonly)
bool gFirstWonly   = false;
// use double mass constrinat (double)
int gMassConstraintMode    = 1;
// number of iterations (iter)
int gNumIterations = 0;
// use fast covar mode (covar)
int gCovarMode  = 1;
// silly loop (silly)
int gSilly = 1;

int main(int argc, char** argv)
{
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
   TClonesArray *reconTcaPtr = new TClonesArray ("TLorentzVector", 6);
   TClonesArray *hepgTcaPtr  = new TClonesArray ("TLorentzVector", 6);
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
   if (! gNumWanted)
   {
      outputFilePtr = TFile::Open (gTarget.c_str(), "RECREATE");
      if (! outputFilePtr->IsOpen())
      {
         cerr << "Couldn't open " << gTarget << " for output." << endl;
         exit(1);
      }
   }
   TH1F *oldHist 
      = new TH1F ("old", "OLD Method Average Top Mass", 40, 100, 300);
   TH1F *newHist 
      = new TH1F ("kmf", "KMF Method Average Top Mass", 40, 100, 300);
   
   cout << "gMassConstraintMode " << gMassConstraintMode << endl;

   KinematicFitter fitter;
   if (gNumIterations > 0)
   {
      fitter.setNumIterations (gNumIterations);
   }
   int W1c   = fitter.createVirtualParticle();
   int B1c   = fitter.createVirtualParticle();
   int Top1c = fitter.createSuperVirtualParticle (W1c, B1c);
   int W2c   = fitter.createVirtualParticle();
   int B2c   = fitter.createVirtualParticle();
   int Top2c = fitter.createSuperVirtualParticle (W2c, B2c);

   // Put Mass constraints on the virtual particles
   // The Ws
   int constr_W1 = fitter.addSingleMassConstraint (W1c,     80.425);
   int constr_W2 = fitter.addSingleMassConstraint (W2c,     80.425);
   // Now, the tops - Note we will not have all three active at the
   // same time.
   //    - Equal mass constraint
   int constr_top  = fitter.addEqualMassConstraint (Top1c, Top2c);
   //    - Two single mass constriants
   int constr_top1 = fitter.addSingleMassConstraint (Top1c, 175.);
   int constr_top2 = fitter.addSingleMassConstraint (Top2c, 175.);

   // First W only?
   if (gFirstWonly)
   {
      fitter.setConstraintInactive (constr_W2);
      fitter.setConstraintInactive (constr_top);
      fitter.setConstraintInactive (constr_top1);
      fitter.setConstraintInactive (constr_top2);
   } else 
   {
      // What should we do about the tops?
      if (1 == gMassConstraintMode)
      {
         cout << "double mass" << endl;
         fitter.setConstraintInactive (constr_top1);
         fitter.setConstraintInactive (constr_top2);
      } else if (2 == gMassConstraintMode)
      {
         // Use two single mass constraints
         cout << "single mass" << endl;
         fitter.setConstraintInactive (constr_top);
      } else {
         // Use no mass constraint
         cout << "no mass" << endl;
         fitter.setConstraintInactive (constr_top);
         fitter.setConstraintInactive (constr_top1);
         fitter.setConstraintInactive (constr_top2);
      }
   } // if ! gFirstOnly

   // save the current constraints as the defaults.
   fitter.saveCurrentConstraintsAsDefault();

   // Fast covariance mode?
   if (1 == gCovarMode)    
   {
      fitter.setCovarianceMode (KinematicFitter::kFastCovarMode);
   } else if (2 == gCovarMode)    
   {
      fitter.setCovarianceMode (KinematicFitter::kKeepCovarMode);
   } else {
      fitter.setCovarianceMode (KinematicFitter::kNormalCovarMode);
   }
   
   for (int loop = 0; loop < nentries; ++loop)
   {
      if (gVerbose)
      {
         cout << endl << endl << "entry " << loop + 1 << endl;
      } 
      if (0 == (loop + 1) % (500 / gSilly))
      {
         cout << loop + 1 << endl;
      }
      treePtr->GetEntry (loop);
      assert ( reconTcaPtr->GetEntries() == 6 && 
               hepgTcaPtr->GetEntries()  == 6 );


      // 0 - 1st bottom
      // 1 - W11
      // 2 - W12
      // 3 - 2nd bottom
      // 4 - W21
      // 5 - W22
      // Get the four vectors
      TLorentzVector b1vec    ( *((TLorentzVector*) reconTcaPtr->At(0)) );
      TLorentzVector w1d1vec  ( *((TLorentzVector*) reconTcaPtr->At(1)) );
      TLorentzVector w1d2vec  ( *((TLorentzVector*) reconTcaPtr->At(2)) );
      TLorentzVector b2vec    ( *((TLorentzVector*) reconTcaPtr->At(3)) );
      TLorentzVector w2d1vec  ( *((TLorentzVector*) reconTcaPtr->At(4)) );
      TLorentzVector w2d2vec  ( *((TLorentzVector*) reconTcaPtr->At(5)) );
      TLorentzVector b1hepg   ( *((TLorentzVector*) hepgTcaPtr->At(0))  );
      TLorentzVector w1d1hepg ( *((TLorentzVector*) hepgTcaPtr->At(1))  );
      TLorentzVector w1d2hepg ( *((TLorentzVector*) hepgTcaPtr->At(2))  );
      TLorentzVector b2hepg   ( *((TLorentzVector*) hepgTcaPtr->At(3))  );
      TLorentzVector w2d1hepg ( *((TLorentzVector*) hepgTcaPtr->At(4))  );
      TLorentzVector w2d2hepg ( *((TLorentzVector*) hepgTcaPtr->At(5))  );

      // Errors on the vectors
      double dE_b1   = 0.15 * b1vec.E();
      double dE_w1d1 = 0.15 * w1d1vec.E();
      double dE_w1d2 = 0.15 * w1d2vec.E();
      double dE_w2d1 = 0.15 * w2d1vec.E();
      double dE_w2d2 = 0.15 * w2d2vec.E();
      double dE_b2   = 0.15 * b2vec.E();
      
      for (int sillyLoop = 0; sillyLoop < gSilly; ++sillyLoop)
      {

         /////////////////////////////////////
         // Setup the Kinematic Mass Fitter //
         /////////////////////////////////////
         fitter.clearAllInputParticles();
         // Add input particles
         int w1d1, w1d2, b1, w2d1, w2d2, b2;
         b1   = fitter.addInputParticle (b1vec,   dE_b1  );
         w2d1 = fitter.addInputParticle (w2d1vec, dE_w2d1);
         w2d2 = fitter.addInputParticle (w2d2vec, dE_w2d2);
         b2   = fitter.addInputParticle (b2vec,   dE_b2  );
         w1d1 = fitter.addInputParticle (w1d1vec, dE_w1d1);
         w1d2 = fitter.addInputParticle (w1d2vec, dE_w1d2);
         // update the Ws
         fitter.updateVirtualParticle (W1c, w1d1, w1d2);
         fitter.updateVirtualParticle (W2c, w2d1, w2d2);
         // update the Bs
         fitter.updateVirtualParticle (B1c, b1);
         fitter.updateVirtualParticle (B2c, b2);
      
         if (gVerbose) 
         {
            cout << fitter << endl;
         }
         // minimize the chi^2
         double chi2 = fitter.minimizeChi2();

         TLorentzVector w1beforevec, w1aftervec, w2beforevec, w2aftervec, 
            top1beforevec, top1aftervec, top2beforevec, top2aftervec; 
         double beforeTopAve, afterTopAve;
         // Original vectors
         KinematicFitter::FourVectorVec origVec, modVec;
         fitter.fillAllVirtualMomentumVector (origVec,
                                              KinematicFitter::kOriginal);
         fitter.fillAllVirtualMomentumVector (modVec,
                                              KinematicFitter::kModified);
         w1beforevec   = origVec [W1c];
         w1aftervec   = modVec [W1c];
         if (! gFirstWonly)
         {
            w2beforevec   = origVec [W2c];
            top1beforevec = origVec [Top1c];
            top2beforevec = origVec [Top2c];
            w2aftervec    = modVec  [W2c];
            top1aftervec  = modVec  [Top1c];
            top2aftervec  = modVec  [Top2c];
         }
         beforeTopAve = (top1beforevec.M() + top2beforevec.M() ) / 2.;
         afterTopAve = (top1aftervec.M() + top2aftervec.M() ) / 2.;

         // HEPG
         TLorentzVector w1hepg   = w1d1hepg + w1d2hepg;
         TLorentzVector top1hepg = b1hepg   + w1hepg;
         TLorentzVector w2hepg   = w2d1hepg + w2d2hepg;
         TLorentzVector top2hepg = b2hepg   + w2hepg;
         double hepgTopAve = ( top1hepg.M() + top2hepg.M() ) / 2.;

         newHist->Fill ( afterTopAve );
         oldHist->Fill ( beforeTopAve ); 

         if (gVerbose)
         {
            // before
            cout << endl 
                 << setw(20) <<  "Before:" 
                 << setw(40) <<  "After:" 
                 << setw(40) <<  "HEPG:" 
                 << endl;
            if (! gFirstWonly) 
            {
               cout << "b1      " 
                    << fitter.particleMom (b1, KinematicFitter::kOriginal)
                    << setw(4) << " " << fitter.particleMom (b1)
                    << setw(4) << " " << b1hepg << endl;
            }
            cout << "W1d1    " 
                 << fitter.particleMom (w1d1, KinematicFitter::kOriginal)
                 << setw(4) << " " << fitter.particleMom (w1d1)
                 << setw(4) << " " << w1d1hepg << endl;
            cout << "W1d2    " 
                 << fitter.particleMom (w1d2, KinematicFitter::kOriginal)
                 << setw(4) << " " << fitter.particleMom (w1d2)
                 << setw(4) << " " << w1d2hepg << endl;
            cout << "W1      " << w1beforevec 
                 << setw(4) << " " << w1aftervec 
                 << setw(4) << " " << w1hepg << endl;
            if (! gFirstWonly) cout << "top1    " << top1beforevec 
                                    << setw(4) << " " << top1aftervec 
                                    << setw(4) << " " << top1hepg << endl; 
            cout << "W Mass 1" 
                 << setw(15) << " " << w1beforevec.M()
                 << setw(32) << " " << w1aftervec.M()
                 << setw(32) << " " << w1hepg.M() 
                 << endl;
            if (! gFirstWonly)
            {
               cout << "Top Mass 1" 
                    << setw(12) << " " << top1beforevec.M()
                    << setw(32) << " " << top1aftervec.M()
                    << setw(32) << " " << top1hepg.M() 
                    << endl;

               cout << "b2      " 
                    << fitter.particleMom (b2, KinematicFitter::kOriginal)
                    << setw(4) << " " << fitter.particleMom (b2)
                    << setw(4) << " " << b2hepg << endl;
               cout << "W2d1    " 
                    << fitter.particleMom (w2d1, KinematicFitter::kOriginal)
                    << setw(4) << " " << fitter.particleMom (w2d1)
                    << setw(4) << " " << w2d1hepg << endl;
               cout << "W2d2    " 
                    << fitter.particleMom (w2d2, KinematicFitter::kOriginal)
                    << setw(4) << " " << fitter.particleMom (w2d2)
                    << setw(4) << " " << w2d2hepg << endl;
               cout << "W2      " << w2beforevec 
                    << setw(4) << " " << w2aftervec 
                    << setw(4) << " " << w2hepg << endl;
               cout << "top2    " << top2beforevec 
                    << setw(4) << " " << top2aftervec 
                    << setw(4) << " " << top2hepg << endl;
               cout << "W Mass 2" 
                    << setw(15)  << " " << w2beforevec.M()
                    << setw(32) << " " << w2aftervec.M()
                    << setw(32) << " " << w2hepg.M() 
                    << endl;
               cout << "Top Mass 2" 
                    << setw(12) << " " << top2beforevec.M()
                    << setw(32) << " " << top2aftervec.M()
                    << setw(32) << " " << top2hepg.M() 
                    << endl;
               cout << "Top Mass Average " 
                    << setw(5)  << " " << beforeTopAve
                    << setw(32) << " " << afterTopAve
                    << setw(32) << " " << hepgTopAve
                    << endl;
            } // if ! firstOnly
            cout << setw(65) << "chi^2 " << chi2 << endl;
         } // if verbose
      } // for sillyLoop
   } // for loop
   if (! gNumWanted)
   {
      newHist->Write();
      oldHist->Write();
   }
   return 0;
}
