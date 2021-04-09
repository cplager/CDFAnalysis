// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include "RootFileCollection/TreeFileInterface.h"
#include "RootFileCollection/dout.h"

using namespace std;

TreeFileInterface::TreeFileInterface(TString treename) : 
   m_treename (treename)
{
   _initialize();
}

TreeFileInterface::TreeFileInterface() : 
   m_treename ("you_forgot_to_set_the_tree_name")
{
   _initialize();
}

TreeFileInterface::TreeFileInterface(TString treename, bool decode) : 
   m_treename (treename)
{
   _initialize();
   m_decodeTopTree = decode;
}

TreeFileInterface::~TreeFileInterface()
{
   // print out a summary if we have opened at least one file
   if (m_firstFile)
   {
      // there's nothing to do here
      return;
   }
   cout << "TreeFileInterface Summary: Ran over " 
        << m_entriesSeen << " total events." << endl;
   // should we write out the clone tree
   if ( ( (m_makeClone && m_cloneTreePtr) ||
          m_objPtrVec.size() || m_treePtrMap.size( ) )
       && ! m_wroteCloneTree)
   {
      cout << "Writing final clone tree." << endl;
      writeFinalCloneTree();
   }
}

// Unload as much as possible from the constructors so we only have
// one routine to update with new variables
void
TreeFileInterface::_initialize()
{
   m_eventsSeen               = 0;
   m_previousEventsSeen       = 0;
   m_totalEventsPreviousFiles = 0;       
   m_eventsWanted             = 0;
   m_entriesThisFile          = 0;
   m_entriesSeen              = 0;
   m_entriesSeenThisFile      = 0;
   m_1inEveryN                = 0;
   m_currentEntry             = 0;
   m_allDone                  = false;
   m_newFile                  = false;
   m_makeClone                = false;
   m_getEntireTree            = false;
   m_decodeTopTree            = false;
   m_firstFile                = true;
   m_wroteCloneTree           = false;
   // Null poiinters do not need casting.
   m_currentFilePtr           = 0;
   m_treePtr                  = 0;
   m_cloneTreePtr             = 0;
   m_outputDirectory          = 0;
   m_decoderPtr               = 0;
}

bool
TreeFileInterface::getNextEvent()
{
   return _internalGetNextEvent();
}

bool
TreeFileInterface::skipNevents(int numEvents)
{
   //dout << "nE " <<  numEvents << endl;
   // first, lets check to see if we've hooked up any file yet
   if (! m_currentFilePtr)
   {
      //dout << "opening new file" << endl;
      // Try and hook up the first file
      if ( ! _hookupNextFile() )
      {
         // Whoops, even the first file isn't there
         return false;
      }
   }
   // Now let's check that the requested number of events isn't stupid
   if (numEvents <= 0)
   {
      cerr << "Error: TreeFileInterface::skipNevents(): You can not pick 0"
           << endl << "or a negative number." << endl;
      return false;
   }
   --numEvents;
   // OK, if we're here then we know we have a file hooked up.  We
   // need to keep opening files (and keeping track of the numbers of
   // events in each file) until we can jump to a specific event in
   // this file.
   int entriesLeft = m_entriesThisFile - m_currentEntry;
   //dout << "eL " << entriesLeft << endl;
   while (numEvents > entriesLeft)
   {
      numEvents -= entriesLeft;
      //dout << "nE " << numEvents << endl;
      if ( ! _hookupNextFile() )
      {
         // no more files, we've skipped to the end
         return false;
      }
      entriesLeft = m_entriesThisFile - m_currentEntry;
   } // while
   // It's ok if this '-1'
   m_currentEntry += numEvents;
   //dout << "cE " << m_currentEntry << endl;
   return _internalGetNextEvent (true);
}

void
TreeFileInterface::addTcaBranch (const string &branchName, string className)
{
   if (! className.length())
   {
      className = branchName;
   }
   // does this branch exist already
   if (m_tcaPtrMap.end() != m_tcaPtrMap.find (branchName))
   {
      // already exists.  Leave it alone
      cerr << "TreeFileInterface::addTcaBranch(): " << branchName 
           << "already exists." << endl;
      return;
   }
   m_tcaPtrMap [branchName] = new TClonesArray (className.c_str(), 1);
   m_retrievedBranchMap [branchName] = false;
   _addToInputBranchSet (branchName);
}

