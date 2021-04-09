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

#include "AngularVars.h"
#include "KinFit.h"
// #include "PhysicsTools/KinFitter/interface/TFitConstraintM.h"
// #include "PhysicsTools/KinFitter/interface/TFitConstraintEp.h"
// #include "PhysicsTools/KinFitter/interface/TFitParticleEtEtaPhi.h"
// #include "PhysicsTools/KinFitter/interface/TFitParticleCart.h"
// #include "PhysicsTools/KinFitter/interface/TKinFitter.h"

using namespace std;

// reco::Particle reCalculateMetWithShiftedJets
// ( edm::EventBase const & event, edm::ParameterSet const & setupParameters );


vector<double> generate_flat10_weights(TH1D* data_npu_estimated){
  // see SimGeneral/MixingModule/python/mix_E7TeV_FlatDist10_2011EarlyData_inTimeOnly_cfi.py copy and paste from there:         
  const double npu_probs[25] = {0.0698146584, 0.0698146584, 0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0630151648,0.0526654164,0.0402754482,0.0292988928,0.0194384503,0.0122016783,0.007207042,0.004003637,0.0020278322,0.0010739954,0.0004595759,0.0002229748,0.0001028162,4.58337152809607E-05};

  vector<double> result(25);
  double s = 0.0;
  for(int npu=0; npu<25; ++npu){
    double npu_estimated = data_npu_estimated->GetBinContent(data_npu_estimated->GetXaxis()->FindBin(npu));  
    result[npu] = npu_estimated / npu_probs[npu];
    s += npu_estimated;
  }
  // normalize weights such that the total sum of weights over thw whole sample is 1.0, i.e., sum_i  result[i] * npu_probs[i] should be 1.0 (!)                                                                                                                
  for(int npu=0; npu<25; ++npu){
    result[npu] /= s;
  }
  return result;
}



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

   // neu, 20/09/2011: use this as the assumed nInteractions dist in Su11 S4 MC samples                              
   //   when performing reweighting incorporating in-time and out-of-time PU effects                                 
   //    (no necessary for ttbar, lvjj analyses)                                                                     
   Double_t PoissonIntDist[25] = {
     0.104109,
     0.0703573,
     0.0698445,
     0.0698254,
     0.0697054,
     0.0697907,
     0.0696751,
     0.0694486,
     0.0680332,
     0.0651044,
     0.0598036,
     0.0527395,
     0.0439513,
     0.0352202,
     0.0266714,
     0.019411,
     0.0133974,
     0.00898536,
     0.0057516,
     0.00351493,
     0.00212087,
     0.00122891,
     0.00070592,
     0.000384744,
     0.000219377
   };

   // neu, 20/09/2011: use this as the assumed nInteractions dist in Su11 S4 MC samples                              
   //    when incorporating in-time PU effects only                                                                  
   //    (recommended for ttbar, lvjj analyses)                                                                      
   Double_t PoissonOneXDist[25] = {
     0.14551,
     0.0644453,
     0.0696412,
     0.0700311,
     0.0694257,
     0.0685655,
     0.0670929,
     0.0646049,
     0.0609383,
     0.0564597,
     0.0508014,
     0.0445226,
     0.0378796,
     0.0314746,
     0.0254139,
     0.0200091,
     0.0154191,
     0.0116242,
     0.00846857,
     0.00614328,
     0.00426355,
     0.00300632,
     0.00203485,
     0.00133045,
     0.000893794
   };

   // Distribution used for Fall2011 MC.

   Double_t Fall2011[50] = {
     0.003388501,
     0.010357558,
     0.024724258,
     0.042348605,
     0.058279812,
     0.068851751,
     0.072914824,
     0.071579609,
     0.066811668,
     0.060672356,
     0.054528356,
     0.04919354,
     0.044886042,
     0.041341896,
     0.0384679,
     0.035871463,
     0.03341952,
     0.030915649,
     0.028395374,
     0.025798107,
     0.023237445,
     0.020602754,
     0.0180688,
     0.015559693,
     0.013211063,
     0.010964293,
     0.008920993,
     0.007080504,
     0.005499239,
     0.004187022,
     0.003096474,
     0.002237361,
     0.001566428,
     0.001074149,
     0.000721755,
     0.000470838,
     0.00030268,
     0.000184665,
     0.000112883,
     6.74043E-05,
     3.82178E-05,
     2.22847E-05,
     1.20933E-05,
     6.96173E-06,
     3.4689E-06,
     1.96172E-06,
     8.49283E-07,
     5.02393E-07,
     2.15311E-07,
    9.56938E-08
   };

   double Fall2011_InTime[55] = {
     0.014583699,
     0.025682975,
     0.038460562,
     0.049414536,
     0.056931087,
     0.061182816,
     0.062534625,
     0.061476918,
     0.058677499,
     0.055449877,
     0.051549051,
     0.047621024,
     0.043923799,
     0.040569076,
     0.037414654,
     0.034227033,
     0.031437714,
     0.028825596,
     0.026218978,
     0.023727061,
     0.021365645,
     0.01918743,
     0.016972815,
     0.014920601,
     0.013038989,
     0.011293777,
     0.009612465,
     0.008193556,
     0.006888047,
     0.005715239,
     0.004711232,
     0.003869926,
     0.003154521,
     0.002547417,
     0.002024714,
     0.001574411,
     0.001245808,
     0.000955206,
     0.000735305,
     0.000557304,
     0.000412503,
     0.000305502,
     0.000231002,
     0.000165701,
     0.000121201,
     9.30006E-05,
     6.40004E-05,
     4.22003E-05,
     2.85002E-05,
     1.96001E-05,
     1.59001E-05,
     1.01001E-05,
     8.50006E-06,
     6.60004E-06,
     2.70002E-06
   };
   
   double Fall2011_Sarah[37] = {
     0.00896038,
     0.0190388,
     0.0313312,
     0.043897, 
     0.0518992,
     0.060569, 
     0.0609229, 
     0.0628996, 
     0.0623707, 
     0.0573285,
     0.0543062,
     0.0496953,
     0.0457721,
     0.0414015,
     0.0380816,
     0.0358757,
     0.0336014,
     0.0308003,
     0.0278926,
     0.0253227,
     0.0227559,
     0.020744, 
     0.0183461, 
     0.0163, 
     0.0142087, 
     0.0124412, 
     0.0105992, 
     0.00897445,
     0.00760707,
     0.00626885,
     0.00517192,
     0.0042419, 
     0.00352101,
     0.00283129,
     0.00221898,
     0.00180273,
     0.00542831
   };

   double Fall2011_Kevin[50] = {
     0.00861472,
     0.0191766,
     0.0315401,
     0.0434044,
     0.052766,
     0.059122,
     0.0620423,
     0.062198,
     0.0603166,
     0.0572191,
     0.0533996,
     0.0495471,
     0.0458081,
     0.0423156,
     0.0390716,
     0.0360298,
     0.0332947,
     0.0306011,
     0.0279503,
     0.0254247,
     0.0229424,
     0.0205835,
     0.0183146,
     0.0161212,
     0.0140661,
     0.0121489,
     0.0103963,
     0.00878846,
     0.00736302,
     0.00607868,
     0.00498397,
     0.00404399,
     0.00324527,
     0.00258741,
     0.00203751,
     0.00158541,
     0.0012321,
     0.000942719,
     0.000715644,
     0.000539022,
     0.000401583,
     0.000298674,
     0.000221364,
     0.000161243,
     0.000116438,
     8.44958e-05,
     6.15894e-05,
     4.39069e-05,
     3.0746e-05,
     2.11883e-05
   };
   
   std::vector< float > Wlumi ;
   std::vector< float > TrueDist2011;

   TFile *fpileup = new TFile("data_pileup_160404-180252_kevin.root");
   TH1D *hPileup = (TH1D*)fpileup->Get("pileup");

   vector<double> dataresult(50);
   for(int npu=0; npu<50; ++npu){
     dataresult[npu]= hPileup->GetBinContent(hPileup->GetXaxis()->FindBin(npu));
   }
   for( int i=0; i<50; ++i) {
     TrueDist2011.push_back(dataresult[i]);
     //Wlumi.push_back(Fall2011[i]);
     //Wlumi.push_back(Fall2011_InTime[i]);
     Wlumi.push_back(Fall2011_Kevin[i]);

   }

   edm::LumiReWeighting LumiWeights_ = edm::LumiReWeighting(Wlumi, TrueDist2011);


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
   //   float minNJetsFloat = (float)minNJets - 0.5;
   //   float maxNJetsFloat = (float)maxNJets + 0.5;

   cout << "Minimum number of jets = " << minNJets << endl;
   cout << "Maximum number of jets = " << maxNJets << endl;


   std::map<TString, double *> doubleBranches;
   std::map<TString, unsigned *> uintBranches;

   //double muPt, muEta, muPhi;
   //double jet1Pt, jet1Eta, jet1Phi;
   
   if (muPlusJets_) {
     doubleBranches["muPt"] = new double(0.0);
     doubleBranches["muEta"] = new double(0.0);
     doubleBranches["muCharge"] = new double(0.0);
     doubleBranches["muPhi"] = new double(0.0);
     doubleBranches["relIso"] = new double(0.0);

   }

   if (ePlusJets_) {
     doubleBranches["elePt_EE"] = new double(0.0);
     doubleBranches["eleEta_EE"] = new double(0.0);
     doubleBranches["eleCharge_EE"] = new double(0.0);
     doubleBranches["elePhi_EE"] = new double(0.0);
     doubleBranches["relIso_EE"] = new double(0.0);
     doubleBranches["hcalIso_EE"] = new double(0.0);
     doubleBranches["ecalIso_EE"] = new double(0.0);
     doubleBranches["trkIso_EE"] = new double(0.0);
     doubleBranches["sihih_EE"] = new double(0.0); 
     doubleBranches["dphi_EE"] = new double(0.0);
     doubleBranches["deta_EE"] = new double(0.0);
     doubleBranches["hoe_EE"] = new double(0.0);
   
     doubleBranches["elePt_EB"] = new double(0.0);
     doubleBranches["eleEta_EB"] = new double(0.0);
     doubleBranches["eleCharge_EB"] = new double(0.0);
     doubleBranches["elePhi_EB"] = new double(0.0);
     doubleBranches["relIso_EB"] = new double(0.0);
     doubleBranches["hcalIso_EB"] = new double(0.0);
     doubleBranches["ecalIso_EB"] = new double(0.0);
     doubleBranches["trkIso_EB"] = new double(0.0);
     doubleBranches["sihih_EB"] = new double(0.0);
     doubleBranches["dphi_EB"] = new double(0.0);
     doubleBranches["deta_EB"] = new double(0.0);
     doubleBranches["hoe_EB"] = new double(0.0);
   }

   doubleBranches["electronEtaAll"] = new double(0.0);
   doubleBranches["discrTCmax"] = new double(0.0);
   doubleBranches["discrTC2nd"] = new double(0.0);
   doubleBranches["nBTagsSSV"] = new double(0.0);
   doubleBranches["nBTagsTC"] = new double(0.0);

   doubleBranches["jet1discrSSV"] = new double(0.0);
   doubleBranches["jet2discrSSV"] = new double(0.0);
   doubleBranches["jet3discrSSV"] = new double(0.0);
   doubleBranches["jet4discrSSV"] = new double(0.0);
   doubleBranches["jet5discrSSV"] = new double(0.0);

   doubleBranches["jet1discrTC"] = new double(0.0);
   doubleBranches["jet2discrTC"] = new double(0.0);
   doubleBranches["jet3discrTC"] = new double(0.0);
   doubleBranches["jet4discrTC"] = new double(0.0);
   doubleBranches["jet5discrTC"] = new double(0.0);
 
   //   doubleBranches["jet
   doubleBranches["jet1Pt"] = new double(0.0);
   doubleBranches["jet2Pt"] = new double(0.0);
   doubleBranches["jet3Pt"] = new double(0.0);
   doubleBranches["jet4Pt"] = new double(0.0);
   doubleBranches["jet5Pt"] = new double(0.0);
   doubleBranches["jet1Eta"] = new double(0.0);
   doubleBranches["jet2Eta"] = new double(0.0);
   doubleBranches["jet3Eta"] = new double(0.0);
   doubleBranches["jet4Eta"] = new double(0.0);
   doubleBranches["jet5Eta"] = new double(0.0);
   doubleBranches["jet1Phi"] = new double(0.0);
   doubleBranches["jet2Phi"] = new double(0.0);
   doubleBranches["jet3Phi"] = new double(0.0);
   doubleBranches["jet4Phi"] = new double(0.0);
   doubleBranches["jet5Phi"] = new double(0.0);

   //   doubleBranches["jet1Mass"] = new double(0.0);
   //   doubleBranches["jet2Mass"] = new double(0.0);
   //   doubleBranches["jet3Mass"] = new double(0.0);
   //   doubleBranches["jet4Mass"] = new double(0.0);
   //   doubleBranches["jet5Mass"] = new double(0.0);



   doubleBranches["sumJetEt"] = new double(0.0);

   doubleBranches["hcalIso"] = new double(0.0);
   doubleBranches["ecalIso"] = new double(0.0);
   doubleBranches["trkIso"] = new double(0.0);
   doubleBranches["nJets"] = new double(0.0);
   doubleBranches["nLowJets"] = new double(0.0);
   doubleBranches["met"] = new double(0.0);
   doubleBranches["metPhi"] = new double (0.0);
   doubleBranches["mT"] = new double(0.0);
   doubleBranches["ht"] = new double(0.0);
   doubleBranches["htLep"] = new double(0.0);
   doubleBranches["htAll"] = new double(0.0);

   doubleBranches["minDPhiMetJet"] = new double(0.0);
   doubleBranches["dPhiMetJet"] = new double(0.0);
   if (muPlusJets_){
     doubleBranches["jet1dRMu"] = new double(0.0);
     doubleBranches["jet2dRMu"] = new double(0.0);
     doubleBranches["jet3dRMu"] = new double(0.0);
     doubleBranches["jet4dRMu"] = new double(0.0);
     doubleBranches["jet5dRMu"] = new double(0.0);
   }

   if (ePlusJets_){
     doubleBranches["jet1dREle"] = new double(0.0);
     doubleBranches["jet2dREle"] = new double(0.0);
     doubleBranches["jet3dREle"] = new double(0.0);
     doubleBranches["jet4dREle"] = new double(0.0);
     doubleBranches["jet5dREle"] = new double(0.0);
   }   
   uintBranches["run"] = new unsigned(0);
   uintBranches["lumi"] = new unsigned(0);
   uintBranches["event"] = new unsigned(0);

   //Robin new variables
   doubleBranches["weight"] = new double(0.0);
   doubleBranches["dPhiMetLep"] = new double(0.0);
   //
   doubleBranches["Mjj_an"] = new double(0.0);
   doubleBranches["WjjPt_an"] = new double(0.0);
   doubleBranches["dEtaJetJet_an"] = new double(0.0);
   doubleBranches["dPhiJetJet_an"] = new double(0.0);

   doubleBranches["Ptlnujj_an"] = new double(0.0);
   doubleBranches["MWWTop2jets_an"] = new double(0.0);
   doubleBranches["minDPhiLepJet_an"] = new double(0.0);
   
   doubleBranches["JacobePeak_an"] = new double(0.0);

   doubleBranches["Pt1st"] = new double(0.0);
   doubleBranches["Pt2nd"] = new double(0.0);
   doubleBranches["Pt3rd"] = new double(0.0);

   doubleBranches["dRlepjj"] = new double(0.0);
   /////
   doubleBranches["Mjj"] = new double(0.0);
   doubleBranches["WjjPt"] = new double(0.0);
   doubleBranches["dEtaJetJet"] = new double(0.0);
   doubleBranches["dPhiJetJet"] = new double(0.0);
   doubleBranches["MWWTop2jets"] = new double(0.0);
   doubleBranches["minDPhiLepJet"] = new double(0.0);
   doubleBranches["Ptlnujj"] = new double(0.0);

   doubleBranches["JacobePeak"] = new double(0.0);
   ////
