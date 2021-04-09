
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include "SHyFT/SkimFilter/interface/SHYFTAnalyzer.h"



#include <iostream>

using namespace std;
using namespace edm;



SHYFTAnalyzer::SHYFTAnalyzer( const ParameterSet & pset ) :
  wPlusJets(pset.getParameter<edm::ParameterSet>("selectionDetails")),
  nProcessed_(0)
{

  // constructor body

  // In theory, we would parse all constructor inputs from the pset
  // for now, we'll just chose the most important

  verbose_ = pset.getUntrackedParameter<bool> ("verbose", true);

  // true is default
  // true means, "Yes, require that events pass trigger cut"
  // false means no trigger cut

  trigRequired_ = pset.getUntrackedParameter<bool> ("trigRequired", true);
  
  // turn on/off trigger cuts
  // they are on by default in the constructor
  
  if (verbose_) {
    cout << "Trigger is required? " << trigRequired_ << endl;
  }
  
  if ( !trigRequired_ ) {

    cout << "Chaning bit requirements to remove trigger" << endl;
      
    wPlusJets.set("Trigger", 0, false);

  }

  cout << "List of active cuts:" << endl;
  wPlusJets.printActiveCuts(cout);

  produces<unsigned int,InRun>("nProcessedSkim");
  
   
  //===================================
  //
  // You've initialized your selector
  // You're ready to loop over events
  //
  //==================================  


  if (verbose_) cout << "Done with Constructor" << endl;


  
  
}

void SHYFTAnalyzer::beginJob() {

  if (verbose_) cout << "Inside beginJob()" << endl;
  
  // Nothing to do for now

  selectedEvents.clear();

  
}

bool SHYFTAnalyzer::beginRun(edm::Run&, edm::EventSetup const&) {

  nProcessed_ = 0;
  return true;

}


bool  SHYFTAnalyzer::filter( edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if (verbose_) cout << "Inside filter for run = " << iEvent.id().run()
       << ", event = " << iEvent.id().event();

  EventID myID = iEvent.id();
  
  // despite living in the std namespace,
  // this class isn't std
  pat::strbitset ret = wPlusJets.getBitTemplate();

  // arguments are an event class, and
  // the str
  bool passed = wPlusJets(iEvent, ret);
  
  if (verbose_) cout << "... Passed? = " << passed << endl;

  if (passed) selectedEvents.push_back(myID);

  
  return passed;
  
}

bool SHYFTAnalyzer::endRun(edm::Run& iRun, edm::EventSetup const& iEventSetup) {

  std::auto_ptr<unsigned int> nProcessedSkim ( new unsigned int(nProcessed_) );
  iRun.put(nProcessedSkim,"nProcessedSkim");
  
  return true;

}


//============= Destructor

SHYFTAnalyzer::~SHYFTAnalyzer () {
  
}

void SHYFTAnalyzer::endJob() {

    if (verbose_) cout << "Inside endJob(), printing acceptance table" << endl;

    wPlusJets.print(std::cout);


    cout << "You have saved " << selectedEvents.size()  << " events in your output." << endl;

    cout << "Dumping Run/Event/Lumi" << endl;


    for ( vector< edm::EventID >::const_iterator iEvent = selectedEvents.begin();
          iEvent != selectedEvents.end();
          iEvent ++ ) {

      cout << "Run: " << iEvent->run()
           << " Lumi: " << iEvent->luminosityBlock()
           << " Event: " << iEvent->event()
           << endl;

    }

    
}



DEFINE_FWK_MODULE(SHYFTAnalyzer);
