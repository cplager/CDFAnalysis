// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CLPBlue/XsecComb.hh"
#include "CLPBlue/CLPUtils.hh"
#include "TRandom.h"
#include "TMath.h"
//#define _DEBUG_QUIET
#include "CLPBlue/dout.hh"
#include <math.h>
#include <stdio.h> // for sprintf
#include <stdlib.h> // for atoi and atol

using namespace std;
using namespace CLPUtilNamespace;

// forward declarations
void likeli_fcn(Int_t &npar, Double_t *gin, Double_t &loglikeli, 
                Double_t *par, Int_t iflag);
void chisq_fcn(Int_t &npar, Double_t *gin, Double_t &chisq, 
               Double_t *par, Int_t iflag);
void constraint_fcn(Int_t &npar, Double_t *gin, Double_t &chisq, 
                    Double_t *par, Int_t iflag);


// static variables
double XsecComb::m_totalpb = 196.87;
double XsecComb::m_wiggleError = 1.00;
XsecComb *XsecComb::m_ptr = 0;
TMinuit *XsecComb::m_chisqMinuit = 0;
TMinuit *XsecComb::m_likeliMinuit = 0;
bool XsecComb::m_fillTree = true;
bool XsecComb::m_fittingVerbose = true;
int XsecComb::m_numTrials = 100;
int XsecComb::m_numCoverageTrials = 100;

XsecComb::XsecComb()
{
   m_gaussConstMinuit = 0;
   clear();
   clearCoverage();
}

void
XsecComb::clear()
{
   clearFit();
   clearCoverage();
   m_numBins = 2500;
   m_binLowX = 0;
   m_binHighX = 25;
   m_numBinsZoom = 600;
   m_binLowXZoom = 4;
   m_binHighXZoom = 10;
   m_mode = kSingleFit;
   m_ptr = this;
   m_isFitMode = true;
   m_fitFlags.reset();
   m_numTrials = 100000;
}

void
XsecComb::clearCoverage()
{
   m_tree = 0;   
   m_trueAccept.clear();
   m_trueBkgnd.clear();
   m_thrownBkgnd.clear();
   m_baseAccept.clear();
   m_baseAcceptErr.clear();
   m_baseBkgnd.clear();
   m_baseBkgndErr.clear();
   
}

void
XsecComb::clearFit()
{
   // DVecs
   m_accept.clear();
   m_observed.clear();
   m_bkgnd.clear();
   m_acceptPctErr.clear();
   m_acceptPctWiggle.clear();
   m_acceptAbsErr.clear();
   m_acceptAbsWiggle.clear();
   m_bkgndPctErr.clear();
   m_bkgndPctWiggle.clear();
   m_bkgndAbsErr.clear();
   m_bkgndAbsWiggle.clear();
   m_commonPctErr.clear();
   m_commonPctWiggle.clear();
   m_commonAbsErr.clear();
   m_commonAbsWiggle.clear();
   m_backgroundPW.clear();
   m_acceptancePW.clear();
   m_numAcceptPctWiggle = 0;
   m_numAcceptAbsWiggle = 0;
   m_numBkgndPctWiggle  = 0;
   m_numBkgndAbsWiggle  = 0;
   m_numCommonPctWiggle = 0;
   m_numCommonAbsWiggle = 0;
   m_chisqValue = m_chisqPlus = m_chisqMinus = 0.;
   m_likeliValue = m_likeliPlus = m_likeliMinus = 0.;
   m_subGroups = 0;
}

double
XsecComb::smooshedXsec()
{
   double accpt = 0;
   double bkgnd = 0;
   double obsrvd = 0;
   for (unsigned int loop = 0; loop < m_subGroups; ++loop)
   {
      //cout << "subgroup   " << loop << endl
      //     << "acceptance " << acceptance(loop) << endl
      //     << "background " << background(loop) << endl
      //     << "observed   " << observed(loop) << endl;
      accpt += acceptance(loop);
      bkgnd += background(loop);
      obsrvd += observed(loop);
   } // for loop
   assert (m_totalpb);
   assert (accpt);
   return (obsrvd - bkgnd) / (accpt * m_totalpb);
}

void
XsecComb::setSubGroups (unsigned int subGroups)
{
   clearFit();
   if (0 == subGroups)
   {
      // nothing else needs to be done
      return;
   }
   m_subGroups = subGroups;
   for (unsigned int loop = 0; loop < m_subGroups; ++loop)
   {
      m_accept.push_back(0.);
      m_observed.push_back(0.);
      m_bkgnd.push_back(0.);
   } // for loop
}

void
XsecComb::setAcceptance (const DVec& input)
{
   assert (input.size() == m_subGroups);
   m_accept = input;
}

void
XsecComb::setBackground (const DVec& input)
{
   assert (input.size() == m_subGroups);
   m_bkgnd = input;
}

void
XsecComb::setObserved (const DVec& input)
{
   assert (input.size() == m_subGroups);
   m_observed = input;
}

double
XsecComb::predicted(double sigma, unsigned int subgroup)
{
   return background(subgroup) +
      sigma * m_totalpb * acceptance(subgroup);
}

double
XsecComb::predicted(double sigma, unsigned int subgroup, 
                    double *constraints)
{
   return background(subgroup, constraints) +
      sigma * m_totalpb * acceptance(subgroup, constraints);
}

double
XsecComb::chisq(double sigma)
{
   double chisq = 0.;
   for (UInt_t subgroup=0; subgroup < m_subGroups; ++subgroup) 
   {
      Double_t prediction = predicted (sigma, subgroup);
      Double_t data = observed(subgroup);
      chisq += prediction - data;
      if (prediction > 0) 
      {
         chisq += data * log(data / prediction);
      }
   } // subgroup
   chisq *= 2.;
   return chisq;
}

double
XsecComb::likeli(double sigma)
{
   double likeli = 1.;
   for (UInt_t subgroup=0; subgroup < m_subGroups; ++subgroup) 
   {
      likeli *= 
         TMath::Poisson( observed(subgroup), predicted(sigma, subgroup) );
   } // subgroup
   return likeli;
}

double
XsecComb::constrained(double sigma, double *constraints)
{
   // We have two choices here.  We can either calculate the likelihood
   // (i.e. take product of several numbers) and then take the -2 * log 
   // at the end *OR*
   // We can can calculate the log likelihood (i.e. sum several numbers).
   // I chose the former.
   double loglikeli = 0.;
   //dout << TMath::Poisson (305, 324.) << endl;
   for (UInt_t subgroup=0; subgroup < m_subGroups; ++subgroup) 
   {
      loglikeli += 
         logPoisson ( observed (subgroup), 
                      predicted (sigma, subgroup, constraints) );
   } // subgroup
   // cout << "ll " << loglikeli << " xsec " << sigma 
   //      << " c " << constraints[1] << endl;
   // remember that constraints start at 1
   for (unsigned int loop = 1; loop <= m_numConstraints; ++loop)
   {
      loglikeli -= (constraints[loop] * constraints[loop]) / 2;
   } // for loop
   // unsigned int loop = 1;
   // cout << loglikeli << " " << constraints[loop] << " " 
   //      <<  (constraints[loop] * constraints[loop]) / 2 << endl;
   return -2 * loglikeli;
}

double
XsecComb::logPoisson (double observed, double expected)
{
   const double kMinDouble = -1.7976931348623157e+308;
  if (observed < 0)
  {
     return kMinDouble;
  }
  else if (observed == 0.0)
  {
     return -1 * expected;
  }
  else 
  {
     double retval =  observed * log(expected) - expected 
        - TMath::LnGamma(observed + 1.);
     if ( isnan(retval) )
     {
        return kMinDouble;
     } else {
        return retval;
     }
  }
}

