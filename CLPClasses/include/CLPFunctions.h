// -*- C++ -*-

#if !defined(CLPFunctions_H)
#define CLPFunctions_H

#include "TClonesArray.h"
#include "TChain.h"
#include "CLPNamespace.h"
#include "CLP4Vector.h"
#include "CLPJet.h"
#include "CLPElectron.h"
#include "CLPMuon.h"
#include "CLPTrack.h"
#include "CLPCandidate.h"
#include <map>
#include <string>
#include <vector>

using namespace CLPNamespace;

namespace CLPFunctions
{

   // enums
   enum
   {
      kStdHep_MatrixElement = 3,
      kStdHep_ShowerOrME    = 2,
   };
   enum
   {
      // Charm Flags
      kHasHEPGCharm,
      kHasCPartonShower,
      kHasCbarPartonShower,
      kHasCMatrixElement,
      kHasCbarMatrixElement,
      kHasCcbarSameJetPartonShower,
      kHasCcbarSameJetMatrixElement,
      // Bottom Flags
      kHasHEPGBottom,
      kHasBPartonShower,
      kHasBbarPartonShower,
      kHasBMatrixElement,
      kHasBbarMatrixElement,
      kHasBbbarSameJetPartonShower,
      kHasBbbarSameJetMatrixElement,
      kNumHeavyFlavorEnums,
   };
 

   // type defs
   typedef std::map< std::string, void* > OptionMap;
   typedef OptionMap::iterator            OptionMapIter;
   typedef std::vector< TVector3 >        TVector3Vec;
   typedef TVector3Vec::const_iterator    TVector3VecConstIter;

   // variables
   extern OptionMap ns_IntMap;
   extern OptionMap ns_DoubleMap;
   extern OptionMap ns_StringMap;
   extern OptionMap ns_BoolMap;

   extern bool   ns_mZc_tightZ;
   extern bool   ns_mZc_noQ;
   extern bool   ns_mZc_useTracks;
   extern double ns_mZc_delta_z0_cut;

   extern int                 ns_topEvent;
   extern int                 ns_numDil;
   extern double              ns_jetZ0;
   extern CLP3Vector         *ns_met3VecPtr;
   extern CLPChargedParticle *ns_leptonPtr;

   // Command line argument parsing
   void parseArguments (int argc, char** argv);
   bool setVariableFromString (const std::string &arg);
   void listAllParameters();

   // Utility functions
   // converts eta to theta
   double eta2theta (double eta);
   // converts theta to eta
   double theta2eta (double theta);
   // converts radians to degrees
   double rad2deg (double theta);
   // converts degrees to radians
   double deg2rad (double theta);
   // adds numbers in quadrature
   double addInQuad (double one, double two);
   double addInQuad (double one, double two, double three);
   double addInQuad (double one, double two, double three, 
                     double four, double five = 0, double six = 0, 
                     double seven = 0, double eight = 0);
   // difference between two phi angles (phi2 - phi1).  
   // Corrected for wrap around.
   double deltaPhi (double phi1, double phi2);
   
   // creates a new TCA of the same type of rhs and 
   // fills it with copys of all of the entries in
   // the rhs.
   // 
   void copyTCA (const TClonesArray *sourcePtr, 
                 TClonesArray *targetPtr);

   // calculate the sphericity tensor eigen values of a list of 3 vectors
   void sphericityTensorEigenValues (CLPNamespace::DVec &eigenvalues,
                                     const CLP3Vector::V3List &v3list);

   // return the sphericity and aplanarity of a list of 3 vectors
   void sphericityAplanarity (double &sphericity,
                              double &aplanarity,
                              const std::vector<CLP3Vector> &v3list);

   // returns the centrality of the four vector list
   double centrality (const CLP3Vector::V3List &v3list);

