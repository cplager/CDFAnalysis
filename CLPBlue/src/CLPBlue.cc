// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include "TMatrixD.h"
#include "CLPBlue/CLPBlue.hh"
#include "CLPBlue/CLPUtils.hh"
#include "CLPBlue/dout.hh"

using namespace std;
using namespace CLPUtilNamespace;

ClassImp (CLPBlue);

const double CLPBlue::kUseMean = -9e9; // something very negative

//////////////////////
// Member Functions //
//////////////////////
CLPBlue::CLPBlue() : m_numMeasurements (0), m_numErrors (0),
                     m_chi2 (-1.), m_totMean (-1.), m_totError (-1.),
                     m_minimumMean (kUseMean - 1), m_ignoreLoadWarnings (false)
{
   initialize();
}

CLPBlue::CLPBlue (UInt numMeasurements, UInt numErrors)  
   : m_numMeasurements (numMeasurements), m_numErrors (numErrors),
     m_chi2 (-1.), m_totMean (-1.), m_totError (-1.),
     m_minimumMean (kUseMean - 1), m_ignoreLoadWarnings (false)
{
   initialize();
}

void
CLPBlue::initialize()
{
   DVec zeroVec;
   zeroVec.push_back(0.);
   // zero everything (except diagonal row of of correlation matrix
   for (UInt measInd1 = 0; measInd1 < kMaxMeasurements; ++measInd1)
   {
      m_totalError[measInd1] = 0.;
      m_mean[measInd1] = -1.; // an error value
      for (UInt errInd = 0; errInd < kMaxErrors; ++errInd)
      {
         m_errorVec[measInd1][errInd] = zeroVec;
         for (UInt measInd2 = 0; measInd2 < kMaxMeasurements; ++measInd2)
         {
            if (measInd2 == measInd1)
            {
               // on diagonal
               m_correlations[measInd1][measInd2][errInd] = 1.0;
            } else {
               // off diagonal
               m_correlations[measInd1][measInd2][errInd] = 0.0;
            } 
         } // for measInd2
      } // for errInd
   } // for measInd1
   for (UInt measInd1 = 0; measInd1 < kMaxMeasurements; ++measInd1)
   {
      for (UInt measInd2 = 0; measInd2 < kMaxMeasurements; ++measInd2)
      {
         m_totalCorrelations[measInd1][measInd2] = 0.;
      } // for measInd2
   } // for measInd1
   for (UInt errInd = 0; errInd < kMaxErrors; ++errInd)
   {
      m_errorBreakdown[errInd] = -1.;
   } // for errInd
}

void
CLPBlue::setNumMeasurements (UInt numMeasurements)
{
   assert (numMeasurements <= kMaxMeasurements);
   m_numMeasurements = numMeasurements;
}

void
CLPBlue::setNumErrors (UInt numErrors)
{
   assert (numErrors <= kMaxErrors);
   m_numErrors = numErrors;
}

void
CLPBlue::setName (UInt measInd, string name)
{
   assert (measInd < m_numMeasurements);
   m_names[measInd] = name;
}

void
CLPBlue::setMean (UInt measInd, double mean)
{
   assert (measInd < m_numMeasurements);
   m_mean[measInd] = mean;
}

void
CLPBlue::setErrorFraction (UInt measInd, UInt errInd, 
                           const DVec &value, bool ignoreWarnings)
{
   if (ignoreWarnings && 
       (measInd >= m_numMeasurements || errInd >= m_numErrors))
   {
      // don't do anything, but don't complain either.
      return;
   }
   assert (measInd < m_numMeasurements);
   assert (errInd < m_numErrors);
   m_errorVec[measInd][errInd] = value;
}

void
CLPBlue::setErrorFraction (UInt measInd, UInt errInd, 
                           double p0, double p1, double p2, double p3)
{
   DVec value;
   int max = 3;
   if (! p3)
   {
      max = 2; // only up to p2
      if (! p2)
      {
         max = 1; // only up to p1
         if (! p1)
         {
            max = 0; // only p0
         } // no p1
      } // no p2
   } // no p3
   value.push_back (p0);
   if (max >= 1) value.push_back (p1);
   if (max >= 2) value.push_back (p2);
   if (max >= 3) value.push_back (p3);
   setErrorFraction (measInd, errInd, value);
}

