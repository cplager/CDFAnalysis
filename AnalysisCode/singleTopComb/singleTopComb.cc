// -*- C++ -*-

// Standard include files
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <algorithm>

// Root include files
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom3.h"

// CLP include files
#include "RootFileCollection/OptionUtils.h"
#include "CLPBlue/CLPUtils.hh"
#include "CLPBlue/dout.hh"
#include "EventFlavorContainer.hh"
#include "dumpSTL.icc"

using namespace std;
using namespace OptionUtils;

// forward declarations
void localInitialize();


// Global variables (ugh!) for command line options
// Number of PEs (numpes)
int  gNumPEs = 10;
// filename
string gFilename = "test2.txt";

// Switch to tell us which sets of files to load (which)

int main (int argc, char** argv)
{
   // TRandom is lousy.  Use TRandom3 instead.
   delete gRandom;
   gRandom = new TRandom3;
   localInitialize();     // hook up local options
   OptionUtils::parseArguments (argc, argv);

   EventFlavorContainer::SEFCMap flavorMap;
   flavorMap["wlight"] = EventFlavorContainer ("wlight");

   // load in a file (or muliple files).
   flavorMap["wlight"].readFile (gFilename);
   // When you are finished loading in files, tell it to "renormalize"
   flavorMap["wlight"].renormalize();
   // you probably don't want to do this next step for large files.
   cout << flavorMap["wlight"] << endl;


   // Since the nameIndexMap is sorted alphabetically and all flavors
   // (e.g., wlight, wbottom, etc.) should all have the same "types".
   // You will need to check this explicitly (below).  
   EventFlavorContainer::SIMap nameIndexMap;
   flavorMap["wlight"].mapNames (nameIndexMap);
   int j2t2l0 = EventFlavorContainer::nameIndex ("j2t2l0", nameIndexMap);
   int j2t2l1 = EventFlavorContainer::nameIndex ("j2t2l1", nameIndexMap);
   int j2t2l2 = EventFlavorContainer::nameIndex ("j2t2l2", nameIndexMap);
   int j2t2l3 = EventFlavorContainer::nameIndex ("j2t2l3", nameIndexMap);
   int j2t2l4 = EventFlavorContainer::nameIndex ("j2t2l4", nameIndexMap);

   
   cout << "j2t2l0 " << j2t2l0 << endl
        << "j2t2l1 " << j2t2l1 << endl
        << "j2t2l2 " << j2t2l2 << endl
        << "j2t2l3 " << j2t2l3 << endl
        << "j2t2l4 " << j2t2l4 << endl;

   return 0;

   const int kMaxNumGroups = 4;
   EventFlavorContainer::IVec wlightNumVec (kMaxNumGroups, 0);
   // Tell it for this PE how many events you want in each sample
   wlightNumVec.at(j2t2l0) = 1000000;
   

   //////////////
   // Throw PE //
   //////////////

   // Since I don't fit, I just print them out.
   cout << "My first PE :-)" << endl;
   // here's where the results of the PE will be stored.
   EventClass::Vec eventsVec;
   // get wlight
   flavorMap["wlight"].getRandomEvents (eventsVec, wlightNumVec);
   // get any other flavors you want.  You'll use the same
   // 'eventsVec', but a different NumVec.
   //dumpSTLendl (eventsVec);
   TH1F *pseudoPtr = new TH1F ("pseudo", "pseudo", 45, -0.5, 44.5);
   EventClass::fillHist (eventsVec, pseudoPtr, 0);
   // for the ME with the map< string, TH1F* >
   // int MEindex=1
   // EventClass.fillHist (eventsVec, mapIter->second, MEindex, mapIter->first);
   TCanvas c1;
   pseudoPtr->Draw();
   c1.Print ("pseudo.eps");

   ////////////////////
   // Make Templates //
   ////////////////////
   TH1F *templatePtr = new TH1F ("template", "template", 45, -0.5, 44.5);
   flavorMap["wlight"].fillHist (templatePtr, 0);
   // for the ME with the map< string, TH1F* >
   // flavorMap["wlight"].fillHist (mapIter->second, 0, mapIter->first);
   templatePtr->Draw();
   //templatePtr->Print("range");
   c1.Print ("template.eps");

   /////////////////////////////
   // Check Maps are the Same //
   /////////////////////////////
   // // How to check that different flavors have the same nameIndexMap.
   // 
   // // lets check to make sure I can do '==' checks on maps
   // EventFlavorContainer::SIMap one, two;
   // flavorMap["wlight"].mapNames (one);
   // //one["j2t2l4"]=21; // make it not match on purpose
   // flavorMap["wlight"].mapNames (two);
   // if (one == two) 
   //    cout << "good" << endl;
   // else 
   //    cout << "bad" << endl;
}

void localInitialize()
{
   // option name is case insensitive.  Capitalization of the strings
   // is for readability only.
   addOptionKey ("numpes",     gNumPEs,
                 "Number of PEs");
   addOptionKey ("filename",   gFilename);
}
