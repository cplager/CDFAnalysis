// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TAuthenticate.h"
#include "CLPClasses/CLPClasses.h"
#include "CLPClasses/dout.h"


using namespace std;
using namespace CLPNamespace;

CLPFunctions::OptionMap CLPFunctions::ns_IntMap;
CLPFunctions::OptionMap CLPFunctions::ns_DoubleMap;
CLPFunctions::OptionMap CLPFunctions::ns_StringMap;
CLPFunctions::OptionMap CLPFunctions::ns_BoolMap;
// bools for makeZcandidates
// require tight z mass
bool   CLPFunctions::ns_mZc_tightZ    = false;
// don't require opposite charge on leptons
bool   CLPFunctions::ns_mZc_noQ       = false;
// use isolated tracks as second legs
bool   CLPFunctions::ns_mZc_useTracks = true;
// value for z0 cut
double CLPFunctions::ns_mZc_delta_z0_cut = 5 * CM;

// for lepton + jets
int                 CLPFunctions::ns_topEvent   = 0;
int                 CLPFunctions::ns_numDil     = 0;
double              CLPFunctions::ns_jetZ0      = 0;
CLP3Vector         *CLPFunctions::ns_met3VecPtr = 0;
CLPChargedParticle *CLPFunctions::ns_leptonPtr  = 0;

void 
CLPFunctions::parseArguments (int argc, char** argv)
{
   bool problems = false;
   for (int loop = 1; loop < argc; ++loop)
   {
      string arg = argv[loop];
      unsigned int where = arg.find_first_of("=");
      if (string::npos != where)
      {
         if (! setVariableFromString (arg) )
         {
            cerr << "Don't understand: " << arg << endl;
            problems = true;
         }
         continue;
      }
      if (arg.at(0) == '-')
      {
         listAllParameters();
         exit(0);
         //    if (! _setFlagFromString (arg) )
         //    {
         //       cerr << "Don't understand: " << arg << endl;
         //    }
         //    continue;
      }
      // if we're still here, we've got a problem
      cerr << "Don't understand: " << arg << endl;
      problems = true;
   } // for loop
   if (problems)
   {
      cerr << "Aborting" << endl;
      exit(1);
   }
}

bool 
CLPFunctions::setVariableFromString (const string &arg)
{
   unsigned int where = arg.find_first_of("=");
   string varname = arg.substr (0, where);
   string value   = arg.substr (where + 1);
   OptionMapIter iter = ns_DoubleMap.find(varname);
   if (iter != ns_DoubleMap.end())
   {
      // we found it
      double *doublePtr = (double*) iter->second;
      *doublePtr = atof(value.c_str());
      return true;
   }
   iter = ns_IntMap.find(varname);
   if (iter != ns_IntMap.end())
   {
      // we found it
      int *intPtr = (int*) iter->second;
      *intPtr = (int) atof( value.c_str() );
      //cout << "setting " << varname << " to " << *intPtr
      //     << " from " << value << endl;
      return true;
   }
   iter = ns_StringMap.find(varname);
   if (iter != ns_StringMap.end())
   {
      // we found it
      string *stringPtr = (string*) iter->second;
      *stringPtr = value;
      return true;
   }
   iter = ns_BoolMap.find(varname);
   if (iter != ns_BoolMap.end())
   {
      // we found it
      bool *boolPtr = (bool*) iter->second;
      int val = atoi(value.c_str());
      if (val)
      {
         *boolPtr = true;
      } else {
         *boolPtr = false;
      }               
      return true;
   }
   return false;
}

void
CLPFunctions::listAllParameters()
{
   // Integers
   if (ns_IntMap.size())
   {
      cout << "Integers:" << endl;
      for (OptionMapIter iter = ns_IntMap.begin();
           iter != ns_IntMap.end();
           ++iter)
      {
         cout << "  - " << iter->first << endl;
      } // for iter
      cout << endl;
   } // if integers
   // Doubles
   if (ns_DoubleMap.size())
   {
      cout << "Doubles:" << endl;
      for (OptionMapIter iter = ns_DoubleMap.begin();
           iter != ns_DoubleMap.end();
           ++iter)
      {
         cout << "  - " << iter->first << endl;
      } // for iter
      cout << endl;
   } // if doubles
   // Strings
   if (ns_StringMap.size())
   {
      cout << "Strings:" << endl;
      for (OptionMapIter iter = ns_StringMap.begin();
           iter != ns_StringMap.end();
           ++iter)
      {
         cout << "  - " << iter->first << endl;
      } // for iter
      cout << endl;
   } // if strings
   // Bools
   if (ns_BoolMap.size())
   {
      cout << "Bools:" << endl;
      for (OptionMapIter iter = ns_BoolMap.begin();
           iter != ns_BoolMap.end();
           ++iter)
      {
         cout << "  - " << iter->first << endl;
      } // for iter
      cout << endl;
   } // if bools
}


double 
CLPFunctions::eta2theta (double eta)
{
   // from some random web site
   // theta = atan( exp( - eta() ) ) * 2.0
   return 2. * atan( exp( - eta ) );
}

double
CLPFunctions::theta2eta (double theta)
{
   // make sure theta is o.k.
   if ((theta <= 0) || (theta >= kPi))
   {
      cout << "Error: CLPFunctions::theta2eta: illegal value of theta " 
           << theta << endl;
      return CLPNamespace::kErrorValue;
   }
   return - log( tan( theta / 2. ) ); 
}

double
CLPFunctions::rad2deg (double theta)
{
   return theta * TO_DEGREES;
}

double
CLPFunctions::deg2rad (double theta)
{
   return theta * TO_RADIANS;
}

double
CLPFunctions::addInQuad (double one, double two)
{
   return sqrt (one * one + two * two);
}

