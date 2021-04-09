// -*- C++ -*-

// These constants are defined to be used with the TreeFileInterface
// class for accessing Top Ntuple branches.

#if !defined(TOPTREE_H)
#define TOPTREENAMESPACE_H

namespace TopTreeNamespace
{
   // TCA Branch names
   const char kEvtBranch[]               = "evt";
   const char kSummaryBranch[]           = "summary";
   const char kPrivertexBranch[]         = "privertex";
   const char kZvtxsBranch[]             = "zvtxs";
   const char kHepgBranch[]              = "hepg";
   const char kMuonBranch[]              = "muon";
   const char kSltmuBranch[]             = "sltmu";
   const char kCosmicBranch[]            = "cosmic";
   const char kElectronBranch[]          = "electron";
   const char kTauBranch[]               = "tau";
   const char kJetBranch[]               = "jet";
   const char kMetBranch[]               = "met";
   const char kMetClassName[]            = "missingEt";
   const char kTopfitBranch[]            = "topfit";
   const char kTopfitClassName[]         = "topFit";
   const char kTrigInfoBranch[]          = "trigInfo";
   const char kTrigNameBranch[]          = "trigName";
   const char kL2CalBranch[]             = "l2Cal";
   const char kOffltrackBranch[]         = "offltrack";
   const char kSecvtxtrackBranch[]       = "secvtxtrack";
   const char kTjassocBranch[]           = "tjassoc";
   const char kTvassocBranch[]           = "tvassoc";
   const char kJetprobtrackBranch[]      = "jetprobtrack";
   const char kJetprobTJassocBranch[]    = "jetprobTJassoc";
   const char kObspBranch[]              = "obsp";
   const char kLsetBranch[]              = "lset";

   // Integer Branch names
   const char kNumZVtxsBranch[]          = "numZVtxs";          
   const char kNumHepgObjsBranch[]       = "numHepgObjs";       
   const char kNumMuObjsBranch[]         = "numMuObjs";         
   const char kNumEmObjsBranch[]         = "numEmObjs";         
   const char kNumTauObjsBranch[]        = "numTauObjs";        
   const char kNumJetObjsBranch[]        = "numJetObjs";        
   const char kNumTopFitObjsBranch[]     = "numTopFitObjs";     
   const char kNumL2CalObjsBranch[]      = "numL2CalObjs";      
   const char kNumOfflTracksBranch[]     = "numOfflTracks";     
   const char kNumSecvtxTracksBranch[]   = "numSecvtxTracks";    
   const char kNumTJAssocBranch[]        = "numTJAssoc";         
   const char kNumTVAssocBranch[]        = "numTVAssoc";         
   const char kNumJetProbTracksBranch[]  = "numJetProbTracks";  
   const char kNumJetProbTJAssocBranch[] = "numJetProbTJAssoc";  
   const char kNumObspObjsBranch[]       = "numObspObjs";       
   const char kNumTopFitBtagsBranch[]    = "numTopFitBtags";    
   const char kNumXFTHitsBranch[]        = "numXFTHits";        
   const char kNumXFTPixelsBranch[]      = "numXFTPixels";      
   const char kNumXFTTracksBranch[]      = "numXFTTracks";      
   const char kNumPhysicsTowerBranch[]   = "numPhysicsTower";    
   const char kNumTL2DTracksBranch[]     = "numTL2DTracks";     
   const char kNumImuTrigsBranch[]       = "numImuTrigs";       
   const char kNumBmuDsBranch[]          = "numBmuDs";          
   const char kNumBsuDsBranch[]          = "numBsuDs";          
   const char kNumTsuDsBranch[]          = "numTsuDs";          
   const char kNumHTDCsBranch[]          = "numHTDCs";           

