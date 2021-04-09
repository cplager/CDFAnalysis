// -*- C++ -*-

#if !defined(CLPHisto_H)
#define CLPHisto_H

#include "TH1F.h"
#include "CLPTypeDef.h"

class CLPHisto
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
                                       const CLPHisto &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPHisto();
      CLPHisto (unsigned int numBins, double lower, double upper);
      CLPHisto (TH1F *hPtr);

       ////////////////
      // One Liners //
      ////////////////
      double total() const { return m_total; }
      unsigned int numBins() const { return m_numBins; }

      static void setNoMinInterval (bool value = true)
      { m_noMinInterval = value; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      void fill (double xValue, double weight = 1.);

      double bin (unsigned int index) const;

      // return max (x,y) 
      double max (double &maxX, double &maxY) const;

      // return min (x,y) 
      double min (double &minX, double &minY) const;

      void find (double y, DVec &retval, bool asIntegrated = false) const;

      double interpolate (double xValue, bool asIntegrated = false) const;

      void findMaxAnd68 (double &max, double &lower, double &upper,
                         double &sixteen, double &fifty, double &eightyfour,
                         double sixtyeight = 0.6828,
                         double allowedError = 0.0001) const;
      
      // called to set things up
      void setNumBinLowerUpper (unsigned int numBins, double lower, 
                                double upper);

      // convert from x to bins and back
      double xToBinD (double xValue, bool asIntegrated = false) const;
      unsigned int xToBin (double xValue) const;
      double binToX (double bin, bool asIntegrated = false) const;

      // return a histogram with same dimensions that has normalized
      // integration of this histogram
      CLPHisto integrate() const;

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
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      DVec m_array;
      double m_total;
      double m_lower;
      double m_upper;
      double m_deltaX;
      unsigned int m_numBins;

      static bool m_noMinInterval;

};


#endif // CLPHisto_H
