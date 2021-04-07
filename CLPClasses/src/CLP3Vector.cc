// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <math.h>
#include "CLPClasses/CLP3Vector.h"
#include "CLPClasses/CLPFunctions.h"

using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace std;

ClassImp (CLP3Vector)

// static constants;
CLP3Vector *CLP3Vector::sm_zero3VecPtr = (CLP3Vector *) 0;
// CLP3Vector::OutputModeType CLP3Vector::sm_outputMode =
// CLP3Vector::kPtEtaPhiOutputMode;
CLP3Vector::OutputModeType CLP3Vector::sm_outputMode =
CLP3Vector::kStandardOutputMode;

CLP3Vector::CLP3Vector()
{
   m_x = m_y = m_z = 0.;
}

CLP3Vector::CLP3Vector (double x, double y, double z)
{
   m_x = x;
   m_y = y;
   m_z = z;
}

CLP3Vector::CLP3Vector (const CLP3Vector &vec)
{
   *this = vec;
}

CLP3Vector::CLP3Vector (const TVector3 &vec)
{
   m_x = vec.x();
   m_y = vec.y();
   m_z = vec.z();
}

// element acessor
double 
CLP3Vector::operator[](const int index) const
{
   if (0 == index)
   {
      return m_x;
   } else if (1 == index)
   {
      return m_y;
   } else if (2 == index)
   {
      return m_z;
   } else 
   {
      // I think this is the safest bad value... 
      return -1000;
   }
} 

CLP3Vector
CLP3Vector::resize (double newMag)
{
   if (zero() == *this)
   {
      cout << "Error: CLP3Vector::resize(): Vector has zero length." << endl;
      return zero();
   }
   // since we know mag() is not 0
   return (*this) * (newMag / mag());
}

double
CLP3Vector::phi() const
{
   if ((0 == x()) && (0 == y()))
   {
      cout << "Error: CLP3Vector::phi(): Vector has no transverse component." 
           << endl;
      return kErrorValue;
   }
   return atan2(y(), x());
}

double
CLP3Vector::theta() const
{
   if (zero() == *this)
   {
      cout << "Error: CLP3Vector::theta(): Vector has zero length." << endl;
      return kErrorValue;
   }
   // we know we have a magnitude
   return acos (z() / mag());
}

double
CLP3Vector::eta() const
{
   if (zero() == *this)
   {
      cout << "Error: CLP3Vector::eta(): Vector has zero length." << endl;
      return kErrorValue;
   }
   return theta2eta(theta());
}

double
CLP3Vector::etaPhiDiff (const CLP3Vector& other) const
{
   double myPhi = phi();
   double otherPhi = other.phi();
   if ((kErrorValue == myPhi) || (kErrorValue == otherPhi))
   {
      cout << "Error: CLP3Vector::etaPhiDiff: "
           << "One of the vectors has no transverse component." 
           << endl;
      return kErrorValue;
   }
   double myEta = eta();
   double otherEta = other.eta();
   return addInQuad (deltaPhi (myPhi, otherPhi), myEta - otherEta);
}

double
CLP3Vector::phiDiff (const CLP3Vector& other) const
{
   double myPhi = phi();
   double otherPhi = other.phi();
   if ((kErrorValue == myPhi) || (kErrorValue == otherPhi))
   {
      cout << "Error: CLP3Vector::phiDiff: "
           << "One of the vectors has no transverse component." 
           << endl;
      return kErrorValue;
   }
   return deltaPhi (myPhi, otherPhi);
}

CLP3Vector
CLP3Vector::transverse() const
{
   return CLP3Vector (x(), y(), 0);
}

CLP3Vector
CLP3Vector::unit() const
{
   double length = mag();
   if (0 == length)
   {
      cout << "Warning: CLP3Vector::unit(): Can not produce "
           << "a unit vector from a zero length vector" << endl;
      return zero();
   }
   return CLP3Vector ( x() / length, 
                       y() / length, 
                       z() / length );
}

CLP3Vector
CLP3Vector::transUnit() const
{
   double length = transMag();
   if (0 == length)
   {
      cout << "Warning: CLP3Vector::transUnit: Can not produce "
           << "a unit vector from a zero length vector" << endl;
      return zero();
   }
   return CLP3Vector ( x() / length, 
                       y() / length,
                       0);
}

