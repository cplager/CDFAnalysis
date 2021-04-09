// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "TMatrixD.h"
#include "StringUtils.h"
#include "CLPBlue.h"
#include "dout.h"

using namespace std;

//////////////////////
// Member Functions //
//////////////////////
CLPBlue::CLPBlue()
{
   m_numMeasurements = 0;
   m_numErrors = 0;
   initialize();
}

CLPBlue::CLPBlue(unsigned int numMeasurements, unsigned int numErrors)
{
   setNumMeasurements (numMeasurements);
   setNumErrors (numErrors);
   initialize();
}

void
CLPBlue::initialize()
{
   DVec zeroVec;
   zeroVec.push_back(0.);
   // zero everything (except diagonal row of of correlation matrix
   for (unsigned int mInd1 = 0; mInd1 < kMaxMeasurements; ++mInd1)
   {
      m_totalError[mInd1] = 0.;
      m_mean[mInd1] = -1.; // an error value
      for (unsigned int eInd1 = 0; eInd1 < kMaxErrors; ++eInd1)
      {
         m_errorVec[mInd1][eInd1] = zeroVec;
         for (unsigned int mInd2 = 0; mInd2 < kMaxMeasurements; ++mInd2)
         {
            if (mInd2 == mInd1)
            {
               // on diagonal
               m_correlations[mInd1][mInd2][eInd1] = 1.0;
            } else {
               // off diagonal
               m_correlations[mInd1][mInd2][eInd1] = 0.0;
            }
         } // for mInd2
      } // for eInd1
   } // for mInd1
}

void
CLPBlue::setNumMeasurements (unsigned int numMeasurements)
{
   assert (numMeasurements <= kMaxMeasurements);
   m_numMeasurements = numMeasurements;
}

void
CLPBlue::setNumErrors (unsigned int numErrors)
{
   assert (numErrors <= kMaxErrors);
   m_numErrors = numErrors;
}

void
CLPBlue::setName (unsigned int mInd1, string name)
{
   assert (mInd1 < m_numMeasurements);
   m_names[mInd1] = name;
}

void
CLPBlue::setMean (unsigned int mInd1, double mean)
{
   assert (mInd1 < m_numMeasurements);
   m_mean[mInd1] = mean;
}

void
CLPBlue::setErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                           const DVec &value)
{
   assert (mInd1 < m_numMeasurements);
   assert (eInd1 < m_numErrors);
   m_errorVec[mInd1][eInd1] = value;
}

void
CLPBlue::setCorrelation (unsigned int mInd1, unsigned int mInd2, 
                         unsigned int eInd1, double value)
{
   assert (mInd1 < m_numMeasurements);
   assert (mInd2 < m_numMeasurements);
   assert (eInd1 < m_numErrors);
   // make it symmetric
   m_correlations[mInd1][mInd2][eInd1] 
      = m_correlations[mInd2][mInd1][eInd1] = value;
}

void
CLPBlue::calcTotalErrors(double mean)
{
   for (unsigned int mInd1 = 0; mInd1 < m_numMeasurements; ++mInd1)
   {
      double sum2 = 0.;
      for (unsigned int eInd1 = 0; eInd1 < m_numErrors; ++eInd1)
      {
         double err = error (mInd1, eInd1, mean);
         sum2 += err * err;
      } // for err
      m_totalError[mInd1] = sqrt(sum2);
   } // for mInd1
}

double
CLPBlue::error (unsigned int mInd1, unsigned int eInd1, double mean)
{
   assert (mInd1 < m_numMeasurements);
   assert (eInd1 < m_numErrors);
   if (mean < 0)
   {
      mean = m_mean[mInd1];
   }
   double xN = 1.; // start at X^0
   unsigned int size = m_errorVec[mInd1][eInd1].size();
   double retval = 0.;
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      retval += m_errorVec[mInd1][eInd1][loop] * xN;
      xN *= mean; // go from x^n to x^(n+1)
   } // for size;
   return retval;
}

