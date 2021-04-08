// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stdlib.h>

#include <KinematicFitter/KinematicFitter.hh>

using namespace std;

ostream& operator<<( ostream& os , const TLorentzVector& p )
{
//   os << "px=" << setw(4) << p.Px()
//        << "py=" << setw(4) << p.Py()
//        << "pz=" << setw(4) << p.Pz()
//        << "en=" << setw(4) << p.E();
  int pre = os.precision();
  os.precision(4);
  os << "(" << setw(4) << p.E()
     << ": " << setw(4) << p.Px()
     << ", " << setw(4) << p.Py()
     << ", " << setw(4) << p.Pz()
     << ")";
  return os;
}

int main(int argc, char** argv)
{
   ////////////////////
   // Open Root File //
   ////////////////////
   TFile *filePtr = TFile::Open("six_4vectors.root");

   TTree *treePtr = (TTree*) filePtr->Get ("T4Vecs");
   TClonesArray *reconTcaPtr = new TClonesArray ("TLorentzVector", 6);
   TClonesArray *hepgTcaPtr  = new TClonesArray ("TLorentzVector", 6);
   treePtr->GetBranch("recon")->SetAutoDelete(false);
   treePtr->SetBranchAddress("recon", &reconTcaPtr);
   treePtr->GetBranch("hepg")->SetAutoDelete(false);
   treePtr->SetBranchAddress("hepg", &hepgTcaPtr);
   Int_t nentries = Int_t(treePtr->GetEntries());
   cout << "Possible entries: " << nentries << endl;

   for (int loop = 0; loop < nentries; ++loop)
   {
     if( loop > 5 ) { break; }
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
      TLorentzVector w1hepg( w1d1hepg + w1d2hepg );
      TLorentzVector w2hepg( w2d1hepg + w2d2hepg );
      TLorentzVector top1hepg( b1hepg+w1hepg );
      TLorentzVector top2hepg( b2hepg+w2hepg );

      // do kinematic fit to reconstructed particles

      KinematicFitter::FourVectorColl fours;
      fours.push_back( b1vec );
      fours.push_back( w1d1vec );
      fours.push_back( w1d2vec );
      fours.push_back( b2vec );
      fours.push_back( w2d1vec );
      fours.push_back( w2d2vec );
      KinematicFitter fitter( fours );
      vector<double> sigmaE;
      sigmaE.push_back( 0.15 );
      sigmaE.push_back( 0.15 );
      sigmaE.push_back( 0.15 );
      sigmaE.push_back( 0.15 );
      sigmaE.push_back( 0.15 );
      sigmaE.push_back( 0.15 );
      fitter.useEnergyCovariance(  sigmaE  );
      set<int> iw1,iw2,it1,it2;
      iw1.insert( 1 );
      iw1.insert( 2 );
      iw2.insert( 4 );
      iw2.insert( 5 );
      it1.insert( 0 );
      it1.insert( 1 );
      it1.insert( 2 );
      it2.insert( 3 );
      it2.insert( 4 );
      it2.insert( 5 );
      fitter.addMassConstraint( iw1 , 80.425 );
      fitter.addMassConstraint( iw2 , 80.425 );
      fitter.addEqualMassConstraint( it1 , it2 );
      fitter.fit();

      assert( fitter.fitOk() == KinematicFitter::OK );

      TLorentzVector b1c,b2c,w1d1c,w1d2c,w2d1c,w2d2c;
      fitter.fitVector( 0 , b1c );
      fitter.fitVector( 1 , w1d1c );
      fitter.fitVector( 2 , w1d2c );
      fitter.fitVector( 3 , b2c );
      fitter.fitVector( 4 , w2d1c );
      fitter.fitVector( 5 , w2d2c );

      TLorentzVector w1r( w1d1vec +w1d2vec );
      TLorentzVector w2r( w2d1vec +w2d2vec );
      TLorentzVector t1r( w1r + b1vec );
      TLorentzVector t2r( w2r + b2vec );
      TLorentzVector w1c( w1d1c + w1d2c );
      TLorentzVector w2c( w2d1c + w2d2c );
      TLorentzVector t1c( w1c + b1c );
      TLorentzVector t2c( w2c + b2c );
      

      if( true )
      {
         // before
         cout << endl 
              << setw(20) <<  "Before:" 
              << setw(40) <<  "After:" 
              << setw(40) <<  "HEPG:" 
              << endl;
         cout << "b1      " 
              << setw(4) << " " << b1vec
              << setw(4) << " " << b1c
              << setw(4) << " " << b1hepg << endl;
         cout << "w1d1    " 
              << setw(4) << " " << w1d1vec
              << setw(4) << " " << w1d1c
              << setw(4) << " " << w1d1hepg << endl;
         cout << "w1d2    " 
              << setw(4) << " " << w1d2vec
              << setw(4) << " " << w1d2c
              << setw(4) << " " << w1d2hepg << endl;
         cout << "W1      " << w1r
              << setw(4) << " " << w1c
              << setw(4) << " " << w1hepg << endl;
         cout << "top1    " << t1r
              << setw(4) << " " << t1c
              << setw(4) << " " << top1hepg << endl; 
         cout << "W Mass 1" 
              << setw(14) << " " << w1r.M()
              << setw(32) << " " << w1c.M()
              << setw(32) << " " << w1hepg.M() 
              << endl;
         cout << "Top Mass 1" 
              << setw(12) << " " << t1r.M()
              << setw(32) << " " << t1c.M()
              << setw(32) << " " << top1hepg.M() 
              << endl;
         cout << "b2      " 
              << setw(4) << " " << b2vec
              << setw(4) << " " << b2c
              << setw(4) << " " << b2hepg << endl;
         cout << "w2d1    " 
              << setw(4) << " " << w2d1vec
              << setw(4) << " " << w2d1c
              << setw(4) << " " << w2d1hepg << endl;
         cout << "w2d2    " 
              << setw(4) << " " << w2d2vec
              << setw(4) << " " << w2d2c
              << setw(4) << " " << w2d2hepg << endl;
         cout << "W2      " << w2r
              << setw(4) << " " << w2c
              << setw(4) << " " << w2hepg << endl;
         cout << "top2    " << t2r
              << setw(4) << " " << t2c
              << setw(4) << " " << top2hepg << endl;
         cout << "W Mass 2" 
              << setw(14)  << " " << w2r.M()
              << setw(32) << " " << w2c.M()
              << setw(32) << " " << w2hepg.M() 
              << endl;
         cout << "Top Mass 2" 
              << setw(12) << " " << t2r.M()
              << setw(32) << " " << t2c.M()
              << setw(32) << " " << top2hepg.M() 
              << endl;
         cout << "Top Mass Average " 
              << setw(5)  << " " << (t1r.M()+t2r.M())/2.
              << setw(32) << " " << (t1c.M()+t2c.M())/2.
              << setw(32) << " " << (top1hepg.M()+top2hepg.M())/2.
              << endl;
         cout << setw(65) << "chi^2 " << fitter.chi2()
              << setw(23) 
              << endl;
           //              << "  ns1 " << numsig1 << "  ns2 " << numsig2 << endl;
      } // if verbose
   } // for loop
   return 0;
}
