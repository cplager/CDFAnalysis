// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <numeric>

#include "TStyle.h"
#include "TGraphErrors.h"
#include "TError.h"
#include "TLine.h"
#include "TArrow.h"
#include "TF1.h"
#include "TApplication.h"

#include "JointPhysics/JPUtils.hh"
#include "JointPhysics/dout.hh"
#include "RootFileCollection/OptionUtils.h"

#include "FCPEs.hh"
#include "defineFunctions.hh"

#include "dumpSTL.icc"

using namespace std;

string   FCPEs::sm_treeName             = "PEtree";
string   FCPEs::sm_varName              = "measBF";
string   FCPEs::sm_lowerKey             = "";
string   FCPEs::sm_nominalSampleName    = "";
string   FCPEs::sm_upperKey             = "";
string   FCPEs::sm_sliceFitFuncName     = "splitGauss2";
string   FCPEs::sm_pbestFitFuncName     = "poly3";
string   FCPEs::sm_lratFitFuncName      = "splitGaussBump";
string   FCPEs::sm_lowerLratFitFuncName = "halfGauss2";
string   FCPEs::sm_upperLratFitFuncName = "upperHalfGauss2";
string   FCPEs::sm_upperBandFitFuncName = "poly4";
string   FCPEs::sm_accBandName          = "";
string   FCPEs::sm_observedName         = "observed";
string   FCPEs::sm_trueName             = "true name";
string   FCPEs::sm_fcBandName           = "FC Acceptance Bands";
string   FCPEs::sm_peNameForm           = "PE Dist %.2f";
string   FCPEs::sm_lrNameForm           = "Likelihood Ratio %.2f";
string   FCPEs::sm_filePrefix           = "";
double   FCPEs::sm_peValueFactor        =  1.;
double   FCPEs::sm_peHistMax            =  -1.;
double   FCPEs::sm_lowerBound           =  0.;
double   FCPEs::sm_upperBound           =  1.;
double   FCPEs::sm_lowerLikeliEdge      = -0.505;
double   FCPEs::sm_upperLikeliEdge      =  0.505;
double   FCPEs::sm_vertEdgeWidth        =  0.01;
double   FCPEs::sm_confidenceLevel      =  0.95;
double   FCPEs::sm_lowerEdgeValue       =  0.;
double   FCPEs::sm_upperEdgeValue       =  0.;
int      FCPEs::sm_numLikeliBins        = 101;
double   FCPEs::sm_lowerVertEdge        = -0.005;
double   FCPEs::sm_upperVertEdge        =  0.105;
int      FCPEs::sm_numVertSlices        = -1;
double   FCPEs::sm_upperExpectedValue   =  1.;
double   FCPEs::sm_lowerExpectedValue   =  0.;
double   FCPEs::sm_observedValue        =  0.;
int      FCPEs::sm_numExpectedBins      = 20;
TCanvas *FCPEs::sm_canvasPtr            =  0;
TFile   *FCPEs::sm_outputPtr            =  0;

// Stand alone fitting variables
string FCPEs::sm_SAFitFunc      = "";
string FCPEs::sm_SAFitParamName = "";
string FCPEs::sm_SAFitXValName  = "";
string FCPEs::sm_SAFitYValName  = "";
string FCPEs::sm_SAFitYErrName  = "";
double FCPEs::sm_SAFitYMax      = 45000.;
double FCPEs::sm_SAFitYMin      = 0.;
bool   FCPEs::sm_SAFitBatchMode = false;

// Control flags
bool FCPEs::sm_makeVerticalSlicesBool = false;
bool FCPEs::sm_fitVerticalSlicesBool  = false;
bool FCPEs::sm_fitPbestBool           = false;
bool FCPEs::sm_loadPEdistsBool        = false;
bool FCPEs::sm_plotPEdistsBool        = false;
bool FCPEs::sm_makeLikeliRatBool      = false;
bool FCPEs::sm_fitLikeliRatBool       = false;
bool FCPEs::sm_plotLikeliRatBool      = false;
bool FCPEs::sm_plotBandsBool          = false;
bool FCPEs::sm_plotObservedValueBool  = false;
bool FCPEs::sm_makeAccBandsBool       = false;
bool FCPEs::sm_forceLowerEdgeValue    = false;
bool FCPEs::sm_forceUpperEdgeValue    = false;
bool FCPEs::sm_fitUpperBandEdge       = false;
bool FCPEs::sm_plotExpectedLimit      = false;

// STL Objects
FCPEs::SFMap       FCPEs::sm_PEmap;
FCPEs::SDVecMap    FCPEs::sm_nameDVecMap;
FCPEs::SBoolMap    FCPEs::sm_nameDVecChangedMap;
FCPEs::STH1FPtrMap FCPEs::sm_vertSlicesHPtrMap;
FCPEs::STF1PtrMap  FCPEs::sm_tf1PtrMap;

int FCPEs::sm_dummyVariable = FCPEs::_staticInitialize();

FCPEs::FCPEs() :
   m_key (""), m_trueValue (-999),
   m_peWild (""), m_chainPtr (0)
{
}

FCPEs::FCPEs(const string &key, double trueValue,
             const string &peWild) :
   m_key (key), m_trueValue (trueValue),
   m_peWild (peWild), m_chainPtr (0)
{
}

int
FCPEs::loadChain()
{
   m_chainPtr = new TChain( sm_treeName.c_str() );
   return m_chainPtr->Add( m_peWild.c_str() );
}

/////////////////////////////
// Static Member Functions //
/////////////////////////////

bool 
FCPEs::loadPEinfo (const string& filename)
{
   ifstream source(filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "file " << filename << "could not be opened" << endl;
      return false;
   }
   string line;
   while (getline (source, line))
   {
      // find the first nonspace
      unsigned int where = line.find_first_not_of(" \t");
      if (string::npos == where)
      {
         // blank line
         continue;
      } 
      char first = line.at (where);
      if ('#' == first)
      {
         // comment - ignore
         continue;
      }
      if ('-' != first && '+' != first && '*' != first)
      {
         dout << "loadPEinfo() error: I don't understand line: "
              << line << endl
              << first << endl;
         exit (-1);
      }
      where =  line.find_first_not_of(" \t", where + 1);
      if (string::npos == where)
      {
         // no non-spaces
         continue;
      }
      // Get string starting at first nonspace after '-'.  Copy it to
      // another string without copying any spaces and stopping at the
      // first '#'.
      string withspaces = line.substr (where);
      // Are we loading PE info
      if ('-' == first)
      {
         JPUtils::SVec words;
         JPUtils::split (words, withspaces);
         if (words.size() < 3)
         {
            dout << "loadPEinfo() error: I don't understand line: "
                 << line << endl
                 << "Not enough words." << endl;
            exit (-1);
         } // if too small
         string key   = words.at(0);
         double value = atof( words.at(1).c_str() );
         string files = words.at(2);
         // make sure we don't already have this PE
         if (sm_PEmap.end() != sm_PEmap.find (key))
         {
            // Already here
            dout << "loadPEinfo() error: key '" << key << "' already exists."
                 << endl;
            exit (-1);
         }
         sm_PEmap[key] = FCPEs (key, value, files);
         continue;
      } // if -
      if ('*' == first)
      {
         JPUtils::SVec words;
         JPUtils::split (words, withspaces);
         int wordsize = (int) words.size();
         if (wordsize < 2)
         {
            dout << "loadPEinfo() error: I don't understand line: "
                 << line << endl
                 << "Not enough words." << endl;
            exit (-1);
         } // if too small
         DVec tempVec;
         for (int loop = 1; loop < wordsize; ++loop)
         {
            tempVec.push_back( atof( words.at(loop).c_str() ) );
         } // for loop
         sm_nameDVecMap[ words.at(0) ] = tempVec;
         continue;
      }
      if ('+' == first)
      {
         // we're seeting a variable
         string nospaces;
         for (int position = 0; 
              position < (int) withspaces.length(); 
              ++position)
         {
            char ch = withspaces[position];
            if ('#' == ch)
            {
               // start of a comment
               break;
            } else if (' ' == ch || '\t' == ch)
            {
               continue;
            }
            nospaces += ch;
         } // for position
         // set the variable from the string UNLESS this variable has
         // already been set (=> ', true')
         if (! OptionUtils::setVariableFromString (nospaces, true) )
         {
            dout << "Don't understand line" << endl << line << endl
                 << "in options file '" << filename << "'.  Aborting."
                 << endl;
            exit(0);
         } // if setting variable failed
         continue;
      } // if +
   } // while getline

   // for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
   //      sm_PEmap.end() != iter;
   //      ++iter)
   // {
   //    cout << iter->first << " : " << iter->second << endl;
   // }
   return true;
}

