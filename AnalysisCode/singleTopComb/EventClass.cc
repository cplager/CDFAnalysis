// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include "TSystem.h"

#include "EventClass.hh"

using namespace std;

EventClass::EventClass() : m_run(0), m_event(0), m_det(0), m_alpgen(0),
                           m_jetBin(2), m_tagBin(1), m_weight(0.)
{
   for (int loop = 0; loop < kNumAnalyses; ++loop)
   {
      m_analyses[loop] = 0;
   } // for loop
}

EventClass::EventClass(int run, int event, int det, int alpgen,
                       int jet, int tag,  float weight) :
   m_run(run), m_event(event), m_det(det), m_alpgen(alpgen), 
   m_jetBin(jet), m_tagBin(tag), m_weight(weight)
{
   for (int loop = 0; loop < kNumAnalyses; ++loop)
   {
      m_analyses[loop] = 0;
   } // for loop
}

void
EventClass::setAllAnalyses (const DVec &analysesVec)
{
   int size = std::min ((const int) kNumAnalyses, (int) analysesVec.size());
   for (int loop = 0; loop < size; ++loop)
   {
      m_analyses[loop] = analysesVec.at(loop);
   } // for loop
}

void
EventClass::fillHist (const Vec &eventVec, TH1F *histPtr, int which,
                      const string &match)
{
   assert (which >= 0 && which < EventClass::kNumAnalyses);
    for (VecConstIter iter = eventVec.begin();
        eventVec.end() != iter;
        ++iter)
   {
      if (match.length() && string::npos == iter->J_T_Lstring().find (match))
      {
         continue;
      }
      histPtr->Fill( iter->analysis(which) );
   } // for iter
}

// friends
ostream& operator<< (ostream& o_stream, const EventClass &rhs)
{
   // run# evt# det alp tchanLF schanLF stME weight
   o_stream << Form ("%6d %9d %2d %3d %2d %2d %9.6f : ",
                     rhs.run(), rhs.event(), rhs.det(), rhs.alpgen(),
                     rhs.jetBin(), rhs.tagBin(),
                     rhs.weight());
   for (int loop = 0; loop < EventClass::kNumAnalyses; ++loop)
   {
      o_stream << Form (" %9.6f", rhs.analysis (loop));
   } // for loop
   return o_stream;
} 
