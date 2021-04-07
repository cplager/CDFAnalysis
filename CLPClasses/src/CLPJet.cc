// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "dout.h"
#include "CLPClasses/CLPJet.h"
#include "CLPClasses/CLPFunctions.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

ClassImp (CLPJet)
;

// default level of jet corrections to use
int                CLPJet::sm_corrType       = CLPJet::kLevel5;
CLPJet::JetCone    CLPJet::sm_jetCone        = CLPJet::k04;
CLPJet::SVXTagType CLPJet::sm_defaultTagType = CLPJet::kLoose;

const CLPJet::FourMomFuncPtr 
  CLPJet::kMasslessFourMom        = &CLPJet::_massless4Vec;
const CLPJet::FourMomFuncPtr 
  CLPJet::kMassiveFourMom         = &CLPJet::_massive4Vec;
const CLPJet::FourMomFuncPtr 
  CLPJet::kMassiveZeroMassFourMom = &CLPJet::_massiveZeroMass4Vec;
const CLPJet::FourMomFuncPtr 
  CLPJet::kMassiveDropMassFourMom = &CLPJet::_massiveDropMass4Vec;
CLPJet::FourMomFuncPtr       
  CLPJet::defaultFourMom          =  CLPJet::kMassiveFourMom;

CLPJet::CLPJet() : CLPObject(CLPObject::kJet), m_heavyFlavor(0)
{
   // set all correction factors to unity
   for (int loop = 0; loop < kNumJetCorrections; ++loop)
   {
      m_correctionScale[loop] = 1.;
   }
   for (int loop = 0; loop < kNumTagTypes; ++loop)
   {
      m_bTag[loop]    = 0;
      m_tagProb[loop] = 0.;
   } // for loop
   m_jetCone = kNoCone;
}

bool
CLPJet::operator< (const CLPJet &rhs) const
{
   return Et() < rhs.Et();
}

bool
CLPJet::operator> (const CLPJet &rhs) const
{
   return Et() > rhs.Et();
}

int
CLPJet::bTag (SVXTagType type) const
{
   if (type >= kNumTagTypes)
   {
      type = sm_defaultTagType;
   }
   return m_bTag[type];
}

float
CLPJet::tagProb (SVXTagType type) const
{
   if (type >= kNumTagTypes)
   {
      type = sm_defaultTagType;
   }
   return m_tagProb[type];
}

void
CLPJet::setBTag (SVXTagType type, int tag) 
{
   if (type >= kNumTagTypes)
   {
      type = sm_defaultTagType;
   }
   m_bTag[type] = tag;
}

void
CLPJet::setTagProb (SVXTagType type, float prob) 
{
   if (type >= kNumTagTypes)
   {
      type = sm_defaultTagType;
   }
   m_tagProb[type] = prob;
}

Int_t
CLPJet::Compare (const TObject *objPtr) const
{
   // This function is setup so that we get the HIGH ET jets first.
   CLPJet *jetPtr = (CLPJet*) objPtr;
   if (*this < *jetPtr)
   {
      return 1;
   } 
   else if (*this > *jetPtr)
   {
      return -1;
   }
   return 0;
}

void
CLPJet::calcQuantities()
{
   if (ns_JETTYPE_Jetclu_04_LJ == m_collectionType)
   {
      m_jetCone = k04;
   }
   //else if (ns_JETTYPE_Jetclu_07_LJ == m_collectionType)
   //{
   //   m_jetCone = k07;
   //}
   else
   {
      m_jetCone = kNoCone;
   }
}

CLP3Vector
CLPJet::deltaCorrectTransVec(int type) const
{
   return ( mom3Vec(type) - mom3Vec(kUncorrected) ).transverse();
}


bool
CLPJet::isBTagged (SVXTagType type) const
{
   return (bTag (type) > 0);
}

void
CLPJet::outputJetList (const CLPJet::JetList &jetlist)
{
   int size = (int) jetlist.size();
   for (int loop = 0; loop < size; ++loop)
   {
      cout << setw(2) << loop << ") " << jetlist[loop] << endl;
   }
}

double
CLPJet::JCscale(int type) const
{
   if (type < 0)
   {
      return 1.;
   }
   else if (type < kNumJetCorrections)
   {
      return m_correctionScale[type];
   }
   else
   {
      cerr << "Shouldn't be here" << endl;
      assert(0);
      return -1.;
   }
}

void
CLPJet::setCorrectionScale(int type, float value)
{
   if (type >= 0 && type < kNumJetCorrections)
   {
      m_correctionScale[type] = value;
   }
   else
   {
      cerr << "Shouldn't be here" << endl;
      assert(0);
   }
}