double
CLP3Vector::cosAngle (const CLP3Vector& other) const
{
   if ((zero() == *this) || (zero() == other))
   {
      cout << "Error: CLP3Vector::cosAngle(): "
           << "One of the vector has zero length." << endl;
      return kErrorValue;
   }
   return unit() * other.unit();
}

// operator functions

bool
CLP3Vector::operator== (const CLP3Vector &other) const 
{
   return (x() == other.x()) && (y() == other.y()) && (z() == other.z());
}

bool
CLP3Vector::operator!= (const CLP3Vector &other) const 
{
   return (x() != other.x()) || (y() != other.y()) || (z() != other.z());
}

CLP3Vector 
CLP3Vector::operator+ (const CLP3Vector &vec) const 
{
   return CLP3Vector(x() + vec.x(),  y() + vec.y(), z() + vec.z());
}

const CLP3Vector 
CLP3Vector::operator+= (const CLP3Vector &vec)
{
   *this = *this + vec;
   return *this;
}

CLP3Vector 
CLP3Vector::operator- (const CLP3Vector &vec) const 
{
   return CLP3Vector(x() - vec.x(),  y() - vec.y(), z() - vec.z());
}

const CLP3Vector 
CLP3Vector::operator-= (const CLP3Vector &vec)
{
   *this = *this - vec;
   return *this;
}

const CLP3Vector
CLP3Vector::operator* (double factor) const 
{
   return CLP3Vector(x() * factor,  y() * factor, z() * factor);
}

const CLP3Vector 
CLP3Vector::operator*= (double factor)
{
   *this = *this * factor;
   return *this;
}

double
CLP3Vector::operator* (const CLP3Vector &vec) const 
{
   return x() * vec.x() + y() * vec.y() + z() * vec.z();
}

double
CLP3Vector::mag2() const
{
   return (*this) * (*this);
}

double
CLP3Vector::mag() const
{
   return sqrt(mag2());
}

double
CLP3Vector::transMag() const
{
   return addInQuad( x(), y() );
}

TVector3
CLP3Vector::tvector3() const
{
   return TVector3 (x(), y(), z());
}

// Static Member functions

const CLP3Vector
&CLP3Vector::zero()
{
   if ( ! sm_zero3VecPtr)
   {
      sm_zero3VecPtr = new CLP3Vector(0., 0., 0.);
   }
   return *sm_zero3VecPtr;
}

CLP3Vector
CLP3Vector::fromEtaPhiMag (double eta, double phi, double magnitude)
{
   double theta = eta2theta(eta);
   return CLP3Vector (magnitude * cos (phi) * sin (theta),
                      magnitude * sin (phi) * sin (theta),
                      magnitude * cos (theta));
}

CLP3Vector
CLP3Vector::fromEtaPhiTrans (double eta, double phi, double trans)
{
   double theta = eta2theta(eta);
   return CLP3Vector (trans * cos (phi),
                      trans * sin (phi),
                      trans * cos (theta) / sin (theta));
}

CLP3Vector
CLP3Vector::fromThetaPhiMag (double theta, double phi, double magnitude)
{
   return CLP3Vector (magnitude * cos (phi) * sin (theta),
                      magnitude * sin (phi) * sin (theta),
                      magnitude * cos (theta));
}

CLP3Vector
CLP3Vector::fromEtaPhiZed (double eta, double phi, double zed)
{
   double theta = eta2theta(eta);
   double magnitude = fabs( zed / cos(theta) );
   return CLP3Vector (magnitude * cos (phi) * sin (theta),
                      magnitude * sin (phi) * sin (theta),
                      magnitude * cos (theta));
}

string
CLP3Vector::stringRep() const
{
   return Form ("%.3f %.3f %.3f", x(), y(), z());
}

// friends
ostream& operator<< (ostream& o_stream, const CLP3Vector &rhs)
{
   if (CLP3Vector::kStandardOutputMode == CLP3Vector::output3Mode())
   {
      // x, y, z
      o_stream << "("  << Form ("%6.2f", rhs.x() )
               << ", " << Form ("%6.2f", rhs.y() )
               << ", " << Form ("%6.2f", rhs.z() )
               << ")";
   } 
   else 
   {
      o_stream << "("  << Form ("%6.2f", rhs.transMag())
               << "; " << Form ("%5.2f", rhs.eta())
               << ", " << Form ("%5.2f", rhs.phi())
               << ")";
   }
   return o_stream;
} 