void
TreeFileInterface::removeTcaBranch (const string &branchName)
{
   TClonesPtrMapIter iter = m_tcaPtrMap.find (branchName);
   if (m_tcaPtrMap.end() == iter)
   {
      // nothing to delete
      cerr << "TreeFileInterface::removeTcaBranch(): " << branchName 
           << "does not exists." << endl;
      return;
   }
   delete iter->second;
   m_tcaPtrMap.erase (iter);
   // take care of m_retrievedBranchMap as well
   NameBoolMapIter nbIter = m_retrievedBranchMap.find (branchName);
   if (m_retrievedBranchMap.end() != nbIter)
   {
      m_retrievedBranchMap.erase (nbIter);
   }
   _removeFromInputBranchSet (branchName);
}

bool
TreeFileInterface::getBranchStatus (const string &branchName) const
{
   // We should decide if we want to print out a warning if the branch
   // you are asking for is not one you have registered.  But in any
   // case, I can see the utility of asking about the existence of a
   // branch that you have no intention of using (or wanting to skim),
   // so I'll return the answer regardless.
   return m_treePtr->GetBranchStatus (branchName.c_str());
}

TClonesArray*
TreeFileInterface::tcaPtr (const string &branchName)
{
   NameBoolMapIter nbIter = m_retrievedBranchMap.find (branchName);
   TClonesPtrMapIter iter = m_tcaPtrMap.find (branchName);
   if (m_tcaPtrMap.end() == iter ||
       m_retrievedBranchMap.end() == nbIter)
   {
      // Uh oh
      cerr << "TreeFileInterface::tcaPtr(): '" << branchName
           << "' does not exist.  Make sure you register branch by using"
           << " 'addTcaBranch()'." << endl;
      _abort (__FILE__, __LINE__);
      return 0;
   }
   // Have we gotten this already (i.e., Have we gotten this branch
   // already or have we gotten the entire tree)?
   if (!nbIter->second && ! m_getEntireTree)
   {
      // Haven't gotten this yet.  Get it now.
      TBranch *branchPtr = m_treePtr->GetBranch( branchName.c_str() );
      if (branchPtr)
      {
         branchPtr->GetEntry (m_loadTreeFlag);
         // mark it as retrieved
         m_retrievedBranchMap[branchName] = true;      
      } else {
         cerr << "TreeFileInterface::tcaPtr(): GetEntry failed for '"
              << branchName << "' branch." << endl;
      }
   } // getting the branch
   return iter->second;
}

TObject*
TreeFileInterface::At (const string &branchName, int which)
{
   TClonesArray *tPtr = tcaPtr (branchName);
   if (! tPtr)
   {
      // Should never happen
      cerr << "TreeFileInterface::At(): '" << branchName
           << "' does not exist.  Make sure you register branch by using"
           << " 'addTcaBranch()'." << endl;
      _abort (__FILE__, __LINE__);
      return 0;
   }
   int howMany = tPtr->GetEntries();
   if (! howMany)
   {
      cerr << "TreeFileInterface::At(): '" << branchName
           << "' is empty. " << endl;
      _abort (__FILE__, __LINE__);
      return 0;
   }
   if (which < 0 || which >= howMany)
   {
      cerr << "TreeFileInterface::At(): '" << branchName
           << "' only has " << howMany << " entries (you asked for the " 
           << which + 1
           << " entry). " << endl;
      _abort (__FILE__, __LINE__);
      return 0;
   }
   return tPtr->At (which);
}

Int_t
TreeFileInterface::GetEntries (const string &branchName)
{
   TClonesArray *tPtr = tcaPtr (branchName);
   if (! tPtr)
   {
      // Should never happen
      cerr << "TreeFileInterface::At(): '" << branchName
           << "' does not exist. " << endl;
      _abort (__FILE__, __LINE__);
      return 0;
   }
   return tPtr->GetEntries();
}

bool
TreeFileInterface::isBranchOK (const string &branchName)
{
   NameBoolMapIter nbIter = m_retrievedBranchMap.find (branchName);
   if (m_retrievedBranchMap.end() == nbIter)
   {
      cout << "TreeInterface::isBranchOK(): '"
           << branchName << "' is not a registered branch." << endl;
   }
   TBranch *branchPtr = m_treePtr->GetBranch( branchName.c_str() );
   if (! branchPtr)
   {
      // don't know what this branch is
      return false;
   }
   int status = m_treePtr->GetBranchStatus( branchName.c_str() );
   return (1 == status);
}

