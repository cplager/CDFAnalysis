// -*- C++ -*-

#if !defined(CLP3Vector_H)
#define CLP3Vector_H

#include "TSystem.h"
#include "TVector3.h"
#include <string>

class CLP3Vector : public TObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      static CLP3Vector *sm_zero3VecPtr;

      // typedef
      typedef std::vector< CLP3Vector > V3List;

      enum OutputModeType 
      {
         kStandardOutputMode, // x, y, z
         kPtEtaPhiOutputMode, // pt; eta, phi
      };

      /////////////
      // friends //
      /////////////

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLP3Vector ();
      CLP3Vector (double x, double y, double z);
      CLP3Vector (const CLP3Vector &vec);
      CLP3Vector (const TVector3 &vec);

      ///////////////////////
      // Operator overloads//
      ///////////////////////

      // three vector addition
      CLP3Vector operator+ (const CLP3Vector& vec) const;
      const CLP3Vector operator+= (const CLP3Vector& vec);
      // three vector subtraction
      CLP3Vector operator- (const CLP3Vector& vec) const;
      const CLP3Vector operator-= (const CLP3Vector& vec);
      // scale vector
      const CLP3Vector operator* (double factor) const;
      const CLP3Vector operator*= (double factor);
      // dot product
      double operator* (const CLP3Vector& vec) const;
      // equality
      bool operator== (const CLP3Vector& other) const;
      // inequality
      bool operator!= (const CLP3Vector& other) const;
      // element accessor
      virtual double operator[](const int index) const;

       ////////////////
      // One Liners //
      ////////////////
      double x() const { return m_x; }
      double y() const { return m_y; }
      double z() const { return m_z; }
      double Px() const { return m_x; }
      double Py() const { return m_y; }
      double Pz() const { return m_z; }
      void setX(double x) { m_x = x; }
      void setY(double y) { m_y = y; }
      void setZ(double z) { m_z = z; }
      double pt() const { return transMag(); }
      double Pt() const { return transMag(); }
      double Et() const { return transMag(); }
      bool isZero() const { return zero() == *this; }
      // setXYZ is useful for 'CLP4Vector's in particular
      void setXYZ (const CLP3Vector &rhs) { *this = rhs; }
      
      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // returns a new vector with the new size but keeping same 
      // direction negative numbers allowed (will flip direction)
      CLP3Vector resize(double newMag);

      // angles of vector
      double phi() const;
      double theta() const;
      double eta() const;
      // cosine of angle between this vector and other vector
      double cosAngle (const CLP3Vector& other) const;
      // eta, phi difference added in quadrature between
      // this and other vector
      double etaPhiDiff (const CLP3Vector& other) const;
      // phi difference between this and other vector
      // this quantity is signed ( = other.phi() - this.phi())
      double phiDiff (const CLP3Vector& other) const;

      // magnitudes
      // magnitude of vector
      double mag() const;
      // magnitude ^ 2 of vector
      double mag2() const;
      // magnitude of transverse vector
      double transMag() const;

      // transverse components only
      CLP3Vector transverse() const;

      // unit vector pointing in the same direction
      CLP3Vector unit() const;

      // unit vector pointing in transverse plane
      CLP3Vector transUnit() const;

      // return a string represenstation of the vector
      virtual std::string stringRep() const;

      // returns a TVector3
      TVector3 tvector3() const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

      // returns a const reference to the 0 vector
      static const CLP3Vector &zero();

      // returns a three vector given eta, phi, and the magnitude
      // Will create a unit vector if only eta and phi are given
      static CLP3Vector fromEtaPhiMag (double eta, double phi, 
                                       double magnitude = 1);
      // returns a three vector given eta, phi, and TRANSVERSE magnitue
      static CLP3Vector fromEtaPhiTrans (double eta, double phi, 
                                         double trans);
      // returns a three vector given theta, phi, and magnitude
      // Will create a unit vector if only eta and phi are given
      static CLP3Vector fromThetaPhiMag (double theta, double phi,
                                         double magnitude = 1);
      // returns a three vector given eta, phi, and Z
      static CLP3Vector fromEtaPhiZed (double eta, double phi,
                                       double zed);

      static void setOutput3Mode (OutputModeType mode)
      { sm_outputMode = mode; }

      static OutputModeType output3Mode()
      { return sm_outputMode; }

  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////
      double m_x, m_y, m_z;

      static OutputModeType sm_outputMode;

      ClassDef (CLP3Vector, 1) // CLP 3 vector class
};

////////////////////////
// ostream Definition //
////////////////////////
extern std::ostream& operator<< (std::ostream& o_stream, 
                                 const CLP3Vector &right);



#endif // CLP3Vector_H
