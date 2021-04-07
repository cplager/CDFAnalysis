// -*- C++ -*-

#include <iostream>

#include "FCPEs.hh"
#include "defineFunctions.hh"

using namespace std;

void defineFunctions()
{
   TF1 *funcPtr = 0;

   ////////////////
   // splitGaus2 //
   ////////////////
   funcPtr = FCPEs::addFittingFunction ("splitGauss2",  splitGauss2, 
                                        FCPEs::lowerVertEdge(), 
                                        FCPEs::upperVertEdge(), 
                                        8 );
   // Sets initial values and parameter names
   funcPtr->SetParNames("Constant",           //  0
                        "Max_bValue",         //  1
                        "Negative_Sigma1",    //  2
                        "Positive_Sigma1",    //  3
                        "Negative_Frac",      //  4
                        "Positive_Frac",      //  5
                        "Negative_SigFrac",   //  6
                        "Positive_SigFrac" ); //  7
   
   funcPtr->SetParameters (4.5e4, //  0: Constant	      
                           0.00,  //  1: Max_bValue      
                           0.040, //  2: Negative_Sigma1 
                           0.035, //  3: Positive_Sigma1 
                           0.4,   //  4: Negative_Frac   
                           0.4,   //  5: Positive_Frac   
                           2,     //  6: Negative_SigFrac
                           2);    //  7: Positive_SigFrac
   
   funcPtr->SetParLimits (0, 0., 1e6);   //  0: Constant	      
   funcPtr->SetParLimits (1, -0.1, 1000); //  1: Max_bValue      
   funcPtr->SetParLimits (2, 0., 50.);    //  2: Negative_Sigma1 
   funcPtr->SetParLimits (3, 0., 50.);    //  3: Positive_Sigma1 
   funcPtr->SetParLimits (4, 0, 1);      //  4: Negative_Frac   
   funcPtr->SetParLimits (5, 0, 1);      //  5: Positive_Frac   
   funcPtr->SetParLimits (6, 0.2, 5);    //  6: Negative_SigFrac
   funcPtr->SetParLimits (7, 0.2, 5);    //  7: Positive_SigFrac

   ////////////////
   // halfGauss2 //
   ////////////////
   funcPtr = FCPEs::addFittingFunction ("halfGauss2", halfGauss2,
                                        FCPEs::lowerLikeliEdge(), 
                                        FCPEs::upperLikeliEdge(), 
                                        5 );
   funcPtr->SetParNames( "Constant",         //  0
                         "Max_bValue",       //  1
                         "Sigma1",           //  2
                         "Frac",             //  3
                         "SigFrac"           //  4 
      );
   funcPtr->SetParameters (1, 0.00, 0.04, 0.04, 0.5 );

   /////////////////////
   // upperHalfGauss2 //
   /////////////////////
   funcPtr = FCPEs::addFittingFunction ("upperHalfGauss2", upperHalfGauss2,
                                        FCPEs::lowerLikeliEdge(), 
                                        FCPEs::upperLikeliEdge(), 
                                        5 );
   funcPtr->SetParNames( "Constant",         //  0
                         "Max_bValue",       //  1
                         "Sigma1",           //  2
                         "Frac",             //  3
                         "SigFrac"           //  4 
      );
   funcPtr->SetParameters (1, 0.00, 0.04, 0.04, 0.5 );

   ////////////////////
   // SplitGaussBump //
   ////////////////////
   funcPtr = FCPEs::addFittingFunction ("splitGaussBump", splitGauss3,
                                        FCPEs::lowerLikeliEdge(), 
                                        FCPEs::upperLikeliEdge(), 
                                        12 );
   funcPtr->SetParNames( "Constant",         //  0
                         "Max_bValue",       //  1
                         "Negative_Sigma1",  //  2
                         "Positive_Sigma1",  //  3
                         "Negative_Frac",    //  4
                         "Positive_Frac",    //  5
                         "Negative_SigFrac", //  6
                         "Positive_SigFrac", //  7
                         "Constant3",        //  8
                         "Mean3",            //  9
                         "Sigma3"            // 10
      );
   funcPtr->SetParName (11, "negconst");

   double pars [12] = {1, 0.0, 0.04, 0.04, 0.5, 0.5, 2, 2, 0.5, 
                       -0.0427, 0.033, 0.00 };
   funcPtr->SetParLimits( 8, 0.0, 0.8 );
   funcPtr->SetParLimits( 9, -0.1, 0.0 );
   funcPtr->SetParLimits(10, 0., 1.0);
   funcPtr->SetParLimits(11, -0.01, 1.0);
   funcPtr->SetParameters(  pars ); 


   //////////////////
   // expPlusConst //
   //////////////////
   funcPtr = FCPEs::addFittingFunction ("expPlusConst", expPlusConst,
                                        FCPEs::lowerVertEdge(), 
                                        FCPEs::upperVertEdge(), 
                                        3 );
   funcPtr->SetParNames( "Norm", "Tau", "Offset" );
   funcPtr->SetParameters( 10000, 0.25, 30000 );
   funcPtr->SetParLimits( 1, 0.001, 1e6 );

   ///////////
   // polyN //
   ///////////
   for (int num = 2; num <= 9; ++num)
   {
      funcPtr = FCPEs::addFittingFunction( new TF1 (Form ("poly%d", num), 
                                                    Form ("pol%d(0)", num) ) );
      funcPtr->SetParameters (45000, 0, 0, 0);
   }
}


