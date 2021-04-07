// -*- C++ -*-

#if !defined(CLPElectron_H)
#define CLPElectron_H


#include <vector>
#include "CLPChargedParticle.h"

class CLPElectron : public CLPChargedParticle
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      // static constants
      static const double kAveragePesDistance;

      // Typedefs
      typedef std::vector< CLPElectron > ElectronList;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPElectron &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPElectron();

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      // EM calorimeter energy      
      float EMenergy() const { return m_EMenergy; }
      // transverse energy
      float transEnergy() const { return m_transEnergy; }
      ////  Classification quantities ////
      int region() const { return m_region; }
      int collectionType() const { return m_collectionType; }
      int conversion() const { return m_conversion; }
      ////  shower "shape" quantities ////
      // lateral sharing
      float LShrTrk() const { return m_LShrTrk; }
      // Energy over momentum
      float EoverP() const { return m_EoverP; }
      // Hadronic energy over EM energy
      float hadOverEm() const { return m_hadOverEm; }
      float deltaX() const { return m_deltaX; }
      float deltaZ() const { return m_deltaZ; }
      ////  PEM quantities ////
      int pem3x3FitTower() const { return m_pem3x3FitTower; }
      float pem3x3DetEta() const { return m_pem3x3DetEta; }
      float pem3x3Phi() const { return m_pem3x3Phi; }
      float pem3x3Chi2() const { return m_pem3x3Chi2; }
      ////  PES quantities ////
      float pes2dEta() const { return m_pes2dEta; }
      float pes2dPhi() const { return m_pes2dPhi; }
      float pes2d5x9U() const { return m_pes2d5x9U; }
      float pes2d5x9V() const { return m_pes2d5x9V; }
      float pes2dX() const { return m_pes2dX; }
      float pes2dY() const { return m_pes2dY; }
      float stripChi2() const { return m_stripChi2; }
      ////  Phoenix quantities ////
      int phxMatch() const { return m_phxMatch; }
      int phxCharge() const { return m_phxCharge; }
      int phxSiHits() const { return m_phxSiHits; }
      float phxTrkZ0() const { return m_phxTrkZ0; }
      ////  Old Quantities ////
      float oldEta() const { return m_oldEta; }
      float oldPhi() const { return m_oldPhi; }
      ////  Is this PEM a PHX duplicate? ////
      bool phxDuplicate() const { return m_phxDuplicate; }
      //// Resutls of cuts ////
      //CLPCuts::ElectronResult result() const { return m_result; }


      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setEMenergy(float EMenergy) { m_EMenergy = EMenergy; }
      void setEt(float transEnergy) { m_transEnergy = transEnergy; }
      void setTransEnergy(float transEnergy) { m_transEnergy = transEnergy; }
      void setRegion(int region) { m_region = region; }
      void setCollectionType(int collectionType) 
      { m_collectionType = collectionType; }
      void setConversion(int conversion) { m_conversion = conversion; }
      void setLShrTrk(float LShrTrk) { m_LShrTrk = LShrTrk; }
      void setEoverP(float EoverP) { m_EoverP = EoverP; }
      void setHadOverEm(float hadOverEm) { m_hadOverEm = hadOverEm; }
      void setDeltaX(float deltaX) { m_deltaX = deltaX; }
      void setDeltaZ(float deltaZ) { m_deltaZ = deltaZ; }
      void setPem3x3FitTower(int pem3x3FitTower) 
      { m_pem3x3FitTower = pem3x3FitTower; }
      void setPem3x3DetEta(float pem3x3DetEta) 
      { m_pem3x3DetEta = pem3x3DetEta; }
      void setPem3x3Phi(float pem3x3Phi) 
      { m_pem3x3Phi = pem3x3Phi; }
      void setPem3x3Chi2(float pem3x3Chi2) { m_pem3x3Chi2 = pem3x3Chi2; }
      void setPes2dEta(float pes2dEta) { m_pes2dEta = pes2dEta; }
      void setPes2dPhi(float pes2dPhi) { m_pes2dPhi = pes2dPhi; }
      void setPes2d5x9U(float pes2d5x9U) { m_pes2d5x9U = pes2d5x9U; }
      void setPes2d5x9V(float pes2d5x9V) { m_pes2d5x9V = pes2d5x9V; }
      void setPes2dX(float pes2dX) { m_pes2dX = pes2dX; }
      void setPes2dY(float pes2dY) { m_pes2dY = pes2dY; }
      void setStripChi2(float stripChi2) { m_stripChi2 = stripChi2; }
      void setPhxTrkZ0(float phxTrkZ0) { m_phxTrkZ0 = phxTrkZ0; }
      void setPhxMatch(int phxMatch) { m_phxMatch = phxMatch; }
      void setPhxCharge(int phxCharge) { m_phxCharge = phxCharge; }
      void setPhxSiHits(int phxSiHits) { m_phxSiHits = phxSiHits; }
      void setOldEta(float eta) { m_oldEta = eta; }
      void setOldPhi(float phi) { m_oldPhi = phi; }
      void setPhxDuplicate(bool phxDuplicate) 
      { m_phxDuplicate = phxDuplicate; }
      //void setResult(CLPCuts::ElectronResult &result) const 
      //{ m_result = result; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // calculate derived quantities from given quantities
      void calcQuantities();
      bool couldBeaPHX() const;
      double pesPemDistance() const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

  protected:
      ///////////////////////
      // Protected Constants //
      ///////////////////////

      /////////////////////////
      // Protected Member Data //
      /////////////////////////
      float m_EMenergy; // EM calorimeter energy      
      float m_transEnergy; // transverse energy
      // Classification quantities
      int m_region;
      int m_collectionType;
      int m_conversion;
      // shower "shape" quantities
      float m_LShrTrk; // lateral sharing
      float m_EoverP; // Energy over momentum
      float m_hadOverEm; // Hadronic energy over EM energy
      float m_deltaX;
      float m_deltaZ;
      // PEM quantities
      int m_pem3x3FitTower;
      float m_pem3x3DetEta;
      float m_pem3x3Phi;
      float m_pem3x3Chi2;
      // PES quantities
      float m_pes2dEta;
      float m_pes2dPhi;
      float m_pes2d5x9U;
      float m_pes2d5x9V;
      float m_pes2dX;
      float m_pes2dY;
      float m_stripChi2;
      // Phoenix quantities
      int m_phxMatch;
      int m_phxCharge;
      int m_phxSiHits;
      float m_phxTrkZ0;
      // Old Quantities
      float m_oldEta;
      float m_oldPhi;
      // Is PEM a copy of a PHX
      bool m_phxDuplicate;
      // results of cuts
      //mutable CLPCuts::ElectronResult m_result;
      
      ClassDef (CLPElectron, 1) // CLP Electron Class
};


#endif // CLPElectron_H