   // modify the two input 4vectors so that they have the given mass
   // by moving each vector by its width.  Returns the 'factor' used.
   // The upper widths are used if the we need to move the mass
   // upwards (otherwise we use the lower masses).  If you do not give
   // lower masses, the upper masses will be used by default.
   double fix4VectorsForMass (CLP4Vector &vec1, CLP4Vector &vec2,
                              double targetMass, 
                              double upperwidth1, double upperwidth2,
                              double lowerwidth1 = -1, 
                              double lowerwidth2 = -1.);

   // splits a line by separator character into a vector of strings.  
   void splitLine (SVec &retval, std::string line, char separator = ' ');

   // assembles a list of Z candidates
   void makeZcandidates (CLPCandidate::CandidateList &candList,
                         const CLPElectron::ElectronList &elecList,
                         const CLPMuon::MuonList &muonList,
                         const CLPTrack::TrackList &trackList);

   // returns true if the event passes lepton + jets selection cuts
   bool passesLJfilter (TClonesArray *jetTcaPtr,
                        TClonesArray *elecTcaPtr,
                        TClonesArray *muonTcaPtr,
                        CLPJet::JetList &jetList);

   // corrects the MET vector (ns_met3VecPtr) for the jet corrections
   void correctMETforJets (const CLPJet::JetList &jetList, 
                           int level = CLPJet::kDefaultCorrection);

   // decodes the heavy flavor content and tells you whether an event
   // should be kept or not for a given sample.  Increments
   // numRejected appropriately.
   bool acceptableHeavyFlavorContent (unsigned int heavyFlavorContent,
                                      bool isHeavyFlavorSample,
                                      int &numRejected);
  
   // returns true if the bit is set
   bool testBit (unsigned int flag, int bit);

   void decodeHeavyFlavorWord (unsigned int heavyFlavorWord);

   //////////////////////
   // Access Functions //
   //////////////////////
   // Data or MC?
   void setDataType (DataType type);
   DataType dataType();
   bool isData();
   bool isMC();
   
   // Debug mode
   void setDebug (bool debug = true);
   bool debug();
   
   
   // ntuple mode
   void setNtupleType (NtupleType type);
   NtupleType ntupleType();
   bool isRochNtuple();
   bool isTopNtuple();
   bool isUCNtuple();
   
   // access and set uesr variables
   void setUserFloat (int index, double value);
   void setUserInt (int index, int value);
   void setUserString (int index, std::string value);
   void setUserBitOn (int index);
   void setUserBitOff (int index);
   bool isUserBitOn (int index);
   double userFloat (int index);
   int userInt (int index);
   std::string userString (int index);

   // outputmode
   void setOutputMode (CLPOutputMode mode);
   CLPOutputMode outputMode();

   // others
   void setTopMass (double mass);
   void setLowerTopMass (double mass);
   void setUpperTopMass (double mass);
   double topMass();
   double lowerTopMass();
   double upperTopMass();

   // Returns the number of sigma deviation, using a positive or
   // negative sigma where appropriate.
   double numAsymSigma (double meas, double trueVal, 
                        double posSig, double negSig);

   // return double bifircated Gaussian value for W mass (mode 0), tWb
   // mass (mode 1), or tZc mass (mode 2)
   double doubleBifGaus (double mass, int mode);

   // returns the 'FoM' for t -> Wb
   double figureOfMerit_tWb (const CLPJet::JetList &jetList,
                             OptMethodType method,
                             bool fixWmass = true);

   // returns the 'Figure of Merit' for the Z candidate and four jets
   double figureOfMerit_ttbar (const CLP4Vector& lep0,
                               const CLP4Vector& lep1,
                               const CLPJet::JetList &jetList,
                               int method = 0);

   // creates a TChain from one file of type 'treename'
   TChain* chainOneFile (const std::string& filename, 
                         const char *treename = "TopTree");

   // creates a TChain from a list of files of type 'treename'
   TChain* chainFromList (const std::string& filename, 
                          const char *treename ="TopTree");


   double W_width   ();
   double tWb_width ();
   double tZc_width ();
   void setW_width   (double width);
   void settWb_width (double width);
   void settZc_width (double width);
   
   double sq (double value);


};


#endif // CLPFunctions_H
