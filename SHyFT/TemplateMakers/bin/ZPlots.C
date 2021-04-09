#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
// #include "DataFormats/FWLite/interface/Run.h"
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

using namespace std;

reco::Particle reCalculateMetWithShiftedJets
( edm::EventBase const & event, edm::ParameterSet const & setupParameters );


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
   PythonProcessDesc builder(argv[1]);
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

   // book a set of histograms
   cout << "Booking histograms." << endl;
   fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );

   //Book the histograms
   std::map<TString, TH1*> histograms;

   bool muPlusJets_ = shyftParameters.getParameter<bool>("muPlusJets");
   bool ePlusJets_  = shyftParameters.getParameter<bool>("ePlusJets");

   if (muPlusJets_) cout << "Running muon event selection and plots." << endl;
   if (ePlusJets_)  cout << "Running electron event selection and plots." << endl;

   //Determine the jet bins we're using
   int minNJets = shyftParameters.getParameter<int>("minJets");
   int maxNJets = shyftParameters.getParameter<int>("maxJets");
   float minNJetsFloat = (float)minNJets - 0.5;
   float maxNJetsFloat = (float)maxNJets + 0.5;

   cout << "Minimum number of jets = " << minNJets << endl;
   cout << "Maximum number of jets = " << maxNJets << endl;

   if( muPlusJets_ ) {
    
      histograms["muPt"] = fs.make<TH1F>( "muPt",     "Muon p_{T} (GeV/c) ",     200, 0, 200 );
      histograms["muPtPlus"] = fs.make<TH1F>( "muPtPlus",     "Muon (+) p_{T} (GeV/c) ",     200, 0, 200 );
      histograms["muPtMinus"] = fs.make<TH1F>( "muPtMinus",     "Muon (-) p_{T} (GeV/c) ",     200, 0, 200 );
      histograms["muEta"] = fs.make<TH1F>( "muEta",    "Muon #eta",                50,   -3.0,  3.0);
      histograms["muPhi"] = fs.make<TH1F>( "muPhi",    "Muon #phi",               50,  -3.1416,  3.1416 );

      //Split some of the above plots by jet bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "muPt_"; histName += iJet; histName += "j";
         TString histTitle = "Muon p_{T} (GeV/c) (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";

         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0., 200.);  
      }

      //Split some of the above plots by jet bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "muEta_"; histName += iJet; histName += "j";
         TString histTitle = "Muon #eta (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";

         histograms[histName] = fs.make<TH1F>(histName, histTitle, 600, -3.0, 3.0);
      }

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
    
   }//muPlusJets_

   if ( ePlusJets_ ) {
     
      histograms["ePt_EE"] = fs.make<TH1F>( "ePt_EE",  "Electron p_{T} (GeV/c) in EE", 200,  0,   200 );
      histograms["ePtPlus_EE"] = fs.make<TH1F>( "ePtPlus_EE",     "Electron (+) p_{T} (GeV/c) in EE",   50, 0, 100 );
      histograms["ePtMinus_EE"] = fs.make<TH1F>( "ePtMinus_EE",     "Electron (-) p_{T} (GeV/c) in EE",   50, 0, 100 );
      histograms["eEta_EE"] = fs.make<TH1F>( "eEta_EE",  "Electron #eta in EE",   50,   -3.0,   3.0);
      histograms["ePhi_EE"] = fs.make<TH1F>( "ePhi_EE",  "Electron #phi in EE",   50, -3.1416, 3.1416);
     
      //Split some of the above plots by jet bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "ePt_"; histName += iJet; histName += "j_EE";
         TString histTitle = "Electron p_{T} (GeV/c) (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EE";

         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0., 200.);  
      }
     
      //Split some of the above plots by jet bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "eEta_"; histName += iJet; histName += "j_EE";
         TString histTitle = "Electron #eta (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EE";
        
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 600, -3.0, 3.0);
      }
  
      histograms["ePt_EB"] = fs.make<TH1F>( "ePt_EB",  "Electron p_{T} (GeV/c) in EB ", 200,  0,   200 );
      histograms["ePtPlus_EB"] = fs.make<TH1F>( "ePtPlus_EB",     "Electron (+) p_{T} (GeV/c) in EB",   50, 0, 100 );
      histograms["ePtMinus_EB"] = fs.make<TH1F>( "ePtMinus_EB",     "Electron (-) p_{T} (GeV/c) in EB",   50, 0, 100 );
      histograms["eEta_EB"] = fs.make<TH1F>( "eEta_EB",  "Electron #eta in EB",   50,   -3.0,   3.0);
      histograms["ePhi_EB"] = fs.make<TH1F>( "ePhi_EB",  "Electron #phi in EB",   50, -3.1416, 3.1416);
     
     
      //Split some of the above plots by jet bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "ePt_"; histName += iJet; histName += "j_EB";
         TString histTitle = "Electron p_{T} (GeV/c) (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EB";
        
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0., 200.);  
      }
     
      //Split some of the above plots by jet bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "eEta_"; histName += iJet; histName += "j_EB";
         TString histTitle = "Electron #eta (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EB";
        
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 600, -3.0, 3.0);
      }
   }  // end  ePlusJets_
  

   //Jets plots (the same whether this is an electron or muon event)

   //Jet counts
   histograms["nJets"]      = fs.make<TH1F>( "nJets", "Number of Jets", maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);
   histograms["nJetsPlus"]  = fs.make<TH1F>( "nJetsPlus",    "Number of Jets (l+)", maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);
   histograms["nJetsMinus"] = fs.make<TH1F>( "nJetsMinus",    "Number of Jets (l-)", maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);

   histograms["nJetsBeforeClean"] = fs.make<TH1F>( "nJetsBeforeClean", "Number of Jets before Clean", 
                                                   maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);

   //Individual jet plots
   for (int iJet = 1; iJet <= maxNJets; ++iJet) {
      TString histName = "jet"; histName += iJet;
      TString histTitle = ""; histTitle += iJet;
      switch (iJet) {
         case 1:
            histTitle += "st";
            break;
         case 2:
            histTitle += "nd";
            break;
         case 3:
            histTitle += "rd";
            break;
         default:
            histTitle += "th";
            break;
      }
      histTitle += " Leading Jet";
    
      histograms[histName + "Pt"] = fs.make<TH1F>( histName+"Pt",   histTitle + " p_{T}", 300, 0, 300);
      histograms[histName + "Eta"] = fs.make<TH1F>( histName+"Eta",  histTitle + " #eta", 50, -3.0,   3.0);
      histograms[histName + "Phi"] = fs.make<TH1F>( histName+"Phi",  histTitle + " #phi", 50, -3.1416,   3.1416);
      histograms[histName + "Et"] = fs.make<TH1F>( histName+"Et",   histTitle + " E_{T}", 300, 0, 300);
      histograms[histName + "Mass"] = fs.make<TH1F>( histName+"Mass", histTitle + " Mass", 100, 0, 100);

      TString histNameDPhiMet = "dPhiMetJet"; histNameDPhiMet += iJet;
      TString histTitleDPhiMet = "#Delta #phi (MET - "; histTitleDPhiMet += iJet;
      switch (iJet) {
         case 1:
            histTitleDPhiMet += "st";
            break;
         case 2:
            histTitleDPhiMet += "nd";
            break;
         case 3:
            histTitleDPhiMet += "rd";
            break;
         default:
            histTitleDPhiMet += "th";
            break;
      }
      histTitleDPhiMet += " Leading Jet)";

      histograms[histNameDPhiMet] = fs.make<TH1F>( histNameDPhiMet, histTitleDPhiMet, 100, 0, 3.1416);


      //Now, get really crazy and separate the above histograms by jet bin (i.e. Jet1Pt_1j, Jet1Pt_2j, etc.)
      for (int jJet = iJet; jJet <= maxNJets; ++jJet) {
         TString histName2 = histName + "Pt_"; histName2 += jJet; histName2 += "j";
         TString histTitle2 = histTitle + " p_{T} ("; 
         histTitle2 += ((jJet == maxNJets) ? "#geq" : "=");
         histTitle2 += jJet;
         histTitle2 += "-Jet Bin)";
         histograms[histName2] = fs.make<TH1F>(histName2, histTitle2, 300, 0., 300.);
      }
    
      for (int jJet = iJet; jJet <= maxNJets; ++jJet) {
         TString histName2 = histName + "Eta_"; histName2 += jJet; histName2 += "j";
         TString histTitle2 = histTitle + " #eta ("; 
         histTitle2 += ((jJet == maxNJets) ? "#geq" : "=");
         histTitle2 += jJet;
         histTitle2 += "-Jet Bin)";
         histograms[histName2] = fs.make<TH1F>(histName2, histTitle2, 600, -3, 3);
      }
    
      for (int jJet = iJet; jJet <= maxNJets; ++jJet) {
         TString histName2 = histName + "Phi_"; histName2 += jJet; histName2 += "j";
         TString histTitle2 = histTitle + " #phi ("; 
         histTitle2 += ((jJet == maxNJets) ? "#geq" : "=");
         histTitle2 += jJet;
         histTitle2 += "-Jet Bin)";
         histograms[histName2] = fs.make<TH1F>(histName2, histTitle2, 100, -3.1416, 3.1416);
      }
    
      for (int jJet = iJet; jJet <= maxNJets; ++jJet) {
         TString histName2 = histName + "Et_"; histName2 += jJet; histName2 += "j";
         TString histTitle2 = histTitle + " E_{T} ("; 
         histTitle2 += ((jJet == maxNJets) ? "#geq" : "=");
         histTitle2 += jJet;
         histTitle2 += "-Jet Bin)";
         histograms[histName2] = fs.make<TH1F>(histName2, histTitle2, 300, 0, 300);
      }
    
      for (int jJet = iJet; jJet <= maxNJets; ++jJet) {
         TString histName2 = histName + "Mass_"; histName2 += jJet; histName2 += "j";
         TString histTitle2 = histTitle + " Mass ("; 
         histTitle2 += ((jJet == maxNJets) ? "#geq" : "=");
         histTitle2 += jJet;
         histTitle2 += "-Jet Bin)";
         histograms[histName2] = fs.make<TH1F>(histName2, histTitle2, 100, 0, 100);
      }

      for (int jJet = iJet; jJet <= maxNJets; ++jJet) {
         TString histName2 = histNameDPhiMet+"_"; histName2 += jJet; histName2 += "j";
         TString histTitle2 = histTitleDPhiMet + " ("; 
         histTitle2 += ((jJet == maxNJets) ? "#geq" : "=");
         histTitle2 += jJet;
         histTitle2 += "-Jet Bin)";
         histograms[histName2] = fs.make<TH1F>(histName2, histTitle2, 100, 0, 3.1416);
      }

   }//iJet
  
   //=========================
   //        HT plots
   //=========================

   //Our conventional way ht = lepPt + sum(jetEt)
 
   if( muPlusJets_ ) {  
      //All Jet Bins
      histograms["ht"] = fs.make<TH1F> ("ht", "H_{T}", 100, 0, 1200);
      histograms["htPlus"] = fs.make<TH1F> ("htPlus", "H_{T} (Mu+)", 100, 0, 1200);
      histograms["htMinus"] = fs.make<TH1F> ("htMinus", "H_{T} (Mu-)", 100, 0, 1200);
      histograms["htVsNJet"] = fs.make<TH2F> ("htVsNJet", ";N_{jets};H_{T}", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200);
     
      //Separated by Jet Bin
      //----- Use separate HT histograms
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "ht_"; histName += iJet; histName += "j";
         TString histTitle = "H_{T} (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";
        
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
        
      }
   }//muPlusJets_
  
   if ( ePlusJets_ ) {
      //All Jet Bins
      histograms["ht_EE"] = fs.make<TH1F> ("ht_EE", "H_{T} in EE", 100, 0, 1200);
      histograms["htPlus_EE"] = fs.make<TH1F> ("htPlus_EE", "H_{T} (e+) in EE", 100, 0, 1200);
      histograms["htMinus_EE"] = fs.make<TH1F> ("htMinus_EE", "H_{T} (e-) in EE", 100, 0, 1200);
      histograms["htVsNJet_EE"] = fs.make<TH2F> ("htVsNJet_EE", ";N_{jets};H_{T} in EE", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200);
     
      histograms["ht_EB"] = fs.make<TH1F> ("ht_EB", "H_{T} in EB", 100, 0, 1200);
      histograms["htPlus_EB"] = fs.make<TH1F> ("htPlus_EB", "H_{T} (e+) in EB", 100, 0, 1200);
      histograms["htMinus_EB"] = fs.make<TH1F> ("htMinus_EB", "H_{T} (e-) in EB", 100, 0, 1200);
      histograms["htVsNJet_EB"] = fs.make<TH2F> ("htVsNJet_EB", ";N_{jets};H_{T} in EB", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200);
     
      //Separated by Jet Bin
      //----- Use separate HT histograms
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "ht_"; histName += iJet; histName += "j_EE";
         TString histTitle = "H_{T} (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EE";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 50, 0., 1200.);
      }
     
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "ht_"; histName += iJet; histName += "j_EB";
         TString histTitle = "H_{T} (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EB";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 50, 0., 1200.);
      }
   }//ePlusJets_
  
   // Book new histos
   // Fit bins of MET
   histograms["ht_ge3j_0met"] = fs.make<TH1F> ("ht_ge3j_0met", "H_{T} for met < 20, ge3j", 100, 0, 1200);
   histograms["ht_ge3j_1met"] = fs.make<TH1F> ("ht_ge3j_1met", "H_{T} for met > 20, ge3j", 100, 0, 1200);
  
   histograms["ht_ge4j_0met"] = fs.make<TH1F> ("ht_ge4j_0met", "H_{T} for met < 20, ge4j", 100, 0, 1200);
   histograms["ht_ge4j_1met"] = fs.make<TH1F> ("ht_ge4j_1met", "H_{T} for met > 20, ge4j", 100, 0, 1200);
  
  
   //Top group ht_lep = lepPt + MET
   //------------------------------
   if( muPlusJets_ ) { 
      //All Jet Bins
      histograms["htLep"] = fs.make<TH1F> ("htLep", "H_{T,Lep}", 100, 0, 1200);
      histograms["htLepPlus"] = fs.make<TH1F> ("htLepPlus", "H_{T,Lep} (Mu+)", 100, 0, 1200);
      histograms["htLepMinus"] = fs.make<TH1F> ("htLepMinus", "H_{T,Lep} (Mu-)", 100, 0, 1200);
      histograms["htLepVsNJet"] = fs.make<TH2F> ("htLepVsNJet", ";N_{jets};H_{T,Lep}", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200);
     
      //Separated by Jet Bin
      //----- Use separate HT histograms
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "htLep_"; histName += iJet; histName += "j";
         TString histTitle = "H_{T,Lep} (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";

         histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
      }
   }//muPlusJets_

   if ( ePlusJets_ ) {
      //All Jet Bins
      histograms["htLep_EE"] = fs.make<TH1F> ("htLep_EE", "H_{T,Lep} in EE", 100, 0, 1200);
      histograms["htLepPlus_EE"] = fs.make<TH1F> ("htLepPlus_EE", "H_{T,Lep} (e+) in EE", 100, 0, 1200);
      histograms["htLepMinus_EE"] = fs.make<TH1F> ("htLepMinus_EE", "H_{T,Lep} (e-) in EE", 100, 0, 1200);
      histograms["htLepVsNJet_EE"] = fs.make<TH2F> ("htLepVsNJet_EE", ";N_{jets};H_{T,Lep} in EE", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200);
     
      histograms["htLep_EB"] = fs.make<TH1F> ("htLep_EB", "H_{T,Lep} in EB", 100, 0, 1200);
      histograms["htLepPlus_EB"] = fs.make<TH1F> ("htLepPlus_EB", "H_{T,Lep} (e+) in EB", 100, 0, 1200);
      histograms["htLepMinus_EB"] = fs.make<TH1F> ("htLepMinus_EB", "H_{T,Lep} (e-) in EB", 100, 0, 1200);
      histograms["htLepVsNJet_EB"] = fs.make<TH2F> ("htLepVsNJet_EB", ";N_{jets};H_{T,Lep} in EB", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200);
     
      //Separated by Jet Bin
      //----- Use separate HT histograms
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "htLep_"; histName += iJet; histName += "j_EE";
         TString histTitle = "H_{T,Lep} (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EE";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 50, 0., 1200.);
      }
     
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "htLep_"; histName += iJet; histName += "j_EB";
         TString histTitle = "H_{T,Lep} (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EB";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 50, 0., 1200.);
      }
   }//ePlusJets_
  
  
   // Book new histos
   // Fit bins of MET
   histograms["htLep_ge3j_0met"] = fs.make<TH1F> ("htLep_ge3j_0met", "H_{T,Lep} for met < 20, ge3j", 100, 0, 1200);
   histograms["htLep_ge3j_1met"] = fs.make<TH1F> ("htLep_ge3j_1met", "H_{T,Lep} for met > 20, ge3j", 100, 0, 1200);
  
   histograms["htLep_ge4j_0met"] = fs.make<TH1F> ("htLep_ge4j_0met", "H_{T,Lep} for met < 20, ge4j", 100, 0, 1200);
   histograms["htLep_ge4j_1met"] = fs.make<TH1F> ("htLep_ge4j_1met", "H_{T,Lep} for met > 20, ge4j", 100, 0, 1200);
  
   //H_T including all the terms: lepPT + MET + sum(jetEt)
   //-----------------------------------------------------
   if( muPlusJets_ ) { 
      //All Jet Bins
      histograms["htAll"] = fs.make<TH1F> ("htAll", "H_{T,All}", 100, 0, 1200);
      histograms["htAllPlus"] = fs.make<TH1F> ("htAllPlus", "H_{T,All} (Mu+)", 100, 0, 1200);
      histograms["htAllMinus"] = fs.make<TH1F> ("htAllMinus", "H_{T,All} (Mu-)", 100, 0, 1200);
      histograms["htAllVsNJet"] = fs.make<TH2F> ("htAllVsNJet", ";N_{jets};H_{T,All}", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200); 
      //Separated by Jet Bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "htAll_"; histName += iJet; histName += "j";
         TString histTitle = "H_{T,All} (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);  
      }
   }//muPlusJets_

   if ( ePlusJets_ ) { 
      histograms["htAll_EE"] = fs.make<TH1F> ("htAll_EE", "H_{T,All} in EE", 100, 0, 1200);
      histograms["htAllPlus_EE"] = fs.make<TH1F> ("htAllPlus_EE", "H_{T,All} (e+) in EE", 100, 0, 1200);
      histograms["htAllMinus_EE"] = fs.make<TH1F> ("htAllMinus_EE", "H_{T,All} (e-) in EE", 100, 0, 1200);
      histograms["htAllVsNJet_EE"] = fs.make<TH2F> ("htAllVsNJet_EE", ";N_{jets};H_{T,All} in EE", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200);
     
      histograms["htAll_EB"] = fs.make<TH1F> ("htAll_EB", "H_{T,All} in EB", 100, 0, 1200);
      histograms["htAllPlus_EB"] = fs.make<TH1F> ("htAllPlus_EB", "H_{T,All} (e+) in EB", 100, 0, 1200);
      histograms["htAllMinus_EB"] = fs.make<TH1F> ("htAllMinus_EB", "H_{T,All} (e-) in EB", 100, 0, 1200);
      histograms["htAllVsNJet_EB"] = fs.make<TH2F> ("htAllVsNJet_EB", ";N_{jets};H_{T,All} in EB", maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 50, 0, 1200);

      //Separated by Jet Bin
      //----- Use separate HT histograms
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "htAll_"; histName += iJet; histName += "j_EE";
         TString histTitle = "H_{T,All} (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EE";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 50, 0., 1200.);
      }
     
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "htAll_"; histName += iJet; histName += "j_EB";
         TString histTitle = "H_{T,All} (N_{jets}";
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EB";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 50, 0., 1200.);
      }
   }//ePlusJets_  

  
   // Book new histos
   // for ht vs met
   histograms["htAll_ge3j_0met"] = fs.make<TH1F> ("htAll_ge3j_0met", "H_{T,All} for met < 20, ge3j", 100, 0, 1200);
   histograms["htAll_ge3j_1met"] = fs.make<TH1F> ("htAll_ge3j_1met", "H_{T,All} for met > 20, ge3j", 100, 0, 1200);
  
   histograms["htAll_ge4j_0met"] = fs.make<TH1F> ("htAll_ge4j_0met", "H_{T,All} for met < 20, ge4j", 100, 0, 1200);
   histograms["htAll_ge4j_1met"] = fs.make<TH1F> ("htAll_ge4j_1met", "H_{T,All} for met > 20, ge4j", 100, 0, 1200);
  
   //======================================
   //M2 = dijet mass of leading two jets
   //======================================
   //All Jet Bins
   histograms["m2"] = fs.make<TH1F> ("m2", "M2", 100, 0, 600);

   //Separated by Jet Bin
   for (int iJet = max(2,minNJets); iJet <= maxNJets; ++iJet) {
      TString histName = "m2_"; histName += iJet; histName += "j";
      TString histTitle = "M2 (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 600, 0., 600.);
   }

   //===============================
   //M3 = Mass of leading three jets
   //===============================
   //All Jet Bins
   histograms["m3"] = fs.make<TH1F> ("m3", "M3", 100, 0, 1200);

   //Separated by Jet Bin
   for (int iJet = max(3,minNJets); iJet <= maxNJets; ++iJet) {
      TString histName = "m3_"; histName += iJet; histName += "j";
      TString histTitle = "M3 (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 600, 0., 600.);
   }
  

   //==========
   //MET plots
   //==========
  
   
   if( muPlusJets_ ) {
      //All Jet Bins
      histograms["met"] = fs.make<TH1F> ("met", "MET", 50, 0, 200);
      histograms["metPlus"] = fs.make<TH1F> ("metPlus", "MET (mu+)", 50, 0, 200);
      histograms["metMinus"] = fs.make<TH1F> ("metMinus", "MET (mu-)", 50, 0,200);
      histograms["metVsNJet"] = fs.make<TH2F> ("metVsNJet", ";N_{jets};MET",
                                               maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 
                                               50, 0, 200);
      //Separated by Jet Bin
      //----- Use separate MET histograms
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "met_"; histName += iJet; histName += "j";
         TString histTitle = "MET (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";
        
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0., 200.);
      }
   }//muPlusJets_  
     
   if ( ePlusJets_ ) { 
      histograms["met_EE"] = fs.make<TH1F> ("met_EE", "MET in EE", 50, 0, 200);
      histograms["metPlus_EE"] = fs.make<TH1F> ("metPlus_EE", "MET (e+) in EE", 50, 0, 200);
      histograms["metMinus_EE"] = fs.make<TH1F> ("metMinus_EE", "MET (e-) in EE", 50, 0,200);
      histograms["metVsNJet_EE"] = fs.make<TH2F> ("metVsNJet_EE", ";N_{jets};MET in EE",
                                                  maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 
                                                  50, 0, 200);
        
      histograms["met_EB"] = fs.make<TH1F> ("met_EB", "MET in EE", 50, 0, 200);
      histograms["metPlus_EB"] = fs.make<TH1F> ("metPlus_EB", "MET (e+) in EE", 50, 0, 200);
      histograms["metMinus_EB"] = fs.make<TH1F> ("metMinus_EB", "MET (e-) in EE", 50, 0,200);
      histograms["metVsNJet_EB"] = fs.make<TH2F> ("metVsNJet_EB", ";N_{jets};MET in EE",
                                                  maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 
                                                  50, 0, 200);
      //Separated by Jet Bin
      //----- Use separate MET histograms
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "met_"; histName += iJet; histName += "j_EE";
         TString histTitle = "MET (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EE";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0., 200.);
      }
         
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "met_"; histName += iJet; histName += "j_EB";
         TString histTitle = "MET (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EB";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0., 200.);
      }
   }//ePlusJets_
     
  
   //================================================
   //   W Transverse Mass for RECO and GenParticles
   //================================================
     
   if( muPlusJets_ ) {
      //All jet bin 
      histograms["mT"]       = fs.make<TH1F>( "mT", "W_TTbar Transverse Mass Reco", 60, 0, 120);
      histograms["mT_Plus"]  = fs.make<TH1F>( "mT_Plus", "W_TTbar Transverse Mass Reco (mu+)", 60, 0, 120);
      histograms["mT_Minus"] = fs.make<TH1F>( "mT_Minus", "W_TTbar Transverse Mass Reco (mu-)", 60, 0, 120);
      //Separated by Jet Bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "mT_"; histName += iJet; histName += "j";
         TString histTitle = "M_{T} (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";     
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0, 200);  
      }
   }//muPlusJets_
     
   if ( ePlusJets_ ) { 
      //All jet bin 
      histograms["mT_EE"]       = fs.make<TH1F>( "mT_EE", "W_TTbar Transverse Mass Reco in EE", 60, 0, 120);
      histograms["mT_Plus_EE"]  = fs.make<TH1F>( "mT_Plus_EE", "W_TTbar Transverse Mass Reco (e+) in EE", 60, 0, 120);
      histograms["mT_Minus_EE"] = fs.make<TH1F>( "mT_Minus_EE", "W_TTbar Transverse Mass Reco (e-) in EE", 60, 0, 120);
        
      histograms["mT_EB"]       = fs.make<TH1F>( "mT_EB", "W_TTbar Transverse Mass Reco in EB", 60, 0, 120);
      histograms["mT_Plus_EB"]  = fs.make<TH1F>( "mT_Plus_EB", "W_TTbar Transverse Mass Reco (e+) in EB", 60, 0, 120);
      histograms["mT_Minus_EB"] = fs.make<TH1F>( "mT_Minus_EB", "W_TTbar Transverse Mass Reco (e-) in EB", 60, 0, 120);
      //Separated by Jet Bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "mT_"; histName += iJet; histName += "j_EE";
         TString histTitle = "M_{T} (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EE";     
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0, 200);  
      }
        
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "mT_"; histName += iJet; histName += "j_EB";
         TString histTitle = "M_{T} (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ") in EB";     
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0, 200);  
      }
   }//ePlusJets_
     
   //====================
   // dPhiMetLep plots
   //====================
   histograms["dPhiMetLep"] = fs.make<TH1F>("dPhiMetLep","#Delta #phi (MET - Lepton)", 100, 0, 3.1416);
     
   //Separated by Jet Bin
   for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
      TString histName = "dPhiMetLep_"; histName += iJet; histName += "j";
      TString histTitle = "#Delta #phi (MET - Lepton) (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
        
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 100, 0, 3.1416);
        
   }
     
   //==============
   // Gen MC stuff 
   //==============
    
   histograms["GenWTransverseMass"] = fs.make<TH1F>( "GenWTransverseMass", "W_TTbar Transverse Mass Gen", 30, 0, 150);
   histograms["MC Truth mT"] = fs.make<TH1F>( "MC Truth mT", "W_TTbar Transverse Mass Gen minus Reco", 40, -100, 100);
        
   //Other MC Truth Studies
   if(muPlusJets_){
      histograms["MC Truth Mu Pt"] = fs.make<TH1F>(  "MC Truth Mu Pt", "MC Truth Muon Pt", 20, -10, 10);
      histograms["MC Truth Mu Eta"] = fs.make<TH1F>( "MC Truth Mu Eta", "MC Truth Muon  Eta", 20, -.01, -.01);
      histograms["MC Truth Mu Phi"] = fs.make<TH1F>( "MC Truth Mu Phi", "MC Truth Muon Phi", 20, -.01, .01);
   }
   if(ePlusJets_){
      histograms["MC Truth Ele Pt"] = fs.make<TH1F>(  "MC Truth Ele Pt", "MC Truth Electron Pt", 20, -10, 10);
      histograms["MC Truth Ele Eta"] = fs.make<TH1F>( "MC Truth Ele Eta", "MC Truth Electron Eta", 20, -.01, -.01);
      histograms["MC Truth Ele Phi"] = fs.make<TH1F>( "MC Truth Ele Phi", "MC Truth Electron Phi", 20, -.01, .01);
   }
   histograms["MC Truth MET Pt"] = fs.make<TH1F>(  "MC Truth MET Pt", "MC Truth MET Pt", 20, -10, 10);
   histograms["MC Truth MET Phi"] = fs.make<TH1F>( "MC Truth MET Phi", "MC Truth MET Phi", 20, -.01, .01);



   // Tessa Z mass
   histograms["Invarient mass of two muons"]  = fs.make<TH1F>( "Invarient mass of two muons", "Invarient Mass", 150, 0, 150);
    histograms["Invarient mass of Z"]  = fs.make<TH1F>( "Invarient mass of Z", "Invarient Mass", 40, 70, 110);
    histograms["Invarient mass of Z (0 jets)"]  = fs.make<TH1F>( "Invarient mass of Z (0 jets)", "Invarient Mass", 40, 70, 110);
    histograms["Invarient mass of Z (1 jet)"]  = fs.make<TH1F>( "Invarient mass of Z (1 jet) ", "Invarient Mass", 40, 70, 110);
    histograms["Invarient mass of Z (>=2 jets)"]  = fs.make<TH1F>( "Invarient mass of Z (>=2 jets)", "Invarient Mass", 40, 70, 110); 
     
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
      //cout << "Event Passed Selector:       ";

      //Since we only care about selected events, why make this more
      //complicated than it has to be?
      if (!passed) continue; // I want events to pass trigger only for data.
      ++nPassed;
    
 
      std::vector<reco::ShallowClonePtrCandidate> const & electrons = wPlusJets.selectedElectrons();
      std::vector<reco::ShallowClonePtrCandidate> const & muons     = wPlusJets.selectedMuons();
      std::vector<reco::ShallowClonePtrCandidate> const & jets      = wPlusJets.cleanedJets();
      //This list may still have electrons (and muons?) double-counted as jets
      std::vector<reco::ShallowClonePtrCandidate> const & jetsBeforeClean = wPlusJets.selectedJets();
      reco::ShallowClonePtrCandidate   const & met             = wPlusJets.selectedMET();
    
      reco::Particle corrMet =  reCalculateMetWithShiftedJets (ev, shyftParameters);
      

      //Some quantities needed for plots:
      int charge = 0;  //Filled in from electron or muon charge
      int nJetsPlot = (int) jets.size();
      if (nJetsPlot > maxNJets) nJetsPlot = maxNJets;

      //Common lepton kinematics
      reco::Particle::LorentzVector leptonLV;
      const reco::GenParticle *genLepton = 0;
      

      //Muon-only plots
      const pat::Muon * muon_ = 0;
      if ( muPlusJets_ ) {
        // cout << "        Size of the moun vecor: " << muons.size() << endl;
         if (muons.size() > 0){
           
           //Tessa Zs
           //cout << "=============== NEW EVENT shyft_pretag Muon unpacking ======================" << endl;

           //cout <<"Trying to get the muon from the selectedMuons" << endl;
           
           muon_ = dynamic_cast<const pat::Muon *>(muons[0].masterClonePtr().get());
          
           if(muons.size()>1){
           const pat::Muon * muon_num2 = dynamic_cast<const pat::Muon *>(muons[1].masterClonePtr().get());
         
           reco::Particle::LorentzVector ZLV = muon_->p4() + muon_num2->p4();

           double Zmass = ZLV.M();
           double M1Charge = muon_->charge();
           double M2Charge = muon_num2->charge();
           histograms["Invarient mass of two muons"]->Fill(Zmass);
           if (M1Charge != M2Charge){
           if(Zmass >76 && Zmass <106){
             // if( corrMet.et() <=15){   //MET Cut
              histograms["Invarient mass of Z"]->Fill(Zmass);
              //cout << "Muon 1 Njets: " <<nJetsPlot << endl;
              // cout << "Muon 2 Njets: " <<nJetsPlot << endl << endl;
              histograms["nJets"]->Fill(nJetsPlot);
              if(nJetsPlot == 0){
                histograms["Invarient mass of Z (0 jets)"]->Fill(Zmass); 
              }
              if(nJetsPlot == 1){
                histograms["Invarient mass of Z (1 jet)"]->Fill(Zmass);
              }
              if(nJetsPlot >= 2){
                histograms["Invarient mass of Z (>=2 jets)"]->Fill(Zmass);
              }
              // }//end of the MET Cut
           }//end Z-mass Peak
           }//end oppositely charged muons
           // cout << "Muon 1 Pt: " << muon_->pt() << "  Muon 2 Pt: "  << muon_num2->pt() << endl;
              }
           else{
             //cout << "Muon 1 Njets: " <<nJetsPlot << endl << endl;
           }
           
           

             
           
           if (muon_ == 0 ) {

             cout << "Oops, we don't have a valid muon pointer in this event, skipping it" << endl;
             continue;

           } else {
             //cout << "We have the muon pointer and it is fine." << endl;
           }
           
           leptonLV = muon_->p4();
           genLepton = muon_->genLepton();
          
            charge = muon_->charge();
            histograms["muPt"]->Fill(muon_->pt());
            if (charge > 0) histograms["muPtPlus"]->Fill(muon_->pt());
            if (charge < 0) histograms["muPtMinus"]->Fill(muon_->pt());
          
            histograms["muEta"]->Fill(muon_->eta());
            histograms["muPhi"]->Fill(muon_->phi());
          
            TString histName = "muPt_";
            histName += nJetsPlot;
            histName += "j";
            histograms[histName]->Fill(muon_->pt());
          
            histName = "muEta_";
            histName += nJetsPlot;
            histName += "j";
            histograms[histName]->Fill(muon_->eta());
          
            //Check whether the muon is matched HLT_Mu9
            edm::InputTag trigTag(shyftParameters.getParameter<edm::InputTag>("trigSrc"));
            edm::Handle<pat::TriggerEvent> triggerEvent;
            ((edm::EventBase *)&ev)->getByLabel(trigTag, triggerEvent);
            pat::TriggerEvent const * trig = &*triggerEvent;
          
            pat::TriggerObjectRefVector trigObj = trig->pathObjects("HLT_Mu9");
          
            double minDR = 9e20;
          
            for ( pat::TriggerObjectRefVector::const_iterator it = trigObj.begin(); it != trigObj.end(); ++it ) {
               const pat::TriggerObjectRef objRef(*it);
             
               if (objRef->collection().find("hltL3MuonCandidates") != std::string::npos) {
                  double dR = reco::deltaR(muon_->eta(),muon_->phi(),objRef->eta(),objRef->phi());
                  if (dR < minDR) minDR = dR;
               }
            }
            histograms["minTrigDR"]->Fill(minDR);
          
            //////////////Muon Values////////
            double hcalIso  = muon_->hcalIso();
            double ecalIso  = muon_->ecalIso();
            double trkIso   = muon_->trackIso();
            double iMuonPt  = muon_->pt();
            double relIso = (iMuonPt != 0 ) ? (ecalIso + hcalIso + trkIso) / iMuonPt : 0;
          
            unsigned stationMask = muon_->stationMask();
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
          
            reco::TrackRef globalTrack = muon_->globalTrack();

            if (!globalTrack.isNonnull()){
              cout << "Oh no! You have a muon that doesn't have a global track :("
                   << "Number of muons in this event is " << muons.size() << endl
                   << "muon.isGlobalMuon() = " << muon_->isGlobalMuon() << endl
                   << "muon.isTrackerMuon() = " << muon_->isTrackerMuon() << endl
                   << "muon.isStandAloneMuon() = " << muon_->isStandAloneMuon() << endl
                   << endl;
              
              cout << "Skipping the rest of the event" << endl;

              continue;
            }
            reco::HitPattern globalTrackHits = globalTrack->hitPattern();
          
            int numberOfMuonHits = globalTrackHits.numberOfValidMuonHits();
            int numberOfStripHits = globalTrackHits.numberOfValidTrackerHits();
            int numberOfPixelLayersWithMeasurement = globalTrackHits.pixelLayersWithMeasurement();
            int missingTrackerHits = globalTrackHits.numberOfLostStripHits();
          
          
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
          
            if (genLepton) {
               double  mu_pt= muon_->pt();
               double  mu_eta= muon_->eta();
               double  mu_phi= muon_->phi();
	  
               double gmu_pt= genLepton->pt();
               double gmu_eta= genLepton->eta();
               double gmu_phi= genLepton->phi(); 
             
               double MCtruth_Mu_pt =  (gmu_pt -mu_pt)/(gmu_eta);
               double MCtruth_Mu_eta = (gmu_eta -mu_eta)/(gmu_eta);
               double MCtruth_Mu_phi = (gmu_phi -mu_phi)/(gmu_phi);
             
               //"MC Truth Pt"    histograms["MC Truth Mu Eta"]
               histograms["MC Truth Mu Pt"]->Fill(MCtruth_Mu_pt);
               histograms["MC Truth Mu Eta"]->Fill(MCtruth_Mu_eta);
               histograms["MC Truth Mu Phi"]->Fill(MCtruth_Mu_phi);
            }
         } else if (!ePlusJets_) {
            continue; //Skip events that don't have muons unless we're also allowing for electrons
         }
      }//muPlusJets_
    
      //Electron-only plots
      const pat::Electron * electron_ = 0;
    
      if ( ePlusJets_ ) {     
         if (electrons.size() > 0){

            electron_ = dynamic_cast<const pat::Electron *>(electrons[0].masterClonePtr().get());
            leptonLV = electron_->p4();
            genLepton = electron_->genLepton();
            charge = electron_->charge();  
            if(electron_->isEE()){
               histograms["ePt_EE"]->Fill(electron_->pt());
               if (charge > 0) histograms["ePtPlus_EE"]->Fill(electron_->pt());
               if (charge < 0) histograms["ePtMinus_EE"]->Fill(electron_->pt());        
               histograms["eEta_EE"]->Fill(electron_->eta());
               histograms["ePhi_EE"]->Fill(electron_->phi());

               TString histName = "ePt_";
               histName += nJetsPlot;
               histName += "j_EE";
               histograms[histName]->Fill(electron_->pt());
            
               histName = "eEta_";
               histName += nJetsPlot;
               histName += "j_EE";
               histograms[histName]->Fill(electron_->eta());
            
            }
            else if(electron_->isEB()){
               histograms["ePt_EB"]->Fill(electron_->pt());
               if (charge > 0) histograms["ePtPlus_EB"]->Fill(electron_->pt());
               if (charge < 0) histograms["ePtMinus_EB"]->Fill(electron_->pt());        
               histograms["eEta_EB"]->Fill(electron_->eta());
               histograms["ePhi_EB"]->Fill(electron_->phi());
             
               TString histName = "ePt_";
               histName += nJetsPlot;
               histName += "j_EB";
               histograms[histName]->Fill(electron_->pt());
            
               histName = "eEta_";
               histName += nJetsPlot;
               histName += "j_EB";
               histograms[histName]->Fill(electron_->eta());
            }
          
            if (genLepton) {
               double  el_pt= electron_->pt();
               double  el_eta= electron_->eta();
               double  el_phi= electron_->phi();
             
               double gel_pt= genLepton->pt();
               double gel_eta= genLepton->eta();
               double gel_phi= genLepton->phi();
             
               double MCtruth_Ele_pt =  (gel_pt -el_pt)/(gel_eta);
               double MCtruth_Ele_eta = (gel_eta -el_eta)/(gel_eta);
               double MCtruth_Ele_phi = (gel_phi -el_phi)/(gel_phi);
             
               //"MC Truth Pt"    histograms["MC Truth Mu Eta"]
               histograms["MC Truth Ele Pt"]->Fill(MCtruth_Ele_pt);
               histograms["MC Truth Ele Eta"]->Fill(MCtruth_Ele_eta);
               histograms["MC Truth Ele Phi"]->Fill(MCtruth_Ele_phi);
            }
         } else if (!muPlusJets_){
            continue;
         }
       
      }//ePlusJets_
    
      //Fill jet count plots

     
      
      //tessa njets
    
      if (charge > 0){
         histograms["nJetsPlus"]->Fill(nJetsPlot);
      }
      else if (charge < 0){
         histograms["nJetsMinus"]->Fill(nJetsPlot);
      }
   
      int nJetsBeforeCleanPlot = ( ((int) jetsBeforeClean.size()) > maxNJets) ? maxNJets : jetsBeforeClean.size();
      histograms["nJetsBeforeClean"]->Fill(nJetsBeforeCleanPlot);

      //Fill individual jet plots
      for (unsigned iJet = 0; (int)iJet < nJetsPlot; ++iJet) {

         const pat::Jet *jet_ = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());

         TString histName = "jet"; histName += (iJet+1);

         histograms[histName + "Pt"]->Fill(jet_->pt());
         histograms[histName + "Eta"]->Fill(jet_->eta());
         histograms[histName + "Phi"]->Fill(jet_->phi());
         histograms[histName + "Et"]->Fill(jet_->et());
         histograms[histName + "Mass"]->Fill(jet_->mass());

         TString histNameDPhiMet = "dPhiMetJet"; histNameDPhiMet += (iJet+1);
         histograms[histNameDPhiMet]->Fill(fabs(reco::deltaPhi(corrMet.phi(),jet_->phi())));

         TString histName2 = histName + "Pt_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jet_->pt());
         histName2 = histName + "Eta_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jet_->eta());
         histName2 = histName + "Phi_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jet_->phi());
         histName2 = histName + "Et_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jet_->et());
         histName2 = histName + "Mass_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jet_->mass());
         histName2 = histNameDPhiMet+"_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(fabs(reco::deltaPhi(corrMet.phi(),jet_->phi())));

      }//iJet
    
      //Calculate Ht using all available jets
      double ht = 0.0;
      if (muPlusJets_ && muons.size() > 0) ht += muon_->pt();
      if (ePlusJets_ && electrons.size() > 0) ht += electron_->pt();

      double htLep = ht + corrMet.et(); //htLep = only lepPt and MET

      for (unsigned int iJet = 0; iJet < jets.size(); ++iJet) {
         const pat::Jet *jet_ = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
         ht += jet_->et();
      }

      double htAll = ht + corrMet.et(); //htAll = everything: lepPt + MET + sum(jetEt)


      //Debugging output
      if (nDumped < maxToDump || maxToDump < 0) {
         int run = ev.id().run();
         int event = ev.id().event();

         int lumi = ev.id().luminosityBlock();
         std::cout << "=== Run " << run  << " Ev " << event << " Lumi " << lumi;
         std::cout << " Ht " << ht << " MET " <<corrMet.et();
         if (muPlusJets_ && muons.size() > 0) std::cout << " pt_mu " << muon_->pt();
         if (ePlusJets_ && electrons.size() > 0) std::cout << " pt_el " << electron_->pt();

         for (unsigned int iJet = 0; iJet < jets.size(); ++iJet) {
            const pat::Jet *jet_ = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
            std::cout << " Jet " << iJet << " Et " << jet_->et();
         }
         std::cout << std::endl;
      
         ++nDumped;
      }//debugging
              
      //Fill Ht plots ("conventional"):
      // All jet bins
      if(muPlusJets_){
         histograms["ht"]->Fill(ht);
         if (charge > 0)  histograms["htPlus"]->Fill(ht);
         if (charge < 0)  histograms["htMinus"]->Fill(ht);

         histograms["htVsNJet"]->Fill(nJetsPlot,ht);
         
         //Fill only the plot for the appropriate jet bin
         TString htPlotName = "ht_";
         htPlotName += nJetsPlot;
         htPlotName += "j";
         histograms[htPlotName]->Fill(ht);
         

         //Fill Ht plots (lepton + MET only):
         // All jet bins
         histograms["htLep"]->Fill(htLep);
         if (charge > 0)  histograms["htLepPlus"]->Fill(htLep);
         if (charge < 0)  histograms["htLepMinus"]->Fill(htLep);

         histograms["htLepVsNJet"]->Fill(nJetsPlot,htLep);
         
         //Fill only the plot for the appropriate jet bin
         htPlotName = "htLep_";
         htPlotName += nJetsPlot;
         htPlotName += "j";
         histograms[htPlotName]->Fill(htLep);
         //Fill Ht plots (lepton + MET + sum(jetEt)):
         // All jet bins
         histograms["htAll"]->Fill(htAll);
         if (charge > 0)  histograms["htAllPlus"]->Fill(htAll);
         if (charge < 0)  histograms["htAllMinus"]->Fill(htAll);
         
         histograms["htAllVsNJet"]->Fill(nJetsPlot,htAll);
         
         //Fill only the plot for the appropriate jet bin
         htPlotName = "htAll_";
         htPlotName += nJetsPlot;
         htPlotName += "j";
         histograms[htPlotName]->Fill(htAll);
         

      }//muPlusJets
    
      if(ePlusJets_){
      
         if(electron_->isEE()){
            histograms["ht_EE"]->Fill(ht);
            if (charge > 0)  histograms["htPlus_EE"]->Fill(ht);
            if (charge < 0)  histograms["htMinus_EE"]->Fill(ht);
          
            histograms["htVsNJet_EE"]->Fill(nJetsPlot,ht);
          
            //Fill only the plot for the appropriate jet bin
            TString htPlotName = "ht_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EE";
            histograms[htPlotName]->Fill(ht);
          
            //Fill Ht plots (lepton + MET only):
            // All jet bins
            histograms["htLep_EE"]->Fill(htLep);
            if (charge > 0)  histograms["htLepPlus_EE"]->Fill(htLep);
            if (charge < 0)  histograms["htLepMinus_EE"]->Fill(htLep);
          
            histograms["htLepVsNJet_EE"]->Fill(nJetsPlot,htLep);
          
            //Fill only the plot for the appropriate jet bin
            htPlotName = "htLep_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EE";
            histograms[htPlotName]->Fill(htLep);
          
            //Fill Ht plots (lepton + MET + sum(jetEt)):
            // All jet bins
            histograms["htAll_EE"]->Fill(htAll);
            if (charge > 0)  histograms["htAllPlus_EE"]->Fill(htAll);
            if (charge < 0)  histograms["htAllMinus_EE"]->Fill(htAll);
          
            histograms["htAllVsNJet_EE"]->Fill(nJetsPlot,htAll);
          
            //Fill only the plot for the appropriate jet bin
            htPlotName = "htAll_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EE";
            histograms[htPlotName]->Fill(htAll);
          
         }
         else if(electron_->isEB()){
            histograms["ht_EB"]->Fill(ht);
            if (charge > 0)  histograms["htPlus_EB"]->Fill(ht);
            if (charge < 0)  histograms["htMinus_EB"]->Fill(ht);
          
            histograms["htVsNJet_EB"]->Fill(nJetsPlot,ht);
          
            //Fill only the plot for the appropriate jet bin
            TString htPlotName = "ht_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EB";
            histograms[htPlotName]->Fill(ht);

            //Fill Ht plots (lepton + MET only):
            // All jet bins
            histograms["htLep_EB"]->Fill(htLep);
            if (charge > 0)  histograms["htLepPlus_EB"]->Fill(htLep);
            if (charge < 0)  histograms["htLepMinus_EB"]->Fill(htLep);
          
            histograms["htLepVsNJet_EB"]->Fill(nJetsPlot,htLep);
          
            //Fill only the plot for the appropriate jet bin
            htPlotName = "htLep_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EB";
            histograms[htPlotName]->Fill(htLep);
          
            //Fill Ht plots (lepton + MET + sum(jetEt)):
            // All jet bins
            histograms["htAll_EB"]->Fill(htAll);
            if (charge > 0)  histograms["htAllPlus_EB"]->Fill(htAll);
            if (charge < 0)  histograms["htAllMinus_EB"]->Fill(htAll);
          
            histograms["htAllVsNJet_EB"]->Fill(nJetsPlot,htAll);
          
            //Fill only the plot for the appropriate jet bin
            htPlotName = "htAll_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EB";
            histograms[htPlotName]->Fill(htAll);
         }  
      }//ePlusJets_

    
      //Calculate some invariant masses
      if (nJetsPlot >= 2) {
         reco::Particle::LorentzVector m2 = jets[0].p4() + jets[1].p4();
         histograms["m2"]->Fill(m2.M());
         TString m2PlotName = "m2_";
         m2PlotName += nJetsPlot;
         m2PlotName += "j";
         histograms[m2PlotName]->Fill(m2.M());
      
         if (nJetsPlot >= 3) {         
            reco::Particle::LorentzVector m3 = jets[0].p4() + jets[1].p4() + jets[2].p4();
            histograms["m3"]->Fill(m3.M());
            TString m3PlotName = "m3_";
            m3PlotName += nJetsPlot;
            m3PlotName += "j";
            histograms[m3PlotName]->Fill(m3.M());
         
         }	
      }
    
      // Fill only ht plot for met < 20
      //
    
      if ( corrMet.et() < 20 ) {
         if (nJetsPlot >= 3) {
            histograms["ht_ge3j_0met"]->Fill(ht);
            histograms["htLep_ge3j_0met"]->Fill(htLep);
            histograms["htAll_ge3j_0met"]->Fill(htAll);
         }
         if (nJetsPlot >=4) {
            histograms["ht_ge4j_0met"]->Fill(ht);
            histograms["htLep_ge4j_0met"]->Fill(htLep);
            histograms["htAll_ge4j_0met"]->Fill(htAll);
        
         }
      
      } else if (corrMet.et() >= 20) {
       
         if (nJetsPlot >= 3) {
            histograms["ht_ge3j_1met"]->Fill(ht);
            histograms["htLep_ge3j_1met"]->Fill(htLep);
            histograms["htAll_ge3j_1met"]->Fill(htAll);                
         }
         if (nJetsPlot >=4) {
            histograms["ht_ge4j_1met"]->Fill(ht);
            histograms["htLep_ge4j_1met"]->Fill(htLep);
            histograms["htAll_ge4j_1met"]->Fill(htAll);                        
         }
      
      }
    
   
      //Fill MET plots:
      // All jet bins

      if(muPlusJets_){

         histograms["met"]->Fill(corrMet.et());
         if (charge > 0) histograms["metPlus"]->Fill(corrMet.et());
         if (charge < 0) histograms["metMinus"]->Fill(corrMet.et());
    
         histograms["metVsNJet"]->Fill(nJetsPlot,corrMet.et());

         //Fill only the plot for the appropriate jet bin
         TString metPlotName = "met_";
         metPlotName += nJetsPlot;
         metPlotName += "j";
         histograms[metPlotName]->Fill(corrMet.et());
      }
 
      if(ePlusJets_){
    
         if(electron_->isEE()){
            histograms["met_EE"]->Fill(met.et());
            if (charge > 0) histograms["metPlus_EE"]->Fill(corrMet.et());
            if (charge < 0) histograms["metMinus_EE"]->Fill(corrMet.et());
       
            histograms["metVsNJet_EE"]->Fill(nJetsPlot,corrMet.et());
       
            //Fill only the plot for the appropriate jet bin
            TString metPlotName = "met_";
            metPlotName += nJetsPlot;
            metPlotName += "j_EE";
            histograms[metPlotName]->Fill(corrMet.et());
         }
         else if(electron_->isEB()){
            histograms["met_EB"]->Fill(met.et());
            if (charge > 0) histograms["metPlus_EB"]->Fill(corrMet.et());
            if (charge < 0) histograms["metMinus_EB"]->Fill(corrMet.et());
       
            histograms["metVsNJet_EB"]->Fill(nJetsPlot,corrMet.et());

            TString metPlotName = "met_";
            metPlotName += nJetsPlot;
            metPlotName += "j_EB";
            histograms[metPlotName]->Fill(corrMet.et());
         }   
      }//ePlusJets_
 
      histograms["dPhiMetLep"]->Fill(fabs(reco::deltaPhi(corrMet.phi(),leptonLV.phi())));
      TString dPhiMetLepPlotName = "dPhiMetLep_";
      dPhiMetLepPlotName += nJetsPlot;
      dPhiMetLepPlotName += "j";
      histograms[dPhiMetLepPlotName]->Fill(fabs(reco::deltaPhi(corrMet.phi(),leptonLV.phi())));
 
 
      const pat::MET * met_ = 0;
      met_ = dynamic_cast<const pat::MET *>(met.masterClonePtr().get());
 
      //Calculate and Fill W Transverse Mass 
      reco::Particle::LorentzVector metLV = corrMet.p4();
      metLV.SetPz(0.); //Just to be safe--MET should never have Z component.  That's why it's called ME*T*!
      metLV.SetE(corrMet.pt());
      reco::Particle::LorentzVector leptonTrans = leptonLV;
      leptonTrans.SetPz(0.);
      leptonTrans.SetE(leptonLV.pt());
 
      reco::Particle::LorentzVector WTrans = leptonTrans + metLV;
 
      if(muPlusJets_){
         histograms["mT"]->Fill(WTrans.M());
         if (charge > 0) histograms["mT_Plus"]->Fill(WTrans.M());
         if (charge < 0) histograms["mT_Minus"]->Fill(WTrans.M());

         TString mTPlotName = "mT_";
         mTPlotName += nJetsPlot;
         mTPlotName += "j";
         histograms[mTPlotName]->Fill(WTrans.M());
      }

      if(ePlusJets_){
    
         if(electron_->isEE()){
            histograms["mT_EE"]->Fill(WTrans.M());
            if (charge > 0) histograms["mT_Plus_EE"]->Fill(WTrans.M());
            if (charge < 0) histograms["mT_Minus_EE"]->Fill(WTrans.M());
       
            TString mTPlotName = "mT_";
            mTPlotName += nJetsPlot;
            mTPlotName += "j_EE";
            histograms[mTPlotName]->Fill(WTrans.M());
         }
         else if(electron_->isEB()){
            histograms["mT_EB"]->Fill(WTrans.M());
            if (charge > 0) histograms["mT_Plus_EB"]->Fill(WTrans.M());
            if (charge < 0) histograms["mT_Minus_EB"]->Fill(WTrans.M());
       
            TString mTPlotName = "mT_";
            mTPlotName += nJetsPlot;
            mTPlotName += "j_EB";
            histograms[mTPlotName]->Fill(WTrans.M());
         }
      }

      //Transverse W Mass made with the Generated particles/////////
      const reco::GenMET * genMet = met_->genMET();
      if(genLepton && genMet){
         reco::Particle::LorentzVector gLeptonLV = genLepton->p4();
         reco::Particle::LorentzVector gLeptonTrans = gLeptonLV;
         gLeptonTrans.SetPz(0.);
         gLeptonTrans.SetE(gLeptonLV.pt());
         reco::Particle::LorentzVector gMetLV = genMet->p4();
         gMetLV.SetPz(0.); //Just to be safe; should never be non-zero.
         gMetLV.SetE(gMetLV.pt());
         reco::Particle::LorentzVector gWTrans = gLeptonTrans + gMetLV; 
      
         histograms["GenWTransverseMass"]->Fill(gWTrans.M());

         double  met_pt= corrMet.pt();
         double  met_phi= corrMet.phi();
      
         double gmet_pt= genMet->pt();
         double gmet_phi= genMet->phi();

         double MCtruth_Met_pt =  (gmet_pt -met_pt)/(gmet_pt);
         double MCtruth_Met_phi = (gmet_phi -met_phi)/(gmet_phi);

         histograms["MC Truth MET Pt"]->Fill(MCtruth_Met_pt);
         histograms["MC Truth MET Phi"]->Fill(MCtruth_Met_phi);
      
      }
    
   } //end event loop
     
   cout << "Printing" << endl;
   wPlusJets.print(std::cout);

   // Won't work until 3.6.x
   //   //Now check to see if this was a skim
   //   bool isSkim = false;
   //   int nRuns = 0;

   //   for (std::vector<std::string>::iterator fileIter = fileNames.begin();
   //        fileIter != fileNames.end(); ++fileIter) {

   //     TFile *file = TFile::Open(fileIter->c_str());
   //     fwlite::Run iRun(file);

   //     for (iRun.toBegin(); !iRun.atEnd(); ++iRun, ++nRuns) {

   //       //Keep track of the number of events that pass the "Inclusive" selection for use in normalization
   //       //If this is a skim, get the inclusive events from the information stored in the skim
   //       edm::Handle< unsigned int > nProcessedSkim;
   //       iRun.getByLabel ( edm::InputTag("nProcessedSkim"),nProccessedSkim);
   //       if (nProcessedSkim.isValid()) {
   // 	if (!isSkim) {
   // 	  if (nRuns == 0) {
   // 	    nInclusive = 0;
   // 	    isSkim = true;
   // 	  } else {
   // 	    cerr << "ERROR: Not smart enough to mix files that are skims and files that are not skims." << endl
   // 		 << "Aborting!" << endl;
   // 	    exit(2);
   // 	  }
   // 	}
   // 	nInclusive += *nProcessedSkim;
   //       } else if (isSkim) {
   // 	cerr << "We are in a skim but we can't find the inclusive number of events for this run!" << endl
   // 	     << "Aborting!" << endl;
   // 	exit(2);
   //       }
   //     }
   //   }

   if (shyftParameters.getParameter<bool>("fitNorm") && !ev.isRealData()) {

      //Normalize as if we had generated exactly 10^6 events, which would
      //correspond to an cross section of 1 nb and an integrated luminosity
      //of 1 fb^-1.  This is what the fitter likes.
      double histScale = 1000000./nInclusive;
    
      std::cout << "Applying scale factor of " 
                << Form("%.3f",histScale) << " to ht plots by jet bin." << std::endl;
    
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
      
         TString histName = "ht_"; histName += iJet; histName += "j";
         TH1 *hist = histograms[histName];
         hist->Sumw2();
         hist->Scale(histScale);
      }
   }
  
   cout << "We're done!" << endl;
   //   if (isSkim) cout << "Ran on skim." << endl;
   cout << "Processed " << nTotal << " events." << endl
        << "Passed " << nPassed << " events." << endl;

  
   return 0;
}