double
CLPFunctions::addInQuad (double one, double two, double three)
{
   return sqrt (one * one + two * two + three * three);
}

double
CLPFunctions::addInQuad (double one, double two, double three, double four,
                    double five, double six, double seven, double eight)
{
   return sqrt (one * one + two * two + three * three + four * four
      + five * five + six * six + seven * seven + eight * eight);
}

double
CLPFunctions::deltaPhi (double phi1, double phi2)
{
   double retval = phi2 - phi1;
   // correct for wrap around
   // use 'while' in case really silly values of phi were given
   // (e.g. abs(phi) > 2 pi)
   while (retval > kPi)
   {
      retval -= 2 * kPi;
   }
   while (retval < -kPi)
   {
      retval += 2 * kPi;
   }
   return retval;
}

void
CLPFunctions::copyTCA (const TClonesArray *sourcePtr, 
                       TClonesArray *targetPtr)
{
   const TClonesArray &source = *sourcePtr;
   TClonesArray &target = *targetPtr;
   target.Clear();
   int size = source.GetEntriesFast();
   TClass *tclass = source.GetClass();
   // Great.  I've got the new memory for the TClonesArray
   // Let's fill it.
   for (int loop = 0; loop < size; ++loop)
   {
      if (0 == strcmp (kElectronClassName, tclass->GetName() ) )
      {
         CLPElectron *ptr = (CLPElectron*) source.At(loop);
         new (target[loop]) CLPElectron( *ptr );
      }
      else if (0 == strcmp (kMuonClassName, tclass->GetName() ) )
      {
         CLPMuon *ptr = (CLPMuon*) source.At(loop);
         new (target[loop]) CLPMuon( *ptr );
      } 
      else if (0 == strcmp (kJetClassName, tclass->GetName() ) )
      {
         CLPJet *ptr = (CLPJet*) source.At(loop);
         new (target[loop]) CLPJet( *ptr );
      }
      else if (0 == strcmp (kTrackClassName, tclass->GetName() ) )
      {
         CLPTrack *ptr = (CLPTrack*) source.At(loop);
         new (target[loop]) CLPTrack( *ptr );
      } 
      else if (0 == strcmp (kHepgObjectClassName, tclass->GetName() ) )
      {
         CLPHEPGObject *ptr = (CLPHEPGObject*) source.At(loop);
         new (target[loop]) CLPHEPGObject( *ptr );
      }
      else if (0 == strcmp (kHepgContClassName, tclass->GetName() ) )
      {
         CLPHEPGCont *ptr = (CLPHEPGCont*) source.At(loop);
         new (target[loop]) CLPHEPGCont( *ptr );
      }
      else if (0 == strcmp (kPermutationClassName, tclass->GetName() ) )
      {
         CLPPermutation *ptr = (CLPPermutation*) source.At(loop);
         new (target[loop]) CLPPermutation( *ptr );
      }
      else
      {
         cerr << "Crap. " << tclass->GetName() << endl;
         assert(0);
      }
   } // for loop
}

void
CLPFunctions::sphericityTensorEigenValues (DVec &eigenvalues,
                                           const CLP3Vector::V3List &v3list)
{
   // special thanks to D0 for some easily readable code
   // http://www-d0.fnal.gov/D0Code/source/truffle/util/sphericity.cpp
   double sumsq = 0;
   TMatrixD matrix (3, 3);
   int size = (int) v3list.size();
   for (int index = 0; index < size; ++index) 
   {
     sumsq += v3list[index].mag2();
     for (int alpha = 0; alpha < 3; ++alpha)
     {
       for (int beta = 0; beta < 3; ++beta)
       {
         matrix (alpha, beta) += v3list[index][alpha] * v3list[index][beta];
       } // for beta
     } // for alpha
   } // for index
 
   TVectorD eval (3);
   matrix.EigenVectors (eval);
   assert (sumsq);
   // Despite what the comments in root say, eval is sorted in
   // _descending_ order.
   eigenvalues.clear();
   eigenvalues.push_back( eval(0) / sumsq ); 
   eigenvalues.push_back( eval(1) / sumsq ); 
   eigenvalues.push_back( eval(2) / sumsq );
   // Despite what the comment above says, let's make sure we get the
   // order we want (decending)
   std::sort (eigenvalues.rbegin(), eigenvalues.rend());
}

void
CLPFunctions::sphericityAplanarity (double &sphericity,
                                    double &aplanarity,
                                    const std::vector<CLP3Vector> &v3list)
{
   DVec eigenvalues;
   sphericityTensorEigenValues (eigenvalues, v3list);
   // sphericity = 3/2 (lambda2 + lambda3)
   sphericity = 1.5 * ( eigenvalues [2 - 1] + eigenvalues [3 - 1]);
   // aplanarity = 3/2 (lambda 3)
   aplanarity = 1.5 * eigenvalues [3 - 1];
}

double
CLPFunctions::centrality (const CLP3Vector::V3List &v3list)
{
   // Centrality is defined as:
   //               sum ( transverse momentum )
   // centrality = -----------------------------
   //                  sum ( | momentum | )
   double transMagSum = 0.;
   double magSum = 0.;
   int size = (int) v3list.size();
   for (int index = 0; index < size; ++index) 
   {
      transMagSum += v3list[index].transMag();
      magSum      += v3list[index].mag();
   } // for index
   assert (magSum);
   return transMagSum / magSum;
}

