// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <algorithm>
#include "TMath.h"
#include "TCanvas.h"
#include "CLPTemplateFitter/CLPTemplateFitter.hh"
#include "CLPTemplateFitter/dout.hh"
#include "CLPTemplateMorph/dumpSTL.icc"

using namespace std;

// static variables
CLPTemplateFitter      *CLPTemplateFitter::sm_tfPtr     = 0;
const string            CLPTemplateFitter::kEmptyString = "";

CLPTemplateFitter::CLPTemplateFitter () :
   m_dataHPtr (0), m_minuitPtr (0), m_arrayAddress (0),
   m_numTemplates (0), m_numBinNorms (0),
   m_numBins (0), m_numMinuitParams (0), m_minuitVerboseLevel (-1),
   m_debugLevel (0), m_verbose (0), m_useAllBinsInPlots (true),
   m_doMinos (false), m_minExpectedBinContent (-1.)
{
}

CLPTemplateFitter::CLPTemplateFitter (const CLPTemplateFitter &rhs)
{
   *this = rhs;
   // unassign the two pointers we will delete
   m_arrayAddress = 0;
   m_minuitPtr = 0;
}

CLPTemplateFitter::~CLPTemplateFitter ()
{
   _destroyMinuit();
}

CLPTemplateFitter 
&CLPTemplateFitter::operator= (const CLPTemplateFitter& rhs)
{
   // Avoid overwriting myself
   if (this == &rhs)
   {
      return *this;
   }
   // assign all other variables
   m_arrayAddress = 0;
   m_minuitPtr = 0;
   return *this;
}

//////////////////////
// Member Functions //
//////////////////////

TH1F*
CLPTemplateFitter::addData (TH1F *dataHist)
{
   _validateHistogram ("addData", dataHist);
   TH1F *retval = m_dataHPtr;
   m_dataHPtr = dataHist;
   return retval;
}

int
CLPTemplateFitter::addTemplate (const string &name, TH1F *tempHist,
                                double norm, double mean, double sigma,
                                double start, double stop, double step)
{
   _validateHistogram ("addTemplate", tempHist);
   int index = (int) m_templateHPtrVec.size();
   m_templateHPtrVec.push_back    ( tempHist );
   m_normVec.push_back            ( norm     );
   m_constraintMeanVec.push_back  ( mean     );
   m_constraintSigmaVec.push_back ( sigma    );
   m_templateNameVec.push_back    ( name     );
   if (start == stop)
   {
      // we need to make values for this.  Since we don't know that we
      // have the data histogram _OR_ the bins picked, we're going to
      // have to make up some numbers.
      double size = tempHist->Integral();
      if (size <= 0)
      {
         size = 1;
      }
      double factor = 100 / size;
      start = -100 * factor;
      stop  =  100 * factor;
      step  = 1e-3 * factor;
   }
   m_startStopStepVec.push_back( StartStopStep (start, stop, step) );
   m_templateIndexMap [name] = index;
   m_numTemplates            = index + 1;
   // Is this really a morphing template?
   if (dynamic_cast< CLPTemplateMorph* > (tempHist) )
   {
      // Yup.
      m_morphSet.insert (index);
   }
   // if we've already setup minuit, unset it up because we're adding
   // new parameters
   if (m_minuitPtr)
   {
      _destroyMinuit();
   } 
  return index;
}

int
CLPTemplateFitter::addBinNorm (const string &name, 
                               const BinNormClass &binNorm,
                               double mean, double sigma)
{
   int index = (int) m_binNormVec.size();
   m_binNormVec.push_back                ( binNorm );
   m_binNormConstraintMeanVec.push_back  ( mean    );
   m_binNormConstraintSigmaVec.push_back ( sigma   );
   m_binNormNameVec.push_back            ( name    );
   m_binNormIndexMap [name] = index;
   m_numBinNorms            = index + 1;
   // if we've already setup minuit, unset it up because we're adding
   // new parameters
   if (m_minuitPtr)
   {
      _destroyMinuit();
   }
   return index;
}

void
CLPTemplateFitter::setBinNormFunctoid (int binNormIndex, int tempIndex,
                                       BinNormFunctoid &functoid)
{
   assert (binNormIndex >= 0 && binNormIndex < (int) m_binNormVec.size());
   m_binNormVec.at(binNormIndex).setFunctoid (tempIndex, functoid);
}

bool
CLPTemplateFitter::fixParameter (const string &name)
{
   int paramIndex = _paramIndex (name);
   if (-1 == paramIndex)
   {
      cerr << "CLPTemplateFitter::fixParameter() Error '"
           << name << "' is not a valid name." << endl;
      return false;
   }
   // insert paramIndex into set
   m_fixedParameterSet.insert (paramIndex);
   if (m_minuitPtr)
   {
      // If we already have Minuit setup, don't forget to tell it too.
      m_minuitPtr->FixParameter(paramIndex);
   }
   return true;
}

bool
CLPTemplateFitter::releaseParameter (const string &name)
{
   int paramIndex = _paramIndex (name);
   if (-1 == paramIndex)
   {
      cerr << "CLPTemplateFitter::releaseParameter() Error '"
           << name << "' is not a valid name." << endl;
      return false;
   }
   // remove paramIndex from set
   BinNormClass::ISetIter setIter = 
      m_fixedParameterSet.find (paramIndex);
   if (m_fixedParameterSet.end() != setIter)
   {
      // We found it.  Get rid of it.
      m_fixedParameterSet.erase (setIter);
   }
   if (m_minuitPtr)
   {
      // If we already have Minuit setup, don't forget to tell it too.
      m_minuitPtr->Release(paramIndex);
   }
   return true;
}

