// -*- C++ -*-

// C++ standard include files
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>

// Root include files
#include "TFile.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "TFeldmanCousins.h"
#include "TRandom3.h"

// User include files
#include "RootFileCollection/OptionUtils.h"
#include "FCBinClass/FCBinClass.hh"
#include "FCBinClass/dout.hh"

using namespace std;
using namespace OptionUtils;

/////////////////////////
// Static Data Members //
/////////////////////////

// constants
const double FCBinClass::kMinAcceptance = 0.00001;

// static member data
int    FCBinClass::sm_numDim             = 1;
int    FCBinClass::sm_numPEs             = 100000;
int    FCBinClass::sm_numGrossPoints     = 20;
int    FCBinClass::sm_debug              = 0;
int    FCBinClass::sm_modeInt            = 0;
bool   FCBinClass::sm_fastMode           = true;
bool   FCBinClass::sm_autoUpdate         = false;
bool   FCBinClass::sm_showVariables      = false;
bool   FCBinClass::sm_davidCorr          = false;
bool   FCBinClass::sm_fromHighToLow      = false;
bool   FCBinClass::sm_makePlots          = false;
double FCBinClass::sm_trueValue          = 0.;
double FCBinClass::sm_stepSize           = 0.001;
double FCBinClass::sm_minProbSum         = 1 - 0.0001;
double FCBinClass::sm_expectedDelta      = 0.0001;
double FCBinClass::sm_confidenceLevel    = 0.95;
double FCBinClass::sm_resizeFactor       = 0.20;
double FCBinClass::sm_bayesUpperIntegral = 1.;
double FCBinClass::sm_minFactorChange    = 0.001;
double FCBinClass::sm_muDenom1           = 0;
double FCBinClass::sm_muDenom2           = 0;
double FCBinClass::sm_minDenomFactor     = 0.01;
double FCBinClass::sm_maxDenomFactor     = 1.00;
int    FCBinClass::sm_numShiftUsed       = 0;
int    FCBinClass::sm_maxNumPETries      = 10;
int    FCBinClass::sm_maxSize            = 300;
string FCBinClass::sm_optionsFile;
string FCBinClass::sm_rootFile;
// kMaxDim variables
int    FCBinClass::sm_indicies       [kMaxDim] = {};
int    FCBinClass::sm_max            [kMaxDim] = {};
int    FCBinClass::sm_numObsv        [kMaxDim] = {};
double FCBinClass::sm_accLum         [kMaxDim] = {};
double FCBinClass::sm_back           [kMaxDim] = {};
double FCBinClass::sm_accLumMu       [kMaxDim] = {};
double FCBinClass::sm_backMu         [kMaxDim] = {};
int    FCBinClass::sm_numObsvPE      [kMaxDim] = {};
double FCBinClass::sm_accLumErr      [kMaxDim][kMaxUserVariables] = {{}};
double FCBinClass::sm_accLumMuErr    [kMaxDim][kMaxUserVariables] = {{}};
double FCBinClass::sm_backErr        [kMaxDim][kMaxUserVariables] = {{}};
double FCBinClass::sm_backMuErr      [kMaxDim][kMaxUserVariables] = {{}};
string FCBinClass::sm_accLumHistname [kMaxDim] = {};
string FCBinClass::sm_backHistname   [kMaxDim] = {};
double FCBinClass::sm_accLumHistNorm [kMaxDim];
double FCBinClass::sm_backHistNorm   [kMaxDim];
double FCBinClass::sm_accLumFactor   [kMaxDim];
double FCBinClass::sm_backFactor     [kMaxDim];
double FCBinClass::sm_ELbackFactor   [kMaxDim];
// User variables
double FCBinClass::sm_shift       [kMaxUserVariables] = {};
bool   FCBinClass::sm_userBools   [kMaxUserVariables] = {};
int    FCBinClass::sm_userInts    [kMaxUserVariables] = {};
double FCBinClass::sm_userDoubles [kMaxUserVariables] = {};
string FCBinClass::sm_userStrings [kMaxUserVariables] = {};
// Class-defined types
FCBinClass::LimitMode FCBinClass::sm_mode = FCBinClass::kFeldmanCousinsMode;
FCBinClass::FCBList   FCBinClass::sm_list;
FCBinClass::FCBList   FCBinClass::sm_rankList;
FCBinClass::DPairSet  FCBinClass::sm_pairSet;
// Pointers
TMinuit *FCBinClass::sm_minuitPtr = 0;
TH1F    *FCBinClass::sm_peHistPtr = 0;

// single global variable
int gNumDim = FCBinClass::numDim();

// Constructors
FCBinClass::FCBinClass(int indicies[kMaxDim], bool forceFast)
{
   for (int dim = 0; dim < numDim(); ++dim)
   {
      m_indicies [dim] = indicies[dim];
   } // for dim
   for (int dim = numDim(); dim < kMaxDim; ++dim)
   {
      m_indicies [dim] = kEmpty;
   } // for dim
   m_rank = 0;
   m_muBest = m_P_muBest = m_P_mu = m_likeliRat = 0.;
   calculateMuBest (forceFast);
}

FCBinClass::FCBinClass(int index1, int index2, int index3, bool forceFast)
{
   m_indicies[ 1 - 1 ] = index1;
   m_indicies[ 2 - 1 ] = index2;
   m_indicies[ 3 - 1 ] = index3;
   m_rank = 0;
   m_muBest = m_P_muBest = m_P_mu = m_likeliRat = 0.;
   calculateMuBest (forceFast);
}

// Operators
/////////////////////////////////////////////////////
// We define > and < backwards so we don't have to //
// sort then reverse                               //
/////////////////////////////////////////////////////
bool
FCBinClass::operator> (const FCBinClass& other) const
{
   // backwards on purpose
   return (m_likeliRat < other.m_likeliRat);
}

bool
FCBinClass::operator< (const FCBinClass& other) const
{
   // backwards on purpose
   return (m_likeliRat > other.m_likeliRat);
}

// Regular Member Functions
void 
FCBinClass::calculateMuBest (bool forceFast)
{
   // muBest is the physically allowed mu that maximizes 
   // the probability of this entry
   //
   double accLumSum = 0.;
   double backSum   = 0.;
   double indexSum  = 0.;
   for (int dim = 1; dim <= numDim(); ++dim)
   {
      accLumSum += accLum (dim);
      backSum   += back   (dim);
      indexSum  += index  (dim);
   } // for dim

   // We set this here now.  If we are in fast mode or only 1 D, this
   // is what the value will be.  If, however, we use Minuit, this
   // gives us a good starting point.
   m_muBest = TMath::Max(0., (indexSum - backSum) / accLumSum);

   // What's the probability of finding m_indicies1 events given
   // muBest and the background

   // If we have multiple dimensions, we need to use Minuit UNLESS
   // forceFast is set true (i.e., we are using these bins for an
   // expected limit calculation, not FC).
   if (forceFast || (sm_fastMode && 1 == numDim()) ) 
   {
      m_P_muBest = 1.;
      for (int dim = 1; dim <= numDim(); ++dim)
      {
         m_P_muBest = TMath::Poisson(index(dim), m_muBest * accLum(dim)
                                     + back(dim));
      } // for dim
   } else {
      // fills both m_P_muBest and m_muBest
      find_max_P_mu();
   }
}

void
FCBinClass::find_max_P_mu()
{
   // Set index1..3
   _setSIndicies(m_indicies);

   // arglist are possilbe arguments you pass into minuit functions
   double arglist[10];
   // error flag
   int ierflg = 0;

   // We just calculated m_muBest assuming no systematic effects.
   // We should use this value as a good place to start the search
   arglist[0] = 1; // Fortran starts at 1
   arglist[1] = m_muBest;
   sm_minuitPtr->mnexcm ("SET PAR", arglist, 2, ierflg);
   double parErr;
   // release the first parameter (0 in C++)
   sm_minuitPtr->Release (0);
   // Start with simplex
   sm_minuitPtr->mnexcm ("SIM", arglist, 0, ierflg);
   // Now lets try Migrad
   sm_minuitPtr->mnexcm ("MIG", arglist, 0, ierflg);   
   // Get muBest   
   sm_minuitPtr->GetParameter (0, m_muBest, parErr);
   if (m_muBest < 0)
   {
      // This is a first attempt.  We should come back and revisit
      // This....
      // o.k.  We can't allow this non-physical answer, so...
      m_muBest = 0;
      m_P_muBest = 1.;
      for (int dim = 1; dim <= sm_numDim; ++dim)
      {
         // if we've got no signal, then it's all background, baby
         m_P_muBest *= TMath::Poisson( SMindex(dim), back(dim) );
      } // for dim
      return;
   } // if m_muBest < 0
   // if we're still here, then the fit returned a physically allowed value
   // now we've just got to figure out what it is
   m_P_muBest = _p_mu_withshift (m_muBest);
}

void 
FCBinClass::minuit_fcn (int &npar, double* grad, double &retval, double *par, 
                        int iflag)
{  
   // get the true value
   double trueValue = par[0];
   // get the shifts
   for (int shift = 0; shift < sm_numShiftUsed; ++shift)
   {
      sm_shift[shift] = par[shift + 1];
   }
   retval = -2 * _p_mu_withshift (trueValue);
   // print out stuff if 3 == iflag
   if (3 == iflag)
   {
      cout << " [" << trueValue << " r" << retval << "] ";
      for (int loop = 0; loop <= sm_numShiftUsed; ++loop)
      {
         cout << par[loop] << " ";
      }
      cout << endl;
   }
}

double
FCBinClass::_p_mu_withshift (double mu)
{
   double retval = 1.;
   // loop through the number of dimensions that we have
   for (int dim = 1; dim <= sm_numDim; ++dim)
   {
      double accLumCur = accLum(dim, true, mu);
      if (accLumCur < kMinAcceptance)
      {
         accLumCur = kMinAcceptance;
      }
      double backCur   = back(dim, true, mu);
      if (backCur < 0)
      {
         backCur = 0;
      }
      double expectedSignal = mu * accLumCur;
      retval *= TMath::Poisson (SMindex(dim), expectedSignal + backCur);
   } // for dim
   for (int shift = 0; shift < sm_numShiftUsed; ++shift)
   {
      // account for penalties
      retval *= _gaussExp( sm_shift[shift] );
   }
   return retval;
}

void 
FCBinClass::updateLikeliRatio()
{
   // Note: It is not necessary to update the likelihood ratio when
   // the Baysian mode.  However, m_P_mu must be updated.

   assert (m_P_muBest > 0);
   // if we are NOT using fast mode, then we've (hopefully)
   // already generated the PEs, so that should have updated
   // m_P_mu.
   //
   // If we ARE using fast mode, then we need to recalculate
   // m_P_mu
   if (sm_fastMode) 
   {
      m_P_mu = 1.;
      for (int dim = 1; dim <= numDim(); ++dim)
      {
         double expected = trueValue() * accLum(dim) + back(dim);
         m_P_mu *= TMath::Poisson(index(dim), expected); 
      } // for dim
      //  // wrong, but for debugging
      //  m_P_mu = TMath::Poisson(m_indicies1, sm_trueValue + sm_back1);
   }
   m_likeliRat = m_P_mu / m_P_muBest;
}

