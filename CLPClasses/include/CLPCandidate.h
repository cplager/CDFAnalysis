// -*- C++ -*-

#if !defined(CLPCandidate_H)
#define CLPCandidate_H

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TClonesArray.h"
#include "TClass.h"
#include "CLPClasses/CLPNamespace.h"
#include "CLPClasses/CLPElectron.h"
#include "CLPClasses/CLPMuon.h"
#include "CLPClasses/CLPTrack.h"
#include "CLPClasses/CLPJet.h"
#include "CLPClasses/CLPPermutation.h"

class CLPCandidate : public TObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      static std::ofstream sm_ofstream;

      enum WhichJetCollection
      {
         kAllJets,
         kFourBestJets,
         kHEPGMatchedJets,
      };

      enum OutputType
      { 
         kEventList,
         kEverything,
         kLeptonTrackList,
         kLeptonTrackIndexList,
         kBtagProbList,
         kDebugList,
      };

      enum {
         kBottomJet,
         kWDaughter1,
         kWDaughter2,
         kCharmJet,
         kArraySize
      };


      // Typedefs
      typedef std::list< CLPCandidate > CandidateList;
      typedef CandidateList::iterator CandidateListIter;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPCandidate &rhs);

      //////////////////////////
      // .  .      ._         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPCandidate();
      CLPCandidate (const CLPCandidate& rhs);
      
      ~CLPCandidate();

      // operators
      CLPCandidate &operator= (const CLPCandidate& rhs);
      bool operator> (const CLPCandidate& rhs) const;
      bool operator< (const CLPCandidate& rhs) const;

      ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      int runNumber() const          { return m_runNumber; }
      int eventNumber() const        { return m_eventNumber; }
      int runSection() const         { return m_runSection; }
      int entryIndex() const         { return m_entryIndex; }
      int rank() const               { return m_rank; }
      double Zmass() const           { return m_Zmass; }
      double weight() const          { return m_weight; }
      static OutputType outputType() { return sm_outputType; }
      int numJets() const            { return m_jetTcaPtr->GetEntries(); }
      int numMuons() const           { return m_muonTcaPtr->GetEntries(); }
      int numElectrons() const       { return m_elecTcaPtr->GetEntries(); }
      int numTracks() const          { return m_trackTcaPtr->GetEntries(); }
      int numPerms() const           { return m_permTcaPtr->GetEntries(); }
      int bestIndex() const          { return m_bestIndex; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setRunNumber (int runNumber)     { m_runNumber = runNumber; }
      void setEventNumber (int eventNumber) { m_eventNumber = eventNumber; }
      void setRunSection (int runSection)   { m_runSection = runSection; }
      void setEntryIndex (int entryIndex)   {  m_entryIndex = entryIndex; }
      void setWeight (double weight)        { m_weight = weight; }
      void setRank (int rank)               { m_rank = rank; }
      void sortJets()                       { m_jetTcaPtr->Sort(); }
      void setBestIndex( int bestIndex)     { m_bestIndex = bestIndex; }
      static void setOutputType (OutputType type) { sm_outputType = type; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // Root Clear function
      void Clear(Option_t *option ="");
      
      // add an electron
      void addElectron (const CLPElectron &electron);

      // add a muon
      void addMuon (const CLPMuon &muon);

      // add a track
      void addTrack (const CLPTrack &track, 
                     const CLPTrack::MomentumType momentumType = 
                     CLPTrack::kAsMuonMom);

      // add a jet
      void addJet (const CLPJet &jet);

      // add a bunch of jets
      void addJets (const CLPJet::JetList& jetlist);
      void addJets (TClonesArray *jetTcaPtr);

      // pass in a pointer to a TCA and get a copy of the jets
      void getJets (TClonesArray *jetTcaPtr);
      void getJets (CLPJet::JetList& jetlist);

      // add a permutation
      void addPermutation (const CLPPermutation &permutation);

      // add a bunch of permutations
      void addPermutations (const CLPPermutation::PermList& permlist);
      void addPermutations (TClonesArray *permutationTcaPtr);

      // pass in a pointer to a TCA/STL vector and get a copy of all
      // of the permutations
      void getPermutations (TClonesArray *permutationTcaPtr);
      void getPermutations (CLPPermutation::PermList& permlist);

      // returns number of matched-to-HEPG jets
      int numMatchedJets() const;

      // returns true if the Z and 4 jets are HEPG matched
      bool HEPGMatched_2l() const
      { return ZisHEPGMatched() && fourJetsAreHEPGMatched(); }

      // returns true if the Z, 2 jets, and third lepton are HEPG matched
      bool HEPGMatched_3l() const
      { return ZisHEPGMatched() 
           && twoJetsAreHEPGMatched()
           && m_lepton2.isTagged();
      }

      // returns true if Z is matched
      bool ZisHEPGMatched() const
      { return m_lepton0.isTagged() && m_lepton1.isTagged(); }

      // returns true if four jets are matched
      bool fourJetsAreHEPGMatched() const
      { return 4 == numMatchedJets(); }

      // returns true if four jets are matched
      bool twoJetsAreHEPGMatched() const
      { return 2 == numMatchedJets(); }

      // calculate derived quantities
      void calculate();      

      // returns a const reference to the lepton in question
      // where which = 1..3
      const CLPChargedParticle &lepton (int which) const;

      // returns a const reference to the jet in question
      // which 0..numJets - 1
      const CLPJet &jet (int which) const;

      // returns a const reference to the permutation in question
      // which 0..numPermutations - 1
      const CLPPermutation &permutation (int which) const;

      // returns a const reference to the muon in question
      // which 0..numMuons - 1
      const CLPMuon &muon (int which) const;

      // returns a const reference to the track in question
      // which 0..numTracks - 1
      const CLPTrack &track (int which) const;

      // returns a const reference to the electron in question
      // which 0..numElectrons - 1
      const CLPElectron &electron (int which) const;

      // returns number of b-tagged jets
      int numJetsTagged (CLPJet::SVXTagType type = CLPJet::kDefaultTag) const;

      // returns true if 'lepton' doesn't match either of the two
      // that are already stored
      bool isDifferentLepton (const CLPChargedParticle &lepton) const;

      // returns true if one of the leptons is correct type
      bool hasaLepton (const CLPChargedParticle::ChargedParticleType type)
         const;

      // returns true if all cuts are passed
      // 'type1' and 'type2' specify which lepton quality cuts.
      //    Passes iff:
      //    one lepton passes 'type1' cuts and the other passed 'type2'
      //    (as stored in user bits)
      // 'zCut' determines mass window:
      //    0 => no cut
      //    1 => [50, 130]
      //    2 => [76, 106]
      // 'sign' checks the relative sign (charge) of the leptons:
      //    0 => c1 * c2 == -1
      //    1 => c1 * c2 < 0 (allows for particles with no charge)
      //    2 => no cut at all
      //    3 => c1 * c2 == 1 (used for background studies
      bool passedCuts (unsigned int type1 = 0, unsigned int type2 = 0,
                       int zCut = 0, int sign = 0) const;

      // returns true if there are at least four jets and the jets
      // pass their respective Et cuts.
      bool jetEtCuts (double et1 = 15, double et2 = 15, 
                      double et3 = 15, double et4 = 15) const;


      // returns 1
      // Writes event out to sm_ofstream if it is open
      int writeEvent() const;

      // returns the best tWb mass difference
      double best_tWb_Diff  (CLPNamespace::OptMethodType method 
                             = CLPNamespace::kSquaredSum) const;

      // returns the HEPG matched tWb mass difference
      double matched_tWb_Diff (CLPNamespace::OptMethodType method 
                             = CLPNamespace::kSquaredSum) const;

      // returns the best ttbar mass difference
      double best_ttbar_Diff  (int method = 0) const;

      double best_ttbar_Diff_onetag  (int method = 0) const;

      // returns the HEPG matched ttbar mass difference
      double matched_ttbar_Diff  (CLPNamespace::OptMethodType method 
                                  = CLPNamespace::kSquaredSum) const;

      // returns the W mass for the matched W daughter jets
      double matched_W_mass (int fixmass = 0) const;

      // returns the t (-> W b) for the matched W and b daughter jets
      double matched_tWb_mass (int fixWmass = 1) const;

      // returns the t (-> Z c) for the matched W and b daughter jets
      double matched_tZc_mass (int fixZmass = 0) const;

      // fills array with the jet indicies of the four matched jets
      void fillMatchedJetIndicies (int array[kArraySize]) const;

      // return the indicies of the best 4 jets in the order of
      // bottom, w1, w2, charm
      void fillBest4Jets (CLPNamespace::OptMethodType method
                          = CLPNamespace::kSquaredSum) const;

      // returns true if any of the jets are loose (true) or tight
      // (false) secvtx tagged.
      bool hasTaggedJet (CLPJet::SVXTagType type = CLPJet::kDefaultTag) const;

      // if all four jets are matched, return an IVec with the four
      // if not, return an empty IVec
      CLPNamespace::IVec fourMatchedJets() const;

      // returns Gt (Ht not including MET)
      double Gt (WhichJetCollection coll = kAllJets) const;

      // Returns transverse mass
      double transverseMass() const;

      // Pt vector sum of the jets
      CLP3Vector jetSumPt (WhichJetCollection coll = kAllJets) const;
      double jetSumPtMag  (WhichJetCollection coll = kAllJets) const;

      // Pt vector of the Z
      CLP3Vector zPt() const;
      double zPtmag() const { return zPt().mag(); }

      void fillFourMatchedJets() const;

      // get sphericity and aplanarity
      void sphericityAplanarity (double &sphericity, double &aplanarity) const;
      double sphericity() const;
      double aplanarity() const;

      // returns the number of permutations less than maxChi2 (returns
      // number of all permutations if maxChi2 < 0)
      int numPermutations (double maxChi2 = -1.) const;

      // returns true if permutations are HEPG-matched
      // takes type=any by default (-1)
      bool HEPGMatched (double maxChi2 = -1.) const;

      // for a HEPG-matched permutation, returns the jetList index
      // of the highest-Et jet
      int GetHighestJetIndexHEPG() const;

      bool bottomMatched() const;
      bool charmMatched() const;
      bool WMatched() const;
      bool BWCMatched() const;
      int HEPGIndex() const;

      // Returns the event tag probability for a given tagger. If there
      // are more than four jets, only the four leading jets are used.
      double eventTagProb (CLPJet::SVXTagType tagger = CLPJet::kDefaultTag) const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      // open the event list file handle for writing
      bool openEventList (const std::string &filename);

      // close the event list file handle
      void closeEventList();

      // gets the requested jet indicies
      void fillJetIndiciesVec (WhichJetCollection coll,
                               CLPNamespace::IVec &ivec) const;
      
  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////

  private:
      
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////

      int m_runNumber;
      int m_eventNumber;
      int m_runSection;
      int m_entryIndex;
      int m_rank;
      int m_numElectrons;
      int m_numMuons;
      int m_numTracks;
      int m_numJets;
      int m_numPerms;
      double m_Zmass;
      double m_weight;
      CLPChargedParticle m_lepton0;
      CLPChargedParticle m_lepton1;
      CLPChargedParticle m_lepton2;
      TClonesArray *m_elecTcaPtr; //->
      TClonesArray *m_muonTcaPtr; //->
      TClonesArray *m_trackTcaPtr; //->
      TClonesArray *m_jetTcaPtr; //->
      TClonesArray *m_permTcaPtr; //->
      mutable CLPNamespace::IVec m_best4jetIndicies; //! Don't save in trees
      mutable CLPNamespace::IVec m_HEPGmatchedJetIndicies; //! Don't save 
      mutable int m_bestIndex; // best index that works with combinatorics

      // static member data
      static OutputType sm_outputType;

      ClassDef (CLPCandidate, 2) // CLP EventFCNC Candidate Class
};


#endif // CLPCandidate_H