bool
CLPTemplateFitter::setParameter (const string &name, double value)
{
   int tempIndex, binNormIndex;
   int paramIndex = _paramIndex (name, tempIndex, binNormIndex);
   if (-1 == paramIndex)
   {
      cerr << "CLPTemplateFitter::setParameter() Error '"
           << name << "' is not a valid name." << endl;
      return false;
   }
   
   // Is this a template?
   if (tempIndex >= 0)
   {
      m_normVec[tempIndex] = value;
   }
   // is this a binNorm
   else if (binNormIndex >= 0)
   {
      m_binNormVec[binNormIndex].setStartingValue (value);
   }
   // If we've already setup a fitVec, then set its value, too
   if (m_fitVec.size())
   {
      m_fitVec[paramIndex] = value;
   }
   if (m_minuitPtr)
   {
      int errflg;
      Double_t arglist[15];
      arglist[0] = paramIndex + 1; // C++ to fortran
      arglist[1] = value;
      // set the parameter value
      m_minuitPtr->mnexcm("SET PAR", arglist, 2, errflg);
   }
   return true;
}

void
CLPTemplateFitter::setConstraint (const string &name, 
                                  double mean, double sigma)
{
   // fill this in soon.
}

void
CLPTemplateFitter::fit()
{
   if (! m_minuitPtr)
   {
      _initializeMinuit();
   }
   sm_tfPtr = this;
   m_minuitPtr->mnsimp(); // SIMPLEX
   double arglist[2];
   int ierflg;
   m_minuitPtr->mnexcm ("SIMP", arglist, 0, ierflg);
   m_minuitPtr->Migrad(); // Migrad
   m_minuitPtr->mnhess(); // Hess
   if (m_doMinos)
   {
      m_minuitPtr->mnexcm("MINO", arglist, 0, ierflg);
   }
   m_fitVec.clear();
   m_errorVec.clear();
   m_posErrorVec.clear();
   m_negErrorVec.clear();
   for (int paramIndex = 0; paramIndex < m_numMinuitParams; ++paramIndex)
   {
      double value = 0, error = 0;
      m_minuitPtr->GetParameter (paramIndex, value, error);
      m_fitVec.push_back (value);
      if (! error || ! m_doMinos)
      {
         m_errorVec.push_back (error);
         m_posErrorVec.push_back (0.);
         m_negErrorVec.push_back (0.);         
         continue;
      }
      double plus = 0, minus = 0, parab = 0, gcc = 0;
      m_minuitPtr->mnerrs(paramIndex, plus, minus, parab, gcc);
      m_posErrorVec.push_back (plus);
      m_negErrorVec.push_back (minus);
      m_errorVec.push_back (parab);
      //m_errorVec.push_back (error);
      dout << paramIndex << " " << error << " : " << parab << endl;
      //      << " +" << plus << "-" << minus << endl;
   } // for paramIndex;
   // dout << endl;
   // dumpSTL (m_errorVec);
   // cout << endl;
   if (m_verbose)
   {
      outputFitResults();
   }

   ////////////////////////////////////////
   // Get covariance matrix from TMinuit //
   // (is there a better way?)           //
   ////////////////////////////////////////
   m_covarMatrixDVV.clear();
   DVecVec tempCovarDVV;
   double *covMat = new double [m_numMinuitParams * m_numMinuitParams];
   m_minuitPtr->mnemat(covMat, m_numMinuitParams);
   
   for (int outer = 0; outer < m_numMinuitParams; ++outer)
   {
      DVec empty;
      m_covarMatrixDVV.push_back (empty);
      tempCovarDVV.push_back (empty);
      for (int inner = 0; inner < m_numMinuitParams; ++inner)
      {
         // Save the value to tempCoverDVV.
         tempCovarDVV[outer].push_back
            ( covMat [ outer * m_numMinuitParams + inner]);
         // Initialize m_covarMatrixDVV to 0.
         m_covarMatrixDVV[outer].push_back (0.);
      } // for inner
   } // for outer
   delete [] covMat;

   //////////////////////////////////////////////////////////////////////
   // Transfer covariance matrix from tempCovarDVV to                  //
   // m_covarMatrixDVV.  The difference here is that Minuit shrinks an //
   // NxN matrix down to an nxn matrix (where you have 'N' parameters, //
   // but only 'n' free parameters.                                    //
   //////////////////////////////////////////////////////////////////////
   // first get a 'map' of 'free parameter' to 'parameter'
   IVec freeParameterVec;
   for (int loop = 0; loop < m_numMinuitParams; ++loop)
   {
      if (! _paramIsFixed (loop) )
      {
         // We've got a free parameter
         freeParameterVec.push_back (loop);
      }
   } // for loop
   
   // Groovy.  Now copy the non-zero values
   int size = (int) freeParameterVec.size();
   assert (m_minuitPtr->GetNumFreePars() == size);
   for (int outer = 0; outer < size; ++outer)
   {
      // outer is the zero-suppressed index (1..n)
      // outerIndex is the full index (1..n)
      int outerIndex = freeParameterVec [outer];
      for (int inner = 0; inner < size; ++inner)
      {
         // inner is the zero-suppressed index (1..n)
         // innerIndex is the full index (1..n)
         int innerIndex = freeParameterVec [inner];
         m_covarMatrixDVV [outerIndex] [innerIndex] =
            tempCovarDVV  [outer]      [inner];
      } // for inner
   } // for outer

   if (m_verbose)
   {
      outputCovarianceMatrix();
   } // if verbose
}

