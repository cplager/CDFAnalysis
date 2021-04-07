// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <assert.h>
#include "CLPClasses/dout.h"
#include "CLPClasses/CLPHEPGCont.h"
#include "CLPClasses/CLPFunctions.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;


CLPHEPGCont::CLPHEPGCont()
{
   m_numObjects = 0;
   m_hepgTcaPtr = new TClonesArray(kHepgObjectClassName, 10);
   m_hepgTcaPtr->BypassStreamer(true);
}

CLPHEPGCont::CLPHEPGCont(const CLPHEPGCont& rhs)
{
   m_hepgTcaPtr = new TClonesArray(kHepgObjectClassName, 10);
   m_hepgTcaPtr->BypassStreamer(true);
   *this = rhs;
}

CLPHEPGCont &
CLPHEPGCont::operator= (const CLPHEPGCont& rhs)
{
   // first copy the easy things
   m_numObjects = rhs.m_numObjects;
   // now copy the TClonesArrays
   copyTCA (rhs.m_hepgTcaPtr,  m_hepgTcaPtr);
   return *this;
}

CLPHEPGObject 
&CLPHEPGCont::operator() (int index)
{
   // make sure that what we're asking for isn't too big
   if (index >= size() || index < 0)
   {
      // now you've done it
      cerr << "ERROR:: CLPHEPGCont::operator(): index "
           << index << " is bigger than size() " 
           << size() << endl;
   }
   CLPHEPGObject *ptr = (CLPHEPGObject *) m_hepgTcaPtr->At(index);
   return *ptr;
}

CLPHEPGObject 
CLPHEPGCont::operator() (int index) const
{
   // make sure that what we're asking for isn't too big
   if (index >= size() || index < 0)
   {
      // now you've done it
      cerr << "ERROR:: CLPHEPGCont::operator(): index "
           << index << " is bigger than size() " 
           << size() << endl;
   }
   CLPHEPGObject *ptr = (CLPHEPGObject *) m_hepgTcaPtr->At(index);
   return *ptr;
}

CLPHEPGCont::~CLPHEPGCont()
{
   clear();
   delete m_hepgTcaPtr;  m_hepgTcaPtr  = (TClonesArray *) 0;
}

void
CLPHEPGCont::clear()
{
   m_hepgTcaPtr->Clear("C");
   m_numObjects = 0;
}

void
CLPHEPGCont::fill (CLPHEPGObject::HEPList &heplist)
{
   clear();
   for (unsigned int loop = 0; loop < heplist.size(); ++loop)
   {
      add( heplist[loop] );
   } // for loop
}

void
CLPHEPGCont::add (const CLPHEPGObject &object)
{
   TClonesArray &hepgTca = *m_hepgTcaPtr;
   new (hepgTca[m_numObjects++]) CLPHEPGObject( object );
}

double
CLPHEPGCont::mass (unsigned int index) const
{
   // first check to make sure we're looking at valid
   // index
   if ( (int) index >= m_numObjects)
   {
      // index too big
      cout << "Error: CLPHEPGCont::mass: Index not valid." << endl;
      return kErrorValue;
   }
   CLPHEPGObject *objPtr = (CLPHEPGObject*) m_hepgTcaPtr->At(index);
   CLP4Vector vec = objPtr->mom4Vec();
   cout << "vec " << vec << endl;
   cout << *this << endl;
   return vec.mass();
}