//    doubleBranches["Mjj_mW"] = new double(0.0);
//    doubleBranches["WjjPt_mW"] = new double(0.0);
//    doubleBranches["dEtaJetJet_mW"] = new double(0.0);
//    doubleBranches["dPhiJetJet_mW"] = new double(0.0);

//    doubleBranches["Ptlnujj_mW"] = new double(0.0);
//    doubleBranches["MWWTop2jets_mW"] = new double(0.0);
//    doubleBranches["minDPhiLepJet_mW"] = new double(0.0);
   
//    doubleBranches["JacobePeak_mW"] = new double(0.0);
   
//    doubleBranches["jetCombo_mW"] = new double(0.0);

   ///

   doubleBranches["Cos_dPhiWW"] = new double(0.0);
   doubleBranches["CosTheta_l"] = new double(0.0);
   doubleBranches["CosTheta_j"] = new double(0.0);
   doubleBranches["Cos_dPhiWH"] = new double(0.0);
   doubleBranches["CosTheta_WH"] = new double(0.0);
   doubleBranches["jacksonAngle"] = new double(0.0);

   doubleBranches["chi2KF"] = new double(0.0);
   doubleBranches["chi2"] = new double(0.0);
   doubleBranches["mWWKF"] = new double(0.0);
   doubleBranches["mJJKF"] = new double(0.0);
   doubleBranches["fitStatus"] = new double(0.0);
   doubleBranches["nPV"] = new double(0.0);
   doubleBranches["nIntxn"] = new double(0.0);

   //---neutrino Pz                                                                                                        
   doubleBranches["real"] = new double(0.0);

   //---------   
   TTree * anaTree = fs.make<TTree> ("anaTree", "anaTree");


   for (std::map<TString, double*>::iterator iDMap = doubleBranches.begin();
        iDMap != doubleBranches.end();
        iDMap ++) {
     
     std::cout << "Creating branch   " << (iDMap->first).Data() << endl;
     TString ttString = iDMap->first;
     ttString += "/D";
     anaTree->Branch(iDMap->first, iDMap->second, ttString);       
   }
     
   for (std::map<TString, unsigned*>::iterator iIMap = uintBranches.begin();
        iIMap != uintBranches.end();
        iIMap ++) {
     
     std::cout << "Creating branch   " << (iIMap->first).Data() << endl;
     TString ttString = iIMap->first;
     ttString += "/i";
     anaTree->Branch(iIMap->first, iIMap->second, ttString);       
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
   cout << "Cuts defined:" << endl;
   wPlusJets.printActiveCuts(cout);
     
     
   //Some other variables used in the loop
   int nInclusive = 0;
   int nPassed = 0;
   //   int nDumped = 0;
   int nTotal  = 0;
   //   int maxToDump = shyftParameters.getParameter<int>("maxDump");
   int maxInclusive = shyftParameters.getParameter<int>("maxInclusive");
   int maxPassed = shyftParameters.getParameter<int>("maxPassed");
   int printEvery = shyftParameters.getParameter<int>("printEvery");
     
   TFile *f = new TFile("pudist.root");
   TH1D *hname = (TH1D*)f->Get("pileup");

   //loop through each event
   cout << "Beginning event loop." << endl;
   for( ev.toBegin();
        ! ev.atEnd();
        ++ev, ++nTotal) {

     for (std::map<TString, double*>::iterator iDMap = doubleBranches.begin();
          iDMap != doubleBranches.end();
          iDMap ++) {

       (* (iDMap->second) ) = -9.99e3;
     }
     
     for (std::map<TString, unsigned*>::iterator iIMap = uintBranches.begin();
          iIMap != uintBranches.end();
          iIMap ++) {

       (* (iIMap->second)) = 9999; // not a lot of good choices here
      
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

     //Check whether the event passes
     ret.set(false);
     bool passed = wPlusJets(ev, ret);

     if (ret[std::string("Inclusive")]) ++nInclusive;

     //Since we only care about selected events, why make this more
     //complicated than it has to be?
     if (!passed) continue; // I want events to pass trigger only for data.
     ++nPassed;
    
 
     std::vector<reco::ShallowClonePtrCandidate> const & electrons = wPlusJets.selectedElectrons();
     std::vector<reco::ShallowClonePtrCandidate> const & muons     = wPlusJets.selectedMuons();
     std::vector<reco::ShallowClonePtrCandidate> const & jets      = wPlusJets.cleanedJets();
     //This list may still have electrons (and muons?) double-counted as jets
     //     std::vector<reco::ShallowClonePtrCandidate> const & jetsBeforeClean = wPlusJets.selectedJets();
     reco::ShallowClonePtrCandidate   const & met             = wPlusJets.selectedMET();
    
     //     reco::Particle corrMet =  reCalculateMetWithShiftedJets (ev, shyftParameters);
      

     //Some quantities needed for plots:
     int charge = 0;  //Filled in from electron or muon charge
     int nJetsPlot = (int) jets.size();
     if (nJetsPlot > maxNJets) nJetsPlot = maxNJets;

     //Common lepton kinematics
     reco::Particle::LorentzVector leptonLV;
     const reco::GenParticle *genLepton = 0;
      
     //more PU info                                                                                                            
     int numInteractions  = -1;
     double event_weight = 1;

     if(!ev.isRealData()){
       edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
       ((edm::EventBase*)&ev)->getByLabel(edm::InputTag("addPileupInfo"),PupInfo);
       std::vector<PileupSummaryInfo>::const_iterator PUi;

       //                                                                                                           
       // neu, 20/9/2011: do the following for incorporating in-time PU contributions                               
       //   this is the one to use for ttbar, lvjj analyses                                                         
       //                                                                                                           
       int npv = -1;
       for(PUi = PupInfo->begin(); PUi != PupInfo->end(); ++PUi) {

	 int BX = PUi->getBunchCrossing();

	 if(BX == 0) {
	   npv = PUi->getPU_NumInteractions();
	   continue;
	 }

       }
       event_weight = LumiWeights_.weight( npv );
       numInteractions = npv;

       //                                                                                                           
       // neu, 20/9/2011: do the following for incorporating in-time+out-of-time PU contributions                   
       //   not necessary for ttbar, lvjj analyses                                                                  
       //                                                                                                           
       //float sum_nvtx = 0;                                                                                        
       //for(PUi = PupInfo->begin(); PUi != PupInfo->end(); ++PUi){                                                 
       //  numInteractions = PUi->getPU_NumInteractions();                                                          
       //  sum_nvtx += float(numInteractions);                                                                      
       //}                                                                                                          
       //float ave_nvtx = sum_nvtx/3.;                                                                              
       //event_weight = LumiWeights_.weight3BX(ave_nvtx);                                                           
     }

     double weight = event_weight;


     if(!ev.isRealData()){
       (*doubleBranches["nIntxn"]) = numInteractions;
     }

     //Muon-only plots
     const pat::Muon * muon_ = 0;
     if ( muPlusJets_ ) {
       if (muons.size() > 0){

         //cout << "=============== NEW EVENT shyft_pretag Muon unpacking ======================" << endl;

         //cout <<"Trying to get the muon from the selectedMuons" << endl;
           
         muon_ = dynamic_cast<const pat::Muon *>(muons[0].masterClonePtr().get());

	 //         const pat::MET * met_ = 0;
	 //         met_ = dynamic_cast<const pat::MET *>(met.masterClonePtr().get());
 

         if (muon_ == 0 ) {

           cout << "Oops, we don't have a valid muon pointer in this event, skipping it" << endl;
           continue;

         } else {
           //cout << "We have the muon pointer and it is fine." << endl;
         }
           
         leptonLV = muon_->p4();
         genLepton = muon_->genLepton();         
         charge = muon_->charge();
                    
            
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
            

         (*doubleBranches["muPt"]) = muon_->pt();         
         (*doubleBranches["muEta"])   = muon_->eta();
	 (*doubleBranches["muCharge"])   = muon_->charge();
	 (*doubleBranches["muPhi"])   = muon_->phi();
	 
          
         //////////////Muon Values////////
         double hcalIso  = muon_->hcalIso();
         double ecalIso  = muon_->ecalIso();
         double trkIso   = muon_->trackIso();
         double iMuonPt  = muon_->pt();
         double relIso = (iMuonPt != 0 ) ? (ecalIso + hcalIso + trkIso) / iMuonPt : 0;

	 (*doubleBranches["relIso"])  = relIso ;
	 (*doubleBranches["hcalIso"]) = hcalIso ;
	 (*doubleBranches["ecalIso"]) = ecalIso ;
	 (*doubleBranches["trkIso"])  = trkIso ; 
         
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
          
         //         int numberOfMuonHits = globalTrackHits.numberOfValidMuonHits();
         //         int numberOfStripHits = globalTrackHits.numberOfValidTrackerHits();
         //         int numberOfPixelLayersWithMeasurement = globalTrackHits.pixelLayersWithMeasurement();
         //         int missingTrackerHits = globalTrackHits.numberOfLostStripHits();
          
          
          
	 //         if (genLepton) {
           //double  mu_pt= muon_->pt();
           // double  mu_eta= muon_->eta();
//            double  mu_phi= muon_->phi();
	  
//            double gmu_pt= genLepton->pt();
//            double gmu_eta= genLepton->eta();
//            double gmu_phi= genLepton->phi(); 
             
//            double MCtruth_Mu_pt =  (gmu_pt -mu_pt)/(gmu_eta);
//            double MCtruth_Mu_eta = (gmu_eta -mu_eta)/(gmu_eta);
//            double MCtruth_Mu_phi = (gmu_phi -mu_phi)/(gmu_phi);
             
           //"MC Truth Pt"    histograms["MC Truth Mu Eta"]
	   //         }
         
       }
       
     }//muPlusJets_

     const pat::Electron * electron_ = 0;

     if ( ePlusJets_ ) {
       if (electrons.size() > 0){
	 
	 electron_ = dynamic_cast<const pat::Electron *>(electrons[0].masterClonePtr().get());
	 
	 leptonLV = electrons[0].p4();
	 /*
	 if (electron_->isEE() ==1){
	 cout << "Electron isEE val: " << electron_->isEE()
              << "Phi is : "<< electron_->phi()
              << "Eta is : "<< electron_->eta()<<endl;
	 }
	 */
	 (*doubleBranches["electronEtaAll"]) = electron_->eta();
	 if(electron_->isEE() == 1 ){


	   (*doubleBranches["elePt_EE"]) = electron_->pt() ;         
	   (*doubleBranches["eleEta_EE"])   = electron_->eta() ;
	   (*doubleBranches["eleCharge_EE"])   = electron_->charge();
	   (*doubleBranches["elePhi_EE"])   = electron_->phi() ;
	   
	   //////////////Electron Values////////
	   double hcalIso  = electron_->dr03HcalTowerSumEt() ;
	   double ecalIso  = electron_->dr03EcalRecHitSumEt() ;
	   double trkIso   = electron_->dr03TkSumPt() ;
	   double iElePt  = electron_->pt();
	   double relIso = (iElePt != 0 ) ? (ecalIso + hcalIso + trkIso) / iElePt : 0;
	   double sihih   = electron_->sigmaIetaIeta();
	   double Dphi    = electron_->deltaPhiSuperClusterTrackAtVtx();
	   double Deta    = electron_->deltaEtaSuperClusterTrackAtVtx();
	   double HoE     = electron_->hadronicOverEm();



	   (*doubleBranches["relIso_EE"])  = relIso ;
 	   (*doubleBranches["hcalIso_EE"]) = hcalIso ;
 	   (*doubleBranches["ecalIso_EE"]) = ecalIso ;
 	   (*doubleBranches["trkIso_EE"])  = trkIso ; 
	   (*doubleBranches["sihih_EE"])  = sihih ; 
	   (*doubleBranches["hoe_EE"])  = HoE ;
	   (*doubleBranches["dphi_EE"])  = Dphi ;
	   (*doubleBranches["deta_EE"])  = Deta;
	 }
	 else if(electron_->isEB() ==1){

	   (*doubleBranches["elePt_EB"]) = electron_->pt() ;         
	   (*doubleBranches["eleEta_EB"])   = electron_->eta() ;
	   (*doubleBranches["eleCharge_EB"])   = electron_->charge();
	   (*doubleBranches["elePhi_EB"])   = electron_->phi() ;
	   
	   //////////////Electron Values////////
	   double hcalIso  = electron_->dr03HcalTowerSumEt() ;
	   double ecalIso  = (0 > (electron_->dr03EcalRecHitSumEt()-1)) ? 0 : (electron_->dr03EcalRecHitSumEt()-1) ;
	   double trkIso   = electron_->dr03TkSumPt() ;
	   double iElePt  = electron_->pt();
	   double relIso = (iElePt != 0 ) ? (ecalIso + hcalIso + trkIso) / iElePt : 0;
	   double sihih   = electron_->sigmaIetaIeta();
	   double Dphi    = electron_->deltaPhiSuperClusterTrackAtVtx();
	   double Deta    = electron_->deltaEtaSuperClusterTrackAtVtx();
	   double HoE     = electron_->hadronicOverEm();


	   (*doubleBranches["relIso_EB"])  = relIso ;
 	   (*doubleBranches["hcalIso_EB"]) = hcalIso ;
 	   (*doubleBranches["ecalIso_EB"]) = ecalIso ;
 	   (*doubleBranches["trkIso_EB"])  = trkIso ; 
	   (*doubleBranches["sihih_EB"])  = sihih ;
           (*doubleBranches["hoe_EB"])  = HoE ;
           (*doubleBranches["dphi_EB"])  = Dphi ;
           (*doubleBranches["deta_EB"])  = Deta;

	 }
	 
       }
     }//ePlusJets_
     
     //Calculate and Fill W Transverse Mass 
     reco::Particle::LorentzVector metLV = met.p4();
     metLV.SetPz(0.); //Just to be safe--MET should never have Z component.  That's why it's called ME*T*!
     metLV.SetE(met.pt());
     reco::Particle::LorentzVector leptonTrans = leptonLV;
     leptonTrans.SetPz(0.);
     leptonTrans.SetE(leptonLV.pt());
     
     reco::Particle::LorentzVector WTrans = leptonTrans + metLV;
     
     //Calculate Ht using all available jets
     double ht = 0.0;
     if (muPlusJets_ && muons.size() > 0) ht += muons[0].pt();
     if (ePlusJets_ && electrons.size() > 0) ht += electrons[0].pt();
     //	 ht += muon_->pt();
     
     
     double htLep = ht + met.et(); //htLep = only lepPt and MET

     for (unsigned int iJet = 0; iJet < jets.size(); ++iJet) {
       const pat::Jet *jet_ = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
       ht += jet_->et();
     }
     
     double htAll = ht + met.et(); //htAll = everything: lepPt + MET + sum(jetEt)
     
        
  //Fill individual jet plots
     double sumJetEt = 0.0;
     
     double minDPhiMetJet = 9e20;
     double dPhiMetJet = 999;
     for (unsigned iJet = 0; (int)iJet < nJetsPlot; ++iJet) {
       
       const pat::Jet *jet_ = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
       
       TString histName = "jet"; histName += (iJet+1);
       
       (*doubleBranches[histName + "Pt"]) = jet_->pt() ;
       (*doubleBranches[histName + "Eta"]) = jet_->eta() ;
       (*doubleBranches[histName + "Phi"]) = jet_->phi() ;
       //  (*doubleBranches[histName + "Mass"]) = jet_->mass();
       sumJetEt += jet_->pt();
       //       cout << "got to new vars"<<endl;       
       if (muPlusJets_){
	 double tempDr = reco::deltaR(muon_->eta(),muon_->phi(),jet_->eta(), jet_->phi());
         (*doubleBranches[histName + "dRMu"]) = tempDr;
       }

       if (ePlusJets_){
         double tempDrEle = reco::deltaR(electron_->eta(),electron_->phi(),jet_->eta(), jet_->phi());
	 (*doubleBranches[histName + "dREle"]) = tempDrEle;
       }

       double tempDPhi = reco::deltaPhi(met.phi(), jet_->phi());
       //       cout<<"got past new vars"<<endl;
       if ( fabs(tempDPhi) < minDPhiMetJet ) minDPhiMetJet = tempDPhi;
       if((iJet+1) == 1 ){dPhiMetJet = reco::deltaPhi(met.phi(), jet_->phi());}
     }

     //Robin
     double dEtajj_f = 0 ;
     double dPhijj_f = 0 ;
     double dPhilj_f = 0 ;
     double m2jj_f = 0 ;
     double m2jj_pt_f = 0;
     double mWWjj_f = 0 ;
     double Ptlnujj_f = 0;
     double Jacobe_f = 0 ;

     if (nJetsPlot >= 2) {
       dEtajj_f = fabs( jets[0].eta() - jets[1].eta() );
       dPhijj_f = fabs(reco::deltaPhi(jets[0].phi(),jets[1].phi())) ;
       if ( fabs(reco::deltaPhi(leptonLV.phi(), jets[0].phi())) < fabs(reco::deltaPhi(leptonLV.phi(), jets[1].phi())) ) {
	 dPhilj_f = fabs(reco::deltaPhi(leptonLV.phi(), jets[0].phi())) ;
       }
       else {  dPhilj_f = fabs(reco::deltaPhi(leptonLV.phi(), jets[1].phi())) ;}
       
       reco::Particle::LorentzVector m2jjTemp = jets[0].p4() + jets[1].p4() ;
       m2jj_f = m2jjTemp.M() ;
       m2jj_pt_f = m2jjTemp.pt();
       reco::Particle::LorentzVector MWWjjTemp = leptonLV + met.p4() + m2jjTemp ;
       mWWjj_f = MWWjjTemp.M() ;
       Ptlnujj_f = MWWjjTemp.pt();
       Jacobe_f = jets[1].pt() / m2jj_f ;
       
       //fill branches
       (*doubleBranches["Mjj"]) =    m2jj_f ;
       (*doubleBranches["WjjPt"]) = m2jj_pt_f;
       (*doubleBranches["dEtaJetJet"]) =  dEtajj_f ;
       (*doubleBranches["dPhiJetJet"]) =  dPhijj_f ;
       (*doubleBranches["MWWTop2jets"]) = mWWjj_f ;
       (*doubleBranches["minDPhiLepJet"]) = dPhilj_f ;
       (*doubleBranches["dPhiMetJet"]) = dPhiMetJet;
       (*doubleBranches["Ptlnujj"]) = Ptlnujj_f;
       (*doubleBranches["JacobePeak"]) = Jacobe_f;
     }
     
  
     /////
     // John
//      double mW = 80.4;
//      double resMW = 999.9;
//      double dEtajj_mW = 0 ;
//      double dPhijj_mW = 0 ;
//      double dPhilj_mW = 0 ;
//      double mjj_mW = 0 ;
//      double mjj_pt_mW = 0;
//      double mWWjj_mW = 0 ;
//      double Ptlnujj_mW = 0;
//      double Jacobe_mW = 0 ;
//      double bestJetCombo = 0;
    
//      int iJetBest = 100;
//      int jJetBest = 100;

//      for(int iJets=0; iJets<nJetsPlot-1; iJets++){
//        for(int jJets=iJets+1; jJets<nJetsPlot; jJets++){
  
// 	 reco::Particle::LorentzVector mJJ_temp = jets[iJets].p4() + jets[jJets].p4();
//          double mW_temp = mJJ_temp.M();
//          double res_MW_temp = fabs(mW-mW_temp);
//          if(res_MW_temp<resMW){
//            iJetBest = iJets;
//            jJetBest = jJets;
//            resMW = res_MW_temp;
// 	 }
//        } // end jJets loop
//      } // end iJets loop

//      // 4Vect of diJet combo with lowest residual to W mass
//      if(iJetBest!=100 && jJetBest!=100){
//        reco::Particle::LorentzVector mJJ_mW = jets[iJetBest].p4() + jets[jJetBest].p4();

//        dEtajj_mW = fabs( jets[iJetBest].eta() - jets[jJetBest].eta() );
//        dPhijj_mW = fabs(reco::deltaPhi(jets[iJetBest].phi(), jets[jJetBest].phi()));
//        if ( fabs(reco::deltaPhi(leptonLV.phi(), jets[iJetBest].phi())) < fabs(reco::deltaPhi(leptonLV.phi(), jets[jJetBest].phi())) ) {
//          dPhilj_mW = fabs(reco::deltaPhi(leptonLV.phi(), jets[iJetBest].phi())) ;
//        }
//        else {  
//          dPhilj_mW = fabs(reco::deltaPhi(leptonLV.phi(), jets[jJetBest].phi())) ;}
       
//        mjj_mW = mJJ_mW.M() ;
//        mjj_pt_mW = mJJ_mW.pt();
     
//        reco::Particle::LorentzVector MWWjjTemp_mW = leptonLV + met.p4() + mJJ_mW ;
    
//        mWWjj_mW = MWWjjTemp_mW.M() ;
//        Ptlnujj_mW = MWWjjTemp_mW.pt();
//        Jacobe_mW = jets[jJetBest].pt() / mjj_mW ;

//        // Jet combo id
//        if( iJetBest==0 && jJetBest==1 ) bestJetCombo = 12;
//        if( iJetBest==0 && jJetBest==2 ) bestJetCombo = 13;
//        if( iJetBest==0 && jJetBest==3 ) bestJetCombo = 14;
//        if( iJetBest==0 && jJetBest==4 ) bestJetCombo = 15;
//        if( iJetBest==1 && jJetBest==2 ) bestJetCombo = 23;
//        if( iJetBest==1 && jJetBest==3 ) bestJetCombo = 24;  
//        if( iJetBest==1 && jJetBest==4 ) bestJetCombo = 25;
//        if( iJetBest==2 && jJetBest==3 ) bestJetCombo = 34;
//        if( iJetBest==2 && jJetBest==4 ) bestJetCombo = 35;
//        if( iJetBest==3 && jJetBest==4 ) bestJetCombo = 45;

//        //fill branches
//        (*doubleBranches["Mjj_mW"]) =    mjj_mW ;
//        (*doubleBranches["WjjPt_mW"]) = mjj_pt_mW;
//        (*doubleBranches["dEtaJetJet_mW"]) =  dEtajj_mW ;
//        (*doubleBranches["dPhiJetJet_mW"]) =  dPhijj_mW ;
//        (*doubleBranches["MWWTop2jets_mW"]) = mWWjj_mW ;
//        (*doubleBranches["minDPhiLepJet_mW"]) = dPhilj_mW ;
//        (*doubleBranches["Ptlnujj_mW"]) = Ptlnujj_mW;
//        (*doubleBranches["JacobePeak_mW"]) = Jacobe_mW;
//        (*doubleBranches["jetCombo_mW"]) = bestJetCombo;
//      } // control in case i or j JetBest not chosen
     
  




     /////
     //Robin
     double minDeta = 999 ;
     double dEtajj = 0 ;
     double dPhijj = 0 ;
     double dPhilj = 0 ;
     double m2jj = 0 ;
     double m2jj_pt = 0;
     double mWWjj = 0 ;
     double Pt1st = 0 ;     
     double Pt2nd = 0 ;     
     double Pt3rd = 0 ;     
     double Ptlnujj = 0;
     double  dRljj= 999;
     double Jacobe = 0 ;
     if (nJetsPlot >= 2) {
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
	     m2jj_pt = m2jj_an.pt();
	     reco::Particle::LorentzVector MWWjj_an = leptonLV + met.p4() + m2jj_an ;
	     mWWjj = MWWjj_an.M() ;
	     Ptlnujj = MWWjj_an.pt();
             dRljj = reco::deltaR(m2jj_an.eta(),m2jj_an.phi(),leptonLV.eta(),leptonLV.phi());
	     
	     Jacobe = jets[jJet].pt() / m2jj ;
	     //pt(lep, jet, jet) ordered
	     if (leptonLV.pt() > jets[iJet].pt()) {
	       Pt1st = leptonLV.pt() ;
	       Pt2nd = jets[iJet].pt() ;
	       Pt3rd = jets[jJet].pt() ;
	     }
	     else if (leptonLV.pt() > jets[jJet].pt() ){
	       Pt2nd = leptonLV.pt() ;
	       Pt1st = jets[iJet].pt() ;
	       Pt3rd = jets[jJet].pt() ;
	     }else {
	       Pt3rd = leptonLV.pt() ;
	       Pt1st = jets[iJet].pt() ;
	       Pt2nd = jets[jJet].pt() ;
	     }
	   }
	 }
       }
     }
     
     double dPhiMetLep = fabs(reco::deltaPhi(met.phi(),leptonLV.phi())) ;
     
     //----btag
     // Loop over all Jets in Event 
     double discrTCmax = -999 ;
     double discrTC2nd = -999 ;
     int maxJet = 0 ;

     for (int iJet = 0; iJet < (int)jets.size(); ++iJet) {
       const pat::Jet *originJet = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
       double discrTC = originJet->bDiscriminator("trackCountingHighEffBJetTags");
       //       cout << "==> jet " << iJet <<" discrTC is " << discrTC << endl;
       if(discrTC > discrTCmax) { discrTCmax = discrTC ; maxJet = iJet ;}
     }

     for (int iJet = 0; iJet < (int)jets.size(); ++iJet) {
       const pat::Jet *originJet = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
       double discrTC = originJet->bDiscriminator("trackCountingHighEffBJetTags");
       if(discrTC > discrTC2nd && iJet != maxJet ) discrTC2nd = discrTC ;
     }
     //Fill a tree with all the jet PTs

     int nbtagsSSV = 0;  
     int nbtagsTC = 0;  
     for (int iJet = 0; iJet < nJetsPlot; ++iJet) {
       const pat::Jet *originJet = dynamic_cast<const pat::Jet *>(jets[iJet].masterClonePtr().get());
       double discrSSV = originJet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
       double discrTC = originJet->bDiscriminator("trackCountingHighEffBJetTags");
       //       cout << "==> jet " << iJet <<" discrTC is " << discrTC << endl;
       if (discrSSV > 1.74) nbtagsSSV++;
       if (discrTC > 3.3) nbtagsTC++;
       TString histName1 = "jet";
       histName1 += (iJet+1);
       (*doubleBranches[histName1 + "discrSSV"]) = discrSSV; 
       (*doubleBranches[histName1 + "discrTC"]) = discrTC; 
     }
     //Fill a tree with the # of Jets with 20<Pt<30
     int nLowJets = 0;
     for (int iJet =0; iJet < nJetsPlot; ++iJet){
       if ( jets[iJet].pt() >= 20 && jets[iJet].pt() <30) nLowJets++;
     }

     //Number of primary vertices
     bool skipSel = shyftParameters.getParameter<bool>("skipSelection");
     edm::Handle<reco::VertexCollection> vertices;
     std::string vertexColl = skipSel ? "goodOfflinePrimaryVertices" : "offlinePrimaryVertices";
     ((edm::EventBase*)&ev)->getByLabel(edm::InputTag(vertexColl), vertices);

     reco::VertexCollection vertexCollection = *(vertices.product());
     int vertex_number_valid =0;
     
     for (reco::VertexCollection::const_iterator itr = vertexCollection.begin(); itr != vertexCollection.end(); ++itr){
       if ( !(itr->isFake())&& itr->ndof() > 4 && abs(itr->z()) <= 15 && itr->position().Rho() <= 2 ) vertex_number_valid++;
       
      }

      (*doubleBranches["nPV"]) = vertex_number_valid;
     
     (*doubleBranches["minDPhiMetJet"]) = minDPhiMetJet ;
     (*doubleBranches["sumJetEt"])   = sumJetEt ;
     
     (*doubleBranches["nJets"])   = nJetsPlot;
     (*doubleBranches["nLowJets"]) = nLowJets;
     (*doubleBranches["met"])     = met.et() ;
     //     (*doubleBranches["metPhi"])     = met.phi() ;
     (*doubleBranches["mT"])      = WTrans.M() ;
     (*doubleBranches["ht"])      = ht ;
     (*doubleBranches["htLep"])   = htLep ;
     (*doubleBranches["htAll"])   = htAll ;
     //     if (muPlusJets_)     (*doubleBranches["minDRMuJet"]) = minDRMuJet ;
     //     if (ePlusJets_)     (*doubleBranches["minDREleJet"]) = minDREleJet ;
     

     (*uintBranches["run"])       = ev.id().run();
     (*uintBranches["lumi"])      = ev.id().luminosityBlock();
     (*uintBranches["event"])     = ev.id().event();

     //Robin
     (*doubleBranches["dPhiMetLep"]) =     dPhiMetLep ;
     (*doubleBranches["Mjj_an"]) =          m2jj ;
     (*doubleBranches["WjjPt_an"]) = m2jj_pt;
     (*doubleBranches["dEtaJetJet_an"]) =  dEtajj ;
     (*doubleBranches["dPhiJetJet_an"]) =  dPhijj ;
     (*doubleBranches["MWWTop2jets_an"]) = mWWjj ;
     (*doubleBranches["minDPhiLepJet_an"]) = dPhilj ;
     (*doubleBranches["Ptlnujj_an"]) = Ptlnujj;
     (*doubleBranches["JacobePeak_an"]) = Jacobe;

     (*doubleBranches["dRlepjj"]) = dRljj;
     (*doubleBranches["weight"]) = weight ;

     (*doubleBranches["Pt1st"]) = Pt1st ;
     (*doubleBranches["Pt2nd"]) = Pt2nd ;
     (*doubleBranches["Pt3rd"]) = Pt3rd ;

     (*doubleBranches["discrTCmax"]) = discrTCmax ;
     (*doubleBranches["discrTC2nd"]) = discrTC2nd ;
     (*doubleBranches["nBTagsSSV"]) = nbtagsSSV;
     (*doubleBranches["nBTagsTC"]) = nbtagsTC;

     //Add in Angular Variables
     if (nJetsPlot >= 2) {
       reco::Particle::LorentzVector METLV = met.p4();
       reco::Particle::LorentzVector jet1 = jets[0].p4();
       reco::Particle::LorentzVector jet2 = jets[1].p4();
       
       float cosdPhiWW=0.; float costhetal=0.; float costhetaj=0.;
       float cosdPhiWH=0.; float costhetaWH=0.; float jacksonAngle = 0.;
     
       //------solve neutrino Pz                                                                                           
       double real = 1.0 ;
       double massW = 80.4 ;
       double massMuon = 0.1;
       double nvPz1 = 0;
       double nvPz2 = 0;
       double coeffA = (massW*massW - massMuon*massMuon)/2 + leptonLV.Px()*METLV.Px() + leptonLV.Py()*METLV.Py();
       double coeffa= leptonLV.E()*leptonLV.E() - leptonLV.Pz()*leptonLV.Pz();
       double coeffb = (-2)*coeffA*leptonLV.Pz() ;
       double coeffc = leptonLV.E()*leptonLV.E()*(METLV.Px()*METLV.Px()+METLV.Py()*METLV.Py()) - coeffA*coeffA;
       double rootDelta = coeffb*coeffb - 4*coeffa*coeffc ;

       // imaginary roots                                                                                                  
       if (rootDelta<0){
         real = 0;
         nvPz1 = (-1)*coeffb/(2*coeffa);
         nvPz2 = (-1)*coeffb/(2*coeffa);
       }
       //real roots                                                                                                        
       else {
         double root1 = (-1)*coeffb/(2*coeffa) + sqrt(rootDelta)/(2*coeffa);
         double root2 = (-1)*coeffb/(2*coeffa) - sqrt(rootDelta)/(2*coeffa);
         if (fabs(root1)>fabs(root2)) {
           nvPz1= root2; nvPz2= root1 ;
         }
         else {
           nvPz1= root1; nvPz2= root2 ;
         }
       }
       (*doubleBranches["real"]) = real;
       double nvEt = sqrt(METLV.E()*METLV.E() + nvPz1*nvPz1);

       TLorentzVector pu(leptonLV.Px(),leptonLV.Py(),leptonLV.Pz(),leptonLV.E());
       //       TLorentzVector pv(METLV.Px(),METLV.Py(),METLV.Pz(),METLV.E());
       TLorentzVector pv(METLV.Px(),METLV.Py(),nvPz1,nvEt);
       TLorentzVector pj1(jet1.Px(),jet1.Py(),jet1.Pz(),jet1.E());
       TLorentzVector pj2(jet2.Px(),jet2.Py(),jet2.Pz(),jet2.E());
       
       //KinFit vars
       double mWWjj_KF = 0 ;
       double mJJ_KF   = 0 ;
       
       TLorentzVector      fit_mup(0,0,0,0); 
       TLorentzVector      fit_nvp(0,0,0,0);
       TLorentzVector      fit_ajp(0,0,0,0); 
       TLorentzVector      fit_bjp(0,0,0,0); 
       
       Float_t             fit_chi2;
       Int_t               fit_NDF;
       Int_t               fit_status;
       double chi2 = 0 ;
       double chi2KF = 0 ;
       
       bool isMuon = false ;
       if ( muPlusJets_ ) isMuon = true ;
       
       doKinematicFit(isMuon, 1,  pu,  pv,  pj1,  pj2, 
		      fit_mup,  fit_nvp, fit_ajp,  fit_bjp, 
		      fit_chi2,  fit_NDF,  fit_status);

       (*doubleBranches["fitStatus"]) = double(fit_status);
       chi2KF = double(fit_chi2/fit_NDF) ;
       mWWjj_KF = (fit_mup + fit_nvp + fit_ajp + fit_bjp).M();
       mJJ_KF   = (fit_ajp + fit_bjp).M();
       chi2 = fit_chi2 ;

       if (fit_status==-10){
	 //         nNaN++;
         chi2 = 999. ;
         mWWjj_KF = -999. ;
         chi2KF = 999. ;
       }

       (*doubleBranches["chi2KF"]) = chi2KF;
       (*doubleBranches["chi2"]) = chi2;
       (*doubleBranches["mWWKF"]) = mWWjj_KF;
       (*doubleBranches["mJJKF"]) = mJJ_KF;

       // 
       dg_kin_Wuv_Wjj( pu, pv, pj1, pj2, cosdPhiWW, cosdPhiWH, costhetal, costhetaj, costhetaWH);
       jacksonAngle = JacksonAngle( pj1,pj2);
       
       // cout << "cosdPhiWW" << cosdPhiWW << " costhetal " << costhetal << " costhetaj" << costhetaj << " cosdPhiWH " << cosdPhiWH << " costhetaWH " << costhetaWH << endl;
       // cout <<"Jackson angle is: " << jacksonAngle << endl;
       (*doubleBranches["Cos_dPhiWW"]) = cosdPhiWW;
       (*doubleBranches["CosTheta_l"]) = costhetal;
       (*doubleBranches["CosTheta_j"]) = costhetaj;
       (*doubleBranches["Cos_dPhiWH"]) = cosdPhiWH;
       (*doubleBranches["CosTheta_WH"]) = costhetaWH;
       (*doubleBranches["jacksonAngle"]) = jacksonAngle;
     } // end if nJetsPlot ==2
    
     anaTree->Fill();
     
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
         //TH1 *hist = histograms[histName];
         //hist->Sumw2();
         //hist->Scale(histScale);
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
     
     //pat::Jet iRawJet = ijet->correctedJet(pat::JetCorrFactors::Raw);
     pat::Jet iRawJet = ijet->correctedJet("Uncorrected");
      

      

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
