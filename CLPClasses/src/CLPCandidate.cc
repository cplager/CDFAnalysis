// -*- C++ -*-
#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdio.h> // for sprintf
#include <assert.h>
#include <map>

#include "CLPClasses/CLPHEPGCont.h"
#include "CLPClasses/CLPCandidate.h"
#include "CLPClasses/CombEngine.h"
#include "CLPClasses/CLPFunctions.h"
#include "CLPClasses/dout.h"

using namespace CLPNamespace;
using namespace CLPFunctions;

using namespace std;

// static constants and member functions
ofstream CLPCandidate::sm_ofstream;

CLPCandidate::OutputType CLPCandidate::sm_outputType 
= CLPCandidate::kEventList;

CLPCandidate::CLPCandidate()
{
   m_runSection = m_entryIndex = -1;
   m_numElectrons = 0;
   m_numMuons     = 0;
   m_numTracks    = 0;
   m_numJets      = 0;
   m_numPerms     = 0;
   m_weight = 1;
   m_elecTcaPtr  = new TClonesArray(kElectronClassName,    2);
   m_muonTcaPtr  = new TClonesArray(kMuonClassName,        2);
   m_trackTcaPtr = new TClonesArray(kTrackClassName,       2);
   m_jetTcaPtr   = new TClonesArray(kJetClassName,         6);
   m_permTcaPtr  = new TClonesArray(kPermutationClassName, 2);
   m_bestIndex = -1;
}

CLPCandidate::CLPCandidate (const CLPCandidate& rhs)
{
   m_elecTcaPtr  = new TClonesArray(kElectronClassName,    2);
   m_muonTcaPtr  = new TClonesArray(kMuonClassName,        2);
   m_trackTcaPtr = new TClonesArray(kTrackClassName,       2);
   m_jetTcaPtr   = new TClonesArray(kJetClassName,         6);
   m_permTcaPtr  = new TClonesArray(kPermutationClassName, 2);
   *this = rhs;
}

CLPCandidate &
CLPCandidate::operator= (const CLPCandidate& rhs)
{
   // first copy the easy things
   m_runNumber    = rhs.m_runNumber;    
   m_eventNumber  = rhs.m_eventNumber; 
   m_runSection   = rhs.m_runSection;
   m_entryIndex   = rhs.m_entryIndex;
   m_rank         = rhs.m_rank;
   m_numElectrons = rhs.m_numElectrons;
   m_numMuons     = rhs.m_numMuons;    
   m_numTracks    = rhs.m_numTracks;    
   m_numJets      = rhs.m_numJets;
   m_numPerms     = rhs.m_numPerms;
   m_lepton0      = rhs.m_lepton0;     
   m_lepton1      = rhs.m_lepton1;     
   m_lepton2      = rhs.m_lepton2;     
   m_Zmass        = rhs.m_Zmass;
   m_weight       = rhs.m_weight;
   m_rank         = rhs.m_rank;
   // now copy the TClonesArrays
   copyTCA (rhs.m_elecTcaPtr,  m_elecTcaPtr);
   copyTCA (rhs.m_muonTcaPtr,  m_muonTcaPtr);
   copyTCA (rhs.m_trackTcaPtr, m_trackTcaPtr);
   copyTCA (rhs.m_jetTcaPtr,   m_jetTcaPtr);
   copyTCA (rhs.m_permTcaPtr,  m_permTcaPtr);
   return *this;
}

// operators are defined "backwards" so greater means 
// better
bool
CLPCandidate::operator> (const CLPCandidate& rhs) const
{
   double thisClose = fabs ( m_Zmass - kZmass );
   double rhsClose  = fabs ( rhs.m_Zmass - kZmass );
   return ( thisClose < rhsClose );
}

bool
CLPCandidate::operator< (const CLPCandidate& rhs) const
{
   double thisClose = fabs ( m_Zmass - kZmass );
   double rhsClose  = fabs ( rhs.m_Zmass - kZmass );
   return ( thisClose > rhsClose );
}

CLPCandidate::~CLPCandidate()
{
   Clear();
   delete m_elecTcaPtr;  m_elecTcaPtr  = (TClonesArray *) 0;
   delete m_muonTcaPtr;  m_muonTcaPtr  = (TClonesArray *) 0;
   delete m_trackTcaPtr; m_trackTcaPtr = (TClonesArray *) 0;
   delete m_jetTcaPtr;   m_jetTcaPtr   = (TClonesArray *) 0;
   delete m_permTcaPtr;  m_permTcaPtr  = (TClonesArray *) 0;
}

void 
CLPCandidate::Clear(Option_t *option)
{
   m_numElectrons = 0;
   m_numMuons     = 0;
   m_numTracks    = 0;
   m_numJets      = 0;
   m_numPerms     = 0;
   m_elecTcaPtr->Clear("C"); 
   m_muonTcaPtr->Clear("C"); 
   m_trackTcaPtr->Clear("C"); 
   m_jetTcaPtr->Clear("C"); 
   m_permTcaPtr->Clear("C"); 
}

void
CLPCandidate::addElectron (const CLPElectron &electron)
{
   TClonesArray &elecTca = *m_elecTcaPtr;
   new (elecTca[m_numElectrons++]) CLPElectron( electron );
   if (1 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton0 = (CLPChargedParticle) electron;
   } 
   else if (2 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton1 = (CLPChargedParticle) electron;
   }
   else if (3 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton2 = (CLPChargedParticle) electron;
   }
   else
   {
      cout << "WARNING: CLPCandidate: More than three leptons" << endl;
   }
}

void
CLPCandidate::addMuon (const CLPMuon &muon)
{
   TClonesArray &muonTca = *m_muonTcaPtr;
   new (muonTca[m_numMuons++]) CLPMuon( muon );
   if (1 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton0 = (CLPChargedParticle) muon;
   } 
   else if (2 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton1 = (CLPChargedParticle) muon;
   }
   else if (3 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton2 = (CLPChargedParticle) muon;
   }
   else
   {
      cout << "WARNING: CLPCandidate: More than three leptons" << endl;
   }
}