double
XsecComb::logLikeli(double sigma)
{
   return -2 * log ( likeli(sigma) );
}

void
XsecComb::getChisqResults (double &center, double &plus, double &minus)
{
   center = m_chisqValue;
   plus   = m_chisqPlus;
   minus  = -1 * m_chisqMinus;
}

void 
XsecComb::getAllChisqResults (double &statCenter, 
                              double &statLower,
                              double &statUpper,
                              double &maxCenter, 
                              double &maxLower,
                              double &maxUpper,
                              double &pctCenter, 
                              double &pctLower,
                              double &pctUpper)
{
   statCenter = m_chisqStatCenter;
   statLower  = -1 * m_chisqStatLower;
   statUpper  = m_chisqStatUpper;
   maxCenter = m_chisqMaxCenter;
   maxLower  = -1 * m_chisqMaxLower;
   maxUpper  = m_chisqMaxUpper;
   pctCenter = m_chisqPctCenter;
   pctLower  = -1 * m_chisqPctLower;
   pctUpper  = m_chisqPctUpper;
}


void
XsecComb::getLikeliResults (double &center, double &plus, double &minus)
{
   center = m_likeliValue;
   plus   = m_likeliPlus;
   minus  = -1 * m_likeliMinus;
}

void 
XsecComb::getAllLikeliResults (double &statCenter, 
                               double &statLower,
                               double &statUpper,
                               double &maxCenter, 
                               double &maxLower,
                               double &maxUpper,
                               double &pctCenter, 
                               double &pctLower,
                               double &pctUpper)
{
   statCenter = m_likeliStatCenter;
   statLower  = -1 * m_likeliStatLower;
   statUpper  = m_likeliStatUpper;
   maxCenter = m_likeliMaxCenter;
   maxLower  = -1 * m_likeliMaxLower;
   maxUpper  = m_likeliMaxUpper;
   pctCenter = m_likeliPctCenter;
   pctLower  = -1 * m_likeliPctLower;
   pctUpper  = m_likeliPctUpper;
}

void
XsecComb::getCnstrntResults (double &center, double &plus, double &minus)
{
   center = m_cnstrntValue;
   plus   = m_cnstrntPlus;
   minus  = -1 * m_cnstrntMinus;
}


void
XsecComb::projectLogLikeli(CLPHisto &histo)
{
   unsigned int numBins = histo.numBins();   
   for (unsigned int bin = 1; bin <= numBins; ++bin)
   {
      double sigma = histo.binToX (bin);
      histo.fill( sigma, -0.5 * logLikeli(sigma) );
   } // for bin
}

void
XsecComb::projectLikeli(CLPHisto &histo)
{
   unsigned int numBins = histo.numBins();   
   for (unsigned int bin = 1; bin <= numBins; ++bin)
   {
      double sigma = histo.binToX (bin);
      histo.fill( sigma, likeli(sigma) );
   } // for bin
}

void
XsecComb::projectLikeli()
{
   projectLikeli (m_likeliHist);
}

void
XsecComb::projectChisq(CLPHisto &histo)
{
   unsigned int numBins = histo.numBins();   
   for (unsigned int bin = 1; bin <= numBins; ++bin)
   {
      double sigma = histo.binToX (bin);
      histo.fill( sigma, chisq(sigma) );
   } // for bin
}

void
XsecComb::initialize()
{
   if (m_fillTree) 
   {
      if (m_isFitMode)
      {
         // setup tree for fitting mode
         _setupTree();
      } else {
         // setup tree for coverage mode
         _setupCoverageTree();
      }
   }
   calcBackAndAccept();
}

void 
XsecComb::initializeWithFile (const string &filename, int mode)
{
   m_filename = filename;
   ifstream source(filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "file " << filename << " could not be opened" << endl;
      assert(0);
   }
   cerr << "# Loading file " << filename << endl;
   string line;
   while (getline (source, line))
   {
      SVec sList;
      DVec values;
      split (sList, line);
      int size = sList.size();
      if (size < 1)
      {
         continue;
      }
      // size must be at least 1
      if ("fitMode" == sList[0])
      {
         m_isFitMode = true;
         continue;
      }
      if ("coverageMode" == sList[0])
      {
         m_isFitMode = false;
         continue;
      }
      if (size < 2)
      {
         continue;
      }
      // size must be at least 2
      for (int loop = 1; loop < size; ++loop)
      {
         values.push_back ( atof( sList[loop].c_str() ) );
      } // for loop         
      if ("+" == sList[0])
      {
         // we had better have three numbers here
         assert (values.size() >= 3);
         m_accept.push_back( values[0] );
         m_observed.push_back( values[1] );
         m_bkgnd.push_back( values[2] );
         m_subGroups = m_accept.size();
         continue;
      }
      if ("fitFlags" == sList[0])
      {
         setFitFlags (atol (sList[1].c_str()) );
         continue;
      }
      if ("parameter" == sList[0])
      {
         if (size < 3)
         {
            continue;
         }
         string key = sList[1];
         double value = atof (sList[2].c_str());
         m_sdMap[ key ] = value;
         // cout << "assiging " << m_sdMap[ key ]
         //      << " to " << key << endl;
         continue;
      }
      setParameters (sList[0], values);
   } // while getline
   if (m_isFitMode)
   {
      m_mode = mode;
   } else {
      m_coverageMode = mode;
   }
   setupEverything();
   // check fitFlags
   //if (useFitMethod (kChisq))
   //{
   //   cerr << "using chisq" << endl;
   //} else {
   //   cerr << "NOT using chisq" << endl;
   //}
   //if (useFitMethod (kLikeli))
   //{
   //   cerr << "using likeli" << endl;
   //} else {
   //   cerr << "NOT using likeli" << endl;
   //}
   //  cerr << "finished loading file" << endl;
   //  cerr << "total Pb " << m_totalpb << endl;
}

double 
XsecComb::getParameter (string name)
{
   if (!m_sdMap.count(name))
   {
      cerr << "Error: Parameter '" << name << "' not found." << endl << endl;
      //assert(0);
      return -1;
   }
   return m_sdMap[ name ];
}

void
XsecComb::setFitFlags (long pattern)
{
   m_fitFlags = bitset< kNumFitModes > (pattern);
}

