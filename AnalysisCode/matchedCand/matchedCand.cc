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
int gWhich         = 11;
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
   if (11 == gWhich) 
   {
      // all signal
      sourceName = "/cdf/scratch/cplager/fcnc/events_all_signal_trk_tight.root";
      basename = "fcnc";
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
   // TFile target ( filename.c_str(), "RECREATE");
   // 
   // ///////////////////////////
   // // Transverse Histograms //
   // ///////////////////////////
   // const int kNumTransBins = 10;
   // const double kTransBoundaries[kNumTransBins + 1] = 
   //    {0, 10, 20, 30, 40, 50, 60, 80, 100, 120, 160};
   // TH1F *absL7TransHistArry[kNumTransBins + 1];
   // TH1F *relL7TransHistArry[kNumTransBins + 1];
   // TH1F *momL7TransHistArry[kNumTransBins + 1];
   // TH1F *absL5TransHistArry[kNumTransBins + 1];
   // TH1F *relL5TransHistArry[kNumTransBins + 1];
   // TH1F *momL5TransHistArry[kNumTransBins + 1];
   // for (int loop = 0; loop <= kNumTransBins; ++loop)
   // {
   //    char relL7TransLine[200], absL7TransLine[200], momL7TransLine[200],
   //       relL7TransShort[50], absL7TransShort[50], momL7TransShort[50];
   //    sprintf (relL7TransShort, "relL7Trans%02d", loop);
   //    sprintf (absL7TransShort, "absL7Trans%02d", loop);
   //    sprintf (momL7TransShort, "momL7Trans%02d", loop);
   //    char relL5TransLine[200], absL5TransLine[200], momL5TransLine[200],
   //       relL5TransShort[50], absL5TransShort[50], momL5TransShort[50];
   //    sprintf (relL5TransShort, "relL5Trans%02d", loop);
   //    sprintf (absL5TransShort, "absL5Trans%02d", loop);
   //    sprintf (momL5TransShort, "momL5Trans%02d", loop);
   //    if (kNumTransBins == loop)
   //    {
   //       sprintf (relL7TransLine, 
   //                "Relative Resolution for Recon Jets L7 Et gt %.0f",
   //                kTransBoundaries[kNumTransBins]);
   //       sprintf (absL7TransLine, 
   //                "Absolute Resolution for Recon Jets L7 Et gt %.0f",
   //                kTransBoundaries[kNumTransBins]);
   //       sprintf (momL7TransLine, 
   //                "L7 Et Distribution for Recon Jets gt %.0f",
   //                kTransBoundaries[kNumTransBins]);
   //       sprintf (relL5TransLine, 
   //                "Relative Resolution for Recon Jets L5 Et gt %.0f",
   //                kTransBoundaries[kNumTransBins]);
   //       sprintf (absL5TransLine, 
   //                "Absolute Resolution for Recon Jets L5 Et gt %.0f",
   //                kTransBoundaries[kNumTransBins]);
   //       sprintf (momL5TransLine, 
   //                "L5 Et Distribution for Recon Jets gt %.0f",
   //                kTransBoundaries[kNumTransBins]);
   //    }
   //    else
   //    {
   //       sprintf (relL7TransLine, 
   //                "Relative Resolution for Recon Jets L7 Et[%.0f, %0.f[",
   //                kTransBoundaries[loop], kTransBoundaries[loop + 1]);
   //       sprintf (absL7TransLine, 
   //                "Absolute Resolution for Recon Jets L7 Et[%.0f, %0.f[",
   //                kTransBoundaries[loop], kTransBoundaries[loop + 1]);
   //       sprintf (momL7TransLine, 
   //                "L7 Et Distribution for Recon Jets[%.0f, %0.f[",
   //                kTransBoundaries[loop], kTransBoundaries[loop + 1]);
   //       sprintf (relL5TransLine, 
   //                "Relative Resolution for Recon Jets L5 Et[%.0f, %0.f[",
   //                kTransBoundaries[loop], kTransBoundaries[loop + 1]);
   //       sprintf (absL5TransLine, 
   //                "Absolute Resolution for Recon Jets L5 Et[%.0f, %0.f[",
   //                kTransBoundaries[loop], kTransBoundaries[loop + 1]);
   //       sprintf (momL5TransLine, 
   //                "L5 Et Distribution for Recon Jets[%.0f, %0.f[",
   //                kTransBoundaries[loop], kTransBoundaries[loop + 1]);
   //    } // else if kNumTransBins
   //    absL7TransHistArry[loop] = 
   //       new TH1F (absL7TransShort, absL7TransLine, 100, -100, 100);
   //    relL7TransHistArry[loop] = 
   //       new TH1F (relL7TransShort, relL7TransLine, 100, -2., 2.);
   //    momL7TransHistArry[loop] = 
   //       new TH1F (momL7TransShort, momL7TransLine, 250, 0, 250);
   //    absL5TransHistArry[loop] = 
   //       new TH1F (absL5TransShort, absL5TransLine, 100, -100, 100);
   //    relL5TransHistArry[loop] = 
   //       new TH1F (relL5TransShort, relL5TransLine, 100, -2., 2.);
   //    momL5TransHistArry[loop] = 
   //       new TH1F (momL5TransShort, momL5TransLine, 250, 0, 250);
   // } // for loop
   // 
   // //////////////////////
   // // Total Histograms //
   // //////////////////////
   // const int kNumTotalBins = 17;
   // const double kTotalBoundaries[kNumTotalBins + 1] = 
   //    {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 160, 180, 200};
   // TH1F *absL7TotalHistArry[kNumTotalBins + 1];
   // TH1F *relL7TotalHistArry[kNumTotalBins + 1];
   // TH1F *momL7TotalHistArry[kNumTotalBins + 1];
   // TH1F *absL5TotalHistArry[kNumTotalBins + 1];
   // TH1F *relL5TotalHistArry[kNumTotalBins + 1];
   // TH1F *momL5TotalHistArry[kNumTotalBins + 1];
   // for (int loop = 0; loop <= kNumTotalBins; ++loop)
   // {
   //    char relL7TotalLine[200], absL7TotalLine[200], momL7TotalLine[200],
   //       relL7TotalShort[50], absL7TotalShort[50], momL7TotalShort[50];
   //    sprintf (relL7TotalShort, "relL7Total%02d", loop);
   //    sprintf (absL7TotalShort, "absL7Total%02d", loop);
   //    sprintf (momL7TotalShort, "momL7Total%02d", loop);
   //    char relL5TotalLine[200], absL5TotalLine[200], momL5TotalLine[200],
   //       relL5TotalShort[50], absL5TotalShort[50], momL5TotalShort[50];
   //    sprintf (relL5TotalShort, "relL5Total%02d", loop);
   //    sprintf (absL5TotalShort, "absL5Total%02d", loop);
   //    sprintf (momL5TotalShort, "momL5Total%02d", loop);
   //    if (kNumTotalBins == loop)
   //    {
   //       sprintf (relL7TotalLine, 
   //                "Relative Resolution for Recon Jets L7 Energy gt %.0f",
   //                kTotalBoundaries[kNumTotalBins]);
   //       sprintf (absL7TotalLine, 
   //                "Absolute Resolution for Recon Jets L7 Energy gt %.0f",
   //                kTotalBoundaries[kNumTotalBins]);
   //       sprintf (momL7TotalLine, 
   //                "L7 Energy Distribution for Recon Jets gt %.0f",
   //                kTotalBoundaries[kNumTotalBins]);
   //       sprintf (relL5TotalLine, 
   //                "Relative Resolution for Recon Jets L5 Energy gt %.0f",
   //                kTotalBoundaries[kNumTotalBins]);
   //       sprintf (absL5TotalLine, 
   //                "Absolute Resolution for Recon Jets L5 Energy gt %.0f",
   //                kTotalBoundaries[kNumTotalBins]);
   //       sprintf (momL5TotalLine, 
   //                "L5 Energy Distribution for Recon Jets gt %.0f",
   //                kTotalBoundaries[kNumTotalBins]);
   //    }
   //    else
   //    {
   //       sprintf (relL7TotalLine, 
   //                "Relative Resolution for Recon Jets L7 Energy[%.0f, %0.f[",
   //                kTotalBoundaries[loop], kTotalBoundaries[loop + 1]);
   //       sprintf (absL7TotalLine, 
   //                "Absolute Resolution for Recon Jets L7 Energy[%.0f, %0.f[",
   //                kTotalBoundaries[loop], kTotalBoundaries[loop + 1]);
   //       sprintf (momL7TotalLine, 
   //                "L7 Energy Distribution for Recon Jets[%.0f, %0.f[",
   //                kTotalBoundaries[loop], kTotalBoundaries[loop + 1]);
   //       sprintf (relL5TotalLine, 
   //                "Relative Resolution for Recon Jets L5 Energy[%.0f, %0.f[",
   //                kTotalBoundaries[loop], kTotalBoundaries[loop + 1]);
   //       sprintf (absL5TotalLine, 
   //                "Absolute Resolution for Recon Jets L5 Energy[%.0f, %0.f[",
   //                kTotalBoundaries[loop], kTotalBoundaries[loop + 1]);
   //       sprintf (momL5TotalLine, 
   //                "L5 Energy Distribution for Recon Jets[%.0f, %0.f[",
   //                kTotalBoundaries[loop], kTotalBoundaries[loop + 1]);
   //    } // else if kNumTotalBins
   //    absL7TotalHistArry[loop] = 
   //       new TH1F (absL7TotalShort, absL7TotalLine, 100, -100, 100);
   //    relL7TotalHistArry[loop] = 
   //       new TH1F (relL7TotalShort, relL7TotalLine, 100, -2., 2.);
   //    momL7TotalHistArry[loop] = 
   //       new TH1F (momL7TotalShort, momL7TotalLine, 250, 0, 250);
   //    absL5TotalHistArry[loop] = 
   //       new TH1F (absL5TotalShort, absL5TotalLine, 100, -100, 100);
   //    relL5TotalHistArry[loop] = 
   //       new TH1F (relL5TotalShort, relL5TotalLine, 100, -2., 2.);
   //    momL5TotalHistArry[loop] = 
   //       new TH1F (momL5TotalShort, momL5TotalLine, 250, 0, 250);
   // } // for loop

 
   ////////////////
   // Input Tree //
   ////////////////
   TTree *tree = (TTree*) source.Get("matchedCand");
   
   //TTree *tree = (TTree*) source.Get("candFCNC");
   if (! tree)
   {
      cerr << "couldn't find candFCNC" << endl;
      exit(0);
   }
   CLPCandidate *candPtr = new CLPCandidate;
   tree->SetBranchAddress ("candidate", &candPtr);   
   Int_t nentries = Int_t(tree->GetEntriesFast());
   cout << "Possible entries: " << nentries << endl;
   if ((gNumWanted > 0) && (gNumWanted < nentries))
   {
      nentries = gNumWanted;    
   }
   CLPJet::setDefaultCorrectionType (CLPJet::kLevel5);
   cout << " We are looping over " << nentries << " events." << endl;
   for (Int_t entryIndex = 0; entryIndex < nentries; ++entryIndex) 
   {
      if (entryIndex % 1000 == 0) cout << entryIndex << endl;
      tree->GetEntry(entryIndex);
      double tWb_massSym = candPtr->matched_tWb_mass (3);
      double tWb_massAsym = candPtr->matched_tWb_mass (4);
      cout << "symmetric " << tWb_massSym << " asymmetric "  
           << tWb_massAsym << endl;

      // //cout << endl << endl << endl << endl << *candPtr << endl;
      // int numJets = candPtr->numJets();
      // for (int jIndex = 0; jIndex < numJets; ++jIndex)
      // {
      //    if (! candPtr->jet(jIndex).isTagged())
      //    {
      //       continue;
      //    }
      //    CLP3Vector reconL7_4vec = 
      //       candPtr->jet(jIndex).mom3Vec( CLPJet::kLevel7 );
      //    CLP3Vector reconL5_4vec = 
      //       candPtr->jet(jIndex).mom3Vec( CLPJet::kLevel5 );
      //    CLP4Vector hepg_4vec = candPtr->jet(jIndex).HEPGMom4Vec();   
      //    int bin; // we are going to use this four times        
      //    // Transverse Histograms         
      //    double hepgTrans   = hepg_4vec.transMag();
      //    // Level 7
      //    double reconL7Trans = reconL7_4vec.transMag();
      //    for (bin = 0; bin < kNumTransBins; ++bin)
      //    {
      //       if (reconL7Trans < kTransBoundaries[bin + 1])
      //       {
      //          break;
      //       }
      //    }
      //    assert ((bin >= 0) && (bin <= kNumTransBins));
      //    absL7TransHistArry[bin]->Fill (reconL7Trans - hepgTrans);
      //    relL7TransHistArry[bin]->Fill ( (reconL7Trans - hepgTrans) / 
      //                                    reconL7Trans );
      //    momL7TransHistArry[bin]->Fill (reconL7Trans);
      //    // Level 5
      //    double reconL5Trans = reconL5_4vec.transMag();
      //    for (bin = 0; bin < kNumTransBins; ++bin)
      //    {
      //       if (reconL5Trans < kTransBoundaries[bin + 1])
      //       {
      //          break;
      //       }
      //    }
      //    assert ((bin >= 0) && (bin <= kNumTransBins));
      //    absL5TransHistArry[bin]->Fill (reconL5Trans - hepgTrans);
      //    relL5TransHistArry[bin]->Fill ( (reconL5Trans - hepgTrans) / 
      //                                    reconL5Trans );
      //    momL5TransHistArry[bin]->Fill (reconL5Trans);
      //    // Total Histograms
      //    double hepgTotal   = hepg_4vec.mag();
      //    // Level 7
      //    double reconL7Total = reconL7_4vec.mag();
      //    for (bin = 0; bin < kNumTotalBins; ++bin)
      //    {
      //       if (reconL7Total < kTotalBoundaries[bin + 1])
      //       {
      //          break;
      //       }
      //    }
      //    assert ((bin >= 0) && (bin <= kNumTotalBins));
      //    absL7TotalHistArry[bin]->Fill (reconL7Total - hepgTotal);
      //    relL7TotalHistArry[bin]->Fill ( (reconL7Total - hepgTotal) / 
      //                                    reconL7Total );
      //    momL7TotalHistArry[bin]->Fill (reconL7Total);
      //    // Level 5
      //    double reconL5Total = reconL5_4vec.mag();
      //    for (bin = 0; bin < kNumTotalBins; ++bin)
      //    {
      //       if (reconL5Total < kTotalBoundaries[bin + 1])
      //       {
      //          break;
      //       }
      //    }
      //    assert ((bin >= 0) && (bin <= kNumTotalBins));
      //    absL5TotalHistArry[bin]->Fill (reconL5Total - hepgTotal);
      //    relL5TotalHistArry[bin]->Fill ( (reconL5Total - hepgTotal) / 
      //                                    reconL5Total );
      //    momL5TotalHistArry[bin]->Fill (reconL5Total);
      // } // for jIndex
   }
   // target.Write();
   return 0;
}

void initialize()
{
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
   ns_IntMap["howmany"]        = (void*) &gNumWanted;
}