void
CLPCandidate::addTrack (const CLPTrack &track, 
                            const CLPTrack::MomentumType momentumType)
{
   // we can rescale the track if Et > Pt for tracks we think are
   // electrons
   CLPTrack rescaleTrack = track;
   rescaleTrack.rescaleMomAs(momentumType);

   TClonesArray &trackTca = *m_trackTcaPtr;
   new (trackTca[m_numTracks++]) CLPTrack( rescaleTrack );
   if (1 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton0 = (CLPChargedParticle) rescaleTrack;
   } 
   else if (2 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton1 = (CLPChargedParticle) rescaleTrack;
   }
   else if (3 == m_numElectrons + m_numMuons + m_numTracks)
   {
      m_lepton2 = (CLPChargedParticle) rescaleTrack;
   }
   else
   {
      cout << "WARNING: CLPCandidate: More than three leptons" << endl;
   }
} //CLPAddTrack

void
CLPCandidate::addJet (const CLPJet &jet)
{
   TClonesArray &jetTca = *m_jetTcaPtr;
   new (jetTca[m_numJets++]) CLPJet( jet );
}

void
CLPCandidate::addJets (const CLPJet::JetList& jetlist)
{
   int size = (int) jetlist.size();
   //cout << "size " << size << " number of starting jets "
   //     << m_numJets << endl;
   for (int loop = 0; loop < size; ++loop)
   {
      addJet( jetlist[loop] );
   } // for loop
   //cout << "number of jets: " << m_numJets << endl;
}

void
CLPCandidate::addJets (TClonesArray* jetTcaPtr)
{
   // copy the jets from the input jetTcaPtr into
   // the member data TCA ptr
   copyTCA (jetTcaPtr, m_jetTcaPtr);
   m_numJets = m_jetTcaPtr->GetEntriesFast();
}

void
CLPCandidate::getJets (TClonesArray* jetTcaPtr)
{
   // this copies FROM the member data TCA ptr
   // to the OUTPUT jetTcaPtr
   copyTCA (m_jetTcaPtr, jetTcaPtr);
}

void
CLPCandidate::getJets (CLPJet::JetList& jetlist)
{
   jetlist.clear();
   for (int loop = 0; loop < numJets(); ++loop)
   {
      jetlist.push_back( jet(loop) );
   } // for loop
}

void
CLPCandidate::addPermutation (const CLPPermutation &permutation)
{
   TClonesArray &permTca = *m_permTcaPtr;
   new (permTca[m_numPerms++]) CLPPermutation( permutation );
}

void
CLPCandidate::addPermutations (const CLPPermutation::PermList& permlist)
{
   int size = (int) permlist.size();
   //cout << "size " << size << " number of starting permutations "
   //     << m_numPerms << endl;
   for (int loop = 0; loop < size; ++loop)
   {
      addPermutation( permlist[loop] );
   } // for loop
   //cout << "number of perms: " << m_numPerms << endl;
}

void
CLPCandidate::addPermutations (TClonesArray* permTcaPtr)
{
   // copy the permutations from the input permTcaPtr into
   // the member data TCA ptr
   copyTCA (permTcaPtr, m_permTcaPtr);
   m_numPerms = m_permTcaPtr->GetEntriesFast();
}

void
CLPCandidate::getPermutations (TClonesArray* permTcaPtr)
{
   // this copies FROM the member data TCA ptr
   // to the OUTPUT permTcaPtr
   copyTCA (m_permTcaPtr, permTcaPtr);
}

void
CLPCandidate::getPermutations (CLPPermutation::PermList& permlist)
{
   permlist.clear();
   int numPerm = numPermutations();
   for (int loop = 0; loop < numPerm; ++loop)
   {
      permlist.push_back( permutation (loop) );
   } // for loop
}

int
CLPCandidate::numMatchedJets() const
{
   map <int, int> alreadyMatched;
   // get number of jets we've got
   int numJets = m_jetTcaPtr->GetEntriesFast();
   // loop over the jets
   int retval = 0;
   for (int loop = 0; loop < numJets; ++loop)
   {
      CLPJet *jetPtr = (CLPJet*) m_jetTcaPtr->At(loop);
      int hepg = jetPtr->taggedToHEPGIndex();
      // is it matched?
      if (hepg >= 0)
      {
         // to what HEPG particle is this tagged?
         // have we found this before?
         map< int, int >::iterator iter = alreadyMatched.find (hepg);
         if (iter == alreadyMatched.end())
         {
            // new match
            ++retval;
            alreadyMatched[hepg] = 1;
         }
         else
         {
            // already there
            ++alreadyMatched[hepg];
         }
      }
   } // for loop
   return retval;
}

//bool
//CLPCandidate::HEPGMatched() const
//{
//   return m_lepton0.isTagged() && m_lepton1.isTagged()
//      && (4 == numMatchedJets());
//}

void
CLPCandidate::calculate()
{
   // make sure we have at least two leptons
   if (m_numElectrons + m_numMuons + m_numTracks < 2)
   {
      cerr << "WARNING: CLPCandidate::calculate()" << endl 
           << "Called before we have two leptons" << endl
           << "nE " << m_numElectrons << " nM " << m_numMuons
           << " nT " << m_numTracks << endl;
      return;
   } else if ((m_lepton0.trackIdOrig() == m_lepton1.trackIdOrig()) &&
              (m_lepton0.trackIdOrig() >= 0))
   {
      cerr << "WARNING: CLPCandidate::calculate()" << endl
           << "Called with the same two leptons" << endl
           << "l1 " << m_lepton0 << endl
           << "l2 " << m_lepton1 << endl;
      return;
   }
   m_Zmass = (m_lepton0.mom4Vec() + m_lepton1.mom4Vec()).mass();
}

const CLPChargedParticle
&CLPCandidate::lepton (int which) const
{
   if (0 == which)
   {
      return m_lepton0;
   } else if (1 == which)
   {
      return m_lepton1;
   } else if (2 == which)
   {
      return m_lepton2;
   } else
   {
      cerr << "Error:: CLPCandidate::lepton(): Illegal value of index: "
           << which << endl;
      assert(0);
   }
   // you'll never get here, but just so the compile won't
   // complain
   return m_lepton0;
}

const CLPJet
&CLPCandidate::jet (int which) const
{
   // make sure that you are asking for a jet that we have
   if ((which < 0) || which >= numJets())
   {
      cerr << "Error:: CLPCandidate::jet(): Illegal value of index: "
           << which << endl;
      assert(0);   
   }
   CLPJet *jetPtr = (CLPJet *) m_jetTcaPtr->At(which);
   return *jetPtr;
}

