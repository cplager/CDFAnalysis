#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
// #include "DataFormats/FWLite/interface/Run.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/MultiChainEvent.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "DataFormats/Provenance/interface/LuminosityBlockRange.h"

#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"

#include "Math/GenVector/PxPyPzM4D.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

//Root includes
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"

using namespace std;

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
  edm::ParameterSet const& shyftParameters = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("shyftAnalysis");
  edm::ParameterSet const& inputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet const& outputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("outputs");

   
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
  int nInclusive = 0;
  int printEvery = shyftParameters.getParameter<int>("printEvery");
  int maxInclusive = shyftParameters.getParameter<int>("maxInclusive");
     
  //loop through each event
  cout << "Beginning event loop." << endl;
  for( ev.toBegin();
       ! ev.atEnd();
       ++ev) {
        
    if (maxInclusive > 0 && nInclusive >= maxInclusive) break;

    if (printEvery > 0 && nInclusive % printEvery == 0) cout << "About to process event " << nInclusive+1 << "." << endl;
    if ( ev.event()->size() == 0 ) continue; // skip trees with no events
      
    ++nInclusive;

    edm::Handle<pat::TriggerEvent> triggerEvent;
    edm::InputTag trigTag_ = shyftParameters.getParameter<edm::InputTag>("trigSrc");
    ((edm::EventBase *)&ev)->getByLabel(trigTag_, triggerEvent);

    pat::TriggerEvent const * trig = &*triggerEvent;

    std::cout << "====Trigger Info for Event:" << std::endl
	      << "  -->Trigger was run? " << trig->wasRun() << std::endl
	      << "  -->Event accepted? " << trig->wasAccept() << std::endl;


    if ( trig->wasRun() && trig->wasAccept() ) {

      std::cout << std::endl
		<< "*** Triggers found ***" << std::endl;

      const pat::TriggerPathCollection *paths = trig->paths();

      for (pat::TriggerPathCollection::const_iterator pIter = paths->begin();
	   pIter != paths->end(); ++pIter) {

	const pat::TriggerPath &path = *pIter;
	std::cout << "  --> Path = " << path.name() << ": Passed? =>" << path.wasAccept() << std::endl;
      }

    }

  } //end event loop
     
  cout << "Printing" << endl;


  return 0;
}


