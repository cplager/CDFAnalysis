// -*- C++ -*-

#if !defined(CLPHEPGObject_H)
#define CLPHEPGObject_H

#include "TClonesArray.h"
#include "CLPClasses/CLPObject.h"
#include "CLPClasses/CLP4Vector.h"
#include "CLPClasses/HEPGNames.h"
#include <vector>
#include <iostream>

class CLPHEPGObject : public CLPObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      // match status enums
      enum
      {
         kIsMatched,
         kIsLeptonMatched,
         kIsTrackMatched,
         kChargesMatch,
         kTrackLeptonMatch
      };

      // typedefs
      typedef std::vector< CLPHEPGObject > HEPList;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPHEPGObject &rhs);

      //////////////////////////
      // .  .      ._         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPHEPGObject();

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      int internalIndex() const { return m_internalIndex; }
      int HEPGtype() const { return m_HEPGtype; }
      //CLP4Vector mom4Vec() const { return HEPGMom4Vec(); }
      //CLP3Vector mom3Vec() const { return HEPGMom4Vec().threeVector(); }
      ////  sign (charge) of particle ////
      float sign() const   { return m_sign; }
      float charge() const { return m_sign; }
      // common three vector accessors
      double pt() const  { return mom3Vec().pt();  }
      double eta() const { return mom3Vec().eta(); }
      int parent() const { return m_parent; }
      int firstDaughter() const { return m_firstDaughter; }
      int numDaughters() const { return m_numDaughters; }
      int taggedToReconIndex() const { return m_taggedToReconIndex; }
      int taggedToTrackIndex() const { return m_taggedToTrackIndex; }
      ObjectType reconType() const { return m_reconType; }
      int numPossibleTags() const { return m_numPossibleTags; }
      int numPossibleTrackTags() const { return m_numPossibleTrackTags; }
      //float momDiffTrack() const { return m_momDiffTrack; }
      virtual CLP3Vector track3Vec() const { return m_track3Vec; }
      // Return 4 trackentum vector
      virtual CLP4Vector track4Vec() const { return CLP4Vector(m_track3Vec); }
      //int stdhep() const { return m_stdhep; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setInternalIndex(int internalIndex) 
      { m_internalIndex = internalIndex; }
      void setHEPGtype(int HEPGtype) { m_HEPGtype = HEPGtype; }
      void setSign  (float sign) { m_sign = sign; }
      void setCharge(float sign) { m_sign = sign; }
      void setParent(int parent) { m_parent = parent; }
      void setFirstDaughter(int firstDaughter) 
      { m_firstDaughter = firstDaughter; }
      void setNumDaughters(int numDaughters) { m_numDaughters = numDaughters; }
      void setTaggedToReconIndex (int taggedToReconIndex) 
      { m_taggedToReconIndex = taggedToReconIndex; }
      void setTaggedToTrackIndex (int taggedToTrackIndex) 
      { m_taggedToTrackIndex = taggedToTrackIndex; }
      void setReconType (ObjectType reconType) { m_reconType = reconType; }
      void setNumPossibleTags (int numPossibleTags) 
      { m_numPossibleTags = numPossibleTags; }
      void setNumPossibleTrackTags (int numPossibleTrackTags) 
      { m_numPossibleTrackTags = numPossibleTrackTags; }
      //void setMomDiffTrack (int momDiffTrack) { m_momDiffTrack = momDiffTrack; }
      void setTrack3Vec (const CLP3Vector& vec) { m_track3Vec = vec; }
      void setTrack3Vec (double px, double py, double pz)
      { m_track3Vec = CLP3Vector (px, py, pz); }
      //void setStdhep(int stdhep) { m_stdhep = stdhep; }

      
      std::string name() const { return HEPGNames::name( m_HEPGtype ); }

      ///////////////////
      // ISA Functions //
      ///////////////////
      // quarks
      bool isaUp () const 
      { return HEPGNames::isaUp       ( m_HEPGtype ); }      
      bool isaDown   () const 
      { return HEPGNames::isaDown     ( m_HEPGtype ); }      
      bool isaCharm   () const 
      { return HEPGNames::isaCharm    ( m_HEPGtype ); }      
      bool isaStrange () const 
      { return HEPGNames::isaStrange  ( m_HEPGtype ); }      
      bool isaTop     () const 
      { return HEPGNames::isaTop      ( m_HEPGtype ); }      
      bool isaBottom   () const 
      { return HEPGNames::isaBottom   ( m_HEPGtype ); }
      bool isaLightQuark () const
      { return isaUp() || isaDown() || isaCharm() || isaStrange();}
      bool isaQuark    () const    
      { return isaLightQuark() || isaTop() || isaBottom(); }
      // true if a quark but not a top quark
      bool notaTopQuark () const
      { return isaQuark() && ! isaTop(); }
      // weak bosons
      bool isaZ       () const 
      { return HEPGNames::isaZ        ( m_HEPGtype ); }      
      bool isaW       () const 
      { return HEPGNames::isaW        ( m_HEPGtype ); }
      bool isaWeakBoson() const
      { return isaZ() || isaW(); }
      // leptons
      bool isaElectron() const 
      { return HEPGNames::isaElectron   ( m_HEPGtype ); }      
      bool isaElectronNu() const 
      { return HEPGNames::isaElectronNu ( m_HEPGtype ); }      
      bool isaMuon    () const 
      { return HEPGNames::isaMuon       ( m_HEPGtype ); } 
      bool isaMuonNu  () const          
      { return HEPGNames::isaMuonNu     ( m_HEPGtype ); } 
      bool isaTau    () const 
      { return HEPGNames::isaTau       ( m_HEPGtype ); } 
      bool isaTauNu  () const          
      { return HEPGNames::isaTauNu     ( m_HEPGtype ); } 
      bool isaLightLepton () const
      { return isaElectron() || isaMuon(); }
      bool isaLightLeptonNu () const
      { return isaElectronNu() || isaMuonNu(); }
      bool isaAnyLightLepton() const
      { return isaLightLepton() || isaLightLeptonNu(); }
      bool isaLepton () const
      { return isaElectron() || isaMuon() || isaTau(); }
      bool isaLeptonNu () const
      { return isaElectronNu() || isaMuonNu() || isaTauNu(); }
      bool isaAnyLepton() const
      { return isaLepton() || isaLeptonNu(); }
      
      // true if we will try to match it to a reconstructed particle
      bool isReconstructable() const
      { return notaTopQuark() || isaLightLepton(); }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // increases all internal counter by 'num'
      // (internalIndex, parent, firstDaughter).  Note that it
      // doesn't increase any value that is negative
      void increaseInternal (int num);

      // returns invariant mass 
      double mass() const;

      // returns bool as to whether object passes simple
      // fiducial cuts
      bool shouldBeFindable() const;

      // sets bits using match status enums (above)
      void setMatchStatusBit (int bit);
      void setMatchStatus (int status) { m_matchStatus = status; }

      
      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      static void outputHepList (const HEPList& heplist, 
                                 ostream &o_stream = std::cout);
      static void outputHepTCA (const TClonesArray* hepPtr,
                                 ostream &o_stream = std::cout);
      static void increaseAllInternal (HEPList& heplist, int num);


  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////

  private:
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      int m_internalIndex;
      int m_HEPGtype;
      float m_sign;
      //int m_stdhep;
      int m_parent;
      int m_firstDaughter;
      int m_numDaughters;
      int m_taggedToReconIndex;
      int m_taggedToTrackIndex;
      int m_numPossibleTags;
      int m_numPossibleTrackTags;
      //float m_momDiffTrack;
      int m_matchStatus;     
      ObjectType m_reconType;
      CLP3Vector m_track3Vec;
      ClassDef (CLPHEPGObject, 3) // CLP HEPG Object class
};


#endif // CLPHEPGObject_H
