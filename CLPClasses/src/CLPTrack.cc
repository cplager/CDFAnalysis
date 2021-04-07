// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <math.h>
#include "CLPTrack.h"
#include "CLPClasses/CLPNamespace.h"
#include "CLPClasses/CLPFunctions.h"
#include "CLPClasses/dout.h"


using namespace CLPNamespace;
using namespace std;

ClassImp (CLPTrack)


CLPTrack::CLPTrack() : CLPChargedParticle()
{
   setType(kTrack);
   // set default to unknown!
   setCpType (kUndefinedTrack);
   setMatchMuon(false);
   setMatchElectron(false);
}

void
CLPTrack::calcQuantities()
{
   //   if (collectionType() == ns_OTRKTYPE_TrackLepton)
   if (collectionType() == ns_OTRKTYPE_DefTrack)
   {
      setCpType (kTrackLepton);
   }
   setMomentumType(kAsMuonMom);
   
   // get base class
   CLPChargedParticle::calcQuantities();
}

bool
CLPTrack::isTrackIsolated(double cutValue) const
{
   //default is CLPNamespace::kTrackDefaultIsolationCut
   if (trackIsol() > cutValue)
   {
      return true;
   } else {
      return false;
   }
}

bool CLPTrack::rescaleMomAs(const MomentumType momentumType)
{
   // will rescale the momentum based on the Et and Pt of a track.
   // The boolean it returns tells if the momentum was actually
   // changed.
   double scale = calEmTower() / BCPt();
   if (kSavedMom == momentumType || momentumType == m_momentumType)
   {
      // lets do nothing because the momentum is alread correct and
      // saved.
      return false;
   } 
   else if (scale < 1 && momentumType != kSavedMom)
   {
      // we aren't going to change anything because the scale factor
      // is less than one.
      m_momentumType = momentumType;
      return false;
   }
   else if (kAsMuonMom == momentumType && kAsElectronMom == m_momentumType)
   {
      setMom3Vec(m_mom3Vec * (1.0 / scale) );
      // setMom3Vec(CLP3Vector::fromEtaPhiTrans(eta(), phi(), BCPt()));
      m_momentumType = kAsMuonMom;
      return true; // maybe I should think about checking to see if it
                   // really changed
   } 
   else if (kAsElectronMom == momentumType && kAsMuonMom == m_momentumType)
   {
      m_momentumType = kAsElectronMom;
      setMom3Vec(m_mom3Vec * scale);
      // setMom3Vec(CLP3Vector::fromEtaPhiTrans(eta(), phi(), 
      //                                        calEmTower()));
      return true;
      // setMom3Vec(CLP3Vector::fromEtaPhiTrans(eta(), phi(), BCPt()));
   } else {
      // we shouldn't be here...
      cerr << "ERROR: CLPTrack::rescaleMomAs - the track isn't an electron or muon... huh?" << endl;
      return false;
   }
}

CLP3Vector CLPTrack::mom3Vec() const
{
   return mom3Vec (kSavedMom);
}

CLP3Vector CLPTrack::mom3Vec(const MomentumType momentumType) const
{
   double scale = calEmTower() / BCPt();
   if (kSavedMom == momentumType || m_momentumType == momentumType || 
       scale < 1)
   {
      return m_mom3Vec;
   }
   else if (kAsMuonMom == momentumType && kAsElectronMom == m_momentumType)
   {
      return m_mom3Vec * (1.0 / scale);
      // return CLP3Vector::fromEtaPhiTrans(eta(), phi(), BCPt());
   }
   else if (kAsElectronMom == momentumType && kAsMuonMom == m_momentumType)
   {
      return m_mom3Vec * scale;
      // return CLP3Vector::fromEtaPhiTrans(eta(), phi(), calEmTower());
      // return CLP3Vector::fromEtaPhiTrans(eta(), phi(), BCPt());
   }
   else 
   {
      // we shouldn't be here...
      cerr << "ERROR: CLPTrack::mom3Vec - the track isn't an electron or muon... huh?" << endl;
      return m_mom3Vec;
   }
}

CLP4Vector CLPTrack::mom4Vec() const
{
   return mom4Vec (kSavedMom);
}

CLP4Vector CLPTrack::mom4Vec(const MomentumType momentumType) const
{
   return CLP4Vector( mom3Vec(momentumType) );
}  

//  double CLPTrack::Pt(const MomentumType momentumType) 
//  {
//     if (kSavedMom == momentumType || m_momentumType == momentumType)
//     {
//        return m_mom3Vec.pt();
//     }
//     else if (kAsMuonMom == momentumType)
//     {
//        return BCPt();
//     }
//     else if (kAsElectronMom == momentumType)
//     {
//        return calEmTower();
//     }
//     else 
//     {
//        // we shouldn't be here...
//        cerr << "the track isn't an electron or muon... huh?" << endl;
//        return BCPt();
//     }
//  }
//  
//  double CLPTrack::Et(const MomentumType momentumType) 
//  {
//     if (kSavedMom == momentumType) 
//     {
//        // I'm going to base this answer on whether we are treating the
//        // track as an electron or muon for the momentum
//        if (kAsMuonMom == m_momentumType)
//        {
//           return calHadTower();
//        } else if (kAsElectronMom == m_momentumType)
//        {
//           return calEmTower();
//        } else
//        {
//           // we shouldn't be here either...
//           cerr << "the track isn't an electron or muon... huh?" << endl;
//        } // if
//     } else if (kAsMuonMom == momentumType)
//     {
//        return calHadTower();
//     }
//     else if (kAsElectronMom == momentumType)
//     {
//        return calEmTower();
//     }
//     else 
//     {
//        // we shouldn't be here...
//        cerr << "the track isn't an electron or muon... huh?" << endl;
//        return BCPt();
//     }
//  }

void 
CLPTrack::matchElectron(TrackList &trackList, 
                        const CLPElectron::ElectronList &elecList)
{
   for (unsigned int outer = 0; outer < trackList.size(); ++outer)
   {
      for (unsigned int inner = 0; inner < elecList.size(); ++inner)
      {
         if (trackList[outer].trackIdOrig() == elecList[inner].trackIdOrig())
         {
            trackList[outer].setMatchElectron(true);
            break;
         }//if the trackId matches
      } //for the electrons
   } //for the tracks
}


void 
CLPTrack::matchMuon(TrackList &trackList, 
                    const CLPMuon::MuonList &muonList)
{
   for (unsigned int outer = 0; outer < trackList.size(); ++outer)
   {
      for (unsigned int inner = 0; inner < muonList.size(); ++inner)
      {
         if (trackList[outer].trackIdOrig() == muonList[inner].trackIdOrig())
         {
            trackList[outer].setMatchMuon(true);
            break;
         }//if the trackId matches
      } //for the muons
   } //for the tracks
}


// friends
ostream& operator<< (ostream& o_stream, const CLPTrack &rhs)
{
   
   o_stream << (CLPChargedParticle) rhs;
   if (rhs.matchElectron())
   {
      o_stream << "  ME";
   } else if (rhs.matchMuon())
   {
      o_stream << "  MM";
   } else 
   {
      o_stream << "    ";
   } //add if the track matches other leptons
   if (kShortOutput != CLPFunctions::outputMode())
   {
      o_stream << "  "
               << " tIsol " << rhs.trackIsol()
               << " mElec " << (rhs.isMatchedToElectron() ? "true" : "false")
               << " mMuon " << (rhs.isMatchedToMuon() ? "true" : "false")
               << " expectSi " << rhs.expectedSiHits();
   }

   return o_stream;
} 