void
XsecComb::setupEverything()
{
   ////////////////////////////////////
   // Make sure everything is kosher //
   ////////////////////////////////////

   m_numAcceptPctWiggle = m_acceptPctErr.size();
   m_numAcceptAbsWiggle = m_acceptAbsErr.size();
   m_numBkgndPctWiggle  = m_bkgndPctErr.size();
   m_numBkgndAbsWiggle  = m_bkgndAbsErr.size(); 
   m_numCommonPctWiggle = m_commonPctErr.size();
   m_numCommonAbsWiggle = m_commonAbsErr.size();
   // loop over four DVecVecs
   for (int outer = 1; outer <= 6; ++outer)
   {
      DVecVec *listPtr = NULL;
      DVec *wigglePtr = NULL;
      string listName;
      unsigned int sizePer = m_subGroups;
      switch (outer)
      {
         case 1:
            listPtr = &m_acceptPctErr;
            wigglePtr = &m_acceptPctWiggle;
            listName = "acceptPctErr";
            break;
         case 2:
            listPtr = &m_acceptAbsErr;
            wigglePtr = &m_acceptAbsWiggle;
            listName = "acceptAbsErr";
            break;
         case 3:
            listPtr = &m_bkgndPctErr;
            wigglePtr = &m_bkgndPctWiggle;
            listName = "bkgndPctErr";
            break;
         case 4:
            listPtr = &m_bkgndAbsErr;
            wigglePtr = &m_bkgndAbsWiggle;
            listName = "bkgndAbsErr";
            break;
         case 5:
            listPtr = &m_commonPctErr;
            wigglePtr = &m_commonPctWiggle;
            sizePer = 2 * m_subGroups;
            listName = "commonPctErr";
            break;
         case 6:
            listPtr = &m_commonAbsErr;
            wigglePtr = &m_commonAbsWiggle;
            sizePer = 2 * m_subGroups;
            listName = "commonAbsErr";
            break;
         default:
            // should never be here
            assert(0);
            break;
      }
      unsigned int entries = listPtr->size();
      //wigglePtr->clear();
      
      for (unsigned int inner = 0; inner < entries; ++inner)
      {
         DVec &vec = (*listPtr)[inner];
         if (vec.size() != sizePer)
         {
            cerr << listName << " has the wrong number of elements: " 
                 << vec.size() << " != " << sizePer << endl;
            assert(0);
         }
         //wigglePtr->push_back(0.);
      } // for inner
   } // for outer
   // make sure the wiggle vectors are the right size
   //cerr << "subGroups " << m_subGroups << endl;
   for (unsigned int loop = 0; loop < m_subGroups; ++loop)
   {
      m_backgroundPW.push_back (0.);
      m_acceptancePW.push_back (0.);
   } // for loop
   //cerr << "acceptPW size " << m_acceptancePW.size() << endl;
   //cerr << "bkgnd PW size " << m_backgroundPW.size() << endl;
   
   // print everything out
   // printConfiguration();
   if (m_fillTree) 
   {
      if (m_isFitMode)
      {
         // setup tree for fitting mode
         _setupTree();
      } else {
         // setup tree for coverage mode
         _setupCoverageTree();
      }
   }
   if (useFitMethod( kConstraint) )
   {
      //cout << "initializing Gaussian Constraints" << endl;
      initializeGaussConstraints();
   }
   calcBackAndAccept();
}

void
XsecComb::setParameters (const string &tag, DVec &values)
{
   // just to make sure
   assert (values.size() > 0);
   /////////////////////////
   // Check Singles First //
   /////////////////////////
   if ("numTrials" == tag)
   {
      m_numTrials = (int) values[0];
      return;
   }
   if ("numCoverageTrials" == tag)
   {
      m_numCoverageTrials = (int) values[0];
      return;
   }
   if ("coverageMode" == tag)
   {
      m_coverageMode = (int) values[0];
      return;
   }
   if ("totalPb" == tag)
   {
      m_totalpb =  values[0];
      assert (m_totalpb);
      return;
   }
   ///////////////////////
   // Load DVecVecs now //
   ///////////////////////
   // Do common{Pct,Abs}Error first because it's special
   if ("commonPctErr" == tag)
   {
      // is the values long enough
      while (values.size() < 2 * m_subGroups)
      {
         values.push_back(0.);
      }
      m_commonPctErr.push_back( values );
      m_commonPctWiggle.push_back( 0 );
      return;
   }
   if ("commonAbsErr" == tag)
   {
      // is the values long enough
      while (values.size() < 2 * m_subGroups)
      {
         values.push_back(0.);
      }
      m_commonAbsErr.push_back( values );
      m_commonAbsWiggle.push_back( 0 );
      return;
   }
   // make sure values is long enogh
   while (values.size() < m_subGroups)
   {
      values.push_back(0.);
   }
   if ("acceptPctErr" == tag)
   {
      m_acceptPctErr.push_back(values);
      m_acceptPctWiggle.push_back( 0 );
      return;
   } 
   if ("acceptAbsErr" == tag)
   {
      m_acceptAbsErr.push_back(values);
      m_acceptAbsWiggle.push_back( 0 );
      return;
   } 
   if ("backPctErr" == tag)
   {
      m_bkgndPctErr.push_back(values);
      m_bkgndPctWiggle.push_back( 0 );
      return;
   } 
   if ("backAbsErr" == tag)
   {
      m_bkgndAbsErr.push_back(values);
      m_bkgndAbsWiggle.push_back( 0 );
      return;
   }
   if ("baseAccept" == tag)
   {
      m_baseAccept = values;
      return;
   }
   if ("baseAcceptErr" == tag)
   {
      m_baseAcceptErr = values;
      return;
   }
   if ("baseBkgnd" == tag)
   {
      m_baseBkgnd = values;
      return;
   }
   if ("baseBkgndErr" == tag)
   {
      m_baseBkgndErr = values;
      return;
   }
   if ("baseXsec" == tag)
   {
      m_baseXsec = values[0];
      return;
   }
   if ("baseXsecErr" == tag)
   {
      m_baseXsecErr = values[0];
      return;
   }
   if ("bins" == tag)
   {
      double lower, upper;
      int bins = 0;
      if (values.size() < 2)
      {
         return;
      }
      lower = values[0];
      upper = values[1];
      if (values.size() >= 3)
      {
         bins = (int) values[2];
      } 
      if (bins < 100) 
      {
         bins = int (( upper -  lower) * 100);
         if (bins < 100) {
            bins = 100;
         }
      }
      m_numBins = bins;
      m_binLowX = lower;
      m_binHighX = upper;
      return;
   }
   if ("binsZoom" == tag)
   {
      double lower, upper;
      int bins = 0;
      if (values.size() < 2)
      {
         return;
      }
      lower = values[0];
      upper = values[1];
      if (values.size() >= 3)
      {
         bins = (int) values[2];
      } 
      if (bins < 100) 
      {
         bins = int (( upper -  lower) * 100);
         if (bins < 100) {
            bins = 100;
         }
      }
      m_numBinsZoom = bins;
      m_binLowXZoom = lower;
      m_binHighXZoom = upper;
      return;
   }
   // if we're still here, we've got a problem.
   cerr << "WARNING: I don't understand '" << tag
        << "' in file " << m_filename << endl;
}

void
XsecComb::getBinInfo(double &lower, double &higher, int &bins, bool zoom)
{
   if (zoom)
   {
      // zoom
      lower  = m_binLowXZoom;
      higher = m_binHighXZoom;
      bins   = m_numBinsZoom;
   } else {
      // regular
      lower  = m_binLowX;
      higher = m_binHighX;
      bins   = m_numBins;
   }
}

void
XsecComb::_setupCoverageTree()
{
   m_tree = new TTree("coverage", "Cross section coverage");
   if ( useFitMethod(kChisq) )
   {
      m_tree->Branch("trueXsec", &m_trueXsec, "trueXsec/D");
      m_tree->Branch("chisqPull1", &m_chisqPull1, "chisqPull1/D");
      m_tree->Branch("chisqMeas1", &m_chisqStatCenter, "chisqMeas1/D");
      if (kStats_only != m_coverageMode)
      {
         cerr << "Not just stat coverage" << endl;
         m_tree->Branch("chisqPull3", &m_chisqPull3, "chisqPull3/D");
         m_tree->Branch("chisqMeas3", &m_chisqPctCenter, "chisqMeas3/D");
         //m_tree->Branch("chisqPull2", &m_chisqPull2, "chisqPull2/D");
         //m_tree->Branch("chisqPull4", &m_chisqPull4, "chisqPull4/D");
      } else {
         cerr << "Only Stats mode" << endl;
      }
   }
   if ( useFitMethod(kLikeli) )
   {
      m_tree->Branch("likeliPull1", &m_likeliPull1, "likeliPull1/D");
      if (kStats_only != m_coverageMode)
      {
         m_tree->Branch("likeliPull2", &m_likeliPull2, "likeliPull2/D");
         m_tree->Branch("likeliPull3", &m_likeliPull3, "likeliPull3/D");
         m_tree->Branch("likeliPull4", &m_likeliPull4, "likeliPull4/D");
      }
   }
}

