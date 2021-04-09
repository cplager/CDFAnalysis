// -*- C++ -*-

#if !defined(TopNtupleDecoder_H)
#define TopNtupleDecoder_H

#include <map>

#include "TTree.h"
#include "TString.h"

class TopNtupleDecoder {
  
   public:
  
      TopNtupleDecoder(TTree *t);

      void readData(int entry);
  
      int getEleCollType     (TString name) const;
      int getMuoViewType     (TString name) const;
      int getJetCollType     (TString name) const;
      int getOfflTrkCollType (TString name) const;
      int getTrigSummaryBit  (TString name) const;

      bool isMonteCarlo() const;
      int  getProductionOfflineVersion() const;

      static void setVerboseMode (int verbose = -1)
      { sm_verbose = verbose; }
      static int  getVerboseMode ()
      { return sm_verbose; }

   private:

      TTree *m_treePtr;
      std::map< TString, int > m_electronCollTypes;
      std::map< TString, int > m_muonViewTypes;
      std::map< TString, int > m_jetCollTypes;
      std::map< TString, int > m_offltrackCollTypes;
      std::map< TString, int > m_trigSummaryBits;
      bool m_isMC;
      int  m_offlVers;
      static int sm_verbose;

};
  
#endif // TopNtupleDecoder_H