void
CLPJet::setMassiveMom4Vec (double px, double py, double pz, 
                           double energy, double Et)
{
   m_massiveMom4Vec = CLP4Vector (px, py, pz, energy);
   m_massiveEt = Et;
   double threeEnergy = ( (CLP3Vector) m_massiveMom4Vec ).mag();
   m_withWithoutMassFactor = energy / threeEnergy;
}

void
CLPJet::setMasslessMom4Vec (double uncorrEt, double eta, double phi)
{
   m_masslessMom4Vec = 
      (CLP4Vector) CLP3Vector::fromEtaPhiTrans (eta, phi, uncorrEt);
}

CLP4Vector 
CLPJet::_massless4Vec (int type) const
{
   if (type < kUncorrected)
   {
      type = sm_corrType;
   }
   return m_masslessMom4Vec.scaleEverything( JCscale (type) );
}

CLP4Vector 
CLPJet::_massive4Vec (int type) const
{
   if (type < kUncorrected)
   {
      type = sm_corrType;
   }
   return m_massiveMom4Vec.scaleEverything( JCscale (type) );
}

CLP4Vector 
CLPJet::_massiveZeroMass4Vec (int type) const
{
   if (type < kUncorrected)
   {
      type = sm_corrType;
   }
   CLP3Vector vec = (CLP3Vector) m_massiveMom4Vec;
   vec *= m_withWithoutMassFactor;
   vec *= JCscale (type);
   return (CLP4Vector) vec;
}

CLP4Vector 
CLPJet::_massiveDropMass4Vec (int type) const
{
   if (type < kUncorrected)
   {
      type = sm_corrType;
   }
   CLP3Vector vec = (CLP3Vector) m_massiveMom4Vec;
   vec *= JCscale (type);
   return (CLP4Vector) vec;
}

// Momentum member functions
CLP4Vector 
CLPJet::mom4Vec (CLPJet::FourMomFuncPtr funcPtr, int type,
                 ScaleFactorType sftype) const
{
   if (kNoScaleFactor == sftype)
   {
      return (this->*funcPtr) (type);
   }
   double scale;
   if (kLightJetScaleFactor == sftype)
   {
      scale = lightJetMultFactor_TM();
   } else if (kBottomJetScaleFactor == sftype)
   {
      scale = bottomJetMultFactor_TM();
   } else {
      scale = 0; // avoid compiler warnings
      assert (0);
   }
   CLP4Vector vec = (this->*funcPtr) (type);
   return vec.scaleEverything (scale);
}

CLP3Vector 
CLPJet::mom3Vec (CLPJet::FourMomFuncPtr funcPtr, int type,
                 ScaleFactorType sftype) const
{
   return (CLP3Vector) mom4Vec (funcPtr, type, sftype);
}

double 
CLPJet::Pt (CLPJet::FourMomFuncPtr funcPtr, int type,
            ScaleFactorType sftype) const
{
   return mom4Vec (funcPtr, type, sftype).Pt();
}

double 
CLPJet::Et (CLPJet::FourMomFuncPtr funcPtr, int type,
            ScaleFactorType sftype) const
{
   double Et = m_massiveEt;
   // Is this NOT the massive case
   if (kMassiveFourMom != funcPtr)
   {
      Et = mom4Vec (funcPtr, type, sftype).Pt();
   }
   return Et * JCscale (type);
}

double
CLPJet::energy (CLPJet::FourMomFuncPtr funcPtr, int type,
                ScaleFactorType sftype) const
{
   return mom4Vec (funcPtr, type, sftype).e();
}

double 
CLPJet::lightJetMultFactor_TM() const
{
   // // First get the MPV (Most Probable Value)
   // Double_t meanw = exp(-1.911-0.01775*jets5[i].Pt()) - 0.01822;
   // relativeMeanFactor = 1 + meanw;

   // must use level 5
   return 1 + exp ( -1.911 - 0.01775 * Et(kLevel5) ) - 0.01822;
}

double 
CLPJet::lightJetRelativeWidth_TM() const
{
   // // Now, get the resolution
   // Double_t pws = exp(2.441-0.02482*detEta[i]*detEta[i]) - 12.67;
   // Double_t relative_sigma = exp(pws-0.02693*jets5[i].Pt()) + 0.09635;
   // absoluteWidth = relative_sigma*jets5[i].Pt();

   // must use level 5
   double detEta1 =  etaDetector();
   double pws1 = exp ( 2.441 -0.02482 * detEta1 * detEta1 ) - 12.67;
   return exp ( pws1 - 0.02693 * Et(kLevel5) ) + 0.09635;
}