void
XsecComb::_setupTree()
{
   m_tree = new TTree("sigma", "Cross section variations");
   m_tree->Branch("back", &m_tBack, "back/D");
   m_tree->Branch("accept", &m_tAccept, "accept/D");
   m_tree->Branch("chisqXsec", &m_tChisqXsec, "chisqXsec/D");
   m_tree->Branch("likeliXsec", &m_tLikeliXsec, "likeliXsec/D");
   char accName[100], backName[100], accNameD[100], backNameD[100];
   for (unsigned int loop = 0; loop < m_subGroups; ++loop)
   {
      sprintf (accName,  "accept%d", loop + 1);
      sprintf (backName, "back%d",   loop + 1);
      sprintf (accNameD,  "accept%d/D", loop + 1);
      sprintf (backNameD, "back%d/D",   loop + 1);
      m_tree->Branch(backName, &(m_backgroundPW[loop]), backNameD);
      m_tree->Branch(accName, &(m_acceptancePW[loop]), accNameD);
   } // for loop
   // put in pull distributions
   for (int outer = 1; outer <= 6; ++outer)
   {
      DVec *wigglePtr = NULL;
      string pullName;
      unsigned int numPulls = 0;
      switch (outer)
      {
         case 1:            
            wigglePtr = &m_acceptPctWiggle;
            pullName = "acceptPctPull";
            numPulls = m_numAcceptPctWiggle;
            break;
         case 2:            
            wigglePtr = &m_acceptAbsWiggle;
            pullName = "acceptAbsPull";
            numPulls = m_numAcceptAbsWiggle;
            break;
         case 3:
            wigglePtr = &m_bkgndPctWiggle;
            pullName = "bkgndPctPull";
            numPulls = m_numBkgndPctWiggle;
            break;
         case 4:
            wigglePtr = &m_bkgndAbsWiggle;
            pullName = "bkgndPull";
            numPulls = m_numBkgndAbsWiggle;
            break;
         case 5:
            wigglePtr = &m_commonPctWiggle;
            pullName = "commonPctPull";
            numPulls = m_numCommonPctWiggle;
            break;
         case 6:
            wigglePtr = &m_commonAbsWiggle;
            pullName = "commonAbsPull";
            numPulls = m_numCommonAbsWiggle;
            break;
         default:
            // should never be here
            assert(0);
            break;
      }
      DVec &vec = *wigglePtr;
      for (unsigned int inner = 0; inner < numPulls; ++inner)
      {
         char longline[100];
         char longDline[100];
         sprintf (longline, "%s%d", pullName.c_str(), inner + 1);
         sprintf (longDline, "%s%d/D", pullName.c_str(), inner + 1);
         m_tree->Branch(longline, &(vec[inner]), longDline);
      } // for inner
   } // for outer
}

void
XsecComb::writeTree(string filename)
{
   if (m_tree)
   {
      cerr << "writing tree to " << filename << "." << endl;
      TFile file (filename.c_str(), "RECREATE", "hist file");
      m_tree->Write();
      file.Close();
   } else {
      cerr << "m_tree " << (void *) m_tree << endl;
      cerr << "tree not setup" << endl;
   }
}
 
void
XsecComb::zeroWiggle()
{
   for (unsigned int loop = 0; loop < m_numAcceptPctWiggle; ++loop)
   {
      m_acceptPctWiggle[loop] = 0;
   } // for loop      
   for (unsigned int loop = 0; loop < m_numAcceptAbsWiggle; ++loop)
   {
      m_acceptAbsWiggle[loop] = 0;
   } // for loop      
   for (unsigned int loop = 0; loop < m_numBkgndPctWiggle; ++loop)
   {
      m_bkgndPctWiggle[loop] = 0;
   } // for loop      
   for (unsigned int loop = 0; loop < m_numBkgndAbsWiggle; ++loop)
   {
      m_bkgndAbsWiggle[loop] = 0;
   } // for loop      
   for (unsigned int loop = 0; loop < m_numCommonPctWiggle; ++loop)
   {
      m_commonPctWiggle[loop] = 0;
   } // for loop
   for (unsigned int loop = 0; loop < m_numCommonAbsWiggle; ++loop)
   {
      m_commonAbsWiggle[loop] = 0;
   } // for loop
   // If we're zeroing the wiggles, make sure they don't get
   // wiggled again
   m_mode = kSingleFit;
   //cerr << "Wiggles zeroed" << endl;
   calcBackAndAccept();
}

void
XsecComb::wiggle()
{
   if (kSingleFit == m_mode)
   {
      // don't wiggle anything
      return;
   }
   // wiggle common errors
   for (unsigned int loop = 0; loop < m_numCommonPctWiggle; ++loop)
   {
      m_commonPctWiggle[loop] = randomGauss();
   } // for loop
   for (unsigned int loop = 0; loop < m_numCommonAbsWiggle; ++loop)
   {
      m_commonAbsWiggle[loop] = randomGauss();
   } // for loop
   if (kBkgnd_mode != m_mode)
   {
      // wiggle acceptance
      // wiggle accept errors
      for (unsigned int loop = 0; loop < m_numAcceptPctWiggle; ++loop)
      {
         m_acceptPctWiggle[loop] = randomGauss();
      } // for loop      
      for (unsigned int loop = 0; loop < m_numAcceptAbsWiggle; ++loop)
      {
         m_acceptAbsWiggle[loop] = randomGauss();
      } // for loop      
   } 
   if (kAccept_mode != m_mode)
   {
      // wiggle background
      // wiggle bkgnd errors
      for (unsigned int loop = 0; loop < m_numBkgndAbsWiggle; ++loop)
      {
         m_bkgndAbsWiggle[loop] = randomGauss();
      } // for loop      
      for (unsigned int loop = 0; loop < m_numBkgndPctWiggle; ++loop)
      {
         m_bkgndPctWiggle[loop] = randomGauss();
      } // for loop      
   }
   calcBackAndAccept();
}

void
XsecComb::zeroBackOrAccptFromCommon()
{
   // Percent error first
   for (unsigned int outer = 0; outer < m_commonPctErr.size(); ++outer)
   {
      for (unsigned int inner = 0; inner < m_subGroups; ++inner)
      {        
         if ((0 == inner % 2) && (kBkgnd_mode == m_mode))
         {
            // we have an even inner and we are in background mode
            // clear out the acceptance error
            m_commonPctErr[outer][inner] = 0.;
         }
         if ((1 == inner % 2) && (kAccept_mode == m_mode))
         {
            // we have an odd inner and we are in accept mode
            // clear out the background error
            m_commonPctErr[outer][inner] = 0.;
         }
      } // for inner
   } // for outer
   // Now absolute error
   for (unsigned int outer = 0; outer < m_commonAbsErr.size(); ++outer)
   {
      for (unsigned int inner = 0; inner < m_subGroups; ++inner)
      {        
         if ((0 == inner % 2) && (kBkgnd_mode == m_mode))
         {
            // we have an even inner and we are in background mode
            // clear out the acceptance error
            m_commonAbsErr[outer][inner] = 0.;
         }
         if ((1 == inner % 2) && (kAccept_mode == m_mode))
         {
            // we have an odd inner and we are in accept mode
            // clear out the background error
            m_commonAbsErr[outer][inner] = 0.;
         }
      } // for inner
   } // for outer
}

