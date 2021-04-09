// -*- C++ -*-

#if !defined(CLPUtils_H)
#define CLPUtils_H

#include "CLPTypeDef.h"

// calculate pull value given true value, measured value, and either
// error or plus and minus error
extern double pullValue (double trueValue, double measured,
                         double plus, double minus = 0);

// given two vectors of equal size, this function returns 
// the correlation coefficient
extern double correlationCoeff (DVec &xvec, DVec &yvec);

// adds up to five numbers in quadrature
double addInQuad(double one, double two, double three = 0.,
                 double four = 0., double five = 0.);


#endif // CLPUtils_H
