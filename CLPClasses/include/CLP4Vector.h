// -*- C++ -*-

#if !defined(CLP4Vector_H)
#define CLP4Vector_H

#include <vector>

#include "TLorentzVector.h"

#include "CLP3Vector.h"

class CLP4Vector : public CLP3Vector
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      static const double kErrorMass;

      // typedef
      typedef std::vector< CLP4Vector > V4List;

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
      CLP4Vector ();
      CLP4Vector (double x, double y, double z, double t);
      CLP4Vector (const TLorentzVector &vec);
      CLP4Vector (const CLP3Vector &vec, double t);
      CLP4Vector (const CLP3Vector &vec);
      CLP4Vector (const CLP4Vector &vec);

       ////////////////
      // One Liners //
      ////////////////
      CLP3Vector threeVector() const { return (CLP3Vector) *this; }
      double t()  const { return m_time; }
      double e()  const { return m_time; }
      double E()  const { return m_time; }

      void setT(double time) { m_time = time; }
      void setE(double time) { m_time = time; }

      // should the scaling conserve mass or scale everything?
      static void setScaleEverything (bool flag) { sm_scaleEverything = flag; }
      
      ////////////////////////
      // Operator overloads //
      ////////////////////////

      // add two four vectors
      CLP4Vector operator+(const CLP4Vector& vec) const;
      // dot product
      double operator*(const CLP4Vector& vec) const;      
      // Scaling a four vector:
      // There are two options:
      //  - scale all four components
      //  - scale the three vector and change the energy so that the
      //    rest mass stays the same.
      // I have set this up to do the former (scale all four
      // components) because of how jets masses are calculated.
      // Operator* and scale are the same (all four components)
      CLP4Vector operator*(double factor) const;
      CLP4Vector scaleEverything (double factor) const;
      const CLP4Vector operator*= (double factor);
      // Returns a 4 vector with all four components multiplied by the
      // same factor


      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // set the energy based on the already existing 3 vector
      // and the given mass
      void setRestMass(double mss);

      // modify 4 vector by boost b
      void boost (const CLP3Vector &vec);

      // return the boost (beta) vector 
      CLP3Vector boostVector () const;


      // magnitude of vector
      double mass() const;

      // mass ^ 2 of vector
      double mass2() const;

      // return a string represenstation of the vector
      std::string stringRep() const;

      // returns a TLorentzVector
      TLorentzVector tlorentz() const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      static CLP4Vector from3VectorMass(const CLP3Vector& vector,
                                        double mass);


  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////
      double m_time;

      static bool sm_scaleEverything;

      ClassDef (CLP4Vector, 1) // CLP 4 vector class

};

////////////////////////
// ostream Definition //
////////////////////////
extern std::ostream& operator<< (std::ostream& o_stream, 
                                 const CLP4Vector &right);

#endif // CLP4Vector_H
