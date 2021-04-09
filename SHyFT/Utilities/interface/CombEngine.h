// -*- C++ -*-

#if !defined(CombEngine_H)
#define CombEngine_H

// Written by Charles Plager, 2004-2007

// This code is meant to be used for either generating all possible
// permutations or combinations.  The below examples are assuming that
// 'from' is the sample size and 'choose' are how many you want at any
// given time.
//
// Example of Permutations:
//    CombEngine perm (from, choose);
//    int numPerm = perm.numPermutations();
//    for (int loop = 0; loop < numPerm; ++loop)
//    {
//       CombEngine::IVec vec = perm.permutation (loop);
//       // use 'vec' to get the indicies
//    } // for loop
//
// Example of Combinations:
//    // DON'T USE THIS VERSION
//    // CombEngine comb (from, choose);
//    // int numComb = comb.numCombinations();
//    // for (int loop = 0; loop < numComb; ++loop)
//    // {
//    //    CombEngine::IVec vec = comb.combination (loop);
//    //    // use 'vec' to get the indicies
//    // } // for loop
//    
//   OR
//    // USE THIS ONE
//    CombEngine::IVec comb = CombEngine::firstCombination (choose);
//    do
//    {
//       double weight = 1.;
//       CombEngine::ISet iset;
//       IVec2ISet (comb, iset);
//       for (int inner = 0; inner < numJets; ++inner)
//       {
//          CombEngine::iset::const_iterator isetIter = iset.find (inner);
//          // Did we find this index?
//          if (iset.end() != isetIter)
//          {
//             // We found it : Count this as a tag
//             weight *= tagRateVec[inner];
//          } else {
//             // We didn't : Count as a no tag
//             weight *= (1 - tagRateVec[inner]);
//          }
//       } // for inner
//       sumWeight += weight;
//    } while ( CombEngine::nextCombination (comb, from) );
//
//  Note that in both cases, you can go from a given permutation or
//  combination back to an index using permVector2Index () and
//  combVector2Index () (see below for details).



#include <vector>
#include <set>

class CombEngine
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      typedef std::set< int >          ISet;
      typedef std::vector< int >       IVec;
      typedef std::vector< double >    DVec;
      typedef std::vector< long long > LVec;
      typedef IVec::iterator           IVecIter;
      typedef LVec::iterator           LVecIter;
      typedef ISet::const_iterator     ISetConstIter;
      typedef IVec::const_iterator     IVecConstIter;
      typedef DVec::const_iterator     DVecConstIter;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CombEngine &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CombEngine ();
      CombEngine (int from, int choose, int startAt = 0);

       ////////////////
      // One Liners //
      ////////////////
      void setStartAt (int startAt)
      { m_startAt = startAt; _setupDefaultVector(); }

      void reset() const { m_currentVec = m_defaultVec; }

      // return the number of possible permutations
      long long numPermutations() const { return m_permutations; }

      // return the number of possible combinations
      long long numCombinations() const { return m_combinations; }

      static long long calcNumPermutations (int from, int choose)  
      { return factorialRatio (from, choose) ; }

      static long long calcNumCombinations (int from, int choose)  
      { return factorialRatio (from, choose, from - choose) ; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // sets from and choose
      void setFromAndChoose (int from, int choose);

      // returns the IVec describing the nth combination
      // nth goes from 0..numCombinations - 1;
      IVec permutation (long long nth, int offset = 0) const;

      // returns the IVec describing the nth combination
      // nth goes from 0..numCombinations - 1;
      IVec combination (long long nth, int offset = 0) const;
      const IVec &combination (IVec &retval, long long nth, 
                               int offset = 0) const;


      // Given a (permutation) vector, return the unique index 
      long long vector2Index (IVec vec) const 
      { return permVector2Index (vec); }
      long long permVector2Index (IVec vec) const;
      // Given a (combination) vector, returns the unique index
      long long combVector2Index (const IVec &vec) const;


      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

      // returns an DVec (of size n + 1) of probabilities of tag
      // distribution probabilities (i.e., the probability of having 0
      // "tags", 1 "tags", etc) given an DVec (of size n) of per-item
      // "tag" rate.
      static void calcTagDistribution (const DVec &tagRateVec,
                                       DVec &tagDistributionVec);

      // Convert an IVec to an ISet
      static void IVec2ISet (const IVec &ivec, ISet &iset);

      // factorial function
      static long long factorial (int value);

      // factorial ratio function
      static long long factorialRatio (int numerator, int denominator);
      static long long factorialRatio (int numerator, int denom1, int denom2);

      // modifies comb to put the nth combination in it.  This routine
      // leaves 'combb' alone, so you had better clear it before
      // passing it in
      static void combination (IVec& comb, int from, int choose,
                               int nth, int offset);

      // returns the fist combination taking N at a time
      static IVec firstCombination (int dim);

      // returns true if it has successfully changed 'comb' to the
      // next combination
      static bool nextCombination (IVec &comb, int max);

      // Returns unique index given combination vector
      static long long combVector2Index (int from, int choose, IVec vec);

  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////

  private:
      //////////////////////////////
      // Private Member Functions //
      //////////////////////////////

      // sets up the default vector
      void _setupDefaultVector();

      // sets up the decoding vector
      void _setupDecodingVector();

      // get the nth component out of the current vector
      int _getNthComponent (int nth) const;

      /////////////////////////////////////
      // Private Static Member Functions //
      /////////////////////////////////////

      // remove the first entry and reduce all members greater than N
      static void _reduceVector (IVec &vec, int n);

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      
      IVec m_defaultVec;
      mutable IVec m_currentVec;
      LVec m_decodeVec;
      int m_from;
      int m_choose;
      int m_startAt;
      long long m_permutations;
      long long m_combinations;
};

std::ostream& operator<< (std::ostream& o_stream, 
                          const CombEngine::IVec &rhs);

std::ostream& operator<< (std::ostream& o_stream, 
                          const CombEngine::LVec &rhs);

// Example
// CombEngine comb (gFrom, gChoose);
// int count = 0;
// cout << "Combinations" << endl;
// IVec vec = CombEngine::firstCombination (gChoose);
// do
// {
//    long long which = CombEngine::combVector2Index (gFrom, gChoose,
//                                                    vec);
//    cout << setw(2) << count + 1 << ") " << comb.combination (count)
//         << " " << vec << " " << which << endl;
//    ++count;
// } while (CombEngine::nextCombination (vec, gFrom));
// cout << endl << "Permutations:" << endl;
// long long numComb = comb.numPermutations();
// for (long long loop = 0; loop < numComb; ++loop)
// {
//    cout << setw(2) << loop + 1 << ") " << comb.permutation (loop) << endl;
// } // for loop



#endif // CombEngine_H