const CLPPermutation
&CLPCandidate::permutation (int which) const
{
   // make sure that you are asking for a permutation that we have.
   // Do _NOT_ use 'numPermutations()' here because it uses
   // permutation.
   if ((which < 0) || which >= m_permTcaPtr->GetEntriesFast())
   {
      cerr << "Error:: CLPCandidate::permutation(): Illegal value of index: "
           << which << endl;
      assert(0);   
   }
   CLPPermutation *permutationPtr 
      = (CLPPermutation *) m_permTcaPtr->At(which);
   return *permutationPtr;
}

const CLPMuon
&CLPCandidate::muon (int which) const
{
   // make sure that you are asking for a muon that we have
   if ((which < 0) || which >= numMuons())
   {
      cerr << "Error:: CLPCandidate::muon(): Illegal value of index: "
           << which << endl;
      assert(0);   
   }
   CLPMuon *muonPtr = (CLPMuon *) m_muonTcaPtr->At(which);
   return *muonPtr;
}

const CLPTrack
&CLPCandidate::track (int which) const
{
   // make sure that you are asking for a track that we have
   if ((which < 0) || which >= numTracks())
   {
      cerr << "Error:: CLPCandidate::muon(): Illegal value of index: "
           << which << endl;
      assert(0);   
   }
   CLPTrack *trackPtr = (CLPTrack *) m_trackTcaPtr->At(which);
   return *trackPtr;
}

const CLPElectron
&CLPCandidate::electron (int which) const
{
   // make sure that you are asking for a electron that we have
   if ((which < 0) || which >= numElectrons())
   {
      cerr << "Error:: CLPCandidate::electron(): Illegal value of index: "
           << which << endl;
      assert(0);   
   }
   CLPElectron *electronPtr = (CLPElectron *) m_elecTcaPtr->At(which);
   return *electronPtr;
}

int
CLPCandidate::numJetsTagged (CLPJet::SVXTagType type) const
{
   // let's loop over all of our jets
   int num = numJets();
   if (num > 4)
   {
      num = 4;
   }
   int retval = 0;
   for (int loop = 0; loop < num; ++loop)
   {
      if (jet(loop).isBTagged (type))
      {
         ++retval;
      }
   } // for loop;
   return retval;
}

bool
CLPCandidate::isDifferentLepton (const CLPChargedParticle &lepton) const
{
   // make sure we have (at least) two leptons
   if (m_numElectrons + m_numMuons + m_numTracks < 2)
   {
      cerr << "Error:: CLPCandidate::isDifferentLepton(): Not enough leptons"
           << endl;
      return false;
   }
   // O.k., let's look at our leptons
   for (int loop = 1; loop < 2; ++loop)
   {
      const CLPChargedParticle *lepPtr;
      if (1 == loop)
      {
         lepPtr = &m_lepton0;
      } else {
         lepPtr = &m_lepton1;
      }
      // does lepton match this pointer
      if ( (lepton.cpType() == lepPtr->cpType() ) &&
           (lepton.index()  == lepPtr->index() ) )
      {
         // Oops, this is the same
         return false;
      }
   } // for loop
   // if we're still here after checking both leptons, we're
   // good to go
   return true;
}

bool 
CLPCandidate::hasaLepton (const CLPChargedParticle::ChargedParticleType 
                              type) const
{
   // First check the (upto) three leptons we have.  Note that if the
   // lepton is empty, it will have type 'kNone'.
   if (m_lepton0.isa(type))
   {
      return true;
   }
   if (m_lepton1.isa(type))
   {
      return true;
   }
   if (m_lepton2.isa(type))
   {
      return true;
   }
   // If we're still here, we don't have it.
   return false;
}


bool
CLPCandidate::passedCuts (unsigned int type1, 
                              unsigned int type2,
                              int zCut, int sign) const
{
   /////////////////
   // Z Mass Cuts //
   /////////////////
   if (1 == zCut)
   {
      if (m_Zmass < 50 || m_Zmass > 130)
      {
         return false;
      }
   } // loose Z mass cut
   else if (2 == zCut)
   {
      if (m_Zmass < 76 || m_Zmass > 106)
      {
         return false;
      }
   } // tight Z mass cut

   ////////////////////////
   // Opposite Sign Cuts //
   ////////////////////////
   if (0 == sign)
   {
      // should be exactly opposite sign
      if ( -1 != m_lepton0.sign() * m_lepton1.sign() )
      {
         return false;
      }
   }
   else if (1 == sign)
   {
      // should be exactly opposite sign
      if ( m_lepton0.sign() * m_lepton1.sign() >= 0 )
      {
         return false;
      }
   }
   // (2 == sign) means no sign cuts
   else if (3 == sign)
   {
      // should be exactly opposite sign
      if ( 1 != m_lepton0.sign() * m_lepton1.sign() )
      {
         return false;
      }
   }


   /////////////////////////
   // Lepton Quality Cuts //
   /////////////////////////

   // does the first lepton pass the first type of cuts
   // and the second lepton pass the second type of cuts?
   if (m_lepton0.isBitSet(type1) && m_lepton1.isBitSet(type2))
   {
      return true;
   }
   // does the first lepton pass the second type of cuts
   // and the second lepton pass the first type of cuts?
   if (m_lepton0.isBitSet(type2) && m_lepton1.isBitSet(type1))
   {
      return true;
   }
   // If we're still here, we didn't make it
   return false;
}

bool
CLPCandidate::jetEtCuts (double et1, double et2, 
                             double et3, double et4) const
{
   // first make sure we've got at least four jets
   if (numJets() < 4)
   {
      return false;
   }
   // first jet
   if (jet(0).Et() < et1)
   {
      return false;
   }
   // second jet
   if (jet(1).Et() < et2)
   {
      return false;
   }
   // third jet
   if (jet(2).Et() < et3)
   {
      return false;
   }
   // fourth jet
   if (jet(3).Et() < et4)
   {
      return false;
   }
   // If we're still here, then everything passed
   return true;
}


int
CLPCandidate::writeEvent() const
{
   if (! sm_ofstream)
   {
   } 
   else
   {
      sm_ofstream << *this << endl;
   }
   return 1;
}

double
CLPCandidate::best_tWb_Diff (OptMethodType method) const
{
   assert (numJets() >= 3);
   int from = numJets();
   int choose = 3;
   CombEngine perm (from, choose);
   int bestIndex = -1;
   double bestFOM = ns_errorFOM;
   int numPerm = perm.numPermutations();
   for (int outer = 0; outer < numPerm; ++outer)
   {
      IVec vec = perm.permutation (outer);
      CLPJet::JetList jetlist;
      for (int inner = 0; inner < choose; ++inner)
      {
         jetlist.push_back( jet( vec[inner] ) );
      } // for inner
      double fom = figureOfMerit_tWb (jetlist, method);
      //dout << fom << endl;
      if (fom < 0)
      {
         // didn't work
         continue;
      }
      if ( (bestIndex < 0) || (fom < bestFOM) )
      {
         bestFOM = fom;
         bestIndex = outer;
      }
   } // for outer
   m_bestIndex = bestIndex;
   if (bestFOM > ns_errorFOM)
   {
      return ns_errorFOM;
   } 
   return bestFOM;
}