void
XsecComb::calcBackAndAccept()
{

   // calculate acceptance and background post wiggle
   for (unsigned int subgroup = 0; subgroup < m_subGroups;  ++subgroup)
   {
      double deltaBkgnd = 0;
      double deltaAccept = 0;
      ////////////////////////
      // Background Wiggles //
      ////////////////////////
      for (unsigned int loop = 0; loop < m_numBkgndPctWiggle; ++loop)
      {
         deltaBkgnd += m_bkgndPctErr[loop][subgroup] 
            * m_bkgndPctWiggle[loop] * m_bkgnd[subgroup];           
         //cout << "m_bkgndPctWiggle " << m_bkgndPctWiggle[loop] << endl;
      } // for loop
      for (unsigned int loop = 0; loop < m_numBkgndAbsWiggle; ++loop)
      {
         deltaBkgnd += m_bkgndAbsErr[loop][subgroup] 
            * m_bkgndAbsWiggle[loop];
      } // for loop
      ////////////////////////
      // Acceptance Wiggles //
      ////////////////////////
      for (unsigned int loop = 0; loop < m_numAcceptPctWiggle; ++loop)
      {
         deltaAccept += m_accept[subgroup] 
            * m_acceptPctWiggle[loop] * m_acceptPctErr[loop][subgroup];
      }
      for (unsigned int loop = 0; loop < m_numAcceptAbsWiggle; ++loop)
      {
         deltaAccept += m_acceptAbsErr[loop][subgroup] 
            * m_acceptAbsWiggle[loop];
      } // for loop
      ////////////////////
      // Common Wiggles //
      ////////////////////
      unsigned int acceptGroup = 2 * subgroup;
      unsigned int  bkgndGroup = 2 * subgroup + 1;
      // First Percent
      for (unsigned int loop = 0; loop < m_numCommonPctWiggle; ++loop)
      {
         // Acceptance
         deltaAccept += m_accept[subgroup] 
            * m_commonPctWiggle[loop] * m_commonPctErr[loop][acceptGroup];
         // Background
         deltaBkgnd += m_bkgnd[subgroup] 
            * m_commonPctWiggle[loop] * m_commonPctErr[loop][bkgndGroup];
      }
      // Now Absolute
      for (unsigned int loop = 0; loop < m_numCommonAbsWiggle; ++loop)
      {
         // Acceptance
         deltaAccept += m_commonAbsWiggle[loop] 
            * m_commonAbsErr[loop][acceptGroup];
         // Background
         deltaBkgnd += m_commonAbsWiggle[loop] 
            * m_commonAbsErr[loop][bkgndGroup];
      }
      //////////////////////////////////
      // Put everything back together //
      //////////////////////////////////
      m_backgroundPW[subgroup] = m_bkgnd[subgroup] + deltaBkgnd;
      if (m_backgroundPW[subgroup] < 0)
      {
         m_backgroundPW[subgroup] = 0.;
      }
      m_acceptancePW[subgroup] = m_accept[subgroup] + deltaAccept;
      if (m_acceptancePW[subgroup] <= 0)
      {
         m_acceptancePW[subgroup] = 0.00000001;
      }
   } // for subgroup
}

double 
XsecComb::getBackground (unsigned int subgroup) const
{
   return m_bkgnd[subgroup];
}

double 
XsecComb::getAcceptance (unsigned int subgroup) const
{
   return m_accept[subgroup];
}

double 
XsecComb::background (unsigned int subgroup) const
{
   return m_backgroundPW[subgroup];
}

double 
XsecComb::acceptance (unsigned int subgroup) const
{
   return m_acceptancePW[subgroup];
}

double 
XsecComb::background (unsigned int subgroup, double *constraints) const
{
   double deltaBkgnd = 0;
   for (unsigned int loop = 0; loop < m_numBkgndPctWiggle; ++loop)
   {
      deltaBkgnd += m_bkgndPctErr[loop][subgroup] 
         * constraints[ m_bkgndPctCMap[loop] ] 
         * m_bkgnd[subgroup];           
   } // for loop
   for (unsigned int loop = 0; loop < m_numBkgndAbsWiggle; ++loop)
   {
      deltaBkgnd += m_bkgndAbsErr[loop][subgroup] 
         * constraints[ m_bkgndAbsCMap[loop] ];
   } // for loop
   ////////////////////
   // Common Wiggles //
   ////////////////////
   unsigned int  bkgndGroup = 2 * subgroup + 1;
   // First Percent
   for (unsigned int loop = 0; loop < m_numCommonPctWiggle; ++loop)
   {
      deltaBkgnd += m_bkgnd[subgroup] 
         * constraints[ m_commonPctCMap[loop] ] 
         * m_commonPctErr[loop][bkgndGroup];
   }
   // Now Absolute
   for (unsigned int loop = 0; loop < m_numCommonAbsWiggle; ++loop)
   {
      deltaBkgnd += m_commonAbsWiggle[loop] 
         *  constraints[ m_commonAbsCMap[loop] ];
   }
   double bkgnd = m_bkgnd[subgroup] + deltaBkgnd;
   if (bkgnd < 0)
   {
      return 0;
   } else {
      return bkgnd;
   }
}

double 
XsecComb::acceptance (unsigned int subgroup, double *constraints) const
{
   double deltaAccept = 0;
   for (unsigned int loop = 0; loop < m_numAcceptPctWiggle; ++loop)
   {
      deltaAccept += m_acceptPctErr[loop][subgroup] 
         * constraints[ m_acceptPctCMap[loop] ] 
         * m_accept[subgroup];           
   } // for loop
   for (unsigned int loop = 0; loop < m_numAcceptAbsWiggle; ++loop)
   {
      deltaAccept += m_acceptAbsErr[loop][subgroup] 
         * constraints[ m_acceptAbsCMap[loop] ];
   } // for loop
   ////////////////////
   // Common Wiggles //
   ////////////////////
   unsigned int  acceptGroup = 2 * subgroup;
   // First Percent
   for (unsigned int loop = 0; loop < m_numCommonPctWiggle; ++loop)
   {
      deltaAccept += m_accept[subgroup] 
         * constraints[ m_commonPctCMap[loop] ] 
         * m_commonPctErr[loop][acceptGroup];
   }
   // Now Absolute
   for (unsigned int loop = 0; loop < m_numCommonAbsWiggle; ++loop)
   {
      deltaAccept += m_commonAbsWiggle[loop] 
         *  constraints[ m_commonAbsCMap[loop] ];
   }
   double accept = m_accept[subgroup] + deltaAccept;
   if (accept < 0.00001)
   {
      return 0.00001;
   } else {
      return accept;
   }
}

double 
XsecComb::observed (unsigned int subgroup) const
{
   return m_observed[subgroup];
}