void
FCBinClass::_generatePEs(TH1F *histPtr, bool useELfactor)
{ 
   //cout << "generating PEs: " << trueValue() << endl;
   // Create histogram to store observed values for each PE.
   // First bin = 0 observed events.
   histPtr->Reset();

   // Throw PEs, number to throw set in static data member section
   for (int loop=0; loop < sm_numPEs; ++loop) 
   {
      if (! _throwAllRandomNumbers (useELfactor))
      {
         cerr << "Aborting" << endl;
         assert (0);
      }

      ////////////////////////////////
      // Store results in histogram //
      ////////////////////////////////
      if (1 == numDim())
      {
         histPtr->Fill ( numObsv (1, true) );
      } 
      else if (2 == numDim())
      {
         TH2F* h2fPtr = (TH2F*) histPtr;
         h2fPtr->Fill ( numObsv (1, true),  numObsv (2, true) );
      }
      else if (3 == numDim())
      {
         TH3F* h3fPtr = (TH3F*) histPtr;
         h3fPtr->Fill ( numObsv (1, true),  numObsv (2, true),
                        numObsv (3, true));
      }
   } // for loop
  
   // if we are doing this with the sm_peHistPtr,
   // then store probability for n observed events
   if (histPtr == sm_peHistPtr)
   {
      for (FCBListIter iter = sm_list.begin(); iter != sm_list.end(); ++iter) 
      {
         double probability = 
            _getBinContent ( histPtr,        iter->index(1),
                             iter->index(2), iter->index(3) ) /
            (double) sm_numPEs;
            iter->setP_mu (probability);
      } // for iter
   } // if using standard histogram
}

bool
FCBinClass::_throwAllRandomNumbers (bool useELfactor)
{
   bool okay = false;
   int redoCount = 0;
   do
   {
      for (int shift = 0; shift < sm_numShiftUsed; ++shift)
      {
         sm_shift[shift] = gRandom->Gaus (0., 1.);
      } // for shift
      // check to make sure all backgrounds and acceptances are
      // physical
      okay = true;
      for (int dim = 1; dim < numDim(); ++dim)
      {
         if ((accLum (dim, true) < kMinAcceptance) ||
             (back   (dim, true) < 0 ) )
         {
            okay = false;
            if (++redoCount > sm_maxNumPETries)
            {
               cerr << "FCBinClass::_throwAllRandomNumbers() Error: Could not"
                    << " throw a successful PE at true value "
                    << trueValue() << " in " << redoCount << " tries."
                    << endl;
               return false;
            } // if exceeded too many rethrowing tries
         } // if acceptances or background non-physical
      } // for dim
   } while (! okay);
   // throw the number of observed
   for (int dim = 1; dim <= numDim(); ++dim)
   {
      //double expected = trueValue() * accLum (dim, true) + back (dim, true);
      double expected = trueValue() * accLum (dim, false) + 
         back (dim, false, -1, useELfactor);
      sm_numObsvPE[ dim - 1 ] = gRandom->Poisson (expected);
   } // for dim
   return true;
}

int 
FCBinClass::_getBinContent (TH1F *histPtr, int index1, int index2, int index3)
{
   if (1 == numDim()) {
      // add 1 because of underflow bin
      int bin1 = index1 + 1;
      return (int) histPtr->GetBinContent(bin1);
   }
   if (2 == numDim()) {
      // add 1 because of underflow bin
      int bin1 = index1 + 1;
      int bin2 = index2 + 1;
      TH2F* h2fPtr = (TH2F*) histPtr;
      return (int) h2fPtr->GetBinContent(bin1, bin2);
   }
   if (3 == numDim()) {
      // add 1 because of underflow bin
      int bin1 = index1 + 1;
      int bin2 = index2 + 1;
      int bin3 = index3 + 1;
      TH3F* h3fPtr = (TH3F*) histPtr;
      return (int) h3fPtr->GetBinContent(bin1, bin2, bin3);
   }
   // shoudln't be here
   assert(0);
   // to keep the compiler happy
   return kEmpty;
}

bool
FCBinClass::matches (const int indicies[kMaxDim]) const
{
   for (int loop = 0; loop < numDim(); ++loop)
   {
      if (indicies [loop] != m_indicies [loop])
      {
         return false;
      }
   } // for loop
   // If we're still here
   return true;
}

bool
FCBinClass::matches (int index1, int index2, int index3) const
{
   return ( (index1 == index (1)) &&
            (index2 == index (2)) &&
            (index3 == index (3)) );
}

int
FCBinClass::index (int nth) const
{
   assert (nth > 0 && nth <= kMaxDim);
   return m_indicies[ nth - 1 ];
}

void
FCBinClass::setIndex (int nth, int index)
{
   assert (nth > 0 && nth <= kMaxDim);
   m_indicies[ nth - 1 ] = index;
}

void
FCBinClass::getIndicies  (int indicies[kMaxDim]) const
{
   for (int loop = 0; loop < kMaxDim; ++loop)
   {
      indicies [loop] = m_indicies [loop];
   } // for loop
}

///////////////////////////////////
// ///////////////////////////// //
// // Static Member Functions // //
// ///////////////////////////// //
///////////////////////////////////

void
FCBinClass::setNumDim (int numDim)
{ 
   assert (numDim >= 1 && numDim <= kMaxDim);
   //cout << "setting numDim = " << numDim << endl;
   sm_numDim = numDim; 
   // Minuit stuff
   _setupMinuit();   
}

int
FCBinClass::numBins()
{
   if (1 == numDim())
   {
      return max (1) + 1;
   } 
   else if (2 == numDim())
   {
      return (max (1) + 1) * (max (2) + 1);
   }
   else
   {
      return (max (1) + 1) * (max (2) + 1) * (max (3) + 1);
   }
}

void
FCBinClass::_setupPEhist()
{
   if (sm_peHistPtr)
   {
      delete sm_peHistPtr;
   }
   if (1 == numDim())
   {
      sm_peHistPtr = new TH1F ("sm_peHist", "PE Histogram", 
                               max (1) + 1, -0.5, max (1) + 0.5);
   }
   else if (2 == numDim())
   {
      TH2F *ptr =  new TH2F ("sm_peHist", "PE Histogram", 
                             max (1) + 1, -0.5, max (1) + 0.5,
                             max (2) + 1, -0.5, max (2) + 0.5);
      sm_peHistPtr = (TH1F*) ptr;
   }
   else if (3 == numDim())
   {
      TH3F *ptr =  new TH3F ("sm_peHist", "PE Histogram", 
                             max (1) + 1, -0.5, max (1) + 0.5,
                             max (2) + 1, -0.5, max (2) + 0.5,
                             max (3) + 1, -0.5, max (3) + 0.5);
      sm_peHistPtr = (TH1F*) ptr;
   }
   else
   {
      // shouldn't be here
      assert(0);
   }
}

void
FCBinClass::_setupMinuit()
{
   if (sm_minuitPtr)
   {
      delete sm_minuitPtr;
   }
   int numParam = 1 + sm_numShiftUsed;
   sm_minuitPtr = new TMinuit (numParam);
   int errFlg;
   Double_t arglist[15];
   if (true)
   {
      // turn off Minuit printing
      arglist[0] = -1;
      sm_minuitPtr->mnexcm("SET PRint", arglist, 1, errFlg);
      arglist[0] = 0;
      sm_minuitPtr->mnexcm("SET NOWarnings", arglist, 1, errFlg);
   }
   // tell Minuit which function to use
   sm_minuitPtr->SetFCN(minuit_fcn);
   // setup particle lables
   // by setting the range to 0., 0., we are telling Minuit to
   // use no range
   sm_minuitPtr->mnparm (0, "trueValue",   0, 0.001, 0., 0., errFlg);
   for (int shift = 0; shift < sm_numShiftUsed; ++shift)
   {
      sm_minuitPtr->mnparm (1 + shift, Form ("shift%d", shift), 
                            0, 0.001, -10., 10., errFlg);
   } // for shift   
   if ( debug (kGeneral_debug) ) dout << "Setup Minuit" << endl;
}

// access functions
int
FCBinClass::max (int nth)
{
   assert (nth > 0 && nth <= kMaxDim);
   return sm_max[ nth - 1 ];
}

void
FCBinClass::setMax (int nth, int max)
{
   assert (nth > 0 && nth <= kMaxDim);
   sm_max[ nth - 1 ] = max;
}

void
FCBinClass::setNumObsv (int nth, int num)
{
   assert (nth > 0 && nth <= kMaxDim);
   sm_numObsv  [ nth - 1 ] = num;
   sm_numObsvPE[ nth - 1 ] = num;
}

void
FCBinClass::setNumObsv (const int numObsv[kMaxDim])
{ 
   for (int loop = 0; loop < kMaxDim; ++loop)
   {
      sm_numObsv [loop] = numObsv [loop]; 
   } // for loop
}     

void
FCBinClass::_setSIndicies (const int indicies[kMaxDim])
{
   for (int loop = 0; loop < kMaxDim; ++loop)
   {
      sm_indicies[loop] = indicies[loop];
   }
}

double
FCBinClass::accLum (int nth, bool useShiftedValue, double value)
{
   if (value < 0)
   {
      value = trueValue();
   }
   assert (nth > 0 && nth <= kMaxDim);
   double retval = sm_accLum[ nth - 1 ] + sm_accLumMu[ nth - 1] * value;
   if (useShiftedValue)
   {
      for (int shift = 0; shift < sm_numShiftUsed; ++shift)
      {
         retval += sm_shift[shift] * 
            (sm_accLumErr[ nth - 1 ][shift] + 
             sm_accLumMuErr[ nth - 1][shift] * value);
      } // for shift
   } // if not default values
   // reduce the value by the factor AFTER shifts
   retval *= sm_accLumFactor[ nth - 1 ];
   // calculate enhancement
   double denom = 1 + value * sm_muDenom1 + value * value * sm_muDenom2;
   if (denom > sm_maxDenomFactor)
   {
      denom = sm_maxDenomFactor;
   }
   if (denom < sm_minDenomFactor)
   {
      denom = sm_minDenomFactor;
   }
   return retval / denom;
}

double
FCBinClass::back (int nth, bool useShiftedValue, double value, 
                  bool useELfactor)
{
   if (value < 0)
   {
      value = trueValue();
   }
   assert (nth > 0 && nth <= kMaxDim);
   double retval = sm_back[ nth - 1 ] + sm_backMu[ nth - 1] * value;
   if (useShiftedValue)
   {
      for (int shift = 0; shift < sm_numShiftUsed; ++shift)
      {
         retval += sm_shift[shift] * 
            (sm_backErr[ nth - 1 ][shift] + 
             sm_backMuErr[ nth - 1][shift] * value);
      } // for shift
   } // if not default values
   // reduce the value by the factor AFTER shifts
   retval *= sm_backFactor[ nth - 1 ];
   if (useELfactor)
   {
      retval *= sm_ELbackFactor[ nth - 1];
   }
   return retval;
}