bool 
FCPEs::loadChains()
{
   for (FCPEs::SFMapIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      if (! iter->second.loadChain())
      {
         // Didn't load any files.  Not good.
         cerr << "FCPEs::loadChains() error: " << iter->first
              << " did not load any files." << endl;
         return false;
      }
   } // for iter
   return true;
}

bool
FCPEs::makeVerticalSlices()
{
   // Let's make some histograms
   gErrorIgnoreLevel = 9999; // No errors
   sm_vertSlicesHPtrMap.clear();
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      cout << "Drawing " << iter->first << endl;
      string histname = iter->second.key() +"_smallhist";
      TH1F *histPtr = sm_vertSlicesHPtrMap[iter->first] =
         new TH1F (histname.c_str(), histname.c_str(),
                   sm_numVertSlices, 
                   sm_lowerVertEdge,
                   sm_upperVertEdge);
      if (sm_outputPtr) histPtr->SetDirectory (sm_outputPtr);
      string drawCommand = Form ("%s>>%s", 
                                 sm_varName.c_str(),
                                 histname.c_str());
      iter->second.chainPtr()->Draw( drawCommand.c_str(), "", "goff" );
   } // for iter
   gErrorIgnoreLevel = 0; // Regular error reporting
   
   double step = (sm_upperVertEdge - sm_lowerVertEdge) / sm_numVertSlices;
   DVec bfVec, pbestVec;
   for (int loop = 1; loop <= sm_numVertSlices; ++loop)
   {
      double lower = sm_lowerVertEdge + (loop - 1) * step;
      double upper = lower + step;
      double center = (lower + upper) / 2.;
      bfVec.push_back (center);
      DVec xcoordVec, ycoordVec;
      for (STH1FPtrMapConstIter iter = sm_vertSlicesHPtrMap.begin();
           sm_vertSlicesHPtrMap.end() != iter;
           ++iter)
      {
         double xval = sm_PEmap[iter->first].trueValue();
         double yval = iter->second->GetBinContent (loop);
         cout << xval << " " << yval << endl;
         xcoordVec.push_back (xval);
         ycoordVec.push_back (yval);
      } // for iter
      string xname = "x_vert";
      string yname = Form ("y_vert_%04.1f", 100 * center);
      addNamedDVec (xname, xcoordVec);
      addNamedDVec (yname, ycoordVec);
   } // for loop
   return true;
}

bool
FCPEs::fitVerticalSlices()
{
   TF1 *funcPtr = sm_tf1PtrMap[sm_sliceFitFuncName];
   assert (funcPtr);

   double step = (sm_upperVertEdge - sm_lowerVertEdge) / sm_numVertSlices;
   DVec bfVec, pbestVec, pbestErrVec;
   for (int loop = 0; loop < sm_numVertSlices; ++loop)
   {
      double lower = sm_lowerVertEdge + loop * step;
      double upper = lower + step;
      double center = (lower + upper) / 2.;
      //funcPtr->SetParameter (1, center);
      string xname = "x_vert";
      string yname = Form ("y_vert_%04.1f", 100 * center);
      string funcname = Form ("func_vert_%04.1f", 100 * center);
      seedFit (funcPtr, funcname);
      const DVec &xcoordVec = sm_nameDVecMap[xname];
      const DVec &ycoordVec = sm_nameDVecMap[yname];
      assert (xcoordVec.size() != 0 && xcoordVec.size() == ycoordVec.size());

      // make TGraph
      TGraph *tgPtr = new TGraph ( (int) xcoordVec.size(),
                                   (&xcoordVec[0]), 
                                   (&ycoordVec[0]) );
      tgPtr->SetMarkerStyle(20);
      tgPtr->SetMarkerColor (kRed);
      gErrorIgnoreLevel = 0;
      tgPtr->Fit (funcPtr, "L");
      sm_canvasPtr =  new TCanvas( "c1", "Feldman-Cousins", 800, 600 );
      string name = Form("vert_%04.1f", center * 100);
      tgPtr->SetName ( name.c_str() );
      tgPtr->SetTitle( name.c_str() );
      tgPtr->Draw("AP");
      sm_canvasPtr->Print( Form("%svert_%04.1f.eps", sm_filePrefix.c_str(),
                                center * 100) );
      if (sm_outputPtr) sm_outputPtr->Append (tgPtr);
      delete sm_canvasPtr;

      const double *paramsPtr = funcPtr->GetParameters();
      const int numParam = funcPtr->GetNpar();
      DVec funcVec    (paramsPtr, paramsPtr + numParam);
      addNamedDVec (funcname, funcVec);

      double value = funcPtr->GetParameter (0);
      double error = funcPtr->GetParError  (0);
      // check to make sure this isn't one of our boundry conditions
      if ( (lower <= sm_lowerBound && upper > sm_lowerBound) ||
           (lower <= sm_upperBound && upper > sm_upperBound) )
      {
         // lower or upper bound
         string key = sm_lowerKey;
         // is it the upper key we should be using?
         if (lower >= sm_upperBound && upper < sm_upperBound)
         {
            key = sm_upperKey;
         }
         assert (key.length());
         value = *std::max_element (ycoordVec.begin(), ycoordVec.end());
         error = 0.; // we want to nail this point
      } // if lower or upper edge          
      bfVec.push_back       (center);
      pbestVec.push_back    (value);
      pbestErrVec.push_back (error);
   } // for loop

   addNamedDVec ("pbest",    pbestVec);
   addNamedDVec ("pbesterr", pbestErrVec);
   addNamedDVec ("bf",       bfVec);
   return true;
}

bool
FCPEs::fitPbest()
{
   TF1 *funcPtr = sm_tf1PtrMap[sm_pbestFitFuncName];
   assert (funcPtr);
   const DVec &pbestVec = sm_nameDVecMap["pbest"];
   const DVec &pbestErrVec = sm_nameDVecMap["pbesterr"];
   const DVec &bfVec = sm_nameDVecMap["bf"];
   seedFit (funcPtr, "func_fitPbest");
   assert (pbestVec.size() != 0 && pbestVec.size() == bfVec.size());
   TGraphErrors *tgPtr = new TGraphErrors ( (int) bfVec.size(),
                                                 (&bfVec[0]),
                                                 (&pbestVec[0]),
                                                 0, // no X errors
                                                 (&pbestErrVec[0]));

   tgPtr->SetMarkerStyle (20);
   tgPtr->SetMarkerColor (kRed);
   tgPtr->SetName ("pbest");
   tgPtr->SetTitle ("pbest");
   tgPtr->Fit (funcPtr, "W");
   tgPtr->Fit (funcPtr);
   sm_canvasPtr =  new TCanvas( "c1", "Feldman-Cousins", 800, 600 );
   double max = *std::max_element (pbestVec.begin(), pbestVec.end()) * 1.1;
   tgPtr->SetMaximum (max);
   tgPtr->SetMinimum (0);
   tgPtr->Draw("AP");
   sm_canvasPtr->Print( (sm_filePrefix + "pbest.eps").c_str());
   if (sm_outputPtr) sm_outputPtr->Append (tgPtr);
   const double *paramsPtr = funcPtr->GetParameters();
   const int numParam = funcPtr->GetNpar();
   DVec funcVec (paramsPtr, paramsPtr + numParam);
   addNamedDVec ("func_fitPbest", funcVec);
   return true;
}

