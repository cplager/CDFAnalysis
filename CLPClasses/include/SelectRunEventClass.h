// -*- C++ -*-

#if !defined(SelectRunEventClass_H)
#define SelectRunEventClass_H

#include <string>
#include <set>
#include <map>

class SelectRunEventClass
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      enum {kNegativeInteger = -1};

      typedef std::set< int >          SRECSet;
      typedef SRECSet::const_iterator  SRECSetConstIter;
      typedef std::map< int, SRECSet > SRECMap;
      typedef SRECMap::const_iterator  SRECMapConstIter;
      // you could change this if you want to exclude early runs.
      // Must be positive
      enum {kMinRunNumber = 0};

      //////////////////////
      // Member Functions //
      //////////////////////
      SelectRunEventClass ();
      SelectRunEventClass (const std::string &filename);

      // returns true if the run and event (or just run if event is
      // negative) exists in this instance.  Will always return true
      // if either m_alwaysTrue (for this instance) or
      // sm_globalAlwaysTrue (for the class) is set to true.
      bool isSelected (int run, int event = kNegativeInteger);

      // load run and events from a file.  Returns true upon
      // successfully opening file.
      bool loadfile (const std::string &filename);

      // add a run and event.  A negative event means all events in
      // this run are to be selected.
      void addRunEvent (int run, int event = kNegativeInteger);

      // tell this instance to return always true
      void setAlwaysTrue (bool value = true) 
      { m_alwaysTrue = value; }

      // tell all instances to return always true
      static void setAlwaysTrueGlobally (bool value = true) 
      { sm_globalAlwaysTrue = value; }

      // Do we print out messages?
      static void setVerboseMode (bool value)
      { sm_verbose = value; }

      // takes a string and returns two non-negative integers.  If
      // there is only one integer, retval1 will contain it and
      // retval2 will be negative.      
      static void _string2intInt (const std::string &line,
                                 int &retval1, int &retval2);

   private:
      //////////////////////////////
      // Private Member Functions //
      //////////////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      SRECMap m_map;
      bool m_alwaysTrue;
      static bool sm_globalAlwaysTrue;
      static bool sm_verbose;
};


#endif // SelectRunEventClass_H
