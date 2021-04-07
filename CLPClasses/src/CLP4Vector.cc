// -*- C++ -*-
#include <iostream>
#include <math.h>
#include "CLPClasses/CLP4Vector.h"
#include "CLPClasses/CLPFunctions.h"
#include "CLPClasses/dout.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

ClassImp (CLP4Vector);



// constants
const double CLP4Vector::kErrorMass = -1.0;
bool CLP4Vector::sm_scaleEverything = true;

CLP4Vector::CLP4Vector() : CLP3Vector()
{
   m_time = 0;
}

CLP4Vector::CLP4Vector(const TLorentzVector &vec) :
   CLP3Vector (vec.Px(), vec.Py(), vec.Pz())
{
   m_time = vec.E();
}

CLP4Vector::CLP4Vector(double x, double y, double z, double t) :
   CLP3Vector(x, y, z)
{
   m_time = t;
}

CLP4Vector::CLP4Vector(const CLP3Vector &vec, double t) :
   CLP3Vector (vec)
{
   m_time = t;
}

CLP4Vector::CLP4Vector(const CLP3Vector &vec) :
   CLP3Vector (vec)
{  
   m_time = mag();
}

CLP4Vector::CLP4Vector(const CLP4Vector &vec)
{
   *this = vec;
}

CLP4Vector 
CLP4Vector::operator+(const CLP4Vector &vec) const 
{
   //CLP4Vector retval(threeVector() + vec.threeVector(),
   //                  t() + vec.t());
   //cout << endl
   //     << "this " << *this << endl
   //     << "that " << vec   << endl
   //     << "t + t" << retval << endl
   //     << "this.t() " << t() << endl
   //     << "that.t() " << vec.t() << endl;
   return CLP4Vector(threeVector() + vec.threeVector(),
                     t() + vec.t());
}

CLP4Vector
CLP4Vector::operator*(double factor) const 
{
   if (sm_scaleEverything)
   {
      return scaleEverything (factor); 
   }
   CLP4Vector retval (threeVector() * factor);
   double oldMass = mass();
   retval.setRestMass(oldMass);
   return retval;
}

const CLP4Vector 
CLP4Vector::operator*= (double factor)
{
   *this = *this * factor;
   return *this;
}

CLP4Vector
CLP4Vector::scaleEverything (double factor) const
{
   CLP4Vector retval = *this;
   retval.CLP3Vector::operator*= (factor);
   retval.m_time *= factor;
   return retval;
}


double
CLP4Vector::operator*(const CLP4Vector &vec) const 
{
   return t() * vec.t() - threeVector() * vec.threeVector();
}

void
CLP4Vector::setRestMass (double mss) 
{
   m_time = sqrt ( mss * mss + mag2() );
}

void
CLP4Vector::boost (const CLP3Vector &boostvec)
{
   // void TLorentzVector::Boost(Double_t bx, Double_t by, Double_t bz)
   // {
   //    Double_t b2 = bx*bx + by*by + bz*bz;
   //    Double_t gamma = 1.0 / TMath::Sqrt(1.0 - b2);
   //    Double_t bp = bx*X() + by*Y() + bz*Z();
   //    Double_t gamma2 = b2 > 0 ? (gamma - 1.0)/b2 : 0.0;
   //    
   //    SetX(  X() + gamma2 * bp * bx + gamma * bx * T()  );
   //    SetY(  Y() + gamma2 * bp * by + gamma * by * T()  );
   //    SetZ(  Z() + gamma2 * bp * bz + gamma * bz * T()  );
   //    SetT( gamma * ( T() + bp ) );
   // }

   double beta2 = boostvec.mag2();
   if ((0 == beta2) || (beta2 >= 1.))
   {
      // there's nothing left to do, so let's get out now
      return;
   }
   double gamma = 1. / sqrt (1. - beta2);
   CLP3Vector three = threeVector();
   double scalarProd = three * boostvec;
   double factor = ( gamma - 1.) / beta2 ;

   // setX( x() + factor * scalarProd * boostvec.x() + 
   //       gamma * boostvec.x() * t() );
   // setY( y() + factor * scalarProd * boostvec.y() + 
   //       gamma * boostvec.y() * t() );
   // setZ( z() + factor * scalarProd * boostvec.z() + 
   //       gamma * boostvec.z() * t() );

   // we need to have the CLP3Vector first in the products 
   // (e.g. boostvec * ..., not ... * boostvec)
   setXYZ( three + boostvec * factor * scalarProd + boostvec * gamma *  t() );
   setT( gamma * ( t() + scalarProd ) );
}
   
CLP3Vector
CLP4Vector::boostVector() const
{
   if (0. == t())
   {
      // we've got no energy, so
      return *CLP3Vector::sm_zero3VecPtr;
   }
   return CLP3Vector (x() / t(), y() / t(), z() / t() );
}

double
CLP4Vector::mass2() const
{
   return (*this) * (*this);
}

double
CLP4Vector::mass() const
{
   double retval = mass2();
   if (retval >= 0) {
      return sqrt(retval);
   } else {
      //cerr << "ERROR: CLP4Vector::mass: mass2 = " << retval << " < 0:" 
      //     << *this << endl;
      return kErrorMass;
   }
}

string
CLP4Vector::stringRep() const
{
   return Form ("%.3f : ", t()) + CLP3Vector::stringRep();
}

TLorentzVector
CLP4Vector::tlorentz() const
{
   TLorentzVector retval (x(), y(), z(), t());
   return retval;
}

// Static Member Functions
CLP4Vector
CLP4Vector::from3VectorMass(const CLP3Vector &vec, double mass)
{
   return CLP4Vector(vec, addInQuad(vec.mag(), mass));
}

// friends
ostream& operator<< (ostream& o_stream, const CLP4Vector &rhs)
{
   if (CLP3Vector::kStandardOutputMode == CLP3Vector::output3Mode())
   {
      // x, y, z
      o_stream << "("  << Form ("%6.2f", rhs.t() )
               << ": " << Form ("%7.2f", rhs.x() )
               << ", " << Form ("%7.2f", rhs.y() )
               << ", " << Form ("%7.2f", rhs.z() )
               << ")";
   } 
   else 
   {
      o_stream << "("  << rhs.t()
               << ": " << Form ("%4.1f", rhs.transMag())
               << "; " << Form ("%5.2f", rhs.eta())
               << ", " << Form ("%5.2f", rhs.phi())
               << ")";
   }

   return o_stream;
} 