void
XsecComb::calculate (bool doMinos)
{
   // we want to fit *this* copy of XsecComb
   m_ptr = this;
   // wiggle everything first
   wiggle();
   // arglist are possible arguments you pass into minuit functions
   Double_t arglist[10]; 
   // whether there was an error or not
   Int_t ierflg = 0;
   // just because
   Double_t err;
   // migrad minimize for chisq
   arglist[0] = 1;
   arglist[1] = smooshedXsec();
   m_chisqMinuit->mnexcm("SET PAR", arglist, 2, ierflg);
   m_likeliMinuit->mnexcm("SET PAR", arglist, 2, ierflg);
   arglist[0] = arglist[1] = 0.;
   if (useFitMethod(kChisq))
   {
      m_chisqMinuit->mnexcm("MIG", arglist, 0, ierflg);
      m_chisqMinuit->GetParameter(0, m_chisqValue, err);
      if (doMinos)
      {
         Double_t parab, gcc;
         // set error to one
         arglist[0] = 1.;
         m_chisqMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
         // Now lets try Minos for assymetric errors
         m_chisqMinuit->mnexcm("MINO", arglist, 0, ierflg);
         m_chisqMinuit->mnerrs(0, m_chisqPlus, m_chisqMinus, parab, gcc);
      }
   }
   
   // migrad minimize for likeli
   if (useFitMethod(kLikeli))
   {
      m_likeliMinuit->mnexcm("MIG", arglist, 0, ierflg);
      m_likeliMinuit->GetParameter(0, m_likeliValue, err);
      if (doMinos)
      {
         Double_t parab, gcc;
         // set error to one
         arglist[0] = 1.;
         m_likeliMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
         // Now lets try Minos for assymetric errors
         m_likeliMinuit->mnexcm("MINO", arglist, 0, ierflg);
         m_likeliMinuit->mnerrs(0, m_likeliPlus, m_likeliMinus, parab, gcc);
      }
   }
   if (useFitMethod(kConstraint) && doMinos)
   {
      arglist[0] = 1;
      arglist[1] = smooshedXsec();
      m_gaussConstMinuit->mnexcm("SET PAR", arglist, 2, ierflg);
      //m_gaussConstMinuit->mnexcm("SIM", arglist, 0, ierflg);
      m_gaussConstMinuit->mnexcm("MIG", arglist, 0, ierflg);
      m_gaussConstMinuit->GetParameter(0, m_cnstrntValue, err);
      Double_t parab, gcc;
      // set error to one
      arglist[0] = 1.;
      m_gaussConstMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
      // Now lets try Minos for assymetric errors
      m_gaussConstMinuit->mnexcm("MINO", arglist, 0, ierflg);
      m_gaussConstMinuit->mnerrs(0, m_cnstrntPlus, m_cnstrntMinus, parab, gcc);
   }
   // Fill tree
   if (m_tree && ! doMinos)
   {
      m_tChisqXsec = m_chisqValue;
      m_tLikeliXsec = m_likeliValue;
      m_tBack = 0;
      m_tAccept = 0;
      for (unsigned int loop = 0; loop < m_subGroups; ++loop)
      {
         m_tBack   += background(loop);
         m_tAccept += acceptance(loop);
      } // for loop
      m_tree->Fill();
   } // if m_tree
}

bool
XsecComb::useFitMethod (int method)
{
   return ! m_fitFlags.test(method);
}

void
XsecComb::calcStatAndSyst(int mode)
{
   CLPTuple chisqTuple, likeliTuple;
   setMode (kSingleFit);
   calculate (true); // with errors
   m_chisqStatCenter = m_chisqValue;
   m_chisqStatUpper = m_chisqValue + m_chisqPlus;
   m_chisqStatLower = m_chisqValue + m_chisqMinus;
   m_likeliStatCenter = m_likeliValue;
   m_likeliStatUpper = m_likeliValue + m_likeliPlus;
   m_likeliStatLower = m_likeliValue + m_likeliMinus;
   if (kSingleFit == mode)
   {
      // don't need to do much more
      return;
   }
   setMode (mode);
   if (m_fittingVerbose)
   {
      cerr << "Trial  0000";
   }
   //cerr << endl << *this << endl;
   int numTrials = m_numTrials;
   if (! useFitMethod (kChisq) && ! useFitMethod (kLikeli) )
   {
      numTrials = 0;
   }
   for (int loop = 1; loop <= numTrials; ++loop)
   {
      if (m_fittingVerbose)
      {
         if (0 == (loop % 50000))
         {
            cerr << endl << "Trial " << loop;
         } else if (0 == (loop % 1000))
         {
            cerr << ".";
            cerr.flush();
         }
      }
      calculate(false); // without errors
      chisqTuple.fill(m_chisqValue);
      likeliTuple.fill(m_likeliValue);
   } // for loop
   if (m_fittingVerbose)
   {
      cerr << endl;
   }
   if (useFitMethod(kChisq))
   {
      chisqTuple.hist().findMaxAnd68(m_chisqMaxCenter, 
                                     m_chisqMaxLower,
                                     m_chisqMaxUpper,
                                     m_chisqPctLower,
                                     m_chisqPctCenter, 
                                     m_chisqPctUpper);
      //dout << endl
      //     << "m_chisqMaxCenter " << m_chisqMaxCenter   
      //     << "m_chisqMaxLower  " << m_chisqMaxLower 
      //     << "m_chisqMaxUpper  " << m_chisqMaxUpper 
      //     << "m_chisqPctLower  " << m_chisqPctLower 
      //     << "m_chisqPctCenter " << m_chisqPctCenter
      //     << "m_chisqPctUpper  " << m_chisqPctUpper 
      //     << endl;
   } else {
      //dout << "waaaaa" << endl;
   }
   if (useFitMethod(kLikeli))
   {
      likeliTuple.hist().findMaxAnd68(m_likeliMaxCenter, 
                                      m_likeliMaxLower,
                                      m_likeliMaxUpper,
                                      m_likeliPctLower,
                                      m_likeliPctCenter, 
                                      m_likeliPctUpper);
   }
}

void 
XsecComb::copyResults (XsecComb &other)
{
   // chisq results
   m_chisqStatCenter = other.m_chisqStatCenter;
   m_chisqStatUpper  = other.m_chisqStatUpper; 
   m_chisqStatLower  = other.m_chisqStatLower; 
   m_chisqMaxCenter  = other.m_chisqMaxCenter;
   m_chisqMaxUpper   = other.m_chisqMaxUpper; 
   m_chisqMaxLower   = other.m_chisqMaxLower; 
   m_chisqPctCenter  = other.m_chisqPctCenter;
   m_chisqPctUpper   = other.m_chisqPctUpper; 
   m_chisqPctLower   = other.m_chisqPctLower; 
   // likeli results
   m_likeliStatCenter = other.m_likeliStatCenter;
   m_likeliStatUpper  = other.m_likeliStatUpper; 
   m_likeliStatLower  = other.m_likeliStatLower; 
   m_likeliMaxCenter  = other.m_likeliMaxCenter;
   m_likeliMaxUpper   = other.m_likeliMaxUpper; 
   m_likeliMaxLower   = other.m_likeliMaxLower; 
   m_likeliPctCenter  = other.m_likeliPctCenter;
   m_likeliPctUpper   = other.m_likeliPctUpper; 
   m_likeliPctLower   = other.m_likeliPctLower; 
}