double
CLPJet::lightJetAbsoluteWidth_TM() const
{
   // Fine for Level 5, but this should be fixed
   return mom3Vec(kLevel5).mag() * lightJetMultFactor_TM()
      * lightJetRelativeWidth_TM();
}

double 
CLPJet::bottomJetMultFactor_TM() const
{
   // Double_t pb = exp(-1.159-0.692*detEta[i]*detEta[i]) - 0.581;
   // Double_t meanb = exp(pb-0.04734*jets5[i].Pt()) + 0.02908;
   // relativeMeanFactor = 1 + meanb;
   double detEta = etaDetector();
   double temp = exp ( -1.159 - 0.692 * detEta * detEta ) - 0.581;
   return 1 + exp ( temp - 0.04734 * Et(kLevel5) ) + 0.02908;
}

double 
CLPJet::bottomJetRelativeWidth_TM() const
{
   // Double_t pbs = exp(4.07-0.005256*detEta[i]*detEta[i]) - 58.02;
   // Double_t sigmab = exp(pbs-0.05118*jets5[i].Pt()) + 0.1316;
   // sigmajets[0][i]=sigmab*jets5[i].Pt();

   // must use level 5
   double detEta = etaDetector();
   double pbs =  exp(4.07 -0.005256 * detEta * detEta) - 58.02;
   return exp (pbs - 0.05118 * Pt()) + 0.1316;
}

double
CLPJet::bottomJetAbsoluteWidth_TM() const
{
   // Fine for Level 5, but this should be fixed
   return mom3Vec().mag() * bottomJetMultFactor_TM()
      * bottomJetRelativeWidth_TM();
}

double 
CLPJet::jetMultFactor_AbsL7Trans() const
{
   double et = Et();
   double retval = 2.10462e+00  + 
      (-3.60600e-02 + (9.33680e-04 + -2.13028e-06 * et) * et) * et;
   // Unfortunately, I did this backwards and used 'measured - actual'
   // instead of 'actual - measured', so I want the opposite of this
   retval *= -1;
   // Now for another stupid point: I measured the shift I wanted
   // (i.e. an additive correction).  But we need a multiplicative
   // correction.  So we need to turn this into a multiplicative
   // function.
   return (et + retval) / et;
}

double 
CLPJet::jetAbsoluteUpperWidth_AbsL7Trans() const
{
   // This is the width if I am increasing the momentum vectors, so I
   // need the 'negative' width since I did this backward
   double et = Et (kLevel7);
   double transRes = 2.76177e+00 +
      (1.44876e-01 + (-9.48037e-04 + 3.05517e-06 * et) * et) * et;
   // check to make sure this value is sane
   if (et < 20 * GEV)
   {
      double minimum = 2 * et / 20;
      if (transRes < minimum)
      {
         transRes = minimum;
      }
   } // if energy is small
   // This resolution is for transverse energy, but I want a
   // resolution for total energy
   return energy(kLevel7) * transRes / et;
}

double 
CLPJet::jetAbsoluteLowerWidth_AbsL7Trans() const
{
   // This is the width if I am decreasing the momentum vectors, so I
   // need the 'positive' width since I did this backward
   double et = Et (kLevel7);
   double transRes = 2.73215e-01 + 
      (1.08640e-01 + (-5.13950e-04 + 1.05850e-06 * et) * et) * et;
   // check to make sure this value is sane
   if (et < 20 * GEV)
   {
      double minimum = 2 * et / 20;
      if (transRes < minimum)
      {
         transRes = minimum;
      }
   }
   // This resolution is for transverse energy, but I want a
   // resolution for total energy
   return energy(kLevel7) * transRes / et;
}


void
CLPJet::setDefaultCorrectionType(int type)
{
   if (type >= 0 && type < kNumJetCorrections)
   {
      sm_corrType = type;
   }
   else if (type < 0)
   {
      sm_corrType = kUncorrected;
   }
   else
   {
      cerr << "Shouldn't be here" << endl;
      assert(0);
   }
}

void
CLPJet::setDefaultTagType (SVXTagType type) 
{
   if (type >= kNumTagTypes)
   {
      cerr << "CLPJet::setDefaultTagType(): Value not accepted.  Ignored."
           << endl;
      return;
   }
   sm_defaultTagType = type;
}

// friends
ostream& operator<< (ostream& o_stream, const CLPJet &rhs)
{
   o_stream << "Jet(" << setw(2) << rhs.index() << "): " 
            << rhs.mom3Vec() << " " << setw(4) << rhs.taggedToHEPGIndex()
            << "  Et (uncorr) : " << rhs.Et(-1)
            << " HF : " << rhs.heavyFlavor();
   return o_stream;
} 
