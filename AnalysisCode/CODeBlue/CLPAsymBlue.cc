// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CLPAsymBlue.h"
#include "StringUtils.h"
#include "dout.h"

using namespace std;
//////////////////////
// Member Functions //
//////////////////////
CLPAsymBlue::CLPAsymBlue()
{
   setNumMeasurements (0);
   setNumErrors (0);
   initialize();
}

CLPAsymBlue::CLPAsymBlue(unsigned int numMeasurements, unsigned int numErrors)
{
   setNumMeasurements (numMeasurements);
   setNumErrors (numErrors);
   initialize();
}

void
CLPAsymBlue::initialize()
{
   m_lowerBlue.initialize();
   m_upperBlue.initialize();
   m_centrBlue.initialize();
}

void
CLPAsymBlue::setNumMeasurements (unsigned int numMeasurements)
{
   assert (numMeasurements <= kMaxMeasurements);
   m_numMeasurements = numMeasurements;
   m_lowerBlue.setNumMeasurements (numMeasurements);
   m_upperBlue.setNumMeasurements (numMeasurements);
   m_centrBlue.setNumMeasurements (numMeasurements);
}

void
CLPAsymBlue::setNumErrors (unsigned int numErrors)
{
   assert (numErrors <= kMaxErrors);
   m_numErrors = numErrors;
   m_lowerBlue.setNumErrors (numErrors);
   m_upperBlue.setNumErrors (numErrors);
   m_centrBlue.setNumErrors (numErrors);
}

void
CLPAsymBlue::setName (unsigned int mInd1, string name)
{
   assert (mInd1 < m_numMeasurements);
   m_lowerBlue.setName (mInd1, name);
   m_upperBlue.setName (mInd1, name);
   m_centrBlue.setName (mInd1, name);
}

void
CLPAsymBlue::setMean (unsigned int mInd1, double mean)
{
   assert (mInd1 < m_numMeasurements);
   m_lowerBlue.setMean (mInd1, mean);
   m_upperBlue.setMean (mInd1, mean);
   m_centrBlue.setMean (mInd1, mean);
}

void
CLPAsymBlue::setUpperErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                                    const DVec &value)
{
   assert (mInd1 < m_numMeasurements);
   assert (eInd1 < m_numErrors);
   m_upperErrorVec[mInd1][eInd1] = value;
   // set upper like you expect
   m_upperBlue.setErrorFraction(mInd1, eInd1, value);
   // now set the center as well
   DVec average = averageDVecs(m_upperErrorVec[mInd1][eInd1],
                               m_lowerErrorVec[mInd1][eInd1]);
   m_centrBlue.setErrorFraction(mInd1, eInd1, average);
}

void
CLPAsymBlue::setLowerErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                                    const DVec &value)
{
   assert (mInd1 < m_numMeasurements);
   assert (eInd1 < m_numErrors);
   m_lowerErrorVec[mInd1][eInd1] = value;
   // set lower like you expect
   m_lowerBlue.setErrorFraction(mInd1, eInd1, value);
   // now set the center as well
   DVec average = averageDVecs(m_upperErrorVec[mInd1][eInd1],
                               m_lowerErrorVec[mInd1][eInd1]);
   m_centrBlue.setErrorFraction(mInd1, eInd1, average);
}

void
CLPAsymBlue::setCorrelation (unsigned int mInd1, unsigned int mInd2, 
                             unsigned int eInd1, double correlation)
{
   assert (mInd1 < m_numMeasurements);
   assert (mInd2 < m_numMeasurements);
   assert (eInd1 < m_numErrors);
   m_lowerBlue.setCorrelation (mInd1, mInd2, eInd1, correlation);
   m_upperBlue.setCorrelation (mInd1, mInd2, eInd1, correlation);
   m_centrBlue.setCorrelation (mInd1, mInd2, eInd1, correlation);
}

void
CLPAsymBlue::calcTotalErrors(double mean)
{
   m_lowerBlue.calcTotalErrors (mean);
   m_upperBlue.calcTotalErrors (mean);
   m_centrBlue.calcTotalErrors (mean);
}

double
CLPAsymBlue::correlations (unsigned int mInd1, unsigned int mInd2, 
                           unsigned int eInd1)
{
   assert (mInd1 < m_numMeasurements);
   assert (mInd2 < m_numMeasurements);
   assert (eInd1 < m_numErrors);
   return m_centrBlue.correlations(mInd1, mInd2, eInd1);
}

bool
CLPAsymBlue::isValid()
{
   return true;
}

