// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "CLPClasses/SelectRunEventClass.h"

using namespace std;

bool SelectRunEventClass::sm_globalAlwaysTrue = false;
bool SelectRunEventClass::sm_verbose    = true;

SelectRunEventClass::SelectRunEventClass () : 
   m_alwaysTrue (false)
{
}

SelectRunEventClass::SelectRunEventClass (const string &filename) : 
   m_alwaysTrue (false)
{
   bool ok = loadfile (filename);
   if (! ok)
   {
      cerr << "SelectRunEventClass: Couldn't load file of good runs." << endl;
      exit(1);
   }
}

bool
SelectRunEventClass::loadfile (const string &filename)
{
   cout << "file: " << filename << endl;
   ifstream source (filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "SelectRunEventClass: Couldn't open file " << filename 
           << "." << endl;
      return false;
   }
   // cout << "Reading file " << filename << endl;
   string line;
   while ( getline (source, line) )
   {
      int run, event;
      _string2intInt (line, run, event);
      // make sure we have a good run number
      if (run < 0) 
      {
         cerr << "run " << run << " event " << event << " didn't make it"
              << endl;
         continue;
      }
      if (sm_verbose)
      {
         cout << "Adding run " << run << " event " << event << endl;
      }
      addRunEvent (run, event);
   }
   //cout << "finished with " << filename << endl;
   source.close();
   return true;
}

void
SelectRunEventClass::addRunEvent (int run, int event)
{
   if (event < 0)
   {
      event = kNegativeInteger;
   }
   m_map[run].insert (event);
}

bool
SelectRunEventClass::isSelected(int run, int event)
{
   // Does this list have any events selected?
   // If not, always return true
   // (i.e.  If we haven't selected any events in particular,
   //  we must want them all).
   if (! m_map.size())
   {
      return true;
   }
   // do we have the run number?
   SRECMapConstIter mapIter = m_map.find(run);
   if (m_map.end() == mapIter)
   {
      return m_alwaysTrue || sm_globalAlwaysTrue;
   }
   // O.k., so we have this run number.
   // Is the event number negative
   if (event < 0)
   {
      // we aren't checking event and we have this run, so return true
      return true;
   }
   // Do we either have this event or kNegativeInteger
   const SRECSet &myset = mapIter->second;
   if ( ( myset.end() != myset.find (event) ) ||
        ( myset.end() != myset.find (kNegativeInteger) ) )
   {
      // we got one of them
      return true;
   }
   return m_alwaysTrue || sm_globalAlwaysTrue;
}

void 
SelectRunEventClass::_string2intInt (const std::string &line,
                                    int &retval1, int &retval2)
{
   // If only C++ were as useful as perl
   // if (m/^\s*(\d+)\D+(\d+)/) { return ($1, $2)} return (-1,-1);

   // initialize return values to some reasonable defaults
   retval1 = retval2 = kNegativeInteger; 

   // find the first thing that is not a space
   unsigned int start1 = line.find_first_not_of(" \t");
   if (string::npos == start1)
   {
      // there is nothing useful here
      return;
   }
   // Is the first non-space character a digit
   char firstCh = line[start1];
   if ((firstCh < '0') || (firstCh > '9'))
   {
      // This isn't a digit; Don't bother
      return;
   }
   // where does this string of numbers end?
   unsigned int end1 = line.find_first_not_of("0123456789", start1);
   if (string::npos == end1)
   {
      // we seem to have only one number, so lets grab it and go
      retval1 = atoi ( line.substr (start1).c_str() );
      return;
   }
   retval1 = atoi (line.substr (start1, end1 - start1).c_str());
   // Where do the next digits start
   unsigned int start2 = line.find_first_of("-0123456789", end1);
   if (string::npos == start2)
   {
      // there is nothing useful here
      return;
   }
   // If we're here, then I think we have all the information that we need
   retval2 = atoi (line.substr (start2).c_str());
}
