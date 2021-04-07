// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h> // for fabs, etc
#include <algorithm>
#include "CLPClasses/CLPCuts.h"
#include "CLPClasses/CLPFunctions.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

const string CLPCuts::kElecNames[CLPCuts::kNumFlags_elec] = 
{
   "NoCategoryDefined",
   "Et",
   "Pt",
   "EoverP",
   "HadOverEm",
   "LShrTrk",
   "DeltaX",
   "DeltaZ",
   "StripChi2",
   "AxialQual",
   "StereoQual",
   "Conversion",
   "Eta",
   "Isolation",
   "PemFitTower",
   "PemChi2",
   "PesUandV",
   "PemPesDistance",
   "Z0",
   "PhxMatch",
   "SiHits"
};

const string CLPCuts::kMuonNames[CLPCuts::kNumFlags_mu] = 
{
   "kNoCategoryDefined",
   "HadEnergy",
   "EMenergy",
   "D0",
   "Z0",
   "AxialQual",
   "StereoQual",
   "Pt",
   "CmnDx",
   "Isolation",
   "EmHadEnergy",
   "CotExitRadius"
};

const string CLPCuts::kTrackNames[CLPCuts::kNumFlags_trk] = 
{
   "Pt",
   "Isolation",
   "AxialCOTHits",
   "StereoCOTHits",
   "CorrectedD0",
   "SiliconHits",
   "BeamConstraint"
};

const string CLPCuts::kJetNames[CLPCuts::kNumFlags_jet] = 
{
   "Et",
   "Eta",
   "Collection_type"
};


// Member Functions

void
CLPCuts::decodeElectronCuts(const CLPCuts::ElectronResult &result)
{
   if (result.none())
   {
      cout << "All electrons cuts are passed" << endl;
      return;
   }
   cout << "Failed cuts:";
   for (int loop = 0; loop < kNumFlags_elec; ++loop)
   {
      if (result.test(loop))
      {
         cout << " " << kElecNames[loop];
      }
   }
   cout << endl;
}

void
CLPCuts::decodeMuonCuts(const CLPCuts::MuonResult &result)
{
   if (result.none())
   {
      cout << "All muon cuts are passed" << endl;
      return;
   }
   cout << "Failed cuts:";
   for (int loop = 0; loop < kNumFlags_mu; ++loop)
   {
      if (result.test(loop))
      {
         cout << " " << kMuonNames[loop];
      }
   }
   cout << endl;
}

void
CLPCuts::decodeTrackCuts(const CLPCuts::TrackResult &result)
{
   if (result.none())
   {
      cout << "All track cuts are passed" << endl;
      return;
   }
   cout << "Failed cuts:";
   for (int loop = 0; loop < kNumFlags_trk; ++loop)
   {
      if (result.test(loop))
      {
         cout << " " << kTrackNames[loop];
      }
   }
   cout << endl;
}

void
CLPCuts::decodeJetCuts(const CLPCuts::JetResult &result)
{
   if (result.none())
   {
      cout << "All jets cuts are passed" << endl;
      return;
   }
   cout << "Failed cuts:";
   for (int loop = 0; loop < kNumFlags_jet; ++loop)
   {
      if (result.test(loop))
      {
         cout << " " << kJetNames[loop];
      }
   }
   cout << endl;
}

