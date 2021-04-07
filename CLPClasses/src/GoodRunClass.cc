// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "CLPClasses/CLPClasses.h"
#include "CLPClasses/dout.h"

using namespace std;

GoodRunClass::GoodRunClass ()
{
}

GoodRunClass::GoodRunClass (const std::string &filename)
{
   if (! loadfile (filename) )
   {
      cerr << "GoodRunClass: Couldn't load file of good runs." << endl;
      exit(1);
   }
}

bool
GoodRunClass::loadfile (const std::string &filename)
{
   cout << "file: " << filename << endl;
   ifstream source (filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "GoodRunClass: Couldn't open file " << filename << "." << endl;
      return false;
   }
   string line;
   while ( getline(source, line) )
   {
      // make sure this line isn't commented out
      string firstChar = line.substr(0, 1);
      if ("#" == firstChar)
      {
         continue;
      }      
      // split the lines into words
      SVec words;
      CLPFunctions::splitLine (words, line);
      // do we have three words?
      if (words.size() < 3)
      {
         // don't have enough words
         continue;
      }
      // grab the first number IFF it is > minimum run number
      // (this throws out any lines that start with a non-number)
      int run   = atoi (words[0].c_str());
      int start = atoi (words[1].c_str());
      int end   = atoi (words[2].c_str());
      if (run > kMinRunNumber) {
         //could use 'std::make_pair' instead of 'pair< int, IPair >'
         m_map.insert( pair< int, IPair > ( run, IPair(start, end) ) );
      }
   }
   source.close();
   return true;
}

bool
GoodRunClass::isGoodRun (int run, int section)
{
   // if map is empty, then always return true
   if (0 == m_map.size())
   {
      return true;
   }
   pair< GRMapIter, GRMapIter > segIter = m_map.equal_range (run);
   if (segIter.first == segIter.second)
   {
      // Don't have anything
      return false;
   }
   // Did we ask about a particular section?
   if (section < 0)
   {
      // Apparently, we don't care what section, just that the run is
      // here.
      return true;
   }
   // O.k.  So we want a particular section in a run that we know we
   // have.  If any section range for this run has our section then we
   // return true.
   for (GRMapIter iter = segIter.first; iter != segIter.second; ++iter)
   {
      // just to be clear
      IPair sectionsPair = iter->second;
      if (section < sectionsPair.first)
      {
         // don't bother, it already didn't pass the first cut
         continue;
      }
      if (sectionsPair.second < 0)
      {
         // the upper end is unlimited, so we're golden
         return true;
      }
      // if we're still here, we need to check that the upper boundry
      // is o.k.
      if (section <= sectionsPair.second)
      {
         return true;
      }
   }
   // If we're still here, we didn't find what we were looking for
   return false;
}
