// -*- C++ -*-

#if !defined(KinematicFitter_HH)
#define KinematicFitter_HH

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "TLorentzVector.h"
#include "TMatrixD.h"

class KinematicFitter
{
   public:
      ///////////////////////////////////
      // Public Constants and Typedefs //
      ///////////////////////////////////

      enum {
         kPx,
         kPy,
         kPz,
         kEnergy,
         kNumParametersPerParticle
      };

      enum {
         kMass = kEnergy
      };

      enum InOutType {
         kOriginal,     // before constraints
         kIntermediate, // while iterating 
         kModified      // after  constraints
      };

      enum ConstraintType {
         kSingleMassConstraint,
         kEqualMassConstraint
      };

      enum Basis {
         kE_P_basis,
         kM_P_basis
      };

      enum CovarianceMode {
         // uses all input particles in covaraiance matrix
         kNormalCovarMode, 
         // uses only needed input particles in covariance matrix
         kFastCovarMode,
         // makes one (full-size) covariance matrix per event
         kKeepCovarMode
      };

      enum ParticleType {
         kNormalParticle,
         kMassiveJet
      };

      // typedefs of built-in types
      typedef std::vector< TLorentzVector > FourVectorVec;
      typedef std::vector< int            > IVec;
      typedef std::vector< IVec           > IVecVec;
      typedef std::vector< TMatrixD       > TMatrixDVec;
      typedef std::vector< TMatrixD*      > TMatrixDPtrVec;
      typedef std::map   < int, int       > IIMap;
      typedef std::set   < int            > ISet;

      struct ConstraintStruct
      {
            int            index1;
            int            index2;
            double         massConst;
            double         def_massConst;
            ConstraintType type;
            bool           isActive;
            bool           def_isActive;
      };

      struct VirtualParticle
      {
            bool usesInputParticles;
            IVec particleList;
      };

      struct InputParticle
      {
            double Px;
            double Py;
            double Pz;
            double E;
            double mass;
            double percentage;
            ParticleType type;
      };

      // typedefs of structs
      typedef std::vector< ConstraintStruct > ConstraintVec;
      typedef std::vector< VirtualParticle  > VirtualParticleVec;
      typedef std::vector< InputParticle    > InputParticleVec;

      // typedefs of iterators
      typedef IVec::iterator                 IVecIter;
      typedef IVecVec::iterator              IVecVecIter;
      typedef IIMap::iterator                IIMapIter;
      typedef ISet::iterator                 ISetIter;
      typedef ConstraintVec::iterator        ConstraintVecIter;
      typedef VirtualParticleVec::iterator   VirtualParticleVecIter;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       KinematicFitter &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      KinematicFitter();
      ~KinematicFitter();

       ////////////////
      // One Liners //
      ////////////////

      // Number of input particles
      int numInputParticles() { return (int) m_inputParticles.size(); }

      // Number of virtual particles
      int numVirtualParticles() { return (int) m_virtualParticles.size(); }

      // Set the maximum number of iterations allowed
      void setNumIterations (int num) { m_numIterations = num; }

      // Set the chi^2 delta difference that will stop iteration
      void setChi2Delta (double delta) { m_chi2delta = delta; }

      // Set the chi^2 limit such that chi^2 > limit are not iterated
      // (i.e., we don't want to waste time on bad combinations)
      void setMaxChi2 (double max) { m_maxChi2 = max; }

      // Set the mode of the covariance matrix
      void setCovarianceMode (CovarianceMode mode) { m_covarMode = mode; }

      // change the basis (use BEFORE adding particles)
      void changeBasis (Basis basis) { m_basis = basis; }

      // set the default particle type
      void setDefaultParticleType (ParticleType type)
      { sm_defaultParticleType = type; }

      ////////////////////////////////////
      // ////////////////////////////// //
      // // Regular Member Functions // //
      // ////////////////////////////// //
      ////////////////////////////////////

      /////////////////////
      // Input Particles //
      /////////////////////

      // Reset all input particles and their error matricies.  Any
      // virtual particles that use input particles (i.e. NOT super
      // particles) will have their particle assignments reset.
      void clearAllInputParticles();
      
      // add an input 4 vector.  The index of the particle is
      // returned.  You can either give the errors (on E, phi, and
      // theta) now or later with 'updateErrorFirInputParticle()'
      int addInputParticle (const TLorentzVector &vec,
                            const TMatrixD &errMat);
      int addInputParticle (const TLorentzVector &vec,
                            ParticleType type,
                            const TMatrixD &errMat);
      int addInputParticle (const TLorentzVector &vec, double dE,
                            double dTheta = 0. ,double dPhi = 0.);
      int addInputParticle (const TLorentzVector &vec, 
                            ParticleType type, double dE,
                            double dTheta = 0. ,double dPhi = 0.);
      int addInputParticle (const TLorentzVector &vec);
      int addInputParticle (const TLorentzVector &vec, ParticleType type);

      // set the error for an input particle
      void updateErrorForInputParticle (int index, double dE,
                                        double dPhi, double dTheta);

      ///////////////////////
      // Virtual Particles //
      ///////////////////////