double
CLPCandidate::best_ttbar_Diff (int method) const
{
   assert (numJets() >= 4);
   int from = numJets();
   if (method >= 13)
   {
      from = 4;
      method -= 5;
   }
   int choose = 4;
   CombEngine perm (from, choose);
   int bestIndex = -1;
   double bestFOM = ns_errorFOM;
   int numPerm = perm.numPermutations();
   CLP4Vector lep0 = m_lepton0.mom4Vec();
   CLP4Vector lep1 = m_lepton1.mom4Vec();
   for (int outer = 0; outer < numPerm; ++outer)
   {
      IVec vec = perm.permutation (outer);
      // Avoid double counting
      if (vec[kW1Jet] > vec[kW2Jet])
      {
         // to avoid double counting
         continue;
      }
      CLPJet::JetList jetlist;
      for (int inner = 0; inner < choose; ++inner)
      {
         jetlist.push_back( jet( vec[inner] ) );
      } // for inner
      double fom = figureOfMerit_ttbar (lep0, lep1, jetlist, method);
      //dout << fom << endl;
      if (fom < 0)
      {
         // didn't work
         continue;
      }
      if ( (bestIndex < 0) || (fom < bestFOM) )
      {
         bestFOM = fom;
         bestIndex = outer;
      }
   } // for outer
   m_bestIndex = bestIndex;
   if (bestFOM > ns_errorFOM)
   {
      return ns_errorFOM;
   }
   return bestFOM;
}

double
CLPCandidate::best_ttbar_Diff_onetag (int method) const
{
   int numjets = numJets();
   assert (numjets >= 4);
   // first, lets make sure that we can decide which is the b-tagged jet
   CLPJet::SVXTagType tagType = CLPJet::kLoose;
   if (1 != numJetsTagged (tagType))
   {
      tagType = CLPJet::kTight;
      // If we don't have exactly one tagged jet, fall back to
      // the old method
      if (1 != numJetsTagged (tagType))
      {
         return best_ttbar_Diff (method);
      }
   }
   // now, lets get the jet list assembled
   CLPJet::JetList mainList;
   for (int loop = 0; loop < numjets; ++loop)
   {
      if (jet(loop).isBTagged (tagType))
      {
         mainList.push_back( jet(loop) );
      } // if tagged
   } // for loop
   assert (1 == mainList.size());
   for (int loop = 0; loop < numjets; ++loop)
   {
      if (! jet(loop).isBTagged (tagType))
      {
         mainList.push_back( jet(loop) );
      } // if NOT tagged
   } // for loop

   // The b jet will be listed first, so start at 1 instead of 0 and
   // we reduce the 'from' and 'choose' by 1 as well.
   int from = numjets - 1;
   int choose = 4 - 1;
   CombEngine perm (from, choose, 1);
   int bestIndex = -1;
   double bestFOM = ns_errorFOM;
   int numPerm = perm.numPermutations();
   CLP4Vector lep0 = m_lepton0.mom4Vec();
   CLP4Vector lep1 = m_lepton1.mom4Vec();
   for (int outer = 0; outer < numPerm; ++outer)
   {
      IVec vec = perm.permutation (outer);
      CLPJet::JetList jetlist;
      jetlist.push_back( mainList[0] );
      for (int inner = 0; inner < choose; ++inner)
      {
         jetlist.push_back( mainList[ vec[inner] ] );
      } // for inner
      double fom = figureOfMerit_ttbar (lep0, lep1, jetlist, method);
      //dout << fom << endl;
      if (fom < 0)
      {
         // didn't work
         continue;
      }
      if ( (bestIndex < 0) || (fom < bestFOM) )
      {
         bestFOM = fom;
         bestIndex = outer;
      }
   } // for outer
   m_bestIndex = bestIndex;
   if (bestFOM > ns_errorFOM)
   {
      return ns_errorFOM;
   }
   return bestFOM;
}



void
CLPCandidate::fillMatchedJetIndicies (int matchedArray[kArraySize]) const
{
   // initialize the array
   for (int index = 0; index < kArraySize; ++index)
   {
      matchedArray[index] = -1;
   } // for index;
   // lets loop over our jets
   for (int index = 0; index < numJets(); ++index)
   {
      switch ( jet(index).taggedToHEPGIndex() )
      {
         case -1: 
            break; // don't bother
         case CLPHEPGCont::kBottom:
            matchedArray[kBottomJet] = index;
            break;
         case CLPHEPGCont::kLight1:
            matchedArray[kWDaughter1] = index;
            break;
         case CLPHEPGCont::kLight2:
            matchedArray[kWDaughter2] = index;
            break;
         case CLPHEPGCont::kCharm:
            matchedArray[kCharmJet] = index;
            break;
         default:
            // shoudln't be here
            break;
      } // switch                  
   } // for index
}

double
CLPCandidate::matched_tWb_Diff (OptMethodType method) const
{
   assert (numJets() >= 3);
   double bestFOM = ns_errorFOM;
   // o.k.  So let's get the three jets we need
   int matchedArray[kArraySize];
   fillMatchedJetIndicies (matchedArray);
   CLPJet::JetList jetlist;
   // we only need the first three jets
   for (int loop = 0; loop <= kWDaughter2; ++loop)
   {
      int jetIndex = matchedArray[loop];
      if (-1 == jetIndex)
      {
         // We didn't match this jet.  Get out now
         return bestFOM;
      }
      jetlist.push_back( jet( jetIndex ) );
   } // for inner
   double fom = figureOfMerit_tWb (jetlist, method);
   if (fom > ns_errorFOM || fom < 0)
   {
      return ns_errorFOM;
   }
   return fom;
}

