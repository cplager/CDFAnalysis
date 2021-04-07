// -*- C++ -*-
// (compile) g++ # -o ##
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CLPClasses/CLPNamespace.h"

using namespace std;

// Variable defs
CLPNamespace::DataType   CLPNamespace::ns_dataType = CLPNamespace::kData;
CLPNamespace::NtupleType CLPNamespace::ns_ntupleType 
= CLPNamespace::kTopNtuple;
bool     CLPNamespace::ns_debug = false;
int      CLPNamespace::ns_userInts[CLPNamespace::kNumUserInts]; 
double   CLPNamespace::ns_userFloats[CLPNamespace::kNumUserFloats];
string   CLPNamespace::ns_userStrings[CLPNamespace::kNumUserStrings];
Long64_t CLPNamespace::ns_userBits;
// public static member data
int   CLPNamespace::ns_runNumber     = 200000;
int   CLPNamespace::ns_eventNumber   = 0;
int   CLPNamespace::ns_veto_jetCount = 4;
float CLPNamespace::ns_veto_jetEt    = 0.;


bool CLPNamespace::ns_ignoreRunNumber = false;
CLPNamespace::CLPOutputMode CLPNamespace::ns_outputMode 
= CLPNamespace::kShortOutput;
double CLPNamespace::ns_topMass      = 120. * GEV;
double CLPNamespace::ns_WMass        =  60. * GEV;
double CLPNamespace::ns_lowerTopMass = 170. * GEV;
double CLPNamespace::ns_upperTopMass = 180. * GEV;
CLPNamespace::OptMethodType CLPNamespace::ns_method = CLPNamespace::kWidth;
// We'll want to look at changing these:
double CLPNamespace::ns_W_width   = 10. * GEV;
double CLPNamespace::ns_tWb_width = 12. * GEV;
double CLPNamespace::ns_tZc_width = 12. * GEV;
// we want the error FOM to scale with our widths.  Since the W width
// is the smallest, we use it.
double CLPNamespace::ns_errorFOM  = 199.9 * GEV / ns_W_width;
bool CLPNamespace::ns_fixWmass    = true;

// Will be set by CollectionAndViewTypes.hh or TopNtupleDecoder
int CLPNamespace::ns_ELETYPE_DefEm           = -1;  
int CLPNamespace::ns_ELETYPE_Phoenix         = -1;
int CLPNamespace::ns_MUOTYPE_DefMuon         = -1;        
int CLPNamespace::ns_MUOTYPE_DefStublessMuon = -1;
int CLPNamespace::ns_JETTYPE_Jetclu_04_LJ    = -1;   
int CLPNamespace::ns_JETTYPE_Jetclu_07_LJ    = -1;      
int CLPNamespace::ns_OTRKTYPE_DefTrack       = -1;   
int CLPNamespace::ns_OTRKTYPE_TrackLepton    = -1;