void
CLPAsymBlue::readFromFile(const string &filename)
{
   ifstream source(filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "file " << filename << " could not be opened" << endl;
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
      if ("lowerErrorVec" == sList[0])
      {
         unsigned int mInd1 = atoi( sList[1].c_str() );
         unsigned int eInd1 = atoi( sList[2].c_str() );
         DVec value;
         for (unsigned int loop = 3; loop < size; ++loop)
         {
            value.push_back( atof( sList[loop].c_str() ) );
         } // for loop 3..size - 1
         setLowerErrorFraction (mInd1, eInd1, value);
         continue;
      }
      if ("upperErrorVec" == sList[0])
      {
         unsigned int mInd1 = atoi( sList[1].c_str() );
         unsigned int eInd1 = atoi( sList[2].c_str() );
         DVec value;
         for (unsigned int loop = 3; loop < size; ++loop)
         {
            value.push_back( atof( sList[loop].c_str() ) );
         } // for loop 3..size - 1
         setUpperErrorFraction (mInd1, eInd1, value);
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
CLPAsymBlue::calcWeightedAverage (double &mean, double &upperError,
                                  double &lowerError, double errMean)
{
   double dummy, totalError;
   double almostUpperError, almostLowerError;
   // get central value from center blue
   m_centrBlue.calcWeightedAverage (mean, totalError);
   // get lower error from lower blue
   m_lowerBlue.calcWeightedAverage (dummy, almostLowerError);
   // get upper error from upper blue
   m_centrBlue.calcWeightedAverage (dummy, almostUpperError);
   //lowerError *= -1; // make it negative for consistency
   double denominator = (almostLowerError + almostUpperError) / 2.;
   assert (denominator);
   double factor = totalError / denominator;
   upperError = factor * almostUpperError;
   lowerError = factor * almostLowerError;
}

void
CLPAsymBlue::calcIterativeBlueAverage (double &mean, double &upperError,
                                       double &lowerError)
{
   double dummy, totalError;
   double almostUpperError, almostLowerError;
   // get central value from center blue
   m_centrBlue.calcIterativeBlueAverage (mean, totalError);
   // get lower error from lower blue
   m_lowerBlue.calcIterativeBlueAverage (dummy, almostLowerError);
   // get upper error from upper blue
   m_centrBlue.calcIterativeBlueAverage (dummy, almostUpperError);
   //lowerError *= -1; // make it negative for consistency
   double denominator = (almostLowerError + almostUpperError) / 2.;
   assert (denominator);
   double factor = totalError / denominator;
   upperError = factor * almostUpperError;
   lowerError = factor * almostLowerError;
}

void
CLPAsymBlue::calcBlueAverage (double &mean, double &upperError,
                              double &lowerError, double errMean)
{
   double dummy, totalError;
   double almostUpperError, almostLowerError;
   // get central value from center blue
   m_centrBlue.calcBlueAverage (mean, totalError);
   // get lower error from lower blue
   m_lowerBlue.calcBlueAverage (dummy, almostLowerError);
   // get upper error from upper blue
   m_centrBlue.calcBlueAverage (dummy, almostUpperError);
   //lowerError *= -1; // make it negative for consistency
   double denominator = (almostLowerError + almostUpperError) / 2.;
   assert (denominator);
   double factor = totalError / denominator;
   upperError = factor * almostUpperError;
   lowerError = factor * almostLowerError;
}

DVec
CLPAsymBlue::getWeights(CLPAsymBlue::WhichType type) const
{
   DVec retval;
   if (kLower == type)
   {
      return m_lowerBlue.getWeights();
   } else if (kUpper == type)
   {
      return m_upperBlue.getWeights();
   } else {
      return m_centrBlue.getWeights();
   }
}

void
CLPAsymBlue::outputWeights (ostream& o_stream, CLPAsymBlue::WhichType type)
{
   DVec weights = getWeights(type);
   o_stream << "Weights:  ";
   for (unsigned int mInd1 = 0; mInd1 < m_numMeasurements; ++mInd1)
   {
      o_stream << weights[mInd1] << "  ";
   } // for mInd1
   o_stream << endl;
}

/////////////////////////////
// Static Member Functions //
/////////////////////////////
DVec
CLPAsymBlue::averageDVecs (const DVec& one, const DVec& two)
{
   unsigned int size1 = one.size();
   unsigned int size2 = two.size();
   // get maximum
   unsigned int max = size1;
   if (size2 > max)
   {
      max = size2;
   }
   DVec retval;
   for (unsigned int loop = 0; loop < max; ++loop)
   {
      double oneVal = 0.;
      double twoVal = 0.;
      if (loop < size1)
      {
         oneVal = one[loop];
      }
      if (loop < size2)
      {
         twoVal = two[loop];
      }
      retval.push_back( (oneVal + twoVal) / 2 );
   }
   if (0 == retval.size())
   {
      retval.push_back(0.);
   }
   return retval;
}

//////////////////////
// Friend Functions //
//////////////////////
ostream& operator<< (ostream& o_stream, const CLPAsymBlue &rhs)
{
   // should probably write something here...
   return o_stream;
} 