double
CLPFunctions::fix4VectorsForMass (CLP4Vector &vec1, CLP4Vector &vec2,
                                  double targetMass, 
                                  double upperwidth1, double upperwidth2,
                                  double lowerwidth1, double lowerwidth2)
{
   double mass = (vec1 + vec2).mass();
   double factor = (targetMass / mass) - 1;
   bool isNeg = false;
   int sign = 1;
   double min = 0;
   double max = -1;
   double e1 = vec1.mag();
   double e2 = vec2.mag();
   double width1 = upperwidth1;
   double width2 = upperwidth2;
   // Do we need to move the reconstructed mass up or down to meet the
   // target mass?
   if (factor < 0)
   {
      // if we're here, we're making the reconstructed mass smaller
      isNeg = true;
      sign = -1;
      factor *= -1;
      // Switch widths if we can
      if (lowerwidth1 >= 0)
      {
         width1 = lowerwidth1;
         width2 = lowerwidth2;
      }
      // we now need to make sure that we don't let the factor get
      // so large (in the negative sense) that we let either of our
      // W daughters disappear
      max = e1 / width1;
      if (e2 / width2 < max)
      {
         max = e2 / width2;
      }
   }
   // get a starting point.
   double numSig = factor * (e1 + e2) / (width1 + width2);
   for (int loop = 1; loop < 20; ++loop)
   {
      CLP4Vector tempvec1 = vec1 * (1 + sign * numSig * width1 / e1);
      CLP4Vector tempvec2 = vec2 * (1 + sign * numSig * width2 / e2);
      double newMass = (tempvec1 + tempvec2).mass();
      if (fabs(targetMass - newMass) < 0.001 * GEV)
      {
         break;
      }
      if ((newMass - targetMass) * sign > 0)
      {
         max = numSig;
         numSig = (min + max) / 2.;
      }  // if too much
      else 
      {
         // do we have a max defined?
         if (max < 0)
         {
            // no max definied
            min = numSig;
            numSig *= 1.2; // grow by 10%
         } else {
            min = numSig;
            numSig = (min + max) / 2.;
         }
      } // if not enough
   } // for loop
   // use latest numSig
   vec1 *= (1 + sign * numSig * width1 / e1);
   vec2 *= (1 + sign * numSig * width2 / e2);
   return numSig;
}

void
CLPFunctions::splitLine (SVec &retval, string line, char separator)
{
   retval.clear();
   line += separator; // get last word of line
   unsigned int last = string::npos;
   unsigned int current = line.find(separator);
   while (current != string::npos)
   {
      unsigned int pos;
      if (last != string::npos)
      {
         pos = last + 1;
      } else {
         pos = 0;
      }
      string part = line.substr( pos, current - last - 1);
      // don't bother adding 0 length strings
      if (part.length()) 
      {
         retval.push_back(part);
      }
      last = current;
      current = line.find(separator, current + 1);
   } 
}

double
CLPFunctions::numAsymSigma (double meas, double trueVal, 
                            double posSig, double negSig)
{
   double diff = meas - trueVal;
   if (diff > 0)
   {
      return diff / posSig;
   } else {
      return fabs (diff / negSig);
   }
}

double 
CLPFunctions::doubleBifGaus (double mass, int mode)
{
   double center      = -1.;    
   double neg_Sigma1  = -1.; 
   double pos_Sigma1  = -1.; 
   double neg_Frac    = -1.;   
   double pos_Frac    = -1.;   
   double neg_SigFrac = -1.;
   double pos_SigFrac = -1.;
   if (0 == mode)
   {
      // W mass
      // Center       7.87642e+01
      // Neg_Sigma1   6.29591e+00
      // Pos_Sigma1   9.92351e+00
      // Neg_Frac     3.52230e-01
      // Pos_Frac     9.59568e-01
      // Neg_SigFrac -2.36451e+00
      // Pos_SigFrac  3.92519e+00
      center      =  7.87642e+01;
      neg_Sigma1  =  6.29591e+00;
      pos_Sigma1  =  9.92351e+00;
      neg_Frac    =  3.52230e-01;
      pos_Frac    =  9.59568e-01;
      neg_SigFrac = -2.36451e+00;
      pos_SigFrac =  3.92519e+00;
   } else if (1 == mode) 
   {
      // t -> Wb mass
      // Center       1.72268e+02
      // Neg_Sigma1   2.15699e+01
      // Pos_Sigma1   1.25298e+01
      // Neg_Frac     6.26057e-01
      // Pos_Frac     9.47776e-01
      // Neg_SigFrac -3.45087e-01
      // Pos_SigFrac  3.47401e+00
      center      =  1.72268e+02;
      neg_Sigma1  =  2.15699e+01;
      pos_Sigma1  =  1.25298e+01;
      neg_Frac    =  6.26057e-01;
      pos_Frac    =  9.47776e-01;
      neg_SigFrac = -3.45087e-01;
      pos_SigFrac =  3.47401e+00;
   } else if (2 == mode) 
   {
      // t -> Zc mass
      // Center       1.73904e+02
      // Neg_Sigma1   6.95831e+00
      // Pos_Sigma1   8.57275e+00
      // Neg_Frac     4.14105e-01
      // Pos_Frac     8.74999e-01
      // Neg_SigFrac  2.83186e+00
      // Pos_SigFrac  2.64147e+00
      center      = 1.73904e+02;
      neg_Sigma1  = 6.95831e+00;
      pos_Sigma1  = 8.57275e+00;
      neg_Frac    = 4.14105e-01;
      pos_Frac    = 8.74999e-01;
      neg_SigFrac = 2.83186e+00;
      pos_SigFrac = 2.64147e+00;
   } else {
      abort ();
   }
   double arg    = mass - center;
   double sigma1 = neg_Sigma1; // negative sigma1
   double frac   = neg_Frac; // negative fraction
   double sigma2 = sigma1 * neg_SigFrac; // negative sigma2
   if (arg > 0)
   {
      sigma1 = pos_Sigma1; // positive sigma1
      frac   = pos_Frac; // positive fraction
      sigma2 = sigma1 * pos_SigFrac; // positive sigma2
   }
   if (fabs(sigma1) < 0.000001)
   {
      sigma1 = 0.0000001;
   }
   if (fabs(sigma2) < 0.000001)
   {
      sigma2 = 0.0000001;
   }
   return  frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1)) 
      + (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2));
}


