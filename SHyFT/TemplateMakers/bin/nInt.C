#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/MultiChainEvent.h"
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "DataFormats/Provenance/interface/LuminosityBlockRange.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"


#include "DataFormats/PatCandidates/interface/TriggerObject.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "Math/GenVector/PxPyPzM4D.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

//Root includes
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TTree.h"



using namespace std;
//using namespace edm;

int main ( int argc, char ** argv )
{
   // load framework libraries
   gSystem->Load( "libFWCoreFWLite" );
   AutoLibraryLoader::enable();

   if ( argc < 2 ) {
      std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
      return 2;
   }
  
   // Get the python configuration
   cout << "Unpacking configuation parameters." << endl;
   PythonProcessDesc builder(argv[1],argc,argv);
   cout<< "here" << endl;
   edm::ParameterSet const& shyftParameters = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("shyftAnalysis");
   edm::ParameterSet const& inputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("inputs");
   edm::ParameterSet const& outputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("outputs");

   // book a set of histograms
   cout << "Booking histograms." << endl;
   fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );
   //Book the histograms
   std::map<TString, TH1*> histograms;
   std::map<TString, TH3*> histograms3D;
   histograms["nInteractions"] =  fs.make<TH1F>("nInteractions", "Number of interactions", 50,-0.5,49.5);
   histograms3D["nInteractions3D"] = fs.make<TH3F>("nInteractions3D","Number of Interactions (x = BX-1, y = BX, z = BX+1)",
                                                 50,-0.5,49.5,
                                                 50,-0.5,49.5,
                                                 50,-0.5,49.5);

   histograms["nPV"] =  fs.make<TH1F>("nPV", "Number of Reconstructed Primary Vertices", 50,-0.5,49.5);
   histograms["nPVvsNInt"] =  fs.make<TH2F>("nPVvsNInt", "Number of Reconstructed Primary Vertices vs Number of Interactions", 50,-0.5,49.5, 50, -0.5, 49.5);

   bool selectLumis = false;
   std::vector<edm::LuminosityBlockRange> lumis;
   if ( inputs.exists("lumisToProcess") ) {
      selectLumis = true;
      std::vector<edm::LuminosityBlockRange> const & lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
      lumis.resize( lumisTemp.size() );
      copy( lumisTemp.begin(), lumisTemp.end(), lumis.begin() );
   }

   // This object 'event' is used both to get all information from the
   // event as well as to store histograms, etc.
   cout << "Initialize ChainEvent" << endl;
   std::vector<std::string> fileNames = inputs.getParameter<std::vector<std::string> > ("fileNames");
   fwlite::ChainEvent ev (fileNames );
     
   // 
   // Immediately check to see if you have no events in your chain
   cout << "Checking to see if we have any events in the files you requested... " << endl;
   
   bool chainIsValid = ev.isValid();
   Long64_t chainSize = ev.size();
     
   cout << "Is valid?  " << chainIsValid << endl
        << "Size?      " << chainSize << endl;
     
   if ( ! ( chainIsValid && (chainSize > 0) ) ) {
        
      cout << "Chain is not valid or size is not > 0... quitting"
           << endl;
        
      return -10;
        
   }     
     
   //Some other variables used in the loop
   int nTotal  = 0;
   int printEvery = shyftParameters.getParameter<int>("printEvery");
     
   //loop through each event
   cout << "Beginning event loop." << endl;
   for( ev.toBegin();
        ! ev.atEnd();
        ++ev, ++nTotal) {     
     
     if (printEvery > 0 && nTotal % printEvery == 0) cout << "About to process event " << nTotal+1 << "." << endl;
     if ( ev.event()->size() == 0 ) continue; // skip trees with no events
     
     //If desired, only process events from good lumi sections.
     if ( selectLumis ) {
       bool goodLumi = false;
       for ( std::vector<edm::LuminosityBlockRange>::const_iterator lumisBegin = lumis.begin(),
               lumisEnd = lumis.end(), ilumi = lumisBegin;
             ilumi != lumisEnd; ++ilumi ) {
         if ( ev.id().run() >= ilumi->startRun() && ev.id().run() <= ilumi->endRun()  &&
              ev.id().luminosityBlock() >= ilumi->startLumi() && ev.id().luminosityBlock() <= ilumi->endLumi() )  {
           goodLumi = true;
           break;  //Don't need to keep searching after you've found a match
         }
       }
       if ( !goodLumi ) continue;
     }
     
     int numInteractions  = -1;
     int numInteractionsM1  = -1;
     int numInteractionsP1  = -1;
     
     if(!ev.isRealData()){
       edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
       ((edm::EventBase*)&ev)->getByLabel(edm::InputTag("addPileupInfo"),PupInfo);      
       std::vector<PileupSummaryInfo>::const_iterator PUi;
       
       
       float sum_nvtx = 0;
       for(PUi = PupInfo->begin(); PUi != PupInfo->end(); ++PUi){
         int BX = PUi->getBunchCrossing();
         if(BX==0){
           numInteractions = PUi->getPU_NumInteractions();
           continue;
         }
         if(BX==-1){
           numInteractionsM1 = PUi->getPU_NumInteractions();
           continue;
         }
         if(BX==1){
           numInteractionsP1 = PUi->getPU_NumInteractions();
           continue;
         }

       }
     }
     
     histograms["nInteractions"]->Fill(numInteractions);
     histograms3D["nInteractions3D"]->Fill(numInteractionsM1,numInteractions,numInteractionsP1);

     edm::Handle<reco::VertexCollection> vertices;
     std::string vertexColl = "goodOfflinePrimaryVertices";
     ((edm::EventBase*)&ev)->getByLabel(edm::InputTag(vertexColl), vertices);
     
     reco::VertexCollection vertexCollection = *(vertices.product());
     int vertex_number = vertexCollection.size();
     
     histograms["nPV"]->Fill(vertex_number);
     histograms["nPVvsNInt"]->Fill(numInteractions,vertex_number);

   } //end event loop
     
   cout << "We're done!" << endl;
   cout << "Processed " << nTotal << " events." << endl;
  
   return 0;
}
