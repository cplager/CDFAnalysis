// -*- C++ -*-
#include <iostream>
#include "TagFraction.hh"

using namespace std;

TagFraction::TagFraction () : m_tagFracIndex (-1)
{
}

double
TagFraction::operator() (int tempIndex, int binIndex,
                         double value, const double *paramArray) const
{
   // Real constants
   const double &tagFrac = paramArray[m_tagFracIndex];
   // if this is the anti-tag region, return 1 - the tag fraction
   if (setContains (binIndex))
   {
      return 1 - tagFrac;
   } else {
      // If we're here, we must be in the tag region, so return the
      // tag fraction.
      return tagFrac;
   }
}