bool
FCPEs::loadPEdists()
{
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      // check to see if we only want to do one PE distribution
      if (sm_accBandName.length() && iter->first != sm_accBandName)
      {
         // We only want to do one and this one isn't it.
         continue;
      }
      // int totalCount = (int) iter->second.chainPtr()->GetEntries();
      // assert (totalCount);
      cout << "Drawing " << iter->first << endl;
      string histname = iter->second.key() +"_hist";
      TH1F *histPtr = sm_vertSlicesHPtrMap[iter->first] =
         new TH1F (histname.c_str(), histname.c_str(),
                   sm_numLikeliBins, 
                   sm_lowerLikeliEdge,
                   sm_upperLikeliEdge);
      if (sm_outputPtr) histPtr->SetDirectory (sm_outputPtr);
      string drawCommand = Form ("%s>>%s", 
                                 sm_varName.c_str(),
                                 histname.c_str());
      iter->second.chainPtr()->Draw( drawCommand.c_str(), "", "goff" );
      if (sm_plotPEdistsBool)
      {
         gStyle->SetOptStat (0);
         TCanvas  c1 ( "c1", "Feldman-Cousins", 800, 600 );
         string title = Form (sm_peNameForm.c_str(),
                              sm_peValueFactor * iter->second.trueValue());
         histPtr->SetTitle( title.c_str() );
         histPtr->GetXaxis()->SetTitle( sm_observedName.c_str() );
         histPtr->GetYaxis()->SetNoExponent();
         if (sm_peHistMax > 0)
         {
            histPtr->SetMaximum( sm_peHistMax );
         }
         histPtr->Draw();
         c1.Print ( Form ("%sPE%s.eps", sm_filePrefix.c_str(),
                          iter->first.c_str() ) );
      }
      DVec binContVec;
      getBinContents (histPtr, binContVec);
      string histVecName = Form ("pe_%s", iter->first.c_str());
      addNamedDVec (histVecName, binContVec);      
   } // for iter
   return true;
}

bool
FCPEs::makeLikeliRats()
{
   // Get X binning
   DVec xvalVec;
   double step =     (sm_upperLikeliEdge - sm_lowerLikeliEdge) 
      / sm_numLikeliBins;
   double vertStep = (sm_upperVertEdge   - sm_lowerVertEdge)
      / sm_numVertSlices;
   double scale = step / vertStep;
   double start = sm_lowerLikeliEdge + step / 2;
   for (int loop = 0; loop < sm_numLikeliBins; ++loop)
   {
      xvalVec.push_back( start + loop * step );
   } // for loop
   addNamedDVec ("x_lhrat", xvalVec);

   // Do we have a lower physical bound?
   DVec lowerPEVec, upperPEVec;
   double lowerValue = 0, upperValue = 0;
   if (sm_lowerKey.size())
   {
      SFMapConstIter iter = sm_PEmap.find (sm_lowerKey);
      assert (sm_PEmap.end() != iter);
      if (sm_forceLowerEdgeValue) 
      {
         lowerValue = sm_lowerEdgeValue;
      } else {
         lowerValue = iter->second.trueValue();
      }
      string histVecName = Form ("pe_%s", iter->first.c_str());
      lowerPEVec = sm_nameDVecMap [histVecName];
      if (! lowerPEVec.size())
      {
         cerr << "Can not find " << histVecName << "." << endl;
         assert (lowerPEVec.size());
      }
   } // if lowerKey
   else
   {
      // make lower value outside the xrange
      lowerValue = sm_lowerLikeliEdge - 3 * step;
   }
   if (sm_upperKey.size())
   {
      SFMapConstIter iter = sm_PEmap.find (sm_upperKey);
      assert (sm_PEmap.end() != iter);
      if (sm_forceUpperEdgeValue) 
      {
         upperValue = sm_upperEdgeValue;
      } else {
         upperValue = iter->second.trueValue();
      }
      string histVecName = Form ("pe_%s", iter->first.c_str());
      upperPEVec = sm_nameDVecMap [histVecName];
      assert (upperPEVec.size());
   } // if upperKey
   else
   {
      // make upper value outside the xrange
      upperValue = sm_upperLikeliEdge + 3 * step;
   }
      

   // load physical region binning
   TF1 *funcPtr = sm_tf1PtrMap[sm_pbestFitFuncName];
   assert (funcPtr);
   if (! seedFit (funcPtr, "func_fitPbest"))
   {
      // we don't have the fit value
      cerr << "FCPEs::makeLikeliRats() Error: No fit found for Pbest.  "
           <<"Aborting." << endl;
      exit (-1);
   }
   
   // loop over all PEs
   double halfstep = step / 2;
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      string histVecName  = Form ("pe_%s", iter->first.c_str());
      const DVec &peVec = sm_nameDVecMap [histVecName];
      assert (peVec.size() == xvalVec.size());
      DVecConstIter peIter, xIter;
      DVec likeliratVec, likeliratErrVec;
      int size = (int) peVec.size();
      for (int loop = 0; loop < size; ++loop)
      {
         double numerator = peVec.at   (loop);
         double xval      = xvalVec.at (loop);
         double denominator = scale * funcPtr->Eval (xval);
         if (xval - halfstep < lowerValue)
         {
            denominator = lowerPEVec.at (loop);
         } else if (xval + halfstep > upperValue)
         {
            denominator = upperPEVec.at (loop);
         }
         if (! numerator) 
         {
            likeliratVec.push_back (0.);
            likeliratErrVec.push_back (0.);
            continue;
         }
         if (! denominator)
         {
            likeliratVec.push_back (1.);
            likeliratErrVec.push_back (1.);
            continue;
         }
         double likelirat = numerator / denominator;
         double relerror = sqrt (1. / numerator + 1. / denominator);
         likeliratVec.push_back (likelirat);
         likeliratErrVec.push_back (relerror * likelirat);         
      } // for loop
      string likeliVecName = Form ("likely_%s", iter->first.c_str());
      string likeliErrName = Form ("likelyerr_%s", iter->first.c_str());
      addNamedDVec (likeliVecName, likeliratVec);
      addNamedDVec (likeliErrName, likeliratErrVec);


   } // for iter
   return true;
}

