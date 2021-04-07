// -*- C++ -*-

#if !defined(CLPTrack_H)
#define CLPTrack_H

#include <vector>
#include "CLPChargedParticle.h"
#include "CLPElectron.h"
#include "CLPMuon.h"
#include "CLPNamespace.h"

class CLPTrack : public CLPChargedParticle
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      typedef std::vector< CLPTrack > TrackList;

      enum MomentumType
      {
         kSavedMom,
         kAsMuonMom,
         kAsElectronMom
      };

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPTrack &rhs);


      //////////////////////////
      // .  .      ._         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPTrack();

      ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////

      // corrected D0 value for cuts
      float corrD0() const { return m_corrD0; }
      int collectionType() const { return m_collectionType; }
      float trackIsol() const { return m_trackIsol; }
      bool matchElectron() const { return m_matchElectron; }
      bool matchMuon() const { return m_matchMuon; }
      bool isMatchedToElectron() const { return m_matchElectron; }
      bool isMatchedToMuon() const { return m_matchMuon; }
      int expectedSiHits() const { return m_expectedSiHits; }
      float BCTransMom() const { return m_BCTransMom; }
      float BCPt() const { return m_BCTransMom; } //just different accessor
      float calEmTower() const { return m_calEmTower; }
      float calHadTower() const { return m_calHadTower; }
      MomentumType momentumType() const { return m_momentumType; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////

      void setCorrD0 (float corrD0) { m_corrD0 = corrD0; }
      void setCollectionType(int collectionType) 
      { m_collectionType = collectionType; }
      void setTrackIsol (float trackIsol) { m_trackIsol = trackIsol; }
      void setMatchElectron (bool matchElectron = true) 
      { m_matchElectron = matchElectron; }
      void setMatchMuon (bool matchMuon = true) 
      { m_matchMuon = matchMuon; }
      void setExpectedSiHits ( int expectedSiHits ) 
      { m_expectedSiHits = expectedSiHits; }
      void setBCTransMom ( float BCTransMom ) { m_BCTransMom = BCTransMom; } 
      void setCalEmTower ( float calEmTower ) 
      { m_calEmTower = calEmTower; }
      void setCalHadTower ( float calHadTower ) 
      { m_calHadTower = calHadTower; }
      void setMomentumType( MomentumType momentumType )
      { m_momentumType = momentumType; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      void calcQuantities();
      bool isTrackIsolated(double cutValue = 
                           CLPNamespace::kDefaultTrackIsolationCut) const;
      bool BCError() { return BCPt() < 0; }

      bool rescaleMomAs(const MomentumType momentumType = kSavedMom);

      // overloaded functions from CLPObject
      // returns 3 momentum vector
      CLP3Vector mom3Vec() const;
      CLP3Vector mom3Vec(const MomentumType momentumType) const;
      // Return 4 momentum vector
      CLP4Vector mom4Vec() const;
      CLP4Vector mom4Vec(const MomentumType momentumType) const;
      // double Pt(const MomentumType momentumType = kSavedMom) const;
      // double Et(const MomentumType momentumType = kSavedMom) const;



      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

      static void matchElectron(TrackList &trackList, 
                                const CLPElectron::ElectronList &elecList);
      static void matchMuon(TrackList &trackList, 
                            const CLPMuon::MuonList &muonList);

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

      float m_corrD0;
      int m_collectionType;
      float m_trackIsol;
      bool m_matchElectron;
      bool m_matchMuon;
      int m_expectedSiHits;
      float m_BCTransMom;
      float m_calEmTower;
      float m_calHadTower;
      MomentumType m_momentumType;
      ClassDef (CLPTrack, 1); // CLP Track Class


};


#endif // CLPTrack_H