double
CLPCandidate::matched_ttbar_Diff (OptMethodType method) const
{
   assert (numJets() >= 4);
   double bestFOM = ns_errorFOM;
   // o.k.  So let's get the three jets we need
   int matchedArray[kArraySize];
   fillMatchedJetIndicies (matchedArray);
   CLPJet::JetList jetlist;
   for (int loop = 0; loop < kArraySize; ++loop)
   {
      int jetIndex = matchedArray[loop];
      if (-1 == jetIndex)
      {
         // We didn't match this jet.  Get out now
         return bestFOM;
      }
      jetlist.push_back( jet( jetIndex ) );
   } // for inner
   double fom = figureOfMerit_tWb (jetlist, method);
   if (fom > ns_errorFOM || fom < 0)
   {
      return ns_errorFOM;
   }
   return bestFOM;
}

double
CLPCandidate::matched_W_mass (int fixmass) const
{
   assert (numJets() >= 4);
   // o.k.  So let's get the three jets we need
   int matchedArray[kArraySize];
   fillMatchedJetIndicies (matchedArray);
   // make sure we've matched the jets we need
   for (int loop = kWDaughter1; loop <= kWDaughter2; ++loop)
   {
      int jetIndex = matchedArray[loop];
      if (-1 == jetIndex)
      {
         // We didn't match this jet.  Get out now
         return -1.;
      }
   } // for inner
   CLP4Vector daut1Vec = 
      jet( matchedArray[kWDaughter1] ).mom4Vec(CLPJet::kLevel5,
                                               CLPJet::kLightJetScaleFactor);
   CLP4Vector daut2Vec = 
      jet( matchedArray[kWDaughter2] ).mom4Vec(CLPJet::kLevel5,
                                               CLPJet::kLightJetScaleFactor);
   if (fixmass)
   {
      double res1 = jet( matchedArray[kWDaughter1] ).lightJetAbsoluteWidth_TM();
      double res2 = jet( matchedArray[kWDaughter2] ).lightJetAbsoluteWidth_TM();
      fix4VectorsForMass (daut1Vec, daut2Vec, kWmass, res1, res2);
   }
   return (daut1Vec + daut2Vec).mass();
}

double
CLPCandidate::matched_tWb_mass (int fixWmass) const
{
   assert (numJets() >= 4);
   // o.k.  So let's get the three jets we need
   int matchedArray[kArraySize];
   fillMatchedJetIndicies (matchedArray);
   // make sure we've matched the jets we need
   for (int loop = kBottomJet; loop <= kWDaughter2; ++loop)
   {
      int jetIndex = matchedArray[loop];
      if (-1 == jetIndex)
      {
         // We didn't match this jet.  Get out now
         return -1.;
      }
   } // for inner
   CLP4Vector bottomVec = 
      jet( matchedArray[kBottomJet] ).mom4Vec(CLPJet::kLevel5,
                                              CLPJet::kBottomJetScaleFactor);
   CLP4Vector daut1Vec = 
      jet( matchedArray[kWDaughter1] ).mom4Vec(CLPJet::kLevel5,
                                               CLPJet::kLightJetScaleFactor);
   CLP4Vector daut2Vec = 
      jet( matchedArray[kWDaughter2] ).mom4Vec(CLPJet::kLevel5,
                                               CLPJet::kLightJetScaleFactor);
   CLP4Vector wBosonVec = daut1Vec + daut2Vec;
   if (1 == fixWmass)
   {
      // change W energy without changing anything else
      wBosonVec.setRestMass (kWmass);
   } else if (2 == fixWmass)
   {
      double res1 = jet( matchedArray[kWDaughter1] ).lightJetAbsoluteWidth_TM();
      double res2 = jet( matchedArray[kWDaughter2] ).lightJetAbsoluteWidth_TM();
      fix4VectorsForMass (daut1Vec, daut2Vec, kWmass, res1, res2);
      wBosonVec = daut1Vec + daut2Vec;
   } // 2 == fixmass
   return (bottomVec + wBosonVec).mass();
}

double
CLPCandidate::matched_tZc_mass(int fixZmass) const
{
   assert (numJets() >= 4);
   // o.k.  So let's get the three jets we need
   int matchedArray[kArraySize];
   fillMatchedJetIndicies (matchedArray);
   // make sure we've matched the jets we need
   int charmIndex = matchedArray[kCharmJet];
   if (-1 == charmIndex)
   {
      return -1.;
   }
   CLP4Vector charmVec = 
      jet( matchedArray[kCharmJet] ).mom4Vec(CLPJet::kLevel5,
                                             CLPJet::kLightJetScaleFactor);
   CLP4Vector zBosonVec = lepton(0).mom4Vec() + lepton(1).mom4Vec();
   if (1 == fixZmass)
   {
      // change W energy without changing anything else
      zBosonVec.setRestMass (kZmass);
   } else if (2 == fixZmass)
   {
      CLP4Vector 
         lepton1 = lepton(0).mom4Vec(),
         lepton2 = lepton(1).mom4Vec();
      double res1 = lepton1.mag() * 0.04;
      double res2 = lepton2.mag() * 0.04;
      fix4VectorsForMass (lepton1, lepton2, kZmass, res1, res2);
      zBosonVec = lepton(0).mom4Vec() + lepton(1).mom4Vec();
   }
   return (zBosonVec + charmVec).mass();
}

void
CLPCandidate::fillBest4Jets (OptMethodType method) const
{
   // this should not be called on a candidate with less than four jets
   assert (numJets() >= 4);
   int from = numJets();
   int choose = 4;
   CombEngine perm (from, choose);
   int bestIndex = -1;
   double bestFOM = ns_errorFOM;
   int numPerm = perm.numPermutations();
   CLP4Vector lep0 = m_lepton0.mom4Vec();
   CLP4Vector lep1 = m_lepton1.mom4Vec();
   for (int outer = 0; outer < numPerm; ++outer)
   {
      IVec vec = perm.permutation (outer);
      CLPJet::JetList jetlist;
      for (int inner = 0; inner < choose; ++inner)
      {
         jetlist.push_back( jet(0) );
      } // for inner
      double fom = figureOfMerit_ttbar (lep0, lep1, jetlist, method);
      if ( (bestIndex < 0) || (fom < bestFOM) )
      {
         bestFOM = fom;
         bestIndex = outer;
      }
   } // for outer
   m_best4jetIndicies =  perm.permutation (bestIndex);
}

bool 
CLPCandidate::hasTaggedJet (CLPJet::SVXTagType type) const
{
   for (int loop = 0; loop < numJets(); ++loop)
   {
      if (jet(loop).isBTagged(type))
      {
         return true;
      }
   } // for loop
   // still here?
   return false;
}

