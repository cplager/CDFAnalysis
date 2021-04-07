// -*- C++ -*-

#if !defined(FCPEs_HH)
#define FCPEs_HH

#include <map>
#include <vector>

#include "TSystem.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"

class FCPEs
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      // STL types
      typedef std::map< std::string, FCPEs > SFMap;
      typedef std::vector< double >          DVec;
      typedef std::map< std::string, DVec  > SDVecMap;
      typedef std::map< std::string, bool  > SBoolMap;
      typedef std::map< std::string, TH1F* > STH1FPtrMap;
      typedef std::map< std::string, TF1*  > STF1PtrMap;
      typedef std::vector< DVec >            DVecVec;
      // iterators
      typedef SFMap::iterator                SFMapIter;
      typedef SFMap::const_iterator          SFMapConstIter;
      typedef DVec::const_iterator           DVecConstIter;
      typedef SDVecMap::iterator             SDVecMapIter;
      typedef SDVecMap::const_iterator       SDVecMapConstIter;
      typedef SBoolMap::const_iterator       SBoolMapConstIter;
      typedef STH1FPtrMap::const_iterator    STH1FPtrMapConstIter;
      typedef STF1PtrMap::const_iterator     STF1PtrMapConstIter;
      // function pointer
      typedef Double_t (*TF1FuncPtrType) (Double_t*, Double_t*);

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const FCPEs &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      FCPEs ();
      FCPEs (const std::string &key, double trueValue,
             const std::string &peWild = "");

       ////////////////
      // One Liners //
      ////////////////

      // Access Functions
      std::string  key()       const { return m_key; }
      double       trueValue() const { return m_trueValue; }
      std::string  peWild()    const { return m_peWild; }
      TChain      *chainPtr()  const { return m_chainPtr; }

      // Setting Functions
      void setKey       (const std::string &key)    { m_key       = key; }
      void setTrueValue (double trueValue)          { m_trueValue = trueValue; }
      void setPeWild    (const std::string &peWild) { m_peWild    = peWild; }
      void setChainPtr  (TChain *chainPtr)          { m_chainPtr  = chainPtr; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // returns the number of files loaded
      int loadChain();

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

      // loads information
      static bool loadPEinfo (const std::string &filename);

      // loads all chains in PEmap
      static bool loadChains();

      // makes and fits vertical slices
      static bool makeVerticalSlices();
      static bool fitVerticalSlices();

      // fit P best
      static bool fitPbest();

      // load PE distributions to make likelihood ratio histograms
      static bool loadPEdists();

      // make likelihood ratio distributions
      static bool makeLikeliRats();

      // fit likelihood ratio distributions
      static bool fitLikeliRats();

      // plot the likelihood ratios with informations from the
      // acceptance bands.
      static bool plotLikeliRats();

      // plot the FC bands.
      static bool plotBands();

      // calculate acceptance bands
      static bool findAcceptanceBands();

      // fit upper band edge for expected limits
      static bool fitUpperBandEdge();

      // plot the expectedLimit
      static bool plotExpectedLimit();
      
      // initialize and terminate properly
      static void initialize (const std::string &controlName);
      static void terminate();

      // add a DVec to the map
      static void addNamedDVec (const std::string &name, const DVec &vec);

      // add functions to the map
      static TF1* addFittingFunction (const char* name, TF1FuncPtrType fcn, 
                                      Double_t xmin, Double_t xmax, 
                                      Int_t npar);
      static TF1* addFittingFunction (TF1* tf1Ptr);

      // seed the fit with the last version if possible.  Returns true
      // on success (false on failure).
      static bool seedFit (const std::string &funcName,
                           const std::string &vecName);
      static bool seedFit (TF1* funcPtr,
                           const std::string &vecName,
                           bool fixEverything = false);

      // gets all bin contents not including under and overflow
      static void getBinContents (const TH1F* histPtr,
                                  DVec &binContVec);

      // return fit function based on key
      static std::string likeliFitFuncName (const std::string &key);

      // routine to fit a given distribution with a given function.
      static bool StandAloneFitDistribution();

      // in 'original' string, replace 'alpha' with 'beta'
      static int replaceAlphaWithBeta (std::string &original,
                                       const std::string &alpha,
                                       const std::string &beta);

      // Inline functions
      static double lowerBound()      { return sm_lowerBound; }
      static double upperBound()      { return sm_upperBound; }
      static double lowerLikeliEdge() { return sm_lowerLikeliEdge; }
      static double upperLikeliEdge() { return sm_upperLikeliEdge; }
      static double lowerVertEdge()   { return sm_lowerVertEdge; }
      static double upperVertEdge()   { return sm_upperVertEdge; }
      static bool makeVerticalSlicesBool()
      { return sm_makeVerticalSlicesBool; }
      static bool fitVerticalSlicesBool()
      { return sm_fitVerticalSlicesBool; }
      static bool fitPbestBool() 
      { return sm_fitPbestBool; }
      static bool loadPEdistsBool()
      { return sm_loadPEdistsBool; }
      static bool makeLikeliRatBool()
      { return sm_makeLikeliRatBool; }
      static bool fitLikeliRatBool()
      { return sm_fitLikeliRatBool; }
      static bool plotLikeliRatBool()
      { return sm_plotLikeliRatBool; }
      static bool makeAccBandsBool()
      { return sm_makeAccBandsBool; }
      static bool plotBandsBool()
      { return sm_plotBandsBool; }
      static bool plotObservedValueBool()
      { return sm_plotObservedValueBool; }
      static bool fitUpperBandEdgeBool()
      { return sm_fitUpperBandEdge; }
      static bool plotExpectedLimitBool()
      { return sm_plotExpectedLimit; }
      static const std::string &SAFitFunc()
      { return sm_SAFitFunc; }

  private:

      /////////////////////////////////////
      // Private Static Member Functions //
      /////////////////////////////////////
      static int _staticInitialize();
      static void _correctStrings();

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      std::string  m_key;
      double       m_trueValue;
      std::string  m_peWild;
      TChain      *m_chainPtr;
      DVec         m_funcParamsVec;

      static std::string sm_treeName;
      static std::string sm_varName; 
      static std::string sm_lowerKey;
      static std::string sm_upperKey;
      static std::string sm_nominalSampleName;
      static std::string sm_sliceFitFuncName;
      static std::string sm_pbestFitFuncName;
      static std::string sm_lratFitFuncName;
      static std::string sm_lowerLratFitFuncName;
      static std::string sm_upperLratFitFuncName;
      static std::string sm_upperBandFitFuncName;
      static std::string sm_accBandName;
      static std::string sm_observedName;
      static std::string sm_trueName;
      static std::string sm_fcBandName;
      static std::string sm_peNameForm;
      static std::string sm_lrNameForm;
      static std::string sm_filePrefix;
      static double      sm_peValueFactor;
      static double      sm_peHistMax;
      static double      sm_lowerBound;
      static double      sm_upperBound;
      static double      sm_lowerLikeliEdge;
      static double      sm_upperLikeliEdge;
      static double      sm_lowerVertEdge;
      static double      sm_upperVertEdge;
      static double      sm_vertEdgeWidth;
      static double      sm_confidenceLevel;
      static double      sm_upperEdgeValue;
      static double      sm_lowerEdgeValue;
      static double      sm_upperExpectedValue;
      static double      sm_lowerExpectedValue;
      static double      sm_observedValue;
      static int         sm_numExpectedBins;
      static int         sm_numLikeliBins;
      static int         sm_numVertSlices;
      static int         sm_dummyVariable;
      static TCanvas    *sm_canvasPtr;
      static TFile      *sm_outputPtr;
    
      // Stand alone fitting variables
      static std::string sm_SAFitFunc;
      static std::string sm_SAFitParamName;
      static std::string sm_SAFitXValName;
      static std::string sm_SAFitYValName;
      static std::string sm_SAFitYErrName;
      static double      sm_SAFitYMax;
      static double      sm_SAFitYMin;
      static bool        sm_SAFitBatchMode;

      // control flags
      static bool sm_makeVerticalSlicesBool;
      static bool sm_fitVerticalSlicesBool;
      static bool sm_fitPbestBool;      
      static bool sm_loadPEdistsBool;
      static bool sm_plotPEdistsBool;
      static bool sm_makeLikeliRatBool;
      static bool sm_fitLikeliRatBool;
      static bool sm_plotLikeliRatBool;
      static bool sm_plotBandsBool;
      static bool sm_plotObservedValueBool;
      static bool sm_makeAccBandsBool;
      static bool sm_forceUpperEdgeValue;
      static bool sm_forceLowerEdgeValue;
      static bool sm_fitUpperBandEdge;
      static bool sm_plotExpectedLimit;
      
      // STL Objects
      static SFMap       sm_PEmap;
      static SDVecMap    sm_nameDVecMap;
      static SBoolMap    sm_nameDVecChangedMap;
      static STH1FPtrMap sm_vertSlicesHPtrMap;
      static STF1PtrMap  sm_tf1PtrMap;
};

#endif // FCPEs_HH
