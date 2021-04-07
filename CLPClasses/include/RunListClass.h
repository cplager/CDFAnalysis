// -*- C++ -*-

#if !defined(RunListClass_H)
#define RunListClass_H

#include <string>
#include <vector>

class RunListClass
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      typedef std::vector< std::string > FileList;

      //////////////////////
      // Member Functions //
      //////////////////////
      // pass in file name
      RunListClass (const std::string &filename,
                    std::string append = "");

      // used with chain->Add() in root
      const char* filename (int index);

      // how many filenames are added here
      int size() { return m_list.size(); }

   private:
      /////////////////////////
      // Private Member Data //
      /////////////////////////
      FileList m_list;
      std::string m_append;
      static std::string m_retval;
};


#endif // RunListClass_H