double
CLPFunctions::figureOfMerit_ttbar (const CLP4Vector& lep0,
                                   const CLP4Vector& lep1,
                                   const CLPJet::JetList &jetList,
                                   int method)
{
   // Method:
   //
   //  0 - the old way (top mass window 170-180); old RMS widths (0, 1); 
   //      uncorr jets (0-2).  Fix W mass (energy only) (0-5)
   //  1 - use 175 for top mass (no window) (1-)
   //  2 - new RMS widths (2-5, 7)
   //  3 - L7 corrected jets (3 only)
   //  4 - L5 + TMT corrected jets (4-)
   //  5 - Corrected Z mass (energy only) (5-7)
   //  6 - fix W daughter 4 vectors (not just energy) (6-);
   //      swapped tWb and tZc (new RMS) widths (test case, 6 only)
   //  7 - correct tWb and tZc (new RMS) widths
   //  8 - fix Z daughter 4 vectors (not just energy) (8-)
   //  9 - new Gaussian fits for widths (9-)
   // 10 - use assymetric widths
   // 11 - use mass likelihoods
   // 12 - use mass likelihoods (both tops use tWb mass likelihood)
   CLP4Vector zBoson = lep0 + lep1;
   // just to be sure
   assert (jetList.size() >= 4);
   if (0 == method)
   {
      // The old way
      CLPNamespace::ns_lowerTopMass = 170. * GEV;
      CLPNamespace::ns_upperTopMass = 180. * GEV;
   } else {
      // 1 or more
      // The way it should be
      CLPNamespace::ns_lowerTopMass = 175. * GEV;
      CLPNamespace::ns_upperTopMass = 175. * GEV;
   }
   if (method < 2)
   {
      // 0 and 1 only
      // Old widths
      CLPNamespace::ns_W_width   = 10. * GEV;
      CLPNamespace::ns_tWb_width = 12. * GEV;
      CLPNamespace::ns_tZc_width = 12. * GEV;
   } else if (method > 8)
   {
      // 9 or more
      // newest widths (see log entry comment below #061017)
      CLPNamespace::ns_W_width   = 12.3 * GEV;
      CLPNamespace::ns_tWb_width = 16.9 * GEV;
      CLPNamespace::ns_tZc_width = 14.3 * GEV;
   } else if (6 == method )
   {
      // 6 only
      // tWb and tZc widths switched
      CLPNamespace::ns_W_width   = 17. * GEV;
      CLPNamespace::ns_tWb_width = 21. * GEV;
      CLPNamespace::ns_tZc_width = 24. * GEV;
   } else {
      // 2-5, 7, 8
      // RMS of old fits
      CLPNamespace::ns_W_width   = 17. * GEV;
      CLPNamespace::ns_tWb_width = 24. * GEV;
      CLPNamespace::ns_tZc_width = 21. * GEV;
   }
   // From http://www-cdf.fnal.gov/~cplager/internal/Analysis/TopFCNC/
   //           log/0610/log.html#061017
   // Mass 	Symm    Pos    Negative Error
   // W      12.3    10.7    -13.8
   // t->Wb  16.9    13.5    -20.2
   // t->Zc  14.3    10.5    -18.0
   double   W_posWidth = 10.7;
   double tWb_posWidth = 13.5;
   double tZc_posWidth = 10.5;
   double   W_negWidth = 13.8;
   double tWb_negWidth = 20.2;
   double tZc_negWidth = 18.0;
   // get the four jet momentums
   CLP4Vector bottom;
   CLP4Vector w1;    
   CLP4Vector w2;    
   CLP4Vector charm; 
   bottom = jetList[kBottomJet].mom4Vec();
   w1     = jetList[kW1Jet].mom4Vec();
   w2     = jetList[kW2Jet].mom4Vec();
   charm  = jetList[kCharmJet].mom4Vec();
         
   if (3 == method)
   {
      bottom = jetList[kBottomJet].mom4Vec (CLPJet::kLevel7);
      w1     = jetList[kW1Jet].mom4Vec (CLPJet::kLevel7);
      w2     = jetList[kW2Jet].mom4Vec (CLPJet::kLevel7);
      charm  = jetList[kCharmJet].mom4Vec (CLPJet::kLevel7);
   } else if (method >= 4)
   {
      bottom = jetList[kBottomJet].mom4Vec (CLPJet::kLevel5)
         * jetList[kBottomJet].bottomJetMultFactor_TM();
      w1     = jetList[kW1Jet].mom4Vec (CLPJet::kLevel5)
         * jetList[kW1Jet].lightJetMultFactor_TM();
      w2     = jetList[kW2Jet].mom4Vec (CLPJet::kLevel5)
         * jetList[kW2Jet].lightJetMultFactor_TM();
      charm  = jetList[kCharmJet].mom4Vec (CLPJet::kLevel5)
         * jetList[kCharmJet].lightJetMultFactor_TM();
   }
   // Make our W
   CLP4Vector wBoson = w1 + w2;
   double wMass = wBoson.mass();
   double wDiff = fabs ( (wMass - kWmass) / ns_W_width );
   if (wDiff > kWmass)
   {
      return CLP4Vector::kErrorMass;      
   }
   // should we fix the W mass before going on?
   if (method <= 5)
   {
      wBoson.setRestMass (kWmass);
   }
   if (method >= 5 && method <= 7)
   {      
      zBoson.setRestMass (kZmass);
      CLPNamespace::ns_tZc_width = 21. * GEV;
   } else if (method > 7)
   {
      CLP4Vector lep0m (lep0);
      CLP4Vector lep1m (lep1);
      fix4VectorsForMass (lep0m, lep1m, kZmass, 
                          0.05 * lep0.E(),
                          0.05 * lep1.E());
      zBoson = lep0m + lep1m;
   } 
   if (method >= 6)
   {
      fix4VectorsForMass (w1, w2, kWmass, 
                          jetList[kW1Jet].lightJetAbsoluteWidth_TM(),
                          jetList[kW2Jet].lightJetAbsoluteWidth_TM());
      wBoson = w1 + w2;
   }
   double tWbMass = (wBoson + bottom).mass();
   double tZcMass = (zBoson + charm).mass();
   double aveTopMass = (tWbMass + tZcMass) / 2;
   // is the average top mass allowed 
   if (aveTopMass < ns_lowerTopMass)
   {
      aveTopMass = ns_lowerTopMass;
   } else if (aveTopMass > ns_upperTopMass)
   {
      aveTopMass = ns_upperTopMass;
   }
   double tWbDiff;
   double tZcDiff;
   if (method != 10)
   {
      // usual method
      tWbDiff = fabs ( (tWbMass - aveTopMass) / ns_tWb_width );
      tZcDiff = fabs ( (tZcMass - aveTopMass) / ns_tZc_width );
   } else {
      // method 10 only
      wDiff   = numAsymSigma (wMass,  kWmass, W_posWidth, W_negWidth);
      tWbDiff = numAsymSigma (tWbMass, aveTopMass, tWb_posWidth, tWb_negWidth);
      tZcDiff = numAsymSigma (tZcMass, aveTopMass, tZc_posWidth, tZc_negWidth);
   }
   if (11 == method)
   {
      double retval = 1.;
      retval *= doubleBifGaus (wMass,   0);
      retval *= doubleBifGaus (tWbMass, 1);
      retval *= doubleBifGaus (tZcMass, 2);
      return -2 * log (retval);
   }
   if (12 == method)
   {
      double retval = 1.;
      retval *= doubleBifGaus (wMass,   0);
      retval *= doubleBifGaus (tWbMass, 1);
      retval *= doubleBifGaus (tZcMass, 1);
      return -2 * log (retval);
   }
   if (tWbMass < 0 || tZcMass < 0)
   {
      return CLP4Vector::kErrorMass;      
   }
   return addInQuad (wDiff, tWbDiff, tZcDiff);
} 

