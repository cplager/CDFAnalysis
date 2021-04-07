// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CLPTemplateMorph/CLPFunctionoid2.hh"

using namespace std;


CLPFunctionoid2::CLPFunctionoid2()
{
}

// friends
ostream& operator<< (ostream& o_stream, const CLPFunctionoid2 &rhs)
{
   return o_stream;
} 