   // char[40] Branch names
   const char kMuoViewDescr1Branch[]     = "muoViewDescr1"; 
   const char kMuoViewDescr2Branch[]     = "muoViewDescr2"; 
   const char kMuoViewDescr3Branch[]     = "muoViewDescr3"; 
   const char kMuoViewDescr4Branch[]     = "muoViewDescr4"; 
   const char kMuoViewDescr5Branch[]     = "muoViewDescr5"; 
   const char kEleViewDescr1Branch[]     = "eleViewDescr1"; 
   const char kEleViewDescr2Branch[]     = "eleViewDescr2"; 
   const char kEleViewDescr3Branch[]     = "eleViewDescr3"; 
   const char kEleViewDescr4Branch[]     = "eleViewDescr4"; 
   const char kEleViewDescr5Branch[]     = "eleViewDescr5"; 
   const char kJetCollDescr1Branch[]     = "jetCollDescr1"; 
   const char kJetCollDescr2Branch[]     = "jetCollDescr2"; 
   const char kJetCollDescr3Branch[]     = "jetCollDescr3"; 
   const char kJetCollDescr4Branch[]     = "jetCollDescr4"; 
   const char kJetCollDescr5Branch[]     = "jetCollDescr5"; 
   const char kJetCollDescr6Branch[]     = "jetCollDescr6"; 
   const char kJetCollDescr7Branch[]     = "jetCollDescr7"; 
   const char kJetCollDescr8Branch[]     = "jetCollDescr8"; 
   const char kJetCollDescr9Branch[]     = "jetCollDescr9"; 
   const char kJetCollDescr10Branch[]    = "jetCollDescr10";
   const char kJetCollDescr11Branch[]    = "jetCollDescr11";
   const char kJetCollDescr12Branch[]    = "jetCollDescr12";
   const char kJetCollDescr13Branch[]    = "jetCollDescr13";
   const char kJetCollDescr14Branch[]    = "jetCollDescr14";
   const char kJetCollDescr15Branch[]    = "jetCollDescr15";
   const char kTrigBitDescr0Branch[]     = "trigBitDescr0"; 
   const char kTrigBitDescr1Branch[]     = "trigBitDescr1"; 
   const char kTrigBitDescr2Branch[]     = "trigBitDescr2"; 
   const char kTrigBitDescr3Branch[]     = "trigBitDescr3"; 
   const char kTrigBitDescr4Branch[]     = "trigBitDescr4"; 
   const char kTrigBitDescr5Branch[]     = "trigBitDescr5"; 
   const char kTrigBitDescr6Branch[]     = "trigBitDescr6"; 
   const char kTrigBitDescr7Branch[]     = "trigBitDescr7"; 
   const char kTrigBitDescr8Branch[]     = "trigBitDescr8"; 
   const char kTrigBitDescr9Branch[]     = "trigBitDescr9"; 
   const char kOtrkCollDescr1Branch[]    = "otrkCollDescr1";
   const char kOtrkCollDescr2Branch[]    = "otrkCollDescr2";
   const char kOtrkCollDescr3Branch[]    = "otrkCollDescr3";
   const char kOtrkCollDescr4Branch[]    = "otrkCollDescr4";
   const char kOtrkCollDescr5Branch[]    = "otrkCollDescr5";

   // The following strings are for use with 'addActiveBranch()'
   // 'addInactiveBranch()' only to allow or remove the entire
   // collections with one command.
   
   // All integers
   const char kAllNumBranches[] = "num";
   // All muon view descriptions
   const char kAllMuoViewBranches[]      = "muoViewDescr";
   // all electron view descriptions     
   const char kAllEleViewBranches[]      = "eleViewDescr"; 
   // all jet collection descriptions    
   const char kAllJetCollBranches[]      = "jetCollDescr"; 
   // all trigger bit descriptions       
   const char kAllTrigBitBranch[]        = "trigBitDescr"; 
   // all offltrack collection descriptions
   const char kAllOtrkCollBranch[]       = "otrkCollDescr";

  // Electron CollTypes:
  const char kDefEmObj[]                 = "DEFEMOBJECTS";
  const char kPhxEmObj[]                 = "PHOENIXEMOBJECTVIEW";
                                         