      // Add a virtual particle made of input particles.
      // Returns the index of the virtual particle.
      int createVirtualParticle (int in1 = -1, int in2 = -1, int in3 = -1, 
                                 int in4 = -1, int in5 = -1, int in6 = -1);
      int createVirtualParticle (IVec &vec);

      // Updates the input particles that a given virtual particle
      // looks at.  This does NOT work for super particles.
      void updateVirtualParticle (int virtualIndex, 
                                  int in1,      int in2 = -1, 
                                  int in3 = -1, int in4 = -1, 
                                  int in5 = -1, int in6 = -1);
      void updateVirtualParticle (int virtualIndex, IVec &vec);

      // Add a virtual particle made of other virtual particles (the
      // other virtual particles must already be defined).  Returns
      // the index of the virtual particle.
      int createSuperVirtualParticle (int in1, int in2, int in3 = -1, 
                                      int in4 = -4, int in5 = -1, 
                                      int in6 = -1);
      int createSuperVirtualParticle (IVec &vec);

      //////////////////////
      // Mass Constraints //
      //////////////////////

      // Add a mass constraint of a virtual particle.  Returns
      // constraint index.
      int addSingleMassConstraint (int virtIndex, double mass);

      // Add a mass constraint to two virtual particles (to each
      // other).  Returns constraint index.
      int addEqualMassConstraint (int virtIndex1, int virtIndex2);

      // update a single mass constraint mass (inputs are constraint
      // index and new mass)
      void updateSingleMassConstraint (int constraintIndex, double mass);

      // return the current mass constraint of a single mass constraint
      double currentSingleMassConstraint (int constraintIndex);

      // sets a constraint as active
      void setConstraintActive (int constraintIndex);

      // sets a constraint as inactive
      void setConstraintInactive (int constraintIndex);

      // returns true if the given constraint is active
      bool isConstraintActive (int constraintIndex);

      // Save the current state of constraints as the default
      void saveCurrentConstraintsAsDefault();

      // reset all constraints to their defaults
      void resetConstraints();

      // Return number of active constraints
      int numConstraints();

      // Return number of total constraints
      int numTotalConstraints();

      ///////////////////////
      // Particle Momentum //
      ///////////////////////

      // return the 4 momentum of an original particle (modified
      // values by default)
      TLorentzVector particleMom (int index, InOutType type = kModified);

      // return the individual components of input particles (used
      // mostly internally; modified values by default)
      double E  (int index, InOutType type = kModified);
      double Px (int index, InOutType type = kModified);
      double Py (int index, InOutType type = kModified);
      double Pz (int index, InOutType type = kModified);
      // return the individual components of virtual particles (used
      // mostly internally; modified values by default)
      double virtualE  (int index, InOutType type = kModified);
      double virtualPx (int index, InOutType type = kModified);
      double virtualPy (int index, InOutType type = kModified);
      double virtualPz (int index, InOutType type = kModified);

      // return the 4 momentum of a virtual particle (modified values
      // by default)
      TLorentzVector virtualParticleMom (int index, 
                                         InOutType type = kModified);

      // Returns an STL vector of TLorentzVectors of all of the input
      // particles for a given type
      void fillAllInputMomentumVector (FourVectorVec &retval,
                                       InOutType type = kModified);

      // Returns an STL vector of TLorentzVectors of all of the
      // virtual particles for a given type
      void fillAllVirtualMomentumVector (FourVectorVec &retval,
                                         InOutType type = kModified);

      /////////////////////////////
      // /////////////////////// //
      // // The Big Enchilada // //
      // /////////////////////// //
      /////////////////////////////
      // The *magic* routine - Minimizes and returns chi^2
      double minimizeChi2 ();


  private:
      //////////////////////////////
      // Private Member Functions //
      //////////////////////////////
      // validate the different indicies
      bool _validateInputIndex (std::string  filename,
                                int linenum,
                                InOutType type,
                                int index);
      bool _validateVirtualPartIndex (std::string  filename,
                                      int linenum,
                                      InOutType type,
                                      int index,
                                      bool checkInputParticles = false);
      bool _validateConstraintIndex (std::string  filename,
                                     int linenum,
                                     int index);


      // setup a single mass constrint
      void _setupSingleConstraint (TMatrixD* D, TMatrixD* dvec, 
                                   int &constraintIndex,
                                   const ConstraintStruct &constraint);

      // setup a single mass constrint
      void _setupEqualConstraint (TMatrixD* D, TMatrixD* dvec, 
                                   int &constraintIndex,
                                   const ConstraintStruct &constraint);

      // returns a pointer to a TTMatrixD containing delta alpha. If you
      // call with useDeleteList = false, you are responsible for
      // cleaning up the pointer.
      TMatrixD* _deltaAlpha (int numUsedParticles, 
                             bool useDeleteList = true);

      // return the complete list of input particles that make up this
      // virtual particle.  When called with 'clear=false', retval
      // is NOT cleared in the function.
      void _allInputParticles (int virtualParticleIndex, IVec &retval,
                               bool clear = true);

      // returns the offset needed for particle/matrix interation
      int _calcOffset (int partIndex);

