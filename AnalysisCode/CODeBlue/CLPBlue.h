// -*- C++ -*-

#if !defined(CLPBlue_H)
#define CLPBlue_H

#include <string>
#include "TMatrixD.h"
#include "CLPTypeDef.h"

class CLPBlue
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      enum 
      {
         kMaxMeasurements = 20,
         kMaxErrors = 20 + 1    // + 1 for statistical errors
      };

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPBlue &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPBlue();
      CLPBlue (unsigned int numMeasurements, unsigned int numErrors);

       ////////////////
      // One Liners //
      ////////////////
      

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // initializes all means, fractions, and correlations
      // By Default, the correlations matricies is initialized to the
      // idenity matrix
      void initialize();

      // set functions
      void setNumMeasurements (unsigned int numMeasurements);
      void setNumErrors (unsigned int numErrors);
      void setName (unsigned int mInd1, std::string name);
      void setMean (unsigned int mInd1, double mean);
      void setErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                             const DVec &value);
      void setCorrelation (unsigned int mInd1, unsigned int mInd2, 
                           unsigned int eInd1, double value);

      // called after all information is already filled
      void calcTotalErrors(double mean = -1.);

      // return the error given the measurement number, the error number,
      // and (optionally) the mean.  If no mean is given, the mean for that
      // measurement will be used.
      double error (unsigned int mInd1, unsigned int eInd1, double mean = -1);

      // returns correlations
      double correlations (unsigned int mInd1, unsigned int mInd2, 
                           unsigned int eInd1);

      // returns true if everthing seems to check out
      bool isValid();

      // read needed information from a file
      void readFromFile (const std::string &filename);

      // calculates weighted average.  'errMean' is the mean about which
      // all errors are calculated 
      // -1 ==> use given mean for each measurement
      void calcWeightedAverage (double &mean, double &error,
                                double errMean = -1);

      // calculates BLUE average.  'errMean' is the mean about which
      // all errors are calculated 
      // -1 ==> use given mean for each measurement
      void calcBlueAverage (double &mean, double &error,
                            double errMean = -1);

      // Calculate interative BLUE average
      // otherwise, same as above
      void calcIterativeBlueAverage (double &mean, double &error);

      // return a DVec with the weight values
      DVec getWeights() const;
      
      // output the weights to o_stream
      void outputWeights (ostream &o_stream) const;

      std::string name (unsigned int mInd) const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

  private:
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      unsigned int m_numMeasurements;
      unsigned int m_numErrors;
      std::string m_names[kMaxMeasurements];
      double m_mean[kMaxMeasurements];
      double m_weight[kMaxMeasurements];
      double m_correlations[kMaxMeasurements][kMaxMeasurements][kMaxErrors];
      DVec m_errorVec[kMaxMeasurements][kMaxErrors];
      double m_totalError[kMaxMeasurements];

};

extern std::ostream& operator<< (std::ostream& o_stream, const TMatrixD &rhs);

#endif // CLPBlue_H
