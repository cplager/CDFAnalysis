// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CLPTemplateFitter/BinNormFunctoid.hh"

using namespace std;

ClassImp (BinNormFunctoid)

BinNormFunctoid::BinNormFunctoid () : m_funcPtr (0)
{
}

BinNormFunctoid::BinNormFunctoid (FuncPtr ptr) : m_funcPtr (ptr)
{
}

double
BinNormFunctoid::operator() (int tempIndex, int binIndex,
                             double value, const double *paramArray) const
{
   cout << "hello" << endl;
   assert (m_funcPtr);
   return (*m_funcPtr) (tempIndex, binIndex, value, paramArray);
}


// friends
ostream& operator<< (ostream& o_stream, const BinNormFunctoid &rhs)
{
   return o_stream;
} 
