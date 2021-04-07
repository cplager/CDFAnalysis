// -*- C++ -*-

#if !defined(CLPHEPGCont_H)
#define CLPHEPGCont_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TClonesArray.h"
#include "TClass.h"
#include "CLPClasses/CLPHEPGObject.h"
#include "CLPClasses/CLPJet.h"

class CLPHEPGObject;

class CLPHEPGCont : public TObject
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      //typedef std::vector< CLPHEPGObject > HEPList;

      enum
      {
         kTop1 = 0,
         kTop2,
         kZboson,
         kCharm,
         kLepton1,
         kLepton2,
         kWboson,
         kBottom,
         kLight1,
         kLight2,
      };

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const CLPHEPGCont &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      CLPHEPGCont();
      CLPHEPGCont(const CLPHEPGCont& rhs);

      ~CLPHEPGCont();

      // operators
      CLPHEPGCont &operator= (const CLPHEPGCont& rhs);
      CLPHEPGObject &operator() (int index);
      CLPHEPGObject  operator() (int index) const;
      //bool operator> (const CLPHEPGCont& rhs) const;
      //bool operator< (const CLPHEPGCont& rhs) const;

       ////////////////
      // One Liners //
      ////////////////
      int size() const { return m_hepgTcaPtr->GetEntriesFast(); }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // add object to tca
      void add (const CLPHEPGObject &object);

      // clears and empties everything
      void clear();

      // returns CLPHEPObject of index
      CLPHEPGObject &object (int index)
      { return operator() (index); }
      CLPHEPGObject object  (int index) const
      { return operator() (index); }

      // fills itself using HEPList
      // -- The line below ought to work, but doesn't seem to
      void fill (CLPHEPGObject::HEPList &heplist);
      // -- so I use the following line instead.
      //void fill (HEPList &heplist);
      //void fill (std::vector< CLPHEPGObject > &heplist);

      // returns invariant mass of nth particle
      double mass (unsigned int index) const;

      // returns true if the top quarks and their decays are
      // all in their expected places (Z -> ll, W -> jets)
      bool topFCNCdecayOk_2l() const;

      // returns true if the top quarks and their decays are
      // all in their expected places (Z -> ll, W -> l nu)
      bool topFCNCdecayOk_3l() const;

      // returns true if the top quarks and their decays are
      // all in their expected places (both W -> jets)
      bool topSMdecayOk_allhad() const;

      // returns true if all particles that should be matched are
      // matched
      bool allReconMatched(bool considerTracks = true) const;

      // same as above, but different
      bool allPossibleRecon () const;

      // resets the HEPG matching for all jet objects to the JetList
      // that is passed in
      void resetJetHEPGMatching(const CLPJet::JetList &jetList);

      // copies internal TCA to target
      void copyTcaToTarget (TClonesArray *target);

      // returns a string classifying the decay
      std::string classify() const;

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

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
      int m_numObjects;
      TClonesArray *m_hepgTcaPtr; //->

      ClassDef (CLPHEPGCont, 1) // CLP HEPG Container Class

};


#endif // CLPHEPGCont_H