      // returns the number of input particles that will be in the
      // covariance matrix. The function will also setup the set
      // of used particles as well as the map used by _calcOffset().
      // - kNormalCovarMode - all of them
      // - kFastCovarMode - only the input particles that part of the
      //   active constraints.  
      int _numUsedInputParticles();



      /////////////////////////////////////
      // Private Static Member Functions //
      /////////////////////////////////////
      // zero a matrix
      static void _zeroMatrix (TMatrixD &matrix);
      static void _zeroMatrix (TMatrixD *matrix) { _zeroMatrix (*matrix); }

      // fills a 4x4 covaraince matrix from a TLorentzVector and dE,
      // dTheta, and dPhi
      static void _covarianceMatrixFromDeltaETP (TMatrixD &matrix,
                                                 const TLorentzVector &vec,
                                                 double dE,
                                                 double dTheta,
                                                 double dPhi,
                                                 Basis basis);
      
      // fills a Marcel-like 4x4 covaraince matrix from a
      // TLorentzVector a percent error
      static void _covarianceMatrixALaMarcel (TMatrixD &matrix,
                                              const TLorentzVector &vec,
                                              double percent = 0.15);

      // inserts a small matrix into a larger matrix at given place
      static void _insertSmallMatrixIntoLarge (const TMatrixD &small,
                                               TMatrixD &large,
                                               int rowOff, int colOff);

      // returns the product of two or three matricies as a NEW
      // pointer.  Either you delete the pointers by hand when you are
      // finished, OR you call these functions with
      // 'useDeleteList=true' and you call '_cleanupTMatrixPtrs()'
      // when you are finished.  If you want to keep the pointer
      // afterwards (e.g., to store the result in member data), call
      // with 'useDeleteList=false'.
      static TMatrixD* _mult (const TMatrixD &alpha, 
                              const TMatrixD &beta,
                              bool useDeleteList = true);
      static TMatrixD* _mult (TMatrixD *alphaPtr, 
                              TMatrixD *betaPtr,
                              bool useDeleteList = true);
      static TMatrixD* _mult (const TMatrixD &alpha, 
                              const TMatrixD &beta,
                              const TMatrixD &gamma,
                              bool useDeleteList = true);
      static TMatrixD* _mult (TMatrixD *alphaPtr, 
                              TMatrixD *betaPtr,
                              TMatrixD *gammaPtr,
                              bool useDeleteList = true);
      static TMatrixD* _add (const TMatrixD &alpha, 
                             const TMatrixD &beta,
                             bool useDeleteList = true);
      static TMatrixD* _add (TMatrixD *alphaPtr, 
                             TMatrixD *betaPtr,
                             bool useDeleteList = true);
      static TMatrixD* _subtract (const TMatrixD &alpha, 
                                  const TMatrixD &beta,
                                  bool useDeleteList = true);
      static TMatrixD* _subtract (TMatrixD *alphaPtr, 
                                  TMatrixD *betaPtr,
                                  bool useDeleteList = true);

      // returns the transpose of a matrix as a NEW pointer.  See
      // memory manamgement comment for '_mult()'
      static TMatrixD* _transpose (const TMatrixD &alpha,
                                   bool useDeleteList = true);
      static TMatrixD* _transpose (TMatrixD *alphaPtr,
                                   bool useDeleteList = true);

      // deletes the Matrix pointers from _mult() and _transpose()
      static void _cleanupMatrixPtrs();

      // square a number
      static double _sq (double x) { return x * x; }

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      
      // input partlces
      InputParticleVec   m_inputParticles;
      InputParticleVec   m_middleParticles; // intermediate step
      InputParticleVec   m_outputParticles;
      TMatrixDVec        m_inputErrorMatricies;

      // virtual particles
      VirtualParticleVec m_virtualParticles;

      // Constraints
      ConstraintVec      m_constraintVec;

      // other parameters
      double             m_chi2delta;
      double             m_maxChi2;
      int                m_numIterations;
      CovarianceMode     m_covarMode;
      ISet               m_usedInputPartSet;
      IIMap              m_usedInputPartMap;
      Basis              m_basis;

      TMatrixD          *m_V_a0;

      // keeps track of temporary allocated TMatrixD pointers for
      // later deletion.
      static TMatrixDPtrVec sm_matrixPtrVec;
      static ParticleType   sm_defaultParticleType;
};

// ostream operators for TMatrixD
std::ostream& operator<< (std::ostream& o_stream, 
                          const TMatrixD &rhs);
std::ostream& operator<< (std::ostream& o_stream, 
                          TMatrixD *rhs);

// ostream operators for TLorentzVectors
std::ostream& operator<< (std::ostream& o_stream, 
                          const TLorentzVector &rhs);

// ostream operators for KinematicFitter types
std::ostream& operator<< (std::ostream& o_stream,
                          const KinematicFitter::ISet &rhs);
std::ostream& operator<< (std::ostream& o_stream,
                          KinematicFitter::IVec &rhs);
std::ostream& operator<< (std::ostream& o_stream,
                          KinematicFitter::InputParticle &rhs);

#endif // KinematicFitter_HH
