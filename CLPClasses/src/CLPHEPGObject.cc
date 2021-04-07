// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include "CLPClasses/CLPHEPGObject.h"
#include "CLPClasses/CLPFunctions.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

ClassImp (CLPHEPGObject)

CLPHEPGObject::CLPHEPGObject()
{
   setType (kHepgParticle);
   m_internalIndex = 0;
   m_parent = -1;
   m_firstDaughter = -1;
   m_numDaughters = -1;
   m_taggedToReconIndex = -1;
   m_taggedToTrackIndex = -1;
   m_numPossibleTags = 0;
   m_numPossibleTrackTags = 0;
   //m_momDiffTrack = -1;
   m_matchStatus = 0;
   m_reconType = kUndefined;
}

void
CLPHEPGObject::increaseInternal (int num)
{
   m_internalIndex += num;
   // don't reset parents and daughters if they are empty
   if (m_parent >= 0)
   {
      m_parent += num;
   }
   if (m_firstDaughter >= 0)
   {
      m_firstDaughter += num;
   }
}

double
CLPHEPGObject::mass() const
{
   return HEPGMom4Vec().mass();
}

bool
CLPHEPGObject::shouldBeFindable() const
{
   // first, make sure it is something that we try to tag
   if (isaTop() || isaWeakBoson())
   {
      // we don't try to tag these
      return false;
   }
   // Are we looking at a jet?
   // at this point isaQuark is equivalent to notaTopQuark
   if (isaQuark())
   {
      CLP3Vector vec = HEPGMom4Vec().threeVector();
      if ( (fabs( vec.eta() ) < 2.0) && (vec.pt() > 20 * GEV))
      {
         return true;
      } 
      else
      {
         return false;
      }
   }
   // Are we looking at a lepton
   if (isaLightLepton())
   {
      CLP3Vector vec = HEPGMom4Vec().threeVector();
      if ( (fabs( vec.eta() ) < 2.0) && (vec.pt() > 20 * GEV))
      {
         return true;
      } 
      else
      {
         return false;
      }
   }
   // if we're still here, something unexpected has happened
   cout << "WARNING: CLPHEPGObject::shouldBeFindable: Unexpected particle"
        << endl;
   return false;
}

void
CLPHEPGObject::setMatchStatusBit (int bit)
{
   m_matchStatus |= 1 << bit;
}

void
CLPHEPGObject::outputHepList (const CLPHEPGObject::HEPList& heplist,
                              ostream &o_stream)
{
   for (unsigned int loop = 0; loop < heplist.size(); ++loop)
   {
      o_stream << setw(2) << loop << ") " << heplist[loop] << endl;
   } // for loop
}

void
CLPHEPGObject::outputHepTCA (const TClonesArray* hepPtr,
                              ostream &o_stream)
{
   const TClonesArray &heplist = *hepPtr;
   int size = heplist.GetEntriesFast();
   for (int loop = 0; loop < size; ++loop)
   {
      CLPHEPGObject *ptr = (CLPHEPGObject*) heplist.At(loop);
      o_stream << setw(2) << loop << ") " << *ptr << endl;
   } // for loop
}

void
CLPHEPGObject::increaseAllInternal (CLPHEPGObject::HEPList& heplist,
                                    int num)
{
   for (unsigned int loop = 0; loop < heplist.size(); ++loop)
   {
      heplist[loop].increaseInternal(num);
   } // for loop
}

// friends
ostream& operator<< (ostream& o_stream, const CLPHEPGObject &rhs)
{
   o_stream << setw(12) << rhs.name() << " [" << setw(2) << rhs.index() 
            << ", " << setw(2) << rhs.internalIndex() << "] Mo: " << setw(2)
            << rhs.parent() << " 1st: " << setw(2) << rhs.firstDaughter()
            << " #: " << setw(2) << rhs.numDaughters()
            << " M: " <<  setw(2) << rhs.taggedToReconIndex()
            << " " << rhs.HEPGMom4Vec().threeVector();
   
   return o_stream;
} 
