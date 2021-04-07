// -*- C++ -*-

#if !defined(CLP_HLO_SkimUtils_H)
#define CLP_HLO_SkimUtils_H

// This namespace has utilities to convert from TopNtuple and
// HighLevelObjects (HLOs) to CLPTree using CLPClasses.  Since these
// functions depend on HighLevelObjects (as well as others), I will
// not make these two files (.h,.cc) part of the CLPClasses library.

#include "HighLevelObjects/HighLevelObjects.hh"
#include "JetUser/JetEnergyCorrections.hh"
#include "BTagObjects/jetMistagWinter2004.hh"
#include "BTagObjects/jetMistagJan2007.hh"
// #include "BTagObjects/PredictedDistribution.hh" // not yet needed
#include "CLPClasses/CLPClasses.h"

namespace CLP_HLO_SkimUtils
{

   ////////////////////////
   // Constants and Data //
   ////////////////////////
   const int kTotalNumJetCorr = 2 * CLPJet::kNumJetCorrections;
   extern double ns_sumEt;
   extern double ns_hardwiredPtCutoff;
   extern double ns_hardwiredEtaCut;
   extern bool   ns_useOldMistag;
   extern bool   ns_debugFlag;
   extern jetMistagWinter2004 *ns_2004looseMistagPtr;
   extern jetMistagWinter2004 *ns_2004tightMistagPtr;
   extern jetMistagJan2007    *ns_2007looseMistagPtr;
   extern jetMistagJan2007    *ns_2007tightMistagPtr;
   extern jetMistagJan2007    *ns_2007ultraMistagPtr;


   /////////////////////
   // /////////////// //
   // // Functions // //
   // /////////////// //
   /////////////////////

   // For historical reasons, the charge of a particle is stored as a
   // float in top ntuple, but an integer in my code.  For this
   // reason, I am writing a converter that "carefully" converts
   // floats to ints.
   int f2i (float value);

   //////////////////////////////////////////
   // Object Conversions (from HLO to CLP) //
   //////////////////////////////////////////
   CLPElectron clpElectron (int index, TClonesArray *elecTcaPtr);
   CLPMuon clpMuon (int index, TClonesArray *muonTcaPtr);
   CLPTrack clpTrack (int index, TClonesArray *trackTcaPtr);
   CLPJet clpJet (int index, TClonesArray *jetTcaPtr,
                         JetEnergyCorrections *jetCorrArray[kTotalNumJetCorr]);
   CLPHEPGObject clpHEPGObject (int index, TClonesArray *hepgTcaPtr);

   // returns a vector of daughter indicies
   CLPNamespace::IVec getHepgDaughters (int index, 
                                        TClonesArray *hepgTcaPtr);

   // Fills HEPG container with top decay
   void fillHEPGCont (CLPHEPGCont &cont, TClonesArray *hepgTcaPtr);

   // Fills HEPG container with Z bosons
   void fillHEPGContWithZs (CLPHEPGCont &cont, TClonesArray *hepgTcaPtr);

   // Does deltaR matching between HEPG particles and the electrons,
   // muons, and tracks.
   void doLeptonHEPGMatching (CLPHEPGCont &cont, 
                              TClonesArray &elecTca,
                              TClonesArray &muonTca,
                              TClonesArray &trackTca);

   // Does deltaR matching between HEPG particles and jets
   void doJetHEPGMatching (CLPHEPGCont &cont, 
                           CLPJet::JetPtrList &jetPtrList);

   // Does a dump to STDOUT of the HLO HEPG bank
   void dumpHepg (TClonesArray *hepgTcaPtr, int depth = 30);

   // Returns true if there are viable Z candidates.  Should be
   // replaced by CLPFunctions::makeZcandidates().
   bool hasGoodZs (TClonesArray &elecTca, TClonesArray &muonTca, 
                   TClonesArray &trackTca, TClonesArray &jetTca);

   // Looks for heavy flavor using the HEPG bank
   void hepgHeavyFlavorCheck (bool &hasBottom, bool &hasCharm, 
                              TClonesArray *hepgTcaPtr);

   // calculates weight factors according to heavy flavor and mistag
   // probabilities.  First pointer is of CLPJets, not HLO's 'jet'
   double jetBtagObspAccounting (TClonesArray *clpjetTcaPtr,
                                 TClonesArray *obspTcaPtr,
                                 int runNumber);

   // prints out migtag numbers for all jets
   void dumpMistagRates (TClonesArray *clpjetTcaPtr);

   // returns true if particle is has bottom or charm quarks
   inline bool hasBottomContent (int cdfid);
   inline bool hasCharmContent  (int cdfid);

   // returns third and fourth digits (base 10) of an integer
   int getThirdDigit (int number);
   int getFourthDigit (int number);

   // loose and tight alpha beta correction factors (stolen from Ulrich)
   double alphaBetaCorrTight (int runnumber, double sumEt);
   double alphaBetaCorrLoose (int runnumber, double sumEt);

   // using the HEPG bank and "good" jet three vectors, routine checks
   // to see what kind of heavy flavor is present.
   unsigned int checkHeavyFlavor (TClonesArray *hepgTcaPtr,
                                  const CLPFunctions::TVector3Vec &jet3VecList);

   //////////////////////////
   // BTagObject Functions //
   //////////////////////////

   // Initializes mistag objects with root files
   void initialize2004BTagObjects (const char* tight_mistag_file,
                                   const char* loose_mistag_file);
   void initialize2007BTagObjects (const char* ultra_mistag_file,
                                   const char* tight_mistag_file,
                                   const char* loose_mistag_file);

   // clears out stored information (called before each event)
   void clearBTagObjects();

   // Applies mistag matrix to the stored jet information
   void applyMistagMatricies();

};

#endif // CLP_HLO_SkimUtils_h