double
CLPTemplateFitter::fitEverythingBut (const string &name,
                                     double value)
{
   // make sure that we know what variable to fix
   int paramIndex = -1;
   SIMapConstIter iter = m_templateIndexMap.find (name);
   if (m_templateIndexMap.end() != iter)
   {
      paramIndex = iter->second;
   } else {
      iter = m_binNormIndexMap.find (name);
      if (m_binNormIndexMap.end() != iter)
      {
         paramIndex = iter->second + m_numTemplates;
      }
   }
   if (-1 == paramIndex)
   {
      cerr << "CLPTemplateFitter::fitEverythingBut() Error '"
           << name << "' is not a valid name." << endl;
      return -99999;
   }
   bool isfixed = _paramIsFixed (paramIndex);
 
   // make sure Minuit is ready to go
   if (! m_minuitPtr)
   {
      _initializeMinuit();
   }
   if (! isfixed)
   {
      m_minuitPtr->FixParameter(paramIndex);
   }
   sm_tfPtr = this;
   int errflg;
   Double_t arglist[15];
   arglist[0] = paramIndex + 1; // C++ to fortran
   arglist[1] = value;
   // set the parameter value
   m_minuitPtr->mnexcm("SET PAR", arglist, 2, errflg);
   double check, checkerror;
   m_minuitPtr->GetParameter(paramIndex, check, checkerror);
   // Start with simplex
   m_minuitPtr->mnexcm ("SIM", arglist, 0, errflg);
   // // Now lets try Migrad
   m_minuitPtr->mnexcm ("MIG", arglist, 0, errflg);   
   m_fitVec.clear();
   m_errorVec.clear();
   for (int paramIndex = 0; paramIndex < m_numMinuitParams; ++paramIndex)
   {
      double value = 0, error = 0;
      m_minuitPtr->GetParameter (paramIndex, value, error);
      m_fitVec.push_back (value);
      m_errorVec.push_back (error);
   } // for paramIndex;
   // Great!  Now get the current value
   m_minuitPtr->GetParameter(paramIndex, check, checkerror);
   double ln0,edm,errdef;
   int nvpar,nparx,icstat;
   m_minuitPtr->mnstat (ln0, edm, errdef, nvpar, nparx, icstat);
   if (! isfixed)
   {
      m_minuitPtr->Release (paramIndex);
   }
   return ln0;
}

void
CLPTemplateFitter::scanVariable (CLPTrapezoid::Vec &retval, const string &name,
                                 double lower, double upper, int numPoints)
{
   retval.clear();
   // make sure that we know what variable to fix
   int paramIndex = -1;
   SIMapConstIter iter = m_templateIndexMap.find (name);
   if (m_templateIndexMap.end() != iter)
   {
      paramIndex = iter->second;
   } else {
      iter = m_binNormIndexMap.find (name);
      if (m_binNormIndexMap.end() != iter)
      {
         paramIndex = iter->second + m_numTemplates;
      }
   }
   if (-1 == paramIndex)
   {
      cerr << "CLPTemplateFitter::scanVariable() Error '"
           << name << "' is not a valid name." << endl;
      return;
   }
   //cout << "Modifying " << paramIndex << endl;
 
   // make sure Minuit is ready to go
   if (! m_minuitPtr)
   {
      _initializeMinuit();
   }
   m_minuitPtr->FixParameter(paramIndex);
   sm_tfPtr = this;
   double step = (upper - lower) / numPoints;
   for (int loop = 0; loop <= numPoints; ++loop)
   {
      double value = lower + loop * step;
      int errflg;
      Double_t arglist[15];
      arglist[0] = paramIndex + 1; // C++ to fortran
      arglist[1] = value;
      // set the parameter value
      m_minuitPtr->mnexcm("SET PAR", arglist, 2, errflg);
      double check, checkerror;
      m_minuitPtr->GetParameter(0, check, checkerror);
      // Start with simplex
      m_minuitPtr->mnexcm ("SIM", arglist, 0, errflg);
      // // Now lets try Migrad
      m_minuitPtr->mnexcm ("MIG", arglist, 0, errflg);   
      // Great!  Now get the current value
      double ln0,edm,errdef;
      int nvpar,nparx,icstat;
      m_minuitPtr->mnstat (ln0, edm, errdef, nvpar, nparx, icstat);
      if (m_debugLevel)
      {
         cout << setw(4) << value << " " << setw (10) << ln0 << endl;
      }
      retval.push_back ( CLPTrapezoid ( value, ln0 ) );
   } // for loop
   m_minuitPtr->Release (paramIndex);
}

double
CLPTemplateFitter::logLikelihoodValue (const DVec &parameterVec)
{
   sm_tfPtr = this;
   int npar = (int) parameterVec.size();
   double *gin = 0;
   int iflag = 0;
   double retval;
   minimizeFcn (npar, gin, retval, (double*) (&(parameterVec[0])), iflag);
   // delete parameterArray;
   return retval;
}

void 
CLPTemplateFitter::resetMinuitParameters()
{
   // I have two choices here.  I cam simply delete and re-initialize
   // Minuit, or I can go through and reset the parameters.
   // Me being lazy:
   if (m_minuitPtr)
   {
      delete m_minuitPtr;
      m_minuitPtr = 0;
   }
   _initializeMinuit();
}


const string&
CLPTemplateFitter::paramName (int paramIndex) const
{
   if (paramIndex < 0)
   {
      return kEmptyString;
   }
   if (paramIndex < m_numTemplates)
   {
      return m_templateNameVec[paramIndex];
   }
   paramIndex -= m_numTemplates;
   if (paramIndex < m_numBinNorms)
   {
      return m_binNormNameVec[paramIndex];
   }
   return kEmptyString;
}

TH1F*
CLPTemplateFitter::updatedHistogram()
{
   return updatedHistogram (m_fitVec);
}

TH1F*
CLPTemplateFitter::updatedHistogram (string name)
{
   return updatedHistogram (name, m_fitVec);
}
 
TH1F*
CLPTemplateFitter::updatedHistogram (const DVec &paramVec)
{
   // make sure we update any morph parameters if necessary
   _setAllMorphingParameters (& (paramVec[0]) );
   TH1F *clone = (TH1F*) m_dataHPtr->Clone ("clone");
   clone->Reset();
   for (int binIndex = 0; binIndex <= m_numBins; ++binIndex)
   {
      clone->SetBinContent (binIndex, _getTotalBinContent (binIndex,
                                                           paramVec));
   } // for binIndex
   return clone;
}