void
CLPBlue::setCorrelation (UInt measInd1, UInt measInd2, 
                         UInt errInd, double value,
                         bool ignoreWarnings)
{
   if (ignoreWarnings && 
       (measInd1 >= m_numMeasurements || measInd2 >= m_numMeasurements || 
        errInd >= m_numErrors))
      {
         // don't do anything, but don't complain either.
         return;
      }
   assert (measInd1 < m_numMeasurements);
   assert (measInd2 < m_numMeasurements);
   assert (errInd < m_numErrors);
   // make it symmetric
   m_correlations[measInd1][measInd2][errInd] 
      = m_correlations[measInd2][measInd1][errInd] = value;
}

void
CLPBlue::getErrorFraction (UInt measInd, UInt errInd, 
                           DVec &retval) const
{
   assert (measInd < m_numMeasurements);
   assert (errInd < m_numErrors);
   retval = m_errorVec[measInd][errInd];
}

double
CLPBlue::getCorrelation (UInt measInd1, UInt measInd2, 
                         UInt errInd) const
{
   assert (measInd1 < m_numMeasurements);
   assert (measInd2 < m_numMeasurements);
   assert (errInd < m_numErrors);
   return m_correlations[measInd1][measInd2][errInd];
}

void
CLPBlue::calcTotalErrors(double mean)
{
   for (UInt measInd = 0; measInd < m_numMeasurements; ++measInd)
   {
      double sum2 = 0.;
      for (UInt errInd = 0; errInd < m_numErrors; ++errInd)
      {
         double err = error (measInd, errInd, mean);
         sum2 += err * err;
      } // for err
      m_totalError[measInd] = sqrt(sum2);
   } // for measInd
}

double
CLPBlue::error (UInt measInd, UInt errInd, double mean)
{
   assert (measInd < m_numMeasurements);
   assert (errInd < m_numErrors);
   if (mean < kUseMean + 1)
   {
      mean = m_mean[measInd];
   } else if (mean < m_minimumMean)
   {
      mean = m_minimumMean;
   }
   double xN = 1.; // start at X^0
   UInt size = m_errorVec[measInd][errInd].size();
   double retval = 0.;
   for (UInt loop = 0; loop < size; ++loop)
   {
      retval += m_errorVec[measInd][errInd][loop] * xN;
      xN *= mean; // go from x^n to x^(n+1)
   } // for size;
   return retval;
}

double
CLPBlue::correlations (UInt measInd1, UInt measInd2, 
                       UInt errInd)
{
   assert (measInd1 < m_numMeasurements);
   assert (measInd2 < m_numMeasurements);
   assert (errInd < m_numErrors);
   return m_correlations[measInd1][measInd2][errInd];
}

double
CLPBlue::totalCorrelations (UInt measInd1, UInt measInd2)
{
   assert (measInd1 < m_numMeasurements);
   assert (measInd2 < m_numMeasurements);
   return m_totalCorrelations[measInd1][measInd2];
}

double
CLPBlue::errorBreakdown (UInt errInd)
{
   assert (errInd < m_numErrors);
   return m_errorBreakdown[errInd];
}
bool
CLPBlue::isValid()
{
   // make sure all means are possitive
   for (UInt loop = 0; loop < m_numMeasurements; ++loop)
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
   ifstream source (filename.c_str(), ios::in);
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
      UInt size = sList.size();
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
         UInt value = atoi( sList[1].c_str() );
         setNumMeasurements (value);
         continue;
      }
      if ("numErrors" == sList[0])
      {
         UInt value = atoi( sList[1].c_str() );
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
         UInt measInd = atoi( sList[1].c_str() );
         setName (measInd, sList[2]);
         continue;
      }
      if ("mean" == sList[0])
      {
         UInt measInd = atoi( sList[1].c_str() );
         double value = atof( sList[2].c_str() );
         setMean (measInd, value);
         continue;
      }
      if (size < 4)
      {
         continue;
      }
      // size must be at least 4
      if ("errorVec" == sList[0])
      {
         UInt measInd = atoi( sList[1].c_str() );
         UInt errInd = atoi( sList[2].c_str() );
         DVec value;
         for (UInt loop = 3; loop < size; ++loop)
         {
            value.push_back( atof( sList[loop].c_str() ) );
         } // for loop 3..size - 1
         setErrorFraction (measInd, errInd, value, m_ignoreLoadWarnings);
         continue;
      }
      if (size < 5)
      {
         continue;
      }
      // size must be at least 5
      if ("correlation" == sList[0])
      {
         UInt measInd1 = atoi( sList[1].c_str() );
         UInt measInd2 = atoi( sList[2].c_str() );
         UInt errInd = atoi( sList[3].c_str() );
         double value = atof( sList[4].c_str() );
         setCorrelation (measInd1, measInd2, errInd, value, 
                         m_ignoreLoadWarnings);
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
   for (UInt measInd = 0; measInd < m_numMeasurements; ++measInd)
   {
      assert (m_totalError[measInd]);
      double weight = m_weight[measInd] 
         = 1 / (m_totalError[measInd] * m_totalError[measInd]);
      weightSum += weight;
      mean += weight * m_mean[measInd];
   } // for measInd
   for (UInt measInd = 0;  measInd < m_numMeasurements; ++measInd)
   {
      m_weight[measInd] /= weightSum;
   } // for measInd
   mean /= weightSum;
   err = 1. / sqrt (weightSum);
}

