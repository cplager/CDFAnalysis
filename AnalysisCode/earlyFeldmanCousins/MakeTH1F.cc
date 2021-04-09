// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "dout.h"
#include "CLPTypeDef.h"
#include "TH1F.h"
#include "CLPHisto.h"
#include "TCanvas.h"
#include "MakeTH1F.h"
#include <stdlib.h> // for atoi and atol

using namespace std;

// static variables

TH1F* MakeTH1F (string filename, string idname, string name)
{
   TH1F* retval;
   if (name == "") 
   {
      name = idname;
   }
   int numBins;
   double lower, upper;
   DVec values;
   ifstream source(filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "file " << filename << "could not be opened" << endl;
      assert(0);
   }
   string line;
   getline (source, line);
   numBins = atoi (line.c_str());
   getline (source, line);
   lower = atof (line.c_str());
   getline (source, line);
   upper = atof (line.c_str());
   cout << "numBins " << numBins << " lower " << lower << " upper "
        << upper << endl;
   while ( getline (source, line) )
   {
      double value = atof (line.c_str());
      values.push_back(value);
   }
   if (values.size() - 2 != numBins)
   {
      cerr << "File is inconsistent" << endl;
      return NULL;
   }
   retval = new TH1F (idname.c_str(), name.c_str(), 
                      numBins, lower, upper);
   for (unsigned int loop = 0; loop < values.size(); ++loop)
   {
      retval->SetBinContent (loop, values[loop]);
   } // for loop
   return retval;
}