//Get the TopNtupleDecoder
TopNtupleDecoder * 
TreeFileInterface::getTopDecoder() 
{
   // make sure we're prepared to decode trees
   if (!m_decodeTopTree) 
   {
      cerr << "WARNING: You must call constructor with 'decode = true'"
           <<" to use getTopDecoder()"
           << endl;
      _abort (__FILE__, __LINE__);
   }
   // make sure we've got a tree to try with
   if (!m_treePtr)
   {
      cerr << "WARNING: You must call getNextEvent() at least once "
           << "before using getTopDecoder()"
           << endl;
      _abort (__FILE__, __LINE__);
   }

   //Make sure that we've got a fresh copy
   bool verboseMode = TopNtupleDecoder::getVerboseMode();
   TopNtupleDecoder::setVerboseMode(false);  //To keep from being overwhelmed by output
   m_decoderPtr->readData(m_loadTreeFlag);
   TopNtupleDecoder::setVerboseMode(verboseMode);

   // If we're still here, we're golden.
   return m_decoderPtr;
}

TFile *
TreeFileInterface::nextFile()
{
   cerr << "TreeFileInterface::nextFile(): Do not use this function."
        << endl;
   _abort (__FILE__, __LINE__);
   return 0;
}

void
TreeFileInterface::makeCloneTree (TDirectory *outFile, Long64_t maxSize)
{
   m_makeClone = true;
   setOutputFile (outFile, maxSize);
}

void
TreeFileInterface::makeCloneTree ()
{
   if (! m_outputDirectory)
   {
      cerr << "TreeFileInterface Error: You can not call 'makeCloneTree()' "
           << "without having called 'setOutputFile()'.  Aborting."
           << endl;
      _abort (__FILE__, __LINE__);
   }
   m_makeClone = true;
}

void
TreeFileInterface::setOutputFile (TDirectory *outFile, Long64_t maxSize)
{
   if (m_outputDirectory)
   {
      cerr << "TreeFileInterface::setOutputFile(): You can only set the "
           << "output file once.  Aborting." << endl;
      _abort (__FILE__, __LINE__);
   }
   m_maxTreeSize = maxSize;
   m_outputDirectory = outFile;
}

TTree *
TreeFileInterface::getCloneTree()
{

   if (!m_cloneTreePtr) 
   {
      cerr << "TreeFileInterface::getCloneTree(): WARNING: No clone tree "
           << "is available.  Make sure you have called  "
           << "makeCloneTree() and getNextEvent() at least once." 
           << endl;
   }
   return m_cloneTreePtr;
}

bool
TreeFileInterface::fillCloneTree()
{
   // Do we have a tree to copy?
   if (!m_treePtr) 
   {
      cerr << "WARNING TreeFileInterface::fillCloneTree(): Have not opened "
           << "a tree yet.  Cannot fill the clone." << endl;
      return false;
   }

   // Have we already set up cloning?
   if (!m_cloneTreePtr) 
   {
      cerr << "WARNING TreeFileInterface::fillCloneTree(): No clone tree is"
           << " available.  Make sure you have called "
           << " makeCloneTree() and getNextEvent() at least once." << endl;
      return false;
   }

   // Great.  So far, so good.  Since we're cloning, get the WHOLE
   // entry (unless we have already gotten everything).
   if (! m_getEntireTree)
   {
      // We haven't gotten it yet, so get it now
      Int_t nbytes = 0;
      if ( (m_currentEntry - 1 >= 0) &&
           (m_currentEntry - 1 < m_entriesThisFile)) 
      {
         //Load all of the branches for thorough cloning
         nbytes = m_treePtr->GetEntry(m_currentEntry - 1, 1);
      } else {
         //This should never happen
         cerr << "ERROR TreeFileInterface::fillCloneTree(): Trying to "
              << "load an event outside the allowed range.  "
              << "Cannot fill clone tree."
              << endl;
         return false;
      }
      
      // Did we read anything?
      if (nbytes <= 0) 
      {
         cerr << "WARNING TreeFileInterface::fillCloneTree(): Cannot read data"
              << " from tree.  Will not fill clone tree."
              << endl;
         return false;
      }
   } // if ! m_getEntireTree

   // Fill this puppy!
   m_cloneTreePtr->Fill();
   return true;
}

