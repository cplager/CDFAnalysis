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
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "PhysicsTools/Utilities/interface/Lumi3DReWeighting.h"

#include "DataFormats/PatCandidates/interface/TriggerObject.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "Math/GenVector/PxPyPzM4D.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

//Root includes
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TRegexp.h"

//handle JEC uncertainties
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"



//angular variables function for HWW
#include "AngularVars.h"

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
    
      histograms["muPt"] = fs.make<TH1F>( "muPt",     "Muon p_{T} (GeV/c) ",     300, 0, 300 );
      histograms["muPtPlus"] = fs.make<TH1F>( "muPtPlus",     "Muon (+) p_{T} (GeV/c) ",     300, 0, 300 );
      histograms["muPtMinus"] = fs.make<TH1F>( "muPtMinus",     "Muon (-) p_{T} (GeV/c) ",     300, 0, 300 );
      histograms["muEta"] = fs.make<TH1F>( "muEta",    "Muon #eta",                50,   -3.0,  3.0);
      histograms["muPhi"] = fs.make<TH1F>( "muPhi",    "Muon #phi",               50,  -3.1416,  3.1416 );

      //Split some of the above plots by jet bin
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "muPt_"; histName += iJet; histName += "j";
         TString histTitle = "Muon p_{T} (GeV/c) (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";

         histograms[histName] = fs.make<TH1F>(histName, histTitle, 300, 0., 300.);  
      }
       for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "muPtPlus_"; histName += iJet; histName += "j";
         TString histTitle = "Muon p_{T} (GeV/c) (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";

         histograms[histName] = fs.make<TH1F>(histName, histTitle, 300, 0., 300.);  
      }
        for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "muPtMinus_"; histName += iJet; histName += "j";
         TString histTitle = "Muon p_{T} (GeV/c) (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";

         histograms[histName] = fs.make<TH1F>(histName, histTitle, 300, 0., 300.);  
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
      histograms["relIso"] = fs.make<TH1F>( "relIso", "rel Iso", 400, 0, 1.25);
      histograms["particleIso"] = fs.make<TH1F>( "particleIso", "particle Iso", 30, -1, 2);
      histograms["chargedHadronIso"] = fs.make<TH1F>( "chargedHadronIso", "charged Hadron Iso", 40, 0, 2);
      histograms["neutralHadronIso"] = fs.make<TH1F>( "neutralHadronIso", "neutral Hadron Iso", 40, 0, 2);
      histograms["photonIso"] = fs.make<TH1F>( "photonIso", "photon Iso", 40, 0, 2);
      histograms["relHadIso"] = fs.make<TH1F>( "relHadIso", "relative Hadron Isolation", 400, 0, 1.25);
      histograms["PFIso"] = fs.make<TH1F>( "PFIso", "Particle Flow Isolation", 400, 0, 1.25);
      histograms["relIso vs PFIso"] = fs.make<TH2F>("relIso vs PFIso", "relIso vs PFIso", 400, 0, 1.25, 400, 0, 1.25);
      histograms["mu d0"] = fs.make<TH1F>( "mu d0", "muon impact parameter dB",100,-0.02,0.02);
      histograms["mu z0"] = fs.make<TH1F>( "mu z0", "muon distance from PV",100,-5,5);
      histograms["nDTStations"] = fs.make<TH1F>( "nDTStations", "nDT Stations", 6, -.5, 5.5);
      histograms["nCSCStations"] = fs.make<TH1F>( "nCSCStations", "nCSC Stations", 6, -.5, 5.5);
      histograms["nTotalStations"] = fs.make<TH1F>( "nTotalStations", "Total Muon Stations", 10, -.5, 9.5);
      histograms["numberOfMuonHits"] = fs.make<TH1F>( "numberOfMuonHits", "Number of Muon Hits", 61, -0.5, 60.5);
      histograms["numberOfStripHits"] = fs.make<TH1F>( "numberOfStripHits", "Number of Strip Hits", 41, -0.5, 40.5);
      histograms["numberOfPixelLayersWithMeasurement"] = fs.make<TH1F>( "numberOfPixelLayersWithMeasurement", "Number of Pixel Layers With Measurement", 5, -0.5, 4.5);
      histograms["missingTrackerHits"] = fs.make<TH1F>( "missingTrackerHits", "Missing Tracker Hits", 4, -.5, 3.5);
      histograms["minTrigDR"] = fs.make<TH1F>("minTrigDR","Minimum #Delta R (#mu - Trig)", 400, 0., 4.); 

      histograms["jesUncert"] = fs.make<TH1F>("jesUncert","JES Uncert", 50, 0, 0.1);
      histograms["jesUncertVsEta"] = fs.make<TH2F>("jesUncertEta","JES Uncert as a function of Eta", 10, -3.0, 3.0, 10, 0, 0.1);
      histograms["jesUncertVsPt"] = fs.make<TH2F>("jesUncertPt","JES Uncert as a function of Pt", 10, 0, 200, 10, 0, 0.1);

      histograms["jesUncertPlus"] = fs.make<TH1F>("jesUncertPlus","JES Uncert", 50, 0, 0.1);
      histograms["jesUncertVsEtaPlus"] = fs.make<TH2F>("jesUncertEtaPlus","JES Uncert as a function of Eta", 10, -3.0, 3.0, 10, 0, 0.1);
      histograms["jesUncertVsPtPlus"] = fs.make<TH2F>("jesUncertPtPlus","JES Uncert as a function of Pt", 10, 0, 200, 10, 0, 0.1);
      
                 
      
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
         histograms[histName2] = fs.make<TH1F>(histName2, histTitle2, 500, 0., 500.);
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
  
   //=================  
   //    Jet Kinematic variables in the lab frame
   //=========
   histograms["dEtaJetJet"] = fs.make<TH1F>("dEtaJetJet",  "#Delta #eta between Jets", 60, 0,   6.0);
   histograms["dPhiJetJet"] = fs.make<TH1F>("dPhiJetJet",  "#Delta #phi between Jets", 50, 0,   3.1416);
   histograms["minDPhiLepJet"] = fs.make<TH1F>("minDPhiLepJet",  "min #Delta #phi between Lepton and Jet", 50, 0,   3.1416);

   //=================  
   //    Jet Kinematic variables in the lab frame Andrea's version
   //=========
   histograms["dEtaJetJet_an"] = fs.make<TH1F>("dEtaJetJet_an",  "#Delta #eta between Jets", 60, 0,   6.0);
   histograms["dPhiJetJet_an"] = fs.make<TH1F>("dPhiJetJet_an",  "#Delta #phi between Jets", 50, 0,   3.1416);
   histograms["minDPhiLepJet_an"] = fs.make<TH1F>("minDPhiLepJet_an",  "min #Delta #phi between Lepton and Jet", 50, 0,   3.1416);

   //==============
   // Jackson angles etc.  ==2jets
   histograms["Cos_dPhiWW"] = fs.make<TH1F>("Cos_dPhiWW","Cos#Delta #phi between two W decay planes", 200, -1, 1);
   histograms["CosTheta_l"] = fs.make<TH1F>("CosTheta_l","Cos#theta of lepton in Wlv rest frame", 200, -1, 1);
   histograms["CosTheta_j"] = fs.make<TH1F>("CosTheta_j","Cos#theta of jet1 in Wjj rest frame", 100, 0, 1);
   histograms["CosTheta_W1"] = fs.make<TH1F>("CosTheta_W1","Cos#theta of leptonic W in Higgs rest frame", 200, -1, 1);
   histograms["Cos_dPhiWH"] = fs.make<TH1F>("Cos_dPhiWH","Cos#Delta #phi between W1 decay plane and Higgs decay plane", 200, -1, 1);

   //More Jet Kinematic variables for top
   histograms["dRjj_min"] = fs.make<TH1F>("dRjj_min", "Minimum dR between jets for event", 1000, 0, 10);
   histograms["dRjj_max"] = fs.make<TH1F>("dRjj_max", "Minimum dR between jets for event", 2000, 0, 20);
   histograms["dRjj_lead2"]  = fs.make<TH1F>("dRjj_lead2", "dR between 2 leading jets", 2000, 0, 20);

   
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
   histograms["m2"] = fs.make<TH1F> ("m2", "M2", 1200, 0, 1200);
   histograms["m2_an"] = fs.make<TH1F> ("m2_an", "M2", 1200, 0, 1200); //Andrea's version

   //Separated by Jet Bin
   for (int iJet = max(2,minNJets); iJet <= maxNJets; ++iJet) {
      TString histName = "m2_"; histName += iJet; histName += "j";
      TString histTitle = "M2 (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }

   //===============================
   //M3 = Mass of leading three jets
   //===============================
   //All Jet Bins
   histograms["m3"] = fs.make<TH1F> ("m3", "M3", 200, 0, 2400);
   histograms["m3_pt"] = fs.make<TH1F> ("m3_pt", "M3", 2400, 0, 2400);
   histograms["m3Top3Jets"] = fs.make<TH1F> ("m3Top3Jets", "M3", 200, 0, 2400);

   //Separated by Jet Bin
   for (int iJet = max(3,minNJets); iJet <= maxNJets; ++iJet) {
      TString histName = "m3_"; histName += iJet; histName += "j";
      TString histTitle = "M3 (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 2000, 0., 2000.);

      histName = "m3Top3Jets_"; histName += iJet; histName += "j";
      histTitle = "M3 (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 2000, 0., 2000.);
   }
  

   //================================
   // Robin --- M(W_hadronic) M_top Number of btags plots     // searching for new physics
   //================================

   //======================================
   //M2Low = dijet mass of two jets with lowest vectorial sum pt
   //======================================
   //All Jet Bins
   histograms["m2Low"] = fs.make<TH1F> ("m2Low", "M2Low", 1200, 0, 1200);

   //Separated by Jet Bin
   for (int iJet = max(2,minNJets); iJet <= maxNJets; ++iJet) {
      TString histName = "m2Low_"; histName += iJet; histName += "j";
      TString histTitle = "M2Low (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }   
   
   //======================================
   //M2High = dijet mass of two jets with highest vectorial sum pt
   //======================================
   //All Jet Bins
   histograms["m2High"] = fs.make<TH1F> ("m2High", "M2High", 1200, 0, 1200);
   
   //Separated by Jet Bin
   for (int iJet = max(2,minNJets); iJet <= maxNJets; ++iJet) {
      TString histName = "m2High_"; histName += iJet; histName += "j";
      TString histTitle = "M2High (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }   

   //======================================
   //M2_dR = dijet mass of two jets with min deltaR
   //======================================
   //All Jet Bins
   histograms["m2_dR"] = fs.make<TH1F> ("m2_dR", "M2dR", 1200, 0, 1200);
   
   //Separated by Jet Bin
   for (int iJet = max(2,minNJets); iJet <= maxNJets; ++iJet) {
      TString histName = "m2_dR_"; histName += iJet; histName += "j";
      TString histTitle = "M2dR (N_{jets}"; 
      histTitle += ((iJet == maxNJets) ? "#geq" : "=");
      histTitle += iJet;
      histTitle += ")";
      histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }   

   //===============================
   //M3Low = Mass of three jets with the lowest vectorial sum pt
   //===============================
   //All Jet Bins
   histograms["m3Low"] = fs.make<TH1F> ("m3Low", "M3Low", 1200, 0, 1200);
    
   //Separated by Jet Bin
   for (int iJet = max(3,minNJets); iJet <= maxNJets; ++iJet) {
     TString histName = "m3Low_"; histName += iJet; histName += "j";
     TString histTitle = "M3Low (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }

 //----------------------------------------------------------------------------   
   //=== di light jet mass
   //we require at least 4 jets with exactly 2 btag jets. the invariant mass of first 2 leading non-btagged jets
   histograms["mW_had"] = fs.make<TH1F> ("mW_had", "di light jet mass", 1200, 0, 1200); // use btag info
   
   for (int iJet = max(4,minNJets); iJet <= maxNJets; ++iJet) {
     TString histName = "mW_had_"; histName += iJet; histName += "j";
     TString histTitle = "di light jet mass (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }

   //== leptonic leg W mass: muon.p4()+met.p4()
   histograms["mW_lep"] = fs.make<TH1F> ("mW_lep", "leptonic leg W mass", 1200, 0, 1200);
   for (int iJet = max(0,minNJets); iJet <= maxNJets; ++iJet) {
     TString histName = "mW_lep_"; histName += iJet; histName += "j";
     TString histTitle = "leptonic leg W mass (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
 }
   
   //=== dijet mass: 
   //the combination of any 2 jets which has the closest Invariant mass to W
   //all jet bin
   histograms["mW2"] = fs.make<TH1F> ("mW2", "dijet mass", 1200, 0, 1200); // don't use btag info
   //Separated by Jet Bin
   for (int iJet = max(2,minNJets); iJet <= maxNJets; ++iJet) {
     TString histName = "mW2_"; histName += iJet; histName += "j";
     TString histTitle = "diJet Mass (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }
   
  //=== hadronic leg top mass
   //the combination of any 3 jets which has the closest Invariant mass to W
   //all jet bin
   histograms["mTop_had"] = fs.make<TH1F> ("mTop_had", "hadronic leg top mass", 1200, 0, 1200);
   //Seperated by Jet Bin
   for (int iJet = max(3,minNJets); iJet <= maxNJets; ++iJet) {
     TString histName = "mTop_had_"; histName += iJet; histName += "j";
     TString histTitle = "hadronic leg top mass (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }
   
   //=== leptonic leg top mass
   // muon.p4()+met.p4()+jet.p4() closest to top mass
   //all jet bin
   histograms["mTop_lep"] = fs.make<TH1F> ("mTop_lep", "leptonic leg top mass", 1200, 0, 1200);
   //Seperated by Jet Bin
   for (int iJet = max(1,minNJets); iJet <= maxNJets; ++iJet) {
     TString histName = "mTop_lep_"; histName += iJet; histName += "j";
     TString histTitle = "leptonic leg top mass (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 1200, 0., 1200.);
   }

   //=== number of btags
   histograms["nBTagTC"] = fs.make<TH1F> ("nBTagTC", "number of TC btagged jets", 6, -0.5, 5.5 );
   
   histograms["nBTagSSV"] = fs.make<TH1F> ("nBTagSSV", "number of SSV btagged jets", 6, -0.5, 5.5 );

   //==============
   // Mttbar
   //==============]
   //all jet bin
   histograms["Mttbar"] = fs.make<TH1F>("Mttbar", "Invariant Mass of TTbar" , 4000, 0 ,4000);
   //Seperated by Jet Bin
   for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
     TString histName = "Mttbar_"; histName += iJet; histName += "j";
     TString histTitle = "Invariant Mass of TTbar (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 4000, 0., 4000.);
   }
   
   //Mttbar with the 4 leading jets
   histograms["MttbarTop4jets"] = fs.make<TH1F>("MttbarTop4jets", "ttbar Invariant Mass with  Leading 4 jets" , 2500, 0 ,2500);
  //Seperated by Jet Bin
   for (int iJet = max(4,minNJets); iJet <= maxNJets; ++iJet) {
     TString histName = "MttbarTop4jets_"; histName += iJet; histName += "j";
     TString histTitle = "ttbar Invariant Mass with  Leading 4 jets (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 2500, 0., 2500.);
   }

   //MWW with the 2 leading jets 	 
   histograms["MWWTop2jets"] = fs.make<TH1F>("MWWTop2jets", "WW Invariant Mass with  Leading 2 jets" , 2000, 0 ,2000); 	 
   histograms["MWWTop2jets_an"] = fs.make<TH1F>("MWWTop2jets_an", "WW Invariant Mass with  minDeta 2 jets" , 2000, 0 ,2000); //Andrea 	 
   //Seperated by Jet Bin 	 
   for (int iJet = max(2,minNJets); iJet <= maxNJets; ++iJet) { 	 
     TString histName = "MWWTop2jets_"; histName += iJet; histName += "j"; 	 
     TString histTitle = "WW Invariant Mass with  Leading 2 jets (N_{jets}"; 	 
     histTitle += ((iJet == maxNJets) ? "#geq" : "="); 	 
     histTitle += iJet; 	 
     histTitle += ")"; 	 
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 2000, 0., 2000.); 	 
   } 	 
   
   //ttbar pt all jet bin
   histograms["ttbarPt"] = fs.make<TH1F>("ttbarPt", "pt of TTbar" , 200, 0 ,200);
   //Seperated by Jet Bin
   for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
     TString histName = "ttbarPt_"; histName += iJet; histName += "j";
     TString histTitle = "pt of TTbar (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0., 200.);
   }


   // max delta ETA between any two jets
   histograms["maxDeta"] = fs.make<TH1F>("maxDeta", "max jj delta Eta" , 60, 0 ,6);
   //Seperated by Jet Bin
   for (int iJet = max(2,minNJets); iJet <= maxNJets; ++iJet) {
     TString histName = "maxDeta_"; histName += iJet; histName += "j";
     TString histTitle = "max jj delta Eta (N_{jets}"; 
     histTitle += ((iJet == maxNJets) ? "#geq" : "=");
     histTitle += iJet;
     histTitle += ")";
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 60, 0., 6.);
   }

   //==========
   //MET plots
   //==========
  
   
   if( muPlusJets_ ) {
      //All Jet Bins
      histograms["met"] = fs.make<TH1F> ("met", "MET", 150, 0, 300);
      histograms["met_pt_4j"] = fs.make<TH1F> ("met_pt_4j", "MET Pt", 150, 0, 300);
      histograms["met_phi_4j"] = fs.make<TH1F> ("met_phi_4j", "MET Phi", 64, -3.2 , 3.2);
      histograms["metPlus"] = fs.make<TH1F> ("metPlus", "MET (mu+)", 150, 0, 300);
      histograms["metMinus"] = fs.make<TH1F> ("metMinus", "MET (mu-)", 150, 0,300);
      histograms["metVsNJet"] = fs.make<TH2F> ("metVsNJet", ";N_{jets};MET",
                                               maxNJets-minNJets+1,minNJetsFloat,maxNJetsFloat, 
                                               150, 0, 300);
      //Separated by Jet Bin
      //----- Use separate MET histograms
      for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "met_"; histName += iJet; histName += "j";
         TString histTitle = "MET (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";
        
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 300, 0., 300.);
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
   
   histograms["W_Pt"]       = fs.make<TH1F>( "W_Pt", "W Transverse momentum", 100, 0, 200); 	 
   //Separated by Jet Bin 	 
   for (int iJet = minNJets; iJet <= maxNJets; ++iJet) { 	 
     TString histName = "W_Pt_"; histName += iJet; histName += "j"; 	 
     TString histTitle = "W_Pt (N_{jets}"; 	 
     histTitle += ((iJet == maxNJets) ? "#geq" : "="); 	 
     histTitle += iJet; 	 
     histTitle += ")"; 	 
     histograms[histName] = fs.make<TH1F>(histName, histTitle, 200, 0, 200); 	 
   } 	 
   
   if( muPlusJets_ ) {
      //All jet bin 
      histograms["mT"]       = fs.make<TH1F>( "mT", "W_TTbar Transverse Mass Reco", 150, 0, 300);
      histograms["mT_Plus"]  = fs.make<TH1F>( "mT_Plus", "W_TTbar Transverse Mass Reco (mu+)", 150, 0, 300);
      histograms["mT_Minus"] = fs.make<TH1F>( "mT_Minus", "W_TTbar Transverse Mass Reco (mu-)", 150, 0, 300);
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
      histograms["mT_EE"]       = fs.make<TH1F>( "mT_EE", "W_TTbar Transverse Mass Reco in EE", 150, 0, 300);
      histograms["mT_Plus_EE"]  = fs.make<TH1F>( "mT_Plus_EE", "W_TTbar Transverse Mass Reco (e+) in EE", 150, 0, 300);
      histograms["mT_Minus_EE"] = fs.make<TH1F>( "mT_Minus_EE", "W_TTbar Transverse Mass Reco (e-) in EE", 150, 0, 300);
        
      histograms["mT_EB"]       = fs.make<TH1F>( "mT_EB", "W_TTbar Transverse Mass Reco in EB", 150, 0, 300);
      histograms["mT_Plus_EB"]  = fs.make<TH1F>( "mT_Plus_EB", "W_TTbar Transverse Mass Reco (e+) in EB", 150, 0, 300);
      histograms["mT_Minus_EB"] = fs.make<TH1F>( "mT_Minus_EB", "W_TTbar Transverse Mass Reco (e-) in EB", 150, 0, 300);
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
   histograms["genDeltaJetPt"] = fs.make<TH1F> ("genDeltaJetPt", "Difference between Gen and Reco Jet Pt", 100, -50, 50);
   histograms["genResoJetPt"] = fs.make<TH1F> ("genResoJetPt", "(Reco-Gen)/Gen Jet Pt", 100, -1, 1);
   
     
        
   //Other MC Truth Studies
   if(muPlusJets_){
      histograms["MC Truth Mu Pt"] = fs.make<TH1F>(  "MC Truth Mu Pt", "MC Truth Muon Pt", 20, -10, 10);
      histograms["MC Truth Mu Eta"] = fs.make<TH1F>( "MC Truth Mu Eta", "MC Truth Muon  Eta", 20, -.01, .01);
      histograms["MC Truth Mu Phi"] = fs.make<TH1F>( "MC Truth Mu Phi", "MC Truth Muon Phi", 20, -.01, .01);
   }
   
   if(ePlusJets_){
      histograms["MC Truth Ele Pt"] = fs.make<TH1F>(  "MC Truth Ele Pt", "MC Truth Electron Pt", 20, -10, 10);
      histograms["MC Truth Ele Eta"] = fs.make<TH1F>( "MC Truth Ele Eta", "MC Truth Electron Eta", 20, -.01, .01);
      histograms["MC Truth Ele Phi"] = fs.make<TH1F>( "MC Truth Ele Phi", "MC Truth Electron Phi", 20, -.01, .01);
   }
   
   histograms["MC Truth MET Pt"] = fs.make<TH1F>(  "MC Truth MET Pt", "MC Truth MET Pt", 20, -10, 10);
   histograms["MC Truth MET Phi"] = fs.make<TH1F>( "MC Truth MET Phi", "MC Truth MET Phi", 20, -.01, .01);


    //==============
   // Pile Up
   //==============   
   
   histograms["nPV"] = fs.make<TH1F>("nPV", "Number of primary vertices", 25,-0.5,24.5);
   for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "nPV_"; histName += iJet; histName += "j";
         TString histTitle = "Number of Primary Vertices (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 25, -0.5, 24.5);
         // cout<< "hist name " << histName << endl;
   }
//    histograms["nPV_3j"] = fs.make<TH1F>("nPV_3j", "Number of primary vertices", 25,-0.5,24.5);
   histograms["nPVvalid"] = fs.make<TH1F>("nPVvalid", "Number of valid primary vertices", 15,-0.5,14.5);
   histograms["nPVvalid_3j"] = fs.make<TH1F>("nPVvalid_3j", "Number of valid primary vertices", 25,-0.5,24.5);
   histograms["nPVvsRUN"] = fs.make<TH2F>("nPVvsRUN", "Number of valid primary vertices vs run number", 10000,140000,150000,15,0.5,14.5);
   histograms["nPVvsLUMI"] = fs.make<TH2F>("nPVvsLUMI", "Number of valid primary vertices vs lumi section", 34,0,3400,15,0.5,14.5);
   histograms["MuPt 1PV"] = fs.make<TH1F>("MuPt 1PV", "muon pt in one valid primary vertex ",200, 0, 200);
   histograms["MuPt 1PV 2j"] = fs.make<TH1F>("MuPt 1PV 2j", "muon pt in one valid primary vertex ",200, 0, 200);
   histograms["MuPt 1PV 3j"] = fs.make<TH1F>("MuPt 1PV 3j", "muon pt in one valid primary vertex ",200, 0, 200);
   histograms["MuPt 1PV >=4j"] = fs.make<TH1F>("MuPt 1PV >=4j", "muon pt in one valid primary vertex ",200, 0, 200);
   histograms["MuPt manyPV"] = fs.make<TH1F>("MuPt manyPV", "muon pt in more than one valid primary vertex ",200, 0, 200);
   histograms["MuPt manyPV 2j"] = fs.make<TH1F>("MuPt manyPV 2j", "muon pt in more than one valid primary vertex ",200, 0, 200);
   histograms["MuPt manyPV 3j"] = fs.make<TH1F>("MuPt manyPV 3j", "muon pt in more than one valid primary vertex ",200, 0, 200);
   histograms["MuPt manyPV >=4j"] = fs.make<TH1F>("MuPt manyPV >=4j", "muon pt in more than one valid primary vertex ",200, 0, 200);
   histograms["NJets 1PV"] =  fs.make<TH1F>("NJets 1PV", "NJets in one valid primary vertex ",maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);
   histograms["NJets manyPV"] =fs.make<TH1F>("NJets manyPV", "NJets pt in more than one valid primary vertex ",maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);

   //more pile up plots 
   histograms["nInteractions"] =  fs.make<TH1F>("nInteractions", "Number of interactions", 50,-0.5,49.5);
   histograms["nInteractionsNoWght"] =  fs.make<TH1F>("nInteractionsNoWght", "Number of interactions (No Reweighting)", 50,-0.5,49.5);
   histograms["nInteractions vs nPV"] = fs.make<TH2F>("nInteractions vs nPV", "Number of interactions vs number of primary vertices",50,0,50,50,0,50);
   histograms["nInteractions vs nPVvalid"] = fs.make<TH2F>("nInteractions vs nPVvalid", "Number of interactions vs number of valid primary vertices",50,0,50,50,0,50);

   for (int iJet = minNJets; iJet <= maxNJets; ++iJet) {
         TString histName = "nInteraction_"; histName += iJet; histName += "j";
         TString histTitle = "number of Interactions (N_{jets}"; 
         histTitle += ((iJet == maxNJets) ? "#geq" : "=");
         histTitle += iJet;
         histTitle += ")";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 50, -0.5, 49.5);
         // cout<< "hist name " << histName << endl;
   }
   histograms["nInteractions vs relIso"]= fs.make<TH2F>("nInteractions vs relIso", "number of interactions vs relIso", 50, -0.5,49.5, 400, 0, 1.25);
   histograms["nInteractions vs relHadIso"]= fs.make<TH2F>("nInteractions vs relHadIso", "number of interactions vs relative Hadronic Isolation",  50, -0.5,49.5, 400, 0, 1.25);
   histograms["nInteractions vs PFIso"]= fs.make<TH2F>("nInteractions vs PFIso", "number of interactions vs  PF Iso", 50, -0.5,49.5, 400, 0, 1.25);
   histograms["nInteractions vs met"]= fs.make<TH2F>("nInteractions vs met", "number of interactions vs met",  50, -0.5,49.5, 50, 0, 200);
   histograms["nInteractions vs nJets"]= fs.make<TH2F>("nInteractions vs nJets", "number of interactions vs number of jets",  50, -0.5,49.5,  maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);

   //Fill plots per nInteractions - offline will combine them into nInteraction regions
   for (int i =0; i <= 10; ++i) {  
         TString histName = "nJets_"; histName += i; histName += "int";
         TString histTitle = "number of jets (N_{interactions}"; 
         histTitle += ((i== 10) ? "#geq" : "=");
         histTitle += i;
         histTitle += ")";
         histograms[histName] = fs.make<TH1F>(histName, histTitle,  maxNJets - minNJets + 1, minNJetsFloat, maxNJetsFloat);
         // cout<< "hist name " << histName << endl;
      }

   for (int i =0; i <= 10; ++i) {  
         TString histName = "relIso_"; histName += i; histName += "int";
         TString histTitle = "number of jets (N_{interactions}"; 
         histTitle += ((i== 10) ? "#geq" : "=");
         histTitle += i;
         histTitle += ")";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 400, 0, 1.25);
      }

    for (int i =0; i <= 10; ++i) {  
         TString histName = "relHadIso_"; histName += i; histName += "int";
         TString histTitle = "number of jets (N_{interactions}"; 
         histTitle += ((i== 10) ? "#geq" : "=");
         histTitle += i;
         histTitle += ")";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 400, 0, 1.25);
      }

    for (int i =0; i <= 10; ++i) {  
         TString histName = "PFIso_"; histName += i; histName += "int";
         TString histTitle = "number of jets (N_{interactions}"; 
         histTitle += ((i== 10) ? "#geq" : "=");
         histTitle += i;
         histTitle += ")";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 400, 0, 1.25);
      }

     for (int i =0; i <= 10; ++i) {  
         TString histName = "met_"; histName += i; histName += "int";
         TString histTitle = "number of jets (N_{interactions}"; 
         histTitle += ((i== 10) ? "#geq" : "=");
         histTitle += i;
         histTitle += ")";
         histograms[histName] = fs.make<TH1F>(histName, histTitle, 400, 0, 1.25);
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
   //Skip the selector and just fill the collections directly
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
   //btag -1 = no btag, 1= 1 btag, 0 = 0 btags
   double btag = shyftParameters.getParameter<double>("btag");
   cout << "number of b-tags:  " << btag << endl;
   //double btagWP = 0.244; //Loose CSV working point
   double btagWP = 0.679; //Medium CSV working point 
   bool passTagReq;
   double PFIsoMin = shyftParameters.getParameter<double>("PFIsoMin");
   cout << "PFIso min cut: " <<  PFIsoMin << endl;
   TString muTrig_ = shyftParameters.getParameter<string>("muTrig");
   cout << "Cutting on Trigger: " << muTrig_ << endl;
     TString JES = shyftParameters.getParameter<string>("JES");
   std::string JECUncertaintyFile_ =  shyftParameters.getParameter<string>("JECUncertaintyFile");
   cout <<"JES:" <<JES<< endl;

   
   JetCorrectionUncertainty *jecUnc_;// = new JetCorrectionUncertainty(JECUncertaintyFile_);
   //get the JEC correction uncertainty for all sources 
   jecUnc_ =  new JetCorrectionUncertainty(*( new JetCorrectorParameters(JECUncertaintyFile_, "Total")));
   

   //loop through each event
   cout << "Beginning event loop." << endl;
   for( ev.toBegin();
        ! ev.atEnd();
        ++ev, ++nTotal) {
     passTagReq = false;
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
      bool passed = false;

      if (skipSel) {
        passed = true;
        ++nInclusive;
      } else {
        passed = wPlusJets(ev, ret);
        if (ret[std::string("Inclusive")]) ++nInclusive;
      }

      //Since we only care about selected events, why make this more
      //complicated than it has to be?
      if (!passed) continue; // I want events to pass trigger only for data.
      ++nPassed;

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
       

        if(btag == -1){passTagReq = true;} //assume the event will pass
        else{passTagReq = false;}
        double nTags = 0;
        edm::Handle< vector< pat::Jet > > jetHandle;
        ((edm::EventBase*)&ev)->getByLabel(shyftParameters.getParameter<edm::InputTag>("jetSrc"),jetHandle);
        for ( std::vector<pat::Jet>::const_iterator jetBegin = jetHandle->begin(),
                jetEnd = jetHandle->end(), ijet = jetBegin;
              ijet != jetEnd; ++ijet ) {

          //BTagging
          double bDiscCSV = ijet->bDiscriminator("combinedSecondaryVertexBJetTags");
          //cout << " bDiscCSV: " << bDiscCSV << endl;
          if(bDiscCSV>=btagWP){
            nTags += 1;
          }
          
          
          //JES
          double ptScale = 1.0;
          jecUnc_->setJetEta(ijet->eta());
          jecUnc_->setJetPt(ijet->pt());
          double uncert = jecUnc_->getUncertainty(true);
          if (JES == "up") {
            ptScale *= (1 + uncert);
          } else if (JES == "down") {
            ptScale *= (1 - uncert);
          }
                   
           reco::ShallowClonePtrCandidate scaledJet (reco::ShallowClonePtrCandidate(edm::Ptr<pat::Jet>(jetHandle, ijet - jetBegin),
                                                                                              ijet->charge(),
                                                                                              ijet->p4()* ptScale));
           

          tempJets.push_back(reco::ShallowClonePtrCandidate( edm::Ptr<pat::Jet>(jetHandle, ijet - jetBegin)));

        }
      
      //Does the event pass our btag cut?
      //cout << "nTags:  " << nTags << endl;
      if(btag == 0 && nTags ==0){passTagReq = true;}
      if(btag > 0){
        if(nTags >= btag){passTagReq = true;}
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
 
      std::vector<reco::ShallowClonePtrCandidate> const & electrons = skipSel ? tempElectrons : wPlusJets.selectedElectrons();
      std::vector<reco::ShallowClonePtrCandidate> const & muons     = skipSel ? tempMuons : wPlusJets.selectedMuons();
      std::vector<reco::ShallowClonePtrCandidate> const & jets      = skipSel ? tempJets :wPlusJets.cleanedJets();
      //This list may still have electrons (and muons?) double-counted as jets
      //For "skipSel" just pass in the original jet collection again...
      std::vector<reco::ShallowClonePtrCandidate> const & jetsBeforeClean = skipSel ? tempJets : wPlusJets.selectedJets();
      reco::ShallowClonePtrCandidate   const & met             = skipSel ? tempMET : wPlusJets.selectedMET();

     
      

      //Some quantities needed for plots:
      int charge = 0;  //Filled in from electron or muon charge
      int nJetsPlot = (int) jets.size();
      if (nJetsPlot > maxNJets) nJetsPlot = maxNJets;
      double relIso;
      double relHadIso;
      double PFIso;

      //Common lepton kinematics
      reco::Particle::LorentzVector leptonLV;
      const reco::GenParticle *genLepton = 0;
     
      
      //more PU info

      int numInteractions  = -1;
      double weight = 1;
      double PUweight =1;
      
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
        PUweight = LumiWeights_.weight3D( nm1,n0,np1);
      }

           

      //Muon-only plots
      const pat::Muon * muonOrig_ = 0;
      if ( muPlusJets_ ) {
         if (muons.size() > 0){

           //cout << "=============== NEW EVENT shyft_pretag Muon unpacking ======================" << endl;

           //cout <<"Trying to get the muon from the selectedMuons" << endl;
           
	   //Note: This should only be used for muon-specific info.
	   //Use the original muons[0] object for kinematics since the
	   //"ShallowClonePtr" may have a rescaled 4-vector.
           muonOrig_ = dynamic_cast<const pat::Muon *>(muons[0].masterClonePtr().get());
           double muonSF = 1;
           
           if (muonOrig_ == 0 ) {

             cout << "Oops, we don't have a valid muon pointer in this event, skipping it" << endl;
             continue;

           } else {
             //cout << "We have the muon pointer and it is fine." << endl;
           }

            if (muonOrig_->pt()<muPtMin){
             //  cout << "muPt: "<< muonOrig_->pt()<< " does not pass muPtMin: " <<  muPtMin << endl;
             continue;
           }

            //PFIsolation Variables
            double particleIso =  muonOrig_->particleIso();
            double chargedHadronIso =  muonOrig_->chargedHadronIso();
            double neutralHadronIso =  muonOrig_->neutralHadronIso();
            double photonIso =  muonOrig_->photonIso();
            double iMuonPt  = muons[0].pt();
             relHadIso = (iMuonPt != 0 ) ? (chargedHadronIso + neutralHadronIso) / iMuonPt : 0;
            PFIso = (iMuonPt != 0 ) ? (chargedHadronIso + neutralHadronIso + photonIso) / iMuonPt : 0;

            //Cut on a min PF Iso
            if (PFIso< PFIsoMin){
              // cout << "PFIso: " << PFIso << " less than the min PFIso of " <<  PFIsoMin << endl;
            continue;
            }

            //Apply a trigger cut
            TRegexp  muTrigRegexp(muTrig_);
            bool matchMuTrigName = false;
            bool passTrigReq = false;
            edm::InputTag trigTag(shyftParameters.getParameter<edm::InputTag>("trigSrc"));
            edm::Handle<pat::TriggerEvent> triggerEvent;
            ((edm::EventBase *)&ev)->getByLabel(trigTag, triggerEvent);
            pat::TriggerEvent const * trig = &*triggerEvent;            

            //loop over TriggerPathCollection which is the full trigger menu for this event
            for(pat::TriggerPathCollection::const_iterator iPath = trig->paths()->begin();
                iPath != trig->paths()->end(); ++iPath){
              TString thisTrigPath = iPath->name();
              //Select our trigger from the full trigger menu
              matchMuTrigName =  thisTrigPath.Contains(muTrigRegexp);
              if(matchMuTrigName == true){
                //cout << "Triger Path : " << thisTrigPath << endl; 
                //get our trigger path from the list of passed trigger
                pat::TriggerPath const * muPath = trig->path(iPath->name());
                //Here we check if the event passed our trigger
                if(muPath != 0 && muPath->wasAccept() ) {
                  passTrigReq = true;
                  // cout << "This Trigger path was accepted"  << endl;
                }
              }
            }
               

            if(passTrigReq == false){
              continue;
            }
            
             if(!ev.isRealData()){
               //Muon SF is a combination of the muon ID SF 0.987 and the IsoMu24 trigger SF a fun. of eta
               if(muonOrig_->eta()>= -2.1 && muonOrig_->eta()< -1.5 ){muonSF =0.987*1.003;}
               if(muonOrig_->eta()>= -1.5 && muonOrig_->eta()< -1.2 ){muonSF =0.987*0.980;}
               if(muonOrig_->eta()>= -1.2 && muonOrig_->eta()< -0.9 ){muonSF =0.987*0.941;}
               if(muonOrig_->eta()>= -0.9 && muonOrig_->eta()< 0    ){muonSF =0.987*0.974;}
               if(muonOrig_->eta()>= 0    && muonOrig_->eta()< 0.9  ){muonSF =0.987*0.977;}
               if(muonOrig_->eta()>= 0.9  && muonOrig_->eta()< 1.2  ){muonSF =0.987*0.939;}
               if(muonOrig_->eta()>= 1.2  && muonOrig_->eta()< 1.5  ){muonSF =0.987*0.967;}
               if(muonOrig_->eta()>= 1.5  && muonOrig_->eta()< 2.1  ){muonSF =0.987*1.023;}
             }
             
            //weight each event by the muon SF and the PU weight
            weight = PUweight * muonSF;       

           leptonLV = muons[0].p4();
           genLepton = muonOrig_->genLepton();
           
           charge = muons[0].charge();  
           
         
         
            
	   histograms["muPt"]->Fill(muons[0].pt(),weight);
            if (charge > 0) histograms["muPtPlus"]->Fill(muons[0].pt(),weight);
            if (charge < 0) histograms["muPtMinus"]->Fill(muons[0].pt(),weight);
          
            histograms["muEta"]->Fill(muons[0].eta(),weight);
            histograms["muPhi"]->Fill(muons[0].phi(),weight);
          
            TString histName = "muPt_";
            histName += nJetsPlot;
            histName += "j";
            histograms[histName]->Fill(muons[0].pt(),weight);

            //Fill MuonPt for l+ l- in each jet bin  
            if(charge > 0){
              TString histNameP = "muPtPlus_";
              histNameP += nJetsPlot;
              histNameP += "j";
              histograms[histNameP]->Fill(muons[0].pt(),weight);
            }
            if(charge < 0){
              TString histNameM = "muPtMinus_";
              histNameM += nJetsPlot;
              histNameM += "j";
              histograms[histNameM]->Fill(muons[0].pt(),weight);
            }

            
          
            histName = "muEta_";
            histName += nJetsPlot;
            histName += "j";
            histograms[histName]->Fill(muons[0].eta(),weight);
          
            pat::TriggerObjectRefVector trigObj = trig->pathObjects("HLT_Mu9");
          
            double minDR = 9e20;
          
            for ( pat::TriggerObjectRefVector::const_iterator it = trigObj.begin(); it != trigObj.end(); ++it ) {
               const pat::TriggerObjectRef objRef(*it);
               if (objRef->collection().find("hltL3MuonCandidates") != std::string::npos) {
                  double dR = reco::deltaR(muons[0].eta(),muons[0].phi(),objRef->eta(),objRef->phi());
                  if (dR < minDR) minDR = dR;
               }
            }
            histograms["minTrigDR"]->Fill(minDR,weight);
          
            //////////////Muon Values////////
            double hcalIso  = muonOrig_->hcalIso();
            double ecalIso  = muonOrig_->ecalIso();
            double trkIso   = muonOrig_->trackIso();
            relIso = (iMuonPt != 0 ) ? (ecalIso + hcalIso + trkIso) / iMuonPt : 0;
           
            //muon parameters
            double muon_d0 =  muonOrig_->dB();
            double muon_z0 =  muonOrig_->vertex().z();

            histograms["mu d0"]->Fill(muon_d0, weight);
            histograms["mu z0"]->Fill(muon_z0, weight);
          
            unsigned stationMask = muonOrig_->stationMask();
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
          
            reco::TrackRef globalTrack = muonOrig_->globalTrack();

            if (!globalTrack.isNonnull()){
              cout << "Oh no! You have a muon that doesn't have a global track :("
                   << "Number of muons in this event is " << muons.size() << endl
                   << "muon.isGlobalMuon() = " << muons[0].isGlobalMuon() << endl
                   << "muon.isTrackerMuon() = " << muons[0].isTrackerMuon() << endl
                   << "muon.isStandAloneMuon() = " << muons[0].isStandAloneMuon() << endl
                   << endl;
              
              cout << "Skipping the rest of the event" << endl;

              continue;
            }
            reco::HitPattern globalTrackHits = globalTrack->hitPattern();
          
            int numberOfMuonHits = globalTrackHits.numberOfValidMuonHits();
            int numberOfStripHits = globalTrackHits.numberOfValidTrackerHits();
            int numberOfPixelLayersWithMeasurement = globalTrackHits.pixelLayersWithMeasurement();
            int missingTrackerHits = globalTrackHits.numberOfLostStripHits();
          
          
            histograms["hcalIso"]->Fill(hcalIso,weight);
            histograms["ecalIso"]->Fill(ecalIso,weight); 
            histograms["trkIso"]->Fill(trkIso,weight);
            histograms["relIso"]->Fill(relIso,weight);
            
            //more isolation plots
            histograms["particleIso"]->Fill(particleIso,weight);
            histograms["chargedHadronIso"]->Fill(chargedHadronIso,weight);
            histograms["neutralHadronIso"]->Fill(neutralHadronIso,weight);
            histograms["photonIso"]->Fill(photonIso,weight);
            histograms["relHadIso"]->Fill(relHadIso,weight);
            histograms["PFIso"]->Fill(PFIso,weight);
            histograms["relIso vs PFIso"]->Fill(relIso, PFIso);


            histograms["nDTStations"]->Fill(nDTStations,weight); 
            histograms["nCSCStations"]->Fill(nCSCStations,weight); 
            histograms["nTotalStations"]->Fill(nDTStations+nCSCStations,weight);
          
            histograms["numberOfMuonHits"]->Fill(numberOfMuonHits,weight); 
            histograms["numberOfStripHits"]->Fill(numberOfStripHits,weight); 
            histograms["numberOfPixelLayersWithMeasurement"]->Fill(numberOfPixelLayersWithMeasurement,weight); 
            histograms["missingTrackerHits"]->Fill(missingTrackerHits,weight); 
          
            if (genLepton) {
               double  mu_pt= muons[0].pt();
               double  mu_eta= muons[0].eta();
               double  mu_phi= muons[0].phi();
	  
               double gmu_pt= genLepton->pt();
               double gmu_eta= genLepton->eta();
               double gmu_phi= genLepton->phi(); 
             
               double MCtruth_Mu_pt =  (gmu_pt -mu_pt)/(gmu_eta);
               double MCtruth_Mu_eta = (gmu_eta -mu_eta)/(gmu_eta);
               double MCtruth_Mu_phi = (gmu_phi -mu_phi)/(gmu_phi);
             
               //"MC Truth Pt"    histograms["MC Truth Mu Eta"]
               histograms["MC Truth Mu Pt"]->Fill(MCtruth_Mu_pt,weight);
               histograms["MC Truth Mu Eta"]->Fill(MCtruth_Mu_eta,weight);
               histograms["MC Truth Mu Phi"]->Fill(MCtruth_Mu_phi,weight);
            }
         } else if (!ePlusJets_) {
            continue; //Skip events that don't have muons unless we're also allowing for electrons
         }
      }//muPlusJets_
      
       //Pile Up Plots
      
       edm::Handle<reco::VertexCollection> vertices;
       std::string vertexColl = skipSel ? "goodOfflinePrimaryVertices" : "offlinePrimaryVertices";
      ((edm::EventBase*)&ev)->getByLabel(edm::InputTag(vertexColl), vertices);

      reco::VertexCollection vertexCollection = *(vertices.product());
      int vertex_number = vertexCollection.size();
      int vertex_number_valid =0;

      // cout<< "Number of Verticies in the Event: " << vertex_number << endl;

      histograms["nPV"]->Fill(vertex_number,weight);
      TString histNameNPV = "nPV_"; histNameNPV+= nJetsPlot; histNameNPV += "j";
      histograms[histNameNPV]->Fill(vertex_number,weight);
//        if(nJetsPlot>=3){
//         histograms["nPV_3j"]->Fill(vertex_number,weight);
//       }


      for (reco::VertexCollection::const_iterator itr = vertexCollection.begin(); itr != vertexCollection.end(); ++itr){
        if ( !(itr->isFake())&& itr->ndof() > 4 && abs(itr->z()) <= 15 && itr->position().Rho() <= 2 ) vertex_number_valid++;
       
      }

      histograms["nPVvalid"]->Fill(vertex_number_valid,weight);
      // cout<< "Is the vertex valid " << vertex_number_valid << endl;
       if(nJetsPlot>=3){
       histograms["nPVvalid_3j"]->Fill(vertex_number_valid,weight);
      }

      int run = ev.id().run();
      int lumi = ev.id().luminosityBlock();

      histograms["nPVvsRUN"]->Fill(run,vertex_number_valid);
      histograms["nPVvsLUMI"]->Fill(lumi,vertex_number_valid);


      if(muPlusJets_){
	if( vertex_number_valid ==1 ){
	  histograms["MuPt 1PV"]->Fill(muons[0].pt(),weight);
	  histograms["NJets 1PV"]->Fill(nJetsPlot,weight);
	  if(nJetsPlot ==2){ histograms["MuPt 1PV 2j"]->Fill(muons[0].pt(),weight);
	  }
	  if(nJetsPlot ==3){ histograms["MuPt 1PV 3j"]->Fill(muons[0].pt(),weight);
	  }
	  if(nJetsPlot >=4){ histograms["MuPt 1PV >=4j"]->Fill(muons[0].pt(),weight);
	  }
	}
	if(vertex_number_valid >=2){
	  histograms["MuPt manyPV"]->Fill(muons[0].pt(),weight);
	  histograms["NJets manyPV"]->Fill(nJetsPlot,weight);
	  if(nJetsPlot ==2){ histograms["MuPt manyPV 2j"]->Fill(muons[0].pt(),weight);
	  }
	  if(nJetsPlot ==3){ histograms["MuPt manyPV 3j"]->Fill(muons[0].pt(),weight);
	  }
	  if(nJetsPlot >=4){ histograms["MuPt manyPV >=4j"]->Fill(muons[0].pt(),weight);
	  }
	}
      } //muPlusJets
      
      if(!ev.isRealData()){
	
	histograms["nInteractions"]->Fill(numInteractions,weight);
	histograms["nInteractionsNoWght"]->Fill(numInteractions);
	histograms["nInteractions vs nPV"]->Fill(numInteractions,vertex_number);
	histograms["nInteractions vs nPVvalid"]->Fill(numInteractions,vertex_number_valid);
	
	histograms["nInteractions vs relIso"]->Fill(numInteractions,relIso);
	histograms["nInteractions vs relHadIso"]->Fill(numInteractions,relHadIso);
	histograms["nInteractions vs PFIso"]->Fill(numInteractions,PFIso);
	histograms["nInteractions vs met"]->Fill(numInteractions,met.et());
	histograms["nInteractions vs nJets"]->Fill(numInteractions, nJetsPlot);
	
	

	//Fill only the plot for the appropriate jet bin
	TString nIntByJetPlotName = "nInteraction_";
	nIntByJetPlotName += nJetsPlot;
	nIntByJetPlotName += "j";
	histograms[nIntByJetPlotName]->Fill(numInteractions,weight);
	
	//Fill only the plot for the appropriate number of interactions
	TString nJetsBynIntPlotName = "nJets_";
	nJetsBynIntPlotName += ((numInteractions >= 10) ? 10: numInteractions);
	nJetsBynIntPlotName += "int";
	histograms[nJetsBynIntPlotName]->Fill(nJetsPlot,weight);
	
	//Fill only the plot for the appropriate number of interactions
	TString relIsoBynIntPlotName = "relIso_";
	relIsoBynIntPlotName += ((numInteractions >= 10) ? 10: numInteractions);
	relIsoBynIntPlotName += "int";
	histograms[relIsoBynIntPlotName]->Fill(relIso,weight);
	
	//Fill only the plot for the appropriate number of interactions
	TString relHadIsoBynIntPlotName = "relHadIso_";
	relHadIsoBynIntPlotName += ((numInteractions >= 10) ? 10: numInteractions);
	relHadIsoBynIntPlotName += "int";
	histograms[relHadIsoBynIntPlotName]->Fill(relHadIso,weight);
	
	//Fill only the plot for the appropriate number of interactions
	TString relHPhIsoBynIntPlotName = "PFIso_";
	relHPhIsoBynIntPlotName += ((numInteractions >= 10) ? 10: numInteractions);
	relHPhIsoBynIntPlotName += "int";
	histograms[relHPhIsoBynIntPlotName]->Fill(PFIso,weight);
	
	//Fill only the plot for the appropriate number of interactions
	TString metBynIntPlotName = "met_";
	metBynIntPlotName += ((numInteractions >= 10) ? 10: numInteractions);
	metBynIntPlotName += "int";
	histograms[metBynIntPlotName]->Fill(met.et(),weight);
	
      }

      
      

      //Electron-only plots
      const pat::Electron * electronOrig_ = 0;
      
      if ( ePlusJets_ ) {     
         if (electrons.size() > 0){

	   //Note: This should only be used for muon-specific info.
	   //Use the original muons[0] object for kinematics since the
	   //"ShallowClonePtr" may have a rescaled 4-vector.
	   electronOrig_ = dynamic_cast<const pat::Electron *>(electrons[0].masterClonePtr().get());
	   leptonLV = electrons[0].p4();
	   genLepton = electronOrig_->genLepton();
	   charge = electrons[0].charge();  
	   if(electronOrig_->isEE()){
	     histograms["ePt_EE"]->Fill(electrons[0].pt(),weight);
	     if (charge > 0) histograms["ePtPlus_EE"]->Fill(electrons[0].pt(),weight);
	     if (charge < 0) histograms["ePtMinus_EE"]->Fill(electrons[0].pt(),weight);        
	     histograms["eEta_EE"]->Fill(electrons[0].eta(),weight);
	     histograms["ePhi_EE"]->Fill(electrons[0].phi(),weight);
	     
	     TString histName = "ePt_";
	     histName += nJetsPlot;
	     histName += "j_EE";
	     histograms[histName]->Fill(electrons[0].pt(),weight);
	     
	     histName = "eEta_";
	     histName += nJetsPlot;
	     histName += "j_EE";
	     histograms[histName]->Fill(electrons[0].eta(),weight);
	     
	   }
	   else if(electronOrig_->isEB()){
	     histograms["ePt_EB"]->Fill(electrons[0].pt(),weight);
	     if (charge > 0) histograms["ePtPlus_EB"]->Fill(electrons[0].pt(),weight);
	     if (charge < 0) histograms["ePtMinus_EB"]->Fill(electrons[0].pt(),weight);        
	     histograms["eEta_EB"]->Fill(electrons[0].eta(),weight);
	     histograms["ePhi_EB"]->Fill(electrons[0].phi(),weight);
             
	     TString histName = "ePt_";
	     histName += nJetsPlot;
	     histName += "j_EB";
	     histograms[histName]->Fill(electrons[0].pt(),weight);
	     
	     histName = "eEta_";
	     histName += nJetsPlot;
	     histName += "j_EB";
	     histograms[histName]->Fill(electrons[0].eta(),weight);
	   }
	   
	   if (genLepton) {
	     double  el_pt= electrons[0].pt();
	     double  el_eta= electrons[0].eta();
	     double  el_phi= electrons[0].phi();
             
	     double gel_pt= genLepton->pt();
	     double gel_eta= genLepton->eta();
	     double gel_phi= genLepton->phi();
             
	     double MCtruth_Ele_pt =  (gel_pt -el_pt)/(gel_eta);
	     double MCtruth_Ele_eta = (gel_eta -el_eta)/(gel_eta);
	     double MCtruth_Ele_phi = (gel_phi -el_phi)/(gel_phi);
             
	     //"MC Truth Pt"    histograms["MC Truth Mu Eta"]
	     histograms["MC Truth Ele Pt"]->Fill(MCtruth_Ele_pt,weight);
	     histograms["MC Truth Ele Eta"]->Fill(MCtruth_Ele_eta,weight);
	     histograms["MC Truth Ele Phi"]->Fill(MCtruth_Ele_phi,weight);
	   }
         } else if (!muPlusJets_){
	   continue;
         }
	 
      }//ePlusJets_
            

      //Fill jet count plots
      
      histograms["nJets"]->Fill(nJetsPlot,weight);
    
      if (charge > 0){
         histograms["nJetsPlus"]->Fill(nJetsPlot,weight);
      }
      else if (charge < 0){
         histograms["nJetsMinus"]->Fill(nJetsPlot,weight);
      }
   
      int nJetsBeforeCleanPlot = ( ((int) jetsBeforeClean.size()) > maxNJets) ? maxNJets : jetsBeforeClean.size();
      histograms["nJetsBeforeClean"]->Fill(nJetsBeforeCleanPlot,weight);

      //Fake JES
      // reco::Particle::LorentzVector jetFake4Vec;


      double dRjj_min = 1000;
      double dRjj_max = 0; 
      //Fill individual jet plots
      for (unsigned iJet = 0; (int)iJet < nJetsPlot; ++iJet) {

        TString histName = "jet"; histName += (iJet+1);

         histograms[histName + "Pt"]->Fill(jets[iJet].pt(),weight);
         histograms[histName + "Eta"]->Fill(jets[iJet].eta(),weight);
         histograms[histName + "Phi"]->Fill(jets[iJet].phi(),weight);
         histograms[histName + "Et"]->Fill(jets[iJet].et(),weight);
         
         
          histograms[histName + "Mass"]->Fill(jets[iJet].mass(),weight);

         TString histNameDPhiMet = "dPhiMetJet"; histNameDPhiMet += (iJet+1);
         histograms[histNameDPhiMet]->Fill(fabs(reco::deltaPhi(met.phi(),jets[iJet].phi())),weight);

         TString histName2 = histName + "Pt_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jets[iJet].pt(),weight);
         histName2 = histName + "Eta_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jets[iJet].eta(),weight);
         histName2 = histName + "Phi_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jets[iJet].phi(),weight);
         histName2 = histName + "Et_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jets[iJet].et(),weight);
         
           
         histName2 = histName + "Mass_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(jets[iJet].mass(),weight);
         histName2 = histNameDPhiMet+"_"; histName2+= nJetsPlot; histName2 += "j";
         histograms[histName2]->Fill(fabs(reco::deltaPhi(met.phi(),jets[iJet].phi())),weight);
	  
         const pat::Jet *origJet_ = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
         
         if(muPlusJets_){
           double jesUncert = wPlusJets.getPtEtaJESUncert(*origJet_);
           double tempPlusUncert = 0.053;
           double sumQuadUncert = sqrt(jesUncert*jesUncert + tempPlusUncert*tempPlusUncert);
         
           histograms["jesUncert"]->Fill(jesUncert,weight);
           histograms["jesUncertVsEta"]->Fill(jets[iJet].eta(), jesUncert);
           histograms["jesUncertVsPt"]->Fill(origJet_->pt(), jesUncert);
           histograms["jesUncertPlus"]->Fill(sumQuadUncert,weight);
           histograms["jesUncertVsEtaPlus"]->Fill(jets[iJet].eta(), sumQuadUncert);
         
           histograms["jesUncertVsPtPlus"]->Fill(origJet_->pt(), sumQuadUncert);
	 }
         //Jet energy correction information
         // std::vector<std::string> JetCorrections = origJet_->availableJECLevels();
         // int sizeJetCorrVec = JetCorrections.size();
         // cout << endl << "jet correction vector" << endl;
         // for(int i = 0; i<sizeJetCorrVec; i++){
         // cout << JetCorrections[i]<< endl;
         // }
         

         const reco::GenJet *myGenJet = origJet_->genJet();
         if (myGenJet){
           double deltaPt = (jets[iJet].pt() - myGenJet->pt());
           double resoPt = deltaPt/myGenJet->pt();

           if (myGenJet->pt() > 15){
             histograms["genDeltaJetPt"]->Fill(deltaPt,weight);
             histograms["genResoJetPt"]->Fill(resoPt,weight);
           }
         }

     //dRjj Variables 
     for(unsigned jJet= 0; jJet<iJet; ++jJet){
       double dRjj = reco::deltaR(jets[iJet].eta(),jets[iJet].phi(),jets[jJet].eta(),jets[jJet].phi());
       if(dRjj<dRjj_min){
         dRjj_min = dRjj;
       }
       if(dRjj>dRjj_max){
          dRjj_max = dRjj;
       }
     }//jJet
           
      }//iJet

      //dRjj of leading 2 jets
      if(jets.size()>1){
      double dRjj_lead2 = reco::deltaR(jets[0].eta(),jets[0].phi(),jets[1].eta(),jets[1].phi());
      histograms["dRjj_lead2"]->Fill(dRjj_lead2, weight);
      }
      
      //fill dRjj min and max plots
      histograms["dRjj_min"]->Fill(dRjj_min, weight);
      histograms["dRjj_max"]->Fill(dRjj_max, weight);
      
      
      double minDphi = 999. ;
      if (nJetsPlot >= 2){

	// jet kinematic variables
	histograms["dEtaJetJet"]->Fill(fabs(jets[0].phi() - jets[1].phi()),weight);
	histograms["dPhiJetJet"]->Fill(fabs(reco::deltaPhi(jets[0].phi(),jets[1].phi())),weight);
	for (int iJet = 0; iJet < (int)jets.size(); ++iJet) {
	  double deltaPhilj =  fabs(reco::deltaPhi(leptonLV.phi(), jets[iJet].phi()));
	  if ( deltaPhilj < minDphi ) minDphi = deltaPhilj ; 
	}
	histograms["minDPhiLepJet"]->Fill(minDphi,weight);
      }
      
      // jet kinematic variables // Andrea's version
     double minDeta = 999 ;
     double dEtajj = 0 ;
     double dPhijj = 0 ;
     double dPhilj = 0 ;
     double m2jj = 0 ;
     double mWWjj = 0 ;

     if (nJetsPlot >= 2 && nJetsPlot < 4) {  
       for (int iJet = 0; iJet < (int)jets.size()-1; ++iJet) {
	 for (int jJet = iJet+1; jJet < (int)jets.size(); ++jJet) {
	   double deltaEta = fabs( jets[iJet].eta() - jets[jJet].eta() );
	   if ( deltaEta < minDeta ) {
	     minDeta = deltaEta ; 
	     dEtajj = minDeta ;
	     dPhijj = fabs(reco::deltaPhi(jets[iJet].phi(),jets[jJet].phi())) ;
	     if ( fabs(reco::deltaPhi(leptonLV.phi(), jets[iJet].phi())) < fabs(reco::deltaPhi(leptonLV.phi(), jets[jJet].phi())) ) { 
	       dPhilj = fabs(reco::deltaPhi(leptonLV.phi(), jets[iJet].phi())) ;
	     }
	     else {  dPhilj = fabs(reco::deltaPhi(leptonLV.phi(), jets[jJet].phi())) ;}
	     reco::Particle::LorentzVector m2jj_an = jets[iJet].p4() + jets[jJet].p4() ;
	     m2jj = m2jj_an.M() ; 
	     reco::Particle::LorentzVector MWWjj_an = leptonLV + met.p4() + m2jj_an ; 	 
	     mWWjj = MWWjj_an.M() ;  
 
	   }
	 }
       }
       
       histograms["dEtaJetJet_an"]->Fill(dEtajj,weight);
       histograms["dPhiJetJet_an"]->Fill(dPhijj,weight);
       histograms["minDPhiLepJet_an"]->Fill(dPhilj,weight);
       histograms["m2_an"]->Fill(m2jj,weight);
       histograms["MWWTop2jets_an"]->Fill(mWWjj,weight);

     }



      //Jackson angles
      //Call the funtion in AngularVars.h
      if (nJetsPlot == 2) {  
	reco::Particle::LorentzVector METLV = met.p4();
	reco::Particle::LorentzVector jet1 = jets[0].p4();
	reco::Particle::LorentzVector jet2 = jets[1].p4();
	 
	float cosdPhiWW=0.; float cosdPhiWH=0.; float costhetal=0.; 
	float costhetaj=0.; float costhetaWH=0.; 
	TLorentzVector pu(leptonLV.Px(),leptonLV.Py(),leptonLV.Pz(),leptonLV.E()); 
	TLorentzVector pv(METLV.Px(),METLV.Py(),METLV.Pz(),METLV.E()); 
	TLorentzVector pj1(jet1.Px(),jet1.Py(),jet1.Pz(),jet1.E()); 
	TLorentzVector pj2(jet2.Px(),jet2.Py(),jet2.Pz(),jet2.E());
	
	//	cout << "momentum " << pu.Px() << " " << pu.Py()  << endl;
	
	dg_kin_Wuv_Wjj( pu, pv, pj1, pj2, cosdPhiWW, cosdPhiWH, costhetal, costhetaj, costhetaWH);
	
	//	cout << "cos angles " << cosdPhiWW << " " << costhetal << " " << costhetaj << endl;
	histograms["Cos_dPhiWW"]->Fill(cosdPhiWW,weight);
	histograms["CosTheta_l"]->Fill(costhetal,weight);
	histograms["CosTheta_j"]->Fill(costhetaj,weight);
	histograms["CosTheta_W1"]->Fill(costhetaWH,weight);
	histograms["Cos_dPhiWH"]->Fill(cosdPhiWH,weight);
      }
            

      //Calculate Ht using all available jets
      double ht = 0.0;
      if (muPlusJets_ && muons.size() > 0) ht += muons[0].pt();
      if (ePlusJets_ && electrons.size() > 0) ht += electrons[0].pt();

      double htLep = ht + met.et(); //htLep = only lepPt and MET

      for (unsigned int iJet = 0; iJet < jets.size(); ++iJet) {
           ht += jets[iJet].et();
      }

      double htAll = ht + met.et(); //htAll = everything: lepPt + MET + sum(jetEt)


      //Debugging output
      if (nDumped < maxToDump || maxToDump < 0) {
         int run = ev.id().run();
         int event = ev.id().event();

         int lumi = ev.id().luminosityBlock();
         std::cout << "=== Run " << run  << " Ev " << event << " Lumi " << lumi;
         std::cout << " Ht " << ht << " MET " <<met.et();
         if (muPlusJets_ && muons.size() > 0) std::cout << " pt_mu " << muons[0].pt();
         if (ePlusJets_ && electrons.size() > 0) std::cout << " pt_el " << electrons[0].pt();

         for (unsigned int iJet = 0; iJet < jets.size(); ++iJet) {
           std::cout << " Jet " << iJet << " Et " << jets[iJet].et();
         }
         std::cout << std::endl;
      
         ++nDumped;
      }//debugging
              
      //Fill Ht plots ("conventional"):
      // All jet bins
      if(muPlusJets_){
         histograms["ht"]->Fill(ht,weight);
         if (charge > 0)  histograms["htPlus"]->Fill(ht,weight);
         if (charge < 0)  histograms["htMinus"]->Fill(ht,weight);

         histograms["htVsNJet"]->Fill(nJetsPlot,ht);
         
         //Fill only the plot for the appropriate jet bin
         TString htPlotName = "ht_";
         htPlotName += nJetsPlot;
         htPlotName += "j";
         histograms[htPlotName]->Fill(ht,weight);
         

         //Fill Ht plots (lepton + MET only):
         // All jet bins
         histograms["htLep"]->Fill(htLep,weight);
         if (charge > 0)  histograms["htLepPlus"]->Fill(htLep,weight);
         if (charge < 0)  histograms["htLepMinus"]->Fill(htLep,weight);

         histograms["htLepVsNJet"]->Fill(nJetsPlot,htLep);
         
         //Fill only the plot for the appropriate jet bin
         htPlotName = "htLep_";
         htPlotName += nJetsPlot;
         htPlotName += "j";
         histograms[htPlotName]->Fill(htLep);
         //Fill Ht plots (lepton + MET + sum(jetEt),weight):
         // All jet bins
         histograms["htAll"]->Fill(htAll,weight);
         if (charge > 0)  histograms["htAllPlus"]->Fill(htAll,weight);
         if (charge < 0)  histograms["htAllMinus"]->Fill(htAll,weight);
         
         histograms["htAllVsNJet"]->Fill(nJetsPlot,htAll);
         
         //Fill only the plot for the appropriate jet bin
         htPlotName = "htAll_";
         htPlotName += nJetsPlot;
         htPlotName += "j";
         histograms[htPlotName]->Fill(htAll,weight);

         

      }//muPlusJets
      
      if(ePlusJets_){
      
         if(electronOrig_->isEE()){
            histograms["ht_EE"]->Fill(ht,weight);
            if (charge > 0)  histograms["htPlus_EE"]->Fill(ht,weight);
            if (charge < 0)  histograms["htMinus_EE"]->Fill(ht,weight);
          
            histograms["htVsNJet_EE"]->Fill(nJetsPlot,ht);
          
            //Fill only the plot for the appropriate jet bin
            TString htPlotName = "ht_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EE";
            histograms[htPlotName]->Fill(ht,weight);
          
            //Fill Ht plots (lepton + MET only):
            // All jet bins
            histograms["htLep_EE"]->Fill(htLep,weight);
            if (charge > 0)  histograms["htLepPlus_EE"]->Fill(htLep,weight);
            if (charge < 0)  histograms["htLepMinus_EE"]->Fill(htLep,weight);
          
            histograms["htLepVsNJet_EE"]->Fill(nJetsPlot,htLep);
          
            //Fill only the plot for the appropriate jet bin
            htPlotName = "htLep_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EE";
            histograms[htPlotName]->Fill(htLep,weight);
          
            //Fill Ht plots (lepton + MET + sum(jetEt)):
            // All jet bins
            histograms["htAll_EE"]->Fill(htAll,weight);
            if (charge > 0)  histograms["htAllPlus_EE"]->Fill(htAll,weight);
            if (charge < 0)  histograms["htAllMinus_EE"]->Fill(htAll,weight);
          
            histograms["htAllVsNJet_EE"]->Fill(nJetsPlot,htAll);
          
            //Fill only the plot for the appropriate jet bin
            htPlotName = "htAll_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EE";
            histograms[htPlotName]->Fill(htAll,weight);
          
         }
         else if(electronOrig_->isEB()){
            histograms["ht_EB"]->Fill(ht,weight);
            if (charge > 0)  histograms["htPlus_EB"]->Fill(ht,weight);
            if (charge < 0)  histograms["htMinus_EB"]->Fill(ht,weight);
          
            histograms["htVsNJet_EB"]->Fill(nJetsPlot,ht);
          
            //Fill only the plot for the appropriate jet bin
            TString htPlotName = "ht_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EB";
            histograms[htPlotName]->Fill(ht,weight);

            //Fill Ht plots (lepton + MET only):
            // All jet bins
            histograms["htLep_EB"]->Fill(htLep,weight);
            if (charge > 0)  histograms["htLepPlus_EB"]->Fill(htLep,weight);
            if (charge < 0)  histograms["htLepMinus_EB"]->Fill(htLep,weight);
          
            histograms["htLepVsNJet_EB"]->Fill(nJetsPlot,htLep);
          
            //Fill only the plot for the appropriate jet bin
            htPlotName = "htLep_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EB";
            histograms[htPlotName]->Fill(htLep,weight);
          
            //Fill Ht plots (lepton + MET + sum(jetEt)):
            // All jet bins
            histograms["htAll_EB"]->Fill(htAll,weight);
            if (charge > 0)  histograms["htAllPlus_EB"]->Fill(htAll,weight);
            if (charge < 0)  histograms["htAllMinus_EB"]->Fill(htAll,weight);
          
            histograms["htAllVsNJet_EB"]->Fill(nJetsPlot,htAll);
          
            //Fill only the plot for the appropriate jet bin
            htPlotName = "htAll_";
            htPlotName += nJetsPlot;
            htPlotName += "j_EB";
            histograms[htPlotName]->Fill(htAll,weight);
         }  
      }//ePlusJets_
      
    
      //Calculate some invariant masses
      if (nJetsPlot >= 2) {
         reco::Particle::LorentzVector m2 = jets[0].p4() + jets[1].p4();
         histograms["m2"]->Fill(m2.M(),weight);
         TString m2PlotName = "m2_";
         m2PlotName += nJetsPlot;
         m2PlotName += "j";
         histograms[m2PlotName]->Fill(m2.M(),weight);
      
         if (nJetsPlot >= 3) {         
            reco::Particle::LorentzVector m3Top3Jets = jets[0].p4() + jets[1].p4() + jets[2].p4();
            histograms["m3Top3Jets"]->Fill(m3Top3Jets.M(),weight);
            TString m3PlotName = "m3Top3Jets_";
            m3PlotName += nJetsPlot;
            m3PlotName += "j";
            histograms[m3PlotName]->Fill(m3Top3Jets.M(),weight);
         
	    //Find the combination with the highest vectorial sum pt
	    reco::Particle::LorentzVector m3(0.,0.,0.,0.);
	    for (int iJet = 0; iJet < (int)jets.size()-2; ++iJet) {
	      for (int jJet = iJet+1; jJet < (int)jets.size()-1; ++jJet) {
            for (int kJet = jJet+1; kJet < (int)jets.size(); ++kJet) {
              reco::Particle::LorentzVector m3temp;
              
              m3temp = jets[iJet].p4() + jets[jJet].p4() + jets[kJet].p4();
              
              if (m3temp.pt() > m3.pt()) m3 = m3temp;
            }
	      }
	    }
      
            histograms["m3"]->Fill(m3.M(),weight);
            histograms["m3_pt"]->Fill(m3.pt(),weight);
            m3PlotName = "m3_";
            m3PlotName += nJetsPlot;
            m3PlotName += "j";
            histograms[m3PlotName]->Fill(m3.M(),weight);

         }	
      }
    
      //=======================
      //Calculate some more invariant masses  // searching for new physics
      //=======================
      //max jj delta ETA
      double maxDeta = 0 ;
      if (nJetsPlot >= 2) {  
	for (int iJet = 0; iJet < (int)jets.size()-1; ++iJet) {
	  for (int jJet = iJet+1; jJet < (int)jets.size(); ++jJet) {
	    double deltaEta = fabs( jets[iJet].eta() - jets[jJet].eta() );
	    if ( deltaEta > maxDeta ) maxDeta = deltaEta ; 
	  }
	}
	histograms["maxDeta"]->Fill(maxDeta,weight);
	TString maxDetaPlotName = "maxDeta_";
	maxDetaPlotName += nJetsPlot;
	maxDetaPlotName += "j";
	histograms[maxDetaPlotName]->Fill(maxDeta,weight);
      }
	   
      // Mttbar  
      //-------------------------------------------------------------------------------------------------------

      reco::Particle::LorentzVector MttbarLVTemp =  leptonLV + met.p4();    

      for (int iJet = 0; iJet < (int)jets.size(); ++iJet) {
        
        MttbarLVTemp += jets[iJet].p4();

        //jet correction output
        //cout << "jet " << iJet << " correction "<<  jets[iJet]->currentJECSet()<< endl;
      }
      histograms["Mttbar"]->Fill(MttbarLVTemp.M(),weight);
      TString MttbarPlotName = "Mttbar_";
      MttbarPlotName += nJetsPlot;
      MttbarPlotName += "j";
      histograms[MttbarPlotName]->Fill(MttbarLVTemp.M(),weight);

      histograms["ttbarPt"]->Fill(MttbarLVTemp.pt(),weight);
      TString ttbarPtPlotName = "ttbarPt_";
      ttbarPtPlotName += nJetsPlot;
      ttbarPtPlotName += "j";
      histograms[ttbarPtPlotName]->Fill(MttbarLVTemp.pt(),weight);

      

      
      if(nJetsPlot >= 4){
	reco::Particle::LorentzVector Mttbar4JetsTemp = leptonLV + met.p4();
	for (int i=0; i<4; i++){ Mttbar4JetsTemp += jets[i].p4();}
	histograms["MttbarTop4jets"]->Fill(Mttbar4JetsTemp.M(),weight);
	TString MttbarTop4jetsPlotName = "MttbarTop4jets_";
	MttbarTop4jetsPlotName += nJetsPlot;
	MttbarTop4jetsPlotName += "j";
	histograms[MttbarTop4jetsPlotName]->Fill(Mttbar4JetsTemp.M(),weight);
      }
         
       // M_WW   HWW analysis 	 
      if(nJetsPlot >= 2){ 	 
	reco::Particle::LorentzVector MWW2JetsTemp = leptonLV + met.p4(); 	 
	for (int i=0; i<2; i++){ MWW2JetsTemp += jets[i].p4();} 	 
	histograms["MWWTop2jets"]->Fill(MWW2JetsTemp.M(),weight); 	 
	TString MWWTop2jetsPlotName = "MWWTop2jets_"; 	 
	MWWTop2jetsPlotName += nJetsPlot; 	 
	MWWTop2jetsPlotName += "j"; 	 
	histograms[MWWTop2jetsPlotName]->Fill(MWW2JetsTemp.M(),weight); 	 
      }
      
      //-------------------------------------------------------------------------------------------------------

      //  M2High M2Low M2_dR
      if (nJetsPlot >= 2) {         
	//Find the combination with the lowest vectorial sum pt
	reco::Particle::LorentzVector m2High(0.,0.,0.,0.);
	reco::Particle::LorentzVector m2Low(-999,-999,-999,9999);
	reco::Particle::LorentzVector m2_dR(0.,0.,0.,0.);
	double m2_deltaR = 999; 
	for (int iJet = 0; iJet < (int)jets.size()-1; ++iJet) {
	  for (int jJet = iJet+1; jJet < (int)jets.size(); ++jJet) {
		  reco::Particle::LorentzVector m2temp = jets[iJet].p4() + jets[jJet].p4();
		  double deltaR = reco::deltaR(jets[iJet].eta(),jets[iJet].phi(),jets[jJet].eta(),jets[jJet].phi());
		  if (m2temp.pt() > m2High.pt()) m2High = m2temp;
		  if (m2temp.pt() < m2Low.pt()) m2Low = m2temp;
		  if ( deltaR <  m2_deltaR )  m2_dR = m2temp;
	  }
	}
	
	histograms["m2High"]->Fill(m2High.M(),weight);
	TString m2HighPlotName = "m2High_";
	m2HighPlotName += nJetsPlot;
	m2HighPlotName += "j";
	histograms[m2HighPlotName]->Fill(m2High.M(),weight);

	histograms["m2Low"]->Fill(m2Low.M(),weight);
	TString m2LowPlotName = "m2Low_";
	m2LowPlotName += nJetsPlot;
	m2LowPlotName += "j";
	histograms[m2LowPlotName]->Fill(m2Low.M(),weight);

	histograms["m2_dR"]->Fill(m2_dR.M(),weight);
	TString m2_dRPlotName = "m2_dR_";
	m2_dRPlotName += nJetsPlot;
	m2_dRPlotName += "j";
	histograms[m2_dRPlotName]->Fill(m2_dR.M(),weight);
      }      

      // M3Low
      if (nJetsPlot >= 3) {         
	//Find the combination with the Lowest vectorial sum pt
	reco::Particle::LorentzVector m3Low(-999,-999,-999,9999);
	for (int iJet = 0; iJet < (int)jets.size()-2; ++iJet) {
	  for (int jJet = iJet+1; jJet < (int)jets.size()-1; ++jJet) {
	    for (int kJet = jJet+1; kJet < (int)jets.size(); ++kJet) {
	      reco::Particle::LorentzVector m3temp = jets[iJet].p4() + jets[jJet].p4() + jets[kJet].p4();
	      if (m3temp.pt() < m3Low.pt()) m3Low = m3temp;
	    }
	  }
	}
	
	histograms["m3Low"]->Fill(m3Low.M(),weight);
	TString m3LowPlotName = "m3Low_";
	m3LowPlotName += nJetsPlot;
	m3LowPlotName += "j";
	histograms[m3LowPlotName]->Fill(m3Low.M(),weight);
      }
      //-----------------------------------------------------
      // Robin --- reconstructed W mass from diJet, no btag required, the combination of any 2 jets which
      //           has the closest invariant mass to W
      double MassW = 80.4 ;
      double mW2 = 0;
      double min_d_mW = 999;
      if (nJetsPlot >= 2) {
	for (int iJet = 0; iJet < (int)jets.size()-1; ++iJet) {
	  for (int jJet = iJet+1; jJet < (int)jets.size(); ++jJet) {
	    reco::Particle::LorentzVector mW2temp = jets[iJet].p4() + jets[jJet].p4();
	    double delta_mW = fabs(mW2temp.M() - MassW);
	    if ( delta_mW < min_d_mW ) { 
	      min_d_mW = delta_mW;
	      mW2 = mW2temp.M();
	    }
	  }
	}
	histograms["mW2"]->Fill(mW2,weight);
	TString mW2PlotName = "mW2_";
	mW2PlotName += nJetsPlot;
	mW2PlotName += "j";
	histograms[mW2PlotName]->Fill(mW2,weight);
      }

      // Robin --- reconstructed W mass from the leptonic leg decy
      double mW_lep = 0;
      reco::Particle::LorentzVector mW_leptemp = leptonLV + met.p4();
      mW_lep = mW_leptemp.M();
      
      histograms["mW_lep"]->Fill(mW_lep,weight);
      TString mW_lepPlotName = "mW_lep_";
      mW_lepPlotName += nJetsPlot;
      mW_lepPlotName += "j";
      histograms[mW_lepPlotName]->Fill(mW_lep,weight);

      // Robin --- reconstructed Top mass from TriJet, no btag required, the combination of any 3 jets which
      //           has the closest invariant mass to Top quark
      double MassTop = 172 ;
      double mTop_had = 0;
      double min_d_mTop = 999;
      if (nJetsPlot >= 3) {
	for (int iJet = 0; iJet < (int)jets.size()-2; ++iJet) {
	  for (int jJet = iJet+1; jJet < (int)jets.size()-1; ++jJet) {
	    for (int kJet = jJet+1; kJet < (int)jets.size(); ++kJet) {
	      reco::Particle::LorentzVector mTop_hadtemp = jets[iJet].p4() + jets[jJet].p4() + jets[kJet].p4();
	      double delta_mTop = fabs(mTop_hadtemp.M() - MassTop);
	      if ( delta_mTop < min_d_mTop ) { 
		min_d_mTop = delta_mTop;
		mTop_had = mTop_hadtemp.M();
	      }
	    }
	  }
	}
	histograms["mTop_had"]->Fill(mTop_had,weight);
	TString mTopPlotName = "mTop_had_";
	mTopPlotName += nJetsPlot;
	mTopPlotName += "j";
	histograms[mTopPlotName]->Fill(mTop_had,weight);
      }

      // Robin --- reconstructed Top mass from leptonic leg, no btag required
      double mTop_lep = 0;
      double min_d_mTop2 = 999;
      reco::Particle::LorentzVector mTop_leptemp = leptonLV + met.p4();
      if (nJetsPlot >= 1) {
	for (int iJet = 0; iJet < (int)jets.size(); ++iJet) {
	  mTop_leptemp += jets[iJet].p4();
	  double delta_mTop2 = fabs(mTop_leptemp.M() - MassTop);
	  if ( delta_mTop2 < min_d_mTop2 ) { 
	    min_d_mTop2 = delta_mTop2;
	    mTop_lep = mTop_leptemp.M();
	  }
	}
	
	histograms["mTop_lep"]->Fill(mTop_lep,weight);
	TString mTopPlotName = "mTop_lep_";
	mTopPlotName += nJetsPlot;
	mTopPlotName += "j";
	histograms[mTopPlotName]->Fill(mTop_lep,weight);
      }

      // Robin --- reconstructed W mass from two leading non-btagged jets
      //           require at least 4 jets with exactly 2 btag jets
      double TCHEM = 3.3 ;
      double SSVHEM = 1.74 ;
      reco::Particle::LorentzVector mW_hadtemp(0,0,0,0) ;
      int nNonBtag = 0 ;
      int nTag = 0;
      if (nJetsPlot >= 4) {
	for (int iJet = 0; iJet < 4; ++iJet) {
	  const pat::Jet *originJet = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
	  double discrTC = originJet->bDiscriminator("trackCountingHighEffBJetTags");
	  if (  fabs(discrTC) > TCHEM ) { nTag++;}
	  else{
	    ++nNonBtag;
	    if (nNonBtag < 3)  mW_hadtemp += jets[iJet].p4() ;
	  }
	}
	if ( nTag == 2 ) {  
	  histograms["mW_had"]->Fill(mW_hadtemp.M(),weight);
	  TString mW_hadPlotName = "mW_had_";
	  mW_hadPlotName += nJetsPlot;
	  mW_hadPlotName += "j";
	  histograms[mW_hadPlotName]->Fill(mW_hadtemp.M(),weight);
	}
	
      }
      
      /// Robin --- number of btagged jets
      int nBTagTC = 0;
      int nBTagSSV = 0;
      for (int iJet = 0; iJet < (int)jets.size(); ++iJet) {
	const pat::Jet *originJet = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
	double discrTC = originJet->bDiscriminator("trackCountingHighEffBJetTags");
	double discrSSV = originJet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
	if ( fabs(discrTC) > TCHEM )  nBTagTC++;
	if ( fabs(discrSSV) > SSVHEM )  nBTagSSV++;
      }
      histograms["nBTagTC"]->Fill(nBTagTC,weight);
      histograms["nBTagSSV"]->Fill(nBTagSSV,weight);

      //----------------------------------------------------------------------

      // Fill only ht plot for met < 20
      //
    
      if ( met.et() < 20 ) {
         if (nJetsPlot >= 3) {
            histograms["ht_ge3j_0met"]->Fill(ht,weight);
            histograms["htLep_ge3j_0met"]->Fill(htLep,weight);
            histograms["htAll_ge3j_0met"]->Fill(htAll,weight);
         }
         if (nJetsPlot >=4) {
            histograms["ht_ge4j_0met"]->Fill(ht,weight);
            histograms["htLep_ge4j_0met"]->Fill(htLep,weight);
            histograms["htAll_ge4j_0met"]->Fill(htAll,weight);
        
         }
      
      } else if (met.et() >= 20) {
       
         if (nJetsPlot >= 3) {
            histograms["ht_ge3j_1met"]->Fill(ht,weight);
            histograms["htLep_ge3j_1met"]->Fill(htLep,weight);
            histograms["htAll_ge3j_1met"]->Fill(htAll,weight);                
         }
         if (nJetsPlot >=4) {
            histograms["ht_ge4j_1met"]->Fill(ht);
            histograms["htLep_ge4j_1met"]->Fill(htLep,weight);
            histograms["htAll_ge4j_1met"]->Fill(htAll,weight);                        
         }
      
      }
    
   
      //Fill MET plots:
      // All jet bins

      if(muPlusJets_){

        histograms["met"]->Fill(met.et(),weight);
        if(nJetsPlot ==4){
          histograms["met_pt_4j"]->Fill(met.pt(),weight);
          histograms["met_phi_4j"]->Fill(met.phi(),weight);
        }
         if (charge > 0) histograms["metPlus"]->Fill(met.et(),weight);
         if (charge < 0) histograms["metMinus"]->Fill(met.et(),weight);
    
         histograms["metVsNJet"]->Fill(nJetsPlot,met.et());

         //Fill only the plot for the appropriate jet bin
         TString metPlotName = "met_";
         metPlotName += nJetsPlot;
         metPlotName += "j";
         histograms[metPlotName]->Fill(met.et(),weight);
      }
      
      if(ePlusJets_){
    
         if(electronOrig_->isEE()){
            histograms["met_EE"]->Fill(met.et(),weight);
            if (charge > 0) histograms["metPlus_EE"]->Fill(met.et(),weight);
            if (charge < 0) histograms["metMinus_EE"]->Fill(met.et(),weight);
       
            histograms["metVsNJet_EE"]->Fill(nJetsPlot,met.et());
       
            //Fill only the plot for the appropriate jet bin
            TString metPlotName = "met_";
            metPlotName += nJetsPlot;
            metPlotName += "j_EE";
            histograms[metPlotName]->Fill(met.et(),weight);
         }
         else if(electronOrig_->isEB()){
            histograms["met_EB"]->Fill(met.et(),weight);
            if (charge > 0) histograms["metPlus_EB"]->Fill(met.et(),weight);
            if (charge < 0) histograms["metMinus_EB"]->Fill(met.et(),weight);
       
            histograms["metVsNJet_EB"]->Fill(nJetsPlot,met.et());

            TString metPlotName = "met_";
            metPlotName += nJetsPlot;
            metPlotName += "j_EB";
            histograms[metPlotName]->Fill(met.et(),weight);
         }   
      }//ePlusJets_
       

      histograms["dPhiMetLep"]->Fill(fabs(reco::deltaPhi(met.phi(),leptonLV.phi())));
      TString dPhiMetLepPlotName = "dPhiMetLep_";
      dPhiMetLepPlotName += nJetsPlot;
      dPhiMetLepPlotName += "j";
      histograms[dPhiMetLepPlotName]->Fill(fabs(reco::deltaPhi(met.phi(),leptonLV.phi())),weight);
 
 
      const pat::MET * met_ = 0;
      met_ = dynamic_cast<const pat::MET *>(met.masterClonePtr().get());
 
      //Calculate and Fill W Transverse Mass 
      reco::Particle::LorentzVector metLV = met.p4();
      metLV.SetPz(0.); //Just to be safe--MET should never have Z component.  That's why it's called ME*T*!
      metLV.SetE(met.pt());
      reco::Particle::LorentzVector leptonTrans = leptonLV;
      leptonTrans.SetPz(0.);
      leptonTrans.SetE(leptonLV.pt());
 
      reco::Particle::LorentzVector WTrans = leptonTrans + metLV;

      //Robin W Pt 	 
      histograms["W_Pt"]->Fill(WTrans.pt(),weight);        //////////// 	 
      
      TString WPtPlotName = "W_Pt_"; 	 
      WPtPlotName += nJetsPlot; 	 
      WPtPlotName += "j"; 	 
      histograms[WPtPlotName]->Fill(WTrans.pt(),weight); 	 
      
      if(muPlusJets_){
         histograms["mT"]->Fill(WTrans.M(),weight);
         if (charge > 0) histograms["mT_Plus"]->Fill(WTrans.M(),weight);
         if (charge < 0) histograms["mT_Minus"]->Fill(WTrans.M(),weight);

         TString mTPlotName = "mT_";
         mTPlotName += nJetsPlot;
         mTPlotName += "j";
         histograms[mTPlotName]->Fill(WTrans.M(),weight);
      }
      
      if(ePlusJets_){
    
         if(electronOrig_->isEE()){
            histograms["mT_EE"]->Fill(WTrans.M(),weight);
            if (charge > 0) histograms["mT_Plus_EE"]->Fill(WTrans.M(),weight);
            if (charge < 0) histograms["mT_Minus_EE"]->Fill(WTrans.M(),weight);
       
            TString mTPlotName = "mT_";
            mTPlotName += nJetsPlot;
            mTPlotName += "j_EE";
            histograms[mTPlotName]->Fill(WTrans.M(),weight);
         }
         else if(electronOrig_->isEB()){
            histograms["mT_EB"]->Fill(WTrans.M(),weight);
            if (charge > 0) histograms["mT_Plus_EB"]->Fill(WTrans.M(),weight);
            if (charge < 0) histograms["mT_Minus_EB"]->Fill(WTrans.M(),weight);
       
            TString mTPlotName = "mT_";
            mTPlotName += nJetsPlot;
            mTPlotName += "j_EB";
            histograms[mTPlotName]->Fill(WTrans.M(),weight);
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
      
         histograms["GenWTransverseMass"]->Fill(gWTrans.M(),weight);

         double  met_pt= met.pt();
         double  met_phi= met.phi();
      
         double gmet_pt= genMet->pt();
         double gmet_phi= genMet->phi();

         double MCtruth_Met_pt =  (gmet_pt -met_pt)/(gmet_pt);
         double MCtruth_Met_phi = (gmet_phi -met_phi)/(gmet_phi);

         histograms["MC Truth MET Pt"]->Fill(MCtruth_Met_pt,weight);
         histograms["MC Truth MET Phi"]->Fill(MCtruth_Met_phi,weight);
      
      }
      
   } //end event loop
     
   if (!skipSel) {
     cout << "Printing" << endl;
     wPlusJets.print(std::cout);
   }

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