int
FCBinClass::numObsv (int nth, bool useShiftedValue)
{
   assert (nth > 0 && nth <= kMaxDim);
   if (! useShiftedValue)
   {
      return sm_numObsv[ nth - 1 ];
   }
   return sm_numObsvPE[ nth - 1 ];
}

int
FCBinClass::SMindex (int nth)
{
   assert (nth > 0 && nth <= kMaxDim);
   return sm_indicies[ nth - 1 ];
}

// setting functions
void 
FCBinClass::setAccLums (double accLum1, double accLum2, double accLum3) 
{ 
   assert (accLum1 > 0 && accLum2 > 0 && accLum3 > 0);
   sm_accLum[1 - 1] = accLum1; 
   sm_accLum[2 - 1] = accLum2; 
   sm_accLum[3 - 1] = accLum3; 
   if (autoUpdate()) _updateAllLikeliRatios(true);
}

void 
FCBinClass::setAccLum (int nth, double accLum) 
{ 
   assert (nth > 0 && nth <= kMaxDim);
   assert (accLum > 0);
   sm_accLum[ nth - 1 ] = accLum; 
   if (autoUpdate()) _updateAllLikeliRatios(true);
}

void 
FCBinClass::setBackgrounds (double back1, double back2, double back3) 
{ 
   sm_back[1 - 1] = back1; 
   sm_back[2 - 1] = back2; 
   sm_back[3 - 1] = back3; 
   if (autoUpdate()) _updateAllLikeliRatios(true);
}

void 
FCBinClass::setBack (int nth, double back) 
{ 
   assert (nth > 0 && nth <= kMaxDim);
   sm_back[ nth - 1 ] = back; 
   if (autoUpdate()) _updateAllLikeliRatios(true);
}

void
FCBinClass::updateAll()
{
   _updateAllLikeliRatios(true);
}

void 
FCBinClass::setTrueValue (double trueValue) 
{ 
   sm_trueValue = trueValue;
   double probSum;
   bool firstTime = true;
   do {
      if (! firstTime)
      {
         // if we're here and it's not the first time, we should
         // double the number of bins
         if ( debug (kGeneral_debug) )dout << endl;
         _resize();
      }
      if (! sm_fastMode) {
         _generatePEs();
      }
      // update the likelihood ratios withOUT
      // recalculating Mu best and P (mu best)
      _updateAllLikeliRatios (false);
      // now check the probability sum to make sure we're close enough
      probSum = 0.;
      for (FCBListIter iter = sm_list.begin(); iter != sm_list.end(); ++iter) 
      {
         probSum += iter->prob();
      } // for iter
      // dout << "probSum " << probSum << endl;
      firstTime = false;
      if ( debug (kGeneral_debug) ) dout << "probSum " << probSum << endl;
   } while (probSum < sm_minProbSum);
}

double
FCBinClass::findTFCUpperLimit (double percent)
{
   // if unphysical, use the default
   if (percent <= 0 || percent >= 1)
   {
      percent = sm_confidenceLevel;
   }
   // to guess where to start, we'll use the root TFeldmanCousins 
   // to tell us where it thinks the right answer is.
   // First, we need to collapse everything down to one bin
   double backSum = 0, accLumSum = 0, numObsvSum = 0;
   for (int dim = 1; dim <= numDim(); ++dim)
   {
      backSum    += back    (dim);
      accLumSum  += accLum  (dim);
      numObsvSum += numObsv (dim);
   } // for dim
   TFeldmanCousins fc(percent);
   double upper = fc.CalculateUpperLimit(numObsvSum, backSum);
   if (upper < 0.001)
   {
      return -1.;
   }
   upper /= accLumSum;
   if (sm_davidCorr)
   {
      if (upper < 0.5)
      {
         upper = (1 - sqrt ( 1 - 2 * upper ) ) / 2.;
      } else {
         upper = 1.0;
      }
   }
   return upper;
}

double
FCBinClass::findTFCLowerLimit (double percent)
{
   // if unphysical, use the default
   if (percent <= 0 || percent >= 1)
   {
      percent = sm_confidenceLevel;
   }
   // to guess where to start, we'll use the root TFeldmanCousins 
   // to tell us where it thinks the right answer is.
   // First, we need to collapse everything down to one bin
   double backSum = 0, accLumSum = 0, numObsvSum = 0;
   for (int dim = 1; dim <= numDim(); ++dim)
   {
      backSum    += back    (dim);
      accLumSum  += accLum  (dim);
      numObsvSum += numObsv (dim);
   } // for dim
   TFeldmanCousins fc(percent);
   return fc.CalculateLowerLimit(numObsvSum, backSum) / accLumSum;
}

double
FCBinClass::findBestValue (int index1, int index2, int index3)
{
   int indicies[kMaxDim] = {index1, index2, index3};
   for (FCBListIter iter = sm_list.begin();
        iter != sm_list.end();
        ++iter)
   {
      if (iter->matches(indicies))
      {
         return iter->muBest();
      }
   } // for iter
   // if we're still here, we never found anything
   return -1.;
}

void
FCBinClass::findLimits (double &lower, double &upper,
                        double &center, double percent)
{
   lower = findLowerLimit (percent);
   upper = findUpperLimit (percent);
   center = findBestValue ();
}

double
FCBinClass::findExpectedLimit (double &toobig, double &RMS,
                               TH1F* expectedHistPtr)
{
   double oldTrueValue = trueValue();
   // for the background distribution, we only want background
   setTrueValue (0.);
   FCBList fcblist = getPElist(true);
   //printAllBins (fcblist);
   fcblist.sort();
   double probSum = 0.;
   double limSum  = 0.;
   double lim2Sum = 0.;
   double biggestLimit = 0.;
   if ( debug (kExpectedLimit_debug) )
   {
      int before = 1 + 2 * (numDim() - 1);
      int after  = 5 + 2 * (numDim() - 1);
      cout << setw(before) << "" << "Bins" << setw(after) << "";
      cout << "Weight      WghtSm       Limit     LoExpLim    UpExpLim"
           << endl;
   }

   // we use these two vectors to keep a 'list' of the last
   // probabilities and probability sums.  We can then go 'back' a
   // flexible number of bins to get the 'Too Big' limit.  My guess is
   // that we want to go back 1 bin per dimension.
   vector< double > probSumVec;
   for (FCBListIter iter = fcblist.begin();
        iter != fcblist.end();
        ++iter)
   {
      int bins[kMaxDim];
      iter->getIndicies (bins);
      if ( debug (kExpectedLimit_debug) )
      {
         cout << setw(4) << bins [0];
         if (numDim() >= 2) cout << " " << setw(3) << bins [1];
         if (numDim() >= 3) cout << " " << setw(3) << bins [2];
         cout.flush();
      }
      double prob = iter->prob();
      FCBinClass::setNumObsv (bins);
      double upper;
      if (kRootFeldmanCousinsMode == mode())
      {
         upper = FCBinClass::findTFCUpperLimit ();
      } else if (kFeldmanCousinsMode == mode()) 
      {
         upper = FCBinClass::findUpperLimit ();
      } else {
         upper = FCBinClass::BayesUpperLimit ();
      }
      // is this the biggest limit yet?
      if (upper > biggestLimit)
      {
         biggestLimit = upper;
      }
      limSum  += upper * prob;
      lim2Sum += upper * upper * prob;
      probSum += prob;
      probSumVec.insert( probSumVec.begin(), probSum );
      if (expectedHistPtr)
      {
         expectedHistPtr->Fill (upper, prob);
      }

      double lowerLim = limSum;
      double upperLim =  (limSum + (1 - probSum) * biggestLimit) / probSum;

      double currentDelta =  upperLim - lowerLim;
      
      // Debugging
      if ( debug (kExpectedLimit_debug) )
      {
         cout << Form (" : %10.6f", prob) 
              << Form ("  %10.6f", probSum)
              << Form ("  %10.6f", upper);
         if (probSumVec.size() >= 2)
         {
            cout << Form ("  %10.6f", lowerLim)
                 << Form ("  %10.6f", upperLim);
         }
         cout << endl;
      } // if debugging
      // If we are close enough, don't bother anymore
      if ( (probSumVec.size() >= 2) &&
           (probSumVec[1] > FCBinClass::minProbSum() ) ||
           ( currentDelta < sm_expectedDelta ) )
      {
         break;
      }
   } // for iter

   toobig = (limSum +  biggestLimit * (1 -  probSum)) / probSum;
   RMS = sqrt (lim2Sum - limSum*limSum);
   if ( debug (kExpectedLimit_debug) )
   {
      cout << "Expected Limit RMS: "
           << Form ("%.4f", RMS)
           << endl;
   }
   
   // just in case they wanted the old true value set
   setTrueValue (oldTrueValue);
   return limSum / probSum;
}

