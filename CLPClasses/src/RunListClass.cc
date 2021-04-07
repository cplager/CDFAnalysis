// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "CLPClasses/RunListClass.h"

using namespace std;

string RunListClass::m_retval;


RunListClass::RunListClass (const string &filename, std::string append)
{
   m_append = append;
   ifstream source (filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "RunListClass: Couldn't open file " << filename 
           << "." << endl;
      exit(1);
   }
   string line;
   while ( getline (source, line) )
   {
      unsigned int start1 = line.find_first_not_of(" \t");
      if (string::npos == start1)
      {
         // there is nothing useful here
         // next line please
         continue;
      }
      unsigned int end1 = line.find_first_of(" \t", start1);
      // Is the first non-space character a digit
      char firstCh = line[start1];
      if ('#' == firstCh)
      {
         // this line is a comment
         continue;
      }
      string name;
      if (string::npos == end1)
      {
         // no trailing spaces
         name = line.substr( start1 );
      } else {
         // there are trailing spaces
         name = line.substr( start1, end1 - start1);
      }
      cout << "Adding file " << name << endl;
      m_list.push_back( name );
   } // while getline   
}

const char*
RunListClass::filename (int index)
{
   m_retval = m_list[index] + m_append;
   return m_retval.c_str();
}