double
CLPBlue::calcBlueAverage (double errMean)
{
   double mean, err;
   return calcBlueAverage (mean, err, errMean);
}

double
CLPBlue::calcBlueAverage (double &mean, double &err,
                          double errMean)
{
   calcTotalErrors(errMean);
   // Calculate S matrix
   TMatrixD sMatrix (m_numMeasurements, m_numMeasurements);
   for (UInt measInd1 = 0; measInd1 < m_numMeasurements; ++measInd1)
   {
      for (UInt measInd2 = measInd1; measInd2 < m_numMeasurements; ++measInd2)
      {
         double sum = 0.;
         for (UInt errInd = 0; errInd < m_numErrors; ++errInd)
         {
            double err1 = error (measInd1, errInd, errMean);
            double err2 = error (measInd2, errInd, errMean);
            sum += correlations (measInd1, measInd2, errInd) * err1 * err2;
         } // for errInd
         sMatrix (measInd1, measInd2) = sMatrix (measInd2, measInd1) = sum;
      } // for measInd2
   } // for measInd1

   // lets maek sure we don't have anything too stupid going on
  

   // Get hMatrix = sMatrix^-1
   TMatrixD hMatrix (TMatrixD::kInverted, sMatrix);
   double sumMatrix = 0.;
   for (UInt measInd1 = 0; measInd1 < m_numMeasurements; ++measInd1)
   {
      for (UInt measInd2 = 0; measInd2 < m_numMeasurements; ++measInd2)
      {
         sumMatrix += hMatrix (measInd1, measInd2);
      } // for measInd2
   } // for measInd1
   assert (sumMatrix);
   err = 1 / sqrt (sumMatrix);
   mean = 0.;
   double weightSum = 0.;
   for (UInt measInd1 = 0; measInd1 < m_numMeasurements; ++measInd1)
   {
      double weight = 0;
      for (UInt measInd2 = 0; measInd2 < m_numMeasurements; ++measInd2)
      {
         weight += hMatrix (measInd1, measInd2);
      } // for measInd2      
      m_weight[measInd1] = weight;
      weightSum += weight;
      mean += weight * m_mean[measInd1];
   } // for measInd1
   assert (weightSum); // make sure it's not 0
   for (UInt measInd1 = 0;  measInd1 < m_numMeasurements; ++measInd1)
   {
      m_weight[measInd1] /= weightSum;
   } // for measInd1
   mean /= weightSum;

   ////////////////////////
   // Total Correlations //
   ////////////////////////
   for (UInt measInd1 = 0; measInd1 < m_numMeasurements; ++measInd1)
   {
      for (UInt measInd2 = measInd1; measInd2 < m_numMeasurements; ++measInd2)
      {
         // Is this on-diagonal?
         if (measInd1 == measInd2)
         {
            m_totalCorrelations[measInd1][measInd2] = 1.;
         }
         // If we're here, we're off diagonal
         m_totalCorrelations[measInd1][measInd2] = 
            m_totalCorrelations[measInd2][measInd1] = 
            sMatrix (measInd1, measInd2) / 
            sqrt ( sMatrix (measInd1, measInd1) * 
                   sMatrix (measInd2, measInd2) );
      } // for measInd2
   } // for measInd1

   /////////////////////
   // Error Breakdown //
   /////////////////////
   for (UInt errInd = 0; errInd < m_numErrors; ++errInd)
   {
      double cov_breakdown = 0.;
      for (UInt measInd1 = 0; measInd1 < m_numMeasurements; ++measInd1)
      {
         // weighted errors
         double wErr1 = error (measInd1, errInd, mean) * m_weight[measInd1];
         for (UInt measInd2 = measInd1; 
              measInd2 < m_numMeasurements; ++measInd2)
         {
            double wErr2 = error (measInd2, errInd, mean) * m_weight[measInd2];
            cov_breakdown += correlations (measInd1, measInd2, errInd) * 
               wErr1 * wErr2;
         } // for measInd2
      } // for measInd1
      assert (cov_breakdown >= 0);
      m_errorBreakdown[errInd] = sqrt (cov_breakdown);
   } // for errInd

   ///////////////////////
   // Chi^2 Calculation //
   ///////////////////////
   // Now that we have the mean, let's calculate the chi^2
   // chi^2 = D^T * hMatrix * D
   // where D = delta column vector
   TMatrixD Dvec  (m_numMeasurements, 1);
   TMatrixD DTvec (1, m_numMeasurements);
   for (UInt measInd1 = 0; measInd1 < m_numMeasurements; ++measInd1)
   {
      Dvec (measInd1, 0) = DTvec (0, measInd1) = m_mean[measInd1] - mean;
   } // for measInd1
   TMatrixD tempMat (1, m_numMeasurements);
   tempMat.Mult (DTvec, hMatrix);
   TMatrixD chi2Matrix (1,1);
   chi2Matrix.Mult (tempMat, Dvec);
   m_chi2 = chi2Matrix(0, 0);
   m_totMean  = mean;
   m_totError = err;
   return m_chi2;
}

