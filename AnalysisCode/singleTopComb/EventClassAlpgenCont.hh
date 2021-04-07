// -*- C++ -*-

#if !defined(EventClassAlpgenCont_HH)
#define EventClassAlpgenCont_HH

#include "EventClass.hh"

class EventClassAlpgenCont
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      typedef std::vector< int >      IVec;
      typedef std::map< int, double > IDMap;
      typedef IVec::const_iterator    IVecConstIter;
      typedef IDMap::const_iterator   IDMapConstIter;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const EventClassAlpgenCont &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      EventClassAlpgenCont();

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // adds an EventClass object.  Can NOT be called after 'renormalize'
      void add (const EventClass &event);

      // calculates total weight and renormalizes each alpgen vector.
      // This can only be done once.
      void renormalize();

      // fills eventVec with numEvents events, properly distributed
      // over the different alpgent samples.  Can only be done after
      // object is normalized.
      void getRandomEvents (EventClass::Vec &eventVec, int numEvents) const;

      // fills a histogram pointer with the analysis discriminant desired
      void fillHist (TH1F *histPtr, int which) const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

  private:
 
      //////////////////////////////
      // Private Member Functions //
      //////////////////////////////
      
      // distributes N events randomly according to the normalized
      // weights.
      void _distributeEventsRandomly (IVec &distribVec, int numEvents) const;

      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////

      bool   m_renormalized;
      double m_totalWeight;
      
      IDMap m_weightMap;
      IDMap m_renormFactorMap;
      EventClass::DVec m_normalizedWeightVec;
      EventClass::IVMap m_map;
};

#endif // EventClassAlpgenCont_HH