double
CLPBlue::correlations (unsigned int mInd1, unsigned int mInd2, 
                       unsigned int eInd1)
{
   assert (mInd1 < m_numMeasurements);
   assert (mInd2 < m_numMeasurements);
   assert (eInd1 < m_numErrors);
   return m_correlations[mInd1][mInd2][eInd1];
}

bool
CLPBlue::isValid()
{
   // make sure all means are possitive
   for (unsigned int loop = 0; loop < m_numMeasurements; ++loop)
   {
      if (m_mean[loop] < 0)
      {
         return false;
      }
   } // for loop
   return true;
}

void
CLPBlue::readFromFile(const string &filename)
{
   ifstream source(filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "file " << filename << "could not be opened" << endl;
      assert(0);
   }
   cerr << "Loading file " << filename << endl;
   string line;
   while (getline (source, line))
   {
      SVec sList;
      DVec values;
      split (sList, line);
      unsigned int size = sList.size();
      if (size < 1)
      {
         continue;
      }
      // size must be at least 1

      if (size < 2)
      {
         continue;
      }
      // size must be at least 1
      if ("numMeasurements" == sList[0])
      {
         unsigned int value = atoi( sList[1].c_str() );
         setNumMeasurements (value);
         continue;
      }
      if ("numErrors" == sList[0])
      {
         unsigned int value = atoi( sList[1].c_str() );
         setNumErrors (value);
         continue;
      }
      if (size < 3)
      {
         continue;
      }
      // size must be at least 3
      if ("name" == sList[0])
      {
         unsigned int mInd1 = atoi( sList[1].c_str() );
         setName (mInd1, sList[2]);
         continue;
      }
      if ("mean" == sList[0])
      {
         unsigned int mInd1 = atoi( sList[1].c_str() );
         double value = atof( sList[2].c_str() );
         setMean (mInd1, value);
         continue;
      }
      if (size < 4)
      {
         continue;
      }
      // size must be at least 4
      if ("errorVec" == sList[0])
      {
         unsigned int mInd1 = atoi( sList[1].c_str() );
         unsigned int eInd1 = atoi( sList[2].c_str() );
         DVec value;
         for (unsigned int loop = 3; loop < size; ++loop)
         {
            value.push_back( atof( sList[loop].c_str() ) );
         } // for loop 3..size - 1
         setErrorFraction (mInd1, eInd1, value);
         continue;
      }
      if (size < 5)
      {
         continue;
      }
      // size must be at least 5
      if ("correlation" == sList[0])
      {
         unsigned int mInd1 = atoi( sList[1].c_str() );
         unsigned int mInd2 = atoi( sList[2].c_str() );
         unsigned int eInd1 = atoi( sList[3].c_str() );
         double value = atof( sList[4].c_str() );
         setCorrelation (mInd1, mInd2, eInd1, value);
         continue;
      }
      
   } // while getline
   calcTotalErrors();
}

void
CLPBlue::calcWeightedAverage (double &mean, double &err,
                              double errMean)
{
   calcTotalErrors(errMean);
   double weightSum = 0;
   mean = 0;
   for (unsigned int mInd1 = 0; mInd1 < m_numMeasurements; ++mInd1)
   {
      assert (m_totalError[mInd1]);
      double weight = m_weight[mInd1] 
         = 1 / (m_totalError[mInd1] * m_totalError[mInd1]);
      weightSum += weight;
      mean += weight * m_mean[mInd1];
   } // for mInd1
   for (unsigned int mInd1 = 0;  mInd1 < m_numMeasurements; ++mInd1)
   {
      m_weight[mInd1] /= weightSum;
   } // for mInd1
   mean /= weightSum;
   err = 1. / sqrt (weightSum);
}

void
CLPBlue::calcIterativeBlueAverage (double &mean, double &error)
{
   // first get weighted average
   calcWeightedAverage (mean, error);
   double oldmean = 0;
   int count = 0;
   while ( (fabs (oldmean - mean) > 0.01) && (++count < 100))
   {
      oldmean = mean;
      calcBlueAverage(mean, error, oldmean);
      //cout << "  mean " << mean << " +- " << error << endl;
   } // while
}