TH1F*
CLPTemplateFitter::updatedHistogram (string name,
                                     const DVec &paramVec)
{
   // make sure we update any morph parameters if necessary
   _setAllMorphingParameters (& (paramVec[0]) );
   int tempIndex = -1;
   SIMapConstIter iter = m_templateIndexMap.find (name);
   if (m_templateIndexMap.end() != iter)
   {
      tempIndex = iter->second;
   }
   if (tempIndex < 0 || tempIndex >= m_numTemplates)
   {
      cerr << "CLPTemplateFitter::updatedHistogram() Error: '"
           << name << "' is not a valid template name." << endl;
      return 0;
   }
   // write clone to memory, not current directory
   TH1::AddDirectory(false);
   name += "_updated";
   TH1F *clone = (TH1F*) m_templateHPtrVec[tempIndex]->Clone( name.c_str() );
   // Loop over all bins.  If desired, we will only "consider" bins
   // used in the fit.
   for (int binIndex = 0; binIndex <= m_numBins; ++binIndex)
   {
      double factor = _getBinFactor (tempIndex, binIndex, paramVec);
      // are we not including ALL bins AND this bin IS NOT to be fit
      if (! m_useAllBinsInPlots && ! _isBinFit (binIndex) )
      {
         factor = 0.;
      }
      double contents = clone->GetBinContent (binIndex) * factor;
      clone->SetBinContent (binIndex, contents);
      // Update error
      if (0)
      {
         double error = clone->GetBinError (binIndex) * factor;
         if (error)
         {
            clone->SetBinError   (binIndex, error);
         } // if non-zero error
      } // if recalculating error
   } // for binIndex;
   return clone;
}

TH1F*
CLPTemplateFitter::ratioHistogram() const
{
   TH1F *ratioHPtr = (TH1F*) m_dataHPtr->Clone("FitRatio");
   ratioHPtr->SetTitle ("Ratio of Data to Fitted Template");
   // Loop over all bins.  If desired, we will only "consider" bins
   // used in the fit.
   for (int binIndex = 0; binIndex <= m_numBins; ++binIndex)
   {
      // Is this in range? Is there anything here?
      double data = ratioHPtr->GetBinContent (binIndex);
      if (data <= 0)
      {
         ratioHPtr->SetBinContent (binIndex, 0.);
         ratioHPtr->SetBinError   (binIndex, 0.);
         continue;
      } // if not a bin we fit
      double ratio = 0, error = 0;      
      double fit  = _getTotalBinContent (binIndex, m_fitVec);
      // are we not including ALL bins AND this bin IS NOT to be fit
      if (! m_useAllBinsInPlots && ! _isBinFit (binIndex) )
      {         
         fit = 0.;
      }
      if (fit > 0)
      {
         ratio = data / fit;
         error = ratio / sqrt (data);
      }
      ratioHPtr->SetBinContent (binIndex, ratio);
      ratioHPtr->SetBinError   (binIndex, error);
   } // for binIndex
   return ratioHPtr;
}

TH1F*
CLPTemplateFitter::residualHistogram() const
{
   TH1F *residualHPtr = (TH1F*) m_dataHPtr->Clone("FitResidual");
   residualHPtr->SetTitle ("Residual of Data to Fitted Template");
   // Loop over all bins.  If desired, we will only "consider" bins
   // used in the fit.
   for (int binIndex = 0; binIndex <= m_numBins; ++binIndex)
   {
      // Is this in range? Is there anything here?
      double data = residualHPtr->GetBinContent (binIndex);
      if (data <= 0)
      {
         residualHPtr->SetBinContent (binIndex, 0.);
         residualHPtr->SetBinError   (binIndex, 0.);
         continue;
      } // if not a bin we fit
      // are we not including ALL bins AND this bin IS NOT to be fit
      if (! m_useAllBinsInPlots && ! _isBinFit (binIndex) )
      {         
         residualHPtr->SetBinContent (binIndex, 0.);
         residualHPtr->SetBinError   (binIndex, 0.);
         continue;
      }
      double fit  = _getTotalBinContent (binIndex, m_fitVec);
      residualHPtr->SetBinContent (binIndex, data - fit);
      residualHPtr->SetBinError   (binIndex, 0.1);
   } // for binIndex
   return residualHPtr;
}

void 
CLPTemplateFitter::outputFitResults() const
{
   cout << "Fit Results:" << endl;
   for (int paramIndex = 0; paramIndex < m_numMinuitParams; ++paramIndex)
   {
      cout << setw(2) << paramIndex << ") " 
           << setw(10) << paramName (paramIndex) << " : "
           << setw(12) << m_fitVec[paramIndex];
      if (m_errorVec[paramIndex])
      {
         cout << " +- " << setw(12) << m_errorVec[paramIndex] << endl;
      } else {
         cout << setw(16) << "(fixed)" << endl;
      }
   } // for paramIndex;
}

void 
CLPTemplateFitter::outputCovarianceMatrix (bool nonZeroOnly) const
{
   // Prepare a set to describe which parameters to look at
   BinNormClass::ISet paramSet;
   _fillParamISet (paramSet, nonZeroOnly);
   cout << endl << "Covariance Matrix:" << endl;
   for (BinNormClass::ISetConstIter outerIter = paramSet.begin();
        paramSet.end() != outerIter;
        ++outerIter)
   {
      cout << setw(2) << *outerIter << ") "
           << setw(10) << paramName( *outerIter ) << " : ";
      for (BinNormClass::ISetConstIter innerIter = paramSet.begin();
           paramSet.end() != innerIter;
           ++innerIter)
      {
         cout << "  " << setw (12) 
              << m_covarMatrixDVV[ *outerIter ][ *innerIter ];
      } // for innerIter
      cout << endl;
   } // for outerIter
   if (0)
   {
      cout << endl << "Minuit Covar:" << endl;
      m_minuitPtr->mnmatu (1);
   } // if printing Minuit's covariance matrix
   cout << endl;
}

void 
CLPTemplateFitter::outputCorrelationMatrix (bool nonZeroOnly) const
{
   // Prepare a set to describe which parameters to look at
   BinNormClass::ISet paramSet;
   _fillParamISet (paramSet, nonZeroOnly);
   cout << endl << "Correlation Matrix:" << endl;
   for (BinNormClass::ISetConstIter outerIter = paramSet.begin();
        paramSet.end() != outerIter;
        ++outerIter)
   {
      cout << setw(2) << *outerIter << ") "
           << setw(10) << paramName( *outerIter ) << " : ";
      for (BinNormClass::ISetConstIter innerIter = paramSet.begin();
           paramSet.end() != innerIter;
           ++innerIter)
      {
         double denom = m_covarMatrixDVV[ *outerIter ][ *outerIter ] *
            m_covarMatrixDVV[ *innerIter ][ *innerIter ];
         double value = 0.;
         if (denom > 0)
         {
            value = m_covarMatrixDVV[ *outerIter ][ *innerIter ] /
               sqrt (denom);
         }
         cout << Form ("  %6.3f", value);
      } // for innerIter
      cout << endl;
   } // for outerIter
   cout << endl;
}