IVec
CLPCandidate::fourMatchedJets() const
{
   static const IVec kEmptyVec; // remains empty
   IVec retval;
   // put these in the same order as kBottomJet enum in Namespace
   const int kFourJetHEPGMatches[kNumJets] = 
      { 
         CLPHEPGCont::kBottom,
         CLPHEPGCont::kCharm,
         CLPHEPGCont::kLight1,
         CLPHEPGCont::kLight2,
      };
   for (int outer = 0; outer < kNumJets; ++outer)
   {
      bool found = false;
      for (int inner = 0; inner < numJets(); ++inner)
      {
         if (kFourJetHEPGMatches[outer] == jet(inner).taggedToHEPGIndex())
         {
            found = true;
            retval.push_back(inner);
            break;
         }
      } // for inner
      if (! found)
      {
         // this jet isn't found
         return kEmptyVec;
      }
   } // for outer
   // If we're still here, that means we've matched all four
   int w1index = retval[kW1Jet];
   int w2index = retval[kW2Jet];
   // make sure w1 has bigger Et than w2
   if (w1index > w2index)
   {
      // swap them
      retval[kW1Jet] = w2index;
      retval[kW2Jet] = w1index;
   }
   return retval;
}

void
CLPCandidate::fillFourMatchedJets() const
{
   m_HEPGmatchedJetIndicies = fourMatchedJets();
}

void
CLPCandidate::sphericityAplanarity (double &sphericity, 
                                    double &aplanarity) const
{
   CLP3Vector::V3List v3list;
   int jetsize = numJets();
   for (int loop = 0; loop < jetsize; ++loop)
   {
      v3list.push_back( jet(loop).mom3Vec() );
   } // for loop
   int leptonsize = numElectrons() + numMuons() + numTracks();
   if (leptonsize >= 2)
   {
      v3list.push_back (lepton (0).mom3Vec() + lepton(1).mom3Vec());
      if (leptonsize >= 3)
      {
         v3list.push_back (lepton (2).mom3Vec());
      }
   }
   CLPFunctions::sphericityAplanarity (sphericity, aplanarity, v3list);
}

double CLPCandidate::sphericity() const
{
   double sphericity, aplanarity;
   CLPCandidate::sphericityAplanarity (sphericity, aplanarity);
   return sphericity;
}

double CLPCandidate::aplanarity() const
{
   double sphericity, aplanarity;
   CLPCandidate::sphericityAplanarity (sphericity, aplanarity);
   return aplanarity;
}

int CLPCandidate::numPermutations (double maxChi2) const
{ 
   if (maxChi2 < 0)
   {
      return m_permTcaPtr->GetEntriesFast(); 
   }

   int numGoodPermutations = 0;
   int numEnt = m_permTcaPtr->GetEntries();
   for (int loop = 0; loop < numEnt; ++loop)
   { 
      if ( permutation (loop).chi2() <= maxChi2 )
      {
         ++numGoodPermutations;
      } // if chi2 low enough
   } // for loop
   return numGoodPermutations;
}

bool CLPCandidate::HEPGMatched(double maxChi2) const
{
   bool isMatched = false;
   int numEnt = m_permTcaPtr->GetEntries();
   // loop over all the permutations
   for ( int loop = 0; loop < numEnt; ++loop )
   {
      CLPPermutation *permPtr = (CLPPermutation*) m_permTcaPtr->At(loop);
      double permChi2 = permPtr->chi2();
      if ( maxChi2 > 0 && permChi2 > maxChi2 )
      {
         continue; // go to the next perm on the list
      }
      // if we have the wrong type, we shouldn't be here
      if ( permPtr->isHEPGMatched() )
      {
         isMatched = true; // set our flag to true
      }
   }
   return isMatched;
} // HEPGMatched


int CLPCandidate::GetHighestJetIndexHEPG() const
{
   int highestJetIndex = -1;
   int numEnt = m_jetTcaPtr->GetEntries();
   for ( int loop = 0; loop < numEnt; ++loop )
   {
      CLPJet *jetPtr = (CLPJet*) m_jetTcaPtr->At(loop);
      if ( jetPtr->taggedToHEPGIndex() > 0 )
      {
         if ( loop > highestJetIndex )
         {
            // only take it if it has a higher index
            highestJetIndex = loop;
         }
      } // if taggedToHEPGIndex 
   } // for 'loop'
   return highestJetIndex;
} // GetHighestJetIndexHEPG (Cheyne)

bool CLPCandidate::bottomMatched() const
{
   CombEngine perm(numJets(), 4);
   if (bestIndex() < 0 || bestIndex() >= perm.numPermutations() )
   {
      // bad bestmatch, return false
      cerr << "ERROR: bottomMatched(), bad bestIndex" << endl;
      return false;
   }
   IVec results = perm.permutation (bestIndex());
   if ( CLPHEPGCont::kBottom == jet(results[0]).taggedToHEPGIndex() )
   { 
      return true;
   }
   return false;
}

bool CLPCandidate::charmMatched() const
{
      CombEngine perm(numJets(), 4);
   if (bestIndex() < 0 || bestIndex() >= perm.numPermutations() )
   {
      // bad bestmatch, return false
      cerr << "ERROR: bottomMatched(), bad bestIndex" << endl;
      return false;
   }
   IVec results = perm.permutation (bestIndex());
   if ( CLPHEPGCont::kCharm == jet(results[3]).taggedToHEPGIndex() )
   { 
      return true;
   }
   return false;
}

bool CLPCandidate::WMatched() const
{
      CombEngine perm(numJets(), 4);
   if (bestIndex() < 0 || bestIndex() >= perm.numPermutations() )
   {
      // bad bestmatch, return false
      cerr << "ERROR: bottomMatched(), bad bestIndex" << endl;
      return false;
   }
   IVec results = perm.permutation (bestIndex());
   if ( ( CLPHEPGCont::kLight1 == jet(results[1]).taggedToHEPGIndex() ||
          CLPHEPGCont::kLight1 == jet(results[2]).taggedToHEPGIndex() ) &&
        ( CLPHEPGCont::kLight2 == jet(results[1]).taggedToHEPGIndex() ||
          CLPHEPGCont::kLight2 == jet(results[2]).taggedToHEPGIndex() ) )
   { 
      return true;
   }
   return false;
}

