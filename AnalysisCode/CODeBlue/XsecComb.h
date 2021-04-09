// -*- C++ -*-

#if !defined(XsecComb_H)
#define XsecComb_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <iomanip>
#include <bitset>
#include "StringUtils.h"
//#include "CLPHisto.h"
#include "CLPTuple.h"
#include "TMinuit.h"
#include "TRandom.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"

class XsecComb
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////
      // modes for fitting cross sections (m_mode)
      enum
      {
         kSingleFit,   // 0
         kBkgnd_mode,  // 1
         kAccept_mode, // 2
         kBoth_mode,   // 3
      };

      // modes for measuring coverage (m_coverageMode)
      enum
      {
         kStats_only,     // 0 - measures stats coverage only
         kSyst_only,      // 1 - measures syst. coverage only
         kStats_and_Syst, // 2 - measures total coverage
      };

      // flags for what parts are turned on
      enum
      {
         kChisq,
         kLikeli,
         kConstraint,
         kNumFitModes
      };

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const XsecComb &rhs);

      // function for likelihood fit in minuit
      friend void likeli_fcn (Int_t &npar, Double_t *gin, Double_t &chisq, 
                              Double_t *par, Int_t iflag);

      // function for chi^2 fit in minuit
      friend void chisq_fcn (Int_t &npar, Double_t *gin, Double_t &chisq, 
                            Double_t *par, Int_t iflag);

      // function for Gaussian-constrained likelihood fit in minuit
      friend void constraint_fcn (Int_t &npar, Double_t *gin, Double_t &chisq, 
                                 Double_t *par, Int_t iflag);



      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      XsecComb();

       ////////////////
      // One Liners //
      ////////////////
      void setMode (int mode) { m_mode = mode; }
      // save tree or not
      static void setFillTree (bool fill) { m_fillTree = fill; }
      // number of trials
      static int numTrials() { return m_numTrials; }
      // total integrated luminosity
      static double getTotalPb() { return m_totalpb; }
      // sets the pointer to this
      void setThis() { m_ptr= this; }
      // are we fitting (as opposed to coverage MC)?
      bool isFitMode() { return m_isFitMode; }
      // number of sub groups
      unsigned int subGroups() const { return m_subGroups; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // load information from file
      void initializeWithFile (const std::string 
                               &filename = "standard.config",
                               int mode = 0);

      // setup all DVecs to the right size
      void setupEverything();

      void initialize();

      // zero wiggle info
      void zeroWiggle();

      // wiggle parameters according to mode
      void wiggle ();

      // clears all DVectors
      void clear();
      // clears everything needed for the single fit
      void clearFit();
      // clears everything needed for coverage MC
      void clearCoverage();

      // # of background events
      // Use this one
      double getBackground (unsigned int group) const;
      // post wiggle
      double background (unsigned int group) const;
      // given constraints
      double background (unsigned int group, double *constraints) const;

      // acceptance
      // Use this one
      double getAcceptance (unsigned int group) const;
      // post wiggle
      double acceptance (unsigned int group) const;
      // given constgraints
      double acceptance (unsigned int group, double *constraints) const;

      // # of observed events
      double observed (unsigned int group) const;

      // # of predicted events per subgroup as a function of sigma
      double predicted (double sigma, unsigned int group);
      double predicted (double sigma, unsigned int group, double *constraints);

      // chi^2 as a function of sigma
      double chisq (double sigma);

      // likeli as a function of sigma
      double likeli (double sigma);

      // likeli as a function of sigma
      double logLikeli (double sigma);

      // return the log of the Poisson probability
      static double logPoisson (double observed, double expected);

      // - 2 * log likeli using constraints
      double constrained (double sigma, double *constraints);

      // return whether or not a particular method should be used or not
      bool useFitMethod (int method);

      // get results from different fits
      void getChisqResults (double &center, double &plus, double &minus);
      void getLikeliResults (double &center, double &plus, double &minus);
      void getCnstrntResults (double &center, double &plus, double &minus);

      // copy results from another copy
      void copyResults (XsecComb &other);

      void getAllChisqResults  (double &statCenter, 
                                double &statLower,
                                double &statUpper,
                                double &maxCenter, 
                                double &maxLower,
                                double &maxUpper,
                                double &pctCenter, 
                                double &pctLower,
                                double &pctUpper);
      void getAllLikeliResults (double &statCenter, 
                                double &statLower,
                                double &statUpper,
                                double &maxCenter, 
                                double &maxLower,
                                double &maxUpper,
                                double &pctCenter, 
                                double &pctLower,
                                double &pctUpper);

      // project current likelihood into a histogram
      // m_likeliHist default
      void projectLikeli(CLPHisto &histo);
      void projectLikeli(); // m_likeliHist default

      void projectLogLikeli(CLPHisto &histo);
      

      // project current likelihood into a histogram
      void projectChisq(CLPHisto &histo);

      // run all minuit stuff at once
      void calculate (bool doMinos = false);

      // calculate statistic and systematic errors
      void calcStatAndSyst(int mode);

      // sets number of of groups and then zeros all DVecs
      void setSubGroups (unsigned int subGroups);

      // sets parameters using a name and a DVec of numbers
      void setParameters (const std::string &tag, DVec &values);

      // set which fits not to do
      void setFitFlags (long pattern);

      // sets particular DVec with input
      void setAcceptance (const DVec& input);
      void setBackground (const DVec& input);
      void setObserved (const DVec& input);
      
      // write the tree
      void writeTree(const std::string filename = "file.root");

      // calculate background and acceptance
      void calcBackAndAccept();

      // zero out either the accept part of the common error
      // or the background part of the common error if only checking
      // the background or acceptance error respectively
      void zeroBackOrAccptFromCommon();

      // return bin info (regular or zoom)
      void getBinInfo (double &lower, double &higher, int &bins, 
                    bool zoom = false);

      // returns the cross section if all bins are smooshed together
      double smooshedXsec();

      // check coverage
      void checkCoverage(int coverageMode);

      // calculated Zscore
      double calcZscore (double trueVal, double center, double upper,
                         double lower, int &coveredCount);

      double getParameter (std::string name);

      // needs to be called AFTER we know how many constraints we have
      void initializeGaussConstraints ();

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

      // should be called first
      static void initializeClass();

      // returns Gaussian distributed random number
      // (Default: unit Gaussian)
      static double randomGauss (double mean = 0., double sigma = 1.);

      // returns Poisson distributed random number
      static int randomPoisson (double mean);

      // returns uniformly distributed random number between
      // ]lower, upper]
      static double randomFlat (double lower, double upper);

   private:
      //////////////////////////////
      // Private Member Functions //
      //////////////////////////////

      // initialize the TTree
      void _setupTree();
      
      // initialize the coverage TTree
      void _setupCoverageTree();
      

      /////////////////////////
      // Private Member Data //
      /////////////////////////
      int m_mode;
      int m_coverageMode;
      unsigned int m_subGroups;
      unsigned int m_numAcceptPctWiggle; 
      unsigned int m_numAcceptAbsWiggle; 
      unsigned int m_numBkgndPctWiggle;
      unsigned int m_numBkgndAbsWiggle; 
      unsigned int m_numCommonPctWiggle;
      unsigned int m_numCommonAbsWiggle;
      DVec m_accept;
      DVec m_observed;
      DVec m_bkgnd;
      // acceptance errors
      DVecVec m_acceptPctErr;
      DVec m_acceptPctWiggle;
      DVecVec m_acceptAbsErr;
      DVec m_acceptAbsWiggle;
      // background errors
      DVecVec m_bkgndPctErr;
      DVec m_bkgndPctWiggle;
      DVecVec m_bkgndAbsErr;
      DVec m_bkgndAbsWiggle;
      // common errors
      // All of the other DVecVec will have one entry for each data point.
      // The m_commonPctErr will have two: the first for common acceptance 
      // error, the second for the common background error
      DVecVec m_commonPctErr; 
      DVec m_commonPctWiggle;
      DVecVec m_commonAbsErr; 
      DVec m_commonAbsWiggle;

      // keep mapping between which constraint is for which error
      IVec m_acceptPctCMap;
      IVec m_acceptAbsCMap;
      IVec m_bkgndPctCMap;
      IVec m_bkgndAbsCMap;
      IVec m_commonPctCMap;
      IVec m_commonAbsCMap;

      // number of constraints 
      // (only includes Abs errors, not Rel errors)
      unsigned int m_numConstraints;
      

      // to speed things up, store background and acceptance
      // postWiggle (PW)
      DVec m_backgroundPW;
      DVec m_acceptancePW;

      // used for likelihood
      CLPHisto m_likeliHist;
      CLPTuple m_chisqTuple;
      CLPTuple m_likeliTuple;
      int m_numBins;
      double m_binLowX;
      double m_binHighX;
      int m_numBinsZoom;
      double m_binLowXZoom;
      double m_binHighXZoom;
      std::string m_filename;

      // tree and associated variables
      TTree *m_tree;
      double m_tBack;
      double m_tAccept;
      double m_tChisqXsec;
      double m_tLikeliXsec;

      // Coverage MC variables
      // m_trueXsec is the value determined for each pseudo-trial
      // It is thrown using baseXsec and baseXsecErr
      double m_trueXsec; 
      DVec m_trueAccept;
      DVec m_trueBkgnd;
      DVec m_thrownBkgnd;
      double m_baseXsec;
      double m_baseXsecErr;
      // The four following vectors should all have the same length
      // Here you decide the "true" acceptance and background.
      DVec m_baseAccept;
      DVec m_baseAcceptErr;
      DVec m_baseBkgnd;
      DVec m_baseBkgndErr;
      
      double m_chisqPull1;
      double m_chisqPull2;
      double m_chisqPull3;
      double m_chisqPull4;
      double m_likeliPull1;
      double m_likeliPull2;
      double m_likeliPull3;
      double m_likeliPull4;
      
      // mode
      bool m_isFitMode;

      // the fit values
      double m_chisqValue, m_chisqPlus, m_chisqMinus;
      double m_likeliValue, m_likeliPlus, m_likeliMinus;
      double m_cnstrntValue, m_cnstrntPlus, m_cnstrntMinus;

      // calcStatAndSyst() answers
      // stat only answers
      double m_chisqStatCenter, m_chisqStatUpper, m_chisqStatLower;
      double m_likeliStatCenter, m_likeliStatUpper, m_likeliStatLower;

      // syst errors
      double m_chisqMaxCenter, m_chisqMaxUpper, m_chisqMaxLower;
      double m_chisqPctCenter, m_chisqPctUpper, m_chisqPctLower;
      double m_likeliMaxCenter, m_likeliMaxUpper, m_likeliMaxLower;
      double m_likeliPctCenter, m_likeliPctUpper, m_likeliPctLower;

      // flags as to which fits are turn OFF
      std::bitset< kNumFitModes > m_fitFlags;

      // set random parameters
      SDMap m_sdMap;

      // non-staticminuit pointer
      TMinuit *m_gaussConstMinuit;     

      ////////////////////////
      // Static Member Data //
      ////////////////////////

      static double m_totalpb;
      static double m_wiggleError;
      static int m_numTrials;
      static int m_numCoverageTrials;

      // pointer for minuit functions
      static XsecComb* m_ptr;

      // minuit pointers
      static TMinuit *m_chisqMinuit;
      static TMinuit *m_likeliMinuit;     

      static bool m_fillTree;
      static bool m_fittingVerbose;
};

#endif // XsecComb_H
