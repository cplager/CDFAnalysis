// -*- C++ -*-

#if !defined(TagFraction_HH)
#define TagFraction_HH

#include <vector>
#include <set>

#include "CLPTemplateFitter/BinNormFunctoid.hh"

class TagFraction : public BinNormFunctoid
{
   public:
      typedef std::set< int >             ISet;
      typedef ISet::const_iterator        ISetConstIter;
      
      TagFraction ();

      virtual double operator() (int tempIndex, int binIndex,
                                 double value, const double *paramArray) const;


      //////////////////////
      // Access Functions //
      //////////////////////
      int tagFracIndex() const { return m_tagFracIndex; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setTagFracIndex (int tagFracIndex) 
      { m_tagFracIndex = tagFracIndex; }
      void setNegBinsSet (const ISet &negBinsSet)
      { m_negBinsSet = negBinsSet; }

      // does the set contain this element?
      bool setContains (int element) const
      {  return ( m_negBinsSet.end() != m_negBinsSet.find (element) ); }

   private:
      int  m_tagFracIndex;
      ISet m_negBinsSet;

};

#endif // TagFraction_HH
