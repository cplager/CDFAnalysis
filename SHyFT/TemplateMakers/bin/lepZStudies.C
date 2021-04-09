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

#include "DataFormats/PatCandidates/interface/TriggerObject.h"

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


// useful helper functions defined in this file
std::string binomialErrorString (double denom, double numer);
double binomialError (double denom, double numer);
double matchToTrigger (fwlite::ChainEvent ev , pat::Muon imuon, edm::InputTag trigTag, std::string triggerName);
void generic_eff (TH1* denom, TH1* numer, TH1* efficiencyHist);
void moveOverflow ( TH1* myElement);


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

   bool selectLumis = false;
   std::vector<edm::LuminosityBlockRange> lumis;
   if ( inputs.exists("lumisToProcess") ) {
      selectLumis = true;
      std::vector<edm::LuminosityBlockRange> const & lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
      lumis.resize( lumisTemp.size() );
      copy( lumisTemp.begin(), lumisTemp.end(), lumis.begin() );
   }


   //Determine the jet bins we're using
   int minNJets = shyftParameters.getParameter<int>("minJets");
   int maxNJets = shyftParameters.getParameter<int>("maxJets");
   float minNJetsFloat = (float)minNJets - 0.5;
   float maxNJetsFloat = (float)maxNJets + 0.5;


   // book a set of histograms
   cout << "Booking histograms." << endl;
   fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );

   std::map<TString, TH1*> histograms;

   // Declare this to keep it consistent
  int nPtBins;
  int ptMin = 0.0;
  int ptMax = 50;

  double ptVals[50];
  ptVals[0] = 0.0;
  ptVals[1] = 2.0;
  ptVals[2] = 4.0;
  ptVals[3] = 6.0;
  ptVals[4] = 8.0;
  ptVals[5] = 10.0;
  ptVals[6] = 12.0;
  ptVals[7] = 14.0;
  ptVals[8] = 16.0;
  ptVals[9] = 18.0;
  ptVals[10] = 20.0;
  ptVals[11] = 22.0;
  ptVals[12] = 25.0;
  ptVals[13] = 30.0;
  ptVals[14] = 40.0;
  ptVals[15] = 60.0;
  ptVals[16] = 100.0;
  nPtBins = 16;

  double etaVals[50];
  int etaBins;
  etaVals[0] = -2.1;
  etaVals[1] = -1.1;
  etaVals[2] = -0.8;
  etaVals[3] = 0.0;
  etaVals[4] = 0.8;
  etaVals[5] = 1.1;
  etaVals[6] = 2.1;
  etaBins = 6;

  double phiVals[50];
  int phiBins;
  // phiVals[0] = 
  phiVals[0] = -2.8;
  phiVals[1] = -2.1;
  phiVals[2] = -1.1;
  phiVals[3] = -0.8;
  phiVals[4] = 0.0;
  phiVals[5] = 0.8;
  phiVals[6] =  1.1;
  phiVals[7] = 2.1;
  phiVals[8] = 2.8;
  phiBins = 8;

  
   //////////////Muon Variables////////////// 
   histograms["hcalIso"] = fs.make<TH1F>( "hcalIso", "hcal Iso", 40, 0, 2);
   histograms["ecalIso"] = fs.make<TH1F>( "ecalIso", "ecal Iso", 40, 0, 2);
   histograms["trkIso"] = fs.make<TH1F>( "trkIso", "track Iso", 40, 0, 2);
   histograms["relIso"] = fs.make<TH1F>( "relIso", "rel Iso", 40, 0, .1);
   histograms["nDTStations"] = fs.make<TH1F>( "nDTStations", "nDT Stations", 6, -.5, 5.5);
   histograms["nCSCStations"] = fs.make<TH1F>( "nCSCStations", "nCSC Stations", 6, -.5, 5.5);
   histograms["nTotalStations"] = fs.make<TH1F>( "nTotalStations", "Total Muon Stations", 10, -.5, 9.5);
   histograms["numberOfMuonHits"] = fs.make<TH1F>( "numberOfMuonHits", "Number of Muon Hits", 61, -0.5, 60.5);
   histograms["numberOfStripHits"] = fs.make<TH1F>( "numberOfStripHits", "Number of Strip Hits", 41, -0.5, 40.5);
   histograms["numberOfPixelLayersWithMeasurement"] = fs.make<TH1F>( "numberOfPixelLayersWithMeasurement", "Number of Pixel Layers With Measurement", 5, -0.5, 4.5);
   histograms["missingTrackerHits"] = fs.make<TH1F>( "missingTrackerHits", "Missing Tracker Hits", 4, -.5, 3.5);
   histograms["minTrigDR"] = fs.make<TH1F>("minTrigDR","Minimum #Delta R (#mu - Trig)", 400, 0., 4.);
   histograms["minJetDR"] = fs.make<TH1F>("minJetDR","Minimum #Delta R (#mu - jet)", 400, 0., 4.); 

       
   histograms["muPt"] = fs.make<TH1F>( "muPt",     "Muon p_{T} (GeV/c) ",     200, 0, 200 );
   histograms["muEta"] = fs.make<TH1F>( "muEta",    "Muon #eta",                50,   -3.0,  3.0);
   histograms["muPhi"] = fs.make<TH1F>( "muPhi",    "Muon #phi",               50,  -3.1416,  3.1416 );

   histograms["massMuMu"]  = fs.make<TH1F>( "Invarient mass of two muons", "Invarient Mass", 150, 0, 150);
   histograms["massMuMu_0j"]  = fs.make<TH1F>( "Invarient mass of two muons (0 jets)", "Invarient Mass",  150, 0, 150);
   histograms["massMuMu_1j"]  = fs.make<TH1F>( "Invarient mass of two muon (1 jet) ", "Invarient Mass",  150, 0, 150);
   histograms["massMuMu_ge2j"]  = fs.make<TH1F>( "Invarient mass of two muon (>=2 jets)", "Invarient Mass",  150, 0, 150);

   histograms["Zmass"]  = fs.make<TH1F>( "Invarient Z mass", "Invarient Z Mass", 150, 0, 150);
   histograms["Zmass_0j"]  = fs.make<TH1F>( "Invarient Z mass(0 jets)", "Invarient Z Mass", 40, 70, 110);
   histograms["Zmass_1j"]  = fs.make<TH1F>( "Invarient mass of Z (1 jet) ", "Invarient Z Mass", 40, 70, 110);
   histograms["Zmass_ge2j"]  = fs.make<TH1F>( "Invarient mass of Z (>=2 jets)", "Invarient Z Mass", 40, 70, 110); 


   histograms["nJets"]      = fs.make<TH1F>( "nJets", "Number of Jets", maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);

   //Efficiency Plots

   histograms["Pt_Base"] = fs.make<TH1F>( "Pt_Base", "Pt_Base",  nPtBins, ptVals);
   histograms["Pt_Trig"] = fs.make<TH1F>( "Pt_Trig", "Pt_Trig",  nPtBins, ptVals);
   histograms["Pt_dR"] = fs.make<TH1F>( "Pt_dR", "Pt_dR",  nPtBins, ptVals);

   histograms["Eta_Base"] = fs.make<TH1F>( "Eta_Base", "Eta_Base",  etaBins, etaVals);
   histograms["Eta_Trig"] = fs.make<TH1F>( "Eta_Trig", "Eta_Trig",  etaBins, etaVals);
   histograms["Eta_dR"] = fs.make<TH1F>( "Eta_dR", "Eta_dR",  etaBins, etaVals);

   histograms["Phi_Base"] = fs.make<TH1F>( "Phi_Base", "Phi_Base",  phiBins, phiVals);
   histograms["Phi_Trig"] = fs.make<TH1F>( "Phi_Trig", "Phi_Trig",  phiBins, phiVals);
   histograms["Phi_dR"] = fs.make<TH1F>( "Phi_dR", "Phi_dR",  phiBins, phiVals);
   
   histograms["Pt Efficiency"] = fs.make<TH1F>( "Pt Efficiency", "Efficiency in Pt",  nPtBins, ptVals);
   histograms["Eta Efficiency"] = fs.make<TH1F>( "Eta Efficiency", "Efficiency in Eta", etaBins, etaVals); 
   histograms["Phi Efficiency"] = fs.make<TH1F>( "Phi Efficiency", "Efficiency in Phi",  phiBins, phiVals);

   histograms["Pt Trigger Efficiency"] = fs.make<TH1F>( "Pt Trigger Efficiency", "Trigger Efficiency in Pt",  nPtBins, ptVals);
   histograms["Eta Trigger Efficiency"] = fs.make<TH1F>( "Eta Trigger Efficiency", "Trigger Efficiency in Eta", etaBins, etaVals); 
   histograms["Phi Trigger Efficiency"] = fs.make<TH1F>( "Phi Trigger Efficiency", "Trigger Efficiency in Phi",  phiBins, phiVals);
   
   
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
   cout << "Cuts defined:" << endl;
   wPlusJets.printActiveCuts(cout);
     
     
   //Some other variables used in the loop
   int nInclusive = 0;
   int nPassed = 0;
   int nDumped = 0;
   int nTotal  = 0;
   int maxToDump = shyftParameters.getParameter<int>("maxDump");
   int maxInclusive = shyftParameters.getParameter<int>("maxInclusive");
   int maxPassed = shyftParameters.getParameter<int>("maxPassed");
   int printEvery = shyftParameters.getParameter<int>("printEvery");

   bool debugOutput = shyftParameters.getParameter<bool>("debugOutput");

   edm::InputTag muonInputTag (shyftParameters.getParameter<edm::InputTag> ("muonSrc"));
   edm::InputTag pvInputTag (shyftParameters.getParameter<edm::ParameterSet> ("muonIdTight").getParameter<edm::InputTag> ("pvSrc"));
   edm::InputTag trigTag(shyftParameters.getParameter<edm::InputTag>("trigSrc"));        

   double numProbes_baseCuts = 0.0;
   double numProbes_trackerHits = 0.0;
   //double numProbes_muonHits = 0.0;
   double numProbes_pixelLayerHits = 0.0;
   double numProbes_numSegments = 0.0;
   double numProbes_d0 = 0.0;
   double numProbes_primaryVertex = 0.0;
   double numProbes_isolation = 0.0;
   double numProbes_dR = 0.0;
   double numProbes_trigger = 0.0;


   ///////////////////////////////////////
   //
   //
   //             Event Loop
   //
   //
   ///////////////////////////////////////



   //loop through each event
   cout << "Beginning event loop." << endl;
   for( ev.toBegin();
        ! ev.atEnd();
        ++ev, ++nTotal) {

        
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

     //Check whether the event passes
     ret.set(false);
     bool passed = wPlusJets(ev, ret);

     if (ret[std::string("Inclusive")]) ++nInclusive;

     //Since we only care about selected events, why make this more
     //complicated than it has to be?
     if (!passed) continue;

     ++nPassed;
    
 
     //Determine if this is a muon event from the generator level
     //fwlite::Handle<reco::GenParticleCollection> genParticles;
     //genParticles.getByLabel(ev,"prunedGenParticles");
     
    std::vector<reco::ShallowClonePtrCandidate> const & muons     = wPlusJets.selectedMuons();
    std::vector<reco::ShallowClonePtrCandidate> const & jets      = wPlusJets.cleanedJets();
    reco::ShallowClonePtrCandidate   const & met             = wPlusJets.selectedMET();

    histograms["nJets"]->Fill( jets.size() );

    int runNumber = ev.id().run();
    string triggerName = "HLT_Mu15_v1";
    
    if (runNumber < 147196) {
      triggerName = "HLT_Mu9";
    }
        
    if (debugOutput) cout << "Run is " << runNumber << " looking for trigger "
                          << triggerName << endl;


    // only proceed if you have at least one tag  muon
    if (muons.size() > 0) {

      if (debugOutput) cout << "=======New event======" << endl
                            << "Found a tag muon candidate... " << endl;
      
      //Muon-only plots

      // Your tag muon is the highest-pt muon
      // selected by the WPlusJetsEvent 


      MuonVPlusJetsIDSelectionFunctor muonIdTight_
        (shyftParameters.getParameter<edm::ParameterSet>("muonIdTight"));


      // Get the PV for the muon z requirement
      fwlite::Handle<std::vector<reco::Vertex> > pvtxHandle;
      pvtxHandle.getByLabel( ev, pvInputTag.label().c_str());
      
      double zvtx = -999;
      if ( pvtxHandle->size() > 0 ) {
        zvtx = pvtxHandle->at(0).z();
      } else {
        throw cms::Exception("InvalidInput") << " There needs to be at least one primary vertex in the event." << std::endl;
      }

      
      std::vector< std::pair<const pat::Muon*, const pat::Muon*> > tagProbePairs;



      
      fwlite::Handle< vector< pat::Muon > > muonHandle;
      muonHandle.getByLabel (ev, muonInputTag.label().c_str());
      

      int nMuons = -1;

      double minPt = 20.0;
      double maxEta = 2.1;
      double maxChi2 = 10;
      
      for ( std::vector<pat::Muon>::const_iterator muonBegin = muonHandle->begin(),
              muonEnd = muonHandle->end(), imuon = muonBegin;
            imuon != muonEnd; ++imuon ) {
        nMuons++;

        int nValidMuonHits = 0;
        if (imuon->isGlobalMuon())
          nValidMuonHits = (int) imuon->globalTrack()->hitPattern().numberOfValidMuonHits();
        
        // If it's not even a reasonable muon, then let's skip it
        if (! (imuon->isGlobalMuon()
               && imuon->isTrackerMuon()
               && imuon->pt() > minPt
               && nValidMuonHits > 0
               && fabs(imuon->eta()) < maxEta
               && imuon->normChi2() < maxChi2 ) )
          continue;


        bool passTight = muonIdTight_(*imuon, ev);

        

        double minTrigDR = matchToTrigger (ev, (*imuon), trigTag, triggerName);


        bool matchTrig = (minTrigDR < 0.2) ? true : false ;

        if ( debugOutput ) {
          if ( passTight && (!matchTrig) ) {
            cout << "Passed cuts, but didn't match trigger! Min delta R is " << minTrigDR << endl;
          }
        }
          

        const pat::Muon* tagLeg = 0 ;
        const pat::Muon* probeLeg = 0 ;
        
        if (passTight && matchTrig) {
          tagLeg = &(*imuon);

          if (debugOutput) cout << "Found one good tag leg, looking for a probe leg... " << endl;
          
          // now loop over every other muon
          //

          for ( std::vector<pat::Muon>::const_iterator jmuon = muonHandle->begin();
            jmuon != muonEnd; ++jmuon ) {


            // don't use the same muon
            if (jmuon == imuon) continue;

            int jValidMuonHits = 0;
            if (imuon->isGlobalMuon())
              jValidMuonHits = (int) imuon->globalTrack()->hitPattern().numberOfValidMuonHits();

            
            // make sure the muon has a minimum quality
            
            
            if (! (jmuon->isGlobalMuon()
                   && jmuon->isTrackerMuon()
                   && jValidMuonHits > 0
                   && jmuon->pt() > minPt
                   && fabs(jmuon->eta()) < maxEta
                   && jmuon->normChi2() < maxChi2 ) )
              continue;


            // take the  tag + probe pair, see what the mass is
           
            reco::Particle::LorentzVector mumuLV = tagLeg->p4() + jmuon->p4();

            //Invariant mass of the Mu Mu pair
             histograms["massMuMu"]->Fill(mumuLV.M());
             if(jets.size()==0){
             histograms["massMuMu_0j"]->Fill(mumuLV.M());
               }
             if(jets.size()==1){
             histograms["massMuMu_1j"]->Fill(mumuLV.M());
               }
             if(jets.size()>=2){
             histograms["massMuMu_ge2j"]->Fill(mumuLV.M());
               }

            
            // if the mass is within the window,
            // put this pair on the list 
            if ( mumuLV.M() < (91.19 + 15)
                 && mumuLV.M() > (91.19 - 15) ) {

              if (debugOutput) cout << "Found a pair with mass " << mumuLV.M()  << endl;
              
              probeLeg = &(*jmuon);              

              tagProbePairs.push_back(std::pair<const pat::Muon*, const pat::Muon*>(tagLeg, probeLeg));
              
              break;
            }

            

            
          }// end for each other muon


        }//end if pass tight

        
      } // end for each muon
      


      if (debugOutput) cout  << "Number of tag probe pairs is " << tagProbePairs.size() << endl;

      



      // Foreach tag and probe pair
      std::vector< std::pair<const pat::Muon*, const pat::Muon*> >::const_iterator iPair;
      for (iPair = tagProbePairs.begin();
           iPair != tagProbePairs.end();
           iPair++){


        // first leg is tag, second leg is probe

        const pat::Muon * iTagLeg = iPair->first;
        const pat::Muon * iProbeLeg = iPair->second;

        // Calculate reliso, then cut on it.
        //
        
        double hcalIso = iProbeLeg->hcalIso();
        double ecalIso = iProbeLeg->ecalIso();
        double trkIso  = iProbeLeg->trackIso();
        double pt      = iProbeLeg->pt();
        double relIso  = (hcalIso + ecalIso + trkIso) / pt;

        double eta     = iProbeLeg->eta();
        double phi     = iProbeLeg->phi();

         unsigned stationMask = iProbeLeg->stationMask();
         unsigned DTMASK = 0xF;
         unsigned CSCMASK = 0xF0;
         unsigned DTbits = stationMask & DTMASK;
         unsigned CSCBits = (stationMask & CSCMASK) >> 4 ;
         unsigned nDTStations = 0;
         unsigned nCSCStations = 0;
          
         // Get masks for the segments
         // each bit is a station in the word
         // 0,1,2,3 = DT station 1, 2, 3, 4
         // 4,5,6,7 = CSC station 1, 2, 3, 4
          
         // Count the bits!
          
         for (nDTStations = 0; DTbits > 0; DTbits >>= 1) {
           nDTStations += (DTbits & 0x1);
         }
          
         // Count the bits!
          
         for (nCSCStations = 0; CSCBits > 0; CSCBits >>= 1) {
           nCSCStations += (CSCBits & 0x1);
         }

         reco::TrackRef globalTrack = iProbeLeg->globalTrack();
         if (!globalTrack.isNonnull()){
              cout << "Oh no! You have a muon that doesn't have a global track :("  << endl;
              
              cout << "Skipping the rest of the event" << endl;

              continue;
            }

        reco::HitPattern globalTrackHits = globalTrack->hitPattern();
        int numberOfMuonHits = globalTrackHits.numberOfValidMuonHits();
        int numberOfStripHits = globalTrackHits.numberOfValidTrackerHits();
        int numberOfPixelLayersWithMeasurement = globalTrackHits.pixelLayersWithMeasurement();
        int missingTrackerHits = globalTrackHits.numberOfLostStripHits();
          


        double deltaZ = iProbeLeg->vertex().z() - zvtx;
        
        double minDeltaR = 9999;
        
        for (unsigned iJet = 0; iJet < jets.size(); iJet++) {
          double iJetDeltaR = reco::deltaR ( iProbeLeg->eta(), iProbeLeg->phi(),
                                             jets[iJet].eta(), jets[iJet].phi() ) ;

          if (iJetDeltaR < minDeltaR)
            minDeltaR = iJetDeltaR;
        }


        double minTrigDR = matchToTrigger (ev, (*iProbeLeg), trigTag, triggerName);
       


        // probe leg Histograms
       histograms["hcalIso"]->Fill(hcalIso);
       histograms["ecalIso"]->Fill(ecalIso);
       histograms["trkIso"]->Fill(trkIso);
       histograms["relIso"]->Fill(relIso);
       histograms["nDTStations"]->Fill(nDTStations);
       histograms["nCSCStations"]->Fill(nCSCStations);
       histograms["nTotalStations"]->Fill(nDTStations+nCSCStations);
       histograms["numberOfMuonHits"]->Fill(numberOfMuonHits);
       histograms["numberOfStripHits"]->Fill(numberOfStripHits);
       histograms["numberOfPixelLayersWithMeasurement"]->Fill(numberOfPixelLayersWithMeasurement); 
       histograms["missingTrackerHits"]->Fill(missingTrackerHits); 
       histograms["minTrigDR"]->Fill(minTrigDR);
       histograms["minJetDR"]->Fill(minDeltaR);  //Not sure if this is filled right

       histograms["muPt"]->Fill(pt);
       histograms["muEta"]->Fill(eta);
       histograms["muPhi"]->Fill(phi);


       //ZMass
       double ZMass;
       reco::Particle::LorentzVector ZLV = iProbeLeg->p4() + iTagLeg->p4();
       double probeCharge = iProbeLeg->charge();
       double tagCharge = iTagLeg->charge();
       //cout << "tag charge " << tagCharge << "  probe charge " << probeCharge << endl;
         if ( ZLV.M() < (91.19 + 15)
                 && ZLV.M() > (91.19 - 15) ) {
           if(probeCharge != tagCharge){
             ZMass = ZLV.M();
           }//end opposite charge cut
         }//end Z mass cut
       
       //ZMass Plots
         histograms["Zmass"]->Fill(ZMass);
         if(jets.size()==0){
         histograms["Zmass_0j"]->Fill(ZMass);
         }
         if(jets.size()==1){
         histograms["Zmass_1j"]->Fill(ZMass);
         }
         if(jets.size()>=2){
         histograms["Zmass_ge2j"]->Fill(ZMass);  
         }


     
        
        
        // probe leg is denominator

        numProbes_baseCuts++;
        histograms["Pt_Base"]->Fill(pt);
        histograms["Eta_Base"]->Fill(eta);
        histograms["Phi_Base"]->Fill(phi);

        

        
        // numerator is probes passing cuts

        if (iProbeLeg->numberOfValidHits() > 10){
          
          numProbes_trackerHits++;
          
          if (iProbeLeg->innerTrack()->hitPattern().pixelLayersWithMeasurement() >=1){
            
            numProbes_pixelLayerHits++;
            
            if (iProbeLeg->numberOfMatches() > 1){
              
              numProbes_numSegments++;
              
              if (fabs(iProbeLeg->dB()) < 0.02) {
                
                numProbes_d0++;
                
                if ( fabs(deltaZ) < 1 ) {
                  
                  numProbes_primaryVertex++;

                  if ( relIso < 0.05 ) {

                    numProbes_isolation++;

                    if ( minDeltaR > 0.3 ) {
                      numProbes_dR++;
                      histograms["Pt_dR"]->Fill(pt);
                      histograms["Eta_dR"]->Fill(eta);
                      histograms["Phi_dR"]->Fill(phi);

                      if ( minTrigDR < 0.2 ) {

                        numProbes_trigger++;
                        histograms["Pt_Trig"]->Fill(pt);
                        histograms["Eta_Trig"]->Fill(eta);
                        histograms["Phi_Trig"]->Fill(phi);
                      }// end if trigger
                    } //end if minDeltaR
                  }// end if iso 
                }//end if zvtx
              }// end if d0
            }// end if num stations            
          }// end if pixel layers
        }//end if number of valid hits
        
        
      }// end for each tag and probe pair
    }// end if at least one muon 

    // Efficiency Plots
    moveOverflow(histograms["Pt_Base"]);
    moveOverflow(histograms["Eta_Base"]);
    moveOverflow(histograms["Phi_Base"]);
    moveOverflow(histograms["Pt_dR"]);
    moveOverflow(histograms["Eta_dR"]);
    moveOverflow(histograms["Phi_dR"]);
    moveOverflow(histograms["Pt_Trig"]);
    moveOverflow(histograms["Eta_Trig"]);
    moveOverflow(histograms["Phi_Trig"]);

    //generic_eff(denominator, numerator, final eff Hist)
    generic_eff(histograms["Pt_Base"],histograms["Pt_dR"],histograms["Pt Efficiency"]);
    generic_eff(histograms["Eta_Base"],histograms["Eta_dR"],histograms["Eta Efficiency"]);
    generic_eff(histograms["Phi_Base"],histograms["Phi_dR"],histograms["Phi Efficiency"]);

    generic_eff(histograms["Pt_dR"],histograms["Pt_Trig"],histograms["Pt Trigger Efficiency"]);
    generic_eff(histograms["Eta_dR"],histograms["Eta_Trig"],histograms["Eta Trigger Efficiency"]);
    generic_eff(histograms["Phi_dR"],histograms["Phi_Trig"],histograms["Phi Trigger Efficiency"]);
        

    if (nDumped < maxToDump) {
      std::cout << "-->NJets = " << std::setw(3) << jets.size()
                << std::endl;
        
      ++nDumped;
    }



    
   } //end event loop
   
   cout << "Printing" << endl;
   wPlusJets.print(std::cout);



   cout << "Number of tag+probe pairs     " << numProbes_baseCuts
        << "  (eff  " << (numProbes_baseCuts / numProbes_baseCuts) << ")" << endl
     
        << "Number passing tracker hits   " << numProbes_trackerHits
        << "  (eff  " << (numProbes_trackerHits / numProbes_baseCuts) << ")" << endl
     
        << "Pixel hits                    " << numProbes_pixelLayerHits 
        << "  (eff  " << (numProbes_pixelLayerHits / numProbes_trackerHits) << ")" << endl
     
        << "Num segments                  " << numProbes_numSegments 
        << "  (eff  " << (numProbes_numSegments / numProbes_pixelLayerHits) << ")" << endl
     
        << "D0                            " << numProbes_d0 
        << "  (eff  " << (numProbes_d0 / numProbes_numSegments) << ")" << endl

        << "delta Z < 1                   " << numProbes_primaryVertex 
        << "  (eff  " << (numProbes_primaryVertex / numProbes_d0) << ")" << endl

        << "isolation                   " << numProbes_isolation 
        << "  (eff  " << (numProbes_isolation / numProbes_primaryVertex) << ")" << endl
     
        << "dR                          " << numProbes_dR 
        << "  (eff  " << (numProbes_dR / numProbes_isolation) << ")" << endl

        << "Triger                      " << numProbes_trigger 
        << "  (eff  " << (numProbes_trigger / numProbes_dR ) << ")" << endl


        << endl;

   cout << "Total Efficiency   =     " << ( numProbes_dR /numProbes_baseCuts )
        << binomialErrorString(numProbes_baseCuts, numProbes_dR) << endl;

   cout << "Trigger Efficiency =     " << (numProbes_trigger / numProbes_dR)
        << binomialErrorString (numProbes_dR, numProbes_trigger) << endl;
  
   cout << "We're done!" << endl;
   cout << "Processed " << nTotal << " events." << endl
        << "Passed " << nPassed << " events." << endl;  

  
   return 0;
}


