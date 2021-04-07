 // -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

#include "TClonesArray.h"
#include "TTree.h"
#include "TSystem.h"
#include "TRandom3.h"

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

// maximum size of output tree before splitting
int    gMaxTreeSize = -1;

// forward declarations
void localInitialize();

int main(int argc, char** argv)
{
   // Parse Command Line Arguments          
   // Decide Which Files and How Many Events
   setupDefaultOptions(); // hook up standard options
   localInitialize();
   parseArguments (argc, argv); // OptionUtils::

   delete gRandom; gRandom = new TRandom3;

   string tag = ""; // any additional tags you want added on an output
                    // filename

   // setup the TFI to use the top tree and TopNtupleDecoder
   setupTFI ("PEtree", tag);

   ////////////////////////
   // Setup TCA Branches //
   ////////////////////////


   // By default, TreeFileInterface only does a 'GetEntry()' on the
   // branches as they are asked for.  When 'fillCloneTree()' is
   // called, it will do a 'GetEntry()' on the entire tree.  If you
   // are modifying the values of the branches, these changes will be
   // lost unless you tell the the TreeFileInterface to get the whole
   // event the first time.  Note that this is slower, but safer.
   ns_tfi.setGetEntireTree (true);
   

   // Tell 'ns_tfi' which branches we want access to.
   const string kMeasBF = "measBF";
   ns_tfi.addBranch< float > (kMeasBF);


   // Send in the clones
   TFile *filePtr = new TFile (ns_outputName.c_str(), "RECREATE");
   if (! filePtr->IsOpen()) {
      cerr << "Did not successfully open " << ns_outputName 
           << " for output.  Aborting." << endl;
      return 0;      
   }
   ns_tfi.makeCloneTree (filePtr, gMaxTreeSize);

   while (ns_tfi.getNextEvent())
   {
      /////////////////////////////////////
      // /////////////////////////////// //
      // // ///////////////////////// // //
      // // // Put Your Magic Here // // //
      // // ///////////////////////// // //
      // /////////////////////////////// //
      /////////////////////////////////////

      float *measBFptr = ns_tfi.ptr< float > (kMeasBF);
      //*measBFptr += gRandom->Gaus (0, 0.0036);
      ns_tfi.fillCloneTree();
   } // while get next event
}

void localInitialize()
{   
   // put options you want here
   // addOptionKey ("tscmio", guUseTSCMIO);

}
