// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include "dout.h"
#include "CLPHisto.h"
#include "TCanvas.h"

using namespace std;

// static variables
bool CLPHisto::m_noMinInterval = false;

CLPHisto::CLPHisto()
{
   m_numBins = 0;
   m_total = 0;
}

CLPHisto::CLPHisto(unsigned int numBins, double lower, double upper)
{
   setNumBinLowerUpper (numBins, lower, upper);
   m_total = 0;
}

CLPHisto::CLPHisto(TH1F *hPtr)
{
   double lower = hPtr->GetXaxis()->GetXmin();
   double upper = hPtr->GetXaxis()->GetXmax();
   int numBins = hPtr->GetNbinsX();
   setNumBinLowerUpper (numBins, lower, upper);
   m_total = 0;
   for (Int_t loop = 0; loop <= numBins + 1; ++loop)
   {
      Double_t contents = hPtr->GetBinContent(loop);
      m_total += contents;
      m_array[loop] = contents;
   } // for loop
}

void
CLPHisto::fill (double xValue, double weight)
{
   m_array[ xToBin(xValue) ] += weight;
   m_total += weight;
}

double
CLPHisto::bin (unsigned int index) const
{
   assert (index <= m_numBins + 1); 
   return m_array[index];
}

void 
CLPHisto::setNumBinLowerUpper (unsigned int numBins,
                               double lower, double upper)
{
   m_lower = lower;
   m_upper = upper;
   assert (m_lower < m_upper);
   m_numBins = numBins;
   m_deltaX = (m_upper - m_lower) / m_numBins;
   m_total = 0;
   assert (m_deltaX != 0);
   // setup vector now
   m_array.clear();
   for (unsigned int loop = 0; loop <= m_numBins + 1; ++loop)
   {
      m_array.push_back(0.);
   } // for loop
}

double
CLPHisto::xToBinD (double xValue, bool asIntegrated) const
{
   // is this an underflow?
   if (xValue <= m_lower)
   {
      return -1; // something too small
   }
   // is this an overflow?
   if (xValue > m_upper)
   {
      return m_numBins + 2; // something too big
   }
   // just a normal ....
   if (asIntegrated)
   {
      // return so that the upper edge of the first bin is 1
      return  (xValue - m_lower) / m_deltaX;
   } else {
      // return so that the middle of the first bin is 1
      return 0.5 + (xValue - m_lower) / m_deltaX;
   }
}

unsigned int
CLPHisto::xToBin (double xValue) const
{
   if (xValue < m_lower)
   {
      return 0;
   }
   // is this an overflow?
   if (xValue >= m_upper)
   {
      return m_numBins + 1;
   }
   // just a normal ....
   return 1 + (int) ((xValue - m_lower) / m_deltaX);
}

double
CLPHisto::binToX (double bin, bool asIntegrated) const
{
   // is this an underflow
   if (bin < 0)
   {
      return m_lower - m_deltaX;
   }
   if (bin >= m_numBins + 1)
   {
      return m_upper + m_deltaX;
   }
   if (asIntegrated)
   {
      return m_lower + bin * m_deltaX;
   } else {
      return m_lower + (bin - 0.5) * m_deltaX;
   }
}

double
CLPHisto::max (double &maxX, double &maxY) const
{
   maxX = 0;
   maxY = -1; // something very "small"
   int maxXindex = 0;
   for (unsigned int loop = 1; loop <= m_numBins; ++loop)
   {
      if (m_array[loop] > maxY)
      {
         maxXindex = loop;
         maxY = m_array[loop];
      }
   } // for loop
   maxX = binToX (maxXindex);
   return maxY;
}

double
CLPHisto::min (double &minX, double &minY) const
{
   minX = 0;
   minY = 1e20; // something very "small"
   int minXindex = 0;
   for (unsigned int loop = 1; loop <= m_numBins; ++loop)
   {
      if (m_array[loop] < minY)
      {
         minXindex = loop;
         minY = m_array[loop];
      }
   } // for loop
   minX = binToX (minXindex);
   return minY;
}

void
CLPHisto::find (double yValue, DVec &retval, bool asIntegrated) const
{
   retval.clear();
   for (unsigned int loop = 1; loop < m_numBins; ++loop)
   {
      double yLower = m_array[loop];
      double yUpper = m_array[loop + 1];
      int xLowerIndex = loop;
      int xUpperIndex = loop + 1;
      // we want yLower to be lower than yUpper
      if (yLower > yUpper)
      {
         double temp = yLower;
         yLower = yUpper;         yUpper = temp;
         int iTemp = xLowerIndex;
         xLowerIndex = xUpperIndex;
         xUpperIndex = iTemp;
      }
      if ((yLower <= yValue) && (yValue < yUpper))
      {
         double xLower = binToX(xLowerIndex, asIntegrated);
         double xUpper = binToX(xUpperIndex, asIntegrated);
         double yDelta = yUpper - yLower;
         double percent = (yValue - yLower) / yDelta;
         double xDelta = xUpper - xLower;
         double xValue = xLower + percent * xDelta;
         retval.push_back(xValue);
      }
   } // for loop
   // if we are doing this 'asIntegrated' and we don't find anything,
   // we need to either assign the lower edge or the upper edge
   if (!retval.size() && asIntegrated)
   {
      // is yValue smaller than the first integrated bin
      if (yValue < m_array[1])
      {
         retval.push_back(m_lower);
      } else {
         retval.push_back(m_upper);
      }
   }
}

