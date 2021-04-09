// -*- C++ -*-
#include <math.h>
#include <assert.h>
#include "CLPUtils.h"

const double kErrorValue = -9999.0;

double pullValue (double trueValue, double measured,
                  double plus, double minus)
{
   if (0 == minus)
   {
      minus = plus;
   } else 
   {
      minus = fabs (minus);
   }
   double delta = measured - trueValue;
   // if delta is positive, that means that measured is bigger than the 
   // true value.  This means that the true value falls in the interval
   // described by measured - minus error and measured.  So use the
   // minus error
   if ((delta > 0) && minus)
   {
      return delta / minus;
   } else if ((delta <= 0) && plus)
   {
      return delta / plus;
   }
   return kErrorValue;
}

double correlationCoeff (DVec &xvec, DVec &yvec)
{
   // correlationCoeff = covariance / (rms1 * rms2) 
   // make sure both vectors are the same lenght
   assert (xvec.size() == yvec.size());
   unsigned int size = xvec.size();
   if (0 == size)
   {
      return 0.;
   } // if 0 size
   double sumX = 0., sumY = 0., sumXY = 0., sumX2 = 0., sumY2 = 0.;
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      double x = xvec[loop];
      double y = yvec[loop];
      sumX  += x;
      sumX2 += x * x;
      sumY  += y;
      sumY2 += y * y;
      sumXY += x * y;
   } // for loop
   double numerator = size * sumXY - sumX * sumY;
   double denomX = size * sumX2 - sumX * sumX;
   double denomY = size * sumY2 - sumY * sumY;
   double denominator = sqrt (denomX * denomY);
   assert (denominator);
   return numerator / denominator;
}

double addInQuad(double one, double two, double three, 
                 double four, double five)
{
   return sqrt (one * one + two * two + three * three
                + four * four + five * five);
}