void 
CLPTemplateFitter::removeBinFromFit (int bin)
{
   BinNormClass::ISetIter iter = m_binsSet.find (bin);
   // Did we find it?
   if (m_binsSet.end() != iter)
   {
      // remove it
      m_binsSet.erase (iter);
   } // if we found it
}

void 
CLPTemplateFitter::removeBinsFromFit (const BinNormClass::ISet &binset)
{
   for (BinNormClass::ISetConstIter iter = binset.begin();
        binset.end() != iter;
        ++iter)
   {
      removeBinFromFit (*iter);
   } // for iter;
}

double
CLPTemplateFitter::getTotalBinContent (int binIndex) const
{
   return _getTotalBinContent (binIndex, m_fitVec);
}

double
CLPTemplateFitter::getTotalBinContent (int binIndex, 
                                       const DVec &paramVec) const
{
   return _getTotalBinContent (binIndex, paramVec);
}

double
CLPTemplateFitter::getValue (const string &name) const
{
   int paramIndex = _paramIndex (name);
   if (paramIndex < 0)
   {
      cerr << "CLPTemplateFitter::getValue() Error '"
           << name << "' is not a valid name." << endl;
      return 0;
   }
   return m_fitVec[paramIndex];
}

TMatrix 
CLPTemplateFitter::getCovarianceMatrix (SVec &paramNamesVec, 
                                        bool nonZeroOnly) const
{
   paramNamesVec.clear();
   // Prepare a set to describe which parameters to look at
   BinNormClass::ISet paramSet;
   _fillParamISet (paramSet, nonZeroOnly);
   int size = (int) paramSet.size();
   assert (size > 0);
   TMatrix covarMatrix (size, size);
   int outer = 0;
   for (BinNormClass::ISetConstIter outerIter = paramSet.begin();
        paramSet.end() != outerIter;
        ++outerIter)
   {
      int inner = 0;
      for (BinNormClass::ISetConstIter innerIter = paramSet.begin();
           paramSet.end() != innerIter;
           ++innerIter)
      {
         covarMatrix ( outer, inner) =
            m_covarMatrixDVV[ *outerIter ][ *innerIter ];
         ++inner;
      } // for innerIter
      ++outer;
      // put the name of this parameter onto the paramsName vector
      paramNamesVec.push_back( paramName( *outerIter ) );
   } // for outerIter
   // for (int outer = 0; outer < size; ++outer)
   // {
   //    for (int inner = 0; inner < size; ++inner)
   //    {
   //       cout << Form (" %8.2e", covarMatrix (inner, outer) );
   //    } // for inner
   //    cout << endl;
   // } // for outer
   return covarMatrix;
}

void 
CLPTemplateFitter::fillParamVec (DVec &paramVec) const
{
   paramVec.clear();
   for (DVecConstIter iter = m_fitVec.begin();
        m_fitVec.end() != iter;
        ++iter)
   {
      paramVec.push_back( *iter );
   } // for iter
}

CLPTemplateFitter::SIMap
CLPTemplateFitter::getNameIndexMap() const
{
   // Start with the template map
   SIMap retval = m_templateIndexMap;
   // now add in the BinNorms
   for (SIMapConstIter iter = m_binNormIndexMap.begin();
        m_binNormIndexMap.end() != iter;
        ++iter)
   {
      retval[ iter->first ] = iter->second + m_numTemplates;
   } // for iter
   return retval;
}

int
CLPTemplateFitter::indexOfName (const string &name, 
                                const SIMap &nameIndexMap)
{
   SIMapConstIter iter = nameIndexMap.find (name);
   if (nameIndexMap.end() == iter)
   {
      // not found
      return -1;
   } else {
      // Got it
      return iter->second;
   }
}

void
CLPTemplateFitter::fillNameVec (SVec &nameVec) const
{
   nameVec  = m_templateNameVec;
   for (SVecConstIter iter = m_binNormNameVec.begin();
        m_binNormNameVec.end() != iter;
        ++iter)
   {
      nameVec.push_back( *iter );
   } // for iter
}

int
CLPTemplateFitter::parameterIndex (const string &name) const
{
   // Check the templates
   SIMapConstIter iter = m_templateIndexMap.find (name);
   if (m_templateIndexMap.end() != iter)
   {
      // Found it.  We're done
      return iter->second;
   }
   // O.k.  Check the BinNorms
   iter = m_binNormIndexMap.find (name);
   if (m_binNormIndexMap.end() != iter)
   {
      // Found it.  We're done
      return iter->second + m_numTemplates;
   }
   // Sorry.  No go
   return -1;
}

void
CLPTemplateFitter::_setAllMorphingParameters (const double *parameterArray)
{
   // are there any morphing templates?
   if (! m_morphSet.size())
   {
      // Nothing to see here
      return;
   }
   for (BinNormClass::ISetConstIter iter = m_morphSet.begin();
        m_morphSet.end() != iter;
        ++iter)
   {
      CLPTemplateMorph *morphPtr =
         dynamic_cast< CLPTemplateMorph* > (m_templateHPtrVec.at (*iter));
      assert (morphPtr);
      if (verbose())
      {
         TString name = morphPtr->GetName();
         TString match = "anti";
         // if (name.Contains(match))
         // {
         //    TCanvas c1;
         //    morphPtr->Draw();
         //    c1.Print (Form ("plots/morph_%s%.1f.eps", 
         //                    (const char*) match,
         //                    parameterArray[5]));
         // } // if anti
      } // if verbose 
      morphPtr->setParamVecFromArray (parameterArray);
      //cout << *morphPtr << endl;
   } // for iter
}