double
CLPFunctions::figureOfMerit_tWb (const CLPJet::JetList &jetList,
                                 OptMethodType method,
                                 bool fixWmass)
{
   // just to be sure
   assert (jetList.size() >= 3);
   CLP4Vector bottom = jetList[kBottomJet].mom4Vec();;
   CLP4Vector w1     = jetList[kW1Jet].mom4Vec();;
   CLP4Vector w2     = jetList[kW2Jet].mom4Vec();;
   // Make our W
   CLP4Vector wBoson = w1 + w2;
   double wDiff = fabs ( (wBoson.mass() - ns_WMass) / ns_W_width );
   // should we fix the W mass before going on?
   if (fixWmass)
   {
      wBoson.setRestMass (ns_WMass);
   }
   double tWbMass = (wBoson + bottom).mass();
   if (tWbMass < 0)
   {
      return CLP4Vector::kErrorMass;
   }
   double tWbDiff = fabs ( (tWbMass - topMass()) / ns_tWb_width );
   return addInQuad (wDiff, tWbDiff);
}

double 
CLPFunctions::sq (double value) 
{ 
   return value * value; 
}


TChain*
CLPFunctions::chainOneFile (const std::string& filename, 
                            const char treename[])
{
   TAuthenticate::SetGlobalUser("cdfdata");
   TChain *chainPtr =  new TChain(treename);
   chainPtr->AddFile( filename.c_str(), TChain::kBigNumber );
   return chainPtr;   
}


TChain*
CLPFunctions::chainFromList (const std::string& filename, 
                             const char treename[])
{
   TAuthenticate::SetGlobalUser("cdfdata");
   
   TChain *chain =  new TChain(treename);

   RunListClass runList (filename, "");
   int size = (int) runList.size();
   cout << "size " << size << endl;
   for (int loop = 0; loop < size; ++loop)
   {
      chain->AddFile( runList.filename(loop), TChain::kBigNumber );
   } // for loop

   return chain;   
}

