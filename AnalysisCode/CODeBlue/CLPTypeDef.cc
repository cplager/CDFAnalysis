// -*- C++ -*-

#include "CLPTypeDef.h"
//#include <iostream>

using namespace std;

ostream& operator<< (ostream& o_stream, const DVec &rhs)
{
   unsigned int size = rhs.size();
   o_stream << "(" << (int) size << ")";
   for (unsigned loop = 0; loop < size; ++loop)
   {
      o_stream << " " << rhs[loop];
   } // for loop
   return o_stream;
} 

ostream& operator<< (ostream& o_stream, const IVec &rhs)
{
   unsigned int size = rhs.size();
   o_stream << "(" << (int) size << ")";
   for (unsigned loop = 0; loop < size; ++loop)
   {
      o_stream << " " << rhs[loop];
   } // for loop
   return o_stream;
} 

ostream& operator<< (ostream& o_stream, const SVec &rhs)
{
   unsigned int size = rhs.size();
   o_stream << "(" << size << ")";
   for (unsigned loop = 0; loop < size; ++loop)
   {
      o_stream << " " << rhs[loop];
   } // for loop
   return o_stream;
} 