///////////////////////////////////
// ///////////////////////////// //
// // Static Member Functions // //
// ///////////////////////////// //
///////////////////////////////////

void 
CLPTemplateFitter::minimizeFcn (int &npar, double *gin, double &retval, 
                                double *parameterArray, int iflag)
{
   // static int count = 0;
   // if (++count < 2)
   // {      
   //    sm_tfPtr->m_debugLevel = 1;      
   //    // dump all input parameters
   //    cout << "Dumping parameters: " << npar << endl;
   //    cout << "Params: " << sm_tfPtr->m_minuitPtr->GetNumPars() << endl;
   //    int numparams = sm_tfPtr->m_minuitPtr->GetNumPars();
   //    for (int loop = 0; loop < numparams; ++loop)
   //    {
   //       cout << setw(2) << loop << ") "
   //            << setw(8) << sm_tfPtr->paramName (loop)
   //            << " : " << parameterArray[loop] << endl;
   //    } // for loop
   //    cout << endl;
   // } else {
   //    sm_tfPtr->m_debugLevel = 0;
   //    //cout << "minimizeFcn: " << ++count << endl;
   // }
   assert (sm_tfPtr);
   // for (int loop = 0; loop < npar; ++loop)
   // {
   //    cout << "  " << parameterArray[loop];
   // }
   // cout << endl;
   sm_tfPtr->_setArrayAddress (parameterArray);
   sm_tfPtr->_setAllMorphingParameters (parameterArray);
   retval = -2 * sm_tfPtr->_totalLogProb();
}

double
CLPTemplateFitter::logPoisson (double observed, double expected)
{
   //const double kMinDouble = -1.7976931348623157e+308;
   const double kMinDouble = -1.7976931348623157e+306;
   if (observed == 0.0)
   {
      return -1 * expected;
   }
   if (observed < 0 || expected <= 0)
   {
      return kMinDouble;
   }
   double retval =  observed * log(expected) - expected 
      - TMath::LnGamma(observed + 1.);
   if ( isnan(retval) )
   {
      // if we're here, then just use the Gaussian approximation.
      double sigma = sqrt (expected);
      double arg = (observed - expected) / sigma;
      retval = -0.5 * arg * arg - log ( sigma * sqrt ( 2 * TMath::Pi() ) );
   } // else if
   return retval;
}

//////////////////////////////
// Private Member Functions //
//////////////////////////////
double
CLPTemplateFitter::_getBinFactor (int tempIndex, int binIndex,
                                  double *arrayAddress) const
{
   _validateMinuitInit    ("getBinFactor");
   _validateTemplateIndex ("getBinFactor", tempIndex);
   _validateBinIndex      ("getBinFactor", binIndex);
   if (! arrayAddress)
   {
      arrayAddress = m_arrayAddress;
   }
   return arrayAddress[tempIndex] *
      _byBin (tempIndex, binIndex, arrayAddress);
}

double
CLPTemplateFitter::_getBinFactor (int tempIndex, int binIndex,
                                  const DVec &paramVec) const
{
   _validateMinuitInit    ("getBinFactor");
   _validateTemplateIndex ("getBinFactor", tempIndex);
   _validateBinIndex      ("getBinFactor", binIndex);
   return paramVec.at(tempIndex) *
      _byBin (tempIndex, binIndex, paramVec);
}

double
CLPTemplateFitter::_getBinContent (int tempIndex, int binIndex,
                                   double *arrayAddress) const
{
   _validateMinuitInit    ("getBinContent");
   _validateTemplateIndex ("getBinContent", tempIndex);
   _validateBinIndex      ("getBinContent", binIndex);
   double retval = _getBinFactor (tempIndex, binIndex, arrayAddress) 
      * m_templateHPtrVec.at(tempIndex)->GetBinContent (binIndex);
   return retval;
}

double
CLPTemplateFitter::_getBinContent (int tempIndex, int binIndex,
                                   const DVec &paramVec) const
{
   _validateMinuitInit    ("getBinContent");
   _validateTemplateIndex ("getBinContent", tempIndex);
   _validateBinIndex      ("getBinContent", binIndex);
   double retval = _getBinFactor (tempIndex, binIndex, paramVec)
      * m_templateHPtrVec.at(tempIndex)->GetBinContent (binIndex);
   return retval;
}

double
CLPTemplateFitter::_getTotalBinContent (int binIndex,
                                        double *arrayAddress) const
{
   _validateMinuitInit    ("gettotalBinContent");
   _validateBinIndex      ("gettotalBinContent", binIndex);
   double retval = 0;
   for (int tempIndex = 0; tempIndex < m_numTemplates; ++tempIndex)
   {
      retval += _getBinContent (tempIndex, binIndex, arrayAddress);
   }
   return retval;
}

double
CLPTemplateFitter::_getTotalBinContent (int binIndex,
                                        const DVec &paramVec) const
{
   _validateMinuitInit    ("gettotalBinContent");
   _validateBinIndex      ("gettotalBinContent", binIndex);
   double retval = 0;
   for (int tempIndex = 0; tempIndex < m_numTemplates; ++tempIndex)
   {
      retval += _getBinContent (tempIndex, binIndex, paramVec);
   }
   return retval;
}

double 
CLPTemplateFitter::_byBin (int tempIndex, int binIndex,
                           double *arrayAddress) const
{
   if (! arrayAddress)
   {
      arrayAddress = m_arrayAddress;
   }
   // Will calculate ALL factors that affect this template and bin,
   // not just the first one.
   double retval = 1.;
   for (BinNormClass::VecConstIter iter = m_binNormVec.begin();
        m_binNormVec.end() != iter;
        ++iter)
   {      
      int index = iter->paramIndex();
      retval *= iter->factor(tempIndex, binIndex, 
                             arrayAddress[index], arrayAddress);
   } // for iter
   return retval;
}

