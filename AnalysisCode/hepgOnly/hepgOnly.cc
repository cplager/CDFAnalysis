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
int gWhich         = 51;
int gNumWanted     = 10;

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

   string sourceName;   
   string basename = "base";
   bool useRunEventList = false;
   if (51 == gWhich) 
   {
      // flat
      sourceName = "/cdf/scratch/cplager/fcnc/events_all_flat_smttbar_hepg.root";
      basename = "flathists";
   } 
   else 
   {
      cout << "invalid value of which " << gWhich << endl;
      exit(0);
   }
   
   SelectRunEventClass select("runevent.list");



   ////////////////////
   // Initialization //
   ////////////////////   
   if (0 == sourceName.length())
   {
      assert(0);
   }
   cout << "trying " << sourceName << endl;

   // skim file
   TFile source   ( sourceName.c_str() );
   string filename = basename + ".root";
   TFile target ( filename.c_str(), "RECREATE");

   TH1F costhetahist ("costheta", "costheta", 20, -1, 1);
   TH1F unscaledpt ("unscaledpt", "unscaledpt", 50, 0, 200);
   TH1F longpt ("longpt",   "Lepton Transverse Momentum", 50, 0, 200);
   TH1F leftpt ("leftpt",   "leftpt", 50, 0, 200);
   TH1F rightpt ("rightpt", "Lepton Transverse Momentum", 50, 0, 200); 
   TH1F handpt ("handpt", "Lepton Transverse Momentum", 50, 0, 200);
   TH1F smpt ("smpt", "Lepton Transverse Momentum", 50, 0, 200);
  

   ////////////////
   // Input Tree //
   ////////////////
   TTree *tree = (TTree*) source.Get("matched");
   if (! tree)
   {
      cerr << "couldn't find candFCNC" << endl;
      exit(0);
   }
   CLPHEPGCont *contPtr = new CLPHEPGCont;
   tree->SetBranchAddress ("cont", &contPtr);   
   Int_t nentries = Int_t(tree->GetEntriesFast());
   cout << "Possible entries: " << nentries << endl;
   if ((gNumWanted > 0) && (gNumWanted < nentries))
   {
      nentries = gNumWanted;    
   }
   cout << " We are looping over " << nentries << " events." << endl;
   for (Int_t entryIndex = 0; entryIndex < nentries; ++entryIndex) 
   {
      if (entryIndex % 200 == 0) cout << entryIndex << endl;
      tree->GetEntry(entryIndex);
      CLPHEPGObject top    = contPtr->object(1);
      CLPHEPGObject boson  = contPtr->object(2);
      CLPHEPGObject lepton = contPtr->object(4);
      CLP4Vector top4vec    = top.HEPGMom4Vec();
      CLP4Vector boson4vec  = boson.HEPGMom4Vec();
      CLP4Vector lepton4vec = lepton.HEPGMom4Vec();
      CLP3Vector boostvec = boson4vec.boostVector() * -1.;
      // get lepton pt BEFORE boost. :-)
      double leptonpt = lepton4vec.pt();
      top4vec.boost(boostvec);   
      boson4vec.boost(boostvec); // for checking only
      lepton4vec.boost(boostvec);      
      // don't forget the strange -1
      double costheta = -1. * (top4vec.unit() * lepton4vec.unit());
      costhetahist.Fill (costheta);
      double longweight = 3. / 4 * (1. - sq(costheta));
      // cout << "longweight " << longweight << " " << 3/4 * (1 - sq(costheta))
      //      << endl;
      double rightweight = 3. / 8 * sq (1. + costheta);
      double leftweight = 3. / 8 * sq (1. - costheta);
      unscaledpt.Fill (leptonpt);
      longpt.Fill (leptonpt, longweight);
      leftpt.Fill (leptonpt, leftweight);
      rightpt.Fill (leptonpt, rightweight);
      handpt.Fill (leptonpt, (leftweight + rightweight) / 2);
      smpt.Fill (leptonpt, 0.7 * longweight + 0.3 * leftweight);
      // cout << "t " << top    << "  "<< top4vec    << endl
      //      << "W " << boson  << "  "<< boson4vec  << endl
      //      << "l " << lepton << "  "<< lepton4vec << endl         
      //      << "long " << longweight << " rw " << rightweight
      //      << " lw " << leftweight << endl
      //      << " ct " << costheta << " sq " << sq(costheta) << endl; 
      // cout << endl;
      // //cout << *contPtr << endl;
   }
   target.Write();
   return 0;
}

void initialize()
{
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_IntMap["howmany"]        = (void*) &gNumWanted;
}
