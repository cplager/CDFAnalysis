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
string gFilename   = "six_new_4vectors.root";
// target file name (target)
string gTarget     = "output.root";
// verbose mode (verbose)
bool gVerbose      = true;
// constrain first W only (wonly)
bool gFirstWonly   = false;
// use 15% relative uncertainty and no angles (use15)
bool gUse15 = true;
// use M_P basis (mass)
bool gUseMassBasis = false;
// use double mass constrinat (double)
int gMassConstraintMode    = 1;
// which CLP mode to use (clp)
int gCLPmode       = 1;
// number of iterations (iter)
int gNumIterations = 0;
// use fast covar mode (covar)
int gCovarMode  = 1;
// silly loop (silly)
int gSilly = 1;
// CLP scale everything (everything)
bool gScaleEverything = false;
// Kinematic fitter massive jets (massive)
bool gUseMassiveJets  = false;

void marcel();
void initializeOptionsMap();

int main(int argc, char** argv)
{
   initializeOptionsMap();
   parseArguments (argc, argv);
   cout << "covarMode " << gCovarMode << endl;
   ////////////////////
   // Open Root File //
   ////////////////////
   CLP4Vector::setScaleEverything (gScaleEverything);
   KinematicFitter::ParticleType type;
   if (gUseMassiveJets)
   {
      type = KinematicFitter::kMassiveJet;
   } else {
      type = KinematicFitter::kNormalParticle;
   }
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
   double widthArray[10] = {};
   treePtr->GetBranch("recon")->SetAutoDelete(false);
   treePtr->SetBranchAddress("recon", &reconTcaPtr);
   treePtr->GetBranch("hepg")->SetAutoDelete(false);
   treePtr->SetBranchAddress("hepg", &hepgTcaPtr);
   treePtr->SetBranchAddress("width", widthArray);
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
   TH1F *clpHist 
      = new TH1F ("clp", "CLP Method Average Top Mass", 40, 100, 300);
   TH1F *newHist 
      = new TH1F ("kmf", "KMF Method Average Top Mass", 40, 100, 300);
   TH1F *wTtaHist 
      = new TH1F ("wTta", "W daughter delta theta", 50, -0.4, 0.4);
   TH1F *wPhiHist 
      = new TH1F ("wPhi", "W daughter delta phi", 50, -0.4, 0.4);
   TH1F *bTtaHist 
      = new TH1F ("bTta", "B jet delta theta", 50, -0.4, 0.4);
   TH1F *bPhiHist 
      = new TH1F ("bPhi", "B jetdelta phi", 50, -0.4, 0.4);
   
   cout << "gMassConstraintMode " << gMassConstraintMode << endl;

   KinematicFitter fitter;
   if (gNumIterations > 0)
   {
      fitter.setNumIterations (gNumIterations);
   }
   if (gUseMassBasis)
   {
      fitter.changeBasis (KinematicFitter::kM_P_basis);
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

      // cout << "Width";
      // for (int inner = 0; inner < 6; ++inner)
      // {
      //    cout << "  " << widthArray[inner];
      // } // for inner
      // cout << endl;
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

      vector< TLorentzVector > bVecs, bHepgs, wVecs, wHepgs;
      // B vectors
      bVecs.push_back (b1vec);
      bVecs.push_back (b2vec);
      bHepgs.push_back (b1hepg);
      bHepgs.push_back (b2hepg);
      wVecs.push_back (w1d1vec);
      wVecs.push_back (w1d2vec);
      wVecs.push_back (w2d1vec);
      wVecs.push_back (w2d2vec);
      wHepgs.push_back (w1d1hepg);
      wHepgs.push_back (w1d2hepg);
      wHepgs.push_back (w2d1hepg);
      wHepgs.push_back (w2d2hepg);

      for (unsigned int loop = 0; loop < bVecs.size(); ++loop)
      {
         TLorentzVector &vec  = bVecs[loop];
         TLorentzVector &hepg = bHepgs[loop];
         double deltaPhi = vec.DeltaPhi (hepg);
         double tta1 = vec.Theta();
         double tta2 = hepg.Theta();
         bTtaHist->Fill (tta1-tta2);
         bPhiHist->Fill (deltaPhi);
      }
      for (unsigned int loop = 0; loop < wVecs.size(); ++loop)
      {
         TLorentzVector &vec  = wVecs[loop];
         TLorentzVector &hepg = wHepgs[loop];
         double deltaPhi = vec.DeltaPhi (hepg);
         double tta1 = vec.Theta();
         double tta2 = hepg.Theta();
         wTtaHist->Fill (tta1-tta2);
         wPhiHist->Fill (deltaPhi);
      }

      // Errors on the vectors
      double dE_b1   = 0.15 * b1vec.E();
      double dE_w1d1 = 0.15 * w1d1vec.E();
      double dE_w1d2 = 0.15 * w1d2vec.E();
      double dE_b2   = 0.15 * b2vec.E();
      double dE_w2d1 = 0.15 * w2d1vec.E();
      double dE_w2d2 = 0.15 * w2d2vec.E();

      if (! gUse15)
      {
         dE_b1   = widthArray[0] * b1vec.E();
         dE_w1d1 = widthArray[1] * w1d1vec.E();
         dE_w1d2 = widthArray[2] * w1d2vec.E();
         dE_b2   = widthArray[3] * b2vec.E();
         dE_w2d1 = widthArray[4] * w2d1vec.E();
         dE_w2d2 = widthArray[5] * w2d2vec.E();
      }
      
      for (int sillyLoop = 0; sillyLoop < gSilly; ++sillyLoop)
      {
         /////////////////////////////////////
         // Setup the Kinematic Mass Fitter //
         /////////////////////////////////////
         fitter.clearAllInputParticles();
         // Add input particles
         int w1d1, w1d2, b1, w2d1, w2d2, b2;
         b1   = fitter.addInputParticle (b1vec,   type, dE_b1  ); //, 0.03, 0.05);
         w2d1 = fitter.addInputParticle (w2d1vec, type, dE_w2d1); //, 0.03, 0.05);
         w2d2 = fitter.addInputParticle (w2d2vec, type, dE_w2d2); //, 0.03, 0.05);
         b2   = fitter.addInputParticle (b2vec,   type, dE_b2  ); //, 0.03, 0.05);
         w1d1 = fitter.addInputParticle (w1d1vec, type, dE_w1d1); //, 0.03, 0.05);
         w1d2 = fitter.addInputParticle (w1d2vec, type, dE_w1d2); //, 0.03, 0.05);
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
         w1aftervec    = modVec [W1c];
         double ns1_kf = 
            (fitter.E(w1d1, KinematicFitter::kModified) -
             fitter.E(w1d1, KinematicFitter::kOriginal))
            / dE_w1d1;
         double ns2_kf = 
            (fitter.E(w1d2, KinematicFitter::kModified) -
             fitter.E(w1d2, KinematicFitter::kOriginal))
            / dE_w1d2;
         if (gVerbose)
         {
            cout << "width1 / width2 " << widthArray[1] / widthArray[2] << endl;
            cout << "ns1_kf " << ns1_kf << " ns2_kf " << ns2_kf 
                 << " chi2 " << ns1_kf * ns1_kf + ns2_kf * ns2_kf
                 << " rat " << ns1_kf / ns2_kf
                 << " rat2 " << ns1_kf * ns1_kf / (ns2_kf * ns2_kf)
                 << endl;
         }
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
         // CLP
         double numsig1 = 0, numsig2 = 0;
         // W1
         CLP4Vector w1d1clp (w1d1vec);
         CLP4Vector w1d2clp (w1d2vec);
         CLP4Vector b1clp   (b1vec);
         // W2
         CLP4Vector w2d1clp (w2d1vec);
         CLP4Vector w2d2clp (w2d2vec);
         CLP4Vector b2clp   (b2vec);
         if (1 == gCLPmode)
         {
            numsig1 = fix4VectorsForMass (w1d1clp, w1d2clp, 80.425,
                                          dE_w1d1, dE_w1d2);
            numsig2 = fix4VectorsForMass (w2d1clp, w2d2clp, 80.425,
                                          dE_w2d1, dE_w2d2);
            //if (! loop ) cout << "CLP proper" << endl;
         } 
         CLP4Vector w1clp   = w1d1clp + w1d2clp;
         CLP4Vector w2clp = w2d1clp + w2d2clp;
         if (2 == gCLPmode)
         {
            w1clp.setRestMass (80.45);
            w2clp.setRestMass (80.45);
            //if (! loop ) cout << "CLP energy only" << endl;
         }
         CLP4Vector top1clp = w1clp + b1clp;
         CLP4Vector top2clp = w2clp + b2clp;
         double clpTopAve = ( top1clp.mass() + top2clp.mass() ) / 2.;      

         // HEPG
         TLorentzVector w1hepg   = w1d1hepg + w1d2hepg;
         TLorentzVector top1hepg = b1hepg   + w1hepg;
         TLorentzVector w2hepg   = w2d1hepg + w2d2hepg;
         TLorentzVector top2hepg = b2hepg   + w2hepg;
         double hepgTopAve = ( top1hepg.M() + top2hepg.M() ) / 2.;

         clpHist->Fill ( clpTopAve);
         newHist->Fill ( afterTopAve );
         oldHist->Fill ( beforeTopAve ); 

         if (gVerbose)
         {
            // before
            cout << endl 
                 << setw(20) <<  "Before:" 
                 << setw(40) <<  "After:" 
                 << setw(40) <<  "CLP:" 
                 << setw(40) <<  "HEPG:" 
                 << endl;
            if (! gFirstWonly) 
            {
               cout << "b1      " 
                    << fitter.particleMom (b1, KinematicFitter::kOriginal)
                    << setw(4) << " " << fitter.particleMom (b1)
                    << setw(4) << " " << b1clp 
                    << setw(4) << " " << b1hepg << endl;
            }
            cout << "W1d1    " 
                 << fitter.particleMom (w1d1, KinematicFitter::kOriginal)
                 << setw(4) << " " << fitter.particleMom (w1d1)
                 << setw(4) << " " << w1d1clp 
                 << setw(4) << " " << w1d1hepg << endl;
            cout << "W1d2    " 
                 << fitter.particleMom (w1d2, KinematicFitter::kOriginal)
                 << setw(4) << " " << fitter.particleMom (w1d2)
                 << setw(4) << " " << w1d2clp 
                 << setw(4) << " " << w1d2hepg << endl;
            cout << "W1      " << w1beforevec 
                 << setw(4) << " " << w1aftervec 
                 << setw(4) << " " << w1clp 
                 << setw(4) << " " << w1hepg << endl;
            if (! gFirstWonly) cout << "top1    " << top1beforevec 
                                    << setw(4) << " " << top1aftervec 
                                    << setw(4) << " " << top1clp 
                                    << setw(4) << " " << top1hepg << endl; 
            cout << "W Mass 1" 
                 << setw(15) << " " << w1beforevec.M()
                 << setw(32) << " " << w1aftervec.M()
                 << setw(32) << " " << w1clp.mass() 
                 << setw(32) << " " << w1hepg.M() 
                 << endl;
            
            if (! gFirstWonly)
            {
               cout << "Top Mass 1" 
                    << setw(12) << " " << top1beforevec.M()
                    << setw(32) << " " << top1aftervec.M()
                    << setw(32) << " " << top1clp.mass() 
                    << setw(32) << " " << top1hepg.M() 
                    << endl;

               cout << "b2      " 
                    << fitter.particleMom (b2, KinematicFitter::kOriginal)
                    << setw(4) << " " << fitter.particleMom (b2)
                    << setw(4) << " " << b2clp 
                    << setw(4) << " " << b2hepg << endl;
               cout << "W2d1    " 
                    << fitter.particleMom (w2d1, KinematicFitter::kOriginal)
                    << setw(4) << " " << fitter.particleMom (w2d1)
                    << setw(4) << " " << w2d1clp 
                    << setw(4) << " " << w2d1hepg << endl;
               cout << "W2d2    " 
                    << fitter.particleMom (w2d2, KinematicFitter::kOriginal)
                    << setw(4) << " " << fitter.particleMom (w2d2)
                    << setw(4) << " " << w2d2clp 
                    << setw(4) << " " << w2d2hepg << endl;
               cout << "W2      " << w2beforevec 
                    << setw(4) << " " << w2aftervec 
                    << setw(4) << " " << w2clp 
                    << setw(4) << " " << w2hepg << endl;
               cout << "top2    " << top2beforevec 
                    << setw(4) << " " << top2aftervec 
                    << setw(4) << " " << top2clp 
                    << setw(4) << " " << top2hepg << endl;
               cout << "W Mass 2" 
                    << setw(15)  << " " << w2beforevec.M()
                    << setw(32) << " " << w2aftervec.M()
                    << setw(32) << " " << w2clp.mass() 
                    << setw(32) << " " << w2hepg.M() 
                    << endl;
               cout << "Top Mass 2" 
                    << setw(12) << " " << top2beforevec.M()
                    << setw(32) << " " << top2aftervec.M()
                    << setw(32) << " " << top2clp.mass() 
                    << setw(32) << " " << top2hepg.M() 
                    << endl;
               cout << "Top Mass Average " 
                    << setw(5)  << " " << beforeTopAve
                    << setw(32) << " " << afterTopAve
                    << setw(32) << " " << clpTopAve
                    << setw(32) << " " << hepgTopAve
                    << endl;
            } // if ! firstOnly
            cout << setw(65) << "chi^2 " << chi2 
                 << setw(23);
            if (gFirstWonly) 
            {
               cout << " chi2 " << 2 * numsig1 * numsig1 
                    << "  ns1 " << numsig1 << endl;
            } else {
               cout << " chi2 "
                    << 2 * (numsig1 * numsig1 + numsig2 * numsig2) 
                    << "  ns1 " << numsig1 << endl
                    << "  ns2 " << numsig2 << endl;
            }
         } // if verbose
      } // for sillyLoop
   } // for loop
   if (! gNumWanted)
   {
      clpHist->Write();
      newHist->Write();
      oldHist->Write();
      bTtaHist->Write();
      bPhiHist->Write();
      wTtaHist->Write();
      wPhiHist->Write();
   }
   return 0;
}

void initializeOptionsMap()
{
   // Int maps
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_IntMap["num"]            = (void*) &gNumWanted;
   ns_IntMap["howmany"]        = (void*) &gNumWanted;
   ns_IntMap["clp"]            = (void*) &gCLPmode;
   ns_IntMap["const"]          = (void*) &gMassConstraintMode;
   ns_IntMap["double"]         = (void*) &gMassConstraintMode;
   ns_IntMap["iter"]           = (void*) &gNumIterations;
   ns_IntMap["silly"]          = (void*) &gSilly;
   ns_IntMap["covar"]          = (void*) &gCovarMode;
   // String maps
   ns_StringMap["file"]        = (void*) &gFilename;
   ns_StringMap["target"]      = (void*) &gTarget;
   // Bool maps
   ns_BoolMap["verbose"]       = (void*) &gVerbose;
   ns_BoolMap["wonly"]         = (void*) &gFirstWonly;
   ns_BoolMap["use15"]         = (void*) &gUse15;
   ns_BoolMap["mass"]          = (void*) &gUseMassBasis;
   ns_BoolMap["everything"]    = (void*) &gScaleEverything;
   ns_BoolMap["massive"]       = (void*) &gUseMassiveJets;
}
