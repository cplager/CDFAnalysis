// -*- C++ -*-

#if !defined(CLPUtils_H)
#define CLPUtils_HH

#include <string>
#include "TMatrixD.h"
#include "TVectorD.h"
#include "CLPBlue/CLPTypeDef.hh"

namespace CLPUtilNamespace
{

   const double kErrorValue = -9999.0;


   // calculate pull value given true value, measured value, and either
   // error or plus and minus error
   double pullValue (double trueValue, double measured,
                         double plus, double minus = 0);

   // given two vectors of equal size, this function returns 
   // the correlation coefficient
   double correlationCoeff (const DVec &xvec, const DVec &yvec);

   // returns the mean and rms of a DVec of numbers
   void calcMeanRMS (double &mean, double &rms, const DVec &xvec);

   // adds up to five numbers in quadrature
   double addInQuad(double one, double two, double three = 0.,
                    double four = 0., double five = 0.);

   // takes the sqauare root of a matrix (easily generizable to take the
   // power 'n' of a matrix)
   void sqrtMatrix (TMatrixD &retval, const TMatrixD &mat);

   // 'retVec' has Gaussian random numbers thrown with the proper
   // correlations when passing in the square root of the covariance
   // matrix and the starting values 'meanVec'
   void gaussCorrRand (TVectorD &retVec, const TMatrixD &sqrtCovMat,
                       const TVectorD &meanVec);

   // splits a line into words
   void split (SVec &retval, std::string line, std::string match = " ");

   // returns a lower case version of a string
   std::string toLowerCase(std::string const &word);

   // returns the fist combination taking N at a time
   IVec firstCombination (int dim);

   // returns true if it has successfully changed 'comb' to the next
   // combination
   bool nextCombination (IVec &comb, int max);

   // reads the first N columns in from a text file and saves them in
   // the DVecVec
   bool readNColumnsFromFile (const std::string &filename,
                              int numColumns,
                              DVecVec &columns);

};

#endif // CLPUtils_HH
