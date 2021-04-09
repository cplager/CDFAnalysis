// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <stdlib.h>     // for getenv
#include <unistd.h>     // for getuid, getpid
#include <sys/types.h>  // for getuid, getpid
#ifndef __NO_TSAM__
#include "TSam.hh"
#endif
#include "RootFileCollection/dout.h"
#include "RootFileCollection/RootFileCollection.h"

using namespace std;

// static member data
RootFileCollection::SSVecMap RootFileCollection::sm_portMap;
int RootFileCollection::sm_magicNumber = _magicNumber();
int initInt = RootFileCollection::_initialize();

RootFileCollection::RootFileCollection(CollectionType type)
{
   setCollectionType (type);
   m_verbose = true;
   m_end = -1;
   m_current = 0;
   m_filePtr = (TFile*) 0;
#ifndef __NO_TSAM__
   m_samPtr = (TSam*) 0;
#endif
}

RootFileCollection::~RootFileCollection()
{
   if (m_filePtr)
   {
      delete m_filePtr;
   }
#ifndef __NO_TSAM__
   // we have SAM
   if (m_samPtr)
   {
      cout << "deleteing m_samPtr" << endl;
      delete m_samPtr;
      cout << "SAM done" << endl;
   }
#endif
}

void
RootFileCollection::setCollectionType 
(RootFileCollection::CollectionType type)
{
#ifdef __NO_TSAM__
   if (kSam == type)
   {
      cerr << "RootFileCollection::setCollectionType(kSam): "
           << "Can not be called without TSAM"
           << endl;
      assert(0);
   }
#endif
   m_type = type;
}

void
RootFileCollection::addFile (string line)
{
   string filename = _cleanFilename(line);
   if (! filename.length() )
   {
      // Nothing to see here folks, keep moving.
      return;
   }   
   cout << "Adding file " << filename << endl;
   m_filenamesVec.push_back( filename );
}

void 
RootFileCollection::addFilesFromList (string listname)
{
   ifstream source (listname.c_str(), ios::in);
   if (! source.good())
   {
      cout << "Couldn't open " << listname << ".  Aborting." << endl;
      return;
   }
   string line;
   while ( getline (source, line) )
   {
      string filename = _cleanFilename(line);
      if (! filename.length() )
      {
         // Nothing to see here folks, keep moving.
         continue;
      }
      m_filenamesVec.push_back( filename );
   } // while getline
}

void
RootFileCollection::setSamDataset (string datasetname)
{
#ifdef __NO_TSAM__
   // No TSam code
   cerr << "RootFileCollection::setSamDataset: Can not be called without TSAM"
        << endl;
   assert(0);
#else
   // With TSam code
   if (m_samPtr) {
      cout << "SAM dataset already set.  Aborting." << endl;
      return;
   }
   cout << "Creating SAM project for dataset " << datasetname << endl;
   m_samPtr = new TSam( datasetname.c_str() );
#endif
}

TFile*
RootFileCollection::nextFile()
{
   // Are we using file lists or SAM
   if (kSam == m_type)
   {
#ifdef __NO_TSAM__
      cerr << "RootFileCollection::nextFile(kSam): "
           << "Can not be called without TSAM"
           << endl;
      assert(0);
      return (TFile*) 0;
#else
      // Do the right thing here
      if (!m_samPtr) m_samPtr = new TSam;
      return m_samPtr->OpenNextFile();
#endif
   }
   ////////////////
   // File Lists //
   ////////////////   
   if (m_filePtr)
   {
      delete m_filePtr;
      m_filePtr = (TFile*) 0;
   }
   string filename = _nextFileName();
   if (filename.length()) {
      m_filePtr = TFile::Open(filename.c_str());
      return m_filePtr;
   }
   return (TFile*) 0;
}

string
RootFileCollection::_nextFileName()
{
   const string kEmpty = "";
   if (m_end < 0)
   {
      m_end = (int) m_filenamesVec.size();
   }
   // are there any more?
   if (m_current >= m_end)
   {
      cout << "done" << endl;
      // Nope.
      return kEmpty;
   }
   if (m_current >= (int) m_filenamesVec.size())
   {
      cerr << "m_current is too big.  Something is strange." << endl;
      return kEmpty;
   }
   return m_filenamesVec[m_current++];
}

void
RootFileCollection::setSegment (int thisSegment, int totalSegments)
{
   // You can't do this if you are in SAM mode
   assert (kSam != m_type);

   // Make sure the segment numbers make sense
   assert (thisSegment > 0 && thisSegment <= totalSegments);

   // The Perl code:
   // my $entries    = @list;
   // my $perSegment = int ($entries / $totalSegments);
   // my $extra      = $entries % $totalSegments;
   // --$thisSegment; # we want 0..n-1 not 1..n
   // my $start = $perSegment * $thisSegment;
   // my $num   = $perSegment - 1;
   // if ($thisSegment < $extra) {
   //    $start += $thisSegment;
   //    ++$num;
   // } else {
   //    $start += $extra;
   // };
   // my $end = $start + $num;
   int entries = (int) m_filenamesVec.size();
   int perSegment = entries / totalSegments;
   int extra = entries % totalSegments;
   --thisSegment; // we want 0..n-1, not 1..n.
   m_current = perSegment * thisSegment;
   int num = perSegment - 1;
   if (thisSegment < extra)
   {
      m_current += thisSegment;
      ++num;
   } else 
   {
      m_current += extra;
   }
   m_end = m_current + num + 1;
}

void
RootFileCollection::reset()
{
   // You can't do this if you are in SAM mode
   assert (kSam != m_type);
   if (m_filePtr)
   {
      delete m_filePtr;
      m_filePtr = (TFile*) 0;
   }
   m_end = -1;
   m_current = 0;
}