void
XsecComb::checkCoverage(int coverageMode)
{
   // don't let CLPHisto do the minimum interval finding
   CLPHisto::setNoMinInterval(true);
   m_coverageMode = coverageMode;
   m_fittingVerbose = false;
   cerr << "coverage mode " << m_coverageMode << endl;
   // setup this entity to be copied
   clearFit();
   // make sure we've got enough to fit
   unsigned int numGroups = m_baseAcceptErr.size();
   assert ( m_baseAcceptErr.size() );
   assert ( m_baseAcceptErr.size() == m_baseBkgndErr.size() );
   assert ( m_baseAcceptErr.size() == m_baseBkgnd.size() );
   assert ( m_baseAcceptErr.size() == m_baseAccept.size() );
   // copy the acceptance errors and background errors to where
   // they can be used for the fit
   setSubGroups (numGroups);
   setParameters ("acceptAbsErr", m_baseAcceptErr);
   setParameters ("backAbsErr", m_baseBkgndErr);
   // Let's start loopin'
   int mode;
   if (kStats_only == m_coverageMode)
   {
      mode = kSingleFit;
   } else {
      mode = kBoth_mode;
   }
   int chisqCovered1 = 0;
   int chisqCovered2 = 0;
   int chisqCovered3 = 0;
   int chisqCovered4 = 0;
   int likeliCovered1 = 0;
   //int likeliCovered2 = 0;
   //int likeliCovered3 = 0;
   //int likeliCovered4 = 0;
   if (kStats_only == m_coverageMode) 
   {
      cerr << "Stats only mode" << endl;
   } else if (kSyst_only == m_coverageMode)
   {
      cerr << "Syst only mode" << endl;
   } else if (kStats_and_Syst == m_coverageMode)
   {
      cerr << "Stats and syst mode" << endl;
   }
   cerr << "Trial  0000";
   int oneK = 1000 / m_numTrials;
   if (oneK == 0)
   {
      oneK = 1;
   }
   int fiftyK = 50 * oneK;
   for (int loop = 1; loop <= m_numCoverageTrials; ++loop)
   {
      if (0 == (loop % fiftyK) )
      {
         cerr << endl << "Trial " << loop;
      } else if (0 == ( loop % oneK ))
      {
         cerr << ".";
         cerr.flush();
      }
      XsecComb comb = *this;
      comb.m_isFitMode = true;
      comb.m_fillTree = false; // do not let this copy fill its own tree
      comb.m_tree = 0;
      comb.setMode(mode);
      // throw the "real" cross section
      m_trueXsec = randomGauss(m_baseXsec, m_baseXsecErr);
      if (m_trueXsec < 0.001)
      {
         m_trueXsec = 0.001;
      }
      double realBkgndPull = randomGauss();
      double realAcceptPull = randomGauss();
      double measBkgndPull;
      double measAcceptPull;
      // if we are measuring stats coverage only, then the
      // measured background and acceptance are the same as
      // the real background and acceptance (i.e. measPulls = 0). 
      // Otherwise, we have to use the measPulls
      if (kStats_only != m_coverageMode)
      {
         // We are checking the systematic coverage
         measBkgndPull = randomGauss();
         measAcceptPull = randomGauss();
      } else {
         // we are only checking statistical coverage
         measBkgndPull = 0;
         measAcceptPull = 0;
      }
      DVec realAcceptVec, realBkgndVec;
      DVec measAcceptVec, measBkgndVec;
      DVec expectedSignal, expectedBkgnd;
      DVec seenSignal, seenBkgnd;
      DVec observedVec;
      for (unsigned int group = 0; group < numGroups; ++group)
      {
         double realAccept = m_baseAccept[group] 
            + m_baseAcceptErr[group] * realAcceptPull;
         double realBkgnd = m_baseBkgnd[group] 
            + m_baseBkgndErr[group] * realBkgndPull;
         double measAccept;
         double measBkgnd;
         measAccept = realAccept + m_baseAcceptErr[group] * measAcceptPull;
         measBkgnd = realBkgnd + m_baseBkgndErr[group] * measBkgndPull;
         double expectedSignal = m_trueXsec * realAccept * m_totalpb;
         double seenBkgnd, seenSignal;
         if (kSyst_only == m_coverageMode)
         {
            // we are checking systematic errors only
            // so we do NOT use Poisson random numbers here
            seenSignal = expectedSignal;
            seenBkgnd  = realBkgnd;
         } else {
            // In most cases, we want to take the expected numbers
            // of events and use the Poisson random number generator
            seenSignal = randomPoisson( expectedSignal );
            seenBkgnd  = randomPoisson( realBkgnd );
         }
         //cout << "observed " << seenSignal << " + " << seenBkgnd << endl
         //     << "acceptance " << measAccept << " " << realAccept 
         //     << " " << m_baseAccept[group] << endl
         //     << "background " << measBkgnd << endl;
         observedVec.push_back( seenSignal + seenBkgnd );
         measAcceptVec.push_back( measAccept );
         measBkgndVec.push_back( measBkgnd );
      } // for group
      comb.setObserved  ( observedVec );
      comb.setAcceptance( measAcceptVec );
      comb.setBackground( measBkgndVec );
      comb.setThis();
      comb.setupEverything();
      comb.calcStatAndSyst(mode);
      //cerr << "comb " << comb << endl;
      copyResults(comb);
      if (kStats_only == m_coverageMode) 
      {
         if (useFitMethod (kChisq))
         {
            m_chisqPull1 = calcZscore (m_trueXsec, m_chisqStatCenter,
                                       m_chisqStatUpper, m_chisqStatLower,
                                       chisqCovered1);
         } // if chisq
         if (useFitMethod (kLikeli))
         {
            m_likeliPull1 = calcZscore (m_trueXsec, m_likeliStatCenter,
                                        m_likeliStatUpper, m_likeliStatLower,
                                        likeliCovered1);
         } // if likeli
      } // if Stats only
      else if (kSyst_only == m_coverageMode)
      {
         if (useFitMethod (kChisq))
         {
            m_chisqPull1 = calcZscore (m_trueXsec, m_chisqStatCenter,
                                       m_chisqPctUpper, m_chisqPctLower,
                                       chisqCovered1);
            //m_chisqPull2 = calcZscore (m_trueXsec, m_chisqMaxCenter,
            //                           m_chisqMaxUpper, m_chisqMaxLower,
            //                           chisqCovered2);
            m_chisqPull3 = calcZscore (m_trueXsec, m_chisqPctCenter,
                                       m_chisqPctUpper, m_chisqPctLower,
                                       chisqCovered3);
            //m_chisqPull4 = calcZscore (m_trueXsec, m_chisqStatCenter,
            //                           m_chisqMaxUpper, m_chisqMaxLower,
            //                           chisqCovered4);
         } // if chisq
      }
      if (m_tree)
      {
         m_tree->Fill();
      } // if m_tree
   } // for loop
   cerr << endl;
   if (useFitMethod (kChisq))
   {
      cerr << "chisq coverage 1 " 
           << (double) chisqCovered1 / m_numCoverageTrials
           << endl;
      if (kStats_only != m_coverageMode)
      {
         cerr << "chisq coverage 2 " 
                 << (double) chisqCovered2 / m_numCoverageTrials
              << endl;
         cerr << "chisq coverage 3 " 
              << (double) chisqCovered3 / m_numCoverageTrials
              << endl;
         cerr << "chisq coverage 4 " 
              << (double) chisqCovered4 / m_numCoverageTrials
              << endl;
      }
      if (useFitMethod (kLikeli))
      {
         double likeliCoverage1 = (double) likeliCovered1 
            / m_numCoverageTrials;
         cerr << "likeli coverage 1 " << likeliCoverage1 << endl;
      }
   }
}

double
XsecComb::calcZscore (double trueVal, double center, double upper, 
                      double lower, int &coveredCount)
{
   double diff = center - trueVal;
   double denom;
   if (diff < 0)
   {
      denom = upper - center;    
   } else {
      denom = center - lower;
   }
   if (0 == denom)
   {
      return -999.;
   }
   double zScore = diff / denom;
   if ((zScore >= -1) && (zScore <= 1))
   {
      ++coveredCount;
   }
   return zScore;
}

/////////////////////////////
// Static Member Functions //
/////////////////////////////
void
XsecComb::initializeClass()
{
   double arglist[10];
   arglist[0] = 1.;
   Int_t ierflg = 0;
   ///////////
   // Chisq //
   ///////////
   m_chisqMinuit = new TMinuit(1);
   // tell minuit to be quiet
   arglist[0] = -1;
   m_chisqMinuit->mnexcm("SET PRINT",arglist,1,ierflg);
   m_chisqMinuit->SetFCN(chisq_fcn);
   arglist[0] = 1.;
   m_chisqMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
   m_chisqMinuit->mnparm(0, "xsec", 7.5, 0.0001, -0.001, 1000., ierflg);
   ////////////////
   // Likelihood //
   ////////////////
   m_likeliMinuit = new TMinuit(1);
   // tell minuit to be quiet
   arglist[0] = -1;
   m_likeliMinuit->mnexcm("SET PRINT",arglist,1,ierflg);
   m_likeliMinuit->SetFCN(likeli_fcn);
   arglist[0] = 1.0;
   m_likeliMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
   m_likeliMinuit->mnparm(0, "xsec", 7.5, 0.0001, -0.001, 1000., ierflg);
   m_likeliMinuit->fLwarn = false;
}

