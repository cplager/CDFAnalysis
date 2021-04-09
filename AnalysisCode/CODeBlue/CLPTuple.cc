// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include "dout.h"
#include "CLPTuple.h"

using namespace std;

CLPTuple::CLPTuple()
{
   m_histoIsCurrent = false;
   m_lower = m_upper = m_numBins = 0;
   m_min = m_max = -1;
   m_binsSet = false;
}

void
CLPTuple::fill (double value)
{
   m_histoIsCurrent = false;
   //m_binsSet = false;
   if (0 == m_values.size())
   {
      m_min = m_max = value;
   } else if (value < m_min)
   {
      m_min = value;
   } else if (value > m_max)
   {
      m_max = value;
   }
   m_values.push_back(value);
}

void
CLPTuple::guessBins() const
{
   // I'm assuming that we are dealing with non-negative integer numbers
   //assert (m_max + 0.5 > 1);
   if (m_min >= 0) 
   {
      m_lower = (int) m_min;
   } else {
      m_lower = (int) m_min - 1;
   }
   m_upper = (int) m_max + 1;
   // If number of bins has been already set, don't change it
   if (0 == m_numBins)
   {
      m_numBins = (unsigned int) (100 * (m_upper - m_lower));
   }
   // Do we want to make sure there aren't too many?
   // anyway, tell the world you're done
   m_deltaX = (m_upper - m_lower) / m_numBins;
   m_binsSet = true;
}

void 
CLPTuple::clear()
{
   m_histoIsCurrent = false;
   m_lower = m_upper = m_numBins = 0;
   m_min = m_max = -1;
   m_binsSet = false;
   m_values.clear();
}

const CLPHisto&
CLPTuple::hist() const
{
   if (! m_histoIsCurrent)
   {
      _fillHist();
   }
   return m_hist;
}

void
CLPTuple::_fillHist() const
{
   // If we don't have bins already, get some
   if (! m_binsSet)
   {
      guessBins();
   }
   m_hist.setNumBinLowerUpper (m_numBins, m_lower, m_upper);
   // loop over values and fill histogram
   unsigned int size = m_values.size();
   for (unsigned loop = 0; loop < size; ++loop)
   {
      m_hist.fill ( m_values[loop] );
   } // for loop
   m_histoIsCurrent = true;
}

void 
CLPTuple::setNumBinLowerUpper (unsigned int numBins,
                               double lower, double upper)
{
   m_lower = lower;
   m_upper = upper;
   m_numBins = numBins;
   m_deltaX = (m_upper - m_lower) / m_numBins;
   m_binsSet = true;
   assert (m_deltaX != 0);
}

void 
CLPTuple::setNumBins (unsigned int numBins)
{
   m_numBins = numBins;
}

TH1F* 
CLPTuple::RootHisto (string idname, string name) const
{
   TH1F* retval;
   if (name == "") 
   {
      name = idname;
   }
   retval = new TH1F (idname.c_str(), name.c_str(), 
                      m_numBins, m_lower, m_upper);
   unsigned int size = m_values.size();
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      retval->Fill ( m_values[loop] );
   } // for loop
   return retval;
}

// friends
ostream& operator<< (ostream& o_stream, const CLPTuple &rhs)
{
   o_stream << "(" << rhs.m_min << ", " << rhs.m_max 
            << "; " << rhs.m_values.size() << ") " << rhs.m_values;
   return o_stream;
} 
