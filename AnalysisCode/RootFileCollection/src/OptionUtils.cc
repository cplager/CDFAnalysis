// -*- C++ -*-
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>

#include "RootFileCollection/OptionUtils.h"
#include "RootFileCollection/dout.h"

using namespace std;

// Variable definitions
OptionUtils::OptionMap  OptionUtils::ns_intMap;
OptionUtils::OptionMap  OptionUtils::ns_doubleMap;
OptionUtils::OptionMap  OptionUtils::ns_stringMap;
OptionUtils::OptionMap  OptionUtils::ns_boolMap;
OptionUtils::StrBoolMap OptionUtils::ns_variableModifiedMap;
OptionUtils::StrStrMap  OptionUtils::ns_variableDescriptionMap;

void 
OptionUtils::parseArguments (int argc, char** argv)
{
   bool printOptions = false;
   for (int loop = 1; loop < argc; ++loop)
   {
      string arg = argv[loop];
      unsigned int where = arg.find_first_of("=");
      if (string::npos != where)
      {
         if (! setVariableFromString (arg) )
         {
            cerr << "Don't understand: " << arg << endl;
            exit(0);
         }
         continue;
      } // tag=value strings
      else if (arg.at(0) == '-')
      {
         unsigned int where = arg.find_first_not_of("-");
         if (string::npos == where)
         {
            // a poorly formed option
            continue;
         }
         lowercaseString (arg);
         char first = arg.at (where);
         // Print the values
         if ('p' == first)
         {
            printOptions = true;
            continue;
         }
         // Exit after printing values
         if ('e' == first || 'h' == first)
         {
            printOptionValues();
            exit (0);
         }
      } // -arg strings
      cerr << "Don't understand: " << arg << endl;
      exit(0);
   } // for loop
   if (printOptions)
   {
      printOptionValues();
   } // if printOptions
}

string
OptionUtils::removeEnding (const string &input, const string &ending)
{
   unsigned int position = input.rfind(ending);
   if (input.length() - ending.length() == position)
   {
      // we've got it
      return input.substr(0, position);
   }
   // If we're still here, it wasn't there
   return input;
}

void
OptionUtils::printOptionValues()
{
   cout << "------------------------------------------------------------------" 
        << endl << "Option Values:" << endl;
   // Print the bools first
   if (ns_boolMap.size())
   {
      cout << "  Bool options:" << endl;
   }
   for (OptionMapIter iter = ns_boolMap.begin(); 
       ns_boolMap.end() != iter; 
       ++iter) 
   {
      string &description = ns_variableDescriptionMap[ iter->first ];
      bool *valuePtr = (bool *) iter->second;
      cout << "    " << iter->first << " = ";
      if (*valuePtr)
      {
         cout << "true";
      } else {
         cout << "false";
      }
      if (description.length())
      {
         cout << " - " << description;
      }
      cout << endl;
   } // for iter

   // Print the ints next
   if (ns_intMap.size())
   {
      cout << "  Int options:" << endl;
   }
   for (OptionMapIter iter = ns_intMap.begin(); 
       ns_intMap.end() != iter; 
       ++iter) 
   {
      string &description = ns_variableDescriptionMap[ iter->first ];
      int *valuePtr = (int *)iter->second;
      cout << "    " << iter->first << " = " << *valuePtr;
      if (description.length())
      {
         cout << " - " << description;
      }
      cout << endl;
   } // for iter

   // Print the doubles next
   if (ns_doubleMap.size())
   {
      cout << "  Double options:" << endl;
   }
   for (OptionMapIter iter = ns_doubleMap.begin(); 
       ns_doubleMap.end() != iter; 
       ++iter) 
   {
      string &description = ns_variableDescriptionMap[ iter->first ];
      double *valuePtr = (double *)iter->second;
      cout << "    " << iter->first << " = " << *valuePtr;
      if (description.length())
      {
         cout << " - " << description;
      }
      cout << endl;
   } // for iter

   // Print the strings next
   if (ns_stringMap.size())
   {
      cout << "  String options:" << endl;
   }
   for (OptionMapIter iter = ns_stringMap.begin(); 
       ns_stringMap.end() != iter; 
       ++iter) 
   {
      string &description = ns_variableDescriptionMap[ iter->first ];
      string *valuePtr = (string *) iter->second;
      cout << "    " << iter->first << " = '" << *valuePtr << "'";
      if (description.length())
      {
         cout << " - " << description;
      }
      cout << endl;
   } // for iter
   cout << "------------------------------------------------------------------" 
        << endl;
}

