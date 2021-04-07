// -*- C++ -*-
#include <iostream>
#include "RunFraction.hh"

using namespace std;

RunFraction::RunFraction () : m_controlIndex (-1), m_fractionIndex (-1),
                              m_jesIndex (-1), m_fractionError (0.10)
{
}

double
RunFraction::operator() (int tempIndex, int binIndex,
                         double value, const double *paramArray) const
{
   return operator() (paramArray [m_controlIndex],
                      paramArray [m_fractionIndex],
                      paramArray [m_jesIndex]);
}

double
RunFraction::operator() (double control, double fraction, 
                         double jes) const
{
   // The real guts go here
   if (jes >  2.5)  jes =  2.5;
   if (jes < -2.5)  jes = -2.5;
   // Parameter come from fit
   // http://www-cdf.fnal.gov/~cplager/internal/Analysis/TopFCNC/log/0801/log.html#080125
   double center =   
         5.07551e-01  
      + -1.18445e-01 * jes
      +  1.63265e-03 * jes * jes 
      +  2.74510e-03 * jes * jes * jes;
   return control * center * (1 + fraction * m_fractionError);
}
