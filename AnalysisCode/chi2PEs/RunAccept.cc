// -*- C++ -*-
#include <iostream>
#include "RunAccept.hh"

using namespace std;

RunAccept::RunAccept () : m_paramVec ()
{
   // setup a default that does something reasonable
   m_paramVec.push_back (1);
   m_paramVec.push_back (0);
   m_paramVec.push_back (1);
   m_paramVec.push_back (0);
   m_paramVec.push_back (0);
}

RunAccept::RunAccept (const DVec &paramVec) : m_paramVec (paramVec)
{
   assert (paramVec.size() >= 5);
}

void
RunAccept::setParamVec (const DVec &paramVec)
{
   assert (paramVec.size() >= 5);
   m_paramVec = paramVec;
}

double
RunAccept::operator() (double value) const
{
   bool flipSign = false;
   if (value < 0)
   {
      flipSign = true;
      value *= -1;
   }
   double numerator =
      m_paramVec.at(0) * value +
      m_paramVec.at(1) * value * value;
   double denominator =
      m_paramVec.at(2) +
      m_paramVec.at(3) * value +
      m_paramVec.at(4) * value * value;
   
   // if (value < 0)
   // {
   //    numerator = m_paramVec.at(0) * value;
   //    denominator = m_paramVec.at(2);
   // }

   //cout << "numerator " << numerator << " denom " << denominator << endl;

   if (fabs (denominator) < 1e-6)
   {
      denominator = 1e-6;
   }

   if (flipSign)
   {
      numerator *= -1;
   }
   return numerator / denominator;
}

double
RunAccept::operator() (int tempIndex, int binIndex,
                       double value, const double *paramArray) const
{
   return operator() (value);
}