void 
OptionUtils::lowercaseString(string& arg)
{
   // assumes 'toLower(ch)' modifies ch
   std::for_each (arg.begin(), arg.end(), OptionUtils::toLower);
   // // assumes 'toLower(ch)' returns the lower case char
   // std::transform (arg.begin(), arg.end(), arg.begin(), 
   //                 OptionUtils::toLower);
}

char 
OptionUtils::toLower (char& ch)
{
   ch = tolower (ch);
   return ch;
}

void
OptionUtils::addOptionKey (string key, int &variable, 
                           const string &description)
{
   lowercaseString (key);
   ns_intMap[key] = (void*) &variable;
   ns_variableModifiedMap[key] = false;
   ns_variableDescriptionMap[key] = description;
}

void
OptionUtils::addOptionKey (string key, double &variable, 
                           const string &description)
{
   lowercaseString (key);
   ns_doubleMap[key] = (void*) &variable;
   ns_variableModifiedMap[key] = false;
   ns_variableDescriptionMap[key] = description;
}

void
OptionUtils::addOptionKey (string key, bool &variable, 
                           const string &description)
{
   lowercaseString (key);
   ns_boolMap[key] = (void*) &variable;
   ns_variableModifiedMap[key] = false;
   ns_variableDescriptionMap[key] = description;
}

void
OptionUtils::addOptionKey (string key, string &variable, 
                           const string &description)
{
   lowercaseString (key);
   ns_stringMap[key] = (void*) &variable;
   ns_variableModifiedMap[key] = false;
   ns_variableDescriptionMap[key] = description;
}

bool
OptionUtils::valueHasBeenModified (const string &key)
{
   StrBoolMapConstIter iter = ns_variableModifiedMap.find (key);
   if (ns_variableModifiedMap.end() == iter)
   {
      // Not found.  Not a valid option
      cerr << "OptionUtils::valueHasBeenModfied () Error: '" 
           << key << "' is not a valid key." << endl;
      return false;
   }
   return iter->second;
}

bool 
OptionUtils::setVariableFromString (const string &arg,
                                    bool dontOverrideChange,
                                    int offset)
{
   unsigned int where = arg.find_first_of("=", offset + 1);
   string varname = arg.substr (offset, where - offset);
   string value   = arg.substr (where + 1);
   lowercaseString (varname);
   // if 'dontOverrideChange' is set, then we are being asked to NOT
   // change any variables that have already been changed.
   if (dontOverrideChange && valueHasBeenModified (varname) )
   {
      // don't go any further
      return true;
   }
   // doubles
   OptionMapIter iter = ns_doubleMap.find(varname);
   if (ns_doubleMap.end() != iter)
   {
      // we found it
      double *doublePtr = (double*) iter->second;
      *doublePtr = atof(value.c_str());
      ns_variableModifiedMap[varname] = true;
      return true;
   }
   // integers
   iter = ns_intMap.find(varname);
   if (ns_intMap.end() != iter)
   {
      // we found it
      int *intPtr = (int*) iter->second;
      // use 'atof' instead of 'atoi' to get scientific notation
      *intPtr = (int) atof( value.c_str() );
      ns_variableModifiedMap[varname] = true;
      return true;
   }
   // strings
   iter = ns_stringMap.find(varname);
   if (ns_stringMap.end() != iter)
   {
      // we found it
      string *stringPtr = (string*) iter->second;
      *stringPtr = value;
      ns_variableModifiedMap[varname] = true;
      return true;
   }
   // bools
   iter = ns_boolMap.find(varname);
   if (ns_boolMap.end() != iter)
   {
      // we found it
      bool *boolPtr = (bool*) iter->second;
      int val = atoi(value.c_str());
      if (val)
      {
         *boolPtr = true;
      } else {
         *boolPtr = false;
      }               
      ns_variableModifiedMap[varname] = true;
      return true;
   }
   // We didn't find your variable.
   return false;
}

bool
OptionUtils::setVariablesFromFile (const string &filename)
{
   ifstream source(filename.c_str(), ios::in);
   if (! source)
   {
      cerr << "file " << filename << "could not be opened" << endl;
      return false;
   }
   string line;
   while (getline (source, line))
   {
      // find the first nonspace
      unsigned int where = line.find_first_not_of(" \t");
      if (string::npos == where)
      {
         // no non-spaces
         continue;
      } 
      char first = line.at (where);
      if ('-' == first)
      {
         where =  line.find_first_not_of(" \t", where + 1);
         if (string::npos == where)
         {
            // no non-spaces
            continue;
         } 
         setVariableFromString (line, true, where);
      }
   } // while getline
   return true;
}