std::string binomialErrorString (double denomVal, double numerVal) {

  double effVal = numerVal / denomVal;
  double errVal = (denomVal && (effVal <=1)) ? sqrt(effVal*(1-effVal)/denomVal) : 0;


  stringstream errString;

  errString << " +/- " << errVal;

  return errString.str();
  
}

void generic_eff (TH1* denom, TH1* numer, TH1* efficiencyHist) {
  for (int iBinX = 1; iBinX < denom->GetNbinsX()+1; iBinX++){
    for (int iBinY = 1; iBinY < denom->GetNbinsY()+1; iBinY++){
      for (int iBinZ = 1; iBinZ < denom->GetNbinsZ()+1; iBinZ++){

        int globalBinNum = denom->GetBin(iBinX, iBinY, iBinZ);
        
               
        float numerVal = numer->GetBinContent(globalBinNum);
        float denomVal = denom->GetBinContent(globalBinNum);

        float effVal = 0;


        effVal = denomVal ? numerVal / denomVal : 0;


        float errVal = (denomVal && (effVal <=1)) ? sqrt(effVal*(1-effVal)/denomVal) : 0;

        
        efficiencyHist->SetBinContent(globalBinNum, effVal);
        efficiencyHist->SetBinError(globalBinNum, errVal);
      }
    }
  }

  efficiencyHist->SetMinimum(0.001);
  efficiencyHist->SetMaximum(1.1);
}