bool
TreeFileInterface::writeFinalCloneTree()
{
   // Have we already set up cloning?
   if ((!m_makeClone || !m_cloneTreePtr) &&
       ! m_objPtrVec.size() && ! m_treePtrMap.size( ) )
   {
      cerr << "WARNING TreeFileInterface::writeCloneTree(): No clone tree is"
           << " available.  Make sure you have called "
           << " makeCloneTree() and getNextEvent() at least once." << endl;
      return false;
   }

   if (m_wroteCloneTree)
   {
      cerr << "WARNING TreeFileInterface::writeCloneTree(): Already wrote "
           << "the tree" << endl;
      return false;
   }

   m_wroteCloneTree = true;
   TDirectory *lastFilePtr = m_outputDirectory;
   int bytesWritten = 0;
   if (m_cloneTreePtr)
   {
      lastFilePtr = m_cloneTreePtr->GetCurrentFile();
      lastFilePtr->cd();
      bytesWritten += m_cloneTreePtr->Write();
   }
   for (NameVoidPtrMapIter iter = m_treePtrMap.begin();
        m_treePtrMap.end() != iter;
        ++iter)
   {
      TTree *treePtr = reinterpret_cast <TTree*> (iter->second);
      lastFilePtr = treePtr->GetCurrentFile();
      lastFilePtr->cd();
      bytesWritten += treePtr->Write();
   }
   for (TObjPtrVecIter iter = m_objPtrVec.begin();
        m_objPtrVec.end() != iter;
        ++iter)
   {
      bytesWritten += (*iter)->Write();
   } // for iter
   // This supposedly is safe now
   lastFilePtr->Close();
   if (bytesWritten) 
   {
      cout << "TreeFileInterface::writeCloneTree(): Successful." << endl;
      return true;
   } else {
      cerr << "Error TreeFileInterface::writeCloneTree(): Writing failed." 
           << endl;
      return false;
   }
}

void 
TreeFileInterface::addTObjectPtrToWrite (TObject *tobjPtr)
{
   if (!m_makeClone) 
   {
      cerr << "WARNING TreeFileInterface::addTObjectPtrToWrite(): "
           << " Must be called after makeCloneTree()." << endl;
      _abort (__FILE__, __LINE__);
   }  
   m_objPtrVec.push_back (tobjPtr);
}

void
TreeFileInterface::addActiveBranch (string branchName, bool noStar)
{
   m_getEntireTree = true;
   if (! noStar)
   {
      branchName += "*";
   }
   m_activeBranchesVec.push_back (branchName);
}

void
TreeFileInterface::addInactiveBranch (string branchName, bool noStar)
{
   m_getEntireTree = true;
   if (! noStar)
   {
      branchName += "*";
   }
   m_inactiveBranchesVec.push_back (branchName);
}

void 
TreeFileInterface::addCloneTreeBranch (const string &branchName, 
                                       void *simplePtr,
                                       const string &branchDescription)
{
   if (! m_makeClone)
   {
      cerr << "TreeFileInterface::addCloneTreeBranch() Error: "
           << "You must initialize CloneTree first with 'makeCloneTree()'."
           << endl;
      _abort (__FILE__, __LINE__);
   }
   m_CTbranchAddressMap[branchName] = simplePtr;
   m_CTbranchDescriptionMap[branchName] = branchDescription;
}

void 
TreeFileInterface::addClassCloneTreeBranch (const string &branchName, 
                                            void *classAddPtr,
                                            const string &branchDescription)
{
   if (! m_makeClone)
   {
      cerr << "TreeFileInterface::addClassCloneTreeBranch() Error: "
           << "You must initialize CloneTree first with 'makeCloneTree()'."
           << endl;
      _abort (__FILE__, __LINE__);
   }
   m_CTbranchPtrAddressMap[branchName] = classAddPtr;
   m_CTbranchDescriptionMap[branchName] = branchDescription;
}

void 
TreeFileInterface::addTCACloneTreeBranch (const string &branchName, 
                                          void *tcaAddPtr)
{
   if (! m_makeClone)
   {
      cerr << "TreeFileInterface::addTCACloneTreeBranch() Error: "
           << "You must initialize CloneTree first with 'makeCloneTree()'."
           << endl;
      _abort (__FILE__, __LINE__);
   }
   m_CTbranchPtrAddressMap[branchName] = tcaAddPtr;   
}

TTree*
TreeFileInterface::addTTree (const string &treename, 
                             const string &description)
{
   if (! m_outputDirectory)
   {
      cerr << "TreeFileInterface Error: You can not call 'addTTree()' "
           << "without having called 'setOutputFile()'.  Aborting."
           << endl;
      _abort (__FILE__, __LINE__);
   }
   m_outputDirectory->cd();
   TTree *treePtr = new TTree (treename.c_str(), description.c_str());
   addTTree (treename, treePtr);
   return treePtr;
}

TTree*
TreeFileInterface::addTTree (const string &treename, TTree *treePtr)
{
   if (! m_outputDirectory)
   {
      cerr << "TreeFileInterface Error: You can not call 'addTTree()' "
           << "without having called 'setOutputFile()'.  Aborting."
           << endl;
      _abort (__FILE__, __LINE__);
   }
   m_treePtrMap [treename] = (void*) treePtr;
   return treePtr;
}

