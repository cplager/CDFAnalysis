// -*- C++ -*-

#include <iostream>
#include <vector>
#include <algorithm>
#include "TString.h"

#include "SHyFT/Utilities/interface/CombEngine.h"

// This is a silly little class that holds all of the tagging
// information needed.  I recommend copying this to the analysis code.
class TagInfo
{
   public:
      typedef std::vector< TagInfo > Vec;
      typedef Vec::const_iterator    VecConstIter;

      TagInfo (double prob, char flavor, double mass) :
         m_prob (prob), m_flavor (flavor), m_mass (mass)
      {}

      double m_prob;
      char   m_flavor;
      double m_mass;
};



using namespace std;

int main()
{
   TagInfo::Vec tagVec;
   // For this test, I am going to assume that we have four jets that
   // are "taggable" and that they are listed in decending order of
   // Pt.  When hooking this up in the analysis code, note that you do
   // NOT need to list jets that have 0 probability (but that it won't
   // really hurt if you do).
   //
   //                         prob  flavor   secvtxmass
   tagVec.push_back( TagInfo (0.55,  'b',     4.) );
   tagVec.push_back( TagInfo (0.10,  'c',     2.) );
   tagVec.push_back( TagInfo (0.05,  'q',     1.) );
   tagVec.push_back( TagInfo (0.45,  'b',     3.) );

   ////////////////////
   // Print out jets //
   ////////////////////
   cout << "prob   flavor  secvtx mass" << endl;
   for (TagInfo::VecConstIter tvIter = tagVec.begin();
        tagVec.end() != tvIter;
        ++tvIter)
   {
      cout << Form ("%0.2f     %c       %.1f", 
                    tvIter->m_prob, tvIter->m_flavor, tvIter->m_mass) 
           << endl;
   } // for tvIter
   cout << endl;

   ////////////////////////////
   // Loop over all possible //
   // number of tagged jets  //
   ////////////////////////////
   // total probability of the event
   double grandTotalProb = 0.;
   int numJets = (int) tagVec.size();
   CombEngine::IVec combVec;
   CombEngine::ISet combSet;
   // from 0 to numJets inclusive
   for (int numTagged = 0; numTagged <= numJets; ++numTagged)
   {
      double sumProb = 0.;
      cout << "numTagged " << numTagged << endl;
      // get the first vector back of possible combinations
      // considering 'numTagged' jets
      combVec = CombEngine::firstCombination (numTagged);
      do
      {
         // Print out the tagged jets
         cout << "  ";
         for (CombEngine::IVecConstIter iter = combVec.begin();
              combVec.end() != iter;
              ++iter)
         {
            cout << "  " << *iter;
         }

         // Intialize counting variables for this combination.  We are
         // currently using the secvtx mass of the leading tagged jet.
         // This code also calculates the average secvtx mass of the
         // first two tagged jets (in case we decide to do this
         // again later).
         double prob = 1.;
         string flavor;
         double singleMass = 0.;
         double doubleMass = 0.;
         // convert vector into set
         CombEngine::IVec2ISet (combVec, combSet);
         for (int inner = 0; inner < numJets; ++inner)
         {
            CombEngine::ISetConstIter combSetIter = combSet.find (inner);
            // Did we find this index?
            if (combSet.end() != combSetIter)
            {
               // We found it : Count this as a b-tagged jet
               prob *= tagVec.at(inner).m_prob;
               flavor += tagVec.at(inner).m_flavor;
               if (! singleMass)
               {
                  // this is our first tagged jet
                  singleMass = tagVec.at(inner).m_mass;
               } else if (! doubleMass)
               {
                  // this is our second tagged jet
                  doubleMass = (singleMass + tagVec.at(inner).m_mass) / 2.;
               }
            } else {
               // We didn't : Count as a not a tagged jet
               prob *= 1 - tagVec.at(inner).m_prob;
            }
         } // for inner
         string doubleFlavor = flavor;
         if (doubleFlavor.size() >= 2)
         {
            // if we have more than two, take only flavor from the
            // first two leading tagged jets.
            doubleFlavor.resize(2);
            // Here I am taking advantage of the fact that 'b' is
            // before 'c' is before 'q' in the alphabet.
            if (doubleFlavor.at(0) > doubleFlavor.at(1))
            {
               swap (doubleFlavor.at(0), doubleFlavor.at(1));
            }
         }
         // make flavor only the flavor of the leading jet
         if (flavor.size())
         {
            flavor.resize(1);
         }

         ////////////////////////
         // Filling Histograms //
         ////////////////////////

         // O.k.  I'm not actually filling histograms here.  But this
         // is where you would do it in the analysis code.  Remember
         // to include any overall event weight: 
         // (e.g., prob -> eventWeight * prob).

         if (numTagged >= 2)
         {
            // In our current analysis code, we won't need this
            // option.
            cout << Form (" : (%.4f) - %s %.1f (%s %.1f)", prob, flavor.c_str(),
                          singleMass, doubleFlavor.c_str(), doubleMass)
                 << endl;
         } else if (numTagged)
         {
            cout << Form (" : (%.4f) - %s %.1f ", prob, flavor.c_str(),
                          singleMass)
                 << endl;
         } else {
            // if we're here, remember to fill the 0_tag templates
            // with the same value that was used for the pretag
            // templates.
            cout << Form (" : (%.4f)", prob)
                 << endl;
         }
         sumProb += prob;         
         // Done!  Now get next combination
      } while ( CombEngine::nextCombination (combVec, numJets) );

      // summary for this number of tags
      cout << "    total probability for " << numTagged 
           << " tags: " << sumProb << endl;
      grandTotalProb += sumProb;
   } // for numTagged

   // grand summary
   cout << "Grand total probability (for all tags): " << grandTotalProb << endl;
   return 0;
}
