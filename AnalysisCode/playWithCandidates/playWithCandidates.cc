// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"

#include "time.h"
#include "CLPClasses/dout.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include "CLPClasses/CLPClasses.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;

// (ugh..) Global Variables
int gWhich         = 0;
int gNumWanted     = 10;
string gSourceName = "";
string gTargetName = "silly2.root";

// forward declarations
void initialize();

int main(int argc, char** argv)
{
   initialize();
   parseArguments (argc, argv);
   cout << "numWanted " << gNumWanted << endl;
   CLP3Vector::setOutput3Mode( CLP3Vector::kPtEtaPhiOutputMode );
   //CLP3Vector::setOutput3Mode( CLP3Vector::kStandardOutputMode);
   CLPCandidate::setOutputType (CLPCandidate::kEverything);
   enum {
      kInput = 0,
      kOutput,
   };

   string basename = "base";
   bool useRunEventList = false;
   if (0 == gWhich) 
   {
      // Do Nothing
   } 
   
   SelectRunEventClass select("runevent.list");



   ////////////////////
   // Initialization //
   ////////////////////   
   if (0 == gSourceName.length())
   {
      assert(0);
   }
   cout << "trying " << gSourceName << endl;

   // skim file
   TFile source   ( gSourceName.c_str() );


   //////////////////////
   // Histograms, etc. //
   //////////////////////

   ////////////////
   // Input Tree //
   ////////////////
   //TTree *treePtr = (TTree*) source.Get("candFCNC");
   TTree *treePtr = (TTree*) source.Get("matchedCand");
   if (! treePtr)
   {
      cerr << "couldn't find candFCNC" << endl;
      exit(0);
   }
   CLPCandidate *candPtr = new CLPCandidate;
   treePtr->SetBranchAddress ("candidate", &candPtr);   

   //////////////////
   // Output Trees //
   //////////////////
   TFile *targetPtr = new TFile ( gTargetName.c_str(), "RECREATE" ); 
   TClonesArray *reconVecTcaPtr =
      new TClonesArray ("TLorentzVector", 6);
   TClonesArray *hepgVecTcaPtr =
      new TClonesArray ("TLorentzVector", 6);
   reconVecTcaPtr->BypassStreamer(true);
   hepgVecTcaPtr->BypassStreamer(true);
   TTree *outputTreePtr = new TTree ("T4Vecs", "Six TLorentz Vectors");
   outputTreePtr->Branch ("recon", &reconVecTcaPtr);
   outputTreePtr->Branch ("hepg",  &hepgVecTcaPtr);

   // Loop Baby
   Int_t nentries = Int_t(treePtr->GetEntries());
   cout << "Possible entries: " << nentries << endl;
   if ((gNumWanted > 0) && (gNumWanted < nentries))
   {
      nentries = gNumWanted;    
   }
   cout << " We are looping over " << nentries << " events." << endl;
   for (Int_t entryIndex = 0; entryIndex < nentries; ++entryIndex) 
   {
      treePtr->GetEntry (entryIndex);
      CLPChargedParticle clpLep0 = candPtr->lepton (0);
      CLPChargedParticle clpLep1 = candPtr->lepton (1);
      if (0)
      {
         cout << "lep0 " << clpLep0 << endl
              << "     " << clpLep0.HEPGMom4Vec() << endl;
         cout << "lep1 " << clpLep1 << endl
              << "     " << clpLep1.HEPGMom4Vec() << endl;
         cout << endl;
      }
      TLorentzVector lep0  = clpLep0.mom4Vec().tlorentz();
      TLorentzVector lep1  = clpLep1.mom4Vec().tlorentz();
      TLorentzVector hepg0 = clpLep0.HEPGMom4Vec().tlorentz();
      TLorentzVector hepg1 = clpLep1.HEPGMom4Vec().tlorentz();
      reconVecTcaPtr->Clear();
      hepgVecTcaPtr->Clear();
      new ((*reconVecTcaPtr)[0]) TLorentzVector (lep0);
      new ((*reconVecTcaPtr)[1]) TLorentzVector (lep1);
      new ((*hepgVecTcaPtr)[0])  TLorentzVector (hepg0);
      new ((*hepgVecTcaPtr)[1])  TLorentzVector (hepg1);
      outputTreePtr->Fill();
   }
   targetPtr->Write();
   return 0;
}

void initialize()
{
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_StringMap["file"]        = (void*) &gSourceName;
   ns_StringMap["target"]      = (void*) &gTargetName;
}

