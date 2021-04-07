// -*- C++ -*-

#if !defined(RunFraction_HH)
#define RunFraction_HH

#include <vector>

#include "CLPTemplateFitter/BinNormFunctoid.hh"

class RunFraction : public BinNormFunctoid
{
   public:
      typedef std::vector< double > DVec;
      
      RunFraction ();

      virtual double operator() (int tempIndex, int binIndex,
                                 double value, const double *paramArray) const;
      virtual double operator() (double control, double fraction, 
                                 double jes) const;


      //////////////////////
      // Access Functions //
      //////////////////////
      int controlIndex()  const { return m_controlIndex; }
      int fractionIndex() const { return m_fractionIndex; }
      int jesIndex()      const { return m_jesIndex; }
      double fractionError() const { return m_fractionError; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setControlIndex  (int controlIndex) 
      { m_controlIndex = controlIndex; }
      void setFractionIndex (int fractionIndex) 
      { m_fractionIndex = fractionIndex; }
      void setJesIndex      (int jesIndex) 
      { m_jesIndex = jesIndex; }
      void setFractionError (double fractionError)
      { m_fractionError = fractionError; }


   private:
      int  m_controlIndex;
      int  m_fractionIndex;
      int  m_jesIndex;
      double m_fractionError;

};

#endif // RunFraction_HH

