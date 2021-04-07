// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include "CLPClasses/CLPElectron.h"
#include "CLPClasses/CLPFunctions.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

ClassImp (CLPElectron)

const double CLPElectron::kAveragePesDistance = 185.4;

CLPElectron::CLPElectron() : CLPChargedParticle(), m_phxDuplicate(false)
{
   setType(CLPObject::kElectron);
   setCpType (kUndefinedElectron);
}

void
CLPElectron::calcQuantities()
{
   // In Phoenix and Plug electrons, we use a different version
   // of eta.  Let's save the old version, just in case.
   setOldEta(_tempeta());
   setOldPhi(_tempphi());
   /////////////////////////////
   // Classify Electron First //
   /////////////////////////////

   ////////////////
   // Top Ntuple //
   ////////////////
   
   // Classify electron
   // Check for TCE
   if ( (ns_ELETYPE_DefEm == collectionType()) &&
        (0 == region()) &&
        (1 == fiducial()) )
   {
      // TCE
      setCpType (kTightCentralElectron);
   } 
   // Check for PHX
   else if ( (ns_ELETYPE_DefEm == collectionType()) &&
             (1 == region()) && 
             ( 1 == phxMatch() ))
   {
      // PHX
      setCpType (kPhoenixElectron);
   } 
   // Check for PEM
   else if ( (ns_ELETYPE_DefEm == collectionType()) &&
               (1 == region()) )
   {
      // PEM
      setCpType (kPlugElectron);
   }

   //////////////////////////////////
   // Make Necessary Modifications //
   //////////////////////////////////
   if (isaPHX())
   {
      // fix charge
      setCharge(phxCharge());
      // use 'right' eta and phi
      setEta(pes2dEta());
      setPhi(pes2dPhi());
      // to make PHX more similar to TCE
      // setZ0(phxTrkZ0());
      //setSiHits(phxSiHits());
   } else if (isaPEM())
   {
      // fix charge
      setCharge(0); // no charge, but what will it cost me? hehehehe
      // use 'right' eta and phi
      setEta(pes2dEta());
      setPhi(pes2dPhi());
      // Rochester doesn't differentiate between PHX and PEM at this
      // point.  This may become a phoenix electron later, so make
      // sure that we have the right stuff laying around.
      // to make PHX more similar to TCE
      // setZ0(phxTrkZ0());
      // setSiHits(phxSiHits());
   }
   // get base class
   CLPChargedParticle::calcQuantities();
}

bool
CLPElectron::couldBeaPHX() const
{
   return isaPHX() || (isaPEM() && ( 1 == phxMatch() ));
}

double 
CLPElectron::pesPemDistance() const
{
   // make a 3 vector of the transverse component only
   CLP3Vector pemTransVec = 
      CLP3Vector::fromEtaPhiZed(pem3x3DetEta(), 
                                pem3x3Phi(), 
                                kAveragePesDistance).transverse();
   CLP3Vector pesTransVec =
      CLP3Vector (pes2dX(), pes2dY(), 0 );
   return (pemTransVec - pesTransVec).mag();
}
   

// friends
ostream& operator<< (ostream& o_stream, const CLPElectron &rhs)
{
   if (kShortOutput == outputMode())
   {
      o_stream << "Electron:"
               << rhs.index()
               << " " << rhs.shortTypeString()
               << " " << rhs.mom3Vec()
               //<< " " << rhs.sign()
               << " " << rhs.taggedToHEPGIndex();
   }
   else
   {
      o_stream << "Electron:"
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
               << " EMenergy: " << rhs.EMenergy()
               << " transEnergy: " << rhs.transEnergy()
               << " region: " << rhs.region()
               << " collectionType: " << rhs.collectionType()
               << " conversion: " << rhs.conversion()
               << " LShrTrk: " << rhs.LShrTrk()
               << " EoverP: " << rhs.EoverP()
               << " hadOverEm: " << rhs.hadOverEm()
               << " deltaX: " << rhs.deltaX()
               << " deltaZ: " << rhs.deltaZ()
               << " pem3x3FitTower: " << rhs.pem3x3FitTower()
               << " pem3x3DetEta: " << rhs.pem3x3DetEta()
               << " pem3x3Chi2: " << rhs.pem3x3Chi2()
               << " pes2dEta: " << rhs.pes2dEta()
               << " pes2dPhi: " << rhs.pes2dPhi()
               << " pes2d5x9U: " << rhs.pes2d5x9U()
               << " pes2d5x9V: " << rhs.pes2d5x9V()
               << " pes2dX: " << rhs.pes2dX()
               << " pes2dY: " << rhs.pes2dY()
               << " stripChi2: " << rhs.stripChi2()
               << " phxTrkZ0: " << rhs.phxTrkZ0()
               << " trackIDOrig " << rhs.trackIdOrig()
               << " CutCode: " << rhs.cutCode();
   }
   return o_stream;
} 
