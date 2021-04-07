// -*- C++ -*-

#if !defined(EventFlavorContainer_HH)
#define EventFlavorContainer_HH

#include "EventClassAlpgenCont.hh"

class EventFlavorContainer
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      typedef std::map< std::string, EventClassAlpgenCont > SACMap;
      typedef std::vector< int >           IVec;
      typedef std::map< std::string, int > SIMap;
      typedef SACMap::iterator             SACMapIter;
      typedef SACMap::const_iterator       SACMapConstIter;
      typedef IVec::const_iterator         IVecConstIter;
      typedef SIMap::const_iterator        SIMapConstIter;
      typedef std::map< std::string, EventFlavorContainer > SEFCMap;
      typedef SEFCMap::iterator                             SECFMapIter;
      typedef SEFCMap::const_iterator                       SECFMapConstIter;
      
      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const EventFlavorContainer &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      EventFlavorContainer ();
      EventFlavorContainer (const std::string &m_flavorName);

       ////////////////
      // One Liners //
      ////////////////
      const std::string &flavorName() const { return m_flavorName; } 
      void setFlavorName (const std::string &name) { m_flavorName = name; }


      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // adds an EventClass object.  Can NOT be called after
      // 'renormalize' or any of the other functions that depend on
      // renormalize.
      void add (const EventClass &event);

      // calculates total weight and renormalizes each alpgen vector.
      // This can only be done once.
      void renormalize();

      // fills eventVec with numEvents events, properly distributed
      // over the different alpgent samples and lepton and detector
      // types.  Can only be done after object is normalized.
      void getRandomEvents (EventClass::Vec &eventVec, IVec numEventsVec) const;

      // returns an SIMap of the the names in order
      void mapNames (SIMap &nameMap) const;

      // reads events from file.  Returns true if everything is fine.
      bool readFile (const std::string &filename);

      // fills a histogram pointer with the analysis descriminant
      // requested from all objects that match the mask.  No mask
      // means all objects.
      void fillHist (TH1F *histPtr, int which, 
                     const std::string &mask = "") const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

      // Returns the index for a given name.  Returns -1 if not found.
      static int nameIndex (const std::string &name, const SIMap &nameMap);

      static int efcCount() { return sm_efcCount; }

  private:
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////

      std::string m_flavorName;
      bool        m_renormalized;
      SACMap      m_map;
      
      ////////////////////////////////
      // Static Private Member Data //
      ////////////////////////////////
      static int  sm_efcCount;
};

#endif // EventFlavorContainer_HH
