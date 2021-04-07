// -*- C++ -*-

#if !defined(CLPMuon_H)
#define CLPMuon_H

#include <vector>
#include "CLPChargedParticle.h"

class CLPMuon : public CLPChargedParticle
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      typedef std::vector< CLPMuon > MuonList;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPMuon &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPMuon();

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      ////  Classification quantities ////
      int viewType() const { return m_viewType; }
      float cmuDx() const { return m_cmuDx; }
      float cmpDx() const { return m_cmpDx; }
      float cmxDx() const { return m_cmxDx; }
      // Pt before CLPMuon corrects it
      float uncorrPt() const { return m_uncorrPt; }
      float EMenergy() const { return m_EMenergy; }
      float HadEnergy() const { return m_HadEnergy; }
      float transverse() const  { return mom3Vec().transMag(); }
      // stub functions for Rochester
      bool hasBMUstub() const { return m_BMUstub; }
      bool hasCMUstub() const { return m_CMUstub; }
      bool hasCMPstub() const { return m_CMPstub; }
      bool hasCMXstub() const { return m_CMXstub; }

      // Gen 5
      int muonType() const { return m_muonType; }
      UChar_t inBluebeam() const { return m_inBluebeam; }
      UChar_t inMiniskirt() const { return m_inMiniskirt; }
      UChar_t inKeystone() const { return m_inKeystone; }
      Float_t cmpFidX() const { return m_cmpFidX; }
      Float_t cmuFidX() const { return m_cmuFidX; }
      Float_t cmxFidX() const { return m_cmxFidX; }
      Float_t cmpFidZ() const { return m_cmpFidZ; }
      Float_t cmuFidZ() const { return m_cmuFidZ; }
      Float_t cmxFidZ() const { return m_cmxFidZ; }


      // fiducial variables for Gen 5 Top ntuple
      bool hasCmu() const { return (muonType() >> 0) & 1; }
      bool hasCmp() const { return (muonType() >> 1) & 1; }
      bool hasCmx() const { return (muonType() >> 2) & 1; }
      bool hasBmu() const { return (muonType() >> 3) & 1; }


      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setViewType(int viewType) { m_viewType = viewType; }
      void setCmuDx(float cmuDx) { m_cmuDx = cmuDx; }
      void setCmpDx(float cmpDx) { m_cmpDx = cmpDx; }
      void setCmxDx(float cmxDx) { m_cmxDx = cmxDx; }
      void setUncorrPt(float uncorrPt) { m_uncorrPt = uncorrPt; }
      void setEMenergy(float EMenergy) { m_EMenergy = EMenergy; }
      void setHadEnergy(float HadEnergy) { m_HadEnergy = HadEnergy; }
      // Gen5
      void setMuonType(int muonType) { m_muonType = muonType; }
      void setInBluebeam(UChar_t inBluebeam) { m_inBluebeam = inBluebeam; }
      void setInMiniskirt(UChar_t inMiniskirt) { m_inMiniskirt = inMiniskirt; }
      void setInKeystone(UChar_t inKeystone) { m_inKeystone = inKeystone; }
      void setCmpFidX(Float_t cmpFidX) { m_cmpFidX = cmpFidX; }
      void setCmuFidX(Float_t cmuFidX) { m_cmuFidX = cmuFidX; }
      void setCmxFidX(Float_t cmxFidX) { m_cmxFidX = cmxFidX; }
      void setCmpFidZ(Float_t cmpFidZ) { m_cmpFidZ = cmpFidZ; }
      void setCmuFidZ(Float_t cmuFidZ) { m_cmuFidZ = cmuFidZ; }
      void setCmxFidZ(Float_t cmxFidZ) { m_cmxFidZ = cmxFidZ; }

      // Rochester NTuple
      void setBMUstub(bool BMUstub) { m_BMUstub = BMUstub; }
      void setCMUstub(bool CMUstub) { m_CMUstub = CMUstub; }
      void setCMPstub(bool CMPstub) { m_CMPstub = CMPstub; }
      void setCMXstub(bool CMXstub) { m_CMXstub = CMXstub; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // calculate derived quantities from given quantities
      void calcQuantities();

      // uncorrected three momemntum
      CLP3Vector uncorrMom3Vec() const;


      // return a three vector representing the direction of the correction
      // of Pt = CorrectedPt - UncorrectedPt
      CLP3Vector deltaCorrectTransVec() const;

      // COT exit radius for CMX and CMP muons
      double COTexitRadius() const;

      // is this a stubless muon
      bool isStubless() const;

      // more ISA functions
      bool isCmu() const;
      bool isCmp() const;
      bool isCmx() const;
      bool isBmu() const;
      

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

  private:
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      // Classification quantities
      int m_viewType;
      float m_cmuDx;
      float m_cmpDx;
      float m_cmxDx;
      float m_uncorrPt; // Pt before CLPMuon corrects it
      float m_EMenergy;
      float m_HadEnergy;
      bool m_BMUstub;
      bool m_CMUstub;
      bool m_CMPstub;
      bool m_CMXstub;
      // gen 5 quantities
      int m_muonType; // I erroneously used 'collectionType'
      UChar_t m_inBluebeam;
      UChar_t m_inMiniskirt;
      UChar_t m_inKeystone;
      Float_t m_cmpFidX;
      Float_t m_cmuFidX;
      Float_t m_cmxFidX;
      Float_t m_cmpFidZ;
      Float_t m_cmuFidZ;
      Float_t m_cmxFidZ;
      ClassDef (CLPMuon, 1); // CLP Muon Class
};


#endif // CLPMuon_H