Double_t splitGauss2(Double_t *x, Double_t *par)
{
   // Parameters:
   //  0: Constant	      
   //  1: Max_bValue      
   //  2: Negative_Sigma1 
   //  3: Positive_Sigma1 
   //  4: Negative_Frac   
   //  5: Positive_Frac   
   //  6: Negative_SigFrac
   //  7: Positive_SigFrac

   double value = x[0];   
   double norm   = par[0];
   double arg    = value - par[1];
   double sigma1 = par[2]; // negative sigma1
   double frac   = par[4]; // negative fraction
   double sigma2 = sigma1 * par[6]; // negative sigma2

   // bifurcate
   if (arg > 0) {
      sigma1 = par[2 + 1]; // positive sigma1
      frac   = par[4 + 1]; // positive fraction
      sigma2 = sigma1 * par[6 + 1]; // positive sigma2
   }

   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1) ) +
      norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2) );
   return fitval;
}

Double_t halfGauss2( Double_t* x, Double_t* par ) 
{
   // Parameters:
   //  0: Constant	      
   //  1: Max_bValue      
   //  2: (positive) Sigma
   //  3: (positive) Frac
   //  4: (positive) SigFrac
   double val    = x[0];   
   double norm   = par[0];
   double arg    = val - par[1];
   double sigma1 = par[2]; // positive sigma1
   double frac   = par[3];
   double sigma2 = sigma1 * par[4]; // positive sigma2

   double fitval = 0;

   // bifurcate: force negative half to be constant
   if (arg > 0) {
      fitval = 
         norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1)) +
         norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2));
   } else {
      fitval = norm;
   }
   return fitval;
}

Double_t upperHalfGauss2( Double_t* x, Double_t* par ) 
{
   // Parameters:
   //  0: Constant	      
   //  1: Max_bValue      
   //  2: (positive) Sigma
   //  3: (positive) Frac
   //  4: (positive) SigFrac
   double val    = x[0];   
   double norm   = par[0];
   double arg    = val - par[1];
   double sigma1 = par[2]; // positive sigma1
   double frac   = par[3];
   double sigma2 = sigma1 * par[4]; // positive sigma2

   double fitval = 0;

   // bifurcate: force positive half to be constant
   if (arg < 0) {
      fitval = 
         norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1)) +
         norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2));
   } else {
      fitval = norm;
   }
   return fitval;
}

Double_t splitGauss2PlusConst(Double_t *x, Double_t *par)
{
   // Parameters:
   //  0: Constant	      
   //  1: Max_bValue      
   //  2: Negative_Sigma1 
   //  3: Positive_Sigma1 
   //  4: Negative_Frac   
   //  5: Positive_Frac   
   //  6: Negative_SigFrac
   //  7: Positive_SigFrac
   //  8: Offset

   double value = x[0];   
   double norm   = par[0];
   double arg    = value - par[1];
   double sigma1 = par[2]; // negative sigma1
   double frac   = par[4]; // negative fraction
   double sigma2 = sigma1 * par[6]; // negative sigma2
   double offset = par[8];

   // bifurcate
   if (arg > 0) {
      sigma1 = par[2 + 1]; // positive sigma1
      frac   = par[4 + 1]; // positive fraction
      sigma2 = sigma1 * par[6 + 1]; // positive sigma2
   } 

   if( arg > -0.15 ) offset = 0;

   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1) ) +
      norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2) ) +
      offset;
   return fitval;
}

Double_t splitGauss3(Double_t *x, Double_t *par)
{
   // Parameters:
   //  0: Constant	      
   //  1: Max_bValue      
   //  2: Negative_Sigma1 
   //  3: Positive_Sigma1 
   //  4: Negative_Frac   
   //  5: Positive_Frac   
   //  6: Negative_SigFrac
   //  7: Positive_SigFrac
   //  8: Constant2   
   //  9: Mean2        
   // 10: Width2

   double value = x[0];   
   double norm   = par[0];
   double arg1   = value - par[1];
   double sigma1 = par[2]; // negative sigma1
   double frac   = par[4]; // negative fraction
   double arg2   = arg1;   // same center
   double sigma2 = sigma1 * par[6]; // negative sigma2
   double norm3  = par[8];
   double arg3   = value - par[9];
   double sigma3 = par[10];
   double negconst = par[11];
  
   // bifurcate
   if (arg1 > 0) 
   {
      sigma1 = par[2 + 1]; // positive sigma1
      frac   = par[4 + 1]; // positive fraction
      sigma2 = sigma1 * par[6 + 1]; // positive sigma2
      negconst = 0;
   }
   norm -= negconst;

   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 * arg1 * arg1 / (sigma1 * sigma1) ) +
      norm * (1 - frac) * TMath::Exp( -0.5 * arg2 * arg2 / (sigma2 * sigma2)) +
      norm3 * TMath::Exp( -0.5 * arg3 * arg3 / ( sigma3 * sigma3 ) )
      + negconst;
   return fitval;
}

Double_t expPlusConst( Double_t* x, Double_t* par )
{
   // Parameters:
   // 0: Norm of exponential
   // 1: "Time constant"
   // 2: Constant offset

   double val    = x[0];
   double norm   = par[0];
   double tau    = par[1];
   double offset = par[2];

   return norm * TMath::Exp( -val/tau ) + offset;
}
