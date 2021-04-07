// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <math.h>
#include "CLPClasses/CLPMuon.h"
#include "CLPClasses/CLPFunctions.h"
#include "CLPClasses/dout.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

ClassImp (CLPMuon)

CLPMuon::CLPMuon() : CLPChargedParticle()
{
   setType(kMuon);
}

void
CLPMuon::calcQuantities()
{
   // WARNING: This has not yet been updated to Gen5
   // set default to unknown!
   setCpType (kUndefinedMuon);
   // Classify muon

   ////////////////
   // Top Ntuple //
   ////////////////

   // Check CMX
   if ( hasCmx() && isCmx() &&
             (ns_MUOTYPE_DefMuon == viewType() ) )
   {
      // CMX
      setCpType (kCMX);
   }

   // Check CMU
   else if ( hasCmu() && isCmu() && ! hasCmp() && ! isCmp() &&
        ( ns_MUOTYPE_DefMuon == viewType() ) )
   {
      // CMU
      setCpType (kCMU);
   } 

   // Check CMP
   else if ( hasCmp() && isCmp() && ! hasCmu() && ! isCmu() &&
             (ns_MUOTYPE_DefMuon == viewType() ) )
   {
      // CMP
      setCpType (kCMP);
   }

   // Check CMUP
   else if ( hasCmu() && isCmu() && hasCmp() && isCmp() &&
             (ns_MUOTYPE_DefMuon == viewType() ) )
   {
      // CMUP
      setCpType (kCMUP);
   }

   // Check CMIO
   else if ( isStubless() &&
             (ns_MUOTYPE_DefMuon == viewType() ||
              ns_MUOTYPE_DefStublessMuon == viewType() ) )
   {
      // CMIO
      setCpType (kCMIO);
   }
   CLPChargedParticle::calcQuantities();
}

bool
CLPMuon::isStubless() const
{
   if ((0 == muonType()) &&
       ! isCmu() && ! isCmp() && ! isCmu() && ! isBmu() 
      )
   {
      return true;
   }
   if (! isCmx() && (inMiniskirt() || inKeystone()) )
   {
      return true;
   }
   // if we're still here, it ain't stubless
   return false;
}

bool 
CLPMuon::isCmu() const
{   
   return  (fiducial() >> 0) & 1;
}

bool 
CLPMuon::isCmp() const
{
   // do exceptions first
   if (! ns_ignoreRunNumber && inBluebeam() && ns_runNumber < 154449)
   {
      return 0;
   }
   if (cmpFidX() > 0 || cmpFidZ() > -3)
   {
      return 0;
   }
   // so far so good
   return  (fiducial() >> 1) & 1;
}

bool 
CLPMuon::isCmx() const
{
   // do exceptions first
   if (! ns_ignoreRunNumber && (inMiniskirt() || inKeystone()) && 
       ns_runNumber < 186598)
   {
      return 0;
   }
   if (cmxFidX() > 0 || cmxFidZ() > -3)
   {
      return 0;
   }
   // so far so good
   return (fiducial() >> 2) & 1;
}

bool 
CLPMuon::isBmu() const
{
   if (! ns_ignoreRunNumber && inKeystone() && ns_runNumber < 186598)
   {
      return 0;
   }
   return  (fiducial() >> 3) & 1;
}


CLP3Vector 
CLPMuon::uncorrMom3Vec() const 
{ 
   return CLP3Vector::fromEtaPhiTrans(mom3Vec().eta(), mom3Vec().phi(), 
                                      uncorrPt());
}


CLP3Vector
CLPMuon::deltaCorrectTransVec() const
{
   return (mom3Vec() - uncorrMom3Vec()).transverse();
}

double 
CLPMuon::COTexitRadius() const
{
  double rho(0);
  const double kZcot = 155 * CM;
  const double theta  = eta2theta( mom3Vec().eta() );
  const double lambda = PI/2 - theta;

  if (mom3Vec().eta() > 0)
  {
     rho = (kZcot - Z0()) / tan(lambda);
  }
  else
  {
     rho = -1 * (kZcot + Z0()) / tan(lambda);
  }
  return rho;
}

// friends
ostream& operator<< (ostream& o_stream, const CLPMuon &rhs)
{
   if (kShortOutput == outputMode())
   {
      o_stream << "Muon:"
               << "  " << rhs.index()
               << "  " << rhs.typeString()
               << "  " << rhs.mom3Vec()
               << "  " << rhs.sign()
               << "  " << rhs.taggedToHEPGIndex();
   }
   else
   {
      o_stream << "Muon:"
               << " Index: " << rhs.index()
               << " Type: " << rhs.typeString()
               << " mom3Vec: " << rhs.mom3Vec()
               << " transMom: " << rhs.Pt()
               << " sign: " << rhs.sign()
               << " fiducial: " << rhs.fiducial()
               << " isolation: " << rhs.isolation()
               << " axSeg: " << rhs.axSeg()
               << " axHits: " << rhs.axHits()
               << " stSeg: " << rhs.stSeg()
               << " stHits: " << rhs.stHits()
               << " siHits: " << rhs.siHits()
               << " eta: " << rhs.mom3Vec().eta()
               << " phi: " << rhs.mom3Vec().phi()
               << " Z0: " << rhs.Z0()
               << " D0: " << rhs.D0()
               << " viewType: " << rhs.viewType()
               << " muonType: " << rhs.muonType()
               << " cmpFidX: " << rhs.cmpFidX()
               << " cmpFidZ: " << rhs.cmpFidZ()
               << " cmuDx: " << rhs.cmuDx()
               << " cmpDx: " << rhs.cmpDx()
               << " cmxDx: " << rhs.cmxDx()
               << " uncorrPt: " << rhs.uncorrPt()
               << " EMenergy: " << rhs.EMenergy()
               << " HadEnergy: " << rhs.HadEnergy()
               << " CutCode: " << rhs.cutCode();
   }
   return o_stream;
} 