void moveOverflow (TH1 * myElement) {

  // This will handle an arbitrary dimension first/last bin
  // but you should think about how you will interpret this for
  // 2D/3D histos
  // Actually, this can't handle abitrary dimensions.
  int maxBin = myElement->GetNbinsX();

  double originalEntries = myElement->GetEntries();
  
  myElement->SetBinContent(1, myElement->GetBinContent(0) + myElement->GetBinContent(1));

  myElement->SetBinContent(0,0.0);

  myElement->SetBinContent(maxBin,myElement->GetBinContent(maxBin) + myElement->GetBinContent(maxBin+1));

  myElement->SetBinContent(maxBin+1,0.0);

  myElement->SetEntries(originalEntries);

}


double binomialError (double denomVal, double numerVal) {

  double effVal = numerVal / denomVal;
  double errVal = (denomVal && (effVal <=1)) ? sqrt(effVal*(1-effVal)/denomVal) : 0;

  return errVal;
  
}


double matchToTrigger (fwlite::ChainEvent ev , pat::Muon imuon, edm::InputTag trigTag, std::string triggerName) {

                       
  //Check whether the muon is matched to HLT path
  
  edm::Handle<pat::TriggerEvent> triggerEvent;
  ((edm::EventBase *)&ev)->getByLabel(trigTag, triggerEvent);
  pat::TriggerEvent const * trig = &*triggerEvent;




        
  pat::TriggerObjectRefVector trigObj = trig->pathObjects(triggerName);
  double minTrigDR = 9e20;

          
  for ( pat::TriggerObjectRefVector::const_iterator it = trigObj.begin(); it != trigObj.end(); ++it ) {
    const pat::TriggerObjectRef objRef(*it);
          
    if (objRef->collection().find("hltL3MuonCandidates") != std::string::npos) {
      double dR = reco::deltaR(imuon.eta(),imuon.phi(),objRef->eta(),objRef->phi());
      if (dR < minTrigDR) minTrigDR = dR;
    }
  }

  return minTrigDR;
}