TTree*
TreeFileInterface::getTTreeAddress (const string &treename)
{
   NameVoidPtrMapIter iter = m_treePtrMap.find (treename);
   if (m_treePtrMap.end() == iter)
   {
      cerr << "TreeFileInterface::getTTreeAddress(): Tree '"
           << treename << "' not found.";
      return 0;
   }
   TTree *treePtr = reinterpret_cast <TTree*> (iter->second);
   return treePtr;
}

TClonesArray*
TreeFileInterface::addTcaBranchObject (const string &branchName,
                                       TClonesArray *ptr)
{
   m_tcaPtrMap [branchName] = ptr;
   return ptr;
}

TClonesArray*
TreeFileInterface::addTcaBranchObject (const string &branchName,
                                       const string &className,
                                       int size)
{
   TClonesArray *ptr = new TClonesArray (className.c_str(), size);
   addTcaBranchObject (branchName, ptr);
   return ptr;
}

TClonesArray*
TreeFileInterface::tcaBranchObjectAddress (const string &branchName)
{
   TClonesPtrMapIter iter = m_tcaPtrMap.find (branchName);
   if (m_tcaPtrMap.end() == iter)
   {
      // Uh oh
      cerr << "TreeFileInterface::tcaBranchObjectAddress(): '" 
           << branchName
           << "' does not exist." << endl;
      _abort (__FILE__, __LINE__);
      return 0;
   }
   return iter->second;
}

bool
TreeFileInterface::addBranchToTree (const string &treeName,
                                    const string &branchName,
                                    string saveAsName)
{
   if (! saveAsName.length() )
   {
      saveAsName = branchName;
   }
   // get the tree pointer
   NameVoidPtrMapIter treeIter = m_treePtrMap.find (treeName);
   if (m_treePtrMap.end() == treeIter)
   {
      cerr << "TreeFileInterface::addBranchToTree() error: '"
           << treeName << "' not found." << endl;
      return false;
   } // didn't find tree
   TTree *treePtr = reinterpret_cast <TTree*> (treeIter->second);

   //////////////////////
   // Search TCA first //
   //////////////////////
   TClonesPtrMapIter tcaIter = m_tcaPtrMap.find (branchName);
   if (m_tcaPtrMap.end() != tcaIter)
   {
      // we've got a TCA branch with the same name
      treePtr->Branch (saveAsName.c_str(),
                       "TClonesArray",
                       tcaIter->second);
      return true;
   } // Found tca branch

   /////////////////////////
   // Next search classes //
   /////////////////////////
   NameVoidPtrMapIter classIter = m_classPtrMap.find (branchName);
   if (m_classPtrMap.end() != classIter)
   {
      // we've found a class branch with the same name
      treePtr->Branch (saveAsName.c_str(),
                       m_rttiMap[branchName].c_str(),
                       classIter->second);
      return true;
   }

   ////////////////////////////////////
   // Search the simple objects last //
   ////////////////////////////////////
   NameVoidPtrMapIter objectIter = m_voidPtrMap.find (branchName);
   if (m_voidPtrMap.end() != objectIter)
   {
      // we've found a object branch with the same name
      // Do we have the necessary information?
      NameStringMapIter simpleIter = m_simplePostfixMap.find (branchName);
      if (m_simplePostfixMap.end() == simpleIter)
      {
         cerr << "TreeFileInterface::addBranchToTree(): "
              << "Found object branch '" << branchName 
              << "', but did not find any PostFix info." << endl;
         return false;
      } // didn't find postfix info
      treePtr->Branch (saveAsName.c_str(),
                       objectIter->second,
                       simpleIter->second.c_str());
      return true;
   }

   // If we're still here, we were not able to match this to any
   // branch we know about.
   cerr << "TreeFileInterface::addBranchToTree(): Did not find branch '"
        << branchName << "'." << endl;   
   return false;
}

bool
TreeFileInterface::_isInputBranchSet (const string& branchname) const
{
   SSetIter iter = m_inputBranchesSet.find (branchname);
   return (m_inputBranchesSet.end() != iter);
}

void
TreeFileInterface::_addToInputBranchSet (const string& branchname)
{
   m_inputBranchesSet.insert (branchname);
}

void
TreeFileInterface::_removeFromInputBranchSet (const string &branchname)
{
   SSetIter iter = m_inputBranchesSet.find (branchname);
   // did we find it?
   if (m_inputBranchesSet.end() != iter)
   {
      // delete it
      m_inputBranchesSet.erase (iter);
   }
}

////////////////////////////////
// Protected Member Functions //
////////////////////////////////

