// -*- C++ -*-

#if !defined(FCBinClass_HH)
#define FCBinClass_HH

#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>

#include "TGraph.h"
#include "TMinuit.h"
#include "TH1F.h"

class FCBinClass
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      // typedefs
      typedef std::vector< int >          IVec;
      typedef std::vector< double >       DVec;
      typedef DVec::const_iterator        DVecConstIter;
      typedef std::list< FCBinClass >     FCBList;
      typedef FCBList::iterator           FCBListIter;
      typedef std::pair< double, double > DPair;
      typedef std::set< DPair >           DPairSet;
      typedef DPairSet::iterator          DPairSetIter;
      typedef std::vector< DPair >        DPairVec;
      typedef DPairVec::iterator          DPairVecIter;

      enum {kEmpty = -2};
      enum {kMaxDim = 3};
      enum {kMaxUserVariables = 5};

      enum LimitMode
      {
         kFeldmanCousinsMode,     // Multi-dim, with systematics
         kRootFeldmanCousinsMode, // Root's TFeldmanCousins
         kBayesMode,              // Profiled, integrated likelihood curve
         kNumLimitModes,
      };

      enum InitMode
      {
         kInitializeEverything,
         kScanExpectedLimits
      };

      // Flags to be used with 'debug()'
      enum
      {
         kGeneral_debug        = 0x1 << 0, // 1
         kExpectedLimit_debug  = 0x1 << 1, // 2
         kScanExpected_debug   = 0x1 << 2, // 4
      };

      static const double kMinAcceptance;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const FCBinClass &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      FCBinClass (int indicies [kMaxDim], bool forceFast = false);
      FCBinClass (int index1, int index2 = kEmpty, int index3 = kEmpty,
                  bool forceFast = false);

      ////////////////
      // One Liners //
      ////////////////
      // access functions
      int    rank()      { return m_rank;      }
      double muBest()    { return m_muBest;    }
      double P_muBest()  { return m_P_muBest;  }
      double likeliRat() { return m_likeliRat; }
      double p_mu()      { return m_P_mu;      }
      double prob()      { return m_P_mu;      }

      // setting functions
      void setRank      (int    rank) { m_rank      = rank; }
      void setLikeliRat (double rat ) { m_likeliRat = rat;  }
      void setP_mu      (double p_mu) { m_P_mu      = p_mu; }
      void setProb      (double p_mu) { m_P_mu      = p_mu; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // operator overloads
      bool operator> (const FCBinClass& other) const;
      bool operator< (const FCBinClass& other) const;

      // calculate muBest and P_muBest at construction.
      void calculateMuBest (bool forceFast = false);
      
      // Find P_muBest by minimizing -1*Poisson through TMinuit
      void find_max_P_mu ();

      // calculate P_mu and likeliRat every time there is a new 'trueValue'
      void updateLikeliRatio ();

      // returns true if all indicies match
      bool matches (const int indicies[kMaxDim]) const;
      bool matches (int index1, int index2 = kEmpty, 
                    int index3 = kEmpty) const;

      // return/set the nth index
      int  index    (int nth) const;
      void setIndex (int nth, int index);

      // get the indicies
      void getIndicies (int indicies[kMaxDim]) const;

   public:

      ///////////////////////////////////
      // ///////////////////////////// //
      // // Static Member Functions // //
      // ///////////////////////////// //
      ///////////////////////////////////

      // Static Inline Functions
      // doubles
      static double trueValue()         { return sm_trueValue;  }
      static double stepSize()          { return sm_stepSize;   }
      static double minProbSum()        { return sm_minProbSum; }
      // integers                       
      static int numDim()               { return sm_numDim; }
      static int numPEs()               { return sm_numPEs; }
      static int debugValue()           { return sm_debug;  }
      // booleans                       
      static bool fastMode()            { return sm_fastMode;   } 
      static bool autoUpdate()          { return sm_autoUpdate; } 
      static bool makePlots()           { return sm_makePlots;  }
      static bool debug (int mask = -1) 
      { return (bool) (sm_debug & mask);  }
      // Enum types                    
      static LimitMode mode()          { return sm_mode; }
      // static setting functions      
      static void setStepSize   (double size)    { sm_stepSize = size;     }
      static void setNumPEs     (int numPEs)     { sm_numPEs = numPEs;     }
      static void setFastMode   (bool fastMode)  { sm_fastMode = fastMode; }
      static void setAutoUpdate (bool au)        { sm_autoUpdate = au;     }
      static void setMode       (LimitMode mode) { sm_mode = mode;         }
      static void setDebug      (int debug)      { sm_debug = debug;       }
      static void orDebug       (int debug)      { sm_debug |= debug;      }
      // often called function
      static double _gaussExp (double shift) 
      { return exp ( - shift * shift / 2 ); }
      
      // Any necessary static initialization should be done here.
      // Called first.
      static int FCBinFirstCall();

      // To be called after setting all other static variables
      static void initialize (InitMode initmode = kInitializeEverything);

      // sets the number of dimensions.
      static void setNumDim (int numDim);

      // returns the number of bins
      static int numBins();

      // finds the upper and lower limit for a specified confidence
      // level. Non-physical values of percent will default to
      // sm_confidenceLevel
      static double findUpperLimit (double percent = -1.);
      static double findLowerLimit (double percent = -1.);
      static void   findLimits (double &lower, double &upper,
                                double &center, double percent = -1.);

      // Root's TFeldmanCousins upper and lower limit. Non-physical
      // values of percent will default to sm_confidenceLevel
      static double findTFCUpperLimit (double percent = -1.);
      static double findTFCLowerLimit (double percent = -1.);
      
      // Finds the expected limit for given background.  If given,
      // 'tooBig' is an overestimation of the limit.  If 'useTFC' is
      // true, the TFeldmanCousins class will be used to calculate the
      // limits.
      static double findExpectedLimit (double &tooBig, double &RMS,
                                       TH1F *expectedHistPtr = 0);
      static double findExpectedLimit () 
      { double tooBig, RMS; return findExpectedLimit (tooBig, RMS); }

      // Using histograms in 'sm_rootFile', this function scans
      // expected limits, modifying an 
      // std::vector< pair< double, double > > where the pair
      // represents cut value and expected limit respectively.
      // Function returns true if it was able to complete the scan
      // successfully.
      static bool scanExpectedLimits (DPairVec &pairVec);
      static bool scanExpectedLimits()
      { DPairVec pairVec; return scanExpectedLimits (pairVec); }

      // returns the most probable value
      static double findBestValue (int index1 = kEmpty,
                                   int index2 = kEmpty,
                                   int index3 = kEmpty);

      // dumps the best limits to stdout
      static void outputLimits (double percent = -1.);

      // sets new mu trueValue.  Then updates all likelihood ratios
      // and creates a new ranked list
      static void setTrueValue (double trueValue);

      // dumps all bins to stdout.
      static void printAllBins ();
      static void printAllBins (FCBList &fcblist);

      // dumps ranked bins to stdout.
      static void printRankedBins();
                  
      // name of current mode
      static std::string limitModeName();

      // returns background, its error, the acceptance * luminosity,
      // its error, or the number of observed events
      // for the first (1), second (2) , or third (3) "dimension"
      static double accLum    (int nth, bool useShiftedValue = false,
                               double value = -1.);
      static double back      (int nth, bool useShiftedValue = false,
                               double value = -1., bool useELfactor = false);
      static int    numObsv   (int nth, bool useShiftedValue = false);
      static int    SMindex   (int nth);
      static int    max       (int nth);

      // Setting functions
      static void setMax        (int nth, int max);
      static void setNumObsv    (int nth, int num);
      static void setNumObsv    (const int numObsv[kMaxDim]);
      static void _setSIndicies (const int indicies[kMaxDim]);

      // User variables
      static int         userInt    (int which);
      static bool        userBool   (int which);
      static double      userDouble (int which);
      static std::string userString (int which);
      static void setUserInt    (int which, int    value);
      static void setUserBool   (int which, bool   value);
      static void setUserDouble (int which, double value);
      static void setUserString (int which, const std::string &value);

      // This function returns the percent interval which the bin 
      // indicated covers.  'upper' is changed to tell you whether
      // this bin is the upper limit or lower limit.
      static double findBinPercentage (IVec &rankedBins,
                                       bool &isUpper, 
                                       int index1 = kEmpty, 
                                       int index2 = kEmpty,
                                       int index3 = kEmpty);
      static double findBinPercentage (bool &isUpper,
                                       int index1 = kEmpty,
                                       int index2 = kEmpty,
                                       int index3 = kEmpty);

      // sets acceptances, then update everything necessary
      static void setAccLums (double accLum1, 
                              double accLum2 = 1.,
                              double accLum3 = 1.);
      static void setAccLum (int nth, double accLum);

      // sets backgrounds and/or errors, then update everything necessary
      // (if autoUpdate)
      static void setBackgrounds (double back1, 
                                  double back2 = 0.,
                                  double back3 = 0.);
      static void setBack (int nth, double back);
      
      // In general, the user will call initialize() and never need
      // to call updateAll.  If, however, you change a background,
      // accLum, or its error, you need to call updateAll.
      // If you change the number of dimensions or one of the maxes,
      // then you need to call initialize instead.
      static void updateAll();

      // Returns a TGraph pointer for the region and step-size
      // requested.
      // No titles or axes are labeled
      // NOTE: User is responsible for deleting the pointer
      static TGraph *getTGraph (double lower, double upper, double step = 0.1);

      // Returns a TH1F pointer for a set of PEs.
      // No titles or axes are labeled.  Results are NOT normalized.
      // NOTE: User is responsible for deleting the pointer
      static TH1F *getPEhist (const std::string &histname = "peHist",
                              bool useELfactor = false);

      // same as above, but returns a FCBList.  Since this is not a pointer,
      // user doesn't need to do anything when she is done.
      // Results ARE normalized to total number of PEs.
      static FCBList getPElist (bool useELfactor = false);

      // verifies all variables are within allowed ranges
      static bool validateVariables();

      // dumps all variables to stdout
      static void dumpAllVariables (double trueValue = 0);

      //////////////////////////
      // Likelihood Functions //
      //////////////////////////

      // returns a TGraph of the likelihood curve.  User is
      // responsible for the TGraph pointer.
      static TGraph* probabilityGraph (bool forceNew = false);

      // returns the 'limit' upper limit using the pair set
      static double BayesUpperLimit (double percent = -1);

      // Function needed by TMinuit
      static void minuit_fcn (int &npar, double* grad, double &retval, 
                              double *par, int iflag);
      
      // test differences between _fastProbabilityAt() and
      // _minuitProbabilityAt().
      static void testDifferences();

   private:
      //////////////////////////////
      // Private Static Functions //
      //////////////////////////////

      // return the likelihood probability at a given value
      static double _fastProbabilityAt (double value);

      // returns the minuit calculated probability at a given value
      static double _minuitProbabilityAt (double value);

      // returns the likelihood given the current values of the minuit
      // fit
      static double _likelihoodFit();

      // fills the sm_pairSet with the likelihood curve
      static void _fillPairSet();

      // Throw all random numbers
      static bool _throwAllRandomNumbers (bool useELfactor = false);

      // gets bin content from the histPtr histogram, doing all
      // necessary casts to TH2F* or TH3F* depending on numDim().
      // index1..3 go from 0..max, NOT 1..max+1
      static int _getBinContent (TH1F *histPtr, int index1,
                                 int index2 = kEmpty, int index3 = kEmpty);
      
      // Runs pseudo-experiments to find the number of times we see
      // n observed events given b background events and muTrue.
      static void _generatePEs(TH1F *histPtr = sm_peHistPtr,
                               bool useELfactor = false);      

      // clears the list of FCBinClasses
      static void _clearList();

      // setup interal list of classes
      static void _createList(bool forceFast = false);
      static void _createList(FCBList &fcblist, bool forceFast = false);
      
      static void _fromAbsIndexToIndicies(int absIndex, int& index1, 
                                          int& index2, int& index3);
      static void _fromAbsIndexToIndicies (int absIndex, 
                                           int indicies[kMaxDim]);


      // updates ALL likelihood ratios and resorts the list
      static void _updateAllLikeliRatios(bool updateMuBest = false);

      // creates sm_rankList (ordered by likelihood ratio)
      static void _rankList();

      // returns P_mu given different shift parameters
      static double _p_mu_withshift (double mu);
      
      // setup minuit pointer
      static void _setupMinuit();

      // setup PE histogram
      static void _setupPEhist();

      // Non-zero Random Gauss
      static double _NZRandomGauss (double mean, double width);

      // calculate the number of shift parameters used
      static void _calculateNumShift();

      // zeros shift array
      static void _zeroShift();

      // resize the binList
      static void _resize (bool forceFast = false);

      // retrieves the bin contents as well as the edge, signed step,
      // and number of bins for a given TH1F pointer.  Uses
      // sm_fromHighToLow to decide whether to integrate from left to
      // right or right to left.

      // If numBins is greater than 0 when passed in to the function,
      // the function will check to see that the numBins, edge, and
      // step are consistent with the values passed in.
      static bool _getHistInfo (TH1F* histPtr, DVec &binIntegral, int &numBins, 
                                double &edge, double &step);

      ///////////////////////////////
      // ///////////////////////// //
      // // Private Member Data // //
      // ///////////////////////// //
      ///////////////////////////////

      ///////////////////////////////////////////////////////////////
      // Note: C++ will create a default copy constructor.  This   //
      // ONLY works if all of the data members themselves have     //
      // good default copy constructors.  Built-in types are fine, //
      // pointers are NOT.                                         //
      ///////////////////////////////////////////////////////////////

      int     m_indicies[kMaxDim];
      int     m_rank;
      double  m_muBest;
      double  m_P_muBest;
      double  m_P_mu;
      double  m_likeliRat;  

      ////////////////////////////////
      // Private Static Member Data //
      ////////////////////////////////
      static int     sm_numDim;
      static int     sm_modeInt;
      static int     sm_numPEs;
      static int     sm_numGrossPoints;
      static int     sm_debug;
      static bool    sm_fastMode;
      static bool    sm_autoUpdate;
      static double  sm_trueValue;
      static double  sm_stepSize;
      static double  sm_minProbSum;
      static double  sm_expectedDelta;
      static double  sm_confidenceLevel;
      static double  sm_resizeFactor;
      static double  sm_bayesUpperIntegral;
      static double  sm_minFactorChange;
      static double  sm_muDenom1;
      static double  sm_muDenom2;
      static double  sm_minDenomFactor;
      static double  sm_maxDenomFactor;

      static LimitMode   sm_mode;
      static TMinuit    *sm_minuitPtr;
      static TH1F       *sm_peHistPtr;
      static DPairSet    sm_pairSet;
      static bool        sm_showVariables;
      static bool        sm_davidCorr;
      static bool        sm_fromHighToLow;
      static bool        sm_makePlots;
      static int         sm_numShiftUsed;
      static int         sm_maxNumPETries;
      static int         sm_maxSize;
      static FCBList     sm_list;
      static FCBList     sm_rankList;
      static std::string sm_rootFile;

      // kMaxDim variables
      static int          sm_max            [kMaxDim];
      static int          sm_indicies       [kMaxDim];
      static int          sm_numObsv        [kMaxDim];
      static int          sm_numObsvPE      [kMaxDim];
      static double       sm_accLum         [kMaxDim];
      static double       sm_accLumMu       [kMaxDim];
      static double       sm_back           [kMaxDim];
      static double       sm_backMu         [kMaxDim];
      static double       sm_accLumErr      [kMaxDim][kMaxUserVariables];
      static double       sm_accLumMuErr    [kMaxDim][kMaxUserVariables];
      static double       sm_backErr        [kMaxDim][kMaxUserVariables];
      static double       sm_backMuErr      [kMaxDim][kMaxUserVariables];
      static std::string  sm_accLumHistname [kMaxDim];
      static std::string  sm_backHistname   [kMaxDim];
      static double       sm_accLumHistNorm [kMaxDim];
      static double       sm_backHistNorm   [kMaxDim];
      static double       sm_accLumFactor   [kMaxDim];
      static double       sm_backFactor     [kMaxDim];
      static double       sm_ELbackFactor   [kMaxDim];
      // user variables  
      static double       sm_shift       [kMaxUserVariables];
      static bool         sm_userBools   [kMaxUserVariables];
      static int          sm_userInts    [kMaxUserVariables];
      static double       sm_userDoubles [kMaxUserVariables];
      static std::string  sm_userStrings [kMaxUserVariables];
      static std::string  sm_optionsFile;
};

#endif // FCBinClass_HH