bool
FCPEs::fitLikeliRats()
{
   const DVec &xvalVec = sm_nameDVecMap["x_lhrat"];
   assert (xvalVec.size());
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      double trueValue = iter->second.trueValue();
      string likeliVecName = Form ("likely_%s", iter->first.c_str());
      string likeliErrName = Form ("likelyerr_%s", iter->first.c_str());
      const DVec &likeliratVec    = sm_nameDVecMap[likeliVecName];
      const DVec &likeliratErrVec = sm_nameDVecMap[likeliErrName];
      assert (xvalVec.size() == likeliratVec.size());
      assert (xvalVec.size() == likeliratErrVec.size());
      
      // Make pretty TGraphs
      TCanvas  c1 ( "c1", "Feldman-Cousins", 800, 600 );
      TGraphErrors *tgPtr = new TGraphErrors ( (int) xvalVec.size(),
                                               (&xvalVec[0]),
                                               (&likeliratVec[0]),
                                               0, // no X errors
                                               (&likeliratErrVec[0]));
      tgPtr->SetName (  Form("likelirat_%05.2f", trueValue * 100));
      tgPtr->SetTitle(  Form("likelirat_%05.2f", trueValue * 100));
      tgPtr->SetMarkerStyle(20);
      tgPtr->SetMarkerColor (kRed);
      tgPtr->SetLineColor (kRed);
      tgPtr->SetMaximum ( 1.1);
      tgPtr->SetMinimum (-0.1);
      // get fit function
      TF1 *funcPtr = sm_tf1PtrMap[likeliFitFuncName (iter->first)];
      dout << likeliFitFuncName (iter->first) << endl;
      assert (funcPtr);
      string fitname = Form ("func_lh_%s", iter->first.c_str());
      if (seedFit (funcPtr, fitname))
      {
         dout << "fit successfully seeded." << endl;
         const double *paramsPtr = funcPtr->GetParameters();
         const int numParam = funcPtr->GetNpar();
         for (int loop = 0; loop < numParam; ++loop)
         {
            cout << loop << ") " << paramsPtr[loop] << endl;
         } 
      } else {
         dout << "No seed available." << endl;
      }
      // fit
      tgPtr->Fit (funcPtr, "W");
      tgPtr->Fit (funcPtr);
      const double *paramsPtr = funcPtr->GetParameters();
      const int numParam = funcPtr->GetNpar();
      DVec funcVec (paramsPtr, paramsPtr + numParam);
      addNamedDVec (fitname, funcVec);
      // Draw
      tgPtr->Draw("AP");
      c1.Print (Form("%slikelirat_%s.eps", 
                     sm_filePrefix.c_str(), iter->first.c_str()));
      if (sm_outputPtr) sm_outputPtr->Append (tgPtr);
   } // for iter
   return true;
}

bool
FCPEs::plotLikeliRats()
{
   const bool kFixEverything = true;
   const DVec &xvalVec = sm_nameDVecMap["x_lhrat"];
   assert (xvalVec.size());
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      double trueValue = iter->second.trueValue();
      string likeliVecName = Form ("likely_%s", iter->first.c_str());
      string likeliErrName = Form ("likelyerr_%s", iter->first.c_str());
      const DVec &likeliratVec    = sm_nameDVecMap[likeliVecName];
      const DVec &likeliratErrVec = sm_nameDVecMap[likeliErrName];
      assert (xvalVec.size() == likeliratVec.size());
      assert (xvalVec.size() == likeliratErrVec.size());
      
      // Make pretty TGraphs
      TCanvas  c1 ( "c1", "Feldman-Cousins", 800, 600 );
      TGraphErrors *tgPtr = new TGraphErrors ( (int) xvalVec.size(),
                                               (&xvalVec[0]),
                                               (&likeliratVec[0]),
                                               0, // no X errors
                                               (&likeliratErrVec[0]));
      tgPtr->SetName (  Form("likelirat_%05.2f", trueValue * 100));
      string title = Form (sm_lrNameForm.c_str(),
                           sm_peValueFactor * iter->second.trueValue());
      tgPtr->SetTitle( title.c_str() );
      tgPtr->SetMarkerStyle(20);
      tgPtr->SetMarkerColor (kRed);
      tgPtr->SetLineColor (kRed);
      tgPtr->SetMaximum ( 1.1);
      tgPtr->SetMinimum (-0.1);
      tgPtr->GetXaxis()->SetTitle( sm_observedName.c_str() );
      // get fit function
      TF1 *funcPtr = sm_tf1PtrMap[likeliFitFuncName (iter->first)];
      assert (funcPtr);
      string fitname = Form ("func_lh_%s", iter->first.c_str());
      if (! seedFit (funcPtr, fitname, kFixEverything))
      {
         cerr << "FCPEs::plotLikeliRats() Warning: Could not seed function for "
              << fitname << ".  Skipping." << endl;
         continue;
      }
      // fit
      tgPtr->Fit (funcPtr);
      // Draw
      tgPtr->Draw("AP");
      // get FC bands info
      string bandname = Form ("accband_%s", iter->first.c_str());
      const DVec &bandVec = sm_nameDVecMap[bandname];
      int bandsize = (int) bandVec.size();
      if (bandsize < 3)
      {
         cerr << "FCPEs::plotLikeliRats() Warning: Could not find info for "
              << bandname << ".  Skipping." << endl;
         continue;
      }
      double yval = bandVec.at(0);
      TLine *horz = new TLine (sm_lowerLikeliEdge, yval,
                               sm_upperLikeliEdge, yval);
      horz->SetLineColor (8);
      horz->Draw("same");
      if (sm_outputPtr) sm_outputPtr->Append (tgPtr);
      for (int index = 1; index + 1 < bandsize; index += 2)
      {
         TLine *acc = new TLine (bandVec.at (index    ), yval,
                                 bandVec.at (index + 1), yval);
         acc->SetLineColor (kBlue);
         acc->SetLineWidth (2);
         acc->Draw("same");
      } // for index
      c1.Print (Form("%slikelirat_line_%s.eps", 
                     sm_filePrefix.c_str(),
                     iter->first.c_str()));
   } // for iter
   return true;
}

bool
FCPEs::plotBands()
{
   // get the max and min of the Y axis.
   double min = 0.;
   double max = 0;
   bool firstIteration = true;
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      if (firstIteration)
      {
         firstIteration = false;
         max = min = iter->second.trueValue();
      } else if (max < iter->second.trueValue())
      {
         max = iter->second.trueValue();
      } else if (min > iter->second.trueValue())
      {
         min = iter->second.trueValue();
      }
   } // for iter
   double diff = max - min;
   double prettyMax = min + 1.1 * diff;
   double prettyMin = max - 1.1 * diff;
   TCanvas  c1 ( "c1", "Feldman-Cousins", 800, 600 );
   gStyle->SetOptStat(0);
   TGraph emptyGraph (1);
   TAxis *axisPtr = emptyGraph.GetXaxis();
   axisPtr->Set (sm_numLikeliBins, sm_lowerLikeliEdge, sm_upperLikeliEdge);
   emptyGraph.SetMinimum (prettyMin);
   emptyGraph.SetMaximum (prettyMax);
   emptyGraph.GetXaxis()->SetTitle( sm_observedName.c_str() );
   emptyGraph.GetYaxis()->SetTitle( sm_trueName.c_str() );
   emptyGraph.SetTitle (sm_fcBandName.c_str());
   emptyGraph.Draw("AP");
   // TH1F emptyHist ("empty", sm_fcBandName.c_str(), 
   //                 sm_numLikeliBins, sm_lowerLikeliEdge, sm_upperLikeliEdge);
   // emptyHist.SetMinimum (prettyMin);
   // emptyHist.SetMaximum (prettyMax);
   // emptyHist.GetXaxis()->SetTitle( sm_observedName.c_str() );
   // emptyHist.Draw();
   vector< TLine* > tlinePtrVec;
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      string bandname = Form ("accband_%s", iter->first.c_str());
      const DVec &bandVec = sm_nameDVecMap[bandname];
      int bandsize = (int) bandVec.size();
      if (bandsize < 3)
      {
         continue;
      }
      for (int loop = 1; loop < bandsize; loop += 2)
      {
         double lower = bandVec.at (loop);
         double upper = bandVec.at (loop + 1);
         
         TLine *linePtr = new TLine (lower, iter->second.trueValue(),
                                     upper, iter->second.trueValue());
         tlinePtrVec.push_back (linePtr);
         linePtr->SetLineWidth (2);
         linePtr->SetLineColor (kBlue);
         linePtr->Draw();
      } // for loop
   } // for iter
   string filename = sm_filePrefix + "fcbands.eps";
   cout << "Saving FC Bands eps file: " << filename << endl;
   c1.Print ( filename.c_str() );
   if (sm_plotObservedValueBool)
   {
      TArrow *observedLine = new TArrow (sm_observedValue, prettyMin,
                                         sm_observedValue, prettyMax,
                                         20, "-<-");
      observedLine->SetLineColor (8);
      observedLine->SetLineWidth (2);
      observedLine->Draw ("same");
      tlinePtrVec.push_back( (TLine*) observedLine );
      filename = sm_filePrefix + "fcbands_observed.eps";
      cout << "Saving FC Bands eps file: " << filename << endl;
      c1.Print ( filename.c_str() );
   }
   return true;
}