bool
FCBinClass::scanExpectedLimits (DPairVec &pairVec)
{
   if ( debug (kGeneral_debug) ) dout << "scanExpectedLimits()" << endl;
   if ( debug (kScanExpected_debug) )
   {
      cout << "In scanExpectedLimits using "
           << limitModeName() << endl;
   }
   
   // clear it now in case I have to return early.
   pairVec.clear();
   ////////////////////////////////////////////////////////
   // Verify that at least one dimension uses histograms //
   ////////////////////////////////////////////////////////
   bool useHisto [kMaxDim] = {};
   bool atLeastOne = false;
   for (int dim = 0; dim < numDim(); ++dim)
   {
      if ( sm_accLumHistNorm[dim] > 0 &&
           sm_backHistNorm[dim]   > 0 &&
           sm_accLumHistname[dim].length() &&
           sm_backHistname[dim].length() )
      {
         useHisto[dim] = true;
         atLeastOne = true;
      } else {
         useHisto[dim] = false;
      }
   } // for dim
   // make sure we've got a root file as well
   if (! sm_rootFile.length() )
   {
      atLeastOne = false;
   }
   if (! atLeastOne)
   {
      cerr << "FCBinClass::scanExpectedLimit() Error: You must specify at "
           << "least one dimension by histogram.  Skipping." << endl;
      return false;
   } // No histograms are used.

   /////////////////////////////////////////////////////
   // Get the information we want from the histograms //
   /////////////////////////////////////////////////////
   TFile *filePtr = TFile::Open( sm_rootFile.c_str() );
   if (! filePtr)
   {
      cerr << "FCBinClass::scanExpectedLimit() Error: Could not open '"
           << sm_rootFile << "' root file.  Skipping." << endl;
      return false;
   } // if filePtr wasn't open
   // If we're here, we've got the file open.  Let's unload the
   // histograms we want.
   DVec accLumIntegral[kMaxDim], backIntegral[kMaxDim];
   int numBins = -1;
   double step, edge;
   for (int dim = 0; dim < numDim(); ++dim)
   {
      if (! useHisto[dim] )
      {
         continue;
      }
      TH1F *accLumHist = (TH1F*) filePtr->Get( sm_accLumHistname[dim].c_str() );
      TH1F *backHist   = (TH1F*) filePtr->Get( sm_backHistname[dim].c_str() );
      if (! accLumHist || ! backHist)
      {
         cerr << "FCBinClass::scanExpectedLimit() Error: Could not get '"
              << sm_accLumHistname[dim] << "' or '"
              << sm_backHistname[dim] << "' from '"
              << sm_rootFile << "'root file.  Skipping." << endl;
         return false;         
      } // if can't get histograms
      if (! _getHistInfo (accLumHist, accLumIntegral[dim],
                          numBins, edge, step) ||
          ! _getHistInfo (backHist, backIntegral[dim],
                          numBins, edge, step))
      {
         cerr << "FCBinClass::scanExpectedLimit() Error: Inconsistent sizes "
              << "of '" << sm_accLumHistname[dim] << "' or '"
              << sm_backHistname[dim] << "' from '"
              << sm_rootFile << "'root file.  Skipping." << endl;
         return false;         
      } // if sizes aren't consistent
   } // for dim
   delete filePtr;
   
   /////////////////////////////////////////////////////
   // Copy all necessary accLum and background values //
   // to local ('l') copies                           //
   /////////////////////////////////////////////////////
   double lAccLum       [kMaxDim];
   double lAccLumMu     [kMaxDim];
   double lBack         [kMaxDim];
   double lBackMu       [kMaxDim];
   double lAccLumErr    [kMaxDim][kMaxUserVariables];
   double lAccLumMuErr  [kMaxDim][kMaxUserVariables];
   double lBackErr      [kMaxDim][kMaxUserVariables];
   double lBackMuErr    [kMaxDim][kMaxUserVariables];
   for (int dim = 0; dim < numDim(); ++dim)
   {
      lAccLum   [dim] = sm_accLum   [dim];
      lAccLumMu [dim] = sm_accLumMu [dim];
      lBack     [dim] = sm_back     [dim];
      lBackMu   [dim] = sm_backMu   [dim];
      for (int shift = 0; shift < sm_numShiftUsed; ++shift)
      {
         lAccLumErr   [dim][shift] = sm_accLumErr   [dim][shift];
         lAccLumMuErr [dim][shift] = sm_accLumMuErr [dim][shift];
         lBackErr     [dim][shift] = sm_backErr     [dim][shift];
         lBackMuErr   [dim][shift] = sm_backMuErr   [dim][shift];
      } // for shift
   } // for dim 

   ////////////////////////////////////////////////////////////////////////
   // We don't want to bother initializing everything to large values if //
   // our first values are much smaller.  Set all values to the first    //
   // point using the histograms and _then_ do the rest of the           //
   // initialization.                                                    //
   ////////////////////////////////////////////////////////////////////////
   for (int dim = 0; dim < numDim(); ++dim)
   {
      if (! useHisto[dim] )
      {
         // we don't need to reset the 'sm_' variables because they
         // already have the correct values
         continue;
      }
      int bin = 0; // use only the first bin for now
      double accLumFac = accLumIntegral[dim][bin] / sm_accLumHistNorm[dim];
      double backFac   = backIntegral[dim][bin]   / sm_backHistNorm[dim];
      if ( debug (kGeneral_debug) )
      {
         dout << "dim " << dim + 1 << endl
              << "  aLF " << accLumFac << endl
              << "  bF  " << backFac << endl;
      }
      sm_accLum   [dim] = accLumFac * lAccLum   [dim];
      sm_accLumMu [dim] = accLumFac * lAccLumMu [dim];
      sm_back     [dim] = backFac   * lBack     [dim];
      sm_backMu   [dim] = backFac   * lBackMu   [dim];
      for (int shift = 0; shift < sm_numShiftUsed; ++shift)
      {
         sm_accLumErr   [dim][shift] = accLumFac 
            * lAccLumErr   [dim][shift];
         sm_accLumMuErr [dim][shift] = accLumFac 
            * lAccLumMuErr [dim][shift];
         sm_backErr     [dim][shift] = backFac   
            * lBackErr     [dim][shift];
         sm_backMuErr   [dim][shift] = backFac   
            * lBackMuErr   [dim][shift];
      } // for shift
   } // for dim
   // now finish the initialization
   if ( debug (kGeneral_debug) ) dout << "about to..." << endl;
   _clearList();
   // Minuit stuff
   _setupMinuit();   
   _createList();
   if ( debug (kGeneral_debug) ) dout << "created list" << endl;
   _setupPEhist();
   if ( debug (kGeneral_debug) ) dout << "setup PEs" << endl;
   setTrueValue (0); // this will make it check its size
   if ( debug (kGeneral_debug) ) dout << "set true value to 0" << endl;
   // don't bother updating all likelihood ratios.  Will be done in
   // limit calculation.
   sm_pairSet.clear();
   if ( debug (kGeneral_debug) ) dout << "cleared sm_pairsSet" << endl;
   _calculateNumShift();
   // Minuit stuff
   _setupMinuit();   
   if ( debug (kGeneral_debug) ) dout << "Initialization finished." << endl;

   if ( debug (kGeneral_debug) ) dumpAllVariables();

   //////////////////////////////////////////////
   // Loop through integral of histograms and  //
   // Calculate expected limits at each point. //
   //////////////////////////////////////////////
   double oldAccLumFac[kMaxDim], oldBackFac[kMaxDim];
   double accLumFac[kMaxDim], backFac[kMaxDim];
   for (int dim = 0; dim < numDim(); ++dim)
   {
      // set the old factors to something big enough so the first
      // point will always be accepted.
      oldAccLumFac[dim] = oldBackFac[dim] = 2;
      // set the standard factors to 1
      accLumFac[dim]    = backFac[dim]    = 1;
   } // for dim
   // This should be true
   int size = numBins + 1; // = numBins + 2 - 1 (don't take last bin)
   for (int bin = 0; bin < size; ++bin)
   {
      double xvalue = edge + bin * step;
      double globalAccLumFac = 0;
      double maxFacIncrease  = 0;
      for (int dim = 0; dim < numDim(); ++dim)
      {
         if (! useHisto[dim] )
         {
            // we don't need to reset the 'sm_' variables because they
            // already have the correct values
            continue;
         }
         accLumFac[dim] = accLumIntegral[dim][bin] / sm_accLumHistNorm[dim];
         backFac[dim]   = backIntegral[dim][bin]   / sm_backHistNorm[dim];
         sm_accLum   [dim] = accLumFac[dim] * lAccLum   [dim];
         sm_accLumMu [dim] = accLumFac[dim] * lAccLumMu [dim];
         sm_back     [dim] = backFac[dim]   * lBack     [dim];
         sm_backMu   [dim] = backFac[dim]   * lBackMu   [dim];
         for (int shift = 0; shift < sm_numShiftUsed; ++shift)
         {
            sm_accLumErr   [dim][shift] = accLumFac[dim] 
               * lAccLumErr   [dim][shift];
            sm_accLumMuErr [dim][shift] = accLumFac[dim] 
               * lAccLumMuErr [dim][shift];
            sm_backErr     [dim][shift] = backFac[dim]   
               * lBackErr     [dim][shift];
            sm_backMuErr   [dim][shift] = backFac[dim]   
               * lBackMuErr   [dim][shift];
         } // for shift
         globalAccLumFac = std::max (accLumFac[dim], globalAccLumFac);
         double accLumRatio = 1 - ( accLumFac[dim] / oldAccLumFac[dim] );
         double backRatio   = 1 - ( backFac[dim]   / oldBackFac[dim]   ); 
         maxFacIncrease = std::max (maxFacIncrease, accLumRatio);
         maxFacIncrease = std::max (maxFacIncrease, backRatio);
      } // for dim
      // check to see if we are going to process this point
      if (maxFacIncrease < sm_minFactorChange)
      {
         // don't bother recalculating the limits at this point, there
         // isn't enough difference.
         //cout << Form ("%7.3f : Skipping", xvalue) << endl;
         continue;
      }
      // if we're still here, reset the oldAccLumFac and oldBackFacs
      for (int dim = 0; dim < numDim(); ++dim)
      {
         oldAccLumFac[dim] = accLumFac[dim];
         oldBackFac[dim]   = backFac[dim];
      } // for dim
      
      // if we're in kFeldmanCousinsMode, then we need to update everything
      if (kFeldmanCousinsMode == mode())
      {
         updateAll();
      }
      double toobig, rms;
      double expected = findExpectedLimit(toobig, rms);
      pairVec.push_back( make_pair( xvalue, expected ) );
      if ( debug (kScanExpected_debug) )
      {
         cout << Form ("%7.3f : %d ", xvalue, numDim());
         for (int dim = 1; dim <= numDim(); ++dim)
         {
            cout << Form ("(%.4f, %.4f]) ", 
                          accLumFac[ dim - 1 ],
                          backFac[ dim - 1 ]);
            
         } // for dim
         cout << Form (" : %5.2f%%  +- %2.2f%%", 100*expected, 100*rms) 
              << endl;
      }
      if (globalAccLumFac < 0.20)
      {
         break;
      }
   } // for bin   

   //////////////////////////////////////////////
   // Return all accLum and background numbers //
   // to their original state                  //
   //////////////////////////////////////////////
   for (int dim = 0; dim < numDim(); ++dim)
   {
      sm_accLum   [dim] = lAccLum   [dim];
      sm_accLumMu [dim] = lAccLumMu [dim];
      sm_back     [dim] = lBack     [dim];
      sm_backMu   [dim] = lBackMu   [dim];
      for (int shift = 0; shift < sm_numShiftUsed; ++shift)
      {
         sm_accLumErr   [dim][shift] = lAccLumErr   [dim][shift];
         sm_accLumMuErr [dim][shift] = lAccLumMuErr [dim][shift];
         sm_backErr     [dim][shift] = lBackErr     [dim][shift];
         sm_backMuErr   [dim][shift] = lBackMuErr   [dim][shift];
      } // for shift
   } // for dim

   return true;
}

void
FCBinClass::outputLimits (double percent)
{
   double lower, upper, center;
   findLimits (lower, upper, center, percent);
   if (lower > 0)
   {
      // we've got a two-sided limit
      double minus = center - lower;
      double plus  = upper  - center;
      cout << "[" << Form ("%0.3f", lower) << ", "
           << Form ("%0.3f", upper) << "] : " << Form ("%0.3f", center) 
           << " +" << Form ("%0.3f", plus) << " -" 
           << Form ("%0.3f", minus) << endl;
   }
   else
   {
      cout << "[" << Form ("%0.3f", lower) << ", "
           << Form ("%0.3f", upper) << "]" << endl;
   }
}

