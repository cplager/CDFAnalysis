// -*- C++ -*-

#include <vector>

#include "CLPTemplateFitter/BinNormFunctoid.hh"

class RunAccept : public BinNormFunctoid
{
   public:
      typedef std::vector< double > DVec;
      
      RunAccept ();
      RunAccept (const DVec &paramVec);

      void setParamVec (const DVec &paramVec);

      virtual double operator() (double value) const;
      virtual double operator() (int tempIndex, int binIndex,
                                 double value, const double *paramArray) const;

   private:
      DVec m_paramVec;

};