double
CLPBlue::calcIterativeBlueAverage ()
{
   double mean, err;
   return calcIterativeBlueAverage (mean, err);
}
double
CLPBlue::calcIterativeBlueAverage (double &mean, double &error)
{
   // first get weighted average
   calcWeightedAverage (mean, error);
   double oldmean = 0;
   int count = 0;
   double chi2 = -1.;
   while ( (2 * fabs (oldmean - mean) / (oldmean + mean) > 0.001) && 
           (++count < 100) )
   {
      oldmean = mean;
      chi2 = calcBlueAverage(mean, error, oldmean);
   } // while
   return chi2;
}

DVec
CLPBlue::getWeights() const
{
   DVec retval;
   for (UInt measInd = 0; measInd < m_numMeasurements; ++measInd)
   {
      retval.push_back( m_weight[measInd] );
   } // for measInd
   return retval;
}

void
CLPBlue::outputWeights (ostream& o_stream) const
{
   o_stream << "Weights:  ";
   for (UInt measInd = 0; measInd < m_numMeasurements; ++measInd)
   {
      o_stream << m_weight[measInd] << "  ";
   } // for measInd
   o_stream << endl;
}

double
CLPBlue::indMean (UInt measInd) const
{
   assert (measInd < m_numMeasurements);
   return m_mean[measInd];
}

string
CLPBlue::name(UInt measInd) const
{
   assert (measInd < m_numMeasurements);
   return m_names[measInd];
}

double
CLPBlue::weight (UInt measInd) const
{
   assert (measInd < m_numMeasurements);
   return m_weight[measInd];
}

// friends
ostream& operator<< (ostream& o_stream, const CLPBlue &rhs)
{
   if (rhs.m_totError >= 0)
   {
      o_stream << "Combination:  " << rhs.m_totMean << " +-" 
               << rhs.m_totError << endl << endl;   
   }
   for (CLPBlue::UInt measInd = 0; measInd < rhs.m_numMeasurements; ++measInd)
   {
      o_stream << "  Name   : " << rhs.m_names[measInd] << endl
               << "  mean   : " << rhs.m_mean[measInd] << endl;
      if (rhs.m_totError >= 0)
      {
         o_stream << "  totErr : " << rhs.m_totalError[measInd] << endl
                  << "  weight : " << rhs.m_weight[measInd];
      }
      o_stream << endl << endl;
   } // for measInd
   return o_stream;
} 

ostream& operator<< (ostream& o_stream, const TMatrixD &rhs)
{
   int numRows = rhs.GetNrows();
   int numCols = rhs.GetNcols();
   o_stream << numRows << " x " << numCols << endl;
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