double
FCBinClass::findUpperLimit (double percent)
{
   // if unphysical, use the default
   if (percent <= 0 || percent >= 1)
   {
      percent = sm_confidenceLevel;
   }
   double fcLimit = findTFCUpperLimit (percent);
   //  Lets lower this estimate by 10%
   fcLimit *= 0.9;
   // O.k.  There's a bug where TFC gives 0 for an upper limit.  In
   // this case, we need to give a different value than 0.
   if (fcLimit < 0)
   {
      // TFC failed us, so we need to come up with something ourselves
      double backSum = 0, accLumSum = 0, numObsvSum = 0;
      for (int dim = 1; dim <= numDim(); ++dim)
      {
         backSum    += back(dim);
         accLumSum  += accLum(dim);
         numObsvSum += numObsv(dim);
      } // for dim
      double guess = backSum;
      if (numObsvSum > guess)
      {
         guess = numObsvSum;
      }
      guess += 1;
      guess *= 2;
      assert (accLumSum);
      fcLimit = guess / accLumSum;
   }


   ////////////////////////
   // Gross Search First //
   ////////////////////////
   double lower = 0.;
   double upper = 0.;
   bool found = false;
   double currentGuess = fcLimit;
   while (! found)
   {
      bool isUpper;
      setTrueValue (currentGuess);
      double value = findBinPercentage (isUpper);
      if ( debug (kGeneral_debug) )
      {
         dout << "[" << lower << ", " << upper << "] : " 
              << value <<  (isUpper ? " true" : " false") 
              << endl;
      }
      // for debugging purposes
      //double oldValue = currentGuess;
      // are we too low
      if (! isUpper || (value < percent))
      {
         // too low
         lower = currentGuess;
         // If we've set an upper bound (on the upper limit),
         // then we'll want to move half way in between the upper bound
         // and the current guess.  Otherwise, we'll just increase
         // the current guess by 20%
         if (upper > 0)
         {
            // move half way in between where we are
            // and the upper limit
            currentGuess += (upper - currentGuess) / 2;
         } else {
            // we've got no upper limit,
            // so increase by 20%
            currentGuess *= 1.2; 
         }
      } else {
         // too high
         upper = currentGuess;
         if (lower > 0)
         {
            currentGuess -= (currentGuess - lower) / 2;
         } else {
            currentGuess *= 0.8; 
         }
      }
      // are we close enough
      if ((upper > 0) &&  (upper - lower < sm_stepSize))
      {
         if (sm_davidCorr)
         {
            if (upper < 0.5)
            {
               upper = (1 - sqrt ( 1 - 2 * upper ) ) / 2.;
            } else {
               upper = 1.0;
            }
         }
         return upper;
      }
   } // while ! found
   return 0.;
}

double
FCBinClass::findLowerLimit (double percent)
{
   // if unphysical, use the default
   if (percent <= 0 || percent >= 1)
   {
      percent = sm_confidenceLevel;
   }

   // before we do anything, check the value at 0
   setTrueValue (0.);
   bool tempBool;
   double zeroValue = findBinPercentage (tempBool);
   if (zeroValue < percent)
   {
      return 0.;
   }
   

   double fcLimit = findTFCLowerLimit (percent);
   if (0 == fcLimit)
   {
      // if the FC limit is 0, but we don't think the limit is 0,
      // start off at a fraction of the upper limit
      fcLimit = 0.1 * findTFCUpperLimit (percent);
   }
   //  //dout << "fcLimit " << fcLimit << endl;
   //  lets lower this estimate by 10%
   fcLimit *= 0.9;

   ////////////////////////
   // Gross Search First //
   ////////////////////////
   double lower = 0.;
   double upper = 0.;
   bool found = false;
   double currentGuess = fcLimit;
   while (! found)
   {
      bool isUpper;
      setTrueValue (currentGuess);
      double value = findBinPercentage (isUpper);
      if ( debug (kGeneral_debug) )
      {
         dout << "[" << lower << ", " << upper << "] : " 
              << value <<  (isUpper ? " true" : " false") 
              << endl;
      }
      // for debugging purposes
      //double oldValue = currentGuess;
      // are we too high
      if (isUpper || (value < percent))
      {
         // too high
         // dout << "too high" << endl;
         upper = currentGuess;
         currentGuess -= (currentGuess - lower) / 2;
      } else {
         // too low
         //dout << "too low" << endl;
         lower = currentGuess;
         if (upper > 0)
         {
            // move half way in between where we are
            // and the upper limit
            currentGuess += (upper - currentGuess) / 2;
         } else {
            // we've got no upper limit,
            // so increase by 20%
            currentGuess *= 1.2; 
         }
      }
      // are we close enough
      if ((upper > 0) &&  (upper - lower < sm_stepSize))
      {
         return upper;
      }
   } // while ! found
   return 0.;
}

void 
FCBinClass::_clearList()
{
   sm_list.clear();
}

double
FCBinClass::findBinPercentage (bool &isUpper, 
                               int index1, int index2, int index3)
{
   IVec rankedBins;
   if (index1 < 0) 
   {
      index1 = sm_numObsv[0];
      index2 = sm_numObsv[1];
      index3 = sm_numObsv[2];
   }
   return FCBinClass::findBinPercentage (rankedBins, isUpper, 
                                         index1, index2, index3);
}

double
FCBinClass::findBinPercentage (FCBinClass::IVec &rankedBins, 
                               bool &isUpper, 
                               int index1, int index2, int index3)
{
   if (index1 < 0) 
   {
      index1 = sm_numObsv[0];
      index2 = sm_numObsv[1];
      index3 = sm_numObsv[2];
   }
   int inner1 = index1 - 1;
   int inner2 = kEmpty, inner3 = kEmpty;
   int outer1 = index1 + 1;
   int outer2 = kEmpty, outer3 = kEmpty;
   
   if (sm_numDim >= 2)
   {
      inner2 = index2 - 1;
      outer2 = index2 + 1;
   }
   if (3 == sm_numDim)
   {
      inner3 = index3 - 1;
      outer3 = index3 + 1;
   }
   bool innerFound = false;
   bool outerFound = false;
   double sumProb = 0.;
   for (FCBListIter iter = sm_rankList.begin();
        iter != sm_rankList.end();
        ++iter)
   {
      rankedBins.push_back( iter->index(1) );
      if (iter->matches (index1, index2, index3))
      { 
         break;
      }
      sumProb += iter->prob();      
      if (iter->matches (inner1, inner2, inner3))
      { 
         innerFound = true;
      }
      if (iter->matches (outer1, outer2, outer3))
      { 
         outerFound = true;
      }
   } // for iter
   if (outerFound)
   {
      // This must be the part of a lower bound of an acceptance interval
      // Which means this is a UPPER limit
      isUpper = true;
      return sumProb;
   }
   if (innerFound)
   {
      isUpper = false;
      return sumProb;
   }
   // If we're here, then we should really think about what we want to do
   // (It looks like this only happens if we don't have any bins
   dout << "waaaaa" << endl;
   isUpper = true;
   return sumProb;
}

///////////////////////////////////
// ///////////////////////////// //
// // Static Member Functions // //
// ///////////////////////////// //
///////////////////////////////////

void
FCBinClass::initialize (InitMode initmode)
{
   // TRandom1 is lousy
   delete gRandom;
   gRandom = new TRandom3;
   // deal with arguments
   if (sm_optionsFile.length())
   {
      OptionUtils::setVariablesFromFile (sm_optionsFile);
   }
   if (sm_modeInt >= (int) kFeldmanCousinsMode &&
       sm_modeInt <  (int) kNumLimitModes)
   {
      sm_mode = (LimitMode) sm_modeInt;
   }
   if (gNumDim != numDim())
   {
      setNumDim (gNumDim);
   }
   if ( debug (kGeneral_debug) )
   {
      dout << " ";
      for (int dim = 1; dim <= numDim(); ++dim)
      {
         cout << "  max" << dim << ": " << max (dim);
      }
      cout << endl;
   }
   // set the 'observed' of non-used dimensions to kEmpty
   for (int dim = numDim() + 1; dim <= kMaxDim; ++dim)
   {
      setNumObsv (dim, kEmpty);
   }
   if (kScanExpectedLimits == initmode)
   {
      // don't bother doing the rest
      sm_trueValue = 0;
      if ( debug (kGeneral_debug) ) 
         dout << "Skipping rest because scanning limits" << endl;
      return;
   }
   //////////////////////////////////////////////////////////////////////////
   // Initialize for everything EXCEPT scanning expected limits below here //
   //////////////////////////////////////////////////////////////////////////
   // Minuit stuff
   if ( debug (kGeneral_debug) ) dout << "about to..." << endl;
   _clearList();
   if ( debug (kGeneral_debug) ) dout << "cleared list" << endl;
   _setupMinuit();   
   _createList(sm_list);
   if ( debug (kGeneral_debug) ) dout << "created list" << endl;
   _setupPEhist();
   if ( debug (kGeneral_debug) ) dout << "setup PEs" << endl;
   if (kFeldmanCousinsMode == mode())
   {
      _updateAllLikeliRatios();
   }
   sm_pairSet.clear();
   _calculateNumShift();
   if ( debug (kGeneral_debug) ) dout << "Initialization finished." << endl;
   // Minuit stuff
   _setupMinuit();   
}

bool
FCBinClass::validateVariables()
{
   // check the number of dimensions
   if (numDim() < 1 || numDim() > 3)
   {
      cerr << "NumDim: " << numDim() << "not in valid range";
      return false;
   }
   for (int dim = 1; dim <= numDim(); ++dim)
   {
      // make sure all numbers >= 0
      if (back (dim) < 0)
      {
         cerr << "background " << dim << ": " << back (dim)
              << " is less than 0" << endl;
         return false;
      }
      if (accLum (dim) <= 0)
      {
         cerr << "accLum " << dim << ": " << accLum (dim)
              << " is less than or equal to 0" << endl;
         return false;
      }
      if (max (dim) < 10)
      {
         cerr << "max " << dim << ": " << max (dim)
              << " is too small" << endl;
         return false;
      }
   } // for dim
   return true;
}

void
FCBinClass::dumpAllVariables (double trueValue)
{
   cout << "numDim:    " << numDim() << endl;
   cout << "numPEs:    " << numPEs() << endl;
   cout << "fastmode:  " << (sm_fastMode ? "true" : "false") << endl;
   cout << "Conf leve: " << sm_confidenceLevel << endl;
   for (int dim = 1; dim <= numDim(); ++dim)
   {
      cout << endl;
      cout << "accLum" << dim << ":  " << accLum (dim, false, trueValue)
           << endl;
      cout << "back" << dim << ":    " << back (dim, false, trueValue)
           << endl;
      cout << "maxBin" << dim << ":  " << max (dim) << endl;
   } // for dim
   cout << endl;
}

void 
FCBinClass::_createList (bool forceFast)
{
   _createList (sm_list, forceFast);
}