void
TreeFileInterface::_abort (const string &codefilename, int linenum)
{
   cerr << endl << "Abort called from " << codefilename 
        << "(" << linenum << ")" << endl;
   if (m_currentFilePtr)
   {
      cerr << "Current file: " << filename() << " event "
           << m_eventsSeen - m_previousEventsSeen << endl;
   } else {
      cerr << "No file currently open." << endl;
   }
   abort();
}


bool
TreeFileInterface::_internalGetNextEvent (bool calledInternally)
{
   // we only want to set this to false if somebody is calling
   // 'getNextEvent().'  If this is being called internally, then we
   // do not want to change the value of this function.
   if (! calledInternally)
   {
      m_newFile = false;
   } 
   // If we want 1 in every N events AND this isn't being called
   // internally, skip N - 1 events
   if (m_1inEveryN && ! calledInternally)
   {
      skipNevents (m_1inEveryN - 1);
   }
   // have we seen everything we want to?
   if ( (m_eventsWanted > 0) && 
        (m_eventsWanted <= m_totalEventsPreviousFiles + m_currentEntry) )
   {
      m_allDone = true;      
   }
   // are we all done?
   if (m_allDone)
   {
      return false;
   }
   // do we need a new file?
   if (! m_currentFilePtr || 
       ( m_currentEntry >= m_entriesThisFile))
   {
      if ( ! _hookupNextFile() )
      {
         // There are no more files to hook up.  We're done
         return false;
      }
   } // if need new file

   if (! calledInternally)
   {
      ++m_entriesSeen;
      ++m_entriesSeenThisFile;
   }

   // If we're here then we have a tree and it's ready to go
   m_loadTreeFlag = m_treePtr->LoadTree (m_currentEntry++);

   if (m_loadTreeFlag < 0)
   {
      // This should never happen.
      cerr << "TreeFileInterface::getNextEvent(): LoadTree failed.  "
           << "Trying next file." << endl;
      m_currentFilePtr = 0;
      m_treePtr = 0;
      return getNextEvent();
   }
   // Should we get the whole tree?
   if (m_getEntireTree)
   {
      m_treePtr->GetEntry (m_loadTreeFlag);
   }
   else
   {
      // Mark all branches as "unretrieved"
      for (NameBoolMapIter iter = m_retrievedBranchMap.begin();
           iter != m_retrievedBranchMap.end();
           ++iter)
      {
         iter->second = false;
      } // for iter
   }
   ++m_eventsSeen;
   return true;
}

bool
TreeFileInterface::_getTreeFromFile()
{
   // make sure we've got a file
   if (! m_currentFilePtr)
   {
      // This shouldn't happen
      cerr << "TreeFileInterface::_getTreeFromFile(): No current file"
           << endl;
      return false;
   }
   m_treePtr = (TTree*) m_currentFilePtr->Get (m_treename);
   if (! m_treePtr)
   {
      cerr << "Warning: '" << m_currentFilePtr->GetName() 
           << "' did not contain a '" << m_treename << "'." << endl;
      return false;
   }

   // If we're going to decode the TopNtuple, lets do it now
   if (m_decodeTopTree) 
   {
      if (m_decoderPtr)
      {
         delete m_decoderPtr;
      }
      m_decoderPtr = new TopNtupleDecoder(m_treePtr);
   }

   // If we're still here, we're golden
   return true;
}