  // Muon ViewTypes:                     
  const char kDefMuons[]                 = "DEFMUONS";
  const char kDefStublessMuons[]         = "DEFSTUBLESSMUONS";

  // Jet CollTypes:
  const char kJetClu04[]     = "JETCLUMODULE-CONE0.4";
  const char kJetClu07[]     = "JETCLUMODULE-CONE0.7";
  const char kJetClu10[]     = "JETCLUMODULE-CONE1.0";
  const char kJetClu04LJ[]   = "MYTOP_JETCLU0.4_LJ_RECLUSTERJETCOLL";
  const char kJetClu07LJ[]   = "MYTOP_JETCLU0.7_LJ_RECLUSTERJETCOLL";
  const char kMidPoint04LJ[] = "MYTOP_MIDPOINT0.4_LJ_RECLUSTERJETCOLL";
  const char kMidPoint07LJ[] = "MYTOP_MIDPOINT0.7_LJ_RECLUSTERJETCOLL";
  const char kJetClu04DIL[]  = "MYTOP_JETCLU0.4_DIL_RECLUSTERJETCOLL";
  const char kJetClu07DIL[]  = "MYTOP_JETCLU0.7_DIL_RECLUSTERJETCOLL";
  const char kJetClu04H1[]   = "JETCLUMODULE-CONE0.4H1";
  const char kJetClu07H1[]   = "JETCLUMODULE-CONE0.7H1";

  // Offline Tracks:
  const char kDefTrks[]      = "DEFTRACKS";
  const char kPHXNoSiTrks[]  = "PHOENIX_TRACKING";
  const char kPHXSiTrks[]    = "PHOENIXSI_TRACKING";
  const char kTrackLeptons[] = "MYTOPTRACKLEPTONVIEW";

  // Trigger Summary Word Bits:
  const char kElectronCentral18[]        = "ELECTRON_CENTRAL_18_V";
  const char kMuonCmup18[]               = "MUON_CMUP18";
  const char kMuonCmx18[]                = "MUON_CMX18";
  const char kMetPem[]                   = "MET_PEM_V";
  const char kMetPemL1[]                 = "MET_PEM_L1";
  const char kMuonBmu9L1Bmu10BsuPt11[]   = "MUON_BMU9_L1_BMU10_BSU_PT11";
  const char kMuonCentralJet20L1Bmu10Bsur[] 
  = "MUON_CENTRAL_JET20_L1_BMU10_BSUR";
  const char kMuonCentralJet20L1Bmu10Pt11[] 
  = "MUON_CENTRAL_JET20_L1_BMU10_PT11";
  const char kTopMultiJet[]              = "TOP_MULTI_JET";
  const char kPlugElectron[]             = "PLUG_ELECTRON";
  const char kJet20[]                    = "JET_20_V";
  const char kJet50[]                    = "JET_50_V";
  const char kJet70[]                    = "JET_70_V";
  const char kJet100[]                   = "JET_100_V";
  const char kJet20Central[]             = "JET_20_CENTRAL_V";
  const char kJetCalSingleTopwer5[]      = "JET_CAL_SINGLETOWER_5_V";
  const char kJetCalSingleTower10[]      = "JET_CAL_SINGLETOWER_10_V";
  const char kPhoton25Iso[]              = "PHOTON_25_ISO_V";
  const char kPhoton25IsoPass4[]         = "PHOTON_25_ISO_PASS4_V";
  const char kElectronCentral8[]         = "ELECTRON_CENTRAL_8_V";
  const char kElectronCentral8L2Dps[]    = "ELECTRON_CENTRAL_8_L2_DPS_V";
  const char kMuonCmup8[]                = "MUON_CMUP8_V";
  const char kMuonCmup8Dps[]             = "MUON_CMUP8_DPS_V";
  const char kPlugZ[]                    = "PLUG_Z";
  const char kZNotrack[]                 = "Z_NOTRACK_V";
  const char kWNoTrack[]                 = "W_NOTRACK_V";
 

};

#endif // TOPTREENAMESPACE_H