void 
FCBinClass::_createList (FCBList &fcblist, bool forceFast)
{
   int numbins = numBins();
   if ( debug (kGeneral_debug) ) 
   {
      dout << "_createList()   numBins " << numbins << endl;
   } // if debug
   int bin1, bin2, bin3;
   for (int absIndex = 0; absIndex < numbins; ++absIndex)
   {
      _fromAbsIndexToIndicies(absIndex, bin1, bin2, bin3);
      // if ( debug (kGeneral_debug)  && (0 == absIndex % 1000) ) 
      // {
      //    dout << "  " << absIndex << " : "
      //         << bin1;
      //    if (numDim() >= 2)
      //    { 
      //       cout << ", " << bin2;
      //    }
      //    if (numDim() >= 3)
      //    { 
      //       cout << ", " << bin3;
      //    }
      // } // if debug && 1 in a 1000
      FCBinClass fcb (bin1, bin2, bin3, forceFast);
      fcblist.push_back( fcb );
   } // for absIndex
   if ( debug (kGeneral_debug) ) dout << "done " << numbins << endl;
}

void
FCBinClass::_fromAbsIndexToIndicies (int absIndex, int& index1, 
                                     int& index2, int& index3)
{
   // Instead of checking bounds and getting upset,
   // we use mod (%) to ensure we always have a physical input
   absIndex %= numBins(); 
   
   // dim 1
   index1 = absIndex % (max (1) + 1);

   // dim 2
   if (numDim() >= 2)
   {
      index2 = ( absIndex / (max (1) + 1) ) % (max (2) + 1);
   } else {
      index2 = kEmpty;
   }

   // dim 3
   if (numDim() >= 3)
   {
      index3 = ( absIndex / ( (max (1) + 1) * (max (2)+ 1) ) ) % (max (3) + 1);
   } else {
      index3 = kEmpty;
   }

   // All done
}

void
FCBinClass::_fromAbsIndexToIndicies (int absIndex, int indicies[kMaxDim])
{
   // Eventually, I should reverse which of the two functions is
   // written out, but for now, this version will simply call the
   // other function.
   _fromAbsIndexToIndicies (absIndex, 
                            indicies[0], indicies[1], indicies[2]);
}

void
FCBinClass::_updateAllLikeliRatios (bool updateMuBest)
{
   if (updateMuBest)
   {
      _zeroShift();
   }
   for (FCBListIter iter = sm_list.begin();
        iter != sm_list.end();
        ++iter)
   {
      if (updateMuBest)
      {
         iter->calculateMuBest();
      }
      // 070430 - moved after 'calculateMuBest()'
      iter->updateLikeliRatio();
   } // for iter
   _rankList();
}

void 
FCBinClass::_rankList()
{
   sm_rankList = sm_list;
   sm_rankList.sort();
   int rank = 0;
   for (FCBListIter iter = sm_rankList.begin();
        iter != sm_rankList.end();
        ++iter)
   {
      iter->setRank(++rank);
   } // for iter
}

// printing functions
void
FCBinClass::printAllBins ()
{
   printAllBins (sm_list);
}

void
FCBinClass::printAllBins (FCBinClass::FCBList &fcblist)
{
   cout << "trueValue " << sm_trueValue << endl;
   cout << " n   p(n|mu) best  Pbest   R" << endl;
   for (FCBListIter iter = fcblist.begin();
        iter != fcblist.end();
        ++iter)
   {
      cout << *iter << endl;
   } // for iter
   cout << endl;
}

void
FCBinClass::printRankedBins()
{
   double sumProb = 0.;
   cout << "trueValue " << sm_trueValue << endl;
   cout << setw( 5 * numDim() - 1) << " " 
        << "n   p(n|mu) best  Pbest   R" << endl;
   for (FCBListIter iter = sm_rankList.begin();
        iter != sm_rankList.end();
        ++iter)
   {      
      sumProb += iter->prob();
      cout << " " << *iter << Form ("%7.3f", sumProb) << endl;
   } // for iter
   cout << endl;
}

string
FCBinClass::limitModeName()
{
   string retval;
   if (kFeldmanCousinsMode == mode())
   {
      retval = "Feldman Cousins Mode";
   }
   else if (kRootFeldmanCousinsMode == mode())
   {
      retval = "Root's TFeldmanCousins Mode";
   }
   else if (kBayesMode == mode())
   {
      retval = "Bayes Mode";
   }
   else
   {
      retval = "Unknown Mode";
   }
   if (sm_fastMode)
   {
      retval += " (fast mode)";
   }
   return retval;
}

double
FCBinClass::_NZRandomGauss (double mean, double width)
{
   assert (mean + width >= 0);
   double retval;
   do {
      retval = gRandom->Gaus (mean, width);
   } while (retval <= 0);
   return retval;
}

TGraph*
FCBinClass::getTGraph (double lower, double upper, double step)
{
   // I'm not sure why I'm bothering, but...
   assert (lower + step < upper);
   assert (step > 0);
   vector< double > xvals, yvals;
   for (double trueVal = lower; trueVal <= upper; trueVal += step)
   {
      if ( debug() ) dout << "  " << trueVal << endl;
      bool isUpper;
      setTrueValue (trueVal);
      double val = findBinPercentage (isUpper);
      xvals.push_back (trueVal);
      yvals.push_back (val);
   } // for true
   return new TGraph (xvals.size(), &(xvals[0]), &(yvals[0]));
}

TH1F*
FCBinClass::getPEhist (const string &histname, bool useELfactor)
{
   TH1F *histPtr = (TH1F*) 0;
   if (1 == numDim())
   {
      histPtr = new TH1F (histname.c_str(), "PE Histogram", 
                          max (1) + 1, -0.5, max (1) + 0.5);
   }
   else if (2 == numDim())
   {
      TH2F *ptr =  new TH2F (histname.c_str(), "PE Histogram", 
                             max (1) + 1, -0.5, max (1) + 0.5,
                             max (2) + 1, -0.5, max (2) + 0.5);
      histPtr = (TH1F*) ptr;
   }
   else if (3 == numDim())
   {
      TH3F *ptr =  new TH3F (histname.c_str(), "PE Histogram", 
                             max (1) + 1, -0.5, max (1) + 0.5,
                             max (2) + 1, -0.5, max (2) + 0.5,
                             max (3) + 1, -0.5, max (3) + 0.5);
      histPtr = (TH1F*) ptr;
   }
   else
   {
      // shouldn't be here
      assert(0);
   }   
   _generatePEs (histPtr, useELfactor);
   // O.k.  Here's the deal: We want to make sure we've got enough
   // bins here.  If we don't then we'll need to resize and try
   // everything again.  Note that we have to duplicate this here
   // since we are not going through the normal channels.
   // assert (1 == numDim());
   // Assuming 1 dimension for now.  Alon will fix this later
   int numGoodEvents = 0;
   int numbins = numBins();
   for (int absIndex = 0; absIndex < numbins; ++absIndex)
   {
      int index1, index2, index3;
      _fromAbsIndexToIndicies (absIndex, index1, index2, index3);
      numGoodEvents += _getBinContent(histPtr, index1, index2, index3);
   }
   double probSum = numGoodEvents / (double) sm_numPEs;
   // are we close enough
   if (probSum < sm_minProbSum)
   {
      // D'oh, no we're not
      delete histPtr;
      // try again
      dout << endl;
      _resize();
      return getPEhist (histname);
   }
   // If we're still here, then just return the pointer and get on our
   // merry way.
   return histPtr;
}

FCBinClass::FCBList
FCBinClass::getPElist (bool useELfactor)
{
   // Create a list to put it in
   FCBList retval;
   _createList (retval, fastMode());
   // if we're in fast mode, we're not going to bother with PEs, but
   // rather calculate all probabilities directly
   if (fastMode())
   {
      double probSum = 0.;
      for (FCBListIter iter = retval.begin(); iter != retval.end(); ++iter) 
      {
         double probability = 1;
         for (int dim = 1; dim <= numDim(); ++dim)
         {
            double expected = back (dim, false, -1, useELfactor) 
               + trueValue() * accLum (dim);
            probability *= TMath::Poisson (iter->index(dim), expected);
         } // for dim
         iter->setP_mu (probability);
         // We are going to want this to be sortable.  Since the sorting
         // works on the likelihood ratio, we are going to set the
         // likelihood ratio equal to the probability.
         iter->setLikeliRat (probability);
         probSum += probability;
      } // for iter
      // we're almost there, but since we didn't use PE generation,
      // we haven't yet checked that we have enough bins
      //dout << "probSum " << probSum << endl;
      return retval;
   }
   // If we're still here, then we need to make PEs to see how these
   // things go.  So....
   // Get a TH1F pointer to a PE
   TH1F *histPtr = getPEhist("peHist", useELfactor);
   // Great.  Get the contents from the histPtr and set the probabilites
   double numEntriesD = (double) histPtr->GetEntries();
   for (FCBListIter iter = retval.begin(); iter != retval.end(); ++iter) 
   {
      double probability = 
         _getBinContent ( histPtr,        iter->index(1),
                          iter->index(2), iter->index(3) ) / numEntriesD;
      iter->setP_mu (probability);
      // We are going to want this to be sortable.  Since the sorting
      // works on the likelihood ratio, we are going to set the
      // likelihood ratio equal to the probability.
      iter->setLikeliRat (probability);
   } // for iter
   delete histPtr;
   return retval;
}

void
FCBinClass::testDifferences()
{
   cout << "in testDifferences()" << endl;
   int indicies [kMaxDim] = {};
   for (int dim = 1; dim <= numDim(); ++dim)
   {
      indicies[dim - 1] = numObsv (dim);
   } // for dim
   _setSIndicies (indicies);
   for (double value = 0.; value <= 1.; value += 0.05)
   {
      sm_minuitPtr->FixParameter(0);
      double fast   = _fastProbabilityAt (value);
      double minuit = _minuitProbabilityAt (value);
      sm_minuitPtr->Release(0);
      cout << setw(4) << value 
           << "  f " << setw(12) << fast
           << "  m " << setw(12) << minuit
           << "  d " << setw(12) << fast - minuit
           << endl;
   }
}

double
FCBinClass::_fastProbabilityAt (double value)
{
   ///////////////
   // Fast Mode //
   ///////////////
   double retval = 1.;
   for (int dim = 1; dim <= numDim(); ++dim)
   {
      // here we do want to use the shifted values
      double expected = 
         value * accLum (dim, true, value) + back (dim, true, value);
      retval *= TMath::Poisson(numObsv(dim), expected); 
   } // for dim
   // and now, we take the hit
   for (int shift = 0; shift < sm_numShiftUsed; ++shift)
   {
      // account for penalties
      retval *= _gaussExp( sm_shift[shift] );
   } // for shift
   return retval;
}

double
FCBinClass::_minuitProbabilityAt (double value)
{
   int errflg;
   Double_t arglist[15];
   arglist[0] = 1;
   arglist[1] = value;
   // set the parameter value
   sm_minuitPtr->mnexcm("SET PAR", arglist, 2, errflg);
   double check, checkerror;
   sm_minuitPtr->GetParameter(0, check, checkerror);
   // Start with simplex
   sm_minuitPtr->mnexcm ("SIM", arglist, 0, errflg);
   // // Now lets try Migrad
   // sm_minuitPtr->mnexcm ("MIG", arglist, 0, errflg);   
   return _likelihoodFit();
}

