// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "SHyFT/Utilities/interface/CombEngine.h"

using namespace std;

CombEngine::CombEngine ()
{
   m_startAt = 0;
   setFromAndChoose (0, 0);
}

CombEngine::CombEngine (int from, int choose, int startAt)
{
   m_startAt = startAt;
   setFromAndChoose (from, choose);
}

void
CombEngine::setFromAndChoose (int from, int choose)
{
   // check our values
   if ((from <= 0) || (choose <= 0) || (from < choose))
   {
      cerr << "Error: CombEngine::setFromAndChoose: Invalid initialization" 
           << endl
           << "from " << from << " choose " << choose << endl;
      assert(0);
   }
   m_from = from;
   m_choose = choose;
   m_permutations = factorialRatio (m_from, m_from - choose);
   m_combinations = factorialRatio (m_from, m_from - choose, choose);
   _setupDefaultVector();
   _setupDecodingVector();
}


CombEngine::IVec 
CombEngine::permutation (long long nth, int offset) const
{
   IVec retval;
   reset();
   // loop over decoding vector
   int size = (int) m_decodeVec.size(); // same as m_choose
   for (int loop = 0; loop < size; ++loop)
   {
      long long value = m_decodeVec[loop];
      int which = nth / value;
      int one = _getNthComponent (which);
      retval.push_back (one + offset);
      nth = nth % value;
   } // for loop
   return retval;
}

CombEngine::IVec 
CombEngine::combination (long long nth, int offset) const
{
   IVec retval;
   combination (retval, m_from, m_choose, nth, offset);
   return retval;
}

const CombEngine::IVec &
CombEngine::combination (IVec &retval, long long nth, int offset) const
{
   combination (retval, m_from, m_choose, nth, offset);
   return retval;
}

long long
CombEngine::permVector2Index (CombEngine::IVec vec) const
{
   if ((int) vec.size() != m_choose)
   {
      // we have a mismatch
      cerr << "Error: CombEngine::permVector2index: invalid vector size" 
           << endl;
      return -1;
   }
   long long retval = 0;
   for (int loop = 0; loop < m_choose; ++loop)
   {
      int value = vec[0];
      retval += (value - m_startAt) * m_decodeVec[loop];
      _reduceVector (vec, value);
   } // for loop
   return retval;
}

long long
CombEngine::combVector2Index (const CombEngine::IVec &vec) const
{
   return combVector2Index (m_from, m_choose, vec);
}


//////////////////////////////
// Private Member Functions //
//////////////////////////////

int
CombEngine::_getNthComponent(int nth) const
{
   assert (nth < (int) m_currentVec.size());
   int count = 0;
   for (IVecIter iter = m_currentVec.begin();
        iter != m_currentVec.end();
        ++iter)
   {
      // are we there yet
      if (count == nth)
      {
         // we're here
         int retval = *iter;
         m_currentVec.erase (iter);
         return retval;
      }
      ++count;
   } // for iter
   assert(0);
   return -1;
}

void
CombEngine::_reduceVector (CombEngine::IVec &vec, int n)
{
   // make sure we have something to work with
   if (0 == vec.size())
   {
      // don't bother
      return;
   }
   // remove the first element
   IVecIter iter = vec.begin();
   vec.erase (iter);
   // loop through the vector and reduce all values > n
   for (int loop = 0; loop < (int) vec.size(); ++loop)
   {
      if (vec[loop] > n)
      {
         --vec[loop];
      }
   } // for loop
}

void
CombEngine::_setupDefaultVector()
{
   m_defaultVec.clear();
   for (int loop = 0; loop < m_from; ++loop)
   {
      m_defaultVec.push_back( loop + m_startAt );
   }
   reset();
}

void
CombEngine::_setupDecodingVector()
{
   m_decodeVec.clear();
   for (int loop = 1; loop <= m_choose; ++loop)
   {
      long long value = factorialRatio (m_from - loop, m_from - m_choose);
      m_decodeVec.push_back (value);
   } // for loop
}

/////////////////////////////
// Static Member Functions //
/////////////////////////////

void 
CombEngine::calcTagDistribution (const DVec &tagRateVec, 
                                 DVec &tagDistributionVec)
{
   tagDistributionVec.clear();
   int numJets = (int) tagRateVec.size();
   for (int numTags = 0; numTags <= numJets; ++numTags)
   {
      double sumWeight = 0.;
      IVec comb = CombEngine::firstCombination (numTags);
      do
      {
         double weight = 1.;
         set< int > iset;
         IVec2ISet (comb, iset);
         for (int inner = 0; inner < numJets; ++inner)
         {
            ISet::const_iterator isetIter = iset.find (inner);
            // Did we find this index?
            if (iset.end() != isetIter)
            {
               // We found it : Count this as a tag
               weight *= tagRateVec[inner];
            } else {
               // We didn't : Count as a no tag
               weight *= (1 - tagRateVec[inner]);
            }
         } // for inner
         sumWeight += weight;
      } while ( CombEngine::nextCombination (comb, numJets) );
      tagDistributionVec.push_back (sumWeight);
   } // for numTags
}

void 
CombEngine::IVec2ISet (const IVec &ivec, ISet &iset)
{
   iset.clear();
   for (IVecConstIter iter = ivec.begin(); 
        ivec.end() != iter; 
        ++iter)
   {
      iset.insert (*iter);
   } // for iter
}


long long
CombEngine::factorial (int value)
{
   if (value <= 1)
   {
      return 1;
   }
   return value * factorial( value - 1 );
}