bool CLPCandidate::BWCMatched() const
{
      CombEngine perm(numJets(), 4);
   if (bestIndex() < 0 || bestIndex() >= perm.numPermutations() )
   {
      // bad bestmatch, return false
      cerr << "ERROR: bottomMatched(), bad bestIndex" << endl;
      return false;
   }
   IVec results = perm.permutation (bestIndex());
   if (   CLPHEPGCont::kBottom == jet(results[0]).taggedToHEPGIndex() &&
      ( ( CLPHEPGCont::kLight1 == jet(results[1]).taggedToHEPGIndex() ||
          CLPHEPGCont::kLight1 == jet(results[2]).taggedToHEPGIndex() ) &&
        ( CLPHEPGCont::kLight2 == jet(results[1]).taggedToHEPGIndex() ||
          CLPHEPGCont::kLight2 == jet(results[2]).taggedToHEPGIndex() ) ) &&
        ( CLPHEPGCont::kCharm  == jet(results[3]).taggedToHEPGIndex() ) )
   { 
      return true;
   }
   return false;
}

int CLPCandidate::HEPGIndex() const
{
   IVec HEPGResults = fourMatchedJets();
   if (HEPGResults.size() != 4)
   {
      // we didn't find all four jets or something...
      return -1;
   }
   CombEngine perm(numJets(), 4);
   return perm.vector2Index(HEPGResults);
}

void 
CLPCandidate::fillJetIndiciesVec(CLPCandidate::WhichJetCollection coll,
                                     IVec &ivec) const
{
   if (kAllJets == coll)
   {
      // use all jets
      for (int loop = 0; loop < numJets(); ++loop)
      {
         ivec.push_back( loop );
      } // for loop
   } 
   else if (kFourBestJets == coll)
   {
      // use the best guess jets only
      ivec = m_best4jetIndicies;
   }
   else 
   {
      // use the HEPG jets only
      ivec = m_HEPGmatchedJetIndicies;
   }
}

double
CLPCandidate::Gt (CLPCandidate::WhichJetCollection coll) const
{
   double retval = (m_lepton0.mom3Vec() + m_lepton1.mom3Vec()).Pt();
   IVec ivec;
   fillJetIndiciesVec (coll, ivec);
   int size = (int) ivec.size();
   if (! size)
   {
      // You need jets to make this measurement
      cerr << "CLPCandidate::Gt - called with no jets" << endl;
      return -1;
   }
   for (int loop = 0; loop < size; ++loop)
   {
      retval += jet(loop).mom3Vec().Pt();
   } // for loop
   return retval;
}

double 
CLPCandidate::transverseMass() const
{
   // start with the Z
   CLP3Vector zvec = (lepton(0).mom3Vec() + lepton(1).mom3Vec());
   double et = zvec.Et();
   CLP3Vector ptvec = zvec; // we'll get the transverse component at the end
   int size = numJets();
   if (size > 4)
   {
      size = 4;
   }
   for (int loop = 0; loop < size; ++loop)
   {
      et += jet(loop).Et();
      ptvec += jet(loop).mom3Vec();
   } // for loop
   double pt =  ptvec.transMag();
   return sqrt (et*et - pt*pt);
}


CLP3Vector
CLPCandidate::jetSumPt (WhichJetCollection coll) const
{
   IVec ivec;
   fillJetIndiciesVec (coll, ivec);
   CLP3Vector retval;
   int size = (int) ivec.size();
   if (! size)
   {
      // You need jets to make this measurement
      cerr << "CLPCandidate::Gt - called with no jets" << endl;
      return CLP3Vector();
   }
   for (int loop = 0; loop < size; ++loop)
   {
      retval += jet(loop).mom3Vec().transverse();
   } // for loop
   return retval;
}

CLP3Vector
CLPCandidate::zPt() const
{
   return (m_lepton0.mom3Vec() + m_lepton1.mom3Vec()).transverse();
}

double
CLPCandidate::jetSumPtMag (WhichJetCollection coll) const
{
   return jetSumPt(coll).mag();
}

double 
CLPCandidate::eventTagProb (CLPJet::SVXTagType tagger) const
{
   if (CLPJet::kDefaultTag == tagger)
   {
      tagger = CLPJet::defaultTagType();
   } // if default
   if (tagger >= CLPJet::kNumTagTypes)
   {
      cerr << "CLPCandidate::eventTagProb() error: Invalid tagger type."
           << endl;
      return -1.;
   } // if invalid
   int numjets = numJets();
   if (numjets > 4)
   {
      numjets = 4;
   }
   double zerotag = 1.;
   for (int jetIndex = 0; jetIndex < numjets; ++jetIndex)
   {
      zerotag *= 1 - jet(jetIndex).tagProb(tagger);
   } // for jetIndex
   return 1 - zerotag;
}

/////////////////////////////
// Static Member Functions //
/////////////////////////////

bool
CLPCandidate::openEventList (const string &filename)
{
   sm_ofstream.open ( filename.c_str(), ios::out );
   if (!sm_ofstream.good())
   {
      cout << "Didn't open sqwat: " << filename << endl;
      return false;
   } 
   else 
   {
      cout << "successfully opened " << filename << endl;
      if (CLPCandidate::kEventList == CLPCandidate::outputType())
      {
         sm_ofstream << "## entryIndex run event lep1_ID lep1_name "
                     << "lep2_ID lep2_name Zmass leadet "
                     << "numjets numloose numtight" << endl
                     << "# acc   leadet 0.01" << endl
                     << "# equiv lep1_ID lep2_ID" << endl
                     << "# swap  lep1_   lep2_" << endl;
      } 
      else if (CLPCandidate::kBtagProbList == CLPCandidate::outputType())
      {
         sm_ofstream << "## run event jet1prob jet2prob jet3prob jet4prob "
                     << "eventprob chi2 transmass" << endl
                     << "# acc jet1prob  0.0001" << endl
                     << "# acc jet2prob  0.0001" << endl
                     << "# acc jet3prob  0.0001" << endl
                     << "# acc jet4prob  0.0001" << endl
                     << "# acc eventprob 0.0001" << endl
                     << "# acc chi2      0.001" << endl
                     << "# acc transmass 0.01" << endl;
      }
      return true;
   }
}

void
CLPCandidate::closeEventList()
{
   if (sm_ofstream)
   {
      sm_ofstream.close();
   }
   //sm_ofstream = (ofstream) 0;
}

