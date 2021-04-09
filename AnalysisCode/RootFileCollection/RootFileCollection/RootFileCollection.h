// -*- C++ -*-

#if !defined(RootFileCollection_H)
#define RootFileCollection_H

#include <string>
#include <vector>
#include <map>
#include "TFile.h"
#include "TChain.h"

class TSam;

class RootFileCollection
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      enum CollectionType 
      {
         kFileList,
         kSam,
      };

      typedef std::vector< int>                    IntVec;
      typedef std::vector< std::string >           StringVec;
      typedef std::map< std::string, StringVec >   SSVecMap;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const RootFileCollection &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      RootFileCollection (CollectionType type = kFileList);
      virtual ~RootFileCollection();

       ////////////////
      // One Liners //
      ////////////////
      CollectionType collectionType() { return m_type; }
      bool isVerbose() { return m_verbose; }

      void setVerbose (bool verbose = true) { m_verbose = verbose; }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////

      // set the type
      void setCollectionType (CollectionType type);
      
      // Add a single file
      void addFile (std::string filename);
      // Add files from a textfile
      void addFilesFromList (std::string listname);
      // Set SAM dataset
      void setSamDataset (std::string datasetname);

      // return the next file
      virtual TFile* nextFile();

      // return the next file name (used internally and for debugging
      // purposes oly - Do not use)
      std::string _nextFileName();

      // setup this job to be 'nth' segment out of 'totalSegments'
      // Works in 'kFileList' mode only
      void setSegment (int nth, int totalSegments);

      // Resets the filelist counters, but NOT the filelist.
      // Works in 'kFileList' mode only
      virtual void reset();

      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////

      // return a TChain using a list of files
      static TChain* chainFromFile (std::string listname,
                                    std::string treeName = "TopTree");

      // return a TChain from a single filename
      static TChain* chainOneFile (std::string filename,
                                   std::string treeName = "TopTree");

      // called first
      static int _initialize();

      // returns a number using section #, pid, user ID, or whatever
      // else I can get my hands on
      static int _magicNumber();

  private:
      //////////////////////////////
      // Private Member Functions //
      //////////////////////////////

      // cleans up filenames, expanding DCAP addresses
      static std::string _cleanFilename (std::string filename);

      // splits a line into a vector of strings
      static void _splitLineIntoWords  (StringVec &retval, 
                                        std::string line,
                                        std::string match = " \t");

      // returns which port should be used.
      static std::string _portString(std::string key);

      /////////////////////////
      // Private Member Data //
      /////////////////////////

      StringVec m_filenamesVec;
      CollectionType m_type;
#ifndef __NO_TSAM__
      TSam* m_samPtr;
#endif
      TFile *m_filePtr;
      bool m_verbose;
      int m_end, m_current;

      // Static member data
      static SSVecMap sm_portMap;
      static int      sm_magicNumber;
};




#endif // RootFileCollection_H