CLPCuts::ElectronResult
CLPCuts::electronCuts(const CLPElectron &electron)
{
   ElectronResult retval; // initialized to 0 by default
   // make sure we know what type of electron this is
   if ( !electron.isaTCE() && !electron.isaPEM() && ! electron.isaPHX() )
   {
      // we don't know what this is
      retval.set(kNoCategoryDefined_elec);
      return retval;
   }
   ///////////////////////////////////////////////
   // Since we are marking all cuts that FAIL,  //
   // we want to check the OPPOSITE of the cut. //
   // (i.e. '<' becomes '>=', etc.)             //
   ///////////////////////////////////////////////

   /////////////////////////////////
   // Tight Central Electron Cuts //
   /////////////////////////////////
   if ( electron.isaTCE())
   {
      if (electron.Et() <= 20. * GEV)
      {
         retval.set(kEt_elec);
      }
      if (electron.Pt() <= 10. * GEV)
      {
         retval.set(kPt_elec);
      }
      if ((electron.EoverP() > 2.) && (electron.trackPt() <= 50. * GEV))
      {
         retval.set(kEoverP_elec);
      }
      if (electron.hadOverEm() >= 
          0.055 + ( 0.00045 * electron.EMenergy() ) )
      {
         retval.set(kHadOverEm_elec);
      }         
      if ( fabs( electron.Z0() ) >= 60. * CM )
      {
         retval.set(kZ0_elec);
      }
      if (electron.LShrTrk() >= 0.2 )
      {
         retval.set(kLShrTrk_elec);
      }
      if ( (electron.charge() * electron.deltaX() <= -3.0 * CM ) ||
           (electron.charge() * electron.deltaX() >= 1.5 * CM) )
      {
         retval.set(kDeltaX_elec);
      }
      if ( fabs( electron.deltaZ() ) >= 3. * CM)
      {
         retval.set(kDeltaZ_elec);
      }
      if (electron.stripChi2() >= 10.)
      {
         retval.set(kStripChi2_elec);
      }
      if ( ( electron.axHits() < 5) ||
           ( electron.axSeg() < 3) )
      {
         retval.set(kAxialQual_elec);
      }
      if ( ( electron.stHits() < 5) ||
           ( electron.stSeg() < 2) )
      {
         retval.set(kStereoQual_elec);
      }
      if (abs(electron.conversion()) == 1 )
      {
         retval.set(kConversion_elec);
      }
      if (electron.isolation() >= 0.1 )
      {
         retval.set(kIsolation_elec);
      }
      
      return retval;
   }   // Tight Central Electron

   ////////////////////////////////////
   // Phoenix and Plug Electron Cuts //
   ////////////////////////////////////
   if (electron.Et() <= 20. )
   {
      retval.set(kEt_elec);
   }
   if ( ( fabs( electron.pes2dEta() ) <= 1.2 ) ||
        ( fabs( electron.pes2dEta() ) >= 3.0 ) )
   {
      retval.set(kEta_elec);
   }
   if (electron.isolation() >= 0.1 )
   {
      retval.set(kIsolation_elec);
   }
   if (electron.hadOverEm() >= 0.05 )
   {
      retval.set(kHadOverEm_elec);
   }
   if (electron.pem3x3FitTower() == 0 )
   {
      retval.set(kPemFitTower_elec);
   }
   if (electron.pem3x3Chi2() >= 10.) 
   {
      retval.set(kPemChi2_elec);
   }
   if ( ( electron.pes2d5x9U() <= 0.65 ) ||
        ( electron.pes2d5x9V() <= 0.65 ) )
   {
      retval.set(kPesUandV_elec);
   }
   if (electron.pesPemDistance() >= 3. * CM)
   {
      retval.set(kPemPesDistance_elec);
   }

   ///////////////////////
   // Phoenix only Cuts //
   ///////////////////////
   if (electron.isaPHX())
   {
      // special eta cut for PHX
      if ( fabs( electron.pes2dEta() ) >= 2.0 )
      {
         retval.set(kEta_elec);
      }
      if ( fabs( electron.Z0() ) >= 60. * CM )
      {
         retval.set(kZ0_elec);
      }
      if (electron.phxMatch() != 1 )
      {
         retval.set(kPhxMatch_elec);
      }
      if (electron.siHits() < 3 )
      {
         retval.set(kSiHits_elec);
      }
   }
   return retval;
}