// friends
ostream& operator<< (ostream& o_stream, const CLPCandidate &rhs)
{
   if (CLPCandidate::kEventList == CLPCandidate::outputType())
   {
      o_stream << setw(7) << rhs.entryIndex() << " "
               << setw(6) << rhs.runNumber() 
               << setw (8) << rhs.eventNumber();
      for (int loop = 0; loop <= 1; ++loop)
      {
         const CLPChargedParticle &lepton = rhs.lepton(loop);
         if (lepton.isaElectron())
         {
            o_stream << "   e" << Form ("%-2d", lepton.index());
         } 
         else if (lepton.isaMuon())
         {
            o_stream << "   m" << Form ("%-2d", lepton.index());
         }
         else 
         {
            o_stream << "   t" << Form ("%-2d", lepton.index());
         }
         o_stream << "  " << setw(8) << lepton.shortTypeString();
      } // for loop
      o_stream << Form("  %6.2f", rhs.Zmass());
      // make sure we have at least one jet
      if (rhs.numJets())
      {
         o_stream << Form("  %6.2f", rhs.jet(0).Et());
      } else {
         o_stream << Form("  %6.2f", 0.);
      }
      o_stream << Form(" %2d", rhs.numJets())
               << Form(" %2d", rhs.numJetsTagged(CLPJet::kLoose))
               << Form(" %2d", rhs.numJetsTagged(CLPJet::kTight));
   } // if kEventList
   else if (CLPCandidate::kLeptonTrackList == CLPCandidate::outputType())
   {
      if (rhs.m_lepton0.isaMuon() && 
          rhs.m_lepton0.isaTightLepton() && 
          rhs.m_lepton1.isaTrack())
      {
         o_stream << Form( "%6d",    rhs.runNumber() )
                  << Form( "%8d",    rhs.eventNumber() )
                  << Form( "%10.3f", rhs.m_lepton0.mom3Vec().Px() )
                  << Form( "%9.3f",  rhs.m_lepton0.mom3Vec().Py() )
                  << Form( "%9.3f",  rhs.m_lepton0.mom3Vec().Pz() )
                  << Form( "%10.3f", rhs.m_lepton1.mom3Vec().Px() )
                  << Form( "%9.3f",  rhs.m_lepton1.mom3Vec().Py() )
                  << Form( "%9.3f",  rhs.m_lepton1.mom3Vec().Pz() )
                  << Form( "%9.3f",  rhs.Zmass() );
      } else {
         // lets not print anything but tight muons and tracks for now
         
      } // if a tight lepton      
   } // if kLeptonTrackList
   else if (CLPCandidate::kLeptonTrackIndexList == 
            CLPCandidate::outputType())
   {
      if (rhs.m_lepton0.isaMuon() && 
          rhs.m_lepton0.isaTightLepton() && 
          rhs.m_lepton1.isaTrack())
      {
         o_stream << Form( " %6d",   rhs.runNumber() )
                  << Form( " %8d",   rhs.eventNumber() )
                  << Form( "  m%d",  rhs.m_lepton0.index() )
                  << Form( " %9.3f", rhs.m_lepton0.mom3Vec().Px() )
                  << Form( " %9.3f", rhs.m_lepton0.mom3Vec().Py() )
                  << Form( " %9.3f", rhs.m_lepton0.mom3Vec().Pz() )
                  << Form( "  t%d",  rhs.m_lepton1.index() )
                  << Form( " %9.3f", rhs.m_lepton1.mom3Vec().Px() )
                  << Form( " %9.3f", rhs.m_lepton1.mom3Vec().Py() )
                  << Form( " %9.3f", rhs.m_lepton1.mom3Vec().Pz() )
                  << Form( " %9.3f", rhs.Zmass() );
      } else {
         // lets not print anything but tight muons and tracks for now
      } // if a tight lepton      
   } // if kLeptonTrackIndexList
   else if (CLPCandidate::kBtagProbList == 
            CLPCandidate::outputType())
   {
      o_stream << Form( " %6d",   rhs.runNumber() )
               << Form( " %8d",   rhs.eventNumber() );
      int numJets = rhs.numJets();
      // if (numJets < 4)
      // {
      //    // don't bother
      //    return o_stream;
      // }
      if (numJets > 4)
      {
         // use only leading four jets
         numJets = 4;
      }
      for (int jetIndex = 0; jetIndex < numJets; ++jetIndex)
      {
         o_stream << Form (" %0.4f", rhs.jet(jetIndex).tagProb());
      } // for jetIndex
      o_stream << Form (" %0.4f", rhs.eventTagProb());      
      double chi = -1;
      int numPerm = rhs.numPermutations();
      for (int permIndex = 0; permIndex < numPerm; ++permIndex)
      {
         const CLPPermutation &perm = rhs.permutation (permIndex);
         if (23 == perm.type())
         {
            chi = sqrt( perm.chi2() );
            break;
         }
      } // for permIndex
      o_stream << Form (" %7.4f  %6.2f", chi, rhs.transverseMass());
   } // if kBtagProbList
   else if (CLPCandidate::kDebugList == 
            CLPCandidate::outputType())
   {
      CLP4Vector total =  rhs.m_lepton0.mom4Vec() + rhs.m_lepton1.mom4Vec();
      o_stream << "run " << setw(6) << rhs.runNumber() << " event " 
               << setw (8) << rhs.eventNumber() << endl;
      o_stream << "  Z:  " 
               << rhs.m_lepton0.mom4Vec() + rhs.m_lepton1.mom4Vec() << endl;
      int numJets = rhs.m_jetTcaPtr->GetEntries();
      for (int loop = 0; loop < numJets; ++loop)
      {
         CLPJet *jetPtr = (CLPJet *) rhs.m_jetTcaPtr->At(loop);
         o_stream << "  j" << loop + 1 << ": " 
                  << jetPtr->mom4Vec(CLPJet::kLevel5) << endl;
         total += jetPtr->mom4Vec(CLPJet::kLevel5);
      } // for loop
      o_stream << "  t: " << total << endl;
      o_stream << "  tm:" << rhs.transverseMass() << endl;
   }
   else 
   {
      o_stream << "run " << setw(6) << rhs.runNumber() << " event " 
               << setw (8) << rhs.eventNumber();
      o_stream << "  Z mass: " << rhs.Zmass() << endl;
      o_stream << "l1: " << rhs.m_lepton0 << endl
               << "l2: " << rhs.m_lepton1 << endl;
      int numJets = rhs.m_jetTcaPtr->GetEntriesFast();
      for (int loop = 0; loop < numJets; ++loop)
      {
         CLPJet *jetPtr = (CLPJet *) rhs.m_jetTcaPtr->At(loop);
         cout << "j" << loop + 1 << ": " << *jetPtr << endl;
      } // for loop
   } // default case
   return o_stream;
} 


ClassImp (CLPCandidate)