bool
CLPHEPGCont::topFCNCdecayOk_2l() const
{
   // We expect the HEPG container to contain the following 
   // events
   // 0) top (6)    - top quark
   // 1) tbar (-6)  - top quark
   // 2) Z0 (23)    - Z0
   // 3) cbar (-4)  - charm or up quark
   // 4) E- (11)    - lepton
   // 5) E+ (-11)   - lepton
   // 6) W+ (24)    - W boson
   // 7) bottom (5) - bottom quark
   // 8) dbar (-1)  - light quark
   // 9) up (2)     - light quark
   
   // first check size
   if (size() != 10)
   {
      return false;
   }
   // top quarks
   if (! object(kTop1).isaTop() || ! object(kTop2).isaTop())
   {
      return false;
   }
   // W and Z bosons
   if (! object(kZboson).isaZ() || ! object(kWboson).isaW())
   {
      dout << endl << object(3) << endl << object(6) << endl;
      return false;
   }
   // leptons
   if (! object(kLepton1).isaLightLepton() 
       || ! object(kLepton2).isaLightLepton())
   {
      return false;
   }
   // bottom and charm/up quark
   if (! object(kBottom).isaBottom() 
       || ! (object(kCharm).isaCharm() || object(kCharm).isaUp() ) )
   {
      return false;
   }
   // light quarks
   if (! object(kLight1).notaTopQuark() 
       || ! object(kLight2).notaTopQuark())
   {
      return false;
   }
   // If we're still here, we're o.k.
   return true;
}

bool
CLPHEPGCont::topFCNCdecayOk_3l() const
{
   // We expect the HEPG container to contain the following 
   // events
   // 0) top (6)    - top quark
   // 1) tbar (-6)  - top quark
   // 2) Z0 (23)    - Z0
   // 3) cbar (-4)  - charm or up quark
   // 4) E- (11)    - lepton
   // 5) E+ (-11)   - lepton
   // 6) W+ (24)    - W boson
   // 7) bottom (5) - bottom quark
   // 8) lepton     - lepton
   // 9) neutrino
   
   // first check size
   if (size() != 10)
   {
      return false;
   }
   // top quarks
   if (! object(kTop1).isaTop() || ! object(kTop2).isaTop())
   {
      return false;
   }
   // W and Z bosons
   if (! object(kZboson).isaZ() || ! object(kWboson).isaW())
   {
      dout << endl << object(3) << endl << object(6) << endl;
      return false;
   }
   // leptons
   if (! object(kLepton1).isaLightLepton() 
       || ! object(kLepton2).isaLightLepton())
   {
      return false;
   }
   // bottom and charm/up quark
   if (! object(kBottom).isaBottom() 
       || ! (object(kCharm).isaCharm() || object(kCharm).isaUp() ) )
   {
      return false;
   }
   // leptons and their neutrinos
   if (! object(kLight1).isaLightLepton() 
       || ! object(kLight2).isaLightLeptonNu() )
   {
      return false;
   }
   // If we're still here, we're o.k.
   return true;
}

bool
CLPHEPGCont::topSMdecayOk_allhad() const
{
   // We expect the HEPG container to contain the following 
   // events
   // 0) top (6)    - top quark
   // 1) tbar (-6)  - top quark
   // 2) W- (23)    - W-
   // 3) bbar (-5)  - bottom quark
   // 4) dbar (-1)  - light quark
   // 5) up (2)     - light quark
   // 6) W+ (24)    - W boson
   // 7) bottom (5) - bottom quark
   // 8) dbar (-1)  - light quark
   // 9) up (2)     - light quark
   
   // first check size
   if (size() != 10)
   {
      return false;
   }
   // top quarks
   if (! object(kTop1).isaTop() || ! object(kTop2).isaTop())
   {
      return false;
   }
   // W and Z bosons
   if (! object(kZboson).isaW() || ! object(kWboson).isaW())
   {
      return false;
   }
   // bottom and charm/up quark
   if (! object(kBottom).isaBottom() || ! object(kCharm).isaBottom())
   {
      return false;
   }
   // light quarks
   if (! object(kLepton1).notaTopQuark() 
       || ! object(kLepton2).notaTopQuark())
   {
      return false;
   }
   // light quarks
   if (! object(kLight1).notaTopQuark() 
       || ! object(kLight2).notaTopQuark())
   {
      return false;
   }
   // If we're still here, we're o.k.
   return true;
}

