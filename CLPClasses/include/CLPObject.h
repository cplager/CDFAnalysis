// -*- C++ -*-

#if !defined(CLPObject_H)
#define CLPObject_H

#include "TSystem.h"
#include "CLP4Vector.h" // include CLP3Vector

class CLPObject : public TObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      enum ObjectType 
      {
         kUndefined,
         kChargedParticle,
         kElectron,
         kMuon,
         kJet,
         kTrack,
         kHepgParticle,
      };

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPObject &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPObject(ObjectType type = kUndefined);

       ////////////////
      // One Liners //
      ////////////////

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setType (ObjectType type) { m_type = type; }
      void setIndex (int index) { m_index = index; }
      void setUserBits (Long64_t userBits) { m_userBits = userBits; }
      virtual void setMom3Vec (const CLP3Vector& vec) { m_mom3Vec = vec; }
      virtual void setMom3Vec (double px, double py, double pz)
      { m_mom3Vec = CLP3Vector (px, py, pz); }
      void setHEPGMom4Vec (const CLP4Vector& vec) { m_HEPGMom4Vec = vec; }
      void setHEPGMom4Vec (double px, double py, double pz, double e)
      { m_HEPGMom4Vec = CLP4Vector (px, py, pz, e); }
      void setTaggedToHEPGIndex (int taggedToHEPGIndex) 
      { m_taggedToHEPGIndex = taggedToHEPGIndex; }


      //////////////////////
      // Access Functions //
      //////////////////////
      ObjectType type() const { return m_type; }
      int index() const { return m_index; }
      Long64_t userBits() const { return m_userBits; }
      // returns 3 momentum vector
      virtual CLP3Vector mom3Vec() const { return m_mom3Vec; }
      // Return 4 momentum vector
      virtual CLP4Vector mom4Vec() const { return CLP4Vector(m_mom3Vec); }
      double eta() const { return this->mom3Vec().eta(); }
      double phi() const { return this->mom3Vec().phi(); }
      double Px() const  { return this->mom3Vec().Px(); }
      double Py() const  { return this->mom3Vec().Py(); }
      double Pz() const  { return this->mom3Vec().Pz(); }
      virtual double Pt() const { return m_mom3Vec.transMag(); }
      virtual double Et() const { return m_mom3Vec.transMag(); }
      CLP4Vector HEPGMom4Vec() const { return m_HEPGMom4Vec; }
      int taggedToHEPGIndex() const { return m_taggedToHEPGIndex; }
      bool isTagged() const { return -1 != m_taggedToHEPGIndex; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      bool isaChargedParticle() const;
      bool isaJet() const;
      bool isaMuon() const;
      bool isaElectron() const;
      bool isaTrack() const;

      // is user bit N set?
      bool isBitSet (unsigned int bit) const;


      // calculate all wanted quantities from given quantities
      virtual void calcQuantities();

      // eta phi difference between tagged HEPG particle
      // and reconstructed particle
      double etaPhiDiff() const;


      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////
      ObjectType m_type;
      int m_index;
      Long64_t m_userBits;
      // three vector momemtum
      CLP3Vector m_mom3Vec;
      // HEPG four momentum
      CLP4Vector m_HEPGMom4Vec;
      int m_taggedToHEPGIndex;

      ClassDef (CLPObject, 1) // CLP Object class
};


#endif // CLPObject_H
