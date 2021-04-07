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
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "CLPClasses/CLPClasses.h"
#include "CLPClasses/CombEngine.h"

using namespace CLPFunctions;
using namespace std;

typedef vector< int > IVec;
typedef vector< CLP4Vector > VVec;

IVec best_t_Wb (const VVec &fourVec, bool firstIsB = false)
{
   // first, calculate the number of possibilities
   int from = (int) fourVec.size();
   int choose = 4;
   int offset = 0;
   if (firstIsB)
   {
      // if the first is a b jet, we only want to cycle 
      // through the rest of the jets
      choose = 3;
      offset = 1;
      --from;
   } // first is a b jet
   //dout << endl << "from " << from << " choose " << choose << endl;
   CombEngine comb (from, choose);
   comb.setStartAt (offset);
   //dout << endl;
   int bestIndex = -1;
   double bestdiff = -1;
   int numComb = comb.numCombinations();
   CLP4Vector bjet = fourVec[ 0 ];      
   for (int loop = 0; loop < numComb; ++loop)
   {
      CombEngine::IVec vec = comb.combination (loop);
      // if the first jet isn't always a b jet,
      // we should assign it
      if (! firstIsB )
      {
         bjet = fourVec[ vec[0] ];
      }
      // lets get the others
      CLP4Vector w1jet    = fourVec[ vec[ 1 - offset ] ];
      CLP4Vector w2jet    = fourVec[ vec[ 2 - offset ] ];
      //CLP4Vector charmjet = fourVec[ vec[ 3 - offset ] ];
      // should we bother looking
      if (w1jet.Et() < w2jet.Et())
      {
         // don't bother going any further
         continue;
      }
      CLP4Vector w = w1jet + w2jet;
      double wmass = w.mass();
      w.setRestMass (CLPNamespace::kWmass);
      CLP4Vector top = w + bjet;
      double topmass = top.mass();
      double diff = addInQuad ( topmass - 175, 
                                wmass - CLPNamespace::kWmass);
      if ( (bestdiff < 0) || (diff < bestdiff) )
      {
         bestdiff = diff;
         bestIndex = loop;
      }
   } // for loop
   IVec retval;
   CombEngine::IVec vec = comb.combination (bestIndex);
   if (firstIsB)
   {
      retval.push_back (0);
   }
   else
   {
      retval.push_back( vec[0] );
   }
   for (int loop = 1; loop <= 3; ++loop)
   {
      retval.push_back( vec[ loop - offset ] );
   }
   return retval;
} 

