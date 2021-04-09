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
#include "PhysicsTools/Utilities/interface/Lumi3DReWeighting.h"

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
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TTree.h"



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
   cout<< "here" << endl;
   edm::ParameterSet const& shyftParameters = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("shyftAnalysis");
   edm::ParameterSet const& inputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("inputs");
   edm::ParameterSet const& outputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("outputs");

   bool selectLumis = false;
   std::vector<edm::LuminosityBlockRange> lumis;
   if ( inputs.exists("lumisToProcess") ) {
      selectLumis = true;
      std::vector<edm::LuminosityBlockRange> const & lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
      lumis.resize( lumisTemp.size() );
      copy( lumisTemp.begin(), lumisTemp.end(), lumis.begin() );
   }

   std::string puWeightFile = shyftParameters.getParameter<std::string>("pileupWeightsFile");
   cout << "Using 3D PU weights from file " << puWeightFile << std::endl;
   edm::Lumi3DReWeighting LumiWeights_;
   LumiWeights_.weight3D_init(puWeightFile);
 
   // book a set of histograms
   cout << "Booking histograms." << endl;
   fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );

   //Book the histograms
   std::map<TString, double *> doubleBranches;
   //---------
   // Mttbar
   //---------
   doubleBranches["truthMtt"] = new double(0.0);
   doubleBranches["reconMtt"] = new double(0.0);
   //-------------------
   //Kinematic Variables
   //-------------------
   //MuPt
   doubleBranches["truth_muPt"] = new double(0.0);
   doubleBranches["recon_muPt"] = new double(0.0);
   //MET
   doubleBranches["truth_met"] = new double(0.0);
   doubleBranches["recon_met"] = new double(0.0);
   //mT
   doubleBranches["truth_mT"] = new double(0.0);
   doubleBranches["recon_mT"] = new double(0.0);
   //nJets
   doubleBranches["truth_nJets"] = new double(0.0);
   doubleBranches["recon_nJets"] = new double(0.0);
   //----------------
   //Other Variables
   //----------------
   doubleBranches["weight"] = new double(0.0);
   doubleBranches["flavor"] = new double(0.0);
  
   
   TTree * mttTree = fs.make<TTree> ("NTuple", "Truth vs Measured");
   for (std::map<TString, double *>::iterator iDMap = doubleBranches.begin();
        iDMap != doubleBranches.end();
        iDMap ++) {
     
     std::cout << "Creating branch   " << (iDMap->first).Data() << endl;
     TString ttString = iDMap->first;
     ttString += "/D";
     mttTree->Branch(iDMap->first, iDMap->second, ttString);       
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
  

   cout << "Initialize WPlusJetsEventSelector" << endl;
   WPlusJetsEventSelector wPlusJets( shyftParameters );
   pat::strbitset ret = wPlusJets.getBitTemplate();

   bool skipSel = shyftParameters.getParameter<bool>("skipSelection");
   if (skipSel) {
     cout << "*******************************************" << endl
          << "*******************************************" << endl
          << "  Selection has been turned off." << endl
          << "  Assuming collections have already been prefiltered." << endl
          << "  This is appropriate for the new TQAF-style skims." << endl
          << "*******************************************" << endl
          << "*******************************************" << endl;
   } else {
     cout << "Cuts defined:" << endl;
     wPlusJets.printActiveCuts(cout);
   }
   
     
     
   //Some other variables used in the loop
   int nInclusive = 0;
   int nPassed = 0;
   int nDumped = 0;
   int nTotal  = 0;
   int maxToDump = shyftParameters.getParameter<int>("maxDump");
   int maxInclusive = shyftParameters.getParameter<int>("maxInclusive");
   int maxPassed = shyftParameters.getParameter<int>("maxPassed");
   int printEvery = shyftParameters.getParameter<int>("printEvery");
   double muPtMin = shyftParameters.getParameter<double>("muPtMin");
   cout << "muPt cut: " << muPtMin << " Gev" << endl;
   int minJets = shyftParameters.getParameter<int>("minJets");
   int maxJets = shyftParameters.getParameter<int>("maxJets");
   // int maxJets = 100;
   string gen = shyftParameters.getParameter<string>("gen");
   TString sgen = gen;
   cout << "GenParicle collection (differnet for PATtup and Skims): " << gen << endl;
   cout << "jet range " << minJets << "-" << maxJets << endl;
   //btag -1 = no btag, 1= 1 btag, 0 = 0 btags
   double btag = shyftParameters.getParameter<double>("btag");
   cout << "number of b-tags:  " << btag << endl;
   double btagWP = 0.244; //Loose CSV working point
   bool passTagReq; 
   
     
   //loop through each event
   cout << "Beginning event loop." << endl;
   for( ev.toBegin();
        ! ev.atEnd();
        ++ev, ++nTotal) {
     passTagReq = false; 
     for (std::map<TString, double *>::iterator iDMap = doubleBranches.begin();
          iDMap != doubleBranches.end();
          iDMap ++) {

       *(iDMap->second) = -9.9e20;
      
     }
     
        
     if (maxInclusive > 0 && nInclusive >= maxInclusive) break;
     if (maxPassed > 0 && nPassed >= maxPassed) break;

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

     
     ret.set(false);
     bool passed = false;  //= wPlusJets(ev, ret);
     
     //Check whether the event passes
     if (skipSel) {
        passed = true;
        ++nInclusive;
      } else {
        passed = wPlusJets(ev, ret);
        if (ret[std::string("Inclusive")]) ++nInclusive;
      }

 
     //Since we only care about selected events, why make this more
     //complicated than it has to be?
     if (!passed) continue;

     ++nPassed;

     //int numInteractions  = -1;
     //double event_weight = 1;

 


     //  double weight = event_weight;
//      // double weight = 1;
   
      
      std::vector<reco::ShallowClonePtrCandidate> tempElectrons; 
      std::vector<reco::ShallowClonePtrCandidate> tempMuons; 
      std::vector<reco::ShallowClonePtrCandidate> tempJets;
      reco::ShallowClonePtrCandidate tempMET;
      if (skipSel) {

        edm::Handle< vector< pat::Electron > > electronHandle;
        ((edm::EventBase*)&ev)->getByLabel(shyftParameters.getParameter<edm::InputTag>("electronSrc"),electronHandle);
        for ( std::vector<pat::Electron>::const_iterator electronBegin = electronHandle->begin(),
                electronEnd = electronHandle->end(), ielectron = electronBegin;
              ielectron != electronEnd; ++ielectron ) {
          tempElectrons.push_back(reco::ShallowClonePtrCandidate( edm::Ptr<pat::Electron>(electronHandle, ielectron - electronBegin)));
        }

        edm::Handle< vector< pat::Muon > > muonHandle;
        ((edm::EventBase*)&ev)->getByLabel(shyftParameters.getParameter<edm::InputTag>("muonSrc"),muonHandle);
        for ( std::vector<pat::Muon>::const_iterator muonBegin = muonHandle->begin(),
                muonEnd = muonHandle->end(), imuon = muonBegin;
              imuon != muonEnd; ++imuon ) {
          tempMuons.push_back(reco::ShallowClonePtrCandidate( edm::Ptr<pat::Muon>(muonHandle, imuon - muonBegin)));
        }        

        if(btag ==0 || btag == -1){passTagReq = true;} //assume the event will pass
        else{passTagReq = false;}  
        edm::Handle< vector< pat::Jet > > jetHandle;
        ((edm::EventBase*)&ev)->getByLabel(shyftParameters.getParameter<edm::InputTag>("jetSrc"),jetHandle);
        for ( std::vector<pat::Jet>::const_iterator jetBegin = jetHandle->begin(),
                jetEnd = jetHandle->end(), ijet = jetBegin;
              ijet != jetEnd; ++ijet ) {
          double bDiscCSV = ijet->bDiscriminator("combinedSecondaryVertexBJetTags");
          //          cout << " bDiscCSV: " << bDiscCSV << endl;
          if(btag == 1){
            if(bDiscCSV>btagWP){passTagReq = true;}
          }
          if(btag == 0){
            if(bDiscCSV>btagWP){passTagReq = false;}
          }     
          tempJets.push_back(reco::ShallowClonePtrCandidate( edm::Ptr<pat::Jet>(jetHandle, ijet - jetBegin)));
        }
        //cout << " Event passes cuts: " << passTagReq << endl;
        if(passTagReq == false){
          continue;
        }
        
        edm::Handle< vector< pat::MET > > metHandle;
        ((edm::EventBase*)&ev)->getByLabel(shyftParameters.getParameter<edm::InputTag>("metSrc"),metHandle);
        tempMET = reco::ShallowClonePtrCandidate(edm::Ptr<pat::MET>(metHandle,0),metHandle->at(0).charge(),
                                                 metHandle->at(0).p4());
        
      }
      
      std::vector<reco::ShallowClonePtrCandidate> const & electrons =  tempElectrons;
      std::vector<reco::ShallowClonePtrCandidate> const & muons     =  tempMuons;
      std::vector<reco::ShallowClonePtrCandidate> const & jets      =  tempJets;
      //This list may still have electrons (and muons?) double-counted as jets
      //For "skipSel" just pass in the original jet collection again...
      std::vector<reco::ShallowClonePtrCandidate> const & jetsBeforeClean = tempJets;
      reco::ShallowClonePtrCandidate   const & met             = tempMET;   
      
     //Determine if this is a muon event from the generator level
      //sgen TString is defined in .py for either skims or pattups
      fwlite::Handle<reco::GenParticleCollection> genParticles;
      genParticles.getByLabel(ev,sgen);
    
     //----------------------------// Apply cuts //--------------------------------------//
     //Jet Cut
     int nJets = jets.size();
     if(nJets<(minJets)){
       // cout << jets.size()+1 << " jet event   Jet min is " << minJets << endl;
       continue;
     }
     if(nJets>(maxJets)){
       // cout << jets.size()+1 << " jet event   Jet max is " << minJets << endl;
       continue;
     }
     //Muon Pt Cut
     const pat::Muon * muonOrig_ = 0;
     if(muons.size()<1){
       continue;
     }
     if(muons.size()>0){
     muonOrig_ = dynamic_cast<const pat::Muon *>(muons[0].masterClonePtr().get());
    
      if (muonOrig_->pt()<muPtMin){
        // cout << "muPt: "<< muonOrig_->pt()<< " does not pass muPtMin: " <<  muPtMin << endl;
             continue;
      }
     }

     //-------------------------//  PU Reweighting  //------------------------------------//
     int numInteractions  = -1;
     double weight = 1;

     if(!ev.isRealData()){
       edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
       ((edm::EventBase*)&ev)->getByLabel(edm::InputTag("addPileupInfo"),PupInfo);
       std::vector<PileupSummaryInfo>::const_iterator PUi;

       //
       // neu, 20/9/2011: do the following for incorporating in-time PU contributions
       //   this is the one to use for ttbar, lvjj analyses
       //

       if (nDumped < maxToDump || maxToDump < 0) {
         std::cout << "===>Pileup information!" << std::endl;
         std::cout << "  Size of PileupSummaryInfo vector " << PupInfo->size() << std::endl;
       }

       int nm1 = -1; int n0 = -1; int np1 = -1;
       for(PUi = PupInfo->begin(); PUi != PupInfo->end(); ++PUi) {

         int BX = PUi->getBunchCrossing();

         if(BX == -1) {
           nm1 = PUi->getPU_NumInteractions();
         }
         if(BX == 0) {
           n0 = PUi->getPU_NumInteractions();
         }
         if(BX == 1) {
           np1 = PUi->getPU_NumInteractions();
         }
       }

       numInteractions = n0;
       weight = LumiWeights_.weight3D( nm1,n0,np1);
     }
     
    if (nDumped < maxToDump) {
      std::cout << "--------------------------------------------" << std::endl;
 
      std::cout << "Dump of header information from GenParticles" << std::endl << std::endl
                << "Ind  ID         Status  Mother ID  Mother Ind  Pt        Eta        Phi" << std::endl
                << "---  ---------  ------  ---------  ----------  --------  ---------  --------" << std::endl;
    }

 
    //variables for gen particles;
    int nTop = 0;
    reco::Candidate::LorentzVector ttbarVect;
    reco::Candidate::LorentzVector muonLV;
    reco::Candidate::LorentzVector neutrinoLV;
    reco::Candidate::LorentzVector WLV;
    reco::Candidate::LorentzVector WTranverseLV;
    
      
    for(size_t i = 2; i < genParticles->size(); ++ i) {

      const reco::GenParticle & p = (*genParticles)[i];

      //tops
      if (abs(p.pdgId()) == 6 && p.status()==3) {
        ttbarVect += p.p4();
        ++nTop;
      }

      //muon
      if (abs(p.pdgId()) == 13 && p.status()==3) {
        //match the recoMuon to the genMuon
        // int nMuons = muons.size();
        //for(int i=0; i<nMuons; i++){
        //  double dR = reco::deltaR(muons[i].eta(),muons[i].phi(),p.eta(),p.phi());
        //  cout << "muon " << i << " dR " << dR << endl;
        // }
        muonLV += p.p4();
        WLV += p.p4();
      
       }


       //muon neutrino
       if (abs(p.pdgId()) == 14 && p.status()==3) {
         neutrinoLV  += p.p4();
         WLV += p.p4();   
       }
      
     
      const reco::Candidate *mo = p.mother();
      int moInd = -999;
      int moId = 0;
      if (mo) {
        
        moId = mo->pdgId();
        
        for (size_t j = 0; j < i; ++j) {
          
          const reco::GenParticle & pj = (*genParticles)[j];
          
          if ((pj.pdgId() == mo->pdgId()) &&
              (pj.px() == mo->px()) &&
              (pj.py() == mo->py()) &&
              (pj.pz() == mo->pz()) &&
              (pj.energy() == mo->energy())) {
            moInd = j;
            break;
          }
        }
      }
     
      
      if (nDumped < maxToDump) {
        std::cout << std::setw(3) << i << "  "
                  << std::setw(9) << p.pdgId() << "  "
                  << std::setw(6) << p.status() << "  "
                  << std::setw(9) << moId << "  "
                  << std::setw(10) << moInd << "  "
                  << Form("%8.2f",p.pt()) << "  "
                  << Form("%9.4f",p.eta()) << "  "
                  << Form("%8.4f",p.phi()) << "  "
                  << std::endl;
        
      }
    }

    if (nTop != 2) {
      std::cout << "*************************************************************" << std::endl
                << "*****                                                   *****" << std::endl
                << "*****                                                   *****" << std::endl
                << "*****       WARNING: Did not find 2 top quarks!         *****" << std::endl
                << "*****       Found " << std::setw(3) << nTop << " instead!                          *****" << std::endl
                << "*****                                                   *****" << std::endl
                << "*****                                                   *****" << std::endl
                << "*************************************************************" << std::endl;
    }

    WTranverseLV += WLV;
    WTranverseLV.SetPz(0.);
    WTranverseLV.SetE(WLV.pt());
    
    *doubleBranches["truthMtt"] = ttbarVect.M();
    *doubleBranches["truth_muPt"] = muonLV.pt();
    *doubleBranches["truth_met"] = neutrinoLV.Et();
    *doubleBranches["truth_mT"] = WTranverseLV.M();
    *doubleBranches["recon_nJets"] = jets.size();
    
    reco::Particle::LorentzVector mttReco = muons[0].p4();

    mttReco += met.p4();

    int jetCounter = 0;
    for (std::vector<reco::ShallowClonePtrCandidate>::const_iterator jetIter = jets.begin();
         jetIter != jets.end(); ++jetIter) {
      jetCounter += 1;
      if(jetCounter<=4){
      mttReco += jetIter->p4();
      }

    }
  
    
    *doubleBranches["reconMtt"] = mttReco.M();
    *doubleBranches["recon_met"] = met.et();
    *doubleBranches["recon_muPt"] = muons[0].pt();

    //mT calculation
    reco::Particle::LorentzVector leptonLV;
    leptonLV = muons[0].p4();
    reco::Particle::LorentzVector metLV = met.p4();
    metLV.SetPz(0.); //Just to be safe--MET should never have Z component.  That's why it's called ME*T*!
    metLV.SetE(met.pt());
    reco::Particle::LorentzVector leptonTrans = leptonLV;
    leptonTrans.SetPz(0.);
    leptonTrans.SetE(leptonLV.pt());
    reco::Particle::LorentzVector WTrans = leptonTrans + metLV;
    
    *doubleBranches["recon_mT"] = WTrans.M();
    
    // fill a weight for each event
    *doubleBranches["weight"] = weight;

    // fill a flavor for each event
    // set all flavors to one for right now
    *doubleBranches["flavor"] = 1.0;

    if (nDumped < maxToDump) {
      std::cout << "-->NJets = " << std::setw(3) << jets.size()
                << ", truth mtt = " << Form("%15.2f",ttbarVect.M()) 
                << ", reco mtt = " << Form("%15.2f",mttReco.M()) << std::endl;
        
      ++nDumped;
    }
  
    mttTree->Fill();
   } //end event loop
   if (!skipSel) {
     cout << "Printing" << endl;
     wPlusJets.print(std::cout);
     
   cout << "We're done!" << endl;
   cout << "Processed " << nTotal << " events." << endl
        << "Passed " << nPassed << " events." << endl;

   } 
   
  
   return 0;
}