CLPCuts::MuonResult
CLPCuts::muonCuts(const CLPMuon &muon)
{
   MuonResult retval; // initialized to 0 by default
   // make sure we know what type of muon this is
   if ( !muon.isaCMUP() && !muon.isaCMU()  &&
        !muon.isaCMP()  && !muon.isaCMX()  &&
        !muon.isaCMIO() )
   {
      // we don't know what this is
      retval.set(kNoCategoryDefined_mu);
      return retval;
   }
   ///////////////////////////////////////////////
   // Since we are marking all cuts that FAIL,  //
   // we want to check the OPPOSITE of the cut. //
   // (i.e. '<' becomes '>=', etc.)             //
   ///////////////////////////////////////////////
   // General cuts
   double mom = muon.mom3Vec().mag();
   if (mom > 100)
   {
      // P > 100
      if (muon.HadEnergy() >= 6. * GEV + 
          std::max(0.,  0.028 * (mom - 100) * GEV) )
      {
         retval.set(kHadEnergy_mu);
      }
      if (muon.EMenergy() >= 2. * GEV + 0.0115 * (mom - 100) * GEV )
      {
         retval.set(kEMenergy_mu);
      }
   }
   else
   {
      // P <= 100
      if (muon.HadEnergy() >= 6. * GEV)
      {
         retval.set(kHadEnergy_mu);
      }
      if (muon.EMenergy() >= 2. * GEV)
      {
         retval.set(kEMenergy_mu);
      }
   }
   if (muon.siHits() > 0)
   {
      // with silicon
      if (fabs(muon.D0()) >= 0.02 * CM)
      {
         retval.set(kD0_mu);
      }
   }
   else
   {
      // without silicon
      if (fabs(muon.D0()) >= 0.2 * CM)
      {
         retval.set(kD0_mu);
      }
   }
   if (muon.Pt() <= 20. * GEV)
   {
      retval.set(kPt_mu);
   }
   if (fabs(muon.Z0()) >= 60. * CM)
   {
      retval.set(kZ0_mu);
   }
   if ( ( muon.axHits() < 5) ||
        ( muon.axSeg() < 3) )
   {
      retval.set(kAxialQual_mu);
   }
   if ( ( muon.stHits() < 5) ||
        ( muon.stSeg() < 2) )
   {
      retval.set(kStereoQual_mu);
   }
   if (muon.isolation() >= 0.1)
   {
      retval.set(kIsolation_mu);
   }
   if ( muon.isaCMU() )
   {
      // CMU
      if (fabs(muon.cmuDx()) >= 3. * CM)
      {
         retval.set(kCmnDx_mu);
      }
   } else if ( muon.isaCMP() )
   {
      // CMP
      if (fabs(muon.cmpDx()) >= 5. * CM)
      {
         retval.set(kCmnDx_mu);
      }
   } else if ( muon.isaCMUP() )
   {
      // CMUP
      if ((fabs(muon.cmuDx()) >= 3. * CM) || 
          (fabs(muon.cmpDx()) >= 5. * CM))
      {
         retval.set(kCmnDx_mu);
      }
   } else if ( muon.isaCMX() )
   {
      // CMX
      if (fabs(muon.cmxDx()) >= 6. * CM)
      {
         retval.set(kCmnDx_mu);
      }
   } else if ( muon.isaCMIO() )
   {
      // CMIO
      if (muon.EMenergy() + muon.HadEnergy() <= 0.1)
      {
         retval.set(kEmHadEnergy_mu);
      }
   }
   // Check COT exit radius
   // Only check for CMUP or CMX
   if (muon.isaCMUP() || muon.isaCMX())
   {
      // | rho | > 140 to pass, so...
      if (fabs( muon.COTexitRadius() ) <= 140 * CM)
      {
         retval.set(kCotExitRadius_mu);
      }
   }
   return retval;
}

CLPCuts::TrackResult
CLPCuts::trackCuts(const CLPTrack &track)
{
   TrackResult retval; // initialized to 0 by default

   ///////////////////////////////////////////////
   // Since we are marking all cuts that FAIL,  //
   // we want to check the OPPOSITE of the cut. //
   // (i.e. '<' becomes '>=', etc.)             //
   ///////////////////////////////////////////////

   if (track.BCPt() < 20. * GEV && track.BCPt() > 0)
   {
      retval.set(kPt_trk);
   }
   if (track.BCPt() < 0 && track.Pt() < 20. * GEV)
   {
      retval.set(kPt_trk);
   } // trying to save the tracks where beam constrained failed
   if (track.axHits() < 24)
   {
      retval.set(kAxialCOTHits_trk);
   }
   if (track.stHits() < 20)
   {
      retval.set(kStereoCOTHits_trk);
   }
   if (fabs(track.corrD0()) > 2500 * MICRONS )
   {
      retval.set(kCorrectedD0_trk);
   }
   if (track.siHits() >= 3 && fabs(track.corrD0()) > 250 * MICRONS )
   {
      retval.set(kCorrectedD0_trk);
   }
   if (track.trackIsol() < CLPNamespace::kDefaultTrackIsolationCut)
   {
      retval.set(kIsolation_trk);
   }
   // if expected hits are greater than or equal to three, then there
   // should be at least three silicon hits.  The opposite of this is:
   // the number of expected hits is greater than or equal to three
   // and the number of actual hits is less than three.
   if (track.expectedSiHits() >= 3 && track.siHits() < 3)
   {
      retval.set(kSiliconHits_trk);
   }
   // Is this track successfully beam constrained?
   if (track.BCPt() < 0)
   {
      retval.set(kBeamConstraint_trk);
   }
   return retval;
}


CLPCuts::JetResult
CLPCuts::jetCuts(const CLPJet &jet)
{
   JetResult retval; // initialized to 0 by default
   ///////////////////////////////////////////////
   // Since we are marking all cuts that FAIL,  //
   // we want to check the OPPOSITE of the cut. //
   // (i.e. '<' becomes '>=', etc.)             //
   ///////////////////////////////////////////////
   if (jet.Et() < 15. * GEV)
   {
      retval.set(kEt_jet);
   }
   if (fabs( jet.etaDetector() ) >= 2.4)
   {
      retval.set(kEta_jet);
   }
   
   //if ((3 != jet.collectionType()) && isTopNtuple())
   //{
   //   retval.set(kCollection_jet);
   //}
   return retval;
}

bool
CLPCuts::deltaZ0Cut(const CLPChargedParticle &one, 
                    const CLPChargedParticle &two)
{
   if (CLPChargedParticle::deltaZ0(one, two) > 5 * CM)
   {
      return true;
   } else {
      return false;
   }
}