double
CLPHisto::interpolate (double xValue, bool asIntegrated) const
{
   double binD = xToBinD(xValue, asIntegrated);
   if (binD < 0)
   {
      // underflow
      if (asIntegrated) 
      {
         // return the underflow bin
         return m_array[0];
      } else {
         // we don't know; return 0
         return 0;
      }
   }
   if (binD >= m_numBins + 1)
   {
      // overflow
      if (asIntegrated)
      {
         // return overflow bin
         return m_array[m_numBins + 1];
      } else {
         // we don't know
         return 0;
      }
   }
   // If we're still here, just interpolate
   int bin = (int) binD;
   double remainder = binD - bin;
   double retval = m_array[bin];
   // correct for remainder
   retval += remainder * (m_array[bin + 1] - m_array[bin]);
   return retval;
}

void
CLPHisto::findMaxAnd68 (double &maxX, double &lower, double &upper,
                        double &sixteen, double &fifty, double &eightyfour,
                        double desiredValue, double allowedError) const
{
   double maxY;
   max(maxX, maxY);
   //cout << "max " << maxX << ", " << maxY << endl;

   TCanvas c2;
   CLPHisto integ = integrate();
   DVec xValues;
   integ.find(.1586, xValues, true);
   if (!xValues.size())
   {
      cerr << "we don't seem to have a size here" << endl;
   }
   sixteen = xValues[0];
   integ.find(.50, xValues, true);
   fifty = xValues[0];
   integ.find(.8414, xValues, true);
   eightyfour = xValues[0];
   if (m_noMinInterval)
   {
      // don't bother continuing
      return;
   }
   //cout << "hist  " << *this << endl;
   //cout << "integ " << integ << endl;
   double currentValue = 0;
   double lowY = 0.;
   double highY = maxY;
   double currentError = fabs (desiredValue - currentValue); 
   double previousError = 0;
   int loopCounts = 0;
   //cout << "current Error " << currentError << endl;
   while (currentError > allowedError)
   {
      ++loopCounts;
      if ((currentError - previousError) / currentError < 0.00001)
      {
         break;
      }
      if (loopCounts > 10000)
      {
         break;
      }
      //cout << "loop " << loopCounts << " " << currentError << endl;
      previousError = currentError;
      // get a new Y coordinate
      double newY = (lowY + highY) / 2;
      if (newY == lowY)
      {
         break;
      }
      DVec xValues;
      find (newY, xValues);
      //cout << "newY " << newY << " : " << xValues << endl;
      // convert what we got into two choices
      int numLow = 0, numHigh = 0;
      double xLow = 0., xHigh = 0.;
      for (unsigned int loop = 0; loop < xValues.size(); ++loop)
      {
         if (xValues[loop] > maxX)
         {
            ++numHigh;
            xHigh += xValues[loop];
         } else {
            ++numLow;
            xLow += xValues[loop];
         }
      }
      if ((0 == numLow) || (0 == numHigh))
      {
         TH1F* histPtr = RootHisto("badlikeli", 
                                   "Problematic Likelihood Curve");
         histPtr->Draw();
         c2.Print("badLikelihood.eps");
         assert (numLow > 0);
         assert (numHigh > 0);
      }
      lower = xLow / numLow;
      upper = xHigh / numHigh;
      double hiArea = integ.interpolate (upper, true);
      double loArea = integ.interpolate (lower, true);
      currentValue = hiArea - loArea;
      assert (currentValue > 0);
      assert (currentValue < 1);
      // is it too big
      if (currentValue > desiredValue)
      {
         // we have gone down too far
         lowY = newY;
         // leave highY alone
      } else {
         // we haven't gone down far enough
         highY = newY;
         //leave lowY alone
      }
      currentError = fabs (desiredValue - currentValue);
   } // while
   //cout << "integ" << integ << endl;
}

CLPHisto
CLPHisto::integrate() const
{
   assert (m_lower != m_upper);
   assert (m_total > 0);
   CLPHisto retval (m_numBins, m_lower, m_upper);
   double localTotal = 0.;
   for (unsigned int loop = 0; loop <= m_numBins + 1; ++loop)
   {
      localTotal += m_array[loop];
      retval.m_array[loop] = localTotal / m_total;
   }
   return retval;
}

TH1F* 
CLPHisto::RootHisto (string idname, string name) const
{
   TH1F* retval;
   if (name == "") 
   {
      name = idname;
   }
   retval = new TH1F (idname.c_str(), name.c_str(), 
                      m_numBins, m_lower, m_upper);
   for (unsigned int loop = 1; loop <= m_numBins; ++loop)
   {
      double xValue =  binToX(loop);
      double weight = m_array[loop];
      //cout << "x " << xValue << " weight " << weight << endl;
      retval->Fill ( xValue, weight );
   } // for loop
   retval->Fill (m_lower - 0.1, m_array[0]);
   retval->Fill (m_upper + 0.1, m_array[m_numBins + 1]);
   return retval;
}

// friends
ostream& operator<< (ostream& o_stream, const CLPHisto &rhs)
{
   unsigned int numBins = rhs.numBins();
   o_stream << "(" << numBins << ", " << rhs.bin(0) << ", " 
            << rhs.m_lower << "; "<< rhs.bin(numBins + 1)
            << ", " << rhs.m_upper << ") :";
   for (unsigned int loop = 1; loop <= numBins; ++loop)
   {
      o_stream << " " << rhs.bin(loop);
   }
   return o_stream;
} 

