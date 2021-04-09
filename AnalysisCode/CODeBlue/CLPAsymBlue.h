// -*- C++ -*-

#if !defined(CLPAsymBlue_H)
#define CLPAsymBlue_H

#include "CLPBlue.h"

class CLPAsymBlue
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      enum
      {
         kMaxMeasurements = CLPBlue::kMaxMeasurements,
         kMaxErrors = CLPBlue::kMaxErrors
      };

      enum WhichType
      {
         kLower,
         kCentral,
         kUpper
      };

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPAsymBlue &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPAsymBlue();
      CLPAsymBlue (unsigned int numMeasurements, unsigned int numErrors);

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
      void setUpperErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                                  const DVec &value);
      void setLowerErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                                  const DVec &value);
      void setCorrelation (unsigned int mInd1, unsigned int mInd2, 
                           unsigned int eInd1, double value);

      void calcTotalErrors(double mean = -1.);

      // return the error given the measurement number, the error number,
      // and (optionally) the mean.  If no mean is given, the mean for that
      // measurement will be used.
      double lowerError (unsigned int mInd1, unsigned int eInd1, 
                         double mean = -1);

      double upperError (unsigned int mInd1, unsigned int eInd1, 
                         double mean = -1);

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
      void calcWeightedAverage (double &mean, double &upperError,
                                double &lowerError, double errMean = -1);

      // calculates BLUE average.  'errMean' is the mean about which
      // all errors are calculated 
      // -1 ==> use given mean for each measurement
      void calcBlueAverage (double &mean, double &upperError,
                            double &lowerError, double errMean = -1);

      // Calculate interative BLUE average
      // otherwise, same as above
      void calcIterativeBlueAverage (double &mean, double &upperError,
                                     double &lowerError);

      // return a DVec with the weight values
      DVec getWeights(WhichType type = kCentral) const;
      
      // output the weights to o_stream
      void outputWeights (ostream &o_stream, WhichType type = kCentral);

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      static DVec averageDVecs (const DVec& one, const DVec& two);

  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////

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
      DVec m_lowerErrorVec[kMaxMeasurements][kMaxErrors];
      DVec m_upperErrorVec[kMaxMeasurements][kMaxErrors];
      CLPBlue m_centrBlue, m_lowerBlue, m_upperBlue;
};

////////////////////////
// ostream Definition //
////////////////////////



#endif // CLPAsymBlue_H
