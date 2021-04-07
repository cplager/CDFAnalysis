// -*- C++ -*-

#if !defined(NAMESPACE_H)
#define NAMESPACE_H

#include <string>
#include <vector>
#include "TSystem.h"
//#include <math.h>

namespace CLPNamespace
{
   /////////////////////
   // /////////////// //
   // // Constants // //
   // /////////////// //
   /////////////////////

   // typedefs
   typedef std::vector< int >         IVec;
   typedef std::vector< double >      DVec;
   typedef std::vector< std::string > SVec;


   /////////////////////////////////////////////////////////////
   // The value next to each unit is the multiplicative scale //
   // needed to change that unit in to the base unit.         //
   //                                                         //
   // The 'TO_' constants are used to display a value in      //
   // those units.                                            //
   //                                                         //
   // For example:                                            //
   // 1 * TeV = 1 * 1e3 = 1000 MeV                            //
   // 1 * degree = 1 * pi / 180 = 0.0174532.... radians       //
   //                                                         //
   // phi = 30 * DEGREES; // phi = 30 degrees or pi/6 radians //
   // cout << phi * TO_DEGREES; // outputs phi in degrees     //
   /////////////////////////////////////////////////////////////

   //Isolation Cut Constants
   const double kDefaultIsolationCut      = 0.1;
   const double kDefaultTrackIsolationCut = 0.9;

   // base constants
   const double kPi = 3.1415926535897932384626433832795;
   const double kE  = 2.71828182845905;
   const double PI  = kPi;
   const double E   = kE;

   // angle (base: 1 radian)
   const double RADIANS    = 1;
   const double DEGREES    = PI / 180;
   const double TO_RADIANS = 1 / RADIANS;
   const double TO_DEGREES = 1 / DEGREES;

   // distance (base: 1 cm)
   const double CM         = 1;
   const double M          = 1e2;
   const double MM         = 1e-1;
   const double MICRONS    = 1e-4;
   const double TO_CM      = 1 / CM;
   const double TO_M       = 1 / M;
   const double TO_MM      = 1 / MM;
   const double TO_MICRONS = 1 / MICRONS;

   // energy (base: 1 GeV)
   const double TEV    = 1e3;
   const double GEV    = 1;
   const double MEV    = 1e-3;
   const double KEV    = 1e-6;
   const double TO_TEV = 1 / TEV;
   const double TO_GEV = 1 / GEV;
   const double TO_MEV = 1 / MEV;
   const double TO_KEV = 1 / KEV;

   // Random constants
   const double kCos10      = cos ( 10 * DEGREES );
   const double kCos20      = cos ( 20 * DEGREES );
   const double kInit       = -999.0;
   const double kErrorValue = -9999999.0;
   // Mass constants
   const double kZmass           =  91.1876 * GEV;
   const double kZwidth          =   2.4952 * GEV;
   const double kWmass           =  80.425  * GEV;
   const double kWwidth          =   2.124  * GEV;
   const double kUpperTightZmass = 106      * GEV;
   const double kLowerTightZmass =  76      * GEV;

   // Top Ntuple Summary Block Top Event cuts
   const int kTopEvent_CosmicFlag = 0x20;
   const int kTopEvent_ZVeto      = 0x08;

   // Root-based constants
#ifndef __CINT__ // silly CINT
   const char kElectronClassName[]    = "CLPElectron";
   const char kMuonClassName[]        = "CLPMuon";
   const char kJetClassName[]         = "CLPJet";
   const char kTrackClassName[]       = "CLPTrack";
   const char kHepgObjectClassName[]  = "CLPHEPGObject";
   const char kHepgContClassName[]    = "CLPHEPGCont";
   const char kPermutationClassName[] = "CLPPermutation"; 
   const char k3VectorClassName[]     = "CLP3Vector";
   const char k4VectorClassName[]     = "CLP4Vector";

   // Integer branches
   const char kEventBranch[]       = "eventNum";
   const char kRunBranch[]         = "runNum";
   const char kSectionBranch[]     = "runSection";
   const char kNumVertexBranch[]   = "numVtxs";
   const char kTopEventBranch[]    = "topEvent";
   const char kNumDilLepBranch[]   = "numDilLep";
   const char kTrigSumWordBranch[] = "trigSumWord";
   const char kHeavyFlavorBranch[] = "heavyFlavorWord";
   // Double branches
   const char kWeightBranch[]      = "weight";
   const char kJetZ0Branch[]       = "jetZ0";
   // TClonesArray Branches        
   const char kElectronBranch[]    = "electrons";
   const char kMuonBranch[]        = "muons";
   const char kJetBranch[]         = "jets";
   const char kTrackBranch[]       = "tracks";
   // Class Branches               
   const char kHepgBranch[]        = "hepg";
   const char kMetBranch[]         = "met";
#endif

   ///////////
   // Enums //
   ///////////
   enum DataType 
   {
      kData,
      kMC
   };
   
   enum NtupleType
   {
      kTopNtuple,
      kRochNtuple,
      kUCNtuple
   };
   
   enum CLPOutputMode
   {
      kStandardOutput,
      kShortOutput,
   };

   enum {kNumUserFloats = 10,
         kNumUserInts = 10,
         kNumUserStrings = 10
   };
   
   enum OptMethodType
   {
      kSquaredSum,
      kWidth,
   };
   
   enum 
   {
      kBottomJet,
      kCharmJet,
      kW1Jet,
      kW2Jet,
      kNumJets,
   };
   
   /////////////////////
   // /////////////// //
   // // Variables // //
   // /////////////// //
   /////////////////////

   extern DataType ns_dataType;
   extern NtupleType ns_ntupleType;
   extern bool ns_debug;
   extern int ns_userInts[kNumUserInts]; 
   extern double ns_userFloats[kNumUserFloats];
   extern std::string ns_userStrings[kNumUserStrings];
   extern Long64_t ns_userBits;
   extern int ns_runNumber;
   extern int ns_eventNumber;
   // signal veto
   extern int ns_veto_jetCount;
   extern float ns_veto_jetEt;
   // should the run number be used in making cuts?
   // (not usually in MC)
   extern bool ns_ignoreRunNumber;
   extern CLPOutputMode ns_outputMode;
   extern double ns_topMass;
   extern double ns_WMass;
   extern double ns_lowerTopMass;
   extern double ns_upperTopMass;
   extern double ns_errorFOM;
   // candidate variablles
   extern OptMethodType ns_method;
   extern double ns_W_width;
   extern double ns_tWb_width;
   extern double ns_tZc_width;
   extern bool ns_fixWmass;

   extern int ns_ELETYPE_DefEm;  
   extern int ns_ELETYPE_Phoenix;
   extern int ns_MUOTYPE_DefMuon;        
   extern int ns_MUOTYPE_DefStublessMuon;
   extern int ns_JETTYPE_Jetclu_04_LJ;   
   extern int ns_JETTYPE_Jetclu_07_LJ;      
   extern int ns_OTRKTYPE_DefTrack;   
   extern int ns_OTRKTYPE_TrackLepton;



};

#endif // NAMESPACE_H
