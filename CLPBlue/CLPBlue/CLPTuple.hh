// -*- C++ -*-

#if !defined(CLPTuple_H)
#define CLPTuple_H

#include "CLPBlue/CLPHisto.hh"

class CLPTuple
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPTuple &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPTuple();

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      double lower() const { return m_lower; }
      double upper() const { return m_upper; }
      double deltaX() const { return m_deltaX; }
      int numBins() const { return m_numBins; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      // void setLower(double lower) { m_lower = lower; }
      // void setUpper(double upper) { m_upper = upper; }
      // void setNumBins(int numBins) { m_numBins = numBins; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // sets all three quantities.  Can be called at any time
      void setNumBinLowerUpper (unsigned int numBins,
                                double lower, double upper);

      // Sets only number of bins.
      void setNumBins (unsigned int numBins);

      // fill value to tuple
      void fill (double value);

      // return projection of CLPTuple into a CLPHisto
      const CLPHisto& hist() const;

      // Guess the bins and range if you haven't already specified
      void guessBins() const;

      // reset everything
      void clear();

      TH1F* RootHisto (std::string idname, std::string name = "") const;



      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////

  private:
      //////////////////////////////
      // Private Member Functions //
      //////////////////////////////

      // fill the histogram
      void _fillHist() const;

      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      DVec m_values;
      double m_min, m_max;
      mutable CLPHisto m_hist;
      mutable double m_lower;
      mutable double m_upper;
      mutable double m_deltaX;
      mutable unsigned int m_numBins;
      mutable bool m_histoIsCurrent;
      mutable bool m_binsSet;

};

////////////////////////
// ostream Definition //
////////////////////////
extern ostream& operator<< (ostream& o_stream, const CLPTuple &right);



#endif // CLPTuple_H