void
CLPFunctions::makeZcandidates (CLPCandidate::CandidateList &candList,
                               const CLPElectron::ElectronList &elecList,
                               const CLPMuon::MuonList &muonList,
                               const CLPTrack::TrackList &trackList)
{
   candList.clear();
   int size = (int) elecList.size();
   int trackSize = (int) trackList.size();
   ///////////////////////
   // Electron-Electron //
   ///////////////////////
   for (int outer = 0; outer < size - 1; ++outer)
   {
      int outerSign = elecList[outer].charge();

      for (int inner = outer + 1; inner < size; ++inner)
      {
         // Is at least one of these tight?  We want to continue if
         // BOTH are NOT tight (i.e. ! && !).
         if (! elecList[outer].isaTightLepton() &&
             ! elecList[inner].isaTightLepton() )
         {
            // we currently want at least one tight
            continue;
         }
         // check the signs (allowing for 0 sign)
         int innerSign = elecList[inner].charge();
         if ( (1 == outerSign * innerSign) && ! ns_mZc_noQ )
         {
            // we don't want same sign (unless we have ns_mZc_noQ set)
            continue;
         }
         CLPCandidate elecCand;
         elecCand.addElectron( elecList[outer] );
         elecCand.addElectron( elecList[inner] );
         elecCand.calculate();
         // Do we need to check Z mass?
         if (ns_mZc_tightZ)
         {
            if (elecCand.Zmass() > kUpperTightZmass ||
                elecCand.Zmass() < kLowerTightZmass)
            {
               // we don't want this one
               continue;
            }            
         }
         candList.push_back (elecCand);
      } // for inner electron
   } // for outer electron

   ////////////////////
   // Electron-Track //
   ////////////////////
   for (int outer = 0;  ns_mZc_useTracks && (outer < size); ++outer)
   {
      // is this a tight lepton
      if (! elecList[outer].isaTightLepton())
      {
         // don't bother going any further
         continue;
      }
      int outerSign = elecList[outer].charge();
      for (int inner = 0;inner < trackSize; ++inner)
      {
         // don't bother if this track is already matched to an
         // electron
         if (trackList[inner].isMatchedToElectron() || 
             trackList[inner].isMatchedToMuon() )
         {
            // Next!
            continue;
         }
         // check the signs (allowing for 0 sign)
         int innerSign = trackList[inner].charge();
         if ( (1 == outerSign * innerSign) && ! ns_mZc_noQ )
         {
            // we don't want same sign (unless we have ns_mZc_noQ set)
            continue;
         }
         // we want to treat this track as an electron
         CLPCandidate elecTrkCand;
         elecTrkCand.addElectron( elecList[outer] );
         elecTrkCand.addTrack( trackList[inner], 
                               CLPTrack::kAsElectronMom );
         elecTrkCand.calculate();
         // Do we need to check Z mass?
         if (ns_mZc_tightZ)
         {
            if (elecTrkCand.Zmass() > kUpperTightZmass ||
                elecTrkCand.Zmass() < kLowerTightZmass)
            {
               // we don't want this one
               continue;
            }            
         }
         candList.push_back (elecTrkCand);
      } // for inner track
   } // for outer electron

   ///////////////
   // Muon-Muon //
   ///////////////
   size = (int) muonList.size();
   for (int outer = 0; outer + 1 < size; ++outer)
   {
      int outerSign = muonList[outer].charge();
      for (int inner = outer + 1; inner < size; ++inner)
      {
         // check the signs (allowing for 0 sign)
         int innerSign = muonList[inner].charge();
         if ( (1 == outerSign * innerSign) && ! ns_mZc_noQ )
         {
            // we don't want same sign (unless we have ns_mZc_noQ set)
            continue;
         }
         // // make sure that both of these is a tight lepton
         // if (! muonList[outer].isaTightLepton() ||
         //     ! muonList[inner].isaTightLepton() )
         // {
         //    // not good enough
         //    continue;
         // }
         CLPCandidate muonCand;
         muonCand.addMuon( muonList[outer] );
         muonCand.addMuon( muonList[inner] );
         muonCand.calculate();
         // Do we need to check Z mass?
         if (ns_mZc_tightZ)
         {
            if (muonCand.Zmass() > kUpperTightZmass ||
                muonCand.Zmass() < kLowerTightZmass)
            {
               // we don't want this one
               continue;
            }            
         }
         candList.push_back (muonCand);
      } // for inner muon
   } // for outer muon

   ////////////////
   // Muon-Track //
   ////////////////
   for (int outer = 0;  ns_mZc_useTracks && (outer < size); ++outer)
   {
      // is this a tight lepton
      if (! muonList[outer].isaTightLepton() )
      {
         // we currently want tight, tight, so don't bother
         continue;
      }
      int outerSign = muonList[outer].charge();
      for (int inner = 0;inner < trackSize; ++inner)
      {
         // don't bother if this track is already matched to an
         // muon
         if (trackList[inner].isMatchedToElectron() || 
             trackList[inner].isMatchedToMuon() )
         {
            // Next!
            continue;
         }
         // check the signs (allowing for 0 sign)
         int innerSign = trackList[inner].charge();
         if ((1 == outerSign * innerSign) && ! ns_mZc_noQ)
         {
            // we don't want same sign
            continue;
         }
         // we want to treat this track as an muon
         CLPCandidate muonTrkCand;
         muonTrkCand.addMuon( muonList[outer] );
         muonTrkCand.addTrack( trackList[inner], 
                               CLPTrack::kAsMuonMom );
         muonTrkCand.calculate();
         // Do we need to check Z mass?
         if (ns_mZc_tightZ)
         {
            if (muonTrkCand.Zmass() > kUpperTightZmass ||
                muonTrkCand.Zmass() < kLowerTightZmass)
            {
               // we don't want this one
               continue;
            }            
         }
         candList.push_back (muonTrkCand);
      } // for inner track
   } // for outer muon

   CLPCandidate::CandidateList tempList;      
   for (CLPCandidate::CandidateListIter iter = candList.begin();
        candList.end() != iter;
        ++iter)
   {
      // does this candidate pass the Z0 cut?
      double deltaZ0 = fabs( iter->lepton(0).Z0() - iter->lepton(1).Z0() );
      if (deltaZ0 < ns_mZc_delta_z0_cut)
      {
         tempList.push_back( *iter );
      }
   } // for iter
   candList = tempList;
}