void
XsecComb::initializeGaussConstraints ()
{
   unsigned int numConstraints = m_numConstraints
      = m_numAcceptPctWiggle
      + m_numAcceptAbsWiggle
      + m_numBkgndPctWiggle 
      + m_numBkgndAbsWiggle 
      + m_numCommonPctWiggle
      + m_numCommonAbsWiggle;
   //cout << "m_numConstraints " << m_numConstraints << endl;
   double arglist[10];
   arglist[0] = 1.;
   Int_t ierflg = 0;
   // check to make sure this has not been assigned already
   assert (0 == m_gaussConstMinuit);
   m_gaussConstMinuit = new TMinuit(numConstraints + 1);
   // tell minuit to be quiet
   arglist[0] = -1;
   m_gaussConstMinuit->mnexcm("SET PRINT",arglist,1,ierflg);
   m_gaussConstMinuit->fLwarn = false;
   m_gaussConstMinuit->SetFCN(constraint_fcn);
   arglist[0] = 1.0;
   m_gaussConstMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
   m_gaussConstMinuit->mnparm(0, "xsec", 7.5, 0.0001, -0.001, 1000., ierflg);
   unsigned int constraint = 0;
   for (unsigned int loop = 1; loop <= m_numAcceptPctWiggle; ++loop)
   {
      char line[100];
      m_acceptPctCMap.push_back(++constraint);
      sprintf (line, "c_ap%02d", constraint);
      m_gaussConstMinuit->mnparm(constraint, line, 
                                 0, 0.0001, -100., 100., ierflg);
   } // for loop (acceptPct)
   for (unsigned int loop = 1; loop <= m_numAcceptAbsWiggle; ++loop)
   {
      char line[100];
      m_acceptAbsCMap.push_back(++constraint);
      sprintf (line, "c_aa%02d", constraint);
      m_gaussConstMinuit->mnparm(constraint, line, 
                                 0, 0.0001, -100., 100., ierflg);
   } // for loop (acceptAbs)
   for (unsigned int loop = 1; loop <= m_numBkgndPctWiggle; ++loop)
   {
      char line[100];
      m_bkgndPctCMap.push_back(++constraint);
      sprintf (line, "c_bp%02d", constraint);
      m_gaussConstMinuit->mnparm(constraint, line, 
                                 0, 0.0001, -100., 100., ierflg);
   } // for loop (bkgndPct)
   for (unsigned int loop = 1; loop <= m_numBkgndAbsWiggle; ++loop)
   {
      char line[100];
      m_bkgndAbsCMap.push_back(++constraint);
      sprintf (line, "c_ba%02d", constraint);
      m_gaussConstMinuit->mnparm(constraint, line, 
                                 0, 0.0001, -100., 100., ierflg);
   } // for loop (bkgndAbs)
   for (unsigned int loop = 1; loop <= m_numCommonPctWiggle; ++loop)
   {
      char line[100];
      m_commonPctCMap.push_back(++constraint);
      sprintf (line, "c_cp%02d", constraint);
      m_gaussConstMinuit->mnparm(constraint, line, 
                                 0, 0.0001, -100., 100., ierflg);
   } // for loop (commonPct)
   for (unsigned int loop = 1; loop <= m_numCommonAbsWiggle; ++loop)
   {
      char line[100];
      m_commonAbsCMap.push_back(++constraint);
      sprintf (line, "c_ca%02d", constraint);
      m_gaussConstMinuit->mnparm(constraint, line, 
                                 0, 0.0001, -100., 100., ierflg);
   } // for loop (commonAbs)
}

double
XsecComb::randomGauss (double mean, double sigma)
{
   return gRandom->Gaus (mean, sigma) * m_wiggleError;
}

int
XsecComb::randomPoisson (double mean)
{
   return gRandom->Poisson(mean);
}

double
XsecComb::randomFlat (double lower, double upper)
{
   return gRandom->Uniform (lower, upper);
}

// friends

void likeli_fcn(Int_t &npar, Double_t *gin, Double_t &loglikeli, 
                Double_t *par, Int_t iflag)
{
   //calculate chisquare
   Double_t sigma = par[0];
   loglikeli = XsecComb::m_ptr->logLikeli(sigma);
}

void chisq_fcn(Int_t &npar, Double_t *gin, Double_t &chisq, 
               Double_t *par, Int_t iflag)
{
   Double_t sigma = par[0];
   chisq = XsecComb::m_ptr->chisq(sigma);
}

void constraint_fcn(Int_t &npar, Double_t *gin, Double_t &chisq, 
               Double_t *par, Int_t iflag)
{
   Double_t sigma = par[0];
   chisq = XsecComb::m_ptr->constrained(sigma, par);
}

ostream& operator<< (ostream& o_stream, const XsecComb &rhs)
{
   // print out everything
   o_stream << "SubGroups: " << rhs.m_subGroups << endl;
   o_stream << endl 
            << " Accpt  Evnts  Bkgnd" << endl
            << " -----  -----  -----" << endl;
   for (unsigned int loop = 0; loop < rhs.m_subGroups; ++loop)
   {
      o_stream << rhs.acceptance(loop)
               << "   " << setw(2) <<  rhs.observed(loop)
               << "    " << rhs.background(loop) << endl;
   } // for loop
   o_stream << "acceptPctErr size " << rhs.m_acceptPctErr.size() << endl
            << "acceptAbsErr size " << rhs.m_acceptAbsErr.size() << endl
            << "bkgndPctErr size  " << rhs.m_bkgndPctErr.size()  << endl
            << "bkgndAbsErr size  " << rhs.m_bkgndAbsErr.size()  << endl
            << "commonPctErr size " << rhs.m_commonPctErr.size() << endl
            << "commonAbsErr size " << rhs.m_commonAbsErr.size() << endl;
   if (rhs.m_acceptPctErr.size())
   {
      o_stream << "acceptPctErr" << endl;
      for (unsigned loop = 0; loop < rhs.m_acceptPctErr.size(); ++loop)
      {
         o_stream << loop << ") " << rhs.m_acceptPctErr[loop] << endl;
      }
   }
   if (rhs.m_acceptAbsErr.size())
   {
      o_stream << "acceptAbsErr" << endl;
      for (unsigned loop = 0; loop < rhs.m_acceptAbsErr.size(); ++loop)
      {
         o_stream << loop << ") " << rhs.m_acceptAbsErr[loop] << endl;
      }
   }
   if (rhs.m_bkgndPctErr.size())
   {
      o_stream << "bkgndPctErr" << endl;
      for (unsigned loop = 0; loop < rhs.m_bkgndPctErr.size(); ++loop)
      {
         o_stream << loop << ") " << rhs.m_bkgndPctErr[loop] << endl;
      }
   }
   if (rhs.m_bkgndAbsErr.size())
   {
      o_stream << "bkgndAbsErr" << endl;
      for (unsigned loop = 0; loop < rhs.m_bkgndAbsErr.size(); ++loop)
      {
         o_stream << loop << ") " << rhs.m_bkgndAbsErr[loop] << endl;
      }
   }
   if (rhs.m_commonPctErr.size())
   {
      o_stream << "commonPctErr" << endl;
      for (unsigned loop = 0; loop < rhs.m_commonPctErr.size(); ++loop)
      {
         o_stream << loop << ") " << rhs.m_commonPctErr[loop] << endl;
      }
   }
   if (rhs.m_commonAbsErr.size())
   {
      o_stream << "commonAbsErr" << endl;
      for (unsigned loop = 0; loop < rhs.m_commonAbsErr.size(); ++loop)
      {
         o_stream << loop << ") " << rhs.m_commonAbsErr[loop] << endl;
      }
   }
   o_stream << endl << endl;
   return o_stream;
} 

// other functions
