#include "TSystem.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "XsecComb.h"
#include "histStats.h"
#include "dout.h"
#include <math.h>
#include <iostream>
#include <assert.h>
#include <string>

using namespace std;

int main (int argc, char** argv)
{
   string filename = "cod_files/dil.cod";
   if (argc >= 2)
   {
      filename = argv[1];
   }
   CLPHisto::setNoMinInterval();
   XsecComb::initializeClass();
   XsecComb comb;
   TCanvas c1;
   comb.initializeWithFile(filename);
   comb.setFitFlags(6); // not likeli or constraint
   double center, plus, minus;
   comb.calculate(true); // calculate with errors
   comb.getChisqResults( center, plus, minus);
   cerr << "# standard: " << center << " +" << plus << " " << minus 
        << endl;
   DVec observedVec;
   observedVec.push_back( 0. );
   double start = comb.getParameter("startObs");
   double end   = comb.getParameter("endObs");
   double step  = comb.getParameter("stepObs");
   if (step < 0)
   {
      step = 1.;
   }
   assert (end > start);
   for (double observed = start; observed < end; observed += step)
   {
      observedVec[0] = observed;
      comb.setObserved( observedVec );
      //cout << comb << endl;
      comb.calculate(true); // calculate with errors
      double center, plus, minus;
      comb.getChisqResults( center, plus, minus);
      cout << center << " +" << plus << " -" << minus 
           << "  " << (plus + minus) / 2 << endl;
   }
}