long long
CombEngine::factorialRatio (int numerator, int denominator)
{
   // check values
   if ((numerator < denominator) || (denominator < 0))
   {
      cerr << "Error: CombEngine::factorialRatio: Invalid initialization" 
           << endl;
      assert(0);
   }
   if (numerator == denominator)
   {
      return 1;
   }
   long long retval = 1;
   for (int loop = denominator + 1; loop <= numerator; ++loop)
   {
      retval *= loop;
   } // for loop
   return retval;
}

long long
CombEngine::factorialRatio (int numerator, int denom1, int denom2)
{
   // check values
   if ( (numerator < denom1) || (denom1 < 0) ||
        (numerator < denom2) || (denom2 < 0) )
   {
      cerr << "Error: CombEngine::factorialRatio: Invalid initialization" 
           << endl;
      assert(0);
   }
   if (denom2 > denom1)
   {
      std::swap (denom1, denom2);
   }
   int retval = factorialRatio (numerator, denom1);
   for (int loop = 2; loop <= denom2; ++loop)
   {
      retval /= loop;
   }
   return retval;
}

void
CombEngine::combination (IVec& comb, int from, int choose,
                         int nth, int offset)
{
   comb.clear();
   if (from == choose)
   {
      for (int loop = 0; loop < choose; ++loop)
      {
         comb.push_back (loop + offset);
      } // for loop     
      return;
   } // if from == choose
   if (1 == choose)
   {
      comb.push_back (nth + offset);
      return;
   } // if 1 == choose
   // If we're still here, we aren't going to get off with one of the
   // easy cases (we'll be using recursion instead).  Let's make sure
   // nth is valid
   long long num = calcNumCombinations (from, choose);
   nth %= num;
   int first = offset;
   --from;
   --choose;
   ++offset;
   do
   {
      num = calcNumCombinations (from, choose);
      if (nth < num)
      {
         comb.push_back (first);
         combination (comb, from, choose, nth, offset);
         return;
      } // if nth < num
      // If we're still here then we're still searching.  Update the
      // arguments and keep going.
      nth -= num;
      --from;
      ++offset;
      ++first;
   } while (from);
   // if we are here, then you gave me bad arguments
   cerr << "CombEngine::combination(): You passed in bad arguments"
        << endl;
   assert(0);
}

CombEngine::IVec
CombEngine::firstCombination (int dim)
{
   IVec retval;
   for (int loop = 0; loop < dim; ++loop)
   {
      retval.push_back (loop);
   }
   return retval;
}

bool
CombEngine::nextCombination (IVec &comb, int max)
{
   int start = (int) comb.size() - 1;
   int localmax = max;
   bool ok = false;
   while (start >= 0)
   {
      if (++comb[start] < localmax)
      {
         ok = true;
         break;
      } else {
         --start;
         --localmax;
      } // else if
   } // while start
   if (! ok)
   {
      return false;
   }
   for (int loop = start + 1; loop < (int) comb.size(); ++loop)
   {
      comb[loop] = comb[loop - 1] + 1;
   }
   return true;
}

long long
CombEngine::combVector2Index (int from, int choose, IVec comb)
{
   // make sure 'comb' is legit
   sort (comb.begin(), comb.end());
   int size = (int) comb.size();
   if (! size)
   {
      cerr << "Combination is not a empty.  Aborting"
           << endl;
      return -1;      
   }
   for (int loop = 0; loop < size - 1; ++loop)
   {
      if (comb[loop] == comb[loop + 1])
      {
         cerr << "Combination " << comb << " is not a combination.  Aborting"
              << endl;
         return -1;
      }
   } // for loop
   if (1 == choose)
   {
      assert (1 == size);
      return comb[0];
   }
   assert (size >= 2);
   long long retval = 0;
   // 'shift' vector
   int first = comb[0];
   comb.erase ( comb.begin() );
   while (first)
   {
      int numComb = calcNumCombinations (from - 1, choose - 1);
      --from;
      // reduce all entries by 1
      for (IVecIter iter = comb.begin(); comb.end() != iter; ++iter)
      {
         --(*iter);
      } // for iter
      retval += numComb;
      --first;
   }
   // Do we need to keep goiing
   if (comb.size())
   {
      // reduce all entries by 1
      for (IVecIter iter = comb.begin(); comb.end() != iter; ++iter)
      {
         --(*iter);
      } // for iter
      --from;
      --choose;
      return retval + combVector2Index (from, choose, comb);
   } // while first
   // If we're still here, then we're done
   return 0;
}

// friends
ostream& operator<< (ostream& o_stream, const CombEngine &rhs)
{
   long long num = rhs.numCombinations();
   int width = (int) log10 ((double) num + 1.) + 1;
   o_stream << "combinations " << num << endl;
   o_stream << "decode  " << rhs.m_decodeVec << endl;
   for (long long loop = 0; loop < num; ++loop)
   {
      CombEngine::IVec vec = rhs.combination(loop);
      long long index = rhs.vector2Index (vec);
      o_stream << setw(width) << loop << ") " << vec
               << " : " <<  index;
      if (loop != index)
      {
         o_stream << " **WARNING**";
      }
      o_stream << endl;
   } // for loop
   return o_stream;
} 

ostream& operator<< (ostream& o_stream, const CombEngine::IVec &rhs)
{
   int size = (int) rhs.size();
   for (int loop = 0; loop < size; ++loop)
   {
      o_stream << rhs[loop] << " ";
   } // for loop
   return o_stream;
} 

ostream& operator<< (ostream& o_stream, const CombEngine::LVec &rhs)
{
   int size = (int) rhs.size();
   for (int loop = 0; loop < size; ++loop)
   {
      o_stream << rhs[loop] << " ";
   } // for loop
   return o_stream;
} 