double
FCBinClass::_likelihoodFit()
{
   int npar = sm_numShiftUsed + 1;
   double *grad = 0;
   double retval;
   int iflag = 2;
   double *parameters = new double[npar];
   for (int loop = 0; loop < npar; ++loop)
   {
      double value, error;
      sm_minuitPtr->GetParameter (loop, value, error);
      parameters[loop] = value;
      // cout << "loop " << loop << " " << value << endl;
   } // for loop
   minuit_fcn (npar, grad, retval, parameters, iflag);
   delete parameters;
   return -0.5 * retval;
}

TGraph*
FCBinClass::probabilityGraph (bool forceNew)
{
   if ( debug (kGeneral_debug) ) dumpAllVariables();
   vector< double > xval, yval;
   if (!sm_pairSet.size() || forceNew)
   {
      _fillPairSet();
   }
   double max = -1.;
   for (DPairSetIter iter = sm_pairSet.begin();
        iter != sm_pairSet.end();
        ++iter)
   {
      double prob = iter->second;
      if (prob > max)
      {
         max = prob;
      }
      xval.push_back( iter->first );
      yval.push_back( prob * sm_userDoubles[0] );
   } // for value
   TGraph *retval = new TGraph (xval.size(), &(xval[0]), &(yval[0]));
   retval->SetMaximum (  1.1 * max );
   retval->SetMinimum ( 0 );
   return retval;
}

double
FCBinClass::BayesUpperLimit (double percent)
{
   double retval = -1;
   // if unphysical, use the default
   if (percent <= 0 || percent >= 1)
   {
      percent = sm_confidenceLevel;
   }
   _fillPairSet();
   double sum = 0.;
   DPairSetIter lower = sm_pairSet.begin();
   DPairSetIter upper = lower; ++upper;
   while (upper != sm_pairSet.end())
   {
      // We're using trapizoidal integration, so
      // base * (lower + upper) / 2
      sum += (upper->first - lower->first) 
         * (upper->second + lower->second) / 2;
      // increment the iterators
      ++lower;
      ++upper;
   } // while ! the end
   double target = sum * percent;
   sum = 0.;
   double lastSum = 0.; // to avoid compiler warnings
   lower = upper = sm_pairSet.begin(); ++upper;
   while (upper != sm_pairSet.end())
   {
      lastSum = sum;
      sum += (upper->first - lower->first) * (upper->second + lower->second) 
         / 2;
      // only increment if we are not yet there
      if (sum < target)
      {
         ++lower;
         ++upper;
      } else {
         break;
      }
   } // while ! the end
   if (sm_pairSet.end() == upper)
   {
      // This ain't good
      cerr << "FCBinClass::BayesUpperLimt: Something bad happened" << endl;
      return retval;      
   }

   // 
   // We have a trapizoid.  We need to figure out how much of it we
   // need to get the 'needed' area.
   //
   // 
   //     |         
   //  b0 |         |
   //     |         | b1
   //     |   wid   |
   //     -----------
   //     | x0 |
   //
   // Where the trapizoid from b0 to b0 + x0 has the 'needed' area
   
   // Our line  y = m x + b0.  Integrating this we get
   // m x^2 / 2 + b0 x = needed (area)
   // Using the quadradic equation, we get
   // - (b0/m) + sqrt ( (b0/m)**2 + 2 area/m )

   double area = target - lastSum;  // needed area
   double b0  = lower->second;   // left height
   double b1  = upper->second;   // right height
   double wid = upper->first - lower->first; // width of base
   double m   = (b1 - b0) / wid; // slope
   double xValue;
   //cout << "m " << m << endl;
   if (0 == m)
   {
      if (0 == b0)
      {
         return retval;
      } // not good
      xValue = area / b0;
   } else {
      double b0m = b0 / m;          // b0 / m (for convienience)
      if (m > 0)
      {
         xValue = - b0m + sqrt (b0m * b0m + 2 * area / m);
      } else {
         xValue = - b0m - sqrt (b0m * b0m + 2 * area / m);
      }
   }
   // // O.k.  Check how we did
   retval = lower->first + xValue;
   return retval;
}

void
FCBinClass::_fillPairSet()
{
   _zeroShift();
   // If we're going to use Minuit, lets' set it up
   if (! sm_fastMode)
   {
      sm_minuitPtr->FixParameter(0);
      int indicies [kMaxDim] = {};
      for (int dim = 1; dim <= numDim(); ++dim)
      {
         indicies[dim - 1] = numObsv (dim);
      } // for dim
      _setSIndicies (indicies);
   }
   sm_pairSet.clear();
   // Use something slightly bigger than 1 as the upper limit to make
   // sure we hit the end point
   // maximum number of points in between gross points
   int numFinePoints  = 100;
   // minimum number of fine points
   int minFinePoints  = 5;
   // we want the number of points to be even
   if (sm_numGrossPoints % 2)
   {
      ++sm_numGrossPoints;
   }
   double step = sm_bayesUpperIntegral / sm_numGrossPoints;
   double maxProb = 0.;
   DPairSet tempSet;
   for (double value = 0; value <= sm_bayesUpperIntegral + step; value += step)
   {
      double prob;
      if (sm_fastMode)
      {
         prob = _fastProbabilityAt (value);
      } else {
         prob = _minuitProbabilityAt (value);
      }
      if (prob > maxProb)
      {
         maxProb = prob;
      }
      // // for debugging
      // double prob = value * value * value;
      tempSet.insert( make_pair(value, prob) );
   } // for value
   // sm_pairSet = tempSet;
   // return;
   sm_pairSet.clear();
   DPairSetIter lower = tempSet.begin();
   DPairSetIter upper = lower; ++upper;
   while (upper != tempSet.end())
   {
      // get the maximum probability (height)
      double height = std::max (lower->second, upper->second);
      int newPoints = int (numFinePoints * height / maxProb);
      if (newPoints < minFinePoints)
      {
         newPoints = minFinePoints;
      }
      double lowerX = lower->first;
      double upperX = upper->first;
      double stepSize = (upperX - lowerX) / newPoints;
      for (int loop = 1; loop <= newPoints; ++loop)
      {
         double xValue = lowerX + loop * stepSize;
         double prob;
         if (sm_fastMode)
         {
            prob = _fastProbabilityAt (xValue);
         } else {
            prob = _minuitProbabilityAt (xValue);
         }
         sm_pairSet.insert( make_pair(xValue, prob) );         
      } // for loop
      // increment the iterators
      ++lower;
      ++upper;
   } // while ! the end
   // If we're going to use Minuit, lets set it up
   if (! sm_fastMode)
   {
      sm_minuitPtr->Release(1);
   }
}

int
FCBinClass::FCBinFirstCall()
{
   // Anything that needs to be done automatically before anything else
   // should be done in this function
   //setNumDim (sm_numDim);
   // setup option maps
   // Integers
   // We can't set sm_numDim directly, so we set gNumDim instead
   addOptionKey ("numdim",        gNumDim); 
   addOptionKey ("numpes",        sm_numPEs);
   addOptionKey ("gross",         sm_numGrossPoints,
                 "Number of gross points for integration");
   addOptionKey ("debug",         sm_debug);
   addOptionKey ("mode",          sm_modeInt,
                 "0 - FC, 1 - TFC, 2 - Bayes");
   // Doubles   
   addOptionKey ("minprobsum",    sm_minProbSum);
   addOptionKey ("expectdel",     sm_expectedDelta);
   addOptionKey ("conflevel",     sm_confidenceLevel);
   addOptionKey ("resizefactor",  sm_resizeFactor);
   addOptionKey ("bayesupper",    sm_bayesUpperIntegral,
                 "Bayes upper integral limit");
   addOptionKey ("minchange",     sm_minFactorChange,
                 "Minimum change necessary in accLum or background to recalculate expected limit");
   addOptionKey ("denommu1",        sm_muDenom1,
                 "Enhancement factor proportional to mu");
   addOptionKey ("denommu2",        sm_muDenom2,
                 "Enhancement factor proportional to mu * mu");
   // Booleans                
   addOptionKey ("fastmode",      sm_fastMode);
   addOptionKey ("showvar",       sm_showVariables);
   addOptionKey ("david",         sm_davidCorr);
   addOptionKey ("hi2lo",         sm_fromHighToLow,
                 "Whether limit scans read histograms from high to low or vice versa");
   addOptionKey ("makeplots",     sm_makePlots);
   // strings
   addOptionKey ("options",       sm_optionsFile);
   addOptionKey ("rootfile",      sm_rootFile,
                 "File containing histograms for scanning expected limits");

   // do the N-dimension variables
   for (int dim = 0; dim < kMaxDim; ++dim)
   {
      addOptionKey( Form ("max%d",         dim + 1), sm_max        [dim] );
      addOptionKey( Form ("numobsv%d",     dim + 1), sm_numObsv    [dim] );
      addOptionKey( Form ("acclum%d",      dim + 1), sm_accLum     [dim] );
      addOptionKey( Form ("back%d",        dim + 1), sm_back       [dim] );
      addOptionKey( Form ("acclummu%d",    dim + 1), sm_accLumMu   [dim] );
      addOptionKey( Form ("backmu%d",      dim + 1), sm_backMu     [dim] );
      for (int loop = 0; loop < kMaxUserVariables; ++loop)
      {
         addOptionKey( Form ("acclumerr%d_%d",   dim + 1, loop), 
                       sm_accLumErr      [dim][loop] );
         addOptionKey( Form ("acclumerrmu%d_%d", dim + 1, loop), 
                       sm_accLumMuErr    [dim][loop] );
         addOptionKey( Form ("backerr%d_%d",     dim + 1, loop), 
                       sm_backErr        [dim][loop] );
         addOptionKey( Form ("backerrmu%d_%d",   dim + 1, loop), 
                       sm_backMuErr      [dim][loop] );
         addOptionKey( Form ("acclumhistname%d", dim + 1, loop), 
                       sm_accLumHistname [dim] );
         addOptionKey( Form ("acclumnorm%d", dim + 1, loop), 
                       sm_accLumHistNorm [dim] );
         addOptionKey( Form ("acclumfactor%d", dim + 1, loop), 
                       sm_accLumFactor   [dim] );
         addOptionKey( Form ("backhistname%d", dim + 1, loop), 
                       sm_backHistname   [dim] );
         addOptionKey( Form ("backnorm%d", dim + 1, loop), 
                       sm_backHistNorm   [dim] );
         addOptionKey( Form ("backfactor%d", dim + 1, loop), 
                       sm_backFactor     [dim] );
         addOptionKey( Form ("elbackfactor%d", dim + 1, loop), 
                       sm_ELbackFactor     [dim] );
      } // for loop
      // setup some default values
      sm_max            [dim] = 20;
      sm_numObsv        [dim] = kEmpty;
      sm_accLum         [dim] =  1.;
      sm_accLumHistNorm [dim] = -1.;
      sm_backHistNorm   [dim] = -1.;
      sm_accLumFactor   [dim] =  1.;
      sm_backFactor     [dim] =  1.;
      sm_ELbackFactor   [dim] =  1.;
   } // for dim

   // do the user variables
   for (int loop = 0; loop < kMaxUserVariables; ++loop)
   {
      addOptionKey ( Form ("userint%d",    loop), sm_userInts   [loop] );
      addOptionKey ( Form ("userdouble%d", loop), sm_userDoubles[loop] );
      addOptionKey ( Form ("userbool%d",   loop), sm_userBools  [loop] );
      addOptionKey ( Form ("userstring%d", loop), sm_userStrings[loop] );
   }
   
   return 0;
}