/////////////////////////////
// Static Member Functions //
/////////////////////////////
TChain*
RootFileCollection::chainFromFile (string listname, string treename)
{
   //TAuthenticate::SetGlobalUser("cdfdata");
   
   TChain *chainPtr =  new TChain(treename.c_str());
   vector< string > runList;
   string line;
   ifstream source (listname.c_str(), ios::in);
   while ( getline (source, line) )
   {
      string filename = _cleanFilename(line);
      if (! filename.length() )
      {
         // Nothing to see here folks, keep moving.
         continue;
      }
      cout << "Adding file " << filename << endl;
      runList.push_back( filename );
   } // while getline
   int size = (int) runList.size();
   cout << "size " << size << endl;
   for (int loop = 0; loop < size; ++loop)
   {
      chainPtr->AddFile( runList[loop].c_str(), TChain::kBigNumber );
   } // for loop

   return chainPtr;   
}

TChain*
RootFileCollection::chainOneFile (string line, string treename)
{
   //TAuthenticate::SetGlobalUser("cdfdata");
   
   TChain *chainPtr =  new TChain(treename.c_str());
   string filename = _cleanFilename(line);
   if (! filename.length() )
   {
      // Nothing to see here folks, keep moving.
      return chainPtr;
   }

   chainPtr->AddFile( filename.c_str(), TChain::kBigNumber );

   return chainPtr;   
}

string
RootFileCollection::_cleanFilename(string filename)
{
   const string kEmpty = "";
   unsigned int start1 = filename.find_first_not_of(" \t");
   // Is there anything here at all?
   if (string::npos == start1)
   {
      // there is nothing useful here
      // next line please
      return kEmpty;
   }
   // get the last non-space
   unsigned int end1 = filename.find_last_not_of(" \t") + 1;
   // Is the first non-space character a '#'
   char firstCh = filename[start1];
   if ('#' == firstCh)
   {
      // this line is a comment
      return kEmpty;
   }
   string retval;
   // Trailing spaces?
   if (string::npos == end1)
   {
      // no trailing spaces
      retval = filename.substr( start1 );
   } else {
      // there are trailing spaces
      retval = filename.substr( start1, end1 - start1);
   }

   // O.k., we've got our filename.  If it has no spaces in it, it is
   // ready to be returned for use.  If it has spaces, we've got more
   // work to do.
   if (string::npos == retval.find_first_of(" \t"))
   {
      // send it back
      return retval;
   } // if no spaces
   
   // So, we've got a filename with spaces.  Lets see what we can do.
   StringVec words;
   _splitLineIntoWords (words, retval);
   // we want 3 words
   int numWords = (int) words.size();
   if (3 != numWords)
   {
      cerr << "Can't initialize a file with: " << retval
           << ". Skipping" << endl;
      return kEmpty;
   }
   string directory = words[2];
   string key = words[0] + "_" + words[1];
   string port = _portString (key);
   if (! port.length())
   {
      cerr << "Could not find available port for '" << key << "'.  Skipping"
           << endl;
      return kEmpty;
   }
   return Form ("%s%s", port.c_str(), directory.c_str());
}

void
RootFileCollection::_splitLineIntoWords  (StringVec &retval, 
                                          string line,
                                          string match)
{
   retval.clear();
   line += " "; // get last word of line
   unsigned int last = string::npos;
   unsigned int current = line.find_first_of(match);
   while (string::npos != current)
   {
      unsigned int pos;
      if (string::npos != last)
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
      current = line.find_first_of(match, current + 1);
   } // while we're finding spaces
}

string
RootFileCollection::_portString(string key)
{
   SSVecMap::iterator iter = sm_portMap.find(key);
   if (sm_portMap.end() == iter)
   {
      // couldn't find the right entry
      return 0;
   }
   int size = (int) iter->second.size();
   if (! size)
   {
      // no available ports
      return 0;
   }
   int index = sm_magicNumber % size;
   return iter->second[index];
}

int
RootFileCollection::_magicNumber()
{
   // Get job ID
   int jobid = 0;
   char *jobPtr = getenv ("CAF_JID");
   if (jobPtr)
   {
      jobid = atoi (jobPtr);
   }
   // if no jobid, use UID instead
   if (! jobid)
   {
      jobid = getuid();
   }
   // Get section number
   int section = 0;
   char *sectionPtr = getenv ("CAF_SECTION");
   if (sectionPtr)
   {
      section = atoi (sectionPtr);
   }
   // if no section number, use PID instead
   if (! section)
   {
      section = getpid();
   }
   return jobid + section;
}

int
RootFileCollection::_initialize()
{
   // Hard code the ports for fcdfrdc3 so we have default values.
   // This can be overwritten by a file (eventually).
   StringVec ports;
   for (int port = 22125; port <= 22137; ++port)
   {
      ports.push_back( Form ("dcap://fcdfrdc3.fnal.gov:%d", port) );
   }  // for port
   string name = "dcap_fcdfrdc3";
   sm_portMap [name] = ports;
   // dcap://fcdfrdc3.fnal.gov:22127/pnfs/diskpool/test/largefile
   return 0;
}

// friends
ostream& operator<< (ostream& o_stream, const RootFileCollection &rhs)
{
   int size = (int) rhs.m_filenamesVec.size();
   o_stream << "Number of files: " << size << endl;
   for (int loop = 0; loop < size; ++loop)
   {
      cout << setw(3) << loop + 1 << ") "
           << rhs.m_filenamesVec[loop] << endl;
   } // for loop
   return o_stream;
} 
