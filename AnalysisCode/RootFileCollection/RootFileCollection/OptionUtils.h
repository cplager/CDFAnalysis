// -*- C++ -*-

#if !defined(OptionUtils_H)
#define OptionUtils_H

#include <map>
#include <string>

namespace OptionUtils
{
   /////////////////////
   // /////////////// //
   // // Constants // //
   // /////////////// //
   /////////////////////

   // typedefs
   typedef std::map< std::string, void* >       OptionMap;
   typedef std::map< std::string, bool  >       StrBoolMap;
   typedef std::map< std::string, std::string > StrStrMap;
   typedef OptionMap::iterator                  OptionMapIter;
   typedef StrBoolMap::const_iterator           StrBoolMapConstIter;

   ///////////////
   // Functions //
   ///////////////

   // parse the command line arguments
   extern void parseArguments (int argc, char** argv);

   // remove an ending (e.g., '.root') from a string
   extern std::string removeEnding (const std::string &input, 
                                    const std::string &ending);   

   // print out all of the variables hooked up
   extern void printOptionValues();

   // converts a string to lower case characters
   extern void lowercaseString(std::string &arg); 

   // converts a single character to lower case
   extern char toLower (char &ch);

   // add variable to option maps
   extern void addOptionKey (std::string key, int         &variable, 
                             const std::string &description = "");
   extern void addOptionKey (std::string key, double      &variable, 
                             const std::string &description = "");
   extern void addOptionKey (std::string key, bool        &variable, 
                             const std::string &description = "");
   extern void addOptionKey (std::string key, std::string &variable, 
                             const std::string &description = "");

   // returns true if a variable has been modified from the command
   // line.
   extern bool valueHasBeenModified (const std::string &key);

   // Sets a variable 'varname' to a 'value' from a string
   // 'varname=value'.  If 'dontOverrideChange' is set true, then the
   // function will NOT set a variable that has been already set.
   // This will allow you to read in a file to set most variables and
   // still be allowed to make changes from the command line.  If you
   // want to ignore the first 'n' characters, simply set 'offset=n'.
   extern bool setVariableFromString (const std::string &arg,
                                      bool dontOverrideChange = false,
                                      int offset = 0);

   // sets variable options from file where lines are formatted as
   // - var=value
   extern bool setVariablesFromFile (const std::string &filename);

   /////////////////////
   // /////////////// //
   // // Variables // //
   // /////////////// //
   /////////////////////

   extern OptionMap  ns_intMap;
   extern OptionMap  ns_doubleMap;
   extern OptionMap  ns_stringMap;
   extern OptionMap  ns_boolMap;
   extern StrBoolMap ns_variableModifiedMap;
   extern StrStrMap  ns_variableDescriptionMap;

};

#endif // OptionUtils_H