double 
CLPTemplateFitter::_byBin (int tempIndex, int binIndex,
                           const DVec &paramVec) const
{
   // Will calculate ALL factors that affect this template and bin,
   // not just the first one.
   double retval = 1.;
   for (BinNormClass::VecConstIter iter = m_binNormVec.begin();
        m_binNormVec.end() != iter;
        ++iter)
   {      
      int index = iter->paramIndex();
      retval *= iter->factor(tempIndex, binIndex, 
                             paramVec.at(index), &paramVec[0]);
   } // for iter
   return retval;
}

int
CLPTemplateFitter::_paramIndex (const string& name, 
                                int &tempIndex, int &binNormIndex) const
{
   int paramIndex = -1;
   tempIndex = binNormIndex = -1;
   SIMapConstIter iter = m_templateIndexMap.find (name);
   if (m_templateIndexMap.end() != iter)
   {
      tempIndex = paramIndex = iter->second;
   } else {
      // not a template.  Is it a binNorm?
      iter = m_binNormIndexMap.find (name);
      if (m_binNormIndexMap.end() != iter)
      {
         binNormIndex = iter->second;
         paramIndex = iter->second + m_numTemplates;
      } // if found
   }
   return paramIndex;
}

double
CLPTemplateFitter::_totalLogProb() const
{
   double logProb = 0.;
   ////////////////////////
   // Loop over all bins //
   ////////////////////////
   for (BinNormClass::ISetConstIter iter = m_binsSet.begin();
        m_binsSet.end() != iter;
        ++iter)
   {
      // if set, we want to make sure that we don't go below the
      // minimum expected bin content limit (if set)
      logProb += logPoisson (m_dataHPtr->GetBinContent (*iter),
                             std::max (_getTotalBinContent (*iter),
                                       m_minExpectedBinContent));
   } // for iter
   ///////////////////////////////
   // Loop over all constraints //
   ///////////////////////////////
   // Template Constraints
   for (int tempIndex = 0; tempIndex < m_numTemplates; ++tempIndex)
   {
      if (m_constraintSigmaVec[tempIndex] > 0)
      {
         double zScore = 
            (m_arrayAddress[tempIndex] - m_constraintMeanVec[tempIndex]) /
            m_constraintSigmaVec[tempIndex];
         logProb -= zScore * zScore / 2;
      }
   } // for tempIndex
   // BinNorm Constraints
   for (int binNormIndex = 0; binNormIndex < m_numBinNorms; ++binNormIndex)
   {
      if (m_binNormConstraintSigmaVec[binNormIndex] > 0)
      {
         int index = binNormIndex + m_numTemplates;
         double zScore = 
            (m_arrayAddress[index] - 
             m_binNormConstraintMeanVec[binNormIndex]) /
            m_binNormConstraintSigmaVec[binNormIndex];
         logProb -= zScore * zScore / 2;
      }
   } // for binNormIndex
   return logProb;
}

void
CLPTemplateFitter::_initializeMinuit()
{
   sm_tfPtr = this;
   if (m_minuitPtr)
   {
      cerr << "CLPTemplateFitter::_initializeMinuit() error: Minuit is alredy"
           << " initialized." << endl;
      return;
   }
   if (! m_numTemplates)
   {
      cerr << "CLPTemplateFitter::_initializeMinuit() error: Must have at"
           << " least 1 template." << endl;
      assert (0);
   }
   if (! m_dataHPtr)
   {
      cerr << "CLPTemplateFitter::_initializeMinuit() error: Must have already"
           << " called addData()." << endl;
      assert (0);
   }
   // are there any morphing templates?
   if (m_morphSet.size())
   {
      SVec nameVec;
      fillNameVec (nameVec);
      for (BinNormClass::ISetConstIter iter = m_morphSet.begin();
           m_morphSet.end() != iter;
           ++iter)
      {
         CLPTemplateMorph *morphPtr =
            dynamic_cast< CLPTemplateMorph* > (m_templateHPtrVec.at (*iter));
         assert (morphPtr);
         morphPtr->setInternalIndicies (nameVec);
      } // for iter
   } // morphing templates
   m_numMinuitParams = m_numTemplates + m_numBinNorms;   
   m_minuitPtr = new TMinuit (m_numMinuitParams + 3);
   m_minuitPtr->SetFCN (CLPTemplateFitter::minimizeFcn); 
   // // tell minuit to be relatively quiet
   m_minuitPtr->SetPrintLevel (m_minuitVerboseLevel);
   // set error to 1
   m_minuitPtr->SetErrorDef (1);
   
   double dataEvents = _eventsInRange (m_dataHPtr);
   assert (dataEvents > 0);
   // set up our parameters
   DVec paramVec;
   for (int tempIndex = 0; tempIndex < m_numTemplates; ++tempIndex)
   {
      double tempEvents = _eventsInRange( m_templateHPtrVec[tempIndex] );
      const StartStopStep  &sss = m_startStopStepVec.at(tempIndex);
      assert (tempEvents > 0);
      int ierrflg = 0;
      m_minuitPtr->mnparm (tempIndex, 
                           m_templateNameVec[tempIndex].c_str(),
                           m_normVec[tempIndex],
                           sss.step(),
                           sss.start(),
                           sss.stop(),
                           ierrflg);
      paramVec.push_back( m_normVec[tempIndex] );
      // should we fix this parameter
      if (m_fixedParameterSet.end() != 
          m_fixedParameterSet.find (tempIndex))
      {
         // yes
         m_minuitPtr->FixParameter (tempIndex);
      }
   } // for tempIndex
   for (int binNormIndex = 0; binNormIndex < m_numBinNorms; ++binNormIndex)
   {
      BinNormClass &binNorm = m_binNormVec[binNormIndex];
      binNorm.setParamIndex (binNormIndex + m_numTemplates);
      double value = binNorm.startingValue();
      int ierrflg = 0;
      int paramIndex = binNormIndex + m_numTemplates;
      m_minuitPtr->mnparm (paramIndex,
                           m_binNormNameVec[binNormIndex].c_str(),
                           value,
                           binNorm.step(),
                           binNorm.start(),
                           binNorm.stop(),
                           ierrflg);
      paramVec.push_back( value );
      // should we fix this parameter?
      if (m_fixedParameterSet.end() != 
          m_fixedParameterSet.find (paramIndex))
      {
         // yes
         m_minuitPtr->FixParameter (paramIndex);
      }
   } // for binNormIndex
   _setAllMorphingParameters (&(paramVec[0]));
}