////////////////////////////////////
//
//  Function to re-calculate MET
//  based on JES shifted jets
//
///////////////////////////////////


reco::Particle reCalculateMetWithShiftedJets
( edm::EventBase const & event, edm::ParameterSet const & setupParameters )
{

   // cout << "=============  Inside MET re-calculator  ===============" << endl;

   edm::InputTag jetTag (setupParameters.getParameter<edm::InputTag>("jetSrc"));
   edm::InputTag metTag (setupParameters.getParameter<edm::InputTag>("metSrc"));
   double jetScale (setupParameters.getParameter<double>("jetScale"));

   edm::Handle< vector< pat::Jet > > jetHandle;
   event.getByLabel (jetTag, jetHandle);
  
   edm::Handle< vector< pat::MET > > metHandle;
   event.getByLabel (metTag, metHandle);

   //  Future update: make sure these handles are valid
   //  

   // Get the MET
   reco::ShallowClonePtrCandidate met_ ( edm::Ptr<pat::MET>( metHandle, 0),
                                         metHandle->at(0).charge(),
                                         metHandle->at(0).p4() );


   double deltaPx = 0.0;
   double deltaPy = 0.0;
   double deltaSumEt = 0.0;


   // for each jet in collection
   //
   // 1. Get the raw jet, because pat jets have L3 corrections
   // 2. Get check raw jet pt > 20, emf < 0.9... all those jets
   //    are included in the met calc
   // 3. The met calc has already accounted for the L3 corrections,
   //    so calculate the difference from that jet
   //    by scaling the L3 corrected momenta
   // 4. Sum the differences over all jets
   //
  
   for ( std::vector<pat::Jet>::const_iterator jetBegin = jetHandle->begin(),
            jetEnd = jetHandle->end(), ijet = jetBegin;
         ijet != jetEnd; ++ijet ) {

     // if you don't do this, you will crash!
     if (!ijet->isCaloJet()) continue;
     
     pat::Jet iRawJet = ijet->correctedJet(pat::JetCorrFactors::Raw);
      

      

      // select based on raw energy, raw emf
      if ( iRawJet.pt() > 20.0 && iRawJet.emEnergyFraction() < 0.9 ) {      

         //   cout << "Found a jet passing selection!" << endl
         //            << "Raw pt = " << iRawJet.pt() << endl
         //            << "Px = " << ijet->px() << ", delta px = " << ijet->px() * (1-jetScale) << endl
         //            << "Py = " << ijet->py() << ", delta py = " << ijet->py() * (1-jetScale) << endl
         //            << endl;
           
         // do calculation based on L3 energy
         deltaPx += ijet->px() * (1-jetScale);
         deltaPy += ijet->py() * (1-jetScale);
         deltaSumEt += ijet->et() *(1-jetScale);      

      } else {
         //cout << "Found a jet with pt < 20 raw! pt = " << iRawJet.pt()
         //     << ", eta = " << iRawJet.eta() << endl;
      }

   } // end for each jet
  

   // add the differences to the met
   // to produce a corrected met
  
   double corrPx = met_.px() + deltaPx;
   double corrPy = met_.py() + deltaPy;

  
   reco::Particle::LorentzVector corrMetLV (corrPx,
                                            corrPy,
                                            0,
                                            sqrt(corrPx*corrPx + corrPy*corrPy)
      );

   reco::Particle corrMetParticle ( met_.charge(), corrMetLV);

   //   cout << "Orginal met = ("
   //        << met_.px()
   //        << ", " << met_.py()
   //        << ", 0, " << met_.pt() << ")"
   //        << endl
   //        << " ... total correction = (" 
   //        << deltaPx
   //        << ", " << deltaPy
   //        << ", 0)" << endl
   //        << "New met = (" << corrMetParticle.px()
   //        << ", " << corrMetParticle.py()
   //        << ", " << corrMetParticle.pz()
   //        << ", " << corrMetParticle.pt()
   //        << ")"
   //        << "   and just for reference et = " << corrMetParticle.et()
   //        << endl;


   return corrMetParticle;

 
}