void
TreeFileInterface::_hookUpBranches()
{
   // TClonesArrays
   for (TClonesPtrMapIter iter = m_tcaPtrMap.begin(); 
        iter != m_tcaPtrMap.end();
        ++iter)
   {         
      if ( ! _isInputBranchSet (iter->first) )
      {
         // this branch is not an input branch
         continue;
      }
      TBranch *branchPtr = m_treePtr->GetBranch( iter->first.c_str() );
      if (branchPtr)
      {
         branchPtr->SetAutoDelete(false);
      } else {
         cerr << "TreeFileInterface:: Warning: Can not find branch '" 
              << iter->first << "' in tree '" << m_treename << "' in file '"
              << filename() << "'." << endl;
      }
      m_treePtr->SetBranchAddress( iter->first.c_str(), &(iter->second) );
   } // for iter

   // For our templated friends, here's the deal:
   // For basic objects (e.g., int, double), you tell Root about the
   // address of the object.  For "complicated" classes, however, you
   // need to hook up the address of the POINTER of the object.  My
   // solution to this was to simply have two types of templated
   // functions: regular and 'Class'.

   // void
   for (NameVoidPtrMapIter iter = m_voidPtrMap.begin(); 
        iter != m_voidPtrMap.end();
        ++iter)
   {         
      if ( ! _isInputBranchSet (iter->first) )
      {
         // this branch is not an input branch
         continue;
      }
      // Give Root the address of the object
      m_treePtr->SetBranchAddress( iter->first.c_str(), iter->second );
   } // for iter

   // class
   for (NameVoidPtrMapIter iter = m_classPtrMap.begin(); 
        iter != m_classPtrMap.end();
        ++iter)
   {         
      if ( ! _isInputBranchSet (iter->first) )
      {
         // this branch is not an input branch
         continue;
      }
      // Give Root the address of the POINTER of the object
      m_treePtr->SetBranchAddress( iter->first.c_str(), &(iter->second) );
   } // for iter

   // Check for consistency between 'addBranch', m_activeBranches, and
   // m_inactiveBranches
   if (m_firstFile)
   {
      _checkConsistency();
   }

   // Do we need to tell this tree that only some of the branches are
   // active?
   if (m_activeBranchesVec.size())
   {
      m_treePtr->SetBranchStatus ("*", 0);
      for (SVecIter iter = m_activeBranchesVec.begin();
           iter != m_activeBranchesVec.end();
           ++iter)
      {
         m_treePtr->SetBranchStatus (iter->c_str(), 1);
      } // for iter
   } // if activeBranchesVec

   // Do we need to tell this tree that only some of the branches are
   // INactive?
   if (m_inactiveBranchesVec.size())
   {
      m_treePtr->SetBranchStatus ("*", 1);
      for (SVecIter iter = m_inactiveBranchesVec.begin();
           iter != m_inactiveBranchesVec.end();
           ++iter)
      {
         m_treePtr->SetBranchStatus (iter->c_str(), 0);
      } // for iter
   } // if activeBranchesVec
}

void
TreeFileInterface::_checkConsistency()
{
   //cout << "In _checkConsistency" << endl;
   // loop over all branches specifically asked for
   for (NameBoolMapIter mapIter = m_retrievedBranchMap.begin();
        mapIter != m_retrievedBranchMap.end();
        ++mapIter)
   {
      // We usually (but not always) add a '*' to the end of the
      // names, so we need to check for both.
      string name = mapIter->first;
      string nameStar = name + "*";
      // loop over "activeBranches"
      bool found = false;
      for (SVecIter actIter = m_activeBranchesVec.begin();
           actIter != m_activeBranchesVec.end();
           ++actIter)
      {
         if (name == *actIter || nameStar == *actIter)
         {
            found = true;
            break;
         } // if found
      } // for actIter
      // IF it is NOT found AND there were already some entries, then
      // we should add it to the active branches list.
      if (! found && m_activeBranchesVec.size())
      {
         // Shame on you
         cerr << "TreeFileInterface: Adding '" << mapIter->first
              << "' to the active branches." << endl;
         string branchName = mapIter->first + "*";
         m_activeBranchesVec.push_back( branchName );
      } // ! found

      // loop over "inactive branches"
      for (SVecIter inactIter = m_inactiveBranchesVec.begin();
           inactIter != m_inactiveBranchesVec.end();
           ++inactIter)
      {
         if (name == *inactIter || nameStar == *inactIter)
         {
            // This shouldn't be here.  Get rid of it
            m_inactiveBranchesVec.erase (inactIter);
            // Note: I am assuming that they only put the improper
            // branchname in the "inactive branches" list once.  If
            // they did it more than once, bad things will happen.
            break;
         } // if found
      } // for inactIter
   } // for mapIter

   // Now, let's make sure that you don't have both active branches
   // list and inactive branches list.
   if (m_activeBranchesVec.size() && m_inactiveBranchesVec.size())
   {
      cerr << "TreeFileInterface Error: You can not have both \"active "
           << "branches\" and \"inactive branches\" at the same time. "
           << "Aborting." << endl;
      _abort (__FILE__, __LINE__);
   } // if both active and inactive
}

void
TreeFileInterface::_hookupExtraCTBranches()
{
   // loop over the simple objects first
   for (NameVoidPtrMapIter iter = m_CTbranchAddressMap.begin();
        m_CTbranchAddressMap.end() != iter;
        ++iter)
   {
      string branchName  = iter->first;
      string description = m_CTbranchDescriptionMap[branchName];
      m_cloneTreePtr->Branch (branchName.c_str(),
                              iter->second,
                              description.c_str());
      string branchNameWild = branchName;
      branchNameWild+="*";
      m_cloneTreePtr->SetBranchStatus(branchNameWild.c_str(), 1);
   } // for iter

   // Now loop over the classes/tcas
   for (NameVoidPtrMapIter iter = m_CTbranchPtrAddressMap.begin();
        m_CTbranchPtrAddressMap.end() != iter;
        ++iter)
   {
      string branchName  = iter->first;
      // Does this item have a description?
      NameStringMapIter descIter = m_CTbranchDescriptionMap.find (branchName);
      if (m_CTbranchDescriptionMap.end() != descIter)
      {
         // This is a class instance
         string description = descIter->second;
         m_cloneTreePtr->Branch (branchName.c_str(),
                                 description.c_str(),
                                 iter->second);
      } else {
         // This is a TCA
         m_cloneTreePtr->Branch (branchName.c_str(),
                                 "TClonesArray",
                                 iter->second);
      }
      string branchNameWild = branchName;
      branchNameWild+="*";
      m_cloneTreePtr->SetBranchStatus(branchNameWild.c_str(), 1);
   } // for iter
}