double
CLPTemplateFitter::_eventsInRange (TH1F* histPtr)
{
   double retval = 0.;
   for (BinNormClass::ISetConstIter iter = m_binsSet.begin();
        m_binsSet.end() != iter;
        ++iter)
   {
      retval += histPtr->GetBinContent (*iter);
   } // for iter
   return retval;
}

void
CLPTemplateFitter::_destroyMinuit()
{
   delete m_minuitPtr;
   m_minuitPtr = 0;
}

void
CLPTemplateFitter::_validateBinIndex (const string &function, 
                                      int binIndex) const
{
   if (binIndex < 0 || binIndex > m_numBins)
   {
      cerr << "CLPTemplateFitter::" << function 
           << "(): bin index " << binIndex << " is invalid." << endl;
      assert (0);
   }
}

void
CLPTemplateFitter::_validateTemplateIndex (const string &function, 
                                           int tempIndex) const
{
   if (tempIndex < 0 || tempIndex > m_numTemplates)
   {
      cerr << "CLPTemplateFitter::" << function 
           << "(): template index " << tempIndex << " is invalid." << endl;
      assert (0);
   }
}

void
CLPTemplateFitter::_validateMinuitInit (const string &function) const
{
   if (! m_minuitPtr)
   {
      cerr << "CLPTemplateFitter::" << function << 
         "() Error: Called before Minuit initialization." << endl;
      assert (0);
   }
}

void
CLPTemplateFitter::_validateHistogram (const string &function, 
                                       TH1F *histPtr)
{
   int numBins = histPtr->GetNbinsX();
   if (m_numBins)
   {
      // this is not the first histogram.  Make sure it's consistent.
      if (m_numBins != numBins)
      {
         cerr << "CLPTemplateFitter::" << function 
              << "() error: histogram "
              << "incompatible. " << m_numBins << " != "
              << numBins << endl;
         assert(0);
      }
   } // if m_numBins
   else
   {
      // this is our first histogram
      m_numBins = numBins;
      // check to see if min/max bin are set
      if (! m_binsSet.size())
      {
         BinNormClass::insertRangeIntoSet (m_binsSet, 1, numBins);
      } else {
         // Check to make sure that this doesn't violate the binning
         // set that we already have.

         // if (m_maxBin > numBins)
         // {
         //    cerr << "CLPTemplateFitter::" << function 
         //         << "() error: histogram "
         //         << "incompatible with  max. " << m_numBins << " < " 
         //         << m_maxBin << "." << endl;
         //    assert(0);    
         // }        
      }
   } // if else m_numBins   
}

bool 
CLPTemplateFitter::_paramIsFixed (int paramIndex) const
{
   double numParams = 0;
   if (m_minuitPtr)
   {
      numParams = m_numMinuitParams;
   } else {
      numParams = m_numTemplates + m_numBinNorms;
   }
   if (paramIndex < 0 || paramIndex >= numParams)
   {
      cerr << "CLPTemplateFitter::_paramIsFixed() Error: paramIndex ("
           << paramIndex << ") is not in allowed ramge [0, "
           << numParams << "[." << endl;
      return false;
   }
   return (m_fixedParameterSet.end() != 
           m_fixedParameterSet.find (paramIndex));
}

void
CLPTemplateFitter::_fillParamISet (BinNormClass::ISet &paramSet, 
                                   bool nonZeroOnly) const
{
   paramSet.clear();
   if (nonZeroOnly)
   {
      // Only use the parameters that are not fixed
      for (int loop = 0; loop < m_numMinuitParams; ++loop)
      {
         if (! _paramIsFixed (loop) )
         {
            // We've got a free parameter
            paramSet.insert (loop);
         }
      } // for loop
   }
   else
   {
      // use all parameters
      BinNormClass::insertRangeIntoSet (paramSet, 0, m_numMinuitParams - 1);
   }
}

/////////////
// Friends //
/////////////

ostream& operator<< (ostream& o_stream, const CLPTemplateFitter &rhs)
{
   for (int tempIndex = 0; tempIndex < rhs.m_numTemplates; ++tempIndex)
   {      
      o_stream << setw(2) << tempIndex << ") "
               << setw(10) << rhs.m_templateNameVec[tempIndex]
               << " " << setw (8) << rhs.m_normVec[tempIndex]
               << " : " << rhs.m_startStopStepVec.at(tempIndex);      
      if (rhs._paramIsFixed (tempIndex))
      {
         o_stream << "  Fixed" << endl;
      } else {
         if (rhs.m_constraintSigmaVec.at(tempIndex))
         {
            cout << Form ("  %5.2f +- %4.2f",
                          rhs.m_constraintMeanVec.at(tempIndex),
                          rhs.m_constraintSigmaVec.at(tempIndex))
                 << endl;
         } else {
            cout << "  Unconstrained" << endl;
         }
      }
   } // for tempIndex
   for (int bnIndex = 0; bnIndex < rhs.m_numBinNorms; ++bnIndex)
   {
      o_stream << setw(2) << bnIndex + rhs.m_numTemplates << ") "
               << "BN " << setw(7) << rhs.m_binNormNameVec[bnIndex]
               << " " << setw (8) << rhs.m_binNormVec[bnIndex].startingValue()
               << " : " << rhs.m_binNormVec.at(bnIndex).startStopStep();
      if (rhs._paramIsFixed (bnIndex + rhs.m_numTemplates))
      {
         o_stream << " Fixed" << endl;
      } else {
         if (rhs.m_binNormConstraintSigmaVec.at(bnIndex))
         {
            cout << Form ("  %5.2f +- %4.2f",
                          rhs.m_binNormConstraintMeanVec.at(bnIndex),
                          rhs.m_binNormConstraintSigmaVec.at(bnIndex))
                 << endl;
         } else {
            cout << "  Unconstrained" << endl;
         }
      }      
   } // for bnIndex
   return o_stream;
} 
