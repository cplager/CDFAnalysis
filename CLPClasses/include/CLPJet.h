// -*- C++ -*-

#if !defined(CLPJet_H)
#define CLPJet_H

#include <vector>
#include "CLPObject.h"
#include "CLP4Vector.h"

class CLPJet : public CLPObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      typedef std::vector< CLPJet >  JetList;
      typedef std::vector< CLPJet* > JetPtrList;
      
      enum
      {
         // something negative
         kUncorrected = -1,
         kDefaultCorrection = kUncorrected - 1
      };

      enum JetCone
      {
         k04,
         //k07,
         //k10,
         kNumberCones,
         kNoCone
      };

      enum SVXTagType
      {
         kLoose,
         kTight,
         kUltra,
         kNumTagTypes,
         kDefaultTag
      };

      enum ScaleFactorType
      {
         kNoScaleFactor,
         kLightJetScaleFactor,
         kBottomJetScaleFactor,
      };

      enum
      {
         kCharmFlavor = 1 << 0,
         kBottomFlavor = 1 << 1,
      };
         
      enum
      {
         kLevel5 = 0,
         kLevel7,
         // plus 1 sigma corrections
         kLevel5Plus,
         kLevel7Plus,
         // minus 1 sigma corrections
         kLevel5Minus,
         kLevel7Minus,
         kNumJetCorrections,
      };

      // You need the CLPJet:: because this is a pointer to a
      // NON-static member function.
      typedef CLP4Vector (CLPJet::*FourMomFuncPtr) (int) const;
      
      static const FourMomFuncPtr kMasslessFourMom;
      static const FourMomFuncPtr kMassiveFourMom;
      static const FourMomFuncPtr kMassiveZeroMassFourMom;
      static const FourMomFuncPtr kMassiveDropMassFourMom;
      static       FourMomFuncPtr defaultFourMom;
      

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPJet &rhs);

      //////////////////////////
      // .  .      ._         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPJet();

      bool operator< (const CLPJet& right) const;
      bool operator> (const CLPJet& right) const;

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      float        eta()            const { return mom3Vec().eta(); }
      float        phi()            const { return mom3Vec().phi(); }
      float        etaDetector()    const { return m_etaDetector; }
      float        emFraction()     const { return m_emFraction; }
      int          collectionType() const { return m_collectionType; }
      bool         IsSortable()     const { return true; } // for root
      JetCone      jetCone()        const { return m_jetCone; }
      float        wwoMassFactor()  const { return m_withWithoutMassFactor; }
      unsigned int heavyFlavor()    const { return m_heavyFlavor; }
      float        correctionScale(int type) const { return JCscale(type); }
      bool         hasCharmFlavor()  const 
      { return (bool) (m_heavyFlavor & kCharmFlavor)  ; }
      bool         hasBottomFlavor() const 
      { return (bool) (m_heavyFlavor & kBottomFlavor) ; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setEtaDetector (float etaDetector) { m_etaDetector = etaDetector; }
      void setEmFraction  (float emFraction)  { m_emFraction = emFraction; }
      void setCollectionType (int collectionType) 
      { m_collectionType = collectionType; }
      void clearHeavyFlavor() { m_heavyFlavor = 0;}
      // OR'd, not assignment
      void setHeavyFlavor (unsigned int flavor) { m_heavyFlavor |= flavor; }
      static void setStandardCone (JetCone cone) { sm_jetCone = cone; }
      

      ///////////////////
      // Isa Functions //
      ///////////////////
      bool isUndefined   () const { return kNoCone    == m_jetCone; }
      bool isStandardCone() const { return sm_jetCone == m_jetCone; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // is a jet tagged
      int bTag (SVXTagType type = kDefaultTag) const;
      
      // probability jet tagged
      float tagProb (SVXTagType type = kDefaultTag) const;
      
      // set the tagging numbers
      void setBTag (SVXTagType type, int tag);

      // set the tagging numbers
      void setTagProb (SVXTagType type, float prob);

      // For whatever reason, the root people have decided not
      // to use the operator function, but rather this strange
      // compare function.  Okey dokey.
      Int_t Compare (const TObject *objPtr) const;

      // calculate all wanted quantities from given quantities
      virtual void calcQuantities();

      void setCorrectionScale (int type, float correctionScale);
      //{ m_correctionScale = correctionScale; }

      ///////////////////////////////
      // Momentum Member Functions //
      ///////////////////////////////
      // set the massive and massless vectors
      void setMassiveMom4Vec (double px, double py, double pz,
                              double energy, double Et);
      void setMasslessMom4Vec (double uncorrEt, double eta, double phi);

      // Our "real" four momentum functions
      // Centroid massless jets
      CLP4Vector _massless4Vec        (int type) const;
      // standard CDF massive jets
      CLP4Vector _massive4Vec         (int type) const;
      // Using Et, eta, and phi from massive jets
      CLP4Vector _massiveZeroMass4Vec (int type) const;
      // Using Pt, eta, and phi from massive jets
      CLP4Vector _massiveDropMass4Vec (int type) const;

      // default version, all parameters
      CLP4Vector mom4Vec (FourMomFuncPtr funcPtr, 
                          int type = kDefaultCorrection,
                          ScaleFactorType sftype = kNoScaleFactor) const;
      CLP3Vector mom3Vec (FourMomFuncPtr funcPtr, 
                          int type = kDefaultCorrection,
                          ScaleFactorType sftype = kNoScaleFactor) const;
      double     Pt      (FourMomFuncPtr funcPtr, 
                          int type = kDefaultCorrection,
                          ScaleFactorType sftype = kNoScaleFactor) const;
      double     Et      (FourMomFuncPtr funcPtr, 
                          int type = kDefaultCorrection,
                          ScaleFactorType sftype = kNoScaleFactor) const;
      double     energy  (FourMomFuncPtr funcPtr, 
                          int type = kDefaultCorrection,
                          ScaleFactorType sftype = kNoScaleFactor) const;
      // Default version, jet correction type only
      CLP4Vector mom4Vec (int type) const
      { return   mom4Vec (defaultFourMom, type); }
      CLP3Vector mom3Vec (int type) const
      { return   mom3Vec (defaultFourMom, type); }
      double     Pt      (int type) const
      { return   Pt      (defaultFourMom, type); }
      double     Et      (int type) const
      { return   Et      (defaultFourMom, type); }
      double     energy  (int type) const
      { return   energy  (defaultFourMom, type); }
      // TMT Scale factor version, jet correction type only
      CLP4Vector mom4Vec (int type, ScaleFactorType sftype) const
      { return   mom4Vec (defaultFourMom, type, sftype); }
      CLP3Vector mom3Vec (int type, ScaleFactorType sftype) const
      { return   mom3Vec (defaultFourMom, type, sftype); }
      double     Pt      (int type, ScaleFactorType sftype) const
      { return   Pt      (defaultFourMom, type, sftype); }
      double     Et      (int type, ScaleFactorType sftype) const
      { return   Et      (defaultFourMom, type, sftype); }
      double     energy  (int type, ScaleFactorType sftype) const
      { return   energy  (defaultFourMom, type, sftype); }
      // overwride CLPObject member funcitons
      CLP4Vector mom4Vec () const
      { return   mom4Vec (defaultFourMom, sm_corrType); }
      CLP3Vector mom3Vec () const
      { return   mom3Vec (defaultFourMom, sm_corrType); }
      double     Pt      () const
      { return   Pt      (defaultFourMom, sm_corrType); }
      double     Et      () const
      { return   Et      (defaultFourMom, sm_corrType); }
      double     energy  () const
      { return   energy  (defaultFourMom, sm_corrType); }

      // return a three vector representing the direction of the correction
      // of Et = CorrectedEt - UncorrectedEt
      CLP3Vector deltaCorrectTransVec (int type = 1) const;

      // return the correct scale factor
      double JCscale (int type = 1) const;

      // returns true if the jet is loose/tight b-tagged
      bool isBTagged (SVXTagType = kDefaultTag) const;

      // Top Mass Template correction factors
      // light jet multiplicative factor
      double lightJetMultFactor_TM() const;
      double lightJetRelativeWidth_TM() const;
      double lightJetAbsoluteWidth_TM() const;
      double bottomJetMultFactor_TM() const;
      double bottomJetRelativeWidth_TM() const;
      double bottomJetAbsoluteWidth_TM() const;

      // Absolute Resolution Level 7 Transverse energy corrections
      // Upper Width is used when increasing momentums
      // Lower Width is used when decrasing momemtums
      double jetMultFactor_AbsL7Trans() const;
      double jetAbsoluteUpperWidth_AbsL7Trans() const;
      double jetAbsoluteLowerWidth_AbsL7Trans() const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      // dumps a jet list to stdout
      static void outputJetList (const JetList &jetlist);

      // sets the default version of jet corrections
      static void setDefaultCorrectionType (int type);

      // set the default tag type
      static void setDefaultTagType (SVXTagType type);

      // returns the default tag type
      static SVXTagType defaultTagType() { return sm_defaultTagType; }
      
  private:
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      // three vector energy
      CLP4Vector   m_massiveMom4Vec;
      CLP4Vector   m_masslessMom4Vec;
      double       m_withWithoutMassFactor;
      float        m_etaDetector;
      float        m_correctionScale[kNumJetCorrections];
      float        m_massiveEt;
      float        m_emFraction;
      int          m_collectionType;
      int          m_bTag [kNumTagTypes];
      float        m_tagProb [kNumTagTypes];
      JetCone      m_jetCone;
      unsigned int m_heavyFlavor;

      // static data members
      static int        sm_corrType;
      static JetCone    sm_jetCone;
      static SVXTagType sm_defaultTagType;

      ClassDef (CLPJet, 4) // CLP Jet Class
};


#endif // CLPJet_H
