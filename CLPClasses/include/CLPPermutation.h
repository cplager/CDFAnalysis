// -*- C++ -*-

#if !defined(CLPPermutation_H)
#define CLPPermutation_H

#include <vector>
#include <map>

#include "TSystem.h"
#include "CLPClasses/CLPNamespace.h"
#include "CLPClasses/CombEngine.h"

class CLPPermutation  : public TObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      enum {kNumUserDoubles = 48};
      enum {kNumRanks = 4};
      enum {kNumBtagIndicies = 4}; // 2 * 2

      enum {
         kBJetTagged = 1 << 0,
         kWJetTagged = 1 << 1,
         kOtherJetTagged = 1 << 2,
      };

      typedef std::vector< CLPPermutation >    PermList;
      typedef std::vector< CLPPermutation* >   PermPtrList;
      typedef std::pair< int, int >            PermPair;
      typedef std::map< PermPair, PermPtrList> PermMap; 
      typedef PermMap::iterator                PermMapIter;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPPermutation &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPPermutation();

      bool operator< (const CLPPermutation& right) const;
      bool operator> (const CLPPermutation& right) const;

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      double chi2()   const { return m_chi2; }
      double mass()   const { return m_mass; }
      double mass2()  const { return m_mass2; }
      int    from()   const { return m_from; }
      int    choose() const { return m_choose; }
      int    type()   const { return m_type; }
      int    rank (int which)      const { return m_rank[which]; }
      int    btagIndex (int which) const { return m_btagIndex[which]; }
      bool   isHEPGMatched()       const { return m_isHEPGMatched; }
      long long combIndex()        const { return m_combIndex; }
      //long long btagFlag()         const { return m_btagFlag; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setChi2 (double chi2)   { m_chi2 = chi2; }
      void setMass (double mass)   { m_mass = mass; }
      void setMass2 (double mass2) { m_mass2 = mass2; }
      void setType (int type)      { m_type = type; }
      void setRank (int which, int rank)       { m_rank[which] = rank; }
      void setBtagIndex (int which, int index) { m_btagIndex[which] = index; }
      void setIsHEPGMatched (bool matched)     { m_isHEPGMatched = matched; }
      void setCombIndex (long long combIndex)  { m_combIndex = combIndex; }
      //void setBtagFlag (long long btagFlag)    { m_btagFlag = btagFlag; }
      void setFromAndChoose (int from, int choose)
      { m_from = from; m_choose = choose; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // return user double
      float userDouble (int which) const;

      // set user double
      void setUserDouble (int which, float value);

      // return the btag status using loose/tight tagging for permutation
      int btagStatus (bool useLoose = true) const;

      // return the number of btag jets
      int numBtagJets (bool useLoose = true) const;

      // return a PermPair to be used as the key in the PermMap.
      // 'true' for loose key and 'false' for tight key.
      PermPair BtagIndexPair (bool useLoose = true) const;

      // set loose and tight btag status
      void setLooseBtagStatus (int status);
      void setTightBtagStatus (int status);

      // set number of loose and tight btag jets
      void setNumLooseBtagJets (int numJets);
      void setNumTightBtagJets (int numJets);

      // boolean function to apply many cuts (to be used in Root)
      // numTags: must match exactly
      // status:
      //  -1 - don't check
      //   1 - proper bs tagged only (0x1)
      //   2 - one or more Ws tagged, no other
      //   3 - other jets tagged (0x4)
      // Rank:
      //  - Negative number - don't check
      //  - otherwise - upper limit
      bool select (bool useLoose, int numTags, int status = -1,
                   int localRank = -1, int globalRank = -1,
                   double chi2 = -1.);

      // return the IVec associated with the combination index.  If
      // 'nth' is not given, it returns the permutation for
      // 'm_combIndex'.
      CLPNamespace::IVec permutation (long long nth = -1);

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

  private:
      /////////////////////////
      // Private Member Data //
      /////////////////////////
      double     m_chi2;
      double     m_mass;
      double     m_mass2;
      int        m_from;
      int        m_choose;
      int        m_rank[kNumRanks];
      int        m_btagIndex[kNumBtagIndicies];
      int        m_type;
      bool       m_isHEPGMatched;
      long long  m_combIndex;
      long long  m_btagFlag;
      float      m_userDoubles[kNumUserDoubles];
      //CombEngine m_comb; //! don't store in ntuple

      ClassDef (CLPPermutation, 2) // CLP Permutation class

};


#endif // CLPPermutation_H
