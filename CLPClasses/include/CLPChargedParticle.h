// -*- C++ -*-

#if !defined(CLPChargedParticle_H)
#define CLPChargedParticle_H

#include <string>
#include <vector>
#include "CLPClasses/CLPObject.h"
#include "CLPClasses/CLP4Vector.h"
#include "CLPClasses/CLPNamespace.h"

class CLPChargedParticle : public CLPObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      enum ChargedParticleType
      {
         kNone,                     //  0 
         kUndefinedElectron,        //  1 
         kTightCentralElectron,     //  2 
         kPhoenixElectron,          //  3 
         kPlugElectron,             //  4 
         kUndefinedMuon,            //  5 
         kCMUP,                     //  6 
         kCMU,                      //  7 
         kCMP,                      //  8 
         kCMX,                      //  9 
         kCMIO,                     // 10
         kUndefinedTrack,           // 11
         kTrackLepton,              // 12
         kNumChargedParticleTypes   // 13
      };

      enum
      {
         kNone_CutCode  = 0,
         kTCEM_CutCode  = 1,
         kTPHX_CutCode  = 2,
         kTCMUP_CutCode = 3,
         kTCMX_CutCode  = 4,      
      };

      enum { kNumTypes = kNumChargedParticleTypes - kNone };

      typedef std::vector< CLPChargedParticle* > ChargedParticlePtrList;
      typedef std::vector< CLPChargedParticle > ChargedParticleList;

      static const std::string kChargedParticleName[kNumTypes];
      static const std::string kShortChargedParticleName[kNumTypes];

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPChargedParticle &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPChargedParticle();

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      ////  type of charged particle ////
      ChargedParticleType cpType() const { return m_cpType; }

      ////  sign (charge) of particle ////
      int sign() const { return m_sign; }
      int charge() const { return m_sign; }

      ////  fiducial region ////
      int fiducial() const { return m_fiducial; }

      ////  isolation quantity ////
      float isolation() const { return m_isolation; }
      // axial track segments
      int axSeg() const { return m_axSeg; }
      // axial track hits
      int axHits() const { return m_axHits; }
      // stereo track segments
      int stSeg() const { return m_stSeg; }
      // stereo track hits
      int stHits() const { return m_stHits; }
      // silicon hits
      int siHits() const { return m_siHits; }
      // top ntuple classification code
      int cutCode() const { return m_cutCode; }
      // my cuts code
      int passesCuts() const { return m_passesCuts; }
      float _tempeta() const { return m_eta; }
      float _tempphi() const { return m_phi; }
      // Z0 of track
      float Z0() const { return m_Z0; }
      // D0 of track
      float D0() const { return m_D0; }
      float trackIdOrig() const { return m_trackIdOrig; }
      float trackPt() const { return m_transMom; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setCpType(ChargedParticleType cpType) { m_cpType = cpType; }
      void setSign(int sign) { m_sign = sign; }
      void setCharge(int sign) { m_sign = sign; }
      void setFiducial(int fiducial) { m_fiducial = fiducial; }
      void setIsolation(float isolation) { m_isolation = isolation; }
      void setAxSeg(int axSeg) { m_axSeg = axSeg; }
      void setAxHits(int axHits) { m_axHits = axHits; }
      void setStSeg(int stSeg) { m_stSeg = stSeg; }
      void setStHits(int stHits) { m_stHits = stHits; }
      void setSiHits(int siHits) { m_siHits = siHits; }
      void setCutCode (int cutCode) { m_cutCode = cutCode; }
      void clearPassesCuts () { m_passesCuts = 0;}
      void orSetPasssesCuts (int cuts) { m_passesCuts |= cuts; }
      void setEta(float eta) { m_eta = eta; }
      void setPhi(float phi) { m_phi = phi; }
      void setZ0(float Z0) { m_Z0 = Z0; }
      void setD0(float D0) { m_D0 = D0; }
      void setPx(float px) { m_Px = px; }
      void setPy(float py) { m_Py = py; }
      void setPz(float pz) { m_Pz = pz; }
      void setTrackIdOrig(int trackIdOrig) {m_trackIdOrig = trackIdOrig; }
      void setTransMom(float transMom) { m_transMom = transMom; }

      
      ///////////////////
      // Isa Functions //
      ///////////////////
      bool isaTCE()  const { return kTightCentralElectron == m_cpType; }
      bool isaPHX()  const { return kPhoenixElectron      == m_cpType; }
      bool isaPEM()  const { return kPlugElectron         == m_cpType; }
      bool isaCMUP() const { return kCMUP                 == m_cpType; }
      bool isaCMU()  const { return kCMU                  == m_cpType; }
      bool isaCMP()  const { return kCMP                  == m_cpType; }
      bool isaCMX()  const { return kCMX                  == m_cpType; }
      bool isaCMIO() const { return kCMIO                 == m_cpType; }
      bool isa (ChargedParticleType type) const { return type == m_cpType; }
      bool isUndefined() const { return kNone == m_cpType ||
                                    kUndefinedElectron == m_cpType ||
                                    kUndefinedMuon     == m_cpType ||
                                    kUndefinedTrack    == m_cpType; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // calculate all wanted quantities from given quantities
      virtual void calcQuantities();

      // is this a tight lepton?
      bool isaTightLepton() const;

      // is this lepton isolated?
      bool isIsolated(double cutValue = 
                      CLPNamespace::kDefaultIsolationCut) const;

      // string of type
      std::string typeString() const;
      std::string shortTypeString() const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

      static bool isSameParticle(const CLPChargedParticle &one, 
                                 const CLPChargedParticle &two)
      { return (one.trackIdOrig() == two.trackIdOrig()); }

      static float deltaZ0(const CLPChargedParticle &one, 
                           const CLPChargedParticle &two)
      { return fabs(one.Z0() - two.Z0()); }


  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////
      // type of charged particle
      ChargedParticleType m_cpType;
      
      // sign (charge) of particle
      int m_sign;
      // fiducial region
      int m_fiducial;
      // isolation quantity
      float m_isolation;
      int m_axSeg;      // axial track segments
      int m_axHits;     // axial track hits
      int m_stSeg;      // stereo track segments
      int m_stHits;     // stereo track hits
      int m_siHits;     // silicon hits
      int m_cutCode;    // top ntuple classification code
      int m_passesCuts; // My cut classification 
      float m_Z0;       // Z0 of track
      float m_D0;       // D0 of track
      // Possibly temporary
      float m_eta; // ! Don't store
      float m_phi; // ! Don't store
      float m_Px;  // ! Don't store
      float m_Py;  // ! Don't store
      float m_Pz;  // ! Don't store
      int m_trackIdOrig;
      // transverse momentum
      float m_transMom;

      ClassDef (CLPChargedParticle, 2) // CLP Charged Particle Class
};


#endif // CLPChargedParticle_H