void
TreeFileInterface::_refreshExtraCTBranches()
{
   // loop over the simple objects first
   for (NameVoidPtrMapIter iter = m_CTbranchAddressMap.begin();
        m_CTbranchAddressMap.end() != iter;
        ++iter)
   {
      string branchName  = iter->first;
      m_cloneTreePtr->SetBranchAddress(branchName.c_str(),iter->second);
      string branchNameWild = branchName;
      branchNameWild+="*";
      m_cloneTreePtr->SetBranchStatus(branchNameWild.c_str(), 1);
   } // for iter

   // Now loop over the classes/tcas
   for (NameVoidPtrMapIter iter = m_CTbranchPtrAddressMap.begin();
        m_CTbranchPtrAddressMap.end() != iter;
        ++iter)
   {
      string branchName  = iter->first;
      m_cloneTreePtr->SetBranchAddress(branchName.c_str(),iter->second);
      string branchNameWild = branchName;
      branchNameWild+="*";
      m_cloneTreePtr->SetBranchStatus(branchNameWild.c_str(), 1);
   } // for iter
}

bool
TreeFileInterface::_hookupNextFile()
{
   m_totalEventsPreviousFiles += m_entriesThisFile;
   if (m_currentFilePtr)
   {
      if (isVerbose())
      {
         cout << "TreeFileInterface: Finished file "
              << m_currentFilePtr->GetName()
              << " using " << m_entriesSeenThisFile;
         if (m_entriesSeenThisFile != m_entriesThisFile)
         {
            cout << " of the " << m_entriesThisFile
                 << " available";
         }
         cout << " events." << endl;
      }
      m_previousEventsSeen = m_eventsSeen;
      m_entriesSeenThisFile = 0;
   }
   m_newFile = true;
   // Let's get us a new file and tree ptr
   m_treePtr = 0;
   while (! m_treePtr)
   {
      m_currentFilePtr = RootFileCollection::nextFile();
      if (! m_currentFilePtr)
      {
         // no more files to be had
         m_allDone = true;
         return false;
      }
      // Can we get ourselves a new tree.  (Either this works, or
      // we stay in the while loop.
      if (_getTreeFromFile())
      {
         m_currentEntry = 0;
         m_entriesThisFile = (int) m_treePtr->GetEntries();
         // If this tree exists, but is empty, go on to the next
         // file.
         if (! m_entriesThisFile)
         {
            m_treePtr = 0;
            continue;
         }
         if (isVerbose())
         {
            cout << "TreeFileInterface: Opening file " 
                 << m_currentFilePtr->GetName()
                 << ".  The '" << m_treename <<"' tree containts " 
                 << m_entriesThisFile << " events."
                 << endl;
         }
         // If we're still here, we've got a tree. Hook it up.
         _hookUpBranches();
         // Send in the Clones
         if (m_makeClone) 
         {
            if (m_cloneTreePtr) 
            {
               // We've got a cloned tree, but we need to update
               // the pointers
               m_treePtr->CopyAddresses(m_cloneTreePtr);
               _refreshExtraCTBranches();
            } else {
               // We need to get our first tree pointer.  This
               // should be the first file.
               m_cloneTreePtr = m_treePtr->CloneTree(0);
               m_cloneTreePtr->SetDirectory(m_outputDirectory);
               if (m_maxTreeSize > 0) 
               {
                  m_cloneTreePtr->SetMaxTreeSize(m_maxTreeSize);
               }
               // We now have our m_cloneTreePtr.  Hook up any
               // extra branches requested.
               _hookupExtraCTBranches();
            } // else if m_cloneTreePtr
         } // if makeClone
         // If we made it this far, then we have finished our first
         // file
         m_firstFile = false;
      } // if we find our tree
   } // while ! m_treePtr
   // If we're here, then we're golden
   return true;
}

// friends
ostream& operator<< (ostream& o_stream, const TreeFileInterface &rhs)
{
   return o_stream;
} 
