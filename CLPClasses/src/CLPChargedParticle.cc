// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CLPClasses/CLPChargedParticle.h"
#include "CLPClasses/CLPFunctions.h"
#include "CLPClasses/CLPTrack.h"


using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

ClassImp (CLPChargedParticle)

const string 
CLPChargedParticle::kChargedParticleName[CLPChargedParticle::kNumTypes] =
{
   "None",
   "UndefinedElectron",
   "TightCentralElectron",
   "PhoenixElectron",
   "PlugElectron",
   "UndefinedMuon",
   "CMUP",
   "CMU",
   "CMP",
   "CMX",
   "CMIO",
   "UndefinedTrack"
   "TrackLepton"
};

const string 
CLPChargedParticle::kShortChargedParticleName[CLPChargedParticle::kNumTypes] =
{
   "None",
   "UndefE",
   "TCE",
   "PHX",
   "PEM",
   "UndefM",
   "CMUP",
   "CMU",
   "CMP",
   "CMX",
   "CMIO",
   "UndefT",
   "TRK"
};


CLPChargedParticle::CLPChargedParticle() : 
   CLPObject(CLPObject::kChargedParticle), 
   m_cpType (CLPChargedParticle::kNone),
   m_sign(0),
   m_cutCode(0),    // 0 = none
   m_passesCuts(0), // 0 = passes nothing
   m_eta(kInit),
   m_phi(kInit), 
   m_transMom(0)
{
}

void
CLPChargedParticle::calcQuantities()
{
   if (isaMuon())
   {
      // Muon
      // Since Pt was corrected, we need to recalculated
      setMom3Vec( CLP3Vector::fromEtaPhiTrans(m_eta, m_phi, m_transMom) );
   } else {
      // The electron values are good to go.  Just use the originals
      setMom3Vec( CLP3Vector(m_Px, m_Py, m_Pz) );
   }
   // get the base class 
   CLPObject::calcQuantities();
}

bool
CLPChargedParticle::isaTightLepton() const
{
   // tight leptons are:
   // TCE, PHX, CMUP, CMX

   if ( ( isa( kTightCentralElectron ) ) ||
      //( isa( kPhoenixElectron      ) ) ||
        ( isa( kCMUP                 ) ) ||
        ( isa( kCMX                  ) ) ) 
   {
      return true;
   } else {
      return false;
   }
}

bool
CLPChargedParticle::isIsolated(double cutValue) const
{
   // default is CLPNamespace::kDefaultIsoationCut
   if (isaTrack())
   {
      cerr << "ERROR: CLPChargedParticle::isIsolated, you shouldn't be calling this function for CLPTracks" 
           << endl;
      return ((CLPTrack*) this)->isTrackIsolated();
   } //changing the function call for Tracks
   
   if (isolation() < cutValue)
   {
      return true;
   } else {
      return false;
   }
}

string
CLPChargedParticle::typeString() const
{
   return kChargedParticleName[m_cpType];
   //switch (m_cpType) {
   //   case kNone:
   //   return "None";
   //   case kUndefinedElectron:
   //   return "UndefinedElectron";
   //   case kTightCentralElectron:
   //   return "TightCentralElectron";
   //   case kPhoenixElectron:
   //   return "PhoenixElectron";
   //   case kPlugElectron:
   //   return "PlugElectron";
   //   case kUndefinedMuon:
   //   return "UndefinedMuon";
   //   case kCMUP:
   //   return "CMUP";
   //   case kCMU:
   //   return "CMU";
   //   case kCMP:
   //   return "CMP";
   //   case kCMX:
   //   return "CMX";
   //   case kCMIO:
   //   return "CMIO";
   //   default:
   //   return "Error!";
   //}
}

string
CLPChargedParticle::shortTypeString() const
{
   return kShortChargedParticleName[m_cpType];
}

// friends
ostream& operator<< (ostream& o_stream, const CLPChargedParticle &rhs)
{
   //o_stream << rhs.index() << ") " << rhs.typeString()
   //         << " " << rhs.mom3Vec() 
   //         // << " is Isolated " << rhs.isIsolated()
   //         << " isolation " << rhs.isolation();
   o_stream << (CLPObject) rhs;
   o_stream << "  ID =" << setw(3) << rhs.trackIdOrig();
   if (rhs.charge() > 0) {
      o_stream << " +";
   } else if (rhs.charge() < 0) {
      o_stream << " -";
   } else {
      //no charge
      o_stream << " 0";
   } //add the charge to the printout
   return o_stream;
} 
