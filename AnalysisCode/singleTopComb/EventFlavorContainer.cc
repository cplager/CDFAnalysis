// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>

#include "CLPBlue/CLPUtils.hh"
#include "EventFlavorContainer.hh"

using namespace std;


// Static memeber data definition(?)
int EventFlavorContainer::sm_efcCount = 0;

EventFlavorContainer::EventFlavorContainer ( ) :
   m_flavorName (""), m_renormalized (false)
{
   ++sm_efcCount;
}

EventFlavorContainer::EventFlavorContainer (const string &flavorName ) :
   m_flavorName (flavorName), m_renormalized (false)
{
   ++sm_efcCount;
}


void
EventFlavorContainer::add (const EventClass &event)
{
   if (m_renormalized)
   {
      cerr << "EventFlavorContainer::add() Error: You can not add "
           << "any more events after renormalizing." << endl;
      assert (m_renormalized);
   }
   string key = event.J_T_Lstring();
   // do we already have this vector already?
   SACMapIter iter = m_map.find (key);
   if (m_map.end() != iter)
   {
      // this is not the first
      iter->second.add (event);
      // we're done
      return;
   }
   // This is the first time.  Set it up
   EventClassAlpgenCont cont;
   cont.add (event);
   m_map[key] = cont; 
}

void
EventFlavorContainer::renormalize()
{
   if (m_renormalized)
   {
      // Will people never learn.  I said once, darn it!
      return;
   }
   m_renormalized = true;
   for (SACMapIter mapIter = m_map.begin();
        m_map.end() != mapIter;
        ++mapIter)
   {
      mapIter->second.renormalize();
   } // for mapIter   
}

void 
EventFlavorContainer::getRandomEvents (EventClass::Vec &eventVec, 
                                       IVec numEventsVec) const
{
   if (! m_renormalized)
   {
      cerr << "EventFlavorContainer::getRandomEvents() Error: You must "
           << "renormalize first." << endl;
      assert (!m_renormalized);      
   }
   assert (numEventsVec.size() >= m_map.size());
   IVecConstIter vecIter;
   SACMapConstIter mapIter;
   for (mapIter = m_map.begin(), vecIter = numEventsVec.begin();
        m_map.end() != mapIter && numEventsVec.end() != vecIter;
        ++mapIter, ++vecIter)
   {
      mapIter->second.getRandomEvents (eventVec, *vecIter);
   } // for mapIter and vecIter
}

void 
EventFlavorContainer::mapNames (SIMap &nameMap) const
{
   if (! m_renormalized)
   {
      cerr << "EventFlavorContainer::mapNames() Error: You must "
           << "renormalize first." << endl;
      assert (!m_renormalized);      
   }
   nameMap.clear();
   int index = 0;
   for (SACMapConstIter mapIter = m_map.begin();
        m_map.end() != mapIter;
        ++mapIter)
   {
      nameMap[mapIter->first] = index++;
   } // for mapIter
}

bool
EventFlavorContainer::readFile (const string &filename)
{
   ifstream source (filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "Can not open '" << filename << "' for reading.  Aborting."
           << endl;
      return false;
   } // if opening file failed
   string line;
   while ( getline (source, line) )
   {
      SVec words;
      CLPUtilNamespace::split (words, line);
      // are there at least three words?
      int size = (int) words.size();
      if (size < 8)
      {
         // don't bother with this line
         continue;
      } // if we don't have enough words
      //  0   1   2    3   4   5     6       7       8    9  10 11
      // run evt lep ntag alp njet weight misweight lepPt LF ME NN
      int run       = atoi (words.at(0).c_str());
      int event     = atoi (words.at(1).c_str());
      int det       = atoi (words.at(2).c_str());
      int tag       = atoi (words.at(3).c_str());
      int alpgen    = atoi (words.at(4).c_str());
      int jet       = atoi (words.at(5).c_str());
      float weight1 = atof (words.at(6).c_str());
      float weight2 = atof (words.at(7).c_str());
      float weight  = weight1 * weight2;
      // we don't want events with 0 weights
      if (! weight)
      {
         // don't bother
         continue;
      }
      EventClass eventObj (run, event, det, alpgen, jet, tag, weight);
      EventClass::DVec analysesVec;
      for (int loop = 9; loop < size; ++loop)
      {
         analysesVec.push_back( atof(words.at(loop).c_str()) );
      } // for loop
      eventObj.setAllAnalyses (analysesVec);
      add (eventObj);
   } // while
   return true;
}

void
EventFlavorContainer::fillHist (TH1F *histPtr, int which, 
                                const std::string &mask) const
{
   for (EventFlavorContainer::SACMapConstIter mapIter = m_map.begin();
        m_map.end() != mapIter;
        ++mapIter)
   {
      // If there is a mask AND it is not in the string name, skip
      // these Alpgen containers.
      if (mask.length() && string::npos == mapIter->first.find (mask))
      {
         // This means that there IS a mask AND this does NOT match
         // the mask, so don't take it.
         continue;
      }
      mapIter->second.fillHist (histPtr, which);
   } // for mapIter
}

/////////////////////////////
// Static Member Functions //
/////////////////////////////

int
EventFlavorContainer::nameIndex (const string &name, 
                                 const SIMap &nameMap)
{
   SIMapConstIter iter = nameMap.find (name);
   if (nameMap.end() == iter)
   {
      return -1;
   }
   return iter->second;
}

// friends
ostream& operator<< (ostream& o_stream, const EventFlavorContainer &rhs)
{
   o_stream << "Flavor: " << rhs.flavorName() << endl;
   for (EventFlavorContainer::SACMapConstIter mapIter = rhs.m_map.begin();
        rhs.m_map.end() != mapIter;
        ++mapIter)
   {
      o_stream << "  " << mapIter->first << endl << mapIter->second << endl;
   } // for mapIter
   
   return o_stream;
} 