bool
FCPEs::findAcceptanceBands()
{
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      // check to see if we only want to do one acceptance band
      if (sm_accBandName.length() && iter->first != sm_accBandName)
      {
         // We only want to do one and this one isn't it.
         continue;
      }
      int totalCount = (int) iter->second.chainPtr()->GetEntries();
      assert (totalCount);
      TF1 *funcPtr = sm_tf1PtrMap[likeliFitFuncName (iter->first)];
      assert (funcPtr);
      string fitname = Form ("func_lh_%s", iter->first.c_str());
      if ( ! seedFit (funcPtr, fitname))
      {
         cout << "Couldn't find likelihood ratio fit for "
              << iter->first << ".  Skipping." << endl;
         return false;
      }
      TCanvas c1;
      funcPtr->SetRange (sm_lowerLikeliEdge, sm_upperLikeliEdge);
      // funcPtr->Draw();
      // c1.Print( (sm_filePrefix + "likerat.eps").c_str() );
      double stepsize = 0.0001;
      double upper = 1.0;
      double lower = 0.0;
      double coverage = 0.;
      double left = sm_lowerLikeliEdge - 1;
      double right = sm_lowerLikeliEdge + 1; // something bigger
      DVec edgesVec;
      cout << "key " << iter->first << endl;
      while ( ( upper - lower ) > stepsize || 
              ( ( upper - lower ) < stepsize && 
                coverage < sm_confidenceLevel ) ) 
      
      {         
         coverage = 0;
         double bar = 0.5 * ( upper + lower );
         edgesVec.clear();
         edgesVec.push_back (bar);
         double isLarger = false;
         left = sm_lowerLikeliEdge - stepsize;
         for (double xval = sm_lowerLikeliEdge - stepsize; 
              xval <= sm_upperLikeliEdge; 
              xval += stepsize ) 
         {
            double likeliRat = funcPtr->Eval( xval );
            // Are we crossing the bar going up?
            if (! isLarger && (likeliRat > bar) )
            {
               left = xval;
               right = xval - 1; // something smaller
               isLarger = true;
               continue;
            }
            // Are we crossing the bar going down?
            if (isLarger && likeliRat < bar)
            {
               right = xval;
               isLarger = false;
               string cutString;
               if (left < sm_lowerLikeliEdge)
               {
                  edgesVec.push_back (sm_lowerLikeliEdge);
                  edgesVec.push_back (right);
                  cutString = Form ("%s<=%f", 
                                    sm_varName.c_str(), right);
               } else {
                  edgesVec.push_back (left);
                  edgesVec.push_back (right);
                  cutString = Form ("(%s>=%f && %s<=%f)", 
                                    sm_varName.c_str(), left,
                                    sm_varName.c_str(), right);
               }
               dout << cutString << endl;
               double count = 
                  iter->second.chainPtr()->Draw ("1", 
                                                 cutString.c_str(), 
                                                 "goff");
               coverage += count / totalCount;
            }
         } // for xval
         // Do we need to finish off an interval?
         if (left > right)
         {
            // yup
            right = sm_upperLikeliEdge;
            edgesVec.push_back (left);
            edgesVec.push_back (right);
            isLarger = false;
            string cutString = 
               Form ("%s>=%f", 
                     sm_varName.c_str(), left);
            dout << cutString << endl;
            double count = 
               iter->second.chainPtr()->Draw ("1", 
                                              cutString.c_str(), 
                                              "goff");
            coverage += count / totalCount;
         } // if need to finish
         cout << "Interval for bar = " << bar << " : ";
         dumpSTL (edgesVec);
         cout << " : " << coverage << endl;
         // Check what we did and try again
         if (coverage > sm_confidenceLevel)
         {
            // bar is too low
            lower = bar;
         } else {
            // bar is too high
            upper = bar;
         }
      } // while
      cout << "Done" << endl << endl;
      addNamedDVec ( Form ("accband_%s", iter->first.c_str()),
                     edgesVec );
   }  // for iter   
   return true;
}

bool
FCPEs::fitUpperBandEdge()
{
   // first check to see if the bands are tilted to the right or to
   // the left.
   FCPEs::SFMapConstIter firstIter = sm_PEmap.begin();
   FCPEs::SFMapConstIter lastIter  = sm_PEmap.end(); --lastIter;
   string firstBand =  Form ("accband_%s", firstIter->first.c_str());
   string lastBand  =  Form ("accband_%s", lastIter->first.c_str());
   const DVec &firstVec = sm_nameDVecMap[firstBand];
   const DVec &lastVec  = sm_nameDVecMap[lastBand];
   if ( firstVec.size() < 3 ||  lastVec.size() < 3)
   {
      cerr << "Couldn't find '" << firstBand << "' or '"
           << lastBand << "'.  Aborting." << endl;
      return false;
   }

   bool normalBands = true;
   // if the last band is smaller than the first, then we do NOT have
   // normal bands.  Rembember that the 0th entry is the height of the
   // band.  It is the first entry that is the left edge of the band.
   if (lastVec.at(1) < firstVec.at(1))
   {
      cout << "Backwards bands." << endl;
      normalBands = false;
   }

   bool starting = true;
   DVec xvalVec, yvalVec;
   double lastValue = 0.;
   double min = 0, max = 0;
   for (FCPEs::SFMapConstIter iter = sm_PEmap.begin();
        sm_PEmap.end() != iter;
        ++iter)
   {
      string bandname = Form ("accband_%s", iter->first.c_str());
      const DVec &bandVec = sm_nameDVecMap[bandname];
      int bandsize = (int) bandVec.size();
      if (bandsize < 3)
      {
         cerr << "FCPEs::fitUpperBandEdge() Warning: Could not find info for "
              << bandname << ".  Skipping." << endl;
         continue;
      }
      double currentValue;
      if (normalBands)
      {
         currentValue = bandVec.at (1);
      } else {
         currentValue = bandVec.at( bandsize - 1 );
      }
      // is this the first band
      if (sm_PEmap.begin() == iter)
      {
         // o.k. Just set lastValue and get out
         lastValue = currentValue;
         xvalVec.push_back (currentValue);
         yvalVec.push_back (iter->second.trueValue());
         min = max = iter->second.trueValue();
         continue;
      }
      if (iter->second.trueValue() > max)
      {
         max = iter->second.trueValue();         
      }
      if (iter->second.trueValue() < min)
      {
         min = iter->second.trueValue();         
      }
      // is this the same value as the last band and are we near the
      // beginning?
      double epsilon = 2 * (currentValue - lastValue) 
         / (currentValue + lastValue);
      if ( fabs(epsilon) < 1e-6 )
      {
         if (starting)
         {
            // update the y value but leave the x value alone
            unsigned int position = yvalVec.size() - 1;
            yvalVec.at (position) = iter->second.trueValue();           
            continue;
         } else {
            // we must be at the end
            break;
         }        
      } // same value
      else {
         starting = false;
      }
      xvalVec.push_back (currentValue);
      yvalVec.push_back (iter->second.trueValue());
   } // for iter
   assert (xvalVec.size() == yvalVec.size());
   // cout << "xval "; dumpSTL (xvalVec); cout << endl;
   // cout << "yval "; dumpSTL (yvalVec); cout << endl;
   TGraph edge ( (int) xvalVec.size(),
                 (double*) &(xvalVec.at(0)),
                 (double*) &(yvalVec.at(0)));
   edge.SetTitle ((sm_trueName + " Upper Limit").c_str());
   edge.SetMarkerStyle (20);
   edge.SetMarkerSize (1);
   edge.SetMarkerColor (kRed);
   TAxis *axisPtr = edge.GetXaxis();
   axisPtr->Set (sm_numLikeliBins, sm_lowerLikeliEdge, sm_upperLikeliEdge);
   axisPtr->SetTitle (sm_observedName.c_str());
   TCanvas  c1 ( "c1", "Feldman-Cousins", 800, 600 );
   gStyle->SetOptStat(0);
   TF1 *funcPtr = sm_tf1PtrMap [sm_upperBandFitFuncName];
   assert (funcPtr);
   string funcname = "func_upperEdge";
   seedFit (funcPtr, funcname);
   edge.Fit (funcPtr, "", "",  sm_lowerLikeliEdge, sm_upperLikeliEdge);
   double funcMax = 
      funcPtr->GetMaximum (sm_lowerLikeliEdge, sm_upperLikeliEdge);
   double funcMin = 
      funcPtr->GetMinimum (sm_lowerLikeliEdge, sm_upperLikeliEdge);
   if (funcMax > max)
   {
      edge.SetMaximum (funcMax);
   }
   if (funcMin < min)
   {
      edge.SetMinimum (funcMin);
   }
   edge.Draw ("AP");
   c1.Print ( (sm_filePrefix + "upperEdge.eps").c_str() );
   const double *paramsPtr = funcPtr->GetParameters();
   const int numParam = funcPtr->GetNpar();
   DVec funcVec    (paramsPtr, paramsPtr + numParam);
   addNamedDVec (funcname, funcVec);
   return true;
}