bool
CLPHEPGCont::allReconMatched (bool considerTracks) const
{
   // So, if we are considering tracks, if the leptons are matched to
   // tracks, that's o.k.  If we're not, the leptons can only be
   // matched to muons or electrons.

   bool lep1tightMatch = (0 != object(kLepton1).numPossibleTags() );
   bool lep2tightMatch = (0 != object(kLepton2).numPossibleTags() );
   bool lep1Match = lep1tightMatch || 
      (0 != object(kLepton1).numPossibleTrackTags() );
   bool lep2Match = lep2tightMatch || 
      (0 != object(kLepton2).numPossibleTrackTags() );

   if (considerTracks)
   {
      // We're considering tracks, so o.k.
      // This has FAILED if:
      // 1 - either lepton is not matched OR
      // 2 - neither lepton is matched to a tight lepton
      if ( ! lep1Match ||  ! lep2Match || 
           ! (lep1tightMatch || lep2tightMatch) )
      {
         return false;
      }
   } else {
      // So, we're not considering track matches.  Both leptons must
      // have a tight match
      if ( ! lep1tightMatch || ! lep2tightMatch)
      {
         return false;
      }
   }

   // check the two leptons
   if (considerTracks)
   {
      // matches to tracks o.k.
      if ( (0 == object(kLepton1).numPossibleTags() &&
            0 == object(kLepton1).numPossibleTrackTags() ) ||
           (0 == object(kLepton2).numPossibleTags() &&
            0 == object(kLepton2).numPossibleTrackTags()) )
      {
         // one of them wasn't matched, even to a track
         return false;
      }
   } else {
   }
   // check bottom and charm
   if (0 == object(kBottom).numPossibleTags() ||
       0 == object(kCharm).numPossibleTags() )
   {
      // one of them wasn't matched
      return false;
   }
   // O.k. we're still here.
   // If we're doing Z -> ll, W -> 2 jets, then we need to check the two W
   // jets.
   // If we're doing Z -> ll, W -> l nu, then we just need to check the
   // lepton.
   if (object(kLight1).notaTopQuark())
   {
      // W -> 2 jets
      // check W jets
      if (0 == object(kLight1).numPossibleTags() ||
          0 == object(kLight2).numPossibleTags() )
      {
         // one of them wasn't matched
         return false;
      }
   } // W -> jets 
   else 
   {
      // W -> l nu
      if (0 == object(kLight1).numPossibleTags())
      {
         return false;
      }
   } // W -> leptons

   // Great.  We're still here.  Let's make sure we're not overly matched
   if ((object(kLepton1).taggedToReconIndex() ==
        object(kLepton2).taggedToReconIndex()) && 
       (object(kLepton1).taggedToTrackIndex() ==
        object(kLepton2).taggedToTrackIndex()) )
   {
      // We have two hepg leptons tagged to one reconstructed lepton
      return false;
   }

   // make sure the bottom quark isn't double tagged
   if ( (object(kBottom).taggedToReconIndex() ==
         object(kCharm).taggedToReconIndex() ) ||
        (object(kBottom).taggedToReconIndex() ==
         object(kLight1).taggedToReconIndex() ) ||
        (object(kBottom).taggedToReconIndex() ==
         object(kLight2).taggedToReconIndex() ) )
   {
      return false;
   }
   // now the charm quark
   // make sure the bottom quark isn't double tagged
   if ( (object(kCharm).taggedToReconIndex() ==
         object(kLight1).taggedToReconIndex() ) ||
        (object(kCharm).taggedToReconIndex() ==
         object(kLight2).taggedToReconIndex() ) )
   {
      return false;
   }
   // The W daughters
   if ( (object(kLight1).taggedToReconIndex() ==
         object(kLight2).taggedToReconIndex() ) )
   {
      return false;
   }

   // Ahhh... So we're still here.  Must be good to go
   return true;
}

