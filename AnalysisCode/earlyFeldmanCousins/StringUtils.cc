#include "StringUtils.h"
using namespace std;

void split (SVec &retval, std::string line)
{
   retval.clear();
   // get rid of any '#' comments
   unsigned int hashPos = line.find("#");
   if (hashPos != string::npos) {
      if (0 == hashPos) {
         // nothing here but the #
         // Don't bother continuing
         return;
      }
      line = line.substr(0, hashPos - 1);
   }
   line += " "; // get last word of line
   unsigned int last = string::npos;
   unsigned int current = line.find(" ");
   while (current != string::npos)
   {
      unsigned int pos;
      if (last != string::npos)
      {
         pos = last + 1;
      } else {
         pos = 0;
      }
      string part = line.substr( pos, current - last - 1);
      // don't bother adding 0 length strings
      if (part.length()) 
      {
         retval.push_back(part);
      }
      last = current;
      current = line.find(" ", current + 1);
   } 
}

string toLowerCase(string const &word)
{
   string retval;
   for (unsigned int loop = 0; loop < word.size(); ++loop)
   {
      int letter = (int) word.at(loop);
      retval += (char) tolower(letter);
   } // for loop
   return retval;
}


