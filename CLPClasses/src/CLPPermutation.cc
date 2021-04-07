// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CLPPermutation.h"

using namespace std;

CLPPermutation::CLPPermutation()
{
   m_chi2          = -1.;
   m_mass          = -1.;
   m_mass2         = -1.;
   m_from          = -1;
   m_choose        = -1;
   m_type          = -1;
   m_isHEPGMatched = false;
   m_combIndex     = -1;
   m_btagFlag      = 0;
   for (int loop = 0; loop < kNumUserDoubles; ++loop)
   {
      m_userDoubles[loop] = 0.;
   } // for loop
   for (int loop = 0; loop < kNumRanks; ++loop)
   {
      m_rank[loop] = -1;
   } // for loop
   for (int loop = 0; loop < kNumBtagIndicies; ++loop)
   {
      m_btagIndex[loop] = -1;
   } // for loop

}

// friends
ostream& operator<< (ostream& o_stream, const CLPPermutation &rhs)
{
   o_stream << "c2 " << rhs.m_chi2
            << " m " << rhs.m_mass << " "<< rhs.m_mass2
            << " r ";
   for (int loop = 0; loop < 3; ++loop)
   {
      o_stream << rhs.m_rank[loop] << " ";
   }
   o_stream << " bt " 
            << rhs.numBtagJets (true)  << " ("
            << rhs.btagStatus  (true)  << ") "
            << rhs.numBtagJets (false) << " ("
            << rhs.btagStatus  (false) << ")";
   return o_stream;
} 

float
CLPPermutation::userDouble (int which) const
{
   assert (which > 0 && which < kNumUserDoubles);
   return m_userDoubles[which];
}

void
CLPPermutation::setUserDouble (int which, float value)
{
   assert (which >= 0 && which < kNumUserDoubles);
   m_userDoubles[which] = value;
}

int
CLPPermutation::btagStatus (bool checkLoose) const
{
   // Loose is the lowest nibble
   // Tight is the second nibble
   int retval;
   if (! checkLoose)
   {
      retval = (int) (m_btagFlag >> 4);
   } else {
      retval = (int) m_btagFlag;
   }
   retval &= 0xF;
   return retval;
}

void
CLPPermutation::setLooseBtagStatus (int status)
{
   // take only the first 4 bits
   status &= 0xF;
   m_btagFlag |= status;
}

void
CLPPermutation::setTightBtagStatus (int status)
{
   // take only the first 4 bits
   status &= 0xF;
   // Shift the status 4 bits (1 nibble)
   status <<= 4;
   m_btagFlag |= status;
}

int
CLPPermutation::numBtagJets (bool checkLoose) const
{
   // Loose is the lowest nibble
   // Tight is the second nibble
   int retval;
   if (! checkLoose)
   {
      retval = (int) (m_btagFlag >> 12);
   } else {
      retval = (int) (m_btagFlag >> 8);
   }
   retval &= 0xF;
   return retval;
}

CLPPermutation::PermPair
CLPPermutation::BtagIndexPair (bool checkLoose) const
{
   PermPair retval;
   retval.first = -1;
   retval.second = -1;
   int status = btagStatus (checkLoose);
   // We don't want any of the other jets tagged
   if (status & kOtherJetTagged)
   {
      // Get out now
      return retval;
   }
   int offset = 2;
   if (checkLoose)
   {
      offset = 0;
   }
   retval.first  = btagIndex (offset + 0);
   retval.second = btagIndex (offset + 1);
   return retval;
}

void
CLPPermutation::setNumLooseBtagJets (int numJets)
{
   // make sure number isn't too big
   if (numJets > 0xF)
   {
      numJets = 0xF;
   }
   // Shift the status 2x4 bits (3 nibbles)
   numJets <<= 8;
   m_btagFlag |= numJets;
}

void
CLPPermutation::setNumTightBtagJets (int numJets)
{
   // make sure number isn't too big
   if (numJets > 0xF)
   {
      numJets = 0xF;
   }
   // Shift the status 3x4 bits (3 nibbles)
   numJets <<= 12;
   m_btagFlag |= numJets;
}

bool
CLPPermutation::select (bool useLoose, int numTags, int statusFlag,
                        int localRank, int globalRank, double chi2)
{
   
   ///////////////////////////
   // Number of Tagged Jets //
   ///////////////////////////
   if ((numTags >= 0) && (numBtagJets (useLoose) != numTags))
   {
      return false;
   }

   //////////////////
   // B Tag Status //
   //////////////////
   int status = btagStatus (useLoose);
   // 1
   if (1 == statusFlag)
   {
      if (kBJetTagged != status)
      {
         return false;
      }
   } // if statusFlag == 1
   // 2
   else if (2 == statusFlag)
   {
      if (! (kWJetTagged & status) ||
          (kOtherJetTagged & status) )
      {
         return false;
      }
   } // if statusFlag == 2
   // 3
   else if (3 == statusFlag)
   {
      if (! (kOtherJetTagged & status) )
      {
         return false;
      }
   } // if statusFlag == 3
   // 4
   else if (4 == statusFlag)
   {
      int wanted = kWJetTagged | kBJetTagged;
      if ( wanted != status )
      {
         return false;
      }
   } // if statusFlag == 4
   // 5
   else if (5 == statusFlag)
   {
      int wanted = kWJetTagged;
      if ( wanted != status )
      {
         return false;
      }
   } // if statusFlag == 5

   ////////////////
   // Local Rank //
   ////////////////
   if (localRank >= 0)
   {
      int offset = 1;
      if (useLoose)
      {
         offset = 0;
      }
      if (rank (1 + offset) > localRank)
      {
         return false;
      }
   } // if localRank

   /////////////////
   // Global Rank //
   /////////////////
   if (globalRank >= 0)
   {
      if (rank(0) > globalRank)
      {
         return false;
      }
   } // if localRank

   //////////
   // Chi2 //
   //////////
   if (chi2 > 0 && chi2 < m_chi2)
   {
      return false;
   }
   
   // If we're still here, return true
   return true;
}

CombEngine::IVec
CLPPermutation::permutation (long long nth)
{
   if (nth < 0)
   {
      nth = m_combIndex;
   }
   CombEngine engine (m_from, m_choose);
   return engine.permutation (nth);
}

bool
CLPPermutation::operator< (const CLPPermutation &rhs) const
{
   return chi2() < rhs.chi2();
}

bool
CLPPermutation::operator> (const CLPPermutation &rhs) const
{
   return chi2() > rhs.chi2();
}

