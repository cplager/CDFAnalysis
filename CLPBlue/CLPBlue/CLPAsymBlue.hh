// -*- C++ -*-

#if !defined(CLPAsymBlue_HH)
#define CLPAsymBlue_HH

#include "CLPBlue/CLPBlue.hh"

class CLPAsymBlue : public TObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      typedef unsigned int    UInt;

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
      // return the chi^2 of the last average
      double chi2()  const { return m_centrBlue.chi2(); }
      // return the mean of the last average
      double fitMean()  const { return m_centrBlue.fitMean(); }
      // return the average error of the last average
      double aveError() const { return m_centrBlue.fitError(); }
      // return the upper and lower errors of the last average
      double upperError() const { return m_upperError; }
      double lowerError() const { return m_lowerError; }
      // return the individual mean of a given measurement
      double indMean (UInt measIndex) const 
      { return m_centrBlue.indMean (measIndex); }
      // tells blue to ignore warnings when loading file
      void   setIgnoreLoadWarnings (bool ignore = true) 
      { m_ignoreLoadWarnings = ignore; }
      // When set to true, the upper and lower BLUEs use the central
      // value of the central BLUE to calculate their errors.
      void   setUpperLowerErrorsOnly (bool only = true)
      { m_upperLowerErrorsOnly = only; }
      // return the number of measurements and errors
      UInt   numMeasurements() const { return m_centrBlue.numMeasurements(); }
      UInt   numErrors()       const { return m_centrBlue.numErrors(); }
      // names and weights
      std::string name (UInt measInd) const 
      { return m_centrBlue.name (measInd); }
      double weight (UInt measInd) const 
      { return m_centrBlue.weight (measInd); }


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
                                  const DVec &value, 
                                  bool ignoreWarnings = false);
      void setLowerErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                                  const DVec &value, 
                                  bool ignoreWarnings = false);
      void setCorrelation (unsigned int mInd1, unsigned int mInd2, 
                           unsigned int eInd1, double value, 
                           bool ignoreWarnings = false);

      // get the upper and lower errors for a given measurement and error
      void getUpperErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                                  DVec &value) const;
      void getLowerErrorFraction (unsigned int mInd1, unsigned int eInd1, 
                                  DVec &value) const;

      void calcTotalErrors(double mean = CLPBlue::kUseMean);

      // return the error given the measurement number, the error number,
      // and (optionally) the mean.  If no mean is given, the mean for that
      // measurement will be used.
      double lowerError (unsigned int mInd1, unsigned int eInd1, 
                         double mean = CLPBlue::kUseMean);
      
      double upperError (unsigned int mInd1, unsigned int eInd1, 
                         double mean = CLPBlue::kUseMean);

      // returns correlations
      double correlations (unsigned int mInd1, unsigned int mInd2, 
                           unsigned int eInd1);

      // returns true if everthing seems to check out
      bool isValid();

      // read needed information from a file
      void readFromFile (const std::string &filename);

      // calculates weighted average.  'errMean' is the mean about which
      // all errors are calculated 
      // CLPBlue::kUseMean ==> use given mean for each measurement
      void calcWeightedAverage (double &mean, double &upperError,
                                double &lowerError, 
                                double errMean = CLPBlue::kUseMean);

      // calculates BLUE average.  'errMean' is the mean about which
      // all errors are calculated 
      // CLPBlue::kUseMean ==> use given mean for each measurement
      void calcBlueAverage (double &mean, double &upperError,
                            double &lowerError, 
                            double errMean = CLPBlue::kUseMean);

      // Calculate interative BLUE average
      // otherwise, same as above
      void calcIterativeBlueAverage (double &mean, double &upperError,
                                     double &lowerError);

      // return a DVec with the weight values
      DVec getWeights (WhichType type = kCentral) const;
      
      // output the weights to o_stream
      void outputWeights (ostream &o_stream, WhichType type = kCentral);

      // tells BLUE to not use a mean for calculating errors less than
      // this value
      void setMinimumMeanForErrors (double minMean);


      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      static DVec averageDVecs (const DVec& one, const DVec& two);

  private:
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      unsigned int m_numMeasurements;
      unsigned int m_numErrors;
      bool m_ignoreLoadWarnings;
      bool m_upperLowerErrorsOnly;
      double m_lowerError;
      double m_upperError;
      std::string m_names[kMaxMeasurements]; //!
      DVec m_lowerErrorVec[kMaxMeasurements][kMaxErrors]; // !
      DVec m_upperErrorVec[kMaxMeasurements][kMaxErrors]; // !
      CLPBlue m_centrBlue, m_lowerBlue, m_upperBlue;

      ClassDef (CLPAsymBlue, 1) // CLPBlue Class


};

////////////////////////
// ostream Definition //
////////////////////////



#endif // CLPAsymBlue_HH
