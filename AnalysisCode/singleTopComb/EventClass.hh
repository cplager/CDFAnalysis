// -*- C++ -*-

#if !defined(EventClass_HH)
#define EventClass_HH

#include <cassert>
#include <map>
#include <vector>
#include <string>

#include "TSystem.h"
#include "TH1F.h"


class EventClass
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      enum 
      {
         kNumAnalyses = 4
      };

      // typedefs
      typedef std::vector< double >     DVec;
      typedef std::vector< EventClass > Vec;
      typedef std::map< int, Vec >      IVMap;
      typedef DVec::iterator            DVecIter;
      typedef DVec::const_iterator      DVecConstIter;
      typedef Vec::iterator             VecIter;
      typedef Vec::const_iterator       VecConstIter;
      typedef IVMap::iterator           IVMapIter;
      typedef IVMap::const_iterator     IVMapConstIter;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const EventClass &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      EventClass();
      EventClass (int run, int event, int det, int alpgen, 
                  int jet, int tag, float weight);

       ////////////////
      // One Liners //
      ////////////////

      //////////////////////
      // Access Functions //
      //////////////////////
      int   run()    const { return m_run;    }
      int   event()  const { return m_event;  }
      char  det()    const { return m_det;    }
      char  alpgen() const { return m_alpgen; }
      char  jetBin() const { return m_jetBin; }
      char  tagBin() const { return m_tagBin; }
      float weight() const { return m_weight; }
      float analysis (int which) const { return m_analyses[which]; }

      ///////////////////////
      // Setting Functions //
      ///////////////////////
      void setRun    (int run)     { m_run    = run;    }
      void setEvent  (int event)   { m_event  = event;  }
      void setDet    (char det)    { m_det    = det;    }
      void setAlpgen (char alpgen) { m_alpgen = alpgen; }
      void setJetBin (char jetBin) { m_jetBin = jetBin; }
      void setTagBin (char tagBin) { m_tagBin = tagBin; }
      void setWeight (char weight) { m_weight = weight; }
      void setAnalysis (int which, float value)
      {  assert (which >= 0 && which < kNumAnalyses); 
         m_analyses[which] = value; }

      void scaleWeight (float scale) { m_weight *= scale; }

      // strings for keys of maps
      std::string J_T_Lstring () const
      { return Form ("j%dt%dl%d", jetBin(), tagBin(), det()); }
      std::string J_Tstring () const
      { return Form ("j%dt%d", jetBin(), tagBin()); }
      std::string Jstring () const
      { return Form ("j%dt%d", jetBin()); }
      std::string NNstring() const
      {if (2 == jetBin()) return J_Tstring(); else return Jstring(); }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      void setAllAnalyses (const DVec &analysesVec);

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      // Fills histogram with selected analysis UNWEIGHTED
      static void fillHist (const Vec &eventVec, TH1F *histPtr, int which,
                            const std::string &match = "");

  private:
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////

      int   m_run;
      int   m_event;
      char  m_det;
      char  m_alpgen;
      char  m_jetBin;
      char  m_tagBin;
      float m_weight;
      float m_analyses[kNumAnalyses];
};

#endif // EventClass_HH
