// -*- C++ -*-
#include <math.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

#include "TRandom.h"
#include "CLPBlue/CLPUtils.hh"

using namespace std;

double 
CLPUtilNamespace::pullValue (double trueValue, double measured,
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

double 
CLPUtilNamespace::correlationCoeff (const DVec &xvec, const DVec &yvec)
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

void 
CLPUtilNamespace::calcMeanRMS (double &mean, double &rms, const DVec &xvec)
{
   double sumX = 0.;
   double sumX2 = 0.;
   int num = 0;
   for (DVecConstIter iter = xvec.begin();
        xvec.end() != iter;
        ++iter)
   {
      ++num;
      sumX  += *iter;
      sumX2 += *iter * *iter;
   }
   if (! num)
   {
      // we haven't summed over anything
      mean = -999.;
      rms  = -999.;
      return;
   }
   mean = sumX / num;
   double aveX2 = sumX2 / num;
   rms = sqrt (aveX2 - mean * mean);
}

double 
CLPUtilNamespace::addInQuad(double one, double two, double three, 
                            double four, double five)
{
   return sqrt (one * one + two * two + three * three
                + four * four + five * five);
}

void 
CLPUtilNamespace::sqrtMatrix (TMatrixD &retval, const TMatrixD &mat)
{
   TVectorD eigenvalues;
   TMatrixD eigenvectors = mat.EigenVectors (eigenvalues);
   TMatrixD vInv = eigenvectors;
   vInv.Invert();
   int size = mat.GetNrows();
   TMatrixD valueMat (size, size);
   for (int loop = 0; loop < size; ++loop)
   {
      valueMat (loop, loop) = sqrt( eigenvalues(loop) );
   }
   retval = eigenvectors;
   retval *=  valueMat;
   retval *= vInv;
}

void 
CLPUtilNamespace::gaussCorrRand (TVectorD &retVec, const TMatrixD &sqrtCovMat,
                                 const TVectorD &meanVec)
{
   // for now, we are assuming that the TVectorDs and the TMatrixD are
   // all the same size.  It's going to be expensive to check every
   // time.
   int size = retVec.GetNrows();
   TVectorD ugr (size); // unit Gauss random number
   for (int loop = 0; loop < size; ++loop)
   {
      ugr(loop) = gRandom->Gaus (0., 1.);

   } // for loop
   retVec = meanVec;
   for (int outer = 0; outer < size; ++outer)
   {
      for (int inner = 0; inner < size; ++inner)
      {
         retVec(outer) += sqrtCovMat(outer, inner) * ugr(inner);
      } // for inner
   } // for outer
}

void 
CLPUtilNamespace::split (SVec &retval, string line, string match)
{
   retval.clear();
   // find the first non-space
   unsigned int start1 = line.find_first_not_of(" \t");
   // Is the first non-space character a '#'
   char firstCh = line[start1];
   if ('#' == firstCh)
   {
      // this line is a comment
      return;
   }
   line += match; // get last word of line
   unsigned int last = string::npos;
   unsigned int current = line.find_first_of(match);
   while (string::npos != current)
   {
      unsigned int pos;
      if (string::npos != last)
      {
         pos = last + 1;
      } else {
         pos = 0;
      }
      string part = line.substr( pos, current - last - 1);
      // don't bother adding 0 length strings
      if (part.length()) 
      {
         retval.push_back(part);
      }
      last = current;
      current = line.find_first_of(match, current + 1);
   } // while we're finding spaces
}

string 
CLPUtilNamespace::toLowerCase(string const &word)
{
   string retval;
   for (unsigned int loop = 0; loop < word.size(); ++loop)
   {
      int letter = (int) word.at(loop);
      retval += (char) tolower(letter);
   } // for loop
   return retval;
}

IVec
CLPUtilNamespace::firstCombination (int dim)
{
   IVec retval;
   for (int loop = 0; loop < dim; ++loop)
   {
      retval.push_back (loop);
   }
   return retval;
}

bool
CLPUtilNamespace::nextCombination (IVec &comb, int max)
{
   int start = (int) comb.size() - 1;
   int localmax = max;
   bool ok = false;
   while (start >= 0)
   {
      if (++comb[start] < localmax)
      {
         ok = true;
         break;
      } else {
         --start;
         --localmax;
      } // else if
   } // while start
   if (! ok)
   {
      return false;
   }
   for (int loop = start + 1; loop < (int) comb.size(); ++loop)
   {
      comb[loop] = comb[loop - 1] + 1;
   }
   return true;
}

bool
CLPUtilNamespace::readNColumnsFromFile (const string &filename,
                                        int numColumns,
                                        DVecVec &columns)
{
   columns.clear();
   columns.resize (numColumns);
   ifstream source (filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "Can not open '" << filename << "' for reading.  Aborting."
           << endl;
      return false;
   } // if opening file failed
   string line;
   while ( getline (source, line) )
   {
      SVec words;
      split (words, line);
      // are there at least three words?
      if ((int) words.size() < numColumns)
      {
         // don't bother with this line
         continue;
      } // if we don't have enough words
      DVec values;
      bool allZero = true;
      for (int loop = 0; loop < numColumns; ++loop)
      {
         double value = atof ( words[loop].c_str() );
         if (value)
         {
            allZero = false;
         }
         values.push_back( value );
      } // for loop
      if (allZero)
      {
         // Don't bother adding all 0 columns
         continue;
      }
      for (int loop = 0; loop < numColumns; ++loop)
      {
         columns[loop].push_back( values[loop] );
      } // for loop
   } // while getline
   return true;
}

