// -*- C++ -*-

#if !defined(CLPBlue_HH)
#define CLPBlue_HH

#include <string>
#include "TMatrixD.h"
#include "TObject.h"
#include "CLPBlue/CLPTypeDef.hh"

class CLPBlue : public TObject
{
      // A couple notes on decifering which variables to pass in:
      //
      // 'measInd' is short hand for measurement index (i.e., the
      // index of the measuerment starting from 0).  If you are
      // comparing two measuremnts, you will use 'measInd1' and
      // 'measInd2'.
      //
      // 'errInd' is short hand for error index (starting from 0).

   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      typedef unsigned int    UInt;

      enum 
      {
         kMaxMeasurements = 20,
         kMaxErrors = 20 + 1    // + 1 for statistical errors
      };

      static const double kUseMean;


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
      CLPBlue (UInt numMeasurements, UInt numErrors);

       ////////////////
      // One Liners //
      ////////////////
      // return the chi^2 of the last average
      double chi2()  const { return m_chi2; }
      // return the mean of the last average
      double fitMean()  const { return m_totMean; }
      // return the error of the last average
      double fitError() const { return m_totError; }
      // tells BLUE to not use a mean for calculating errors less than
      // this value
      void   setMinimumMeanForErrors (double minMean)
      { m_minimumMean = minMean; }
      // tells blue to ignore warnings when loading file
      void   setIgnoreLoadWarnings (bool ignore = true) 
      { m_ignoreLoadWarnings = ignore; }
      // return the number of measurements and errors
      UInt   numMeasurements() const { return m_numMeasurements; }
      UInt   numErrors()       const { return m_numErrors; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // initializes all means, fractions, and correlations
      // By Default, the correlations matricies is initialized to the
      // idenity matrix
      void initialize();

      // set functions
      void setNumMeasurements (UInt numMeasurements);
      void setNumErrors (UInt numErrors);
      void setName (UInt measInd, std::string name);
      void setMean (UInt measInd, double mean);
      void setErrorFraction (UInt measInd, UInt errInd, 
                             const DVec &value,
                             bool ignoreWarnings = false);
      void setErrorFraction (UInt measInd, UInt errInd, 
                             double p0, double p1 = 0., 
                             double p2 = 0., double p3 = 0.);
      void setCorrelation (UInt measInd1, UInt measInd2, 
                           UInt errInd, double value,
                           bool ignoreWarnings = false);

      // get functions
      void getErrorFraction (UInt measInd, UInt errInd, 
                             DVec &retval) const;

      double getCorrelation (UInt measInd1, UInt measInd2, 
                             UInt errInd) const;

      // called after all information is already filled
      void calcTotalErrors (double mean = kUseMean);

      // return the error given the measurement number, the error number,
      // and (optionally) the mean.  If no mean is given, the mean for that
      // measurement will be used.
      double error (UInt measInd1, UInt errInd, double mean = kUseMean);

      // returns correlations
      double correlations (UInt measInd1, UInt measInd2, 
                           UInt errInd);

      // returns total correlation AFTER average has been calculated.
      // This function returns the overall correlation between two
      // measurement taking all of the different error types into
      // account.
      double totalCorrelations (UInt measInd1, UInt measInd2);

      // returns error breakdown AFTER avarage has been caluclated
      double errorBreakdown (UInt errInd);

      // returns true if everthing seems to check out
      bool isValid();

      // read needed information from a file
      void readFromFile (const std::string &filename);

      // calculates weighted average.  'errMean' is the mean about which
      // all errors are calculated 
      // kUseMean ==> use given mean for each measurement
      void calcWeightedAverage (double &mean, double &error,
                                double errMean = kUseMean);

      // calculates BLUE average.  'errMean' is the mean about which
      // all errors are calculated 
      // kUseMean ==> use given mean for each measurement
      // Returns the chi^2 of the combination
      double calcBlueAverage (double errMean = kUseMean);
      double calcBlueAverage (double &mean, double &error,
                              double errMean = kUseMean);

      // Calculate interative BLUE average
      // otherwise, same as above
      // Returns the chi^2 of the combination
      double calcIterativeBlueAverage ();
      double calcIterativeBlueAverage (double &mean, double &error);

      // return a DVec with the weight values
      DVec getWeights() const;
      
      // output the weights to o_stream
      void outputWeights (ostream &o_stream) const;

      // return the indiviudal means
      double indMean (UInt measInd) const;

      // returns name of measurement
      std::string name (UInt measInd) const;

      // returns the weight of a measurement
      double weight (UInt measInd) const;

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
      UInt m_numMeasurements;
      UInt m_numErrors;
      double m_chi2;
      double m_totMean;
      double m_totError;
      double m_minimumMean;
      bool   m_ignoreLoadWarnings;
      std::string  m_names[kMaxMeasurements]; //!
      double m_mean[kMaxMeasurements];
      double m_weight[kMaxMeasurements];
      double m_correlations[kMaxMeasurements][kMaxMeasurements][kMaxErrors];
      double m_totalCorrelations[kMaxMeasurements][kMaxMeasurements];
      DVec   m_errorVec[kMaxMeasurements][kMaxErrors]; // !
      double m_totalError[kMaxMeasurements];
      double m_errorBreakdown[kMaxErrors];

      
      
      ClassDef (CLPBlue, 1) // CLPBlue Class

};

extern std::ostream& operator<< (std::ostream& o_stream, const TMatrixD &rhs);

#endif // CLPBlue_HH
