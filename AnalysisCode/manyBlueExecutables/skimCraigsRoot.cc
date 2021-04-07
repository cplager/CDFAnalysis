 // -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>

#include "TClonesArray.h"
#include "TTree.h"
#include "TSystem.h"
#include "TRandom3.h"
#include "TLeaf.h"

#include "RootFileCollection/TreeFileInterface.h"
#include "RootFileCollection/TFINamespace.h"
#include "RootFileCollection/OptionUtils.h"
#include "RootFileCollection/dout.h"

using namespace std;
using namespace OptionUtils;
using namespace TFINamespace;

// Polluting the gLobal namespace - gLobal variables
// Note, name in parenthesis is command-line variable name.
// e.g. 'which=12 usetracks=1' will set gWhich = 12 and gUseTracks = false

// default true beta
double gDefaultTrueBeta = 1.0;
// maximum size of output tree before splitting
int    gMaxTreeSize = -1;
// no pos and neg errors (noerrors)
bool   gNoErrors = false;
// use defaultTrueBeta as beta (usedefault)
bool   gUseDefaultTrueBeta = false;

// forward declarations
void localInitialize();

int main(int argc, char** argv)
{
   // Parse Command Line Arguments          
   // Decide Which Files and How Many Events
   setupDefaultOptions(); // hook up standard options
   localInitialize();
   parseArguments (argc, argv); // OptionUtils::

   TChain *chainPtr = 0;
   if (ns_fileName.length())
   {
      chainPtr = RootFileCollection::chainOneFile (ns_fileName, "Fits");
   } else if (ns_listName.length())
   {
      chainPtr = RootFileCollection::chainFromFile (ns_listName, "Fits");
   }
   if (! chainPtr)
   {
      cerr << "No chain has been defined.  Aborting." << endl;
      exit (1);
   }
   assert (ns_outputName.length());
   TFile *outputFilePtr = TFile::Open (ns_outputName.c_str(), "RECREATE");
   assert (outputFilePtr);
   float trueBeta = gDefaultTrueBeta;
   float measLF   = 0.;
   float measME   = 0.;
   float measNN   = 0.;
   float posLFErr = 0.;
   float posMEErr = 0.;
   float posNNErr = 0.;
   float negLFErr = 0.;
   float negMEErr = 0.;
   float negNNErr = 0.;
   TTree *treePtr = new TTree ("STCtree", "STCtree"); 
   if (gMaxTreeSize > 0) 
   {
      treePtr->SetMaxTreeSize(gMaxTreeSize);
   }   
   treePtr->Branch ("trueBeta", &trueBeta, "trueBeta/F");
   treePtr->Branch ("measLF",   &measLF  , "measLF/F"  );
   treePtr->Branch ("posLFErr", &posLFErr, "posLFErr/F");
   treePtr->Branch ("negLFErr", &negLFErr, "negLFErr/F");
   treePtr->Branch ("measME",   &measME  , "measME/F"  );
   treePtr->Branch ("posMEErr", &posMEErr, "posMEErr/F");
   treePtr->Branch ("negMEErr", &negMEErr, "negMEErr/F");
   treePtr->Branch ("measNN",   &measNN  , "measNN/F"  );
   treePtr->Branch ("posNNErr", &posNNErr, "posNNErr/F");
   treePtr->Branch ("negNNErr", &negNNErr, "negNNErr/F");

   // TLeaf pointers
   TLeaf* me_value   = 0;
   TLeaf* lf_value   = 0;
   TLeaf* nn_value   = 0;
   TLeaf* me_errorUp = 0;
   TLeaf* lf_errorUp = 0;
   TLeaf* nn_errorUp = 0;
   TLeaf* me_errorDn = 0;
   TLeaf* lf_errorDn = 0;
   TLeaf* nn_errorDn = 0;

   int numEvents = (int) chainPtr->GetEntries();
   int prevTree = -1;
   cout << "Writing to " << ns_outputName << endl;
   for (int loop = 0; loop < numEvents; ++loop)
   {

      if (0 == loop % 100000) cout << loop << endl;
      chainPtr->LoadTree (loop);

      // Is this a new file?
      int curTree = chainPtr->GetTreeNumber();      
      if (prevTree != curTree)
      {
         prevTree = curTree;

         // hook up leaves
         lf_value   = chainPtr->FindLeaf("value.LF");
         me_value   = chainPtr->FindLeaf("value.ME");
         nn_value   = chainPtr->FindLeaf("value.NN");
         if (! gNoErrors)
         {
            lf_errorDn = chainPtr->FindLeaf("error.LF_DN");
            me_errorDn = chainPtr->FindLeaf("error.ME_DN");
            nn_errorDn = chainPtr->FindLeaf("error.NN_DN");
            lf_errorUp = chainPtr->FindLeaf("error.LF_UP");
            me_errorUp = chainPtr->FindLeaf("error.ME_UP");
            nn_errorUp = chainPtr->FindLeaf("error.NN_UP");
         }

         if (gUseDefaultTrueBeta)
         {
            continue;
         }

         // Get true value of beta from file name
         string name = chainPtr->GetFile()->GetName();
         const string kLeftMatch  = "combine_";
         const string kRightMatch = "_Tree";
         string::size_type left  = name.find (kLeftMatch);
         string::size_type right = name.find (kRightMatch);
         if (string::npos != left && string::npos != right)
         {
            int start = left + kLeftMatch.length();
            int end   = right - start;
            trueBeta = 0.1 * atoi( name.substr (start, end).c_str() );
            //cout << name.substr (start, end) << " " << trueBeta << endl;
         }
      } // if new tree

      chainPtr->GetEntry(loop);

      measLF   = lf_value->GetValue();
      measME   = me_value->GetValue();
      measNN   = nn_value->GetValue();
      if (! gNoErrors)
      {
         posLFErr = lf_errorUp->GetValue();
         posMEErr = me_errorUp->GetValue();
         posNNErr = nn_errorUp->GetValue();
         negLFErr = lf_errorDn->GetValue();
         negMEErr = me_errorDn->GetValue();
         negNNErr = nn_errorDn->GetValue();
      }
      treePtr->Fill();
      
   } // for loop
   outputFilePtr->cd();
   treePtr->Write();
   delete outputFilePtr;
   
}

void localInitialize()
{   
   // put options you want here
   // addOptionKey ("tscmio", guUseTSCMIO);
   addOptionKey ("defaultTrueBeta", gDefaultTrueBeta);
   addOptionKey ("maxtreesize",     gMaxTreeSize);
   addOptionKey ("noerrors",        gNoErrors);
   addOptionKey ("usedefault",      gUseDefaultTrueBeta);
}
