#include "TSystem.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "XsecComb.h"
#include "CLPUtils.h"
#include "dout.h"
#include <math.h>
#include <iostream>

using namespace std;

const int kTrials = 10;

int main(int argc, char** argv)
{
   int mode = XsecComb::kSingleFit;
   TString filename = "cod_files/dil.cod";
   if (argc >= 2)
   {
      filename = argv[1];
   }
   CLPHisto::setNoMinInterval();
   XsecComb::initializeClass();
   XsecComb comb;
   TCanvas c1;
   comb.initializeWithFile( (const char*) filename );
   cerr << "simple answer " << comb.smooshedXsec() << endl;
   //cout << "comb " << comb << endl << endl;
   comb.calcStatAndSyst(mode);   
   ////////////////
   // Constraint //
   ////////////////
   if (comb.useFitMethod( XsecComb::kConstraint ))
   {
      double center, plus, minus;
      comb.getCnstrntResults (center, plus, minus);
      cout << "Constraint " << center << " +" << plus << " -" << minus 
           << " : " << (plus + minus) / 2 << endl;
   }
   ///////////
   // Chisq //
   ///////////
   if (comb.useFitMethod( XsecComb::kChisq ))
   {
      double chisqstatCenter; 
      double chisqstatLower;
      double chisqstatUpper;
      comb.getChisqResults(chisqstatCenter, 
                           chisqstatUpper,
                           chisqstatLower);
      cout << "chisq stat " << chisqstatCenter << " +" 
           << chisqstatUpper
           << " -" << chisqstatLower
           << " : " << (chisqstatUpper + chisqstatLower) / 2 << endl;
   }
}