bool
FCPEs::plotExpectedLimit()
{
   TF1 *funcPtr = sm_tf1PtrMap [sm_upperBandFitFuncName];
   assert (funcPtr);
   string funcname = "func_upperEdge";
   if (!seedFit (funcPtr, funcname))
   {
      cerr << "'" << funcname << "' seeding failed.  Aborting."
           << endl;
      return false;
   }
   TH1F expectedLimit ("expected", (sm_trueName + " Expected Limit").c_str(),
                       sm_numExpectedBins,
                       sm_lowerExpectedValue,
                       sm_upperExpectedValue);
   const DVec &xvalVec = sm_nameDVecMap ["x_lhrat"];
   const DVec &yvalVec = sm_nameDVecMap ["pe_" + sm_nominalSampleName];
   if (! xvalVec.size() || xvalVec.size() != yvalVec.size() )
   {
      cerr << "Couldn't find binning or PEs requested.  Aborting."
           << endl;
      return false;
   }
   // int numPEs = (int) accumulate (yvalVec.begin(), yvalVec.end(), 0.);
   // dout << numPEs << endl;
   FCPEs::SFMapConstIter iter = sm_PEmap.find (sm_nominalSampleName);
   if (sm_PEmap.end() == iter)
   {
      cerr << "Couldn't find '" << sm_nominalSampleName << "'.  Aborting."
           << endl;
      return false;
   }
   double numEntries = iter->second.chainPtr()->GetEntries();
   int size = (int) xvalVec.size();
   double sumWeight = 0, prevSumWeight = 0;
   double prevLimit = 0;
   double prevQuantLimit = 0.;
   DVec quantileVec;
   quantileVec.push_back (0.1586552539315); // -1 sigma
                                            // = (1 - erf (1 / sqrt(2))) / 2
   quantileVec.push_back (0.50);            // median
   quantileVec.push_back (0.8413447460685); // +1 sigma
   DVecConstIter qvIter = quantileVec.begin();
   double qvValue = *qvIter;
   for (int loop = 0; loop < size; ++loop)
   {
      double limit = funcPtr->Eval( xvalVec.at(loop));
      double weight = yvalVec.at (loop) / numEntries;
      expectedLimit.Fill (limit, weight);
      sumWeight += weight;
      // did our sumWeight just pass a boundry?
      if (sumWeight > qvValue && qvValue >= prevSumWeight)
      {
         // O.k. Alice.  Hold on to your white rabbit
         double weight = (qvValue - prevSumWeight) / 
            (sumWeight - prevSumWeight);
         double quantLimit = weight * limit + (1 - weight) * prevLimit;
         if (quantileVec.begin() != qvIter)
         {
            cout << Form ("Quantile %.2f%% : %.4f : %.4f", 
                          100 * qvValue, quantLimit, 
                          fabs (quantLimit - prevQuantLimit))
                 << endl;
         } else {
            cout << Form ("Quantile %.2f%% : %.4f", 
                          100 * qvValue, quantLimit)
                 << endl;
         }
         prevQuantLimit = quantLimit;
         if (quantileVec.end() != qvIter)
         {
            ++qvIter;
            qvValue = *qvIter;
         }
      } // one of the quantiles
      // set all variables for the next iteration
      prevLimit = limit;
      prevSumWeight = sumWeight;
   } // for loop
   TCanvas  c1 ( "c1", "Feldman-Cousins", 800, 600 );
   gStyle->SetOptStat(0);
   expectedLimit.GetXaxis()->SetTitle( sm_trueName.c_str() );
   expectedLimit.Draw();
   c1.Print ( (sm_filePrefix + "expectedLimit.eps").c_str() );
   return true;
}

void
FCPEs::initialize (const string &controlName)
{
   defineFunctions();
   sm_outputPtr = TFile::Open ( (sm_filePrefix + "output.root").c_str(), 
                                "recreate");
   loadPEinfo (controlName);
   _correctStrings();
   if (sm_numVertSlices < 0)
   {
      sm_numVertSlices = (int) ( ( (sm_upperVertEdge - sm_lowerVertEdge) /
                                   sm_vertEdgeWidth) + 0.5 );
      // cout << "Setting sm_numVertSlices to " 
      //      << sm_numVertSlices << endl;                                 
   } // if calculating sm_numVertSlices
}

void
FCPEs::terminate()
{
   if (sm_outputPtr) 
   {
      sm_outputPtr->Write();
      sm_outputPtr->Close();
   }
   cout << setprecision (8);
   cout << endl << endl;
   for (SDVecMapConstIter iter = sm_nameDVecMap.begin();
        sm_nameDVecMap.end() != iter;
        ++iter)
   {
      if (! sm_nameDVecChangedMap[ iter->first ])
      {
         continue;
      }
      cout << "* " << iter->first << "  ";
      dumpSTL (iter->second);
      cout << endl;
   } // for iter
   cout << endl << endl;
}

void
FCPEs::addNamedDVec (const string &name, const DVec &vec)
{
   sm_nameDVecMap[name] = vec;
   sm_nameDVecChangedMap[name] = true;
}

TF1*
FCPEs::addFittingFunction (const char* name, TF1FuncPtrType fcn, 
                           Double_t xmin, Double_t xmax, 
                           Int_t npar)
{
   return addFittingFunction (new TF1 (name, fcn, xmin, xmax, npar));
}

TF1*
FCPEs::addFittingFunction (TF1* tf1Ptr)
{
   string name = tf1Ptr->GetName();
   // make sure we don't already have it
   // ... (to fill in) ...
   sm_tf1PtrMap[name] = tf1Ptr;
   return tf1Ptr;
}

bool
FCPEs::seedFit (const string &funcName,
                const string &vecName)
{
   // Need to implement (you lazy ....)
   return false;
}
   

bool
FCPEs::seedFit (TF1* funcPtr,
                const string &vecName,
                bool fixEverything)
{
   SDVecMapIter iter = sm_nameDVecMap.find (vecName);
   if ( sm_nameDVecMap.end() == iter)
   {
      // didn't find it
      return false;
   }
   // Make sure this vector has enough parameters.  
   const int numParam = funcPtr->GetNpar();
   if (numParam > (int) iter->second.size())
   {
      // It doesn't, so add '0's.  While we're at it, we should let
      // people know...
      cerr << "FCPEs::seedFit() warning: Function '" 
           << funcPtr->GetName() << "' has " << numParam
           << " parameters while '" << vecName << "' only has "
           << iter->second.size() << ".  '0's will be added to make the "
           << " lengths agree." << endl;
      while (numParam > (int) iter->second.size())
      {
         iter->second.push_back(0);
      } // while not long enoguh
   } // if numParam > size
   if (fixEverything)
   {
      const DVec &paramVec = iter->second;
      int size = (int) paramVec.size();
      for (int loop = 0; loop < size; ++loop)
      {
         funcPtr->FixParameter (loop, paramVec.at(loop));
      } // for loop
   } else {
      funcPtr->SetParameters ( & (iter->second[0]) );
   }
   return true;
}
   