bool
CLPFunctions::passesLJfilter(TClonesArray *jetTcaPtr,
                             TClonesArray *elecTcaPtr,
                             TClonesArray *muonTcaPtr,
                             CLPJet::JetList &tightJetList)
{
   //////////
   // Jets //
   //////////
   CLPJet::JetList looseJetList;
   tightJetList.clear();
   int numJets = jetTcaPtr->GetEntries();
   for (int jetLoop = 0; jetLoop < numJets; ++jetLoop)
   {
      CLPJet *jetPtr = 
         (CLPJet*) jetTcaPtr->At (jetLoop);
      // Is this a loose jet (one we use to correct MET)?
      if (jetPtr->Et(CLPJet::kLevel5) > 12 * GEV &&
          fabs(jetPtr->etaDetector()) < 2.4)
      {
         looseJetList.push_back (*jetPtr);
         // Is this a tight jet (counted towards jet count)
         if (jetPtr->Et(CLPJet::kLevel5) > 20 * GEV &&
             fabs(jetPtr->etaDetector()) < 2.0)
         {
            tightJetList.push_back (*jetPtr);
         } // if tight jet
      } // if loose jet
   } // for jetLoop
      
   // Fix MET
   correctMETforJets (looseJetList, CLPJet::kLevel5);

   ns_leptonPtr = 0;
   ///////////////
   // Electrons //
   ///////////////
   int numPassed = 0;
   int numElecs = elecTcaPtr->GetEntries();
   for (int elecLoop = 0; elecLoop < numElecs; ++elecLoop)
   {
      CLPElectron *elecPtr =  (CLPElectron*) elecTcaPtr->At (elecLoop);
      CLPCuts::ElectronResult result = CLPCuts::electronCuts(*elecPtr);
      if (result.none() && ! elecPtr->isaPHX() && ! elecPtr->isaPEM())
      {
         ++numPassed;
         ns_leptonPtr = (CLPChargedParticle*) elecPtr;
      } // add electron to the list
   } // for elecLoop

   ///////////
   // Muons //
   ///////////
   CLPMuon::MuonList muonList;
   int numMuons = muonTcaPtr->GetEntries();
   for (int muonLoop = 0; muonLoop < numMuons; ++muonLoop)
   {
      CLPMuon *muonPtr = (CLPMuon*) muonTcaPtr->At (muonLoop);
      CLPCuts::MuonResult result = CLPCuts::muonCuts(*muonPtr);
      if (result.none() && muonPtr->isaTightLepton())
      {
         ++numPassed;
         ns_leptonPtr = (CLPChargedParticle*) muonPtr;
         // cout << "muon " << *muonPtr << endl;
      } // add muon to the list
   } // for muonLoop

   // Does this have at exactly one tight lepton?
   if (1 != numPassed)                                  return false;

   // dilepton veto
   if (ns_numDil >= 2)                                  return false;

   // Z veto
   if (ns_topEvent & kTopEvent_ZVeto)                   return false;

   // vertex cut
   if (fabs (ns_jetZ0) > 60 * CM)                       return false;

   // Is the lepton within 5 CM of Z0
   if (fabs( ns_leptonPtr->Z0() - ns_jetZ0 ) > 5 * CM)  return false;

   // are there at least three jets?
   if (tightJetList.size() < 3)                         return false;

   // is MET at least 
   if (ns_met3VecPtr->transMag() < 20 * GEV)            return false;

   // If we're still here, take it
   return true;
}

void
CLPFunctions::correctMETforJets (const CLPJet::JetList &jetList, int level)
{
   int numgoodjets = jetList.size();
   for (int jetLoop = 0; jetLoop < numgoodjets; ++jetLoop)
   {
      // The correction is the direction * delta Et
      CLP3Vector correction = 
         jetList[jetLoop].mom3Vec().transverse().unit() *
         (jetList[jetLoop].Et(level) - 
          jetList[jetLoop].Et(CLPJet::kUncorrected));
      *ns_met3VecPtr -= correction;
   } // for jetLoop
}

bool 
CLPFunctions::acceptableHeavyFlavorContent (unsigned int heavyFlavorContent,
                                            bool isHeavyFlavorSample,
                                            int &numRejected)
{
   // print events that are thrown out
   // LF: keep if no HF or (massless) cc/bb from parton shower in same jet
   if ( ! isHeavyFlavorSample ) 
   {
      if ( testBit (heavyFlavorContent, kHasCMatrixElement ) ||
           testBit (heavyFlavorContent, kHasCbarMatrixElement ) ||
           testBit (heavyFlavorContent, kHasBMatrixElement ) ||
           testBit (heavyFlavorContent, kHasBbarMatrixElement ) ) 
      {
         if (ns_debug)
         {
            cout << ns_runNumber << ", " << ns_eventNumber
                 << ": (LF) reject massless c/b" << endl;
         }
         ++numRejected;
         return false;
      } // if has matrix element heavy flavor
      if ( testBit (heavyFlavorContent, kHasCPartonShower ) ||
           testBit (heavyFlavorContent, kHasCbarPartonShower ) ||
           testBit (heavyFlavorContent, kHasBPartonShower ) ||
           testBit (heavyFlavorContent, kHasBbarPartonShower ) ) 
      {
         if ( testBit (heavyFlavorContent, kHasCcbarSameJetPartonShower ) ||
              testBit (heavyFlavorContent, kHasBbbarSameJetPartonShower )  ) 
         {
            // keep
            return true;
         } else {
            if (ns_debug)
            {
               cout << ns_runNumber << ", " << ns_eventNumber
                    << ": (LF) reject separated c/b from PS" << endl;
            }
            ++numRejected;
            return false;
         }
      } // if has parton shower heavy flavor
   } // is not HF
   else {
      if ( testBit (heavyFlavorContent, kHasCcbarSameJetMatrixElement ) ||
           testBit (heavyFlavorContent, kHasBbbarSameJetMatrixElement ) ) 
      {
         if (ns_debug)
         {
            cout << ns_runNumber << ", " << ns_eventNumber
                 << ": (HF) reject cc/bb within same jet" << endl;
         }
         ++numRejected;
         return false;
      } // if has parton show heavy flavor in same jet
      return true;
   } // is HF 
   // Should never get here, but just to avoid warnings...
   return true;
} 

