// -*- C++ -*-

#if !defined(HEPGNames_H)
#define HEPGNames_H

#include <map>
#include <math.h>

class HEPGNames
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      typedef std::map< int, std::string > HNMap;
      typedef HNMap::iterator HNMapIter;

      enum 
      {
         kTop        = 6,
         kBottom     = 5,
         kCharm      = 4,         
         kStrange    = 3,
         kUp         = 2,
         kDown       = 1,
         kZ          = 23,
         kW          = 24,
         kElectron   = 11,
         kElectronNu = 12,
         kMuon       = 13,
         kMuonNu     = 14,
         kTau        = 15,
         kTauNu      = 16,
      }; // don't forget the ';' at the end of enums

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////

       ////////////////
      // One Liners //
      ////////////////

      // isa functions
      // quarks
      static bool isaUp         (int type) { return abs(type) == kUp; }
      static bool isaDown       (int type) { return abs(type) == kDown; }
      static bool isaCharm      (int type) { return abs(type) == kCharm; }
      static bool isaStrange    (int type) { return abs(type) == kStrange; }
      static bool isaTop        (int type) { return abs(type) == kTop; }
      static bool isaBottom     (int type) { return abs(type) == kBottom; }
      // Weak bosons                       
      static bool isaZ          (int type) { return abs(type) == kZ; }
      static bool isaW          (int type) { return abs(type) == kW; }
      // leptons                           
      static bool isaElectron   (int type) { return abs(type) == kElectron; }
      static bool isaElectronNu (int type) { return abs(type) == kElectronNu; }
      static bool isaMuon       (int type) { return abs(type) == kMuon; }
      static bool isaMuonNu     (int type) { return abs(type) == kMuonNu; }
      static bool isaTau        (int type) { return abs(type) == kTau; }
      static bool isaTauNu      (int type) { return abs(type) == kTauNu; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      static std::string name (int pdgType);

  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////

  private:

      /////////////////////////////////////
      // Private Static Member Functions //
      /////////////////////////////////////

      // initializes map
      static int initialize();


      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      static int sm_usedForInitialization;
      static HNMap sm_map;

};




#endif // HEPGNames_H
