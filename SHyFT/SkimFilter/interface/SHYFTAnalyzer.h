#ifndef Analysis_FullFW_SHyFT_SHYFTAnalyzer_h
#define Analysis_FullFW_SHyFT_SHYFTAnalyzer_h

#include "PhysicsTools/SelectorUtils/interface/EventSelector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/SelectorUtils/interface/ElectronVPlusJetsIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/MuonVPlusJetsIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace std;

class SHYFTAnalyzer : public edm::EDFilter {

  
  
public: 
  explicit SHYFTAnalyzer (const edm::ParameterSet &);
  ~SHYFTAnalyzer();
  
  
private:
  virtual void beginJob();

  virtual bool beginRun(edm::Run&, edm::EventSetup const&);

  virtual bool filter( edm::Event&, const edm::EventSetup&);
  
  virtual bool endRun(edm::Run&, edm::EventSetup const&);

  virtual void endJob() ;

  WPlusJetsEventSelector wPlusJets;

  bool verbose_;

  double jesShift_;

  bool trigRequired_;

  unsigned int nProcessed_;

  vector < edm::EventID > selectedEvents;
  
};


#endif
