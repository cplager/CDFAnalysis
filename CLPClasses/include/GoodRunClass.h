// -*- C++ -*-

#if !defined(GoodRunClass_H)
#define GoodRunClass_H

#include <string>
#include <map>

class GoodRunClass
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      typedef std::pair <int, int> IPair;
      typedef std::multimap <int, IPair> GRMap;
      typedef GRMap::iterator GRMapIter;
      // you could change this if you want to exclude early runs.
      // Do NOT lower below 100000 (1e5)
      enum {kMinRunNumber = 100000};

      //////////////////////
      // Member Functions //
      //////////////////////
      // constructor
      GoodRunClass ();
      GoodRunClass (const std::string &filename);

      // Load list from a file.  Note that the way the search is
      // conducted, loading multiple files will 'OR' the good run
      // lists together (i.e., if any section range is found that
      // contains a section, it will be marked as good).
      bool loadfile (const std::string &filename);
     
      // Is this a good run?
      // NOTE: If no good run list is loaded, ALL runs will be marked
      // good.
      bool isGoodRun (int run, int section = -1);

      // clears the good run list.
      void clear() { m_map.clear(); }

  private:
      //////////////////////////////
      // Private Member Functions //
      //////////////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      GRMap m_map;
};


#endif // GoodRunClass_H