// user variables
int
FCBinClass::userInt (int which)
{
   assert (which >= 0 && which < kMaxUserVariables);
   return sm_userInts[which];
}

bool
FCBinClass::userBool (int which)
{
   assert (which >= 0 && which < kMaxUserVariables);
   return sm_userBools[which];
}

double
FCBinClass::userDouble (int which)
{
   assert (which >= 0 && which < kMaxUserVariables);
   return sm_userDoubles[which];
}

string
FCBinClass::userString (int which)
{
   assert (which >= 0 && which < kMaxUserVariables);
   return sm_userStrings[which];
}

void
FCBinClass::setUserInt (int which, int value)
{
   assert (which >= 0 && which < kMaxUserVariables);
   sm_userInts[which] = value;
}

void
FCBinClass::setUserBool (int which, bool value)
{
   assert (which >= 0 && which < kMaxUserVariables);
   sm_userBools[which] = value;
}

void
FCBinClass::setUserDouble (int which, double value)
{
   assert (which >= 0 && which < kMaxUserVariables);
   sm_userDoubles[which] = value;
}

void
FCBinClass::setUserString (int which, const string &value)
{
   assert (which >= 0 && which < kMaxUserVariables);
   sm_userStrings[which] = value;
}

void
FCBinClass::_calculateNumShift()
{
   sm_numShiftUsed = 0;
   if (sm_fastMode)
   {
      // don't bother doing anything else
      if ( debug (kGeneral_debug) ) 
         dout << " numShift " << sm_numShiftUsed << endl;
      return;
   }
   // start high and go low
   for (int loop = kMaxUserVariables - 1; loop >= 0; --loop)
   {
      // loop over all dimensions
      for (int dim = 0; dim < kMaxDim; ++ dim)
      {
         // are any of these non-zero?
         if (sm_accLumErr[dim][loop] ||
             sm_accLumMuErr[dim][loop] ||
             sm_backErr[dim][loop] ||
             sm_backMuErr[dim][loop])
         {
            if ( debug (kGeneral_debug) ) 
               dout << " dim " << loop + 1 << endl;
            // set number used
            sm_numShiftUsed = loop + 1;
            // set loop so that it will break out of that loop too
            loop = -1;
            // break out of the 'dim' loop
            break;
         }
      } // for dim
   } // for loop
   if ( debug (kGeneral_debug) ) 
      dout << " numShift " << sm_numShiftUsed << endl;
}

void
FCBinClass::_zeroShift()
{
   for (int loop = 0; loop < kMaxDim; ++loop)
   {
      sm_shift [loop] = 0;
   } // for loop
}

void
FCBinClass::_resize (bool forceFast)
{
   if ( debug (kGeneral_debug) )
   {
      dout << "";
      for (int dim = 1; dim <= numDim(); ++dim)
      {
         cout << "  max" << dim << ": " << max (dim);
      }
      cout << endl;
   }
   int oldMax [kMaxDim] = {};
   int numbins = numBins();
   // Loop over all bins.  If it is an edge bin for a certain
   // dimension, sum up the probability.  We want to increase the
   // dimensions that need it the most (i.e., that have the highest
   // probability sums)
   double dimProbSum [kMaxDim] = {};
   if (numDim() > 1)
   {
      // Loop over all bins.  If it is an edge bin, sum it up
      for (FCBListIter iter = sm_list.begin();
           sm_list.end() != iter;
           ++iter)
      {
         for (int dim = 0; dim < numDim(); ++dim)
         {
            int indicies [kMaxDim] = {};
            iter->getIndicies (indicies);
            if (max (dim + 1) == indicies [dim])
            {
               // cout << dim << ") " << indicies[0]
               //      << ", " << indicies[1] << " : "
               //      << iter->prob() << endl;
               dimProbSum [dim] += iter->prob();
            } // if edge
         } // for dim
      } // for iter
      // Find the maximum
      double maxSum = 0;
      for (int dim = 0; dim < numDim(); ++dim)
      {
         maxSum = std::max (maxSum, dimProbSum[dim]);
      } // for dim
      // normalize so maximum is no 1.
      if ( debug (kGeneral_debug) ) dout << " resize " << endl;
      for (int dim = 0; dim < numDim(); ++dim)
      {
         dimProbSum [dim] /= maxSum;
         if ( debug (kGeneral_debug) ) 
            dout << " dPS" << dim << ": "<< dimProbSum[dim] << endl;
      } // for dim
      if ( debug (kGeneral_debug) ) dout << " maxSum " << maxSum << endl;
   }
   else 
   {
      // Don't bother looping, just set the sum to 1.
      dimProbSum[0] = 1.;
   }
   for (int dim = 0; dim < kMaxDim; ++dim)
   {
      oldMax [dim] = sm_max [dim];
      // for now, just grow each dimension 
      sm_max [dim] = (int) (sm_max [dim] * 
                            ( 1. + sm_resizeFactor * dimProbSum[dim]) );
      // just in case that wasn't big enough to actually change the
      // value
      if (oldMax [dim] == sm_max [dim])
      {
         // make sure that we should have changed this one
         if (dimProbSum[dim] > 0.5)
         {
            // this one probaby should have changed, so
            sm_max [dim] += 1;
         } // if prob sum > 0.5
      } // if no change
      else if (sm_max[dim] > sm_maxSize)
      {
         cerr << "sm_max[" << dim << "] is greater than limit ("
              << sm_maxSize << ").  Aborting." << endl;
         exit(1);
      }
   } // for dim
   // Loop over all bins.  If it is a new bin, initialize it.
   numbins = numBins(); // get the new total.
   for (int absIndex = 0; absIndex < numbins; ++absIndex)
   {
      int indicies [kMaxDim] = {};
      _fromAbsIndexToIndicies (absIndex, indicies);
      bool newbin = false;
      for (int dim = 0; dim < numDim(); ++dim)
      {
         if (indicies[dim] > oldMax[dim])
         {
            // make a new FCBinClass instance and add it to the list
            newbin = true;
            break;
         } // if new bin
      } // for dim
      if (! newbin)
      {
         // no need to do anything
         continue;
      } // ! newbinInitialization finished.
      FCBinClass fcb (indicies, forceFast);
      fcb.updateLikeliRatio();
      sm_list.push_back( fcb );
   } // for absIndex
   if ( debug (kGeneral_debug) )
   {
      dout << "Resizing Finished.  Size " 
           << numBins() << " : " << sm_list.size() << "  :  MAX: " 
           << max (1) << " " << max(2) << endl;
   }
   _setupPEhist();
   if ( debug (kGeneral_debug) ) dout << "setup PEs" << endl;
   sm_pairSet.clear();
   _calculateNumShift();
   // Minuit stuff
   _setupMinuit();   
}

bool
FCBinClass::_getHistInfo (TH1F* histPtr, DVec &binIntegral, int &numBins, 
                          double &edge, double &step)
{
   int localNumBins = histPtr->GetNbinsX();
   double lowerEdge = histPtr->GetXaxis()->GetXmin();
   double upperEdge = histPtr->GetXaxis()->GetXmax();
   double localStep = (upperEdge - lowerEdge) / localNumBins;
   double localEdge;
   DVec binContents;
   for (int bin = 0; bin <= localNumBins + 1; ++bin)
   {
      binContents.push_back( histPtr->GetBinContent (bin) );
   } // for bin
   // if we are going from high to low, then we want to "integrate"
   // the bins from low to high.  Likewise, if we are going from low
   // to high, we want to integrate from high to low.  In this case,
   // we will need to reverse the content vector.

   if (sm_fromHighToLow)
   {
      // from high to low (or left to right)
      // 'step' should be negative
      localStep *= -1;      
      localEdge = upperEdge;
   } // from high to low 
   else 
   {
      // from low to high (or right to left)
      localEdge = lowerEdge;
      // reverse the vector
      reverse( binContents.begin(), binContents.end() );
   } // else from low to high
   double sum = 0;
   binIntegral.clear();
   for (DVecConstIter iter = binContents.begin();
        binContents.end() != iter;
        ++iter)
   {
      DVecConstIter nextIter = iter + 1;
      // We don't want the very last (overflow) bin
      if (binContents.end() != nextIter)
      {
         sum += *iter;
         binIntegral.push_back( sum );
      } // if not last bin
   } // for iter
   // we want the integral to go from high values to low values
   reverse( binIntegral.begin(), binIntegral.end() );
   if (numBins > 0)
   {
      // check all values
      if (localNumBins != numBins)
      {
         return false;
      }
      if (edge || localEdge)
      {
         if ( fabs( 2 * (edge - localEdge) / (edge + localEdge) ) > 0.001 )
         {
            return false;
         }
      }
      if ( fabs( 2 * (step - localStep) / (step + localStep) ) > 0.001 )
      {
         return false;
      }
      // If we're still here, the histograms match, so we're good to go.
   }
   numBins = localNumBins;
   step    = localStep;
   edge    = localEdge;

    if ( debug (kGeneral_debug) ) 
       dout << "binIntegral " << binIntegral.size() << endl;
   return true;
}

// friends
ostream& operator<< (ostream& o_stream, const FCBinClass &rhs)
{
   for (int dim = 0; dim < FCBinClass::numDim(); ++dim)
   {
      if (dim)
      {
         // not the first one
         o_stream << ", ";
      } 
      o_stream << setw(3) << rhs.m_indicies[dim];
   }
   o_stream << ") ";
   o_stream << Form("%.3f",  rhs.m_P_mu)      << "  " 
            << Form("%4.1f", rhs.m_muBest)    << "  "
            << Form("%.3f",  rhs.m_P_muBest)  << "  "
            << Form("%.3f",  rhs.m_likeliRat) << "  " 
        //  << setw(2) << rhs.m_rank
            << flush;
   return o_stream;
} 


// Getting fit result
//  Now fill fitted variables E0,G0 and associated errors
//  minuit.GetParameter(0,G0,errG0);
//  minuit.GetParameter(1,E0,errE0);
//  Get minimum log likelihood
//  double ln0,edm,errdef;
//  int nvpar,nparx,icstat;
//  minuit.mnstat(ln0,edm,errdef,nvpar,nparx,icstat);
//  Get 1 and 2 sigma contour
// //1 sigma
// Tgraph* graph1 =
// (TGraph*) minuit.Contour(10,0,1);
// // 2 sigma
// minuit.SetErrorDef(2);
// Tgraph*  graph2 =
// TGraph*) minuit.Contour(10,0,1);