bool 
CLPFunctions::testBit (unsigned int flag, int bit)
{  
   return flag & (1 << bit); 
}   

void
CLPFunctions::decodeHeavyFlavorWord (unsigned int heavyFlavorWord)
{
   const string kBitNames [kNumHeavyFlavorEnums] =
   {
      // Charm Flags
      "HasHEPGCharm",
      "HasCPartonShower",
      "HasCbarPartonShower",
      "HasCMatrixElement",
      "HasCbarMatrixElement",
      "HasCcbarSameJetPartonShower",
      "HasCcbarSameJetMatrixElement",
      // Bottom Flags
      "HasHEPGBottom",
      "HasBPartonShower",
      "HasBbarPartonShower",
      "HasBMatrixElement",
      "HasBbarMatrixElement",
      "HasBbbarSameJetPartonShower",
      "HasBbbarSameJetMatrixElement"
   };
   for (int bit = 0; bit < kNumHeavyFlavorEnums; ++bit)
   {
      if (testBit (heavyFlavorWord, bit))
      {
         cout << " " << kBitNames[bit];
      } // if bit is set
   } // for bit 
   cout << endl;
}


//////////////////////
// Access Functions //
//////////////////////

void 
CLPFunctions::setDataType (DataType type) 
{ 
   ns_dataType = type; 
}

CLPNamespace::DataType 
CLPFunctions::dataType() 
{ 
   return ns_dataType; 
}

bool 
CLPFunctions::isData()
{
   return kData == ns_dataType;
}

bool 
CLPFunctions::isMC()
{
   return kMC == ns_dataType;
}

void 
CLPFunctions::setDebug (bool debug) 
{ 
   ns_debug = debug; }

bool 
CLPFunctions::debug() 
{ 
   return ns_debug; 
}

void 
CLPFunctions::setNtupleType (NtupleType type) 
{ 
   ns_ntupleType = type; 
}

CLPNamespace::NtupleType 
CLPFunctions::ntupleType() 
{ 
   return ns_ntupleType; 
}

bool 
CLPFunctions::isRochNtuple() 
{ 
   return kRochNtuple == ns_ntupleType; 
}

bool 
CLPFunctions::isTopNtuple() 
{ 
   return kTopNtuple == ns_ntupleType; }

bool 
CLPFunctions::isUCNtuple() 
{ 
   return kUCNtuple == ns_ntupleType; 
}

void 
CLPFunctions::setUserFloat (int index, double value)
{
   assert ((index >= 0) && (index < kNumUserFloats));
   ns_userFloats[index] = value;
}

void 
CLPFunctions::setUserInt (int index, int value)
{
   assert ((index >= 0) && (index < kNumUserInts));
   ns_userInts[index] = value;
}

void 
CLPFunctions::setUserString (int index, std::string value)
{
   assert ((index >= 0) && (index < kNumUserStrings));
   ns_userStrings[index] = value;
}

double 
CLPFunctions::userFloat (int index)
{
   assert ((index >= 0) && (index < kNumUserFloats));
   return ns_userFloats[index];
}

int 
CLPFunctions::userInt (int index)
{
   assert ((index >= 0) && (index < kNumUserInts));
   return ns_userInts[index];
}

void 
CLPFunctions::setUserBitOn (int bit)
{
   assert ((bit >= 0) && (bit < 64));
   ns_userBits |= ( (Long64_t) 1 ) << bit;
}

void 
CLPFunctions::setUserBitOff (int bit)
{
   assert ((bit >= 0) && (bit < 64));
   Long64_t mask = ( (Long64_t) 1 ) << bit;
   mask = ~mask;
   ns_userBits |= mask;
}

bool
CLPFunctions::isUserBitOn (int bit)
{
   assert ((bit >= 0) && (bit < 64));
   Long64_t mask =  ( (Long64_t) 1 ) << bit;
   return ns_userBits & mask;
}

std::string 
CLPFunctions::userString (int index)
{
   assert ((index >= 0) && (index < kNumUserStrings));
   return ns_userStrings[index];
}

void 
CLPFunctions::setOutputMode (CLPNamespace::CLPOutputMode mode) 
{ 
   ns_outputMode = mode; 
}

CLPNamespace::CLPOutputMode 
CLPFunctions::outputMode() 
{ 
   return ns_outputMode; 
}

void 
CLPFunctions::setTopMass (double mass) 
{ 
   ns_topMass = mass;
}

void 
CLPFunctions::setLowerTopMass (double mass) 
{ 
   ns_lowerTopMass = mass;
}

void 
CLPFunctions::setUpperTopMass (double mass) 
{ 
   ns_upperTopMass = mass;
}

double
CLPFunctions::topMass() 
{ 
   return ns_topMass; 
}

double
CLPFunctions::lowerTopMass() 
{ 
   return ns_lowerTopMass; 
}

double
CLPFunctions::upperTopMass() 
{ 
   return ns_upperTopMass; 
}



double 
CLPFunctions::W_width   () 
{ 
   return ns_W_width;   
}

double 
CLPFunctions::tWb_width () 
{ 
   return ns_tWb_width; 
}

double 
CLPFunctions::tZc_width () 
{ 
   return ns_tZc_width; 
}

void 
CLPFunctions::setW_width   (double width) 
{ 
   ns_W_width   = width; 
}

void 
CLPFunctions::settWb_width (double width) 
{ 
   ns_tWb_width = width; 
}

void 
CLPFunctions::settZc_width (double width) 
{ 
   ns_tZc_width = width; 
}