bool
CLPHEPGCont::allPossibleRecon() const
{
   set< int > jetSet;
   for (int loop = 0; loop < size(); ++loop)
   {
      if (object(loop).isaTop() || object(loop).isaWeakBoson() )
      {
         //cout << "not a " << object(loop).name() << endl;
         continue;
      }
      // is this object found?
      if (! object(loop).numPossibleTags())
      {
         //cout << object(loop).name() << " not found." << endl;
         return false;
      }
      if (object(loop).isaQuark())
      {
         int recon = object(loop).taggedToReconIndex();
         if (jetSet.end() == jetSet.find (recon))
         {
            jetSet.insert (recon);
         } else {
            // we've seen this one before
            return false;
         }
      }
   } // for loop
   return true;
}

void
CLPHEPGCont::resetJetHEPGMatching(const CLPJet::JetList &jetList)
{
   // First, go through and zero out any matches of possible jets
   for (int loop = 0; loop < m_numObjects; ++loop)
   {
      // Don't bother unless its a (not a top) quark
      if (! operator()(loop).notaTopQuark() )
      {
         continue;
      }
      // If we're still here, we've got a quark jet
      CLPHEPGObject &hepg = operator() (loop);
      hepg.setNumPossibleTags (0);
      hepg.setReconType (CLPObject::kUndefined);
      hepg.setTaggedToReconIndex (-1);
   } // for loop
   
   // Loop through the jets and hook up the matches
   int jetSize = (int) jetList.size();
   for (int loop = 0; loop < jetSize; ++loop)
   {
      if (jetList[loop].isTagged())
      {
         // here we go
         int index = jetList[loop].taggedToHEPGIndex();
         CLPHEPGObject &hepg = operator() (index);
         hepg.setNumPossibleTags (1);
         hepg.setReconType (CLPObject::kJet);
         hepg.setTaggedToReconIndex (loop);         
      }
   } // for loop
}

// CLPHEPGObject
// CLPHEPGCont::object (int index) const
// {
//    // // first check to make sure we're looking at valid
//    // // index
//    // if ( (int) index >= m_numObjects)
//    // {
//    //    // index too big
//    //    cout << "Error: CLPHEPGCont::object: Index not valid." << endl;
//    //    CLPHEPGObject retval;
//    //    return retval;
//    // }
//    // CLPHEPGObject *objPtr = (CLPHEPGObject*) m_hepgTcaPtr->At(index);
//    // return *objPtr;
//    return operator() (index);
// }

void
CLPHEPGCont::copyTcaToTarget (TClonesArray *target)
{
   copyTCA (m_hepgTcaPtr, target);
}

string
CLPHEPGCont::classify() const
{
   string retval;
   CLPHEPGObject zobj = object (kZboson);
   if (zobj.isaZ())
   {
      retval += "Z";
   } else if (zobj.isaW())
   {
      retval += "W";
   } else {
      retval += "_";
   }
   CLPHEPGObject zdaut = object (kLepton1);
   if (zdaut.isaLightLepton())
   {
      retval += "ll";
   } else if (zdaut.isaTau())
   {
      retval += "tautau";
   } else if (zdaut.isaLeptonNu())
   {
      retval += "vv";
   } else if (zdaut.isaQuark())
   {
      retval += "qq";
   } else {
      retval += "__";
   }
   retval += " ";
   CLPHEPGObject wobj = object (kWboson);
   if (wobj.isaZ())
   {
      retval += "Z";
   } else if (wobj.isaW())
   {
      retval += "W";
   } else {
      retval += "_";
   }
   CLPHEPGObject wdaut = object (kLight1);
   if (wdaut.isaLightLepton() || wdaut.isaLightLeptonNu())
   {
      retval += "lv";
   } else if (wdaut.isaTau() || wdaut.isaTauNu())
   {
      retval += "tauv";
   } else if (wdaut.isaQuark())
   {
      retval += "qq'";
   } else {
      retval += "__";
   }
   return retval;
}

// friends
ostream& operator<< (ostream& o_stream, const CLPHEPGCont &rhs)
{
   CLPHEPGObject::outputHepTCA (rhs.m_hepgTcaPtr, o_stream);
   return o_stream;
} 

ClassImp (CLPHEPGCont) 