void
CLPBlue::calcBlueAverage (double &mean, double &err,
                          double errMean)
{
   calcTotalErrors(errMean);
   // Calculate S matrix
   TMatrixD sMatrix (m_numMeasurements, m_numMeasurements);
   for (unsigned int mInd1 = 0; mInd1 < m_numMeasurements; ++mInd1)
   {
      for (unsigned int mInd2 = mInd1; mInd2 < m_numMeasurements; ++mInd2)
      {
         double sum = 0.;
         for (unsigned int eInd1 = 0; eInd1 < m_numErrors; ++eInd1)
         {
            double err1 = error (mInd1, eInd1, errMean);
            double err2 = error (mInd2, eInd1, errMean);
            sum += correlations (mInd1, mInd2, eInd1) * err1 * err2;
         } // for eInd1
         sMatrix (mInd1, mInd2) = sMatrix (mInd2, mInd1) = sum;
      } // for mInd2
   } // for mInd1
   // Get hMatrix = sMatrix^-1
   TMatrixD hMatrix (TMatrixD::kInverted, sMatrix);
   double sumMatrix = 0.;
   for (unsigned int mInd1 = 0; mInd1 < m_numMeasurements; ++mInd1)
   {
      for (unsigned int mInd2 = 0; mInd2 < m_numMeasurements; ++mInd2)
      {
         sumMatrix += hMatrix (mInd1, mInd2);
      } // for mInd2
   } // for mInd1
   assert (sumMatrix);
   err = 1 / sqrt (sumMatrix);
   mean = 0.;
   double weightSum = 0.;
   for (unsigned int mInd1 = 0; mInd1 < m_numMeasurements; ++mInd1)
   {
      double weight = 0;
      for (unsigned int mInd2 = 0; mInd2 < m_numMeasurements; ++mInd2)
      {
         weight += hMatrix (mInd1, mInd2);
      } // for mInd2      
      m_weight[mInd1] = weight;
      weightSum += weight;
      mean += weight * m_mean[mInd1];
   } // for mInd1
   assert (weightSum); // make sure it's not 0
   for (unsigned int mInd1 = 0;  mInd1 < m_numMeasurements; ++mInd1)
   {
      m_weight[mInd1] /= weightSum;
   } // for mInd1
   mean /= weightSum;
}

string
CLPBlue::name(unsigned int mInd) const
{
   assert (mInd < m_numMeasurements);
   return m_names[mInd];
}

DVec
CLPBlue::getWeights() const
{
   DVec retval;
   for (unsigned int mInd1 = 0; mInd1 < m_numMeasurements; ++mInd1)
   {
      retval.push_back( m_weight[mInd1] );
   } // for mInd1
   return retval;
}

void
CLPBlue::outputWeights (ostream& o_stream) const
{
   o_stream << "Weights:  ";
   for (unsigned int mInd1 = 0; mInd1 < m_numMeasurements; ++mInd1)
   {
      o_stream << m_weight[mInd1] << "  ";
   } // for mInd1
   o_stream << endl;
}

// friends
ostream& operator<< (ostream& o_stream, const CLPBlue &rhs)
{
   for (unsigned int mInd1 = 0; mInd1 < rhs.m_numMeasurements; ++mInd1)
   {
      o_stream << "Name: " << rhs.m_names[mInd1] << endl
               << "mean: " << rhs.m_mean[mInd1] << endl
               << "tErr: " << rhs.m_totalError[mInd1] << endl << endl;
   } // for mInd1
   return o_stream;
} 

ostream& operator<< (ostream& o_stream, const TMatrixD &rhs)
{
   int numRows = rhs.GetNrows();
   int numCols = rhs.GetNcols();
   for (int row = 0; row < numRows; ++row)
   {
      for (int col = 0; col < numCols; ++col)
      {
         char line[100];
         sprintf (line, "%9.4f ", rhs(row, col));
         o_stream << line;
      } // for col
      o_stream << endl;
   } // for row
   return o_stream;
}