void 
FCPEs::getBinContents (const TH1F* histPtr,
                       DVec &binContVec)
{
   binContVec.clear();
   int numBins = histPtr->GetNbinsX();
   for (int loop = 1; loop <= numBins; ++loop)
   {
      binContVec.push_back( histPtr->GetBinContent(loop) );
   } // for loop
}

string 
FCPEs::likeliFitFuncName(const std::string &key)
{
   string fitfunc = sm_lratFitFuncName;
   if (sm_lowerKey == key && sm_lowerLratFitFuncName.length())
   {
      fitfunc = sm_lowerLratFitFuncName;
   } 
   if (sm_upperKey == key && sm_upperLratFitFuncName.length())
   {
      fitfunc = sm_upperLratFitFuncName;
   }
   return fitfunc;
}

bool
FCPEs::StandAloneFitDistribution()
{
   TApplication *emptyAppPtr = 0;
   if (! sm_SAFitBatchMode)
   {
      emptyAppPtr = new TApplication ("emptyApp", 0, (char **) 0);
      gROOT->SetBatch (kFALSE);
   }
   TF1 *funcPtr = sm_tf1PtrMap [sm_SAFitFunc];
   if (! funcPtr)
   {
      dout << endl;
      return false;
   }
   seedFit (funcPtr, sm_SAFitParamName);
   const DVec &xValueVec = sm_nameDVecMap [sm_SAFitXValName];
   const DVec &yValueVec = sm_nameDVecMap [sm_SAFitYValName];
   const DVec &yErrorVec = sm_nameDVecMap [sm_SAFitYErrName];
   if (! yValueVec.size())
   {
      dout << endl;
      return false;
   }
   if (xValueVec.size() != yValueVec.size())
   {
      dout << endl;
      return false;
   }
   const double *errPtr = 0;
   if (yErrorVec.size())
   {
      errPtr = &(yErrorVec.at(0));
   } else {
      cout << "No errors" << endl;
   }
   TGraphErrors *tgPtr = new TGraphErrors( (int) xValueVec.size(),
                                           &xValueVec[0],
                                           &yValueVec[0],
                                           0, // no X errors
                                           errPtr);
   tgPtr->SetMarkerStyle (20);
   tgPtr->SetMarkerColor (kRed);
   TCanvas  c1 ( "c1", "Feldman-Cousins", 800, 600 );
   DVecVec paramVecVec;
   do 
   {
      cout << "command> ";
      string line;
      getline (cin, line);
      JPUtils::SVec words;
      JPUtils::split (words,  line);
      int size = (int) words.size();
      if (! size)
      {
         continue;
      }
      string command = words[0];
      OptionUtils::lowercaseString (command);
      if (! command.size())
      {
         continue;
      }
      // exit
      if ('e' == command[0] || 'q' == command[0] || 
          ".q" == command.substr(0, 2))
      {
         break;
      }
      // fixing parameters
      if ("st" == command.substr (0, 2))
      {
         const double *paramsPtr = funcPtr->GetParameters();
         const int numParam = funcPtr->GetNpar();
         paramVecVec.push_back( DVec (paramsPtr, paramsPtr + numParam) );
         cout << "Storing parameters in position " 
              << paramVecVec.size() << endl;
         continue;
      }
      if ("ret" == command.substr (0, 3))
      {
         int wordsize = (int) words.size();
         if (wordsize < 3)
         {
            // don't bother
            continue;
         }
         int which = atoi( words.at(1).c_str() );
         if (which <= 0 || which > (int) paramVecVec.size())
         {
            cerr << "which '" << which << "' out of range." << endl;
            continue;
         }
         const DVec &paramVec = paramVecVec.at (which - 1);
         cout << "Retreiving parameters ";
         for (int loop = 2; loop < wordsize; ++loop)
         {
            int paramIndex = atoi( words.at(loop).c_str() );
            if (paramIndex < 1 || paramIndex > (int) paramVec.size())
            {
               continue;
            }
            cout << "[" << paramIndex << ", " << paramVec.at( paramIndex - 1)
                 << "] ";
            funcPtr->SetParameter (paramIndex - 1, 
                                   paramVec.at( paramIndex - 1));
         } // for loop
         cout << "from position " 
              << which << endl;
         continue;
      }
      if ('i' == command[0])
      {
         cout << "fixing parameters";
         for (int loop = 1; loop + 1 < size; loop += 2)
         {
            int    num = atoi( words.at(loop).c_str() );
            double val = atof( words.at(loop + 1).c_str() );
            cout << " " << num << " to " << val << ", "<< endl;
            funcPtr->FixParameter (num - 1, val);
         } // for loop
         cout << endl;
         continue;
      }
      // release
      if ('r' == command[0])
      {
         cout << "releasing parameters";
         for (int loop = 1; loop < size; ++loop)
         {
            int  num = atoi( words.at(loop).c_str() );
            cout << " " << num; 
            funcPtr->ReleaseParameter (num - 1);
         } // for loop
         cout << endl;
         continue;
      }
      // set
      if ('s' == command[0])
      {
         cout << "setting parameters";
         for (int loop = 1; loop + 1 < size; loop += 2)
         {
            int    num = atoi( words.at(loop).c_str() );
            double val = atof( words.at(loop + 1).c_str() );
            cout << " " << num << " to " << val << ", "<< endl;
            funcPtr->SetParameter (num - 1, val);
         } // for loop
         cout << endl;
         continue;         
      }
      // fit
      if ('f' == command[0])
      {
         // fit
         if (size >= 4)
         {
            string option = words.at(1);            
            double min = atof( words.at(2).c_str() );
            double max = atof( words.at(3).c_str() );
            if ("." == option)
            {
               // we use '.' instead of blank
               option = "";
            }
            tgPtr->Fit (funcPtr, option.c_str(), "", min, max);
         } else if (size >=2)
         {
            string option = words.at(1);
            tgPtr->Fit (funcPtr, option.c_str());
         } else {
            tgPtr->Fit (funcPtr);
         }
         const double *paramsPtr = funcPtr->GetParameters();
         const double *errorsPtr = funcPtr->GetParErrors();
         const int numParam = funcPtr->GetNpar();
         DVec funcVec (paramsPtr, paramsPtr + numParam);
         DVec errVec (errorsPtr, errorsPtr + numParam);
         tgPtr->SetMaximum (sm_SAFitYMax);
         tgPtr->SetMinimum (sm_SAFitYMin);
         tgPtr->Draw("AP");
         c1.Print ( (sm_filePrefix + "fit.eps").c_str());
         cout << setprecision (8);
         cout << "* " << sm_SAFitParamName << " ";
         dumpSTL (funcVec);
         cout << endl;
         cout << "* " << sm_SAFitParamName << "err ";
         dumpSTL (errVec);
         cout << endl;
         continue;
      }
      // help
      if ('h' == command[0])
      {
         cout << "(E)xit/(Q)uit" << endl;
         cout << "(F)it [option] [lower upper]" << endl;
         cout << "f(I)x par1 val1 [par2 val2 ...]" << endl;
         cout << "(H)elp" << endl;
         cout << "(R)elease par1 [par2 ...]" << endl;
         cout << "(S)et par1 val1 [par2 val2 ...]" << endl;
         continue;
      }
      cout << "I don't understand: '" << command 
           << ".'  Type 'Help' for help."<< endl;
   } while (1);
   return true;
}

