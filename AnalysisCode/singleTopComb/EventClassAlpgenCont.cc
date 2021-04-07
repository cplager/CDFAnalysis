// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>

#include "TRandom.h"

#include "CLPBlue/dout.hh"
#include "EventClassAlpgenCont.hh"
#include "dumpSTL.icc"

using namespace std;

EventClassAlpgenCont::EventClassAlpgenCont() :
   m_renormalized (false), m_totalWeight (0.)
{
}

void
EventClassAlpgenCont::add (const EventClass &event)
{
   if (m_renormalized)
   {
      cerr << "EventClassAlpgenCont::add() Error: You can not add "
           << "any more events after renormalizing." << endl;
      assert (m_renormalized);
   }
   int alpgen = event.alpgen();
   // do we already have this vector already?
   EventClass::IVMapIter iter = m_map.find (alpgen);
   if (m_map.end() != iter)
   {
      // this is not the first
      iter->second.push_back (event);
      // we're done
      return;
   }
   // This is the first time.  Set it up
   EventClass::Vec vec;
   vec.push_back (event);
   m_map[alpgen] = vec;   
}

void
EventClassAlpgenCont::renormalize()
{
   if (m_renormalized)
   {
      // Will people never learn.  I said once, darn it!
      return;
   }
   m_renormalized = true;
   m_normalizedWeightVec.clear();
   m_weightMap.clear();
   m_renormFactorMap.clear();
   for (EventClass::IVMapIter mapIter = m_map.begin();
        m_map.end() != mapIter;
        ++mapIter)
   {
      float  maxWeight = 0.;
      double weight    = 0.;
      EventClass::Vec &vec = mapIter->second;
      for (EventClass::VecConstIter vecIter = vec.begin();
           vec.end() != vecIter;
           ++vecIter)
      {
         float thisWeight = vecIter->weight();
         weight   += thisWeight;
         maxWeight = std::max (maxWeight, thisWeight);
      } // for vecIter
      m_renormFactorMap[ mapIter->first ] = maxWeight;
      m_weightMap[ mapIter->first ] = weight;
      assert (maxWeight);
      float scale = 1 / maxWeight;
      for (EventClass::VecIter vecIter = vec.begin();
           vec.end() != vecIter;
           ++vecIter)
      {
         vecIter->scaleWeight (scale);
      } // for vecIter
      m_totalWeight += weight;
      m_normalizedWeightVec.push_back (m_totalWeight);
   } // for mapIter   
   // We have all the weights.  Now lets normalize them
   for (EventClass::DVecIter iter = m_normalizedWeightVec.begin();
        m_normalizedWeightVec.end() != iter;
        ++iter)
   {
      *iter /= m_totalWeight;
   }
}

void 
EventClassAlpgenCont::getRandomEvents (EventClass::Vec &eventVec, 
                                       int numEvents) const
{
   assert (m_renormalized);
   IVec distribVec;
   _distributeEventsRandomly (distribVec, numEvents);
   assert (distribVec.size() == m_map.size());
   IVecConstIter vecIter;
   EventClass::IVMapConstIter mapIter;
   for (mapIter = m_map.begin(), vecIter = distribVec.begin();
        m_map.end() != mapIter && distribVec.end() != vecIter;
        ++mapIter, ++vecIter)
   {
      int eventsTaken = 0;
      int eventsWanted = *vecIter;
      int availableEvents = (int) mapIter->second.size();
      while (eventsTaken < eventsWanted)
      {
         // which event should we try first?
         int eventIndex = gRandom->Integer (availableEvents);
         float scalefactor = gRandom->Rndm();
         const EventClass &object = mapIter->second.at(eventIndex);
         if (scalefactor < object.weight())
         {
            // take it
            eventVec.push_back (object);
            ++eventsTaken;
         } // if we take it
      } // while we need events
   } // for mapIter and vecIter
}

void
EventClassAlpgenCont::fillHist (TH1F *histPtr, int which) const
{
   assert (m_renormalized);
   assert (which >= 0 && which < EventClass::kNumAnalyses);
   for (EventClass::IVMapConstIter mapIter = m_map.begin();
        m_map.end() != mapIter;
        ++mapIter)
   {
      const EventClass::Vec &vec = mapIter->second;
      IDMapConstIter factorIter= m_renormFactorMap.find (mapIter->first);
      assert (m_renormFactorMap.end() != factorIter);
      double factor = factorIter->second;
      for (EventClass::VecConstIter vecIter = vec.begin();
           vec.end() != vecIter;
           ++vecIter)
      {
         // dout << vecIter->analysis (which)
         //      << ", "  << vecIter->weight()
         //      << ", "  << factor << endl;
         histPtr->Fill( vecIter->analysis(which),
                        vecIter->weight() * factor);
      } // for vecIter
   } // for mapIter
}

void
EventClassAlpgenCont::_distributeEventsRandomly (IVec &distribVec, 
                                                 int numEvents) const
{
   distribVec.clear();
   int size = m_normalizedWeightVec.size();
   for (int loop = 0; loop < size; ++loop)
   {
      distribVec.push_back (0);
   } // for loop
   int numAssigned = 0;
   for (int event = 0; event < numEvents; ++event)
   {
      double random = gRandom->Rndm();
      for (int loop = 0; loop < size; ++loop)
      {
         if (random < m_normalizedWeightVec.at(loop))
         {
            ++distribVec.at(loop);
            ++numAssigned;
            break;
         }
      } // for loop      
   } // for event
   if (numAssigned != numEvents)
   {
      cout << "numAssigned " << numAssigned << " numEvents " << numEvents << endl;
   }
   assert (numAssigned == numEvents);
}

// friends
ostream& operator<< (ostream& o_stream, const EventClassAlpgenCont &rhs)
{
   int count = 0;
   for (EventClass::IVMapConstIter mapIter = rhs.m_map.begin();
        rhs.m_map.end() != mapIter;
        ++mapIter)
   {
      const EventClass::Vec &vec = mapIter->second;
      o_stream << "  Alpgen " << mapIter->first 
               << " : " << vec.size() << " entries - ";
      if (rhs.m_renormalized)
      {
         EventClassAlpgenCont::IDMapConstIter renormIter = 
            rhs.m_renormFactorMap.find (mapIter->first);
         EventClassAlpgenCont::IDMapConstIter weightIter = 
            rhs.m_weightMap.find (mapIter->first);
         assert (rhs.m_renormFactorMap.end() != renormIter &&
                 rhs.m_weightMap.end()       != weightIter);
         o_stream << " max " << renormIter->second
                  << " weight " << weightIter->second
                  << " prob " << rhs.m_normalizedWeightVec.at(count)
                  << endl;
      } else {
         o_stream << " not renormalized. " << endl;
      }
      for (EventClass::VecConstIter vecIter = vec.begin();
           vec.end() != vecIter;
           ++vecIter)
      {
         o_stream << "    " << *vecIter << endl;
      } // for vecIter
      ++count;
   } // for mapIter
   return o_stream;
} 
