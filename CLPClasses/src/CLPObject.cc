// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CLPClasses/CLPObject.h"
#include "CLPClasses/CLPFunctions.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

ClassImp (CLPObject)

CLPObject::CLPObject(CLPObject::ObjectType type)
{
   m_type = type;
   m_taggedToHEPGIndex = -1; // something less than 0
}

bool
CLPObject::isaChargedParticle() const
{
   if ((kChargedParticle == m_type) ||
       (kElectron == m_type) ||
       (kMuon == m_type) ||
       (kTrack == m_type) )
   {
      return true;
   } else {
      return false;
   }
}

bool
CLPObject::isaMuon() const
{
   if (kMuon == m_type)
   {
      return true;
   } else {
      return false;
   }
}

bool
CLPObject::isaElectron() const
{
   if (kElectron == m_type)
   {
      return true;
   } else {
      return false;
   }
}

bool
CLPObject::isaJet() const 
{
   if (kJet == m_type)
   {
      return true;
   } else {
      return false;
   }
}

bool
CLPObject::isaTrack() const 
{
   if (kTrack == m_type)
   {
      return true;
   } else {
      return false;
   }
}

bool
CLPObject::isBitSet (unsigned int bit) const
{
   if (bit > 63)
   {
      cerr << "CLPObject::isBitSet Error: bit "
           << bit << " > 63 " << endl;
      return false;
   }
   return (m_userBits & (1 << bit) );
}

void
CLPObject::calcQuantities()
{
}

double 
CLPObject::etaPhiDiff() const
{ 
   // first check to make sure we have a match
   if ( m_taggedToHEPGIndex < 0)
   {
      return -1.;
   }
   // make sure neither vector is empty
   if ((0 == m_mom3Vec.mag2()) || (0 == m_HEPGMom4Vec.mag2() ))
   {
      return -1.;
   }
   return m_mom3Vec.etaPhiDiff( m_HEPGMom4Vec ); 
}


// friends
ostream& operator<< (ostream& o_stream, const CLPObject &rhs)
{
   switch (rhs.m_type)
   {
      case CLPObject::kChargedParticle:
         o_stream << "ChargedParticle(" << setw(2) << rhs.index() << "): ";
         break;
      case CLPObject::kElectron:
         o_stream << "Electron(" << setw(2) << rhs.index() << "): ";
         break;
      case CLPObject::kMuon:
         o_stream << "Muon(" << setw(2) << rhs.index() << "): ";
         break;
      case CLPObject::kTrack:
         o_stream << "Track(" << setw(2) << rhs.index() << "): ";
         break;
      case CLPObject::kJet:
         o_stream << "Jet(" << setw(2) << rhs.index() << "): ";
         break;
      default:
         o_stream << "Undefined: ";
   } // switch type
   o_stream << rhs.mom3Vec() << " " << setw(4) << rhs.taggedToHEPGIndex();
   return o_stream;
} 