int
FCPEs::replaceAlphaWithBeta (string &original,
                             const string &alpha,
                             const string &beta)
{
   const int kAlphaLength = alpha.length();
   string::size_type index = original.find (alpha);
   int count = 0;
   while (string::npos != index)
   {
      original.replace (index, kAlphaLength, beta);
      index = original.find (alpha);
      ++count;
   } // while 
   return count;
}

/////////////////////////////////////
// Private Static Member Functions //
/////////////////////////////////////

int
FCPEs::_staticInitialize()
{
   using namespace OptionUtils;
   addOptionKey ("treename",      sm_treeName,
                 "Tree name");
   addOptionKey ("varname",       sm_varName,
                 "name of variable containing result of PE");
   addOptionKey ("upperkey",      sm_upperKey,
                 "name of key representing upper edge of "
                 "physically allowed values");
   addOptionKey ("nominalSample", sm_nominalSampleName);
   addOptionKey ("lowerkey",      sm_lowerKey,
                 "name of key representing lower edge of "
                 "physically allowed values");
   addOptionKey ("conflevel",  sm_confidenceLevel,
                 "desired C.L.");
   addOptionKey ("sliceFitFunc",       sm_sliceFitFuncName);
   addOptionKey ("pbestFitFunc",       sm_pbestFitFuncName);
   addOptionKey ("lowerLikeliEdge",    sm_lowerLikeliEdge);
   addOptionKey ("upperLikeliEdge",    sm_upperLikeliEdge);
   addOptionKey ("numLikeliBins",      sm_numLikeliBins);
   addOptionKey ("lowerVertEdge",      sm_lowerVertEdge);
   addOptionKey ("upperVertEdge",      sm_upperVertEdge);
   addOptionKey ("numVertSlices",      sm_numVertSlices);
   addOptionKey ("vertEdgeWidth",      sm_vertEdgeWidth,
                 "Used to calculate numVertSlices if numVertSlices < 0");
   addOptionKey ("lowerEdgeValue",     sm_lowerEdgeValue,
                 "Use with 'forceLowerEdge'");
   addOptionKey ("upperEdgeValue",     sm_upperEdgeValue,
                 "Use with 'forceUpperEdge'");
   addOptionKey ("lratFunc",           sm_lratFitFuncName);
   addOptionKey ("upperLratFunc",      sm_upperLratFitFuncName);
   addOptionKey ("lowerLratFunc",      sm_lowerLratFitFuncName);
   addOptionKey ("upperBandFunc",      sm_upperBandFitFuncName);
   addOptionKey ("upperExpectedValue", sm_upperExpectedValue);
   addOptionKey ("lowerExpectedValue", sm_lowerExpectedValue);
   addOptionKey ("observedValue",      sm_observedValue);
   addOptionKey ("numExpectedBins",    sm_numExpectedBins);
   addOptionKey ("accBandName",        sm_accBandName,
                 "Find acceptance band for only 'accBandName'");
   addOptionKey ("onlyName",           sm_accBandName,
                 "Find acceptance band or PE for only 'onlyName'");
   addOptionKey ("observedName",       sm_observedName,
                 "X axis name for FC bands");
   addOptionKey ("trueName",           sm_trueName,
                 "Y axis name for FC bands");
   addOptionKey ("fcBandName",         sm_fcBandName,
                 "Title of FC Band Plot");
   addOptionKey ("peNameForm",         sm_peNameForm,
                 "String to format title of PE distributions");
   addOptionKey ("lrNameForm",         sm_lrNameForm,
                 "String to format title of Likelihood Ratio distributions");
   addOptionKey ("filePrefix",         sm_filePrefix,
                 "Prefix given to all output files");
   addOptionKey ("peValueFactor",      sm_peValueFactor,
                 "Multiplicitive factor for peNameForm");
   addOptionKey ("peHistMax",          sm_peHistMax,
                 "Maximum Height for PE Distributions");

   // stand alone fitting
   addOptionKey ("SAFitFunc",      sm_SAFitFunc);      
   addOptionKey ("SAFitParamName", sm_SAFitParamName);
   addOptionKey ("SAFitXValName",  sm_SAFitXValName);
   addOptionKey ("SAFitYValName",  sm_SAFitYValName);
   addOptionKey ("SAFitYErrName",  sm_SAFitYErrName);
   addOptionKey ("SAFitYMax",      sm_SAFitYMax);
   addOptionKey ("SAFitYMin",      sm_SAFitYMin);
   addOptionKey ("SAFitBatchMode", sm_SAFitBatchMode,
                 "True if you do not want a window to pop up with fit results");
   // Control Flags
   addOptionKey ("makevert",   sm_makeVerticalSlicesBool,
                 "Make vertical slices");
   addOptionKey ("fitvert",    sm_fitVerticalSlicesBool,
                 "Fit vertical slices");
   addOptionKey ("fitPbest",   sm_fitPbestBool,
                 "Fit Pbest");      
   addOptionKey ("loadPEs",    sm_loadPEdistsBool,
                 "Load all PE distributions");
   addOptionKey ("plotPEs",    sm_plotPEdistsBool,
                 "Plot all PE distributions.  Works with loadPEs");
   addOptionKey ("makeLikeli", sm_makeLikeliRatBool,
                 "Make likelihood ratio distributions");
   addOptionKey ("fitLikeli",  sm_fitLikeliRatBool,
                 "Fit likelihood ratio distributions");
   addOptionKey ("plotBands",  sm_plotBandsBool,
                 "Plot FC acceptance bands");
   addOptionKey ("plotObservedValue",  sm_plotObservedValueBool,
                 "Plot observed value on FC acceptance bands");
   addOptionKey ("plotLikeli", sm_plotLikeliRatBool,
                 "Plot likelihood ratio distributions");
   addOptionKey ("makeAcc",    sm_makeAccBandsBool,
                 "Make acceptance bands");
   addOptionKey ("forceLowerEdge", sm_forceLowerEdgeValue,
                 "Force lower edge value to 'LowerEdgeValue'");
   addOptionKey ("forceUpperEdge", sm_forceUpperEdgeValue,
                 "Force upper edge value to 'UpperEdgeValue'");
   addOptionKey ("fitUpperBandEdge", sm_fitUpperBandEdge,
                 "Fit upper acceptance band edge for expected limts");
   addOptionKey ("plotExpectedLimit", sm_plotExpectedLimit);
   return 1;
}

void
FCPEs::_correctStrings()
{
   vector< string* > stringPtrVec;
   stringPtrVec.push_back( &sm_observedName );
   stringPtrVec.push_back( &sm_trueName     );
   stringPtrVec.push_back( &sm_fcBandName   );
   stringPtrVec.push_back( &sm_peNameForm   );
   stringPtrVec.push_back( &sm_lrNameForm   );
   for (vector< string* >::const_iterator iter = stringPtrVec.begin();
        stringPtrVec.end() != iter;
        ++iter)
   {
      // replace '-' with ' '
      // replace '@@' with '#'
      // cout << **iter << " : ";
      FCPEs::replaceAlphaWithBeta ( * (*iter), "_", " ");
      FCPEs::replaceAlphaWithBeta ( * (*iter), "@@", "#");
      // cout << **iter << endl;
   } // for iter

   // finish off sm_filePrefix if necessary
   if (sm_filePrefix.length())
   {
      if ('_' != sm_filePrefix[ sm_filePrefix.length() - 1 ])
      {
         sm_filePrefix += "_";
      }
   }
}

/////////////
// friends //
/////////////

ostream& operator<< (ostream& o_stream, const FCPEs &rhs)
{
   o_stream << setw (6) << rhs.key() << "   "
            << Form ("%.6f", rhs.trueValue()) << "  "
            << rhs.peWild();
   return o_stream;
} 
