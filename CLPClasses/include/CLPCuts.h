// -*- C++ -*-

#if !defined(CLPCuts_H)
#define CLPCuts_H

#include <bitset>
#include <string>
#include "CLPClasses/CLPElectron.h"
#include "CLPClasses/CLPMuon.h"
#include "CLPClasses/CLPTrack.h"
#include "CLPClasses/CLPJet.h"

class CLPCuts
{
   public:
      
      //////////////////////
      // Public Constants //
      //////////////////////

      // Electron Cuts
      enum 
      {
         kNoCategoryDefined_elec,
         kEt_elec,
         kPt_elec,
         kEoverP_elec,
         kHadOverEm_elec,
         kLShrTrk_elec,
         kDeltaX_elec,
         kDeltaZ_elec,
         kStripChi2_elec,
         kAxialQual_elec,
         kStereoQual_elec,
         kConversion_elec,
         kEta_elec,
         kIsolation_elec,
         kPemFitTower_elec,
         kPemChi2_elec,
         kPesUandV_elec,
         kPemPesDistance_elec,
         kZ0_elec,
         kPhxMatch_elec,
         kSiHits_elec,
         kNumFlags_elec // MUST be last
      }; // Don't forget the ';'

      // Muon Cuts
      enum
      {
         kNoCategoryDefined_mu, // Not one of the categories we're looking for.
         kHadEnergy_mu,
         kEMenergy_mu,
         kD0_mu,
         kZ0_mu,
         kAxialQual_mu,
         kStereoQual_mu,
         kPt_mu,
         kCmnDx_mu,
         kIsolation_mu,
         kEmHadEnergy_mu,
         kCotExitRadius_mu,
         kNumFlags_mu
      }; // Don't forget the ';'

      // Track Cuts
      enum
      {
         kPt_trk = 0,
         kIsolation_trk,
         kAxialCOTHits_trk,
         kStereoCOTHits_trk,
         kCorrectedD0_trk,
         kSiliconHits_trk,
         kBeamConstraint_trk,
         kNumFlags_trk
      }; // Don't forget the ';'

      // Jet Cuts
      enum
      {
         kEt_jet,
         kEta_jet,
         kCollection_jet,
         kNumFlags_jet
      }; // Don't forget the ';'

      // typedefs
      typedef std::bitset< kNumFlags_elec > ElectronResult;
      typedef std::bitset< kNumFlags_mu > MuonResult;
      typedef std::bitset< kNumFlags_trk > TrackResult;
      typedef std::bitset< kNumFlags_jet > JetResult;      

      // static constants
      static const std::string kElecNames[kNumFlags_elec];
      static const std::string kMuonNames[kNumFlags_mu];
      static const std::string kTrackNames[kNumFlags_trk];
      static const std::string kJetNames[kNumFlags_jet];

      /////////////
      // friends //
      /////////////

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      // run cuts
      static CLPCuts::ElectronResult electronCuts(const CLPElectron &electron);
      static CLPCuts::MuonResult muonCuts(const CLPMuon &muon);
      static CLPCuts::TrackResult trackCuts(const CLPTrack &track);
      static CLPCuts::JetResult jetCuts(const CLPJet &jet);
      // decode Cuts
      static void decodeElectronCuts  (const ElectronResult &result);
      static void decodeMuonCuts  (const MuonResult &result);
      static void decodeTrackCuts  (const TrackResult &result);
      static void decodeJetCuts  (const JetResult &result);

      static bool deltaZ0Cut (const CLPChargedParticle &one, 
                              const CLPChargedParticle &two);

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

};


#endif // CLPCuts_H