int main(int argc, char** argv)
{
   //CLP3Vector::setOutput3Mode( CLP3Vector::kStandardOutputMode);
   CLP3Vector::setOutput3Mode( CLP3Vector::kPtEtaPhiOutputMode);
   int which = 1;
   string sourceName;
   if (1 == which) 
   {
      sourceName = "/cdf/scratch/cplager/fcnc/events_ee_nt5.root";
   } 
   else 
   {
      
   }

   int numWanted = 10;
   if (argc >= 2)
   {
      numWanted = atol (argv[1]);
   }
   cout << "numWanted " << numWanted << endl;

   ////////////////////
   // 
   //bool debug = false;
   if (0 == sourceName.length())
   {
      sourceName = "/cdf/scratch/cplager/fcnc/skim.root";
   }
   cout << "From: " << sourceName << endl;

   // skim file
   TFile file   ( sourceName.c_str() );


   ////////////////////////
   // ////////////////// //
   // // Matched Tree // //
   // ////////////////// //
   ////////////////////////

   // setup my variables
   // TClones stuff
   CLPHEPGCont *contPtr = new CLPHEPGCont; // defined above
   // counters
   Int_t runNum;    
   Int_t eventNum;  
   // find the tree
   TTree *tree = (TTree*) file.Get("matched");
   if (0 == tree)
   {
      // something bad has happened
      cerr << "Can't get matched.  Sorry, eh." << endl;
      return 0;
   }
   // start hooking things up
   // tclones
   // hepg container
   tree->SetBranchAddress ("cont", &contPtr);
   // ints
   tree->SetBranchAddress("runNum",   &runNum);
   tree->SetBranchAddress("eventNum", &eventNum);

   Int_t nentries = Int_t(tree->GetEntriesFast());
   cout << "Possible entries: " << nentries << endl;
   if ((numWanted > 0) && (numWanted < nentries))
   {
      nentries = numWanted;    
   }
   TH1F zmassHist   ("zmass",    "Z mass", 25, 50, 125);
   TH1F wmassHist   ("wmass",    "W mass", 50, 0, 150);
   TH1F tzcmassHist ("tzcmass",  "top to Z charm mass", 50, 100, 250);
   TH1F twbmassHist ("twbmass1", "top to W b mass", 50, 100, 250);
   TH1F tzcmass2Hist ("tzcmass2", "top to Z (fixed) charm mass", 50, 100, 250);
   TH1F twbmass2Hist ("twbmass2", "top to W (fixed)  b mass", 50, 100, 250);
   TH1F zmassHepgHist   ("zmasshepg",    "HEPG Z mass", 25, 50, 125);
   TH1F wmassHepgHist   ("wmasshepg",    "HEPG W mass", 50, 0, 150);
   TH1F tzcmassHepgHist ("tzcmasshepg",  "HEPG top to Z charm mass", 
                         50, 100, 250);
   TH1F twbmassHepgHist ("twbmass1hepg", "HEPG top to W b mass", 50, 100, 250);
   TH1F charmEtHist ("charmEtHist", "Charm Et Distribution", 100, 0, 400);
   TH1F w1EtHist ("w1EtHist", "W1 Et Distribution", 100, 0, 400);
   TH1F w2EtHist ("w2EtHist", "W2 Et Distribution", 100, 0, 400);
   TH1F charmw1diffHist ("cw1diff", "Charm - W1 Et Difference", 
                         200, -200, 400);
   TH1F charmw2diffHist ("cw2diff", "Charm - W2 Et Difference", 
                         200, -200, 400);
   TH1F wheresCharm ("wheresCharm", "Charm quark location", 4, -0.5, 3.5);
   TH1F wheresBtagCharm ("wheresCharmBtag", "Charm quark location (using B Tag)", 
                     4, -0.5, 3.5);
   cout << " We are looping over " << nentries << " events." << endl;
   int greater1Num = 0, lesser1Num = 0;
   int greater2Num = 0, lesser2Num = 0;
   for (Int_t entryIndex=0; entryIndex < nentries; ++entryIndex) 
   {
      // check run
      tree->GetEntry(entryIndex);
      if ( !contPtr->topFCNCdecayOk_2l() )
      {
         continue;
      }
      CLP4Vector lep1   = contPtr->object(CLPHEPGCont::kLepton1).mom3Vec();
      CLP4Vector lep2   = contPtr->object(CLPHEPGCont::kLepton2).mom3Vec();
      CLP4Vector bottom = contPtr->object(CLPHEPGCont::kBottom).mom3Vec();
      CLP4Vector charm  = contPtr->object(CLPHEPGCont::kCharm).mom3Vec();
      CLP4Vector light1 = contPtr->object(CLPHEPGCont::kLight1).mom3Vec();
      CLP4Vector light2 = contPtr->object(CLPHEPGCont::kLight2).mom3Vec();
      CLP4Vector Zboson = lep1 + lep2;
      CLP4Vector Wboson = light1 + light2;
      CLP4Vector top1   = Zboson + charm;
      CLP4Vector top2   = Wboson + bottom;
      Zboson.setRestMass (CLPNamespace::kZmass);
      Wboson.setRestMass (CLPNamespace::kWmass);
      CLP4Vector top1a   = Zboson + charm;
      CLP4Vector top2a   = Wboson + bottom;
      // find the best combination
      VVec jetvec;
      jetvec.push_back( bottom );
      jetvec.push_back( light1 );
      jetvec.push_back( light2 );
      jetvec.push_back( charm  );

      if ((bottom != light1) && (bottom != light2) && (bottom != charm) &&
          (light1 != light2) && (light1 != charm) && (light2 != charm))
      {
         IVec results = best_t_Wb (jetvec, false);
         int whichIsCharm = results[3];
         wheresCharm.Fill (whichIsCharm);
         results = best_t_Wb (jetvec, true);
         whichIsCharm = results[3];
         wheresBtagCharm.Fill (whichIsCharm);
      }

      // hepg
      CLP4Vector lep1Hepg   = 
         contPtr->object(CLPHEPGCont::kLepton1).HEPGMom4Vec();
      CLP4Vector lep2Hepg   = 
         contPtr->object(CLPHEPGCont::kLepton2).HEPGMom4Vec();
      CLP4Vector bottomHepg = 
         contPtr->object(CLPHEPGCont::kBottom).HEPGMom4Vec();
      CLP4Vector charmHepg  = 
         contPtr->object(CLPHEPGCont::kCharm).HEPGMom4Vec();
      CLP4Vector light1Hepg = 
         contPtr->object(CLPHEPGCont::kLight1).HEPGMom4Vec();
      CLP4Vector light2Hepg = 
         contPtr->object(CLPHEPGCont::kLight2).HEPGMom4Vec();
      CLP4Vector ZbosonHepg = lep1Hepg + lep2Hepg;
      CLP4Vector WbosonHepg = light1Hepg + light2Hepg;
      CLP4Vector top1Hepg = ZbosonHepg + charmHepg;
      CLP4Vector top2Hepg = WbosonHepg + bottomHepg;
      double charmEt = charm.transMag();
      double w1Et = light1.transMag();
      double w2Et = light2.transMag();
      if (w2Et > w1Et)
      {
         // swap them
         double temp = w1Et;
         w1Et = w2Et;
         w2Et = temp;
      }
      charmEtHist.Fill (charmEt);
      w1EtHist.Fill    (w1Et);
      charmw1diffHist.Fill (charmEt - w1Et);
      w2EtHist.Fill (w2Et);
      charmw2diffHist.Fill (charmEt - w2Et);
      if (charmEt > w1Et)
      {
         ++greater1Num;
      }
      else if (charmEt < w1Et)
      {
         ++lesser1Num;
      }  
      if (charmEt > w2Et)
      {
         ++greater2Num;
      }
      else if (charmEt < w2Et)
      {
         ++lesser2Num;
      }  
               

      //cout << "charm " << charm << endl << "hepg " << charmHepg << endl;
      
      //cout << "Z mass " << Zboson.mass() << endl;
      
      zmassHist.Fill(  Zboson.mass() );
      wmassHist.Fill(  Wboson.mass() );
      tzcmassHist.Fill ( top1.mass() );
      twbmassHist.Fill ( top2.mass() );
      tzcmass2Hist.Fill ( top1a.mass() );
      twbmass2Hist.Fill ( top2a.mass() );
      // hepg
      zmassHepgHist.Fill(  ZbosonHepg.mass() );
      wmassHepgHist.Fill(  WbosonHepg.mass() );
      tzcmassHepgHist.Fill ( top1Hepg.mass() );
      twbmassHepgHist.Fill ( top2Hepg.mass() );
   }

   TFile outputfile ("output.root", "RECREATE");
   zmassHist.Write();
   wmassHist.Write();
   tzcmassHist.Write ();
   twbmassHist.Write ();
   tzcmass2Hist.Write ();
   twbmass2Hist.Write ();
   zmassHepgHist.Write();
   wmassHepgHist.Write();
   tzcmassHepgHist.Write ();
   twbmassHepgHist.Write ();
   charmEtHist.Write();
   w1EtHist.Write();
   charmw1diffHist.Write();
   w2EtHist.Write();
   charmw2diffHist.Write();
   wheresCharm.Write();
   wheresBtagCharm.Write();
   cout << "greater1 " << greater1Num << " lesser1 " << lesser1Num << endl;
   cout << "greater2 " << greater2Num << " lesser2 " << lesser2Num << endl;

   ////////////////////////////
   // ////////////////////// //
   // // matchedCand Tree // //
   // ////////////////////// //
   ////////////////////////////
   TTree *matched = (TTree*) file.Get("matchedCand");
   CLPCandidate *matchedCandPtr = new CLPCandidate;
   matched->SetBranchAddress ("candidate", &matchedCandPtr);
   nentries = Int_t (matched->GetEntriesFast());
   if ((numWanted > 0) && (numWanted < nentries))
   {
      nentries = numWanted;    
   }
   CLPJet::JetList jetlist, goodlist;
   int numCand = 0, numCandCut = 0;
   int numCharmFirst = 0, numUntaggedFirst = 0, numProblems = 0;
   TH1F numJetsHist ("numjetsHist", "Number of Jets", 6, 3.5, 9.5);
   TH1F numminJetsHist ("numminETjetsHist", "Number of Jets with higher Et than tagged jets", 6, 3.5, 9.5);
   for (Int_t entryIndex=0; entryIndex < nentries; ++entryIndex) 
   {
      // check run
      matched->GetEntry (entryIndex);
      matchedCandPtr->sortJets();
      matchedCandPtr->getJets (jetlist);
      //CLPJet::outputJetList (jetlist);
      //cout << endl << endl << endl << endl;
      goodlist.clear();
      int numTagged = 0;
      int numTaggedCut = 0;
      int size = (int) jetlist.size();
      double minEt = 1e3;
      for (int loop = 0; loop < size; ++loop)
      {
         
         if (jetlist[loop].isTagged())
         {
            ++numTagged;
         }
         if (jetlist[loop].Et() > 15 && fabs(jetlist[loop].eta() < 2.4))
         {
            goodlist.push_back( jetlist[loop] );
            if (jetlist[loop].isTagged())
            {
               ++numTaggedCut;
               if (jetlist[loop].Et() < minEt)
               {
                  minEt = jetlist[loop].Et();
               }
            }
         }
      } // for loop
      int numJets = (int) goodlist.size();
      int minCount = 0;
      int whichFirst = 0;  // do we find the charm or an untagged jet first
      for (int loop = 0; loop < numJets; ++loop)
      {
         if (! whichFirst)
         {
            if (goodlist[loop].isTagged())
            {
               // could be the charm
               if (CLPHEPGCont::kCharm == goodlist[loop].taggedToHEPGIndex())
               {
                  // we got it
                  whichFirst = 1;
               }
            } 
            else
            {
               // nope, untagged first
               whichFirst = 2;
            }
            
         }
         if (goodlist[loop].Et() >= minEt)
         {
            ++minCount;
         }
      } // for loop
      if (numJets > 9) numJets = 9;
      if (minCount > 9) minCount = 9;
      if (numTagged == 4)
      {
         ++numCand;
      }
      if (numTaggedCut == 4)
      {
         if (1 == whichFirst)
         {
            ++numCharmFirst;
         }
         else if (2 == whichFirst)
         {
            ++numUntaggedFirst;
         }
         else 
         {
            ++numProblems;
         }
         numJetsHist.Fill(numJets);
         numminJetsHist.Fill(minCount);
         ++numCandCut;
      }
   } // for entryIndex
   cout << "number of jets" << endl;
   numJetsHist.Print("range");
   cout << endl << "number of jets where all four are tagged" << endl;
   numminJetsHist.Print("range");
   cout << "Cands   " << numCand << endl
        << "w/ cuts " << numCandCut << endl;
   cout << "charm first  " << numCharmFirst << endl
        << "untagged 1st " << numUntaggedFirst << endl
        << "problems:    " << numProblems<< endl;
   return 0;
}
