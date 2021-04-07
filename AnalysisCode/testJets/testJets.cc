// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"

#include "time.h"
#include "CLPClasses/dout.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include "CLPClasses/CLPClasses.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;

// (ugh..) Global Variables
int gWhich         = 11;
int gNumWanted     = 10;


// forward declarations
void initialize();

int main(int argc, char** argv)
{
   cout << "Hi Kate" << endl;
   CLPJet jet;
   jet.setMassiveMom4Vec (3, 4, 0, 6, 6);
   jet.setMasslessMom4Vec (5, 0, PI/6);
   cout << jet.mom4Vec() << endl;
   cout << jet.mom4Vec (CLPJet::kMasslessFourMomPtr) << endl;
}

void initialize()
{
   ns_IntMap["which"]          = (void*) &gWhich;
   ns_IntMap["numwanted"]      = (void*) &gNumWanted;
}

